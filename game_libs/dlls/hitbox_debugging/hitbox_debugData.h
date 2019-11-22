#pragma once

#include "standard_includes.h"

class CHitboxDebugData
{
public:
	CHitboxDebugData();

	bool IsValid() const;
	void Clear();

	CBasePlayer* Attacker() const;
	void SetAttacker(CBasePlayer* attacker);

	CBasePlayer* Victim() const;
	void SetVictim(CBasePlayer* victim);

private:
	EHANDLE m_Attacker;
	EHANDLE m_Victim;
};
