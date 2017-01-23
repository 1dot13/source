/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "builddefines.h"
#include <stdio.h>
#include <time.h>
#include "sgp.h"
#include "gameloop.h"
#include "MiniGame.h"
#include "WCheck.h"
#include "Utilities.h"
#include "WordWrap.h"
#include "Cursors.h"
#include "Line.h"
#include "Insurance Text.h"
#include "Encrypted File.h"
#include "Text.h"
#include "Multi Language Graphic Utils.h"
#include "random.h"
#include "Interface.h"
#include "DropDown.h"
#include "finances.h"
#include "Game Clock.h"
#include "GameVersion.h"
#include "GameSettings.h"
#include "message.h"
#include "Game Event Hook.h"
#include "Overhead.h"
#include "Sound Control.h"
#include "himage.h"
#include "vobject.h"
#include "vobject_blitters.h"
#include "wcheck.h"
#include "input.h"
#include "font.h"
#include "timer.h"
#include "mousesystem.h"
#include <wchar.h>
#include <tchar.h>
#include "Timer Control.h"
#include "Sys Globals.h"
#include "interface.h"
#include "utilities.h"
#include "render dirty.h"
#include "cursor control.h"
#include "utilities.h"
#include "english.h"
#include "WordWrap.h"
#include "gamescreen.h"
#include "Isometric Utils.h"
#include "math.h"

// remember the previous screen
UINT32 guiMiniGamePreviousScreen = GAME_SCREEN;

int gMiniGame = MINIGAME_FIRST;
int gMiniGameState = MINIGAME_STARTSCREEN;
BOOLEAN gMiniGameIsInit = FALSE;

void SetNextGame( UINT32 ausGame, UINT32 ausGameState )
{
	gMiniGame = ausGame;
	gMiniGameState = ausGameState;

	guiMiniGamePreviousScreen = guiCurrentScreen;
}

void MiniGameDataInit()
{
	if ( !gMiniGameIsInit )
	{
		switch ( gMiniGame )
		{
		case TETRIS:
			MiniGame_Init_Tetris( );
			break;

		case PONG:
			MiniGame_Init_Pong( );
			break;

		default:
			break;
		}

		gMiniGameIsInit = TRUE;
	}
}

BOOLEAN MiniGameExit()
{
	gMiniGameIsInit = FALSE;

	gMiniGame = MINIGAME_FIRST;
	gMiniGameState = MINIGAME_STARTSCREEN;

	SetPendingNewScreen( GAME_SCREEN );

	FadeInGameScreen();

	HandleTacticalUI( );

	return GAME_SCREEN;
}

UINT32	MiniGameScreenInit( void )
{
	return TRUE;
}

UINT32	MiniGameScreenHandle( void )
{
	if ( gfKeyState[ESC] )
	{
		return MiniGameExit();
	}

	// init game data if not already done so
	MiniGameDataInit();

	switch ( gMiniGame )
	{
	case TETRIS:
		return MiniGame_Handle_Tetris( );
		break;

	case PONG:
		return MiniGame_Handle_Pong( );
		break;

	default:
		break;
	}

	return MINIGAME_SCREEN;
}

UINT32	MiniGameScreenShutdown( void )
{
	return TRUE;
}

void DisplayPNGImage( SGPRect aDstRect, std::string& arStrImage )
{
	VSURFACE_DESC		vs_desc = {};
	HVSURFACE			hVSurface;
	UINT32				uiLoadScreen;

	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE | VSURFACE_CREATE_FROMPNG_FALLBACK;

	arStrImage.copy( vs_desc.ImageFile, sizeof(vs_desc.ImageFile) - 1 );

	if ( FileExists( vs_desc.ImageFile ) && AddVideoSurface( &vs_desc, &uiLoadScreen ) )
	{
		//Blit the background image
		GetVideoSurface( &hVSurface, uiLoadScreen );

		// Stretch the background image
		SGPRect SrcRect;
		SrcRect.iLeft = 0;
		SrcRect.iTop = 0;
		SrcRect.iRight = hVSurface->usWidth;
		SrcRect.iBottom = hVSurface->usHeight;

		BltStretchVideoSurface( FRAME_BUFFER, uiLoadScreen, 0, 0, 0, &SrcRect, &aDstRect );

		DeleteVideoSurfaceFromIndex( uiLoadScreen );
	}
	else
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, aDstRect.iLeft, aDstRect.iTop, aDstRect.iRight, aDstRect.iBottom, Get16BPPColor( FROMRGB( 55, 55, 55 ) ) );
	}
}

// On order to wrk in all resolutions, limit our screen to this
#define MINIGAME_BG_X_LOW	iScreenWidthOffset
#define MINIGAME_BG_X_HIGH	(SCREEN_WIDTH - iScreenWidthOffset)
#define MINIGAME_BG_Y_LOW	iScreenHeightOffset
#define MINIGAME_BG_Y_HIGH	(SCREEN_HEIGHT - iScreenHeightOffset)

BOOLEAN gMiniGameRenewBackground = TRUE;						// In order to save cpu-time (not really an issue here, but it's the thought that counts!), only redraw the background if we have to

int gMiniGameMenuSelection = 0;								// In a small menu, we can select some options, so we need to keep track where we are
int gMiniGameMenuScreen = 0;								// What page of the menu is currently used?

////////////////////////////////////////////// TETRIS //////////////////////////////////////////////
// In case you somehow don't know how Tetris works:
// In this game, the player has to rotate and move a tile, consisting of several types of each 4 blocks put together in a variety of orientations, while it falls down.
// Once the tile has been placed, a new tile appears.
// If the player manages t complete a full line of blocks, that line is erased, and points are awarded.
// The game is to reach a high a score as possible.

// This defines what tiles a block can belong to
enum
{
	TETRISBLOCKTYPE_NONE = 0,			// empty block
	TETRISBLOCKTYPE_L,
	TETRISBLOCKTYPE_L_INVERTED,
	TETRISBLOCKTYPE_T,
	TETRISBLOCKTYPE_SQUARE,
	TETRISBLOCKTYPE_LONG,
	TETRISBLOCKTYPE_Z,
	TETRISBLOCKTYPE_Z_INVERTED,
	TETRISBLOCKTYPE_CONFLICT,			// we spawned a new tile into an existing tile. If this happens the game will end

	TETRISBLOCKTYPE_MAX
};

// we use this struct to remember whether a block is moving, controlled by the player, and what block type it has (necessary for colouring)
typedef struct
{
	BOOLEAN resting;
	BOOLEAN playercontrolled;
	INT8 blocktype;
} TETRIS_BLOCK;

// as it would be tedious to always determine the player-controlled blocks again, we remember them with this struct
typedef struct
{
	int x;
	int y;
} TETRIS_BLOCK_PLAYER;

// the dimensions of the playing field
#define TETRIS_FIELD_ROWS	18
#define TETRIS_FIELD_COLS	10

// we define out playing field
TETRIS_BLOCK gTetrisBlock[TETRIS_FIELD_ROWS][TETRIS_FIELD_COLS];

// rememeber the tiles the player controls (easier than to always look for them)
TETRIS_BLOCK_PLAYER gTetrisBlockPlayer[4];

// each tile can be rotated, so a tile has 4 orientations (for some types, these are identical)
#define TETRIS_ORIENTATIONS 4

// a little helper struct to define a pair of coordinates
typedef struct coordpair
{
	int x;
	int y;

	coordpair( int a, int b ) : x( a ), y( b ) {}
	coordpair( ){}
} coordpair;

// Each blocktype has 4 orientations. This struct contains the x- and y-offsets for each of the 4 blocks
// When we later place a tile, we simpyl determine the 'anchor'-coordinates, and then simply have to add the offsets to get the 4 blocks
// This of course relies on one block (the first one) retaining the same coordinates (no offset then).
typedef struct TETRIS_BLOCK_ORIENTATION
{
	coordpair cords[4];

	TETRIS_BLOCK_ORIENTATION( coordpair a, coordpair b, coordpair c, coordpair d )
	{
		cords[0] = a;
		cords[1] = b;
		cords[2] = c;
		cords[3] = d;
	}

	TETRIS_BLOCK_ORIENTATION( ){}
} TETRIS_BLOCK_ORIENTATION;

TETRIS_BLOCK_ORIENTATION gTetrisOrientation[TETRISBLOCKTYPE_MAX][TETRIS_ORIENTATIONS] =
{
	// TETRISBLOCKTYPE_NONE
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),

	// TETRISBLOCKTYPE_L
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ), coordpair( 1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 1 ), coordpair( -1, 0 ), coordpair( 1, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, -1 ), coordpair( 0, -1 ), coordpair( 0, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 1, 0 ), coordpair( 1, -1 ) ),

	// TETRISBLOCKTYPE_L_INVERTED
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ), coordpair( -1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 1, 0 ), coordpair( -1, 0 ), coordpair( -1, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ), coordpair( 1, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 1, 0 ), coordpair( -1, 0 ), coordpair( 1, 1 ) ),

	// TETRISBLOCKTYPE_T	
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 1, 0 ), coordpair( 0, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 1, 0 ), coordpair( 0, 1 ), coordpair( 0, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 1, 0 ), coordpair( 0, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ) ),

	// TETRISBLOCKTYPE_SQUARE
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, -1 ), coordpair( -1, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, -1 ), coordpair( -1, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, -1 ), coordpair( -1, -1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, -1 ), coordpair( -1, -1 ) ),

	// TETRISBLOCKTYPE_LONG
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ), coordpair( 0, 2 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 1, 0 ), coordpair( 2, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( 0, 1 ), coordpair( 0, 2 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 1, 0 ), coordpair( 2, 0 ) ),

	// TETRISBLOCKTYPE_Z
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 1, 0 ), coordpair( 0, 1 ), coordpair( -1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, -1 ), coordpair( -1, 0 ), coordpair( 0, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 1, 0 ), coordpair( 0, 1 ), coordpair( -1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, -1 ), coordpair( -1, 0 ), coordpair( 0, 1 ) ),

	// TETRISBLOCKTYPE_Z_INVERTED
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, 1 ), coordpair( 1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( -1, 0 ), coordpair( -1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( -1, 0 ), coordpair( 0, 1 ), coordpair( 1, 1 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, -1 ), coordpair( -1, 0 ), coordpair( -1, 1 ) ),

	// TETRISBLOCKTYPE_CONFLICT
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
	TETRIS_BLOCK_ORIENTATION( coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ), coordpair( 0, 0 ) ),
};

// a few global variables
BOOLEAN gTetrisplayercontrolledtile = FALSE;				// Does the player currently control a tile? (If not, maybe we should add a new one...)
int gTetrisTileSpawnTime = 0;								// The last tile was spawned at this time (we use this to award points fo placing tiles fast)
int gTetrisplayertileOrientation = 0;						// Orientation of the current tile?
int gTetrisNextTile = TETRISBLOCKTYPE_NONE;					// What will be the type of the next tile that will spawn? We need this as we preview show the player
int gTetrisScore = 0;
int gTetrisLinesDone = 0;
int gTetrisInitialLevel = 0;
int gTetrisHandicap = 0;

// for each level of handicap, we add 2 partially filled lines
#define TETRIS_MAX_HANDICAP	5

void MiniGame_Init_Tetris()
{
	for ( int i = 0; i < TETRIS_FIELD_ROWS; ++i )
	{
		for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
		{
			gTetrisBlock[i][j].resting = TRUE;
			gTetrisBlock[i][j].playercontrolled = FALSE;
			gTetrisBlock[i][j].blocktype = TETRISBLOCKTYPE_NONE;
		}
	}

	for ( int i = 0; i < 4; ++i )
	{
		gTetrisBlockPlayer[i].x = 0;
		gTetrisBlockPlayer[i].y = 0;
	}

	gTetrisplayercontrolledtile = FALSE;
	gTetrisTileSpawnTime = 0;
	gTetrisplayertileOrientation = 0;
	gTetrisNextTile = TETRISBLOCKTYPE_NONE;
	gTetrisScore = 0;
	gTetrisLinesDone = 0;
		
	gMiniGameMenuSelection = 0;
	gMiniGameMenuScreen = 0;
	gMiniGameRenewBackground = TRUE;

	// don't reset, so we don't have to set it again every time we start a game
	//gTetrisInitialLevel = 0;
	//gTetrisHandicap = 0;
}

void AddScore_Tetris( int aPoints )
{
	// bonus points:
	// - per level (either initial or lines completed / 10) 15%
	// per handicap 10%
	FLOAT bonusperc = 1.0f + (FLOAT)((15 * (gTetrisInitialLevel + gTetrisLinesDone / 10) + gTetrisHandicap * 10) / 100.0f);

	gTetrisScore += aPoints * bonusperc;
}

#define TETRIS_BLOCKLENGTH	20

// game area
#define MINIGAME_TETRIS_X_LOW		(MINIGAME_BG_X_LOW + 100)
#define MINIGAME_TETRIS_X_HIGH		(MINIGAME_TETRIS_X_LOW + TETRIS_FIELD_COLS * TETRIS_BLOCKLENGTH)
#define MINIGAME_TETRIS_Y_LOW		(MINIGAME_BG_Y_LOW + 100)
#define MINIGAME_TETRIS_Y_HIGH		(MINIGAME_TETRIS_Y_LOW + TETRIS_FIELD_ROWS * TETRIS_BLOCKLENGTH)

// we also need a small 4x4 area to display the next tile
#define TETRIS_PREVIEW_X	(MINIGAME_TETRIS_X_HIGH + 50)
#define TETRIS_PREVIEW_Y	(MINIGAME_TETRIS_Y_HIGH - 4 * TETRIS_BLOCKLENGTH)

UINT32 MiniGame_Handle_Tetris()
{
	static UINT32 uiTimeOfLastUpdate = GetJA2Clock( );

	if ( gMiniGame == TETRIS )
	{
		// coordinates for the background rectangle
		if ( gMiniGameRenewBackground )
		{
			SGPRect backgroundrect;
			backgroundrect.iLeft = MINIGAME_BG_X_LOW;
			backgroundrect.iTop = MINIGAME_BG_Y_LOW;
			backgroundrect.iRight = MINIGAME_BG_X_HIGH;
			backgroundrect.iBottom = MINIGAME_BG_Y_HIGH;

			// background: display a png picture
			std::string strImage = "Interface\\background_tetris_game.png";
			if ( gMiniGameState == MINIGAME_STARTSCREEN )
				strImage = "Interface\\background_tetris_startscreen.png";
			else if ( gMiniGameState == MINIGAME_GAMEOVER )
				strImage = "Interface\\background_tetris_gameover.png";

			DisplayPNGImage( backgroundrect, strImage );

			gMiniGameRenewBackground = FALSE;
		}
				
		UINT16 menu_x_low = MINIGAME_BG_X_LOW + 250;
		UINT16 menu_x_high = MINIGAME_BG_X_HIGH - 250;
		UINT16 menu_y_low = MINIGAME_BG_Y_LOW + 200;
		UINT16 menu_y_high = MINIGAME_BG_Y_HIGH - 200;

		INT32 fontused = LARGEFONT1;

		SetFontShadow( MILITARY_SHADOW );

		// we have to keep key presses on a delay, otherwise we move to fast
		BOOLEAN keyactivation_up	= FALSE;
		BOOLEAN keyactivation_down	= FALSE;
		BOOLEAN keyactivation_left	= FALSE;
		BOOLEAN keyactivation_right = FALSE;
		BOOLEAN keyactivation_enter	= FALSE;
		BOOLEAN keyactivation_p		= FALSE;

		UINT32 TETRIS_MENU_INPUTDELAY = 120;
		static int gTetristimesincelastmove = 0;
		if ( GetJA2Clock( ) - gTetristimesincelastmove > TETRIS_MENU_INPUTDELAY )
		{
			// arrow keys navigate the menu
			if ( gfKeyState[UPARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_up = TRUE;
			}

			if ( gfKeyState[DNARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_down = TRUE;
			}

			if ( gfKeyState[LEFTARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_left = TRUE;
			}

			if ( gfKeyState[RIGHTARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_right = TRUE;
			}

			if ( gfKeyState[ENTER] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_enter = TRUE;
			}

			// 'p' key is 80
			if ( gfKeyState[80] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_p = TRUE;
			}
		}
		
		InvalidateScreen( );

		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

		if ( keyactivation_p && gMiniGameState == MINIGAME_GAME )
		{
			gMiniGameState = MINIGAME_PAUSE;
			gMiniGameMenuScreen = 3;
			gMiniGameMenuSelection = 0;
		}

		if ( gMiniGameState == MINIGAME_STARTSCREEN )
		{
			// menu background
			ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			// display our score
			UINT16 sX = menu_x_low + 10;
			UINT16 sY = menu_y_low + 10;
			UINT16 width = 0;
						
			CHAR16		sText[800];
			swprintf( sText, L"" );
			
			// different options are here, the one currently selected is coloured differently
			if ( gMiniGameMenuScreen == 0 )
			{
				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> New Game" : L"   New Game" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE, 
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Options" : L"   Options" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Quit" : L"   Quit" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gMiniGameMenuSelection = max( 0, gMiniGameMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gMiniGameMenuSelection = min( menuentry - 1, gMiniGameMenuSelection + 1 );
				}
								
				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gMiniGameState = MINIGAME_GAME;

						gMiniGameRenewBackground = TRUE;
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gMiniGameMenuScreen = 1;

						gMiniGameRenewBackground = TRUE;
					}
					else if ( gMiniGameMenuSelection == 2 )
					{
						return MiniGameExit( );
					}
				}
			}
			else if ( gMiniGameMenuScreen == 1 )
			{
				// menu background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high + 100, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Starting Level: %d" : L"   Starting Level: %d", gTetrisInitialLevel );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Handicap: %d" : L"   Handicap: %d", gTetrisHandicap );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Apply" : L"   Apply" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gMiniGameMenuSelection = max( 0, gMiniGameMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gMiniGameMenuSelection = min( menuentry - 1, gMiniGameMenuSelection + 1 );
				}
				// arrow keys alter settings
				else if ( keyactivation_left )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gTetrisInitialLevel = max( 0, gTetrisInitialLevel - 1 );
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gTetrisHandicap = max( 0, gTetrisHandicap - 1 );
					}
				}
				else if ( keyactivation_right )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gTetrisInitialLevel = min( 20, gTetrisInitialLevel + 1 );
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gTetrisHandicap = min( TETRIS_MAX_HANDICAP, gTetrisHandicap + 1 );
					}
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gMiniGameMenuSelection == 2 )
					{
						gMiniGameMenuScreen = 0;
						gMiniGameMenuSelection = 1;

						gMiniGameRenewBackground = TRUE;
					}
				}
			}
		}
		else if ( gMiniGameState == MINIGAME_GAME )
		{
			UINT32 TETRIS_TICK_FREQUENCY = max( 150, 800 - (gTetrisInitialLevel + gTetrisLinesDone / 10) * 35 );
			
			BOOLEAN fDoLineUpdate = FALSE;

			if ( GetJA2Clock( ) - uiTimeOfLastUpdate > TETRIS_TICK_FREQUENCY )
			{
				uiTimeOfLastUpdate = GetJA2Clock( );
				fDoLineUpdate = TRUE;
			}

			// on start of the game, if we have a handicap, place handicap tiles
			if ( !gTetrisTileSpawnTime && gTetrisHandicap )
			{
				for ( int i = max(0, TETRIS_FIELD_ROWS - 2 * gTetrisHandicap); i < TETRIS_FIELD_ROWS; ++i )
				{
					// add several lone blocks per line
					int tilestoadd = min( 2 + Random( 4 ), TETRIS_FIELD_COLS - 1);

					while ( tilestoadd )
					{
						int j = Random( TETRIS_FIELD_COLS );

						if ( gTetrisBlock[i][j].blocktype == TETRISBLOCKTYPE_NONE )
						{
							gTetrisBlock[i][j].resting = TRUE;
							gTetrisBlock[i][j].playercontrolled = FALSE;
							gTetrisBlock[i][j].blocktype = 1 + Random( TETRISBLOCKTYPE_Z_INVERTED );

							--tilestoadd;
						}
					}
				}

				gTetrisTileSpawnTime = 1;
			}

			// coloured block that we can move
			int block_x_delta = 0;
			int block_y_delta = 0;

			BOOLEAN turnmove = FALSE;
			
			// we check keypresses to decide whether the player wants to move a piece
			// however, after doing a move, wait for a 'cooldown' - we don't want to move to fast
			if ( !fDoLineUpdate )
			{
				if ( keyactivation_left )
					block_x_delta += -1;

				if ( keyactivation_right )
					block_x_delta += 1;

				if ( keyactivation_up )
					turnmove = TRUE;

				if ( keyactivation_down )
					block_y_delta += 1;
			}

			// only continue if there  is something to do
			if ( fDoLineUpdate || block_x_delta || block_y_delta || turnmove )
			{
				// display our score
				UINT16 sX = TETRIS_PREVIEW_X + 10;
				UINT16 sY = MINIGAME_TETRIS_Y_LOW + 10;

				// score background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, TETRIS_PREVIEW_X, MINIGAME_TETRIS_Y_LOW, TETRIS_PREVIEW_X + 130, MINIGAME_TETRIS_Y_LOW + 80, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
					
				UINT16 width = 0;

				CHAR16		sText[800];
				swprintf( sText, L"" );

				swprintf( sText, L"Score: %d", gTetrisScore );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				swprintf( sText, L"Lines: %d", gTetrisLinesDone );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				swprintf( sText, L"Level: %d", gTetrisInitialLevel + gTetrisLinesDone / 10 );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				swprintf( sText, L"Handicap: %d", gTetrisHandicap );
				width = min( SCREEN_WIDTH - 2 * sX, StringPixLength( sText, fontused ) );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				if ( turnmove )
				{
					int anchor_x = gTetrisBlockPlayer[0].x;
					int anchor_y = gTetrisBlockPlayer[0].y;

					INT8 playerblocktype = gTetrisBlock[anchor_y][anchor_x].blocktype;

					// check whether new orientation can be applied
					BOOLEAN turnpossible = FALSE;

					int turntestcnt = 0;

					while ( !turnpossible && turntestcnt < 3)
					{
						++turntestcnt;

						turnpossible = TRUE;

						++gTetrisplayertileOrientation;
						if ( gTetrisplayertileOrientation >= TETRIS_ORIENTATIONS )
							gTetrisplayertileOrientation = 0;

						TETRIS_BLOCK_ORIENTATION orientation = gTetrisOrientation[playerblocktype][gTetrisplayertileOrientation];

						for ( int k = 0; k < 4; ++k )
						{
							int new_x = anchor_x + orientation.cords[k].x;
							int new_y = anchor_y + orientation.cords[k].y;

							if ( new_x < 0 ||
								 new_x >= TETRIS_FIELD_COLS ||
								 new_y < 0 ||
								 new_y >= TETRIS_FIELD_ROWS ||
								 (gTetrisBlock[new_y][new_x].blocktype != TETRISBLOCKTYPE_NONE && !gTetrisBlock[new_y][new_x].playercontrolled)
								 )
							{
								turnpossible = FALSE;
							}
						}

						// if turn is possible, well, move the tile
						if ( turnpossible )
						{
							for ( int k = 0; k < 4; ++k )
							{
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].resting = TRUE;
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].playercontrolled = FALSE;
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype = TETRISBLOCKTYPE_NONE;
							}

							for ( int k = 0; k < 4; ++k )
							{
								gTetrisBlockPlayer[k].x = anchor_x + orientation.cords[k].x;
								gTetrisBlockPlayer[k].y = anchor_y + orientation.cords[k].y;

								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].resting = FALSE;
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].playercontrolled = TRUE;
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype = playerblocktype;
							}

							PlayJA2Sample( COMPUTER_SWITCH1_OUT, RATE_11025, 15, 1, MIDDLEPAN );
						}
					}
				}

				if ( fDoLineUpdate || block_x_delta || block_y_delta )
				{
					BOOLEAN playercontrollost = FALSE;

					if ( fDoLineUpdate )
					{
						block_y_delta += 1;
					}

					if ( gTetrisplayercontrolledtile && (block_x_delta || block_y_delta) )
					{
						// move the player-controlled blocks
						INT8 playerblocktype = TETRISBLOCKTYPE_NONE;

						for ( int k = 0; k < 4; ++k )
						{
							int x = gTetrisBlockPlayer[k].x;
							int y = gTetrisBlockPlayer[k].y;

							playerblocktype = gTetrisBlock[y][x].blocktype;

							// check whether move is possible
							if ( block_x_delta &&
								 (x + block_x_delta < 0 ||
								 x + block_x_delta >= TETRIS_FIELD_COLS ||
								 (gTetrisBlock[y][x + block_x_delta].blocktype != TETRISBLOCKTYPE_NONE && !gTetrisBlock[y][x + block_x_delta].playercontrolled))
								 )
							{
								block_x_delta = 0;
							}

							if ( block_y_delta &&
								 (y + block_y_delta < 0 ||
								 y + block_y_delta >= TETRIS_FIELD_ROWS ||
								 (gTetrisBlock[y + block_y_delta][x].blocktype != TETRISBLOCKTYPE_NONE && !gTetrisBlock[y + block_y_delta][x].playercontrolled))
								 )
							{
								block_y_delta = 0;
							}

							gTetrisBlock[y][x].resting = TRUE;
							gTetrisBlock[y][x].playercontrolled = FALSE;
							gTetrisBlock[y][x].blocktype = TETRISBLOCKTYPE_NONE;
						}

						for ( int k = 0; k < 4; ++k )
						{
							gTetrisBlockPlayer[k].x += block_x_delta;
							gTetrisBlockPlayer[k].y += block_y_delta;
						}

						// set the new ones
						for ( int k = 0; k < 4; ++k )
						{
							gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].resting = FALSE;
							gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].playercontrolled = TRUE;
							gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype = playerblocktype;
						}

						if ( fDoLineUpdate && !block_y_delta )
						{
							playercontrollost = TRUE;

							// give points to the player, depending on how fast the block was placed

							// level of highest placed tile
							int highestlevel = TETRIS_FIELD_ROWS;
							for ( int i = 0; i < TETRIS_FIELD_ROWS; ++i )
							{
								for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
								{
									if ( gTetrisBlock[i][j].blocktype != TETRISBLOCKTYPE_NONE )
									{
										highestlevel = i;

										break;
									}
								}

								if ( highestlevel < TETRIS_FIELD_ROWS )
									break;
							}

							// time the tile would have taken if it dropped on its own, assuming it would land on the highest tile
							// tile spawned a bit lower, take that into account
							int maxdroptime = TETRIS_TICK_FREQUENCY * ((highestlevel - 4));

							if ( maxdroptime > 0 )
							{
								int timetaken = GetJA2Clock( ) - gTetrisTileSpawnTime;

								int speedbonuspoints = 10 * max( 0, (maxdroptime - timetaken) ) / maxdroptime;

								AddScore_Tetris( speedbonuspoints );
							}
						}
					}

					if ( fDoLineUpdate )
					{
						for ( int i = TETRIS_FIELD_ROWS - 1; i >= 0; --i )
						{
							BOOLEAN lowerline = TRUE;

							// first, check whether we can lower the line in the first place
							BOOLEAN emptyline = TRUE;

							for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
							{
								if ( i == TETRIS_FIELD_ROWS - 1 )
								{
									lowerline = FALSE;
									break;
								}
								else if ( gTetrisBlock[i][j].resting )
								{
									lowerline = FALSE;
									break;
								}
								else if ( !gTetrisBlock[i][j].playercontrolled && gTetrisBlock[i + 1][j].blocktype != TETRISBLOCKTYPE_NONE )
								{
									lowerline = FALSE;
									break;
								}

								if ( gTetrisBlock[i][j].blocktype != TETRISBLOCKTYPE_NONE )
								{
									emptyline = FALSE;
								}
							}

							// don't bother with empty lines
							if ( emptyline )
								lowerline = FALSE;

							if ( lowerline )
							{
								// there is space below us - move down
								for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
								{
									if ( !gTetrisBlock[i][j].playercontrolled )
									{
										gTetrisBlock[i + 1][j].resting = gTetrisBlock[i][j].resting;
										gTetrisBlock[i + 1][j].playercontrolled = gTetrisBlock[i][j].playercontrolled;
										gTetrisBlock[i + 1][j].blocktype = gTetrisBlock[i][j].blocktype;

										gTetrisBlock[i][j].resting = TRUE;
										gTetrisBlock[i][j].playercontrolled = FALSE;
										gTetrisBlock[i][j].blocktype = TETRISBLOCKTYPE_NONE;
									}
								}
							}
							else
							{
								// otherwise stop moving
								for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
								{
									if ( !gTetrisBlock[i][j].playercontrolled )
									{
										gTetrisBlock[i][j].resting = TRUE;
									}
								}
							}
						}
					}

					// if the player-controlled tile has been put down,
					// - release player control for all player-controlled blocks if one loses controls
					// - erase completed lines
					// - award points
					BOOLEAN erasecompleteline = FALSE;
					if ( playercontrollost )
					{
						for ( int i = 0; i < 4; ++i )
						{
							gTetrisBlock[gTetrisBlockPlayer[i].y][gTetrisBlockPlayer[i].x].playercontrolled = FALSE;
							gTetrisBlock[gTetrisBlockPlayer[i].y][gTetrisBlockPlayer[i].x].resting = TRUE;

							gTetrisBlockPlayer[i].x = 0;
							gTetrisBlockPlayer[i].y = 0;
						}

						gTetrisplayercontrolledtile = FALSE;

						PlayJA2Sample( COMPUTER_BEEP2_IN, RATE_11025, 15, 1, MIDDLEPAN );

						// 
						int linesdone = 0;
						for ( int i = TETRIS_FIELD_ROWS - 1; i >= 0; --i )
						{
							BOOLEAN eraseline = TRUE;

							for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
							{
								if ( gTetrisBlock[i][j].blocktype == TETRISBLOCKTYPE_NONE )
								{
									eraseline = FALSE;
								}

								// if a line has been erased below, we will fall down...
								if ( erasecompleteline )
								{
									gTetrisBlock[i][j].resting = FALSE;
								}
							}

							if ( eraseline )
							{
								erasecompleteline = TRUE;

								for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
								{
									gTetrisBlock[i][j].resting = TRUE;
									gTetrisBlock[i][j].playercontrolled = FALSE;
									gTetrisBlock[i][j].blocktype = TETRISBLOCKTYPE_NONE;
								}

								++linesdone;
							}
						}

						if ( linesdone )
						{
							switch ( linesdone )
							{
							case 1: AddScore_Tetris( 40 ); break;
							case 2: AddScore_Tetris( 100 ); break;
							case 3: AddScore_Tetris( 300 ); break;
							case 4:
								AddScore_Tetris( 1200 );

								if ( gusSelectedSoldier != NOBODY )
								{
									// Set to current
									SOLDIERTYPE* pSoldier = NULL;
									GetSoldier( &pSoldier, gusSelectedSoldier );

									if ( pSoldier )
									{
										pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
									}
								}
								break;
							}

							gTetrisLinesDone += linesdone;
						}
					}

					// if the plyer doesn't currently control a tile, add next one
					if ( !gTetrisplayercontrolledtile )
					{
						// check whether there are still tiles dropping... if not, add new tile
						BOOLEAN stilldropping = FALSE;
						for ( int i = TETRIS_FIELD_ROWS - 1; i >= 0; --i )
						{
							for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
							{
								if ( !gTetrisBlock[i][j].resting )
								{
									stilldropping = TRUE;
									break;
								}
							}
						}

						if ( !stilldropping )
						{
							// add new playercontrolled piece
							// check if entry space is free
							BOOLEAN spaceisfree = TRUE;

							if ( gTetrisNextTile == TETRISBLOCKTYPE_NONE )
								gTetrisNextTile = 1 + Random( TETRISBLOCKTYPE_Z_INVERTED );

							int playerblocktype = gTetrisNextTile;

							gTetrisNextTile = 1 + Random( TETRISBLOCKTYPE_Z_INVERTED );

							gTetrisplayertileOrientation = 0;

							TETRIS_BLOCK_ORIENTATION orientation = gTetrisOrientation[playerblocktype][gTetrisplayertileOrientation];

							int anchor_x = 4;
							int anchor_y = 1;

							for ( int k = 0; k < 4; ++k )
							{
								gTetrisBlockPlayer[k].x = anchor_x + orientation.cords[k].x;
								gTetrisBlockPlayer[k].y = anchor_y + orientation.cords[k].y;

								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].resting = FALSE;
								gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].playercontrolled = TRUE;

								if ( gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype != TETRISBLOCKTYPE_NONE )
								{
									gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype = TETRISBLOCKTYPE_CONFLICT;
									spaceisfree = FALSE;
								}
								else
								{
									gTetrisBlock[gTetrisBlockPlayer[k].y][gTetrisBlockPlayer[k].x].blocktype = playerblocktype;
								}
							}

							gTetrisplayercontrolledtile = TRUE;
							gTetrisTileSpawnTime = GetJA2Clock();

							if ( !spaceisfree )
							{
								gMiniGameState = MINIGAME_GAMEOVER;
								gMiniGameMenuScreen = 2;
								gMiniGameMenuSelection = 0;

								// game over, man, game over!
								if ( gusSelectedSoldier != NOBODY )
								{
									// Set to current
									SOLDIERTYPE* pSoldier = NULL;
									GetSoldier( &pSoldier, gusSelectedSoldier );

									if ( pSoldier )
									{
										pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
									}
								}
							}
						}
					}
				}

				// draw stuff
				{
					UINT16 col_red = Get16BPPColor( FROMRGB( 255, 0, 0 ) );
					UINT16 col_green = Get16BPPColor( FROMRGB( 0, 255, 0 ) );
					UINT16 col_blue = Get16BPPColor( FROMRGB( 0, 0, 255 ) );
					UINT16 col_yellow = Get16BPPColor( FROMRGB( 255, 255, 0 ) );
					UINT16 col_violet = Get16BPPColor( FROMRGB( 127, 0, 127 ) );
					UINT16 col_cyan = Get16BPPColor( FROMRGB( 0, 127, 127 ) );
					UINT16 col_orange = Get16BPPColor( FROMRGB( 255, 127, 0 ) );
					UINT16 col_pink = Get16BPPColor( FROMRGB( 255, 0, 255 ) );
					UINT16 col_grey = Get16BPPColor( FROMRGB( 55, 55, 55 ) );

					// draw blocks
					{
						// background
						ColorFillVideoSurfaceArea( FRAME_BUFFER, MINIGAME_TETRIS_X_LOW, MINIGAME_TETRIS_Y_LOW, MINIGAME_TETRIS_X_HIGH, MINIGAME_TETRIS_Y_HIGH, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

						for ( int i = 0; i < TETRIS_FIELD_ROWS; ++i )
						{
							for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
							{
								if ( gTetrisBlock[i][j].blocktype != TETRISBLOCKTYPE_NONE )
								{
									UINT16 x = MINIGAME_TETRIS_X_LOW + j * TETRIS_BLOCKLENGTH;
									UINT16 y = MINIGAME_TETRIS_Y_LOW + i * TETRIS_BLOCKLENGTH;

									UINT16 colour1 = 0;
									UINT16 colour2 = col_grey;

									switch ( gTetrisBlock[i][j].blocktype )
									{
									case TETRISBLOCKTYPE_L:				colour1 = col_red;			break;
									case TETRISBLOCKTYPE_L_INVERTED:	colour1 = col_green;		break;
									case TETRISBLOCKTYPE_T:				colour1 = col_blue;			break;
									case TETRISBLOCKTYPE_SQUARE:		colour1 = col_yellow;		break;
									case TETRISBLOCKTYPE_LONG:			colour1 = col_violet;		break;
									case TETRISBLOCKTYPE_Z:				colour1 = col_cyan;			break;
									case TETRISBLOCKTYPE_Z_INVERTED:	colour1 = col_orange;		break;
									case TETRISBLOCKTYPE_CONFLICT:		colour1 = col_pink;			break;
									case TETRISBLOCKTYPE_NONE:
									default:
										colour1 = Get16BPPColor( FROMRGB( 0, 0, 0 ) );
										colour2 = colour1;
										break;
									}

									ColorFillVideoSurfaceArea( FRAME_BUFFER, x, y, x + TETRIS_BLOCKLENGTH, y + TETRIS_BLOCKLENGTH, colour2 );
									ColorFillVideoSurfaceArea( FRAME_BUFFER, x + 1, y + 1, x + TETRIS_BLOCKLENGTH - 1, y + TETRIS_BLOCKLENGTH - 1, colour1 );
								}
							}
						}
					}

					// draw preview area
					{
						// we also need a small 4x4 area to display the next tile
						ColorFillVideoSurfaceArea( FRAME_BUFFER, TETRIS_PREVIEW_X, TETRIS_PREVIEW_Y, TETRIS_PREVIEW_X + 4 * TETRIS_BLOCKLENGTH, MINIGAME_TETRIS_Y_HIGH, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

						int preview_anchor_x = 2;
						int preview_anchor_y = 1;

						TETRIS_BLOCK_ORIENTATION previeworientation = gTetrisOrientation[gTetrisNextTile][0];

						UINT16 colour1 = 0;
						UINT16 colour2 = col_grey;

						switch ( gTetrisNextTile )
						{
						case TETRISBLOCKTYPE_L:				colour1 = col_red;			break;
						case TETRISBLOCKTYPE_L_INVERTED:	colour1 = col_green;		break;
						case TETRISBLOCKTYPE_T:				colour1 = col_blue;			break;
						case TETRISBLOCKTYPE_SQUARE:		colour1 = col_yellow;		break;
						case TETRISBLOCKTYPE_LONG:			colour1 = col_violet;		break;
						case TETRISBLOCKTYPE_Z:				colour1 = col_cyan;			break;
						case TETRISBLOCKTYPE_Z_INVERTED:	colour1 = col_orange;		break;
						case TETRISBLOCKTYPE_CONFLICT:		colour1 = col_pink;			break;
						case TETRISBLOCKTYPE_NONE:
						default:
							colour1 = Get16BPPColor( FROMRGB( 0, 0, 0 ) );
							colour2 = colour1;
							break;
						}

						for ( int k = 0; k < 4; ++k )
						{
							UINT16 x = TETRIS_PREVIEW_X + (preview_anchor_x + previeworientation.cords[k].x) * TETRIS_BLOCKLENGTH;
							UINT16 y = TETRIS_PREVIEW_Y + (preview_anchor_y + previeworientation.cords[k].y) * TETRIS_BLOCKLENGTH;

							ColorFillVideoSurfaceArea( FRAME_BUFFER, x, y, x + TETRIS_BLOCKLENGTH, y + TETRIS_BLOCKLENGTH, colour2 );
							ColorFillVideoSurfaceArea( FRAME_BUFFER, x + 1, y + 1, x + TETRIS_BLOCKLENGTH - 1, y + TETRIS_BLOCKLENGTH - 1, colour1 );
						}
					}
				}
			}
		}
		else if ( gMiniGameState == MINIGAME_GAMEOVER )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			// display our score
			UINT16 sX = menu_x_low + 10;
			UINT16 sY = menu_y_low + 10;
			UINT16 width = 0;

			CHAR16		sText[800];
			swprintf( sText, L"" );

			swprintf( sText, L"GAME OVER" );
			width = StringPixLength( sText, fontused );
			sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_RED, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

			if ( gMiniGameMenuScreen == 2 )
			{
				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Main Menu" : L"   Main Menu" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Quit" : L"   Quit" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gMiniGameMenuSelection = max( 0, gMiniGameMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gMiniGameMenuSelection = min( menuentry - 1, gMiniGameMenuSelection + 1 );
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						MiniGame_Init_Tetris();

						gMiniGameState = MINIGAME_STARTSCREEN;
						gMiniGameRenewBackground = TRUE;
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						return MiniGameExit( );
					}
				}
			}
		}
		else if ( gMiniGameState == MINIGAME_PAUSE )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high, menu_y_high - 20, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			// display our score
			UINT16 sX = menu_x_low + 10;
			UINT16 sY = menu_y_low + 10;
			UINT16 width = 0;

			CHAR16		sText[800];
			swprintf( sText, L"" );

			swprintf( sText, L"Game paused" );
			width = StringPixLength( sText, fontused );
			sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_RED, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

			if ( gMiniGameMenuScreen == 3 )
			{
				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Continue" : L"   Continue" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				if ( keyactivation_enter )
				{
					gMiniGameState = MINIGAME_GAME;

					gMiniGameRenewBackground = TRUE;
				}
			}
		}
	}
	else if ( gMiniGame == PONG )
	{

	}

	return MINIGAME_SCREEN;
}
////////////////////////////////////////////// TETRIS //////////////////////////////////////////////

////////////////////////////////////////////// PONG //////////////////////////////////////////////
// Pong is one of the very first computer games ever created.
// Basically, you have 2 players who each control a paddle on opposing sides of the playing field.
// A ball travels between the two sides, the goal is to prevent the other player from guiding the ball behind you.
// We implement that here with  few extra options (multiple players and such)

// the different types of objects we use 
enum
{
	PLAYER_PAD = 0,
	BALL,
};

// this struct is used to stre data of paddles and balls
typedef struct PONG_OBJECT
{
	PONG_OBJECT( ) : x( 0 ), y( 0 ), angle( 0 ), speed( 0 ), r( 1 ), type( BALL ), timepassedsincespeedup( GetJA2Clock( ) ), lastplayerthattouchedthis( 0 ), erasethis( FALSE ) {}

	double x;
	double y;
	
	double angle;
	double speed;

	double r;
	int type;

	int timepassedsincespeedup;			// the longer a ball is in play, the faster it gets (to prevent the game from going on forever).
	int lastplayerthattouchedthis;		// we log the last player paddle that touched a ball here

	BOOLEAN erasethis;					// marker: delete this thing during deletion procedure (ball going out of game area)
} PONG_OBJECT;

int gPongPlayers = 1;				// number of players (1 to 4), with 2 or 4 paddles used accordingly

// game area
#define MINIGAME_PONG_X_LOW		(MINIGAME_BG_X_LOW + 20)
#define MINIGAME_PONG_X_HIGH	((!gPongForce4Paddles && gPongPlayers < 3) ? MINIGAME_BG_X_HIGH - 100 : MINIGAME_PONG_X_LOW + 400 )
#define MINIGAME_PONG_Y_LOW		(MINIGAME_BG_Y_LOW + 60)
#define MINIGAME_PONG_Y_HIGH	((!gPongForce4Paddles && gPongPlayers < 3) ? MINIGAME_BG_Y_HIGH - 100 : MINIGAME_PONG_Y_LOW + 400)

#define PONG_MAX_SPEED	30		// max speed
#define PONG_SPEED_INCREASE_TIMER	10000

#define PONG_MAX_BALLS	100		// maximum number of balls

std::vector< PONG_OBJECT > gPongObjects;
int gPongBallsToUse = 1;
BOOLEAN gPongForce4Paddles = FALSE;
BOOLEAN gPongAIHard = FALSE;
int gPongMaxSpeed = PONG_MAX_SPEED / 2;		// max speed internally (the player can set this up to PONG_MAX_SPEED)
int gPongScorePlayer[5];

// as movement depends on time spent, we have to create an offset whenever we enter pause mode
int gPongTimePauseActivated = 0;
int gPongPauseTimeOffset = 0;

BOOLEAN gPongObjectsInit = FALSE;

void MiniGame_Init_Pong()
{
	gPongObjects.clear();

	for (int i = 0; i < 5; ++i )
		gPongScorePlayer[i] = 0;

	gPongTimePauseActivated = 0;
	gPongPauseTimeOffset = 0;

	gMiniGameMenuSelection = 0;
	gMiniGameMenuScreen = 0;

	gMiniGameRenewBackground = TRUE;

	gPongObjectsInit = FALSE;
}

UINT32 MiniGame_Handle_Pong()
{
	static UINT32 uiTimeOfLastUpdate = GetJA2Clock( );

	if ( gMiniGame == PONG )
	{
		// coordinates for the background rectangle
		if ( gMiniGameRenewBackground )
		{
			SGPRect backgroundrect;
			backgroundrect.iLeft = MINIGAME_BG_X_LOW;
			backgroundrect.iTop = MINIGAME_BG_Y_LOW;
			backgroundrect.iRight = MINIGAME_BG_X_HIGH;
			backgroundrect.iBottom = MINIGAME_BG_Y_HIGH;

			// background: display a png picture
			std::string strImage = "Interface\\background_pong_game.png";
			if ( gMiniGameState == MINIGAME_STARTSCREEN )
				strImage = "Interface\\background_tetris_startscreen.png";
			else if ( gMiniGameState == MINIGAME_GAMEOVER )
				strImage = "Interface\\background_tetris_gameover.png";

			DisplayPNGImage( backgroundrect, strImage );

			gMiniGameRenewBackground = FALSE;
		}

		UINT16 menu_x_low = MINIGAME_BG_X_LOW + 250;
		UINT16 menu_x_high = MINIGAME_BG_X_HIGH - 200;
		UINT16 menu_y_low = MINIGAME_BG_Y_LOW + 200;
		UINT16 menu_y_high = MINIGAME_BG_Y_HIGH - 150;

		INT32 fontused = LARGEFONT1;

		SetFontShadow( MILITARY_SHADOW );

		// we have to keep key presses on a delay, otherwise we move to fast
		BOOLEAN keyactivation_up = FALSE;
		BOOLEAN keyactivation_down = FALSE;
		BOOLEAN keyactivation_left = FALSE;
		BOOLEAN keyactivation_right = FALSE;
		BOOLEAN keyactivation_enter = FALSE;
		BOOLEAN keyactivation_p = FALSE;

		UINT32 TETRIS_MENU_INPUTDELAY = 120;
		static int gTetristimesincelastmove = 0;
		if ( GetJA2Clock( ) - gTetristimesincelastmove > TETRIS_MENU_INPUTDELAY )
		{
			// arrow keys navigate the menu
			if ( gfKeyState[UPARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_up = TRUE;
			}

			if ( gfKeyState[DNARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_down = TRUE;
			}

			if ( gfKeyState[LEFTARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_left = TRUE;
			}

			if ( gfKeyState[RIGHTARROW] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_right = TRUE;
			}

			if ( gfKeyState[ENTER] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_enter = TRUE;
			}

			// 'p' key is 80
			if ( gfKeyState[80] )
			{
				gTetristimesincelastmove = GetJA2Clock( );

				keyactivation_p = TRUE;
			}
		}

		InvalidateScreen( );

		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

		if ( keyactivation_p && gMiniGameState == MINIGAME_GAME )
		{
			gMiniGameState = MINIGAME_PAUSE;
			gMiniGameMenuScreen = 3;
			gMiniGameMenuSelection = 0;

			gPongTimePauseActivated = GetJA2Clock();
			gPongPauseTimeOffset = 0;
		}

		if ( gMiniGameState == MINIGAME_STARTSCREEN )
		{
			// menu background
			ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			// display our score
			UINT16 sX = menu_x_low + 10;
			UINT16 sY = menu_y_low + 10;
			UINT16 width = 0;

			CHAR16		sText[800];
			swprintf( sText, L"" );

			// different options are here, the one currently selected is coloured differently
			if ( gMiniGameMenuScreen == 0 )
			{
				int menuentry = 0;

				swprintf( sText, L"%s %d Players", (gMiniGameMenuSelection == menuentry) ? L"->" : L"  ", gPongPlayers );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Options" : L"   Options" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Quit" : L"   Quit" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gMiniGameMenuSelection = max( 0, gMiniGameMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gMiniGameMenuSelection = min( menuentry - 1, gMiniGameMenuSelection + 1 );
				}
				// arrow keys alter settings
				else if ( keyactivation_left )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gPongPlayers = max( 1, gPongPlayers - 1);
					}
				}
				else if ( keyactivation_right )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gPongPlayers = min( 4, gPongPlayers + 1 );
					}
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gMiniGameState = MINIGAME_GAME;

						gMiniGameRenewBackground = TRUE;
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gMiniGameMenuScreen = 1;

						gMiniGameRenewBackground = TRUE;
					}
					else if ( gMiniGameMenuSelection == 2 )
					{
						return MiniGameExit( );
					}
				}
			}
			else if ( gMiniGameMenuScreen == 1 )
			{
				// menu background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high + 100, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Max Speed: %d" : L"   Max Speed: %d", gPongMaxSpeed );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Balls: %d" : L"   Balls: %d", gPongBallsToUse );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, L"%s Force 4 Paddles: %s", (gMiniGameMenuSelection == menuentry) ? L"->" : L"  ", gPongForce4Paddles ? L"True" : L"False" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, L"%s AI: %s", (gMiniGameMenuSelection == menuentry) ? L"->" : L"  ", gPongAIHard ? L"Hard" : L"Easy" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Apply" : L"   Apply" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gMiniGameMenuSelection = max( 0, gMiniGameMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gMiniGameMenuSelection = min( menuentry - 1, gMiniGameMenuSelection + 1 );
				}
				// arrow keys alter settings
				else if ( keyactivation_left )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gPongMaxSpeed = max( 1, gPongMaxSpeed - 1 );
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gPongBallsToUse = max( 1, gPongBallsToUse - 1 );
					}
					else if ( gMiniGameMenuSelection == 2 )
					{
						gPongForce4Paddles = FALSE;
					}
					else if ( gMiniGameMenuSelection == 3 )
					{
						gPongAIHard = FALSE;
					}
				}
				else if ( keyactivation_right )
				{
					if ( gMiniGameMenuSelection == 0 )
					{
						gPongMaxSpeed = min( PONG_MAX_SPEED, gPongMaxSpeed + 1 );
					}
					else if ( gMiniGameMenuSelection == 1 )
					{
						gPongBallsToUse = min( PONG_MAX_BALLS, gPongBallsToUse + 1 );
					}
					else if ( gMiniGameMenuSelection == 2 )
					{
						gPongForce4Paddles = TRUE;
					}
					else if ( gMiniGameMenuSelection == 3 )
					{
						gPongAIHard = TRUE;
					}
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gMiniGameMenuSelection == menuentry - 1 )
					{
						gMiniGameMenuScreen = 0;
						gMiniGameMenuSelection = 1;

						gMiniGameRenewBackground = TRUE;
					}
				}
			}
		}
		else if ( gMiniGameState == MINIGAME_GAME )
		{
			// we also have to account for any time we spent in pause mode
			int timesincelasttick = GetJA2Clock( ) - uiTimeOfLastUpdate - gPongPauseTimeOffset;

			for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin(); it != gPongObjects.end(); ++it )
			{
				if ( it->type == BALL )
				{
					it->timepassedsincespeedup += gPongPauseTimeOffset;
				}
			}

			gPongPauseTimeOffset = 0;
			uiTimeOfLastUpdate = GetJA2Clock( );

			if ( !gPongObjectsInit )
			{
				PONG_OBJECT player1paddle;
				player1paddle.x = MINIGAME_PONG_X_LOW + (MINIGAME_PONG_X_HIGH - MINIGAME_PONG_X_LOW) / 2;
				player1paddle.y = MINIGAME_PONG_Y_HIGH;
				player1paddle.r = 20;
				player1paddle.type = PLAYER_PAD;

				gPongObjects.push_back( player1paddle );

				PONG_OBJECT player2paddle;
				player2paddle.x = MINIGAME_PONG_X_LOW + (MINIGAME_PONG_X_HIGH - MINIGAME_PONG_X_LOW) / 2;
				player2paddle.y = MINIGAME_PONG_Y_LOW;
				player2paddle.r = 20;
				player2paddle.type = PLAYER_PAD;

				gPongObjects.push_back( player2paddle );

				if ( gPongForce4Paddles || gPongPlayers >= 3 )
				{
					PONG_OBJECT player3paddle;
					player3paddle.x = MINIGAME_PONG_X_LOW;
					player3paddle.y = MINIGAME_PONG_Y_LOW + (MINIGAME_PONG_Y_HIGH - MINIGAME_PONG_Y_LOW) / 2;
					player3paddle.r = 20;
					player3paddle.type = PLAYER_PAD;

					gPongObjects.push_back( player3paddle );

					PONG_OBJECT player4paddle;
					player4paddle.x = MINIGAME_PONG_X_HIGH;
					player4paddle.y = MINIGAME_PONG_Y_LOW + (MINIGAME_PONG_Y_HIGH - MINIGAME_PONG_Y_LOW) / 2;
					player4paddle.r = 20;
					player4paddle.type = PLAYER_PAD;

					gPongObjects.push_back( player4paddle );
				}

				if ( gPongBallsToUse > 3 && gusSelectedSoldier != NOBODY )
				{
					// Set to current
					SOLDIERTYPE* pSoldier = NULL;
					GetSoldier( &pSoldier, gusSelectedSoldier );

					if ( pSoldier )
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					}
				}

				gPongObjectsInit = TRUE;
			}

			// check whether we need to add a ball
			int ballsinplay = 0;
			for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin(); it != gPongObjects.end(); ++it )
			{
				if ( it->type == BALL )
					++ballsinplay;
			}

			// as balls can get erased, we always check whether we have to add new ones
			while ( ballsinplay < gPongBallsToUse )
			{
				PONG_OBJECT ball;
				ball.x = MINIGAME_PONG_X_LOW + (MINIGAME_PONG_X_HIGH - MINIGAME_PONG_X_LOW) / 2 - 50 + Random(100);
				ball.y = MINIGAME_PONG_Y_LOW + (MINIGAME_PONG_Y_HIGH - MINIGAME_PONG_Y_LOW) / 2 - 30 + Random(60);
				ball.angle = Random(10);
				ball.speed = min( gPongMaxSpeed, gPongMaxSpeed / 2 + Random( gPongMaxSpeed ) );
				ball.r = 4;

				gPongObjects.push_back( ball );

				++ballsinplay;
			}

			// iterators to player-paddles
			// note: as we use iterators and check against gPongObjects.end( ) to know whether a paddle exists, erasing elements should only be done at the very end
			std::vector<PONG_OBJECT>::iterator it_player1 = gPongObjects.end( );
			std::vector<PONG_OBJECT>::iterator it_player2 = gPongObjects.end( );
			std::vector<PONG_OBJECT>::iterator it_player3 = gPongObjects.end( );
			std::vector<PONG_OBJECT>::iterator it_player4 = gPongObjects.end( );

			// for the 'AI' we need to know which ball will be the next one to hit our side
			std::vector<PONG_OBJECT>::iterator it_nextballfor_player2 = gPongObjects.end( );
			std::vector<PONG_OBJECT>::iterator it_nextballfor_player3 = gPongObjects.end( );
			std::vector<PONG_OBJECT>::iterator it_nextballfor_player4 = gPongObjects.end( );

			double nextballfor_player2_ydist = 99999;
			double nextballfor_player3_ydist = 99999;
			double nextballfor_player4_ydist = 99999;
			int cnt = 0;
			for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin(); it != gPongObjects.end(); ++it )
			{
				if ( it->type == PLAYER_PAD )
				{
					// also, control stuff
					if ( cnt == 0 )
						it_player1 = it;
					else if ( cnt == 1 )
						it_player2 = it;
					else if ( cnt == 2 )
						it_player3 = it;
					else if ( cnt == 3 )
						it_player4 = it;
				}
				else if ( it->type == BALL )
				{
					// only balls that travel in our direction are important
					if ( cos( it->angle ) < 0 )
					{
						double dist = it->y - MINIGAME_PONG_Y_LOW;

						if ( dist < nextballfor_player2_ydist )
						{
							nextballfor_player2_ydist = dist;
							it_nextballfor_player2 = it;
						}
					}

					if ( sin( it->angle ) < 0 )
					{
						double dist = it->x - MINIGAME_PONG_X_LOW;

						if ( dist < nextballfor_player3_ydist )
						{
							nextballfor_player3_ydist = dist;
							it_nextballfor_player3 = it;
						}
					}

					if ( sin( it->angle ) > 0 )
					{
						double dist = MINIGAME_PONG_X_HIGH - it->x;

						if ( dist < nextballfor_player4_ydist )
						{
							nextballfor_player4_ydist = dist;
							it_nextballfor_player4 = it;
						}
					}
				}

				++cnt;
			}
			
			if ( it_player1 != gPongObjects.end() )
			{
				it_player1->angle = 0;
				it_player1->speed = 0;

				if ( gfKeyState[LEFTARROW] )
				{
					it_player1->angle -= PI / 2;
					it_player1->speed = gPongMaxSpeed;
				}
				if ( gfKeyState[RIGHTARROW] )
				{
					it_player1->angle += PI / 2;
					it_player1->speed = gPongMaxSpeed;
				}
			}

			if ( it_player2 != gPongObjects.end( ) )
			{
				it_player2->angle = 0;
				it_player2->speed = 0;

				if ( !gPongForce4Paddles && gPongPlayers >= 2 )
				{
					// control with 4 and 6 in numblock
					if ( gfKeyState[239] )
					{
						it_player2->angle -= PI / 2;
						it_player2->speed = gPongMaxSpeed;
					}
					if ( gfKeyState[241] )
					{
						it_player2->angle += PI / 2;
						it_player2->speed = gPongMaxSpeed;
					}
				}
				// 'AI'
				else
				{
					if ( it_nextballfor_player2 != gPongObjects.end() )
					{
						double offset = gPongAIHard ? 0 : it_player2->r;

						if ( it_nextballfor_player2->x < it_player2->x + offset )
						{
							it_player2->angle -= PI / 2;
							it_player2->speed = gPongMaxSpeed;
						}
						else if ( it_nextballfor_player2->x > it_player2->x - offset )
						{
							it_player2->angle += PI / 2;
							it_player2->speed = gPongMaxSpeed;
						}
					}
				}
			}

			if ( it_player3 != gPongObjects.end( ) )
			{
				it_player3->angle = 0;
				it_player3->speed = 0;

				if ( !gPongForce4Paddles && gPongPlayers >= 3 )
				{
					// 'w'
					if ( gfKeyState[87] )
					{
						it_player3->angle += PI;
						it_player3->speed = gPongMaxSpeed;
					}
					// 's'
					if ( gfKeyState[83] )
					{
						it_player3->angle -= 0;
						it_player3->speed = gPongMaxSpeed;
					}
				}
				// 'AI'
				else
				{
					if ( it_nextballfor_player3 != gPongObjects.end( ) )
					{
						double offset = gPongAIHard ? 0 : it_player3->r;

						if ( it_nextballfor_player3->y < it_player3->y + offset )
						{
							it_player3->angle += PI;
							it_player3->speed = gPongMaxSpeed;
						}
						else if ( it_nextballfor_player3->y > it_player3->y - offset )
						{
							it_player3->angle -= 0;
							it_player3->speed = gPongMaxSpeed;
						}
					}
				}
			}

			if ( it_player4 != gPongObjects.end( ) )
			{
				it_player4->angle = 0;
				it_player4->speed = 0;

				if ( !gPongForce4Paddles && gPongPlayers >= 4 )
				{
					// 'i'
					if ( gfKeyState[73] )
					{
						it_player4->angle += PI;
						it_player4->speed = gPongMaxSpeed;
					}
					// 'k'
					if ( gfKeyState[75] )
					{
						it_player4->angle -= 0;
						it_player4->speed = gPongMaxSpeed;
					}
				}
				// 'AI'
				else
				{
					if ( it_nextballfor_player4 != gPongObjects.end( ) )
					{
						double offset = gPongAIHard ? 0 : it_player4->r;

						if ( it_nextballfor_player4->y < it_player4->y + offset )
						{
							it_player4->angle += PI;
							it_player4->speed = gPongMaxSpeed;
						}
						else if ( it_nextballfor_player4->y > it_player4->y - offset )
						{
							it_player4->angle -= 0;
							it_player4->speed = gPongMaxSpeed;
						}
					}
				}
			}
						
			// move stuff
			// only if time has passed
			if ( timesincelasttick > 0 )
			{
				// display our score
				UINT16 sX = (!gPongForce4Paddles && gPongPlayers < 3) ? MINIGAME_BG_X_LOW + 200 : MINIGAME_BG_X_LOW + 100;
				UINT16 sY = MINIGAME_BG_Y_LOW + 10;

				// score background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, sX - 10, sY - 2, (!gPongForce4Paddles && gPongPlayers < 3) ? sX + 220 : sX + 500, sY + 20, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
				
				CHAR16		sText[800];
				swprintf( sText, L"" );

				CHAR16		player1name[20];
				swprintf( player1name, L"Player 1" );
								
				CHAR16		player2name[20];
				swprintf( player2name, L"Player 2" );

				CHAR16		player3name[20];
				swprintf( player3name, L"Player 3" );

				CHAR16		player4name[20];
				swprintf( player4name, L"Player 4" );

				// get the name of the players - use the currently controlled mercs and anybody nearby for this
				BOOLEAN foundplayer_1 = FALSE;
				BOOLEAN foundplayer_2 = FALSE;
				BOOLEAN foundplayer_3 = FALSE;
				BOOLEAN foundplayer_4 = FALSE;
				INT32 player1gridno = NOWHERE;
				SOLDIERTYPE*		pSoldier = NULL;
				if ( gusSelectedSoldier != NOBODY )
				{
					GetSoldier( &pSoldier, gusSelectedSoldier );

					if ( pSoldier )
					{
						swprintf( player1name, pSoldier->GetName( ) );
						player1gridno = pSoldier->sGridNo;
					}
				}

				UINT16				bMercID = 0;
				for ( pSoldier = MercPtrs[bMercID]; bMercID <= TOTAL_SOLDIERS; ++bMercID, ++pSoldier )
				{
					// everybody other merc in the same sector gets annoyed
					if ( bMercID != gusSelectedSoldier && pSoldier->bActive && pSoldier->bInSector )
					{						
						if ( PythSpacesAway( player1gridno, pSoldier->sGridNo ) < 6 )
						{
							if ( !foundplayer_2 )
							{
								swprintf( player2name, pSoldier->GetName( ) );
								foundplayer_2 = TRUE;

								if ( !gPongForce4Paddles && gPongPlayers < 3 )
									break;
							}
							else if ( gPongForce4Paddles || gPongPlayers > 2 )
							{
								if ( !foundplayer_3 )
								{
									swprintf( player3name, pSoldier->GetName( ) );
									foundplayer_3 = TRUE;
								}
								else if ( !foundplayer_4 )
								{
									swprintf( player4name, pSoldier->GetName( ) );
									foundplayer_4 = TRUE;

									break;
								}
							}
						}
					}
				}

				if ( !gPongForce4Paddles && gPongPlayers < 3 )
					swprintf( sText, L"%s %d - %s %d", player1name, gPongScorePlayer[1], player2name, gPongScorePlayer[2] );
				else
					swprintf( sText, L"%s %d - %s %d - %s %d - %s %d", player1name, gPongScorePlayer[1], player2name, gPongScorePlayer[2], player3name, gPongScorePlayer[3], player4name, gPongScorePlayer[4] );

				sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				// display key bindings
				sX = MINIGAME_PONG_X_HIGH + 20;
				sY = MINIGAME_PONG_Y_LOW;

				swprintf( sText, L"Key bindings" );
				sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				swprintf( sText, L"%s: <, >", player1name );
				sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				if ( !gPongForce4Paddles )
				{
					if ( gPongPlayers >= 2 )
					{
						swprintf( sText, L"%s: 4, 6", player2name );
						sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
					}

					if ( gPongPlayers >= 3 )
					{
						swprintf( sText, L"%s: w, s", player3name );
						sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
					}

					if ( gPongPlayers >= 4 )
					{
						swprintf( sText, L"%s: i, k", player4name );
						sY += DisplayWrappedString( sX, sY, StringPixLength( sText, fontused ), 2, fontused, FONT_WHITE, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
					}
				}

				// movement is meant per, say, 100 ticks
				FLOAT movefraction = (FLOAT)(timesincelasttick) / 50.0f;

				if ( movefraction > 0.0f )
				{
					// due to our use of iterators, we only erase objects later on
					// this might seem somewhat odd, but makes use of iterators easier
					BOOLEAN eraseobject = FALSE;

					for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin(); it != gPongObjects.end(); ++it )
					{
						double delta_x = sin( it->angle ) * it->speed;
						double delta_y = cos( it->angle ) * it->speed;

						if ( it->type == PLAYER_PAD )
						{
							if ( it == it_player1 || it == it_player2 )
							{
								// for safety
								delta_y = 0;
							}
							else if ( it == it_player3 || it == it_player4 )
							{
								// for safety
								delta_x = 0;
							}
						}
						
						it->x += movefraction * delta_x;
						it->y += movefraction * delta_y;

						if ( it->type == BALL )
						{
							if ( GetJA2Clock( ) - it->timepassedsincespeedup > PONG_SPEED_INCREASE_TIMER )
							{
								it->speed = min( gPongMaxSpeed, it->speed + 1 );

								it->timepassedsincespeedup = 0;
							}
						}
						
						if ( it->type == PLAYER_PAD )
						{
							if ( it == it_player1 || it == it_player2 )
							{
								// for safety
								it->x = min( MINIGAME_PONG_X_HIGH - it->r, max( MINIGAME_PONG_X_LOW + it->r, it->x ) );
							}
							else if ( it == it_player3 || it == it_player4 )
							{
								// for safety
								it->y = min( MINIGAME_PONG_Y_HIGH - it->r, max( MINIGAME_PONG_Y_LOW + it->r, it->y ) );
							}
						}
						
						double factor_move_offset = 0.1f;
						double factor_paddle_pos_offset = 0.1f;
												
						if ( it->type == BALL )
						{
							// check whether we will be reflected by a paddle
							if ( it->y + it->r > MINIGAME_PONG_Y_HIGH )
							{
								if ( it_player1 != gPongObjects.end() )
								{
									if ( it->x >= it_player1->x - it_player1->r && it->x <= it_player1->x + it_player1->r )
									{
										it->y = MINIGAME_PONG_Y_HIGH - it->r;

										// if a paddle moves, add an offset
										double paddle_move_offset = PI - it_player1->angle;

										// add an offset depending on where th paddle is hit - the further the hit is from the center, the bigger the offset
										double paddle_pos_offset = ((it->x - it_player1->x) / it_player1->r) * PI / 2;

										// we need the perpendicular
										double n = 2 * PI;
										it->angle = 2 * n + PI - it->angle + factor_move_offset * paddle_move_offset + factor_paddle_pos_offset * paddle_pos_offset;

										it->lastplayerthattouchedthis = 1;

										PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, 15, 1, MIDDLEPAN );
									}
								}
							}

							if ( it->y - it->r < MINIGAME_PONG_Y_LOW )
							{
								if ( it_player2 != gPongObjects.end( ) )
								{
									if ( it->x >= it_player2->x - it_player2->r && it->x <= it_player2->x + it_player2->r )
									{
										it->y = MINIGAME_PONG_Y_LOW + it->r;
										
										// if a paddle moves, add an offset
										double paddle_move_offset = PI - it_player2->angle;

										// add an offset depending on where th paddle is hit - the further the hit is from the center, the bigger the offset
										double paddle_pos_offset = ((it->x - it_player2->x) / it_player2->r) * PI / 2;

										// we need the perpendicular
										double n = PI;
										it->angle = 2 * n + PI - it->angle + factor_move_offset * paddle_move_offset + factor_paddle_pos_offset * paddle_pos_offset;

										it->lastplayerthattouchedthis = 2;

										PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, 15, 1, MIDDLEPAN );
									}
								}
							}

							// depending on how many players there are, there are paddles or walls on the sides
							if ( it->x - it->r < MINIGAME_PONG_X_LOW )
							{
								if ( it_player3 != gPongObjects.end( ) )
								{
									if ( it->y >= it_player3->y - it_player3->r && it->y <= it_player3->y + it_player3->r )
									{
										it->x = MINIGAME_PONG_X_LOW + it->r;

										// if a paddle moves, add an offset
										double paddle_move_offset = PI - it_player3->angle;

										// add an offset depending on where th paddle is hit - the further the hit is from the center, the bigger the offset
										double paddle_pos_offset = ((it->y - it_player3->y) / it_player3->r) * PI / 2;

										// we need the perpendicular
										double n = PI / 2;
										it->angle = 2 * n + PI - it->angle + factor_move_offset * paddle_move_offset + factor_paddle_pos_offset * paddle_pos_offset;

										it->lastplayerthattouchedthis = 3;

										PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, 15, 1, MIDDLEPAN );
									}
								}
								else
								{
									it->x = MINIGAME_PONG_X_LOW + it->r;

									double n = PI / 2;
									it->angle = 2 * n + PI - it->angle;
								}
							}
							else if ( it->x + it->r > MINIGAME_PONG_X_HIGH )
							{
								if ( it_player4 != gPongObjects.end( ) )
								{
									if ( it->y >= it_player4->y - it_player4->r && it->y <= it_player4->y + it_player4->r )
									{
										it->x = MINIGAME_PONG_X_HIGH - it->r;

										// if a paddle moves, add an offset
										double paddle_move_offset = PI - it_player4->angle;

										// add an offset depending on where th paddle is hit - the further the hit is from the center, the bigger the offset
										double paddle_pos_offset = ((it->y - it_player4->y) / it_player4->r) * PI / 2;

										// we need the perpendicular
										double n = - PI / 2;
										it->angle = 2 * n + PI - it->angle + factor_move_offset * paddle_move_offset + factor_paddle_pos_offset * paddle_pos_offset;

										it->lastplayerthattouchedthis = 4;

										PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, 15, 1, MIDDLEPAN );
									}
								}
								else
								{
									it->x = MINIGAME_PONG_X_HIGH - it->r;

									double n = -PI / 2;
									it->angle = 2 * n + PI - it->angle;
								}
							}

							// check whether we will be reflected by another ball
							std::vector<PONG_OBJECT>::iterator it2 = it;
							++it2;
							for ( ; it2 != gPongObjects.end(); ++it2)
							{
								if ( it2->type == BALL )
								{
									if ( sqrt( (it->x - it2->x) * (it->x - it2->x) + (it->y - it2->y) * (it->y - it2->y) ) <= it->r )
									{
										double n2 = PI / 2 + it2->angle;
										double n1 = PI / 2 + it->angle;
										it->angle = 2 * n2 + PI - it->angle;
										it2->angle = 2 * n1 + PI - it2->angle;

										double tmp = it->speed;
										it->speed = it2->speed;
										it2->speed = tmp;
										
										double delta_x = sin( it2->angle ) * it2->speed;
										double delta_y = cos( it2->angle ) * it2->speed;
										
										it2->x += movefraction * delta_x;
										it2->y += movefraction * delta_y;

										PlayJA2Sample( VSM_SWITCH2_OUT, RATE_11025, 15, 1, MIDDLEPAN );
									}
								}
							}
							
							// award points to the last player that touched the ball (if he doesn't somehow knock it into his own side)
							if ( it->y < MINIGAME_PONG_Y_LOW )
							{
								eraseobject = TRUE;
								it->erasethis = TRUE;

								if ( it->lastplayerthattouchedthis != 2 )
									gPongScorePlayer[it->lastplayerthattouchedthis] += 1;
							}
							else if ( it->y > MINIGAME_PONG_Y_HIGH )
							{
								eraseobject = TRUE;
								it->erasethis = TRUE;

								if ( it->lastplayerthattouchedthis != 1 )
									gPongScorePlayer[it->lastplayerthattouchedthis] += 1;
							}
							else if ( (gPongForce4Paddles || gPongPlayers >= 3) && it->x < MINIGAME_PONG_X_LOW )
							{
								eraseobject = TRUE;
								it->erasethis = TRUE;

								if ( it->lastplayerthattouchedthis != 3 )
									gPongScorePlayer[it->lastplayerthattouchedthis] += 1;
							}
							else if ( (gPongForce4Paddles || gPongPlayers >= 3) && it->x > MINIGAME_PONG_X_HIGH )
							{
								eraseobject = TRUE;
								it->erasethis = TRUE;

								if ( it->lastplayerthattouchedthis != 4 )
									gPongScorePlayer[it->lastplayerthattouchedthis] += 1;
							}
						}
					}

					// erase objects now
					if ( eraseobject )
					{
						for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin(); it != gPongObjects.end(); )
						{
							if ( it->erasethis )
								it = gPongObjects.erase( it );
							else
								++it;
						}

						PlayJA2Sample( BIG_SWITCH3_OUT, RATE_11025, 15, 1, MIDDLEPAN );
					}
				}
			}

			// draw stuff
			{
				UINT16 col_green = Get16BPPColor( FROMRGB( 0, 255, 0 ) );
				UINT16 col_white = Get16BPPColor( FROMRGB( 255, 255, 255 ) );

				// background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, MINIGAME_PONG_X_LOW, MINIGAME_PONG_Y_LOW, MINIGAME_PONG_X_HIGH, MINIGAME_PONG_Y_HIGH, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

				for ( std::vector<PONG_OBJECT>::iterator it = gPongObjects.begin( ); it != gPongObjects.end( ); ++it )
				{
					switch ( it->type )
					{
					case PLAYER_PAD:
						{
							if ( it == it_player1 || it == it_player2 )
							{
								ColorFillVideoSurfaceArea( FRAME_BUFFER, 
															max( MINIGAME_PONG_X_LOW, it->x - it->r), 
															max( MINIGAME_PONG_Y_LOW, it->y - 7),
															min( MINIGAME_PONG_X_HIGH, it->x + it->r),
															min( MINIGAME_PONG_Y_HIGH, it->y + 7),
															col_green );
							}
							else
							{
								ColorFillVideoSurfaceArea( FRAME_BUFFER,
														   max( MINIGAME_PONG_X_LOW, it->x - 7 ),
														   max( MINIGAME_PONG_Y_LOW, it->y - it->r ),
														   min( MINIGAME_PONG_X_HIGH, it->x + 7 ),
														   min( MINIGAME_PONG_Y_HIGH, it->y + it->r ),
														   col_green );
							}
						}
						break;

					case BALL:
						{
							for ( int x = it->x - it->r + 1; x <= it->x + it->r; ++x )
							{
								// colour x, y if
								// (it->x - x)^2 + (it->y - y)^2 <= r^2
								// <=> (it->y - y) <= +/- sqrt( r^2 - (it->x - x)^2 )
								// <=> y >= it->y +/- sqrt( r^2 - (it->x - x)^2 )
								int sqrstuff = sqrt( it->r * it->r - (it->x - x) * (it->x - x) );
								
								ColorFillVideoSurfaceArea( FRAME_BUFFER, 
														   max( MINIGAME_PONG_X_LOW, x),
														   max( MINIGAME_PONG_Y_LOW, it->y - sqrstuff),
														   min( MINIGAME_PONG_X_HIGH, x + 1),
														   min( MINIGAME_PONG_Y_HIGH, it->y + sqrstuff),
														   col_white );
							}
						}
						break;

					default:
						break;
					}
				}
			}
			
		}
		else if ( gMiniGameState == MINIGAME_GAMEOVER )
		{

		}
		else if ( gMiniGameState == MINIGAME_PAUSE )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high, menu_y_high - 20, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			// display our score
			UINT16 sX = menu_x_low + 10;
			UINT16 sY = menu_y_low + 10;
			UINT16 width = 0;

			CHAR16		sText[800];
			swprintf( sText, L"" );

			swprintf( sText, L"Game paused" );
			width = StringPixLength( sText, fontused );
			sY += DisplayWrappedString( sX, sY, width, 2, fontused, FONT_RED, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

			if ( gMiniGameMenuScreen == 3 )
			{
				int menuentry = 0;

				swprintf( sText, (gMiniGameMenuSelection == menuentry) ? L"-> Continue" : L"   Continue" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gMiniGameMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gMiniGameMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				if ( keyactivation_enter )
				{
					gMiniGameState = MINIGAME_GAME;

					gMiniGameRenewBackground = TRUE;

					gPongPauseTimeOffset = GetJA2Clock( ) - gPongTimePauseActivated;
					gPongTimePauseActivated = 0;
				}
			}
		}
	}

	return MINIGAME_SCREEN;
}
////////////////////////////////////////////// PONG //////////////////////////////////////////////
