#include "SurfacePropertySounds.h"
#include "sound/SoundInstance.h"
#include "resources/SoundResources.h"
#include "standard_includes.h"

namespace SurfacePropertySounds
{
	void GetHitSoundForSurface(SurfaceProp surfaceProp, CSoundInstance& sound)
	{
		SurfaceSoundId soundId = SoundResources::SurfaceSounds.SurfaceSoundForSurfaceProp(surfaceProp);
		const char* soundPath = SoundResources::SurfaceSounds.GetRandomSoundPath(soundId);

		sound.SetSoundPath(soundPath);
		sound.SetPitch(96, 111);
		sound.SetAttenuation(ATTN_NORM);

		switch ( surfaceProp )
		{
			case SurfaceProp_Concrete:
			case SurfaceProp_Metal:
			case SurfaceProp_MetalGrate:
			case SurfaceProp_Water:
			case SurfaceProp_Wood:
			{
				sound.SetVolume(0.9f);
				break;
			}

			case SurfaceProp_Tile:
			case SurfaceProp_Glass:
			case SurfaceProp_Computer:
			{
				sound.SetVolume(0.8f);
				break;
			}

			case SurfaceProp_VentDuct:
			{
				sound.SetVolume(0.5f);
				break;
			}

			default:
			{
				break;
			}
		}
	}
}
