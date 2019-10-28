#include "weapon_fists.h"
#include "weapon_pref_weights.h"
#include "weapon_fists_atts.h"

LINK_ENTITY_TO_CLASS(weapon_fists, CWeaponFists);
LINK_ENTITY_TO_CLASS(weapon_dukes, CWeaponFists);	// For NF compatibility

CWeaponFists::CWeaponFists() : CGenericMeleeWeapon()
{
	m_pPunchAttack = GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH);
	m_pPunchComboAttack = GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH_COMBO);

	m_pPrimaryAttackMode = m_pPunchAttack;
	m_pSecondaryAttackMode = GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(ATTACKMODE_KARATE_CHOP);
}

const WeaponAtts::WACollection& CWeaponFists::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFists>();
}

bool CWeaponFists::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !CGenericMeleeWeapon::InvokeWithAttackMode(type, attackMode) )
	{
		return false;
	}

	if ( type == WeaponAttackType::Primary )
	{
		// Alternate between modes.
		m_pPrimaryAttackMode = m_pPrimaryAttackMode == m_pPunchAttack
			? m_pPunchComboAttack
			: m_pPunchAttack;
	}

	return true;
}

#ifndef CLIENT_DLL
float CWeaponFists::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	// TODO
	return 0;
}

void CWeaponFists::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	// TODO
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>()
	{
		return ::StaticWeaponAttributes;
	}
}
