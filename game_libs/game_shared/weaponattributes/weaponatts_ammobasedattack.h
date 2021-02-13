#pragma once

#include "vector_classes.h"
#include "weaponatts_baseattack.h"
#include "weapons.h"
#include "resources/ShellImpactSoundResources.h"

namespace WeaponAtts
{
	struct AccuracyParameters
	{
		// Minimum allowed spread for this weapon.
		Vector2D MinSpread;

		// Maximum allowed spread for this weapon.
		Vector2D MaxSpread;

		// The following values should be specified in the range [0 1].
		// A value of 0 corresponds to the min spread, and a value of 1
		// corresponds to the max spread.

		// Value representing spread when the player is standing still.
		float RestValue = 0.1f;

		// Value representing spread when the player is running at
		// sv_weapon_inaccuracy_maxspeed units per second.
		float RunValue = 0.8f;

		// Once the speed-based spread value in range [RestValue RunValue]
		// is calculated, one or more of these shifts are added based on
		// the state of the player. The resulting value will be clamped to
		// the range [0 1].

		// Shift applied when the player is crouching.
		float CrouchShift = -0.1f;

		// Shift applied when the player is moving on Z at sv_weapon_inaccuracy_maxfallspeed
		// units per second. If the player is moving slower than this, the
		// actual shift that is applied is interpolated based on the Z speed.
		float FallShift = 0.2f;
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
