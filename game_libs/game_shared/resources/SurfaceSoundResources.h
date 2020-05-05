#pragma once

#include "BaseSoundResourceCollection.h"

enum class SurfaceSoundId
{
	HitNone = 0,
	HitConcrete,
	HitMetal,
	HitVentDuct,
	HitMetalGrate,
	HitGlassA,
	HitGlassB,
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
	virtual void Initialise() override;
};
