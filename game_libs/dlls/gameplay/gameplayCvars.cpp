#include "gameplay/gameplayCvars.h"
#include "standard_includes.h"

namespace GameplayCvars
{
#define DEFINE_CVAR(name, value) static cvar_t name = { #name, #value };

	// By default this is slightly lower than the max movement speed of the player,
	// to avoid jitter at the top end of the velocity.
	DEFINE_CVAR(sv_weapon_inaccuracy_maxspeed, 275);
	DEFINE_CVAR(sv_weapon_inaccuracy_maxfallspeed, 400);

	DEFINE_CVAR(sv_weapon_debug_inac_enabled, 0);
	DEFINE_CVAR(sv_weapon_debug_inac_restvalue, 0.1);
	DEFINE_CVAR(sv_weapon_debug_inac_restspread, 0);
	DEFINE_CVAR(sv_weapon_debug_inac_runvalue, 0.5);
	DEFINE_CVAR(sv_weapon_debug_inac_runspread, 0);
	DEFINE_CVAR(sv_weapon_debug_inac_crouchshift, -0.08);
	DEFINE_CVAR(sv_weapon_debug_inac_fallshift, 0.2);
	DEFINE_CVAR(sv_weapon_debug_inac_followcoeff, 0.5);
	DEFINE_CVAR(sv_weapon_debug_inac_fireimpulse, 0.1);
	DEFINE_CVAR(sv_weapon_debug_inac_fireimpulseceil, 0.3);
	DEFINE_CVAR(sv_weapon_debug_inac_fireimpulsehold, 0.05);

#undef DEFINE_CVAR

	void RegisterCvars()
	{
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxspeed);
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxfallspeed);

		CVAR_REGISTER(&sv_weapon_debug_inac_enabled);
		CVAR_REGISTER(&sv_weapon_debug_inac_restvalue);
		CVAR_REGISTER(&sv_weapon_debug_inac_restspread);
		CVAR_REGISTER(&sv_weapon_debug_inac_runvalue);
		CVAR_REGISTER(&sv_weapon_debug_inac_runspread);
		CVAR_REGISTER(&sv_weapon_debug_inac_crouchshift);
		CVAR_REGISTER(&sv_weapon_debug_inac_fallshift);
		CVAR_REGISTER(&sv_weapon_debug_inac_followcoeff);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulse);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulseceil);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulsehold);
	}
}
