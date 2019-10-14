#include "generichitscanweapon.h"
#include "weaponatts_hitscanattack.h"
#include "skill.h"

void CGenericHitscanWeapon::WeaponIdle()
{
	if ( m_pPrimaryAttackMode && m_pPrimaryAttackMode->Classify() == WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		m_pPlayer->GetAutoaimVector(static_cast<const WeaponAtts::WAHitscanAttack*>(m_pPrimaryAttackMode)->AutoAim);
	}

	CGenericWeapon::WeaponIdle();
}

void CGenericHitscanWeapon::PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode)
{
	CGenericWeapon::PrecacheAttackMode(attackMode);

	if ( attackMode.Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		ASSERT(false);
		return;
	}

	const WeaponAtts::WAHitscanAttack& hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack&>(attackMode);

	PRECACHE_MODEL(hitscanAttack.ShellModelName);
}

bool CGenericHitscanWeapon::InvokeWithAttackMode(const CGenericWeapon::WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode || attackMode->Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		return false;
	}

	const WeaponAtts::WAHitscanAttack* hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack*>(attackMode);

	if ( hitscanAttack->AttackRate <= 0.0f || hitscanAttack->BulletsPerShot < 1 )
	{
		return false;
	}

	// Check base class allows the attack:
	if ( !CGenericWeapon::InvokeWithAttackMode(type, hitscanAttack) )
	{
		return false;
	}

	DecrementAmmo(hitscanAttack, 1);

	m_pPlayer->pev->effects = (int)( m_pPlayer->pev->effects ) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_pPlayer->m_iWeaponVolume = hitscanAttack->Volume;
	m_pPlayer->m_iWeaponFlash = hitscanAttack->MuzzleFlashBrightness;

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming;

	if( hitscanAttack->AutoAim > 0.0f )
	{
		vecAiming = m_pPlayer->GetAutoaimVector(hitscanAttack->AutoAim);
	}
	else
	{
		vecAiming = gpGlobals->v_forward;
	}

	Vector vecDir = FireBulletsPlayer(*hitscanAttack, vecSrc, vecAiming);

	if ( m_AttackModeEvents[hitscanAttack->Signature()->Index] )
	{
		PLAYBACK_EVENT_FULL(DefaultEventFlags(),
							m_pPlayer->edict(),
							m_AttackModeEvents[hitscanAttack->Signature()->Index],
							0.0,
							(float *)&g_vecZero,
							(float *)&g_vecZero,
							vecDir.x,
							vecDir.y,
							m_pPlayer->random_seed,
							0,
							m_iClip == 0 ? 1 : 0,
							0);
	}

	DelayFiring(1.0f / hitscanAttack->AttackRate);

	if ( !HasAmmo(hitscanAttack, 1, true) && !HasAmmo(hitscanAttack, 1, false) )
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	SetNextIdleTime(5, true);
	return true;
}

Vector CGenericHitscanWeapon::FireBulletsPlayer(const WeaponAtts::WAHitscanAttack& hitscanAttack,
												const Vector& vecSrc,
										 		const Vector& vecDirShooting)
{
#ifdef CLIENT_DLL
	// The client doesn't actually do any bullet simulation, we just make sure that
	// the generated random vectors match up.
	return FireBulletsPlayer_Client(hitscanAttack);
#else
	TraceResult tr;
	Vector vecRight = gpGlobals->v_right;
	Vector vecUp = gpGlobals->v_up;
	float x = 0.0f;
	float y = 0.0f;

	entvars_t* const pevAttacker = m_pPlayer->pev;
	const int shared_rand = m_pPlayer->random_seed;

	ClearMultiDamage();
	gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;

	const uint32_t numShots = hitscanAttack.BulletsPerShot;
	for( uint32_t shot = 1; shot <= numShots; shot++ )
	{
		float damagePerShot = 1.0f;
		const WeaponAtts::WASkillRecord::SkillDataEntryPtr dmgPtr = hitscanAttack.BaseDamagePerShot;
		if ( dmgPtr )
		{
			damagePerShot = gSkillData.*dmgPtr;
		}

		GetSharedCircularGaussianSpread(shot, shared_rand, x, y);

		Vector vecDir = vecDirShooting +
						(x * hitscanAttack.SpreadX * vecRight) +
						(y * hitscanAttack.SpreadY * vecUp);
		Vector vecEnd;

		vecEnd = vecSrc + vecDir * DEFAULT_BULLET_TRACE_DISTANCE;
		UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev), &tr);

		// do damage, paint decals
		if( tr.flFraction != 1.0 )
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

			pEntity->TraceAttack(pevAttacker, damagePerShot, vecDir, &tr, DMG_BULLET);
			TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, BULLET_GENERIC);
		}

		// make bullet trails
		UTIL_BubbleTrail(vecSrc, tr.vecEndPos, (int)((DEFAULT_BULLET_TRACE_DISTANCE * tr.flFraction) / 64.0));
	}

	ApplyMultiDamage(pev, pevAttacker);

	return Vector(x * hitscanAttack.SpreadX, y * hitscanAttack.SpreadY, 0.0);
#endif
}

#ifdef CLIENT_DLL
Vector CGenericHitscanWeapon::FireBulletsPlayer_Client(const WeaponAtts::WAHitscanAttack& hitscanAttack)
{
	float x = 0, y = 0;

	const uint32_t numShots = hitscanAttack.BulletsPerShot;
	for( uint32_t shot = 1; shot <= numShots; shot++ )
	{
		GetSharedCircularGaussianSpread(shot, m_pPlayer->random_seed, x, y);
	}

	return Vector(x * hitscanAttack.SpreadX, y * hitscanAttack.SpreadY, 0.0);
}
#endif
