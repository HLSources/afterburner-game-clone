#include "WeaponSoundResources.h"

static constexpr const char* const BulletRicochetSounds[] =
{
	"weapons/ric1.wav",
	"weapons/ric2.wav",
	"weapons/ric3.wav",
	"weapons/ric4.wav",
	"weapons/ric5.wav",
};

void CWeaponSoundResources::Initialise()
{
	InitialiseSounds(WeaponSoundId::BulletRicochet, BulletRicochetSounds);
	InitialiseSound(WeaponSoundId::GrenadeBounce, "weapons/grenade_hit1.wav");
}
