#include "gameplay/inaccuracymodifiers.h"
#include "standard_includes.h"
#include "miniutl.h"

namespace InaccuracyModifiers
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

	float GetSpeedBasedInaccuracy(CBasePlayer* player, float maxAllowedSpeed)
	{
		if ( !player )
		{
			return 0.0f;
		}

		float playerXYSpeed = player->pev->velocity.Length2D();

		return RemapClamped(playerXYSpeed, 0.0f, maxAllowedSpeed, 0.0f, 1.0f);
	}
}
