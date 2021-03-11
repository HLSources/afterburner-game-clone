#include "gameplay/crosshairCvars.h"
#include "cvardef.h"
#include "cl_dll.h"
#include "debug_assert.h"

namespace CrosshairCvars
{
	static cvar_t* CvarCheats = nullptr;
	static cvar_t* CvarDebugSpread = nullptr;
	static cvar_t* CvarOverrideCrosshairAtts = nullptr;
	static cvar_t* CvarOverrideCrosshairRadiusMin = nullptr;
	static cvar_t* CvarOverrideCrosshairRadiusMax = nullptr;
	static cvar_t* CvarOverrideCrosshairBarLengthMin = nullptr;
	static cvar_t* CvarOverrideCrosshairBarLengthMax = nullptr;
	static bool CvarsLoaded = false;

	inline cvar_t* GetOrCreateClientCvar(const char* name, const char* defaultValue = "0")
	{
		cvar_t* cvar = gEngfuncs.pfnGetCvarPointer(name);

		if ( !cvar )
		{
			cvar = gEngfuncs.pfnRegisterVariable(name, defaultValue, FCVAR_CLIENTDLL);
		}

		return cvar;
	}

	void Init()
	{
		CvarCheats = gEngfuncs.pfnGetCvarPointer("sv_cheats");
		CvarDebugSpread = GetOrCreateClientCvar("cl_debug_weapon_spread");
		CvarOverrideCrosshairAtts = GetOrCreateClientCvar("cl_crosshair_ovr_enabled");
		CvarOverrideCrosshairRadiusMin = GetOrCreateClientCvar("cl_crosshair_ovr_rad_min", "0");
		CvarOverrideCrosshairRadiusMax = GetOrCreateClientCvar("cl_crosshair_ovr_rad_max", "0.5");
		CvarOverrideCrosshairBarLengthMin = GetOrCreateClientCvar("cl_crosshair_ovr_bar_min", "0.04");
		CvarOverrideCrosshairBarLengthMax = GetOrCreateClientCvar("cl_crosshair_ovr_bar_max", "0.03");

		CvarsLoaded =
			CvarCheats &&
			CvarDebugSpread &&
			CvarOverrideCrosshairAtts &&
			CvarOverrideCrosshairRadiusMin &&
			CvarOverrideCrosshairRadiusMax &&
			CvarOverrideCrosshairBarLengthMin &&
			CvarOverrideCrosshairBarLengthMax;

		ASSERTSZ(CvarsLoaded, "Unable to load crosshair debugging cvars.");
	}

	bool SpreadVisualisationEnabled()
	{
		return
			CvarsLoaded &&
			CvarCheats->value != 0.0f &&
			static_cast<int>(CvarDebugSpread->value) > 0;
	}

	bool CrosshairOverrideEnabled()
	{
		return
			CvarsLoaded &&
			CvarCheats->value != 0.0f &&
			CvarOverrideCrosshairAtts->value != 0.0f;
	}

	int AttackModeForSpreadVisualisation()
	{
		return CvarsLoaded ? static_cast<int>(CvarDebugSpread->value) : 0;
	}

	float RadiusMin()
	{
		return CvarsLoaded ? CvarOverrideCrosshairRadiusMin->value : 0.0f;
	}

	float RadiusMax()
	{
		return CvarsLoaded ? CvarOverrideCrosshairRadiusMax->value : 0.0f;
	}

	float BarLengthMin()
	{
		return CvarsLoaded ? CvarOverrideCrosshairBarLengthMin->value : 0.0f;
	}

	float BarLengthMax()
	{
		return CvarsLoaded ? CvarOverrideCrosshairBarLengthMax->value : 0.0f;
	}
}
