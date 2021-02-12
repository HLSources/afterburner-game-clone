#include "hud_crosshair.h"
#include "customGeometry/geometryStatics.h"

namespace
{
	// Crosshair bars are added in this order.
	// The first two points of each bar are inner, and the second two are outer.
	enum CrosshairBar
	{
		TopBar = 0,
		BottomBar,
		LeftBar,
		RightBar
	};

	static constexpr size_t BAR_HALF_WIDTH = 1;

	inline constexpr uint8_t PointOffset(CrosshairBar bar)
	{
		return 4 * bar;
	}
}

int CHudCrosshair::Init()
{
	m_CrosshairGeometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
	m_iFlags |= HUD_ACTIVE;
	m_CrosshairCvar = gEngfuncs.pfnGetCvarPointer("crosshair");

	gHUD.AddHudElem(this);
	return 1;
}

int CHudCrosshair::VidInit()
{
	SCREENINFO screenInfo;

	memset(&screenInfo, 0, sizeof(screenInfo));
	screenInfo.iSize = sizeof(screenInfo);
	gEngfuncs.pfnGetScreenInfo(&screenInfo);

	m_Params.SetScreenDimensions(UIVec2(screenInfo.iWidth, screenInfo.iHeight));
	m_CrosshairGeometry->SetColour(0xFF0000FF);
	m_CrosshairGeometry->SetDrawType(CustomGeometry::DrawType::Triangles);

	return 1;
}

void CHudCrosshair::Reset()
{
}

int CHudCrosshair::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL) || !m_CrosshairCvar || m_CrosshairCvar->value != 2 )
	{
		return 1;
	}

	UpdateParameters();
	UpdateGeometry();

	CustomGeometry::RenderAdHocGeometry(m_CrosshairGeometry);
	return 1;
}

void CHudCrosshair::UpdateParameters()
{
	m_Params.SetWeaponInaccuracy(static_cast<float>(gHUD.m_iWeaponInaccuracy) / 255.0f);

	// TODO: We need to use attributes from the active weapon here.
	// For debugging purposes, use these for now.
	m_InnerRadius = 0.05f + (0.1f * m_Params.WeaponInaccuracy());
	m_OuterRadius = 0.1f + m_InnerRadius;
}

void CHudCrosshair::InitialiseGeometry()
{
	if ( !m_CrosshairGeometry )
	{
		return;
	}

	m_CrosshairGeometry->ClearGeometry();
	const UIVec2 screenCentre = m_Params.HalfScreenDimensions();

	// Compute crosshair spokes. The positions will be manually modified later
	// according to the parameters we have.
	// The first two points of each bar are inner, and the second two are outer.

	// Top
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y - 1, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y - 1, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y - 2, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y - 2, 0)
	);

	// Bottom
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y + 1, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y + 1, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y + 2, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y + 2, 0)
	);

	// Left
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x - 1, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 1, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 2, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 2, screenCentre.y + BAR_HALF_WIDTH, 0)
	);

	// Right
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x + 1, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 1, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 2, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 2, screenCentre.y - BAR_HALF_WIDTH, 0)
	);
}

void CHudCrosshair::UpdateGeometry()
{
	if ( !m_CrosshairGeometry )
	{
		return;
	}

	if ( m_CrosshairGeometry->GetPointCount() != 4 * 4 )
	{
		InitialiseGeometry();
	}

	const UIVec2 screenCentre = m_Params.HalfScreenDimensions();
	const int innerDisp = m_Params.DisplacementFromScreenCentre(m_InnerRadius);
	const int outerDisp = m_Params.DisplacementFromScreenCentre(m_OuterRadius);

	for ( uint8_t bar = 0; bar < 4; ++bar )
	{
		const uint8_t base = PointOffset(static_cast<CrosshairBar>(bar));

		switch ( bar )
		{
			case TopBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).y = screenCentre.y - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).y = screenCentre.y - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).y = screenCentre.y - outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).y = screenCentre.y - outerDisp;
				break;
			}

			case BottomBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).y = screenCentre.y + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).y = screenCentre.y + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).y = screenCentre.y + outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).y = screenCentre.y + outerDisp;
				break;
			}

			case LeftBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).x = screenCentre.x - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).x = screenCentre.x - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).x = screenCentre.x - outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).x = screenCentre.x - outerDisp;
				break;
			}

			case RightBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).x = screenCentre.x + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).x = screenCentre.x + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).x = screenCentre.x + outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).x = screenCentre.x + outerDisp;
				break;
			}
		}
	}
}
