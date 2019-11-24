#pragma once

#include "weapondebugevent_base.h"
#include "standard_includes.h"

class CWeaponDebugEvent_HitscanFire : public CWeaponDebugEvent_Base
{
public:
	CWeaponDebugEvent_HitscanFire(CGenericWeapon& weapon);
	virtual ~CWeaponDebugEvent_HitscanFire() {}

	virtual CWeaponDebugEvent_Base::EventType Type() const override;

	const Vector& TraceBegin() const;
	const Vector& TraceEnd() const;
	float TraceHitFraction() const;

	void SetTrace(const Vector& begin, const Vector& end, float hitFraction);

private:
	Vector m_TraceBegin;
	Vector m_TraceEnd;
	float m_TraceHitFraction;
};
