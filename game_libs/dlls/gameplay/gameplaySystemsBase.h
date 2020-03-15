#pragma once

class CGameplaySystemsBase
{
public:
	virtual ~CGameplaySystemsBase() {}

	virtual bool IsMultiplayer() const = 0;

	virtual void OnServerActivated();
	virtual void OnServerDeactivated();
};
