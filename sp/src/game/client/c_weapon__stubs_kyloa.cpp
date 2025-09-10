#include "cbase.h"
#include "c_weapon__stubs.h"
#include "basehlcombatweapon_shared.h"
#include "c_basehlcombatweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//Kyloa

//You're apparently not supposed to have one if you're making your own clientside version of it
// I don't need to transfer the energy from the weapon directly anymore so I'm uncommenting that
STUB_WEAPON_CLASS(weapon_staplegun, WeaponStapleGun, C_BaseHLCombatWeapon);
STUB_WEAPON_CLASS(weapon_pipe, WeaponPipe, C_BaseHLBludgeonWeapon);
STUB_WEAPON_CLASS(weapon_mp7, WeaponMP7, C_HLSelectFireMachineGun);
STUB_WEAPON_CLASS(weapon_spas12, WeaponSpas12, C_BaseHLCombatWeapon);
STUB_WEAPON_CLASS(weapon_energyar, WeaponEnergyAr, C_HLMachineGun);
STUB_WEAPON_CLASS(weapon_uspmatch, WeaponUSPMatch, C_BaseHLCombatWeapon);
STUB_WEAPON_CLASS(weapon_glock, WeaponGlock, C_BaseHLCombatWeapon);
STUB_WEAPON_CLASS(weapon_22, Weapon22, C_BaseHLCombatWeapon);
//STUB_WEAPON_CLASS(weapon_flamethrower, WeaponFlamethrower, C_BaseHLCombatWeapon);