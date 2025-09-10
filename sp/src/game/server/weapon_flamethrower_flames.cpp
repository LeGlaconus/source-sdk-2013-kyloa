#include "cbase.h"
#include "weapon_flamethrower_flames.h"

#include "tier0/memdbgon.h"


static constexpr const int iFlameDmgType = (DMG_BURN | DMG_NEVERGIB);
static constexpr const float flFlameLifetimeOnEntities = 25.0f;
static constexpr const float flFlameLifetimeOnBreakables = flFlameLifetimeOnEntities / 2.5f;

LINK_ENTITY_TO_CLASS(flamethrower_flame, C_FT_Flame);
IMPLEMENT_AUTO_LIST(IFT_FlameAutoList);

BEGIN_DATADESC(C_FT_Flame)
DEFINE_FIELD(m_vInitialPos,		FIELD_VECTOR),
DEFINE_FIELD(m_vPrevPos,		FIELD_VECTOR),
DEFINE_FIELD(m_vBaseVelocity,	FIELD_VECTOR),
DEFINE_FIELD(m_vAddedImpulse,	FIELD_VECTOR),
DEFINE_FIELD(m_flRemovalTime,	FIELD_TIME),
END_DATADESC();

IMPLEMENT_SERVERCLASS_ST(C_FT_Flame, DT_FT_Flame)
	SendPropTime(SENDINFO(m_flRemovalTime)),
END_SEND_TABLE();

C_FT_Flame::C_FT_Flame()
{ }

void C_FT_Flame::Spawn()
{
	SetSolid(SOLID_NONE);
	SetSolidFlags(FSOLID_NOT_SOLID);
	SetCollisionGroup(COLLISION_GROUP_NONE);
	SetMoveType(MOVETYPE_NOCLIP, MOVECOLLIDE_DEFAULT);
	AddEFlags(EFL_NO_WATER_VELOCITY_CHANGE);
	SetTakeDamage(DAMAGE_NO);

	const float flBoxSize = ft_flame_size.GetFloat();
	UTIL_SetSize(this, -Vector(flBoxSize, flBoxSize, flBoxSize), Vector(flBoxSize, flBoxSize, flBoxSize));

	m_vInitialPos = GetAbsOrigin();
	m_vPrevPos = m_vInitialPos;

	CWeaponFlamethrower* pWeapon = dynamic_cast<CWeaponFlamethrower*>(GetOwnerEntity());
	if(pWeapon)
	{
		m_flRemovalTime = gpGlobals->curtime + (ft_flame_lifetime.GetFloat() * random->RandomFloat(0.9f, 1.1f));
	}
	else
	{
		KillFlame();
		DevMsg("Flamethrower flame doesn't have a valid owner !\n");
	}

	SetNextThink(gpGlobals->curtime);
}

void C_FT_Flame::Think()
{
	//Did it reach the end of the free trial phase
	if (gpGlobals->curtime > m_flRemovalTime)
	{
		KillFlame();
		return;
	}
	else
		SetNextThink(gpGlobals->curtime);

	if (g_debug_ft.GetBool())
		NDebugOverlay::Sphere(GetAbsOrigin(), ft_flame_size.GetFloat(), 255, 0, 0, false, gpGlobals->frametime * 4.0f);

	//Apply damage if the flame moved
	if (GetAbsOrigin() != m_vPrevPos)
	{
		const Vector vCurPos = GetAbsOrigin();

		Ray_t rWorld;
		rWorld.Init(m_vInitialPos, vCurPos, WorldAlignMins(), WorldAlignMaxs());

		trace_t trWorld;
		UTIL_TraceRay(rWorld, MASK_SOLID, this, COLLISION_GROUP_DEBRIS, &trWorld);

		const bool bHitWorld = trWorld.DidHitWorld();

		Ray_t rEnt;
		rEnt.Init(m_vPrevPos, GetAbsOrigin(), WorldAlignMins(), WorldAlignMaxs());

		//The owner of the flamethrower is the player
		C_FT_FlameEnumerator eFlameEnum(GetOwnerEntity()->GetOwnerEntity());
		enginetrace->EnumerateEntities(rEnt, false, &eFlameEnum);

		bool bHitSomething = false;
		FOR_EACH_VEC(eFlameEnum.GetTargets(), i)
		{
			CBaseEntity* pEnt = eFlameEnum.GetTargets()[i];

			//int iIndex = m_hBurntEntities.Find(pEnt);
			//if (iIndex != m_hBurntEntities.InvalidIndex())
			//	continue;

			//This gets checked again and invalidates the call to apply damage to the entity
			//if (pEnt->GetFlags() & FL_ONFIRE)
			//	continue;

			//Was this entity hit before touching the world
			if (bHitWorld)
			{
				trace_t trEnt;
				enginetrace->ClipRayToEntity(rWorld, MASK_SOLID | CONTENTS_HITBOX, pEnt, &trEnt);
				if (trEnt.fraction >= trWorld.fraction)
					continue;
			}

			Collide(pEnt);

			bHitSomething = true;
		}

		LightUpNearbyFires();

		DecalTrace(&trWorld, "SmallScorch"); //test : outside of bHitWorld check
		
		if (bHitWorld)
		{
			KillFlame();
		}

		m_vBaseVelocity *= ft_flame_drag.GetFloat();
		const Vector vVelocity = m_vBaseVelocity + Vector(0, 0, -ft_flame_gravity.GetFloat()) + m_vAddedImpulse;
		SetAbsVelocity(vVelocity);

		m_vPrevPos = GetAbsOrigin();
	}
}

inline void C_FT_Flame::KillFlame()
{
	UTIL_Remove(this);
}

bool C_FT_Flame::IsBurnable(CBaseAnimating* pEnt)
{	
	if (CAI_BaseNPC* pNpc = dynamic_cast<CAI_BaseNPC*>(pEnt))
	{
		CBasePlayer* pPlayer = ToBasePlayer(GetOwnerEntity()->GetOwnerEntity());
		const Disposition_t eRelationship = pNpc->IRelationType(pPlayer);
		bool bBurnOrNah = true;
		if ((eRelationship == D_ER) || (eRelationship == D_LI))
			bBurnOrNah = false;
		//DevMsg("Burn or nah : %i\n", bBurnOrNah);
		return bBurnOrNah;
	}
	
	if (dynamic_cast<CBasePlayer*>(pEnt) || dynamic_cast<CBaseProp*>(pEnt) || dynamic_cast<CRagdollProp*>(pEnt))
		return true;

	return false;
}

static constexpr char* strFlammablePropData[] =
{
	"Wooden.",
	"Cardboard.",
	"Plastic.",
};

void C_FT_Flame::Collide(CBaseEntity* pOther)
{
	if ((UTIL_PointContents(GetAbsOrigin()) & MASK_WATER))
	{
		KillFlame();
		return;
	}

	CBaseEntity* pAttacker = GetOwnerEntity()->GetOwnerEntity();
	if (!pAttacker)
		return;

	//------ ------ ------ ------
	//If the entity was burnt previously :

	const float flDamage = Max(ft_flame_dmg.GetFloat(), 0.1f);

	CTakeDamageInfo info(GetOwnerEntity(), pAttacker, GetOwnerEntity(), flDamage, iFlameDmgType, 0);
	info.SetReportedPosition(pAttacker->GetAbsOrigin());

	//Kyloa TODO : m_hBurntEntities resets between Collide() calls, and sometimes not
	//if (m_hBurntEntities.Find(pOther) != m_hBurntEntities.InvalidIndex())
	if (pOther->GetFlags() & FL_ONFIRE)
	{
		if (random->RandomInt(0, 10) != 0)
			return;

		trace_t tr;
		UTIL_TraceLine(WorldSpaceCenter(), pOther->WorldSpaceCenter(), MASK_SOLID | CONTENTS_HITBOX, this, COLLISION_GROUP_NONE, &tr);
	
		pOther->DispatchTraceAttack(info, GetAbsVelocity(), &tr);
		ApplyMultiDamage();

		return;
	}

	//------ ------ ------ ------
	//If the entity was never burnt previously :

	//So nested it hurts
	if (CBaseAnimating* pOtherDynamic = dynamic_cast<CBaseAnimating*>(pOther))
	{
		//you really thought I was gonna use goto
		const bool bSkipChecks = ft_flame_burn_EVERYTHING.GetBool();

		if (bSkipChecks || IsBurnable(pOtherDynamic))
		{
			if (bSkipChecks || pOtherDynamic->PassesDamageFilter(info))
			{
				if (CBreakableProp* pOtherBreakable = dynamic_cast<CBreakableProp*>(pOther))
				{
					bool bHackFlammable = false;

					if (bSkipChecks)
					{
						bHackFlammable = true;
					}
					else
					{
						//Kyloa TODO : probably very slow, it would be better to check hashes
						const char* szPropData = STRING(pOtherBreakable->GetBasePropData());
						for (size_t i = 0; i < ARRAYSIZE(strFlammablePropData); i++)
						{
							const char* szCurStr = strFlammablePropData[i];
							if (!Q_strncmp(szPropData, szCurStr, Q_strlen(szCurStr)))
							{
								bHackFlammable = true;
								break;
							}
							else
								continue;
						}
					}

					if(bHackFlammable)
						pOtherBreakable->SetInteraction(PROPINTER_FIRE_FLAMMABLE);

					pOtherBreakable->Ignite(flFlameLifetimeOnBreakables, false);

					if (bHackFlammable)
						pOtherBreakable->RemoveInteraction(PROPINTER_FIRE_FLAMMABLE);
				}
				else
				{
					pOtherDynamic->Ignite(flFlameLifetimeOnEntities, false);
					//FireSystem_StartFire(pOtherDynamic, pOtherDynamic->BoundingRadius(), 6.0f, FT_FLAME_LIFETIME_ON_ENTITIES, SF_FIRE_SMOKELESS | SF_FIRE_VISIBLE_FROM_ABOVE | SF_FIRE_DONT_DROP, GetOwnerEntity(), FIRE_NATURAL);
				}

				//if (pOtherDynamic->IsOnFire())
				//{
				//	//Remember that we've burnt this entity
				//	m_hBurntEntities.AddToTail(pOther);
				//}
			}
		}
	}
}

void C_FT_Flame::LightUpNearbyFires()
{
	constexpr const int iFiresToCheck = 4;

	CFire* pFires[iFiresToCheck];

	int fireCount = FireSystem_GetFiresInSphere(pFires, iFiresToCheck, false, GetAbsOrigin(), ft_flame_size.GetFloat());
	for (int i = 0; i < fireCount; i++)
	{
		pFires[i]->AddHeat(ft_flame_heat.GetFloat(), false, true);
	}

	if (fireCount > 0)
		KillFlame();
}

C_FT_Flame* C_FT_Flame::Create(const Vector& vecOrigin, const QAngle& vecAngles, CBaseEntity* pOwner, float flSpeed)
{
	C_FT_Flame* pFlame = static_cast<C_FT_Flame*>(BaseClass::Create("flamethrower_flame", vecOrigin, vecAngles, pOwner));
	if (!pFlame)
		return nullptr;
	//No character holding the weapon
	if (!pOwner->GetOwnerEntity())
		return nullptr;

	Vector vForward, vRight, vUp;
	AngleVectors(vecAngles, &vForward, &vRight, &vUp);

	pFlame->m_vBaseVelocity = vForward * flSpeed;
	const float vRand = ft_flame_vecrand.GetFloat();
	pFlame->m_vBaseVelocity += RandomVector(-flSpeed * vRand, flSpeed * vRand);

	pFlame->m_vAddedImpulse = pOwner->GetOwnerEntity()->GetAbsVelocity();
	pFlame->m_vAddedImpulse /= 8.0f;

	pFlame->SetAbsVelocity(pFlame->m_vBaseVelocity);
	pFlame->SetAbsAngles(vecAngles);

	//pFlame->m_vPrevPos += Vector(0, 0, 1); //hack

	return pFlame;
}

//------------------------------------------------

C_FT_FlameEnumerator::C_FT_FlameEnumerator(CBaseEntity* pIgnore)
	:m_pIgnore(pIgnore)
{ }

bool C_FT_FlameEnumerator::EnumEntity(IHandleEntity* pHandleEntity)
{
	CBaseEntity* pEnt = static_cast<CBaseEntity*>(pHandleEntity);

	if (m_pIgnore == pEnt)
		return true;

	//Only entities derived from CBaseAnimating can burn
	if (dynamic_cast<CBaseAnimating*>(pEnt) != nullptr)
		//Kyloa Confirm : I imagine that there's no need to do a check ?
		m_vecTargets.AddToTail(pEnt);

	//Kept for mp lol
	else if (pEnt->IsPlayer() && pEnt->IsAlive())
		m_vecTargets.AddToTail(pEnt);

	return true;
}

//------------------------------------------------