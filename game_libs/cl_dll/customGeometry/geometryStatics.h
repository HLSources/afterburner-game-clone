#pragma once

#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	void Initialise();
	void RenderAllGeometry();
	void RenderAdHocGeometry(const GeometryItemPtr_t& item);
	void ClearAllGeometry();
}
