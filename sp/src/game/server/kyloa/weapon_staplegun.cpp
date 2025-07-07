#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "te_effect_dispatch.h"
#include "gamestats.h"
#include "rumble_shared.h"

#include "basebludgeonweapon.h"

#include "weapon_staplegun.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM);

enum
{
	STAPLEGUN_VIEWKICK_HIT = 0,
	STAPLEGUN_VIEWKICK_MISS,
	STAPLEGUN_VIEWKICK_CRAWLER,
};

LINK_ENTITY_TO_CLASS(weapon_staplegun, CWeaponStapleGun);

PRECACHE_WEAPON_REGISTER(weapon_staplegun);

IMPLEMENT_SERVERCLASS_ST(CWeaponStapleGun, DT_WeaponStapleGun)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponStapleGun)

DEFINE_FIELD(m_iEnergyCounter, FIELD_INTEGER),

END_DATADESC()

acttable_t	CWeaponStapleGun::m_acttable[] =
{
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
	
	//Kyloa : temp test
	{ ACT_MELEE_ATTACK2,			ACT_MELEE_ATTACK_SWING,			true},
	

#ifdef MAPBASE
	// 
	// Activities ported from weapon_alyxgun below
	// 

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_PISTOL_RELAXED,		false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_PISTOL_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_STEALTH,				ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_RELAXED,				ACT_WALK_PISTOL_RELAXED,		false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_PISTOL_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_STEALTH,				ACT_WALK_STEALTH_PISTOL,		false },

	{ ACT_RUN_RELAXED,				ACT_RUN_PISTOL_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_PISTOL_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_STEALTH,				ACT_RUN_STEALTH_PISTOL,			false },

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_PISTOL_RELAXED,		false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_PISTOL_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_PISTOL,			false },//always aims
	{ ACT_IDLE_AIM_STEALTH,			ACT_IDLE_STEALTH_PISTOL,		false },

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_PISTOL_RELAXED,		false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_PISTOL,			false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_PISTOL,			false },//always aims
	{ ACT_WALK_AIM_STEALTH,			ACT_WALK_AIM_STEALTH_PISTOL,	false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_PISTOL_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_PISTOL,				false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_PISTOL,				false },//always aims
	{ ACT_RUN_AIM_STEALTH,			ACT_RUN_AIM_STEALTH_PISTOL,		false },//always aims
	//End readiness activities
#endif

	// Crouch activities
	{ ACT_CROUCHIDLE_STIMULATED,	ACT_CROUCHIDLE_STIMULATED,		false },
	{ ACT_CROUCHIDLE_AIM_STIMULATED,ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims
	{ ACT_CROUCHIDLE_AGITATED,		ACT_RANGE_AIM_PISTOL_LOW,		false },//always aims

	// Readiness translations
	{ ACT_READINESS_RELAXED_TO_STIMULATED, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED, false },
	{ ACT_READINESS_RELAXED_TO_STIMULATED_WALK, ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK, false },
	{ ACT_READINESS_AGITATED_TO_STIMULATED, ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED, false },
	{ ACT_READINESS_STIMULATED_TO_RELAXED, ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED, false },
#endif

#if EXPANDED_HL2_WEAPON_ACTIVITIES
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_PISTOL,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_PISTOL,		true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_PISTOL,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_PISTOL,		true },
#endif

#if EXPANDED_HL2_COVER_ACTIVITIES
	{ ACT_RANGE_AIM_MED,			ACT_RANGE_AIM_PISTOL_MED,			false },
	{ ACT_RANGE_ATTACK1_MED,		ACT_RANGE_ATTACK_PISTOL_MED,		false },

	{ ACT_COVER_WALL_R,			ACT_COVER_WALL_R_PISTOL,		false },
	{ ACT_COVER_WALL_L,			ACT_COVER_WALL_L_PISTOL,		false },
	{ ACT_COVER_WALL_LOW_R,		ACT_COVER_WALL_LOW_R_PISTOL,	false },
	{ ACT_COVER_WALL_LOW_L,		ACT_COVER_WALL_LOW_L_PISTOL,	false },
#endif

#ifdef MAPBASE
	// HL2:DM activities (for third-person animations in SP)
	{ ACT_HL2MP_IDLE,                    ACT_HL2MP_IDLE_PISTOL,                    false },
	{ ACT_HL2MP_RUN,                    ACT_HL2MP_RUN_PISTOL,                    false },
	{ ACT_HL2MP_IDLE_CROUCH,            ACT_HL2MP_IDLE_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_WALK_CROUCH,            ACT_HL2MP_WALK_CROUCH_PISTOL,            false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,    ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,    false },
	{ ACT_HL2MP_GESTURE_RELOAD,            ACT_HL2MP_GESTURE_RELOAD_PISTOL,        false },
	{ ACT_HL2MP_JUMP,                    ACT_HL2MP_JUMP_PISTOL,                    false },
	{ ACT_HL2MP_WALK,					ACT_HL2MP_WALK_PISTOL,						false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK2,	ACT_HL2MP_GESTURE_RANGE_ATTACK2_PISTOL,		false },
};


IMPLEMENT_ACTTABLE(CWeaponStapleGun);

// Allows Weapon_BackupActivity() to access the staplegun's activity table.
acttable_t* GetStapleGunActtable()
{
	return CWeaponStapleGun::m_acttable;
}

int GetStapleGunActtableCount()
{
	return ARRAYSIZE(CWeaponStapleGun::m_acttable);
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponStapleGun::CWeaponStapleGun()
{
	AddSpawnFlags(SF_ENERGY_WEAPON);
	
	m_bReloadsSingly = false;
	m_bFiresUnderwater = true;
	m_bAltFiresUnderwater = true;

	m_iEnergyCounter = 3;
	//m_iEnergy = STAPLEGUN_MAX_ENERGY;
	//m_iEnergy = GetEnergy();

	//Kyloa TODO : those values could be lowered, NPCs tend to shoot from far away
	m_fMinRange1 = 12;
	m_fMaxRange1 = 1000;
	m_fMinRange2 = 12;
	m_fMaxRange2 = 200;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponStapleGun::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	switch (pEvent->event)
	{
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

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponStapleGun::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	WeaponSound(SINGLE_NPC);
	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 1);
	pOperator->DoMuzzleFlash();
	m_iClip1--;
}

//-----------------------------------------------------------------------------
// Purpose: Some things need this. (e.g. the new Force(X)Fire inputs or blindfire actbusy)
//-----------------------------------------------------------------------------
void CWeaponStapleGun::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	AngleVectors(angShootDir, &vecShootDir);
	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponStapleGun::PrimaryAttack()
{
	// Only the player fires this way so we can cast
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
		}

		return;
	}

	//Not enough energy
	//if (m_iClip2 <= 0)
	if (GetEnergy() <= 0)
	{
		//Taunt because Special 1, 2 & 3 are already used and that I don't want to touch weapon_parse.cpp ever again
		WeaponSound(TAUNT);
		m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
		SendWeaponAnim(ACT_VM_DRYFIRE);

		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	WeaponSound(SINGLE);
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRate();

	m_iClip1--;

	//Decrease 1/6 * 3th of the energy
	m_iEnergyCounter--;
	if (m_iEnergyCounter <= 0)
	{
		m_iEnergyCounter = 3;
	//	m_iClip2--;
		AddEnergy(-1);
	}

	//DevMsg("Energy counter : %i \n", m_iEnergyCounter);
	//DevMsg("Energy : %i \n", (int)m_iClip2);
	//DevMsg("Energy : %i \n", (int)m_iEnergy);

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	//pPlayer->FireBullets(1, vecSrc, vecAiming, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0);
	pPlayer->FireBullets(1, vecSrc, vecAiming, pPlayer->GetAttackSpread(this), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0);

	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);

	pPlayer->ViewPunch(QAngle(-2, random->RandomFloat(-0.5, 0.5), 0));

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner());
}

Activity CWeaponStapleGun::GetSecondaryAttackActivity()
{
	return ACT_VM_HITCENTER2;
}

Activity CWeaponStapleGun::ChooseIntersectionPointAndActivity(trace_t& hitTrace, const Vector& mins, const Vector& maxs, CBasePlayer* pOwner)
{
	int			i, j, k;
	float		distance;
	const float* minmaxs[2] = { mins.Base(), maxs.Base() };
	trace_t		tmpTrace;
	Vector		vecHullEnd = hitTrace.endpos;
	Vector		vecEnd;

	distance = 1e6f;
	Vector vecSrc = hitTrace.startpos;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc) * 2);
	UTIL_TraceLine(vecSrc, vecHullEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace);
	if (tmpTrace.fraction == 1.0)
	{
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				for (k = 0; k < 2; k++)
				{
					vecEnd.x = vecHullEnd.x + minmaxs[i][0];
					vecEnd.y = vecHullEnd.y + minmaxs[j][1];
					vecEnd.z = vecHullEnd.z + minmaxs[k][2];

					UTIL_TraceLine(vecSrc, vecEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &tmpTrace);
					if (tmpTrace.fraction < 1.0)
					{
						float thisDistance = (tmpTrace.endpos - vecSrc).Length();
						if (thisDistance < distance)
						{
							hitTrace = tmpTrace;
							distance = thisDistance;
						}
					}
				}
			}
		}
	}
	else
	{
		hitTrace = tmpTrace;
	}


	return GetSecondaryAttackActivity();
}

bool CWeaponStapleGun::ImpactWater(const Vector& start, const Vector& end)
{
	//FIXME: This doesn't handle the case of trying to splash while being underwater, but that's not going to look good
	//		 right now anyway...

	// We must start outside the water
	if (UTIL_PointContents(start) & (CONTENTS_WATER | CONTENTS_SLIME))
		return false;

	// We must end inside of water
	if (!(UTIL_PointContents(end) & (CONTENTS_WATER | CONTENTS_SLIME)))
		return false;

	trace_t	waterTrace;

	UTIL_TraceLine(start, end, (CONTENTS_WATER | CONTENTS_SLIME), GetOwner(), COLLISION_GROUP_NONE, &waterTrace);

	if (waterTrace.fraction < 1.0f)
	{
		CEffectData	data;

		data.m_fFlags = 0;
		data.m_vOrigin = waterTrace.endpos;
		data.m_vNormal = waterTrace.plane.normal;
		data.m_flScale = 8.0f;

		// See if we hit slime
		//Kyloa TODO : better way to check for slime because %CompileSlime sucks
		//Carry the change over to basebludgeonweapon.cpp
		if (waterTrace.contents & CONTENTS_SLIME)
		{
			data.m_fFlags |= FX_WATER_IN_SLIME;
		}

		DispatchEffect("watersplash", data);
	}

	return true;
}

void CWeaponStapleGun::ImpactEffect(trace_t& traceHit)
{
	// See if we hit water (we don't do the other impact effects in this case)
	if (ImpactWater(traceHit.startpos, traceHit.endpos))
		return;

	//FIXME: need new decals
	UTIL_ImpactTrace(&traceHit, DMG_CLUB);
}

void CWeaponStapleGun::AddViewKick(int type)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	QAngle punchAng;
	
	switch (type)
	{
	case STAPLEGUN_VIEWKICK_HIT:
		punchAng.x = random->RandomFloat(1.5f, 0.5f);
		punchAng.y = random->RandomFloat(2.0f, 0.5f);
		punchAng.z = 0.05f;
		break;

	case STAPLEGUN_VIEWKICK_MISS:
		punchAng.x = random->RandomFloat(1.75f, 0.5f);
		punchAng.y = random->RandomFloat(2.0f, 2.0f);
		punchAng.z = 0.0f;
		break;

	case STAPLEGUN_VIEWKICK_CRAWLER:
		//there will probably be a more compliacated animation

		punchAng.x = random->RandomFloat(2.0f, 3.0f);
		punchAng.y = random->RandomFloat(-3.0f, -2.0f);
		//might need to switch back to 0.0f
		punchAng.z = 0.10f;
		break;

	default:
		break;
	}

	pPlayer->ViewPunch(punchAng);
}

//Because we are sure this is a crawler, we could make it get a CNPCCrawler* instead,
// but that would require casting it before calling the function
void CWeaponStapleGun::FatalitySequence(CBaseEntity* pCrawler, CBasePlayer* pPlayer)
{
	//Instead of using an input, I could call a function directly
	variant_t emptyVariant;
	pCrawler->AcceptInput("StartExtract", pPlayer, this, emptyVariant, 0);
	
	//Play animation here and shit

	//Does not refill the enregy anymore, the crawlers do that
	//m_iClip2 = GetMaxClip2();
	//m_iEnergy = STAPLEGUN_MAX_ENERGY;
}

//void CWeaponStapleGun::AddEnergy(int iAmount)
//{
//	if (iAmount <= 0)
//		return;
//
//	if (m_iEnergy + iAmount >= STAPLEGUN_MAX_ENERGY)
//	{
//		m_iEnergy += iAmount;
//		//Fraction system ?
//		m_iEnergyCounter = 3;
//	}
//}

void CWeaponStapleGun::Hit(trace_t& traceHit, Activity nHitActivity, bool bIsSecondary, bool bHitCrawler)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (bHitCrawler)
		AddViewKick(STAPLEGUN_VIEWKICK_CRAWLER);
	else
		AddViewKick(STAPLEGUN_VIEWKICK_HIT);

	//Make sound for the AI
	CSoundEnt::InsertSound(SOUND_BULLET_IMPACT, traceHit.endpos, 400, 0.2f, pPlayer);

	// This isn't great, but it's something for when the crowbar hits.
	pPlayer->RumbleEffect(RUMBLE_AR2, 0, RUMBLE_FLAG_RESTART);

	CBaseEntity* pHitEntity = traceHit.m_pEnt;

	//Start extraction sequence
	if (bHitCrawler)
	{
		FatalitySequence(pHitEntity, pPlayer);
	}

	//Apply damage to a hit target
	if (pHitEntity != NULL)
	{
		Vector hitDirection;
		pPlayer->EyeVectors(&hitDirection, NULL, NULL);
		VectorNormalize(hitDirection);

		CTakeDamageInfo info(GetOwner(), GetOwner(), GetDamageForActivity(nHitActivity), GetDamageType());


		if (pPlayer && pHitEntity->IsNPC())
		{
			// If bonking an NPC, adjust damage.
			info.AdjustPlayerDamageInflictedForSkillLevel();
		}

		CalculateMeleeDamageForce(&info, hitDirection, traceHit.endpos);


		//Kyloa TODO : Crawlers should die instantly, but maybe this should be handled in their StartExtract input;
		//then this call shouldn't exist at all
		pHitEntity->DispatchTraceAttack(info, hitDirection, &traceHit);
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers(info, traceHit.startpos, traceHit.endpos, hitDirection);

		if (ToBaseCombatCharacter(pHitEntity))
		{
			gamestats->Event_WeaponHit(pPlayer, !bIsSecondary, GetClassname(), info);
		}
	}

	// Apply an impact effect
	ImpactEffect(traceHit);
}

float CWeaponStapleGun::GetDamageForActivity(Activity hitActivity)
{
	if ((GetOwner() != NULL) && (GetOwner()->IsPlayer()))
		return sk_plr_dmg_staplegun_melee.GetFloat();

	return sk_npc_dmg_staplegun_melee.GetFloat();
}

void CWeaponStapleGun::SecondaryAttack()
{
	trace_t traceHit;

	// Try a ray
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	pOwner->RumbleEffect(RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART);

	Vector swingStart = pOwner->Weapon_ShootPosition();
	Vector forward;

	forward = pOwner->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT, GetRange());

	Vector swingEnd = swingStart + forward * GetRange();

	//Kyloa TODO : Bullshit, never works

	/*
	Vector viewAngle = swingEnd - swingStart;
	VectorNormalize(viewAngle);
	//0.25 can be changed with an actual angle
	float viewAngleDot = viewAngle.Dot(Vector(0, 0, 1));
	if (viewAngleDot > 0.25f)
	{
		NDebugOverlay::Line(swingEnd, swingStart, 255, 0, 0, true, 4);
		DevMsg("%s \n", VecToString(swingEnd));
		swingEnd *= 0.75f;
		DevMsg("%s \n", VecToString(swingEnd));
		NDebugOverlay::Line(swingEnd, swingStart, 0, 255, 0, true, 4);
	}
	*/

	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit);
	Activity nHitActivity = GetSecondaryAttackActivity();

	// Like bullets, bludgeon traces have to trace against triggers.
	CTakeDamageInfo triggerInfo(GetOwner(), GetOwner(), GetDamageForActivity(nHitActivity), GetDamageType());
	triggerInfo.SetDamagePosition(traceHit.startpos);
	triggerInfo.SetDamageForce(forward);
	TraceAttackToTriggers(triggerInfo, traceHit.startpos, traceHit.endpos, forward);

	//Kyloa Crawler mechanic
	bool bHitCrawler = false;
	//======

	if (traceHit.fraction == 1.0)
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull(swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit);
		if (traceHit.fraction < 1.0 && traceHit.m_pEnt)
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize(vecToTarget);

			float dot = vecToTarget.Dot(forward);

			// YWB:  Make sure they are sort of facing the guy at least...
			if (dot < 0.70721f)
			{
				// Force amiss
				traceHit.fraction = 1.0f;
			}
			else
			{
				nHitActivity = ChooseIntersectionPointAndActivity(traceHit, g_bludgeonMins, g_bludgeonMaxs, pOwner);
			}
		}
	}

	m_iSecondaryAttacks++;

	gamestats->Event_WeaponFired(pOwner, false, GetClassname());

	// -------------------------
	//	Miss
	// -------------------------
	if (traceHit.fraction == 1.0f)
	{
		nHitActivity = ACT_VM_MISSCENTER2;

		AddViewKick(STAPLEGUN_VIEWKICK_MISS);
	}

	//Special Crawler mechanic, done here because the weapon animation is sent right after and that I need to send a custom one
	if (traceHit.m_pEnt)
	{
		if (FClassnameIs(traceHit.m_pEnt, "npc_crawler"))
		{
			bHitCrawler = true;
			//not defining a new one, this is for the hitkill sequence
			nHitActivity = ACT_VM_SWINGHARD;
		}
	}

	// Send the anim
	SendWeaponAnim(nHitActivity);

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	//GetFireRateMelee is added so that I can shorten the second attack with a cvar
	m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration() + GetFireRateMelee();

	pOwner->SetAnimation(PLAYER_ATTACK1);

	if (GetHitDelay() > 0.f)
	{
		//Play swing sound
		//Special 3 because single is already used
		WeaponSound(SPECIAL3);

	}
	else
	{
		if (traceHit.fraction == 1.0f)
		{
			// We want to test the first swing again
			Vector testEnd = swingStart + forward * GetRange();

			//Play swing sound
			WeaponSound(SPECIAL3);

			// See if we happened to hit water
			ImpactWater(swingStart, testEnd);
		}
		else
		{
			// Other melee sounds
			if (traceHit.m_pEnt && traceHit.m_pEnt->IsWorld())
				WeaponSound(MELEE_HIT_WORLD);
			else if (traceHit.m_pEnt && !traceHit.m_pEnt->PassesDamageFilter(triggerInfo))
				WeaponSound(MELEE_MISS);
			else
				WeaponSound(MELEE_HIT);

			Hit(traceHit, nHitActivity, true, bHitCrawler);
		}
	}
}

static void RefillEnergy_f(const CCommand &args)
{
	CHL2_Player* player = (CHL2_Player*)UTIL_GetLocalPlayer();
	if (player != nullptr)
	{
		player->SetEnergy(ENERGY_MAX_STAPLEGUN);

	}
	//reset energy counter ?
}

ConCommand staplegun_refillenergy("staplegun_refillenergy", RefillEnergy_f, "Refills the energy, temporary", FCVAR_CHEAT);