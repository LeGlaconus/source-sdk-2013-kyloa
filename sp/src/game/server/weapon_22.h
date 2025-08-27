#pragma once

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "ai_basenpc.h"
#include "gamestats.h"
#include "hl2_gamerules.h"
#include "npcevent.h"
#include "te_effect_dispatch.h"
#include "ammodef.h"
#include "in_buttons.h"

ConVar sk_22_firerate("sk_22_firerate", "0", FCVAR_NONE);
ConVar sk_22_firerate_hammer_penalty("sk_22_firerate_hammer_penalty", "0", FCVAR_NONE);
ConVar sk_22_firerate_real("sk_22_firerate_real", "0", FCVAR_NONE);

ConVar g_debug_22("g_debug_22", "0", FCVAR_ARCHIVE, "Set to 1 for Hammer info, set to 2 for Hammer + penalty info", true, 0, true, 2);

extern acttable_t* Get22Acttable();
extern int Get22ActtableCount();

#define WEAPON_22_HAMMER_DELAY	0.45f
#define WEAPON_22_ACCURACY_PENALTY_MIN -0.5f
#define WEAPON_22_ACCURACY_PENALTY_MAX 1.5f
#define WEAPON_22_PERFECT_ACCURACY_TRESHOLD -0.35f

class CWeapon22 : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeapon22, CBaseHLCombatWeapon);
public:

	CWeapon22();

	void PrimaryAttack();
	void ItemPostFrame();
	void ItemBusyFrame();
	bool Holster(CBaseCombatWeapon* pSwitchingTo);
	bool Reload();
	void Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	void CheckToggleHammer();
	void ToggleHammer();

	void AddViewKick();

	int CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	virtual int GetMinBurst() { return 1; }
	virtual int GetMaxBurst() { return 1; }

	virtual float GetFireRate() { return m_bHammerDown ? (sk_22_firerate.GetFloat() + sk_22_firerate_hammer_penalty.GetFloat()) : sk_22_firerate.GetFloat(); }

	virtual const Vector& GetBulletSpread()
	{
		static Vector cone;
		if (m_bPerfectShot)
			cone = vec3_origin;
		else if (m_bHammerDown)
			cone = VECTOR_CONE_3DEGREES;
		else
			cone = VECTOR_CONE_12_5DEGREES;

		if (!GetOwner() || !GetOwner()->IsNPC())
			return cone;

		static Vector AllyCone = VECTOR_CONE_4DEGREES;
		static Vector NPCCone = VECTOR_CONE_5DEGREES;

		if (GetOwner()->MyNPCPointer()->IsPlayerAlly())
		{
			// 22 allies should be cooler
			return AllyCone;
		}

		return NPCCone;
	}

	void FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	//void FireNPCSecondaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);

	virtual acttable_t* GetBackupActivityList() { return Get22Acttable(); }
	virtual int				GetBackupActivityListCount() { return Get22ActtableCount(); }

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

private:
	bool m_bHammerDown = false;
	float m_flHammerDelay = 0.0f;
	int m_iHammerAmmoType = 0;

	bool m_bQuickShot = false;
	bool m_bPerfectShot = false;

	float m_flAccuracyPenalty = 0.0f;
	float m_flSoonestPrimaryAttack = 0.0f;
};

