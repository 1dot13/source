#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "language defines.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <time.h>
	#include "sgp.h"
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "sysutil.h"
	#include "overhead.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "interface.h"
	#include "vsurface.h"
	#include "wcheck.h"
	#include "input.h"
	#include "Handle UI.h"
	#include "renderworld.h"
	#include "sys globals.h"
	#include "cursors.h"
	#include "radar screen.h"
	#include "worldman.h"
	#include "Font Control.h"
	#include "render dirty.h"
	#include "utilities.h"
	#include "Sound Control.h"
	#include "Interface Panels.h"
	#include "Animation Control.h"
	#include "Soldier Control.h"
	#include "pathai.h"
	#include "weapons.h"
	#include "lighting.h"
	#include "faces.h"
	#include "mapscreen.h"
	#include "message.h"
	#include "text.h"
	#include "Interface Items.h"
	#include "Font Control.h"
	#include "Cursor Control.h"
	#include "interface cursors.h"
	#include "interface utils.h"
	#include "interface items.h"
	#include "wordwrap.h"
	#include "interface control.h"
	#include "vobject_blitters.h"
	#include "world items.h"
	#include "points.h"
	#include "physics.h"
	#include "finances.h"
	#include "ui cursors.h"
	#include "handle ui.h"
	#include "ShopKeeper Interface.h"
	#include "dialogue control.h"
	#include "english.h"
	#include "keys.h"
	#include "Strategicmap.h"
	#include "Arms Dealer Init.h"
	#include "soldier macros.h"
	#include "game clock.h"
	#include "squads.h"
	#include "LaptopSave.h"
	#include "MessageBoxScreen.h"
	#include "Language Defines.h"
	#include "GameSettings.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Quests.h"
	#include "Map Screen Interface.h"
	#include "Campaign Types.h"
	#include "opplist.h"
	#include "los.h"
	#include "Map Screen Interface Map.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// HEADROCK: Defined two new color pointers for the Enhanced Description Box
#define		ITEMDESC_FONTPOSITIVE		FONT_MCOLOR_LTGREEN
#define		ITEMDESC_FONTNEGATIVE		FONT_MCOLOR_LTRED
#define		BULLET_WIDTH						3
#define		BULLET_GAP							5

// HEADROCK: Added these STIs for the icons used in the Enhanced Description Box
UINT32		guiItemInfoWeaponIcon;
UINT32		guiItemInfoAmmoIcon;
UINT32		guiItemInfoExplosiveIcon;
UINT32		guiItemInfoArmorIcon;
UINT32		guiItemInfoMiscIcon;

// HEADROCK: Start additional values.
INT16		BULLET_DRAW_X;
INT16		BULLET_DRAW_Y;
INT16		BULLET_AUTO_X;
INT16		BULLET_AUTO_Y;
INT16		BULLET_RELOAD_X;
INT16		BULLET_RELOAD_Y;
INT16		BULLET_MRELOAD_X;
INT16		BULLET_MRELOAD_Y;
INT16		BULLET_RANGE_X;
INT16		BULLET_RANGE_Y;
INT16		BULLET_DAMAGE_X;
INT16		BULLET_DAMAGE_Y;
INT16		BULLET_AUTOPEN_X;
INT16		BULLET_AUTOPEN_Y;
INT16		BULLET_AUTO5AP_X;
INT16		BULLET_AUTO5AP_Y;
INT16		BULLET_ACC_X;
INT16		BULLET_ACC_Y;
INT16		BULLET_RELIABILITY_X;
INT16		BULLET_RELIABILITY_Y;
INT16		BULLET_REPAIR_X;
INT16		BULLET_REPAIR_Y;
INT16		BULLET_LOUDNESS_X;
INT16		BULLET_LOUDNESS_Y;
INT16		BULLET_FLASH_X;
INT16		BULLET_FLASH_Y;
INT16		BULLET_TOHIT_X;
INT16		BULLET_TOHIT_Y;
INT16		BULLET_BESTLASERRANGE_X;
INT16		BULLET_BESTLASERRANGE_Y;
INT16		BULLET_AIMBONUS_X;
INT16		BULLET_AIMBONUS_Y;
INT16		BULLET_MINRANGEAIMBONUS_X;
INT16		BULLET_MINRANGEAIMBONUS_Y;
INT16		BULLET_BIPOD_X;
INT16		BULLET_BIPOD_Y;

// HEADROCK: Icon Locations for AMMO stats
INT16		BULLET_ARMORPEN_X;
INT16		BULLET_ARMORPEN_Y;
INT16		BULLET_TUMBLE_X;
INT16		BULLET_TUMBLE_Y;
INT16		BULLET_PREARMOR_X;
INT16		BULLET_PREARMOR_Y;
INT16		BULLET_TRACER_X;
INT16		BULLET_TRACER_Y;
INT16		BULLET_ANTITANK_X;
INT16		BULLET_ANTITANK_Y;
INT16		BULLET_LOCKBUSTER_X;
INT16		BULLET_LOCKBUSTER_Y;
INT16		BULLET_IGNOREARMOR_X;
INT16		BULLET_IGNOREARMOR_Y;
INT16		BULLET_ACIDIC_X;
INT16		BULLET_ACIDIC_Y;
INT16		BULLET_AMMORANGEBON_X;
INT16		BULLET_AMMORANGEBON_Y;
INT16		BULLET_AMMODAMAGEBON_X;
INT16		BULLET_AMMODAMAGEBON_Y;
INT16		BULLET_AMMOTOHITBON_X;
INT16		BULLET_AMMOTOHITBON_Y;
INT16		BULLET_AMMOBURSTBON_X;
INT16		BULLET_AMMOBURSTBON_Y;
INT16		BULLET_AMMORELIABILITYBON_X;
INT16		BULLET_AMMORELIABILITYBON_Y;
INT16		BULLET_AMMONOISESUPP_X;
INT16		BULLET_AMMONOISESUPP_Y;

// HEADROCK: Icon Locations for EXPLOSIVES stats
INT16		BULLET_EXPLOSIVEDAMAGE_X;
INT16		BULLET_EXPLOSIVEDAMAGE_Y;
INT16		BULLET_EXPLOSIVESTUNDAMAGE_X;
INT16		BULLET_EXPLOSIVESTUNDAMAGE_Y;
INT16		BULLET_EXPLOSIVELOUDNESS_X;
INT16		BULLET_EXPLOSIVELOUDNESS_Y;
INT16		BULLET_EXPLOSIVEVOLATILITY_X;
INT16		BULLET_EXPLOSIVEVOLATILITY_Y;
INT16		BULLET_EXPLOSIVERADIUS_X;
INT16		BULLET_EXPLOSIVERADIUS_Y;
INT16		BULLET_EXPLOSIVESTARTRADIUS_X;
INT16		BULLET_EXPLOSIVESTARTRADIUS_Y;
INT16		BULLET_EXPLOSIVEARROW_X;
INT16		BULLET_EXPLOSIVEARROW_Y;
INT16		BULLET_EXPLOSIVEENDRADIUS_X;
INT16		BULLET_EXPLOSIVEENDRADIUS_Y;
INT16		BULLET_EXPLOSIVEDURATION_X;
INT16		BULLET_EXPLOSIVEDURATION_Y;

// HEADROCK: Icon Locations for ARMOR stats
INT16		BULLET_ARMOR_X;
INT16		BULLET_ARMOR_Y;
INT16		BULLET_ARMORCOVERAGE_X;
INT16		BULLET_ARMORCOVERAGE_Y;
INT16		BULLET_ARMORDEGRADE_X;
INT16		BULLET_ARMORDEGRADE_Y;
INT16		BULLET_ARMORAPBONUS_X;
INT16		BULLET_ARMORAPBONUS_Y;
INT16		BULLET_ARMORAIMBONUS_X;
INT16		BULLET_ARMORAIMBONUS_Y;
INT16		BULLET_ARMORWOODCAMO_X;
INT16		BULLET_ARMORWOODCAMO_Y;
INT16		BULLET_ARMORURBANCAMO_X;
INT16		BULLET_ARMORURBANCAMO_Y;
INT16		BULLET_ARMORDESERTCAMO_X;
INT16		BULLET_ARMORDESERTCAMO_Y;
INT16		BULLET_ARMORSNOWCAMO_X;
INT16		BULLET_ARMORSNOWCAMO_Y;
INT16		BULLET_ARMORSTEALTH_X;
INT16		BULLET_ARMORSTEALTH_Y;
INT16		BULLET_ARMORVISIONRANGEBONUS_X;
INT16		BULLET_ARMORVISIONRANGEBONUS_Y;
INT16		BULLET_ARMORDAYVISIONRANGEBONUS_X;
INT16		BULLET_ARMORDAYVISIONRANGEBONUS_Y;
INT16		BULLET_ARMORNIGHTVISIONRANGEBONUS_X;
INT16		BULLET_ARMORNIGHTVISIONRANGEBONUS_Y;
INT16		BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_X;
INT16		BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_Y;
INT16		BULLET_ARMORCAVEVISIONRANGEBONUS_X;
INT16		BULLET_ARMORCAVEVISIONRANGEBONUS_Y;
INT16		BULLET_ARMORPERCENTTUNNELVISION_X;
INT16		BULLET_ARMORPERCENTTUNNELVISION_Y;
INT16		BULLET_ARMORHEARINGRANGEBONUS_X;
INT16		BULLET_ARMORHEARINGRANGEBONUS_Y;

// HEADROCK: Generic Icon Locations for MISC Item Stats
INT16		BULLET_COL1ROW1_X;
INT16		BULLET_COL1ROW1_Y;
INT16		BULLET_COL1ROW2_X;
INT16		BULLET_COL1ROW2_Y;
INT16		BULLET_COL1ROW3_X;
INT16		BULLET_COL1ROW3_Y;
INT16		BULLET_COL1ROW4_X;
INT16		BULLET_COL1ROW4_Y;
INT16		BULLET_COL1ROW5_X;
INT16		BULLET_COL1ROW5_Y;
INT16		BULLET_COL1ROW6_X;
INT16		BULLET_COL1ROW6_Y;
INT16		BULLET_COL1ROW7_X;
INT16		BULLET_COL1ROW7_Y;
INT16		BULLET_COL1ROW8_X;
INT16		BULLET_COL1ROW8_Y;
INT16		BULLET_COL1ROW9_X;
INT16		BULLET_COL1ROW9_Y;
INT16		BULLET_COL1ROW10_X;
INT16		BULLET_COL1ROW10_Y;
INT16		BULLET_COL1ROW11_X;
INT16		BULLET_COL1ROW11_Y;
INT16		BULLET_COL1ROW12_X;
INT16		BULLET_COL1ROW12_Y;
INT16		BULLET_COL2ROW1_X;
INT16		BULLET_COL2ROW1_Y;
INT16		BULLET_COL2ROW2_X;
INT16		BULLET_COL2ROW2_Y;
INT16		BULLET_COL2ROW3_X;
INT16		BULLET_COL2ROW3_Y;
INT16		BULLET_COL2ROW4_X;
INT16		BULLET_COL2ROW4_Y;
INT16		BULLET_COL2ROW5_X;
INT16		BULLET_COL2ROW5_Y;
INT16		BULLET_COL2ROW6_X;
INT16		BULLET_COL2ROW6_Y;
INT16		BULLET_COL2ROW7_X;
INT16		BULLET_COL2ROW7_Y;
// HEADROCK: End additional values.

// HEADROCK: New regions for stats tooltips
MOUSE_REGION		gStatsFasthelpRegions[20];

extern INV_DESC_STATS gWeaponStats[];
extern INV_DESC_STATS gMoneyStats[];
extern SGPRect gItemDescProsConsRects[];
extern INV_DESC_STATS gLBEStats[];
extern INV_ATTACHXY gItemDescAttachmentsXY[];

// HEADROCK: New array to contain locations for MiscItem stats.
INV_DESC_STATS gMiscItemStatsEDB[28];

// WEAPON stat tooltips
SGPRect gWeaponDescStatsRects[20];

// Ammo Stat Tooltips
SGPRect gAmmoDescStatsRects[20];

// ARMOR Stat Tooltips
SGPRect gArmorDescStatsRects[20];

// EXPLOSIVES Stat Tooltips
SGPRect gExplosiveDescStatsRects[20];

// MISC ITEM Stat Tooltips
SGPRect gMiscItemDescStatsRects[20];

extern void HelpTextDoneCallback( void );

UINT8 UsingEDBSystem()
{
	if(iResolution == 0)	//640x480, never allow EDB
		return 0;
	if(gGameSettings.fOptions[ TOPTION_ENHANCED_DESC_BOX ] == TRUE)
	{
		if(guiCurrentScreen == MAP_SCREEN)	//Strategic
		{
			if(gGameExternalOptions.fEnhancedDescriptionBox == 0)
				return 1;
			if(gGameExternalOptions.fEnhancedDescriptionBox == 1)
				return 1;
		}
		else	//Tactical
		{
			if(gGameExternalOptions.fEnhancedDescriptionBox == 0)
				return (UsingNewInventorySystem()==true?1:2);
			if(gGameExternalOptions.fEnhancedDescriptionBox == 2)
				return (UsingNewInventorySystem()==true?1:2);
		}
	}

	return 0;
}

void InitEDBCoords()
{
	if(UsingEDBSystem() == 0)
		return;
	if(guiCurrentScreen == MAP_SCREEN)
	{
		// Icon Locations
		gMiscItemStatsEDB[0].sX = 47;	gMiscItemStatsEDB[0].sY = 124;	gMiscItemStatsEDB[0].sValDx = 0;	// COL 1; ROW 1
		gMiscItemStatsEDB[1].sX = 47;	gMiscItemStatsEDB[1].sY = 136;	gMiscItemStatsEDB[1].sValDx = 0;	// COL 1; ROW 2
		gMiscItemStatsEDB[2].sX = 47;	gMiscItemStatsEDB[2].sY = 148;	gMiscItemStatsEDB[2].sValDx = 0;	// COL 1; ROW 3
		gMiscItemStatsEDB[3].sX = 47;	gMiscItemStatsEDB[3].sY = 160;	gMiscItemStatsEDB[3].sValDx = 0;	// COL 1; ROW 4
		gMiscItemStatsEDB[4].sX = 47;	gMiscItemStatsEDB[4].sY = 172;	gMiscItemStatsEDB[4].sValDx = 0;	// COL 1; ROW 5
		gMiscItemStatsEDB[5].sX = 47;	gMiscItemStatsEDB[5].sY = 184;	gMiscItemStatsEDB[5].sValDx = 0;	// COL 1; ROW 6
		gMiscItemStatsEDB[6].sX = 47;	gMiscItemStatsEDB[6].sY = 196;	gMiscItemStatsEDB[6].sValDx = 0;	// COL 1; ROW 7
		gMiscItemStatsEDB[7].sX = 144;	gMiscItemStatsEDB[7].sY = 124;	gMiscItemStatsEDB[7].sValDx = 0;	// COL 2; ROW 1
		gMiscItemStatsEDB[8].sX = 144;	gMiscItemStatsEDB[8].sY = 136;	gMiscItemStatsEDB[8].sValDx = 0;	// COL 2; ROW 2
		gMiscItemStatsEDB[9].sX = 144;	gMiscItemStatsEDB[9].sY = 148;	gMiscItemStatsEDB[9].sValDx = 0;	// COL 2; ROW 3
		gMiscItemStatsEDB[10].sX = 144;	gMiscItemStatsEDB[10].sY = 160;	gMiscItemStatsEDB[10].sValDx = 0;	// COL 2; ROW 4
		gMiscItemStatsEDB[11].sX = 144;	gMiscItemStatsEDB[11].sY = 172;	gMiscItemStatsEDB[11].sValDx = 0;	// COL 2; ROW 5
		gMiscItemStatsEDB[12].sX = 144;	gMiscItemStatsEDB[12].sY = 184;	gMiscItemStatsEDB[12].sValDx = 0;	// COL 2; ROW 6
		gMiscItemStatsEDB[13].sX = 144;	gMiscItemStatsEDB[13].sY = 196;	gMiscItemStatsEDB[13].sValDx = 0;	// COL 2; ROW 7
		// EQUALS signs Locations
		gMiscItemStatsEDB[14].sX = 97;	gMiscItemStatsEDB[14].sY = 124;	gMiscItemStatsEDB[14].sValDx = 12;	// COL 1; ROW 1
		gMiscItemStatsEDB[15].sX = 97;	gMiscItemStatsEDB[15].sY = 136;	gMiscItemStatsEDB[15].sValDx = 12;	// COL 1; ROW 2
		gMiscItemStatsEDB[16].sX = 97;	gMiscItemStatsEDB[16].sY = 148;	gMiscItemStatsEDB[16].sValDx = 12;	// COL 1; ROW 3
		gMiscItemStatsEDB[17].sX = 97;	gMiscItemStatsEDB[17].sY = 160;	gMiscItemStatsEDB[17].sValDx = 12;	// COL 1; ROW 4
		gMiscItemStatsEDB[18].sX = 97;	gMiscItemStatsEDB[18].sY = 172;	gMiscItemStatsEDB[18].sValDx = 12;	// COL 1; ROW 5
		gMiscItemStatsEDB[19].sX = 97;	gMiscItemStatsEDB[19].sY = 184;	gMiscItemStatsEDB[19].sValDx = 12;	// COL 1; ROW 6
		gMiscItemStatsEDB[20].sX = 97;	gMiscItemStatsEDB[20].sY = 196;	gMiscItemStatsEDB[20].sValDx = 12;	// COL 1; ROW 7
		gMiscItemStatsEDB[21].sX = 194;	gMiscItemStatsEDB[21].sY = 124;	gMiscItemStatsEDB[21].sValDx = 12;	// COL 2; ROW 1
		gMiscItemStatsEDB[22].sX = 194;	gMiscItemStatsEDB[22].sY = 136;	gMiscItemStatsEDB[22].sValDx = 12;	// COL 2; ROW 2
		gMiscItemStatsEDB[23].sX = 194;	gMiscItemStatsEDB[23].sY = 148;	gMiscItemStatsEDB[23].sValDx = 12;	// COL 2; ROW 3
		gMiscItemStatsEDB[24].sX = 194;	gMiscItemStatsEDB[24].sY = 160;	gMiscItemStatsEDB[24].sValDx = 12;	// COL 2; ROW 4
		gMiscItemStatsEDB[25].sX = 194;	gMiscItemStatsEDB[25].sY = 172;	gMiscItemStatsEDB[25].sValDx = 12;	// COL 2; ROW 5
		gMiscItemStatsEDB[26].sX = 194;	gMiscItemStatsEDB[26].sY = 184;	gMiscItemStatsEDB[26].sValDx = 12;	// COL 2; ROW 6
		gMiscItemStatsEDB[27].sX = 194;	gMiscItemStatsEDB[27].sY = 196;	gMiscItemStatsEDB[27].sValDx = 12;	// COL 2; ROW 7

		gWeaponDescStatsRects[0].iLeft = 46;	gWeaponDescStatsRects[0].iTop = 229;	gWeaponDescStatsRects[0].iRight = 103;	gWeaponDescStatsRects[0].iBottom = 240; // First Column
		gWeaponDescStatsRects[1].iLeft = 46;	gWeaponDescStatsRects[1].iTop = 241;	gWeaponDescStatsRects[1].iRight = 103;	gWeaponDescStatsRects[1].iBottom = 252;
		gWeaponDescStatsRects[2].iLeft = 46;	gWeaponDescStatsRects[2].iTop = 253;	gWeaponDescStatsRects[2].iRight = 103;	gWeaponDescStatsRects[2].iBottom = 264;
		gWeaponDescStatsRects[3].iLeft = 46;	gWeaponDescStatsRects[3].iTop = 265;	gWeaponDescStatsRects[3].iRight = 103;	gWeaponDescStatsRects[3].iBottom = 276;
		gWeaponDescStatsRects[4].iLeft = 46;	gWeaponDescStatsRects[4].iTop = 277;	gWeaponDescStatsRects[4].iRight = 103;	gWeaponDescStatsRects[4].iBottom = 288;
		gWeaponDescStatsRects[5].iLeft = 46;	gWeaponDescStatsRects[5].iTop = 289;	gWeaponDescStatsRects[5].iRight = 103;	gWeaponDescStatsRects[5].iBottom = 300;
		gWeaponDescStatsRects[6].iLeft = 46;	gWeaponDescStatsRects[6].iTop = 301;	gWeaponDescStatsRects[6].iRight = 103;	gWeaponDescStatsRects[6].iBottom = 312;
		gWeaponDescStatsRects[7].iLeft = 111;	gWeaponDescStatsRects[7].iTop = 241;	gWeaponDescStatsRects[7].iRight = 168;	gWeaponDescStatsRects[7].iBottom = 252; // Second Column
		gWeaponDescStatsRects[8].iLeft = 111;	gWeaponDescStatsRects[8].iTop = 253;	gWeaponDescStatsRects[8].iRight = 168;	gWeaponDescStatsRects[8].iBottom = 264;
		gWeaponDescStatsRects[9].iLeft = 111;	gWeaponDescStatsRects[9].iTop = 265;	gWeaponDescStatsRects[9].iRight = 168;	gWeaponDescStatsRects[9].iBottom = 276;
		gWeaponDescStatsRects[10].iLeft = 111;	gWeaponDescStatsRects[10].iTop = 277;	gWeaponDescStatsRects[10].iRight = 168;	gWeaponDescStatsRects[10].iBottom = 288;
		gWeaponDescStatsRects[11].iLeft = 111;	gWeaponDescStatsRects[11].iTop = 289;	gWeaponDescStatsRects[11].iRight = 168;	gWeaponDescStatsRects[11].iBottom = 300;
		gWeaponDescStatsRects[12].iLeft = 111;	gWeaponDescStatsRects[12].iTop = 301;	gWeaponDescStatsRects[12].iRight = 168;	gWeaponDescStatsRects[12].iBottom = 312;
		gWeaponDescStatsRects[13].iLeft = 173;	gWeaponDescStatsRects[13].iTop = 229;	gWeaponDescStatsRects[13].iRight = 230;	gWeaponDescStatsRects[13].iBottom = 240; // Third Column
		gWeaponDescStatsRects[14].iLeft = 173;	gWeaponDescStatsRects[14].iTop = 241;	gWeaponDescStatsRects[14].iRight = 230;	gWeaponDescStatsRects[14].iBottom = 252;
		gWeaponDescStatsRects[15].iLeft = 173;	gWeaponDescStatsRects[15].iTop = 253;	gWeaponDescStatsRects[15].iRight = 230;	gWeaponDescStatsRects[15].iBottom = 264;
		gWeaponDescStatsRects[16].iLeft = 173;	gWeaponDescStatsRects[16].iTop = 265;	gWeaponDescStatsRects[16].iRight = 230;	gWeaponDescStatsRects[16].iBottom = 276;
		gWeaponDescStatsRects[17].iLeft = 173;	gWeaponDescStatsRects[17].iTop = 277;	gWeaponDescStatsRects[17].iRight = 230;	gWeaponDescStatsRects[17].iBottom = 288;
		gWeaponDescStatsRects[18].iLeft = 173;	gWeaponDescStatsRects[18].iTop = 289;	gWeaponDescStatsRects[18].iRight = 230;	gWeaponDescStatsRects[18].iBottom = 300;
		gWeaponDescStatsRects[19].iLeft = 173;	gWeaponDescStatsRects[19].iTop = 301;	gWeaponDescStatsRects[19].iRight = 230;	gWeaponDescStatsRects[19].iBottom = 312;

		gAmmoDescStatsRects[0].iLeft = 46;		gAmmoDescStatsRects[0].iTop = 229;	gAmmoDescStatsRects[0].iRight = 103;	gAmmoDescStatsRects[0].iBottom = 240;	// First Column
		gAmmoDescStatsRects[1].iLeft = 46;		gAmmoDescStatsRects[1].iTop = 241;	gAmmoDescStatsRects[1].iRight = 103;	gAmmoDescStatsRects[1].iBottom = 252;
		gAmmoDescStatsRects[2].iLeft = 46;		gAmmoDescStatsRects[2].iTop = 253;	gAmmoDescStatsRects[2].iRight = 103;	gAmmoDescStatsRects[2].iBottom = 264;
		gAmmoDescStatsRects[3].iLeft = 113;		gAmmoDescStatsRects[3].iTop = 229;	gAmmoDescStatsRects[3].iRight = 135;	gAmmoDescStatsRects[3].iBottom = 252;	// Second Column
		gAmmoDescStatsRects[4].iLeft = 140;		gAmmoDescStatsRects[4].iTop = 229;	gAmmoDescStatsRects[4].iRight = 165;	gAmmoDescStatsRects[4].iBottom = 252;
		gAmmoDescStatsRects[5].iLeft = 113;		gAmmoDescStatsRects[5].iTop = 253;	gAmmoDescStatsRects[5].iRight = 135;	gAmmoDescStatsRects[5].iBottom = 276;
		gAmmoDescStatsRects[6].iLeft = 140;		gAmmoDescStatsRects[6].iTop = 253;	gAmmoDescStatsRects[6].iRight = 165;	gAmmoDescStatsRects[6].iBottom = 276;
		gAmmoDescStatsRects[7].iLeft = 113;		gAmmoDescStatsRects[7].iTop = 277;	gAmmoDescStatsRects[7].iRight = 135;	gAmmoDescStatsRects[7].iBottom = 300;
		gAmmoDescStatsRects[8].iLeft = 173;		gAmmoDescStatsRects[8].iTop = 229;	gAmmoDescStatsRects[8].iRight = 230;	gAmmoDescStatsRects[8].iBottom = 240;	// Third Column
		gAmmoDescStatsRects[9].iLeft = 173;		gAmmoDescStatsRects[9].iTop = 241;	gAmmoDescStatsRects[9].iRight = 230;	gAmmoDescStatsRects[9].iBottom = 252;
		gAmmoDescStatsRects[10].iLeft = 173;	gAmmoDescStatsRects[10].iTop = 253;	gAmmoDescStatsRects[10].iRight = 230;	gAmmoDescStatsRects[10].iBottom = 264;
		gAmmoDescStatsRects[11].iLeft = 173;	gAmmoDescStatsRects[11].iTop = 265;	gAmmoDescStatsRects[11].iRight = 230;	gAmmoDescStatsRects[11].iBottom = 276;
		gAmmoDescStatsRects[12].iLeft = 173;	gAmmoDescStatsRects[12].iTop = 277;	gAmmoDescStatsRects[12].iRight = 230;	gAmmoDescStatsRects[12].iBottom = 288;	// Fourth Column
		gAmmoDescStatsRects[13].iLeft = 173;	gAmmoDescStatsRects[13].iTop = 289;	gAmmoDescStatsRects[13].iRight = 230;	gAmmoDescStatsRects[13].iBottom = 300;
		gAmmoDescStatsRects[14].iLeft = 0;		gAmmoDescStatsRects[14].iTop = 0;	gAmmoDescStatsRects[14].iRight = 1;		gAmmoDescStatsRects[14].iBottom = 1;	// Placeholders
		gAmmoDescStatsRects[15].iLeft = 0;		gAmmoDescStatsRects[15].iTop = 0;	gAmmoDescStatsRects[15].iRight = 1;		gAmmoDescStatsRects[15].iBottom = 1;
		gAmmoDescStatsRects[16].iLeft = 0;		gAmmoDescStatsRects[16].iTop = 0;	gAmmoDescStatsRects[16].iRight = 1;		gAmmoDescStatsRects[16].iBottom = 1;
		gAmmoDescStatsRects[17].iLeft = 0;		gAmmoDescStatsRects[17].iTop = 0;	gAmmoDescStatsRects[17].iRight = 1;		gAmmoDescStatsRects[17].iBottom = 1;
		gAmmoDescStatsRects[18].iLeft = 0;		gAmmoDescStatsRects[18].iTop = 0;	gAmmoDescStatsRects[18].iRight = 1;		gAmmoDescStatsRects[18].iBottom = 1;
		gAmmoDescStatsRects[19].iLeft = 0;		gAmmoDescStatsRects[19].iTop = 0;	gAmmoDescStatsRects[19].iRight = 1;		gAmmoDescStatsRects[19].iBottom = 1;

        gArmorDescStatsRects[0].iLeft = 46;		gArmorDescStatsRects[0].iTop = 229;		gArmorDescStatsRects[0].iRight = 103;	gArmorDescStatsRects[0].iBottom = 240;    // First Column
        gArmorDescStatsRects[1].iLeft = 46;		gArmorDescStatsRects[1].iTop = 241;		gArmorDescStatsRects[1].iRight = 103;	gArmorDescStatsRects[1].iBottom = 252;
        gArmorDescStatsRects[2].iLeft = 46;		gArmorDescStatsRects[2].iTop = 253;		gArmorDescStatsRects[2].iRight = 103;	gArmorDescStatsRects[2].iBottom = 264;
        gArmorDescStatsRects[3].iLeft = 111;	gArmorDescStatsRects[3].iTop = 229;		gArmorDescStatsRects[3].iRight = 168;	gArmorDescStatsRects[3].iBottom = 240;    // Second Column
        gArmorDescStatsRects[4].iLeft = 111;	gArmorDescStatsRects[4].iTop = 241;		gArmorDescStatsRects[4].iRight = 168;	gArmorDescStatsRects[4].iBottom = 252;
		gArmorDescStatsRects[5].iLeft = 111;	gArmorDescStatsRects[5].iTop = 253;		gArmorDescStatsRects[5].iRight = 168;	gArmorDescStatsRects[5].iBottom = 264;
		gArmorDescStatsRects[6].iLeft = 111;	gArmorDescStatsRects[6].iTop = 265;		gArmorDescStatsRects[6].iRight = 168;	gArmorDescStatsRects[6].iBottom = 276;
		gArmorDescStatsRects[7].iLeft = 111;	gArmorDescStatsRects[7].iTop = 277;		gArmorDescStatsRects[7].iRight = 168;	gArmorDescStatsRects[7].iBottom = 288;
		gArmorDescStatsRects[8].iLeft = 111;	gArmorDescStatsRects[8].iTop = 289;		gArmorDescStatsRects[8].iRight = 168;	gArmorDescStatsRects[8].iBottom = 300;
		gArmorDescStatsRects[9].iLeft = 111;	gArmorDescStatsRects[9].iTop = 301;		gArmorDescStatsRects[9].iRight = 168;	gArmorDescStatsRects[9].iBottom = 312;
		gArmorDescStatsRects[10].iLeft = 173;	gArmorDescStatsRects[10].iTop = 229;	gArmorDescStatsRects[10].iRight = 230;	gArmorDescStatsRects[10].iBottom = 240;	// Third Column
		gArmorDescStatsRects[11].iLeft = 173;	gArmorDescStatsRects[11].iTop = 241;	gArmorDescStatsRects[11].iRight = 230;	gArmorDescStatsRects[11].iBottom = 252;
		gArmorDescStatsRects[12].iLeft = 173;	gArmorDescStatsRects[12].iTop = 253;	gArmorDescStatsRects[12].iRight = 230;	gArmorDescStatsRects[12].iBottom = 264;
		gArmorDescStatsRects[13].iLeft = 173;	gArmorDescStatsRects[13].iTop = 265;	gArmorDescStatsRects[13].iRight = 230;	gArmorDescStatsRects[13].iBottom = 276;
		gArmorDescStatsRects[14].iLeft = 173;	gArmorDescStatsRects[14].iTop = 277;	gArmorDescStatsRects[14].iRight = 230;	gArmorDescStatsRects[14].iBottom = 288;
		gArmorDescStatsRects[15].iLeft = 173;	gArmorDescStatsRects[15].iTop = 289;	gArmorDescStatsRects[15].iRight = 230;	gArmorDescStatsRects[15].iBottom = 300;
		gArmorDescStatsRects[16].iLeft = 173;	gArmorDescStatsRects[16].iTop = 301;	gArmorDescStatsRects[16].iRight = 230;	gArmorDescStatsRects[16].iBottom = 312;
		gArmorDescStatsRects[17].iLeft = 0;		gArmorDescStatsRects[17].iTop = 0;		gArmorDescStatsRects[17].iRight = 1;	gArmorDescStatsRects[17].iBottom = 1;	// Placeholders
		gArmorDescStatsRects[18].iLeft = 0;		gArmorDescStatsRects[18].iTop = 0;		gArmorDescStatsRects[18].iRight = 1;	gArmorDescStatsRects[18].iBottom = 1;
		gArmorDescStatsRects[19].iLeft = 0;		gArmorDescStatsRects[19].iTop = 0;		gArmorDescStatsRects[19].iRight = 1;	gArmorDescStatsRects[19].iBottom = 1;

		gExplosiveDescStatsRects[0].iLeft = 46;		gExplosiveDescStatsRects[0].iTop = 229;	gExplosiveDescStatsRects[0].iRight = 103;	gExplosiveDescStatsRects[0].iBottom = 240;	// First Column
		gExplosiveDescStatsRects[1].iLeft = 46;		gExplosiveDescStatsRects[1].iTop = 241;	gExplosiveDescStatsRects[1].iRight = 103;	gExplosiveDescStatsRects[1].iBottom = 252;
		gExplosiveDescStatsRects[2].iLeft = 46;		gExplosiveDescStatsRects[2].iTop = 253;	gExplosiveDescStatsRects[2].iRight = 103;	gExplosiveDescStatsRects[2].iBottom = 264;
		gExplosiveDescStatsRects[3].iLeft = 46;		gExplosiveDescStatsRects[3].iTop = 265;	gExplosiveDescStatsRects[3].iRight = 103;	gExplosiveDescStatsRects[3].iBottom = 276;
		gExplosiveDescStatsRects[4].iLeft = 131;	gExplosiveDescStatsRects[4].iTop = 229;	gExplosiveDescStatsRects[4].iRight = 147;	gExplosiveDescStatsRects[4].iBottom = 264;	// Second Column
		gExplosiveDescStatsRects[5].iLeft = 113;	gExplosiveDescStatsRects[5].iTop = 229;	gExplosiveDescStatsRects[5].iRight = 129;	gExplosiveDescStatsRects[5].iBottom = 264;
		gExplosiveDescStatsRects[6].iLeft = 145;	gExplosiveDescStatsRects[6].iTop = 229;	gExplosiveDescStatsRects[6].iRight = 165;	gExplosiveDescStatsRects[6].iBottom = 264;
		gExplosiveDescStatsRects[7].iLeft = 111;	gExplosiveDescStatsRects[7].iTop = 277;	gExplosiveDescStatsRects[7].iRight = 168;	gExplosiveDescStatsRects[7].iBottom = 288;
		gExplosiveDescStatsRects[8].iLeft = 0;		gExplosiveDescStatsRects[8].iTop = 0;	gExplosiveDescStatsRects[8].iRight = 1;		gExplosiveDescStatsRects[8].iBottom = 1;	// Placeholders
		gExplosiveDescStatsRects[9].iLeft = 0;		gExplosiveDescStatsRects[9].iTop = 0;	gExplosiveDescStatsRects[9].iRight = 1;		gExplosiveDescStatsRects[9].iBottom = 1;
		gExplosiveDescStatsRects[10].iLeft = 0;		gExplosiveDescStatsRects[10].iTop = 0;	gExplosiveDescStatsRects[10].iRight = 1;	gExplosiveDescStatsRects[10].iBottom = 1;
		gExplosiveDescStatsRects[11].iLeft = 0;		gExplosiveDescStatsRects[11].iTop = 0;	gExplosiveDescStatsRects[11].iRight = 1;	gExplosiveDescStatsRects[11].iBottom = 1;
		gExplosiveDescStatsRects[12].iLeft = 0;		gExplosiveDescStatsRects[12].iTop = 0;	gExplosiveDescStatsRects[12].iRight = 1;	gExplosiveDescStatsRects[12].iBottom = 1;
		gExplosiveDescStatsRects[13].iLeft = 0;		gExplosiveDescStatsRects[13].iTop = 0;	gExplosiveDescStatsRects[13].iRight = 1;	gExplosiveDescStatsRects[13].iBottom = 1;
		gExplosiveDescStatsRects[14].iLeft = 0;		gExplosiveDescStatsRects[14].iTop = 0;	gExplosiveDescStatsRects[14].iRight = 1;	gExplosiveDescStatsRects[14].iBottom = 1;
		gExplosiveDescStatsRects[15].iLeft = 0;		gExplosiveDescStatsRects[15].iTop = 0;	gExplosiveDescStatsRects[15].iRight = 1;	gExplosiveDescStatsRects[15].iBottom = 1;
		gExplosiveDescStatsRects[16].iLeft = 0;		gExplosiveDescStatsRects[16].iTop = 0;	gExplosiveDescStatsRects[16].iRight = 1;	gExplosiveDescStatsRects[16].iBottom = 1;
		gExplosiveDescStatsRects[17].iLeft = 0;		gExplosiveDescStatsRects[17].iTop = 0;	gExplosiveDescStatsRects[17].iRight = 1;	gExplosiveDescStatsRects[17].iBottom = 1;
		gExplosiveDescStatsRects[18].iLeft = 0;		gExplosiveDescStatsRects[18].iTop = 0;	gExplosiveDescStatsRects[18].iRight = 1;	gExplosiveDescStatsRects[18].iBottom = 1;
		gExplosiveDescStatsRects[19].iLeft = 0;		gExplosiveDescStatsRects[19].iTop = 0;	gExplosiveDescStatsRects[19].iRight = 1;	gExplosiveDescStatsRects[19].iBottom = 1;

		gMiscItemDescStatsRects[0].iLeft = 46;		gMiscItemDescStatsRects[0].iTop = 229;	gMiscItemDescStatsRects[0].iRight = 103;	gMiscItemDescStatsRects[0].iBottom = 240; // First Column
		gMiscItemDescStatsRects[1].iLeft = 46;		gMiscItemDescStatsRects[1].iTop = 241;	gMiscItemDescStatsRects[1].iRight = 103;	gMiscItemDescStatsRects[1].iBottom = 252;
		gMiscItemDescStatsRects[2].iLeft = 46;		gMiscItemDescStatsRects[2].iTop = 253;	gMiscItemDescStatsRects[2].iRight = 103;	gMiscItemDescStatsRects[2].iBottom = 264;
		gMiscItemDescStatsRects[3].iLeft = 46;		gMiscItemDescStatsRects[3].iTop = 265;	gMiscItemDescStatsRects[3].iRight = 103;	gMiscItemDescStatsRects[3].iBottom = 276;
		gMiscItemDescStatsRects[4].iLeft = 46;		gMiscItemDescStatsRects[4].iTop = 277;	gMiscItemDescStatsRects[4].iRight = 103;	gMiscItemDescStatsRects[4].iBottom = 288;
		gMiscItemDescStatsRects[5].iLeft = 46;		gMiscItemDescStatsRects[5].iTop = 289;	gMiscItemDescStatsRects[5].iRight = 103;	gMiscItemDescStatsRects[5].iBottom = 300;
		gMiscItemDescStatsRects[6].iLeft = 46;		gMiscItemDescStatsRects[6].iTop = 301;	gMiscItemDescStatsRects[6].iRight = 103;	gMiscItemDescStatsRects[6].iBottom = 312;
		gMiscItemDescStatsRects[7].iLeft = 144;		gMiscItemDescStatsRects[7].iTop = 229;	gMiscItemDescStatsRects[7].iRight = 230;	gMiscItemDescStatsRects[7].iBottom = 240;	// First Column
		gMiscItemDescStatsRects[8].iLeft = 144;		gMiscItemDescStatsRects[8].iTop = 241;	gMiscItemDescStatsRects[8].iRight = 230;	gMiscItemDescStatsRects[8].iBottom = 252;
		gMiscItemDescStatsRects[9].iLeft = 144;		gMiscItemDescStatsRects[9].iTop = 253;	gMiscItemDescStatsRects[9].iRight = 230;	gMiscItemDescStatsRects[9].iBottom = 264;
		gMiscItemDescStatsRects[10].iLeft = 144;	gMiscItemDescStatsRects[10].iTop = 265;	gMiscItemDescStatsRects[10].iRight = 230;	gMiscItemDescStatsRects[10].iBottom = 276;
		gMiscItemDescStatsRects[11].iLeft = 144;	gMiscItemDescStatsRects[11].iTop = 277;	gMiscItemDescStatsRects[11].iRight = 230;	gMiscItemDescStatsRects[11].iBottom = 288;
		gMiscItemDescStatsRects[12].iLeft = 144;	gMiscItemDescStatsRects[12].iTop = 289;	gMiscItemDescStatsRects[12].iRight = 230;	gMiscItemDescStatsRects[12].iBottom = 300;
		gMiscItemDescStatsRects[13].iLeft = 144;	gMiscItemDescStatsRects[13].iTop = 301;	gMiscItemDescStatsRects[13].iRight = 230;	gMiscItemDescStatsRects[13].iBottom = 312;
		gMiscItemDescStatsRects[14].iLeft = 0;		gMiscItemDescStatsRects[14].iTop = 0;	gMiscItemDescStatsRects[14].iRight = 1;		gMiscItemDescStatsRects[14].iBottom = 1;	// Placeholders
		gMiscItemDescStatsRects[15].iLeft = 0;		gMiscItemDescStatsRects[15].iTop = 0;	gMiscItemDescStatsRects[15].iRight = 1;		gMiscItemDescStatsRects[15].iBottom = 1;
		gMiscItemDescStatsRects[16].iLeft = 0;		gMiscItemDescStatsRects[16].iTop = 0;	gMiscItemDescStatsRects[16].iRight = 1;		gMiscItemDescStatsRects[16].iBottom = 1;
		gMiscItemDescStatsRects[17].iLeft = 0;		gMiscItemDescStatsRects[17].iTop = 0;	gMiscItemDescStatsRects[17].iRight = 1;		gMiscItemDescStatsRects[17].iBottom = 1;
		gMiscItemDescStatsRects[18].iLeft = 0;		gMiscItemDescStatsRects[18].iTop = 0;	gMiscItemDescStatsRects[18].iRight = 1;		gMiscItemDescStatsRects[18].iBottom = 1;
		gMiscItemDescStatsRects[19].iLeft = 0;		gMiscItemDescStatsRects[19].iTop = 0;	gMiscItemDescStatsRects[19].iRight = 1;		gMiscItemDescStatsRects[19].iBottom = 1;
	}
	else
	{
		if (UsingEDBSystem() == 1)
		{
			// Icon Locations
			gMiscItemStatsEDB[0].sX = 203;	gMiscItemStatsEDB[0].sY = 44;	gMiscItemStatsEDB[0].sValDx = 0;	// 0) COL 1; ROW 1
			gMiscItemStatsEDB[1].sX = 203;	gMiscItemStatsEDB[1].sY = 56;	gMiscItemStatsEDB[1].sValDx = 0;	// 1) COL 1; ROW 2
			gMiscItemStatsEDB[2].sX = 203;	gMiscItemStatsEDB[2].sY = 68;	gMiscItemStatsEDB[2].sValDx = 0;	// 2) COL 1; ROW 3
			gMiscItemStatsEDB[3].sX = 203;	gMiscItemStatsEDB[3].sY = 80;	gMiscItemStatsEDB[3].sValDx = 0;	// 3) COL 1; ROW 4
			gMiscItemStatsEDB[4].sX = 203;	gMiscItemStatsEDB[4].sY = 92;	gMiscItemStatsEDB[4].sValDx = 0;	// 4) COL 1; ROW 5
			gMiscItemStatsEDB[5].sX = 203;	gMiscItemStatsEDB[5].sY = 104;	gMiscItemStatsEDB[5].sValDx = 0;	// 5) COL 1; ROW 6
			gMiscItemStatsEDB[6].sX = 203;	gMiscItemStatsEDB[6].sY = 116;	gMiscItemStatsEDB[6].sValDx = 0;	// 6) COL 1; ROW 7
			gMiscItemStatsEDB[7].sX = 203;	gMiscItemStatsEDB[7].sY = 128;	gMiscItemStatsEDB[7].sValDx = 0;	// 7) COL 1; ROW 8
			gMiscItemStatsEDB[8].sX = 203;	gMiscItemStatsEDB[8].sY = 140;	gMiscItemStatsEDB[8].sValDx = 0;	// 8) COL 1; ROW 9
			gMiscItemStatsEDB[9].sX = 203;	gMiscItemStatsEDB[9].sY = 152;	gMiscItemStatsEDB[9].sValDx = 0;	// 9) COL 1; ROW 10
			gMiscItemStatsEDB[10].sX = 203;	gMiscItemStatsEDB[10].sY = 164;	gMiscItemStatsEDB[10].sValDx = 0;	// 10) COL 1; ROW 11
			gMiscItemStatsEDB[11].sX = 203;	gMiscItemStatsEDB[11].sY = 176;	gMiscItemStatsEDB[11].sValDx = 0;	// 11) COL 1; ROW 12
			// EQUALS signs Locations
			gMiscItemStatsEDB[12].sX = 283;	gMiscItemStatsEDB[12].sY = 44;	gMiscItemStatsEDB[12].sValDx = 12;	// 12) COL 1; ROW 1
			gMiscItemStatsEDB[13].sX = 283;	gMiscItemStatsEDB[13].sY = 56;	gMiscItemStatsEDB[13].sValDx = 12;	// 13) COL 1; ROW 2
			gMiscItemStatsEDB[14].sX = 283;	gMiscItemStatsEDB[14].sY = 68;	gMiscItemStatsEDB[14].sValDx = 12;	// 14) COL 1; ROW 3
			gMiscItemStatsEDB[15].sX = 283;	gMiscItemStatsEDB[15].sY = 80;	gMiscItemStatsEDB[15].sValDx = 12;	// 15) COL 1; ROW 4
			gMiscItemStatsEDB[16].sX = 283;	gMiscItemStatsEDB[16].sY = 92;	gMiscItemStatsEDB[16].sValDx = 12;	// 16) COL 1; ROW 5
			gMiscItemStatsEDB[17].sX = 283;	gMiscItemStatsEDB[17].sY = 104;	gMiscItemStatsEDB[17].sValDx = 12;	// 17) COL 1; ROW 6
			gMiscItemStatsEDB[18].sX = 283;	gMiscItemStatsEDB[18].sY = 116;	gMiscItemStatsEDB[18].sValDx = 12;	// 18) COL 1; ROW 7
			gMiscItemStatsEDB[19].sX = 283;	gMiscItemStatsEDB[19].sY = 128;	gMiscItemStatsEDB[19].sValDx = 12;	// 19) COL 1; ROW 8
			gMiscItemStatsEDB[20].sX = 283;	gMiscItemStatsEDB[20].sY = 140;	gMiscItemStatsEDB[20].sValDx = 12;	// 20) COL 1; ROW 9
			gMiscItemStatsEDB[21].sX = 283;	gMiscItemStatsEDB[21].sY = 152;	gMiscItemStatsEDB[21].sValDx = 12;	// 21) COL 1; ROW 10
			gMiscItemStatsEDB[22].sX = 283;	gMiscItemStatsEDB[22].sY = 164;	gMiscItemStatsEDB[22].sValDx = 12;	// 22) COL 1; ROW 11
			gMiscItemStatsEDB[23].sX = 283;	gMiscItemStatsEDB[23].sY = 176;	gMiscItemStatsEDB[23].sValDx = 12;	// 23) COL 1; ROW 12
			// MISC
			gMiscItemStatsEDB[24].sX = 0;	gMiscItemStatsEDB[24].sY = 0;	gMiscItemStatsEDB[24].sValDx = 0;
			gMiscItemStatsEDB[25].sX = 0;	gMiscItemStatsEDB[25].sY = 0;	gMiscItemStatsEDB[25].sValDx = 0;
			gMiscItemStatsEDB[26].sX = 0;	gMiscItemStatsEDB[26].sY = 0;	gMiscItemStatsEDB[26].sValDx = 0;
			gMiscItemStatsEDB[27].sX = 0;	gMiscItemStatsEDB[27].sY = 0;	gMiscItemStatsEDB[27].sValDx = 0;

			gWeaponDescStatsRects[0].iLeft = 202;	gWeaponDescStatsRects[0].iTop = 42;		gWeaponDescStatsRects[0].iRight = 260;	gWeaponDescStatsRects[0].iBottom = 53;	// First Column
			gWeaponDescStatsRects[1].iLeft = 202;	gWeaponDescStatsRects[1].iTop = 54;		gWeaponDescStatsRects[1].iRight = 260;	gWeaponDescStatsRects[1].iBottom = 65;
			gWeaponDescStatsRects[2].iLeft = 202;	gWeaponDescStatsRects[2].iTop = 66;		gWeaponDescStatsRects[2].iRight = 260;	gWeaponDescStatsRects[2].iBottom = 77;
			gWeaponDescStatsRects[3].iLeft = 202;	gWeaponDescStatsRects[3].iTop = 78;		gWeaponDescStatsRects[3].iRight = 260;	gWeaponDescStatsRects[3].iBottom = 89;
			gWeaponDescStatsRects[4].iLeft = 202;	gWeaponDescStatsRects[4].iTop = 90;		gWeaponDescStatsRects[4].iRight = 260;	gWeaponDescStatsRects[4].iBottom = 101;
			gWeaponDescStatsRects[5].iLeft = 202;	gWeaponDescStatsRects[5].iTop = 102;	gWeaponDescStatsRects[5].iRight = 260;	gWeaponDescStatsRects[5].iBottom = 113;
			gWeaponDescStatsRects[6].iLeft = 202;	gWeaponDescStatsRects[6].iTop = 114;	gWeaponDescStatsRects[6].iRight = 260;	gWeaponDescStatsRects[6].iBottom = 125;
			gWeaponDescStatsRects[7].iLeft = 267;	gWeaponDescStatsRects[7].iTop = 54;		gWeaponDescStatsRects[7].iRight = 325;	gWeaponDescStatsRects[7].iBottom = 65;	// Second Column
			gWeaponDescStatsRects[8].iLeft = 267;	gWeaponDescStatsRects[8].iTop = 66;		gWeaponDescStatsRects[8].iRight = 325;	gWeaponDescStatsRects[8].iBottom = 77;
			gWeaponDescStatsRects[9].iLeft = 267;	gWeaponDescStatsRects[9].iTop = 78;		gWeaponDescStatsRects[9].iRight = 325;	gWeaponDescStatsRects[9].iBottom = 89;
			gWeaponDescStatsRects[10].iLeft = 267;	gWeaponDescStatsRects[10].iTop = 90;	gWeaponDescStatsRects[10].iRight = 325;	gWeaponDescStatsRects[10].iBottom = 101;
			gWeaponDescStatsRects[11].iLeft = 267;	gWeaponDescStatsRects[11].iTop = 102;	gWeaponDescStatsRects[11].iRight = 325;	gWeaponDescStatsRects[11].iBottom = 113;
			gWeaponDescStatsRects[12].iLeft = 267;	gWeaponDescStatsRects[12].iTop = 114;	gWeaponDescStatsRects[12].iRight = 325;	gWeaponDescStatsRects[12].iBottom = 125;
			gWeaponDescStatsRects[13].iLeft = 267;	gWeaponDescStatsRects[13].iTop = 134;	gWeaponDescStatsRects[13].iRight = 325;	gWeaponDescStatsRects[13].iBottom = 145;
			gWeaponDescStatsRects[14].iLeft = 267;	gWeaponDescStatsRects[14].iTop = 146;	gWeaponDescStatsRects[14].iRight = 325;	gWeaponDescStatsRects[14].iBottom = 157;
			gWeaponDescStatsRects[15].iLeft = 267;	gWeaponDescStatsRects[15].iTop = 158;	gWeaponDescStatsRects[15].iRight = 325;	gWeaponDescStatsRects[15].iBottom = 169;
			gWeaponDescStatsRects[16].iLeft = 267;	gWeaponDescStatsRects[16].iTop = 170;	gWeaponDescStatsRects[16].iRight = 325;	gWeaponDescStatsRects[16].iBottom = 181;
			gWeaponDescStatsRects[17].iLeft = 225;	gWeaponDescStatsRects[17].iTop = 134;	gWeaponDescStatsRects[17].iRight = 241;	gWeaponDescStatsRects[17].iBottom = 157;
			gWeaponDescStatsRects[18].iLeft = 202;	gWeaponDescStatsRects[18].iTop = 158;	gWeaponDescStatsRects[18].iRight = 260;	gWeaponDescStatsRects[18].iBottom = 169;
			gWeaponDescStatsRects[19].iLeft = 202;	gWeaponDescStatsRects[19].iTop = 170;	gWeaponDescStatsRects[19].iRight = 260;	gWeaponDescStatsRects[19].iBottom = 181;

			gAmmoDescStatsRects[0].iLeft = 202;		gAmmoDescStatsRects[0].iTop = 42;	gAmmoDescStatsRects[0].iRight = 260;	gAmmoDescStatsRects[0].iBottom = 53;	// First Column
			gAmmoDescStatsRects[1].iLeft = 202;		gAmmoDescStatsRects[1].iTop = 54;	gAmmoDescStatsRects[1].iRight = 260;	gAmmoDescStatsRects[1].iBottom = 65;
			gAmmoDescStatsRects[2].iLeft = 202;		gAmmoDescStatsRects[2].iTop = 66;	gAmmoDescStatsRects[2].iRight = 260;	gAmmoDescStatsRects[2].iBottom = 77;
			gAmmoDescStatsRects[3].iLeft = 269;		gAmmoDescStatsRects[3].iTop = 42;	gAmmoDescStatsRects[3].iRight = 291;	gAmmoDescStatsRects[3].iBottom = 65;	// Second Column
			gAmmoDescStatsRects[4].iLeft = 296;		gAmmoDescStatsRects[4].iTop = 42;	gAmmoDescStatsRects[4].iRight = 321;	gAmmoDescStatsRects[4].iBottom = 65;
			gAmmoDescStatsRects[5].iLeft = 269;		gAmmoDescStatsRects[5].iTop = 66;	gAmmoDescStatsRects[5].iRight = 291;	gAmmoDescStatsRects[5].iBottom = 89;
			gAmmoDescStatsRects[6].iLeft = 296;		gAmmoDescStatsRects[6].iTop = 66;	gAmmoDescStatsRects[6].iRight = 321;	gAmmoDescStatsRects[6].iBottom = 89;
			gAmmoDescStatsRects[7].iLeft = 269;		gAmmoDescStatsRects[7].iTop = 90;	gAmmoDescStatsRects[7].iRight = 291;	gAmmoDescStatsRects[7].iBottom = 113;
			gAmmoDescStatsRects[8].iLeft = 202;		gAmmoDescStatsRects[8].iTop = 134;	gAmmoDescStatsRects[8].iRight = 260;	gAmmoDescStatsRects[8].iBottom = 145;	// Third Column
			gAmmoDescStatsRects[9].iLeft = 202;		gAmmoDescStatsRects[9].iTop = 146;	gAmmoDescStatsRects[9].iRight = 260;	gAmmoDescStatsRects[9].iBottom = 157;
			gAmmoDescStatsRects[10].iLeft = 202;	gAmmoDescStatsRects[10].iTop = 158;	gAmmoDescStatsRects[10].iRight = 260;	gAmmoDescStatsRects[10].iBottom = 169;
			gAmmoDescStatsRects[11].iLeft = 202;	gAmmoDescStatsRects[11].iTop = 170;	gAmmoDescStatsRects[11].iRight = 260;	gAmmoDescStatsRects[11].iBottom = 181;
			gAmmoDescStatsRects[12].iLeft = 267;	gAmmoDescStatsRects[12].iTop = 134;	gAmmoDescStatsRects[12].iRight = 325;	gAmmoDescStatsRects[12].iBottom = 145;	// Fourth Column
			gAmmoDescStatsRects[13].iLeft = 267;	gAmmoDescStatsRects[13].iTop = 146;	gAmmoDescStatsRects[13].iRight = 325;	gAmmoDescStatsRects[13].iBottom = 1;
			gAmmoDescStatsRects[14].iLeft = 0;		gAmmoDescStatsRects[14].iTop = 0;	gAmmoDescStatsRects[14].iRight = 1;		gAmmoDescStatsRects[14].iBottom = 1;	// Placeholders
			gAmmoDescStatsRects[15].iLeft = 0;		gAmmoDescStatsRects[15].iTop = 0;	gAmmoDescStatsRects[15].iRight = 1;		gAmmoDescStatsRects[15].iBottom = 1;
			gAmmoDescStatsRects[16].iLeft = 0;		gAmmoDescStatsRects[16].iTop = 0;	gAmmoDescStatsRects[16].iRight = 1;		gAmmoDescStatsRects[16].iBottom = 1;
			gAmmoDescStatsRects[17].iLeft = 0;		gAmmoDescStatsRects[17].iTop = 0;	gAmmoDescStatsRects[17].iRight = 1;		gAmmoDescStatsRects[17].iBottom = 1;
			gAmmoDescStatsRects[18].iLeft = 0;		gAmmoDescStatsRects[18].iTop = 0;	gAmmoDescStatsRects[18].iRight = 1;		gAmmoDescStatsRects[18].iBottom = 1;
			gAmmoDescStatsRects[19].iLeft = 0;		gAmmoDescStatsRects[19].iTop = 0;	gAmmoDescStatsRects[19].iRight = 1;		gAmmoDescStatsRects[19].iBottom = 1;

			gArmorDescStatsRects[0].iLeft = 202;	gArmorDescStatsRects[0].iTop = 42;		gArmorDescStatsRects[0].iRight = 260;	gArmorDescStatsRects[0].iBottom = 53;	// First Column
			gArmorDescStatsRects[1].iLeft = 202;	gArmorDescStatsRects[1].iTop = 54;		gArmorDescStatsRects[1].iRight = 260;	gArmorDescStatsRects[1].iBottom = 65;
			gArmorDescStatsRects[2].iLeft = 202;	gArmorDescStatsRects[2].iTop = 66;		gArmorDescStatsRects[2].iRight = 260;	gArmorDescStatsRects[2].iBottom = 77;
			gArmorDescStatsRects[3].iLeft = 267;	gArmorDescStatsRects[3].iTop = 42;		gArmorDescStatsRects[3].iRight = 325;	gArmorDescStatsRects[3].iBottom = 53;	// Second Column
			gArmorDescStatsRects[4].iLeft = 267;	gArmorDescStatsRects[4].iTop = 54;		gArmorDescStatsRects[4].iRight = 325;	gArmorDescStatsRects[4].iBottom = 65;
			gArmorDescStatsRects[5].iLeft = 267;	gArmorDescStatsRects[5].iTop = 66;		gArmorDescStatsRects[5].iRight = 325;	gArmorDescStatsRects[5].iBottom = 77;
			gArmorDescStatsRects[6].iLeft = 267;	gArmorDescStatsRects[6].iTop = 78;		gArmorDescStatsRects[6].iRight = 325;	gArmorDescStatsRects[6].iBottom = 89;
			gArmorDescStatsRects[7].iLeft = 267;	gArmorDescStatsRects[7].iTop = 90;		gArmorDescStatsRects[7].iRight = 325;	gArmorDescStatsRects[7].iBottom = 101;
			gArmorDescStatsRects[8].iLeft = 267;	gArmorDescStatsRects[8].iTop = 102;		gArmorDescStatsRects[8].iRight = 325;	gArmorDescStatsRects[8].iBottom = 113;
			gArmorDescStatsRects[9].iLeft = 267;	gArmorDescStatsRects[9].iTop = 114;		gArmorDescStatsRects[9].iRight = 325;	gArmorDescStatsRects[9].iBottom = 125;
			gArmorDescStatsRects[10].iLeft = 202;	gArmorDescStatsRects[10].iTop = 134;	gArmorDescStatsRects[10].iRight = 260;	gArmorDescStatsRects[10].iBottom = 145;	// Third Column
			gArmorDescStatsRects[11].iLeft = 202;	gArmorDescStatsRects[11].iTop = 146;	gArmorDescStatsRects[11].iRight = 260;	gArmorDescStatsRects[11].iBottom = 157;
			gArmorDescStatsRects[12].iLeft = 202;	gArmorDescStatsRects[12].iTop = 158;	gArmorDescStatsRects[12].iRight = 260;	gArmorDescStatsRects[12].iBottom = 169;
			gArmorDescStatsRects[13].iLeft = 202;	gArmorDescStatsRects[13].iTop = 170;	gArmorDescStatsRects[13].iRight = 260;	gArmorDescStatsRects[13].iBottom = 181;
			gArmorDescStatsRects[14].iLeft = 267;	gArmorDescStatsRects[14].iTop = 134;	gArmorDescStatsRects[14].iRight = 325;	gArmorDescStatsRects[14].iBottom = 145;	// Fourth Column
			gArmorDescStatsRects[15].iLeft = 267;	gArmorDescStatsRects[15].iTop = 146;	gArmorDescStatsRects[15].iRight = 325;	gArmorDescStatsRects[15].iBottom = 157;
			gArmorDescStatsRects[16].iLeft = 267;	gArmorDescStatsRects[16].iTop = 158;	gArmorDescStatsRects[16].iRight = 325;	gArmorDescStatsRects[16].iBottom = 169;
			gArmorDescStatsRects[17].iLeft = 0;		gArmorDescStatsRects[17].iTop = 0;		gArmorDescStatsRects[17].iRight = 1;	gArmorDescStatsRects[17].iBottom = 1;	// Placeholders
			gArmorDescStatsRects[18].iLeft = 0;		gArmorDescStatsRects[18].iTop = 0;		gArmorDescStatsRects[18].iRight = 1;	gArmorDescStatsRects[18].iBottom = 1;
			gArmorDescStatsRects[19].iLeft = 0;		gArmorDescStatsRects[19].iTop = 0;		gArmorDescStatsRects[19].iRight = 1;	gArmorDescStatsRects[19].iBottom = 1;

			gExplosiveDescStatsRects[0].iLeft = 202;	gExplosiveDescStatsRects[0].iTop = 42;	gExplosiveDescStatsRects[0].iRight = 260;	gExplosiveDescStatsRects[0].iBottom = 53;	// First Column
			gExplosiveDescStatsRects[1].iLeft = 202;	gExplosiveDescStatsRects[1].iTop = 54;	gExplosiveDescStatsRects[1].iRight = 260;	gExplosiveDescStatsRects[1].iBottom = 65;
			gExplosiveDescStatsRects[2].iLeft = 202;	gExplosiveDescStatsRects[2].iTop = 66;	gExplosiveDescStatsRects[2].iRight = 260;	gExplosiveDescStatsRects[2].iBottom = 77;
			gExplosiveDescStatsRects[3].iLeft = 202;	gExplosiveDescStatsRects[3].iTop = 78;	gExplosiveDescStatsRects[3].iRight = 260;	gExplosiveDescStatsRects[3].iBottom = 89;
			gExplosiveDescStatsRects[4].iLeft = 290;	gExplosiveDescStatsRects[4].iTop = 42;	gExplosiveDescStatsRects[4].iRight = 306;	gExplosiveDescStatsRects[4].iBottom = 77;	// Second Column
			gExplosiveDescStatsRects[5].iLeft = 269;	gExplosiveDescStatsRects[5].iTop = 42;	gExplosiveDescStatsRects[5].iRight = 285;	gExplosiveDescStatsRects[5].iBottom = 77;
			gExplosiveDescStatsRects[6].iLeft = 301;	gExplosiveDescStatsRects[6].iTop = 42;	gExplosiveDescStatsRects[6].iRight = 321;	gExplosiveDescStatsRects[6].iBottom = 77;
			gExplosiveDescStatsRects[7].iLeft = 267;	gExplosiveDescStatsRects[7].iTop = 90;	gExplosiveDescStatsRects[7].iRight = 325;	gExplosiveDescStatsRects[7].iBottom = 101;
			gExplosiveDescStatsRects[8].iLeft = 0;		gExplosiveDescStatsRects[8].iTop = 0;	gExplosiveDescStatsRects[8].iRight = 1;		gExplosiveDescStatsRects[8].iBottom = 1;	// Placeholders
			gExplosiveDescStatsRects[9].iLeft = 0;		gExplosiveDescStatsRects[9].iTop = 0;	gExplosiveDescStatsRects[9].iRight = 1;		gExplosiveDescStatsRects[9].iBottom = 1;
			gExplosiveDescStatsRects[10].iLeft = 0;		gExplosiveDescStatsRects[10].iTop = 0;	gExplosiveDescStatsRects[10].iRight = 1;	gExplosiveDescStatsRects[10].iBottom = 1;
			gExplosiveDescStatsRects[11].iLeft = 0;		gExplosiveDescStatsRects[11].iTop = 0;	gExplosiveDescStatsRects[11].iRight = 1;	gExplosiveDescStatsRects[11].iBottom = 1;
			gExplosiveDescStatsRects[12].iLeft = 0;		gExplosiveDescStatsRects[12].iTop = 0;	gExplosiveDescStatsRects[12].iRight = 1;	gExplosiveDescStatsRects[12].iBottom = 1;
			gExplosiveDescStatsRects[13].iLeft = 0;		gExplosiveDescStatsRects[13].iTop = 0;	gExplosiveDescStatsRects[13].iRight = 1;	gExplosiveDescStatsRects[13].iBottom = 1;
			gExplosiveDescStatsRects[14].iLeft = 0;		gExplosiveDescStatsRects[14].iTop = 0;	gExplosiveDescStatsRects[14].iRight = 1;	gExplosiveDescStatsRects[14].iBottom = 1;
			gExplosiveDescStatsRects[15].iLeft = 0;		gExplosiveDescStatsRects[15].iTop = 0;	gExplosiveDescStatsRects[15].iRight = 1;	gExplosiveDescStatsRects[15].iBottom = 1;
			gExplosiveDescStatsRects[16].iLeft = 0;		gExplosiveDescStatsRects[16].iTop = 0;	gExplosiveDescStatsRects[16].iRight = 1;	gExplosiveDescStatsRects[16].iBottom = 1;
			gExplosiveDescStatsRects[17].iLeft = 0;		gExplosiveDescStatsRects[17].iTop = 0;	gExplosiveDescStatsRects[17].iRight = 1;	gExplosiveDescStatsRects[17].iBottom = 1;
			gExplosiveDescStatsRects[18].iLeft = 0;		gExplosiveDescStatsRects[18].iTop = 0;	gExplosiveDescStatsRects[18].iRight = 1;	gExplosiveDescStatsRects[18].iBottom = 1;
			gExplosiveDescStatsRects[19].iLeft = 0;		gExplosiveDescStatsRects[19].iTop = 0;	gExplosiveDescStatsRects[19].iRight = 1;	gExplosiveDescStatsRects[19].iBottom = 1;

			gMiscItemDescStatsRects[0].iLeft = 202;		gMiscItemDescStatsRects[0].iTop = 42;	gMiscItemDescStatsRects[0].iRight = 325;	gMiscItemDescStatsRects[0].iBottom = 53; // Only one column for misc items in tactical - 12 fields.
			gMiscItemDescStatsRects[1].iLeft = 202;		gMiscItemDescStatsRects[1].iTop = 54;	gMiscItemDescStatsRects[1].iRight = 325;	gMiscItemDescStatsRects[1].iBottom = 65;
			gMiscItemDescStatsRects[2].iLeft = 202;		gMiscItemDescStatsRects[2].iTop = 66;	gMiscItemDescStatsRects[2].iRight = 325;	gMiscItemDescStatsRects[2].iBottom = 77;
			gMiscItemDescStatsRects[3].iLeft = 202;		gMiscItemDescStatsRects[3].iTop = 78;	gMiscItemDescStatsRects[3].iRight = 325;	gMiscItemDescStatsRects[3].iBottom = 89;
			gMiscItemDescStatsRects[4].iLeft = 202;		gMiscItemDescStatsRects[4].iTop = 90;	gMiscItemDescStatsRects[4].iRight = 325;	gMiscItemDescStatsRects[4].iBottom = 101;
			gMiscItemDescStatsRects[5].iLeft = 202;		gMiscItemDescStatsRects[5].iTop = 102;	gMiscItemDescStatsRects[5].iRight = 325;	gMiscItemDescStatsRects[5].iBottom = 113;
			gMiscItemDescStatsRects[6].iLeft = 202;		gMiscItemDescStatsRects[6].iTop = 114;	gMiscItemDescStatsRects[6].iRight = 325;	gMiscItemDescStatsRects[6].iBottom = 125;
			gMiscItemDescStatsRects[7].iLeft = 202;		gMiscItemDescStatsRects[7].iTop = 126;	gMiscItemDescStatsRects[7].iRight = 325;	gMiscItemDescStatsRects[7].iBottom = 137;
			gMiscItemDescStatsRects[8].iLeft = 202;		gMiscItemDescStatsRects[8].iTop = 138;	gMiscItemDescStatsRects[8].iRight = 325;	gMiscItemDescStatsRects[8].iBottom = 149;
			gMiscItemDescStatsRects[9].iLeft = 202;		gMiscItemDescStatsRects[9].iTop = 150;	gMiscItemDescStatsRects[9].iRight = 325;	gMiscItemDescStatsRects[9].iBottom = 161;
			gMiscItemDescStatsRects[10].iLeft = 202;	gMiscItemDescStatsRects[10].iTop = 162;	gMiscItemDescStatsRects[10].iRight = 325;	gMiscItemDescStatsRects[10].iBottom = 173;
			gMiscItemDescStatsRects[11].iLeft = 202;	gMiscItemDescStatsRects[11].iTop = 174;	gMiscItemDescStatsRects[11].iRight = 325;	gMiscItemDescStatsRects[11].iBottom = 185;
			gMiscItemDescStatsRects[12].iLeft = 0;		gMiscItemDescStatsRects[12].iTop = 0;	gMiscItemDescStatsRects[12].iRight = 1;		gMiscItemDescStatsRects[12].iBottom = 1;	// Placeholders
			gMiscItemDescStatsRects[13].iLeft = 0;		gMiscItemDescStatsRects[13].iTop = 0;	gMiscItemDescStatsRects[13].iRight = 1;		gMiscItemDescStatsRects[13].iBottom = 1;
			gMiscItemDescStatsRects[14].iLeft = 0;		gMiscItemDescStatsRects[14].iTop = 0;	gMiscItemDescStatsRects[14].iRight = 1;		gMiscItemDescStatsRects[14].iBottom = 1;
			gMiscItemDescStatsRects[15].iLeft = 0;		gMiscItemDescStatsRects[15].iTop = 0;	gMiscItemDescStatsRects[15].iRight = 1;		gMiscItemDescStatsRects[15].iBottom = 1;
			gMiscItemDescStatsRects[16].iLeft = 0;		gMiscItemDescStatsRects[16].iTop = 0;	gMiscItemDescStatsRects[16].iRight = 1;		gMiscItemDescStatsRects[16].iBottom = 1;
			gMiscItemDescStatsRects[17].iLeft = 0;		gMiscItemDescStatsRects[17].iTop = 0;	gMiscItemDescStatsRects[17].iRight = 1;		gMiscItemDescStatsRects[17].iBottom = 1;
			gMiscItemDescStatsRects[18].iLeft = 0;		gMiscItemDescStatsRects[18].iTop = 0;	gMiscItemDescStatsRects[18].iRight = 1;		gMiscItemDescStatsRects[18].iBottom = 1;
			gMiscItemDescStatsRects[19].iLeft = 0;		gMiscItemDescStatsRects[19].iTop = 0;	gMiscItemDescStatsRects[19].iRight = 1;		gMiscItemDescStatsRects[19].iBottom = 1;
		}
		else if (UsingEDBSystem() == 2) // Tactical OIV-EDB
		{
			// Icon Locations
			gMiscItemStatsEDB[0].sX = 274;	gMiscItemStatsEDB[0].sY = 14;	gMiscItemStatsEDB[0].sValDx = 0;	// 0) COL 1; ROW 1
			gMiscItemStatsEDB[1].sX = 274;	gMiscItemStatsEDB[1].sY = 26;	gMiscItemStatsEDB[1].sValDx = 0;	// 1) COL 1; ROW 2
			gMiscItemStatsEDB[2].sX = 274;	gMiscItemStatsEDB[2].sY = 38;	gMiscItemStatsEDB[2].sValDx = 0;	// 2) COL 1; ROW 3
			gMiscItemStatsEDB[3].sX = 274;	gMiscItemStatsEDB[3].sY = 50;	gMiscItemStatsEDB[3].sValDx = 0;	// 3) COL 1; ROW 4
			gMiscItemStatsEDB[4].sX = 274;	gMiscItemStatsEDB[4].sY = 62;	gMiscItemStatsEDB[4].sValDx = 0;	// 4) COL 1; ROW 5
			gMiscItemStatsEDB[5].sX = 274;	gMiscItemStatsEDB[5].sY = 74;	gMiscItemStatsEDB[5].sValDx = 0;	// 5) COL 1; ROW 6
			gMiscItemStatsEDB[6].sX = 274;	gMiscItemStatsEDB[6].sY = 86;	gMiscItemStatsEDB[6].sValDx = 0;	// 6) COL 1; ROW 7
			gMiscItemStatsEDB[7].sX = 375;	gMiscItemStatsEDB[7].sY = 14;	gMiscItemStatsEDB[7].sValDx = 0;	// 7) COL 1; ROW 8
			gMiscItemStatsEDB[8].sX = 375;	gMiscItemStatsEDB[8].sY = 26;	gMiscItemStatsEDB[8].sValDx = 0;	// 8) COL 1; ROW 9
			gMiscItemStatsEDB[9].sX = 375;	gMiscItemStatsEDB[9].sY = 38;	gMiscItemStatsEDB[9].sValDx = 0;	// 9) COL 1; ROW 10
			gMiscItemStatsEDB[10].sX = 375;	gMiscItemStatsEDB[10].sY = 50;	gMiscItemStatsEDB[10].sValDx = 0;	// 10) COL 1; ROW 11
			gMiscItemStatsEDB[11].sX = 375;	gMiscItemStatsEDB[11].sY = 62;	gMiscItemStatsEDB[11].sValDx = 0;	// 11) COL 1; ROW 12
			// EQUALS signs Locations
			gMiscItemStatsEDB[12].sX = 324;	gMiscItemStatsEDB[12].sY = 14;	gMiscItemStatsEDB[12].sValDx = 12;	// 12) COL 1; ROW 1
			gMiscItemStatsEDB[13].sX = 324;	gMiscItemStatsEDB[13].sY = 26;	gMiscItemStatsEDB[13].sValDx = 12;	// 13) COL 1; ROW 2
			gMiscItemStatsEDB[14].sX = 324;	gMiscItemStatsEDB[14].sY = 38;	gMiscItemStatsEDB[14].sValDx = 12;	// 14) COL 1; ROW 3
			gMiscItemStatsEDB[15].sX = 324;	gMiscItemStatsEDB[15].sY = 50;	gMiscItemStatsEDB[15].sValDx = 12;	// 15) COL 1; ROW 4
			gMiscItemStatsEDB[16].sX = 324;	gMiscItemStatsEDB[16].sY = 62;	gMiscItemStatsEDB[16].sValDx = 12;	// 16) COL 1; ROW 5
			gMiscItemStatsEDB[17].sX = 324;	gMiscItemStatsEDB[17].sY = 74;	gMiscItemStatsEDB[17].sValDx = 12;	// 17) COL 1; ROW 6
			gMiscItemStatsEDB[18].sX = 324;	gMiscItemStatsEDB[18].sY = 86;	gMiscItemStatsEDB[18].sValDx = 12;	// 18) COL 1; ROW 7
			gMiscItemStatsEDB[19].sX = 425;	gMiscItemStatsEDB[19].sY = 14;	gMiscItemStatsEDB[19].sValDx = 12;	// 19) COL 1; ROW 8
			gMiscItemStatsEDB[20].sX = 425;	gMiscItemStatsEDB[20].sY = 26;	gMiscItemStatsEDB[20].sValDx = 12;	// 20) COL 1; ROW 9
			gMiscItemStatsEDB[21].sX = 425;	gMiscItemStatsEDB[21].sY = 38;	gMiscItemStatsEDB[21].sValDx = 12;	// 21) COL 1; ROW 10
			gMiscItemStatsEDB[22].sX = 425;	gMiscItemStatsEDB[22].sY = 50;	gMiscItemStatsEDB[22].sValDx = 12;	// 22) COL 1; ROW 11
			gMiscItemStatsEDB[23].sX = 425;	gMiscItemStatsEDB[23].sY = 62;	gMiscItemStatsEDB[23].sValDx = 12;	// 23) COL 1; ROW 12
			// MISC
			gMiscItemStatsEDB[24].sX = 0;	gMiscItemStatsEDB[24].sY = 0;	gMiscItemStatsEDB[24].sValDx = 0;
			gMiscItemStatsEDB[25].sX = 0;	gMiscItemStatsEDB[25].sY = 0;	gMiscItemStatsEDB[25].sValDx = 0;
			gMiscItemStatsEDB[26].sX = 0;	gMiscItemStatsEDB[26].sY = 0;	gMiscItemStatsEDB[26].sValDx = 0;
			gMiscItemStatsEDB[27].sX = 0;	gMiscItemStatsEDB[27].sY = 0;	gMiscItemStatsEDB[27].sValDx = 0;

			gWeaponDescStatsRects[0].iLeft = 273;	gWeaponDescStatsRects[0].iTop = 12;		gWeaponDescStatsRects[0].iRight = 331;	gWeaponDescStatsRects[0].iBottom = 23;	// First Column
			gWeaponDescStatsRects[1].iLeft = 273;	gWeaponDescStatsRects[1].iTop = 24;		gWeaponDescStatsRects[1].iRight = 331;	gWeaponDescStatsRects[1].iBottom = 35;
			gWeaponDescStatsRects[2].iLeft = 273;	gWeaponDescStatsRects[2].iTop = 36;		gWeaponDescStatsRects[2].iRight = 331;	gWeaponDescStatsRects[2].iBottom = 47;
			gWeaponDescStatsRects[3].iLeft = 273;	gWeaponDescStatsRects[3].iTop = 48;		gWeaponDescStatsRects[3].iRight = 331;	gWeaponDescStatsRects[3].iBottom = 59;
			gWeaponDescStatsRects[4].iLeft = 273;	gWeaponDescStatsRects[4].iTop = 60;		gWeaponDescStatsRects[4].iRight = 331;	gWeaponDescStatsRects[4].iBottom = 71;
			gWeaponDescStatsRects[5].iLeft = 273;	gWeaponDescStatsRects[5].iTop = 72;		gWeaponDescStatsRects[5].iRight = 331;	gWeaponDescStatsRects[5].iBottom = 83;
			gWeaponDescStatsRects[6].iLeft = 273;	gWeaponDescStatsRects[6].iTop = 84;		gWeaponDescStatsRects[6].iRight = 331;	gWeaponDescStatsRects[6].iBottom = 95;
			gWeaponDescStatsRects[7].iLeft = 340;	gWeaponDescStatsRects[7].iTop = 24;		gWeaponDescStatsRects[7].iRight = 398;	gWeaponDescStatsRects[7].iBottom = 35;	// Second Column
			gWeaponDescStatsRects[8].iLeft = 340;	gWeaponDescStatsRects[8].iTop = 36;		gWeaponDescStatsRects[8].iRight = 398;	gWeaponDescStatsRects[8].iBottom = 47;
			gWeaponDescStatsRects[9].iLeft = 340;	gWeaponDescStatsRects[9].iTop = 48;		gWeaponDescStatsRects[9].iRight = 398;	gWeaponDescStatsRects[9].iBottom = 59;
			gWeaponDescStatsRects[10].iLeft = 340;	gWeaponDescStatsRects[10].iTop = 60;	gWeaponDescStatsRects[10].iRight = 398;	gWeaponDescStatsRects[10].iBottom = 71;
			gWeaponDescStatsRects[11].iLeft = 340;	gWeaponDescStatsRects[11].iTop = 72;	gWeaponDescStatsRects[11].iRight = 398;	gWeaponDescStatsRects[11].iBottom = 83;
			gWeaponDescStatsRects[12].iLeft = 340;	gWeaponDescStatsRects[12].iTop = 84;	gWeaponDescStatsRects[12].iRight = 398;	gWeaponDescStatsRects[12].iBottom = 95;
			gWeaponDescStatsRects[13].iLeft = 407;	gWeaponDescStatsRects[13].iTop = 36;	gWeaponDescStatsRects[13].iRight = 465;	gWeaponDescStatsRects[13].iBottom = 47;	// These are non-sequential!
			gWeaponDescStatsRects[14].iLeft = 407;	gWeaponDescStatsRects[14].iTop = 48;	gWeaponDescStatsRects[14].iRight = 465;	gWeaponDescStatsRects[14].iBottom = 59;
			gWeaponDescStatsRects[15].iLeft = 407;	gWeaponDescStatsRects[15].iTop = 60;	gWeaponDescStatsRects[15].iRight = 465;	gWeaponDescStatsRects[15].iBottom = 71;
			gWeaponDescStatsRects[16].iLeft = 407;	gWeaponDescStatsRects[16].iTop = 72;	gWeaponDescStatsRects[16].iRight = 465;	gWeaponDescStatsRects[16].iBottom = 83;
			gWeaponDescStatsRects[17].iLeft = 407;	gWeaponDescStatsRects[17].iTop = 12;	gWeaponDescStatsRects[17].iRight = 465;	gWeaponDescStatsRects[17].iBottom = 23;	
			gWeaponDescStatsRects[18].iLeft = 407;	gWeaponDescStatsRects[18].iTop = 24;	gWeaponDescStatsRects[18].iRight = 465;	gWeaponDescStatsRects[18].iBottom = 35;
			gWeaponDescStatsRects[19].iLeft = 407;	gWeaponDescStatsRects[19].iTop = 84;	gWeaponDescStatsRects[19].iRight = 465;	gWeaponDescStatsRects[19].iBottom = 95;

			gAmmoDescStatsRects[0].iLeft = 273;		gAmmoDescStatsRects[0].iTop = 12;	gAmmoDescStatsRects[0].iRight = 331;	gAmmoDescStatsRects[0].iBottom = 23;	// First Column
			gAmmoDescStatsRects[1].iLeft = 273;		gAmmoDescStatsRects[1].iTop = 24;	gAmmoDescStatsRects[1].iRight = 331;	gAmmoDescStatsRects[1].iBottom = 35;
			gAmmoDescStatsRects[2].iLeft = 273;		gAmmoDescStatsRects[2].iTop = 36;	gAmmoDescStatsRects[2].iRight = 331;	gAmmoDescStatsRects[2].iBottom = 47;
			gAmmoDescStatsRects[3].iLeft = 342;		gAmmoDescStatsRects[3].iTop = 12;	gAmmoDescStatsRects[3].iRight = 364;	gAmmoDescStatsRects[3].iBottom = 35;	// Second Column
			gAmmoDescStatsRects[4].iLeft = 369;		gAmmoDescStatsRects[4].iTop = 12;	gAmmoDescStatsRects[4].iRight = 391;	gAmmoDescStatsRects[4].iBottom = 35;
			gAmmoDescStatsRects[5].iLeft = 342;		gAmmoDescStatsRects[5].iTop = 36;	gAmmoDescStatsRects[5].iRight = 364;	gAmmoDescStatsRects[5].iBottom = 59;
			gAmmoDescStatsRects[6].iLeft = 369;		gAmmoDescStatsRects[6].iTop = 36;	gAmmoDescStatsRects[6].iRight = 391;	gAmmoDescStatsRects[6].iBottom = 59;
			gAmmoDescStatsRects[7].iLeft = 342;		gAmmoDescStatsRects[7].iTop = 60;	gAmmoDescStatsRects[7].iRight = 364;	gAmmoDescStatsRects[7].iBottom = 83;
			gAmmoDescStatsRects[8].iLeft = 407;		gAmmoDescStatsRects[8].iTop = 12;	gAmmoDescStatsRects[8].iRight = 465;	gAmmoDescStatsRects[8].iBottom = 23;	// Third Column
			gAmmoDescStatsRects[9].iLeft = 407;		gAmmoDescStatsRects[9].iTop = 24;	gAmmoDescStatsRects[9].iRight = 465;	gAmmoDescStatsRects[9].iBottom = 35;
			gAmmoDescStatsRects[10].iLeft = 407;	gAmmoDescStatsRects[10].iTop = 36;	gAmmoDescStatsRects[10].iRight = 465;	gAmmoDescStatsRects[10].iBottom = 47;
			gAmmoDescStatsRects[11].iLeft = 407;	gAmmoDescStatsRects[11].iTop = 48;	gAmmoDescStatsRects[11].iRight = 465;	gAmmoDescStatsRects[11].iBottom = 59;
			gAmmoDescStatsRects[12].iLeft = 407;	gAmmoDescStatsRects[12].iTop = 60;	gAmmoDescStatsRects[12].iRight = 465;	gAmmoDescStatsRects[12].iBottom = 71;
			gAmmoDescStatsRects[13].iLeft = 407;	gAmmoDescStatsRects[13].iTop = 72;	gAmmoDescStatsRects[13].iRight = 465;	gAmmoDescStatsRects[13].iBottom = 1;
			gAmmoDescStatsRects[14].iLeft = 0;		gAmmoDescStatsRects[14].iTop = 0;	gAmmoDescStatsRects[14].iRight = 1;		gAmmoDescStatsRects[14].iBottom = 1;	// Placeholders
			gAmmoDescStatsRects[15].iLeft = 0;		gAmmoDescStatsRects[15].iTop = 0;	gAmmoDescStatsRects[15].iRight = 1;		gAmmoDescStatsRects[15].iBottom = 1;
			gAmmoDescStatsRects[16].iLeft = 0;		gAmmoDescStatsRects[16].iTop = 0;	gAmmoDescStatsRects[16].iRight = 1;		gAmmoDescStatsRects[16].iBottom = 1;
			gAmmoDescStatsRects[17].iLeft = 0;		gAmmoDescStatsRects[17].iTop = 0;	gAmmoDescStatsRects[17].iRight = 1;		gAmmoDescStatsRects[17].iBottom = 1;
			gAmmoDescStatsRects[18].iLeft = 0;		gAmmoDescStatsRects[18].iTop = 0;	gAmmoDescStatsRects[18].iRight = 1;		gAmmoDescStatsRects[18].iBottom = 1;
			gAmmoDescStatsRects[19].iLeft = 0;		gAmmoDescStatsRects[19].iTop = 0;	gAmmoDescStatsRects[19].iRight = 1;		gAmmoDescStatsRects[19].iBottom = 1;

			gArmorDescStatsRects[0].iLeft = 273;	gArmorDescStatsRects[0].iTop = 12;		gArmorDescStatsRects[0].iRight = 331;	gArmorDescStatsRects[0].iBottom = 23;	// First Column
			gArmorDescStatsRects[1].iLeft = 273;	gArmorDescStatsRects[1].iTop = 24;		gArmorDescStatsRects[1].iRight = 331;	gArmorDescStatsRects[1].iBottom = 35;
			gArmorDescStatsRects[2].iLeft = 273;	gArmorDescStatsRects[2].iTop = 36;		gArmorDescStatsRects[2].iRight = 331;	gArmorDescStatsRects[2].iBottom = 47;
			gArmorDescStatsRects[3].iLeft = 340;	gArmorDescStatsRects[3].iTop = 12;		gArmorDescStatsRects[3].iRight = 398;	gArmorDescStatsRects[3].iBottom = 23;	// Second Column
			gArmorDescStatsRects[4].iLeft = 340;	gArmorDescStatsRects[4].iTop = 24;		gArmorDescStatsRects[4].iRight = 398;	gArmorDescStatsRects[4].iBottom = 35;
			gArmorDescStatsRects[5].iLeft = 340;	gArmorDescStatsRects[5].iTop = 36;		gArmorDescStatsRects[5].iRight = 398;	gArmorDescStatsRects[5].iBottom = 47;
			gArmorDescStatsRects[6].iLeft = 340;	gArmorDescStatsRects[6].iTop = 48;		gArmorDescStatsRects[6].iRight = 398;	gArmorDescStatsRects[6].iBottom = 59;
			gArmorDescStatsRects[7].iLeft = 340;	gArmorDescStatsRects[7].iTop = 50;		gArmorDescStatsRects[7].iRight = 398;	gArmorDescStatsRects[7].iBottom = 71;
			gArmorDescStatsRects[8].iLeft = 340;	gArmorDescStatsRects[8].iTop = 62;		gArmorDescStatsRects[8].iRight = 398;	gArmorDescStatsRects[8].iBottom = 83;
			gArmorDescStatsRects[9].iLeft = 340;	gArmorDescStatsRects[9].iTop = 74;		gArmorDescStatsRects[9].iRight = 398;	gArmorDescStatsRects[9].iBottom = 95;
			gArmorDescStatsRects[10].iLeft = 407;	gArmorDescStatsRects[10].iTop = 12;		gArmorDescStatsRects[10].iRight = 465;	gArmorDescStatsRects[10].iBottom = 23;	// Third Column
			gArmorDescStatsRects[11].iLeft = 407;	gArmorDescStatsRects[11].iTop = 24;		gArmorDescStatsRects[11].iRight = 465;	gArmorDescStatsRects[11].iBottom = 35;
			gArmorDescStatsRects[12].iLeft = 407;	gArmorDescStatsRects[12].iTop = 36;		gArmorDescStatsRects[12].iRight = 465;	gArmorDescStatsRects[12].iBottom = 47;
			gArmorDescStatsRects[13].iLeft = 407;	gArmorDescStatsRects[13].iTop = 48;		gArmorDescStatsRects[13].iRight = 465;	gArmorDescStatsRects[13].iBottom = 59;
			gArmorDescStatsRects[14].iLeft = 407;	gArmorDescStatsRects[14].iTop = 60;		gArmorDescStatsRects[14].iRight = 465;	gArmorDescStatsRects[14].iBottom = 71;	
			gArmorDescStatsRects[15].iLeft = 407;	gArmorDescStatsRects[15].iTop = 72;		gArmorDescStatsRects[15].iRight = 465;	gArmorDescStatsRects[15].iBottom = 83;
			gArmorDescStatsRects[16].iLeft = 407;	gArmorDescStatsRects[16].iTop = 84;		gArmorDescStatsRects[16].iRight = 465;	gArmorDescStatsRects[16].iBottom = 95;
			gArmorDescStatsRects[17].iLeft = 0;		gArmorDescStatsRects[17].iTop = 0;		gArmorDescStatsRects[17].iRight = 1;	gArmorDescStatsRects[17].iBottom = 1;	// Placeholders
			gArmorDescStatsRects[18].iLeft = 0;		gArmorDescStatsRects[18].iTop = 0;		gArmorDescStatsRects[18].iRight = 1;	gArmorDescStatsRects[18].iBottom = 1;
			gArmorDescStatsRects[19].iLeft = 0;		gArmorDescStatsRects[19].iTop = 0;		gArmorDescStatsRects[19].iRight = 1;	gArmorDescStatsRects[19].iBottom = 1;

			gExplosiveDescStatsRects[0].iLeft = 273;	gExplosiveDescStatsRects[0].iTop = 12;	gExplosiveDescStatsRects[0].iRight = 331;	gExplosiveDescStatsRects[0].iBottom = 23;	// First Column
			gExplosiveDescStatsRects[1].iLeft = 273;	gExplosiveDescStatsRects[1].iTop = 24;	gExplosiveDescStatsRects[1].iRight = 331;	gExplosiveDescStatsRects[1].iBottom = 35;
			gExplosiveDescStatsRects[2].iLeft = 273;	gExplosiveDescStatsRects[2].iTop = 36;	gExplosiveDescStatsRects[2].iRight = 331;	gExplosiveDescStatsRects[2].iBottom = 47;
			gExplosiveDescStatsRects[3].iLeft = 273;	gExplosiveDescStatsRects[3].iTop = 48;	gExplosiveDescStatsRects[3].iRight = 331;	gExplosiveDescStatsRects[3].iBottom = 59;
			gExplosiveDescStatsRects[4].iLeft = 358;	gExplosiveDescStatsRects[4].iTop = 12;	gExplosiveDescStatsRects[4].iRight = 374;	gExplosiveDescStatsRects[4].iBottom = 47;	// Second Column
			gExplosiveDescStatsRects[5].iLeft = 342;	gExplosiveDescStatsRects[5].iTop = 12;	gExplosiveDescStatsRects[5].iRight = 358;	gExplosiveDescStatsRects[5].iBottom = 47;
			gExplosiveDescStatsRects[6].iLeft = 376;	gExplosiveDescStatsRects[6].iTop = 12;	gExplosiveDescStatsRects[6].iRight = 395;	gExplosiveDescStatsRects[6].iBottom = 47;
			gExplosiveDescStatsRects[7].iLeft = 341;	gExplosiveDescStatsRects[7].iTop = 60;	gExplosiveDescStatsRects[7].iRight = 465;	gExplosiveDescStatsRects[7].iBottom = 71;
			gExplosiveDescStatsRects[8].iLeft = 0;		gExplosiveDescStatsRects[8].iTop = 0;	gExplosiveDescStatsRects[8].iRight = 1;		gExplosiveDescStatsRects[8].iBottom = 1;	// Placeholders
			gExplosiveDescStatsRects[9].iLeft = 0;		gExplosiveDescStatsRects[9].iTop = 0;	gExplosiveDescStatsRects[9].iRight = 1;		gExplosiveDescStatsRects[9].iBottom = 1;
			gExplosiveDescStatsRects[10].iLeft = 0;		gExplosiveDescStatsRects[10].iTop = 0;	gExplosiveDescStatsRects[10].iRight = 1;	gExplosiveDescStatsRects[10].iBottom = 1;
			gExplosiveDescStatsRects[11].iLeft = 0;		gExplosiveDescStatsRects[11].iTop = 0;	gExplosiveDescStatsRects[11].iRight = 1;	gExplosiveDescStatsRects[11].iBottom = 1;
			gExplosiveDescStatsRects[12].iLeft = 0;		gExplosiveDescStatsRects[12].iTop = 0;	gExplosiveDescStatsRects[12].iRight = 1;	gExplosiveDescStatsRects[12].iBottom = 1;
			gExplosiveDescStatsRects[13].iLeft = 0;		gExplosiveDescStatsRects[13].iTop = 0;	gExplosiveDescStatsRects[13].iRight = 1;	gExplosiveDescStatsRects[13].iBottom = 1;
			gExplosiveDescStatsRects[14].iLeft = 0;		gExplosiveDescStatsRects[14].iTop = 0;	gExplosiveDescStatsRects[14].iRight = 1;	gExplosiveDescStatsRects[14].iBottom = 1;
			gExplosiveDescStatsRects[15].iLeft = 0;		gExplosiveDescStatsRects[15].iTop = 0;	gExplosiveDescStatsRects[15].iRight = 1;	gExplosiveDescStatsRects[15].iBottom = 1;
			gExplosiveDescStatsRects[16].iLeft = 0;		gExplosiveDescStatsRects[16].iTop = 0;	gExplosiveDescStatsRects[16].iRight = 1;	gExplosiveDescStatsRects[16].iBottom = 1;
			gExplosiveDescStatsRects[17].iLeft = 0;		gExplosiveDescStatsRects[17].iTop = 0;	gExplosiveDescStatsRects[17].iRight = 1;	gExplosiveDescStatsRects[17].iBottom = 1;
			gExplosiveDescStatsRects[18].iLeft = 0;		gExplosiveDescStatsRects[18].iTop = 0;	gExplosiveDescStatsRects[18].iRight = 1;	gExplosiveDescStatsRects[18].iBottom = 1;
			gExplosiveDescStatsRects[19].iLeft = 0;		gExplosiveDescStatsRects[19].iTop = 0;	gExplosiveDescStatsRects[19].iRight = 1;	gExplosiveDescStatsRects[19].iBottom = 1;

			gMiscItemDescStatsRects[0].iLeft = 273;		gMiscItemDescStatsRects[0].iTop = 12;	gMiscItemDescStatsRects[0].iRight = 370;	gMiscItemDescStatsRects[0].iBottom = 23; // Column 1
			gMiscItemDescStatsRects[1].iLeft = 273;		gMiscItemDescStatsRects[1].iTop = 24;	gMiscItemDescStatsRects[1].iRight = 370;	gMiscItemDescStatsRects[1].iBottom = 35;
			gMiscItemDescStatsRects[2].iLeft = 273;		gMiscItemDescStatsRects[2].iTop = 36;	gMiscItemDescStatsRects[2].iRight = 370;	gMiscItemDescStatsRects[2].iBottom = 47;
			gMiscItemDescStatsRects[3].iLeft = 273;		gMiscItemDescStatsRects[3].iTop = 48;	gMiscItemDescStatsRects[3].iRight = 370;	gMiscItemDescStatsRects[3].iBottom = 59;
			gMiscItemDescStatsRects[4].iLeft = 273;		gMiscItemDescStatsRects[4].iTop = 60;	gMiscItemDescStatsRects[4].iRight = 370;	gMiscItemDescStatsRects[4].iBottom = 71;
			gMiscItemDescStatsRects[5].iLeft = 273;		gMiscItemDescStatsRects[5].iTop = 72;	gMiscItemDescStatsRects[5].iRight = 370;	gMiscItemDescStatsRects[5].iBottom = 83;
			gMiscItemDescStatsRects[6].iLeft = 273;		gMiscItemDescStatsRects[6].iTop = 84;	gMiscItemDescStatsRects[6].iRight = 370;	gMiscItemDescStatsRects[6].iBottom = 95;
			gMiscItemDescStatsRects[7].iLeft = 374;		gMiscItemDescStatsRects[7].iTop = 12;	gMiscItemDescStatsRects[7].iRight = 471;	gMiscItemDescStatsRects[7].iBottom = 23; // Column Two
			gMiscItemDescStatsRects[8].iLeft = 374;		gMiscItemDescStatsRects[8].iTop = 24;	gMiscItemDescStatsRects[8].iRight = 471;	gMiscItemDescStatsRects[8].iBottom = 35;
			gMiscItemDescStatsRects[9].iLeft = 374;		gMiscItemDescStatsRects[9].iTop = 36;	gMiscItemDescStatsRects[9].iRight = 471;	gMiscItemDescStatsRects[9].iBottom = 47;
			gMiscItemDescStatsRects[10].iLeft = 374;	gMiscItemDescStatsRects[10].iTop = 48;	gMiscItemDescStatsRects[10].iRight = 471;	gMiscItemDescStatsRects[10].iBottom = 59;
			gMiscItemDescStatsRects[11].iLeft = 374;	gMiscItemDescStatsRects[11].iTop = 60;	gMiscItemDescStatsRects[11].iRight = 471;	gMiscItemDescStatsRects[11].iBottom = 71;
			gMiscItemDescStatsRects[12].iLeft = 0;		gMiscItemDescStatsRects[12].iTop = 0;	gMiscItemDescStatsRects[12].iRight = 1;		gMiscItemDescStatsRects[12].iBottom = 1;	// Placeholders
			gMiscItemDescStatsRects[13].iLeft = 0;		gMiscItemDescStatsRects[13].iTop = 0;	gMiscItemDescStatsRects[13].iRight = 1;		gMiscItemDescStatsRects[13].iBottom = 1;
			gMiscItemDescStatsRects[14].iLeft = 0;		gMiscItemDescStatsRects[14].iTop = 0;	gMiscItemDescStatsRects[14].iRight = 1;		gMiscItemDescStatsRects[14].iBottom = 1;
			gMiscItemDescStatsRects[15].iLeft = 0;		gMiscItemDescStatsRects[15].iTop = 0;	gMiscItemDescStatsRects[15].iRight = 1;		gMiscItemDescStatsRects[15].iBottom = 1;
			gMiscItemDescStatsRects[16].iLeft = 0;		gMiscItemDescStatsRects[16].iTop = 0;	gMiscItemDescStatsRects[16].iRight = 1;		gMiscItemDescStatsRects[16].iBottom = 1;
			gMiscItemDescStatsRects[17].iLeft = 0;		gMiscItemDescStatsRects[17].iTop = 0;	gMiscItemDescStatsRects[17].iRight = 1;		gMiscItemDescStatsRects[17].iBottom = 1;
			gMiscItemDescStatsRects[18].iLeft = 0;		gMiscItemDescStatsRects[18].iTop = 0;	gMiscItemDescStatsRects[18].iRight = 1;		gMiscItemDescStatsRects[18].iBottom = 1;
			gMiscItemDescStatsRects[19].iLeft = 0;		gMiscItemDescStatsRects[19].iTop = 0;	gMiscItemDescStatsRects[19].iRight = 1;		gMiscItemDescStatsRects[19].iBottom = 1;
		}
	}
}

void SetupItemDescAttachmentsXY(UINT8 ID, INT16 sX, INT16 sY, INT16 sHeight, INT16 sWidth, INT16 sBarDx, INT16 sBarDy)
{
	gItemDescAttachmentsXY[ID].sX		= sX;
	gItemDescAttachmentsXY[ID].sY		= sY;
	gItemDescAttachmentsXY[ID].sHeight	= sHeight;
	gItemDescAttachmentsXY[ID].sWidth	= sWidth;
	gItemDescAttachmentsXY[ID].sBarDx	= sBarDx;
	gItemDescAttachmentsXY[ID].sBarDy	= sBarDy;
}

void InitDescStatCoords()
{
	if( guiCurrentScreen == MAP_SCREEN )
	{
		SetupItemDescAttachmentsXY(0, 175,  9, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
		SetupItemDescAttachmentsXY(1, 213,  9, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
		SetupItemDescAttachmentsXY(2, 175, 37, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
		SetupItemDescAttachmentsXY(3, 213, 37, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
	}
	else
	{
		SetupItemDescAttachmentsXY(0, 133, 11, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
		SetupItemDescAttachmentsXY(1, 167, 11, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
		SetupItemDescAttachmentsXY(2, 133, 38, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
		SetupItemDescAttachmentsXY(3, 167, 38, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
	}

	if(UsingEDBSystem() > 0)
	{
		if( guiCurrentScreen == MAP_SCREEN )
		{
			gWeaponStats[0].sX = 52;	gWeaponStats[0].sY = 98;	gWeaponStats[0].sValDx = 76;	// 0) STATUS
			gWeaponStats[1].sX = 52;	gWeaponStats[1].sY = 108;	gWeaponStats[1].sValDx = 76;	// 1) WEIGHT
			// Labels and Values
			gWeaponStats[2].sX = 47;	gWeaponStats[2].sY = 124;	gWeaponStats[2].sValDx = 30;	// 2) COL 1; ROW 1
			gWeaponStats[3].sX = 47;	gWeaponStats[3].sY = 136;	gWeaponStats[3].sValDx = 30;	// 3) COL 1; ROW 2
			gWeaponStats[4].sX = 47;	gWeaponStats[4].sY = 148;	gWeaponStats[4].sValDx = 30;	// 4) COL 1; ROW 3
			gWeaponStats[5].sX = 47;	gWeaponStats[5].sY = 160;	gWeaponStats[5].sValDx = 30;	// 5) COL 1; ROW 4
			gWeaponStats[6].sX = 47;	gWeaponStats[6].sY = 172;	gWeaponStats[6].sValDx = 30;	// 6) COL 1; ROW 5
			gWeaponStats[7].sX = 47;	gWeaponStats[7].sY = 184;	gWeaponStats[7].sValDx = 30;	// 7) COL 1; ROW 6
			gWeaponStats[8].sX = 47;	gWeaponStats[8].sY = 196;	gWeaponStats[8].sValDx = 30;	// 8) COL 1; ROW 7
			gWeaponStats[9].sX = 112;	gWeaponStats[9].sY = 124;	gWeaponStats[9].sValDx = 29;	// 9) COL 2; ROW 1
			gWeaponStats[10].sX = 112;	gWeaponStats[10].sY = 136;	gWeaponStats[10].sValDx = 29;	// 10) COL 2; ROW 2
			gWeaponStats[11].sX = 112;	gWeaponStats[11].sY = 148;	gWeaponStats[11].sValDx = 29;	// 11) COL 2; ROW 3
			gWeaponStats[12].sX = 112;	gWeaponStats[12].sY = 160;	gWeaponStats[12].sValDx = 29;	// 12) COL 2; ROW 4
			gWeaponStats[13].sX = 112;	gWeaponStats[13].sY = 172;	gWeaponStats[13].sValDx = 29;	// 13) COL 2; ROW 5
			gWeaponStats[14].sX = 112;	gWeaponStats[14].sY = 184;	gWeaponStats[14].sValDx = 29;	// 14) COL 2; ROW 6
			gWeaponStats[15].sX = 112;	gWeaponStats[15].sY = 196;	gWeaponStats[15].sValDx = 29;	// 15) COL 2; ROW 7
			gWeaponStats[16].sX = 174;	gWeaponStats[16].sY = 124;	gWeaponStats[16].sValDx = 30;	// 16) COL 3; ROW 1
			gWeaponStats[17].sX = 174;	gWeaponStats[17].sY = 136;	gWeaponStats[17].sValDx = 30;	// 17) COL 3; ROW 2
			gWeaponStats[18].sX = 174;	gWeaponStats[18].sY = 148;	gWeaponStats[18].sValDx = 30;	// 18) COL 3; ROW 3
			gWeaponStats[19].sX = 174;	gWeaponStats[19].sY = 160;	gWeaponStats[19].sValDx = 30;	// 19) COL 3; ROW 4
			gWeaponStats[20].sX = 174;	gWeaponStats[20].sY = 172;	gWeaponStats[20].sValDx = 30;	// 20) COL 3; ROW 5
			gWeaponStats[21].sX = 174;	gWeaponStats[21].sY = 184;	gWeaponStats[21].sValDx = 30;	// 21) COL 3; ROW 6
			gWeaponStats[22].sX = 174;	gWeaponStats[22].sY = 196;	gWeaponStats[22].sValDx = 30;	// 22) COL 3; ROW 7
			// EQUALS signs
			gWeaponStats[23].sX = 79;	gWeaponStats[23].sY = 124;	gWeaponStats[23].sValDx = 0;	// 23) COL 1; ROW 1
			gWeaponStats[24].sX = 79;	gWeaponStats[24].sY = 136;	gWeaponStats[24].sValDx = 0;	// 24) COL 1; ROW 2
			gWeaponStats[25].sX = 79;	gWeaponStats[25].sY = 148;	gWeaponStats[25].sValDx = 0;	// 25) COL 1; ROW 3
			gWeaponStats[26].sX = 79;	gWeaponStats[26].sY = 160;	gWeaponStats[26].sValDx = 0;	// 26) COL 1; ROW 4
			gWeaponStats[27].sX = 79;	gWeaponStats[27].sY = 172;	gWeaponStats[27].sValDx = 0;	// 27) COL 1; ROW 5
			gWeaponStats[28].sX = 79;	gWeaponStats[28].sY = 184;	gWeaponStats[28].sValDx = 0;	// 28) COL 1; ROW 6
			gWeaponStats[29].sX = 79;	gWeaponStats[29].sY = 196;	gWeaponStats[29].sValDx = 0;	// 29) COL 1; ROW 7
			gWeaponStats[30].sX = 145;	gWeaponStats[30].sY = 124;	gWeaponStats[30].sValDx = 0;	// 30) COL 2; ROW 1
			gWeaponStats[31].sX = 145;	gWeaponStats[31].sY = 136;	gWeaponStats[31].sValDx = 0;	// 31) COL 2; ROW 2
			gWeaponStats[32].sX = 145;	gWeaponStats[32].sY = 148;	gWeaponStats[32].sValDx = 0;	// 32) COL 2; ROW 3
			gWeaponStats[33].sX = 145;	gWeaponStats[33].sY = 160;	gWeaponStats[33].sValDx = 0;	// 33) COL 2; ROW 4
			gWeaponStats[34].sX = 145;	gWeaponStats[34].sY = 172;	gWeaponStats[34].sValDx = 0;	// 34) COL 2; ROW 5
			gWeaponStats[35].sX = 145;	gWeaponStats[35].sY = 184;	gWeaponStats[35].sValDx = 0;	// 35) COL 2; ROW 6
			gWeaponStats[36].sX = 145;	gWeaponStats[36].sY = 196;	gWeaponStats[36].sValDx = 0;	// 36) COL 2; ROW 7
			gWeaponStats[37].sX = 206;	gWeaponStats[37].sY = 124;	gWeaponStats[37].sValDx = 0;	// 37) COL 3; ROW 1
			gWeaponStats[38].sX = 206;	gWeaponStats[38].sY = 136;	gWeaponStats[38].sValDx = 0;	// 38) COL 3; ROW 2
			gWeaponStats[39].sX = 206;	gWeaponStats[39].sY = 148;	gWeaponStats[39].sValDx = 0;	// 39) COL 3; ROW 3
			gWeaponStats[40].sX = 206;	gWeaponStats[40].sY = 160;	gWeaponStats[40].sValDx = 0;	// 40) COL 3; ROW 4
			gWeaponStats[41].sX = 206;	gWeaponStats[41].sY = 172;	gWeaponStats[41].sValDx = 0;	// 41) COL 3; ROW 5
			gWeaponStats[42].sX = 206;	gWeaponStats[42].sY = 184;	gWeaponStats[42].sValDx = 0;	// 42) COL 3; ROW 6
			gWeaponStats[43].sX = 206;	gWeaponStats[43].sY = 196;	gWeaponStats[43].sValDx = 0;	// 43) COL 3; ROW 7
			// Nonregular locations for explosive data (COL 2; ROW 3)
			gWeaponStats[44].sX = 116;	gWeaponStats[44].sY = 148;	gWeaponStats[44].sValDx = 0;	// 44) COL 2; ROW 3; LOC 1
			gWeaponStats[45].sX = 134;	gWeaponStats[45].sY = 148;	gWeaponStats[45].sValDx = 0;	// 45) COL 2; ROW 3; LOC 2
			gWeaponStats[46].sX = 150;	gWeaponStats[46].sY = 148;	gWeaponStats[46].sValDx = 0;	// 46) COL 2; ROW 3; LOC 3

			gMoneyStats[0].sX = 51;	gMoneyStats[0].sY = 124;	gMoneyStats[0].sValDx = 45;	// Money
			gMoneyStats[1].sX = 61;	gMoneyStats[1].sY = 136;	gMoneyStats[1].sValDx = 65;	// Remaining
			gMoneyStats[2].sX = 51;	gMoneyStats[2].sY = 172;	gMoneyStats[2].sValDx = 45;	// Money
			gMoneyStats[3].sX = 61;	gMoneyStats[3].sY = 184;	gMoneyStats[3].sValDx = 65;	// Removing
			gMoneyStats[4].sX = 47;	gMoneyStats[4].sY = 148;	gMoneyStats[4].sValDx = 62;	// AMT remaining
			gMoneyStats[5].sX = 47;	gMoneyStats[5].sY = 196;	gMoneyStats[5].sValDx = 62;	// AMT removing

			gItemDescProsConsRects[0].iLeft = 14;	gItemDescProsConsRects[0].iTop = 291;	gItemDescProsConsRects[0].iRight = 250;	gItemDescProsConsRects[0].iBottom = 298;
			gItemDescProsConsRects[1].iLeft = 14;	gItemDescProsConsRects[1].iTop = 299;	gItemDescProsConsRects[1].iRight = 250;	gItemDescProsConsRects[1].iBottom = 306;
		}
		else
		{
			if (UsingEDBSystem() == 1) // NIV EDB
			{
				gWeaponStats[0].sX = 205;	gWeaponStats[0].sY = 14;	gWeaponStats[0].sValDx = 91;	// 0) STATUS
				gWeaponStats[1].sX = 205;	gWeaponStats[1].sY = 24;	gWeaponStats[1].sValDx = 91;	// 1) WEIGHT
				// Labels and Values
				gWeaponStats[2].sX = 203;	gWeaponStats[2].sY = 44;	gWeaponStats[2].sValDx = 30;	// 2) COL 1; ROW 1
				gWeaponStats[3].sX = 203;	gWeaponStats[3].sY = 56;	gWeaponStats[3].sValDx = 30;	// 3) COL 1; ROW 2
				gWeaponStats[4].sX = 203;	gWeaponStats[4].sY = 68;	gWeaponStats[4].sValDx = 30;	// 4) COL 1; ROW 3
				gWeaponStats[5].sX = 203;	gWeaponStats[5].sY = 80;	gWeaponStats[5].sValDx = 30;	// 5) COL 1; ROW 4
				gWeaponStats[6].sX = 203;	gWeaponStats[6].sY = 92;	gWeaponStats[6].sValDx = 30;	// 6) COL 1; ROW 5
				gWeaponStats[7].sX = 203;	gWeaponStats[7].sY = 104;	gWeaponStats[7].sValDx = 30;	// 7) COL 1; ROW 6
				gWeaponStats[8].sX = 203;	gWeaponStats[8].sY = 116;	gWeaponStats[8].sValDx = 30;	// 8) COL 1; ROW 7
				gWeaponStats[9].sX = 268;	gWeaponStats[9].sY = 44;	gWeaponStats[9].sValDx = 30;	// 9) COL 2; ROW 1
				gWeaponStats[10].sX = 268;	gWeaponStats[10].sY = 56;	gWeaponStats[10].sValDx = 30;	// 10) COL 2; ROW 2
				gWeaponStats[11].sX = 268;	gWeaponStats[11].sY = 68;	gWeaponStats[11].sValDx = 30;	// 11) COL 2; ROW 3
				gWeaponStats[12].sX = 268;	gWeaponStats[12].sY = 80;	gWeaponStats[12].sValDx = 30;	// 12) COL 2; ROW 4
				gWeaponStats[13].sX = 268;	gWeaponStats[13].sY = 92;	gWeaponStats[13].sValDx = 30;	// 13) COL 2; ROW 5
				gWeaponStats[14].sX = 268;	gWeaponStats[14].sY = 104;	gWeaponStats[14].sValDx = 30;	// 14) COL 2; ROW 6
				gWeaponStats[15].sX = 268;	gWeaponStats[15].sY = 116;	gWeaponStats[15].sValDx = 30;	// 15) COL 2; ROW 7
				gWeaponStats[16].sX = 203;	gWeaponStats[16].sY = 136;	gWeaponStats[16].sValDx = 30;	// 16) COL 3; ROW 1
				gWeaponStats[17].sX = 203;	gWeaponStats[17].sY = 160;	gWeaponStats[17].sValDx = 30;	// 17) COL 3; ROW 3
				gWeaponStats[18].sX = 268;	gWeaponStats[18].sY = 136;	gWeaponStats[18].sValDx = 30;	// 18) COL 4; ROW 1
				gWeaponStats[19].sX = 268;	gWeaponStats[19].sY = 148;	gWeaponStats[19].sValDx = 30;	// 19) COL 4; ROW 2
				gWeaponStats[20].sX = 268;	gWeaponStats[20].sY = 160;	gWeaponStats[20].sValDx = 30;	// 20) COL 4; ROW 3
				gWeaponStats[21].sX = 268;	gWeaponStats[21].sY = 172;	gWeaponStats[21].sValDx = 30;	// 21) COL 4; ROW 4
				gWeaponStats[22].sX = 203;	gWeaponStats[22].sY = 172;	gWeaponStats[22].sValDx = 30;	// 22) COL 3; ROW 4
				// EQUALS signs
				gWeaponStats[23].sX = 235;	gWeaponStats[23].sY = 44;	gWeaponStats[23].sValDx = 0;	// 23) COL 1; ROW 1
				gWeaponStats[24].sX = 235;	gWeaponStats[24].sY = 56;	gWeaponStats[24].sValDx = 0;	// 24) COL 1; ROW 2
				gWeaponStats[25].sX = 235;	gWeaponStats[25].sY = 68;	gWeaponStats[25].sValDx = 0;	// 25) COL 1; ROW 3
				gWeaponStats[26].sX = 235;	gWeaponStats[26].sY = 80;	gWeaponStats[26].sValDx = 0;	// 26) COL 1; ROW 4
				gWeaponStats[27].sX = 235;	gWeaponStats[27].sY = 92;	gWeaponStats[27].sValDx = 0;	// 27) COL 1; ROW 5
				gWeaponStats[28].sX = 235;	gWeaponStats[28].sY = 104;	gWeaponStats[28].sValDx = 0;	// 28) COL 1; ROW 6
				gWeaponStats[29].sX = 235;	gWeaponStats[29].sY = 116;	gWeaponStats[29].sValDx = 0;	// 29) COL 1; ROW 7
				gWeaponStats[30].sX = 300;	gWeaponStats[30].sY = 44;	gWeaponStats[30].sValDx = 0;	// 30) COL 2; ROW 1
				gWeaponStats[31].sX = 300;	gWeaponStats[31].sY = 56;	gWeaponStats[31].sValDx = 0;	// 31) COL 2; ROW 2
				gWeaponStats[32].sX = 300;	gWeaponStats[32].sY = 68;	gWeaponStats[32].sValDx = 0;	// 32) COL 2; ROW 3
				gWeaponStats[33].sX = 300;	gWeaponStats[33].sY = 80;	gWeaponStats[33].sValDx = 0;	// 33) COL 2; ROW 4
				gWeaponStats[34].sX = 300;	gWeaponStats[34].sY = 92;	gWeaponStats[34].sValDx = 0;	// 34) COL 2; ROW 5
				gWeaponStats[35].sX = 300;	gWeaponStats[35].sY = 104;	gWeaponStats[35].sValDx = 0;	// 35) COL 2; ROW 6
				gWeaponStats[36].sX = 300;	gWeaponStats[36].sY = 116;	gWeaponStats[36].sValDx = 0;	// 36) COL 2; ROW 7
				gWeaponStats[37].sX = 235;	gWeaponStats[37].sY = 136;	gWeaponStats[37].sValDx = 0;	// 37) COL 3; ROW 1
				gWeaponStats[38].sX = 235;	gWeaponStats[38].sY = 160;	gWeaponStats[38].sValDx = 0;	// 38) COL 3; ROW 3
				gWeaponStats[39].sX = 300;	gWeaponStats[39].sY = 136;	gWeaponStats[39].sValDx = 0;	// 39) COL 4; ROW 1
				gWeaponStats[40].sX = 300;	gWeaponStats[40].sY = 148;	gWeaponStats[40].sValDx = 0;	// 40) COL 4; ROW 2
				gWeaponStats[41].sX = 300;	gWeaponStats[41].sY = 160;	gWeaponStats[41].sValDx = 0;	// 41) COL 4; ROW 3
				gWeaponStats[42].sX = 300;	gWeaponStats[42].sY = 172;	gWeaponStats[42].sValDx = 0;	// 42) COL 4; ROW 4
				gWeaponStats[43].sX = 235;	gWeaponStats[43].sY = 172;	gWeaponStats[43].sValDx = 0;	// 43) COL 3; ROW 4
				// Nonregular locations for explosive radius values (COL 2; ROW 3)
				gWeaponStats[44].sX = 273;	gWeaponStats[44].sY = 68;	gWeaponStats[44].sValDx = 0;	// 44) COL 2; ROW 3; LOC 1 - Starting Radius
				gWeaponStats[45].sX = 291;	gWeaponStats[45].sY = 68;	gWeaponStats[45].sValDx = 0;	// 45) COL 2; ROW 3; LOC 2 - Single Radius
				gWeaponStats[46].sX = 307;	gWeaponStats[46].sY = 68;	gWeaponStats[46].sValDx = 0;	// 46) COL 2; ROW 3; LOC 3 - Final Radius

				gMoneyStats[0].sX = 203;	gMoneyStats[0].sY = 44;		gMoneyStats[0].sValDx = 45;		// Money
				gMoneyStats[1].sX = 213;	gMoneyStats[1].sY = 56;		gMoneyStats[1].sValDx = 65;		// Remaining
				gMoneyStats[2].sX = 203;	gMoneyStats[2].sY = 92;		gMoneyStats[2].sValDx = 45;		// Money
				gMoneyStats[3].sX = 213;	gMoneyStats[3].sY = 104;	gMoneyStats[3].sValDx = 65;		// Removing
				gMoneyStats[4].sX = 203;	gMoneyStats[4].sY = 56;		gMoneyStats[4].sValDx = 100;	// AMT remaining
				gMoneyStats[5].sX = 203;	gMoneyStats[5].sY = 104;	gMoneyStats[5].sValDx = 100;	// AMT removing

				gItemDescProsConsRects[0].iLeft = 14;	gItemDescProsConsRects[0].iTop = 169;	gItemDescProsConsRects[0].iRight = 188;	gItemDescProsConsRects[0].iBottom = 176;
				gItemDescProsConsRects[1].iLeft = 14;	gItemDescProsConsRects[1].iTop = 177;	gItemDescProsConsRects[1].iRight = 188;	gItemDescProsConsRects[1].iBottom = 184;
			}
			if (UsingEDBSystem() == 2) // OIV EDB
			{
				gWeaponStats[0].sX = 202;	gWeaponStats[0].sY = 16;	gWeaponStats[0].sValDx = 36;	// 0) STATUS
				gWeaponStats[1].sX = 202;	gWeaponStats[1].sY = 64;	gWeaponStats[1].sValDx = 36;	// 1) WEIGHT
				// Labels and Values
				gWeaponStats[2].sX = 274;	gWeaponStats[2].sY = 14;	gWeaponStats[2].sValDx = 30;	// 2) COL 1; ROW 1
				gWeaponStats[3].sX = 274;	gWeaponStats[3].sY = 26;	gWeaponStats[3].sValDx = 30;	// 3) COL 1; ROW 2
				gWeaponStats[4].sX = 274;	gWeaponStats[4].sY = 38;	gWeaponStats[4].sValDx = 30;	// 4) COL 1; ROW 3
				gWeaponStats[5].sX = 274;	gWeaponStats[5].sY = 50;	gWeaponStats[5].sValDx = 30;	// 5) COL 1; ROW 4
				gWeaponStats[6].sX = 274;	gWeaponStats[6].sY = 62;	gWeaponStats[6].sValDx = 30;	// 6) COL 1; ROW 5
				gWeaponStats[7].sX = 274;	gWeaponStats[7].sY = 74;	gWeaponStats[7].sValDx = 30;	// 7) COL 1; ROW 6
				gWeaponStats[8].sX = 274;	gWeaponStats[8].sY = 86;	gWeaponStats[8].sValDx = 30;	// 8) COL 1; ROW 7
				gWeaponStats[9].sX = 341;	gWeaponStats[9].sY = 14;	gWeaponStats[9].sValDx = 30;	// 9) COL 2; ROW 1
				gWeaponStats[10].sX = 341;	gWeaponStats[10].sY = 26;	gWeaponStats[10].sValDx = 30;	// 10) COL 2; ROW 2
				gWeaponStats[11].sX = 341;	gWeaponStats[11].sY = 38;	gWeaponStats[11].sValDx = 30;	// 11) COL 2; ROW 3
				gWeaponStats[12].sX = 341;	gWeaponStats[12].sY = 50;	gWeaponStats[12].sValDx = 30;	// 12) COL 2; ROW 4
				gWeaponStats[13].sX = 341;	gWeaponStats[13].sY = 62;	gWeaponStats[13].sValDx = 30;	// 13) COL 2; ROW 5
				gWeaponStats[14].sX = 341;	gWeaponStats[14].sY = 74;	gWeaponStats[14].sValDx = 30;	// 14) COL 2; ROW 6
				gWeaponStats[15].sX = 341;	gWeaponStats[15].sY = 86;	gWeaponStats[15].sValDx = 30;	// 15) COL 2; ROW 7
				gWeaponStats[16].sX = 408;	gWeaponStats[16].sY = 14;	gWeaponStats[16].sValDx = 30;	// 16) COL 3; ROW 1
				gWeaponStats[17].sX = 408;	gWeaponStats[17].sY = 26;	gWeaponStats[17].sValDx = 30;	// 17) COL 3; ROW 2
				gWeaponStats[18].sX = 408;	gWeaponStats[18].sY = 38;	gWeaponStats[18].sValDx = 30;	// 18) COL 3; ROW 3
				gWeaponStats[19].sX = 408;	gWeaponStats[19].sY = 50;	gWeaponStats[19].sValDx = 30;	// 19) COL 3; ROW 4
				gWeaponStats[20].sX = 408;	gWeaponStats[20].sY = 62;	gWeaponStats[20].sValDx = 30;	// 20) COL 3; ROW 5
				gWeaponStats[21].sX = 408;	gWeaponStats[21].sY = 74;	gWeaponStats[21].sValDx = 30;	// 21) COL 3; ROW 6
				gWeaponStats[22].sX = 408;	gWeaponStats[22].sY = 86;	gWeaponStats[22].sValDx = 30;	// 22) COL 3; ROW 7
				// EQUALS signs
				gWeaponStats[23].sX = 306;	gWeaponStats[23].sY = 14;	gWeaponStats[23].sValDx = 0;	// 23) COL 1; ROW 1
				gWeaponStats[24].sX = 306;	gWeaponStats[24].sY = 26;	gWeaponStats[24].sValDx = 0;	// 24) COL 1; ROW 2
				gWeaponStats[25].sX = 306;	gWeaponStats[25].sY = 38;	gWeaponStats[25].sValDx = 0;	// 25) COL 1; ROW 3
				gWeaponStats[26].sX = 306;	gWeaponStats[26].sY = 50;	gWeaponStats[26].sValDx = 0;	// 26) COL 1; ROW 4
				gWeaponStats[27].sX = 306;	gWeaponStats[27].sY = 62;	gWeaponStats[27].sValDx = 0;	// 27) COL 1; ROW 5
				gWeaponStats[28].sX = 306;	gWeaponStats[28].sY = 74;	gWeaponStats[28].sValDx = 0;	// 28) COL 1; ROW 6
				gWeaponStats[29].sX = 306;	gWeaponStats[29].sY = 86;	gWeaponStats[29].sValDx = 0;	// 39) COL 1; ROW 7
				gWeaponStats[30].sX = 373;	gWeaponStats[30].sY = 14;	gWeaponStats[30].sValDx = 0;	// 30) COL 2; ROW 1
				gWeaponStats[31].sX = 373;	gWeaponStats[31].sY = 26;	gWeaponStats[31].sValDx = 0;	// 31) COL 2; ROW 2
				gWeaponStats[32].sX = 373;	gWeaponStats[32].sY = 38;	gWeaponStats[32].sValDx = 0;	// 32) COL 2; ROW 3
				gWeaponStats[33].sX = 373;	gWeaponStats[33].sY = 50;	gWeaponStats[33].sValDx = 0;	// 33) COL 2; ROW 4
				gWeaponStats[34].sX = 373;	gWeaponStats[34].sY = 62;	gWeaponStats[34].sValDx = 0;	// 34) COL 2; ROW 5
				gWeaponStats[35].sX = 373;	gWeaponStats[35].sY = 74;	gWeaponStats[35].sValDx = 0;	// 35) COL 2; ROW 6
				gWeaponStats[36].sX = 373;	gWeaponStats[36].sY = 86;	gWeaponStats[36].sValDx = 0;	// 36) COL 2; ROW 7
				gWeaponStats[37].sX = 440;	gWeaponStats[37].sY = 14;	gWeaponStats[37].sValDx = 0;	// 37) COL 3; ROW 1
				gWeaponStats[38].sX = 440;	gWeaponStats[38].sY = 26;	gWeaponStats[38].sValDx = 0;	// 48) COL 3; ROW 2
				gWeaponStats[39].sX = 440;	gWeaponStats[39].sY = 38;	gWeaponStats[39].sValDx = 0;	// 49) COL 3; ROW 3
				gWeaponStats[40].sX = 440;	gWeaponStats[40].sY = 50;	gWeaponStats[40].sValDx = 0;	// 40) COL 3; ROW 4
				gWeaponStats[41].sX = 440;	gWeaponStats[41].sY = 62;	gWeaponStats[41].sValDx = 0;	// 41) COL 3; ROW 5
				gWeaponStats[42].sX = 440;	gWeaponStats[42].sY = 74;	gWeaponStats[42].sValDx = 0;	// 42) COL 3; ROW 6
				gWeaponStats[43].sX = 440;	gWeaponStats[43].sY = 86;	gWeaponStats[43].sValDx = 0;	// 43) COL 3; ROW 7
				// Nonregular locations for explosive radius values (COL 2; ROW 3)
				gWeaponStats[44].sX = 345;	gWeaponStats[44].sY = 38;	gWeaponStats[44].sValDx = 0;	// 44) COL 2; ROW 3; LOC 1 - Starting Radius
				gWeaponStats[45].sX = 362;	gWeaponStats[45].sY = 38;	gWeaponStats[45].sValDx = 0;	// 45) COL 2; ROW 3; LOC 2 - Single Radius
				gWeaponStats[46].sX = 381;	gWeaponStats[46].sY = 38;	gWeaponStats[46].sValDx = 0;	// 46) COL 2; ROW 3; LOC 3 - Final Radius

				gMoneyStats[0].sX = 202;	gMoneyStats[0].sY = 14;		gMoneyStats[0].sValDx = 45;		// Money
				gMoneyStats[1].sX = 202;	gMoneyStats[1].sY = 26;		gMoneyStats[1].sValDx = 65;		// Remaining
				gMoneyStats[2].sX = 202;	gMoneyStats[2].sY = 62;		gMoneyStats[2].sValDx = 45;		// Money
				gMoneyStats[3].sX = 202;	gMoneyStats[3].sY = 74;	gMoneyStats[3].sValDx = 65;		// Removing
				gMoneyStats[4].sX = 202;	gMoneyStats[4].sY = 38;		gMoneyStats[4].sValDx = 100;	// AMT remaining
				gMoneyStats[5].sX = 202;	gMoneyStats[5].sY = 86;	gMoneyStats[5].sValDx = 100;	// AMT removing

				gItemDescProsConsRects[0].iLeft = 433;	gItemDescProsConsRects[0].iTop = 104;	gItemDescProsConsRects[0].iRight = 467;	gItemDescProsConsRects[0].iBottom = 115;
				gItemDescProsConsRects[1].iLeft = 433;	gItemDescProsConsRects[1].iTop = 116;	gItemDescProsConsRects[1].iRight = 467;	gItemDescProsConsRects[1].iBottom = 127;
			}
		}
	}
	else
	{
		if(guiCurrentScreen ==MAP_SCREEN)
		{
			gWeaponStats[0].sX = 52;	gWeaponStats[0].sY = 98;	gWeaponStats[0].sValDx = 89;	// 0) STATUS
			gWeaponStats[1].sX = 52;	gWeaponStats[1].sY = 108;	gWeaponStats[1].sValDx = 89;	// 1) WEIGHT
			gWeaponStats[2].sX = 117;	gWeaponStats[2].sY = 124;	gWeaponStats[2].sValDx = 21;	// 2) RANGE
			gWeaponStats[3].sX = 52;	gWeaponStats[3].sY = 124;	gWeaponStats[3].sValDx = 30;	// 3) DAMAGE
			gWeaponStats[4].sX = 52;	gWeaponStats[4].sY = 98;	gWeaponStats[4].sValDx = 80;
			gWeaponStats[5].sX = 52;	gWeaponStats[5].sY = 135;	gWeaponStats[5].sValDx = 30;	// 5) AP
			gWeaponStats[6].sX = 117;	gWeaponStats[6].sY = 135;	gWeaponStats[6].sValDx = 25;
			gWeaponStats[7].sX = 86;	gWeaponStats[7].sY = 135;	gWeaponStats[7].sValDx = 0;	// 7) =
			gWeaponStats[8].sX = 145;	gWeaponStats[8].sY = 135;	gWeaponStats[8].sValDx = 0;	// 8) =
			gWeaponStats[9].sX = 0;		gWeaponStats[9].sY = 0;		gWeaponStats[9].sValDx = 0;
			gWeaponStats[10].sX = 0;	gWeaponStats[10].sY = 0;	gWeaponStats[10].sValDx = 0;
			gWeaponStats[11].sX = 0;	gWeaponStats[11].sY = 0;	gWeaponStats[11].sValDx = 0;
			gWeaponStats[12].sX = 0;	gWeaponStats[12].sY = 0;	gWeaponStats[12].sValDx = 0;
			gWeaponStats[13].sX = 0;	gWeaponStats[13].sY = 0;	gWeaponStats[13].sValDx = 0;
			gWeaponStats[14].sX = 0;	gWeaponStats[14].sY = 0;	gWeaponStats[14].sValDx = 0;
			gWeaponStats[15].sX = 0;	gWeaponStats[15].sY = 0;	gWeaponStats[15].sValDx = 0;
			gWeaponStats[16].sX = 0;	gWeaponStats[16].sY = 0;	gWeaponStats[16].sValDx = 0;
			gWeaponStats[17].sX = 0;	gWeaponStats[17].sY = 0;	gWeaponStats[17].sValDx = 0;
			gWeaponStats[18].sX = 0;	gWeaponStats[18].sY = 0;	gWeaponStats[18].sValDx = 0;
			gWeaponStats[19].sX = 0;	gWeaponStats[19].sY = 0;	gWeaponStats[19].sValDx = 0;
			gWeaponStats[20].sX = 0;	gWeaponStats[20].sY = 0;	gWeaponStats[20].sValDx = 0;
			gWeaponStats[21].sX = 0;	gWeaponStats[21].sY = 0;	gWeaponStats[21].sValDx = 0;
			gWeaponStats[22].sX = 0;	gWeaponStats[22].sY = 0;	gWeaponStats[22].sValDx = 0;
			gWeaponStats[23].sX = 0;	gWeaponStats[23].sY = 0;	gWeaponStats[23].sValDx = 0;
			gWeaponStats[24].sX = 0;	gWeaponStats[24].sY = 0;	gWeaponStats[24].sValDx = 0;
			gWeaponStats[25].sX = 0;	gWeaponStats[25].sY = 0;	gWeaponStats[25].sValDx = 0;
			gWeaponStats[26].sX = 0;	gWeaponStats[26].sY = 0;	gWeaponStats[26].sValDx = 0;
			gWeaponStats[27].sX = 0;	gWeaponStats[27].sY = 0;	gWeaponStats[27].sValDx = 0;
			gWeaponStats[28].sX = 0;	gWeaponStats[28].sY = 0;	gWeaponStats[28].sValDx = 0;
			gWeaponStats[29].sX = 0;	gWeaponStats[29].sY = 0;	gWeaponStats[29].sValDx = 0;
			gWeaponStats[30].sX = 0;	gWeaponStats[30].sY = 0;	gWeaponStats[30].sValDx = 0;
			gWeaponStats[31].sX = 0;	gWeaponStats[31].sY = 0;	gWeaponStats[31].sValDx = 0;
			gWeaponStats[32].sX = 0;	gWeaponStats[32].sY = 0;	gWeaponStats[32].sValDx = 0;
			gWeaponStats[33].sX = 0;	gWeaponStats[33].sY = 0;	gWeaponStats[33].sValDx = 0;
			gWeaponStats[34].sX = 0;	gWeaponStats[34].sY = 0;	gWeaponStats[34].sValDx = 0;
			gWeaponStats[35].sX = 0;	gWeaponStats[35].sY = 0;	gWeaponStats[35].sValDx = 0;
			gWeaponStats[36].sX = 0;	gWeaponStats[36].sY = 0;	gWeaponStats[36].sValDx = 0;
			gWeaponStats[37].sX = 0;	gWeaponStats[37].sY = 0;	gWeaponStats[37].sValDx = 0;
			gWeaponStats[38].sX = 0;	gWeaponStats[38].sY = 0;	gWeaponStats[38].sValDx = 0;
			gWeaponStats[39].sX = 0;	gWeaponStats[39].sY = 0;	gWeaponStats[39].sValDx = 0;
			gWeaponStats[40].sX = 0;	gWeaponStats[40].sY = 0;	gWeaponStats[40].sValDx = 0;
			gWeaponStats[41].sX = 0;	gWeaponStats[41].sY = 0;	gWeaponStats[41].sValDx = 0;
			gWeaponStats[42].sX = 0;	gWeaponStats[42].sY = 0;	gWeaponStats[42].sValDx = 0;
			gWeaponStats[43].sX = 0;	gWeaponStats[43].sY = 0;	gWeaponStats[43].sValDx = 0;
			gWeaponStats[44].sX = 0;	gWeaponStats[44].sY = 0;	gWeaponStats[44].sValDx = 0;
			gWeaponStats[45].sX = 0;	gWeaponStats[45].sY = 0;	gWeaponStats[45].sValDx = 0;
			gWeaponStats[46].sX = 0;	gWeaponStats[46].sY = 0;	gWeaponStats[46].sValDx = 0;

			gMoneyStats[0].sX = 51;	gMoneyStats[0].sY = 97;		gMoneyStats[0].sValDx = 45;
			gMoneyStats[1].sX = 61;	gMoneyStats[1].sY = 107;	gMoneyStats[1].sValDx = 75;
			gMoneyStats[2].sX = 51;	gMoneyStats[2].sY = 125;	gMoneyStats[2].sValDx = 45;
			gMoneyStats[3].sX = 61;	gMoneyStats[3].sY = 135;	gMoneyStats[3].sValDx = 70;
			gMoneyStats[4].sX = 0;	gMoneyStats[4].sY = 0;		gMoneyStats[4].sValDx = 0;
			gMoneyStats[5].sX = 0;	gMoneyStats[5].sY = 0;		gMoneyStats[5].sValDx = 0;

			gItemDescProsConsRects[0].iLeft = 14;	gItemDescProsConsRects[0].iTop = 231;	gItemDescProsConsRects[0].iRight = 240;	gItemDescProsConsRects[0].iBottom = 238;
			gItemDescProsConsRects[1].iLeft = 14;	gItemDescProsConsRects[1].iTop = 239;	gItemDescProsConsRects[1].iRight = 240;	gItemDescProsConsRects[1].iBottom = 246;
		}
		else
		{
			gWeaponStats[0].sX = 202;	gWeaponStats[0].sY = 15;	gWeaponStats[0].sValDx = 83;	// 0) STATUS
			gWeaponStats[1].sX = 202;	gWeaponStats[1].sY = 25;	gWeaponStats[1].sValDx = 83;	// 1) WEIGHT
			gWeaponStats[2].sX = 265;	gWeaponStats[2].sY = 40;	gWeaponStats[2].sValDx = 20;	// 2) RANGE
			gWeaponStats[3].sX = 202;	gWeaponStats[3].sY = 40;	gWeaponStats[3].sValDx = 32;	// 3) DAMAGE
			gWeaponStats[4].sX = 202;	gWeaponStats[4].sY = 15;	gWeaponStats[4].sValDx = 83;	// 4) KEY
			gWeaponStats[5].sX = 202;	gWeaponStats[5].sY = 50;	gWeaponStats[5].sValDx = 32;	// 5) AP
			gWeaponStats[6].sX = 265;	gWeaponStats[6].sY = 50;	gWeaponStats[6].sValDx = 20;
			gWeaponStats[7].sX = 234;	gWeaponStats[7].sY = 50;	gWeaponStats[7].sValDx = 0;		// 7) =
			gWeaponStats[8].sX = 290;	gWeaponStats[8].sY = 50;	gWeaponStats[8].sValDx = 0;		// 8) =
			gWeaponStats[9].sX = 0;		gWeaponStats[9].sY = 0;		gWeaponStats[9].sValDx = 0;
			gWeaponStats[10].sX = 0;	gWeaponStats[10].sY = 0;	gWeaponStats[10].sValDx = 0;
			gWeaponStats[11].sX = 0;	gWeaponStats[11].sY = 0;	gWeaponStats[11].sValDx = 0;
			gWeaponStats[12].sX = 0;	gWeaponStats[12].sY = 0;	gWeaponStats[12].sValDx = 0;
			gWeaponStats[13].sX = 0;	gWeaponStats[13].sY = 0;	gWeaponStats[13].sValDx = 0;
			gWeaponStats[14].sX = 0;	gWeaponStats[14].sY = 0;	gWeaponStats[14].sValDx = 0;
			gWeaponStats[15].sX = 0;	gWeaponStats[15].sY = 0;	gWeaponStats[15].sValDx = 0;
			gWeaponStats[16].sX = 0;	gWeaponStats[16].sY = 0;	gWeaponStats[16].sValDx = 0;
			gWeaponStats[17].sX = 0;	gWeaponStats[17].sY = 0;	gWeaponStats[17].sValDx = 0;
			gWeaponStats[18].sX = 0;	gWeaponStats[18].sY = 0;	gWeaponStats[18].sValDx = 0;
			gWeaponStats[19].sX = 0;	gWeaponStats[19].sY = 0;	gWeaponStats[19].sValDx = 0;
			gWeaponStats[20].sX = 0;	gWeaponStats[20].sY = 0;	gWeaponStats[20].sValDx = 0;
			gWeaponStats[21].sX = 0;	gWeaponStats[21].sY = 0;	gWeaponStats[21].sValDx = 0;
			gWeaponStats[22].sX = 0;	gWeaponStats[22].sY = 0;	gWeaponStats[22].sValDx = 0;
			gWeaponStats[23].sX = 0;	gWeaponStats[23].sY = 0;	gWeaponStats[23].sValDx = 0;
			gWeaponStats[24].sX = 0;	gWeaponStats[24].sY = 0;	gWeaponStats[24].sValDx = 0;
			gWeaponStats[25].sX = 0;	gWeaponStats[25].sY = 0;	gWeaponStats[25].sValDx = 0;
			gWeaponStats[26].sX = 0;	gWeaponStats[26].sY = 0;	gWeaponStats[26].sValDx = 0;
			gWeaponStats[27].sX = 0;	gWeaponStats[27].sY = 0;	gWeaponStats[27].sValDx = 0;
			gWeaponStats[28].sX = 0;	gWeaponStats[28].sY = 0;	gWeaponStats[28].sValDx = 0;
			gWeaponStats[29].sX = 0;	gWeaponStats[29].sY = 0;	gWeaponStats[29].sValDx = 0;
			gWeaponStats[30].sX = 0;	gWeaponStats[30].sY = 0;	gWeaponStats[30].sValDx = 0;
			gWeaponStats[31].sX = 0;	gWeaponStats[31].sY = 0;	gWeaponStats[31].sValDx = 0;
			gWeaponStats[32].sX = 0;	gWeaponStats[32].sY = 0;	gWeaponStats[32].sValDx = 0;
			gWeaponStats[33].sX = 0;	gWeaponStats[33].sY = 0;	gWeaponStats[33].sValDx = 0;
			gWeaponStats[34].sX = 0;	gWeaponStats[34].sY = 0;	gWeaponStats[34].sValDx = 0;
			gWeaponStats[35].sX = 0;	gWeaponStats[35].sY = 0;	gWeaponStats[35].sValDx = 0;
			gWeaponStats[36].sX = 0;	gWeaponStats[36].sY = 0;	gWeaponStats[36].sValDx = 0;
			gWeaponStats[37].sX = 0;	gWeaponStats[37].sY = 0;	gWeaponStats[37].sValDx = 0;
			gWeaponStats[38].sX = 0;	gWeaponStats[38].sY = 0;	gWeaponStats[38].sValDx = 0;
			gWeaponStats[39].sX = 0;	gWeaponStats[39].sY = 0;	gWeaponStats[39].sValDx = 0;
			gWeaponStats[40].sX = 0;	gWeaponStats[40].sY = 0;	gWeaponStats[40].sValDx = 0;
			gWeaponStats[41].sX = 0;	gWeaponStats[41].sY = 0;	gWeaponStats[41].sValDx = 0;
			gWeaponStats[42].sX = 0;	gWeaponStats[42].sY = 0;	gWeaponStats[42].sValDx = 0;
			gWeaponStats[43].sX = 0;	gWeaponStats[43].sY = 0;	gWeaponStats[43].sValDx = 0;
			gWeaponStats[44].sX = 0;	gWeaponStats[44].sY = 0;	gWeaponStats[44].sValDx = 0;
			gWeaponStats[45].sX = 0;	gWeaponStats[45].sY = 0;	gWeaponStats[45].sValDx = 0;
			gWeaponStats[46].sX = 0;	gWeaponStats[46].sY = 0;	gWeaponStats[46].sValDx = 0;

			gMoneyStats[0].sX = 202;	gMoneyStats[0].sY = 14;		gMoneyStats[0].sValDx = 78;
			gMoneyStats[1].sX = 212;	gMoneyStats[1].sY = 25;		gMoneyStats[1].sValDx = 78;
			gMoneyStats[2].sX = 203;	gMoneyStats[2].sY = 40;		gMoneyStats[2].sValDx = 78;
			gMoneyStats[3].sX = 212;	gMoneyStats[3].sY = 51;		gMoneyStats[3].sValDx = 78;
			gMoneyStats[4].sX = 0;		gMoneyStats[4].sY = 0;		gMoneyStats[4].sValDx = 0;
			gMoneyStats[5].sX = 0;		gMoneyStats[5].sY = 0;		gMoneyStats[5].sValDx = 0;

			gItemDescProsConsRects[0].iLeft = 7;	gItemDescProsConsRects[0].iTop = 111;	gItemDescProsConsRects[0].iRight = 313;	gItemDescProsConsRects[0].iBottom = 118;
			gItemDescProsConsRects[1].iLeft = 7;	gItemDescProsConsRects[1].iTop = 119;	gItemDescProsConsRects[1].iRight = 313;	gItemDescProsConsRects[1].iBottom = 126;
		}
	}
}

void InitLBECoords()
{
	if(guiCurrentScreen ==MAP_SCREEN)	//Strategic
	{
		if(UsingEDBSystem() > 0)
		{
			gLBEStats[0].sX = 65;	gLBEStats[0].sY = 367;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 101;	gLBEStats[1].sY = 367;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 65;	gLBEStats[2].sY = 391;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 101;	gLBEStats[3].sY = 391;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 77;	gLBEStats[4].sY = 415;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 22;	gLBEStats[12].sY = 417;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 58;	gLBEStats[13].sY = 417;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 94;	gLBEStats[14].sY = 417;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 130;	gLBEStats[15].sY = 417;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 166;	gLBEStats[16].sY = 417;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 22;	gLBEStats[17].sY = 441;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 58;	gLBEStats[18].sY = 441;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 94;	gLBEStats[19].sY = 441;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 130;	gLBEStats[20].sY = 441;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 166;	gLBEStats[21].sY = 441;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 202;	gLBEStats[22].sY = 417;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 202;	gLBEStats[23].sY = 441;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 66;	gLBEStats[24].sY = 341;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 102;	gLBEStats[25].sY = 341;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 66;	gLBEStats[26].sY = 365;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 102;	gLBEStats[27].sY = 365;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 69;	gLBEStats[28].sY = 389;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 69;	gLBEStats[29].sY = 413;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 69;	gLBEStats[30].sY = 437;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 32;	gLBEStats[36].sY = 347;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 32;	gLBEStats[37].sY = 371;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 32;	gLBEStats[38].sY = 395;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 32;	gLBEStats[39].sY = 419;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 68;	gLBEStats[40].sY = 347;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 68;	gLBEStats[41].sY = 371;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 68;	gLBEStats[42].sY = 395;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 68;	gLBEStats[43].sY = 419;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 104;	gLBEStats[44].sY = 347;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 104;	gLBEStats[45].sY = 371;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 104;	gLBEStats[46].sY = 395;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 104;	gLBEStats[47].sY = 419;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
		else
		{
			gLBEStats[0].sX = 101;	gLBEStats[0].sY = 287;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 137;	gLBEStats[1].sY = 287;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 101;	gLBEStats[2].sY = 311;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 137;	gLBEStats[3].sY = 311;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 112;	gLBEStats[4].sY = 335;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 22;	gLBEStats[12].sY = 299;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 58;	gLBEStats[13].sY = 299;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 94;	gLBEStats[14].sY = 299;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 130;	gLBEStats[15].sY = 299;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 166;	gLBEStats[16].sY = 299;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 22;	gLBEStats[17].sY = 323;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 58;	gLBEStats[18].sY = 323;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 94;	gLBEStats[19].sY = 323;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 130;	gLBEStats[20].sY = 323;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 166;	gLBEStats[21].sY = 323;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 202;	gLBEStats[22].sY = 299;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 202;	gLBEStats[23].sY = 323;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 101;	gLBEStats[24].sY = 287;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 137;	gLBEStats[25].sY = 287;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 101;	gLBEStats[26].sY = 311;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 137;	gLBEStats[27].sY = 311;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 104;	gLBEStats[28].sY = 335;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 104;	gLBEStats[29].sY = 359;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 104;	gLBEStats[30].sY = 383;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 68;	gLBEStats[36].sY = 275;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 68;	gLBEStats[37].sY = 299;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 68;	gLBEStats[38].sY = 323;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 68;	gLBEStats[39].sY = 347;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 104;	gLBEStats[40].sY = 275;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 104;	gLBEStats[41].sY = 299;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 104;	gLBEStats[42].sY = 323;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 104;	gLBEStats[43].sY = 347;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 140;	gLBEStats[44].sY = 275;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 140;	gLBEStats[45].sY = 299;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 140;	gLBEStats[46].sY = 323;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 140;	gLBEStats[47].sY = 347;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
	}
	else
	{
		if(UsingEDBSystem() > 0)
		{
			gLBEStats[0].sX = 456;	gLBEStats[0].sY = 10;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 492;	gLBEStats[1].sY = 10;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 456;	gLBEStats[2].sY = 34;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 492;	gLBEStats[3].sY = 34;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 467;	gLBEStats[4].sY = 58;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 456;	gLBEStats[12].sY = 10;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 492;	gLBEStats[13].sY = 10;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 528;	gLBEStats[14].sY = 10;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 564;	gLBEStats[15].sY = 10;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 600;	gLBEStats[16].sY = 10;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 456;	gLBEStats[17].sY = 34;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 492;	gLBEStats[18].sY = 34;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 528;	gLBEStats[19].sY = 34;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 564;	gLBEStats[20].sY = 34;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 600;	gLBEStats[21].sY = 34;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 636;	gLBEStats[22].sY = 10;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 636;	gLBEStats[23].sY = 34;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 456;	gLBEStats[24].sY = 10;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 492;	gLBEStats[25].sY = 10;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 456;	gLBEStats[26].sY = 34;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 492;	gLBEStats[27].sY = 34;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 528;	gLBEStats[28].sY = 10;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 528;	gLBEStats[29].sY = 34;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 528;	gLBEStats[30].sY = 58;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 456;	gLBEStats[36].sY = 10;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 456;	gLBEStats[37].sY = 34;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 456;	gLBEStats[38].sY = 58;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 456;	gLBEStats[39].sY = 82;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 492;	gLBEStats[40].sY = 10;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 492;	gLBEStats[41].sY = 34;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 492;	gLBEStats[42].sY = 58;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 492;	gLBEStats[43].sY = 82;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 528;	gLBEStats[44].sY = 10;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 528;	gLBEStats[45].sY = 34;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 528;	gLBEStats[46].sY = 58;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 528;	gLBEStats[47].sY = 82;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
		else
		{
			gLBEStats[0].sX = 586;	gLBEStats[0].sY = 10;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 622;	gLBEStats[1].sY = 10;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 586;	gLBEStats[2].sY = 34;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 622;	gLBEStats[3].sY = 34;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 597;	gLBEStats[4].sY = 58;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 269;	gLBEStats[12].sY = 139;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 305;	gLBEStats[13].sY = 139;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 341;	gLBEStats[14].sY = 139;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 377;	gLBEStats[15].sY = 139;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 413;	gLBEStats[16].sY = 139;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 269;	gLBEStats[17].sY = 163;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 305;	gLBEStats[18].sY = 163;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 341;	gLBEStats[19].sY = 163;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 377;	gLBEStats[20].sY = 163;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 413;	gLBEStats[21].sY = 163;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 449;	gLBEStats[22].sY = 139;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 449;	gLBEStats[23].sY = 163;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 585;	gLBEStats[24].sY = 10;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 621;	gLBEStats[25].sY = 10;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 585;	gLBEStats[26].sY = 34;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 621;	gLBEStats[27].sY = 34;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 657;	gLBEStats[28].sY = 10;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 657;	gLBEStats[29].sY = 34;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 657;	gLBEStats[30].sY = 58;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 585;	gLBEStats[36].sY = 10;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 585;	gLBEStats[37].sY = 34;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 585;	gLBEStats[38].sY = 58;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 585;	gLBEStats[39].sY = 82;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 621;	gLBEStats[40].sY = 10;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 621;	gLBEStats[41].sY = 34;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 621;	gLBEStats[42].sY = 58;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 621;	gLBEStats[43].sY = 82;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 657;	gLBEStats[44].sY = 10;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 657;	gLBEStats[45].sY = 34;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 657;	gLBEStats[46].sY = 58;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 657;	gLBEStats[47].sY = 82;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
	}
}

void InitItemDescriptionBoxOffsets()
{
	// Initiate variables for Enhanced Item Description Box system
	if(guiCurrentScreen ==MAP_SCREEN)	//Strategic
	{
		if(UsingEDBSystem() > 0)
		{
			// STRATEGIC Description Box Icons
			BULLET_DRAW_X				= (112 + gsInvDescX);
			BULLET_DRAW_Y				= (136 + gsInvDescY);
			BULLET_SING_X				= (112 + gsInvDescX);
			BULLET_SING_Y				= (148 + gsInvDescY);
			BULLET_BURST_X			= (112 + gsInvDescX);
			BULLET_BURST_Y			= (160 + gsInvDescY);
			BULLET_AUTO_X			= (112 + gsInvDescX);
			BULLET_AUTO_Y			= (172 + gsInvDescY);
			BULLET_RELOAD_X			= (112 + gsInvDescX);
			BULLET_RELOAD_Y			= (184 + gsInvDescY);
			BULLET_MRELOAD_X			= (112 + gsInvDescX);
			BULLET_MRELOAD_Y			= (196 + gsInvDescY);
			BULLET_RANGE_X			= (47 + gsInvDescX);
			BULLET_RANGE_Y			= (124 + gsInvDescY);
			BULLET_DAMAGE_X			= (47 + gsInvDescX);
			BULLET_DAMAGE_Y			= (136 + gsInvDescY);
			BULLET_AUTOPEN_X			= (47 + gsInvDescX);
			BULLET_AUTOPEN_Y			= (148 + gsInvDescY);
			BULLET_AUTO5AP_X			= (47 + gsInvDescX);
			BULLET_AUTO5AP_Y			= (160 + gsInvDescY);
			BULLET_ACC_X			= (47 + gsInvDescX);
			BULLET_ACC_Y			= (172 + gsInvDescY);
			BULLET_RELIABILITY_X			= (47 + gsInvDescX);
			BULLET_RELIABILITY_Y			= (184 + gsInvDescY);
			BULLET_REPAIR_X			= (47 + gsInvDescX);
			BULLET_REPAIR_Y			= (196 + gsInvDescY);
			BULLET_LOUDNESS_X		= (174 + gsInvDescX);
			BULLET_LOUDNESS_Y		= (136 + gsInvDescY);
			BULLET_FLASH_X			= (198 + gsInvDescX);
			BULLET_FLASH_Y			= (124 + gsInvDescY);
			BULLET_TOHIT_X			= (174 + gsInvDescX);
			BULLET_TOHIT_Y			= (148 + gsInvDescY);
			BULLET_BESTLASERRANGE_X			= (174 + gsInvDescX);
			BULLET_BESTLASERRANGE_Y			= (160 + gsInvDescY);
			BULLET_AIMBONUS_X			= (174 + gsInvDescX);
			BULLET_AIMBONUS_Y			= (172 + gsInvDescY);
			BULLET_MINRANGEAIMBONUS_X	=	(174 + gsInvDescX);
			BULLET_MINRANGEAIMBONUS_Y	=	(184 + gsInvDescY);
			BULLET_BIPOD_X			= (174 + gsInvDescX);
			BULLET_BIPOD_Y			= (196 + gsInvDescY);

			// Icon Locations for AMMO stats
			BULLET_ARMORPEN_X		= (47 + gsInvDescX);
			BULLET_ARMORPEN_Y		= (124 + gsInvDescY);
			BULLET_TUMBLE_X		= (47 + gsInvDescX);
			BULLET_TUMBLE_Y		= (136 + gsInvDescY);
			BULLET_PREARMOR_X		= (47 + gsInvDescX);
			BULLET_PREARMOR_Y		= (148 + gsInvDescY);
			BULLET_TRACER_X		= (115 + gsInvDescX);
			BULLET_TRACER_Y		= (124 + gsInvDescY);
			BULLET_ANTITANK_X		= (140 + gsInvDescX);
			BULLET_ANTITANK_Y		= (124 + gsInvDescY);
			BULLET_LOCKBUSTER_X		= (115 + gsInvDescX);
			BULLET_LOCKBUSTER_Y		= (148 + gsInvDescY);
			BULLET_IGNOREARMOR_X		= (140 + gsInvDescX);
			BULLET_IGNOREARMOR_Y		= (148 + gsInvDescY);
			BULLET_ACIDIC_X		= (115 + gsInvDescX);
			BULLET_ACIDIC_Y		= (172 + gsInvDescY);
			BULLET_AMMORANGEBON_X		= (174 + gsInvDescX);
			BULLET_AMMORANGEBON_Y		= (124 + gsInvDescY);
			BULLET_AMMODAMAGEBON_X		= (174 + gsInvDescX);
			BULLET_AMMODAMAGEBON_Y		= (136 + gsInvDescY);
			BULLET_AMMOTOHITBON_X		= (174 + gsInvDescX);
			BULLET_AMMOTOHITBON_Y		= (148 + gsInvDescY);
			BULLET_AMMOBURSTBON_X		= (174 + gsInvDescX);
			BULLET_AMMOBURSTBON_Y		= (160 + gsInvDescY);
			BULLET_AMMORELIABILITYBON_X		= (174 + gsInvDescX);
			BULLET_AMMORELIABILITYBON_Y		= (172 + gsInvDescY);
			BULLET_AMMONOISESUPP_X		= (174 + gsInvDescX);
			BULLET_AMMONOISESUPP_Y		= (184 + gsInvDescY);

			// Icon Locations for EXPLOSIVES stats
			BULLET_EXPLOSIVEDAMAGE_X	= (47 + gsInvDescX);
			BULLET_EXPLOSIVEDAMAGE_Y	= (124 + gsInvDescY);
			BULLET_EXPLOSIVESTUNDAMAGE_X	= (47 + gsInvDescX);
			BULLET_EXPLOSIVESTUNDAMAGE_Y	= (136 + gsInvDescY);
			BULLET_EXPLOSIVELOUDNESS_X	= (47 + gsInvDescX);
			BULLET_EXPLOSIVELOUDNESS_Y	= (148 + gsInvDescY);
			BULLET_EXPLOSIVEVOLATILITY_X	= (47 + gsInvDescX);
			BULLET_EXPLOSIVEVOLATILITY_Y	= (160 + gsInvDescY);
			BULLET_EXPLOSIVERADIUS_X	= (131 + gsInvDescX);
			BULLET_EXPLOSIVERADIUS_Y	= (124 + gsInvDescY);
			BULLET_EXPLOSIVESTARTRADIUS_X	= (113 + gsInvDescX);
			BULLET_EXPLOSIVESTARTRADIUS_Y	= (124 + gsInvDescY);
			BULLET_EXPLOSIVEARROW_X	= (131 + gsInvDescX);
			BULLET_EXPLOSIVEARROW_Y	= (130 + gsInvDescY);
			BULLET_EXPLOSIVEENDRADIUS_X	= (146 + gsInvDescX);
			BULLET_EXPLOSIVEENDRADIUS_Y	= (124 + gsInvDescY);
			BULLET_EXPLOSIVEDURATION_X	= (112 + gsInvDescX);
			BULLET_EXPLOSIVEDURATION_Y	= (172 + gsInvDescY);

			// Icon Locations for ARMOR stats
			BULLET_ARMOR_X	= (47 + gsInvDescX);
			BULLET_ARMOR_Y	= (124 + gsInvDescY);
			BULLET_ARMORCOVERAGE_X	= (47 + gsInvDescX);
			BULLET_ARMORCOVERAGE_Y	= (136 + gsInvDescY);
			BULLET_ARMORDEGRADE_X	= (47 + gsInvDescX);
			BULLET_ARMORDEGRADE_Y	= (148 + gsInvDescY);
			BULLET_ARMORAPBONUS_X	= (112 + gsInvDescX);
			BULLET_ARMORAPBONUS_Y	= (124 + gsInvDescY);
			BULLET_ARMORAIMBONUS_X	= (112 + gsInvDescX);
			BULLET_ARMORAIMBONUS_Y	= (136 + gsInvDescY);
			BULLET_ARMORWOODCAMO_X	= (112 + gsInvDescX);
			BULLET_ARMORWOODCAMO_Y	= (148 + gsInvDescY);
			BULLET_ARMORURBANCAMO_X	= (112 + gsInvDescX);
			BULLET_ARMORURBANCAMO_Y	= (160 + gsInvDescY);
			BULLET_ARMORDESERTCAMO_X	= (112 + gsInvDescX);
			BULLET_ARMORDESERTCAMO_Y	= (172 + gsInvDescY);
			BULLET_ARMORSNOWCAMO_X	= (112 + gsInvDescX);
			BULLET_ARMORSNOWCAMO_Y	= (184 + gsInvDescY);
			BULLET_ARMORSTEALTH_X	= (112 + gsInvDescX);
			BULLET_ARMORSTEALTH_Y	= (196 + gsInvDescY);
			BULLET_ARMORVISIONRANGEBONUS_X	= (174 + gsInvDescX);
			BULLET_ARMORVISIONRANGEBONUS_Y	= (124 + gsInvDescY);
			BULLET_ARMORDAYVISIONRANGEBONUS_X	= (175 + gsInvDescX);
			BULLET_ARMORDAYVISIONRANGEBONUS_Y	= (136 + gsInvDescY);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_X	= (175 + gsInvDescX);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_Y	= (148 + gsInvDescY);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_X	= (175 + gsInvDescX);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_Y	= (160 + gsInvDescY);
			BULLET_ARMORCAVEVISIONRANGEBONUS_X	= (175 + gsInvDescX);
			BULLET_ARMORCAVEVISIONRANGEBONUS_Y	= (172 + gsInvDescY);
			BULLET_ARMORPERCENTTUNNELVISION_X	= (175 + gsInvDescX);
			BULLET_ARMORPERCENTTUNNELVISION_Y	= (184 + gsInvDescY);
			BULLET_ARMORHEARINGRANGEBONUS_X	= (174 + gsInvDescX);
			BULLET_ARMORHEARINGRANGEBONUS_Y	= (196 + gsInvDescY);

			// Generic Icon Locations for MISC Item Stats
			BULLET_COL1ROW1_X = (47 + gsInvDescX);
			BULLET_COL1ROW1_Y = (124 + gsInvDescY);
			BULLET_COL1ROW2_X = (47 + gsInvDescX);
			BULLET_COL1ROW2_Y = (136 + gsInvDescY);
			BULLET_COL1ROW3_X = (47 + gsInvDescX);
			BULLET_COL1ROW3_Y = (148 + gsInvDescY);
			BULLET_COL1ROW4_X = (47 + gsInvDescX);
			BULLET_COL1ROW4_Y = (160 + gsInvDescY);
			BULLET_COL1ROW5_X = (47 + gsInvDescX);
			BULLET_COL1ROW5_Y = (172 + gsInvDescY);
			BULLET_COL1ROW6_X = (47 + gsInvDescX);
			BULLET_COL1ROW6_Y = (184 + gsInvDescY);
			BULLET_COL1ROW7_X = (47 + gsInvDescX);
			BULLET_COL1ROW7_Y = (196 + gsInvDescY);
			BULLET_COL2ROW1_X = (144 + gsInvDescX);
			BULLET_COL2ROW1_Y = (124 + gsInvDescY);
			BULLET_COL2ROW2_X = (144 + gsInvDescX);
			BULLET_COL2ROW2_Y = (136 + gsInvDescY);
			BULLET_COL2ROW3_X = (144 + gsInvDescX);
			BULLET_COL2ROW3_Y = (148 + gsInvDescY);
			BULLET_COL2ROW4_X = (144 + gsInvDescX);
			BULLET_COL2ROW4_Y = (160 + gsInvDescY);
			BULLET_COL2ROW5_X = (144 + gsInvDescX);
			BULLET_COL2ROW5_Y = (172 + gsInvDescY);
			BULLET_COL2ROW6_X = (144 + gsInvDescX);
			BULLET_COL2ROW6_Y = (184 + gsInvDescY);
			BULLET_COL2ROW7_X = (144 + gsInvDescX);
			BULLET_COL2ROW7_Y = (196 + gsInvDescY);

			ITEMDESC_CALIBER_X		= (105 + gsInvDescX);
			ITEMDESC_CALIBER_Y		= ( 66 + gsInvDescY);
			ITEMDESC_CALIBER_WIDTH	= 149;
			ITEMDESC_NAME_X			= (  7 + gsInvDescX);
			ITEMDESC_NAME_Y			= ( 65 + gsInvDescY);
			ITEMDESC_ITEM_X			= ( 25 + gsInvDescX);
			ITEMDESC_ITEM_Y			= (  6 + gsInvDescY);
			// END ICONS SEGMENT
			ITEMDESC_DESC_START_X	= ( 23 + gsInvDescX );
			ITEMDESC_DESC_START_Y	= (230 + gsInvDescY);
			ITEMDESC_PROS_START_X	= ( 23 + gsInvDescX );
			ITEMDESC_PROS_START_Y	= (290 + gsInvDescY);
			ITEMDESC_CONS_START_X	= ( 23 + gsInvDescX );
			ITEMDESC_CONS_START_Y	= (300 + gsInvDescY);
			ITEMDESC_DESC_WIDTH		= 220;
			ITEMDESC_ITEM_STATUS_X	= ( 18 + gsInvDescX );
			ITEMDESC_ITEM_STATUS_Y	= ( 53 + gsInvDescY );
			ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
		}
		else
		{
			// STRATEGIC Description Box Icon locations
			BULLET_SING_X			= ( 77 + gsInvDescX);
			BULLET_SING_Y			= (135 + gsInvDescY);
			BULLET_BURST_X			= (117 + gsInvDescX);
			BULLET_BURST_Y			= (135 + gsInvDescY);
			ITEMDESC_CALIBER_X		= (105 + gsInvDescX);
			ITEMDESC_CALIBER_Y		= ( 66 + gsInvDescY);
			ITEMDESC_CALIBER_WIDTH	= 149;
			ITEMDESC_NAME_X			= (  7 + gsInvDescX);
			ITEMDESC_NAME_Y			= ( 65 + gsInvDescY);
			ITEMDESC_ITEM_X			= ( 25 + gsInvDescX);
			ITEMDESC_ITEM_Y			= (  6 + gsInvDescY);
			// END ICONS SEGMENT
			ITEMDESC_DESC_START_X	= ( 23 + gsInvDescX ); // Description Text Box, Strategic
			ITEMDESC_DESC_START_Y	= (170 + gsInvDescY);
			ITEMDESC_PROS_START_X	= ( 23 + gsInvDescX ); // Pros, Strategic
			ITEMDESC_PROS_START_Y	= (230 + gsInvDescY);
			ITEMDESC_CONS_START_X	= ( 23 + gsInvDescX ); // Cons, Strategic
			ITEMDESC_CONS_START_Y	= (240 + gsInvDescY);
			ITEMDESC_DESC_WIDTH		= 220;
			ITEMDESC_ITEM_STATUS_X	= ( 18 + gsInvDescX );
			ITEMDESC_ITEM_STATUS_Y	= ( 53 + gsInvDescY );
			ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
		}
	}
	else	//Tactical
	{
		if(UsingEDBSystem() == 1) // NIV EDB
		{
			// Icon locations, Tactical
			BULLET_DRAW_X	=			(268 + gsInvDescX);
			BULLET_DRAW_Y	=			(56 + gsInvDescY);
			BULLET_SING_X	=			(268 + gsInvDescX);
			BULLET_SING_Y	=			(68 + gsInvDescY);
			BULLET_BURST_X	=		(268 + gsInvDescX);
			BULLET_BURST_Y	=		(80 + gsInvDescY);
			BULLET_AUTO_X	=		(268 + gsInvDescX);
			BULLET_AUTO_Y	=		(92 + gsInvDescY);
			BULLET_RELOAD_X	=		(268 + gsInvDescX);
			BULLET_RELOAD_Y	=		(104 + gsInvDescY);
			BULLET_MRELOAD_X	=		(268 + gsInvDescX);
			BULLET_MRELOAD_Y	=		(116 + gsInvDescY);
			BULLET_RANGE_X	=		(203 + gsInvDescX);
			BULLET_RANGE_Y	=		(44 + gsInvDescY);
			BULLET_DAMAGE_X		=	(203 + gsInvDescX);
			BULLET_DAMAGE_Y		=	(56 + gsInvDescY);
			BULLET_AUTOPEN_X	=		(203 + gsInvDescX);
			BULLET_AUTOPEN_Y	=		(68 + gsInvDescY);
			BULLET_AUTO5AP_X	=		(203 + gsInvDescX);
			BULLET_AUTO5AP_Y	=		(80 + gsInvDescY);
			BULLET_ACC_X		=	(203 + gsInvDescX);
			BULLET_ACC_Y		=	(92 + gsInvDescY);
			BULLET_RELIABILITY_X	=		(203 + gsInvDescX);
			BULLET_RELIABILITY_Y	=		(104 + gsInvDescY);
			BULLET_REPAIR_X		=	(203 + gsInvDescX);
			BULLET_REPAIR_Y		=	(116 + gsInvDescY);
			BULLET_FLASH_X		=	(226 + gsInvDescX);	//291
			BULLET_FLASH_Y		=	(136 + gsInvDescY);	//136
			BULLET_LOUDNESS_X	=	(203 + gsInvDescX);	//268
			BULLET_LOUDNESS_Y	=	(160 + gsInvDescY);	//159
			BULLET_TOHIT_X		=	(268 + gsInvDescX);	//203
			BULLET_TOHIT_Y		=	(136 + gsInvDescY);	//136
			BULLET_BESTLASERRANGE_X		=	(268 + gsInvDescX);	//203
			BULLET_BESTLASERRANGE_Y		=	(148 + gsInvDescY);	//148
			BULLET_AIMBONUS_X	=		(268 + gsInvDescX);	//203
			BULLET_AIMBONUS_Y	=		(160 + gsInvDescY);	//160
			BULLET_MINRANGEAIMBONUS_X	=	(268 + gsInvDescX);	//203
			BULLET_MINRANGEAIMBONUS_Y	=	(172 + gsInvDescY);	//172
			BULLET_BIPOD_X	=		(203 + gsInvDescX);	//268
			BULLET_BIPOD_Y	=		(172 + gsInvDescY);	//171

			// Icon Locations for AMMO stats
			BULLET_ARMORPEN_X	=	(203 + gsInvDescX);
			BULLET_ARMORPEN_Y	=	(44 + gsInvDescY);
			BULLET_TUMBLE_X	=	(203 + gsInvDescX);
			BULLET_TUMBLE_Y	=	(56 + gsInvDescY);
			BULLET_PREARMOR_X	=	(203 + gsInvDescX);
			BULLET_PREARMOR_Y	=	(68 + gsInvDescY);
			BULLET_TRACER_X	=	(271 + gsInvDescX);
			BULLET_TRACER_Y	=	(44 + gsInvDescY);
			BULLET_ANTITANK_X	=	(296 + gsInvDescX);
			BULLET_ANTITANK_Y	=	(44 + gsInvDescY);
			BULLET_LOCKBUSTER_X	=	(271 + gsInvDescX);
			BULLET_LOCKBUSTER_Y	=	(68 + gsInvDescY);
			BULLET_IGNOREARMOR_X	=	(296 + gsInvDescX);
			BULLET_IGNOREARMOR_Y	=	(68 + gsInvDescY);
			BULLET_ACIDIC_X	=	(271 + gsInvDescX);
			BULLET_ACIDIC_Y	=	(92 + gsInvDescY);
			BULLET_AMMORANGEBON_X	=	(203 + gsInvDescX);
			BULLET_AMMORANGEBON_Y	=	(136 + gsInvDescY);
			BULLET_AMMODAMAGEBON_X	=	(203 + gsInvDescX);
			BULLET_AMMODAMAGEBON_Y	=	(148 + gsInvDescY);
			BULLET_AMMOTOHITBON_X	=	(203 + gsInvDescX);
			BULLET_AMMOTOHITBON_Y	=	(160 + gsInvDescY);
			BULLET_AMMOBURSTBON_X	=	(203 + gsInvDescX);
			BULLET_AMMOBURSTBON_Y	=	(172 + gsInvDescY);
			BULLET_AMMORELIABILITYBON_X	=	(268 + gsInvDescX);
			BULLET_AMMORELIABILITYBON_Y	=	(136 + gsInvDescY);
			BULLET_AMMONOISESUPP_X	=	(268 + gsInvDescX);
			BULLET_AMMONOISESUPP_Y	=	(148 + gsInvDescY);

			// Icon Locations for EXPLOSIVES stats
			BULLET_EXPLOSIVEDAMAGE_X	=	(203 + gsInvDescX);
			BULLET_EXPLOSIVEDAMAGE_Y	=	(44 + gsInvDescY);
			BULLET_EXPLOSIVESTUNDAMAGE_X	=	(203 + gsInvDescX);
			BULLET_EXPLOSIVESTUNDAMAGE_Y	=	(56 + gsInvDescY);
			BULLET_EXPLOSIVELOUDNESS_X	=	(203 + gsInvDescX);
			BULLET_EXPLOSIVELOUDNESS_Y	=	(68 + gsInvDescY);
			BULLET_EXPLOSIVEVOLATILITY_X	=	(203 + gsInvDescX);
			BULLET_EXPLOSIVEVOLATILITY_Y	=	(80 + gsInvDescY);
			BULLET_EXPLOSIVERADIUS_X	=	(288 + gsInvDescX);
			BULLET_EXPLOSIVERADIUS_Y	=	(44 + gsInvDescY);
			BULLET_EXPLOSIVESTARTRADIUS_X	=	(270 + gsInvDescX);
			BULLET_EXPLOSIVESTARTRADIUS_Y	=	(44 + gsInvDescY);
			BULLET_EXPLOSIVEARROW_X	=	(287 + gsInvDescX);
			BULLET_EXPLOSIVEARROW_Y	=	(50 + gsInvDescY);
			BULLET_EXPLOSIVEENDRADIUS_X	=	(303 + gsInvDescX);
			BULLET_EXPLOSIVEENDRADIUS_Y	=	(44 + gsInvDescY);
			BULLET_EXPLOSIVEDURATION_X	=	(268 + gsInvDescX);
			BULLET_EXPLOSIVEDURATION_Y	=	(92 + gsInvDescY);

			// Icon Locations for ARMOR stats
			BULLET_ARMOR_X	=	(203 + gsInvDescX);
			BULLET_ARMOR_Y	=	(44 + gsInvDescY);
			BULLET_ARMORCOVERAGE_X	=	(203 + gsInvDescX);
			BULLET_ARMORCOVERAGE_Y	=	(56 + gsInvDescY);
			BULLET_ARMORDEGRADE_X	=	(203 + gsInvDescX);
			BULLET_ARMORDEGRADE_Y	=	(68 + gsInvDescY);
			BULLET_ARMORAPBONUS_X	=	(268 + gsInvDescX);
			BULLET_ARMORAPBONUS_Y	=	(44 + gsInvDescY);
			BULLET_ARMORAIMBONUS_X	=	(268 + gsInvDescX);
			BULLET_ARMORAIMBONUS_Y	=	(56 + gsInvDescY);
			BULLET_ARMORWOODCAMO_X	=	(268 + gsInvDescX);
			BULLET_ARMORWOODCAMO_Y	=	(68 + gsInvDescY);
			BULLET_ARMORURBANCAMO_X	=	(268 + gsInvDescX);
			BULLET_ARMORURBANCAMO_Y	=	(80 + gsInvDescY);
			BULLET_ARMORDESERTCAMO_X	=	(268 + gsInvDescX);
			BULLET_ARMORDESERTCAMO_Y	=	(92 + gsInvDescY);
			BULLET_ARMORSNOWCAMO_X	=	(268 + gsInvDescX);
			BULLET_ARMORSNOWCAMO_Y	=	(104 + gsInvDescY);
			BULLET_ARMORSTEALTH_X	=	(268 + gsInvDescX);
			BULLET_ARMORSTEALTH_Y	=	(116 + gsInvDescY);
			BULLET_ARMORVISIONRANGEBONUS_X	=	(203 + gsInvDescX);
			BULLET_ARMORVISIONRANGEBONUS_Y	=	(136 + gsInvDescY);
			BULLET_ARMORDAYVISIONRANGEBONUS_X	=	(203 + gsInvDescX);
			BULLET_ARMORDAYVISIONRANGEBONUS_Y	=	(148 + gsInvDescY);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_X	=	(203 + gsInvDescX);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_Y	=	(160 + gsInvDescY);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_X	=	(203 + gsInvDescX);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_Y	=	(172 + gsInvDescY);
			BULLET_ARMORCAVEVISIONRANGEBONUS_X	=	(268 + gsInvDescX);
			BULLET_ARMORCAVEVISIONRANGEBONUS_Y	=	(136 + gsInvDescY);
			BULLET_ARMORPERCENTTUNNELVISION_X	=	(268 + gsInvDescX);
			BULLET_ARMORPERCENTTUNNELVISION_Y	=	(148 + gsInvDescY);
			BULLET_ARMORHEARINGRANGEBONUS_X	=	(268 + gsInvDescX);
			BULLET_ARMORHEARINGRANGEBONUS_Y	=	(160 + gsInvDescY);

			// Generic Icon Locations for MISC Item Stats
			BULLET_COL1ROW1_X =	(203 + gsInvDescX);
			BULLET_COL1ROW1_Y =	(44 + gsInvDescY);
			BULLET_COL1ROW2_X =	(203 + gsInvDescX);
			BULLET_COL1ROW2_Y =	(56 + gsInvDescY);
			BULLET_COL1ROW3_X =	(203 + gsInvDescX);
			BULLET_COL1ROW3_Y =	(68 + gsInvDescY);
			BULLET_COL1ROW4_X =	(203 + gsInvDescX);
			BULLET_COL1ROW4_Y =	(80 + gsInvDescY);
			BULLET_COL1ROW5_X =	(203 + gsInvDescX);
			BULLET_COL1ROW5_Y =	(92 + gsInvDescY);
			BULLET_COL1ROW6_X =	(203 + gsInvDescX);
			BULLET_COL1ROW6_Y =	(104 + gsInvDescY);
			BULLET_COL1ROW7_X =	(203 + gsInvDescX);
			BULLET_COL1ROW7_Y =	(116 + gsInvDescY);
			BULLET_COL1ROW8_X =	(203 + gsInvDescX);
			BULLET_COL1ROW8_Y =	(128 + gsInvDescY);
			BULLET_COL1ROW9_X =	(203 + gsInvDescX);
			BULLET_COL1ROW9_Y =	(140 + gsInvDescY);
			BULLET_COL1ROW10_X =	(203 + gsInvDescX);
			BULLET_COL1ROW10_Y =	(152 + gsInvDescY);
			BULLET_COL1ROW11_X =	(203 + gsInvDescX);
			BULLET_COL1ROW11_Y =	(164 + gsInvDescY);
			BULLET_COL1ROW12_X =	(203 + gsInvDescX);
			BULLET_COL1ROW12_Y =	(176 + gsInvDescY);
			ITEMDESC_CALIBER_X		= ( 50 + gsInvDescX);
			ITEMDESC_CALIBER_Y		= ( 78 + gsInvDescY);
			ITEMDESC_CALIBER_WIDTH	= 142;
			ITEMDESC_NAME_X			= ( 16 + gsInvDescX);
			ITEMDESC_NAME_Y			= ( 67 + gsInvDescY);
			ITEMDESC_ITEM_X			= (  8 + gsInvDescX);
			ITEMDESC_ITEM_Y			= ( 11 + gsInvDescY);
			ITEMDESC_DESC_START_X	= ( 11 + gsInvDescX);
			ITEMDESC_DESC_START_Y	= ( 94 + gsInvDescY);
			ITEMDESC_PROS_START_X	= ( 11 + gsInvDescX );
			ITEMDESC_PROS_START_Y	= (168 + gsInvDescY);
			ITEMDESC_CONS_START_X	= ( 11 + gsInvDescX );
			ITEMDESC_CONS_START_Y	= (178 + gsInvDescY);
			ITEMDESC_DESC_WIDTH		=	180;
			ITEMDESC_ITEM_STATUS_X	= (  6 + gsInvDescX );
			ITEMDESC_ITEM_STATUS_Y	= ( 60 + gsInvDescY );
			ITEMDESC_ITEM_STATUS_HEIGHT	= 50;
		}
		else if(UsingEDBSystem() == 2) // OIV EDB
		{
			// Icon locations, Tactical
			BULLET_DRAW_X	=			(341 + gsInvDescX);
			BULLET_DRAW_Y	=			(26 + gsInvDescY);
			BULLET_SING_X	=			(341 + gsInvDescX);
			BULLET_SING_Y	=			(38 + gsInvDescY);
			BULLET_BURST_X	=		(341 + gsInvDescX);
			BULLET_BURST_Y	=		(50 + gsInvDescY);
			BULLET_AUTO_X	=		(341 + gsInvDescX);
			BULLET_AUTO_Y	=		(62 + gsInvDescY);
			BULLET_RELOAD_X	=		(341 + gsInvDescX);
			BULLET_RELOAD_Y	=		(74 + gsInvDescY);
			BULLET_MRELOAD_X	=		(341 + gsInvDescX);
			BULLET_MRELOAD_Y	=		(86 + gsInvDescY);
			BULLET_RANGE_X	=		(274 + gsInvDescX);
			BULLET_RANGE_Y	=		(14 + gsInvDescY);
			BULLET_DAMAGE_X		=	(274 + gsInvDescX);
			BULLET_DAMAGE_Y		=	(26 + gsInvDescY);
			BULLET_AUTOPEN_X	=		(274 + gsInvDescX);
			BULLET_AUTOPEN_Y	=		(38 + gsInvDescY);
			BULLET_AUTO5AP_X	=		(274 + gsInvDescX);
			BULLET_AUTO5AP_Y	=		(50 + gsInvDescY);
			BULLET_ACC_X		=	(274 + gsInvDescX);
			BULLET_ACC_Y		=	(62 + gsInvDescY);
			BULLET_RELIABILITY_X	=		(274 + gsInvDescX);
			BULLET_RELIABILITY_Y	=		(74 + gsInvDescY);
			BULLET_REPAIR_X		=	(274 + gsInvDescX);
			BULLET_REPAIR_Y		=	(86 + gsInvDescY);
			BULLET_FLASH_X		=	(431 + gsInvDescX);	//291
			BULLET_FLASH_Y		=	(14 + gsInvDescY);	//136
			BULLET_LOUDNESS_X	=	(408 + gsInvDescX);	//268
			BULLET_LOUDNESS_Y	=	(26 + gsInvDescY);	//159
			BULLET_TOHIT_X		=	(408 + gsInvDescX);	//203
			BULLET_TOHIT_Y		=	(38 + gsInvDescY);	//136
			BULLET_BESTLASERRANGE_X		=	(408 + gsInvDescX);	//203
			BULLET_BESTLASERRANGE_Y		=	(50 + gsInvDescY);	//148
			BULLET_AIMBONUS_X	=		(408 + gsInvDescX);	//203
			BULLET_AIMBONUS_Y	=		(62 + gsInvDescY);	//160
			BULLET_MINRANGEAIMBONUS_X	=	(408 + gsInvDescX);	//203
			BULLET_MINRANGEAIMBONUS_Y	=	(74 + gsInvDescY);	//172
			BULLET_BIPOD_X	=		(408 + gsInvDescX);	//268
			BULLET_BIPOD_Y	=		(86 + gsInvDescY);	//171

			// Icon Locations for AMMO stats
			BULLET_ARMORPEN_X	=	(274 + gsInvDescX);
			BULLET_ARMORPEN_Y	=	(14 + gsInvDescY);
			BULLET_TUMBLE_X	=	(274 + gsInvDescX);
			BULLET_TUMBLE_Y	=	(26 + gsInvDescY);
			BULLET_PREARMOR_X	=	(274 + gsInvDescX);
			BULLET_PREARMOR_Y	=	(38 + gsInvDescY);
			BULLET_TRACER_X	=	(344 + gsInvDescX);
			BULLET_TRACER_Y	=	(14 + gsInvDescY);
			BULLET_ANTITANK_X	=	(369 + gsInvDescX);
			BULLET_ANTITANK_Y	=	(14 + gsInvDescY);
			BULLET_LOCKBUSTER_X	=	(344 + gsInvDescX);
			BULLET_LOCKBUSTER_Y	=	(38 + gsInvDescY);
			BULLET_IGNOREARMOR_X	=	(369 + gsInvDescX);
			BULLET_IGNOREARMOR_Y	=	(38 + gsInvDescY);
			BULLET_ACIDIC_X	=	(344 + gsInvDescX);
			BULLET_ACIDIC_Y	=	(62 + gsInvDescY);
			BULLET_AMMORANGEBON_X	=	(408 + gsInvDescX);
			BULLET_AMMORANGEBON_Y	=	(14 + gsInvDescY);
			BULLET_AMMODAMAGEBON_X	=	(408 + gsInvDescX);
			BULLET_AMMODAMAGEBON_Y	=	(26 + gsInvDescY);
			BULLET_AMMOTOHITBON_X	=	(408 + gsInvDescX);
			BULLET_AMMOTOHITBON_Y	=	(38 + gsInvDescY);
			BULLET_AMMOBURSTBON_X	=	(408 + gsInvDescX);
			BULLET_AMMOBURSTBON_Y	=	(50 + gsInvDescY);
			BULLET_AMMORELIABILITYBON_X	=	(408 + gsInvDescX);
			BULLET_AMMORELIABILITYBON_Y	=	(62 + gsInvDescY);
			BULLET_AMMONOISESUPP_X	=	(408 + gsInvDescX);
			BULLET_AMMONOISESUPP_Y	=	(74 + gsInvDescY);

			// Icon Locations for EXPLOSIVES stats
			BULLET_EXPLOSIVEDAMAGE_X	=	(274 + gsInvDescX);
			BULLET_EXPLOSIVEDAMAGE_Y	=	(14 + gsInvDescY);
			BULLET_EXPLOSIVESTUNDAMAGE_X	=	(274 + gsInvDescX);
			BULLET_EXPLOSIVESTUNDAMAGE_Y	=	(26 + gsInvDescY);
			BULLET_EXPLOSIVELOUDNESS_X	=	(274 + gsInvDescX);
			BULLET_EXPLOSIVELOUDNESS_Y	=	(38 + gsInvDescY);
			BULLET_EXPLOSIVEVOLATILITY_X	=	(274 + gsInvDescX);
			BULLET_EXPLOSIVEVOLATILITY_Y	=	(50 + gsInvDescY);
			BULLET_EXPLOSIVERADIUS_X	=	(359 + gsInvDescX);
			BULLET_EXPLOSIVERADIUS_Y	=	(14 + gsInvDescY);
			BULLET_EXPLOSIVESTARTRADIUS_X	=	(343 + gsInvDescX);
			BULLET_EXPLOSIVESTARTRADIUS_Y	=	(14 + gsInvDescY);
			BULLET_EXPLOSIVEARROW_X	=	(362 + gsInvDescX);
			BULLET_EXPLOSIVEARROW_Y	=	(20 + gsInvDescY);
			BULLET_EXPLOSIVEENDRADIUS_X	=	(378 + gsInvDescX);
			BULLET_EXPLOSIVEENDRADIUS_Y	=	(14 + gsInvDescY);
			BULLET_EXPLOSIVEDURATION_X	=	(341 + gsInvDescX);
			BULLET_EXPLOSIVEDURATION_Y	=	(62 + gsInvDescY);

			// Icon Locations for ARMOR stats
			BULLET_ARMOR_X	=	(274 + gsInvDescX);
			BULLET_ARMOR_Y	=	(14 + gsInvDescY);
			BULLET_ARMORCOVERAGE_X	=	(274 + gsInvDescX);
			BULLET_ARMORCOVERAGE_Y	=	(26 + gsInvDescY);
			BULLET_ARMORDEGRADE_X	=	(274 + gsInvDescX);
			BULLET_ARMORDEGRADE_Y	=	(38 + gsInvDescY);
			BULLET_ARMORAPBONUS_X	=	(341 + gsInvDescX);
			BULLET_ARMORAPBONUS_Y	=	(14 + gsInvDescY);
			BULLET_ARMORAIMBONUS_X	=	(341 + gsInvDescX);
			BULLET_ARMORAIMBONUS_Y	=	(26 + gsInvDescY);
			BULLET_ARMORWOODCAMO_X	=	(341 + gsInvDescX);
			BULLET_ARMORWOODCAMO_Y	=	(38 + gsInvDescY);
			BULLET_ARMORURBANCAMO_X	=	(341 + gsInvDescX);
			BULLET_ARMORURBANCAMO_Y	=	(50 + gsInvDescY);
			BULLET_ARMORDESERTCAMO_X	=	(341 + gsInvDescX);
			BULLET_ARMORDESERTCAMO_Y	=	(62 + gsInvDescY);
			BULLET_ARMORSNOWCAMO_X	=	(341 + gsInvDescX);
			BULLET_ARMORSNOWCAMO_Y	=	(74 + gsInvDescY);
			BULLET_ARMORSTEALTH_X	=	(341 + gsInvDescX);
			BULLET_ARMORSTEALTH_Y	=	(86 + gsInvDescY);
			BULLET_ARMORVISIONRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORVISIONRANGEBONUS_Y	=	(14 + gsInvDescY);
			BULLET_ARMORDAYVISIONRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORDAYVISIONRANGEBONUS_Y	=	(26 + gsInvDescY);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORNIGHTVISIONRANGEBONUS_Y	=	(38 + gsInvDescY);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_Y	=	(50 + gsInvDescY);
			BULLET_ARMORCAVEVISIONRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORCAVEVISIONRANGEBONUS_Y	=	(68 + gsInvDescY);
			BULLET_ARMORPERCENTTUNNELVISION_X	=	(408 + gsInvDescX);
			BULLET_ARMORPERCENTTUNNELVISION_Y	=	(72 + gsInvDescY);
			BULLET_ARMORHEARINGRANGEBONUS_X	=	(408 + gsInvDescX);
			BULLET_ARMORHEARINGRANGEBONUS_Y	=	(86 + gsInvDescY);

			// Generic Icon Locations for MISC Item Stats
			BULLET_COL1ROW1_X =	(274 + gsInvDescX);
			BULLET_COL1ROW1_Y =	(14 + gsInvDescY);
			BULLET_COL1ROW2_X =	(274 + gsInvDescX);
			BULLET_COL1ROW2_Y =	(26 + gsInvDescY);
			BULLET_COL1ROW3_X =	(274 + gsInvDescX);
			BULLET_COL1ROW3_Y =	(38 + gsInvDescY);
			BULLET_COL1ROW4_X =	(274 + gsInvDescX);
			BULLET_COL1ROW4_Y =	(50 + gsInvDescY);
			BULLET_COL1ROW5_X =	(274 + gsInvDescX);
			BULLET_COL1ROW5_Y =	(62 + gsInvDescY);
			BULLET_COL1ROW6_X =	(274 + gsInvDescX);
			BULLET_COL1ROW6_Y =	(74 + gsInvDescY);
			BULLET_COL1ROW7_X =	(274 + gsInvDescX);
			BULLET_COL1ROW7_Y =	(86 + gsInvDescY);
			BULLET_COL1ROW8_X =	(375 + gsInvDescX);
			BULLET_COL1ROW8_Y =	(14 + gsInvDescY);
			BULLET_COL1ROW9_X =	(375 + gsInvDescX);
			BULLET_COL1ROW9_Y =	(26 + gsInvDescY);
			BULLET_COL1ROW10_X =	(375 + gsInvDescX);
			BULLET_COL1ROW10_Y =	(38 + gsInvDescY);
			BULLET_COL1ROW11_X =	(375 + gsInvDescX);
			BULLET_COL1ROW11_Y =	(50 + gsInvDescY);
			BULLET_COL1ROW12_X =	(375 + gsInvDescX);
			BULLET_COL1ROW12_Y =	(62 + gsInvDescY);
			ITEMDESC_CALIBER_X		= ( 53 + gsInvDescX); // Caliber text moved two lines below item name, and pulled closer
			ITEMDESC_CALIBER_Y		= ( 78 + gsInvDescY);
			ITEMDESC_CALIBER_WIDTH	= 142;
			ITEMDESC_NAME_X			= ( 16 + gsInvDescX);
			ITEMDESC_NAME_Y			= ( 67 + gsInvDescY);
			ITEMDESC_ITEM_X			= (  8 + gsInvDescX);
			ITEMDESC_ITEM_Y			= ( 11 + gsInvDescY);
			ITEMDESC_DESC_START_X	= (11 + gsInvDescX); // Description text Y moved downwards one line.
			ITEMDESC_DESC_START_Y	= (102 + gsInvDescY);
			ITEMDESC_PROS_START_X	= (438 + gsInvDescX ); // Pros and cons moved to the extreme right of the text box. They
			ITEMDESC_PROS_START_Y	= (106 + gsInvDescY);  // now show just the words "PROS" and "CONS", and have a tooltip
			ITEMDESC_CONS_START_X	= (438 + gsInvDescX ); // region over them for expanded text. :D
			ITEMDESC_CONS_START_Y	= (118 + gsInvDescY);
			ITEMDESC_DESC_WIDTH		=	415; // Much wider in OIV tactical than other shapes
			ITEMDESC_ITEM_STATUS_X	= (  6 + gsInvDescX );
			ITEMDESC_ITEM_STATUS_Y	= ( 60 + gsInvDescY );
			ITEMDESC_ITEM_STATUS_HEIGHT	= 50;
		}
		else
		{
			// Icon locations, TACTICAL
			BULLET_SING_X	=			(222 + gsInvDescX);
			BULLET_SING_Y	=			(49 + gsInvDescY);
			BULLET_BURST_X	=		(263 + gsInvDescX);
			BULLET_BURST_Y	=		(49 + gsInvDescY);
			ITEMDESC_CALIBER_X		= (162 + gsInvDescX); // Gun Caliber string location, Tactical
			ITEMDESC_CALIBER_Y		= ( 67 + gsInvDescY);
			ITEMDESC_CALIBER_WIDTH	= 142;
			ITEMDESC_NAME_X			= ( 16 + gsInvDescX);
			ITEMDESC_NAME_Y			= ( 67 + gsInvDescY);
			ITEMDESC_ITEM_X			= (  8 + gsInvDescX);
			ITEMDESC_ITEM_Y			= ( 11 + gsInvDescY);
			ITEMDESC_DESC_START_X	= (11 + gsInvDescX); // Item Desc Box offset, tactical
			ITEMDESC_DESC_START_Y	= (80 + gsInvDescY);
			ITEMDESC_PROS_START_X	= ( 11 + gsInvDescX ); // Pros, tactical
			ITEMDESC_PROS_START_Y	= (110 + gsInvDescY);
			ITEMDESC_CONS_START_X	= ( 11 + gsInvDescX ); // Cons, Tactical
			ITEMDESC_CONS_START_Y	= (120 + gsInvDescY);
			ITEMDESC_DESC_WIDTH		= 301; // Text box width, Tactical
			ITEMDESC_ITEM_STATUS_X	= (  6 + gsInvDescX );
			ITEMDESC_ITEM_STATUS_Y	= ( 60 + gsInvDescY );
			ITEMDESC_ITEM_STATUS_HEIGHT	= 50;
		}
	}
}

BOOLEAN InternalInitEnhancedDescBox()
{
	VOBJECT_DESC    VObjectDesc;

	if(UsingEDBSystem() > 0)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoWeaponIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoWeaponIcon) );

		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoAmmoIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoAmmoIcon ) );

		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoExplosiveIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoExplosiveIcon ) );

		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoArmorIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoArmorIcon ) );

		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoMiscIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoMiscIcon ) );
	}

	return (TRUE);
}

void InternalInitEDBTooltipRegion( OBJECTTYPE * gpItemDescObject, UINT32 guiCurrentItemDescriptionScreen )
{
	INT32		cnt;
	INT32		threshold;

	InitEDBCoords();

	//start by deleting the currently defined regions if they exist
	for ( cnt = 0; cnt < 20; cnt++ )
	{
		if( gStatsFasthelpRegions[ cnt ].IDNumber != 0 )
			MSYS_RemoveRegion( &gStatsFasthelpRegions[ cnt ]);
	}

	if( guiCurrentScreen == MAP_SCREEN )
		threshold = 13;
	else
		threshold = 11;
	
	// Strategic EDB
	if ( guiCurrentItemDescriptionScreen == MAP_SCREEN )
	{
		///////////////////////////////////////////
		// Begin Weapon Item Tooltip Regions

		if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gWeaponDescStatsRects[ cnt ].iLeft),
				(INT16)(gWeaponDescStatsRects[ cnt ].iTop),
				(INT16)(gWeaponDescStatsRects[ cnt ].iRight),
				(INT16)(gWeaponDescStatsRects[ cnt ].iBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzWeaponStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}
	
		// Enable regions one-by-one as necessary
		
			// Relevant to all weapons

			// RELIABILITY
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );

			// REPAIR EASE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );

			//////////////////////////////////
			// BEGIN SINGLE ATTACK SEGMENT
						
			// Non-Launcher Guns and Grenade-Launchers only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER|IC_PUNCH) && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				
				// SINGLE-SHOT AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				
				if ( Weapon[gpItemDescObject->usItem].NoSemiAuto )
				{
					// "NO SINGLE-SHOT"
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelp[ 23 ] );
				}
			}

			// Rocket-Launchers Only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && Item[gpItemDescObject->usItem].rocketlauncher )
			{
				// SINGLE ROCKET-LAUNCH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelp[ 21 ] );
			}

			// Throwing-Knives Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_THROWING_KNIFE )
			{
				// THROW KNIFE AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelp[ 20 ] );
			}

			// Blades Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_BLADE )
			{
				// SLASH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelp[ 22 ] );
			}

			// Blunts Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_PUNCH )
			{
				// SLASH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelp[ 26 ] );
			}

			// END SINGLE ATTACK SEGMENT
			////////////////////////////////////
			
			////////////////////////////////////
			// BEGIN BURST ATTACK SEGMENT

			// Non-Launcher Guns and Grenade-Launchers only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				// BURST
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );

				if (GetShotsPerBurst(gpItemDescObject) <= 0 )
				{	
					// "NO BURST"
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 9 ]), gzWeaponStatsFasthelp[ 24 ] );
				}
			}

			// END BURST ATTACK SEGMENT
			////////////////////////////////////

			////////////////////////////////////
			// BEGIN AUTO ATTACK SEGMENT
			
			// Non-Launcher Guns Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
				
				if (GetAutofireShotsPerFiveAPs(gpItemDescObject) <= 0 )
				{
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 10 ]), gzWeaponStatsFasthelp[ 25 ] );
				}
			}

			// END AUTO ATTACK SEGMENT
			///////////////////////////////////

			// Guns, Throwing knives and Launchers only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
			{
				// RANGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );			
			}

			// Guns and Launchers Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				// ACCURACY
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// Guns and Multi-use Launchers Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				// RELOAD AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );

				if ( Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
				{
					// RELOAD MANUALLY AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
				}
			}
				
			// Guns and Grenade Launchers only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_PUNCH) && !Item[ gpItemDescObject->usItem].rocketlauncher )
			{
				// DRAW AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}
			
			// Non-Launcher Guns, Blades, and Throwing Knives only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_BLADE|IC_PUNCH|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				// DAMAGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );
			}

			// Non-Launcher Guns Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher )
			{
				if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 || GetShotsPerBurst(gpItemDescObject)> 0 )
				{
					// BURST PENALTY
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

					if ( Weapon[ gpItemDescObject->usItem ].bAutofireShotsPerFiveAP > 0 )
					{
						// AUTOFIRE SHOTS PER 5 AP
						MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
					}
				}
			}
				
			//////////////////////////////////////////////////////////////////////
			// Begin non-type-specific icons. These are not type specific because they represent bonuses
			// which may one day be added to various types of weapons by modders...

			if (Weapon[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubAttackVolume > 0 )
			{
				// NOISE REDUCTION
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 14 ] );
			}
				
			if (IsFlashSuppressorAlt( gpItemDescObject ) == TRUE)
			{
				// HIDE FLASH
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );
			}

			if (GetFlatToHitBonus( gpItemDescObject ) != 0)
			{
				// HIT BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 15 ] );

				if (GetFlatToHitBonus( gpItemDescObject ) > 0 && GetAverageBestLaserRange( gpItemDescObject ) / 10 > 0 )
				{
					// LASER RANGE
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 16 ] );
				}	
			}

			if (GetBipodBonus( gpItemDescObject ) != 0)
			{
				// BIPOD BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 19 ] );
			}
			
			if (IsScoped( gpItemDescObject ))
			{
				// SCOPE AIM BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 17 ] );
				if (GetMinRangeForAimBonus( gpItemDescObject ) > 0)
				{
					// LASER RANGE
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 18 ] );
				}	
			}	

		}

		///////////////////////////////////////////////////
		// Begin Ammo Item Tooltip Section

		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_AMMO )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gAmmoDescStatsRects[ cnt ]. iLeft),
				(INT16)(gAmmoDescStatsRects[ cnt ].iTop),
				(INT16)(gAmmoDescStatsRects[ cnt ].iRight),
				(INT16)(gAmmoDescStatsRects[ cnt ].iBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzAmmoStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// AMMO PENETRATION
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );

			// AMMO TUMBLE
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );

			// AMMO PRE-IMPACT EXPLOSION
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// TRACER
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// ANTI-TANK
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].antiTank )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// LOCKBUSTER
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
			}

			// IGNORE ARMOR
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}
			
			// ACIDIC ICON
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}

			// RANGE BONUS
			if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
			}

			// DAMAGE BONUS
			if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );
			}

			// TO HIT BONUS
			if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
			}

			// BURST BONUS
			if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 || Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );
			}

			// RELIABILITY BONUS
			if ( Item[ gpItemDescObject->usItem ].bReliability != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
			}

			// NOISE SUPPRESSION
			if ( Item[ gpItemDescObject->usItem ].percentnoisereduction > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );
			}
		}

		///////////////////////////////////////////////////
		// Begin Armor Item Tooltip Section

		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gArmorDescStatsRects[ cnt ]. iLeft),
				(INT16)(gArmorDescStatsRects[ cnt ].iTop),
				(INT16)(gArmorDescStatsRects[ cnt ].iRight),
				(INT16)(gArmorDescStatsRects[ cnt ].iBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzArmorStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// PROTECTION
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );

			// COVERAGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );

			// ARMOR DEGRADE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// AP BONUS
			if ( GetAPBonus ( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// AIM BONUS
			if ( GetFlatAimBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// WOODLAND CAMO
			if ( GetCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
			}

			// URBAN CAMO
			if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}

			// DESERT CAMO
			if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}

			// SNOW CAMO
			if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
			}

			// STEALTH
			if ( GetStealthBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );
			}

			// VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
			}

			// DAY VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 1 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );
			}

			// NIGHT VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
			}

			// BRIGHT LIGHT VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );
			}

			// CAVE VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 14 ] );
			}

			// PERCENT TUNNEL VISION
			if ( GetItemPercentTunnelVision( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 15 ] );
			}

			// HEARING RANGE BONUS
			if ( GetItemHearingRangeBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 16 ] );
			}
		}

		//////////////////////////////////////
		// Begin Explosive Item Tooltips Section
		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gExplosiveDescStatsRects[ cnt ]. iLeft),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iTop),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iRight),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzExplosiveStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );
			}

			// STUN DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );
			}
			
			// LOUDNESS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// VOLATILITY
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// SINGLE-EVENT EXPLOSION data
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 )
			{
				// RADIUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );

			}
			// PROLONGED EXPLOSION data
			else if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
			{

				// DURATION
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );

				// STARTING RADIUS
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
				
				// FINAL RADIUS
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}
		}

		//////////////////////////////////////////////////////////
		// The following is more complex - it sets up tooltip regions based not on predetermined order but on how much data there is on the chart.
		// It begins with defining all regions, then goes on to set the helptext of each one based on each value encountered in the item's stats.
		// At the very end, it enables as many tooltips as values encountered.

		else
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gMiscItemDescStatsRects[ cnt ]. iLeft),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iTop),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iRight),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}
			
			cnt=0;

			// ITEM SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].itemsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 0 ] );
				cnt++;
			}
			
			// RELIABILITY MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].bReliability != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 1 ] );
				cnt++;
			}
			
			// NOISE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentnoisereduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 2 ] );
				cnt++;
			}

			// HIDE MUZZLE FLASH ICON
			if ( Item[ gpItemDescObject->usItem ].hidemuzzleflash > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 3 ] );
				cnt++;
			}

			// BIPOD ICON
			if ( Item[ gpItemDescObject->usItem ].bipod != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 4 ] );
				cnt++;
			}

			// RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 5 ] );
				cnt++;
			}

			// TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 6 ] );
				cnt++;
			}

			// LASER RANGE
			if ( Item[ gpItemDescObject->usItem ].bestlaserrange > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 7 ] );
				cnt++;
			}

			// AIMING MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].aimbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 8 ] );
				cnt++;
			}

			// MINIMUM RANGE FOR AIMING BONUS ICON
			if ( Item[ gpItemDescObject->usItem ].minrangeforaimbonus > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 33 ] );
				cnt++;
			}

			// MAGAZINE SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].magsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 17 ] );
				cnt++;
			}

			// BURST SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].burstsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 9 ] );
				cnt++;
			}

			// BURST TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 10 ] );
				cnt++;
			}

			// AUTO TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 11 ] );
				cnt++;
			}

			// AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].APBonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 12 ] );
				cnt++;
			}

			// BURST AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentburstfireapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 13 ] );
				cnt++;
			}

			// AUTOFIRE AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentautofireapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 14 ] );
				cnt++;
			}

			// READY AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentreadytimeapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 15 ] );
				cnt++;
			}

			// RELOAD AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentreloadtimeapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 16 ] );
				cnt++;
			}

			// SINGLE AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 18 ] );
				cnt++;
			}

			// DAMAGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 19 ] );
				cnt++;
			}

			// MELEE DAMAGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].meleedamagebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 20 ] );
				cnt++;
			}

			// WOOD CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].camobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 21 ] );
				cnt++;
			}

			// URBAN CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].urbanCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 22 ] );
				cnt++;
			}

			// DESERT CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].desertCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 23 ] );
				cnt++;
			}

			// SNOW CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].snowCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 24 ] );
				cnt++;
			}

			// STEALTH MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].stealthbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 25 ] );
				cnt++;
			}

			// HEARING RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].hearingrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 26 ] );
				cnt++;
			}

			// VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 27 ] );
				cnt++;
			}

			// DAY VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 28 ] );
				cnt++;
			}

			// NIGHT VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 29 ] );
				cnt++;
			}

			// BRIGHT LIGHT VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 30 ] );
				cnt++;
			}

			// CAVE VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 31 ] );
				cnt++;
			}

			// PERCENT TUNNEL VISION MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percenttunnelvision != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 32 ] );
				cnt++;
			}

			UINT8 cntval = cnt;

			// Enable as many regions as required
			for ( cnt = 0; cnt<cntval; cnt++ )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

		}
	}
	// HEADROCK: TACTICAL description box tooltips
	else if (guiCurrentItemDescriptionScreen !=MAP_SCREEN)
	{
		///////////////////////////////////////////
		// Begin Weapon Item Tooltip Regions

		if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gWeaponDescStatsRects[ cnt ].iLeft + gsInvDescX),
				(INT16)(gWeaponDescStatsRects[ cnt ].iTop + gsInvDescY),
				(INT16)(gWeaponDescStatsRects[ cnt ].iRight + gsInvDescX),
				(INT16)(gWeaponDescStatsRects[ cnt ].iBottom + gsInvDescY),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzWeaponStatsFasthelpTactical[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}
	
		// Enable regions one-by-one as necessary
		
			// Relevant to all weapons

			// RELIABILITY
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );

			// REPAIR EASE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );

			//////////////////////////////////
			// BEGIN SINGLE ATTACK SEGMENT
						
			// Non-Launcher Guns and Grenade-Launchers only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER|IC_PUNCH) && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				
				// SINGLE-SHOT AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				
				if ( Weapon[gpItemDescObject->usItem].NoSemiAuto )
				{
					// "NO SINGLE-SHOT"
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelpTactical[ 23 ] );
				}
			}

			// Rocket-Launchers Only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && Item[gpItemDescObject->usItem].rocketlauncher )
			{
				// SINGLE ROCKET-LAUNCH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelpTactical[ 21 ] );
			}

			// Throwing-Knives Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_THROWING_KNIFE )
			{
				// THROW KNIFE AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelpTactical[ 20 ] );
			}

			// Blades Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_BLADE )
			{
				// SLASH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelpTactical[ 22 ] );
			}

			// Blunts Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_PUNCH )
			{
				// SLASH AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ 8 ]), gzWeaponStatsFasthelpTactical[ 26 ] );
			}

			// END SINGLE ATTACK SEGMENT
			////////////////////////////////////
			
			////////////////////////////////////
			// BEGIN BURST ATTACK SEGMENT

			// Non-Launcher Guns and Grenade-Launchers only
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				// BURST
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );

				if (GetShotsPerBurst(gpItemDescObject) <= 0 )
				{	
					// "NO BURST"
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 9 ]), gzWeaponStatsFasthelpTactical[ 24 ] );
				}
			}

			// END BURST ATTACK SEGMENT
			////////////////////////////////////

			////////////////////////////////////
			// BEGIN AUTO ATTACK SEGMENT
			
			// Non-Launcher Guns Only
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
				
				if (GetAutofireShotsPerFiveAPs(gpItemDescObject) <= 0 )
				{
					SetRegionFastHelpText( &(gStatsFasthelpRegions[ 10 ]), gzWeaponStatsFasthelpTactical[ 25 ] );
				}
			}

			// END AUTO ATTACK SEGMENT
			///////////////////////////////////

			// Guns, Throwing knives and Launchers only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
			{
				// RANGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );			
			}

			// Guns and Launchers Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				// ACCURACY
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// Guns and Multi-use Launchers Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				// RELOAD AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );

				if ( Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
				{
					// RELOAD MANUALLY AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
				}
			}
				
			// Guns and Grenade Launchers only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_PUNCH) && !Item[ gpItemDescObject->usItem].rocketlauncher )
			{
				// DRAW AP
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}
			
			// Non-Launcher Guns, Blades, and Throwing Knives only
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_BLADE|IC_PUNCH|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				// DAMAGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );
			}

			// Non-Launcher Guns Only
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher )
			{
				if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 || GetShotsPerBurst(gpItemDescObject)> 0 )
				{
					// BURST PENALTY
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

					if ( Weapon[ gpItemDescObject->usItem ].bAutofireShotsPerFiveAP > 0 )
					{
						// AUTOFIRE SHOTS PER 5 AP
						MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
					}
				}
			}
				
			//////////////////////////////////////////////////////////////////////
			// Begin non-type-specific icons. These are not type specific because they represent bonuses
			// which may one day be added to various types of weapons by modders...

			if (Weapon[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubAttackVolume > 0 )
			{
				// NOISE REDUCTION
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 18 ] );	//14
			}
				
			if (IsFlashSuppressorAlt( gpItemDescObject ) == TRUE)
			{
				// HIDE FLASH
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 17 ] );	//13
			}

			if (GetFlatToHitBonus( gpItemDescObject ) != 0)
			{
				// HIT BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );	//15

				if (GetFlatToHitBonus( gpItemDescObject ) > 0 && GetAverageBestLaserRange( gpItemDescObject ) / 10 > 0 )
				{
					// LASER RANGE
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 14 ] );	//16
				}	
			}

			if (GetBipodBonus( gpItemDescObject ) != 0)
			{
				// BIPOD BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 19 ] );
			}
			
			if (IsScoped( gpItemDescObject ))
			{
				// SCOPE AIM BONUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 15 ] );	//17
				if (GetMinRangeForAimBonus( gpItemDescObject ) / 10 > 0 )
				{
					// LASER RANGE
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 16 ] );	//18
				}	
			}	

		}

		///////////////////////////////////////////////////
		// Begin Ammo Item Tooltip Section

		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_AMMO )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gAmmoDescStatsRects[ cnt ]. iLeft + gsInvDescX),
				(INT16)(gAmmoDescStatsRects[ cnt ].iTop + gsInvDescY),
				(INT16)(gAmmoDescStatsRects[ cnt ].iRight + gsInvDescX),
				(INT16)(gAmmoDescStatsRects[ cnt ].iBottom + gsInvDescY),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzAmmoStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// AMMO PENETRATION
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );

			// AMMO TUMBLE
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );

			// AMMO PRE-IMPACT EXPLOSION
			MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// TRACER
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// ANTI-TANK
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].antiTank )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// LOCKBUSTER
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
			}

			// IGNORE ARMOR
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}
			
			// ACIDIC ICON
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}

			// RANGE BONUS
			if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
			}

			// DAMAGE BONUS
			if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );
			}

			// TO HIT BONUS
			if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
			}

			// BURST BONUS
			if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 || Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );
			}

			// RELIABILITY BONUS
			if ( Item[ gpItemDescObject->usItem ].bReliability != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
			}

			// NOISE SUPPRESSION
			if ( Item[ gpItemDescObject->usItem ].percentnoisereduction > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );
			}
		}

		///////////////////////////////////////////////////
		// Begin Armor Item Tooltip Section

		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gArmorDescStatsRects[ cnt ]. iLeft + gsInvDescX),
				(INT16)(gArmorDescStatsRects[ cnt ].iTop + gsInvDescY),
				(INT16)(gArmorDescStatsRects[ cnt ].iRight + gsInvDescX),
				(INT16)(gArmorDescStatsRects[ cnt ].iBottom + gsInvDescY),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzArmorStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// PROTECTION
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );

			// COVERAGE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );

			// ARMOR DEGRADE
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// AP BONUS
			if ( GetAPBonus ( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// AIM BONUS
			if ( GetFlatAimBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );
			}

			// WOODLAND CAMO
			if ( GetCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
			}

			// URBAN CAMO
			if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}

			// DESERT CAMO
			if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );
			}

			// SNOW CAMO
			if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 8 ] );
			}

			// STEALTH
			if ( GetStealthBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 9 ] );
			}

			// VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 10 ] );
			}

			// DAY VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 1 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 11 ] );
			}

			// NIGHT VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 12 ] );
			}

			// BRIGHT LIGHT VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 13 ] );
			}

			// CAVE VISION RANGE BONUS
			if ( GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 14 ] );
			}

			// PERCENT TUNNEL VISION
			if ( GetItemPercentTunnelVision( gpItemDescObject ) > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 15 ] );
			}

			// HEARING RANGE BONUS
			if ( GetItemHearingRangeBonus( gpItemDescObject ) != 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 16 ] );
			}
		}

		//////////////////////////////////////
		// Begin Explosive Item Tooltips Section
		else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gExplosiveDescStatsRects[ cnt ]. iLeft + gsInvDescX),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iTop + gsInvDescY),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iRight + gsInvDescX),
				(INT16)(gExplosiveDescStatsRects[ cnt ].iBottom + gsInvDescY),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzExplosiveStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

			// DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 0 ] );
			}

			// STUN DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 1 ] );
			}
			
			// LOUDNESS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 2 ] );

			// VOLATILITY
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 3 ] );
			}

			// SINGLE-EVENT EXPLOSION data
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 )
			{
				// RADIUS
				MSYS_EnableRegion( &gStatsFasthelpRegions[ 4 ] );

			}
			// PROLONGED EXPLOSION data
			else if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
			{

				// DURATION
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 7 ] );

				// STARTING RADIUS
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 5 ] );
				
				// FINAL RADIUS
					MSYS_EnableRegion( &gStatsFasthelpRegions[ 6 ] );
			}
		}

		//////////////////////////////////////////////////////////
		// The following is more complex - it sets up tooltip regions based not on predetermined order but on how much data there is on the chart.
		// It begins with defining all regions, then goes on to set the helptext of each one based on each value encountered in the item's stats.
		// At the very end, it enables as many tooltips as values encountered.

		else
		{
			// Create all regions
			for (cnt = 0; cnt < 20; cnt++)
			{
				MSYS_DefineRegion( &gStatsFasthelpRegions[ cnt ],
				(INT16)(gMiscItemDescStatsRects[ cnt ].iLeft + gsInvDescX),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iTop + gsInvDescY),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iRight + gsInvDescX),
				(INT16)(gMiscItemDescStatsRects[ cnt ].iBottom + gsInvDescY),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gStatsFasthelpRegions[ cnt ]);
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ cnt ] );
				SetRegionHelpEndCallback( &(gStatsFasthelpRegions[ cnt ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gStatsFasthelpRegions[ cnt ] );
			}
			
			cnt=0;

			// ITEM SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].itemsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 0 ] );
				cnt++;
			}
			
			// RELIABILITY MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].bReliability != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 1 ] );
				cnt++;
			}
			
			// NOISE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentnoisereduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 2 ] );
				cnt++;
			}

			// HIDE MUZZLE FLASH ICON
			if ( Item[ gpItemDescObject->usItem ].hidemuzzleflash > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 3 ] );
				cnt++;
			}

			// BIPOD ICON
			if ( Item[ gpItemDescObject->usItem ].bipod != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 4 ] );
				cnt++;
			}

			// RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 5 ] );
				cnt++;
			}

			// TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 6 ] );
				cnt++;
			}

			// LASER RANGE
			if ( Item[ gpItemDescObject->usItem ].bestlaserrange > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 7 ] );
				cnt++;
			}

			// AIMING MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].aimbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 8 ] );
				cnt++;
			}

			// MINIMUM RANGE FOR AIMING BONUS ICON
			if ( Item[ gpItemDescObject->usItem ].minrangeforaimbonus > 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 33 ] );
				cnt++;
			}

			// MAGAZINE SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].magsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 17 ] );
				cnt++;
			}

			// BURST SIZE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].burstsizebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 9 ] );
				cnt++;
			}

			// BURST TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 10 ] );
				cnt++;
			}

			// AUTO TO-HIT MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 11 ] );
				cnt++;
			}

			// AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].APBonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 12 ] );
				cnt++;
			}

			// BURST AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentburstfireapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 13 ] );
				cnt++;
			}

			// AUTOFIRE AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentautofireapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 14 ] );
				cnt++;
			}

			// READY AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentreadytimeapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 15 ] );
				cnt++;
			}

			// RELOAD AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentreloadtimeapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 16 ] );
				cnt++;
			}

			// SINGLE AP MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percentapreduction != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 18 ] );
				cnt++;
			}

			// DAMAGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 19 ] );
				cnt++;
			}

			// MELEE DAMAGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].meleedamagebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 20 ] );
				cnt++;
			}

			// WOOD CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].camobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 21 ] );
				cnt++;
			}

			// URBAN CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].urbanCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 22 ] );
				cnt++;
			}

			// DESERT CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].desertCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 23 ] );
				cnt++;
			}

			// SNOW CAMO ICON
			if ( Item[ gpItemDescObject->usItem ].snowCamobonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 24 ] );
				cnt++;
			}

			// STEALTH MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].stealthbonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 25 ] );
				cnt++;
			}

			// HEARING RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].hearingrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 26 ] );
				cnt++;
			}

			// VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 27 ] );
				cnt++;
			}

			// DAY VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 28 ] );
				cnt++;
			}

			// NIGHT VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 29 ] );
				cnt++;
			}

			// BRIGHT LIGHT VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 30 ] );
				cnt++;
			}

			// CAVE VISION RANGE MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 31 ] );
				cnt++;
			}

			// PERCENT TUNNEL VISION MODIFIER ICON
			if ( Item[ gpItemDescObject->usItem ].percenttunnelvision != 0 && cnt<=threshold )
			{
				SetRegionFastHelpText( &(gStatsFasthelpRegions[ cnt ]), gzMiscItemStatsFasthelp[ 32 ] );
				cnt++;
			}

			UINT8 cntval = cnt;

			// Enable as many regions as required
			for ( cnt = 0; cnt<cntval; cnt++ )
			{
				MSYS_EnableRegion( &gStatsFasthelpRegions[ cnt ] );
			}

		}
	}
}

void DeleteEnhancedDescBox( UINT32 guiCurrentItemDescriptionScreen )
{
	INT32 cnt;

	DeleteVideoObjectFromIndex( guiItemInfoWeaponIcon );
	DeleteVideoObjectFromIndex( guiItemInfoAmmoIcon );
	DeleteVideoObjectFromIndex( guiItemInfoArmorIcon );
	DeleteVideoObjectFromIndex( guiItemInfoExplosiveIcon );
	DeleteVideoObjectFromIndex( guiItemInfoMiscIcon );

	if ( guiCurrentItemDescriptionScreen ==MAP_SCREEN && UsingEDBSystem() > 0 )
	{
		UINT16 RegionsToRemove = 20;

		for (cnt = 0; cnt < RegionsToRemove; cnt++)
		{
			MSYS_RemoveRegion( &gStatsFasthelpRegions[cnt] );
		}
	}

	if ( guiCurrentItemDescriptionScreen !=MAP_SCREEN && UsingEDBSystem() > 0 )
	{
		UINT16 RegionsToRemove = 20;

		for (cnt = 0; cnt < RegionsToRemove; cnt++)
		{
			MSYS_RemoveRegion( &gStatsFasthelpRegions[cnt] );
		}
	}
}

void DrawWeaponStats( OBJECTTYPE * gpItemDescObject )
{
	INT32	cnt;

	if( UsingEDBSystem() == 0 )
		return;

	// Start with icons relevant to all weapons

	// RELIABILITY ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 9, BULLET_RELIABILITY_X, BULLET_RELIABILITY_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	// REPAIR EASE ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, BULLET_REPAIR_X, BULLET_REPAIR_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	//////////////////////////////////
	// BEGIN SINGLE ATTACK SEGMENT
				
	// Non-Launcher Guns only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
	{
		// "NO SINGLE-SHOT" ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 19, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
		{
			// SINGLE SHOT AP ICON overwrites shadow
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, BULLET_SING_X+1, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	// Rocket-Launchers Only
	if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && Item[gpItemDescObject->usItem].rocketlauncher )
	{
		// SINGLE ROCKET-LAUNCH AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 21, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Grenade-Launchers Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpItemDescObject->usItem].rocketlauncher )
	{
		if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
		{
			// SINGLE GRENADE-LAUNCH AP ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 24, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	// Throwing-Knives Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_THROWING_KNIFE )
	{
		// THROW KNIFE AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 22, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Blades Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_BLADE )
	{
		// SLASH AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 20, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Blunts Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_PUNCH )
	{
		// BASH AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 26, BULLET_SING_X, BULLET_SING_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// END SINGLE ATTACK SEGMENT
	////////////////////////////////////
	
	////////////////////////////////////
	// BEGIN BURST ATTACK SEGMENT

	// Non-Launcher Guns Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
	{
		// "NO BURST" ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 11, BULLET_BURST_X, BULLET_BURST_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		if (GetShotsPerBurst(gpItemDescObject)> 0)
		{
			for ( cnt = 0; cnt < GetShotsPerBurst(gpItemDescObject); cnt++ )
			{
				// BURST FIRE ICON overwrites shadow
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, BULLET_BURST_X + cnt * (BULLET_WIDTH/2 + 1) +1, BULLET_BURST_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}
	}

	// Grenade-Launchers Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpItemDescObject->usItem].rocketlauncher )
	{
		if (GetShotsPerBurst(gpItemDescObject)> 0)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 25, BULLET_BURST_X, BULLET_BURST_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	// END BURST ATTACK SEGMENT
	////////////////////////////////////

	////////////////////////////////////
	// BEGIN AUTO ATTACK SEGMENT

	// Non-Launcher Guns Only
	if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
	{
		// "NO-AUTO" ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 12, BULLET_AUTO_X, BULLET_AUTO_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
		{
			for ( cnt = 0; cnt < 10; cnt++ )
			{
				// AUTO FIRE ICON overwrites shadow
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, BULLET_AUTO_X + cnt * (BULLET_WIDTH/2 + 1) +1, BULLET_AUTO_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}
	}

	// END AUTO ATTACK SEGMENT
	///////////////////////////////////

	// Guns, Throwing knives and Launchers only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
	{
		// RANGE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 4, BULLET_RANGE_X, BULLET_RANGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Guns and Launchers Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		// ACCURACY ICON (Always displayed for these items, even if accuracy is 0)
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 8, BULLET_ACC_X, BULLET_ACC_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		// NOISE REDUCTION ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 17, BULLET_LOUDNESS_X, BULLET_LOUDNESS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Guns and Multi-use Launchers Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
	{
		// RELOAD AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 2, BULLET_RELOAD_X, BULLET_RELOAD_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			
		if ( Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
		{
			// AP to Reload Manually ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 3, BULLET_MRELOAD_X, BULLET_MRELOAD_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}
		
	// Guns and Grenade Launchers only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem].rocketlauncher )
	{
		// DRAW AP ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 1, BULLET_DRAW_X, BULLET_DRAW_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	
	// Non-Launcher Guns, Blades, and Throwing Knives only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
	{
		// DAMAGE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 5, BULLET_DAMAGE_X, BULLET_DAMAGE_Y-1, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// Non-Launcher Guns Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher )
	{
		if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 || GetShotsPerBurst(gpItemDescObject)> 0 )
		{
			// AUTO PENALTY ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 6, BULLET_AUTOPEN_X, BULLET_AUTOPEN_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			
			if ( Weapon[ gpItemDescObject->usItem ].bAutofireShotsPerFiveAP > 0 )
			{
				// AUTOFIRE SHOTS PER 5 AP ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 7, BULLET_AUTO5AP_X, BULLET_AUTO5AP_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}
	}
		
	//////////////////////////////////////////////////////////////////////
	// Begin non-type-specific icons. These are not type specific because they represent bonuses
	// which may one day be added to various types of weapons by modders...

	if (IsFlashSuppressorAlt( gpItemDescObject ) == TRUE)
	{
		UINT8	icon = 28;
		if(guiCurrentScreen != MAP_SCREEN && UsingNewInventorySystem() == true)
			icon = 18;
		// HIDE FLASH ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, icon, BULLET_FLASH_X, BULLET_FLASH_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if (GetFlatToHitBonus( gpItemDescObject ) != 0)
	{
		// HIT BONUS ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 13, BULLET_TOHIT_X, BULLET_TOHIT_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		if (GetFlatToHitBonus( gpItemDescObject ) > 0 && GetAverageBestLaserRange( gpItemDescObject ) / 10 > 0)
		{
			// LASER RANGE ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 14, BULLET_BESTLASERRANGE_X, BULLET_BESTLASERRANGE_Y-1, VO_BLT_SRCTRANSPARENCY, NULL );
		}	
	}

	if (GetBipodBonus( gpItemDescObject ) != 0)
	{
		// BIPOD BONUS ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 16, BULLET_BIPOD_X, BULLET_BIPOD_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	
	if (IsScoped( gpItemDescObject ))
	{
		// SCOPE AIM BONUS ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 15, BULLET_AIMBONUS_X, BULLET_AIMBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		if (GetMinRangeForAimBonus( gpItemDescObject ))
		{					
			// SCOPE MIN RANGE FOR AIM BONUS ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 27, BULLET_MINRANGEAIMBONUS_X, BULLET_MINRANGEAIMBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}	
}

void DrawAmmoStats( OBJECTTYPE * gpItemDescObject )
{
	// ARMOR PIERCING ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 0, BULLET_ARMORPEN_X, BULLET_ARMORPEN_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// TUMBLE ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 1, BULLET_TUMBLE_X, BULLET_TUMBLE_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// PRE-ARMOR EXPLOSION ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 2, BULLET_PREARMOR_X, BULLET_PREARMOR_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// ANTI-TANK ICON
	if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].antiTank )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 3, BULLET_ANTITANK_X, BULLET_ANTITANK_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// IGNORE ARMOR ICON
	if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 4, BULLET_IGNOREARMOR_X, BULLET_IGNOREARMOR_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	
	// ACIDIC ICON
	if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 5, BULLET_ACIDIC_X, BULLET_ACIDIC_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// LOCKBUSTER ICON
	if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 6, BULLET_LOCKBUSTER_X, BULLET_LOCKBUSTER_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// TRACER
	if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 7, BULLET_TRACER_X, BULLET_TRACER_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// RANGE BONUS
	if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 8, BULLET_AMMORANGEBON_X, BULLET_AMMORANGEBON_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// DAMAGE BONUS
	if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 9, BULLET_AMMODAMAGEBON_X, BULLET_AMMODAMAGEBON_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// TO HIT BONUS
	if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 11, BULLET_AMMOTOHITBON_X, BULLET_AMMOTOHITBON_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// BURST BONUS
	if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 || Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 10, BULLET_AMMOBURSTBON_X, BULLET_AMMOBURSTBON_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// RELIABILITY BONUS
	if ( Item[ gpItemDescObject->usItem ].bReliability != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 12, BULLET_AMMORELIABILITYBON_X, BULLET_AMMORELIABILITYBON_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// NOISE SUPPRESSION
	if ( Item[ gpItemDescObject->usItem ].percentnoisereduction > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 13, BULLET_AMMONOISESUPP_X, BULLET_AMMONOISESUPP_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}

void DrawExplosiveStats( OBJECTTYPE * gpItemDescObject )
{
	if( UsingEDBSystem() == 0 )
		return;

	// DAMAGE ICON
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 0, BULLET_EXPLOSIVEDAMAGE_X, BULLET_EXPLOSIVEDAMAGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// STUN DAMAGE ICON
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 1, BULLET_EXPLOSIVESTUNDAMAGE_X, BULLET_EXPLOSIVESTUNDAMAGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	
	// LOUDNESS ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 2, BULLET_EXPLOSIVELOUDNESS_X, BULLET_EXPLOSIVELOUDNESS_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// VOLATILITY ICON
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 3, BULLET_EXPLOSIVEVOLATILITY_X, BULLET_EXPLOSIVEVOLATILITY_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// SINGLE-EVENT EXPLOSION data
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 )
	{
		// SOUND RADIUS ICON
		if ( Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 5 )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 6, BULLET_EXPLOSIVERADIUS_X, BULLET_EXPLOSIVERADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else if ( Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 1 )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 5, BULLET_EXPLOSIVERADIUS_X, BULLET_EXPLOSIVERADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			// RADIUS ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 4, BULLET_EXPLOSIVERADIUS_X, BULLET_EXPLOSIVERADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

	}
	// PROLONGED EXPLOSION data
	else if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
	{
		// ARROW
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 8, BULLET_EXPLOSIVEARROW_X, BULLET_EXPLOSIVEARROW_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		// DURATION ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 7, BULLET_EXPLOSIVEDURATION_X, BULLET_EXPLOSIVEDURATION_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 2)
		{
			// STARTING RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 9, BULLET_EXPLOSIVESTARTRADIUS_X, BULLET_EXPLOSIVESTARTRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			// FINAL RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 10, BULLET_EXPLOSIVEENDRADIUS_X, BULLET_EXPLOSIVEENDRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// MUSTARD GAS
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 3)
		{
			// STARTING RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 13, BULLET_EXPLOSIVESTARTRADIUS_X, BULLET_EXPLOSIVESTARTRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			// FINAL RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 14, BULLET_EXPLOSIVEENDRADIUS_X, BULLET_EXPLOSIVEENDRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// LIGHT
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 4)
		{
			// HEADROCK: Please note that these two are reversed for lights, as for some reason lights start
			// at larger radius and decay to a smaller one.

			// STARTING RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 18, BULLET_EXPLOSIVESTARTRADIUS_X, BULLET_EXPLOSIVESTARTRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			// FINAL RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 17, BULLET_EXPLOSIVEENDRADIUS_X, BULLET_EXPLOSIVEENDRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// SMOKE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 6)
		{
			// STARTING RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 11, BULLET_EXPLOSIVESTARTRADIUS_X, BULLET_EXPLOSIVESTARTRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			// FINAL RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 12, BULLET_EXPLOSIVEENDRADIUS_X, BULLET_EXPLOSIVEENDRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// NAPALM
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 8)
		{
			// STARTING RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 15, BULLET_EXPLOSIVESTARTRADIUS_X, BULLET_EXPLOSIVESTARTRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			// FINAL RADIUS ICON
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 16, BULLET_EXPLOSIVEENDRADIUS_X, BULLET_EXPLOSIVEENDRADIUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}

	}
}

void DrawArmorStats( OBJECTTYPE * gpItemDescObject )
{
	if( UsingEDBSystem() == 0 )
		return;

	// HELMET
	if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 0 )
	{
	// ARMOR VALUE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 1, BULLET_ARMOR_X, BULLET_ARMOR_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	// ARMOR COVERAGE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 4, BULLET_ARMORCOVERAGE_X, BULLET_ARMORCOVERAGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	// LEGGINGS
	else if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 2 )
	{
	// ARMOR VALUE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 2, BULLET_ARMOR_X, BULLET_ARMOR_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	// ARMOR COVERAGE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 5, BULLET_ARMORCOVERAGE_X, BULLET_ARMORCOVERAGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	// VEST and anything else
	else
	{
	// ARMOR VALUE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 0, BULLET_ARMOR_X, BULLET_ARMOR_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	// ARMOR COVERAGE ICON
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 3, BULLET_ARMORCOVERAGE_X, BULLET_ARMORCOVERAGE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// ARMOR DEGRADE ICON
	BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 6, BULLET_ARMORDEGRADE_X, BULLET_ARMORDEGRADE_Y, VO_BLT_SRCTRANSPARENCY, NULL );

	// AP BONUS ICON
	if ( GetAPBonus ( gpItemDescObject ) != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 11, BULLET_ARMORAPBONUS_X, BULLET_ARMORAPBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// AIM BONUS ICON
	if ( GetFlatAimBonus( gpItemDescObject ) != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 12, BULLET_ARMORAIMBONUS_X, BULLET_ARMORAIMBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// WOODLAND CAMO ICON
	if ( GetCamoBonus( gpItemDescObject ) > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 7, BULLET_ARMORWOODCAMO_X, BULLET_ARMORWOODCAMO_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// URBAN CAMO ICON
	if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 8, BULLET_ARMORURBANCAMO_X, BULLET_ARMORURBANCAMO_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// DESERT CAMO ICON
	if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 9, BULLET_ARMORDESERTCAMO_X, BULLET_ARMORDESERTCAMO_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// SNOW CAMO ICON
	if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 10, BULLET_ARMORSNOWCAMO_X, BULLET_ARMORSNOWCAMO_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// STEALTH ICON
	if ( GetStealthBonus( gpItemDescObject ) != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 18, BULLET_ARMORSTEALTH_X, BULLET_ARMORSTEALTH_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 17, BULLET_ARMORVISIONRANGEBONUS_X, BULLET_ARMORVISIONRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// DAY VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 14, BULLET_ARMORDAYVISIONRANGEBONUS_X, BULLET_ARMORDAYVISIONRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// NIGHT VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 13, BULLET_ARMORNIGHTVISIONRANGEBONUS_X, BULLET_ARMORNIGHTVISIONRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// BRIGHT LIGHT VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 15, BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_X, BULLET_ARMORBRIGHTLIGHTVISIONRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// CAVE VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 16, BULLET_ARMORCAVEVISIONRANGEBONUS_X, BULLET_ARMORCAVEVISIONRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// PERCENT TUNNEL VISION ICON
	if ( Item[ gpItemDescObject->usItem ].percenttunnelvision > 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 19, BULLET_ARMORPERCENTTUNNELVISION_X, BULLET_ARMORPERCENTTUNNELVISION_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// HEARING RANGE BONUS ICON
	if ( GetItemHearingRangeBonus( gpItemDescObject ) != 0 )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 20, BULLET_ARMORHEARINGRANGEBONUS_X, BULLET_ARMORHEARINGRANGEBONUS_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}

void DrawMiscStats( OBJECTTYPE * gpItemDescObject )
{
	INT32	cnt = 0;
	INT32	threshold;

	if( guiCurrentScreen == MAP_SCREEN )
		threshold = 13;
	else
		threshold = 11;
	
	if( UsingEDBSystem() == 0 )
		return;

	// ITEM SIZE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].itemsizebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 0, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}
	
	// RELIABILITY MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].bReliability != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 1, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}
	
	// NOISE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentnoisereduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 2, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// HIDE MUZZLE FLASH ICON
	if ( Item[ gpItemDescObject->usItem ].hidemuzzleflash > 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 3, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// BIPOD ICON
	if ( Item[ gpItemDescObject->usItem ].bipod != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 4, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 5, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// TO-HIT MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 6, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// LASER RANGE ICON
	if ( Item[ gpItemDescObject->usItem ].bestlaserrange > 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 7, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY + 2, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// AIMING MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].aimbonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 8, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// MINIMUM RANGE FOR AIMING ICON
	if ( Item[ gpItemDescObject->usItem ].minrangeforaimbonus > 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 33, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// MAGAZINE SIZE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].magsizebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 17, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// BURST SIZE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].burstsizebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 9, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// BURST TO-HIT MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 10, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// AUTO TO-HIT MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 11, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].APBonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 12, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// BURST AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentburstfireapreduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 13, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// AUTOFIRE AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentautofireapreduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 14, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// READY AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentreadytimeapreduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 15, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// RELOAD AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentreloadtimeapreduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 16, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// SINGLE AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentapreduction != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 18, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// DAMAGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 19, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// MELEE DAMAGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].meleedamagebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 20, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// WOOD CAMO ICON
	if ( Item[ gpItemDescObject->usItem ].camobonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 21, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// URBAN CAMO ICON
	if ( Item[ gpItemDescObject->usItem ].urbanCamobonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 22, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// DESERT CAMO ICON
	if ( Item[ gpItemDescObject->usItem ].desertCamobonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 23, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// SNOW CAMO ICON
	if ( Item[ gpItemDescObject->usItem ].snowCamobonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 24, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// STEALTH MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].stealthbonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 25, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// HEARING RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].hearingrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 26, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// VISION RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 27, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// DAY VISION RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 29, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// NIGHT VISION RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 28, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// BRIGHT LIGHT VISION RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 30, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// CAVE VISION RANGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 31, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	// PERCENT TUNNEL VISION MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percenttunnelvision != 0 && cnt<=threshold )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoMiscIcon, 32, gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}
}

void DrawWeaponValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16	pStr[ 100 ];
	INT16			usX, usY;
	INT16			ubAttackAPs;

	if( UsingEDBSystem() == 0 )
		return;

	// Start with data relevant to all weapons

	// "AP COSTS" header
	SetFontForeground( FONT_MCOLOR_WHITE );
	mprintf( gWeaponStats[ 9 ].sX + gsInvDescX + 2, gWeaponStats[ 9 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 2 ] );
	SetFontForeground( 6 );

	// RELIABILITY
	// equals
	mprintf( gWeaponStats[ 28 ].sX + gsInvDescX, gWeaponStats[ 28 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );
	
	// Calculate ammo reliability
	INT8 iAmmoReliability = 0;
	if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && (*gpItemDescObject)[0]->data.gun.ubGunShotsLeft > 0 )
	{
		iAmmoReliability = Item[( *gpItemDescObject )[0]->data.gun.usGunAmmoItem].bReliability;
	}
	else
	{
		iAmmoReliability = 0;
	}

	INT8 iReliabilityValue = iAmmoReliability + Item[ gpItemDescObject->usItem ].bReliability;

	// Determine color for value print
	SetFontForeground ( 5 );
	if (iReliabilityValue < 0)
	{
		SetFontForeground( ITEMDESC_FONTNEGATIVE );
	}
	else if ( iReliabilityValue > 0 )
	{
		SetFontForeground( ITEMDESC_FONTPOSITIVE );
	}

	// print value
	if ( iReliabilityValue > 0 )
	{
		swprintf( pStr, L"%+2d", iReliabilityValue );
	}
	else
	{
		swprintf( pStr, L"%2d", iReliabilityValue );
	}
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 7 ].sX + gsInvDescX + gWeaponStats[ 7 ].sValDx), (INT16)(gWeaponStats[ 7 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
	mprintf( usX, usY, pStr );
	SetFontForeground( 6 );
	

	// REPAIR EASE
	// equals
	mprintf( gWeaponStats[ 29 ].sX + gsInvDescX, gWeaponStats[ 29 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );
	
	// Determine repair ease
	INT8 iRepairEaseValue = Item[ gpItemDescObject->usItem ].bRepairEase;

	// Determine color for value printing
	SetFontForeground ( 5 );
	if (iRepairEaseValue < 0)
	{
		SetFontForeground( ITEMDESC_FONTNEGATIVE );
	}
	else if ( iRepairEaseValue > 0 )
	{
		SetFontForeground( ITEMDESC_FONTPOSITIVE );
	}

	// print value
	if ( iRepairEaseValue > 0 )
	{
		swprintf( pStr, L"%+2d", iRepairEaseValue );
	}
	else
	{
		swprintf( pStr, L"%2d", iRepairEaseValue );
	}
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 8 ].sX + gsInvDescX + gWeaponStats[ 8 ].sValDx), (INT16)(gWeaponStats[ 8 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
	mprintf( usX, usY, pStr );

	SetFontForeground( 6 );

	// ShotsPer4Turns -> ubAttackAPs, used later for all shot AP values
	ubAttackAPs = BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject );

	// SINGLE SHOT AP
	if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
	{
		// equals			
		mprintf( gWeaponStats[ 32 ].sX + gsInvDescX, gWeaponStats[ 32 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// Determine color for printing value
		if (ubAttackAPs <= EXCEPTIONAL_AP_COST)
		{
			SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
		}
		else
		{
			SetFontForeground( 5 );
		}

		// print value
		swprintf( pStr, L"%2d", ubAttackAPs );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 11 ].sX + gsInvDescX + gWeaponStats[ 11 ].sValDx), (INT16)(gWeaponStats[ 11 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}

	// BURST AP
	if (GetShotsPerBurst(gpItemDescObject)> 0)
	{
		// equals
		mprintf( gWeaponStats[ 33 ].sX + gsInvDescX, gWeaponStats[ 33 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", ubAttackAPs + CalcAPsToBurst( APBPConstants[DEFAULT_APS], gpItemDescObject ) );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 12 ].sX + gsInvDescX + gWeaponStats[ 12 ].sValDx), (INT16)(gWeaponStats[ 12 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}

	// AUTO AP
	if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
	{
		// equals
		mprintf( gWeaponStats[ 34 ].sX + gsInvDescX, gWeaponStats[ 34 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", ubAttackAPs + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], gpItemDescObject, 3 ) );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 13 ].sX + gsInvDescX + gWeaponStats[ 13 ].sValDx), (INT16)(gWeaponStats[ 13 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}

	// PRINT VALUES FOR SPECIFIC WEAPON TYPES

	// Guns and Launchers Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		// ACCURACY
		// equals
		mprintf( gWeaponStats[ 27 ].sX + gsInvDescX, gWeaponStats[ 27 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );			

		// Get accuracy value
		INT8 iAccuracyValue = Weapon[ gpItemDescObject->usItem ].bAccuracy;

		// Determine color for value printing
		SetFontForeground( 5 );
		if (iAccuracyValue < 0)
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAccuracyValue > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}

		// print value
		if ( iAccuracyValue > 0 )
		{
			swprintf( pStr, L"%+2d", iAccuracyValue );
		}
		else
		{
			swprintf( pStr, L"%2d", iAccuracyValue );
		}
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 6 ].sX + gsInvDescX + gWeaponStats[ 6 ].sValDx), (INT16)(gWeaponStats[ 6 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );

		// LOUDNESS
		// equals
		mprintf( gWeaponStats[ 38 ].sX + gsInvDescX, gWeaponStats[ 38 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );			

		// Get loudness value
		INT16 iLoudnessValue = GetFinalLoudness( gpItemDescObject );

		
		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", iLoudnessValue );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 17 ].sX + gsInvDescX + gWeaponStats[ 17 ].sValDx), (INT16)(gWeaponStats[ 17 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}

	// Guns and Multi-use Launchers Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
	{
		// RELOAD AP
		// equals
		mprintf( gWeaponStats[ 35 ].sX + gsInvDescX, gWeaponStats[ 35 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );
		
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", Weapon[ gpItemDescObject->usItem ].APsToReload );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 14 ].sX + gsInvDescX + gWeaponStats[ 14 ].sValDx), (INT16)(gWeaponStats[ 14 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
			
		if ( Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
		{
			// RELOAD MANUALLY AP
			// equals
			mprintf( gWeaponStats[ 36 ].sX + gsInvDescX, gWeaponStats[ 36 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", Weapon[gpItemDescObject->usItem].APsToReloadManually );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 15 ].sX + gsInvDescX + gWeaponStats[ 15 ].sValDx), (INT16)(gWeaponStats[ 15 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		}
	}
		
	// Guns and Grenade Launchers only
	if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem].rocketlauncher )
	{
		// DRAW AP
		// equals
		mprintf( gWeaponStats[ 31 ].sX + gsInvDescX, gWeaponStats[ 31 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// calculate Draw AP
		INT16 iDrawAP = 0;
		iDrawAP = (Weapon[ gpItemDescObject->usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(gpItemDescObject)) / 100);

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", iDrawAP );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 10 ].sX + gsInvDescX + gWeaponStats[ 10 ].sValDx), (INT16)(gWeaponStats[ 10 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}
	
	// Non-Launcher Guns Only
	if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher )
	{

		if ( GetShotsPerBurst(gpItemDescObject)> 0 )
		{
			// BURST PENALTY
			// equals
			mprintf( gWeaponStats[ 25 ].sX + gsInvDescX, gWeaponStats[ 25 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );	

			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", GetBurstPenalty( gpItemDescObject, FALSE ) );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 4 ].sX + gsInvDescX + gWeaponStats[ 4 ].sValDx), (INT16)(gWeaponStats[ 4 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		}
		// Note "else if" here to avoid displaying both types of penalties...

		else if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
		{
			// AUTO PENALTY
			// equals
			mprintf( gWeaponStats[ 25 ].sX + gsInvDescX, gWeaponStats[ 25 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );					
			
			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", GetAutoPenalty( gpItemDescObject, FALSE ) );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 4 ].sX + gsInvDescX + gWeaponStats[ 4 ].sValDx), (INT16)(gWeaponStats[ 4 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		}

		// Note "if" here to display this regardless of BURST PENALTY
		if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
		{
			// AUTOFIRE SHOTS PER 5 AP
			// equals
			mprintf( gWeaponStats[ 26 ].sX + gsInvDescX, gWeaponStats[ 26 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", GetAutofireShotsPerFiveAPs( gpItemDescObject ) );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 5 ].sX + gsInvDescX + gWeaponStats[ 5 ].sValDx), (INT16)(gWeaponStats[ 5 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		
		}
	}
		
	//////////////////////////////////////////////////////////////////////
	// Begin non-weapontype-specific data. These are not type specific because they represent bonuses
	// which may one day be added to various types of weapons by modders...

	if (GetFlatToHitBonus( gpItemDescObject ) != 0)
	{
		// HIT BONUS
		// equals
		mprintf( gWeaponStats[ 39 ].sX + gsInvDescX, gWeaponStats[ 39 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// Determine Color for Value Printing
		SetFontForeground( 5 );
		if (GetFlatToHitBonus ( gpItemDescObject ) < 0)
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if (GetFlatToHitBonus ( gpItemDescObject ) > 0)
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// Print Value
		if ( GetFlatToHitBonus ( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d", GetFlatToHitBonus ( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d", GetFlatToHitBonus ( gpItemDescObject ) );
		}
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 18 ].sX + gsInvDescX + gWeaponStats[ 18 ].sValDx), (INT16)(gWeaponStats[ 18 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );

		if (GetFlatToHitBonus( gpItemDescObject ) > 0 && GetAverageBestLaserRange( gpItemDescObject ) / 10 > 0 )
		{
			// LASER RANGE
			// equals
			mprintf( gWeaponStats[ 40 ].sX + gsInvDescX, gWeaponStats[ 40 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", GetAverageBestLaserRange( gpItemDescObject ) / 10 );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 19 ].sX + gsInvDescX + gWeaponStats[ 19 ].sValDx), (INT16)(gWeaponStats[ 19 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		}	
	}

	if (GetBipodBonus( gpItemDescObject ) != 0)
	{
		// BIPOD BONUS
		// equals
		mprintf( gWeaponStats[ 43 ].sX + gsInvDescX, gWeaponStats[ 43 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if (GetBipodBonus( gpItemDescObject ) < 0)
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if (GetBipodBonus( gpItemDescObject ) > 0)
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}

		if ( GetBipodBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d", GetBipodBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d", GetBipodBonus( gpItemDescObject ) );
		}
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 22 ].sX + gsInvDescX + gWeaponStats[ 22 ].sValDx), (INT16)(gWeaponStats[ 22 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );
	}
	
	if (IsScoped( gpItemDescObject ))
	{
		// SCOPE AIM BONUS
		// equals
		mprintf( gWeaponStats[ 41 ].sX + gsInvDescX, gWeaponStats[ 41 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if (GetFlatAimBonus( gpItemDescObject ) < 0)
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if (GetFlatAimBonus( gpItemDescObject ) > 0)
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}

		if ( GetFlatAimBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d", GetFlatAimBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d", GetFlatAimBonus( gpItemDescObject ) );
		}
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 20 ].sX + gsInvDescX + gWeaponStats[ 20 ].sValDx), (INT16)(gWeaponStats[ 20 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
		SetFontForeground( 6 );


		if (GetMinRangeForAimBonus(gpItemDescObject) > 0) 
		{
			// SCOPE MINIMUM RANGE FOR AIM BONUS
			// equals
			mprintf( gWeaponStats[ 42 ].sX + gsInvDescX, gWeaponStats[ 42 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

			// print value
			SetFontForeground( 5 );
			swprintf( pStr, L"%2d", GetMinRangeForAimBonus( gpItemDescObject )/10 );
			FindFontRightCoordinates( (INT16)(gWeaponStats[ 21 ].sX + gsInvDescX + gWeaponStats[ 21 ].sValDx), (INT16)(gWeaponStats[ 21 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
			SetFontForeground( 6 );
		}
	}
}

void DrawAmmoValues( OBJECTTYPE * gpItemDescObject, int shotsLeft )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				sStrX;
	UINT16				uiStringLength;

	if( UsingEDBSystem() == 0 )
		return;

	// AMMO AMOUNT
	// label
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 0 ].sX + gsInvDescX, gWeaponStats[ 0 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 16 ] );

	// data
	SetFontForeground( 5 );
	swprintf( pStr, L"%d/%d", shotsLeft, Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubMagSize ); //Pulmu: Correct # of rounds for stacked ammo.
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 0 ].sX + gsInvDescX + gWeaponStats[ 0 ].sValDx+6), (INT16)(gWeaponStats[ 0 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	if (UsingEDBSystem() == 2)
		mprintf( sStrX - 6, gWeaponStats[ 0 ].sY + gsInvDescY + 12, pStr );
	else
		mprintf( sStrX, gWeaponStats[ 0 ].sY + gsInvDescY, pStr );

	// AMMO PENETRATION
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 23 ].sX + gsInvDescX -5, gWeaponStats[ 23 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// determine value
	FLOAT fArmourImpactReduction = 0.0;
	fArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionDivisor;
	
	// determine value print color
	SetFontForeground( 5 );
	if ( fArmourImpactReduction > 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTNEGATIVE );
	}
	else if ( fArmourImpactReduction < 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTPOSITIVE );
	}
	
	// print value
	swprintf( pStr, L"%1.2f", fArmourImpactReduction );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 2 ].sX + gsInvDescX + gWeaponStats[ 2 ].sValDx), (INT16)(gWeaponStats[ 2 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	mprintf( sStrX, gWeaponStats[ 2 ].sY + gsInvDescY, pStr );

	// AMMO TUMBLE
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 24 ].sX + gsInvDescX -5, gWeaponStats[ 24 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// determine value
	FLOAT fAfterArmourImpactReduction = 0.0;
	fAfterArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageDivisor;
	
	// determine value print color
	SetFontForeground( 5 );
	if ( fAfterArmourImpactReduction < 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTNEGATIVE );
	}
	else if ( fAfterArmourImpactReduction > 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTPOSITIVE );
	}
	
	// print value
	swprintf( pStr, L"%1.2f", fAfterArmourImpactReduction );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 3 ].sX + gsInvDescX + gWeaponStats[ 3 ].sValDx), (INT16)(gWeaponStats[ 3 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	mprintf( sStrX, gWeaponStats[ 3 ].sY + gsInvDescY, pStr );

	// AMMO PRE-ARMOR EXPLOSION
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 25 ].sX + gsInvDescX -5, gWeaponStats[ 25 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// determine value
	FLOAT fBeforeArmourImpactReduction = 0.0;
	fBeforeArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageDivisor;
	
	// determine value print color
	SetFontForeground( 5 );
	if ( fBeforeArmourImpactReduction < 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTNEGATIVE );
	}
	else if ( fBeforeArmourImpactReduction > 1.0 )
	{
		SetFontForeground( ITEMDESC_FONTPOSITIVE );
	}
	
	// print value
	swprintf( pStr, L"%1.2f", fBeforeArmourImpactReduction );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 4 ].sX + gsInvDescX + gWeaponStats[ 4 ].sValDx), (INT16)(gWeaponStats[ 4 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	mprintf( sStrX, gWeaponStats[ 4 ].sY + gsInvDescY, pStr );

	if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 )
	{
		// AMMO RANGE BONUS
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 37 ].sX + gsInvDescX, gWeaponStats[ 37 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iRangeBonus = 0;
		iRangeBonus = Item[ gpItemDescObject->usItem ].rangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 16 ].sX + gsInvDescX + gWeaponStats[ 16 ].sValDx), (INT16)(gWeaponStats[ 16 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 16 ].sY + gsInvDescY, pStr );
	}

	if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 )
	{
		// AMMO DAMAGE BONUS
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 38 ].sX + gsInvDescX, gWeaponStats[ 38 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iDamageBonus = 0;
		iDamageBonus = Item[ gpItemDescObject->usItem ].damagebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iDamageBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iDamageBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iDamageBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iDamageBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iDamageBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 17 ].sX + gsInvDescX + gWeaponStats[ 17 ].sValDx), (INT16)(gWeaponStats[ 17 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 17 ].sY + gsInvDescY, pStr );
	}

	if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 )
	{
		// AMMO TO-HIT BONUS
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 39 ].sX + gsInvDescX, gWeaponStats[ 39 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iToHitBonus = 0;
		iToHitBonus = Item[ gpItemDescObject->usItem ].tohitbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iToHitBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iToHitBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iToHitBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iToHitBonus );
		}
		else
		{
			swprintf( pStr, L"%2d", iToHitBonus );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 18 ].sX + gsInvDescX + gWeaponStats[ 18 ].sValDx), (INT16)(gWeaponStats[ 18 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 18 ].sY + gsInvDescY, pStr );
	}

	if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 || Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 )
	{
		// AMMO BURST/AUTO BONUS
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 40 ].sX + gsInvDescX, gWeaponStats[ 40 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBurstBonus = 0;
		if ( Item[ gpItemDescObject->usItem ].bursttohitbonus > 0 || Item[ gpItemDescObject->usItem ].autofiretohitbonus > 0)
		{
			iBurstBonus = __max( Item[ gpItemDescObject->usItem ].bursttohitbonus, Item[ gpItemDescObject->usItem ].autofiretohitbonus ) ;
		}
		else
		{	
			iBurstBonus = __min( Item[ gpItemDescObject->usItem ].bursttohitbonus, Item[ gpItemDescObject->usItem ].autofiretohitbonus ) ;
		}
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBurstBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBurstBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBurstBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iBurstBonus );
		}
		else
		{
			swprintf( pStr, L"%2d", iBurstBonus );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 19 ].sX + gsInvDescX + gWeaponStats[ 19 ].sValDx), (INT16)(gWeaponStats[ 19 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 19 ].sY + gsInvDescY, pStr );
	}

	if ( Item[ gpItemDescObject->usItem ].bReliability != 0 )
	{
		// AMMO RELIABILITY BONUS
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 41 ].sX + gsInvDescX, gWeaponStats[ 41 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iReliabilityBonus = 0;
		iReliabilityBonus = Item[ gpItemDescObject->usItem ].bReliability;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iReliabilityBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iReliabilityBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iReliabilityBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iReliabilityBonus );
		}
		else
		{
			swprintf( pStr, L"%2d", iReliabilityBonus );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 20 ].sX + gsInvDescX + gWeaponStats[ 20 ].sValDx), (INT16)(gWeaponStats[ 20 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 20 ].sY + gsInvDescY, pStr );
	}

	if ( Item[ gpItemDescObject->usItem ].percentnoisereduction > 0 )
	{
		// AMMO NOISE SUPPRESSION
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 41 ].sX + gsInvDescX, gWeaponStats[ 41 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iNoiseSuppression = 0;
		iNoiseSuppression = Item[ gpItemDescObject->usItem ].percentnoisereduction * -1;
		
		SetFontForeground( 5 );
		if ( iNoiseSuppression > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iNoiseSuppression < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}

		// print value
		if ( iNoiseSuppression > 0 )
		{
			swprintf( pStr, L"%+2d%%", iNoiseSuppression );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iNoiseSuppression );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 21 ].sX + gsInvDescX + gWeaponStats[ 21 ].sValDx), (INT16)(gWeaponStats[ 21 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 21 ].sY + gsInvDescY, pStr );
	}
}

void DrawExplosiveValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				sStrX;
	UINT16				uiStringLength;

	if( UsingEDBSystem() == 0 )
		return;

	// DAMAGE
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 23 ].sX + gsInvDescX, gWeaponStats[ 23 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage + ( (Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage * gGameExternalOptions.ubExplosivesDamageMultiplier ) / 100 ));
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 2 ].sX + gsInvDescX + gWeaponStats[ 2 ].sValDx), (INT16)(gWeaponStats[ 2 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 2 ].sY + gsInvDescY, pStr );
	}

	// STUN DAMAGE
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 24 ].sX + gsInvDescX, gWeaponStats[ 24 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage  + ( (Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage * gGameExternalOptions.ubExplosivesDamageMultiplier ) / 100 ));
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 3 ].sX + gsInvDescX + gWeaponStats[ 3 ].sValDx), (INT16)(gWeaponStats[ 3 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 3 ].sY + gsInvDescY, pStr );
	}
	
	// LOUDNESS
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 25 ].sX + gsInvDescX, gWeaponStats[ 25 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// print value
	SetFontForeground( 5 );
	swprintf( pStr, L"%2d", Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolume );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 4 ].sX + gsInvDescX + gWeaponStats[ 4 ].sValDx), (INT16)(gWeaponStats[ 4 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	mprintf( sStrX, gWeaponStats[ 4 ].sY + gsInvDescY, pStr );

	// VOLATILITY
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 26 ].sX + gsInvDescX, gWeaponStats[ 26 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 5 ].sX + gsInvDescX + gWeaponStats[ 5 ].sValDx), (INT16)(gWeaponStats[ 5 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 5 ].sY + gsInvDescY, pStr );
	}

	// SINGLE-EVENT EXPLOSION data
	if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 )
	{
		// BLAST RADIUS
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 24 ].sX + gsInvDescX, gWeaponStats[ 24 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%d", 1 + ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubRadius * 2 ) );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontCenterCoordinates( (INT16)(gWeaponStats[ 45 ].sX + gsInvDescX), (INT16)(gWeaponStats[ 45 ].sY + gsInvDescY ), 12 ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 45 ].sY + gsInvDescY, pStr );

	}
	// PROLONGED EXPLOSION data
	else if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
	{
		// DURATION
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 34 ].sX + gsInvDescX, gWeaponStats[ 34 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d", Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 13 ].sX + gsInvDescX + gWeaponStats[ 13 ].sValDx), (INT16)(gWeaponStats[ 13 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 13 ].sY + gsInvDescY, pStr );

		// Headrock: this integer contains the number of the array item that contains the location
		// in which to display each radius value. If the explosive is a light, the values are displayed
		// in reverse locations.

		INT16 isLightLocation = 0;
		
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 4 )
		{
			isLightLocation = 2;
		}

		// STARTING RADIUS
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 33 ].sX + gsInvDescX, gWeaponStats[ 33 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%d", 1 + ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStartRadius * 2) );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontCenterCoordinates( (INT16)(gWeaponStats[ 44+isLightLocation ].sX + gsInvDescX), (INT16)(gWeaponStats[ 44+isLightLocation ].sY + gsInvDescY ), 12 ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 44+isLightLocation ].sY + gsInvDescY, pStr );

		// FINAL RADIUS
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 33 ].sX + gsInvDescX, gWeaponStats[ 33 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%d", 1 + ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubRadius * 2) );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontCenterCoordinates( (INT16)(gWeaponStats[ 46-isLightLocation ].sX + gsInvDescX), (INT16)(gWeaponStats[ 46-isLightLocation ].sY + gsInvDescY ), 12 ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 46-isLightLocation ].sY + gsInvDescY, pStr );

	}
}

void DrawArmorValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				sStrX;
	UINT16				uiStringLength;

	if( UsingEDBSystem() == 0 )
		return;

	// ARMOR VALUE
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 23 ].sX + gsInvDescX, gWeaponStats[ 23 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// print value
	SetFontForeground( 5 );

	// Calculate Value including attachments
	UINT16 iProtection = 0;
	iProtection = Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubProtection;

	for (attachmentList::iterator iter = (*gpItemDescObject)[0]->attachments.begin(); iter != (*gpItemDescObject)[0]->attachments.end(); ++iter) 
	{
		if (Item[ iter->usItem ].usItemClass == IC_ARMOUR)
		{
			iProtection += Armour[Item[iter->usItem].ubClassIndex].ubProtection;
		}
	} 

	swprintf( pStr, L"%2d", iProtection );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 2 ].sX + gsInvDescX + gWeaponStats[ 2 ].sValDx), (INT16)(gWeaponStats[ 2 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	//wcscat( pStr, L"%%" );
	mprintf( sStrX, gWeaponStats[ 2 ].sY + gsInvDescY, pStr );

	// ARMOR COVERAGE
	// equals
	SetFontForeground( 6 );
	//mprintf( gWeaponStats[ 23 ].sX + gsInvDescX, gWeaponStats[ 23 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// print value
	SetFontForeground( 5 );
	swprintf( pStr, L"%2d%%", Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubCoverage );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 3 ].sX + gsInvDescX + gWeaponStats[ 3 ].sValDx), (INT16)(gWeaponStats[ 3 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	wcscat( pStr, L"%%" );
	mprintf( sStrX, gWeaponStats[ 3 ].sY + gsInvDescY, pStr );
	
	// ARMOR DEGRADE
	// equals
	SetFontForeground( 6 );
	mprintf( gWeaponStats[ 25 ].sX + gsInvDescX, gWeaponStats[ 25 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

	// print value
	SetFontForeground( 5 );
	swprintf( pStr, L"%2d", Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDegradePercent );
	uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
	FindFontRightCoordinates( (INT16)(gWeaponStats[ 4 ].sX + gsInvDescX + gWeaponStats[ 4 ].sValDx), (INT16)(gWeaponStats[ 4 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
	mprintf( sStrX, gWeaponStats[ 4 ].sY + gsInvDescY, pStr );

	// AP BONUS
	if ( GetAPBonus ( gpItemDescObject ) != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 30 ].sX + gsInvDescX, gWeaponStats[ 30 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAPBonus = 0;
		iAPBonus = GetAPBonus( gpItemDescObject );
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAPBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAPBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAPBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iAPBonus );
		}
		else
		{
			swprintf( pStr, L"%2d", iAPBonus );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 9 ].sX + gsInvDescX + gWeaponStats[ 9 ].sValDx), (INT16)(gWeaponStats[ 9 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 9 ].sY + gsInvDescY, pStr );
	}

	// AIM BONUS
	if ( GetFlatAimBonus( gpItemDescObject ) != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 31 ].sX + gsInvDescX, gWeaponStats[ 31 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAimBonus = 0;
		iAimBonus = GetFlatAimBonus( gpItemDescObject );
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAimBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAimBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAimBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iAimBonus );
		}
		else
		{
			swprintf( pStr, L"%2d", iAimBonus );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 10 ].sX + gsInvDescX + gWeaponStats[ 10 ].sValDx), (INT16)(gWeaponStats[ 10 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 10 ].sY + gsInvDescY, pStr );
	}

	// WOODLAND CAMO
	if ( GetCamoBonus( gpItemDescObject ) > 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 31 ].sX + gsInvDescX, gWeaponStats[ 31 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if ( GetCamoBonus( gpItemDescObject ) < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( GetCamoBonus( gpItemDescObject ) > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( GetCamoBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d%%", GetCamoBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d%%", GetCamoBonus( gpItemDescObject ) );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 11 ].sX + gsInvDescX + gWeaponStats[ 11 ].sValDx), (INT16)(gWeaponStats[ 11 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 11 ].sY + gsInvDescY, pStr );
	}

	// URBAN CAMO
	if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 32 ].sX + gsInvDescX, gWeaponStats[ 32 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if ( GetUrbanCamoBonus( gpItemDescObject ) < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( GetUrbanCamoBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d%%", GetUrbanCamoBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d%%", GetUrbanCamoBonus( gpItemDescObject ) );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 12 ].sX + gsInvDescX + gWeaponStats[ 12 ].sValDx), (INT16)(gWeaponStats[ 12 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 12 ].sY + gsInvDescY, pStr );
	}

	// DESERT CAMO
	if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 34 ].sX + gsInvDescX, gWeaponStats[ 34 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if ( GetDesertCamoBonus( gpItemDescObject ) < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( GetDesertCamoBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d%%", GetDesertCamoBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d%%", GetDesertCamoBonus( gpItemDescObject ) );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 13 ].sX + gsInvDescX + gWeaponStats[ 13 ].sValDx), (INT16)(gWeaponStats[ 13 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 13 ].sY + gsInvDescY, pStr );
	}

	// SNOW CAMO
	if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 35 ].sX + gsInvDescX, gWeaponStats[ 35 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if ( GetSnowCamoBonus( gpItemDescObject ) < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( GetSnowCamoBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d%%", GetSnowCamoBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d%%", GetSnowCamoBonus( gpItemDescObject ) );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 14 ].sX + gsInvDescX + gWeaponStats[ 14 ].sValDx), (INT16)(gWeaponStats[ 14 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 14 ].sY + gsInvDescY, pStr );
	}

	// STEALTH
	if ( GetBasicStealthBonus( gpItemDescObject ) != 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 35 ].sX + gsInvDescX, gWeaponStats[ 35 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		if ( GetBasicStealthBonus( gpItemDescObject ) < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( GetBasicStealthBonus( gpItemDescObject ) > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( GetBasicStealthBonus( gpItemDescObject ) > 0 )
		{
			swprintf( pStr, L"%+2d%%", GetBasicStealthBonus( gpItemDescObject ) );
		}
		else
		{
			swprintf( pStr, L"%2d%%", GetBasicStealthBonus( gpItemDescObject ) );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 15 ].sX + gsInvDescX + gWeaponStats[ 15 ].sValDx), (INT16)(gWeaponStats[ 15 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 15 ].sY + gsInvDescY, pStr );
	}

	// VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 37 ].sX + gsInvDescX, gWeaponStats[ 37 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeBonus = 0;
		iVisionRangeBonus = Item[ gpItemDescObject->usItem ].visionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 16 ].sX + gsInvDescX + gWeaponStats[ 16 ].sValDx), (INT16)(gWeaponStats[ 16 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 16 ].sY + gsInvDescY, pStr );
	}

	// DAY VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 38 ].sX + gsInvDescX, gWeaponStats[ 38 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeBonus = 0;
		iVisionRangeBonus = Item[ gpItemDescObject->usItem ].dayvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 17 ].sX + gsInvDescX + gWeaponStats[ 17 ].sValDx), (INT16)(gWeaponStats[ 17 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 17 ].sY + gsInvDescY, pStr );
	}

	// NIGHT VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 39 ].sX + gsInvDescX, gWeaponStats[ 39 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeBonus = 0;
		iVisionRangeBonus = Item[ gpItemDescObject->usItem ].nightvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 18 ].sX + gsInvDescX + gWeaponStats[ 18 ].sValDx), (INT16)(gWeaponStats[ 18 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 18 ].sY + gsInvDescY, pStr );
	}

	// BRIGHT LIGHT VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 40 ].sX + gsInvDescX, gWeaponStats[ 40 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeBonus = 0;
		iVisionRangeBonus = Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 19 ].sX + gsInvDescX + gWeaponStats[ 19 ].sValDx), (INT16)(gWeaponStats[ 19 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 19 ].sY + gsInvDescY, pStr );
	}

	// CAVE VISION RANGE BONUS ICON
	if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 41 ].sX + gsInvDescX, gWeaponStats[ 41 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeBonus = 0;
		iVisionRangeBonus = Item[ gpItemDescObject->usItem ].cavevisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 20 ].sX + gsInvDescX + gWeaponStats[ 20 ].sValDx), (INT16)(gWeaponStats[ 20 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 20 ].sY + gsInvDescY, pStr );
	}

	// PERCENT TUNNEL VISION ICON
	if ( Item[ gpItemDescObject->usItem ].percenttunnelvision > 0 )
	{
		// equals
		SetFontForeground( 6 );
		//mprintf( gWeaponStats[ 34 ].sX + gsInvDescX, gWeaponStats[ 34 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// print value
		SetFontForeground( 5 );
		swprintf( pStr, L"%2d%%", Item[ gpItemDescObject->usItem ].percenttunnelvision );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 21 ].sX + gsInvDescX + gWeaponStats[ 21 ].sValDx), (INT16)(gWeaponStats[ 21 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gWeaponStats[ 21 ].sY + gsInvDescY, pStr );
	}

	// HEARING RANGE BONUS ICON
	if ( GetItemHearingRangeBonus( gpItemDescObject ) != 0 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gWeaponStats[ 42 ].sX + gsInvDescX, gWeaponStats[ 42 ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iHearingRangeBonus = 0;
		iHearingRangeBonus = GetItemHearingRangeBonus( gpItemDescObject );
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iHearingRangeBonus < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iHearingRangeBonus > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iHearingRangeBonus > 0 )
		{
			swprintf( pStr, L"%+2d", iHearingRangeBonus / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iHearingRangeBonus / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gWeaponStats[ 22 ].sX + gsInvDescX + gWeaponStats[ 22 ].sValDx), (INT16)(gWeaponStats[ 22 ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gWeaponStats[ 22 ].sY + gsInvDescY, pStr );
	}
}

void DrawMiscValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				sStrX;
	UINT16				uiStringLength;
	UINT8				cnt;

	if( UsingEDBSystem() == 0 )
		return;

	if(guiCurrentScreen ==MAP_SCREEN)
		cnt = 14;
	else
		cnt = 12;

	// ITEM SIZE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].itemsizebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iItemSizeModifier = 0;
		iItemSizeModifier = Item[ gpItemDescObject->usItem ].itemsizebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iItemSizeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iItemSizeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iItemSizeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iItemSizeModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iItemSizeModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}
	
	// RELIABILITY MODIFIER
	if ( Item[ gpItemDescObject->usItem ].bReliability != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iReliabilityModifier = 0;
		iReliabilityModifier = Item[ gpItemDescObject->usItem ].bReliability;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iReliabilityModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iReliabilityModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iReliabilityModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iReliabilityModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iReliabilityModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}
	
	// NOISE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].percentnoisereduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iLoudnessModifier = 0;
		iLoudnessModifier = Item[ gpItemDescObject->usItem ].percentnoisereduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iLoudnessModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iLoudnessModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iLoudnessModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iLoudnessModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iLoudnessModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// HIDE MUZZLE FLASH
	// NO DATA SHOWN, icon either appears or does not appear.

	// BIPOD
	if ( Item[ gpItemDescObject->usItem ].bipod != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBipodModifier = 0;
		iBipodModifier = Item[ gpItemDescObject->usItem ].bipod;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBipodModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBipodModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBipodModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iBipodModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iBipodModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// RANGE
	if ( Item[ gpItemDescObject->usItem ].rangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iRangeModifier = 0;
		iRangeModifier = Item[ gpItemDescObject->usItem ].rangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// TO-HIT MODIFIER
	if ( Item[ gpItemDescObject->usItem ].tohitbonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iToHitModifier = 0;
		iToHitModifier = Item[ gpItemDescObject->usItem ].tohitbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iToHitModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iToHitModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iToHitModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iToHitModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iToHitModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// LASER RANGE
	if ( Item[ gpItemDescObject->usItem ].bestlaserrange > 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iLaserRange = 0;
		iLaserRange = Item[ gpItemDescObject->usItem ].bestlaserrange;
		
		// determine value print color
		SetFontForeground( 5 );
		
		// print value
		swprintf( pStr, L"%2d", iLaserRange / 10 );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// AIMING MODIFIER
	if ( Item[ gpItemDescObject->usItem ].aimbonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAimModifier = 0;
		iAimModifier = Item[ gpItemDescObject->usItem ].aimbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAimModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAimModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAimModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iAimModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iAimModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// MINIMUM RANGE FOR AIMING BONUS
	if ( Item[ gpItemDescObject->usItem ].minrangeforaimbonus > 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iMinRangeAimingBonus = 0;
		iMinRangeAimingBonus = Item[ gpItemDescObject->usItem ].minrangeforaimbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		
		// print value
		swprintf( pStr, L"%2d", iMinRangeAimingBonus / 10 );
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// MAGAZINE SIZE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].magsizebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iMagSizeModifier = 0;
		iMagSizeModifier = Item[ gpItemDescObject->usItem ].magsizebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iMagSizeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iMagSizeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iMagSizeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iMagSizeModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iMagSizeModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// BURST SIZE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].burstsizebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBurstSizeModifier = 0;
		iBurstSizeModifier = Item[ gpItemDescObject->usItem ].burstsizebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBurstSizeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBurstSizeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBurstSizeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iBurstSizeModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iBurstSizeModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// BURST TO-HIT MODIFIER
	if ( Item[ gpItemDescObject->usItem ].bursttohitbonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBurstToHitModifier = 0;
		iBurstToHitModifier = Item[ gpItemDescObject->usItem ].bursttohitbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBurstToHitModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBurstToHitModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBurstToHitModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iBurstToHitModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iBurstToHitModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// AUTO TO-HIT MODIFIER
	if ( Item[ gpItemDescObject->usItem ].autofiretohitbonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAutoToHitModifier = 0;
		iAutoToHitModifier = Item[ gpItemDescObject->usItem ].autofiretohitbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAutoToHitModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAutoToHitModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAutoToHitModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iAutoToHitModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iAutoToHitModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// AP MODIFIER
	if ( Item[ gpItemDescObject->usItem ].APBonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAPModifier = 0;
		iAPModifier = Item[ gpItemDescObject->usItem ].APBonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// BURST AP MODIFIER
	if ( Item[ gpItemDescObject->usItem ].percentburstfireapreduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBurstAPModifier = 0;
		iBurstAPModifier = Item[ gpItemDescObject->usItem ].percentburstfireapreduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBurstAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBurstAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBurstAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iBurstAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iBurstAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// AUTOFIRE AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentautofireapreduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iAutoAPModifier = 0;
		iAutoAPModifier = Item[ gpItemDescObject->usItem ].percentautofireapreduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iAutoAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iAutoAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iAutoAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iAutoAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iAutoAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// READY AP MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].percentreadytimeapreduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iReadyAPModifier = 0;
		iReadyAPModifier = Item[ gpItemDescObject->usItem ].percentreadytimeapreduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iReadyAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iReadyAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iReadyAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iReadyAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iReadyAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// RELOAD AP MODIFIER
	if ( Item[ gpItemDescObject->usItem ].percentreloadtimeapreduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iReloadAPModifier = 0;
		iReloadAPModifier = Item[ gpItemDescObject->usItem ].percentreloadtimeapreduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iReloadAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iReloadAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iReloadAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iReloadAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iReloadAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// SINGLE AP MODIFIER
	if ( Item[ gpItemDescObject->usItem ].percentapreduction != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iSingleAPModifier = 0;
		iSingleAPModifier = Item[ gpItemDescObject->usItem ].percentapreduction * -1 ;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iSingleAPModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iSingleAPModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iSingleAPModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iSingleAPModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iSingleAPModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// DAMAGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].damagebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iDamageModifier = 0;
		iDamageModifier = Item[ gpItemDescObject->usItem ].damagebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iDamageModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iDamageModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iDamageModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iDamageModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iDamageModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// MELEE DAMAGE MODIFIER ICON
	if ( Item[ gpItemDescObject->usItem ].meleedamagebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iMeleeDamageModifier = 0;
		iMeleeDamageModifier = Item[ gpItemDescObject->usItem ].meleedamagebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iMeleeDamageModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iMeleeDamageModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iMeleeDamageModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iMeleeDamageModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iMeleeDamageModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// WOOD CAMO
	if ( Item[ gpItemDescObject->usItem ].camobonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iWoodCamoModifier = 0;
		iWoodCamoModifier = Item[ gpItemDescObject->usItem ].camobonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iWoodCamoModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iWoodCamoModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iWoodCamoModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iWoodCamoModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iWoodCamoModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// URBAN CAMO
	if ( Item[ gpItemDescObject->usItem ].urbanCamobonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iUrbanCamoModifier = 0;
		iUrbanCamoModifier = Item[ gpItemDescObject->usItem ].urbanCamobonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iUrbanCamoModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iUrbanCamoModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iUrbanCamoModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iUrbanCamoModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iUrbanCamoModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// DESERT CAMO
	if ( Item[ gpItemDescObject->usItem ].desertCamobonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iDesertCamoModifier = 0;
		iDesertCamoModifier = Item[ gpItemDescObject->usItem ].desertCamobonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iDesertCamoModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iDesertCamoModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iDesertCamoModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iDesertCamoModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iDesertCamoModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// SNOW CAMO
	if ( Item[ gpItemDescObject->usItem ].snowCamobonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iSnowCamoModifier = 0;
		iSnowCamoModifier = Item[ gpItemDescObject->usItem ].snowCamobonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iSnowCamoModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iSnowCamoModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iSnowCamoModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iSnowCamoModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iSnowCamoModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// STEALTH MODIFIER
	if ( Item[ gpItemDescObject->usItem ].stealthbonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iStealthModifier = 0;
		iStealthModifier = Item[ gpItemDescObject->usItem ].stealthbonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iStealthModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iStealthModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iStealthModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iStealthModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iStealthModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// HEARING RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].hearingrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iHearingRangeModifier = 0;
		iHearingRangeModifier = Item[ gpItemDescObject->usItem ].hearingrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iHearingRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iHearingRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iHearingRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iHearingRangeModifier );
		}
		else
		{
			swprintf( pStr, L"%2d", iHearingRangeModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// VISION RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].visionrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iVisionRangeModifier = 0;
		iVisionRangeModifier = Item[ gpItemDescObject->usItem ].visionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iVisionRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iVisionRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iVisionRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iVisionRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iVisionRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// DAY VISION RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].dayvisionrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iDayVisionRangeModifier = 0;
		iDayVisionRangeModifier = Item[ gpItemDescObject->usItem ].dayvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iDayVisionRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iDayVisionRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iDayVisionRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iDayVisionRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iDayVisionRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// NIGHT VISION RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].nightvisionrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iNightVisionRangeModifier = 0;
		iNightVisionRangeModifier = Item[ gpItemDescObject->usItem ].nightvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iNightVisionRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iNightVisionRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iNightVisionRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iNightVisionRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iNightVisionRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// BRIGHT LIGHT VISION RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iBrightLightVisionRangeModifier = 0;
		iBrightLightVisionRangeModifier = Item[ gpItemDescObject->usItem ].brightlightvisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iBrightLightVisionRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iBrightLightVisionRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iBrightLightVisionRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iBrightLightVisionRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iBrightLightVisionRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// CAVE VISION RANGE MODIFIER
	if ( Item[ gpItemDescObject->usItem ].cavevisionrangebonus != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iCaveVisionRangeModifier = 0;
		iCaveVisionRangeModifier = Item[ gpItemDescObject->usItem ].cavevisionrangebonus;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iCaveVisionRangeModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iCaveVisionRangeModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iCaveVisionRangeModifier > 0 )
		{
			swprintf( pStr, L"%+2d", iCaveVisionRangeModifier / 10 );
		}
		else
		{
			swprintf( pStr, L"%2d", iCaveVisionRangeModifier / 10 );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}

	// PERCENT TUNNEL VISION MODIFIER
	if ( Item[ gpItemDescObject->usItem ].percenttunnelvision != 0 && cnt<=27 )
	{
		// equals
		SetFontForeground( 6 );
		mprintf( gMiscItemStatsEDB[ cnt ].sX + gsInvDescX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, L"%s", gWeaponStatsDesc[ 8 ] );

		// determine value
		INT16 iPercentTunnelVisionModifier = 0;
		iPercentTunnelVisionModifier = Item[ gpItemDescObject->usItem ].percenttunnelvision;
		
		// determine value print color
		SetFontForeground( 5 );
		if ( iPercentTunnelVisionModifier > 0 )
		{
			SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( iPercentTunnelVisionModifier < 0 )
		{
			SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		
		// print value
		if ( iPercentTunnelVisionModifier > 0 )
		{
			swprintf( pStr, L"%+2d%%", iPercentTunnelVisionModifier );
		}
		else
		{
			swprintf( pStr, L"%2d%%", iPercentTunnelVisionModifier );
		}
		uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
		FindFontRightCoordinates( (INT16)(gMiscItemStatsEDB[ cnt ].sX + gsInvDescX + gMiscItemStatsEDB[ cnt ].sValDx), (INT16)(gMiscItemStatsEDB[ cnt ].sY + gsInvDescY ), ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &sStrX, &usY);
		wcscat( pStr, L"%%" );
		mprintf( sStrX, gMiscItemStatsEDB[ cnt ].sY + gsInvDescY, pStr );
		cnt++;
	}
}

