#include "weapondebugevent_hitscanfire.h"

CWeaponDebugEvent_HitscanFire::CWeaponDebugEvent_HitscanFire(CGenericWeapon& weapon) :
	CWeaponDebugEvent_Base(weapon),
	m_TraceBegin(),
	m_TraceEnd(),
	m_TraceHitFraction(0.0f)
{
}

CWeaponDebugEvent_Base::EventType CWeaponDebugEvent_HitscanFire::Type() const
{
	return CWeaponDebugEvent_Base::EventType::Event_HitscanFire;
}

const Vector& CWeaponDebugEvent_HitscanFire::TraceBegin() const
{
	return m_TraceBegin;
}

const Vector& CWeaponDebugEvent_HitscanFire::TraceEnd() const
{
	return m_TraceEnd;
}

float CWeaponDebugEvent_HitscanFire::TraceHitFraction() const
{
	return m_TraceHitFraction;
}

void CWeaponDebugEvent_HitscanFire::SetTrace(const Vector& begin, const Vector& end, float hitFraction)
{
	m_TraceBegin = begin;
	m_TraceEnd = end;
	m_TraceHitFraction = hitFraction;
}
