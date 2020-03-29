#include "baseMessageWriter.h"
#include "standard_includes.h"

namespace Messages
{
	CBaseMessageWriter::CBaseMessageWriter(const char* name, int id) :
		m_MessageName(name),
		m_iMessageId(id)
	{
		ASSERT(name);
		ASSERT(id != -1);
	}

	bool CBaseMessageWriter::BeginMessage(CBasePlayer* target)
	{
		return BeginMessage(target ? target->pev : nullptr);
	}

	bool CBaseMessageWriter::BeginMessage(entvars_t* target)
	{
		if ( !CanWriteMessage() )
		{
			return false;
		}

		if ( target )
		{
			MESSAGE_BEGIN(MSG_ONE, m_iMessageId, nullptr, target);
		}
		else
		{
			MESSAGE_BEGIN(MSG_ALL, m_iMessageId);
		}

		return true;
	}

	void CBaseMessageWriter::EndMessage()
	{
		MESSAGE_END();
	}

	bool CBaseMessageWriter::CanWriteMessage() const
	{
		if ( m_iMessageId == 0 )
		{
			ALERT(at_warning, "Attempted to send '%s' message before registration!\n", m_MessageName);
			return false;
		}

		return true;
	}
}
