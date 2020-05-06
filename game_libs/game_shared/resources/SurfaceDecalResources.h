#pragma once

#include "BaseResourceCollection.h"

enum class SurfaceDecalId
{
	None = 0,
	Generic,
	Carpet,
	Dirt,
	Glass,
	BulletproofGlass,
	Grass,
	Metal,
	Gold,
	Plaster,
	Sand,
	Scorch,
	Snow,
	Stone,
	Wood,
	Count
};

class CSurfaceDecalResources : public CBaseResourceCollection<SurfaceDecalId, static_cast<size_t>(SurfaceDecalId::Count)>
{
public:
	CSurfaceDecalResources();
};
