#include "gameplay/gameplayCvars.h"
#include "standard_includes.h"
#include "utlstring.h"

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

	static void DumpValues(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "This command requires sv_cheats to be 1.\n");
			return;
		}

		CUtlString output;

		output.AppendFormat("Weapon inaccuracy values:\n");
		output.AppendFormat("  sv_weapon_debug_inac_enabled = %s\n", sv_weapon_debug_inac_enabled.value ? "true" : "false");
		output.AppendFormat("  sv_weapon_debug_inac_restvalue = %f\n", sv_weapon_debug_inac_restvalue.value);
		output.AppendFormat("  sv_weapon_debug_inac_restspread = %f\n", sv_weapon_debug_inac_restspread.value);
		output.AppendFormat("  sv_weapon_debug_inac_runvalue = %f\n", sv_weapon_debug_inac_runvalue.value);
		output.AppendFormat("  sv_weapon_debug_inac_runspread = %f\n", sv_weapon_debug_inac_runspread.value);
		output.AppendFormat("  sv_weapon_debug_inac_crouchshift = %f\n", sv_weapon_debug_inac_crouchshift.value);
		output.AppendFormat("  sv_weapon_debug_inac_fallshift = %f\n", sv_weapon_debug_inac_fallshift.value);
		output.AppendFormat("  sv_weapon_debug_inac_followcoeff = %f\n", sv_weapon_debug_inac_followcoeff.value);
		output.AppendFormat("  sv_weapon_debug_inac_fireimpulse = %f\n", sv_weapon_debug_inac_fireimpulse.value);
		output.AppendFormat("  sv_weapon_debug_inac_fireimpulseceil = %f\n", sv_weapon_debug_inac_fireimpulseceil.value);
		output.AppendFormat("  sv_weapon_debug_inac_fireimpulsehold = %f\n", sv_weapon_debug_inac_fireimpulsehold.value);

		ALERT(at_console, "%s", output.Get());
	}

	void Init()
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

		g_engfuncs.pfnAddServerCommand("sv_weapon_debug_inac_dump", &DumpValues);
	}
}
