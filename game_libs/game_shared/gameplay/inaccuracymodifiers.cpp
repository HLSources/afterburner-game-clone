#include "gameplay/inaccuracymodifiers.h"
#include "standard_includes.h"
#include "miniutl.h"
#include "util/extramath.h"

namespace InaccuracyModifiers
{
	float GetSpeedBasedInaccuracy(CBasePlayer* player, float maxAllowedSpeed)
	{
		if ( !player )
		{
			return 0.0f;
		}

		float playerXYSpeed = player->pev->velocity.Length2D();

		return ExtraMath::RemapClamped(playerXYSpeed, 0.0f, maxAllowedSpeed, 0.0f, 1.0f);
	}

	Vector2D GetInterpolatedSpread(const Vector2D& min, const Vector2D& max, float interpolant)
	{
		return min + (interpolant * (max - min));
	}
}
