#pragma once

#include <cstdint>
#include "weaponids.h"
#include "cdll_dll.h"

namespace WeaponAtts
{
	struct WACollection;
}

class CGenericWeapon;

class CWeaponRegistry
{
public:
	CWeaponRegistry();

	void Add(const WeaponAtts::WACollection* atts);
	const WeaponAtts::WACollection* Get(int index) const;
	const WeaponAtts::WACollection* Get(WeaponId_e id) const;
	void RegisterCvars();

	template<typename T>
	void ForEach(const T& callback)
	{
		for ( uint32_t index = 0; index < MAX_WEAPONS; ++index )
		{
			const WeaponAtts::WACollection* atts = m_AttributesList[index];

			// Note that index 0 (Weapon_None) will not have a valid pointer.
			if ( atts )
			{
				callback(*atts);
			}
		}
	}

	static CWeaponRegistry& StaticInstance();

private:
	const WeaponAtts::WACollection* m_AttributesList[MAX_WEAPONS];
};
