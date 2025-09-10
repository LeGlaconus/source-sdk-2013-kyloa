#include "cbase.h"
#include "c_weapon_flamethrower.h"

STUB_WEAPON_CLASS_IMPLEMENT(weapon_flamethrower, C_WeaponFlamethrower);

IMPLEMENT_CLIENTCLASS_DT(C_WeaponFlamethrower, DT_WeaponFlamethrower, CWeaponFlamethrower)
	RecvPropInt(RECVINFO(m_eState)),
END_RECV_TABLE();

C_WeaponFlamethrower::C_WeaponFlamethrower()
	:m_bEmitting(false), BaseClass()
{ }

C_WeaponFlamethrower::~C_WeaponFlamethrower()
{
	m_pFlameJet->StopEmission();
	StopParticleEffects(this);
}

void C_WeaponFlamethrower::OnDataChanged(DataUpdateType_t updateType)
{
	if (updateType == DATA_UPDATE_CREATED)
	{
		m_pFlameJet = ParticleProp()->Create("ft_flamejet", PATTACH_POINT_FOLLOW, "flameout");
		if (m_pFlameJet)
			SetNextClientThink(CLIENT_THINK_ALWAYS);	
		else
		{
			DevMsg("No flame jet particle effect for flamethrower !\n");
			SetNextClientThink(CLIENT_THINK_NEVER);
		}
	}

	BaseClass::OnDataChanged(updateType);
}

void C_WeaponFlamethrower::ClientThink()
{
	switch (m_eState)
	{
	case FT_State::Idle:
		StopEmitting();
		break;
	case FT_State::Primary:
	default:
		if (!m_bEmitting)
			StartEmitting();
		break;
	}
}

void C_WeaponFlamethrower::StartEmitting()
{
	m_bEmitting = true;

	m_pFlameJet->SetDormant(false);
	m_pFlameJet->StartEmission();
}

void C_WeaponFlamethrower::StopEmitting()
{
	m_bEmitting = false;

	m_pFlameJet->SetDormant(true);
}