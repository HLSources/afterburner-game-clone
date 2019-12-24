#pragma once

#include "standard_includes.h"
#include "utlvector.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CGeometryCollection
	{
	public:
		static CGeometryCollection& StaticInstance();

		size_t Count() const;
		GeometryItemPtr_t ItemAt(uint32_t index) const;

		void AddItem(const GeometryItemPtr_t& item);
		void Clear();

	private:
		CUtlVector<GeometryItemPtr_t> m_Items;
	};
}
