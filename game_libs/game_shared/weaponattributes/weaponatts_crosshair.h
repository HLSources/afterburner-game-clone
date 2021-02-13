#pragma once

#include "weaponatts_base.h"

namespace WeaponAtts
{
	struct WACrosshair : public WABase
	{
		bool HasCrosshair = true;

		// A scale of 1 means that maximum inaccuracy correlates with
		// the crosshair bars being pushed out right to the edge of
		// the shorted screen dimension.
		float Scale = 0.2f;

		// These scales specify how long the crosshair bars are at minimum
		// and maximum inaccuracy. A value of 1 means the bar is the length
		// of the shortest screen dimension.
		float BarScaleMin = 0.1f;
		float BarScaleMax = 0.1f;
	};
}
