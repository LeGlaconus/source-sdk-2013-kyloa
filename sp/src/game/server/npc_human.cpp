#include "cbase.h"

#include "npc_human.h"

void CNPCHuman::Spawn()
{
	BaseClass::Spawn();

	CapabilitiesRemove(bits_CAP_FRIENDLY_DMG_IMMUNE);
}

LINK_ENTITY_TO_CLASS(npc_human, CNPCHuman);