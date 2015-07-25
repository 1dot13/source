#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Background.h"
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
	#include "Interface.h"
#endif



//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_BACKGROUND__TITLE_FONT				FONT14ARIAL

#define			IMP_BACKGROUND__FONT							FONT12ARIAL
#define			IMP_BACKGROUND__COLOR						FONT_MCOLOR_WHITE

#define	IMP_BACKGROUND_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 136 )
#define	IMP_BACKGROUND_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_BACKGROUND__SPACE_BTN_BUTTONS									( 38 + 0 )

#define	IMP_BACKGROUND__TRAITS_TO_START_RIGHT_COL			8

#define	IMP_BACKGROUND__NONE_BTN_LOC_X											( LAPTOP_SCREEN_UL_X + 136 )
#define	IMP_BACKGROUND__NONE_BTN_LOC_Y											( LAPTOP_SCREEN_WEB_UL_Y + 306 )

#define	IMP_BACKGROUND__TEXT_OFFSET_X											65
#define	IMP_BACKGROUND__TEXT_OFFSET_Y											12

#define	IMP_BACKGROUND__TITLE_X											LAPTOP_SCREEN_UL_X - 111

#define	IMP_BACKGROUND__GREY_BOX_OFFSET_X									5
#define	IMP_BACKGROUND__GREY_BOX_OFFSET_Y									7

//*******************************************************************
//
// Global Variables
//
//*******************************************************************

BOOLEAN gfIMPBackground_Redraw=FALSE;

#define IMP_BACKGROUND_DISPLAYED_CHOICES	8
#define IMP_BACKGROUND_NONE					IMP_BACKGROUND_DISPLAYED_CHOICES - 1
BOOLEAN	gfBackgroundQuestions[ IMP_BACKGROUND_DISPLAYED_CHOICES ];

// these are the buttons for the questions
INT32 giIMPBackgroundAnswerButton[ IMP_BACKGROUND_DISPLAYED_CHOICES ];
INT32 giIMPBackgroundAnswerButtonImage[ IMP_BACKGROUND_DISPLAYED_CHOICES ];

// this is the Done	buttons
INT32 giIMPBackgroundFinishButton;
INT32 giIMPBackgroundFinishButtonImage;

INT8	gbLastSelectedBackground[ 10 ];

//image handle
UINT32	guiIMPBackground_GreyGoldBox;

MOUSE_REGION	gMR_BackgroundHelpTextRegions[IMP_BACKGROUND_DISPLAYED_CHOICES];

// forward/backward buttons
INT32 giIMPBackgroundButton[ 2 ];
INT32 giIMPBackgroundButtonImage[ 2 ];

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPBackgroundAnswerCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPBackgroundFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddIMPBackgroundButtons();
void		HandleBackgroundButtonStates( );
void		HandleIMPBackgroundAnswers( UINT16 uiSkillPressed );
void		IMPBackgroundDisplaySkills();
BOOLEAN		CameBackToBackgroundPageButNotFinished();

void		DestroyIMPBackgroundButtons( void );

// callbacks
void		BtnIMPBackgroundNextCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPBackgroundPreviousCallback(GUI_BUTTON *btn,INT32 reason);

void		UpdateDisplaySkills( BOOLEAN fIncrease );
UINT16		GetDisplaySkill( UINT16 aNr );
void		ResetDisplaySkills();
//ppp

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPBackground( void )
{
	VOBJECT_DESC	VObjectDesc;
		
	//add the skill trait buttons
	AddIMPBackgroundButtons();

	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIMPBackground_GreyGoldBox ) )
	{
		Assert( 0 );
		return;
	}
	
	giIMPBackgroundFinishButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPBackgroundFinishButton = CreateIconAndTextButton( giIMPBackgroundFinishButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPBackgroundFinishCallback );
	
	SetButtonCursor( giIMPBackgroundFinishButton, CURSOR_WWW);

	//if we are not DONE and are just reviewing
	if( iCurrentProfileMode != IMP__FINISH )
	{
		for (UINT16 ubCnt = 0; ubCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++ubCnt )
		{
			gfBackgroundQuestions[ ubCnt ] = FALSE;
		}
		//Have the NONE trait initially selected
		gfBackgroundQuestions[ IMP_BACKGROUND_NONE ] = TRUE;
	}

	HandleBackgroundButtonStates( );

	// add regions for help texts
	UINT16 usPosY = IMP_BACKGROUND_COLUMN_START_Y + 8;
	for( UINT16 ubCnt=0; ubCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++ubCnt )
	{
		MSYS_DefineRegion( &gMR_BackgroundHelpTextRegions[ubCnt], ( IMP_BACKGROUND_COLUMN_START_X + 62 ), ( usPosY ),
						(IMP_BACKGROUND_COLUMN_START_X + 218), ( usPosY + 17), MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
		MSYS_AddRegion( &gMR_BackgroundHelpTextRegions[ubCnt] );
						
		usPosY += IMP_BACKGROUND__SPACE_BTN_BUTTONS;
	}

	ResetDisplaySkills();

	//reset last selected trait
	memset( gbLastSelectedBackground, -1, 10 );
}


void RenderIMPBackground( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	IMPBackgroundDisplaySkills();
}


void ExitIMPBackground( void )
{
	DeleteVideoObjectFromIndex( guiIMPBackground_GreyGoldBox );

	DestroyIMPBackgroundButtons( );

	//remove the skill buttons
	for(UINT16 iCnt = 0; iCnt < IMP_BACKGROUND_DISPLAYED_CHOICES; iCnt++)
	{
		//if there is a button allocated
		if( giIMPBackgroundAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPBackgroundAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPBackgroundAnswerButtonImage[ iCnt ] );
		}
		
		MSYS_RemoveRegion( &gMR_BackgroundHelpTextRegions[iCnt] );
	}

	RemoveButton( giIMPBackgroundFinishButton );
	UnloadButtonImage( giIMPBackgroundFinishButtonImage );
}


void HandleIMPBackground( void )
{
	if( gfIMPBackground_Redraw )
	{
		RenderIMPBackground( );
		gfIMPBackground_Redraw = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddIMPBackgroundButtons()
{
	UINT16 usPosX, usPosY;
		
	// next button
	giIMPBackgroundButtonImage[0]=	LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,1,-1,3,-1 );
	giIMPBackgroundButton[0] = CreateIconAndTextButton( giIMPBackgroundButtonImage[0], pImpButtonText[ 13 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 383 ), LAPTOP_SCREEN_WEB_UL_Y + ( 150 ),BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPBackgroundNextCallback);
	
	// previous button
	giIMPBackgroundButtonImage[ 1 ]=	LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,0,-1,2,-1 );
	giIMPBackgroundButton[ 1 ] = CreateIconAndTextButton( giIMPBackgroundButtonImage[ 1 ], pImpButtonText[ 12 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 43), LAPTOP_SCREEN_WEB_UL_Y + ( 150 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPBackgroundPreviousCallback);

	SetButtonCursor(giIMPBackgroundButton[0], CURSOR_WWW);
	SetButtonCursor(giIMPBackgroundButton[1], CURSOR_WWW);

	usPosX = IMP_BACKGROUND_COLUMN_START_X;
	usPosY = IMP_BACKGROUND_COLUMN_START_Y;
	
	for(UINT16 iCnt = 0; iCnt < IMP_BACKGROUND_DISPLAYED_CHOICES; ++iCnt)
	{
		//reset
		giIMPBackgroundAnswerButton[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfBackgroundQuestions[ iCnt ] = FALSE;
		}				

		if( iCnt == 0 )
			giIMPBackgroundAnswerButtonImage[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );
		else
			giIMPBackgroundAnswerButtonImage[ iCnt ] =	UseLoadedButtonImage( giIMPBackgroundAnswerButtonImage[ 0 ], -1,0,-1,1,-1 );
		
		giIMPBackgroundAnswerButton[iCnt] = QuickCreateButton( giIMPBackgroundAnswerButtonImage[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPBackgroundAnswerCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPBackgroundAnswerButton[iCnt], 0, iCnt );
		SetButtonCursor( giIMPBackgroundAnswerButton[iCnt], CURSOR_WWW);

		usPosX = IMP_BACKGROUND_COLUMN_START_X;
		usPosY += IMP_BACKGROUND__SPACE_BTN_BUTTONS;

		//if its the none button
		if( iCnt == IMP_BACKGROUND_NONE-1 )
		{
			usPosX = IMP_BACKGROUND__NONE_BTN_LOC_X;
			usPosY = IMP_BACKGROUND__NONE_BTN_LOC_Y;
		}
	}
}


void BtnIMPBackgroundAnswerCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		UINT16 usBackground = (UINT16)(MSYS_GetBtnUserData( btn, 0 ));

		HandleIMPBackgroundAnswers( usBackground );
	}
}

void HandleIMPBackgroundAnswers( UINT16 uiSkillPressed )
{
	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//make sure its a valid skill
	if( uiSkillPressed > IMP_BACKGROUND_DISPLAYED_CHOICES )
	{
		Assert( 0 );
		return;
	}

	//reset all other buttons
	for( UINT16 uiCnt=0; uiCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++uiCnt )
	{
		gfBackgroundQuestions[ uiCnt ] = FALSE;
	}

	usBackground = GetDisplaySkill( uiSkillPressed );

	//Set the skill
	gfBackgroundQuestions[ uiSkillPressed ] = TRUE;
		
	//Play the button sound
	if( gfBackgroundQuestions[ uiSkillPressed ] )
	{
		PlayButtonSound( giIMPBackgroundAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
	}
	else
	{
		PlayButtonSound( giIMPBackgroundAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
	}

	//update buttons
	HandleBackgroundButtonStates( );

	//redraw the screen
	gfIMPBackground_Redraw = TRUE;
}

void HandleBackgroundButtonStates( )
{
	for( UINT16 uiCnt=0; uiCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++uiCnt )
	{
		//if the skill is selected ( ie depressed )
		if( gfBackgroundQuestions[ uiCnt ] )
		{
			ButtonList[ giIMPBackgroundAnswerButton[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPBackgroundAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPBackgroundDisplaySkills()
{
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	HVOBJECT	hImageHandle;

	//Display the title
	DrawTextToScreen( szBackgroundTitleText[0], IMP_BACKGROUND__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, IMP_BACKGROUND__TITLE_FONT, IMP_BACKGROUND__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Stats
	GetVideoObject(&hImageHandle, guiIMPBackground_GreyGoldBox );
	
	usPosX = IMP_BACKGROUND_COLUMN_START_X + IMP_BACKGROUND__TEXT_OFFSET_X;
	usPosY = IMP_BACKGROUND_COLUMN_START_Y + IMP_BACKGROUND__TEXT_OFFSET_Y;

	for( UINT16 uiCnt=0; uiCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++uiCnt )
	{
		UINT16 background = GetDisplaySkill( uiCnt );

		usBoxPosX = usPosX - IMP_BACKGROUND__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_BACKGROUND__GREY_BOX_OFFSET_Y;

		//if the trait is selected
		if( gfBackgroundQuestions[ uiCnt ] )
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
		if ( uiCnt == IMP_BACKGROUND_NONE )
			DrawTextToScreen( pImpButtonText[ 26 ], usPosX, usPosY, 0, IMP_BACKGROUND__FONT, IMP_BACKGROUND__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		else
			DrawTextToScreen( zBackground[ background ].szShortName, usPosX, usPosY, 0, IMP_BACKGROUND__FONT, IMP_BACKGROUND__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		
		AssignBackgroundHelpText( background, &(gMR_BackgroundHelpTextRegions[uiCnt]) );
		
		usPosX = IMP_BACKGROUND_COLUMN_START_X + IMP_BACKGROUND__TEXT_OFFSET_X;
		usPosY += IMP_BACKGROUND__SPACE_BTN_BUTTONS;

		//if its the none button
		if( uiCnt == IMP_BACKGROUND_NONE-1 )
		{
			usPosX = IMP_BACKGROUND__NONE_BTN_LOC_X + IMP_BACKGROUND__TEXT_OFFSET_X;
			usPosY = IMP_BACKGROUND__NONE_BTN_LOC_Y + IMP_BACKGROUND__TEXT_OFFSET_Y;
		}
	}
}


void BtnIMPBackgroundFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
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

			fButtonPendingFlag = TRUE;

			if( CameBackToBackgroundPageButNotFinished() )
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

BOOLEAN CameBackToBackgroundPageButNotFinished()
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

void AssignBackgroundHelpText( UINT16 ubNumber, MOUSE_REGION* pMouseregion )
{
	CHAR16	apStr[ 4500 ];
	CHAR16	atStr[  260 ];

	swprintf( apStr, L"" );
	
	if ( ubNumber )
	{
		swprintf(atStr, zBackground[ ubNumber ].szName );
		wcscat( apStr, atStr );
		wcscat( apStr, L"\n" );

		swprintf(atStr, zBackground[ ubNumber ].szDescription );
		wcscat( apStr, atStr );
		wcscat( apStr, L"\n" );

		// ability description
		for ( UINT8 i = 0; i < BACKGROUND_FLAG_MAX; ++i)
		{
			// some properties are hidden
			if ( ((UINT64)1 << i) & BACKGROUND_HIDDEN_FLAGS )
				continue;

			if ( zBackground[ ubNumber ].uiFlags & ((UINT64)1 << i) )
			{
				swprintf(atStr, szBackgroundText_Flags[ i ] );
				wcscat( apStr, atStr );
			}
		}

		UINT8 strused = 0;
		for ( UINT8 i = 0; i < BG_MAX; ++i)
		{
			if ( BG_DISLIKEBG == i && zBackground[ubNumber].value[i] )
			{
				swprintf( atStr, szBackgroundText_Value[strused] );
				wcscat( apStr, atStr );
			}
			else if ( BG_TRACKER_ABILITY == i && zBackground[ubNumber].value[i] )
			{
				swprintf( atStr, szBackgroundText_Value[strused], (UINT16)((gSkillTraitValues.usSVTrackerMaxRange * zBackground[ubNumber].value[i]) / 100) );
				wcscat( apStr, atStr );
			}
			else if ( BG_SMOKERTYPE == i )
			{
				if ( zBackground[ubNumber].value[i] == 1 )
					swprintf( atStr, szBackgroundText_Value[strused] );
				else
					swprintf( atStr, szBackgroundText_Value[strused + 1] );

				wcscat( apStr, L" " );
				wcscat( apStr, atStr );
				wcscat( apStr, L"\n" );

				// smoke has 2 texts, so extra increase of counter is needed
				++strused;
			}
			else if ( zBackground[ ubNumber ].value[i] )
			{
				swprintf( atStr, szBackgroundText_Value[strused], zBackground[ubNumber].value[i] > 0 ? L"+" : L"", zBackground[ubNumber].value[i] );
				wcscat( apStr, atStr );
			}

			++strused;
		}
	}
	else
		return;

	// Set region help text
	SetRegionFastHelpText( pMouseregion, apStr );
	SetRegionHelpEndCallback( pMouseregion, MSYS_NO_CALLBACK );

	return;
}


UINT16 displayedskills[IMP_BACKGROUND_DISPLAYED_CHOICES];
UINT16 gBackgroundFirstItem = 0;
UINT16 gBackgroundLastItem  = 0;

void UpdateDisplaySkills( BOOLEAN fIncrease )
{
	// fill a helper array with all viable choices
	UINT16 tmparray[NUM_BACKGROUND];
	UINT16 arraycounter = 0;
	UINT16 found = 0;
	for (UINT16 ubCnt = 0; ubCnt<num_found_background + 1; ++ubCnt )
	{
		if ( IsBackGroundAllowed(ubCnt) )
		{
			tmparray[ arraycounter++ ] = ubCnt;
			++found;
		}
	}

	UINT16 firstitem, lastitem;
	// select the range from which we rebuild our array
	if ( fIncrease )
	{
		// the last item of our new selection
		lastitem  = min(found , gBackgroundLastItem + IMP_BACKGROUND_DISPLAYED_CHOICES - 1);
		firstitem = max(0, lastitem - IMP_BACKGROUND_DISPLAYED_CHOICES + 1);
	}
	// select previous items
	else
	{
		// the first item of our new selection
		firstitem = max(0, gBackgroundFirstItem - IMP_BACKGROUND_DISPLAYED_CHOICES + 1);
		lastitem  = min(found , firstitem + IMP_BACKGROUND_DISPLAYED_CHOICES - 1);
	}

	UINT16 i = 0;
	for (UINT16 ubCnt = firstitem; ubCnt<lastitem; ++ubCnt )
	{
		displayedskills[ i++ ] = tmparray[ ubCnt ];
	}

	gBackgroundFirstItem = firstitem;
	gBackgroundLastItem = lastitem;
}

UINT16 GetDisplaySkill( UINT16 aNr )
{
	if ( aNr >= IMP_BACKGROUND_DISPLAYED_CHOICES )
		return 0;

	return displayedskills[ aNr ];
};

void ResetDisplaySkills()
{
	for (UINT16 ubCnt = 0; ubCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++ubCnt )
		displayedskills[ ubCnt ] = 0;

	gBackgroundFirstItem = 0;
	gBackgroundLastItem  = 0;

	UpdateDisplaySkills( TRUE );
};

extern INT32 SkillsList[ ATTITUDE_LIST_SIZE ];

BOOLEAN IsBackGroundAllowed( UINT16 ubNumber )
{
	if ( !ubNumber  )
		return FALSE;

	// some backgrounds are only allowed to specific genders. Set both to forbid a background from ever showing up in IMP creation (for merc-specific backgrounds)
	if ( fCharacterIsMale && zBackground[ ubNumber ].uiFlags & BACKGROUND_NO_MALE )
		return FALSE;
	else if ( !fCharacterIsMale && zBackground[ ubNumber ].uiFlags & BACKGROUND_NO_FEMALE )
		return FALSE;

	if ( SkillsList[0] == HEAVY_WEAPONS_NT || SkillsList[1] == HEAVY_WEAPONS_NT || SkillsList[2] == HEAVY_WEAPONS_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_ARTILLERY] > 0 )
			return FALSE;
	}

	if ( SkillsList[0] == SNIPER_NT || SkillsList[1] == SNIPER_NT || SkillsList[2] == SNIPER_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_CTH_MAX] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == SURVIVAL_NT || SkillsList[1] == SURVIVAL_NT || SkillsList[2] == SURVIVAL_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_CAMO] < 0 )
			return FALSE;
	}
	
	if ( SkillsList[0] == MARTIAL_ARTS_NT || SkillsList[1] == MARTIAL_ARTS_NT || SkillsList[2] == MARTIAL_ARTS_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_DAMAGE_MELEE] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == TECHNICIAN_NT || SkillsList[1] == TECHNICIAN_NT || SkillsList[2] == TECHNICIAN_NT )
	{
		if ( zBackground[ ubNumber ].value[MECHANICAL] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == DOCTOR_NT || SkillsList[1] == DOCTOR_NT || SkillsList[2] == DOCTOR_NT )
	{
		if ( zBackground[ ubNumber ].value[MEDICAL] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == MELEE_NT || SkillsList[1] == MELEE_NT || SkillsList[2] == MELEE_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_CTH_BLADE] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == STEALTHY_NT || SkillsList[1] == STEALTHY_NT || SkillsList[2] == STEALTHY_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_STEALTH] < 0 )
			return FALSE;
	}

	if ( SkillsList[0] == ATHLETICS_NT || SkillsList[1] == ATHLETICS_NT || SkillsList[2] == ATHLETICS_NT )
	{
		if ( zBackground[ ubNumber ].value[BG_PERC_SPEED_RUNNING] < 0 )
			return FALSE;
	}
		
	if ( SkillsList[0] == DEMOLITIONS_NT || SkillsList[1] == DEMOLITIONS_NT || SkillsList[2] == DEMOLITIONS_NT )
	{
		if ( zBackground[ ubNumber ].value[EXPLOSIVE_ASSIGN] < 0 )
			return FALSE;
	}

	switch ( iPersonality )
	{
	case HEAT_INTOLERANT:
		if ( zBackground[ ubNumber ].value[BG_DESERT] > 0 )
			return FALSE;
		break;
	case NERVOUS:
		break;
	case CLAUSTROPHOBIC:
		if ( zBackground[ ubNumber ].uiFlags & BACKGROUND_EXP_UNDERGROUND )
			return FALSE;
		break;
	case NONSWIMMER:
		if ( zBackground[ ubNumber ].value[BG_RIVER] > 0 || zBackground[ ubNumber ].value[BG_COASTAL] > 0 || zBackground[ ubNumber ].value[BG_SWIMMING] > 0 )
			return FALSE;
		break;
	case FEAR_OF_INSECTS:
		if ( zBackground[ ubNumber ].value[BG_SWAMP] > 0 || zBackground[ ubNumber ].value[BG_TROPICAL] > 0 )
			return FALSE;
		break;
	case FORGETFUL:
		if ( zBackground[ ubNumber ].value[LEADERSHIP] > 0 )
			return FALSE;
		break;
	case PSYCHO:
		if ( zBackground[ ubNumber ].value[LEADERSHIP] > 0 )
			return FALSE;
		break;
	default:
		break;
	}


	switch ( iAttitude )
	{
	case CHAR_TRAIT_SOCIABLE:
		if ( zBackground[ ubNumber ].uiFlags & BACKGROUND_XENOPHOBIC )
			return FALSE;
		break;
	case CHAR_TRAIT_LONER:
		if ( zBackground[ ubNumber ].value[LEADERSHIP] > 0 )
			return FALSE;
		break;
	case CHAR_TRAIT_OPTIMIST:
	case CHAR_TRAIT_ASSERTIVE:
	case CHAR_TRAIT_INTELLECTUAL:
	case CHAR_TRAIT_PRIMITIVE:
	case CHAR_TRAIT_AGGRESSIVE:
	case CHAR_TRAIT_PHLEGMATIC:
	case CHAR_TRAIT_DAUNTLESS:
	case CHAR_TRAIT_PACIFIST:
	case CHAR_TRAIT_MALICIOUS:
	case CHAR_TRAIT_SHOWOFF:
		break;
	default:
		break;
	}

	return TRUE;
}

void DestroyIMPBackgroundButtons( void )
{
	// the next button
	RemoveButton(giIMPBackgroundButton[ 0 ] );
	UnloadButtonImage(giIMPBackgroundButtonImage[ 0 ] );

	// the previous button
	RemoveButton(giIMPBackgroundButton[ 1 ] );
	UnloadButtonImage(giIMPBackgroundButtonImage[ 1 ] );
}

void BtnIMPBackgroundNextCallback(GUI_BUTTON *btn,INT32 reason)
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

			// next picture!!
			UpdateDisplaySkills( TRUE );

			gfIMPBackground_Redraw = TRUE;

			//reset all other buttons
			for( UINT16 uiCnt=0; uiCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++uiCnt )
			{
				gfBackgroundQuestions[ uiCnt ] = FALSE;
				ButtonList[ giIMPBackgroundAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}
			//Have the NONE trait initially selected
			gfBackgroundQuestions[ IMP_BACKGROUND_NONE ] = TRUE;
			ButtonList[ giIMPBackgroundAnswerButton[ IMP_BACKGROUND_NONE ] ]->uiFlags |= BUTTON_CLICKED_ON;

			//reset choice
			usBackground = 0;
		}
	}
}

void BtnIMPBackgroundPreviousCallback(GUI_BUTTON *btn,INT32 reason)
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

			// previous picture, please!!!
			UpdateDisplaySkills( FALSE );

			gfIMPBackground_Redraw = TRUE;

			//reset all other buttons
			for( UINT16 uiCnt=0; uiCnt<IMP_BACKGROUND_DISPLAYED_CHOICES; ++uiCnt )
			{
				gfBackgroundQuestions[ uiCnt ] = FALSE;
				ButtonList[ giIMPBackgroundAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}
			//Have the NONE trait initially selected
			gfBackgroundQuestions[ IMP_BACKGROUND_NONE ] = TRUE;
			ButtonList[ giIMPBackgroundAnswerButton[ IMP_BACKGROUND_NONE ] ]->uiFlags |= BUTTON_CLICKED_ON;

			//reset choice
			usBackground = 0;
		}
	}
}