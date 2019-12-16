#pragma once

#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CMessageReader
	{
	public:
		enum class ReadResult
		{
			Error = 0,
			OK,
			Clear
		};

		ReadResult ReadMessage(void* buffer, int size, CGeometryItem& item);
	};
}
