#pragma once

#include <memory>
#include "ui/core/uiVec2.h"
#include "weapons/weaponids.h"

namespace WeaponAtts
{
	struct AccuracyParameters;
	struct CrosshairParameters;
}

class CCrosshairParameters
{
public:
	CCrosshairParameters();
	void Reset();

	const UIVec2& ScreenDimensions() const;
	void SetScreenDimensions(const UIVec2& dim);

	float WeaponInaccuracy() const;
	void SetWeaponInaccuracy(float inaccuracy);

	float Radius() const;
	void SetRadius(float radius);

	float BarLength() const;
	void SetBarLength(float length);

	WeaponId_e WeaponID() const;
	void SetWeaponID(WeaponId_e id);

	UIVec2 HalfScreenDimensions() const;
	int DisplacementFromScreenCentre(float fraction) const;

	const WeaponAtts::AccuracyParameters* WeaponAccuracyParamsForAttack(size_t index) const;
	const WeaponAtts::CrosshairParameters* CrosshairParamsForAttack(size_t index) const;

private:
	UIVec2 m_ScreenDimensions;
	float m_WeaponInaccuracy = 0.0f;
	float m_Radius = 0.0f;
	float m_BarLength = 0.1f;
	WeaponId_e m_WeaponID = WeaponId_e::WeaponNone;
};
