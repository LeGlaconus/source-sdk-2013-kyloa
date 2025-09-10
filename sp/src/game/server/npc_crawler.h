#pragma once

#include "cbase.h"
#include "ai_basenpc.h"
#include "game.h"
#include "item_energy_nugget.h"
#include "npcevent.h"

ConVar sk_crawler_health("sk_crawler_health", "0");

//Might want to create a new motor here

class CNPCCrawler : public CAI_BaseNPC
{
	DECLARE_CLASS(CNPCCrawler, CAI_BaseNPC);
	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

public:
	void Precache();
	void Spawn();

	//Sounds
	void DeathSound(const CTakeDamageInfo& info);
	void PainSound(const CTakeDamageInfo& info);
	void IdleSound();
	bool ShouldPlayIdleSound() override;
	virtual bool AllowedToIgnite() override { return true; }

	//todo, might want to make him hide from the bright flashlight when in the underground sections
	//that would also involve creating a new global
	virtual void	PlayerHasIlluminatedNPC(CBasePlayer* pPlayer, float flDot) { BaseClass::PlayerHasIlluminatedNPC(pPlayer, flDot); }

	virtual void SelectModel()
	{
		if (GetModelName() == NULL_STRING)
			SetModelName(AllocPooledString("models/breen.mdl"));
	}

	Class_T Classify();

	void HandleAnimEvent(animevent_t* pEvent); //for footstep sounds, particles (?) and possibly more

	void Event_Killed(const CTakeDamageInfo& info);

	//BodyTarget() could be useful

	//MaxYawSpeed() might need to be changed (it's also handled in the motor)

	void InputStartExtract(inputdata_t& inputData);

private:
};