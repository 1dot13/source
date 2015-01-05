#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "_Ja25Englishtext.h"
#else
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
#endif



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
BOOLEAN ShouldTraitBeSkipped( UINT32 uiTrait );
//void		AddSelectedSkillsToSkillsList(); // SANDRO - second declaration not needed
void		HandleLastSelectedTraits( INT8 bNewTrait );
INT8 GetLastSelectedSkill( void );
BOOLEAN CameBackToSpecialtiesPageButNotFinished();

BOOLEAN DoesSkillHaveExpertLevel( UINT32 uiSkillTrait );

MOUSE_REGION	gMR_SkillTraitHelpTextRegions[IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS];

void AssignSkillTraitHelpText( UINT8 ubTraitNumber, BOOLEAN fExpertLevel );
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
	for( UINT32 uiCnt=0; uiCnt < (UINT8)( gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS ); uiCnt++ )
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

	for(iCnt = 0; iCnt < (iCntMax); iCnt++)
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

	for(iCnt = 0; iCnt < (iCntMax); iCnt++)
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
		else if ( gfSkillTraitQuestions2[ uiSkillPressed ] && !DoesSkillHaveExpertLevel( uiSkillPressed ) )
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
						for( uiCnt=0; uiCnt<uiCntMax; uiCnt++ )
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
			for ( uiCnt=0; uiCnt<(uiCntMax-1); uiCnt++ )
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
		else if ( gfSkillTraitQuestions[ uiSkillPressed ] && !DoesSkillHaveExpertLevel( uiSkillPressed ) )
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

	for( uiCnt=0; uiCnt < uiCntMax; uiCnt++ )
	{
		if( !fIncludeNoneSkill && (uiCnt == (uiCntMax-1)) )
			continue;

		//if the skill is selected ( ie depressed )
		if( gfSkillTraitQuestions[ uiCnt ] )
		{
			iNumberSkills++;
		}
		if( gfSkillTraitQuestions2[ uiCnt ] )
		{
			iNumberSkills++;
		} 
	}

	return( iNumberSkills );
}


void HandleSkillTraitButtonStates( )
{
	INT32 uiCnt;

	for( uiCnt=0; uiCnt<(gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS : IMP_SKILL_TRAITS__NUMBER_SKILLS); uiCnt++ )
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
		AssignSkillTraitHelpText( uiCnt, ((gfSkillTraitQuestions[uiCnt] && gfSkillTraitQuestions2[uiCnt]) ? TRUE : FALSE) );

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

BOOLEAN DoesSkillHaveExpertLevel( UINT32 uiSkillTrait )
{
	if( gGameOptions.fNewTraitSystem )
	{
		// WANNE: Yes we have no animation, but the Trait bonus also works with the missing animations.
		/*
		// WANNE: No material arts allowed for big body types and females, because we don't have a material art animation on big body types!
		if (uiSkillTrait == IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS && (!fCharacterIsMale || bBigBodySelected()))
			return (FALSE);
		*/
		
		return( TRUE );
	}
	else
	{
		if (uiSkillTrait == IMP_SKILL_TRAITS__ELECTRONICS || 
			uiSkillTrait == IMP_SKILL_TRAITS__AMBIDEXTROUS || 
			uiSkillTrait == IMP_SKILL_TRAITS__CAMO )
		{
			return( FALSE );
		}
		
		return( TRUE );		
	}

	return( TRUE );
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

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
			return (60);
		else
			return (45);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
			return (55);
		else
			return (40);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		return (45);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		return (40);
	}

	return(0);
}
INT32 AgilityRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		return (45);
	}

	return(0);
}
INT32 DexterityRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		return (45);
	}

	return(0);
}
INT32 HealthRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
			return (60);
		else
			return (45);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		return (45);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS ])
	{
		return (45);
	}

	return(0);
}
INT32 LeadershipRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
			return (50);
		else
			return (35);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_COVERT ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_COVERT ])
			return (45);
		else
			return (30);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		return (35);
	}
	else if ( gfSkillTraitQuestions2[IMP_SKILL_TRAITS_NEW_COVERT] )
	{
		return (30);
	}

	return(0);
}

INT32 WisdomRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
			return (75);
		else
			return (60);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_COVERT ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_COVERT ])
			return (75);
		else
			return (60);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
			return (65);
		else
			return (50);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_SQUADLEADER ])
	{
		return (60);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		return (50);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		return (45);
	}

	return(0);
}
INT32 MarksmanshipRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
			return (80);
		else
			return (65);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
			return (70);
		else
			return (55);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_PROF_SNIPER ])
	{
		return (65);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_RANGER ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_GUNSLINGER ])
	{
		return (55);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS ])
	{
		return (45);
	}

	return(0);
}
INT32 MechanicalRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		return (45);
	}

	return(0);
}
INT32 MedicalRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_DOCTOR ])
	{
		return (45);
	}

	return(0);
}
INT32 ExplosivesRequiredDueToMajorSkills( void )
{
	// Only for new trait system
	if (!gGameOptions.fNewTraitSystem || !gSkillTraitValues.fAllowAttributePrereq)
	{
		return 0;
	}

	// Check first skill trait (plus second for double trait)
	if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
			return (60);
		else
			return (45);
	}
	else if (gfSkillTraitQuestions[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
			return (60);
		else
			return (45);
	}
	// Check the second skill trait
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS ])
	{
		return (45);
	}
	else if (gfSkillTraitQuestions2[ IMP_SKILL_TRAITS_NEW_TECHNICIAN ])
	{
		return (45);
	}

	return(0);
}

void AssignSkillTraitHelpText( UINT8 ubTraitNumber, BOOLEAN fExpertLevel )
{
	CHAR16	apStr[ 5000 ];
	CHAR16	atStr[ 1500 ];

	if( gGameOptions.fNewTraitSystem )
	{
		switch( ubTraitNumber )
		{
			case IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubAWBonusCtHAssaultRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[0], ( gSkillTraitValues.ubAWBonusCtHAssaultRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWBonusCtHSMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[1], ( gSkillTraitValues.ubAWBonusCtHSMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWBonusCtHLMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[2], ( gSkillTraitValues.ubAWBonusCtHLMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWFiringSpeedBonusLMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[3], ( gSkillTraitValues.ubAWFiringSpeedBonusLMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWPercentReadyLMGReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[4], ( gSkillTraitValues.ubAWPercentReadyLMGReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWAutoFirePenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[5], ( gSkillTraitValues.ubAWAutoFirePenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWUnwantedBulletsReduction > 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[6]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[0], ( gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWRocketLaunchersAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[1], ( gSkillTraitValues.ubHWRocketLaunchersAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[2], ( gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWBonusCtHRocketLaunchers != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[3], ( gSkillTraitValues.ubHWBonusCtHRocketLaunchers * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWMortarAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[4], ( gSkillTraitValues.ubHWMortarAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWMortarCtHPenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[5], ( gSkillTraitValues.ubHWMortarCtHPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWDamageTanksBonusPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[6], ( gSkillTraitValues.ubHWDamageTanksBonusPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWDamageBonusPercentForHW != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[7], ( gSkillTraitValues.ubHWDamageBonusPercentForHW * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_PROF_SNIPER:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSNBonusCtHRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[0], ( gSkillTraitValues.ubSNBonusCtHRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNBonusCtHSniperRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[1], ( gSkillTraitValues.ubSNBonusCtHSniperRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNEffRangeToTargetReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[2], ( gSkillTraitValues.ubSNEffRangeToTargetReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNAimingBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[3], ( gSkillTraitValues.ubSNAimingBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNDamageBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[4], ( gSkillTraitValues.ubSNDamageBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					if( gSkillTraitValues.ubSNDamageBonusFromNumClicks == 0)
					{
						wcscat( apStr, gzIMPMajorTraitsHelpTextsSniper[5] );
						wcscat( apStr, gzIMPMajorTraitsHelpTextsSniper[6] );
					}
					else if( gSkillTraitValues.ubSNDamageBonusFromNumClicks == 1 )
					{
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[6] );
					}
					else
					{
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[6] );
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[gSkillTraitValues.ubSNDamageBonusFromNumClicks + 4] );
					}
					wcscat( atStr, L"\n" );
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNChamberRoundAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[14], ( gSkillTraitValues.ubSNChamberRoundAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNAimClicksAdded != 0 )
				{
					if( UsingNewCTHSystem() == true )
					{
						if( gSkillTraitValues.ubSNAimClicksAdded == 1 && !fExpertLevel )
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[17]);
						else
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[18], ( gSkillTraitValues.ubSNAimClicksAdded * (fExpertLevel ? 2 : 1)));
					}
					else
					{
						if( gSkillTraitValues.ubSNAimClicksAdded == 1 && !fExpertLevel)
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[15]);
						else
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[16], ( gSkillTraitValues.ubSNAimClicksAdded * (fExpertLevel ? 2 : 1)));
					}
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_RANGER:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubRABonusCtHRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[0], ( gSkillTraitValues.ubRABonusCtHRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRABonusCtHShotguns != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[1], ( gSkillTraitValues.ubRABonusCtHShotguns * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAPumpShotgunsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[2], ( gSkillTraitValues.ubRAPumpShotgunsAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAFiringSpeedBonusShotguns != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[3], ( gSkillTraitValues.ubRAFiringSpeedBonusShotguns * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAAimClicksAdded != 0 )
				{
					if( (gSkillTraitValues.ubRAAimClicksAdded >= 2 && !fExpertLevel) || (gSkillTraitValues.ubRAAimClicksAdded == 1 && fExpertLevel) )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[4], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) / 2.0f));
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[5], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) / 2.0f));

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAGroupTimeSpentForTravellingFoot != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[6], ( gSkillTraitValues.ubRAGroupTimeSpentForTravellingFoot * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAGroupTimeSpentForTravellingVehicle != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[7], ( gSkillTraitValues.ubRAGroupTimeSpentForTravellingVehicle * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRABreathForTravellingReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[8], ( gSkillTraitValues.ubRABreathForTravellingReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAWeatherPenaltiesReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[9], ( gSkillTraitValues.ubRAWeatherPenaltiesReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
			//	if( gSkillTraitValues.ubRACamoEffectivenessBonus != 0 )
			//	{
			//		swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[10], ( gSkillTraitValues.ubRACamoEffectivenessBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
			//		wcscat( apStr, atStr );
			//	}
				if( gSkillTraitValues.ubRACamoWornountSpeedReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[11], ( gSkillTraitValues.ubRACamoWornountSpeedReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_GUNSLINGER:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubGSFiringSpeedBonusPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[0], ( gSkillTraitValues.ubGSFiringSpeedBonusPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSEffectiveRangeBonusPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[1], ( gSkillTraitValues.ubGSEffectiveRangeBonusPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSBonusCtHPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[2], ( gSkillTraitValues.ubGSBonusCtHPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSBonusCtHMachinePistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[3], ( gSkillTraitValues.ubGSBonusCtHMachinePistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					if( gSkillTraitValues.ubGSCtHMPExcludeAuto )
						wcscat( atStr, gzIMPMajorTraitsHelpTextsGunslinger[4] );
					wcscat( atStr, L"\n");
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSAimingBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[5], ( gSkillTraitValues.ubGSAimingBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSPercentReadyPistolsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[6], ( gSkillTraitValues.ubGSPercentReadyPistolsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[7], ( gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSAimClicksAdded != 0 )
				{
					if( gSkillTraitValues.ubGSAimClicksAdded == 1 && !fExpertLevel )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[8], ( gSkillTraitValues.ubGSAimClicksAdded * (fExpertLevel ? 2 : 1)));
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[9], ( gSkillTraitValues.ubGSAimClicksAdded * (fExpertLevel ? 2 : 1)));

					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubMAPunchAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[0], ( gSkillTraitValues.ubMAPunchAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusCtHBareHands != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[1], ( gSkillTraitValues.ubMABonusCtHBareHands * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusCtHBrassKnuckles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[2], ( gSkillTraitValues.ubMABonusCtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusDamageHandToHand != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[3], ( gSkillTraitValues.ubMABonusDamageHandToHand * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusBreathDamageHandToHand != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[4], ( gSkillTraitValues.ubMABonusBreathDamageHandToHand * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usMALostBreathRegainPenalty != 0 )
				{
					if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 25)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[5]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 50)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[6]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 100)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[7]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 200)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[8]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 400)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[9]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 700)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[10]);
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[11]);

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usMAAimedPunchDamageBonus != 0 )
				{
					if (!fCharacterIsMale || bBigBodySelected() || (gSkillTraitValues.fPermitExtraAnimationsOnlyToMA && (!gfSkillTraitQuestions[ubTraitNumber] || !gfSkillTraitQuestions2[ubTraitNumber])))
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[12], ( gSkillTraitValues.usMAAimedPunchDamageBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[13], ( gSkillTraitValues.usMAAimedPunchDamageBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToDodgeHtH != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[14], ( gSkillTraitValues.ubMAChanceToDodgeHtH * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAOnTopCTDHtHBareHanded != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[15], ( gSkillTraitValues.ubMAOnTopCTDHtHBareHanded * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles == gSkillTraitValues.ubMAOnTopCTDHtHBareHanded ) 
						wcscat( apStr, gzIMPMajorTraitsHelpTextsMartialArts[16] );
					else if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles > 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[17], ( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					wcscat( apStr, L"\n" );
				}
				else if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[18], ( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToDodgeMelee != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[19], ( gSkillTraitValues.ubMAChanceToDodgeMelee * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAReducedAPsToSteal != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[20], ( gSkillTraitValues.ubMAReducedAPsToSteal * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAAPsChangeStanceReduction != 0 && ( gSkillTraitValues.ubMAAPsChangeStanceReduction == gSkillTraitValues.ubMAApsTurnAroundReduction == gSkillTraitValues.ubMAAPsClimbOrJumpReduction ))
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[21], ( gSkillTraitValues.ubMAAPsChangeStanceReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				else 
				{
					if( gSkillTraitValues.ubMAAPsChangeStanceReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[22], ( gSkillTraitValues.ubMAAPsChangeStanceReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					if( gSkillTraitValues.ubMAApsTurnAroundReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[23], ( gSkillTraitValues.ubMAApsTurnAroundReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					if( gSkillTraitValues.ubMAAPsClimbOrJumpReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[24], ( gSkillTraitValues.ubMAAPsClimbOrJumpReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
				}
				if( gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving != 0 && gGameOptions.fImprovedInterruptSystem )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[27], ( gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToCkickDoors != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[25], ( gSkillTraitValues.ubMAChanceToCkickDoors * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if (fCharacterIsMale && !bBigBodySelected() && 
					((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA && fExpertLevel) ||
					!gSkillTraitValues.fPermitExtraAnimationsOnlyToMA ))
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[26]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_SQUADLEADER:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSLBonusAPsPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[0], ( gSkillTraitValues.ubSLBonusAPsPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLEffectiveLevelInRadius != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[1], ( gSkillTraitValues.ubSLEffectiveLevelInRadius * (fExpertLevel ? 2 : 1)), (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLEffectiveLevelAsStandby != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[2], ( gSkillTraitValues.ubSLEffectiveLevelAsStandby * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLCollectiveInterruptsBonus != 0 && gGameOptions.fImprovedInterruptSystem )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[11], ( gSkillTraitValues.ubSLCollectiveInterruptsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLOverallSuppresionBonusPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[3], ( gSkillTraitValues.ubSLOverallSuppresionBonusPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLMoraleGainBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[4], ( gSkillTraitValues.ubSLMoraleGainBonus * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLMoraleLossReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[5], ( gSkillTraitValues.ubSLMoraleLossReduction * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
							
				swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[6], gSkillTraitValues.usSLRadiusNormal);
				wcscat( apStr, atStr );
				if( gSkillTraitValues.usSLRadiusExtendedEar > gSkillTraitValues.usSLRadiusNormal )
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[7], gSkillTraitValues.usSLRadiusExtendedEar);

				wcscat( apStr, atStr );
				wcscat( apStr, L"\n" );

				if( gSkillTraitValues.ubSLMaxBonuses > 1 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[8], gSkillTraitValues.ubSLMaxBonuses ) ;
					wcscat( apStr, atStr );
				}
				
				if( gSkillTraitValues.ubSLFearResistance != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[9], ( gSkillTraitValues.ubSLFearResistance * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLDeathMoralelossMultiplier != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[10], (1 + ( gSkillTraitValues.ubSLDeathMoralelossMultiplier * (fExpertLevel ? 2 : 1))), (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_TECHNICIAN:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.usTERepairSpeedBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[0], ( gSkillTraitValues.usTERepairSpeedBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTELockpickingBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[1], ( gSkillTraitValues.usTELockpickingBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTEDisarmElTrapBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[2], ( gSkillTraitValues.usTEDisarmElTrapBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTEAttachingItemsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[3], ( gSkillTraitValues.usTEAttachingItemsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTEUnjamGunBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[4], ( gSkillTraitValues.ubTEUnjamGunBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTERepairElectronicsPenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[5], ( gSkillTraitValues.ubTERepairElectronicsPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTEChanceToDetectTrapsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[6], ( gSkillTraitValues.ubTEChanceToDetectTrapsBonus * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTECtHControlledRobotBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[7], ( gSkillTraitValues.ubTECtHControlledRobotBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTETraitsNumToRepairRobot == 2 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[8], (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				else if( gSkillTraitValues.ubTETraitsNumToRepairRobot == 1 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[8], (fExpertLevel ? gzIMPSkillTraitsTextNewMajor[ubTraitNumber + NEWTRAIT_EXPERT_OFFSET] : gzIMPSkillTraitsTextNewMajor[ubTraitNumber]));
					wcscat( apStr, atStr );

					if( gSkillTraitValues.ubTERepairRobotPenaltyReduction != 0 && fExpertLevel)
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[9], ( gSkillTraitValues.ubTERepairRobotPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_DOCTOR:
			{
				BOOLEAN fCanSurgery = FALSE;
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubDONumberTraitsNeededForSurgery != 0 && ((gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop) > 0))
				{
					if( gSkillTraitValues.ubDONumberTraitsNeededForSurgery <= (fExpertLevel ? 2 : 1))
					{
						fCanSurgery = TRUE;
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[0]);
						wcscat( apStr, atStr );
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[1], (gSkillTraitValues.ubDOSurgeryHealPercentBase + ( gSkillTraitValues.ubDOSurgeryHealPercentOnTop * (fExpertLevel ? 2 : 1))), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
						if( gSkillTraitValues.usDOSurgeryMedBagConsumption >= 60 )
							wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[2] );

						wcscat( apStr, L"\n" );
					}
				}
				if( (gSkillTraitValues.usDORepairStatsRateBasic + gSkillTraitValues.usDORepairStatsRateOnTop) > 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[3]);
					wcscat( apStr, atStr );
					if( fCanSurgery )
						wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[4] );

					wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[5] );
				}
				if( gSkillTraitValues.usDODoctorAssignmentBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[6], ( gSkillTraitValues.usDODoctorAssignmentBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDOBandagingSpeedPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[7], ( gSkillTraitValues.ubDOBandagingSpeedPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDONaturalRegenBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[8], ( gSkillTraitValues.ubDONaturalRegenBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					if( gSkillTraitValues.ubDOMaxRegenBonuses > 1 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[9], gSkillTraitValues.ubDOMaxRegenBonuses ) ;
						wcscat( apStr, atStr );
					}
					wcscat( apStr, L"\n" );
				}
				break;
			}
			case IMP_SKILL_TRAITS_NEW_COVERT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[0]);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[1]);
				wcscat( apStr, atStr );

				if ( !fExpertLevel )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[2], gSkillTraitValues.sCOCloseDetectionRange);
					wcscat( apStr, atStr );
				}

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[3], gSkillTraitValues.sCOCloseDetectionRangeSoldierCorpse);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[4], ( gSkillTraitValues.sCOMeleeCTHBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[5], ( gSkillTraitValues.sCoMeleeInstakillBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				
				INT16 apreduction =  ( gSkillTraitValues.sCODisguiseAPReduction * (fExpertLevel ? 2 : 1));
				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[6], apreduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS_NEW_MAJOR_NONE:
			{
				swprintf( apStr, gzIMPMajorTraitsHelpTextsNone[0] );
				break;
			}
		}
	}
	else
	{
		switch( ubTraitNumber )
		{
			case IMP_SKILL_TRAITS__LOCKPICK:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[LOCKPICKING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[0], ( gbSkillTraitBonus[LOCKPICKING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__HAND_2_HAND:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[HANDTOHAND_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[1], (  gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[2], (  3 * gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[3], (  gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__ELECTRONICS:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[4] );
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__NIGHT_OPS:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[5], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[6], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[7], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[8], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[9], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__THROWING:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[THROWING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[10], (  gbSkillTraitBonus[THROWING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[11], (  gbSkillTraitBonus[THROWING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[12], (  10 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__TEACHING:
			{
				swprintf( apStr, L"" );
				if( gGameExternalOptions.ubTeachBonusToTrain != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[13], (  gGameExternalOptions.ubTeachBonusToTrain * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( (gGameExternalOptions.usTeacherTraitEffectOnLeadership - 100) > 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[14], (  (gGameExternalOptions.usTeacherTraitEffectOnLeadership - 100) * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__HEAVY_WEAPONS:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[HEAVY_WEAPS_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[15], (  gbSkillTraitBonus[HEAVY_WEAPS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__AUTO_WEAPONS:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[16], (  2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[17] );
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__STEALTHY:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[18], (  25 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				if( gGameExternalOptions.ubStealthTraitCoverValue != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[19], (  gGameExternalOptions.ubStealthTraitCoverValue * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__AMBIDEXTROUS:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[20] );
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__KNIFING:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[KNIFING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[21], (  gbSkillTraitBonus[KNIFING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[22], (  gbSkillTraitBonus[KNIFING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[23], (  gbSkillTraitBonus[KNIFING_OT] / 3 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[24], (  gbSkillTraitBonus[KNIFING_OT] / 2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case IMP_SKILL_TRAITS__PROF_SNIPER:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[PROF_SNIPER_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[25], (  gbSkillTraitBonus[PROF_SNIPER_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[26], (  10 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__CAMO:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[27] );
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__MARTIAL_ARTS:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[MARTIALARTS_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[28], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[29], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[30], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[31], (  gbSkillTraitBonus[MARTIALARTS_OT] * 2 / 3 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[32], (  gbSkillTraitBonus[MARTIALARTS_OT] / 2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[33]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[34]);
				wcscat( apStr, atStr );
				break;
			}
			case IMP_SKILL_TRAITS__NONE:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[35] );
				wcscat( apStr, atStr );
				break;
			}
		}
	}

	// Set region help text
	SetRegionFastHelpText( &(gMR_SkillTraitHelpTextRegions[ubTraitNumber]), apStr );
	SetRegionHelpEndCallback( &gMR_SkillTraitHelpTextRegions[ubTraitNumber], MSYS_NO_CALLBACK );

	return;
}