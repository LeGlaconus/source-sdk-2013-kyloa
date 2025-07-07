#pragma once

#include "cbase.h"
#include "ai_basenpc.h"
#include "game.h"
#include "item_energy_nugget.h"

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

	//todo, might want to make him hide from the bright flashlight when in the underground sections
	//that would also involve creating a new global
	virtual void	PlayerHasIlluminatedNPC(CBasePlayer* pPlayer, float flDot) { BaseClass::PlayerHasIlluminatedNPC(pPlayer, flDot); }

	//Kyloa TODO : Use SelectModel() instead
	//so that it doesn't crash when npc_create-ing it
	//this is hella bad
	inline string_t GetModelName() const
	{
		if (BaseClass::GetModelName() == NULL_STRING)
			//return AllocPooledString("models/crawler/crawler.mdl");
			return AllocPooledString("models/breen.mdl");
		else
			return BaseClass::GetModelName();
	}

	Class_T Classify();

	void HandleAnimEvent(animevent_t* pEvent) { BaseClass::HandleAnimEvent(pEvent); } //for footstep sounds, particles (?) and possibly more

	void Event_Killed(const CTakeDamageInfo& info);

	//BodyTarget() could be useful

	//MaxYawSpeed() might need to be changed (it's also handled in the motor)

	void InputStartExtract(inputdata_t& inputData);

private:
};