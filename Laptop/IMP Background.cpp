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
	#include "IMP Disability Trait.h"
    #include "IMP Character Trait.h"
    #include "IMP Minor Trait.h"
	#include "GameSettings.h"
	#include "Interface.h"



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
	if( uiSkillPressed >= IMP_BACKGROUND_DISPLAYED_CHOICES )
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
	
	if (!ubNumber)
		return;

	swprintf(atStr, zBackground[ubNumber].szName);
	wcscat(apStr, atStr);
	wcscat(apStr, L"\n");

	swprintf(atStr, zBackground[ubNumber].szDescription);
	wcscat(apStr, atStr);
	wcscat(apStr, L"\n");

	// ability description
	if (gGameExternalOptions.fBackgroundTooltipDetails)
	{
		for (UINT8 i = 0; i < BACKGROUND_FLAG_MAX; ++i)
		{
			// some properties are hidden
			if (((UINT64)1 << i) & BACKGROUND_HIDDEN_FLAGS)
				continue;

			if (zBackground[ubNumber].uiFlags & ((UINT64)1 << i))
			{
				swprintf(atStr, szBackgroundText_Flags[i]);
				wcscat(apStr, atStr);
			}
		}

		UINT8 strused = 0;
		for (UINT8 i = 0; i < BG_MAX; ++i)
		{
			if (BG_DISLIKEBG == i && zBackground[ubNumber].value[i])
			{
				swprintf(atStr, szBackgroundText_Value[strused]);
				wcscat(apStr, atStr);
			}
			else if (BG_TRACKER_ABILITY == i && zBackground[ubNumber].value[i])
			{
				swprintf(atStr, szBackgroundText_Value[strused], (UINT16)((gSkillTraitValues.usSVTrackerMaxRange * zBackground[ubNumber].value[i]) / 100));
				wcscat(apStr, atStr);
			}
			else if (BG_SMOKERTYPE == i)
			{
				if (zBackground[ubNumber].value[i] == 1)
					swprintf(atStr, szBackgroundText_Value[strused]);
				else
					swprintf(atStr, szBackgroundText_Value[strused + 1]);

				wcscat(apStr, L" ");
				wcscat(apStr, atStr);
				wcscat(apStr, L"\n");

				// smoke has 2 texts, so extra increase of counter is needed
				++strused;
			}
			else if (zBackground[ubNumber].value[i])
			{
				swprintf(atStr, szBackgroundText_Value[strused], zBackground[ubNumber].value[i] > 0 ? L"+" : L"", zBackground[ubNumber].value[i]);
				wcscat(apStr, atStr);
			}

			++strused;
		}
	}

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

extern BOOLEAN	gfSkillTraitQuestions[20];
extern BOOLEAN	gfSkillTraitQuestions2[20];

extern BOOLEAN	gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS];

BOOLEAN IsBackGroundAllowed(UINT16 ubNumber)
{
	if (!ubNumber)
		return FALSE;

	// some backgrounds are only allowed to specific genders. Set both to forbid a background from ever showing up in IMP creation (for merc-specific backgrounds)
	if (fCharacterIsMale && zBackground[ubNumber].uiFlags & BACKGROUND_NO_MALE)
		return FALSE;
	else if (!fCharacterIsMale && zBackground[ubNumber].uiFlags & BACKGROUND_NO_FEMALE)
		return FALSE;

	// added new ini-options and accompanying background-tag
	// choices in skills/character-traits/disabilities determine available backgrounds if true in ja2options.ini
	// new tag <alt_impcreation> has been added to backgrounds.xml

	if (!gGameExternalOptions.fAltIMPCreation)

	{
		if (zBackground[ubNumber].uiFlags & BACKGROUND_ALT_IMP_CREATION) // don't show BG with tag <alt_impcreation> if Alt_Imp_Creation isn't true in ja2options.ini
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	// show BG with tag <alt_impcreation>, 
	// but don't show BG whose tags would contradict with a main bonus/penalty gained by skill/char-trait/disability

	if (gGameExternalOptions.fAltIMPCreation)

    // define which tags are considered contradicting for major traits, minor traits, disablities and character traits

	// major traits  (single-trait and dual-trait)
	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS])
        {    
			if (zBackground[ubNumber].value[BG_ARTILLERY] > 0) //dual trait (expert) 
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_ARTILLERY] > 10) //single trait
				return FALSE;
		}
	}

	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER])
		{
			if (zBackground[ubNumber].value[BG_PERC_CTH_MAX] < 0 || zBackground[ubNumber].value[BG_MARKSMANSHIP] < 0 )
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_PERC_CTH_MAX] < 0 )  
				return FALSE;
		}
	}

	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS])
		{
			
				if (zBackground[ubNumber].value[BG_RESI_PHYSICAL] < 0 )  
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_RESI_PHYSICAL] < 0 )  
				return FALSE;
		}
	}

	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN])
		{
			if (zBackground[ubNumber].value[BG_MECHANICAL] < 0)  
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_MECHANICAL] < 0)  
				return FALSE;
		}
	}

	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR])
		{
			if (zBackground[ubNumber].value[BG_PERC_BANDAGING] < 0 || zBackground[ubNumber].value[BG_MEDICAL] < 0 )
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_PERC_BANDAGING] < 0)  
				return FALSE;
		}
	}

	if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER])
	{

		if (gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER])
		{
			if (zBackground[ubNumber].value[BG_RESI_SUPPRESSION] < 0 || zBackground[ubNumber].value[BG_RESI_FEAR] < 0 )
				return FALSE;
		}
		else
		{
			if (zBackground[ubNumber].value[BG_RESI_SUPPRESSION] < 0)
				return FALSE;
		}
	}

	// Minor Traits (single trait only) 
	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_MELEE])
	{
		if (zBackground[ubNumber].value[BG_PERC_CTH_BLADE] < 0 || zBackground[ubNumber].value[BG_PERC_DAMAGE_MELEE] < 0 )
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_STEALTHY])
	{
		if (zBackground[ubNumber].value[BG_PERC_STEALTH] < 0)
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_DEMOLITIONS])  
	{
		if (zBackground[ubNumber].value[BG_BONUS_BREACHINGCHARGE] < 0 || zBackground[ubNumber].value[BG_EXPLOSIVE_ASSIGN] < 0 )
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_SURVIVAL]) 
	{
		if (zBackground[ubNumber].value[BG_TRAVEL_FOOT] < 0 || zBackground[ubNumber].value[BG_TRAVEL_CAR] <0 || zBackground[ubNumber].value[BG_TRAVEL_AIR] <0 ||
			zBackground[ubNumber].value[BG_RESI_DISEASE] < 0  || zBackground[ubNumber].value[BG_SNAKEDEFENSE] < 0 )
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_BODYBUILDING])
	{
		if (zBackground[ubNumber].value[BG_PERC_CARRYSTRENGTH] < 0)
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS])
	{
		if (zBackground[ubNumber].value[BG_INVENTORY] > 0)
			return FALSE;
	}

	if (gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_NIGHT_OPS])
	{
		if (zBackground[ubNumber].value[BG_PERC_SLEEP] > 0 || zBackground[ubNumber].value[BG_PERC_HEARING_NIGHT] < 0 )
			return FALSE;
	}

	// Disabiliies
	switch (iChosenDisabilityTrait())
	{
	case HEAT_INTOLERANT:
		if (zBackground[ubNumber].value[BG_DESERT] > 0 || zBackground[ubNumber].value[BG_TROPICAL] > 0)  
			return FALSE;
		break;
	case NERVOUS:
		break;
	case CLAUSTROPHOBIC:
		if ( zBackground[ ubNumber ].uiFlags & BACKGROUND_EXP_UNDERGROUND )
			return FALSE;
		break;
	case NONSWIMMER:
		if ( zBackground[ ubNumber ].value[BG_RIVER] > 0 || zBackground[ ubNumber ].value[BG_COASTAL] > 0 )
			return FALSE;
		break;
	case FEAR_OF_INSECTS:
		if ( zBackground[ ubNumber ].value[BG_SWAMP] > 0 || zBackground[ ubNumber ].value[BG_TROPICAL] > 0 )
			return FALSE;
		break;
	case FORGETFUL:
		if (zBackground[ubNumber].value[BG_INVENTORY] < 0 || zBackground[ubNumber].value[BG_ASSAULT] < 0 )
			return FALSE;
		break;
	case PSYCHO:
		if (zBackground[ubNumber].value[BG_LEADERSHIP] > 5)
			return FALSE;
		break;
	case DEAF:
		if (zBackground[ ubNumber ].value[BG_PERC_HEARING_NIGHT] > 0 || zBackground[ ubNumber ].value[BG_PERC_HEARING_DAY] > 0)
			return FALSE;
		break;
	case SHORTSIGHTED:
		if (zBackground[ ubNumber ].value[BG_PERC_CTH_MAX] > 0 || zBackground[ubNumber].value[BG_PERC_SPOTTER] > 0 )
			return FALSE;
		break;
	case HEMOPHILIAC:
		if (zBackground[ubNumber].value[BG_RESI_DISEASE] > 0)
			return FALSE;
		break;
	case AFRAID_OF_HEIGHTS:
		if ( zBackground[ ubNumber ].value[BG_HEIGHT] > 0 || zBackground[ubNumber].value[BG_AIRDROP] > 0 )
			return FALSE;
		break;
	case SELF_HARM:
		if (zBackground[ubNumber].value[BG_RESI_DISEASE] > 0)
			return FALSE;
		break;
	default:
		break;
	}

	// Character Traits
	switch (iChosenCharacterTrait())
	{
	case CHAR_TRAIT_SOCIABLE:
		if (zBackground[ubNumber].uiFlags & BACKGROUND_XENOPHOBIC || zBackground[ubNumber].value[BG_PERC_SPOTTER] < 0)
			return FALSE;
		break;
	case CHAR_TRAIT_LONER:
		if (zBackground[ubNumber].value[BG_LEADERSHIP] > 0 || zBackground[ubNumber].value[BG_PERC_SPOTTER] > 0)
			return FALSE;
		break;
	case CHAR_TRAIT_OPTIMIST:
		if (zBackground[ubNumber].uiFlags & BACKGROUND_TRAPLEVEL)
			return FALSE;
		break;
	case CHAR_TRAIT_ASSERTIVE:
		if (zBackground[ubNumber].value[BG_PERC_INTERROGATION] < 0 || zBackground[ubNumber].value[BG_PERC_APPROACH_THREATEN] < 0)
			return FALSE;
		break;
	case CHAR_TRAIT_INTELLECTUAL:
		if (zBackground[ubNumber].value[BG_ADMINISTRATION_ASSIGNMENT] < 0)
			return FALSE;
		break;
	case CHAR_TRAIT_PRIMITIVE:
		if (zBackground[ubNumber].value[BG_ADMINISTRATION_ASSIGNMENT] > 0)
			return FALSE;
		break;
	case CHAR_TRAIT_AGGRESSIVE:
		if (zBackground[ubNumber].uiFlags & BACKGROUND_TRAPLEVEL || zBackground[ubNumber].value[BG_PERC_DISARM] > 0 )
			return FALSE;
		break;
	case CHAR_TRAIT_PHLEGMATIC:
		if (zBackground[ubNumber].value[BG_ASSAULT] > 0 )
			return FALSE;
		break;
	case CHAR_TRAIT_DAUNTLESS:
		if (zBackground[ubNumber].value[BG_CROUCHEDDEFENSE] < 0 )
			return FALSE;
		break;
	case CHAR_TRAIT_PACIFIST:
		break;
	case CHAR_TRAIT_MALICIOUS:
		if (zBackground[ubNumber].value[BG_PERC_APPROACH_FRIENDLY] > 0)
			return FALSE;
		break;
	case CHAR_TRAIT_SHOWOFF:
		break;
	case CHAR_TRAIT_COWARD:
		if (zBackground[ubNumber].value[BG_PERC_CAPITULATION] > 0)
			return FALSE;
		break;
	default:
		break;
	}

	// show exclusivly the BG's with tag <alt_impcreation> if Reduced_Imp_Creation is true in ja2options.ini
	// requires Alt_Imp_Creation to be true in ja2options.ini (otherwise BG's with tag <alt_impcreation> won't be shown)

	if (gGameExternalOptions.fReducedIMPCreation)

	{

		if (zBackground[ubNumber].uiFlags & BACKGROUND_ALT_IMP_CREATION)   
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
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
