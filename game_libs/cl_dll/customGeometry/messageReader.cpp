#include "messageReader.h"
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "parsemsg.h"

namespace CustomGeometry
{
	CMessageReader::ReadResult CMessageReader::GetLastReadResult() const
	{
		return m_ReadResult;
	}

	GeometryItemPtr_t CMessageReader::GetGeometryItem() const
	{
		return m_GeometryItem;
	}

	Category CMessageReader::GetGeometryCategory() const
	{
		return m_GeomCategory;
	}

	CMessageReader::ReadResult CMessageReader::ReadMessage(void* buffer, int size)
	{
		// Invalidate to start off with.
		SetErrorState();

		// If anything goes wrong in the process, invalidate before returning.
		if ( !ReadMessageInternal(buffer, size) )
		{
			SetErrorState();
		}

		return m_ReadResult;
	}

	bool CMessageReader::ReadMessageInternal(void* buffer, int size)
	{
		if ( !buffer || size < 1 )
		{
			return false;
		}

		BEGIN_READ(buffer, size);

		if ( !ReadGeometryCategory() )
		{
			return false;
		}

		const uint8_t drawType = static_cast<uint8_t>(READ_BYTE());

		if ( drawType == static_cast<uint8_t>(DrawType::None) )
		{
			m_ReadResult = ReadResult::Clear;

			// Nothing else to read.
			return true;
		}

		m_GeometryItem.reset(new CGeometryItem());

		switch ( static_cast<DrawType>(drawType) )
		{
			case DrawType::Lines:
			{
				m_GeometryItem->SetDrawType(static_cast<DrawType>(drawType));
				break;
			}

			default:
			{
				return false;
			}
		}

		m_GeometryItem->SetColour(READ_LONG());

		const uint16_t pointCount = static_cast<uint16_t>(READ_SHORT());

		if ( pointCount > MAX_POINTS_PER_MSG )
		{
			return false;
		}

		for ( uint32_t index = 0; index < pointCount; ++index )
		{
			Vector vec;
			READ_VEC_PRECISE(vec);
			m_GeometryItem->AddPoint(vec);
		}

		const uint16_t indexCount = static_cast<uint16_t>(READ_SHORT());

		if ( indexCount > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		for ( uint32_t index = 0; index < indexCount; ++index )
		{
			m_GeometryItem->AddIndex(static_cast<uint8_t>(READ_CHAR()));
		}

		m_ReadResult = ReadResult::OK;
		return true;
	}

	bool CMessageReader::ReadGeometryCategory()
	{
		const uint8_t categoryInt = static_cast<uint8_t>(READ_BYTE());

		if ( categoryInt >= CATEGORY_COUNT )
		{
			SetErrorState();
			return false;
		}

		m_GeomCategory = static_cast<Category>(categoryInt);
		return true;
	}

	void CMessageReader::SetErrorState()
	{
		m_GeometryItem.reset();
		m_GeomCategory = Category::None;
		m_ReadResult = ReadResult::Error;
	}
}
