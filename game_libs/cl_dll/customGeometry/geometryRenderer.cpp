#include "geometryRenderer.h"
#include "cl_dll.h"
#include "triangleapi.h"
#include "debug_assert.h"
#include "hud.h"
#include "cl_util.h"

namespace CustomGeometry
{
	CGeometryRenderer::CGeometryRenderer()
	{
		m_ColourSprite = SPR_Load("sprites/white.spr");
	}

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

		// This sprite is used as a base texture which we then set the colour of.
		// For some reason lines don't draw correctly unless a texture is bound...
		const model_s* colourSprite = gEngfuncs.GetSpritePointer(m_ColourSprite);

		if ( !colourSprite )
		{
			return;
		}

		// UGH. Awful HL const-incorrectness again.
		gEngfuncs.pTriAPI->SpriteTexture(const_cast<model_s*>(colourSprite), 0);
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

			gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
			gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0]);

			gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
			gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1]);
		}

		gEngfuncs.pTriAPI->End();
	}
}
