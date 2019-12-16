#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CMessageWriter
	{
	public:
		static void RegisterUserMessage();

		// By default, the message is sent to all clients.
		void SetTargetClient(CBasePlayer* client);
		void ClearTargetClient();

		bool WriteMessage(const CGeometryItem& geometry);
		bool WriteClearMessage();

	private:
		bool CanWriteMessage() const;
		void WriteMessageBegin();

		static int m_iMessageId;

		CBasePlayer* m_pTargetClient = nullptr;
	};
}
