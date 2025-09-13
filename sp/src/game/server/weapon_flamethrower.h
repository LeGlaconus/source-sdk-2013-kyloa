#pragma once

#include "basehlcombatweapon.h"
#include "cbase.h"
#include "weapon_flamethrower_flames.h"
#include "in_buttons.h"
#include "particle_parse.h"
#include "soundenvelope.h"

extern acttable_t* GetPistolActtable();
extern int GetPistolActtableCount();

static ConVar ft_fireeject_interval(			"ft_fireeject_interval",			"0.1",	FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_fireeject_interval_secondary(	"ft_fireeject_interval_secondary",	"0.35", FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_secondary_min_warmup_time(		"ft_secondary_min_warmup_time",		"0.25",	FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_secondary_max_warmup_time(		"ft_secondary_max_warmup_time",		"1.3",	FCVAR_CHEAT | FCVAR_REPLICATED);

static ConVar g_debug_ft("g_debug_ft", "0", FCVAR_CHEAT | FCVAR_ARCHIVE, "Shows debug info about the flamethrower and the flames");

enum class FT_State
{
	Idle = 0,
	Primary,
	Secondary,
};

class C_FT_Flame;

class CWeaponFlamethrower : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponFlamethrower, CBaseHLCombatWeapon);
public:
//	DECLARE_ACTTABLE();
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CWeaponFlamethrower();
	~CWeaponFlamethrower();

	void Precache();
	
	void Spawn();

	int CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1 | bits_CAP_WEAPON_RANGE_ATTACK2; }
	WeaponClass_t WeaponClassify() { return WEPCLASS_HEAVY; }

	void ItemPostFrame();
	C_FT_Flame* ThrowFlame(CBasePlayer* pPlayer, Vector& vShootPosition, QAngle& vShootDir);
	void PrimaryAttack();
	K_NOINLINE void SecondaryAttackWarmup();
	K_NOINLINE void SecondaryAttack(float flLength);
	K_NOINLINE void SecondaryAttackRelease();

	void WeaponIdle();
	
	void StopFireEjectSound();

	bool UsesClipsForAmmo1() const { return false; }

	#ifdef MAPBASE
	virtual acttable_t*		GetBackupActivityList()			{ return GetPistolActtable();		}
	virtual int				GetBackupActivityListCount()	{ return GetPistolActtableCount();	}
	#endif

private:
	float	m_flFireEjectStopTime;
	float	m_flNextFireEjectTime;
	float	m_flWarmupTime;

	CSoundPatch* m_pFireEjectSoundPatch;

	CNetworkVar(FT_State, m_eState);
};