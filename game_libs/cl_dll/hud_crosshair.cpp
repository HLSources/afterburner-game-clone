#include "hud_crosshair.h"
#include "customGeometry/geometryStatics.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "util/extramath.h"

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
	static constexpr const char CL_DEBUG_WEAPON_SPREAD[] = "cl_debug_weapon_spread";

	inline constexpr uint8_t PointOffset(CrosshairBar bar)
	{
		return 4 * bar;
	}
}

int CHudCrosshair::Init()
{
	m_CrosshairGeometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
	m_iFlags |= HUD_ACTIVE;

	m_CheatsCvar = gEngfuncs.pfnGetCvarPointer("sv_cheats");
	m_CrosshairCvar = gEngfuncs.pfnGetCvarPointer("crosshair");
	m_DebugSpreadCvar = gEngfuncs.pfnGetCvarPointer(CL_DEBUG_WEAPON_SPREAD);

	if ( !m_DebugSpreadCvar )
	{
		m_DebugSpreadCvar = gEngfuncs.pfnRegisterVariable(CL_DEBUG_WEAPON_SPREAD, "0", FCVAR_CLIENTDLL);
	}

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

	if ( !UpdateParameters() )
	{
		return 1;
	}

	UpdateGeometry();
	CustomGeometry::RenderAdHocGeometry(m_CrosshairGeometry);

	if ( m_CheatsCvar &&
		 m_CheatsCvar->value &&
		 m_DebugSpreadCvar &&
		 static_cast<int>(m_DebugSpreadCvar->value) > 0 )
	{
		m_SpreadVisualiser.Draw(m_Params, static_cast<size_t>(m_DebugSpreadCvar->value) - 1);
	}

	return 1;
}

bool CHudCrosshair::UpdateParameters()
{
	WEAPON* weapon = gHUD.m_Ammo.GetCurrentWeapon();

	if ( !weapon || weapon->iId < 1 )
	{
		return false;
	}

	m_Params.SetWeaponID(static_cast<WeaponId_e>(weapon->iId));

	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(m_Params.WeaponID());

	if ( !atts )
	{
		return false;
	}

	m_CrosshairAtts = &atts->Crosshair;

	if ( !m_CrosshairAtts->HasCrosshair )
	{
		return false;
	}

	m_Params.SetWeaponInaccuracy(static_cast<float>(gHUD.m_iWeaponInaccuracy) / 255.0f);

	// At inaccuracy 0, radius is m_CrosshairAtts->RadiusMin.
	// At inaccuracy 1, radius is m_CrosshairAtts->RadiusMax.
	const float radius = ExtraMath::RemapClamped(m_Params.WeaponInaccuracy(), 0, 1, m_CrosshairAtts->RadiusMin, m_CrosshairAtts->RadiusMax);
	m_Params.SetRadius(radius);

	// At inaccuracy 0, bar length is m_CrosshairAtts->BarScaleMin.
	// At inaccuracy 1, bar length is m_CrosshairAtts->BarScaleMax.
	const float barLength = ExtraMath::RemapClamped(m_Params.WeaponInaccuracy(), 0, 1, m_CrosshairAtts->BarScaleMin, m_CrosshairAtts->BarScaleMax);
	m_Params.SetBarLength(barLength);

	return true;
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
	const int innerDisp = m_Params.DisplacementFromScreenCentre(m_Params.Radius());
	const int outerDisp = m_Params.DisplacementFromScreenCentre(m_Params.Radius() + m_Params.BarLength());

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
