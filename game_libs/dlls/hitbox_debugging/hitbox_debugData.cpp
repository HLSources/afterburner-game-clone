#include "hitbox_debugData.h"
#include "weaponregistry.h"
#include "weapondebugevents/weapondebugevent_hitscanfire.h"

static constexpr const char* EVENT_CALLBACK_ID = "CHitboxDebugData";

CHitboxDebugData::CHitboxDebugData()
{
	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();

	evSource.RegisterCallback(EVENT_CALLBACK_ID, CWeaponDebugEvent_Base::EventType::Event_HitscanFire,
	[this](const CWeaponDebugEvent_Base* event)
	{
		HandleHitscanFire(dynamic_cast<const CWeaponDebugEvent_HitscanFire*>(event));
	});
}

CHitboxDebugData::~CHitboxDebugData()
{
	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();
	evSource.UnregisterCallback(EVENT_CALLBACK_ID);
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

void CHitboxDebugData::HandleHitscanFire(const CWeaponDebugEvent_HitscanFire* event)
{
	if ( !event )
	{
		return;
	}

	// TODO: Implement this
	const Vector& begin = event->TraceBegin();
	const Vector& end = event->TraceEnd();
	float frac = event->TraceHitFraction();

	ALERT(at_console, "Hitscan fire event: Begin = (%f %f %f), End = (%f %f %f) (%f%% of trace)\n",
		  begin[0],
		  begin[1],
		  begin[2],
		  end[0],
		  end[1],
		  end[2],
		  frac);
}
