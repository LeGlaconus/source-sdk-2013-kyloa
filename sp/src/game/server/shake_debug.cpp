#include "cbase.h"
#include "util.h"

extern ConCommand shake_debug;

static void ShakeDebug_f(const CCommand& args)
{
	if (args.ArgC() <= 6)
	{
		Warning("Incorrect usage !\n");
		Msg("%s\n", shake_debug.GetHelpText());
		return;
	}

	CBasePlayer* pPlayer = UTIL_GetCommandClient();
	if (!pPlayer)
		return;

	UTIL_ScreenShake(pPlayer->WorldSpaceCenter(), atof(args.Arg(1)), atof(args.Arg(2)), atof(args.Arg(3)), atof(args.Arg(4)), (ShakeCommand_t)atoi(args.Arg(5)), atof(args.Arg(6)));
}

ConCommand shake_debug("shake_debug", ShakeDebug_f, "Shake debugging\nUsage : amplitude frequency duration radius command airshake", FCVAR_CHEAT);