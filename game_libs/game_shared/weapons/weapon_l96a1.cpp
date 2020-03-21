#include "weapon_l96a1.h"
#include "weaponregistry.h"
#include "weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_l96a1_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_l96a1, CWeaponL96A1)

const WeaponAtts::WACollection& CWeaponL96A1::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponL96A1>();
}

#ifndef CLIENT_DLL
float CWeaponL96A1::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponPref_L96A1) / static_cast<float>(WeaponPref_Max);
}

void CWeaponL96A1::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(70);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 0.7f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponL96A1>()
	{
		return ::StaticWeaponAttributes;
	}
}

class CAmmol96A1 : public CGenericAmmo
{
public:
	CAmmol96A1() : CGenericAmmo("models/weapon_l96a1/w_ammo_l96a1.mdl", Ammo_L96A1)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_l96a1, CAmmol96A1)
LINK_ENTITY_TO_CLASS(ammo_sniper, CAmmol96A1)	// For backwards-compatibility.
