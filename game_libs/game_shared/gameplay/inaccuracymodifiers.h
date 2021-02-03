#pragma once

class CBasePlayer;

namespace InaccuracyModifiers
{
	float GetSpeedBasedInaccuracy(CBasePlayer* player, float maxAllowedSpeed);
}
