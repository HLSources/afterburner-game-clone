#include "ui/crosshair/crosshairParameters.h"
#include "miniutl.h"
#include "util/extramath.h"

CCrosshairParameters::CCrosshairParameters()
{
	Reset();
}

void CCrosshairParameters::Reset()
{
	m_ScreenDimensions.x = 0;
	m_ScreenDimensions.y = 0;
	m_WeaponInaccuracy = 0.0f;
	m_Radius = 0.0f;
	m_BarLength = 0.1f;
}

const UIVec2& CCrosshairParameters::ScreenDimensions() const
{
	return m_ScreenDimensions;
}

void CCrosshairParameters::SetScreenDimensions(const UIVec2& dim)
{
	m_ScreenDimensions = dim;
}

float CCrosshairParameters::WeaponInaccuracy() const
{
	return m_WeaponInaccuracy;
}

void CCrosshairParameters::SetWeaponInaccuracy(float inaccuracy)
{
	m_WeaponInaccuracy = ExtraMath::Clamp(0.0f, inaccuracy, 1.0f);
}

float CCrosshairParameters::Radius() const
{
	return m_Radius;
}

void CCrosshairParameters::SetRadius(float radius)
{
	m_Radius = Max(radius, 0.0f);
}

float CCrosshairParameters::BarLength() const
{
	return m_BarLength;
}

void CCrosshairParameters::SetBarLength(float length)
{
	m_BarLength = length;
}

UIVec2 CCrosshairParameters::HalfScreenDimensions() const
{
	return m_ScreenDimensions / 2;
}

int CCrosshairParameters::DisplacementFromScreenCentre(float fraction) const
{
	const UIVec2 centre = HalfScreenDimensions();
	const int smallerDim = Min(centre.x, centre.y);

	return static_cast<int>(static_cast<float>(smallerDim) * fraction);
}
