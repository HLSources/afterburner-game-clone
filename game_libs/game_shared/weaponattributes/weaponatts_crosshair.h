#pragma once

#include "weaponatts_base.h"

namespace WeaponAtts
{
	struct WACrosshair : public WABase
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
}
