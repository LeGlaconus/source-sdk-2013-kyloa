#pragma once

#include "cbase.h"
#include "items.h"
#include "Sprite.h"
#include "energyaccessor.h"


#define ITEM_ENERGY_NUGGET_SPRITE "sprites/energy_nugget01.vmt"


class CItemEnergyNugget : public CItem, public IBaseEnergyAccessor
{
	DECLARE_CLASS(CItemEnergyNugget, CItem);
	//DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
public:

	virtual void Precache();
	virtual void Spawn();

	//Needed for if I want to make that merging feature
	virtual void Think();

	//virtual void VPhysicsCollision(int index, gamevcollisionevent_t* pEvent);


private:

	CSprite* pSrite = nullptr;

	//Might want to put a merged count if I make that merging feature
	//int	m_iMergeCount;
};