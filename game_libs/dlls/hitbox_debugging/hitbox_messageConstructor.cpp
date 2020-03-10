#include "hitbox_messageConstructor.h"
#include "standard_includes.h"
#include "weapondebugevents/weapondebugevent_hitscanfire.h"
#include "customGeometry/messageWriter.h"
#include "geometryConstructors/hitboxGeometryConstructor.h"

namespace
{
	static bool HitGroupWasHit(const CWeaponDebugEvent_HitscanFire& event, uint32_t hitGroup)
	{
		for ( uint32_t traceIndex = 0; traceIndex < event.TraceCount(); ++traceIndex )
		{
			if ( event.GetTrace(traceIndex)->traceResult.iHitgroup == hitGroup )
			{
				return true;
			}
		}

		return false;
	}

	static inline bool HitboxWasHit(const CWeaponDebugEvent_HitscanFire& event, const edict_t* edict, uint32_t hitboxIndex)
	{
		int hitGroup = g_engfuncs.pfnGetHitboxHitGroup(edict, hitboxIndex);
		return hitGroup >= 0 && HitGroupWasHit(event, hitGroup);
	}
}

CHitboxMessageConstructor::CHitboxMessageConstructor(CBasePlayer* attacker, CBaseAnimating* target) :
	m_Attacker(attacker),
	m_Target(target)
{
}

void CHitboxMessageConstructor::FireMessages(const CWeaponDebugEvent_HitscanFire& event)
{
	if ( !m_Attacker || !m_Target )
	{
		return;
	}

	FireClearMessage();

	uint32_t hitboxCount = g_engfuncs.pfnGetHitboxCount(m_Target->edict());

	if ( hitboxCount < 1 )
	{
		return;
	}

	{
		CustomGeometry::GeometryItemPtr_t missTraceLines(new CustomGeometry::CGeometryItem());
		missTraceLines->SetDrawType(CustomGeometry::DrawType::Lines);
		missTraceLines->SetColour(0xFFFFFFFF);

		CustomGeometry::GeometryItemPtr_t hitTraceLines(new CustomGeometry::CGeometryItem());
		hitTraceLines->SetDrawType(CustomGeometry::DrawType::Lines);
		hitTraceLines->SetColour(0xFF0000FF);

		for ( uint32_t traceIndex = 0; traceIndex < event.TraceCount(); ++traceIndex )
		{
			const CWeaponDebugEvent_HitscanFire::Trace* trace = event.GetTrace(traceIndex);

			if ( trace->traceResult.flFraction < 1.0f &&
				 trace->traceResult.pHit &&
				 GetClassPtrFromEdict<CBasePlayer>(trace->traceResult.pHit) &&
				 trace->traceResult.iHitgroup < hitboxCount )
			{
				hitTraceLines->AddLine(trace->begin, trace->traceResult.vecEndPos);
			}
			else
			{
				missTraceLines->AddLine(trace->begin, trace->traceResult.vecEndPos);
			}
		}

		CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
		writer.SetTargetClient(m_Attacker);
		writer.WriteMessage(*missTraceLines);
		writer.WriteMessage(*hitTraceLines);
	}

	for ( uint32_t hitboxIndex = 0; hitboxIndex < hitboxCount; ++hitboxIndex )
	{
		CHitboxGeometryConstructor constructor;

		constructor.SetEntity(m_Target);
		constructor.SetHitboxIndex(hitboxIndex);

		CustomGeometry::GeometryItemPtr_t hitboxGeometry(new CustomGeometry::CGeometryItem());
		hitboxGeometry->SetColour(HitboxWasHit(event, m_Target->edict(), hitboxIndex) ? 0xFF0000FF : 0xFFFFFFFF);
		constructor.AddGeometry(hitboxGeometry);

		CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
		writer.SetTargetClient(m_Attacker);
		writer.WriteMessage(*hitboxGeometry);
	}
}

void CHitboxMessageConstructor::FireClearMessage()
{
	if ( !m_Attacker )
	{
		return;
	}

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
	writer.SetTargetClient(m_Attacker);
	writer.WriteClearMessage();
}
