#include "genericweapon.h"
#include "studio_utils_shared.h"
#include "weaponslots.h"
#include "gamerules.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#include "cl_entity.h"
#endif

namespace
{
	static constexpr const char* DEFAULT_WEAPON_PICKUP_SOUND = "items/gunpickup1.wav";
}

CGenericWeapon::CGenericWeapon()
	: CBasePlayerWeapon(),
	  m_pPrimaryAttackMode(nullptr),
	  m_pSecondaryAttackMode(nullptr),
	  m_iViewModelIndex(0),
	  m_iViewModelBody(0),
	  m_iWeaponSlot(-1),
	  m_iWeaponSlotPosition(-1),
	  m_bPrimaryAttackHeldDown(false),
	  m_bSecondaryAttackHeldDown(false)
{
}

CGenericWeapon::~CGenericWeapon()
{
	// Default implementation of blank virtual destructor.
}

void CGenericWeapon::Spawn()
{
	const WeaponAtts::WACore& core = WeaponAttributes().Core;
	core.Validate();

	pev->classname = MAKE_STRING(core.Classname); // hack to allow for old names
	Precache();
	m_iId = static_cast<int>(core.Id);
	FindWeaponSlotInfo();

	const WeaponAtts::WAPlayerModel& pm = WeaponAttributes().PlayerModel;

	if ( pm.WorldModelName )
	{
		SET_MODEL(ENT(pev), pm.WorldModelName);
	}

	m_iDefaultAmmo = WeaponAttributes().Ammo.PrimaryAmmoOnFirstPickup;

	FallInit();// get ready to fall down.
}

void CGenericWeapon::Precache()
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();

	PrecacheCore(atts.Core);
	PrecacheViewModel(atts.ViewModel);
	PrecachePlayerModel(atts.PlayerModel);

	FOR_EACH_VEC(atts.AttackModes, index)
	{
		ASSERT(atts.AttackModes[index].get());
		PrecacheAttackMode(*atts.AttackModes[index]);
	}
}

void CGenericWeapon::PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode)
{
	PrecacheSoundSet(attackMode.AttackSounds);

	const uint32_t index = attackMode.Signature()->Index;

	if ( m_AttackModeEvents.Count() < index + 1 )
	{
		m_AttackModeEvents.SetCount(index + 1);
	}

	m_AttackModeEvents[index] = PRECACHE_EVENT(1, attackMode.EventScript);
}

void CGenericWeapon::PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds)
{
	const WeightedValueList<const char*>& soundList = sounds.SoundNames;

	for ( uint32_t index = 0; index < soundList.Count(); ++index )
	{
		PRECACHE_SOUND(soundList.Value(index));
	}
}

void CGenericWeapon::PrecacheCore(const WeaponAtts::WACore& core)
{
	PRECACHE_SOUND(core.PickupSoundOverride ? core.PickupSoundOverride : DEFAULT_WEAPON_PICKUP_SOUND);
}

void CGenericWeapon::PrecacheViewModel(const WeaponAtts::WAViewModel& viewModel)
{
	if ( viewModel.ModelName )
	{
		m_iViewModelIndex = PRECACHE_MODEL(viewModel.ModelName);
		StudioGetAnimationDurations(m_iViewModelIndex, m_ViewAnimDurations);
	}

	PrecacheSoundSet(viewModel.ReloadSounds);
}

void CGenericWeapon::PrecachePlayerModel(const WeaponAtts::WAPlayerModel& playerModel)
{
	if ( playerModel.PlayerModelName )
	{
		PRECACHE_MODEL(playerModel.PlayerModelName);
	}

	if ( playerModel.WorldModelName )
	{
		PRECACHE_MODEL(playerModel.WorldModelName);
	}
}

int CGenericWeapon::GetItemInfo(ItemInfo *p)
{
	FindWeaponSlotInfo();

	const WeaponAtts::WACollection& atts = WeaponAttributes();

	p->pszName = STRING(pev->classname);
	p->iMaxClip = atts.Ammo.MaxClip;
	p->iSlot = m_iWeaponSlot;
	p->iPosition = m_iWeaponSlotPosition;
	p->iFlags = atts.Core.Flags;
	p->iId = m_iId = static_cast<int>(atts.Core.Id);
	p->iWeight = atts.Core.SwitchWeight;

	const CAmmoDef* primaryAmmo = atts.Ammo.PrimaryAmmo;
	if ( primaryAmmo )
	{
		p->pszAmmo1 = primaryAmmo->AmmoName;
		p->iMaxAmmo1 = primaryAmmo->MaxCarry;
	}
	else
	{
		p->pszAmmo1 = NULL;
		p->iMaxAmmo1 = -1;
	}

	const CAmmoDef* secondaryAmmo = atts.Ammo.SecondaryAmmo;
	if ( secondaryAmmo )
	{
		p->pszAmmo2 = secondaryAmmo->AmmoName;
		p->iMaxAmmo2 = secondaryAmmo->MaxCarry;
	}
	else
	{
		p->pszAmmo2 = NULL;
		p->iMaxAmmo2 = -1;
	}

	return 1;
}

int CGenericWeapon::AddToPlayer(CBasePlayer *pPlayer)
{
	if( !CBasePlayerWeapon::AddToPlayer(pPlayer) )
	{
		return FALSE;
	}

	MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
	MESSAGE_END();
	return TRUE;
}

BOOL CGenericWeapon::Deploy()
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();

	return DefaultDeploy(atts.ViewModel.ModelName,
						 atts.PlayerModel.PlayerModelName,
						 atts.ViewModel.Anim_Draw,
						 atts.PlayerModel.PlayerAnimExtension,
						 m_iViewModelBody);
}

void CGenericWeapon::PrimaryAttack()
{
	InvokeAttack(WeaponAttackType::Primary);
}

void CGenericWeapon::SecondaryAttack()
{
	InvokeAttack(WeaponAttackType::Secondary);
}

bool CGenericWeapon::InvokeAttack(WeaponAttackType type)
{
	if ( type == WeaponAttackType::None )
	{
		return false;
	}

	return InvokeWithAttackMode(type, type == WeaponAttackType::Primary ? m_pPrimaryAttackMode : m_pSecondaryAttackMode);
}

bool CGenericWeapon::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( type == WeaponAttackType::None || !attackMode )
	{
		return false;
	}

	if ( (m_pPlayer->pev->waterlevel == 3 && !attackMode->FunctionsUnderwater) || !HasAmmo(attackMode) )
	{
		if( m_fFireOnEmpty )
		{
			PlayEmptySound();
			DelayFiring(0.2f, false, type);
		}

		return false;
	}

	return true;
}

// TODO: Allow this to handle secondary too? Do we need this?
void CGenericWeapon::Reload()
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(m_pPrimaryAttackMode);

	if ( !ammoAttack )
	{
		return;
	}

	const WeaponAtts::WACollection& atts = WeaponAttributes();
	const int maxClip = atts.Ammo.MaxClip;

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 1 || m_iClip == maxClip ||
		 m_flNextPrimaryAttack > UTIL_WeaponTimeBase() )
	{
		return;
	}

	if ( ammoAttack->SpecialReload )
	{
		// Reload is more complicated, so let the derived weapon class do it.
		m_fInSpecialReload = HandleSpecialReload(m_fInSpecialReload);
		return;
	}

	const WeightedValueList<int>* reloadAnimList = m_iClip < 1
		? &atts.ViewModel.AnimList_ReloadEmpty
		: &atts.ViewModel.AnimList_Reload;

	if ( m_iClip < 1 && reloadAnimList->Count() < 1 )
	{
		reloadAnimList = &atts.ViewModel.AnimList_Reload;
	}

	int anim = reloadAnimList->Count() > 0
		? reloadAnimList->ItemByProbabilisticValue(UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1))
		: -1;

	const float animDuration = anim >= 0
		? ViewModelAnimationDuration(anim)
		: 0;

	if ( DefaultReload(maxClip, anim, animDuration, m_iViewModelBody) )
	{
		PlaySound(atts.ViewModel.ReloadSounds);
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	}
}

// TODO: Refactor this!
void CGenericWeapon::ItemPostFrame()
{
	WeaponTick();

	if( ( m_fInReload ) && ( m_pPlayer->m_flNextAttack <= UTIL_WeaponTimeBase() ) )
	{
		// TODO: Ammo needs to be sync'd to client for this to work.
#ifndef CLIENT_DLL
		// complete the reload.
		int j = Q_min( iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

		// Add them to the clip
		m_iClip += j;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= j;
#else
	// No idea why this is so arbitrary, but Half Life had it in...
	m_iClip += 10;
#endif

		m_fInReload = FALSE;
	}

	if( !(m_pPlayer->pev->button & IN_ATTACK ) )
	{
		m_flLastFireTime = 0.0f;
		m_bPrimaryAttackHeldDown = false;
	}

	if ( !(m_pPlayer->pev->button & IN_ATTACK2) )
	{
		m_bSecondaryAttackHeldDown = false;
	}

	const bool priAttackIsContinuous = m_pPrimaryAttackMode && m_pPrimaryAttackMode->IsContinuous;
	const bool secAttackIsContinuous = m_pSecondaryAttackMode && m_pSecondaryAttackMode->IsContinuous;

	if( (m_pPlayer->pev->button & IN_ATTACK2) &&
		CanAttack(m_flNextSecondaryAttack, gpGlobals->time, UseDecrement()) &&
		(secAttackIsContinuous || !m_bSecondaryAttackHeldDown) )
	{
		SetFireOnEmptyState(m_pSecondaryAttackMode);
		SecondaryAttack();
		m_bSecondaryAttackHeldDown = true;
	}
	else if( (m_pPlayer->pev->button & IN_ATTACK) &&
			 CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement()) &&
			 (priAttackIsContinuous || !m_bPrimaryAttackHeldDown) )
	{
		SetFireOnEmptyState(m_pPrimaryAttackMode);
		PrimaryAttack();
		m_bPrimaryAttackHeldDown = true;
	}
	else if( m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload )
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if( !( m_pPlayer->pev->button & ( IN_ATTACK | IN_ATTACK2 ) ) )
	{
		// no fire buttons down
		m_fFireOnEmpty = FALSE;

#ifndef CLIENT_DLL
		if( !IsUseable() && m_flNextPrimaryAttack < ( UseDecrement() ? 0.0 : gpGlobals->time ) )
		{
			// weapon isn't useable, switch.
			if( !( iFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY ) && g_pGameRules->GetNextBestWeapon( m_pPlayer, this ) )
			{
				m_flNextPrimaryAttack = ( UseDecrement() ? 0.0 : gpGlobals->time ) + 0.3;
				return;
			}
		}
		else
#endif
		{
			// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
			if( m_iClip == 0 && !(iFlags() & ITEM_FLAG_NOAUTORELOAD ) && m_flNextPrimaryAttack < ( UseDecrement() ? 0.0 : gpGlobals->time ) )
			{
				Reload();
				return;
			}
		}

		WeaponIdle();
		return;
	}

	// catch all
	if( ShouldWeaponIdle() )
	{
		WeaponIdle();
	}
}

void CGenericWeapon::SetFireOnEmptyState(const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode )
	{
		return;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		return;
	}

	const char* ammoName = nullptr;
	int ammoIndex = -1;

	switch ( ammoAttack->UsesAmmoPool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			ammoName = pszAmmo1();
			ammoIndex = PrimaryAmmoIndex();
			break;
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			ammoName = pszAmmo2();
			ammoIndex = SecondaryAmmoIndex();
			break;
		}

		default:
		{
			return;
		}
	}

	if( ammoName && ammoIndex >= 0 && m_pPlayer->m_rgAmmo[ammoIndex] < 1 )
	{
		m_fFireOnEmpty = TRUE;
	}
}

int CGenericWeapon::HandleSpecialReload(int currentState)
{
	return 0;
}

void CGenericWeapon::WeaponIdle()
{
	const WeaponAtts::WACollection& Atts = WeaponAttributes();

	ResetEmptySound();

	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
	{
		return;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(m_pPrimaryAttackMode);

	if ( (ammoAttack && ammoAttack->SpecialReload) ? IdleProcess_CheckSpecialReload() : IdleProcess_CheckReload() )
	{
		// Reload did something, so don't play idle animations;
		return;
	}

	IdleProcess_PlayIdleAnimation();
}

bool CGenericWeapon::IdleProcess_CheckReload()
{
	const WeaponAtts::WAAmmoBasedAttack* attackMode = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(m_pPrimaryAttackMode);
	if ( attackMode && attackMode->AutoReload && m_iClip < 1 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0 )
	{
		Reload();
		return true;
	}

	return false;
}

bool CGenericWeapon::IdleProcess_CheckSpecialReload()
{
	if ( m_fInSpecialReload > 0 ||									// Haven't finished reloading yet
		 (m_iClip < 1 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]) )	// Need to fire off reload sequence automatically
	{
		m_fInSpecialReload = HandleSpecialReload(m_fInSpecialReload);
		return true;
	}

	return false;
}

void CGenericWeapon::IdleProcess_PlayIdleAnimation()
{
	const WeaponAtts::WAViewModel& vm = WeaponAttributes().ViewModel;
	const WeightedValueList<int>* animList = m_iClip < 1
		? &vm.AnimList_IdleEmpty
		: &vm.AnimList_Idle;

	if ( m_iClip < 1 && animList->Count() < 1 )
	{
		animList = &vm.AnimList_Idle;
	}

	if ( animList->Count() < 1 )
	{
		return;
	}

	const float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);
	const int anim = animList->ItemByProbabilisticValue(flRand);

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + ViewModelAnimationDuration(anim);
	SendWeaponAnim(anim, m_iViewModelBody);
}

void CGenericWeapon::PlaySound(const WeaponAtts::WASoundSet& sound, int channel, float volModifier)
{
	if ( sound.SoundNames.Count() < 1 )
	{
		return;
	}

	const float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);
	const char* soundName = sound.SoundNames.ItemByProbabilisticValue(flRand);
	const float volume = (sound.MinVolume < sound.MaxVolume)
		? UTIL_SharedRandomFloat(m_pPlayer->random_seed, sound.MinVolume, sound.MaxVolume)
		: sound.MaxVolume;
	const int pitch = (sound.MinPitch < sound.MaxPitch)
		? UTIL_SharedRandomLong(m_pPlayer->random_seed, sound.MinPitch, sound.MaxPitch)
		: sound.MaxPitch;

	EMIT_SOUND_DYN(ENT(m_pPlayer->pev),
				   channel,
				   soundName,
				   volume * volModifier,
				   sound.Attenuation,
				   sound.Flags,
				   pitch);
}

void CGenericWeapon::SetViewModelBody(int body, bool immediate)
{
	m_iViewModelBody = body;

#ifdef CLIENT_DLL
	if ( immediate )
	{
		struct cl_entity_s* viewModel = gEngfuncs.GetViewModel();
		if ( viewModel )
		{
			viewModel->curstate.body = body;
		}
	}
#endif
};

float CGenericWeapon::ViewModelAnimationDuration(int anim) const
{
	if ( anim < 0 || anim >= m_ViewAnimDurations.Count() )
	{
		return 0.0f;
	}

	return m_ViewAnimDurations[anim];
}

void CGenericWeapon::DelayPendingActions(float secs, bool allowIfEarlier)
{
	DelayFiring(secs, allowIfEarlier);
	SetNextIdleTime(secs, allowIfEarlier);
}

void CGenericWeapon::DelayFiring(float secs, bool allowIfEarlier, WeaponAttackType type)
{
	if ( type == WeaponAttackType::None || type == WeaponAttackType::Primary )
	{
		SetNextPrimaryAttack(secs, allowIfEarlier);
	}

	if ( type == WeaponAttackType::None || type == WeaponAttackType::Secondary )
	{
		SetNextSecondaryAttack(secs, allowIfEarlier);
	}
}

bool CGenericWeapon::HasAmmo(const WeaponAtts::WABaseAttack* attackMode, int minCount, bool useClip) const
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		// Treat as an infinite pool.
		return true;
	}

	switch ( ammoAttack->UsesAmmoPool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			return useClip
				? (m_iClip >= minCount)
				: (m_iPrimaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] >= minCount);
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			return m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] >= minCount;
		}

		default:
		{
			// Treat as an infinite pool.
			return true;
		}
	}
}

bool CGenericWeapon::DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode, int decrement)
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		// Treat as an infinite pool.
		return true;
	}

	switch ( ammoAttack->UsesAmmoPool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			if ( m_iClip < decrement )
			{
				return false;
			}

			m_iClip -= decrement;
			return true;
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			if ( m_iSecondaryAmmoType < 0 || m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] < decrement )
			{
				return false;
			}

			m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] -= decrement;
			return true;
		}

		default:
		{
			// Treat as an infinite pool.
			return true;
		}
	}
}

int CGenericWeapon::iItemSlot()
{
	ASSERT(m_iWeaponSlot >= 0);
	return m_iWeaponSlot;
}

void CGenericWeapon::FindWeaponSlotInfo()
{
	if ( m_iWeaponSlot >= 0 && m_iWeaponSlotPosition >= 0 )
	{
		return;
	}

	const int id = static_cast<const int>(WeaponAttributes().Core.Id);

	for ( int slot = 0; slot < MAX_WEAPON_SLOTS; ++slot )
	{
		for ( int position = 0; position < MAX_WEAPON_POSITIONS; ++position )
		{
			if ( WEAPON_HUD_SLOTS[slot][position] == id )
			{
				m_iWeaponSlot = slot;
				m_iWeaponSlotPosition = position;
				return;
			}
		}
	}

	ASSERTSZ(false, "No slot/position found for this weapon.");
}

void CGenericWeapon::GetSharedCircularGaussianSpread(uint32_t shot, int shared_rand, float& x, float& y)
{
	x = UTIL_SharedRandomFloat( shared_rand + shot, -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 1 + shot ) , -0.5, 0.5 );
	y = UTIL_SharedRandomFloat( shared_rand + ( 2 + shot ), -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 3 + shot ), -0.5, 0.5 );
}
