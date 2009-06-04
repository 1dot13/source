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
	#include "soldier profile type.h"
	#include "IMP Compile Character.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_CHARACTER_TRAIT__TITLE_FONT				FONT14ARIAL

#define			IMP_CHARACTER_TRAIT__FONT							FONT12ARIAL
#define			IMP_CHARACTER_TRAIT__COLOR						FONT_MCOLOR_WHITE

enum
{
	IMP_CHARACTER_TRAIT__TITLE_TEXT=IMP_CHARACTER_TRAITS_NUMBER,
};



#define	IMP_CHARACTER_1ST_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 15 )
#define	IMP_CHARACTER_1ST_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_CHARACTER_2ND_COLUMN_START_X								( IMP_CHARACTER_1ST_COLUMN_START_X + 241 )
#define	IMP_CHARACTER_2ND_COLUMN_START_Y								IMP_CHARACTER_1ST_COLUMN_START_Y

#define	IMP_CHARACTER_TRAIT__SPACE_BTN_BUTTONS									38

#define	IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN			5 

#define	IMP_CHARACTER_TRAIT__TEXT_OFFSET_X											65
#define	IMP_CHARACTER_TRAIT__TEXT_OFFSET_Y											12

#define	IMP_CHARACTER_TRAIT__TITLE_X											LAPTOP_SCREEN_UL_X - 111
#define	IMP_CHARACTER_TRAIT__TITLE_Y											iScreenHeightOffset + 53
#define	IMP_CHARACTER_TRAIT__TITLE_WIDTH												( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X )

#define	IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_X									5
#define	IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_Y									7


//*******************************************************************
//
// Global Variables
//
//*******************************************************************

BOOLEAN gfIST_Redraw2=FALSE;

BOOLEAN	gfCharacterTraitQuestions[ IMP_CHARACTER_TRAITS_NUMBER ];

// these are the buttons for the questions
INT32 giIMPCharacterTraitAnswerButton[ IMP_CHARACTER_TRAITS_NUMBER ];
INT32 giIMPCharacterTraitAnswerButtonImage[ IMP_CHARACTER_TRAITS_NUMBER ];

// this is the Done	buttons
INT32 giIMPCharacterTraitFinsihButton;
INT32 giIMPCharacterTraitFinsihButtonImage;

//image handle
UINT32	guiIST_GreyGoldBox2;

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

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPCharacterTrait( void )
{
//	UINT32 uiCnt;
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
		gfCharacterTraitQuestions[ IMP_CHARACTER_TRAIT_NORMAL ] = TRUE;
	}

	HandleCharacterTraitButtonStates( );

}


void RenderIMPCharacterTrait( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	//Display the title
	DrawTextToScreen( gzIMPCharacterTraitText[IMP_CHARACTER_TRAITS_NUMBER], LAPTOP_SCREEN_UL_X - 111, iScreenHeightOffset + 53, ( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X ), FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	IMPCharacterTraitDisplayCharacterTraits();
}


void ExitIMPCharacterTrait( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIST_GreyGoldBox2 );

	//remove the skill buttons
	for(iCnt = 0; iCnt < IMP_CHARACTER_TRAITS_NUMBER; iCnt++)
	{
		//if there is a button allocated
		if( giIMPCharacterTraitAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPCharacterTraitAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPCharacterTraitAnswerButtonImage[ iCnt ] );
		}
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

	for(iCnt = 0; iCnt < IMP_CHARACTER_TRAITS_NUMBER; iCnt++)
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
		if( iCnt < IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN )
			usPosX = IMP_CHARACTER_1ST_COLUMN_START_X;
		else
			usPosX = IMP_CHARACTER_2ND_COLUMN_START_X;

		//Determine the next Y location
		if( iCnt == IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN )
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
	UINT32 uiCnt;

	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//reset all other buttons
	for( uiCnt=0; uiCnt<IMP_CHARACTER_TRAITS_NUMBER; uiCnt++ )
	{
		gfCharacterTraitQuestions[ uiCnt ] = FALSE;
	}

	//make sure its a valid character trait
	if( uiSkillPressed > IMP_CHARACTER_TRAITS_NUMBER )
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
	UINT32 uiCnt;

	for( uiCnt=0; uiCnt<IMP_CHARACTER_TRAITS_NUMBER; uiCnt++ )
	{

		//if the skill is selected ( ie depressed )
		if( gfCharacterTraitQuestions[ uiCnt ] )
		{
			ButtonList[ giIMPCharacterTraitAnswerButton[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPCharacterTraitAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPCharacterTraitDisplayCharacterTraits()
{
	UINT32 uiCnt;
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	HVOBJECT	hImageHandle;

	// Stats
	GetVideoObject(&hImageHandle, guiIST_GreyGoldBox2 );

	usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;
	usPosY = IMP_CHARACTER_1ST_COLUMN_START_Y + IMP_CHARACTER_TRAIT__TEXT_OFFSET_Y;

	for( uiCnt=0; uiCnt<IMP_CHARACTER_TRAITS_NUMBER; uiCnt++ )
	{
		usBoxPosX = usPosX - IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_CHARACTER_TRAIT__GREY_BOX_OFFSET_Y;

		//if the trait is selected
		if( gfCharacterTraitQuestions[ uiCnt ] )
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
		DrawTextToScreen( gzIMPCharacterTraitText[ uiCnt ], usPosX, usPosY, 0, IMP_CHARACTER_TRAIT__FONT, IMP_CHARACTER_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

		//Determine the next x location
		if( uiCnt < IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN )
			usPosX = IMP_CHARACTER_1ST_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;
		else
			usPosX = IMP_CHARACTER_2ND_COLUMN_START_X + IMP_CHARACTER_TRAIT__TEXT_OFFSET_X;

		//Determine the next Y location
		if( uiCnt == IMP_CHARACTER_TRAIT_NUMBER_TO_START_2ND_COLUMN )
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
	UINT32	uiCnt;
	INT8 iCharacterTraitNumber = 0;

	//loop through all the buttons and reset them
	for( uiCnt=0; uiCnt<IMP_CHARACTER_TRAITS_NUMBER; uiCnt++ )
	{
		//if the trait is selected
		if( gfCharacterTraitQuestions[ uiCnt ] )
		{
			iCharacterTraitNumber = uiCnt;
		}
	}
	return( iCharacterTraitNumber );
}

