#pragma once

#include "weaponatts_base.h"
#include "ammodefs.h"

namespace WeaponAtts
{
	struct WAAmmoDef : public WABase
	{
		const CAmmoDef* PrimaryAmmo = nullptr;
		const CAmmoDef* SecondaryAmmo = nullptr;
		int PrimaryAmmoOnFirstPickup = 0;
		int MaxClip = 0;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(PrimaryAmmoOnFirstPickup >= 0, "Amount of primary ammo on pickup must be positive.");
		}
	};
}
