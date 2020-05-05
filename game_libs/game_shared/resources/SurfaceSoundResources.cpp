#include "SurfaceSoundResources.h"
#include "standard_includes.h"

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
	"surfaceprop/hit_water1.wav"
};

CSurfaceSoundResources::CSurfaceSoundResources() :
	BaseClass()
{
	m_SoundForSurfaceProp.SetCount(SurfaceProp__Count);

	FOR_EACH_VEC(m_SoundForSurfaceProp, index)
	{
		m_SoundForSurfaceProp[index] = SurfaceSoundId::HitNone;
	}
}

void CSurfaceSoundResources::Initialise()
{
	InitialiseSounds(SurfaceSoundId::HitConcrete, HitConcrete);
	InitialiseSounds(SurfaceSoundId::HitVentDuct, HitHollowContainer);
	InitialiseSounds(SurfaceSoundId::HitFlesh, HitFlesh);
	InitialiseSounds(SurfaceSoundId::HitFleshCritical, HitFleshCritical);
	InitialiseSounds(SurfaceSoundId::HitGlass, HitGlass);
	InitialiseSounds(SurfaceSoundId::HitMetal, HitMetal);
	InitialiseSounds(SurfaceSoundId::HitMetalGrate, HitMetalGrate);
	InitialiseSounds(SurfaceSoundId::HitSnow, HitSnow);
	InitialiseSounds(SurfaceSoundId::HitWood, HitWood);
	InitialiseSounds(SurfaceSoundId::HitTile, HitTile);
	InitialiseSounds(SurfaceSoundId::HitWater, HitWater);

	InitialiseSurfacePropMappings();
}

SurfaceSoundId CSurfaceSoundResources::SurfaceSoundForSurfaceProp(SurfaceProp surfaceProp)
{
	if ( surfaceProp >= SurfaceProp__Count )
	{
		return SurfaceSoundId::HitNone;
	}

	return m_SoundForSurfaceProp[surfaceProp];
}

void CSurfaceSoundResources::InitialiseSurfacePropMappings()
{
	// Remember that these are the sound made by a given surface. Some surfaces,
	// eg. sand and snow, are similar enough to just use the same sound.
	// Entries listed with FIXME use the most appropriate existing sound effect,
	// but really should have their own.
	m_SoundForSurfaceProp[SurfaceProp_BulletproofGlass] = SurfaceSoundId::HitGlass;
	m_SoundForSurfaceProp[SurfaceProp_Carpet] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Cloth] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Computer] = SurfaceSoundId::HitGlass;
	m_SoundForSurfaceProp[SurfaceProp_Concrete] = SurfaceSoundId::HitConcrete;
	m_SoundForSurfaceProp[SurfaceProp_Dirt] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Flesh] = SurfaceSoundId::HitFlesh;
	m_SoundForSurfaceProp[SurfaceProp_Glass] = SurfaceSoundId::HitGlass;
	m_SoundForSurfaceProp[SurfaceProp_Gold] = SurfaceSoundId::HitMetal;
	m_SoundForSurfaceProp[SurfaceProp_Grass] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Gravel] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Metal] = SurfaceSoundId::HitMetal;
	m_SoundForSurfaceProp[SurfaceProp_MetalGrate] = SurfaceSoundId::HitMetalGrate;
	m_SoundForSurfaceProp[SurfaceProp_Paper] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Plaster] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Plastic] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Rock] = SurfaceSoundId::HitConcrete;
	m_SoundForSurfaceProp[SurfaceProp_Rubber] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_Sand] = SurfaceSoundId::HitSnow;
	m_SoundForSurfaceProp[SurfaceProp_Snow] = SurfaceSoundId::HitSnow;
	m_SoundForSurfaceProp[SurfaceProp_Tile] = SurfaceSoundId::HitConcrete; // FIXME
	m_SoundForSurfaceProp[SurfaceProp_VentDuct] = SurfaceSoundId::HitVentDuct;
	m_SoundForSurfaceProp[SurfaceProp_Water] = SurfaceSoundId::HitWater;
	m_SoundForSurfaceProp[SurfaceProp_Wood] = SurfaceSoundId::HitWood;
}
