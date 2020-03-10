#pragma once

class CBasePlayer;
class CBaseAnimating;
class CWeaponDebugEvent_HitscanFire;

class CHitboxMessageConstructor
{
public:
	CHitboxMessageConstructor(CBasePlayer* attacker, CBaseAnimating* target = nullptr);

	void FireMessages(const CWeaponDebugEvent_HitscanFire& event);
	void FireClearMessage();

private:

	CBasePlayer* m_Attacker = nullptr;
	CBaseAnimating* m_Target = nullptr;
};
