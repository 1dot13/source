#include <iostream>
#include <sstream>


#include "connect.h"
#include "FileMan.h"
#include "GameSettings.h"
#include "interface Dialogue.h"
#include "Strategic All.h"
#include "Luaglobal.h"
#include "Boxing.h"
#include "LuaInitNPCs.h"
#include "Overhead.h"
#include "Music Control.h"

#include "Intro.h"
#include "Queen Command.h"
#include "gamescreen.h"
#include "Map Screen Interface Map.h"

#include "Creature Spreading.h"
#include "World Items.h"
#include "Map Screen Helicopter.h"
#include "Campaign Types.h"

#include "ub_config.h"
#include "Ja25_Tactical.h"

#include "Game Clock.h"

#include "Dialogue Control.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


using namespace std;

extern	BOOLEAN	gfDoneWithSplashScreen;
extern UINT8	gubWaitingForAllMercsToExitCode;


void IniGlobalGameSetting(lua_State *L)
{

	lua_pushinteger(L, giNPCReferenceCount);
	lua_setglobal(L, "giNPCReferenceCount");
	
	lua_pushinteger(L, gubWaitingForAllMercsToExitCode);
	lua_setglobal(L, "gubWaitingForAllMercsToExitCode");
	
	// -------------------------------
	// Start new game settings (use "new" prefix)
	// -------------------------------

	lua_pushinteger(L, gGameOptions.ubGameStyle);
	lua_setglobal(L, "newGAME_STYLE");
	
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "newDIFFICULTY_LEVEL");
	
#ifdef JA2UB
	//Old
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "difficultyLevel");
	
	//new
	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "UB_difficultyLevel");
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
	//old
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
	
	//new
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorX);
	lua_setglobal(L, "UB_iniDEFAULT_END_SECTOR_X");
	
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorY);
	lua_setglobal(L, "UB_iniDEFAULT_END_SECTOR_Y");
	
	lua_pushinteger(L, gGameUBOptions.ubEndDefaultSectorZ);
	lua_setglobal(L, "UB_iniDEFAULT_END_SECTOR_Z");
	
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 0 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO1");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 1 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO2");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 2 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO3");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 3 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO4");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 4 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO5");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 5 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO6");
	lua_pushinteger(L, gGameUBOptions.InitialHeliGridNo[ 6 ]);
	lua_setglobal(L, "UB_iniINITIALHELIGRIDNO7");
	
	lua_pushinteger(L, gGameUBOptions.JerryGridNo);
	lua_setglobal(L, "UB_iniJERRYGRIDNO");
	
	lua_pushboolean(L, gGameUBOptions.LaptopQuestEnabled);
	lua_setglobal(L, "UB_iniLAPTOP_QUEST");
	
	lua_pushboolean(L, gGameUBOptions.InJerry);
	lua_setglobal(L, "UB_iniJERRY");
	
	lua_pushboolean(L, gGameUBOptions.JerryQuotes);
	lua_setglobal(L, "UB_iniJERRYQUOTES");
	
	lua_pushboolean(L, gGameUBOptions.InGameHeliCrash);
	lua_setglobal(L, "UB_iniINGAMEHELICRASH");
	
	
	lua_pushboolean(L, gGameUBOptions.InGameHeli);
	lua_setglobal(L, "UB_iniINGAMEHELI");
	
	
	
	lua_pushinteger(L, gGameUBOptions.SectorLaunchMisslesX);
	lua_setglobal(L, "UB_iniSECTOR_LAUNCH_MISSLES_X");
	
	lua_pushinteger(L, gGameUBOptions.SectorLaunchMisslesY);
	lua_setglobal(L, "UB_iniSECTOR_LAUNCH_MISSLES_Y");
	
	lua_pushinteger(L, gGameUBOptions.SectorLaunchMisslesZ);
	lua_setglobal(L, "UB_iniSECTOR_LAUNCH_MISSLES_Z");
	
	
	lua_pushinteger(L, gGameUBOptions.SectorFanX);
	lua_setglobal(L, "UB_iniPOWERGEN_SECTOR_X");
	
	lua_pushinteger(L, gGameUBOptions.SectorFanY);
	lua_setglobal(L, "UB_iniPOWERGEN_SECTOR_Y");
	
	lua_pushinteger(L, gGameUBOptions.SectorFanZ);
	lua_setglobal(L, "UB_iniPOWERGEN_SECTOR_Z");
	

	lua_pushinteger(L, gGameUBOptions.SectorDoorInTunnelX);
	lua_setglobal(L, "UB_iniSECTOR_DOOR_IN_TUNNEL_X");
	
	lua_pushinteger(L, gGameUBOptions.SectorDoorInTunnelY);
	lua_setglobal(L, "UB_iniSECTOR_DOOR_IN_TUNNEL_Y");
	
	lua_pushinteger(L, gGameUBOptions.SectorDoorInTunnelZ);
	lua_setglobal(L, "UB_SECTOR_DOOR_IN_TUNNEL_Z");

	
	
	
#endif
	
	//Mod Setting.ini

	lua_pushinteger(L, gModSettings.ubHideoutSectorX);
	lua_setglobal(L, "iniHIDEOUT_SECTOR_X");
	
	lua_pushinteger(L, gModSettings.ubHideoutSectorY);
	lua_setglobal(L, "iniHIDEOUT_SECTOR_Y");
	
	lua_pushinteger(L, gModSettings.ubHideoutSectorZ);
	lua_setglobal(L, "iniHIDEOUT_SECTOR_Z");
	
	lua_pushinteger(L, gModSettings.iHideoutExitGrid);
	lua_setglobal(L, "iniHIDEOUT_EXIT");
	
	lua_pushinteger(L, gModSettings.ubHideoutSurfaceX);
	lua_setglobal(L, "iniHIDEOUT_SURFACE_X");
	
	lua_pushinteger(L, gModSettings.ubHideoutSurfaceY);
	lua_setglobal(L, "iniHIDEOUT_SURFACE_Y");
	
	lua_pushinteger(L, gModSettings.ubHideoutSurfaceZ);
	lua_setglobal(L, "iniHIDEOUT_SURFACE_Z");
	
	lua_pushinteger(L, gModSettings.iHideoutEntryGrid);
	lua_setglobal(L, "iniHIDEOUT_ENTRY");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[0]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_1");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[1]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_2");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[2]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_3");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[3]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_4");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[4]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_5");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[5]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_6");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[6]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_7");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[7]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_8");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[8]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_9");
	
	lua_pushinteger(L, gModSettings.iBasementEntry[9]);
	lua_setglobal(L, "iniBASEMENT_ENTRY_10");

	lua_pushinteger(L, gModSettings.iBasementExit[0]);
	lua_setglobal(L, "iniBASEMENT_EXIT_1");
	
	lua_pushinteger(L, gModSettings.iBasementExit[1]);
	lua_setglobal(L, "iniBASEMENT_EXIT_2");
	
	lua_pushinteger(L, gModSettings.iBasementExit[2]);
	lua_setglobal(L, "iniBASEMENT_EXIT_3");
	
	lua_pushinteger(L, gModSettings.iBasementExit[3]);
	lua_setglobal(L, "iniBASEMENT_EXIT_4");
	
	lua_pushinteger(L, gModSettings.iBasementExit[4]);
	lua_setglobal(L, "iniBASEMENT_EXIT_5");
	
	lua_pushinteger(L, gModSettings.iBasementExit[5]);
	lua_setglobal(L, "iniBASEMENT_EXIT_6");
	
	lua_pushinteger(L, gModSettings.iBasementExit[6]);
	lua_setglobal(L, "iniBASEMENT_EXIT_7");
	
	lua_pushinteger(L, gModSettings.iBasementExit[7]);
	lua_setglobal(L, "iniBASEMENT_EXIT_8");
	
	lua_pushinteger(L, gModSettings.iBasementExit[8]);
	lua_setglobal(L, "iniBASEMENT_EXIT_9");
	
	lua_pushinteger(L, gModSettings.iBasementExit[9]);
	lua_setglobal(L, "iniBASEMENT_EXIT_10");
	
	lua_pushinteger(L, gModSettings.iBasementExit[10]);
	lua_setglobal(L, "iniBASEMENT_EXIT_11");
	
	lua_pushinteger(L, gModSettings.iBasementExit[11]);
	lua_setglobal(L, "iniBASEMENT_EXIT_12");
	
	// Rebel Hideout
	lua_pushinteger(L, gModSettings.iFinalCrateGrid);
	lua_setglobal(L, "iniFINAL_CRATE_GRID");
	
	lua_pushinteger(L, gModSettings.usCrateTileDef);
	lua_setglobal(L, "iniCRATE_TILE_DEF");
	
	lua_pushinteger(L, gModSettings.usTrapdoorTileDef);
	lua_setglobal(L, "iniTRAPDOOR_TILE_DEF");

	// San Mona
	lua_pushinteger(L, gModSettings.usPornShopRoomHans);
	lua_setglobal(L, "iniPORN_SHOP_ROOM_HANS");
	
	lua_pushinteger(L, gModSettings.iHansGridNo);
	lua_setglobal(L, "iniHANS_POSTION");
	
	lua_pushinteger(L, gModSettings.usPornShopRoomBrenda);
	lua_setglobal(L, "iniPORN_SHOP_ROOM_BRENDA");
	
	lua_pushinteger(L, gModSettings.usPornShopRoomTony);
	lua_setglobal(L, "iniPORN_SHOP_ROOM_TONY");
	
	//San Mona
	lua_pushinteger(L, gModSettings.usLeatherShop);
	lua_setglobal(L, "iniANGELS_LEATHERSHOP");
	
	lua_pushinteger(L, gModSettings.iBambiDoorGridNo);
	lua_setglobal(L, "iniDOOR_TO_BAMBIS_ROOM");
	
	lua_pushinteger(L, gModSettings.iCarlaDoorGridNo);
	lua_setglobal(L, "iniDOOR_TO_CARLAS_ROOM");
	
	lua_pushinteger(L, gModSettings.iCindyDoorGridNo);
	lua_setglobal(L, "iniDOOR_TO_CINDYS_ROOM");
	
	lua_pushinteger(L, gModSettings.iMariaDoorGridNo);
	lua_setglobal(L, "iniDOOR_TO_MARIAS_ROOM");

	//San Mona
	lua_pushinteger(L, gModSettings.usBrothelRoomRangeStart);
	lua_setglobal(L, "iniFIRST_ROOM_IN_BROTHEL");
	
	lua_pushinteger(L, gModSettings.usBrothelRoomRangeEnd);
	lua_setglobal(L, "iniLAST_ROOM_IN_BROTHEL");
	
	lua_pushinteger(L, gModSettings.usBrothelGuardRoom);
	lua_setglobal(L, "iniBROTHEL_GUARD_ROOM");
	
	//San Mona
	lua_pushinteger(L, gModSettings.iBrothelDoor1);
	lua_setglobal(L, "iniBROTHEL_DOOR_1");

	lua_pushinteger(L, gModSettings.iBrothelDoor2);
	lua_setglobal(L, "iniBROTHEL_DOOR_2");
	
	lua_pushinteger(L, gModSettings.iBrothelDoor3);
	lua_setglobal(L, "iniBROTHEL_DOOR_3");

	lua_pushinteger(L, gModSettings.ubKingpinMoneySectorX);
	lua_setglobal(L, "iniKINGPIN_MONEY_SECTOR_X");
	
	lua_pushinteger(L, gModSettings.ubKingpinMoneySectorY);
	lua_setglobal(L, "iniKINGPIN_MONEY_SECTOR_Y");
	
	lua_pushinteger(L, gModSettings.ubKingpinMoneySectorZ);
	lua_setglobal(L, "iniKINGPIN_MONEY_SECTOR_Z");

	//Gear Dropoff
	lua_pushinteger(L, gModSettings.ubOmertaDropOffX);
	lua_setglobal(L, "iniOMERTA_DROPOFF_X");
	
	lua_pushinteger(L, gModSettings.ubOmertaDropOffY);
	lua_setglobal(L, "iniOMERTA_DROPOFF_Y");
	
	lua_pushinteger(L, gModSettings.ubOmertaDropOffZ);
	lua_setglobal(L, "iniOMERTA_DROPOFF_Z");
	
	lua_pushinteger(L, gModSettings.iOmertaDropOff);
	lua_setglobal(L, "iniOMERTA_DROPOFF_GRIDNO");
	
	//Alma
	lua_pushinteger(L, gModSettings.sRocketRifleGridNo);
	lua_setglobal(L, "iniROCKETRIFLE_POSITION");

	//Hospital Sector
	lua_pushinteger(L, gModSettings.ubHospitalSectorX);
	lua_setglobal(L, "iniHOSPITAL_SECTOR_X");
	
	lua_pushinteger(L, gModSettings.ubHospitalSectorY);
	lua_setglobal(L, "iniHOSPITAL_SECTOR_Y");
	
	lua_pushinteger(L, gModSettings.ubHospitalSectorZ);
	lua_setglobal(L, "iniHOSPITAL_SECTOR_Z");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector1X);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_1_X");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector1Y);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_1_Y");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector2X);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_2_X");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector2Y);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_2_Y");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector3X);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_3_X");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector3Y);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_3_Y");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector4X);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_4_X");

	lua_pushinteger(L, gModSettings.ubMeanwhileAddMadlabSector4Y);
	lua_setglobal(L, "iniADD_MADLAB_SECTOR_4_Y");
	
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
	
	lua_pushinteger(L, guiDay);
	lua_setglobal(L, "guiDay");
	
	lua_pushinteger(L, guiHour);
	lua_setglobal(L, "guiHour");
	
	lua_pushinteger(L, guiMin);
	lua_setglobal(L, "guiMin");
	
	lua_pushinteger(L, guiCurrentScreen);
	lua_setglobal(L, "guiCurrentScreen");

#ifdef JA2UB
	//old
	lua_pushboolean(L, gGameUBOptions.fTexAndJohn);
	lua_setglobal(L, "enabledJohnAndTex");		
	
	lua_pushboolean(L, gGameUBOptions.fRandomManuelText);
	lua_setglobal(L, "RandomManuelText");	
	
	//new
	lua_pushboolean(L, gGameUBOptions.fTexAndJohn);
	lua_setglobal(L, "UB_enabledJohnAndTex");		
	
	lua_pushboolean(L, gGameUBOptions.fRandomManuelText);
	lua_setglobal(L, "UB_RandomManuelText");	
#endif
	
#ifdef JA2UB
	//Old
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
	
	lua_pushinteger(L, TEX_MOVIE_HAVE_HONDA);
	lua_setglobal(L, "itemTEX_MOVIE_HAVE_HONDA");
	
	lua_pushinteger(L, LAPTOP_TRANSMITTER);
	lua_setglobal(L, "itemLAPTOP_TRANSMITTER");
	
	lua_pushinteger(L, CHE_GUEVARA_CANTEEN);
	lua_setglobal(L, "itemCHE_GUEVARA_CANTEEN");
	
	lua_pushinteger(L, MERC_WRISTWATCH);
	lua_setglobal(L, "itemMERC_WRISTWATCH");
	
	lua_pushinteger(L, SAM_GARVER_COMBAT_KNIFE);
	lua_setglobal(L, "itemSAM_GARVER_COMBAT_KNIFE");
	
	lua_pushinteger(L, MERC_UMBRELLA_OLD);
	lua_setglobal(L, "itemMERC_UMBRELLA_OLD");
	
	lua_pushinteger(L, MORRIS_INSTRUCTION_NOTE);
	lua_setglobal(L, "itemMORRIS_INSTRUCTION_NOTE");
	
	//New
	lua_pushinteger(L, BARRETT_UB);
	lua_setglobal(L, "UB_itemBARRETT");	

	lua_pushinteger(L, CALICO_960_UB);
	lua_setglobal(L, "UB_itemCALICO_960");	
	
	lua_pushinteger(L, PSG1_UB);
	lua_setglobal(L, "UB_itemPSG1");	
	
	lua_pushinteger(L, L85_UB);
	lua_setglobal(L, "UB_itemL85");
	
	lua_pushinteger(L, TAR21_UB);
	lua_setglobal(L, "UB_itemTAR21");

	lua_pushinteger(L, VAL_SILENT_UB);
	lua_setglobal(L, "UB_itemVAL_SILENT");

	lua_pushinteger(L, MICRO_UZI_UB);
	lua_setglobal(L, "UB_itemMICRO_UZI");
	
	lua_pushinteger(L, CALICO_950_UB);
	lua_setglobal(L, "UB_itemCALICO_950");
	
	lua_pushinteger(L, CALICO_900_UB);
	lua_setglobal(L, "UB_itemCALICO_900");
	
	lua_pushinteger(L, CLIP_CANNON_BALL);
	lua_setglobal(L, "UB_itemCLIP_CANNON_BALL");
	
	lua_pushinteger(L, MERC_UMBRELLA);
	lua_setglobal(L, "UB_itemMERC_UMBRELLA");
	
	lua_pushinteger(L, HAND_CANNON);
	lua_setglobal(L, "UB_itemHAND_CANNON");
	
	lua_pushinteger(L, HARTFORD_6_SHOOTER);
	lua_setglobal(L, "UB_itemHARTFORD_6_SHOOTER");
	
	lua_pushinteger(L, TEX_MOVIE_ATTACK_CLYDESDALES);
	lua_setglobal(L, "UB_itemTEX_MOVIE_ATTACK_CLYDESDALES");
	
	lua_pushinteger(L, TEX_MOVIE_WILD_EAST);
	lua_setglobal(L, "UB_itemTEX_MOVIE_WILD_EAST");
	
	lua_pushinteger(L, TEX_MOVIE_HAVE_HONDA);
	lua_setglobal(L, "UB_itemTEX_MOVIE_HAVE_HONDA");
	
	lua_pushinteger(L, LAPTOP_TRANSMITTER);
	lua_setglobal(L, "UB_itemLAPTOP_TRANSMITTER");
	
	lua_pushinteger(L, CHE_GUEVARA_CANTEEN);
	lua_setglobal(L, "UB_itemCHE_GUEVARA_CANTEEN");
	
	lua_pushinteger(L, MERC_WRISTWATCH);
	lua_setglobal(L, "UB_itemMERC_WRISTWATCH");
	
	lua_pushinteger(L, SAM_GARVER_COMBAT_KNIFE);
	lua_setglobal(L, "UB_itemSAM_GARVER_COMBAT_KNIFE");
	
	lua_pushinteger(L, MERC_UMBRELLA_OLD);
	lua_setglobal(L, "UB_itemMERC_UMBRELLA_OLD");
	
	lua_pushinteger(L, MORRIS_INSTRUCTION_NOTE);
	lua_setglobal(L, "UB_itemMORRIS_INSTRUCTION_NOTE");
#endif
}