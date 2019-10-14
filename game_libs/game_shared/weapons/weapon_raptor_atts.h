#include "weapon_raptor.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum RaptorAnimations_e
{
	RAPTOR_IDLE1 = 0,
	RAPTOR_IDLE2,
	RAPTOR_IDLE3,
	RAPTOR_IDLE4,
	RAPTOR_SHOOT,
	RAPTOR_RELOAD_NOT_EMPTY,
	RAPTOR_SHOOT_LAST,
	RAPTOR_RELOAD_EMPTY,
	RAPTOR_IDLE1_EMPTY,
	RAPTOR_IDLE2_EMPTY,
	RAPTOR_IDLE3_EMPTY,
	RAPTOR_IDLE4_EMPTY,
	RAPTOR_DRAW,
	RAPTOR_HOLSTER
};

enum RaptorAttackMode_e
{
	ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float RAPTOR_FIRE_RATE = 1.0f / 0.4f;

static const CAmmoDef Ammo_Raptor =
{
	.ClassName = "ammo_raptor",
	.AmmoName = "ammodef_raptor",
	.MaxCarry = 72,
	.AmmoBoxGive = 9
};

static const WeaponAtts::WACollection StaticWeaponAttributes([](WeaponAtts::WACollection& obj)
{
	using namespace WeaponAtts;

	WACore& core = obj.Core;
	core.Classname = "weapon_raptor";
	core.Id = WeaponId_e::WeaponRaptor;
	core.SwitchWeight = WeaponPref_Raptor;

	WAAmmoDef& ammo = obj.Ammo;
	ammo.PrimaryAmmo = &Ammo_Raptor;
	ammo.MaxClip = 9;
	ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

	WAViewModel& vm = obj.ViewModel;
	vm.ModelName = "models/weapon_raptor/v_raptor.mdl";
	vm.Anim_Draw = RAPTOR_DRAW;
	vm.AnimList_Idle << RAPTOR_IDLE1 << RAPTOR_IDLE2 << RAPTOR_IDLE3 << RAPTOR_IDLE4;
	vm.AnimList_Reload << RAPTOR_RELOAD_NOT_EMPTY;
	vm.AnimList_ReloadEmpty << RAPTOR_RELOAD_EMPTY;

	WAPlayerModel& pm = obj.PlayerModel;
	pm.PlayerModelName = "models/weapon_raptor/p_raptor.mdl";
	pm.WorldModelName = "models/weapon_raptor/w_raptor.mdl";
	pm.PlayerAnimExtension = "python";

	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_raptor", &skilldata_t::plrDmgRaptor));

	WAHitscanAttack* priAttack = new WAHitscanAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

	priAttack->EventScript = "events/weapon_raptor/fire01.sc";
	priAttack->FunctionsUnderwater = true;
	priAttack->IsContinuous = false;
	priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
	priAttack->SetUniformSpread(0.0f);
	priAttack->AttackRate = RAPTOR_FIRE_RATE;
	priAttack->BaseDamagePerShot = &skilldata_t::plrDmgRaptor;
	priAttack->AutoAim = AUTOAIM_10DEGREES;
	priAttack->Volume = LOUD_GUN_VOLUME;
	priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
	priAttack->ViewPunchY = -3.0f;
	priAttack->ShellModelName = "models/shell.mdl";

	priAttack->ViewModelAnimList_Attack << RAPTOR_SHOOT;
	priAttack->ViewModelAnimList_AttackEmpty << RAPTOR_SHOOT_LAST;

	priAttack->AttackSounds.MinVolume = 0.97f;
	priAttack->AttackSounds.MaxVolume = 1.0f;
	priAttack->AttackSounds.MinPitch = 95;
	priAttack->AttackSounds.MaxPitch = 100;
	priAttack->AttackSounds.SoundNames << "weapons/weapon_raptor/raptor_fire1.wav";
});
