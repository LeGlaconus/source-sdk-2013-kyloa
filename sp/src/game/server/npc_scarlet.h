#pragma once

#include "cbase.h"
#include "ai_baseactor.h"
#include "ai_basehumanoid.h"
#include "npc_playercompanion.h"

//Kyloa TODO : change that
//If I want the vehicle animations, I have to use the ep2 model
#define SCARLET_MODEL	"models/alyx.mdl"

ConVar sk_scarlet_health("sk_scarlet_health", "0", FCVAR_NONE);

class CNPCScarlet : CNPC_PlayerCompanion
{
	DECLARE_CLASS(CNPCScarlet, CNPC_PlayerCompanion);
	DECLARE_DATADESC();
public:

	void Spawn();
	void Activate();
	void Precache();
	void SelectModel();

	void GatherConditions();

	//Kyloa TODO : find what ShouldPlayerAvoid() can be used for, I don't get what Alyx's implementation does

	//Overriding IsValidEnemy can be useful if an enemy scares scarlet for no reason

	//Overriding SpeakIfAllowed can be useful, Alyx uses it for dropping the sound of her breathing when in darkness mode but you can use it for something else

	//Overriding IsAllowedToSpeak can be useful to prevent her from speaking in sections where she's supposed to be quiet or something like that

	//I can use Event_KilledOther to play a custom response rule when killing an enemy, even if the default one is given more parameters
	//Alyx also uses it to make her continue shooting it the corpse after its death, I can copy that behavior but only for certain classes
	//and only if there's no enemies around
	//Kyloa Confirm : find out why Mapbase makes Alyx wants to play her custom response instead of the vanilla one

	//Scarlet should already be able to get into vehicles, because every player companion can

	//A custom function can be made to check if she's inside of a vehicle, it's not default behavior for some reason

	//Alyx calls directly functions from the passenger and func_tank behavior in AimGun() for some reason

	//Player companions have a feature in GetActualShootPosition() where they have a 1/3 chance of doing a headshot with a zombie, Alyx adds a 1/2 to also have a zombie headshot, but it's added on top
	//I could do the same headshot chance thing with crawlers

	//ai_lead_time seems to be a sort of way for shots to be missed, figure out if I need to change that

	//Alyx turns slowly when crouched for some reason

	//OnUpdateShotRegulator() is used by Alyx to make her fire longer bursts when crouched, I can also modify the shot regulator this way, or I could just override Scarlet's one

	//Might want to change IsCrouchedActivity(), Kyloa TODO : figure out why Alyx uses it, does she just have new activities ?

	//Alyx uses a function called SpeakAttacking() to play TLK_ATTACKING in OnBeginMoveAndShoot()
	//TLK_ATTACKING is also called by player companions in TASK_ANNOUNCE_ATTACK, but it's used outside of here by other npcs for variable reasons

	//Alyx and vortigaunts don't call the base ally's CanFlinch function because Mapbase disables flinching if a cvar is checked, because of new citizen flinching animations
	virtual bool CanFlinch() { CAI_BaseActor::CanFlinch(); }

	//I might want to override GetGameTextSpeechParams() for prototyping, it's here for the new mapbase debug text event

	//Alyx overrides the jump gravity for some reason, I don't know if I want that



	//Barney uses a special Use function to play lines when interacted with, I should probably fo the same but with more response rules
	//Alyx has the same thing but it doesn't work when inside of a vehicle
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	Class_T Classify() { return CLASS_PLAYER_ALLY_VITAL; }
};