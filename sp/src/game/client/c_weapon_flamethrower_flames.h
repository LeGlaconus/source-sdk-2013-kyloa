#pragma once

#include "cbase.h"
#include "particlemgr.h"

#define CLFT_FLAME_PARTICLE_STOP_RATIO (0.75f)

class C_CL_FT_Flame : public C_BaseEntity
{
	DECLARE_CLASS(C_CL_FT_Flame, C_BaseEntity);
public:
	DECLARE_CLIENTCLASS();

	C_CL_FT_Flame();
	~C_CL_FT_Flame();

	virtual void OnDataChanged(DataUpdateType_t updateType);
	virtual void ClientThink();

private:
	CNewParticleEffect* pParticleEffect;

	float m_flRemovalTime;
	float m_flLifetime;
};