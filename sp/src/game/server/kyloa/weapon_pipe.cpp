#include "cbase.h"
#include "weapon_pipe.h"

#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS(weapon_pipe, CWeaponPipe);
PRECACHE_REGISTER(weapon_pipe);

ConVar sk_plr_dmg_pipe("sk_plr_dmg_pipe", "0", FCVAR_NONE);
ConVar sk_plr_dmg_pipe_secondary("sk_plr_dmg_pipe_secondary", "0", FCVAR_NONE);

IMPLEMENT_SERVERCLASS_ST(CWeaponPipe, DT_WeaponPipe)
END_SEND_TABLE()

acttable_t CWeaponPipe::m_acttable[] =
{
	{ ACT_MELEE_ATTACK1,	ACT_MELEE_ATTACK_SWING,	true },
	{ ACT_IDLE,				ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_IDLE_ANGRY,		ACT_IDLE_ANGRY_MELEE,	false },
#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_RUN,				ACT_RUN_MELEE,			false },
	{ ACT_WALK,				ACT_WALK_MELEE,			false },

	{ ACT_ARM,				ACT_ARM_MELEE,			false },
	{ ACT_DISARM,			ACT_DISARM_MELEE,		false },
#endif

#ifdef MAPBASE
	// HL2:DM activities (for third-person animations in SP)
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SLAM,					true },
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_MELEE,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_MELEE,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_MELEE,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_MELEE,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_MELEE,					false },
#if EXPANDED_HL2DM_ACTIVITIES
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK2,	ACT_HL2MP_GESTURE_RANGE_ATTACK2_MELEE,	false },
	{ ACT_HL2MP_WALK,					ACT_HL2MP_WALK_MELEE,					false },
#endif
#endif
};

IMPLEMENT_ACTTABLE(CWeaponPipe);

float CWeaponPipe::GetDamageForActivity(Activity hitActivity)
{
	//I used to do that by checking HITCENTER2 but it's just better this way
	return m_bBoostedHit ? sk_plr_dmg_pipe_secondary.GetFloat() : sk_plr_dmg_pipe.GetFloat();
}

void CWeaponPipe::AddViewKick()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	QAngle punchAng;

	if (m_bBoostedHit)
	{
		punchAng.x = random->RandomFloat(1.0f, 2.0f);
		punchAng.y = random->RandomFloat(-2.0f, -1.0f);
	}
	else
	{
		punchAng.x = random->RandomFloat(1.5f, 2.5f);
		punchAng.y = random->RandomFloat(-2.5f, -1.5f);
	}
	punchAng.z = 0.0f;

	pPlayer->ViewPunch(punchAng);
}

void CWeaponPipe::SecondaryAttack()
{
	m_bBoostedHit = true;

	BaseClass::SecondaryAttack();

	//Swing sets that to the sequence duration...
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRate();

	m_bBoostedHit = false;
}