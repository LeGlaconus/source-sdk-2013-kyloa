#pragma once

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "ai_basenpc.h"

ConVar sk_22_firerate("sk_22_firerate", "0", FCVAR_NONE);

extern acttable_t* Get22Acttable();
extern int Get22ActtableCount();

class CWeapon22 : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeapon22, CBaseHLCombatWeapon);
public:

	CWeapon22();

	void PrimaryAttack();
	void SecondaryAttack();
	void Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	int CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1 | bits_CAP_WEAPON_RANGE_ATTACK2; }

	virtual int GetMinBurst() { return 1; }
	virtual int GetMaxBurst() { return 1; }

	virtual float GetFireRate() { return sk_22_firerate.GetFloat(); }

	virtual const Vector& GetBulletSpread()
	{
		static Vector cone = VECTOR_CONE_15DEGREES;
		if (!GetOwner() || !GetOwner()->IsNPC())
			return cone;

		static Vector AllyCone = VECTOR_CONE_2DEGREES;
		static Vector NPCCone = VECTOR_CONE_5DEGREES;

		if (GetOwner()->MyNPCPointer()->IsPlayerAlly())
		{
			// 22 allies should be cooler
			return AllyCone;
		}

		return NPCCone;
	}

	void FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void FireNPCSecondaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);

	virtual acttable_t* GetBackupActivityList() { return Get22Acttable(); }
	virtual int				GetBackupActivityListCount() { return Get22ActtableCount(); }

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

void CWeapon22::PrimaryAttack()
{
	BaseClass::PrimaryAttack();
}

void CWeapon22::SecondaryAttack()
{
	BaseClass::SecondaryAttack();
}

void CWeapon22::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
}

void CWeapon22::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	BaseClass::Operator_ForceNPCFire(pOperator, bSecondary);
}

