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
#include "ub_config.h"

#include "Creature Spreading.h"
#include "World Items.h"
#include "Map Screen Helicopter.h"
#include "Campaign Types.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Ja25_Tactical.h"

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
	
#ifdef JA2UB
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "difficultyLevel");
#endif

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

#ifdef JA2UB
	lua_pushinteger(L, gGameUBOptions.LOCATEGRIDNO);
	lua_setglobal(L, "iniNEW_MERC_ARRIVAL_LOCATION");
#else
	lua_pushinteger(L, gGameExternalOptions.iInitialMercArrivalLocation);
	lua_setglobal(L, "iniNEW_MERC_ARRIVAL_LOCATION");
#endif
 
	lua_pushinteger(L, gGameExternalOptions.ubDefaultArrivalSectorY);
	lua_setglobal(L, "iniDEFAULT_ARRIVAL_SECTOR_Y");
	
	lua_pushinteger(L, gGameExternalOptions.ubDefaultArrivalSectorX);
	lua_setglobal(L, "iniDEFAULT_ARRIVAL_SECTOR_X");	
	
#ifdef JA2UB
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorX);
	lua_setglobal(L, "iniDEFAULT_END_SECTOR_X");
	
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorY);
	lua_setglobal(L, "iniDEFAULT_END_SECTOR_Y");
	
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorZ);
	lua_setglobal(L, "iniDEFAULT_END_SECTOR_Z");
	
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 0 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO1");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 1 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO2");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 2 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO3");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 3 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO4");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 4 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO5");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 5 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO6");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 6 ]);
	lua_setglobal(L, "iniINITIALHELIGRIDNO7");
	
	lua_pushinteger(L, gGameUBOptions.JerryGridNo);
	lua_setglobal(L, "iniJERRYGRIDNO");
	
	lua_pushboolean(L, gGameUBOptions.LaptopQuestEnabled);
	lua_setglobal(L, "iniLAPTOP_QUEST");
	
	lua_pushboolean(L, gGameUBOptions.InJerry);
	lua_setglobal(L, "iniJERRY");
	
	lua_pushboolean(L, gGameUBOptions.JerryQuotes);
	lua_setglobal(L, "iniJERRYQUOTES");
	
	lua_pushboolean(L, gGameUBOptions.InGameHeliCrash);
	lua_setglobal(L, "iniINGAMEHELICRASH");
	
	
	lua_pushboolean(L, gGameUBOptions.InGameHeli);
	lua_setglobal(L, "iniINGAMEHELI");
#endif
	
	// -------------------------------
	// Other global variables
	// -------------------------------
	
#ifdef JA2UB
	lua_pushinteger(L, gGameUBOptions.TestUB);
	lua_setglobal(L, "TestUB");
#endif
	
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

//	lua_pushboolean(L, gfForceMusicToTense);
//	lua_setglobal(L, "gfForceMusicToTense");
	
	//GameExternalOptions
	lua_pushboolean(L, gGameExternalOptions.gfInvestigateSector);
	lua_setglobal(L, "gfInvestigateSector");
	
	lua_pushboolean(L, gGameExternalOptions.fMercDayOne);
	lua_setglobal(L, "fMercDayOne");
	
	lua_pushinteger(L, gGameOptions.ubGameStyle);
	lua_setglobal(L, "gameStyle");
	
	lua_pushinteger(L, gGameExternalOptions.fEnableCrepitus);
	lua_setglobal(L, "enableCrepitus");
	
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "difficultyLevel");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashNovice);
	lua_setglobal(L, "startingCashNovice");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashExperienced);
	lua_setglobal(L, "startingCashExperienced");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashExpert);
	lua_setglobal(L, "startingCashExpert");
	
	lua_pushinteger(L, gGameExternalOptions.iStartingCashInsane);
	lua_setglobal(L, "startingCashInsane");
	
	lua_pushinteger(L, gGameExternalOptions.fCanTrueCiviliansBecomeHostile);
	lua_setglobal(L, "fCanTrueCiviliansBecomeHostile");

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

#ifdef JA2UB
	lua_pushboolean(L, gGameUBOptions.fTexAndJohn);
	lua_setglobal(L, "enabledJohnAndTex");		
	
	lua_pushboolean(L, gGameUBOptions.fRandomManuelText);
	lua_setglobal(L, "RandomManuelText");	
#endif
	
#ifdef JA2UB
	//Items
	lua_pushinteger(L, BARRETT_UB);
	lua_setglobal(L, "itemBARRETT_UB");	

	lua_pushinteger(L, CALICO_960_UB);
	lua_setglobal(L, "itemCALICO_960_UB");	
	
	lua_pushinteger(L, PSG1_UB);
	lua_setglobal(L, "itemPSG1_UB");	
	
	lua_pushinteger(L, L85_UB);
	lua_setglobal(L, "itemL85_UB");
	
	lua_pushinteger(L, TAR21_UB);
	lua_setglobal(L, "itemTAR21_UB");

	lua_pushinteger(L, VAL_SILENT_UB);
	lua_setglobal(L, "itemVAL_SILENT_UB");

	lua_pushinteger(L, MICRO_UZI_UB);
	lua_setglobal(L, "itemMICRO_UZI_UB");
	
	lua_pushinteger(L, CALICO_950_UB);
	lua_setglobal(L, "itemCALICO_950_UB");
	
	lua_pushinteger(L, CALICO_900_UB);
	lua_setglobal(L, "itemCALICO_900_UB");
	
	lua_pushinteger(L, CLIP_CANNON_BALL);
	lua_setglobal(L, "itemCLIP_CANNON_BALL");
	
	lua_pushinteger(L, MERC_UMBRELLA);
	lua_setglobal(L, "itemMERC_UMBRELLA");
	
	lua_pushinteger(L, HAND_CANNON);
	lua_setglobal(L, "itemHAND_CANNON");
	
	lua_pushinteger(L, HARTFORD_6_SHOOTER);
	lua_setglobal(L, "itemHARTFORD_6_SHOOTER");
	
	lua_pushinteger(L, TEX_MOVIE_ATTACK_CLYDESDALES);
	lua_setglobal(L, "itemTEX_MOVIE_ATTACK_CLYDESDALES");
	
	lua_pushinteger(L, TEX_MOVIE_WILD_EAST);
	lua_setglobal(L, "itemTEX_MOVIE_WILD_EAST");
	
	lua_pushboolean(L, TEX_MOVIE_HAVE_HONDA);
	lua_setglobal(L, "itemTEX_MOVIE_HAVE_HONDA");
	
	lua_pushboolean(L, LAPTOP_TRANSMITTER);
	lua_setglobal(L, "itemLAPTOP_TRANSMITTER");
	
	lua_pushboolean(L, CHE_GUEVARA_CANTEEN);
	lua_setglobal(L, "itemCHE_GUEVARA_CANTEEN");
	
	lua_pushboolean(L, MERC_WRISTWATCH);
	lua_setglobal(L, "itemMERC_WRISTWATCH");
	
	lua_pushboolean(L, SAM_GARVER_COMBAT_KNIFE);
	lua_setglobal(L, "itemSAM_GARVER_COMBAT_KNIFE");
	
	lua_pushboolean(L, MERC_UMBRELLA_OLD);
	lua_setglobal(L, "itemMERC_UMBRELLA_OLD");
	
	lua_pushboolean(L, MORRIS_INSTRUCTION_NOTE);
	lua_setglobal(L, "itemMORRIS_INSTRUCTION_NOTE");
#endif
}