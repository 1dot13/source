/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Prejudice.h"
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
	#include "DropDown.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define	IMP_PREJUDICE__FONT							FONT12ARIAL
#define	IMP_PREJUDICE__COLOR						FONT_MCOLOR_WHITE

#define	IMP_PREJUDICE__TITLE_X						LAPTOP_SCREEN_UL_X - 111
#define	IMP_PREJUDICE__TITLE_Y						iScreenHeightOffset + 53
#define	IMP_PREJUDICE__TITLE_WIDTH					( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X )

//*******************************************************************
//
// Local Variables
//
//*******************************************************************

BOOLEAN gfIMPPrejudice_Redraw=FALSE;

// this is the Done	buttons
INT32 giIMPPrejudiceFinishButton;
INT32 giIMPPrejudiceFinishButtonImage;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPPrejudiceFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		IMPPrejudiceDisplay();
BOOLEAN		CameBackToPrejudicePageButNotFinished();

//*******************************************************************
//
// Functions
//
//*******************************************************************


//*******************************************************************
//
// DropDown instances
//
//*******************************************************************
#define DROPDOWN_MARKUP_Y	50

enum definedDropDowns
{
	DROPDOWNNR_APPEARANCE = 0,
	DROPDOWNNR_APPEARANCECARE,
	DROPDOWNNR_REFINEMENT,
	DROPDOWNNR_REFINEMENTCARE,
	DROPDOWNNR_NATIONALITY,
	DROPDOWNNR_HATEDNATIONALITY,
	DROPDOWNNR_HATEDNATIONALITYCARE,
	DROPDOWNNR_RACIST,
	DROPDOWNNR_RACE,
	DROPDOWNNR_SEXIST,
	DROPDOWNNR_RACISTFEATURE,
};

template<>	void	DropDownTemplate<DROPDOWNNR_APPEARANCE>::SetRefresh()			{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::SetRefresh()		{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_REFINEMENT>::SetRefresh()			{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::SetRefresh()		{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_NATIONALITY>::SetRefresh()			{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::SetRefresh()		{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_RACIST>::SetRefresh()				{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_RACE>::SetRefresh()					{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_SEXIST>::SetRefresh()				{ gfIMPPrejudice_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_RACISTFEATURE>::SetRefresh()		{ gfIMPPrejudice_Redraw = TRUE; }

void EnterIMPPrejudice( void )
{
	RenderProfileBackGround();
				
	giIMPPrejudiceFinishButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPPrejudiceFinishButton = CreateIconAndTextButton( giIMPPrejudiceFinishButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPrejudiceFinishCallback );
	
	SetButtonCursor( giIMPPrejudiceFinishButton, CURSOR_WWW);

	UINT16 usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOULOOK], FONT12ARIAL ) + 10;
	UINT16 usY = LAPTOP_SCREEN_WEB_UL_Y + 43;
	////////////////// DropDown_Appearance //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Appearance;
	for(UINT8 i = 0; i < NUM_APPEARANCES; ++i)
		entryvecDropDown_Appearance.push_back( std::make_pair<INT16, STR16>(i, szAppearanceText[i]) );
	
	DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().SetEntries(entryvecDropDown_Appearance);
	DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_APPEARANCE] );
	DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().Create(usX, usY);
	////////////////// DropDown_Appearance //////////////////////////

	usX = DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ANDAPPEARANCEIS], FONT12ARIAL ) + 20;
	////////////////// DropDown_AppearanceCare //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_AppearanceCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_AppearanceCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );
	
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().SetEntries(entryvecDropDown_AppearanceCare);
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_APPEARANCECARE] );
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().Create(usX, usY);
	////////////////// DropDown_AppearanceCare //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUHAVE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDown_Refinement //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Refinement;
	for(UINT8 i = 0; i < NUM_REFINEMENT; ++i)
		entryvecDropDown_Refinement.push_back( std::make_pair<INT16, STR16>(i, szRefinementText[i]) );

	DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().SetEntries(entryvecDropDown_Refinement);
	DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_REFINEMENT] );
	DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().Create(usX, usY);
	////////////////// DropDown_Refinement //////////////////////////

	usX = DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ANDCARE], FONT12ARIAL ) + 20;
	////////////////// DropDownTemplate<DROPDOWNNR_REFINEMENTCARE> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_RefinementCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_RefinementCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );

	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().SetEntries(entryvecDropDown_RefinementCare);
	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_REFINEMENTCARE] );
	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_REFINEMENTCARE> //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDownTemplate<DROPDOWNNR_NATIONALITY> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Nationality;
	for(UINT8 i = 0; i < NUM_NATIONALITIES; ++i)
		entryvecDropDown_Nationality.push_back( std::make_pair<INT16, STR16>(i, szNationalityText[i]) );

	DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().SetEntries(entryvecDropDown_Nationality);
	DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_NATIONALITY] );
	DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_NATIONALITY> //////////////////////////

	usX = DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ADHATEEVERYONE], FONT12ARIAL ) + 20;
	////////////////// DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_HatedNationality;
	// we can also hate no nation at all...
	entryvecDropDown_HatedNationality.push_back( std::make_pair<INT16, STR16>(-1, szNationalityText_Special[1]) );

	for(UINT8 i = 0; i < NUM_NATIONALITIES; ++i)
		entryvecDropDown_HatedNationality.push_back( std::make_pair<INT16, STR16>(i, szNationalityText[i]) );

	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().SetEntries(entryvecDropDown_HatedNationality);
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_HATEDNATIONALITY] );
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY> //////////////////////////

	usX = DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().GetLastX() + 5;
	////////////////// DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_HatedNationalityCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_HatedNationalityCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );

	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().SetEntries(entryvecDropDown_HatedNationalityCare);
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_HATEDNATIONALITYCARE] );
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE> //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDownTemplate<DROPDOWNNR_RACIST> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Racist;
	for(UINT8 i = 0; i < NUM_RACIST; ++i)
		entryvecDropDown_Racist.push_back( std::make_pair<INT16, STR16>(i, szRacistText[i]) );

	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().SetEntries(entryvecDropDown_Racist);
	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_RACIST] );
	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_RACIST> //////////////////////////

	usX = DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_RACISTAGAINSTNON], FONT12ARIAL ) + 10;
	////////////////// DropDownTemplate<DROPDOWNNR_RACE> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Race;
	for(UINT8 i = 0; i < NUM_RACES; ++i)
		entryvecDropDown_Race.push_back( std::make_pair<INT16, STR16>(i, szRaceText[i]) );

	DropDownTemplate<DROPDOWNNR_RACE>::getInstance().SetEntries(entryvecDropDown_Race);
	DropDownTemplate<DROPDOWNNR_RACE>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_RACE] );
	DropDownTemplate<DROPDOWNNR_RACE>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_RACE> //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDownTemplate<DROPDOWNNR_SEXIST> //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Sexist;
	for(UINT8 i = 0; i < NUM_SEXIST; ++i)
		entryvecDropDown_Sexist.push_back( std::make_pair<INT16, STR16>(i, szSexistText[i]) );

	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().SetEntries(entryvecDropDown_Sexist);
	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().SetHelpText( szPersonalityHelpText[DROPDOWNNR_SEXIST] );
	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().Create(usX, usY);
	////////////////// DropDownTemplate<DROPDOWNNR_SEXIST> //////////////////////////
}


void RenderIMPPrejudice( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	IMPPrejudiceDisplay();

	DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_RACE>::getInstance().Display();
	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().Display();
}


void ExitIMPPrejudice( void )
{
	DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_RACE>::getInstance().Destroy();
	DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().Destroy();

	RemoveButton( giIMPPrejudiceFinishButton );
	UnloadButtonImage( giIMPPrejudiceFinishButtonImage );
}


void HandleIMPPrejudice( void )
{
	if( gfIMPPrejudice_Redraw )
	{
		RenderIMPPrejudice( );
		gfIMPPrejudice_Redraw = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void IMPPrejudiceDisplay()
{
	UINT16 usPosX, usPosY;

	//Display the title
	DrawTextToScreen( L"I.M.P. Prejudices", IMP_PREJUDICE__TITLE_X, IMP_PREJUDICE__TITLE_Y, IMP_PREJUDICE__TITLE_WIDTH, FONT14ARIAL, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	ColorFillVideoSurfaceArea( FRAME_BUFFER, LAPTOP_SCREEN_UL_X + 2, LAPTOP_SCREEN_WEB_UL_Y + 40, LAPTOP_SCREEN_LR_X - 2,	LAPTOP_SCREEN_WEB_UL_Y + 20 + 5 * DROPDOWN_MARKUP_Y, Get16BPPColor( FROMRGB( 100, 100, 100 ) ) );

	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 50;
		
	// Appearance
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOULOOK], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ANDAPPEARANCEIS], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_IMPORTANTTOYOU], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
				
	// Refinement
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUHAVE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ANDCARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ABOUTTHAT], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Hated Nations
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ADHATEEVERYONE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Racism
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_RACISTAGAINSTNON], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_RACE>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_PEOPLE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Sexism
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_DOT], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
}


void BtnIMPPrejudiceFinishCallback(GUI_BUTTON *btn,INT32 reason)
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
		// Flugente: setting determines wether we see the background page
		else
		{
			// we finished this -> write down our choices
			bRace						= ( INT8)DropDownTemplate<DROPDOWNNR_RACE>::getInstance().GetSelectedEntryKey();
			bNationality				= ( INT8)DropDownTemplate<DROPDOWNNR_NATIONALITY>::getInstance().GetSelectedEntryKey();
			bAppearance					= ( INT8)DropDownTemplate<DROPDOWNNR_APPEARANCE>::getInstance().GetSelectedEntryKey();
			bAppearanceCareLevel		= ( INT8)DropDownTemplate<DROPDOWNNR_APPEARANCECARE>::getInstance().GetSelectedEntryKey();
			bRefinement					= ( INT8)DropDownTemplate<DROPDOWNNR_REFINEMENT>::getInstance().GetSelectedEntryKey();
			bRefinementCareLevel		= ( INT8)DropDownTemplate<DROPDOWNNR_REFINEMENTCARE>::getInstance().GetSelectedEntryKey();
			bHatedNationality			= ( INT8)DropDownTemplate<DROPDOWNNR_HATEDNATIONALITY>::getInstance().GetSelectedEntryKey();
			bHatedNationalityCareLevel	= ( INT8)DropDownTemplate<DROPDOWNNR_HATEDNATIONALITYCARE>::getInstance().GetSelectedEntryKey();
			bRacist						= ( INT8)DropDownTemplate<DROPDOWNNR_RACIST>::getInstance().GetSelectedEntryKey();
			bSexist						= (UINT8)DropDownTemplate<DROPDOWNNR_SEXIST>::getInstance().GetSelectedEntryKey();

			iCurrentImpPage = IMP_MAIN_PAGE;

			if( CameBackToPrejudicePageButNotFinished() )
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

BOOLEAN CameBackToPrejudicePageButNotFinished()
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
