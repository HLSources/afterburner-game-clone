#include <cstddef>
#include "ui/crosshair/spreadVisualiser.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/geometryStatics.h"
#include "vector_classes.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "hud.h"
#include "cl_util.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"

namespace
{
	static constexpr float YOFFSET_FRAC = 0.7f;
	static constexpr size_t PADDING = 40;
	static constexpr size_t SCALE_HEIGHT = 50;
	static constexpr size_t MARKER_WIDTH = 25;
}

void CSpreadVisualiser::Draw(const CCrosshairParameters& params, size_t attackMode)
{
	ConstructGeometry(params);
	UpdateInaccuracyMarker(params);
	UpdateDynamicBars(params, attackMode);

	CustomGeometry::RenderAdHocGeometry(m_Geometry);
	CustomGeometry::RenderAdHocGeometry(m_DynamicBars);
	DrawInfoText(params, attackMode);
}

void CSpreadVisualiser::ConstructGeometry(const CCrosshairParameters& params)
{
	if ( m_Geometry && m_ScreenDim == params.ScreenDimensions() )
	{
		return;
	}

	m_Geometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());

	m_Geometry->SetColour(0xFFFFFFFF);
	m_Geometry->SetDrawType(CustomGeometry::DrawType::Lines);

	m_ScreenDim = params.ScreenDimensions();
	m_ScaleMinX = PADDING;
	m_ScaleMaxX = params.ScreenDimensions().x - PADDING;
	m_ScaleYOffset = YOFFSET_FRAC * params.ScreenDimensions().y;

	const float halfScaleHeight = static_cast<float>(SCALE_HEIGHT) / 2.0f;

	// Horizontal ruled line for scale
	m_Geometry->AddLine(Vector(m_ScaleMinX, m_ScaleYOffset, 0.0f), Vector(m_ScaleMaxX, m_ScaleYOffset, 0.0f));

	// Caps at each end
	m_Geometry->AddLine(Vector(m_ScaleMinX, m_ScaleYOffset - halfScaleHeight, 0), Vector(m_ScaleMinX, m_ScaleYOffset + halfScaleHeight, 0));
	m_Geometry->AddLine(Vector(m_ScaleMaxX, m_ScaleYOffset - halfScaleHeight, 0), Vector(m_ScaleMaxX, m_ScaleYOffset + halfScaleHeight, 0));

	const float inaccuracyX = ExtraMath::RemapLinear(params.WeaponInaccuracy(), 0, 1, m_ScaleMinX, m_ScaleMaxX);

	// Arrow marker for current inaccuracy.
	// This will be updated properly later.
	m_InaccuracyMarkerBegin = m_Geometry->GetPointCount();
	m_Geometry->AddLine(Vector(inaccuracyX, m_ScaleYOffset - SCALE_HEIGHT, 0), Vector(inaccuracyX, m_ScaleYOffset, 0));
	m_Geometry->AddLine(Vector(inaccuracyX, m_ScaleYOffset - SCALE_HEIGHT, 0), Vector(inaccuracyX, m_ScaleYOffset, 0));
}

void CSpreadVisualiser::UpdateInaccuracyMarker(const CCrosshairParameters& params)
{
	const float inaccuracyX = ExtraMath::RemapLinear(params.WeaponInaccuracy(), 0, 1, m_ScaleMinX, m_ScaleMaxX);
	const float halfMarkerWidth = static_cast<float>(MARKER_WIDTH) / 2.0f;

	uint8_t index = m_InaccuracyMarkerBegin;
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX - halfMarkerWidth, m_ScaleYOffset - SCALE_HEIGHT, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX, m_ScaleYOffset, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX + halfMarkerWidth, m_ScaleYOffset - SCALE_HEIGHT, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX, m_ScaleYOffset, 0);
}

void CSpreadVisualiser::UpdateDynamicBars(const CCrosshairParameters& params, size_t attackMode)
{
	if ( !m_DynamicBars )
	{
		m_DynamicBars = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
		m_DynamicBars->SetColour(0xFFD800FF);
		m_DynamicBars->SetDrawType(CustomGeometry::DrawType::Lines);
	}

	m_DynamicBars->ClearGeometry();

	const WeaponAtts::AccuracyParameters* accuracyParams = params.WeaponAccuracyParamsForAttack(attackMode);

	if ( !accuracyParams )
	{
		return;
	}

	const float halfHeight = (2.0f * static_cast<float>(SCALE_HEIGHT)) / 6.0f;

	// Line representing rest inaccuracy.
	const float restX = ExtraMath::RemapLinear(accuracyParams->RestValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(Vector(restX, m_ScaleYOffset - halfHeight, 0), Vector(restX, m_ScaleYOffset + halfHeight, 0));

	// Line representing run inaccuracy.
	const float runX = ExtraMath::RemapLinear(accuracyParams->RunValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(Vector(runX, m_ScaleYOffset - halfHeight, 0), Vector(runX, m_ScaleYOffset + halfHeight, 0));
}

void CSpreadVisualiser::DrawInfoText(const CCrosshairParameters& params, size_t attackMode)
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(params.WeaponID());
	const char* weaponName = atts ? atts->Core.Classname : "UNKNOWN";

	const WeaponAtts::AccuracyParameters* accuracyParams = params.WeaponAccuracyParamsForAttack(attackMode);
	const float restInaccuracy = accuracyParams ? accuracyParams->RestValue : 0.0f;
	const float runInaccuracy = accuracyParams ? accuracyParams->RunValue : 0.0f;
	const float crouchShift = accuracyParams ? accuracyParams->CrouchShift : 0.0f;
	const float fallShift = accuracyParams ? accuracyParams->FallShift : 0.0f;
	const Vector2D minSpread = accuracyParams ? accuracyParams->MinSpread : Vector2D();
	const Vector2D maxSpread = accuracyParams ? accuracyParams->MaxSpread : Vector2D();

	CUtlString text;

	text.AppendFormat("Weapon: %s (ID %u)\n", weaponName, params.WeaponID());
	text.AppendFormat("Current inaccuracy: %f\n", params.WeaponInaccuracy());
	text.AppendFormat("Attributes for attack mode %u:\n", attackMode);
	text.AppendFormat("  Spread: (%f, %f) - (%f, %f)\n", minSpread.x, minSpread.y, maxSpread.x, maxSpread.y);
	text.AppendFormat("  Rest-run range: %f - %f\n", restInaccuracy, runInaccuracy);
	text.AppendFormat("  Crouch shift: %f\n", crouchShift);
	text.AppendFormat("  Fall shift: %f\n", fallShift);

	DrawConsoleString(m_ScaleMinX, m_ScaleYOffset + SCALE_HEIGHT + 2, text.Get());
}
