#include "cbase.h"

#ifdef GAME_DLL

#include "baseachievement.h"
#include "achievementmgr.h"

CAchievementMgr g_AchievementMgrKyloa;

#define ACHIEVEMENT_KYLOA_GET_STAPLEGUN 1

DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_KYLOA_GET_STAPLEGUN, "KYLOA_GET_STAPLEGUN", 5);

#endif //GAME_DLL