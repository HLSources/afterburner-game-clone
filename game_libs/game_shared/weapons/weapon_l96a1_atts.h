#pragma once

#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum L96A1Animations_e
{
	L96A1_IDLE1,
	L96A1_IDLE2,
	L96A1_FIRE,
	L96A1_FIRE_LAST,
	L96A1_RELOAD,
	L96A1_RELOAD_CHARGE,
	L96A1_HOLSTER,
	L96A1_DRAW,
};

// TODO: 110 damage on shot
// TODO: Work out refire rate

static constexpr CAmmoDef Ammo_L96A1 =
{
	"ammo_l96a1",	// ClassName
	"ammodef_l96a1",	// AmmoName
	50,	// MaxCarry
	10	// AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes([](WeaponAtts::WACollection& obj)
{
	using namespace WeaponAtts;

	WACore& core = obj.Core;
	core.Classname = "weapon_l96a1";
	core.Id = WeaponId_e::Weapon_L96A1;
	core.SwitchWeight = WeaponPref_L96A1;

	WAAmmoDef& ammo = obj.Ammo;
	ammo.PrimaryAmmo = &Ammo_L96A1;
	ammo.MaxClip = 10;
	ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

	WAViewModel& vm = obj.ViewModel;
	vm.ModelName = "models/weapon_l96a1/v_l96a1.mdl";
	vm.Anim_Draw = L96A1_DRAW;
	vm.AnimList_Idle << L96A1_IDLE1 << L96A1_IDLE2;
	vm.AnimList_Reload << L96A1_RELOAD;
	vm.AnimList_ReloadEmpty << L96A1_RELOAD_CHARGE;

	WAPlayerModel& pm = obj.PlayerModel;
	pm.PlayerModelName = "models/weapon_l96a1/p_l96a1.mdl";
	pm.WorldModelName = "models/weapon_l96a1/w_l96a1.mdl";
	pm.PlayerAnimExtension = "bow";

	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_l96a1", &skilldata_t::plrDmgL96A1));
});
