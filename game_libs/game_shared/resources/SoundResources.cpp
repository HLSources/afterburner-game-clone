#include "SoundResources.h"
#include "standard_includes.h"
#include "enginecallback.h"

namespace SoundResources
{
	CItemSoundResources ItemSounds;
	CWeaponSoundResources WeaponSounds;
	CSurfaceSoundResources SurfaceSounds;

	static void Precache(const IIterableResourceCollection& collection)
	{
		for ( IIterableResourceCollection::Iterator it = collection.Begin(); it.IsValid(); ++it )
		{
			PRECACHE_SOUND(it.Path());
		}
	}

	void Precache()
	{
		Precache(ItemSounds);
		Precache(WeaponSounds);
		Precache(SurfaceSounds);
	}
}
