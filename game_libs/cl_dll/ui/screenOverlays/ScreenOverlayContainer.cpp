#include "ScreenOverlayContainer.h"

#include "SniperScopeOverlay.h"

namespace ScreenOverlays
{
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

		CreateAllOverlays();
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

	CBaseScreenOverlay* CScreenOverlayContainer::GetOverlay(ScreenOverlays::OverlayId id)
	{
		if ( id > ScreenOverlays::Overlay_None && id < ScreenOverlays::Overlay__Count )
		{
			return m_Overlays[id].get();
		}

		return nullptr;
	}

	void CScreenOverlayContainer::CreateAllOverlays()
	{
		using namespace ScreenOverlays;

		for ( OverlayId id = static_cast<OverlayId>(Overlay_None + 1);
			id < Overlay__Count;
			id = static_cast<OverlayId>(id + 1) )
		{
			if ( m_FactoryFunctions[id] )
			{
				m_Overlays[id].reset(m_FactoryFunctions[id]());
			}
		}
	}
}
