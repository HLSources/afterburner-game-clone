#include "weapondebugevent_base.h"

CWeaponDebugEvent_Base::CWeaponDebugEvent_Base(CGenericWeapon& weapon) :
	m_pWeapon(&weapon)
{
}

CGenericWeapon* CWeaponDebugEvent_Base::Weapon() const
{
	return m_pWeapon;
}
