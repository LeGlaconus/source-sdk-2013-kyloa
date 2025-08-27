#include "cbase.h"
#include "weapon_flamethrower.h"

//---------------
// Flame element
//---------------

BEGIN_DATADESC_NO_BASE(C_FT_FlameElement)
DEFINE_FIELD(m_vPos, FIELD_VECTOR),
DEFINE_FIELD(m_vPrevPos, FIELD_VECTOR),
DEFINE_FIELD(m_vPredicted, FIELD_VECTOR),

DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
DEFINE_FIELD(m_flLifetime, FIELD_FLOAT),
END_DATADESC();

void C_FT_FlameElement::Delete()
{
	m_vPos = m_vPrevPos = m_vPredicted = vec3_origin;
	m_bActive = false;
	m_flLifetime = -1.0f;
}

#pragma warning(push)
#pragma warning(disable : 6305)
void* C_FT_FlameElement::operator[] (size_t idx)
{
	return this + (sizeof(*this) * idx);
}

const void* C_FT_FlameElement::operator[] (size_t idx) const
{
	return this + (sizeof(*this) * idx);
}
#pragma warning(pop)

//---------------
// Flame manager
//---------------

BEGIN_DATADESC_NO_BASE(C_FT_FlameManager)
DEFINE_UTLVECTOR(m_Flames, FIELD_EMBEDDED),
DEFINE_FIELD(m_hPlayer, FIELD_EHANDLE),
DEFINE_FIELD(m_hFlamethrower, FIELD_EHANDLE),
END_DATADESC();

void C_FT_FlameManager::Init()
{
	return;
}

void C_FT_FlameManager::Step(float dt)
{
	for (size_t i = 0; i < (uint)m_Flames.Count(); i++)
	{
		C_FT_FlameElement* pCurFlameElement = &m_Flames[i];
		if(pCurFlameElement->IsActive())
			pCurFlameElement->m_flLifetime -= dt;
		if (pCurFlameElement->m_flLifetime < 0.0f)
			DeleteFlame(i);

		//debug
		if(g_debug_ft.GetBool())
			NDebugOverlay::Cross3D(pCurFlameElement->m_vPos, 1.0f, 255, 0, 0, true, dt);
	}

	if (g_debug_ft.GetBool())
		DevMsg("dt : %f\n", dt);

	m_pPhysics.Simulate(m_Flames.Base()->GetNode(), m_Flames.Count(), &m_pPhysicsDelegate, dt, 0.0f);
}

void C_FT_FlameManager::TerminateOldestFlame()
{
	uint iLowestIndex = 0;
	float iLowestLifetime = 0.0f;

	for (size_t i = 0; i < (uint)m_Flames.Count(); i++)
	{
		if (m_Flames[i].m_flLifetime < iLowestLifetime)
			iLowestIndex = i;
	}

	TerminateFlame(iLowestIndex);
}

void C_FT_FlameManager::DeleteFlame(size_t index)
{
	m_Flames.Remove(index);
}

void C_FT_FlameManager::AddFlame(C_FT_FlameElement& pElement)
{
	if (m_Flames.Count() > FT_MAX_FLAMES)
		TerminateOldestFlame();
	else
	{
		//pElement.m_vPrevPos = pElement.m_vPos - m_vWishDir;
		m_Flames.AddToTail(pElement);
	}
}

void C_FT_FlameManager::CPhysicsDelegate::GetNodeForces(CSimplePhysics::CNode* pNodes, int iNode, Vector* pAccel)
{
	if (!pManager->GetCurrentPlayerHandle()->IsValid())
		return;

	C_FT_FlameElement* pCurFlame = &pManager->GetFlameVector()->Element(iNode);
	Vector vLookDir;
	pManager->GetCurrentPlayer()->EyeVectors(&vLookDir);
	VectorNormalize(vLookDir);

	//temp
	*pAccel = vLookDir * 25.0f;
}

void C_FT_FlameManager::CPhysicsDelegate::ApplyConstraints(CSimplePhysics::CNode* pNodes, int nNodes)
{
	return;
}

//------------
// Weapon
//------------

LINK_ENTITY_TO_CLASS(weapon_flamethrower, CWeaponFlamethrower);
PRECACHE_WEAPON_REGISTER(weapon_flamethrower);

IMPLEMENT_SERVERCLASS_ST(CWeaponFlamethrower, DT_WeaponFlamethrower)
END_SEND_TABLE();

BEGIN_DATADESC(CWeaponFlamethrower)
DEFINE_FIELD(m_FTManager, FIELD_EMBEDDED),
END_DATADESC()

CWeaponFlamethrower::CWeaponFlamethrower()
{
	m_fMinRange1 = m_fMinRange2 = 32;
	m_fMaxRange1 = m_fMaxRange2 = 192;

	m_FTManager.Init();
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (pPlayer)
		m_FTManager.GetCurrentPlayerHandle()->Set(pPlayer);
	m_FTManager.GetCurrentWeaponHandle()->Set(this);
}

void CWeaponFlamethrower::ItemPostFrame()
{
	//CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	//if (!pPlayer)
	//	return;

	BaseClass::ItemPostFrame();
}

inline void CWeaponFlamethrower::ThrowFlame()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	static Vector vShootPosition = pPlayer->Weapon_ShootPosition();

	static QAngle vShootDir = pPlayer->GetLocalAngles();
	m_FTManager.SetWishDir(vShootDir);

	C_FT_FlameElement flame(vShootPosition);
	m_FTManager.AddFlame(flame);
}

void CWeaponFlamethrower::PrimaryAttack()
{
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;

	if (m_iClip1 > 0)
	{
		ThrowFlame();
		m_iClip1;
	}
}

void CWeaponFlamethrower::Think()
{
	const float dt = TICKS_TO_TIME(m_nLastThinkTick) - gpGlobals->curtime;
	m_FTManager.Step(dt);

	SetNextThink(gpGlobals->curtime + 0.125f);
}