#pragma once

#include "BaseSoundResourceCollection.h"
#include "utlvector.h"

typedef enum _SurfaceProp SurfaceProp;

enum class SurfaceSoundId
{
	HitNone = 0,
	HitConcrete,
	HitMetal,
	HitVentDuct,
	HitMetalGrate,
	HitGlass,
	HitFlesh,
	HitFleshCritical,
	HitSnow,
	HitWood,
	HitTile,
	HitWater,
	Count
};

class CSurfaceSoundResources : public CBaseSoundResourceCollection<SurfaceSoundId, static_cast<size_t>(SurfaceSoundId::Count)>
{
public:
	CSurfaceSoundResources();

	virtual void Initialise() override;

	SurfaceSoundId SurfaceSoundForSurfaceProp(SurfaceProp surfaceProp);

private:
	using BaseClass = CBaseSoundResourceCollection<SurfaceSoundId, static_cast<size_t>(SurfaceSoundId::Count)>;

	void InitialiseSurfacePropMappings();

	// To avoid having to pull in the mangled header dependency hell that the HLSDK has
	// just to get SurfaceProp__Count, we use a vector instead.
	CUtlVector<SurfaceSoundId> m_SoundForSurfaceProp;
};
