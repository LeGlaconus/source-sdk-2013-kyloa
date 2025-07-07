#include "cbase.h"
#include "energyaccessor.h"

IBaseEnergyAccessor::IBaseEnergyAccessor()
{
	FindPlayer();
}

//Called at the start of each interface function because I really need that player
void IBaseEnergyAccessor::FindPlayer()
{
	if (m_bValid)
		return;

	//Could also be UTIL_GetLocalPlayer()
	m_pHL2Player = (CHL2_Player*)UTIL_PlayerByIndex(1);
	if (m_pHL2Player != nullptr)
		m_bValid = true;
}

IBaseEnergyAccessor::~IBaseEnergyAccessor()
{
	m_pHL2Player = nullptr;
}

int IBaseEnergyAccessor::GetEnergy()
{
	FindPlayer();
	return m_pHL2Player->GetEnergy();
}

void IBaseEnergyAccessor::SetEnergy(int amnt)
{
	FindPlayer();
	m_pHL2Player->SetEnergy(amnt);
}

void IBaseEnergyAccessor::AddEnergy(int amnt)
{
	FindPlayer();
	m_pHL2Player->AddEnergy(amnt);
}