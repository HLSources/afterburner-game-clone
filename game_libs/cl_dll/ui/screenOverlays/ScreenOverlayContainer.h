#pragma once

#include <memory>
#include "ScreenOverlayIds.h"
#include "BaseScreenOverlay.h"

class CScreenOverlayContainer
{
public:
	using FactoryFunc = CBaseScreenOverlay* (*)(void);

	static CScreenOverlayContainer& StaticInstance();

	CScreenOverlayContainer();

	// This is called once on client initialisation.
	// Any overlays supported in the game should be registered within this function.
	void RegisterOverlays();

	// Called when the client connects to a server.
	// All overlays are recreated and have their own Precache() functions called.
	void Precache();

	// Called whenever the video settings change, eg. monitor resolution.
	// All existing overlays are informed.
	void VidInit();

	// Called when the currently active overlay should be drawn.
	void DrawCurrentOverlay(float time);

	void SetCurrentOverlay(ScreenOverlays::OverlayId id);
	void ResetCurrentOverlay();

private:
	using ScreenOverlayPtr = std::unique_ptr<CBaseScreenOverlay>;

	template<typename T>
	inline void MapIdToClass()
	{
		if ( T::OVERLAY_ID > ScreenOverlays::Overlay_None && T::OVERLAY_ID < ScreenOverlays::Overlay__Count )
		{
			m_FactoryFunctions[T::OVERLAY_ID] = [](){ return static_cast<CBaseScreenOverlay*>(new T()); };
		}
	}

	template<typename FUNC>
	inline void ForEachValidOverlay(FUNC callback, bool create = false)
	{
		using namespace ScreenOverlays;

		for ( OverlayId id = static_cast<OverlayId>(Overlay_None + 1);
			  id < Overlay__Count;
			  id = static_cast<OverlayId>(id + 1) )
		{
			CBaseScreenOverlay* overlay = GetOverlay(id, create);

			if ( overlay )
			{
				callback(id, overlay);
			}
		}
	}

	CBaseScreenOverlay* GetOverlay(ScreenOverlays::OverlayId id, bool create = false);

	ScreenOverlayPtr m_Overlays[ScreenOverlays::Overlay__Count];
	FactoryFunc m_FactoryFunctions[ScreenOverlays::Overlay__Count];
	ScreenOverlays::OverlayId m_CurrentOverlay = ScreenOverlays::Overlay_None;
};
