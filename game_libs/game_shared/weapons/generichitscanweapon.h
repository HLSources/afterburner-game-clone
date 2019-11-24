#pragma once

#include "genericweapon.h"
#include "weaponatts_hitscanattack.h"

class CGenericHitscanWeapon : public CGenericWeapon
{
public:
	virtual void WeaponIdle() override;

protected:
	virtual void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode) override;
	virtual bool InvokeWithAttackMode(const CGenericWeapon::WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

private:
	Vector FireBulletsPlayer(const WeaponAtts::WAHitscanAttack& hitscanAttack,
							 const Vector& vecSrc,
							 const Vector& vecDirShooting);

#ifndef CLIENT_DLL
	void GenerateHitscanFireEvent(const Vector& start, const TraceResult& tr);
#endif

#ifdef CLIENT_DLL
	Vector FireBulletsPlayer_Client(const WeaponAtts::WAHitscanAttack& hitscanAttack);
#endif
};
