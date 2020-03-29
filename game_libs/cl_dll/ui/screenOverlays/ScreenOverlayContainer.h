#pragma once

#include <memory>
#include "ScreenOverlayIds.h"
#include "BaseScreenOverlay.h"
#include "utlvector.h"

class CScreenOverlayContainer
{
public:
	static CScreenOverlayContainer& StaticInstance();

	CScreenOverlayContainer();

	// This is called once on client initialisation.
	// Any overlays supported in the game should be registered within this function.
	void RegisterOverlays();

	// Called whenever the video settings change, eg. monitor resolution.
	// All existing overlays are informed.
	void VidInit();

	// Called when the currently active overlay should be drawn.
	void DrawCurrentOverlay(float time);

	// The source is a string used to uniquely identify the caller that
	// placed this overlay. It should be compile-time constant and should not be null.
	bool PushOverlayOntoStack(const char* source, ScreenOverlays::OverlayId id);
	bool PopOverlayFromStack(const char* source);
	void ClearOverlayStack();

private:
	static constexpr size_t MAX_OVERLAY_STACK_DEPTH = 16;

	using FactoryFunc = CBaseScreenOverlay* (*)(void);
	using ScreenOverlayPtr = std::unique_ptr<CBaseScreenOverlay>;

	struct StackItem
	{
		const char* source;
		ScreenOverlays::OverlayId id;
	};

	template<typename T>
	inline void MapIdToClass()
	{
		volatile ScreenOverlays::OverlayId id = T::OVERLAY_ID;
		if ( id > ScreenOverlays::Overlay_None && id < ScreenOverlays::Overlay__Count )
		{
			m_FactoryFunctions[id] = [](){ return static_cast<CBaseScreenOverlay*>(new T()); };
		}
	}

	template<typename FUNC>
	inline void ForEachValidOverlay(FUNC callback)
	{
		using namespace ScreenOverlays;

		for ( OverlayId id = static_cast<OverlayId>(Overlay_None + 1);
			  id < Overlay__Count;
			  id = static_cast<OverlayId>(id + 1) )
		{
			CBaseScreenOverlay* overlay = GetOverlay(id);

			if ( overlay )
			{
				callback(id, overlay);
			}
		}
	}

	void CreateAllOverlays();
	CBaseScreenOverlay* GetOverlay(ScreenOverlays::OverlayId id);
	CBaseScreenOverlay* GetCurrentOverlayFromStack();

	ScreenOverlayPtr m_Overlays[ScreenOverlays::Overlay__Count];
	FactoryFunc m_FactoryFunctions[ScreenOverlays::Overlay__Count];
	CUtlVector<StackItem> m_OverlayStack;
};
