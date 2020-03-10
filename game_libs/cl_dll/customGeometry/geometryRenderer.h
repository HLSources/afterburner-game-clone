#pragma once

#include "customGeometry/geometryItem.h"
#include "cdll_int.h"

namespace CustomGeometry
{
	class CGeometryRenderer
	{
	public:
		CGeometryRenderer();

		void Render(const CGeometryItem& item);

	private:
		void DrawLines(const CGeometryItem& item);

		HSPRITE m_ColourSprite = 0;
	};
}
