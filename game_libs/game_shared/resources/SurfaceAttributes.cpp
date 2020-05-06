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

	atts = &m_Attributes[SurfaceProp_Carpet];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Cloth];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Computer];
	atts->hitSound = SurfaceSoundId::HitGlassB;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2f;

	atts = &m_Attributes[SurfaceProp_Concrete];
	atts->hitSound = SurfaceSoundId::HitConcrete;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.33f;

	atts = &m_Attributes[SurfaceProp_Dirt];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.6f;

	atts = &m_Attributes[SurfaceProp_Flesh];
	atts->hitSound = SurfaceSoundId::HitFlesh;
	atts->hitSoundVol = 0.9f;

	atts = &m_Attributes[SurfaceProp_Glass];
	atts->hitSound = SurfaceSoundId::HitGlassB;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2f;

	atts = &m_Attributes[SurfaceProp_Gold];
	atts->hitSound = SurfaceSoundId::HitMetal;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.5f;

	atts = &m_Attributes[SurfaceProp_Grass];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Gravel],
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Metal];
	atts->hitSound = SurfaceSoundId::HitMetal;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.8;

	atts = &m_Attributes[SurfaceProp_MetalGrate];
	atts->hitSound = SurfaceSoundId::HitMetalGrate;
	atts->hitSoundVol = 0.9f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.2;

	atts = &m_Attributes[SurfaceProp_Paper],
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Plaster];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.8f;

	atts = &m_Attributes[SurfaceProp_Plastic];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.7f;

	atts = &m_Attributes[SurfaceProp_Rock];
	atts->hitSound = SurfaceSoundId::HitConcrete;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.4;

	atts = &m_Attributes[SurfaceProp_Rubber];
	atts->hitSound = SurfaceSoundId::HitConcrete; // FIXME: proper sound needed
	atts->hitSoundVol = 0.7f;

	atts = &m_Attributes[SurfaceProp_Sand];
	atts->hitSound = SurfaceSoundId::HitSand;
	atts->hitSoundVol = 0.7f;

	atts = &m_Attributes[SurfaceProp_Snow];
	atts->hitSound = SurfaceSoundId::HitSnow;
	atts->hitSoundVol = 0.6f;

	atts = &m_Attributes[SurfaceProp_Tile];
	atts->hitSound = SurfaceSoundId::HitTile;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.33f;

	atts = &m_Attributes[SurfaceProp_VentDuct];
	atts->hitSound = SurfaceSoundId::HitVentDuct;
	atts->hitSoundVol = 0.8f;
	atts->ricochetSoundVol = RIC_NORM;
	atts->ricochetChance = 0.5f;

	atts = &m_Attributes[SurfaceProp_Water];
	atts->hitSound = SurfaceSoundId::HitWater;
	atts->hitSoundVol = 0.6f;

	atts = &m_Attributes[SurfaceProp_Wood];
	atts->hitSound = SurfaceSoundId::HitWood;
	atts->hitSoundVol = 0.8f;
}

const CSurfaceAttributes::Attributes& CSurfaceAttributes::GetAttributes(SurfaceProp surfaceProp) const
{
	if ( surfaceProp >= SurfaceProp__Count )
	{
		surfaceProp = SurfaceProp_None;
	}

	return m_Attributes[surfaceProp];
}

void CSurfaceAttributes::GetHitSoundForSurface(SurfaceProp surfaceProp, CSoundInstance& sound)
{
	const CSurfaceAttributes::Attributes surfaceAtts = CSurfaceAttributes::StaticInstance().GetAttributes(surfaceProp);
	const char* soundPath = SoundResources::SurfaceSounds.RandomResourcePath(surfaceAtts.hitSound);

	sound.SetSoundPath(soundPath);
	sound.SetPitch(96, 111);
	sound.SetAttenuation(ATTN_NORM);
	sound.SetVolume(surfaceAtts.hitSoundVol);
}
