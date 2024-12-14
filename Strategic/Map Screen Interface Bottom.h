#ifndef __MAP_INTERFACE_BOTTOM
#define __MAP_INTERFACE_BOTTOM

#include "types.h"
#include "Soldier Control.h"


#ifdef CHINESE //zwwoooooo: Chinese fonts relatively high , so to reduce the number of rows
#define MAX_MESSAGES_ON_MAP_BOTTOM	6
#else
#define MAX_MESSAGES_ON_MAP_BOTTOM	9
#endif
#ifdef JA2UB
extern	INT8 gbExitingMapScreenToWhere;
#endif
// exit to where defines
enum{
	MAP_EXIT_TO_LAPTOP = 0,
	MAP_EXIT_TO_TACTICAL,
	MAP_EXIT_TO_OPTIONS,
	MAP_EXIT_TO_LOAD,
	MAP_EXIT_TO_SAVE,
#ifdef JA2UB
	MAP_EXIT_TO_INTRO_SCREEN,
#endif
	// OJW - 20090210 - clean resources on disconnect
	MAP_EXIT_TO_MAINMENU
};

// there's no button for entering SAVE/LOAD screen directly...
extern UINT32 guiMapBottomExitButtons[ 3 ];

extern BOOLEAN fLapTop;
extern BOOLEAN fLeavingMapScreen;
extern BOOLEAN gfDontStartTransitionFromLaptop;
extern BOOLEAN gfStartMapScreenToLaptopTransition;


// function prototypes

BOOLEAN LoadMapScreenInterfaceBottom( void );
void DeleteMapScreenInterfaceBottom( void );
void DestroyButtonsForMapScreenInterfaceBottom( void );
BOOLEAN CreateButtonsForMapScreenInterfaceBottom( void );
// Headrock: Added argument to this function:
void RenderMapScreenInterfaceBottom( BOOLEAN fForceMapscreenBottomRerender );

// delete map bottom graphics
void DeleteMapBottomGraphics( void );

// load bottom graphics
void HandleLoadOfMapBottomGraphics( void );

// allowed to time compress?
BOOLEAN AllowedToTimeCompress( void );

void EnableDisAbleMapScreenOptionsButton( BOOLEAN fEnable );

// create and destroy masks to cover the time compression buttons as needed
void CreateDestroyMouseRegionMasksForTimeCompressionButtons( void );

BOOLEAN CommonTimeCompressionChecks( void );

BOOLEAN AnyUsableRealMercenariesOnTeam( void );

void RequestTriggerExitFromMapscreen( INT8 bExitToWhere );
BOOLEAN AllowedToExitFromMapscreenTo( INT8 bExitToWhere );
void HandleExitsFromMapScreen( void );

void MapScreenMsgScrollDown( UINT8 ubLinesDown );
void MapScreenMsgScrollUp( UINT8 ubLinesUp );

void ChangeCurrentMapscreenMessageIndex( UINT8 ubNewMessageIndex );
void MoveToEndOfMapScreenMessageList( void );

// HEADROCK HAM 3.6: Reset coordinates for slider bar and message window
void InitMapScreenInterfaceBottomCoords( void );

#endif
