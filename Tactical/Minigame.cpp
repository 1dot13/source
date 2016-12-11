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

	default:
		break;
	}

	return MINIGAME_SCREEN;
}

UINT32	MiniGameScreenShutdown( void )
{
	return TRUE;
}

////////////////////////////////////////////// TETRIS //////////////////////////////////////////////
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

typedef struct
{
	BOOLEAN resting;
	BOOLEAN playercontrolled;
	INT8 blocktype;
} TETRIS_BLOCK;

typedef struct
{
	int x;
	int y;
} TETRIS_BLOCK_PLAYER;

#define TETRIS_ORIENTATIONS 4

typedef struct coordpair
{
	int x;
	int y;

	coordpair( int a, int b ) : x( a ), y( b ) {}
	coordpair( ){}
} coordpair;

typedef struct TETRIS_BLOCK_ORIENTATION
{
	coordpair cords[TETRIS_ORIENTATIONS];

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

// the dimensions of the playing field
#define TETRIS_FIELD_ROWS	18
#define TETRIS_FIELD_COLS	10

TETRIS_BLOCK gTetrisBlock[TETRIS_FIELD_ROWS][TETRIS_FIELD_COLS];

// rememeber the tiles the player controls (easier than to always look for them)
TETRIS_BLOCK_PLAYER gTetrisBlockPlayer[4];

BOOLEAN gTetrisplayercontrolledtile = FALSE;
int gTetrisTimeSinceTileSpawned = 0;
int gTetrisplayertileOrientation = 0;
int gTetrisNextTile = TETRISBLOCKTYPE_NONE;
int gTetrisScore = 0;
int gTetrisLinesDone = 0;
int gTetrisMenuSelection = 0;
int gTetrisMenuScreen = 0;
int gTetrisInitialLevel = 0;
int gTetrisHandicap = 0;
BOOLEAN gTetrisRenewBackground = TRUE;

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
	gTetrisTimeSinceTileSpawned = 0;
	gTetrisplayertileOrientation = 0;
	gTetrisNextTile = TETRISBLOCKTYPE_NONE;
	gTetrisScore = 0;
	gTetrisLinesDone = 0;
		
	gTetrisMenuSelection = 0;
	gTetrisMenuScreen = 0;
	gTetrisRenewBackground = TRUE;

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

#define MINIGAME_BG_X_LOW	iScreenWidthOffset
#define MINIGAME_BG_X_HIGH	(SCREEN_WIDTH - iScreenWidthOffset)
#define MINIGAME_BG_Y_LOW	iScreenHeightOffset
#define MINIGAME_BG_Y_HIGH	(SCREEN_HEIGHT - iScreenHeightOffset)

#define TETRIS_BLOCKLENGTH	20

// game area
#define MINIGAME_X_LOW		(MINIGAME_BG_X_LOW + 100)
#define MINIGAME_X_HIGH		(MINIGAME_X_LOW + TETRIS_FIELD_COLS * TETRIS_BLOCKLENGTH)
#define MINIGAME_Y_LOW		(MINIGAME_BG_Y_LOW + 100)
#define MINIGAME_Y_HIGH		(MINIGAME_Y_LOW + TETRIS_FIELD_ROWS * TETRIS_BLOCKLENGTH)

// we also need a small 4x4 area to display the next tile
#define TETRIS_PREVIEW_X	(MINIGAME_X_HIGH + 50)
#define TETRIS_PREVIEW_Y	(MINIGAME_Y_HIGH - 4 * TETRIS_BLOCKLENGTH)

UINT32 MiniGame_Handle_Tetris()
{
	static UINT32 uiTimeOfLastUpdate = GetJA2Clock( );

	if ( gMiniGame == TETRIS )
	{
		// coordinates for the background rectangle
		if ( gTetrisRenewBackground )
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

			gTetrisRenewBackground = FALSE;
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
			gTetrisMenuScreen = 3;
			gTetrisMenuSelection = 0;
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
			if ( gTetrisMenuScreen == 0 )
			{
				int menuentry = 0;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> New Game" : L"   New Game" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE, 
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Options" : L"   Options" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Quit" : L"   Quit" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gTetrisMenuSelection = max( 0, gTetrisMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gTetrisMenuSelection = min( menuentry - 1, gTetrisMenuSelection + 1 );
				}
								
				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gTetrisMenuSelection == 0 )
					{
						gMiniGameState = MINIGAME_GAME;

						gTetrisRenewBackground = TRUE;
					}
					else if ( gTetrisMenuSelection == 1 )
					{
						gTetrisMenuScreen = 1;

						gTetrisRenewBackground = TRUE;
					}
					else if ( gTetrisMenuSelection == 2 )
					{
						return MiniGameExit( );
					}
				}
			}
			else if ( gTetrisMenuScreen == 1 )
			{
				// menu background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, menu_x_low, menu_y_low, menu_x_high + 100, menu_y_high, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

				int menuentry = 0;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Starting Level: %d" : L"   Starting Level: %d", gTetrisInitialLevel );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Handicap: %d" : L"   Handicap: %d", gTetrisHandicap );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Apply" : L"   Apply" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_WHITE,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gTetrisMenuSelection = max( 0, gTetrisMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gTetrisMenuSelection = min( menuentry - 1, gTetrisMenuSelection + 1 );
				}
				// arrow keys alter settings
				else if ( keyactivation_left )
				{
					if ( gTetrisMenuSelection == 0 )
					{
						gTetrisInitialLevel = max( 0, gTetrisInitialLevel - 1 );
					}
					else if ( gTetrisMenuSelection == 1 )
					{
						gTetrisHandicap = max( 0, gTetrisHandicap - 1 );
					}
				}
				else if ( keyactivation_right )
				{
					if ( gTetrisMenuSelection == 0 )
					{
						gTetrisInitialLevel = min( 20, gTetrisInitialLevel + 1 );
					}
					else if ( gTetrisMenuSelection == 1 )
					{
						gTetrisHandicap = min( TETRIS_MAX_HANDICAP, gTetrisHandicap + 1 );
					}
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gTetrisMenuSelection == 2 )
					{
						gTetrisMenuScreen = 0;
						gTetrisMenuSelection = 1;

						gTetrisRenewBackground = TRUE;
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
			if ( !gTetrisTimeSinceTileSpawned && gTetrisHandicap )
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

				gTetrisTimeSinceTileSpawned = 1;
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
				UINT16 sY = MINIGAME_Y_LOW + 10;

				// score background
				ColorFillVideoSurfaceArea( FRAME_BUFFER, TETRIS_PREVIEW_X, MINIGAME_Y_LOW, TETRIS_PREVIEW_X + 130, MINIGAME_Y_LOW + 80, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
					
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
								int timetaken = GetJA2Clock( ) - gTetrisTimeSinceTileSpawned;

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
							gTetrisTimeSinceTileSpawned = GetJA2Clock();

							if ( !spaceisfree )
							{
								gMiniGameState = MINIGAME_GAMEOVER;
								gTetrisMenuScreen = 2;
								gTetrisMenuSelection = 0;

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
						ColorFillVideoSurfaceArea( FRAME_BUFFER, MINIGAME_X_LOW, MINIGAME_Y_LOW, MINIGAME_X_HIGH, MINIGAME_Y_HIGH, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

						for ( int i = 0; i < TETRIS_FIELD_ROWS; ++i )
						{
							for ( int j = 0; j < TETRIS_FIELD_COLS; ++j )
							{
								if ( gTetrisBlock[i][j].blocktype != TETRISBLOCKTYPE_NONE )
								{
									UINT16 x = MINIGAME_X_LOW + j * TETRIS_BLOCKLENGTH;
									UINT16 y = MINIGAME_Y_LOW + i * TETRIS_BLOCKLENGTH;

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
						ColorFillVideoSurfaceArea( FRAME_BUFFER, TETRIS_PREVIEW_X, TETRIS_PREVIEW_Y, TETRIS_PREVIEW_X + 4 * TETRIS_BLOCKLENGTH, MINIGAME_Y_HIGH, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

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

			if ( gTetrisMenuScreen == 2 )
			{
				int menuentry = 0;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Main Menu" : L"   Main Menu" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Quit" : L"   Quit" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;
				
				// arrow keys navigate the menu
				if ( keyactivation_up )
				{
					gTetrisMenuSelection = max( 0, gTetrisMenuSelection - 1 );
				}
				else if ( keyactivation_down )
				{
					gTetrisMenuSelection = min( menuentry - 1, gTetrisMenuSelection + 1 );
				}

				// ENTER selects menu entry
				if ( keyactivation_enter )
				{
					if ( gTetrisMenuSelection == 0 )
					{
						MiniGame_Init_Tetris();

						gMiniGameState = MINIGAME_STARTSCREEN;
						gTetrisRenewBackground = TRUE;
					}
					else if ( gTetrisMenuSelection == 1 )
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

			if ( gTetrisMenuScreen == 3 )
			{
				int menuentry = 0;

				swprintf( sText, (gTetrisMenuSelection == menuentry) ? L"-> Continue" : L"   Continue" );
				width = StringPixLength( sText, fontused );
				sY += DisplayWrappedString( sX, sY, width, 2, fontused, (gTetrisMenuSelection == menuentry) ? FONT_BLACK : FONT_RED,
											sText, (gTetrisMenuSelection == menuentry) ? FONT_MCOLOR_WHITE : FONT_MCOLOR_BLACK, FALSE, 0 );
				++menuentry;

				if ( keyactivation_enter )
				{
					gMiniGameState = MINIGAME_GAME;

					gTetrisRenewBackground = TRUE;
				}
			}
		}
	}

	return MINIGAME_SCREEN;
}

////////////////////////////////////////////// TETRIS //////////////////////////////////////////////
