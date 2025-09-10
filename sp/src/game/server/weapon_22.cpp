#include "cbase.h"

#include "weapon_22.h"

#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS(weapon_22, CWeapon22);

PRECACHE_WEAPON_REGISTER(weapon_22);

IMPLEMENT_SERVERCLASS_ST(CWeapon22, DT_Weapon22)
END_SEND_TABLE();

BEGIN_DATADESC(CWeapon22)
DEFINE_FIELD(m_bHammerDown, FIELD_BOOLEAN),
DEFINE_FIELD(m_flAccuracyPenalty, FIELD_FLOAT),
DEFINE_FIELD(m_flSoonestPrimaryAttack, FIELD_TIME),

//DEFINE_FIELD(m_iHammerAmmoType, FIELD_INTEGER), //No need to save it
END_DATADESC()

acttable_t	CWeapon22::m_acttable[] =
{
#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_IDLE,						ACT_IDLE_REVOLVER,					true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_REVOLVER,				true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_REVOLVER,			true },
	{ ACT_RELOAD,					ACT_RELOAD_REVOLVER,					true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_REVOLVER,				true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_REVOLVER,				true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_REVOLVER,	true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_REVOLVER_LOW,				false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_REVOLVER_LOW,		false },
	{ ACT_COVER_LOW,				ACT_COVER_REVOLVER_LOW,				false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_REVOLVER_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_REVOLVER,			false },
	{ ACT_WALK,						ACT_WALK_REVOLVER,					true },
	{ ACT_RUN,						ACT_RUN_REVOLVER,					true },
#else
	{ ACT_IDLE,						ACT_IDLE_PISTOL,				true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_PISTOL,			true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD,					ACT_RELOAD_PISTOL,				true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_PISTOL,			true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_PISTOL,				true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_PISTOL,true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_PISTOL_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_PISTOL_LOW,	false },
	{ ACT_COVER_LOW,				ACT_COVER_PISTOL_LOW,			false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_PISTOL_LOW,		false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_WALK,						ACT_WALK_PISTOL,				false },
	{ ACT_RUN,						ACT_RUN_PISTOL,					false },
#endif

	// 
	// Activities ported from weapon_alyxgun below
	// 

	// Readiness activities (not aiming)
#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL_RELAXED,		false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_PISTOL_STIMULATED,		false },
#else
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL,				false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_STIMULATED,			false },
#endif
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_STEALTH,				ACT_IDLE_STEALTH_PISTOL,		false },

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_WALK_RELAXED,				ACT_WALK_PISTOL_RELAXED,		false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_PISTOL_STIMULATED,		false },
#else
	{ ACT_WALK_RELAXED,				ACT_WALK,						false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_STIMULATED,			false },
#endif
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_STEALTH,				ACT_WALK_STEALTH_PISTOL,		false },

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_RUN_RELAXED,				ACT_RUN_PISTOL_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_PISTOL_STIMULATED,		false },
#else
	{ ACT_RUN_RELAXED,				ACT_RUN,						false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_STIMULATED,				false },
#endif
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_STEALTH,				ACT_RUN_STEALTH_PISTOL,			false },

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_PISTOL,				false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_ANGRY_PISTOL,			false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_AIM_STEALTH,			ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK,						false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_PISTOL,			false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_AIM_STEALTH,			ACT_WALK_AIM_STEALTH_PISTOL,	false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN,						false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_PISTOL,				false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_AIM_STEALTH,			ACT_RUN_AIM_STEALTH_PISTOL,		false },//always aims
	//End readiness activities

	// Crouch activities
	{ ACT_CROUCHIDLE_STIMULATED,	ACT_CROUCHIDLE_STIMULATED,		false },
	{ ACT_CROUCHIDLE_AIM_STIMULATED,ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims
	{ ACT_CROUCHIDLE_AGITATED,		ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims

	// Readiness translations
	{ ACT_READINESS_RELAXED_TO_STIMULATED, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED, false },
	{ ACT_READINESS_RELAXED_TO_STIMULATED_WALK, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK, false },
	{ ACT_READINESS_AGITATED_TO_STIMULATED, ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED, false },
	{ ACT_READINESS_STIMULATED_TO_RELAXED, ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED, false },

#if EXPANDED_HL2_COVER_ACTIVITIES
	{ ACT_RANGE_AIM_MED,			ACT_RANGE_AIM_REVOLVER_MED,			false },
	{ ACT_RANGE_ATTACK1_MED,		ACT_RANGE_ATTACK_REVOLVER_MED,		false },
#endif

#ifdef MAPBASE
	// HL2:DM activities (for third-person animations in SP)
#if EXPANDED_HL2DM_ACTIVITIES
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_REVOLVER,                    false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_REVOLVER,                    false },
	{ ACT_HL2MP_WALK,					ACT_HL2MP_WALK_REVOLVER,                    false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_REVOLVER,            false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_REVOLVER,            false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_REVOLVER,    false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK2,	ACT_HL2MP_GESTURE_RANGE_ATTACK2_REVOLVER,    false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_REVOLVER,        false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_REVOLVER,                    false },
#else
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_PISTOL,                    false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_PISTOL,                    false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,    false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_PISTOL,        false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_PISTOL,                    false },
#endif
#endif
};


IMPLEMENT_ACTTABLE(CWeapon22);

// Allows Weapon_BackupActivity() to access the 22's activity table.
acttable_t* Get22Acttable()
{
	return CWeapon22::m_acttable;
}

int Get22ActtableCount()
{
	return ARRAYSIZE(CWeapon22::m_acttable);
}

CWeapon22::CWeapon22()
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;

	m_fMinRange1 = 24;
	m_fMaxRange1 = 1000;
	m_fMinRange2 = 24;
	m_fMaxRange2 = 200;

	m_flSoonestPrimaryAttack = gpGlobals->curtime;
	m_iHammerAmmoType = GetAmmoDef()->Index("22_HammerDown");
}

bool CWeapon22::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	//Don't cheat the system by constantly pressing the switch weapon key
	if(m_flAccuracyPenalty > 0.35f)
		m_flAccuracyPenalty = 0.35f;

	return BaseClass::Holster(pSwitchingTo);
}

void CWeapon22::ItemBusyFrame()
{
	if (m_flAccuracyPenalty >= WEAPON_22_ACCURACY_PENALTY_MIN)
		m_flAccuracyPenalty -= gpGlobals->absoluteframetime / 4.0f;

	if (g_debug_22.GetInt() > 1)
		DevMsg("Accuracy penalty : %f \n", m_flAccuracyPenalty);

	BaseClass::ItemBusyFrame();
}

void CWeapon22::ItemPostFrame()
{
	if (m_flAccuracyPenalty >= WEAPON_22_ACCURACY_PENALTY_MIN)
		m_flAccuracyPenalty -= gpGlobals->absoluteframetime / 4.0f;

	if(g_debug_22.GetInt() > 1)
		DevMsg("Accuracy penalty : %f \n", m_flAccuracyPenalty);

	BaseClass::ItemPostFrame();

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	if ((pPlayer->m_nButtons & IN_ATTACK) == false && m_flSoonestPrimaryAttack <= gpGlobals->curtime)
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 0.1f;
		m_bQuickShot = true;
	}

	CheckToggleHammer();
}

bool CWeapon22::Reload()
{
	const float temp = (GetMaxClip1() - m_iClip1) / 10.0f;

	if ((m_flAccuracyPenalty - temp) >= WEAPON_22_ACCURACY_PENALTY_MIN)
		m_flAccuracyPenalty -= temp;

	return BaseClass::Reload();
}

void CWeapon22::AddViewKick()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	//Disorient the player
	QAngle angles = pPlayer->GetLocalAngles();

	if (m_bHammerDown)
	{
		angles.x += random->RandomInt(-0.175f, 0.175f) * (m_flAccuracyPenalty / 14.0f) * 0.75f;
		angles.y += random->RandomInt(-0.175f, 0.175f) * (m_flAccuracyPenalty / 12.0f) * 0.75f;
	}
	else
	{
		angles.x += random->RandomInt(-0.225f, 0.225f) * (m_flAccuracyPenalty / 14.0f) * 0.75f;
		angles.y += random->RandomInt(-0.225f, 0.225f) * (m_flAccuracyPenalty / 12.0f) * 0.75f;
	}

	angles.z = 0;

	pPlayer->SnapEyeAngles(angles);

	//Like in the USP because the view drifts too much
	pPlayer->ViewPunchReset();

	pPlayer->ViewPunch(QAngle(-8, random->RandomFloat(-2, 2), 0));
}

void CWeapon22::PrimaryAttack()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
			Reload();
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = 0.15;
		}
		return;
	}

	if (m_flAccuracyPenalty < 0)
		m_flAccuracyPenalty = 0;
	if (m_flAccuracyPenalty <= WEAPON_22_ACCURACY_PENALTY_MAX)
		m_flAccuracyPenalty += 0.15f;

	if (m_flAccuracyPenalty <= WEAPON_22_PERFECT_ACCURACY_TRESHOLD)
		m_bPerfectShot = true;

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	if (!m_bHammerDown)
		WeaponSound(SINGLE);
	else
		WeaponSound(SPECIAL1);

	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flSoonestPrimaryAttack = gpGlobals->curtime + sk_22_firerate_real.GetFloat();
	if (m_bHammerDown)
		m_flSoonestPrimaryAttack += m_bQuickShot ? (sk_22_firerate_hammer_penalty.GetFloat() * 0.125f) : sk_22_firerate_hammer_penalty.GetFloat();
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	const int ammo = m_bHammerDown ? m_iHammerAmmoType : m_iPrimaryAmmoType;
	const float multiplier = m_bHammerDown ? m_flAccuracyPenalty * 0.5f : m_flAccuracyPenalty;
	pPlayer->FireBullets(1, vecSrc, vecAiming, GetBulletSpread() * multiplier, MAX_TRACE_LENGTH, ammo, 1);
	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);

	AddViewKick();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner());

	//You need to pull it again
	if ((g_debug_22.GetInt() >= 1) && m_bHammerDown)
		DevMsg("Hammer reset ! \n");
	m_bHammerDown = false;
	
	m_bQuickShot = false;
	m_bPerfectShot = false;
}

void CWeapon22::CheckToggleHammer()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	if (pPlayer->m_afButtonPressed & IN_ATTACK2 && (m_flHammerDelay <= gpGlobals->curtime))
		ToggleHammer();
}

void CWeapon22::ToggleHammer()
{
	m_flHammerDelay = gpGlobals->curtime + WEAPON_22_HAMMER_DELAY;

	WeaponSound(SPECIAL2);
	m_bHammerDown = !m_bHammerDown;

	m_flNextPrimaryAttack += 0.025f;

	if (g_debug_22.GetInt() >= 1)
		DevMsg("Hammer toggled ! current state : %i \n", m_bHammerDown);
}

void CWeapon22::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	WeaponSound(SINGLE_NPC);
	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1);
	pOperator->DoMuzzleFlash();
	m_iClip1--;
}

void CWeapon22::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	AngleVectors(angShootDir, &vecShootDir);
	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
}

void CWeapon22::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	switch (pEvent->event)
	{
	case EVENT_WEAPON_RELOAD:
	{
		CEffectData data;

		// Emit six spent shells
		for (int i = 0; i < 6; i++)
		{
			data.m_vOrigin = pOwner->WorldSpaceCenter() + RandomVector(-4, 4);
			data.m_vAngles = QAngle(90, random->RandomInt(0, 360), 0);
			data.m_nEntIndex = entindex();

			DispatchEffect("ShellEject", data);
		}

		break;
	}
	case EVENT_WEAPON_PISTOL_FIRE:
	{
		Vector vecShootOrigin, vecShootDir;
		vecShootOrigin = pOperator->Weapon_ShootPosition();

		CAI_BaseNPC* npc = pOperator->MyNPCPointer();
		ASSERT(npc != NULL);

		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

		FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
	}
	break;
	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}