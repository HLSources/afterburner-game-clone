#include "messageReader.h"
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "parsemsg.h"

namespace CustomGeometry
{
	CMessageReader::ReadResult CMessageReader::ReadMessage(void* buffer, int size, CGeometryItem& item)
	{
		item.Clear();

		if ( !buffer || size < 1 )
		{
			return ReadResult::Error;
		}

		BEGIN_READ(buffer, size);

		const uint8_t drawType = static_cast<uint8_t>(READ_BYTE());

		if ( drawType == static_cast<uint8_t>(DrawType::None) )
		{
			return ReadResult::Clear;
		}

		switch ( static_cast<DrawType>(drawType) )
		{
			case DrawType::Lines:
			{
				item.SetDrawType(static_cast<DrawType>(drawType));
				break;
			}

			default:
			{
				item.Clear();
				return ReadResult::Error;
			}
		}

		item.SetColour(READ_LONG());

		const uint16_t pointCount = static_cast<uint16_t>(READ_SHORT());

		if ( pointCount > MAX_POINTS_PER_MSG )
		{
			item.Clear();
			return ReadResult::Error;
		}

		for ( uint32_t index = 0; index < pointCount; ++index )
		{
			Vector vec;

			for ( uint32_t coOrdAxis = 0; coOrdAxis < 3; ++coOrdAxis )
			{
				float value = 0;
				uint8_t* data = reinterpret_cast<uint8_t*>(&value);

				for ( uint32_t dataIndex = 0; dataIndex < sizeof(float); ++dataIndex )
				{
					data[dataIndex] = static_cast<uint8_t>(READ_BYTE());
				}

				vec[coOrdAxis] = value;
			}

			item.AddPoint(vec);
		}

		return ReadResult::OK;
	}
}
