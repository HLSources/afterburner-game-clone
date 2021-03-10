#pragma once

namespace WeaponAtts
{
	struct AccuracyParameters;
}

typedef struct cvar_s cvar_t;

class CBasePlayer;

class CWeaponInaccuracyCalculator
{
public:
	using Accuracy = WeaponAtts::AccuracyParameters;

	CWeaponInaccuracyCalculator();

	bool IsValid() const;
	void Clear();
	void CalculateInaccuracy();
	void AddInaccuracyPenaltyFromFiring();

	float InstantaneousInaccuracy() const;
	float SmoothedInaccuracy() const;
	void SetLastSmoothedInaccuracy(float inaccuracy);

	const Accuracy* AccuracyParams() const;
	void SetAccuracyParams(const Accuracy* params);

	CBasePlayer* Player() const;
	void SetPlayer(CBasePlayer* player);

	void SetFiredThisFrame(bool fired);
	void SetLastFireTime(float time);

private:
	void CalculateInstantaneousInaccuracy();
	void CalculateSmoothedInaccuracy();

	// These variables don't have defaults set here, because they
	// are set to their defaults in Clear() instead.
	float m_InstantaneousInaccuracy;
	float m_SmoothedInaccuracy;
	float m_LastSmoothedInaccuracy;
	bool m_FiredThisFrame;
	float m_LastFireTime;
	const WeaponAtts::AccuracyParameters* m_AccuracyParams;
	CBasePlayer* m_Player;

	cvar_t* m_CvarMaxSpeed = nullptr;
	cvar_t* m_CvarMaxFallSpeed = nullptr;
};
