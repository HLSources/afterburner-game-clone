#pragma once

#include <memory>
#include "standard_includes.h"
#include "utlvector.h"
#include "customGeometry/sharedDefs.h"

namespace CustomGeometry
{
	class CGeometryItem
	{
	public:
		void Clear();

		DrawType GetDrawType() const;
		void SetDrawType(DrawType type);

		uint32_t GetColour() const;
		void SetColour(uint32_t col);

		void AddPoint(const Vector& p0);
		void AddLine(const Vector& p0, const Vector& p1);

		const CUtlVector<Vector>& GetPoints() const;

	private:
		DrawType m_DrawType = DrawType::None;
		uint32_t m_Colour = 0xFFFFFFFF;
		CUtlVector<Vector> m_Points;
	};

	typedef std::shared_ptr<CGeometryItem> GeometryItemPtr_t;
}
