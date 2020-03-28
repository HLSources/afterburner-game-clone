#pragma once

#include "BaseScreenOverlay.h"
#include "standard_includes.h"
#include "cdll_int.h"

class CSniperScopeOverlay : public CBaseScreenOverlay
{
public:
	static constexpr ScreenOverlays::OverlayId OVERLAY_ID = ScreenOverlays::Overlay_SniperScope;

	virtual ~CSniperScopeOverlay();

	virtual ScreenOverlays::OverlayId Id() const override;
	virtual void Precache() override;
	virtual void VidInit() override;
	virtual void Draw(float time) override;

private:
	HSPRITE m_hScopeImage = 0;
};
