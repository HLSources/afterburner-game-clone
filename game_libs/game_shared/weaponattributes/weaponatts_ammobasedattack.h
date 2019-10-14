#pragma once

#include "weaponatts_baseattack.h"
#include "weapons.h"

namespace WeaponAtts
{
    struct WAAmmoBasedAttack : public WABaseAttack
    {
        enum class AmmoPool
		{
			None = -1,
			Primary = 0,
			Secondary = 1
		};

        static constexpr size_t AMMO_POOL_COUNT = 2;

        AmmoPool UsesAmmoPool = AmmoPool::None;
        float SpreadX = 0.0f;
        float SpreadY = 0.0f;
        int MuzzleFlashBrightness = NORMAL_GUN_FLASH;
        float ViewPunchY = 0.0f;
        const char* ShellModelName = nullptr;
        bool AutoReload = true;
        bool SpecialReload = false;

        inline void SetUniformSpread(float spread)
        {
            SpreadX = spread;
            SpreadY = spread;
        }

        virtual void Validate() const override
        {
            WABaseAttack::Validate();

            ASSERTSZ_Q(SpreadX >= 0.0f, "Spread cannot be negative.");
            ASSERTSZ_Q(SpreadY >= 0.0f, "Spread cannot be negative.");
        }
    };
}
