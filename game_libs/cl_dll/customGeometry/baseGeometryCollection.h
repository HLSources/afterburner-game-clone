#pragma once

#include "standard_includes.h"
#include "utlvector.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CBaseGeometryCollection
	{
	public:
		size_t Count() const;
		GeometryItemPtr_t ItemAt(uint32_t index) const;

		virtual void ItemReceived(const GeometryItemPtr_t& item);
		virtual void ClearMessageReceived();
		virtual void Render();

	protected:
		CUtlVector<GeometryItemPtr_t> m_Items;
	};
}
