#pragma once

#include "cbase.h"
#include "hl2_player.h"

class IBaseEnergyAccessor : public IBaseInterface
{
	DECLARE_CLASS(IBaseEnergyAccessor, IBaseInterface);
public:
	IBaseEnergyAccessor();
	~IBaseEnergyAccessor();

	void FindPlayer();
	int GetEnergy();
	void SetEnergy(int amnt);
	void AddEnergy(int amnt);
private:
	CHL2_Player* m_pHL2Player;
	bool m_bValid = false;
};