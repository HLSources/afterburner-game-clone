#pragma once

class CSoundInstance;
typedef enum _SurfaceProp SurfaceProp;

namespace SurfacePropertySounds
{
	void GetHitSoundForSurface(SurfaceProp surfaceProp, CSoundInstance& sound);
}
