#pragma once

namespace CustomGeometry
{
	static constexpr const char* MESSAGE_NAME = "DrawCustomGeometry";
	static constexpr size_t MAX_POINTS_PER_MSG = 256;

	// For grouping geometry by purpose, eg. visualising hitboxes, drawing overlays, etc.
	// When sending a "clear" message, only the geometry from the specified category is cleared.
	// Values must fit in a uint8_t.
	enum class Category
	{
		None = 0,	// Sending this on a clear message clears all geometry.
		DebugPlayerBounds,
		HitboxDebugging,

		CategoryCount
	};

	static constexpr size_t CATEGORY_COUNT = static_cast<size_t>(Category::CategoryCount);

	// Values must fit in a uint8_t;
	enum class DrawType
	{
		None = 0,	// For clearing previously drawn geometry.
		Lines
	};
}
