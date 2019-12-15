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

	FireHitboxSnapshotClearMessage();
}

bool CHitboxDebugData::IsValid() const
{
	return m_Attacker.Get() && m_Victim.Get();
}

void CHitboxDebugData::Clear()
{
	FireHitboxSnapshotClearMessage();

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

	if ( !m_Attacker )
	{
		ALERT(at_console, "Attacker no longer valid, turning hitbox debugging off.\n");
		Clear();
		return;
	}

	if ( !m_Victim )
	{
		ALERT(at_console, "Victim no longer valid, turning hitbox debugging off.\n");
		Clear();
		return;
	}

	if ( event->WeaponOwner() != m_Attacker.StaticCast<CBasePlayer>() )
	{
		return;
	}

	FireHitboxSnapshotMessages();
}

void CHitboxDebugData::FireHitboxSnapshotMessages()
{
	ALERT(at_warning, "TODO: Fire messages\n");
}

void CHitboxDebugData::FireHitboxSnapshotClearMessage()
{
	ALERT(at_warning, "TODO: Fire message\n");
}
