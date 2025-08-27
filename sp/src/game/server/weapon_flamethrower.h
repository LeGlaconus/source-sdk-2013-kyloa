#pragma once

#include "basehlcombatweapon.h"
#include "cbase.h"
#include "simple_physics.h"
#include "tier1/utlvector.h"
#include "saverestore_utlvector.h"

#define FT_MAX_FLAMES 16u
#define FT_FLAME_DEFAULT_LIFETIME 2.0f

//Prototyping only
#define FT_FLAME_SPRITE "sprites/flatflame.vmt"

ConVar g_debug_ft("g_debug_ft", "0", FCVAR_CHEAT | FCVAR_ARCHIVE, "Shows debug info about the flamethrower and the flames");

class CWeaponFlamethrower;

class C_FT_FlameElement : public CSimplePhysics::CNode
{
	DECLARE_CLASS(C_FT_FlameElement, CSimplePhysics::CNode);
public:
	DECLARE_DATADESC();

	C_FT_FlameElement()
		:m_flLifetime(FT_FLAME_DEFAULT_LIFETIME), m_bActive(true)
	{}

	C_FT_FlameElement(Vector& vPos, float _flLifetime = FT_FLAME_DEFAULT_LIFETIME, bool _bActive = true)
		:m_flLifetime(_flLifetime), m_bActive(_bActive)
	{
		Init(vPos);
	}

	C_FT_FlameElement(C_FT_FlameElement& _element)
		:m_flLifetime(_element.m_flLifetime), m_bActive(_element.m_bActive)
	{
		m_vPos = _element.m_vPos;
		m_vPrevPos = _element.m_vPrevPos;
		m_vPredicted = _element.m_vPredicted;
	}

	C_FT_FlameElement(const C_FT_FlameElement& _element)
		:m_flLifetime(_element.m_flLifetime), m_bActive(_element.m_bActive)
	{
		m_vPos = _element.m_vPos;
		m_vPrevPos = _element.m_vPrevPos;
		m_vPredicted = _element.m_vPredicted;
	}

	~C_FT_FlameElement()
	{
		m_vPos = m_vPredicted = m_vPrevPos = vec3_origin;
		m_bActive = false;
		m_flLifetime = -1.0f;
	}

	void* operator[](size_t idx);
	const void* operator[](size_t idx) const;

	void Delete();

	inline bool IsActive() { return m_bActive; }

	inline CSimplePhysics::CNode* GetNode() { return static_cast<CSimplePhysics::CNode*>(this); }

	float m_flLifetime = 0.0f;
	bool m_bActive = false;
};

class C_FT_FlameManager
{
	DECLARE_CLASS_NOBASE(C_FT_FlameManager);
public:

	DECLARE_DATADESC();

	C_FT_FlameManager()
		:m_hPlayer(nullptr)
	{
		m_Flames.EnsureCapacity(FT_MAX_FLAMES);
		m_pPhysicsDelegate.pManager = this;
		m_pPhysics.Init(0.0f);
	}

	~C_FT_FlameManager()
	{
		//m_Flames.PurgeAndDeleteElements();
		m_Flames.Purge();
	}

	void Init();
	void Step(float dt);

	void TerminateOldestFlame();
	inline void TerminateFlame(size_t index) { DeleteFlame(index); } //Alias to DeleteFlame for now
	void DeleteFlame(size_t index);
	void AddFlame(C_FT_FlameElement& pElement);

	inline CUtlVectorFixed<C_FT_FlameElement, FT_MAX_FLAMES>* GetFlameVector() { return &m_Flames; }
	inline int GetCurrentFlames() { return m_Flames.Count(); }
	inline CBasePlayer* GetCurrentPlayer() { return m_hPlayer.Get(); }
	inline CBasePlayerHandle* GetCurrentPlayerHandle() { return &m_hPlayer; }
	inline CWeaponFlamethrower* GetCurrentWeapon() { return m_hFlamethrower.Get(); }
	inline CHandle<CWeaponFlamethrower>* GetCurrentWeaponHandle() { return &m_hFlamethrower; }
	
	inline void SetWishDir(QAngle& vWishDir) 
	{
		AngleVectors(vWishDir, &m_vWishDir);
	}


	class CPhysicsDelegate : public CSimplePhysics::IHelper
	{
		DECLARE_CLASS(C_FT_FlameManager::CPhysicsDelegate, CSimplePhysics::IHelper);

	public:
		virtual void	GetNodeForces(CSimplePhysics::CNode* pNodes, int iNode, Vector* pAccel);
		virtual void	ApplyConstraints(CSimplePhysics::CNode* pNodes, int nNodes);

		C_FT_FlameManager* pManager;
	};
	friend class CPhysicsDelegate;

private:
	CUtlVectorFixed<C_FT_FlameElement, FT_MAX_FLAMES> m_Flames;

	CPhysicsDelegate m_pPhysicsDelegate;
	CSimplePhysics m_pPhysics;

	CBasePlayerHandle m_hPlayer;
	CHandle<CWeaponFlamethrower> m_hFlamethrower;

	//Velocity added to every newly created particle, it's mainly the character's angles
	Vector m_vWishDir;
};

class CWeaponFlamethrower : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponFlamethrower, CBaseHLCombatWeapon);
public:
//	DECLARE_ACTTABLE();
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CWeaponFlamethrower();

	int CapabilitiesGet() { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	WeaponClass_t WeaponClassify() { return WEPCLASS_HEAVY; }

	void ItemPostFrame();
	inline void ThrowFlame();
	void PrimaryAttack();

	void Think();

private:
	C_FT_FlameManager m_FTManager;
};