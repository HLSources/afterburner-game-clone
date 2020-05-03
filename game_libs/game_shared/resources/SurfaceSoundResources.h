#pragma once

#include "BaseSoundResourceCollection.h"

enum class SurfaceSoundId
{
	HitConcrete = 0,
	HitMetal,
	HitVent,
	HitGrate,
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
	virtual void Initialise() override;
};
