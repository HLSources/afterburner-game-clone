#ifndef WEAPON_PREF_WEIGHTS_H
#define WEAPON_PREF_WEIGHTS_H

// All in one place, so it's easy to see how weapons compare to one another.
// Weapons with higher preference values are more desirable than those with
// lower values.
enum WeaponPreferenceWeight
{
	// Least preferable
	WeaponPref_Fists = 1,
	WeaponPref_P99,
	WeaponPref_Raptor,
	WeaponPref_Frinesi,
	WeaponPref_GrenadeLauncher,
	WeaponPref_Max = WeaponPref_GrenadeLauncher
	// Most preferable
};

#endif // WEAPON_PREF_WEIGHTS_H