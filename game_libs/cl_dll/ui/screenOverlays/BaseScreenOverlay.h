#pragma once

#include "ScreenOverlayIds.h"

class CBaseScreenOverlay
{
public:
	virtual ~CBaseScreenOverlay();

	// Unique ID for this overlay.
	virtual ScreenOverlays::OverlayId Id() const = 0;

	// For precaching resources, eg. loading sprites.
	virtual void Precache();

	// Called when video settings change, eg. screen resolution.
	// Construction of relevant screen geometry should happen here.
	virtual void VidInit();

	// Called when the overlay should be drawn.
	// Intended for drawing contents that have been constructed in VidInit.
	virtual void Draw(float time);
};
