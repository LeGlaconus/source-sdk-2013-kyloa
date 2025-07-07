#include "cbase.h"
#include "basehlcombatweapon.h"
#include "energyaccessor.h"

ConVar sk_energyar_firerate("sk_energyar_firerate", "0", FCVAR_NONE);
ConVar sk_energyar_firerate_zoomed("sk_energyar_firerate_zoomed", "0", FCVAR_NONE);
ConVar sk_energyar_accuracy("sk_energyar_accuracy", "0", FCVAR_NONE);
ConVar sk_energyar_accuracy_zoomed("sk_energyar_accuracy_zoomed", "0", FCVAR_NONE);
ConVar sk_energyar_fov_zoomed("sk_energyar_fov_zoomed", "0", FCVAR_NONE);

constexpr int ENERGYAR_MAX_ENERGY = 16;

class CWeaponEnergyAr : public CHLMachineGun, IBaseEnergyAccessor
{
	DECLARE_CLASS(CWeaponEnergyAr, CHLMachineGun);

public:
	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

	CWeaponEnergyAr();

	virtual WeaponClass_t WeaponClassify() { return WEPCLASS_RIFLE; }

	int GetMinBurst() { return 2; }
	int GetMaxBurst() { return 5; }
	float GetFireRate() { return m_bZooming ? sk_energyar_firerate_zoomed.GetFloat() : sk_energyar_firerate.GetFloat(); }

	int CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	virtual const Vector& GetBulletSpread()
	{
		static Vector cone = m_bZooming ? VECTOR_CONE_2DEGREES : VECTOR_CONE_3DEGREES;
		return cone;
	}

	Vector CWeaponEnergyAr::GetBulletSpread(WeaponProficiency_t proficiency);
	const WeaponProficiencyInfo_t* GetProficiencyValues();
	const WeaponProficiencyInfo_t* GetProficiencyValuesZoomed();

	const char* GetTracerType() { return "AR2Tracer"; }

	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, bool bUseWeaponAngles);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	void	Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);

	void	AddViewKick();

	//Needs a fancier system for managing energy

	void	ItemPostFrame();
	void	ItemBusyFrame();
	void	CheckZoomToggle();
	void	ToggleZoom();
	void	StopZoomEffects();
	bool	Holster(CBaseCombatWeapon* pSwitchingTo = NULL); //Kyloa Confirm : is = NULL necessary ?

	void	PrimaryAttack();

	bool	Reload();

	Activity	GetPrimaryAttackActivity();

	void	DoImpactEffect(trace_t& tr, int nDamageType);

protected:
	int m_nVentPose; //useless for when the model will be changed
	bool m_bZooming;
};