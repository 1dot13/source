#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "Language Defines.h"
	#include "HelpScreen.h"
	#include "GameSettings.h"
#else
	#include "sgp.h"
	#include "jascreens.h"
	#include "laptop.h"
	#include "worlddef.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "email.h"
	#include "Game Clock.h"
	#include "soldier profile.h"
	#include "strategicmap.h"
	#include "game init.h"
	#include "animation data.h"
	#include "finances.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "strategic.h"
	#include "history.h"
	#include "merc entering.h"
	#include "Squads.h"
	#include "Campaign Init.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "gameloop.h"
	#include "Random.h"
	#include "Map Screen Interface.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "Message.h"
	#include "Game Event Hook.h"
	#include "Strategic Movement.h"
	#include "Creature Spreading.h"
	#include "Quests.h"
	#include "Strategic AI.h"
	#include "Laptopsave.h"
	#include "AimMembers.h"
	#include "dialogue control.h"
	#include "npc.h"
	#include "GameSettings.h"
	#include "interface dialogue.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Helicopter.h"
	#include "Vehicles.h"
	#include "Map Screen Interface Map.h"
	#include "PreBattle Interface.h"
	#include "bobbyr.h"
	#include "helpscreen.h"
	#include "Air Raid.h"
	#include "Interface.h"
	#include "cheats.h"
	#include "Interface Panels.h"
	// HEADROCK HAM 3.6: Include for adding facility debt reset
	#include "Facilities.h"
	// HEADROCK HAM 4: Include for initializing Manual Restrictions
	#include "MilitiaSquads.h"
	#include "Map Screen Interface Map Inventory.h"//dnl ch51 081009
	#include "CampaignStats.h"						// added by Flugente
	#include "PMC.h"								// added by Flugente
	#include "ASD.h"								// added by Flugente
	#include "MiniEvents.h"
	#include "Rebel Command.h"
#endif

#include "Vehicles.h"
#include "text.h"
#include "connect.h"
#include "XML.h"
#include "mercs.h"
#include "aim.h"
#include "Map Screen Interface.h"
#ifdef JA2UB
#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#endif

#include "LuaInitNPCs.h"
#include "Luaglobal.h"
#include "SaveLoadScreen.h"

#ifdef JA113DEMO
#include "Merc Contract.h"
#include "Soldier Add.h"
#include "Map Screen Interface Bottom.h"
#endif

#include "GameInitOptionsScreen.h"

#include "PostalService.h"
extern CPostalService gPostalService;
extern void initMapViewAndBorderCoordinates(void);

class OBJECTTYPE;
class SOLDIERTYPE;

// Temp function
void QuickSetupOfMercProfileItems( UINT32 uiCount, UINT8 ubProfileIndex );
BOOLEAN QuickGameMemberHireMerc( UINT8 ubCurrentSoldier );
extern UINT32 guiExitScreen;
extern UINT32 uiMeanWhileFlags;
extern BOOLEAN gfGamePaused;

extern UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );

UINT8 gubCheatLevel = STARTING_CHEAT_LEVEL;

UINT8			gubScreenCount=0;

#ifdef JA2UB
void InitCustomStrategicLayer ( void )
{
	LetLuaGameInit(2); //load custom InitStrategicLayer
}

#endif

void InitNPCs( void )
{		

#ifdef LUA_GAME_INIT_NPCS
	LetLuaGameInit(1);
#else

	MERCPROFILESTRUCT * pProfile;

	// add the pilot at a random location!
	pProfile = &(gMercProfiles[ SKYRIDER ]);

	if (!is_networked)
	{
		switch( Random( 4 ) )
		{
			case 0:
				pProfile->sSectorX = 15;
				pProfile->sSectorY = MAP_ROW_B;
				pProfile->bSectorZ = 0;
				break;
			case 1:
				pProfile->sSectorX = 14;
				pProfile->sSectorY = MAP_ROW_E;
				pProfile->bSectorZ = 0;
				break;
			case 2:
				pProfile->sSectorX = 12;
				pProfile->sSectorY = MAP_ROW_D;
				pProfile->bSectorZ = 0;
				break;
			case 3:
				pProfile->sSectorX = 16;
				pProfile->sSectorY = MAP_ROW_C;
				pProfile->bSectorZ = 0;
				break;
			default:
				AssertMsg(false, "Skyrider was not set up properly");
		}
	}
	// WANNE - MP: Set Skyrider in a MP game hardcoded to 15/B
	else
	{
		pProfile->sSectorX = 15;
		pProfile->sSectorY = MAP_ROW_B;
		pProfile->bSectorZ = 0;
	}

	#ifdef JA2TESTVERSION
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Skyrider in %c %d", 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
	#endif

	// use alternate map, with Skyrider's shack, in this sector
	SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;

	// set up Madlab's secret lab (he'll be added when the meanwhile scene occurs)

	if (!is_networked)
	{
		switch( Random( 4 ) )
		{
			case 0:
				// use alternate map in this sector
				SectorInfo[ SECTOR( 7, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				break;
			case 1:
				SectorInfo[ SECTOR( 16, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				break;
			case 2:
				SectorInfo[ SECTOR( 11, MAP_ROW_I ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				break;
			case 3:
				SectorInfo[ SECTOR( 4, MAP_ROW_E ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				break;
		}
	}
	// WANNE - MP: Set Madlab in a MP game hardcoded to 7/H
	else
	{
		SectorInfo[ SECTOR( 7, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
	}

	// add Micky in random location

	pProfile = &(gMercProfiles[MICKY]);

	if (!is_networked)
	{
		switch( Random( 5 ) )
		{
			case 0:
				pProfile->sSectorX = 9;
				pProfile->sSectorY = MAP_ROW_G;
				pProfile->bSectorZ = 0;
				break;
			case 1:
				pProfile->sSectorX = 13;
				pProfile->sSectorY = MAP_ROW_D;
				pProfile->bSectorZ = 0;
				break;
			case 2:
				pProfile->sSectorX = 5;
				pProfile->sSectorY = MAP_ROW_C;
				pProfile->bSectorZ = 0;
				break;
			case 3:
				pProfile->sSectorX = 2;
				pProfile->sSectorY = MAP_ROW_H;
				pProfile->bSectorZ = 0;
				break;
			case 4:
				pProfile->sSectorX = 6;
				pProfile->sSectorY = MAP_ROW_C;
				pProfile->bSectorZ = 0;
				break;
		}
	}
	// WANNE - MP: Set Micky in a MP game hardcoded to 9/G
	else
	{
		pProfile->sSectorX = 9;
		pProfile->sSectorY = MAP_ROW_G;
		pProfile->bSectorZ = 0;
	}

	#ifdef JA2TESTVERSION
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s in %c %d", pProfile->zNickname, 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
	#endif

	// use alternate map in this sector
	//SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;

	gfPlayerTeamSawJoey = FALSE;


	if ( gGameOptions.ubGameStyle == STYLE_SCIFI && gGameExternalOptions.fEnableCrepitus )
	{
		// add Bob
		pProfile = &(gMercProfiles[BOB]);
		pProfile->sSectorX = 8;
		pProfile->sSectorY = MAP_ROW_F;
		pProfile->bSectorZ = 0;

		// add Gabby in random location
		pProfile = &(gMercProfiles[GABBY]);
		switch( Random( 2 ) )
		{
			case 0:
				pProfile->sSectorX = 11;
				pProfile->sSectorY = MAP_ROW_H;
				pProfile->bSectorZ = 0;
				break;
			case 1:
				pProfile->sSectorX = 4;
				pProfile->sSectorY = MAP_ROW_I;
				pProfile->bSectorZ = 0;
				break;
		}

		#ifdef JA2TESTVERSION
			ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s in %c %d", pProfile->zNickname, 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
		#endif

		// use alternate map in this sector
		SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
	}
	else
	{ //not scifi, so use alternate map in Tixa's b1 level that doesn't have the stairs going down to the caves.
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( 9, 10, 1 ); //j9_b1
		if( pSector )
		{
			pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		}
	}

	// init hospital variables
	giHospitalTempBalance = 0;
	giHospitalRefund = 0;
	gbHospitalPriceModifier = 0;

	// set up Devin so he will be placed ASAP
	gMercProfiles[ DEVIN ].bNPCData = 3;
	
#endif

}

void InitBloodCatSectors()
{
	///////////////////////////////////////////////////
	// HEADROCK HAM 3.6: Bloodcat Placement Externalization
	//
	// It is now possible for the user/modder to set the maximum number of bloodcats that may appear in the sector.
	// We can now place bloodcats anywhere we'd like (aboveground), stretching the limits of maps without disturbing
	// the game. The placements defined here are still the absolute MAXIMUM, so if the map has more placements available
	// they will be ignored.
	// Please note that if the mapfile has LESS placements than indicated here, then the game will reduce the number of
	// placements automatically to fit the MAP, not the XML.

	UINT16 x = 0;
	UINT8 PlacementType = 0;
	UINT8 ubMaxBloodcats = 0;
	UINT8 ubDifficulty = gGameOptions.ubDifficultyLevel-1; // This way, novice=0.
	INT16 ubLairSectorId = -1; // Location of ONE lair sector (the first one found)
	
	// Clean all bloodcat data.
	for( x = 0; x < 256; x++ )
	{
		SectorInfo[ x ].bBloodCats = 0;
		SectorInfo[ x ].bBloodCatPlacements = 0;
	}

	// Begin loop for every sector.
	for (x = 0; x < 256; x++)
	{
		// Set "maximum placements" value to whatever is defined in the XML file for this difficulty level.
		SectorInfo[ x ].bBloodCatPlacements = gBloodcatPlacements[x][ubDifficulty].ubMaxBloodcats;

		PlacementType = gBloodcatPlacements[x][0].PlacementType;

		// Record the first sector you can find which is defined as a LAIR sector. Only one sector can serve
		// this purpose, because the quests rely on it.
		if (PlacementType == BLOODCAT_PLACEMENT_LAIR && ubLairSectorId == -1)
		{
			ubLairSectorId = x;
		}

		if (PlacementType != 0)
		{
			// Sector is a lair or static bloodcat sector. Fill it up with actual bloodcats please.
			SectorInfo[ x ].bBloodCats = gBloodcatPlacements[x][ubDifficulty].ubMaxBloodcats;
			// "Ambush" sectors (PlacementType 0) do not have bloodcats in them until triggered.
		}
	}

	if (ubLairSectorId >= 0)
	{
		gubBloodcatLairSectorId = (UINT8)ubLairSectorId;
	}
	else
	{
		SGP_THROW(L"At least one Bloodcat Lair must be defined in BloodcatPlacements.XML!");
	}

	/*
		

	INT32 i;
	//Hard coded table of bloodcat populations.	We don't have
	//access to the real population (if different) until we physically
	//load the map.	If the real population is different, then an error
	//will be reported.
	for( i = 0; i < 255; i++ )
	{
		SectorInfo[ i ].bBloodCats = -1;
	}
	SectorInfo[ SEC_A15	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_B4	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_B16	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_C3	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_C8	].bBloodCatPlacements = 13;
	SectorInfo[ SEC_C11	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_D4	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_D9	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_E11	].bBloodCatPlacements = 10;
	SectorInfo[ SEC_E13	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_F3	].bBloodCatPlacements = 13;
	SectorInfo[ SEC_F5	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_F7	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_F12	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_F14	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_F15	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_G6	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_G10	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_G12	].bBloodCatPlacements = 11;
	SectorInfo[ SEC_H5	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_I4	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_I15	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_J6	].bBloodCatPlacements = 11;
	SectorInfo[ SEC_K3	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_K6	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_K10	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_K14	].bBloodCatPlacements = 14;

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY: //50%
			SectorInfo[ SEC_I16	].bBloodCatPlacements = 14;
			SectorInfo[ SEC_I16	].bBloodCats = 14;
			SectorInfo[ SEC_N5	].bBloodCatPlacements = 8;
			SectorInfo[ SEC_N5	].bBloodCats = 8;
			break;
		case DIF_LEVEL_MEDIUM: //75%
			SectorInfo[ SEC_I16	].bBloodCatPlacements = 19;
			SectorInfo[ SEC_I16	].bBloodCats = 19;
			SectorInfo[ SEC_N5	].bBloodCatPlacements = 10;
			SectorInfo[ SEC_N5	].bBloodCats = 10;
			break;
		case DIF_LEVEL_HARD: //100%
			SectorInfo[ SEC_I16	].bBloodCatPlacements = 26;
			SectorInfo[ SEC_I16	].bBloodCats = 26;
			SectorInfo[ SEC_N5	].bBloodCatPlacements = 12;
			SectorInfo[ SEC_N5	].bBloodCats = 12;
			break;
		case DIF_LEVEL_INSANE: //150%
			SectorInfo[ SEC_I16	].bBloodCatPlacements = 39;
			SectorInfo[ SEC_I16	].bBloodCats = 39;
			SectorInfo[ SEC_N5	].bBloodCatPlacements = 18;
			SectorInfo[ SEC_N5	].bBloodCats = 18;
			break;
	}*/
}


void InitStrategicLayer( void )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitStrategicLayer");

	// Clear starategic layer!
	SetupNewStrategicGame();
	InitQuestEngine();

	//Setup a new campaign via the enemy perspective.
	InitNewCampaign();
	// Init Squad Lists
	InitSquads();
	// Init vehicles
	InitAllVehicles( );
	
	#ifdef JA2UB
	InitCustomStrategicLayer ( );
	#endif

#ifdef JA2UB	
	//Ja25 UB
	InitJerryQuotes();	
	if ( gGameUBOptions.JerryQuotes == TRUE )
	{
		HandleJerryMiloQuotes( TRUE ); //AA
	}
	
	InitJa25StrategicAi( );
#endif

	
#ifdef JA2UB
	////if ( gGameUBOptions.InitTownLoyalty_UB == TRUE )
		//InitTownLoyalty(); //Ja25 no loyalty
#else
		// init town loyalty
		InitTownLoyalty(); //Ja25 no loyalty
#endif
	// init the mine management system
	InitializeMines();
	// initialize map screen flags
	InitMapScreenFlags();
	// initialize NPCs, select alternate maps, etc
	InitNPCs();
	// init Skyrider and his helicopter
	InitializeHelicopter();
	//Clear out the vehicle list
	ClearOutVehicleList();

	InitBloodCatSectors();

	InitializeSAMSites();

	// make Orta, Tixa, SAM sites not found
	InitMapSecrets();

	// HEADROCK HAM 3.6: Initialize facilities.
	InitFacilities();
	
	// free up any leave list arrays that were left allocated
	ShutDownLeaveList( );

	// re-set up leave list arrays for dismissed mercs
	InitLeaveList( );

	// Flugente: set up VIP locations
	InitVIPSectors();

#ifdef JA2UB
	LuaInitStrategicLayer(0); //JA25 UB InitStrategicLayer.lua 
#endif

	// reset time compression mode to X0 (this will also pause it)
	SetGameTimeCompressionLevel( TIME_COMPRESS_X0 );

	// select A9 Omerta as the initial selected sector
	// HEADROCK HAM 3.5: Actually, this is where we set the starting sector based on external variables.
	ChangeSelectedMapSector( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );

	// Reset these flags or mapscreen could be disabled and cause major headache.
	fDisableDueToBattleRoster = FALSE;
	fDisableMapInterfaceDueToBattle = FALSE;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitStrategicLayer done");
}

void ShutdownStrategicLayer()
{
	DeleteAllStrategicEvents();
	RemoveAllGroups();
	TrashUndergroundSectorInfo();

#ifdef JA2UB
//Ja25 No creatures
//Ja25 No strategic ai
#else
	DeleteCreatureDirectives(); 

	KillStrategicAI();
	
#endif
	DeleteCreatureDirectives();
	KillStrategicAI();
	ClearTacticalMessageQueue();
}

extern void DebugQuestInfo(STR szOutput);

BOOLEAN InitNewGame( BOOLEAN fReset )
{	
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame");

	if( fReset )
	{
		gubScreenCount = 0;
		return( TRUE );
	}
	
	if( gubScreenCount == 0 )
	{	
		IniLuaGlobal();//Lua
	}
	
	//reset autosave
	AutoSaveToSlot[0] = FALSE;
	AutoSaveToSlot[1] = FALSE;
	AutoSaveToSlot[2] = FALSE;
	AutoSaveToSlot[3] = FALSE;
	AutoSaveToSlot[4] = FALSE;
	
	gGameExternalOptions.gfAllowReinforcements = zDiffSetting[gGameOptions.ubDifficultyLevel].bAllowReinforcements;
	
#ifdef JA2UB
//Ja25 no meanwhiles
#else
	// reset meanwhile flags
	uiMeanWhileFlags = 0;
#endif

#ifdef JA2UB
fFirstTimeInMapScreen = TRUE;
#endif


	// Reset the selected soldier
	gusSelectedSoldier = NOBODY;

	// CHRISL: Init inventory
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: set initial inventory coords");
	if( gubScreenCount == 0 )
	{
		// Have to initialize map UI Coordinates, because inventory panel layout location depends on them.
		initMapViewAndBorderCoordinates();
		if((UsingNewInventorySystem() == true))
		{
			InitNewInventorySystem();
			InitializeSMPanelCoordsNew();
			InitializeInvPanelCoordsNew();
		}
		else
		{
			InitOldInventorySystem();
			InitializeSMPanelCoordsOld();
			InitializeInvPanelCoordsOld();
		}
	}

	if(gGameExternalOptions.fEnableInventoryPoolQ)//dnl ch51 081009
		MemFreeInventoryPoolQ();

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: loading merc profiles");
	if( gubScreenCount == 0 )
	{
		if( !LoadMercProfiles() )
			return(FALSE);
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: init arms dealers");
	
	//Initialize the Arms Dealers and Bobby Rays inventory
	if( gubScreenCount == 0 )
	{
		//Init all the arms dealers inventory
		InitAllArmsDealers();
#ifdef JA2UB		
		if ( gGameUBOptions.fBobbyRSite == TRUE )
		InitBobbyRayInventory();  //Ja25 UB
#else
		InitBobbyRayInventory();  
#endif
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: clearing messages");
	
	// clear tactical
	ClearTacticalMessageQueue( );

	// clear mapscreen messages
	if(!is_networked)
		FreeGlobalMessageList(); //hayden

	// Do not hide any person names
	if ( gGameExternalOptions.fIndividualHiddenPersonNames == FALSE )
	{
		for (int i=0;i<500;i++)
		{
			zHiddenNames[i].Hidden = FALSE;
		}	
	}
	else
	{
		// Hide individual person names depending on HiddenNames.xml
		LoadHiddenNames();
	}
	
	// ------------------------
	// Camo Faces
	// ------------------------
	if (gGameExternalOptions.fShowCamouflageFaces == TRUE )
	{		
		for (int i=0;i<NUM_PROFILES;++i)
		{
			gCamoFace[i].gCamoface = FALSE;
			gCamoFace[i].gUrbanCamoface = FALSE;
			gCamoFace[i].gDesertCamoface = FALSE;
			gCamoFace[i].gSnowCamoface = FALSE;
		}
		
		if ( !gGameOptions.fNewTraitSystem ) // SANDRO - only with old traits, we get the camo instantly
		{
			for(int uiLoop=0; uiLoop< NUM_PROFILES; uiLoop++)
			{
				if ( ProfileHasSkillTrait( uiLoop, CAMOUFLAGED_OT ) > 0 )
				{
					gCamoFace[uiLoop].gCamoface = TRUE;
				}									
			}
		}
	}
		
	// ------------------------
	// Reset mercs profiles
	// ------------------------
	UINT8 i;

	for(i=0; i<NUM_PROFILES; i++)
	{
		gConditionsForMercAvailability[i].uiIndex = gConditionsForMercAvailabilityTemp[i].uiIndex;
		gConditionsForMercAvailability[i].ProfilId = gConditionsForMercAvailabilityTemp[i].ProfilId;
		gConditionsForMercAvailability[i].usMoneyPaid = gConditionsForMercAvailabilityTemp[i].usMoneyPaid;
		gConditionsForMercAvailability[i].usDay = gConditionsForMercAvailabilityTemp[i].usDay;
		gConditionsForMercAvailability[i].ubMercArrayID = gConditionsForMercAvailabilityTemp[i].ubMercArrayID;
		gConditionsForMercAvailability[i].NewMercsAvailable = gConditionsForMercAvailabilityTemp[i].NewMercsAvailable;
		gConditionsForMercAvailability[i].StartMercsAvailable = gConditionsForMercAvailabilityTemp[i].StartMercsAvailable;
		gConditionsForMercAvailability[i].Drunk = gConditionsForMercAvailabilityTemp[i].Drunk;
		gConditionsForMercAvailability[i].uiAlternateIndex = gConditionsForMercAvailabilityTemp[i].uiAlternateIndex;
		gConditionsForMercAvailability[i].MercBio = gConditionsForMercAvailabilityTemp[i].MercBio;		
	}	
	
	for(i=0; i<NUM_PROFILES; i++)
	{
		gAimAvailability[i].uiIndex = gAimAvailabilityTemp[i].uiIndex;
		gAimAvailability[i].ProfilId = gAimAvailabilityTemp[i].ProfilId;
		gAimAvailability[i].ubAimArrayID = gAimAvailabilityTemp[i].ubAimArrayID;
		gAimAvailability[i].AimBio = gAimAvailabilityTemp[i].AimBio;
	}
	
#ifdef ENABLE_BRIEFINGROOM
	BackupBRandEncyclopedia ( gBriefingRoomDataBackup, gBriefingRoomData, 0);
#endif //ENABLE_BRIEFINGROOM

	// IF our first time, go into laptop!
	if ( gubScreenCount == 0 )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: first time: init laptop");
		//Init the laptop here
		InitLaptopAndLaptopScreens();

		// Flugente campaign stats
		gCampaignStats.clear();

		// Flugente: PMC
		GameInitPMC();

		InitStrategicLayer();

		// Set new game flag
		SetLaptopNewGameFlag( );

		// this is for the "mercs climbing down from a rope" animation, NOT Skyrider!!
		ResetHeliSeats( );

#ifdef LUA_GAME_INIT_NEW_GAME
		
		LetLuaGameInit(0);

		#ifdef JA2UB
		InitCustomStrategicLayer ( );
		#endif
#else
		#ifdef JA2UB

		#else
		INT32		iStartingCash;

		// Setup two new messages!
		AddPreReadEmail(OLD_ENRICO_1,OLD_ENRICO_1_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddPreReadEmail(OLD_ENRICO_2,OLD_ENRICO_2_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddPreReadEmail(RIS_REPORT,RIS_REPORT_LENGTH,RIS_EMAIL,  GetWorldTotalMin() );
		AddPreReadEmail(OLD_ENRICO_3,OLD_ENRICO_3_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddEmail(IMP_EMAIL_INTRO,IMP_EMAIL_INTRO_LENGTH,CHAR_PROFILE_SITE,  GetWorldTotalMin(), -1, -1);
		//AddEmail(ENRICO_CONGRATS,ENRICO_CONGRATS_LENGTH,MAIL_ENRICO, GetWorldTotalMin() );
		if(gGameExternalOptions.fMercDayOne)
		{
			AddEmail(MERC_INTRO, MERC_INTRO_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin( ), -1, 1 );
		}

		// ATE: Set starting cash....
		iStartingCash = zDiffSetting[gGameOptions.ubDifficultyLevel].iStartingCash;
		
		// Setup initial money
 		AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), iStartingCash );
		#endif
#endif
	
		UINT32	uiDaysTimeMercSiteAvailable = Random( 2 ) + 1;

		// schedule email for message from spec at 7am 3 days in the future
		AddFutureDayStrategicEvent( EVENT_DAY3_ADD_EMAIL_FROM_SPECK, 60*7, 0, uiDaysTimeMercSiteAvailable );
		
		// HEADROCK HAM 3.5: Reset LZ to default arrival
		gsMercArriveSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;
		gsMercArriveSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;

		if(is_networked)
		{
			SetLaptopExitScreen( MAP_SCREEN ); //hayden
			SetPendingNewScreen( MAP_SCREEN );						
		}
		else
		{
			#ifdef JA113DEMO
			
			//QuickSetupOfMercProfileItems( 0,4 ); //Vicki
			//QuickGameMemberHireMerc( 4 );
			
			//QuickSetupOfMercProfileItems( 0,7 ); // Ivan
			//QuickGameMemberHireMerc( 7 );
			
			//QuickSetupOfMercProfileItems( 0,10 ); // Shadow
			//QuickGameMemberHireMerc( 10 );
			
			//QuickSetupOfMercProfileItems( 0,33 ); // 
			//QuickGameMemberHireMerc( 33 );
			
			//QuickSetupOfMercProfileItems( 0,42 ); // 
			//QuickGameMemberHireMerc( 42 );
			
			SetLaptopExitScreen( MAP_SCREEN );
			SetPendingNewScreen( MAP_SCREEN );
			
			#else
			SetLaptopExitScreen( INIT_SCREEN );
			SetPendingNewScreen(LAPTOP_SCREEN);
			#endif
		}
		
		gubScreenCount = 1;

#ifdef JA2UB		
		//ja25 ub
		//Init the initial hweli crash sequence variable
		if ( gGameUBOptions.InGameHeli == FALSE )
		InitializeHeliGridnoAndTime( FALSE );
	
		//If tex is in the game ( John is NOT in the game )
		//if( gJa25SaveStruct.fJohnKulbaIsInGame == FALSE )
		//{
			//make sure Betty offers his videos for sale
			//AddTexsVideosToBettysInventory();
		//}

		InitJerryMiloInfo(); //JA25 UB
#endif

		//Set the fact the game is in progress
		gTacticalStatus.fHasAGameBeenStarted = TRUE;
			
	#ifdef JA113DEMO
	RequestTriggerExitFromMapscreen( MAP_EXIT_TO_TACTICAL );
	#endif	

		InitMiniEvents();

		RebelCommand::Init();

		return( TRUE );
	}

	if ( gubScreenCount == 1 )
	{		
		gubScreenCount = 2;
		return( TRUE );
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame done");
	return( TRUE );
}


BOOLEAN AnyMercsHired( )
{
	INT32 cnt;
	SOLDIERTYPE		*pTeamSoldier;
	INT16				bLastTeamID;

	// Find first guy availible in team
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->bActive )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void QuickStartGame( )
{
	INT32		cnt;
	UINT16	usVal;
	UINT8 ub1 = 0, ub2 = 0;

	for ( cnt = 0; cnt < 3; cnt++ )
	{
		if ( cnt == 0 )
		{
			usVal = (UINT16)Random( 40 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}
		else if ( cnt == 1 )
		{
			do
			{
				usVal = (UINT16)Random( 40 );
			}
			while( usVal != ub1 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}
		else if ( cnt == 2 )
		{
			do
			{
				usVal = (UINT16)Random( 40 );
			}
			while( usVal != ub1 && usVal != ub2 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}

	}
}


// TEMP FUNCTION!
void QuickSetupOfMercProfileItems( UINT32 uiCount, UINT8 ubProfileIndex )
{
	// Quickly give some guys we hire some items

	if ( uiCount == 0 )
	{
		//CreateGun( GLOCK_17, &(pSoldier->inv[ HANDPOS ] ) );
		//gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = HAND_GRENADE;
		//gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		//gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 3;
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = C7;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK1POS ] = CAWS;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK1POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK1POS ] = 1;

		if (gGameOptions.fNewTraitSystem) // SANDRO - traits
			gMercProfiles[ ubProfileIndex ].bSkillTraits[0] = MARTIAL_ARTS_NT;
		else
			gMercProfiles[ ubProfileIndex ].bSkillTraits[0] = MARTIALARTS_OT;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = KEY_2;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK5POS ] = LOCKSMITHKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK5POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK5POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK3POS ] = MEDICKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK4POS ] = SHAPED_CHARGE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK4POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK4POS ] = 1;


		// TEMP!
		// make carman's opinion of us high!
		if (OKToCheckOpinion(ubProfileIndex))
			gMercProfiles[ CARMEN ].bMercOpinion[ ubProfileIndex ] = 25;

	}
	else if ( uiCount == 1 )
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = CAWS;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = KEY_1;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;


	}
	else if ( uiCount == 2 )
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = GLOCK_17;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SECONDHANDPOS ] = 5;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SECONDHANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SECONDHANDPOS ] = 1;


		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK1POS ] = SILENCER;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK1POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK1POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK2POS ] = SNIPERSCOPE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK2POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK2POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = LASERSCOPE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK5POS ] = BIPOD;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK5POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK5POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK6POS ] = LOCKSMITHKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK6POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK6POS ] = 1;

	}
	else
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = (UINT8)Random(30);
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;


	}

	gMercProfiles[ ubProfileIndex ].inv[ HELMETPOS ] = KEVLAR_HELMET;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ HELMETPOS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ HELMETPOS ] = 1;

	gMercProfiles[ ubProfileIndex ].inv[ VESTPOS ] = KEVLAR_VEST;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ VESTPOS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ VESTPOS ] = 1;

	gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK2POS ] = RDX;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK2POS ] = 10;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK2POS ] = 1;

	gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK4POS ] = HAND_GRENADE;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK4POS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK4POS ] = 4;

	// Give special items to some NPCs
	//gMercProfiles[ 78 ].inv[ SMALLPOCK4POS ] = TERRORIST_INFO;
	//gMercProfiles[ 78 ].bInvStatus[ SMALLPOCK4POS ] = 100;
	//gMercProfiles[ 78 ].bInvNumber[ SMALLPOCK4POS ] = 1;

}


BOOLEAN QuickGameMemberHireMerc( UINT8 ubCurrentSoldier )
{
	MERC_HIRE_STRUCT HireMercStruct;
	
	#ifdef JA113DEMO
	INT16		sSoldierID=0;
	#endif

	memset(&HireMercStruct, 0, sizeof(MERC_HIRE_STRUCT));

	HireMercStruct.ubProfileID = ubCurrentSoldier;

	HireMercStruct.sSectorX = gsMercArriveSectorX;
	HireMercStruct.sSectorY = gsMercArriveSectorY;
	HireMercStruct.fUseLandingZoneForArrival = TRUE;

	HireMercStruct.fCopyProfileItemsOver =	TRUE;
	HireMercStruct.ubInsertionCode				= INSERTION_CODE_CHOPPER;

	#ifdef JA113DEMO
	HireMercStruct.iTotalContractLength = 61;
	#else
	HireMercStruct.iTotalContractLength = 7;
	#endif

	//specify when the merc should arrive
	HireMercStruct.uiTimeTillMercArrives = 0;

	//if we succesfully hired the merc
	if( !HireMerc( &HireMercStruct ) )
	{
		return(FALSE);
	}
	
	#ifdef JA113DEMO
	sSoldierID = GetSoldierIDFromMercID( ubCurrentSoldier );
	if( sSoldierID == -1 )
		return( FALSE );
	Menptr[ sSoldierID ].bTypeOfLastContract = CONTRACT_EXTEND_1_WEEK;
	#endif
	

	//add an entry in the finacial page for the hiring of the merc
	AddTransactionToPlayersBook(HIRED_MERC, ubCurrentSoldier, GetWorldTotalMin(), -(INT32) gMercProfiles[ubCurrentSoldier].uiWeeklySalary );

	if( gMercProfiles[ ubCurrentSoldier ].bMedicalDeposit )
	{
		//add an entry in the finacial page for the medical deposit
		AddTransactionToPlayersBook(MEDICAL_DEPOSIT, ubCurrentSoldier, GetWorldTotalMin(), -(gMercProfiles[ubCurrentSoldier].sMedicalDepositAmount) );
	}

	//add an entry in the history page for the hiring of the merc
	AddHistoryToPlayersLog( HISTORY_HIRED_MERC_FROM_AIM, ubCurrentSoldier, GetWorldTotalMin(), -1, -1 );

	return(TRUE);
}




//This function is called when the game is REstarted.	Things that need to be reinited are placed in here
void ReStartingGame()
{
	UINT16	cnt;

	//Pause the game
	gfGamePaused = TRUE;

	//Reset the sectors
	gWorldSectorX = gWorldSectorY = 0;
	gbWorldSectorZ = -1;
	
	//Legion by Jazz
	if (gGameExternalOptions.fShowCamouflageFaces == TRUE )
	{
		//Camo Face
		for (int i=0;i<NUM_PROFILES;++i)
		{
			gCamoFace[i].gCamoface = FALSE;
			gCamoFace[i].gUrbanCamoface = FALSE;
			gCamoFace[i].gDesertCamoface = FALSE;
			gCamoFace[i].gSnowCamoface = FALSE;
		}
		
		if ( !gGameOptions.fNewTraitSystem ) // SANDRO - only with old traits, we get the camo instantly
		{
			for(int uiLoop=0; uiLoop< NUM_PROFILES; uiLoop++)
			{
				if ( ProfileHasSkillTrait( uiLoop, CAMOUFLAGED_OT ) > 0 )
					{
						gCamoFace[uiLoop].gCamoface = TRUE;
					}
//				else if ( gMercProfiles[uiLoop].bSkillTrait == CAMOUFLAGED_URBAN || gMercProfiles[uiLoop].bSkillTrait2 == CAMOUFLAGED_URBAN )
//					{
//						gCamoFace[uiLoop].gUrbanCamoface = TRUE;
//					}			
//				else if ( gMercProfiles[uiLoop].bSkillTrait == CAMOUFLAGED_DESERT || gMercProfiles[uiLoop].bSkillTrait2 == CAMOUFLAGED_DESERT )
//					{
//						gCamoFace[uiLoop].gDesertCamoface = TRUE;
//					}
//				else if ( gMercProfiles[uiLoop].bSkillTrait == CAMOUFLAGED_SNOW || gMercProfiles[uiLoop].bSkillTrait2 == CAMOUFLAGED_SNOW )
//					{
//						gCamoFace[uiLoop].gSnowCamoface = TRUE;
//					}						
			}
		}
		
	}
		
	//Legion by Jazz
	if ( gGameExternalOptions.fIndividualHiddenPersonNames == FALSE )
	{
		for (int i=0;i<500;i++)
		{
			zHiddenNames[i].Hidden = FALSE;
		}
	} 
	else
	{
		//Restart Game and New Game
		LoadHiddenNames();
	}


	// WANNE: By disabling the following line, this should fix the bug, that the main music start again, whenever we click a button in the main menu
	//SoundStopAll( );

	//we are going to restart a game so initialize the variable so we can initialize a new game
	InitNewGame( TRUE );

	//Deletes all the Temp files in the Maps\Temp directory
	InitTacticalSave( TRUE );

	//Loop through all the soldier and delete them all
	for( cnt=0; cnt< TOTAL_SOLDIERS; cnt++)
	{
		TacticalRemoveSoldier( cnt );
	}

	// Re-init overhead...
	InitOverhead( );

	//Reset the email list
	ShutDownEmailList();

	//Reinit the laptopn screen variables
	InitLaptopAndLaptopScreens();
	LaptopScreenInit();

	//Reload the Merc profiles
	LoadMercProfiles( );

	if (gGameExternalOptions.fWriteProfileDataToXML)
	{
		// HEADROCK PROFEX: Create mercprofiles xml output
		WriteMercProfiles();

		// HEADROCK PROFEX: Create mercopinions xml output
		WriteMercOpinions();

		WriteMercStartingGearStats();
	}

	// Reload quote files
	ReloadAllQuoteFiles();

	//Initialize the ShopKeeper Interface ( arms dealer inventory, etc. )
	ShopKeeperScreenInit();

	//Delete the world info
	TrashWorld();

	//Init the help screen system
	InitHelpScreenSystem();

	EmptyDialogueQueue( );

	if ( InAirRaid( ) )
	{
		EndAirRaid( );
}

#ifdef JA2TESTVERSION
	//Reset so we can use the 'cheat key' to start with mercs
	TempHiringOfMercs( 0, TRUE );
#endif

	//Make sure the game starts in the TEAM panel ( it wasnt being reset )
	gsCurInterfacePanel = TEAM_PANEL;

	//Delete all the strategic events
	DeleteAllStrategicEvents();

	// Dealtar: Read in shipping destinations and delivery methods
	gPostalService.Clear(true);

	//This function gets called when ur in a game a click the quit to main menu button, therefore no game is in progress
	gTacticalStatus.fHasAGameBeenStarted = FALSE;

	// Reset timer callbacks
	gpCustomizableTimerCallback = NULL;

	// WANNE - MP: Set cheat level to 0 for mp
	if (gGameExternalOptions.gfCheatMode)
		gubCheatLevel = 6;
	else if (!is_networked)
		gubCheatLevel = STARTING_CHEAT_LEVEL;
	else
		gubCheatLevel = 0;

}

// Flugente: set up VIP locations
void InitVIPSectors()
{
	if ( !gGameExternalOptions.fEnemyGenerals )
		return;

	gStrategicStatus.usVIPsTotal = gGameExternalOptions.usEnemyGeneralsNumber;
	gStrategicStatus.usVIPsLeft = 0;

	// first VIP is the general, if he is still alive
	if ( gMercProfiles[GENERAL].bLife > 0 )
	{
		UINT16 generalsector = SECTOR( gMercProfiles[GENERAL].sSectorX, gMercProfiles[GENERAL].sSectorY );

		// place new VIP in sector
		StrategicMap[generalsector].usFlags |= ENEMY_VIP_PRESENT;

		// no extra troops here. The player might have already taken the barracks but left the general alive
		//SectorInfo[generalsector].ubNumElites += 5;

		++gStrategicStatus.usVIPsLeft;
	}

	for ( UINT8 i = gStrategicStatus.usVIPsLeft; i < gStrategicStatus.usVIPsTotal; ++i )
	{
		UINT16 vipspawnsector = 0;
		if ( GetPossibleVIPSector( vipspawnsector ) )
		{
			// place new VIP in sector
			StrategicMap[vipspawnsector].usFlags |= ENEMY_VIP_PRESENT;
			
			// increase troop count - VIP plus bodyguards
			SectorInfo[vipspawnsector].ubNumElites += ( 1 + gGameExternalOptions.usEnemyGeneralsBodyGuardsNumber );

			// VIP placed
			++gStrategicStatus.usVIPsLeft;
		}
		// spawn the remaining generals in Meduna
		else
		{
			if ( GetRandomEnemyTownSector( MEDUNA, vipspawnsector ) )
			{
				// place new VIP in sector
				StrategicMap[vipspawnsector].usFlags |= ENEMY_VIP_PRESENT;

				// increase troop count - VIP plus bodyguards
				SectorInfo[vipspawnsector].ubNumElites += ( 1 + gGameExternalOptions.usEnemyGeneralsBodyGuardsNumber );

				// VIP placed
				++gStrategicStatus.usVIPsLeft;
			}
		}
	}
}
