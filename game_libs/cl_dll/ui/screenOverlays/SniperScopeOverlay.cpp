#include "SniperScopeOverlay.h"
#include "cl_dll.h"
#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "customGeometry/geometryStatics.h"
#include "coreutil/coreutil.h"

// TODO: Properly integrate these, then remove.
/*static inline void DrawSniperScopeOverlay()
{
	static int scopeHandle = 0;

	if ( scopeHandle == 0 )
	{
		scopeHandle = SPR_Load("sprites/sniper_scope.spr");

		if ( scopeHandle == 0 )
		{
			return;
		}
	}

	SPR_Set(scopeHandle, 255, 255, 255);

	sprite_draw_args_t args;
	memset(&args, 0, sizeof(args));

	args.x = 448;
	args.y = 28;
	args.width = 1024;
	args.height = 1024;
	args.renderMode = kRenderTransTexture;

	SPR_DrawCustom(&args);
}*/

static inline void DrawSolidQuad(int x, int y, int width, int height)
{
	gEngfuncs.pTriAPI->Begin(TRI_QUADS);

	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(x, y, 0);

	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(x + width, y, 0);

	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(x + width, y + height, 0);

	gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	gEngfuncs.pTriAPI->Vertex3f(x, y + height, 0);

	gEngfuncs.pTriAPI->End();
}

static inline void SetUpSolidQuadDrawing()
{
	static int whiteHandle = 0;

	if ( whiteHandle == 0 )
	{
		whiteHandle = SPR_Load("sprites/white.spr");

		if ( whiteHandle == 0 )
		{
			return;
		}
	}

	const model_s* sprite = gEngfuncs.GetSpritePointer(whiteHandle);

	if ( !sprite )
	{
		return;
	}

	gEngfuncs.pTriAPI->SpriteTexture(const_cast<model_s*>(sprite), 0);
	gEngfuncs.pTriAPI->RenderMode(kRenderNormal);
	gEngfuncs.pTriAPI->Color4ub(0, 0, 0, 255);
}

static inline void DrawSniperScopeSurroundings()
{
	DrawSolidQuad(0, 0, 1920, 28);
	DrawSolidQuad(0, 28 + 1024, 1920, 28);
	DrawSolidQuad(0, 28, 448, 1024);
	DrawSolidQuad(448 + 1024, 28, 448, 1024);
}

static inline void DrawSniperScopeCrosshair(int width)
{
	DrawSolidQuad((1920 / 2) - (width / 2), 0, width, 1080);
	DrawSolidQuad(0, (1080 / 2) - (width / 2), 1920, width);
}

static inline void GetCirclePointCoOrds(int point, int segmentCount, float radius, float& x, float& y)
{
	static constexpr float LOCAL_2PI = 2.0f * 3.1415926f;

	const float theta = (LOCAL_2PI * (float)point) / (float)segmentCount;
	x = radius * cosf(theta);
	y = radius * sinf(theta);
}

static void DrawCircleWithLines(int x, int y, float radius, int segmentCount)
{
	gEngfuncs.pTriAPI->Begin(TRI_LINES);

	for ( int point = 0; point < segmentCount; ++point )
	{
		float vx = 0.0f;
		float vy = 0.0f;

		GetCirclePointCoOrds(point, segmentCount, radius, vx, vy);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((float)x + vx, (float)y + vy, 0);

		GetCirclePointCoOrds((point + 1) % segmentCount, segmentCount, radius, vx, vy);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((float)x + vx, (float)y + vy, 0);
	}

	gEngfuncs.pTriAPI->End();
}

static void DrawCircleWithQuads(int x, int y, float outerRadius, float innerRadius, int segmentCount)
{
	typedef struct
	{
		float x;
		float y;
	} CoOrd;

	gEngfuncs.pTriAPI->Begin(TRI_QUADS);

	for ( int point = 0; point < segmentCount; ++point )
	{
		CoOrd quadPoints[4];

		const int nextPoint = (point + 1) % segmentCount;
		GetCirclePointCoOrds(point, segmentCount, innerRadius, quadPoints[0].x, quadPoints[0].y);
		GetCirclePointCoOrds(point, segmentCount, outerRadius, quadPoints[1].x, quadPoints[1].y);
		GetCirclePointCoOrds(nextPoint, segmentCount, outerRadius, quadPoints[2].x, quadPoints[2].y);
		GetCirclePointCoOrds(nextPoint, segmentCount, innerRadius, quadPoints[3].x, quadPoints[3].y);

		for ( int index = 0; index < 4; ++index )
		{
			gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
			gEngfuncs.pTriAPI->Vertex3f((float)x + quadPoints[index].x, (float)y + quadPoints[index].y, 0);
		}
	}

	gEngfuncs.pTriAPI->End();
}

static inline void DrawSniperScopeExtraBars()
{
	static constexpr size_t BAR_HEIGHT = 75;
	static constexpr size_t BAR_WIDTH = 2;

	DrawSolidQuad((1920 / 2) - 250, (1080 / 2) - (BAR_HEIGHT / 2), BAR_WIDTH, BAR_HEIGHT);
	DrawSolidQuad((1920 / 2) + 250, (1080 / 2) - (BAR_HEIGHT / 2), BAR_WIDTH, BAR_HEIGHT);
}

static inline void DrawSniperScopeOuterNotches()
{
	static constexpr size_t NOTCH_WIDTH = 34;
	static constexpr size_t NOTCH_HEIGHT = 150;
	static constexpr size_t NOTCH_TIP_HEIGHT = 20;

	// Top
	{
		const int baseY = (1080 / 2) - 512;

		DrawSolidQuad((1920 / 2) - (NOTCH_WIDTH / 2), baseY, NOTCH_WIDTH, NOTCH_HEIGHT - NOTCH_TIP_HEIGHT);

		gEngfuncs.pTriAPI->Begin(TRI_TRIANGLES);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) - (NOTCH_WIDTH / 2), baseY + NOTCH_HEIGHT - NOTCH_TIP_HEIGHT, 0);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) + 1, baseY + NOTCH_HEIGHT, 0);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) + (NOTCH_WIDTH / 2), baseY + NOTCH_HEIGHT - NOTCH_TIP_HEIGHT, 0);

		gEngfuncs.pTriAPI->End();
	}

	// Bottom
	{
		const int baseY = (1080 / 2) + 512 - NOTCH_HEIGHT;

		DrawSolidQuad((1920 / 2) - (NOTCH_WIDTH / 2), baseY + NOTCH_TIP_HEIGHT, NOTCH_WIDTH, NOTCH_HEIGHT - NOTCH_TIP_HEIGHT);

		gEngfuncs.pTriAPI->Begin(TRI_TRIANGLES);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) + (NOTCH_WIDTH / 2), baseY + NOTCH_TIP_HEIGHT, 0);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) + 1, baseY, 0);

		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
		gEngfuncs.pTriAPI->Vertex3f((1920 / 2) - (NOTCH_WIDTH / 2), baseY + NOTCH_TIP_HEIGHT, 0);

		gEngfuncs.pTriAPI->End();
	}

	// Left
	{
		DrawSolidQuad((1920 / 2) - 512, (1080 / 2) - (NOTCH_WIDTH / 2), NOTCH_HEIGHT - NOTCH_TIP_HEIGHT, NOTCH_WIDTH);
	}

	// Right
	{
		DrawSolidQuad((1920 / 2) + 512 - (NOTCH_HEIGHT - NOTCH_TIP_HEIGHT), (1080 / 2) - (NOTCH_WIDTH / 2), NOTCH_HEIGHT - NOTCH_TIP_HEIGHT, NOTCH_WIDTH);
	}
}

namespace
{
	// How much of the short edge of the screen should the base scope image take up?
	// This must be in the interval (0 1], otherwise you're in for bad times.
	static constexpr float SCREEN_SHORTEDGE_FACTOR = 0.95f;
}

CSniperScopeOverlay::~CSniperScopeOverlay()
{
}

ScreenOverlays::OverlayId CSniperScopeOverlay::Id() const
{
	return OVERLAY_ID;
}

void CSniperScopeOverlay::VidInit()
{
	m_hScopeImage = SPR_Load("sprites/sniper_scope.spr");

	SCREENINFO screenInfo;
	memset(&screenInfo, 0, sizeof(screenInfo));

	// This API is really weird, dude. We should write something that just return the screen dimensions as integers...
	screenInfo.iSize = sizeof(screenInfo);

	gEngfuncs.pfnGetScreenInfo(&screenInfo);

	m_ScreenDim.x = screenInfo.iWidth;
	m_ScreenDim.y = screenInfo.iHeight;
	m_ScreenHalfDim = m_ScreenDim / 2;

	CalculateScopeSpriteParameters();
	ConstructGeometry();
}

void CSniperScopeOverlay::CalculateScopeSpriteParameters()
{
	const size_t shortEdge = m_ScreenDim.y < m_ScreenDim.x ? m_ScreenDim.y : m_ScreenDim.x;
	const size_t scopeImageDim = static_cast<size_t>(static_cast<float>(shortEdge) * SCREEN_SHORTEDGE_FACTOR);

	m_ScopeImageDim = UIVec2(scopeImageDim, scopeImageDim);

	// We do dim + 1 to make sure that the image position is always closer to the top left if the dimension is odd.
	const size_t scopeImageHalfDim = (scopeImageDim + 1) / 2;
	m_ScopeImagePos = m_ScreenHalfDim - UIVec2(scopeImageHalfDim, scopeImageHalfDim);
}

void CSniperScopeOverlay::ConstructGeometry()
{
	ConstructSurroundingBlocks();
}

CustomGeometry::GeometryItemPtr_t CSniperScopeOverlay::CreateNewGeometryItem(CustomGeometry::DrawType drawType)
{
	CustomGeometry::GeometryItemPtr_t item(new CustomGeometry::CGeometryItem());
	item->SetColour(0x000000FF);
	item->SetDrawType(drawType);

	return item;
}

void CSniperScopeOverlay::Draw(float time)
{
	DrawScopeBackgroundSprite();
	CustomGeometry::RenderAdHocGeometry(m_SurroundingBlocks);
}

void CSniperScopeOverlay::DrawScopeBackgroundSprite()
{
	SPR_Set(m_hScopeImage, 255, 255, 255);

	sprite_draw_args_t args;
	memset(&args, 0, sizeof(args));

	args.x = m_ScopeImagePos.x;
	args.y = m_ScopeImagePos.y;
	args.width = m_ScopeImageDim.x;
	args.height = m_ScopeImageDim.y;
	args.renderMode = kRenderTransTexture;

	SPR_DrawCustom(&args);
}

void CSniperScopeOverlay::ConstructSurroundingBlocks()
{
	m_SurroundingBlocks = CreateNewGeometryItem(CustomGeometry::DrawType::TriangleStrip);

	UIVec2 points[8];

	// Points are added in the correct order for a triangle strip that surrounds the scope sprite.
	// The strip begins at the top left and alternates between outer and inner points, proceeding clockwise.
	points[0] = UIVec2(0,0);
	points[2] = UIVec2(m_ScreenDim.x, 0);
	points[4] = m_ScreenDim;
	points[6] = UIVec2(0, m_ScreenDim.y);

	points[1] = m_ScopeImagePos;
	points[3] = m_ScopeImagePos + UIVec2(m_ScopeImageDim.x, 0);
	points[5] = m_ScopeImagePos + m_ScopeImageDim;
	points[7] = m_ScopeImagePos + UIVec2(0, m_ScopeImageDim.y);

	// Add points and indices to the geometry.
	for ( uint8_t index = 0; index < CoreUtil::ArraySize(points); ++index )
	{
		m_SurroundingBlocks->AddPoint(Vector(points[index].x, points[index].y, 0));
		m_SurroundingBlocks->AddIndex(index);
	}

	// We need two extra indices to form the last two triangles.
	m_SurroundingBlocks->AddIndex(0);
	m_SurroundingBlocks->AddIndex(1);
}
