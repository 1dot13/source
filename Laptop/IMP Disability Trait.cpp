#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Disability Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Disability Trait.h"
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
	#include "soldier profile type.h"
	#include "IMP Compile Character.h"
	#include "GameSettings.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_DISABILITY_TRAIT__TITLE_FONT				FONT14ARIAL

#define			IMP_DISABILITY_TRAIT__FONT							FONT12ARIAL
#define			IMP_DISABILITY_TRAIT__COLOR						FONT_MCOLOR_WHITE

enum
{
	IMP_DISABILITY_TRAIT__TITLE_TEXT=IMP_DISABILITIES_NUMBER,
};



#define	IMP_DISABILITY_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 136 )
#define	IMP_DISABILITY_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_DISABILITY_TRAIT__SPACE_BTN_BUTTONS									38

#define	IMP_DISABILITY_TRAIT__TEXT_OFFSET_X											65
#define	IMP_DISABILITY_TRAIT__TEXT_OFFSET_Y											12

#define	IMP_DISABILITY_TRAIT__TITLE_X											LAPTOP_SCREEN_UL_X - 111
#define	IMP_DISABILITY_TRAIT__TITLE_Y											iScreenHeightOffset + 53
#define	IMP_DISABILITY_TRAIT__TITLE_WIDTH												( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X )

#define	IMP_DISABILITY_TRAIT__GREY_BOX_OFFSET_X									5
#define	IMP_DISABILITY_TRAIT__GREY_BOX_OFFSET_Y									7


//*******************************************************************
//
// Global Variables
//
//*******************************************************************

BOOLEAN gfIST_Redraw3=FALSE;

BOOLEAN	gfDisabilityTraitQuestions[ IMP_DISABILITIES_NUMBER ];

// these are the buttons for the questions
INT32 giIMPDisabilityTraitAnswerButton[ IMP_DISABILITIES_NUMBER ];
INT32 giIMPDisabilityTraitAnswerButtonImage[ IMP_DISABILITIES_NUMBER ];

// this is the Done	buttons
INT32 giIMPDisabilityTraitFinsihButton;
INT32 giIMPDisabilityTraitFinsihButtonImage;

//image handle
UINT32	guiIST_GreyGoldBox3;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPDisabilityTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPDisabilityTraitFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddImpDisabilityTraitButtons();
void		HandleDisabilityTraitButtonStates( );
void		HandleIMPDisabilityTraitAnswers( UINT32 uiSkillPressed );
void		IMPDisabilityTraitDisplayDisabilityTraits();

BOOLEAN CameBackToDisabilityTraitPageButNotFinished();


//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPDisabilityTrait( void )
{

	VOBJECT_DESC	VObjectDesc;

	//add the disability trait buttons
	AddImpDisabilityTraitButtons();

	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIST_GreyGoldBox3 ) )
	{
		Assert( 0 );
		return;
	}



	giIMPDisabilityTraitFinsihButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPDisabilityTraitFinsihButton = CreateIconAndTextButton( giIMPDisabilityTraitFinsihButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPDisabilityTraitFinishCallback );



	SetButtonCursor( giIMPDisabilityTraitFinsihButton, CURSOR_WWW);

	//if we are not DONE and are just reviewing
	if( iCurrentProfileMode != IMP__FINISH )
	{
		//Have the NONE trait initially selected
		gfDisabilityTraitQuestions[ IMP_NO_DISABILITY ] = TRUE;
	}

	HandleDisabilityTraitButtonStates( );

}


void RenderIMPDisabilityTrait( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	// Display title
	DrawTextToScreen( gzIMPDisabilityTraitText[IMP_DISABILITIES_NUMBER], LAPTOP_SCREEN_UL_X - 111, iScreenHeightOffset + 53, ( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X ), FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	IMPDisabilityTraitDisplayDisabilityTraits();
}


void ExitIMPDisabilityTrait( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIST_GreyGoldBox3 );

	//remove the skill buttons
	for(iCnt = 0; iCnt < IMP_DISABILITIES_NUMBER; iCnt++)
	{
		//if there is a button allocated
		if( giIMPDisabilityTraitAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPDisabilityTraitAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPDisabilityTraitAnswerButtonImage[ iCnt ] );
		}
	}

	RemoveButton( giIMPDisabilityTraitFinsihButton );
	UnloadButtonImage( giIMPDisabilityTraitFinsihButtonImage );
}


void HandleIMPDisabilityTrait( void )
{
	if( gfIST_Redraw3 )
	{
		RenderIMPDisabilityTrait( );
		gfIST_Redraw3 = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddImpDisabilityTraitButtons()
{
	INT32 iCnt = 0;
	UINT16 usPosX, usPosY;

	usPosX = IMP_DISABILITY_COLUMN_START_X;
	usPosY = IMP_DISABILITY_COLUMN_START_Y;

	for(iCnt = 0; iCnt < IMP_DISABILITIES_NUMBER; iCnt++)
	{
		//reset
		giIMPDisabilityTraitAnswerButton[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfDisabilityTraitQuestions[ iCnt ] = FALSE;
		}

		if( iCnt == 0 )
			giIMPDisabilityTraitAnswerButtonImage[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );
		else
			giIMPDisabilityTraitAnswerButtonImage[ iCnt ] =	UseLoadedButtonImage( giIMPDisabilityTraitAnswerButtonImage[ 0 ], -1,0,-1,1,-1 );

		giIMPDisabilityTraitAnswerButton[iCnt] = QuickCreateButton( giIMPDisabilityTraitAnswerButtonImage[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPDisabilityTraitAnswerCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPDisabilityTraitAnswerButton[iCnt], 0, iCnt );
		SetButtonCursor( giIMPDisabilityTraitAnswerButton[iCnt], CURSOR_WWW);

		//Get rid of playing the button sound, it will be handled here
		//ButtonList[ giIMPDisabilityTraitAnswerButton[ iCnt ] ]->ubSoundSchemeID = 0;

		//Determine the next x location
		//if( iCnt < IMP_DISABILITY_TRAIT_NUMBER_TO_START_2ND_COLUMN )
			usPosX = IMP_DISABILITY_COLUMN_START_X;
		//else
		//	usPosX = IMP_DISABILITY_2ND_COLUMN_START_X;

		//Determine the next Y location
		//if( iCnt == IMP_DISABILITY_TRAIT_NUMBER_TO_START_2ND_COLUMN )
		//	usPosY = IMP_DISABILITY_2ND_COLUMN_START_Y;
		//else
			usPosY += IMP_DISABILITY_TRAIT__SPACE_BTN_BUTTONS;
	}
}


void BtnIMPDisabilityTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason)
{

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags|=(BUTTON_CLICKED_ON);

		INT32 iDisabilityTrait =	MSYS_GetBtnUserData( btn, 0 );

		HandleIMPDisabilityTraitAnswers( iDisabilityTrait );
	}
}

void HandleIMPDisabilityTraitAnswers( UINT32 uiSkillPressed )
{
	UINT32 uiCnt;

	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//reset all other buttons
	for( uiCnt=0; uiCnt<IMP_DISABILITIES_NUMBER; uiCnt++ )
	{
		gfDisabilityTraitQuestions[ uiCnt ] = FALSE;
	}

	//make sure its a valid disability trait
	if( uiSkillPressed > IMP_DISABILITIES_NUMBER )
	{
		Assert( 0 );
		return;
	}

	//if its allready set
	if( gfDisabilityTraitQuestions[ uiSkillPressed ] )
	{
		//dont need to do anything
		return;
	}

	//Set the skill
	gfDisabilityTraitQuestions[ uiSkillPressed ] = TRUE;

	//Play the button sound
	if( gfDisabilityTraitQuestions[ uiSkillPressed ] )
	{
		PlayButtonSound( giIMPDisabilityTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
	}
	else
	{
		PlayButtonSound( giIMPDisabilityTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
	}

	//update buttons
	HandleDisabilityTraitButtonStates( );

	//redraw the screen
	gfIST_Redraw3 = TRUE;
}

void HandleDisabilityTraitButtonStates( )
{
	UINT32 uiCnt;

	for( uiCnt=0; uiCnt<IMP_DISABILITIES_NUMBER; uiCnt++ )
	{

		//if the skill is selected ( ie depressed )
		if( gfDisabilityTraitQuestions[ uiCnt ] )
		{
			ButtonList[ giIMPDisabilityTraitAnswerButton[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPDisabilityTraitAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPDisabilityTraitDisplayDisabilityTraits()
{
	UINT32 uiCnt;
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	HVOBJECT	hImageHandle;


	//Display the title
	//DrawTextToScreen( gzIMPSkillTraitsText[ IMP_DISABILITY_TRAIT__TITLE_TEXT ], IMP_DISABILITY_TRAIT__TITLE_X, IMP_DISABILITY_TRAIT__TITLE_Y, IMP_DISABILITY_TRAIT__TITLE_WIDTH, IMP_DISABILITY_TRAIT__TITLE_FONT, IMP_DISABILITY_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Stats
	GetVideoObject(&hImageHandle, guiIST_GreyGoldBox3 );

	usPosX = IMP_DISABILITY_COLUMN_START_X + IMP_DISABILITY_TRAIT__TEXT_OFFSET_X;
	usPosY = IMP_DISABILITY_COLUMN_START_Y + IMP_DISABILITY_TRAIT__TEXT_OFFSET_Y;

	for( uiCnt=0; uiCnt<IMP_DISABILITIES_NUMBER; uiCnt++ )
	{
		usBoxPosX = usPosX - IMP_DISABILITY_TRAIT__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_DISABILITY_TRAIT__GREY_BOX_OFFSET_Y;

		//if the trait is selected
		if( gfDisabilityTraitQuestions[ uiCnt ] )
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
		DrawTextToScreen( gzIMPDisabilityTraitText[ uiCnt ], usPosX, usPosY, 0, IMP_DISABILITY_TRAIT__FONT, IMP_DISABILITY_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		//Determine the next x location
	//	if( uiCnt < IMP_DISABILITY_TRAIT_NUMBER_TO_START_2ND_COLUMN )
			usPosX = IMP_DISABILITY_COLUMN_START_X + IMP_DISABILITY_TRAIT__TEXT_OFFSET_X;
		//else
		//	usPosX = IMP_DISABILITY_2ND_COLUMN_START_X + IMP_DISABILITY_TRAIT__TEXT_OFFSET_X;

		//Determine the next Y location
		//if( uiCnt == IMP_DISABILITY_TRAIT_NUMBER_TO_START_2ND_COLUMN )
		//	usPosY = IMP_DISABILITY_2ND_COLUMN_START_Y + IMP_DISABILITY_TRAIT__TEXT_OFFSET_Y;
		//else
			usPosY += IMP_DISABILITY_TRAIT__SPACE_BTN_BUTTONS;
	}
}


void BtnIMPDisabilityTraitFinishCallback(GUI_BUTTON *btn,INT32 reason)
{

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		//if we are just reviewing the page
		if( iCurrentProfileMode == IMP__FINISH )
		{
			//go back tot he done screen
			iCurrentImpPage = IMP_FINISH;
		}
		else
		{
			iCurrentImpPage = IMP_MAIN_PAGE;

			if( CameBackToDisabilityTraitPageButNotFinished() )
			{
			}
			else
			{
				if ( iCurrentProfileMode != IMP__FINISH )
					iCurrentProfileMode = IMP__PERSONALITY;
				else
					iCurrentProfileMode = IMP__FINISH;
			}
		}
	}
}


BOOLEAN CameBackToDisabilityTraitPageButNotFinished()
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

INT8 iChosenDisabilityTrait()
{
	UINT32	uiCnt;
	INT8 iDisabilityTraitNumber = 0;

	//loop through all the buttons and reset them
	for( uiCnt=0; uiCnt<IMP_DISABILITIES_NUMBER; uiCnt++ )
	{
		//if the trait is selected
		if( gfDisabilityTraitQuestions[ uiCnt ] )
		{
			iDisabilityTraitNumber = uiCnt;
		}
	}
	return( iDisabilityTraitNumber );
}

INT8 iPlayersAttributePointsBonusForDisabilitySelected()
{
	INT8		bExtraAttributePoints = 0;

	//if there is any disability selected, give a bonus to attributes (counted there)
	if( ( iChosenDisabilityTrait() > 0 ) && ( iChosenDisabilityTrait() <= IMP_DISABILITIES_NUMBER ) )
	{
		bExtraAttributePoints = gGameExternalOptions.iBonusPointsForDisability;
	}
	else
	{
		bExtraAttributePoints = 0;
	}

	return( bExtraAttributePoints );
}