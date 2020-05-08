#pragma once

#include "ItemSoundResources.h"
#include "WeaponSoundResources.h"
#include "SurfaceSoundResources.h"
#include "FootstepSoundResources.h"

namespace SoundResources
{
	extern CItemSoundResources ItemSounds;
	extern CWeaponSoundResources WeaponSounds;
	extern CSurfaceSoundResources SurfaceSounds;
	extern CFootstepSoundResources FootstepSounds;

	void Precache();
}
