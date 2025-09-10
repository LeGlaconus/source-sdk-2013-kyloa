//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FIRE_H
#define FIRE_H
#ifdef _WIN32
#pragma once
#endif

#include "entityoutput.h"
#include "fire_smoke.h"
#include "plasma.h"

//Spawnflags
#define	SF_FIRE_INFINITE			0x00000001
#define	SF_FIRE_SMOKELESS			0x00000002
#define	SF_FIRE_START_ON			0x00000004
#define	SF_FIRE_START_FULL			0x00000008
#define SF_FIRE_DONT_DROP			0x00000010
#define	SF_FIRE_NO_GLOW				0x00000020
#define SF_FIRE_DIE_PERMANENT		0x00000080
#define SF_FIRE_VISIBLE_FROM_ABOVE	0x00000100

//==================================================
// CFire
//==================================================

enum fireType_e
{
	FIRE_NATURAL = 0,
	FIRE_PLASMA,
};

class CFire : public CBaseEntity
{
public:
	DECLARE_CLASS(CFire, CBaseEntity);

	int DrawDebugTextOverlays(void);

	CFire(void);

	virtual void UpdateOnRemove(void);

	void	Precache(void);
	void	Init(const Vector& position, float scale, float attackTime, float fuel, int flags, int fireType);
	bool	GoOut();

	void	BurnThink();
	void	GoOutThink();
	void	GoOutInSeconds(float seconds);

	void	SetOwner(CBaseEntity* hOwner) { m_hOwner = hOwner; }

	void	Scale(float end, float time);
	void	AddHeat(float heat, bool selfHeat = false, bool bPlayFTSound = false);
	int		OnTakeDamage(const CTakeDamageInfo& info);

	bool	IsBurning(void) const;

	bool	GetFireDimensions(Vector* pFireMins, Vector* pFireMaxs);

	void	Extinguish(float heat);
	void	DestroyEffect();

	virtual	void Update(float simTime);

	void	Spawn(void);
	void	Activate(void);
	void	StartFire(bool bPlayFTSound = false);
	void	Start();
	void	SetToOutSize()
	{
		UTIL_SetSize(this, Vector(-8, -8, 0), Vector(8, 8, 8));
	}

	float	GetHeatLevel() { return m_flHeatLevel; }

	virtual int UpdateTransmitState();

	void DrawDebugGeometryOverlays(void)
	{
		if (m_debugOverlays & OVERLAY_BBOX_BIT)
		{
			if (m_lastDamage > gpGlobals->curtime && m_flHeatAbsorb > 0)
			{
				NDebugOverlay::EntityBounds(this, 88, 255, 128, 0, 0);
				char tempstr[512];
				Q_snprintf(tempstr, sizeof(tempstr), "Heat: %.1f", m_flHeatAbsorb);
				EntityText(1, tempstr, 0);
			}
			else if (!IsBurning())
			{
				NDebugOverlay::EntityBounds(this, 88, 88, 128, 0, 0);
			}

			if (IsBurning())
			{
				Vector mins, maxs;
				if (GetFireDimensions(&mins, &maxs))
				{
					NDebugOverlay::Box(GetAbsOrigin(), mins, maxs, 128, 0, 0, 10, 0);
				}
			}


		}
		BaseClass::DrawDebugGeometryOverlays();
	}

	void Disable();

	//Inputs
	void	InputStartFire(inputdata_t& inputdata);
	void	InputExtinguish(inputdata_t& inputdata);
	void	InputExtinguishTemporary(inputdata_t& inputdata);
	void	InputEnable(inputdata_t& inputdata);
	void	InputDisable(inputdata_t& inputdata);

protected:

	void	Spread(void);
	void	SpawnEffect(fireType_e type, float scale);

	CHandle<CBaseFire>	m_hEffect;
	EHANDLE		m_hOwner;

	int		m_nFireType;

	float	m_flFuel;
	float	m_flDamageTime;
	float	m_lastDamage;
	float	m_flFireSize;	// size of the fire in world units

	float	m_flHeatLevel;	// Used as a "health" for the fire.  > 0 means the fire is burning
	float	m_flHeatAbsorb;	// This much heat must be "absorbed" before it gets transferred to the flame size
	float	m_flDamageScale;

	float	m_flMaxHeat;
	float	m_flLastHeatLevel;

	//NOTENOTE: Lifetime is an expression of the sum total of these amounts plus the global time when started
	float	m_flAttackTime;	//Amount of time to scale up

	bool	m_bEnabled;
	bool	m_bStartDisabled;
	bool	m_bDidActivate;


	COutputEvent	m_OnIgnited;
	COutputEvent	m_OnExtinguished;

	DECLARE_DATADESC();
};

//==================================================

// NPCs and grates do not prevent fire from travelling
#define	MASK_FIRE_SOLID	 ( MASK_SOLID & (~(CONTENTS_MONSTER|CONTENTS_GRATE)) )

//==================================================
// FireSystem
//==================================================
int FireSystem_GetFiresInSphere(CFire** pList, int listMax, bool onlyActiveFires, const Vector& origin, float radius);
bool FireSystem_StartFire(CBaseAnimating* pEntity, float fireHeight, float attack, float fuel, int flags, CBaseEntity* owner, fireType_e type);
bool FireSystem_StartFire(const Vector& position, float fireHeight, float attack, float fuel, int flags, CBaseEntity* owner, fireType_e type);

bool FireSystem_StartFire( const Vector &position, float fireHeight, float attack, float fuel, int flags, CBaseEntity *owner, fireType_e type = FIRE_NATURAL);
bool FireSystem_AddHeatInRadius( const Vector &origin, float radius, float heat ); //Kyloa : changed return type from void to bool to indicate if heat was added to a fire or nah
bool FireSystem_ExtinguishInRadius( const Vector &origin, float radius, float rate ); //Kyloa : same thing here

bool FireSystem_GetFireDamageDimensions( CBaseEntity *pFire, Vector *pFireMins, Vector *pFireMaxs );

#endif // FIRE_H
