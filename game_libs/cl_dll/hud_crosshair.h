#pragma once

#include "hud.h"
#include "customGeometry/geometryItemPtr.h"
#include "ui/crosshair/crosshairParameters.h"
#include "weapons/weaponids.h"
#include "weaponattributes/weaponatts_crosshair.h"
#include "ui/crosshair/spreadVisualiser.h"

class CHudCrosshair : public CHudBase
{
public:
	virtual ~CHudCrosshair() = default;

	int Init();
	int VidInit();
	void Reset();
	int Draw(float flTime);

private:
	bool UpdateParameters();
	void InitialiseGeometry();
	void UpdateGeometry();

	cvar_t* m_CheatsCvar = nullptr;
	cvar_t* m_CrosshairCvar = nullptr;
	cvar_t* m_DebugSpreadCvar = nullptr;

	const WeaponAtts::WACrosshair* m_CrosshairAtts = nullptr;

	CustomGeometry::GeometryItemPtr_t m_CrosshairGeometry;
	CCrosshairParameters m_Params;
	CSpreadVisualiser m_SpreadVisualiser;
};
