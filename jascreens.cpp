#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <time.h>
	#include "sgp.h"
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "vobject_private.h"
	#include "vobject_blitters.h"
	#include "wcheck.h"
	#include "input.h"
	#include "font.h"
	#include "timer.h"
	#include "mousesystem.h"
	#include "screenids.h"
	#include "screens.h"
	#include "Font Control.h"
	#include "sysutil.h"
	#include "renderworld.h"
	#include "tiledef.h"
	#include "worlddef.h"
	#include "editscreen.h"
	#include <wchar.h>
	#include <tchar.h>
	#include "Timer Control.h"
	#include "Sys Globals.h"
	#include "interface.h"
	#include "overhead.h"
	#include "utilities.h"
	#include "render dirty.h"
	#include "jascreens.h"
	#include "gameloop.h"
	#include "cursors.h"
	#include "Event Pump.h"
	#include "environment.h"
	#include "animation cache.h"
	#include "lighting.h"
	#include "interface panels.h"
	#include "Music Control.h"
	#include "mainmenuscreen.h"
	#include "Game Init.h"
	#include "init.h"
	#include "cursor control.h"
	#include "utilities.h"
	#include "GameVersion.h"
	#include "game clock.h"
	#include "gamescreen.h"
	#include "english.h"
	#include "Random.h"
	#include "WordWrap.h"
	#include "Sound Control.h"
	#include "fade screen.h"
	#include "WordWrap.h"
	#include "Multi Language Graphic Utils.h"
	#include "text.h"
	#include "Language Defines.h"
#endif

#define _UNICODE
// Networking Stuff
#ifdef NETWORKED

#include "Communication.h"
#include "Application Msg.h"
#include "Networking.h"

extern ClientReadyType				gMsgClientReady;
extern BOOLEAN			gfAmINetworked;
#endif

#define MAX_DEBUG_PAGES 4


// GLOBAL FOR PAL EDITOR
UINT8	CurrentPalette = 0;
UINT32	guiBackgroundRect;
BOOLEAN	gfExitPalEditScreen = FALSE;
BOOLEAN	gfExitDebugScreen = FALSE;
BOOLEAN gfInitRect = TRUE;
static BOOLEAN FirstTime = TRUE;
BOOLEAN	gfDoneWithSplashScreen = FALSE;



void PalEditRenderHook(	);
BOOLEAN PalEditKeyboardHook( InputAtom *pInputEvent );

void DebugRenderHook(	);
BOOLEAN DebugKeyboardHook( InputAtom *pInputEvent );
INT8 gCurDebugPage = 0;


HVSURFACE			hVAnims[ 7 ];
INT8					bTitleAnimFrame = 0;
UINT32				uiTitleAnimTime = 0;
UINT32				uiDoTitleAnimTime = 0;
BOOLEAN				gfDoTitleAnimation = FALSE;
BOOLEAN				gfStartTitleAnimation = FALSE;


void DefaultDebugPage1( );
void DefaultDebugPage2( );
void DefaultDebugPage3( );
void DefaultDebugPage4( );
RENDER_HOOK				gDebugRenderOverride[ MAX_DEBUG_PAGES ] =
{ (RENDER_HOOK)DefaultDebugPage1, (RENDER_HOOK)DefaultDebugPage2,
	(RENDER_HOOK)DefaultDebugPage3, (RENDER_HOOK)DefaultDebugPage4 };

extern HVSURFACE ghFrameBuffer;

void DisplayFrameRate( )
{
	static UINT32		uiFPS = 0;
	static UINT32		uiFrameCount = 0;
	INT16 sMapPos;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;

	// Increment frame count
	uiFrameCount++;

	if ( COUNTERDONE( FPSCOUNTER ) )
	{
		// Reset counter
		RESETCOUNTER( FPSCOUNTER );

		uiFPS = uiFrameCount;
		uiFrameCount = 0;
	}

	// Create string
	SetFont( SMALLFONT1 );

	//DebugMsg(TOPIC_JA2, DBG_LEVEL_0, String( "FPS: %d ", __min( uiFPS, 1000 ) ) );

	if ( uiFPS < 20 )
	{
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_LTRED );
	}
	else
	{
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}

	if ( gbFPSDisplay == SHOW_FULL_FPS )
	{
		// FRAME RATE
		memset( &VideoOverlayDesc, 0, sizeof( VideoOverlayDesc ) );
		swprintf( VideoOverlayDesc.pzText, L"%ld", __min( uiFPS, 1000 ) );
		VideoOverlayDesc.uiFlags	= VOVERLAY_DESC_TEXT;
		UpdateVideoOverlay( &VideoOverlayDesc, giFPSOverlay, FALSE );

		// TIMER COUNTER
		swprintf( VideoOverlayDesc.pzText, L"%ld", __min( giTimerDiag, 1000 ) );
		VideoOverlayDesc.uiFlags	= VOVERLAY_DESC_TEXT;
		UpdateVideoOverlay( &VideoOverlayDesc, giCounterPeriodOverlay, FALSE );


		if( GetMouseMapPos( &sMapPos) )
		{
			//gprintfdirty( 0, 315, L"(%d)",sMapPos);
			//mprintf( 0,315,L"(%d)",sMapPos);
		}
		else
		{
			//gprintfdirty( 0, 315, L"(%d %d)",gusMouseXPos, gusMouseYPos - INTERFACE_START_Y );
			//mprintf( 0,315,L"(%d %d)",gusMouseXPos, gusMouseYPos - INTERFACE_START_Y );
		}
	}

	if ( ( gTacticalStatus.uiFlags & GODMODE ) )
	{
		SetFont( SMALLFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKRED );
		//gprintfdirty( 0, 0, L"GOD MODE" );
		//mprintf( 0, 0, L"GOD MODE" );
	}

	if ( ( gTacticalStatus.uiFlags & DEMOMODE ) )
	{
		SetFont( SMALLFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKGRAY );
		//gprintfdirty( 0, 0, L"DEMO MODE" );
		//mprintf( 0, 0, L"DEMO MODE" );
	}

#ifdef _DEBUG

	SetFont( SMALLFONT1 );
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_MCOLOR_DKGRAY );

	if ( gbFPSDisplay == SHOW_FULL_FPS )
	{
		// Debug
		if (gDebugStr[0] != '\0')
		{
			//gprintfdirty( 0, 345, L"DEBUG: %S",gDebugStr);
			//mprintf( 0,345,L"DEBUG: %S",gDebugStr);
		}

		if (gSystemDebugStr[0] != '\0')
		{
			//gprintfdirty( 0, 345, L"%S",gSystemDebugStr);
			//mprintf( 0,345,L"%S",gSystemDebugStr);
			gSystemDebugStr[0] = '\0';
		}

		// Print Num tiles
		//gprintfdirty( 0, 280, L"%d Tiles", gTileDatabaseSize );
		//mprintf( 0, 280, L"%d Tiles", gTileDatabaseSize );

		// Print tile mem usage
		//gprintfdirty( 0, 300, L"%d kB", guiMemTotal / 1024 );
		//mprintf( 0, 300, L"%d kB", guiMemTotal / 1024 );

	}
#endif

}


//USELESS!!!!!!!!!!!!!!!!!!
UINT32 SavingScreenInitialize(void)
{
	return( TRUE );
}
UINT32 SavingScreenHandle( void )
{
	return SAVING_SCREEN;
}
UINT32 SavingScreenShutdown( void )
{
	return TRUE;
}

UINT32 LoadingScreenInitialize(void)
{
	return( TRUE );
}
UINT32 LoadingScreenHandle(void)
{
	return LOADING_SCREEN;
}
UINT32 LoadingScreenShutdown(void)
{
	return( TRUE );
}



UINT32 ErrorScreenInitialize(void)
{
	return( TRUE );
}

UINT32 ErrorScreenHandle(void)
{
	InputAtom	InputEvent;
	static BOOLEAN	fFirstTime = FALSE;
#if defined ( JA2BETAVERSION ) || defined ( FORCE_ASSERTS_ON )
	CHAR16 str[256];
#endif

	// For quick setting of new video stuff / to be changed
	StartFrameBufferRender( );

	// Create string
	SetFont( LARGEFONT1 );
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_MCOLOR_LTGRAY );
	mprintf( 50, 200, L"RUNTIME ERROR" );

	mprintf( 50, 225, L"PRESS <ESC> TO EXIT" );

	SetFont( FONT12ARIAL );
	SetFontForeground( FONT_YELLOW );
	SetFontShadow( 60 );		//60 is near black
	mprintf( 50, 255, L"%S", gubErrorText );
	SetFontForeground( FONT_LTRED );


#if defined ( JA2BETAVERSION ) || defined ( FORCE_ASSERTS_ON )

	if( gubAssertString[0] )
	{
		swprintf( str, L"%S", gubAssertString );
		DisplayWrappedString( 50, 270, 560, 2, FONT12ARIAL, FONT_RED, str, FONT_BLACK, TRUE, LEFT_JUSTIFIED );
	}
#endif

	if ( !fFirstTime )
	{
		DebugMsg(TOPIC_JA2, DBG_LEVEL_0, String( "Runtime Error: %s ", gubErrorText ) );
		fFirstTime = TRUE;
	}

	// For quick setting of new video stuff / to be changed
	InvalidateScreen( );
	EndFrameBufferRender( );

	// Check for esc
	while (DequeueEvent(&InputEvent) == TRUE)
	{
		if( InputEvent.usEvent == KEY_DOWN )
			{
				if( InputEvent.usParam == ESC || InputEvent.usParam == 'x' && InputEvent.usKeyState & ALT_DOWN )
				{ // Exit the program
					DebugMsg(TOPIC_GAME, DBG_LEVEL_0, "GameLoop: User pressed ESCape, TERMINATING");

					// handle shortcut exit
					HandleShortCutExitState( );
				}
			}
	}

	return( ERROR_SCREEN );
}

UINT32 ErrorScreenShutdown(void)
{
	return( TRUE );
}



UINT32 InitScreenInitialize(void)
{
	return( TRUE );
}

UINT32 InitScreenHandle(void)
{
	VSURFACE_DESC					vs_desc;
	static HVSURFACE			hVSurface;
	static UINT8					ubCurrentScreen = 255;

	if ( ubCurrentScreen == 255 )
	{
	#ifdef ENGLISH
		if( gfDoneWithSplashScreen )
		{
			ubCurrentScreen = 0;
		}
		else
		{
			SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
			return( INTRO_SCREEN );
		}
	#else
		ubCurrentScreen = 0;
	#endif
	}

	if ( ubCurrentScreen == 0 )
	{
		if ( strcmp( gzCommandLine, "-NODD" ) == 0 )
		{
			gfDontUseDDBlits = TRUE;
		}

		// Load version number....
		//HandleLimitedNumExecutions( );

		// Load init screen and blit!
		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

		strcpy( vs_desc.ImageFile, "ja2_logo.STI" );

		hVSurface = CreateVideoSurface( &vs_desc );
//SB: JA2 Gold Rus doesn't contain such file!
#ifndef RUSSIAN
		if( !hVSurface )
			AssertMsg( 0, "Failed to load ja2_logo.sti!" );
#endif
		//BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, 0, 0, VS_BLT_FAST, NULL );
		ubCurrentScreen = 1;

		// Init screen

		// Set Font
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		//mprintf( 10, 420, zVersionLabel );

#ifdef _DEBUG
		mprintf( 10, 10, L"%s: %s Debug %S", pMessageStrings[ MSG_VERSION ], zVersionLabel, czVersionNumber );
#else
		mprintf( 10, 10, L"%s: %s %S", pMessageStrings[ MSG_VERSION ], zVersionLabel, czVersionNumber );
#endif

#ifdef CRIPPLED_VERSION

		mprintf( 10, 0, L"(LIMITED PRESS PREVIEW VERSION)" );

#elif defined JA2BETAVERSION

		mprintf( 10, 0, L"(Beta version error reporting enabled)" );

#endif

#ifdef _DEBUG
		mprintf( 10, 20, L"SOLDIERTYPE: %d bytes", sizeof( SOLDIERTYPE ) );
#endif

		if ( gfDontUseDDBlits )
		{
			#ifdef _DEBUG
				mprintf( 10, 10, L"SOLDIERTYPE: %d bytes", sizeof( SOLDIERTYPE ) );
			#else
				mprintf( 10, 20, L"Using software blitters" );
			#endif
		}

		InvalidateScreen( );

		// Delete video Surface
//SB: JA2 Gold Rus doesn't contain such file!
#ifdef RUSSIAN
		if( hVSurface )
#endif
		DeleteVideoSurface( hVSurface );
		//ATE: Set to true to reset before going into main screen!

		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

		return( INIT_SCREEN );
	}

	if ( ubCurrentScreen == 1 )
	{
		ubCurrentScreen = 2;
		return( InitializeJA2( ) );
	}

	if ( ubCurrentScreen == 2 )
	{
		InitMainMenu( );
		ubCurrentScreen = 3;
		return( INIT_SCREEN );
	}

	// Let one frame pass....
	if ( ubCurrentScreen == 3 )
	{
		ubCurrentScreen = 4;
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
		return( INIT_SCREEN );
	}

	if ( ubCurrentScreen == 4 )
	{
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
		InitNewGame( FALSE );
	}
	return( INIT_SCREEN );
}


UINT32 InitScreenShutdown(void)
{

	return( TRUE );
}



UINT32 PalEditScreenInit(void)
{
	return( TRUE );
}

UINT32 PalEditScreenHandle(void)
{
	static BOOLEAN FirstTime = TRUE;

	if ( gfExitPalEditScreen )
	{
		gfExitPalEditScreen = FALSE;
		FirstTime = TRUE;
		FreeBackgroundRect( guiBackgroundRect );
		SetRenderHook( (RENDER_HOOK)NULL );
		SetUIKeyboardHook( (UIKEYBOARD_HOOK)NULL );
		return( GAME_SCREEN );
	}

	if ( FirstTime )
	{
		FirstTime = FALSE;

		SetRenderHook( (RENDER_HOOK)PalEditRenderHook );
		SetUIKeyboardHook( (UIKEYBOARD_HOOK)PalEditKeyboardHook );

		guiBackgroundRect = RegisterBackgroundRect( BGND_FLAG_PERMANENT, NULL, 50, 10, 600 , 400);

	}
	else
	{

		(*(GameScreens[GAME_SCREEN].HandleScreen))();

	}

	return( PALEDIT_SCREEN );

}

UINT32 PalEditScreenShutdown(void)
{

	return( TRUE );
}


void PalEditRenderHook(	)
{
	SOLDIERTYPE		*pSoldier;

	if ( gusSelectedSoldier != NOBODY )
	{
		// Set to current
		GetSoldier( &pSoldier, gusSelectedSoldier );

		DisplayPaletteRep( pSoldier->HeadPal, 50, 10, FRAME_BUFFER );
		DisplayPaletteRep( pSoldier->PantsPal, 50, 50, FRAME_BUFFER );
		DisplayPaletteRep( pSoldier->VestPal, 50, 90, FRAME_BUFFER );
		DisplayPaletteRep( pSoldier->SkinPal, 50, 130, FRAME_BUFFER );

	}
}

BOOLEAN PalEditKeyboardHook( InputAtom *pInputEvent )
{
	UINT8					ubType;
	SOLDIERTYPE		*pSoldier;
	UINT8					ubPaletteRep;
	UINT32				cnt;
	UINT8					ubStartRep = 0;
	UINT8					ubEndRep = 0;

	if ( gusSelectedSoldier == NOBODY )
	{
		return( FALSE );
	}

	if ((pInputEvent->usEvent == KEY_DOWN )&& ( pInputEvent->usParam == ESC ))
	{
		gfExitPalEditScreen = TRUE;
		return( TRUE );
	}

	if ((pInputEvent->usEvent == KEY_DOWN )&& ( pInputEvent->usParam == 'h' ))
	{
			// Get Soldier
			GetSoldier( &pSoldier, gusSelectedSoldier );

			// Get index of current
			CHECKF( GetPaletteRepIndexFromID( pSoldier->HeadPal, &ubPaletteRep ) );
			ubType = gpPalRep[ ubPaletteRep ].ubType;

			ubPaletteRep++;

			// Count start and end index
			for ( cnt = 0; cnt < ubType; cnt++ )
			{
				ubStartRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ cnt ]);
			}

			ubEndRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ ubType ]);

			if ( ubPaletteRep == ubEndRep )
			{
				ubPaletteRep = ubStartRep;
			}
			SET_PALETTEREP_ID ( pSoldier->HeadPal,	gpPalRep[ ubPaletteRep ].ID );

			pSoldier->CreateSoldierPalettes( );

			return( TRUE );
	}


	if ((pInputEvent->usEvent == KEY_DOWN )&& ( pInputEvent->usParam == 'v' ))
	{
			// Get Soldier
			GetSoldier( &pSoldier, gusSelectedSoldier );

			// Get index of current
			CHECKF( GetPaletteRepIndexFromID( pSoldier->VestPal, &ubPaletteRep ) );
			ubType = gpPalRep[ ubPaletteRep ].ubType;

			ubPaletteRep++;

			// Count start and end index
			for ( cnt = 0; cnt < ubType; cnt++ )
			{
				ubStartRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ cnt ]);
			}

			ubEndRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ ubType ]);

			if ( ubPaletteRep == ubEndRep )
			{
				ubPaletteRep = ubStartRep;
			}
			SET_PALETTEREP_ID ( pSoldier->VestPal,	gpPalRep[ ubPaletteRep ].ID );

			pSoldier->CreateSoldierPalettes( );

			return( TRUE );
	}

	if ((pInputEvent->usEvent == KEY_DOWN )&& ( pInputEvent->usParam == 'p' ))
	{
			// Get Soldier
			GetSoldier( &pSoldier, gusSelectedSoldier );

			// Get index of current
			CHECKF( GetPaletteRepIndexFromID( pSoldier->PantsPal, &ubPaletteRep ) );
			ubType = gpPalRep[ ubPaletteRep ].ubType;

			ubPaletteRep++;

			// Count start and end index
			for ( cnt = 0; cnt < ubType; cnt++ )
			{
				ubStartRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ cnt ]);
			}

			ubEndRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ ubType ]);

			if ( ubPaletteRep == ubEndRep )
			{
				ubPaletteRep = ubStartRep;
			}
			SET_PALETTEREP_ID ( pSoldier->PantsPal,	gpPalRep[ ubPaletteRep ].ID );

			pSoldier->CreateSoldierPalettes( );

			return( TRUE );
	}

	if ((pInputEvent->usEvent == KEY_DOWN )&& ( pInputEvent->usParam == 's' ))
	{
			// Get Soldier
			GetSoldier( &pSoldier, gusSelectedSoldier );

			// Get index of current
			CHECKF( GetPaletteRepIndexFromID( pSoldier->SkinPal, &ubPaletteRep ) );
			ubType = gpPalRep[ ubPaletteRep ].ubType;

			ubPaletteRep++;

			// Count start and end index
			for ( cnt = 0; cnt < ubType; cnt++ )
			{
				ubStartRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ cnt ]);
			}

			ubEndRep = (UINT8)(ubStartRep + gubpNumReplacementsPerRange[ ubType ]);

			if ( ubPaletteRep == ubEndRep )
			{
				ubPaletteRep = ubStartRep;
			}
			SET_PALETTEREP_ID ( pSoldier->SkinPal,	gpPalRep[ ubPaletteRep ].ID );

			pSoldier->CreateSoldierPalettes( );

			return( TRUE );
	}

	return( FALSE );
}

UINT32 DebugScreenInit(void)
{
	return( TRUE );
}


BOOLEAN CheckForAndExitTacticalDebug( )
{
	if ( gfExitDebugScreen )
	{
		FirstTime = TRUE;
		gfInitRect = TRUE;
		gfExitDebugScreen = FALSE;
		FreeBackgroundRect( guiBackgroundRect );
		SetRenderHook( (RENDER_HOOK)NULL );
		SetUIKeyboardHook( (UIKEYBOARD_HOOK)NULL );

		return( TRUE );
	}

	return( FALSE );
}

void ExitDebugScreen( )
{
	if ( guiCurrentScreen == DEBUG_SCREEN )
	{
		gfExitDebugScreen = TRUE;
	}

	CheckForAndExitTacticalDebug( );
}


UINT32 DebugScreenHandle(void)
{
	if ( CheckForAndExitTacticalDebug() )
	{
		return( GAME_SCREEN );
	}

	if ( gfInitRect )
	{
		guiBackgroundRect = RegisterBackgroundRect( BGND_FLAG_PERMANENT, NULL, 0, 0, 600 , 360);
		gfInitRect = FALSE;
	}


	if ( FirstTime )
	{
		FirstTime = FALSE;

		SetRenderHook( (RENDER_HOOK)DebugRenderHook );
		SetUIKeyboardHook( (UIKEYBOARD_HOOK)DebugKeyboardHook );

	}
	else
	{

		(*(GameScreens[GAME_SCREEN].HandleScreen))();

	}

	return( DEBUG_SCREEN );

}

UINT32 DebugScreenShutdown(void)
{

	return( TRUE );
}


void DebugRenderHook(	)
{
	gDebugRenderOverride[ gCurDebugPage ]( );
}

BOOLEAN DebugKeyboardHook( InputAtom *pInputEvent )
{
	if ((pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == 'q' ))
	{
		gfExitDebugScreen = TRUE;
		return( TRUE );
	}

	if ((pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == PGUP ))
	{
		// Page down
		gCurDebugPage++;

		if ( gCurDebugPage == MAX_DEBUG_PAGES )
		{
			gCurDebugPage = 0;
		}

		FreeBackgroundRect( guiBackgroundRect );
		gfInitRect = TRUE;

	}

	if ((pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == PGDN ))
	{
		// Page down
		gCurDebugPage--;

		if ( gCurDebugPage < 0 )
		{
			gCurDebugPage = MAX_DEBUG_PAGES-1;
		}

		FreeBackgroundRect( guiBackgroundRect );
		gfInitRect = TRUE;

	}

	return( FALSE );
}


void SetDebugRenderHook( RENDER_HOOK pDebugRenderOverride, INT8 ubPage )
{
	gDebugRenderOverride[ ubPage ] = pDebugRenderOverride;
}

void DefaultDebugPage1( )
{
	SetFont( LARGEFONT1 );
	gprintf( 0,0,L"DEBUG PAGE ONE" );
}

void DefaultDebugPage2( )
{
	SetFont( LARGEFONT1 );
	gprintf( 0,0,L"DEBUG PAGE TWO" );
}

void DefaultDebugPage3( )
{
	SetFont( LARGEFONT1 );
	gprintf( 0,0,L"DEBUG PAGE THREE" );
}

void DefaultDebugPage4( )
{
	SetFont( LARGEFONT1 );
	gprintf( 0,0,L"DEBUG PAGE FOUR" );
}


UINT32 SexScreenInit(void)
{
	return( TRUE );
}

#define SMILY_DELAY						100
#define SMILY_END_DELAY				1000

UINT32 SexScreenHandle(void)
{
	static UINT8					ubCurrentScreen = 0;
	VOBJECT_DESC					VObjectDesc;
	static UINT32					guiSMILY;
	static INT8						bCurFrame = 0;
	static UINT32					uiTimeOfLastUpdate = 0, uiTime;
	ETRLEObject						*pTrav;
	HVOBJECT							hVObject;
	INT16									sX, sY;

	// OK, Clear screen and show smily face....
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, 640,	480, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	if ( ubCurrentScreen == 0 )
	{
		// Load face....
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\luckysmile.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiSMILY ) )
			AssertMsg(0, "Missing INTERFACE\\luckysmile.sti" );

		// Init screen
		bCurFrame = 0;

		ubCurrentScreen = 1;

		uiTimeOfLastUpdate = GetJA2Clock( );

		return( SEX_SCREEN );

	}

	// Update frame
	uiTime = GetJA2Clock( );

	// if we are animation smile...
	if ( ubCurrentScreen == 1 )
	{
		PlayJA2StreamingSampleFromFile( "Sounds\\Sex.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN, NULL );
		if ( ( uiTime - uiTimeOfLastUpdate ) > SMILY_DELAY )
		{
			uiTimeOfLastUpdate = uiTime;

			bCurFrame++;

			if ( bCurFrame == 32 )
			{
				// Start end delay
				ubCurrentScreen = 2;
			}
		}
	}

	if ( ubCurrentScreen == 2 )
	{
		if ( ( uiTime - uiTimeOfLastUpdate ) > SMILY_END_DELAY )
		{
			uiTimeOfLastUpdate = uiTime;

			ubCurrentScreen = 0;

			// Remove video object...
			DeleteVideoObjectFromIndex( guiSMILY );

			FadeInGameScreen( );

			// Advance time...
			// Chris.... do this based on stats?
			WarpGameTime( ( ( 5 + Random( 20 ) ) * NUM_SEC_IN_MIN ), WARPTIME_NO_PROCESSING_OF_EVENTS );

			return( GAME_SCREEN );
		}
	}

	// Calculate smily face positions...
	GetVideoObject( &hVObject, guiSMILY );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );

	sX = (INT16)(( 640 - pTrav->usWidth ) / 2);
	sY = (INT16)(( 480 - pTrav->usHeight ) / 2);

	if( bCurFrame < 24 )
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, guiSMILY, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	else
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, guiSMILY, (INT8)(bCurFrame%8 ), sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	InvalidateRegion( sX, sY, (INT16)( sX + pTrav->usWidth ), (INT16)( sY + pTrav->usHeight ) );

	return( SEX_SCREEN );
}

UINT32 SexScreenShutdown(void)
{
	return( TRUE );
}



UINT32 DemoExitScreenInit(void)
{
	return( TRUE );
}


void DoneFadeOutForDemoExitScreen( void )
{
	gfProgramIsRunning = FALSE;
}

extern INT8 gbFadeSpeed;

#ifdef GERMAN
void DisplayTopwareGermanyAddress()
{
	VOBJECT_DESC		vo_desc;
	UINT32					uiTempID;
	UINT8 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	SGPRect ClipRect;

	//bring up the Topware address screen
	vo_desc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	sprintf( vo_desc.ImageFile, "German\\topware_germany.sti" );
	if( !AddVideoObject( &vo_desc, &uiTempID ) )
	{
		AssertMsg( 0, "Failed to load German\\topware_germany.sti" );
		return;
	}

	//Shade out a background piece to emphasize the German address.
	ClipRect.iLeft = iScreenWidthOffset + 208;
	ClipRect.iRight = iScreenWidthOffset + 431;
	ClipRect.iTop = iScreenHeightOffset + 390;
	ClipRect.iBottom = iScreenHeightOffset + 475;
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	Blt16BPPBufferShadowRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
	UnLockVideoSurface( FRAME_BUFFER );

	//Draw the anti-aliased address now.
	BltVideoObjectFromIndex( FRAME_BUFFER, uiTempID, 0, iScreenWidthOffset + 218, iScreenHeightOffset + 400, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObjectFromIndex( FRAME_BUFFER, uiTempID, 0, iScreenWidthOffset + 218, iScreenHeightOffset + 400, VO_BLT_SRCTRANSPARENCY, NULL );
	InvalidateRegion( iScreenWidthOffset + 208, iScreenHeightOffset + 390, iScreenWidthOffset + 431, iScreenHeightOffset + 475 );
	DeleteVideoObjectFromIndex( uiTempID );
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();
}
#endif

UINT32 DemoExitScreenHandle(void)
{
	gfProgramIsRunning = FALSE;
	return( DEMO_EXIT_SCREEN );
}


UINT32 DemoExitScreenShutdown(void)
{
	return( TRUE );
}

#ifndef JA2EDITOR

UINT32 LoadSaveScreenInit()
{
	return TRUE;
}

UINT32 LoadSaveScreenHandle()
{
	return TRUE;
}

UINT32 LoadSaveScreenShutdown()
{
	return TRUE;
}

UINT32 EditScreenInit()
{
	return TRUE;
}

UINT32 EditScreenHandle()
{
	return TRUE;
}

UINT32 EditScreenShutdown()
{
	return TRUE;
}


#endif
