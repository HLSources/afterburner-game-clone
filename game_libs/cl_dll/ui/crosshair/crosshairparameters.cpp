#include "ui/crosshair/crosshairparameters.h"
#include "miniutl.h"

CCrosshairParameters::CCrosshairParameters()
{
	Reset();
}

void CCrosshairParameters::Reset()
{
	m_ScreenDimensions.x = 0;
	m_ScreenDimensions.y = 0;
	m_WeaponInaccuracy = 0.0f;
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
	if ( inaccuracy < 0.0f )
	{
		inaccuracy = 0.0f;
	}
	else if ( inaccuracy > 1.0f )
	{
		inaccuracy = 1.0f;
	}

	m_WeaponInaccuracy = inaccuracy;
}

UIVec2 CCrosshairParameters::HalfScreenDimensions() const
{
	// TODO: Take away (1, 1) so that it matches what the HL crosshair uses.
	return m_ScreenDimensions / 2;
}

int CCrosshairParameters::DisplacementFromScreenCentre(float fraction) const
{
	const UIVec2 centre = HalfScreenDimensions();
	const int smallerDim = Min(centre.x, centre.y);

	return static_cast<int>(static_cast<float>(smallerDim) * fraction);
}
