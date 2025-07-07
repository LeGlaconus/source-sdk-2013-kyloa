
#include "cbase.h"
#include "convar.h"

#ifdef KYLOA_FMOD

//The same shitty workaround over again
extern ConCommand fmod_playsound;

#include "fmod_manager.h"

void PlayFMODSound_f(const CCommand& args)
{
	if (args.ArgC() <= 0 || FStrEq(args.Arg(1), ""))
	{
		Warning("incorrect usage of fmod_playsound !\n");
		Msg("%s \n", fmod_playsound.GetHelpText());
	}
	const char* szSoundName = args.Arg(1);

	DevMsg("Sound name : %s \n", szSoundName);
	if (FMODManager()->IsSoundPlaying(szSoundName))
	{
		DevWarning("Sound : %s is already playing ! \n", szSoundName);
	}
	else
	{
		DevMsg("Playing it\n");
		FMODManager()->PlayAmbientSound(szSoundName, false);
	}


}

ConCommand fmod_playsound("fmod_playsound", PlayFMODSound_f, "Plays a sound with FMOD.\nUsage : fmod_playsound <path/soundname.extension>", FCVAR_CHEAT);

void StopFMODSounds_f(const CCommand& args)
{
	bool bFadeOut = false;

	if (args.ArgC() >= 1 || !FStrEq(args.Arg(1), ""))
	{
		bFadeOut = (bool)atoi(args.Arg(1));
	}
		
	FMODManager()->StopAmbientSound(bFadeOut);
}

ConCommand fmod_stopsound("fmod_stopsound", StopFMODSounds_f, "Stops all currently playing sounds that were created with FMOD.\nUsage : fmod_stopsound [fadeout]", FCVAR_CHEAT);

#endif