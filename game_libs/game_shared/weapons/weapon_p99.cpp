#include "weapon_p99.h"
#include "weaponregistry.h"
#include "weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_p99_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_p99, CWeaponP99)
LINK_ENTITY_TO_CLASS(weapon_pp9, CWeaponP99)	// NF devs called it "PP9" for some reason.

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_kowloon, CWeaponP99)
LINK_ENTITY_TO_CLASS(weapon_mp9, CWeaponP99)
#endif

CWeaponP99::CWeaponP99()
	: CGenericHitscanWeapon(),
	  m_pAttackUnsilenced(nullptr),
	  m_pAttackSilenced(nullptr),
	  m_bSilenced(false)
{
	m_pAttackSilenced = GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(ATTACKMODE_SILENCED);
	m_pAttackUnsilenced = GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(ATTACKMODE_UNSILENCED);

	m_pPrimaryAttackMode = m_pAttackUnsilenced;
}

const WeaponAtts::WACollection& CWeaponP99::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponP99>();
}

void CWeaponP99::SecondaryAttack()
{
	const int anim = m_bSilenced ? P99_REMOVE_SILENCER : P99_ADD_SILENCER;

	// We must animate using the silenced body group in either case, so we can see the silencer.
	SendWeaponAnim(anim, P99BODY_SILENCED);

	m_bSilenced = !m_bSilenced;
	SetViewModelBody(m_bSilenced ? P99BODY_SILENCED : P99BODY_UNSILENCED);
	m_pPrimaryAttackMode = m_bSilenced ? m_pAttackSilenced : m_pAttackUnsilenced;

	DelayPendingActions(ViewModelAnimationDuration(anim));
}

bool CWeaponP99::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	const bool newSilencedState = from->iuser1 == 1;
	if ( m_bSilenced != newSilencedState )
	{
		m_bSilenced = newSilencedState;

		// The silenced state has changed, so immediately update the viewmodel body to be accurate.
		SetViewModelBody(m_bSilenced ? P99BODY_SILENCED : P99BODY_UNSILENCED, true);
	}

	return true;
}

bool CWeaponP99::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->iuser1 = m_bSilenced ? 1 : 0;
	return true;
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION	CWeaponP99::m_SaveData[] =
{
	DEFINE_FIELD(CWeaponP99, m_bSilenced, FIELD_BOOLEAN)
};

IMPLEMENT_SAVERESTORE(CWeaponP99, CGenericWeapon)

float CWeaponP99::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponPref_P99) / static_cast<float>(WeaponPref_Max);
}

void CWeaponP99::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	static constexpr float BOT_REFIRE_DELAY = 1.0f/6.0f;

	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(80);
	fightStyle.SetNextShootTime(1.0f / P99_FIRE_RATE, BOT_REFIRE_DELAY, 0.4f, 0.7f);
}
#endif

namespace WeaponAtts
{
	template<>
	const struct WACollection& StaticWeaponAttributes<CWeaponP99>()
	{
		return ::StaticWeaponAttributes;
	}
}

class CAmmoP99 : public CGenericAmmo
{
public:
	CAmmoP99() : CGenericAmmo("models/weapon_p99/w_ammo_p99.mdl", Ammo_P99)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_p99, CAmmoP99)

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_kowloon, CAmmoP99)
LINK_ENTITY_TO_CLASS(ammo_mp9, CAmmoP99)
#endif
