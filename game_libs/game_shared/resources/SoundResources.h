#pragma once

#include "ItemSoundResources.h"
#include "WeaponSoundResources.h"

namespace SoundResources
{
	extern CItemSoundResources ItemSounds;
	extern CWeaponSoundResources WeaponSounds;

	void Initialise();
	void Precache();
}
