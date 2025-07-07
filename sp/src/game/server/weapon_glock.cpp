//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		357 - hand gun
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "te_effect_dispatch.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// CWeaponGlock
//-----------------------------------------------------------------------------

ConVar sk_glock_firerate("sk_glock_firerate", "0", FCVAR_NONE);
ConVar sk_glock_firerate_secondary("sk_glock_firerate_secondary", "0", FCVAR_NONE);

#ifdef MAPBASE
extern acttable_t* GetPistolActtable();
extern int GetPistolActtableCount();
#endif

class CWeaponGlock : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponGlock, CBaseHLCombatWeapon);
public:

	CWeaponGlock();

	void	PrimaryAttack();
	void	SecondaryAttack();
	void	Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	float	WeaponAutoAimScale() { return 0.6f; }

#ifdef MAPBASE
	int		CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1 | bits_CAP_INNATE_MELEE_ATTACK2; }

	virtual int	GetMinBurst() { return 1; }
	virtual int	GetMaxBurst() { return 1; }
	virtual float	GetMinRestTime(void) { return 1.0f; }
	virtual float	GetMaxRestTime(void) { return 2.5f; }

	virtual float GetFireRate() { return sk_glock_firerate.GetFloat(); }
	virtual float GetFireRateSecondary() { return sk_glock_firerate_secondary.GetFloat(); }

	//Note : the primary attack should not have bullet spread
	virtual const Vector& GetBulletSpread()
	{
		static Vector cone = VECTOR_CONE_12_5DEGREES;

		return cone;
	}

	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void	FireNPCSecondaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);

	virtual acttable_t* GetBackupActivityList() { return GetPistolActtable(); }
	virtual int				GetBackupActivityListCount() { return GetPistolActtableCount(); }
#endif

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
#ifdef MAPBASE
	DECLARE_ACTTABLE();
#endif
};

LINK_ENTITY_TO_CLASS(weapon_glock, CWeaponGlock);

PRECACHE_WEAPON_REGISTER(weapon_glock);

IMPLEMENT_SERVERCLASS_ST(CWeaponGlock, DT_WeaponGlock)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponGlock)
END_DATADESC()

#ifdef MAPBASE
acttable_t	CWeaponGlock::m_acttable[] =
{
	{ ACT_IDLE,						ACT_IDLE_PISTOL,				true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_PISTOL,			true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD,					ACT_RELOAD_PISTOL,				true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_PISTOL,			true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_PISTOL,				true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_PISTOL,true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_PISTOL_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_PISTOL_LOW,	false },
	{ ACT_COVER_LOW,				ACT_COVER_PISTOL_LOW,			false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_PISTOL_LOW,		false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_WALK,						ACT_WALK_PISTOL,				false },
	{ ACT_RUN,						ACT_RUN_PISTOL,					false },

#ifdef MAPBASE
	// 
	// Activities ported from weapon_alyxgun below
	// 

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL_RELAXED,		false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_PISTOL_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_STEALTH,				ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_RELAXED,				ACT_WALK_PISTOL_RELAXED,		false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_PISTOL_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_STEALTH,				ACT_WALK_STEALTH_PISTOL,		false },

	{ ACT_RUN_RELAXED,				ACT_RUN_PISTOL_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_PISTOL_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_STEALTH,				ACT_RUN_STEALTH_PISTOL,			false },

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_PISTOL_RELAXED,		false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_PISTOL_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_AIM_STEALTH,			ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_PISTOL_RELAXED,		false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_PISTOL,			false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_AIM_STEALTH,			ACT_WALK_AIM_STEALTH_PISTOL,	false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_PISTOL_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_PISTOL,				false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_AIM_STEALTH,			ACT_RUN_AIM_STEALTH_PISTOL,		false },//always aims
	//End readiness activities
#else
	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL,				false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_STIMULATED,			false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_STEALTH,				ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_RELAXED,				ACT_WALK,						false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_STIMULATED,			false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_STEALTH,				ACT_WALK_STEALTH_PISTOL,		false },

	{ ACT_RUN_RELAXED,				ACT_RUN,						false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_STIMULATED,				false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_STEALTH,				ACT_RUN_STEALTH_PISTOL,			false },

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_PISTOL,				false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_ANGRY_PISTOL,			false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_AIM_STEALTH,			ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK,						false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_PISTOL,			false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_AIM_STEALTH,			ACT_WALK_AIM_STEALTH_PISTOL,	false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN,						false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_PISTOL,				false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_AIM_STEALTH,			ACT_RUN_AIM_STEALTH_PISTOL,		false },//always aims
	//End readiness activities
#endif

	// Crouch activities
	{ ACT_CROUCHIDLE_STIMULATED,	ACT_CROUCHIDLE_STIMULATED,		false },
	{ ACT_CROUCHIDLE_AIM_STIMULATED,ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims
	{ ACT_CROUCHIDLE_AGITATED,		ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims

	// Readiness translations
	{ ACT_READINESS_RELAXED_TO_STIMULATED, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED, false },
	{ ACT_READINESS_RELAXED_TO_STIMULATED_WALK, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK, false },
	{ ACT_READINESS_AGITATED_TO_STIMULATED, ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED, false },
	{ ACT_READINESS_STIMULATED_TO_RELAXED, ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED, false },
#endif

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_PISTOL,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_PISTOL,		true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_PISTOL,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_PISTOL,		true },
#endif

#if EXPANDED_HL2_COVER_ACTIVITIES
	{ ACT_RANGE_AIM_MED,			ACT_RANGE_AIM_PISTOL_MED,			false },
	{ ACT_RANGE_ATTACK1_MED,		ACT_RANGE_ATTACK_PISTOL_MED,		false },

	{ ACT_COVER_WALL_R,			ACT_COVER_WALL_R_PISTOL,		false },
	{ ACT_COVER_WALL_L,			ACT_COVER_WALL_L_PISTOL,		false },
	{ ACT_COVER_WALL_LOW_R,		ACT_COVER_WALL_LOW_R_PISTOL,	false },
	{ ACT_COVER_WALL_LOW_L,		ACT_COVER_WALL_LOW_L_PISTOL,	false },
#endif

#ifdef MAPBASE
	// HL2:DM activities (for third-person animations in SP)
	{ ACT_HL2MP_IDLE,                    ACT_HL2MP_IDLE_PISTOL,                    false },
	{ ACT_HL2MP_RUN,                    ACT_HL2MP_RUN_PISTOL,                    false },
	{ ACT_HL2MP_IDLE_CROUCH,            ACT_HL2MP_IDLE_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_WALK_CROUCH,            ACT_HL2MP_WALK_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,    ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,    false },
	{ ACT_HL2MP_GESTURE_RELOAD,            ACT_HL2MP_GESTURE_RELOAD_PISTOL,        false },
	{ ACT_HL2MP_JUMP,                    ACT_HL2MP_JUMP_PISTOL,                    false },
#if EXPANDED_HL2DM_ACTIVITIES
	{ ACT_HL2MP_WALK,					ACT_HL2MP_WALK_PISTOL,						false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK2,	ACT_HL2MP_GESTURE_RANGE_ATTACK2_PISTOL,		false },
#endif
#endif
};


IMPLEMENT_ACTTABLE(CWeaponGlock);

// Allows Weapon_BackupActivity() to access the 357's activity table.
acttable_t* GetGlockActtable()
{
	return CWeaponGlock::m_acttable;
}

int GetGlockActtableCount()
{
	return ARRAYSIZE(CWeaponGlock::m_acttable);
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponGlock::CWeaponGlock(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;

#ifdef MAPBASE
	m_fMinRange1 = 24;
	m_fMaxRange1 = 1000;
	m_fMinRange2 = 24;
	m_fMaxRange2 = 200;
#endif
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponGlock::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	switch (pEvent->event)
	{
	case EVENT_WEAPON_RELOAD:
	{
		CEffectData data;

		// Emit six spent shells
		for (int i = 0; i < 6; i++)
		{
			data.m_vOrigin = pOwner->WorldSpaceCenter() + RandomVector(-4, 4);
			data.m_vAngles = QAngle(90, random->RandomInt(0, 360), 0);
			data.m_nEntIndex = entindex();

			DispatchEffect("ShellEject", data);
		}

		break;
	}
#ifdef MAPBASE
	case EVENT_WEAPON_PISTOL_FIRE:
	{
		Vector vecShootOrigin, vecShootDir;
		vecShootOrigin = pOperator->Weapon_ShootPosition();

		CAI_BaseNPC* npc = pOperator->MyNPCPointer();
		ASSERT(npc != NULL);

		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

		FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
	}
	break;
	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
#endif
	}
}

#ifdef MAPBASE
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponGlock::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	WeaponSound(SINGLE_NPC);
	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1);
	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}

void CWeaponGlock::FireNPCSecondaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	//WeaponSound(SINGLE_NPC);
	WeaponSound(DOUBLE_NPC);
	//pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1);
	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_1DEGREES, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1);
	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}

//-----------------------------------------------------------------------------
// Purpose: Some things need this. (e.g. the new Force(X)Fire inputs or blindfire actbusy)
//-----------------------------------------------------------------------------
void CWeaponGlock::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	AngleVectors(angShootDir, &vecShootDir);
	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
}
#endif

void CWeaponGlock::SecondaryAttack()
{
	// Only the player fires this way so we can cast
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	WeaponSound(SINGLE);
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRateSecondary();

	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	//secondary attacks are infinite
	pPlayer->FireBullets(1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0);

	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner());
}

void CWeaponGlock::PrimaryAttack()
{
	// Only the player fires this way so we can cast
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	WeaponSound(SINGLE);
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	pPlayer->FireBullets(1, vecSrc, vecAiming, pPlayer->GetAttackSpread(this), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0);

	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner());
}