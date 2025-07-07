#include "cbase.h"

#include "npc_crawler.h"

LINK_ENTITY_TO_CLASS(npc_crawler, CNPCCrawler);

BEGIN_DATADESC(CNPCCrawler)

DEFINE_INPUTFUNC(FIELD_VOID, "StartExtract", InputStartExtract),

END_DATADESC();

AI_BEGIN_CUSTOM_NPC(npc_crawler, CNPCCrawler)
AI_END_CUSTOM_NPC()

void CNPCCrawler::Precache()
{
	BaseClass::Precache();

	PrecacheScriptSound("NPC_Crawler.Pain");
	PrecacheScriptSound("NPC_Crawler.Wake");
	PrecacheScriptSound("NPC_Crawler.Death");
	PrecacheScriptSound("NPC_Crawler.DeathFinality");

	//Might want to use SelectModel() instead of my crappy workaround

	//Should be "models/crawler/crawler.mdl" but you never know
	PrecacheModel(STRING(GetModelName()));
}

void CNPCCrawler::Spawn()
{
	Precache();

	SetModel(STRING(GetModelName()));
	//Same than for npc_zombie_torso, might need additional testing (<forget, temp HULL_HUMAN)
	SetHullType(HULL_HUMAN);
	//Might need to use the SetHullSizeSmall() function
	SetHullSizeNormal();

	SetSolid(SOLID_BBOX);
	AddSolidFlags(FSOLID_NOT_STANDABLE);
	//Might need to change due to their ability to walk on ceilings and walls
	SetMoveType(MOVETYPE_STEP);

	// Kyloa TODO: change the particles
	SetBloodColor(BLOOD_COLOR_CRAWLER);

	m_iHealth = sk_crawler_health.GetFloat();
	//Might need to change, set to 0.2 for zombies
	m_flFieldOfView = 0.5;
	//Kyloa Confirm : why not IDLE ? Is it handled automatically ? why even bother setting it then ?
	m_NPCState = NPC_STATE_NONE;

	//Kyloa Confirm : why are we clearing it ? it shouldn't initialize it, so is it set to something else before ?
	CapabilitiesClear();
	//Kyloa Confirm : is bits_CAP_MOVE_CRAWL really unimplemented ?
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_TURN_HEAD);

	NPCInit();
}

Class_T CNPCCrawler::Classify()
{
	//Kyloa Temporary, but after looking at the huge tables in hl2_gamerules.cpp, I'm not doing the relationships yet
	return CLASS_ZOMBIE;
}


void CNPCCrawler::DeathSound(const CTakeDamageInfo& info)
{
	//This seems to not only apply to sentences, but every sound coming out of CHAN_VOICE
	SentenceStop();

	EmitSound("NPC_Crawler.Death");
}

void CNPCCrawler::PainSound(const CTakeDamageInfo& info)
{
	//Kyloa TODO : I should probably play different sounds depending on the type / value
	EmitSound("NPC_Crawler.Pain");
}

bool CNPCCrawler::ShouldPlayIdleSound()
{
	//Kyloa : do not play the sounds when alerted and pick a higher frequency than 1/100 (1/200)
	//might want to make AlertSound() and play less idle sounds when spying
	if ((m_NPCState == NPC_STATE_IDLE && random->RandomInt(0, 199) == 0 && !HasSpawnFlags(SF_NPC_GAG)))
	{
		return true;
	}

	return false;
}

//Kyloa Confirm : Idle sounds can also be played when in alert state, I don't know if I want that
//I have to override ShouldPlayIdleSound() if that needs to be changed
//1 in 100 chance of being played every time it thinks, I probably want to change that as well
void CNPCCrawler::IdleSound()
{
	//Kyloa TODO : I've got nothing to play for now
	//Kyloa TODO : I should check if the crawler is spying the player, then play different sounds, or recode the entire idle sound system
	EmitSound("NPC_Crawler.Idle");
}

void CNPCCrawler::Event_Killed(const CTakeDamageInfo& info)
{
	//Energy nuggets

	Vector mins = GetHullMins();
	Vector maxs = GetHullMaxs();

	for (int i = 0; i <= RandomInt(1, 2); i++)
	{
		Vector tmp()
		

		CItemEnergyNugget* pNugget = (CItemEnergyNugget*)Create("item_energy_nugget", GetAbsOrigin(), QAngle(0, 0, 0), this);
	}
	

	//Agony sequence ? Depending on the damage type / attacker / surroundings ?

	BaseClass::Event_Killed(info);
}

//Extraction sequence, should turn the monster into a ragdoll and play some sort of particle
void CNPCCrawler::InputStartExtract(inputdata_t &inputData)
{
	//DevMsg("Crawler NPC : %s received input StartExtract from activator : %s caller : %s \n", GetDebugName(), inputData.pActivator->GetDebugName(), inputData.pCaller->GetDebugName());
	
	//Make fancier death animation
	SetHealth(0);
	//Play sound
	EmitSound("NPC_Crawler.DeathFinality");
	//Play particle
}