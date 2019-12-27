#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CMessageWriter
	{
	public:
		static void RegisterUserMessage();

		// Category must be greater than None and less than CategoryCount.
		explicit CMessageWriter(Category category);

		Category GetCategory() const;
		bool SetCategory(Category category);

		// By default, the message is sent to all clients.
		CBasePlayer* GetTargetClient() const;
		void SetTargetClient(CBasePlayer* client);
		void ClearTargetClient();

		// Writes message given currently set category.
		// If category is None, these functions will fail and return false.
		bool WriteMessage(const CGeometryItem& geometry);
		bool WriteClearMessage();

		// Ignores currently set category and writes clear message for all categories.
		bool WriteClearAllMessage();

	private:
		static bool CanWriteMessage();
		static bool IsValidSpecificCategory(Category geomCategory);

		void WriteMessageBegin();
		void WriteMessageHeader(Category category, DrawType drawType);
		void WriteClearMessageInternal(Category geomCategory);

		static int m_iMessageId;

		Category m_Category;
		CBasePlayer* m_pTargetClient;
	};
}
