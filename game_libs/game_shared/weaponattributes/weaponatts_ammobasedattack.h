#pragma once

#include "vector_classes.h"
#include "weaponatts_baseattack.h"
#include "weapons.h"
#include "resources/ShellImpactSoundResources.h"

namespace WeaponAtts
{
	struct AccuracyParameters
	{
		Vector2D MinSpread;
		Vector2D MaxSpread;

		// Amount of spread when the player is standing still.
		// This shoudl be in the range [0 1], where 0 is min
		// spread and 1 is max spread.
		float BaseSpreadInterpolant = 0.0f;

		// Spread interpolant is multiplied by this when the player is crouching.
		float CrouchSpreadMultiplier = 0.5f;

		// Spread is multiplied by this when the player is running at full speed.
		// "Full speed" is considered to be whatever the max forward speed is.
		// At speeds below this, the spread increase is linearly interpolated.
		float RunSpreadModifier = 1.5f;
	};

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
		AccuracyParameters Accuracy;
		int MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		float ViewPunchY = 0.0f;
		const char* ShellModelName = nullptr;
		ShellType ShellModelType = ShellType::Default;
		bool AutoReload = true;
		bool SpecialReload = false;

		virtual void Validate() const override
		{
			WABaseAttack::Validate();

			ASSERTSZ_Q(Accuracy.MinSpread.x >= 0.0f && Accuracy.MinSpread.y >= 0.0f, "Spread cannot be negative.");
			ASSERTSZ_Q(Accuracy.MaxSpread.x >= 0.0f && Accuracy.MaxSpread.y >= 0.0f, "Spread cannot be negative.");
		}
	};
}
