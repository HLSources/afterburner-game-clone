#pragma once

#include "standard_includes.h"

class CWeaponDebugEvent_HitscanFire;

class CHitboxDebugData
{
public:
	CHitboxDebugData();
	~CHitboxDebugData();

	bool IsValid() const;
	void Clear();

	CBasePlayer* Attacker() const;
	void SetAttacker(CBasePlayer* attacker);

	CBasePlayer* Victim() const;
	void SetVictim(CBasePlayer* victim);

private:
	void FireHitboxSnapshotMessages();
	void FireHitboxSnapshotClearMessage();
	void HandleHitscanFire(const CWeaponDebugEvent_HitscanFire* event);

	EHANDLE m_Attacker;
	EHANDLE m_Victim;
	EHANDLE m_Snapshot;
};
