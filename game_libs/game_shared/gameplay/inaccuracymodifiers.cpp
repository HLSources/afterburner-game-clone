#include "gameplay/inaccuracymodifiers.h"
#include "standard_includes.h"
#include "miniutl.h"
#include "util/extramath.h"

namespace InaccuracyModifiers
{
	Vector2D GetInterpolatedSpread(const Vector2D& min, const Vector2D& max, float interpolant)
	{
		const Vector2D spread = min + (interpolant * (max - min));
		return Vector2D(Max(spread.x, 0.0f), Max(spread.y, 0.0f));
	}
}
