#include "weapon_raptor.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_raptor_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_raptor, CWeaponRaptor)

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_pdw90, CWeaponRaptor)
LINK_ENTITY_TO_CLASS(weapon_l96a1, CWeaponRaptor)
#endif

CWeaponRaptor::CWeaponRaptor() :
	CGenericHitscanWeapon()
{
	m_pPrimaryAttackMode = GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(ATTACKMODE_NORMAL);
}

const WeaponAtts::WACollection& CWeaponRaptor::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponRaptor>();
}

#ifndef CLIENT_DLL
float CWeaponRaptor::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponPref_P99) / static_cast<float>(WeaponPref_Max);
}

void CWeaponRaptor::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	static constexpr float BOT_REFIRE_DELAY = 0.2f;

	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(80);
	fightStyle.SetNextShootTime(1.0f / RAPTOR_FIRE_RATE, BOT_REFIRE_DELAY, 0.2f, 1.0f);
}
#endif

namespace WeaponAtts
{
	template<>
	const struct WACollection& StaticWeaponAttributes<CWeaponRaptor>()
	{
		return ::StaticWeaponAttributes;
	}
}

class CAmmoRaptor : public CGenericAmmo
{
public:
	CAmmoRaptor()
		: CGenericAmmo("models/weapon_raptor/w_ammo_raptor.mdl", Ammo_Raptor)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_raptor, CAmmoRaptor)

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_pdw90, CAmmoRaptor)
LINK_ENTITY_TO_CLASS(ammo_sniper, CAmmoRaptor)
#endif
