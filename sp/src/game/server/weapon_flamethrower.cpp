#include "cbase.h"
#include "weapon_flamethrower.h"

#include "tier0/memdbgon.h"

//------------
// Weapon
//------------

LINK_ENTITY_TO_CLASS(weapon_flamethrower, CWeaponFlamethrower);
PRECACHE_WEAPON_REGISTER(weapon_flamethrower);

IMPLEMENT_SERVERCLASS_ST(CWeaponFlamethrower, DT_WeaponFlamethrower)
	SendPropInt(SENDINFO(m_eState)),
END_SEND_TABLE();

BEGIN_DATADESC(CWeaponFlamethrower)
DEFINE_FIELD(m_flFireEjectStopTime, FIELD_TIME),
DEFINE_FIELD(m_flNextFireEjectTime, FIELD_TIME),
DEFINE_FIELD(m_flWarmupTime,		FIELD_FLOAT), //generic unit not real time
DEFINE_FIELD(m_eState,				FIELD_INTEGER),
DEFINE_SOUNDPATCH(m_pFireEjectSoundPatch),
END_DATADESC();


CWeaponFlamethrower::CWeaponFlamethrower()
	:m_pFireEjectSoundPatch(nullptr), BaseClass()
{
	m_fMinRange1 = m_fMinRange2 = 32;
	m_fMaxRange1 = m_fMaxRange2 = 192;

	m_iPrimaryAttacks = 0;
	m_bFiresUnderwater = false;
}

CWeaponFlamethrower::~CWeaponFlamethrower()
{
	CSoundEnvelopeController& controller = CSoundEnvelopeController::GetController();
	controller.SoundDestroy(m_pFireEjectSoundPatch);
}

void CWeaponFlamethrower::Precache()
{
	BaseClass::Precache();

	PrecacheScriptSound("Weapon_Flamethrower.LightUpFire");
	PrecacheParticleSystem("ft_flame");
	PrecacheParticleSystem("ft_flamejet");
	PrecacheParticleSystem("ft_flame_sec");
}

void CWeaponFlamethrower::Spawn()
{
	BaseClass::Spawn();
	
	CSoundEnvelopeController& controller = CSoundEnvelopeController::GetController();
	CPASAttenuationFilter filter(this);
	m_pFireEjectSoundPatch = controller.SoundCreate(filter, entindex(), GetShootSound(SINGLE));

	controller.Play(m_pFireEjectSoundPatch, 0.0f, PITCH_NORM);
}

void CWeaponFlamethrower::ItemPostFrame()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	if (pPlayer->m_nButtons & IN_ATTACK)
		PrimaryAttack();
	else if (pPlayer->m_nButtons & IN_ATTACK2)
		SecondaryAttackWarmup();
	else if (pPlayer->m_afButtonReleased & IN_ATTACK2)
	{
		if (gpGlobals->curtime >= m_flNextFireEjectTime)
			SecondaryAttackRelease();
	}
	else
		WeaponIdle();

	if(pPlayer->m_afButtonReleased & IN_ATTACK)
		if (!(pPlayer->m_nButtons & IN_ATTACK))
		{
			StopFireEjectSound();
			WeaponSound(SPECIAL1);
		}

}

void CWeaponFlamethrower::StopFireEjectSound()
{
	CSoundEnvelopeController& controller = CSoundEnvelopeController::GetController();
	controller.SoundChangeVolume(m_pFireEjectSoundPatch, 0.0f, 1.0f);
}

void CWeaponFlamethrower::WeaponIdle()
{
	m_eState = FT_State::Idle;

	BaseClass::WeaponIdle();
}

C_FT_Flame* CWeaponFlamethrower::ThrowFlame(CBasePlayer* pPlayer, Vector& vShootPosition, QAngle& vShootDir)
{
	C_FT_Flame* pFlame = C_FT_Flame::Create(vShootPosition, vShootDir, this, ft_flame_velocity.GetFloat());

	return pFlame;
}

void CWeaponFlamethrower::PrimaryAttack()
{
	if (gpGlobals->curtime < m_flNextFireEjectTime)
		return;

	m_flNextFireEjectTime = gpGlobals->curtime + ft_fireeject_interval.GetFloat();

		CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
		if (!pPlayer)
			return;

	if (pPlayer->GetWaterLevel() == 3 && !m_bFiresUnderwater)
	{
		// This weapon doesn't fire underwater
		m_eState = FT_State::Idle;

		StopFireEjectSound();

		WeaponSound(EMPTY);
		return;
	}

	if (HasPrimaryAmmo())
	{
		Vector vShootPosition = pPlayer->Weapon_ShootPosition();
		QAngle vShootDir = pPlayer->GetLocalAngles();

		//Vector vShootPosition;
		//QAngle vShootDir;
		//GetAttachment("flameout", vShootPosition, vShootDir);

		m_eState = FT_State::Primary;

		ThrowFlame(pPlayer, vShootPosition, vShootDir);

		m_iPrimaryAttacks++;
		if((m_iPrimaryAttacks % 4) == 0)
			pPlayer->RemoveAmmo(1, GetPrimaryAmmoType());


		CSoundEnvelopeController& controller = CSoundEnvelopeController::GetController();
		controller.SoundChangeVolume(m_pFireEjectSoundPatch, VOL_NORM, 1.0f);
	}
}

void CWeaponFlamethrower::SecondaryAttackWarmup()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	if (gpGlobals->curtime < m_flNextFireEjectTime)
		return;

	if (!HasPrimaryAmmo())
	{
		WeaponSound(EMPTY);
		m_flNextFireEjectTime = gpGlobals->curtime + ft_fireeject_interval_secondary.GetFloat();
		return;
	}

	m_flWarmupTime += gpGlobals->frametime;

	if (g_debug_ft.GetInt() >= 2)
		DevMsg("Warmup Time : %f\n", m_flWarmupTime);

	if(m_flWarmupTime <= ft_secondary_min_warmup_time.GetFloat())
		UTIL_ScreenShake(pPlayer->WorldSpaceCenter(), 4.0f, 2.5f, ft_secondary_max_warmup_time.GetFloat(), 256.0f, SHAKE_START, true);
	else
		UTIL_ScreenShake(pPlayer->WorldSpaceCenter(), 4.0f, Max(2.5f, m_flWarmupTime / (ft_secondary_max_warmup_time.GetFloat() / 4.0f)), ft_secondary_max_warmup_time.GetFloat(), 256.0f, SHAKE_FREQUENCY, true);

	if (m_flWarmupTime >= ft_secondary_max_warmup_time.GetFloat())
		SecondaryAttackRelease();
}

void CWeaponFlamethrower::SecondaryAttackRelease()
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	UTIL_ScreenShake(pPlayer->WorldSpaceCenter(), 4.0f, 2.5f, 2.0f, 256.0f, SHAKE_STOP, true);

	if (m_flWarmupTime <= ft_secondary_min_warmup_time.GetFloat())
		return;

	const float flLengthRemapped = (m_flWarmupTime * 1.5f) / ft_secondary_max_warmup_time.GetFloat();

	m_flWarmupTime = 0.0f;

	SecondaryAttack(flLengthRemapped);
}

void CWeaponFlamethrower::SecondaryAttack(float flLength)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	m_flNextFireEjectTime = gpGlobals->curtime + ft_fireeject_interval_secondary.GetFloat();

	if(g_debug_ft.GetBool())
		DevMsg("Secondary Attack ! Length : %f\n", flLength);

	constexpr const int iNumFlames = 8;

	C_FT_Flame* aFlames[iNumFlames];
	for (size_t i = 0; i < iNumFlames; i++)
	{
		aFlames[i] = ThrowFlame(pPlayer, pPlayer->Weapon_ShootPosition(), (QAngle&)pPlayer->GetLocalAngles());
		const Vector vPrevBaseVelocity = aFlames[i]->GetBaseVelocity();
		aFlames[i]->SetBaseVelocity(vPrevBaseVelocity * 1.75f * Max(1.0f, flLength));
	}

	m_iClip1 -= 4.0f;

	WeaponSound(WPN_DOUBLE);

	DispatchParticleEffect("ft_flame_sec", PATTACH_POINT_FOLLOW, (CBaseEntity*)this, "flameout", vec3_origin, vec3_origin, false);
}