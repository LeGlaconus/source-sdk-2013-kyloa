#include "cbase.h"
#include "basehlcombatweapon.h"

class CWeaponMP7 : public CHLSelectFireMachineGun
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS(CWeaponMP7, CHLSelectFireMachineGun);

	CWeaponMP7();

	DECLARE_SERVERCLASS();

	void	Precache(void);
	void	AddViewKick(void);
	void	SecondaryAttack(void);

	int		GetMinBurst() { return 2; }
	int		GetMaxBurst() { return 5; }

	virtual void Equip(CBaseCombatCharacter* pOwner);
	bool	Reload(void);

	float	GetFireRate(void) { return 0.075f; }	// 13.3hz
	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	int		WeaponRangeAttack2Condition(float flDot, float flDist);
	Activity	GetPrimaryAttackActivity(void);

	virtual const Vector& GetBulletSpread(void)
	{
		static const Vector cone = VECTOR_CONE_5DEGREES;
		return cone;
	}

	const WeaponProficiencyInfo_t* GetProficiencyValues();

	void FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	void Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	DECLARE_ACTTABLE();

protected:

	Vector	m_vecTossVelocity;
	float	m_flNextGrenadeCheck;
};