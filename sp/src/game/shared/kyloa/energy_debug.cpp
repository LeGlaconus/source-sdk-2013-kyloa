#include "cbase.h"
#include "icvar.h"
#include "hl2_player.h"

static void RefillEnergyStaplegun_f(const CCommand& args)
{
	CHL2_Player* player = (CHL2_Player*)UTIL_GetLocalPlayer();
	if (player != nullptr)
	{
		player->SetMaxEnergy(ENERGY_MAX_STAPLEGUN);
		player->SetEnergy(ENERGY_MAX_STAPLEGUN);
	}
	//reset energy counter ?
}

static void RefillEnergyEnergyAr_f(const CCommand& args)
{
	CHL2_Player* player = (CHL2_Player*)UTIL_GetLocalPlayer();
	if (player != nullptr)
	{
		player->SetMaxEnergy(ENERGY_MAX_ENERGYAR);
		player->SetEnergy(ENERGY_MAX_ENERGYAR);
	}
	//reset energy counter ?
}

ConCommand energy_refillstaplegun("energy_refillstaplegun", RefillEnergyStaplegun_f, "Refills the energy to 6, temporary", FCVAR_CHEAT);
ConCommand energy_refillenergyar("energy_refillenergyar", RefillEnergyEnergyAr_f, "Refills the energy to 16, temporary", FCVAR_CHEAT);

CON_COMMAND_F(colonthree, ":3", FCVAR_CHEAT)
{
	unsigned int num = 10;
	if (args.ArgC() >= 1)
		num = Q_atoi(args.Arg(1));
	for (int i = 0; i <= 10; ++i)
	{
		DevMsg(":3\n");
	}
}