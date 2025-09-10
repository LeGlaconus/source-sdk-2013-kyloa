#include "cbase.h"
#include "c_weapon_flamethrower_flames.h"

#include "tier0/memdbgon.h"

class C_FT_Flame;

IMPLEMENT_CLIENTCLASS_DT(C_CL_FT_Flame, DT_FT_Flame, C_FT_Flame)
	RecvPropTime(RECVINFO(m_flRemovalTime)),
END_RECV_TABLE();

C_CL_FT_Flame::C_CL_FT_Flame()
	:m_flLifetime(0.0f), pParticleEffect(nullptr)
{ }

C_CL_FT_Flame::~C_CL_FT_Flame()
{
	if (pParticleEffect)
		StopParticleEffects(this);
}

void C_CL_FT_Flame::OnDataChanged(DataUpdateType_t updateType)
{
	BaseClass::OnDataChanged(updateType);

	if (updateType == DATA_UPDATE_CREATED)
	{
		m_flLifetime = m_flRemovalTime - gpGlobals->curtime;

		pParticleEffect = ParticleProp()->Create("ft_flame", PATTACH_ABSORIGIN_FOLLOW);
		if (!pParticleEffect)
			DevMsg("Failed to create flame particle system for flame ent !\n");
		else
			pParticleEffect->StartEmission(); //Kyloa TODO : figure out the params

		SetNextClientThink(CLIENT_THINK_ALWAYS);

		return;
	}
}

void C_CL_FT_Flame::ClientThink()
{
	if (!pParticleEffect)
		return;
	if (pParticleEffect->m_bEmissionStopped)
			return;

	const float flCurLifetimeRatio = m_flLifetime / (m_flRemovalTime - gpGlobals->curtime);
	
	if (flCurLifetimeRatio < CLFT_FLAME_PARTICLE_STOP_RATIO)
		pParticleEffect->StopEmission(); //Kyloa TODO : figure out what the params mean
}