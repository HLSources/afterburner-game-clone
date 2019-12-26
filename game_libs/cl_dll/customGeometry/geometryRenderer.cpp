#include "geometryRenderer.h"
#include "cl_dll.h"
#include "triangleapi.h"

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
				DrawLines(points, item.GetColour());
				break;
			}

			default:
			{
				break;
			}
		}
	}

	void CGeometryRenderer::DrawLines(const CUtlVector<Vector>& points, const uint32_t colour)
	{
		const size_t halfCount = points.Count() / 2;

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
			gEngfuncs.pTriAPI->Vertex3fv(points[2 * index]);
			gEngfuncs.pTriAPI->Vertex3fv(points[(2 * index) + 1]);
		}

		gEngfuncs.pTriAPI->End();
	}
}
