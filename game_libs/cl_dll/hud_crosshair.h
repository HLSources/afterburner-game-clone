#pragma once

#include "hud.h"
#include "customGeometry/geometryItem.h"
#include "ui/core/uiVec2.h"

class CHudCrosshair : public CHudBase
{
public:
	virtual ~CHudCrosshair() = default;

	int Init();
	int VidInit();
	void Reset();
	int Draw(float flTime);

private:
	UIVec2 m_ScreenDim;
	UIVec2 m_ScreenHalfDim;
	CustomGeometry::GeometryItemPtr_t m_CrosshairGeometry;
};
