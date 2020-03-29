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
		bool IsEmpty() const;

		DrawType GetDrawType() const;
		void SetDrawType(DrawType type);

		uint32_t GetColour() const;
		void SetColour(uint32_t col);

		// These add points only.
		bool AddPoint(const Vector& p0);

		// These add points and relevant indices.
		bool AddLine(const Vector& p0, const Vector& p1);
		bool AddPointAndIndex(const Vector& p0);

		// Expects a quad with vertices specified in consecutive order around the perimeter.
		// The direction that the perimeter is traversed defines the winding of the quad.
		bool AddTriangleQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3);

		// These only add indices, and assume that the indices are for valid points
		// (they are not checked).
		bool AddIndex(uint8_t index);
		bool AddLineIndices(uint8_t i0, uint8_t i1);

		size_t GetPointCount() const;

		const CUtlVector<Vector>& GetPoints() const;
		const CUtlVector<uint8_t>& GetIndices() const;

	private:
		uint8_t CurrentBaseIndex() const;

		DrawType m_DrawType = DrawType::None;
		uint32_t m_Colour = 0xFFFFFFFF;
		CUtlVector<Vector> m_Points;
		CUtlVector<uint8_t> m_Indices;
	};

	typedef std::shared_ptr<CGeometryItem> GeometryItemPtr_t;
}
