#include "hitbox_debugData.h"

CHitboxDebugData::CHitboxDebugData()
{
}

bool CHitboxDebugData::IsValid() const
{
	return m_Attacker.Get() && m_Victim.Get();
}

void CHitboxDebugData::Clear()
{
	m_Attacker.Set(nullptr);
	m_Victim.Set(nullptr);
}

CBasePlayer* CHitboxDebugData::Attacker() const
{
	return m_Attacker.StaticCast<CBasePlayer>();
}

void CHitboxDebugData::SetAttacker(CBasePlayer* attacker)
{
	m_Attacker.Set(attacker ? attacker->edict() : nullptr);
}

CBasePlayer* CHitboxDebugData::Victim() const
{
	return m_Victim.StaticCast<CBasePlayer>();
}

void CHitboxDebugData::SetVictim(CBasePlayer* victim)
{
	m_Victim.Set(victim ? victim->edict() : nullptr);
}
