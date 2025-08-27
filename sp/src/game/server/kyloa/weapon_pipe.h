#pragma once

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "basebludgeonweapon.h"
#include "ai_basenpc.h"
#include "npcevent.h"

ConVar sk_pipe_firerate("sk_pipe_firerate", "0", FCVAR_NONE);
ConVar sk_pipe_firerate_secondary("sk_pipe_firerate_secondary", "0", FCVAR_NONE);
ConVar sk_pipe_range("sk_pipe_range", "0", FCVAR_NONE);
ConVar sk_pipe_range_secondary("sk_pipe_range_secondary", "0", FCVAR_NONE);

class CWeaponPipe : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS(CWeaponPipe, CBaseHLBludgeonWeapon);
	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	void		SecondaryAttack();
	void		AddViewKick();
	float		GetDamageForActivity(Activity hitActivity);

	int			CapabilitiesGet() { return bits_CAP_WEAPON_MELEE_ATTACK1 | bits_CAP_WEAPON_MELEE_ATTACK2; }
	float		GetRange() { return	m_bBoostedHit ? sk_pipe_range_secondary.GetFloat() : sk_pipe_range.GetFloat(); }
	float		GetFireRate() { return m_bBoostedHit ? sk_pipe_firerate_secondary.GetFloat() : sk_pipe_firerate.GetFloat(); }

	// Don't use backup activities
	acttable_t* GetBackupActivityList() { return NULL; }
	int			GetBackupActivityListCount() { return 0; }

private:
	bool m_bBoostedHit = false;
};