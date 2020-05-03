#include "SoundResources.h"
#include "standard_includes.h"
#include "enginecallback.h"

namespace SoundResources
{
	CItemSoundResources ItemSounds;
	CWeaponSoundResources WeaponSounds;
	CSurfaceSoundResources SurfaceSounds;

	static void Precache(const IIterableSoundResourceCollection& collection)
	{
		for ( IIterableSoundResourceCollection::Iterator it = collection.Begin(); it.IsValid(); ++it )
		{
			PRECACHE_SOUND(it.SoundName());
		}
	}

	void Initialise()
	{
		ItemSounds.Initialise();
		WeaponSounds.Initialise();
		SurfaceSounds.Initialise();
	}

	void Precache()
	{
		Precache(ItemSounds);
		Precache(WeaponSounds);
		Precache(SurfaceSounds);
	}
}
