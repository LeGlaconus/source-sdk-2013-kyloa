#include "cbase.h"
#include "items.h"

#include "weapon_staplegun.h"

#include "item_energy_nugget.h"

BEGIN_DATADESC(CItemEnergyNugget)
END_DATADESC();

LINK_ENTITY_TO_CLASS(item_energy_nugget, CItemEnergyNugget);

void CItemEnergyNugget::Precache()
{
	//Will soon be a sprite
	//PrecacheMaterial(ITEM_ENERGY_NUGGET_SPRITE);
	//Precache sounds
	PrecacheScriptSound("ItemEnergyNugget.Touch");

}

void CItemEnergyNugget::Spawn()
{
	BaseClass::Precache();

	Precache();

	pSrite = CSprite::SpriteCreate(ITEM_ENERGY_NUGGET_SPRITE, GetAbsOrigin(), false);
	if (pSrite)
	{
		pSrite->SetRenderMode(kRenderTransAdd); //might want to do that in the vmt
		pSrite->SetScale(10.0f);
		pSrite->SetParent(this); //might want attachments in the future
		pSrite->Spawn();
	}

	//Might need to change
	SetMoveType(MOVETYPE_FLY);
	
	//It's going to be a sprite so it doesn't matter this much for the moment
	AddEffects(EF_NOSHADOW | EF_NORECEIVESHADOW);

	//Is that actually needed ?
	SetHealth(1);

	SetNextThink(gpGlobals->curtime + 0.05f);
}

void CItemEnergyNugget::Think()
{
	trace_t tr;
	CBasePlayer* player = UTIL_GetLocalPlayer();
	if (player)
	{
		UTIL_TraceLine(GetAbsOrigin(), player->GetAbsOrigin(), MASK_SOLID, this, COLLISION_GROUP_NONE, &tr);
		
		Vector tmp = player->GetAbsOrigin() - GetAbsOrigin();

		if (tr.m_pEnt->IsPlayer())
		{
			SetAbsVelocity(tmp);
		}

		//Instead of doing it in Touch, I'm just checking the distance
		if (pSrite)
		{
			if (g_debug_energynugget.GetBool())
				DevMsg("Distance from player to energynugget : %f \n", tmp.Length());

			if (tmp.IsLengthLessThan(32.0f))
			{
				AddEnergy(sk_energy_nugget_health.GetInt());
				EmitSound("ItemEnergyNugget.Touch");

				SetThink(&CItemEnergyNugget::SUB_Remove);
			}
		}
	}

	if (pSrite)
	{
		pSrite->SetScale(1 + (0.5 * sin(gpGlobals->curtime)), 0.05f);
	
		if (g_debug_energynugget.GetBool())
			DevMsg("Scale : %f \n", 1 + (0.5*sin(gpGlobals->curtime)) );
	}

	SetNextThink(gpGlobals->curtime + 0.05f);
}