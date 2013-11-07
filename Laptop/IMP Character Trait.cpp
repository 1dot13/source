#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Character Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Character Trait.h"
	#include "Button System.h"
	#include "utilities.h"
	#include "Debug.h"
	#include "Text.h"
	#include "Font Control.h"
	#include "font.h"
	#include "laptop.h"
	#include "cursors.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "_Ja25EnglishText.h"
	#include "wordwrap.h"
	#include "CharProfile.h"
	#include "GameSettings.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_CHARACTER_TRAIT__TITLE_FONT				FONT14ARIAL

#define			IMP_CHARACTER_TRAIT__FONT							FONT12ARIAL
#define			IMP_CHARACTER_TRAIT__COLOR						FONT_MCOLOR_WHITE

#define	IMP_CHARACTER_1ST_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 15 )
#define	IMP_CHARACTER_1ST_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_CHARACTER_2ND_COLUMN_START_X								( IMP_CHARACTER_1ST_COLUMN_START_X + 241 )
#define	IMP_CHARACTER_2ND_COLUMN_START_Y								IMP_CHARACTER_1ST_COLUMN_START_Y

#define	IMP_CHARACTER_TRAIT__SPACE_BTN_BUTTONS									38

//#define	IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN			5 

#define	IMP_CHARACTER_TRAIT__TEXT_OFFSET_X											65
#define	IMP_CHARACTER_TRAIT__TEXT_OFFSET_Y											12

#define	IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_X									5
#define	IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_Y									7


//*******************************************************************
//
// Global Variables
//
//*******************************************************************

BOOLEAN gfIST_Redraw2=FALSE;

BOOLEAN	gfCharacterTraitQuestions[ 15 ];

// these are the buttons for the questions
INT32 giIMPCharacterTraitAnswerButton[ 15 ];
INT32 giIMPCharacterTraitAnswerButtonImage[ 15 ];

// this is the Done	buttons
INT32 giIMPCharacterTraitFinsihButton;
INT32 giIMPCharacterTraitFinsihButtonImage;

//image handle
UINT32	guiIST_GreyGoldBox2;

MOUSE_REGION	gMR_CharacterTraitHelpTextRegions[IMP_NUMBER_CHARACTER_TRAITS];

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPCharacterTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPCharacterTraitFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddImpCharacterTraitButtons();
void		HandleCharacterTraitButtonStates( );
void		HandleIMPCharacterTraitAnswers( UINT32 uiSkillPressed );
void		IMPCharacterTraitDisplayCharacterTraits();
//INT8		CountNumCharacterTraitsSelected( BOOLEAN fIncludeNoneSkill );
//BOOLEAN ShouldTraitBeSkipped( UINT32 uiTrait );
//void		SaveSelectedPersonality();
//void		HandleLastSelectedCharacterTraits( INT8 bNewTrait );
BOOLEAN CameBackToCharacterTraitPageButNotFinished();
//ppp
void AssignCharacterTraitHelpText( UINT8 ubNumber );

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPCharacterTrait( void )
{
//	UINT32 ubCnt;
	VOBJECT_DESC	VObjectDesc;

	//add the skill trait buttons
	AddImpCharacterTraitButtons();

	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIST_GreyGoldBox2 ) )
	{
		Assert( 0 );
		return;
	}



	giIMPCharacterTraitFinsihButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPCharacterTraitFinsihButton = CreateIconAndTextButton( giIMPCharacterTraitFinsihButtonImage, pImpButtonText[ 13 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPCharacterTraitFinishCallback );



	SetButtonCursor( giIMPCharacterTraitFinsihButton, CURSOR_WWW);

	//if we are not DONE and are just reviewing
	if( iCurrentProfileMode != IMP__FINISH )
	{
		//Have the NONE trait initially selected
		gfCharacterTraitQuestions[ 0 ] = TRUE;
	}

	HandleCharacterTraitButtonStates( );

	if ( gGameOptions.fNewTraitSystem )
	{
		// add regions for help texts
		UINT16 usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + 62;
		UINT16 usPosY = IMP_CHARACTER_1ST_COLUMN_START_Y + 8;
		for( UINT8 ubCnt=0; ubCnt<(gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); ubCnt++ )
		{
			MSYS_DefineRegion( &gMR_CharacterTraitHelpTextRegions[ubCnt], ( usPosX ), ( usPosY ),
							(usPosX + 156), ( usPosY + 17), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gMR_CharacterTraitHelpTextRegions[ubCnt] );
			
			//Determine the next x location
			if( ubCnt < (gGameOptions.fNewTraitSystem ? 6 : 5) )
				usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + 62;
			else
				usPosX = IMP_CHARACTER_2ND_COLUMN_START_X + 62;

			//Determine the next Y location
			if( ubCnt == (gGameOptions.fNewTraitSystem ? 6 : 5) )
				usPosY = IMP_CHARACTER_2ND_COLUMN_START_Y + 8;
			else
				usPosY += IMP_CHARACTER_TRAIT__SPACE_BTN_BUTTONS;
		}
	}
}


void RenderIMPCharacterTrait( void )
{	
	//render the metal background graphic
	RenderProfileBackGround();

	//Display the title
	if ( gGameOptions.fNewTraitSystem )
		DrawTextToScreen( gzIMPCharacterTraitText[IMP_NUMBER_CHARACTER_TRAITS], LAPTOP_SCREEN_UL_X - 111, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	else
		DrawTextToScreen( gzIMPAttitudesText[IMP_NUMBER_ATTITUDES], LAPTOP_SCREEN_UL_X - 111, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	IMPCharacterTraitDisplayCharacterTraits();
}


void ExitIMPCharacterTrait( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIST_GreyGoldBox2 );

	//remove the skill buttons
	for(iCnt = 0; iCnt < (gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); iCnt++)
	{
		//if there is a button allocated
		if( giIMPCharacterTraitAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPCharacterTraitAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPCharacterTraitAnswerButtonImage[ iCnt ] );
		}

		if ( gGameOptions.fNewTraitSystem )
			MSYS_RemoveRegion( &gMR_CharacterTraitHelpTextRegions[iCnt] );
	}

	RemoveButton( giIMPCharacterTraitFinsihButton );
	UnloadButtonImage( giIMPCharacterTraitFinsihButtonImage );
}


void HandleIMPCharacterTrait( void )
{
	if( gfIST_Redraw2 )
	{
		RenderIMPCharacterTrait( );
		gfIST_Redraw2 = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddImpCharacterTraitButtons()
{
	INT32 iCnt = 0;
	UINT16 usPosX, usPosY;

	usPosX = IMP_CHARACTER_1ST_COLUMN_START_X;
	usPosY = IMP_CHARACTER_1ST_COLUMN_START_Y;

	for(iCnt = 0; iCnt < (gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES) ; iCnt++)
	{
		//reset
		giIMPCharacterTraitAnswerButton[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfCharacterTraitQuestions[ iCnt ] = FALSE;
		}

		if( iCnt == 0 )
			giIMPCharacterTraitAnswerButtonImage[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );
		else
			giIMPCharacterTraitAnswerButtonImage[ iCnt ] =	UseLoadedButtonImage( giIMPCharacterTraitAnswerButtonImage[ 0 ], -1,0,-1,1,-1 );

		giIMPCharacterTraitAnswerButton[iCnt] = QuickCreateButton( giIMPCharacterTraitAnswerButtonImage[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPCharacterTraitAnswerCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPCharacterTraitAnswerButton[iCnt], 0, iCnt );
		SetButtonCursor( giIMPCharacterTraitAnswerButton[iCnt], CURSOR_WWW);

		//Get rid of playing the button sound, it will be handled here
		//ButtonList[ giIMPCharacterTraitAnswerButton[ iCnt ] ]->ubSoundSchemeID = 0;

		//Determine the next x location
		if( iCnt < (gGameOptions.fNewTraitSystem ? 6 : 5) )
			usPosX = IMP_CHARACTER_1ST_COLUMN_START_X;
		else
			usPosX = IMP_CHARACTER_2ND_COLUMN_START_X;

		//Determine the next Y location
		if( iCnt == (gGameOptions.fNewTraitSystem ? 6 : 5) )
			usPosY = IMP_CHARACTER_2ND_COLUMN_START_Y;
		else
			usPosY += IMP_CHARACTER_TRAIT__SPACE_BTN_BUTTONS;
	}
}


void BtnIMPCharacterTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags|=(BUTTON_CLICKED_ON);

		INT32 iCharacterTrait =	MSYS_GetBtnUserData( btn, 0 );

		HandleIMPCharacterTraitAnswers( iCharacterTrait );
	}
}

void HandleIMPCharacterTraitAnswers( UINT32 uiSkillPressed )
{
	INT8 bCnt;

	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//reset all other buttons
	for( bCnt=0; bCnt < (gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); bCnt++ )
	{
		gfCharacterTraitQuestions[ bCnt ] = FALSE;
	}

	//make sure its a valid character trait
	if( uiSkillPressed > (UINT32)(gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES) )
	{
		Assert( 0 );
		return;
	}

	//if its allready set
	if( gfCharacterTraitQuestions[ uiSkillPressed ] )
	{
		//dont need to do anything
		return;
	}

	//Set the skill
	gfCharacterTraitQuestions[ uiSkillPressed ] = TRUE;

	//Play the button sound
	if( gfCharacterTraitQuestions[ uiSkillPressed ] )
	{
		PlayButtonSound( giIMPCharacterTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
	}
	else
	{
		PlayButtonSound( giIMPCharacterTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
	}

	//update buttons
	HandleCharacterTraitButtonStates( );

	//redraw the screen
	gfIST_Redraw2 = TRUE;
}

void HandleCharacterTraitButtonStates( )
{
	INT8 bCnt;

	for( bCnt=0; bCnt < (gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); bCnt++ )
	{

		//if the skill is selected ( ie depressed )
		if( gfCharacterTraitQuestions[ bCnt ] )
		{
			ButtonList[ giIMPCharacterTraitAnswerButton[ bCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPCharacterTraitAnswerButton[ bCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPCharacterTraitDisplayCharacterTraits()
{
	INT8 bCnt;
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	HVOBJECT	hImageHandle;

	// Stats
	GetVideoObject(&hImageHandle, guiIST_GreyGoldBox2 );

	usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;
	usPosY = IMP_CHARACTER_1ST_COLUMN_START_Y + IMP_CHARACTER_TRAIT__TEXT_OFFSET_Y;

	for( bCnt=0; bCnt<(gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); bCnt++ )
	{
		usBoxPosX = usPosX - IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_Y;

		//if the trait is selected
		if( gfCharacterTraitQuestions[ bCnt ] )
		{
			//Display the gold background box
			BltVideoObject(FRAME_BUFFER, hImageHandle, 1, usBoxPosX, usBoxPosY, VO_BLT_SRCTRANSPARENCY,NULL);
		}
		else
		{
			//Display the grey background box
			BltVideoObject(FRAME_BUFFER, hImageHandle, 0,usBoxPosX, usBoxPosY, VO_BLT_SRCTRANSPARENCY,NULL);
		}

		//draw the text to the screenx
		if ( gGameOptions.fNewTraitSystem )
			DrawTextToScreen( gzIMPCharacterTraitText[ bCnt ], usPosX, usPosY, 0, IMP_CHARACTER_TRAIT__FONT, IMP_CHARACTER_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		else
			DrawTextToScreen( gzIMPAttitudesText[ bCnt ], usPosX, usPosY, 0, IMP_CHARACTER_TRAIT__FONT, IMP_CHARACTER_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		if ( gGameOptions.fNewTraitSystem )
		{
			AssignCharacterTraitHelpText( bCnt );
		}

		//Determine the next x location
		if( bCnt < (gGameOptions.fNewTraitSystem ? 6 : 5) )
			usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;
		else
			usPosX = IMP_CHARACTER_2ND_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;

		//Determine the next Y location
		if( bCnt == (gGameOptions.fNewTraitSystem ? 6 : 5) )
			usPosY = IMP_CHARACTER_2ND_COLUMN_START_Y + IMP_CHARACTER_TRAIT__TEXT_OFFSET_Y;
		else
			usPosY += IMP_CHARACTER_TRAIT__SPACE_BTN_BUTTONS;
	}
}


void BtnIMPCharacterTraitFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		//if we are just reviewing the page
		//if( iCurrentProfileMode == IMP__FINISH )
		//{
			//go back tot he done screen
		iCurrentImpPage = IMP_DISABILITY_PAGE;
		fButtonPendingFlag = TRUE;
		//}
		/*else
		{
			iCurrentImpPage = IMP_DISABILITY_PAGE;

			if( CameBackToCharacterTraitPageButNotFinished() )
			{
			}
			else
			{
				//We are finished on this page
				iCurrentProfileMode = IMP__PERSONALITY;
			}
		}*/
	}
}


BOOLEAN CameBackToCharacterTraitPageButNotFinished()
{
	//if we are in a page that comes after this one
	if( iCurrentProfileMode == IMP__ATTRIBUTES || iCurrentProfileMode == IMP__PERSONALITY )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

INT8 iChosenCharacterTrait()
{
	INT8	bCnt;
	INT8 iCharacterTraitNumber = 0;

	//loop through all the buttons and reset them
	for( bCnt=0; bCnt<(gGameOptions.fNewTraitSystem ? IMP_NUMBER_CHARACTER_TRAITS : IMP_NUMBER_ATTITUDES); bCnt++ )
	{
		//if the trait is selected
		if( gfCharacterTraitQuestions[ bCnt ] )
		{
			iCharacterTraitNumber = bCnt;
		}
	}
	return( iCharacterTraitNumber );
}


void AssignCharacterTraitHelpText( UINT8 ubNumber )
{
	CHAR16	apStr[ 2000 ];

	swprintf( apStr, gzIMPNewCharacterTraitsHelpTexts[ubNumber] );

	// Set region help text
	SetRegionFastHelpText( &(gMR_CharacterTraitHelpTextRegions[ubNumber]), apStr );
	SetRegionHelpEndCallback( &gMR_CharacterTraitHelpTextRegions[ubNumber], MSYS_NO_CALLBACK );

	return;
}
