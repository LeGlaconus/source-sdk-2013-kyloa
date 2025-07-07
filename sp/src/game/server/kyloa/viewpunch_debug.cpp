#include "convar.h"
#include "cbase.h"

//uhhhhh
extern ConCommand viewpunch_debug;

static void ViewPunchDebug_f(const CCommand &args)
{
	if (args.ArgC() < 5 || FStrEq(args.Arg(1), ""))
	{
		Warning("incorrect usage of viewpunch_debug !\n");
		Msg("%s \n", viewpunch_debug.GetHelpText());
	}

	CBasePlayer* pPlayer = UTIL_GetLocalPlayer();

	if (pPlayer == NULL)
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat(atof(args.Arg(1)), atof(args.Arg(2)));
	punchAng.y = random->RandomFloat(atof(args.Arg(3)), atof(args.Arg(4)));
	punchAng.z = atof(args.Arg(5));

	pPlayer->ViewPunch(punchAng);
}

ConCommand viewpunch_debug("viewpunch_debug", ViewPunchDebug_f, "Usage : xa xb ya yb z", FCVAR_NONE);