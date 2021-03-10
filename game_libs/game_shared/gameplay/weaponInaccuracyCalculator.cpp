#include "standard_includes.h"
#include "entity_state.h"
#include "cvardef.h"
#include "gameplay/weaponInaccuracyCalculator.h"
#include "util/extramath.h"
#include "util/cvarFuncs.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

bool CWeaponInaccuracyCalculator::m_StandardCvarsLoaded = false;
bool CWeaponInaccuracyCalculator::m_DebuggingCvarsLoaded = false;
cvar_t* CWeaponInaccuracyCalculator::m_CvarMaxSpeed = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarMaxFallSpeed = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarCheats = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarEnableDebugging = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugRestValue = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugRestSpread = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugRunValue = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugRunSpread = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugCrouchShift = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugFallShift = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugFollowCoefficient = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugFireImpulse = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugFireImpulseCeiling = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarDebugFireImpulseHoldTime = nullptr;
WeaponAtts::AccuracyParameters CWeaponInaccuracyCalculator::m_StaticDebugParams;

CWeaponInaccuracyCalculator::CWeaponInaccuracyCalculator()
{
	Clear();
	InitCvars();
}

float CWeaponInaccuracyCalculator::InstantaneousInaccuracy() const
{
	return m_InstantaneousInaccuracy;
}

float CWeaponInaccuracyCalculator::SmoothedInaccuracy() const
{
	return m_SmoothedInaccuracy;
}

void CWeaponInaccuracyCalculator::SetLastSmoothedInaccuracy(float inaccuracy)
{
	m_LastSmoothedInaccuracy = inaccuracy;
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

void CWeaponInaccuracyCalculator::SetLastFireTime(float time)
{
	m_LastFireTime = time;
}

void CWeaponInaccuracyCalculator::SetLastFireTimeIsDecremented(bool decremented)
{
	m_LastFireTimeIsDecremented = decremented;
}

bool CWeaponInaccuracyCalculator::IsValid() const
{
	return m_Player && m_StandardCvarsLoaded;
}

void CWeaponInaccuracyCalculator::Clear()
{
	m_InstantaneousInaccuracy = 0.0f;
	m_SmoothedInaccuracy = 0.0f;
	m_LastSmoothedInaccuracy = 0.0f;
	m_FiredThisFrame = false;
	m_LastFireTime = 0.0f;
	m_LastFireTimeIsDecremented = false;
	m_AccuracyParams = nullptr;
	m_Player = nullptr;
}

void CWeaponInaccuracyCalculator::CalculateInaccuracy()
{
	const WeaponAtts::AccuracyParameters* params = GetInternalParameters();
	CalculateInstantaneousInaccuracy(params);
	CalculateSmoothedInaccuracy(params);
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

	const WeaponAtts::AccuracyParameters* params = GetInternalParameters();

	if ( !params )
	{
		return;
	}

	m_SmoothedInaccuracy += params->FireImpulse;

	const float maxInaccuracy = m_InstantaneousInaccuracy + params->FireImpulseCeiling;

	if ( m_SmoothedInaccuracy > maxInaccuracy )
	{
		m_SmoothedInaccuracy = maxInaccuracy;
	}
}

void CWeaponInaccuracyCalculator::CalculateInstantaneousInaccuracy(const WeaponAtts::AccuracyParameters* params)
{
	m_InstantaneousInaccuracy = 0.0f;

	if ( !IsValid() || !params )
	{
		return;
	}

	if ( (params->RunSpread - params->RestSpread).Length() < 0.001f )
	{
		// No spread difference, don't bother.
		return;
	}

	const float maxPlayerSpeed = m_CvarMaxSpeed->value;
	float spreadValue = ExtraMath::RemapLinear(m_Player->pev->velocity.Length2D(),
											   0.0f,
											   maxPlayerSpeed,
											   params->RestValue,
											   params->RunValue);

	if ( m_Player->pev->button & IN_DUCK )
	{
		spreadValue += params->CrouchShift;
	}

	const float zSpeed = fabs(m_Player->pev->velocity.z);
	const float maxZSpeed = m_CvarMaxFallSpeed->value;
	const float shiftFromZSpeed = ExtraMath::RemapSqrt(zSpeed, 0.0f, maxZSpeed, 0, params->FallShift);

	spreadValue += shiftFromZSpeed;
	m_InstantaneousInaccuracy = ExtraMath::Clamp(0.0f, spreadValue, 1.0f);
}

void CWeaponInaccuracyCalculator::CalculateSmoothedInaccuracy(const WeaponAtts::AccuracyParameters* params)
{
	m_SmoothedInaccuracy = m_InstantaneousInaccuracy;

	if ( !IsValid() || !params )
	{
		return;
	}

	const float difference = m_InstantaneousInaccuracy - m_LastSmoothedInaccuracy;
	float smoothed = m_LastSmoothedInaccuracy;

	// Move towards the new inaccuracy only if it's greater than the last, or the hold time has passed.
	if ( difference > 0.0f || !LastFireTimeIsInHoldRegion(params->FireImpulseHoldTime) )
	{
		smoothed += params->FollowCoefficient * difference;
	}

	m_SmoothedInaccuracy = ExtraMath::Clamp(0.0f, smoothed, 1.0f);
}

bool CWeaponInaccuracyCalculator::LastFireTimeIsInHoldRegion(float holdTime) const
{
	if ( !IsValid() )
	{
		return false;
	}

	const float baseTime = m_LastFireTimeIsDecremented ? 0.0f : gpGlobals->time;
	return (baseTime - m_LastFireTime) <= holdTime;
}

const WeaponAtts::AccuracyParameters* CWeaponInaccuracyCalculator::GetInternalParameters() const
{
	const WeaponAtts::AccuracyParameters* params = m_AccuracyParams;

	if ( m_CvarCheats->value && m_CvarEnableDebugging->value && m_DebuggingCvarsLoaded )
	{
		const bool gotValues = GetValuesFromDebugCvars(m_StaticDebugParams);
		params = gotValues ? &m_StaticDebugParams : nullptr;
	}

	return params;
}

void CWeaponInaccuracyCalculator::InitCvars()
{
	if ( m_StandardCvarsLoaded && m_DebuggingCvarsLoaded )
	{
		return;
	}

	m_StandardCvarsLoaded = false;
	m_DebuggingCvarsLoaded = false;

	m_CvarMaxSpeed = GetCvarByName("sv_weapon_inaccuracy_maxspeed");
	m_CvarMaxFallSpeed = GetCvarByName("sv_weapon_inaccuracy_maxfallspeed");
	m_CvarCheats = GetCvarByName("sv_cheats");

	m_CvarEnableDebugging = GetCvarByName("sv_weapon_debug_inac_enabled");
	m_CvarDebugRestValue = GetCvarByName("sv_weapon_debug_inac_restvalue");
	m_CvarDebugRestSpread = GetCvarByName("sv_weapon_debug_inac_restspread");
	m_CvarDebugRunValue = GetCvarByName("sv_weapon_debug_inac_runvalue");
	m_CvarDebugRunSpread = GetCvarByName("sv_weapon_debug_inac_runvalue");
	m_CvarDebugCrouchShift = GetCvarByName("sv_weapon_debug_inac_crouchshift");
	m_CvarDebugFallShift = GetCvarByName("sv_weapon_debug_inac_fallshift");
	m_CvarDebugFollowCoefficient = GetCvarByName("sv_weapon_debug_inac_followcoeff");
	m_CvarDebugFireImpulse = GetCvarByName("sv_weapon_debug_inac_fireimpulse");
	m_CvarDebugFireImpulseCeiling = GetCvarByName("sv_weapon_debug_inac_fireimpulseceil");
	m_CvarDebugFireImpulseHoldTime = GetCvarByName("sv_weapon_debug_inac_fireimpulsehold");

	m_StandardCvarsLoaded =
		m_CvarMaxSpeed &&
		m_CvarMaxFallSpeed &&
		m_CvarCheats;

	m_DebuggingCvarsLoaded =
		m_CvarCheats &&
		m_CvarEnableDebugging &&
		m_CvarDebugRestValue &&
		m_CvarDebugRestSpread &&
		m_CvarDebugRunValue &&
		m_CvarDebugRunSpread &&
		m_CvarDebugCrouchShift &&
		m_CvarDebugFallShift &&
		m_CvarDebugFollowCoefficient &&
		m_CvarDebugFireImpulse &&
		m_CvarDebugFireImpulseCeiling &&
		m_CvarDebugFireImpulseHoldTime;
}

bool CWeaponInaccuracyCalculator::GetValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params)
{
	InitCvars();

	if ( !m_DebuggingCvarsLoaded )
	{
		return false;
	}

	params.RestValue = m_CvarDebugRestValue->value;
	params.RestSpread = Vector2D(m_CvarDebugRestSpread->value, m_CvarDebugRestSpread->value);
	params.RunValue = m_CvarDebugRunValue->value;
	params.RunSpread = Vector2D(m_CvarDebugRunSpread->value, m_CvarDebugRunSpread->value);
	params.CrouchShift = m_CvarDebugCrouchShift->value;
	params.FallShift = m_CvarDebugFallShift->value;
	params.FollowCoefficient = m_CvarDebugFollowCoefficient->value;
	params.FireImpulse = m_CvarDebugFireImpulse->value;
	params.FireImpulseCeiling = m_CvarDebugFireImpulseCeiling->value;
	params.FireImpulseHoldTime = m_CvarDebugFireImpulseHoldTime->value;

	return true;
}
