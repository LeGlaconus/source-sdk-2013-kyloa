//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basecombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "weapon_ar2.h"
#include "grenade_ar2.h"
#include "gamerules.h"
#include "game.h"
#include "in_buttons.h"
#include "ai_memory.h"
#include "soundent.h"
#include "hl2_player.h"
#include "EntityFlame.h"
#include "weapon_flaregun.h"
#include "te_effect_dispatch.h"
#include "prop_combine_ball.h"
#include "beam_shared.h"
#include "npc_combine.h"
#include "rumble_shared.h"
#include "gamestats.h"
#ifdef MAPBASE
#include "npc_playercompanion.h"
#endif

#include "weapon_energyar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_weapon_ar2_alt_fire_radius("sk_weapon_ar2_alt_fire_radius", "10");
ConVar sk_weapon_ar2_alt_fire_duration("sk_weapon_ar2_alt_fire_duration", "2");
ConVar sk_weapon_ar2_alt_fire_mass("sk_weapon_ar2_alt_fire_mass", "150");

//=========================================================
//=========================================================

BEGIN_DATADESC(CWeaponEnergyAr)

//DEFINE_FIELD(m_flDelayedFire, FIELD_TIME),
//DEFINE_FIELD(m_bShotDelayed, FIELD_BOOLEAN),
//DEFINE_FIELD( m_nVentPose, FIELD_INTEGER ),

DEFINE_FIELD(m_bZooming, FIELD_BOOLEAN),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CWeaponEnergyAr, DT_WeaponEnergyAr)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(weapon_energyar, CWeaponEnergyAr);
PRECACHE_WEAPON_REGISTER(weapon_energyar);

acttable_t	CWeaponEnergyAr::m_acttable[] =
{
#if AR2_ACTIVITY_FIX == 1
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			true },
	{ ACT_RELOAD,					ACT_RELOAD_AR2,				true },
	{ ACT_IDLE,						ACT_IDLE_AR2,					true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_AR2,			false },

	{ ACT_WALK,						ACT_WALK_AR2,					true },

	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_AR2_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_AR2_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_AR2,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_AR2_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_AR2_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_AR2,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_AR2_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_AR2_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_AR2_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_AR2_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_AR2,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_AR2_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_AR2_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_AR2,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_AR2_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_AR2_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
	//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_AR2,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_AR2,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_AR2,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_AR2,	false },
	{ ACT_COVER_LOW,				ACT_COVER_AR2_LOW,				true },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_AR2_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_AR2_LOW,		false },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_AR2_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_AR2,		true },
	//	{ ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK_AR2_GRENADE, true },
	#else
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,				true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,			true },		// FIXME: hook to AR2 unique

	{ ACT_WALK,						ACT_WALK_RIFLE,					true },

	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SMG1_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SMG1_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_RIFLE_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_RIFLE_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_SMG1_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_RIFLE_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_RIFLE_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_RIFLE_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
	//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_RIFLE,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_AR2,	false },
	{ ACT_COVER_LOW,				ACT_COVER_SMG1_LOW,				false },		// FIXME: hook to AR2 unique
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_AR2_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SMG1_LOW,		true },		// FIXME: hook to AR2 unique
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,		true },
	//	{ ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK_AR2_GRENADE, true },
	#endif

	#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_ARM,						ACT_ARM_RIFLE,					false },
	{ ACT_DISARM,					ACT_DISARM_RIFLE,				false },
	#endif

	#if EXPANDED_HL2_COVER_ACTIVITIES
	{ ACT_RANGE_AIM_MED,			ACT_RANGE_AIM_AR2_MED,			false },
	{ ACT_RANGE_ATTACK1_MED,		ACT_RANGE_ATTACK_AR2_MED,		false },

	{ ACT_COVER_WALL_R,				ACT_COVER_WALL_R_RIFLE,			false },
	{ ACT_COVER_WALL_L,				ACT_COVER_WALL_L_RIFLE,			false },
	{ ACT_COVER_WALL_LOW_R,			ACT_COVER_WALL_LOW_R_RIFLE,		false },
	{ ACT_COVER_WALL_LOW_L,			ACT_COVER_WALL_LOW_L_RIFLE,		false },
	#endif

	#ifdef MAPBASE
	// HL2:DM activities (for third-person animations in SP)
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_AR2,                    false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_AR2,                    false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_AR2,            false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_AR2,            false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_AR2,    false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_AR2,        false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_AR2,                    false },
	#if EXPANDED_HL2DM_ACTIVITIES
	{ ACT_HL2MP_WALK,					ACT_HL2MP_WALK_AR2,						false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK2,	ACT_HL2MP_GESTURE_RANGE_ATTACK2_AR2,	false },
	#endif
	#endif
};

IMPLEMENT_ACTTABLE(CWeaponEnergyAr);

#ifdef MAPBASE
// Allows Weapon_BackupActivity() to access the AR2's activity table.
acttable_t* GetEnergyArActtable()
{
	return CWeaponEnergyAr::m_acttable;
}

int GetEnergyArActtableCount()
{
	return ARRAYSIZE(CWeaponEnergyAr::m_acttable);
}
#endif

CWeaponEnergyAr::CWeaponEnergyAr()
{
	//Ash only wants the clip to be shown, might do a fancier hud later
	//I also need to check if it's an energy weapon for weapon switching
	AddSpawnFlags(SF_ENERGY_WEAPON);
	
	//Temporary fix for it depleting alongside energy
	AddSpawnFlags(SF_WEAPON_NO_AUTO_SWITCH_WHEN_EMPTY);

	m_fMinRange1 = 65;
	m_fMaxRange1 = 2048;

	m_fMinRange2 = 256;
	m_fMaxRange2 = 1024;

	m_nShotsFired = 0;
	m_nVentPose = -1;

	m_bAltFiresUnderwater = false;

	m_bZooming = false;
}

bool CWeaponEnergyAr::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	StopZoomEffects();
	return BaseClass::Holster(pSwitchingTo);
}

void CWeaponEnergyAr::StopZoomEffects()
{
	if (m_bZooming)
	{
		ToggleZoom();
	}

	//Disable weapon effect / reticle
}

void CWeaponEnergyAr::ToggleZoom()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == nullptr)
		return;

	if (m_bZooming)
	{
		if (pPlayer->SetFOV(this, 0, 0.2f))
		{
			m_bZooming = false;
			WeaponSound(SPECIAL1);
		}
	}
	else
	{
		if (pPlayer->SetFOV(this, sk_energyar_fov_zoomed.GetInt(), 0.1f))
		{
			m_bZooming = true;
			WeaponSound(SPECIAL2);
		}
	}
}

void CWeaponEnergyAr::CheckZoomToggle()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer->m_afButtonPressed & IN_ATTACK2)
	{
		ToggleZoom();
	}
}

void CWeaponEnergyAr::ItemBusyFrame()
{
	CheckZoomToggle();
}

void CWeaponEnergyAr::ItemPostFrame()
{
	// See if we need to fire off our secondary round
	//if (m_bShotDelayed && gpGlobals->curtime > m_flDelayedFire)
	//{
	//	DelayedAttack();
	//}

	// Update our pose parameter for the vents
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (pOwner)
	{
		CBaseViewModel* pVM = pOwner->GetViewModel();


		//Kyloa : useless for when the viewmodel will be changed
		if (pVM)
		{
			if (m_nVentPose == -1)
			{
				m_nVentPose = pVM->LookupPoseParameter("VentPoses");
			}

			float flVentPose = RemapValClamped(m_nShotsFired, 0, 5, 0.0f, 1.0f);
			pVM->SetPoseParameter(m_nVentPose, flVentPose);
		}
	}

	CheckZoomToggle();

	//Kyloa : update energy count
	m_iClip1 = GetEnergy();

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponEnergyAr::GetPrimaryAttackActivity()
{
	if (m_nShotsFired < 2)
		return ACT_VM_PRIMARYATTACK;

	if (m_nShotsFired < 3)
		return ACT_VM_RECOIL1;

	if (m_nShotsFired < 4)
		return ACT_VM_RECOIL2;

	return ACT_VM_RECOIL3;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &tr - 
//			nDamageType - 
//-----------------------------------------------------------------------------
void CWeaponEnergyAr::DoImpactEffect(trace_t& tr, int nDamageType)
{
	CEffectData data;

	data.m_vOrigin = tr.endpos + (tr.plane.normal * 1.0f);
	data.m_vNormal = tr.plane.normal;

	DispatchEffect("AR2Impact", data);

	BaseClass::DoImpactEffect(tr, nDamageType);
}

void CWeaponEnergyAr::PrimaryAttack()
{
	// Only the player fires this way so we can cast
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	// Abort here to handle burst and auto fire modes
	if ((UsesClipsForAmmo1() && m_iClip1 == 0) || (!UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType)))
		return;

	m_nShotsFired++;

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = GetFireRate();

	// MUST call sound before removing a round from the clip of a CHLMachineGun
	while (m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if (UsesClipsForAmmo1())
	{
		if (iBulletsToFire > m_iClip1)
			iBulletsToFire = m_iClip1;
		m_iClip1 -= iBulletsToFire;
		AddEnergy(-1 * iBulletsToFire);
		//DevMsg("m_iClip : %i \n", (int)m_iClip1);
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	// Fire the bullets
	FireBulletsInfo_t info;
	info.m_iShots = iBulletsToFire;
	info.m_vecSrc = pPlayer->Weapon_ShootPosition();
	info.m_vecDirShooting = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);
	info.m_vecSpread = pPlayer->GetAttackSpread(this);
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	FireBullets(info);

	//Factor in the view kick
	AddViewKick();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer);

	SendWeaponAnim(GetPrimaryAttackActivity());
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	// Register a muzzleflash for the AI
	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);
}

//-----------------------------------------------------------------------------
// Purpose: Override if we're waiting to release a shot
//-----------------------------------------------------------------------------
bool CWeaponEnergyAr::Reload()
{

	//Kyloa TODO Confirm : should always be false ?

	if (GetEnergy() <= 0)
		return false;

	return BaseClass::Reload();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOperator - 
//-----------------------------------------------------------------------------
void CWeaponEnergyAr::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, bool bUseWeaponAngles)
{
	Vector vecShootOrigin, vecShootDir;

	CAI_BaseNPC* npc = pOperator->MyNPCPointer();
	ASSERT(npc != NULL);

	if (bUseWeaponAngles)
	{
		QAngle	angShootDir;
		GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
		AngleVectors(angShootDir, &vecShootDir);
	}
	else
	{
		vecShootOrigin = pOperator->Weapon_ShootPosition();
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);
	}

	WeaponSoundRealtime(SINGLE_NPC);

	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2);

	// NOTENOTE: This is overriden on the client-side
	// pOperator->DoMuzzleFlash();

	--m_iClip1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponEnergyAr::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	if (bSecondary)
	{
		//Kyloa : never call
		//FireNPCSecondaryAttack(pOperator, true);
	}
	else
	{
		// Ensure we have enough rounds in the clip
		//Kyloa TODO Confirm : Change this for energy ?
		m_iClip1++;

		FireNPCPrimaryAttack(pOperator, true);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pEvent - 
//			*pOperator - 
//-----------------------------------------------------------------------------
void CWeaponEnergyAr::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	switch (pEvent->event)
	{
	case EVENT_WEAPON_AR2:
	{
		FireNPCPrimaryAttack(pOperator, false);
	}
	break;

	default:
		CBaseCombatWeapon::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponEnergyAr::AddViewKick()
{
#define	EASY_DAMPEN			0.5f
#define	MAX_VERTICAL_KICK	8.0f	//Degrees
#define	SLIDE_LIMIT			5.0f	//Seconds

	//Get the view kick
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	float flDuration = m_fFireDuration;

	if (g_pGameRules->GetAutoAimMode() == AUTOAIM_ON_CONSOLE)
	{
		// On the 360 (or in any configuration using the 360 aiming scheme), don't let the
		// AR2 progressive into the late, highly inaccurate stages of its kick. Just
		// spoof the time to make it look (to the kicking code) like we haven't been
		// firing for very long.
		flDuration = MIN(flDuration, 0.75f);
	}

	DoMachineGunKick(pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, flDuration, SLIDE_LIMIT);
}

//-----------------------------------------------------------------------------
Vector CWeaponEnergyAr::GetBulletSpread(WeaponProficiency_t proficiency)
{
	//Vector baseSpread = BaseClass::GetBulletSpread(proficiency);
	Vector baseSpread = BaseClass::GetBulletSpread();

	//bool yes = true;
	//DevMsg("yes or no : %s \n", yes ? "yes" : "no");
	const WeaponProficiencyInfo_t* pProficiencyValues = m_bZooming ? GetProficiencyValuesZoomed() : GetProficiencyValues();
	float flModifier = (pProficiencyValues)[proficiency].spreadscale;
	return (baseSpread * flModifier);
}

const WeaponProficiencyInfo_t* CWeaponEnergyAr::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.5	},
		{ 5.00,		0.5	},
		{ 3.0,		0.5},
		{ 5.0 / 3.0, 0.5},
		{ 1.00,		1.0	},
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}

const WeaponProficiencyInfo_t* CWeaponEnergyAr::GetProficiencyValuesZoomed()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.9	},
		{ 5.00,		0.9	},
		{ 3.0,		0.9	},
		{ 5.0 / 3.0,0.9 },
		{ 1.00,		1.0	},
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}