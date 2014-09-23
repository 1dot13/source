/**
* @file
* @author Flugente (bears-pit.com)
*/

#ifdef PRECOMPILEDHEADERS
#include "Laptop All.h"
#include "IMP Skill Trait.h"
#include "_Ja25Englishtext.h"
#else
#include "IMP Gear.h"
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
#include "Interface Items.h"
#include "InterfaceItemImages.h"
#include "IMP Confirm.h"
#include "IMP Skill Trait.h"
#include "IMP Minor Trait.h"
#include "IMP Gear Entrance.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define	IMP_GEAR__FONT						FONT12ARIAL
#define	IMP_GEAR__COLOR						FONT_MCOLOR_WHITE

#define	IMP_GEAR__TITLE_X					LAPTOP_SCREEN_UL_X - 111

#define IMP_GEAR_ITEMDISPLAY_WIDTH			60
#define IMP_GEAR_ITEMDISPLAY_HEIGHT			25

#define IMP_GEAR_SPACE_BETWEEN_BOXES		1

//*******************************************************************
//
// Local Variables
//
//*******************************************************************

BOOLEAN gfIMPGear_Redraw = FALSE;

// this is the Done	button
INT32 giIMPGearFinishButton;
INT32 giIMPGearFinishButtonImage;

// We access the dropdowns via pointers, which we identify with these enums
// They corrspond with the enums in Dropdown.h
// NOTE: make sure that there are as many of these enums as there are DropDowns for this site. Otherwise odd things migt happen - consider yourself warned.
enum impgeardropdownenums
{
	IMPGEAR_DROPDOWN_LBE1 = 0,
	IMPGEAR_DROPDOWN_LBE2,
	IMPGEAR_DROPDOWN_LBE3,
	IMPGEAR_DROPDOWN_LBE4,
	IMPGEAR_DROPDOWN_LBE5,

	IMPGEAR_DROPDOWN_GUN_1,
	IMPGEAR_DROPDOWN_AMMO_1,
	IMPGEAR_DROPDOWN_GUN_2,
	IMPGEAR_DROPDOWN_AMMO_2,
	IMPGEAR_DROPDOWN_MELEE,
		
	IMPGEAR_DROPDOWN_HELMET,
	IMPGEAR_DROPDOWN_VEST,
	IMPGEAR_DROPDOWN_LEGS,
	IMPGEAR_DROPDOWN_FACE1,
	IMPGEAR_DROPDOWN_FACE2,

	IMPGEAR_DROPDOWN_MISC_1,
	IMPGEAR_DROPDOWN_MISC_2,
	IMPGEAR_DROPDOWN_MISC_3,
	IMPGEAR_DROPDOWN_MISC_4,
	IMPGEAR_DROPDOWN_MISC_5,
	IMPGEAR_DROPDOWN_MISC_6,
	IMPGEAR_DROPDOWN_MISC_7,

	IMPGEAR_DROPDOWN_MAX,
};

// as accessing DropDowns via parameter is tedious, we use pointers, which allows looping
DropDownBase* pIMPGEARDropDown[IMPGEAR_DROPDOWN_MAX] = {NULL};
INT16 gIMPGearGun1 = -1;
INT16 gIMPGearGun2 = -1;

// a set of all items that we can choose from
std::set<UINT16> gIMPGearPossibleItems;

// a map with all items we have selected, and how many we want
std::map<UINT16, UINT8> gIMPGearSelectedItems;

extern BOOLEAN	gfSkillTraitQuestions[20];
extern BOOLEAN	gfSkillTraitQuestions2[20];

extern BOOLEAN	gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS];

// +/- buttons to alter the number of items
INT32 gIMPGearAddButtonImage[2];
INT32 gIMPGearAddButton[2 * IMPGEAR_DROPDOWN_MAX] = {-1};
BOOLEAN gIMPGearButtonCreated[2 * IMPGEAR_DROPDOWN_MAX] = {FALSE};

UINT8 gIMPGearCount[IMPGEAR_DROPDOWN_MAX] = {0};
UINT8 gIMPGearMaximum[IMPGEAR_DROPDOWN_MAX] = {0};

// cost of IMP gear
INT32 gIMPGearCost = 0;

extern BOOLEAN fNewIMPGearMethodUsed;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************


void		IMPGearDisplay( );
void		DisplayGear( UINT16 usItem, UINT16 usPosX, UINT16 usPosY );
void		BtnIMPGearFinishCallback( GUI_BUTTON *btn, INT32 reason );

// determine all items that are selectable according to xml and choices
void		CalculatePossibleItems();

// store the selected gear
void		StoreSelectedIMPGear();

// fill dropdowns with items
void		DistributePossibleItemsOnDropDowns();

void		GearAddButtonCallback( GUI_BUTTON *btn, INT32 reason );


//*******************************************************************
//
// DropDown instances
//
//*******************************************************************

template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_GUN_1>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_AMMO_1>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_GUN_2>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_AMMO_2>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MELEE>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE1>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE2>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE3>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE4>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE5>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_HELMET>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_VEST>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_LEGS>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_FACE1>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_FACE2>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_1>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_2>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_3>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_4>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_5>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_6>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_7>::SetRefresh( )			{ gfIMPGear_Redraw = TRUE; }

void EnterIMPGear( void )
{
	RenderProfileBackGround( );

	giIMPGearFinishButtonImage = LoadButtonImage( "LAPTOP\\button_5.sti", -1, 0, -1, 1, -1 );
	giIMPGearFinishButton = CreateIconAndTextButton( giIMPGearFinishButtonImage, pImpButtonText[11], FONT12ARIAL,
														  FONT_WHITE, DEFAULT_SHADOW,
														  FONT_WHITE, DEFAULT_SHADOW,
														  TEXT_CJUSTIFIED,
														  LAPTOP_SCREEN_UL_X + (350), LAPTOP_SCREEN_WEB_UL_Y + (340), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														  BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPGearFinishCallback );

	SetButtonCursor( giIMPGearFinishButton, CURSOR_WWW );

	// we access the dropdowns via pointers, which allows us to loop over them
	// for each category, we set a maximum of items that can be stacked at once, as multiple magazines are reasonably, but multiple guns are a no-no
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE1] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE1>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LBE1] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE2] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE2>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LBE2] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE3] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE3>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LBE3] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE4] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE4>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LBE4] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE5] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LBE5>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LBE5] = 1;

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_1] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_GUN_1>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_GUN_1] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_AMMO_1>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_AMMO_1] = 6;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_2] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_GUN_2>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_GUN_2] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_AMMO_2>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_AMMO_2] = 6;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MELEE] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MELEE>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MELEE] = 1;

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_HELMET] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_HELMET>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_HELMET] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_VEST] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_VEST>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_VEST] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LEGS] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_LEGS>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_LEGS] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_FACE1] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_FACE1>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_FACE1] = 1;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_FACE2] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_FACE2>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_FACE2] = 1;

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_1] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_1>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_1] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_2] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_2>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_2] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_3] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_3>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_3] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_4] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_4>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_4] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_5] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_5>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_5] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_6] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_6>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_6] = 4;
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_7] = &DropDownTemplate<DROPDOWNNR_IMPGEAR_MISC_7>::getInstance( );
	gIMPGearMaximum[IMPGEAR_DROPDOWN_MISC_7] = 4;
	
	// determine all possible items
	CalculatePossibleItems();

	DistributePossibleItemsOnDropDowns();	

	UINT16 usX = LAPTOP_SCREEN_UL_X + 20 + IMP_GEAR_ITEMDISPLAY_WIDTH;
	UINT16 usY = LAPTOP_SCREEN_WEB_UL_Y + 30;

	int placement = 0;
	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		gIMPGearCount[i] = min( 1, gIMPGearMaximum[i]);

		if ( i == IMPGEAR_DROPDOWN_LBE1 )
		{
			usX = LAPTOP_SCREEN_UL_X + 20 + IMP_GEAR_ITEMDISPLAY_WIDTH + 230;
			placement = 5;
		}
		else if ( i == IMPGEAR_DROPDOWN_HELMET )
		{
			usX = LAPTOP_SCREEN_UL_X + 20 + IMP_GEAR_ITEMDISPLAY_WIDTH + 230;
			placement = 0;
		}
		else if ( i == IMPGEAR_DROPDOWN_MISC_1 )
		{
			usX = LAPTOP_SCREEN_UL_X + 20 + IMP_GEAR_ITEMDISPLAY_WIDTH;
			placement = 5;
		}
		else if ( i == IMPGEAR_DROPDOWN_GUN_1 )
		{
			usX = LAPTOP_SCREEN_UL_X + 20 + IMP_GEAR_ITEMDISPLAY_WIDTH;
			placement = 0;
		}

		// if not using LBE gear, don't show any
		if ( !UsingNewInventorySystem() )
		{
			if ( i >= IMPGEAR_DROPDOWN_LBE1 && i < IMPGEAR_DROPDOWN_LBE5 )
				continue;
		}

		pIMPGEARDropDown[i]->SetHelpText( szIMPGearDropDownText[i] );
		pIMPGEARDropDown[i]->Create( usX, usY + placement * (IMP_GEAR_ITEMDISPLAY_HEIGHT + IMP_GEAR_SPACE_BETWEEN_BOXES) );

		++placement;
	}

	// load the images for the +/- buttons
	gIMPGearAddButtonImage[0] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 2, -1, 2, -1 );
	gIMPGearAddButtonImage[1] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 3, -1, 3, -1 );
}


void RenderIMPGear( void )
{
	//render the metal background graphic
	RenderProfileBackGround( );

	IMPGearDisplay( );

	// reversed order - lower boxes first. It is otherwise possible that open boxes are overlayed by closed boxes
	for ( int i = IMPGEAR_DROPDOWN_MAX - 1; i >= 0; --i )
	{
		if ( pIMPGEARDropDown[i] )
			pIMPGEARDropDown[i]->Display( );
	}

	// we have to destroy and recreate the +/- buttons on every refresh, as they would otherwise get overdrawn or work when they shouldn't be there
	for ( int i = 0; i < 2 * IMPGEAR_DROPDOWN_MAX; ++i )
	{
		if ( gIMPGearButtonCreated[i] )
		{
			// delete militia panel bottom
			RemoveButton( gIMPGearAddButton[i] );

			gIMPGearButtonCreated[i] = FALSE;
		}
	}

	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		// if dropdown is displayed, has real items in it and can be bought in bulk, add '+'/'-'-buttons
		if ( pIMPGEARDropDown[i] && pIMPGEARDropDown[i]->IsDisplayed( ) && pIMPGEARDropDown[i]->HasEntries( ) && pIMPGEARDropDown[i]->GetSelectedEntryKey( ) > 0 && gIMPGearMaximum[i] > 1 )
		{
			// '+' button
			gIMPGearAddButton[i] = QuickCreateButton( gIMPGearAddButtonImage[0], pIMPGEARDropDown[i]->GetX( ) - 6, pIMPGEARDropDown[i]->GetY(), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)GearAddButtonCallback );
			ButtonList[gIMPGearAddButton[i]]->UserData[0] = i;
			gIMPGearButtonCreated[i] = TRUE;

			// '-' button
			gIMPGearAddButton[IMPGEAR_DROPDOWN_MAX + i] = QuickCreateButton( gIMPGearAddButtonImage[1], pIMPGEARDropDown[i]->GetX( ) - 6, pIMPGEARDropDown[i]->GetY( ) + IMP_GEAR_ITEMDISPLAY_HEIGHT - 6, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)GearAddButtonCallback );
			ButtonList[gIMPGearAddButton[IMPGEAR_DROPDOWN_MAX + i]]->UserData[0] = IMPGEAR_DROPDOWN_MAX + i;
			gIMPGearButtonCreated[IMPGEAR_DROPDOWN_MAX + i] = TRUE;
		}
	}
}


void ExitIMPGear( void )
{
	// before we leave, we store all the gear that was selected (the dropdowns will be destroyed, so we cannot get the values from them later on)
	StoreSelectedIMPGear();

	for ( int i = 0; i < 2 * IMPGEAR_DROPDOWN_MAX; ++i )
	{
		if ( gIMPGearButtonCreated[i] )
		{
			RemoveButton( gIMPGearAddButton[i] );
			gIMPGearButtonCreated[i] = FALSE;
		}
	}

	UnloadButtonImage( gIMPGearAddButtonImage[0] );
	UnloadButtonImage( gIMPGearAddButtonImage[1] );

	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		if ( pIMPGEARDropDown[i] )
		{
			pIMPGEARDropDown[i]->Destroy( );
			pIMPGEARDropDown[i] = NULL;
		}
	}

	gIMPGearGun1 = -1;
	gIMPGearGun2 = -1;

	RemoveButton( giIMPGearFinishButton );
	UnloadButtonImage( giIMPGearFinishButtonImage );
}


void HandleIMPGear( void )
{
	if ( gfIMPGear_Redraw )
	{
		RenderIMPGear( );
		gfIMPGear_Redraw = FALSE;
	}

	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}

void IMPGearDisplay( )
{
	//Display the title
	DrawTextToScreen( szIMPGearWebSiteText[5], IMP_GEAR__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, FONT14ARIAL, IMP_GEAR__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	// depending on guns selected, we need to alter the ammochoices	
	{
		if ( pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_1]->HasEntries() )
		{
			if ( !pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->HasEntries( ) || gIMPGearGun1 != pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_1]->GetSelectedEntryKey( ) )
			{
				gIMPGearGun1 = pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_1]->GetSelectedEntryKey( );

				std::vector<std::pair<INT16, STR16> > entries;
				entries.push_back( std::make_pair( 0, szIMPGearDropDownNoneText[IMPGEAR_DROPDOWN_AMMO_1] ) );

				if ( Item[gIMPGearGun1].usItemClass & IC_GUN )
				{
					WEAPONTYPE* pWeapon = &(Weapon[gIMPGearGun1]);
					UINT16 usLoop = 0;
					while ( Magazine[usLoop].ubCalibre != NOAMMO )
					{
						if ( Magazine[usLoop].ubCalibre == pWeapon->ubCalibre && Magazine[usLoop].ubMagSize == pWeapon->ubMagSize )//&& AmmoTypes[Magazine[usLoop].ubAmmoType].standardIssue )
						{
							UINT16 mag = MagazineClassIndexToItemType( usLoop );

							entries.push_back( std::make_pair( mag, Item[mag].szItemName ) );
						}

						++usLoop;
					}
				}
			
				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->SetEntries( entries );
				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->SetNthEntry( 1 );
				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->Create( pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->GetX(), pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->GetY() );
			}
		}
	}

	{
		if ( pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_2]->HasEntries() )
		{
			if ( !pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->HasEntries( ) || gIMPGearGun2 != pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_2]->GetSelectedEntryKey( ) )
			{
				gIMPGearGun2 = pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_2]->GetSelectedEntryKey();

				std::vector<std::pair<INT16, STR16> > entries;
				entries.push_back( std::make_pair( 0, szIMPGearDropDownNoneText[IMPGEAR_DROPDOWN_AMMO_2] ) );

				if ( Item[gIMPGearGun2].usItemClass & IC_GUN )
				{
					WEAPONTYPE* pWeapon = &(Weapon[gIMPGearGun2]);
					UINT16 usLoop = 0;
					while ( Magazine[usLoop].ubCalibre != NOAMMO )
					{
						if ( Magazine[usLoop].ubCalibre == pWeapon->ubCalibre && Magazine[usLoop].ubMagSize == pWeapon->ubMagSize )//&& AmmoTypes[Magazine[usLoop].ubAmmoType].standardIssue )
						{
							UINT16 mag = MagazineClassIndexToItemType( usLoop );

							entries.push_back( std::make_pair( mag, Item[mag].szItemName ) );
						}

						++usLoop;
					}
				}
				else if ( Item[gIMPGearGun2].usItemClass & IC_LAUNCHER )
				{
					std::set<UINT16>::iterator itend = gIMPGearPossibleItems.end( );
					for ( std::set<UINT16>::iterator it = gIMPGearPossibleItems.begin( ); it != itend; ++it )
					{
						UINT16 usItem = (*it);

						if ( ValidLaunchable( usItem, gIMPGearGun2 ) )
							entries.push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
					}
				}

				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->SetEntries( entries );
				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->SetNthEntry( 1 );
				pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->Create( pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->GetX( ), pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_2]->GetY( ) );
			}
		}
	}

	gIMPGearCost = 0;
	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		if ( pIMPGEARDropDown[i] && pIMPGEARDropDown[i]->IsDisplayed( ) && pIMPGEARDropDown[i]->HasEntries( ) )
		{
			// if something is shown, it has to appear at least once...
			gIMPGearCount[i] = max( 1, gIMPGearCount[i] );

			INT16 sItem = (UINT16)pIMPGEARDropDown[i]->GetSelectedEntryKey( );

			DisplayGear( (UINT16)sItem, pIMPGEARDropDown[i]->GetX( ) - IMP_GEAR_ITEMDISPLAY_WIDTH, pIMPGEARDropDown[i]->GetY( ) );

			if ( sItem > 0 )
			{
				if ( sItem )
					gIMPGearCost += gIMPGearCount[i] * Item[sItem].usPrice;

				// if more than 1?
				if ( gIMPGearCount[i] > 1 )
				{
					CHAR16 sString[128];
					swprintf( sString, L"%d", gIMPGearCount[i] );
					INT16 sX, sY;
					FindFontRightCoordinates( (INT16)(pIMPGEARDropDown[i]->GetX( ) - IMP_GEAR_ITEMDISPLAY_WIDTH), (INT16)(pIMPGEARDropDown[i]->GetY( ) + 6), (INT16)(10), (INT16)(GetFontHeight( FONT10ARIAL )), sString, FONT10ARIAL, &sX, &sY );
					mprintf( sX, sY, sString );
				}
			}
		}
		else
		{
			gIMPGearCount[i] = 0;
		}
	}
	
	CHAR16 wTemp[50];
	swprintf( wTemp, szIMPGearWebSiteText[6], GetIMPGearCost( ) );
	DrawTextToScreen( wTemp, LAPTOP_SCREEN_UL_X + 180, LAPTOP_SCREEN_WEB_UL_Y + 360, LAPTOP_TEXT_WIDTH, FONT14ARIAL, IMP_GEAR__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
}

void DisplayGear( UINT16 usItem, UINT16 usPosX, UINT16 usPosY )
{
	if ( usItem )
	{
		// draw background 'box' for an item
		ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY, usPosX + IMP_GEAR_ITEMDISPLAY_WIDTH, usPosY + IMP_GEAR_ITEMDISPLAY_HEIGHT, Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );

		HVOBJECT		hVObject;
		ETRLEObject		*pTrav;

		INVTYPE* pItem = &Item[usItem];

		UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;

		GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );

		if ( usGraphicNum < hVObject->usNumberOfObjects )
		{
			pTrav = &(hVObject->pETRLEObject[usGraphicNum]);
		}
		else
		{
			SGP_THROW( _BS( L"Number of images in VObject [" ) << hVObject->usNumberOfObjects
					   << L"] is smaller than the requested index [" << (int)pItem->ubGraphicNum << L"]" << _BS::wget );
		}

		INT16 PosX = usPosX + (IMP_GEAR_ITEMDISPLAY_WIDTH - pTrav->usWidth) / 2 - pTrav->sOffsetX;
		INT16 PosY = usPosY + (IMP_GEAR_ITEMDISPLAY_HEIGHT - pTrav->usHeight) / 2 - pTrav->sOffsetY;

		//blt the shadow of the item
		if ( gGameSettings.fOptions[TOPTION_SHOW_ITEM_SHADOW] )
			BltVideoObjectOutlineShadowFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, PosX, PosY );

		//blt the item
		BltVideoObjectOutlineFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, PosX, PosY, 0, FALSE );
	}
	else
	{
		// if there is not item to display, sue a different colour for the box
		ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY, usPosX + IMP_GEAR_ITEMDISPLAY_WIDTH, usPosY + IMP_GEAR_ITEMDISPLAY_HEIGHT, Get16BPPColor( FROMRGB( 230, 84, 43 ) ) );
	}
}

void BtnIMPGearFinishCallback( GUI_BUTTON *btn, INT32 reason )
{
	// btn callback for IMP personality quiz answer button
	if ( !(btn->uiFlags & BUTTON_ENABLED) )
		return;

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);

		iCurrentImpPage = IMP_FINISH;
		iCurrentProfileMode = IMP__FINISH;
	}
}

INT32	GetIMPGearCost( )
{
	if ( IsIMPGearUsed( ) )
		return max( 0, gIMPGearCost - 3000 );

	return 0;
}

void AddItemCategoryToPossibleGear(UINT8 aCategory)
{
	for ( int j = 0; j < 50; ++j )
	{
		UINT16 usItem = gIMPItemChoices[aCategory].bItemNo[j];
		if ( usItem && ItemIsLegal( usItem ) )
		{
			gIMPGearPossibleItems.insert( usItem );
		}
	}
}

void CalculatePossibleItems()
{
	gIMPGearPossibleItems.clear( );

	for ( int i = 0; i < IMP_AUTO_WEAPONS; ++i )
	{
		AddItemCategoryToPossibleGear(i);
	}

	//loop through all the buttons and reset them
	for ( UINT8 uiCnt = 0; uiCnt< (gGameOptions.fNewTraitSystem ? IMP_SKILL_TRAITS_NEW_MAJOR_NONE : IMP_SKILL_TRAITS__NONE); ++uiCnt )
	{
		//if the trait is selected
		if ( gfSkillTraitQuestions[uiCnt] && gfSkillTraitQuestions2[uiCnt] )
		{
			if ( gGameOptions.fNewTraitSystem )
			{				
				switch ( uiCnt )
				{
				case IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS:		AddItemCategoryToPossibleGear( IMP_AUTO_WEAPONS_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS:	AddItemCategoryToPossibleGear( IMP_HEAVY_WEAPONS_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_PROF_SNIPER:		AddItemCategoryToPossibleGear( IMP_SNIPER_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_RANGER:			AddItemCategoryToPossibleGear( IMP_RANGER_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_GUNSLINGER:		AddItemCategoryToPossibleGear( IMP_GUNSLINGER_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS:		AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_SQUADLEADER:		AddItemCategoryToPossibleGear( IMP_SQUADLEADER_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_TECHNICIAN:		AddItemCategoryToPossibleGear( IMP_TECHNICIAN_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_DOCTOR:			AddItemCategoryToPossibleGear( IMP_DOCTOR_EXP );	break;
				case IMP_SKILL_TRAITS_NEW_COVERT:			AddItemCategoryToPossibleGear( IMP_COVERT_EXP );	break;

				default:
					break;
				}
			}
			else
			{
				switch ( uiCnt )
				{
				case IMP_SKILL_TRAITS__LOCKPICK:		AddItemCategoryToPossibleGear( IMP_LOCKPICKING_EXP );	break;
				case IMP_SKILL_TRAITS__HAND_2_HAND:		AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS_EXP );	break;
				case IMP_SKILL_TRAITS__ELECTRONICS:		AddItemCategoryToPossibleGear( IMP_ELECTRONICS );	break;
				case IMP_SKILL_TRAITS__NIGHT_OPS:		AddItemCategoryToPossibleGear( IMP_NIGHT_OPS_EXP );	break;
				case IMP_SKILL_TRAITS__THROWING:		AddItemCategoryToPossibleGear( IMP_THROWING_EXP );	break;
				case IMP_SKILL_TRAITS__TEACHING:		AddItemCategoryToPossibleGear( IMP_TEACHING_EXP );	break;
				case IMP_SKILL_TRAITS__HEAVY_WEAPONS:	AddItemCategoryToPossibleGear( IMP_HEAVY_WEAPONS_EXP );	break;
				case IMP_SKILL_TRAITS__AUTO_WEAPONS:	AddItemCategoryToPossibleGear( IMP_AUTO_WEAPONS_EXP );	break;
				case IMP_SKILL_TRAITS__STEALTHY:		AddItemCategoryToPossibleGear( IMP_STEALTHY_EXP );	break;
				case IMP_SKILL_TRAITS__AMBIDEXTROUS:	AddItemCategoryToPossibleGear( IMP_AMBIDEXTROUS );	break;
				case IMP_SKILL_TRAITS__KNIFING:			AddItemCategoryToPossibleGear( IMP_MELEE_EXP );
				case IMP_SKILL_TRAITS__PROF_SNIPER:		AddItemCategoryToPossibleGear( IMP_SNIPER_EXP );	break;
				case IMP_SKILL_TRAITS__CAMO:			AddItemCategoryToPossibleGear( IMP_CAMOUFLAGED );	break;
				case IMP_SKILL_TRAITS__MARTIAL_ARTS:	AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS_EXP );	break;

				default:
					break;
				}
			}
		}
		else if ( gfSkillTraitQuestions[uiCnt] || gfSkillTraitQuestions2[uiCnt] )
		{
			if ( gGameOptions.fNewTraitSystem )
			{
				switch ( uiCnt )
				{
				case IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS:		AddItemCategoryToPossibleGear( IMP_AUTO_WEAPONS );	break;
				case IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS:	AddItemCategoryToPossibleGear( IMP_HEAVY_WEAPONS );	break;
				case IMP_SKILL_TRAITS_NEW_PROF_SNIPER:		AddItemCategoryToPossibleGear( IMP_SNIPER );	break;
				case IMP_SKILL_TRAITS_NEW_RANGER:			AddItemCategoryToPossibleGear( IMP_RANGER );	break;
				case IMP_SKILL_TRAITS_NEW_GUNSLINGER:		AddItemCategoryToPossibleGear( IMP_GUNSLINGER );	break;
				case IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS:		AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS );	break;
				case IMP_SKILL_TRAITS_NEW_SQUADLEADER:		AddItemCategoryToPossibleGear( IMP_SQUADLEADER );	break;
				case IMP_SKILL_TRAITS_NEW_TECHNICIAN:		AddItemCategoryToPossibleGear( IMP_TECHNICIAN );	break;
				case IMP_SKILL_TRAITS_NEW_DOCTOR:			AddItemCategoryToPossibleGear( IMP_DOCTOR );	break;
				case IMP_SKILL_TRAITS_NEW_COVERT:			AddItemCategoryToPossibleGear( IMP_COVERT );	break;

				default:
					break;
				}
			}
			else
			{
				switch ( uiCnt )
				{
				case IMP_SKILL_TRAITS__LOCKPICK:		AddItemCategoryToPossibleGear( IMP_LOCKPICKING );	break;
				case IMP_SKILL_TRAITS__HAND_2_HAND:		AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS_EXP );	break;
				case IMP_SKILL_TRAITS__ELECTRONICS:		AddItemCategoryToPossibleGear( IMP_ELECTRONICS );	break;
				case IMP_SKILL_TRAITS__NIGHT_OPS:		AddItemCategoryToPossibleGear( IMP_NIGHT_OPS );	break;
				case IMP_SKILL_TRAITS__THROWING:		AddItemCategoryToPossibleGear( IMP_THROWING );	break;
				case IMP_SKILL_TRAITS__TEACHING:		AddItemCategoryToPossibleGear( IMP_TEACHING );	break;
				case IMP_SKILL_TRAITS__HEAVY_WEAPONS:	AddItemCategoryToPossibleGear( IMP_HEAVY_WEAPONS );	break;
				case IMP_SKILL_TRAITS__AUTO_WEAPONS:	AddItemCategoryToPossibleGear( IMP_AUTO_WEAPONS );	break;
				case IMP_SKILL_TRAITS__STEALTHY:		AddItemCategoryToPossibleGear( IMP_STEALTHY );	break;
				case IMP_SKILL_TRAITS__AMBIDEXTROUS:	AddItemCategoryToPossibleGear( IMP_AMBIDEXTROUS );	break;
				case IMP_SKILL_TRAITS__KNIFING:			AddItemCategoryToPossibleGear( IMP_MELEE );
				case IMP_SKILL_TRAITS__PROF_SNIPER:		AddItemCategoryToPossibleGear( IMP_SNIPER );	break;
				case IMP_SKILL_TRAITS__CAMO:			AddItemCategoryToPossibleGear( IMP_CAMOUFLAGED );	break;
				case IMP_SKILL_TRAITS__MARTIAL_ARTS:	AddItemCategoryToPossibleGear( IMP_MARTIAL_ARTS );	break;

				default:
					break;
				}
			}
		}
	}

	if ( gGameOptions.fNewTraitSystem )
	{
		for ( UINT8 uiCnt = 0; uiCnt < IMP_SKILL_TRAITS_NEW_MINOR_NONE; ++uiCnt )
		{
			//if the trait is selected
			if ( gfMinorTraitQuestions[uiCnt] )
			{
				//switch on the trait, and add it to the list
				switch ( uiCnt )
				{
				case IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS:		AddItemCategoryToPossibleGear( IMP_AMBIDEXTROUS );	break;
				case IMP_SKILL_TRAITS_NEW_MELEE:			AddItemCategoryToPossibleGear( IMP_MELEE );	break;
				case IMP_SKILL_TRAITS_NEW_THROWING:			AddItemCategoryToPossibleGear( IMP_THROWING );	break;
				case IMP_SKILL_TRAITS_NEW_NIGHT_OPS:		AddItemCategoryToPossibleGear( IMP_NIGHT_OPS );	break;
				case IMP_SKILL_TRAITS_NEW_STEALTHY:			AddItemCategoryToPossibleGear( IMP_STEALTHY );	break;
				case IMP_SKILL_TRAITS_NEW_ATHLETICS:		AddItemCategoryToPossibleGear( IMP_ATHLETICS );	break;
				case IMP_SKILL_TRAITS_NEW_BODYBUILDING:		AddItemCategoryToPossibleGear( IMP_BODYBUILDING );	break;
				case IMP_SKILL_TRAITS_NEW_DEMOLITIONS:		AddItemCategoryToPossibleGear( IMP_DEMOLITIONS );	break;
				case IMP_SKILL_TRAITS_NEW_TEACHING:			AddItemCategoryToPossibleGear( IMP_TEACHING );	break;
				case IMP_SKILL_TRAITS_NEW_SCOUTING:			AddItemCategoryToPossibleGear( IMP_SCOUTING );	break;
				case IMP_SKILL_TRAITS_NEW_RADIO_OPERATOR:	AddItemCategoryToPossibleGear( IMP_RADIO_OPERATOR );	break;
					
				default:
					break;
				}
			}
		}
	}
}

// store the selected gear
void StoreSelectedIMPGear()
{
	gIMPGearSelectedItems.clear();

	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		if ( pIMPGEARDropDown[i] && pIMPGEARDropDown[i]->IsDisplayed( ) && pIMPGEARDropDown[i]->HasEntries( ) )
		{
			UINT16 usItem = (UINT16)pIMPGEARDropDown[i]->GetSelectedEntryKey( );

			if ( usItem )
			{
				// add item (make sure it is at least 1)
				gIMPGearSelectedItems[usItem] = max(1, gIMPGearCount[i]);
			}
		}
	}
}

void DistributePossibleItemsOnDropDowns()
{
	// these vectors contain the items to be shown in the dropdowns
	std::map< UINT16, std::vector<std::pair<INT16, STR16> > > entryvec;
	
	// it is always possible to not select something
	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		entryvec[i].push_back( std::make_pair( 0, szIMPGearDropDownNoneText[i] ) );
	}

	std::set<UINT16>::iterator itend = gIMPGearPossibleItems.end( );
	for ( std::set<UINT16>::iterator it = gIMPGearPossibleItems.begin(); it != itend; ++it )
	{
		UINT16 usItem = (*it);

		if ( Item[usItem].usItemClass & IC_GUN )
		{
			entryvec[IMPGEAR_DROPDOWN_GUN_1].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );

			if ( !Item[usItem].twohanded )
				entryvec[IMPGEAR_DROPDOWN_GUN_2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
		else if ( Item[usItem].usItemClass & IC_LAUNCHER )
		{
			entryvec[IMPGEAR_DROPDOWN_GUN_2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
		else if ( Item[usItem].usItemClass & (IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH) )
		{
			entryvec[IMPGEAR_DROPDOWN_MELEE].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
		else if ( Item[usItem].usItemClass & IC_ARMOUR )
		{
			if ( Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET )
				entryvec[IMPGEAR_DROPDOWN_HELMET].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			else if ( Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_VEST )
				entryvec[IMPGEAR_DROPDOWN_VEST].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			else if ( Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS )
				entryvec[IMPGEAR_DROPDOWN_LEGS].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
		else if ( Item[usItem].usItemClass & IC_LBEGEAR )
		{
			switch ( LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass )
			{
			case THIGH_PACK:
				entryvec[IMPGEAR_DROPDOWN_LBE4].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_LBE5].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				break;
			case VEST_PACK:
				entryvec[IMPGEAR_DROPDOWN_LBE1].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				break;
			case COMBAT_PACK:
				entryvec[IMPGEAR_DROPDOWN_LBE2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				break;
			case BACKPACK:
				entryvec[IMPGEAR_DROPDOWN_LBE3].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				break;			
			default:
				entryvec[IMPGEAR_DROPDOWN_LBE4].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_LBE5].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				break;
			}
		}
		else if ( Item[usItem].usItemClass & IC_FACE )
		{
			entryvec[IMPGEAR_DROPDOWN_FACE1].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_FACE2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
		else if ( Item[usItem].usItemClass & ( IC_GRENADE | IC_BOMB ) )
		{
			// ignore item if it is a launchable - we already list it under ammo if a launcher is selected
			BOOLEAN found = FALSE;
			UINT16 iLoop = 0;
			while ( Launchable[iLoop][0] != 0 )
			{
				if ( Launchable[iLoop][0] == usItem )
				{
					found = TRUE;
					break;
				}

				++iLoop;
			}

			if ( !found )
			{
				entryvec[IMPGEAR_DROPDOWN_MISC_1].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_3].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_4].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_5].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_6].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				entryvec[IMPGEAR_DROPDOWN_MISC_7].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			}
		}
		else
		{
			entryvec[IMPGEAR_DROPDOWN_MISC_1].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_2].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_3].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_4].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_5].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_6].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
			entryvec[IMPGEAR_DROPDOWN_MISC_7].push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
		}
	}

	// check for oddities...
	SGP_THROW_IFFALSE( pIMPGEARDropDown[0] != NULL, L"Bad pointer in DistributePossibleItemsOnDropDowns. How did you do that?" );

	for ( int i = 0; i < IMPGEAR_DROPDOWN_MAX; ++i )
	{
		pIMPGEARDropDown[i]->SetEntries( entryvec[i] );
	}

	// per default, each dropdown selects 'no item'. Here we set some defaults
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_GUN_1]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_AMMO_1]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MELEE]->SetNthEntry( 1 );

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE1]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE2]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LBE3]->SetNthEntry( 1 );

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_HELMET]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_VEST]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_LEGS]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_FACE1]->SetNthEntry( 1 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_FACE2]->SetNthEntry( 2 );

	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_1]->SetNthEntry( 4 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_2]->SetNthEntry( 3 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_3]->SetNthEntry( 2 );
	pIMPGEARDropDown[IMPGEAR_DROPDOWN_MISC_4]->SetNthEntry( 1 );
}

void GiveIMPSelectedGear( MERCPROFILESTRUCT *pProfile )
{
	std::map<UINT16, UINT8>::iterator itend = gIMPGearSelectedItems.end( );
	for ( std::map<UINT16, UINT8>::iterator it = gIMPGearSelectedItems.begin( ); it != itend; ++it )
	{
		MakeProfileInvItemAnySlot( pProfile, (*it).first, 100, (*it).second );
	}
}

void GearAddButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			INT32 id = btn->UserData[0];

			if ( id >= 0 )
			{
				if ( id < IMPGEAR_DROPDOWN_MAX )
				{
					// increase
					gIMPGearCount[id] = min( gIMPGearCount[id] + 1, gIMPGearMaximum[id] );
				}
				else if ( id < 2 * IMPGEAR_DROPDOWN_MAX )
				{
					// decrease
					gIMPGearCount[id - IMPGEAR_DROPDOWN_MAX] = max( gIMPGearCount[id - IMPGEAR_DROPDOWN_MAX] - 1, 1 );
				}
			}

			RenderIMPGear( );
		}
	}
}
