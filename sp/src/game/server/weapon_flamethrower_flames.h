#pragma once

#include "cbase.h"
#include "weapon_flamethrower.h"
#include "ai_basenpc.h"
#include "particle_system.h"
#include "fire.h"
#include "props.h"
#include "props_shared.h"
#include "physics_prop_ragdoll.h"

static ConVar ft_flame_velocity(	"ft_flame_velocity",	"2300.0",	FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_drag(		"ft_flame_drag",		"0.9",		FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_gravity(		"ft_flame_gravity",		"6",		FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_size(		"ft_flame_size",		"12",		FCVAR_CHEAT | FCVAR_REPLICATED, "Size (not radius) of the individual flames", true, 0.0f, false, 0.0f);
static ConVar ft_flame_lifetime(	"ft_flame_lifetime",	"1",		FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_dmg(			"ft_flame_dmg",			"2",		FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_vecrand(		"ft_flame_vecrand",		"0.06",		FCVAR_CHEAT | FCVAR_REPLICATED);
static ConVar ft_flame_heat(		"ft_flame_heat",		"0.5",		FCVAR_CHEAT | FCVAR_REPLICATED);

static ConVar ft_flame_burn_EVERYTHING("ft_flame_burn_EVERYTHING", "0", FCVAR_CHEAT, "BURN THEIR HOUSE DOWN");

DECLARE_AUTO_LIST(IFT_FlameAutoList);
class C_FT_Flame : public CBaseEntity, public IFT_FlameAutoList
{
	DECLARE_CLASS(C_FT_Flame, CBaseEntity);
public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	C_FT_Flame();
	virtual void Spawn();
	virtual void Think();

	int UpdateTransmitState()
	{
		return FL_EDICT_DONTSEND;
	}

	inline void KillFlame();

	void LightUpNearbyFires();
	void Collide(CBaseEntity* pOther);
	K_NOINLINE bool IsBurnable(CBaseAnimating* pEnt);

	static C_FT_Flame* Create(const Vector& vecOrigin, const QAngle& vecAngles, CBaseEntity* pOwner, float flSpeed);

	inline Vector GetBaseVelocity() { return m_vBaseVelocity; }
	inline void SetBaseVelocity(Vector& vBaseVelocity) { m_vBaseVelocity = vBaseVelocity; }

private:
	Vector	m_vInitialPos;
	Vector	m_vPrevPos;
	Vector	m_vBaseVelocity;
	Vector	m_vAddedImpulse; //Added velocity by the charachter holding the flamethrower
	CNetworkVar(float, m_flRemovalTime);
	//CUtlVector<EHANDLE> m_hBurntEntities; //Entities burnt by this flame
};

class C_FT_FlameEnumerator : public IEntityEnumerator
{
	DECLARE_CLASS(C_FT_Flame, IEntityEnumerator);
public:
	C_FT_FlameEnumerator(CBaseEntity* pIgnore);

	virtual bool EnumEntity(IHandleEntity* pHandleEntity);
	inline const CUtlVector<CBaseEntity*>& GetTargets() { return m_vecTargets; }

private:
	CBaseEntity* m_pIgnore;
	CUtlVector<CBaseEntity*> m_vecTargets;
};