#include "weaponregistry.h"
#include "standard_includes.h"
#include "genericweapon.h"

CWeaponRegistry::CWeaponRegistry()
	: m_AttributesList{0}
{
}

CWeaponRegistry& CWeaponRegistry::StaticInstance()
{
	static CWeaponRegistry registry;
	return registry;
}

void CWeaponRegistry::Add(const WeaponAtts::WACollection* atts)
{
	if ( !atts )
	{
		return;
	}

	const int id = static_cast<const int>(atts->Core.Id);
	ASSERTSZ_Q(id >= 0 && id < MAX_WEAPONS, "Weapon ID is out of range!");
	ASSERTSZ_Q(m_AttributesList[id] == NULL, "Attributes already present at this index.");

	m_AttributesList[id] = atts;
}

const WeaponAtts::WACollection* CWeaponRegistry::Get(int index) const
{
	// Don't allow index 0 as this indicates no weapon.
	return (index > 0 && index < MAX_WEAPONS) ? m_AttributesList[index] : NULL;
}

const WeaponAtts::WACollection* CWeaponRegistry::Get(WeaponId_e id) const
{
	return Get(static_cast<int>(id));
}

void CWeaponRegistry::RegisterCvars()
{
	ForEach([](const WeaponAtts::WACollection& atts)
	{
		FOR_EACH_VEC(atts.SkillRecords, index)
		{
			atts.SkillRecords[index].RegisterCvars();
		}

		FOR_EACH_VEC(atts.CustomCvars, index)
		{
			CVAR_REGISTER(atts.CustomCvars[index]);
		}
	});
}
