#include "hud_crosshair.h"
#include "customGeometry/geometryStatics.h"

int CHudCrosshair::Init()
{
	m_CrosshairGeometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());

	gHUD.AddHudElem(this);
	return 1;
}

int CHudCrosshair::VidInit()
{
	SCREENINFO screenInfo;

	memset(&screenInfo, 0, sizeof(screenInfo));
	screenInfo.iSize = sizeof(screenInfo);
	gEngfuncs.pfnGetScreenInfo(&screenInfo);

	m_ScreenDim.x = screenInfo.iWidth;
	m_ScreenDim.y = screenInfo.iHeight;

	// We take away 1 so that the screen centre matches what the engine crosshair sprites actually use.
	m_ScreenHalfDim = (m_ScreenDim / 2) - UIVec2(1,1);

	m_CrosshairGeometry->AddTriangleQuad(
		Vector(m_ScreenHalfDim.x - 10, m_ScreenHalfDim.y, 0),
		Vector(m_ScreenHalfDim.x + 10, m_ScreenHalfDim.y, 0),
		Vector(m_ScreenHalfDim.x + 10, m_ScreenHalfDim.y - 20, 0),
		Vector(m_ScreenHalfDim.x - 10, m_ScreenHalfDim.y - 20, 0)
	);

	m_CrosshairGeometry->SetColour(0x00FF0000);
	m_CrosshairGeometry->SetDrawType(CustomGeometry::DrawType::Triangles);
	m_CrosshairGeometry->SetScale(1.0f);

	return 1;
}

void CHudCrosshair::Reset()
{
}

int CHudCrosshair::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL) )
	{
		return 1;
	}

	CustomGeometry::RenderAdHocGeometry(m_CrosshairGeometry);

	return 1;
}
