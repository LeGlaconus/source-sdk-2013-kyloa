#include "cbase.h"
//#include "c_weapon__stubs.h"
//#include "c_basehlcombatweapon.h"
//
//#include "c_weapon_staplegun.h"
//
//
//STUB_WEAPON_CLASS_IMPLEMENT(weapon_staplegun, C_WeaponStapleGun);
//
//IMPLEMENT_CLIENTCLASS_DT(C_WeaponStapleGun, DT_WeaponStapleGun, CWeaponStapleGun)
//
//	RecvPropInt(RECVINFO(m_iEnergy)),
//
//END_RECV_TABLE();
//
//C_WeaponStapleGun::C_WeaponStapleGun()
//{
//	//Kyloa Confirm : why would I need to set it to smth ? To not send dummy data to the vgui panel ?
//	m_iEnergy = -1;
//}
//
//void C_WeaponStapleGun::OnDataChanged(DataUpdateType_t updateType)
//{
//	BaseClass::OnDataChanged(updateType);
//	//Kyloa Confirm : this is ripped off from the grav gun code, confirm if this is what must be done
//	SetNextClientThink(CLIENT_THINK_ALWAYS);
//}
//
//void C_WeaponStapleGun::ClientThink()
//{
//	BaseClass::ClientThink();
//}
//
//
//int C_WeaponStapleGun::GetEnergy()
//{
//	return m_iEnergy;
//}