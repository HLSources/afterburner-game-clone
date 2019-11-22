#include "hitbox_commands.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "mp_utils.h"

namespace HitboxDebugging
{
	// TODO: Make this into a proper class.
	bool IsEnabled = false;
	static EHANDLE Attacker;
	static EHANDLE Victim;

	static void HitboxDebugClear(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Hitbox debugging requires sv_cheats to be 1.\n");
			return;
		}

		if ( IsEnabled )
		{
			Attacker.Set(nullptr);
			Victim.Set(nullptr);
			IsEnabled = false;
		}

		ALERT(at_console, "Hitbox debugging turned off.\n");
	}

	static void HitboxDebugSet(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Hitbox debugging requires sv_cheats to be 1.\n");
			return;
		}

		if ( !g_pGameRules || !g_pGameRules->IsMultiplayer() )
		{
			ALERT(at_error, "Hitbox debugging can only be performed in a multiplayer game.\n");
			return;
		}

		int argc = g_engfuncs.pfnCmd_Argc();

		if ( argc != 3 )
		{
			ALERT(at_console, "Usage: hitbox_debug_set <#attackerId|attackerName> <#victimId|victimName>\n");
			return;
		}

		IsEnabled = false;

		for ( int index = 1; index < 3; ++index )
		{
			const char* input = g_engfuncs.pfnCmd_Argv(index);
			EHANDLE& handle = index == 1 ? Attacker : Victim;

			if ( !input || !input[0] )
			{
				ALERT(at_error, "Invalid argument for %s\n", index == 1 ? "attacker" : "victim");
				Attacker = nullptr;
				Victim = nullptr;
				return;
			}

			CBasePlayer* player = nullptr;

			if ( input[0] == '#' && input[1] != '\0' && isdigit(input[1]) )
			{
				++input;
				player = MPUtils::CBasePlayerFromUserId(atoi(input));
			}
			else
			{
				player = MPUtils::CBasePlayerFromNetName(input);
			}

			if ( !player )
			{
				ALERT(at_error, "Could not find %s player '%s'.\n", index == 1 ? "attacker" : "victim", input);
				Attacker = nullptr;
				Victim = nullptr;
				return;
			}

			handle = static_cast<CBaseEntity*>(player);
		}

		IsEnabled = true;

		ALERT(at_console, "Set hitbox debugging attacker '%s' and victim '%s'.\n",
			MPUtils::PlayerNetName(Attacker.EntityCast<CBasePlayer>()),
			MPUtils::PlayerNetName(Victim.EntityCast<CBasePlayer>()));
	}

	void Initialise()
	{
		g_engfuncs.pfnAddServerCommand("hitbox_debug_set", &HitboxDebugSet);
		g_engfuncs.pfnAddServerCommand("hitbox_debug_clear", &HitboxDebugClear);
	}

	bool Enabled()
	{
		return IsEnabled;
	}
}
