#pragma once

namespace CustomGeometry
{
	static constexpr const char* MESSAGE_NAME = "DrawCustomGeometry";
	static constexpr size_t MAX_POINTS_PER_MSG = 256;

	enum class DrawType
	{
		None = 0,	// For clearing previously drawn geometry.
		Lines
	};
}
