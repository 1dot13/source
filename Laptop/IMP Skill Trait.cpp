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
	#include "GameSettings.h" // added by SANDRO
	#include "IMP Color Choosing.h" // added by SANDRO
	#include "IMP Minor Trait.h"
	#include "Soldier Profile.h"
	#include "personnel.h"		// added by Flugente



//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_SKILL_TRAIT__TITLE_FONT				FONT14ARIAL

#define			IMP_SKILL_TRAIT__FONT							FONT12ARIAL
#define			IMP_SKILL_TRAIT__COLOR						FONT_MCOLOR_WHITE


#define	IMP_SKILL_TRAIT__LEFT_COLUMN_START_X								( LAPTOP_SCREEN_UL_X + 15 )
#define	IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y								( LAPTOP_SCREEN_WEB_UL_Y + 40 )

#define	IMP_SKILL_TRAIT__RIGHT_COLUMN_START_X								( IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + 241 )
#define	IMP_SKILL_TRAIT__RIGHT_COLUMN_START_Y								IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y

#define	IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS									38

//#define	IMP_SKILL_TRAIT__SKILL_TRAIT_TO_START_RIGHT_COL			6

#define	IMP_SKILL_TRAIT__NONE_BTN_LOC_X											( LAPTOP_SCREEN_UL_X + 136 )
#define	IMP_SKILL_TRAIT__NONE_BTN_LOC_Y											( LAPTOP_SCREEN_WEB_UL_Y + 306 ) // added this one - SANDRO

#define	IMP_SKILL_TRAIT__TEXT_OFFSET_X											65
#define	IMP_SKILL_TRAIT__TEXT_OFFSET_Y											12

#define	IMP_SKILL_TRAIT__TITLE_X											LAPTOP_SCREEN_UL_X - 111

#define	IMP_SKILL_TRAIT__GREY_BOX_OFFSET_X									5
#define	IMP_SKILL_TRAIT__GREY_BOX_OFFSET_Y									7

#define NEWTRAIT_EXPERT_OFFSET												12	// Flugente: offset between normal and expert trait strings

// Flugente: Due to the unfortunate nature of our enums, they do not match the trait numbers. We thus have to remap them when obtaining their description
UINT8 gusNewMajorTraitRemap[IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS] =
{
	AUTO_WEAPONS_NT,	// IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS
	HEAVY_WEAPONS_NT,	// IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS
	SNIPER_NT,			// IMP_SKILL_TRAITS_NEW_PROF_SNIPER
	RANGER_NT,			// IMP_SKILL_TRAITS_NEW_RANGER
	GUNSLINGER_NT,		// IMP_SKILL_TRAITS_NEW_GUNSLINGER
	MARTIAL_ARTS_NT,	// IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS
	SQUADLEADER_NT,		// IMP_SKILL_TRAITS_NEW_SQUADLEADER
	TECHNICIAN_NT,		// IMP_SKILL_TRAITS_NEW_TECHNICIAN
	DOCTOR_NT,			// IMP_SKILL_TRAITS_NEW_DOCTOR
	COVERT_NT,			// IMP_SKILL_TRAITS_NEW_COVERT
	NO_SKILLTRAIT_NT,	// IMP_SKILL_TRAITS_NEW_MAJOR_NONE
};

//shadooow: same as above but for old skill traits
UINT8 gusOldMajorTraitRemap[IMP_SKILL_TRAITS__NUMBER_SKILLS] =
{
	LOCKPICKING_OT,
	HANDTOHAND_OT,
	ELECTRONICS_OT,
	NIGHTOPS_OT,
	THROWING_OT,
	TEACHING_OT,
	HEAVY_WEAPS_OT,
	AUTO_WEAPS_OT,
	STEALTHY_OT,
	AMBIDEXT_OT,
	KNIFING_OT,
	PROF_SNIPER_OT,
	CAMOUFLAGED_OT,
	MARTIALARTS_OT,
	NO_SKILLTRAIT_OT,
};

//*******************************************************************
//
// Global Variables
//
//*******************************************************************
BOOLEAN	gfSkillTraitQuestions[ 20 ];
BOOLEAN	gfSkillTraitQuestions2[ 20 ];


BOOLEAN gfIST_Redraw=FALSE;

// these are the buttons for the questions
INT32 giIMPSkillTraitAnswerButton[ 20 ];
INT32 giIMPSkillTraitAnswerButton2[ 20 ];
INT32 giIMPSkillTraitAnswerButtonImage[ 20 ];
INT32 giIMPSkillTraitAnswerButtonImage2[ 20 ];

// this is the Done	buttons
INT32 giIMPSkillTraitFinsihButton;
INT32 giIMPSkillTraitFinsihButtonImage;

//BOOLEAN	gfSkillTraitButtonChanged=FALSE;

INT8	gbLastSelectedTraits[ 10 ];

//image handle
UINT32	guiIST_GreyGoldBox;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPSkillTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPSkillTraitAnswerCallback2(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPSkillTraitFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddImpSkillTraitButtons();
void		HandleSkillTraitButtonStates( );
void		HandleIMPSkillTraitAnswers( UINT32 uiSkillPressed, BOOLEAN fSecondTrait );
void		IMPSkillTraitDisplaySkills();
BOOLEAN		ShouldTraitBeSkipped( UINT32 uiTrait );
//void		AddSelectedSkillsToSkillsList(); // SANDRO - second declaration not needed
void		HandleLastSelectedTraits( INT8 bNewTrait );
INT8		GetLastSelectedSkill( void );
BOOLEAN		CameBackToSpecialtiesPageButNotFinished();

MOUSE_REGION	gMR_SkillTraitHelpTextRegions[IMP_SKILL_TRAITS__NUMBER_SKILLS];
//ppp

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPSkillTrait( void )
{
//	UINT32 uiCnt;
	VOBJECT_DESC	VObjectDesc;

	//add the skill trait buttons
	AddImpSkillTraitButtons();


	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIST_GreyGoldBox ) )
	{
		Assert( 0 );
		return;
	}
	
	giIMPSkillTraitFinsihButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPSkillTraitFinsihButton = CreateIconAndTextButton( giIMPSkillTraitFinsihButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPSkillTraitFinishCallback );
	
	SetButtonCursor( giIMPSkillTraitFinsihButton, CURSOR_WWW);

	//if we are not DONE and are just reviewing
	if( iCurrentProfileMode != IMP__FINISH )
	{
		//Have the NONE trait initially selected
		if( gGameOptions.fNewTraitSystem )
		{
			gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_MAJOR_NONE ] = TRUE;
			gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MAJOR_NONE ] = TRUE;
		}
		else
		{
			gfSkillTraitQuestions[ IMP_SKILL_TRAITS__NONE ] = TRUE;
			gfSkillTraitQuestions2[ IMP_SKILL_TRAITS__NONE ] = TRUE;
		}
	}

	HandleSkillTraitButtonStates( );

	// add regions for help texts
	UINT16 usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + 62;
	UINT16 usPosY = IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y + 8;
	for( UINT32 uiCnt=0; uiCnt < (UINT8)( gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS ); ++uiCnt )
	{
		if( ShouldTraitBeSkipped( uiCnt ) )
		{
			//if its the none button
			if( uiCnt == ( gGameOptions.fNewTraitSystem ? (IMP_SKILL_TRAITS_NEW_MAJOR_NONE-1) : (IMP_SKILL_TRAITS__NONE-1) ) )
			{
				usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + 62;
				usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y + 8;
			}
			continue;
		}
		MSYS_DefineRegion( &gMR_SkillTraitHelpTextRegions[uiCnt], ( usPosX ), ( usPosY ),
						(usPosX + 156), ( usPosY + 17), MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
		MSYS_AddRegion( &gMR_SkillTraitHelpTextRegions[uiCnt] );
		
		//Determine the next x location
		if( uiCnt < (UINT8)(gGameOptions.fNewTraitSystem ? 4 : 6) )
			usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + 62;
		else
			usPosX = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_X + 62;

		//Determine the next Y location
		if( uiCnt == (gGameOptions.fNewTraitSystem ? 4 : 6) )
			usPosY = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_Y + 8;
		else
		{
			if( gGameOptions.fNewTraitSystem )
				usPosY += IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS + 5;
			else
				usPosY += IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS;
		}
		//if its the none button
		if( uiCnt == (gGameOptions.fNewTraitSystem ? (IMP_SKILL_TRAITS_NEW_MAJOR_NONE-1) : (IMP_SKILL_TRAITS__NONE-1) ) )
		{
			usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + 62;
			usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y + 8;
		}
	}

	//reset last selecterd trait
	memset( gbLastSelectedTraits, -1, 10 );
}


void RenderIMPSkillTrait( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	IMPSkillTraitDisplaySkills();
}


void ExitIMPSkillTrait( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIST_GreyGoldBox );

	//remove the skill buttons
	for(iCnt = 0; iCnt < ( gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS ); iCnt++)
	{
		//if there is a button allocated
		if( giIMPSkillTraitAnswerButton[iCnt] != -1 )
		{
			RemoveButton(giIMPSkillTraitAnswerButton[ iCnt ] );
			UnloadButtonImage(giIMPSkillTraitAnswerButtonImage[ iCnt ] );
		}
	}

	//remove the skill buttons
	for(iCnt = 0; iCnt < ( gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS ); iCnt++)
	{
		//if there is a button allocated
		if( giIMPSkillTraitAnswerButton2[iCnt] != -1 )
		{
			RemoveButton(giIMPSkillTraitAnswerButton2[ iCnt ] );
			UnloadButtonImage(giIMPSkillTraitAnswerButtonImage2[ iCnt ] );
		}
	}

	RemoveButton( giIMPSkillTraitFinsihButton );
	UnloadButtonImage( giIMPSkillTraitFinsihButtonImage );

	// SANDRO - remove regions
	for( iCnt=0; iCnt < ( gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS); iCnt++ )
	{
		if( ShouldTraitBeSkipped( iCnt ) )
			continue;

		MSYS_RemoveRegion( &gMR_SkillTraitHelpTextRegions[iCnt] );
	}
}


void HandleIMPSkillTrait( void )
{
	if( gfIST_Redraw )
	{
		RenderIMPSkillTrait( );
		gfIST_Redraw = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddImpSkillTraitButtons()
{
	INT32 iCnt = 0;
	INT32 iCntMax = 0;
	INT8 iNumSkillsToStartRightColumn = 0;
	UINT16 usPosX, usPosY, usSpaceBetweenButtons;

	if ( gGameOptions.fNewTraitSystem )
	{
		iCntMax = IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS;
		iNumSkillsToStartRightColumn = 4;
		usSpaceBetweenButtons = IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS + 5;
	}
	else
	{
		iCntMax = IMP_SKILL_TRAITS__NUMBER_SKILLS;
		iNumSkillsToStartRightColumn = 6;
		usSpaceBetweenButtons = IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// FIRST COLUMN OF BUTTONS

	usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X;
	usPosY = IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y;

	for(iCnt = 0; iCnt < iCntMax; ++iCnt)
	{
		//reset
		giIMPSkillTraitAnswerButton[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfSkillTraitQuestions[ iCnt ] = FALSE;
		}

		//if the merc is a FEMALE, skip this skill cause there isnt any fenmal martial artists
		if( ShouldTraitBeSkipped( iCnt ) )
		{
			//if its the none button
			if( iCnt == iCntMax-2 )
			{
				usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X;
				usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y;
			}
			continue;
		}

		if( iCnt == 0 )
			giIMPSkillTraitAnswerButtonImage[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_9a.sti", -1,0,-1,1,-1 );
		else
			giIMPSkillTraitAnswerButtonImage[ iCnt ] =	UseLoadedButtonImage( giIMPSkillTraitAnswerButtonImage[ 0 ], -1,0,-1,1,-1 );

		giIMPSkillTraitAnswerButton[iCnt] = QuickCreateButton( giIMPSkillTraitAnswerButtonImage[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPSkillTraitAnswerCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPSkillTraitAnswerButton[iCnt], 0, iCnt );
		SetButtonCursor( giIMPSkillTraitAnswerButton[iCnt], CURSOR_WWW);

		//Get rid of playing the button sound, it will be handled here
		//ButtonList[ giIMPSkillTraitAnswerButton[ iCnt ] ]->ubSoundSchemeID = 0; // - This would disable the sounds totally resulted to play no sound - SANDRO

		//Determine the next x location
		if( iCnt < iNumSkillsToStartRightColumn )
			usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X;
		else
			usPosX = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_X;

		//Determine the next Y location
		if( iCnt == iNumSkillsToStartRightColumn )
			usPosY = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_Y;
		else
			usPosY += usSpaceBetweenButtons;

		//if its the none button
		if( iCnt == iCntMax-2 )
		{
			usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X;
			usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y;
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// SECOND COLUMN OF BUTTONS

	usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + 26;
	usPosY = IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y;

	for(iCnt = 0; iCnt < iCntMax; ++iCnt)
	{
		//reset
		giIMPSkillTraitAnswerButton2[iCnt] = -1;

		//if we are not DONE and are just reviewing
		if( iCurrentProfileMode != IMP__FINISH )
		{
			gfSkillTraitQuestions2[ iCnt ] = FALSE;
		}

		//if the merc is a FEMALE, skip this skill cause there isnt any fenmal martial artists
		if( ShouldTraitBeSkipped( iCnt ) )
		{
			//if its the none button
			if( iCnt == iCntMax-2 )
			{
				usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + 26;
				usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y;
			}
			continue;
		}

		if( iCnt == 0 )
			giIMPSkillTraitAnswerButtonImage2[ iCnt ] =	LoadButtonImage( "LAPTOP\\button_9b.sti", -1,0,-1,1,-1 );
		else
			giIMPSkillTraitAnswerButtonImage2[ iCnt ] =	UseLoadedButtonImage( giIMPSkillTraitAnswerButtonImage2[ 0 ], -1,0,-1,1,-1 );

		giIMPSkillTraitAnswerButton2[iCnt] = QuickCreateButton( giIMPSkillTraitAnswerButtonImage2[ iCnt ], usPosX, usPosY,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPSkillTraitAnswerCallback2);

		//Set the button data
		MSYS_SetBtnUserData( giIMPSkillTraitAnswerButton2[iCnt], 0, iCnt );
		SetButtonCursor( giIMPSkillTraitAnswerButton2[iCnt], CURSOR_WWW);

		//Get rid of playing the button sound, it will be handled here
		//ButtonList[ giIMPSkillTraitAnswerButton[ iCnt ] ]->ubSoundSchemeID = 0; // - This would disable the sounds totally resulted to play no sound - SANDRO

		//Determine the next x location
		if( iCnt < iNumSkillsToStartRightColumn )
			usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + 26;
		else
			usPosX = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_X + 26;

		//Determine the next Y location
		if( iCnt == iNumSkillsToStartRightColumn )
			usPosY = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_Y;
		else
			usPosY += usSpaceBetweenButtons;

		//if its the none button
		if( iCnt == iCntMax-2 )
		{
			usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + 26;
			usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y;
		}
	}
}


void BtnIMPSkillTraitAnswerCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags|=(BUTTON_CLICKED_ON);

		INT32 iSkillTrait =	MSYS_GetBtnUserData( btn, 0 );

		HandleIMPSkillTraitAnswers( iSkillTrait, FALSE );
	}
}

void BtnIMPSkillTraitAnswerCallback2(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags|=(BUTTON_CLICKED_ON);

		INT32 iSkillTrait =	MSYS_GetBtnUserData( btn, 0 );

		HandleIMPSkillTraitAnswers( iSkillTrait, TRUE );
	}
}

void HandleIMPSkillTraitAnswers( UINT32 uiSkillPressed, BOOLEAN fSecondTrait )
{
	UINT32 uiCnt, uiCntMax;

	if ( gGameOptions.fNewTraitSystem )
		uiCntMax = IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS;
	else
		uiCntMax = IMP_SKILL_TRAITS__NUMBER_SKILLS;

	//if we are DONE and are just reviewing
	if( iCurrentProfileMode == IMP__FINISH )
	{
		return;
	}

	//make sure its a valid skill
	if( uiSkillPressed > uiCntMax )
	{
		Assert( 0 );
		return;
	}

	if( !fSecondTrait )
	{
		//if its allready set
		if( gfSkillTraitQuestions[ uiSkillPressed ] )
		{
			gfSkillTraitQuestions[ uiSkillPressed ] = FALSE;

			BOOLEAN fNoSkillSelected = TRUE;
			for ( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
			{
				if ( gfSkillTraitQuestions[ uiCnt ] == TRUE )
					fNoSkillSelected = FALSE;
			}
			if ( fNoSkillSelected )
			{
				// select NONE button
				gfSkillTraitQuestions[ uiCntMax-1 ] = TRUE;
			}
		}
		// if cannot have expert level of skill, don't continue
		else if ( gfSkillTraitQuestions2[uiSkillPressed] && !TwoStagedTrait( gGameOptions.fNewTraitSystem ? gusNewMajorTraitRemap[uiSkillPressed] : uiSkillPressed ) )
		{
			//dont need to do anything
			return;
		}
		else
		{
			// if the other trait has not been set yet, we rather set it now, 
			// instead of deselecting the already set one
			if( gfSkillTraitQuestions2[ uiCntMax-1 ] == TRUE && gfSkillTraitQuestions[ uiCntMax-1 ] == FALSE && (uiSkillPressed != (uiCntMax-1)) )
			{
				// Unselect NONE button
				gfSkillTraitQuestions2[ uiCntMax-1 ] = FALSE;
				//Set the skill
				gfSkillTraitQuestions2[ uiSkillPressed ] = TRUE;
			}
			else
			{
				if ( CountNumSkillTraitsSelected( FALSE ) < gSkillTraitValues.ubNumberOfMajorTraitsAllowedForIMP && CountNumSkillTraitsSelected( FALSE ) < gSkillTraitValues.ubMaxNumberOfTraitsForIMP )
				{
					// we can select next one, simply do so
					gfSkillTraitQuestions[ uiSkillPressed ] = TRUE;
					// Unselect NONE button if necessary
					gfSkillTraitQuestions[ uiCntMax-1 ] = FALSE;

					HandleLastSelectedTraits( (INT8)uiSkillPressed );
				}
				else
				{
					if ( gSkillTraitValues.ubNumberOfMajorTraitsAllowedForIMP == 2 )
					{
						//loop through all the skill and reset them
						for( uiCnt=0; uiCnt<uiCntMax; ++uiCnt )
						{
							gfSkillTraitQuestions[ uiCnt ] = FALSE;
						}
					}
					else
					{
						// we have to deselect last selected skill
						if ( gfSkillTraitQuestions[ GetLastSelectedSkill() ] || gfSkillTraitQuestions2[ GetLastSelectedSkill() ] )
						{
							if ( gfSkillTraitQuestions[ GetLastSelectedSkill() ] )
							{
								gfSkillTraitQuestions[ GetLastSelectedSkill() ] = FALSE;
							}
							else 
							{
								gfSkillTraitQuestions2[ GetLastSelectedSkill() ] = FALSE;
							}
							HandleLastSelectedTraits( -1 );
						}
						else
						{
							// something gone wrong, deselect the first one we can find
							for( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
							{
								if ( gfSkillTraitQuestions[ uiCnt ] == TRUE )
								{
									gfSkillTraitQuestions[ uiCnt ] = FALSE;
									break;
								}
								if ( uiCnt >= (uiCntMax-2) )
								{
									for( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
									{
										if ( gfSkillTraitQuestions2[ uiCnt ] == TRUE )
										{
											gfSkillTraitQuestions2[ uiCnt ] = FALSE;
											break;
										}
									}
								}
							}
						}
					}
					//Set the skill
					gfSkillTraitQuestions[ uiSkillPressed ] = TRUE;

					HandleLastSelectedTraits( (INT8)uiSkillPressed );
				}
			}
		}
		
		//Play the button sound
		if( gfSkillTraitQuestions[ uiSkillPressed ] )
		{
			PlayButtonSound( giIMPSkillTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
		}
		else
		{
			PlayButtonSound( giIMPSkillTraitAnswerButton[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
		}
	}
	else
	{
		//if its allready set
		if( gfSkillTraitQuestions2[ uiSkillPressed ] )
		{
			gfSkillTraitQuestions2[ uiSkillPressed ] = FALSE;

			BOOLEAN fNoSkillSelected = TRUE;
			for ( uiCnt=0; uiCnt<(uiCntMax-1); ++uiCnt )
			{
				if ( gfSkillTraitQuestions2[ uiCnt ] == TRUE )
					fNoSkillSelected = FALSE;
			}
			if ( fNoSkillSelected )
			{
				// select NONE button
				gfSkillTraitQuestions2[ uiCntMax-1 ] = TRUE;
			}
		}
		// if cannot have expert level of skill, don't continue
		else if ( gfSkillTraitQuestions[uiSkillPressed] && !TwoStagedTrait( gGameOptions.fNewTraitSystem ? gusNewMajorTraitRemap[uiSkillPressed] : uiSkillPressed ) )
		{
			//dont need to do anything
			return;
		}
		else
		{
			// if the other trait has not been set yet, we rather set it now, 
			// instead of deselecting the already set one
			if( gfSkillTraitQuestions[ uiCntMax-1 ] == TRUE && gfSkillTraitQuestions2[ uiCntMax-1 ] == FALSE && (uiSkillPressed != (uiCntMax-1)) )
			{
				// Unselect NONE button
				gfSkillTraitQuestions[ uiCntMax-1 ] = FALSE;
				//Set the skill
				gfSkillTraitQuestions[ uiSkillPressed ] = TRUE;
			}
			else
			{
				if ( CountNumSkillTraitsSelected( FALSE ) < gSkillTraitValues.ubNumberOfMajorTraitsAllowedForIMP && CountNumSkillTraitsSelected( FALSE ) < gSkillTraitValues.ubMaxNumberOfTraitsForIMP )
				{
					// we can select next one, simply do so
					gfSkillTraitQuestions2[ uiSkillPressed ] = TRUE;
					// Unselect NONE button if necessary
					gfSkillTraitQuestions2[ uiCntMax-1 ] = FALSE;

					HandleLastSelectedTraits( (INT8)uiSkillPressed );
				}
				else
				{
					if ( gSkillTraitValues.ubNumberOfMajorTraitsAllowedForIMP == 2 )
					{
						//loop through all the skill and reset them
						for( uiCnt=0; uiCnt<uiCntMax; uiCnt++ )
						{
							gfSkillTraitQuestions2[ uiCnt ] = FALSE;
						}
					}
					else
					{
						// we have to deselect last selected skill
						if ( gfSkillTraitQuestions[ GetLastSelectedSkill() ] || gfSkillTraitQuestions2[ GetLastSelectedSkill() ] )
						{
							if ( gfSkillTraitQuestions2[ GetLastSelectedSkill() ] )
							{
								gfSkillTraitQuestions2[ GetLastSelectedSkill() ] = FALSE;
							}
							else 
							{
								gfSkillTraitQuestions[ GetLastSelectedSkill() ] = FALSE;
							}
							HandleLastSelectedTraits( -1 );
						}
						else
						{
							// something gone wrong, deselect the first one we can find
							for( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
							{
								if ( gfSkillTraitQuestions2[ uiCnt ] == TRUE )
								{
									gfSkillTraitQuestions2[ uiCnt ] = FALSE;
									break;
								}
								if ( uiCnt >= (uiCntMax-2) )
								{
									for( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
									{
										if ( gfSkillTraitQuestions[ uiCnt ] == TRUE )
										{
											gfSkillTraitQuestions[ uiCnt ] = FALSE;
											break;
										}
									}
								}
							}
						}
					}
					//Set the skill
					gfSkillTraitQuestions2[ uiSkillPressed ] = TRUE;

					HandleLastSelectedTraits( (INT8)uiSkillPressed );
				}
			}
		}
		
		//Play the button sound
		if( gfSkillTraitQuestions2[ uiSkillPressed ] )
		{
			PlayButtonSound( giIMPSkillTraitAnswerButton2[ uiSkillPressed ], BUTTON_SOUND_CLICKED_ON );
		}
		else
		{
			PlayButtonSound( giIMPSkillTraitAnswerButton2[ uiSkillPressed ], BUTTON_SOUND_CLICKED_OFF );
		}
	}

	//update buttons
	HandleSkillTraitButtonStates( );

	//redraw the screen
	gfIST_Redraw = TRUE;
}

INT8	CountNumSkillTraitsSelected( BOOLEAN fIncludeNoneSkill )
{
	UINT32	uiCnt, uiCntMax;
	INT8	iNumberSkills=0;

	if ( gGameOptions.fNewTraitSystem )
		uiCntMax = IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS;
	else
		uiCntMax = IMP_SKILL_TRAITS__NUMBER_SKILLS;

	for( uiCnt=0; uiCnt < uiCntMax; ++uiCnt )
	{
		if( !fIncludeNoneSkill && (uiCnt == (uiCntMax-1)) )
			continue;

		//if the skill is selected ( ie depressed )
		if( gfSkillTraitQuestions[ uiCnt ] )
		{
			++iNumberSkills;
		}
		if( gfSkillTraitQuestions2[ uiCnt ] )
		{
			++iNumberSkills;
		} 
	}

	return( iNumberSkills );
}


void HandleSkillTraitButtonStates( )
{
	for ( INT32 uiCnt = 0; uiCnt<(gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS); ++uiCnt )
	{
			//if the merc is a FEMALE, skip this skill cause there isnt any fenmal martial artists
		if( ShouldTraitBeSkipped( uiCnt ) )
		{
			continue;
		}

		//if the skill is selected ( ie depressed )
		if( gfSkillTraitQuestions[ uiCnt ] )
		{
			ButtonList[ giIMPSkillTraitAnswerButton[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPSkillTraitAnswerButton[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}

		if( gfSkillTraitQuestions2[ uiCnt ] )
		{
			ButtonList[ giIMPSkillTraitAnswerButton2[ uiCnt ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			ButtonList[ giIMPSkillTraitAnswerButton2[ uiCnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}

void IMPSkillTraitDisplaySkills()
{
	UINT32 uiCnt, uiCntMax;
	UINT16 usPosX, usPosY;
	UINT16 usBoxPosX, usBoxPosY;
	INT16 usSpaceBetweenButtons;
	UINT8 uiNumSkillsToStartRightColumn;
	HVOBJECT	hImageHandle;	

	if ( gGameOptions.fNewTraitSystem )
	{
		uiCntMax = IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS;
		uiNumSkillsToStartRightColumn = 4;
		usSpaceBetweenButtons = IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS + 5;

		//Display the title
		DrawTextToScreen( gzIMPSkillTraitsTextNewMajor[ IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS ], IMP_SKILL_TRAIT__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, IMP_SKILL_TRAIT__TITLE_FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	else
	{
		uiCntMax = IMP_SKILL_TRAITS__NUMBER_SKILLS;
		uiNumSkillsToStartRightColumn = 6;
		usSpaceBetweenButtons = IMP_SKILL_TRAIT__SPACE_BTN_BUTTONS;

		//Display the title
		DrawTextToScreen( gzIMPSkillTraitsText[ IMP_SKILL_TRAITS__NUMBER_SKILLS ], IMP_SKILL_TRAIT__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, IMP_SKILL_TRAIT__TITLE_FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}

	// Stats
	GetVideoObject(&hImageHandle, guiIST_GreyGoldBox );

	usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + IMP_SKILL_TRAIT__TEXT_OFFSET_X;
	usPosY = IMP_SKILL_TRAIT__LEFT_COLUMN_START_Y + IMP_SKILL_TRAIT__TEXT_OFFSET_Y;

	for( uiCnt=0; uiCnt<uiCntMax; uiCnt++ )
	{
		usBoxPosX = usPosX - IMP_SKILL_TRAIT__GREY_BOX_OFFSET_X;
		usBoxPosY = usPosY - IMP_SKILL_TRAIT__GREY_BOX_OFFSET_Y;

		//if the merc is a FEMALE, skip this skill cause there isnt any fenmal martial artists
		if( ShouldTraitBeSkipped( uiCnt ) )
		{
			//if its the none button
			if( uiCnt == uiCntMax-2 )
			{
				usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + IMP_SKILL_TRAIT__TEXT_OFFSET_X;
				usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y + IMP_SKILL_TRAIT__TEXT_OFFSET_Y;
			}
			continue;
		}

		//if the trait is selected
		if( gfSkillTraitQuestions[ uiCnt ] || gfSkillTraitQuestions2[ uiCnt ] )
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
		if( gGameOptions.fNewTraitSystem )
		{
			if ( gfSkillTraitQuestions[ uiCnt ] && gfSkillTraitQuestions2[ uiCnt ] && uiCnt != IMP_SKILL_TRAITS_NEW_MAJOR_NONE )
				DrawTextToScreen( gzIMPSkillTraitsTextNewMajor[ uiCnt+12 ], usPosX, usPosY, 0, IMP_SKILL_TRAIT__FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
			else 
				DrawTextToScreen( gzIMPSkillTraitsTextNewMajor[ uiCnt ], usPosX, usPosY, 0, IMP_SKILL_TRAIT__FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		}
		else
		{
			if ( gfSkillTraitQuestions[ uiCnt ] && gfSkillTraitQuestions2[ uiCnt ] && uiCnt != IMP_SKILL_TRAITS__NONE )
			{
				CHAR16 sSkillTraitString[50];
				swprintf(sSkillTraitString, L"%s %s", gzIMPSkillTraitsText[ uiCnt ], gzIMPSkillTraitsText[ IMP_SKILL_TRAITS__NUMBER_SKILLS+1 ]);
				DrawTextToScreen( sSkillTraitString, usPosX, usPosY, 0, IMP_SKILL_TRAIT__FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
			}
			else
				DrawTextToScreen( gzIMPSkillTraitsText[ uiCnt ], usPosX, usPosY, 0, IMP_SKILL_TRAIT__FONT, IMP_SKILL_TRAIT__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		}

		// assign help texts
		CHAR16	apStr[5000];
		swprintf( apStr, L"" );
		AssignPersonnelSkillTraitHelpText(gGameOptions.fNewTraitSystem ? gusNewMajorTraitRemap[uiCnt] : gusOldMajorTraitRemap[uiCnt], ((gfSkillTraitQuestions[uiCnt] && gfSkillTraitQuestions2[uiCnt]) ? TRUE : FALSE), fCharacterIsMale && !bBigBodySelected( ), apStr );

		// Set region help text
		SetRegionFastHelpText( &(gMR_SkillTraitHelpTextRegions[uiCnt]), apStr );
		SetRegionHelpEndCallback( &gMR_SkillTraitHelpTextRegions[uiCnt], MSYS_NO_CALLBACK );

		//Determine the next x location
		if( uiCnt < uiNumSkillsToStartRightColumn )
			usPosX = IMP_SKILL_TRAIT__LEFT_COLUMN_START_X + IMP_SKILL_TRAIT__TEXT_OFFSET_X;
		else
			usPosX = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_X + IMP_SKILL_TRAIT__TEXT_OFFSET_X;

		//Determine the next Y location
		if( uiCnt == uiNumSkillsToStartRightColumn )
			usPosY = IMP_SKILL_TRAIT__RIGHT_COLUMN_START_Y + IMP_SKILL_TRAIT__TEXT_OFFSET_Y;
		else
			usPosY += usSpaceBetweenButtons;

		//if its the none button
		if( uiCnt == uiCntMax-2 )
		{
			usPosX = IMP_SKILL_TRAIT__NONE_BTN_LOC_X + IMP_SKILL_TRAIT__TEXT_OFFSET_X;
			usPosY = IMP_SKILL_TRAIT__NONE_BTN_LOC_Y + IMP_SKILL_TRAIT__TEXT_OFFSET_Y;
		}
	}
}


void BtnIMPSkillTraitFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if( gGameOptions.fNewTraitSystem )
		{
			iCurrentImpPage = IMP_MINOR_TRAITS_PAGE;
			fButtonPendingFlag = TRUE;		
		}
		else
		{
			//if we are just reviewing the page
			if( iCurrentProfileMode == IMP__FINISH )
			{
				//go back tot he done screen
				iCurrentImpPage = IMP_FINISH;
			}
			else
			{
				iCurrentImpPage = IMP_MAIN_PAGE;

				if( CameBackToSpecialtiesPageButNotFinished() )
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
}

BOOLEAN CameBackToSpecialtiesPageButNotFinished()
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

INT32	DoesPlayerHaveExtraAttibutePointsToDistributeBasedOnSkillSelection()
{
	INT32		iExtraPoints=0; // set to zero - SANDRO
	INT32		bNumSkills=0;

	//Count the number of skills selected
	bNumSkills = CountNumSkillTraitsSelected( FALSE );

	// Externilized bonus for not taking skills - SANDRO
	if( gGameOptions.fNewTraitSystem )
	{
		// add selected minor traits
		bNumSkills += CountNumMinorTraitsSelected( FALSE );

		iExtraPoints = (max( 0, 3 - bNumSkills)) * (gGameExternalOptions.iBonusPointsPerSkillNotTaken); 
	}
	else
	{
		iExtraPoints = (max( 0, 2 - bNumSkills)) * (gGameExternalOptions.iBonusPointsPerSkillNotTaken); 
	}

	return( iExtraPoints );
}



BOOLEAN ShouldTraitBeSkipped( UINT32 uiTrait )
{
	// there are no traits discriminating any gender in new trait system
	if( gGameOptions.fNewTraitSystem )
	{
		return( FALSE );
	}
	else
	{
		// WANNE: No material arts selection allowed for big body types and females, because we don't have a material art animation on big body types!
		if( uiTrait == IMP_SKILL_TRAITS__MARTIAL_ARTS && (!fCharacterIsMale || bBigBodySelected()) )
			return( TRUE );
		else
			return( FALSE );
	}
}

void AddSelectedSkillsToSkillsList()
{
	INT32	uiCnt;

	//loop through all the buttons and reset them
	for( uiCnt=0; uiCnt<(gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_MAJOR_NONE : IMP_SKILL_TRAITS__NONE); uiCnt++ )
	{
		//if the trait is selected
		if( gfSkillTraitQuestions[ uiCnt ] )
		{
			// ugly check for old/new traits - SANDRO
			if( gGameOptions.fNewTraitSystem )
			{
				//switch on the trait, and add it to the list
				switch( uiCnt )
				{
					case IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS:
						AddSkillToSkillList( AUTO_WEAPONS_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS:
						AddSkillToSkillList( HEAVY_WEAPONS_NT );
						break;
	
					case IMP_SKILL_TRAITS_NEW_PROF_SNIPER:
						AddSkillToSkillList( SNIPER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_RANGER:
						AddSkillToSkillList( RANGER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_GUNSLINGER:
						AddSkillToSkillList( GUNSLINGER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS:
						AddSkillToSkillList( MARTIAL_ARTS_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_SQUADLEADER:
						AddSkillToSkillList( SQUADLEADER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_TECHNICIAN:
						AddSkillToSkillList( TECHNICIAN_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_DOCTOR:
						AddSkillToSkillList( DOCTOR_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_COVERT:
						AddSkillToSkillList( COVERT_NT );
						break;

					default:
						break;
				}
			}
			else
			{
				//switch on the trait, and add it to the list
				switch( uiCnt )
				{
					case IMP_SKILL_TRAITS__LOCKPICK:
						AddSkillToSkillList( LOCKPICKING_OT );
						break;

					case IMP_SKILL_TRAITS__HAND_2_HAND:
						AddSkillToSkillList( HANDTOHAND_OT );
						break;

					case IMP_SKILL_TRAITS__ELECTRONICS:
						AddSkillToSkillList( ELECTRONICS_OT );
						break;

					case IMP_SKILL_TRAITS__NIGHT_OPS:
						AddSkillToSkillList( NIGHTOPS_OT );
						break;

					case IMP_SKILL_TRAITS__THROWING:
						AddSkillToSkillList( THROWING_OT );
						break;

					case IMP_SKILL_TRAITS__TEACHING:
						AddSkillToSkillList( TEACHING_OT );
						break;

					case IMP_SKILL_TRAITS__HEAVY_WEAPONS:
						AddSkillToSkillList( HEAVY_WEAPS_OT );
						break;

					case IMP_SKILL_TRAITS__AUTO_WEAPONS:
						AddSkillToSkillList( AUTO_WEAPS_OT );
						break;

					case IMP_SKILL_TRAITS__STEALTHY:
						AddSkillToSkillList( STEALTHY_OT );
						break;

					case IMP_SKILL_TRAITS__AMBIDEXTROUS:
						AddSkillToSkillList( AMBIDEXT_OT );
						break;

					case IMP_SKILL_TRAITS__KNIFING:
						AddSkillToSkillList( KNIFING_OT );
						break;

					case IMP_SKILL_TRAITS__PROF_SNIPER:
						AddSkillToSkillList( PROF_SNIPER_OT );
						break;

					case IMP_SKILL_TRAITS__CAMO:
						AddSkillToSkillList( CAMOUFLAGED_OT );
						break;

					case IMP_SKILL_TRAITS__MARTIAL_ARTS:
						AddSkillToSkillList( MARTIALARTS_OT );
						break;

					default:
						break;
				}
			}
		}
				//if the trait is selected
		if( gfSkillTraitQuestions2[ uiCnt ] )
		{
			// ugly check for old/new traits - SANDRO
			if( gGameOptions.fNewTraitSystem )
			{
				//switch on the trait, and add it to the list
				switch( uiCnt )
				{
					case IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS:
						AddSkillToSkillList( AUTO_WEAPONS_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS:
						AddSkillToSkillList( HEAVY_WEAPONS_NT );
						break;
	
					case IMP_SKILL_TRAITS_NEW_PROF_SNIPER:
						AddSkillToSkillList( SNIPER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_RANGER:
						AddSkillToSkillList( RANGER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_GUNSLINGER:
						AddSkillToSkillList( GUNSLINGER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS:
						AddSkillToSkillList( MARTIAL_ARTS_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_SQUADLEADER:
						AddSkillToSkillList( SQUADLEADER_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_TECHNICIAN:
						AddSkillToSkillList( TECHNICIAN_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_DOCTOR:
						AddSkillToSkillList( DOCTOR_NT );
						break;

					case IMP_SKILL_TRAITS_NEW_COVERT:
						AddSkillToSkillList( COVERT_NT );
						break;

					default:
						break;
				}
			}
			else
			{
				//switch on the trait, and add it to the list
				switch( uiCnt )
				{
					case IMP_SKILL_TRAITS__LOCKPICK:
						AddSkillToSkillList( LOCKPICKING_OT );
						break;

					case IMP_SKILL_TRAITS__HAND_2_HAND:
						AddSkillToSkillList( HANDTOHAND_OT );
						break;

					case IMP_SKILL_TRAITS__ELECTRONICS:
						if (gfSkillTraitQuestions[ uiCnt ] != gfSkillTraitQuestions2[ uiCnt ] )
							AddSkillToSkillList( ELECTRONICS_OT );
						break;

					case IMP_SKILL_TRAITS__NIGHT_OPS:
						AddSkillToSkillList( NIGHTOPS_OT );
						break;

					case IMP_SKILL_TRAITS__THROWING:
						AddSkillToSkillList( THROWING_OT );
						break;

					case IMP_SKILL_TRAITS__TEACHING:
						AddSkillToSkillList( TEACHING_OT );
						break;

					case IMP_SKILL_TRAITS__HEAVY_WEAPONS:
						AddSkillToSkillList( HEAVY_WEAPS_OT );
						break;

					case IMP_SKILL_TRAITS__AUTO_WEAPONS:
						AddSkillToSkillList( AUTO_WEAPS_OT );
						break;

					case IMP_SKILL_TRAITS__STEALTHY:
						AddSkillToSkillList( STEALTHY_OT );
						break;

					case IMP_SKILL_TRAITS__AMBIDEXTROUS:
						if (gfSkillTraitQuestions[ uiCnt ] != gfSkillTraitQuestions2[ uiCnt ] )
							AddSkillToSkillList( AMBIDEXT_OT );
						break;

					case IMP_SKILL_TRAITS__KNIFING:
						AddSkillToSkillList( KNIFING_OT );
						break;

					case IMP_SKILL_TRAITS__PROF_SNIPER:
						AddSkillToSkillList( PROF_SNIPER_OT );
						break;

					case IMP_SKILL_TRAITS__CAMO:
						if (gfSkillTraitQuestions[ uiCnt ] != gfSkillTraitQuestions2[ uiCnt ] )
							AddSkillToSkillList( CAMOUFLAGED_OT );
						break;

					case IMP_SKILL_TRAITS__MARTIAL_ARTS:
						AddSkillToSkillList( MARTIALARTS_OT );
						break;
						
					default:
						break;
				}
			}
		}
	}
}


void HandleLastSelectedTraits( INT8 bNewTrait )
{
	INT16 iCnt;

	if (bNewTrait == -1)
	{
		// we only want to delete the last selected trait from list
		for( iCnt=9; iCnt>=0; iCnt-- )
		{
			if ( gbLastSelectedTraits[ iCnt ] > 0 )
			{
				gbLastSelectedTraits[ iCnt ] = 0;
				break;
			}
		}
		
	}
	else
	{
		for( iCnt=0; iCnt<9; iCnt++ )
		{
			gbLastSelectedTraits[ (iCnt + 1) ] = gbLastSelectedTraits[ iCnt ];
		}
		gbLastSelectedTraits[ 0 ] = bNewTrait;
	}
}

INT8 GetLastSelectedSkill( void )
{
	INT16 iCnt;

	for( iCnt=9; iCnt>=0; iCnt-- )
	{
		if ( gbLastSelectedTraits[ iCnt ] > 0 )
		{
			return ( gbLastSelectedTraits[ iCnt ] );
			break;
		}
	}
	return ( 0 );
}

INT32 StrengthRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
			minreq = max( minreq, 55 );
		else
			minreq = max( minreq, 40 );
	}

	return minreq;
}

INT32 AgilityRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 DexterityRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 HealthRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 LeadershipRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
			minreq = max( minreq, 50 );
		else
			minreq = max( minreq, 35 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_COVERT] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_COVERT] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_COVERT] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_COVERT] )
			minreq = max( minreq, 45 );
		else
			minreq = max( minreq, 30 );
	}

	return minreq;
}

INT32 WisdomRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_SQUADLEADER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_SQUADLEADER] )
			minreq = max( minreq, 75 );
		else
			minreq = max( minreq, 60 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_COVERT] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_COVERT] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_COVERT] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_COVERT] )
			minreq = max( minreq, 75 );
		else
			minreq = max( minreq, 60 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
			minreq = max( minreq, 65 );
		else
			minreq = max( minreq, 50 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 MarksmanshipRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_PROF_SNIPER] )
			minreq = max( minreq, 80 );
		else
			minreq = max( minreq, 65 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_RANGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_RANGER] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_GUNSLINGER] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_GUNSLINGER] )
			minreq = max( minreq, 70 );
		else
			minreq = max( minreq, 55 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 MechanicalRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 MedicalRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_DOCTOR] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_DOCTOR] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}

INT32 ExplosivesRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	INT32 minreq = 0;

	// Check first skill trait (plus second for double trait)
	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] || gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
	{
		if ( gfSkillTraitQuestions[IMP_SKILL_TRAITS_NEW_TECHNICIAN] && gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_TECHNICIAN] )
			minreq = max( minreq, 60 );
		else
			minreq = max( minreq, 45 );
	}

	return minreq;
}
