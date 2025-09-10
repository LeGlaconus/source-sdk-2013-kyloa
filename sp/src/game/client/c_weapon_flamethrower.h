#pragma once

#include "cbase.h"
#include "c_basecombatweapon.h"
#include "particlemgr.h"
#include "c_weapon__stubs.h"

//Double declaration but who cares
enum class FT_State
{
	Idle = 0,
	Primary,
	Secondary,
};

class C_WeaponFlamethrower : public C_BaseCombatWeapon
{
	DECLARE_CLASS(C_WeaponFlamethrower, C_BaseCombatWeapon);
public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_WeaponFlamethrower();
	~C_WeaponFlamethrower();

	virtual void OnDataChanged(DataUpdateType_t updateType);
	virtual void ClientThink();

	void StartEmitting();
	void StopEmitting();
	
private:
	CNewParticleEffect* m_pFlameJet;

	FT_State			m_eState;
	bool				m_bEmitting;
};