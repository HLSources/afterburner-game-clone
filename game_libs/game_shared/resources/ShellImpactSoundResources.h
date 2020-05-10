#pragma once

#include "BaseResourceCollection.h"

enum class ShellImpactSoundId
{
	BrassOnHardSurface = 0,
	HollowOnHardSurface,
	SolidOnHardSurface,
	SolidOnMediumSurface,
	SolidOnMetalSurface,
	SolidOnWoodSurface,
	IntoWater,
	Count
};

class CShellImpactSoundResources : public CBaseResourceCollection<ShellImpactSoundId, static_cast<size_t>(ShellImpactSoundId::Count)>
{
public:
	CShellImpactSoundResources();
};
