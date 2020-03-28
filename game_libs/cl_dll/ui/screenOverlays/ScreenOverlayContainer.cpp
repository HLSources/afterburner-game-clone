#include "ScreenOverlayContainer.h"

#include "SniperScopeOverlay.h"

CScreenOverlayContainer& CScreenOverlayContainer::StaticInstance()
{
	static CScreenOverlayContainer container;
	return container;
}

CScreenOverlayContainer::CScreenOverlayContainer()
{
	memset(m_FactoryFunctions, 0, sizeof(m_FactoryFunctions));
}

void CScreenOverlayContainer::RegisterOverlays()
{
	MapIdToClass<CSniperScopeOverlay>();
}

void CScreenOverlayContainer::Precache()
{
	ForEachValidOverlay([](ScreenOverlays::OverlayId id, CBaseScreenOverlay* overlay)
	{
		overlay->Precache();
	},
	true);
}

void CScreenOverlayContainer::VidInit()
{
	ForEachValidOverlay([](ScreenOverlays::OverlayId id, CBaseScreenOverlay* overlay)
	{
		overlay->VidInit();
	});
}

void CScreenOverlayContainer::DrawCurrentOverlay(float time)
{
	CBaseScreenOverlay* overlay = GetOverlay(m_CurrentOverlay);

	if ( overlay )
	{
		overlay->Draw(time);
	}
}

void CScreenOverlayContainer::SetCurrentOverlay(ScreenOverlays::OverlayId id)
{
	if ( id < ScreenOverlays::Overlay__Count )
	{
		m_CurrentOverlay = id;
	}
}

void CScreenOverlayContainer::ResetCurrentOverlay()
{
	m_CurrentOverlay = ScreenOverlays::Overlay_None;
}

CBaseScreenOverlay* CScreenOverlayContainer::GetOverlay(ScreenOverlays::OverlayId id, bool create)
{
	if ( id <= ScreenOverlays::Overlay_None || id >= ScreenOverlays::Overlay__Count )
	{
		return nullptr;
	}

	if ( create && m_FactoryFunctions[id] )
	{
		m_Overlays[id].reset(m_FactoryFunctions[id]());
	}

	return m_Overlays[id].get();
}
