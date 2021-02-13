#pragma once

#include "cvardef.h"

namespace GameplayCvars
{
	extern cvar_t sv_weapon_inaccuracy_maxspeed;
	extern cvar_t sv_weapon_inaccuracy_maxfallspeed;

	void RegisterCvars();
}
