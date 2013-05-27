#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "GameSettings.h"
#else
	#include "CharProfile.h"
	#include "IMP Attribute Selection.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "Utilities.h"
	#include "input.h"
	#include "Isometric Utils.h"
	#include "IMP Skill Trait.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "IMP Text System.h"
	#include "IMP Compile Character.h"
	#include "text.h"
	#include "GameSettings.h"
	// Added these 3 - SANDRO
	#include "IMP Disability Trait.h" 
	#include "IMP Color Choosing.h"
	#include "IMP Minor Trait.h"

#endif

#define STARTING_LEVEL_BOX_POS_X	( 51 )
#define STARTING_LEVEL_BOX_POS_Y	( 296 )


// width of the slider bar region
#define BAR_WIDTH 423 - 197

// width of the slider bar itself
#define SLIDER_BAR_WIDTH 38

// the sizeof one skill unit on the sliding bar in pixels
#define BASE_SKILL_PIXEL_UNIT_SIZE ( ( 423 - 230 ) )

//#define	MAX_ATTIBUTEPOINT		90
//#define	START_ATTRIBEPOINT		55
//#define	MIN_ATTIBUTEPOINT	 35
//#define MAX_ZERO_BONUS			15


enum{
	HEALTH_ATTRIBUTE,
	DEXTERITY_ATTRIBUTE,
	AGILITY_ATTRIBUTE,
	STRENGTH_ATTRIBUTE,
	WISDOM_ATTRIBUTE,
	LEADERSHIP_ATTRIBUTE,
	MARKSMANSHIP_SKILL,
	EXPLOSIVE_SKILL,
	MEDICAL_SKILL,
	MECHANICAL_SKILL,
};


enum
{
	SLIDER_ERROR,
	SLIDER_OK,
	SLIDER_OUT_OF_RANGE,
};

// Snap: these globals will be properly initialized in SetAttributes

INT32 iStartingLevel = 1; // Added by SANDRO
INT32 iLevelCostMultiplier; // Added by SANDRO

// the skills as they stand
INT32 iCurrentStrength	= 0;
INT32 iCurrentAgility	 = 0;
INT32 iCurrentDexterity	= 0;
INT32 iCurrentHealth		= 0;
INT32 iCurrentLeaderShip	= 0;
INT32 iCurrentWisdom		= 0;
INT32 iCurrentMarkmanship = 0;
INT32 iCurrentMechanical	= 0;
INT32 iCurrentMedical	 = 0;
INT32 iCurrentExplosives	= 0;

INT32 iStrengthMinimum = gGameExternalOptions.iMinAttribute;
INT32 iAgilityMinimum	 = gGameExternalOptions.iMinAttribute;
INT32 iDexterityMinimum	= gGameExternalOptions.iMinAttribute;
INT32 iHealthMinimum		= gGameExternalOptions.iMinAttribute;
INT32 iLeaderShipMinimum	= gGameExternalOptions.iMinAttribute;
INT32 iWisdomMinimum		= gGameExternalOptions.iMinAttribute;
INT32 iMarkmanshipMinimum = gGameExternalOptions.iMinAttribute;
INT32 iMechanicalMinimum	= gGameExternalOptions.iMinAttribute;
INT32 iMedicalMinimum	 = gGameExternalOptions.iMinAttribute;
INT32 iExplosivesMinimum	= gGameExternalOptions.iMinAttribute;



// which stat is message about stat at zero about
INT32 iCurrentStatAtZero = 0;

// total number of bonus points
// Kaiden: Externalized this value for option-ation
INT32 iCurrentBonusPoints = 0;
//INT32 iCurrentBonusPoints = 40;

// diplsay the 0 skill point warning..if skill set to 0, warn character
BOOLEAN fSkillAtZeroWarning = FALSE;

// is the sliding of the sliding bar active right now?
BOOLEAN fSlideIsActive = TRUE;

// first time in game
BOOLEAN fFirstIMPAttribTime = TRUE;

// review mode
BOOLEAN fReviewStats = FALSE;

// buttons
UINT32 giIMPAttributeSelectionButton[ 1 ];
UINT32 giIMPAttributeSelectionButtonImage[ 1 ];

// STARTING LEVEL BUTTONS ADDED - SANDRO
UINT32 giIMPStartingLevelButton[ 2 ];
UINT32 giIMPStartingLevelButtonImage[ 2 ];


// slider buttons
UINT32 giIMPAttributeSelectionSliderButton[ 20 ];
UINT32 giIMPAttributeSelectionSliderButtonImage[ 20 ];

// mouse regions
MOUSE_REGION pSliderRegions[ 10 ];
MOUSE_REGION pSliderBarRegions[ 10 ];


//The currently "anchored scroll bar"
MOUSE_REGION *gpCurrentScrollBox = NULL;
INT32					giCurrentlySelectedStat = -1;

// has any of the sliding bars moved?...for re-rendering puposes
BOOLEAN fHasAnySlidingBarMoved = FALSE;

INT32 uiBarToReRender = -1;

// are we actually coming back to edit, or are we restarting?
BOOLEAN fReturnStatus = FALSE;

UINT32	guiIST_GreyGoldBoxLvl;


// function definition
void ProcessAttributes( void );
void DestroyIMPAttributeSelectionButtons( void );
void CreateIMPAttributeSelectionButtons( void );
UINT8 IncrementStat( INT32 iStatToIncrement );
UINT8 DecrementStat( INT32 iStatToDecrement	);
BOOLEAN DoWeHaveThisManyBonusPoints( INT32 iBonusPoints );
void CreateAttributeSliderButtons( void );
void DestroyAttributeSliderButtons( void );
void CreateSlideRegionMouseRegions( void );
void DestroySlideRegionMouseRegions( void );
INT32 GetCurrentAttributeValue( INT32 iAttribute );
void CreateSliderBarMouseRegions( void );
void DestroySlideBarMouseRegions( void );
void SetAttributes( void );
void DrawBonusPointsRemaining( void );
void SetGeneratedCharacterAttributes( void );
INT32 DetermineNewValue( INT32 iNewX );
INT32 DetermineNewPosition( INT32 iAttribute );

// callbacks
void BtnIMPAttributeFinishCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPAttributeSliderLeftCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPAttributeSliderRightCallback(GUI_BUTTON *btn,INT32 reason);
void SliderRegionButtonCallback(MOUSE_REGION * pRegion, INT32 iReason );
void SliderBarRegionButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );
void StatAtZeroBoxCallBack( UINT8 bExitValue );

// added starting level choosing
void CreateIMPStartingLevelSelectionButtons( void );
void BtnIMPLevelSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnIMPLevelSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );
INT32 iBonusPointsNeededForLevelUp();
INT32 iBonusPointsRecievedForLevelDown();

void EnterIMPAttributeSelection( void )
{


	// set attributes and skills
	if( ( fReturnStatus == FALSE ) && ( fFirstIMPAttribTime == TRUE ) )
	{
		// re starting
		SetAttributes( );

		iStartingLevel = 1; // ADDED - SADNRO

		gpCurrentScrollBox = NULL;
		giCurrentlySelectedStat = -1;

	// does character have PROBLEMS!!?!?!
		/*
	if( DoesCharacterHaveAnAttitude() )
		{
		iCurrentBonusPoints+= 10;
		}
	if( DoesCharacterHaveAPersoanlity( ) )
		{
		iCurrentBonusPoints += 10;
		}
		*/
	}
	fReturnStatus = TRUE;
	fFirstIMPAttribTime = FALSE;

	// create done button
	CreateIMPAttributeSelectionButtons( );
	// create clider buttons
	CreateAttributeSliderButtons( );

	// the mouse regions
	CreateSlideRegionMouseRegions( );
	//CreateSliderBarMouseRegions( );

	// create starting level choice button
	CreateIMPStartingLevelSelectionButtons( );

	// render background
	RenderIMPAttributeSelection( );



	return;
}

void RenderIMPAlteredAttribute( void )
{


}
void RenderIMPAttributeSelection( void )
{
	// the background
	RenderProfileBackGround( );


	// attribute frame
	RenderAttributeFrame( 51, 87 );

	// render attribute boxes
	RenderAttributeBoxes( );

	RenderAttrib1IndentFrame(51, 30 );

	if( fReviewStats != TRUE )
	{
	RenderAttrib2IndentFrame(350, 42 );
	}

	// reset rerender flag
	fHasAnySlidingBarMoved = FALSE;

	// print text for screen
	PrintImpText( );

	// texts around
	// don't blit bonus if reviewing
	if( fReviewStats != TRUE )
	{
		// Bonus points remaining
		DisplayWrappedString( LAPTOP_SCREEN_UL_X + 355, LAPTOP_SCREEN_WEB_UL_Y + 51,  ( 640 ), 2, FONT12ARIAL, FONT_WHITE, sgAttributeSelectionText[2],FONT_BLACK,FALSE,0);

		// this show us the exact maximum attribute based on ini option
		CHAR16 sMaxPntsString[200];
		INT32 iMaxPointsToShow  = gGameExternalOptions.iMaxAttribute;

		if (iMaxPointsToShow > 100 )
			iMaxPointsToShow = 100;

		swprintf(sMaxPntsString, L"%s %d%s", sgAttributeSelectionText[0], iMaxPointsToShow, L".");

		// Explanatory text
		DisplayWrappedString( LAPTOP_SCREEN_UL_X + 59, LAPTOP_SCREEN_WEB_UL_Y + 36,  ( 240 ), 2, FONT10ARIAL, 142, sMaxPntsString,FONT_BLACK,FALSE,0);

		InvalidateRegion( LAPTOP_SCREEN_UL_X + 65, LAPTOP_SCREEN_WEB_UL_Y + 51, LAPTOP_SCREEN_UL_X + 85, LAPTOP_SCREEN_WEB_UL_Y + 71 );

	}
	else
	{
		//LoadAndDisplayIMPText( LAPTOP_SCREEN_UL_X + 60, LAPTOP_SCREEN_WEB_UL_Y + 44, ( 200 ), sgAttributeSelectionTextReview, FONT10ARIAL, 142, TRUE, 0);
		DisplayWrappedString( LAPTOP_SCREEN_UL_X + 60, LAPTOP_SCREEN_WEB_UL_Y + 44,  ( 240 ), 2, FONT10ARIAL, 142, sgAttributeSelectionText[1],FONT_BLACK,FALSE,0);
	}

	// amt of bonus pts
	DrawBonusPointsRemaining( );

	//***********************************************************************************************
	// STARTING LEVEL BOX
	///////////////////////////

	// Bonus frame for starting level
	RenderAttribStartingLevelFrame( STARTING_LEVEL_BOX_POS_X, STARTING_LEVEL_BOX_POS_Y );

	// text
	DrawTextToScreen( sgAttributeSelectionText[3], LAPTOP_SCREEN_UL_X + STARTING_LEVEL_BOX_POS_X + 9, LAPTOP_SCREEN_WEB_UL_Y + STARTING_LEVEL_BOX_POS_Y + 4, ( 100 ), FONT12ARIAL, FONT_WHITE, FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	CHAR16 sStartLevelString[64];
	swprintf(sStartLevelString, L"%d", iStartingLevel );

	// print string
	DrawTextToScreen( sStartLevelString, STARTING_LEVEL_BOX_POS_X + LAPTOP_SCREEN_UL_X + 99, STARTING_LEVEL_BOX_POS_Y + LAPTOP_SCREEN_WEB_UL_Y + 6, ( 100 ), FONT14ARIAL, 142, FONT_BLACK, FALSE, CENTER_JUSTIFIED );

	InvalidateRegion( STARTING_LEVEL_BOX_POS_X + LAPTOP_SCREEN_UL_X + 94, STARTING_LEVEL_BOX_POS_Y + LAPTOP_SCREEN_WEB_UL_Y + 8, STARTING_LEVEL_BOX_POS_X + LAPTOP_SCREEN_UL_X + 130, STARTING_LEVEL_BOX_POS_Y + LAPTOP_SCREEN_WEB_UL_Y + 42 );

	//****************************************

	return;
}

void ExitIMPAttributeSelection( void )
{
	// get rid of slider buttons
	DestroyAttributeSliderButtons( );

	// the mouse regions
	DestroySlideRegionMouseRegions( );
	//DestroySlideBarMouseRegions( );

	// get rid of done buttons
	DestroyIMPAttributeSelectionButtons( );

	fReturnStatus = FALSE;

	return;
}


void HandleIMPAttributeSelection( void )
{

	// review mode, do not allow changes
	if( fReviewStats )
	{
		return;
	}

	// set the currently selectd slider bar
	if( gfLeftButtonState && gpCurrentScrollBox != NULL )
	{
		//if theuser is holding down the mouse cursor to left of the start of the slider bars
		if( gusMouseXPos < ( SKILL_SLIDE_START_X + LAPTOP_SCREEN_UL_X ) )
		{
			DecrementStat( giCurrentlySelectedStat );
		}

		//else if the user is holding down the mouse button to the right of the scroll bars
		else if( gusMouseXPos > ( LAPTOP_SCREEN_UL_X + SKILL_SLIDE_START_X + BAR_WIDTH	) )
		{
			IncrementStat( giCurrentlySelectedStat );
		}
		else
		{
			INT32		iCurrentAttributeValue;
			INT32		sNewX = gusMouseXPos;
			INT32		iNewValue;
			INT32		iCounter;


			// get old stat value
		iCurrentAttributeValue = GetCurrentAttributeValue( giCurrentlySelectedStat );
			iNewValue = DetermineNewValue( sNewX );

			// chenged, move mouse region if change large enough
		if( iCurrentAttributeValue != iNewValue )
			{
		 // update screen
		 fHasAnySlidingBarMoved = TRUE;
			}

		// change is enough
		if ( iNewValue - iCurrentAttributeValue > 0)
			{
				// positive, increment stat
		iCounter = iNewValue - iCurrentAttributeValue;
				for( iCounter; iCounter > 0; iCounter--)
				{
					IncrementStat( giCurrentlySelectedStat );
				}
			}
			else
			{
				// negative, decrement stat
		iCounter = iCurrentAttributeValue - iNewValue;
				for( iCounter; iCounter > 0; iCounter--)
				{
					DecrementStat( giCurrentlySelectedStat );
				}
			}
		}

		RenderIMPAttributeSelection( );
	}
	else
	{
		gpCurrentScrollBox = NULL;
		giCurrentlySelectedStat = -1;
	}


	// prcoess current state of attributes
	ProcessAttributes( );

	// has any bar moved?
	if( fHasAnySlidingBarMoved )
	{
		// render
		if( uiBarToReRender == -1 )
		{
			RenderIMPAttributeSelection( );
		}
		else
		{

			RenderAttributeFrameForIndex( 51, 87, uiBarToReRender );
			/*
			// print text for screen
			PrintImpText( );

			// amt of bonus pts
			DrawBonusPointsRemaining( );

			RenderAttributeFrame( 51, 87 );

			// render attribute boxes
			RenderAttributeBoxes( );

			PrintImpText( );

			InvalidateRegion( LAPTOP_SCREEN_UL_X + 51, LAPTOP_SCREEN_WEB_UL_Y + 87, LAPTOP_SCREEN_UL_X + 51 + 400, LAPTOP_SCREEN_WEB_UL_Y + 87 + 220 );
*/
			uiBarToReRender = -1;
			MarkButtonsDirty();
		}

		fHasAnySlidingBarMoved = FALSE;

	}
	if ( fSkillAtZeroWarning == TRUE )
	{
		DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pSkillAtZeroWarning[ 0 ], LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, StatAtZeroBoxCallBack);
		fSkillAtZeroWarning = FALSE;

	}
	return;
}



void ProcessAttributes( void )
{

	//Kaiden: Ignore the values in the below comment, this is now controlled by
	// Values externalized in the Ja2_Options.ini file.

	// this function goes through and confirms thet state of attributes, ie not allowing attributes to
	// drop below 35 or skills to go below 0...and if skill is 34 set to 0

	// check any attribute below 35

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxAttribute = gGameExternalOptions.iMaxAttribute;

	// strength
	if( iCurrentStrength <= iStrengthMinimum )
	{
		iCurrentStrength = iStrengthMinimum;
		// disable button too


	}

	// dex
	if( iCurrentDexterity <= iDexterityMinimum )
	{
		iCurrentDexterity = iDexterityMinimum;
		// disable button too


	}

	// agility
	if( iCurrentAgility <= iAgilityMinimum )
	{
		iCurrentAgility = iAgilityMinimum;
		// disable button too


	}

	// wisdom
	if( iCurrentWisdom <= iWisdomMinimum )
	{
		iCurrentWisdom = iWisdomMinimum;
		// disable button too


	}

	// Kaiden: Leadership should be zero-able
	// LeaderShip
	//if( iCurrentLeaderShip <= MIN_ATTIBUTEPOINT )
	//{
	//iCurrentLeaderShip = MIN_ATTIBUTEPOINT;
	// disable button too
	//}

	// health
	if( iCurrentHealth <= iHealthMinimum )
	{
		iCurrentHealth = iHealthMinimum;
		// disable button too

	}




	// now check for above MAX_ATTIBUTEPOINT
	// strength
	if( iCurrentStrength >= iMaxAttribute )
	{
		iCurrentStrength = iMaxAttribute;
		// disable button too


	}

	// dex
	if( iCurrentDexterity >= iMaxAttribute )
	{
		iCurrentDexterity = iMaxAttribute;
		// disable button too


	}

	// agility
	if( iCurrentAgility >= iMaxAttribute )
	{
		iCurrentAgility = iMaxAttribute;
		// disable button too


	}

	// wisdom
	if( iCurrentWisdom >= iMaxAttribute )
	{
		iCurrentWisdom = iMaxAttribute;
		// disable button too


	}

	// LeaderShip
	if( iCurrentLeaderShip >= iMaxAttribute )
	{
		iCurrentLeaderShip = iMaxAttribute;
		// disable button too


	}

	// health
	if( iCurrentHealth >= iMaxAttribute )
	{
		iCurrentHealth = iMaxAttribute;
		// disable button too

	}

	return;
}

UINT8 IncrementStat( INT32 iStatToIncrement )
{
	// this function is responsable for incrementing a stat


	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxAttribute = gGameExternalOptions.iMaxAttribute;
	INT32 iMaxZeroBonus = gGameExternalOptions.iMaxZeroBonus;

	// review mode, do not allow changes
	if( fReviewStats )
	{
		return( SLIDER_ERROR );
	}

	// make sure we have enough bonus points
	if( iCurrentBonusPoints < 1 )
	{
		// nope...GO GET SOME BONUS POINTS, IDIOT!
		return( SLIDER_ERROR );
	}

	// check to make sure stat isn't maxed out already
	switch( iStatToIncrement )
	{
		case( STRENGTH_ATTRIBUTE ):
		if( iCurrentStrength > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				iCurrentStrength++;
			iCurrentBonusPoints--;
			}
		break;
	case( DEXTERITY_ATTRIBUTE ):
		if( iCurrentDexterity > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				iCurrentDexterity++;
			iCurrentBonusPoints--;
			}
		break;
		case( AGILITY_ATTRIBUTE ):
		if( iCurrentAgility > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				iCurrentAgility++;
			iCurrentBonusPoints--;
			}
		break;
		case( WISDOM_ATTRIBUTE ):
		if( iCurrentWisdom > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				iCurrentWisdom++;
			iCurrentBonusPoints--;
			}
		break;
		case( HEALTH_ATTRIBUTE ):
		if( iCurrentHealth > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				iCurrentHealth++;
			iCurrentBonusPoints--;
			}
		break;
		case( LEADERSHIP_ATTRIBUTE ):
		if( iCurrentLeaderShip > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
			}
		else
			{
				if( iCurrentLeaderShip == 0)
				{
					if( DoWeHaveThisManyBonusPoints( iMaxZeroBonus ) == TRUE )
					{
					iCurrentLeaderShip+=iLeaderShipMinimum;
					iCurrentBonusPoints-=iMaxZeroBonus;
						fSkillAtZeroWarning = FALSE;
					}
					else
					{
						return( SLIDER_OK );
					}
				}
				else
				{
				iCurrentLeaderShip++;
			 iCurrentBonusPoints--;
				}
			}
		break;
		case( MARKSMANSHIP_SKILL ):
		if( iCurrentMarkmanship > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				if( iCurrentMarkmanship == 0)
				{
					if( DoWeHaveThisManyBonusPoints( iMaxZeroBonus ) == TRUE )
					{
					iCurrentMarkmanship+=iMarkmanshipMinimum;
					iCurrentBonusPoints-=iMaxZeroBonus;
						fSkillAtZeroWarning = FALSE;
					}
					else
					{
						return( SLIDER_OK );
					}
				}
				else
				{
				iCurrentMarkmanship++;
			 iCurrentBonusPoints--;
				}
			}
		break;
		case( MECHANICAL_SKILL ):
		if( iCurrentMechanical > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				if( iCurrentMechanical == 0)
				{
					if( DoWeHaveThisManyBonusPoints( iMaxZeroBonus ) == TRUE )
					{
					iCurrentMechanical+=iMechanicalMinimum;
					iCurrentBonusPoints-=iMaxZeroBonus;
						fSkillAtZeroWarning = FALSE;
					}
					else
					{
						return( SLIDER_OK );
					}
				}
				else
				{
				iCurrentMechanical++;
			 iCurrentBonusPoints--;
				}
			}
		break;
		case( MEDICAL_SKILL ):
		if( iCurrentMedical > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
			if( iCurrentMedical == 0)
				{
					if( DoWeHaveThisManyBonusPoints( iMaxZeroBonus ) == TRUE )
					{
					iCurrentMedical+=iMedicalMinimum;
					iCurrentBonusPoints-=iMaxZeroBonus;
						fSkillAtZeroWarning = FALSE;
					}
					else
					{
						return( SLIDER_OK );
					}
				}
				else
				{
				iCurrentMedical++;
			 iCurrentBonusPoints--;
				}
			}
		break;
		case( EXPLOSIVE_SKILL ):
		if( iCurrentExplosives > iMaxAttribute -1 )
			{
				// too high, leave
				return( SLIDER_OUT_OF_RANGE );
		}
		else
			{
				if( iCurrentExplosives == 0)
				{
					if( DoWeHaveThisManyBonusPoints( iMaxZeroBonus ) == TRUE )
					{
					iCurrentExplosives+=iExplosivesMinimum;
					iCurrentBonusPoints-=iMaxZeroBonus;
						fSkillAtZeroWarning = FALSE;
					}
					else
					{
						return( SLIDER_OK );
					}
				}
				else
				{
				iCurrentExplosives++;
			 iCurrentBonusPoints--;
				}
			}
		break;

	}

	return( SLIDER_OK );
}


UINT8 DecrementStat( INT32 iStatToDecrement	)
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxZeroBonus = gGameExternalOptions.iMaxZeroBonus;


	// review mode, do not allow changes
	if( fReviewStats )
	{
		return( SLIDER_ERROR );
	}

	// decrement a stat
	// check to make sure stat isn't maxed out already
	switch( iStatToDecrement )
	{
		case( STRENGTH_ATTRIBUTE ):
			if( iCurrentStrength > iStrengthMinimum )
			{
				// ok to decrement
				iCurrentStrength--;
				iCurrentBonusPoints++;
			}
			else
			{
				return( SLIDER_OUT_OF_RANGE );
			}
		break;
		case( DEXTERITY_ATTRIBUTE ):
			if( iCurrentDexterity > iDexterityMinimum )
			{
				// ok to decrement
				iCurrentDexterity--;
				iCurrentBonusPoints++;
			}
			else
			{
				return( SLIDER_OUT_OF_RANGE );
			}
		break;
		case( AGILITY_ATTRIBUTE ):
			if( iCurrentAgility > iAgilityMinimum )
			{
				// ok to decrement
				iCurrentAgility--;
				iCurrentBonusPoints++;
			}
			else
			{
				return( SLIDER_OUT_OF_RANGE );
			}
		break;
		case( WISDOM_ATTRIBUTE ):
			if( iCurrentWisdom > iWisdomMinimum )
			{
				// ok to decrement
				iCurrentWisdom--;
				iCurrentBonusPoints++;
			}
			else
			{
				return( SLIDER_OUT_OF_RANGE );
			}
		break;
		case( LEADERSHIP_ATTRIBUTE ):
			if( iCurrentLeaderShip > iLeaderShipMinimum )
			{
				// ok to decrement
				iCurrentLeaderShip--;
				iCurrentBonusPoints++;
			}
			//Kaiden: Leadership couldn't be set to zero fixed below:
			else if( iCurrentLeaderShip == iLeaderShipMinimum)
			{
				if (LeadershipRequiredDueToMajorSkills() == 0 && LeaderShipRequiredAdjustmentForMinorTraits( 0 ) == 0 )
				{
					// ok to decrement
					iCurrentLeaderShip-=iLeaderShipMinimum;
					iCurrentBonusPoints+=iMaxZeroBonus;
					fSkillAtZeroWarning = TRUE;
				}
			}
		break;
		case( HEALTH_ATTRIBUTE ):
			if( iCurrentHealth > iHealthMinimum )
			{
				// ok to decrement
				iCurrentHealth--;
				iCurrentBonusPoints++;
			}
			else
			{
				return( SLIDER_OUT_OF_RANGE );
			}
		break;
		case( MARKSMANSHIP_SKILL ):
			if( iCurrentMarkmanship > iMarkmanshipMinimum )
			{
				// ok to decrement
				iCurrentMarkmanship--;
				iCurrentBonusPoints++;
			}
			else if( iCurrentMarkmanship == iMarkmanshipMinimum)
			{
				if (MarksmanshipRequiredDueToMajorSkills() == 0 && MarksmanshipRequiredAdjustmentForMinorTraits( 0 ) == 0 )
				{
					// ok to decrement
					iCurrentMarkmanship-=iMarkmanshipMinimum;
					iCurrentBonusPoints+=iMaxZeroBonus;
					fSkillAtZeroWarning = TRUE;
				}
			}
		break;
		case( MEDICAL_SKILL ):
			if( iCurrentMedical > iMedicalMinimum )
			{
				// ok to decrement
				iCurrentMedical--;
				iCurrentBonusPoints++;
			}
			else if( iCurrentMedical == iMedicalMinimum)
			{
				if (MedicalRequiredDueToMajorSkills() == 0 && MedicalRequiredAdjustmentForMinorTraits( 0 ) == 0 )
				{
					// ok to decrement
					iCurrentMedical-=iMedicalMinimum;
					iCurrentBonusPoints+=iMaxZeroBonus;
					fSkillAtZeroWarning = TRUE;
				}
			}
		break;
		case( MECHANICAL_SKILL ):
			if( iCurrentMechanical > iMechanicalMinimum )
			{
				// ok to decrement
				iCurrentMechanical--;
				iCurrentBonusPoints++;
			}
			else if( iCurrentMechanical == iMechanicalMinimum)
			{
				if (MechanicalRequiredDueToMajorSkills() == 0 && MechanicalRequiredAdjustmentForMinorTraits( 0 ) == 0 )
				{
					// ok to decrement
					iCurrentMechanical-=iMechanicalMinimum;
					iCurrentBonusPoints+=iMaxZeroBonus;
					fSkillAtZeroWarning = TRUE;
				}
			}
		break;
		case( EXPLOSIVE_SKILL ):
			if( iCurrentExplosives > iExplosivesMinimum )
			{
				// ok to decrement
				iCurrentExplosives--;
				iCurrentBonusPoints++;
			}
			else if( iCurrentExplosives == iExplosivesMinimum)
			{
				if (ExplosivesRequiredDueToMajorSkills() == 0 && ExplosivesRequiredAdjustmentForMinorTraits( 0 ) == 0 )
				{
					// ok to decrement
					iCurrentExplosives-=iExplosivesMinimum;
					iCurrentBonusPoints+=iMaxZeroBonus;
					fSkillAtZeroWarning = TRUE;
				}
			}
		break;


	}

	if( fSkillAtZeroWarning == TRUE )
	{
		// current stat at zero
		iCurrentStatAtZero = iStatToDecrement;
	}

	return( SLIDER_OK );
}

BOOLEAN DoWeHaveThisManyBonusPoints( INT32 iBonusPoints )
{

	// returns if player has at least this many bonus points
	if( iCurrentBonusPoints >= iBonusPoints )
	{
		// yep, return true
		return ( TRUE );
	}
	else
	{
		// nope, return false
		return ( FALSE );
	}
}



void CreateIMPAttributeSelectionButtons( void )
{

	// the finished button
	giIMPAttributeSelectionButtonImage[0]=	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
/*	giIMPAttributeSelectionButton[0] = QuickCreateButton( giIMPAttributeSelectionButtonImage[0], LAPTOP_SCREEN_UL_X +	( 136 ), LAPTOP_SCREEN_WEB_UL_Y + ( 314 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPAttributeFinishCallback );
	*/
	giIMPAttributeSelectionButton[0] = CreateIconAndTextButton( giIMPAttributeSelectionButtonImage[0], pImpButtonText[ 11 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPAttributeFinishCallback);



	SetButtonCursor(giIMPAttributeSelectionButton[0], CURSOR_WWW);
	return;
}

void DestroyIMPAttributeSelectionButtons( void )
{
	// this function will destroy the buttons needed for the IMP attrib enter page

	// the begin	button
	RemoveButton(giIMPAttributeSelectionButton[ 0 ] );
	UnloadButtonImage(giIMPAttributeSelectionButtonImage[ 0 ] );

	// Destroy also starting level buttons
	RemoveButton(giIMPStartingLevelButton[ 0 ] );
	RemoveButton(giIMPStartingLevelButton[ 1 ] );
	UnloadButtonImage(giIMPStartingLevelButtonImage[ 0 ] );
	UnloadButtonImage(giIMPStartingLevelButtonImage[ 1 ] );

 	return;
}


void BtnIMPAttributeFinishCallback(GUI_BUTTON *btn,INT32 reason)
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

			//iCurrentImpPage = IMP_MAIN_PAGE;

			// SANDRO WAS HERE - THE FINISH PAGE AFTER ATTRIBUTES IS WORTHLESS AND HAVE BEEN CUT OUT
			//if ( fReviewStats == TRUE )
			//{
				iCurrentImpPage = IMP_FINISH;
			//}
			//else
			//{
				iCurrentProfileMode = IMP__FINISH; // All done
			//}

			// SET ATTRIBUTES NOW
			SetGeneratedCharacterAttributes( );
			//fButtonPendingFlag = TRUE;
		}
	}
}

void CreateIMPStartingLevelSelectionButtons( void )
{
	giIMPStartingLevelButtonImage[ 0 ]=	LoadButtonImage( "LAPTOP\\AttributeArrows.sti" ,-1,0,-1,1,-1 );
	giIMPStartingLevelButtonImage[ 1 ]=	LoadButtonImage( "LAPTOP\\AttributeArrows.sti" ,-1,3,-1,4,-1 );


	// left button - decrement level
	giIMPStartingLevelButton[ 0 ] = QuickCreateButton( giIMPStartingLevelButtonImage[ 0 ], STARTING_LEVEL_BOX_POS_X + LAPTOP_SCREEN_UL_X + 112, STARTING_LEVEL_BOX_POS_Y + LAPTOP_SCREEN_WEB_UL_Y + 3,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPLevelSelectionLeftCallback );

	// right button - increment level
	giIMPStartingLevelButton[ 1 ] = QuickCreateButton( giIMPStartingLevelButtonImage[ 1 ], STARTING_LEVEL_BOX_POS_X + LAPTOP_SCREEN_UL_X + 166, STARTING_LEVEL_BOX_POS_Y + LAPTOP_SCREEN_WEB_UL_Y + 3,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPLevelSelectionRightCallback );

	SetButtonCursor(giIMPStartingLevelButton[0], CURSOR_WWW);
	SetButtonCursor(giIMPStartingLevelButton[1 ], CURSOR_WWW);

	// set user data
	MSYS_SetBtnUserData(giIMPStartingLevelButton[0],0, 0 / 2 );
	MSYS_SetBtnUserData(giIMPStartingLevelButton[1],0, 1 / 2 );

	//Get rid of playing the button sound, it will be handled here
	//ButtonList[ giIMPStartingLevelButton[0] ]->ubSoundSchemeID = 0;
	//ButtonList[ giIMPStartingLevelButton[1] ]->ubSoundSchemeID = 0;

	MarkButtonsDirty( );
}
void BtnIMPLevelSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( (iStartingLevel > 1) && !fReviewStats )
		{
			PlayButtonSound( giIMPStartingLevelButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBonusPoints += iBonusPointsRecievedForLevelDown();
			iStartingLevel--;

			fHasAnySlidingBarMoved = TRUE;
		}
		else
		{
			PlayButtonSound( giIMPStartingLevelButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( (iStartingLevel > 1) && !fReviewStats )
		{
			PlayButtonSound( giIMPStartingLevelButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBonusPoints += iBonusPointsRecievedForLevelDown();
			iStartingLevel--;

			fHasAnySlidingBarMoved = TRUE;
		}
		else
		{
			PlayButtonSound( giIMPStartingLevelButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}
void BtnIMPLevelSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ((iBonusPointsNeededForLevelUp() <= iCurrentBonusPoints) && ( iStartingLevel < 10 ) && !fReviewStats )
		{
			PlayButtonSound( giIMPStartingLevelButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBonusPoints -= iBonusPointsNeededForLevelUp();
			iStartingLevel++;

			fHasAnySlidingBarMoved = TRUE;
		}
		else
		{
			PlayButtonSound( giIMPStartingLevelButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ((iBonusPointsNeededForLevelUp() <= iCurrentBonusPoints) && ( iStartingLevel < 10 ) && !fReviewStats )
		{
			PlayButtonSound( giIMPStartingLevelButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBonusPoints -= iBonusPointsNeededForLevelUp();
			iStartingLevel++;

			fHasAnySlidingBarMoved = TRUE;
		}
		else
		{
			PlayButtonSound( giIMPStartingLevelButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

INT32 iBonusPointsNeededForLevelUp()
{
	INT32 iBonusPointsNeeded = 0; 

	if (iStartingLevel > 0 && iStartingLevel < 10)
	{
		iBonusPointsNeeded = (iStartingLevel + 1) * iLevelCostMultiplier;
	}

	return( iBonusPointsNeeded );

}

INT32 iBonusPointsRecievedForLevelDown()
{
	INT32 iBonusPointsRecieved=0; 

	if (iStartingLevel > 1 && iStartingLevel <= 10)
	{
		iBonusPointsRecieved = iStartingLevel * iLevelCostMultiplier;
	}

	return( iBonusPointsRecieved );
}
void RenderAttributeBoxes( void )
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;


	// this function will render the boxes in the sliding attribute bar, based on position
	INT32 iCnt=STRENGTH_ATTRIBUTE;
	INT16 sX = 0;
	INT16 sY = 0;
	CHAR16 sString[4];

	// set last char to null
	sString[2] = 0;

	// font stuff
	SetFont(FONT10ARIAL);
	SetFontShadow(NO_SHADOW);
	SetFontForeground(FONT_WHITE);
	SetFontBackground(FONT_BLACK);

	// run through and render each slider bar
	for( iCnt = HEALTH_ATTRIBUTE; iCnt <= MECHANICAL_SKILL; iCnt++)
	{
		// position is	( width *	( stat - 35 ) ) /50]
		// unless 0, then it is 0 - for skills

		// get y position
		sY = SKILL_SLIDE_START_Y + SKILL_SLIDE_HEIGHT * ( ( INT16 )iCnt ) ;

		switch( iCnt )
		{
		case (STRENGTH_ATTRIBUTE):
		 // blt in strength slider
				sX =	DetermineNewPosition( iCurrentStrength - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentStrength );
				sX += LAPTOP_SCREEN_UL_X;
				sY += LAPTOP_SCREEN_WEB_UL_Y;
				mprintf(sX + 13, sY + 3, sString);
		break;
			case (DEXTERITY_ATTRIBUTE):
		 // blt in strength slider
				sX =	DetermineNewPosition( iCurrentDexterity - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentDexterity );
			sX += LAPTOP_SCREEN_UL_X;
				sY += LAPTOP_SCREEN_WEB_UL_Y;
				mprintf(sX + 13, sY + 3, sString);

		break;
			case (AGILITY_ATTRIBUTE):
		 // blt in strength slider
				sX =	DetermineNewPosition( iCurrentAgility - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
			MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentAgility );
				sX += LAPTOP_SCREEN_UL_X;
				sY += LAPTOP_SCREEN_WEB_UL_Y;
				mprintf(sX + 13, sY + 3, sString);

		break;
			case (WISDOM_ATTRIBUTE):
		 // blt in strength slider
				sX =	DetermineNewPosition( iCurrentWisdom - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentWisdom );
				sX += LAPTOP_SCREEN_UL_X;
				sY += LAPTOP_SCREEN_WEB_UL_Y;
				mprintf(sX + 13, sY + 3, sString);
		break;
		case (LEADERSHIP_ATTRIBUTE):
		 // blt in strength slider
				sX =	DetermineNewPosition( iCurrentLeaderShip - iMinAttribute );
				RenderSliderBar( sX, sY );
		// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentLeaderShip );
				sX += LAPTOP_SCREEN_UL_X;
				sY += LAPTOP_SCREEN_WEB_UL_Y;
				mprintf(sX + 13 , sY + 3, sString);
		break;
			case (HEALTH_ATTRIBUTE):
		 // blt in health slider
				sX =	DetermineNewPosition( iCurrentHealth - iMinAttribute );
				RenderSliderBar( sX, sY );

		// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentHealth );
			sY += LAPTOP_SCREEN_WEB_UL_Y;
				sX += LAPTOP_SCREEN_UL_X;
				mprintf(sX + 13, sY + 3, sString);
		break;
		case (MARKSMANSHIP_SKILL):
		 // blt in marksmanship slider

				sX =	DetermineNewPosition( iCurrentMarkmanship - iMinAttribute );
				RenderSliderBar( sX, sY );
		// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );


				// the text
				swprintf( sString, L"%d", iCurrentMarkmanship );
			sY += LAPTOP_SCREEN_WEB_UL_Y;
				sX += LAPTOP_SCREEN_UL_X;
				mprintf(sX + 13, sY + 3, sString);
		break;
		case (MEDICAL_SKILL):
		 // blt in medical slider

				sX =	DetermineNewPosition( iCurrentMedical - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentMedical );
			sY += LAPTOP_SCREEN_WEB_UL_Y;
				sX += LAPTOP_SCREEN_UL_X;
				mprintf(sX + 13, sY + 3, sString);
		break;
		case (MECHANICAL_SKILL):
		 // blt in mech slider

				sX =	DetermineNewPosition( iCurrentMechanical - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );


				// the text
				swprintf( sString, L"%d", iCurrentMechanical );
			sY += LAPTOP_SCREEN_WEB_UL_Y;
				sX += LAPTOP_SCREEN_UL_X;
				mprintf(sX + 13, sY + 3, sString);
		break;
			case (EXPLOSIVE_SKILL):
		 // blt in explosive slider

				sX =	DetermineNewPosition( iCurrentExplosives - iMinAttribute );
				RenderSliderBar( sX, sY );

				// set sliderbar mouse region
				MSYS_MoveMouseRegionTo( &pSliderBarRegions[ iCnt ], ( INT16 )(sX + LAPTOP_SCREEN_UL_X ), ( INT16 )( sY + LAPTOP_SCREEN_WEB_UL_Y ) );

				// the text
				swprintf( sString, L"%d", iCurrentExplosives );
			sY += LAPTOP_SCREEN_WEB_UL_Y;
				sX += LAPTOP_SCREEN_UL_X;
				mprintf(sX + 13, sY + 3, sString);
		break;

		}
	}

		// reset shadow
	SetFontShadow(DEFAULT_SHADOW);

	return;
}



void CreateAttributeSliderButtons( void )
{
	// this function will create the buttons for the attribute slider
	// the finished button
	INT32 iCounter =0;

	giIMPAttributeSelectionSliderButtonImage[ 0 ]=	LoadButtonImage( "LAPTOP\\AttributeArrows.sti" ,-1,0,-1,1,-1 );
	giIMPAttributeSelectionSliderButtonImage[ 1 ]=	LoadButtonImage( "LAPTOP\\AttributeArrows.sti" ,-1,3,-1,4,-1 );

	for(iCounter = 0; iCounter < 20; iCounter+=2 )
	{
	// left button - decrement stat
	giIMPAttributeSelectionSliderButton[ iCounter ] = QuickCreateButton( giIMPAttributeSelectionSliderButtonImage[ 0 ], LAPTOP_SCREEN_UL_X +	( 163 ), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + ( 99 + iCounter / 2 * 20 ) ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPAttributeSliderLeftCallback );

	// right button - increment stat
	giIMPAttributeSelectionSliderButton[ iCounter + 1 ] = QuickCreateButton( giIMPAttributeSelectionSliderButtonImage[ 1 ], LAPTOP_SCREEN_UL_X +	( 420 ), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + ( 99 + iCounter / 2 * 20 ) ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPAttributeSliderRightCallback );

		SetButtonCursor(giIMPAttributeSelectionSliderButton[iCounter], CURSOR_WWW);
		SetButtonCursor(giIMPAttributeSelectionSliderButton[iCounter + 1 ], CURSOR_WWW);
		// set user data
		MSYS_SetBtnUserData(giIMPAttributeSelectionSliderButton[iCounter],0, iCounter / 2 );
		MSYS_SetBtnUserData(giIMPAttributeSelectionSliderButton[iCounter + 1],0, iCounter / 2 );
	}

	MarkButtonsDirty( );
}



void DestroyAttributeSliderButtons( void )
{
	// this function will destroy the buttons used for attribute manipulation
	INT32 iCounter =0;

	// get rid of image
	UnloadButtonImage(giIMPAttributeSelectionSliderButtonImage[ 0 ] );
	UnloadButtonImage(giIMPAttributeSelectionSliderButtonImage[ 1 ] );

	for(iCounter = 0; iCounter < 20; iCounter++ )
	{
		// get rid of button
	 RemoveButton(giIMPAttributeSelectionSliderButton[ iCounter ] );

	}

	return;
}


void BtnIMPAttributeSliderLeftCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32 iValue = -1;

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;


	iValue = (INT32)MSYS_GetBtnUserData( btn, 0 );

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		DecrementStat( iValue );
			// stat has changed, rerender
		fHasAnySlidingBarMoved = TRUE;
			uiBarToReRender = iValue;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

	DecrementStat( iValue );
		fHasAnySlidingBarMoved = TRUE;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	uiBarToReRender = iValue;
	}

	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		}
	}
}


void BtnIMPAttributeSliderRightCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32 iValue = -1;

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	iValue = (INT32)MSYS_GetBtnUserData( btn, 0 );

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		IncrementStat( iValue	);
			// stat has changed, rerender
		fHasAnySlidingBarMoved = TRUE;
			uiBarToReRender = iValue;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

	IncrementStat( iValue );
		fHasAnySlidingBarMoved = TRUE;
		uiBarToReRender = iValue;
		btn->uiFlags|=(BUTTON_CLICKED_ON);

	}

	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		}
	}
}



void CreateSlideRegionMouseRegions( void )
{
	// this function will create that mouse regions on the sliding area, that, if the player clicks on, the bar will automatically jump to
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 10; iCounter++ )
	{
		// define the region
		MSYS_DefineRegion( &pSliderRegions[ iCounter ], ( INT16 ) ( SKILL_SLIDE_START_X + LAPTOP_SCREEN_UL_X), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + SKILL_SLIDE_START_Y + iCounter * SKILL_SLIDE_HEIGHT ) , ( INT16 ) ( LAPTOP_SCREEN_UL_X + SKILL_SLIDE_START_X + BAR_WIDTH ), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + SKILL_SLIDE_START_Y + iCounter * SKILL_SLIDE_HEIGHT + 15 ),
			MSYS_PRIORITY_HIGH + 2, CURSOR_WWW, MSYS_NO_CALLBACK, SliderRegionButtonCallback);

		// define user data
			MSYS_SetRegionUserData(&pSliderRegions[iCounter],0,iCounter);
		// now add it
		MSYS_AddRegion(&pSliderRegions[ iCounter ]);
	}

	return;
}

void CreateSliderBarMouseRegions( void )
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;

	// this function will create that mouse regions on the sliding bars, that, if the player clicks on, the bar will automatically jump to
	INT32 iCounter = 0;
	INT16 sX = 0;

	// set the starting X
	// SANDRO - some changes in attribute points distribution method
	//sX = DetermineNewPosition( gGameExternalOptions.iStartAttribute - iMinAttribute );
	sX = DetermineNewPosition( 0 );

	for( iCounter = 0; iCounter < 10; iCounter++ )
	{
		// define the region
		MSYS_DefineRegion( &pSliderBarRegions[ iCounter ], ( INT16 ) ( sX ), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + SKILL_SLIDE_START_Y + iCounter * SKILL_SLIDE_HEIGHT ) , ( INT16 ) (sX + SLIDER_BAR_WIDTH ), ( INT16 ) ( LAPTOP_SCREEN_WEB_UL_Y + SKILL_SLIDE_START_Y + iCounter * SKILL_SLIDE_HEIGHT + 15 ),
			MSYS_PRIORITY_HIGH + 2, CURSOR_WWW, MSYS_NO_CALLBACK, SliderBarRegionButtonCallback);

		// define user data
			MSYS_SetRegionUserData(&pSliderBarRegions[iCounter],0,iCounter);
		// now add it
		MSYS_AddRegion(&pSliderBarRegions[ iCounter ]);
	}

	return;
}


void DestroySlideRegionMouseRegions( void )
{
	// this function will destroy the regions user for the slider ' jumping'
	INT32 iCounter=0;

	// delete the regions
	for( iCounter=0; iCounter < 10; iCounter++ )
	{
	MSYS_RemoveRegion( &pSliderRegions[ iCounter ] );
	}

	return;
}


void DestroySlideBarMouseRegions( void )
{
	// this function will destroy the regions user for the slider ' jumping'
	INT32 iCounter=0;

	// delete the regions
	for( iCounter=0; iCounter < 10; iCounter++ )
	{
	MSYS_RemoveRegion( &pSliderBarRegions[ iCounter ] );
	}

	return;
}


void SliderRegionButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxAttribute = gGameExternalOptions.iMaxAttribute;

	INT32 iCurrentAttributeValue = 0;
	INT32 iNewAttributeValue = 0;
	INT32 iAttributeDelta = 0;
	INT32 iCounter =0;
	INT16 sX =0;
	INT16 sY = 0;
	static INT16 sOldX = -1;
	static INT16 sOldY = -1;
	static INT32 iAttribute = -1;
	INT32 iNewValue = 0;
	INT16 sNewX = -1;


	//if we already have an anchored slider bar
	if( gpCurrentScrollBox != pRegion && gpCurrentScrollBox != NULL )
		return;


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT)
	{
		if( fSlideIsActive == FALSE)
		{
			// not active leave
		return;
		}


	// check to see if we have moved
	if( MSYS_GetRegionUserData(pRegion, 0) != iAttribute )
		{
			// different regions
			iAttribute = MSYS_GetRegionUserData(pRegion, 0);
		sOldX = -1;
			sOldY = -1;
		return;
		}

		uiBarToReRender = iAttribute;

		giCurrentlySelectedStat = iAttribute;
		gpCurrentScrollBox = pRegion;

	// get new attribute value x
		sNewX = pRegion->MouseXPos;

		// sOldX has been reset, set to sNewX
		if( sOldX == -1)
		{
			sOldX = sNewX;
		return;
		}
		// check against old x
		if( sNewX != sOldX )
		{

			// get old stat value
		iCurrentAttributeValue = GetCurrentAttributeValue( iAttribute );
			iNewValue = DetermineNewValue( sNewX );
			// chenged, move mouse region if change large enough
		if( iCurrentAttributeValue != iNewValue )
			{
		 // update screen
		 fHasAnySlidingBarMoved = TRUE;
			}

		// change is enough
		if ( iNewValue - iCurrentAttributeValue > 0)
			{
				// positive, increment stat
		iCounter = iNewValue - iCurrentAttributeValue;
				for( iCounter; iCounter > 0; iCounter--)
				{
					IncrementStat( iAttribute );
				}
			}
			else
			{
				// negative, decrement stat
		iCounter = iCurrentAttributeValue - iNewValue;
				for( iCounter; iCounter > 0; iCounter--)
				{
					DecrementStat( iAttribute );
				}
			}

			sOldX = sNewX;
		}
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{

		if( fSlideIsActive )
		{
		// reset slide is active flag
		fSlideIsActive = FALSE;
			return;
		}


		// get mouse XY

		sX = pRegion->MouseXPos;
		sY = pRegion->MouseYPos;

	// which region are we in?

		// get attribute
		iAttribute = MSYS_GetRegionUserData(pRegion, 0);
	uiBarToReRender = iAttribute;

		// get value of attribute
		iCurrentAttributeValue = GetCurrentAttributeValue( iAttribute );

		// set the new attribute value based on position of mouse click
		iNewAttributeValue = DetermineNewValue( sX );

		// too high, reset to MAX_ATTIBUTEPOINT
		if( iNewAttributeValue > iMaxAttribute )
	{
			iNewAttributeValue = iMaxAttribute;
		}

		// get the delta
		iAttributeDelta = iCurrentAttributeValue - iNewAttributeValue;

		// set Counter
		iCounter = iAttributeDelta;

		// check if increment or decrement
		if( iAttributeDelta > 0)
		{
			// decrement
			for( iCounter = 0; iCounter < iAttributeDelta; iCounter++ )
			{
		DecrementStat( iAttribute );
			}
		}
		else
		{
			// increment attribute
		for( iCounter = iAttributeDelta; iCounter < 0; iCounter++ )
			{
				if( iCurrentAttributeValue == 0)
				{
					iCounter = 0;
				}
		IncrementStat( iAttribute );
			}
		}

		// update screen
	fHasAnySlidingBarMoved = TRUE;


	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{


	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

		// get mouse positions
		sX = pRegion->MouseXPos;
		sY = pRegion->MouseYPos;

		// get attribute
		iAttribute = MSYS_GetRegionUserData(pRegion, 0);
	uiBarToReRender = iAttribute;

		// get value of attribute
		iCurrentAttributeValue = GetCurrentAttributeValue( iAttribute );

		// get the boxes bounding x
		sNewX = DetermineNewPosition( iCurrentAttributeValue - iMinAttribute );
		sNewX += LAPTOP_SCREEN_UL_X;

		// the sNewX is below 0, reset to zero
		if( sNewX < 0 )
		{
			sNewX = 0;
		}

		if( ( sX > sNewX )&&( sX < sNewX + SLIDER_BAR_WIDTH) )
		{
			// we are within the slide bar, set fact we want to drag and draw
		fSlideIsActive = TRUE;
		}
		else
		{
			// otherwise want to jump to position
			fSlideIsActive =FALSE;
		}
	}

}

void SliderBarRegionButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{


	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	fSlideIsActive = TRUE;
		SliderRegionButtonCallback( &pSliderRegions[ MSYS_GetRegionUserData(pRegion, 0) ], MSYS_CALLBACK_REASON_LBUTTON_REPEAT);
	}
	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		fSlideIsActive = FALSE;
	}

}

INT32 GetCurrentAttributeValue( INT32 iAttribute )
{
	// this function will get the value of the attribute that was passed to this fucntion via iAttribute
	INT32 iValue =0;

	switch( iAttribute )
	{
		case ( STRENGTH_ATTRIBUTE ):
			iValue = iCurrentStrength;
		break;
		case ( DEXTERITY_ATTRIBUTE ):
			iValue = iCurrentDexterity;
		break;
	case ( AGILITY_ATTRIBUTE ):
			iValue = iCurrentAgility;
		break;
	case ( HEALTH_ATTRIBUTE ):
			iValue = iCurrentHealth;
		break;
		case ( WISDOM_ATTRIBUTE ):
			iValue = iCurrentWisdom;
		break;
		case ( LEADERSHIP_ATTRIBUTE ):
			iValue = iCurrentLeaderShip;
		break;
		case ( MARKSMANSHIP_SKILL ):
			iValue = iCurrentMarkmanship;
		break;
		case ( MEDICAL_SKILL ):
			iValue = iCurrentMedical;
		break;
		case ( MECHANICAL_SKILL ):
			iValue = iCurrentMechanical;
		break;
		case ( EXPLOSIVE_SKILL ):
			iValue = iCurrentExplosives;
		break;


	}

	return iValue;
}


void SetAttributes( void )
{
	INT32	iExtraPoints = 0;

	// added externilized multiplier for starting level - SADNRO
	iLevelCostMultiplier = gGameExternalOptions.iIMPStartingLevelCostMultiplier;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - determine minimum attribute values due to chosen traits

	// For new trait system only though
	if ( gGameOptions.fNewTraitSystem )
	{
		// reset bonus pts
		iCurrentBonusPoints = gGameExternalOptions.iImpAttributePoints;
		//Determine if the player has any extra points
		iExtraPoints += iPlayersAttributePointsBonusForDisabilitySelected();
		iExtraPoints += DoesPlayerHaveExtraAttibutePointsToDistributeBasedOnSkillSelection();

		if( iExtraPoints > 0 )
			iCurrentBonusPoints += iExtraPoints;

		// Get Strength minimum
		iStrengthMinimum = StrengthRequiredDueToMajorSkills();
		if ( iStrengthMinimum < gGameExternalOptions.iMinAttribute )
			iStrengthMinimum = gGameExternalOptions.iMinAttribute;

		iStrengthMinimum += StrengthRequiredAdjustmentForMinorTraits( iStrengthMinimum );

		// hehehe, make the alternative weapon holding anim requiring a big strength
		if ( bBadAssSelected() && iStrengthMinimum < 80 )
			iStrengthMinimum = 80;

		if ( iStrengthMinimum > gGameExternalOptions.iMaxAttribute )
			iStrengthMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentStrength = iStrengthMinimum; 
		iCurrentBonusPoints -= iStrengthMinimum;

		// Get Agility minimum
		iAgilityMinimum = AgilityRequiredDueToMajorSkills();
		if ( iAgilityMinimum < gGameExternalOptions.iMinAttribute )
			iAgilityMinimum = gGameExternalOptions.iMinAttribute;

		iAgilityMinimum += AgilityRequiredAdjustmentForMinorTraits( iAgilityMinimum );

		if ( iAgilityMinimum > gGameExternalOptions.iMaxAttribute )
			iAgilityMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentAgility = iAgilityMinimum; 
		iCurrentBonusPoints -= iAgilityMinimum;

		// Get Dexterity minimum
		iDexterityMinimum = DexterityRequiredDueToMajorSkills();
		if ( iDexterityMinimum < gGameExternalOptions.iMinAttribute )
			iDexterityMinimum = gGameExternalOptions.iMinAttribute;

		iDexterityMinimum += DexterityRequiredAdjustmentForMinorTraits( iDexterityMinimum );

		if ( iDexterityMinimum > gGameExternalOptions.iMaxAttribute )
			iDexterityMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentDexterity = iDexterityMinimum; 
		iCurrentBonusPoints -= iDexterityMinimum;

		// Get Health minimum
		iHealthMinimum = HealthRequiredDueToMajorSkills();
		if ( iHealthMinimum <= 0 )
			iHealthMinimum = gGameExternalOptions.iMinAttribute;

		iHealthMinimum += HealthRequiredAdjustmentForMinorTraits( iHealthMinimum );

		if ( iHealthMinimum > gGameExternalOptions.iMaxAttribute )
			iHealthMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentHealth = iHealthMinimum; 
		iCurrentBonusPoints -= iHealthMinimum;

		// Get Leadership minimum
		iLeaderShipMinimum = LeadershipRequiredDueToMajorSkills();
		if ( iLeaderShipMinimum < gGameExternalOptions.iMinAttribute )
			iLeaderShipMinimum = gGameExternalOptions.iMinAttribute;

		iLeaderShipMinimum += LeaderShipRequiredAdjustmentForMinorTraits( iLeaderShipMinimum );

		if ( iLeaderShipMinimum > gGameExternalOptions.iMaxAttribute )
			iLeaderShipMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentLeaderShip = iLeaderShipMinimum; 
		iCurrentBonusPoints -= (iLeaderShipMinimum - gGameExternalOptions.iMinAttribute + gGameExternalOptions.iMaxZeroBonus);

		// Get Wisdom minimum
		iWisdomMinimum = WisdomRequiredDueToMajorSkills();
		if ( iWisdomMinimum < gGameExternalOptions.iMinAttribute )
			iWisdomMinimum = gGameExternalOptions.iMinAttribute;

		iWisdomMinimum += WisdomRequiredAdjustmentForMinorTraits( iWisdomMinimum );

		if ( iWisdomMinimum > gGameExternalOptions.iMaxAttribute )
			iWisdomMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentWisdom = iWisdomMinimum; 
		iCurrentBonusPoints -= iWisdomMinimum;

		// Get Marksmanship minimum
		iMarkmanshipMinimum = MarksmanshipRequiredDueToMajorSkills();
		if ( iMarkmanshipMinimum < gGameExternalOptions.iMinAttribute )
			iMarkmanshipMinimum = gGameExternalOptions.iMinAttribute;
		
		iMarkmanshipMinimum += MarksmanshipRequiredAdjustmentForMinorTraits( iMarkmanshipMinimum );

		if ( iMarkmanshipMinimum > gGameExternalOptions.iMaxAttribute )
			iMarkmanshipMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentMarkmanship = iMarkmanshipMinimum; 
		iCurrentBonusPoints -= (iMarkmanshipMinimum - gGameExternalOptions.iMinAttribute + gGameExternalOptions.iMaxZeroBonus);

		// Get Mechanical minimum
		iMechanicalMinimum = MechanicalRequiredDueToMajorSkills();
		if ( iMechanicalMinimum < gGameExternalOptions.iMinAttribute )
			iMechanicalMinimum = gGameExternalOptions.iMinAttribute;

		iMechanicalMinimum += MechanicalRequiredAdjustmentForMinorTraits( iMechanicalMinimum );

		if ( iMechanicalMinimum > gGameExternalOptions.iMaxAttribute )
			iMechanicalMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentMechanical = iMechanicalMinimum; 
		iCurrentBonusPoints -= (iMechanicalMinimum - gGameExternalOptions.iMinAttribute + gGameExternalOptions.iMaxZeroBonus);

		// Get Medical minimum
		iMedicalMinimum = MedicalRequiredDueToMajorSkills();
		if ( iMedicalMinimum < gGameExternalOptions.iMinAttribute )
			iMedicalMinimum = gGameExternalOptions.iMinAttribute;

		iMedicalMinimum += MedicalRequiredAdjustmentForMinorTraits( iMedicalMinimum );

		if ( iMedicalMinimum > gGameExternalOptions.iMaxAttribute )
			iMedicalMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentMedical = iMedicalMinimum; 
		iCurrentBonusPoints -= (iMedicalMinimum - gGameExternalOptions.iMinAttribute + gGameExternalOptions.iMaxZeroBonus);

		// Get Explosives minimum
		iExplosivesMinimum = ExplosivesRequiredDueToMajorSkills();
		if ( iExplosivesMinimum < gGameExternalOptions.iMinAttribute )
			iExplosivesMinimum = gGameExternalOptions.iMinAttribute;

		iExplosivesMinimum += ExplosivesRequiredAdjustmentForMinorTraits( iExplosivesMinimum );

		if ( iExplosivesMinimum > gGameExternalOptions.iMaxAttribute )
			iExplosivesMinimum = gGameExternalOptions.iMaxAttribute;

		iCurrentExplosives = iExplosivesMinimum; 
		iCurrentBonusPoints -= (iExplosivesMinimum - gGameExternalOptions.iMinAttribute + gGameExternalOptions.iMaxZeroBonus);


		// if we exceeded points available, just give us zero points, but keep the attribute minimums untouched
		if ( iCurrentBonusPoints < 0 )
			iCurrentBonusPoints = 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	else // ORIGINAL 
	{
		// default
		iStrengthMinimum = gGameExternalOptions.iMinAttribute;
		iAgilityMinimum	 = gGameExternalOptions.iMinAttribute;
		iDexterityMinimum	= gGameExternalOptions.iMinAttribute;
		iHealthMinimum		= gGameExternalOptions.iMinAttribute;
		iLeaderShipMinimum	= gGameExternalOptions.iMinAttribute;
		iWisdomMinimum		= gGameExternalOptions.iMinAttribute;
		
		iMarkmanshipMinimum = gGameExternalOptions.iMinAttribute;
		iMechanicalMinimum	= gGameExternalOptions.iMinAttribute;
		iMedicalMinimum	 = gGameExternalOptions.iMinAttribute;
		iExplosivesMinimum	= gGameExternalOptions.iMinAttribute;

		iCurrentBonusPoints = gGameExternalOptions.iImpAttributePoints;

		//Determine if the player has any extra points
		iExtraPoints += iPlayersAttributePointsBonusForDisabilitySelected();
		iExtraPoints += DoesPlayerHaveExtraAttibutePointsToDistributeBasedOnSkillSelection();
		if( iExtraPoints > 0 )
			iCurrentBonusPoints += iExtraPoints;

		iCurrentStrength = gGameExternalOptions.iMinAttribute;
		iCurrentDexterity = gGameExternalOptions.iMinAttribute;
		iCurrentHealth = gGameExternalOptions.iMinAttribute;
		iCurrentLeaderShip = gGameExternalOptions.iMinAttribute;
		iCurrentWisdom = gGameExternalOptions.iMinAttribute;
		iCurrentAgility = gGameExternalOptions.iMinAttribute;
		iCurrentMarkmanship = gGameExternalOptions.iMinAttribute;
		iCurrentMechanical = gGameExternalOptions.iMinAttribute;
		iCurrentMedical = gGameExternalOptions.iMinAttribute;
		iCurrentExplosives = gGameExternalOptions.iMinAttribute;
		
		iCurrentBonusPoints -= ((5 * gGameExternalOptions.iMinAttribute) + (5 * gGameExternalOptions.iMaxZeroBonus));
	}
	////////////////////////////////////////////////////

	ResetIncrementCharacterAttributes( );

	return;
}


void DrawBonusPointsRemaining( void )
{
	// draws the amount of points remaining player has
	CHAR16 sString[64];

	// just reviewing, don't blit stats
	if( fReviewStats == TRUE )
	{
	return;
	}
	// parse amountof bns pts remaining
	swprintf(sString, L"%d", iCurrentBonusPoints);

	// set font color
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );
	SetFont( FONT12ARIAL );
	// print string
	mprintf( LAPTOP_SCREEN_UL_X + 425 ,LAPTOP_SCREEN_WEB_UL_Y + 51, sString );

	InvalidateRegion( LAPTOP_SCREEN_UL_X + 425, LAPTOP_SCREEN_WEB_UL_Y + 51, LAPTOP_SCREEN_UL_X + 475, LAPTOP_SCREEN_WEB_UL_Y + 71 );
	return;
}


void SetGeneratedCharacterAttributes( void )
{
	// copies over the attributes of the player generated character
	iStrength = iCurrentStrength	;
		iDexterity = iCurrentDexterity;
		iHealth = iCurrentHealth;
		iLeadership = iCurrentLeaderShip;
		iWisdom = iCurrentWisdom;
		iAgility = iCurrentAgility;

	// skills
	iMarksmanship = iCurrentMarkmanship;
		iMechanical = iCurrentMechanical;
		iMedical = iCurrentMedical;
		iExplosives = iCurrentExplosives;


	return;
}


void StatAtZeroBoxCallBack( UINT8 bExitValue )
{
	// yes, so start over, else stay here and do nothing for now
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		MarkButtonsDirty();
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{
		IncrementStat( iCurrentStatAtZero );
		fHasAnySlidingBarMoved = TRUE;
		MarkButtonsDirty();
	}

	return;
}

INT32 DetermineNewValue( INT32 iNewX )
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxAttribute = gGameExternalOptions.iMaxAttribute;

	INT32 iNewValue=0;
	INT32	iStartLoc = SKILL_SLIDE_START_X + LAPTOP_SCREEN_UL_X;
	INT32 iPositionX = iNewX - iStartLoc;
	FLOAT fPercentOfBar = iPositionX / ( FLOAT)( BAR_WIDTH - SLIDER_BAR_WIDTH );

	iNewValue = (INT32)( fPercentOfBar * ( iMaxAttribute - (FLOAT)iMinAttribute ) );

	iNewValue += iMinAttribute;

	// too high, reset to MAX_ATTIBUTEPOINT
	if( iNewValue > iMaxAttribute )
	{
		iNewValue = iMaxAttribute;
	}

	return( iNewValue );
}


INT32 DetermineNewPosition( INT32 iAttribute )
{

	INT32 iMinAttribute = gGameExternalOptions.iMinAttribute;
	INT32 iMaxAttribute = gGameExternalOptions.iMaxAttribute;

	INT32 iNewLoc=0;
	INT32	iStartLoc = SKILL_SLIDE_START_X;
	FLOAT	fBasePixelScaleWidth = ( ( BAR_WIDTH - SLIDER_BAR_WIDTH ) / (FLOAT)( iMaxAttribute - iMinAttribute ) );

	iNewLoc = (INT32)(iAttribute * fBasePixelScaleWidth);

	if( iNewLoc < 0 )
	{
		iNewLoc = 0;
	}

	iNewLoc += iStartLoc;

	return( iNewLoc );
}

INT8 StartingLevelChosen()
{
	if ( (iStartingLevel > 0) && (iStartingLevel <= 10) )
		return(iStartingLevel);
	else
		return(1);
}
