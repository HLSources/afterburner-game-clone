#include "PlayerMoveResources.h"
#include "standard_includes.h"
#include "SurfaceAttributes.h"
#include "FootstepSoundResources.h"
#include "SoundResources.h"

#ifdef __cplusplus
extern "C" {
#endif

int PMRes_GetStepSoundIdForSurface(SurfaceProp surfaceProp)
{
	CSurfaceAttributes& inst = CSurfaceAttributes::StaticInstance();
	return static_cast<int>(inst.GetAttributes(surfaceProp).footstepSound);
}

int PMRes_GetStepSoundIdForWater(int deepWater)
{
	// TODO: We need a sound for this.
	// For now, play something that is clearly wrong.
	return static_cast<int>(FootstepSoundId::Metal);
}

int PMRes_GetSwimSoundId(void)
{
	// TODO: We need a sound for this.
	// For now, play something that is clearly wrong.
	return static_cast<int>(FootstepSoundId::Metal);
}

float PMRes_GetStepSoundVolumeMultiplierForSurface(SurfaceProp surfaceProp, int walking)
{
	CSurfaceAttributes& inst = CSurfaceAttributes::StaticInstance();
	const CSurfaceAttributes::Attributes& atts = inst.GetAttributes(surfaceProp);

	return walking ? atts.footstepSoundVolWalk : atts.footstepSoundVolRun;
}

const char* PMRes_GetStepSoundPath(int stepSoundId, int isLeftFoot)
{
	FootstepSoundId id = static_cast<FootstepSoundId>(stepSoundId);
	return SoundResources::FootstepSounds.RandomFootstepSoundPath(id, isLeftFoot ? CFootstepSoundResources::Foot::Left : CFootstepSoundResources::Foot::Right);
}

#ifdef __cplusplus
}
#endif
