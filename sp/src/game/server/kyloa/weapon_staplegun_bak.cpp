//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "game.h"
#include "in_buttons.h"
#include "ai_memory.h"
#include "soundent.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "weapon_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_staplegun_firerate( "sk_staplegun_firerate", "0.75" );

//-----------------------------------------------------------------------------
// CWeaponStapleGun
//-----------------------------------------------------------------------------
class CWeaponStapleGun : public CBaseHLCombatWeapon
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS(CWeaponStapleGun, CBaseHLCombatWeapon);

	CWeaponStapleGun();

	DECLARE_SERVERCLASS();

	void			Spawn();
	void			AddViewKick();
	void			PrimaryAttack();
	void			SecondaryAttack();

	//void			ItemPostFrame();

	virtual int	GetMinBurst() { return 1; }
	virtual int	GetMaxBurst() { return 2; } //Confirm : is this relevant ?
	virtual float GetFireRate() { return sk_staplegun_firerate.GetFloat(); }
	virtual const Vector& GetBulletSpread()
	{
		static Vector cone = VECTOR_CONE_3DEGREES;
		return cone;
	}

	//virtual void	Equip(CBaseCombatCharacter* pOwner);
	bool			Reload();

	int				CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	int				WeaponRangeAttack2Condition(float flDot, float flDist);
	Activity		GetPrimaryAttackActivity();


	//void FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	//void Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	void Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	DECLARE_ACTTABLE();

private:
	int		m_nNumShotsFired;
};

IMPLEMENT_SERVERCLASS_ST(CWeaponStapleGun, DT_WeaponStapleGun)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(weapon_staplegun, CWeaponStapleGun);
PRECACHE_WEAPON_REGISTER(weapon_staplegun);

BEGIN_DATADESC(CWeaponStapleGun)
DEFINE_FIELD(m_nNumShotsFired, FIELD_INTEGER)
END_DATADESC()

acttable_t	CWeaponStapleGun::m_acttable[] =
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
};

IMPLEMENT_ACTTABLE(CWeaponStapleGun);

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CWeaponStapleGun::CWeaponStapleGun()
{
	m_fMinRange1 = 24;
	m_fMaxRange1 = 1500;
	m_fMinRange2 = 24;
	m_fMaxRange2 = 200;

	m_bFiresUnderwater = true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponStapleGun::Spawn()
{
	BaseClass::Spawn();
}

void CWeaponStapleGun::PrimaryAttack(void)
{

	m_nNumShotsFired++;

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, GetOwner());

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (pOwner)
	{
		// Each time the player fires the pistol, reset the view punch. This prevents
		// the aim from 'drifting off' when the player fires very quickly. This may
		// not be the ideal way to achieve this, but it's cheap and it works, which is
		// great for a feature we're evaluating. (sjb)
		pOwner->ViewPunchReset();
	}

	BaseClass::PrimaryAttack();

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pOwner, true, GetClassname());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
//void CWeaponStapleGun::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
//{
//	// FIXME: Use the returned number of bullets to account for >10hz firerate
//	WeaponSoundRealtime(SINGLE_NPC);
//
//	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());
//	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2, entindex(), 0);
//
//	pOperator->DoMuzzleFlash();
//	m_iClip1--;
//}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
//void CWeaponStapleGun::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
//{
//	// Ensure we have enough rounds in the magazine
//	m_iClip1++;
//
//	Vector vecShootOrigin, vecShootDir;
//	QAngle angShootDir;
//	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
//	AngleVectors(angShootDir, &vecShootDir);
//	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
//}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponStapleGun::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	switch (pEvent->event)
	{
	case EVENT_WEAPON_PISTOL_FIRE:
	{
		Vector vecShootOrigin, vecShootDir;
		vecShootOrigin = pOperator->Weapon_ShootPosition();

		CAI_BaseNPC* npc = pOperator->MyNPCPointer();
		ASSERT(npc != NULL);

		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

		CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

		WeaponSound(SINGLE_NPC);
		pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2);
		pOperator->DoMuzzleFlash();
		m_iClip1 = m_iClip1 - 1;
	}
	break;
	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponStapleGun::GetPrimaryAttackActivity()
{
	if (m_nNumShotsFired < 1)
		return ACT_VM_PRIMARYATTACK;

	if (m_nNumShotsFired < 2)
		return ACT_VM_RECOIL1;

	return ACT_VM_RECOIL2;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CWeaponStapleGun::Reload()
{
	//Kyloa : is that correct ? confirm
	m_nNumShotsFired = 0;

	bool fRet = DefaultReload(GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD);
	if (fRet)
	{
		WeaponSound(RELOAD);
	}
	return fRet;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponStapleGun::AddViewKick()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	QAngle	viewPunch;

	viewPunch.x = random->RandomFloat(0.25f, 0.5f);
	viewPunch.y = random->RandomFloat(-.6f, .6f);
	viewPunch.z = 0.0f;

	//Add it to the view punch
	pPlayer->ViewPunch(viewPunch);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponStapleGun::SecondaryAttack()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : flDot -
//          flDist -
// Output : int
//-----------------------------------------------------------------------------
int CWeaponStapleGun::WeaponRangeAttack2Condition(float flDot, float flDist)
{
	return COND_NONE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
//const WeaponProficiencyInfo_t* CWeaponStapleGun::GetProficiencyValues()
//{
//	static WeaponProficiencyInfo_t proficiencyTable[] =
//	{
//		{ 7.0,		0.75	},
//		{ 5.00,		0.75	},
//		{ 10.0 / 3.0, 0.75	},
//		{ 5.0 / 3.0,	0.75	},
//		{ 1.00,		1.0		},
//	};
//
//	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);
//
//	return proficiencyTable;
//}

//Added for every weapon

//class CWeaponStapleGunWeaponInfo : public FileWeaponInfo_t
//{
//	DECLARE_CLASS_GAMEROOT(CWeaponStapleGunWeaponInfo, FileWeaponInfo_t);
//
//	void Parse(::KeyValues* pKeyValuesData, const char* szWeaponName)
//	{
//		BaseClass::Parse(pKeyValuesData, szWeaponName);
//
//		m_flFireRate = pKeyValuesData->GetFloat("FireRate", 0.75f);
//	}
//
//	float	m_flFireRate;
//};

//FileWeaponInfo_t* CreateWeaponInfo()
//{
//	return new CWeaponStapleGunWeaponInfo;
//}