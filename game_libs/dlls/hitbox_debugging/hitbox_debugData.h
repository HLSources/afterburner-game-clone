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

	CBasePlayer* AttackerPlayer() const;
	void SetAttackerPlayer(CBasePlayer* attacker);

	// The target can either be a player or a general entity with a model.
	CBasePlayer* TargetPlayer() const;
	void SetTargetPlayer(CBasePlayer* victim);

	CBaseAnimating* TargetEnt() const;
	void SetTargetEnt(CBaseAnimating* ent);

private:
	void FireHitboxSnapshotMessages(const CWeaponDebugEvent_HitscanFire* event);
	void FireHitboxSnapshotClearMessage();
	void HandleHitscanFire(const CWeaponDebugEvent_HitscanFire* event);

	EHANDLE m_Attacker;
	EHANDLE m_Target;
};
