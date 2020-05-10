#include "ShellImpactSoundResources.h"

static constexpr const char* const BrassShellOnHardSurface[] =
{
	"shells/brass_conc1.wav",
	"shells/brass_conc2.wav",
	"shells/brass_conc3.wav",
	"shells/brass_conc4.wav"
};

static constexpr const char* const HollowShellOnHardSurface[] =
{
	"shells/hollow_conc1.wav",
	"shells/hollow_conc2.wav",
	"shells/hollow_conc3.wav",
	"shells/hollow_conc4.wav",
	"shells/hollow_conc5.wav",
	"shells/hollow_conc6.wav"
};

static constexpr const char* const ShellOnMetalSurface[] =
{
	"shells/metal1.wav",
	"shells/metal2.wav",
	"shells/metal3.wav",
	"shells/metal4.wav"
};

static constexpr const char* const SolidShellOnConcreteSurface[] =
{
	"shells/solid_conc1.wav",
	"shells/solid_conc2.wav"
};

static constexpr const char* const SolidShellOnTileSurface[] =
{
	"shells/tile1.wav",
	"shells/tile2.wav"
};

static constexpr const char* const ShellWaterImpact[] =
{
	"shells/water1.wav",
	"shells/water2.wav"
};

static constexpr const char* const SolidShellOnWoodSurface[] =
{
	"shells/wood1.wav",
	"shells/wood2.wav"
};

CShellImpactSoundResources::CShellImpactSoundResources() :
	CBaseResourceCollection<ShellImpactSoundId, static_cast<size_t>(ShellImpactSoundId::Count)>()
{
	InitialiseResources(ShellImpactSoundId::BrassOnHardSurface, BrassShellOnHardSurface);
	InitialiseResources(ShellImpactSoundId::HollowOnHardSurface, HollowShellOnHardSurface);
	InitialiseResources(ShellImpactSoundId::IntoWater, ShellWaterImpact);
	InitialiseResources(ShellImpactSoundId::SolidOnHardSurface, SolidShellOnTileSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnMediumSurface, SolidShellOnConcreteSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnMetalSurface, ShellOnMetalSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnWoodSurface, SolidShellOnWoodSurface);
}
