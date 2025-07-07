#pragma once

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
#include "rumble_shared.h"

#include "energyaccessor.h"

#include "basebludgeonweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_staplegun_firerate;
extern ConVar sk_staplegun_firerate_melee;
extern ConVar sk_plr_dmg_staplegun_melee;
extern ConVar sk_npc_dmg_staplegun_melee;
extern ConVar sk_staplegun_meleerange;
extern ConVar sk_max_staplegun_energy;

#ifdef MAPBASE
extern acttable_t* GetPistolActtable();
extern int GetPistolActtableCount();
#endif

class CWeaponStapleGun : public CBaseHLCombatWeapon, IBaseEnergyAccessor
{
	DECLARE_CLASS(CWeaponStapleGun, CBaseHLCombatWeapon);
public:

	CWeaponStapleGun();

	virtual WeaponClass_t WeaponClassify() { return WEPCLASS_HANDGUN; } //(it's also a melee weapon but it's alright)

	void	PrimaryAttack();
	void	SecondaryAttack();
	void	Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	float	WeaponAutoAimScale() { return 0.6f; }

	int		CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1 | bits_CAP_WEAPON_MELEE_ATTACK2; }

	int		GetMinBurst() { return 1; }
	int		GetMaxBurst() { return 1; }
	float	GetMinRestTime() { return 1.0f; }
	float	GetMaxRestTime() { return 2.5f; }

	//This weapon is both ranged and melee, so I merge a bit of CBaseHLBludgeonWeapon here
	void			ImpactEffect(trace_t& trace);
	bool			ImpactWater(const Vector& start, const Vector& end);
	void			Hit(trace_t& traceHit, Activity nHitActivity, bool bIsSecondary, bool bHitCrawler);
	Activity		ChooseIntersectionPointAndActivity(trace_t& hitTrace, const Vector& mins, const Vector& maxs, CBasePlayer* pOwner);
	float			GetRange() { return sk_staplegun_meleerange.GetFloat(); }
	float			GetDamageForActivity(Activity hitActivity);
	float			GetHitDelay() { return 0.f; }

	Activity		GetSecondaryAttackActivity();
	//bool			UsesSecondaryAmmo() { return false; }

	//Plays the special animation
	void			FatalitySequence(CBaseEntity* pCrawler, CBasePlayer* pPlayer);
	void			AddViewKick(int type);

	float GetFireRate() { return sk_staplegun_firerate.GetFloat(); }
	float GetFireRateMelee() { return sk_staplegun_firerate_melee.GetFloat(); }

	const Vector& GetBulletSpread()
	{
		static Vector cone = VECTOR_CONE_15DEGREES;

		//Scarlet is strugling so here it goes :
		if (!GetOwner() || !GetOwner()->IsNPC())
			return cone;

		cone = VECTOR_CONE_2DEGREES;

		return cone;
	}

	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);

	acttable_t* GetBackupActivityList() { return GetPistolActtable(); }
	virtual int				GetBackupActivityListCount() { return GetPistolActtableCount(); }

private:
	int		m_iEnergyCounter;


	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};