/**
* @file
* @author Flugente (bears-pit.com)
*/

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

extern BOOLEAN gfGlowTimerExpired;
BOOLEAN fShowIMPItemHighLight = FALSE;
struct rgbcolorImpGear
{
	UINT8 ubRed;
	UINT8 ubGreen;
	UINT8 ubBlue;
};
rgbcolorImpGear GlowColorsA[] = {
	{0,0,0},
	{25,0,0},
	{50,0,0},
	{75,0,0},
	{100,0,0},
	{125,0,0},
	{150,0,0},
	{175,0,0},
	{200,0,0},
	{225,0,0},
	{250,0,0},
};
extern BOOLEAN bBigBody;

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
INV_REGIONS gIMPGearInvData[NUM_INV_SLOTS];
MOUSE_REGION gIMPGearInvRegion[NUM_INV_SLOTS];
MOUSE_REGION gIMPGearInvPoolRegion[25];
UINT32 gIMPInvDoneButtonImage;
UINT32 gIMPInvDoneButton;
UINT32 gIMPInvArrowButtonImage[2];
UINT32 gIMPInvArrowButton[2];
INT32 gIMPCurrentInventoryPoolPage = 0;
INT32 gIMPLastInventoryPoolPage = 0;
INT32 gCurrentImpGearChoices = 0;
std::map< UINT16, std::vector<std::pair<INT16, STR16> > > gIMPPossibleItems;
std::map< UINT16, std::pair<INT16, UINT8> > gIMPPocketSelectedItems; //Stores what item and amount is currently selected in merc inventory
UINT32 gIMPGlowX = 0;
UINT32 gIMPGlowY = 0;


// this is the Done	button
INT32 giIMPGearFinishButton;
INT32 giIMPGearFinishButtonImage;


INT16 gIMPGearGun1 = -1;
INT16 gIMPGearGun2 = -1;
INT16 gIMPGearGun3 = -1;

// a set of all items that we can choose from
std::set<UINT16> gIMPGearPossibleItems;
// a map with all items we have selected, and how many we want
std::map<UINT16, UINT8> gIMPGearSelectedItems;

extern BOOLEAN	gfSkillTraitQuestions[20];
extern BOOLEAN	gfSkillTraitQuestions2[20];

extern BOOLEAN	gfMinorTraitQuestions[IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS];

// cost of IMP gear
INT32 gIMPGearCost = 0;

extern BOOLEAN fNewIMPGearMethodUsed;


//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************
void		IMPGearDisplay( );
void		BtnIMPGearFinishCallback( GUI_BUTTON *btn, INT32 reason );
// determine all items that are selectable according to xml and choices
void		CalculatePossibleItems();
// store the selected gear
void		StoreSelectedIMPGear();
void		InitImpGearCoords(void);
void		RenderImpGearSelectionChoices(UINT32 userData);
void		DisplayPagesForImpInventoryPool(void);
void		DistributePossibleItemsToVectors(void);
void		DrawImpGearInventoryPool(void);
void SetChoiceForPocket(INT32 pocket, INT32 index, UINT8 amount);
void IMPInvMoveCallback(MOUSE_REGION* pRegion, INT32 iReason);
void IMPInventoryPoolSelectCallback(MOUSE_REGION* pRegion, INT32 iReason);
extern void HandleCommonGlowTimer();
extern BOOLEAN RestoreExternBackgroundRect(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight);
extern void SetClippingRegionAndImageWidth(
	int iImageWidth,
	int iClipStartX,
	int iClipStartY,
	int iClipWidth,
	int iClipHeight
);
extern void RectangleDraw(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, UINT8* ScreenPtr);
void GlowImpInvPoolItem(void);
void IMPGearHandleMousePageScroll(void);
void RenderIMPGearBodytype(void);
void IMPInventoryPoolDoneBtn(GUI_BUTTON* btn, INT32 reason);
void ImpInventoryPoolNextBtn(GUI_BUTTON* btn, INT32 reason);
void ImpInventoryPoolPrevBtn(GUI_BUTTON* btn, INT32 reason);


void EnterIMPGear( void )
{
	RenderProfileBackGround( );

	giIMPGearFinishButtonImage = LoadButtonImage( "LAPTOP\\button_5.sti", -1, 0, -1, 1, -1 );
	giIMPGearFinishButton = CreateIconAndTextButton( 
		giIMPGearFinishButtonImage, pImpButtonText[11], FONT12ARIAL,
		FONT_WHITE, DEFAULT_SHADOW,
		FONT_WHITE, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		LAPTOP_SCREEN_UL_X + (350), LAPTOP_SCREEN_WEB_UL_Y + (340), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPGearFinishCallback 
	);
	SetButtonCursor( giIMPGearFinishButton, CURSOR_WWW );


	// create done and next/previous page buttons for inventory pool
	gIMPInvDoneButtonImage = LoadButtonImage("INTERFACE\\done_button.sti", -1, 0, -1, 1, -1);
	gIMPInvDoneButton = QuickCreateButton(gIMPInvDoneButtonImage, gIMPInvPoolLayout.x + 247, gIMPInvPoolLayout.y + 187,
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
		(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)IMPInventoryPoolDoneBtn);

	gIMPInvArrowButtonImage[0] = LoadButtonImage("INTERFACE\\map_screen_bottom_arrows.sti", 10, 1, -1, 3, -1);
	gIMPInvArrowButton[0] = QuickCreateButton(gIMPInvArrowButtonImage[0], gIMPInvPoolLayout.x + 223, gIMPInvPoolLayout.y + 189,
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
		(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ImpInventoryPoolNextBtn);


	gIMPInvArrowButtonImage[1] = LoadButtonImage("INTERFACE\\map_screen_bottom_arrows.sti", 9, 0, -1, 2, -1);
	gIMPInvArrowButton[1] = QuickCreateButton(gIMPInvArrowButtonImage[1], gIMPInvPoolLayout.x + 150, gIMPInvPoolLayout.y + 189,
		BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
		(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ImpInventoryPoolPrevBtn);

	SetButtonCursor(gIMPInvDoneButton, CURSOR_WWW);
	SetButtonCursor(gIMPInvArrowButton[0], CURSOR_WWW);
	SetButtonCursor(gIMPInvArrowButton[1], CURSOR_WWW);

	// Hide and disable them for now
	DisableButton(gIMPInvDoneButton);
	DisableButton(gIMPInvArrowButton[0]);
	DisableButton(gIMPInvArrowButton[1]);
	HideButton(gIMPInvDoneButton);
	HideButton(gIMPInvArrowButton[0]);
	HideButton(gIMPInvArrowButton[1]);


	InitImpGearCoords();
	// determine all possible items
	CalculatePossibleItems();
	DistributePossibleItemsToVectors();
}


void RenderIMPGear( void )
{
	//render the metal background graphic
	RenderProfileBackGround( );
	RenderImpGearSelection( );
	RenderIMPGearBodytype( );
	IMPGearDisplay( );
}


void ExitIMPGear( void )
{
	// before we leave, we store all the gear that was selected
	StoreSelectedIMPGear();

	gIMPGearGun1 = -1;
	gIMPGearGun2 = -1;
	gIMPGearGun3 = -1;

	RemoveButton( giIMPGearFinishButton );
	UnloadButtonImage( giIMPGearFinishButtonImage );
	RemoveButton(gIMPInvDoneButton);
	UnloadButtonImage(gIMPInvDoneButtonImage);
	RemoveButton(gIMPInvArrowButton[0]);
	UnloadButtonImage(gIMPInvArrowButtonImage[0]);
	RemoveButton(gIMPInvArrowButton[1]);
	UnloadButtonImage(gIMPInvArrowButtonImage[1]);

	for (UINT8 cnt = 0; cnt < NUM_INV_SLOTS; cnt++)
	{
		MSYS_RemoveRegion(&gIMPGearInvRegion[cnt]);
	}
	for (size_t i = 0; i < 25; i++)
	{
		MSYS_RemoveRegion(&gIMPGearInvPoolRegion[i]);
	}
}


void HandleIMPGear( void )
{
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
	IMPGearHandleMousePageScroll();
	HandleCommonGlowTimer();
	GlowImpInvPoolItem();
}


void IMPGearDisplay( )
{
	//Display the title
	DrawTextToScreen( szIMPGearWebSiteText[5], IMP_GEAR__TITLE_X, LAPTOP_TITLE_Y, LAPTOP_TEXT_WIDTH, FONT14ARIAL, IMP_GEAR__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	// Add default ammo for guns & sidearm in case the gun was changed
	if (gIMPPossibleItems[HANDPOS].size() > 1)
	{
		INT16 currentWeapon = gIMPPocketSelectedItems[HANDPOS].first;
		if (gIMPPossibleItems[SMALLPOCK1POS].size() <= 1 || gIMPGearGun1 != currentWeapon)
		{
			gIMPGearGun1 = currentWeapon;

			std::vector<std::pair<INT16, STR16> > entries;
			entries.push_back(std::make_pair(0, pLongAssignmentStrings[40])); //Reusing AssignmentString for "Empty" text

			if (Item[gIMPGearGun1].usItemClass & IC_GUN)
			{
				WEAPONTYPE* pWeapon = &(Weapon[gIMPGearGun1]);
				UINT16 usLoop = 0;
				while (Magazine[usLoop].ubCalibre != NOAMMO)
				{
					if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre && Magazine[usLoop].ubMagSize == pWeapon->ubMagSize)
					{
						UINT16 mag = MagazineClassIndexToItemType(usLoop);

						entries.push_back(std::make_pair(mag, Item[mag].szItemName));
					}
					++usLoop;
				}
			}

			for (size_t i = SMALLPOCK1POS; i < SMALLPOCK6POS; i++)
			{
				gIMPPossibleItems[i] = entries;
			}

			// Add default magazine for pockets, set to empty if no gun
			for (size_t i = SMALLPOCK1POS; i < SMALLPOCK6POS; i++)
			{
				if (currentWeapon != 0)
				{
					SetChoiceForPocket(i, 1, 1);
				}
				else
				{
					SetChoiceForPocket(i, 0, 1);
				}
			}

			if (!UsingNewInventorySystem()) //Only 4 magazine slots for old inventory
			{
				SetChoiceForPocket(SMALLPOCK5POS, 0, 0);
			}
		}
	}

	if (gIMPPossibleItems[SECONDHANDPOS].size() > 1)
	{
		INT16 currentWeapon = gIMPPocketSelectedItems[SECONDHANDPOS].first;
		if (gIMPPossibleItems[SMALLPOCK11POS].size() <= 1 || gIMPGearGun2 != currentWeapon)
		{
			gIMPGearGun2 = currentWeapon;


			std::vector<std::pair<INT16, STR16> > entries;
			entries.push_back(std::make_pair(0, pLongAssignmentStrings[40])); //Reusing AssignmentString for "Empty" text


			if (Item[gIMPGearGun2].usItemClass & IC_GUN)
			{
				WEAPONTYPE* pWeapon = &(Weapon[gIMPGearGun2]);
				UINT16 usLoop = 0;
				while (Magazine[usLoop].ubCalibre != NOAMMO)
				{
					if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre && Magazine[usLoop].ubMagSize == pWeapon->ubMagSize)
					{
						UINT16 mag = MagazineClassIndexToItemType(usLoop);

						entries.push_back(std::make_pair(mag, Item[mag].szItemName));
					}
					++usLoop;
				}
			}

			for (size_t i = SMALLPOCK11POS; i < SMALLPOCK19POS; i++)
			{
				gIMPPossibleItems[i] = entries;
			}


			// Add default magazine for first 4 pockets
			if (currentWeapon != 0)
			{
				for (size_t i = SMALLPOCK11POS; i < SMALLPOCK15POS; i++)
				{
					SetChoiceForPocket(i, 1, 1);
				}
			}
			else
			{
				for (size_t i = SMALLPOCK11POS; i < SMALLPOCK19POS; i++)
				{
					SetChoiceForPocket(i, 0, 1);
				}
			}
		}
	}

	if (gIMPPossibleItems[GUNSLINGPOCKPOS].size() > 1)
	{
		INT16 currentWeapon = gIMPPocketSelectedItems[GUNSLINGPOCKPOS].first;
		if (gIMPPossibleItems[SMALLPOCK6POS].size() <= 1 || gIMPGearGun3 != currentWeapon)
		{
			gIMPGearGun3 = currentWeapon;

			std::vector<std::pair<INT16, STR16> > entries;
			entries.push_back(std::make_pair(0, pLongAssignmentStrings[40])); //Reusing AssignmentString for "Empty" text

			if (Item[gIMPGearGun3].usItemClass & IC_GUN)
			{
				WEAPONTYPE* pWeapon = &(Weapon[gIMPGearGun3]);
				UINT16 usLoop = 0;
				while (Magazine[usLoop].ubCalibre != NOAMMO)
				{
					if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre && Magazine[usLoop].ubMagSize == pWeapon->ubMagSize)
					{
						UINT16 mag = MagazineClassIndexToItemType(usLoop);

						entries.push_back(std::make_pair(mag, Item[mag].szItemName));
					}
					++usLoop;
				}
			}
			else if ( Item[gIMPGearGun3].usItemClass & IC_LAUNCHER )
			{
				std::set<UINT16>::iterator itend = gIMPGearPossibleItems.end( );
				for ( std::set<UINT16>::iterator it = gIMPGearPossibleItems.begin( ); it != itend; ++it )
				{
					UINT16 usItem = (*it);

					if ( ValidLaunchable( usItem, gIMPGearGun3 ) )
						entries.push_back( std::make_pair( usItem, Item[usItem].szItemName ) );
				}
			}


			for (size_t i = SMALLPOCK6POS; i < SMALLPOCK11POS; i++)
			{
				gIMPPossibleItems[i] = entries;
			}

			// Add default magazine for pockets
			for (size_t i = SMALLPOCK6POS; i < SMALLPOCK11POS; i++)
			{
				if (currentWeapon != 0)
				{
					SetChoiceForPocket(i, 1, 1);
				}
				else
				{
					SetChoiceForPocket(i, 0, 1);
				}
			}

			if (!UsingNewInventorySystem()) //Only 4 magazine slots for old inventory
			{
				SetChoiceForPocket(SMALLPOCK10POS, 0, 0);
			}
		}
	}



	gIMPGearCost = 0;
	// Draw selected gear and silhouettes for empty pockets
	for (const auto& kv : gIMPPocketSelectedItems) {
		UINT16 x = 0;
		UINT16 y = 0;

		auto pocket = kv.first;
		INT16 sItem = kv.second.first;
		UINT8 amount = kv.second.second;

		// Offset so item graphics are more in the center of the slots. Going by the item region coordinates misplaces small pocket items for some reason
		UINT16 xOffset = -7;
		if (pocket == HEAD1POS || pocket == HEAD2POS || pocket == KNIFEPOCKPOS || ( MEDPOCK4POS < pocket && pocket < NUM_INV_SLOTS))
		{
			xOffset = -14;
		}

		
		x = gIMPGearInvData[pocket].sX + xOffset; 
		y = gIMPGearInvData[pocket].sY;

		if (sItem > 0)
		{
			DisplayGear((UINT16)sItem, x, y, FALSE, amount, FALSE);
			gIMPGearCost += amount * Item[sItem].usPrice;
		}
		else // Render silhouette if no item
		{
			INT16 silhouetteIndex = 12;
			switch (pocket)
			{
			case HANDPOS:			silhouetteIndex = 5;	break;
			case SECONDHANDPOS:		silhouetteIndex = 13;	break; // Sidearm
			case VESTPOCKPOS:		silhouetteIndex = 0;	break;
			case LTHIGHPOCKPOS:		silhouetteIndex = 1;	break;
			case RTHIGHPOCKPOS:		silhouetteIndex = 2;	break;
			case CPACKPOCKPOS:		silhouetteIndex = 3;	break;
			case BPACKPOCKPOS:		silhouetteIndex = 4;	break;
			case GUNSLINGPOCKPOS:	silhouetteIndex = 6;	break; // Launcher / 2-handed weapon
			case KNIFEPOCKPOS:		silhouetteIndex = 11;	break;
			case MEDPOCK1POS:
			case MEDPOCK2POS:
			case MEDPOCK3POS:
			case MEDPOCK4POS:		silhouetteIndex = 24;	break; // Medkit/Toolkit
			case SMALLPOCK1POS:
			case SMALLPOCK2POS:
			case SMALLPOCK3POS:
			case SMALLPOCK4POS:
			case SMALLPOCK5POS:
			case SMALLPOCK6POS:
			case SMALLPOCK7POS:
			case SMALLPOCK8POS:
			case SMALLPOCK9POS:
			case SMALLPOCK10POS:	silhouetteIndex = 18;	break; // Main gun ammo
			case SMALLPOCK11POS:
			case SMALLPOCK12POS:
			case SMALLPOCK13POS:
			case SMALLPOCK14POS:
			case SMALLPOCK15POS:
			case SMALLPOCK16POS:
			case SMALLPOCK17POS:
			case SMALLPOCK18POS:	silhouetteIndex = 16;	break; // Sidearm ammo

			default:
				break;
			}

				if (pocket > HEAD2POS)
			{
				INVRenderSilhouette(FRAME_BUFFER, ITEM_NOT_FOUND, silhouetteIndex, x - xOffset, y, gIMPGearInvData[pocket].sWidth, gIMPGearInvData[pocket].sHeight);
			}
		}
	}

	// Display extra cost
	if ( gIMPGearCost > gGameExternalOptions.iIMPProfileCost )
	{
		CHAR16 wTemp[50];
		swprintf( wTemp, szIMPGearWebSiteText[6], GetIMPGearCost(), gGameExternalOptions.iIMPProfileCost );
		DrawTextToScreen( wTemp, LAPTOP_SCREEN_UL_X + 100, LAPTOP_SCREEN_WEB_UL_Y + 360, LAPTOP_TEXT_WIDTH, FONT12ARIAL, IMP_GEAR__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}
}


void DisplayGear( UINT16 usItem, UINT16 usPosX, UINT16 usPosY, BOOLEAN fWithBackGround, UINT8 aNumber, BOOLEAN fDisplayNumber )
{
	if ( usItem )
	{
		if ( fWithBackGround )
		{
			// draw background 'box' for an item
			ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY, usPosX + IMP_GEAR_ITEMDISPLAY_WIDTH, usPosY + IMP_GEAR_ITEMDISPLAY_HEIGHT, Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
		}

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

		// display number if more than 1
		if ( fDisplayNumber && aNumber > 1 )
		{
			CHAR16 sString[128];
			swprintf( sString, L"%d", aNumber );
			INT16 sX, sY;
			FindFontRightCoordinates( (INT16)(usPosX), (INT16)(usPosY + 6), 10, (INT16)(GetFontHeight( FONT10ARIAL )), sString, FONT10ARIAL, &sX, &sY );
			mprintf( sX, sY, sString );
		}
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
		fShowIMPItemHighLight = FALSE;
	}
}


INT32	GetIMPGearCost( )
{
	if ( IsIMPGearUsed( ) )
		return max( 0, gIMPGearCost - gGameExternalOptions.iIMPProfileCost );

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

	// always add sidearms
	AddItemCategoryToPossibleGear(IMP_SIDEARM);

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
				case IMP_SKILL_TRAITS_NEW_SURVIVAL:			AddItemCategoryToPossibleGear( IMP_SURVIVAL );	break;
					
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

	for (const auto& kv : gIMPPocketSelectedItems) 
	{
		auto pocket = kv.first;
		INT16 sItem = kv.second.first;
		UINT8 amount = kv.second.second;
		if (sItem)
		{
			// add item (make sure it is at least 1)
			gIMPGearSelectedItems[sItem] = max( 1, gIMPGearSelectedItems[sItem] + amount );
		}
	}
}


void GiveIMPSelectedGear( MERCPROFILESTRUCT *pProfile )
{
	std::map<UINT16, UINT8>::iterator itend = gIMPGearSelectedItems.end( );
	for ( std::map<UINT16, UINT8>::iterator it = gIMPGearSelectedItems.begin( ); it != itend; ++it )
	{
		MakeProfileInvItemAnySlot( pProfile, (*it).first, 100, (*it).second );
	}
}



void ImpInvNextPage(void)
{
	// if can go to next page, go there
	if (gIMPCurrentInventoryPoolPage < gIMPLastInventoryPoolPage)
	{
		gIMPCurrentInventoryPoolPage++;
		fShowIMPItemHighLight = FALSE;
		DrawImpGearInventoryPool();
	}
}

void ImpInvPreviousPage(void)
{
	// if can go to next page, go there
	if (gIMPCurrentInventoryPoolPage > 0)
	{
		gIMPCurrentInventoryPoolPage--;	
		fShowIMPItemHighLight = FALSE;
		DrawImpGearInventoryPool();
	}
}

void ImpInventoryPoolNextBtn(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
			ImpInvNextPage();
		}
	}
}

void ImpInventoryPoolPrevBtn(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
			ImpInvPreviousPage();
		}
	}
}


void IMPGearHandleMousePageScroll(void)
{
	SGPPoint	MousePos;
	GetMousePos(&MousePos);
	const auto x = MousePos.iX;
	const auto y = MousePos.iY;
	const auto xmin = gIMPInvPoolLayout.x; const auto ymin = gIMPInvPoolLayout.y;
	const auto xmax = gIMPInvPoolLayout.x + gIMPInvPoolLayout.width; const auto ymax = gIMPInvPoolLayout.y + gIMPInvPoolLayout.height;

	if ((xmin < x && x < xmax) && (ymin < y && y < ymax))
	{
		const auto Wheelstate = _WheelValue * (gGameSettings.fOptions[TOPTION_INVERT_WHEEL] ? -1 : 1);
		if (Wheelstate < 0)
		{
			ImpInvNextPage();
		}
		else if (Wheelstate > 0)
		{
			ImpInvPreviousPage();
		}
		_WheelValue = 0;
	}
}


void IMPCloseInventoryPool(void)
{
	fShowIMPItemHighLight = FALSE;

	for (UINT8 cnt = 0; cnt < NUM_INV_SLOTS; cnt++)
	{
		MSYS_EnableRegion(&gIMPGearInvRegion[cnt]);
	}
	for (size_t i = 0; i < 25; i++)
	{
		MSYS_DisableRegion(&gIMPGearInvPoolRegion[i]);
	}

	DisableButton(gIMPInvDoneButton);
	DisableButton(gIMPInvArrowButton[0]);
	DisableButton(gIMPInvArrowButton[1]);
	HideButton(gIMPInvDoneButton);
	HideButton(gIMPInvArrowButton[0]);
	HideButton(gIMPInvArrowButton[1]);

	RenderIMPGear();
}


void IMPInventoryPoolDoneBtn(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
		}
		IMPCloseInventoryPool();
	}
}


void IMPInventoryPoolSelectCallback(MOUSE_REGION* pRegion, INT32 iReason)
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN || iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		PlayButtonSound(giIMPGearFinishButton, BUTTON_SOUND_CLICKED_ON);
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		PlayButtonSound(giIMPGearFinishButton, BUTTON_SOUND_CLICKED_OFF);
		INT32 slot = MSYS_GetRegionUserData(pRegion, 0);

		INT32 itemIndex = slot + gIMPCurrentInventoryPoolPage * 25;
		if (itemIndex < gIMPPossibleItems[gCurrentImpGearChoices].size())
		{
			SetChoiceForPocket(gCurrentImpGearChoices, itemIndex, 1);

			// Redraw and enable glow
			DrawImpGearInventoryPool();
		}
	}
	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		IMPCloseInventoryPool();
	}
}


void IMPInvMoveCallback(MOUSE_REGION* pRegion, INT32 iReason)
{
	// Empty on purpose
}


void IMPInvClickCallback(MOUSE_REGION* pRegion, INT32 iReason)
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		PlayButtonSound(giIMPGearFinishButton, BUTTON_SOUND_CLICKED_ON);
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		PlayButtonSound(giIMPGearFinishButton, BUTTON_SOUND_CLICKED_OFF);

		// Set current gear and pages
		gCurrentImpGearChoices = MSYS_GetRegionUserData(pRegion, 0);
		gIMPCurrentInventoryPoolPage = 0;
		gIMPLastInventoryPoolPage = gIMPPossibleItems[gCurrentImpGearChoices].size() / 25;

		// Disable inv pocket mouse regions so we don't accidentally click them
		for (UINT8 cnt = 0; cnt < NUM_INV_SLOTS; cnt++)
		{
			MSYS_DisableRegion(&gIMPGearInvRegion[cnt]);
		}

		for (size_t i = 0; i < 25; i++)
		{
			MSYS_EnableRegion(&gIMPGearInvPoolRegion[i]);
		}

		EnableButton(gIMPInvDoneButton);
		EnableButton(gIMPInvArrowButton[0]);
		EnableButton(gIMPInvArrowButton[1]);

		DrawImpGearInventoryPool();
		fShowIMPItemHighLight = TRUE;
	}
}

void InitImpGearCoords(void)
{
	const auto x = gIMPGearLayout.x;
	const auto y = gIMPGearLayout.y;

	if (UsingNewInventorySystem())
	{
		const auto vestRow1y = 40;
		const auto vestRow2y = 64;

		gIMPGearInvData[HELMETPOS].sX = x + 125;		gIMPGearInvData[HELMETPOS].sY = y + 8;					gIMPGearInvData[HELMETPOS].sWidth = HEAD_INV_SLOT_WIDTH; gIMPGearInvData[HELMETPOS].sHeight = HEAD_INV_SLOT_HEIGHT;		// HELMETPOS
		gIMPGearInvData[VESTPOS].sX = x + 125;			gIMPGearInvData[VESTPOS].sY = y + 36;					gIMPGearInvData[VESTPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[VESTPOS].sHeight = VEST_INV_SLOT_HEIGHT; // VESTPOS
		gIMPGearInvData[LEGPOS].sX = x + 125;			gIMPGearInvData[LEGPOS].sY = y + 96;					gIMPGearInvData[LEGPOS].sWidth = LEGS_INV_SLOT_WIDTH; gIMPGearInvData[LEGPOS].sHeight = LEGS_INV_SLOT_HEIGHT; // LEGPOS
		gIMPGearInvData[HEAD1POS].sX = x + 12;			gIMPGearInvData[HEAD1POS].sY = y + 8;					gIMPGearInvData[HEAD1POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[HEAD1POS].sHeight = SM_INV_SLOT_HEIGHT; // HEAD1POS
		gIMPGearInvData[HEAD2POS].sX = x + 12;			gIMPGearInvData[HEAD2POS].sY = y + 31;					gIMPGearInvData[HEAD2POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[HEAD2POS].sHeight = SM_INV_SLOT_HEIGHT; // HEAD2POS
		gIMPGearInvData[HANDPOS].sX = x + 14;			gIMPGearInvData[HANDPOS].sY = y + 86;					gIMPGearInvData[HANDPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[HANDPOS].sHeight = BIG_INV_SLOT_HEIGHT; // HANDPOS
		gIMPGearInvData[SECONDHANDPOS].sX = x + 14;		gIMPGearInvData[SECONDHANDPOS].sY = y + 110;			gIMPGearInvData[SECONDHANDPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[SECONDHANDPOS].sHeight = BIG_INV_SLOT_HEIGHT; // SECONDHANDPOS
		gIMPGearInvData[VESTPOCKPOS].sX = x + 207;		gIMPGearInvData[VESTPOCKPOS].sY = y + 8;				gIMPGearInvData[VESTPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[VESTPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // VESTPOCK
		gIMPGearInvData[LTHIGHPOCKPOS].sX = x + 32;		gIMPGearInvData[LTHIGHPOCKPOS].sY = y + 144;			gIMPGearInvData[LTHIGHPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[LTHIGHPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // LTHIGHPOCK
		gIMPGearInvData[RTHIGHPOCKPOS].sX = x + 115;	gIMPGearInvData[RTHIGHPOCKPOS].sY = y + 144;			gIMPGearInvData[RTHIGHPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[RTHIGHPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // RTHIGHPOCK
		gIMPGearInvData[CPACKPOCKPOS].sX = x + 206;		gIMPGearInvData[CPACKPOCKPOS].sY = y + 96;				gIMPGearInvData[CPACKPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[CPACKPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // CPACKPOCK
		gIMPGearInvData[BPACKPOCKPOS].sX = x + 277;		gIMPGearInvData[BPACKPOCKPOS].sY = y + 96;				gIMPGearInvData[BPACKPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[BPACKPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // BPACKPOCK
		gIMPGearInvData[GUNSLINGPOCKPOS].sX = x + 281;	gIMPGearInvData[GUNSLINGPOCKPOS].sY = y + 8;			gIMPGearInvData[GUNSLINGPOCKPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[GUNSLINGPOCKPOS].sHeight = BIG_INV_SLOT_HEIGHT; // GUNSLINGPOCKPOS
		gIMPGearInvData[KNIFEPOCKPOS].sX = x + 371;		gIMPGearInvData[KNIFEPOCKPOS].sY = y + 8;				gIMPGearInvData[KNIFEPOCKPOS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[KNIFEPOCKPOS].sHeight = SM_INV_SLOT_HEIGHT; // KNIFEPOCKPOS
		gIMPGearInvData[BIGPOCK1POS].sX = x + 198;		gIMPGearInvData[BIGPOCK1POS].sY = y + 176;				gIMPGearInvData[BIGPOCK1POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK1POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK1
		gIMPGearInvData[BIGPOCK2POS].sX = x + 198;		gIMPGearInvData[BIGPOCK2POS].sY = y + 200;				gIMPGearInvData[BIGPOCK2POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK2POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK2
		gIMPGearInvData[BIGPOCK3POS].sX = x + 198;		gIMPGearInvData[BIGPOCK3POS].sY = y + 224;				gIMPGearInvData[BIGPOCK3POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK3POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK3
		gIMPGearInvData[BIGPOCK4POS].sX = x + 344;		gIMPGearInvData[BIGPOCK4POS].sY = y + 128;				gIMPGearInvData[BIGPOCK4POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK4POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK4
		gIMPGearInvData[BIGPOCK5POS].sX = x + 344;		gIMPGearInvData[BIGPOCK5POS].sY = y + 152;				gIMPGearInvData[BIGPOCK5POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK5POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK5
		gIMPGearInvData[BIGPOCK6POS].sX = x + 344;		gIMPGearInvData[BIGPOCK6POS].sY = y + 176;				gIMPGearInvData[BIGPOCK6POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK6POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK6
		gIMPGearInvData[BIGPOCK7POS].sX = x + 344;		gIMPGearInvData[BIGPOCK7POS].sY = y + 200;				gIMPGearInvData[BIGPOCK7POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK7POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK7
		gIMPGearInvData[MEDPOCK1POS].sX = x + 190;		gIMPGearInvData[MEDPOCK1POS].sY = y + vestRow1y;		gIMPGearInvData[MEDPOCK1POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK1POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK1
		gIMPGearInvData[MEDPOCK2POS].sX = x + 190;		gIMPGearInvData[MEDPOCK2POS].sY = y + vestRow2y;		gIMPGearInvData[MEDPOCK2POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK2POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK2
		gIMPGearInvData[MEDPOCK3POS].sX = x + 32;		gIMPGearInvData[MEDPOCK3POS].sY = y + 224;				gIMPGearInvData[MEDPOCK3POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK3POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK3
		gIMPGearInvData[MEDPOCK4POS].sX = x + 115;		gIMPGearInvData[MEDPOCK4POS].sY = y + 224;				gIMPGearInvData[MEDPOCK4POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK4POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK4
		gIMPGearInvData[SMALLPOCK1POS].sX = x + 241;	gIMPGearInvData[SMALLPOCK1POS].sY = y + vestRow1y;		gIMPGearInvData[SMALLPOCK1POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK1POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK1
		gIMPGearInvData[SMALLPOCK2POS].sX = x + 277;	gIMPGearInvData[SMALLPOCK2POS].sY = y + vestRow1y;		gIMPGearInvData[SMALLPOCK2POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK2POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK2
		gIMPGearInvData[SMALLPOCK3POS].sX = x + 313;	gIMPGearInvData[SMALLPOCK3POS].sY = y + vestRow1y;		gIMPGearInvData[SMALLPOCK3POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK3POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK3
		gIMPGearInvData[SMALLPOCK4POS].sX = x + 349;	gIMPGearInvData[SMALLPOCK4POS].sY = y + vestRow1y;		gIMPGearInvData[SMALLPOCK4POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK4POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK4
		gIMPGearInvData[SMALLPOCK5POS].sX = x + 385;	gIMPGearInvData[SMALLPOCK5POS].sY = y + vestRow1y;		gIMPGearInvData[SMALLPOCK5POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK5POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK5
		gIMPGearInvData[SMALLPOCK6POS].sX = x + 241;	gIMPGearInvData[SMALLPOCK6POS].sY = y + vestRow2y;		gIMPGearInvData[SMALLPOCK6POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK6POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK6
		gIMPGearInvData[SMALLPOCK7POS].sX = x + 277;	gIMPGearInvData[SMALLPOCK7POS].sY = y + vestRow2y;		gIMPGearInvData[SMALLPOCK7POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK7POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK7
		gIMPGearInvData[SMALLPOCK8POS].sX = x + 313;	gIMPGearInvData[SMALLPOCK8POS].sY = y + vestRow2y;		gIMPGearInvData[SMALLPOCK8POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK8POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK8	
		gIMPGearInvData[SMALLPOCK9POS].sX = x + 349;	gIMPGearInvData[SMALLPOCK9POS].sY = y + vestRow2y;		gIMPGearInvData[SMALLPOCK9POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK9POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK9
		gIMPGearInvData[SMALLPOCK10POS].sX = x + 385;	gIMPGearInvData[SMALLPOCK10POS].sY = y + vestRow2y;		gIMPGearInvData[SMALLPOCK10POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK10POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK10
		gIMPGearInvData[SMALLPOCK11POS].sX = x + 21;	gIMPGearInvData[SMALLPOCK11POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK11POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK11POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK11
		gIMPGearInvData[SMALLPOCK12POS].sX = x + 57;	gIMPGearInvData[SMALLPOCK12POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK12POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK12POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK12
		gIMPGearInvData[SMALLPOCK13POS].sX = x + 21;	gIMPGearInvData[SMALLPOCK13POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK13POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK13POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK13
		gIMPGearInvData[SMALLPOCK14POS].sX = x + 57;	gIMPGearInvData[SMALLPOCK14POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK14POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK14POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK14
		gIMPGearInvData[SMALLPOCK15POS].sX = x + 103;	gIMPGearInvData[SMALLPOCK15POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK15POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK15POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK15
		gIMPGearInvData[SMALLPOCK16POS].sX = x + 139;	gIMPGearInvData[SMALLPOCK16POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK16POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK16POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK16
		gIMPGearInvData[SMALLPOCK17POS].sX = x + 103;	gIMPGearInvData[SMALLPOCK17POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK17POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK17POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK17
		gIMPGearInvData[SMALLPOCK18POS].sX = x + 139;	gIMPGearInvData[SMALLPOCK18POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK18POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK18POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK18
		gIMPGearInvData[SMALLPOCK19POS].sX = x + 195;	gIMPGearInvData[SMALLPOCK19POS].sY = y + 128;			gIMPGearInvData[SMALLPOCK19POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK19POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK19
		gIMPGearInvData[SMALLPOCK20POS].sX = x + 231;	gIMPGearInvData[SMALLPOCK20POS].sY = y + 128;			gIMPGearInvData[SMALLPOCK20POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK20POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK20
		gIMPGearInvData[SMALLPOCK21POS].sX = x + 195;	gIMPGearInvData[SMALLPOCK21POS].sY = y + 152;			gIMPGearInvData[SMALLPOCK21POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK21POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK21
		gIMPGearInvData[SMALLPOCK22POS].sX = x + 231;	gIMPGearInvData[SMALLPOCK22POS].sY = y + 152;			gIMPGearInvData[SMALLPOCK22POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK22POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK22
		gIMPGearInvData[SMALLPOCK23POS].sX = x + 272;	gIMPGearInvData[SMALLPOCK23POS].sY = y + 128;			gIMPGearInvData[SMALLPOCK23POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK23POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK23
		gIMPGearInvData[SMALLPOCK24POS].sX = x + 272;	gIMPGearInvData[SMALLPOCK24POS].sY = y + 152;			gIMPGearInvData[SMALLPOCK24POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK24POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK24
		gIMPGearInvData[SMALLPOCK25POS].sX = x + 272;	gIMPGearInvData[SMALLPOCK25POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK25POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK25POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK25
		gIMPGearInvData[SMALLPOCK26POS].sX = x + 272;	gIMPGearInvData[SMALLPOCK26POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK26POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK26POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK26
		gIMPGearInvData[SMALLPOCK27POS].sX = x + 308;	gIMPGearInvData[SMALLPOCK27POS].sY = y + 128;			gIMPGearInvData[SMALLPOCK27POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK27POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK27
		gIMPGearInvData[SMALLPOCK28POS].sX = x + 308;	gIMPGearInvData[SMALLPOCK28POS].sY = y + 152;			gIMPGearInvData[SMALLPOCK28POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK28POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK28
		gIMPGearInvData[SMALLPOCK29POS].sX = x + 308;	gIMPGearInvData[SMALLPOCK29POS].sY = y + 176;			gIMPGearInvData[SMALLPOCK29POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK29POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK29
		gIMPGearInvData[SMALLPOCK30POS].sX = x + 308;	gIMPGearInvData[SMALLPOCK30POS].sY = y + 200;			gIMPGearInvData[SMALLPOCK30POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK30POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK30
	}
	else
	{
		gIMPGearInvData[HELMETPOS].sX = x + 205;		gIMPGearInvData[HELMETPOS].sY = y + 10;					gIMPGearInvData[HELMETPOS].sWidth = HEAD_INV_SLOT_WIDTH; gIMPGearInvData[HELMETPOS].sHeight = HEAD_INV_SLOT_HEIGHT;		// HELMETPOS
		gIMPGearInvData[VESTPOS].sX = x + 205;			gIMPGearInvData[VESTPOS].sY = y + 38;					gIMPGearInvData[VESTPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[VESTPOS].sHeight = VEST_INV_SLOT_HEIGHT; // VESTPOS
		gIMPGearInvData[LEGPOS].sX = x + 205;			gIMPGearInvData[LEGPOS].sY = y + 98;					gIMPGearInvData[LEGPOS].sWidth = LEGS_INV_SLOT_WIDTH; gIMPGearInvData[LEGPOS].sHeight = LEGS_INV_SLOT_HEIGHT; // LEGPOS
		gIMPGearInvData[HEAD1POS].sX = x + 21;			gIMPGearInvData[HEAD1POS].sY = y + 9;					gIMPGearInvData[HEAD1POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[HEAD1POS].sHeight = SM_INV_SLOT_HEIGHT; // HEAD1POS
		gIMPGearInvData[HEAD2POS].sX = x + 21;			gIMPGearInvData[HEAD2POS].sY = y + 33;					gIMPGearInvData[HEAD2POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[HEAD2POS].sHeight = SM_INV_SLOT_HEIGHT; // HEAD2POS
		gIMPGearInvData[HANDPOS].sX = x + 22;			gIMPGearInvData[HANDPOS].sY = y + 87;					gIMPGearInvData[HANDPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[HANDPOS].sHeight = BIG_INV_SLOT_HEIGHT; // HANDPOS
		gIMPGearInvData[SECONDHANDPOS].sX = x + 22;		gIMPGearInvData[SECONDHANDPOS].sY = y + 111;			gIMPGearInvData[SECONDHANDPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[SECONDHANDPOS].sHeight = BIG_INV_SLOT_HEIGHT; // SECONDHANDPOS
		gIMPGearInvData[GUNSLINGPOCKPOS].sX = x + 98;	gIMPGearInvData[GUNSLINGPOCKPOS].sY = y + 144;			gIMPGearInvData[GUNSLINGPOCKPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[GUNSLINGPOCKPOS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK1
		gIMPGearInvData[KNIFEPOCKPOS].sX = x + 98;		gIMPGearInvData[KNIFEPOCKPOS].sY = y + 169;				gIMPGearInvData[KNIFEPOCKPOS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[KNIFEPOCKPOS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK2
		gIMPGearInvData[MEDPOCK1POS].sX = x + 98;		gIMPGearInvData[MEDPOCK1POS].sY = y + 192;				gIMPGearInvData[MEDPOCK1POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK1POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK3
		gIMPGearInvData[MEDPOCK2POS].sX = x + 98;		gIMPGearInvData[MEDPOCK2POS].sY = y + 217;				gIMPGearInvData[MEDPOCK2POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK2POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK4
		gIMPGearInvData[SMALLPOCK1POS].sX = x + 23;		gIMPGearInvData[SMALLPOCK1POS].sY = y + 144;			gIMPGearInvData[SMALLPOCK1POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK1POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK1
		gIMPGearInvData[SMALLPOCK2POS].sX = x + 23;		gIMPGearInvData[SMALLPOCK2POS].sY = y + 168;			gIMPGearInvData[SMALLPOCK2POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK2POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK2
		gIMPGearInvData[SMALLPOCK3POS].sX = x + 23;		gIMPGearInvData[SMALLPOCK3POS].sY = y + 192;			gIMPGearInvData[SMALLPOCK3POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK3POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK3
		gIMPGearInvData[SMALLPOCK4POS].sX = x + 23;		gIMPGearInvData[SMALLPOCK4POS].sY = y + 216;			gIMPGearInvData[SMALLPOCK4POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK4POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK4
		gIMPGearInvData[SMALLPOCK11POS].sX = x + 61;		gIMPGearInvData[SMALLPOCK11POS].sY = y + 144;			gIMPGearInvData[SMALLPOCK11POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK11POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK5
		gIMPGearInvData[SMALLPOCK12POS].sX = x + 61;		gIMPGearInvData[SMALLPOCK12POS].sY = y + 168;			gIMPGearInvData[SMALLPOCK12POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK12POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK6
		gIMPGearInvData[SMALLPOCK13POS].sX = x + 61;		gIMPGearInvData[SMALLPOCK13POS].sY = y + 192;			gIMPGearInvData[SMALLPOCK13POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK13POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK7
		gIMPGearInvData[SMALLPOCK14POS].sX = x + 61;		gIMPGearInvData[SMALLPOCK14POS].sY = y + 216;			gIMPGearInvData[SMALLPOCK14POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK14POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK8	
		gIMPGearInvData[SMALLPOCK6POS].sX = x + 180;	gIMPGearInvData[SMALLPOCK6POS].sY = y + 144;			gIMPGearInvData[SMALLPOCK6POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK6POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK9
		gIMPGearInvData[SMALLPOCK7POS].sX = x + 180;	gIMPGearInvData[SMALLPOCK7POS].sY = y + 168;			gIMPGearInvData[SMALLPOCK7POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK7POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK10
		gIMPGearInvData[SMALLPOCK8POS].sX = x + 180;	gIMPGearInvData[SMALLPOCK8POS].sY = y + 192;			gIMPGearInvData[SMALLPOCK8POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK8POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK11
		gIMPGearInvData[SMALLPOCK9POS].sX = x + 180;	gIMPGearInvData[SMALLPOCK9POS].sY = y + 216;			gIMPGearInvData[SMALLPOCK9POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK9POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK12
		gIMPGearInvData[SMALLPOCK19POS].sX = x + 219;	gIMPGearInvData[SMALLPOCK19POS].sY = y + 144;			gIMPGearInvData[SMALLPOCK19POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK19POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK13
		gIMPGearInvData[SMALLPOCK20POS].sX = x + 219;	gIMPGearInvData[SMALLPOCK20POS].sY = y + 168;			gIMPGearInvData[SMALLPOCK20POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK20POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK14
		gIMPGearInvData[SMALLPOCK21POS].sX = x + 219;	gIMPGearInvData[SMALLPOCK21POS].sY = y + 192;			gIMPGearInvData[SMALLPOCK21POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK21POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK15
		gIMPGearInvData[SMALLPOCK22POS].sX = x + 219;	gIMPGearInvData[SMALLPOCK22POS].sY = y + 216;			gIMPGearInvData[SMALLPOCK22POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK22POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK16

		// Move unused slots to lower right corner so they are unreachable
		gIMPGearInvData[VESTPOCKPOS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[VESTPOCKPOS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[VESTPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[VESTPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // VESTPOCK
		gIMPGearInvData[LTHIGHPOCKPOS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[LTHIGHPOCKPOS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[LTHIGHPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[LTHIGHPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // LTHIGHPOCK
		gIMPGearInvData[RTHIGHPOCKPOS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[RTHIGHPOCKPOS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[RTHIGHPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[RTHIGHPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // RTHIGHPOCK
		gIMPGearInvData[CPACKPOCKPOS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[CPACKPOCKPOS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[CPACKPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[CPACKPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // CPACKPOCK
		gIMPGearInvData[BPACKPOCKPOS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BPACKPOCKPOS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BPACKPOCKPOS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[BPACKPOCKPOS].sHeight = VEST_INV_SLOT_HEIGHT; // BPACKPOCK
		gIMPGearInvData[BIGPOCK1POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK1POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK1POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK1POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK1
		gIMPGearInvData[BIGPOCK2POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK2POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK2POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK2POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK2
		gIMPGearInvData[BIGPOCK3POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK3POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK3POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK3POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK3
		gIMPGearInvData[BIGPOCK4POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK4POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK4POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK4POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK4
		gIMPGearInvData[BIGPOCK5POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK5POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK5POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK5POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK5
		gIMPGearInvData[BIGPOCK6POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK6POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK6POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK6POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK6
		gIMPGearInvData[BIGPOCK7POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[BIGPOCK7POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[BIGPOCK7POS].sWidth = BIG_INV_SLOT_WIDTH; gIMPGearInvData[BIGPOCK7POS].sHeight = BIG_INV_SLOT_HEIGHT; // BIGPOCK7
		gIMPGearInvData[MEDPOCK3POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[MEDPOCK3POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[MEDPOCK3POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK3POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK3
		gIMPGearInvData[MEDPOCK4POS].sX = SCREEN_WIDTH + 1;		gIMPGearInvData[MEDPOCK4POS].sY = SCREEN_HEIGHT + 1;				gIMPGearInvData[MEDPOCK4POS].sWidth = VEST_INV_SLOT_WIDTH; gIMPGearInvData[MEDPOCK4POS].sHeight = VEST_INV_SLOT_HEIGHT; // MEDPOCK4
		gIMPGearInvData[SMALLPOCK5POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK5POS].sY = SCREEN_HEIGHT + 1;		gIMPGearInvData[SMALLPOCK5POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK5POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK5
		gIMPGearInvData[SMALLPOCK10POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK10POS].sY = SCREEN_HEIGHT + 1;		gIMPGearInvData[SMALLPOCK10POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK10POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK10
		gIMPGearInvData[SMALLPOCK15POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK15POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK15POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK15POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK15
		gIMPGearInvData[SMALLPOCK16POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK16POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK16POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK16POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK16
		gIMPGearInvData[SMALLPOCK17POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK17POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK17POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK17POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK17
		gIMPGearInvData[SMALLPOCK18POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK18POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK18POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK18POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK18
		gIMPGearInvData[SMALLPOCK23POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK23POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK23POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK23POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK23
		gIMPGearInvData[SMALLPOCK24POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK24POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK24POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK24POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK24
		gIMPGearInvData[SMALLPOCK25POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK25POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK25POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK25POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK25
		gIMPGearInvData[SMALLPOCK26POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK26POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK26POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK26POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK26
		gIMPGearInvData[SMALLPOCK27POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK27POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK27POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK27POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK27
		gIMPGearInvData[SMALLPOCK28POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK28POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK28POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK28POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK28
		gIMPGearInvData[SMALLPOCK29POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK29POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK29POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK29POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK29
		gIMPGearInvData[SMALLPOCK30POS].sX = SCREEN_WIDTH + 1;	gIMPGearInvData[SMALLPOCK30POS].sY = SCREEN_HEIGHT + 1;			gIMPGearInvData[SMALLPOCK30POS].sWidth = SM_INV_SLOT_WIDTH; gIMPGearInvData[SMALLPOCK30POS].sHeight = SM_INV_SLOT_HEIGHT; // SMALLPOCK30
	}

	// Add regions for inventory pocket slots
	for (UINT8 cnt = 0; cnt < NUM_INV_SLOTS; ++cnt)
	{
		MSYS_DefineRegion(&gIMPGearInvRegion[cnt], 
			gIMPGearInvData[cnt].sX, gIMPGearInvData[cnt].sY, 
			(INT16)(gIMPGearInvData[cnt].sX + gIMPGearInvData[cnt].sWidth), (INT16)(gIMPGearInvData[cnt].sY + gIMPGearInvData[cnt].sHeight),
			MSYS_PRIORITY_HIGH, CURSOR_WWW, IMPInvMoveCallback, IMPInvClickCallback
		);

		// Add region
		MSYS_AddRegion(&gIMPGearInvRegion[cnt]);
		MSYS_SetRegionUserData(&gIMPGearInvRegion[cnt], 0, cnt);

		SetRegionFastHelpText(&gIMPGearInvRegion[cnt], szIMPGearPocketText[cnt]);
	}

	// Inventory pool slots
	for (size_t i = 0; i < 25; i++)
	{
		const UINT32 xOffset = gIMPInvPoolLayout.x + 24; // top left coords of the first item slot in selection grid sti
		const UINT32 yOffset = gIMPInvPoolLayout.y + 8;
		const UINT32 xStep = 72; // steps to the next slot column and row
		const UINT32 yStep = 32;
		const auto x = xOffset + (UINT8)((i) % 5) * xStep;
		const auto y = yOffset + (UINT8)((i) / 5) * yStep;

		MSYS_DefineRegion(&gIMPGearInvPoolRegion[i],
			x, y,
			x + 60, y + 22,
			MSYS_PRIORITY_HIGH, CURSOR_WWW, IMPInvMoveCallback, IMPInventoryPoolSelectCallback
		);

		// Add region
		MSYS_AddRegion(&gIMPGearInvPoolRegion[i]);
		MSYS_SetRegionUserData(&gIMPGearInvPoolRegion[i], 0, i);
		SetRegionFastHelpText(&gIMPGearInvPoolRegion[i], szIMPGearPocketText[55]);
		MSYS_DisableRegion(&gIMPGearInvPoolRegion[i]);
	}
}


void DisplayPagesForImpInventoryPool(void)
{
	// get the current and last pages and display them
	CHAR16 sString[32];
	INT16 sX, sY;

	SetFont(COMPFONT);
	SetFontForeground(183);
	SetFontBackground(FONT_BLACK);

	// set the buffer
	SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	// grab current and last pages
	swprintf(sString, L"%d / %d", gIMPCurrentInventoryPoolPage + 1, gIMPLastInventoryPoolPage + 1);

	// grab centered coords
	FindFontCenterCoordinates(
		gIMPInvPoolLayout.x + 170,
		gIMPInvPoolLayout.y + 190,
		46, 13, sString, BLOCKFONT2, &sX, &sY
	);

	mprintf(sX, sY, sString);

	SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);
}


void DrawItemTextToInvPool(STR16 itemName, UINT32 x, UINT32 y)
{
	// the name
	CHAR16 sString[64];
	wcscpy(sString, itemName);

	if (StringPixLength(sString, SMALLCOMPFONT) >= (65))
	{
		ReduceStringLength(sString, (INT16)(65 - StringPixLength(L" ...", SMALLCOMPFONT)), SMALLCOMPFONT);
	}

	INT16 sX, sY;
	FindFontCenterCoordinates(
		x,
		y,
		65, 6, sString, SMALLCOMPFONT, &sX, &sY
	);

	SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	SetFont(SMALLCOMPFONT);
	SetFontForeground(FONT_GRAY2);
	SetFontBackground(FONT_BLACK);

	mprintf(sX,	sY,	sString);
}


void RenderImpGearSelectionChoices(UINT32 pocket)
{
	const UINT32 xOffset = gIMPInvPoolLayout.x + 24; // top left coords of the first item slot in selection grid sti
	const UINT32 yOffset = gIMPInvPoolLayout.y + 8;
	const UINT32 xStep = 72; // steps to the next slot column and row
	const UINT32 yStep = 32;
	
	const UINT32 pageShift = gIMPCurrentInventoryPoolPage * 25;
	UINT32 end = gIMPPossibleItems[pocket].size();
	if (gIMPCurrentInventoryPoolPage < gIMPLastInventoryPoolPage)
	{
		end = pageShift + 25;
	}

	for (UINT32 i = 0 + pageShift; i < end; i++)
	{
		const auto item = gIMPPossibleItems[pocket][i].first;
		const auto x = xOffset + (UINT8)((i - pageShift)%5) * xStep;
		const auto y = yOffset + (UINT8)((i - pageShift)/5)* yStep;

		DisplayGear(item, x, y, FALSE, 1, FALSE);

		// Coordinates for item text
		const auto xText = x;
		const auto yText = y + 24;
		DrawItemTextToInvPool(gIMPPossibleItems[pocket][i].second, xText, yText);


		// Check if currently selected item is shown in pool and adjust glow coordinates
		if (item == gIMPPocketSelectedItems[pocket].first)
		{
			gIMPGlowX = x;
			gIMPGlowY = y;
			fShowIMPItemHighLight = TRUE;
		}
	}
}


void DrawImpGearInventoryPool(void)
{
	RenderImpGearSelectionGrid();
	ShowButton(gIMPInvDoneButton);
	ShowButton(gIMPInvArrowButton[0]);
	ShowButton(gIMPInvArrowButton[1]);
	DisplayPagesForImpInventoryPool();
	RenderImpGearSelectionChoices(gCurrentImpGearChoices);
}



void SetChoiceForPocket(INT32 pocket, INT32 index, UINT8 amount)
{
	if (gIMPPossibleItems[pocket].size() > index)
	{
		gIMPPocketSelectedItems[pocket] = std::make_pair(gIMPPossibleItems[pocket][index].first, amount);
	}
}


void DistributePossibleItemsToVectors(void)
{
	// these vectors contain the possible items in selection screen pockets
	gIMPPossibleItems.clear();

	// it is always possible to not select something
	for (int i = 0; i < NUM_INV_SLOTS; ++i)
	{
		gIMPPossibleItems[i].push_back(std::make_pair(0, pLongAssignmentStrings[40])); //Reusing AssignmentString for "Empty" text
		SetChoiceForPocket(i, 0, 1); // Initialize all selections to empty at first
	}

	std::set<UINT16>::iterator itend = gIMPGearPossibleItems.end();
	for (std::set<UINT16>::iterator it = gIMPGearPossibleItems.begin(); it != itend; ++it)
	{
		UINT16 usItem = (*it);

		if (Item[usItem].usItemClass & IC_GUN)
		{
			gIMPPossibleItems[HANDPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));

			if (Item[usItem].twohanded) {
				gIMPPossibleItems[GUNSLINGPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName)); 
			}
			else { 
				gIMPPossibleItems[SECONDHANDPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName)); 
			}
		}
		else if (Item[usItem].usItemClass & IC_LAUNCHER)
		{
			gIMPPossibleItems[GUNSLINGPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
		}
		else if (Item[usItem].usItemClass & (IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH))
		{
			gIMPPossibleItems[KNIFEPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
		}
		else if (Item[usItem].usItemClass & IC_ARMOUR)
		{
			if (Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET)
				gIMPPossibleItems[HELMETPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			else if (Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_VEST)
				gIMPPossibleItems[VESTPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			else if (Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS)
				gIMPPossibleItems[LEGPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
		}
		else if (Item[usItem].usItemClass & IC_LBEGEAR)
		{
			switch (LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass)
			{
			case THIGH_PACK:
				gIMPPossibleItems[LTHIGHPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				gIMPPossibleItems[RTHIGHPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				break;
			case VEST_PACK:
				gIMPPossibleItems[VESTPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				break;
			case COMBAT_PACK:
				gIMPPossibleItems[CPACKPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				break;
			case BACKPACK:
				gIMPPossibleItems[BPACKPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				break;
			default:
				gIMPPossibleItems[LTHIGHPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				gIMPPossibleItems[RTHIGHPOCKPOS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				break;
			}
		}
		else if (Item[usItem].usItemClass & IC_FACE)
		{
			gIMPPossibleItems[HEAD1POS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			gIMPPossibleItems[HEAD2POS].push_back(std::make_pair(usItem, Item[usItem].szItemName));
		}
		else if (Item[usItem].usItemClass & (IC_GRENADE | IC_BOMB))
		{
			// ignore item if it is a launchable - we already list it under ammo if a launcher is selected
			BOOLEAN found = FALSE;
			UINT16 iLoop = 0;
			while (Launchable[iLoop][0] != 0)
			{
				if (Launchable[iLoop][0] == usItem)
				{
					found = TRUE;
					break;
				}

				++iLoop;
			}

			if (!found)
			{
				for (size_t i = BIGPOCK1POS; i < MEDPOCK1POS; i++)
				{
					gIMPPossibleItems[i].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				}
				for (size_t i = SMALLPOCK19POS; i < NUM_INV_SLOTS; i++)
				{
					gIMPPossibleItems[i].push_back(std::make_pair(usItem, Item[usItem].szItemName));
				}
			}
		}
		else if (Item[usItem].usItemClass & (IC_MEDKIT | IC_KIT))
		{
			for (size_t i = MEDPOCK1POS; i < SMALLPOCK1POS; i++)
			{
				gIMPPossibleItems[i].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			}
		}
		else
		{
			for (size_t i = BIGPOCK1POS; i < MEDPOCK1POS; i++)
			{
				gIMPPossibleItems[i].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			}
			for (size_t i = SMALLPOCK19POS; i < NUM_INV_SLOTS; i++)
			{
				gIMPPossibleItems[i].push_back(std::make_pair(usItem, Item[usItem].szItemName));
			}
		}
	}


	//Assign default choices to select pockets
	SetChoiceForPocket(HELMETPOS, 1, 1);
	SetChoiceForPocket(VESTPOS, 1, 1);
	SetChoiceForPocket(LEGPOS, 1, 1);
	SetChoiceForPocket(HEAD1POS, 1, 1);
	SetChoiceForPocket(HEAD2POS, 2, 1);

	SetChoiceForPocket(HANDPOS, 1, 1);
	SetChoiceForPocket(KNIFEPOCKPOS, 1, 1);
	SetChoiceForPocket(MEDPOCK1POS, 1, 1);

	if (UsingNewInventorySystem())
	{
		SetChoiceForPocket(VESTPOCKPOS, 1, 1);
		SetChoiceForPocket(CPACKPOCKPOS, 1, 1);
		SetChoiceForPocket(BPACKPOCKPOS, 1, 1);
		SetChoiceForPocket(LTHIGHPOCKPOS, 1, 1);
		SetChoiceForPocket(RTHIGHPOCKPOS, 1, 1);

		SetChoiceForPocket(SMALLPOCK19POS, 4, 1);
		SetChoiceForPocket(SMALLPOCK20POS, 3, 1);
		SetChoiceForPocket(SMALLPOCK21POS, 2, 1);
		SetChoiceForPocket(SMALLPOCK22POS, 1, 1);
		SetChoiceForPocket(SMALLPOCK23POS, 1, 1);
	}
	else
	{
		SetChoiceForPocket(SMALLPOCK19POS, 4, 1);
		SetChoiceForPocket(SMALLPOCK20POS, 3, 1);
		SetChoiceForPocket(SMALLPOCK21POS, 2, 1);
		SetChoiceForPocket(SMALLPOCK22POS, 1, 1);
	}

}

void GlowImpInvPoolItem(void)
{
	static INT32 iColorNum = 10;
	static BOOLEAN fDelta = FALSE;
	static BOOLEAN fOldItemGlow = FALSE;
	UINT32 uiDestPitchBYTES;
	UINT8* pDestBuf;

	// not glowing right now, leave
	if (fShowIMPItemHighLight == FALSE)
	{
		iColorNum = 0;
		fDelta = TRUE;
		fOldItemGlow = FALSE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if (!gfGlowTimerExpired)
		return;


	fOldItemGlow = TRUE;

	// change direction of glow?
	if ((iColorNum == 0) || (iColorNum == 10))
	{
		fDelta = !fDelta;
	}

	// increment color
	if (!fDelta)
		iColorNum++;
	else
		iColorNum--;

	// restore background
	auto x = gIMPGlowX;
	auto y = gIMPGlowY;
	const auto width = 60;
	const auto height = 24;

	UINT16 usColor = Get16BPPColor(FROMRGB(GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue));
	pDestBuf = LockVideoSurface(FRAME_BUFFER, &uiDestPitchBYTES);
	SetClippingRegionAndImageWidth(uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	const auto xEnd = x + width;
	const auto yEnd = y + height;
	RectangleDraw(TRUE, x, y, xEnd, yEnd, usColor, pDestBuf);
	InvalidateRegion(x, y, xEnd + 1, yEnd + 1);
	UnLockVideoSurface(FRAME_BUFFER);
}

void RenderIMPGearBodytype(void)
{
	UINT32 index =2; // Regular male
	if (bBigBody)
	{
		index = 3; // Big male
	}
	if (!fCharacterIsMale)
	{
		index = 4; // Female
	}

	INT32 x = gIMPGearLayout.x + 31;
	INT32 y = gIMPGearLayout.y + 8;
	if (!UsingNewInventorySystem())
	{
		x = gIMPGearLayout.x + 71;
		y = gIMPGearLayout.y + 9;
	}

	BltVideoObjectFromIndex(FRAME_BUFFER, gIMPINVENTORY, index, x, y, VO_BLT_SRCTRANSPARENCY, NULL);
}