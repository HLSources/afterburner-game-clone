#pragma once

#include "hud.h"
#include "customGeometry/geometryItemPtr.h"
#include "ui/crosshair/crosshairParameters.h"
#include "weapons/weaponids.h"
#include "weaponattributes/weaponatts_crosshair.h"

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

	WeaponId_e m_CurrentWeaponID = WeaponId_e::WeaponNone;
	const WeaponAtts::WACrosshair* m_CrosshairAtts = nullptr;

	cvar_t* m_CrosshairCvar = nullptr;
	CustomGeometry::GeometryItemPtr_t m_CrosshairGeometry;
	CCrosshairParameters m_Params;
};
