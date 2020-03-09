#include "hitbox_debugData.h"
#include "weaponregistry.h"
#include "weapondebugevents/weapondebugevent_hitscanfire.h"
#include "customGeometry/messageWriter.h"
#include "geometryConstructors/hitboxGeometryConstructor.h"

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
	return m_Attacker.Get() && m_Target.Get();
}

void CHitboxDebugData::Clear()
{
	FireHitboxSnapshotClearMessage();

	m_Attacker.Set(nullptr);
	m_Target.Set(nullptr);
}

CBasePlayer* CHitboxDebugData::AttackerPlayer() const
{
	return m_Attacker.StaticCast<CBasePlayer>();
}

void CHitboxDebugData::SetAttackerPlayer(CBasePlayer* attacker)
{
	FireHitboxSnapshotClearMessage();
	m_Attacker.Set(attacker ? attacker->edict() : nullptr);
}

CBasePlayer* CHitboxDebugData::TargetPlayer() const
{
	// Entity may not be a player, so dynamic cast is needed.
	return m_Target.DynamicCast<CBasePlayer>();
}

void CHitboxDebugData::SetTargetPlayer(CBasePlayer* victim)
{
	m_Target.Set(victim ? victim->edict() : nullptr);
}

CBaseAnimating* CHitboxDebugData::TargetEnt() const
{
	// Any entity set should always be a subclass of CBaseAnimating,
	// so only static cast is needed.
	return m_Target.StaticCast<CBaseAnimating>();
}

void CHitboxDebugData::SetTargetEnt(CBaseAnimating* ent)
{
	m_Target.Set(ent ? ent->edict() : nullptr);
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

	if ( !m_Target )
	{
		ALERT(at_console, "Victim no longer valid, turning hitbox debugging off.\n");
		Clear();
		return;
	}

	if ( event->WeaponOwner() != m_Attacker.StaticCast<CBasePlayer>() )
	{
		return;
	}

	FireHitboxSnapshotMessages(event);
}

void CHitboxDebugData::FireHitboxSnapshotMessages(const CWeaponDebugEvent_HitscanFire* event)
{
	CBaseAnimating* target = m_Target.DynamicCast<CBaseAnimating>();
	CBasePlayer* attacker = m_Attacker.StaticCast<CBasePlayer>();

	if ( !target || !attacker )
	{
		return;
	}

	uint32_t hitboxCount = g_engfuncs.pfnGetHitboxCount(target->edict());

	if ( hitboxCount < 1 )
	{
		return;
	}

	{
		CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
		writer.SetTargetClient(attacker);
		writer.WriteClearMessage();
	}

	for ( uint32_t hitboxIndex = 0; hitboxIndex < hitboxCount; ++hitboxIndex )
	{
		CHitboxGeometryConstructor constructor;

		constructor.SetEntity(target);
		constructor.SetHitboxIndex(hitboxIndex);

		CustomGeometry::GeometryItemPtr_t hitboxGeometry(new CustomGeometry::CGeometryItem());
		constructor.AddGeometry(hitboxGeometry);

		CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
		writer.SetTargetClient(attacker);
		writer.WriteMessage(*hitboxGeometry);
	}

	CustomGeometry::GeometryItemPtr_t tracelineGeometry(new CustomGeometry::CGeometryItem());
	tracelineGeometry->SetDrawType(CustomGeometry::DrawType::Lines);

	for ( uint32_t traceIndex = 0; traceIndex < event->TraceCount(); ++traceIndex )
	{
		const CWeaponDebugEvent_HitscanFire::Trace* trace = event->GetTrace(traceIndex);
		tracelineGeometry->AddLine(trace->begin, trace->traceResult.vecEndPos);
	}

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
	writer.SetTargetClient(attacker);
	writer.WriteMessage(*tracelineGeometry);
}

void CHitboxDebugData::FireHitboxSnapshotClearMessage()
{
	CBasePlayer* player = m_Attacker.StaticCast<CBasePlayer>();

	if ( !player )
	{
		return;
	}

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
	writer.SetTargetClient(player);
	writer.WriteClearMessage();
}
