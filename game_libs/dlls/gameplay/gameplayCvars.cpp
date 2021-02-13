#include "gameplay/gameplayCvars.h"
#include "standard_includes.h"

namespace GameplayCvars
{
	// By default this is slightly lower than the max movement speed of the player,
	// to avoid jitter at the top end of the velocity.
	cvar_t sv_weapon_inaccuracy_maxspeed = { "sv_weapon_inaccuracy_maxspeed", "275" };
	cvar_t sv_weapon_inaccuracy_maxfallspeed = { "sv_weapon_inaccuracy_maxfallspeed", "400" };

	void RegisterCvars()
	{
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxspeed);
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxfallspeed);
	}
}
