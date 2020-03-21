#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponL96A1 : public CGenericHitscanWeapon
{
public:
	CWeaponL96A1();

	virtual const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponL96A1>();
}
