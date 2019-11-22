#include "hitbox_commands.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "mp_utils.h"
#include "hitbox_debugData.h"

namespace HitboxDebugging
{
	static CHitboxDebugData DebugData;

	static void HitboxDebugClear(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Hitbox debugging requires sv_cheats to be 1.\n");
			return;
		}

		if ( DebugData.IsValid() )
		{
			DebugData.Clear();
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

		DebugData.Clear();

		for ( int index = 1; index < 3; ++index )
		{
			const char* input = g_engfuncs.pfnCmd_Argv(index);

			if ( !input || !input[0] )
			{
				ALERT(at_error, "Invalid argument for %s\n", index == 1 ? "attacker" : "victim");
				break;
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
				break;
			}

			if ( index == 1 )
			{
				DebugData.SetAttacker(player);
			}
			else
			{
				DebugData.SetVictim(player);
			}
		}

		if ( DebugData.IsValid() )
		{
			ALERT(at_console, "Set hitbox debugging attacker '%s' and victim '%s'.\n",
				MPUtils::PlayerNetName(DebugData.Attacker()),
				MPUtils::PlayerNetName(DebugData.Victim()));
		}
		else
		{
			DebugData.Clear();
			ALERT(at_console, "Hitbox debugging turned off.\n");
		}
	}

	void Initialise()
	{
		g_engfuncs.pfnAddServerCommand("hitbox_debug_set", &HitboxDebugSet);
		g_engfuncs.pfnAddServerCommand("hitbox_debug_clear", &HitboxDebugClear);
	}

	bool Enabled()
	{
		return DebugData.IsValid();
	}
}
