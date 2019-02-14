#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "language defines.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
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
	#include "Font Control.h"
	#include "utilities.h"
	#include "Interface Panels.h"
	#include "Animation Control.h"
	#include "Soldier Control.h"
	#include "weapons.h"
	#include "lighting.h"
	#include "faces.h"
	#include "mapscreen.h"
	#include "message.h"
	#include "text.h"
	#include "Interface Items.h"
	#include "Font Control.h"
	#include "Cursor Control.h"
	#include "interface utils.h"
	#include "interface items.h"
	#include "wordwrap.h"
	#include "vobject_blitters.h"
	#include "world items.h"
	#include "points.h"
	#include "physics.h"
	#include "handle ui.h"
	#include "ShopKeeper Interface.h"
	#include "english.h"
	#include "keys.h"
	#include "Strategicmap.h"
	#include "soldier macros.h"
	#include "squads.h"
	#include "MessageBoxScreen.h"
	#include "Language Defines.h"
	#include "GameSettings.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Quests.h"
	#include "Map Screen Interface.h"
	#include "Campaign Types.h"
	#include "los.h"
	#include "Map Screen Interface Map.h"
	#include "Food.h"	// added by Flugente
#endif

#include "Multi Language Graphic Utils.h"

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
//UINT32		guiItemInfoMiscIcon;
// HEADROCK HAM 4: Image for secondary icons
UINT32		guiItemInfoSecondaryIcon;
// HEADROCK HAM 4: This replaces "Misc" icons.
UINT32		guiItemInfoAdvancedIcon;
// Flugente: added icons for WH40K
UINT32		guiItemInfoWH40KIcon;

// HEADROCK HAM 4: New tooltip regions for UDB replace the old regions.
#define NUM_UDB_FASTHELP_REGIONS 103
MOUSE_REGION		gUDBFasthelpRegions[NUM_UDB_FASTHELP_REGIONS];

extern INV_DESC_STATS gMoneyStats[];
extern INV_DESC_STATS gLBEStats[];
extern INV_ATTACHXY gItemDescAttachmentsXY[];
extern SOLDIERTYPE *gpItemDescSoldier;
// HEADROCK HAM 4: Declare here so other functions may use it.
void DrawSecondaryStats( OBJECTTYPE * gpItemDescObject );
void DrawAdvancedStats( OBJECTTYPE * gpItemDescObject );
void DrawAdvancedValues( OBJECTTYPE *gpItemDescObject );

extern void HelpTextDoneCallback( void );

UINT8 UsingEDBSystem()
{
	if (iResolution >= _640x480 && iResolution < _800x600)	//640x480, never allow EDB
		return 0;

	if(gGameSettings.fOptions[ TOPTION_ENHANCED_DESC_BOX ] == TRUE)
	{
		if(guiCurrentScreen == MAP_SCREEN)	//Strategic
		{
			if(gGameExternalOptions.iEnhancedDescriptionBox == 0 || gGameExternalOptions.iEnhancedDescriptionBox == 1 )
				return 1;
		}
		else	//Tactical
		{
			// HEADROCK HAM 4: With the introduction of UDB, it is no longer possible to display an extended
			// description box for OIV (unless the interface panel is increased... which I doubt it will be).
			if (UsingNewInventorySystem()==false)
				return 0;
			if(gGameExternalOptions.iEnhancedDescriptionBox == 0 || gGameExternalOptions.iEnhancedDescriptionBox == 2)
				return 1;
		}
	}

	return 0;
}

void InitEDBCoords(OBJECTTYPE * pObject)
{
	if (UsingEDBSystem() > 0)
	{
		//////////////////////////////////////////////////////////////
		// UDB Offsets.
		// These offsets allow us to efficiently render UDB in two different places (for Strategic
		// and Tactical) without changing anything else. This is thanks to UDB being almost exactly
		// the same interface in both screens!

		INT16 UDBOffsetX;
		INT16 UDBOffsetY;
		
		UDBOffsetX = gsInvDescX;
		UDBOffsetY = gsInvDescY;

		// Adjust for NAS
		if(UsingNewAttachmentSystem() == true)
		{
			UDBOffsetX += sNASXCorrection(pObject);
			UDBOffsetY += sNASYCorrection(pObject);
		}
		// If no NAS, adjust tactical screen coordinates separately.
		if (guiCurrentScreen != MAP_SCREEN)
		{
			UDBOffsetX += 224;
			UDBOffsetY += -114;
			if (UseNASDesc( pObject ))
			{
				UDBOffsetX -= 128;
			}
		}

		///////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB LBE background coordinates
		// These depend on Strategic/Tactical...
		if (guiCurrentScreen == MAP_SCREEN)
		{
			gItemDescLBEBackground[0].sLeft = 58 + UDBOffsetX;	gItemDescLBEBackground[0].sTop = 366 + UDBOffsetY;
			gItemDescLBEBackground[1].sLeft = 15 + UDBOffsetX;	gItemDescLBEBackground[1].sTop = 416 + UDBOffsetY;
			gItemDescLBEBackground[2].sLeft = 59 + UDBOffsetX;	gItemDescLBEBackground[2].sTop = 340 + UDBOffsetY;
			gItemDescLBEBackground[3].sLeft = 25 + UDBOffsetX;	gItemDescLBEBackground[3].sTop = 346 + UDBOffsetY;
		}
		else
		{
			gItemDescLBEBackground[0].sLeft = 251 + UDBOffsetX;	gItemDescLBEBackground[0].sTop = 121 + UDBOffsetY;
			gItemDescLBEBackground[1].sLeft = 251 + UDBOffsetX;	gItemDescLBEBackground[1].sTop = 121 + UDBOffsetY;
			gItemDescLBEBackground[2].sLeft = 251 + UDBOffsetX;	gItemDescLBEBackground[2].sTop = 121 + UDBOffsetY;
			gItemDescLBEBackground[3].sLeft = 251 + UDBOffsetX;	gItemDescLBEBackground[3].sTop = 121 + UDBOffsetY;
		}

		///////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB Description Page

		// Full item description
		gItemDescTextRegions[0].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[0].sTop = 128 + UDBOffsetY;		gItemDescTextRegions[0].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[0].sBottom = 200 + UDBOffsetY;

		// Condition
		gItemDescTextRegions[1].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[1].sTop = 230 + UDBOffsetY;		gItemDescTextRegions[1].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[1].sBottom = 238 + UDBOffsetY;

		// Weight
		gItemDescTextRegions[2].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[2].sTop = 242 + UDBOffsetY;		gItemDescTextRegions[2].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[2].sBottom = 250 + UDBOffsetY;

		// PROS label
		gItemDescTextRegions[3].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[3].sTop = 280 + UDBOffsetY;		gItemDescTextRegions[3].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[3].sBottom = 288 + UDBOffsetY;

		// PROS text
		gItemDescTextRegions[4].sLeft = 50 + UDBOffsetX;		gItemDescTextRegions[4].sTop = 280 + UDBOffsetY;		gItemDescTextRegions[4].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[4].sBottom = 288 + UDBOffsetY;

		// CONS label
		gItemDescTextRegions[5].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[5].sTop = 292 + UDBOffsetY;		gItemDescTextRegions[5].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[5].sBottom = 300 + UDBOffsetY;

		// CONS text
		gItemDescTextRegions[6].sLeft = 50 + UDBOffsetX;		gItemDescTextRegions[6].sTop = 292 + UDBOffsetY;		gItemDescTextRegions[6].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[6].sBottom = 300 + UDBOffsetY;

		// silversurfer: spread patterns. Is displayed above the Pros label.
		gItemDescTextRegions[7].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[7].sTop = 254 + UDBOffsetY;		gItemDescTextRegions[7].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[7].sBottom = 262 + UDBOffsetY;

		// Flugente FTW 1: Temperature. Is displayed between Weight and the PROS label, so the indexes should be moved... but it is unclear if this description is needed, anyway
		gItemDescTextRegions[8].sLeft = 20 + UDBOffsetX;		gItemDescTextRegions[8].sTop = 266 + UDBOffsetY;		gItemDescTextRegions[8].sRight = 230 + UDBOffsetX;	gItemDescTextRegions[8].sBottom = 274 + UDBOffsetY;

		////////////////// MONEY TEXT COORDINATES
		gMoneyStats[0].sX = 20 + UDBOffsetX;		gMoneyStats[0].sY = 230 + UDBOffsetY;	// Money
		gMoneyStats[1].sX = 63 + UDBOffsetX;		gMoneyStats[1].sY = 230 + UDBOffsetY;	// Remaining
		gMoneyStats[2].sX = 20 + UDBOffsetX;		gMoneyStats[2].sY = 245 + UDBOffsetY;	// Money
		gMoneyStats[3].sX = 63 + UDBOffsetX;		gMoneyStats[3].sY = 245 + UDBOffsetY;	// Removing
		gMoneyStats[4].sX = 145 + UDBOffsetX;		gMoneyStats[4].sY = 230 + UDBOffsetY;	// AMT remaining
		gMoneyStats[5].sX = 145 + UDBOffsetX;		gMoneyStats[5].sY = 245 + UDBOffsetY;	// AMT removing

		///////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB General Page - Header locations

		// "Primary" header
		gItemDescGenHeaderRegions[0].sLeft = 12 + UDBOffsetX;	gItemDescGenHeaderRegions[0].sTop = 119 + UDBOffsetY;	gItemDescGenHeaderRegions[0].sRight = 126 + UDBOffsetX;	gItemDescGenHeaderRegions[0].sBottom = 131 + UDBOffsetY;

		// "Secondary" header (for weapons: "AP costs")
		gItemDescGenHeaderRegions[1].sLeft = 128 + UDBOffsetX;	gItemDescGenHeaderRegions[1].sTop = 119 + UDBOffsetY;	gItemDescGenHeaderRegions[1].sRight = 242 + UDBOffsetX;	gItemDescGenHeaderRegions[1].sBottom = 131 + UDBOffsetY;

		// "Burst / Auto" header (for weapons only)
		gItemDescGenHeaderRegions[2].sLeft = 128 + UDBOffsetX;	gItemDescGenHeaderRegions[2].sTop = 230 + UDBOffsetY;	gItemDescGenHeaderRegions[2].sRight = 242 + UDBOffsetX;	gItemDescGenHeaderRegions[2].sBottom = 242 + UDBOffsetY;

		///////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB General Page - Index Locations

		// "Primary" index
		gItemDescGenIndexRegions[0][0].sLeft = 12 + UDBOffsetX;		gItemDescGenIndexRegions[0][0].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[0][0].sRight = 61 + UDBOffsetX;	gItemDescGenIndexRegions[0][0].sBottom = 146 + UDBOffsetY;	
		gItemDescGenIndexRegions[0][1].sLeft = 63 + UDBOffsetX;		gItemDescGenIndexRegions[0][1].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[0][1].sRight = 84 + UDBOffsetX;	gItemDescGenIndexRegions[0][1].sBottom = 146 + UDBOffsetY;
		gItemDescGenIndexRegions[0][2].sLeft = 86 + UDBOffsetX;		gItemDescGenIndexRegions[0][2].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[0][2].sRight = 105 + UDBOffsetX;	gItemDescGenIndexRegions[0][2].sBottom = 146 + UDBOffsetY;
		gItemDescGenIndexRegions[0][3].sLeft = 107 + UDBOffsetX;	gItemDescGenIndexRegions[0][3].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[0][3].sRight = 127 + UDBOffsetX;	gItemDescGenIndexRegions[0][3].sBottom = 146 + UDBOffsetY;

		// "Secondary" index
		gItemDescGenIndexRegions[1][0].sLeft = 130 + UDBOffsetX;	gItemDescGenIndexRegions[1][0].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[1][0].sRight = 179 + UDBOffsetX;	gItemDescGenIndexRegions[1][0].sBottom = 146 + UDBOffsetY;	
		gItemDescGenIndexRegions[1][1].sLeft = 181 + UDBOffsetX;	gItemDescGenIndexRegions[1][1].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[1][1].sRight = 202 + UDBOffsetX;	gItemDescGenIndexRegions[1][1].sBottom = 146 + UDBOffsetY;
		gItemDescGenIndexRegions[1][2].sLeft = 204 + UDBOffsetX;	gItemDescGenIndexRegions[1][2].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[1][2].sRight = 223 + UDBOffsetX;	gItemDescGenIndexRegions[1][2].sBottom = 146 + UDBOffsetY;
		gItemDescGenIndexRegions[1][3].sLeft = 225 + UDBOffsetX;	gItemDescGenIndexRegions[1][3].sTop = 134 + UDBOffsetY;	gItemDescGenIndexRegions[1][3].sRight = 245 + UDBOffsetX;	gItemDescGenIndexRegions[1][3].sBottom = 146 + UDBOffsetY;

		// "Tertiary" index (for weapons only)
		gItemDescGenIndexRegions[2][0].sLeft = 130 + UDBOffsetX;	gItemDescGenIndexRegions[2][0].sTop = 245 + UDBOffsetY;	gItemDescGenIndexRegions[2][0].sRight = 179 + UDBOffsetX;	gItemDescGenIndexRegions[2][0].sBottom = 257 + UDBOffsetY;	
		gItemDescGenIndexRegions[2][1].sLeft = 181 + UDBOffsetX;	gItemDescGenIndexRegions[2][1].sTop = 245 + UDBOffsetY;	gItemDescGenIndexRegions[2][1].sRight = 202 + UDBOffsetX;	gItemDescGenIndexRegions[2][1].sBottom = 257 + UDBOffsetY;
		gItemDescGenIndexRegions[2][2].sLeft = 204 + UDBOffsetX;	gItemDescGenIndexRegions[2][2].sTop = 245 + UDBOffsetY;	gItemDescGenIndexRegions[2][2].sRight = 223 + UDBOffsetX;	gItemDescGenIndexRegions[2][2].sBottom = 257 + UDBOffsetY;
		gItemDescGenIndexRegions[2][3].sLeft = 225 + UDBOffsetX;	gItemDescGenIndexRegions[2][3].sTop = 245 + UDBOffsetY;	gItemDescGenIndexRegions[2][3].sRight = 245 + UDBOffsetX;	gItemDescGenIndexRegions[2][3].sBottom = 257 + UDBOffsetY;

		//////////////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB General Page - Data locations
		// There are NUM_UDB_GEN_LINES*2 lines on the UDB page. Half are on the left side, half
		// on the right. Each line has 4 individual parts, see the indexes above.
		// The right column is currently only used for WEAPONS. Other item types have their
		// own coordinate arrays as they are used to display non-tabulated information.

		// Left Column, row 1
		gItemDescGenRegions[0][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[0][0].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[0][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[0][0].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[0][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[0][1].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[0][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[0][1].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[0][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[0][2].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[0][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[0][2].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[0][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[0][3].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[0][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[0][3].sBottom = 162 + UDBOffsetY;

		// Left Column, row 2
		gItemDescGenRegions[1][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[1][0].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[1][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[1][0].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[1][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[1][1].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[1][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[1][1].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[1][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[1][2].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[1][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[1][2].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[1][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[1][3].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[1][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[1][3].sBottom = 175 + UDBOffsetY;

		// Left Column, row 3
		gItemDescGenRegions[2][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[2][0].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[2][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[2][0].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[2][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[2][1].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[2][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[2][1].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[2][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[2][2].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[2][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[2][2].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[2][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[2][3].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[2][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[2][3].sBottom = 188 + UDBOffsetY;

		// Left Column, row 4
		gItemDescGenRegions[3][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[3][0].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[3][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[3][0].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[3][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[3][1].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[3][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[3][1].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[3][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[3][2].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[3][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[3][2].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[3][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[3][3].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[3][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[3][3].sBottom = 201 + UDBOffsetY;

		// Left Column, row 5
		gItemDescGenRegions[4][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[4][0].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[4][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[4][0].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[4][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[4][1].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[4][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[4][1].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[4][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[4][2].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[4][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[4][2].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[4][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[4][3].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[4][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[4][3].sBottom = 214 + UDBOffsetY;

		// Left Column, row 6
		gItemDescGenRegions[5][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[5][0].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[5][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[5][0].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[5][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[5][1].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[5][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[5][1].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[5][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[5][2].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[5][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[5][2].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[5][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[5][3].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[5][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[5][3].sBottom = 227 + UDBOffsetY;

		// Left Column, row 7
		gItemDescGenRegions[6][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[6][0].sTop = 228 + UDBOffsetY;	gItemDescGenRegions[6][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[6][0].sBottom = 242 + UDBOffsetY;	
		gItemDescGenRegions[6][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[6][1].sTop = 228 + UDBOffsetY;	gItemDescGenRegions[6][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[6][1].sBottom = 242 + UDBOffsetY;	
		gItemDescGenRegions[6][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[6][2].sTop = 228 + UDBOffsetY;	gItemDescGenRegions[6][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[6][2].sBottom = 242 + UDBOffsetY;	
		gItemDescGenRegions[6][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[6][3].sTop = 228 + UDBOffsetY;	gItemDescGenRegions[6][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[6][3].sBottom = 242 + UDBOffsetY;

		// Left Column, row 8
		gItemDescGenRegions[7][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[7][0].sTop = 243 + UDBOffsetY;	gItemDescGenRegions[7][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[7][0].sBottom = 255 + UDBOffsetY;	
		gItemDescGenRegions[7][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[7][1].sTop = 243 + UDBOffsetY;	gItemDescGenRegions[7][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[7][1].sBottom = 255 + UDBOffsetY;	
		gItemDescGenRegions[7][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[7][2].sTop = 243 + UDBOffsetY;	gItemDescGenRegions[7][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[7][2].sBottom = 255 + UDBOffsetY;	
		gItemDescGenRegions[7][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[7][3].sTop = 243 + UDBOffsetY;	gItemDescGenRegions[7][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[7][3].sBottom = 255 + UDBOffsetY;

		// Left Column, row 9
		gItemDescGenRegions[8][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[8][0].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[8][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[8][0].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[8][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[8][1].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[8][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[8][1].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[8][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[8][2].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[8][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[8][2].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[8][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[8][3].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[8][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[8][3].sBottom = 268 + UDBOffsetY;

		// Left Column, row 10
		gItemDescGenRegions[9][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[9][0].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[9][0].sRight = 61 + UDBOffsetX;		gItemDescGenRegions[9][0].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[9][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[9][1].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[9][1].sRight = 84 + UDBOffsetX;		gItemDescGenRegions[9][1].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[9][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[9][2].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[9][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[9][2].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[9][3].sLeft = 107 + UDBOffsetX;		gItemDescGenRegions[9][3].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[9][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[9][3].sBottom = 281 + UDBOffsetY;

		// Left Column, row 11
		gItemDescGenRegions[10][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[10][0].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[10][0].sRight = 61 + UDBOffsetX;	gItemDescGenRegions[10][0].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[10][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[10][1].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[10][1].sRight = 84 + UDBOffsetX;	gItemDescGenRegions[10][1].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[10][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[10][2].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[10][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[10][2].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[10][3].sLeft = 107 + UDBOffsetX;	gItemDescGenRegions[10][3].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[10][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[10][3].sBottom = 294 + UDBOffsetY;

		// Left Column, row 12
		gItemDescGenRegions[11][0].sLeft = 12 + UDBOffsetX;		gItemDescGenRegions[11][0].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[11][0].sRight = 61 + UDBOffsetX;	gItemDescGenRegions[11][0].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[11][1].sLeft = 63 + UDBOffsetX;		gItemDescGenRegions[11][1].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[11][1].sRight = 84 + UDBOffsetX;	gItemDescGenRegions[11][1].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[11][2].sLeft = 86 + UDBOffsetX;		gItemDescGenRegions[11][2].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[11][2].sRight = 105 + UDBOffsetX;	gItemDescGenRegions[11][2].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[11][3].sLeft = 107 + UDBOffsetX;	gItemDescGenRegions[11][3].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[11][3].sRight = 127 + UDBOffsetX;	gItemDescGenRegions[11][3].sBottom = 307 + UDBOffsetY;

		// Right Column, row 1
		gItemDescGenRegions[13][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[13][0].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[13][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[13][0].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[13][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[13][1].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[13][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[13][1].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[13][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[13][2].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[13][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[13][2].sBottom = 162 + UDBOffsetY;	
		gItemDescGenRegions[13][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[13][3].sTop = 150 + UDBOffsetY;	gItemDescGenRegions[13][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[13][3].sBottom = 162 + UDBOffsetY;

		// Right Column, row 2
		gItemDescGenRegions[14][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[14][0].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[14][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[14][0].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[14][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[14][1].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[14][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[14][1].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[14][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[14][2].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[14][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[14][2].sBottom = 175 + UDBOffsetY;	
		gItemDescGenRegions[14][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[14][3].sTop = 163 + UDBOffsetY;	gItemDescGenRegions[14][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[14][3].sBottom = 175 + UDBOffsetY;

		// Right Column, row 3
		gItemDescGenRegions[15][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[15][0].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[15][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[15][0].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[15][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[15][1].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[15][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[15][1].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[15][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[15][2].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[15][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[15][2].sBottom = 188 + UDBOffsetY;	
		gItemDescGenRegions[15][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[15][3].sTop = 176 + UDBOffsetY;	gItemDescGenRegions[15][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[15][3].sBottom = 188 + UDBOffsetY;

		// Right Column, row 4
		gItemDescGenRegions[16][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[16][0].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[16][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[16][0].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[16][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[16][1].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[16][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[16][1].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[16][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[16][2].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[16][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[16][2].sBottom = 201 + UDBOffsetY;	
		gItemDescGenRegions[16][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[16][3].sTop = 189 + UDBOffsetY;	gItemDescGenRegions[16][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[16][3].sBottom = 201 + UDBOffsetY;

		// Right Column, row 5
		gItemDescGenRegions[17][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[17][0].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[17][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[17][0].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[17][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[17][1].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[17][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[17][1].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[17][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[17][2].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[17][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[17][2].sBottom = 214 + UDBOffsetY;	
		gItemDescGenRegions[17][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[17][3].sTop = 202 + UDBOffsetY;	gItemDescGenRegions[17][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[17][3].sBottom = 214 + UDBOffsetY;

		// Right Column, row 6
		gItemDescGenRegions[18][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[18][0].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[18][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[18][0].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[18][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[18][1].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[18][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[18][1].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[18][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[18][2].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[18][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[18][2].sBottom = 227 + UDBOffsetY;	
		gItemDescGenRegions[18][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[18][3].sTop = 215 + UDBOffsetY;	gItemDescGenRegions[18][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[18][3].sBottom = 227 + UDBOffsetY;

		// NOTE a skip of Y here - these belong to the "tertiary" column, which is below the "secondary" column, separated by an index and header.
		// Right Column, row 7
		gItemDescGenRegions[19][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[19][0].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[19][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[19][0].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[19][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[19][1].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[19][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[19][1].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[19][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[19][2].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[19][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[19][2].sBottom = 268 + UDBOffsetY;	
		gItemDescGenRegions[19][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[19][3].sTop = 256 + UDBOffsetY;	gItemDescGenRegions[19][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[19][3].sBottom = 268 + UDBOffsetY;

		// Right Column, row 8
		gItemDescGenRegions[20][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[20][0].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[20][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[20][0].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[20][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[20][1].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[20][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[20][1].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[20][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[20][2].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[20][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[20][2].sBottom = 281 + UDBOffsetY;	
		gItemDescGenRegions[20][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[20][3].sTop = 269 + UDBOffsetY;	gItemDescGenRegions[20][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[20][3].sBottom = 281 + UDBOffsetY;

		// Right Column, row 9
		gItemDescGenRegions[21][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[21][0].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[21][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[21][0].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[21][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[21][1].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[21][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[21][1].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[21][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[21][2].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[21][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[21][2].sBottom = 294 + UDBOffsetY;	
		gItemDescGenRegions[21][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[21][3].sTop = 282 + UDBOffsetY;	gItemDescGenRegions[21][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[21][3].sBottom = 294 + UDBOffsetY;

		// Right Column, row 10
		gItemDescGenRegions[22][0].sLeft = 130 + UDBOffsetX;		gItemDescGenRegions[22][0].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[22][0].sRight = 179 + UDBOffsetX;	gItemDescGenRegions[22][0].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[22][1].sLeft = 181 + UDBOffsetX;		gItemDescGenRegions[22][1].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[22][1].sRight = 202 + UDBOffsetX;	gItemDescGenRegions[22][1].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[22][2].sLeft = 204 + UDBOffsetX;		gItemDescGenRegions[22][2].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[22][2].sRight = 223 + UDBOffsetX;	gItemDescGenRegions[22][2].sBottom = 307 + UDBOffsetY;	
		gItemDescGenRegions[22][3].sLeft = 225 + UDBOffsetX;		gItemDescGenRegions[22][3].sTop = 295 + UDBOffsetY;	gItemDescGenRegions[22][3].sRight = 245 + UDBOffsetX;	gItemDescGenRegions[22][3].sBottom = 307 + UDBOffsetY;

		///////////////////////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB General Page - Secondary Data locations
		// These icons will appear on the RIGHT columns of the general page. The are larger
		// than the normal icons, and indicate flags. Armor, Ammo and Misc Items will use
		// these to display special flag properties like "Electronic" or "Tracer effect".

		// First row
		gItemDescGenSecondaryRegions[0].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[0].sTop = 135 + UDBOffsetY;		gItemDescGenSecondaryRegions[0].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[0].sBottom = 160 + UDBOffsetY;
		gItemDescGenSecondaryRegions[1].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[1].sTop = 135 + UDBOffsetY;		gItemDescGenSecondaryRegions[1].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[1].sBottom = 160 + UDBOffsetY;
		gItemDescGenSecondaryRegions[2].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[2].sTop = 135 + UDBOffsetY;		gItemDescGenSecondaryRegions[2].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[2].sBottom = 160 + UDBOffsetY;
		gItemDescGenSecondaryRegions[3].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[3].sTop = 135 + UDBOffsetY;		gItemDescGenSecondaryRegions[3].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[3].sBottom = 160 + UDBOffsetY;

		// Second row
		gItemDescGenSecondaryRegions[4].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[4].sTop = 163 + UDBOffsetY;		gItemDescGenSecondaryRegions[4].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[4].sBottom = 188 + UDBOffsetY;
		gItemDescGenSecondaryRegions[5].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[5].sTop = 163 + UDBOffsetY;		gItemDescGenSecondaryRegions[5].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[5].sBottom = 188 + UDBOffsetY;
		gItemDescGenSecondaryRegions[6].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[6].sTop = 163 + UDBOffsetY;		gItemDescGenSecondaryRegions[6].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[6].sBottom = 188 + UDBOffsetY;
		gItemDescGenSecondaryRegions[7].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[7].sTop = 163 + UDBOffsetY;		gItemDescGenSecondaryRegions[7].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[7].sBottom = 188 + UDBOffsetY;

		// Third row

		gItemDescGenSecondaryRegions[8].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[8].sTop = 191 + UDBOffsetY;		gItemDescGenSecondaryRegions[8].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[8].sBottom = 216 + UDBOffsetY;
		gItemDescGenSecondaryRegions[9].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[9].sTop = 191 + UDBOffsetY;		gItemDescGenSecondaryRegions[9].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[9].sBottom = 216 + UDBOffsetY;
		gItemDescGenSecondaryRegions[10].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[10].sTop = 191 + UDBOffsetY;		gItemDescGenSecondaryRegions[10].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[10].sBottom = 216 + UDBOffsetY;
		gItemDescGenSecondaryRegions[11].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[11].sTop = 191 + UDBOffsetY;		gItemDescGenSecondaryRegions[11].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[11].sBottom = 216 + UDBOffsetY;

		// Fourth row
		gItemDescGenSecondaryRegions[12].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[12].sTop = 219 + UDBOffsetY;		gItemDescGenSecondaryRegions[12].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[12].sBottom = 244 + UDBOffsetY;
		gItemDescGenSecondaryRegions[13].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[13].sTop = 219 + UDBOffsetY;		gItemDescGenSecondaryRegions[13].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[13].sBottom = 244 + UDBOffsetY;
		gItemDescGenSecondaryRegions[14].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[14].sTop = 219 + UDBOffsetY;		gItemDescGenSecondaryRegions[14].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[14].sBottom = 244 + UDBOffsetY;
		gItemDescGenSecondaryRegions[15].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[15].sTop = 219 + UDBOffsetY;		gItemDescGenSecondaryRegions[15].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[15].sBottom = 244 + UDBOffsetY;

		// Fifth row
		gItemDescGenSecondaryRegions[16].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[16].sTop = 247 + UDBOffsetY;		gItemDescGenSecondaryRegions[16].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[16].sBottom = 272 + UDBOffsetY;
		gItemDescGenSecondaryRegions[17].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[17].sTop = 247 + UDBOffsetY;		gItemDescGenSecondaryRegions[17].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[17].sBottom = 272 + UDBOffsetY;
		gItemDescGenSecondaryRegions[18].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[18].sTop = 247 + UDBOffsetY;		gItemDescGenSecondaryRegions[18].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[18].sBottom = 272 + UDBOffsetY;
		gItemDescGenSecondaryRegions[19].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[19].sTop = 247 + UDBOffsetY;		gItemDescGenSecondaryRegions[19].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[19].sBottom = 272 + UDBOffsetY;

		// Sixth row
		gItemDescGenSecondaryRegions[20].sLeft = 131 + UDBOffsetX;	gItemDescGenSecondaryRegions[20].sTop = 275 + UDBOffsetY;		gItemDescGenSecondaryRegions[20].sRight = 156 + UDBOffsetX;	gItemDescGenSecondaryRegions[20].sBottom = 300 + UDBOffsetY;
		gItemDescGenSecondaryRegions[21].sLeft = 159 + UDBOffsetX;	gItemDescGenSecondaryRegions[21].sTop = 275 + UDBOffsetY;		gItemDescGenSecondaryRegions[21].sRight = 184 + UDBOffsetX;	gItemDescGenSecondaryRegions[21].sBottom = 300 + UDBOffsetY;
		gItemDescGenSecondaryRegions[22].sLeft = 187 + UDBOffsetX;	gItemDescGenSecondaryRegions[22].sTop = 275 + UDBOffsetY;		gItemDescGenSecondaryRegions[22].sRight = 212 + UDBOffsetX;	gItemDescGenSecondaryRegions[22].sBottom = 300 + UDBOffsetY;
		gItemDescGenSecondaryRegions[23].sLeft = 215 + UDBOffsetX;	gItemDescGenSecondaryRegions[23].sTop = 275 + UDBOffsetY;		gItemDescGenSecondaryRegions[23].sRight = 240 + UDBOffsetX;	gItemDescGenSecondaryRegions[23].sBottom = 300 + UDBOffsetY;

		///////////////////////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB Advanced Page
		// The set of data appears on the third UDB page. It shows bonuses given by items either
		// to other items (through attachment/ammo) or directly to the person wearing/using
		// the item.
		// Each row contains 4 elements, the first being the icon position, and then
		// one element per stance (Standing, crouching, prone).

		/////////// INDEX
		gItemDescAdvIndexRegions[0][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvIndexRegions[0][0].sTop = 119 + UDBOffsetY;		gItemDescAdvIndexRegions[0][0].sRight = 125 + UDBOffsetX;	gItemDescAdvIndexRegions[0][0].sBottom = 132 + UDBOffsetY;
		gItemDescAdvIndexRegions[0][1].sLeft = 128 + UDBOffsetX;	gItemDescAdvIndexRegions[0][1].sTop = 119 + UDBOffsetY;		gItemDescAdvIndexRegions[0][1].sRight = 157 + UDBOffsetX;	gItemDescAdvIndexRegions[0][1].sBottom = 132 + UDBOffsetY;
		gItemDescAdvIndexRegions[0][2].sLeft = 160 + UDBOffsetX;	gItemDescAdvIndexRegions[0][2].sTop = 119 + UDBOffsetY;		gItemDescAdvIndexRegions[0][2].sRight = 189 + UDBOffsetX;	gItemDescAdvIndexRegions[0][2].sBottom = 132 + UDBOffsetY;
		gItemDescAdvIndexRegions[0][3].sLeft = 193 + UDBOffsetX;	gItemDescAdvIndexRegions[0][3].sTop = 119 + UDBOffsetY;		gItemDescAdvIndexRegions[0][3].sRight = 220 + UDBOffsetX;	gItemDescAdvIndexRegions[0][3].sBottom = 132 + UDBOffsetY;

		gItemDescAdvRegions[0][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[0][0].sTop = 138 + UDBOffsetY;	gItemDescAdvRegions[0][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[0][0].sBottom = 150 + UDBOffsetY; 
		gItemDescAdvRegions[0][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[0][1].sTop = 138 + UDBOffsetY;	gItemDescAdvRegions[0][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[0][1].sBottom = 150 + UDBOffsetY; 
		gItemDescAdvRegions[0][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[0][2].sTop = 138 + UDBOffsetY;	gItemDescAdvRegions[0][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[0][2].sBottom = 150 + UDBOffsetY; 
		gItemDescAdvRegions[0][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[0][3].sTop = 138 + UDBOffsetY;	gItemDescAdvRegions[0][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[0][3].sBottom = 150 + UDBOffsetY; 

		gItemDescAdvRegions[1][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[1][0].sTop = 151 + UDBOffsetY;	gItemDescAdvRegions[1][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[1][0].sBottom = 163 + UDBOffsetY; 
		gItemDescAdvRegions[1][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[1][1].sTop = 151 + UDBOffsetY;	gItemDescAdvRegions[1][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[1][1].sBottom = 163 + UDBOffsetY; 
		gItemDescAdvRegions[1][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[1][2].sTop = 151 + UDBOffsetY;	gItemDescAdvRegions[1][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[1][2].sBottom = 163 + UDBOffsetY; 
		gItemDescAdvRegions[1][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[1][3].sTop = 151 + UDBOffsetY;	gItemDescAdvRegions[1][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[1][3].sBottom = 163 + UDBOffsetY; 

		gItemDescAdvRegions[2][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[2][0].sTop = 164 + UDBOffsetY;	gItemDescAdvRegions[2][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[2][0].sBottom = 176 + UDBOffsetY; 
		gItemDescAdvRegions[2][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[2][1].sTop = 164 + UDBOffsetY;	gItemDescAdvRegions[2][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[2][1].sBottom = 176 + UDBOffsetY; 
		gItemDescAdvRegions[2][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[2][2].sTop = 164 + UDBOffsetY;	gItemDescAdvRegions[2][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[2][2].sBottom = 176 + UDBOffsetY; 
		gItemDescAdvRegions[2][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[2][3].sTop = 164 + UDBOffsetY;	gItemDescAdvRegions[2][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[2][3].sBottom = 176 + UDBOffsetY; 

		gItemDescAdvRegions[3][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[3][0].sTop = 177 + UDBOffsetY;	gItemDescAdvRegions[3][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[3][0].sBottom = 189 + UDBOffsetY; 
		gItemDescAdvRegions[3][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[3][1].sTop = 177 + UDBOffsetY;	gItemDescAdvRegions[3][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[3][1].sBottom = 189 + UDBOffsetY; 
		gItemDescAdvRegions[3][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[3][2].sTop = 177 + UDBOffsetY;	gItemDescAdvRegions[3][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[3][2].sBottom = 189 + UDBOffsetY; 
		gItemDescAdvRegions[3][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[3][3].sTop = 177 + UDBOffsetY;	gItemDescAdvRegions[3][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[3][3].sBottom = 189 + UDBOffsetY; 

		gItemDescAdvRegions[4][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[4][0].sTop = 190 + UDBOffsetY;	gItemDescAdvRegions[4][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[4][0].sBottom = 202 + UDBOffsetY; 
		gItemDescAdvRegions[4][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[4][1].sTop = 190 + UDBOffsetY;	gItemDescAdvRegions[4][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[4][1].sBottom = 202 + UDBOffsetY; 
		gItemDescAdvRegions[4][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[4][2].sTop = 190 + UDBOffsetY;	gItemDescAdvRegions[4][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[4][2].sBottom = 202 + UDBOffsetY; 
		gItemDescAdvRegions[4][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[4][3].sTop = 190 + UDBOffsetY;	gItemDescAdvRegions[4][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[4][3].sBottom = 202 + UDBOffsetY; 

		gItemDescAdvRegions[5][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[5][0].sTop = 203 + UDBOffsetY;	gItemDescAdvRegions[5][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[5][0].sBottom = 215 + UDBOffsetY; 
		gItemDescAdvRegions[5][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[5][1].sTop = 203 + UDBOffsetY;	gItemDescAdvRegions[5][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[5][1].sBottom = 215 + UDBOffsetY; 
		gItemDescAdvRegions[5][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[5][2].sTop = 203 + UDBOffsetY;	gItemDescAdvRegions[5][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[5][2].sBottom = 215 + UDBOffsetY; 
		gItemDescAdvRegions[5][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[5][3].sTop = 203 + UDBOffsetY;	gItemDescAdvRegions[5][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[5][3].sBottom = 215 + UDBOffsetY; 

		gItemDescAdvRegions[6][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[6][0].sTop = 216 + UDBOffsetY;	gItemDescAdvRegions[6][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[6][0].sBottom = 228 + UDBOffsetY; 
		gItemDescAdvRegions[6][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[6][1].sTop = 216 + UDBOffsetY;	gItemDescAdvRegions[6][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[6][1].sBottom = 228 + UDBOffsetY; 
		gItemDescAdvRegions[6][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[6][2].sTop = 216 + UDBOffsetY;	gItemDescAdvRegions[6][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[6][2].sBottom = 228 + UDBOffsetY; 
		gItemDescAdvRegions[6][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[6][3].sTop = 216 + UDBOffsetY;	gItemDescAdvRegions[6][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[6][3].sBottom = 228 + UDBOffsetY; 

		gItemDescAdvRegions[7][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[7][0].sTop = 229 + UDBOffsetY;	gItemDescAdvRegions[7][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[7][0].sBottom = 241 + UDBOffsetY; 
		gItemDescAdvRegions[7][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[7][1].sTop = 229 + UDBOffsetY;	gItemDescAdvRegions[7][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[7][1].sBottom = 241 + UDBOffsetY; 
		gItemDescAdvRegions[7][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[7][2].sTop = 229 + UDBOffsetY;	gItemDescAdvRegions[7][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[7][2].sBottom = 241 + UDBOffsetY; 
		gItemDescAdvRegions[7][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[7][3].sTop = 229 + UDBOffsetY;	gItemDescAdvRegions[7][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[7][3].sBottom = 241 + UDBOffsetY; 

		gItemDescAdvRegions[8][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[8][0].sTop = 242 + UDBOffsetY;	gItemDescAdvRegions[8][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[8][0].sBottom = 254 + UDBOffsetY; 
		gItemDescAdvRegions[8][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[8][1].sTop = 242 + UDBOffsetY;	gItemDescAdvRegions[8][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[8][1].sBottom = 254 + UDBOffsetY; 
		gItemDescAdvRegions[8][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[8][2].sTop = 242 + UDBOffsetY;	gItemDescAdvRegions[8][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[8][2].sBottom = 254 + UDBOffsetY; 
		gItemDescAdvRegions[8][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[8][3].sTop = 242 + UDBOffsetY;	gItemDescAdvRegions[8][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[8][3].sBottom = 254 + UDBOffsetY; 

		gItemDescAdvRegions[9][0].sLeft = 12 + UDBOffsetX;		gItemDescAdvRegions[9][0].sTop = 255 + UDBOffsetY;	gItemDescAdvRegions[9][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[9][0].sBottom = 267 + UDBOffsetY; 
		gItemDescAdvRegions[9][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[9][1].sTop = 255 + UDBOffsetY;	gItemDescAdvRegions[9][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[9][1].sBottom = 267 + UDBOffsetY; 
		gItemDescAdvRegions[9][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[9][2].sTop = 255 + UDBOffsetY;	gItemDescAdvRegions[9][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[9][2].sBottom = 267 + UDBOffsetY; 
		gItemDescAdvRegions[9][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[9][3].sTop = 255 + UDBOffsetY;	gItemDescAdvRegions[9][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[9][3].sBottom = 267 + UDBOffsetY; 

		gItemDescAdvRegions[10][0].sLeft = 12 + UDBOffsetX;			gItemDescAdvRegions[10][0].sTop = 268 + UDBOffsetY;	gItemDescAdvRegions[10][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[10][0].sBottom = 280 + UDBOffsetY; 
		gItemDescAdvRegions[10][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[10][1].sTop = 268 + UDBOffsetY;	gItemDescAdvRegions[10][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[10][1].sBottom = 280 + UDBOffsetY; 
		gItemDescAdvRegions[10][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[10][2].sTop = 268 + UDBOffsetY;	gItemDescAdvRegions[10][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[10][2].sBottom = 280 + UDBOffsetY; 
		gItemDescAdvRegions[10][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[10][3].sTop = 268 + UDBOffsetY;	gItemDescAdvRegions[10][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[10][3].sBottom = 280 + UDBOffsetY; 

		gItemDescAdvRegions[11][0].sLeft = 12 + UDBOffsetX;			gItemDescAdvRegions[11][0].sTop = 281 + UDBOffsetY;	gItemDescAdvRegions[11][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[11][0].sBottom = 293 + UDBOffsetY; 
		gItemDescAdvRegions[11][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[11][1].sTop = 281 + UDBOffsetY;	gItemDescAdvRegions[11][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[11][1].sBottom = 293 + UDBOffsetY; 
		gItemDescAdvRegions[11][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[11][2].sTop = 281 + UDBOffsetY;	gItemDescAdvRegions[11][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[11][2].sBottom = 293 + UDBOffsetY; 
		gItemDescAdvRegions[11][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[11][3].sTop = 281 + UDBOffsetY;	gItemDescAdvRegions[11][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[11][3].sBottom = 293 + UDBOffsetY; 

		gItemDescAdvRegions[12][0].sLeft = 12 + UDBOffsetX;			gItemDescAdvRegions[12][0].sTop = 294 + UDBOffsetY;	gItemDescAdvRegions[12][0].sRight = 126 + UDBOffsetX;		gItemDescAdvRegions[12][0].sBottom = 306 + UDBOffsetY; 
		gItemDescAdvRegions[12][1].sLeft = 130 + UDBOffsetX;		gItemDescAdvRegions[12][1].sTop = 294 + UDBOffsetY;	gItemDescAdvRegions[12][1].sRight = 157 + UDBOffsetX;		gItemDescAdvRegions[12][1].sBottom = 306 + UDBOffsetY; 
		gItemDescAdvRegions[12][2].sLeft = 162 + UDBOffsetX;		gItemDescAdvRegions[12][2].sTop = 294 + UDBOffsetY;	gItemDescAdvRegions[12][2].sRight = 189 + UDBOffsetX;		gItemDescAdvRegions[12][2].sBottom = 306 + UDBOffsetY; 
		gItemDescAdvRegions[12][3].sLeft = 194 + UDBOffsetX;		gItemDescAdvRegions[12][3].sTop = 294 + UDBOffsetY;	gItemDescAdvRegions[12][3].sRight = 221 + UDBOffsetX;		gItemDescAdvRegions[12][3].sBottom = 306 + UDBOffsetY; 
	}
	else // ODB
	{
		///////////////////////////////////////////////////////////
		// HEADROCK HAM 4: UDB LBE background coordinates
		// These depend on Strategic/Tactical...
		if (guiCurrentScreen == MAP_SCREEN)
		{
			INT16 sOffsetY = sNASYCorrection( pObject );

			gItemDescLBEBackground[0].sLeft = 94 + gsInvDescX;	gItemDescLBEBackground[0].sTop = 286 + gsInvDescY + sOffsetY;
			gItemDescLBEBackground[1].sLeft = 15 + gsInvDescX;	gItemDescLBEBackground[1].sTop = 298 + gsInvDescY + sOffsetY;
			gItemDescLBEBackground[2].sLeft = 94 + gsInvDescX;	gItemDescLBEBackground[2].sTop = 286 + gsInvDescY + sOffsetY;
			gItemDescLBEBackground[3].sLeft = 61 + gsInvDescX;	gItemDescLBEBackground[3].sTop = 274 + gsInvDescY + sOffsetY;

			////////////////// MONEY TEXT COORDINATES
			gMoneyStats[0].sX = 48 + gsInvDescX;		gMoneyStats[0].sY = 98 + gsInvDescY + sOffsetY;	// Money
			gMoneyStats[1].sX = 56 + gsInvDescX;		gMoneyStats[1].sY = 108 + gsInvDescY + sOffsetY;	// Remaining
			gMoneyStats[2].sX = 48 + gsInvDescX;		gMoneyStats[2].sY = 125 + gsInvDescY + sOffsetY;	// Money
			gMoneyStats[3].sX = 56 + gsInvDescX;		gMoneyStats[3].sY = 135 + gsInvDescY + sOffsetY;	// Removing
			gMoneyStats[4].sX = 140 + gsInvDescX;		gMoneyStats[4].sY = 108 + gsInvDescY + sOffsetY;	// AMT remaining
			gMoneyStats[5].sX = 140 + gsInvDescX;		gMoneyStats[5].sY = 135 + gsInvDescY + sOffsetY;	// AMT removing

			//////////////////////////////////////////////////////////////////////
			// ODB has exactly four separate regions. Each has 8 sub-regions, which can be
			// used to display data at irregular intervals. This allows ODB to function as
			// smoothly as EDB does.
			// In addition, ODB uses some of the UDB regions to display things like PROS/CONS
			// and Description Text. This reduces the use of redundant arrays...

			// First Data Row (Condition / Ammo label)
			gODBItemDescRegions[0][0].sLeft = 52 + gsInvDescX; gODBItemDescRegions[0][0].sTop = 98 + gsInvDescY + sOffsetY; gODBItemDescRegions[0][0].sRight = 167 + gsInvDescX; gODBItemDescRegions[0][0].sBottom = 106 + gsInvDescY + sOffsetY;

			// Second Data Row (Weight)
			gODBItemDescRegions[1][0].sLeft = 52 + gsInvDescX; gODBItemDescRegions[1][0].sTop = 108 + gsInvDescY + sOffsetY; gODBItemDescRegions[1][0].sRight = 167 + gsInvDescX; gODBItemDescRegions[1][0].sBottom = 116 + gsInvDescY + sOffsetY;
 
			// Third Data Row (Usually basic Gun Statistics)
				// Left Side
				gODBItemDescRegions[2][0].sLeft = 52 + gsInvDescX; gODBItemDescRegions[2][0].sTop = 124 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[2][3].sLeft = 91 + gsInvDescX; gODBItemDescRegions[2][3].sTop = 124 + gsInvDescY + sOffsetY;	gODBItemDescRegions[2][3].sRight = 107 + gsInvDescX; gODBItemDescRegions[2][3].sBottom = 132 + gsInvDescY + sOffsetY;

				// Right Side
				gODBItemDescRegions[2][4].sLeft = 117 + gsInvDescX; gODBItemDescRegions[2][4].sTop = 124 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[2][7].sLeft = 152 + gsInvDescX; gODBItemDescRegions[2][7].sTop = 124 + gsInvDescY + sOffsetY;	gODBItemDescRegions[2][7].sRight = 168 + gsInvDescX; gODBItemDescRegions[2][7].sBottom = 132 + gsInvDescY + sOffsetY;

			// Fourth Data Row (Usually AP Costs)
				// Left Side
				gODBItemDescRegions[3][0].sLeft = 52 + gsInvDescX; gODBItemDescRegions[3][0].sTop = 135 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[3][1].sLeft = 77 + gsInvDescX; gODBItemDescRegions[3][1].sTop = 135 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[3][2].sLeft = 86 + gsInvDescX; gODBItemDescRegions[3][2].sTop = 135 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[3][3].sLeft = 91 + gsInvDescX; gODBItemDescRegions[3][3].sTop = 135 + gsInvDescY + sOffsetY;	gODBItemDescRegions[3][3].sRight = 107 + gsInvDescX; gODBItemDescRegions[3][3].sBottom = 143 + gsInvDescY + sOffsetY;
				// Right Side
				gODBItemDescRegions[3][4].sLeft = 117 + gsInvDescX; gODBItemDescRegions[3][4].sTop = 135 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[3][6].sLeft = 145 + gsInvDescX; gODBItemDescRegions[3][6].sTop = 135 + gsInvDescY + sOffsetY;
				gODBItemDescRegions[3][7].sLeft = 152 + gsInvDescX; gODBItemDescRegions[3][7].sTop = 135 + gsInvDescY + sOffsetY;	gODBItemDescRegions[3][7].sRight = 168 + gsInvDescX; gODBItemDescRegions[3][7].sBottom = 143 + gsInvDescY + sOffsetY;

			// Description Text Area
			gItemDescTextRegions[0].sLeft = 23 + gsInvDescX;	gItemDescTextRegions[0].sTop = 170 + gsInvDescY + sOffsetY;	gItemDescTextRegions[0].sRight = 233 + gsInvDescX;

			// PROS/CONS areas
			gItemDescTextRegions[3].sLeft = 23 + gsInvDescX;	gItemDescTextRegions[3].sTop = 230 + gsInvDescY + sOffsetY;	gItemDescTextRegions[3].sRight = 233 + gsInvDescX;	gItemDescTextRegions[3].sBottom = 238 + gsInvDescY + sOffsetY; 
			gItemDescTextRegions[4].sLeft = 59 + gsInvDescX;	gItemDescTextRegions[4].sTop = 230 + gsInvDescY + sOffsetY;	gItemDescTextRegions[4].sRight = 233 + gsInvDescX;	gItemDescTextRegions[4].sBottom = 238 + gsInvDescY + sOffsetY; 
			gItemDescTextRegions[5].sLeft = 23 + gsInvDescX;	gItemDescTextRegions[5].sTop = 240 + gsInvDescY + sOffsetY;	gItemDescTextRegions[5].sRight = 233 + gsInvDescX;	gItemDescTextRegions[5].sBottom = 250 + gsInvDescY + sOffsetY; 
			gItemDescTextRegions[6].sLeft = 59 + gsInvDescX;	gItemDescTextRegions[6].sTop = 240 + gsInvDescY + sOffsetY;	gItemDescTextRegions[6].sRight = 233 + gsInvDescX;	gItemDescTextRegions[6].sBottom = 250 + gsInvDescY + sOffsetY; 

		}
		else
		{
			// Tactical NAS/ODB and regular ODB are radically different in shape, so we need two separate
			// definition tables.
			if (UseNASDesc( pObject ))
			{
				gItemDescLBEBackground[0].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[0].sTop = 8 + gsInvDescY;
				gItemDescLBEBackground[1].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[1].sTop = 8 + gsInvDescY;
				gItemDescLBEBackground[2].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[2].sTop = 8 + gsInvDescY;
				gItemDescLBEBackground[3].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[3].sTop = 8 + gsInvDescY;

				////////////////// MONEY TEXT COORDINATES
				gMoneyStats[0].sX = 266 + gsInvDescX;		gMoneyStats[0].sY = 14 + gsInvDescY;	// Money
				gMoneyStats[1].sX = 274 + gsInvDescX;		gMoneyStats[1].sY = 24 + gsInvDescY;	// Remaining
				gMoneyStats[2].sX = 266 + gsInvDescX;		gMoneyStats[2].sY = 41 + gsInvDescY;	// Money
				gMoneyStats[3].sX = 274 + gsInvDescX;		gMoneyStats[3].sY = 51 + gsInvDescY;	// Removing
				gMoneyStats[4].sX = 344 + gsInvDescX;		gMoneyStats[4].sY = 24 + gsInvDescY;	// AMT remaining
				gMoneyStats[5].sX = 344 + gsInvDescX;		gMoneyStats[5].sY = 51 + gsInvDescY;	// AMT removing

				//////////////////////////////////////////////////////////////////////
				// ODB has exactly four separate regions. Each has 8 sub-regions, which can be
				// used to display data at irregular intervals. This allows ODB to function as
				// smoothly as EDB does.
				// In addition, ODB uses some of the UDB regions to display things like PROS/CONS
				// and Description Text. This reduces the use of redundant arrays...

				// First Data Row (Condition / Ammo label)
				gODBItemDescRegions[0][0].sLeft = 270 + gsInvDescX; gODBItemDescRegions[0][0].sTop = 15 + gsInvDescY; gODBItemDescRegions[0][0].sRight = 418 + gsInvDescX; gODBItemDescRegions[0][0].sBottom = 23 + gsInvDescY;

				// Second Data Row (Weight)
				gODBItemDescRegions[1][0].sLeft = 270 + gsInvDescX; gODBItemDescRegions[1][0].sTop = 25 + gsInvDescY; gODBItemDescRegions[1][0].sRight = 418 + gsInvDescX; gODBItemDescRegions[1][0].sBottom = 33 + gsInvDescY;

				// Third Data Row (Usually basic Gun Statistics)
					// Left Side
					gODBItemDescRegions[2][0].sLeft = 270 + gsInvDescX; gODBItemDescRegions[2][0].sTop = 40 + gsInvDescY;
					gODBItemDescRegions[2][3].sLeft = 209 + gsInvDescX; gODBItemDescRegions[2][3].sTop = 40 + gsInvDescY;	gODBItemDescRegions[2][3].sRight = 334 + gsInvDescX; gODBItemDescRegions[2][3].sBottom = 48 + gsInvDescY;

					// Right Side
					gODBItemDescRegions[2][4].sLeft = 346 + gsInvDescX; gODBItemDescRegions[2][4].sTop = 40 + gsInvDescY;
					gODBItemDescRegions[2][7].sLeft = 383 + gsInvDescX; gODBItemDescRegions[2][7].sTop = 40 + gsInvDescY;	gODBItemDescRegions[2][7].sRight = 418 + gsInvDescX; gODBItemDescRegions[2][7].sBottom = 48 + gsInvDescY;

				// Fourth Data Row (Usually AP Costs)
					// Left Side
					gODBItemDescRegions[3][0].sLeft = 270 + gsInvDescX; gODBItemDescRegions[3][0].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][1].sLeft = 290 + gsInvDescX; gODBItemDescRegions[3][1].sTop = 49 + gsInvDescY;
					gODBItemDescRegions[3][2].sLeft = 302 + gsInvDescX; gODBItemDescRegions[3][2].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][3].sLeft = 309 + gsInvDescX; gODBItemDescRegions[3][3].sTop = 50 + gsInvDescY;	gODBItemDescRegions[3][3].sRight = 334 + gsInvDescX; gODBItemDescRegions[3][3].sBottom = 58 + gsInvDescY;
					// Right Side
					gODBItemDescRegions[3][4].sLeft = 344 + gsInvDescX; gODBItemDescRegions[3][4].sTop = 49 + gsInvDescY;
					gODBItemDescRegions[3][6].sLeft = 372 + gsInvDescX; gODBItemDescRegions[3][6].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][7].sLeft = 383 + gsInvDescX; gODBItemDescRegions[3][7].sTop = 50 + gsInvDescY;	gODBItemDescRegions[3][7].sRight = 418 + gsInvDescX; gODBItemDescRegions[3][7].sBottom = 58 + gsInvDescY;

				// Description Text Area
				gItemDescTextRegions[0].sLeft = 265 + gsInvDescX;	gItemDescTextRegions[0].sTop = 70 + gsInvDescY;	gItemDescTextRegions[0].sRight = 415 + gsInvDescX;

				// PROS/CONS areas
				gItemDescTextRegions[3].sLeft = 265 + gsInvDescX;	gItemDescTextRegions[3].sTop = 170 + gsInvDescY;	gItemDescTextRegions[3].sRight = 415 + gsInvDescX;	gItemDescTextRegions[3].sBottom = 188 + gsInvDescY; 
				gItemDescTextRegions[4].sLeft = 295 + gsInvDescX;	gItemDescTextRegions[4].sTop = 170 + gsInvDescY;	gItemDescTextRegions[4].sRight = 415 + gsInvDescX;	gItemDescTextRegions[4].sBottom = 188 + gsInvDescY; 
				gItemDescTextRegions[5].sLeft = 265 + gsInvDescX;	gItemDescTextRegions[5].sTop = 180 + gsInvDescY;	gItemDescTextRegions[5].sRight = 415 + gsInvDescX;	gItemDescTextRegions[5].sBottom = 200 + gsInvDescY; 
				gItemDescTextRegions[6].sLeft = 295 + gsInvDescX;	gItemDescTextRegions[6].sTop = 180 + gsInvDescY;	gItemDescTextRegions[6].sRight = 415 + gsInvDescX;	gItemDescTextRegions[6].sBottom = 200 + gsInvDescY; 
			}
			else
			{
				gItemDescLBEBackground[0].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[0].sTop = 8 + gsInvDescY;
				gItemDescLBEBackground[1].sLeft = 3 + gsInvDescX;	gItemDescLBEBackground[1].sTop = 137 + gsInvDescY;
				gItemDescLBEBackground[2].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[2].sTop = 8 + gsInvDescY;
				gItemDescLBEBackground[3].sLeft = 320 + gsInvDescX;	gItemDescLBEBackground[3].sTop = 8 + gsInvDescY;

				////////////////// MONEY TEXT COORDINATES
				gMoneyStats[0].sX = 204 + gsInvDescX;		gMoneyStats[0].sY = 14 + gsInvDescY;	// Money
				gMoneyStats[1].sX = 212 + gsInvDescX;		gMoneyStats[1].sY = 24 + gsInvDescY;	// Remaining
				gMoneyStats[2].sX = 204 + gsInvDescX;		gMoneyStats[2].sY = 41 + gsInvDescY;	// Money
				gMoneyStats[3].sX = 212 + gsInvDescX;		gMoneyStats[3].sY = 51 + gsInvDescY;	// Removing
				gMoneyStats[4].sX = 282 + gsInvDescX;		gMoneyStats[4].sY = 24 + gsInvDescY;	// AMT remaining
				gMoneyStats[5].sX = 282 + gsInvDescX;		gMoneyStats[5].sY = 51 + gsInvDescY;	// AMT removing

				//////////////////////////////////////////////////////////////////////
				// ODB has exactly four separate regions. Each has 8 sub-regions, which can be
				// used to display data at irregular intervals. This allows ODB to function as
				// smoothly as EDB does.
				// In addition, ODB uses some of the UDB regions to display things like PROS/CONS
				// and Description Text. This reduces the use of redundant arrays...

				// First Data Row (Condition / Ammo label)
				gODBItemDescRegions[0][0].sLeft = 202 + gsInvDescX; gODBItemDescRegions[0][0].sTop = 15 + gsInvDescY; gODBItemDescRegions[0][0].sRight = 311 + gsInvDescX; gODBItemDescRegions[0][0].sBottom = 23 + gsInvDescY;

				// Second Data Row (Weight)
				gODBItemDescRegions[1][0].sLeft = 202 + gsInvDescX; gODBItemDescRegions[1][0].sTop = 25 + gsInvDescY; gODBItemDescRegions[1][0].sRight = 311 + gsInvDescX; gODBItemDescRegions[1][0].sBottom = 33 + gsInvDescY;

				// Third Data Row (Usually basic Gun Statistics)
					// Left Side
					gODBItemDescRegions[2][0].sLeft = 202 + gsInvDescX; gODBItemDescRegions[2][0].sTop = 40 + gsInvDescY;
					gODBItemDescRegions[2][3].sLeft = 141 + gsInvDescX; gODBItemDescRegions[2][3].sTop = 40 + gsInvDescY;	gODBItemDescRegions[2][3].sRight = 260 + gsInvDescX; gODBItemDescRegions[2][3].sBottom = 48 + gsInvDescY;

					// Right Side
					gODBItemDescRegions[2][4].sLeft = 265 + gsInvDescX; gODBItemDescRegions[2][4].sTop = 40 + gsInvDescY;
					gODBItemDescRegions[2][7].sLeft = 302 + gsInvDescX; gODBItemDescRegions[2][7].sTop = 40 + gsInvDescY;	gODBItemDescRegions[2][7].sRight = 311 + gsInvDescX; gODBItemDescRegions[2][7].sBottom = 48 + gsInvDescY;

				// Fourth Data Row (Usually AP Costs)
					// Left Side
					gODBItemDescRegions[3][0].sLeft = 202 + gsInvDescX; gODBItemDescRegions[3][0].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][1].sLeft = 222 + gsInvDescX; gODBItemDescRegions[3][1].sTop = 49 + gsInvDescY;
					gODBItemDescRegions[3][2].sLeft = 234 + gsInvDescX; gODBItemDescRegions[3][2].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][3].sLeft = 241 + gsInvDescX; gODBItemDescRegions[3][3].sTop = 50 + gsInvDescY;	gODBItemDescRegions[3][3].sRight = 260 + gsInvDescX; gODBItemDescRegions[3][3].sBottom = 58 + gsInvDescY;
					// Right Side
					gODBItemDescRegions[3][4].sLeft = 263 + gsInvDescX; gODBItemDescRegions[3][4].sTop = 49 + gsInvDescY;
					gODBItemDescRegions[3][6].sLeft = 291 + gsInvDescX; gODBItemDescRegions[3][6].sTop = 50 + gsInvDescY;
					gODBItemDescRegions[3][7].sLeft = 302 + gsInvDescX; gODBItemDescRegions[3][7].sTop = 50 + gsInvDescY;	gODBItemDescRegions[3][7].sRight = 311 + gsInvDescX; gODBItemDescRegions[3][7].sBottom = 58 + gsInvDescY;

				// Description Text Area
				gItemDescTextRegions[0].sLeft = 11 + gsInvDescX;	gItemDescTextRegions[0].sTop = 80 + gsInvDescY;	gItemDescTextRegions[0].sRight = 300 + gsInvDescX;

				// PROS/CONS areas
				gItemDescTextRegions[3].sLeft = 11 + gsInvDescX;	gItemDescTextRegions[3].sTop = 110 + gsInvDescY;	gItemDescTextRegions[3].sRight = 300 + gsInvDescX;	gItemDescTextRegions[3].sBottom = 118 + gsInvDescY; 
				gItemDescTextRegions[4].sLeft = 47 + gsInvDescX;	gItemDescTextRegions[4].sTop = 110 + gsInvDescY;	gItemDescTextRegions[4].sRight = 300 + gsInvDescX;	gItemDescTextRegions[4].sBottom = 118 + gsInvDescY; 
				gItemDescTextRegions[5].sLeft = 11 + gsInvDescX;	gItemDescTextRegions[5].sTop = 120 + gsInvDescY;	gItemDescTextRegions[5].sRight = 300 + gsInvDescX;	gItemDescTextRegions[5].sBottom = 130 + gsInvDescY; 
				gItemDescTextRegions[6].sLeft = 47 + gsInvDescX;	gItemDescTextRegions[6].sTop = 120 + gsInvDescY;	gItemDescTextRegions[6].sRight = 300 + gsInvDescX;	gItemDescTextRegions[6].sBottom = 130 + gsInvDescY; 
			}
		}

	}
}

void SetupItemDescAttachmentsXY(UINT8 ID, INT16 sX, INT16 sY, INT16 sHeight, INT16 sWidth, INT16 sBarDx, INT16 sBarDy)
{
	//Madd: enable hiding the inventory slots by placing them outside of the viewable area
	if ( sX >= 300 || sY >= 300 )
	{
		sX = 0;
		sY = 0;
		sHeight = 0;
		sWidth = 0;
		sBarDx = 0;
		sBarDy = 0;
	}

	gItemDescAttachmentsXY[ID].sX		= sX;
	gItemDescAttachmentsXY[ID].sY		= sY;
	gItemDescAttachmentsXY[ID].sHeight	= sHeight;
	gItemDescAttachmentsXY[ID].sWidth	= sWidth;
	gItemDescAttachmentsXY[ID].sBarDx	= sBarDx;
	gItemDescAttachmentsXY[ID].sBarDy	= sBarDy;
}

void InitDescStatCoords(OBJECTTYPE *pObject)
{	
	std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(pObject);
	memset(gItemDescAttachmentsXY, 0, MAX_ATTACHMENTS);
	if( guiCurrentScreen == MAP_SCREEN )
	{	
		//WarmSteel - If we're using the new attachment system and the item in question does not have an empty attachment list.
		if(UsingNewAttachmentSystem()==true && !usAttachmentSlotIndexVector.empty() && UseNASDesc(pObject))
		{
			//Loop throught the attachments and get their slot id's
			for (UINT8 x = 0; x < usAttachmentSlotIndexVector.size(); x++){
				//WarmSteel - Tell this slot where it's supposed to be.
				
				if(AttachmentSlots[usAttachmentSlotIndexVector[x]].fBigSlot){
					SetupItemDescAttachmentsXY(x, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosX, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosY, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_BIG_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
				} else {
					SetupItemDescAttachmentsXY(x, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosX, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosY, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
				}
				
			}
		}
		//Not a valid item for NAS, use default
		else
		{
			SetupItemDescAttachmentsXY(0, 175,  9, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
			SetupItemDescAttachmentsXY(1, 213,  9, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
			SetupItemDescAttachmentsXY(2, 175, 37, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
			SetupItemDescAttachmentsXY(3, 213, 37, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
		}
		
	}
	else
	{
		//WarmSteel - If we're using the new attachment system and the item in question does not have an empty attachment list.
		if(UsingNewAttachmentSystem()==true && !usAttachmentSlotIndexVector.empty() && UseNASDesc(pObject))
		{
			for (UINT8 x = 0; x < usAttachmentSlotIndexVector.size(); x++){
				//WarmSteel - Tell this slot where it's supposed to be.
				
				if(AttachmentSlots[usAttachmentSlotIndexVector[x]].fBigSlot){
					SetupItemDescAttachmentsXY(x, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosX, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosY, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_BIG_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
				} else {
					SetupItemDescAttachmentsXY(x, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosX, AttachmentSlots[usAttachmentSlotIndexVector[x]].usDescPanelPosY, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-9, INV_BAR_DY);
				}
				
			}
		}
		//Not a valid item for NAS, use default
		else
		{
			//default setup
			SetupItemDescAttachmentsXY(0, 133, 11, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
			SetupItemDescAttachmentsXY(1, 167, 11, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
			SetupItemDescAttachmentsXY(2, 133, 38, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
			SetupItemDescAttachmentsXY(3, 167, 38, SM_INV_SLOT_HEIGHT, ATTACH_SLOT_WIDTH, INV_BAR_DX-8, INV_BAR_DY+1);
		}
	}
}

void InitLBECoords()
{
	if(guiCurrentScreen ==MAP_SCREEN)	//Strategic
	{
		if(UsingEDBSystem() > 0)
		{
			gLBEStats[0].sX = 65;	gLBEStats[0].sY = 123;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 101;	gLBEStats[1].sY = 123;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 65;	gLBEStats[2].sY = 147;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 101;	gLBEStats[3].sY = 147;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 137;	gLBEStats[4].sY = 147;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 12;	gLBEStats[12].sY = 123;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 48;	gLBEStats[13].sY = 123;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 84;	gLBEStats[14].sY = 123;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 120;	gLBEStats[15].sY = 123;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 156;	gLBEStats[16].sY = 123;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 12;	gLBEStats[17].sY = 147;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 48;	gLBEStats[18].sY = 147;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 84;	gLBEStats[19].sY = 147;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 120;	gLBEStats[20].sY = 147;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 156;	gLBEStats[21].sY = 147;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 192;	gLBEStats[22].sY = 123;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 192;	gLBEStats[23].sY = 147;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 32;	gLBEStats[24].sY = 123;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 68;	gLBEStats[25].sY = 123;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 32;	gLBEStats[26].sY = 147;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 68;	gLBEStats[27].sY = 147;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 104;	gLBEStats[28].sY = 123;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 104;	gLBEStats[29].sY = 147;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 171;	gLBEStats[30].sY = 123;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 12;	gLBEStats[36].sY = 123;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 12;	gLBEStats[37].sY = 147;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 72;	gLBEStats[38].sY = 123;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 72;	gLBEStats[39].sY = 147;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 42;	gLBEStats[40].sY = 123;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 42;	gLBEStats[41].sY = 147;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 102;	gLBEStats[42].sY = 123;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 102;	gLBEStats[43].sY = 147;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 132;	gLBEStats[44].sY = 123;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 132;	gLBEStats[45].sY = 147;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 193;	gLBEStats[46].sY = 123;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 193;	gLBEStats[47].sY = 147;	gLBEStats[47].sValDx = 0;	//BPack 12
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
		if(UsingEDBSystem() > 0 && UsingNewAttachmentSystem() == true)	// UDB & NAS
		{
			gLBEStats[0].sX = 658;	gLBEStats[0].sY = 9;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 694;	gLBEStats[1].sY = 9;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 658;	gLBEStats[2].sY = 33;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 694;	gLBEStats[3].sY = 33;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 669;	gLBEStats[4].sY = 57;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 658;	gLBEStats[12].sY = 9;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 694;	gLBEStats[13].sY = 9;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 730;	gLBEStats[14].sY = 9;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 766;	gLBEStats[15].sY = 9;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[17].sX = 658;	gLBEStats[17].sY = 33;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 694;	gLBEStats[18].sY = 33;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 730;	gLBEStats[19].sY = 33;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 766;	gLBEStats[20].sY = 33;	gLBEStats[20].sValDx = 0;	//Vest 9
			
			if (iResolution >= _1024x768)
			{
				gLBEStats[16].sX = 802;	gLBEStats[16].sY = 9;	gLBEStats[16].sValDx = 0;	//Vest 5
				gLBEStats[21].sX = 802;	gLBEStats[21].sY = 33;	gLBEStats[21].sValDx = 0;	//Vest 10
				gLBEStats[22].sX = 838;	gLBEStats[22].sY = 9;	gLBEStats[22].sValDx = 0;	//Vest 11
				gLBEStats[23].sX = 838;	gLBEStats[23].sY = 33;	gLBEStats[23].sValDx = 0;	//Vest 12
			}
			else
			{
				gLBEStats[16].sX = 688;	gLBEStats[16].sY = 57;	gLBEStats[16].sValDx = 0;	//Vest 5
				gLBEStats[21].sX = 688;	gLBEStats[21].sY = 81;	gLBEStats[21].sValDx = 0;	//Vest 10
				gLBEStats[22].sX = 724;	gLBEStats[22].sY = 57;	gLBEStats[22].sValDx = 0;	//Vest 11
				gLBEStats[23].sX = 724;	gLBEStats[23].sY = 81;	gLBEStats[23].sValDx = 0;	//Vest 12
			}

			gLBEStats[24].sX = 658;	gLBEStats[24].sY = 9;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 694;	gLBEStats[25].sY = 9;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 658;	gLBEStats[26].sY = 33;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 694;	gLBEStats[27].sY = 33;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 730;	gLBEStats[28].sY = 9;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 730;	gLBEStats[29].sY = 33;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 730;	gLBEStats[30].sY = 57;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 658;	gLBEStats[36].sY = 9;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 658;	gLBEStats[37].sY = 33;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 658;	gLBEStats[38].sY = 57;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 658;	gLBEStats[39].sY = 81;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 694;	gLBEStats[40].sY = 9;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 694;	gLBEStats[41].sY = 33;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 694;	gLBEStats[42].sY = 57;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 694;	gLBEStats[43].sY = 81;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 730;	gLBEStats[44].sY = 9;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 730;	gLBEStats[45].sY = 33;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 730;	gLBEStats[46].sY = 57;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 730;	gLBEStats[47].sY = 81;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
		else if(UsingEDBSystem() > 0 && UsingNewAttachmentSystem() == false)	// UDB & OAS
		{
			gLBEStats[0].sX = 597;	gLBEStats[0].sY = 9;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 633;	gLBEStats[1].sY = 9;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 597;	gLBEStats[2].sY = 33;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 633;	gLBEStats[3].sY = 33;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 608;	gLBEStats[4].sY = 57;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 597;	gLBEStats[12].sY = 9;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 633;	gLBEStats[13].sY = 9;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 669;	gLBEStats[14].sY = 9;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 705;	gLBEStats[15].sY = 9;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 741;	gLBEStats[16].sY = 9;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 597;	gLBEStats[17].sY = 33;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 633;	gLBEStats[18].sY = 33;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 669;	gLBEStats[19].sY = 33;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 705;	gLBEStats[20].sY = 33;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 741;	gLBEStats[21].sY = 33;	gLBEStats[21].sValDx = 0;	//Vest 10
			
			if (iResolution >= _1024x768)
			{
				gLBEStats[22].sX = 777;	gLBEStats[22].sY = 9;	gLBEStats[22].sValDx = 0;	//Vest 11
				gLBEStats[23].sX = 777;	gLBEStats[23].sY = 33;	gLBEStats[23].sValDx = 0;	//Vest 12
			}
			else
			{
				gLBEStats[22].sX = 663;	gLBEStats[22].sY = 57;	gLBEStats[22].sValDx = 0;	//Vest 11
				gLBEStats[23].sX = 663;	gLBEStats[23].sY = 81;	gLBEStats[23].sValDx = 0;	//Vest 12
			}
			gLBEStats[24].sX = 597;	gLBEStats[24].sY = 9;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 633;	gLBEStats[25].sY = 9;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 597;	gLBEStats[26].sY = 33;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 633;	gLBEStats[27].sY = 33;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 669;	gLBEStats[28].sY = 9;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 669;	gLBEStats[29].sY = 33;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 669;	gLBEStats[30].sY = 57;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 597;	gLBEStats[36].sY = 9;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 597;	gLBEStats[37].sY = 33;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 597;	gLBEStats[38].sY = 57;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 597;	gLBEStats[39].sY = 81;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 633;	gLBEStats[40].sY = 9;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 633;	gLBEStats[41].sY = 33;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 633;	gLBEStats[42].sY = 57;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 633;	gLBEStats[43].sY = 81;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 669;	gLBEStats[44].sY = 9;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 669;	gLBEStats[45].sY = 33;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 669;	gLBEStats[46].sY = 57;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 669;	gLBEStats[47].sY = 81;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
		else if(UsingEDBSystem() == 0 && UsingNewAttachmentSystem() == true)	// ODB & NAS
		{
			gLBEStats[0].sX = 556;	gLBEStats[0].sY = 9;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 592;	gLBEStats[1].sY = 9;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 556;	gLBEStats[2].sY = 33;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 592;	gLBEStats[3].sY = 33;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 567;	gLBEStats[4].sY = 57;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 556;	gLBEStats[12].sY = 9;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 592;	gLBEStats[13].sY = 9;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 628;	gLBEStats[14].sY = 9;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 664;	gLBEStats[15].sY = 9;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 700;	gLBEStats[16].sY = 9;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 556;	gLBEStats[17].sY = 33;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 592;	gLBEStats[18].sY = 33;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 628;	gLBEStats[19].sY = 33;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 664;	gLBEStats[20].sY = 33;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 700;	gLBEStats[21].sY = 33;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 736;	gLBEStats[22].sY = 9;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 736;	gLBEStats[23].sY = 33;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 556;	gLBEStats[24].sY = 9;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 592;	gLBEStats[25].sY = 9;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 556;	gLBEStats[26].sY = 33;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 592;	gLBEStats[27].sY = 33;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 628;	gLBEStats[28].sY = 9;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 628;	gLBEStats[29].sY = 33;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 628;	gLBEStats[30].sY = 57;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 556;	gLBEStats[36].sY = 9;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 556;	gLBEStats[37].sY = 33;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 556;	gLBEStats[38].sY = 57;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 556;	gLBEStats[39].sY = 81;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 592;	gLBEStats[40].sY = 9;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 592;	gLBEStats[41].sY = 33;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 592;	gLBEStats[42].sY = 57;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 592;	gLBEStats[43].sY = 81;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 628;	gLBEStats[44].sY = 9;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 628;	gLBEStats[45].sY = 33;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 628;	gLBEStats[46].sY = 57;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 628;	gLBEStats[47].sY = 81;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
		else if(UsingEDBSystem() == 0 && UsingNewAttachmentSystem() == false)	// ODB & OAS
		{
			gLBEStats[0].sX = 446;	gLBEStats[0].sY = 9;	gLBEStats[0].sValDx = 0;	//Thigh 1
			gLBEStats[1].sX = 482;	gLBEStats[1].sY = 9;	gLBEStats[1].sValDx = 0;	//Thigh 2
			gLBEStats[2].sX = 446;	gLBEStats[2].sY = 33;	gLBEStats[2].sValDx = 0;	//Thigh 3
			gLBEStats[3].sX = 482;	gLBEStats[3].sY = 33;	gLBEStats[3].sValDx = 0;	//Thigh 4
			gLBEStats[4].sX = 457;	gLBEStats[4].sY = 57;	gLBEStats[4].sValDx = 0;	//Thigh 5
			gLBEStats[5].sX = 0;	gLBEStats[5].sY = 0;	gLBEStats[5].sValDx = 0;	//Thigh 6
			gLBEStats[6].sX = 0;	gLBEStats[6].sY = 0;	gLBEStats[6].sValDx = 0;	//Thigh 7
			gLBEStats[7].sX = 0;	gLBEStats[7].sY = 0;	gLBEStats[7].sValDx = 0;	//Thigh 8
			gLBEStats[8].sX = 0;	gLBEStats[8].sY = 0;	gLBEStats[8].sValDx = 0;	//Thigh 9
			gLBEStats[9].sX = 0;	gLBEStats[9].sY = 0;	gLBEStats[9].sValDx = 0;	//Thigh 10
			gLBEStats[10].sX = 0;	gLBEStats[10].sY = 0;	gLBEStats[10].sValDx = 0;	//Thigh 11
			gLBEStats[11].sX = 0;	gLBEStats[11].sY = 0;	gLBEStats[11].sValDx = 0;	//Thigh 12
			gLBEStats[12].sX = 445;	gLBEStats[12].sY = 9;	gLBEStats[12].sValDx = 0;	//Vest 1
			gLBEStats[13].sX = 481;	gLBEStats[13].sY = 9;	gLBEStats[13].sValDx = 0;	//Vest 2
			gLBEStats[14].sX = 517;	gLBEStats[14].sY = 9;	gLBEStats[14].sValDx = 0;	//Vest 3
			gLBEStats[15].sX = 553;	gLBEStats[15].sY = 9;	gLBEStats[15].sValDx = 0;	//Vest 4
			gLBEStats[16].sX = 589;	gLBEStats[16].sY = 9;	gLBEStats[16].sValDx = 0;	//Vest 5
			gLBEStats[17].sX = 445;	gLBEStats[17].sY = 33;	gLBEStats[17].sValDx = 0;	//Vest 6
			gLBEStats[18].sX = 481;	gLBEStats[18].sY = 33;	gLBEStats[18].sValDx = 0;	//Vest 7
			gLBEStats[19].sX = 517;	gLBEStats[19].sY = 33;	gLBEStats[19].sValDx = 0;	//Vest 8
			gLBEStats[20].sX = 553;	gLBEStats[20].sY = 33;	gLBEStats[20].sValDx = 0;	//Vest 9
			gLBEStats[21].sX = 589;	gLBEStats[21].sY = 33;	gLBEStats[21].sValDx = 0;	//Vest 10
			gLBEStats[22].sX = 625;	gLBEStats[22].sY = 9;	gLBEStats[22].sValDx = 0;	//Vest 11
			gLBEStats[23].sX = 625;	gLBEStats[23].sY = 33;	gLBEStats[23].sValDx = 0;	//Vest 12
			gLBEStats[24].sX = 446;	gLBEStats[24].sY = 9;	gLBEStats[24].sValDx = 0;	//CPack 1
			gLBEStats[25].sX = 482;	gLBEStats[25].sY = 9;	gLBEStats[25].sValDx = 0;	//CPack 2
			gLBEStats[26].sX = 446;	gLBEStats[26].sY = 33;	gLBEStats[26].sValDx = 0;	//CPack 3
			gLBEStats[27].sX = 482;	gLBEStats[27].sY = 33;	gLBEStats[27].sValDx = 0;	//CPack 4
			gLBEStats[28].sX = 518;	gLBEStats[28].sY = 9;	gLBEStats[28].sValDx = 0;	//CPack 5
			gLBEStats[29].sX = 518;	gLBEStats[29].sY = 33;	gLBEStats[29].sValDx = 0;	//CPack 6
			gLBEStats[30].sX = 518;	gLBEStats[30].sY = 57;	gLBEStats[30].sValDx = 0;	//CPack 7
			gLBEStats[31].sX = 0;	gLBEStats[31].sY = 0;	gLBEStats[31].sValDx = 0;	//CPack 8
			gLBEStats[32].sX = 0;	gLBEStats[32].sY = 0;	gLBEStats[32].sValDx = 0;	//CPack 9
			gLBEStats[33].sX = 0;	gLBEStats[33].sY = 0;	gLBEStats[33].sValDx = 0;	//CPack 10
			gLBEStats[34].sX = 0;	gLBEStats[34].sY = 0;	gLBEStats[34].sValDx = 0;	//CPack 11
			gLBEStats[35].sX = 0;	gLBEStats[35].sY = 0;	gLBEStats[35].sValDx = 0;	//CPack 12
			gLBEStats[36].sX = 446;	gLBEStats[36].sY = 9;	gLBEStats[36].sValDx = 0;	//BPack 1
			gLBEStats[37].sX = 446;	gLBEStats[37].sY = 33;	gLBEStats[37].sValDx = 0;	//BPack 2
			gLBEStats[38].sX = 446;	gLBEStats[38].sY = 57;	gLBEStats[38].sValDx = 0;	//BPack 3
			gLBEStats[39].sX = 446;	gLBEStats[39].sY = 81;	gLBEStats[39].sValDx = 0;	//BPack 4
			gLBEStats[40].sX = 482;	gLBEStats[40].sY = 9;	gLBEStats[40].sValDx = 0;	//BPack 5
			gLBEStats[41].sX = 482;	gLBEStats[41].sY = 33;	gLBEStats[41].sValDx = 0;	//BPack 6
			gLBEStats[42].sX = 482;	gLBEStats[42].sY = 57;	gLBEStats[42].sValDx = 0;	//BPack 7
			gLBEStats[43].sX = 482;	gLBEStats[43].sY = 81;	gLBEStats[43].sValDx = 0;	//BPack 8
			gLBEStats[44].sX = 518;	gLBEStats[44].sY = 9;	gLBEStats[44].sValDx = 0;	//BPack 9
			gLBEStats[45].sX = 518;	gLBEStats[45].sY = 33;	gLBEStats[45].sValDx = 0;	//BPack 10
			gLBEStats[46].sX = 518;	gLBEStats[46].sY = 57;	gLBEStats[46].sValDx = 0;	//BPack 11
			gLBEStats[47].sX = 518;	gLBEStats[47].sY = 81;	gLBEStats[47].sValDx = 0;	//BPack 12
		}
	}
}

void InitItemDescriptionBoxOffsets(OBJECTTYPE * pObject)
{
	if(UsingNewAttachmentSystem()==true){
		INT16 sY = gsInvDescY + sNASYCorrection(pObject);
		INT16 sX = gsInvDescX + sNASXCorrection(pObject);

		// Initiate variables for Enhanced Item Description Box system
		if(guiCurrentScreen ==MAP_SCREEN)	//Strategic
		{
			if(UsingEDBSystem() > 0)
			{
				//WarmSteel - LBE and money do not get a NAS discription box.
				if(!UseNASDesc(pObject)){
					ITEMDESC_CALIBER_X		= (105 + sX);
					ITEMDESC_CALIBER_Y		= ( 66 + sY);
					ITEMDESC_CALIBER_WIDTH	= 149;
					ITEMDESC_NAME_X			= (  7 + sX);
					ITEMDESC_NAME_Y			= ( 65 + sY);
					ITEMDESC_ITEM_X			= ( 25 + sX);
					ITEMDESC_ITEM_Y			= (  6 + sY);
					ITEMDESC_ITEM_STATUS_X	= ( 18 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (28 + sX);
					ITEMDESC_AMMO_Y = (44 + sY);
					ITEMDESC_DONE_X = (199 + sX);
					ITEMDESC_DONE_Y = (334 + sY);

				} else {

					ITEMDESC_CALIBER_X		= (105 + sX);
					ITEMDESC_CALIBER_Y		= ( 66 + sY);
					ITEMDESC_CALIBER_WIDTH	= 149;
					ITEMDESC_NAME_X			= (  7 + sX);
					ITEMDESC_NAME_Y			= ( 65 + sY);
					ITEMDESC_ITEM_X			= ( 54 +sX);
					ITEMDESC_ITEM_Y			= ( -51 +sY);
					ITEMDESC_ITEM_STATUS_X	= ( 47 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( -4 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (55 + sX);
					ITEMDESC_AMMO_Y = (-14 + sY);
					ITEMDESC_DONE_X = (199 + sX);
					ITEMDESC_DONE_Y = (334 + sY);
				}
			}
			else
			{
				//WarmSteel - LBE and money do not get a NAS discription box.
				if(!UseNASDesc(pObject)){
					ITEMDESC_CALIBER_X		= (105 + sX);
					ITEMDESC_CALIBER_Y		= ( 66 + sY);
					ITEMDESC_CALIBER_WIDTH	= 149;
					ITEMDESC_NAME_X			= (  7 + sX);
					ITEMDESC_NAME_Y			= ( 65 + sY);
					ITEMDESC_ITEM_X			= ( 25 + sX);
					ITEMDESC_ITEM_Y			= (  6 + sY);
					ITEMDESC_ITEM_STATUS_X	= ( 18 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (28 + sX);
					ITEMDESC_AMMO_Y = (44 + sY);
					ITEMDESC_DONE_X = (204 + sX);
					ITEMDESC_DONE_Y = (107 + sY);
				} else {
					ITEMDESC_CALIBER_X		= (105 + sX);
					ITEMDESC_CALIBER_Y		= ( 65 + sY);
					ITEMDESC_CALIBER_WIDTH	= 149;
					ITEMDESC_NAME_X			= (  7 + sX);
					ITEMDESC_NAME_Y			= ( 65 + sY);
					ITEMDESC_ITEM_X			= ( 54 + sX);
					ITEMDESC_ITEM_Y			= ( -51 + sY);
					ITEMDESC_ITEM_STATUS_X	= ( 47 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( -4 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (54 + sX);
					ITEMDESC_AMMO_Y = (-15 + sY);
					ITEMDESC_DONE_X = (204 + sX);
					ITEMDESC_DONE_Y = (107 + sY);
				}
			}
		}
		else	//Tactical
		{
			if(UsingEDBSystem() == 1) // NIV EDB
			{
				//WarmSteel - LBE and money do not get a NAS discription box.
				if(!UseNASDesc(pObject)){
					// Icon locations, Tactical
					ITEMDESC_CALIBER_X		= (50 + sX);
					ITEMDESC_CALIBER_Y		= (78 + sY);
					ITEMDESC_CALIBER_WIDTH	= 240;
					ITEMDESC_NAME_X			= (16 + sX);
					ITEMDESC_NAME_Y			= (67 + sY);
					ITEMDESC_ITEM_X			= ( 15 + sX);
					ITEMDESC_ITEM_Y			= ( 8  + sY);
					ITEMDESC_ITEM_STATUS_X	= (  6 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (15 + sX);
					ITEMDESC_AMMO_Y = (43 + sY);
				} else {
					ITEMDESC_CALIBER_X		= (-180 + sX);
					ITEMDESC_CALIBER_Y		= (180 + sY);
					ITEMDESC_CALIBER_WIDTH	= 240;
					ITEMDESC_NAME_X			= (-180 + sX);
					ITEMDESC_NAME_Y			= (180 + sY);
					ITEMDESC_ITEM_X			= ( -133 + sX);
					ITEMDESC_ITEM_Y			= ( 63 + sY);
					ITEMDESC_ITEM_STATUS_X	= (  -143 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 109 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = (-135 + sX);
					ITEMDESC_AMMO_Y = (99 + sY);
				}
			} else {
			
				//WarmSteel - LBE and money do not get a NAS discription box.
				if(!UseNASDesc(pObject)){
					ITEMDESC_CALIBER_X		= (16 + sX); // Gun Caliber string location, Tactical, money or LBE
					ITEMDESC_CALIBER_Y		= (67 + sY);
					ITEMDESC_CALIBER_WIDTH	= 240;//142;
					ITEMDESC_NAME_X			= (16 + sX);
					ITEMDESC_NAME_Y			= (67 + sY);
					ITEMDESC_ITEM_X			= ( 15 + sX);
					ITEMDESC_ITEM_Y			= ( 8 + sY);
					ITEMDESC_ITEM_STATUS_X	= (  6 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = 15 + sX;
					ITEMDESC_AMMO_Y = 43 + sY;
				} else {
					// Icon locations, TACTICAL
					ITEMDESC_CALIBER_X		= (-80 + sX); // Gun Caliber string location, Tactical
					ITEMDESC_CALIBER_Y		= (180 + sY);
					ITEMDESC_CALIBER_WIDTH	= 240;//142;
					ITEMDESC_NAME_X			= (-80 + sX);
					ITEMDESC_NAME_Y			= (180 + sY);
					ITEMDESC_ITEM_X			= ( -33 + sX);
					ITEMDESC_ITEM_Y			= ( 64 + sY);
					ITEMDESC_ITEM_STATUS_X	= (  -42 + sX );
					ITEMDESC_ITEM_STATUS_Y	= ( 109 + sY );
					ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
					ITEMDESC_AMMO_X = -33 + sX;
					ITEMDESC_AMMO_Y = 99 + sY;
				}
			}
		}
	} else {
		INT16 sY = gsInvDescY;
		INT16 sX = gsInvDescX;

		// Initiate variables for Enhanced Item Description Box system
		if(guiCurrentScreen ==MAP_SCREEN)	//Strategic
		{
			if(UsingEDBSystem() > 0)
			{
				// STRATEGIC Description Box Icons
				ITEMDESC_CALIBER_X		= (105 + sX);
				ITEMDESC_CALIBER_Y		= ( 66 + sY);
				ITEMDESC_CALIBER_WIDTH	= 149;
				ITEMDESC_NAME_X			= (  7 + sX);
				ITEMDESC_NAME_Y			= ( 65 + sY);
				ITEMDESC_ITEM_X			= ( 25 + sX);
				ITEMDESC_ITEM_Y			= (  6 + sY);
				ITEMDESC_ITEM_STATUS_X	= ( 18 + sX );
				ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
				ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
				ITEMDESC_AMMO_X = 10 + 16 + sX;
				ITEMDESC_AMMO_Y = 50 - 6 + sY;
				ITEMDESC_DONE_X = 199 + sX;
				ITEMDESC_DONE_Y = 334 + sY;
			}
			else
			{
				ITEMDESC_CALIBER_X		= (105 + sX);
				ITEMDESC_CALIBER_Y		= ( 66 + sY);
				ITEMDESC_CALIBER_WIDTH	= 149;
				ITEMDESC_NAME_X			= (  7 + sX);
				ITEMDESC_NAME_Y			= ( 65 + sY);
				ITEMDESC_ITEM_X			= ( 25 + sX);
				ITEMDESC_ITEM_Y			= (  6 + sY);
				ITEMDESC_ITEM_STATUS_X	= ( 18 + sX );
				ITEMDESC_ITEM_STATUS_Y	= ( 53 + sY );
				ITEMDESC_ITEM_STATUS_HEIGHT	= 40;
				ITEMDESC_AMMO_X = 10 + 18 + sX;
				ITEMDESC_AMMO_Y = 50 - 6 + sY;
				ITEMDESC_DONE_X = 204 + sX;
				ITEMDESC_DONE_Y = 107 + sY;
			}
		}
		else	//Tactical
		{
			if(UsingEDBSystem() == 1) // NIV EDB
			{
				ITEMDESC_CALIBER_X		= ( 50 + sX);
				ITEMDESC_CALIBER_Y		= ( 78 + sY);
				ITEMDESC_CALIBER_WIDTH	= 142;
				ITEMDESC_NAME_X			= ( 16 + sX);
				ITEMDESC_NAME_Y			= ( 67 + sY);
				ITEMDESC_ITEM_X			= (  8 + sX);
				ITEMDESC_ITEM_Y			= ( 11 + sY);
				ITEMDESC_ITEM_STATUS_X	= (  6 + sX );
				ITEMDESC_ITEM_STATUS_Y	= ( 60 + sY );
				ITEMDESC_ITEM_STATUS_HEIGHT	= 50;
				ITEMDESC_AMMO_X = 10 + sX;
				ITEMDESC_AMMO_Y = 50 + sY;
			} else {
				ITEMDESC_CALIBER_X		= (162 + sX); // Gun Caliber string location, Tactical
				ITEMDESC_CALIBER_Y		= ( 67 + sY);
				ITEMDESC_CALIBER_WIDTH	= 142;
				ITEMDESC_NAME_X			= ( 16 + sX);
				ITEMDESC_NAME_Y			= ( 67 + sY);
				ITEMDESC_ITEM_X			= (  8 + sX);
				ITEMDESC_ITEM_Y			= ( 11 + sY);
				ITEMDESC_ITEM_STATUS_X	= (  6 + sX );
				ITEMDESC_ITEM_STATUS_Y	= ( 60 + sY );
				ITEMDESC_ITEM_STATUS_HEIGHT	= 50;
				ITEMDESC_AMMO_X = 10 + sX;
				ITEMDESC_AMMO_Y = 50 + sY;
			}
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

		// HEADROCK HAM 4: Secondary icons
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoSecondaryIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoSecondaryIcon ) );

		// HEADROCK HAM 4: Advanced Icons
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		GetMLGFilename( VObjectDesc.ImageFile, MLG_ITEMINFOADVANCEDICONS );	// WANNE: Now the icons are for multi-language
		//strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoAdvancedIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoAdvancedIcon ) );

		// Flugente: added icons for WH40K
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoWH40KIcons.STI" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemInfoWH40KIcon ) );
	}

	return (TRUE);
}

void InternalInitEDBTooltipRegion( OBJECTTYPE * gpItemDescObject, UINT32 guiCurrentItemDescriptionScreen )
{
	INT32		cnt, cnt2;
	INT32		threshold;
	INT32		iRegionsCreated = 0;
	INT32		iFirstDataRegion = 0;
	UINT8		ubRegionOffset = 0; // HEADROCK HAM 5: To make it easier to edit.

	InitEDBCoords( gpItemDescObject );

	//start by deleting the currently defined regions if they exist
	for ( cnt = 0; cnt < NUM_UDB_FASTHELP_REGIONS; cnt++ )
	{
		if( gUDBFasthelpRegions[ cnt ].IDNumber != 0 )
			MSYS_RemoveRegion( &gUDBFasthelpRegions[ cnt ]);
	}

	if( guiCurrentScreen == MAP_SCREEN )
		threshold = 13;
	else
		threshold = 11;


	/////////////////////////////////////////////////////////////////////////////////
	// WEAPONS
	/////////////////////////////////////////////////////////////////////////////////
	if ( (Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH) && gubDescGenPage == 0 )
	{
		if (gubDescBoxPage == 1)
		{
			/////////////////// HEADERS

			for (cnt = 0; cnt < 3; cnt++)
			{
				/////////////////// HEADER #1: Primary Properties
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenHeaderRegions[cnt].sLeft),
					(INT16)(gItemDescGenHeaderRegions[cnt].sTop),
					(INT16)(gItemDescGenHeaderRegions[cnt].sRight),
					(INT16)(gItemDescGenHeaderRegions[cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					if (cnt > 0)
					{
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt+1 ] );
					}
					else
					{
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt ] );
					}
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
			}

			////////////////// INDEXES
			for (cnt = 0; cnt < 3; cnt++)
			{
				for (cnt2 = 0; cnt2 < 4; cnt2++)
				{
					MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
						(INT16)(gItemDescGenIndexRegions[cnt][cnt2].sLeft),
						(INT16)(gItemDescGenIndexRegions[cnt][cnt2].sTop),
						(INT16)(gItemDescGenIndexRegions[cnt][cnt2].sRight),
						(INT16)(gItemDescGenIndexRegions[cnt][cnt2].sBottom),
						MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		
						MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt2 ] );
						SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
		
						iRegionsCreated++;
				}
			}

			iFirstDataRegion = iRegionsCreated;
			CHAR16 pStr[1000];

			///////////////// PRIMARY DATA - ICONS
			for (cnt = 0; cnt < 13; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					if(UsingNewCTHSystem() == true)
					{
						// with the new Laser Performance Bonus we need to display a different text for laser on a weapons general tab
						if ( cnt == 6
							&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) )
							swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 64 ], szUDBGenWeaponsStatsExplanationsTooltipText[ cnt ]);
						else if ( cnt == 10 && gGameExternalOptions.fAdvRepairSystem && !gSkillTraitValues.fTETraitsCanRestoreItemThreshold )
							swprintf( pStr, L"%s%s", szUDBGenWeaponsStatsTooltipText[ cnt ], szUDBGenWeaponsStatsExplanationsTooltipText[ 22 ]);
						else
							swprintf( pStr, L"%s%s", szUDBGenWeaponsStatsTooltipText[ cnt ], szUDBGenWeaponsStatsExplanationsTooltipText[ cnt ]);
					}
					else
					{
						if ( cnt == 11 && gGameExternalOptions.fAdvRepairSystem && !gSkillTraitValues.fTETraitsCanRestoreItemThreshold )
							swprintf( pStr, L"%s", gzWeaponStatsFasthelpTactical[ 32 ]);
						else
							swprintf( pStr, L"%s", gzWeaponStatsFasthelpTactical[ cnt ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), pStr );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			// HEADROCK HAM 4: TODO in the future: Add tooltips explaining how much bonus is given by EACH ATTACHMENT.

			///////////////// AP COST DATA - ICONS
			for (cnt = 13; cnt < 19; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					if(UsingNewCTHSystem() == true)
						swprintf( pStr, L"%s%s", szUDBGenWeaponsStatsTooltipText[ cnt ], szUDBGenWeaponsStatsExplanationsTooltipText[ cnt ]);
					else
						swprintf( pStr, L"%s", gzWeaponStatsFasthelpTactical[ cnt ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), pStr );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			///////////////// BURST/AUTOFIRE DATA - ICONS
			for (cnt = 19; cnt < 23; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					if(UsingNewCTHSystem() == true)
						swprintf( pStr, L"%s%s", szUDBGenWeaponsStatsTooltipText[ cnt ], szUDBGenWeaponsStatsExplanationsTooltipText[ cnt ]);
					else
						swprintf( pStr, L"%s", gzWeaponStatsFasthelpTactical[ cnt ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), pStr );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			/////////////////////////////////////////////////////////
			// Now enable regions that fit this specific weapon's information.

			/////////////////// ACCURACY
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 0;
				}
				else
				{
					ubRegionOffset = 2;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// DAMAGE
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_BLADE|IC_PUNCH|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				ubRegionOffset = 1;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// RANGE
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 2;
				}
				else
				{
					ubRegionOffset = 0;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );			
			}

			/////////////////// GUN HANDLING
			if ( UsingNewCTHSystem() == TRUE && Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				ubRegionOffset = 3;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );	
			}

			/////////////////// AIMING LEVELS
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 4;
				}
				else
				{
					ubRegionOffset = 3;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );			
			}

			/////////////////// OCTH AIMING BONUS
			if ( UsingNewCTHSystem() == false && 
				(GetFlatAimBonus( gpItemDescObject ) != 0 || Item[gpItemDescObject->usItem].aimbonus != 0) )
			{
				ubRegionOffset = 4;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// SCOPE MAGNIFICATION
			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN )
			{
				ubRegionOffset = 5;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// OCTH MINIMUM RANGE FOR AIMING BONUS
			if( UsingNewCTHSystem() == false && 
				( Item[gpItemDescObject->usItem].minrangeforaimbonus > 0 || GetMinRangeForAimBonus( NULL, gpItemDescObject ) > 0 ) )
			{
				ubRegionOffset = 5;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// PROJECTION FACTOR / NCTH BEST LASER RANGE
			// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor but we still need the mouse region
			if (UsingNewCTHSystem() && ( GetProjectionFactor( gpItemDescObject ) > 1.0 || GetBestLaserRange( gpItemDescObject ) > 0
				&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) )
			{
				ubRegionOffset = 6;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// OCTH TO=HIT BONUS
			if (UsingNewCTHSystem() == false && 
				(Item[gpItemDescObject->usItem].tohitbonus != 0 || GetFlatToHitBonus( gpItemDescObject ) != 0) )
			{
				ubRegionOffset = 6;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// OCTH BEST LASER RANGE
			if (!UsingNewCTHSystem() && (Item[gpItemDescObject->usItem].bestlaserrange > 0 || GetAverageBestLaserRange( gpItemDescObject ) > 0 ) )
			{
				ubRegionOffset = 7;
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// FLASH SUPPRESSION
			if (IsFlashSuppressorAlt( gpItemDescObject ) == TRUE)
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 7;
				}
				else
				{
					ubRegionOffset = 8;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// LOUDNESS
			if (Weapon[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubAttackVolume > 0 )
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 8;
				}
				else
				{
					ubRegionOffset = 9;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// RELIABILITY
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 9;
				}
				else
				{
					ubRegionOffset = 10;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// REPAIR EASE
			{
				if (UsingNewCTHSystem() == true)
				{
					ubRegionOffset = 10;
				}
				else
				{
					ubRegionOffset = 11;
				}
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + ubRegionOffset ] );
			}

			/////////////////// AP TO DRAW
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_PUNCH) && !Item[ gpItemDescObject->usItem].rocketlauncher )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 13 ] );
			}
			
			/////////////////// AP TO SINGLE ATTACK
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 14 ] );
			}

			/////////////////// AP TO BURST
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 15 ] );
			}

			/////////////////// AP TO AUTOFIRE
			if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 16 ] );
			}

			/////////////////// AP TO RELOAD
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 17 ] );
			}

			/////////////////// AP TO RELOAD MANUALLY
			if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher && Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 18 ] );
			}

			/////////////////// RECOIL X/Y
			if( UsingNewCTHSystem() == true )
			{
				if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher && (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 || GetShotsPerBurst(gpItemDescObject)> 0 ) )
				{
					// HEADROCK HAM 5: One value to rule them all.
					// MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 18 ] );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 20 ] );
				}
			}
			else	/////////////////// BIPOD & BURST PENALTY
			{
				if( GetBurstPenalty(gpItemDescObject) > 0 )
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 19 ] );
				if( GetBipodBonus(gpItemDescObject) > 0 )
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 20 ] );
			}

			/////////////////// BULLETS PER 5 AP
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher && GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 21 ] );
			}

			/////////////////// AUTOFIRE PENALTY
			if( UsingNewCTHSystem() == false && GetAutoPenalty(gpItemDescObject) > 0 )
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 22 ] );
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// ARMOR
	/////////////////////////////////////////////////////////////////////////////////

	else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR )
	{
		if (gubDescBoxPage == 1)
		{
			/////////////////// HEADERS

			for (cnt = 0; cnt < 2; cnt++)
			{
				/////////////////// HEADER #1: Primary Properties
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenHeaderRegions[cnt].sLeft),
					(INT16)(gItemDescGenHeaderRegions[cnt].sTop),
					(INT16)(gItemDescGenHeaderRegions[cnt].sRight),
					(INT16)(gItemDescGenHeaderRegions[cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
			}

			////////////////// INDEX
			for (cnt = 0; cnt < 4; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenIndexRegions[0][cnt].sLeft),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sTop),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sRight),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
	
					iRegionsCreated++;
			}

			iFirstDataRegion = iRegionsCreated;
			CHAR16 pStr[1000];

			///////////////// PRIMARY DATA - ICONS
			for (cnt = 0; cnt < 4; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					if ( cnt == 3 && gGameExternalOptions.fAdvRepairSystem && !gSkillTraitValues.fTETraitsCanRestoreItemThreshold )
						swprintf( pStr, L"%s%s", szUDBGenArmorStatsTooltipText[ cnt ], szUDBGenArmorStatsExplanationsTooltipText[ 4 ]);
					else
						swprintf( pStr, L"%s%s", szUDBGenArmorStatsTooltipText[ cnt ], szUDBGenArmorStatsExplanationsTooltipText[ cnt ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), pStr );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			//////////////////// PROTECTION
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 0 ] );
			}

			//////////////////// COVERAGE
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 1 ] );
			}

			//////////////////// DEGRADE RATE
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 2 ] );
			}
			//////////////////// REPAIR EASE
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}
		}
	}

	else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_AMMO )
	{
		if (gubDescBoxPage == 1)
		{
			/////////////////// HEADERS
			for (cnt = 0; cnt < 2; cnt++)
			{
				/////////////////// HEADER #1: Primary Properties
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenHeaderRegions[cnt].sLeft),
					(INT16)(gItemDescGenHeaderRegions[cnt].sTop),
					(INT16)(gItemDescGenHeaderRegions[cnt].sRight),
					(INT16)(gItemDescGenHeaderRegions[cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
			}

			////////////////// INDEX
			for (cnt = 0; cnt < 4; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenIndexRegions[0][cnt].sLeft),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sTop),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sRight),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
	
					iRegionsCreated++;
			}

			iFirstDataRegion = iRegionsCreated;
			CHAR16 pStr[1000];

			///////////////// PRIMARY DATA - ICONS
			for (cnt = 0; cnt < 6; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					swprintf( pStr, L"%s%s", szUDBGenAmmoStatsTooltipText[ cnt ], szUDBGenAmmoStatsExplanationsTooltipText[ cnt ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), pStr );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			//////////////////// ARMOR PIERCING
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 0 ] );
			}

			//////////////////// TUMBLE
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 1 ] );
			}
			
			//////////////////// PRE-ARMOR EXPLOSION
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 2 ] );
			}

			// Flugente
			if ( gGameExternalOptions.fWeaponOverheating )
			{	
				//////////////////// TEMPERATURE MODIFICATOR
				{
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
				}
			}

			//////////////////// DIRT MODIFICATOR
			{
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 5 ] );
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// EXPLOSIVES
	/////////////////////////////////////////////////////////////////////////////////

	else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
	{
		if (gubDescBoxPage == 1)
		{
			/////////////////// HEADERS

			for (cnt = 0; cnt < 2; cnt++)
			{
				/////////////////// HEADER #1: Primary Properties
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenHeaderRegions[cnt].sLeft),
					(INT16)(gItemDescGenHeaderRegions[cnt].sTop),
					(INT16)(gItemDescGenHeaderRegions[cnt].sRight),
					(INT16)(gItemDescGenHeaderRegions[cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
			}
			
			////////////////// INDEX
			for (cnt = 0; cnt < 4; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenIndexRegions[0][cnt].sLeft),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sTop),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sRight),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
	
					iRegionsCreated++;
			}

			iFirstDataRegion = iRegionsCreated;
			CHAR16 pStr[1000];

			///////////////// PRIMARY DATA - ICONS
			for (cnt = 0; cnt < 12; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			//////////////////// DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 0 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 0 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 0 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 0 ] );
			}

			//////////////////// STUN-DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 1 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 1 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 1 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 1 ] );
			}

			// HEADROCK HAM 5
			//////////////////// EXPLODE ON IMPACT
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].fExplodeOnImpact )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 2 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 2 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 2 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 2 ] );
			}

			//////////////////// BLAST RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType != 5 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType != 1 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 3 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 3 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// STUN BLAST RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 1 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 4 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 4 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// NOISE BLAST RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 5 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 5 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 5 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// TEARGAS START RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 2 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 6 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 6 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// MUSTARD GAS START RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 3 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 7 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 7 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// LIGHT START RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 4 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 8 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 8 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// SMOKE START RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 6 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 9 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 9 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// INCENDIARY START RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 8 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 10 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 10 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 3 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 3 ] );
			}

			//////////////////// TEARGAS END RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 2 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 11 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 11 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 4 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 4 ] );
			}

			//////////////////// MUSTARD GAS END RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 3 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 12 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 12 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 4 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 4 ] );
			}

			//////////////////// LIGHT END RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 4 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 13 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 13 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 4 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 4 ] );
			}

			//////////////////// SMOKE END RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 6 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 14 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 14 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 4 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 4 ] );
			}

			//////////////////// NAPALM END RADIUS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
				&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 8 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 15 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 15 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 4 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 4 ] );
			}

			//////////////////// DURATION
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 16 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 16 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 5 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 5 ] );
			}

			// HEADROCK HAM 5: Fragmentation
			//////////////////// NUMBER OF FRAGMENTS
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 17 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 17 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 6 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 6 ] );
			}

			//////////////////// FRAGMENT DAMAGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 18 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 18 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 7 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 7 ] );
			}

			//////////////////// FRAGMENT RANGE
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 19 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 19 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 8 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 8 ] );
			}

			//////////////////// LOUDNESS
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 20 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 20 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 9 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 9 ] );
			}

			//////////////////// VOLATILITY
			if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 21 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 21 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 10 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 10 ] );
			}

			//////////////////// REPAIR EASE
			{
				swprintf( pStr, L"%s%s", szUDBGenExplosiveStatsTooltipText[ 22 ], szUDBGenExplosiveStatsExplanationsTooltipText[ 22 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 11 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 11 ] );
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// OTHER ITEMS
	/////////////////////////////////////////////////////////////////////////////////

	else
	{
		if (gubDescBoxPage == 1)
		{
			/////////////////// HEADERS

			for (cnt = 0; cnt < 2; cnt++)
			{
				/////////////////// HEADER #1: Primary Properties
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenHeaderRegions[cnt].sLeft),
					(INT16)(gItemDescGenHeaderRegions[cnt].sTop),
					(INT16)(gItemDescGenHeaderRegions[cnt].sRight),
					(INT16)(gItemDescGenHeaderRegions[cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBHeaderTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
			}
			
			////////////////// INDEX
			for (cnt = 0; cnt < 4; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenIndexRegions[0][cnt].sLeft),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sTop),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sRight),
					(INT16)(gItemDescGenIndexRegions[0][cnt].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
	
					iRegionsCreated++;
			}

			iFirstDataRegion = iRegionsCreated;
			CHAR16 pStr[1000];

			///////////////// PRIMARY DATA - ICONS
			for (cnt = 0; cnt < 3; cnt++)
			{
				MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
					(INT16)(gItemDescGenRegions[cnt][0].sLeft),
					(INT16)(gItemDescGenRegions[cnt][0].sTop),
					(INT16)(gItemDescGenRegions[cnt][0].sRight),
					(INT16)(gItemDescGenRegions[cnt][0].sBottom),
					MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
					MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

					iRegionsCreated++;
			}

			//////////////////// REPAIR EASE
			// not for weapons. They have this one their primary page
			if ( !(Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH) )
			{
				swprintf( pStr, L"%s%s", szUDBGenCommonStatsTooltipText[ 0 ], szUDBGenCommonStatsExplanationsTooltipText[ 0 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 0 ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 0 ] );
			}
			//////////////////// LBE Stuff
			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
			{
				//////////////////// LBE AVAILABLE VOLUME (for MOLLE carriers)
				if ( LoadBearingEquipment[ Item[ gpItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume > 0 )
				{
					swprintf( pStr, L"%s%s", szUDBGenCommonStatsTooltipText[ 1 ], szUDBGenCommonStatsExplanationsTooltipText[ 1 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 1 ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 1 ] );
				}
				//////////////////// POCKET VOLUME (for MOLLE pockets)
				if ( Item[ gpItemDescObject->usItem ].nasAttachmentClass != 0 && GetFirstPocketOnItem(gpItemDescObject->usItem) != 0 )
				{
					swprintf( pStr, L"%s%s", szUDBGenCommonStatsTooltipText[ 2 ], szUDBGenCommonStatsExplanationsTooltipText[ 2 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + 2 ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + 2 ] );
				}
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// SECONDARY PROPERTIES TOOLTIPS
	// These are item-flags, shown as large icons on the right hand column of the General
	// UDB page. They appear only for non-weapon items (no room in the weapon page).

	if (gubDescBoxPage == 1 && 
		( !(Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH ) || gubDescGenPage == 1 ) )
	{
		iFirstDataRegion = iRegionsCreated;
		CHAR16 pStr[1000];

		for (cnt = 0; cnt < GEN_SEC_REGION_ENTRIES; cnt++)
		{
			MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
				(INT16)(gItemDescGenSecondaryRegions[cnt].sLeft),
				(INT16)(gItemDescGenSecondaryRegions[cnt].sTop),
				(INT16)(gItemDescGenSecondaryRegions[cnt].sRight),
				(INT16)(gItemDescGenSecondaryRegions[cnt].sBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
				SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

				iRegionsCreated++;
		}

		cnt = 0;

		if (Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR)
		{
			//////////////////// EXPLOSIVE RESISTANCE
			if (Item[ gpItemDescObject->usItem ].flakjacket)
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 5 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 5 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			//////////////////// FIRE RESISTANCE
			if ( Item[gpItemDescObject->usItem].sFireResistance )
			{
				CHAR16 pStr2[256];
				swprintf( pStr2, szUDBGenSecondaryStatsExplanationsTooltipText[45], Item[gpItemDescObject->usItem].sFireResistance );

				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[45], pStr2 );
				SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
				cnt++;
			}
		}

		if (Item[ gpItemDescObject->usItem ].usItemClass & IC_AMMO)
		{
			/////////////////// TRACER AMMO
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 0 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 0 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			/////////////////// ANTI-TANK AMMO
			if ( AmmoTypes[Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubAmmoType].dDamageModifierTank >= 0.1f )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 1 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 1 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			/////////////////// IGNORE ARMOR AMMO
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 2 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 2 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			/////////////////// ACIDIC AMMO
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 3 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 3 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			/////////////////// LOCKBUSTING AMMO
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 4 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 4 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			////////////////// ANTI-MATERIEL AMMO
			if ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ammoflag & AMMO_ANTIMATERIEL )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 37 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 37 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}
		}

		if (Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV)
		{
			////////////////// LOCK BOMB
			if (Item[ gpItemDescObject->usItem ].lockbomb)
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 25 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 25 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}
		}

		/////////////////////////////////////////////////
		// Draw stats that can be had by any item.

		//////////////////// WATERPROOF
		if (!Item[ gpItemDescObject->usItem ].waterdamages)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 6 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 6 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}
 
		//////////////////// ELECTRONIC
		if (Item[ gpItemDescObject->usItem ].electronic)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 7 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 7 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}
		
		//////////////////// GAS MASK
		if (Item[ gpItemDescObject->usItem ].gasmask)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 8 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 8 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// NEEDS BATTERIES
		if (Item[ gpItemDescObject->usItem ].needsbatteries)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 9 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 9 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// LOCKSMITH'S KIT
		if (Item[ gpItemDescObject->usItem ].locksmithkit)
		{
			swprintf( pStr, L"%s%s%d", szUDBGenSecondaryStatsTooltipText[ 10 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 10 ], (Item[ gpItemDescObject->usItem ].LockPickModifier > 0 ?
				( Item[ gpItemDescObject->usItem ].LockPickModifier * (*gpItemDescObject)[0]->data.objectStatus / 100 ) : Item[ gpItemDescObject->usItem ].LockPickModifier ) );
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// WIRE CUTTERS
		if (Item[ gpItemDescObject->usItem ].wirecutters)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 11 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 11 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// CROWBAR
		if (Item[ gpItemDescObject->usItem ].crowbar)
		{
			swprintf( pStr, L"%s%s%d", szUDBGenSecondaryStatsTooltipText[ 12 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 12 ], Item[ gpItemDescObject->usItem ].CrowbarModifier );
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// METAL DETECTOR
		if (Item[ gpItemDescObject->usItem ].metaldetector)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 13 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 13 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// REMOTE TRIGGER
		if (Item[ gpItemDescObject->usItem ].remotetrigger)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 14 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 14 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// REMOTE DETONATOR
		if (Item[ gpItemDescObject->usItem ].remotedetonator)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 15 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 15 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// TIMER DETONATOR
		if (Item[ gpItemDescObject->usItem ].detonator)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 16 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 16 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// GAS CAN
		if (Item[ gpItemDescObject->usItem ].gascan)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 17 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 17 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// TOOLKIT
		if (Item[ gpItemDescObject->usItem ].toolkit)
		{
			swprintf( pStr, L"%s%s%d", szUDBGenSecondaryStatsTooltipText[ 18 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 18 ], Item[ gpItemDescObject->usItem ].RepairModifier );
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// THERMAL OPTICS
		if (Item[ gpItemDescObject->usItem ].thermaloptics)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 19 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 19 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// X-RAY DEVICE
		if (Item[ gpItemDescObject->usItem ].xray)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 20 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 20 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// CANTEEN
		if (Item[ gpItemDescObject->usItem ].canteen)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 21 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 21 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// ALCOHOL
		if ( Item[gpItemDescObject->usItem].alcohol > 0.0f )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 22 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 22 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// FIRST-AID KIT
		if (Item[ gpItemDescObject->usItem ].firstaidkit)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 23 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 23 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// MEDICAL KIT
		if (Item[ gpItemDescObject->usItem ].medicalkit)
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 24 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 24 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// drinkable WATER
		UINT32 foodtype = Item[gpItemDescObject->usItem].foodtype;
		if ( foodtype > 0 )
		{
			if ( Food[foodtype].bDrinkPoints > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 26 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 26 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}

			if ( Food[foodtype].bFoodPoints > 0 )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 27 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 27 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}
		}

		//////////////////// EXTERNAL FEEDING
		if ( gGameExternalOptions.ubExternalFeeding )
		{
			if ( HasItemFlag(gpItemDescObject->usItem, AMMO_BELT) )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 28 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 28 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}
			else if ( HasItemFlag(gpItemDescObject->usItem, AMMO_BELT_VEST) )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 29 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 29 ]);
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
				cnt++;
			}
		}
		///////////////////// DEFUSAL KIT
		if ( Item[gpItemDescObject->usItem].DisarmModifier > 0)
		{
			swprintf( pStr, L"%s%s%d", szUDBGenSecondaryStatsTooltipText[ 30 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 30 ], Item[gpItemDescObject->usItem].DisarmModifier);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// COVERT ITEM
		if ( HasItemFlag(gpItemDescObject->usItem, COVERT) )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 31 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 31 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// NOT DAMAGEABLE
		if ( Item[gpItemDescObject->usItem].damageable == 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 32 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 32 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// METAL
		if ( Item[gpItemDescObject->usItem].metal > 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 33 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 33 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// SINKS
		if ( Item[gpItemDescObject->usItem].sinks > 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 34 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 34 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// TWO HANDED
		if ( Item[gpItemDescObject->usItem].twohanded > 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 35 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 35 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// BLOCKS IRON SIGHTS
		if ( Item[gpItemDescObject->usItem].blockironsight > 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[ 36 ], szUDBGenSecondaryStatsExplanationsTooltipText[ 36 ]);
			SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + cnt ]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + cnt ] );
			cnt++;
		}

		//////////////////// DISEASE
		if ( gGameExternalOptions.fDisease )
		{
			if ( HasItemFlag( gpItemDescObject->usItem, DISEASEPROTECTION_1 ) )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[38], szUDBGenSecondaryStatsExplanationsTooltipText[38] );
				SetRegionFastHelpText( &(gUDBFasthelpRegions[iFirstDataRegion + cnt]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
				cnt++;
			}
			else if ( HasItemFlag( gpItemDescObject->usItem, DISEASEPROTECTION_2 ) )
			{
				swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[39], szUDBGenSecondaryStatsExplanationsTooltipText[39] );
				SetRegionFastHelpText( &(gUDBFasthelpRegions[iFirstDataRegion + cnt]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
				cnt++;
			}
		}

		//////////////////// SHIELD
		if ( Item[gpItemDescObject->usItem].usRiotShieldStrength > 0 )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[40], szUDBGenSecondaryStatsExplanationsTooltipText[40] );
			SetRegionFastHelpText( &(gUDBFasthelpRegions[iFirstDataRegion + cnt]), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}

		//////////////////// CAMERA
		if ( HasItemFlag( gpItemDescObject->usItem, CAMERA ) )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[41], szUDBGenSecondaryStatsExplanationsTooltipText[41] );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}
		
		//////////////////// BURIAL MODIFIER
		if ( Item[gpItemDescObject->usItem].usBurialModifier  )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[42], szUDBGenSecondaryStatsExplanationsTooltipText[42] );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}

		//////////////////// EMPTY BLOOD BAG
		if ( HasItemFlag( gpItemDescObject->usItem, EMPTY_BLOOD_BAG ) )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[43], szUDBGenSecondaryStatsExplanationsTooltipText[43] );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}

		//////////////////// BLOOD BAG
		if ( HasItemFlag( gpItemDescObject->usItem, BLOOD_BAG ) )
		{
			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[44], szUDBGenSecondaryStatsExplanationsTooltipText[44] );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}

		//////////////////// ADMINISTRATION MODIFIER
		if ( Item[gpItemDescObject->usItem].usAdministrationModifier )
		{
			CHAR16 pStr2[256];
			swprintf( pStr2, szUDBGenSecondaryStatsExplanationsTooltipText[46], Item[gpItemDescObject->usItem].usAdministrationModifier );

			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[46], pStr2 );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}

		//////////////////// HACKING MODIFIER
		if ( Item[gpItemDescObject->usItem].usHackingModifier )
		{
			CHAR16 pStr2[256];
			swprintf( pStr2, szUDBGenSecondaryStatsExplanationsTooltipText[47], Item[gpItemDescObject->usItem].usHackingModifier );

			swprintf( pStr, L"%s%s", szUDBGenSecondaryStatsTooltipText[47], pStr2 );
			SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + cnt] ), pStr );
			MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + cnt] );
			++cnt;
		}
	}

	//////////////////////////////////////////////////////
	// ADVANCED PROPERTY TOOLTIPS
	
	if (gubDescBoxPage == 2)
	{
		INT16 sFirstLine = gubDescBoxLine;
		INT16 sLastLine = gubDescBoxLine + NUM_UDB_ADV_LINES;
		INT16 cnt = 0;
		INT16 sOffsetX = 0;
		INT16 sOffsetY = 0;

		//////////////////// INDEX

		for (cnt = 0; cnt < 4; cnt++)
		{
			MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
				(INT16)(gItemDescAdvIndexRegions[0][cnt].sLeft),
				(INT16)(gItemDescAdvIndexRegions[0][cnt].sTop),
				(INT16)(gItemDescAdvIndexRegions[0][cnt].sRight),
				(INT16)(gItemDescAdvIndexRegions[0][cnt].sBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
				if( UsingNewCTHSystem() == true || cnt == 0)
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBAdvIndexTooltipText[ cnt ] );
				else if( cnt > 0 )
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBAdvIndexTooltipText[ 4 ] );
				SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

				iRegionsCreated++;
		}

		/////////////////// DATA

		iFirstDataRegion = iRegionsCreated;
		CHAR16 pStr[1000];

		for (cnt = 0; cnt < NUM_UDB_ADV_LINES; cnt++)
		{
			MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
				(INT16)(gItemDescAdvRegions[cnt][0].sLeft),
				(INT16)(gItemDescAdvRegions[cnt][0].sTop),
				(INT16)(gItemDescAdvRegions[cnt][0].sRight),
				(INT16)(gItemDescAdvRegions[cnt][0].sBottom),
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
	
				MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
				SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );
				MSYS_DisableRegion( &gUDBFasthelpRegions[ iRegionsCreated ] );

				iRegionsCreated++;
		}

		cnt = 0;

		///////////////////// ACCURACY MODIFIER
		if (GetAccuracyModifier( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
			if( UsingNewCTHSystem() == true )
			{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 0 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 0 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 0 ], szUDBAdvStatsExplanationsTooltipText[ 0 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}
		///////////////////// FLAT BASE MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATBASE ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATBASE ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATBASE ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass& (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 1 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 1 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 1 ], szUDBAdvStatsExplanationsTooltipText[ 1 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// PERCENT BASE MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTBASE ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTBASE ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTBASE ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 2 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 2 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 2 ], szUDBAdvStatsExplanationsTooltipText[ 2 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// FLAT AIM MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATAIM ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATAIM ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATAIM ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 3 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 3 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 3 ], szUDBAdvStatsExplanationsTooltipText[ 3 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// PERCENT AIM MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTAIM ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTAIM ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTAIM ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 4 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 4 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 4 ], szUDBAdvStatsExplanationsTooltipText[ 4 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// AIMING LEVELS MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS ) != 0
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_AIMLEVELS ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_AIMLEVELS ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 5 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 5 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 5 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 5 ], szUDBAdvStatsExplanationsTooltipText[ 5 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}

		///////////////////// AIM BONUS MODIFIER
		if(UsingNewCTHSystem() == false)
		{
			if ( GetAimBonus( gpItemDescSoldier, gpItemDescObject, 100, 1 ) != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if( UsingNewCTHSystem() == false )
					{
						swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 47 ]);
					}
					else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 47 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 47 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 47 ], szUDBAdvStatsExplanationsTooltipText[ 47 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// TO-HIT MODIFIER
		if(UsingNewCTHSystem() == false)
		{
			if ( GetToHitBonus( gpItemDescObject, 100, 1, FALSE ) != 0 
				|| GetToHitBonus( gpItemDescObject, 100, 1, TRUE ) != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if( UsingNewCTHSystem() == false )
					{
						swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 46 ]);
					}
					else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 46 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 46 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 46 ], szUDBAdvStatsExplanationsTooltipText[ 46 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// CTH CAP MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTCAP ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTCAP ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTCAP ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 6 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 6 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 6 ], szUDBAdvStatsExplanationsTooltipText[ 6 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// GUN HANDLING MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTHANDLING ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTHANDLING ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 7 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 7 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 7 ], szUDBAdvStatsExplanationsTooltipText[ 7 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// DROP COMPENSATION MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_DROPCOMPENSATION ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 8 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 8 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 8 ], szUDBAdvStatsExplanationsTooltipText[ 8 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// TARGET TRACKING MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_TRACKING ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_TRACKING ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_TRACKING ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 9 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 9 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 9 ], szUDBAdvStatsExplanationsTooltipText[ 9 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
		}

		///////////////////// DAMAGE MODIFIER
		if (GetDamageBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 10 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 10 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 10 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 10 ], szUDBAdvStatsExplanationsTooltipText[ 10 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// MELEE DAMAGE MODIFIER
		if (GetMeleeDamageBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 11 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 11 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 11 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 11 ], szUDBAdvStatsExplanationsTooltipText[ 11 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// RANGE MODIFIER
		if (GetRangeBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 12 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 12 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 12 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 12 ], szUDBAdvStatsExplanationsTooltipText[ 12 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// SCOPE MAGNIFICATION
		if (GetHighestScopeMagnificationFactor( gpItemDescObject ) > 1.0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 13 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 13 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 13 ], szUDBAdvStatsExplanationsTooltipText[ 13 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// PROJECTION FACTOR / BEST LASER RANGE
		// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor
		if ( ( UsingNewCTHSystem() && GetBestLaserRange( gpItemDescObject ) > 0
			&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) )
			|| ( !UsingNewCTHSystem() && GetBestLaserRange( gpItemDescObject ) > 0 ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 64 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 14 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 64 ], szUDBAdvStatsExplanationsTooltipText[ 14 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}
		else if (GetProjectionFactor( gpItemDescObject ) > 1.0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 14 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 14 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 14 ], szUDBAdvStatsExplanationsTooltipText[ 14 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// PERCENT RECOIL MODIFIER
		if ( GetPercentRecoilModifier( gpItemDescObject ) != 0 )
		{
			if ( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 65 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 52 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 65 ], szUDBAdvStatsExplanationsTooltipText[ 65 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		FLOAT bRecoilModifierX;
		FLOAT bRecoilModifierY;
		GetFlatRecoilModifier( gpItemDescObject, &bRecoilModifierX, &bRecoilModifierY );
		///////////////////// LATERAL RECOIL MODIFIER
		if (bRecoilModifierX != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 15 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 15 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 15 ], szUDBAdvStatsExplanationsTooltipText[ 15 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// VERTICAL RECOIL MODIFIER
		if (bRecoilModifierY != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 16 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 16 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 16 ], szUDBAdvStatsExplanationsTooltipText[ 16 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		// HEADROCK HAM 5: Moved here because it makes more sense.
		///////////////////// MAX COUNTER FORCE
		if ((CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_STAND ) != 0 
			|| CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH ) != 0 
			|| CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE ) != 0 ) )
		{
			if( UsingNewCTHSystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_GUN )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 44 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 44 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 44 ], szUDBAdvStatsExplanationsTooltipText[ 44 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// MAX COUNTER FORCE MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 )
		{
			if( UsingNewCTHSystem() == true ) //&& Item[gpItemDescObject->usItem].usItemClass == IC_GUN )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 17 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 17 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 17 ], szUDBAdvStatsExplanationsTooltipText[ 17 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// COUNTER FORCE ACCURACY MODIFIER
		if ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
			|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 )
		{
			if( UsingNewCTHSystem() == true )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 18 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 18 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 18 ], szUDBAdvStatsExplanationsTooltipText[ 18 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
		}

		///////////////////// AP MODIFIER
		if (GetAPBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 20 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 20 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 20 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 20 ], szUDBAdvStatsExplanationsTooltipText[ 20 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// DRAW AP MODIFIER
		if (GetPercentReadyTimeAPReduction( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 21 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 21 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 21 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 21 ], szUDBAdvStatsExplanationsTooltipText[ 21 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// SINGLE-ATTACK AP MODIFIER
		if (GetPercentAPReduction( NULL, gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 22 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 22 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 22 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 22 ], szUDBAdvStatsExplanationsTooltipText[ 22 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// BURST AP MODIFIER
		if (GetPercentBurstFireAPReduction( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 23 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 23 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 23 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 23 ], szUDBAdvStatsExplanationsTooltipText[ 23 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// AUTOFIRE AP MODIFIER
		if (GetPercentAutofireAPReduction( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 24 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 24 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 24 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 24 ], szUDBAdvStatsExplanationsTooltipText[ 24 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// RELOAD AP MODIFIER
		if (GetPercentReloadTimeAPReduction( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 25 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 25 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 25 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 25 ], szUDBAdvStatsExplanationsTooltipText[ 25 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// MAGAZINE SIZE MODIFIER
		if (GetMagSizeBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 26 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 26 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 26 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 26 ], szUDBAdvStatsExplanationsTooltipText[ 26 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// BURST SIZE MODIFIER
		if (GetBurstSizeBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 27 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 27 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 27 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 27 ], szUDBAdvStatsExplanationsTooltipText[ 27 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// HIDE MUZZLE FLASH
		if (IsFlashSuppressorAlt( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 28 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 28 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 28 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 28 ], szUDBAdvStatsExplanationsTooltipText[ 28 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// LOUDNESS MODIFIER
		if (GetPercentNoiseVolume( gpItemDescObject )-100 != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 29 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 29 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 29 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 29 ], szUDBAdvStatsExplanationsTooltipText[ 29 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// ALTER ITEM SIZE
		if (CalculateItemSize( gpItemDescObject ) - Item[ gpItemDescObject->usItem ].ItemSize != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 30 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 30 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 30 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 30 ], szUDBAdvStatsExplanationsTooltipText[ 30 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// RELIABILITY MODIFIER
		if (!(Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH|IC_ARMOUR|IC_EXPLOSV)) &&
			GetReliability( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 31 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 31 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 31 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 31 ], szUDBAdvStatsExplanationsTooltipText[ 31 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// WOODLAND CAMO
		if (GetCamoBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 32 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 32 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 32 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 32 ], szUDBAdvStatsExplanationsTooltipText[ 32 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// URBAN CAMO
		if (GetUrbanCamoBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 33 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 33 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 33 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 33 ], szUDBAdvStatsExplanationsTooltipText[ 33 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// DESERT CAMO
		if (GetDesertCamoBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 34 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 34 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 34 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 34 ], szUDBAdvStatsExplanationsTooltipText[ 34 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}

		///////////////////// SNOW CAMO
		if (GetSnowCamoBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 35 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 35 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 35 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 35 ], szUDBAdvStatsExplanationsTooltipText[ 35 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// STEALTH MODIFIER
		if (GetStealthBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 36 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 36 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 36 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 36 ], szUDBAdvStatsExplanationsTooltipText[ 36 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}

		///////////////////// HEARING RANGE MODIFIER
		if (GetItemHearingRangeBonus( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 37 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 37 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 37 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 37 ], szUDBAdvStatsExplanationsTooltipText[ 37 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}

		///////////////////// VISION RANGE MODIFIER
		if (GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 38 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 38 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 38 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 38 ], szUDBAdvStatsExplanationsTooltipText[ 38 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
			}
			cnt++;
		}

		///////////////////// NIGHT VISION RANGE MODIFIER
		if (GetItemVisionRangeBonus( gpItemDescObject, 2 ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 39 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 39 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 39 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 39 ], szUDBAdvStatsExplanationsTooltipText[ 39 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// DAY VISION RANGE MODIFIER
		if (GetItemVisionRangeBonus( gpItemDescObject, 1 ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 40 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 40 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 40 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 40 ], szUDBAdvStatsExplanationsTooltipText[ 40 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// BRIGHT LIGHT VISION RANGE MODIFIER
		if (GetItemVisionRangeBonus( gpItemDescObject, 3 ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 41 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 41 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 41 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 41 ], szUDBAdvStatsExplanationsTooltipText[ 41 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// CAVE VISION RANGE MODIFIER
		if (GetItemVisionRangeBonus( gpItemDescObject, 4 ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 42 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 42 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 42 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 42 ], szUDBAdvStatsExplanationsTooltipText[ 42 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		///////////////////// PERCENT TUNNEL VISION
		if (GetItemPercentTunnelVision( gpItemDescObject ) != 0 )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				if( UsingNewCTHSystem() == false )
				{
					swprintf( pStr, L"%s", szUDBAdvStatsTooltipText[ 43 ]);
				}
				else if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 43 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 43 ]);
				}
				else
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 43 ], szUDBAdvStatsExplanationsTooltipText[ 43 ]);
				}
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 			}
			cnt++;
		}

		// Moa: used to check if general Indexes (Prop., 0, +-, =) should be shown
		INT16 fDrawGenIndexes = FALSE;

		// Flugente
		if ( gGameExternalOptions.fWeaponOverheating )
		{
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// new index line here?
				///////////////////// SINGLE SHOT TEMPERATURE
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 48 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 48 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 48 ], szUDBAdvStatsExplanationsTooltipText[ 48 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;

				///////////////////// COOLDOWN FACTOR
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 49 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 49 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 49 ], szUDBAdvStatsExplanationsTooltipText[ 49 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;

				///////////////////// JAM THRESHOLD
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 50 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 50 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 50 ], szUDBAdvStatsExplanationsTooltipText[ 50 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;

				///////////////////// DAMAGE THRESHOLD
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_WEAPON|IC_PUNCH))
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 51 ], szUDBAdvStatsExplanationsTooltipTextForWeapons[ 51 ]);
					}
					else
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 51 ], szUDBAdvStatsExplanationsTooltipText[ 51 ]);
					}
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}
			else if ( Item[gpItemDescObject->usItem].barrel == TRUE )		// for barrel items
			{
				if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// new index line here?
				///////////////////// COOLDOWN FACTOR
				if (cnt >= sFirstLine && cnt < sLastLine)
				{					
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 49 ], szUDBAdvStatsExplanationsTooltipText[ 49 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 				}
				cnt++;
			}

			// other stuff: various overheat modificators
			if ( ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) )
			{
				if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// new index line here?
				///////////////////// TEMPERATURE MODIFICATOR
				if ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 )
				{
					if (cnt >= sFirstLine && cnt < sLastLine)
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 52 ], szUDBAdvStatsExplanationsTooltipText[ 52 ]);
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
						MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 					}
					cnt++;
				}

				///////////////////// COOLDOWN FACTOR MODIFICATOR
				if ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 )
				{
					if (cnt >= sFirstLine && cnt < sLastLine)
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 53 ], szUDBAdvStatsExplanationsTooltipText[ 53 ]);
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
						MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 					}
					cnt++;
				}

				///////////////////// JAM THRESHOLD MODIFICATOR
				if ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 )
				{
					if (cnt >= sFirstLine && cnt < sLastLine)
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 54 ], szUDBAdvStatsExplanationsTooltipText[ 54 ]);
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
						MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 					}
					cnt++;
				}

				///////////////////// DAMAGE THRESHOLD MODIFICATOR
				if ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 )
				{
					if (cnt >= sFirstLine && cnt < sLastLine)
					{
						swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 55 ], szUDBAdvStatsExplanationsTooltipText[ 55 ]);
						SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
						MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
 					}
					cnt++;
				}
			}
		}
				
		///////////////////// DIRT MODIFICATOR
		{
			if ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )
			{
				if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// new index line here?
				if (cnt >= sFirstLine && cnt < sLastLine )
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 57 ], szUDBAdvStatsExplanationsTooltipText[ 57 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
 			}
		}

		///////////////////// FANTHEHAMMER
		if ( Item[gpItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) && gGameOptions.fNewTraitSystem && gSkillTraitValues.fCanFanTheHammer )
		{
			if ( !fDrawGenIndexes )
				fDrawGenIndexes = ++cnt;		// new index line here?

			if ( cnt >= sFirstLine && cnt < sLastLine )
			{
				swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 66 ], szUDBAdvStatsExplanationsTooltipText[ 66 ] );
				SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + ( cnt - sFirstLine )] ), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + ( cnt - sFirstLine )] );
			}
			++cnt;
		}

		///////////////////// MULTIPLE BARRELS
		if ( ( Item[gpItemDescObject->usItem].usItemClass & IC_BOBBY_GUN ) )
		{
			if ( !fDrawGenIndexes )
				fDrawGenIndexes = ++cnt;		// new index line here?

			if ( cnt >= sFirstLine && cnt < sLastLine )
			{
				swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[67], szUDBAdvStatsExplanationsTooltipText[67] );
				SetRegionFastHelpText( &( gUDBFasthelpRegions[iFirstDataRegion + ( cnt - sFirstLine )] ), pStr );
				MSYS_EnableRegion( &gUDBFasthelpRegions[iFirstDataRegion + ( cnt - sFirstLine )] );
			}
			++cnt;
		}

		if ( UsingFoodSystem() && Item[gpItemDescObject->usItem].foodtype > 0 )
		{
			if (!fDrawGenIndexes)
				fDrawGenIndexes = ++cnt;		// new index line here?

			////////////////////// POISONED FOOD
			if ( (*gpItemDescObject)[0]->data.bTemperature != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{//reusing Poison text here
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 58 ], szUDBAdvStatsExplanationsTooltipText[ 58 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
			UINT32 foodtype = Item[gpItemDescObject->usItem].foodtype;
			////////////////////// FOOD POINTS
			if ( Food[foodtype].bFoodPoints != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 59 ], szUDBAdvStatsExplanationsTooltipText[ 59 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
			/////////////////////// DRINK POINTS
			if ( Food[foodtype].bDrinkPoints != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 60 ], szUDBAdvStatsExplanationsTooltipText[ 60 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
			////////////////////// PORTION SIZE
			if ( Item[gpItemDescObject->usItem].usPortionSize != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 61 ], szUDBAdvStatsExplanationsTooltipText[ 61 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}
			
			////////////////////// MORAL MODIFIER
			if ( FoodOpinions[gpItemDescSoldier->ubProfile].sFoodOpinion[foodtype] != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 62 ], szUDBAdvStatsExplanationsTooltipText[ 62 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}

			////////////////////// DECAY RATE
			if ( Food[foodtype].usDecayRate != 0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					swprintf( pStr, L"%s%s", szUDBAdvStatsTooltipText[ 63 ], szUDBAdvStatsExplanationsTooltipText[ 63 ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ]), pStr );
					MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (cnt-sFirstLine) ] );
				}
				cnt++;
			}

		}
		// Moa: moved additional Indexes to the end to avoid doublication. We have allready secured a free slot for this line so no cnt++ here!
		if (fDrawGenIndexes)
		{
			///////////////////// INDEXES (Prop., 0, +/-, =)
			if (fDrawGenIndexes >= sFirstLine && fDrawGenIndexes < sLastLine)
			{
				SetRegionFastHelpText( &(gUDBFasthelpRegions[ iFirstDataRegion + (fDrawGenIndexes-sFirstLine-1) ]), gzUDBGenIndexTooltipText[ 0 ]);
				MSYS_EnableRegion( &gUDBFasthelpRegions[ iFirstDataRegion + (fDrawGenIndexes-sFirstLine-1) ] );
				//Moa: start bugfix (changed gItemDescAdvIndexRegions[] to gItemDescAdvRegions[] and adding correct region)

				for (cnt2 = 1; cnt2 < 4; cnt2++)
				{
					MSYS_DefineRegion( &gUDBFasthelpRegions[ iRegionsCreated ],
						(INT16)(gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][cnt2].sLeft),
						(INT16)(gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][cnt2].sTop),
						(INT16)(gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][cnt2].sRight),
						(INT16)(gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][cnt2].sBottom),
						MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

					MSYS_AddRegion( &gUDBFasthelpRegions[ iRegionsCreated ]);
					SetRegionFastHelpText( &(gUDBFasthelpRegions[ iRegionsCreated ]), gzUDBGenIndexTooltipText[ cnt2 ] );
					SetRegionHelpEndCallback( &(gUDBFasthelpRegions[ iRegionsCreated ]), HelpTextDoneCallback );

					iRegionsCreated++;
				}
			}
			//cnt++;		//allready increased the counter above!
			//Moa: end bugfix
		}

		gubDescBoxTotalAdvLines = (UINT8)cnt;
	}
}

void DeleteEnhancedDescBox( UINT32 guiCurrentItemDescriptionScreen )
{
	INT32 cnt;

	DeleteVideoObjectFromIndex( guiItemInfoWeaponIcon );
	DeleteVideoObjectFromIndex( guiItemInfoAmmoIcon );
	DeleteVideoObjectFromIndex( guiItemInfoArmorIcon );
	DeleteVideoObjectFromIndex( guiItemInfoExplosiveIcon );
	// HEADROCK HAM 4: Delete new icons
	DeleteVideoObjectFromIndex( guiItemInfoSecondaryIcon );
	DeleteVideoObjectFromIndex( guiItemInfoAdvancedIcon );
	DeleteVideoObjectFromIndex( guiItemInfoWH40KIcon );

	if ( UsingEDBSystem() > 0 )
	{
		// HEADROCK HAM 4: UDB fasthelp
		for ( cnt = 0; cnt < NUM_UDB_FASTHELP_REGIONS; cnt++ )
		{
			if( gUDBFasthelpRegions[ cnt ].IDNumber != 0 )
			{
				MSYS_RemoveRegion( &gUDBFasthelpRegions[ cnt ]);
			}
		}
	}
}

void DrawWeaponStats( OBJECTTYPE * gpItemDescObject )
{
	INT32	cnt;
	INT16 sOffsetX = 2;
	INT16 sOffsetY = 1;
	UINT8 ubNumLine;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{

		// anv: if ctrl is pressed in map inventory, show comparison with selected weapon
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL && gpComparedItemDescObject->usItem )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) )
					fComparisonMode = TRUE;
			}
		}

		//////////////////// ACCURACY
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) ) ||
			(  fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 0;
			}
			else
			{
				ubNumLine = 2;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 8, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// DAMAGE
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher ) ||
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher ) )
		{
			ubNumLine = 1;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 5, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// RANGE
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) ) ||
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) ) )
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 2;
			}
			else
			{
				ubNumLine = 0;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 4, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// GUN HANDLING
		if ( ( UsingNewCTHSystem() == TRUE && Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) ) || 
			( fComparisonMode && UsingNewCTHSystem() == TRUE && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{
			ubNumLine = 3;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 33, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// ALLOWED AIM LEVELS
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) ) ||
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) ) )
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 4;
			}
			else
			{
				ubNumLine = 3;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 32, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// OCTH AIMING BONUS
		if ( ( UsingNewCTHSystem() == false && 
			(GetFlatAimBonus( gpItemDescObject ) != 0 || Item[gpItemDescObject->usItem].aimbonus != 0) ) ||
			( fComparisonMode && UsingNewCTHSystem() == false && 
			(GetFlatAimBonus( gpComparedItemDescObject ) != 0 || Item[gpComparedItemDescObject->usItem].aimbonus != 0) ) )
		{
			ubNumLine = 4;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 15, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// SCOPE MAGNIFICATION
		if ( ( UsingNewCTHSystem() == true && Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN ) ||
			( fComparisonMode && UsingNewCTHSystem() == true && Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_GUN ) )
		{
			ubNumLine = 5;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 15, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// OCTH MINIMUM RANGE FOR AIMING BONUS
		if( (UsingNewCTHSystem() == false && 
			( Item[gpItemDescObject->usItem].minrangeforaimbonus > 0 || GetMinRangeForAimBonus( NULL, gpItemDescObject ) > 0 ) ) ||
			( fComparisonMode &&UsingNewCTHSystem() == false && 
			( Item[gpComparedItemDescObject->usItem].minrangeforaimbonus > 0 || GetMinRangeForAimBonus( NULL, gpComparedItemDescObject ) > 0 ) ) )
		{
			ubNumLine = 5;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 27, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// PROJECTION FACTOR / NCTH BEST LASER RANGE
		// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor but we use the same icon
		if ( (UsingNewCTHSystem() && ( GetProjectionFactor( gpItemDescObject ) > 1.0 ||	( GetBestLaserRange( gpItemDescObject ) > 0
			&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) ) ||
			( fComparisonMode && UsingNewCTHSystem() && ( (GetProjectionFactor( gpItemDescObject ) > 1.0 || GetProjectionFactor( gpComparedItemDescObject ) > 1.0) ||
			( GetBestLaserRange( gpComparedItemDescObject ) > 0
			&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) ) )
		{
			ubNumLine = 6;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 14, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// OCTH TO=HIT BONUS
		if ( (UsingNewCTHSystem() == false && 
			(Item[gpItemDescObject->usItem].tohitbonus != 0 || GetFlatToHitBonus( gpItemDescObject ) != 0) ) ||
			( fComparisonMode && UsingNewCTHSystem() == false && 
			(Item[gpComparedItemDescObject->usItem].tohitbonus != 0 || GetFlatToHitBonus( gpComparedItemDescObject ) != 0) ) )
		{
			ubNumLine = 6;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 13, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// OCTH BEST LASER RANGE
		if ( !UsingNewCTHSystem() && ( GetBestLaserRange( gpItemDescObject ) ||
			( fComparisonMode && GetBestLaserRange( gpComparedItemDescObject ) ) ) )
		{
			ubNumLine = 7;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 14, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// FLASH SUPPRESSION
		if (IsFlashSuppressorAlt( gpItemDescObject ) ||
			( fComparisonMode && IsFlashSuppressorAlt( gpComparedItemDescObject ) ) )
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 7;
			}
			else
			{
				ubNumLine = 8;
			}
			// HIDE FLASH ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 28, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// LOUDNESS
		//if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 8;
			}
			else
			{
				ubNumLine = 9;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 17, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// RELIABILITY
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 9;
			}
			else
			{
				ubNumLine = 10;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 9, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// REPAIR EASE
		{
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 10;
			}
			else
			{
				ubNumLine = 11;
			}

			if ( !Item[gpItemDescObject->usItem].repairable || 
				( fComparisonMode && !Item[gpComparedItemDescObject->usItem].repairable ) )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 35, gItemDescGenRegions[ubNumLine][0].sLeft + sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			else if ( !gGameExternalOptions.fAdvRepairSystem )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, gItemDescGenRegions[ubNumLine][0].sLeft + sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			else
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 34, gItemDescGenRegions[ubNumLine][0].sLeft + sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
//			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// DRAW COST
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem].rocketlauncher ) || 
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem].rocketlauncher ) )
		{
			ubNumLine = 13;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 1, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////////// SINGLE SHOT COST - GUN
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher && !fComparisonMode ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpComparedItemDescObject->usItem].rocketlauncher ) )
		{
			ubNumLine = 14;
			// "NO SINGLE-SHOT" ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 19, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );

			if ( ( !Weapon[gpItemDescObject->usItem].NoSemiAuto && !fComparisonMode ) || ( fComparisonMode && !Weapon[gpItemDescObject->usItem].NoSemiAuto ) )
			{
				// SINGLE SHOT AP ICON overwrites shadow
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX+1, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		/////////////////// SINGLE SHOT COST - ROCKET
		if ( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && Item[gpItemDescObject->usItem].rocketlauncher && !fComparisonMode ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) && Item[gpComparedItemDescObject->usItem].rocketlauncher ) )
		{
			ubNumLine = 14;
			// SINGLE ROCKET-LAUNCH AP ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 21, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		/////////////////// SINGLE SHOT COST - GRENADE LAUNCHER
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpItemDescObject->usItem].rocketlauncher
			&& !Weapon[gpItemDescObject->usItem].NoSemiAuto && !fComparisonMode ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpComparedItemDescObject->usItem].rocketlauncher
			&& !Weapon[gpComparedItemDescObject->usItem].NoSemiAuto ) )
		{
			ubNumLine = 14;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 24, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		/////////////////// SINGLE SHOT COST - THROWING KNIFE
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_THROWING_KNIFE && !fComparisonMode ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_THROWING_KNIFE ) )
		{
			ubNumLine = 14;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 22, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		/////////////////// SINGLE SHOT COST - STABBING KNIFE
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_BLADE && !fComparisonMode ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_BLADE ) )
		{
			ubNumLine = 14;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 20, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		/////////////////// SINGLE SHOT COST - BLUNT WEAPON
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_PUNCH && !fComparisonMode ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_PUNCH ) )
		{
			ubNumLine = 14;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 26, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		/////////////////// BURST COST - GUN
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpComparedItemDescObject->usItem].rocketlauncher ) )
		{
			ubNumLine = 15;
			// "NO BURST" ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 11, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );

			if ( ( GetShotsPerBurst(gpItemDescObject)> 0 && !fComparisonMode ) || ( fComparisonMode && GetShotsPerBurst(gpComparedItemDescObject)> 0 ) )
			{
				for ( cnt = 0; cnt < GetShotsPerBurst(gpItemDescObject); cnt++ )
				{
					// BURST FIRE ICON overwrites shadow
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX + cnt * (BULLET_WIDTH/2 + 1) +1, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}
		}
		////////////////// BURST COST - GRENADE LAUNCHER
		else if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpItemDescObject->usItem].rocketlauncher 
			&& GetShotsPerBurst(gpItemDescObject)> 0 ) || 
			( fComparisonMode && Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER && !Item[gpItemDescObject->usItem].rocketlauncher 
			&& GetShotsPerBurst(gpItemDescObject)> 0 ) )
		{
			ubNumLine = 15;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 25, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////// AUTOFIRE COST
		if ( ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpItemDescObject->usItem].rocketlauncher ) || 
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass == IC_GUN && !Item[gpComparedItemDescObject->usItem].rocketlauncher ) )
		{
			ubNumLine = 16;
			// "NO-AUTO" ICON
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 12, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );

			if ( ( GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 && !fComparisonMode ) || ( fComparisonMode && GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) ) )
			{
				for ( cnt = 0; cnt < 10; cnt++ )
				{
					// AUTO FIRE ICON overwrites shadow
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 0, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX + cnt * (BULLET_WIDTH/2 + 1) +1, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}
		}


		////////////////// RELOAD COST
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher ) ||
			(  fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher ) )
		{
			ubNumLine = 17;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 2, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
			
		////////////////// MANUAL RELOAD COST
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher 
			&& Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 ) || 
			(  fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher 
			&& Weapon[gpComparedItemDescObject->usItem].APsToReloadManually > 0 ) )
		{
			ubNumLine = 18;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 3, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		///////////////// RECOIL X/Y
		if( UsingNewCTHSystem() == true )
		{
			ubNumLine = 20;
			if ( ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher
				&& ( GetShotsPerBurst(gpItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 ) ) || 
				( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
				&& ( GetShotsPerBurst(gpComparedItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 ) ) )
			{
				// HEADROCK HAM 5: One value to rule them all! Line 19 left empty intentionally.
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 31, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}

		}
		else	///////////////// BIPOD & BURST PENALTY
		{
			if( ( GetBurstPenalty(gpItemDescObject) > 0 ) || 
				( fComparisonMode && GetBurstPenalty(gpComparedItemDescObject) > 0  ) )
			{
				ubNumLine = 19;
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 30, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			if( ( GetBipodBonus(gpItemDescObject) > 0 && !fComparisonMode ) || 
				( fComparisonMode && GetBipodBonus(gpComparedItemDescObject) > 0  ) )
			{
				ubNumLine = 20;
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 16, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		///////////////// AUTOFIRE SHOTS PER 5 AP ICON
		if ( ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher
			&& GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 ) ||
			 ( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
			&& GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 ) )
		{
			ubNumLine = 21;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 7, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		///////////////// AUTOFIRE PENALTY
		if( ( UsingNewCTHSystem() == false && GetAutoPenalty(gpItemDescObject) > 0 ) ||
			( fComparisonMode && UsingNewCTHSystem() == false && GetAutoPenalty(gpComparedItemDescObject) > 0 ) )
		{
			ubNumLine = 22;
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 29, gItemDescGenRegions[ubNumLine][0].sLeft+sOffsetX, gItemDescGenRegions[ubNumLine][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}
	else if(gubDescBoxPage == 2)
	{
		DrawAdvancedStats( gpItemDescObject );
	}
}

void DrawAmmoStats( OBJECTTYPE * gpItemDescObject )
{
	INT16 sOffsetX = 2;
	INT16 sOffsetY = 1;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{
		//////////////// ARMOR PIERCING
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 0, gItemDescGenRegions[0][0].sLeft+sOffsetX, gItemDescGenRegions[0][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////// TUMBLING
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 1, gItemDescGenRegions[1][0].sLeft+sOffsetX, gItemDescGenRegions[1][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//////////////// PRE-ARMOR EXPLOSION
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 2, gItemDescGenRegions[2][0].sLeft+sOffsetX, gItemDescGenRegions[2][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// Flugente
		if ( gGameExternalOptions.fWeaponOverheating )
		{
			//////////////// TEMPERATURE MODIFICATOR
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 16, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		//////////////// DIRT MODIFICATOR
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAmmoIcon, 17, gItemDescGenRegions[5][0].sLeft+sOffsetX, gItemDescGenRegions[5][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		DrawSecondaryStats( gpItemDescObject );
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedStats( gpItemDescObject );
	}

}

void DrawExplosiveStats( OBJECTTYPE * gpItemDescObject )
{
	INT16 sOffsetX = 2;
	INT16 sOffsetY = 1;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{

		// anv: if ctrl is pressed in map inventory, show comparison with selected explosives
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
				{
					fComparisonMode = TRUE;
				}
			}
		}

		////////////////////// DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 || 
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 0, gItemDescGenRegions[0][0].sLeft+sOffsetX, gItemDescGenRegions[0][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// STUN DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 1, gItemDescGenRegions[1][0].sLeft+sOffsetX, gItemDescGenRegions[1][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// HEADROCK HAM 5
		////////////////////// EXPLODE ON IMPACT
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].fExplodeOnImpact ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].fExplodeOnImpact ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 19, gItemDescGenRegions[2][0].sLeft+sOffsetX, gItemDescGenRegions[2][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// SOUND BLAST
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 5 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 5 ) )

		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 6, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// STUN BLAST
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 1 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 1 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 5, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// OTHER BLASTS
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType != 1 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType != 5 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType != 1 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType != 5 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 4, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// START+END RADIUS: TEAR GAS
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 2 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 2 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 9, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 10, gItemDescGenRegions[4][0].sLeft+sOffsetX, gItemDescGenRegions[4][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// START+END RADIUS: MUSTARD GAS
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 3 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 3 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 13, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 14, gItemDescGenRegions[4][0].sLeft+sOffsetX, gItemDescGenRegions[4][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// START+END RADIUS: LIGHT
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 4 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 4 ) )
		{
			// Note light is reversed (large to small)
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 18, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 17, gItemDescGenRegions[4][0].sLeft+sOffsetX, gItemDescGenRegions[4][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// START+END RADIUS: SMOKE
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 6 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 6 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 11, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 12, gItemDescGenRegions[4][0].sLeft+sOffsetX, gItemDescGenRegions[4][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// START+END RADIUS: NAPALM
		if ( ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpItemDescObject->usItem].ubClassIndex ].ubType == 8 && !fComparisonMode ) ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 
			&& Explosive[Item[ gpComparedItemDescObject->usItem].ubClassIndex ].ubType == 8 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 15, gItemDescGenRegions[3][0].sLeft+sOffsetX, gItemDescGenRegions[3][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 16, gItemDescGenRegions[4][0].sLeft+sOffsetX, gItemDescGenRegions[4][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// DURATION
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 || 
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 7, gItemDescGenRegions[5][0].sLeft+sOffsetX, gItemDescGenRegions[5][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// HEADROCK HAM 5: Fragmentation
		////////////////////// NUMBER OF FRAGMENTS
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 20, gItemDescGenRegions[6][0].sLeft+sOffsetX, gItemDescGenRegions[6][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// FRAGMENT DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 21, gItemDescGenRegions[7][0].sLeft+sOffsetX, gItemDescGenRegions[7][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// FRAGMENT RANGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 22, gItemDescGenRegions[8][0].sLeft+sOffsetX, gItemDescGenRegions[8][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// LOUDNESS
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 2, gItemDescGenRegions[9][0].sLeft+sOffsetX, gItemDescGenRegions[9][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////////// VOLATILITY
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 ||
			( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoExplosiveIcon, 3, gItemDescGenRegions[10][0].sLeft+sOffsetX, gItemDescGenRegions[10][0].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////// REPAIR EASE
		if ( ( !Item[gpItemDescObject->usItem].repairable && !fComparisonMode ) ||
			( fComparisonMode && !Item[ gpComparedItemDescObject->usItem ].repairable ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 35, gItemDescGenRegions[11][0].sLeft + sOffsetX, gItemDescGenRegions[11][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, gItemDescGenRegions[11][0].sLeft + sOffsetX, gItemDescGenRegions[11][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		DrawSecondaryStats( gpItemDescObject );
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedStats( gpItemDescObject );
	}
}

void DrawArmorStats( OBJECTTYPE * gpItemDescObject )
{
	INT16 sOffsetX = 2;
	INT16 sOffsetY = 1;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{
		// anv: if ctrl is pressed in map inventory, show comparison with selected armor
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_ARMOUR )
				{
					if( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == Armour[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubArmourClass )
						fComparisonMode = TRUE;
				}
			}
		}

		/////////////////// PROTECTION VALUE
		{
			// HELMET
			if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 0 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 1, gItemDescGenRegions[0][0].sLeft + sOffsetX, gItemDescGenRegions[0][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			// LEGGINGS
			else if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 2 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 2, gItemDescGenRegions[0][0].sLeft + sOffsetX, gItemDescGenRegions[0][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			// VEST and anything else
			else
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 0, gItemDescGenRegions[0][0].sLeft + sOffsetX, gItemDescGenRegions[0][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		/////////////////// COVERAGE VALUE
		{
			// HELMET
			if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 0 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 4, gItemDescGenRegions[1][0].sLeft + sOffsetX, gItemDescGenRegions[1][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			// LEGGINGS
			else if ( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == 2 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 5, gItemDescGenRegions[1][0].sLeft + sOffsetX, gItemDescGenRegions[1][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			// VEST and anything else
			else
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 3, gItemDescGenRegions[1][0].sLeft + sOffsetX, gItemDescGenRegions[1][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		////////////////// ARMOR DEGRADE
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoArmorIcon, 6, gItemDescGenRegions[2][0].sLeft + sOffsetX, gItemDescGenRegions[2][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////// REPAIR EASE
		if ( ( !Item[gpItemDescObject->usItem].repairable && !fComparisonMode ) ||
			( fComparisonMode && !Item[gpComparedItemDescObject->usItem].repairable ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 35, gItemDescGenRegions[3][0].sLeft + sOffsetX, gItemDescGenRegions[3][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else if ( !gGameExternalOptions.fAdvRepairSystem )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, gItemDescGenRegions[3][0].sLeft + sOffsetX, gItemDescGenRegions[3][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 34, gItemDescGenRegions[3][0].sLeft + sOffsetX, gItemDescGenRegions[3][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		////////////////// SECONDARY ARMOR PROPERTIES
		DrawSecondaryStats( gpItemDescObject );
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedStats( gpItemDescObject );
	}
}

void DrawAdvancedStats( OBJECTTYPE * gpItemDescObject )
{
	if (gubDescBoxPage != 2)
	{
		return;
	}

	INT16 sFirstLine = gubDescBoxLine;
	INT16 sLastLine = gubDescBoxLine + NUM_UDB_ADV_LINES;
	INT16 cnt = 0;	// Counts number of properties encountered
	INT16 sOffsetX = 0;
	INT16 sOffsetY = 0;


	OBJECTTYPE *gpComparedItemDescObject = NULL;
	// anv: if ctrl is pressed in map inventory, show comparison with selected item
	BOOLEAN fComparisonMode = FALSE;
	if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
	{
		if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
			gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
		if( gpComparedItemDescObject != NULL )
		{
			if( Item[ gpItemDescObject->usItem ].usItemClass == Item[ gpComparedItemDescObject->usItem ].usItemClass )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_AMMO )
					fComparisonMode = TRUE;
				else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_ARMOUR )
				{
					if( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == Armour[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubArmourClass )
						fComparisonMode = TRUE;
				}
				else 
					fComparisonMode = TRUE;
			}
			if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_EXPLOSV && Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
				fComparisonMode = TRUE;
			if( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) )
				fComparisonMode = TRUE;
		}
	}

	///////////////////// ACCURACY MODIFIER
	if ( ( GetAccuracyModifier( gpItemDescObject ) ) ||
		( fComparisonMode && GetAccuracyModifier( gpComparedItemDescObject ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 0, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}	
			cnt++;
		}
	}

	///////////////////// FLAT BASE MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATBASE ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATBASE ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 1, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// PERCENT BASE MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTBASE ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTBASE ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTBASE ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 2, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// FLAT AIM MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATAIM ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATAIM ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 3, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// PERCENT AIM MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTAIM ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTAIM ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTAIM ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 4, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// AIM LEVELS MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS ) != 0
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_AIMLEVELS ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_AIMLEVELS ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS ) != 0
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_AIMLEVELS ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_AIMLEVELS ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 5, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// AIM BONUS MODIFIER
	if(UsingNewCTHSystem() == false)
	{
		//if ( GetFlatAimBonus( gpItemDescObject ) != 0 )
		if ( ( GetAimBonus( gpItemDescSoldier, gpItemDescObject, 100, 1 ) != 0 ) ||
			( fComparisonMode && GetAimBonus( gpItemDescSoldier, gpComparedItemDescObject, 100, 1 ) != 0 ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 1, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// TO-HIT MODIFIER
	if(UsingNewCTHSystem() == false)
	{
		if ( ( GetToHitBonus( gpItemDescObject, 100, 1, FALSE ) != 0 
			|| GetToHitBonus( gpItemDescObject, 100, 1, TRUE ) != 0 ) ||
			( fComparisonMode && ( GetToHitBonus( gpComparedItemDescObject, 100, 1, FALSE ) != 0 
			|| GetToHitBonus( gpComparedItemDescObject, 100, 1, TRUE ) != 0 ) ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 6, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// CTH CAP MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTCAP ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTCAP ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTCAP ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTCAP ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTCAP ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTCAP ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 6, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// GUN HANDLING MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTHANDLING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTHANDLING ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTHANDLING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTHANDLING ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 7, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// DROP COMPENSATION MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_DROPCOMPENSATION ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_DROPCOMPENSATION ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_DROPCOMPENSATION ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 8, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}
	///////////////////// TARGET TRACKING MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_TRACKING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_TRACKING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_TRACKING ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_TRACKING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_TRACKING ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_TRACKING ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 9, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// DAMAGE MODIFIER
	if ( (GetDamageBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetDamageBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 10, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// MELEE DAMAGE MODIFIER
	if ( (GetMeleeDamageBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetMeleeDamageBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 11, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// RANGE MODIFIER
	if ( (GetRangeBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && (GetRangeBonus( gpComparedItemDescObject ) != 0 ) ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 12, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// SCOPE MAGNIFICATION
	if ( (GetHighestScopeMagnificationFactor( gpItemDescObject ) > 1.0 ) ||
		( fComparisonMode && GetHighestScopeMagnificationFactor( gpComparedItemDescObject ) > 1.0 ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 13, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// PROJECTION FACTOR / BEST LASER RANGE
	// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor but we use the same icon
	if ( ( UsingNewCTHSystem() && ( GetProjectionFactor( gpItemDescObject ) > 1.0 || ( GetBestLaserRange( gpItemDescObject ) > 0
		&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) ||
		( fComparisonMode && ( GetProjectionFactor( gpComparedItemDescObject ) > 1.0 || ( GetBestLaserRange( gpComparedItemDescObject ) > 0
		&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) ) ) ||
		( !UsingNewCTHSystem() && ( GetBestLaserRange( gpItemDescObject ) > 0 || ( fComparisonMode && GetBestLaserRange( gpComparedItemDescObject ) > 0 ) ) ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 14, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// PERCENT RECOIL MODIFIER
	if ( ( GetPercentRecoilModifier( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentRecoilModifier( gpComparedItemDescObject ) != 0 ) )
	{
		if ( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 62, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	FLOAT bRecoilModifierX;
	FLOAT bRecoilModifierY;
	FLOAT bComparedRecoilModifierX;
	FLOAT bComparedRecoilModifierY;
	GetFlatRecoilModifier( gpItemDescObject, &bRecoilModifierX, &bRecoilModifierY );
	if( fComparisonMode )
		GetFlatRecoilModifier( gpComparedItemDescObject, &bComparedRecoilModifierX, &bComparedRecoilModifierY );
	///////////////////// LATERAL RECOIL MODIFIER
	if ( (bRecoilModifierX != 0) ||
		( fComparisonMode && bComparedRecoilModifierX != 0) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 15, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// VERTICAL RECOIL MODIFIER
	if ( (bRecoilModifierY != 0 ) ||
		( fComparisonMode && bComparedRecoilModifierY != 0 ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 16, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// MAX COUNTER FORCE
	// HEADROCK HAM 5: Moved here because it makes more sense.
	if ( (CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_STAND ) != 0 
		|| CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH ) != 0 
		|| CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE ) != 0 ) ||
		( fComparisonMode && (CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND ) != 0 
		|| CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH ) != 0 
		|| CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_GUN )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 19, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// MAX COUNTER FORCE MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEMAX ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true ) //&& Item[gpItemDescObject->usItem].usItemClass == IC_GUN )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 17, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// COUNTER FORCE ACCURACY MODIFIER
	if ( ( GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 ) ||
		( fComparisonMode && ( GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 
		|| GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEACCURACY ) != 0 ) ) )
	{
		if( UsingNewCTHSystem() == true )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 18, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
 			}
			cnt++;
		}
	}

	///////////////////// AP MODIFIER
	if ( (GetAPBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetAPBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 20, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// DRAW AP MODIFIER
	if ( (GetPercentReadyTimeAPReduction( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentReadyTimeAPReduction( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 21, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// SINGLE-ATTACK AP MODIFIER
	if ( (GetPercentAPReduction( NULL, gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentAPReduction( NULL, gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 22, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// BURST AP MODIFIER
	if ( (GetPercentBurstFireAPReduction( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentBurstFireAPReduction( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 23, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// AUTOFIRE AP MODIFIER
	if ( (GetPercentAutofireAPReduction( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentAutofireAPReduction( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 24, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// RELOAD AP MODIFIER
	if ( (GetPercentReloadTimeAPReduction( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetPercentReloadTimeAPReduction( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 25, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// MAGAZINE SIZE MODIFIER
	if ( (GetMagSizeBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetMagSizeBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 26, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// BURST SIZE MODIFIER
	if ( (GetBurstSizeBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetBurstSizeBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 27, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// HIDE MUZZLE FLASH
	if ( (IsFlashSuppressorAlt( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && IsFlashSuppressorAlt( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 28, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// LOUDNESS MODIFIER
	if ( (GetPercentNoiseVolume( gpItemDescObject )-100 != 0 ) ||
		( fComparisonMode && GetPercentNoiseVolume( gpComparedItemDescObject )-100 != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 29, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// ITEM SIZE MODIFIER
	if ( (CalculateItemSize( gpItemDescObject ) - Item[ gpItemDescObject->usItem ].ItemSize != 0 ) ||
		( fComparisonMode && ( CalculateItemSize( gpComparedItemDescObject ) - Item[ gpComparedItemDescObject->usItem ].ItemSize ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 30, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// RELIABILITY MODIFIER
	if ( (!(Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH|IC_ARMOUR|IC_EXPLOSV)) &&
		GetReliability( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && !(Item[gpComparedItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH|IC_ARMOUR|IC_EXPLOSV)) &&
		GetReliability( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 31, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// WOODLAND CAMO
	if ( (GetCamoBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetCamoBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 32, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// URBAN CAMO
	if ( (GetUrbanCamoBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetUrbanCamoBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 33, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// DESERT CAMO
	if ( (GetDesertCamoBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetDesertCamoBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 34, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// SNOW CAMO
	if ( (GetSnowCamoBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetSnowCamoBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 35, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// STEALTH MODIFIER
	if ( (GetStealthBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetStealthBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 36, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// HEARING RANGE MODIFIER
	if ( (GetItemHearingRangeBonus( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetItemHearingRangeBonus( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 37, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// VISION RANGE MODIFIER
	if ( (GetItemVisionRangeBonus( gpItemDescObject, 0 ) != 0 ) ||
		( fComparisonMode && GetItemVisionRangeBonus( gpComparedItemDescObject, 0 ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 38, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// NIGHT VISION RANGE MODIFIER
	if ( (GetItemVisionRangeBonus( gpItemDescObject, 2 ) != 0 ) ||
		( fComparisonMode && GetItemVisionRangeBonus( gpComparedItemDescObject, 2 ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 39, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// DAY VISION RANGE MODIFIER
	if ( (GetItemVisionRangeBonus( gpItemDescObject, 1 ) != 0 ) ||
		( fComparisonMode && GetItemVisionRangeBonus( gpComparedItemDescObject, 1 ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 40, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// BRIGHT LIGHT VISION RANGE MODIFIER
	if ( (GetItemVisionRangeBonus( gpItemDescObject, 3 ) != 0 ) ||
		( fComparisonMode && GetItemVisionRangeBonus( gpComparedItemDescObject, 3 ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 41, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// CAVE VISION RANGE MODIFIER
	if ( (GetItemVisionRangeBonus( gpItemDescObject, 4 ) != 0 ) ||
		( fComparisonMode && GetItemVisionRangeBonus( gpComparedItemDescObject, 4 ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 42, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	///////////////////// PERCENT TUNNEL VISION
	if ( (GetItemPercentTunnelVision( gpItemDescObject ) != 0 ) ||
		( fComparisonMode && GetItemPercentTunnelVision( gpComparedItemDescObject ) != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 43, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		cnt++;
	}

	// Flugente
	// new line is necessary (Moa: only if needed)
	INT16 fDrawGenIndexes = FALSE;
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		if( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) || Item[gpItemDescObject->usItem].barrel == TRUE || ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ||
			( fComparisonMode && ( Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) || Item[gpComparedItemDescObject->usItem].barrel == TRUE || ( Item[gpComparedItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt; // new index line here?
		}
	}

	// Flugente
	if ( gGameExternalOptions.fWeaponOverheating )
	{		
		if( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{
			///////////////////// SINGLE SHOT TEMPERATURE
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 51, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;

			///////////////////// COOLDOWN FACTOR
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 47, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;

			///////////////////// JAM THRESHOLD
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 45, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;

			///////////////////// DAMAGE THRESHOLD
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 46, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
		if( ( Item[gpItemDescObject->usItem].barrel == TRUE ) ||	// for barrel items
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].barrel == TRUE ) )
		{
			///////////////////// COOLDOWN FACTOR
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 47, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}

		// for overheat modifiers on attachments and wherenot
		if ( ( ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ||
			( fComparisonMode && ( ( Item[gpComparedItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ) )
		{
			///////////////////// TEMPERATURE MODIFICATOR
			if ( ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) ||
				( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 44, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}

			///////////////////// COOLDOWN FACTOR MODIFICATOR
			if ( ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 ) ||
				( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatCooldownModificator != 0.0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 48, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}

			///////////////////// JAM THRESHOLD MODIFICATOR
			if ( ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) ||
				( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 49, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}

			///////////////////// DAMAGE THRESHOLD MODIFICATOR
			if ( ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ||
				( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 50, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}
		}
	}
	
	///////////////////// DIRT MODIFICATOR
	{
		if ( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 55, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			cnt++;
		}
	}

	///////////////////// FANTHEHAMMER
	if ( gGameOptions.fNewTraitSystem && gSkillTraitValues.fCanFanTheHammer &&
		( ( Item[gpItemDescObject->usItem].usItemClass & IC_BOBBY_GUN ) || ( fComparisonMode && ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_BOBBY_GUN ) ) ) )
	{
		if ( cnt >= sFirstLine && cnt < sLastLine )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 65, gItemDescAdvRegions[cnt - sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt - sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		++cnt;
	}

	///////////////////// MULTIPLE BARRELS
	if ( ( ( Item[gpItemDescObject->usItem].usItemClass & IC_BOBBY_GUN ) || ( fComparisonMode && ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_BOBBY_GUN ) ) ) )
	{
		if ( cnt >= sFirstLine && cnt < sLastLine )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 66, gItemDescAdvRegions[cnt - sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt - sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		++cnt;
	}

	if ( UsingFoodSystem() )
	{
		if ( ( Item[gpItemDescObject->usItem].foodtype > 0 ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].foodtype > 0 ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt; // new index line here?

			if ( ( (*gpItemDescObject)[0]->data.bTemperature != 0 ) ||
				( fComparisonMode && (*gpComparedItemDescObject)[0]->data.bTemperature != 0 ) )
			{
				//////////////////// POISONED FOOD
				if (cnt >= sFirstLine && cnt < sLastLine)
				{// using damage icon here (skull)
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 10, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY,VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}
			if ( ( Food[Item[gpItemDescObject->usItem].foodtype].bFoodPoints > 0 ) ||
				( fComparisonMode && Food[Item[gpComparedItemDescObject->usItem].foodtype].bFoodPoints > 0 ) )
			{
				//////////////////// FOOD POINTS
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 57, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}
			if ( ( Food[Item[gpItemDescObject->usItem].foodtype].bDrinkPoints > 0 ) ||
				( fComparisonMode && Food[Item[gpComparedItemDescObject->usItem].foodtype].bDrinkPoints > 0 ) )
			{
				//////////////////// DRINK POINTS
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 58, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}
			if ( (Item[gpItemDescObject->usItem].usPortionSize > 0) ||
				 (fComparisonMode && Item[gpComparedItemDescObject->usItem].usPortionSize > 0) )
			{
				//////////////////// PORTION SIZE
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 59, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}

			if ( ( FoodOpinions[gpItemDescSoldier->ubProfile].sFoodOpinion[Item[gpItemDescObject->usItem].foodtype] != 0 ) ||
				( fComparisonMode && FoodOpinions[gpItemDescSoldier->ubProfile].sFoodOpinion[Item[gpComparedItemDescObject->usItem].foodtype] != 0 ) )
			{
				//////////////////// MORAL MODIFIER
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 60, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}

			if ( ( Food[Item[gpItemDescObject->usItem].foodtype].usDecayRate > 0 ) ||
				( fComparisonMode && Food[Item[gpComparedItemDescObject->usItem].foodtype].usDecayRate > 0 ) )
			{
				//////////////////// DECAY RATE
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 61, gItemDescAdvRegions[cnt-sFirstLine][0].sLeft + sOffsetX, gItemDescAdvRegions[cnt-sFirstLine][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				cnt++;
			}
		}
	}
}

void DrawMiscStats( OBJECTTYPE * gpItemDescObject )
{
	INT16 sOffsetX = 2;
	INT16 sOffsetY = 1;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{
		OBJECTTYPE *gpComparedItemDescObject = NULL;
		// anv: if ctrl is pressed in map inventory, show comparison with selected misc
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass == Item[ gpItemDescObject->usItem ].usItemClass )
					fComparisonMode = TRUE;
			}
		}

		////////////////// REPAIR EASE
		// not for weapons. They have this one their primary page
		if ( !(Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH) )
		{
			if ( !Item[gpItemDescObject->usItem].repairable && !fComparisonMode || 
				( fComparisonMode && !Item[gpComparedItemDescObject->usItem].repairable ) )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 35, gItemDescGenRegions[0][0].sLeft + sOffsetX, gItemDescGenRegions[0][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			else
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoWeaponIcon, 10, gItemDescGenRegions[0][0].sLeft + sOffsetX, gItemDescGenRegions[0][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}
		//////////////////// LBE Stuff
		if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
		{
			//////////////////// LBE AVAILABLE VOLUME (for MOLLE carriers)
			if ( LoadBearingEquipment[ Item[ gpItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume > 0 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 63, gItemDescGenRegions[1][0].sLeft + sOffsetX, gItemDescGenRegions[1][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
			//////////////////// POCKET VOLUME (for MOLLE pockets)
			if ( Item[ gpItemDescObject->usItem ].nasAttachmentClass != 0 && GetFirstPocketOnItem(gpItemDescObject->usItem) != 0 )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 64, gItemDescGenRegions[2][0].sLeft + sOffsetX, gItemDescGenRegions[2][0].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}

		DrawSecondaryStats( gpItemDescObject );
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedStats( gpItemDescObject );
	}

}

///////////////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 4: UDB: Draws item flags as large icons on the secondary column
void DrawSecondaryStats( OBJECTTYPE * gpItemDescObject )
{
	if (UsingEDBSystem() == 0)
	{
		return;
	}

	if (gubDescBoxPage != 1)
	{
		// Secondary stats are drawn only on page 1.
		return;
	}

	INT32 cnt = 0;
	INT32 sOffsetX = 0;
	INT32 sOffsetY = 0;

	OBJECTTYPE *gpComparedItemDescObject = NULL;
	// anv: if ctrl is pressed in map inventory, show comparison with selected item
	BOOLEAN fComparisonMode = FALSE;
	if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
	{
		if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
			gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
		if( gpComparedItemDescObject != NULL )
		{
			if( Item[ gpItemDescObject->usItem ].usItemClass == Item[ gpComparedItemDescObject->usItem ].usItemClass )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_AMMO )
					fComparisonMode = TRUE;
				else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_ARMOUR )
				{
					if( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == Armour[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubArmourClass )
						fComparisonMode = TRUE;
				}
				else 
					fComparisonMode = TRUE;
			}
			if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_EXPLOSV && Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
					fComparisonMode = TRUE;
			if( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) )
				fComparisonMode = TRUE;

		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Start with class-specific secondaries

	if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_ARMOUR))
	{
		////////////////// FLAK JACKET
		if ( ( Item[ gpItemDescObject->usItem ].flakjacket && !fComparisonMode ) || 
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].flakjacket ))
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 5, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		////////////////// FIRE RESISTANCE
		if ( ( Item[gpItemDescObject->usItem].sFireResistance && !fComparisonMode ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].sFireResistance ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 43, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			++cnt;
		}
	}

	if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_AMMO))
	{
		/////////////////// TRACER AMMO
		if ( ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect && !fComparisonMode ) ||
			( fComparisonMode && AmmoTypes[Magazine[Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].tracerEffect ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 0, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		/////////////////// ANTI-TANK AMMO
		if ( (AmmoTypes[Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubAmmoType].dDamageModifierTank >= 0.1f && !fComparisonMode) ||
			 (fComparisonMode && AmmoTypes[Magazine[Item[gpComparedItemDescObject->usItem].ubClassIndex].ubAmmoType].dDamageModifierTank >= 0.1f ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 1, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		/////////////////// IGNORE ARMOR AMMO
		if ( ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour && !fComparisonMode ) ||
			( fComparisonMode && AmmoTypes[Magazine[Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].ignoreArmour ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 2, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
		
		/////////////////// ACIDIC AMMO
		if ( ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic && !fComparisonMode ) ||
			( fComparisonMode && AmmoTypes[Magazine[Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].acidic ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 3, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		/////////////////// LOCKBUSTING AMMO
		if ( ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower && !fComparisonMode ) ||
			( fComparisonMode && AmmoTypes[Magazine[Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].lockBustingPower ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 4, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		////////////////// ANTI-MATERIEL AMMO
		if ( ( AmmoTypes[Magazine[Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].ammoflag & AMMO_ANTIMATERIEL && !fComparisonMode ) ||
			( fComparisonMode && AmmoTypes[Magazine[Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].ammoflag & AMMO_ANTIMATERIEL ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 36, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
	}

	if (Item[ gpItemDescObject->usItem ].usItemClass & (IC_EXPLOSV))
	{
		////////////////// LOCK BOMB
		if ( ( Item[ gpItemDescObject->usItem ].lockbomb && !fComparisonMode ) ||
			( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].lockbomb ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 25, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
	}

	/////////////////////////////////////////////////
	// Draw stats that can be had by any item.

	//////////////////// WATERPROOF
	if ( ( !Item[ gpItemDescObject->usItem ].waterdamages && !fComparisonMode ) ||
		( fComparisonMode && !Item[ gpComparedItemDescObject->usItem ].waterdamages ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 6, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// ELECTRONIC
	if ( ( Item[ gpItemDescObject->usItem ].electronic && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].electronic ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 7, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// GAS MASK
	if ( ( Item[ gpItemDescObject->usItem ].gasmask && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].gasmask ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 8, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// NEEDS BATTERIES
	if ( ( Item[ gpItemDescObject->usItem ].needsbatteries && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].needsbatteries ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 9, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// LOCKSMITH'S KIT
	if ( ( Item[ gpItemDescObject->usItem ].locksmithkit && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].locksmithkit ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 10, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// WIRE CUTTERS
	if ( ( Item[ gpItemDescObject->usItem ].wirecutters && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].wirecutters ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 11, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// CROWBAR
	if ( ( Item[ gpItemDescObject->usItem ].crowbar && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].crowbar ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 12, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// CROWBAR
	if ( ( Item[ gpItemDescObject->usItem ].metaldetector && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].metaldetector ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 13, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// REMOTE TRIGGER
	if ( ( Item[ gpItemDescObject->usItem ].remotetrigger && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].remotetrigger ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 14, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// REMOTE DETONATOR
	if ( ( Item[ gpItemDescObject->usItem ].remotedetonator && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].remotedetonator ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 15, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// TIMER DETONATOR
	if ( ( Item[ gpItemDescObject->usItem ].detonator && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].detonator ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 16, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// GAS CAN
	if ( ( Item[ gpItemDescObject->usItem ].gascan && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].gascan ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 17, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// TOOLKIT
	if ( ( Item[ gpItemDescObject->usItem ].toolkit && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].toolkit ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 18, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// THERMAL OPTICS
	if ( ( Item[ gpItemDescObject->usItem ].thermaloptics && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].thermaloptics ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 19, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// X-RAY DEVICE
	if ( ( Item[ gpItemDescObject->usItem ].xray && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].xray ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 20, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// CANTEEN
	if ( ( Item[ gpItemDescObject->usItem ].canteen && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].canteen ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 21, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// ALCOHOL
	if ( (Item[gpItemDescObject->usItem].alcohol > 0.0f) ||
		 (fComparisonMode && Item[gpComparedItemDescObject->usItem].alcohol > 0.0f) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 22, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// FIRST-AID KIT
	if ( ( Item[ gpItemDescObject->usItem ].firstaidkit && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].firstaidkit ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 23, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// MEDICAL KIT
	if ( ( Item[ gpItemDescObject->usItem ].medicalkit && !fComparisonMode ) ||
		( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].medicalkit ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 24, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// FOOD
	UINT32 foodtype = Item[gpItemDescObject->usItem].foodtype;
	UINT32 comparedfoodtype = 0;
	if ( fComparisonMode )
		comparedfoodtype = Item[gpComparedItemDescObject->usItem].foodtype;
	if ( ( foodtype > 0 && !fComparisonMode ) || 
		( fComparisonMode || comparedfoodtype > 0 ) )
	{
		if ( ( Food[foodtype].bDrinkPoints > 0 && !fComparisonMode ) || 
			( fComparisonMode && Food[comparedfoodtype].bDrinkPoints ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 26, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}

		if ( ( Food[foodtype].bFoodPoints > 0 && !fComparisonMode ) ||
			( fComparisonMode && Food[comparedfoodtype].bFoodPoints ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 27, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
	}

	//////////////////// EXTERNAL FEEDING
	if ( gGameExternalOptions.ubExternalFeeding )
	{
		if ( ( HasItemFlag(gpItemDescObject->usItem, AMMO_BELT) && !fComparisonMode ) ||
			( fComparisonMode && HasItemFlag(gpComparedItemDescObject->usItem, AMMO_BELT) ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 28, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
		else if ( ( HasItemFlag(gpItemDescObject->usItem, AMMO_BELT_VEST) && !fComparisonMode ) ||
			( fComparisonMode && HasItemFlag(gpComparedItemDescObject->usItem, AMMO_BELT_VEST) ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 29, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			cnt++;
		}
	}

	////////////////// DEFUSAL KIT
	//JMich_SkillsModifiers: Still needs a picture, currently using the wirecutters.
	if ( ( Item[gpItemDescObject->usItem].DisarmModifier > 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].DisarmModifier > 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 11, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// COVERT ITEM
	if ( ( HasItemFlag(gpItemDescObject->usItem, COVERT) && !fComparisonMode ) ||
		( fComparisonMode && HasItemFlag(gpComparedItemDescObject->usItem, COVERT) ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 30, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// NOT DAMAGEABLE
	if ( ( Item[gpItemDescObject->usItem].damageable == 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].damageable == 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 31, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// METAL
	if ( ( Item[gpItemDescObject->usItem].metal > 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].metal > 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 32, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// SINKS
	if ( ( Item[gpItemDescObject->usItem].sinks > 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].sinks > 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 33, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// TWO HANDED
	if ( ( Item[gpItemDescObject->usItem].twohanded > 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].twohanded > 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 34, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// BLOCKS IRON SIGHTS
	if ( ( Item[gpItemDescObject->usItem].blockironsight > 0 && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].blockironsight > 0 ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 35, gItemDescGenSecondaryRegions[cnt].sLeft+sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop+sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// DISEASE
	if ( gGameExternalOptions.fDisease )
	{
		if ( (HasItemFlag( gpItemDescObject->usItem, DISEASEPROTECTION_1 ) && !fComparisonMode) ||
			 (fComparisonMode && HasItemFlag( gpComparedItemDescObject->usItem, DISEASEPROTECTION_1 )) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 37, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			++cnt;
		}
		else if ( (HasItemFlag( gpItemDescObject->usItem, DISEASEPROTECTION_2 ) && !fComparisonMode) ||
				  (fComparisonMode && HasItemFlag( gpComparedItemDescObject->usItem, DISEASEPROTECTION_2 )) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 37, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
			++cnt;
		}
	}

	//////////////////// SHIELD
	if ( (Item[gpItemDescObject->usItem].usRiotShieldStrength > 0 && !fComparisonMode) ||
		 (fComparisonMode && Item[gpComparedItemDescObject->usItem].usRiotShieldStrength > 0) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 38, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		++cnt;
	}

	//////////////////// CAMERA
	if ( ( HasItemFlag( gpItemDescObject->usItem, CAMERA ) && !fComparisonMode ) ||
		( fComparisonMode && HasItemFlag( gpComparedItemDescObject->usItem, CAMERA ) ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 39, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}
		
	//////////////////// BURIAL MODIFIER
	if ( ( Item[gpItemDescObject->usItem].usBurialModifier && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].usBurialModifier ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 40, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		cnt++;
	}

	//////////////////// EMPTY BLOOD BAG
	if ( ( HasItemFlag( gpItemDescObject->usItem, EMPTY_BLOOD_BAG ) && !fComparisonMode ) ||
		( fComparisonMode && HasItemFlag( gpComparedItemDescObject->usItem, EMPTY_BLOOD_BAG ) ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 41, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		++cnt;
	}

	//////////////////// BLOOD BAG
	if ( ( HasItemFlag( gpItemDescObject->usItem, BLOOD_BAG ) && !fComparisonMode ) ||
		( fComparisonMode && HasItemFlag( gpComparedItemDescObject->usItem, BLOOD_BAG ) ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 42, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		++cnt;
	}

	//////////////////// ADMINISTRATION MODIFIER
	if ( ( Item[gpItemDescObject->usItem].usAdministrationModifier && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].usAdministrationModifier ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 44, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		++cnt;
	}

	//////////////////// HACKING MODIFIER
	if ( ( Item[gpItemDescObject->usItem].usHackingModifier && !fComparisonMode ) ||
		( fComparisonMode && Item[gpComparedItemDescObject->usItem].usHackingModifier ) )
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoSecondaryIcon, 45, gItemDescGenSecondaryRegions[cnt].sLeft + sOffsetX, gItemDescGenSecondaryRegions[cnt].sTop + sOffsetY, VO_BLT_SRCTRANSPARENCY, NULL );
		++cnt;
	}
}

void DrawPropertyValueInColour( INT16 iValue, UINT8 ubNumLine, UINT8 ubNumRegion, BOOLEAN fComparisonMode, BOOLEAN fModifier, BOOLEAN fHigherBetter, UINT16 uiOverwriteColour = 0, BOOLEAN fPercentSign = FALSE )
{
	static CHAR16	pStr[ 100 ];
	INT16			usX, usY;

	// Set Y coordinates
	INT16 sTop = gItemDescGenRegions[ubNumLine][1].sTop;
	INT16 sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

	// Set X coordinates
	INT16 sLeft = gItemDescGenRegions[ubNumLine][ubNumRegion].sLeft;
	INT16 sWidth = gItemDescGenRegions[ubNumLine][ubNumRegion].sRight - sLeft;

	SetFontForeground( 5 );
	if( !fComparisonMode && !fModifier )
	{
		if( uiOverwriteColour )
			SetFontForeground( uiOverwriteColour );
		if ( iValue == 0 )
			swprintf( pStr, L"--" );
		else
			swprintf( pStr, L"%d", iValue );
	}
	else
	{
		if ( iValue > 0 )
		{
			if( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			else
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"+%d", iValue );
		}
		else if ( iValue < 0 )
		{
			if( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			else
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"%d", iValue );
		}
		else if ( fModifier && fComparisonMode )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
		else if ( fModifier )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"--" );
		}
		else if ( fComparisonMode )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
	}
	
	if( fPercentSign && wcscmp( pStr, L"--" ) != 0 && wcscmp( pStr, L"=" ) != 0 )
	{
		wcscat( pStr, L"%" );	
	}

	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);

	if( fPercentSign && wcscmp( pStr, L"--" ) != 0 && wcscmp( pStr, L"=" ) != 0 )
	{
	#ifdef CHINESE
		wcscat( pStr, ChineseSpecString1 );
	#else
		wcscat( pStr, L"%" );
	#endif
	}

	mprintf( usX, usY, pStr );

	// Reset font color
	SetFontForeground( 6 );
}

void DrawPropertyValueInColour_X( INT16 iValue, UINT8 numBullets, UINT8 ubNumLine, UINT8 ubNumRegion, BOOLEAN fComparisonMode, BOOLEAN fModifier, BOOLEAN fHigherBetter, UINT16 uiOverwriteColour = 0, BOOLEAN fPercentSign = FALSE )
{
	static CHAR16	pStr[100];
	INT16			usX, usY;

	// Set Y coordinates
	INT16 sTop = gItemDescGenRegions[ubNumLine][1].sTop;
	INT16 sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

	// Set X coordinates
	INT16 sLeft = gItemDescGenRegions[ubNumLine][ubNumRegion].sLeft;
	INT16 sWidth = gItemDescGenRegions[ubNumLine][ubNumRegion].sRight - sLeft;

	SetFontForeground( 5 );
	if ( !fComparisonMode && !fModifier )
	{
		if ( uiOverwriteColour )
			SetFontForeground( uiOverwriteColour );
		if ( iValue == 0 )
			swprintf( pStr, L"--" );
		else
			swprintf( pStr, L"%d", iValue );
	}
	else
	{
		if ( iValue > 0 )
		{
			if ( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			else
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			if ( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"+%d", iValue );
		}
		else if ( iValue < 0 )
		{
			if ( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			else
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			if ( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"%d", iValue );
		}
		else if ( fModifier && fComparisonMode )
		{
			SetFontForeground( 5 );
			if ( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
		else if ( fModifier )
		{
			SetFontForeground( 5 );
			if ( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"--" );
		}
		else if ( fComparisonMode )
		{
			SetFontForeground( 5 );
			if ( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
	}

	if ( fPercentSign && wcscmp( pStr, L"--" ) != 0 && wcscmp( pStr, L"=" ) != 0 )
	{
		wcscat( pStr, L"%" );
	}

	if ( numBullets > 1)
	{
		swprintf( pStr, L"%sX%d", pStr, numBullets );
	}

	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );

	if ( fPercentSign && wcscmp( pStr, L"--" ) != 0 && wcscmp( pStr, L"=" ) != 0 )
	{
#ifdef CHINESE
		wcscat( pStr, ChineseSpecString1 );
#else
		wcscat( pStr, L"%" );
#endif
	}

	mprintf( usX, usY, pStr );

	// Reset font color
	SetFontForeground( 6 );
}

void DrawPropertyTextInColour( CHAR16	pText[ 100 ], UINT8 ubNumLine, UINT8 ubNumRegion, UINT16 uiOverwriteColour = 0 )
{
	static CHAR16	pStr[ 100 ];
	INT16			usX, usY;

	// Set Y coordinates
	INT16 sTop = gItemDescGenRegions[ubNumLine][1].sTop;
	INT16 sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

	// Set X coordinates
	INT16 sLeft = gItemDescGenRegions[ubNumLine][ubNumRegion].sLeft;
	INT16 sWidth = gItemDescGenRegions[ubNumLine][ubNumRegion].sRight - sLeft;

	SetFontForeground( 5 );

	if( uiOverwriteColour )
		SetFontForeground( uiOverwriteColour );

	swprintf( pStr, pText );
			
	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
	mprintf( usX, usY, pStr );

	// Reset font color
	SetFontForeground( 6 );
}

void DrawPropertyValueInColourFloat( FLOAT fValue, UINT8 ubNumLine, UINT8 ubNumRegion, BOOLEAN fComparisonMode, BOOLEAN fModifier, BOOLEAN fHigherBetter, UINT16 uiOverwriteColour = 0, FLOAT fTreshold = 1.0f, UINT8 ubDecimals = 1 )
{
	static CHAR16	pStr[ 100 ];
	INT16			usX, usY;

	// Set Y coordinates
	INT16 sTop = gItemDescGenRegions[ubNumLine][1].sTop;
	INT16 sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

	// Set X coordinates
	INT16 sLeft = gItemDescGenRegions[ubNumLine][ubNumRegion].sLeft;
	INT16 sWidth = gItemDescGenRegions[ubNumLine][ubNumRegion].sRight - sLeft;

	SetFontForeground( 5 );
	if( !fComparisonMode && !fModifier )
	{
		if ( fValue > fTreshold )
		{
			if ( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			else
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
		}
		else if ( fValue < fTreshold )
		{
			if ( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			else
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
		}
		if( uiOverwriteColour )
			SetFontForeground( uiOverwriteColour );
		swprintf( pStr, L"%3.*f", ubDecimals, fValue );
	}
	else
	{
		if ( fValue > 0.0f )
		{
			if( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			else
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"+%3.*f", ubDecimals, fValue );
		}
		else if ( fValue < 0.0f )
		{
			if( fHigherBetter )
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
			else
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"%3.*f", ubDecimals, fValue );
		}
		else if ( fModifier && fComparisonMode )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
		else if ( fModifier )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"--" );
		}
		else if ( fComparisonMode )
		{
			SetFontForeground( 5 );
			if( uiOverwriteColour )
				SetFontForeground( uiOverwriteColour );
			swprintf( pStr, L"=" );
		}
	}
			
	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
	mprintf( usX, usY, pStr );

	// Reset font color
	SetFontForeground( 6 );
}

void DrawWeaponValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16	pStr[ 100 ];
	INT16			usX, usY;
	INT16			ubAttackAPs, ubBasicAttackAPs;
	UINT8			ubNumLine;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	// ShotsPer4Turns -> ubAttackAPs, used later for all shot AP values
	// silversurfer: Knuckle Dusters count as bare hand attacks and use the AP_PUNCH constant.
	if ( Item[gpItemDescObject->usItem].brassknuckles )
	{
		ubAttackAPs = APBPConstants[AP_PUNCH];
		ubBasicAttackAPs = APBPConstants[AP_PUNCH];
	}
	else
	{
		ubAttackAPs = BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject, gpItemDescSoldier );
		ubBasicAttackAPs = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject );
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// The following attributes are only drawn when the UDB is set to show the General Page.

	if (gubDescBoxPage == 1)
	{
		////////////////////////////////////////////////// HEADERS
		
		SetFontForeground( FONT_MCOLOR_WHITE );

		// anv: if ctrl is pressed in map inventory, show comparison with selected weapon
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL && gpComparedItemDescObject->usItem )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) )
					fComparisonMode = TRUE;
			}
		}

		// "PRIMARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 0 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sTop, gItemDescGenHeaderRegions[0].sRight - gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sBottom - gItemDescGenHeaderRegions[0].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "AP COSTS" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 2 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sTop, gItemDescGenHeaderRegions[1].sRight - gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sBottom - gItemDescGenHeaderRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "BURST / AUTOFIRE" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 3 ] );
		// If weapon has no burst/autofire, draw in greyish color.
		if( !fComparisonMode )
		{
			if (GetShotsPerBurst(gpItemDescObject) <= 0 && GetAutofireShotsPerFiveAPs(gpItemDescObject) <= 0)
				SetFontForeground( FONT_MCOLOR_DKGRAY );
		}
		else
		{
			if (GetShotsPerBurst(gpItemDescObject) <= 0 && GetAutofireShotsPerFiveAPs(gpItemDescObject) <= 0)
			{
				if (GetShotsPerBurst(gpComparedItemDescObject) <= 0 && GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) <= 0)
				{
					SetFontForeground( FONT_MCOLOR_DKGRAY );
				}
				else
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
				}
			}
			else
			{
				if (GetShotsPerBurst(gpComparedItemDescObject) <= 0 && GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) <= 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
				}
				else
				{
					SetFontForeground( FONT_MCOLOR_WHITE );
				}
			}
		}
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[2].sLeft, gItemDescGenHeaderRegions[2].sTop, gItemDescGenHeaderRegions[2].sRight - gItemDescGenHeaderRegions[2].sLeft, gItemDescGenHeaderRegions[2].sBottom - gItemDescGenHeaderRegions[2].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		SetFontForeground( 6 );

		///////////////////////////////////////////////////// INDEXES

		SetFontForeground( FONT_MCOLOR_WHITE );

		// Find out whether we need to display 2 or 3 indexes.
		UINT8 Limit;
		if (GetShotsPerBurst(gpItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0)
		{
			Limit = 3;
		}
		else
		{
			Limit = 2;
		}

		// Repeatedly draw each index: four separate column headers per index.
		for (UINT8 cnt = 0; cnt < Limit; cnt++)
		{
			for (UINT8 cnt2 = 0; cnt2 < 4; cnt2++)
			{
				// Select "PROPERTY", "0", "+" or "=" as appropriate.
				swprintf(pStr, L"%s", gzItemDescGenIndexes[ cnt2 ]);

				FindFontCenterCoordinates( gItemDescGenIndexRegions[cnt][cnt2].sLeft, gItemDescGenIndexRegions[cnt][cnt2].sTop, gItemDescGenIndexRegions[cnt][cnt2].sRight - gItemDescGenIndexRegions[cnt][cnt2].sLeft, gItemDescGenIndexRegions[cnt][cnt2].sBottom - gItemDescGenIndexRegions[cnt][cnt2].sTop, pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );
			}
		}

		SetFontForeground( 6 );
		//////////////////// ACCURACY
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 0;
			}
			else
			{
				ubNumLine = 2;
			}
			
			// Get accuracy value
			INT8 iAccuracyValue = (UsingNewCTHSystem() == true ? Weapon[ gpItemDescObject->usItem ].nAccuracy : Weapon[ gpItemDescObject->usItem ].bAccuracy);

			// Get Modified Accuracy value
			INT8 iFinalAccuracyValue = GetGunAccuracy( gpItemDescObject );

			// Get difference
			INT8 iAccuracyDifference = iFinalAccuracyValue - iAccuracyValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iAccuracyValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iAccuracyDifference, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalAccuracyValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get accuracy value
				INT8 iComparedAccuracyValue = (UsingNewCTHSystem() == true ? Weapon[ gpComparedItemDescObject->usItem ].nAccuracy : Weapon[ gpComparedItemDescObject->usItem ].bAccuracy);
				// Get Modified Accuracy value
				INT8 iComparedFinalAccuracyValue = GetGunAccuracy( gpComparedItemDescObject );
				// Get difference
				INT8 iComparedAccuracyDifference = iComparedFinalAccuracyValue - iComparedAccuracyValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedAccuracyValue - iAccuracyValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedAccuracyDifference - iAccuracyDifference, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalAccuracyValue - iFinalAccuracyValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
				ubNumLine = 0;
			else
				ubNumLine = 2;

			// Get accuracy value
			INT8 iAccuracyValue = (UsingNewCTHSystem() == true ? Weapon[ gpComparedItemDescObject->usItem ].nAccuracy : Weapon[ gpComparedItemDescObject->usItem ].bAccuracy);
			// Get Modified Accuracy value
			INT8 iFinalAccuracyValue = GetGunAccuracy( gpComparedItemDescObject );
			// Get difference
			INT8 iAccuracyDifference = iFinalAccuracyValue - iAccuracyValue;

			// Print base value
			DrawPropertyValueInColour( iAccuracyValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iAccuracyDifference, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalAccuracyValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
		}
		/////////////// DAMAGE
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
		{
			// Set line to draw into
			ubNumLine = 1;

			// Get base damage value
			UINT8 iDamageValue = GetBasicDamage ( gpItemDescObject );

			// Get Modified damage value
			UINT8 iFinalDamageValue = GetDamage ( gpItemDescObject );

			// Get difference
			INT8 iDamageModifier = iFinalDamageValue - iDamageValue;

			if( !fComparisonMode )
			{
				// Flugente: if we use buckshot, display damage a bullet does times number of bullets
				if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].numberOfBullets > 1 )
				{
					UINT8 damage = (UINT8)( iDamageValue * AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].multipleBulletDamageMultiplier / max( 1, AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].multipleBulletDamageDivisor ) );
					UINT8 finaldamage = (UINT8)( iFinalDamageValue * AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].multipleBulletDamageMultiplier / max( 1, AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].multipleBulletDamageDivisor ) );

					// Print base value
					DrawPropertyValueInColour_X( damage, AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].numberOfBullets, ubNumLine, 1, fComparisonMode, FALSE, TRUE );

					// Print modifier
					DrawPropertyValueInColour( iDamageModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );

					// Print final value
					DrawPropertyValueInColour_X( finaldamage, AmmoTypes[( *gpItemDescObject )[0]->data.gun.ubGunAmmoType].numberOfBullets, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
				else
				{
					DrawPropertyValueInColour( iDamageValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );

					// Print modifier
					DrawPropertyValueInColour( iDamageModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );

					// Print final value
					DrawPropertyValueInColour( iFinalDamageValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
			}
			else
			{
				// Get base damage value
				UINT8 iComparedDamageValue = GetBasicDamage ( gpComparedItemDescObject );
				// Get Modified damage value
				UINT8 iComparedFinalDamageValue = GetDamage ( gpComparedItemDescObject );
				// Get difference
				INT8 iComparedDamageModifier = iComparedFinalDamageValue - iComparedDamageValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedDamageValue - iDamageValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedDamageModifier - iDamageModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalDamageValue - iFinalDamageValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher )
		{
			// Set line to draw into
			ubNumLine = 1;

			// Get base damage value
			UINT8 iDamageValue = GetBasicDamage ( gpComparedItemDescObject );
			// Get Modified damage value
			UINT8 iFinalDamageValue = GetDamage ( gpComparedItemDescObject );
			// Get difference
			INT8 iDamageModifier = iFinalDamageValue - iDamageValue;
			// Print base value
			DrawPropertyValueInColour( iDamageValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iDamageModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalDamageValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
		}
		/////////////// RANGE
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 2;
			}
			else
			{
				ubNumLine = 0;
			}

			// Get base Range value
			UINT16 iRangeValue = Weapon[ gpItemDescObject->usItem ].usRange;

			// apply Ini modifiers
			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN )
				iRangeValue = (UINT16)( (FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ] / 10.0f );
			else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_LAUNCHER )
				iRangeValue = (UINT16)( (FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierLauncher / 10.0f );
			else if ( Item[gpItemDescObject->usItem].usItemClass & IC_THROWING_KNIFE )
				iRangeValue = (UINT16)((FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierThrowingKnife / 10.0f);

			// Get Final Range value
			UINT16 iFinalRangeValue = (UINT16)( GunRange( gpItemDescObject, gpItemDescSoldier ) / 10.0f );

			// Get difference
			INT16 iRangeModifier = iFinalRangeValue - iRangeValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Range value
				UINT16 iComparedRangeValue = Weapon[ gpComparedItemDescObject->usItem ].usRange;

				// apply Ini modifiers
				if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_GUN )
					iComparedRangeValue = (UINT16)( (FLOAT)iComparedRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ] / 10.0f );
				else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER )
					iComparedRangeValue = (UINT16)( (FLOAT)iComparedRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierLauncher / 10.0f );
				else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_THROWING_KNIFE )
					iComparedRangeValue = (UINT16)((FLOAT)iComparedRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierThrowingKnife / 10.0f);

				// Get Final Range value
				UINT16 iComparedFinalRangeValue = (UINT16)( GunRange( gpComparedItemDescObject, gpItemDescSoldier ) / 10.0f );
				// Get difference
				INT16 iComparedRangeModifier = iComparedFinalRangeValue - iComparedRangeValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedRangeValue - iRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedRangeModifier - iRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalRangeValue - iFinalRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
		{
			if (UsingNewCTHSystem() == true)
				ubNumLine = 2;
			else
				ubNumLine = 0;

			// Get base Range value
			UINT16 iRangeValue = Weapon[ gpComparedItemDescObject->usItem ].usRange;

			// apply Ini modifiers
			if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_GUN )
				iRangeValue = (UINT16)( (FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ] / 10.0f );
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER )
				iRangeValue = (UINT16)( (FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierLauncher / 10.0f );
			else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_THROWING_KNIFE )
				iRangeValue = (UINT16)((FLOAT)iRangeValue * (FLOAT)(gGameExternalOptions.iGunRangeModifier / 100) * gItemSettings.fRangeModifierThrowingKnife / 10.0f);

			// Get Final Range value
			UINT16 iFinalRangeValue = (UINT16)( GunRange( gpComparedItemDescObject, NULL ) / 10.0f );
			// Get difference
			INT16 iRangeModifier = iFinalRangeValue - iRangeValue;
			// Print base value
			DrawPropertyValueInColour( iRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
		}

		/////////////// GUN HANDLING
		if ( UsingNewCTHSystem() == TRUE && 
			Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			// Set line to draw into
			ubNumLine = 3;

			// Get base Gun Handling value
			UINT16 iHandlingValue = Weapon[ gpItemDescObject->usItem ].ubHandling;
			// modify by ini values
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN )
				iHandlingValue *= gItemSettings.fHandlingModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iHandlingValue *= gItemSettings.fHandlingModifierLauncher;

			// Get modifier
			INT16 iHandlingModifier = (iHandlingValue * GetObjectModifier( gpItemDescSoldier, gpItemDescObject , ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING )) / 100;

			// Get Final Gun Handling value
			UINT16 iFinalHandlingValue =  iHandlingValue + iHandlingModifier;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iHandlingValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iHandlingModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalHandlingValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Gun Handling value
				UINT16 iComparedHandlingValue = Weapon[ gpComparedItemDescObject->usItem ].ubHandling;
				// modify by ini values
				if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
					iComparedHandlingValue *= gItemSettings.fHandlingModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
				else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
					iComparedHandlingValue *= gItemSettings.fHandlingModifierLauncher;
				// Get modifier
				INT16 iComparedHandlingModifier = (iHandlingValue * GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject , ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING )) / 100;
				// Get Final Gun Handling value
				UINT16 iComparedFinalHandlingValue =  iComparedHandlingValue + iComparedHandlingModifier;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedHandlingValue - iHandlingValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedHandlingModifier - iHandlingModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalHandlingValue - iFinalHandlingValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
		}
		else if( fComparisonMode && UsingNewCTHSystem() == TRUE && 
				Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			// Set line to draw into
			ubNumLine = 3;
			// Get base Gun Handling value
			UINT16 iHandlingValue = Weapon[ gpComparedItemDescObject->usItem ].ubHandling;
			// modify by ini values
			if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
				iHandlingValue *= gItemSettings.fHandlingModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iHandlingValue *= gItemSettings.fHandlingModifierLauncher;
			// Get modifier
			INT16 iHandlingModifier = (iHandlingValue * GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject , ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING )) / 100;
			// Get Final Gun Handling value
			UINT16 iFinalHandlingValue =  iHandlingValue + iHandlingModifier;
			// Print base value
			DrawPropertyValueInColour( iHandlingValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iHandlingModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalHandlingValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
		}

		/////////////// AIM LEVELS
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 4;
			}
			else
			{
				ubNumLine = 3;
			}
			
			// Get Final Aiming Levels
			UINT16 iFinalAimLevelsValue = GetAllowedAimingLevelsForItem( gpItemDescSoldier, gpItemDescObject, ANIM_STAND );

			// Get modifier
			INT16 iAimLevelsModifier = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS );
			iAimLevelsModifier += GetAimLevelsTraitModifier( gpItemDescSoldier, gpItemDescObject );

			// Get Base Value
			UINT16 iAimLevelsValue = iFinalAimLevelsValue - iAimLevelsModifier;

			if( !fComparisonMode )
			{
				if (UsingNewCTHSystem() == true)
				{
					// Print base value
					DrawPropertyValueInColour( iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print modifier
					DrawPropertyValueInColour( iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print final value
					DrawPropertyValueInColour( iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
				}
				else
				{
					// Print base value
					DrawPropertyValueInColour( iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print modifier
					DrawPropertyValueInColour( iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					// Print final value
					DrawPropertyValueInColour( iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
			}
			else
			{
				// Get Final Aiming Levels
				UINT16 iComparedFinalAimLevelsValue = GetAllowedAimingLevelsForItem( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND );

				// Get modifier
				INT16 iComparedAimLevelsModifier = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS );
				iComparedAimLevelsModifier += GetAimLevelsTraitModifier( gpItemDescSoldier, gpComparedItemDescObject );

				// Get Base Value
				UINT16 iComparedAimLevelsValue = iComparedFinalAimLevelsValue - iComparedAimLevelsModifier;

				if (UsingNewCTHSystem() == true)
				{
					// Print difference in base value
					DrawPropertyValueInColour( iComparedAimLevelsValue - iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print difference in modifier
					DrawPropertyValueInColour( iComparedAimLevelsModifier - iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalAimLevelsValue - iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
				}
				else
				{
					// Print difference in base value
					DrawPropertyValueInColour( iComparedAimLevelsValue - iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print difference in modifier
					DrawPropertyValueInColour( iComparedAimLevelsModifier - iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalAimLevelsValue - iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
				}
			}
		}
		else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
		{
			if (UsingNewCTHSystem() == true)
				ubNumLine = 4;
			else
				ubNumLine = 3;
			// Get Final Aiming Levels
			UINT16 iFinalAimLevelsValue = GetAllowedAimingLevelsForItem( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND );
			// Get modifier
			INT16 iAimLevelsModifier = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS );
			iAimLevelsModifier += GetAimLevelsTraitModifier( gpItemDescSoldier, gpComparedItemDescObject );
			// Get Base Value
			UINT16 iAimLevelsValue = iFinalAimLevelsValue - iAimLevelsModifier;
			if (UsingNewCTHSystem() == true)
			{
				// Print base value
				DrawPropertyValueInColour( iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
				// Print modifier
				DrawPropertyValueInColour( iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
			}
			else
			{
				// Print base value
				DrawPropertyValueInColour( iAimLevelsValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
				// Print modifier
				DrawPropertyValueInColour( iAimLevelsModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalAimLevelsValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			}
		}
		//////////////// OCTH AIMING BONUS
		if ( UsingNewCTHSystem() == false && 
			(GetFlatAimBonus( gpItemDescObject ) != 0 || Item[gpItemDescObject->usItem].aimbonus != 0) )
		//if ( UsingNewCTHSystem() == false && GetAimBonus( gpItemDescSoldier, gpItemDescObject, 100, 1 ) != 0 )
		{
			// Set line to draw into
			ubNumLine = 4;

			// Get base Aim Bonus value
			INT16 iAimBonusValue = __max(0, Item[ gpItemDescObject->usItem ].aimbonus);
			// Get final Aim Bonus value
			INT16 iFinalAimBonusValue = GetFlatAimBonus( gpItemDescObject );
			// Get Aim Bonus modifier
			INT16 iAimBonusModifier = iFinalAimBonusValue - iAimBonusValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Aim Bonus value
				INT16 iComparedAimBonusValue = __max(0, Item[ gpComparedItemDescObject->usItem ].aimbonus);
				// Get final Aim Bonus value
				INT16 iComparedFinalAimBonusValue = GetFlatAimBonus( gpComparedItemDescObject );
				// Get Aim Bonus modifier
				INT16 iComparedAimBonusModifier = iComparedFinalAimBonusValue - iComparedAimBonusValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedAimBonusValue - iAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedAimBonusModifier - iAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalAimBonusValue - iFinalAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && UsingNewCTHSystem() == false && (GetFlatAimBonus( gpComparedItemDescObject ) != 0 || Item[gpComparedItemDescObject->usItem].aimbonus != 0) )
		{
			// Set line to draw into
			ubNumLine = 4;
			// Get base Aim Bonus value
			INT16 iAimBonusValue = __max(0, Item[ gpComparedItemDescObject->usItem ].aimbonus);
			// Get final Aim Bonus value
			INT16 iFinalAimBonusValue = GetFlatAimBonus( gpComparedItemDescObject );
			// Get Aim Bonus modifier
			INT16 iAimBonusModifier = iFinalAimBonusValue - iAimBonusValue;
			// Print base value
			DrawPropertyValueInColour( iAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyValueInColour( iAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		//////////////// SCOPE MAGNIFICATION FACTOR 
		if ( UsingNewCTHSystem() == true && Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN )
		{
			// Set line to draw into
			ubNumLine = 5;

			// Get base Magnification value
			FLOAT iScopeMagValue = __max(1.0f, Item[ gpItemDescObject->usItem ].scopemagfactor);
			// Get best Magnification value
			FLOAT iScopeMagModifier = GetHighestScopeMagnificationFactor( gpItemDescObject );
			// Get final Magnification value
			FLOAT iFinalScopeMagValue = __max( iScopeMagValue, iScopeMagModifier );
			// get the real effective mag factor for this soldier
			iFinalScopeMagValue = CalcEffectiveMagFactor(gpItemDescSoldier, iFinalScopeMagValue);

			if( !fComparisonMode )
			{
				// Print base value
				if( iScopeMagValue > 1.0f )
					DrawPropertyValueInColourFloat( iScopeMagValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 5 );
				else
					DrawPropertyTextInColour( L"--", ubNumLine, 1 ); 
				// Print modifier
				if (iScopeMagModifier > 1.0f && iScopeMagModifier > iScopeMagValue)
					DrawPropertyValueInColourFloat( iScopeMagModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE  );
				else
					DrawPropertyTextInColour( L"--", ubNumLine, 2 ); 
				// Print final value
				DrawPropertyValueInColourFloat( iFinalScopeMagValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Magnification value
				FLOAT iComparedScopeMagValue = __max(1.0f, Item[ gpComparedItemDescObject->usItem ].scopemagfactor);
				// Get best Magnification value
				FLOAT iComparedScopeMagModifier = GetHighestScopeMagnificationFactor( gpComparedItemDescObject );
				// Get final Magnification value
				FLOAT iComparedFinalScopeMagValue = __max( iComparedScopeMagValue, iComparedScopeMagModifier );
				// get the real effective mag factor for this soldier
				iComparedFinalScopeMagValue = CalcEffectiveMagFactor(gpItemDescSoldier, iComparedFinalScopeMagValue);

				// Print difference in base value
				DrawPropertyValueInColourFloat( iComparedScopeMagValue - iScopeMagValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColourFloat( iComparedScopeMagModifier - iScopeMagModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColourFloat( iComparedFinalScopeMagValue - iFinalScopeMagValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && UsingNewCTHSystem() == true && Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_GUN )
		{
			ubNumLine = 5;
			// Get base Magnification value
			FLOAT iScopeMagValue = __max(1.0f, Item[ gpComparedItemDescObject->usItem ].scopemagfactor);
			// Get best Magnification value
			FLOAT iScopeMagModifier = GetHighestScopeMagnificationFactor( gpComparedItemDescObject );
			// Get final Magnification value
			FLOAT iFinalScopeMagValue = __max( iScopeMagValue, iScopeMagModifier );
			// get the real effective mag factor for this soldier
			iFinalScopeMagValue = CalcEffectiveMagFactor(gpItemDescSoldier, iFinalScopeMagValue);

			// Print base value
			if( iScopeMagValue > 1.0f )
				DrawPropertyValueInColourFloat( iScopeMagValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			else
				DrawPropertyTextInColour( L"=", ubNumLine, 1 ); 
			// Print modifier
			if (iScopeMagModifier > 1.0f && iScopeMagModifier > iScopeMagValue)
				DrawPropertyValueInColourFloat( iScopeMagModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE  );
			else
				DrawPropertyTextInColour( L"=", ubNumLine, 2 ); 
			// Print final value
			DrawPropertyValueInColourFloat( iFinalScopeMagValue - 1.0f, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		/////////////////// OCTH MINIMUM RANGE FOR AIMING BONUS
		if( UsingNewCTHSystem() == false && 
			( Item[gpItemDescObject->usItem].minrangeforaimbonus > 0 || GetMinRangeForAimBonus( NULL, gpItemDescObject ) > 0 ) )
		{
			// Set line to draw into
			ubNumLine = 5;

			// Get base Minimum Range For Aim Bonus value
			INT16 iMinRangeForAimBonusValue = Item[gpItemDescObject->usItem].minrangeforaimbonus / 10;

			// Get final Minimum Range For Aim Bonus value
			INT16 iFinalMinRangeForAimBonusValue = GetMinRangeForAimBonus( NULL, gpItemDescObject ) / 10;

			// Get Minimum Range For Aim Bonus modifier
			INT16 iMinRangeForAimBonusModifier = iFinalMinRangeForAimBonusValue - iMinRangeForAimBonusValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iMinRangeForAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iMinRangeForAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalMinRangeForAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Minimum Range For Aim Bonus value
				INT16 iComparedMinRangeForAimBonusValue = Item[gpComparedItemDescObject->usItem].minrangeforaimbonus / 10;
				// Get final Minimum Range For Aim Bonus value
				INT16 iComparedFinalMinRangeForAimBonusValue = GetMinRangeForAimBonus( NULL, gpComparedItemDescObject ) / 10;
				// Get Minimum Range For Aim Bonus modifier
				INT16 iComparedMinRangeForAimBonusModifier = iComparedFinalMinRangeForAimBonusValue - iComparedMinRangeForAimBonusValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedMinRangeForAimBonusValue - iMinRangeForAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedMinRangeForAimBonusModifier - iMinRangeForAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalMinRangeForAimBonusValue - iFinalMinRangeForAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && UsingNewCTHSystem() == false && 
				( Item[gpComparedItemDescObject->usItem].minrangeforaimbonus > 0 || GetMinRangeForAimBonus( NULL, gpComparedItemDescObject ) > 0 ) )
		{
			ubNumLine = 5;
			// Get base Minimum Range For Aim Bonus value
			INT16 iMinRangeForAimBonusValue = Item[gpComparedItemDescObject->usItem].minrangeforaimbonus / 10;
			// Get final Minimum Range For Aim Bonus value
			INT16 iFinalMinRangeForAimBonusValue = GetMinRangeForAimBonus( NULL, gpComparedItemDescObject ) / 10;
			// Get Minimum Range For Aim Bonus modifier
			INT16 iMinRangeForAimBonusModifier = iFinalMinRangeForAimBonusValue - iMinRangeForAimBonusValue;
			// Print base value
			DrawPropertyValueInColour( iMinRangeForAimBonusValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
			// Print modifier
			DrawPropertyValueInColour( iMinRangeForAimBonusModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalMinRangeForAimBonusValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );	
		}

		///////////////// (LASER) PROJECTION FACTOR
		// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor
		if (UsingNewCTHSystem() == true && 
			( (Item[gpItemDescObject->usItem].projectionfactor > 1.0 || GetProjectionFactor( gpItemDescObject ) > 1.0) ||
			( GetBestLaserRange( gpItemDescObject ) > 0
			&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) )
		{
			// Set line to draw into
			ubNumLine = 6;

			FLOAT iProjectionValue = 0;
			FLOAT iProjectionModifier = 0;
			FLOAT iFinalProjectionValue = 0;
			BOOLEAN bNewCode = FALSE;

			if ( GetBestLaserRange( gpItemDescObject ) > 0
				&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) )
			{
				// Get base laser range
				iProjectionValue = __max(0, Item[ gpItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE);
				// Get best laser range
				iProjectionModifier = ((FLOAT)GetBestLaserRange( gpItemDescObject ) / CELL_X_SIZE);
				// Get final laser range
				iFinalProjectionValue = __max( iProjectionValue, iProjectionModifier );
				bNewCode = TRUE;
			}
			else
			{
				// Get base Projection value
				iProjectionValue = __max(1.0f, Item[ gpItemDescObject->usItem ].projectionfactor);
				// Get best Projection value
				iProjectionModifier = GetProjectionFactor( gpItemDescObject );
				// Get final Projection value
				iFinalProjectionValue = __max( iProjectionValue, iProjectionModifier );
			}
	
			if( !fComparisonMode )
			{
				if ( bNewCode )
				{
					// Print base value
					if( iProjectionValue > 0.0f )
						DrawPropertyValueInColourFloat( iProjectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 5 );
					else
						DrawPropertyTextInColour( L"--", ubNumLine, 1 );
					// Print modifier
					if (iProjectionModifier > 0.0f && iProjectionModifier > iProjectionValue)
						DrawPropertyValueInColourFloat( iProjectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					else
						DrawPropertyTextInColour( L"--", ubNumLine, 2 );
					// Print final value
					DrawPropertyValueInColourFloat( iFinalProjectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
				else
				{
					// Print base value
					if( iProjectionValue > 1.0f )
						DrawPropertyValueInColourFloat( iProjectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 5 );
					else
						DrawPropertyTextInColour( L"--", ubNumLine, 1 );
					// Print modifier
					if (iProjectionModifier > 1.0f && iProjectionModifier > iProjectionValue)
						DrawPropertyValueInColourFloat( iProjectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					else
						DrawPropertyTextInColour( L"--", ubNumLine, 2 );
					// Print final value
					DrawPropertyValueInColourFloat( iFinalProjectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
			}
			else
			{
				FLOAT iComparedProjectionValue = 0;
				FLOAT iComparedProjectionModifier = 0;
				FLOAT iComparedFinalProjectionValue = 0;
				if ( bNewCode )
				{
					// Get base laser range
					iComparedProjectionValue = __max(0, Item[ gpComparedItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE);
					// Get best laser range
					iComparedProjectionModifier = ((FLOAT)GetBestLaserRange( gpComparedItemDescObject ) / CELL_X_SIZE);
					// Get final laser range
					iComparedFinalProjectionValue = __max( iComparedProjectionValue, iComparedProjectionModifier );
				}
				else
				{
					// Get base Projection value
					iComparedProjectionValue = __max(1.0f, Item[ gpComparedItemDescObject->usItem ].projectionfactor);
					// Get best Projection value
					iComparedProjectionModifier = GetProjectionFactor( gpComparedItemDescObject );
					// Get final Projection value
					iComparedFinalProjectionValue = __max( iComparedProjectionValue, iComparedProjectionModifier );
				}
				// Print difference in base value
				DrawPropertyValueInColourFloat( iComparedProjectionValue - iProjectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColourFloat( iComparedProjectionModifier - iProjectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColourFloat( iComparedFinalProjectionValue - iFinalProjectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && UsingNewCTHSystem() == true && 
				( (Item[gpComparedItemDescObject->usItem].projectionfactor > 1.0 || GetProjectionFactor( gpComparedItemDescObject ) > 1.0) ||
				( GetBestLaserRange( gpItemDescObject ) > 0
				&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) )
		{
			ubNumLine = 6;

			FLOAT iProjectionValue = 0;
			FLOAT iProjectionModifier = 0;
			FLOAT iFinalProjectionValue = 0;
			BOOLEAN bNewCode = FALSE;

			if ( GetBestLaserRange( gpComparedItemDescObject ) > 0
				&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) )
			{
				// Get base laser range
				iProjectionValue = __max(0, Item[ gpComparedItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE);
				// Get best laser range
				iProjectionModifier = ((FLOAT)GetBestLaserRange( gpComparedItemDescObject ) / CELL_X_SIZE);
				// Get final laser range
				iFinalProjectionValue = __max( iProjectionValue, iProjectionModifier );
				bNewCode = TRUE;
			}
			else
			{
				// Get base Projection value
				iProjectionValue = __max(1.0f, Item[ gpComparedItemDescObject->usItem ].projectionfactor);
				// Get best Projection value
				iProjectionModifier = GetProjectionFactor( gpComparedItemDescObject );
				// Get final Projection value
				iFinalProjectionValue = __max( iProjectionValue, iProjectionModifier );
			}
			if ( bNewCode )
			{
				// Print base value
				if( iProjectionValue > 0.0f )
					DrawPropertyValueInColourFloat( iProjectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				else
					DrawPropertyTextInColour( L"=", ubNumLine, 1 );
				// Print modifier
				if (iProjectionModifier > 0.0f && iProjectionModifier > iProjectionValue)
					DrawPropertyValueInColourFloat( iProjectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				else
					DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( iFinalProjectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
			else
			{
				// Print base value
				if( iProjectionValue > 1.0f )
					DrawPropertyValueInColourFloat( iProjectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				else
					DrawPropertyTextInColour( L"=", ubNumLine, 1 );
				// Print modifier
				if (iProjectionModifier > 1.0f && iProjectionModifier > iProjectionValue)
					DrawPropertyValueInColourFloat( iProjectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				else
					DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( iFinalProjectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		///////////////// OCTH TO-HIT BONUS
		if (UsingNewCTHSystem() == false && 
			(Item[gpItemDescObject->usItem].tohitbonus != 0 || GetFlatToHitBonus( gpItemDescObject ) != 0) )
		{
			// Set line to draw into
			ubNumLine = 6;

			// Get base To Hit value
			INT16 iToHitValue = Item[ gpItemDescObject->usItem ].tohitbonus;
			// Get final Projection value
			INT16 iFinalToHitValue = GetFlatToHitBonus( gpItemDescObject );
			// Get To Hit Modifier value
			INT16 iToHitModifier = iFinalToHitValue - iToHitValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iToHitValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iToHitModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalToHitValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base To Hit value
				INT16 iComparedToHitValue = Item[ gpComparedItemDescObject->usItem ].tohitbonus;
				// Get final Projection value
				INT16 iComparedFinalToHitValue = GetFlatToHitBonus( gpComparedItemDescObject );
				// Get To Hit Modifier value
				INT16 iComparedToHitModifier = iComparedFinalToHitValue - iComparedToHitValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedToHitValue - iToHitValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedToHitModifier - iToHitModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalToHitValue - iFinalToHitValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}

		}
		else if( fComparisonMode && UsingNewCTHSystem() == false && 
				(Item[gpComparedItemDescObject->usItem].tohitbonus != 0 || GetFlatToHitBonus( gpComparedItemDescObject ) != 0) )
		{
			ubNumLine = 6;
			// Get base To Hit value
			INT16 iToHitValue = Item[ gpComparedItemDescObject->usItem ].tohitbonus;
			// Get final Projection value
			INT16 iFinalToHitValue = GetFlatToHitBonus( gpComparedItemDescObject );
			// Get To Hit Modifier value
			INT16 iToHitModifier = iFinalToHitValue - iToHitValue;
			// Print base value
			DrawPropertyValueInColour( iToHitValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyValueInColour( iToHitModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalToHitValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		///////////////// OCTH BEST LASER RANGE
		if ( !UsingNewCTHSystem() && GetBestLaserRange( gpItemDescObject ) > 0 )
		{
			// Set line to draw into
			ubNumLine = 7;

			// Get base Best Laser Range value
			INT16 iBestLaserRangeValue = Item[ gpItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE;
			// Get final Best Laser Range value
			INT16 iFinalBestLaserRangeValue = GetAverageBestLaserRange( gpItemDescObject ) / CELL_X_SIZE;
			// Get Best Laser Range Modifier value
			INT16 iBestLaserRangeModifier = iFinalBestLaserRangeValue - iBestLaserRangeValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iBestLaserRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iBestLaserRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalBestLaserRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Best Laser Range value
				INT16 iComparedBestLaserRangeValue = Item[ gpComparedItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE;
				// Get final Best Laser Range value
				INT16 iComparedFinalBestLaserRangeValue = GetAverageBestLaserRange( gpComparedItemDescObject ) / CELL_X_SIZE;
				// Get Best Laser Range Modifier value
				INT16 iComparedBestLaserRangeModifier = iComparedFinalBestLaserRangeValue - iComparedBestLaserRangeValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedBestLaserRangeValue - iBestLaserRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedBestLaserRangeModifier - iBestLaserRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalBestLaserRangeValue - iFinalBestLaserRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( !UsingNewCTHSystem() && ( fComparisonMode && GetBestLaserRange( gpComparedItemDescObject ) > 0 ) )
		{
			// Set line to draw into
			ubNumLine = 7;
			// Get base Best Laser Range value
			INT16 iBestLaserRangeValue = Item[ gpComparedItemDescObject->usItem ].bestlaserrange * gItemSettings.fBestLaserRangeModifier / CELL_X_SIZE;
			// Get final Best Laser Range value
			INT16 iFinalBestLaserRangeValue = GetAverageBestLaserRange( gpComparedItemDescObject ) / CELL_X_SIZE;
			// Get Best Laser Range Modifier value
			INT16 iBestLaserRangeModifier = iFinalBestLaserRangeValue - iBestLaserRangeValue;
			// Print base value
			DrawPropertyValueInColour( iBestLaserRangeValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iBestLaserRangeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
			// Print final value
			DrawPropertyValueInColour( iFinalBestLaserRangeValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
		}

		///////////////// FLASH SUPPRESSION
		if ( IsFlashSuppressorAlt( gpItemDescObject ) == TRUE )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 7;
			}
			else
			{
				ubNumLine = 8;
			}

			// Does gun have flash suppression?
			BOOLEAN iFlashValue = Item[ gpItemDescObject->usItem ].hidemuzzleflash;

			if( !fComparisonMode )
			{		
				if ( iFlashValue )
				{
					// Print base value
					DrawPropertyTextInColour( L"Y", ubNumLine, 1 );
					// Print modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 );

				}
				else
				{
					// Print base value
					DrawPropertyTextInColour( L"--", ubNumLine, 1 );
					// Print modifier
					DrawPropertyTextInColour( L"Y", ubNumLine, 2, ITEMDESC_FONTPOSITIVE );
				}			
				// Print final value
				DrawPropertyTextInColour( L"Y", ubNumLine, 3, FONT_MCOLOR_WHITE );
			}
			else if( IsFlashSuppressorAlt( gpComparedItemDescObject ) == TRUE )
			{
				BOOLEAN iComparedFlashValue = Item[ gpComparedItemDescObject->usItem ].hidemuzzleflash;
				if ( iFlashValue )
				{
					if (iComparedFlashValue)
					{
						DrawPropertyTextInColour( L"=", ubNumLine, 1 );
						DrawPropertyTextInColour( L"=", ubNumLine, 2 );
					}
					else
					{
						DrawPropertyTextInColour( L"N", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
						DrawPropertyTextInColour( L"Y", ubNumLine, 2, ITEMDESC_FONTPOSITIVE );
					}
				}
				else
				{
					if (!iComparedFlashValue)
					{
						DrawPropertyTextInColour( L"=", ubNumLine, 1 );
						DrawPropertyTextInColour( L"=", ubNumLine, 2 );
					}
					else
					{
						DrawPropertyTextInColour( L"Y", ubNumLine, 1, ITEMDESC_FONTPOSITIVE );
						DrawPropertyTextInColour( L"N", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
					}
				}
				// Print final value
				DrawPropertyTextInColour( L"=", ubNumLine, 3 );
			}
			else
			{
				if ( iFlashValue )
				{
					// Print base value
					DrawPropertyTextInColour( L"N", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
					// Print modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				}
				else
				{
					// Print base value
					DrawPropertyTextInColour( L"--", ubNumLine, 1 );
					// Print modifier
					DrawPropertyTextInColour( L"N", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				}			
				// Print final value
				DrawPropertyTextInColour( L"N", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}
		}
		else if( fComparisonMode && IsFlashSuppressorAlt( gpComparedItemDescObject ) == TRUE )
		{
			if (UsingNewCTHSystem() == true)
				ubNumLine = 7;
			else
				ubNumLine = 8;
			// Does gun have flash suppression?
			BOOLEAN iFlashValue = Item[ gpComparedItemDescObject->usItem ].hidemuzzleflash;
			if ( iFlashValue )
			{
				// Print base value
				DrawPropertyTextInColour( L"Y", ubNumLine, 1, ITEMDESC_FONTPOSITIVE );
				// Print modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			}
			else
			{
				// Print base value
				DrawPropertyTextInColour( L"=", ubNumLine, 1 );
				// Print modifier
				DrawPropertyTextInColour( L"Y", ubNumLine, 2, ITEMDESC_FONTPOSITIVE );
			}			
			// Print final value
			DrawPropertyTextInColour( L"Y", ubNumLine, 3, ITEMDESC_FONTPOSITIVE );
		}
		////////////////// LOUDNESS
		//if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 8;
			}
			else
			{
				ubNumLine = 9;
			}

			// Get base Loudness value
			UINT8 iLoudnessValue = Weapon[Item[gpItemDescObject->usItem].ubClassIndex].ubAttackVolume;

			// Get final Loudness value
			INT16 iFinalLoudnessValue = GetFinalLoudness( gpItemDescObject );

			// Get Loudness modifier
			INT16 iLoudnessModifier = iFinalLoudnessValue - iLoudnessValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iLoudnessValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iLoudnessModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalLoudnessValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Loudness value
				UINT8 iComparedLoudnessValue = Weapon[Item[gpComparedItemDescObject->usItem].ubClassIndex].ubAttackVolume;
				// Get final Loudness value
				INT16 iComparedFinalLoudnessValue = GetFinalLoudness( gpComparedItemDescObject );
				// Get Loudness modifier
				INT16 iComparedLoudnessModifier = iComparedFinalLoudnessValue - iComparedLoudnessValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedLoudnessValue - iLoudnessValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedLoudnessModifier - iLoudnessModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalLoudnessValue - iFinalLoudnessValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
		}

		/////////////////// RELIABILITY
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 9;
			}
			else
			{
				ubNumLine = 10;
			}

			// Get base Reliability value
			INT8 iReliabilityValue = Item[gpItemDescObject->usItem].bReliability;

			// Get final Reliability value
			INT16 iFinalReliabilityValue = GetReliability( gpItemDescObject );

			// Get Reliability modifier
			INT16 iReliabilityModifier = iFinalReliabilityValue - iReliabilityValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iReliabilityValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iReliabilityModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalReliabilityValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Reliability value
				INT8 iComparedReliabilityValue = Item[gpComparedItemDescObject->usItem].bReliability;
				// Get final Reliability value
				INT16 iComparedFinalReliabilityValue = GetReliability( gpComparedItemDescObject );
				// Get Reliability modifier
				INT16 iComparedReliabilityModifier = iComparedFinalReliabilityValue - iComparedReliabilityValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedReliabilityValue - iReliabilityValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedReliabilityModifier - iReliabilityModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalReliabilityValue - iFinalReliabilityValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		/////////////////// REPAIR EASE
		{
			// Set line to draw into
			if (UsingNewCTHSystem() == true)
			{
				ubNumLine = 10;
			}
			else
			{
				ubNumLine = 11;
			}

			// Get base Reliability value
			INT8 iRepairEaseValue = Item[gpItemDescObject->usItem].bRepairEase;

			// Get final Reliability value
			INT8 iFinalRepairEaseValue = iRepairEaseValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( 0, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Reliability value
				INT8 iComparedRepairEaseValue = Item[gpComparedItemDescObject->usItem].bRepairEase;
				// Get final Reliability value
				INT8 iComparedFinalRepairEaseValue = iComparedRepairEaseValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedRepairEaseValue - iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( 0, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalRepairEaseValue - iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		///////////////////// DRAW AP
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem].rocketlauncher )
		{
			// Set line to draw into
			ubNumLine = 13;

			// Get final Draw Cost
			INT16 iFinalDrawAPCost = (Weapon[ gpItemDescObject->usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(gpItemDescObject)) / 100);
			// final draw cost modified by traits
			if( gGameOptions.fNewTraitSystem )
			{
				// LMGs
				if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_LMG && HAS_SKILL_TRAIT(gpItemDescSoldier, AUTO_WEAPONS_NT) )
					iFinalDrawAPCost = (FLOAT)iFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubAWPercentReadyLMGReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, AUTO_WEAPONS_NT) ) / 100.0 );
				// Pistols and Revolvers
				else if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_PISTOL && HAS_SKILL_TRAIT(gpItemDescSoldier, GUNSLINGER_NT) )
					iFinalDrawAPCost = (FLOAT)iFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, GUNSLINGER_NT) ) / 100.0 );
			}

			// Get base Draw Cost
			INT16 iDrawAPCost = Weapon[ gpItemDescObject->usItem ].ubReadyTime;

			// Get Draw Cost Modifier
			INT16 iDrawAPCostModifier = iFinalDrawAPCost - iDrawAPCost;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iDrawAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iDrawAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalDrawAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final Draw Cost
				INT16 iComparedFinalDrawAPCost = (Weapon[ gpComparedItemDescObject->usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(gpComparedItemDescObject)) / 100);
				// final draw cost modified by traits
				if( gGameOptions.fNewTraitSystem )
				{
					// LMGs
					if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_LMG && HAS_SKILL_TRAIT(gpItemDescSoldier, AUTO_WEAPONS_NT) )
						iComparedFinalDrawAPCost = (FLOAT)iComparedFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubAWPercentReadyLMGReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, AUTO_WEAPONS_NT) ) / 100.0 );
					// Pistols and Revolvers
					else if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_PISTOL && HAS_SKILL_TRAIT(gpItemDescSoldier, GUNSLINGER_NT) )
						iComparedFinalDrawAPCost = (FLOAT)iComparedFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, GUNSLINGER_NT) ) / 100.0 );
				}

				// Get base Draw Cost
				INT16 iComparedDrawAPCost = Weapon[ gpComparedItemDescObject->usItem ].ubReadyTime;
				// Get Draw Cost Modifier
				INT16 iComparedDrawAPCostModifier = iComparedFinalDrawAPCost - iComparedDrawAPCost;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedDrawAPCost - iDrawAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedDrawAPCostModifier - iDrawAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalDrawAPCost - iFinalDrawAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
		}
		else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem].rocketlauncher )
		{
			ubNumLine = 13;
			// Get final Draw Cost
			INT16 iFinalDrawAPCost = (Weapon[ gpComparedItemDescObject->usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(gpComparedItemDescObject)) / 100);
			// final draw cost modified by traits
			if( gGameOptions.fNewTraitSystem )
			{
				// LMGs
				if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_LMG && HAS_SKILL_TRAIT(gpItemDescSoldier, AUTO_WEAPONS_NT) )
					iFinalDrawAPCost = (FLOAT)iFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubAWPercentReadyLMGReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, AUTO_WEAPONS_NT) ) / 100.0 );
				// Pistols and Revolvers
				else if( Weapon[gpItemDescObject->usItem].ubWeaponType == GUN_PISTOL && HAS_SKILL_TRAIT(gpItemDescSoldier, GUNSLINGER_NT) )
					iFinalDrawAPCost = (FLOAT)iFinalDrawAPCost * __max(0, (FLOAT)(100 - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(gpItemDescSoldier, GUNSLINGER_NT) ) / 100.0 );
			}

			// Get base Draw Cost
			INT16 iDrawAPCost = Weapon[ gpComparedItemDescObject->usItem ].ubReadyTime;
			// Get Draw Cost Modifier
			INT16 iDrawAPCostModifier = iFinalDrawAPCost - iDrawAPCost;
			// Print base value
			if( iDrawAPCost > 0 )
				DrawPropertyValueInColour( iDrawAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
			else
				DrawPropertyTextInColour( L"=", ubNumLine, 1 );
			// Print modifier
			DrawPropertyValueInColour( iDrawAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
			// Print final value
			if( iFinalDrawAPCost > 0 )
				DrawPropertyValueInColour( iFinalDrawAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
			else
				DrawPropertyTextInColour( L"=", ubNumLine, 3 );

		}

		////////////////// SINGLE SHOT AP
		if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
		{
			// Set line to draw into
			ubNumLine = 14;

			// Get final Attack Cost
			INT16 iFinalSingleAPCost = (INT16)(ubAttackAPs * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpItemDescObject, WM_NORMAL ) + 0.5);

			// Get base Attack Cost
			INT16 iSingleAPCost = ubBasicAttackAPs;

			// Get Attack Cost Modifier
			INT16 iSingleAPCostModifier = iFinalSingleAPCost - iSingleAPCost;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iSingleAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iSingleAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalSingleAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if( !Weapon[gpComparedItemDescObject->usItem].NoSemiAuto )
			{
				// Get final Attack Cost
				INT16 iComparedFinalSingleAPCost = (INT16)(BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier ) * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpItemDescObject, WM_NORMAL ) + 0.5);
				// Get base Attack Cost
				INT16 iComparedSingleAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject );
				// Get Attack Cost Modifier
				INT16 iComparedSingleAPCostModifier = iComparedFinalSingleAPCost - iComparedSingleAPCost;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedSingleAPCost - iSingleAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedSingleAPCostModifier - iSingleAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalSingleAPCost - iFinalSingleAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}
		}
		else if( fComparisonMode && !Weapon[gpComparedItemDescObject->usItem].NoSemiAuto )
		{
			// Set line to draw into
			ubNumLine = 14;
			// Get final Attack Cost
			INT16 iFinalSingleAPCost = (INT16)(BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier ) * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpItemDescObject, WM_NORMAL ) + 0.5);
			// Get base Attack Cost
			INT16 iSingleAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject );
			// Get Attack Cost Modifier
			INT16 iSingleAPCostModifier = iFinalSingleAPCost - iSingleAPCost;
			// Print base value
			DrawPropertyValueInColour( iSingleAPCost, ubNumLine, 1, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iSingleAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalSingleAPCost, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
		}

		/////////////////// BURST AP
		if ( GetShotsPerBurst(gpItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 15;

			// Get final Burst Cost
			INT16 iFinalBurstAPCost = (INT16)( (FLOAT)ubAttackAPs * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpItemDescObject, WM_BURST ) + CalcAPsToBurst( APBPConstants[DEFAULT_APS], gpItemDescObject, gpItemDescSoldier ) + 0.5f );

			// Get base Burst Cost
			INT16 iBurstAPCost = ubBasicAttackAPs + CalcAPsToBurstNoModifier( APBPConstants[DEFAULT_APS], gpItemDescObject );

			// Get Burst Cost Modifier
			INT16 iBurstAPCostModifier = iFinalBurstAPCost - iBurstAPCost;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iBurstAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iBurstAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalBurstAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if( GetShotsPerBurst(gpComparedItemDescObject) > 0 )
			{
				// Get final Burst Cost
				INT16 iComparedFinalBurstAPCost = (INT16)( (FLOAT)BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier )
					 * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpComparedItemDescObject, WM_BURST )
					 + CalcAPsToBurst( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, gpItemDescSoldier ) + 0.5f );
				// Get base Burst Cost
				INT16 iComparedBurstAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject )
					+ CalcAPsToBurstNoModifier( APBPConstants[DEFAULT_APS], gpComparedItemDescObject );
				// Get Burst Cost Modifier
				INT16 iComparedBurstAPCostModifier = iComparedFinalBurstAPCost - iComparedBurstAPCost;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedBurstAPCost - iBurstAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedBurstAPCostModifier - iBurstAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalBurstAPCost - iFinalBurstAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}
		}
		else if( fComparisonMode && GetShotsPerBurst(gpComparedItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 15;
			INT16 iFinalBurstAPCost = (INT16)( (FLOAT)BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier )
				 * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpComparedItemDescObject, WM_BURST )
				 + CalcAPsToBurst( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, gpItemDescSoldier ) + 0.5f );
			// Get base Burst Cost
			INT16 iBurstAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject )
				+ CalcAPsToBurstNoModifier( APBPConstants[DEFAULT_APS], gpComparedItemDescObject );
			// Get Burst Cost Modifier
			INT16 iBurstAPCostModifier = iFinalBurstAPCost - iBurstAPCost;
			// Print base value
			DrawPropertyValueInColour( iBurstAPCost, ubNumLine, 1, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iBurstAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalBurstAPCost, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
		}
		
		////////////////// AUTO AP
		if ( GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 16;

			// Get final Autofire Cost
			INT16 iFinalAutoAPCost = (INT16)( (FLOAT)ubAttackAPs * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpItemDescObject, WM_AUTOFIRE ) + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], gpItemDescObject, 3, gpItemDescSoldier ) + 0.5f );

			// Get base Autofire Cost
			INT16 iAutoAPCost = ubBasicAttackAPs + CalcAPsToAutofireNoModifier( APBPConstants[DEFAULT_APS], gpItemDescObject, 3 );

			// Get Autofire Cost Modifier
			INT16 iAutoAPCostModifier = iFinalAutoAPCost - iAutoAPCost;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iAutoAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iAutoAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalAutoAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if( GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 )
			{
				// Get final Autofire Cost
				INT16 iComparedFinalAutoAPCost = (INT16)( (FLOAT)BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier )
					 * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpComparedItemDescObject, WM_AUTOFIRE )
					 + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, 3, gpItemDescSoldier ) + 0.5f );
				// Get base Autofire Cost
				INT16 iComparedAutoAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject )
					+ CalcAPsToAutofireNoModifier( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, 3 );
				// Get Autofire Cost Modifier
				INT16 iComparedAutoAPCostModifier = iComparedFinalAutoAPCost - iComparedAutoAPCost;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedAutoAPCost - iAutoAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedAutoAPCostModifier - iAutoAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalAutoAPCost - iFinalAutoAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}
		}
		else if( fComparisonMode && GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 16;
			// Get final Autofire Cost
			INT16 iFinalAutoAPCost = (INT16)( (FLOAT)BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject, gpItemDescSoldier )
				 * GetAttackAPTraitMultiplier( gpItemDescSoldier, gpComparedItemDescObject, WM_AUTOFIRE )
				 + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, 3, gpItemDescSoldier ) + 0.5f );
			// Get base Autofire Cost
			INT16 iAutoAPCost = BaseAPsToShootOrStabNoModifier( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpComparedItemDescObject )
				+ CalcAPsToAutofireNoModifier( APBPConstants[DEFAULT_APS], gpComparedItemDescObject, 3 );
			// Get Autofire Cost Modifier
			INT16 iAutoAPCostModifier = iFinalAutoAPCost - iAutoAPCost;
			// Print base value
			DrawPropertyValueInColour( iAutoAPCost, ubNumLine, 1, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyValueInColour( iAutoAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalAutoAPCost, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
		}

		///////////////////// RELOAD AP
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
		{
			// Set line to draw into
			ubNumLine = 17;

			// Get final Reload Cost
			INT16 iFinalReloadAPCost = GetAPsToReload( gpItemDescObject );
			if( gGameOptions.fNewTraitSystem && !(Item[ gpItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
			{
				// pistols and revolvers
				if( Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_PISTOL || Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_M_PISTOL )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost 
										* max(0,( 100 - gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * NUM_SKILL_TRAITS( gpItemDescSoldier, GUNSLINGER_NT )
										- gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0f + 0.5f );
				// shotguns
				else if( Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN && Weapon[gpItemDescObject->usItem].swapClips != 1 )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost 
										* max(0,(100 - gSkillTraitValues.ubRAReloadSpeedShotgunsManual * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ) 
										- gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0f + 0.5f );
				// ambidextrous reload bullets
				else if( Weapon[gpItemDescObject->usItem].swapClips != 1 )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
				// ambidextrous reload magazines
				else
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
			}

			// Get base Reload Cost
			INT16 iReloadAPCost = Weapon[ gpItemDescObject->usItem ].APsToReload;
			// modify by ini values
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN )
				iReloadAPCost *= gItemSettings.fAPtoReloadModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iReloadAPCost *= gItemSettings.fAPtoReloadModifierLauncher;

			// Get Reload Cost Modifier
			INT16 iReloadAPCostModifier = iFinalReloadAPCost - iReloadAPCost;
	
			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iReloadAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iReloadAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalReloadAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher )
			{
				// Get final Reload Cost
				INT16 iComparedFinalReloadAPCost = GetAPsToReload( gpComparedItemDescObject );

				if( gGameOptions.fNewTraitSystem && !(Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
				{
					// pistols and revolvers
					if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_PISTOL || Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_M_PISTOL )
						iComparedFinalReloadAPCost = (INT16)( (FLOAT)iComparedFinalReloadAPCost 
											* max(0,( 100 - gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * NUM_SKILL_TRAITS( gpItemDescSoldier, GUNSLINGER_NT )
											- gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0f + 0.5f );
					// shotguns
					else if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN && Weapon[gpComparedItemDescObject->usItem].swapClips != 1 )
						iComparedFinalReloadAPCost = (INT16)( (FLOAT)iComparedFinalReloadAPCost 
											* max(0,(100 - gSkillTraitValues.ubRAReloadSpeedShotgunsManual * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ) 
											- gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0f + 0.5f );
					// ambidextrous reload bullets
					else if( Weapon[gpComparedItemDescObject->usItem].swapClips != 1 )
						iComparedFinalReloadAPCost = (INT16)( (FLOAT)iComparedFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
					// ambidextrous reload magazines
					else
						iComparedFinalReloadAPCost = (INT16)( (FLOAT)iComparedFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
				}

				// Get base Reload Cost
				INT16 iComparedReloadAPCost = Weapon[ gpComparedItemDescObject->usItem ].APsToReload;
				// modify by ini values
				if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
					iComparedReloadAPCost *= gItemSettings.fAPtoReloadModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
				else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
					iComparedReloadAPCost *= gItemSettings.fAPtoReloadModifierLauncher;
				// Get Reload Cost Modifier
				INT16 iComparedReloadAPCostModifier = iComparedFinalReloadAPCost - iComparedReloadAPCost;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedReloadAPCost - iReloadAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedReloadAPCostModifier - iReloadAPCostModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalReloadAPCost - iFinalReloadAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTPOSITIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTPOSITIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTPOSITIVE );
			}
		}
		else if( fComparisonMode && ( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher ) )
		{
			ubNumLine = 17;
			// Get final Reload Cost
			INT16 iFinalReloadAPCost = GetAPsToReload( gpComparedItemDescObject );

			if( gGameOptions.fNewTraitSystem && !(Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
			{
				// pistols and revolvers
				if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_PISTOL || Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_M_PISTOL )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost 
										* max(0,( 100 - gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * NUM_SKILL_TRAITS( gpItemDescSoldier, GUNSLINGER_NT )
										- gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0f + 0.5f );
				// shotguns
				else if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN && Weapon[gpComparedItemDescObject->usItem].swapClips != 1 )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost 
										* max(0,(100 - gSkillTraitValues.ubRAReloadSpeedShotgunsManual * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ) 
										- gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT ) ) ) / 100.0 + 0.5f );
				// ambidextrous reload bullets
				else if( Weapon[gpComparedItemDescObject->usItem].swapClips != 1 )
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedLoose * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
				// ambidextrous reload magazines
				else
					iFinalReloadAPCost = (INT16)( (FLOAT)iFinalReloadAPCost * max( 0,(100 - gSkillTraitValues.ubAMReloadSpeedMagazines * NUM_SKILL_TRAITS( gpItemDescSoldier, AMBIDEXTROUS_NT )) ) / 100.0f + 0.5f);
			}

				// Get base Reload Cost
			INT16 iReloadAPCost = Weapon[ gpComparedItemDescObject->usItem ].APsToReload;
			// modify by ini values
			if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
				iReloadAPCost *= gItemSettings.fAPtoReloadModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iReloadAPCost *= gItemSettings.fAPtoReloadModifierLauncher;
			// Get Reload Cost Modifier
			INT16 iReloadAPCostModifier = iFinalReloadAPCost - iReloadAPCost;
			// Print base value
			DrawPropertyValueInColour( iReloadAPCost, ubNumLine, 1, FALSE, FALSE, FALSE,ITEMDESC_FONTNEGATIVE );
			// Print modifier
			DrawPropertyValueInColour( iReloadAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalReloadAPCost, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
		}

		///////////////////// MANUAL RELOAD AP
		if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher 
			&& Weapon[gpItemDescObject->usItem].APsToReloadManually > 0 )
		{
			// Set line to draw into
			ubNumLine = 18;

			// Get final Manual Reload Cost
			INT16 iFinalManualReloadAPCost = Weapon[ gpItemDescObject->usItem ].APsToReloadManually;
			// modify by ini values
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN )
				iFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierLauncher;

			// Get base Manual Reload Cost
			INT16 iManualReloadAPCost = iFinalManualReloadAPCost;

			// trait bonuses
			if( gGameOptions.fNewTraitSystem && !(Item[ gpItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
			{
				// rifles and sniper rifles
				if( Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_SN_RIFLE || Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_RIFLE )
					iFinalManualReloadAPCost = (INT16)( (FLOAT)iFinalManualReloadAPCost * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, SNIPER_NT )) / 100.0f + 0.5 );
				// shotguns
				else if( Weapon[ gpItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN )
					iFinalManualReloadAPCost = (INT16)( (FLOAT)iFinalManualReloadAPCost * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ))  /100.0f + 0.5);
			}

			// Get manual reload cost modifier
			INT16 iManualReloadAPCostModifier = iFinalManualReloadAPCost - iManualReloadAPCost;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iManualReloadAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iManualReloadAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalManualReloadAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher 
					&& Weapon[gpComparedItemDescObject->usItem].APsToReloadManually > 0  )
			{
				// Get final Manual Reload Cost
				INT16 iComparedFinalManualReloadAPCost = Weapon[ gpComparedItemDescObject->usItem ].APsToReloadManually;
				// modify by ini values
				if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
					iComparedFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
				else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
					iComparedFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierLauncher;

				// Get base Manual Reload Cost
				INT16 iComparedManualReloadAPCost = iComparedFinalManualReloadAPCost;

				// trait bonuses
				if( gGameOptions.fNewTraitSystem && !(Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
				{
					// rifles and sniper rifles
					if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SN_RIFLE || Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_RIFLE )
						iComparedFinalManualReloadAPCost = (INT16)( (FLOAT)iComparedFinalManualReloadAPCost * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, SNIPER_NT )) / 100.0f + 0.5 );
					// shotguns
					else if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN )
						iComparedFinalManualReloadAPCost = (INT16)( (FLOAT)iComparedFinalManualReloadAPCost * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ))  /100.0f + 0.5);
				}

				// Get manual reload cost modifier
				INT16 iComparedManualReloadAPCostModifier = iComparedFinalManualReloadAPCost - iComparedManualReloadAPCost;

				// Print difference in base value
				DrawPropertyValueInColour( iComparedManualReloadAPCost - iManualReloadAPCost, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iComparedManualReloadAPCostModifier - iManualReloadAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalManualReloadAPCost - iFinalManualReloadAPCost, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTPOSITIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTPOSITIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTPOSITIVE );
			}
		}
		else if( fComparisonMode && ( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher 
					&& Weapon[gpComparedItemDescObject->usItem].APsToReloadManually > 0 ) )
		{
			ubNumLine = 18;
			// Get final Manual Reload Cost
			INT16 iFinalManualReloadAPCost = Weapon[ gpComparedItemDescObject->usItem ].APsToReloadManually;
			// modify by ini values
			if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
				iFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_LAUNCHER )
				iFinalManualReloadAPCost *= gItemSettings.fAPtoReloadManuallyModifierLauncher;

			// Get base Manual Reload Cost
			INT16 iManualReloadAPCost = iFinalManualReloadAPCost;

			// trait bonuses
			if( gGameOptions.fNewTraitSystem && !(Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LAUNCHER) )
			{
				// rifles and sniper rifles
				if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SN_RIFLE || Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_RIFLE )
					iFinalManualReloadAPCost = (INT16)( (FLOAT)iFinalManualReloadAPCost * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, SNIPER_NT )) / 100.0f + 0.5 );
				// shotguns
				else if( Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType == GUN_SHOTGUN )
					iFinalManualReloadAPCost = (INT16)( (FLOAT)iFinalManualReloadAPCost * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( gpItemDescSoldier, RANGER_NT ))  /100.0f + 0.5);
			}

			// Get manual reload cost modifier
			INT16 iManualReloadAPCostModifier = iFinalManualReloadAPCost - iManualReloadAPCost;

			// Print base value
			DrawPropertyValueInColour( iManualReloadAPCost, ubNumLine, 1, FALSE, FALSE, FALSE,ITEMDESC_FONTNEGATIVE );
			// Print modifier
			DrawPropertyValueInColour( iManualReloadAPCostModifier, ubNumLine, 2, FALSE, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalManualReloadAPCost, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
		}

		///////////////////// RECOIL X/Y
		if ( UsingNewCTHSystem() == true )
		{
			if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher
				&& GetShotsPerBurst(gpItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpItemDescObject) )
			{
				// HEADROCK HAM 5: One value to rule them all.
				// Set line to draw into
				ubNumLine = 20;				

				FLOAT iFinalRecoilX = 0;
				FLOAT iFinalRecoilY = 0;

				// Get final Recoil
				GetRecoil( gpItemDescSoldier, gpItemDescObject, &iFinalRecoilX, &iFinalRecoilY, 3 );

				// Get base Recoil
				FLOAT iRecoilX = Weapon[ gpItemDescObject->usItem ].bRecoilX;
				FLOAT iRecoilY = Weapon[ gpItemDescObject->usItem ].bRecoilY;

				if (iRecoilX == -127) { iRecoilX = 0; } // -127 means "invalid". These guns don't actually have any recoil parameters.
				if (iRecoilY == -127) { iRecoilY = 0; }

				// modify by ini values
				if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN )
				{
					iRecoilX *= gItemSettings.fRecoilXModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
					iRecoilY *= gItemSettings.fRecoilYModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
				}

				FLOAT dBaseRecoil = sqrt( ((iRecoilX * iRecoilX)+(iRecoilY * iRecoilY)) );
				FLOAT dFinalRecoil = sqrt( ((iFinalRecoilX * iFinalRecoilX) + (iFinalRecoilY * iFinalRecoilY)) );
				FLOAT dRecoilModifier = dFinalRecoil - dBaseRecoil;

				if( !fComparisonMode )
				{
					// Print base value
					DrawPropertyValueInColourFloat( dBaseRecoil, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 5, 1.0f );
					// Print modifier
					DrawPropertyValueInColourFloat( dRecoilModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print final value
					DrawPropertyValueInColourFloat( dFinalRecoil, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE, 1.0f );
				}
				else if( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
						&& GetShotsPerBurst(gpComparedItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) )
				{
					FLOAT iComparedFinalRecoilX = 0;
					FLOAT iComparedFinalRecoilY = 0;

					// Get final Recoil
					GetRecoil( gpItemDescSoldier, gpComparedItemDescObject, &iComparedFinalRecoilX, &iComparedFinalRecoilY, 3 );

					// Get base Recoil
					FLOAT iComparedRecoilX = Weapon[ gpComparedItemDescObject->usItem ].bRecoilX;
					FLOAT iComparedRecoilY = Weapon[ gpComparedItemDescObject->usItem ].bRecoilY;

					if (iComparedRecoilX == -127) { iComparedRecoilX = 0; } // -127 means "invalid". These guns don't actually have any recoil parameters.
					if (iComparedRecoilY == -127) { iComparedRecoilY = 0; }

					// modify by ini values
					if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
					{
						iComparedRecoilX *= gItemSettings.fRecoilXModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
						iComparedRecoilY *= gItemSettings.fRecoilYModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					}

					FLOAT dComparedBaseRecoil = sqrt( ((iComparedRecoilX * iComparedRecoilX)+(iComparedRecoilY * iComparedRecoilY)) );
					FLOAT dComparedFinalRecoil = sqrt( ((iComparedFinalRecoilX * iComparedFinalRecoilX) + (iComparedFinalRecoilY * iComparedFinalRecoilY)) );
					FLOAT dComparedRecoilModifier = dComparedFinalRecoil - dComparedBaseRecoil;

					// Print difference in base value
					DrawPropertyValueInColourFloat( dComparedBaseRecoil - dBaseRecoil, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print difference in modifier
					DrawPropertyValueInColourFloat( dComparedRecoilModifier - dRecoilModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print difference in final value
					DrawPropertyValueInColourFloat( dComparedFinalRecoil - dFinalRecoil, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
				}
				else
				{
					DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
					DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
					DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
				}
				
				/*
				// RECOIL Y

				// Set line to draw into
				ubNumLine = 20;
				// Set Y coordinates
				sTop = gItemDescGenRegions[ubNumLine][1].sTop;
				sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

				// Print base value
				SetFontForeground( 5 );
				sLeft = gItemDescGenRegions[ubNumLine][1].sLeft;
				sWidth = gItemDescGenRegions[ubNumLine][1].sRight - sLeft;
				swprintf( pStr, L"%d", iRecoilY );
				FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );

				// Print modifier
				SetFontForeground( 5 );
				if ( (iRecoilYModifier < 0 && iRecoilY > 0) || (iRecoilYModifier > 0 && iRecoilY < 0) )
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
				}
				else if ( (iRecoilYModifier > 0 && iRecoilY > 0) || (iRecoilYModifier < 0 && iRecoilY < 0) )
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
				}
				// Add positive/negative sign
				if ( iRecoilYModifier > 0 )
				{
					swprintf( pStr, L"+%d", iRecoilYModifier );
				}
				else if ( iRecoilYModifier < 0 )
				{
					swprintf( pStr, L"%d", iRecoilYModifier );
				}
				else
				{
					swprintf( pStr, L"--" );
				}
				sLeft = gItemDescGenRegions[ubNumLine][2].sLeft;
				sWidth = gItemDescGenRegions[ubNumLine][2].sRight - sLeft;
				FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );

				// Print final value
				SetFontForeground( FONT_MCOLOR_WHITE );
				sLeft = gItemDescGenRegions[ubNumLine][3].sLeft;
				sWidth = gItemDescGenRegions[ubNumLine][3].sRight - sLeft;
				swprintf( pStr, L"%d", iFinalRecoilY );
				FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );

				// Reset font color
				SetFontForeground( 6 );
				*/
			}
			else if( fComparisonMode && ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
				&& GetShotsPerBurst(gpComparedItemDescObject)> 0 || GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) ) )
			{

				ubNumLine = 20;	
						
				FLOAT iFinalRecoilX = 0;
				FLOAT iFinalRecoilY = 0;

				// Get final Recoil
				GetRecoil( gpItemDescSoldier, gpComparedItemDescObject, &iFinalRecoilX, &iFinalRecoilY, 3 );

				// Get base Recoil
				FLOAT iRecoilX = Weapon[ gpComparedItemDescObject->usItem ].bRecoilX;
				FLOAT iRecoilY = Weapon[ gpComparedItemDescObject->usItem ].bRecoilY;

				if (iRecoilX == -127) { iRecoilX = 0; } // -127 means "invalid". These guns don't actually have any recoil parameters.
				if (iRecoilY == -127) { iRecoilY = 0; }

				// modify by ini values
				if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN )
				{
					iRecoilX *= gItemSettings.fRecoilXModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					iRecoilY *= gItemSettings.fRecoilYModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
				}

				FLOAT dBaseRecoil = sqrt( ((iRecoilX * iRecoilX)+(iRecoilY * iRecoilY)) );
				FLOAT dFinalRecoil = sqrt( ((iFinalRecoilX * iFinalRecoilX) + (iFinalRecoilY * iFinalRecoilY)) );
				FLOAT dRecoilModifier = dFinalRecoil - dBaseRecoil;
				// Print base value
				DrawPropertyValueInColourFloat( dBaseRecoil, ubNumLine, 1, FALSE, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
				// Print modifier
				DrawPropertyValueInColourFloat( dRecoilModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColourFloat( dFinalRecoil, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTNEGATIVE );
			}
		}
		else	///////////////// BIPOD & BURST PENALTY
		{
			if( GetBurstPenalty(gpItemDescObject) > 0 )
			{
				// Set line to draw into
				ubNumLine = 19;

				// Get base Burst Penalty value
				INT16 iBurstValue = Weapon[gpItemDescObject->usItem].ubBurstPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);

				// Get final Burst Penalty value
				INT16 iFinalBurstValue;
				if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
				{
					iFinalBurstValue = GetBurstToHitBonus(gpItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
					iFinalBurstValue = max(0, (iBurstValue * (100 - iFinalBurstValue))/100 );
				}
				else
				{
					iFinalBurstValue = GetBurstPenalty(gpItemDescObject);
					if(gGameExternalOptions.bAimedBurstEnabled)
						iFinalBurstValue += Weapon[gpItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
				}

				// Get Burst Penalty modifier
				INT16 iBurstModifier = iFinalBurstValue - iBurstValue;

				if( !fComparisonMode )
				{
					// Print base value
					DrawPropertyValueInColour( iBurstValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print modifier
					DrawPropertyValueInColour( iBurstModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print final value
					DrawPropertyValueInColour( iFinalBurstValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
				}
				else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) && !Item[ gpComparedItemDescObject->usItem ].singleshotrocketlauncher )
				{
					// Get base Burst Penalty value
					INT16 iComparedBurstValue = Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
					// Get final Burst Penalty value
					INT16 iComparedFinalBurstValue;
					if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
					{
						iComparedFinalBurstValue = GetBurstToHitBonus(gpComparedItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
						iComparedFinalBurstValue = max(0, (iComparedBurstValue * (100 - iComparedFinalBurstValue))/100 );
					}
					else
					{
						iComparedFinalBurstValue = GetBurstPenalty(gpComparedItemDescObject);
						if(gGameExternalOptions.bAimedBurstEnabled)
							iComparedFinalBurstValue += Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
					}
					// Get Burst Penalty modifier
					INT16 iComparedBurstModifier = iComparedFinalBurstValue - iComparedBurstValue;
					// Print difference in base value
					DrawPropertyValueInColour( iComparedBurstValue - iBurstValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print difference in modifier
					DrawPropertyValueInColour( iComparedBurstModifier - iBurstModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalBurstValue - iFinalBurstValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
				}
			}
			else if( fComparisonMode && GetBurstPenalty(gpComparedItemDescObject) > 0 )
			{
				ubNumLine = 19;	
				// Get base Burst Penalty value
				INT16 iBurstValue = Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
				// Get final Burst Penalty value
				INT16 iFinalBurstValue;
				if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
				{
					iFinalBurstValue = GetBurstToHitBonus(gpComparedItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
					iFinalBurstValue = max(0, (iBurstValue * (100 - iFinalBurstValue))/100 );
				}
				else
				{
					iFinalBurstValue = GetBurstPenalty(gpComparedItemDescObject);
					if(gGameExternalOptions.bAimedBurstEnabled)
						iFinalBurstValue += Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
				}
				// Get Burst Penalty modifier
				INT16 iBurstModifier = iFinalBurstValue - iBurstValue;
				// Print base value
				DrawPropertyValueInColour( iBurstValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iBurstModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalBurstValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );

			}
			if( GetBipodBonus(gpItemDescObject) > 0)
			{
				// Set line to draw into
				ubNumLine = 20;

				// Get base Reliability value
				INT16 iBipodValue = Item[gpItemDescObject->usItem].bipod;

				// Get final Reliability value
				INT16 iFinalBipodValue = GetBipodBonus(gpItemDescObject);

				// Get Reliability modifier
				INT16 iBipodModifier = iFinalBipodValue - iBipodValue;

				if( !fComparisonMode )
				{
					// Print base value
					DrawPropertyValueInColour( iBipodValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print modifier
					DrawPropertyValueInColour( iBipodModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					// Print final value
					DrawPropertyValueInColour( iFinalBipodValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
				else
				{
					// Get base Reliability value
					INT16 iComparedBipodValue = Item[gpComparedItemDescObject->usItem].bipod;
					// Get final Reliability value
					INT16 iComparedFinalBipodValue = GetBipodBonus(gpComparedItemDescObject);
					// Get Reliability modifier
					INT16 iComparedBipodModifier = iComparedFinalBipodValue - iComparedBipodValue;
					// Print difference in base value
					DrawPropertyValueInColour( iComparedBipodValue - iBipodValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print difference in modifier
					DrawPropertyValueInColour( iComparedBipodModifier - iBipodModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalBipodValue - iFinalBipodValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
				}
			}
			else if( fComparisonMode && GetBipodBonus(gpComparedItemDescObject) > 0)
			{
				ubNumLine = 20;	
				// Get base Reliability value
				INT16 iBipodValue = Item[gpComparedItemDescObject->usItem].bipod;
				// Get final Reliability value
				INT16 iFinalBipodValue = GetBipodBonus(gpComparedItemDescObject);
				// Get Reliability modifier
				INT16 iBipodModifier = iFinalBipodValue - iBipodValue;
				// Print base value
				DrawPropertyValueInColour( iBipodValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iBipodModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalBipodValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		/////////////////// AUTOFIRE BULLETS PER 5 AP
		if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpItemDescObject->usItem].rocketlauncher
			&& GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 21;

			// Get final B/5AP
			INT16 iFinalB5AP = GetAutofireShotsPerFiveAPs( gpItemDescObject );

			// Get base B/5AP Cost
			INT16 iB5AP = iFinalB5AP;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iB5AP, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalB5AP, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
					&& GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 )
			{
				// Get final B/5AP
				INT16 iComparedFinalB5AP = GetAutofireShotsPerFiveAPs( gpComparedItemDescObject );
				// Get base B/5AP Cost
				INT16 iComparedB5AP = iComparedFinalB5AP;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedB5AP - iB5AP, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalB5AP - iFinalB5AP, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}
		}
		else if( fComparisonMode && ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
			&& GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 ) )
		{
			ubNumLine = 21;	
			// Get final B/5AP
			INT16 iFinalB5AP = GetAutofireShotsPerFiveAPs( gpComparedItemDescObject );
			// Get base B/5AP Cost
			INT16 iB5AP = iFinalB5AP;
			// Print base value
			DrawPropertyValueInColour( iB5AP, ubNumLine, 1, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalB5AP, ubNumLine, 3, fComparisonMode, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );		
		}

		/////////////////// AUTOFIRE PENALTY
		if( UsingNewCTHSystem() == false && GetAutoPenalty(gpItemDescObject) > 0 )
		{
			// Set line to draw into
			ubNumLine = 22;

			// Get base Auto Penalty value
			INT16 iAutoValue = Weapon[gpItemDescObject->usItem].AutoPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);

			// Get final Auto Penalty value
			INT16 iFinalAutoValue;
			if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
			{
				iFinalAutoValue = GetAutoToHitBonus(gpItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
				iFinalAutoValue = max(0, (iAutoValue * (100 - iFinalAutoValue))/100 );
			}
			else
			{
				iFinalAutoValue = GetAutoPenalty(gpItemDescObject);
				if(gGameExternalOptions.bAimedBurstEnabled)
					iFinalAutoValue += Weapon[gpItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
			}

			// Get Auto Penalty modifier
			INT16 iAutoModifier = iFinalAutoValue - iAutoValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iAutoValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyValueInColour( iAutoModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print final value
				DrawPropertyValueInColour( iFinalAutoValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else if ( Item[ gpComparedItemDescObject->usItem ].usItemClass == IC_GUN && !Item[ gpComparedItemDescObject->usItem].rocketlauncher
					&& GetAutofireShotsPerFiveAPs(gpComparedItemDescObject) > 0 )
			{
				// Get base Auto Penalty value
				INT16 iComparedAutoValue = Weapon[gpComparedItemDescObject->usItem].AutoPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
				// Get final Auto Penalty value
				INT16 iComparedFinalAutoValue;
				if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
				{
					iComparedFinalAutoValue = GetAutoToHitBonus(gpComparedItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
					iComparedFinalAutoValue = max(0, (iComparedAutoValue * (100 - iComparedFinalAutoValue))/100 );
				}
				else
				{
					iComparedFinalAutoValue = GetAutoPenalty(gpComparedItemDescObject);
					if(gGameExternalOptions.bAimedBurstEnabled)
						iComparedFinalAutoValue += Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
				}
				// Get Auto Penalty modifier
				INT16 iComparedAutoModifier = iComparedFinalAutoValue - iComparedAutoValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedAutoValue - iAutoValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedAutoModifier - iAutoModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalAutoValue - iFinalAutoValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
			else
			{
				DrawPropertyTextInColour( L"-", ubNumLine, 1, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 2, ITEMDESC_FONTNEGATIVE );
				DrawPropertyTextInColour( L"-", ubNumLine, 3, ITEMDESC_FONTNEGATIVE );
			}

		}
		else if( fComparisonMode && UsingNewCTHSystem() == false && GetAutoPenalty(gpComparedItemDescObject) > 0 )
		{
			ubNumLine = 22;	
			// Get base Auto Penalty value
			INT16 iAutoValue = Weapon[gpComparedItemDescObject->usItem].AutoPenalty * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
			// Get final Auto Penalty value
			INT16 iFinalAutoValue;
			if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
			{
				iFinalAutoValue = GetAutoToHitBonus(gpComparedItemDescObject) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
				iFinalAutoValue = max(0, (iAutoValue * (100 - iFinalAutoValue))/100 );
			}
			else
			{
				iFinalAutoValue = GetAutoPenalty(gpComparedItemDescObject);
				if(gGameExternalOptions.bAimedBurstEnabled)
					iFinalAutoValue += Weapon[gpComparedItemDescObject->usItem].ubBurstPenalty * gGameExternalOptions.uAimedBurstPenalty;
			}
			// Get Auto Penalty modifier
			INT16 iAutoModifier = iFinalAutoValue - iAutoValue;
			// Print base value
			DrawPropertyValueInColour( iAutoValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
			// Print modifier
			DrawPropertyValueInColour( iAutoModifier, ubNumLine, 2, fComparisonMode, TRUE, FALSE );
			// Print final value
			DrawPropertyValueInColour( iFinalAutoValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
		}
	}
	if (gubDescBoxPage == 2)
	{
		DrawAdvancedValues( gpItemDescObject );
	}
}

void DrawAmmoValues( OBJECTTYPE * gpItemDescObject, int shotsLeft )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				usX;
	UINT8				ubNumLine;
	INT16				sTop, sHeight;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 0)
	{
		////////////////////////// AMMO REMAINING
		SetFontForeground( 6 );
		swprintf( pStr, gWeaponStatsDesc[16] );
		mprintf( gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sTop, pStr );

		SetFontForeground( 5 );
		swprintf( pStr, L"%d/%d", (*gpItemDescObject)[ gubItemDescStatusIndex ]->data.ubShotsLeft, Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubMagSize );
		FindFontRightCoordinates( gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sTop, gItemDescTextRegions[1].sRight - gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sBottom - gItemDescTextRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, pStr );
	}
	else if (gubDescBoxPage == 1)
	{

		// anv: if ctrl is pressed in map inventory, show comparison with selected ammo
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_AMMO )
					fComparisonMode = TRUE;
			}
		}

		////////////////////////////////////////////////// HEADERS

		SetFontForeground( FONT_MCOLOR_WHITE );

		// "PRIMARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 0 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sTop, gItemDescGenHeaderRegions[0].sRight - gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sBottom - gItemDescGenHeaderRegions[0].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "SECONDARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 1 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sTop, gItemDescGenHeaderRegions[1].sRight - gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sBottom - gItemDescGenHeaderRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		SetFontForeground( 6 );

		///////////////////////////////////////////////////// INDEXES

		SetFontForeground( FONT_MCOLOR_WHITE );

		for (UINT8 cnt = 0; cnt < 4; cnt++)
		{
			// Select "PROPERTY", "0", "+" or "=" as appropriate.
			swprintf(pStr, L"%s", gzItemDescGenIndexes[ cnt ]);

			FindFontCenterCoordinates( gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sTop, gItemDescGenIndexRegions[0][cnt].sRight - gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sBottom - gItemDescGenIndexRegions[0][cnt].sTop, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}

		SetFontForeground( 6 );

		///////////////// ARMOR PIERCING
		{
			// Set line to draw into
			ubNumLine = 0;

			// Get final Penetration
			FLOAT fArmourImpactReduction = ((FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionDivisor);

			// Get base Penetration
			FLOAT fFinalArmourImpactReduction = fArmourImpactReduction;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColourFloat( fArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 1.0f, 2 );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( fFinalArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE, 1.0f, 2 );
			}
			else
			{
				// Get final Penetration
				FLOAT fComparedArmourImpactReduction = ((FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].armourImpactReductionDivisor);
				// Get base Penetration
				FLOAT fComparedFinalArmourImpactReduction = fComparedArmourImpactReduction;
				// Print difference in base value
				DrawPropertyValueInColourFloat( fComparedArmourImpactReduction - fArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColourFloat( fComparedFinalArmourImpactReduction - fFinalArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
			}
		}

		///////////////////// TUMBLING
		{
			// Set line to draw into
			ubNumLine = 1;

			// Get final Tumbling
			FLOAT fAfterArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageDivisor;

			// Get base Tumbling
			FLOAT fFinalAfterArmourImpactReduction = fAfterArmourImpactReduction;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColourFloat( fAfterArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 0, 1.0f, 2 );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( fFinalAfterArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE, 1.0f, 2 );
			}
			else
			{
				// Get final Tumbling
				FLOAT fComparedAfterArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].afterArmourDamageDivisor;
				// Get base Tumbling
				FLOAT fComparedFinalAfterArmourImpactReduction = fComparedAfterArmourImpactReduction;
				// Print difference in base value
				DrawPropertyValueInColourFloat( fComparedAfterArmourImpactReduction - fAfterArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 0, 0.0f, 2 );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColourFloat( fComparedFinalAfterArmourImpactReduction - fFinalAfterArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, TRUE, 0, 0.0f, 2 );
			}
		}

		///////////////////// PRE-ARMOR EXPLOSION
		{
			// Set line to draw into
			ubNumLine = 2;

			// Get final Explosion
			FLOAT fPreArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageDivisor;

			// Get base Explosion
			FLOAT fFinalPreArmourImpactReduction = fPreArmourImpactReduction;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColourFloat( fPreArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 0, 1.0f, 2 );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( fFinalPreArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE, 1.0f, 2 );
			}
			else
			{
				// Get final Explosion
				FLOAT fComparedPreArmourImpactReduction = (FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageMultiplier / (FLOAT) AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].beforeArmourDamageDivisor;
				// Get base Explosion
				FLOAT fComparedFinalPreArmourImpactReduction = fComparedPreArmourImpactReduction;
				// Print difference in base value
				DrawPropertyValueInColourFloat( fComparedPreArmourImpactReduction - fPreArmourImpactReduction, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColourFloat( fComparedFinalPreArmourImpactReduction - fFinalPreArmourImpactReduction, ubNumLine, 3, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
			}
		}

		// Flugente
		if ( gGameExternalOptions.fWeaponOverheating )
		{
			///////////////////// TEMPERATURE MODIFICATOR
			// Set line to draw into
			ubNumLine = 3;
			// Set Y coordinates
			sTop = gItemDescGenRegions[ubNumLine][1].sTop;
			sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

			// base modificator
			FLOAT basevalue = AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].temperatureModificator;
			FLOAT modificator = 0;							// Does not exist (yet?)
			FLOAT finalvalue = basevalue - modificator;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColourFloat( basevalue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( finalvalue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE, 0.0f, 2 );
			}
			else
			{
				// base modificator
				FLOAT Comparedbasevalue = AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].temperatureModificator;
				FLOAT Comparedmodificator = 0;							// Does not exist (yet?)
				FLOAT Comparedfinalvalue = Comparedbasevalue - Comparedmodificator;
				// Print difference in base value
				DrawPropertyValueInColourFloat( Comparedbasevalue - basevalue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColourFloat( Comparedfinalvalue - finalvalue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2  );
			}
		}
		
		///////////////////// DIRT MODIFICATOR
		{
			// Set line to draw into
			ubNumLine = 5;
			// Set Y coordinates
			sTop = gItemDescGenRegions[ubNumLine][1].sTop;
			sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

			// base modificator
			FLOAT basevalue = AmmoTypes[Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex].ubAmmoType].dirtModificator;
			FLOAT modificator = 0;							// Does not exist (yet?)
			FLOAT finalvalue = basevalue - modificator;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColourFloat( basevalue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColourFloat( finalvalue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE, 0.0f, 2 );
			}
			else
			{
				// base modificator
				FLOAT Comparedbasevalue = AmmoTypes[Magazine[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex].ubAmmoType].dirtModificator;
				FLOAT Comparedmodificator = 0;							// Does not exist (yet?)
				FLOAT Comparedfinalvalue = Comparedbasevalue - Comparedmodificator;
				// Print difference in base value
				DrawPropertyValueInColourFloat( Comparedbasevalue - basevalue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2 );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColourFloat( Comparedfinalvalue - finalvalue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, 0, 0.0f, 2  );
			}
		}
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedValues( gpItemDescObject );
	}
}

void DrawExplosiveValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				usX;
	UINT8				ubNumLine;
	INT16				sTop, sHeight;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	if (gubDescBoxPage == 1)
	{
		// anv: if ctrl is pressed in map inventory, show comparison with selected explosives
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
				{
					fComparisonMode = TRUE;
				}
			}
		}

		////////////////////////////////////////////////// HEADERS

		SetFontForeground( FONT_MCOLOR_WHITE );

		// "PRIMARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 0 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sTop, gItemDescGenHeaderRegions[0].sRight - gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sBottom - gItemDescGenHeaderRegions[0].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "SECONDARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 1 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sTop, gItemDescGenHeaderRegions[1].sRight - gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sBottom - gItemDescGenHeaderRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		SetFontForeground( 6 );

		///////////////////////////////////////////////////// INDEXES

		SetFontForeground( FONT_MCOLOR_WHITE );

		for (UINT8 cnt = 0; cnt < 4; cnt++)
		{
			// Select "PROPERTY", "0", "+" or "=" as appropriate.
			swprintf(pStr, L"%s", gzItemDescGenIndexes[ cnt ]);

			FindFontCenterCoordinates( gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sTop, gItemDescGenIndexRegions[0][cnt].sRight - gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sBottom - gItemDescGenIndexRegions[0][cnt].sTop, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}

		SetFontForeground( 6 );

		////////////////// DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
		{
			// Set line to draw into
			ubNumLine = 0;

			// Get final damage
			INT16 iFinalDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDamage, 0 );

			// Get base damage
			INT16 iDamage = iFinalDamage;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDamage, 0 );
				// Get base damage
				INT16 iComparedDamage = iComparedFinalDamage;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedDamage - iDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalDamage - iFinalDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDamage > 0 )
		{
			// Set line to draw into
			ubNumLine = 0;
			// Get final damage
			INT16 iFinalDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDamage, 0 );
			// Get base damage
			INT16 iDamage = iFinalDamage;
			// Print base value
			DrawPropertyValueInColour( iDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		/////////////////// STUN DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
		{
			// Set line to draw into
			ubNumLine = 1;

			// Get final damage
			INT16 iFinalStunDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStunDamage, 1 );

			// Get base damage
			INT16 iStunDamage = iFinalStunDamage;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iStunDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalStunDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalStunDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStunDamage, 1 );
				// Get base damage
				INT16 iComparedStunDamage = iComparedFinalStunDamage;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedStunDamage - iStunDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalStunDamage - iFinalStunDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStunDamage > 0 )
		{
			// Set line to draw into
			ubNumLine = 1;
			// Get final damage
			INT16 iFinalStunDamage = GetModifiedExplosiveDamage( Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStunDamage, 1 );
			// Get base damage
			INT16 iStunDamage = iFinalStunDamage;
			// Print base value
			DrawPropertyValueInColour( iStunDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalStunDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		// HEADROCK HAM 5: Pushed everyone one line down to make room for Contact Explosives.

		////////////////////// BLAST RADIUS
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration == 0 )
		{
			// Set line to draw into
			ubNumLine = 3;

			// Get final damage
			INT16 iFinalBlastRadius = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubRadius;

			// Get base damage
			INT16 iBlastRadius = iFinalBlastRadius;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iBlastRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalBlastRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalBlastRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubRadius;
				// Get base damage
				INT16 iComparedBlastRadius = iComparedFinalBlastRadius;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedBlastRadius - iBlastRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalBlastRadius - iFinalBlastRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		////////////////////// PROLONGED EFFECT: START
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
		{
			// Set line to draw into
			ubNumLine = 3;

			INT16 iFinalEffectStartRadius;
			// Get final damage
			if (Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 4)
			{
				iFinalEffectStartRadius = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubRadius;
			}
			else
			{
				iFinalEffectStartRadius = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStartRadius;
			}

			// Get base damage
			INT16 iEffectStartRadius = iFinalEffectStartRadius;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iEffectStartRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalEffectStartRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				INT16 iComparedFinalEffectStartRadius;
				// Get final damage
				if (Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubType == 4)
					iComparedFinalEffectStartRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubRadius;
				else
					iComparedFinalEffectStartRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStartRadius;
				// Get base damage
				INT16 iComparedEffectStartRadius = iComparedFinalEffectStartRadius;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedEffectStartRadius - iEffectStartRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalEffectStartRadius - iFinalEffectStartRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		////////////////////// PROLONGED EFFECT: END
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
		{
			// Set line to draw into
			ubNumLine = 4;

			INT16 iFinalEffectEndRadius;
			// Get final damage
			if (Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubType == 4)
			{
				iFinalEffectEndRadius = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubStartRadius;
			}
			else
			{
				iFinalEffectEndRadius = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubRadius;
			}

			// Get base damage
			INT16 iEffectEndRadius = iFinalEffectEndRadius;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iEffectEndRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalEffectEndRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				INT16 iComparedFinalEffectEndRadius;
				// Get final damage
				if (Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubType == 4)
					iComparedFinalEffectEndRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubStartRadius;
				else
					iComparedFinalEffectEndRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubRadius;
				// Get base damage
				INT16 iComparedEffectEndRadius = iComparedFinalEffectEndRadius;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedEffectEndRadius - iEffectEndRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalEffectEndRadius - iFinalEffectEndRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration > 0)
		{
			ubNumLine = 4;
			INT16 iFinalEffectEndRadius;
			// Get final damage
			iFinalEffectEndRadius = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubRadius;
			// Get base damage
			INT16 iEffectEndRadius = iFinalEffectEndRadius;
			// Print base value
			DrawPropertyValueInColour( iEffectEndRadius, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalEffectEndRadius, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		////////////////////// PROLONGED EFFECT: DURATION
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration > 0 )
		{
			// Set line to draw into
			ubNumLine = 5;

			// Get final damage
			INT16 iFinalEffectDuration = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDuration;

			// Get base damage
			INT16 iEffectDuration = iFinalEffectDuration;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iEffectDuration, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalEffectDuration, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalEffectDuration = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration;
				// Get base damage
				INT16 iComparedEffectDuration = iComparedFinalEffectDuration;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedEffectDuration - iEffectDuration, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalEffectDuration - iFinalEffectDuration, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration> 0 )
		{
			ubNumLine = 5;
			// Get final damage
			INT16 iFinalEffectDuration = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDuration;
			// Get base damage
			INT16 iEffectDuration = iFinalEffectDuration;
			// Print base value
			DrawPropertyValueInColour( iEffectDuration, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalEffectDuration, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		// HEADROCK HAM 5: FRAGMENTATIONS
		//////////////////// NUMBER OF FRAGMENTS
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
		{
			// Set line to draw into
			ubNumLine = 6;

			// Get final Num Fragments
			INT16 iFinalNumFragments = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments;

			// Get base Num Fragments
			INT16 iNumFragments = iFinalNumFragments;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iNumFragments, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalNumFragments, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final Num Fragments
				INT16 iComparedFinalNumFragments = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments;
				// Get base Num Fragments
				INT16 iComparedNumFragments = iComparedFinalNumFragments;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedNumFragments - iNumFragments, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalNumFragments - iFinalNumFragments, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments> 0 )
		{
			// Set line to draw into
			ubNumLine = 6;
			// Get final Num Fragments
			INT16 iFinalNumFragments = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments;
			// Get base Num Fragments
			INT16 iNumFragments = iFinalNumFragments;
			// Print base value
			DrawPropertyValueInColour( iNumFragments, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalNumFragments, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		//////////////////// FRAGMENT DAMAGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
		{
			// Set line to draw into
			ubNumLine = 7;

			// Get final Num Fragments
			INT16 iFinalFragDamage = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubFragDamage;

			// Get base Num Fragments
			INT16 iFragDamage = iFinalFragDamage;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iFragDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalFragDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final Num Fragments
				INT16 iComparedFinalFragDamage = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubFragDamage;
				// Get base Num Fragments
				INT16 iComparedFragDamage = iComparedFinalFragDamage;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedFragDamage - iFragDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalFragDamage - iFinalFragDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments> 0 )
		{
			ubNumLine = 7;
			// Get final Num Fragments
			INT16 iFinalFragDamage = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubFragDamage;
			// Get base Num Fragments
			INT16 iFragDamage = iFinalFragDamage;
			// Print base value
			DrawPropertyValueInColour( iFragDamage, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalFragDamage, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		//////////////////// FRAG RANGE
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].usNumFragments > 0 )
		{
			// Set line to draw into
			ubNumLine = 8;

			// Get final Num Fragments
			INT16 iFinalFragRange = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubFragRange / CELL_X_SIZE;

			// Get base Num Fragments
			INT16 iFragRange = iFinalFragRange;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iFragRange, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalFragRange, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final Num Fragments
				INT16 iComparedFinalFragRange = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubFragRange / CELL_X_SIZE;
				// Get base Num Fragments
				INT16 iComparedFragRange = iComparedFinalFragRange;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedFragRange - iFragRange, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalFragRange - iFinalFragRange, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].usNumFragments> 0 )
		{
			ubNumLine = 8;
			// Get final Num Fragments
			INT16 iFinalFragRange = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubFragRange / CELL_X_SIZE;
			// Get base Num Fragments
			INT16 iFragRange = iFinalFragRange;
			// Print base value
			DrawPropertyValueInColour( iFragRange, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalFragRange, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
		}

		//////////////////// LOUDNESS
		{
			// Set line to draw into
			ubNumLine = 9;

			// Get final damage
			INT16 iFinalLoudness = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolume;

			// Get base damage
			INT16 iLoudness = iFinalLoudness;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iLoudness, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalLoudness, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalLoudness = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubVolume;
				// Get base damage
				INT16 iComparedLoudness = iComparedFinalLoudness;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedLoudness - iLoudness, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalLoudness - iFinalLoudness, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
		}

		//////////////////// VOLATILITY
		if ( Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
		{
			// Set line to draw into
			ubNumLine = 10;

			// Get final damage
			INT16 iFinalVolatility = Explosive[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubVolatility;

			// Get base damage
			INT16 iVolatility = iFinalVolatility;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iVolatility, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalVolatility, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get final damage
				INT16 iComparedFinalVolatility = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubVolatility;
				// Get base damage
				INT16 iComparedVolatility = iComparedFinalVolatility;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedVolatility - iVolatility, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalVolatility - iFinalVolatility, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			}
		}
		else if( fComparisonMode && Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubVolatility > 0 )
		{
			// Set line to draw into
			ubNumLine = 10;
			// Get final damage
			INT16 iFinalVolatility = Explosive[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubVolatility;
			// Get base damage
			INT16 iVolatility = iFinalVolatility;
			// Print base value
			DrawPropertyValueInColour( iVolatility, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
			// Print modifier
			DrawPropertyTextInColour( L"=", ubNumLine, 2 );
			// Print final value
			DrawPropertyValueInColour( iFinalVolatility, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
			
		}

		//////////////////// REPAIR EASE
		{
			// Set line to draw into
			ubNumLine = 11;
			// Set Y coordinates
			sTop = gItemDescGenRegions[ubNumLine][1].sTop;
			sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

			// Get base Reliability value
			INT8 iRepairEaseValue = Item[gpItemDescObject->usItem].bRepairEase;

			// Get final Reliability value
			INT8 iFinalRepairEaseValue = iRepairEaseValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Reliability value
				INT8 iComparedRepairEaseValue = Item[gpComparedItemDescObject->usItem].bRepairEase;
				// Get final Reliability value
				INT8 iComparedFinalRepairEaseValue = iComparedRepairEaseValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedRepairEaseValue - iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalRepairEaseValue - iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedValues( gpItemDescObject );
	}
}

void DrawArmorValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				usX;
	UINT8				ubNumLine;

	OBJECTTYPE *gpComparedItemDescObject = NULL;

	if( UsingEDBSystem() == 0 )
		return;

	//////////////////////////////////////////////////////////////////////////////////////////
	// The following attributes are only drawn when the UDB is set to show the General Page.

	if (gubDescBoxPage == 1)
	{

		// anv: if ctrl is pressed in map inventory, show comparison with selected armor
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_ARMOUR )
				{
					if( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == Armour[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubArmourClass )
						fComparisonMode = TRUE;
				}
			}
		}

		////////////////////////////////////////////////// HEADERS

		SetFontForeground( FONT_MCOLOR_WHITE );

		// "PRIMARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 0 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sTop, gItemDescGenHeaderRegions[0].sRight - gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sBottom - gItemDescGenHeaderRegions[0].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "SECONDARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 1 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sTop, gItemDescGenHeaderRegions[1].sRight - gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sBottom - gItemDescGenHeaderRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		SetFontForeground( 6 );

		///////////////////////////////////////////////////// INDEXES

		SetFontForeground( FONT_MCOLOR_WHITE );

		for (UINT8 cnt = 0; cnt < 4; cnt++)
		{
			// Select "PROPERTY", "0", "+" or "=" as appropriate.
			swprintf(pStr, L"%s", gzItemDescGenIndexes[ cnt ]);

			FindFontCenterCoordinates( gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sTop, gItemDescGenIndexRegions[0][cnt].sRight - gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sBottom - gItemDescGenIndexRegions[0][cnt].sTop, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}

		SetFontForeground( 6 );

		/////////////////// PROTECTION
		{
			// Set line to draw into
			ubNumLine = 0;
			
			// Get protection value
			UINT16 iProtectionValue = Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubProtection;

			// Get Final Protection value
			UINT16 iFinalProtectionValue = iProtectionValue;
			for (attachmentList::iterator iter = (*gpItemDescObject)[0]->attachments.begin(); iter != (*gpItemDescObject)[0]->attachments.end(); ++iter) 
			{
				if (Item[ iter->usItem ].usItemClass == IC_ARMOUR)
				{
					iFinalProtectionValue += Armour[Item[iter->usItem].ubClassIndex].ubProtection;
				}
			}

			// Get difference
			INT16 iProtectionModifier = iFinalProtectionValue - iProtectionValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iProtectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyValueInColour( iProtectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print final value
				DrawPropertyValueInColour( iFinalProtectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get protection value
				UINT16 iComparedProtectionValue = Armour[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubProtection;
				// Get Final Protection value
				UINT16 iComparedFinalProtectionValue = iComparedProtectionValue;
				for (attachmentList::iterator iter = (*gpComparedItemDescObject)[0]->attachments.begin(); iter != (*gpComparedItemDescObject)[0]->attachments.end(); ++iter) 
				{
					if (Item[ iter->usItem ].usItemClass == IC_ARMOUR)
					{
						iComparedFinalProtectionValue += Armour[Item[iter->usItem].ubClassIndex].ubProtection;
					}
				}
				// Get difference
				INT16 iComparedProtectionModifier = iComparedFinalProtectionValue - iComparedProtectionValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedProtectionValue - iProtectionValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyValueInColour( iComparedProtectionModifier - iProtectionModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalProtectionValue - iFinalProtectionValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}

		/////////////////// COVERAGE
		{
			// Set line to draw into
			ubNumLine = 1;
			
			// Get Coverage value
			UINT8 iCoverageValue = Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubCoverage;

			// Get Final Coverage value
			UINT8 iFinalCoverageValue = iCoverageValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iCoverageValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 0, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalCoverageValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE, TRUE );
			}
			else
			{
				// Get Coverage value
				UINT8 iComparedCoverageValue = Armour[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubCoverage;
				// Get Final Coverage value
				UINT8 iComparedFinalCoverageValue = iComparedCoverageValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedCoverageValue - iCoverageValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE, 0, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalCoverageValue - iFinalCoverageValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, 0, TRUE );
			}
		}

		//////////////////// DEGRADE RATE
		{
			// Set line to draw into
			ubNumLine = 2;
			
			// Get Degrade value
			UINT8 iDegradeValue = Armour[Item[ gpItemDescObject->usItem ].ubClassIndex ].ubDegradePercent;

			// Get Final Degrade value
			UINT8 iFinalDegradeValue = iDegradeValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iDegradeValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalDegradeValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE, TRUE );
			}
			else
			{
				// Get Degrade value
				UINT8 iComparedDegradeValue = Armour[Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubDegradePercent;
				// Get Final Degrade value
				UINT8 iComparedFinalDegradeValue = iComparedDegradeValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedDegradeValue - iDegradeValue, ubNumLine, 1, fComparisonMode, FALSE, FALSE, 0, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalDegradeValue - iFinalDegradeValue, ubNumLine, 3, fComparisonMode, FALSE, FALSE, 0, TRUE );
			}
		}

		//////////////////// REPAIR EASE
		{
			// Set line to draw into
			ubNumLine = 3;

			// Get base Reliability value
			INT8 iRepairEaseValue = Item[gpItemDescObject->usItem].bRepairEase;

			// Get final Reliability value
			INT8 iFinalRepairEaseValue = iRepairEaseValue;

			if( !fComparisonMode )
			{
				// Print base value
				DrawPropertyValueInColour( iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print modifier
				DrawPropertyTextInColour( L"--", ubNumLine, 2 );
				// Print final value
				DrawPropertyValueInColour( iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
			}
			else
			{
				// Get base Reliability value
				INT8 iComparedRepairEaseValue = Item[gpComparedItemDescObject->usItem].bRepairEase;
				// Get final Reliability value
				INT8 iComparedFinalRepairEaseValue = iComparedRepairEaseValue;
				// Print difference in base value
				DrawPropertyValueInColour( iComparedRepairEaseValue - iRepairEaseValue, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
				// Print difference in modifier
				DrawPropertyTextInColour( L"=", ubNumLine, 2 );
				// Print difference in final value
				DrawPropertyValueInColour( iComparedFinalRepairEaseValue - iFinalRepairEaseValue, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
			}
		}
	}
	else if (gubDescBoxPage == 2)
	{
		DrawAdvancedValues( gpItemDescObject );
	}
}

void DrawAdvancedValues( OBJECTTYPE *gpItemDescObject )
{
	if (gubDescBoxPage != 2)
	{
		return;
	}

	INT16 sFirstLine = gubDescBoxLine;
	INT16 sLastLine = gubDescBoxLine + NUM_UDB_ADV_LINES;
	INT16 cnt = 0;

	static CHAR16		pStr[ 100 ], pStr2[100];
	INT16				usY;
	INT16				usX;
	INT16				sLeft, sTop, sWidth, sHeight;

	INT16 iModifier[3];
	FLOAT iFloatModifier[3];

	INT16 iComparedModifier[3];
	FLOAT iComparedFloatModifier[3];

	FLOAT bRecoilModifierX;
	FLOAT bRecoilModifierY;

	FLOAT bComparedRecoilModifierX;
	FLOAT bComparedRecoilModifierY;

	OBJECTTYPE *gpComparedItemDescObject = NULL;
	// anv: if ctrl is pressed in map inventory, show comparison with selected item
	BOOLEAN fComparisonMode = FALSE;
	if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
	{
		if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
			gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
		if( gpComparedItemDescObject != NULL )
		{
			if( Item[ gpItemDescObject->usItem ].usItemClass == Item[ gpComparedItemDescObject->usItem ].usItemClass )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_AMMO )
					fComparisonMode = TRUE;
				else if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_ARMOUR )
				{
					if( Armour[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubArmourClass == Armour[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].ubArmourClass )
						fComparisonMode = TRUE;
				}
				else 
					fComparisonMode = TRUE;
			}
			if( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_EXPLOSV && Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
				fComparisonMode = TRUE;
			if( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) && Item[ gpComparedItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE|IC_BLADE|IC_PUNCH) )
				fComparisonMode = TRUE;
		}
	}

	///////////////////// INDEX
	swprintf( pStr, gzItemDescGenIndexes[0] );
	sLeft = gItemDescAdvIndexRegions[0][0].sLeft;
	sTop = gItemDescAdvIndexRegions[0][0].sTop;
	sWidth = gItemDescAdvIndexRegions[0][0].sRight - sLeft;
	sHeight = gItemDescAdvIndexRegions[0][0].sBottom - sTop;
	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
	SetFontForeground( FONT_MCOLOR_WHITE );
	mprintf( usX, usY, pStr );

	///////////////////// ACCURACY MODIFIER
	iModifier[0] = GetAccuracyModifier( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetAccuracyModifier( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if ( ( iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) ) && UsingNewCTHSystem() == true)
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// FLAT BASE MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATBASE );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATBASE );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATBASE );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATBASE );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATBASE );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATBASE );
	}
	if ( ( ( iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0 ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// PERCENT BASE MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTBASE );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTBASE );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTBASE );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTBASE );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTBASE );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTBASE );
	}
	if ( ( ( iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0 ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// FLAT AIM MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATAIM );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATAIM );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATAIM );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_FLATAIM );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_FLATAIM );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_FLATAIM );
	}
	if ( ( ( iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0 ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// PERCENT AIM MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTAIM );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTAIM );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTAIM );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTAIM );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTAIM );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTAIM );
	}
	if ( ( ( iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0 ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// AIM LEVELS MODIFIER
	if(UsingNewCTHSystem() )
		{
		iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS );
		iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_AIMLEVELS );
		iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_AIMLEVELS );
		if( fComparisonMode )
		{
			iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_AIMLEVELS );
			iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_AIMLEVELS );
			iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_AIMLEVELS );
		}
		if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ) ||
			( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					if (UsingNewCTHSystem() == false && cnt2 > 0)
						break;
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
					}

					if (iModifier[cnt2] > 0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"+%d", iModifier[cnt2] );
					}
					else if (iModifier[cnt2] < 0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"%d", iModifier[cnt2] );
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
					}
					else
					{
						swprintf( pStr, L"--" );
					}
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		}
	}

	///////////////////// AIM BONUS MODIFIER
	if(UsingNewCTHSystem() == false)
	{
		iModifier[0] = GetAimBonus( gpItemDescSoldier, gpItemDescObject, 100, 1 );
		iModifier[1] = 0;
		iModifier[2] = 0;
		if( fComparisonMode )
		{
			iComparedModifier[0] = GetAimBonus( gpItemDescSoldier, gpComparedItemDescObject, 100, 1 );
			iComparedModifier[1] = 0;
			iComparedModifier[2] = 0;
		}
		if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ) ||
			( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					if (UsingNewCTHSystem() == false && cnt2 > 0)
						break;
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
					}
					if (iModifier[cnt2] > 0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"+%d", iModifier[cnt2] );
						wcscat( pStr, L"%" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iModifier[cnt2] < 0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"%d", iModifier[cnt2] );
						wcscat( pStr, L"%" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		}
	}

	///////////////////// TO-HIT MODIFIER
	if(UsingNewCTHSystem() == false)
	{
		iModifier[0] = GetToHitBonus( gpItemDescObject, 100, 1, FALSE );
		iModifier[1] = GetToHitBonus( gpItemDescObject, 100, 1, FALSE );
		iModifier[2] = GetToHitBonus( gpItemDescObject, 100, 1, TRUE );
		if( fComparisonMode )
		{
			iComparedModifier[0] = GetToHitBonus( gpComparedItemDescObject, 100, 1, FALSE );
			iComparedModifier[1] = GetToHitBonus( gpComparedItemDescObject, 100, 1, FALSE );
			iComparedModifier[2] = GetToHitBonus( gpComparedItemDescObject, 100, 1, TRUE );
		}
		if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ) ||
			( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		{
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
					}
					if (iModifier[cnt2] > 0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"+%d", iModifier[cnt2] );
						wcscat( pStr, L"%" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iModifier[cnt2] < 0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"%d", iModifier[cnt2] );
						wcscat( pStr, L"%" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode && cnt2 != 1 )
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						//swprintf( pStr, L"--" );
						//FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		}
	}

	///////////////////// CTH CAP MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTCAP );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTCAP );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTCAP );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTCAP );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTCAP );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTCAP );
	}
	if ( ( ( iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0 ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// GUN HANDLING MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTHANDLING );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTHANDLING );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_PERCENTHANDLING );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_PERCENTHANDLING );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_PERCENTHANDLING );
	}
	if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// DROP COMPENSATION MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_DROPCOMPENSATION );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_DROPCOMPENSATION );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_DROPCOMPENSATION );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_DROPCOMPENSATION );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_DROPCOMPENSATION );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_DROPCOMPENSATION );
	}
	if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// TARGET TRACKING MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_TRACKING );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_TRACKING );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_TRACKING );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_TRACKING );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_TRACKING );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_TRACKING );
	}
	if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// DAMAGE MODIFIER
	if (Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_THROWING_KNIFE))
		iModifier[0] = (GetDamageBonus( gpItemDescObject ) * gGameExternalOptions.iGunDamageModifier) / 100;
	else if (Item[gpItemDescObject->usItem].usItemClass & (IC_BLADE|IC_PUNCH))
		iModifier[0] = (GetDamageBonus( gpItemDescObject ) * gGameExternalOptions.iMeleeDamageModifier) / 100;
	else if (Item[gpItemDescObject->usItem].usItemClass & (IC_EXPLOSV|IC_LAUNCHER))
		iModifier[0] = (GetDamageBonus( gpItemDescObject ) * gGameExternalOptions.iExplosivesDamageModifier) / 100;
	else
		iModifier[0] = GetDamageBonus( gpItemDescObject );

	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];

	if( fComparisonMode )
	{
		if (Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_THROWING_KNIFE))
			iComparedModifier[0] = (GetDamageBonus( gpComparedItemDescObject ) * gGameExternalOptions.iGunDamageModifier) / 100;
		else if (Item[gpComparedItemDescObject->usItem].usItemClass & (IC_BLADE|IC_PUNCH))
			iComparedModifier[0] = (GetDamageBonus( gpComparedItemDescObject ) * gGameExternalOptions.iMeleeDamageModifier) / 100;
		else if (Item[gpComparedItemDescObject->usItem].usItemClass & (IC_EXPLOSV|IC_LAUNCHER))
			iComparedModifier[0] = (GetDamageBonus( gpComparedItemDescObject ) * gGameExternalOptions.iExplosivesDamageModifier) / 100;
		else
			iComparedModifier[0] = GetDamageBonus( gpComparedItemDescObject );

		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}

	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// MELEE DAMAGE MODIFIER
	iModifier[0] = GetMeleeDamageBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetMeleeDamageBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		
		cnt++;
	}

	///////////////////// RANGE MODIFIER
	iModifier[0] = GetRangeBonus( gpItemDescObject ) / CELL_X_SIZE;
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetRangeBonus( gpComparedItemDescObject ) / CELL_X_SIZE;
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// SCOPE MAGNIFICATION
	iFloatModifier[0] = GetHighestScopeMagnificationFactor( gpItemDescObject );
	iFloatModifier[1] = iFloatModifier[0];
	iFloatModifier[2] = iFloatModifier[0];
	if( fComparisonMode )
	{
		iComparedFloatModifier[0] = GetHighestScopeMagnificationFactor( gpComparedItemDescObject );
		iComparedFloatModifier[1] = iComparedFloatModifier[0];
		iComparedFloatModifier[2] = iComparedFloatModifier[0];
	}
	if ( ( iFloatModifier[0] > 1.0 || ( fComparisonMode && iComparedFloatModifier[0] > 1.0 ) ) && UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					if (iFloatModifier[cnt2] > 0.0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"+%3.1fx", iFloatModifier[cnt2] );
					}
					else if (iFloatModifier[cnt2] < 0.0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"%3.1fx", iFloatModifier[cnt2] );
					}
					else
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
					}
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iFloatModifier[cnt2] > 1.0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%3.1fx", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"1.0x" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	BOOLEAN bNewCode = FALSE;
	///////////////////// PROJECTION FACTOR / BEST LASER RANGE
	// with the reworked NCTH code and the laser performance factor we will display BestLaserRange instead of ProjectionFactor
	if ( ( UsingNewCTHSystem() && ( GetBestLaserRange( gpItemDescObject ) > 0 || ( fComparisonMode && GetBestLaserRange( gpComparedItemDescObject ) > 0
		&& (gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) ) ) )
		|| ( !UsingNewCTHSystem() && ( GetBestLaserRange( gpItemDescObject ) > 0 || ( fComparisonMode && GetBestLaserRange( gpComparedItemDescObject ) > 0 ) ) ) )
	{
		iFloatModifier[0] = ((FLOAT)GetBestLaserRange( gpItemDescObject ) / CELL_X_SIZE);
		bNewCode = TRUE;
	}
	else
		iFloatModifier[0] = GetProjectionFactor( gpItemDescObject );

	iFloatModifier[1] = iFloatModifier[0];
	iFloatModifier[2] = iFloatModifier[0];
	if( fComparisonMode )
	{
		if ( bNewCode )
			iComparedFloatModifier[0] = ((FLOAT)GetBestLaserRange( gpComparedItemDescObject ) / CELL_X_SIZE);
		else
			iComparedFloatModifier[0] = GetProjectionFactor( gpComparedItemDescObject );
		iComparedFloatModifier[1] = iComparedFloatModifier[0];
		iComparedFloatModifier[2] = iComparedFloatModifier[0];
	}
	if ( iFloatModifier[0] > 1.0 || ( fComparisonMode && iComparedFloatModifier[0] > 1.0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				// laser range
				if( fComparisonMode && bNewCode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					if (iFloatModifier[cnt2] > 0.0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"+%3.1f", iFloatModifier[cnt2] );
					}
					else if (iFloatModifier[cnt2] < 0.0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"%3.1f", iFloatModifier[cnt2] );
					}
					else
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
					}
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				// projection factor
				else if( fComparisonMode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					if (iFloatModifier[cnt2] > 0.0)
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"+%3.1fx", iFloatModifier[cnt2] );
					}
					else if (iFloatModifier[cnt2] < 0.0)
					{
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
						swprintf( pStr, L"%3.1fx", iFloatModifier[cnt2] );
					}
					else
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
					}
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iFloatModifier[cnt2] > 1.0 || bNewCode)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					if ( bNewCode )
						swprintf( pStr, L"%3.0f", iFloatModifier[cnt2] );
					else
						swprintf( pStr, L"%3.1fx", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"1.0x" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// PERCENT RECOIL MODIFIER
	iModifier[0] = GetPercentRecoilModifier( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentRecoilModifier( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if ( ( iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) ) && UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	GetFlatRecoilModifier( gpItemDescObject, &bRecoilModifierX, &bRecoilModifierY );
	if( fComparisonMode )
	{
		GetFlatRecoilModifier( gpComparedItemDescObject, &bComparedRecoilModifierX, &bComparedRecoilModifierY );
	}
	///////////////////// LATERAL RECOIL MODIFIER
	iFloatModifier[0] = bRecoilModifierX;
	iFloatModifier[1] = bRecoilModifierX;
	iFloatModifier[2] = bRecoilModifierX;
	if( fComparisonMode )
	{
		iComparedFloatModifier[0] = bComparedRecoilModifierX;
		iComparedFloatModifier[1] = bComparedRecoilModifierX;
		iComparedFloatModifier[2] = bComparedRecoilModifierX;
	}
	if ( ( iFloatModifier[0] != 0 || ( fComparisonMode && iComparedFloatModifier[0] != 0 ) ) && UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
				}
				if (iFloatModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iFloatModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// VERTICAL RECOIL MODIFIER
	iFloatModifier[0] = bRecoilModifierY;
	iFloatModifier[1] = bRecoilModifierY;
	iFloatModifier[2] = bRecoilModifierY;
	if( fComparisonMode )
	{
		iComparedFloatModifier[0] = bComparedRecoilModifierY;
		iComparedFloatModifier[1] = bComparedRecoilModifierY;
		iComparedFloatModifier[2] = bComparedRecoilModifierY;
	}
	if ( ( iFloatModifier[0] != 0 || ( fComparisonMode && iComparedFloatModifier[0] != 0 ) ) && UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
				}
				if (iFloatModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iFloatModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// MAX COUNTER FORCE
	// HEADROCK HAM 5: Moved here because it makes more sense.
	iFloatModifier[0] = CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_STAND );
	iFloatModifier[1] = CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH );
	iFloatModifier[2] = CalcCounterForceMax( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE );
	if( fComparisonMode )
	{
		iComparedFloatModifier[0] = CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND );
		iComparedFloatModifier[1] = CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH );
		iComparedFloatModifier[2] = CalcCounterForceMax( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE );
	}
	if ( ((iFloatModifier[0] != 0 || iFloatModifier[1] != 0 || iFloatModifier[2] != 0) && UsingNewCTHSystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_GUN ) ||
		( fComparisonMode && (iComparedFloatModifier[0] != 0 || iComparedFloatModifier[1] != 0 || iComparedFloatModifier[2] != 0) && UsingNewCTHSystem() == true && Item[gpComparedItemDescObject->usItem].usItemClass == IC_GUN ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
				}
				if (iFloatModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iFloatModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%3.1f", iFloatModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// MAX COUNTER FORCE MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEMAX );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEMAX );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEMAX );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEMAX );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEMAX );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEMAX );
	}
	if ( ((iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) && UsingNewCTHSystem() == true /*&& Item[gpItemDescObject->usItem].usItemClass == IC_GUN*/ ) ||
		( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) && UsingNewCTHSystem() == true /*&& Item[gpComparedItemDescObject->usItem].usItemClass == IC_GUN*/ ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// COUNTER FORCE ACCURACY MODIFIER
	iModifier[0] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEACCURACY );
	iModifier[1] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEACCURACY );
	iModifier[2] = GetObjectModifier( gpItemDescSoldier, gpItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEACCURACY );
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_STAND, ITEMMODIFIER_COUNTERFORCEACCURACY );
		iComparedModifier[1] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_CROUCH, ITEMMODIFIER_COUNTERFORCEACCURACY );
		iComparedModifier[2] = GetObjectModifier( gpItemDescSoldier, gpComparedItemDescObject, ANIM_PRONE, ITEMMODIFIER_COUNTERFORCEACCURACY );
	}
	if ( ( (iModifier[0] != 0 || iModifier[1] != 0 || iModifier[2] != 0) || ( fComparisonMode && (iComparedModifier[0] != 0 || iComparedModifier[1] != 0 || iComparedModifier[2] != 0) ) )
		&& UsingNewCTHSystem() == true )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// AP MODIFIER
	iModifier[0] = GetAPBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetAPBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// DRAW AP MODIFIER
	iModifier[0] = GetPercentReadyTimeAPReduction( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentReadyTimeAPReduction( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"-%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// SINGLE-ATTACK AP MODIFIER
	iModifier[0] = GetPercentAPReduction( NULL, gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentAPReduction( NULL, gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"-%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// BURST AP MODIFIER
	iModifier[0] = GetPercentBurstFireAPReduction( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentBurstFireAPReduction( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"-%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// AUTOFIRE AP MODIFIER
	iModifier[0] = GetPercentAutofireAPReduction( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentAutofireAPReduction( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"-%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// RELOAD AP MODIFIER
	iModifier[0] = GetPercentReloadTimeAPReduction( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentReloadTimeAPReduction( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"-%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// MAGAZINE SIZE MODIFIER
	iModifier[0] = GetMagSizeBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetMagSizeBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// BURST SIZE MODIFIER
	iModifier[0] = GetBurstSizeBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetBurstSizeBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// HIDE MUZZLE FLASH
	iModifier[0] = IsFlashSuppressorAlt( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = IsFlashSuppressorAlt( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if ( fComparisonMode && iComparedModifier[0] )
				{
					if (iModifier[cnt2])
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
					}
					else
					{
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
						swprintf( pStr, L"Y" );
					}
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if ( fComparisonMode && !iComparedModifier[0] )
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"N" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2])
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"Y" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// LOUDNESS MODIFIER
	iModifier[0] = GetPercentNoiseVolume( gpItemDescObject )-100;
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetPercentNoiseVolume( gpComparedItemDescObject )-100;
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", abs(iModifier[cnt2]) );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// ITEM SIZE MODIFIER
	iModifier[0] = CalculateItemSize( gpItemDescObject ) - Item[ gpItemDescObject->usItem ].ItemSize;
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = CalculateItemSize( gpComparedItemDescObject ) - Item[ gpComparedItemDescObject->usItem ].ItemSize;
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// RELIABILITY MODIFIER
	iModifier[0] = GetReliability( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetReliability( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if ( (!(Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH|IC_ARMOUR|IC_EXPLOSV)) &&
		iModifier[0] != 0 ) ||
		( fComparisonMode && !(Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH|IC_ARMOUR|IC_EXPLOSV)) &&
		iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// WOODLAND CAMO
	iModifier[0] = GetCamoBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetCamoBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// URBAN CAMO
	iModifier[0] = GetUrbanCamoBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetUrbanCamoBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// DESERT CAMO
	iModifier[0] = GetDesertCamoBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetDesertCamoBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// SNOW CAMO
	iModifier[0] = GetSnowCamoBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetSnowCamoBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// STEALTH MODIFIER
	iModifier[0] = GetStealthBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetStealthBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// HEARING RANGE MODIFIER
	iModifier[0] = GetItemHearingRangeBonus( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemHearingRangeBonus( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );			
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// VISION RANGE MODIFIER
	iModifier[0] = GetItemVisionRangeBonus( gpItemDescObject, 0 );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemVisionRangeBonus( gpComparedItemDescObject, 0 );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// NIGHT VISION RANGE MODIFIER
	iModifier[0] = GetItemVisionRangeBonus( gpItemDescObject, 2 );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemVisionRangeBonus( gpComparedItemDescObject, 2 );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// DAY VISION RANGE MODIFIER
	iModifier[0] = GetItemVisionRangeBonus( gpItemDescObject, 1 );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemVisionRangeBonus( gpComparedItemDescObject, 1 );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// BRIGHT LIGHT VISION RANGE MODIFIER
	iModifier[0] = GetItemVisionRangeBonus( gpItemDescObject, 3 );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemVisionRangeBonus( gpComparedItemDescObject, 3 );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// CAVE VISION RANGE MODIFIER
	iModifier[0] = GetItemVisionRangeBonus( gpItemDescObject, 4 );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemVisionRangeBonus( gpComparedItemDescObject, 4 );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}

	///////////////////// PERCENT TUNNEL VISION
	iModifier[0] = GetItemPercentTunnelVision( gpItemDescObject );
	iModifier[1] = iModifier[0];
	iModifier[2] = iModifier[0];
	if( fComparisonMode )
	{
		iComparedModifier[0] = GetItemPercentTunnelVision( gpComparedItemDescObject );
		iComparedModifier[1] = iComparedModifier[0];
		iComparedModifier[2] = iComparedModifier[0];
	}
	if (iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
	{
		if (cnt >= sFirstLine && cnt < sLastLine)
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

			// Print Values
			for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
			{
				if (UsingNewCTHSystem() == false && cnt2 > 0)
					break;
				SetFontForeground( 5 );
				sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
				sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
				if( fComparisonMode )
				{
					iModifier[cnt2] = iComparedModifier[cnt2] - iModifier[cnt2];
				}
				if (iModifier[cnt2] > 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"+%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if (iModifier[cnt2] < 0)
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"%d", iModifier[cnt2] );
					wcscat( pStr, L"%" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					#ifdef CHINESE
						wcscat( pStr, ChineseSpecString1 );
					#else
						wcscat( pStr, L"%" );
					#endif
				}
				else if( fComparisonMode )
				{
					swprintf( pStr, L"=" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					swprintf( pStr, L"--" );
					FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}
		}
		cnt++;
	}
	
	// Flugente: draw a new description line 
	// Moa: only if required
	INT16 fDrawGenIndexes = FALSE;

	// Flugente	
	if ( gGameExternalOptions.fWeaponOverheating )
	{	
		if ( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{			
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// insert Indexes here?
			///////////////////// SINGLE SHOT TEMPERATURE
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Weapon[ gpItemDescObject->usItem ].usOverheatingSingleShotTemperature;
				if ( Item[gpItemDescObject->usItem].usItemClass & IC_GUN )
					iFloatModifier[0] *= gItemSettings.fOverheatTemperatureModifierGun[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ];
				else if ( Item[gpItemDescObject->usItem].usItemClass & IC_LAUNCHER )
					iFloatModifier[0] *= gItemSettings.fOverheatTemperatureModifierLauncher;

				iFloatModifier[1] = GetSingleShotTemperature( gpItemDescObject ) - iFloatModifier[0];
				iFloatModifier[2] = GetSingleShotTemperature( gpItemDescObject );

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Weapon[ gpComparedItemDescObject->usItem ].usOverheatingSingleShotTemperature;
					if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_GUN )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatTemperatureModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_LAUNCHER )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatTemperatureModifierLauncher;
					iComparedFloatModifier[1] = GetSingleShotTemperature( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetSingleShotTemperature( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		
			///////////////////// COOLDOWN FACTOR		
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Item[gpItemDescObject->usItem].usOverheatingCooldownFactor;
				if ( Item[gpItemDescObject->usItem].usItemClass & IC_GUN )
					iFloatModifier[0] *= gItemSettings.fOverheatCooldownModifierGun[ Weapon[gpItemDescObject->usItem].ubWeaponType ];
				else if ( Item[gpItemDescObject->usItem].usItemClass & IC_LAUNCHER )
					iFloatModifier[0] *= gItemSettings.fOverheatCooldownModifierLauncher;
				iFloatModifier[1] = GetItemCooldownFactor(gpItemDescObject) - iFloatModifier[0];
				iFloatModifier[2] = GetItemCooldownFactor(gpItemDescObject);

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Item[ gpComparedItemDescObject->usItem ].usOverheatingCooldownFactor;
					if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_GUN )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatCooldownModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_LAUNCHER )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatCooldownModifierLauncher;
					iComparedFloatModifier[1] = GetItemCooldownFactor( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetItemCooldownFactor( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;

			///////////////////// JAM THRESHOLD
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Weapon[ gpItemDescObject->usItem ].usOverheatingJamThreshold;
				if ( Item[gpItemDescObject->usItem].usItemClass & IC_GUN )
					iFloatModifier[0] *= gItemSettings.fOverheatJamThresholdModifierGun[ Weapon[gpItemDescObject->usItem].ubWeaponType ];
				else if ( Item[gpItemDescObject->usItem].usItemClass & IC_LAUNCHER )
					iFloatModifier[0] *= gItemSettings.fOverheatJamThresholdModifierLauncher;
				iFloatModifier[1] = GetOverheatJamThreshold(gpItemDescObject) - iFloatModifier[0];
				iFloatModifier[2] = GetOverheatJamThreshold(gpItemDescObject);

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Weapon[ gpComparedItemDescObject->usItem ].usOverheatingJamThreshold;
					if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_GUN )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatJamThresholdModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_LAUNCHER )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatJamThresholdModifierLauncher;
					iComparedFloatModifier[1] = GetOverheatJamThreshold( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetOverheatJamThreshold( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.0f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.0f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;

			///////////////////// DAMAGE THRESHOLD
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Weapon[ gpItemDescObject->usItem ].usOverheatingDamageThreshold;
				if ( Item[gpItemDescObject->usItem].usItemClass & IC_GUN )
					iFloatModifier[0] *= gItemSettings.fOverheatDamageThresholdModifierGun[ Weapon[gpItemDescObject->usItem].ubWeaponType ];
				else if ( Item[gpItemDescObject->usItem].usItemClass & IC_LAUNCHER )
					iFloatModifier[0] *= gItemSettings.fOverheatDamageThresholdModifierLauncher;
				iFloatModifier[1] = GetOverheatDamageThreshold(gpItemDescObject) - iFloatModifier[0];
				iFloatModifier[2] = GetOverheatDamageThreshold(gpItemDescObject);

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Weapon[ gpComparedItemDescObject->usItem ].usOverheatingDamageThreshold;
					if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_GUN )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatDamageThresholdModifierGun[ Weapon[ gpComparedItemDescObject->usItem ].ubWeaponType ];
					else if ( Item[gpComparedItemDescObject->usItem].usItemClass & IC_LAUNCHER )
						iComparedFloatModifier[0] *= gItemSettings.fOverheatDamageThresholdModifierLauncher;
					iComparedFloatModifier[1] = GetOverheatDamageThreshold( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetOverheatDamageThreshold( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.0f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.0f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;

		} 
		else if ( ( Item[gpItemDescObject->usItem].barrel == TRUE )	||	// display for barrel items
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].barrel == TRUE ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// insert Indexes here?
			///////////////////// COOLDOWN FACTOR		
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Item[gpItemDescObject->usItem].usOverheatingCooldownFactor;
				iFloatModifier[1] = GetItemCooldownFactor(gpItemDescObject) - iFloatModifier[0];
				iFloatModifier[2] = GetItemCooldownFactor(gpItemDescObject);

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Item[gpComparedItemDescObject->usItem].usOverheatingCooldownFactor;
					iComparedFloatModifier[1] = GetItemCooldownFactor( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetItemCooldownFactor( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		}
		else if ( ( ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ||
			( fComparisonMode && ( ( Item[gpComparedItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatCooldownModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) || ( Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt;		// insert Indexes here?

			///////////////////// TEMPERATURE MODIFIER
			if ( ( Item[gpItemDescObject->usItem].overheatTemperatureModificator != 0.0 ) && cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = -1.0f - Item[gpItemDescObject->usItem].overheatTemperatureModificator;
				iFloatModifier[1] = GetTemperatureModifier( gpItemDescObject ) - iFloatModifier[0];
				iFloatModifier[2] = GetTemperatureModifier( gpItemDescObject );

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = -1.0f - Item[gpComparedItemDescObject->usItem].overheatTemperatureModificator;
					iComparedFloatModifier[1] = GetTemperatureModifier( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetTemperatureModifier( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
			///////////////////// TEMPERATURE MODIFIER

			///////////////////// COOLDOWN MODIFIER
			if ( ( Item[gpItemDescObject->usItem].overheatCooldownModificator != 0.0 || ( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatCooldownModificator != 0.0 ) ) 
				&& cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Item[gpItemDescObject->usItem].overheatCooldownModificator;
				iFloatModifier[1] = GetItemCooldownModificator( gpItemDescObject ) - iFloatModifier[0];
				iFloatModifier[2] = GetItemCooldownModificator( gpItemDescObject );

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Item[gpComparedItemDescObject->usItem].overheatCooldownModificator;
					iComparedFloatModifier[1] = GetItemCooldownModificator( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetItemCooldownModificator( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
			///////////////////// COOLDOWN MODIFIER

			///////////////////// JAM THRESHOLD MODIFIER
			if ( ( Item[gpItemDescObject->usItem].overheatJamThresholdModificator != 0.0 || ( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator != 0.0 ) ) 
				&& cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = -1.0f - Item[gpItemDescObject->usItem].overheatJamThresholdModificator;
				iFloatModifier[1] = GetOverheatJamThresholdModifier( gpItemDescObject ) - iFloatModifier[0];
				iFloatModifier[2] = GetOverheatJamThresholdModifier( gpItemDescObject );

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = -1.0f - Item[gpComparedItemDescObject->usItem].overheatJamThresholdModificator;
					iComparedFloatModifier[1] = GetOverheatJamThresholdModifier( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetOverheatJamThresholdModifier( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
			///////////////////// JAM THRESHOLD MODIFIER

			///////////////////// DAMAGE THRESHOLD MODIFIER
			if ( ( Item[gpItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 || ( fComparisonMode && Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator != 0.0 ) ) 
				&& cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = -1.0f - Item[gpItemDescObject->usItem].overheatDamageThresholdModificator;
				iFloatModifier[1] = GetOverheatDamageThresholdModifier( gpItemDescObject ) - iFloatModifier[0];
				iFloatModifier[2] = GetOverheatDamageThresholdModifier( gpItemDescObject );

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = -1.0f - Item[gpComparedItemDescObject->usItem].overheatDamageThresholdModificator;
					iComparedFloatModifier[1] = GetOverheatDamageThresholdModifier( gpComparedItemDescObject ) - iComparedFloatModifier[0];
					iComparedFloatModifier[2] = GetOverheatDamageThresholdModifier( gpComparedItemDescObject );
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
			///////////////////// DAMAGE THRESHOLD MODIFIER
		}
	}
		
	///////////////////// DIRT MODIFICATOR
	{	
		if ( ( Item[gpItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) ||
			( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt; //insert Indexes here?
			if (cnt >= sFirstLine && cnt < sLastLine)
			{
				// Set Y coordinates
				sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
				sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;		

				iFloatModifier[0] = Item[ gpItemDescObject->usItem ].dirtIncreaseFactor * gGameExternalOptions.iDirtGlobalModifier;				
				iFloatModifier[2] = GetItemDirtIncreaseFactor( gpItemDescObject, TRUE );
				iFloatModifier[1] = iFloatModifier[2] - iFloatModifier[0];

				if( fComparisonMode )
				{
					iComparedFloatModifier[0] = Item[ gpComparedItemDescObject->usItem ].dirtIncreaseFactor * gGameExternalOptions.iDirtGlobalModifier;	
					iComparedFloatModifier[2] = GetItemDirtIncreaseFactor( gpComparedItemDescObject, TRUE );
					iComparedFloatModifier[1] = iComparedFloatModifier[2] - iComparedFloatModifier[0];
				}

				// Print Values
				for (UINT8 cnt2 = 0; cnt2 < 3; cnt2++)
				{
					SetFontForeground( 5 );
					sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
					sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
					if( fComparisonMode )
					{
						iFloatModifier[cnt2] = iComparedFloatModifier[cnt2] - iFloatModifier[cnt2];
					}
					if (iFloatModifier[cnt2] > 0)
					{						
						if ( cnt2 == 1 || fComparisonMode)
							SetFontForeground( ITEMDESC_FONTNEGATIVE );

						if ( !fComparisonMode && cnt2 != 1 )
							swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						else
							swprintf( pStr, L"+%4.2f", iFloatModifier[cnt2] );

						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if (iFloatModifier[cnt2] < 0)
					{
						if ( cnt2 == 1 || fComparisonMode)
							SetFontForeground( ITEMDESC_FONTPOSITIVE );

						swprintf( pStr, L"%4.2f", iFloatModifier[cnt2] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
						#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
						#else
							wcscat( pStr, L"%" );
						#endif
					}
					else if( fComparisonMode )
					{
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					else
					{
						swprintf( pStr, L"--" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
					}
					mprintf( usX, usY, pStr );
				}
			}
			cnt++;
		}
	}

	///////////////////// FANTHEHAMMMER
	if ( gGameOptions.fNewTraitSystem && gSkillTraitValues.fCanFanTheHammer &&
		( ( Item[gpItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) || ( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) ) )
	{
		if ( !fDrawGenIndexes )
			fDrawGenIndexes = ++cnt; //insert Indexes here?

		if ( cnt >= sFirstLine && cnt < sLastLine )
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt - sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt - sFirstLine][1].sBottom - sTop;

			BOOL a = Weapon[gpItemDescObject->usItem].fBurstOnlyByFanTheHammer;
			BOOL b = ( fComparisonMode && Weapon[gpComparedItemDescObject->usItem].fBurstOnlyByFanTheHammer);
			
			SetFontForeground( 5 );
			sLeft = gItemDescAdvRegions[cnt - sFirstLine][1].sLeft;
			sWidth = gItemDescAdvRegions[cnt - sFirstLine][1].sRight - sLeft;

			if ( fComparisonMode )
			{
				SetFontForeground( a ? ITEMDESC_FONTPOSITIVE : ITEMDESC_FONTNEGATIVE );

				swprintf( pStr, L"%s/%s", a ? "Y" : "N", b ? "Y" : "N" );
			}
			else
			{
				SetFontForeground( 5 );

				swprintf( pStr, L"%s", a ? "Y" : "N" );
			}

			FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
			mprintf( usX, usY, pStr );
		}

		++cnt;
	}

	///////////////////// MULTIPLE BARRELS
	if ( ( ( Item[gpItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) || ( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) ) )
	{
		if ( !fDrawGenIndexes )
			fDrawGenIndexes = ++cnt; //insert Indexes here?

		if ( cnt >= sFirstLine && cnt < sLastLine )
		{
			// Set Y coordinates
			sTop = gItemDescAdvRegions[cnt - sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt - sFirstLine][1].sBottom - sTop;

			// list all modes
			swprintf( pStr, L"" );
			swprintf( pStr2, L"" );

			std::vector<UINT8> tmp = Weapon[Item[gpItemDescObject->usItem].ubClassIndex].barrelconfigurations;

			for ( std::vector<UINT8>::iterator it = tmp.begin(); it != tmp.end(); ++it )
			{
				UINT8 bla = ( *it );
				swprintf( pStr2, L"%s x%d", pStr, bla );
				swprintf( pStr, L"%s", pStr2 );
			}

			if ( ( fComparisonMode && Item[gpComparedItemDescObject->usItem].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) )
			{
				wcscat( pStr, L" / " );

				std::vector<UINT8> tmp2 = Weapon[Item[gpComparedItemDescObject->usItem].ubClassIndex].barrelconfigurations;

				for ( std::vector<UINT8>::iterator it = tmp2.begin(); it != tmp2.end(); ++it )
				{
					swprintf( pStr2, L"%s x%d", pStr, ( *it ) );
					swprintf( pStr, L"%s", pStr2 );
				}
			}
			
			SetFontForeground( 5 );
			sLeft = gItemDescAdvRegions[cnt - sFirstLine][1].sLeft;
			sWidth = gItemDescAdvRegions[cnt - sFirstLine][1].sRight - sLeft;
			
			FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
			mprintf( usX, usY, pStr );
		}

		++cnt;
	}

	////////////////////// FOOD
	if ( UsingFoodSystem() )
	{
		UINT32 fFoodtype = Item[gpItemDescObject->usItem].foodtype;
		UINT32 fComparedFoodtype = 0;
		if( fComparisonMode )
		{
			fComparedFoodtype = Item[gpComparedItemDescObject->usItem].foodtype;
		}
		if ( fFoodtype > 0 || ( fComparisonMode && fComparedFoodtype > 0 ) )
		{
			if (!fDrawGenIndexes) fDrawGenIndexes = ++cnt; //insert Indexes here?
			//////////////////////////// POISONED FOOD
			iFloatModifier[0] = (*gpItemDescObject)[0]->data.bTemperature;//temperature is reused for poisoned food
			if( fComparisonMode )
				iComparedFloatModifier[0] = (*gpComparedItemDescObject)[0]->data.bTemperature;
			if ( ( iFloatModifier[0] != 0.0f || ( fComparisonMode && iComparedFloatModifier[0] != 0.0f ) ) && OVERHEATING_MAX_TEMPERATURE > 0.0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					iFloatModifier[0] = min( 1.0f, iFloatModifier[0] / OVERHEATING_MAX_TEMPERATURE );//FoodCondition: fresh=1.0 poisoned=0.0
					sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
					sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
					iModifier[0] = 0;
					//FOOD_BAD_THRESHOLD
					if ( iFloatModifier[0] < 0.5f )
					{
						iModifier[1] = (INT16)( max( Food[fFoodtype].bFoodPoints, Food[fFoodtype].bDrinkPoints ) * (1.0 - iFloatModifier[0]) * 0.025 );//Poison formula coppied from food.cpp
					}
					else
						iModifier[1] = 0;
					iModifier[2] = iModifier[1];
					if( fComparisonMode )
					{
						iComparedFloatModifier[0] = min( 1.0f, iComparedFloatModifier[0] / OVERHEATING_MAX_TEMPERATURE );
						iComparedModifier[0] = 0;
						//FOOD_BAD_THRESHOLD		
						if ( iComparedFloatModifier[0] < 0.5f )
						{
							iComparedModifier[1] = (INT16)( max( Food[fComparedFoodtype].bFoodPoints, Food[fComparedFoodtype].bDrinkPoints ) * (1.0 - iComparedFloatModifier[0]) * 0.025 );//Poison formula coppied from food.cpp
						}
						else
							iComparedModifier[1] = 0;
						iComparedModifier[2] = iComparedModifier[1];
						iModifier[0] = iComparedModifier[0] - iModifier[0];
						iModifier[1] = iComparedModifier[1] - iModifier[1];
						iModifier[2] = iComparedModifier[2] - iModifier[2];
					}
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;					
						if ( fComparisonMode && iModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
							swprintf( pStr, L"+%d", iModifier[cnt2] );
						}
						else if ( iModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
							swprintf( pStr, L"%d", iModifier[cnt2] );
						}
						else if ( iModifier[cnt2] < 0 )
						{
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
							swprintf( pStr, L"%d", iModifier[cnt2] );
						}
						else if( fComparisonMode )
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"=" );
						}
						else
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"--" );
						}
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}
			else if( fComparisonMode && OVERHEATING_MAX_TEMPERATURE > 0.0 )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
					sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						SetFontForeground( 5 );
						swprintf( pStr, L"=" );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}
			//////////////////////////// FOOD POINTS
			iFloatModifier[0] = ( (FLOAT) Food[fFoodtype].bFoodPoints )/1000;
			if( fComparisonMode )
			{
				iComparedFloatModifier[0] = ( (FLOAT) Food[fComparedFoodtype].bFoodPoints )/1000;
			}
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
			if ( iFloatModifier[0] != 0 || ( fComparisonMode && iComparedFloatModifier[0] != 0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					iFloatModifier[2] = ( ( (FLOAT)(*gpItemDescObject)[0]->data.objectStatus )/100 ) * iFloatModifier[0];
					iFloatModifier[1] = iFloatModifier[2] - iFloatModifier[0];
					if ( fComparisonMode )
					{
						iComparedFloatModifier[2] = ( ( (FLOAT)(*gpComparedItemDescObject)[0]->data.objectStatus )/100 ) * iComparedFloatModifier[0];
						iComparedFloatModifier[1] = iComparedFloatModifier[2] - iComparedFloatModifier[0];
						iFloatModifier[0] = iComparedFloatModifier[0] - iFloatModifier[0];
						iFloatModifier[1] = iComparedFloatModifier[1] - iFloatModifier[1];
						iFloatModifier[2] = iComparedFloatModifier[2] - iFloatModifier[2];
					}
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						if ( fComparisonMode && iFloatModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
							swprintf( pStr, L"+%4.3f", iFloatModifier[cnt2] );
						}
						else if ( iFloatModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
							swprintf( pStr, L"%4.3f", iFloatModifier[cnt2] );
						}
						else if ( iFloatModifier[cnt2] < 0 )
						{
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
							swprintf( pStr, L"%4.3f", iFloatModifier[cnt2] );
						}
						else if( fComparisonMode )
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"=" );
						}
						else
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"--" );
						}
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}
			////////////////////////// DRINK POINTS
			iFloatModifier[0] = ( (FLOAT) Food[fFoodtype].bDrinkPoints )/1000;
			if( fComparisonMode )
			{
				iComparedFloatModifier[0] = ( (FLOAT) Food[fComparedFoodtype].bDrinkPoints )/1000;		
			}
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
			if ( iFloatModifier[0] != 0 || ( fComparisonMode && iComparedFloatModifier[0] != 0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					iFloatModifier[2] = ( ( (FLOAT)(*gpItemDescObject)[0]->data.objectStatus )/100 ) * iFloatModifier[0];
					iFloatModifier[1] = iFloatModifier[2] - iFloatModifier[0];
					if ( fComparisonMode )
					{
						iComparedFloatModifier[2] = ( ( (FLOAT)(*gpComparedItemDescObject)[0]->data.objectStatus )/100 ) * iComparedFloatModifier[0];
						iComparedFloatModifier[1] = iComparedFloatModifier[2] - iComparedFloatModifier[0];
						iFloatModifier[0] = iComparedFloatModifier[0] - iFloatModifier[0];
						iFloatModifier[1] = iComparedFloatModifier[1] - iFloatModifier[1];
						iFloatModifier[2] = iComparedFloatModifier[2] - iFloatModifier[2];
					}
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						if ( fComparisonMode && iFloatModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
							swprintf( pStr, L"+%3.2f", iFloatModifier[cnt2] );
						}
						else if ( iFloatModifier[cnt2] > 0 )
						{
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
							swprintf( pStr, L"%3.2f", iFloatModifier[cnt2] );
						}
						else if ( iFloatModifier[cnt2] < 0 )
						{
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
							swprintf( pStr, L"%3.2f", iFloatModifier[cnt2] );
						}
						else if( fComparisonMode )
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"=" );
						}
						else
						{
							SetFontForeground( 5 );
							swprintf( pStr, L"--" );
						}
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}
			////////////////////////// PORTION SIZE
			iModifier[0] = Item[gpItemDescObject->usItem].usPortionSize;
			if( fComparisonMode )
			{
				iComparedModifier[0] = Item[gpItemDescObject->usItem].usPortionSize;
				iModifier[0] = iComparedModifier[0] - iModifier[0];
			}
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
			if ( iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if( fComparisonMode )
					{					
						if ( iModifier[0] < 0 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
						else if ( iModifier[0] > 0 )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
						else
							SetFontForeground( 5 );
					}
					else if ( iModifier[0] <= 20 )
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
					else
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						if ( fComparisonMode && iModifier[0] > 0 )
							swprintf( pStr, L"+%d", iModifier[0] );
						else if ( fComparisonMode && iModifier[0] < 0 )
							swprintf( pStr, L"%d", iModifier[0] );
						else if ( fComparisonMode )
							swprintf( pStr, L"=" );	
						else
							swprintf( pStr, L"%d", iModifier[0] );

						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						if( !( fComparisonMode && iModifier[0] == 0 ) )
						{
							wcscat( pStr, L"%" );							
#ifdef CHINESE
							wcscat( pStr, ChineseSpecString1 );
#else
							wcscat( pStr, L"%" );
#endif
						}
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}

			////////////////////////// MORALE MODIFIER
			iModifier[0] = FoodOpinions[gpItemDescSoldier->ubProfile].sFoodOpinion[fFoodtype];
			if( fComparisonMode )
			{
				iComparedModifier[0] = FoodOpinions[gpItemDescSoldier->ubProfile].sFoodOpinion[fComparedFoodtype];
				iModifier[0] = iComparedModifier[0] - iModifier[0];
			}
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
			if ( iModifier[0] != 0 || ( fComparisonMode && iComparedModifier[0] != 0 ) )
			{
				if (cnt >= sFirstLine && cnt < sLastLine)
				{
					if( fComparisonMode )
					{					
						if ( iModifier[0] > 0 )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
						else
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
					}
					else if ( iModifier[0] > 0 )
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
					else
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						if ( fComparisonMode && iModifier[0] > 0 )
							swprintf( pStr, L"+%d", iModifier[0] );
						else if ( fComparisonMode && iModifier[0] < 0 )
							swprintf( pStr, L"%d", iModifier[0] );
						else if ( fComparisonMode )
							swprintf( pStr, L"=" );	
						else
							swprintf( pStr, L"%d", iModifier[0] );

						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}

			////////////////////////// DECAY RATE
			iFloatModifier[0] = Food[fFoodtype].usDecayRate;
			if( fComparisonMode )
			{
				iComparedFloatModifier[0] = Food[fComparedFoodtype].usDecayRate;
				iFloatModifier[0] = iComparedFloatModifier[0] - iFloatModifier[0];
			}
			sTop = gItemDescAdvRegions[cnt-sFirstLine][1].sTop;
			sHeight = gItemDescAdvRegions[cnt-sFirstLine][1].sBottom - sTop;
			if ( iFloatModifier[0] != 0 || ( fComparisonMode && iComparedFloatModifier[0] != 0 ) )
			{
				if ( cnt >= sFirstLine && cnt < sLastLine )
				{
					if( fComparisonMode )
					{					
						if ( iFloatModifier[0] < 0.0f )
							SetFontForeground( ITEMDESC_FONTPOSITIVE );
						else if ( iFloatModifier[0] > 0.0f )
							SetFontForeground( ITEMDESC_FONTNEGATIVE );
						else
							SetFontForeground( 5 );
					}
					else if ( iFloatModifier[0] > 1.0f )
						SetFontForeground( ITEMDESC_FONTNEGATIVE );
					else
						SetFontForeground( ITEMDESC_FONTPOSITIVE );
					for ( UINT8 cnt2 = 0; cnt2 < 3; cnt2++ )
					{
						sLeft = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sLeft;
						sWidth = gItemDescAdvRegions[cnt-sFirstLine][cnt2+1].sRight - sLeft;
						if( fComparisonMode && iFloatModifier[0] > 0.0f )
							swprintf( pStr, L"+%4.2f", iFloatModifier[0] );
						else
							swprintf( pStr, L"%4.2f", iFloatModifier[0] );
						FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY );
						mprintf( usX, usY, pStr );
					}
				}
				cnt++;
			}
		}//foodtype > 0
	}//gGameOptions.fFoodSystem

	//Moa: moved indexes here to avoid doublicates of the index
	///////////////////// ADDITIONAL GENERAL INDEX (if required: cnt was allready increased)
	if ( fDrawGenIndexes )
	{	
		if (fDrawGenIndexes >= sFirstLine && fDrawGenIndexes < sLastLine)
		{
			SetFontForeground( FONT_MCOLOR_WHITE );

			// Set Y coordinates
			sTop = gItemDescAdvRegions[ fDrawGenIndexes-sFirstLine-1 ][1].sTop;
			sHeight = gItemDescAdvRegions[ fDrawGenIndexes-sFirstLine-1 ][1].sBottom - sTop;

			// Repeatedly draw each index: four separate column headers per index.
			for (UINT8 i = 0; i < 4; i++)
			{
				// Select "PROPERTY", "0", "+" or "=" as appropriate.
				swprintf(pStr, L"%s", gzItemDescGenIndexes[ i ]);

				sLeft = gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][i].sLeft;
				sWidth = gItemDescAdvRegions[fDrawGenIndexes-sFirstLine-1][i].sRight - sLeft;			

				FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);

				mprintf( usX, usY, pStr );
			}
		}
	}
}

void DrawMiscValues( OBJECTTYPE * gpItemDescObject )
{
	static CHAR16		pStr[ 100 ];
	INT16				usY;
	INT16				usX;
	UINT8				ubNumLine;
	INT16				sLeft, sTop, sWidth, sHeight;

	if( UsingEDBSystem() == 0 )
		return;

	//////////////////////////////////////////////////////////////////////////////////////////
	// The following attributes are only drawn when the UDB is set to show the General Page.

	if (gubDescBoxPage == 1)
	{
		OBJECTTYPE *gpComparedItemDescObject = NULL;
		// anv: if ctrl is pressed in map inventory, show comparison with selected misc
		BOOLEAN fComparisonMode = FALSE;
		if( _KeyDown( CTRL ) && gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object.usItem )
				gpComparedItemDescObject = &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCurrentlyHighLightedItem].object;// = pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object;
			if( gpComparedItemDescObject != NULL )
			{
				if( Item[ gpComparedItemDescObject->usItem ].usItemClass == Item[ gpItemDescObject->usItem ].usItemClass )
					fComparisonMode = TRUE;
			}
		}

		////////////////////////////////////////////////// HEADERS

		SetFontForeground( FONT_MCOLOR_WHITE );

		// "PRIMARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 0 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sTop, gItemDescGenHeaderRegions[0].sRight - gItemDescGenHeaderRegions[0].sLeft, gItemDescGenHeaderRegions[0].sBottom - gItemDescGenHeaderRegions[0].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		// "SECONDARY" header
		swprintf( pStr, L"%s", gzItemDescGenHeaders[ 1 ] );
		FindFontCenterCoordinates( gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sTop, gItemDescGenHeaderRegions[1].sRight - gItemDescGenHeaderRegions[1].sLeft, gItemDescGenHeaderRegions[1].sBottom - gItemDescGenHeaderRegions[1].sTop, pStr, BLOCKFONT2, &usX, &usY );
		mprintf( usX, usY, pStr );

		SetFontForeground( 6 );

		///////////////////////////////////////////////////// INDEXES

		SetFontForeground( FONT_MCOLOR_WHITE );

		for (UINT8 cnt = 0; cnt < 4; cnt++)
		{
			// Select "PROPERTY", "0", "+" or "=" as appropriate.
			swprintf(pStr, L"%s", gzItemDescGenIndexes[ cnt ]);

			FindFontCenterCoordinates( gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sTop, gItemDescGenIndexRegions[0][cnt].sRight - gItemDescGenIndexRegions[0][cnt].sLeft, gItemDescGenIndexRegions[0][cnt].sBottom - gItemDescGenIndexRegions[0][cnt].sTop, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}

		SetFontForeground( 6 );

		//////////////////// REPAIR EASE
		// not for weapons. They have this one their primary page
		if ( !(Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH) )
		{
			// Set line to draw into
			ubNumLine = 0;
			// Set Y coordinates
			sTop = gItemDescGenRegions[ubNumLine][1].sTop;
			sHeight = gItemDescGenRegions[ubNumLine][1].sBottom - sTop;

			// Get base Repair Ease value
			INT8 iRepairEaseValue = Item[gpItemDescObject->usItem].bRepairEase;

			// Get final Repair Ease value
			INT8 iFinalRepairEaseValue = iRepairEaseValue;

			// Print base value
			SetFontForeground( 5 );
			sLeft = gItemDescGenRegions[ubNumLine][1].sLeft;
			sWidth = gItemDescGenRegions[ubNumLine][1].sRight - sLeft;
			if( fComparisonMode )
			{
				INT8 iComparedRepairEaseValue = Item[gpComparedItemDescObject->usItem].bRepairEase;
				INT8 iComparedRepairEaseDifference = iComparedRepairEaseValue - iRepairEaseValue;
				if (iComparedRepairEaseDifference < 0)
				{
					SetFontForeground( ITEMDESC_FONTNEGATIVE );
					swprintf( pStr, L"%d", iComparedRepairEaseDifference );
				}
				else if ( iComparedRepairEaseDifference > 0 )
				{
					SetFontForeground( ITEMDESC_FONTPOSITIVE );
					swprintf( pStr, L"+%d", iComparedRepairEaseDifference );
				}
				else
				{
					swprintf( pStr, L"=" );
				}
			}
			else if (iRepairEaseValue < 0)
			{
				SetFontForeground( ITEMDESC_FONTNEGATIVE );
				swprintf( pStr, L"%d", iRepairEaseValue );
			}
			else if ( iRepairEaseValue > 0 )
			{
				SetFontForeground( ITEMDESC_FONTPOSITIVE );
				swprintf( pStr, L"+%d", iRepairEaseValue );
			}
			else
			{
				swprintf( pStr, L"--" );
			}
			FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );

			// Print modifier
			SetFontForeground( 5 );
			swprintf( pStr, L"--" );
			sLeft = gItemDescGenRegions[ubNumLine][2].sLeft;
			sWidth = gItemDescGenRegions[ubNumLine][2].sRight - sLeft;
			FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );

			// Print final value
			SetFontForeground( FONT_MCOLOR_WHITE );
			sLeft = gItemDescGenRegions[ubNumLine][3].sLeft;
			sWidth = gItemDescGenRegions[ubNumLine][3].sRight - sLeft;
			swprintf( pStr, L"%d", iFinalRepairEaseValue );
			FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}
		//////////////////// LBE Stuff
		if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
		{
			//////////////////// LBE AVAILABLE VOLUME (for MOLLE carriers)
			if ( LoadBearingEquipment[ Item[ gpItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume > 0 )
			{
				// Set line to draw into
				ubNumLine = 1;

				// Get base available volume
				INT16 iAvailableVolume = LoadBearingEquipment[ Item[ gpItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume;
				// Get final available volume
				INT16 iFinalAvailableVolume = iAvailableVolume - GetVolumeAlreadyTaken( gpItemDescObject, NO_SLOT );
				// difference
				INT16 iVolumeModifier = iFinalAvailableVolume - iAvailableVolume;

				if( !fComparisonMode )
				{
					// Print base value
					DrawPropertyValueInColour( iAvailableVolume, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print modifier
					DrawPropertyValueInColour( iVolumeModifier, ubNumLine, 2, fComparisonMode, TRUE, TRUE );
					// Print final value
					DrawPropertyValueInColour( iFinalAvailableVolume, ubNumLine, 3, fComparisonMode, FALSE, TRUE, FONT_MCOLOR_WHITE );
				}
				else
				{
					INT16 iComparedAvailableVolume = 0;
					INT16 iComparedFinalAvailableVolume = 0;
					if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
					{
						// Get base available volume
						iComparedAvailableVolume = LoadBearingEquipment[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume;
						// Get final available volume
						iComparedFinalAvailableVolume = iComparedAvailableVolume - GetVolumeAlreadyTaken( gpComparedItemDescObject, NO_SLOT );
					}
					// Print difference in base value
					DrawPropertyValueInColour( iComparedAvailableVolume - iAvailableVolume, ubNumLine, 1, fComparisonMode, FALSE, TRUE );
					// Print difference in modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 ); 
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalAvailableVolume - iFinalAvailableVolume, ubNumLine, 3, fComparisonMode, FALSE, TRUE );
				}
			}
			else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
			{
				if ( LoadBearingEquipment[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume > 0
					&& LoadBearingEquipment[ Item[ gpItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume > 0)
				{
					// Set line to draw into
					ubNumLine = 1;
					// Get base available volume
					INT16 iAvailableVolume = LoadBearingEquipment[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].lbeAvailableVolume;
					// Get final available volume
					INT16 iFinalAvailableVolume = iAvailableVolume - GetVolumeAlreadyTaken( gpComparedItemDescObject, NO_SLOT );
					// difference
					INT16 iVolumeModifier = iFinalAvailableVolume - iAvailableVolume;

					// Print base value
					DrawPropertyValueInColour( iAvailableVolume, ubNumLine, 1, FALSE, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
					// Print modifier
					DrawPropertyValueInColour( iVolumeModifier, ubNumLine, 2, FALSE, TRUE, TRUE );
					// Print final value
					DrawPropertyValueInColour( iFinalAvailableVolume, ubNumLine, 3, FALSE, FALSE, TRUE, ITEMDESC_FONTPOSITIVE );
				}
			}
			//////////////////// POCKET VOLUME (for MOLLE pockets)
			if ( Item[ gpItemDescObject->usItem ].nasAttachmentClass != 0 && GetFirstPocketOnItem(gpItemDescObject->usItem) != 0 )
			{
				// Set line to draw into
				ubNumLine = 2;

				// Get base pocket volume
				INT16 iPocketVolume = LBEPocketType[GetFirstPocketOnItem(gpItemDescObject->usItem)].pVolume;
				// Get final pocket volume
				INT16 iFinalPocketVolume = iPocketVolume;

				if( !fComparisonMode )
				{
					// Print base value
					DrawPropertyValueInColour( iPocketVolume, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 );
					// Print final value
					DrawPropertyValueInColour( iFinalPocketVolume, ubNumLine, 3, fComparisonMode, FALSE, FALSE, FONT_MCOLOR_WHITE );
				}
				else
				{
					INT16 iComparedPocketVolume = 0;
					INT16 iComparedFinalPocketVolume = 0;
					if ( Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LBEGEAR && Item[ gpComparedItemDescObject->usItem ].nasAttachmentClass != 0 )
					{
						// Get base available volume
						iComparedPocketVolume = LBEPocketType[GetFirstPocketOnItem(gpComparedItemDescObject->usItem)].pVolume;
						// Get final available volume
						iComparedFinalPocketVolume = iComparedPocketVolume;
					}
					// Print difference in base value
					DrawPropertyValueInColour( iComparedPocketVolume - iPocketVolume, ubNumLine, 1, fComparisonMode, FALSE, FALSE );
					// Print difference in modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 ); 
					// Print difference in final value
					DrawPropertyValueInColour( iComparedFinalPocketVolume - iFinalPocketVolume, ubNumLine, 3, fComparisonMode, FALSE, FALSE );
				}
			}
			else if( fComparisonMode && Item[ gpComparedItemDescObject->usItem ].usItemClass & IC_LBEGEAR )
			{
				if ( Item[ gpComparedItemDescObject->usItem ].nasAttachmentClass != 0 && LoadBearingEquipment[ Item[ gpComparedItemDescObject->usItem ].ubClassIndex ].lbePocketIndex[0] > 0
					&& Item[ gpItemDescObject->usItem ].nasAttachmentClass != 0 )
				{
					// Set line to draw into
					ubNumLine = 2;
					// base available volume
					INT16 iPocketVolume = LBEPocketType[GetFirstPocketOnItem(gpComparedItemDescObject->usItem)].pVolume;
					// current available volume
					INT16 iFinalPocketVolume = iPocketVolume;

					// Print base value
					DrawPropertyValueInColour( iPocketVolume, ubNumLine, 1, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
					// Print modifier
					DrawPropertyTextInColour( L"--", ubNumLine, 2 );
					// Print final value
					DrawPropertyValueInColour( iFinalPocketVolume, ubNumLine, 3, FALSE, FALSE, FALSE, ITEMDESC_FONTPOSITIVE );
				}
			}
		}
	}

	if (gubDescBoxPage == 2)
	{
		DrawAdvancedValues( gpItemDescObject );
	}
}
