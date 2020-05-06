#pragma once

#include "SurfaceSoundResources.h"
#include "SurfaceDecalResources.h"
#include "standard_includes.h"

class CSoundInstance;

class CSurfaceAttributes
{
public:
	// Default values should be zero.
	struct Attributes
	{
		SurfaceSoundId hitSound;
		float hitSoundVol;
		float ricochetSoundVol;
		float ricochetChance;
		SurfaceDecalId decal;
	};

	static CSurfaceAttributes& StaticInstance();

	CSurfaceAttributes();
	const Attributes& GetAttributes(SurfaceProp surfaceProp) const;

private:
	Attributes m_Attributes[SurfaceProp__Count];
};
