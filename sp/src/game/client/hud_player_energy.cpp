#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"

#include <vgui_controls/AnimationController.h>

#include "c_weapon_staplegun.h"

using namespace vgui;

class CHUDPlayerEnergy : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE(CHUDPlayerEnergy, CHudNumericDisplay);

public:
	CHUDPlayerEnergy(const char* pElementName);
	void Init();
//	void VidInit();
	void Reset();

	//Might want to remove playAnimation
	void SetCount(int count, bool playAnimation);

	virtual void Paint();

protected:
	virtual void OnThink();

	void UpdateEnergyDisplay();

private:


	int m_iCount;
	//icon(s)
};

DECLARE_HUDELEMENT(CHUDPlayerEnergy);

CHUDPlayerEnergy::CHUDPlayerEnergy(const char* pElementName)
	: BaseClass(NULL, "HudPlayerEnergy"), CHudElement(pElementName)
{
	SetHiddenBits(HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);

	//Kyloa : Not doing Logitech bullshit
}

void CHUDPlayerEnergy::Init()
{
	DevMsg("CHudStaplegunEnergy Init\n");

	m_iCount = -1;

	//Should initiliaze icon(s) + localization label text here

}

//void CHUDPlayerEnergy::VidInit()
//{
//	//Kyloa Confirm : Find out what that does and if I should remove it
//}

//Save-restore related
void CHUDPlayerEnergy::Reset()
{
	BaseClass::Reset();

	m_iCount = 0;

	UpdateEnergyDisplay();
}

void CHUDPlayerEnergy::OnThink()
{
	UpdateEnergyDisplay();
}

void CHUDPlayerEnergy::SetCount(int count, bool playAnimation)
{

	//Animations, copied from hud_ammo.cpp for now
	if (count != m_iCount)
	{
		if (count == 0)
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoEmpty");
		}
		else if (count < m_iCount)
		{
			// ammo has decreased
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoDecreased");
		}
		else
		{
			// ammunition has increased
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoIncreased");
		}

		m_iCount = count;
	}

	SetDisplayValue(count);
}

void CHUDPlayerEnergy::UpdateEnergyDisplay()
{
	C_BaseHLPlayer* player = (C_BaseHLPlayer*)C_BasePlayer::GetLocalPlayer();
	C_BaseCombatWeapon* wpn = GetActiveWeapon();

	if (!wpn || !player)
	{
		SetPaintEnabled(false);
		SetPaintBackgroundEnabled(false);

		return;
	}

	//Kyloa TODO : That's very hacky
	if (wpn->HasSpawnFlags(SF_ENERGY_WEAPON) && !FClassnameIs(wpn, "weapon_energyar"))
	{
		SetPaintEnabled(true);
		SetPaintBackgroundEnabled(true);
	}
	else
	{
		SetPaintEnabled(false);
		SetPaintBackgroundEnabled(false);
	}

	int energy = player->GetEnergy();

	SetCount(energy, true);
}

void CHUDPlayerEnergy::Paint()
{
	BaseClass::Paint();

	//Draw icon stuff
}