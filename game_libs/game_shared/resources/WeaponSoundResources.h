#pragma once

#include "BaseSoundResourceCollection.h"

enum class WeaponSoundId
{
	BulletRicochet = 0,
	GrenadeBounce,
	Count
};

class CWeaponSoundResources : public CBaseSoundResourceCollection<WeaponSoundId, static_cast<size_t>(WeaponSoundId::Count)>
{
public:
	virtual void Initialise() override;
};
