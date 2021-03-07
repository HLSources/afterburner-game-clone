#include "standard_includes.h"
#include "entity_state.h"
#include "cvardef.h"
#include "gameplay/weaponInaccuracyCalculator.h"
#include "util/extramath.h"
#include "util/cvarFuncs.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

CWeaponInaccuracyCalculator::CWeaponInaccuracyCalculator()
{
	Clear();

	m_CvarMaxSpeed = GetCvarByName("sv_weapon_inaccuracy_maxspeed");
	m_CvarMaxFallSpeed = GetCvarByName("sv_weapon_inaccuracy_maxfallspeed");
}

float CWeaponInaccuracyCalculator::InstantaneousInaccuracy() const
{
	return m_InstantaneousInaccuracy;
}

float CWeaponInaccuracyCalculator::SmoothedInaccuracy() const
{
	return m_SmoothedInaccuracy;
}

const CWeaponInaccuracyCalculator::Accuracy* CWeaponInaccuracyCalculator::AccuracyParams() const
{
	return m_AccuracyParams;
}

void CWeaponInaccuracyCalculator::SetAccuracyParams(const Accuracy* params)
{
	m_AccuracyParams = params;
}

CBasePlayer* CWeaponInaccuracyCalculator::Player() const
{
	return m_Player;
}

void CWeaponInaccuracyCalculator::SetPlayer(CBasePlayer* player)
{
	m_Player = player;
}

void CWeaponInaccuracyCalculator::SetFiredThisFrame(bool fired)
{
	m_FiredThisFrame = fired;
}

bool CWeaponInaccuracyCalculator::IsValid() const
{
	return m_Player && m_AccuracyParams && m_CvarMaxSpeed && m_CvarMaxFallSpeed;
}

void CWeaponInaccuracyCalculator::Clear()
{
	m_InstantaneousInaccuracy = 0.0f;
	m_SmoothedInaccuracy = 0.0f;
	m_LastSmoothedInaccuracy = 0.0f;
	m_FiredThisFrame = false;
	m_AccuracyParams = nullptr;
	m_Player = nullptr;
}

void CWeaponInaccuracyCalculator::CalculateInaccuracy()
{
	m_LastSmoothedInaccuracy = m_SmoothedInaccuracy;
	CalculateInstantaneousInaccuracy();
	CalculateSmoothedInaccuracy();
}

void CWeaponInaccuracyCalculator::AddInaccuracyPenaltyFromFiring()
{
	if ( !m_FiredThisFrame )
	{
		return;
	}

	m_FiredThisFrame = false;

	if ( !IsValid() )
	{
		return;
	}

	m_SmoothedInaccuracy += m_AccuracyParams->FireImpulse;

	const float maxInaccuracy = m_InstantaneousInaccuracy + m_AccuracyParams->FireImpulseCeiling;

	if ( m_SmoothedInaccuracy > maxInaccuracy )
	{
		m_SmoothedInaccuracy = maxInaccuracy;
	}
}

void CWeaponInaccuracyCalculator::CalculateInstantaneousInaccuracy()
{
	m_InstantaneousInaccuracy = 0.0f;

	if ( !IsValid() )
	{
		return;
	}

	if ( (m_AccuracyParams->RunSpread - m_AccuracyParams->RestSpread).Length() < 0.001f )
	{
		// No spread difference, don't bother.
		return;
	}

	const float maxPlayerSpeed = m_CvarMaxSpeed->value;
	float spreadValue = ExtraMath::RemapLinear(m_Player->pev->velocity.Length2D(),
											   0.0f,
											   maxPlayerSpeed,
											   m_AccuracyParams->RestValue,
											   m_AccuracyParams->RunValue);

	if ( m_Player->pev->button & IN_DUCK )
	{
		spreadValue += m_AccuracyParams->CrouchShift;
	}

	const float zSpeed = fabs(m_Player->pev->velocity.z);
	const float maxZSpeed = m_CvarMaxFallSpeed->value;
	const float shiftFromZSpeed = ExtraMath::RemapSqrt(zSpeed, 0.0f, maxZSpeed, 0, m_AccuracyParams->FallShift);

	spreadValue += shiftFromZSpeed;
	m_InstantaneousInaccuracy = ExtraMath::Clamp(0.0f, spreadValue, 1.0f);
}

void CWeaponInaccuracyCalculator::CalculateSmoothedInaccuracy()
{
	m_SmoothedInaccuracy = m_InstantaneousInaccuracy;

	if ( !IsValid() )
	{
		return;
	}

	const float difference = m_InstantaneousInaccuracy - m_LastSmoothedInaccuracy;
	const float smoothed = m_LastSmoothedInaccuracy + (m_AccuracyParams->FollowCoefficient * difference);

	m_SmoothedInaccuracy = ExtraMath::Clamp(0.0f, smoothed, 1.0f);
}

void CWeaponInaccuracyCalculator::ReadFromPredictionData(const weapon_data_t& from)
{
	m_SmoothedInaccuracy = from.m_iInaccuracy;
}

void CWeaponInaccuracyCalculator::WriteToPredictionData(weapon_data_t& to)
{
	to.m_iInaccuracy = m_SmoothedInaccuracy;
}
