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

	static bool GetValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params);

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
	void SetLastFireTimeIsDecremented(bool decremented);

private:
	static void InitCvars();

	const WeaponAtts::AccuracyParameters* GetInternalParameters() const;
	bool IsValid() const;
	void CalculateInstantaneousInaccuracy(const WeaponAtts::AccuracyParameters* params);
	void CalculateSmoothedInaccuracy(const WeaponAtts::AccuracyParameters* params);
	bool LastFireTimeIsInHoldRegion(float holdTime) const;

	// These variables don't have defaults set here, because they
	// are set to their defaults in Clear() instead.
	float m_InstantaneousInaccuracy;
	float m_SmoothedInaccuracy;
	float m_LastSmoothedInaccuracy;
	bool m_FiredThisFrame;
	float m_LastFireTime;
	bool m_LastFireTimeIsDecremented;
	const WeaponAtts::AccuracyParameters* m_AccuracyParams;
	CBasePlayer* m_Player;

	static bool m_StandardCvarsLoaded;
	static bool m_DebuggingCvarsLoaded;

	static cvar_t* m_CvarMaxSpeed;
	static cvar_t* m_CvarMaxFallSpeed;
	static cvar_t* m_CvarCheats;

	// Debugging cvars:
	static cvar_t* m_CvarEnableDebugging;
	static cvar_t* m_CvarDebugRestValue;
	static cvar_t* m_CvarDebugRestSpread;
	static cvar_t* m_CvarDebugRunValue;
	static cvar_t* m_CvarDebugRunSpread;
	static cvar_t* m_CvarDebugCrouchShift;
	static cvar_t* m_CvarDebugFallShift;
	static cvar_t* m_CvarDebugFollowCoefficient;
	static cvar_t* m_CvarDebugFireImpulse;
	static cvar_t* m_CvarDebugFireImpulseCeiling;
	static cvar_t* m_CvarDebugFireImpulseHoldTime;

	static WeaponAtts::AccuracyParameters m_StaticDebugParams;
};
