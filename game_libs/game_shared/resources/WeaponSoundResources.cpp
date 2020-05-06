#include "WeaponSoundResources.h"

static constexpr const char* const BulletRicochetSounds[] =
{
	"weapons/ric1.wav",
	"weapons/ric2.wav",
	"weapons/ric3.wav",
	"weapons/ric4.wav",
	"weapons/ric5.wav",
};

CWeaponSoundResources::CWeaponSoundResources() :
	CBaseResourceCollection<WeaponSoundId, static_cast<size_t>(WeaponSoundId::Count)>()
{
	InitialiseResources(WeaponSoundId::BulletRicochet, BulletRicochetSounds);
	InitialiseResource(WeaponSoundId::GrenadeBounce, "weapons/grenade_hit1.wav");
}
