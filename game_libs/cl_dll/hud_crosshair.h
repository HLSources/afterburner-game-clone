#pragma once

#include "hud.h"
#include "customGeometry/geometryItemPtr.h"
#include "ui/crosshair/crosshairparameters.h"

class CHudCrosshair : public CHudBase
{
public:
	virtual ~CHudCrosshair() = default;

	int Init();
	int VidInit();
	void Reset();
	int Draw(float flTime);

private:
	void UpdateParameters();
	void InitialiseGeometry();
	void UpdateGeometry();

	cvar_t* m_CrosshairCvar = nullptr;
	CustomGeometry::GeometryItemPtr_t m_CrosshairGeometry;

	CCrosshairParameters m_Params;

	// TODO: Later these should be factored out into crosshair variants
	float m_InnerRadius = 0.0f;
	float m_OuterRadius = 0.0f;
};
