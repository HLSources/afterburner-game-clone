#include "geometryStatics.h"
#include "cl_dll.h"
#include "customGeometry/geometryCollectionManager.h"
#include "customGeometry/geometryRenderer.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/messageReader.h"
#include "projectInterface/IProjectInterface.h"

namespace CustomGeometry
{
	static void HandleSuccessfullyReceivedMessage(const CMessageReader& reader)
	{
		if ( reader.GetLastReadResult() == CMessageReader::ReadResult::Clear &&
			 reader.GetGeometryCategory() == Category::None )
		{
			ClearAllGeometry();
			return;
		}

		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();
		CBaseGeometryCollection* collection = manager.CollectionForCategory(reader.GetGeometryCategory());

		if ( !collection )
		{
			return;
		}

		switch ( reader.GetLastReadResult() )
		{
			case CMessageReader::ReadResult::OK:
			{
				collection->ItemReceived(reader.GetGeometryItem());
				break;
			}

			case CMessageReader::ReadResult::Clear:
			{
				collection->ClearMessageReceived();
				break;
			}

			default:
			{
				ASSERT(false);
				break;
			}
		}
	}

	static int HandleCustomGeometryMessage(const char* msgName, int size, void* buffer)
	{
		CMessageReader reader;

		if ( reader.ReadMessage(buffer, size) != CMessageReader::ReadResult::Error )
		{
			HandleSuccessfullyReceivedMessage(reader);
		}
		else
		{
			ILogInterface& log = IProjectInterface::ProjectInterfaceImpl()->LogInterface();
			log.Log(ILogInterface::Level::Error, "Failed to parse custom geometry message.\n");
		}

		return 1;
	}

	void Init()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		// Specify any custom geometry collection factory functions before this call, via manager.SetFactoryFunction().
		// This is used for providing the manager with custom CBaseGeometryCollection subclasses.
		manager.Initialise();

		gEngfuncs.pfnHookUserMsg(MESSAGE_NAME, &HandleCustomGeometryMessage);
	}

	void RenderAllGeometry()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CBaseGeometryCollection* collection = manager.CollectionForCategory(static_cast<Category>(index));

			if ( !collection )
			{
				continue;
			}

			collection->Render();
		}
	}

	void ClearAllGeometry()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CBaseGeometryCollection* collection = manager.CollectionForCategory(static_cast<Category>(index));

			if ( !collection )
			{
				continue;
			}

			collection->ClearMessageReceived();
		}
	}
}
