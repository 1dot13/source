#ifndef _SAVE_LOAD_SCREEN__H_
#define _SAVE_LOAD_SCREEN__H_

#include	"MessageBoxScreen.h"


#define		NUM_SAVE_GAMES_OLD											11
#define		NUM_SAVE_GAMES_NEW											19

#define		NUM_SAVE_GAMES	255//19

extern BOOLEAN AutoSaveToSlot[5];

// WANNE: Used For auto save games
extern	UINT32	lastLoadedSaveGameDay;
extern	UINT8	lastLoadedSaveGameHour;

extern	UINT16 	NUM_SLOT;
extern	UINT32	SLG_SAVELOCATION_HEIGHT;
extern	UINT32	SLG_GAP_BETWEEN_LOCATIONS;
extern	UINT32	SLG_DATE_OFFSET_Y;
extern	UINT32	SLG_SELECTED_SLOT_GRAPHICS_NUMBER;
extern	UINT32	SLG_UNSELECTED_SLOT_GRAPHICS_NUMBER;
extern void LoadSaveGameOldOrNew();

//This flag is used to diferentiate between loading a game and saveing a game.
// gfSaveGame=TRUE		For saving a game
// gfSaveGame=FALSE		For loading a game
extern BOOLEAN		gfSaveGame;

//if there is to be a slot selected when entering this screen
extern INT32			gbSetSlotToBeSelected;

extern	BOOLEAN			gbSaveGameArray[ NUM_SAVE_GAMES ];

extern	BOOLEAN			gfCameDirectlyFromGame;

UINT32	SaveLoadScreenShutdown( void );
UINT32	SaveLoadScreenHandle( void );
UINT32	SaveLoadScreenInit( void );

//Kaiden : Trying to get Editor to work
extern BOOLEAN		DoSaveLoadMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );

BOOLEAN		InitSaveGameArray();

void			DoneFadeOutForSaveLoadScreen( void );
void			DoneFadeInForSaveLoadScreen( void );


BOOLEAN DoQuickSave();
BOOLEAN DoQuickLoad();

BOOLEAN IsThereAnySavedGameFiles();

void			DeleteSaveGameNumber( UINT8 ubSaveGameSlotID );

extern BOOLEAN DoAutoSave( int ubSaveGameID, STR16 pGameDesc );
extern BOOLEAN SaveDataSaveToSaveGameFile( HWFILE hFile );
extern BOOLEAN LoadDataSaveFromLoadGameFile( HWFILE hFile );
#endif
