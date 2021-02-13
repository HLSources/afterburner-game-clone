#pragma once

#include <cmath>
#include "miniutl.h"

namespace ExtraMath
{
	static inline float Clamp(float min, float val, float max)
	{
		return Max(min, Min(val, max));
	}

	// Linearly remaps value from [a b] range to [x y] range.
	static inline float RemapClamped(float value, float a, float b, float x, float y)
	{
		if ( fabs(a - b) < 0.001f )
		{
			// No domain to remap from.
			return x;
		}

		return x + Clamp(0.0f, (value - a) / (b - a), 1.0f) * (y - x);
	}
}
