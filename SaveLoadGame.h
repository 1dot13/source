#ifndef _SAVE_LOAD_GAME_H_
#define _SAVE_LOAD_GAME_H_

#include "GameSettings.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define		BYTESINMEGABYTE						1048576 //1024*1024
#define		REQUIRED_FREE_SPACE				(20 * BYTESINMEGABYTE)

#define		SIZE_OF_SAVE_GAME_DESC				128

#define		GAME_VERSION_LENGTH					16

#define		SAVE__ERROR_NUM						249//99

// WANNE:	The end turn have 2 saves (Auto00.sav and Auto01.sav). This 2 save slots should be underneath the 5 auto save slots
#define		SAVE__END_TURN_NUM					248//98
#define		SAVE__END_TURN_NUM_1				6//98
#define		SAVE__END_TURN_NUM_2				7//98

// WDS - Automatically try to save when an assertion failure occurs

// WANNE:	This SAVEGAME should not show up in the load screen
#define		SAVE__ASSERTION_FAILURE				247//97

#define     SAVE__TIMED_AUTOSAVE_SLOT1				1 //19	// WANNE: This slot does not show up in the load/save screen because it is > 18
#define     SAVE__TIMED_AUTOSAVE_SLOT2				2 
#define     SAVE__TIMED_AUTOSAVE_SLOT3				3
#define     SAVE__TIMED_AUTOSAVE_SLOT4				4
#define     SAVE__TIMED_AUTOSAVE_SLOT5				5

#define		EARLIST_SPECIAL_SAVE				247//97


typedef struct
{
	UINT32	uiSavedGameVersion;
	CHAR8		zGameVersionNumber[ GAME_VERSION_LENGTH ];

	CHAR16	sSavedGameDesc[ SIZE_OF_SAVE_GAME_DESC ];

	UINT32	uiFlags;

#ifdef CRIPPLED_VERSION
	UINT8 ubCrippleFiller[20];
#endif


	//The following will be used to quickly access info to display in the save/load screen
	UINT32	uiDay;
	UINT8		ubHour;
	UINT8		ubMin;
	INT16		sSectorX;
	INT16		sSectorY;
	INT8		bSectorZ;
	UINT8		ubNumOfMercsOnPlayersTeam;
	INT32		iCurrentBalance;

	UINT32	uiCurrentScreen;

	BOOLEAN	fAlternateSector;

	BOOLEAN	fWorldLoaded;

	UINT8		ubLoadScreenID;		//The load screen that should be used when loading the saved game

	GAME_OPTIONS	sInitialGameOptions;	//need these in the header so we can get the info from it on the save load screen.

	UINT32	uiRandom;

	UINT8		ubFiller[500];	// WANNE: Decrease this filler by 1, for each new UINT8 variable!

} SAVED_GAME_HEADER;


extern	UINT32		guiScreenToGotoAfterLoadingSavedGame;
extern UINT32 guiCurrentSaveGameVersion;

void			CreateSavedGameFileNameFromNumber( UINT8 ubSaveGameID, STR pzNewFileName );


BOOLEAN InitSaveDir();

BOOLEAN SaveGame( int ubSaveGameID, STR16 pGameDesc );
BOOLEAN LoadSavedGame( int ubSavedGameID );

BOOLEAN CopySavedSoldierInfoToNewSoldier( SOLDIERTYPE *pDestSourceInfo, SOLDIERTYPE *pSourceInfo );

BOOLEAN		SaveFilesToSavedGame( STR pSrcFileName, HWFILE hFile );
BOOLEAN		LoadFilesFromSavedGame( STR pSrcFileName, HWFILE hFile );

BOOLEAN DoesUserHaveEnoughHardDriveSpace();

void GetBestPossibleSectorXYZValues( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ );


extern UINT32	guiLastSaveGameNum;			// The end turn auto save number (0 = Auto00.sav, 1 = Auto01.sav)
INT8		GetNumberForAutoSave( BOOLEAN fLatestAutoSave );

/*CHRISL: This function is designed to allow reading the save game file one field at a time.  We currently save structures by saving a block of memory,
but variables are stored in memory so that they fit neatly into a WORD resulting in the program automatically adding some padding.  This padding is saved
during the save game process and this function is designed to calculate where that padding is so that we can account for it during the load process.  The
use of this function should allow changes to be made to various structures within the designated "POD", while still allowing for save game continuity.*/
INT32 ReadFieldByField( HWFILE hFile, PTR pDest, UINT32 uiFieldSize, UINT32 uiElementSize, UINT32  uiCurByteCount );

#endif
