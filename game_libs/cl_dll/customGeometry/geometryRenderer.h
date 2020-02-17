#pragma once

#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CGeometryRenderer
	{
	public:
		void Render(const CGeometryItem& item);

	private:
		void DrawLines(const CGeometryItem& item);
	};
}
