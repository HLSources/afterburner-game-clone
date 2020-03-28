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
		void Prepare(int renderMode, uint32_t colour);
		void DrawLines(const CGeometryItem& item);
		void DrawTriangleFan(const CGeometryItem& item);

		HSPRITE m_ColourSprite = 0;
	};
}
