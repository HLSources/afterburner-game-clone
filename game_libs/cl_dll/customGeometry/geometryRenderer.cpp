#include "geometryRenderer.h"
#include "cl_dll.h"
#include "triangleapi.h"
#include "debug_assert.h"

namespace CustomGeometry
{
	void CGeometryRenderer::Render(const CGeometryItem& item)
	{
		const CUtlVector<Vector>& points = item.GetPoints();

		if ( points.Count() < 1 )
		{
			return;
		}

		switch ( item.GetDrawType() )
		{
			case DrawType::Lines:
			{
				DrawLines(item);
				break;
			}

			default:
			{
				break;
			}
		}
	}

	void CGeometryRenderer::DrawLines(const CGeometryItem& item)
	{
		const CUtlVector<Vector>& points = item.GetPoints();
		const CUtlVector<uint8_t>& indices = item.GetIndices();
		const size_t halfCount = indices.Count() / 2;
		const uint32_t colour = item.GetColour();

		if ( halfCount < 1 )
		{
			return;
		}

		gEngfuncs.pTriAPI->RenderMode(kRenderNormal);
		gEngfuncs.pTriAPI->Color4ub((colour & 0xFF000000) >> 24,
									(colour & 0x00FF0000) >> 16,
									(colour & 0x0000FF00) >> 8,
									(colour & 0x000000FF) >> 0);

		gEngfuncs.pTriAPI->Begin(TRI_LINES);

		for ( uint32_t index = 0; index < halfCount; ++index )
		{
			uint8_t pointIndex0 = indices[2 * index];
			uint8_t pointIndex1 = indices[(2 * index) + 1];

			ASSERTSZ(pointIndex0 < static_cast<size_t>(points.Count()), "Index was out of range.");
			ASSERTSZ(pointIndex1 < static_cast<size_t>(points.Count()), "Index was out of range.");

			gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0]);
			gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1]);
		}

		gEngfuncs.pTriAPI->End();
	}
}
