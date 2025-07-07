#include "cbase.h"

#include "npc_scarlet.h"

LINK_ENTITY_TO_CLASS(npc_scarlet, CNPCScarlet);

BEGIN_DATADESC(CNPCScarlet)

END_DATADESC();

void CNPCScarlet::Spawn()
{
	BaseClass::Spawn();

	SetHealth(sk_scarlet_health.GetInt());
	//Note : Alyx doesn't bleed
	
	//Physcannon interactions aren't gonna happen but you never know what Ash is up to
	AddEFlags(EFL_NO_DISSOLVE | EFL_NO_PHYSCANNON_INTERACTION | EFL_NO_MEGAPHYSCANNON_RAGDOLL);

	//Note : Alyx's code makes it so that she doesn't pickup weapons, this should probably be implemented here too

}

void CNPCScarlet::SelectModel()
{
	if(GetModelName() == NULL_STRING)
		SetModelName(AllocPooledString(SCARLET_MODEL));
}

void CNPCScarlet::Precache()
{
	BaseClass::Precache();

	//Kyloa TODO : could be scenes, so that we have flex animation for pain as well
	PrecacheScriptSound("NPC_Scarlet.Pain");
	PrecacheScriptSound("NPC_Scarlet.Death");

}

void CNPCScarlet::Activate()
{
	BaseClass::Activate();

	//Similar to Alyx's behavior : never let her die when reloading saves
	if (IsPlayerAlly() && GetHealth() < sk_scarlet_health.GetInt())
		SetHealth(sk_scarlet_health.GetInt());

	//Kyloa Confirm TODO : You apparently have to make her always assume that she said hello to the player

	//Kyloa Confirm : Alyx has code for knowing what kind of weapon she's currently holding, find out why she's doing that

}

void CNPCScarlet::GatherConditions()
{
	BaseClass::GatherConditions();
	
	//Alyx has code to play flinching animations when she's close to an explosion
}

//This apparently requires to use SetUse even if it's overloading the function
void CNPCScarlet::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	BaseClass::Use();

	//Kyloa TODO : copy the Alyx Use() code and maybe modify it
}