#include "SurfaceAttributes.h"
#include "standard_includes.h"
#include "sound/SoundInstance.h"
#include "resources/SoundResources.h"

static constexpr float RIC_LOUD = 1.0f;
static constexpr float RIC_NORM = 0.8f;

static inline void Init(CSurfaceAttributes::Attributes& atts,
						SurfaceSoundId id,
						float hitVol,
						float ricVol)
{
	atts.hitSound = id;
	atts.hitSoundVol = hitVol;
	atts.ricochetSoundVol = ricVol;
}

CSurfaceAttributes& CSurfaceAttributes::StaticInstance()
{
	static CSurfaceAttributes instance;
	return instance;
}

CSurfaceAttributes::CSurfaceAttributes()
{
	memset(m_Attributes, 0, sizeof(m_Attributes));
	Attributes* atts = nullptr;

	atts = &m_Attributes[SurfaceProp_BulletproofGlass];
	atts->hitSound = SurfaceSoundId::HitGlassA;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_LOUD;
	atts->ricochetChance = 0.85f;
	atts->decal = SurfaceDecalId::BulletproofGlass;

	atts = &m_Attributes[SurfaceProp_Carpet];
	atts->hitSound = SurfaceSoundId::HitPaper; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Carpet;

	atts = &m_Attributes[SurfaceProp_Cloth];
	atts->hitSound = SurfaceSoundId::HitPaper; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Carpet;

	atts = &m_Attributes[SurfaceProp_Computer];
	atts->hitSound = SurfaceSoundId::HitGlassB;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2f;
	atts->decal = SurfaceDecalId::Metal;

	atts = &m_Attributes[SurfaceProp_Concrete];
	atts->hitSound = SurfaceSoundId::HitConcrete;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.33f;
	atts->decal = SurfaceDecalId::Generic;

	atts = &m_Attributes[SurfaceProp_Dirt];
	atts->hitSound = SurfaceSoundId::HitSand; // FIXME: proper sound needed
	atts->hitSoundVol = 0.6f;
	atts->decal = SurfaceDecalId::Dirt;

	atts = &m_Attributes[SurfaceProp_Flesh];
	atts->hitSound = SurfaceSoundId::HitFlesh;
	atts->hitSoundVol = 0.9f;

	atts = &m_Attributes[SurfaceProp_Glass];
	atts->hitSound = SurfaceSoundId::HitGlassB;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2f;
	atts->decal = SurfaceDecalId::Glass;

	atts = &m_Attributes[SurfaceProp_Gold];
	atts->hitSound = SurfaceSoundId::HitMetal;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.5f;
	atts->decal = SurfaceDecalId::Gold;

	atts = &m_Attributes[SurfaceProp_Grass];
	atts->hitSound = SurfaceSoundId::HitSand; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Grass;

	atts = &m_Attributes[SurfaceProp_Gravel],
	atts->hitSound = SurfaceSoundId::HitSand; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Dirt;

	atts = &m_Attributes[SurfaceProp_Metal];
	atts->hitSound = SurfaceSoundId::HitMetal;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.8;
	atts->decal = SurfaceDecalId::Metal;

	atts = &m_Attributes[SurfaceProp_MetalGrate];
	atts->hitSound = SurfaceSoundId::HitMetalGrate;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2;

	atts = &m_Attributes[SurfaceProp_Paper],
	atts->hitSound = SurfaceSoundId::HitPaper;
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Plaster;

	atts = &m_Attributes[SurfaceProp_Plaster];
	atts->hitSound = SurfaceSoundId::HitPlaster;
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Plaster;

	atts = &m_Attributes[SurfaceProp_Plastic];
	atts->hitSound = SurfaceSoundId::HitPlastic;
	atts->hitSoundVol = 0.7f;
	atts->decal = SurfaceDecalId::Generic;

	atts = &m_Attributes[SurfaceProp_Rock];
	atts->hitSound = SurfaceSoundId::HitConcrete;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.4;
	atts->decal = SurfaceDecalId::Stone;

	atts = &m_Attributes[SurfaceProp_Rubber];
	atts->hitSound = SurfaceSoundId::HitPlaster;
	atts->hitSoundVol = 0.7f;
	atts->decal = SurfaceDecalId::Generic;

	atts = &m_Attributes[SurfaceProp_Sand];
	atts->hitSound = SurfaceSoundId::HitSand;
	atts->hitSoundVol = 0.7f;
	atts->decal = SurfaceDecalId::Sand;

	atts = &m_Attributes[SurfaceProp_Snow];
	atts->hitSound = SurfaceSoundId::HitSnow;
	atts->hitSoundVol = 0.6f;
	atts->decal = SurfaceDecalId::Snow;

	atts = &m_Attributes[SurfaceProp_Tile];
	atts->hitSound = SurfaceSoundId::HitTile;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.33f;
	atts->decal = SurfaceDecalId::Generic;

	atts = &m_Attributes[SurfaceProp_VentDuct];
	atts->hitSound = SurfaceSoundId::HitVentDuct;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.5f;
	atts->decal = SurfaceDecalId::Metal;

	atts = &m_Attributes[SurfaceProp_Water];
	atts->hitSound = SurfaceSoundId::HitWater;
	atts->hitSoundVol = 0.6f;

	atts = &m_Attributes[SurfaceProp_Wood];
	atts->hitSound = SurfaceSoundId::HitWood;
	atts->hitSoundVol = 0.8f;
	atts->decal = SurfaceDecalId::Wood;
}

const CSurfaceAttributes::Attributes& CSurfaceAttributes::GetAttributes(SurfaceProp surfaceProp) const
{
	if ( surfaceProp >= SurfaceProp__Count )
	{
		surfaceProp = SurfaceProp_None;
	}

	return m_Attributes[surfaceProp];
}
