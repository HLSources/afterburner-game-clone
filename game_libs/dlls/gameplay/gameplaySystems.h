#pragma once

#include <memory>

class CGameplaySystemsSingleplayer;
class CGameplaySystemsMultiplayer;

namespace GameplaySystems
{
	void Create(bool isMultiplayer);
	void Destroy();
	bool Exists();
	bool IsMultiplayer();

	void NotifyServerActivated();
	void NotifyServerDeactivated();

	CGameplaySystemsSingleplayer* GetSingleplayer();
	CGameplaySystemsMultiplayer* GetMultiplayer();
}
