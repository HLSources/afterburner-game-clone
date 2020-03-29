#include <cstring>
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
	CBaseScreenOverlay* overlay = GetCurrentOverlayFromStack();

	if ( overlay )
	{
		overlay->Draw(time);
	}
}

bool CScreenOverlayContainer::PushOverlayOntoStack(const char* source, ScreenOverlays::OverlayId id)
{
	if ( !source ||
		 id <= ScreenOverlays::Overlay_None ||
		 id >= ScreenOverlays::Overlay__Count ||
		 m_OverlayStack.Count() >= MAX_OVERLAY_STACK_DEPTH )
	{
		return false;
	}

	m_OverlayStack.AddToTail({source, id});
	return true;
}

bool CScreenOverlayContainer::PopOverlayFromStack(const char* source)
{
	if ( !source )
	{
		return false;
	}

	int foundIndex = -1;

	for ( int index = 0; index < m_OverlayStack.Count(); ++index )
	{
		if ( strcmp(source, m_OverlayStack[index].source) == 0 )
		{
			foundIndex = index;
			break;
		}
	}

	if ( foundIndex < 0 )
	{
		return false;
	}

	m_OverlayStack.Remove(foundIndex);
	return true;
}

void CScreenOverlayContainer::ClearOverlayStack()
{
	m_OverlayStack.Purge();
}

CBaseScreenOverlay* CScreenOverlayContainer::GetOverlay(ScreenOverlays::OverlayId id)
{
	if ( id > ScreenOverlays::Overlay_None && id < ScreenOverlays::Overlay__Count )
	{
		return m_Overlays[id].get();
	}

	return nullptr;
}

CBaseScreenOverlay* CScreenOverlayContainer::GetCurrentOverlayFromStack()
{
	if ( m_OverlayStack.Count() < 1 )
	{
		return nullptr;
	}

	return GetOverlay(m_OverlayStack[m_OverlayStack.Count() - 1].id);
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
