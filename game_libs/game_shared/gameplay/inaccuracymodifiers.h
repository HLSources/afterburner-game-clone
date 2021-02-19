#pragma once

#include "vector_classes.h"

class CBasePlayer;

namespace InaccuracyModifiers
{
	Vector2D GetInterpolatedSpread(const Vector2D& min, const Vector2D& max, float interpolant);
}
