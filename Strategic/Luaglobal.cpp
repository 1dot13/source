#include <iostream>
#include <string>
#include <sstream>


#include "connect.h"
#include "FileMan.h"
#include "GameSettings.h"
#include "Game Init.h"
#include "interface Dialogue.h"
#include "opplist.h"
#include "Strategic All.h"
#include "Luaglobal.h"
#include "Boxing.h"
#include "LuaInitNPCs.h"
#include "Game Clock.h"
#include "Overhead.h"
#include "Music Control.h"

#include "Intro.h"
#include "End Game.h"
#include "Queen Command.h"
#include "gamescreen.h"
#include "Map Screen Interface Map.h"

#include "Creature Spreading.h"
#include "World Items.h"
#include "Map Screen Helicopter.h"
#include "Campaign Types.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

extern	BOOLEAN	gfDoneWithSplashScreen;


void IniGlobalGameSetting(lua_State *L)
{
	// -------------------------------
	// Start new game settings (use "new" prefix)
	// -------------------------------

	lua_pushinteger(L, gGameOptions.ubGameStyle);
	lua_setglobal(L, "newGAME_STYLE");
	
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "newDIFFICULTY_LEVEL");

	// -------------------------------
	// ja2_options.ini Settings (use "ini" prefix)
	// -------------------------------

	lua_pushinteger(L, gGameExternalOptions.fEnableCrepitus);
	lua_setglobal(L, "iniENABLE_CREPITUS");

	// WANNE: Currently not used
	lua_pushboolean(L, gGameExternalOptions.gfInvestigateSector);
	lua_setglobal(L, "iniENEMY_INVESTIGATE_SECTOR");
	
	lua_pushboolean(L, gGameExternalOptions.fMercDayOne);
	lua_setglobal(L, "iniMERC_WEBSITE_IMMEDIATELY_AVAILABLE");

	lua_pushinteger(L, gGameExternalOptions.ubChanceTonyAvailable);
	lua_setglobal(L, "iniCHANCE_TONY_AVAILABLE");

	lua_pushinteger(L, gGameExternalOptions.iStartingCashNovice);
	lua_setglobal(L, "iniSTARTING_CASH_NOVICE");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashExperienced);
	lua_setglobal(L, "iniSTARTING_CASH_EXPERIENCED");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashExpert);
	lua_setglobal(L, "iniSTARTING_CASH_EXPERT");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashInsane);
	lua_setglobal(L, "iniSTARTING_CASH_INSANE");
	
	// WANNE: Currently not used
	lua_pushinteger(L, gGameExternalOptions.fCanTrueCiviliansBecomeHostile);
	lua_setglobal(L, "iniCAN_TRUE_CIVILIANS_BECOME_HOSTILE");

	// -------------------------------
	// Other global variables
	// -------------------------------

	lua_pushinteger(L, gGameExternalOptions.ubDefaultArrivalSectorY);
	lua_setglobal(L, "ubDefaultArrivalSectorY");
	
	lua_pushinteger(L, gGameExternalOptions.ubDefaultArrivalSectorX);
	lua_setglobal(L, "ubDefaultArrivalSectorX");
	
	lua_pushinteger(L, gGameExternalOptions.iFirstArrivalDelay);
	lua_setglobal(L, "iFirstArrivalDelay");
	
	lua_pushinteger(L, guiHelicopterSkyriderTalkState);
	lua_setglobal(L, "guiHelicopterSkyriderTalkState");
	
	lua_pushinteger(L, guiTimeOfLastSkyriderMonologue);
	lua_setglobal(L, "guiTimeOfLastSkyriderMonologue");
	
	lua_pushinteger(L, gubBloodcatLairSectorId);
	lua_setglobal(L, "gubBloodcatLairSectorId");
	
	lua_pushinteger(L, gubSrcSoldierProfile);
	lua_setglobal(L, "gubSrcSoldierProfile");

	lua_pushinteger(L, giLairID);
	lua_setglobal(L, "giLairID");
	
	lua_pushinteger(L, is_networked);
	lua_setglobal(L, "is_networked");
	
	lua_pushinteger(L, gWorldSectorX);
	lua_setglobal(L, "gWorldSectorX");
	
	lua_pushinteger(L, gWorldSectorY);
	lua_setglobal(L, "gWorldSectorY");
	
	lua_pushinteger(L, gbWorldSectorZ);
	lua_setglobal(L, "gWorldSectorZ");
	
	lua_pushinteger(L, gbWorldSectorZ);
	lua_setglobal(L, "gbWorldSectorZ");

	lua_pushinteger(L, giHospitalTempBalance);
	lua_setglobal(L, "giHospitalTempBalance");
	
	lua_pushinteger(L, gbHospitalPriceModifier);
	lua_setglobal(L, "gbHospitalPriceModifier");
	
	lua_pushinteger(L, giHospitalRefund);
	lua_setglobal(L, "giHospitalRefund");
	
	lua_pushboolean(L, gfBoxersResting);
	lua_setglobal(L, "gfBoxersResting");
	
	lua_pushinteger(L, gubBoxersRests);
	lua_setglobal(L, "gubBoxersRests");
	
	lua_pushinteger(L, gubBoxingMatchesWon);
	lua_setglobal(L, "gubBoxingMatchesWon");
	
	lua_pushinteger(L, uiHourLua);
	lua_setglobal(L, "cHour");
	
	lua_pushinteger(L, uiDayLua);
	lua_setglobal(L, "cDay");
	
	lua_pushinteger(L, uiMinLua);
	lua_setglobal(L, "cMin");
	
	lua_pushinteger(L, gbPlayerNum);
	lua_setglobal(L, "gbPlayerNum");
	
	lua_pushinteger(L, GetMusicHandle());
	lua_setglobal(L, "uiMusicHandle");
	
	lua_pushinteger(L, GetMusicMode());
	lua_setglobal(L, "gubMusicMode");
	
	lua_pushboolean(L, IsMusicPlaying());
	lua_setglobal(L, "fMusicPlaying");

	// unused
	//lua_pushboolean(L, gfForceMusicToTense);
	//lua_setglobal(L, "gfForceMusicToTense");

	// WANNE: Currently not used
	lua_pushinteger(L, guiNumWorldItems);
	lua_setglobal(L, "guiNumWorldItems");
	
	lua_pushboolean(L, gfEnteringMapScreen);
	lua_setglobal(L, "gfEnteringMapScreen");
	
	lua_pushboolean(L, guiIntroExitScreen);
	lua_setglobal(L, "guiIntroExitScreen");
	
	lua_pushboolean(L, gfIntroScreenExit);
	lua_setglobal(L, "gfIntroScreenExit");
	
	lua_pushinteger(L, sSelMapX);
	lua_setglobal(L, "sSelMapX");
	
	lua_pushinteger(L, sSelMapY);
	lua_setglobal(L, "sSelMapY");
	
	lua_pushinteger(L, iCurrentMapSectorZ);
	lua_setglobal(L, "iCurrentMapSectorZ");	
	
	lua_pushboolean(L, gfDoneWithSplashScreen);
	lua_setglobal(L, "gfDoneWithSplashScreen");		
	
	lua_pushinteger(L, gbIntroScreenMode);
	lua_setglobal(L, "gbIntroScreenMode");
}