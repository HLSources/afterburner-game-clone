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

		// Default behaviour: add item to tail of list, clear list
		// of all items, render all items. Create a subclass to
		// override this behaviour.
		virtual void AddItem(const GeometryItemPtr_t& item);
		virtual void Clear();
		virtual void Render();

	protected:
		CUtlVector<GeometryItemPtr_t> m_Items;
	};
}
