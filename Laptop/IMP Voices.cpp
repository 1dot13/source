#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "CharProfile.h"
	#include "IMP Voices.h"
	#include "IMP MainPage.h"
	#include "IMP HomePage.h"
	#include "IMPVideoObjects.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "input.h"
	#include "Isometric Utils.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "Sound Control.h"
	#include "IMP Text System.h"
	#include "text.h"
#endif

//current and last pages

// WANNE NEW: Set iLastVoice dynamically, skip used voices
INT32 iCurrentVoices = 0;

INT32 iVoiceId = 0;

//INT32 iVoiceId = 0;


UINT32 uiVocVoiceSound = 0;
// buttons needed for the IMP Voices screen
INT32 giIMPVoicesButton[ 3 ];
INT32 giIMPVoicesButtonImage[ 3 ];


// hacks to be removeed later
BOOLEAN fVoiceAVisited = FALSE, fVoiceBVisited = FALSE, fVoiceCVisited = FALSE;


// redraw protrait screen
BOOLEAN fReDrawVoicesScreenFlag = FALSE;

// the portrait region, for player to click on and re-hear voice
MOUSE_REGION gVoicePortraitRegion; 

// function definitions
void IncrementVoice( void );
void DecrementVoice( void );
void CreateIMPVoicesButtons( void );
void DestroyIMPVoicesButtons( void );
void CreateIMPVoiceMouseRegions( void );
void DestroyIMPVoiceMouseRegions( void );
void RenderVoiceIndex( void );
BOOLEAN CameBackToVoicePageButNotFinished();

// callbacks
void BtnIMPVoicesNextCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPVoicesPreviousCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPVoicesDoneCallback(GUI_BUTTON *btn,INT32 reason);
void IMPPortraitRegionButtonCallback(MOUSE_REGION * pRegion, INT32 iReason );

void EnterIMPVoices( void )
{
  fVoiceAVisited = FALSE;
	fVoiceBVisited = FALSE;
	fVoiceCVisited = FALSE;

	// Set the initialize voice
	iVoiceId = -1;
	IncrementVoice();

	// create buttons
	CreateIMPVoicesButtons( );
	 
	// create mouse regions
	CreateIMPVoiceMouseRegions( );

	// render background
	RenderIMPVoices( );

	// play voice once
	uiVocVoiceSound = PlayVoice( );

	return;
}


void RenderIMPVoices( void )
{
  

  // render background
	RenderProfileBackGround( );

	// the Voices frame
	RenderPortraitFrame( 191, 167 );
  
	// the sillouette
	RenderLargeSilhouette( 200, 176 );

	// indent for the text
	RenderAttrib1IndentFrame( 128, 65); 

	// render voice index value
	RenderVoiceIndex( );

	// text
	PrintImpText( );

	return;
}


void ExitIMPVoices( void )
{
	// destroy buttons for IMP Voices page
  DestroyIMPVoicesButtons( ); 

  // destroy mouse regions for this screen
	DestroyIMPVoiceMouseRegions( );


	return;
}

void HandleIMPVoices( void )
{

	// do we need to re write screen
	if ( fReDrawVoicesScreenFlag == TRUE ) 
	{
    RenderIMPVoices( );
		
		// reset redraw flag
		fReDrawVoicesScreenFlag = FALSE;
	}
	return;
}

BOOLEAN IsIMPSlotFree(INT32 iIMPId)
{
	if (wcscmp(gMercProfiles[iIMPId].zName, L"") == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// WANNE NEW
void IncrementVoice( void )
{
	INT32 iIMPIndex = -1;
	INT32 iOffset;
	INT32 i;

	iVoiceId++;

	if (iVoiceId > 2)
		iVoiceId = 0;

	if (fCharacterIsMale == TRUE)
		iOffset = 0;
	else
		iOffset = 3;

	// Just to be save (so we use no endless loop)
	for (i = 0; i < 20; i++)
	{
		if (IsIMPSlotFree(PLAYER_GENERATED_CHARACTER_ID + iOffset + iVoiceId) == TRUE)
		{
			// This is the free imp index
			iIMPIndex = PLAYER_GENERATED_CHARACTER_ID + iOffset + iVoiceId;
			break;
		}

		iVoiceId++;

		// Upper range -> Reset to 0
		if (iVoiceId > 2)
			iVoiceId = 0;
	}

	// Set the voice id of the free slot
	iCurrentVoices = iIMPIndex - iOffset - PLAYER_GENERATED_CHARACTER_ID;
	iVoiceId = iCurrentVoices;

  return; 
}


void DecrementVoice( void )
{
  	INT32 iIMPIndex = -1;
	INT32 iOffset;
	INT32 i;

	iVoiceId--;

	if (iVoiceId < 0)
		iVoiceId = 2;

	if (fCharacterIsMale == TRUE)
		iOffset = 0;
	else
		iOffset = 3;

	// Just to be save (so we use no endless loop)
	for (i = 0; i < 20; i++)
	{
		if (IsIMPSlotFree(PLAYER_GENERATED_CHARACTER_ID + iOffset + iVoiceId) == TRUE)
		{
			// This is the free imp index
			iIMPIndex = PLAYER_GENERATED_CHARACTER_ID + iOffset + iVoiceId;
			break;
		}

		iVoiceId--;

		// Upper range -> Reset to 0
		if (iVoiceId < 0)
			iVoiceId = 2;
	}

	// Set the voice id of the free slot
	iCurrentVoices = iIMPIndex - iOffset - PLAYER_GENERATED_CHARACTER_ID;
	iVoiceId = iCurrentVoices;

  return; 
}


void CreateIMPVoicesButtons( void )
{
  // will create buttons need for the IMP Voices screen

	// next button
  giIMPVoicesButtonImage[0]=  LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,1,-1,3,-1 );
	/*giIMPVoicesButton[0] = QuickCreateButton( giIMPVoicesButtonImage[0], LAPTOP_SCREEN_UL_X +  ( 18 ), LAPTOP_SCREEN_WEB_UL_Y + ( 184 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesNextCallback );
  */
	giIMPVoicesButton[0] = CreateIconAndTextButton( giIMPVoicesButtonImage[0], pImpButtonText[ 13 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														  LAPTOP_SCREEN_UL_X +  ( 343 ), LAPTOP_SCREEN_WEB_UL_Y + ( 205 ),BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 	BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesNextCallback);


	// previous button
	giIMPVoicesButtonImage[ 1 ]=  LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,0,-1,2,-1 );
/*	giIMPVoicesButton[ 1 ] = QuickCreateButton( giIMPVoicesButtonImage[ 1 ], LAPTOP_SCREEN_UL_X +  ( 18 ), LAPTOP_SCREEN_WEB_UL_Y + ( 254 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesPreviousCallback );
  */
  giIMPVoicesButton[ 1 ] = CreateIconAndTextButton( giIMPVoicesButtonImage[ 1 ], pImpButtonText[ 12 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 93), LAPTOP_SCREEN_WEB_UL_Y + ( 205 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesPreviousCallback);


	// done button
  giIMPVoicesButtonImage[ 2 ]=  LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	/* giIMPVoicesButton[ 2 ] = QuickCreateButton( giIMPVoicesButtonImage[ 1 ], LAPTOP_SCREEN_UL_X +  ( 349 ), LAPTOP_SCREEN_WEB_UL_Y + ( 220 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesDoneCallback );
  */
  giIMPVoicesButton[ 2 ] = CreateIconAndTextButton( giIMPVoicesButtonImage[ 2 ], pImpButtonText[ 11 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 187 ), LAPTOP_SCREEN_WEB_UL_Y + ( 330 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 	BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPVoicesDoneCallback);



   SetButtonCursor(giIMPVoicesButton[0], CURSOR_WWW);
	 SetButtonCursor(giIMPVoicesButton[1], CURSOR_WWW);
	 SetButtonCursor(giIMPVoicesButton[2], CURSOR_WWW);
}




void DestroyIMPVoicesButtons( void )
{

	// will destroy buttons created for IMP Voices screen
  
	// the next button
  RemoveButton(giIMPVoicesButton[ 0 ] );
  UnloadButtonImage(giIMPVoicesButtonImage[ 0 ] );
	
	// the previous button
  RemoveButton(giIMPVoicesButton[ 1 ] );
  UnloadButtonImage(giIMPVoicesButtonImage[ 1 ] );
	
	// the done button
  RemoveButton(giIMPVoicesButton[ 2 ] );
  UnloadButtonImage(giIMPVoicesButtonImage[ 2 ] );

	return;
}


void BtnIMPVoicesNextCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// next voice!!
			IncrementVoice( );
			// play voice
			if( ! SoundIsPlaying( uiVocVoiceSound ) )
			{
				 uiVocVoiceSound = PlayVoice( );
			}	
			else
			{
				SoundStop( uiVocVoiceSound );
				uiVocVoiceSound = PlayVoice( );
			}

      
			fReDrawVoicesScreenFlag = TRUE;
		}
	}	
} 

void BtnIMPVoicesPreviousCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// previous voice, please!!!
			DecrementVoice( );
			// play voice
			if( ! SoundIsPlaying( uiVocVoiceSound ) )
			{
        uiVocVoiceSound = PlayVoice( );
			}	
			else
			{
				SoundStop( uiVocVoiceSound );
				uiVocVoiceSound = PlayVoice( );
			}

			fReDrawVoicesScreenFlag = TRUE;
		}
	}	
} 

void BtnIMPVoicesDoneCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// go to main page
			iCurrentImpPage = IMP_MAIN_PAGE;

			// if we are already done, leave
	    if( iCurrentProfileMode == IMP__FINISH )
			{
	      iCurrentImpPage = IMP_FINISH;
			} 
			else
			{
				if( CameBackToVoicePageButNotFinished() )
				{
					iCurrentImpPage = IMP_MAIN_PAGE;
				}
				else
				{
					iCurrentProfileMode = IMP__PERSONALITY;
				}
			}
/*
			// current mode now is voice
		  else if( iCurrentProfileMode < IMP__PORTRAIT )
			{
				iCurrentProfileMode = IMP__PORTRAIT;
			}

			else if( iCurrentProfileMode == IMP__PORTRAIT )
			{
				// all done profiling
				iCurrentProfileMode = IMP__VOICE;
				iCurrentImpPage = IMP_FINISH;
			}
*/
			// set voice id, to grab character slot
      
			// WANNE 10:
			if( fCharacterIsMale == TRUE )
			{
				LaptopSaveInfo.iVoiceId = iCurrentVoices;
			}
			else
			{
				LaptopSaveInfo.iVoiceId = iCurrentVoices + 3;
			}

			// set button up image  pending
			fButtonPendingFlag = TRUE;
		}
	}	
} 

BOOLEAN CameBackToVoicePageButNotFinished()
{
	//if we are in a page that comes after this one
	if( iCurrentProfileMode == IMP__PERSONALITY ||
			iCurrentProfileMode == IMP__ATTRIBUTES )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


UINT32 PlayVoice( void )
{
//	CHAR16 sString[ 64 ];

	// gender?

	if( fCharacterIsMale == TRUE )
	{
	  switch( iCurrentVoices )
		{
		  case( 0 ):
			  return( PlayJA2SampleFromFile( "Speech\\051_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
			case( 1 ):
		    return( PlayJA2SampleFromFile( "Speech\\052_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
			case( 2 ):
				return( PlayJA2SampleFromFile( "Speech\\053_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
			case( 3 ):
				return( PlayJA2SampleFromFile( "Speech\\057_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
		}
	}
	else
	{
    switch( iCurrentVoices )
		{
		  case( 0 ):
				return( PlayJA2SampleFromFile( "Speech\\054_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
		  case( 1 ):
				return( PlayJA2SampleFromFile( "Speech\\055_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
			case( 2 ):
	    	return( PlayJA2SampleFromFile( "Speech\\056_001.wav", RATE_11025, MIDVOLUME, 1 , MIDDLEPAN ) );	 
			  break;
		}

	}
	return 0;
}


void CreateIMPVoiceMouseRegions( void )
{
  // will create mouse regions needed for the IMP voices page
	MSYS_DefineRegion( &gVoicePortraitRegion, LAPTOP_SCREEN_UL_X + 200, LAPTOP_SCREEN_WEB_UL_Y + 176 ,LAPTOP_SCREEN_UL_X + 200 + 100, LAPTOP_SCREEN_WEB_UL_Y + 176 + 100,MSYS_PRIORITY_HIGH,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, IMPPortraitRegionButtonCallback ); 
 
  
	MSYS_AddRegion( &gVoicePortraitRegion );

	return;
}

void DestroyIMPVoiceMouseRegions( void )
{
  // will destroy already created mouse reiogns for IMP voices page
  MSYS_RemoveRegion( &gVoicePortraitRegion );

	return;
}


void IMPPortraitRegionButtonCallback(MOUSE_REGION * pRegion, INT32 iReason )
{ 	
	// callback handler for imp portrait region button events

  if (iReason & MSYS_CALLBACK_REASON_INIT)
  {
 	  return;
  }
  if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
  { 
    if( ! SoundIsPlaying( uiVocVoiceSound ) )
		{
       uiVocVoiceSound = PlayVoice( );
		}
		
  }

	return;
}


void RenderVoiceIndex( void )
{

	CHAR16 sString[ 32 ];
	INT16 sX, sY;

	// render the voice index value on the the blank portrait
	swprintf( sString, L"%s %d", pIMPVoicesStrings[ 0 ], iCurrentVoices + 1 );

	FindFontCenterCoordinates( 290 + LAPTOP_UL_X, 0, 100, 0, sString, FONT12ARIAL, &sX, &sY );

	SetFont( FONT12ARIAL );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	mprintf( sX, iScreenHeightOffset + 320, sString );

	return;
}