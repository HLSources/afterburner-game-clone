#pragma once

#include "weapon_frinesi.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum FrinesiAnimations_e
{
	FRINESI_IDLE1 = 0,
	FRINESI_SHOOT,
	FRINESI_SHOOT_BIG,
	FRINESI_RELOAD,
	FRINESI_PUMP,
	FRINESI_START_RELOAD,
	FRINESI_DRAW,
	FRINESI_HOLSTER,
	FRINESI_IDLE2,
	FRINESI_IDLE3,
	FRINESI_IDLE4
};

enum FrinesiAttackMode_e
{
	ATTACKMODE_AUTO = 0,
	ATTACKMODE_PUMP
};

// Shots per second:
static constexpr float FRINESI_FIRE_RATE_PUMP = 1.0f;
static constexpr float FRINESI_FIRE_RATE_AUTO = 4.0f;

static constexpr uint8_t FRINESI_PELLETS_PER_SHOT = 6;

static const CAmmoDef Ammo_Frinesi =
{
	.ClassName = "ammo_frinesi",
	.AmmoName = "ammodef_frinesi",
	.MaxCarry = 72,
	.AmmoBoxGive = 125
};

static const WeaponAtts::WACollection StaticWeaponAttributes([](WeaponAtts::WACollection& obj)
{
	using namespace WeaponAtts;

	WACore& core = obj.Core;
	core.Classname = "weapon_frinesi";
	core.Id = WeaponId_e::WeaponFrinesi;
	core.SwitchWeight = WeaponPref_Frinesi;

	WAAmmoDef& ammo = obj.Ammo;
	ammo.PrimaryAmmo = &Ammo_Frinesi;
	ammo.MaxClip = 8;
	ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

	WAViewModel& vm = obj.ViewModel;
	vm.ModelName = "models/weapon_frinesi/v_frinesi.mdl";
	vm.Anim_Draw = FRINESI_DRAW;
	vm.AnimList_Idle << FRINESI_IDLE1 << FRINESI_IDLE2 << FRINESI_IDLE3 << FRINESI_IDLE4;
	vm.AnimList_Reload << FRINESI_START_RELOAD;

	vm.ReloadSounds.MinVolume = 1.0f;
	vm.ReloadSounds.MaxVolume = 1.0f;
	vm.ReloadSounds.MinPitch = 98;
	vm.ReloadSounds.MaxPitch = 102;
	vm.ReloadSounds.SoundNames << "weapons/weapon_frinesi/frinesi_reload1.wav"
							   << "weapons/weapon_frinesi/frinesi_reload2.wav";

	WAPlayerModel& pm = obj.PlayerModel;
	pm.PlayerModelName = "models/weapon_frinesi/p_frinesi.mdl";
	pm.WorldModelName = "models/weapon_frinesi/w_frinesi.mdl";
	pm.PlayerAnimExtension = "shotgun";

	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_frinesi_auto", &skilldata_t::plrDmgFrinesiAuto));
	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_frinesi_pump", &skilldata_t::plrDmgFrinesiPump));

	// From Nightfire:
	// - 6 bullets per shot in either mode
	// - 81 damage over all 6 shots in auto mode = 13.5 per shot
	// - 159 damage over all 6 shots in pump mode = 26.5 per shot

	// Auto
	WAHitscanAttack* priAttack = new WAHitscanAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

	priAttack->EventScript = "events/weapon_frinesi/fire01.sc";
	priAttack->FunctionsUnderwater = true;
	priAttack->IsContinuous = false;
	priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
	priAttack->SetUniformSpread(0.05f);
	priAttack->AttackRate = FRINESI_FIRE_RATE_AUTO;
	priAttack->BaseDamagePerShot = &skilldata_t::plrDmgFrinesiAuto;
	priAttack->BulletsPerShot = 6;
	priAttack->AutoAim = AUTOAIM_10DEGREES;
	priAttack->Volume = LOUD_GUN_VOLUME;
	priAttack->MuzzleFlashBrightness = NORMAL_GUN_FLASH;
	priAttack->ViewPunchY = -5.0f;
	priAttack->ShellModelName = "models/shotgunshell.mdl";
	priAttack->SpecialReload = true;

	priAttack->ViewModelAnimList_Attack << FRINESI_SHOOT;

	priAttack->AttackSounds.MinVolume = 0.95f;
	priAttack->AttackSounds.MaxVolume = 1.0f;
	priAttack->AttackSounds.MinPitch = 98;
	priAttack->AttackSounds.MaxPitch = 102;
	priAttack->AttackSounds.SoundNames << "weapons/weapon_frinesi/frinesi_fire.wav";

	// Pump
	WAHitscanAttack* secAttack = new WAHitscanAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

	// Base off primary attack.
	*secAttack = *priAttack;
	secAttack->EventScript = "events/weapon_frinesi/fire02.sc";
	secAttack->AttackRate = FRINESI_FIRE_RATE_PUMP;
	secAttack->BaseDamagePerShot = &skilldata_t::plrDmgFrinesiPump;
	secAttack->SetUniformSpread(0.1f);
	secAttack->ViewPunchY = -10.0f;

	secAttack->ViewModelAnimList_Attack.Clear();
	secAttack->ViewModelAnimList_Attack << FRINESI_SHOOT_BIG;

	secAttack->AttackSounds.SoundNames.Clear();
	secAttack->AttackSounds.SoundNames << "weapons/weapon_frinesi/frinesi_altfire.wav";
});
