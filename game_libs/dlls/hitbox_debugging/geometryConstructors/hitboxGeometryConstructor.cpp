#include "hitboxGeometryConstructor.h"

void CHitboxGeometryConstructor::SetEntity(CBaseAnimating* entity)
{
	m_pEntity = entity;
}

void CHitboxGeometryConstructor::SetHitboxIndex(uint32_t index)
{
	m_iHitboxIndex = index;
}

bool CHitboxGeometryConstructor::AddGeometry(const CustomGeometry::GeometryItemPtr_t& geom) const
{
	if ( !m_pEntity )
	{
		return false;
	}

	edict_t* edict = m_pEntity->edict();
	uint32_t hitboxCount = g_engfuncs.pfnGetHitboxCount(edict);

	if ( m_iHitboxIndex >= hitboxCount )
	{
		return false;
	}

	HitboxFloats floats;

	if ( !g_engfuncs.pfnGetTransformedHitboxPoints(edict, m_iHitboxIndex, floats.floats) )
	{
		return false;
	}

	HitboxPoints points;
	HitboxFloatsToPoints(floats, points);
	CreateGeometryFromPoints(geom, points);

	return true;
}

void CHitboxGeometryConstructor::CreateGeometryFromPoints(const CustomGeometry::GeometryItemPtr_t& geom,
														  const HitboxPoints& points) const
{
	// Bottom rect
	CreateRectFromPoints(geom, points, 0);

	// Top rect
	CreateRectFromPoints(geom, points, 4);

	// Connect up the corners
	for ( uint32_t index = 0; index < 4; ++index )
	{
		const Vector& p0 = points.points[index];
		const Vector& p1 = points.points[index + 4];

		geom->AddLine(p0, p1);
	}
}

void CHitboxGeometryConstructor::CreateRectFromPoints(const CustomGeometry::GeometryItemPtr_t& geom,
													  const HitboxPoints& points,
													  uint32_t offset) const
{
	ASSERT(HitboxPoints::NUM_ELEMENTS - offset >= 4);

	for ( uint32_t index = 0; index < 4; ++index )
	{
		const Vector& p0 = points.points[offset + index];
		const Vector& p1 = points.points[offset + ((index + 1) % 4)];
		geom->AddLine(p0, p1);
	}
}

void CHitboxGeometryConstructor::HitboxFloatsToPoints(const HitboxFloats& floats, HitboxPoints& points)
{
	for ( uint32_t pointIndex = 0; pointIndex < HitboxPoints::NUM_ELEMENTS; ++pointIndex )
	{
		points.points[pointIndex] = Vector(&floats.floats[3 * pointIndex]);
	}
}
