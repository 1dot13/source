#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Minor Trait.h"
	#include "IMP Skill Trait.h"
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

#define			IMP_MINOR_TRAIT__TITLE_FONT				FONT14ARIAL

#define			IMP_MINOR_TRAIT__FONT							FONT12ARIAL
#define			IMP_MINOR_TRAIT__COLOR						FONT_MCOLOR_WHITE


#define	IMP_MINOR_TRAIT__LEFT_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 15 )
#define	IMP_MINOR_TRAIT__LEFT_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_MINOR_TRAIT__RIGHT_COLUMN_START_X								( IMP_MINOR_TRAIT__LEFT_COLUMN_START_X + 241 )
#define	IMP_MINOR_TRAIT__RIGHT_COLUMN_START_Y								IMP_MINOR_TRAIT__LEFT_COLUMN_START_Y

#define	IMP_MINOR_TRAIT__SPACE_BTN_BUTTONS									( 38 + 5 )

#define	IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL			5

#define	IMP_MINOR_TRAIT__NONE_BTN_LOC_X											( LAPTOP_SCREEN_UL_X + 136 )
#define	IMP_MINOR_TRAIT__NONE_BTN_LOC_Y											( LAPTOP_SCREEN_WEB_UL_Y + 306 )

#define	IMP_MINOR_TRAIT__TEXT_OFFSET_X											65
#define	IMP_MINOR_TRAIT__TEXT_OFFSET_Y											12

#define	IMP_MINOR_TRAIT__TITLE_X											LAPTOP_SCREEN_UL_X - 111


#define	IMP_MINOR_TRAIT__GREY_BOX_OFFSET_X									5
#define	IMP_MINOR_TRAIT__GREY_BOX_OFFSET_Y									7

//*******************************************************************
//
// Global Variables
//
//*******************************************************************

BOOLEAN gfIMT_Redraw=FALSE;

BOOLEAN	gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS ];

// these are the buttons for the questions
INT32 giIMPMinorTraitAnswerButton[ IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS ];
INT32 giIMPMinorTraitAnswerButtonImage[ IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS ];

// this is the Done	buttons
INT32 giIMPMinorlTraitFinsihButton;
INT32 giIMPMinorlTraitFinsihButtonImage;

INT8	gbLastSelectedMinorTrait[ NUM_MINOR_TRAITS ];

//image handle
UINT32	guiIMT_GreyGoldBox;

MOUSE_REGION	gMR_MinorTraitHelpTextRegions[IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS];

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPMinorTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPMinorTraitFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddIMPMinorTraitButtons();
void		HandleMinorTraitButtonStates( );
void		HandleIMPMinorTraitAnswers( UINT32 uiSkillPressed );
void		IMPMinorTraitDisplaySkills();
BOOLEAN CameBackToMinorTraitPageButNotFinished();
void HandleLastSelectedMinorTrait( INT8 bNewTrait );
INT8 GetLastSelectedMinorTrait( void );
INT8		NumAvailableMinorTraits();

void AssignMinorTraitHelpText( UINT8 ubNumber );
//ppp

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPMinorTrait( void )
{
	UINT8 ubCnt;

//	UINT32 ubCnt;
	VOBJECT_DESC	VObjectDesc;

	//add the skill trait buttons
	AddIMPMinorTraitButtons();


	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIMT_GreyGoldBox ) )
	{
		Assert( 0 );
		return;
	}



	giIMPMinorlTraitFinsihButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPMinorlTraitFinsihButton = CreateIconAndTextButton( giIMPMinorlTraitFinsihButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPMinorTraitFinishCallback );



	SetButtonCursor( giIMPMinorlTraitFinsihButton, CURSOR_WWW);

	//if we are not DONE and are just reviewing
	if( iCurrentProfileMode != IMP__FINISH )
	{
		for (ubCnt = 0; ubCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; ubCnt++ )
		{
			gfMinorTraitQuestions[ ubCnt ] = FALSE;
		}
		//Have the NONE trait initially selected
		gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MINOR_NONE ] = TRUE;
	}

	HandleMinorTraitButtonStates( );

	// add regions for help texts
	UINT16 usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X + 62;
	UINT16 usPosY = IMP_MINOR_TRAIT__LEFT_COLUMN_START_Y + 8;
	for( ubCnt=0; ubCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; ubCnt++ )
	{
		MSYS_DefineRegion( &gMR_MinorTraitHelpTextRegions[ubCnt], ( usPosX ), ( usPosY ),
						(usPosX + 156), ( usPosY + 17), MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
		MSYS_AddRegion( &gMR_MinorTraitHelpTextRegions[ubCnt] );
		
		//Determine the next x location
		if( ubCnt < IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X + 62;
		else
			usPosX = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_X + 62;

		//Determine the next Y location
		if( ubCnt == IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosY = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_Y + 8;
		else
			usPosY += IMP_MINOR_TRAIT__SPACE_BTN_BUTTONS;

		//if its the none button
		if( ubCnt == IMP_SKILL_TRAITS_NEW_MINOR_NONE-1 )
		{
			usPosX = IMP_MINOR_TRAIT__NONE_BTN_LOC_X + 62;
			usPosY = IMP_MINOR_TRAIT__NONE_BTN_LOC_Y + 8;
		}
	}

	//reset last selected trait
	memset( gbLastSelectedMinorTrait, -1, NUM_MINOR_TRAITS );
}


void RenderIMPMinorTrait( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	IMPMinorTraitDisplaySkills();
}


void ExitIMPMinorTrait( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIMT_GreyGoldBox );

	//remove the skill buttons
	for(iCnt = 0; iCnt < IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; iCnt++)
	{
		//if there is a button allocated
		if( giIMPMinorTraitAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPMinorTraitAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPMinorTraitAnswerButtonImage[ iCnt ] );
		}
		
		MSYS_RemoveRegion( &gMR_MinorTraitHelpTextRegions[iCnt] );
	}

	RemoveButton( giIMPMinorlTraitFinsihButton );
	UnloadButtonImage( giIMPMinorlTraitFinsihButtonImage );
}


void HandleIMPMinorTrait( void )
{
	if( gfIMT_Redraw )
	{
		RenderIMPMinorTrait( );
		gfIMT_Redraw = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddIMPMinorTraitButtons()
{
	INT32 iCnt = 0;
	UINT16 usPosX, usPosY;

	usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X;
	usPosY = IMP_MINOR_TRAIT__LEFT_COLUMN_START_Y;


	for(iCnt = 0; iCnt < IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; iCnt++)
	{
		//reset
		giIMPMinorTraitAnswerButton[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfMinorTraitQuestions[ iCnt ] = FALSE;
		}

		if( iCnt == 0 )
			giIMPMinorTraitAnswerButtonImage[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );
		else
			giIMPMinorTraitAnswerButtonImage[ iCnt ] =	UseLoadedButtonImage( giIMPMinorTraitAnswerButtonImage[ 0 ], -1,0,-1,1,-1 );

		giIMPMinorTraitAnswerButton[iCnt] = QuickCreateButton( giIMPMinorTraitAnswerButtonImage[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPMinorTraitAnswerCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPMinorTraitAnswerButton[iCnt], 0, iCnt );
		SetButtonCursor( giIMPMinorTraitAnswerButton[iCnt], CURSOR_WWW);

		//Determine the next x location
		if( iCnt < IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X;
		else
			usPosX = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_X;

		//Determine the next Y location
		if( iCnt == IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosY = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_Y;
		else
			usPosY += IMP_MINOR_TRAIT__SPACE_BTN_BUTTONS;

		//if its the none button
		if( iCnt == IMP_SKILL_TRAITS_NEW_MINOR_NONE-1 )
		{
			usPosX = IMP_MINOR_TRAIT__NONE_BTN_LOC_X;
			usPosY = IMP_MINOR_TRAIT__NONE_BTN_LOC_Y;
		}
	}
}


void BtnIMPMinorTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags|=(BUTTON_CLICKED_ON);

		INT32 iMinorTrait =	MSYS_GetBtnUserData( btn, 0 );

		HandleIMPMinorTraitAnswers( iMinorTrait );
	}
}

void HandleIMPMinorTraitAnswers( UINT32 uiSkillPressed )
{
	UINT32 uiCnt;

	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//make sure its a valid skill
	if( uiSkillPressed >= IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS )
	{
		Assert( 0 );
		return;
	}

	//if its allready set
	if( gfMinorTraitQuestions[ uiSkillPressed ] && uiSkillPressed != IMP_SKILL_TRAITS_NEW_MINOR_NONE )
	{
		gfMinorTraitQuestions[ uiSkillPressed ] = FALSE;

		BOOLEAN fNoSkillSelected = TRUE;
		for ( uiCnt=0; uiCnt<(IMP_SKILL_TRAITS_NEW_MINOR_NONE); uiCnt++ )
		{
			if ( gfMinorTraitQuestions[ uiCnt ] == TRUE )
				fNoSkillSelected = FALSE;
		}
		if ( fNoSkillSelected )
		{
			// select NONE button
			gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MINOR_NONE ] = TRUE;
		}
	}
	else
	{
		//if we are to reset all the buttons
		if( CountNumMinorTraitsSelected( FALSE ) >= NumAvailableMinorTraits() )
		{
			// try to remove the last selected minor trait
			if ( gfMinorTraitQuestions[ GetLastSelectedMinorTrait() ] > 0 )
			{
				gfMinorTraitQuestions[ GetLastSelectedMinorTrait() ] = FALSE;

				// remove this one from list
				HandleLastSelectedMinorTrait( -1 );
			}
			// if there is none, remove all for sure
			else
			{
				//loop through all the skill and reset them
				for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
				{
					gfMinorTraitQuestions[ uiCnt ] = FALSE;
				}
			}
		}
		//Set the skill
		gfMinorTraitQuestions[ uiSkillPressed ] = TRUE;

		HandleLastSelectedMinorTrait( uiSkillPressed );

		//if the NONE trait was selected, clear the rest of the buttons
		if( uiSkillPressed == IMP_SKILL_TRAITS_NEW_MINOR_NONE )
		{
			//reset all the traits
			for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
			{
				gfMinorTraitQuestions[ uiCnt ] = FALSE;
			}
			gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MINOR_NONE ] = TRUE;
		}

		//make sure the none skill is NOT selected if we select anything else
		if( uiSkillPressed != IMP_SKILL_TRAITS_NEW_MINOR_NONE )
		{
			gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MINOR_NONE ] = FALSE;
		}
	}

	//Play the button sound
	if( gfMinorTraitQuestions[ uiSkillPressed ] )
	{
		PlayButtonSound( giIMPMinorTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
	}
	else
	{
		PlayButtonSound( giIMPMinorTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
	}

	//update buttons
	HandleMinorTraitButtonStates( );

	//redraw the screen
	gfIMT_Redraw = TRUE;
}

void HandleMinorTraitButtonStates( )
{
	UINT32 uiCnt;

	for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
	{
		//if the skill is selected ( ie depressed )
		if( gfMinorTraitQuestions[ uiCnt ] )
		{
			ButtonList[ giIMPMinorTraitAnswerButton[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPMinorTraitAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPMinorTraitDisplaySkills()
{
	UINT32 uiCnt;
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	HVOBJECT	hImageHandle;	

	//Display the title
	DrawTextToScreen( gzIMPSkillTraitsTextNewMinor[ IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS ], IMP_MINOR_TRAIT__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, IMP_MINOR_TRAIT__TITLE_FONT, IMP_MINOR_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Stats
	GetVideoObject(&hImageHandle, guiIMT_GreyGoldBox );

	usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X + IMP_MINOR_TRAIT__TEXT_OFFSET_X;
	usPosY = IMP_MINOR_TRAIT__LEFT_COLUMN_START_Y + IMP_MINOR_TRAIT__TEXT_OFFSET_Y;

	for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
	{
		usBoxPosX = usPosX - IMP_MINOR_TRAIT__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_MINOR_TRAIT__GREY_BOX_OFFSET_Y;

		//if the trait is selected
		if( gfMinorTraitQuestions[ uiCnt ] )
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
		DrawTextToScreen( gzIMPSkillTraitsTextNewMinor[ uiCnt ], usPosX, usPosY, 0, IMP_MINOR_TRAIT__FONT, IMP_MINOR_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		
		AssignMinorTraitHelpText( uiCnt );

		//Determine the next x location
		if( uiCnt < IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosX = IMP_MINOR_TRAIT__LEFT_COLUMN_START_X + IMP_MINOR_TRAIT__TEXT_OFFSET_X;
		else
			usPosX = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_X + IMP_MINOR_TRAIT__TEXT_OFFSET_X;

		//Determine the next Y location
		if( uiCnt == IMP_MINOR_TRAIT__TRAITS_TO_START_RIGHT_COL )
			usPosY = IMP_MINOR_TRAIT__RIGHT_COLUMN_START_Y + IMP_MINOR_TRAIT__TEXT_OFFSET_Y;
		else
			usPosY += IMP_MINOR_TRAIT__SPACE_BTN_BUTTONS;

		//if its the none button
		if( uiCnt == IMP_SKILL_TRAITS_NEW_MINOR_NONE-1 )
		{
			usPosX = IMP_MINOR_TRAIT__NONE_BTN_LOC_X + IMP_MINOR_TRAIT__TEXT_OFFSET_X;
			usPosY = IMP_MINOR_TRAIT__NONE_BTN_LOC_Y + IMP_MINOR_TRAIT__TEXT_OFFSET_Y;
		}
	}
}


void BtnIMPMinorTraitFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		// Flugente: setting determines whether we see the background page
		if ( gGameOptions.fBackGround )
		{
			iCurrentImpPage = IMP_BACKGROUND;

			fButtonPendingFlag = TRUE;
		}
		//if we are just reviewing the page
		else if( iCurrentProfileMode == IMP__FINISH )
		{
			//go back to the done screen
			iCurrentImpPage = IMP_FINISH;
		}
		else
		{
			iCurrentImpPage = IMP_MAIN_PAGE;

			fButtonPendingFlag = TRUE;	

			if( CameBackToMinorTraitPageButNotFinished() )
			{

			}
			else
			{
				//We are finished on this page
				iCurrentProfileMode = IMP__ATTRIBUTES;
			}
		}
	}
}

BOOLEAN CameBackToMinorTraitPageButNotFinished()
{
	//if we are in a page that comes after this one
	if( iCurrentProfileMode == IMP__ATTRIBUTES )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

INT8 CountNumMinorTraitsSelected( BOOLEAN fIncludeNoneSkill )
{
	UINT32	uiCnt;
	INT8	iNumberSkills=0;

	for( uiCnt=0; uiCnt < IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
	{
		if( !fIncludeNoneSkill && uiCnt == IMP_SKILL_TRAITS_NEW_MINOR_NONE )
			continue;
		//if the skill is selected ( ie depressed )
		else if( gfMinorTraitQuestions[ uiCnt ] )
		{
			iNumberSkills++;
		}
	}

	return( iNumberSkills );
}

INT8 NumAvailableMinorTraits()
{
	INT8		bNumMinorTraits=0;
	INT8		bNumMajorTraits=0;

	//Count the number of skills selected
	bNumMajorTraits = CountNumSkillTraitsSelected( FALSE );

	bNumMinorTraits = gSkillTraitValues.ubMaxNumberOfTraits - bNumMajorTraits;

	bNumMinorTraits = max( 1, bNumMinorTraits );

	//if ( bNumMajorTraits == 0 )
	//	bNumMinorTraits = 3;
	//else if ( bNumMajorTraits == 1 )
	//	bNumMinorTraits = 2;
	//else if ( bNumMajorTraits == 2 )
	//	bNumMinorTraits = 1;
	//else
	//{
	//	Assert( 0 );
	//	bNumMinorTraits = 0;
	//}

	return(bNumMinorTraits);
}


void AddSelectedMinorTraitsToSkillsList()
{
	UINT32	uiCnt;

	// if we have somehow reached beyond available traits, try to fix it by removing the last one selected
	if ( NumAvailableMinorTraits() < CountNumMinorTraitsSelected( FALSE ) )
	{
		if (gfMinorTraitQuestions[ GetLastSelectedMinorTrait() ] == TRUE )
			gfMinorTraitQuestions[ GetLastSelectedMinorTrait() ] = FALSE;
	}
	// if still something is wrong, remove them all
	if ( NumAvailableMinorTraits() < CountNumMinorTraitsSelected( FALSE ) )
	{
		for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS; uiCnt++ )
		{
			gfMinorTraitQuestions[ uiCnt ] = FALSE;
		}
		gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MINOR_NONE ] = TRUE;	
	}

	// OK, now add what we have to the list
	for( uiCnt=0; uiCnt<IMP_SKILL_TRAITS_NEW_MINOR_NONE; uiCnt++ )
	{
		//if the trait is selected
		if( gfMinorTraitQuestions[ uiCnt ] )
		{
			//switch on the trait, and add it to the list
			switch( uiCnt )
			{
				case IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS:
					AddSkillToSkillList( AMBIDEXTROUS_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_MELEE:
					AddSkillToSkillList( MELEE_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_THROWING:
					AddSkillToSkillList( THROWING_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_NIGHT_OPS:
					AddSkillToSkillList( NIGHT_OPS_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_STEALTHY:
					AddSkillToSkillList( STEALTHY_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_ATHLETICS:
					AddSkillToSkillList( ATHLETICS_NT );
					break;
	
				case IMP_SKILL_TRAITS_NEW_BODYBUILDING:
					AddSkillToSkillList( BODYBUILDING_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_DEMOLITIONS:
					AddSkillToSkillList( DEMOLITIONS_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_TEACHING:
					AddSkillToSkillList( TEACHING_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_SCOUTING:
					AddSkillToSkillList( SCOUTING_NT );
					break;

				case IMP_SKILL_TRAITS_NEW_RADIO_OPERATOR:
					AddSkillToSkillList( RADIO_OPERATOR_NT );
					break;
						
				default:
					break;
			}
		}
	}
}


void HandleLastSelectedMinorTrait( INT8 bNewTrait )
{
	INT16 iCnt;

	if (bNewTrait == -1)
	{
		// we only want to delete the last selected trait from list
		for( iCnt=NUM_MINOR_TRAITS-1; iCnt>=0; iCnt-- )
		{
			if ( gbLastSelectedMinorTrait[ iCnt ] > 0 )
			{
				gbLastSelectedMinorTrait[ iCnt ] = 0;
				break;
			}
		}
		
	}
	else
	{
		for( iCnt=0; iCnt<NUM_MINOR_TRAITS-1; iCnt++ )
		{
			gbLastSelectedMinorTrait[ (iCnt + 1) ] = gbLastSelectedMinorTrait[ iCnt ];
		}
		gbLastSelectedMinorTrait[ 0 ] = bNewTrait;
	}

	// here in "gbLastSelectedMinorTrait[ 0 ]" must always be the oldest minor trait selected
	// this is indeed pretty sh*tty code
	/*if( bNewTrait == IMP_SKILL_TRAITS_NEW_MINOR_NONE )
	{
		gbLastSelectedMinorTrait[ 0 ] = -1;
		gbLastSelectedMinorTrait[ 1 ] = -1;
		gbLastSelectedMinorTrait[ 2 ] = -1;
		return;
	}

	if( gfMinorTraitQuestions[ gbLastSelectedMinorTrait[ 0 ] ] == FALSE )
		gbLastSelectedMinorTrait[ 0 ] = -1;
	if( gfMinorTraitQuestions[ gbLastSelectedMinorTrait[ 1 ] ] == FALSE ) 
		gbLastSelectedMinorTrait[ 1 ] = -1;
	if( gfMinorTraitQuestions[ gbLastSelectedMinorTrait[ 2 ] ] == FALSE ) 
		gbLastSelectedMinorTrait[ 2 ] = -1;

	if( gbLastSelectedMinorTrait[ 0 ] == -1 )
	{
		if( gbLastSelectedMinorTrait[ 1 ] != -1 ) 
			gbLastSelectedMinorTrait[ 0 ] = gbLastSelectedMinorTrait[ 1 ];	
		else if( gbLastSelectedMinorTrait[ 2 ] != -1 ) 
			gbLastSelectedMinorTrait[ 0 ] = gbLastSelectedMinorTrait[ 2 ];	
	}
	else if( gbLastSelectedMinorTrait[ 1 ] == -1 && gbLastSelectedMinorTrait[ 2 ] != -1 )
	{
		gbLastSelectedMinorTrait[ 1 ] = gbLastSelectedMinorTrait[ 2 ];	
	}

	if( NumAvailableMinorTraits() == 1 )
	{
		gbLastSelectedMinorTrait[ 0 ] = bNewTrait;
	}
	else if( NumAvailableMinorTraits() == 2 )
	{
		if( gbLastSelectedMinorTrait[ 0 ] == -1 )
		{
			gbLastSelectedMinorTrait[ 0 ] = bNewTrait;
		}
		else if( gbLastSelectedMinorTrait[ 1 ] == -1 )
		{
			gbLastSelectedMinorTrait[ 1 ] = bNewTrait;
		}
		else 
		{
			gbLastSelectedMinorTrait[ 0 ] = gbLastSelectedMinorTrait[ 1 ];
			gbLastSelectedMinorTrait[ 1 ] = bNewTrait;
		}
	}
	else if( NumAvailableMinorTraits() == 3 )
	{
		if( gbLastSelectedMinorTrait[ 0 ] == -1 )
		{
			gbLastSelectedMinorTrait[ 0 ] = bNewTrait;
		}
		else if( gbLastSelectedMinorTrait[ 1 ] == -1 )
		{
			gbLastSelectedMinorTrait[ 1 ] = bNewTrait;
		}
		else if( gbLastSelectedMinorTrait[ 2 ] == -1 )
		{
			gbLastSelectedMinorTrait[ 2 ] = bNewTrait;
		}
		else
		{
			gbLastSelectedMinorTrait[ 0 ] = gbLastSelectedMinorTrait[ 1 ];
			gbLastSelectedMinorTrait[ 1 ] = gbLastSelectedMinorTrait[ 2 ];
			gbLastSelectedMinorTrait[ 2 ] = bNewTrait;
		}
	}
	else
	{
		Assert( 0 );
		gbLastSelectedMinorTrait[ 0 ] = bNewTrait;
	}*/
}

INT8 GetLastSelectedMinorTrait( void )
{
	INT16 iCnt;

	for( iCnt=NUM_MINOR_TRAITS-1; iCnt>=0; iCnt-- )
	{
		if ( gbLastSelectedMinorTrait[ iCnt ] > 0 )
		{
			return ( gbLastSelectedMinorTrait[ iCnt ] );
			break;
		}
	}
	return ( 0 );
}

UINT8 StrengthRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
		// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_MELEE ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}
	return( 0 );
}
UINT8 DexterityRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS ] && gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_THROWING ] )
	{
		// 70 is minimum if having both, +20 is addition to other preset requirements
		if( (iInitialValue + 20) < 70 )
		{
			// if less than minimum, add how much we need
			return ( 70 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 20 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_THROWING ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}
UINT8 AgilityRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_ATHLETICS ] && gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_STEALTHY ] )
	{
		// 70 is minimum if having both, +20 is addition to other preset requirements
		if( (iInitialValue + 20) < 70 )
		{
			// if less than minimum, add how much we need
			return ( 70 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 20 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_ATHLETICS ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_STEALTHY ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}
UINT8 HealthRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_BODYBUILDING ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}
UINT8 WisdomRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_NIGHT_OPS ] && gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_SCOUTING ] )
	{
		// 70 is minimum if having both, +20 is addition to other preset requirements
		if( (iInitialValue + 20) < 70 )
		{
			// if less than minimum, add how much we need
			return ( 70 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 20 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_NIGHT_OPS ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}
	else if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_SCOUTING ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 60 )
		{
			// if less than minimum, add how much we need
			return ( 60 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}
UINT8 LeaderShipRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_TEACHING ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 45 )
		{
			// if less than minimum, add how much we need
			return ( 45 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}
UINT8 MarksmanshipRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// nothing here
	return( 0 );
}
UINT8 MechanicalRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// nothing here
	return( 0 );
}
UINT8 MedicalRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// nothing here
	return( 0 );
}
UINT8 ExplosivesRequiredAdjustmentForMinorTraits( INT32 iInitialValue )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	if ( gfMinorTraitQuestions[ IMP_SKILL_TRAITS_NEW_DEMOLITIONS ] )
	{
		// 60 is minimum, +10 is addition to other preset requirements
		if( (iInitialValue + 10) < 45 )
		{
			// if less than minimum, add how much we need
			return ( 45 - iInitialValue );
		}
		else 
		{
			// otherwise just add 10 on top
			return ( 10 );
		}
	}

	return( 0 );
}

void AssignMinorTraitHelpText( UINT8 ubNumber )
{
	CHAR16	apStr[ 5000 ];
	CHAR16	atStr[ 1500 ];

	switch( ubNumber )
	{
		case IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubAMPenaltyDoubleReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[0], gSkillTraitValues.ubAMPenaltyDoubleReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMReloadSpeedMagazines != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[1], gSkillTraitValues.ubAMReloadSpeedMagazines, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMReloadSpeedLoose != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[2], gSkillTraitValues.ubAMReloadSpeedLoose, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMPickItemsAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[3], gSkillTraitValues.ubAMPickItemsAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMWorkBackpackAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[4], gSkillTraitValues.ubAMWorkBackpackAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMHandleDoorsAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[5], gSkillTraitValues.ubAMHandleDoorsAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMHandleBombsAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[6], gSkillTraitValues.ubAMHandleBombsAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubAMAttachingItemsAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[7], gSkillTraitValues.ubAMAttachingItemsAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_MELEE:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubMEBladesAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[0], gSkillTraitValues.ubMEBladesAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMECtHBladesBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[1], gSkillTraitValues.ubMECtHBladesBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMECtHBluntBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[2], gSkillTraitValues.ubMECtHBluntBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMEDamageBonusBlades != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[3], gSkillTraitValues.ubMEDamageBonusBlades, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMEDamageBonusBlunt != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[4], gSkillTraitValues.ubMEDamageBonusBlunt, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.usMEAimedMeleeAttackDamageBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[5], gSkillTraitValues.usMEAimedMeleeAttackDamageBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMEDodgeBladesBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[6], gSkillTraitValues.ubMEDodgeBladesBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[7], gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMEDodgeBluntBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[8], gSkillTraitValues.ubMEDodgeBluntBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[9], gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_THROWING:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubTHBladesAPsReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[0], gSkillTraitValues.ubTHBladesAPsReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesMaxRange != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[1], gSkillTraitValues.ubTHBladesMaxRange, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesCtHBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[2], gSkillTraitValues.ubTHBladesCtHBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesCtHBonusPerClick != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[3], gSkillTraitValues.ubTHBladesCtHBonusPerClick, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesDamageBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[4], gSkillTraitValues.ubTHBladesDamageBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesDamageBonusPerClick != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[5], gSkillTraitValues.ubTHBladesDamageBonusPerClick, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesSilentCriticalHitChance != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[6], gSkillTraitValues.ubTHBladesSilentCriticalHitChance, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[7], gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTHBladesAimClicksAdded != 0 )
			{
				if( gSkillTraitValues.ubTHBladesAimClicksAdded == 1 )
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[8], gSkillTraitValues.ubTHBladesAimClicksAdded );
				else
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[9], gSkillTraitValues.ubTHBladesAimClicksAdded );

				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_NIGHT_OPS:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubNOeSightRangeBonusInDark != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[0], gSkillTraitValues.ubNOeSightRangeBonusInDark, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubNOHearingRangeBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[1], gSkillTraitValues.ubNOHearingRangeBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubNOHearingRangeBonusInDark != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[2], gSkillTraitValues.ubNOHearingRangeBonusInDark, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubNOIterruptsBonusInDark != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[3], gSkillTraitValues.ubNOIterruptsBonusInDark, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubNONeedForSleepReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[4], gSkillTraitValues.ubNONeedForSleepReduction);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_STEALTHY:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubSTStealthModeSpeedBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[0], gSkillTraitValues.ubSTStealthModeSpeedBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubSTBonusToMoveQuietly != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[1], gSkillTraitValues.ubSTBonusToMoveQuietly, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubSTStealthBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[2], gSkillTraitValues.ubSTStealthBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubSTReducedAPsRegistered != 0 && gGameOptions.fImprovedInterruptSystem )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[4], gSkillTraitValues.ubSTReducedAPsRegistered, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubSTStealthPenaltyForMovingReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[3], gSkillTraitValues.ubSTStealthPenaltyForMovingReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_ATHLETICS:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubATAPsMovementReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAthletics[0], gSkillTraitValues.ubATAPsMovementReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubATBPsMovementReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsAthletics[1], gSkillTraitValues.ubATBPsMovementReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_BODYBUILDING:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubBBDamageResistance != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[0], gSkillTraitValues.ubBBDamageResistance, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubBBCarryWeightBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[1], gSkillTraitValues.ubBBCarryWeightBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubBBBreathLossForHtHImpactReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[2], gSkillTraitValues.ubBBBreathLossForHtHImpactReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.usBBIncreasedNeededDamageToFallDown != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[3], gSkillTraitValues.usBBIncreasedNeededDamageToFallDown, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_DEMOLITIONS:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubDEAPsNeededToThrowGrenadesReduction != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[0], gSkillTraitValues.ubDEAPsNeededToThrowGrenadesReduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEMaxRangeToThrowGrenades != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[1], gSkillTraitValues.ubDEMaxRangeToThrowGrenades, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDECtHWhenThrowingGrenades != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[2], gSkillTraitValues.ubDECtHWhenThrowingGrenades, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEDamageOfBombsAndMines != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[3], gSkillTraitValues.ubDEDamageOfBombsAndMines, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEAttachDetonatorCheckBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[4], gSkillTraitValues.ubDEAttachDetonatorCheckBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[5], gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEPlacedBombLevelBonus != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[6], gSkillTraitValues.ubDEPlacedBombLevelBonus);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubDEShapedChargeDamageMultiplier != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[7], gSkillTraitValues.ubDEShapedChargeDamageMultiplier);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_TEACHING:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubTGBonusToTrainMilitia != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[0], gSkillTraitValues.ubTGBonusToTrainMilitia, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[1], gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTGBonusToTeachOtherMercs != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[2], gSkillTraitValues.ubTGBonusToTeachOtherMercs, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTGEffectiveSkillValueForTeaching != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[3], gSkillTraitValues.ubTGEffectiveSkillValueForTeaching);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubTGBonusOnPractising != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[4], gSkillTraitValues.ubTGBonusOnPractising, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_SCOUTING:
		{
			swprintf( apStr, L"" );
			if( gSkillTraitValues.ubSCSightRangebonusWithScopes != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[0], gSkillTraitValues.ubSCSightRangebonusWithScopes, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.usSCSightRangebonusWithBinoculars != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[1], gSkillTraitValues.usSCSightRangebonusWithBinoculars, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars != 0 )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[2], gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.fSCCanDetectEnemyPresenseAround  )
			{
				if( gSkillTraitValues.fSCCanDetermineEnemyNumbersAround )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[3]);
					wcscat( apStr, atStr );
				}
				else
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[4]);
					wcscat( apStr, atStr );
				}
			}
			if( gSkillTraitValues.fSCPreventsTheEnemyToAmbushMercs )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[5]);
				wcscat( apStr, atStr );
			}
			if( gSkillTraitValues.fSCPreventsBloodcatsAmbushes )
			{
				swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[6]);
				wcscat( apStr, atStr );
			}
			break;
		}
		case IMP_SKILL_TRAITS_NEW_RADIO_OPERATOR:
		{
			swprintf( apStr, L"" );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[0]);
			wcscat( apStr, atStr );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[1]);
			wcscat( apStr, atStr );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[2]);
			wcscat( apStr, atStr );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[3]);
			wcscat( apStr, atStr );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[4]);
			wcscat( apStr, atStr );
			swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[5]);
			wcscat( apStr, atStr );
			break;
		}
		case IMP_SKILL_TRAITS_NEW_MINOR_NONE:
		{
			swprintf( apStr, gzIMPMinorTraitsHelpTextsNone[0] );
			break;
		}
	}

	// Set region help text
	SetRegionFastHelpText( &(gMR_MinorTraitHelpTextRegions[ubNumber]), apStr );
	SetRegionHelpEndCallback( &gMR_MinorTraitHelpTextRegions[ubNumber], MSYS_NO_CALLBACK );

	return;
}