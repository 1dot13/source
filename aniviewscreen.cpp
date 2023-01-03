#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <time.h>
	#include "sgp.h"
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "vobject_blitters.h"
	#include "worlddef.h"
	#include "renderworld.h"
	#include "input.h"
	#include "font.h"
	#include "screenids.h"
	#include "container.h"
	#include "overhead.h"
	#include "sysutil.h"
	#include "Font Control.h"
	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "Event Pump.h"
	#include "Render Dirty.h"
	#include "Sys Globals.h"
	#include "Interface.h"
	#include <wchar.h>
	#include <tchar.h>
	#include "english.h"
	#include "Fileman.h"
	#include "messageboxscreen.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


void BuildListFile( );


BOOLEAN	gfAniEditMode = FALSE;
static UINT16		usStartAnim = 0;
static UINT8		ubStartHeight = 0;
static SOLDIERTYPE *pSoldier;

static BOOLEAN fOKFiles = FALSE;
static UINT8	 ubNumStates = 0;
static UINT16   *pusStates = NULL;
static INT8   ubCurLoadedState = 0;

void CycleAnimations( )
{
	INT32 cnt;

	// FInd the next animation with start height the same...
	for ( cnt = usStartAnim + 1; cnt < NUMANIMATIONSTATES; cnt++ )
	{
		if ( gAnimControl[ cnt ].ubHeight == ubStartHeight )
		{
			usStartAnim = ( UINT8) cnt;
			pSoldier->EVENT_InitNewSoldierAnim( usStartAnim, 0 , TRUE );
			return;
		}
	}

	usStartAnim = 0;
	pSoldier->EVENT_InitNewSoldierAnim( usStartAnim, 0 , TRUE );
}


UINT32 AniEditScreenInit(void)
{

  return TRUE;
}

// The ShutdownGame function will free up/undo all things that were started in InitializeGame()
// It will also be responsible to making sure that all Gaming Engine tasks exit properly

UINT32 AniEditScreenShutdown(void)
{

	return TRUE;
}



UINT32  AniEditScreenHandle(void)
{
  InputAtom					InputEvent;
	static BOOLEAN		fFirstTime = TRUE;
	static UINT16			usOldState;
	static BOOLEAN		fToggle = FALSE;
	static BOOLEAN		fToggle2 = FALSE;

//	EV_S_SETPOSITION SSetPosition;

	// Make backups
	if ( fFirstTime )
	{
		gfAniEditMode = TRUE;

		usStartAnim   = 0;
		ubStartHeight = ANIM_STAND;

		fFirstTime = FALSE;
		fToggle		 = FALSE;
		fToggle2   = FALSE;
		ubCurLoadedState = 0;

		pSoldier = MercPtrs[ gusSelectedSoldier ];

		gTacticalStatus.uiFlags |= LOADING_SAVED_GAME;

		pSoldier->EVENT_InitNewSoldierAnim( usStartAnim, 0 , TRUE );

		BuildListFile( );

	}



	/////////////////////////////////////////////////////
	StartFrameBufferRender( );

	RenderWorld( );

	ExecuteBaseDirtyRectQueue( );


	/////////////////////////////////////////////////////
	EndFrameBufferRender( );


	SetFont( LARGEFONT1 );
	mprintf( 0,0,L"SOLDIER ANIMATION VIEWER" );
	gprintfdirty( (INT16)0,(INT16)0,L"SOLDIER ANIMATION VIEWER" );


	mprintf( 0,20,L"Current Animation: %S %S", gAnimControl[ usStartAnim ].zAnimStr, gAnimSurfaceDatabase[ pSoldier->usAnimSurface ].Filename );
	gprintfdirty( (INT16)0,(INT16)20,L"Current Animation: %S %S", gAnimControl[ usStartAnim ].zAnimStr, gAnimSurfaceDatabase[ pSoldier->usAnimSurface ].Filename );


	switch( ubStartHeight )
	{
		case ANIM_STAND:

			mprintf( 0,40,L"Current Stance: STAND" );
			break;

		case ANIM_CROUCH:

			mprintf( 0,40,L"Current Stance: CROUCH" );
			break;

		case ANIM_PRONE:

			mprintf( 0,40,L"Current Stance: PRONE" );
			break;
	}
	gprintfdirty( (INT16)0,(INT16)40,L"Current Animation: %S", gAnimControl[ usStartAnim ].zAnimStr );


	if ( fToggle )
	{
		mprintf( 0,60,L"FORCE ON" );
		gprintfdirty( (INT16)0,(INT16)60,L"FORCE OFF" );
	}

	if ( fToggle2 )
	{
		mprintf( 0,70,L"LOADED ORDER ON" );
		gprintfdirty( (INT16)0,(INT16)70,L"LOADED ORDER ON" );

		mprintf( 0,90,L"LOADED ORDER : %S", gAnimControl[ pusStates[ ubCurLoadedState ] ].zAnimStr );
		gprintfdirty( (INT16)0,(INT16)90,L"LOADED ORDER : %S", gAnimControl[ pusStates[ ubCurLoadedState ] ].zAnimStr );

	}

  if (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
  {
    if ((InputEvent.usEvent == KEY_DOWN)&&(InputEvent.usParam == ESC))
    {
			 fFirstTime = TRUE;

			 gfAniEditMode = FALSE;

	  	 fFirstTimeInGameScreen = TRUE;

			 gTacticalStatus.uiFlags &= (~LOADING_SAVED_GAME);

			 if ( fOKFiles )
			 {
					 MemFree( pusStates );
			 }

			 fOKFiles = FALSE;

			 return( GAME_SCREEN );
    }

		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == SPACE ))
		{
			if ( !fToggle && !fToggle2 )
			{
				CycleAnimations( );
			}
		}

		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == 's' ))
		{
			if ( !fToggle )
			{
				UINT16 usAnim=0;
				usOldState = usStartAnim;

				switch( ubStartHeight )
				{
					case ANIM_STAND:

						usAnim = STANDING;
						break;

					case ANIM_CROUCH:

						usAnim = CROUCHING;
						break;

					case ANIM_PRONE:

						usAnim = PRONE;
						break;
				}

				pSoldier->EVENT_InitNewSoldierAnim( usAnim, 0 , TRUE );
			}
			else
			{
				pSoldier->EVENT_InitNewSoldierAnim( usOldState, 0 , TRUE );
			}

			fToggle = !fToggle;
		}

		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == 'l' ))
		{
			if ( !fToggle2 )
			{
				usOldState = usStartAnim;

				pSoldier->EVENT_InitNewSoldierAnim( pusStates[ ubCurLoadedState ], 0 , TRUE );
			}
			else
			{
				pSoldier->EVENT_InitNewSoldierAnim( usOldState, 0 , TRUE );
			}

			fToggle2 = !fToggle2;
		}


		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == PGUP ))
		{
			 if ( fOKFiles && fToggle2 )
			 {
					ubCurLoadedState++;

					if ( ubCurLoadedState == ubNumStates )
					{
						ubCurLoadedState = 0;
					}

					pSoldier->EVENT_InitNewSoldierAnim( pusStates[ ubCurLoadedState ], 0 , TRUE );

			 }
		}


		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == PGDN ))
		{
			 if ( fOKFiles && fToggle2 )
			 {
					ubCurLoadedState--;

					if ( ubCurLoadedState == 0 )
					{
						ubCurLoadedState = ubNumStates;
					}

					pSoldier->EVENT_InitNewSoldierAnim( pusStates[ ubCurLoadedState ], 0 , TRUE );
			 }
		}

		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == 'c' ))
		{
			// CLEAR!
			usStartAnim = 0;
			pSoldier->EVENT_InitNewSoldierAnim( usStartAnim, 0 , TRUE );
		}

		if ((InputEvent.usEvent == KEY_UP) && (InputEvent.usParam == ENTER ))
		{
			if ( ubStartHeight == ANIM_STAND )
			{
				ubStartHeight = ANIM_CROUCH;
			}
			else if ( ubStartHeight == ANIM_CROUCH )
			{
				ubStartHeight = ANIM_PRONE;
			}
			else
			{
				ubStartHeight = ANIM_STAND;
			}
		}

  }


  return( ANIEDIT_SCREEN );

}


UINT16 GetAnimStateFromName( STR8 zName )
{
	INT32 cnt;

	// FInd the next animation with start height the same...
	for ( cnt = 0; cnt < NUMANIMATIONSTATES; cnt++ )
	{
		if ( _stricmp( gAnimControl[ cnt ].zAnimStr, zName ) == 0 )
		{
			return( (UINT16) cnt );
		}
	}

	return( 5555 );
}


void BuildListFile( )
{
	FILE *infoFile;
	CHAR8 currFilename[128];
	int numEntries = 0;
	int	cnt;
	UINT16 usState;
	CHAR16 zError[128];


	//Verify the existance of the header text file.
	infoFile = fopen( "ANITEST.DAT", "rb");
	if(!infoFile)
	{
		return;
	}
	//count STIs inside header and verify each one's existance.
	while( !feof( infoFile ) )
	{
		fgets( currFilename, 128, infoFile );
		//valid entry in header, continue on...

		numEntries++;
	}
	fseek( infoFile, 0, SEEK_SET ); //reset header file

	// Allocate array
	pusStates = (UINT16 *) MemAlloc( sizeof( UINT16 ) * numEntries );

	fOKFiles = TRUE;

	cnt = 0;
	while( !feof( infoFile ) && cnt < numEntries )
	{
		fgets( currFilename, 128, infoFile );

		// Remove newline
		currFilename[ strlen( currFilename ) -1 ] = '\0';
		currFilename[ strlen( currFilename ) -1 ] = '\0';

		usState = GetAnimStateFromName( currFilename );

		if ( usState != 5555 )
		{
			// Bob: swapped places, there's numEntries things to be put in pusStates, I'm guessing supposed to start at 0
			pusStates[cnt] = usState;
			cnt++;
			ubNumStates	= (UINT8)cnt;			
			// pusStates[ cnt ] = usState;
		}
		else
		{
			swprintf( zError, L"Animation str %S is not known: ", currFilename );
			DoMessageBox( MSG_BOX_BASIC_STYLE, zError, ANIEDIT_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, NULL, NULL );
			fclose( infoFile );
			return;
		}
	}

}
