#include "messageWriter.h"

namespace CustomGeometry
{
	int CMessageWriter::m_iMessageId = 0;

	void CMessageWriter::RegisterUserMessage()
	{
		if ( m_iMessageId != 0 )
		{
			return;
		}

		m_iMessageId = REG_USER_MSG(MESSAGE_NAME, -1);
	}

	CMessageWriter::CMessageWriter(Category category) :
		m_Category(Category::None),
		m_pTargetClient(nullptr)
	{
		SetCategory(category);
	}

	Category CMessageWriter::GetCategory() const
	{
		return m_Category;
	}

	bool CMessageWriter::SetCategory(Category category)
	{
		if ( !IsValidSpecificCategory(category) )
		{
			return false;
		}

		m_Category = category;
		return true;
	}

	CBasePlayer* CMessageWriter::GetTargetClient() const
	{
		return m_pTargetClient;
	}

	void CMessageWriter::SetTargetClient(CBasePlayer* client)
	{
		if ( !client || client->pev->flags | FL_FAKECLIENT || !(client->pev->flags | FL_CLIENT) )
		{
			return;
		}

		m_pTargetClient = client;
	}

	void CMessageWriter::ClearTargetClient()
	{
		m_pTargetClient = nullptr;
	}

	// Message format:
	// - Header
	// - Colour (uint32)
	// - Point count (uint16)
	// - Points (float) [3 * point count]
	bool CMessageWriter::WriteMessage(const CGeometryItem& geometry)
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			return false;
		}

		const CUtlVector<Vector>& points = geometry.GetPoints();

		if ( points.Count() < 1 )
		{
			return false;
		}

		if ( points.Count() > MAX_POINTS_PER_MSG )
		{
			ALERT(at_warning, "Attempted to send custom geometry message with %d points when max point count is %u.\n",
				  points.Count(),
				  MAX_POINTS_PER_MSG);
			return false;
		}

		WriteMessageBegin();
		WriteMessageHeader(m_Category, geometry.GetDrawType());

		WRITE_LONG(geometry.GetColour());
		WRITE_SHORT(static_cast<uint16_t>(points.Count()));

		FOR_EACH_VEC(points, index)
		{
			WRITE_VEC_PRECISE(points[index]);
		}

		MESSAGE_END();
		return true;
	}

	bool CMessageWriter::WriteClearMessage()
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			return false;
		}

		WriteClearMessageInternal(m_Category);
		return true;
	}

	void CMessageWriter::WriteMessageBegin()
	{
		if ( m_pTargetClient )
		{
			MESSAGE_BEGIN(MSG_ONE, m_iMessageId, nullptr, m_pTargetClient->pev);
		}
		else
		{
			MESSAGE_BEGIN(MSG_ALL, m_iMessageId);
		}
	}

	// Message format:
	// - Header only; draw type is None.
	void CMessageWriter::WriteClearMessageInternal(Category geomCategory)
	{
		WriteMessageBegin();
		WriteMessageHeader(geomCategory, DrawType::None);
		MESSAGE_END();
	}

	// Header format:
	// - Category (uint8)
	// - Draw type (uint8)
	void CMessageWriter::WriteMessageHeader(Category category, DrawType drawType)
	{
		WRITE_BYTE(static_cast<uint8_t>(category));
		WRITE_BYTE(static_cast<uint8_t>(drawType));
	}

	bool CMessageWriter::CanWriteMessage()
	{
		if ( m_iMessageId == 0 )
		{
			ALERT(at_warning, "Attempted to send custom geometry message before registration!\n");
			return false;
		}

		return true;
	}

	bool CMessageWriter::IsValidSpecificCategory(Category geomCategory)
	{
		return geomCategory > Category::None && geomCategory < Category::CategoryCount;
	}
}
