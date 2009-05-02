#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "PreBattle Interface.h"
	#include "civ quotes.h"
	#include "Militia Control.h"
	#include "Strategic Event Handler.h"
	#include "HelpScreen.h"
	#include "Cheats.h"
	#include "Animated ProgressBar.h"
	#include "Shopkeeper Interface.h"
#else
	#include "Types.h"
	#include "Soldier Profile.h"
	#include "FileMan.h"
	#include <string.h>
	#include <stdio.h>
	#include "Debug.h"
	#include "OverHead.h"
	#include "Keys.h"
	#include "finances.h"
	#include "History.h"
	#include "files.h"
	#include "Laptop.h"
	#include "iniReader.h"
	#include "Email.h"
	#include "Strategicmap.h"
	#include "Game Events.h"
	#include "Game Clock.h"
	#include "Soldier Create.h"
	#include "WorldDef.h"
	#include "LaptopSave.h"
	#include "strategicmap.h"
	#include "Queen Command.h"
	#include "SaveLoadGame.h"
	#include "Tactical Save.h"
	#include "Squads.h"
	#include "Environment.h"
	#include "Lighting.h"
	#include "Strategic Movement.h"
	#include "Strategic.h"
	#include "Isometric Utils.h"
	#include "Quests.h"
	#include "opplist.h"
	#include "message.h"
	#include "NPC.h"
	#include "Merc Hiring.h"
	#include "SaveLoadScreen.h"
	#include "GameVersion.h"
	#include "GameSettings.h"
	#include "Music Control.h"
	#include "GameScreen.h"
	#include "Options Screen.h"
	#include "Ai.h"
	#include "RenderWorld.h"
	#include "SmokeEffects.h"
	#include "Random.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Interface Bottom.h"

	#include "Interface.h"
	#include "Map Screen Helicopter.h"
	#include "Environment.h"
	#include "Arms Dealer Init.h"
	#include "Tactical Placement GUI.h"

	#include "Strategic Mines.h"
	#include "Strategic Town Loyalty.h"
	#include "Vehicles.h"
	#include "Merc Contract.h"
	#include "Bullets.h"
	#include "air raid.h"
	#include "physics.h"
	#include "Strategic Pathing.h"

	#include "TeamTurns.h"

	#include "explosion control.h"
	#include "Creature Spreading.h"
	#include "Strategic Status.h"
	#include "Prebattle Interface.h"
	#include "Boxing.h"
	#include "Strategic AI.h"
	#include "Map Screen Interface Map.h"

	#include "Meanwhile.h"
	#include "dialogue control.h"
	#include "text.h"
	#include	"Map Screen Interface.h"
	#include	"lighteffects.h"
	#include "HelpScreen.h"
	#include "Animated ProgressBar.h"
	#include "merctextbox.h"
	#include "render dirty.h"
	#include "Map Information.h"
	#include "Interface Items.h"
	#include "Civ Quotes.h"
	#include "Scheduling.h"
	#include "Animation Data.h"
	#include "Game Init.h"
	#include "cheats.h"
	#include "Strategic Event Handler.h"
	#include "interface panels.h"
	#include "interface dialogue.h"
	#include "Assignments.h"
	#include "Interface Items.h"
	#include "Shopkeeper Interface.h"
	#include "postalservice.h"
	// HEADROCK HAM B1: Additional Include for HAM
	#include "MilitiaSquads.h"
#endif

#include		"BobbyR.h"
#include		"Imp Portraits.h"
#include		"Loading Screen.h"
#include		"Interface Utils.h"
#include		"Squads.h"
#include		"IMP Confirm.h"
#include "Enemy Soldier Save.h"
#include "BobbyRMailOrder.h"
#include "Mercs.h"
#include "INIReader.h"

//rain
#include "Rain.h"
//end rain

#include "connect.h"

/////////////////////////////////////////////////////
//
// Local Defines
//
/////////////////////////////////////////////////////


void GetBestPossibleSectorXYZValues( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ );
extern void NextLoopCheckForEnoughFreeHardDriveSpace();
extern void UpdatePersistantGroupsFromOldSave( UINT32 uiSavedGameVersion );
extern void TrashAllSoldiers( );
extern void ResetJA2ClockGlobalTimers( void );

extern void BeginLoadScreen( void );
extern void EndLoadScreen();

extern		CPostalService		gPostalService;

//Global variable used
#ifdef JA2BETAVERSION
UINT32		guiNumberOfMapTempFiles;		//Test purposes
UINT32		guiSizeOfTempFiles;
CHAR			gzNameOfMapTempFile[128];
#endif

extern		SOLDIERTYPE		*gpSMCurrentMerc;
extern		INT32					giSortStateForMapScreenList;
extern		INT16					sDeadMercs[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];
extern		INT32					giRTAILastUpdateTime;
extern		BOOLEAN				gfRedrawSaveLoadScreen;
extern		UINT8					gubScreenCount;
extern		INT16					sWorldSectorLocationOfFirstBattle;
extern		BOOLEAN				gfGamePaused;
extern		BOOLEAN				gfLockPauseState;
extern		BOOLEAN				gfLoadedGame;
extern		HELP_SCREEN_STRUCT gHelpScreen;
extern		UINT8					gubDesertTemperature;
extern		UINT8					gubGlobalTemperature;
extern		BOOLEAN				gfCreatureMeanwhileScenePlayed;
#ifdef JA2BETAVERSION
	extern		UINT8					gubReportMapscreenLock;
#endif

BOOLEAN				gMusicModeToPlay = FALSE;

#ifdef JA2BETAVERSION
BOOLEAN		gfDisplaySaveGamesNowInvalidatedMsg = FALSE;
#endif

BOOLEAN	gfUseConsecutiveQuickSaveSlots = FALSE;
UINT32	guiCurrentQuickSaveNumber = 0;
UINT32	guiLastSaveGameNum;
BOOLEAN DoesAutoSaveFileExist( BOOLEAN fLatestAutoSave );

UINT32	guiJA2EncryptionSet = 0;
UINT32 CalcJA2EncryptionSet( SAVED_GAME_HEADER * pSaveGameHeader );

typedef struct
{
	//The screen that the gaem was saved from
	UINT32	uiCurrentScreen;

	UINT32	uiCurrentUniqueSoldierId;

	//The music that was playing when the game was saved
	UINT8		ubMusicMode;

	//Flag indicating that we have purchased something from Tony
	BOOLEAN	fHavePurchasedItemsFromTony;

	//The selected soldier in tactical
	UINT16	usSelectedSoldier;

	// The x and y scroll position
	INT16		sRenderCenterX;
	INT16		sRenderCenterY;

	BOOLEAN	fAtLeastOneMercWasHired;

	//General Map screen state flags
	BOOLEAN	fShowItemsFlag;
	BOOLEAN	fShowTownFlag;
	BOOLEAN	fShowTeamFlag;
	BOOLEAN	fShowMineFlag;
	BOOLEAN	fShowAircraftFlag;

	// is the helicopter available to player?
	BOOLEAN fHelicopterAvailable;

	// helicopter vehicle id
	INT32 iHelicopterVehicleId;

	// total distance travelled
	INT32 UNUSEDiTotalHeliDistanceSinceRefuel;

	// total owed to player
	INT32 iTotalAccumulatedCostByPlayer;

	// whether or not skyrider is alive and well? and on our side yet?
	BOOLEAN fSkyRiderAvailable;

	// skyrider engaging in a monologue
	BOOLEAN UNUSEDfSkyriderMonologue;

	// list of sector locations
	INT16 UNUSED[ 2 ][ 2 ];

	// is the heli in the air?
	BOOLEAN fHelicopterIsAirBorne;

	// is the pilot returning straight to base?
	BOOLEAN fHeliReturnStraightToBase;

	// heli hovering
	BOOLEAN fHoveringHelicopter;

	// time started hovering
	UINT32 uiStartHoverTime;

	// what state is skyrider's dialogue in in?
	UINT32 uiHelicopterSkyriderTalkState;

	// the flags for skyrider events
	BOOLEAN fShowEstoniRefuelHighLight;
	BOOLEAN fShowOtherSAMHighLight;
	BOOLEAN fShowDrassenSAMHighLight;

	UINT32	uiEnvWeather;

	UINT8		ubDefaultButton;




	BOOLEAN	fSkyriderEmptyHelpGiven;
	BOOLEAN	fEnterMapDueToContract;
	UINT8		ubHelicopterHitsTaken;
	UINT8		ubQuitType;
	BOOLEAN fSkyriderSaidCongratsOnTakingSAM;
	INT16		sContractRehireSoldierID;


	GAME_OPTIONS	GameOptions;

	UINT32	uiSeedNumber;

	//The GetJA2Clock() value
	UINT32	uiBaseJA2Clock;

	INT16		sCurInterfacePanel;

	UINT8		ubSMCurrentMercID;

	BOOLEAN	fFirstTimeInMapScreen;

	BOOLEAN	fDisableDueToBattleRoster;

	BOOLEAN fDisableMapInterfaceDueToBattle;

	INT16		sBoxerGridNo[ NUM_BOXERS ];
	UINT8		ubBoxerID[ NUM_BOXERS ];
	BOOLEAN	fBoxerFought[ NUM_BOXERS ];

	BOOLEAN	fHelicopterDestroyed;								//if the chopper is destroyed
	BOOLEAN	fShowMapScreenHelpText;							//If true, displays help in mapscreen

	INT32		iSortStateForMapScreenList;
	BOOLEAN	fFoundTixa;

	UINT32	uiTimeOfLastSkyriderMonologue;
	BOOLEAN fShowCambriaHospitalHighLight;
	BOOLEAN fSkyRiderSetUp;
	BOOLEAN fRefuelingSiteAvailable[ NUMBER_OF_REFUEL_SITES ];


	//Meanwhile stuff
	MEANWHILE_DEFINITION	gCurrentMeanwhileDef;

	BOOLEAN ubPlayerProgressSkyriderLastCommentedOn;

	BOOLEAN								gfMeanwhileTryingToStart;
	BOOLEAN								gfInMeanwhile;

	// list of dead guys for squads...in id values->-1 means no one home 
	INT16 sDeadMercs[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];

	// levels of publicly known noises
	INT8	gbPublicNoiseLevel[MAXTEAMS];

	UINT8		gubScreenCount;

	UINT16	usOldMeanWhileFlags;

	INT32		iPortraitNumber;

	INT16		sWorldSectorLocationOfFirstBattle;

	BOOLEAN	fUnReadMailFlag;
	BOOLEAN fNewMailFlag;
	BOOLEAN	fOldUnReadFlag;
	BOOLEAN	fOldNewMailFlag;

	BOOLEAN	fShowMilitia;

	BOOLEAN	fNewFilesInFileViewer;

	BOOLEAN	fLastBoxingMatchWonByPlayer;

	UINT32	uiUNUSED;

	BOOLEAN fSamSiteFound[ MAX_NUMBER_OF_SAMS ];

	UINT8		ubNumTerrorists;
	UINT8		ubCambriaMedicalObjects;

	BOOLEAN	fDisableTacticalPanelButtons;

	INT16		sSelMapX;
	INT16		sSelMapY;
	INT32		iCurrentMapSectorZ;

	UINT16	usHasPlayerSeenHelpScreenInCurrentScreen;
	BOOLEAN	fHideHelpInAllScreens;
	UINT8		ubBoxingMatchesWon;

	UINT8		ubBoxersRests;
	BOOLEAN	fBoxersResting;
	UINT8		ubDesertTemperature;
	UINT8		ubGlobalTemperature;

	INT16		sMercArriveSectorX;
	INT16		sMercArriveSectorY;

	BOOLEAN fCreatureMeanwhileScenePlayed;
	UINT8		ubPlayerNum;
	//New stuff for the Prebattle interface / autoresolve
	BOOLEAN fPersistantPBI;
	UINT8 ubEnemyEncounterCode;

	BOOLEAN ubExplicitEnemyEncounterCode;
	BOOLEAN fBlitBattleSectorLocator;
	UINT8 ubPBSectorX;
	UINT8 ubPBSectorY;

	UINT8 ubPBSectorZ;
	BOOLEAN fCantRetreatInPBI;
	BOOLEAN fExplosionQueueActive;
	UINT8	ubUnused[1];

	UINT32	uiMeanWhileFlags;

	INT8 bSelectedInfoChar;
	INT8 bHospitalPriceModifier;
	INT8 bUnused2[ 2 ];

	INT32 iHospitalTempBalance;
	INT32 iHospitalRefund;

	INT8	fPlayerTeamSawJoey;
	INT8	fMikeShouldSayHi;

	UINT8		ubFiller[550];		//This structure should be 1024 bytes

} GENERAL_SAVE_INFO;



UINT32 guiCurrentSaveGameVersion = SAVE_GAME_VERSION;

/////////////////////////////////////////////////////
//
// Global Variables
//
/////////////////////////////////////////////////////

//CHAR8		gsSaveGameNameWithPath[ 512 ];

CHAR8			gSaveDir[ MAX_PATH ];	// Snap: Initilized by InitSaveDir

UINT8			gubSaveGameLoc=0;

UINT32		guiScreenToGotoAfterLoadingSavedGame = 0;

extern		EmailPtr	pEmailList;
extern		UINT32		guiCurrentUniqueSoldierId;
extern		BOOLEAN		gfHavePurchasedItemsFromTony;


/////////////////////////////////////////////////////
//
// Function Prototypes
//
/////////////////////////////////////////////////////

BOOLEAN		SaveMercProfiles( HWFILE hFile );
BOOLEAN		LoadSavedMercProfiles( HWFILE hwFile );

BOOLEAN		SaveSoldierStructure( HWFILE hFile );
BOOLEAN		LoadSoldierStructure( HWFILE hFile );

// CHRISL: New functions to save and load LBENODE data
BOOLEAN SaveLBENODEToSaveGameFile( HWFILE hFile );
BOOLEAN LoadLBENODEFromSaveGameFile( HWFILE hFile );

//BOOLEAN		SavePtrInfo( PTR *pData, UINT32 uiSizeOfObject, HWFILE hFile );
//BOOLEAN		LoadPtrInfo( PTR *pData, UINT32 uiSizeOfObject, HWFILE hFile );

BOOLEAN		SaveEmailToSavedGame( HWFILE hFile );
BOOLEAN		LoadEmailFromSavedGame( HWFILE hFile );

BOOLEAN		SaveTacticalStatusToSavedGame( HWFILE hFile );
BOOLEAN		LoadTacticalStatusFromSavedGame( HWFILE hFile );


BOOLEAN		SetMercsInsertionGridNo( );

BOOLEAN		LoadOppListInfoFromSavedGame( HWFILE hFile );
BOOLEAN		SaveOppListInfoToSavedGame( HWFILE hFile );

BOOLEAN		LoadMercPathToSoldierStruct( HWFILE hFilem, UINT8	ubID );
BOOLEAN		SaveMercPathFromSoldierStruct( HWFILE hFilem, UINT8	ubID );

BOOLEAN		LoadGeneralInfo( HWFILE hFile );
BOOLEAN		SaveGeneralInfo( HWFILE hFile );
BOOLEAN		SavePreRandomNumbersToSaveGameFile( HWFILE hFile );
BOOLEAN		LoadPreRandomNumbersFromSaveGameFile( HWFILE hFile );

BOOLEAN SaveWatchedLocsToSavedGame( HWFILE hFile );
BOOLEAN LoadWatchedLocsFromSavedGame( HWFILE hFile );

BOOLEAN LoadMeanwhileDefsFromSaveGameFile( HWFILE hFile );
BOOLEAN SaveMeanwhileDefsFromSaveGameFile( HWFILE hFile );

void	PauseBeforeSaveGame( void );
void	UnPauseAfterSaveGame( void );
void	UpdateMercMercContractInfo();
void	HandleOldBobbyRMailOrders();
//ppp


#ifdef JA2BETAVERSION
	void			InitSaveGameFilePosition();
	void			InitLoadGameFilePosition();
	void			SaveGameFilePosition( INT32 iPos, STR pMsg );
	void			LoadGameFilePosition( INT32 iPos, STR pMsg );


	void WriteTempFileNameToFile( STR pFileName, UINT32 uiSizeOfFile, HFILE hSaveFile );
	void InitShutDownMapTempFileTest( BOOLEAN fInit, STR pNameOfFile, UINT8 ubSaveGameID	);
#endif

#ifdef JA2BETAVERSION
	extern BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode );
#endif
void ValidateStrategicGroups();

/////////////////////////////////////////////////////
//
// Functions
//
/////////////////////////////////////////////////////

//ADB
//Some notes on saving and loading functions:
//some of these classes are saved in 2 places, in the savegame and in the sector file
//when this happens, the class data saved to the sector file is not encrypted,
//while the savegame may or may not be, this is why there is a bool for save.

//to load from a sector, the data is first loaded in one fell swoop and put in a buffer
//then, each class's data is read from the buffer, so it needs a function that takes a buffer not a file
//this function can automatically assume it is reading non encrypted data
//the regular loading function may or may not be reading encrypted data
//but that is determined on a class by class basis

//when saving encrypted data there is only one new function,
//but there are 2 loading functions to support really old versions

//the encryption set is determined by the savegame header, so make sure
//that saving and loading are logically mirrors, or when changing
//savegame versions you'll try decrypting it differently

//some class's structure have changed.	for these, there is a conversion that needs to take place
//the savegame version is read and the data loaded accordingly, then converted
//load once, convert as many times as necessary

//when changing the structure of some class, check if it is someplace in
//LoadWorld, if it is, you've got to rebuild the maps

//if all that sounds compilcated, it is

extern int gLastLBEUniqueID;
/*
// CHRISL: New function to save/load LBENODE data
BOOLEAN SaveLBENODEToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	if ( !FileWrite( hFile, &gLastLBEUniqueID, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	return TRUE;
}

BOOLEAN LoadLBENODEFromSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	if ( !FileRead( hFile, &gLastLBEUniqueID, sizeof(int), &uiNumBytesRead ) )
	{
		return(FALSE);
	}
	return TRUE;
}
*/
BOOLEAN LBENODE::Load( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, this, SIZEOF_LBENODE_POD, &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if (uniqueID >= gLastLBEUniqueID) {
			//can happen because of the order things are saved and loaded,
			//when combined with copy assignment which makes a new LBENODE
			gLastLBEUniqueID = uniqueID + 1;
		}
		int size;
		if ( !FileRead( hFile, &size, sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}

		inv.resize(size);
		for (std::vector<OBJECTTYPE>::iterator iter = inv.begin(); iter != inv.end(); ++iter) {
			if (! iter->Load(hFile)) {
				return FALSE;
			}
		}
	}
	else
	{
		//we shouldn't be loading from anything before the first change
		AssertGE(guiCurrentSaveGameVersion, NIV_SAVEGAME_DATATYPE_CHANGE);
	}
	return TRUE;
}

BOOLEAN	LBENODE::Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion )
{
	if (dMajorMapVersion >= 6.0 && ubMinorMapVersion > 26 ) {
		LOADDATA( this, *hBuffer, SIZEOF_LBENODE_POD );
		if (uniqueID >= gLastLBEUniqueID) {
			//can happen because of the order things are saved and loaded,
			//when combined with copy assignment which makes a new LBENODE
			gLastLBEUniqueID = uniqueID + 1;
		}
		int size;
		LOADDATA( &size, *hBuffer, sizeof(int) );
		inv.resize(size);
		for (std::vector<OBJECTTYPE>::iterator iter = inv.begin(); iter != inv.end(); ++iter) {
			iter->Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
		}
	}
	else {
		AssertGE(guiCurrentSaveGameVersion, NIV_SAVEGAME_DATATYPE_CHANGE);
	}
	return TRUE;
}

BOOLEAN LBENODE::Save( HWFILE hFile, bool fSavingMap )
{
	UINT32 uiNumBytesWritten;
	int size = inv.size();

	if ( !FileWrite( hFile, this, SIZEOF_LBENODE_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &size, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	for (std::vector<OBJECTTYPE>::iterator iter = inv.begin(); iter != inv.end(); ++iter) {
		//we are not saving to a map, at least not yet
		if (! iter->Save(hFile, false)) {
			return FALSE;
		}
	}
	return TRUE;
}


BOOLEAN LoadArmsDealerInventoryFromSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT8	ubArmsDealer;
	UINT16	usItemIndex;

	//Free all the dealers special inventory arrays
	ShutDownArmsDealers();

	if (guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE) {
		int dealers;
		if (!FileRead( hFile, &dealers, sizeof( int ), &uiNumBytesRead ))
		{
			return( FALSE );
		}
		gArmsDealersInventory.resize(dealers);

		if (!FileRead( hFile, gArmsDealerStatus, sizeof( gArmsDealerStatus ), &uiNumBytesRead ))
		{
			return( FALSE );
		}

		//loop through all the dealers inventories
		for( ubArmsDealer=0; ubArmsDealer<dealers; ubArmsDealer++ )
		{
			int size;
			if (!FileRead( hFile, &size, sizeof( int ), &uiNumBytesRead ))
			{
				return( FALSE );
			}
			gArmsDealersInventory[ubArmsDealer].resize(size);

			//loop through this dealer's individual items
			for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
				iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
				if (! iter->Load(hFile) ) {
					return FALSE;
				}
				//CHRISL: Because of the "100rnd" bug that Tony is experiencing, lets look at each item as we create it.  If the
				//	item is ammo, reset the bItemCondition value to equal the number of rounds.  Hopefully this will be unneccessary
				//	but for the time being, it may help to alleviate the "100rnd" bug.
				if(Item[iter->object.usItem].usItemClass == IC_AMMO)
				{
					//This condition should catch any dealer ammo items that have both invalid bItemCondition and ubShotsLeft values.
					//	We'll only make bItemCondition valid at this point.  Let the next condition correct ubShotsLeft.
					if(iter->bItemCondition > Magazine[Item[iter->object.usItem].ubClassIndex].ubMagSize)
					{
						iter->bItemCondition = Magazine[Item[iter->object.usItem].ubClassIndex].ubMagSize;
					}
					//This condition should catch any dealer ammo items that have an invalid bItemCondition assuming ubShotsLeft is valid
					if(iter->bItemCondition != iter->object[0]->data.ubShotsLeft)
					{
						iter->bItemCondition = iter->object[0]->data.ubShotsLeft;
					}
				}
			}
		}
	}
	else {
		//the format has changed and needs to be updated
		OLD_ARMS_DEALER_STATUS_101 gOldArmsDealerStatus[NUM_ARMS_DEALERS];
		//OLD_DEALER_ITEM_HEADER_101 gOldArmsDealersInventory[NUM_ARMS_DEALERS][MAXITEMS];

		//pointer to an array of OLD_DEALER_ITEM_HEADER_101 that is sized [NUM_ARMS_DEALERS][MAXITEMS]
		typedef OLD_DEALER_ITEM_HEADER_101(*pointerToArmsDealersInventoryArray)[NUM_ARMS_DEALERS][MAXITEMS];

		pointerToArmsDealersInventoryArray pOldArmsDealersInventory
			= (pointerToArmsDealersInventoryArray) new OLD_DEALER_ITEM_HEADER_101[NUM_ARMS_DEALERS][MAXITEMS];


		// Elgin was added to the dealers list in Game Version #54, enlarging these 2 tables...
		// Manny was added to the dealers list in Game Version #55, enlarging these 2 tables...
		bool fIncludesElgin = guiCurrentSaveGameVersion >= 54;
		bool fIncludesManny = guiCurrentSaveGameVersion >= 55;
		UINT32	uiDealersSaved;
		if (fIncludesElgin && fIncludesManny ){
			uiDealersSaved = NUM_ORIGINAL_ARMS_DEALERS;
		}
		else if ( !fIncludesElgin ) {
			// read 2 fewer element without Elgin or Manny in there...
			uiDealersSaved = NUM_ORIGINAL_ARMS_DEALERS - 2;
		}
		else {
			// read one fewer element without Elgin in there...
			uiDealersSaved = NUM_ORIGINAL_ARMS_DEALERS - 1;
		}
		gArmsDealersInventory.resize(uiDealersSaved);

		if (!FileRead( hFile, gOldArmsDealerStatus, uiDealersSaved * sizeof( OLD_ARMS_DEALER_STATUS_101 ), &uiNumBytesRead ))
		{
			return( FALSE );
		}
		if (!FileRead( hFile, (*pOldArmsDealersInventory), uiDealersSaved * sizeof( OLD_DEALER_ITEM_HEADER_101 ) * MAXITEMS, &uiNumBytesRead ))
		{
			return( FALSE );
		}

		if ( !fIncludesElgin ) {
			// initialize Elgin now...
			InitializeOneArmsDealer( ARMS_DEALER_ELGIN );
		}
		if ( !fIncludesManny ) {
			// initialize Manny now...
			InitializeOneArmsDealer( ARMS_DEALER_MANNY );
		}

		OLD_DEALER_SPECIAL_ITEM_101 oldSpecial;
		DEALER_SPECIAL_ITEM loadedSpecial;
		//loop through all the dealers inventories
		for( ubArmsDealer=0; ubArmsDealer<uiDealersSaved; ubArmsDealer++ )
		{
			gArmsDealerStatus[ubArmsDealer] = gOldArmsDealerStatus[ubArmsDealer];

			//loop through this dealer's individual items
			for(usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
			{
				if ( Item[usItemIndex].usItemClass == 0 )
					continue;

				//some things are much much better stored in status now
				gArmsDealerStatus[ubArmsDealer].fPreviouslyEligible[usItemIndex] = (*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].fPreviouslyEligible;
				gArmsDealerStatus[ubArmsDealer].ubStrayAmmo[usItemIndex] = (*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubStrayAmmo;

				//if there are any perfect items, insert them immediately
				if ((*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubPerfectItems) {
					gArmsDealersInventory[ubArmsDealer].push_back(DEALER_SPECIAL_ITEM());
					DEALER_SPECIAL_ITEM* pPerfectItem = &gArmsDealersInventory[ubArmsDealer].back();
					CreateObjectForDealer(usItemIndex, 100, (*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubPerfectItems, &pPerfectItem->object);
					//cannot set to 100, could be ammo
					pPerfectItem->bItemCondition = pPerfectItem->object[0]->data.objectStatus;
				}

				//if there are any items on order, order them
				if ((*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubQtyOnOrder) {
					OrderDealerItems(ubArmsDealer, usItemIndex,
						(*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubQtyOnOrder,
						(*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].uiOrderArrivalTime);
				}

				//if there are any special elements allocated for this item, load them
				for ( int x = 0; x < (*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex].ubElementsAlloced; ++x) {
					if (!FileRead( hFile, &oldSpecial, sizeof( OLD_DEALER_SPECIAL_ITEM_101 ), &uiNumBytesRead ))
					{
						return( FALSE );
					}
					//not all elements alloced are full, some are empty!
					//convert and add to arms dealer list (if applicable)
					loadedSpecial.ConvertFrom101((*pOldArmsDealersInventory)[ubArmsDealer][usItemIndex], oldSpecial, ubArmsDealer, usItemIndex);
				}
			}
		}
		delete [] pOldArmsDealersInventory;
	}

	return( TRUE );
}


BOOLEAN DEALER_SPECIAL_ITEM::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	if ( !FileWrite( hFile, this, SIZEOF_DEALER_SPECIAL_ITEM_POD, &uiNumBytesWritten ) )
	{
		return FALSE;
	}
	if ( !this->object.Save(hFile, FALSE) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN DEALER_SPECIAL_ITEM::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, this, SIZEOF_DEALER_SPECIAL_ITEM_POD, &uiNumBytesRead ) )
		{
			return FALSE;
		}
		if ( !this->object.Load(hFile) )
		{
			return FALSE;
		}
	}
	else
	{
		//this will never be loaded from sometime before the first change
		AssertGE(guiCurrentSaveGameVersion, NIV_SAVEGAME_DATATYPE_CHANGE);
	}
	return TRUE;
}

BOOLEAN REAL_OBJECT::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	if ( !FileWrite( hFile, this, SIZEOF_REAL_OBJECT_POD, &uiNumBytesWritten ) )
	{
		return FALSE;
	}
	if ( !this->Obj.Save(hFile, FALSE) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN REAL_OBJECT::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, this, SIZEOF_REAL_OBJECT_POD, &uiNumBytesRead ) )
		{
			return FALSE;
		}
		if ( !this->Obj.Load(hFile) )
		{
			return FALSE;
		}
	}
	else
	{
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OLD_REAL_OBJECT_101 oldObject;
			if ( !FileRead( hFile, &oldObject, sizeof( OLD_REAL_OBJECT_101 ), &uiNumBytesRead ) )
			{
				return FALSE;
			}
			*this = oldObject;
		}
	}
	return TRUE;
}

BOOLEAN INVENTORY_IN_SLOT::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	if ( !FileWrite( hFile, this, SIZEOF_INVENTORY_IN_SLOT_POD, &uiNumBytesWritten ) )
	{
		return FALSE;
	}
	if ( !this->ItemObject.Save(hFile, FALSE) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN INVENTORY_IN_SLOT::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, this, SIZEOF_INVENTORY_IN_SLOT_POD, &uiNumBytesRead ) )
		{
			return FALSE;
		}
		if ( !this->ItemObject.Load(hFile) )
		{
			return FALSE;
		}
	}
	else
	{
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OLD_INVENTORY_IN_SLOT_101 oldItem;
			if ( !FileRead( hFile, &oldItem, sizeof( OLD_INVENTORY_IN_SLOT_101 ), &uiNumBytesRead ) )
			{
				return FALSE;
			}
			*this = oldItem;
		}
	}
	return TRUE;
}

BOOLEAN MERC_LEAVE_ITEM::Save(HWFILE hFile)
{
	if ( !this->object.Save(hFile, FALSE) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN MERC_LEAVE_ITEM::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !this->object.Load(hFile) )
		{
			return FALSE;
		}
	}
	else
	{
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OLD_MERC_LEAVE_ITEM_101 oldItem;
			if ( !FileRead( hFile, &oldItem, sizeof( OLD_MERC_LEAVE_ITEM_101 ), &uiNumBytesRead ) )
			{
				return FALSE;
			}
			this->object = oldItem.oldObject;
		}
	}
	return TRUE;
}

BOOLEAN ITEM_CURSOR_SAVE_INFO::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		//3 bytes of info, screw being neat
		if ( !FileRead( hFile, this, 3, &uiNumBytesRead ) )
		{
			return FALSE;
		}
		if ( !this->ItemPointerInfo.Load(hFile) )
		{
			return FALSE;
		}
	}
	else
	{
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OLD_ITEM_CURSOR_SAVE_INFO_101 oldInfo101;
			if ( !FileRead( hFile, &oldInfo101, sizeof(OLD_ITEM_CURSOR_SAVE_INFO_101), &uiNumBytesRead ) )
			{
				return FALSE;
			}
			*this = oldInfo101;
		}

	}
	return TRUE;
}

BOOLEAN ITEM_CURSOR_SAVE_INFO::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	//3 bytes of info, screw being neat
	if ( !FileWrite( hFile, this, 3, &uiNumBytesWritten ) )
	{
		return FALSE;
	}
	if ( !this->ItemPointerInfo.Save(hFile, FALSE) )
	{
		return FALSE;
	}
	return TRUE;
}


BOOLEAN SOLDIERCREATE_STRUCT::Save(HWFILE hFile, bool fSavingMap)
{
	UINT32 uiNumBytesWritten;
	if ( !FileWrite( hFile, this, SIZEOF_SOLDIERCREATE_STRUCT_POD, &uiNumBytesWritten ) )
	{
		return FALSE;
	}
	if ( !this->Inv.Save(hFile, fSavingMap) )
	{
		return FALSE;
	}

	//ADB screw checksums, they suck, also, checksums aren't always saved
	/*
	if (fSavingMap == false) {
		UINT16 usCheckSum = GetChecksum();
		if ( !FileWrite( hFile, &usCheckSum, 2, &uiNumBytesWritten ) )
		{
			return FALSE;
		}
	}
	*/
	return TRUE;
}

BOOLEAN SOLDIERCREATE_STRUCT::Load(INT8 **hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	if (dMajorMapVersion >= 6.0 && ubMinorMapVersion > 26 ) {
		LOADDATA( this, *hBuffer, SIZEOF_SOLDIERCREATE_STRUCT_POD );
		this->Inv.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
	}
	else {
		//ADB checksum was not saved under these circumstances!
		OLD_SOLDIERCREATE_STRUCT_101 OldSavedSoldierInfo101;
		LOADDATA( &OldSavedSoldierInfo101, *hBuffer, SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD );
		OldSavedSoldierInfo101.CopyOldInventoryToNew();
		*this = OldSavedSoldierInfo101;
	}
	return TRUE;
}

BOOLEAN SOLDIERCREATE_STRUCT::Load(HWFILE hFile, int versionToLoad, bool loadChecksum)
{
	UINT32 uiNumBytesRead;

	//ADB need to overwrite guiCurrentSaveGameVersion so that the other things loaded (like inv) are loaded ok
	int tempVersion = guiCurrentSaveGameVersion;
	guiCurrentSaveGameVersion = versionToLoad;

	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE)
	{
		//the info has changed at version 102
		//first, load the POD
		if ( !FileRead( hFile, this, SIZEOF_SOLDIERCREATE_STRUCT_POD, &uiNumBytesRead ) )
		{
			guiCurrentSaveGameVersion = tempVersion;
			return(FALSE);
		}

		//load the OO inventory
		if ( !this->Inv.Load(hFile) )
		{
			guiCurrentSaveGameVersion = tempVersion;
			return(FALSE);
		}
	}
	else
	{
		OLD_SOLDIERCREATE_STRUCT_101 OldSavedSoldierInfo101;
		//we are loading an older version (only load once, so use "else if")
		//first load the data based on what version was stored
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			if ( !FileRead( hFile, &OldSavedSoldierInfo101, SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD, &uiNumBytesRead ) )
			{
				guiCurrentSaveGameVersion = tempVersion;
				return FALSE;
			}
		}
		/*
		else if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
			OldSavedSoldierInfo999.initialize();
			FileRead( hFile, &OldSavedSoldierInfo999, SIZEOF_OLD_SOLDIERCREATE_STRUCT_999_POD, &uiNumBytesRead );
		*/

		//now we have the data that needs to be converted (keep on converting up, so use "if")
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OldSavedSoldierInfo101.CopyOldInventoryToNew();
			*this = OldSavedSoldierInfo101;
			//OldSavedSoldierInfo999 = OldSavedSoldierInfo101;
		}
		//change this when changing the file version again
		/*
		if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
		{
			*this = OldSavedSoldierInfo999;
		}
		*/


		//ADB need to make sure we are loading the checksum only if the older version saved it
		if (loadChecksum == true) {
#ifdef JA2TESTVERSION
			CHAR8		zReason[256];
#endif

			UINT16 usCheckSum;
			FileRead( hFile, &usCheckSum, 2, &uiNumBytesRead );
			if( uiNumBytesRead != 2 )
			{
				#ifdef JA2TESTVERSION
					sprintf( zReason, "Load SoldierCreateStruct -- EOF while reading usCheckSum.	KM"	);
					AssertMsg( 0, zReason );
				#endif
				guiCurrentSaveGameVersion = tempVersion;
				return FALSE;
			}
			//ADB screw checksums, they suck
			/*
			//verify the checksum equation (anti-hack) -- see save 
			UINT16 usFileCheckSum = GetChecksum();
			if( usCheckSum != usFileCheckSum )
			{	//Hacker has modified the stats on the enemy placements.
				#ifdef JA2TESTVERSION
					sprintf( zReason, "EnemySoldier -- checksum for placement %d failed.	KM", usFileCheckSum );
					AssertMsg( 0, zReason );
				#endif
				guiCurrentSaveGameVersion = tempVersion;
				return FALSE;
			}
			*/
		}
	}

	guiCurrentSaveGameVersion = tempVersion;
	return TRUE;
}

// Changed by ADB, rev 1513
//BOOLEAN MERCPROFILESTRUCT::Load(HWFILE hFile, bool forceLoadOldVersion)
BOOLEAN MERCPROFILESTRUCT::Load(HWFILE hFile, bool forceLoadOldVersion, bool forceLoadOldEncryption, bool wasSavedWithEncryption)
{
	UINT32 uiNumBytesRead;
	this->initialize();

	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE && forceLoadOldVersion == false)
	{
		if ( !FileRead( hFile, this, SIZEOF_MERCPROFILESTRUCT_POD, &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		int size;
		if ( !FileRead( hFile, &size, sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		inv.resize(size);
		bInvStatus.resize(size);
		bInvNumber.resize(size);
		int item;
		int status;
		int number;
		for (int x = 0; x < size; ++x) {
			if ( !FileRead( hFile, &item, sizeof(int), &uiNumBytesRead ) )
			{
				return(FALSE);
			}
			if ( !FileRead( hFile, &status, sizeof(int), &uiNumBytesRead ) )
			{
				return(FALSE);
			}
			if ( !FileRead( hFile, &number, sizeof(int), &uiNumBytesRead ) )
			{
				return(FALSE);
			}
			inv[x] = item;
			bInvStatus[x] = status;
			bInvNumber[x] = number;
		}
		if ( this->uiProfileChecksum != this->GetChecksum() )
		{
			return( FALSE );
		}
	}
	else
	{
		//we are loading an older version (only load once, so use "else if")
		//first load the data based on what version was stored
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE || forceLoadOldVersion == true)
		{
			OLD_MERCPROFILESTRUCT_101 old;
			typedef BOOLEAN (*functionPtr) ( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead );
			functionPtr pLoadingFunction;
			// Changed by ADB, rev 1513
			//if ( guiCurrentSaveGameVersion < 87 || forceLoadOldVersion == true)
			if (wasSavedWithEncryption == false) {
				pLoadingFunction = &FileRead;
			}
			else if ( guiCurrentSaveGameVersion < 87 || forceLoadOldEncryption == true)
			{
				pLoadingFunction = &JA2EncryptedFileRead;
			}
			else
			{
				pLoadingFunction = &NewJA2EncryptedFileRead;
			}

			if ( !(*pLoadingFunction)( hFile, &old, SIZEOF_OLD_MERCPROFILESTRUCT_101_POD, &uiNumBytesRead ) )
			{
				return(FALSE);
			}
			*this = old;
		}
		//skip checksum
	}
	return TRUE;
}

BOOLEAN MERCPROFILESTRUCT::Save(HWFILE hFile)
{
	//for now the MERCPROFILESTRUCT remains unchanged
	//it does not have any data types that need their own saving function
	//if that ever changes this will make updating saves easier
	UINT32 uiNumBytesWritten;
	this->uiProfileChecksum = this->GetChecksum();
	if ( !FileWrite( hFile, this, SIZEOF_MERCPROFILESTRUCT_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	int size = inv.size();
	if ( !FileWrite( hFile, &size, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	for (int x = 0; x < size; ++x) {
		int item = inv[x];
		int status = bInvStatus[x];
		int number = bInvNumber[x];
		if ( !FileWrite( hFile, &item, sizeof(int), &uiNumBytesWritten ) )
		{
			return(FALSE);
		}
		if ( !FileWrite( hFile, &status, sizeof(int), &uiNumBytesWritten ) )
		{
			return(FALSE);
		}
		if ( !FileWrite( hFile, &number, sizeof(int), &uiNumBytesWritten ) )
		{
			return(FALSE);
		}
	}
	return TRUE;
}

BOOLEAN SOLDIERTYPE::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	// calculate checksum for soldier
	this->uiMercChecksum = this->GetChecksum();
	if ( !FileWrite( hFile, this, SIZEOF_SOLDIERTYPE_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}

	//save OO data like inventory
	if ( !this->inv.Save(hFile, FALSE) )
	{
		return(FALSE);
	}

	if ( !FileWrite( hFile, &this->aiData, sizeof(STRUCT_AIData), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->flags, sizeof(STRUCT_Flags), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->timeChanges, sizeof(STRUCT_TimeChanges), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->timeCounters, sizeof(STRUCT_TimeCounters), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->drugs, sizeof(STRUCT_Drugs), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->stats, sizeof(STRUCT_Statistics), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &this->pathing, sizeof(STRUCT_Pathing), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	return TRUE;
}

INT32 ReadFieldByField(HWFILE hFile, PTR pDest, UINT32 uiFieldSize, UINT32 uiElementSize, UINT32  uiCurByteCount)
{
	UINT32	uiNumBytesRead;
	char	padding[10];
	UINT32	uiBytesRead = uiCurByteCount;	// used to track our new byte count
	UINT32	sampleBytesRead = uiBytesRead;	// used to determine how much padding is needed

	while((sampleBytesRead % uiElementSize) > 0)	// as long as we have a remainder
		sampleBytesRead++;

	if(sampleBytesRead != uiBytesRead)	// if we need padding
	{
		FileRead(hFile, &padding, (sampleBytesRead-uiBytesRead), &uiNumBytesRead );
		uiBytesRead += uiNumBytesRead;
	}

	// the actual file read
	FileRead(hFile, pDest, uiFieldSize, &uiNumBytesRead );
	uiBytesRead += uiNumBytesRead;

	return uiBytesRead;
}

BOOLEAN SOLDIERTYPE::Load(HWFILE hFile)
{
	UINT32 uiNumBytesRead;
	INT32	numBytesRead = 0;
	char	padding[10];

	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		//the soldier type info has changed at version 102
		//CHRISL: It was further changed at version 104 to allow for 16bit action points for the 100AP system
		//first, load the POD
		if ( guiCurrentSaveGameVersion >= AP100_SAVEGAME_DATATYPE_CHANGE )
		{
			if ( !FileRead( hFile, this, SIZEOF_SOLDIERTYPE_POD, &uiNumBytesRead ) )
			{
				return(FALSE);
			}
		}
		else
		{
			numBytesRead = 0;
			FileRead( hFile, this, (offsetof(SOLDIERTYPE, ubBodyType)+sizeof(ubBodyType)), &uiNumBytesRead );
			numBytesRead += uiNumBytesRead;
			numBytesRead = ReadFieldByField(hFile, &this->bActionPoints, 1, 1, numBytesRead);
			numBytesRead = ReadFieldByField(hFile, &this->bInitialActionPoints, 1, 1, numBytesRead);
			numBytesRead = ReadFieldByField(hFile, &this->bOldLife, sizeof(bOldLife), sizeof(INT8), numBytesRead);
			numBytesRead = ReadFieldByField(hFile, &this->bVisible, sizeof(bVisible), sizeof(INT8), numBytesRead);
			numBytesRead = ReadFieldByField(hFile, &this->bActive, sizeof(bActive), sizeof(INT8), numBytesRead);
			numBytesRead = ReadFieldByField(hFile, &this->bTeam, sizeof(bTeam), sizeof(INT8), numBytesRead);
			//CHRISL: in the old structure, there are 3 padding bytes at this point in the file.  We need to account for
			//	them before continuing.  We also want to include them in our numBytesRead since they cover the changes
			//	caused by increasing two elements by 1byte each.  This includes shifting one byte of padding
			FileRead( hFile, padding, 3, &uiNumBytesRead );
			numBytesRead += uiNumBytesRead;
			FileRead( hFile, &this->pTempObject, (SIZEOF_SOLDIERTYPE_POD-numBytesRead), &uiNumBytesRead );
			numBytesRead += uiNumBytesRead;
			if(numBytesRead != SIZEOF_SOLDIERTYPE_POD)
				return(FALSE);
		}

		//load the OO inventory
		if ( !this->inv.Load(hFile) )
		{
			return(FALSE);
		}

		//load some structs, atm just POD but could change
		//Load STRUCT_AIData
		numBytesRead = 0;
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bOppList, sizeof(aiData.bOppList), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bLastAction, sizeof(aiData.bLastAction), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bAction, sizeof(aiData.bAction), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.usActionData, sizeof(aiData.usActionData), sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNextAction, sizeof(aiData.bNextAction), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.usNextActionData, sizeof(aiData.usNextActionData), sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bActionInProgress, sizeof(aiData.bActionInProgress), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bAlertStatus, sizeof(aiData.bAlertStatus), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bOppCnt, sizeof(aiData.bOppCnt), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNeutral, sizeof(aiData.bNeutral), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNewSituation, sizeof(aiData.bNewSituation), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNextTargetLevel, sizeof(aiData.bNextTargetLevel), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bOrders, sizeof(aiData.bOrders), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bAttitude, sizeof(aiData.bAttitude), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bUnderFire, sizeof(aiData.bUnderFire), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bShock, sizeof(aiData.bShock), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bUnderEscort, sizeof(aiData.bUnderEscort), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bBypassToGreen, sizeof(aiData.bBypassToGreen), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubLastMercToRadio, sizeof(aiData.ubLastMercToRadio), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bDominantDir, sizeof(aiData.bDominantDir), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bPatrolCnt, sizeof(aiData.bPatrolCnt), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNextPatrolPnt, sizeof(aiData.bNextPatrolPnt), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.sPatrolGrid, sizeof(aiData.sPatrolGrid), sizeof(INT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.sNoiseGridno, sizeof(aiData.sNoiseGridno), sizeof(INT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubNoiseVolume, sizeof(aiData.ubNoiseVolume), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bLastAttackHit, sizeof(aiData.bLastAttackHit), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubXRayedBy, sizeof(aiData.ubXRayedBy), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.dHeightAdjustment, sizeof(aiData.dHeightAdjustment), sizeof(FLOAT), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bMorale, sizeof(aiData.bMorale), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bTeamMoraleMod, sizeof(aiData.bTeamMoraleMod), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bTacticalMoraleMod, sizeof(aiData.bTacticalMoraleMod), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bStrategicMoraleMod, sizeof(aiData.bStrategicMoraleMod), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bAIMorale, sizeof(aiData.bAIMorale), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubPendingAction, sizeof(aiData.ubPendingAction), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubPendingActionAnimCount, sizeof(aiData.ubPendingActionAnimCount), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.uiPendingActionData1, sizeof(aiData.uiPendingActionData1), sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.sPendingActionData2, sizeof(aiData.sPendingActionData2), sizeof(INT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bPendingActionData3, sizeof(aiData.bPendingActionData3), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubDoorHandleCode, sizeof(aiData.ubDoorHandleCode), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.uiPendingActionData4, sizeof(aiData.uiPendingActionData4), sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bInterruptDuelPts, sizeof(aiData.bInterruptDuelPts), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bPassedLastInterrupt, sizeof(aiData.bPassedLastInterrupt), sizeof(INT8), numBytesRead);
		if ( guiCurrentSaveGameVersion >= AP100_SAVEGAME_DATATYPE_CHANGE )
			numBytesRead = ReadFieldByField(hFile, &this->aiData.bIntStartAPs, sizeof(aiData.bIntStartAPs), sizeof(INT8), numBytesRead);
		else
			numBytesRead = ReadFieldByField(hFile, &this->aiData.bIntStartAPs, 1, 1, numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bMoved, sizeof(aiData.bMoved), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bHunting, sizeof(aiData.bHunting), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubLastCall, sizeof(aiData.ubLastCall), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.ubCaller, sizeof(aiData.ubCaller), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.sCallerGridNo, sizeof(aiData.sCallerGridNo), sizeof(INT16), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bCallPriority, sizeof(aiData.bCallPriority), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bCallActedUpon, sizeof(aiData.bCallActedUpon), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bFrenzied, sizeof(aiData.bFrenzied), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bNormalSmell, sizeof(aiData.bNormalSmell), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bMonsterSmell, sizeof(aiData.bMonsterSmell), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bMobility, sizeof(aiData.bMobility), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bRTPCombat, sizeof(aiData.bRTPCombat), sizeof(INT8), numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.fAIFlags, sizeof(aiData.fAIFlags), sizeof(INT8), numBytesRead);
		if ( guiCurrentSaveGameVersion >= AP100_SAVEGAME_DATATYPE_CHANGE )
			numBytesRead = ReadFieldByField(hFile, &this->aiData.bAimTime, sizeof(aiData.bAimTime), sizeof(INT8), numBytesRead);
		else
			numBytesRead = ReadFieldByField(hFile, &this->aiData.bAimTime, 1, 1, numBytesRead);
		numBytesRead = ReadFieldByField(hFile, &this->aiData.bShownAimTime, sizeof(aiData.bShownAimTime), sizeof(INT8), numBytesRead);
		//CHRISL: We also need to make sure the structure aligns properly and that we don't need to read any extra
		//	padding bytes
		while((numBytesRead%__alignof(STRUCT_AIData)) > 0)
		{
			FileRead( hFile, &padding, 1,  &uiNumBytesRead );
			numBytesRead += uiNumBytesRead;
		}
		if ( guiCurrentSaveGameVersion < AP100_SAVEGAME_DATATYPE_CHANGE )
		{
			//CHRISL: Because we're increasing the size of two elements in this structure by 1byte each, we need to
			//	increase the size of numBytesRead by 2 so that our sizes match.  However, we also need to increase the
			//	size of numBytesRead by another 2 so that the new structure alignment is accounted for.
			numBytesRead += 4;
		}
		if(numBytesRead != sizeof(STRUCT_AIData))
			return(FALSE);

		//Load STRUCT_Flags
		if ( !FileRead( hFile, &this->flags, sizeof(STRUCT_Flags), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &this->timeChanges, sizeof(STRUCT_TimeChanges), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &this->timeCounters, sizeof(STRUCT_TimeCounters), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &this->drugs, sizeof(STRUCT_Drugs), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &this->stats, sizeof(STRUCT_Statistics), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &this->pathing, sizeof(STRUCT_Pathing), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		// check checksum
		if ( this->GetChecksum() != this->uiMercChecksum )
		{
			return( FALSE );
		}
	}
	else
	{
		OLDSOLDIERTYPE_101 OldSavedSoldierInfo101;
		//we are loading an older version (only load once, so use "else if")
		//first load the data based on what version was stored
		typedef BOOLEAN (*functionPtr) ( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead );
		functionPtr pLoadingFunction;
		if ( guiCurrentSaveGameVersion < 87 )
		{
			pLoadingFunction = &JA2EncryptedFileRead;
		}
		else
		{
			pLoadingFunction = &NewJA2EncryptedFileRead;
		}

		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OldSavedSoldierInfo101.initialize();
			if ( !(*pLoadingFunction)( hFile, &OldSavedSoldierInfo101, SIZEOF_OLDSOLDIERTYPE_101_POD, &uiNumBytesRead ) )
			{
				return(FALSE);
			}
		}
		/*
		else if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
			OldSavedSoldierInfo999.initialize();
			(*pLoadingFunction)( hFile, &OldSavedSoldierInfo999, SIZEOF_OLDSOLDIERTYPE_999_POD, &uiNumBytesRead );
		*/

		//now we have the data that needs to be converted (keep on converting up, so use "if")
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			OldSavedSoldierInfo101.CopyOldInventoryToNew();
			(*this) = OldSavedSoldierInfo101;
			//OldSavedSoldierInfo999 = OldSavedSoldierInfo101;
		}
		//change this when changing the file version again
		/*
		if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
		{
			*this = OldSavedSoldierInfo999;
		}
		*/
		//assume checksum is ok
	}
	return TRUE;
}

BOOLEAN WORLDITEM::Save(HWFILE hFile, bool fSavingMap)
{
	//save the POD
	UINT32 uiNumBytesWritten;
	if ( !FileWrite( hFile, this, SIZEOF_WORLDITEM_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}

	//save the OO OBJECTTYPE
	if ( !this->object.Save(hFile, fSavingMap) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN WORLDITEM::Load(INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	//if we are at the most current MAP version, 5.0 and 27, then fine
	if (dMajorMapVersion >= 6.0 && ubMinorMapVersion > 26 )
	{
		//load the POD
		LOADDATA( this, *hBuffer, SIZEOF_WORLDITEM_POD );

		//now load the OO OBJECTTYPE
		this->object.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
	}
	//if we need to load an older save
	else {
		//load the old data into a suitable structure, it's just POD
		OLD_WORLDITEM_101 oldWorldItem;
		LOADDATA( &oldWorldItem, *hBuffer, sizeof( OLD_WORLDITEM_101 ) );
		*this = oldWorldItem;
	}
	return TRUE;
}

BOOLEAN WORLDITEM::Load(HWFILE hFile)
{
	UINT32	uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		//load the POD
		if ( !FileRead( hFile, this, SIZEOF_WORLDITEM_POD, &uiNumBytesRead ) )
		{
			return(FALSE);
		}

		//now load the OO OBJECTTYPE
		if ( !this->object.Load(hFile) )
		{
			return FALSE;
		}
	}
	//if we need to load an older save
	else {
		//load the old data into a suitable structure, it's just POD
		OLD_WORLDITEM_101	oldWorldItem;
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			if ( !FileRead( hFile, &oldWorldItem, sizeof(OLD_WORLDITEM_101), &uiNumBytesRead ) )
			{
				return(FALSE);
			}
		}

		//now we have the data that needs to be converted (keep on converting up, so use "if")
		//the first conversion is simple enough that it can be done here
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			*this = oldWorldItem;
		}
	}
	return TRUE;
}

BOOLEAN StackedObjectData::Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion )
{
	//when this function is called, the file has been loaded into a buffer using FileRead
	//if we are at the most current version, then fine
	//but we can also be loading this from a map that is up to date when the savegame isn't!
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE
		|| (dMajorMapVersion == gdMajorMapVersion && gubMinorMapVersion == ubMinorMapVersion))
	{
		int size;
		LOADDATA(&(this->data), *hBuffer, sizeof(ObjectData) );
		LOADDATA(&size, *hBuffer, sizeof(int) );
		attachments.resize(size);
		for (attachmentList::iterator iter = attachments.begin(); iter != attachments.end(); ++iter) {
			if (! iter->Load( hBuffer, dMajorMapVersion, ubMinorMapVersion ) ) {
				return FALSE;
			}
		}
	}
	else {
		//we shouldn't be loading this
		AssertMsg(false, "we shouldn't be loading this");
	}
	return TRUE;
}

BOOLEAN StackedObjectData::Load( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, &(this->data), sizeof(ObjectData), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		int size;
		if ( !FileRead( hFile, &size, sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		attachments.resize(size);
		for (attachmentList::iterator iter = attachments.begin(); iter != attachments.end(); ++iter) {
			if (! iter->Load(hFile) ) {
				return FALSE;
			}
		}
	}
	else {
		//we shouldn't be loading this
		Assert(false);
	}
	return TRUE;
}
BOOLEAN StackedObjectData::Save( HWFILE hFile, bool fSavingMap )
{
	//if we are saving this to a map file it will be loaded with FileRead
	UINT32 uiNumBytesWritten;
	int size = 0;
	for (attachmentList::iterator iter = attachments.begin(); iter != attachments.end(); ++iter) {
		if (iter->exists() == true) {
			++size;
		}
		else {
			//attachments should always exist, if they didn't they should have been removed from the list
			DebugBreakpoint();
		}
	}

	if ( !FileWrite( hFile, &(this->data), sizeof(ObjectData), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &size, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	for (attachmentList::iterator iter = attachments.begin(); iter != attachments.end(); ++iter) {
		if (iter->exists() == true) {
			if (! iter->Save(hFile, fSavingMap)) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOLEAN OBJECTTYPE::Load( HWFILE hFile )
{

	UINT32	uiNumBytesRead;
	//if we are at the most current version, then fine
	if ( guiCurrentSaveGameVersion >= NIV_SAVEGAME_DATATYPE_CHANGE )
	{
		if ( !FileRead( hFile, this, SIZEOF_OBJECTTYPE_POD, &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		int size;
		if ( !FileRead( hFile, &size, sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}

		objectStack.resize(size);
		int x = 0;
		for (StackedObjects::iterator iter = objectStack.begin(); iter != objectStack.end(); ++iter, ++x) {
			if (! iter->Load(hFile)) {
				return FALSE;
			}
			if (this->IsActiveLBE(x) == true) {
				LBEArray.push_back(LBENODE());
				if (! LBEArray.back().Load(hFile)) {
					return FALSE;
				}
			}
		}
	}
	else
	{
		OLD_OBJECTTYPE_101 OldSavedObject101;
		//we are loading an older version (only load once, so use "else if")
		//first load the data based on what version was stored
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			if ( !FileRead( hFile, &OldSavedObject101, sizeof(OLD_OBJECTTYPE_101), &uiNumBytesRead ) )
			{
				return(FALSE);
			}
		}
		/*
		else if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
			(*pLoadingFunction)( hFile, &OldSavedObject999, sizeof(OLD_OBJECTTYPE_999), &uiNumBytesRead );
		*/

		//now we have the data that needs to be converted (keep on converting up, so use "if")
		if ( guiCurrentSaveGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE )
		{
			(*this) = OldSavedObject101;
			//OldSavedObject999 = OldSavedObject101;
		}
		//change this when changing the file version again
		/*
		if ( guiCurrentSaveGameVersion < SECOND_SAVEGAME_DATATYPE_CHANGE )
		{
			(*this) = OldSavedObject999;
		}
		*/
	}
	return TRUE;
}

BOOLEAN OBJECTTYPE::Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion )
{
	//when this function is called, the file has been loaded into a buffer using FileRead
	//if we are at the most current MAP version, 6.27, then fine
	if (dMajorMapVersion >= 6.0 && ubMinorMapVersion > 26 )
	{
		int size;
		LOADDATA(this, *hBuffer, SIZEOF_OBJECTTYPE_POD );
		LOADDATA(&size, *hBuffer, sizeof(int) );
		objectStack.resize(size);
		int x = 0;
		for (StackedObjects::iterator iter = objectStack.begin(); iter != objectStack.end(); ++iter, ++x) {
			iter->Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
			if (this->IsActiveLBE(x) == true) {
				LBEArray.push_back(LBENODE());
				if (! LBEArray.back().Load(hBuffer, dMajorMapVersion, ubMinorMapVersion)) {
					return FALSE;
				}
			}
		}
	}
	else
	{
		OLD_OBJECTTYPE_101 OldSavedObject101;
		LOADDATA( &OldSavedObject101, *hBuffer, sizeof(OLD_OBJECTTYPE_101) );
		(*this) = OldSavedObject101;
	}
	return TRUE;
}

BOOLEAN OBJECTTYPE::Save( HWFILE hFile, bool fSavingMap )
{
	//if we are saving this to a map file it will be loaded with FileRead
	UINT32 uiNumBytesWritten;
	int size = objectStack.size();
	if ( !FileWrite( hFile, this, SIZEOF_OBJECTTYPE_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	if ( !FileWrite( hFile, &size, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}
	int x = 0;
	for (StackedObjects::iterator iter = objectStack.begin(); iter != objectStack.end(); ++iter, ++x) {
		if (! iter->Save(hFile, fSavingMap)) {
			return FALSE;
		}
		if (this->IsActiveLBE(x) == true) {
			LBENODE* pLBE = this->GetLBEPointer(x);
			if (! pLBE->Save(hFile, fSavingMap)) {
				return FALSE;
			}
			//CHRISL: I've remarked this to resolve a problem when the autosave option is active where LBEArray info is
			//    lost and not rebuilt until after combat is over.  This may not be the best way to resolve this, though,
			//    as ADB may well have had a valid reason to erase the LBEArray during saves.
/*
			int uniqueID = (*this)[x]->data.lbe.uniqueID;
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
*/
		}
	}
	return TRUE;
}

BOOLEAN Inventory::Load( HWFILE hFile )
{

	UINT32 uiNumBytesRead;
	int sizeInventory;//how many items are in the stack?
	if ( !FileRead( hFile, &sizeInventory, sizeof(int), &uiNumBytesRead ) )
	{
		return FALSE;
	}

	if (sizeInventory < 0 || sizeInventory > NUM_INV_SLOTS)
	{
		return(FALSE);
	}

	inv.resize(sizeInventory);
	bNewItemCount.resize(sizeInventory);
	bNewItemCycleCount.resize(sizeInventory);
	for (int x = 0; x < sizeInventory; ++x) {
		//load the OO OBJECTTYPE
		if ( !inv[x].Load(hFile) )
		{
			return FALSE;
		}

		UINT32 uiNumBytesRead;
		if ( !FileRead( hFile, &bNewItemCount[x], sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &bNewItemCycleCount[x], sizeof(int), &uiNumBytesRead ) )
		{
			return(FALSE);
		}
	}
	return (TRUE);
}

BOOLEAN Inventory::Load(INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	int sizeInventory;
	LOADDATA(&sizeInventory, *hBuffer, sizeof(int) );
	if (sizeInventory < 0 || sizeInventory > NUM_INV_SLOTS)
	{
		return(FALSE);
	}

	inv.resize(sizeInventory);
	bNewItemCount.resize(sizeInventory);
	bNewItemCycleCount.resize(sizeInventory);

	for (int x = 0; x < sizeInventory; ++x) {
		//load the OO OBJECTTYPE
		if ( !inv[x].Load(hBuffer, dMajorMapVersion, ubMinorMapVersion) )
		{
			return FALSE;
		}

		LOADDATA( &bNewItemCount[x], *hBuffer, sizeof(int) );
		LOADDATA( &bNewItemCycleCount[x], *hBuffer, sizeof(int) );
	}
	return TRUE;
}

BOOLEAN Inventory::Save( HWFILE hFile, bool fSavingMap )
{
	UINT32 uiNumBytesWritten;
	int sizeInventory = size();//how many items are in the stack?
	if ( !FileWrite( hFile, &sizeInventory, sizeof(int), &uiNumBytesWritten ) )
	{
		return(FALSE);
	}

	for (int x = 0; x < sizeInventory; ++x) {
		//save the OO OBJECTTYPE
		if ( !inv[x].Save(hFile, fSavingMap) )
		{
			return (FALSE);
		}

		if ( !FileWrite( hFile, &(bNewItemCount[x]), sizeof(int), &uiNumBytesWritten ) )
		{
			return(FALSE);
		}
		if ( !FileWrite( hFile, &(bNewItemCycleCount[x]), sizeof(int), &uiNumBytesWritten ) )
		{
			return(FALSE);
		}
	}
	return TRUE;
}

// Snap: Initializes gSaveDir global, creating the save directory if necessary
// The save directory now resides in the data directory (default or custom)
BOOLEAN InitSaveDir()
{
	// Look for a custom data dir first
	std::string dataDir = gCustomDataCat.GetRootDir();
	if( dataDir.empty() || FileGetAttributes( (STR) dataDir.c_str() ) == 0xFFFFFFFF ) {
		dataDir = gDefaultDataCat.GetRootDir();
	}

	// The locale-specific save dir location is of the form L"..\\SavedGames"
	// This has not changed; instead, we strip the ".." at the beginning
	if(is_networked)
	{
		sprintf(	gSaveDir, "%s%S", dataDir.c_str(), pMessageStrings[ MSG_MPSAVEDIRECTORY	 ] + 2 );
	}
	else
		sprintf(	gSaveDir, "%s%S", dataDir.c_str(), pMessageStrings[ MSG_SAVEDIRECTORY ] + 2 );

	// This was moved here from SaveGame
	//Check to see if the save directory exists
	if( FileGetAttributes( (STR) gSaveDir ) ==	0xFFFFFFFF )
	{
		//ok the direcotry doesnt exist, create it
		if( !MakeFileManDirectory( (CHAR8 *)gSaveDir ) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

// WDS - Automatically try to save when an assertion failure occurs
extern bool alreadySaving = false;


BOOLEAN SaveGame( int ubSaveGameID, STR16 pGameDesc )
{
	UINT32	uiNumBytesWritten=0;
	HWFILE	hFile=0;
	SAVED_GAME_HEADER SaveGameHeader;
	CHAR8		zSaveGameName[ MAX_PATH ];
	//UINT8		saveDir[100];
	BOOLEAN	fPausedStateBeforeSaving = gfGamePaused;
	BOOLEAN	fLockPauseStateBeforeSaving = gfLockPauseState;
	INT32		iSaveLoadGameMessageBoxID = -1;
	UINT16	usPosX, usActualWidth, usActualHeight;
	BOOLEAN fWePausedIt = FALSE;


	//sprintf(	saveDir, "%S", pMessageStrings[ MSG_SAVEDIRECTORY ] );

#ifdef JA2BETAVERSION
#ifndef CRIPPLED_VERSION
	//AssertMsg( uiSizeOfGeneralInfo == 1024, String( "Saved General info is NOT 1024, it is %d.	DF 1.", uiSizeOfGeneralInfo ) );
	//AssertMsg( sizeof( LaptopSaveInfoStruct ) == 7440, String( "LaptopSaveStruct is NOT 7440, it is %d.	DF 1.", sizeof( LaptopSaveInfoStruct ) ) );
#endif
#endif

	if( ubSaveGameID >= NUM_SAVE_GAMES && ubSaveGameID < EARLIST_SPECIAL_SAVE )
		return( FALSE );		//ddd
	alreadySaving = true;

	//clear out the save game header
	memset( &SaveGameHeader, 0, sizeof( SAVED_GAME_HEADER ) );

	if ( !GamePaused() )
	{
		PauseBeforeSaveGame();
		fWePausedIt = TRUE;
	}


	#ifdef JA2BETAVERSION
		InitShutDownMapTempFileTest( TRUE, "SaveMapTempFile", ubSaveGameID );
	#endif


	//Place a message on the screen telling the user that we are saving the game
	iSaveLoadGameMessageBoxID = PrepareMercPopupBox( iSaveLoadGameMessageBoxID, BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, zSaveLoadText[ SLG_SAVING_GAME_MESSAGE ], 300, 0, 0, 0, &usActualWidth, &usActualHeight);
	usPosX = ( SCREEN_WIDTH - usActualWidth ) / 2 ;

	RenderMercPopUpBoxFromIndex( iSaveLoadGameMessageBoxID, usPosX, iScreenHeightOffset + 160, FRAME_BUFFER );

	InvalidateRegion(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

	ExecuteBaseDirtyRectQueue( );
	EndFrameBufferRender( );
	RefreshScreen( NULL );

	if( RemoveMercPopupBoxFromIndex( iSaveLoadGameMessageBoxID ) )
	{
		iSaveLoadGameMessageBoxID = -1;
	}

	//
	// make sure we redraw the screen when we are done
	//

	//if we are in the game screen
	if( guiCurrentScreen == GAME_SCREEN )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	else if( guiCurrentScreen == MAP_SCREEN )
	{
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}

	else if( guiCurrentScreen == SAVE_LOAD_SCREEN )
	{
		gfRedrawSaveLoadScreen = TRUE;
	}




	gubSaveGameLoc = ubSaveGameID;

#ifdef JA2BETAVERSION
	InitSaveGameFilePosition();
#endif


	//Set the fact that we are saving a game
	gTacticalStatus.uiFlags |= LOADING_SAVED_GAME;


	//ADB this has been moved ahead of SaveCurrentSectorsInformationToTempItemFile
	//there was a problem loading savegames if the savegame version changed, because
	//the encryption algorithm uses the version of the header
	//
	// Setup the save game header
	//

	SaveGameHeader.uiSavedGameVersion = SAVE_GAME_VERSION;
	wcscpy( SaveGameHeader.sSavedGameDesc, pGameDesc );
	strcpy( SaveGameHeader.zGameVersionNumber, czVersionNumber );

	SaveGameHeader.uiFlags;

	//The following will be used to quickly access info to display in the save/load screen
	SaveGameHeader.uiDay = GetWorldDay();
	SaveGameHeader.ubHour = (UINT8)GetWorldHour();
	SaveGameHeader.ubMin = (UINT8)guiMin;
	if(is_networked)
	{
		SaveGameHeader.uiDay = CLIENT_NUM;
		SaveGameHeader.ubHour = MAX_CLIENTS;
		SaveGameHeader.ubMin = MAX_MERCS;
	}

	//copy over the initial game options
	memcpy( &SaveGameHeader.sInitialGameOptions, &gGameOptions, sizeof( GAME_OPTIONS ) );


	SaveGameHeader.ubNumOfMercsOnPlayersTeam = NumberOfMercsOnPlayerTeam();
	SaveGameHeader.iCurrentBalance = LaptopSaveInfo.iCurrentBalance;


	SaveGameHeader.uiCurrentScreen = guiPreviousOptionScreen;

	SaveGameHeader.fAlternateSector = GetSectorFlagStatus( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, SF_USE_ALTERNATE_MAP );

	if( gfWorldLoaded )
	{
		SaveGameHeader.fWorldLoaded = TRUE;
		SaveGameHeader.ubLoadScreenID = GetLoadScreenID( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	}
	else
	{
		SaveGameHeader.fWorldLoaded = FALSE;
		SaveGameHeader.ubLoadScreenID = 0;
	}

	SaveGameHeader.uiRandom = Random( RAND_MAX );

	// CHRISL: We need to know what inventory system we're using early on
	SaveGameHeader.ubInventorySystem = gGameOptions.ubInventorySystem;

	//
	// Save the Save Game header file
	//
	guiJA2EncryptionSet = CalcJA2EncryptionSet( &SaveGameHeader );


	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("starting SaveCurrentSectorsInformationToTempItemFile" ) );
	//Save the current sectors open temp files to the disk
	if( !SaveCurrentSectorsInformationToTempItemFile() )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR in SaveCurrentSectorsInformationToTempItemFile()");
		goto FAILED_TO_SAVE;
	}



	//if we are saving the quick save,
	if( ubSaveGameID == 0 )
	{
#ifdef JA2BETAVERSION
		//Increment the quicksave counter
		guiCurrentQuickSaveNumber++;

		if( gfUseConsecutiveQuickSaveSlots )
			swprintf( pGameDesc, L"%s%03d", pMessageStrings[ MSG_QUICKSAVE_NAME ], guiCurrentQuickSaveNumber );
		else
#endif
			swprintf( pGameDesc, pMessageStrings[ MSG_QUICKSAVE_NAME ] );
		wcscpy(SaveGameHeader.sSavedGameDesc, pGameDesc);
	}

	//If there was no string, add one
	if( pGameDesc[0] == '\0' )
		wcscpy( pGameDesc, pMessageStrings[ MSG_NODESC ] );

	//Check to see if the save directory exists
	/*if( FileGetAttributes( (STR) saveDir ) ==	0xFFFFFFFF )
	{
		//ok the direcotry doesnt exist, create it
		if( !MakeFileManDirectory( (CHAR8 *)saveDir ) ) 
		{
			goto FAILED_TO_SAVE;
		}
	}*/

	//Create the name of the file
	CreateSavedGameFileNameFromNumber( ubSaveGameID, zSaveGameName );

	//if the file already exists, delete it
	if( FileExists( zSaveGameName ) )
	{
		if( !FileDelete( zSaveGameName ) )
		{
			ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR deleting old save");
			goto FAILED_TO_SAVE;
		}
	}

	// create the save game file
	hFile = FileOpen( zSaveGameName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if( !hFile )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR creating new save");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Just Opened File" );
	#endif





	//
	// If there are no enemy or civilians to save, we have to check BEFORE savinf the sector info struct because
	// the NewWayOfSavingEnemyAndCivliansToTempFile will RESET the civ or enemy flag AFTER they have been saved. 
	//
	NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, TRUE, TRUE );
	NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, FALSE, TRUE );







	//Get the sector value to save.
	GetBestPossibleSectorXYZValues( &SaveGameHeader.sSectorX, &SaveGameHeader.sSectorY, &SaveGameHeader.bSectorZ );

/*

	//if the current sector is valid
	if( gfWorldLoaded )
	{
		SaveGameHeader.sSectorX = gWorldSectorX;
		SaveGameHeader.sSectorY = gWorldSectorY;
		SaveGameHeader.bSectorZ = gbWorldSectorZ;
	}
	else if( Squad[ iCurrentTacticalSquad ][ 0 ] && iCurrentTacticalSquad != NO_CURRENT_SQUAD )
	{
//		if( Squad[ iCurrentTacticalSquad ][ 0 ]->bAssignment != IN_TRANSIT )
		{
			SaveGameHeader.sSectorX = Squad[ iCurrentTacticalSquad ][ 0 ]->sSectorX;
			SaveGameHeader.sSectorY = Squad[ iCurrentTacticalSquad ][ 0 ]->sSectorY;
			SaveGameHeader.bSectorZ = Squad[ iCurrentTacticalSquad ][ 0 ]->bSectorZ;
		}
	}
	else
	{
		INT16					sSoldierCnt;
		SOLDIERTYPE		*pSoldier;
		INT16					bLastTeamID;
		INT8					bCount=0;
		BOOLEAN				fFoundAMerc=FALSE;

		// Set locator to first merc
		sSoldierCnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

		for ( pSoldier = MercPtrs[ sSoldierCnt ]; sSoldierCnt <= bLastTeamID; sSoldierCnt++,pSoldier++)
		{
			if( pSoldier->bActive )
			{
				if ( pSoldier->bAssignment != IN_TRANSIT && !pSoldier->flags.fBetweenSectors)
				{
					SaveGameHeader.sSectorX = pSoldier->sSectorX;
					SaveGameHeader.sSectorY = pSoldier->sSectorY;
					SaveGameHeader.bSectorZ = pSoldier->bSectorZ;
					fFoundAMerc = TRUE;
					break;
				}
			}
		}

		if( !fFoundAMerc )
		{
			SaveGameHeader.sSectorX = gWorldSectorX;
			SaveGameHeader.sSectorY = gWorldSectorY;
			SaveGameHeader.bSectorZ = gbWorldSectorZ;
		}
	}
*/



	FileWrite( hFile, &SaveGameHeader, sizeof( SAVED_GAME_HEADER ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( SAVED_GAME_HEADER ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing save game header");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Save Game Header" );
	#endif




	//
	//Save the gTactical Status array, plus the curent secotr location
	//
	if( !SaveTacticalStatusToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing tactical status");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Tactical Status" );
	#endif





	// save the game clock info
	if( !SaveGameClock( hFile, fPausedStateBeforeSaving, fLockPauseStateBeforeSaving ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing game clock");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Game Clock" );
	#endif




	// save the strategic events
	if( !SaveStrategicEventsToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing strategic events");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Strategic Events" );
	#endif



	if( !SaveLaptopInfoToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing laptop info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Laptop Info" );
	#endif

	//
	// Save the merc profiles
	//
	if( !SaveMercProfiles( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing merc profiles");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Merc Profiles" );
	#endif




	//
	// Save the soldier structure
	//
	if( !SaveSoldierStructure( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing soldier structure");
		goto FAILED_TO_SAVE;
	}

	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Soldier Structure" );
	#endif


	//Save the Finaces Data file 
	if( !SaveFilesToSavedGame( FINANCES_DATA_FILE, hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing finances");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Finances Data File" );
	#endif



	//Save the history file
	if( !SaveFilesToSavedGame( HISTORY_DATA_FILE, hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing history");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "History file" );
	#endif



	//Save the Laptop File file
	if( !SaveFilesToSavedGame( FILES_DAT_FILE, hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing laptop files");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "The Laptop FILES file" );
	#endif




	//Save email stuff to save file
	if( !SaveEmailToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing email");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Email " );
	#endif



	//Save the strategic information
	if( !SaveStrategicInfoToSavedFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing strategic info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Strategic Information" );
	#endif




	//save the underground information
	if( !SaveUnderGroundSectorInfoToSaveGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing underground info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Underground Information" );
	#endif




	//save the squad info
	if( !SaveSquadInfoToSavedGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing squad info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Squad Info" );
	#endif




	if( !SaveStrategicMovementGroupsToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing strategic movements");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Strategic Movement Groups" );
	#endif





	//Save all the map temp files from the maps\temp directory into the saved game file
	if( !SaveMapTempFilesToSavedGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing map temp files");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "All the Map Temp files" );
	#endif





	if( !SaveQuestInfoToSavedGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing quest info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Quest Info" );
	#endif




	if( !SaveOppListInfoToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing opplist");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "OppList info" );
	#endif





	if( !SaveMapScreenMessagesToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing map screen messages");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "MapScreen Messages" );
	#endif





	if( !SaveNPCInfoToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing NPC info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "NPC Info" );
	#endif





	if( !SaveKeyTableToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing key table");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "KeyTable" );
	#endif




	if( !SaveTempNpcQuoteArrayToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing NPC quotes");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "NPC Temp Quote File" );
	#endif





	if( !SavePreRandomNumbersToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing pre random numbers");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "PreGenerated Random Files" );
	#endif





	if( !SaveSmokeEffectsToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing smoke effects");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Smoke Effect Structures" );
	#endif


	if( !SaveArmsDealerInventoryToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing arms dealer inventory");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Arms Dealers Inventory" );
	#endif




	if( !SaveGeneralInfo( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR general info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Misc. Info" );
	#endif



	if( !SaveMineStatusToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing mine status");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Mine Status" );
	#endif




	if( !SaveStrategicTownLoyaltyToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing town loyalty");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Town Loyalty" );
	#endif



	if( !SaveVehicleInformationToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing vehicle info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Vehicle Information" );
	#endif



	if( !SaveBulletStructureToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing bullet structure");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Bullet Information" );
	#endif



	if( !SavePhysicsTableToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing physics table");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Physics Table" );
	#endif





	if( !SaveAirRaidInfoToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing air raid info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Air Raid Info" );
	#endif


	if( !SaveTeamTurnsToTheSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing team turns");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Team Turn Info" );
	#endif



	if( !SaveExplosionTableToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing explosions");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Explosion Table" );
	#endif



	if( !SaveCreatureDirectives( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing creature directives");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Creature Spreading" );
	#endif


	if( !SaveStrategicStatusToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing strategic status");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Strategic Status" );
	#endif



	if( !SaveStrategicAI( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing strategic AI");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Strategic AI" );
	#endif


	if( !SaveLightEffectsToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing light effects");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Lighting Effects" );
	#endif


	if( !SaveWatchedLocsToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing watched locs");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Watched Locs Info" );
	#endif

	if( !SaveItemCursorToSavedGame( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing item cursor");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "ItemCursor Info" );
	#endif

	if( !SaveCivQuotesToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing civ quotes");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Civ Quote System" );
	#endif

	if( !SaveBackupNPCInfoToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing backup NPC info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Backed up NPC Info" );
	#endif

	if ( !SaveMeanwhileDefsFromSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing meanwhiles");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Meanwhile Definitions" );
	#endif

	// save meanwhiledefs

	if ( !SaveSchedules( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing schedules");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
		SaveGameFilePosition( FileGetPos( hFile ), "Schedules" );
	#endif

		// Save extra vehicle info
	if ( !NewSaveVehicleMovementInfoToSavedGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing vehicle movement info");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION

	SaveGameFilePosition( FileGetPos( hFile ), "Vehicle Movement Stuff" );
	#endif


	// Save contract renewal sequence stuff
	if ( !SaveContractRenewalDataToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing contract renewal");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
	SaveGameFilePosition( FileGetPos( hFile ), "Contract Renewal Data" );
	#endif


	// Save leave list stuff
	if ( !SaveLeaveItemList( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing leave items list");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
	SaveGameFilePosition( FileGetPos( hFile ), "leave list" );
	#endif


	//do the new way of saving bobbyr mail order items
	if( !NewWayOfSavingBobbyRMailOrdersToSaveGameFile( hFile ) )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing mail orders");
		goto FAILED_TO_SAVE;
	}
	#ifdef JA2BETAVERSION
	SaveGameFilePosition( FileGetPos( hFile ), "New way of saving Bobby R mailorders" );
	#endif

	// Dealtar: New shipment system data
	if(!gPostalService.SaveShipmentListToSaveGameFile(hFile))
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"ERROR writing mail orders");
		goto FAILED_TO_SAVE;
	}



	//Close the saved game file
	FileClose( hFile );


	//if we succesfully saved the game, mark this entry as the last saved game file
	if( ubSaveGameID <  EARLIST_SPECIAL_SAVE)
	{
		gGameSettings.bLastSavedGameSlot = ubSaveGameID;
	}

	//Save the save game settings
	SaveGameSettings();

	//
	// Display a screen message that the save was succesful
	//

	//if its the quick save slot
	if( ubSaveGameID == 0 )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, pMessageStrings[ MSG_SAVESUCCESS ] );
	}
//#ifdef JA2BETAVERSION
	else if( ubSaveGameID == SAVE__END_TURN_NUM )
	{
//		ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, pMessageStrings[ MSG_END_TURN_AUTO_SAVE ] );
	}
//#endif
	else
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, pMessageStrings[ MSG_SAVESLOTSUCCESS ] );
	}

	//restore the music mode
	SetMusicMode( gubMusicMode );

	//Unset the fact that we are saving a game
	gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;

	UnPauseAfterSaveGame();

	#ifdef JA2BETAVERSION
		InitShutDownMapTempFileTest( FALSE, "SaveMapTempFile", ubSaveGameID );
	#endif

	#ifdef JA2BETAVERSION
		ValidateSoldierInitLinks( 2 );
	#endif

	//Check for enough free hard drive space
	NextLoopCheckForEnoughFreeHardDriveSpace();

	alreadySaving = false;

	return( TRUE );

	//if there is an error saving the game
FAILED_TO_SAVE:

#ifdef JA2BETAVERSION
	SaveGameFilePosition( FileGetPos( hFile ), "Failed to Save!!!" );
#endif

	FileClose( hFile );

	if ( fWePausedIt )
	{
		UnPauseAfterSaveGame();
	}

	//Delete the failed attempt at saving
#if 0
	// 0verhaul:	Temporarily disabled to try to troubleshoot save game problems
	DeleteSaveGameNumber( ubSaveGameID );
#endif

	//Put out an error message
	ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, zSaveLoadText[SLG_SAVE_GAME_ERROR] );

	#ifdef JA2BETAVERSION
		InitShutDownMapTempFileTest( FALSE, "SaveMapTempFile", ubSaveGameID );
	#endif

	//Check for enough free hard drive space
	NextLoopCheckForEnoughFreeHardDriveSpace();

#ifdef JA2BETAVERSION
	if( fDisableDueToBattleRoster || fDisableMapInterfaceDueToBattle )
	{
		gubReportMapscreenLock = 2;
	}
#endif

	Assert(guiCurrentSaveGameVersion == SAVE_GAME_VERSION);
	guiCurrentSaveGameVersion = SAVE_GAME_VERSION;

	alreadySaving = false;
	return( FALSE );
}



UINT32 guiBrokenSaveGameVersion = 0;
extern int gEnemyPreservedTempFileVersion[256];
extern int gCivPreservedTempFileVersion[256];

BOOLEAN LoadSavedGame( int ubSavedGameID )
{
	HWFILE	hFile;
	SAVED_GAME_HEADER SaveGameHeader;
	UINT32	uiNumBytesRead=0;

	INT16 sLoadSectorX;
	INT16 sLoadSectorY;
	INT8 bLoadSectorZ;
	CHAR8		zSaveGameName[ MAX_PATH ];
	UINT32 uiRelStartPerc;
	UINT32 uiRelEndPerc;

#ifdef JA2BETAVERSION
	gfDisplaySaveGamesNowInvalidatedMsg = FALSE;
#endif

	uiRelStartPerc = uiRelEndPerc =0;

	TrashAllSoldiers( );

	//Empty the dialogue Queue cause someone could still have a quote in waiting
	EmptyDialogueQueue( );

	//If there is someone talking, stop them
	StopAnyCurrentlyTalkingSpeech( );

	ZeroAnimSurfaceCounts( );

	ShutdownNPCQuotes();

	//very small TODO
	//Bugfix = Stop the chopter sound


#ifdef JA2BETAVERSION
	//AssertMsg( uiSizeOfGeneralInfo == 1024, String( "Saved General info is NOT 1024, it is %d.	DF 1.", uiSizeOfGeneralInfo ) );
#endif

	//is it a valid save number
	if( ubSavedGameID >= NUM_SAVE_GAMES )
	{
		if( ubSavedGameID != SAVE__END_TURN_NUM )
			return( FALSE );
	}
	else if( !gbSaveGameArray[ ubSavedGameID ] )
		return( FALSE );


	#ifdef JA2BETAVERSION
		InitShutDownMapTempFileTest( TRUE, "LoadMapTempFile", ubSavedGameID );
	#endif


	//Used in mapescreen to disable the annoying 'swoosh' transitions
	gfDontStartTransitionFromLaptop = TRUE;

	// Reset timer callbacks
	gpCustomizableTimerCallback = NULL;

	gubSaveGameLoc = ubSavedGameID;
#ifdef JA2BETAVERSION
	InitLoadGameFilePosition();
#endif

	//Set the fact that we are loading a saved game
	gTacticalStatus.uiFlags |= LOADING_SAVED_GAME;

	//Trash the existing world.	This is done to ensure that if we are loading a game that doesn't have 
	//a world loaded, that we trash it beforehand -- else the player could theoretically enter that sector
	//where it would be in a pre-load state.	
	TrashWorld();

	// Lesh: To kill memory leaks during re-loading a saved game
	//		release strategic layer resources
	ShutdownStrategicLayer();
	FreeGlobalMessageList();

	//Deletes all the Temp files in the Maps\Temp directory
	InitTacticalSave( TRUE );

	// ATE; Added to empry dialogue q
	EmptyDialogueQueue( );

	//Create the name of the file
	CreateSavedGameFileNameFromNumber( ubSavedGameID, zSaveGameName );

	// open the save game file
	hFile = FileOpen( zSaveGameName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FileOpen failed" ) );
		FileClose( hFile );
		return(FALSE);
	}

	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Just Opened File" );
	#endif


	//Load the Save Game header file
	FileRead( hFile, &SaveGameHeader, sizeof( SAVED_GAME_HEADER ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( SAVED_GAME_HEADER ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Loading Save Game Header failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Save Game Header" );
	#endif

	guiJA2EncryptionSet = CalcJA2EncryptionSet( &SaveGameHeader );
	guiCurrentSaveGameVersion = SaveGameHeader.uiSavedGameVersion;
	guiBrokenSaveGameVersion = SaveGameHeader.uiSavedGameVersion;

	// CHRISL: We need to know what inventory system we're using early on
	if(SaveGameHeader.uiSavedGameVersion < NIV_SAVEGAME_DATATYPE_CHANGE)
		SaveGameHeader.ubInventorySystem = 0;
	gGameOptions.ubInventorySystem = SaveGameHeader.ubInventorySystem;
	if((UsingNewInventorySystem() == true))
	{
		if(IsNIVModeValid() == FALSE){
			// Only load NewInv in higher screen res
			FileClose( hFile );
			return(FALSE);
		}
		InitInventoryNew();
		InitNewInventorySystem();
		InitializeSMPanelCoordsNew();
		InitializeInvPanelCoordsNew();
	}
	else
	{
		InitInventoryOld();
		InitOldInventorySystem();
		InitializeSMPanelCoordsOld();
		InitializeInvPanelCoordsOld();
	}

	//if the player is loading up an older version of the game, and the person DOESNT have the cheats on, 
	if( guiCurrentSaveGameVersion < 65 && !CHEATER_CHEAT_LEVEL( ) )
	{
#ifdef JA2BETAVERSION
		gfDisplaySaveGamesNowInvalidatedMsg = TRUE;
#endif
		//Fail loading the save
		FileClose( hFile );
		return(FALSE);
	}

	//Store the loading screenID that was saved
	gubLastLoadingScreenID = SaveGameHeader.ubLoadScreenID;


/*
	if( !LoadGeneralInfo( hFile ) )
	{
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Misc info" );
	#endif
*/



	//Load the gtactical status structure plus the current sector x,y,z
	if( !LoadTacticalStatusFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadTacticalStatusFromSavedGame failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Tactical Status" );
	#endif


	//This gets reset by the above function
	gTacticalStatus.uiFlags |= LOADING_SAVED_GAME;


	//Load the game clock ingo
	if( !LoadGameClock( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadGameClock failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Game Clock" );
	#endif


	//if we are suppose to use the alternate sector
	if( SaveGameHeader.fAlternateSector )
	{
		SetSectorFlag( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, SF_USE_ALTERNATE_MAP );
		gfUseAlternateMap = TRUE;
	}


	for (int x = 0; x < 256; ++x) {
		gEnemyPreservedTempFileVersion[x] = guiCurrentSaveGameVersion;
		gCivPreservedTempFileVersion[x] = guiCurrentSaveGameVersion;
	}

	//if the world was loaded when saved, reload it, otherwise dont
	if( SaveGameHeader.fWorldLoaded || guiCurrentSaveGameVersion < 50 )
	{
		//Get the current world sector coordinates
		sLoadSectorX = gWorldSectorX;
		sLoadSectorY = gWorldSectorY;
		bLoadSectorZ = gbWorldSectorZ;

		// This will guarantee that the sector will be loaded
		gbWorldSectorZ = -1;


		//if we should load a sector ( if the person didnt just start the game game )
		if( ( gWorldSectorX != 0 ) && ( gWorldSectorY != 0 ) )
		{
			//Load the sector
			SetCurrentWorldSector( sLoadSectorX, sLoadSectorY, bLoadSectorZ );
		}
	}
	else
	{ //By clearing these values, we can avoid "in sector" checks -- at least, that's the theory.
		gWorldSectorX = gWorldSectorY = 0;

		//Since there is no 
		if( SaveGameHeader.sSectorX == -1 || SaveGameHeader.sSectorY == -1 || SaveGameHeader.bSectorZ == -1 )
			gubLastLoadingScreenID = LOADINGSCREEN_HELI;
		else
			gubLastLoadingScreenID = GetLoadScreenID( SaveGameHeader.sSectorX, SaveGameHeader.sSectorY, SaveGameHeader.bSectorZ );

		BeginLoadScreen();
	}

	CreateLoadingScreenProgressBar();
	SetProgressBarColor( 0, 0, 0, 150 );

#ifdef JA2BETAVERSION
	//set the font
	SetProgressBarMsgAttributes( 0, FONT12ARIAL, FONT_MCOLOR_WHITE, 0 );

	//
	// Set the tile so we don see the text come up
	//

	//if the world is unloaded, we must use the save buffer for the text
	if( SaveGameHeader.fWorldLoaded )
		SetProgressBarTextDisplayFlag( 0, TRUE, TRUE, FALSE );
	else
		SetProgressBarTextDisplayFlag( 0, TRUE, TRUE, TRUE );
#endif

	uiRelStartPerc = 0;

	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Strategic Events..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;

	//load the game events
	if( !LoadStrategicEventsFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicEventsFromSavedGame failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Strategic Events" );
	#endif


	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Laptop Info" );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadLaptopInfoFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadLaptopInfoFromSavedGame failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Laptop Info" );
	#endif



	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Merc Profiles..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;

	//
	// Load all the saved Merc profiles
	//
	if( !LoadSavedMercProfiles( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadSavedMercProfiles failed" ) );
		FileClose( hFile );
		return(FALSE);
	}

	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Merc Profiles" );
	#endif



	uiRelEndPerc += 30;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Soldier Structure..." );
	uiRelStartPerc = uiRelEndPerc;


	//
	// Load the soldier structure info
	// 
	if( !LoadSoldierStructure( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadSoldierStructure failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Soldier Structure" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Finances Data File..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//
	// Load the Finances Data and write it to a new file
	//
	if( !LoadFilesFromSavedGame( FINANCES_DATA_FILE, hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadFilesFromSavedGame FINANCES_DATA_FILE failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Finances Data File" );
	#endif




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"History File..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//
	// Load the History Data and write it to a new file
	//
	if( !LoadFilesFromSavedGame( HISTORY_DATA_FILE, hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadFilesFromSavedGame HISTORY_DATA_FILE failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "History File" );
	#endif




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"The Laptop FILES file..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//
	// Load the Files Data and write it to a new file
	//
	if( !LoadFilesFromSavedGame( FILES_DAT_FILE, hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadFilesFromSavedGame FILES_DAT_FILE failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "The Laptop FILES file" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Email..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	// Load the data for the emails
	if( !LoadEmailFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadEmailFromSavedGame failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Email" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Strategic Information..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	//Load the strategic Information
	if( !LoadStrategicInfoFromSavedFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicInfoFromSavedFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Strategic Information" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"UnderGround Information..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//Load the underground information
	if( !LoadUnderGroundSectorInfoFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadUnderGroundSectorInfoFromSavedGame failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "UnderGround Information" );
	#endif


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Squad Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	// Load all the squad info from the saved game file 
	if( !LoadSquadInfoFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadSquadInfoFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Squad Info" );
	#endif


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Strategic Movement Groups..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//Load the group linked list
	if( !LoadStrategicMovementGroupsFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicMovementGroupsFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Strategic Movement Groups" );
	#endif

	ValidateStrategicGroups();


	uiRelEndPerc += 30;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"All the Map Temp files..." );
	uiRelStartPerc = uiRelEndPerc;


	// Load all the map temp files from the saved game file into the maps\temp directory
	if( !LoadMapTempFilesFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadMapTempFilesFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "All the Map Temp files" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Quest Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadQuestInfoFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadQuestInfoFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Quest Info" );
	#endif


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"OppList Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	if( !LoadOppListInfoFromSavedGame( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadOppListInfoFromSavedGame failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "OppList Info" );
	#endif




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"MapScreen Messages..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadMapScreenMessagesFromSaveGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadMapScreenMessagesFromSaveGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "MapScreen Messages" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"NPC Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadNPCInfoFromSavedGameFile( hFile, guiCurrentSaveGameVersion ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadNPCInfoFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "NPC Info" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"KeyTable..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadKeyTableFromSaveedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadKeyTableFromSaveedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "KeyTable" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Npc Temp Quote File..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	if( !LoadTempNpcQuoteArrayToSaveGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadTempNpcQuoteArrayToSaveGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Npc Temp Quote File" );
	#endif




	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"PreGenerated Random Files..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	if( !LoadPreRandomNumbersFromSaveGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadPreRandomNumbersFromSaveGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "PreGenerated Random Files" );
	#endif




	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Smoke Effect Structures..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadSmokeEffectsFromLoadGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadSmokeEffectsFromLoadGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Smoke Effect Structures" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Arms Dealers Inventory..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadArmsDealerInventoryFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadArmsDealerInventoryFromSavedGameFile failed" ) );
		FileClose( hFile );
		return( FALSE );
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Arms Dealers Inventory" );
	#endif



	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Misc info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;

	if( !LoadGeneralInfo( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadGeneralInfo failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Misc info" );
	#endif

	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Mine Status..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( !LoadMineStatusFromSavedGameFile( hFile ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadMineStatusFromSavedGameFile failed" ) );
		FileClose( hFile );
		return(FALSE);
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Mine Status" );
	#endif




	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Town Loyalty..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;




	if( guiCurrentSaveGameVersion	>= 21 )
	{
		if( !LoadStrategicTownLoyaltyFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicTownLoyaltyFromSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Town Loyalty" );
		#endif
	}





	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Vehicle Information..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 22 )
	{
		if( !LoadVehicleInformationFromSavedGameFile( hFile, guiCurrentSaveGameVersion ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadVehicleInformationFromSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Vehicle Information" );
		#endif
	}



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Bullet Information..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 24 )
	{
		if( !LoadBulletStructureFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadBulletStructureFromSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Bullet Information" );
		#endif
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Physics table..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;




	if( guiCurrentSaveGameVersion	>= 24 )
	{
		if( !LoadPhysicsTableFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadPhysicsTableFromSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Physics table" );
		#endif
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Air Raid Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 24 )
	{
		if( !LoadAirRaidInfoFromSaveGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadAirRaidInfoFromSaveGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Air Raid Info" );
		#endif
	}



	uiRelEndPerc += 0;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Team Turn Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 24 )
	{
		if( !LoadTeamTurnsFromTheSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadTeamTurnsFromTheSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Team Turn Info" );
		#endif
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Explosion Table..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 25 )
	{
		if( !LoadExplosionTableFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadExplosionTableFromSavedGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Explosion Table" );
		#endif
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Creature Spreading..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;




	if( guiCurrentSaveGameVersion	>= 27 )
	{
		if( !LoadCreatureDirectives( hFile, guiCurrentSaveGameVersion ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadCreatureDirectives failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Creature Spreading" );
		#endif
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Strategic Status..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;




	if( guiCurrentSaveGameVersion	>= 28 )
	{
		if( !LoadStrategicStatusFromSaveGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicStatusFromSaveGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Strategic Status" );
		#endif
	}



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Strategic AI..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 31 )
	{
		if( !LoadStrategicAI( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadStrategicAI failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Strategic AI" );
		#endif
	}



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Lighting Effects..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 37 )
	{
		if( !LoadLightEffectsFromLoadGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadLightEffectsFromLoadGameFile failed" ) );
			FileClose( hFile );
			return(FALSE);
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Lighting Effects" );
		#endif
	}



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Watched Locs Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion	>= 38 )
	{
		if ( !LoadWatchedLocsFromSavedGame( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadWatchedLocsFromSavedGame failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Watched Locs Info" );
	#endif




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Item cursor Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	if( guiCurrentSaveGameVersion	>= 39 )
	{
		if ( !LoadItemCursorFromSavedGame( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadItemCursorFromSavedGame failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Item cursor Info" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Civ Quote System..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion >= 51 )
	{
		if( !LoadCivQuotesFromLoadGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadCivQuotesFromLoadGameFile failed" ) );
			FileClose( hFile );
			return FALSE;
		}
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Civ Quote System" );
	#endif




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Backed up NPC Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion >= 53 )
	{
		if( !LoadBackupNPCInfoFromSavedGameFile( hFile, guiCurrentSaveGameVersion ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadBackupNPCInfoFromSavedGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
	}
	#ifdef JA2BETAVERSION
		LoadGameFilePosition( FileGetPos( hFile ), "Backed up NPC Info" );
	#endif



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Meanwhile definitions..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion >= 58 )
	{
		if( !LoadMeanwhileDefsFromSaveGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadMeanwhileDefsFromSaveGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Meanwhile definitions" );
		#endif
	}
	else
	{
		memcpy( &gMeanwhileDef[gCurrentMeanwhileDef.ubMeanwhileID], &gCurrentMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) );
	}




	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Schedules..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion >= 59 )
	{
		// trash schedules loaded from map
		DestroyAllSchedulesWithoutDestroyingEvents();
		if ( !LoadSchedulesFromSave( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadSchedulesFromSave failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Schedules" );
		#endif
	}



	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Extra Vehicle Info..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion >= 61 )
	{
	if( guiCurrentSaveGameVersion < 84 )
	{
		if ( !LoadVehicleMovementInfoFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadVehicleMovementInfoFromSavedGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Extra Vehicle Info" );
		#endif
	}
	else
	{
		if ( !NewLoadVehicleMovementInfoFromSavedGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("NewLoadVehicleMovementInfoFromSavedGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Extra Vehicle Info" );
		#endif
	}
	}

	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Contract renewal sequence stuff..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	if( guiCurrentSaveGameVersion < 62 )
	{
		// the older games had a bug where this flag was never being set
		gfResetAllPlayerKnowsEnemiesFlags = TRUE;
	}

	if( guiCurrentSaveGameVersion >= 67 )
	{
		if ( !LoadContractRenewalDataFromSaveGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadContractRenewalDataFromSaveGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Contract renewal sequence stuff" );
		#endif
	}


	if( guiCurrentSaveGameVersion >= 70 )
	{
		if ( !LoadLeaveItemList( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadLeaveItemList failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "Leave List" );
		#endif
	}

	if( guiCurrentSaveGameVersion <= 73 )
	{
	// Patch vehicle fuel
	AddVehicleFuelToSave( );
	}


	if( guiCurrentSaveGameVersion >= 85 )
	{
		if ( !NewWayOfLoadingBobbyRMailOrdersToSaveGameFile( hFile ) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("NewWayOfLoadingBobbyRMailOrdersToSaveGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
		#ifdef JA2BETAVERSION
			LoadGameFilePosition( FileGetPos( hFile ), "New way of loading Bobby R mailorders" );
		#endif
	}

	//If there are any old Bobby R Mail orders, tranfer them to the new system
	if( guiCurrentSaveGameVersion < 85 )
	{
		HandleOldBobbyRMailOrders();
	}

	//Dealtar's Airport Externalization
	if( SaveGameHeader.uiSavedGameVersion >= 104 )
	{
		if ( !gPostalService.LoadShipmentListFromSaveGameFile(hFile) )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("gPostalService.LoadShipmentListFromSaveGameFile failed" ) );
			FileClose( hFile );
			return( FALSE );
		}
	}


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;



	//
	//Close the saved game file
	//
	FileClose( hFile );

	// ATE: Patch? Patch up groups.....( will only do for old saves.. )
	UpdatePersistantGroupsFromOldSave( guiCurrentSaveGameVersion );


	if( guiCurrentSaveGameVersion	<= 40 )
	{
		// Cancel all pending purchase orders for BobbyRay's.	Starting with version 41, the BR orders events are 
		// posted with the usItemIndex itself as the parameter, rather than the inventory slot index.	This was
		// done to make it easier to modify BR's traded inventory lists later on without breaking saves.
		CancelAllPendingBRPurchaseOrders();
	}


	//if the world is loaded, apply the temp files to the loaded map
	if( SaveGameHeader.fWorldLoaded || guiCurrentSaveGameVersion < 50 )
	{
		//the current sector has been updated, we need to temporarily change the game version we are loading!
		UINT32 tempVersion = guiCurrentSaveGameVersion;
		guiCurrentSaveGameVersion = SAVE_GAME_VERSION;

		// Load the current sectors Information From the temporary files
		if( !LoadCurrentSectorsInformationFromTempItemsFile() )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadCurrentSectorsInformationFromTempItemsFile failed" ) );
			InitExitGameDialogBecauseFileHackDetected();
			return( TRUE );
		}
		guiCurrentSaveGameVersion = tempVersion;
	}

	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;

	InitAI();

	//Update the mercs in the sector with the new soldier info
	UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//ReconnectSchedules();
	PostSchedules();


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//Reset the lighting level if we are outside
	if( gbWorldSectorZ == 0 )
		LightSetBaseLevel( GetTimeOfDayAmbientLightLevel() );

	//if we have been to this sector before
//	if( SectorInfo[ SECTOR( gWorldSectorX,gWorldSectorY) ].uiFlags & SF_ALREADY_VISITED )
	{
		//Reset the fact that we are loading a saved game
		gTacticalStatus.uiFlags &= ~LOADING_SAVED_GAME;
	}

	// CJC January 13: we can't do this because (a) it resets militia IN THE MIDDLE OF 
	// COMBAT, and (b) if we add militia to the teams while LOADING_SAVED_GAME is set,
	// the team counters will not be updated properly!!!
//	ResetMilitia();


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;

	//if the UI was locked in the saved game file
	if( gTacticalStatus.ubAttackBusyCount > 1 )
	{
		//Lock the ui
		SetUIBusy( (UINT8)gusSelectedSoldier );
	}
	// OJW - 20090403 - Fix Tactical Interface is still locked after quick-load
	else
	{
		UIHandleLUIEndLock(NULL); // this only takes action if it needs to
	}

	//Reset the shadow 
	SetFontShadow( DEFAULT_SHADOW );

#ifdef JA2BETAVERSION
	//AssertMsg( uiSizeOfGeneralInfo == 1024, String( "Saved General info is NOT 1024, it is %d.	DF 1.", uiSizeOfGeneralInfo ) );
#endif

	//if we succesfully LOADED! the game, mark this entry as the last saved game file
	gGameSettings.bLastSavedGameSlot		= ubSavedGameID;

	//Save the save game settings
	SaveGameSettings();


	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	//Reset the Ai Timer clock
	giRTAILastUpdateTime = 0;

	//if we are in tactical
	if( guiScreenToGotoAfterLoadingSavedGame == GAME_SCREEN )
	{
		//Initialize the current panel
		InitializeCurrentPanel( );

		SelectSoldier( gusSelectedSoldier, FALSE, TRUE );
	}

	uiRelEndPerc += 1;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Final Checks..." );
	RenderProgressBar( 0, 100 );
	uiRelStartPerc = uiRelEndPerc;


	// init extern faces
	InitalizeStaticExternalNPCFaces( );

	// load portraits
	LoadCarPortraitValues( );

	// OK, turn OFF show all enemies....
	gTacticalStatus.uiFlags&= (~SHOW_ALL_MERCS );
	gTacticalStatus.uiFlags &= ~SHOW_ALL_ITEMS ;

	#ifdef JA2BETAVERSION
		InitShutDownMapTempFileTest( FALSE, "LoadMapTempFile", ubSavedGameID );
	#endif

	if ( (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Setting attack busy count to 0 from load" ) );
#ifdef DEBUG_ATTACKBUSY
		OutputDebugString( "Resetting attack busy due to load game.\n");
#endif
		gTacticalStatus.ubAttackBusyCount = 0;
		gfPendingEnemies = CheckPendingEnemies();
	}

	// ATE: if we are within this window where skyridder was foobared, fix!
	if ( guiCurrentSaveGameVersion >= 61 && guiCurrentSaveGameVersion <= 65 )
	{
		SOLDIERTYPE				*pSoldier;
		MERCPROFILESTRUCT *pProfile;

		if ( !fSkyRiderSetUp )
		{
			// see if we can find him and remove him if so....
			pSoldier = FindSoldierByProfileID( SKYRIDER, FALSE );

			if ( pSoldier != NULL )
			{
				TacticalRemoveSoldier( pSoldier->ubID );
			}

			// add the pilot at a random location!
			pProfile = &(gMercProfiles[ SKYRIDER ]);
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
			}
		}
	}

	if ( guiCurrentSaveGameVersion < 68 )
	{
		// correct bVehicleUnderRepairID for all mercs
		UINT8	ubID;
		for( ubID = 0; ubID < MAXMERCS; ubID++ )
		{
			Menptr[ ubID ].bVehicleUnderRepairID = -1;
		}
	}

	if ( guiCurrentSaveGameVersion < 73 )
	{
		if( LaptopSaveInfo.fMercSiteHasGoneDownYet )
			LaptopSaveInfo.fFirstVisitSinceServerWentDown = 2;
	}


	//Update the MERC merc contract lenght.	Before save version 77 the data was stored in the SOLDIERTYPE, 
	//after 77 the data is stored in the profile
	if ( guiCurrentSaveGameVersion < 77 )
	{
		UpdateMercMercContractInfo();
	}


	if ( guiCurrentSaveGameVersion <= 89 )
	{
		// ARM: A change was made in version 89 where refuel site availability now also depends on whether the player has
		// airspace control over that sector.	To update the settings immediately, must call it here.
		UpdateRefuelSiteAvailability();
	}

	if( guiCurrentSaveGameVersion < 91 )
	{
		//update the amount of money that has been paid to speck
		CalcAproximateAmountPaidToSpeck();
	}

	gfLoadedGame = TRUE;

	uiRelEndPerc = 100;
	SetRelativeStartAndEndPercentage( 0, uiRelStartPerc, uiRelEndPerc, L"Done!" );
	RenderProgressBar( 0, 100 );

	RemoveLoadingScreenProgressBar();

	SetMusicMode( gMusicModeToPlay );

#ifndef JA2TESTVERSION
	RESET_CHEAT_LEVEL( );
#endif

#ifdef JA2BETAVERSION
	if( fDisableDueToBattleRoster || fDisableMapInterfaceDueToBattle )
	{
		gubReportMapscreenLock = 1;
	}
#endif

	// reset once-per-convo records for everyone in the loaded sector
	ResetOncePerConvoRecordsForAllNPCsInLoadedSector();

	if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		// fix lingering attack busy count problem on loading saved game by resetting a.b.c
		// if we're not in combat.
		gTacticalStatus.ubAttackBusyCount = 0;
#ifdef DEBUG_ATTACKBUSY
		OutputDebugString( "Resetting attack busy due to load game.\n");
#endif
	}

	// fix squads
	CheckSquadMovementGroups();
	// HEADROCK HAM B1: Re-Adjust Dynamic Roaming Militia restrictions
	if (gGameExternalOptions.bDynamicRestrictRoaming)
	{
		AdjustRoamingRestrictions();
	}

	//The above function LightSetBaseLevel adjusts ALL the level node light values including the merc node,
	//we must reset the values
	HandlePlayerTogglingLightEffects( FALSE );

	//now change the savegame format so that temp files are saved and loaded correctly
	guiCurrentSaveGameVersion = SAVE_GAME_VERSION;
	return( TRUE );
}







BOOLEAN SaveMercProfiles( HWFILE hFile )
{
	UINT16	cnt;
	//Loop through all the profiles to save
	for( cnt=0; cnt< NUM_PROFILES; cnt++)
	{
		if ( !gMercProfiles[ cnt ].Save(hFile) )
		{
			return FALSE;
		}
	}

	return( TRUE );
}



BOOLEAN	LoadSavedMercProfiles( HWFILE hFile )
{
	UINT16	cnt;
	//Loop through all the profiles to Load
	for( cnt=0; cnt< NUM_PROFILES; cnt++)
	{
		// Changed by ADB, rev 1513
		//if ( !gMercProfiles[cnt].Load(hFile, false) )
		if ( !gMercProfiles[cnt].Load(hFile, false, false, true) )
		{
			return(FALSE);
		}
	}

	return( TRUE );
}



//Not saving any of these in the soldier struct

//	struct TAG_level_node				*pLevelNode;
//	struct TAG_level_node				*pExternShadowLevelNode;
//	struct TAG_level_node				*pRoofUILevelNode;
//	UINT16											*pBackGround;
//	UINT16											*pZBackground;
//	UINT16											*pForcedShade;
//
// 	UINT16											*pEffectShades[ NUM_SOLDIER_EFFECTSHADES ]; // Shading tables for effects
//	THROW_PARAMS								*pThrowParams;
//	UINT16											*pCurrentShade;
//	UINT16											*pGlowShades[ 20 ]; // 
//	UINT16											*pShades[ NUM_SOLDIER_SHADES ]; // Shading tables
//	UINT16											*p16BPPPalette;
//	SGPPaletteEntry							*p8BPPPalette
//	OBJECTTYPE									*pTempObject;

//Not saving any of these in the soldier struct

BOOLEAN SaveSoldierStructure( HWFILE hFile )
{
	UINT16	cnt;
	UINT32	uiNumBytesWritten=0;
	UINT8		ubOne = 1;
	UINT8		ubZero = 0;

	//Loop through all the soldier structs to save
	for( cnt=0; cnt< TOTAL_SOLDIERS; cnt++)
	{

		//if the soldier isnt active, dont add them to the saved game file.
		if( !Menptr[ cnt ].bActive )
		{
			// Save the byte specifing to NOT load the soldiers 
			FileWrite( hFile, &ubZero, 1, &uiNumBytesWritten );
			if( uiNumBytesWritten != 1 )
			{
				return(FALSE);
			}
		}

		else
		{
			// Save the byte specifing to load the soldiers 
			FileWrite( hFile, &ubOne, 1, &uiNumBytesWritten );
			if( uiNumBytesWritten != 1 )
			{
				return(FALSE);
			}

			// Save the soldier structure
			if ( !Menptr[ cnt ].Save(hFile) )
			{
				return FALSE;
			}

			//
			// Save all the pointer info from the structure
			//
			//Save the pMercPath
			if( !SaveMercPathFromSoldierStruct( hFile, (UINT8)cnt ) )
				return( FALSE );

			//
			//do we have a 	KEY_ON_RING									*pKeyRing;
			//

			if( Menptr[ cnt ].pKeyRing != NULL )
			{
				// write to the file saying we have the ....
				FileWrite( hFile, &ubOne, 1, &uiNumBytesWritten );
				if( uiNumBytesWritten != 1 )
				{
					return(FALSE);
				}

				// Now save the ....
				FileWrite( hFile, Menptr[ cnt ].pKeyRing, NUM_KEYS * sizeof( KEY_ON_RING ), &uiNumBytesWritten );
				if( uiNumBytesWritten != NUM_KEYS * sizeof( KEY_ON_RING ) )
				{
					return(FALSE);
				}
			}
			else
			{
				// write to the file saying we DO NOT have the Key ring
				FileWrite( hFile, &ubZero, 1, &uiNumBytesWritten );
				if( uiNumBytesWritten != 1 )
				{
					return(FALSE);
				}
			}
		}
	}
	return( TRUE );
}


BOOLEAN LoadSoldierStructure( HWFILE hFile )
{
	UINT16	cnt;
	UINT32	uiNumBytesRead=0;
	UINT8		ubId;
	UINT8		ubOne = 1;
	UINT8		ubActive = 1;
	UINT32	uiPercentage;
	SOLDIERTYPE SavedSoldierInfo;

	//Loop through all the soldier and delete them all
	for( cnt=0; cnt< TOTAL_SOLDIERS; cnt++)
	{
		TacticalRemoveSoldier( cnt );
	}

	//Loop through all the soldier structs to load
	for( cnt=0; cnt< TOTAL_SOLDIERS; cnt++)
	{
		//update the progress bar
		uiPercentage = (cnt * 100) / (TOTAL_SOLDIERS-1);
		RenderProgressBar( 0, uiPercentage );

		//Read in a byte to tell us whether or not there is a soldier loaded here.
		FileRead( hFile, &ubActive, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return(FALSE);
		}

		// if the soldier is not active, continue 
		if( !ubActive )
		{
			continue;
		}

		// else if there is a soldier 
		else
		{
			//Read in the saved soldier info into a Temp structure
			if ( !SavedSoldierInfo.Load(hFile) )
			{
				return FALSE;
			}

			//Make sure all the pointer references are NULL'ed out.	
			SavedSoldierInfo.pTempObject	= NULL;
			SavedSoldierInfo.pKeyRing	= NULL;
			SavedSoldierInfo.p8BPPPalette	= NULL;
			SavedSoldierInfo.p16BPPPalette	= NULL;
			memset( SavedSoldierInfo.pShades, 0, sizeof( UINT16* ) * NUM_SOLDIER_SHADES );
			memset( SavedSoldierInfo.pGlowShades, 0, sizeof( UINT16* ) * 20 );
			SavedSoldierInfo.pCurrentShade	= NULL;
			SavedSoldierInfo.pThrowParams	= NULL;
			SavedSoldierInfo.pLevelNode	= NULL;
			SavedSoldierInfo.pExternShadowLevelNode	= NULL;
			SavedSoldierInfo.pRoofUILevelNode	= NULL;
			SavedSoldierInfo.pBackGround	= NULL;
			SavedSoldierInfo.pZBackground	= NULL;
			SavedSoldierInfo.pForcedShade	= NULL;
			SavedSoldierInfo.pMercPath	= NULL;
			memset( SavedSoldierInfo.pEffectShades, 0, sizeof( UINT16* ) * NUM_SOLDIER_EFFECTSHADES );


			//if the soldier wasnt active, dont add them now.	Advance to the next merc
			//if( !SavedSoldierInfo.bActive )
			//	continue;


			//Create the new merc
			SOLDIERCREATE_STRUCT CreateStruct;
			CreateStruct.bTeam								= SavedSoldierInfo.bTeam;
			CreateStruct.ubProfile						= SavedSoldierInfo.ubProfile;
			CreateStruct.fUseExistingSoldier	= TRUE;
			CreateStruct.pExistingSoldier			= &SavedSoldierInfo;

			if( !TacticalCreateSoldier( &CreateStruct, &ubId ) )
				return( FALSE );


			// Load the pMercPath
			if( !LoadMercPathToSoldierStruct( hFile, ubId ) )
				return( FALSE );


			//
			//do we have a 	KEY_ON_RING									*pKeyRing;
			//

			// Read the file to see if we have to load the keys
			FileRead( hFile, &ubOne, 1, &uiNumBytesRead );
			if( uiNumBytesRead != 1 )
			{
				return(FALSE);
			}

			if( ubOne )
			{
				// Now Load the ....
				FileRead( hFile, Menptr[ cnt ].pKeyRing, NUM_KEYS * sizeof( KEY_ON_RING ), &uiNumBytesRead );
				if( uiNumBytesRead != NUM_KEYS * sizeof( KEY_ON_RING ) )
				{
					return(FALSE);
				}

			}
			else
			{
				Assert( Menptr[ cnt ].pKeyRing == NULL );
			}

			//if the soldier is an IMP character
			if( Menptr[cnt].ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER && Menptr[cnt].bTeam == gbPlayerNum )
			{
				ResetIMPCharactersEyesAndMouthOffsets( Menptr[ cnt ].ubProfile );
			}

			//if the saved game version is before x, calculate the amount of money paid to mercs
			if( guiCurrentSaveGameVersion < 83 )
			{
				//if the soldier is someone
				if( Menptr[ cnt ].ubProfile != NO_PROFILE )
				{
					if( Menptr[cnt].ubWhatKindOfMercAmI == MERC_TYPE__MERC )
					{
						gMercProfiles[ Menptr[ cnt ].ubProfile ].uiTotalCostToDate = gMercProfiles[ Menptr[ cnt ].ubProfile ].sSalary * gMercProfiles[ Menptr[ cnt ].ubProfile ].iMercMercContractLength;
					}
					else
					{
						gMercProfiles[ Menptr[ cnt ].ubProfile ].uiTotalCostToDate = gMercProfiles[ Menptr[ cnt ].ubProfile ].sSalary * Menptr[ cnt ].iTotalContractLength;
					}
				}
			}

#ifdef GERMAN
			// Fix neutral flags
			if ( guiCurrentSaveGameVersion < 94 )
			{
				if ( Menptr[ cnt].bTeam == OUR_TEAM && Menptr[ cnt ].aiData.bNeutral && Menptr[ cnt ].bAssignment != ASSIGNMENT_POW )
				{
					// turn off neutral flag
					Menptr[ cnt].aiData.bNeutral = FALSE;
				}
			}
#endif
			// JA2Gold: fix next-to-previous attacker value
			if ( guiCurrentSaveGameVersion < 99 )
			{
				Menptr[ cnt ].ubNextToPreviousAttackerID = NOBODY;
			}

		}
	}

	// Fix robot
	if ( guiCurrentSaveGameVersion <= 87 )
	{
		SOLDIERTYPE * pSoldier;

		if ( gMercProfiles[ ROBOT ].inv[ VESTPOS ] == SPECTRA_VEST )
		{
			// update this
			gMercProfiles[ ROBOT ].inv[ VESTPOS ] = SPECTRA_VEST_18;
			gMercProfiles[ ROBOT ].inv[ HELMETPOS ] = SPECTRA_HELMET_18;
			gMercProfiles[ ROBOT ].inv[ LEGPOS ] = SPECTRA_LEGGINGS_18;
			gMercProfiles[ ROBOT ].bAgility = 50;
			pSoldier = FindSoldierByProfileID( ROBOT, FALSE );
			if ( pSoldier )
			{
				pSoldier->inv[ VESTPOS ].usItem = SPECTRA_VEST_18;
				pSoldier->inv[ HELMETPOS ].usItem = SPECTRA_HELMET_18;
				pSoldier->inv[ LEGPOS ].usItem = SPECTRA_LEGGINGS_18;
				pSoldier->stats.bAgility = 50;
			}
		}
	}

	return( TRUE );
}


/*
BOOLEAN SavePtrInfo( PTR *pData, UINT32 uiSizeOfObject, HWFILE hFile )
{
	UINT8		ubOne = 1;
	UINT8		ubZero = 0;
	UINT32	uiNumBytesWritten;

	if( pData != NULL )
	{
		// write to the file saying we have the ....
		FileWrite( hFile, &ubOne, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Write Soldier Structure to File" ) );
			return(FALSE);
		}

		// Now save the ....
		FileWrite( hFile, pData, uiSizeOfObject, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSizeOfObject )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Write Soldier Structure to File" ) );
			return(FALSE);
		}
	}
	else
	{
		// write to the file saying we DO NOT have the ...
		FileWrite( hFile, &ubZero, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Write Soldier Structure to File" ) );
			return(FALSE);
		}
	}

	return( TRUE );
}



BOOLEAN LoadPtrInfo( PTR *pData, UINT32 uiSizeOfObject, HWFILE hFile )
{
	UINT8		ubOne = 1;
	UINT8		ubZero = 0;
	UINT32	uiNumBytesRead;

	// Read the file to see if we have to load the ....
	FileRead( hFile, &ubOne, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Read Soldier Structure from File" ) );
		return(FALSE);
	}

	if( ubOne )
	{
		// if there is memory already allocated, free it
		MemFree( pData );

		//Allocate space for the structure data
		*pData = MemAlloc( uiSizeOfObject );
		if( pData == NULL )
			return( FALSE );

		// Now Load the ....
		FileRead( hFile, pData, uiSizeOfObject, &uiNumBytesRead );
		if( uiNumBytesRead != uiSizeOfObject )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Write Soldier Structure to File" ) );
			return(FALSE);
		}
	}
	else
	{
		// if there is memory already allocated, free it
		if( pData )
		{
			MemFree( pData );
			pData = NULL;
		}
	}


	return( TRUE );
}
*/

BOOLEAN SaveFilesToSavedGame( STR pSrcFileName, HWFILE hFile )
{
	UINT32	uiFileSize;
	UINT32	uiNumBytesWritten=0;
	HWFILE	hSrcFile;
	UINT8		*pData;
	UINT32	uiNumBytesRead;


	//open the file
	hSrcFile = FileOpen( pSrcFileName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( !hSrcFile )
	{
		return( FALSE );
	}

	#ifdef JA2BETAVERSION
	guiNumberOfMapTempFiles++;		//Increment counter:	To determine where the temp files are crashing
	#endif


	//Get the file size of the source data file
	uiFileSize = FileGetSize( hSrcFile );
	if( uiFileSize == 0 )
		return( FALSE );

	// Write the the size of the file to the saved game file
	FileWrite( hFile, &uiFileSize, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return(FALSE);
	}



	//Allocate a buffer to read the data into
	pData = (UINT8 *) MemAlloc( uiFileSize );
	if( pData == NULL )
		return( FALSE );
	memset( pData, 0, uiFileSize);

	// Read the saource file into the buffer
	FileRead( hSrcFile, pData, uiFileSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiFileSize )
	{
		//Free the buffer
		MemFree( pData );

		return(FALSE);
	}



	// Write the buffer to the saved game file
	FileWrite( hFile, pData, uiFileSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiFileSize )
	{
		//Free the buffer
		MemFree( pData );

		return(FALSE);
	}

	//Free the buffer
	MemFree( pData );

	//Clsoe the source data file
	FileClose( hSrcFile );

	#ifdef JA2BETAVERSION
	//Write out the name of the temp file so we can track whcih ones get loaded, and saved
		WriteTempFileNameToFile( pSrcFileName, uiFileSize, hFile );
	#endif

	return( TRUE );
}


BOOLEAN LoadFilesFromSavedGame( STR pSrcFileName, HWFILE hFile )
{
	UINT32	uiFileSize;
	UINT32	uiNumBytesWritten=0;
	HWFILE	hSrcFile;
	UINT8		*pData;
	UINT32	uiNumBytesRead;



	//If the source file exists, delete it
	if( FileExists( pSrcFileName ) )
	{
		if( !FileDelete( pSrcFileName ) )
		{
			//unable to delete the original file
			return( FALSE );
		}
	}

	#ifdef JA2BETAVERSION
	guiNumberOfMapTempFiles++;		//Increment counter:	To determine where the temp files are crashing
	#endif

	//open the destination file to write to
	hSrcFile = FileOpen( pSrcFileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if( !hSrcFile )
	{
		//error, we cant open the saved game file
		return( FALSE );
	}


	// Read the size of the data 
	FileRead( hFile, &uiFileSize, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		FileClose( hSrcFile );

		return(FALSE);
	}


	//if there is nothing in the file, return;
	if( uiFileSize == 0 )
	{
		FileClose( hSrcFile );
		return( TRUE );
	}

	//Allocate a buffer to read the data into
	pData = (UINT8 *) MemAlloc( uiFileSize );
	if( pData == NULL )
	{
		FileClose( hSrcFile );
		return( FALSE );
	}
	//ADB looks hardly necessary if there is a read right below
	//memset( pData, 0, uiFileSize);

	// Read into the buffer
	FileRead( hFile, pData, uiFileSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiFileSize )
	{
		FileClose( hSrcFile );

		//Free the buffer
		MemFree( pData );

		return(FALSE);
	}



	// Write the buffer to the new file
	FileWrite( hSrcFile, pData, uiFileSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiFileSize )
	{
		FileClose( hSrcFile );
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Write to the %s File", pSrcFileName ) );
		//Free the buffer
		MemFree( pData );

		return(FALSE);
	}

	//Free the buffer
	MemFree( pData );

	//Close the source data file
	FileClose( hSrcFile );

	#ifdef JA2BETAVERSION
		WriteTempFileNameToFile( pSrcFileName, uiFileSize, hFile );
	#endif
	return( TRUE );
}


BOOLEAN SaveEmailToSavedGame( HWFILE hFile )
{
	UINT32	uiNumOfEmails=0;
	UINT32		uiSizeOfEmails=0;
	EmailPtr	pEmail = pEmailList;
	UINT32	cnt;
	UINT32	uiStringLength=0;
	UINT32	uiNumBytesWritten=0;

	SavedEmailStruct SavedEmail;

	//loop through all the email to find out the total number
	while(pEmail)
	{
		pEmail=pEmail->Next;
		uiNumOfEmails++;
	}

	uiSizeOfEmails = sizeof( Email ) * uiNumOfEmails;

	//write the number of email messages
	FileWrite( hFile, &uiNumOfEmails, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return(FALSE);
	}


	//loop trhough all the emails, add each one individually
	pEmail = pEmailList;
	for( cnt=0; cnt<uiNumOfEmails; cnt++)
	{
		//Get the strng length of the subject
		uiStringLength = ( wcslen( pEmail->pSubject ) + 1 ) * 2;

		//write the length of the current emails subject to the saved game file
		FileWrite( hFile, &uiStringLength, sizeof( UINT32 ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( UINT32 ) )
		{
			return(FALSE);
		}

		//write the subject of the current email to the saved game file
		FileWrite( hFile, pEmail->pSubject, uiStringLength, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiStringLength )
		{
			return(FALSE);
		}


		//Get the current emails data and asign it to the 'Saved email' struct
		SavedEmail.usOffset = pEmail->usOffset;
		SavedEmail.usLength = pEmail->usLength;
		SavedEmail.ubSender = pEmail->ubSender;
		SavedEmail.iDate = pEmail->iDate;
		SavedEmail.iId = pEmail->iId;
		SavedEmail.iFirstData = pEmail->iFirstData;
		SavedEmail.uiSecondData = pEmail->uiSecondData;
		SavedEmail.fRead = pEmail->fRead;
		SavedEmail.fNew = pEmail->fNew;
		SavedEmail.iThirdData = pEmail->iThirdData;
		SavedEmail.iFourthData = pEmail->iFourthData;
		SavedEmail.uiFifthData = pEmail->uiFifthData;
		SavedEmail.uiSixData = pEmail->uiSixData;
		SavedEmail.iCurrentIMPPosition = pEmail->iCurrentIMPPosition;

		// write the email header to the saved game file
		FileWrite( hFile, &SavedEmail, sizeof( SavedEmailStruct ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( SavedEmailStruct ) )
		{
			return(FALSE);
		}

		//advance to the next email
		pEmail = pEmail->Next;
	}

	return( TRUE );
}


BOOLEAN LoadEmailFromSavedGame( HWFILE hFile )
{
	UINT32		uiNumOfEmails=0;
	UINT32		uiSizeOfSubject=0;
	EmailPtr	pEmail = pEmailList;
	EmailPtr	pTempEmail = NULL;
	UINT8			*pData = NULL;
	UINT32		cnt;
	SavedEmailStruct SavedEmail;
	UINT32		uiNumBytesRead=0;

	//Delete the existing list of emails
	ShutDownEmailList();

	pEmailList = NULL;
	//Allocate memory for the header node
	pEmailList = (EmailPtr) MemAlloc( sizeof( Email ) );
	if( pEmailList == NULL )
		return( FALSE );

	memset( pEmailList, 0, sizeof( Email ) );

	//read in the number of email messages
	FileRead( hFile, &uiNumOfEmails, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return(FALSE);
	}

	//loop through all the emails, add each one individually
	pEmail = pEmailList;
	for( cnt=0; cnt<uiNumOfEmails; cnt++)
	{
		//get the length of the email subject
		FileRead( hFile, &uiSizeOfSubject, sizeof( UINT32 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT32 ) )
		{
			return(FALSE);
		}

		//allocate space for the subject
		pData = (UINT8 *) MemAlloc( EMAIL_SUBJECT_LENGTH * sizeof( CHAR16 ) );
		if( pData == NULL )
			return( FALSE );
		memset( pData, 0, EMAIL_SUBJECT_LENGTH * sizeof( CHAR16 ) );

		//Get the subject
		FileRead( hFile, pData, uiSizeOfSubject, &uiNumBytesRead );
		if( uiNumBytesRead != uiSizeOfSubject )
		{
			return(FALSE);
		}

		//get the rest of the data from the email
		FileRead( hFile, &SavedEmail, sizeof( SavedEmailStruct ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( SavedEmailStruct ) )
		{
			return(FALSE);
		}

		//
		//add the email
		//

		//if we havent allocated space yet
		pTempEmail = (EmailPtr) MemAlloc( sizeof( Email ) );
		if( pTempEmail == NULL )
			return( FALSE );
		memset( pTempEmail, 0, sizeof( Email ) );

		pTempEmail->usOffset = SavedEmail.usOffset;
		pTempEmail->usLength = SavedEmail.usLength;
		pTempEmail->ubSender = SavedEmail.ubSender;
		pTempEmail->iDate = SavedEmail.iDate;
		pTempEmail->iId = SavedEmail.iId;
		pTempEmail->fRead = SavedEmail.fRead;
		pTempEmail->fNew = SavedEmail.fNew;
		pTempEmail->pSubject = (STR16) pData;
		pTempEmail->iFirstData = SavedEmail.iFirstData;
		pTempEmail->uiSecondData = SavedEmail.uiSecondData;
		pTempEmail->iThirdData = SavedEmail.iThirdData;
		pTempEmail->iFourthData = SavedEmail.iFourthData;
		pTempEmail->uiFifthData = SavedEmail.uiFifthData;
		pTempEmail->uiSixData = SavedEmail.uiSixData;
		pTempEmail->iCurrentIMPPosition = SavedEmail.iCurrentIMPPosition;

		//add the current email in
		pEmail->Next = pTempEmail;
		pTempEmail->Prev = pEmail;

		//moved to the next email
		pEmail = pEmail->Next;

		AddMessageToPages( pTempEmail->iId );

	}

	//if there are emails
	if( cnt )
	{
		//the first node of the LL was a dummy, node,get rid	of it
		pTempEmail = pEmailList;
		pEmailList = pEmailList->Next;
		pEmailList->Prev = NULL;
		MemFree( pTempEmail );
	}
	else
	{
		MemFree( pEmailList );
		pEmailList = NULL;
	}

	return( TRUE );
}


BOOLEAN SaveTacticalStatusToSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	//write the gTacticalStatus to the saved game file
	FileWrite( hFile, &gTacticalStatus, sizeof( TacticalStatusType ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( TacticalStatusType ) )
	{
		return(FALSE);
	}

	//
	//Save the current sector location to the saved game file
	//

	// save gWorldSectorX
	FileWrite( hFile, &gWorldSectorX, sizeof( gWorldSectorX ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gWorldSectorX ) )
	{
		return(FALSE);
	}


	// save gWorldSectorY
	FileWrite( hFile, &gWorldSectorY, sizeof( gWorldSectorY ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gWorldSectorY ) )
	{
		return(FALSE);
	}


	// save gbWorldSectorZ
	FileWrite( hFile, &gbWorldSectorZ, sizeof( gbWorldSectorZ ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gbWorldSectorZ ) )
	{
		return(FALSE);
	}

	return( TRUE );
}

void FailedLoadingGameCallBack( UINT8 bExitValue );


// WDS
// This was going to be used to allow for increasing the sizes of teams once a game started but it is unfinished
//TacticalTeamType savedTeamSettings[MAXTEAMS];


BOOLEAN LoadTacticalStatusFromSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

//	for (unsigned idx=0; idx <= MAXTEAMS; ++idx) {
//		savedTeamSettings[idx] = gTacticalStatus.Team[idx];
//	}

	//Read the gTacticalStatus to the saved game file
	FileRead( hFile, &gTacticalStatus, sizeof( TacticalStatusType ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( TacticalStatusType ) ) {
		return(FALSE);
	}

//	for (unsigned idx=0; idx <= MAXTEAMS; ++idx) {
//		gTacticalStatus.Team[idx] = savedTeamSettings[idx];
//	}

	// WDS - make number of mercenaries, etc. be configurable
	// Check that the team lists match what we expect given the .ini settings
	int cntFromFile = gTacticalStatus.Team[ OUR_TEAM ].bLastID - gTacticalStatus.Team[ OUR_TEAM ].bFirstID + 1;
	int cntFromIni = gGameExternalOptions.ubGameMaximumNumberOfPlayerMercs + gGameExternalOptions.ubGameMaximumNumberOfPlayerVehicles;
	if (cntFromFile != cntFromIni) {
		CHAR16 errorMessage[512];
		swprintf(errorMessage, L"Internal error in reading mercenary/vehicle slots from save file: number of slots in save file (%d) differs from number of slots in .ini (%d)", cntFromFile, cntFromIni);
		DoScreenIndependantMessageBox(errorMessage, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
		return FALSE;
	}

	cntFromFile = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID - gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID + 1;
	cntFromIni = gGameExternalOptions.ubGameMaximumNumberOfEnemies;
	if (cntFromFile != cntFromIni) {
		CHAR16 errorMessage[512];
		swprintf(errorMessage, L"Internal error in reading enemy slots from save file: number of slots in save file (%d) differs from number of slots in .ini (%d)", cntFromFile, cntFromIni);
		DoScreenIndependantMessageBox(errorMessage, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
		return FALSE;
	}

	cntFromFile = gTacticalStatus.Team[ CREATURE_TEAM ].bLastID - gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID + 1;
	cntFromIni = gGameExternalOptions.ubGameMaximumNumberOfCreatures;
	if (cntFromFile != cntFromIni) {
		CHAR16 errorMessage[512];
		swprintf(errorMessage, L"Internal error in reading creature slots from save file: number of slots in save file (%d) differs from number of slots in .ini (%d)", cntFromFile, cntFromIni);
		DoScreenIndependantMessageBox(errorMessage, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
		return FALSE;
	}

	cntFromFile = gTacticalStatus.Team[ MILITIA_TEAM ].bLastID - gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID + 1;
	cntFromIni = gGameExternalOptions.ubGameMaximumNumberOfRebels;
	if (cntFromFile != cntFromIni) {
		CHAR16 errorMessage[512];
		swprintf(errorMessage, L"Internal error in reading militia slots from save file: number of slots in save file (%d) differs from number of slots in .ini (%d)", cntFromFile, cntFromIni);
		DoScreenIndependantMessageBox(errorMessage, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
		return FALSE;
	}

	cntFromFile = gTacticalStatus.Team[ CIV_TEAM ].bLastID - gTacticalStatus.Team[ CIV_TEAM ].bFirstID + 1;
	cntFromIni = gGameExternalOptions.ubGameMaximumNumberOfCivilians;
	if (cntFromFile != cntFromIni) {
		CHAR16 errorMessage[512];
		swprintf(errorMessage, L"Internal error in reading civilian slots from save file: number of slots in save file (%d) differs from number of slots in .ini (%d)", cntFromFile, cntFromIni);
		DoScreenIndependantMessageBox(errorMessage, MSG_BOX_FLAG_OK, FailedLoadingGameCallBack );
		return FALSE;
	}

	//
	//Load the current sector location to the saved game file
	//

	// Load gWorldSectorX
	FileRead( hFile, &gWorldSectorX, sizeof( gWorldSectorX ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gWorldSectorX ) )
	{
		return(FALSE);
	}


	// Load gWorldSectorY
	FileRead( hFile, &gWorldSectorY, sizeof( gWorldSectorY ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gWorldSectorY ) )
	{
		return(FALSE);
	}


	// Load gbWorldSectorZ
	FileRead( hFile, &gbWorldSectorZ, sizeof( gbWorldSectorZ ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gbWorldSectorZ ) )
	{
		return(FALSE);
	}

	return( TRUE );
}



BOOLEAN CopySavedSoldierInfoToNewSoldier( SOLDIERTYPE *pDestSourceInfo, SOLDIERTYPE *pSourceInfo )
{
	*pDestSourceInfo = *pSourceInfo;
	return( TRUE );
}


BOOLEAN SetMercsInsertionGridNo( )
{
	UINT16 cnt=0;

	// loop through all the mercs
	for ( cnt=0; cnt < TOTAL_SOLDIERS; cnt++ )
	{
		//if the soldier is active
		if( Menptr[ cnt ].bActive )
		{

			if( Menptr[ cnt ].sGridNo != NOWHERE )
			{
				//set the insertion type to gridno
				Menptr[ cnt ].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;

				//set the insertion gridno
				Menptr[ cnt ].usStrategicInsertionData = Menptr[ cnt ].sGridNo;

				//set the gridno
				Menptr[ cnt ].sGridNo = NOWHERE;
			}
		}
	}

	return( TRUE );
}


BOOLEAN SaveOppListInfoToSavedGame( HWFILE hFile )
{
	UINT32	uiSaveSize=0;
	UINT32	uiNumBytesWritten=0;


	// Save the Public Opplist
	uiSaveSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileWrite( hFile, gbPublicOpplist, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	// Save the Seen Oppenents
	uiSaveSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileWrite( hFile, gbSeenOpponents, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}



	// Save the Last Known Opp Locations
	uiSaveSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileWrite( hFile, gsLastKnownOppLoc, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	// Save the Last Known Opp Level
	uiSaveSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileWrite( hFile, gbLastKnownOppLevel, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}


	// Save the Public Last Known Opp Locations
	uiSaveSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileWrite( hFile, gsPublicLastKnownOppLoc, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	// Save the Public Last Known Opp Level
	uiSaveSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileWrite( hFile, gbPublicLastKnownOppLevel, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}


	// Save the Public Noise Volume
	uiSaveSize = MAXTEAMS;
	FileWrite( hFile, gubPublicNoiseVolume, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	// Save the Public Last Noise Gridno
	uiSaveSize = MAXTEAMS;
	FileWrite( hFile, gsPublicNoiseGridno, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}



	return( TRUE );
}


BOOLEAN LoadOppListInfoFromSavedGame( HWFILE hFile )
{
	UINT32	uiLoadSize=0;
	UINT32	uiNumBytesRead=0;

	// Load the Public Opplist
	uiLoadSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileRead( hFile, gbPublicOpplist, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	// Load the Seen Oppenents
	uiLoadSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileRead( hFile, gbSeenOpponents, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}



	// Load the Last Known Opp Locations
	uiLoadSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileRead( hFile, gsLastKnownOppLoc, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	// Load the Last Known Opp Level
	uiLoadSize = TOTAL_SOLDIERS * TOTAL_SOLDIERS;
	FileRead( hFile, gbLastKnownOppLevel, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}


	// Load the Public Last Known Opp Locations
	uiLoadSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileRead( hFile, gsPublicLastKnownOppLoc, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	// Load the Public Last Known Opp Level
	uiLoadSize = MAXTEAMS * TOTAL_SOLDIERS;
	FileRead( hFile, gbPublicLastKnownOppLevel, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}


	// Load the Public Noise Volume
	uiLoadSize = MAXTEAMS;
	FileRead( hFile, gubPublicNoiseVolume, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	// Load the Public Last Noise Gridno
	uiLoadSize = MAXTEAMS;
	FileRead( hFile, gsPublicNoiseGridno, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN SaveWatchedLocsToSavedGame( HWFILE hFile )
{
	UINT32	uiArraySize;
	UINT32	uiSaveSize=0;
	UINT32	uiNumBytesWritten=0;

	uiArraySize = TOTAL_SOLDIERS * NUM_WATCHED_LOCS;

	// save locations of watched points
	uiSaveSize = uiArraySize * sizeof( INT16 );
	FileWrite( hFile, gsWatchedLoc, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	uiSaveSize = uiArraySize * sizeof( INT8 );

	FileWrite( hFile, gbWatchedLocLevel, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	FileWrite( hFile, gubWatchedLocPoints, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	FileWrite( hFile, gfWatchedLocReset, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN LoadWatchedLocsFromSavedGame( HWFILE hFile )
{
	UINT32	uiArraySize;
	UINT32	uiLoadSize=0;
	UINT32	uiNumBytesRead=0;

	uiArraySize = TOTAL_SOLDIERS * NUM_WATCHED_LOCS;

	uiLoadSize = uiArraySize * sizeof( INT16 );
	FileRead( hFile, gsWatchedLoc, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	uiLoadSize = uiArraySize * sizeof( INT8 );
	FileRead( hFile, gbWatchedLocLevel, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	FileRead( hFile, gubWatchedLocPoints, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}

	FileRead( hFile, gfWatchedLocReset, uiLoadSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiLoadSize )
	{
		return( FALSE );
	}


	return( TRUE );
}

void CreateSavedGameFileNameFromNumber( UINT8 ubSaveGameID, STR pzNewFileName )
{
	//if we are creating the QuickSave file
	if( ubSaveGameID == 0 )
	{
#ifdef JA2BETAVERSION
		//if the user wants to have consecutive quick saves
		if( gfUseConsecutiveQuickSaveSlots )
		{
			//if we are loading a game, and the user hasnt saved any consecutinve saves, load the defualt save
			if( guiCurrentQuickSaveNumber == 0 )
				sprintf( pzNewFileName , "%s\\%S.%S", gSaveDir, pMessageStrings[ MSG_QUICKSAVE_NAME ], pMessageStrings[ MSG_SAVEEXTENSION ] );
			else
				sprintf( pzNewFileName , "%s\\%S%02d.%S", gSaveDir, pMessageStrings[ MSG_QUICKSAVE_NAME ], guiCurrentQuickSaveNumber, pMessageStrings[ MSG_SAVEEXTENSION ] );
		}
		else
#endif
			sprintf( pzNewFileName , "%s\\%S.%S", gSaveDir, pMessageStrings[ MSG_QUICKSAVE_NAME ], pMessageStrings[ MSG_SAVEEXTENSION ] );
	}
//#ifdef JA2BETAVERSION
	else if( ubSaveGameID == SAVE__END_TURN_NUM )
	{
		//The name of the file
		sprintf( pzNewFileName , "%s\\Auto%02d.%S", gSaveDir, guiLastSaveGameNum, pMessageStrings[ MSG_SAVEEXTENSION ] );

		//increment end turn number
		guiLastSaveGameNum++;

		//just have 2 saves
		if( guiLastSaveGameNum == 2 )
		{
			guiLastSaveGameNum = 0;
		}
	}
//#endif

	else
		sprintf( pzNewFileName , "%s\\%S%02d.%S", gSaveDir, pMessageStrings[ MSG_SAVE_NAME ], ubSaveGameID, pMessageStrings[ MSG_SAVEEXTENSION ] );
}




BOOLEAN SaveMercPathFromSoldierStruct( HWFILE hFile, UINT8	ubID )
{
	UINT32	uiNumOfNodes=0;
	PathStPtr	pTempPath = Menptr[ ubID ].pMercPath;
	UINT32	uiNumBytesWritten=0;


	//loop through to get all the nodes
	while( pTempPath )
	{
		uiNumOfNodes++;
		pTempPath = pTempPath->pNext;
	}


	//Save the number of the nodes
	FileWrite( hFile, &uiNumOfNodes, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return(FALSE);
	}

	//loop through all the nodes and add them
	pTempPath = Menptr[ ubID ].pMercPath;


	//loop through nodes and save all the nodes
	while( pTempPath )
	{
		//Save the number of the nodes
		FileWrite( hFile, pTempPath, sizeof( PathSt ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( PathSt ) )
		{
			return(FALSE);
		}

		pTempPath = pTempPath->pNext;
	}



	return( TRUE );
}



BOOLEAN LoadMercPathToSoldierStruct( HWFILE hFile, UINT8	ubID )
{
	UINT32	uiNumOfNodes=0;
	PathStPtr	pTempPath = NULL;
	PathStPtr	pTemp = NULL;
	UINT32	uiNumBytesRead=0;
	UINT32	cnt;



	//The list SHOULD be empty at this point
/*
	//if there is nodes, loop through and delete them
	if( Menptr[ ubID ].pMercPath )
	{
		pTempPath = Menptr[ ubID ].pMercPath;
		while( pTempPath )
		{
			pTemp = pTempPath;
			pTempPath = pTempPath->pNext;

			MemFree( pTemp );
			pTemp=NULL;
		}

		Menptr[ ubID ].pMercPath = NULL;
	}
*/

	//Load the number of the nodes
	FileRead( hFile, &uiNumOfNodes, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return(FALSE);
	}

	//load all the nodes
	for( cnt=0; cnt<uiNumOfNodes; cnt++ )
	{
		//Allocate memory for the new node
		pTemp = (PathStPtr) MemAlloc( sizeof( PathSt ) );
		if( pTemp == NULL )
		{
			pTempPath = MoveToBeginningOfPathList( pTempPath );
			ClearStrategicPathList( pTempPath, -1 );
 			return( FALSE );
		}
		memset( pTemp, 0 , sizeof( PathSt ) );


		//Load the node
		FileRead( hFile, pTemp, sizeof( PathSt ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( PathSt ) )
		{
			MemFree( pTemp);
			pTempPath = MoveToBeginningOfPathList( pTempPath );
			ClearStrategicPathList( pTempPath, -1 );
			return(FALSE);
		}

		//Put the node into the list 
		if( cnt == 0 )
		{
			pTempPath = pTemp;
			pTemp->pPrev = NULL;
		}
		else
		{
			pTempPath->pNext = pTemp;
			pTemp->pPrev = pTempPath;

			pTempPath = pTempPath->pNext;
		}

		pTemp->pNext = NULL;
	}

	//move to beginning of list
	pTempPath = MoveToBeginningOfPathList( pTempPath );

	Menptr[ ubID ].pMercPath = pTempPath;
	if( Menptr[ ubID ].pMercPath )
		Menptr[ ubID ].pMercPath->pPrev = NULL;

	return( TRUE );
}


#ifdef JA2BETAVERSION
void InitSaveGameFilePosition()
{
	CHAR8		zFileName[ MAX_PATH ];

	sprintf( zFileName, "%s\\SaveGameFilePos%2d.txt", gSaveDir, gubSaveGameLoc );

	FileDelete( zFileName );
}

void SaveGameFilePosition( INT32 iPos, STR pMsg )
{
	HWFILE	hFile;
	CHAR8		zTempString[512];
	UINT32	uiNumBytesWritten;
	UINT32	uiStrLen=0;
	CHAR8		zFileName[MAX_PATH];

	sprintf( zFileName, "%s\\SaveGameFilePos%2d.txt", gSaveDir, gubSaveGameLoc );

	// create the save game file
	hFile = FileOpen( zFileName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return;
	}

	FileSeek( hFile, 0, FILE_SEEK_FROM_END );

	sprintf( zTempString, "%8d	 %s\n", iPos, pMsg );
	uiStrLen = strlen( zTempString );

	FileWrite( hFile, zTempString, uiStrLen, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiStrLen )
	{
		FileClose( hFile );
		return;
	}

	FileClose( hFile );
}



void InitLoadGameFilePosition()
{
	CHAR8		zFileName[MAX_PATH];

	sprintf( zFileName, "%s\\LoadGameFilePos%2d.txt", gSaveDir, gubSaveGameLoc );

	FileDelete( zFileName );
}
void LoadGameFilePosition( INT32 iPos, STR pMsg )
{
	HWFILE	hFile;
	CHAR8		zTempString[512];
	UINT32	uiNumBytesWritten;
	UINT32	uiStrLen=0;

	CHAR8		zFileName[MAX_PATH];

	sprintf( zFileName, "%s\\LoadGameFilePos%2d.txt", gSaveDir, gubSaveGameLoc );

	// create the save game file
	hFile = FileOpen( zFileName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return;
	}

	FileSeek( hFile, 0, FILE_SEEK_FROM_END );

	sprintf( zTempString, "%8d	 %s\n", iPos, pMsg );
	uiStrLen = strlen( zTempString );

	FileWrite( hFile, zTempString, uiStrLen, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiStrLen )
	{
		FileClose( hFile );
		return;
	}

	FileClose( hFile );


}
#endif



BOOLEAN SaveGeneralInfo( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	GENERAL_SAVE_INFO sGeneralInfo;
	memset( &sGeneralInfo, 0, sizeof( GENERAL_SAVE_INFO ) );

	sGeneralInfo.ubMusicMode = gubMusicMode;
	sGeneralInfo.uiCurrentUniqueSoldierId = guiCurrentUniqueSoldierId;
	sGeneralInfo.uiCurrentScreen = guiPreviousOptionScreen;

	sGeneralInfo.usSelectedSoldier = gusSelectedSoldier;
	sGeneralInfo.sRenderCenterX = gsRenderCenterX;
	sGeneralInfo.sRenderCenterY = gsRenderCenterY;
	sGeneralInfo.fAtLeastOneMercWasHired = gfAtLeastOneMercWasHired;
	sGeneralInfo.fHavePurchasedItemsFromTony = gfHavePurchasedItemsFromTony;

	sGeneralInfo.fShowItemsFlag			= fShowItemsFlag;
	sGeneralInfo.fShowTownFlag			= fShowTownFlag;
	sGeneralInfo.fShowMineFlag			= fShowMineFlag;
	sGeneralInfo.fShowAircraftFlag	= fShowAircraftFlag;
	sGeneralInfo.fShowTeamFlag			= fShowTeamFlag;

	sGeneralInfo.fHelicopterAvailable = fHelicopterAvailable;

	// helicopter vehicle id
	sGeneralInfo.iHelicopterVehicleId = iHelicopterVehicleId;

	// total distance travelled
//	sGeneralInfo.iTotalHeliDistanceSinceRefuel = iTotalHeliDistanceSinceRefuel;

	// total owed by player
	sGeneralInfo.iTotalAccumulatedCostByPlayer = iTotalAccumulatedCostByPlayer;

	// whether or not skyrider is alive and well? and on our side yet?
	sGeneralInfo.fSkyRiderAvailable = fSkyRiderAvailable;

	// is the heli in the air?
	sGeneralInfo.fHelicopterIsAirBorne = fHelicopterIsAirBorne;

	// is the pilot returning straight to base?
	sGeneralInfo.fHeliReturnStraightToBase = fHeliReturnStraightToBase;

	// heli hovering
	sGeneralInfo.fHoveringHelicopter = fHoveringHelicopter;

	// time started hovering
	sGeneralInfo.uiStartHoverTime = uiStartHoverTime;

	// what state is skyrider's dialogue in in?
	sGeneralInfo.uiHelicopterSkyriderTalkState = guiHelicopterSkyriderTalkState;

	// the flags for skyrider events
	sGeneralInfo.fShowEstoniRefuelHighLight = fShowEstoniRefuelHighLight;
	sGeneralInfo.fShowOtherSAMHighLight = fShowOtherSAMHighLight;
	sGeneralInfo.fShowDrassenSAMHighLight = fShowDrassenSAMHighLight;
	sGeneralInfo.fShowCambriaHospitalHighLight = fShowCambriaHospitalHighLight;

	//The current state of the weather
	sGeneralInfo.uiEnvWeather = guiEnvWeather;

	sGeneralInfo.ubDefaultButton = gubDefaultButton;

	sGeneralInfo.fSkyriderEmptyHelpGiven = gfSkyriderEmptyHelpGiven;
	sGeneralInfo.ubHelicopterHitsTaken = gubHelicopterHitsTaken;
	sGeneralInfo.fSkyriderSaidCongratsOnTakingSAM = gfSkyriderSaidCongratsOnTakingSAM;
	sGeneralInfo.ubPlayerProgressSkyriderLastCommentedOn = gubPlayerProgressSkyriderLastCommentedOn;

	sGeneralInfo.fEnterMapDueToContract = fEnterMapDueToContract;
	sGeneralInfo.ubQuitType = ubQuitType;

	if( pContractReHireSoldier != NULL )
		sGeneralInfo.sContractRehireSoldierID = pContractReHireSoldier->ubID;
	else
		sGeneralInfo.sContractRehireSoldierID = -1;

	memcpy( &sGeneralInfo.GameOptions, &gGameOptions, sizeof( GAME_OPTIONS ) );


	#ifdef JA2BETAVERSION
	//Everytime we save get, and set a seed value, when reload, seed again
	sGeneralInfo.uiSeedNumber = GetJA2Clock();
	srand( sGeneralInfo.uiSeedNumber );
	#endif

	//Save the Ja2Clock()
	sGeneralInfo.uiBaseJA2Clock = guiBaseJA2Clock;

	// Save the current tactical panel mode
	sGeneralInfo.sCurInterfacePanel = gsCurInterfacePanel;

	// Save the selected merc
	if( gpSMCurrentMerc )
		sGeneralInfo.ubSMCurrentMercID = gpSMCurrentMerc->ubID;
	else
		sGeneralInfo.ubSMCurrentMercID = 255;

	//Save the fact that it is the first time in mapscreen
	sGeneralInfo.fFirstTimeInMapScreen = fFirstTimeInMapScreen;

	//save map screen disabling buttons
	sGeneralInfo.fDisableDueToBattleRoster = fDisableDueToBattleRoster;
	sGeneralInfo.fDisableMapInterfaceDueToBattle = fDisableMapInterfaceDueToBattle;

	// Save boxing info
	memcpy( &sGeneralInfo.sBoxerGridNo, &gsBoxerGridNo, NUM_BOXERS * sizeof( INT16 ) );
	memcpy( &sGeneralInfo.ubBoxerID, &gubBoxerID, NUM_BOXERS * sizeof( INT8 ) );
	memcpy( &sGeneralInfo.fBoxerFought, &gfBoxerFought, NUM_BOXERS * sizeof( BOOLEAN ) );

	//Save the helicopter status
	sGeneralInfo.fHelicopterDestroyed = fHelicopterDestroyed;
	sGeneralInfo.fShowMapScreenHelpText = fShowMapScreenHelpText;

	sGeneralInfo.iSortStateForMapScreenList = giSortStateForMapScreenList;
	sGeneralInfo.fFoundTixa = fFoundTixa;

	sGeneralInfo.uiTimeOfLastSkyriderMonologue = guiTimeOfLastSkyriderMonologue;
	sGeneralInfo.fSkyRiderSetUp = fSkyRiderSetUp;

	memcpy( &sGeneralInfo.fRefuelingSiteAvailable, &fRefuelingSiteAvailable, NUMBER_OF_REFUEL_SITES * sizeof( BOOLEAN ) );


	//Meanwhile stuff
	memcpy( &sGeneralInfo.gCurrentMeanwhileDef, &gCurrentMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) );
	//sGeneralInfo.gfMeanwhileScheduled = gfMeanwhileScheduled;
	sGeneralInfo.gfMeanwhileTryingToStart = gfMeanwhileTryingToStart;
	sGeneralInfo.gfInMeanwhile = gfInMeanwhile;


	// list of dead guys for squads...in id values->-1 means no one home 
	memcpy( &sGeneralInfo.sDeadMercs, &sDeadMercs, sizeof( INT16 ) * NUMBER_OF_SQUADS * NUMBER_OF_SOLDIERS_PER_SQUAD );

	// level of public noises
	memcpy( &sGeneralInfo.gbPublicNoiseLevel, &gbPublicNoiseLevel, sizeof( INT8 ) * MAXTEAMS );

	//The screen count for the initscreen
	sGeneralInfo.gubScreenCount = gubScreenCount;


	//used for the mean while screen
	sGeneralInfo.uiMeanWhileFlags = uiMeanWhileFlags;

	//Imp portrait number
	sGeneralInfo.iPortraitNumber = iPortraitNumber;

	// location of first enocunter with enemy
	sGeneralInfo.sWorldSectorLocationOfFirstBattle = sWorldSectorLocationOfFirstBattle;


	//State of email flags
	sGeneralInfo.fUnReadMailFlag = fUnReadMailFlag;
	sGeneralInfo.fNewMailFlag = fNewMailFlag;
	sGeneralInfo.fOldUnReadFlag = fOldUnreadFlag;
	sGeneralInfo.fOldNewMailFlag = fOldNewMailFlag;

	sGeneralInfo.fShowMilitia	= fShowMilitia;

	sGeneralInfo.fNewFilesInFileViewer = fNewFilesInFileViewer;

	sGeneralInfo.fLastBoxingMatchWonByPlayer = gfLastBoxingMatchWonByPlayer;

	memcpy( &sGeneralInfo.fSamSiteFound, &fSamSiteFound, MAX_NUMBER_OF_SAMS * sizeof( BOOLEAN ) );

	sGeneralInfo.ubNumTerrorists = gubNumTerrorists;
	sGeneralInfo.ubCambriaMedicalObjects = gubCambriaMedicalObjects;

	sGeneralInfo.fDisableTacticalPanelButtons = gfDisableTacticalPanelButtons;

	sGeneralInfo.sSelMapX						= sSelMapX;
	sGeneralInfo.sSelMapY						= sSelMapY;
	sGeneralInfo.iCurrentMapSectorZ	= iCurrentMapSectorZ;

	//Save the current status of the help screens flag that say wether or not the user has been there before
	sGeneralInfo.usHasPlayerSeenHelpScreenInCurrentScreen = gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen;

	sGeneralInfo.ubBoxingMatchesWon = gubBoxingMatchesWon;
	sGeneralInfo.ubBoxersRests = gubBoxersRests;
	sGeneralInfo.fBoxersResting = gfBoxersResting;

	sGeneralInfo.ubDesertTemperature = gubDesertTemperature;
	sGeneralInfo.ubGlobalTemperature = gubGlobalTemperature;

	sGeneralInfo.sMercArriveSectorX	= gsMercArriveSectorX;
	sGeneralInfo.sMercArriveSectorY	= gsMercArriveSectorY;

	sGeneralInfo.fCreatureMeanwhileScenePlayed = gfCreatureMeanwhileScenePlayed;

	//save the global player num
	sGeneralInfo.ubPlayerNum = gbPlayerNum;

	//New stuff for the Prebattle interface / autoresolve
	sGeneralInfo.fPersistantPBI									= gfPersistantPBI;
	sGeneralInfo.ubEnemyEncounterCode						= gubEnemyEncounterCode;
	sGeneralInfo.ubExplicitEnemyEncounterCode		= gubExplicitEnemyEncounterCode;
	sGeneralInfo.fBlitBattleSectorLocator				= gfBlitBattleSectorLocator;
	sGeneralInfo.ubPBSectorX										= gubPBSectorX;
	sGeneralInfo.ubPBSectorY										= gubPBSectorY;
	sGeneralInfo.ubPBSectorZ										= gubPBSectorZ;
	sGeneralInfo.fCantRetreatInPBI							= gfCantRetreatInPBI;
	sGeneralInfo.fExplosionQueueActive					= gfExplosionQueueActive;

	sGeneralInfo.bSelectedInfoChar							= bSelectedInfoChar;

	sGeneralInfo.iHospitalTempBalance						= giHospitalTempBalance;
	sGeneralInfo.iHospitalRefund								= giHospitalRefund;
	sGeneralInfo.bHospitalPriceModifier					= gbHospitalPriceModifier;
	sGeneralInfo.fPlayerTeamSawJoey			 = gfPlayerTeamSawJoey;
	sGeneralInfo.fMikeShouldSayHi								= gfMikeShouldSayHi;

	//Setup the 
	//Save the current music mode
	FileWrite( hFile, &sGeneralInfo, sizeof( GENERAL_SAVE_INFO ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( GENERAL_SAVE_INFO ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	return( TRUE );
}

//rain
extern UINT32 guiRainLoop;
//end rain

BOOLEAN LoadGeneralInfo( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	GENERAL_SAVE_INFO sGeneralInfo;
	memset( &sGeneralInfo, 0, sizeof( GENERAL_SAVE_INFO ) );


	//Load the current music mode
	FileRead( hFile, &sGeneralInfo, sizeof( GENERAL_SAVE_INFO ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( GENERAL_SAVE_INFO ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	gMusicModeToPlay = sGeneralInfo.ubMusicMode;

	guiCurrentUniqueSoldierId = sGeneralInfo.uiCurrentUniqueSoldierId;

	guiScreenToGotoAfterLoadingSavedGame = sGeneralInfo.uiCurrentScreen;

//	gusSelectedSoldier = NOBODY;
	gusSelectedSoldier = sGeneralInfo.usSelectedSoldier;

	gsRenderCenterX = sGeneralInfo.sRenderCenterX;
	gsRenderCenterY = sGeneralInfo.sRenderCenterY;

	gfAtLeastOneMercWasHired = sGeneralInfo.fAtLeastOneMercWasHired;



	gfHavePurchasedItemsFromTony = sGeneralInfo.fHavePurchasedItemsFromTony;

	fShowItemsFlag		= sGeneralInfo.fShowItemsFlag;
	fShowTownFlag			= sGeneralInfo.fShowTownFlag;
	fShowMineFlag			= sGeneralInfo.fShowMineFlag;
	fShowAircraftFlag	= sGeneralInfo.fShowAircraftFlag;
	fShowTeamFlag			= sGeneralInfo.fShowTeamFlag;

	fHelicopterAvailable = sGeneralInfo.fHelicopterAvailable;

	// helicopter vehicle id
	iHelicopterVehicleId = sGeneralInfo.iHelicopterVehicleId;

	// total distance travelled
//	iTotalHeliDistanceSinceRefuel = sGeneralInfo.iTotalHeliDistanceSinceRefuel;

	// total owed to player
	iTotalAccumulatedCostByPlayer = sGeneralInfo.iTotalAccumulatedCostByPlayer;

	// whether or not skyrider is alive and well? and on our side yet?
	fSkyRiderAvailable = sGeneralInfo.fSkyRiderAvailable;

	// is the heli in the air?
	fHelicopterIsAirBorne = sGeneralInfo.fHelicopterIsAirBorne;

	// is the pilot returning straight to base?
	fHeliReturnStraightToBase = sGeneralInfo.fHeliReturnStraightToBase;

	// heli hovering
	fHoveringHelicopter = sGeneralInfo.fHoveringHelicopter;

	// time started hovering
	uiStartHoverTime = sGeneralInfo.uiStartHoverTime;

	// what state is skyrider's dialogue in in?
	guiHelicopterSkyriderTalkState = sGeneralInfo.uiHelicopterSkyriderTalkState;

	// the flags for skyrider events
	fShowEstoniRefuelHighLight = sGeneralInfo.fShowEstoniRefuelHighLight;
	fShowOtherSAMHighLight = sGeneralInfo.fShowOtherSAMHighLight;
	fShowDrassenSAMHighLight = sGeneralInfo.fShowDrassenSAMHighLight;
	fShowCambriaHospitalHighLight = sGeneralInfo.fShowCambriaHospitalHighLight;

	//The current state of the weather
	guiEnvWeather = sGeneralInfo.uiEnvWeather;

	//rain
	if ( guiRainLoop != NO_SAMPLE )
	{
		SoundStop( guiRainLoop );
		guiRainLoop = NO_SAMPLE;
	}
	//end rain

	gubDefaultButton = sGeneralInfo.ubDefaultButton;

	gfSkyriderEmptyHelpGiven = sGeneralInfo.fSkyriderEmptyHelpGiven;
	gubHelicopterHitsTaken = sGeneralInfo.ubHelicopterHitsTaken;
	gfSkyriderSaidCongratsOnTakingSAM = sGeneralInfo.fSkyriderSaidCongratsOnTakingSAM;
	gubPlayerProgressSkyriderLastCommentedOn = sGeneralInfo.ubPlayerProgressSkyriderLastCommentedOn;

	fEnterMapDueToContract = sGeneralInfo.fEnterMapDueToContract;
	ubQuitType = sGeneralInfo.ubQuitType;

	//if the soldier id is valid
	if( sGeneralInfo.sContractRehireSoldierID == -1 )
		pContractReHireSoldier = NULL;
	else
		pContractReHireSoldier = &Menptr[ sGeneralInfo.sContractRehireSoldierID ];

	memcpy( &gGameOptions, &sGeneralInfo.GameOptions, sizeof( GAME_OPTIONS ) );

	#ifdef JA2BETAVERSION
	//Reset the random 'seed' number
	srand( sGeneralInfo.uiSeedNumber );
	#endif

	//Restore the JA2 Clock
	guiBaseJA2Clock = sGeneralInfo.uiBaseJA2Clock;

	// whenever guiBaseJA2Clock changes, we must reset all the timer variables that use it as a reference
	ResetJA2ClockGlobalTimers();


	// Restore the selected merc
	if( sGeneralInfo.ubSMCurrentMercID == 255 )
		gpSMCurrentMerc = NULL;
	else
		gpSMCurrentMerc = &Menptr[ sGeneralInfo.ubSMCurrentMercID ];

	//Set the interface panel to the team panel
	ShutdownCurrentPanel( );

	//Restore the current tactical panel mode
	gsCurInterfacePanel = sGeneralInfo.sCurInterfacePanel;

	/*
	//moved to last stage in the LoadSaveGame() function
	//if we are in tactical
	if( guiScreenToGotoAfterLoadingSavedGame == GAME_SCREEN )
	{
		//Initialize the current panel
		InitializeCurrentPanel( );

		SelectSoldier( gusSelectedSoldier, FALSE, TRUE );
	}
	*/

	//Restore the fact that it is the first time in mapscreen
	fFirstTimeInMapScreen = sGeneralInfo.fFirstTimeInMapScreen;

	//Load map screen disabling buttons
	fDisableDueToBattleRoster = sGeneralInfo.fDisableDueToBattleRoster;
	fDisableMapInterfaceDueToBattle = sGeneralInfo.fDisableMapInterfaceDueToBattle;

	memcpy( &gsBoxerGridNo, &sGeneralInfo.sBoxerGridNo, NUM_BOXERS * sizeof( INT16 ) );
	memcpy( &gubBoxerID, &sGeneralInfo.ubBoxerID, NUM_BOXERS * sizeof( INT8 ) );
	memcpy( &gfBoxerFought, &sGeneralInfo.fBoxerFought, NUM_BOXERS * sizeof( BOOLEAN ) );

	//Load the helicopter status
	fHelicopterDestroyed = sGeneralInfo.fHelicopterDestroyed;
	fShowMapScreenHelpText = sGeneralInfo.fShowMapScreenHelpText;


	giSortStateForMapScreenList = sGeneralInfo.iSortStateForMapScreenList;
	fFoundTixa = sGeneralInfo.fFoundTixa;

	guiTimeOfLastSkyriderMonologue = sGeneralInfo.uiTimeOfLastSkyriderMonologue;
	fSkyRiderSetUp = sGeneralInfo.fSkyRiderSetUp;

	memcpy( &fRefuelingSiteAvailable, &sGeneralInfo.fRefuelingSiteAvailable, NUMBER_OF_REFUEL_SITES * sizeof( BOOLEAN ) );


	//Meanwhile stuff
	memcpy( &gCurrentMeanwhileDef, &sGeneralInfo.gCurrentMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) );
//	gfMeanwhileScheduled = sGeneralInfo.gfMeanwhileScheduled;
	gfMeanwhileTryingToStart = sGeneralInfo.gfMeanwhileTryingToStart;
	gfInMeanwhile = sGeneralInfo.gfInMeanwhile;

	// list of dead guys for squads...in id values->-1 means no one home 
	memcpy( &sDeadMercs, &sGeneralInfo.sDeadMercs, sizeof( INT16 ) * NUMBER_OF_SQUADS * NUMBER_OF_SOLDIERS_PER_SQUAD );

	// level of public noises
	memcpy( &gbPublicNoiseLevel, &sGeneralInfo.gbPublicNoiseLevel, sizeof( INT8 ) * MAXTEAMS );

	//the screen count for the init screen
	gubScreenCount = sGeneralInfo.gubScreenCount;

	//used for the mean while screen
	if ( guiCurrentSaveGameVersion < 71 )
	{
		uiMeanWhileFlags = sGeneralInfo.usOldMeanWhileFlags;
	}
	else
	{
		uiMeanWhileFlags = sGeneralInfo.uiMeanWhileFlags;
	}

	//Imp portrait number
	iPortraitNumber = sGeneralInfo.iPortraitNumber;

	// location of first enocunter with enemy
	sWorldSectorLocationOfFirstBattle = sGeneralInfo.sWorldSectorLocationOfFirstBattle;

	fShowMilitia	= sGeneralInfo.fShowMilitia;

	fNewFilesInFileViewer = sGeneralInfo.fNewFilesInFileViewer;

	gfLastBoxingMatchWonByPlayer = sGeneralInfo.fLastBoxingMatchWonByPlayer;

	memcpy( &fSamSiteFound, &sGeneralInfo.fSamSiteFound, MAX_NUMBER_OF_SAMS * sizeof( BOOLEAN ) );

	gubNumTerrorists = sGeneralInfo.ubNumTerrorists;
	gubCambriaMedicalObjects = sGeneralInfo.ubCambriaMedicalObjects;

	gfDisableTacticalPanelButtons = sGeneralInfo.fDisableTacticalPanelButtons;

	sSelMapX						= sGeneralInfo.sSelMapX;
	sSelMapY						= sGeneralInfo.sSelMapY;
	iCurrentMapSectorZ	= sGeneralInfo.iCurrentMapSectorZ;

	//State of email flags
	fUnReadMailFlag = sGeneralInfo.fUnReadMailFlag;
	fNewMailFlag = sGeneralInfo.fNewMailFlag;
	fOldUnreadFlag = sGeneralInfo.fOldUnReadFlag;
	fOldNewMailFlag = sGeneralInfo.fOldNewMailFlag;

	//Save the current status of the help screens flag that say wether or not the user has been there before
	gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen = sGeneralInfo.usHasPlayerSeenHelpScreenInCurrentScreen;

	gubBoxingMatchesWon = sGeneralInfo.ubBoxingMatchesWon;
	gubBoxersRests = sGeneralInfo.ubBoxersRests;
	gfBoxersResting = sGeneralInfo.fBoxersResting;

	gubDesertTemperature = sGeneralInfo.ubDesertTemperature;
	gubGlobalTemperature = sGeneralInfo.ubGlobalTemperature;

	gsMercArriveSectorX = sGeneralInfo.sMercArriveSectorX;
	gsMercArriveSectorY = sGeneralInfo.sMercArriveSectorY;

	gfCreatureMeanwhileScenePlayed = sGeneralInfo.fCreatureMeanwhileScenePlayed;

	//load the global player num
	gbPlayerNum = sGeneralInfo.ubPlayerNum;

	//New stuff for the Prebattle interface / autoresolve
	gfPersistantPBI									= sGeneralInfo.fPersistantPBI;
	gubEnemyEncounterCode						= sGeneralInfo.ubEnemyEncounterCode;
	gubExplicitEnemyEncounterCode		= sGeneralInfo.ubExplicitEnemyEncounterCode;
	gfBlitBattleSectorLocator				= sGeneralInfo.fBlitBattleSectorLocator;
	gubPBSectorX										= sGeneralInfo.ubPBSectorX;
	gubPBSectorY										= sGeneralInfo.ubPBSectorY;
	gubPBSectorZ										= sGeneralInfo.ubPBSectorZ;
	gfCantRetreatInPBI							= sGeneralInfo.fCantRetreatInPBI;
	gfExplosionQueueActive					= sGeneralInfo.fExplosionQueueActive;

	bSelectedInfoChar	= sGeneralInfo.bSelectedInfoChar;

	giHospitalTempBalance		= sGeneralInfo.iHospitalTempBalance;
	giHospitalRefund				= sGeneralInfo.iHospitalRefund;
	gbHospitalPriceModifier = sGeneralInfo.bHospitalPriceModifier;
	gfPlayerTeamSawJoey	 = sGeneralInfo.fPlayerTeamSawJoey;
	gfMikeShouldSayHi				= sGeneralInfo.fMikeShouldSayHi;

	return( TRUE );
}

BOOLEAN SavePreRandomNumbersToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	//Save the Prerandom number index
	FileWrite( hFile, &guiPreRandomIndex, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	//Save the Prerandom number index
	for (std::vector<UINT32>::iterator iter = guiPreRandomNums.begin();
			iter != guiPreRandomNums.end(); ++iter) {
		FileWrite( hFile, &*iter, sizeof( UINT32 ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}
	}
	//FileWrite( hFile, guiPreRandomNums, sizeof( UINT32 ) * MAX_PREGENERATED_NUMS, &uiNumBytesWritten );
	//if( uiNumBytesWritten != sizeof( UINT32 ) * MAX_PREGENERATED_NUMS )
	//{
	//	return( FALSE );
	//}

	return( TRUE );
}

BOOLEAN LoadPreRandomNumbersFromSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	//Load the Prerandom number index
	FileRead( hFile, &guiPreRandomIndex, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	//Load the Prerandom number index
	for (std::vector<UINT32>::iterator iter = guiPreRandomNums.begin();
			iter != guiPreRandomNums.end(); ++iter) {
		FileRead( hFile, &*iter, sizeof( UINT32 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}
	}
//	FileRead( hFile, guiPreRandomNums, sizeof( UINT32 ) * MAX_PREGENERATED_NUMS, &uiNumBytesRead );
//	if( uiNumBytesRead != sizeof( UINT32 ) * MAX_PREGENERATED_NUMS )
//	{
//		return( FALSE );
//	}

	return( TRUE );
}

BOOLEAN LoadMeanwhileDefsFromSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	if ( guiCurrentSaveGameVersion < 72 )
	{
		//Load the array of meanwhile defs
		FileRead( hFile, gMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) * (NUM_MEANWHILES-1), &uiNumBytesRead );
		if ( uiNumBytesRead != sizeof( MEANWHILE_DEFINITION ) * (NUM_MEANWHILES-1) )
		{
			return( FALSE );
		}
		// and set the last one
		memset( &(gMeanwhileDef[ NUM_MEANWHILES - 1]), 0, sizeof( MEANWHILE_DEFINITION ) );

	}
	else
	{
		//Load the array of meanwhile defs
		FileRead( hFile, gMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) * NUM_MEANWHILES, &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( MEANWHILE_DEFINITION ) * NUM_MEANWHILES )
		{
			return( FALSE );
		}
	}

	return( TRUE );
}

BOOLEAN SaveMeanwhileDefsFromSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	//Save the array of meanwhile defs
	FileWrite( hFile, &gMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) * NUM_MEANWHILES, &uiNumBytesWritten );
	if ( uiNumBytesWritten != sizeof( MEANWHILE_DEFINITION ) * NUM_MEANWHILES )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN DoesUserHaveEnoughHardDriveSpace()
{
	UINT32			uiBytesFree=0;

	uiBytesFree = GetFreeSpaceOnHardDriveWhereGameIsRunningFrom( );

	//check to see if there is enough hard drive space
	if( uiBytesFree < REQUIRED_FREE_SPACE )
	{
		return( FALSE );
	}

	return( TRUE );
}

#ifdef JA2BETAVERSION

void InitShutDownMapTempFileTest( BOOLEAN fInit, STR pNameOfFile, UINT8 ubSaveGameID )
{
	CHAR8		zFileName[MAX_PATH];
	HWFILE	hFile;
	CHAR8		zTempString[512];
	UINT32	uiStrLen;
	UINT32	uiNumBytesWritten;

	//strcpy( gzNameOfMapTempFile, pNameOfFile);
	sprintf( gzNameOfMapTempFile, "%s%d", pNameOfFile, ubSaveGameID );

	sprintf( zFileName, "%s\\%s.txt", gSaveDir, gzNameOfMapTempFile );

	if( fInit )
	{
		guiNumberOfMapTempFiles = 0;		//Test:	To determine where the temp files are crashing
		guiSizeOfTempFiles = 0;

		if( FileExists( zFileName ) )
		{
			FileDelete( zFileName );
		}
	}
	else
	{
		// create the save game file
		hFile = FileOpen( zFileName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
		if( !hFile )
		{
			FileClose( hFile );
			return;
		}

		FileSeek( hFile, 0, FILE_SEEK_FROM_END );

		sprintf( zTempString, "Number Of Files: %6d.	Size of all files: %6d.\n", guiNumberOfMapTempFiles, guiSizeOfTempFiles );
		uiStrLen = strlen( zTempString );

		FileWrite( hFile, zTempString, uiStrLen, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiStrLen )
		{
			FileClose( hFile );
			return;
		}

		FileClose( hFile );

	}
}

void WriteTempFileNameToFile( STR pFileName, UINT32 uiSizeOfFile, HFILE hSaveFile )
{
	HWFILE	hFile;
	CHAR8		zTempString[512];
	UINT32	uiNumBytesWritten;
	UINT32	uiStrLen=0;

	CHAR8		zFileName[MAX_PATH];

	guiSizeOfTempFiles += uiSizeOfFile;

	sprintf( zFileName, "%s\\%s.txt", gSaveDir, gzNameOfMapTempFile );

	// create the save game file
	hFile = FileOpen( zFileName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return;
	}

	FileSeek( hFile, 0, FILE_SEEK_FROM_END );

	sprintf( zTempString, "%8d	%6d	%s\n", FileGetPos( hSaveFile ), uiSizeOfFile, pFileName );
	uiStrLen = strlen( zTempString );

	FileWrite( hFile, zTempString, uiStrLen, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiStrLen )
	{
		FileClose( hFile );
		return;
	}

	FileClose( hFile );
}

#endif

void GetBestPossibleSectorXYZValues( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ )
{
	//if the current sector is valid
	if( gfWorldLoaded )
	{
		*psSectorX = gWorldSectorX;
		*psSectorY = gWorldSectorY;
		*pbSectorZ = gbWorldSectorZ;
	}
	else if( iCurrentTacticalSquad != NO_CURRENT_SQUAD && Squad[ iCurrentTacticalSquad ][ 0 ] )
	{
		if( Squad[ iCurrentTacticalSquad ][ 0 ]->bAssignment != IN_TRANSIT )
		{
			*psSectorX = Squad[ iCurrentTacticalSquad ][ 0 ]->sSectorX;
			*psSectorY = Squad[ iCurrentTacticalSquad ][ 0 ]->sSectorY;
			*pbSectorZ = Squad[ iCurrentTacticalSquad ][ 0 ]->bSectorZ;
		}
	}
	else
	{
		INT16					sSoldierCnt;
		SOLDIERTYPE		*pSoldier;
		INT16					bLastTeamID;
		BOOLEAN				fFoundAMerc=FALSE;

		// Set locator to first merc
		sSoldierCnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

		//loop through all the mercs on the players team to find the one that is not moving
		for ( pSoldier = MercPtrs[ sSoldierCnt ]; sSoldierCnt <= bLastTeamID; sSoldierCnt++,pSoldier++)
		{
			if( pSoldier->bActive )
			{
				if ( pSoldier->bAssignment != IN_TRANSIT && !pSoldier->flags.fBetweenSectors)
				{
					//we found an alive, merc that is not moving
					*psSectorX = pSoldier->sSectorX;
					*psSectorY = pSoldier->sSectorY;
					*pbSectorZ = pSoldier->bSectorZ;
					fFoundAMerc = TRUE;
					break;
				}
			}
		}

		//if we didnt find a merc
		if( !fFoundAMerc )
		{
			// Set locator to first merc
			sSoldierCnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

			//loop through all the mercs and find one that is moving
			for ( pSoldier = MercPtrs[ sSoldierCnt ]; sSoldierCnt <= bLastTeamID; sSoldierCnt++,pSoldier++)
			{
				if( pSoldier->bActive )
				{
					//we found an alive, merc that is not moving
					*psSectorX = pSoldier->sSectorX;
					*psSectorY = pSoldier->sSectorY;
					*pbSectorZ = pSoldier->bSectorZ;
					fFoundAMerc = TRUE;
					break;
				}
			}

			//if we STILL havent found a merc, give up and use the -1, -1, -1
			if( !fFoundAMerc )
			{
				*psSectorX = gWorldSectorX;
				*psSectorY = gWorldSectorY;
				*pbSectorZ = gbWorldSectorZ;
			}
		}
	}
}


void PauseBeforeSaveGame( void )
{
	//if we are not in the save load screen
	if( guiCurrentScreen != SAVE_LOAD_SCREEN )
	{
		//Pause the game
		PauseGame();
	}
}

void UnPauseAfterSaveGame( void )
{
	//if we are not in the save load screen
	if( guiCurrentScreen != SAVE_LOAD_SCREEN )
	{
		//UnPause time compression
		UnPauseGame();
	}
}


void ValidateStrategicGroups()
{
	SECTORINFO *pSector;
	INT32 i;

	for( i = SEC_A1; i < SEC_P16; i++ )
	{
		pSector = &SectorInfo[ i ];
		if( pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites > gGameExternalOptions.iMaxEnemyGroupSize )
		{
		}
		//militia
		if( pSector->ubNumberOfCivsAtLevel[0] + pSector->ubNumberOfCivsAtLevel[1] + pSector->ubNumberOfCivsAtLevel[2] > gGameExternalOptions.iMaxEnemyGroupSize )
		{
		}
	}

	GROUP *pGroup = gpGroupList;
	GROUP *next;
	while( pGroup ) {
		next = pGroup->next;
		if (pGroup->ubSectorX < MINIMUM_VALID_X_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
		}
		else if (pGroup->ubSectorX > MAXIMUM_VALID_X_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
//			RemoveGroupFromList( pGroup );
		}
		else if (pGroup->ubSectorY < MINIMUM_VALID_Y_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
		}
		else if (pGroup->ubSectorY > MAXIMUM_VALID_Y_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
//			RemoveGroupFromList( pGroup );
		}
		else if (pGroup->ubSectorZ < MINIMUM_VALID_Z_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
//			RemoveGroupFromList( pGroup );
		}
		else if (pGroup->ubSectorZ > MAXIMUM_VALID_Z_COORDINATE) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
//			RemoveGroupFromList( pGroup );
		}
		else if (pGroup->ubGroupSize == 0) {
			//ScreenMsg( FONT_MCOLOR_WHITE, MSG_ERROR, L"...");
		}
		pGroup = next;
	}
}


void UpdateMercMercContractInfo()
{
	UINT8	ubCnt;
	SOLDIERTYPE				*pSoldier;

	for( ubCnt=BIFF; ubCnt<= BUBBA; ubCnt++ )
	{
		pSoldier = FindSoldierByProfileID( ubCnt, TRUE );

		//if the merc is on the team
		if( pSoldier == NULL )
			continue;

		gMercProfiles[ ubCnt ].iMercMercContractLength = pSoldier->iTotalContractLength;

		pSoldier->iTotalContractLength = 0;
	}
}

INT8 GetNumberForAutoSave( BOOLEAN fLatestAutoSave )
{
	CHAR	zFileName1[MAX_PATH];
	CHAR	zFileName2[MAX_PATH];
	HWFILE	hFile;
	BOOLEAN	fFile1Exist, fFile2Exist;
	SGP_FILETIME	CreationTime1, LastAccessedTime1, LastWriteTime1;
	SGP_FILETIME	CreationTime2, LastAccessedTime2, LastWriteTime2;

	fFile1Exist = FALSE;
	fFile2Exist = FALSE;


	//The name of the file
	sprintf( zFileName1, "%s\\Auto%02d.%S", gSaveDir, 0, pMessageStrings[ MSG_SAVEEXTENSION ] );
	sprintf( zFileName2, "%s\\Auto%02d.%S", gSaveDir, 1, pMessageStrings[ MSG_SAVEEXTENSION ] );

	if( FileExists( zFileName1 ) )
	{
		hFile = FileOpen( zFileName1, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );

		GetFileManFileTime( hFile, &CreationTime1, &LastAccessedTime1, &LastWriteTime1 );

		FileClose( hFile );

		fFile1Exist = TRUE;
	}

	if( FileExists( zFileName2 ) )
	{
		hFile = FileOpen( zFileName2, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );

		GetFileManFileTime( hFile, &CreationTime2, &LastAccessedTime2, &LastWriteTime2 );

		FileClose( hFile );

		fFile2Exist = TRUE;
	}

	if( !fFile1Exist && !fFile2Exist )
		return( -1 );
	else if( fFile1Exist && !fFile2Exist )
	{
		if( fLatestAutoSave )
			return( 0 );
		else
			return( -1 );
	}
	else if( !fFile1Exist && fFile2Exist )
	{
		if( fLatestAutoSave )
			return( 1 );
		else
			return( -1 );
	}
	else
	{
		if( CompareSGPFileTimes( &LastWriteTime1, &LastWriteTime2 ) > 0 )
			return( 0 );
		else
			return( 1 );
	}
}

void HandleOldBobbyRMailOrders()
{
	INT32 iCnt;
	INT32	iNewListCnt=0;

	if( LaptopSaveInfo.usNumberOfBobbyRayOrderUsed != 0 )
	{
		//Allocate memory for the list
		gpNewBobbyrShipments = (NewBobbyRayOrderStruct *) MemAlloc( sizeof( NewBobbyRayOrderStruct ) * LaptopSaveInfo.usNumberOfBobbyRayOrderUsed );
		if( gpNewBobbyrShipments == NULL )
		{
			Assert(0);
			return;
		}

		giNumberOfNewBobbyRShipment = LaptopSaveInfo.usNumberOfBobbyRayOrderUsed;

		//loop through and add the old items to the new list
		for( iCnt=0; iCnt< LaptopSaveInfo.usNumberOfBobbyRayOrderItems; iCnt++ )
		{
			//if this slot is used
			if( LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[iCnt].fActive )
			{
				//copy over the purchase info
				memcpy( gpNewBobbyrShipments[ iNewListCnt ].BobbyRayPurchase,
								LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[iCnt].BobbyRayPurchase,
								sizeof( BobbyRayPurchaseStruct ) * MAX_PURCHASE_AMOUNT );

				gpNewBobbyrShipments[ iNewListCnt ].fActive = TRUE;
				gpNewBobbyrShipments[ iNewListCnt ].ubDeliveryLoc = BR_DRASSEN;
				gpNewBobbyrShipments[ iNewListCnt ].ubDeliveryMethod = 0;
				gpNewBobbyrShipments[ iNewListCnt ].ubNumberPurchases = LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[iCnt].ubNumberPurchases;
				gpNewBobbyrShipments[ iNewListCnt ].uiPackageWeight = 1;
				gpNewBobbyrShipments[ iNewListCnt ].uiOrderedOnDayNum = GetWorldDay();
				gpNewBobbyrShipments[ iNewListCnt ].fDisplayedInShipmentPage = TRUE;

				iNewListCnt++;
			}
		}

		//Clear out the old list
		LaptopSaveInfo.usNumberOfBobbyRayOrderUsed = 0;
		MemFree( LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray );
		LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray = NULL;
	}
}


UINT32 CalcJA2EncryptionSet( SAVED_GAME_HEADER * pSaveGameHeader )
{
	UINT32	uiEncryptionSet = 0;

	uiEncryptionSet = pSaveGameHeader->uiSavedGameVersion;
	uiEncryptionSet *= pSaveGameHeader->uiFlags;
	uiEncryptionSet += pSaveGameHeader->iCurrentBalance;
	uiEncryptionSet *= (pSaveGameHeader->ubNumOfMercsOnPlayersTeam + 1);
	uiEncryptionSet += pSaveGameHeader->bSectorZ * 3;
	uiEncryptionSet += pSaveGameHeader->ubLoadScreenID;

	if ( pSaveGameHeader->fAlternateSector )
	{
		uiEncryptionSet += 7;
	}

	if ( pSaveGameHeader->uiRandom % 2 == 0 )
	{
		uiEncryptionSet++;

		if ( pSaveGameHeader->uiRandom % 7 == 0)
		{
			uiEncryptionSet++;
			if ( pSaveGameHeader->uiRandom % 23 == 0 )
			{
				uiEncryptionSet++;
			}
			if ( pSaveGameHeader->uiRandom % 79 == 0 )
			{
				uiEncryptionSet += 2;
			}
		}
	}

	#ifdef GERMAN
		uiEncryptionSet *= 11;
	#endif

	uiEncryptionSet = uiEncryptionSet % 10;

	uiEncryptionSet += pSaveGameHeader->uiDay / 10;

	uiEncryptionSet = uiEncryptionSet % 19;

	// now pick a different set of #s depending on what game options we've chosen
	if ( pSaveGameHeader->sInitialGameOptions.fGunNut )
	{
		uiEncryptionSet += BASE_NUMBER_OF_ROTATION_ARRAYS * 6;
	}

	if ( pSaveGameHeader->sInitialGameOptions.ubGameStyle == STYLE_SCIFI)
	{
		uiEncryptionSet += BASE_NUMBER_OF_ROTATION_ARRAYS * 3;
	}

	switch( pSaveGameHeader->sInitialGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			uiEncryptionSet += 0;
			break;
		case DIF_LEVEL_MEDIUM:
			uiEncryptionSet += BASE_NUMBER_OF_ROTATION_ARRAYS;
			break;
		case DIF_LEVEL_HARD:
			uiEncryptionSet += BASE_NUMBER_OF_ROTATION_ARRAYS * 2;
			break;
		case DIF_LEVEL_INSANE:
			uiEncryptionSet += BASE_NUMBER_OF_ROTATION_ARRAYS * 3;
			break;
	}

	return( uiEncryptionSet );
}
