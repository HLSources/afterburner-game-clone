#include "geometryStatics.h"
#include "cl_dll.h"
#include "customGeometry/geometryCollection.h"
#include "customGeometry/geometryRenderer.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/messageReader.h"
#include "projectInterface/IProjectInterface.h"

namespace CustomGeometry
{
	static int HandleCustomGeometryMessage(const char* msgName, int size, void* buffer)
	{
		GeometryItemPtr_t item(new CGeometryItem());
		CMessageReader reader;

		switch ( reader.ReadMessage(buffer, size, *item) )
		{
			case CMessageReader::ReadResult::OK:
			{
				CGeometryCollection::StaticInstance().AddItem(item);
				break;
			}

			case CMessageReader::ReadResult::Clear:
			{
				CGeometryCollection::StaticInstance().Clear();
				break;
			}

			default:
			{
				ILogInterface& log = IProjectInterface::ProjectInterfaceImpl()->LogInterface();
				log.Log(ILogInterface::Level::Error, "Failed to parse custom geometry message.\n");
				break;
			}
		}

		return 1;
	}

	void Init()
	{
		gEngfuncs.pfnHookUserMsg(MESSAGE_NAME, &HandleCustomGeometryMessage);
	}

	void RenderAll()
	{
		CGeometryCollection& collection = CGeometryCollection::StaticInstance();
		const size_t count = collection.Count();

		for ( uint32_t index = 0; index < count; ++index )
		{
			GeometryItemPtr_t item = collection.ItemAt(index);

			if ( !item )
			{
				continue;
			}

			CGeometryRenderer renderer;
			renderer.Render(*item);
		}
	}
}
