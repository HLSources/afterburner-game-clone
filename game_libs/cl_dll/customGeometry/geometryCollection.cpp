#include "geometryCollection.h"

namespace CustomGeometry
{
	CGeometryCollection& CGeometryCollection::StaticInstance()
	{
		static CGeometryCollection collection;
		return collection;
	}

	void CGeometryCollection::Clear()
	{
		m_Items.Purge();
	}

	size_t CGeometryCollection::Count() const
	{
		return m_Items.Count();
	}

	GeometryItemPtr_t CGeometryCollection::ItemAt(uint32_t index) const
	{
		return index < m_Items.Count() ? m_Items[index] : GeometryItemPtr_t();
	}

	void CGeometryCollection::AddItem(const GeometryItemPtr_t& item)
	{
		if ( !item || item->GetDrawType() == DrawType::None )
		{
			return;
		}

		m_Items.AddToTail(item);
	}
}
