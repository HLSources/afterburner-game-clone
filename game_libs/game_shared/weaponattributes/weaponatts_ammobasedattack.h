#pragma once

#include "vector_classes.h"
#include "weaponatts_baseattack.h"
#include "weapons.h"
#include "resources/ShellImpactSoundResources.h"

namespace WeaponAtts
{
	struct AccuracyParameters
	{
		// Minimum allowed spread for this weapon, when
		// the weapon's inaccuracy value is 0.
		// This spread can be negative, if the point of
		// zero spread should be at an inaccuracy value
		// higher than zero. When interpolating between
		// min and max spread, the gun's actual spread
		// is clamped to a minimum of zero.
		Vector2D MinSpread;

		// Maximum allowed spread for this weapon, when
		// the weapon's inaccuracy value is 1.
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
		float CrouchShift = -0.08f;

		// Shift applied when the player is moving on Z at sv_weapon_inaccuracy_maxfallspeed
		// units per second. If the player is moving slower than this, the
		// actual shift that is applied is interpolated based on the Z speed.
		float FallShift = 0.2f;

		// All attributes above affect the base, instantaneous level of inaccuracy for the
		// current frame. A smoothed inaccuracy level is then calculated based on the
		// difference between the current and previous frames. The follow coefficient
		// specifies how much smoothing is applied. A value of 1 means no smoothing (the
		// instantaneous inaccuracy is used verbatim), and values closer to 0 cause more
		// smoothing (ie. the smoothed value tends towards the instantaneous value more
		// slowly). A value of 0 means the inaccuracy value will never actually change.
		float FollowCoefficient = 0.5f;

		// After the base level of inaccuracy is calculated, this impulse is added on the
		// frame when a weapon is fired. The inaccuracy recedes towards the base level
		// again on subsequent frames, according to the strength of the follow coefficient.
		float FireImpulse = 0.1f;

		// The maximum deviation from the frame's instantaneous inaccuracy level that fire
		// impulses are allowed to cause.
		float FireImpulseCeiling = 0.3f;
	};

	struct CrosshairParameters
	{
		bool HasCrosshair = true;

		// The radius is how far away from the centre of the screen each
		// crosshair bar is. A value of 1 means the bar is the length
		// of the shortest screen dimension.
		float RadiusMin = 0.0f;
		float RadiusMax = 0.5f;

		// These scales specify how long the crosshair bars are at minimum
		// and maximum inaccuracy. A value of 1 means the bar is the length
		// of the shortest screen dimension.
		float BarScaleMin = 0.1f;
		float BarScaleMax = 0.05f;
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
		CrosshairParameters Crosshair;
		int MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		float ViewPunchY = 0.0f;
		const char* ShellModelName = nullptr;
		ShellType ShellModelType = ShellType::Default;
		bool AutoReload = true;
		bool SpecialReload = false;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(Accuracy.FollowCoefficient >= 0.0f && Accuracy.FollowCoefficient <= 1.0f, "Follow coefficient must be between 0 and 1.");
		}
	};
}
