#pragma once

#include "BaseSoundResourceCollection.h"

enum class WeaponSoundId
{
	BulletRicochet = 0,
	GrenadeBounce,
	Count
};

class CWeaponSoundResources : public CBaseResourceCollection<WeaponSoundId, static_cast<size_t>(WeaponSoundId::Count)>
{
public:
	CWeaponSoundResources();
};
