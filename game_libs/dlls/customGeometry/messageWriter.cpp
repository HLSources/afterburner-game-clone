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

	bool CMessageWriter::WriteMessage(const CGeometryItem& geometry)
	{
		if ( !CanWriteMessage() )
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

		WRITE_BYTE(static_cast<uint8_t>(geometry.GetDrawType()));
		WRITE_LONG(geometry.GetColour());
		WRITE_SHORT(static_cast<uint16_t>(points.Count()));

		FOR_EACH_VEC(points, index)
		{
			const Vector& vec = points[index];

			for ( uint32_t coOrdAxis = 0; coOrdAxis < 3; ++coOrdAxis )
			{
				const float value = vec[coOrdAxis];

				// WRITE_COORD appears to have an 8-unit precision and we want to be more precise than that,
				// so we write each float individually as bytes.
				const uint8_t* data = reinterpret_cast<const uint8_t*>(&value);

				for ( uint32_t dataIndex = 0; dataIndex < sizeof(float); ++dataIndex )
				{
					WRITE_BYTE(data[dataIndex]);
				}
			}
		}

		MESSAGE_END();
		return true;
	}

	bool CMessageWriter::WriteClearMessage()
	{
		if ( !CanWriteMessage() )
		{
			return false;
		}

		WriteMessageBegin();
		WRITE_BYTE(static_cast<uint8_t>(DrawType::None));
		MESSAGE_END();

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

	bool CMessageWriter::CanWriteMessage() const
	{
		if ( m_iMessageId == 0 )
		{
			ALERT(at_warning, "Attempted to send custom geometry message before registration!\n");
			return false;
		}

		return true;
	}
}
