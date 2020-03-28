#include <memory>
#include "geometryStatics.h"
#include "cl_dll.h"
#include "customGeometry/geometryCollectionManager.h"
#include "customGeometry/geometryRenderer.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/messageReader.h"
#include "projectInterface/IProjectInterface.h"
#include "customGeometry/logger_client.h"

namespace CustomGeometry
{
	static std::unique_ptr<CGeometryRenderer> AdHocRenderer;

	static void HandleSuccessfullyReceivedMessage(const CMessageReader& reader)
	{
		if ( reader.GetLastReadResult() == CMessageReader::ReadResult::Clear &&
			 reader.GetGeometryCategory() == Category::None )
		{
			CL_LOG().LogF(ILogInterface::Level::Message, "Received custom geometry ClearAll message.\n");
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
				GeometryItemPtr_t item = reader.GetGeometryItem();

				CL_LOG().LogF(ILogInterface::Level::Message,
					"Received custom geometry for category %s (%d points, %d indices)\n",
					CustomGeometry::CategoryName(reader.GetGeometryCategory()),
					item->GetPoints().Count(),
					item->GetIndices().Count());

				collection->AddItem(item);
				break;
			}

			case CMessageReader::ReadResult::Clear:
			{
				CL_LOG().LogF(ILogInterface::Level::Message,
					"Received custom geometry clear message for category %s\n",
					CustomGeometry::CategoryName(reader.GetGeometryCategory()));

				collection->Clear();
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

	void Initialise()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		// Specify any custom geometry collection factory functions before this call, via manager.SetFactoryFunction().
		// This is used for providing the manager with custom CBaseGeometryCollection subclasses.
		manager.Initialise();
		AdHocRenderer.reset(new CGeometryRenderer());

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

	void RenderAdHocGeometry(const GeometryItemPtr_t& item)
	{
		if ( AdHocRenderer && item )
		{
			AdHocRenderer->Render(*item);
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

			collection->Clear();
		}
	}
}
