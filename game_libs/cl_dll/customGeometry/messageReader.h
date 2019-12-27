#pragma once

#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CMessageReader
	{
	public:
		enum class ReadResult
		{
			Error = 0,
			OK,
			Clear
		};

		ReadResult ReadMessage(void* buffer, int size);

		// Filled out after a call to ReadMessage().
		ReadResult GetLastReadResult() const;
		GeometryItemPtr_t GetGeometryItem() const;
		Category GetGeometryCategory() const;

	private:
		void SetErrorState();
		bool ReadMessageInternal(void* buffer, int size);
		bool ReadGeometryCategory();

		ReadResult m_ReadResult = ReadResult::Error;
		GeometryItemPtr_t m_GeometryItem;
		Category m_GeomCategory = Category::None;
	};
}
