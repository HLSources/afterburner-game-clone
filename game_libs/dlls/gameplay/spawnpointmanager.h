#pragma once

#include <cstdint>
#include "standard_includes.h"
#include "spawnpointlist.h"
#include "gamerules.h"

class CSpawnPointManager
{
public:
	enum SpawnFlag
	{
		Flag_AvoidDeathSite = 1 << 0,
		Flag_IgnoreNearbyPlayers = 1 << 1
	};

	CSpawnPointManager(CGameRules& gameRules);

	void Initialise();

	CBaseEntity* GetNextSpawnPoint(CBasePlayer* player, uint32_t flags = 0);

private:
	enum SpawnPointType
	{
		SP_Default = 0,
		SP_Deathmatch,
		SP_CoOp,

		SP__Count
	};

	static bool SpawnPointValid(CBaseEntity* spawnPoint, CBasePlayer* player, uint32_t flags);
	static void KillPlayersAtPoint(const vec3_t& origin, CBasePlayer* player);

	CBaseEntity* GetNextCoOpSpawnPoint();
	CBaseEntity* GetNextDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags);
	CBaseEntity* GetNextDefaultSpawnPoint();
	CBaseEntity* GetNextValidDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags);

	static const char* const SpawnPointClassNames[SP__Count];

	CGameRules* m_pGameRules;
	CSpawnPointList m_Lists[SP__Count];
};
