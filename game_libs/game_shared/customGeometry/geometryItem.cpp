#include "geometryItem.h"

namespace CustomGeometry
{
	void CGeometryItem::Clear()
	{
		m_Colour = 0xFFFFFFFF;
		m_DrawType = DrawType::None;
		m_Points.Purge();
	}

	DrawType CGeometryItem::GetDrawType() const
	{
		return m_DrawType;
	}

	void CGeometryItem::SetDrawType(DrawType type)
	{
		m_DrawType = type;
	}

	uint32_t CGeometryItem::GetColour() const
	{
		return m_Colour;
	}

	void CGeometryItem::SetColour(uint32_t col)
	{
		m_Colour = col;
	}

	const CUtlVector<Vector>& CGeometryItem::GetPoints() const
	{
		return m_Points;
	}

	void CGeometryItem::AddPoint(const Vector& p0)
	{
		m_Points.AddToTail(p0);
	}

	void CGeometryItem::AddLine(const Vector& p0, const Vector& p1)
	{
		m_Points.AddToTail(p0);
		m_Points.AddToTail(p1);
	}
}
