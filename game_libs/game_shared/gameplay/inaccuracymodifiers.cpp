#include "gameplay/inaccuracymodifiers.h"
#include "standard_includes.h"
#include "miniutl.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

namespace InaccuracyModifiers
{
	Vector2D GetInterpolatedSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy)
	{
		// The inaccuracy starts out in the overall range [0 1]. The rest and run values also live within this range.
		// Remap this to a new [0 1] value, where 0 represents rest and 1 represents running at full speed.
		// If the original inaccuracy was outside the range [RestValue RunValue], the remapped inaccuracy will be
		// outside the new range [0 1].
		const float remappedInaccuracy = ExtraMath::RemapLinear(inaccuracy, params.RestValue, params.RunValue, 0.0f, 1.0f, false);

		// Calculate a spread vector based on the remapped inaccuracy.
		const Vector2D spread = params.RestSpread + (remappedInaccuracy * (params.RunSpread - params.RestSpread));

		// Return this, but make sure components are not less than zero.
		return Vector2D(Max(spread.x, 0.0f), Max(spread.y, 0.0f));
	}
}
