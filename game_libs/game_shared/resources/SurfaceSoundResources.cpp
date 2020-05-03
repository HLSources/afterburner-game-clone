#include "SurfaceSoundResources.h"

static constexpr const char* const HitConcrete[] =
{
	"surfaceprop/hit_concrete1.wav",
	"surfaceprop/hit_concrete2.wav"
};

static constexpr const char* const HitHollowContainer[] =
{
	"surfaceprop/hit_hollowcontainer1.wav",
	"surfaceprop/hit_hollowcontainer2.wav",
	"surfaceprop/hit_hollowcontainer3.wav"
};

static constexpr const char* const HitFlesh[] =
{
	"surfaceprop/hit_flesh1.wav",
	"surfaceprop/hit_flesh2.wav",
	"surfaceprop/hit_flesh3.wav",
	"surfaceprop/hit_flesh4.wav",
	"surfaceprop/hit_flesh5.wav",
	"surfaceprop/hit_flesh6.wav",
	"surfaceprop/hit_flesh7.wav",
	"surfaceprop/hit_flesh8.wav",
};

static constexpr const char* const HitFleshCritical[] =
{
	"surfaceprop/hit_flesh_crit1.wav",
	"surfaceprop/hit_flesh_crit2.wav"
};

static constexpr const char* const HitGlass[] =
{
	"surfaceprop/hit_glass1.wav",
	"surfaceprop/hit_glass2.wav"
};

static constexpr const char* const HitMetal[] =
{
	"surfaceprop/hit_metal1.wav",
	"surfaceprop/hit_metal2.wav",
	"surfaceprop/hit_metal3.wav"
};

static constexpr const char* const HitMetalGrate[] =
{
	"surfaceprop/hit_metalgrate1.wav",
	"surfaceprop/hit_metalgrate2.wav",
	"surfaceprop/hit_metalgrate3.wav"
};

static constexpr const char* const HitSnow[] =
{
	"surfaceprop/hit_snow1.wav"
};

static constexpr const char* const HitWood[] =
{
	"surfaceprop/hit_wood1.wav",
	"surfaceprop/hit_wood2.wav",
	"surfaceprop/hit_wood3.wav",
	"surfaceprop/hit_wood4.wav"
};

static constexpr const char* const HitTile[] =
{
	"surfaceprop/hit_tile1.wav",
	"surfaceprop/hit_tile2.wav"
};

static constexpr const char* const HitWater[] =
{
	"surfaceprop/hit_water1.wav",
	"surfaceprop/hit_water2.wav"
};

void CSurfaceSoundResources::Initialise()
{
	InitialiseSounds(SurfaceSoundId::HitConcrete, HitConcrete);
	InitialiseSounds(SurfaceSoundId::HitVent, HitHollowContainer);
	InitialiseSounds(SurfaceSoundId::HitFlesh, HitFlesh);
	InitialiseSounds(SurfaceSoundId::HitFleshCritical, HitFleshCritical);
	InitialiseSounds(SurfaceSoundId::HitGlass, HitGlass);
	InitialiseSounds(SurfaceSoundId::HitMetal, HitMetal);
	InitialiseSounds(SurfaceSoundId::HitGrate, HitMetalGrate);
	InitialiseSounds(SurfaceSoundId::HitSnow, HitSnow);
	InitialiseSounds(SurfaceSoundId::HitWood, HitWood);
	InitialiseSounds(SurfaceSoundId::HitTile, HitTile);
	InitialiseSounds(SurfaceSoundId::HitWater, HitWater);
}
