#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "language defines.h"
#else
	#include "builddefines.h"
	#include "mapscreen.h"
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
	#include "Interface Enhanced.h"
	#include "InterfaceItemImages.h"
	#include "Auto Resolve.h"
	#include "popup_callback.h"
	// BOB : quick attachment popup
	#include "popup_class.h"
#endif

#ifdef JA2UB
#include "Ja25_Tactical.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - all "APBPConstants[AP_PICKUP_ITEM]" were replaced by GetBasicAPsToPickupItem()
////////////////////////////////////////////////////////////////////////////////////////////////////////

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//CHRISL: Moved to Interface Items.h for EDB
//#define		ITEMDESC_FONT							BLOCKFONT2
#define		ITEMDESC_FONTSHADOW1			MILITARY_SHADOW
#define		ITEMDESC_FONTSHADOW2			32
#define		ITEMDESC_FONTSHADOW3			34
#define		ITEMDESC_FONTFORE1				33
#define		ITEMDESC_FONTFORE2				32

#define		ITEMDESC_FONTAPFORE					218
#define		ITEMDESC_FONTHPFORE					24
#define		ITEMDESC_FONTBSFORE					125
#define		ITEMDESC_FONTHEFORE					75
#define		ITEMDESC_FONTHEAPFORE				76

#define		ITEMDESC_AMMO_FORE					209

//CHRISL: Moved to Interface Items.h for EDB
//#define		ITEMDESC_FONTHIGHLIGHT		FONT_MCOLOR_WHITE

#define			STATUS_BAR_SHADOW				FROMRGB( 140, 136,  119 )
#define			STATUS_BAR							FROMRGB( 201, 172,  133 )
#define			DESC_STATUS_BAR_SHADOW	STATUS_BAR_SHADOW
#define			DESC_STATUS_BAR					STATUS_BAR

#define			MIN_LOB_RANGE						4

//CHRISL: Moved to Interface Items.h for EDB
//#define			INV_BAR_DX							5
//#define			INV_BAR_DY							21
//#define		ITEM_STATS_WIDTH					26
//#define		ITEM_STATS_HEIGHT					8

#define			RENDER_ITEM_NOSTATUS		20
#define			RENDER_ITEM_ATTACHMENT1	200

//LBE node stuff
// Changed some coord defines to INT16s so we can adjust their value based on game options
INT16	ITEMDESC_START_X;
INT16	ITEMDESC_START_Y;
INT16	ITEMDESC_HEIGHT;
INT16	ITEMDESC_WIDTH;
extern int SM_ITEMDESC_START_X;
extern int SM_ITEMDESC_START_Y;

extern int INV_REGION_Y;

#define		CAMO_REGION_HEIGHT			75
#define		CAMO_REGION_WIDTH				75

#define		BULLET_WIDTH						3
#define		BULLET_GAP							5

INT16		ITEMDESC_CALIBER_X;
INT16		ITEMDESC_CALIBER_Y;
INT16		ITEMDESC_CALIBER_WIDTH;
INT16		ITEMDESC_NAME_X;
INT16		ITEMDESC_NAME_Y;
INT16		ITEMDESC_ITEM_X;
INT16		ITEMDESC_ITEM_Y;
INT16		ITEMDESC_ITEM_STATUS_X;
INT16		ITEMDESC_ITEM_STATUS_Y;
INT16		ITEMDESC_ITEM_STATUS_HEIGHT;
INT16		ITEMDESC_AMMO_X;
INT16		ITEMDESC_AMMO_Y;
INT16		ITEMDESC_DONE_X;
INT16		ITEMDESC_DONE_Y;

#define		DOTDOTDOT L"..."
#define		COMMA_AND_SPACE L", "

#define		ITEM_PROS_AND_CONS( usItem ) ( ( Item[ usItem ].usItemClass & IC_GUN && !Item[ usItem ].rocketlauncher ) )

#define		ITEMDESC_ITEM_STATUS_WIDTH			2
#define		ITEMDESC_ITEM_WIDTH			117
#define		ITEMDESC_ITEM_HEIGHT		54

#define		ITEMDESC_AMMO_TEXT_X	3
#define		ITEMDESC_AMMO_TEXT_Y	1
#define		ITEMDESC_AMMO_TEXT_WIDTH 31

#define		WORD_WRAP_INV_WIDTH			58

#define		ITEM_BAR_WIDTH					2
#define		ITEM_BAR_HEIGHT					20

#define		ITEM_FONT								TINYFONT1

//CHRISL: Moved to Interface Items.h for EDB
//#define EXCEPTIONAL_GUN_DAMAGE					40
//#define EXCEPTIONAL_WEIGHT					20
//#define EXCEPTIONAL_RANGE					400
//#define EXCEPTIONAL_MAGAZINE				50
//#define EXCEPTIONAL_AP_COST					5
//#define EXCEPTIONAL_BURST_SIZE			5
//#define EXCEPTIONAL_RELIABILITY			2
//#define EXCEPTIONAL_REPAIR_EASE			2
//#define EXCEPTIONAL_ACCURACY			4

#define BAD_DAMAGE									23
#define BAD_WEIGHT									45
#define BAD_RANGE									150
#define BAD_MAGAZINE								10
#define BAD_RELIABILITY							-2
#define BAD_REPAIR_EASE							-2
#define BAD_ACCURACY							-1

// CHRISL: Adjust coords for layout changes needed for new inventory system
extern int KEYRING_X;
extern int KEYRING_Y;
extern int MAP_KEYRING_X;
extern int MAP_KEYRING_Y;
//#define KEYRING_X 487
//#define KEYRING_Y (105 + INV_INTERFACE_START_Y)
//#define MAP_KEYRING_X 217
//#define MAP_KEYRING_Y 271
#define KEYRING_WIDTH 30
#define KEYRING_HEIGHT 22

#define TACTICAL_INVENTORY_KEYRING_GRAPHIC_OFFSET_X 215
//enum used for the money buttons
enum
{
	M_1000,
	M_100,
	M_10,
	M_DONE,
};

// AN ARRAY OF MOUSE REGIONS, ONE FOR EACH OBJECT POSITION ON BUDDY
MOUSE_REGION				gInvRegions[ NUM_INV_SLOTS ];

extern	MOUSE_REGION    gMPanelRegion;
extern	BOOLEAN					fMapInventoryItem;
extern	BOOLEAN					gfAddingMoneyToMercFromPlayersAccount;
extern	SOLDIERTYPE			*gpSMCurrentMerc;
extern  UINT8 gubSelectSMPanelToMerc;
extern	MOUSE_REGION		gSM_SELMERCMoneyRegion;
extern	UINT32					guiMapInvSecondHandBlockout;

//jackaians: global variables added for the stealing pickup menu
BOOLEAN				gfStealing = FALSE;
SOLDIERTYPE			*gpOpponent = NULL;

MOUSE_REGION				gInvDesc;

OBJECTTYPE		*gpItemPointer = NULL;
extern OBJECTTYPE		gItemPointer; // symbol already defined in mapscreen.cpp (jonathanl)
BOOLEAN				gfItemPointerDifferentThanDefault = FALSE;
extern SOLDIERTYPE		*gpItemPointerSoldier; // symbol already defined in mapscreen.cpp (jonathanl)
INT8					gbItemPointerSrcSlot;
UINT16				gusItemPointer = 255;
UINT16				usItemSnapCursor;
UINT32				guiNewlyPlacedItemTimer = 0;
BOOLEAN				gfBadThrowItemCTGH;
BOOLEAN				gfDontChargeAPsToPickup = FALSE;
BOOLEAN				gbItemPointerLocateGood = FALSE;
INT32				iItemPosition = 0;
//CHRISL: Global variable to store stack item for message box system
UINT8				gbMessageBoxSubObject = 0;

// ITEM DESCRIPTION BOX STUFF
UINT32			guiItemDescBox;
UINT32			guiItemDescBoxBackground;
// HEADROCK HAM 4: Image file for LBE backgrounds
UINT32		guiItemInfoLBEBackground;
UINT32      guiMapItemDescBox;
UINT32			guiAttachmentSlot;

// HEADROCK HAM 4: This replaces "Misc" icons.
extern UINT32			guiItemInfoAdvancedIcon;	// added here by Flugente

UINT32			guiItemGraphic;
UINT32			guiMoneyGraphicsForDescBox;
UINT32			guiBullet;
// HEADROCK HAM 5: Icon indicating a transformation is available.
UINT32			guiTransformIconGraphic;
BOOLEAN			gfInItemDescBox = FALSE;
UINT32			guiCurrentItemDescriptionScreen=0;
OBJECTTYPE	*gpItemDescObject = NULL;
// HEADROCK HAM 4: Remembers the object that was open before an attachment desc is opened on top of it.
OBJECTTYPE  *gpItemDescPrevObject = NULL;
// HEADROCK HAM 5: This is a clone, used to contain an item's details when the item has been destroyed while the box is open.
OBJECTTYPE	gCloneItemDescObject;
BOOLEAN			gfItemDescObjectIsAttachment = FALSE;
OBJECTTYPE	*gpItemDescOrigAttachmentObject = NULL;

// HEADROCK HAM 5: Used by the callback confirming whether we want to transform.
TransformInfoStruct * gTransformInProgress;
UINT32 guiTransformInProgressPrevScreen;

CHAR16			gzItemName[ SIZE_ITEM_NAME ];
CHAR16			gzItemDesc[ SIZE_ITEM_INFO ];
CHAR16			gzItemPros[ SIZE_ITEM_PROS ];
CHAR16			gzItemCons[ SIZE_ITEM_CONS ];
CHAR16			gzFullItemPros[ SIZE_ITEM_PROS ];
CHAR16			gzFullItemCons[ SIZE_ITEM_PROS ];
CHAR16			gzFullItemTemp[ SIZE_ITEM_PROS ]; // necessary, unfortunately
//CHRISL: moved to Interface Items.h for EDB
//void ItemDescCallback( MOUSE_REGION * pRegion, INT32 iReason );
INT16				gsInvDescX;
INT16				gsInvDescY;
UINT8				gubItemDescStatusIndex;
INT32				giItemDescAmmoButtonImages;
INT32				giItemDescAmmoButton;
BOOLEAN			gfItemAmmoDown = FALSE;
SOLDIERTYPE *gpItemDescSoldier;
BOOLEAN			fItemDescDelete = FALSE;
MOUSE_REGION		gItemDescAttachmentRegions[MAX_ATTACHMENTS];
MOUSE_REGION		gProsAndConsRegions[2];

// BOB : quick attachment popups
BOOLEAN		gItemDescAttachmentPopupsInitialized = FALSE;

POPUP*		gItemDescAttachmentPopups[MAX_ATTACHMENTS];
INT32		giActiveAttachmentPopup = -1;

POPUP*		gEquipPopups[NUM_INV_SLOTS];
INT32		giActiveEquipPopup = -1;

// HEADROCK HAM 5: Item Transformation Popups
BOOLEAN		gfItemDescTransformPopupInitialized = FALSE;
POPUP*		gItemDescTransformPopup;
BOOLEAN		gfItemDescTransformPopupVisible = FALSE;
BOOLEAN		gfSkipDestroyTransformPopup = FALSE;	// This makes sure the 
// And forward declarations...
void TransformationMenuPopup_Hide(void);
void TransformationMenuPopup_Transform(TransformInfoStruct * Transform);
BOOLEAN TransformationMenuPopup_TestValid(TransformInfoStruct * Transform);
void ItemDescTransformRegionCallback( MOUSE_REGION *pRegion, INT32 reason );
void TransformationMenuPopup_Unjam();
void TransformationMenuPopup_SplitCrate( UINT16 usMagazineItem );
void TransformationMenuPopup_SplitCrateInInventory( );
void TransformFromItemDescBox( TransformInfoStruct * Transform);
void ConfirmTransformationMessageBoxCallBack( UINT8 bExitValue );

// HEADROCK HAM 5: The maximum number of attachment asterisks shown for an item.
UINT32 guiAttachmentAsterisks;
#define MAX_NUM_ASTERISKS 10

void				BtnMoneyButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32			guiMoneyButtonBtn[OLD_MAX_ATTACHMENTS_101];
INT32				guiMoneyButtonImage;
INT32				guiMoneyDoneButtonImage;

attachmentList	gOriginalAttachments;
SOLDIERTYPE * gpAttachSoldier;
extern BOOLEAN	gfSMDisableForItems;

//CHRISL: moved to Interface Items.h for EDB
/*typedef struct
{
	UINT16 x;
	UINT16 y;
} MoneyLoc;*/

/*
 * Yes i know thats a little bit compicated but remeber that we have now only tactical screen
 * a little bit dynamic and customizable.
 * gMoneyButtonLoc its a location of money button
 * we need to initialize it in run-time becouse we dona have constant initializer
 * any questions? joker
 */
MoneyLoc			gMoneyButtonLoc;// =			{ (343 + INTERFACE_START_Y), ( 11 + INV_INTERFACE_START_Y ) };
MoneyLoc			gMoneyButtonOffsets[] = { 0,0,  34,0,  0,32, 34,32, 8,22 };

// show the description
extern BOOLEAN fShowDescriptionFlag;
extern BOOLEAN fShowInventoryFlag;

void ItemDescAttachmentsCallback( MOUSE_REGION * pRegion, INT32 iReason );
void ItemDescAmmoCallback(GUI_BUTTON *btn,INT32 reason);
void DeletePool(ITEM_POOL *pItemPool);

// number of keys on keyring, temp for now
#define			NUMBER_KEYS_ON_KEYRING 28
#define			KEY_RING_ROW_WIDTH 7
#define			MAP_KEY_RING_ROW_WIDTH 4
#define			INV_ITEM_ROW_WIDTH 7
#define			MAP_INV_ITEM_ROW_WIDTH 4

// ITEM STACK POPUP STUFF
BOOLEAN			gfInItemStackPopup = FALSE;
BOOLEAN			gfInSectorStackPopup = FALSE;
UINT32			guiItemPopupBoxes;
OBJECTTYPE	*gpItemPopupObject;
INT16				gsItemPopupWidth;
INT16				gsItemPopupHeight;
INT16				gsItemPopupX;
INT16				gsItemPopupY;
MOUSE_REGION				gItemPopupRegions[MAX_OBJECTS_PER_SLOT];
MOUSE_REGION				gKeyRingRegions[ NUMBER_KEYS_ON_KEYRING ];
BOOLEAN							gfInKeyRingPopup = FALSE;
UINT8								gubNumItemPopups = 0;
MOUSE_REGION				gItemPopupRegion;
INT16				gsItemPopupInvX;
INT16				gsItemPopupInvY;
INT16				gsItemPopupInvWidth;
INT16				gsItemPopupInvHeight;

INT16				gsKeyRingPopupInvX;
INT16				gsKeyRingPopupInvY;
INT16				gsKeyRingPopupInvWidth;
INT16				gsKeyRingPopupInvHeight;


SOLDIERTYPE *gpItemPopupSoldier;
extern BOOLEAN fMapScreenBottomDirty;

// inventory description done button for mapscreen
INT32 giMapInvDescButtonImage;
INT32 giMapInvDescButton = -1;

// HEADROCK HAM 4: Item Description Box TAB buttons for UDB
INT32 giMapInvDescTabButtonImage;
INT32 giInvDescTabButtonImage;
INT32 giInvDescTabButton[3] = {-1, -1, -1};
void ItemDescTabButtonCallback( GUI_BUTTON *btn, INT32 reason );
void ItemDescTabButtonOn( UINT8 ubItemDescTabButtonIndex );
void ItemDescTabButtonOff( UINT8 ubItemDescTabButtonIndex );
// HEADROCK HAM 4: Item Description Box Scroll Buttons for UDB
INT32 giInvDescAdvButtonUpImage;
INT32 giInvDescAdvButtonDownImage;
INT32 giInvDescAdvButton[2] = {-1, -1};
// HEADROCK HAM 5: Item Transformation click region.
MOUSE_REGION gInvDescTransformRegion;
void ItemDescAdvButtonCallback( GUI_BUTTON *btn, INT32 reason );
void ItemDescAdvButtonOn( UINT8 ubItemDescAdvButtonIndex );
void ItemDescAdvButtonOff( UINT8 ubItemDescAdvButtonIndex );
void ItemDescAdvButtonCheck( void );
// HEADROCK HAM 5: Item Adjustment Button(s)
void ItemDescTransformRegionCallback( MOUSE_REGION *pRegion, INT32 reason );

// the done descrition button callback
void ItemDescDoneButtonCallback( GUI_BUTTON *btn, INT32 reason );


extern BOOLEAN fMapInventoryItem;
BOOLEAN	gfItemPopupRegionCallbackEndFix = FALSE;
extern void InternalMAPBeginItemPointer( SOLDIERTYPE *pSoldier );


void	ItemPopupRegionCallback( MOUSE_REGION * pRegion, INT32 iReason );
void ItemPopupFullRegionCallback( MOUSE_REGION * pRegion, INT32 iReason );
BOOLEAN ReloadItemDesc( );


extern void HelpTextDoneCallback( void );
void RemoveMoney();
BOOLEAN	CompatibleItemForApplyingOnMerc( OBJECTTYPE *pTestObject );

extern BOOLEAN MAPInternalInitItemDescriptionBox( OBJECTTYPE *pObject, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier );
extern void	StartSKIDescriptionBox( void );

void UpdateItemHatches();

void ShadowNIVPanel();

extern void BeginInventoryPoolPtr( OBJECTTYPE *pInventorySlot );

UINT8		ubRGBItemCyclePlacedItemColors[] =
{
	25,		25,		25,
	50,		50,		50,
	75,   75,   75,
	100,	100,	100,
	125,	125,	125,
	150,	150,	150,
	175,	175,	175,
	200,	200,	200,
	225,	225,	225,
	250,	250,	250,

	250,	250,	250,
	225,	225,	225,
	200,	200,	200,
	175,	175,	175,
	150,	150,	150,
	125,	125,	125,
	100,	100,	100,
	75,   75,   75,
	50,		50,		50,
	25,		25,		25

};

//CHRISL: Moved to interface items.h
/*typedef struct
{
	INT16		sX;
	INT16		sY;
	INT16		sValDx;

} INV_DESC_STATS;*/

//CHRISL: Moved to interface items.h
/*typedef struct
{
	INT16		sX;
	INT16		sY;
	INT16		sHeight;
	INT16		sWidth;
	INT16		sBarDx;
	INT16		sBarDy;

} INV_ATTACHXY;*/

#define NUM_INV_HELPTEXT_ENTRIES 1

typedef struct
{
	INT32 iXPosition[NUM_INV_HELPTEXT_ENTRIES];
	INT32 iYPosition[NUM_INV_HELPTEXT_ENTRIES];
	INT32 iWidth[NUM_INV_HELPTEXT_ENTRIES];
	STR16 sString1[NUM_INV_HELPTEXT_ENTRIES];
	STR16 sString2[NUM_INV_HELPTEXT_ENTRIES];
} INV_HELPTEXT;

//CHRISL: Moved declaration to InitDescStatCoords for EDB
// displayed AFTER the mass/weight/"Kg" line
INV_DESC_STATS gMoneyStats[6];

//CHRISL: Created for LBENODE Rendering with EDB
INV_DESC_STATS gLBEStats[48];

INV_ATTACHXY	gItemDescAttachmentsXY[MAX_ATTACHMENTS];

INV_HELPTEXT gItemDescHelpText =
{
	{ 69 }, // x locations
	{ 12 }, // y locations
	{ 170 }, // widths
	{ Message[STR_ATTACHMENT_HELP] },
	{ Message[STR_ATTACHMENT_INVALID_HELP] },
};

BOOLEAN gfItemDescHelpTextOffset = FALSE;

// ARRAY FOR INV PANEL INTERFACE ITEM POSITIONS (sX,sY get set via InitInvSlotInterface() )
/* CHRISL: Add new region definitions for the new inventory slots needed for the new inventory
system to work properly. */
INV_REGIONS gSMInvData[NUM_INV_SLOTS];

//CHRISL: Moved to interface items.h
/*typedef struct
{
	UINT32	uiTotalAmount;
	UINT32	uiMoneyRemaining;
	UINT32	uiMoneyRemoving;

} REMOVE_MONEY;*/
REMOVE_MONEY gRemoveMoney;

MOUSE_REGION		gSMInvRegion[ NUM_INV_SLOTS ];
MOUSE_REGION		gKeyRingPanel;
MOUSE_REGION		gSMInvCamoRegion;
INT8						gbCompatibleAmmo[ NUM_INV_SLOTS ];
INT8						gbInvalidPlacementSlot[ NUM_INV_SLOTS ];
UINT16					us16BPPItemCyclePlacedItemColors[ 20 ];
// Kaiden: Vehicle Inventory change - Increase this from 4-2 to 5-2
UINT32					guiBodyInvVO[ 5 ][ 2 ];
UINT32					guiGoldKeyVO;
INT8						gbCompatibleApplyItem = FALSE;

// HEADROCK HAM 4: New region coordinate tables for UDB
INV_DESC_REGIONS gItemDescLBEBackground[4]; // Coordinates for displaying LBE background image
INV_DESC_REGIONS gItemDescGenHeaderRegions[3]; // Header text regions for various parts of the General Tab
INV_DESC_REGIONS gItemDescGenIndexRegions[3][4]; // Index text regions for various parts of the General Tab
INV_DESC_REGIONS gItemDescGenRegions[NUM_UDB_GEN_LINES * 2][4]; // Data regions, 4 sub-columns each
INV_DESC_REGIONS gItemDescGenSecondaryRegions[26]; // Secondary data regions, 3x5
INV_DESC_REGIONS gItemDescTextRegions[8]; // Main description regions
INV_DESC_REGIONS gItemDescAdvIndexRegions[1][4];
INV_DESC_REGIONS gItemDescAdvRegions[NUM_UDB_ADV_LINES][4]; // Advanced data regions, 4 sub-columns each

INV_DESC_REGIONS gODBItemDescRegions[4][8]; // Four regions of eight sub-regions each.

// ------------------- Attachment popup callbacks

// BOB : globals for telling attachment popups _where_ should the attachment go
UINT8	gubPopupStatusIndex;
UINT32	guiPopupItemPos;
void popupCallbackItem(INT16 itemId){


	for(UINT16 i = 0; i < pInventoryPoolList.size(); i++){
		if( pInventoryPoolList[i].object.usItem == itemId ) {
			
			gpItemPointer = &pInventoryPoolList[i].object;				// pick up the object (or stack)
			DoAttachment((UINT8)gubPopupStatusIndex, guiPopupItemPos);	// try to attach it
			gpItemPointer = NULL;										// and drop it

			gItemDescAttachmentPopups[giActiveAttachmentPopup]->hide();
			RenderItemDescriptionBox();
			giActiveAttachmentPopup = -1;

			//UpdateAttachmentTooltips(gpItemDescObject, gubItemDescStatusIndex);
			return;
			
		}
	}

}

bool popupCallbackItemInSector(INT16 itemId){
	for(UINT16 i = 0; i < pInventoryPoolList.size(); i++)
		if( pInventoryPoolList[i].object.usItem == itemId ) return true;

	return false;
}

void hideAttachmentPopup(){
	giActiveAttachmentPopup = -1;

	if (giItemDescAmmoButton > -1)
	{
		if (giInvDescTabButton[0] != giItemDescAmmoButton)	
			ShowButton(giItemDescAmmoButton);
	}
}

void hideOtherAttachmentPopups(UINT32 cnt){

	// if there's a bullet icon, hide it. It tends to overlap my popup boxes
	if (giItemDescAmmoButton != -1)
	{
		if (giInvDescTabButton[0] != giItemDescAmmoButton)	
			HideButton(giItemDescAmmoButton);
	}

	RenderItemDescriptionBox();	// also, redraw the IDB to clean up helptext
	
	if( cnt > MAX_ATTACHMENTS  ) return;

	for(UINT32 i = 0; i < cnt; i++){
		if (gItemDescAttachmentPopups[i] != NULL)
			gItemDescAttachmentPopups[i]->hide();
	}
}



BOOLEAN AttemptToAddSubstring( STR16 zDest, STR16 zTemp, UINT32 * puiStringLength, UINT32 uiPixLimit )
{
	UINT32 uiRequiredStringLength, uiTempStringLength;

	uiTempStringLength = StringPixLength( zTemp, ITEMDESC_FONT );
	uiRequiredStringLength = *puiStringLength + uiTempStringLength;
	if (zDest[0] != 0)
	{
		uiRequiredStringLength += StringPixLength( COMMA_AND_SPACE, ITEMDESC_FONT );
	}
	if (uiRequiredStringLength < uiPixLimit)
	{
		if (zDest[0] != 0)
		{
			wcscat( zDest, COMMA_AND_SPACE );
		}
		wcscat( zDest, zTemp );
		*puiStringLength = uiRequiredStringLength;
		return( TRUE );
	}
	else
	{
		wcscat( zDest, DOTDOTDOT );
		return( FALSE );
	}
}

void GenerateProsString( STR16 zItemPros, OBJECTTYPE * pObject, UINT32 uiPixLimit )
{
	UINT32			uiStringLength = 0;
	STR16		zTemp;
	UINT16			usItem = pObject->usItem;
	UINT16				ubWeight;

	zItemPros[0] = 0;

	ubWeight = Item[ usItem ].ubWeight;
	if (Item[ usItem ].usItemClass == IC_GUN)
	{
		ubWeight = ubWeight + Item[ (*pObject)[0]->data.gun.usGunAmmoItem ].ubWeight;
	}

	// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
	FLOAT accuracyheatmultiplicator = 1.0;
	if ( gGameOptions.fWeaponOverheating )
	{
		FLOAT overheatdamagepercentage = GetGunOverheatDamagePercentage( pObject );
		FLOAT accuracymalus = (FLOAT)((max(1.0, overheatdamagepercentage) - 1.0) * 0.1);
		accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
	}

	//CHRISL: TODO - This needs to be updated for NCTH
	if (accuracyheatmultiplicator * Weapon[usItem].bAccuracy >= EXCEPTIONAL_ACCURACY )
	{
		zTemp = Message[STR_ACCURATE];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (Item[usItem].ubWeight <= EXCEPTIONAL_WEIGHT)
	{
		zTemp = Message[STR_LIGHT];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	//CHRISL: Using "FitsInSmallPocket" in NewInv will result in every item appearing to be "small".  We don't want that so
	// so in this case, even in NewInv, we should look at the items ubPerPocket value.
	//if (FitsInSmallPocket(pObject) == true) // fits in a small pocket
	if (Item[usItem].ubPerPocket >= 1) // fits in a small pocket
	{
		zTemp = Message[STR_SMALL];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	UINT16 exceptionalRange = (UINT16)((EXCEPTIONAL_RANGE * gGameExternalOptions.iGunRangeModifier ) / 100);

	if ( GunRange( pObject, NULL ) >= exceptionalRange ) // SANDRO - added argument
	{
		zTemp = Message[STR_LONG_RANGE];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	UINT16 exceptionalDamage;
	// Melee damage
	if ( Item[pObject->usItem].usItemClass == IC_BLADE || Item[pObject->usItem].usItemClass == IC_PUNCH || Item[pObject->usItem].usItemClass == IC_TENTACLES )
	{
		exceptionalDamage = (UINT16)(( EXCEPTIONAL_MELEE_DAMAGE * gGameExternalOptions.iMeleeDamageModifier ) / 100);
	}
	// Gun damage
	else
	{
		exceptionalDamage = (UINT16)(( EXCEPTIONAL_GUN_DAMAGE * gGameExternalOptions.iGunDamageModifier ) / 100);
	}

	if (GetDamage(pObject) >= exceptionalDamage)
	{
		zTemp = Message[STR_HIGH_DAMAGE];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject, NULL ) <= EXCEPTIONAL_AP_COST)
	{
		zTemp = Message[STR_QUICK_FIRING];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetShotsPerBurst(pObject) >= EXCEPTIONAL_BURST_SIZE )//|| usItem == G11)
	{
		zTemp = Message[STR_FAST_BURST];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetMagSize(pObject) > EXCEPTIONAL_MAGAZINE)
	{
		zTemp = Message[STR_LARGE_AMMO_CAPACITY];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( GetReliability( pObject ) >= EXCEPTIONAL_RELIABILITY )
	{
		zTemp = Message[STR_RELIABLE];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( Item[usItem].bRepairEase >= EXCEPTIONAL_REPAIR_EASE )
	{
		zTemp = Message[STR_EASY_TO_REPAIR];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetAutofireShotsPerFiveAPs(pObject) > 0)
	{
		zTemp = Message[STR_AUTOFIRE];
		if ( ! AttemptToAddSubstring( zItemPros, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( zItemPros[0] == 0 )
	{
		// empty string, so display "None"
		if ( ! AttemptToAddSubstring( zItemPros, Message[ STR_NONE ], &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}
}

void GenerateConsString( STR16 zItemCons, OBJECTTYPE * pObject, UINT32 uiPixLimit )
{
	UINT32			uiStringLength = 0;
	STR16		zTemp;
	UINT16				ubWeight;
	UINT16			usItem = pObject->usItem;

	zItemCons[0] = 0;

	// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
	FLOAT accuracyheatmultiplicator = 1.0;
	if ( gGameOptions.fWeaponOverheating )
	{
		FLOAT overheatdamagepercentage = GetGunOverheatDamagePercentage( pObject );
		FLOAT accuracymalus = (FLOAT)((max(1.0, overheatdamagepercentage) - 1.0) * 0.1);
		accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
	}

	//CHRISL: TODO - This needs to be updated for NCTH
	if (accuracyheatmultiplicator * Weapon[usItem].bAccuracy <= BAD_ACCURACY)
	{
		zTemp = Message[STR_INACCURATE];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( Weapon[usItem].NoSemiAuto )
	{
		zTemp = Message[STR_NO_SEMI_AUTO];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	// calculate the weight of the item plus ammunition but not including any attachments
	ubWeight = Item[ usItem ].ubWeight;
	if (Item[ usItem ].usItemClass == IC_GUN)
	{
		ubWeight = ubWeight + CalculateAmmoWeight((*pObject)[0]->data.gun.usGunAmmoItem, (*pObject)[0]->data.gun.ubGunShotsLeft);
		//ubWeight = ubWeight + Item[ (*pObject)[0]->data.gun.usGunAmmoItem ].ubWeight;
	}

	if (ubWeight >= BAD_WEIGHT)
	{
		zTemp = Message[STR_HEAVY];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( GunRange( pObject, NULL ) <= BAD_RANGE) // SANDRO - added argument
	{
		zTemp = Message[STR_SHORT_RANGE];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}


	if (GetDamage(pObject) <= BAD_DAMAGE)
	{
		zTemp = Message[STR_LOW_DAMAGE];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject, NULL ) >= APBPConstants[BAD_AP_COST])
	{
		zTemp = Message[STR_SLOW_FIRING];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetShotsPerBurst(pObject) == 0)
	{
		zTemp = Message[STR_NO_BURST];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetAutofireShotsPerFiveAPs(pObject) == 0)
	{
		zTemp = Message[STR_NO_AUTOFIRE];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (GetMagSize(pObject) < BAD_MAGAZINE)
	{
		zTemp = Message[STR_SMALL_AMMO_CAPACITY];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if (  GetReliability( pObject ) <= BAD_RELIABILITY )
	{
		zTemp = Message[STR_UNRELIABLE];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}

	if ( Item[usItem].bRepairEase <= BAD_REPAIR_EASE )
	{
		zTemp = Message[STR_HARD_TO_REPAIR];
		if ( ! AttemptToAddSubstring( zItemCons, zTemp, &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}


	if ( zItemCons[0] == 0 )
	{
		// empty string, so display "None"
		if ( ! AttemptToAddSubstring( zItemCons, Message[ STR_NONE ], &uiStringLength, uiPixLimit ) )
		{
			return;
		}
	}
}

BOOLEAN UseNASDesc(OBJECTTYPE *pObject){
	if(pObject->exists() == FALSE)
		return FALSE;
	if(guiCurrentScreen == MAP_SCREEN && Item[pObject->usItem].usItemClass == IC_LBEGEAR && UsingNewAttachmentSystem()==true)
		return FALSE;	// the map screen can't support NAS and LBEGEAR.
	return (/*Item[pObject->usItem].usItemClass != IC_LBEGEAR && Item[pObject->usItem].usItemClass != IC_MONEY && */UsingNewAttachmentSystem()==true);
}

//WarmSteel - This function is used to move groups of items with NAS along the Y axe
INT16 sNASYCorrection(OBJECTTYPE * pObject){

	//WarmSteel - If this confuses you, it's basically the same structure as in InitDescStatCoords.
	if(UseNASDesc(pObject)){
		if(UsingEDBSystem() > 0)
		{
			if( guiCurrentScreen == MAP_SCREEN )
			{
				return 113;
			}
			else
			{
				if (UsingEDBSystem() == 1) // NIV EDB
				{
					return 0;
				}
				if (UsingEDBSystem() == 2) // OIV EDB
				{
					return 0;
				}
			}
		}
		else
		{
			if(guiCurrentScreen ==MAP_SCREEN)
			{
				return 113;
			}
			else
			{
				return 0;
			}
		}
	}
	//WarmSteel - Item does not qualify for NAS layout.
	return 0;
}
//WarmSteel - This function is used to move groups of items with NAS along the X axe
INT16 sNASXCorrection(OBJECTTYPE * pObject){

	//WarmSteel - If this confuses you, it's basically the same structure as in InitDescStatCoords.
	if(UseNASDesc(pObject)){
		if(UsingEDBSystem() > 0)
		{
			if( guiCurrentScreen == MAP_SCREEN )
			{
				return 0;
			}
			else
			{
				if (UsingEDBSystem()) // NIV EDB
				{
					return 190;
				}
			}
		}
		else //ODB
		{
			if(guiCurrentScreen ==MAP_SCREEN)
			{
				return 0;
			}
			else
			{
				return 90;
			}
		}
	}
	//WarmSteel - Item does not qualify for NAS layout.
	return 0;
}

// CHRISL: New function to setup GSMInvData based on game options
void InitInvData(INV_REGIONS &InvData, BOOLEAN fBigPocket, INT16 sBarDx, INT16 sBarDy, INT16 sWidth, INT16 sHeight, INT16 sX, INT16 sY)
{
	InvData.fBigPocket = fBigPocket;
	InvData.sBarDx = sBarDx;
	InvData.sBarDy = sBarDy;
	InvData.sHeight = sHeight;
	InvData.sWidth = sWidth;
	InvData.sX = sX;
	InvData.sY = sY;
}

void ResetInvData()
{
	for ( INT32 cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		// set inventory pocket coordinates from the table passed in
		gSMInvData[ cnt ].sX = 0;
		gSMInvData[ cnt ].sY =0;
		gSMInvData[ cnt ].sWidth =0;
		gSMInvData[ cnt ].sHeight =0;
		gSMInvData[ cnt ].sBarDx =0;
		gSMInvData[ cnt ].sBarDy =0;
		gSMInvData[ cnt ].fBigPocket =0;
	}
}

void InitInventoryOld()
{
	BODYPOSFINAL		= GUNSLINGPOCKPOS;//RESET in initInventory
	BIGPOCKFINAL		= BIGPOCK5POS;//RESET in initInventory
	MEDPOCKSTART		= SMALLPOCK1POS;//RESET in initInventory
	MEDPOCKFINAL		= SMALLPOCK1POS;//RESET in initInventory
	SMALLPOCKFINAL		= SMALLPOCK9POS;//RESET in initInventory

	ResetInvData();


	InitInvData(gSMInvData[HELMETPOS],		FALSE,	INV_BAR_DX,	INV_BAR_DY,	HEAD_INV_SLOT_WIDTH,	HEAD_INV_SLOT_HEIGHT,	0, 0);	// HELMETPOS
	InitInvData(gSMInvData[VESTPOS],		FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// VESTPOS
	InitInvData(gSMInvData[LEGPOS],			FALSE,	INV_BAR_DX,	INV_BAR_DY,	LEGS_INV_SLOT_WIDTH,	LEGS_INV_SLOT_HEIGHT,	0, 0);	// LEGPOS
	InitInvData(gSMInvData[HEAD1POS],		FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD1POS
	InitInvData(gSMInvData[HEAD2POS],		FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD2POS
	InitInvData(gSMInvData[HANDPOS],		TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// HANDPOS
	InitInvData(gSMInvData[SECONDHANDPOS],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// SECONDHANDPOS
	InitInvData(gSMInvData[BIGPOCK1POS],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK1
	InitInvData(gSMInvData[BIGPOCK2POS],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK2
	InitInvData(gSMInvData[BIGPOCK3POS],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK3
	InitInvData(gSMInvData[BIGPOCK4POS],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK4
	InitInvData(gSMInvData[SMALLPOCK1POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK1
	InitInvData(gSMInvData[SMALLPOCK2POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK2
	InitInvData(gSMInvData[SMALLPOCK3POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK3
	InitInvData(gSMInvData[SMALLPOCK4POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK4
	InitInvData(gSMInvData[SMALLPOCK5POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK5
	InitInvData(gSMInvData[SMALLPOCK6POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK6
	InitInvData(gSMInvData[SMALLPOCK7POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK7
	InitInvData(gSMInvData[SMALLPOCK8POS],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK8
}
void InitInventoryNew()
{
	BODYPOSFINAL		= GUNSLINGPOCKPOS;//RESET in initInventory
	BIGPOCKFINAL		= MEDPOCK1POS;//RESET in initInventory
	MEDPOCKSTART		= MEDPOCK1POS;//RESET in initInventory
	MEDPOCKFINAL		= SMALLPOCK1POS;//RESET in initInventory
	SMALLPOCKFINAL		= NUM_INV_SLOTS;//RESET in initInventory

	ResetInvData();

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		InitInvData(gSMInvData[0],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	HEAD_INV_SLOT_WIDTH,	HEAD_INV_SLOT_HEIGHT,	0, 0);	// HELMETPOS
		InitInvData(gSMInvData[1],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// VESTPOS
		InitInvData(gSMInvData[2],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	LEGS_INV_SLOT_WIDTH,	LEGS_INV_SLOT_HEIGHT,	0, 0);	// LEGPOS
		InitInvData(gSMInvData[3],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD1POS
		InitInvData(gSMInvData[4],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD2POS
		InitInvData(gSMInvData[5],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// HANDPOS
		InitInvData(gSMInvData[6],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// SECONDHANDPOS
		InitInvData(gSMInvData[7],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// VESTPOCKPOS
		InitInvData(gSMInvData[8],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// LTHIGHPOCKPOS
		InitInvData(gSMInvData[9],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// RTHIGHPOCKPOS
		InitInvData(gSMInvData[10],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// CPACKPOCKPOS
		InitInvData(gSMInvData[11],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// BPACKPOCKPOS
		InitInvData(gSMInvData[12],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// GUNSLINGPOCKPOS
		InitInvData(gSMInvData[13],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// KNIFEPOCKPOS
		InitInvData(gSMInvData[14],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK1
		InitInvData(gSMInvData[15],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK2
		InitInvData(gSMInvData[16],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK3
		InitInvData(gSMInvData[17],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK4
		InitInvData(gSMInvData[18],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK5
		InitInvData(gSMInvData[19],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK6
		InitInvData(gSMInvData[20],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	48,	20,	0, 0);	// BIGPOCK7
		InitInvData(gSMInvData[21],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// MEDPOCK1
		InitInvData(gSMInvData[22],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// MEDPOCK2
		InitInvData(gSMInvData[23],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// MEDPOCK3
		InitInvData(gSMInvData[24],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	30,	20,	0, 0);	// MEDPOCK4
		InitInvData(gSMInvData[25],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK1
		InitInvData(gSMInvData[26],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK2
		InitInvData(gSMInvData[27],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK3
		InitInvData(gSMInvData[28],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK4
		InitInvData(gSMInvData[29],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK5
		InitInvData(gSMInvData[30],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK6
		InitInvData(gSMInvData[31],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK7
		InitInvData(gSMInvData[32],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK8
		InitInvData(gSMInvData[33],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK9
		InitInvData(gSMInvData[34],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK10
		InitInvData(gSMInvData[35],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK11
		InitInvData(gSMInvData[36],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK12
		InitInvData(gSMInvData[37],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK13
		InitInvData(gSMInvData[38],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK14
		InitInvData(gSMInvData[39],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK15
		InitInvData(gSMInvData[40],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK16
		InitInvData(gSMInvData[41],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK17
		InitInvData(gSMInvData[42],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK18
		InitInvData(gSMInvData[43],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK19
		InitInvData(gSMInvData[44],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK20
		InitInvData(gSMInvData[45],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK21
		InitInvData(gSMInvData[46],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK22
		InitInvData(gSMInvData[47],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK23
		InitInvData(gSMInvData[48],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK24
		InitInvData(gSMInvData[49],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK25
		InitInvData(gSMInvData[50],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK26
		InitInvData(gSMInvData[51],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK27
		InitInvData(gSMInvData[52],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK28
		InitInvData(gSMInvData[52],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK29
		InitInvData(gSMInvData[54],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	17,	20,	0, 0);	// SMALLPOCK30
	}
	else{
		InitInvData(gSMInvData[0],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	HEAD_INV_SLOT_WIDTH,	HEAD_INV_SLOT_HEIGHT,	0, 0);	// HELMETPOS
		InitInvData(gSMInvData[1],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// VESTPOS
		InitInvData(gSMInvData[2],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	LEGS_INV_SLOT_WIDTH,	LEGS_INV_SLOT_HEIGHT,	0, 0);	// LEGPOS
		InitInvData(gSMInvData[3],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD1POS
		InitInvData(gSMInvData[4],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// HEAD2POS
		InitInvData(gSMInvData[5],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// HANDPOS
		InitInvData(gSMInvData[6],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// SECONDHANDPOS
		InitInvData(gSMInvData[7],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// VESTPOCKPOS
		InitInvData(gSMInvData[8],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// LTHIGHPOCKPOS
		InitInvData(gSMInvData[9],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// RTHIGHPOCKPOS
		InitInvData(gSMInvData[10],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// CPACKPOCKPOS
		InitInvData(gSMInvData[11],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// BPACKPOCKPOS
		InitInvData(gSMInvData[12],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// GUNSLINGPOCKPOS
		InitInvData(gSMInvData[13],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// KNIFEPOCKPOS
		InitInvData(gSMInvData[14],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK1
		InitInvData(gSMInvData[15],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK2
		InitInvData(gSMInvData[16],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK3
		InitInvData(gSMInvData[17],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK4
		InitInvData(gSMInvData[18],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK5
		InitInvData(gSMInvData[19],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK6
		InitInvData(gSMInvData[20],	TRUE,	INV_BAR_DX,	INV_BAR_DY,	BIG_INV_SLOT_WIDTH,		BIG_INV_SLOT_HEIGHT,	0, 0);	// BIGPOCK7
		InitInvData(gSMInvData[21],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// MEDPOCK1
		InitInvData(gSMInvData[22],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// MEDPOCK2
		InitInvData(gSMInvData[23],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// MEDPOCK3
		InitInvData(gSMInvData[24],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	VEST_INV_SLOT_WIDTH,	VEST_INV_SLOT_HEIGHT,	0, 0);	// MEDPOCK4
		InitInvData(gSMInvData[25],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK1
		InitInvData(gSMInvData[26],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK2
		InitInvData(gSMInvData[27],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK3
		InitInvData(gSMInvData[28],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK4
		InitInvData(gSMInvData[29],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK5
		InitInvData(gSMInvData[30],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK6
		InitInvData(gSMInvData[31],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK7
		InitInvData(gSMInvData[32],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK8
		InitInvData(gSMInvData[33],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK9
		InitInvData(gSMInvData[34],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK10
		InitInvData(gSMInvData[35],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK11
		InitInvData(gSMInvData[36],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK12
		InitInvData(gSMInvData[37],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK13
		InitInvData(gSMInvData[38],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK14
		InitInvData(gSMInvData[39],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK15
		InitInvData(gSMInvData[40],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK16
		InitInvData(gSMInvData[41],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK17
		InitInvData(gSMInvData[42],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK18
		InitInvData(gSMInvData[43],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK19
		InitInvData(gSMInvData[44],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK20
		InitInvData(gSMInvData[45],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK21
		InitInvData(gSMInvData[46],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK22
		InitInvData(gSMInvData[47],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK23
		InitInvData(gSMInvData[48],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK24
		InitInvData(gSMInvData[49],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK25
		InitInvData(gSMInvData[50],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK26
		InitInvData(gSMInvData[51],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK27
		InitInvData(gSMInvData[52],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK28
		InitInvData(gSMInvData[53],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK29
		InitInvData(gSMInvData[54],	FALSE,	INV_BAR_DX,	INV_BAR_DY,	SM_INV_SLOT_WIDTH,		SM_INV_SLOT_HEIGHT,		0, 0);	// SMALLPOCK30
	}
}
void InitInventoryVehicle(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity)
{
	for(int cnt=INV_START_POS; cnt<NUM_INV_SLOTS; cnt++)
	{
		MSYS_RemoveRegion( &gSMInvRegion[ cnt ]);
		if(vehicleInv[cnt])
			InitInvData(gSMInvData[cnt], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, pRegionDesc[cnt].sX, pRegionDesc[cnt].sY);
		else
			InitInvData(gSMInvData[cnt], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, 0, 0);
		MSYS_DefineRegion( &gSMInvRegion[ cnt ], gSMInvData[ cnt ].sX, gSMInvData[ cnt ].sY, (INT16)(gSMInvData[ cnt ].sX + gSMInvData[ cnt ].sWidth), (INT16)(gSMInvData[ cnt ].sY + gSMInvData[ cnt ].sHeight), ( INT8 )( fSetHighestPrioity ? MSYS_PRIORITY_HIGHEST : MSYS_PRIORITY_HIGH ),
							 MSYS_NO_CURSOR, INVMoveCallback, INVClickCallback ); 
		// Add region
		MSYS_AddRegion( &gSMInvRegion[ cnt ] );
		MSYS_SetRegionUserData( &gSMInvRegion[ cnt ], 0, cnt );
	}
}
// CHRISL: Function to recreate inventory mouse regions
void InitInventorySoldier(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity, BOOLEAN fNIVmode)
{
	if(fNIVmode == TRUE)
		InitInventoryNew();
	else
	{
		if(gSMInvData[0].sHeight == 0 && gSMInvData[0].sWidth == 0)	//base values havn't been set yet
			InitInventoryOld();
		else
			return;
	}
	for(int cnt=INV_START_POS; cnt<NUM_INV_SLOTS; cnt++)
	{
		gSMInvData[ cnt ].sX = pRegionDesc[cnt].sX;
		gSMInvData[ cnt ].sY = pRegionDesc[cnt].sY;
		MSYS_RemoveRegion( &gSMInvRegion[ cnt ]);
		MSYS_DefineRegion( &gSMInvRegion[ cnt ], gSMInvData[ cnt ].sX, gSMInvData[ cnt ].sY, (INT16)(gSMInvData[ cnt ].sX + gSMInvData[ cnt ].sWidth), (INT16)(gSMInvData[ cnt ].sY + gSMInvData[ cnt ].sHeight), ( INT8 )( fSetHighestPrioity ? MSYS_PRIORITY_HIGHEST : MSYS_PRIORITY_HIGH ),
							 MSYS_NO_CURSOR, INVMoveCallback, INVClickCallback ); 
		// Add region
		MSYS_AddRegion( &gSMInvRegion[ cnt ] );
		MSYS_SetRegionUserData( &gSMInvRegion[ cnt ], 0, cnt );
	}
}

BOOLEAN InitInvSlotInterface( INV_REGION_DESC *pRegionDesc , INV_REGION_DESC *pCamoRegion, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, MOUSE_CALLBACK INVMoveCammoCallback, MOUSE_CALLBACK INVClickCammoCallback, BOOLEAN fSetHighestPrioity )
{
	INT32 cnt;
  VOBJECT_DESC    VObjectDesc;

/*
 *  Ok i think that here the money button and window is initialized
 *  so we also will initialize here coord of button
 *  this function is called in tactical screen when SMPanel is initialized
 *  and also in StrategicScreen i think i didnt check it.
 *  any questions? joker
 */

	// CHRISL: Adjusted location of the Money button on the tactical inventory screen
	// HEADROCK: Readjusted this, for the TACTICAL Enhanced Description Box 
	if ( guiCurrentScreen == MAP_SCREEN )
	{
		if(UsingNewAttachmentSystem() == true)
		{
			gMoneyButtonLoc.x = 186;
			gMoneyButtonLoc.y = 170;
		}
		else
		{
			gMoneyButtonLoc.x = 174;
			gMoneyButtonLoc.y = 115;
		}
	}
	else if ( UsingEDBSystem() > 0 )
	{
		if(UsingNewAttachmentSystem() == true)
		{
			gMoneyButtonLoc.x = ((UsingNewInventorySystem() == false)) ? (401 + INTERFACE_START_X) : (302 + INTERFACE_START_X);
			gMoneyButtonLoc.y = ( 64 + INV_INTERFACE_START_Y );
		}
		else
		{
			gMoneyButtonLoc.x = ((UsingNewInventorySystem() == false)) ? (343 + INTERFACE_START_X) : (244 + INTERFACE_START_X);
			gMoneyButtonLoc.y = ( 11 + INV_INTERFACE_START_Y );
		}
	}
	else
	{
		if(UsingNewAttachmentSystem() == true)
		{
			gMoneyButtonLoc.x = ((UsingNewInventorySystem() == false)) ? (401 + INTERFACE_START_X) : (302 + INTERFACE_START_X);
			gMoneyButtonLoc.y = ( 64 + INV_INTERFACE_START_Y );
		}
		else
		{
			gMoneyButtonLoc.x = ((UsingNewInventorySystem() == false)) ? (343 + INTERFACE_START_X) : (244 + INTERFACE_START_X);
			gMoneyButtonLoc.y = ( 11 + INV_INTERFACE_START_Y );
		}
	}

	// Load all four body type images
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_figure_large_male.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 1 ][ 0 ] ) ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_figure_large_male_H.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 1 ][ 1 ] ) ) );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_normal_male.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 0 ][ 0 ] ) ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_normal_male_H.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 0 ][ 1 ] ) ) );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_normal_male.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 2 ][ 0 ] ) ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_normal_male.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 2 ][ 1 ] ) ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_figure_female.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 3 ][ 0 ] ) ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_figure_female_H.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 3 ][ 1 ] ) ) );

	// Kaiden: Vehicle Inventory change - Added two new STI's for Vehicle Inventory
	// Feel free to change them to more appropriate pictures, I just blanked out
	// the body image for now, I'm no graphics artist.
	//VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("INTERFACE\\inventory_figure_Vehicle.sti",
	//VObjectDesc.ImageFile);
	//CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 4 ][ 0 ] ) ) );

	//VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("INTERFACE\\inventory_figure_Vehicle_h.sti",
	//VObjectDesc.ImageFile);
	//CHECKF( AddVideoObject( &VObjectDesc, &(guiBodyInvVO[ 4 ][ 1 ] ) ) );

	// add gold key graphic
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\gold_key_button.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiGoldKeyVO ) );

	// Add cammo region
	MSYS_DefineRegion( &gSMInvCamoRegion, pCamoRegion->sX, pCamoRegion->sY, (INT16)(pCamoRegion->sX + CAMO_REGION_WIDTH ), (INT16)(pCamoRegion->sY + CAMO_REGION_HEIGHT ), MSYS_PRIORITY_HIGH,
						 MSYS_NO_CURSOR, INVMoveCammoCallback, INVClickCammoCallback );
	// Add region
	MSYS_AddRegion( &gSMInvCamoRegion );

	// Add regions for inventory slots
	for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		// set inventory pocket coordinates from the table passed in
		gSMInvData[ cnt ].sX = pRegionDesc[ cnt ].sX;
		gSMInvData[ cnt ].sY = pRegionDesc[ cnt ].sY;

		MSYS_DefineRegion( &gSMInvRegion[ cnt ], gSMInvData[ cnt ].sX, gSMInvData[ cnt ].sY, (INT16)(gSMInvData[ cnt ].sX + gSMInvData[ cnt ].sWidth), (INT16)(gSMInvData[ cnt ].sY + gSMInvData[ cnt ].sHeight), ( INT8 )( fSetHighestPrioity ? MSYS_PRIORITY_HIGHEST : MSYS_PRIORITY_HIGH ),
							 MSYS_NO_CURSOR, INVMoveCallback, INVClickCallback );
		// Add region
		MSYS_AddRegion( &gSMInvRegion[ cnt ] );
		MSYS_SetRegionUserData( &gSMInvRegion[ cnt ], 0, cnt );
	}

	memset( gbCompatibleAmmo, 0, sizeof( gbCompatibleAmmo ) );

	return( TRUE );
}

void InitKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback )
{
		MSYS_DefineRegion( &gKeyRingPanel, KEYRING_X, KEYRING_Y, KEYRING_X + KEYRING_WIDTH, KEYRING_Y + KEYRING_HEIGHT, MSYS_PRIORITY_HIGH,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, KeyRingClickCallback );

		SetRegionFastHelpText( &(gKeyRingPanel), TacticalStr[ KEYRING_HELP_TEXT ] );

}

void InitMapKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback )
{
		MSYS_DefineRegion( &gKeyRingPanel, MAP_KEYRING_X, MAP_KEYRING_Y, MAP_KEYRING_X + KEYRING_WIDTH, MAP_KEYRING_Y + KEYRING_HEIGHT, MSYS_PRIORITY_HIGH,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, KeyRingClickCallback );

		SetRegionFastHelpText( &(gKeyRingPanel), TacticalStr[ KEYRING_HELP_TEXT ] );
}

void EnableKeyRing( BOOLEAN fEnable )
{
	if ( fEnable )
	{
		MSYS_EnableRegion( &gKeyRingPanel );
	}
	else
	{
		MSYS_DisableRegion( &gKeyRingPanel );
	}
}


void ShutdownKeyRingInterface( void )
{
	MSYS_RemoveRegion( &gKeyRingPanel );
	return;
}

void ShutdownInventoryInterface( void )
{
	// Add regions for inventory slots
	for ( INT32 cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		MSYS_RemoveRegion(&gSMInvRegion[ cnt ]);		
	}
}

void DisableInvRegions( BOOLEAN fDisable )
{
	INT32 cnt;

	for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		if ( fDisable )
		{
			MSYS_DisableRegion( &gSMInvRegion[ cnt ] );
		}
		else
		{
			MSYS_EnableRegion( &gSMInvRegion[ cnt ] );
		}
	}

	if ( fDisable )
	{
		MSYS_DisableRegion( &gSMInvCamoRegion );

		MSYS_DisableRegion( &gSM_SELMERCMoneyRegion );
		EnableKeyRing( FALSE );
		RenderBackpackButtons(DISABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	}
	else
	{
		MSYS_EnableRegion( &gSMInvCamoRegion );

		MSYS_EnableRegion( &gSM_SELMERCMoneyRegion );
		EnableKeyRing( TRUE );
		RenderBackpackButtons(ENABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	}

}

void ShutdownInvSlotInterface( )
{
	UINT32		cnt;

	// Kaiden: Vehicle Inventory change - Added 4-0 and 4-1
    // to be deleted as well.

	// Remove all body type panels
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 0 ][ 0 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 2 ][ 0 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 1 ][ 0 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 3 ][ 0 ] );
	//DeleteVideoObjectFromIndex( guiBodyInvVO[ 4 ][ 0 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 0 ][ 1 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 2 ][ 1 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 1 ][ 1 ] );
	DeleteVideoObjectFromIndex( guiBodyInvVO[ 3 ][ 1 ] );
	//DeleteVideoObjectFromIndex( guiBodyInvVO[ 4 ][ 1 ] );

	DeleteVideoObjectFromIndex( guiGoldKeyVO );

	// Remove regions
	// Add regions for inventory slots
	for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		// Remove region
		MSYS_RemoveRegion( &gSMInvRegion[ cnt ] );
	}

	// Remove cammo
	MSYS_RemoveRegion( &gSMInvCamoRegion );

}

void RenderInvBodyPanel( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY )
{
	// Blit body inv, based on body type
	INT8 bSubImageIndex = gbCompatibleApplyItem;

	// Kaiden: Vehicle Inventory change - Added IF Test, Else function call was
	// the original statement
	if ( (gGameExternalOptions.fVehicleInventory) && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
	{
		//BltVideoObjectFromIndex( guiSAVEBUFFER, guiBodyInvVO[4][0], 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	else
	{
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiBodyInvVO[ pSoldier->ubBodyType ][ bSubImageIndex ], 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );

	}
}

void HandleRenderInvSlots( SOLDIERTYPE *pSoldier, UINT8 fDirtyLevel )
{
	INT32	sX, sY;
	static CHAR16					pStr[ 512 ];
	if ( InItemDescriptionBox( ) || InItemStackPopup( ) || InKeyRingPopup( ) )
	{

	}
	else
	{
		for ( UINT32 cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			if ( fDirtyLevel == DIRTYLEVEL2 )
			{
#       if defined( _DEBUG ) /* Sergeant_Kolja, to be removed later again */
			if( pSoldier->inv[ cnt ][0]->data.gun.ubGunAmmoType >= MAXITEMS )
        {
         	DebugMsg(TOPIC_JA2, DBG_LEVEL_1, String("pObject (%S) corrupted! GetHelpTextForItem() can crash.", (pSoldier->inv[ cnt ].usItem<MAXITEMS) ? Item[pSoldier->inv[ cnt ].usItem].szItemName : L"???" ));
    	    ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"pObject (%s) corrupted! GetHelpTextForItem() can crash.",    (pSoldier->inv[ cnt ].usItem<MAXITEMS) ? Item[pSoldier->inv[ cnt ].usItem].szItemName : L"???" );
          DebugBreak();
          AssertMsg( 0, "pObject corrupted! GetHelpTextForItem() can crash." );
        }
#       endif

				GetHelpTextForItem( pStr, &( pSoldier->inv[ cnt ] ), pSoldier );

				SetRegionFastHelpText( &(gSMInvRegion[ cnt ]), pStr );
			}

			INVRenderINVPanelItem( pSoldier, (INT16)cnt, fDirtyLevel );
		}

		if ( KeyExistsInKeyRing( pSoldier, ANYKEY, NULL ) )
		{
			// blit gold key here?
			// CHRISL: adjust settings to use variables for coords
			if ( guiCurrentItemDescriptionScreen != MAP_SCREEN )
			{
				sX=((UsingNewInventorySystem() == false))?496:221;
				sY=((UsingNewInventorySystem() == false))?INV_INTERFACE_START_Y+106:INV_INTERFACE_START_Y+5;
			}
			else
			{
				sX=((UsingNewInventorySystem() == false))?217:188;
				sY=((UsingNewInventorySystem() == false))?271:126;
			}
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiGoldKeyVO, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
			RestoreExternBackgroundRect( sX, sY, 29, 23 );
		}
	}	
}

// CHRISL: New function to determine whether to activate Combat and Backpack slots
BOOLEAN CheckActivationStatus(SOLDIERTYPE *pSoldier, INT16 cSlot, INT16 bSlot, INT16 sPocket)
{
	INT8	cLevel, bLevel;

	cLevel = LoadBearingEquipment[Item[pSoldier->inv[cSlot].usItem].ubClassIndex].lbeCombo;
	bLevel = LoadBearingEquipment[Item[pSoldier->inv[bSlot].usItem].ubClassIndex].lbeCombo;
	
	if(sPocket==cSlot)
	{
		if(pSoldier->inv[bSlot].exists() == true)
		{
			if(bLevel == NOTHING)
			{
				return(TRUE);
			}
		}
	}
	if(sPocket==bSlot)
	{
		if(pSoldier->inv[cSlot].exists() == true)
		{
			if(cLevel == NOTHING)
			{
				return(TRUE);
			}
		}
	}
	return(FALSE);
}

// THE_BOB: functions for mag-making gizmo (TODO: clean up this code)
std::vector<OBJECTTYPE *> * getSoldierGuns( SOLDIERTYPE *pTeamSoldier )
{
	UINT32 bLoop;
	std::vector<OBJECTTYPE *> * guns = new std::vector<OBJECTTYPE *>;

	// Search for gun in soldier inventory
	for (bLoop = 0; bLoop < pTeamSoldier->inv.size(); bLoop++)
	{
		if (	(Item[pTeamSoldier->inv[bLoop].usItem].usItemClass & IC_GUN) 
			||  (Item[pTeamSoldier->inv[bLoop].usItem].usItemClass == IC_LAUNCHER) )
		{
			guns->push_back( &(pTeamSoldier->inv[bLoop]) );
		}
	}

	// no guns found, get rid of the vector
	if( guns->size() < 1 ) {
		delete guns;
		guns = NULL;
	}

	return guns;
}

INT16 pocketTypeInSlot(SOLDIERTYPE *pSoldier, INT16 sPocket){

	INT16		lbePocket = ITEM_NOT_FOUND;

	if((UsingNewInventorySystem() == false) && !oldInv[sPocket])
		return lbePocket;
	if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && UsingNewInventorySystem() == true && !vehicleInv[sPocket])
		return lbePocket;

	switch (icClass[sPocket])
	{
		case THIGH_PACK:
		case VEST_PACK:
		case COMBAT_PACK:
		case BACKPACK:
			lbePocket = 
				(pSoldier->inv[icLBE[sPocket]].exists() == false) 
				? LoadBearingEquipment[Item[icDefault[sPocket]].ubClassIndex].lbePocketIndex[icPocket[sPocket]] 
				: LoadBearingEquipment[Item[pSoldier->inv[icLBE[sPocket]].usItem].ubClassIndex].lbePocketIndex[icPocket[sPocket]];

			break;
		case LBE_POCKET:
				if ( sPocket == VESTPOCKPOS )
					lbePocket = 0;
				else if ( sPocket == LTHIGHPOCKPOS )
					lbePocket = 1;
				else if ( sPocket == RTHIGHPOCKPOS )
					lbePocket = 2;
				else if ( sPocket == CPACKPOCKPOS )
					lbePocket = 3;
				else if ( sPocket == BPACKPOCKPOS )
					lbePocket = 4;

		case OTHER_POCKET:

				if ( sPocket == GUNSLINGPOCKPOS ) // Gun Sling
					lbePocket = 1;
				else
					lbePocket = 2;
			break;

		default:
			// ?
			break;
	}

	return lbePocket;
}

//THE_BOB: mag-making popups
static POPUP * sPocketPopup = NULL;
static BOOL sPocketPopupInitialized = FALSE;

void popupCallbackAmmo(UINT16 item, UINT16 pocket, SOLDIERTYPE* pSoldier ){

	if(!(gTacticalStatus.uiFlags & INCOMBAT))
	{
		UINT16		magSize, ubShotsLeft;
		OBJECTTYPE	*pObj = NULL;
		OBJECTTYPE	tempClip;
		OBJECTTYPE	tempStack;
		bool		clipCreated;
		UINT32		newItem = 0;
		INT16		pocketType =  pocketTypeInSlot(pSoldier, pocket);
		UINT8		capacity = 0;
			
		if ( pocketType != -1 ){
		capacity = LBEPocketType[pocketTypeInSlot(pSoldier, pocket)].ItemCapacityPerSize[ Item[item].ItemSize ];
		} else {
		capacity = 1;
		}
			
		UINT8		bLoop;


		// find an ammo crate that can be used to make requested mag
		for(UINT16 i = 0; i < pInventoryPoolList.size(); i++)
		{	

			if(		Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubMagType >= AMMO_BOX	// item is ammo box/crate
				&&	Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubAmmoType	// same ammo type
					==	Magazine[ Item[item].ubClassIndex ].ubAmmoType							//	as the mag we found?
				&&	Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubCalibre	// same calibre
					==	Magazine[ Item[item].ubClassIndex ].ubCalibre 							//	as the mag we found?
			) 
			{
				pObj = &pInventoryPoolList[i].object; // found ammo crate
				break;
			} 
		} 

		if(!pObj) {
			sPocketPopup->hide();
			return;
		}

		//find the ammo item we want to try and create
		newItem = item;

		//Create a stack of up to 5 "newItem" clips 
		tempStack.initialize();
		clipCreated = false;
		ubShotsLeft = (*pObj)[0]->data.ubShotsLeft;
		for(UINT8 clip = 0; clip < capacity; clip++)
		{
			magSize = Magazine[ Item[item].ubClassIndex ].ubMagSize;
			if(ubShotsLeft < magSize)
				magSize = ubShotsLeft;
			
			if(CreateAmmo(newItem, &tempClip, magSize))
			{
				tempStack.AddObjectsToStack(tempClip, -1, pSoldier, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
				ubShotsLeft -= magSize;
				clipCreated = true;
				if(ubShotsLeft < 1)
					break;
			}

		}
		//Try to place the stack somewhere on the active merc
		if(clipCreated == true)
		{
			clipCreated = false;
			bLoop = tempStack.ubNumberOfObjects;
			while(tempStack.ubNumberOfObjects > 0)
			{
				if(pocket != -1)
				{
					pSoldier->inv[pocket].AddObjectsToStack(tempStack, bLoop, pSoldier, pocket);
				}
				else
				{
					bLoop--;
				}
				if(bLoop < 1)
					break;
			}
			if(tempStack.ubNumberOfObjects < 1)
				clipCreated = true;
			else
			{
				//Try to place stack on ground
				if( AutoPlaceObjectToWorld(pSoldier, &tempStack) )
				{
					clipCreated = true;
					if(guiCurrentScreen == GAME_SCREEN)
						NotifySoldiersToLookforItems( );
				}
			}
		}
		if(clipCreated == true)
		{
			(*pObj)[0]->data.ubShotsLeft = ubShotsLeft;
		}
		if((*pObj)[0]->data.ubShotsLeft < 1)
			pObj->RemoveObjectsFromStack(1);

		sPocketPopup->hide();
		return;
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMapInventoryErrorString[6] );

		sPocketPopup->hide();
		return;
	}

}

void popupCallbackPlaceLeastDamagedFromStack(OBJECTTYPE * pObj, UINT16 pocket, SOLDIERTYPE* pSoldier ){

	// can't be sure enough that it will fit
	if ( CanItemFitInPosition(pSoldier, pObj, pocket, false) && !pSoldier->inv[pocket].exists() ) {

		// if this is a stack, try to find the least damaged object
		
		if( pObj->ubNumberOfObjects > 1 ){

			UINT8 numObjectsToPlace;	// try to fit as many objects as possible

			{
				INT16 pocketType = pocketTypeInSlot( pSoldier, pocket );

				if( pocketType != -1 ){
					numObjectsToPlace =  min( pObj->objectStack.size(), LBEPocketType[ pocketType ].ItemCapacityPerSize[ Item[pObj->usItem].ItemSize ] ); 
				} else {
					numObjectsToPlace = 1;
				}
			}

			for (UINT8 j = 1; j <= numObjectsToPlace; j++){

				UINT16 i = 0, leastDamagedIndex = 0;
				INT16 leastDamagedStatus = 0;

				StackedObjects::iterator p = pObj->objectStack.begin();
				while(p != pObj->objectStack.end()) {
				
					if( p->data.objectStatus > leastDamagedStatus ){
						leastDamagedIndex = i;
						leastDamagedStatus = p->data.objectStatus;
					}
					i++;p++;
				}

				OBJECTTYPE pObjTmp;
				pObjTmp.initialize();

				if( pObj->RemoveObjectAtIndex(leastDamagedIndex, &pObjTmp) )
					PlaceObject( pSoldier, pocket, &pObjTmp );
			}

		} else {
			PlaceObject( pSoldier, pocket, pObj );
		}
				
	}

	sPocketPopup->hide();
}

// THE_BOB: quick equip popups
extern	BOOLEAN CanPlayerUseSectorInventory( SOLDIERTYPE *pSelectedSoldier );

extern  void RenderTeamRegionBackground();
void createMagPopupAfter(SOLDIERTYPE *pSoldier){	// after showing the menu, this callback marks the interface as dirty and redraws it
	
	fTeamPanelDirty = TRUE;
	fMapPanelDirty = TRUE;
	RenderTeamRegionBackground();

}

INT16 getStatusOfLeastDamagedItemInStack( OBJECTTYPE * stack ){

	INT16 leastDamagedStatus = 0;

	// find the status of the least damaged item on the stack
	if( stack->ubNumberOfObjects > 1 ){ // (unless there's only one item on the stack)		

		StackedObjects::iterator p = stack->objectStack.begin();
		while(p != stack->objectStack.end()) {
				
			if( p->data.objectStatus > leastDamagedStatus ){
				leastDamagedStatus = p->data.objectStatus;
			}

			p++;
		}

	} else {
		leastDamagedStatus = stack->objectStack.begin()->data.objectStatus;
	}

	return leastDamagedStatus;
}

// ugly hack to pretend that weapon class/type defines are bitfields
static const int pow2[] = {1,2,4,8,16,32,64,128,256}; // I'm a funny man.

std::map<UINT32,OBJECTTYPE*> findLeastDamagedStackForPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, int itemClass = -1, int weaponClass = -1, int weaponType = -1, int attachType = -1 ){

	std::map<UINT32,OBJECTTYPE*> bestItems;
	std::map<UINT32,INT16> bestItemsStatus;

	for(UINT16 i = 0; i < pInventoryPoolList.size(); i++)
	{	

		OBJECTTYPE * currentStack = &pInventoryPoolList[i].object;
		UINT16 currentItem = currentStack->usItem;

		if(		( ( itemClass == -1 ) || Item[currentItem].usItemClass & itemClass )
			&&	( ( attachType == -1 ) || Item[currentItem].nasAttachmentClass == attachType )
			// weapon checks
			&&	( ( ( weaponClass == -1 ) || ( weaponClass == 0 && Weapon[currentItem].ubWeaponClass == weaponClass ) || pow2[Weapon[currentItem].ubWeaponClass] & weaponClass ) )
			&&	( ( ( weaponType == -1 ) || ( weaponType == 0 && Weapon[currentItem].ubWeaponType == weaponType ) || pow2[Weapon[currentItem].ubWeaponType] & weaponType ) )
			// if type/class conditions are met, do more expensive check on whether it will fit
			&&	CanItemFitInPosition(pSoldier, currentStack, sPocket, FALSE) 
			)
		{				

			INT16 leastDamagedStatus = getStatusOfLeastDamagedItemInStack( currentStack );

			if( bestItemsStatus[ currentItem ] < leastDamagedStatus ){	// either not indexed yet or worse then current

				bestItemsStatus[ currentItem ] = leastDamagedStatus;
				bestItems[ currentItem ] = currentStack;

			}

		} // found item

	} // inv loop

	return bestItems;

}

// add generic items of some class to the popup, picking the best stacks in sector

void addItemsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup, int itemClass = -1, int weaponClass = -1, int weaponType = -1, int attachType = -1 ){

	std::map<UINT32,OBJECTTYPE*> bestItems = findLeastDamagedStackForPopup( pSoldier, sPocket, itemClass, weaponClass, weaponType, attachType );
	
	UINT8 optsTotal = 0, numObjectsToPlace = 1;
	INT16 pocketType = pocketTypeInSlot( pSoldier, sPocket );
	POPUP * currPopup = popup;
	
	for(std::map<UINT32,OBJECTTYPE*>::iterator itr = bestItems.begin(); itr != bestItems.end(); ++itr){

		if( optsTotal > 0 && optsTotal%15 == 0 ){ // divide to subBoxes every 10 items

			POPUP * currPopupTmp = currPopup->addSubMenuOption( new std::wstring(L"more...") );	// the new popup
			POPUP_SUB_POPUP_OPTION * currSubPopupTmp = currPopup->getSubPopupOption( currPopup->subPopupOptionCount-1 );	// the sub-popup option in prev popup

			
			// where to put the new box?
			if( currPopup->Position.iX + 200 > SCREEN_WIDTH){

				// near screen edge, put it below the original box					
				currSubPopupTmp->setPopupPosition(	popup->Position.iX,
													popup->Position.iY + popup->getCurrentHeight(),
													POPUP_POSITION_TOP_LEFT);
			
			} else {
				// we still got room, position the next box to the right of the previous one
				currSubPopupTmp->setPopupPosition(	10,
													0,
													POPUP_POSITION_RELATIVE);

			}


			currPopup = currPopupTmp;	// swap popups, now adding options to the new sub-popup
		}

		if( pocketType != -1 ){
			numObjectsToPlace =   min( itr->second->objectStack.size(), LBEPocketType[ pocketType ].ItemCapacityPerSize[ Item[itr->second->usItem].ItemSize ] ); 
		} else {
			numObjectsToPlace = 1;
		}

		if( numObjectsToPlace > 1 ){	// add number of items that will be placed

			static CHAR16 pStr[ 100 ];
			swprintf( pStr, L"%s (%d)", Item[ itr->first ].szItemName, numObjectsToPlace );

			currPopup->addOption( 
								&std::wstring( pStr ), 
								new popupCallbackFunction3<void,OBJECTTYPE*,UINT16,SOLDIERTYPE*>(&popupCallbackPlaceLeastDamagedFromStack,itr->second,sPocket,pSoldier) 
								);

		} else {
			currPopup->addOption( 
								&std::wstring( Item[ itr->first ].szItemName ), 
								new popupCallbackFunction3<void,OBJECTTYPE*,UINT16,SOLDIERTYPE*>(&popupCallbackPlaceLeastDamagedFromStack,itr->second,sPocket,pSoldier) 
								);		
		}

		optsTotal++;

	}

}

void addArmorToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_ARMOUR );

}

void addLBEToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_LBEGEAR );

}

void addWeaponsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_WEAPON );

}

void addWeaponGroupsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	POPUP * subPopup = NULL;

	subPopup = popup->addSubMenuOption( new std::wstring(L"Guns") );
	popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,
																				10,
																				POPUP_POSITION_RELATIVE );


	std::map<UINT32,OBJECTTYPE*> bestItems = findLeastDamagedStackForPopup( pSoldier, sPocket, IC_GUN, 
							pow2[HANDGUNCLASS] + pow2[SMGCLASS] + pow2[RIFLECLASS] + pow2[MGCLASS] + pow2[SHOTGUNCLASS], 
							pow2[GUN_PISTOL] + pow2[GUN_M_PISTOL] + pow2[GUN_SMG] + pow2[GUN_RIFLE] + pow2[GUN_SN_RIFLE] + pow2[GUN_AS_RIFLE] + pow2[GUN_LMG] + pow2[GUN_SHOTGUN],
							0);

	POPUP* weaponTypePopup;
	UINT8 weaponTypeCtr;
	for( weaponTypeCtr = 1; weaponTypeCtr <= 8; weaponTypeCtr++ ){
		
		weaponTypePopup = subPopup->addSubMenuOption( new std::wstring( WeaponType[weaponTypeCtr] ) );

		for(std::map<UINT32,OBJECTTYPE*>::iterator itr = bestItems.begin(); itr != bestItems.end(); ++itr){

			if ( Weapon[ itr->first ].ubWeaponType == weaponTypeCtr )
			weaponTypePopup->addOption( 
								&std::wstring( Item[ itr->first ].szItemName ), 
								new popupCallbackFunction3<void,OBJECTTYPE*,UINT16,SOLDIERTYPE*>(&popupCallbackPlaceLeastDamagedFromStack,itr->second,sPocket,pSoldier) 
								);
		}

	}
	

	subPopup = popup->addSubMenuOption( new std::wstring(L"Grenade launchers") );
	popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,
																				10,
																				POPUP_POSITION_RELATIVE );
	addItemsToPocketPopup( pSoldier, sPocket, subPopup, IC_LAUNCHER, -1, -1, 0 );

	subPopup = popup->addSubMenuOption( new std::wstring(L"Rocket launchers") );
	popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,
																				10,
																				POPUP_POSITION_RELATIVE );
	addItemsToPocketPopup( pSoldier, sPocket, subPopup, IC_GUN, -1, 0, 0);


	subPopup = popup->addSubMenuOption( new std::wstring(L"Melee & thrown weapons") );
	popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,
																				10,
																				POPUP_POSITION_RELATIVE );
	addItemsToPocketPopup( pSoldier, sPocket, subPopup, IC_BLADE+IC_THROWING_KNIFE, pow2[KNIFECLASS] );

}

void addGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_GRENADE, -1, -1, 0 );
	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_GRENADE, -1, -1, AC_GRENADE );

}

void addRifleGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_GRENADE, -1, -1, AC_GRENADE );

}

void addRocketAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_GRENADE, -1, -1, AC_ROCKET );

}

void addKitsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_MEDKIT + IC_KIT );

}

void addBombsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_BOMB );

}

void addMiscToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_MISC, -1, -1, 1 );

}

void addFaceGearToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	addItemsToPocketPopup( pSoldier, sPocket, popup, IC_FACE );

}


void addAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup ){

	// get the guns on current soldier
	std::vector<OBJECTTYPE *> * guns = getSoldierGuns(pSoldier);

	if( guns != NULL && guns->size() > 0)
	{
		INT16 lbePocket = pocketTypeInSlot(pSoldier, sPocket);
		
		for(std::vector<OBJECTTYPE*>::iterator gun=guns->begin(); gun != guns->end(); ++gun)
		{
			UINT8 ammoFound = 0;

			POPUP_OPTION * o = popup->addOption( &std::wstring( Item[ (*gun)->usItem ].szItemName ), NULL );
			o->color_shade = COLOR_LTGREY;
			//o->color_background = COLOR_LTGREY;
					
			//find the ammo item we want to try and create
			for(UINT32 loop = 0; loop < MAXITEMS; loop++)
			{
				if(Item[loop].usItemClass == IC_AMMO)
				{
					if(		Magazine[Item[loop].ubClassIndex].ubCalibre == Weapon[ (*gun)->usItem ].ubCalibre 
						&&	Magazine[Item[loop].ubClassIndex].ubMagSize == GetMagSize((*gun)) 
						&&	( lbePocket == -1 || LBEPocketType[lbePocket].ItemCapacityPerSize[ Item[loop].ItemSize ] > 0 )  )
					{ // found ammo for gun, look for its ammo crate in sector.
												
						for(UINT16 i = 0; i < pInventoryPoolList.size(); i++)
						{	// TODO: index ammo crates in sector, don't loop over entire inventory for each mag/gun

							if(		Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubMagType >= AMMO_BOX // item is ammo box/crate
								&&	Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubAmmoType	// same ammo type
									==	Magazine[ Item[loop].ubClassIndex ].ubAmmoType							//	as the mag we found?
								&&	Magazine[ Item[pInventoryPoolList[i].object.usItem].ubClassIndex ].ubCalibre	// same calibre
									==	Magazine[ Item[loop].ubClassIndex ].ubCalibre 							//	as the mag we found?
							) 
							{	
								ammoFound++;
								UINT8 capacity = 0;

								if( lbePocket != -1 ){
									UINT16 ammoLeft = pInventoryPoolList[i].object.objectStack.begin()->data.ubShotsLeft;
									UINT16 magSize = Magazine[ Item[loop].ubClassIndex ].ubMagSize;

									UINT8 maxPerPocket = LBEPocketType[pocketTypeInSlot(pSoldier, sPocket)].ItemCapacityPerSize[ Item[loop].ItemSize ];
										
									capacity = min( maxPerPocket, UINT8(ammoLeft/magSize) );
								}
								else if( CanItemFitInPosition(pSoldier, &pInventoryPoolList[i].object, sPocket, FALSE) ){
									capacity = 1;
								} else {
									continue;
								}
										
								static CHAR16 pStr[ 100 ];
								swprintf( pStr, L"%s (%d)", Item[loop].szItemName,capacity );

								popup->addOption( &std::wstring( pStr ), new popupCallbackFunction3<void,UINT16,UINT16,SOLDIERTYPE*>(&popupCallbackAmmo,loop,sPocket,pSoldier) );

							} // found ammo crate, crate matches mag
						} // inv loop
					} // mag matches
				} // mag found
			} // mag loop

			if (!ammoFound){
				POPUP_OPTION * o = popup->addOption( &std::wstring( L"- no matching ammo -" ), NULL );
				o->color_shade = COLOR_RED;
			}

		}// gun loop

		delete guns;
	} // found guns
	else 
	{
		POPUP_OPTION * o = popup->addOption( &std::wstring( L"- no guns in inventory -" ), NULL );
		o->color_shade = COLOR_RED;
	}

}

POPUP * createPopupForPocket( SOLDIERTYPE *pSoldier, INT16 sPocket ){

	if(	!(	
		guiCurrentItemDescriptionScreen == MAP_SCREEN 
	&&	fShowMapInventoryPool 
	&&	(	( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX == sSelMapX )
		&&	( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY == sSelMapY )
		&&	( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ == iCurrentMapSectorZ ) 
		)
	&&	CanPlayerUseSectorInventory( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] )	
	) )
	{
		return NULL;
	}

	INT16 sX, sY;
	// get pocket type under cursor
	INT16 lbePocket = pocketTypeInSlot(pSoldier, sPocket);
	/*				
	if ( lbePocket != -1 ) {
	*/
		if (!sPocketPopupInitialized) {
			sPocketPopup = new POPUP("Pocket popup");
			sPocketPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,SOLDIERTYPE*>( createMagPopupAfter,pSoldier ) );
			sPocketPopupInitialized = true;
		} else {
			sPocketPopup->~POPUP();
			sPocketPopup = new POPUP("Pocket popup");
			sPocketPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,SOLDIERTYPE*>( createMagPopupAfter,pSoldier ) );
		}

		sX = gSMInvData[ sPocket ].sX;
		sY = gSMInvData[ sPocket ].sY;

		UINT8 thisPopupsPositionType;
		if ( sX < 170 && sY < 180 ){
			thisPopupsPositionType = POPUP_POSITION_TOP_LEFT;
		} else if ( sX > 170 && sY < 180 ){
			thisPopupsPositionType = POPUP_POSITION_TOP_RIGHT;
		} else if ( sX > 170 && sY > 180 ){
			thisPopupsPositionType = POPUP_POSITION_BOTTOM_RIGHT;
		} else if ( sX < 170 && sY > 180 ){
			thisPopupsPositionType = POPUP_POSITION_BOTTOM_LEFT;
		} else {
			thisPopupsPositionType = POPUP_POSITION_TOP_LEFT;
		}

		sPocketPopup->setPosition(			sX + 12,
		/* Put it near the current slot	 */	sY + 32,
											thisPopupsPositionType);
		return sPocketPopup;
/*
	} // identified pocket
	else 	{
		return NULL;
	}
*/
}

void PocketPopupFull( SOLDIERTYPE *pSoldier, INT16 sPocket ){

	POPUP * popup = createPopupForPocket( pSoldier, sPocket );

	if( popup != NULL ){

		POPUP * subPopup = NULL;

		subPopup = popup->addSubMenuOption( new std::wstring(L"Weapons") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addWeaponGroupsToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"Ammo") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addAmmoToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"Armor") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addArmorToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"LBE") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addLBEToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"Grenades") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addGrenadesToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"Bombs") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addBombsToPocketPopup( pSoldier, sPocket, subPopup );

		subPopup = popup->addSubMenuOption( new std::wstring(L"Face Gear") );
		popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
		addFaceGearToPocketPopup( pSoldier, sPocket, subPopup );

		popup->show();
	}

}

/*
	typedef enum eLBE_CLASS	// Designation of lbeClass
{
	THIGH_PACK=1,
	VEST_PACK,
	COMBAT_PACK,
	BACKPACK,
	LBE_POCKET,
	OTHER_POCKET
};

typedef enum ePOCKET_TYPE
{
	NO_POCKET_TYPE = 0,
	GUNSLING_POCKET_TYPE = 1,
	KNIFE_POCKET_TYPE = 2,
	VEHICLE_POCKET_TYPE = 3,
};
*/

UINT16 gsPocketUnderCursor;

void PocketPopupDefault( SOLDIERTYPE *pSoldier, INT16 sPocket ){

	POPUP * popup = createPopupForPocket( pSoldier, sPocket );

	if( popup ){

		switch (sPocket){
			case 	HELMETPOS:
			case	VESTPOS:
			case	LEGPOS:
				addArmorToPocketPopup( pSoldier, sPocket, popup );
				break;

			case	HEAD1POS:
			case	HEAD2POS:
				addFaceGearToPocketPopup( pSoldier, sPocket, popup );
				break;

			case	HANDPOS:
				addWeaponGroupsToPocketPopup( pSoldier, sPocket, popup );
				break;
			case	SECONDHANDPOS:
				addWeaponGroupsToPocketPopup( pSoldier, sPocket, popup );
				break;

			case	VESTPOCKPOS:
			case	BPACKPOCKPOS:
			case	CPACKPOCKPOS:
			case	LTHIGHPOCKPOS:
			case	RTHIGHPOCKPOS:
				addLBEToPocketPopup( pSoldier, sPocket, popup );
				break;
			
			case	GUNSLINGPOCKPOS:
				addWeaponsToPocketPopup( pSoldier, sPocket, popup );
				break;

			case	KNIFEPOCKPOS:
				addWeaponsToPocketPopup( pSoldier, sPocket, popup );
				break;

			default:				
				UINT8 pocketType = pocketTypeInSlot(pSoldier,sPocket);

				if( LBEPocketPopup.find(pocketType) == LBEPocketPopup.end() ){
					// default for LBE slots - grenades + ammo for merc's guns
				addAmmoToPocketPopup( pSoldier, sPocket, popup );

				POPUP * subPopup = popup->addSubMenuOption( new std::wstring(L"Grenades") );
				popup->getSubPopupOption( popup->subPopupOptionCount-1 )->setPopupPosition(	10,10,POPUP_POSITION_RELATIVE );
				addGrenadesToPocketPopup( pSoldier, sPocket, subPopup );
				} else {
					// popup definition for this slot exists - apply it to the popup
					gsPocketUnderCursor = sPocket;
					LBEPocketPopup[pocketType].applyToBox( popup );
				}


		}

		popup->show();
	
	}

}

// THE_BOB: end of inventory popups


void INVRenderINVPanelItem( SOLDIERTYPE *pSoldier, INT16 sPocket, UINT8 fDirtyLevel )
{
	// CHRISL: Only run if we're looking at a legitimate pocket
	if((UsingNewInventorySystem() == false) && !oldInv[sPocket])
		return;
	if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && UsingNewInventorySystem() == true && !vehicleInv[sPocket])
		return;
	INT16 sX, sY, newX, newY;
	INT16	sBarX, sBarY;
	OBJECTTYPE  *pObject;
	BOOLEAN	fOutline = FALSE;
	INT16		sOutlineColor = 0, lbePocket = ITEM_NOT_FOUND;
	UINT8		fRenderDirtyLevel;
	BOOLEAN fHatchItOut = FALSE;
	UINT32		iClass;


	//Assign the screen
	guiCurrentItemDescriptionScreen = guiCurrentScreen;

	pObject = &(pSoldier->inv[ sPocket ]);

	sX = gSMInvData[ sPocket ].sX;
	sY = gSMInvData[ sPocket ].sY;

	if((UsingNewInventorySystem() == true) && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
	{
		// If sPocket is not an equiped pocket, gather pocket information
		if(icClass[sPocket] != ITEM_NOT_FOUND)
		{
			switch (icClass[sPocket])
			{
				case THIGH_PACK:
				case VEST_PACK:
				case COMBAT_PACK:
				case BACKPACK:
					lbePocket = 
						(pSoldier->inv[icLBE[sPocket]].exists() == false) 
						? LoadBearingEquipment[Item[icDefault[sPocket]].ubClassIndex].lbePocketIndex[icPocket[sPocket]] 
						: LoadBearingEquipment[Item[pSoldier->inv[icLBE[sPocket]].usItem].ubClassIndex].lbePocketIndex[icPocket[sPocket]];
					iClass = Item[pSoldier->inv[sPocket].usItem].usItemClass;
					if(icLBE[sPocket] == BPACKPOCKPOS && !(pSoldier->flags.ZipperFlag) && (gTacticalStatus.uiFlags & INCOMBAT))
						lbePocket = 0;
					if (lbePocket == 0)	// Deactivate Pocket
					{
						fHatchItOut = TRUE;
					}
					else if ( pObject->exists() == false )	// Nothing in sPocket.  Display silouhette.
					{
						INVRenderSilhouette( guiSAVEBUFFER, lbePocket, 0, sX, sY, gSMInvData[ sPocket ].sWidth, gSMInvData[ sPocket ].sHeight);
					}
					break;
				case LBE_POCKET:
					if ( pObject->exists() == false )
					{
						if ( sPocket == VESTPOCKPOS )
							lbePocket = 0;
						else if ( sPocket == LTHIGHPOCKPOS )
							lbePocket = 1;
						else if ( sPocket == RTHIGHPOCKPOS )
							lbePocket = 2;
						else if ( sPocket == CPACKPOCKPOS )
							lbePocket = 3;
						else if ( sPocket == BPACKPOCKPOS )
							lbePocket = 4;
						if ( lbePocket != ITEM_NOT_FOUND )
							INVRenderSilhouette( guiSAVEBUFFER, ITEM_NOT_FOUND, lbePocket, sX, sY, gSMInvData[ sPocket ].sWidth, gSMInvData[ sPocket ].sHeight);
					}
					// Removed backpack/gunsling restrictions
					if ( CheckActivationStatus(pSoldier, CPACKPOCKPOS, BPACKPOCKPOS, sPocket)/* ||
						(sPocket == BPACKPOCKPOS && pSoldier->inv[GUNSLINGPOCKPOS].exists() == true)*/)
					{
						fHatchItOut = TRUE;
					}
					break;
				case OTHER_POCKET:
					if ( pObject->exists() == false )
					{
						if ( sPocket == GUNSLINGPOCKPOS ) // Gun Sling
							lbePocket = 1;
						else
							lbePocket = 2;
						INVRenderSilhouette( guiSAVEBUFFER, lbePocket, 0, sX, sY, gSMInvData[ sPocket ].sWidth, gSMInvData[ sPocket ].sHeight);
					}
					break;
				default:
					if ( pObject->exists() == false )
					{
						// Display appropriate silouhette
					}
					break;
			}
		}
	}

	if ( fDirtyLevel == DIRTYLEVEL2 )
	{
		// CHECK FOR COMPATIBILITY WITH MAGAZINES

/*	OLD VERSION OF GUN/AMMO MATCH HIGHLIGHTING
		UINT32	uiDestPitchBYTES;
		UINT8		*pDestBuf;
		UINT16	usLineColor;

		if ( ( Item [ pSoldier->inv[ HANDPOS ].usItem ].usItemClass & IC_GUN )  && ( Item[ pObject->usItem ].usItemClass & IC_AMMO ) )
		{
			// CHECK
			if (Weapon[pSoldier->inv[ HANDPOS ].usItem].ubCalibre == Magazine[Item[pObject->usItem].ubClassIndex].ubCalibre )
			{
				// IT's an OK calibre ammo, do something!
				// Render Item with specific color
				//fOutline = TRUE;
				//sOutlineColor = Get16BPPColor( FROMRGB( 96, 104, 128 ) );
				//sOutlineColor = Get16BPPColor( FROMRGB( 20, 20, 120 ) );

				// Draw rectangle!
				pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );
				SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, 640, 480 );

				//usLineColor = Get16BPPColor( FROMRGB( 255, 255, 0 ) );
				usLineColor = Get16BPPColor( FROMRGB( 230, 215, 196 ) );
				RectangleDraw( TRUE, (sX+1), (sY+1), (sX + gSMInvData[ sPocket ].sWidth - 2 ),( sY + gSMInvData[ sPocket ].sHeight - 2 ), usLineColor, pDestBuf );

				SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, 640, 480 );

				UnLockVideoSurface( guiSAVEBUFFER );
			}
		}
*/

		if ( gbCompatibleAmmo[ sPocket ] )
		{
			fOutline = TRUE;
			sOutlineColor = Get16BPPColor( FROMRGB( 255, 255, 255 ) );
		}

		// IF it's the second hand and this hand cannot contain anything, remove the second hand position graphic
//		if (sPocket == SECONDHANDPOS && Item[pSoldier->inv[HANDPOS].usItem].fFlags & ITEM_TWO_HANDED)
		if (sPocket == SECONDHANDPOS && Item[pSoldier->inv[HANDPOS].usItem].twohanded )
		{
			// CHRISL: Change coords for STI that covers 2nd hand location when carrying a 2handed weapon
			if( guiCurrentItemDescriptionScreen != MAP_SCREEN )
			{
				newX = ((UsingNewInventorySystem() == false)) ? (xResOffset + 217) : ((SCREEN_WIDTH - xResSize)/2 + 114);
				newY = ((UsingNewInventorySystem() == false)) ? sY : (sY - 1);
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiSecItemHiddenVO, UsingNewInventorySystem(), newX, newY, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( newX, newY, 72, 28 );
			}
			else
			{	
				newX = ((UsingNewInventorySystem() == false)) ? (14 + xResOffset) : (6 + xResOffset);
				newY = ((UsingNewInventorySystem() == false)) ? 218 : 217;
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiMapInvSecondHandBlockout, UsingNewInventorySystem(), newX, newY, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( newX, newY, 102, 24 );
			}
		}		
	}

	// If we have a new item and we are in the right panel...
	if ( pSoldier->inv.bNewItemCount[ sPocket ] > 0 && gsCurInterfacePanel == SM_PANEL && fInterfacePanelDirty != DIRTYLEVEL2 )
	{
		fRenderDirtyLevel = DIRTYLEVEL0;
		//fRenderDirtyLevel = fDirtyLevel;
	}
	else
	{
		fRenderDirtyLevel = fDirtyLevel;
	}

	//Now render as normal
	//INVRenderItem( guiSAVEBUFFER, pObject, (INT16)(sX + gSMInvData[ sPocket ].sSubX), (INT16)(sY + gSMInvData[ sPocket ].sSubY), gSMInvData[ sPocket ].sWidth, gSMInvData[ sPocket ].sHeight, fDirtyLevel, &(gfSM_HandInvDispText[ sPocket ] ) );
	INVRenderItem( guiSAVEBUFFER, pSoldier, pObject, sX, sY, gSMInvData[ sPocket ].sWidth, gSMInvData[ sPocket ].sHeight, fRenderDirtyLevel, NULL, 0, fOutline, sOutlineColor );

	// CHRISL: Display pocket capacity if we're holding something in the cursor
	if (!gfSMDisableForItems && (UsingNewInventorySystem() == true) && gpItemPointer != NULL)
	{
		UINT8 itemSlotLimit = ItemSlotLimit(gpItemPointer, sPocket, pSoldier);
		RenderPocketItemCapacity( guiSAVEBUFFER, itemSlotLimit, sPocket, pSoldier, &pSoldier->inv[sPocket], sX, sY );
		if(itemSlotLimit == 0 && !CanItemFitInPosition(pSoldier, gpItemPointer, (INT8)sPocket, FALSE)) {
			fHatchItOut = TRUE;
		}
	}

	// CHRISL: Change whether we hatch a pocket to be dependant on the current item
	if(gpItemPointer != NULL)
	{
		if(!gfSMDisableForItems && !CanItemFitInPosition(pSoldier, gpItemPointer, (INT8)sPocket, FALSE)){
			if((UsingNewAttachmentSystem()==false && !ValidAttachment(gpItemPointer->usItem, pObject )) ||
				(UsingNewAttachmentSystem()==true && !ValidItemAttachmentSlot(pObject, gpItemPointer->usItem, FALSE, FALSE))){
				fHatchItOut = TRUE;
			}
			else{
				fHatchItOut = FALSE;
			}
		}
	}
	else if(pObject->exists() == true)
	{
		if(!gfSMDisableForItems && !CanItemFitInPosition(pSoldier, pObject, (INT8)sPocket, FALSE))
			fHatchItOut = TRUE;
	}
	// CHRISL: Don't hatch second hand position if we're holding a two handed item
	if(sPocket == SECONDHANDPOS)
	{
		if(Item[pSoldier->inv[HANDPOS].usItem].twohanded)
			fHatchItOut = FALSE;
	}
#if 0
	if ( gbInvalidPlacementSlot[ sPocket ] )
	{
		if ( sPocket != SECONDHANDPOS )
		{
			// If we are in inv panel and our guy is not = cursor guy...
			if ( !gfSMDisableForItems )
			{
				fHatchItOut = TRUE;
			}
		}
	}
#endif

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		if( ShouldSoldierDisplayHatchOnItem( pSoldier->ubProfile, sPocket ) && !gbInvalidPlacementSlot[ sPocket ] )
		{
			fHatchItOut = TRUE;
		}
	}

	if ( fHatchItOut )
	{
		UINT32 uiWhichBuffer = ( guiCurrentItemDescriptionScreen == MAP_SCREEN ) ? guiSAVEBUFFER : guiRENDERBUFFER;
		DrawHatchOnInventory( uiWhichBuffer, sX, sY, (UINT16)(gSMInvData[ sPocket ].sWidth-1), (UINT16)(gSMInvData[ sPocket ].sHeight-1) );
	}

	// if there's an item in there
	if ( pObject->exists() == true )
	{
		// Add item status bar
		sBarX = sX - gSMInvData[ sPocket ].sBarDx;
		sBarY = sY + gSMInvData[ sPocket ].sBarDy;
		DrawItemUIBarEx( pObject, 0, sBarX, sBarY, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT,	Get16BPPColor( STATUS_BAR ), Get16BPPColor( STATUS_BAR_SHADOW ), TRUE , guiSAVEBUFFER);
	}

}


BOOLEAN CompatibleAmmoForGun( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject )
{
	if ( ( Item[ pTryObject->usItem ].usItemClass & IC_AMMO ) )
	{
		// CHECK
		if (Weapon[ pTestObject->usItem ].ubCalibre == Magazine[Item[pTryObject->usItem].ubClassIndex].ubCalibre )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN CompatibleGunForAmmo( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject )
{
	if ( ( Item[ pTryObject->usItem ].usItemClass & IC_GUN ) )
	{
		// CHECK
		if (Weapon[ pTryObject->usItem ].ubCalibre == Magazine[Item[pTestObject->usItem].ubClassIndex].ubCalibre )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN	CompatibleItemForApplyingOnMerc( OBJECTTYPE *pTestObject )
{
	UINT16 usItem = pTestObject->usItem;

  // ATE: If in mapscreen, return false always....
	if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
  {
     return( FALSE );
  }

	// ATE: Would be nice to have flag here to check for these types....
	if ( Item[usItem].camouflagekit || usItem == ADRENALINE_BOOSTER || usItem == REGEN_BOOSTER ||
			 usItem == SYRINGE_3		 || usItem == SYRINGE_4 || usItem == SYRINGE_5 ||
			 Item[usItem].alcohol  || Item[usItem].canteen || usItem == JAR_ELIXIR || (usItem == 1022 && gGameExternalOptions.fCamoRemoving)  ) // Added rag usable on self - SANDRO
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}



BOOLEAN SoldierContainsAnyCompatibleStuff( SOLDIERTYPE *pSoldier, OBJECTTYPE *pTestObject )
{
	UINT32				cnt;
	OBJECTTYPE  *pObject;

	if( ( Item [ pTestObject->usItem ].usItemClass & IC_GUN ) )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleAmmoForGun( pObject, pTestObject ) )
			{
				return( TRUE );
			}
		}
	}

	if( ( Item [ pTestObject->usItem ].usItemClass & IC_AMMO ) )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleGunForAmmo( pObject, pTestObject ) )
			{
				return( TRUE );
			}
		}
	}

	// ATE: Put attachment checking here.....

	return( FALSE );
}


void HandleAnyMercInSquadHasCompatibleStuff( UINT8 ubSquad, OBJECTTYPE *pObject, BOOLEAN fReset )
{
	INT32 iCounter = 0;

	if ( ubSquad == NUMBER_OF_SQUADS )
	{
		return;
	}

	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if(  Squad[ iCurrentTacticalSquad ][ iCounter ] != NULL )
		{
			if ( !fReset )
			{
				if ( SoldierContainsAnyCompatibleStuff( Squad[ iCurrentTacticalSquad ][ iCounter ], pObject )	)
				{
					// Get face and set value....
					gFacesData[ Squad[ iCurrentTacticalSquad ][ iCounter ]->iFaceIndex ].fCompatibleItems = TRUE;
				}
			}
			else
			{
				gFacesData[ Squad[ iCurrentTacticalSquad ][ iCounter ]->iFaceIndex ].fCompatibleItems = FALSE;
			}
		}
	}

}

BOOLEAN HandleCompatibleAmmoUIForMapScreen( SOLDIERTYPE *pSoldier, INT32 bInvPos, BOOLEAN fOn, BOOLEAN fFromMerc   )
{
	BOOLEAN			fFound = FALSE;
	UINT32				cnt;
	OBJECTTYPE  *pObject, *pTestObject ;
	BOOLEAN			fFoundAttachment = FALSE;

	if( fFromMerc == FALSE )
	{
		pTestObject = &( pInventoryPoolList[ bInvPos ].object );
	}
	else
	{
		if ( bInvPos == NO_SLOT )
		{
			pTestObject = NULL;
		}
		else
		{
			pTestObject = &(pSoldier->inv[ bInvPos ]);
		}
	}

	// ATE: If pTest object is NULL, test only for existence of syringes, etc...
	if ( pTestObject == NULL )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleItemForApplyingOnMerc( pObject ) )
			{
				if ( fOn != gbCompatibleAmmo[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				gbCompatibleAmmo[ cnt ] = fOn;

			}
		}


		if ( gpItemPointer != NULL )
		{
			if ( CompatibleItemForApplyingOnMerc( gpItemPointer ) )
			{
				// OK, Light up portrait as well.....
				if ( fOn )
				{
					gbCompatibleApplyItem = TRUE;
				}
				else
				{
					gbCompatibleApplyItem = FALSE;
				}

				fFound = TRUE;
			}
		}

		if ( fFound )
		{
			fInterfacePanelDirty = DIRTYLEVEL2;
			//HandleRenderInvSlots( pSoldier, DIRTYLEVEL2 );
		}

		return( fFound );
	}

//	if ( !(Item[ pTestObject->usItem ].fFlags & ITEM_HIDDEN_ADDON) )
	if ( !(Item[ pTestObject->usItem ].hiddenaddon ) )
	{
		// First test attachments, which almost any type of item can have....
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

//			if ( Item[ pObject->usItem ].fFlags & ITEM_HIDDEN_ADDON )
			if ( Item[ pObject->usItem ].hiddenaddon  )
			{
				// don't consider for UI purposes
				continue;
			}

			if ( (UsingNewAttachmentSystem()==false && ValidAttachment( pObject->usItem, pTestObject )) ||
					 (UsingNewAttachmentSystem()==false && ValidAttachment( pTestObject->usItem, pObject )) ||
					 (UsingNewAttachmentSystem()==true && ValidItemAttachmentSlot(pTestObject, pObject->usItem, FALSE, FALSE, 0, cnt)) ||
					 (UsingNewAttachmentSystem()==true && ValidItemAttachmentSlot(pObject, pTestObject->usItem, FALSE, FALSE, 0, cnt)) ||
					 ValidLaunchable( pTestObject->usItem, pObject->usItem ) ||
					 ValidLaunchable( pObject->usItem, pTestObject->usItem ) )
			{
				fFoundAttachment = TRUE;

				if ( fOn != gbCompatibleAmmo[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				gbCompatibleAmmo[ cnt ] = fOn;
			}
		}
	}


	if ( ( Item [ pTestObject->usItem ].usItemClass & IC_GUN ) )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleAmmoForGun( pObject, pTestObject ) )
			{
				if ( fOn != gbCompatibleAmmo[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				gbCompatibleAmmo[ cnt ] = fOn;
			}
		}
	}
	else if( ( Item [ pTestObject->usItem ].usItemClass & IC_AMMO ) )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleGunForAmmo( pObject, pTestObject ) )
			{
				if ( fOn != gbCompatibleAmmo[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				gbCompatibleAmmo[ cnt ] = fOn;

			}
		}
	}


	return( fFound );
}

BOOLEAN HandleCompatibleAmmoUIForMapInventory( SOLDIERTYPE *pSoldier, INT32 bInvPos, INT32 iStartSlotNumber, BOOLEAN fOn, BOOLEAN fFromMerc   )
{
	// CJC: ATE, needs fixing here!

	BOOLEAN			fFound = FALSE;
	INT32				cnt;
	OBJECTTYPE  *pObject, *pTestObject ;
	BOOLEAN			fFoundAttachment = FALSE;

	if( fFromMerc == FALSE )
	{
		pTestObject = &( pInventoryPoolList[ iStartSlotNumber + bInvPos ].object);
	}
	else
	{
		if ( bInvPos == NO_SLOT )
		{
			pTestObject = NULL;
		}
		else
		{
			pTestObject = &(pSoldier->inv[ bInvPos ]);
		}
	}

	// First test attachments, which almost any type of item can have....
	for ( cnt = 0; cnt < MAP_INVENTORY_POOL_SLOT_COUNT; cnt++ )
	{
		pObject = &( pInventoryPoolList[ iStartSlotNumber + cnt ].object );

//		if ( Item[ pObject->usItem ].fFlags & ITEM_HIDDEN_ADDON )
		if ( Item[ pObject->usItem ].hiddenaddon  )
		{
			// don't consider for UI purposes
			continue;
		}

		if ( ValidAttachment( pObject->usItem, pTestObject ) ||
				 ValidAttachment( pTestObject->usItem, pObject ) ||
				 ValidLaunchable( pTestObject->usItem, pObject->usItem ) ||
				 ValidLaunchable( pObject->usItem, pTestObject->usItem ) )
		{
			fFoundAttachment = TRUE;

			if ( fOn != fMapInventoryItemCompatable[ cnt ] )
			{
				fFound = TRUE;
			}

			// IT's an OK calibere ammo, do something!
			// Render Item with specific color
			fMapInventoryItemCompatable[ cnt ] = fOn;
		}
	}


	if( ( Item [ pTestObject->usItem ].usItemClass & IC_GUN ) )
	{
		for ( cnt = 0; cnt < MAP_INVENTORY_POOL_SLOT_COUNT; cnt++ )
		{
			pObject = &( pInventoryPoolList[ iStartSlotNumber + cnt ].object );

			if ( CompatibleAmmoForGun( pObject, pTestObject ) )
			{
				if ( fOn != fMapInventoryItemCompatable[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				fMapInventoryItemCompatable[ cnt ] = fOn;
			}
		}
	}
	else if( ( Item [ pTestObject->usItem ].usItemClass & IC_AMMO ) )
	{
		for ( cnt = 0; cnt < MAP_INVENTORY_POOL_SLOT_COUNT; cnt++ )
		{
			pObject = &( pInventoryPoolList[ iStartSlotNumber + cnt ].object );

			if ( CompatibleGunForAmmo( pObject, pTestObject ) )
			{
				if ( fOn != fMapInventoryItemCompatable[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				fMapInventoryItemCompatable[ cnt ] = fOn;

			}
		}
	}


	return( fFound );
}


BOOLEAN InternalHandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, OBJECTTYPE *pTestObject, BOOLEAN fOn  )
{
	BOOLEAN			fFound = FALSE;
	UINT32				cnt;
	OBJECTTYPE  *pObject;
	BOOLEAN			fFoundAttachment = FALSE;

	// ATE: If pTest object is NULL, test only for existence of syringes, etc...
	if ( pTestObject == NULL )
	{
		for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
		{
			pObject = &(pSoldier->inv[ cnt ]);

			if ( CompatibleItemForApplyingOnMerc( pObject ) )
			{
				if ( fOn != gbCompatibleAmmo[ cnt ] )
				{
					fFound = TRUE;
				}

				// IT's an OK calibere ammo, do something!
				// Render Item with specific color
				gbCompatibleAmmo[ cnt ] = fOn;

			}
		}


		if ( gpItemPointer != NULL )
		{
			if ( CompatibleItemForApplyingOnMerc( gpItemPointer ) )
			{
				// OK, Light up portrait as well.....
				if ( fOn )
				{
					gbCompatibleApplyItem = TRUE;
				}
				else
				{
					gbCompatibleApplyItem = FALSE;
				}

				fFound = TRUE;
			}
		}

		if ( fFound )
		{
			fInterfacePanelDirty = DIRTYLEVEL2;
			//HandleRenderInvSlots( pSoldier, DIRTYLEVEL2 );
		}

		return( fFound );
	}

	// First test attachments, which almost any type of item can have....
	for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
	{
		pObject = &(pSoldier->inv[ cnt ]);

//		if ( Item[ pObject->usItem ].fFlags & ITEM_HIDDEN_ADDON )
//		if ( Item[ pObject->usItem ].hiddenaddon  )
		if ( Item[ pObject->usItem ].hiddenaddon || (!Item[pObject->usItem].attachment && !Item[pTestObject->usItem].attachment) )
		{
			// don't consider for UI purposes
			continue;
		}

		/*if ( (UsingNewAttachmentSystem()==false && ValidAttachment( pObject->usItem, pTestObject )) ||
				 (UsingNewAttachmentSystem()==false && ValidAttachment( pTestObject->usItem, pObject )) ||
				 (UsingNewAttachmentSystem()==true && ValidItemAttachmentSlot(pTestObject, pObject->usItem, FALSE, FALSE, 0, cnt)) ||
				 (UsingNewAttachmentSystem()==true && ValidItemAttachmentSlot(pObject, pTestObject->usItem, FALSE, FALSE, 0, cnt)) ||
				 ValidLaunchable( pTestObject->usItem, pObject->usItem ) ||
				 ValidLaunchable( pObject->usItem, pTestObject->usItem ) )*/		
		if ( (ValidAttachment( pObject->usItem, pTestObject )) ||
				 (ValidAttachment( pTestObject->usItem, pObject )) ||
				 ValidLaunchable( pTestObject->usItem, pObject->usItem ) ||
				 ValidLaunchable( pObject->usItem, pTestObject->usItem ) )
		{
			fFoundAttachment = TRUE;

			if ( fOn != gbCompatibleAmmo[ cnt ] )
			{
				fFound = TRUE;
			}

			// IT's an OK calibere ammo, do something!
			// Render Item with specific color
			gbCompatibleAmmo[ cnt ] = fOn;
		}
	}

	//if ( !fFoundAttachment )
	//{
		if( ( Item [ pTestObject->usItem ].usItemClass & IC_GUN ) )
		{
			for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
			{
				pObject = &(pSoldier->inv[ cnt ]);

				if ( CompatibleAmmoForGun( pObject, pTestObject ) )
				{
					if ( fOn != gbCompatibleAmmo[ cnt ] )
					{
						fFound = TRUE;
					}

					// IT's an OK calibere ammo, do something!
					// Render Item with specific color
					gbCompatibleAmmo[ cnt ] = fOn;
				}
			}
		}

		else if( ( Item [ pTestObject->usItem ].usItemClass & IC_AMMO ) )
		{
			for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
			{
				pObject = &(pSoldier->inv[ cnt ]);

				if ( CompatibleGunForAmmo( pObject, pTestObject ) )
				{
					if ( fOn != gbCompatibleAmmo[ cnt ] )
					{
						fFound = TRUE;
					}

					// IT's an OK calibere ammo, do something!
					// Render Item with specific color
					gbCompatibleAmmo[ cnt ] = fOn;

				}
			}
		}
		else if ( CompatibleItemForApplyingOnMerc( pTestObject ) )
		{
			//If we are currently NOT in the Shopkeeper interface
			if( !( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
			{
				fFound = TRUE;
				gbCompatibleApplyItem = fOn;
			}
		}
	//}


	if ( !fFound )
	{
		for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
		{
			if ( gbCompatibleAmmo[ cnt ] )
			{
				fFound = TRUE;
				gbCompatibleAmmo[ cnt ] = FALSE;
			}

			if ( gbCompatibleApplyItem )
			{
				fFound = TRUE;
				gbCompatibleApplyItem = FALSE;
			}
		}
	}

	if ( fFound )
	{
		fInterfacePanelDirty = DIRTYLEVEL2;
		//HandleRenderInvSlots( pSoldier, DIRTYLEVEL2 );
	}

	return( fFound );

}

void ResetCompatibleItemArray( )
{
	INT32 cnt = 0;

	for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
	{
		if ( gbCompatibleAmmo[ cnt ] )
		{
			gbCompatibleAmmo[ cnt ] = FALSE;
		}
	}
}

BOOLEAN HandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, INT8 bInvPos, BOOLEAN fOn )
{
	INT32 cnt;
	OBJECTTYPE  *pTestObject;
	BOOLEAN			fFound = FALSE;

	//if we are in the shopkeeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		// if the inventory position is -1, this is a flag from the Shopkeeper interface screen
		//indicating that we are to use a different object to do the search
		if( bInvPos == -1 )
		{
			if( fOn )
			{
				if( gpHighLightedItemObject )
				{
					pTestObject = gpHighLightedItemObject;
//					gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
				}
				else
					return( FALSE );
			}
			else
			{
				gpHighLightedItemObject = NULL;

				for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
				{
					if ( gbCompatibleAmmo[ cnt ] )
					{
						fFound = TRUE;
						gbCompatibleAmmo[ cnt ] = FALSE;
					}
				}

				gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;
				return( TRUE );
			}
		}
		else
		{
			if( fOn )
			{
				pTestObject = &(pSoldier->inv[ bInvPos ]);
				gpHighLightedItemObject = pTestObject;
			}
			else
			{
				pTestObject = &(pSoldier->inv[ bInvPos ]);
				gpHighLightedItemObject = NULL;
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;
			}
		}
	}
	else
	{
//		if( fOn )

		if ( bInvPos == NO_SLOT )
		{
			pTestObject = NULL;
		}
		else
		{
			pTestObject = &(pSoldier->inv[ bInvPos ]);
		}

	}

	return( InternalHandleCompatibleAmmoUI( pSoldier, pTestObject, fOn ) );

}

void GetSlotInvXY( UINT8 ubPos, INT16 *psX, INT16 *psY )
{
	*psX = gSMInvData[ ubPos ].sX;
	*psY = gSMInvData[ ubPos ].sY;
}

void GetSlotInvHeightWidth( UINT8 ubPos, INT16 *psWidth, INT16 *psHeight )
{
	*psWidth	= gSMInvData[ ubPos ].sWidth;
	*psHeight   = gSMInvData[ ubPos ].sHeight;
}

void HandleNewlyAddedItems( SOLDIERTYPE *pSoldier, BOOLEAN *fDirtyLevel )
{
	// If item description up.... stop
	if ( gfInItemDescBox )
	{
		return;
	}

	UINT32 cnt;
	INT16  sX, sY;
	OBJECTTYPE		*pObject;

	for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
	{
		if ( pSoldier->inv.bNewItemCount[ cnt ] == -2 )
		{
			// Stop
			*fDirtyLevel = DIRTYLEVEL2;
			pSoldier->inv.bNewItemCount[ cnt ] = 0;
		}

		if ( pSoldier->inv.bNewItemCount[ cnt ] > 0 )
		{

			sX = gSMInvData[ cnt ].sX;
			sY = gSMInvData[ cnt ].sY;

			pObject = &(pSoldier->inv[ cnt ]);

			if ( pObject->exists() == false )
			{
				gbNewItem[ cnt ] = 0;
				continue;
			}

			INVRenderItem( guiSAVEBUFFER, pSoldier, pObject, sX, sY, gSMInvData[ cnt ].sWidth, gSMInvData[ cnt ].sHeight, DIRTYLEVEL2, NULL, 0, TRUE, us16BPPItemCyclePlacedItemColors[ pSoldier->inv.bNewItemCycleCount[ cnt ] ] );

		}

	}
}

void CheckForAnyNewlyAddedItems( SOLDIERTYPE *pSoldier )
{
	// OK, l0ok for any new...
	for ( UINT32 cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
	{
		if ( pSoldier->inv.bNewItemCount[ cnt ] == -1 )
		{
			pSoldier->inv.bNewItemCount[ cnt ]	= NEW_ITEM_CYCLES - 1;
		}
	}

}

void DegradeNewlyAddedItems( )
{
	UINT32 uiTime;
	UINT32 cnt, cnt2;
	SOLDIERTYPE		*pSoldier;

	// If time done
	uiTime = GetJA2Clock();

	if ( ( uiTime - guiNewlyPlacedItemTimer ) > 100 )
	{
		guiNewlyPlacedItemTimer = uiTime;

		for ( cnt2 = 0; cnt2 < gGameOptions.ubSquadSize; cnt2++ )
		{
			// GET SOLDIER
			if ( gTeamPanel[ cnt2 ].fOccupied )
			{
				pSoldier = MercPtrs[ gTeamPanel[ cnt2 ].ubID ];

				for ( cnt = 0; cnt < pSoldier->inv.size(); cnt++ )
				{
					if ( pSoldier->inv.bNewItemCount[ cnt ] > 0 )
					{
						// Decrement all the time!
						pSoldier->inv.bNewItemCycleCount[ cnt ]--;

						if ( pSoldier->inv.bNewItemCycleCount[ cnt ] == 0 )
						{
							// OK, cycle down....
							pSoldier->inv.bNewItemCount[ cnt ]--;

							if ( pSoldier->inv.bNewItemCount[ cnt ] == 0 )
							{
								// Stop...
								pSoldier->inv.bNewItemCount[ cnt ] = -2;
							}
							else
							{
								// Reset!
								pSoldier->inv.bNewItemCycleCount[ cnt ]	= NEW_ITEM_CYCLE_COUNT;
								continue;
							}
						}
					}
				}
			}
		}
	}
}


void InitItemInterface( )
{
	UINT32 cnt, cnt2;

	for ( cnt = 0, cnt2 = 0; cnt2 < 20; cnt+=3, cnt2++ )
	{
		us16BPPItemCyclePlacedItemColors[ cnt2 ] = Get16BPPColor( FROMRGB( ubRGBItemCyclePlacedItemColors[ cnt ], ubRGBItemCyclePlacedItemColors[ cnt + 1 ], ubRGBItemCyclePlacedItemColors[ cnt + 2] ) );
	}

}

// CHRISL: Function to display pocket inventory quantity based on object in cursor
void RenderPocketItemCapacity( UINT32 uiWhichBuffer, INT8 pCapacity, INT16 bPos, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT16 sX, INT16 sY )
{
	static CHAR16		pStr[ 100 ];

	// Can pocket hold the item in the cursor?
	if(InItemDescriptionBox( ))
		return;
	if(pSoldier != NULL && !CanItemFitInPosition( pSoldier, gpItemPointer, (INT8)bPos, FALSE ))
	{
		// Further check to see if the cursor item is valid ammo or a valid attachment
		if(!CompatibleAmmoForGun(gpItemPointer, &pSoldier->inv[bPos]) && !ValidAttachment(gpItemPointer->usItem, &(pSoldier->inv[bPos])))
		{
			return;
		}
	}

	// Setup display parameters
	SetFont( ITEM_FONT );
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_RED );
	if(pSoldier == NULL || (pCapacity != 0 && CanItemFitInPosition(pSoldier, gpItemPointer, (INT8)bPos, FALSE)))
	{
		// Adjust capacity to account for current items
		if(gpItemPointer->usItem == pObj->usItem)
		{
			SetFontForeground( FONT_GREEN );
			pCapacity = pCapacity - pObj->ubNumberOfObjects;
			if(pCapacity > 0)
				swprintf( pStr, L"+%d", pCapacity );
			else
				swprintf( pStr, L"-" );
		}
		else
			swprintf( pStr, L"%d", pCapacity );
	}
	else if(CompatibleAmmoForGun(gpItemPointer, &pSoldier->inv[bPos]) || ValidLaunchable(gpItemPointer->usItem, pSoldier->inv[bPos].usItem))
	{
		SetFontForeground( FONT_YELLOW );
		swprintf( pStr, L"L" );
	}
	else if((UsingNewAttachmentSystem()==false && ValidAttachment(gpItemPointer->usItem, &(pSoldier->inv[bPos]) )) ||
		(UsingNewAttachmentSystem()==true && ValidItemAttachmentSlot(&(pSoldier->inv[bPos]), gpItemPointer->usItem, FALSE, FALSE)))
	{
		SetFontForeground( FONT_YELLOW );
		swprintf( pStr, L"A" );
	}
	sX = sX + 1;

	// Display pocket capacity
	if ( uiWhichBuffer == guiSAVEBUFFER )
	{
		RestoreExternBackgroundRect( sX, sY, 15, 15 );
	}
	mprintf( sX, sY, pStr );
	gprintfinvalidate( sX, sY, pStr );
	return;
}
// CHRISL: New function to render silhouettes
void INVRenderSilhouette( UINT32 uiBuffer, INT16 PocketIndex, INT16 SilIndex, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight)
{
	ETRLEObject						*pTrav;
	HVOBJECT						hVObject;
	UINT32							usHeight, usWidth;
	INT16							sCenX, sCenY;
	
	if(gfSMDisableForItems)
		return;
	SetFont( ITEM_FONT );
	GetVideoObject( &hVObject, guiSILHOUETTE );
	if ( PocketIndex != ITEM_NOT_FOUND )
		SilIndex = LBEPocketType[PocketIndex].pSilhouette;

	pTrav = &(hVObject->pETRLEObject[ SilIndex ] );

	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;

	// CENTER IN SLOT!
	// CANCEL OFFSETS!
	sCenX =  sX + (INT16)( abs( sWidth - (double)usWidth ) / 2 ) - pTrav->sOffsetX;
	sCenY =  sY + (INT16)( abs( sHeight - (double)usHeight ) / 2 ) - pTrav->sOffsetY;

	BltVideoObjectOutlineFromIndex( uiBuffer, guiSILHOUETTE, SilIndex, sCenX, sCenY, 0 /*sOutlineColor*/, FALSE );

	if ( uiBuffer == FRAME_BUFFER )
	{
		InvalidateRegion( sX, sY, (INT16)(sX + sWidth), (INT16)(sY + sHeight ) );
	}
	else
	{
		RestoreExternBackgroundRect( sX, sY, sWidth, sHeight );
	}
}

// Flugente FTW 1: Function to get the number of the item condition string
UINT8 GetTemperatureString( FLOAT overheatpercentage, UINT32* apRed, UINT32* apGreen, UINT32* abBlue )
{
	*apRed   = (UINT32) ( gGameExternalOptions.ubOverheatThermometerRedOffset + (255 - gGameExternalOptions.ubOverheatThermometerRedOffset) * ( (max(1.0, overheatpercentage) - 1.0)/(max(1.0, overheatpercentage)) ) );
	*apGreen = 0;
	*abBlue  = 0;

	if (overheatpercentage < 0.17)			// very low
		return 0;
	else if (overheatpercentage < 0.33)		// low
		return 1;
	else if (overheatpercentage < 0.5)		// medium
		return 2;
	else if (overheatpercentage < 0.67)		// high
		return 3;
	else if (overheatpercentage < 0.83)		// very high
		return 4;
	else if (overheatpercentage < 1.0)		// dangerous
		return 5;
	else if (overheatpercentage < 2.0)		// CRITICAL
		return 6;
	else									// DRAMATIC
		return 7;
			
	// Default: unknown
	return 8;
}

void INVRenderItem( UINT32 uiBuffer, SOLDIERTYPE * pSoldier, OBJECTTYPE  *pObject, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, UINT8 fDirtyLevel, UINT8 *pubHighlightCounter, UINT8 ubStatusIndex, BOOLEAN fOutline, INT16 sOutlineColor, UINT8 iter )
{
	UINT16								uiStringLength;
	INVTYPE								*pItem;
  ETRLEObject						*pTrav;
	UINT32								usHeight, usWidth;
	INT16									sCenX, sCenY, sNewY, sNewX;
	HVOBJECT							hVObject;
	BOOLEAN								fLineSplit = FALSE;
	INT16									sFontX2 = 0, sFontY2 = 0;
	INT16									sFontX = 0, sFontY = 0;

	static CHAR16					pStr[ 100 ], pStr2[ 100 ];

	if ( pObject->exists() == false )
	{
		return;
	}

	if ( ubStatusIndex < RENDER_ITEM_ATTACHMENT1 )
	{
		pItem = &Item[ pObject->usItem ];
	}
	else
	{
		pItem = &Item[ (*pObject)[iter]->GetAttachmentAtIndex( ubStatusIndex - RENDER_ITEM_ATTACHMENT1 )->usItem ];
	}

	if ( fDirtyLevel == DIRTYLEVEL2 )
	{
		// TAKE A LOOK AT THE VIDEO OBJECT SIZE ( ONE OF TWO SIZES ) AND CENTER!
		GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );
		UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;
		pTrav = &(hVObject->pETRLEObject[ usGraphicNum ] );
		usHeight				= (UINT32)pTrav->usHeight;
		usWidth					= (UINT32)pTrav->usWidth;



		// CENTER IN SLOT!
		// CANCEL OFFSETS!
		sCenX =  sX + (INT16)( abs( sWidth - (double)usWidth ) / 2 ) - pTrav->sOffsetX;
		sCenY =  sY + (INT16)( abs( sHeight - (double)usHeight ) / 2 ) - pTrav->sOffsetY;

		// Shadow area
		if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]) BltVideoObjectOutlineShadowFromIndex( uiBuffer, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX - 2, sCenY + 2 );

		BltVideoObjectOutlineFromIndex( uiBuffer, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX, sCenY, sOutlineColor, fOutline );


		if ( uiBuffer == FRAME_BUFFER )
		{
			InvalidateRegion( sX, sY, (INT16)(sX + sWidth), (INT16)(sY + sHeight ) );
		}
		else
		{
			RestoreExternBackgroundRect( sX, sY, sWidth, sHeight );
		}

	}

	SetFont( ITEM_FONT );

	if ( fDirtyLevel != DIRTYLEVEL0 )
	{

		if ( ubStatusIndex < RENDER_ITEM_ATTACHMENT1 )
		{

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_DKGRAY );
#if 1
			//CHRISL: Moved this condition to the start so that stacked guns will show number in stack
			if ( ubStatusIndex != RENDER_ITEM_NOSTATUS )
			{
				// Now display # of items
				if ( pObject->ubNumberOfObjects > 1 )
				{
					SetFontForeground( FONT_GRAY4 );

					sNewY = sY + sHeight - 10;
					swprintf( pStr, L"%d", pObject->ubNumberOfObjects );

					// Get length of string
					uiStringLength=StringPixLength(pStr, ITEM_FONT );

					sNewX = sX + sWidth - uiStringLength - 4;

					if ( uiBuffer == guiSAVEBUFFER )
					{
						RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
					}
					mprintf( sNewX, sNewY, pStr );
					gprintfinvalidate( sNewX, sNewY, pStr );
				}

			}
#endif
			
			if ( pItem->usItemClass == IC_GUN && !Item[pObject->usItem].rocketlauncher )
			{
				sNewY = sY + sHeight - 10;
				sNewX = sX + 1;

				// Flugente: If we display the thermometer for overheating, move the ammo counter a bit to the right
				if ( gGameOptions.fWeaponOverheating == TRUE &&  gGameExternalOptions.fDisplayOverheatThermometer == TRUE )
					sNewX = sX + 6;

				// Flugente: check for underbarrel weapons and use that object if necessary
				OBJECTTYPE*	pObjShown = pObject;
				INVTYPE		*pItemShown = pItem;
				if ( pItem->usItemClass == IC_GUN && pSoldier && ( pSoldier->bWeaponMode == WM_ATTACHED_UB || pSoldier->bWeaponMode == WM_ATTACHED_UB_BURST || pSoldier->bWeaponMode == WM_ATTACHED_UB_AUTO ) )
				{
					OBJECTTYPE  *pObjectUnderBarrel = FindAttachment_UnderBarrel( pObject );

					if ( pObjectUnderBarrel )
					{
						INVTYPE		*pItemUnderBarrel;
						if ( ubStatusIndex < RENDER_ITEM_ATTACHMENT1 )
						{
							pItemUnderBarrel = &Item[ pObjectUnderBarrel->usItem ];
						}
						else
						{
							pItemUnderBarrel = &Item[ (*pObjectUnderBarrel)[iter]->GetAttachmentAtIndex( ubStatusIndex - RENDER_ITEM_ATTACHMENT1 )->usItem ];
						}

						pObjShown = pObjectUnderBarrel;
						pItemShown = pItemUnderBarrel;
					}
				}

				if ( pItemShown->usItemClass == IC_GUN && !Item[pObjShown->usItem].rocketlauncher )
				{
					SetFontForeground ( AmmoTypes[(*pObjShown)[iter]->data.gun.ubGunAmmoType].fontColour );
									
					// HEADROCK HAM 3.4: Get estimate of bullets left.
					if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
					{
						// Soldier doesn't know.
						EstimateBulletsLeft( pSoldier, pObjShown );
						swprintf( pStr, L"%s", gBulletCount );
					}
					else
					{
						swprintf( pStr, L"%d", (*pObjShown)[iter]->data.gun.ubGunShotsLeft );
					}
					
					if ( uiBuffer == guiSAVEBUFFER )
					{
						RestoreExternBackgroundRect( sNewX, sNewY, 20, 15 );
					}
					mprintf( sNewX, sNewY, pStr );
					gprintfinvalidate( sNewX, sNewY, pStr );

					sNewX = sX + 1;

					SetFontForeground( FONT_MCOLOR_DKGRAY );

					// Display 'JAMMED' if we are jammed
					if ( (*pObjShown)[iter]->data.gun.bGunAmmoStatus < 0 )
					{
						SetFontForeground( FONT_MCOLOR_RED );

						if ( sWidth >= ( BIG_INV_SLOT_WIDTH - 10 )  )
						{
							swprintf( pStr, TacticalStr[ JAMMED_ITEM_STR ] );
						}
						else
						{
							swprintf( pStr, TacticalStr[ SHORT_JAMMED_GUN ] );
						}

						VarFindFontCenterCoordinates( sX, sY, sWidth, sHeight , ITEM_FONT, &sNewX, &sNewY, pStr );

						mprintf( sNewX, sNewY, pStr );
						gprintfinvalidate( sNewX, sNewY, pStr );
					}
				}
			}
#if 0
			else
			{
				if ( ubStatusIndex != RENDER_ITEM_NOSTATUS )
				{
					// Now display # of items
					if ( pObject->ubNumberOfObjects > 1 )
					{
						SetFontForeground( FONT_GRAY4 );

						sNewY = sY + sHeight - 10;
						swprintf( pStr, L"%d", pObject->ubNumberOfObjects );

						// Get length of string
						uiStringLength=StringPixLength(pStr, ITEM_FONT );

						sNewX = sX + sWidth - uiStringLength - 4;

						if ( uiBuffer == guiSAVEBUFFER )
						{
							RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
						}
						mprintf( sNewX, sNewY, pStr );
						gprintfinvalidate( sNewX, sNewY, pStr );
					}

				}
			}
#endif
			if ( ItemHasAttachments( pObject, pSoldier, iter ) )
			{
				if ( !IsGrenadeLauncherAttached( pObject, iter ) )
				{
					SetFontForeground( FONT_GREEN );
				}
				else
				{
					SetFontForeground( FONT_YELLOW );
				}

				sNewY = sY;
				swprintf( pStr, L"*" );

				// Get length of string
				uiStringLength=StringPixLength(pStr, ITEM_FONT );

				sNewX = sX + sWidth - uiStringLength - 4;

				if ( uiBuffer == guiSAVEBUFFER )
				{
					RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
				}
				mprintf( sNewX, sNewY, pStr );
				gprintfinvalidate( sNewX, sNewY, pStr );
			}

			// Flugente FTW 1
			if ( gGameOptions.fWeaponOverheating == TRUE &&  gGameExternalOptions.fDisplayOverheatThermometer == TRUE && ( pItem->usItemClass & (IC_GUN | IC_LAUNCHER) || Item[pObject->usItem].barrel == TRUE ) )
			{	
				OBJECTTYPE*	pObjShown = pObject;

				if ( pSoldier )
					pObjShown = pSoldier->GetUsedWeapon(pObject);

				FLOAT overheatjampercentage = GetGunOverheatJamPercentage( pObjShown );
												
				UINT32 red, green, blue;
				UINT8 TemperatureStringNum = GetTemperatureString( overheatjampercentage, &red, &green, &blue );

				UINT16 colour = Get16BPPColor( FROMRGB( red, green, blue ) );

				DrawItemUIBarEx( pObjShown, DRAW_ITEM_TEMPERATURE, sX, sY + sHeight-1, ITEMDESC_ITEM_STATUS_WIDTH, sHeight-1, colour, colour, TRUE, guiSAVEBUFFER );								
			}

			// display symbol if we are leaning our weapon on something
			// display only if eapon resting is allowed, display is allowed, item is a gun/launcher, we are a person, we hold the gun in our hand, and we are resting the gun
			if ( gGameExternalOptions.fWeaponResting && gGameExternalOptions.fDisplayWeaponRestingIndicator && pItem->usItemClass & (IC_GUN | IC_LAUNCHER) && pSoldier &&  &(pSoldier->inv[pSoldier->ubAttackingHand]) == pObject && pSoldier->IsWeaponMounted() )
			{
				SetRGBFontForeground( 95, 160, 154 );
												
				sNewY = sY;
				swprintf( pStr, L"M" );

				// Get length of string
				uiStringLength=StringPixLength(pStr, ITEM_FONT );

				sNewX = sX + sWidth - uiStringLength - 13;

				if ( uiBuffer == guiSAVEBUFFER )
				{
					RestoreExternBackgroundRect( sNewX, sNewY, uiStringLength, 15 );
				}
				mprintf( sNewX, sNewY, pStr );
				gprintfinvalidate( sNewX, sNewY, pStr );
			}

			if ( gGameExternalOptions.fScopeModes && gGameExternalOptions.fDisplayScopeModes && pSoldier && pObject == &(pSoldier->inv[HANDPOS] ) && Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN )
			{
				sNewX = sX + 5; // rather arbitrary
				sNewY = sY;

				// added by Flugente
				// HEADROCK HAM 4: Advanced Icons
				VOBJECT_DESC    VObjectDesc;
				VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
				strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoAdvancedIcons.STI" );
				AddStandardVideoObject( &VObjectDesc, &guiItemInfoAdvancedIcon );

				std::map<INT8, OBJECTTYPE*> ObjList;
				GetScopeLists(&pSoldier->inv[HANDPOS], ObjList);
								
				if (ObjList[pSoldier->bScopeMode] != NULL && IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_SCOPE ) )
				{					
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 54, sNewX, sNewY, VO_BLT_TRANSSHADOW, NULL );

					SetFontForeground( FONT_ORANGE );
					FLOAT scopemagfactor = Item[ObjList[pSoldier->bScopeMode]->usItem].scopemagfactor;

					INT16 sMagX = sNewX + 9;

					if ( scopemagfactor > (INT8)scopemagfactor )
						swprintf( pStr, L"%2.1f", scopemagfactor );
					else
					{
						swprintf( pStr, L"%2.0f", scopemagfactor );
					}

					// Get length of string
					uiStringLength=StringPixLength(pStr, ITEM_FONT );
										
					if ( uiBuffer == guiSAVEBUFFER )
					{
						RestoreExternBackgroundRect( sNewX, sNewY, 20, 15 );
					}
					mprintf( sMagX, sNewY, pStr );
					gprintfinvalidate( sMagX, sNewY, pStr );
				}
				// improved iron sights are attachable iron sights (the 'normal' iron sight is the gun itself)
				else if (ObjList[pSoldier->bScopeMode] != NULL &&  IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_IRONSIGHT ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 52, sNewX, sNewY, VO_BLT_TRANSSHADOW, NULL );

					SetFontForeground( FONT_ORANGE );

					INT16 sMagX = sNewX + 13;

					swprintf( pStr, L"*" );

					// Get length of string
					uiStringLength=StringPixLength(pStr, ITEM_FONT );
										
					if ( uiBuffer == guiSAVEBUFFER )
					{
						RestoreExternBackgroundRect( sNewX, sNewY, 20, 15 );
					}
					mprintf( sMagX, sNewY, pStr );
					gprintfinvalidate( sMagX, sNewY, pStr );
				}
				else if (ObjList[pSoldier->bScopeMode] != NULL &&  IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_SIGHT ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 53, sNewX, sNewY, VO_BLT_TRANSSHADOW, NULL );

					RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
				}
				else
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoAdvancedIcon, 52, sNewX, sNewY, VO_BLT_TRANSSHADOW, NULL );

					RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
				}								
			}

			if((UsingNewInventorySystem() == true))
			{
				// CHRISL: Display astrisk when LBENODE active
				if ( pObject->HasAnyActiveLBEs(pSoldier, iter) )
				{
					SetFontForeground( FONT_BLUE );

					sNewY = sY;
					swprintf( pStr, L"*" );

					// Get length of string
					uiStringLength=StringPixLength(pStr, ITEM_FONT );

					sNewX = sX + sWidth - uiStringLength - 4;

					if ( uiBuffer == guiSAVEBUFFER )
					{
						RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
					}
					mprintf( sNewX, sNewY, pStr );
					gprintfinvalidate( sNewX, sNewY, pStr );

				}
			}

			if ( pSoldier && pObject == &(pSoldier->inv[HANDPOS] ) && pSoldier->bWeaponMode != WM_NORMAL && Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN )
			{
				sNewY = sY + 13; // rather arbitrary
				if ( pSoldier->bWeaponMode == WM_BURST )
				{
					swprintf( pStr, New113Message[MSG113_BRST] );
					SetFontForeground( FONT_RED );
				}
				else if(pSoldier->bWeaponMode == WM_AUTOFIRE)
				{
					swprintf( pStr, New113Message[MSG113_AUTO] );
					SetFontForeground( FONT_RED );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_GL)
				{
					swprintf( pStr, New113Message[MSG113_GL] );
					SetFontForeground( FONT_YELLOW );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST)
				{
					swprintf( pStr, New113Message[MSG113_GL_BRST] );
					SetFontForeground( FONT_YELLOW );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO)
				{
					swprintf( pStr, New113Message[MSG113_GL_AUTO] );
					SetFontForeground( FONT_YELLOW );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_UB)
				{
					swprintf( pStr, New113Message[MSG113_UB] );
					SetFontForeground( FONT_ORANGE );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_UB_BURST)
				{
					swprintf( pStr, New113Message[MSG113_UB_BRST] );
					SetFontForeground( FONT_ORANGE );
				}
				else if(pSoldier->bWeaponMode == WM_ATTACHED_UB_AUTO)
				{
					swprintf( pStr, New113Message[MSG113_UB_AUTO] );
					SetFontForeground( FONT_ORANGE );
				}

				// Get length of string
				uiStringLength=StringPixLength(pStr, ITEM_FONT );

				sNewX = sX + sWidth - uiStringLength - 4;

				if ( uiBuffer == guiSAVEBUFFER )
				{
					RestoreExternBackgroundRect( sNewX, sNewY, 15, 15 );
				}
				mprintf( sNewX, sNewY, pStr );
				gprintfinvalidate( sNewX, sNewY, pStr );

			}
		}
	}

	if ( pubHighlightCounter != NULL )
	{
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_LTGRAY );

		// DO HIGHLIGHT
		if ( *pubHighlightCounter )
		{
			// Set string
			if ( ubStatusIndex < RENDER_ITEM_ATTACHMENT1 )
			{
				swprintf( pStr, L"%s", ShortItemNames[ pObject->usItem ] );
			}
			else
			{
				OBJECTTYPE* pAttachment = (*pObject)[iter]->GetAttachmentAtIndex(ubStatusIndex - RENDER_ITEM_ATTACHMENT1);
				if (pAttachment->exists()) {
					swprintf( pStr, L"%s", ShortItemNames[ pAttachment->usItem ] );
				}
			}

			fLineSplit = WrapString( pStr, pStr2, WORD_WRAP_INV_WIDTH, ITEM_FONT );

			VarFindFontCenterCoordinates( sX, sY, sWidth, sHeight , ITEM_FONT, &sFontX, &sFontY, pStr );
			sFontY = sY + 1;
			gprintfinvalidate( sFontX, sFontY, pStr );

			if ( fLineSplit )
			{
				VarFindFontCenterCoordinates( sX, sY, sWidth, sHeight , ITEM_FONT, &sFontX2, &sFontY2, pStr2 );
				sFontY2 = sY + 13;
				gprintfinvalidate( sFontX2, sFontY2, pStr2 );
			}

		}

		if ( *pubHighlightCounter == 2 )
		{
			mprintf( sFontX, sFontY, pStr );

			if ( fLineSplit )
			{
				mprintf( sFontX2, sFontY2, pStr2 );
			}
		}
		else if ( *pubHighlightCounter == 1 )
		{
			*pubHighlightCounter = 0;
			gprintfRestore( sFontX, sFontY, pStr );

			if ( fLineSplit )
			{
				gprintfRestore( sFontX2, sFontY2, pStr2 );
			}
		}
	}
}

// HEADROCK HAM 5.1: Render item in a BigItem sector inventory slot.
// This function works largely like the one above it, with several exceptions. For one, the BigItemPic is used,
// which allows us to add lots of data. Since this is only used in the sector inventory, we can forgo things
// like dirtylevels and just draw everything here.
void MAPINVRenderItem( UINT32 uiBuffer, SOLDIERTYPE * pSoldier, OBJECTTYPE  *pObject, UINT32 uiItemGraphicNum, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, BOOLEAN fOutline, INT16 sOutlineColor )
{
	UINT16 uiStringLength;
	INVTYPE	*pItem;
	ETRLEObject	*pTrav;
	UINT32 usHeight, usWidth;
	INT16 sCenX, sCenY, sNewY, sNewX;
	HVOBJECT hVObject;
	BOOLEAN	fLineSplit = FALSE;
	INT16 sFontX2 = 0, sFontY2 = 0;
	INT16 sFontX = 0, sFontY = 0;

	static CHAR16					pStr[ 100 ], pStr2[ 100 ];

	if ( pObject->exists() == false )
	{
		return;
	}

	pItem = &Item[ pObject->usItem ];

	// Get the video object for this BigItem image. We get this fed from the previous function (inventory render)
	// which already selected the proper image.
	GetVideoObject( &hVObject, uiItemGraphicNum );

	// Check height and width...
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;

	// Center in the slot. Dimensions are also supplied to us by the calling function.
	sCenX =  sX + (INT16)( abs( sWidth - (double)usWidth ) / 2 ) - pTrav->sOffsetX;
	sCenY =  sY + (INT16)( abs( sHeight - (double)usHeight ) / 2 ) - pTrav->sOffsetY;

	// If option, draw item shadow.
	if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ])
	{
		BltVideoObjectOutlineShadow( guiSAVEBUFFER, hVObject, 0, sCenX - 2, sCenY + 2  );
	}
	// Draw the item on-screen.
	BltVideoObject( uiBuffer , hVObject, 0, sCenX, sCenY , VO_BLT_SRCTRANSPARENCY,NULL );

	// OUTLINE
	if (fOutline)
	{
		DrawItemOutlineZoomedInventory( sX, sY, sX+sWidth, sY+sHeight, sOutlineColor, uiBuffer );
	}

	//////////////////////////////////
	// Data display
	//
	// Given the larger size of the slots used in zoom view, it is possible to add significant amounts of information.
	// This function draws more info than a small-item render. Note the changes below.

	SetFontBackground( FONT_MCOLOR_BLACK );

	///////////////// # OF ITEMS ///////////////
	if ( pObject->ubNumberOfObjects > 1 )
	{
		// Set font properties.
		SetFont( FONT14ARIAL );
		SetFontForeground( FONT_GRAY4 );

		// Get number of objects in this slot
		swprintf( pStr, L"x%d", pObject->ubNumberOfObjects );

		// Get length of string
		uiStringLength=StringPixLength(pStr, ITEM_FONT );
		// We draw close to the bottom-left corner of the slot.
		// Locate starting X.
		sNewX = (sX + sWidth - uiStringLength) - 10;
		// Locate starting Y
		sNewY = (sY + sHeight) - 13;

		// Restore background
		if ( uiBuffer == guiSAVEBUFFER )
		{
			RestoreExternBackgroundRect( sNewX, sNewY, sWidth, sHeight );
		}

		// Print
		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );
	}

	//////////////////// GUN DATA //////////////////
	if ( pItem->usItemClass == IC_GUN && !Item[pObject->usItem].rocketlauncher )
	{
		//////////////// AMMO REMAINING

		// Set font properties
		SetFont( LARGEFONT1 );
		// Get color from ammo details
		UINT8 ubAmmoColor =	AmmoTypes[(*pObject)[0]->data.gun.ubGunAmmoType].fontColour;
		SetFontForeground ( ubAmmoColor );

		sNewX = sX + 5;
		sNewY = ((sY + sHeight) - GetFontHeight( LARGEFONT1 )) - 2;
		
		// HEADROCK HAM 3.4: Get estimate of bullets left.
		if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			// Soldier doesn't know.
			swprintf( pStr, L"%s", "??" );
		}
		else
		{
			swprintf( pStr, L"%d", (*pObject)[0]->data.gun.ubGunShotsLeft );
		}

		// Restore background
		if ( uiBuffer == guiSAVEBUFFER )
		{
			RestoreExternBackgroundRect( sNewX, sNewY, 20, 15 );
		}

		// Print
		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );

		// Record how long this text was in pixels.
		UINT16 usAmmoWidth = StringPixLength( pStr, LARGEFONT1 );

		// Set font properties
		SetFontForeground( FONT_GRAY4 );
		swprintf( pStr, L"/" );
		sNewX += usAmmoWidth;
		mprintf( sNewX, sNewY, pStr );
		usAmmoWidth = StringPixLength( pStr, LARGEFONT1 );

		//////////////// AMMO MAX CAPACITY

		// Set font properties
		SetFontForeground( FONT_GRAY4 );
		SetFont( TINYFONT1 );

		// Find difference in width and height.
		INT16 sFontHeightDifference = 1;
		//INT16 sFontHeightDifference = GetFontHeight( LARGEFONT1 ) - GetFontHeight( FONT14ARIAL );
		sNewX += usAmmoWidth;
		sNewY += sFontHeightDifference;

		// Print total magazine size
		swprintf( pStr, L"%d", GetMagSize(pObject) );
		
		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );

		///////////////// Display 'JAMMED' if we are jammed
		if ( (*pObject)[0]->data.gun.bGunAmmoStatus < 0 )
		{
			SetFont( FONT12ARIAL );
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_RED );

			swprintf( pStr, TacticalStr[ JAMMED_ITEM_STR ] );

			VarFindFontCenterCoordinates( sX, sY, sWidth, sHeight , FONT12ARIAL, &sNewX, &sNewY, pStr );

			mprintf( sNewX, sNewY, pStr );
			gprintfinvalidate( sNewX, sNewY, pStr );
		}

		// Reset font color
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}

	//////////////////// AMMO DATA //////////////////
	if ( pItem->usItemClass & IC_AMMO )
	{
		//////////////// AMMO REMAINING

		// Set font properties
		SetFont( LARGEFONT1 );
		// Get color from ammo details
		UINT8 ubAmmoColor =	AmmoTypes[Magazine[Item[ pObject->usItem ].ubClassIndex].ubAmmoType].fontColour;
		SetFontForeground ( ubAmmoColor );

		sNewX = sX + 5;
		sNewY = ((sY + sHeight) - GetFontHeight( LARGEFONT1 )) - 2;
		
		// HEADROCK HAM 3.4: Get estimate of bullets left.
		if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			// Soldier doesn't know.
			swprintf( pStr, L"%s", "??" );
		}
		else
		{
			swprintf( pStr, L"%d", (*pObject)[0]->data.ubShotsLeft );
		}

		// Restore background
		if ( uiBuffer == guiSAVEBUFFER )
		{
			RestoreExternBackgroundRect( sNewX, sNewY, 20, 15 );
		}

		// Print
		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );

		// Record how long this text was in pixels.
		UINT16 usAmmoWidth = StringPixLength( pStr, LARGEFONT1 );

		// Set font properties
		SetFontForeground( FONT_GRAY4 );
		swprintf( pStr, L"/" );
		sNewX += usAmmoWidth;
		mprintf( sNewX, sNewY, pStr );
		usAmmoWidth = StringPixLength( pStr, LARGEFONT1 );

		//////////////// AMMO MAX CAPACITY

		// Set font properties
		SetFontForeground( FONT_GRAY4 );
		SetFont( TINYFONT1 );

		// Find difference in width and height.
		INT16 sFontHeightDifference = 1;
		//INT16 sFontHeightDifference = GetFontHeight( LARGEFONT1 ) - GetFontHeight( FONT14ARIAL );
		sNewX += usAmmoWidth;
		sNewY += sFontHeightDifference;

		// Print total magazine size
		swprintf( pStr, L"%d", Magazine[Item[ pObject->usItem ].ubClassIndex].ubMagSize );
		
		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );

		// Reset font color
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}

	////////// LBE /////////////
	if((UsingNewInventorySystem() == true) && pObject->HasAnyActiveLBEs(pSoldier, 0) )
	{
		INT16 sOffsetX = 0;
		INT16 sOffsetY = 0;

		LBENODE* pLBE = NULL;
		UINT16 usCountLBEItems = 0;

		pLBE = pObject->GetLBEPointer(0);
		UINT32 lClass = pLBE->lbeClass;

		std::vector<INT8> pocketKey;

		switch (lClass)
		{
			case THIGH_PACK:
				GetLBESlots(LTHIGHPOCKPOS, pocketKey);
				break;
			case VEST_PACK:
				GetLBESlots(VESTPOCKPOS, pocketKey);
				break;
			case COMBAT_PACK:
				GetLBESlots(CPACKPOCKPOS, pocketKey);
				break;
			case BACKPACK:
				GetLBESlots(BPACKPOCKPOS, pocketKey);
				break;
		}

		for(unsigned int cnt=0; cnt<pocketKey.size(); cnt++)
		{
			if(pLBE->inv[cnt].exists() == true)
				usCountLBEItems++;
		}

		GetVideoObject( &hVObject, guiAttachmentAsterisks );

		// Draw large yellow asterisk.
		pTrav = &(hVObject->pETRLEObject[ 3 ] );
		sOffsetX += (UINT32)pTrav->usWidth;

		sNewX = ((sX + sWidth) - 6) - sOffsetX;
		sNewY = sY + 4;
		BltVideoObject( uiBuffer , hVObject, 3, sNewX, sNewY , VO_BLT_SRCTRANSPARENCY,NULL );

		usCountLBEItems--;
		for (INT16 x = 0; x < usCountLBEItems; x++)
		{
			// Draw small blue asterisk
			pTrav = &(hVObject->pETRLEObject[ 4 ] );
			sOffsetX += ((x+1) % 2) * (UINT32)pTrav->usWidth;
			sOffsetY = (x % 2) * ((UINT32)pTrav->usHeight + 1);

			sNewX = ((sX + sWidth) - 6) - sOffsetX;
			sNewY = sY + 4 + sOffsetY;
			BltVideoObject( uiBuffer , hVObject, 4, sNewX, sNewY , VO_BLT_SRCTRANSPARENCY,NULL );
		}
	}
	////////// ATTACHMENTS /////////////
	// Attachments are displayed as a series of asterisks rather than the original one asterisk.
	// There's the option here to use differently colored (or shaped) asterisks by editing the STI.
	else if ( ItemHasAttachments( pObject, pSoldier, 0 ) && !(Item[pObject->usItem].usItemClass & IC_AMMO) )
	{
		// This offset is used to draw the asterisks one after the other.
		INT16 sOffsetX = 0;
		INT16 sOffsetY = 0;
		
		// Record the number of asterisks we'll need from each type.
		INT8 iCurAsterisk = 0;
		UINT16 uiNumAttachments = 0;
		UINT16 uiNumAttachmentsGeneral = 0;
		UINT16 uiNumAttachmentsGL = 0;
		UINT16 uiNumAttachmentsOptical = 0;
		UINT16 uiNumAttachmentsRecoil = 0;

		// Iterate through the attachments.
		if (pObject->exists() == true) 
		{
			for (attachmentList::iterator iter = (*pObject)[0]->attachments.begin(); iter != (*pObject)[0]->attachments.end(); ++iter)
			{
				if (iter->exists() == false)
				{
					continue;
				}

				iCurAsterisk = ATTACHMENT_GENERAL;
				if (Item[iter->usItem].grenadelauncher )
				{
					//iCurAsterisk = ATTACHMENT_GL;
					uiNumAttachmentsGL++;
				}
				else if (Item[iter->usItem].visionrangebonus > 0 || Item[iter->usItem].dayvisionrangebonus > 0 ||
							Item[iter->usItem].nightvisionrangebonus> 0 || Item[iter->usItem].brightlightvisionrangebonus > 0 ||
							Item[iter->usItem].cavevisionrangebonus > 0 )
				{
					iCurAsterisk = ATTACHMENT_OPTICAL;
				}
				else
				{
					if (UsingNewCTHSystem() == true)
					{
						if (Item[iter->usItem].scopemagfactor > 1.0f || Item[iter->usItem].projectionfactor > 1.0f )
							
						{
							iCurAsterisk = ATTACHMENT_OPTICAL;
						}
						else
						{
							for (INT8 x = 0; x < 3; x++)
							{
								if (Item[iter->usItem].counterforceaccuracymodifier[x] > 0 ||
									Item[iter->usItem].maxcounterforcemodifier[x] > 0 ||
									Item[iter->usItem].PercentRecoilModifier < 0 ||
									Item[iter->usItem].RecoilModifierX < 0 ||
									Item[iter->usItem].RecoilModifierY < 0 )
								{
									iCurAsterisk = ATTACHMENT_RECOILREDUCTION;
								}
							}
						}
					}
					else
					{
						if ( (Item[iter->usItem].minrangeforaimbonus > 0 && Item[iter->usItem].aimbonus > 0 ) ||
							(Item[iter->usItem].bestlaserrange > 0 && Item[iter->usItem].tohitbonus > 0 ) )
						{
							iCurAsterisk = ATTACHMENT_OPTICAL;
						}
						else if ( Item[iter->usItem].autofiretohitbonus > 0 || Item[iter->usItem].bursttohitbonus > 0 ||
							Item[iter->usItem].bipod > 0 )
						{
							iCurAsterisk = ATTACHMENT_RECOILREDUCTION;
						}
					}
				}
				uiNumAttachments++;
			}

			uiNumAttachmentsGeneral = uiNumAttachments - uiNumAttachmentsGL;

			if (uiNumAttachments > 0)
			{
				// More than one attachment. Draw a large attachment asterisk.
				GetVideoObject( &hVObject, guiAttachmentAsterisks );

				if (uiNumAttachmentsGL > 0)
				{
					// Draw large yellow asterisk.
					pTrav = &(hVObject->pETRLEObject[ 1 ] );
					sOffsetX += (UINT32)pTrav->usWidth;

					sNewX = ((sX + sWidth) - 6) - sOffsetX;
					sNewY = sY + 4;
					BltVideoObject( uiBuffer , hVObject, 1, sNewX, sNewY , VO_BLT_SRCTRANSPARENCY,NULL );
					
					// reduce the number of GL attachments
					uiNumAttachmentsGL--;
					// Pool the remaining GLs into the general attachments
					uiNumAttachmentsGeneral += uiNumAttachmentsGL;
				}
				else
				{
					// Draw large green asterisk.
					pTrav = &(hVObject->pETRLEObject[ 0 ] );
					sOffsetX += (UINT32)pTrav->usWidth;

					sNewX = ((sX + sWidth) - 6) - sOffsetX;
					sNewY = sY + 4;
					BltVideoObject( uiBuffer , hVObject, 0, sNewX, sNewY , VO_BLT_SRCTRANSPARENCY,NULL );
				}
				
				uiNumAttachments--;

				// Any attachments remaining?
				if (uiNumAttachments > 0)
				{
					for (INT16 x = 0; x < uiNumAttachments; x++)
					{
						// Draw small green asterisk
						pTrav = &(hVObject->pETRLEObject[ 2 ] );
						sOffsetX += ((x+1) % 2) * (UINT32)pTrav->usWidth;
						sOffsetY = (x % 2) * ((UINT32)pTrav->usHeight + 1);
	
						sNewX = ((sX + sWidth) - 6) - sOffsetX;
						sNewY = sY + 4 + sOffsetY;
						BltVideoObject( uiBuffer , hVObject, 2, sNewX, sNewY , VO_BLT_SRCTRANSPARENCY,NULL );
					}
				}
			}
		}
	}
	/*
	else if( pItem->usItemClass & IC_AMMO && pObject->ubNumberOfObjects > 1)
	{
		SetFontForeground( FONT_GRAY1 );
		SetFont( FONT10ARIAL );
		
		UINT16 uiTotalAmmo = 0;
		for (INT16 x = 0; x < pObject->ubNumberOfObjects; x++)
		{
			uiTotalAmmo += (*pObject)[x]->data.ubShotsLeft;
		}

		swprintf( pStr, L"(%d)", uiTotalAmmo );

		sNewX = ((sX + sWidth) - 4) - StringPixLength( pStr, FONT10ARIAL );
		sNewY = sY + 3;

		mprintf( sNewX, sNewY, pStr );
		gprintfinvalidate( sNewX, sNewY, pStr );

		// Reset font color
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}
	*/
}



BOOLEAN InItemDescriptionBox( )
{
	return( gfInItemDescBox );
}

void CycleItemDescriptionItem( INT16 sX, INT16 sY )
{
	INT16 usOldItem;

	// Delete old box...
	DeleteItemDescriptionBox( );

	// Cycle item....
	usOldItem = CycleItems(gpItemDescSoldier->inv[ HANDPOS ].usItem);

	CreateItem( (UINT16)usOldItem, 100, &( gpItemDescSoldier->inv[ HANDPOS ] ) );

	InternalInitItemDescriptionBox( &( gpItemDescSoldier->inv[ HANDPOS ] ), sX, sY, gubItemDescStatusIndex, gpItemDescSoldier );
}

INT16 CycleItems( INT16 usOldItem )
{
	if ( _KeyDown( SHIFT ) )
	{
		usOldItem--;
		if ( usOldItem < 1 )
		{
			usOldItem = MAXITEMS-1;
		}
		while ( usOldItem > 0 && ( Item[usOldItem].usItemClass == IC_NONE || Item[usOldItem].usItemClass == 0 ))
		{
			usOldItem--;
			if(usOldItem < 1)
				usOldItem = MAXITEMS-1;
		}
	}
	else
	{
		usOldItem++;
		if ( usOldItem > MAXITEMS )
		{
			usOldItem = 0;
		}
		while (usOldItem < MAXITEMS && (Item[usOldItem].usItemClass == IC_NONE || Item[usOldItem].usItemClass == 0 ))
		{
			usOldItem++;
			if(usOldItem >= MAXITEMS)
				usOldItem = 1;
		}
	}

	if ( usOldItem > MAXITEMS )
	{
		usOldItem = 0;
	}

	return(usOldItem);
}

BOOLEAN InitItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex )
{
	OBJECTTYPE *pObject;

//DEF:
	//if we are in the shopkeeper screen, and we are to use the
	if( guiCurrentScreen == SHOPKEEPER_SCREEN && ubPosition == 255 )
	{
		pObject = pShopKeeperItemDescObject;
	}

	//else use item from the hand position
	else
	{
		pObject = &(pSoldier->inv[ ubPosition ] );
	}

	return( InternalInitItemDescriptionBox( pObject, sX, sY, ubStatusIndex, pSoldier, ubPosition ) );
}

// HEADROCK: The following two functions set various coordinates for use by the description box, depending on
// an options.ini setting that determines whether the Enhanced box or the Original box is used.
void InitItemDescriptionBoxStartCoords( BOOLEAN fIsEnhanced, BOOLEAN fUsingNAS )
{
	UINT8	mode = UsingEDBSystem();
	if(fUsingNAS){
		if( UsingNewInventorySystem() == true )	// NIV
		{
			ITEMDESC_START_X	= INTERFACE_START_X + 115;
			ITEMDESC_START_Y	= (1 + INV_INTERFACE_START_Y);
			ITEMDESC_HEIGHT		= 195;
			ITEMDESC_WIDTH		= 335;
}
		else	// OIV
		{
			ITEMDESC_HEIGHT		= 193;
			ITEMDESC_WIDTH		= 430;
			ITEMDESC_START_X	= INTERFACE_START_X + 214;
			ITEMDESC_START_Y	= (1-(ITEMDESC_HEIGHT-INV_INTERFACE_HEIGHT) + INV_INTERFACE_START_Y);
		}

		if(UsingNewInventorySystem() == true && (guiCurrentScreen == GAME_SCREEN || guiCurrentScreen == SHOPKEEPER_SCREEN))
		{
			if (iResolution >= _640x480 && iResolution < _800x600)
				ITEMDESC_WIDTH = 526;
			else if (iResolution < _1024x768)
				ITEMDESC_WIDTH = 686;
			else
				ITEMDESC_WIDTH = 910;
		}

		//CHRISL: This allows EDB to work in Strategic OIV mode
		if(guiCurrentScreen == MAP_SCREEN)
		{
			if (iResolution >= _640x480 && iResolution < _800x600)
				ITEMDESC_HEIGHT = 268;
			else if(iResolution >= _1024x768 || UsingNewInventorySystem() == true)	// 1024x768 or NIV
				ITEMDESC_HEIGHT = 490;
			else if (iResolution < _1024x768)
				ITEMDESC_HEIGHT = (mode > 0) ? 490 : 373;

			ITEMDESC_WIDTH	= ((UsingNewInventorySystem() == true && iResolution >= _800x600)) ? 272 : 272;
		}
	} 
	else 
	{
		if( UsingNewInventorySystem() == true )	// ODB/NIV
		{
			// HEADROCK HAM 4: 
			ITEMDESC_START_X	= INTERFACE_START_X + 115;
			ITEMDESC_START_Y	= (1 + INV_INTERFACE_START_Y);
			ITEMDESC_HEIGHT		= 195;
			ITEMDESC_WIDTH		= 335; // OIV only
		}
		else	// ODB/OIV
		{
			ITEMDESC_START_X	= INTERFACE_START_X + 214;
			ITEMDESC_START_Y	= (1 + INV_INTERFACE_START_Y);
			ITEMDESC_HEIGHT		= 133;
			ITEMDESC_WIDTH		= 320; // OIV only
		}

		if(UsingNewInventorySystem() == true && guiCurrentScreen == GAME_SCREEN || guiCurrentScreen == SHOPKEEPER_SCREEN)
		{
			if (iResolution >= _640x480 && iResolution < _800x600)
				ITEMDESC_WIDTH = 526;
			else if (iResolution < _1024x768)
				ITEMDESC_WIDTH = 686;
			else
				ITEMDESC_WIDTH = 910;
		}

		//CHRISL: This allows EDB to work in Strategic OIV mode
		if(guiCurrentScreen == MAP_SCREEN)
		{
			if (iResolution >= _640x480 && iResolution < _800x600)
				ITEMDESC_HEIGHT = 268;
			else if(iResolution >= _1024x768 || UsingNewInventorySystem() == true)	// 1024x768 or NIV
				ITEMDESC_HEIGHT = 490;
			else if (iResolution < _1024x768)
				ITEMDESC_HEIGHT = 373;

			ITEMDESC_WIDTH	= ((UsingNewInventorySystem() == true && iResolution >= _800x600)) ? 272 : 272;
		}
	}
}

BOOLEAN InitKeyItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex )
{
	OBJECTTYPE *pObject;

	AllocateObject( &pObject );
	CreateKeyObject( pObject, pSoldier->pKeyRing[ ubPosition ].ubNumber ,pSoldier->pKeyRing[ ubPosition ].ubKeyID );

	return( InternalInitItemDescriptionBox( pObject, sX, sY, ubStatusIndex, pSoldier ) );
}

BOOLEAN InternalInitItemDescriptionBox( OBJECTTYPE *pObject, INT16 sX, INT16 sY, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier, UINT8 ubPosition )
{
	VOBJECT_DESC    VObjectDesc;
	CHAR8 ubString[48];
	INT32		cnt;
	CHAR16		pStr[10];
	INT16	usX, usY;
	INT16		sForeColour;

	//CHRISL: We only want this condition to be true when looking at MONEY.  Not IC_MONEY since we can't actually split
	//	things like gold nuggets or wallets.
	// ADB: Make sure the current object isn't money if there's something in hand
	//if (Item[ pObject->usItem].usItemClass & IC_MONEY && gpItemPointer != NULL && gpItemPointer->usItem != 0) {
	if(pObject->usItem == MONEY && gpItemPointer != NULL && gpItemPointer->usItem != 0 && gpItemPointer->usItem != MONEY) {
		//ADB oops, money splits and puts a new item on the cursor, which would replace what's already on the cursor!
//		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Unable to split money due to having an item on your cursor." );
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_UNABLETOSPLITMONEY] );
		return FALSE;
	}


	//Set the current screen
	guiCurrentItemDescriptionScreen = guiCurrentScreen;

	// Set up start coordinates for the box.
	InitItemDescriptionBoxStartCoords( gGameExternalOptions.iEnhancedDescriptionBox, UseNASDesc( pObject )  );

	// Set X, Y
	if(guiCurrentScreen == GAME_SCREEN || (sX == 0 && sY == 0))
	{
		gsInvDescX = ITEMDESC_START_X;	//sX;
		gsInvDescY = ITEMDESC_START_Y;	//sY;
	}
	else
	{
		gsInvDescX = sX;
		gsInvDescY = sY;
	}

	// Headrock: initialize coordinates of icons, values and text, based on gsInvDescXY and the type of description
	// box we want to use.
	//CHRISL: Initialize coords based on EDB/NIV settings
	// This initializes the start coordinates of the description box, based on the box we want to use.
	InitDescStatCoords( pObject );
	InitEDBCoords( pObject );
	InitItemDescriptionBoxOffsets(pObject);

	gpItemDescObject = pObject;
	gubItemDescStatusIndex = ubStatusIndex;
	gpItemDescSoldier = pSoldier;
	fItemDescDelete		= FALSE;

	// Build a mouse region here that is over any others.....
	if (guiCurrentItemDescriptionScreen ==  MAP_SCREEN )
	{

		//return( FALSE );

			MSYS_DefineRegion( &gInvDesc, (UINT16)gsInvDescX, (UINT16)gsInvDescY ,(UINT16)(gsInvDescX + ITEMDESC_WIDTH), (UINT16)(gsInvDescY + ITEMDESC_HEIGHT), MSYS_PRIORITY_HIGHEST - 2,
				CURSOR_NORMAL, MSYS_NO_CALLBACK, ItemDescCallback );
		 	MSYS_AddRegion( &gInvDesc);

			giMapInvDescButtonImage=  LoadButtonImage( "INTERFACE\\itemdescdonebutton.sti" ,-1,0,-1,1,-1 );

			// HEADROCK: Create "Done" button. Coordinates changed - button moved to new location for
			// enhanced Description Box project.
			if(UsingEDBSystem() > 0)
				giMapInvDescButton= QuickCreateButton( giMapInvDescButtonImage, (UINT16)( ITEMDESC_DONE_X ), (UINT16)(ITEMDESC_DONE_Y),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										( GUI_CALLBACK )BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ItemDescDoneButtonCallback );
			else
				giMapInvDescButton= QuickCreateButton( giMapInvDescButtonImage, (UINT16)( ITEMDESC_DONE_X ), (UINT16)(ITEMDESC_DONE_Y),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										( GUI_CALLBACK )BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ItemDescDoneButtonCallback );

			fShowDescriptionFlag = TRUE;
	}
	else if(guiCurrentItemDescriptionScreen == SHOPKEEPER_SCREEN)
	{
//		MSYS_DefineRegion( &gInvDesc, (UINT16)SM_ITEMDESC_START_X, (UINT16)SM_ITEMDESC_START_Y ,(UINT16)(SM_ITEMDESC_START_X + ITEMDESC_WIDTH), (UINT16)(SM_ITEMDESC_START_Y + ITEMDESC_HEIGHT), MSYS_PRIORITY_HIGHEST,
//			MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		MSYS_DefineRegion( &gInvDesc, (UINT16)gsInvDescX, (UINT16)gsInvDescY ,(UINT16)(gsInvDescX + ITEMDESC_WIDTH), (UINT16)(gsInvDescY + ITEMDESC_HEIGHT), MSYS_PRIORITY_HIGHEST,
			MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		MSYS_AddRegion( &gInvDesc);
	}
	else
	{
		MSYS_DefineRegion( &gInvDesc, (UINT16)SM_ITEMDESC_START_X, (UINT16)SM_ITEMDESC_START_Y ,(UINT16)(SM_ITEMDESC_START_X + ITEMDESC_WIDTH), (UINT16)(SM_ITEMDESC_START_Y + ITEMDESC_HEIGHT), MSYS_PRIORITY_HIGHEST,
			MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );
		MSYS_AddRegion( &gInvDesc);
	}

	// HEADROCK HAM 5: Adjust Item Region
	// Before creating the ammo button, we set a region that spans the entire Big Item image.  Clicking on the image
	// will trigger a Transformation menu.
	{
		MSYS_DefineRegion( &gInvDescTransformRegion, (UINT16)ITEMDESC_ITEM_X, (UINT16)ITEMDESC_ITEM_Y ,(UINT16)(ITEMDESC_ITEM_X + ITEMDESC_ITEM_WIDTH), (UINT16)(ITEMDESC_ITEM_Y + ITEMDESC_ITEM_HEIGHT), MSYS_PRIORITY_HIGHEST,
			MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescTransformRegionCallback );
		MSYS_AddRegion( &gInvDescTransformRegion);
	}

	// Add ammo eject button for GUN type objects.
	if ( (Item[ pObject->usItem ].usItemClass & IC_GUN) && !Item[pObject->usItem].rocketlauncher )
	{
		if ( GetMagSize(gpItemDescObject) <= 99 )
		{
			// HEADROCK HAM 3.4: "Bullet Hide" feature - bullet count only shown during combat if character is competent enough.
			if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
			{
				EstimateBulletsLeft( pSoldier, pObject );
				swprintf(pStr, L"%s/%d", gBulletCount, GetMagSize(gpItemDescObject) );
			}
			else
			{
				swprintf( pStr, L"%d/%d", (*gpItemDescObject)[ubStatusIndex]->data.gun.ubGunShotsLeft, GetMagSize(gpItemDescObject));
			}
		}
		else
		{
			// HEADROCK HAM 3.4: "Bullet Hide" feature - bullet count only shown during combat if character is competent enough.
			if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
			{
				EstimateBulletsLeft( pSoldier, pObject );
				swprintf( pStr, L"%s", gBulletCount );
			}
			else
			{
				swprintf( pStr, L"%d", (*gpItemDescObject)[ubStatusIndex]->data.gun.ubGunShotsLeft );
			}
		}

		FilenameForBPP("INTERFACE\\infobox.sti", ubString);
		 sForeColour = ITEMDESC_AMMO_FORE;

		giItemDescAmmoButtonImages	= LoadButtonImage(ubString,AmmoTypes[(*pObject)[ubStatusIndex]->data.gun.ubGunAmmoType].grayed,AmmoTypes[(*pObject)[ubStatusIndex]->data.gun.ubGunAmmoType].offNormal,-1,AmmoTypes[(*pObject)[ubStatusIndex]->data.gun.ubGunAmmoType].onNormal,-1 );

		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			// in mapscreen, move over a bit
			giItemDescAmmoButton = CreateIconAndTextButton( giItemDescAmmoButtonImages, pStr, TINYFONT1,
															 sForeColour, FONT_MCOLOR_BLACK,
															 sForeColour, FONT_MCOLOR_BLACK,
															 TEXT_CJUSTIFIED,
															 (INT16)(ITEMDESC_AMMO_X), (INT16)(ITEMDESC_AMMO_Y), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescAmmoCallback );

		}
		else
		{

			// not in mapscreen
			giItemDescAmmoButton = CreateIconAndTextButton( giItemDescAmmoButtonImages, pStr, TINYFONT1,
															 sForeColour, FONT_MCOLOR_BLACK,
															 sForeColour, FONT_MCOLOR_BLACK,
															 TEXT_CJUSTIFIED,
															 (INT16)(ITEMDESC_AMMO_X), (INT16)(ITEMDESC_AMMO_Y), BUTTON_TOGGLE ,MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescAmmoCallback );
		}
		//if we are being init from the shop keeper screen and this is a dealer item we are getting info from
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE && pShopKeeperItemDescObject != NULL )
		{
			//disable the eject button
			SpecifyDisabledButtonStyle( giItemDescAmmoButton, DISABLED_STYLE_HATCHED );

			DisableButton( giItemDescAmmoButton );
			SetButtonFastHelpText( giItemDescAmmoButton, L"\0" );
		}
		else{
			SetButtonFastHelpText( giItemDescAmmoButton, Message[ STR_EJECT_AMMO ] );
			//CHRISL: Include the ubStatusIndex in the region information so we know which object in a stack we're looking at
			MSYS_SetBtnUserData( giItemDescAmmoButton, 1, ubStatusIndex );
		}

		FindFontCenterCoordinates( (INT16)ITEMDESC_AMMO_TEXT_X, (INT16)ITEMDESC_AMMO_TEXT_Y, ITEMDESC_AMMO_TEXT_WIDTH, GetFontHeight( TINYFONT1 ), pStr, TINYFONT1, &usX, &usY);

		SpecifyButtonTextOffsets( giItemDescAmmoButton, (UINT8) usX, (UINT8) usY, TRUE );

		gfItemAmmoDown = FALSE;

	}
	else
	{
		// Reset!!
		giItemDescAmmoButton = -1;
	}

	// HEADROCK: Tooltip Regions for stats. Only happens with Enhanced Description Box turned on.
	if(UsingEDBSystem() > 0)
		InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );

	// Item pros and cons tooltips region is created now.
	if ( ITEM_PROS_AND_CONS( gpItemDescObject->usItem ) )
	{
		for ( cnt = 0; cnt < 2; cnt++ )
		{
			// HEADROCK: Rectangle coordinates for tooltip determined by description box used.
			// Add region for pros/cons help text
			MSYS_DefineRegion( &gProsAndConsRegions[ cnt ],
				gItemDescTextRegions[3+(cnt*2)].sLeft,
				gItemDescTextRegions[3+(cnt*2)].sTop,
				gItemDescTextRegions[3+(cnt*2)].sRight,
				gItemDescTextRegions[3+(cnt*2)].sBottom,
				MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescCallback );

			MSYS_AddRegion( &gProsAndConsRegions[cnt]);

			if (cnt == 0)
			{
				wcscpy( gzFullItemPros, gzProsLabel );
				wcscat( gzFullItemPros, L" " );
				// use temp variable to prevent an initial comma from being displayed
				GenerateProsString( gzFullItemTemp, gpItemDescObject, 1000 );
				wcscat( gzFullItemPros, gzFullItemTemp );
				SetRegionFastHelpText( &(gProsAndConsRegions[ cnt ]), gzFullItemPros );
			}
			else
			{
				wcscpy( gzFullItemCons, gzConsLabel );
				wcscat( gzFullItemCons, L" " );
				// use temp variable to prevent an initial comma from being displayed
				GenerateConsString( gzFullItemTemp, gpItemDescObject, 1000 );
				wcscat( gzFullItemCons, gzFullItemTemp );
				SetRegionFastHelpText( &(gProsAndConsRegions[ cnt ]), gzFullItemCons );
			}
			SetRegionHelpEndCallback( &(gProsAndConsRegions[ cnt ]), HelpTextDoneCallback );
		}
	}

	// Load graphic
	// HEADROCK: Select STI based on Description Box used:
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\infobox_interface.sti" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiItemDescBox) );

	if(ubPosition != 255 && UsingNewInventorySystem() == true)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\infobox_background.sti" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemDescBoxBackground) );
	}
	else
		guiItemDescBoxBackground = 0;

	// HEADROCK: Select STI based on Description Box used:
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\iteminfoc.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiMapItemDescBox) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\bullet.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiBullet) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\ATTACHMENT_SLOT.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiAttachmentSlot) );

	// HEADROCK HAM 5: Transformation Indicator
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\INFOBOX_Transform_Icon.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiTransformIconGraphic) );

	// HEADROCK: Added new STIs
	CHECKF( InternalInitEnhancedDescBox() );

	if ( gpItemDescObject->usItem != MONEY  )
	{
		UpdateAttachmentTooltips(pObject, ubStatusIndex);
	}
	else
	{
		memset( &gRemoveMoney, 0, sizeof( REMOVE_MONEY ) );
		gRemoveMoney.uiTotalAmount = (*gpItemDescObject)[0]->data.money.uiMoneyAmount;
		gRemoveMoney.uiMoneyRemaining = (*gpItemDescObject)[0]->data.money.uiMoneyAmount;
		gRemoveMoney.uiMoneyRemoving = 0;

		// Load graphic
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\info_bil.sti" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiMoneyGraphicsForDescBox) );

		//Create buttons for the money
//		if (guiCurrentScreen ==  MAP_SCREEN )
		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			guiMoneyButtonImage = LoadButtonImage("INTERFACE\\Info_bil.sti", -1,1,-1,2,-1 );
			for(cnt=0; cnt<OLD_MAX_ATTACHMENTS_101-1; cnt++)
			{
				guiMoneyButtonBtn[cnt] = CreateIconAndTextButton( guiMoneyButtonImage, gzMoneyAmounts[cnt], BLOCKFONT2,
																 5, DEFAULT_SHADOW,
																 5, DEFAULT_SHADOW,
																 TEXT_CJUSTIFIED,
																 (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[cnt].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[cnt].y), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
																 DEFAULT_MOVE_CALLBACK, BtnMoneyButtonCallback );
				MSYS_SetBtnUserData( guiMoneyButtonBtn[cnt], 0, cnt);
				if( cnt == M_1000 && gRemoveMoney.uiTotalAmount < 1000 )
					DisableButton( guiMoneyButtonBtn[cnt] );
				else if( cnt == M_100 && gRemoveMoney.uiTotalAmount < 100 )
					DisableButton( guiMoneyButtonBtn[cnt] );
				else if( cnt == M_10 && gRemoveMoney.uiTotalAmount < 10 )
					DisableButton( guiMoneyButtonBtn[cnt] );
			}
			//Create the Done button
			guiMoneyDoneButtonImage = UseLoadedButtonImage( guiMoneyButtonImage, -1,3,-1,4,-1 );
			guiMoneyButtonBtn[cnt] = CreateIconAndTextButton( guiMoneyDoneButtonImage, gzMoneyAmounts[cnt], BLOCKFONT2,
															 5, DEFAULT_SHADOW,
															 5, DEFAULT_SHADOW,
															 TEXT_CJUSTIFIED,
															 (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[cnt].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[cnt].y), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, BtnMoneyButtonCallback );
			MSYS_SetBtnUserData( guiMoneyButtonBtn[cnt], 0, cnt);

		}
		else
		{
			guiMoneyButtonImage = LoadButtonImage("INTERFACE\\Info_bil.sti", -1,1,-1,2,-1 );
			for(cnt=0; cnt<OLD_MAX_ATTACHMENTS_101-1; cnt++)
			{
				guiMoneyButtonBtn[cnt] = CreateIconAndTextButton( guiMoneyButtonImage, gzMoneyAmounts[cnt], BLOCKFONT2,
																 5, DEFAULT_SHADOW,
																 5, DEFAULT_SHADOW,
																 TEXT_CJUSTIFIED,
																 (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[cnt].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[cnt].y), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
																 DEFAULT_MOVE_CALLBACK, BtnMoneyButtonCallback );
				MSYS_SetBtnUserData( guiMoneyButtonBtn[cnt], 0, cnt);
				if( cnt == M_1000 && gRemoveMoney.uiTotalAmount < 1000 )
					DisableButton( guiMoneyButtonBtn[cnt] );
				else if( cnt == M_100 && gRemoveMoney.uiTotalAmount < 100 )
					DisableButton( guiMoneyButtonBtn[cnt] );
				else if( cnt == M_10 && gRemoveMoney.uiTotalAmount < 10 )
					DisableButton( guiMoneyButtonBtn[cnt] );
			}

			//Create the Done button
			guiMoneyDoneButtonImage = UseLoadedButtonImage( guiMoneyButtonImage, -1,3,6,4,5 );
			guiMoneyButtonBtn[cnt] = CreateIconAndTextButton( guiMoneyDoneButtonImage, gzMoneyAmounts[cnt], BLOCKFONT2,
															 5, DEFAULT_SHADOW,
															 5, DEFAULT_SHADOW,
															 TEXT_CJUSTIFIED,
															 (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[cnt].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[cnt].y), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, BtnMoneyButtonCallback );
			MSYS_SetBtnUserData( guiMoneyButtonBtn[cnt], 0, cnt);
		}
	}


	// HEADROCK HAM 4: Three UDB tab buttons
	if (UsingEDBSystem() > 0)
	{
		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			giMapInvDescTabButtonImage=  LoadButtonImage( "INTERFACE\\iteminfotabbutton.sti" ,-1,0,-1,1,-1 );
		}
		else
		{
			giInvDescTabButtonImage=  LoadButtonImage( "INTERFACE\\iteminfotacticaltabbutton.sti" ,-1,0,-1,1,-1 );
		}
		for (cnt = 0; cnt < 3; cnt++)
		{
			if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
			{
				giInvDescTabButton[cnt] = CreateIconAndTextButton( giMapInvDescTabButtonImage, gzItemDescTabButtonText[cnt], BLOCKFONT2,
															 5, DEFAULT_SHADOW,
															 5, DEFAULT_SHADOW,
															 TEXT_CJUSTIFIED,
															 (UINT16)( gsInvDescX + 11 + (cnt*77) ), (UINT16)( gsInvDescY + 92 + sNASYCorrection( pObject ) ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescTabButtonCallback );
			}
			else
			{
				// NAS-specific offset to these buttons
				INT16 sNASOffsetX;
				if (UseNASDesc( pObject ))
				{
					sNASOffsetX = 62;
				}
				else
				{
					sNASOffsetX = 0;
				}
				giInvDescTabButton[cnt] = CreateIconAndTextButton( giInvDescTabButtonImage, gzItemDescTabButtonShortText[cnt], BLOCKFONT2,
															 5, DEFAULT_SHADOW,
															 5, DEFAULT_SHADOW,
															 TEXT_CJUSTIFIED,
															 (UINT16)( gsInvDescX + 200 + sNASOffsetX ), (UINT16)( gsInvDescY + 4 + (cnt*63) ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
															 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescTabButtonCallback );
			}
			//giInvDescTabButton[cnt] = QuickCreateButton( giMapInvDescTabButtonImage, (UINT16)( gsInvDescX + 11 + (cnt*77) ), (UINT16)( gsInvDescY + 92 ),
			//									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
			//									DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescTabButtonCallback );
			MSYS_SetBtnUserData( giInvDescTabButton[cnt], 0, cnt);
			SetButtonFastHelpText( giInvDescTabButton[ cnt ], gzUDBButtonTooltipText[ cnt ] );
		}
 
		// Toggle on the button that corresponds to our selected tab.
		ItemDescTabButtonOn( gubDescBoxPage );
	}

	// HEADROCK HAM 4: Up/Down Arrow Buttons for Advanced tab.
	if (UsingEDBSystem() > 0)
	{
		INT16 sButtonLocationX;
		INT16 sButtonLocationY1;
		INT16 sButtonLocationY2;
		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			// NAS and NON-NAS uses the same basic coordinates - adds Y Offset as necessary through the NAS-related
			// adjustment function.
			sButtonLocationX = 224;
			sButtonLocationY1 = 118;
			sButtonLocationY2 = 295;
		}
		else
		{
			// NAS pushes everything 62 pixels right.
			if (UseNASDesc(pObject))
			{
				sButtonLocationX = 510;
				sButtonLocationY1 = 5;
				sButtonLocationY2 = 175;
			}
			else
			{
				sButtonLocationX = 448;
				sButtonLocationY1 = 5;
				sButtonLocationY2 = 175;
			}
		}

		/////////// Up Button
		giInvDescAdvButtonUpImage=  LoadButtonImage( "INTERFACE\\iteminfoadvancedbutton.sti" ,-1,0,-1,2,-1 );
		giInvDescAdvButton[0] = QuickCreateButton( giInvDescAdvButtonUpImage, (UINT16)( gsInvDescX + sButtonLocationX ), (UINT16)( gsInvDescY + sButtonLocationY1 + sNASYCorrection( pObject )  ),
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
											DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescAdvButtonCallback );

		MSYS_SetBtnUserData( giInvDescAdvButton[0], 0, 0);

		////////// Down Button
		giInvDescAdvButtonDownImage=  LoadButtonImage( "INTERFACE\\iteminfoadvancedbutton.sti" ,-1,1,-1,3,-1 );
		giInvDescAdvButton[1] = QuickCreateButton( giInvDescAdvButtonDownImage, (UINT16)( gsInvDescX + sButtonLocationX ), (UINT16)( gsInvDescY + sButtonLocationY2 + sNASYCorrection( pObject )  ),
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
											DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemDescAdvButtonCallback );

		MSYS_SetBtnUserData( giInvDescAdvButton[1], 0, 1);

		// Hide both buttons. They'll be shown if required.
		HideButton( giInvDescAdvButton[0] );
		HideButton( giInvDescAdvButton[1] );
	}

	fInterfacePanelDirty = DIRTYLEVEL2;


	gfInItemDescBox = TRUE;

	CHECKF( ReloadItemDesc( ) );

	if ( gpItemPointer )
	{
		gpAttachSoldier = gpItemPointerSoldier;
	}
	else
	{
		gpAttachSoldier = pSoldier;
	}
	//CHRISL: Instead of using attachments on item 0, use attachments on item we right clicked on using ubStatusIndex
	// store attachments that item originally had
	gOriginalAttachments = (*pObject)[ubStatusIndex]->attachments;

	if ( (gpItemPointer != NULL) && (gfItemDescHelpTextOffset == FALSE) && (CheckFact( FACT_ATTACHED_ITEM_BEFORE, 0 ) == FALSE) )
	{
		// set up help text for attachments
		for ( cnt = 0; cnt < NUM_INV_HELPTEXT_ENTRIES; cnt++ )
		{
			gItemDescHelpText.iXPosition[ cnt ] += gsInvDescX;
			gItemDescHelpText.iYPosition[ cnt ] += gsInvDescY;
		}

//		if ( !(Item[ pObject->usItem ].fFlags & ITEM_HIDDEN_ADDON) && ( ValidAttachment( gpItemPointer->usItem, pObject->usItem ) || ValidLaunchable( gpItemPointer->usItem, pObject->usItem ) || ValidMerge( gpItemPointer->usItem, pObject->usItem ) ) )
		if ( !(Item[ pObject->usItem ].hiddenaddon ) && ( ValidAttachment( gpItemPointer->usItem, pObject ) || ValidLaunchable( gpItemPointer->usItem, pObject->usItem ) || ValidMerge( gpItemPointer->usItem, pObject->usItem ) ) )
		{
			SetUpFastHelpListRegions(
				gItemDescHelpText.iXPosition,
				gItemDescHelpText.iYPosition,
				gItemDescHelpText.iWidth,
				gItemDescHelpText.sString1,
				NUM_INV_HELPTEXT_ENTRIES );
		}
		else
		{
			SetUpFastHelpListRegions(
				gItemDescHelpText.iXPosition,
				gItemDescHelpText.iYPosition,
				gItemDescHelpText.iWidth,
				gItemDescHelpText.sString2,
				NUM_INV_HELPTEXT_ENTRIES );
		}

		StartShowingInterfaceFastHelpText();

		SetFactTrue( FACT_ATTACHED_ITEM_BEFORE );
		gfItemDescHelpTextOffset = TRUE;
	}



	return( TRUE );
}

//CHRISL: This function is designed to recreate the attachment tooltips
void UpdateAttachmentTooltips(OBJECTTYPE *pObject, UINT8 ubStatusIndex)
{
	UINT32	slotCount = 0;

	//WarmSteel - Copied most of this from bobby rays attachment listing. No need to reinvent the wheel.
	BOOLEAN		fAttachmentsFound = FALSE;
	// Contains entire string of attachment names
	CHAR16		attachStr[2500];
	// Contains current attachment string
	CHAR16		attachStr2[200];
	// Contains temporary attachment list before added to string constant from text.h
	CHAR16		attachStr3[2500];
	UINT16		usAttachment;
	std::vector<UINT16>	attachList, parseList;
	std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(pObject);
	UINT64		point = GetAvailableAttachmentPoint(pObject, 0); //Madd: Common Attachment Framework

	//start by deleting the currently defined regions if they exist
	//BOB : also, clean up the popup boxes (in case they're still around)
	//		and init them to NULL in case they contain garbage
	for (INT32 cnt = 0; cnt < MAX_ATTACHMENTS; cnt++ )
	{
		if( gItemDescAttachmentRegions[cnt].IDNumber != 0 )
			MSYS_RemoveRegion( &gItemDescAttachmentRegions[cnt]);

		if( gItemDescAttachmentPopupsInitialized && gItemDescAttachmentPopups[cnt] != NULL ){
			delete(gItemDescAttachmentPopups[cnt]);			
		}
		gItemDescAttachmentPopups[cnt] = NULL;
	}
	gItemDescAttachmentPopupsInitialized = TRUE;

	//now, create new regions
	for (slotCount = 0; ;++slotCount )
	{
		//stopping conditions, not inside the for loop because it is different depending on the attachment system.
		if (UsingNewAttachmentSystem()==true)	{
			if(slotCount >= usAttachmentSlotIndexVector.size())
				break;
		} else {
			if(slotCount >= OLD_MAX_ATTACHMENTS_101)
				break;
		}

		//Empty those Strings
		swprintf( attachStr, L"" );
		swprintf( attachStr2, L"" );
		swprintf( attachStr3, L"" );

		fAttachmentsFound = FALSE;

		//Madd: if the slot is hidden, don't bother doing any of this
		if ( gItemDescAttachmentsXY[slotCount].sHeight > 0 && gItemDescAttachmentsXY[slotCount].sWidth > 0 )
		{
			// Build a mouse region here that is over any others.....
			//if (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
			if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
				 MSYS_DefineRegion( &gItemDescAttachmentRegions[slotCount], (INT16)(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX), (INT16)(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY), (INT16)(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX + gItemDescAttachmentsXY[slotCount].sWidth), (INT16)(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY + gItemDescAttachmentsXY[slotCount].sHeight), MSYS_PRIORITY_HIGHEST,
								 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescAttachmentsCallback );
			else
				 MSYS_DefineRegion( &gItemDescAttachmentRegions[slotCount], (INT16)(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX), (INT16)(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY), (INT16)(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX + gItemDescAttachmentsXY[slotCount].sBarDx + gItemDescAttachmentsXY[slotCount].sWidth), (INT16)(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY + gItemDescAttachmentsXY[slotCount].sHeight), MSYS_PRIORITY_HIGHEST,
								 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemDescAttachmentsCallback );
			// Add region
			MSYS_AddRegion( &gItemDescAttachmentRegions[slotCount]);
			MSYS_SetRegionUserData( &gItemDescAttachmentRegions[slotCount], 0, slotCount );

			//CHRISL: Include the ubStatusIndex in the region information so we know which object in a stack we're looking at
			MSYS_SetRegionUserData( &gItemDescAttachmentRegions[slotCount], 1, ubStatusIndex );

			// CHRISL: Instead of looking at object 0, let's look at the object we actually right clicked on using ubStatusIndex
			OBJECTTYPE* pAttachment = (*pObject)[ubStatusIndex]->GetAttachmentAtIndex(slotCount);
			if (pAttachment->exists()) {
				SetRegionFastHelpText( &(gItemDescAttachmentRegions[ slotCount ]), ItemNames[ pAttachment->usItem ] );
			} else if (UsingNewAttachmentSystem()==true && !usAttachmentSlotIndexVector.empty()) {	

				UINT16 usLoopSlotID = usAttachmentSlotIndexVector[slotCount];
				attachList.clear();
			
				//Print all attachments that fit on this item.
				for(UINT16 usLoop = 0; usLoop < MAXATTACHMENTS; usLoop++)
				{	//We no longer find valid attachments from AttachmentSlots.xml so we need to work a bit harder to get our list
					usAttachment = 0;
				
					//Madd: Common Attachment Framework
					if (Item[usLoop].nasAttachmentClass & AttachmentSlots[usLoopSlotID].nasAttachmentClass && IsAttachmentPointAvailable(point, usLoop, TRUE)) 
					{
						usAttachment = usLoop;
						if( !Item[usAttachment].hiddenaddon && !Item[usAttachment].hiddenattachment && ItemIsLegal(usAttachment))
						{
							bool exists = false;
							for (UINT32 i = 0; i < attachList.size(); i++)
							{
								if ( attachList[i] == usAttachment )
								{
									exists = true;
									break;
								}
							}
							if (!exists)
								attachList.push_back(usAttachment);
						}
					}

					usAttachment = 0;
					if(Attachment[usLoop][1] == pObject->usItem && AttachmentSlots[usLoopSlotID].nasAttachmentClass & Item[Attachment[usLoop][0]].nasAttachmentClass)
					{	//search primary item attachments.xml
						usAttachment = Attachment[usLoop][0];
					}
					else if(Launchable[usLoop][1] == pObject->usItem && AttachmentSlots[usLoopSlotID].nasAttachmentClass & Item[Launchable[usLoop][0]].nasAttachmentClass)
					{	//search primary item launchables.xml
						usAttachment = Launchable[usLoop][0];
					}
					else
					{	//search for attachments/launchables made valid by other attachments
						for(UINT8 x=0; x<(*pObject)[ubStatusIndex]->attachments.size(); x++)
						{
							OBJECTTYPE* pAttachment2 = (*pObject)[ubStatusIndex]->GetAttachmentAtIndex(x);
							if(pAttachment2->exists())
							{
								if(Attachment[usLoop][1] == pAttachment2->usItem && AttachmentSlots[usLoopSlotID].nasAttachmentClass & Item[Attachment[usLoop][0]].nasAttachmentClass)
									usAttachment = Attachment[usLoop][0];
								else if(Launchable[usLoop][1] == pAttachment2->usItem && AttachmentSlots[usLoopSlotID].nasAttachmentClass & Item[Launchable[usLoop][0]].nasAttachmentClass)
									usAttachment = Launchable[usLoop][0];
							}
						}
					}
					if(Attachment[usLoop][0] == 0 && Launchable[usLoop][0] == 0 && Item[usLoop].usItemClass == 0)
						break;

					if( usAttachment > 0  && !Item[usAttachment].hiddenaddon && !Item[usAttachment].hiddenattachment && ItemIsLegal(usAttachment))
					{
						bool exists = false;
						for (UINT32 i = 0; i < attachList.size(); i++)
						{
							if ( attachList[i] == usAttachment )
							{
								exists = true;
								break;
							}
						}
	
						if (!exists)
							attachList.push_back(usAttachment);
					}
				}

				if(attachList.size()>0){
					parseList = attachList;
					for(std::vector<UINT16>::iterator pIter=parseList.begin(); pIter != parseList.end(); ++pIter){
						BOOLEAN fDuplicate = FALSE;
						for(std::vector<UINT16>::iterator aIter=attachList.begin(); aIter != attachList.end();){
							UINT16 pi = *pIter;
							UINT16 ai = *aIter;
							if(pi == ai && !fDuplicate){
								fDuplicate = TRUE;
								++aIter;
								continue;
							} else if(pi == ai && fDuplicate){
								aIter = attachList.erase(aIter);
								continue;
							} else {
								++aIter;
							}
						}
					}
				}
				BOOLEAN showAttachmentPopups = FALSE;

				if(		guiCurrentItemDescriptionScreen == MAP_SCREEN 
				&&		fShowMapInventoryPool 
				&&		(	(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX == sSelMapX )
						&&	( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY == sSelMapY )
						&&	( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ == iCurrentMapSectorZ ) 
						)
				&&		CanPlayerUseSectorInventory( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] )
				)
				{
					showAttachmentPopups = TRUE;
				}

				// create quick attachment popup boxes here
				if (showAttachmentPopups)
				{
					gItemDescAttachmentPopups[slotCount] = new POPUP("Attachment list");	// init attachment popup for this slot
				
					UINT8 thisPopupsPositionType;
					if ( gsInvDescX + gItemDescAttachmentsXY[slotCount].sX < 170 && gsInvDescY + gItemDescAttachmentsXY[slotCount].sY < 180 ){
						thisPopupsPositionType = POPUP_POSITION_TOP_LEFT;
					} else if ( gsInvDescX + gItemDescAttachmentsXY[slotCount].sX > 170 && gsInvDescY + gItemDescAttachmentsXY[slotCount].sY < 180 ){
						thisPopupsPositionType = POPUP_POSITION_TOP_RIGHT;
					} else if ( gsInvDescX + gItemDescAttachmentsXY[slotCount].sX > 170 && gsInvDescY + gItemDescAttachmentsXY[slotCount].sY > 180 ){
						thisPopupsPositionType = POPUP_POSITION_BOTTOM_RIGHT;
					} else if ( gsInvDescX + gItemDescAttachmentsXY[slotCount].sX < 170 && gsInvDescY + gItemDescAttachmentsXY[slotCount].sY > 180 ){
						thisPopupsPositionType = POPUP_POSITION_BOTTOM_LEFT;
					} else {
						thisPopupsPositionType = POPUP_POSITION_TOP_LEFT;
					}

					gItemDescAttachmentPopups[slotCount]->setPosition(	(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX) + 12 ,
					/* Put it near the current slot	 */					(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY) + 32 ,
																		 thisPopupsPositionType );

					// the show callback should redraw the description box (in case we had any helptext and close other attachment popups)
					gItemDescAttachmentPopups[slotCount]->setCallback( POPUP_CALLBACK_SHOW, new popupCallbackFunction<void,UINT32>( &hideOtherAttachmentPopups, usAttachmentSlotIndexVector.size() ) );
					// the hide callback tells the program we're no longer displaying an attachment callback
					gItemDescAttachmentPopups[slotCount]->setCallback( POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,void>( &hideAttachmentPopup ) );
				}
				for(UINT16 loop = 0; loop < attachList.size(); loop++){
					usAttachment = attachList[loop];
					// If the attachment is not hidden
					if (usAttachment > 0 && !Item[ usAttachment ].hiddenaddon && !Item[ usAttachment ].hiddenattachment)
					{
						if (wcslen( attachStr3 ) + wcslen(Item[usAttachment].szItemName) > 3600)
						{
							// End list early to avoid overflow
							wcscat( attachStr3, L"\n..." );
							break;
						}
						else
						{// Add the attachment's name to the list.
							fAttachmentsFound = TRUE;
							swprintf( attachStr2, L"\n%s", Item[ usAttachment ].szItemName );
							wcscat( attachStr3, attachStr2);
						}

						if (showAttachmentPopups)
						{	// add the current attachment to the popup assigned to this attachment slot
							POPUP_OPTION * o = new POPUP_OPTION(	&std::wstring( Item[ usAttachment ].szItemName ), 
																	new popupCallbackFunction<void,UINT16>(&popupCallbackItem,usAttachment));
							// set an availiability callback to gray out any compatible attachments not found in this sector
							o->setAvail( new popupCallbackFunction<bool,UINT16>(&popupCallbackItemInSector,usAttachment) );
						
							if (loop == 11 && attachList.size() > 11){ // if there's too much stuff to list, we create a subpopup for the rest
								gItemDescAttachmentPopups[slotCount]->addSubMenuOption( &std::wstring(L"More...") );
								POPUP_SUB_POPUP_OPTION * tmp = gItemDescAttachmentPopups[slotCount]->getSubPopupOption(0);

								// positioning sub popups is handled through the option that holds them
								tmp->setPopupPosition(	(gsInvDescX + gItemDescAttachmentsXY[slotCount].sX),
													(gsInvDescY + gItemDescAttachmentsXY[slotCount].sY) + 32 ,
													POPUP_POSITION_TOP_RIGHT);	// put it to the right of the main box

								// hide the other box too
								// note that we're working with the sub popup options's sub-popup here, not the option itself
								tmp->subPopup->setCallback( POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,UINT32>( &hideOtherAttachmentPopups, usAttachmentSlotIndexVector.size() ) );

								// finally add the option
								tmp->subPopup->addOption(*o);

							} else if (loop > 11){	// add surplus options to the subpopup
								gItemDescAttachmentPopups[slotCount]->getSubPopupOption(0)->subPopup->addOption(*o);
							} else {	// options 0-11 go into the main popup
								gItemDescAttachmentPopups[slotCount]->addOption(*o);
							}
						}
					}
				}
			
				if (fAttachmentsFound)
				{
					// Add extra empty line and attachment list title
					swprintf( attachStr, L"%s:\n ", Message[ STR_ATTACHMENTS ] );
					// Print the attachments
					wcscat( attachStr, attachStr3 );
				} else if(usLoopSlotID != 0) {
					swprintf( attachStr2, L"\n%s", AttachmentSlots[usLoopSlotID].szSlotName );
					wcscat( attachStr, attachStr2);
				} else {
					wcscat( attachStr, Message[ STR_ATTACHMENTS ] );
				}
				SetRegionFastHelpText( &(gItemDescAttachmentRegions[ slotCount ]), attachStr );
			} else {
				SetRegionFastHelpText( &(gItemDescAttachmentRegions[ slotCount ]), Message[ STR_ATTACHMENTS ] );
			}
			SetRegionHelpEndCallback( &(gItemDescAttachmentRegions[ slotCount ]), HelpTextDoneCallback );
		}
	}
}

BOOLEAN ReloadItemDesc( )
{
	if ( !LoadTileGraphicForItem( &(Item[ gpItemDescObject->usItem ]), &guiItemGraphic ) )
	{
		return( FALSE );
	}

	//
	// Load name, desc
	//

	//if the player is extracting money from the players account, use a different item name and description
	if( gfAddingMoneyToMercFromPlayersAccount && gpItemDescObject->usItem == MONEY )
	{
		if ( !LoadItemInfo( MONEY_FOR_PLAYERS_ACCOUNT, gzItemName, gzItemDesc ) )
		{
			return( FALSE );
		}
	}
	else
	{
		if ( !LoadItemInfo( gpItemDescObject->usItem, gzItemName, gzItemDesc ) )
		{
			return( FALSE );
		}
	}

/*
	if (Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON)
	{
		// load item pros and cons
		if ( !LoadItemProsAndCons( gpItemDescObject->usItem, gzItemPros, gzItemCons ) )
		{
			return( FALSE );
		}
	}
	else
	{
		wcscpy( gzItemPros, L"" );
		wcscpy( gzItemCons, L"" );
	}
	*/

	return( TRUE );
}

void RenderBulletIcon(OBJECTTYPE *pObject, UINT32 ubStatusIndex)
{
	CHAR16		pStr[10];
	CHAR8		ubString[48];

	if(pObject->exists() == false)
		return;

	if(!gfInItemDescBox)
		return;

	if ( GetMagSize(pObject) <= 99 )
		swprintf( pStr, L"%d/%d", (*pObject)[ubStatusIndex]->data.gun.ubGunShotsLeft, GetMagSize(pObject));
	else
		swprintf( pStr, L"%d", (*pObject)[ubStatusIndex]->data.gun.ubGunShotsLeft );

	FilenameForBPP("INTERFACE\\infobox.sti", ubString);

	SpecifyButtonText( giItemDescAmmoButton, pStr );
}

void ItemDescAmmoCallback(GUI_BUTTON *btn,INT32 reason)
{
	static BOOLEAN fRightDown = FALSE;
	UINT32		ubStatusIndex = MSYS_GetBtnUserData( btn, 1 );

/*	region gets disabled in SKI for shopkeeper boxes.  It now works normally for merc's inventory boxes!
	//if we are currently in the shopkeeper interface, return;
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		return;
	}
*/

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fRightDown = TRUE;
		gfItemAmmoDown = TRUE;
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;
		gfItemAmmoDown = FALSE;

		//CHRISL: We dont' want to be able to reload guns using the ammo crate from this function
		if(gpItemPointer != NULL && Magazine[Item[gpItemPointer->usItem].ubClassIndex].ubMagType >= AMMO_BOX)
		{
			fInterfacePanelDirty = DIRTYLEVEL2;
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			return;
		}

		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			//if ( gpItemPointer == NULL && EmptyWeaponMagazine( gpItemDescObject, &gItemPointer, ubStatusIndex ) )
			EmptyWeaponMagazine( gpItemDescObject, &gTempObject, ubStatusIndex );
			if(gpItemPointer == NULL)
			{
				//not holding anything
				if(gTempObject.exists() == true)
				{
					gTempObject.MoveThisObjectTo(gItemPointer);
					gpItemPointer = &gItemPointer;
				}
			}
			else
			{
				//holding an item
				if(Magazine[Item[gpItemPointer->usItem].ubClassIndex].ubCalibre == Weapon[Item[gpItemDescObject->usItem].ubClassIndex].ubCalibre)
				{
					ReloadGun(gpItemDescSoldier, gpItemDescObject, gpItemPointer, ubStatusIndex);
				}
				if(gpItemPointer->ubNumberOfObjects == 0)
				{
					// nothing left in cursor
					gTempObject.MoveThisObjectTo(gItemPointer);
					gpItemPointer = &gItemPointer;
					if(gpItemPointer->ubNumberOfObjects == 0)
					{
						// delete object if nothing left
						MAPEndItemPointer( );
					}
				}
				else
				{
					// still holding someting so drop the clip we just pulled from the weapon
					// start by searching merc for a place to put the clip
					if(AutoPlaceObject(gpItemDescSoldier, &gTempObject, TRUE) == FALSE)
					{
						// couldn't find a place on the merc, so drop into the sector
						AutoPlaceObjectToWorld(gpItemDescSoldier, &gTempObject);
						/*if(fShowMapInventoryPool)	//sector inventory panel is open
						{
							AutoPlaceObjectInInventoryStash(&gTempObject, gpItemDescSoldier->sGridNo);
							fMapPanelDirty = TRUE;
						}
						else	//sector inventory panel is closed
						{
							AddItemToPool(gpItemDescSoldier->sGridNo, &gTempObject, 1, gpItemDescSoldier->pathing.bLevel, WORLD_ITEM_REACHABLE, 0);
						}*/
					}
				}
			}
			// OK, END the description box
			//fItemDescDelete = TRUE;
			fInterfacePanelDirty = DIRTYLEVEL2;
			gpItemPointerSoldier = gpItemDescSoldier;

			RenderBulletIcon(gpItemDescObject, ubStatusIndex);

			// Set mouse
			if(gpItemPointer->exists() == true)
			{
				guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
				gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

				MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
				MSYS_SetCurrentCursor( EXTERN_CURSOR );

				fMapInventoryItem=TRUE;
				fTeamPanelDirty=TRUE;
			}
		}
		else
		{
			// Set pointer to item
			EmptyWeaponMagazine( gpItemDescObject, &gTempObject, ubStatusIndex );
			if(gpItemPointer == NULL)
			{
				//not holding anything
				if(gTempObject.exists() == true)
				{
					gTempObject.MoveThisObjectTo(gItemPointer);
					gpItemPointer = &gItemPointer;
				}
			}
			else
			{
				//holding an item
				if(Magazine[Item[gpItemPointer->usItem].ubClassIndex].ubCalibre == Weapon[Item[gpItemDescObject->usItem].ubClassIndex].ubCalibre)
				{
					ReloadGun(gpItemDescSoldier, gpItemDescObject, gpItemPointer);
				}
				if(gpItemPointer->ubNumberOfObjects == 0)
				{
					// nothing left in cursor
					gTempObject.MoveThisObjectTo(gItemPointer);
					gpItemPointer = &gItemPointer;
					if(gpItemPointer->ubNumberOfObjects == 0)
					{
						// delete object if nothing left
						EndItemPointer( );
					}
				}
				else
				{
					// still holding someting so drop the clip we just pulled from the weapon
					// start by searching merc for a place to put the clip
					if(AutoPlaceObject(gpItemDescSoldier, &gTempObject, TRUE) == FALSE)
					{
						AddItemToPool(gpItemDescSoldier->sGridNo, &gTempObject, 1, gpItemDescSoldier->pathing.bLevel, WORLD_ITEM_REACHABLE, 0);
					}
				}
			}
			gpItemPointerSoldier = gpItemDescSoldier;

			// if in SKI, load item into SKI's item pointer
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				// pick up bullets from weapon into cursor (don't try to sell)
				BeginSkiItemPointer( PLAYERS_INVENTORY, -1, FALSE );
			}

			// OK, END the description box
			//fItemDescDelete = TRUE;
			fInterfacePanelDirty = DIRTYLEVEL2;

			RenderBulletIcon(gpItemDescObject, ubStatusIndex);

			fItemDescDelete = TRUE;

		}
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

	}

}


//CHRISL: We need to know which item in the stack we're working with.
void DoAttachment( UINT8 subObject, INT32 iItemPos )
{
	if ( gpItemDescObject->AttachObject( gpItemDescSoldier, gpItemPointer, TRUE, subObject, iItemPos) )
	{
		if (gpItemPointer->exists() == false)
		{
			// attachment attached, merge item consumed, etc

			if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
			{
				MAPEndItemPointer( );
			}
			else
			{
				// End Item pickup
				gpItemPointer = NULL;
				EnableSMPanelButtons( TRUE , TRUE );

				MSYS_ChangeRegionCursor( &gSMPanelRegion , CURSOR_NORMAL );
				SetCurrentCursorFromDatabase( CURSOR_NORMAL );

				//if we are currently in the shopkeeper interface
				if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
				{
					//Clear out the moving cursor
					gMoveingItem.initialize();

					//change the curosr back to the normal one
					SetSkiCursor( CURSOR_NORMAL );
				}
			}
		}

		if ( gpItemDescObject->exists() == false )
		{
			// close desc panel panel
			DeleteItemDescriptionBox();
		}
		//Dirty interface
		fInterfacePanelDirty = DIRTYLEVEL2;

		ReloadItemDesc( );
	}

	// re-evaluate repairs
	gfReEvaluateEveryonesNothingToDo = TRUE;
}

void PermanantAttachmentMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		DoAttachment(gbMessageBoxSubObject, iItemPosition);
	}
	gbMessageBoxSubObject = 0;
	// else do nothing
}

void ItemDescAttachmentsCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32					uiItemPos, ubStatusIndex;
	static BOOLEAN	fRightDown = FALSE;

	if ( gfItemDescObjectIsAttachment || !gpItemDescObject->exists() )
	{
		// screen out completely
		return;
	}

	uiItemPos = MSYS_GetRegionUserData( pRegion, 0 );
	ubStatusIndex = MSYS_GetRegionUserData( pRegion, 1 );
	OBJECTTYPE* pAttachment = (*gpItemDescObject)[ubStatusIndex]->GetAttachmentAtIndex(uiItemPos);

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// if the item being described belongs to a shopkeeper, ignore attempts to pick it up / replace it
		if( ( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) && ( pShopKeeperItemDescObject != NULL ) )
		{
			return;
		}

		// Try to place attachment if something is in our hand
		// require as many APs as to reload
		if ( gpItemPointer != NULL )
		{
			// nb pointer could be NULL because of inventory manipulation in mapscreen from sector inv
			if ( !gpItemPointerSoldier || EnoughPoints( gpItemPointerSoldier, AttachmentAPCost( gpItemPointer->usItem, gpItemDescObject, gpItemPointerSoldier ), 0, TRUE ) )
			{
//				if ( (Item[ gpItemPointer->usItem ].fFlags & ITEM_INSEPARABLE) && ValidAttachment( gpItemPointer->usItem, gpItemDescObject->usItem ) )
				if ( (Item[ gpItemPointer->usItem ].inseparable == 1) && ValidAttachment( gpItemPointer->usItem, gpItemDescObject ) )
				{
					iItemPosition = uiItemPos;
					gbMessageBoxSubObject = (UINT8)ubStatusIndex;
					DoScreenIndependantMessageBox( Message[ STR_PERMANENT_ATTACHMENT ], ( UINT8 )MSG_BOX_FLAG_YESNO, PermanantAttachmentMessageBoxCallBack );
					return;
				}

				DoAttachment((UINT8)ubStatusIndex, uiItemPos);

				if(gpItemPointer->exists()){
					if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
					{
						// Set mouse
						guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
						gusExternVoSubIndex = Item[ gpItemPointer->usItem ].ubGraphicNum;

						MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
						MSYS_SetCurrentCursor( EXTERN_CURSOR );
						fMapInventoryItem=TRUE;
						fTeamPanelDirty=TRUE;
					}

					//Dirty interface
					fInterfacePanelDirty = DIRTYLEVEL2;

					// re-evaluate repairs
					gfReEvaluateEveryonesNothingToDo = TRUE;

					UpdateItemHatches();


				}
				if(gpItemDescObject != NULL)
				{
					InitDescStatCoords(gpItemDescObject);
					UpdateAttachmentTooltips(gpItemDescObject, ubStatusIndex);
					InternalInitEDBTooltipRegion(gpItemDescObject, guiCurrentItemDescriptionScreen);
				}
			}
		}
		else
		{
			// ATE: Make sure we have enough AP's to drop it if we pick it up!
			if ( pAttachment->exists() && EnoughPoints( gpItemDescSoldier, ( AttachmentAPCost( pAttachment->usItem, gpItemDescObject, gpItemPointerSoldier ) + APBPConstants[AP_PICKUP_ITEM] ), 0, TRUE ) )
			{
				// Get attachment if there is one
				// The follwing function will handle if no attachment is here
				if ( gpItemDescObject->RemoveAttachment( pAttachment, &gItemPointer, ubStatusIndex, gpItemDescSoldier ) )
				{
					gpItemPointer = &gItemPointer;
					gpItemPointerSoldier = gpItemDescSoldier;

					if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
					{
						// Set mouse
						guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
						gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

						MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
						MSYS_SetCurrentCursor( EXTERN_CURSOR );
						fMapInventoryItem=TRUE;
						fTeamPanelDirty=TRUE;
					}

					//if we are currently in the shopkeeper interface
					else if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						// pick up attachment from item into cursor (don't try to sell)
						BeginSkiItemPointer( PLAYERS_INVENTORY, -1, FALSE );
					}

					//Dirty interface
					fInterfacePanelDirty = DIRTYLEVEL2;

					// re-evaluate repairs
					gfReEvaluateEveryonesNothingToDo = TRUE;

					UpdateItemHatches();

				}

																		// BOB : show the quick attachment popup
			} else if( 	guiCurrentItemDescriptionScreen == MAP_SCREEN	// if we're in the map screen
					&&	fShowMapInventoryPool							// and are viweing the sector inventory
					&&	!pAttachment->exists()							// and the clicked attachment slot is empty
					&&	gItemDescAttachmentPopupsInitialized && gItemDescAttachmentPopups[uiItemPos] != 0 ) {	// and the popup exists				

				// the popup slot that is going to be displayed
				giActiveAttachmentPopup = uiItemPos;
				// which slot was this ? No better (simple) way to let the callbacks know.
				gubPopupStatusIndex = (UINT8)ubStatusIndex;
				guiPopupItemPos = uiItemPos;

				return; // and leave before UpdateAttachmentTooltips overwrites the popup
			}

			if(gpItemDescObject != NULL)
			{
				InitDescStatCoords(gpItemDescObject);
				UpdateAttachmentTooltips(gpItemDescObject, ubStatusIndex);
				InternalInitEDBTooltipRegion(gpItemDescObject, guiCurrentItemDescriptionScreen);
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		if ( pAttachment->exists() )
		{
			static OBJECTTYPE Object2;

			BOOLEAN fShopkeeperItem = FALSE;

			// remember if this is a shopkeeper's item we're viewing ( pShopKeeperItemDescObject will get nuked on deletion )
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE && pShopKeeperItemDescObject != NULL )
			{
				fShopkeeperItem = TRUE;
			}

			OBJECTTYPE *pTemp = gpItemDescObject;
			DeleteItemDescriptionBox( );
			gpItemDescPrevObject = pTemp;

			gpItemDescOrigAttachmentObject = pAttachment;

			Object2 = *pAttachment;
			gfItemDescObjectIsAttachment = TRUE;
			InternalInitItemDescriptionBox( &Object2, gsInvDescX, gsInvDescY, 0, gpItemDescSoldier );

			if ( fShopkeeperItem )
			{
				pShopKeeperItemDescObject = &Object2;
				StartSKIDescriptionBox( );
			}
		}
	}
}

INT8 DetermineShowBox( )
{
	INT8 bShowBox = 0;

	// HEADROCK HAM 4: Function altered for Ultra Description Box

	if(gpItemDescObject == NULL)
		return 0;

	if (iResolution >= _640x480 && iResolution < _800x600)
		return 0;

	if(UsingEDBSystem() <= 0)
		if (guiCurrentItemDescriptionScreen == MAP_SCREEN)
			if (UseNASDesc( gpItemDescObject) )
				return 2;
			else
				return 1;
		else
			if (UseNASDesc( gpItemDescObject) )
				return 1;
			else
				return 0;

	if(UsingNewInventorySystem() == false)	//OIV
	{
		if (guiCurrentItemDescriptionScreen == MAP_SCREEN)
		{
			if (UsingEDBSystem() > 0)
			{
				if (Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH))
				{
					switch (gubDescBoxPage)
					{
						case 0:
							bShowBox = 3;
							break;
						case 1:
							bShowBox = 4;
							break;
						case 2:
							bShowBox = 6;
							break;
					}
				}
				else
				{
					switch (gubDescBoxPage)
					{
						case 0:
							bShowBox = 3;
							break;
						case 1:
							bShowBox = 5;
							break;
						case 2:
							bShowBox = 6;
							break;
					}
				}
			}
			else
			{
				if (UseNASDesc( gpItemDescObject) )
					bShowBox = 2;
				else
					bShowBox = 1;
			}
		}
		else
			return 0; // Tactical OIV no longer enables UDB. It always defaults to ODB.
	}
	else if(UsingNewInventorySystem() == true)	//NIV
	{
		if (Item[gpItemDescObject->usItem].usItemClass & (IC_WEAPON|IC_PUNCH) && UsingEDBSystem() > 0 )
		{
			if (guiCurrentItemDescriptionScreen == MAP_SCREEN)
			{
				switch (gubDescBoxPage)
				{
					case 0:
						bShowBox = 3;
						break;
					case 1:
						bShowBox = 4;
						break;
					case 2:
						bShowBox = 6;
						break;
				}
			}
			else
			{
				switch (gubDescBoxPage)
				{
					case 0:
						bShowBox = 2;
						break;
					case 1:
						bShowBox = 3;
						break;
					case 2:
						bShowBox = 5;
						break;
				}
			}
		}
		else if (UsingEDBSystem() > 0)
		{
			if (guiCurrentItemDescriptionScreen == MAP_SCREEN)
			{
				switch (gubDescBoxPage)
				{
					case 0:
						bShowBox = 3;
						break;
					case 1:
						bShowBox = 5;
						break;
					case 2:
						bShowBox = 6;
						break;
				}
			}
			else
			{
				switch (gubDescBoxPage)
				{
					case 0:
						bShowBox = 2;
						break;
					case 1:
						bShowBox = 4;
						break;
					case 2:
						bShowBox = 5;
						break;
				}
			}
		}
		else
			if (UseNASDesc( gpItemDescObject) )
				bShowBox = 2;
			else
				bShowBox = 1;
	}

	if (UsingEDBSystem() > 0 && UseNASDesc( gpItemDescObject ))
	{
		bShowBox += 4;
	}

	return bShowBox;
}

// HEADROCK HAM 4: This picks an LBE picture to be displayed on top of the description box when necessary.
INT8 DetermineShowLBE( )
{
	if(gpItemDescObject == NULL)
		return -1;

	if (iResolution >= _640x480 && iResolution < _800x600)
		return -1;

	if(!UsingNewInventorySystem())
	{
		return -1;
	}
	else
	{
		if(!(gpItemDescObject->IsActiveLBE(gubItemDescStatusIndex) || Item[gpItemDescObject->usItem].usItemClass == IC_LBEGEAR))
		{
			return -1;
		}
		else
		{
			if(gpItemDescObject->IsActiveLBE(gubItemDescStatusIndex))
				return (gpItemDescObject->GetLBEPointer(gubItemDescStatusIndex)->lbeClass - 1);
			else if(Item[gpItemDescObject->usItem].usItemClass == IC_LBEGEAR)
				return (LoadBearingEquipment[Item[gpItemDescObject->usItem].ubClassIndex].lbeClass - 1);
			else
				return -1;
		}
	}
}

// HEADROCK HAM 4: Function to get the number of the item condition string
UINT8 GetConditionString( UINT8 ubStatus, UINT8 *ubFontColor )
{
	if (ubStatus > 99)
	{
		*ubFontColor = FONT_WHITE;
		return 0;
	}
	else if (ubStatus >= 95 && ubStatus <= 99)
	{
		*ubFontColor = FONT_LTGREEN;
		return 1;
	}
	else if (ubStatus >= 80 && ubStatus <= 94)
	{
		*ubFontColor = FONT_GREEN;
		return 2;
	}
	else if (ubStatus >= 60 && ubStatus <= 79)
	{
		*ubFontColor = FONT_YELLOW;
		return 3;
	}
	else if (ubStatus >= 40 && ubStatus <= 59)
	{
		*ubFontColor = FONT_ORANGE;
		return 4;
	}
	else if (ubStatus >= 10 && ubStatus <= 39)
	{
		*ubFontColor = FONT_RED;
		return 5;
	}
	else if (ubStatus < 10)
	{
		*ubFontColor = FONT_DKRED;
		return 6;
	}
	// Default
	else
	{
		return 0;
	}
}

void RenderItemDescriptionBox( )
{
	ETRLEObject			*pTrav;
	UINT32				usHeight, usWidth;
	INT32				sOffsetX, sOffsetY;
	INT16				sCenX, sCenY, sStrX;
	HVOBJECT			hVObject;
	CHAR16				sTempString[ 128 ];

	UINT16								uiStringLength;
	static CHAR16		pStr[ 100 ];
	INT32				cnt;
	FLOAT				fWeight;
	INT16				usX, usY;
	INT16				ubAttackAPs;
	INT16				sProsConsIndent;
	INT8				showBox = DetermineShowBox();
	INT8				showLBE = DetermineShowLBE();
	std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(gpItemDescObject, gubItemDescStatusIndex);

	int status = 0;
	int shotsLeft = 0;
	if (gpItemDescObject && gubItemDescStatusIndex < gpItemDescObject->ubNumberOfObjects) {
		status = (*gpItemDescObject)[ gubItemDescStatusIndex ]->data.objectStatus;
		shotsLeft = (*gpItemDescObject)[ gubItemDescStatusIndex ]->data.ubShotsLeft;
	}

	if( gfInItemDescBox )
	{
    	// TAKE A LOOK AT THE VIDEO OBJECT SIZE ( ONE OF TWO SIZES ) AND CENTER!
		GetVideoObject( &hVObject, guiItemGraphic );
		if(hVObject->ubBitDepth == 8)
		{
			pTrav = &(hVObject->pETRLEObject[ 0 ] );
			usHeight				= (UINT32)pTrav->usHeight;
			usWidth					= (UINT32)pTrav->usWidth;
			sOffsetX				= (INT32)pTrav->sOffsetX;
			sOffsetY				= (INT32)pTrav->sOffsetY;
		}
		else if(hVObject->ubBitDepth == 16 && hVObject->p16BPPObject)
		{
			usHeight				= hVObject->p16BPPObject->usHeight;
			usWidth					= hVObject->p16BPPObject->usWidth;
			sOffsetX				= hVObject->p16BPPObject->sOffsetX;
			sOffsetY				= hVObject->p16BPPObject->sOffsetY;
		}
		else if(hVObject->ubBitDepth == 32)
		{
			usHeight				= hVObject->p16BPPObject->usHeight;
			usWidth					= hVObject->p16BPPObject->usWidth;
			sOffsetX				= hVObject->p16BPPObject->sOffsetX;
			sOffsetY				= hVObject->p16BPPObject->sOffsetY;
		}

		// CENTER IN SLOT!
		// REMOVE OFFSETS!
		sCenX = ITEMDESC_ITEM_X + (INT16)( abs( ITEMDESC_ITEM_WIDTH - (double)usWidth ) / 2 ) - sOffsetX;
		sCenY = ITEMDESC_ITEM_Y + (INT16)( abs( ITEMDESC_ITEM_HEIGHT - (double)usHeight ) / 2 )- sOffsetY;

		RenderBackpackButtons(DEACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
		if(guiCurrentItemDescriptionScreen == SHOPKEEPER_SCREEN && gGameSettings.fOptions[TOPTION_ENHANCED_DESC_BOX])
			EnableDisableShopkeeperButtons(guiCurrentItemDescriptionScreen, DEACTIVATE_BUTTON);

		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiMapItemDescBox, showBox, gsInvDescX, gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		else
		{
			if(guiItemDescBoxBackground != 0)
			{
				if (iResolution >= _640x480 && iResolution < _800x600)
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemDescBoxBackground, 0, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				else if (iResolution < _1024x768)
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemDescBoxBackground, 1, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				else	
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemDescBoxBackground, 2, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );

				// WANNE: Yes here we want to overlay image!
				//ShadowNIVPanel();
			}
			
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemDescBox, showBox, gsInvDescX, gsInvDescY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//WarmSteel - Draw the attachment slots needed for NAS, read their positions from XML.
		if(UsingNewAttachmentSystem()==true && UseNASDesc(gpItemDescObject) && !usAttachmentSlotIndexVector.empty()){

			for(UINT16 slotCount = 0; slotCount < usAttachmentSlotIndexVector.size(); slotCount++){
				//Madd: hide slots if their x or y position is >= 300
				if ( AttachmentSlots[usAttachmentSlotIndexVector[slotCount]].usDescPanelPosX <= 300 && AttachmentSlots[usAttachmentSlotIndexVector[slotCount]].usDescPanelPosY <= 300 )
				{
					INT16 sX = gsInvDescX + AttachmentSlots[usAttachmentSlotIndexVector[slotCount]].usDescPanelPosX - 6; //Warmsteel - Retracting a number to account for the status bar.
					INT16 sY = gsInvDescY + AttachmentSlots[usAttachmentSlotIndexVector[slotCount]].usDescPanelPosY - 1;

					//WarmSteel - Clear the background rectangle so we can paint it.
					if ( guiSAVEBUFFER == FRAME_BUFFER )
					{
						InvalidateRegion( sX, sY, (INT16)(sX + 8), (INT16)(sY + 8 ) );
					}
					else
					{
						RestoreExternBackgroundRect( sX, sY, 8, 8 );
					}
					if(AttachmentSlots[usAttachmentSlotIndexVector[slotCount]].fBigSlot){
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiAttachmentSlot, 1, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
					} else {
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiAttachmentSlot, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
			}
		}
		//Display the money 'seperating' border
 		if ( gpItemDescObject->usItem == MONEY )
		{
			//Render the money Boxes
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiMoneyGraphicsForDescBox, 0, (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[0].x - 1), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[0].y), VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// Display item
		if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ])
		{
			BltVideoObjectOutlineShadowFromIndex( guiSAVEBUFFER, guiItemGraphic, 0, sCenX - 2, sCenY + 2  );
		}
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemGraphic, 0, sCenX, sCenY, VO_BLT_SRCTRANSPARENCY, NULL );

		// HEADROCK HAM 5: Superimpose with Transform Icon graphic
		/* This bit will later be used for manual unjam via transformations. Hopefully. If not, erase it.
		if (Item[gpItemDescObject->usItem].usItemClass == IC_GUN && (*gpItemDescObject)[0]->data.gun.bGunAmmoStatus < 0 )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiTransformIconGraphic, 0, (ITEMDESC_ITEM_X+ITEMDESC_ITEM_WIDTH)-10, (ITEMDESC_ITEM_Y+ITEMDESC_ITEM_HEIGHT)-5, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else*/
		{
			for (UINT16 x = 0; x < MAXITEMS; x++)
			{
				if (Transform[x].usItem == -1)
				{
					break;
				}
				if (Transform[x].usItem == gpItemDescObject->usItem)
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiTransformIconGraphic, 0, (ITEMDESC_ITEM_X+ITEMDESC_ITEM_WIDTH)-13, (ITEMDESC_ITEM_Y+ITEMDESC_ITEM_HEIGHT)-17, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}
		}

		// Display status
		DrawItemUIBarEx( gpItemDescObject, gubItemDescStatusIndex, (INT16)ITEMDESC_ITEM_STATUS_X, (INT16)ITEMDESC_ITEM_STATUS_Y, ITEMDESC_ITEM_STATUS_WIDTH, ITEMDESC_ITEM_STATUS_HEIGHT, 	Get16BPPColor( DESC_STATUS_BAR ), Get16BPPColor( DESC_STATUS_BAR_SHADOW ), TRUE, guiSAVEBUFFER );

		// CHRISL:  This block will display hatching for inactive LBE pockets
		// Display LBENODE attached items
//		if(UsingNewInventorySystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_LBEGEAR)
//		{
//			RenderLBENODEItems( gpItemDescObject, gubItemDescStatusIndex );
//		}

		// HEADROCK HAM 4: First, display LBE Background
		if(UsingNewInventorySystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_LBEGEAR && showLBE >= 0)
		{
			// Exchange LBE images to conserve space in tactical, as appropriate for the UDB/ODB system used.
			INT8 showLBEImage = showLBE;
			if (guiCurrentItemDescriptionScreen != MAP_SCREEN)
				if (UsingEDBSystem() > 0)
					showLBEImage += 4;
				else
					showLBEImage += 8;

			// Draw LBE background image
//			BltVideoObjectFromIndex( guiSAVEBUFFER, guiItemInfoLBEBackground, showLBEImage, gItemDescLBEBackground[showLBE].sLeft, gItemDescLBEBackground[showLBE].sTop, VO_BLT_SRCTRANSPARENCY, NULL );
			// Render LBE items
			RenderLBENODEItems( gpItemDescObject, gubItemDescStatusIndex );
		}

		if (gpItemPointer)
		{
			if(UsingNewAttachmentSystem()==true)
			{
				//WarmSteel - This hatches out attachment slots one by one, instead of all of em.
				for(cnt = 0; cnt < (INT32)(*gpItemDescObject)[gubItemDescStatusIndex]->attachments.size(); cnt++)
				{
					//if ( ( Item[ gpItemPointer->usItem ].fFlags & ITEM_HIDDEN_ADDON ) ||
					if ( ( Item[ gpItemPointer->usItem ].hiddenaddon ) ||

					 ( !ValidItemAttachmentSlot( gpItemDescObject, gpItemPointer->usItem, FALSE, FALSE, gubItemDescStatusIndex, cnt, FALSE, NULL, usAttachmentSlotIndexVector) &&
						 !ValidMerge( gpItemPointer->usItem, gpItemDescObject->usItem ) ) )
					{
						//Madd: only if the slot isn't hidden
						if (  gItemDescAttachmentsXY[ cnt ].sHeight > 0 &&  gItemDescAttachmentsXY[ cnt ].sWidth > 0 )
						{
							// hatch out this attachment panel
							DrawHatchOnInventory( guiSAVEBUFFER, (INT16) (gsInvDescX + gItemDescAttachmentsXY[ cnt ].sX), (INT16) (gsInvDescY + gItemDescAttachmentsXY[ cnt ].sY - 2), (INT16)(gItemDescAttachmentsXY[ cnt ].sWidth + gItemDescAttachmentsXY[ cnt ].sBarDx), (INT16) (gItemDescAttachmentsXY[ cnt ].sHeight + 2) );
						}
					}
				}
			} else {
				for(cnt = 0; cnt < OLD_MAX_ATTACHMENTS_101; cnt++)
				{
					//if ( ( Item[ gpItemPointer->usItem ].fFlags & ITEM_HIDDEN_ADDON ) ||
					if ( ( Item[ gpItemPointer->usItem ].hiddenaddon ) ||

					 ( !ValidItemAttachment( gpItemDescObject, gpItemPointer->usItem, FALSE, FALSE, gubItemDescStatusIndex, usAttachmentSlotIndexVector) &&
						 !ValidMerge( gpItemPointer->usItem, gpItemDescObject->usItem ) && !ValidLaunchable( gpItemPointer->usItem, gpItemDescObject->usItem ) ) )
					{
						//Madd: only if the slot isn't hidden
						if (  gItemDescAttachmentsXY[ cnt ].sHeight > 0 &&  gItemDescAttachmentsXY[ cnt ].sWidth > 0 )
						{
							// hatch out the attachment panels
							DrawHatchOnInventory( guiSAVEBUFFER, (INT16) (gsInvDescX + gItemDescAttachmentsXY[ cnt ].sX), (INT16) (gsInvDescY + gItemDescAttachmentsXY[ cnt ].sY - 2), (INT16)(gItemDescAttachmentsXY[ cnt ].sWidth + gItemDescAttachmentsXY[ cnt ].sBarDx), (INT16) (gItemDescAttachmentsXY[ cnt ].sHeight + 2) );
						}
					}
				}
			}
		}

		// Display attachments
		cnt = 0;
		for (attachmentList::iterator iter = (*gpItemDescObject)[gubItemDescStatusIndex]->attachments.begin();
			iter != (*gpItemDescObject)[gubItemDescStatusIndex]->attachments.end(); ++iter, ++cnt) {

			//Madd: again, only do this if the slot isn't hidden
			if( iter->exists() && gItemDescAttachmentsXY[cnt].sHeight > 0 && gItemDescAttachmentsXY[cnt].sWidth > 0 ){
				sCenX = gsInvDescX + gItemDescAttachmentsXY[cnt].sX;
				sCenY = gsInvDescY + gItemDescAttachmentsXY[cnt].sY;
				INVRenderItem( guiSAVEBUFFER, NULL, gpItemDescObject, sCenX, sCenY, gItemDescAttachmentsXY[cnt].sWidth, gItemDescAttachmentsXY[cnt].sHeight, DIRTYLEVEL2, NULL, (UINT8)(RENDER_ITEM_ATTACHMENT1 + cnt), FALSE, 0, gubItemDescStatusIndex );
				sCenX = sCenX + gItemDescAttachmentsXY[cnt].sBarDx;
				sCenY = sCenY + gItemDescAttachmentsXY[cnt].sBarDy;
				DrawItemUIBarEx( gpItemDescObject, (UINT8)(DRAW_ITEM_STATUS_ATTACHMENT1 + cnt), sCenX, sCenY, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT, Get16BPPColor( STATUS_BAR ), Get16BPPColor( STATUS_BAR_SHADOW ), TRUE , guiSAVEBUFFER, gubItemDescStatusIndex );

				if ( gGameOptions.fWeaponOverheating && ( Item[ (iter)->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) || Item[ (iter)->usItem ].barrel == TRUE) )	// Flugente FTW 1.1
				{
					FLOAT overheatjampercentage = GetGunOverheatJamPercentage( &(*iter));
								
					UINT32 red, green, blue;
					UINT8 TemperatureStringNum = GetTemperatureString( overheatjampercentage, &red, &green, &blue );

					UINT16 colour = Get16BPPColor( FROMRGB( red, green, blue ) );

					DrawItemUIBarEx( &(*iter), DRAW_ITEM_TEMPERATURE, sCenX+4, sCenY, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT, colour, colour, TRUE, guiSAVEBUFFER );
				}
			}
		}

		// HEADROCK HAM 4: If vieweing the UDB Advanced page, reveal buttons and set them as appropriate
		if (UsingEDBSystem() && gubDescBoxPage == 2)
		{
			ShowButton( giInvDescAdvButton[0] );
			ShowButton( giInvDescAdvButton[1] );
			ItemDescAdvButtonCheck();
		}
		else if (UsingEDBSystem())
		{
			HideButton( giInvDescAdvButton[0] );
			HideButton( giInvDescAdvButton[1] );
		}

		//HEADROCK/CHRISL: This condition needs to run differently depending on whether EDB is active of not
		if(UsingEDBSystem() > 0)
		{
			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH )
			{
				DrawWeaponStats(gpItemDescObject);
			}
			else if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_AMMO )
			{
				DrawAmmoStats(gpItemDescObject);
			}
			else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
			{
				DrawExplosiveStats(gpItemDescObject);
			}
			else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR )
			{
				DrawArmorStats(gpItemDescObject);
			}
			else
			{
				DrawMiscStats(gpItemDescObject);
			}
		}
		else
		{
			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN || Item[ gpItemDescObject->usItem ].usItemClass & IC_LAUNCHER )
			{
				// display bullets for ROF
				if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiBullet, 0, gODBItemDescRegions[3][1].sLeft, gODBItemDescRegions[3][1].sTop, VO_BLT_SRCTRANSPARENCY, NULL );

				if ( GetShotsPerBurst(gpItemDescObject) > 0 )
				{
					for ( cnt = 0; cnt < __min(GetShotsPerBurst(gpItemDescObject),12); cnt++ )
					{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiBullet, 0, gODBItemDescRegions[3][4].sLeft + cnt * (BULLET_WIDTH/2 + 1), gODBItemDescRegions[3][4].sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
				else if ( GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
				{
					for ( cnt = 0; cnt < 10; cnt++ )
					{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiBullet, 0, gODBItemDescRegions[3][4].sLeft + cnt * (BULLET_WIDTH/2 + 1), gODBItemDescRegions[3][4].sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}

			}
		}

		if( guiCurrentItemDescriptionScreen == GAME_SCREEN )
			RestoreExternBackgroundRect( SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, ITEMDESC_WIDTH, ITEMDESC_HEIGHT );
		else
			RestoreExternBackgroundRect( gsInvDescX, gsInvDescY, ITEMDESC_WIDTH, ITEMDESC_HEIGHT );

		// CHRISL: This block will display misc information for items stored in LBE Pockets
		// Display LBENODE attached items
		if(UsingNewInventorySystem() == true && Item[gpItemDescObject->usItem].usItemClass == IC_LBEGEAR)
		{
			RenderLBENODEItems( gpItemDescObject, gubItemDescStatusIndex );
		}

		////////////////////////////////////////////////////////////////////////////////////
		// This section displays on screen all values that are common to both EDB and ODB.
		// They are drawn based on variable coordinates, which have been set beforehand
		// based on the system being used. 
		////////////////////////////////////////////////////////////////////////////////////

		// Render font desc
		SetFont( ITEMDESC_FONT );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_FCOLOR_WHITE );
		SetFontShadow( ITEMDESC_FONTSHADOW3 );

		// Render name
		// SET CLIPPING RECT FOR FONTS
		#ifdef JA2TESTVERSION
			mprintf( ITEMDESC_NAME_X, ITEMDESC_NAME_Y, L"%s (%d)", gzItemName, gpItemDescObject->usItem );
		#else
			mprintf( ITEMDESC_NAME_X, ITEMDESC_NAME_Y, L"%s", gzItemName );
		#endif

		// Render extra data on name bar for weapons and launchers (Caliber, Fingerprints)
		if ( ITEM_PROS_AND_CONS( gpItemDescObject->usItem ) )
		{
			SetFontForeground( FONT_BLACK );
			SetFontShadow( ITEMDESC_FONTSHADOW2 );
			// Caliber
			if ( (Item[gpItemDescObject->usItem].fingerprintid ) && (*gpItemDescObject)[gubItemDescStatusIndex]->data.ubImprintID < NO_PROFILE )
			{
				// Fingerprint ID
				swprintf( pStr, L"%s %s (%s)", AmmoCaliber[ Weapon[ gpItemDescObject->usItem ].ubCalibre ], WeaponType[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ], gMercProfiles[ (*gpItemDescObject)[gubItemDescStatusIndex]->data.ubImprintID ].zNickname );
			}
			else
			{
				// No Fingerprint ID
 				swprintf( pStr, L"%s %s", AmmoCaliber[ Weapon[ gpItemDescObject->usItem ].ubCalibre ], WeaponType[ Weapon[ gpItemDescObject->usItem ].ubWeaponType ] );
			}

			FindFontRightCoordinates( (INT16) ITEMDESC_CALIBER_X, (INT16) ITEMDESC_CALIBER_Y, ITEMDESC_CALIBER_WIDTH, ITEM_STATS_HEIGHT, pStr, ITEMDESC_FONT, &usX, &usY);
			mprintf( usX, usY, pStr );
		}

		/////////////////////////////////////////////////////////////////////////////////////
		// Render Item DESCRIPTION page. Contains the item's full-text description, item
		// condition, item weight, and for weapons also PROS and CONS.
		if (UsingEDBSystem() == 0 || gubDescBoxPage == 0)
		{
			UINT8 ForegroundColor;
			UINT8 ShadowColor;
			SetFont( BLOCKFONT2 );

			/////////// DISPLAY FULL DESCRIPTION TEXT
			if (UsingEDBSystem())
			{
				ForegroundColor = 6;
				ShadowColor = DEFAULT_SHADOW;
			}
			else
			{
				ForegroundColor = FONT_MCOLOR_BLACK;
				ShadowColor = ITEMDESC_FONTSHADOW2;
			}
			SetFontShadow( ShadowColor );

			DisplayWrappedString( gItemDescTextRegions[0].sLeft, gItemDescTextRegions[0].sTop, gItemDescTextRegions[0].sRight - gItemDescTextRegions[0].sLeft, 2, ITEMDESC_FONT, ForegroundColor,  gzItemDesc, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

			/////////// DISPLAY CONDITION
			ForegroundColor = 6;
			SetFontShadow( DEFAULT_SHADOW );

			// Note: for Ammo items, remaining bullets replace condition, so it's handled later on.
			// For money, condition is irrelevant, and not displayed here.
			if ( Item[ gpItemDescObject->usItem ].usItemClass != IC_AMMO && Item[ gpItemDescObject->usItem ].usItemClass != IC_MONEY )
			{
				////////// label
				if (UsingEDBSystem())
				{
					// Flugente FTW 1: display temperature string
					if ( gGameOptions.fWeaponOverheating && ( Item[ gpItemDescObject->usItem ].usItemClass == IC_GUN || Item[gpItemDescObject->usItem].usItemClass == IC_LAUNCHER || Item[gpItemDescObject->usItem].barrel == TRUE ) )
					{						
						// UDB system displays a string with colored condition text.
						int regionindex = 7;
						SetFontForeground( ForegroundColor );
						swprintf( pStr, L"%s", gTemperatureDesc[0] ); // "Temperature is "
						mprintf( gItemDescTextRegions[regionindex].sLeft, gItemDescTextRegions[regionindex].sTop, pStr );
						// Record length
						INT16 indent = StringPixLength( gTemperatureDesc[0], ITEMDESC_FONT );
						
						FLOAT overheatjampercentage = GetGunOverheatJamPercentage( gpItemDescObject);

						UINT32 red, green, blue;
						UINT8 TemperatureStringNum = GetTemperatureString( overheatjampercentage, &red, &green, &blue );
						swprintf( pStr, L"%s", gTemperatureDesc[TemperatureStringNum+1 ] );

						// text is coloured only in case of danger
						if ( overheatjampercentage >= 1.0 )
							SetRGBFontForeground( red, green, blue );

						mprintf( gItemDescTextRegions[regionindex].sLeft+indent+2, gItemDescTextRegions[regionindex].sTop, pStr );
						// Record length
						indent += StringPixLength( gTemperatureDesc[TemperatureStringNum + 1], ITEMDESC_FONT );

						SetFontForeground( ForegroundColor );
						swprintf( pStr, L"%s", gTemperatureDesc[10] ); // "."
						mprintf( gItemDescTextRegions[regionindex].sLeft + indent + 2, gItemDescTextRegions[regionindex].sTop, pStr );

						// to get the text to the left side...
						swprintf( pStr, L"");

						FindFontRightCoordinates( gItemDescTextRegions[regionindex].sLeft, gItemDescTextRegions[regionindex].sTop, gItemDescTextRegions[regionindex].sRight - gItemDescTextRegions[regionindex].sLeft ,gItemDescTextRegions[regionindex].sBottom - gItemDescTextRegions[regionindex].sTop ,pStr, BLOCKFONT2, &usX, &usY);

						mprintf( usX, usY, pStr );
					}

					// UDB system displays a string with colored condition text.
					SetFontForeground( ForegroundColor );
					swprintf( pStr, L"%s", gConditionDesc[0] ); // "In "
					mprintf( gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sTop, pStr );
					// Record length
					INT16 indent = StringPixLength( gConditionDesc[0], ITEMDESC_FONT );

					UINT8 ConditionColor = 0;
					UINT8 ConditionStringNum = GetConditionString( status, &ConditionColor );
					swprintf( pStr, L"%s", gConditionDesc[ConditionStringNum+1 ] );
					SetFontForeground( ConditionColor );
					mprintf( gItemDescTextRegions[1].sLeft+indent+2, gItemDescTextRegions[1].sTop, pStr );
					// Record length
					indent += StringPixLength( gConditionDesc[ConditionStringNum + 1], ITEMDESC_FONT );

					SetFontForeground( ForegroundColor );
					swprintf( pStr, L"%s", gConditionDesc[8] ); // " Condition."
					mprintf( gItemDescTextRegions[1].sLeft + indent + 2, gItemDescTextRegions[1].sTop, pStr );
				}
				else
				{
					SetFontForeground( ForegroundColor );
					swprintf( pStr, L"%s", gWeaponStatsDesc[ 0 ] );
					mprintf( gODBItemDescRegions[0][0].sLeft, gODBItemDescRegions[0][0].sTop, pStr );
				}

				SetFontForeground( 5 );

				// value
				// This is gross, but to get the % to work out right...
				swprintf( pStr, L"%2d%%", status);
				
				if (UsingEDBSystem())
				{
					FindFontRightCoordinates( gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sTop, gItemDescTextRegions[1].sRight - gItemDescTextRegions[1].sLeft, gItemDescTextRegions[1].sBottom - gItemDescTextRegions[1].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					FindFontRightCoordinates( gODBItemDescRegions[0][0].sLeft, gODBItemDescRegions[0][0].sTop, gODBItemDescRegions[0][0].sRight - gODBItemDescRegions[0][0].sLeft, gODBItemDescRegions[0][0].sBottom - gODBItemDescRegions[0][0].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				}

				#ifdef CHINESE
					wcscat( pStr, ChineseSpecString1 );
				#else			
					wcscat( pStr, L"%%" );
				#endif

				mprintf( usX, usY, pStr );
			}

			/////////// DISPLAY WEIGHT
			// Note: Money has no weight. Skip this.
			if(Item[ gpItemDescObject->usItem ].usItemClass != IC_MONEY)
			{
				SetFontForeground( 6 );

				// label
				if (UsingEDBSystem())
				{
					mprintf( gItemDescTextRegions[2].sLeft, gItemDescTextRegions[2].sTop, gWeaponStatsDesc[1] );
				}
				else
				{
					mprintf( gODBItemDescRegions[1][0].sLeft, gODBItemDescRegions[1][0].sTop, gWeaponStatsDesc[1] );
				}
				// Calculate total weight of item and attachments
				fWeight = gpItemDescObject->GetWeightOfObjectInStack(gubItemDescStatusIndex) / 10.0f;
				if ( !gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ] ) // metric units not enabled
				{
					fWeight = fWeight * 2.2f;
				}
				if ( fWeight < 0.1 && gubItemDescStatusIndex < gpItemDescObject->ubNumberOfObjects )
				{
					fWeight = 0.1f;
				}
				// Change value colors
				if (fWeight <= (EXCEPTIONAL_WEIGHT / 10) && Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON )
				{
					SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
				}
				else
				{
					SetFontForeground( 5 );
				}
				//Print
				swprintf( pStr, L"%1.1f %s", fWeight, GetWeightUnitString() );
				if (UsingEDBSystem())
				{
					FindFontRightCoordinates( gItemDescTextRegions[2].sLeft, gItemDescTextRegions[2].sTop, gItemDescTextRegions[2].sRight - gItemDescTextRegions[2].sLeft ,gItemDescTextRegions[2].sBottom - gItemDescTextRegions[2].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				}
				else
				{
					FindFontRightCoordinates( gODBItemDescRegions[1][0].sLeft, gODBItemDescRegions[1][0].sTop, gODBItemDescRegions[1][0].sRight - gODBItemDescRegions[1][0].sLeft ,gODBItemDescRegions[1][0].sBottom - gODBItemDescRegions[1][0].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				}
				mprintf( usX, usY, pStr );
			}

			/////////// DISPLAY PROS/CONS (weapons and launchers only)

			if ( ITEM_PROS_AND_CONS( gpItemDescObject->usItem ) )
			{
				SetFontForeground( FONT_MCOLOR_DKWHITE2 );
				SetFontShadow( ITEMDESC_FONTSHADOW3 );
				// PROs
				mprintf( gItemDescTextRegions[3].sLeft, gItemDescTextRegions[3].sTop, gzProsLabel );

				sProsConsIndent = __max( StringPixLength( gzProsLabel, ITEMDESC_FONT ), StringPixLength( gzConsLabel, ITEMDESC_FONT ) ) + 10;
				gzItemPros[0] = 0;
				GenerateProsString( gzItemPros, gpItemDescObject, (gItemDescTextRegions[4].sRight - gItemDescTextRegions[4].sLeft) - StringPixLength( DOTDOTDOT, ITEMDESC_FONT ) );
				if (gzItemPros[0] != 0)
				{
					if (UsingEDBSystem())
					{
						ForegroundColor = 5;
						SetFontShadow( DEFAULT_SHADOW );
					}
					else
					{
						ForegroundColor = FONT_MCOLOR_BLACK;
						SetFontShadow( ITEMDESC_FONTSHADOW2 );
					}

					DisplayWrappedString( gItemDescTextRegions[4].sLeft, gItemDescTextRegions[4].sTop, (gItemDescTextRegions[4].sRight - gItemDescTextRegions[4].sLeft), 2, ITEMDESC_FONT, ForegroundColor,  gzItemPros, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
 				}

				SetFontForeground( FONT_MCOLOR_DKWHITE2 );
				SetFontShadow( ITEMDESC_FONTSHADOW3 );
				// CONs
				mprintf( gItemDescTextRegions[5].sLeft, gItemDescTextRegions[5].sTop, gzConsLabel );

				// HEADROCK: Disabled PROs/CONs text for OIV EDB Tactical

				GenerateConsString( gzItemCons, gpItemDescObject, (gItemDescTextRegions[6].sRight - gItemDescTextRegions[6].sLeft) - StringPixLength( DOTDOTDOT, ITEMDESC_FONT ) );
				if (gzItemCons[0] != 0)
				{
					if (UsingEDBSystem())
					{
						ForegroundColor = 5;
						SetFontShadow( DEFAULT_SHADOW );
					}
					else
					{
						ForegroundColor = FONT_MCOLOR_BLACK;
						SetFontShadow( ITEMDESC_FONTSHADOW2 );
					}

					DisplayWrappedString( gItemDescTextRegions[6].sLeft, gItemDescTextRegions[6].sTop, (gItemDescTextRegions[6].sRight - gItemDescTextRegions[6].sLeft), 2, ITEMDESC_FONT, ForegroundColor,  gzItemCons, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
				}
				MSYS_EnableRegion( &(gProsAndConsRegions[ 0 ]) );
				MSYS_EnableRegion( &(gProsAndConsRegions[ 1 ]) );
			}
		}
		// HEADROCK HAM 4: If not displaying the UDB Description Page, disable the pros/cons rects.
		else
		{
			MSYS_DisableRegion( &(gProsAndConsRegions[ 0 ]) );
			MSYS_DisableRegion( &(gProsAndConsRegions[ 1 ]) );
		}

		// Render "normal" item data
		SetFontShadow( DEFAULT_SHADOW );

		//////////////////////////////////////////////////////////////////////////////
		// DATA VALUES
		// This segment combines the printing of "equals" signs and actual characteristic
		// data for each characterstic displayed. Labels are no longer used for most data, 
		// they are replaced by icons, which have already been blitted above.
		//////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////
		// BEGIN WEAPONS-SPECIFIC CHARACTERISTICS
		/////////////////////////////////////////
		if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH )
		{
			if(UsingEDBSystem() > 0)
			{
				SetFontShadow( ITEMDESC_FONTSHADOW3 );
				DrawWeaponValues(gpItemDescObject);
			}
			else
			{
				if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER|IC_THROWING_KNIFE) )
				{
					// RANGE
					SetFontForeground( 6 );
					mprintf( gODBItemDescRegions[2][4].sLeft, gODBItemDescRegions[2][4].sTop, L"%s", gWeaponStatsDesc[ 3 ] );

					if ( GunRange( gpItemDescObject, NULL ) >= EXCEPTIONAL_RANGE)
					{
						SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
					}
					else
					{
						SetFontForeground( 5 );
					}
					swprintf( pStr, L"%2d", ( GunRange( gpItemDescObject, NULL ) ) / 10 );
					FindFontRightCoordinates( gODBItemDescRegions[2][7].sLeft, gODBItemDescRegions[2][7].sTop, gODBItemDescRegions[2][7].sRight - gODBItemDescRegions[2][7].sLeft, gODBItemDescRegions[2][7].sBottom - gODBItemDescRegions[2][7].sTop ,pStr, BLOCKFONT2, &usX, &usY);
					mprintf( usX, usY, pStr );
				}
				if ( Item[ gpItemDescObject->usItem ].usItemClass & (IC_GUN|IC_PUNCH|IC_BLADE|IC_THROWING_KNIFE) && !Item[ gpItemDescObject->usItem ].singleshotrocketlauncher )
				{
					// DAMAGE
					SetFontForeground( 6 );
					mprintf( gODBItemDescRegions[2][0].sLeft, gODBItemDescRegions[2][0].sTop, L"%s", gWeaponStatsDesc[ 4 ] );

					UINT8 ubImpact = GetDamage(gpItemDescObject);

				UINT16 exceptionalDamage;
				// Melee damage
				if ( Item[gpItemDescObject->usItem].usItemClass == IC_BLADE || Item[gpItemDescObject->usItem].usItemClass == IC_PUNCH || Item[gpItemDescObject->usItem].usItemClass == IC_TENTACLES )
				{
					exceptionalDamage = (UINT16)(( EXCEPTIONAL_MELEE_DAMAGE * gGameExternalOptions.iMeleeDamageModifier ) / 100);
				}
				// Gun damage
				else
				{
					exceptionalDamage = (UINT16)(( EXCEPTIONAL_GUN_DAMAGE * gGameExternalOptions.iGunDamageModifier ) / 100);
				}

				if ( ubImpact  >= exceptionalDamage)
				{
					SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
				}
				else
				{
					SetFontForeground( 5 );
				}

					//Damage
					swprintf( pStr, L"%2d", ubImpact );
					FindFontRightCoordinates( gODBItemDescRegions[2][3].sLeft, gODBItemDescRegions[2][3].sTop, gODBItemDescRegions[2][3].sRight - gODBItemDescRegions[2][3].sLeft, gODBItemDescRegions[2][3].sBottom - gODBItemDescRegions[2][3].sTop ,pStr, BLOCKFONT2, &usX, &usY);
					mprintf( usX, usY, pStr );
				}

				//LABELS
				SetFontForeground( 6 );
				mprintf( gODBItemDescRegions[3][0].sLeft, gODBItemDescRegions[3][0].sTop, L"%s", gWeaponStatsDesc[ 6 ] );
				if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN || Item[ gpItemDescObject->usItem ].usItemClass & IC_LAUNCHER)
				{
					// equals sign
					if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
						mprintf( gODBItemDescRegions[3][2].sLeft, gODBItemDescRegions[3][2].sTop, L"%s", gWeaponStatsDesc[ 7 ] );
				}

				if (GetShotsPerBurst(gpItemDescObject) > 0 || GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0 )
				{
					mprintf( gODBItemDescRegions[3][6].sLeft, gODBItemDescRegions[3][6].sTop, L"%s", gWeaponStatsDesc[ 8 ] );
				}

				ubAttackAPs = BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], gpItemDescObject, NULL );

				// WANNE: Fixed CTD when trowing an item with open description box
				if (ubAttackAPs != -1 )
				{
					if (ubAttackAPs <= EXCEPTIONAL_AP_COST)
					{
						SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
					}
					else
					{
						SetFontForeground( 5 );
					}

					 //AP's
					if ( !Weapon[gpItemDescObject->usItem].NoSemiAuto )
					{
						swprintf( pStr, L"%2d", ubAttackAPs );
						FindFontRightCoordinates( gODBItemDescRegions[3][3].sLeft, gODBItemDescRegions[3][3].sTop, gODBItemDescRegions[3][3].sRight - gODBItemDescRegions[3][3].sLeft, gODBItemDescRegions[3][3].sBottom - gODBItemDescRegions[3][3].sTop ,pStr, BLOCKFONT2, &usX, &usY);
						mprintf( usX, usY, pStr );
					}

					if (GetShotsPerBurst(gpItemDescObject) > 0)
					{
						if (GetShotsPerBurst(gpItemDescObject) >= EXCEPTIONAL_BURST_SIZE )//|| gpItemDescObject->usItem == G11)
						{
							SetFontForeground( ITEMDESC_FONTHIGHLIGHT );
						}
						else
						{
							SetFontForeground( 5 );
						}
						swprintf( pStr, L"%2d", ubAttackAPs + CalcAPsToBurst( APBPConstants[DEFAULT_APS], gpItemDescObject, NULL ) );
						FindFontRightCoordinates( gODBItemDescRegions[3][7].sLeft, gODBItemDescRegions[3][7].sTop, gODBItemDescRegions[3][7].sRight - gODBItemDescRegions[3][7].sLeft, gODBItemDescRegions[3][7].sBottom - gODBItemDescRegions[3][7].sTop ,pStr, BLOCKFONT2, &usX, &usY);
						mprintf( usX, usY, pStr );
					}
					else if (GetAutofireShotsPerFiveAPs(gpItemDescObject) > 0)
					{
						SetFontForeground( 5 );
						swprintf( pStr, L"%2d", ubAttackAPs + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], gpItemDescObject, 3, NULL ) );
						FindFontRightCoordinates( gODBItemDescRegions[3][7].sLeft, gODBItemDescRegions[3][7].sTop, gODBItemDescRegions[3][7].sRight - gODBItemDescRegions[3][7].sLeft, gODBItemDescRegions[3][7].sBottom - gODBItemDescRegions[3][7].sTop ,pStr, BLOCKFONT2, &usX, &usY);
						mprintf( usX, usY, pStr );
					}
				}
				else
				{
					// WANNE: Close the description box after we threw the item.
					DeleteItemDescriptionBox( );
				}
			}
		}
		else if ( gpItemDescObject->usItem == MONEY )
		{
			if ((UsingEDBSystem() && gubDescBoxPage == 0) || !UsingEDBSystem())
			{
				for (INT8 x = 0; x < 4; x++)
				{
					EnableButton(guiMoneyButtonBtn[ x ]);
				}
				//Labels
				SetFont( BLOCKFONT2 );
				SetFontShadow( DEFAULT_SHADOW );
				SetFontForeground( 6 );

				//Display the 'Seperate text'
				if( gfAddingMoneyToMercFromPlayersAccount )
					mprintf( (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[4].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[4].y), gzMoneyAmounts[5] );
				else
					mprintf( (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[4].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[4].y ), gzMoneyAmounts[4] );

				//if the player is removing money from the players account
				if( gfAddingMoneyToMercFromPlayersAccount )
				{
					//Display the 'Removing'
					mprintf( gMoneyStats[ 0 ].sX, gMoneyStats[ 0 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_PLAYERS ] );
					//Display the 'REmaining'
					mprintf( gMoneyStats[ 2 ].sX, gMoneyStats[ 2 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_AMOUNT_2_WITHDRAW ] );
				}
				else
				{
					//Display the 'Removing'
					mprintf( gMoneyStats[ 0 ].sX, gMoneyStats[ 0 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_AMOUNT ] );
					//Display the 'REmaining'
					mprintf( gMoneyStats[ 2 ].sX, gMoneyStats[ 2 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_AMOUNT_2_SPLIT ] );
				}

				// if the player is taking money from their account
				if( gfAddingMoneyToMercFromPlayersAccount )
				{
					//Display the 'Amt removing'
					mprintf( gMoneyStats[ 1 ].sX, gMoneyStats[ 1 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_BALANCE ] );
					//Display the 'REmaining amount'
					mprintf( gMoneyStats[ 3 ].sX, gMoneyStats[ 3 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_TO_WITHDRAW ] );
				}
				else
				{
					//Display the 'Amt removing'
					mprintf( gMoneyStats[ 1 ].sX, gMoneyStats[ 1 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_REMAINING ] );
					//Display the 'REmaining amount'
					mprintf( gMoneyStats[ 3 ].sX, gMoneyStats[ 3 ].sY, L"%s", gMoneyStatsDesc[ MONEY_DESC_TO_SPLIT ] );
				}

				//Display the total amount of money remaining
				SetFontForeground( 5 );
				swprintf( pStr, L"%ld", gRemoveMoney.uiMoneyRemaining );
				InsertCommasForDollarFigure( pStr );
				InsertDollarSignInToString( pStr );
				FindFontRightCoordinates( gMoneyStats[ 4 ].sX + 50, gMoneyStats[ 4 ].sY, ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );

				//Display the total amount of money removing
				SetFontForeground( 5 );
				swprintf( pStr, L"%ld", gRemoveMoney.uiMoneyRemoving );
				InsertCommasForDollarFigure( pStr );
				InsertDollarSignInToString( pStr );
				FindFontRightCoordinates( gMoneyStats[ 5 ].sX + 50, gMoneyStats[ 5 ].sY, ITEM_STATS_WIDTH ,ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );
			}
			else
			{
				for (INT8 x = 0; x < 4; x++)
				{
					DisableButton(guiMoneyButtonBtn[ x ]);
				//Labels
				SetFont( BLOCKFONT2 );
				SetFontShadow( DEFAULT_SHADOW );
				SetFontForeground( FONT_GRAY6 );

				//Display the 'Seperate text'
				if( gfAddingMoneyToMercFromPlayersAccount )
					mprintf( (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[4].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[4].y), gzMoneyAmounts[5] );
				else
					mprintf( (UINT16)(gMoneyButtonLoc.x + gMoneyButtonOffsets[4].x), (UINT16)(gMoneyButtonLoc.y + gMoneyButtonOffsets[4].y ), gzMoneyAmounts[4] );
				}
			}
		}
		else if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_MONEY )
		{
			SetFontForeground( FONT_FCOLOR_WHITE );
			SetFontShadow( DEFAULT_SHADOW );
			swprintf( pStr, L"%ld", (*gpItemDescObject)[gubItemDescStatusIndex]->data.money.uiMoneyAmount );
			InsertCommasForDollarFigure( pStr );
			InsertDollarSignInToString( pStr );
			if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
				FindFontRightCoordinates( (INT16)(ITEMDESC_NAME_X), (INT16)(ITEMDESC_NAME_Y ), 245, ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			else if ( UsingEDBSystem() > 0 )
				FindFontRightCoordinates( (INT16) ITEMDESC_CALIBER_X, (INT16) ITEMDESC_CALIBER_Y, ITEMDESC_CALIBER_WIDTH, ITEM_STATS_HEIGHT, pStr, ITEMDESC_FONT, &usX, &usY);
			else
				FindFontRightCoordinates( (INT16)(ITEMDESC_NAME_X), (INT16)(ITEMDESC_NAME_Y ), 295, ITEM_STATS_HEIGHT ,pStr, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY, pStr );
		}
		else if ( ( InKeyRingPopup() == TRUE ) || ( Item[ gpItemDescObject->usItem ].usItemClass & IC_KEY ) )
		{
			SetFontForeground( 6 );
			SetFontShadow( DEFAULT_SHADOW );

			if(UsingEDBSystem() > 0 && gubDescBoxPage == 0)
			{
				// build description for keys .. the sector found
				swprintf( pStr, L"%s", sKeyDescriptionStrings[ 0 ] );
				mprintf( gItemDescTextRegions[3].sLeft, gItemDescTextRegions[3].sTop, pStr );
				swprintf( pStr, L"%s", sKeyDescriptionStrings[ 1 ] );
				mprintf( gItemDescTextRegions[5].sLeft, gItemDescTextRegions[5].sTop, pStr );
			}
			else if(UsingEDBSystem() == 0)
			{
				// build description for keys .. the sector found
				swprintf( pStr, L"%s", sKeyDescriptionStrings[ 0 ] );
				mprintf( gODBItemDescRegions[2][0].sLeft, gODBItemDescRegions[2][0].sTop, pStr );
				swprintf( pStr, L"%s", sKeyDescriptionStrings[ 1 ] );
				mprintf( gODBItemDescRegions[3][0].sLeft, gODBItemDescRegions[3][0].sTop, pStr );
			}

			SetFontForeground( 5 );
			GetShortSectorString( ( INT16 ) SECTORX( KeyTable[ (*gpItemDescObject)[gubItemDescStatusIndex]->data.key.ubKeyID ].usSectorFound ), ( INT16 ) SECTORY( KeyTable[ (*gpItemDescObject)[gubItemDescStatusIndex]->data.key.ubKeyID ].usSectorFound ), sTempString  );
			swprintf( pStr, L"%s", sTempString );
			if(UsingEDBSystem() > 0 && gubDescBoxPage == 0)
			{
				FindFontRightCoordinates( gItemDescTextRegions[4].sLeft, gItemDescTextRegions[4].sTop, gItemDescTextRegions[4].sRight - gItemDescTextRegions[4].sLeft ,gItemDescTextRegions[4].sBottom - gItemDescTextRegions[4].sTop ,pStr, BLOCKFONT2, &sStrX, &usY);
				mprintf( sStrX, usY, pStr );
			}
			else if (UsingEDBSystem() == 0)
			{
				FindFontRightCoordinates( gODBItemDescRegions[2][7].sLeft, gODBItemDescRegions[2][7].sTop, gODBItemDescRegions[2][7].sRight - gODBItemDescRegions[2][7].sLeft ,gODBItemDescRegions[2][7].sBottom - gODBItemDescRegions[2][7].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );
			}

			swprintf( pStr, L"%d", KeyTable[ (*gpItemDescObject)[gubItemDescStatusIndex]->data.key.ubKeyID ].usDateFound );
			if(UsingEDBSystem() > 0 && gubDescBoxPage == 0)
			{
				FindFontRightCoordinates( gItemDescTextRegions[6].sLeft, gItemDescTextRegions[6].sTop, gItemDescTextRegions[6].sRight - gItemDescTextRegions[6].sLeft ,gItemDescTextRegions[6].sBottom - gItemDescTextRegions[6].sTop ,pStr, BLOCKFONT2, &sStrX, &usY);
				mprintf( sStrX, usY, pStr );
			}
			else if (UsingEDBSystem() == 0)
			{
				FindFontRightCoordinates( gODBItemDescRegions[3][7].sLeft, gODBItemDescRegions[3][7].sTop, gODBItemDescRegions[3][7].sRight - gODBItemDescRegions[3][7].sLeft ,gODBItemDescRegions[3][7].sBottom - gODBItemDescRegions[3][7].sTop ,pStr, BLOCKFONT2, &usX, &usY);
				mprintf( usX, usY, pStr );
			}
		}
		else
		{
			//Labels
			SetFont( BLOCKFONT2 );
			SetFontForeground( 6 );
			SetFontShadow( DEFAULT_SHADOW );

			if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_AMMO )
			{
				if(UsingEDBSystem() > 0)
					DrawAmmoValues(gpItemDescObject,shotsLeft);
				else
				{
					mprintf( gODBItemDescRegions[0][0].sLeft, gODBItemDescRegions[0][0].sTop, L"%s", gWeaponStatsDesc[ 5 ] );

					// Values
					SetFontForeground( 5 );

					// Ammo
					swprintf( pStr, L"%d/%d", shotsLeft, Magazine[ Item[ gpItemDescObject->usItem ].ubClassIndex ].ubMagSize ); //Pulmu: Correct # of rounds for stacked ammo.
					uiStringLength=StringPixLength(pStr, ITEMDESC_FONT );
					FindFontRightCoordinates( gODBItemDescRegions[0][0].sLeft, gODBItemDescRegions[0][0].sTop, gODBItemDescRegions[0][0].sRight - gODBItemDescRegions[0][0].sLeft ,gODBItemDescRegions[0][0].sBottom - gODBItemDescRegions[0][0].sTop ,pStr, BLOCKFONT2, &sStrX, &usY);
					mprintf( sStrX, usY, pStr );
				}
			}
			else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_EXPLOSV )
			{
				if(UsingEDBSystem() > 0)
					DrawExplosiveValues(gpItemDescObject);
			}
			else if ( Item[ gpItemDescObject->usItem ].usItemClass & IC_ARMOUR )
			{
				if(UsingEDBSystem() > 0)
					DrawArmorValues(gpItemDescObject);
			}
			else
			{
				if(UsingEDBSystem() > 0)
					DrawMiscValues(gpItemDescObject);
			}
		}
		SetFontShadow( DEFAULT_SHADOW );
	}
}

// CHRISL: New function to display items stored in an LBENODE
void RenderLBENODEItems( OBJECTTYPE *pObj, int subObject )
{
	extern int			PLAYER_INFO_X;
	extern int			PLAYER_INFO_Y;
	INV_REGIONS			LBEInvPocketXY[12];
	SOLDIERTYPE	*pSoldier;
	OBJECTTYPE	*pObject;
	INT16		sX, sY;
	INT16		sBarX, sBarY;
	INT16		lbePocket = ITEM_NOT_FOUND;
	UINT32		lClass;
	bool		activeNode = pObj->IsActiveLBE(subObject);
	bool		wornItem = false;
	int			usX, usY;

	// Start by verifying that this item is an LBENODE
	if(Item[pObj->usItem].usItemClass != IC_LBEGEAR || (iResolution >= _640x480 && iResolution < _800x600))
		return;

	// Set selected merc
	if(guiCurrentItemDescriptionScreen == MAP_SCREEN)
		GetSoldier( &pSoldier, gCharactersList[bSelectedInfoChar].usSolID );
	else
		pSoldier = gpSMCurrentMerc;
	
	// Is the object we're looking at currently worn
	for(unsigned int x = VESTPOCKPOS; x <= BPACKPOCKPOS; x++)
	{
		if(pObj == &pSoldier->inv[x])
		{
			wornItem = true;
			break;
		}
	}

	LBENODE* pLBE = NULL;
	if(activeNode == true)
	{
		pLBE = pObj->GetLBEPointer(subObject);
		lClass = pLBE->lbeClass;
	}
	else {
		lClass = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass;
	}

	std::vector<INT8> pocketKey;
	InitLBECoords();
	if(guiCurrentItemDescriptionScreen == MAP_SCREEN)
	{
		usX = PLAYER_INFO_X;
		usY = PLAYER_INFO_Y;
	}
	else
	{
		usX = INTERFACE_START_X;
		usY = INV_INTERFACE_START_Y;
	}
	// Setup pocket coords
	switch (lClass)
	{
		case THIGH_PACK:
			if(pObj->IsActiveLBE(subObject) == false && pObj == &pSoldier->inv[RTHIGHPOCKPOS])
				GetLBESlots(RTHIGHPOCKPOS, pocketKey);
			else
				GetLBESlots(LTHIGHPOCKPOS, pocketKey);
			InitInvData(LBEInvPocketXY[0], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[0].sX, usY + gLBEStats[0].sY);
			InitInvData(LBEInvPocketXY[1], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[1].sX, usY + gLBEStats[1].sY);
			InitInvData(LBEInvPocketXY[2], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[2].sX, usY + gLBEStats[2].sY);
			InitInvData(LBEInvPocketXY[3], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[3].sX, usY + gLBEStats[3].sY);
			InitInvData(LBEInvPocketXY[4], 2    , INV_BAR_DX, INV_BAR_DY, VEST_INV_SLOT_WIDTH, VEST_INV_SLOT_HEIGHT, usX + gLBEStats[4].sX, usY + gLBEStats[4].sY);
			InitInvData(LBEInvPocketXY[5], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[5].sX, usY + gLBEStats[5].sY);
			InitInvData(LBEInvPocketXY[6], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[6].sX, usY + gLBEStats[6].sY);
			InitInvData(LBEInvPocketXY[7], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[7].sX, usY + gLBEStats[7].sY);
			InitInvData(LBEInvPocketXY[8], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[8].sX, usY + gLBEStats[8].sY);
			InitInvData(LBEInvPocketXY[9], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[9].sX, usY + gLBEStats[9].sY);
			InitInvData(LBEInvPocketXY[10], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[10].sX, usY + gLBEStats[10].sY);
			InitInvData(LBEInvPocketXY[11], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[11].sX, usY + gLBEStats[11].sY);
			break;
		case VEST_PACK:
			GetLBESlots(VESTPOCKPOS, pocketKey);
			InitInvData(LBEInvPocketXY[0], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[12].sX, usY + gLBEStats[12].sY);
			InitInvData(LBEInvPocketXY[1], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[13].sX, usY + gLBEStats[13].sY);
			InitInvData(LBEInvPocketXY[2], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[14].sX, usY + gLBEStats[14].sY);
			InitInvData(LBEInvPocketXY[3], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[15].sX, usY + gLBEStats[15].sY);
			InitInvData(LBEInvPocketXY[4], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[16].sX, usY + gLBEStats[16].sY);
			InitInvData(LBEInvPocketXY[5], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[17].sX, usY + gLBEStats[17].sY);
			InitInvData(LBEInvPocketXY[6], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[18].sX, usY + gLBEStats[18].sY);
			InitInvData(LBEInvPocketXY[7], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[19].sX, usY + gLBEStats[19].sY);
			InitInvData(LBEInvPocketXY[8], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[20].sX, usY + gLBEStats[20].sY);
			InitInvData(LBEInvPocketXY[9], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[21].sX, usY + gLBEStats[21].sY);
			InitInvData(LBEInvPocketXY[10], 2, INV_BAR_DX, INV_BAR_DY, VEST_INV_SLOT_WIDTH, VEST_INV_SLOT_HEIGHT, usX + gLBEStats[22].sX, usY + gLBEStats[22].sY);
			InitInvData(LBEInvPocketXY[11], 2, INV_BAR_DX, INV_BAR_DY, VEST_INV_SLOT_WIDTH, VEST_INV_SLOT_HEIGHT, usX + gLBEStats[23].sX, usY + gLBEStats[23].sY);
			break;
		case COMBAT_PACK:
			GetLBESlots(CPACKPOCKPOS, pocketKey);
			InitInvData(LBEInvPocketXY[0], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[24].sX, usY + gLBEStats[24].sY);
			InitInvData(LBEInvPocketXY[1], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[25].sX, usY + gLBEStats[25].sY);
			InitInvData(LBEInvPocketXY[2], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[26].sX, usY + gLBEStats[26].sY);
			InitInvData(LBEInvPocketXY[3], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[27].sX, usY + gLBEStats[27].sY);
			InitInvData(LBEInvPocketXY[4], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[28].sX, usY + gLBEStats[28].sY);
			InitInvData(LBEInvPocketXY[5], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[29].sX, usY + gLBEStats[29].sY);
			InitInvData(LBEInvPocketXY[6], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[30].sX, usY + gLBEStats[30].sY);
			InitInvData(LBEInvPocketXY[7], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[31].sX, usY + gLBEStats[31].sY);
			InitInvData(LBEInvPocketXY[8], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[32].sX, usY + gLBEStats[32].sY);
			InitInvData(LBEInvPocketXY[9], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[33].sX, usY + gLBEStats[33].sY);
			InitInvData(LBEInvPocketXY[10], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[34].sX, usY + gLBEStats[34].sY);
			InitInvData(LBEInvPocketXY[11], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[35].sX, usY + gLBEStats[35].sY);
			break;
		case BACKPACK:
			GetLBESlots(BPACKPOCKPOS, pocketKey);
			InitInvData(LBEInvPocketXY[0], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[36].sX, usY + gLBEStats[36].sY);
			InitInvData(LBEInvPocketXY[1], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[37].sX, usY + gLBEStats[37].sY);
			InitInvData(LBEInvPocketXY[2], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[38].sX, usY + gLBEStats[38].sY);
			InitInvData(LBEInvPocketXY[3], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[39].sX, usY + gLBEStats[39].sY);
			InitInvData(LBEInvPocketXY[4], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[40].sX, usY + gLBEStats[40].sY);
			InitInvData(LBEInvPocketXY[5], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[41].sX, usY + gLBEStats[41].sY);
			InitInvData(LBEInvPocketXY[6], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[42].sX, usY + gLBEStats[42].sY);
			InitInvData(LBEInvPocketXY[7], FALSE, INV_BAR_DX, INV_BAR_DY, SM_INV_SLOT_WIDTH, SM_INV_SLOT_HEIGHT, usX + gLBEStats[43].sX, usY + gLBEStats[43].sY);
			InitInvData(LBEInvPocketXY[8], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[44].sX, usY + gLBEStats[44].sY);
			InitInvData(LBEInvPocketXY[9], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[45].sX, usY + gLBEStats[45].sY);
			InitInvData(LBEInvPocketXY[10], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[46].sX, usY + gLBEStats[46].sY);
			InitInvData(LBEInvPocketXY[11], TRUE, INV_BAR_DX, INV_BAR_DY, BIG_INV_SLOT_WIDTH, BIG_INV_SLOT_HEIGHT, usX + gLBEStats[47].sX, usY + gLBEStats[47].sY);
			break;
		default:
			break;
	}
	// Display contents of LBENODE
	for(unsigned int cnt=0; cnt<pocketKey.size(); cnt++)
	{
		sX = LBEInvPocketXY[cnt].sX;
		sY = LBEInvPocketXY[cnt].sY;
		if(sX != 0 && sY != 0)
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiAttachmentSlot, LBEInvPocketXY[cnt].fBigPocket, sX-7, sY-1, VO_BLT_SRCTRANSPARENCY, NULL );
		lbePocket = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[icPocket[pocketKey[cnt]]];
		pObject = NULL;
		if(wornItem == true)
		{
			if(pSoldier->inv[pocketKey[cnt]].exists() == true)
				pObject = &(pSoldier->inv[pocketKey[cnt]]);
		}
		else if(activeNode == true)
		{
			if(pLBE->inv[cnt].exists() == true)
				pObject = &pLBE->inv[cnt];
		}
		if(lbePocket == 0){	// Deactivate Pocket
			DrawHatchOnInventory( guiSAVEBUFFER, sX, sY, (UINT16)(LBEInvPocketXY[cnt].sWidth-1), (UINT16)(LBEInvPocketXY[cnt].sHeight-1), 0 );
		}
		else if(pObject == NULL){	// Nothing in sPocket.  Display silouhette.
			INVRenderSilhouette( guiSAVEBUFFER, lbePocket, 0, sX, sY, LBEInvPocketXY[cnt].sWidth, LBEInvPocketXY[cnt].sHeight);
		}
		else if(pObject != NULL){
			INVRenderItem( guiSAVEBUFFER, pSoldier, pObject, sX, sY, LBEInvPocketXY[cnt].sWidth, LBEInvPocketXY[cnt].sHeight, 2, NULL, 0, 0, 0 );
			sBarX = sX - LBEInvPocketXY[cnt].sBarDx;
			sBarY = sY + LBEInvPocketXY[cnt].sBarDy;
			DrawItemUIBarEx( pObject, 0, sBarX, sBarY, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT,	Get16BPPColor( STATUS_BAR ), Get16BPPColor( STATUS_BAR_SHADOW ), TRUE , guiSAVEBUFFER);
		}
//		if (gpItemPointer != NULL)
//			RenderPocketItemCapacity( ItemSlotLimit(gpItemPointer, pocketKey[cnt], pSoldier), pocketKey[cnt], pSoldier);
		//CHRISL: Remove this line for now.  Though we do need something to better show that pockets on this screen are read-only
		//ShadowVideoSurfaceRect(guiSAVEBUFFER,sX,sY,(gSMInvData[ pocketKey[cnt] ].sWidth+sX),(gSMInvData[ pocketKey[cnt] ].sHeight+sY));
	}
}

void HandleItemDescriptionBox( BOOLEAN *pfDirty )
{
	if ( fItemDescDelete )
	{
		DeleteItemDescriptionBox( );
		fItemDescDelete = FALSE;
		*pfDirty = DIRTYLEVEL2;
	}

}


void DeleteItemDescriptionBox( )
{
	INT32 cnt;

	if( gfInItemDescBox == FALSE )
	{
		return;
	}

//	DEF:

	//Used in the shopkeeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		DeleteShopKeeperItemDescBox();
	}

	// check for any AP costs
	if ( ( gTacticalStatus.uiFlags & TURNBASED ) && ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if (gpAttachSoldier)
		{
			UINT8 ubAPCost = 0;

			// check for change in attachments
			//unsigned int originalSize = gOriginalAttachments.size();
			//unsigned int newSize = (*gpItemDescObject)[0]->attachments.size();
			
			//WarmSteel size() is no longer sufficient, because the list contains null objects.
			unsigned int originalSize = 0;
			unsigned int newSize = 0;
			attachmentList::iterator originalIter;
			attachmentList::iterator newIter;

			for (originalIter = gOriginalAttachments.begin(), newIter = (*gpItemDescObject)[0]->attachments.begin();
			originalIter != gOriginalAttachments.end() && newIter != (*gpItemDescObject)[0]->attachments.end();
			++originalIter, ++newIter){
				if(originalIter->exists()){
					originalSize++;
				}
				if(newIter->exists()){
					newSize++;
				}
			}


			if (newSize != originalSize) {
				//an attachment was changed, find the change
				for (originalIter = gOriginalAttachments.begin(), newIter = (*gpItemDescObject)[0]->attachments.begin();
					originalIter != gOriginalAttachments.end() && newIter != (*gpItemDescObject)[0]->attachments.end();
					++originalIter, ++newIter) {
					if (*originalIter == *newIter) {
						continue;
					}
					else {
						break;
					}
				}
				if (newSize < originalSize) {
					//an attachment was removed, charge APs
					ubAPCost = AttachmentAPCost(originalIter->usItem,gpItemDescObject, gpAttachSoldier ); // SANDRO - added argument
				}
				else {
					//an attachment was added charge APs
					//lalien: changed to charge AP's for reloading a GL/RL
					if ( Item[ gpItemDescObject->usItem ].usItemClass == IC_LAUNCHER || Item[gpItemDescObject->usItem].cannon )
					{
						ubAPCost = GetAPsToReload( gpItemDescObject );
					}
					else
					{
						ubAPCost = AttachmentAPCost(newIter->usItem,gpItemDescObject, gpAttachSoldier); // SANDRO - added argument
					}
				}
			}

			if (ubAPCost)
			{
				DeductPoints( gpAttachSoldier, ubAPCost, 0 );
			}
		}
	}

	//Remove
	DeleteVideoObjectFromIndex( guiItemDescBox );
	if(UsingNewInventorySystem() == true && guiItemDescBoxBackground != 0 && !gfInKeyRingPopup)
		DeleteVideoObjectFromIndex( guiItemDescBoxBackground );
	if(UsingNewInventorySystem() == true && guiItemInfoLBEBackground != 0)
		DeleteVideoObjectFromIndex( guiItemInfoLBEBackground );
	DeleteVideoObjectFromIndex( guiMapItemDescBox );
	DeleteVideoObjectFromIndex( guiAttachmentSlot );
	// HEADROCK HAM 5: Transform Icon
	DeleteVideoObjectFromIndex( guiTransformIconGraphic );
	RenderBackpackButtons(ACTIVATE_BUTTON);	// CHRISL: Needed for new inventory backpack buttons 
	if(guiCurrentItemDescriptionScreen == SHOPKEEPER_SCREEN && gGameSettings.fOptions[TOPTION_ENHANCED_DESC_BOX])
		EnableDisableShopkeeperButtons(guiCurrentItemDescriptionScreen, ACTIVATE_BUTTON);
	DeleteVideoObjectFromIndex( guiBullet );
	DeleteEnhancedDescBox( guiCurrentItemDescriptionScreen );
	// Delete item graphic
	DeleteVideoObjectFromIndex( guiItemGraphic );

	gfInItemDescBox = FALSE;

//	if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN  )
	if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
	{
		UnloadButtonImage( giMapInvDescButtonImage );
		RemoveButton( giMapInvDescButton );
	}

	// HEADROCK HAM 4: Remove TAB buttons for UDB
	if (UsingEDBSystem() > 0)
	{
		if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
		{
			UnloadButtonImage( giMapInvDescTabButtonImage );
		}
		else
		{
			UnloadButtonImage( giInvDescTabButtonImage );
		}
		for (cnt = 0; cnt < 3; cnt++)
		{
			RemoveButton( giInvDescTabButton[cnt] );
		}
	}

	// HEADROCK HAM 4: Remove ADVANCED buttons for UDB
	if (UsingEDBSystem() > 0)
	{
		UnloadButtonImage( giInvDescAdvButtonUpImage );
		UnloadButtonImage( giInvDescAdvButtonDownImage );
		for (cnt = 0; cnt < 2; cnt++)
		{
			if ( giInvDescAdvButton[cnt] != -1 )
			{
				RemoveButton( giInvDescAdvButton[cnt] );
			}
		}
	}

	// HEADROCK HAM 5: Remove Transform Item Region
	if (&gInvDescTransformRegion)
	{
		MSYS_RemoveRegion( &gInvDescTransformRegion );
		if (gfItemDescTransformPopupInitialized && !gfSkipDestroyTransformPopup ) // Gotta make sure we don't destroy it when it's running!
		{
			delete(gItemDescTransformPopup);
			gItemDescTransformPopup = NULL;
			gfItemDescTransformPopupInitialized = FALSE;
			gfItemDescTransformPopupVisible = FALSE;
		}
	}

	// Remove region
	MSYS_RemoveRegion( &gInvDesc);


	if( gpItemDescObject->usItem != MONEY )
	{
		if (UsingNewAttachmentSystem()==true)
		{
			// WANNE: Bugfix: Fixed CTD when closing sector inventory in strategy screen with open item description
			if (gpItemDescObject[0].objectStack.size() == 0)
			{
				for ( cnt = 0; cnt < MAX_ATTACHMENTS; cnt++)
				{
					MSYS_RemoveRegion( &gItemDescAttachmentRegions[cnt]);
				}
			}
			else
			{
				for ( cnt = 0; cnt < (INT32)(*gpItemDescObject)[0]->attachments.size(); cnt++ )
				{
					MSYS_RemoveRegion( &gItemDescAttachmentRegions[cnt]);

					//BOB : also, clean up the popup boxes
					if( gItemDescAttachmentPopupsInitialized && gItemDescAttachmentPopups[cnt] != NULL ){
						delete(gItemDescAttachmentPopups[cnt]);
						gItemDescAttachmentPopups[cnt] = NULL;
					}
				}

				giActiveAttachmentPopup = -1;
			}
		}
		else
		{
			// WANNE: Bugfix: Attachment regions were not removed when playing without NAS and when closing the description box
			for (cnt = 0; cnt < OLD_MAX_ATTACHMENTS_101; cnt++)
			{
				MSYS_RemoveRegion( &gItemDescAttachmentRegions[cnt]);
			}
		}
	}
	else
	{
		UnloadButtonImage( guiMoneyButtonImage );
		UnloadButtonImage( guiMoneyDoneButtonImage );
		for ( cnt = 0; cnt < OLD_MAX_ATTACHMENTS_101; cnt++ )
		{
			RemoveButton( guiMoneyButtonBtn[cnt] );
		}
	}

	// HEADROCK HAM 5: Instead of checking the item, check for the regions!!
	//if ( ITEM_PROS_AND_CONS( gpItemDescObject->usItem ) )
	if (&(gProsAndConsRegions[0]))
	{
		MSYS_RemoveRegion( &gProsAndConsRegions[0] );
	}
	if (&(gProsAndConsRegions[1]))
	{
		MSYS_RemoveRegion( &gProsAndConsRegions[1] );
	}

	// HEADROCK HAM 5: Instead of checking the item, check for the button!!

	//if(( ( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN ) && !Item[gpItemDescObject->usItem].rocketlauncher ) )
	if (giItemDescAmmoButton > -1)
	{
		// Remove button
		UnloadButtonImage( giItemDescAmmoButtonImages );
		RemoveButton( giItemDescAmmoButton );
	}
//	if(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
	if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
	{
	 fCharacterInfoPanelDirty=TRUE;
	 fMapPanelDirty = TRUE;
	 fTeamPanelDirty = TRUE;
	 fMapScreenBottomDirty = TRUE;
	}

	if( InKeyRingPopup() == TRUE )
	{
		DeleteKeyObject(gpItemDescObject);
		gpItemDescObject = NULL;
		fShowDescriptionFlag = FALSE;
		fInterfacePanelDirty = DIRTYLEVEL2;
		return;
	}

	fShowDescriptionFlag = FALSE;
	fInterfacePanelDirty = DIRTYLEVEL2;

	if( gpItemDescObject->usItem == MONEY )
	{
		//if there is no money remaining
		if( gRemoveMoney.uiMoneyRemaining == 0 && !gfAddingMoneyToMercFromPlayersAccount )
		{
			//get rid of the money in the slot
			gpItemDescObject->initialize();
			gpItemDescObject = NULL;
		}
	}

	if( gfAddingMoneyToMercFromPlayersAccount )
		gfAddingMoneyToMercFromPlayersAccount = FALSE;

	gfItemDescObjectIsAttachment = FALSE;
	gpItemDescObject = NULL;
	gpItemDescPrevObject = NULL;
	// HEADROCK HAM 5: This stores an attachment object while we're looking at its copy.
	gpItemDescOrigAttachmentObject = NULL;

}


void InternalBeginItemPointer( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject, INT8 bHandPos )
{
//	BOOLEAN fOk;

	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	// Copy into cursor...
	gItemPointer = *pObject;

	// Dirty interface
	fInterfacePanelDirty = DIRTYLEVEL2;
	gpItemPointer = &gItemPointer;
	gpItemPointerSoldier = pSoldier;
	gbItemPointerSrcSlot = bHandPos;
	gbItemPointerLocateGood = TRUE;

	CheckForDisabledForGiveItem( );

	EnableSMPanelButtons( FALSE, TRUE );

	gfItemPointerDifferentThanDefault = FALSE;

	// re-evaluate repairs
	gfReEvaluateEveryonesNothingToDo = TRUE;
}

void BeginItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
{
	int numToMove = 0;
	if (_KeyDown( SHIFT ))
	{
		// Remove all from soldier's slot
		numToMove = ALL_OBJECTS;
	}
	else
	{
		numToMove = 1;
	}
	pSoldier->inv[ubHandPos].MoveThisObjectTo(gTempObject, numToMove, pSoldier, ubHandPos);

	if ( gTempObject.exists() == false )
	{
		//oops, the move failed.  It might have failed because the object was force placed
		//to a slot where the ItemSizeLimit is 0, try again
		//this method won't work with LBEs in LBE pockets
		pSoldier->inv[ubHandPos].MoveThisObjectTo(gTempObject, numToMove);
	}

	if ( gTempObject.exists() == true )
	{
		InternalBeginItemPointer( pSoldier, &gTempObject, ubHandPos );
	}
}


void BeginKeyRingItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubKeyRingPosition )
{
	BOOLEAN fOk;

	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	if (_KeyDown( SHIFT ))
	{
		// Remove all from soldier's slot
		fOk = RemoveKeysFromSlot( pSoldier, ubKeyRingPosition, pSoldier->pKeyRing[ ubKeyRingPosition ].ubNumber, &gItemPointer );
	}
	else
	{
		RemoveKeyFromSlot( pSoldier, ubKeyRingPosition, &gItemPointer );
		fOk = (gItemPointer.ubNumberOfObjects == 1);
	}


	if (fOk)
	{
		// ATE: Look if we are a BLOODIED KNIFE, and change if so, making guy scream...

		// Dirty interface
		fInterfacePanelDirty = DIRTYLEVEL2;
		gpItemPointer = &gItemPointer;
		gpItemPointerSoldier = pSoldier;
		gbItemPointerSrcSlot = ubKeyRingPosition;

		if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
		{
			guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
			gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

			fMapInventoryItem=TRUE;
			MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
			MSYS_SetCurrentCursor( EXTERN_CURSOR );
		}
	}
	else
	{
		//Debug mesg
	}



	gfItemPointerDifferentThanDefault = FALSE;
}

void EndItemPointer( )
{
	if ( gpItemPointer != NULL )
	{
		gpItemPointer = NULL;
		gbItemPointerSrcSlot = NO_SLOT;
		MSYS_ChangeRegionCursor( &gSMPanelRegion , CURSOR_NORMAL );
		MSYS_SetCurrentCursor( CURSOR_NORMAL );

		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			gMoveingItem.initialize();
			SetSkiCursor( CURSOR_NORMAL );
		}
		else
		{
			EnableSMPanelButtons( TRUE , TRUE );
		}

		gbItemPointerLocateGood = FALSE;

		// re-evaluate repairs
		gfReEvaluateEveryonesNothingToDo = TRUE;
	}
}

void DrawItemFreeCursor( )
{
	//OBJECTTYPE		*gpItemPointer;
	//UINT16				usItemSnapCursor;

	// Get usIndex and then graphic for item
	guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
	gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

	MSYS_ChangeRegionCursor( &gSMPanelRegion , EXTERN_CURSOR );
	MSYS_SetCurrentCursor( EXTERN_CURSOR );
}

void HideItemTileCursor( )
{
//	RemoveTopmost( gusCurMousePos, gusItemPointer );

}

BOOLEAN SoldierCanSeeCatchComing( SOLDIERTYPE *pSoldier, INT32 sSrcGridNo )
{
	return( TRUE );
/*-
	INT32							cnt;
	INT8							bDirection, bTargetDirection;

	bTargetDirection = (INT8)GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, sSrcGridNo );

	// Look 3 directions Clockwise from what we are facing....
	bDirection = pSoldier->bDirection;

	for ( cnt = 0; cnt < 3; cnt++ )
	{
		if ( bDirection == bTargetDirection )
		{
			return( TRUE );
		}

		bDirection = gOneCDirection[ bDirection ];
	}

	// Look 3 directions CounterClockwise from what we are facing....
	bDirection = pSoldier->bDirection;

	for ( cnt = 0; cnt < 3; cnt++ )
	{
		if ( bDirection == bTargetDirection )
		{
			return( TRUE );
		}

		bDirection = gOneCCDirection[ bDirection ];
	}

	// If here, nothing good can happen!
	return( FALSE );
-*/

}

void DrawItemTileCursor( )
{
	INT32 usMapPos;
	UINT16						usIndex;
	UINT8							ubSoldierID;
	INT16							sAPCost;
	BOOLEAN						fRecalc;
	UINT32						uiCursorFlags;
	INT32 sFinalGridNo;
	UINT32						uiCursorId = CURSOR_ITEM_GOOD_THROW;
	SOLDIERTYPE				*pSoldier;
	BOOLEAN						fGiveItem = FALSE;
	INT32 sActionGridNo;
	UINT8							ubDirection;
	static UINT32			uiOldCursorId = 0;
	static UINT16			usOldMousePos = 0;
	INT16							sEndZ = 0;
	INT16							sDist;
	INT8							bLevel;

	if (GetMouseMapPos( &usMapPos) )
	{
		/*CHRISL: For some reason it's possible that gpItemPointerSoldier is not correctly set when we come into this function, but we require it to be set for 
			this function to work.  So for now, let's set it using gusUIFullTargetID.*/
		if(gpItemPointerSoldier->exists() == false)
			gpItemPointerSoldier = MercPtrs[ gusUIFullTargetID ];
		if ( gfUIFullTargetFound )
		{
			// Force mouse position to guy...
			usMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
		}

		gusCurMousePos = usMapPos;

		if( gusCurMousePos != usOldMousePos )
		{
			gfItemPointerDifferentThanDefault = FALSE;
		}

		// Save old one..
		usOldMousePos = gusCurMousePos;

		// Default to turning adjacent area gridno off....
		gfUIHandleShowMoveGrid = FALSE;

		// If we are over a talkable guy, set flag
		if ( IsValidTalkableNPCFromMouse( &ubSoldierID, TRUE, FALSE, TRUE ) )
		{
			fGiveItem = TRUE;
		}


		// WANNE: Prevent duplication of items cheat-bug:
		// You need two mercenaries. You place required item to first mercenary's inventory, 
		// then you take the item and point cursor with the item at the second mercenary. 
		// Then you click right mouse button and then left one. 
		// During the process of giving the item from the first mercenary to the second quicly move the item to the another slot of the inventory. 
		// As a result of it the item will be given to the second mercenary and the first mercenary will still have it.
		
		// OK, if different than default, change....
		//if ( gfItemPointerDifferentThanDefault )
		//{
		//	fGiveItem = !fGiveItem;
		//}


		// Get recalc and cursor flags
		fRecalc = GetMouseRecalcAndShowAPFlags( &uiCursorFlags, NULL );

		// OK, if we begin to move, reset the cursor...
		if ( uiCursorFlags & MOUSE_MOVING )
		{
			EndPhysicsTrajectoryUI( );
		}

		// Get Pyth spaces away.....
		sDist = PythSpacesAway( gpItemPointerSoldier->sGridNo, gusCurMousePos );

		// If we are here and we are not selected, select!
		// ATE Design discussion propably needed here...
		if ( gpItemPointerSoldier->ubID != gusSelectedSoldier )
		{
			SelectSoldier( gpItemPointerSoldier->ubID, FALSE, FALSE );
		}

		// ATE: if good for locate, locate to selected soldier....
		if ( gbItemPointerLocateGood )
		{
			gbItemPointerLocateGood = FALSE;
			LocateSoldier( gusSelectedSoldier, FALSE );
		}

		if ( !fGiveItem )
		{
			if ( UIHandleOnMerc( FALSE ) && usMapPos != gpItemPointerSoldier->sGridNo )
			{
				// We are on a guy.. check if they can catch or not....
				if ( gfUIFullTargetFound )
				{
					// Get soldier
					pSoldier = MercPtrs[ gusUIFullTargetID ];

					// Are they on our team?
					// ATE: Can't be an EPC
					if ( pSoldier->bTeam == gbPlayerNum && !AM_AN_EPC( pSoldier ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
					{
						if ( sDist <= PASSING_ITEM_DISTANCE_OKLIFE )
						{
							// OK, on a valid pass
							gfUIMouseOnValidCatcher = 4;
							gubUIValidCatcherID			= (UINT8)gusUIFullTargetID;
						}
						else
						{
							// Can they see the throw?
							if ( SoldierCanSeeCatchComing( pSoldier, gpItemPointerSoldier->sGridNo ) )
							{
								// OK, set global that this buddy can see catch...
								gfUIMouseOnValidCatcher = TRUE;
								gubUIValidCatcherID			= (UINT8)gusUIFullTargetID;
							}
						}
					}
				}
			}

			// We're going to toss it!
			if ( gTacticalStatus.uiFlags & INCOMBAT )
			{
				gfUIDisplayActionPoints = TRUE;
				gUIDisplayActionPointsOffX = 15;
				gUIDisplayActionPointsOffY = 15;
			}

			// If we are tossing...
			if (  sDist <= 1 && gfUIMouseOnValidCatcher == 0 || gfUIMouseOnValidCatcher == 4 )
			{
				gsCurrentActionPoints = GetBasicAPsToPickupItem( gpItemPointerSoldier );
			}
			else
			{
				gsCurrentActionPoints = APBPConstants[AP_TOSS_ITEM];
			}

		}
		else
		{
			if ( gfUIFullTargetFound )
			{
				UIHandleOnMerc( FALSE );

				// OK, set global that this buddy can see catch...
				gfUIMouseOnValidCatcher = 2;
				gubUIValidCatcherID			= (UINT8)gusUIFullTargetID;

				// If this is a robot, change to say 'reload'
				if ( MercPtrs[ gusUIFullTargetID ]->flags.uiStatusFlags & SOLDIER_ROBOT )
				{
					gfUIMouseOnValidCatcher = 3;
				}

				if ( !( uiCursorFlags & MOUSE_MOVING ) )
				{
					// Find adjacent gridno...
					sActionGridNo =  FindAdjacentGridEx( gpItemPointerSoldier, gusCurMousePos, &ubDirection, NULL, FALSE, FALSE );
					if ( sActionGridNo == -1 )
					{
						sActionGridNo = gusCurMousePos;
					}

					// Display location...
					gsUIHandleShowMoveGridLocation = sActionGridNo;
					gfUIHandleShowMoveGrid = TRUE;


					// Get AP cost
					if ( MercPtrs[ gusUIFullTargetID ]->flags.uiStatusFlags & SOLDIER_ROBOT )
					{
						sAPCost = GetAPsToReloadRobot( gpItemPointerSoldier, MercPtrs[ gusUIFullTargetID ] );
					}
					else
					{
						sAPCost = GetAPsToGiveItem( gpItemPointerSoldier, sActionGridNo );
					}

					gsCurrentActionPoints = sAPCost;
				}

				// Set value
				if ( gTacticalStatus.uiFlags & INCOMBAT )
				{
					gfUIDisplayActionPoints = TRUE;
					gUIDisplayActionPointsOffX = 15;
					gUIDisplayActionPointsOffY = 15;
				}
			}
		}


		if ( fGiveItem )
		{
			uiCursorId = CURSOR_ITEM_GIVE;
		}
		else
		{
			// How afar away are we?
			if ( sDist <= 1 && gfUIMouseOnValidCatcher == 0 )
			{
				// OK, we want to drop.....

				// Write the word 'drop' on cursor...
				wcscpy( gzIntTileLocation, pMessageStrings[ MSG_DROP ] );
				gfUIIntTileLocation = TRUE;
			}
			else
			{
				if ( usMapPos == gpItemPointerSoldier->sGridNo )
				{
					EndPhysicsTrajectoryUI( );
				}
        else if ( gfUIMouseOnValidCatcher == 4 )
        {
          // ATE: Don't do if we are passing....
        }
        else
				// ( sDist > PASSING_ITEM_DISTANCE_OKLIFE )
				{
					// Write the word 'drop' on cursor...
					if ( gfUIMouseOnValidCatcher == 0 )
					{
						wcscpy( gzIntTileLocation, pMessageStrings[ MSG_THROW ] );
						gfUIIntTileLocation = TRUE;
					}

					gfUIHandlePhysicsTrajectory = TRUE;

					if ( fRecalc && usMapPos != gpItemPointerSoldier->sGridNo )
					{
						if ( gfUIMouseOnValidCatcher )
						{
							switch( gAnimControl[ MercPtrs[ gubUIValidCatcherID ]->usAnimState ].ubHeight )
							{
								case ANIM_STAND:

									sEndZ = 150;
									break;

								case ANIM_CROUCH:

									sEndZ = 80;
									break;

								case ANIM_PRONE:

									sEndZ = 10;
									break;
							}

							if ( MercPtrs[ gubUIValidCatcherID ]->pathing.bLevel > 0 )
							{
								sEndZ = 0;
							}
						}

						// Calculate chance to throw here.....
						if ( !CalculateLaunchItemChanceToGetThrough( gpItemPointerSoldier, gpItemPointer, usMapPos, (INT8)gsInterfaceLevel, (INT16)( ( gsInterfaceLevel * 256 ) + sEndZ ), &sFinalGridNo, FALSE, &bLevel, TRUE ) )
						{
							gfBadThrowItemCTGH = TRUE;
						}
						else
						{
							gfBadThrowItemCTGH = FALSE;
						}

						BeginPhysicsTrajectoryUI( sFinalGridNo, bLevel, gfBadThrowItemCTGH );
					}
				}

				if ( gfBadThrowItemCTGH )
				{
					uiCursorId = CURSOR_ITEM_BAD_THROW;
				}
			}
		}

		//Erase any cursor in viewport
		//MSYS_ChangeRegionCursor( &gViewportRegion , VIDEO_NO_CURSOR );

		// Get tile graphic fro item
		usIndex = GetTileGraphicForItem( &(Item[ gpItemPointer->usItem ]) );

		// ONly load if different....
		if ( usIndex != gusItemPointer || uiOldCursorId != uiCursorId )
		{
			// OK, Tile database gives me subregion and video object to use...
			SetExternVOData( uiCursorId, gTileDatabase[ usIndex ].hTileSurface, gTileDatabase[ usIndex ].usRegionIndex );
			gusItemPointer = usIndex;
			uiOldCursorId = uiCursorId;
		}


		MSYS_ChangeRegionCursor( &gViewportRegion , (UINT16)uiCursorId );

	}
}


BOOLEAN IsValidAmmoToReloadRobot( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject )
{
	if ( !CompatibleAmmoForGun( pObject, &( pSoldier->inv[ HANDPOS ] ) ) )
	{
		// Build string...
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ ROBOT_NEEDS_GIVEN_CALIBER_STR ], AmmoCaliber[ Weapon[ pSoldier->inv[ HANDPOS ].usItem ].ubCalibre ] );

		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN HandleItemPointerClick( INT32 usMapPos )
{
	// Determine what to do
	if ( SelectedGuyInBusyAnimation( ) )
	{
		return( FALSE );
	}

	if ( giUIMessageOverlay != -1 )
	{
		EndUIMessage( );
		return( FALSE );
	}

	// Don't allow if our soldier is a # of things...
	if ( AM_AN_EPC( gpItemPointerSoldier ) || gpItemPointerSoldier->stats.bLife < OKLIFE || gpItemPointerSoldier->MercInDeepWater( ) )
	{
		return( FALSE );
	}

	// This implies we have no path....
	if ( gsCurrentActionPoints == 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
		return( FALSE );
	}


	UINT8 ubDirection;
	UINT8	ubSoldierID;
	UINT16	  usItem;
	INT16			sAPCost;
	SOLDIERTYPE		*pSoldier=NULL;
	UINT8			ubThrowActionCode=0;
	UINT32		uiThrowActionData=0;
	INT16			sEndZ = 0;
	BOOLEAN		fGiveItem = FALSE;
	INT32			sGridNo;
	INT16			sDist;

	if ( gfUIFullTargetFound )
	{
		// Force mouse position to guy...
		usMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;

		if ( gAnimControl[ MercPtrs[ gusUIFullTargetID ]->usAnimState ].uiFlags & ANIM_MOVING )
		{
			return( FALSE );
		}

	}

	// Check if we have APs....
	if ( !EnoughPoints( gpItemPointerSoldier, gsCurrentActionPoints, 0, TRUE ) )
	{
		if ( gfDontChargeAPsToPickup && gsCurrentActionPoints == GetBasicAPsToPickupItem( gpItemPointerSoldier ) )
		{

		}
		else
		{
			return( FALSE );
		}
	}

	// SEE IF WE ARE OVER A TALKABLE GUY!
	if ( IsValidTalkableNPCFromMouse( &ubSoldierID, TRUE, FALSE, TRUE ) )
	{
		fGiveItem = TRUE;
	}

	// WANNE: Prevent duplication of items cheat-bug:
	// You need two mercenaries. You place required item to first mercenary's inventory, 
	// then you take the item and point cursor with the item at the second mercenary. 
	// Then you click right mouse button and then left one. 
	// During the process of giving the item from the first mercenary to the second quicly move the item to the another slot of the inventory. 
	// As a result of it the item will be given to the second mercenary and the first mercenary will still have it.
	// OK, if different than default, change....
	//if ( gfItemPointerDifferentThanDefault )
	//{
	//	fGiveItem = !fGiveItem;
	//}


	// Get Pyth spaces away.....
	sDist = PythSpacesAway( gpItemPointerSoldier->sGridNo, gusCurMousePos );


	if ( fGiveItem )
	{
		usItem = gpItemPointer->usItem;

		// If the target is a robot,
		if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_ROBOT )
		{
			// Charge APs to reload robot!
			sAPCost = GetAPsToReloadRobot( gpItemPointerSoldier,  MercPtrs[ ubSoldierID ] );
		}
		else
		{
			// Calculate action point costs!
			sAPCost = GetAPsToGiveItem( gpItemPointerSoldier, usMapPos );
		}

		//CHRISL: This doesn't make sense to me.  If we take an item out of a stack and then click on something, why would we first attempt to put the item
		//	back in the stack we took it from.  The item in the cursor should be recognized as a seperate stack at this point.
		// Place it back in our hands!
		gTempObject = *gpItemPointer;

//		if ( gbItemPointerSrcSlot != NO_SLOT )
//		{
//			PlaceObject( gpItemPointerSoldier, gbItemPointerSrcSlot, gpItemPointer );
//			fInterfacePanelDirty = DIRTYLEVEL2;
//		}
/*
		//if the user just clicked on an arms dealer
		if( IsMercADealer( MercPtrs[ ubSoldierID ]->ubProfile ) )
		{
			if ( EnoughPoints( gpItemPointerSoldier, sAPCost, 0, TRUE ) )
			{
				//Enter the shopkeeper interface
				EnterShopKeeperInterfaceScreen( MercPtrs[ ubSoldierID ]->ubProfile );

				EndItemPointer( );
			}

			return( TRUE );
		}
*/

		if ( EnoughPoints( gpItemPointerSoldier, sAPCost, 0, TRUE ) )
		{
			// If we are a robot, check if this is proper item to reload!
			if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_ROBOT )
			{
				// Check if we can reload robot....
				if ( IsValidAmmoToReloadRobot( MercPtrs[ ubSoldierID ], &gTempObject ) )
				{
					 INT32 sActionGridNo;
					 UINT8	ubDirection;
					 INT32 sAdjustedGridNo;

					 // Walk up to him and reload!
					 // See if we can get there to stab
					 sActionGridNo =  FindAdjacentGridEx( gpItemPointerSoldier, MercPtrs[ ubSoldierID ]->sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

					 if ( sActionGridNo != -1 && gbItemPointerSrcSlot != NO_SLOT )
					 {
							// Make a temp object for ammo...
							OBJECTTYPE::CopyToOrCreateAt( &gpItemPointerSoldier->pTempObject, &gTempObject);

							// Remove from soldier's inv...
							gpItemPointerSoldier->inv[ gbItemPointerSrcSlot ].RemoveObjectsFromStack(1);

							gpItemPointerSoldier->aiData.sPendingActionData2  = sAdjustedGridNo;
							gpItemPointerSoldier->aiData.uiPendingActionData1 = gbItemPointerSrcSlot;
							gpItemPointerSoldier->aiData.bPendingActionData3  = ubDirection;
							gpItemPointerSoldier->aiData.ubPendingActionAnimCount = 0;

							// CHECK IF WE ARE AT THIS GRIDNO NOW
							if ( gpItemPointerSoldier->sGridNo != sActionGridNo )
							{
								// SEND PENDING ACTION
								gpItemPointerSoldier->aiData.ubPendingAction = MERC_RELOADROBOT;

								// WALK UP TO DEST FIRST
								gpItemPointerSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, gpItemPointerSoldier->usUIMovementMode, FALSE, FALSE );
							}
							else
							{
								gpItemPointerSoldier->EVENT_SoldierBeginReloadRobot( sAdjustedGridNo, ubDirection, gbItemPointerSrcSlot );
							}

							// OK, set UI
							SetUIBusy( gpItemPointerSoldier->ubID );
					 }

				}

				gfDontChargeAPsToPickup = FALSE;
				EndItemPointer( );
			}
			else
			{
				//if (gbItemPointerSrcSlot != NO_SLOT )
				{
					// Give guy this item.....
					SoldierGiveItem( gpItemPointerSoldier, MercPtrs[ ubSoldierID ], &gTempObject, gbItemPointerSrcSlot );

					gfDontChargeAPsToPickup = FALSE;
					EndItemPointer( );

					// If we are giving it to somebody not on our team....
					//if ( MercPtrs[ ubSoldierID ]->ubProfile < FIRST_RPC || MercPtrs[ubSoldierID]->ubProfile >= GASTON || RPC_RECRUITED( MercPtrs[ ubSoldierID ] ) )
					//new profiles by Jazz	
					if ( gProfilesIMP[MercPtrs[ ubSoldierID ]->ubProfile].ProfilId == MercPtrs[ ubSoldierID ]->ubProfile || gProfilesAIM[MercPtrs[ ubSoldierID ]->ubProfile].ProfilId == MercPtrs[ ubSoldierID ]->ubProfile || gProfilesMERC[MercPtrs[ ubSoldierID ]->ubProfile].ProfilId == MercPtrs[ ubSoldierID ]->ubProfile || RPC_RECRUITED( MercPtrs[ ubSoldierID ] ) )			
					{

					}
					else
					{
						SetEngagedInConvFromPCAction( gpItemPointerSoldier );
					}
				}
			}
		}

		return( TRUE );
	}

	// CHECK IF WE ARE NOT ON THE SAME GRIDNO
	if ( sDist <= 1 && !( gfUIFullTargetFound && gusUIFullTargetID != gpItemPointerSoldier->ubID ) )
	{
		// Check some things here....
		// 1 ) are we at the exact gridno that we stand on?
		if ( usMapPos == gpItemPointerSoldier->sGridNo )
		{
			// Drop
			if ( !gfDontChargeAPsToPickup )
			{
				// Deduct points
				DeductPoints( gpItemPointerSoldier, GetBasicAPsToPickupItem( gpItemPointerSoldier ), 0 );
			}

			SoldierDropItem( gpItemPointerSoldier, gpItemPointer );
		}
		else
		{
			// Try to drop in an adjacent area....
			// 1 ) is this not a good OK destination
			// this will sound strange, but this is OK......
			if ( !NewOKDestination( gpItemPointerSoldier, usMapPos, FALSE, gpItemPointerSoldier->pathing.bLevel ) || FindBestPath( gpItemPointerSoldier, usMapPos, gpItemPointerSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, 0 ) == 1 )
			{
				// Drop
				if ( !gfDontChargeAPsToPickup )
				{
					// Deduct points
					DeductPoints( gpItemPointerSoldier, GetBasicAPsToPickupItem(gpItemPointerSoldier), 0 );
				}

				// Play animation....
				// Don't show animation of dropping item, if we are not standing



				switch ( gAnimControl[ gpItemPointerSoldier->usAnimState ].ubHeight )
				{
					case ANIM_STAND:

						OBJECTTYPE::CopyToOrCreateAt( &gpItemPointerSoldier->pTempObject, gpItemPointer);
						if (gpItemPointerSoldier->pTempObject != NULL)
						{
							gpItemPointerSoldier->aiData.sPendingActionData2 = usMapPos;

	 						// Turn towards.....gridno
							gpItemPointerSoldier->EVENT_SetSoldierDesiredDirection( (INT8)GetDirectionFromGridNo( usMapPos, gpItemPointerSoldier ) );

							gpItemPointerSoldier->EVENT_InitNewSoldierAnim( DROP_ADJACENT_OBJECT, 0 , FALSE );
						}
						break;

					case ANIM_CROUCH:
					case ANIM_PRONE:

						AddItemToPool( usMapPos, gpItemPointer, 1, gpItemPointerSoldier->pathing.bLevel, 0 , -1 );
						NotifySoldiersToLookforItems( );
						break;
				}
			}
			else
			{
				// Drop in place...
				if ( !gfDontChargeAPsToPickup )
				{
					// Deduct points
					DeductPoints( gpItemPointerSoldier, GetBasicAPsToPickupItem(gpItemPointerSoldier), 0 );
				}

				SoldierDropItem( gpItemPointerSoldier, gpItemPointer );
			}
		}
	}
	else
	{
		sGridNo = usMapPos;

		// Kaiden: Vehicle Inventory change - Commented the following If test:
		//if ( sDist <= PASSING_ITEM_DISTANCE_OKLIFE && gfUIFullTargetFound && MercPtrs[ gusUIFullTargetID ]->bTeam == gbPlayerNum && !AM_AN_EPC( MercPtrs[ gusUIFullTargetID ] ) && !( MercPtrs[ gusUIFullTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE ) )

		// And added this one instead:
		if ( ( sDist <= PASSING_ITEM_DISTANCE_OKLIFE && gfUIFullTargetFound && MercPtrs[ gusUIFullTargetID ]->bTeam == gbPlayerNum && !AM_AN_EPC( MercPtrs[ gusUIFullTargetID ] ) ) && !( (!gGameExternalOptions.fVehicleInventory) && (MercPtrs[ gusUIFullTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE) ) )
		{
			// OK, do the transfer...
			{
				pSoldier = MercPtrs[ gusUIFullTargetID ];

				{
					// Change to inventory....
					//gfSwitchPanel = TRUE;
					//gbNewPanel = SM_PANEL;
					//gubNewPanelParam = (UINT8)pSoldier->ubID;
					if ( !EnoughPoints( pSoldier, 3, 0, TRUE ) ||
							 !EnoughPoints( gpItemPointerSoldier, 3, 0, TRUE ) )
					{
						return( FALSE );
					}

					// Check LOS....
					if ( !SoldierTo3DLocationLineOfSightTest( pSoldier, gpItemPointerSoldier->sGridNo,  gpItemPointerSoldier->pathing.bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
					{
						return( FALSE );
					}

					// Charge AP values...
					DeductPoints( pSoldier, 3, 0, UNTRIGGERED_INTERRUPT );
					DeductPoints( gpItemPointerSoldier, 3, 0, UNTRIGGERED_INTERRUPT );

					usItem = gpItemPointer->usItem;

					// try to auto place object....
					if ( AutoPlaceObject( pSoldier, gpItemPointer, TRUE ) )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_ITEM_PASSED_TO_MERC ], ShortItemNames[ usItem ], pSoldier->name );

						// Check if it's the same now!
						if ( gpItemPointer->exists() == false )
						{
							EndItemPointer( );
						}

	    			// OK, make guys turn towards each other and do animation...
			  		{
						  UINT8	ubFacingDirection;

						  // Get direction to face.....
						  ubFacingDirection = (UINT8)GetDirectionFromGridNo( gpItemPointerSoldier->sGridNo, pSoldier );

						  // Stop merc first....
						  pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );

						  // WANNE: Also turn merc if he is crouched and he passes an item
						  if ( !pSoldier->MercInWater( ) )
						  {
							  // Turn to face, then do animation....
							  pSoldier->EVENT_SetSoldierDesiredDirection( ubFacingDirection );
							  pSoldier->flags.fTurningUntilDone	 = TRUE;

							 if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
							 {
							  pSoldier->usPendingAnimation = PASS_OBJECT;
              }
						 }

						  // WANNE: Also turn merc if he is crouched and he received the passed item
						  if ( !gpItemPointerSoldier->MercInWater(  ) )
						  {
							  gpItemPointerSoldier->EVENT_SetSoldierDesiredDirection( gOppositeDirection[ ubFacingDirection ] );
							  gpItemPointerSoldier->flags.fTurningUntilDone	 = TRUE;

							  if (gAnimControl[ gpItemPointerSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
							  {
							  gpItemPointerSoldier->usPendingAnimation = PASS_OBJECT;
						  }
					  }
					  }

						return( TRUE );
					}
					else
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_NO_ROOM_TO_PASS_ITEM ], ShortItemNames[ usItem ], pSoldier->name );
  					return( FALSE );
					}
				}
			}
		}
		else
		{
			// CHECK FOR VALID CTGH
			if ( gfBadThrowItemCTGH )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANNOT_THROW_TO_DEST_STR ] );
				return( FALSE );
			}

			// Deduct points
			//DeductPoints( gpItemPointerSoldier, APBPConstants[AP_TOSS_ITEM], 0 );
			gpItemPointerSoldier->flags.fDontChargeTurningAPs = TRUE;
			// Will be dome later....

			ubThrowActionCode = NO_THROW_ACTION;

			// OK, CHECK FOR VALID THROW/CATCH
			// IF OVER OUR GUY...
			if ( gfUIFullTargetFound )
			{
				pSoldier = MercPtrs[ gusUIFullTargetID ];

				// Kaiden: Vehicle Inventory change - Commented the following If Test:
				//if ( pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE && !AM_AN_EPC( pSoldier ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )

				// And replaced it with this one:
				if ( ( pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE && !AM_AN_EPC( pSoldier ) ) && !( ( !gGameExternalOptions.fVehicleInventory ) &&  ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ) )
				{
					// OK, on our team,

					// How's our direction?
					if ( SoldierCanSeeCatchComing( pSoldier, gpItemPointerSoldier->sGridNo ) )
					{
						// Setup as being the catch target
						ubThrowActionCode = THROW_TARGET_MERC_CATCH;
						uiThrowActionData = pSoldier->ubID;

						sGridNo = pSoldier->sGridNo;

						switch( gAnimControl[ pSoldier->usAnimState ].ubHeight )
						{
								case ANIM_STAND:

									sEndZ = 150;
									break;

								case ANIM_CROUCH:

									sEndZ = 80;
									break;

								case ANIM_PRONE:

									sEndZ = 10;
									break;
						}

						if ( pSoldier->pathing.bLevel > 0 )
						{
							sEndZ = 0;
						}

						// Get direction
						ubDirection = (UINT8)GetDirectionFromGridNo( gpItemPointerSoldier->sGridNo, pSoldier );

						// ATE: Goto stationary...
						pSoldier->SoldierGotoStationaryStance( );

						// Set direction to turn...
						pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );

					}
				}
			}


			// CHANGE DIRECTION AT LEAST
			ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, gpItemPointerSoldier );
			gpItemPointerSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
			gpItemPointerSoldier->flags.fTurningUntilDone = TRUE;

			// Increment attacker count...
			// gTacticalStatus.ubAttackBusyCount++;
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incremtning ABC: Throw item to %d", gTacticalStatus.ubAttackBusyCount) );
			DebugAttackBusy( "Incrementing ABC: Throw item\n" );


			// Given our gridno, throw grenate!
			CalculateLaunchItemParamsForThrow( gpItemPointerSoldier, sGridNo, gpItemPointerSoldier->pathing.bLevel, (INT16)( ( gsInterfaceLevel * 256 ) + sEndZ ), gpItemPointer, 0, ubThrowActionCode, uiThrowActionData );

			// OK, goto throw animation
			HandleSoldierThrowItem( gpItemPointerSoldier, usMapPos );
		}
	}

	gfDontChargeAPsToPickup = FALSE;
	EndItemPointer( );


	return( TRUE );
}

BOOLEAN ItemCursorInLobRange( INT32 usMapPos )
{
	// Draw item depending on distance from buddy
	if ( GetRangeFromGridNoDiff( usMapPos, gpItemPointerSoldier->sGridNo ) > MIN_LOB_RANGE )
	{
		return( FALSE );
	}
	else
	{
		return( TRUE );
	}
}



BOOLEAN InSectorStackPopup( )
{
	return( gfInSectorStackPopup );
}

BOOLEAN InItemStackPopup( )
{
	return( gfInItemStackPopup );
}

BOOLEAN InKeyRingPopup( )
{
	return( gfInKeyRingPopup );
}

BOOLEAN InitSectorStackPopup( SOLDIERTYPE *pSoldier, WORLDITEM *pInventoryPoolList, INT32 ubPosition, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight )
{
	VOBJECT_DESC    VObjectDesc;
	SGPRect			aRect;
	ETRLEObject		*pTrav;
	HVOBJECT		hVObject;
	INT32			cnt;
	UINT16			usPopupWidth, usPopupHeight;
	INT16			sOffSetY = 0, sOffSetX = 0;
	INT16			sItemWidth = 0;
	static CHAR16	pStr[ 512 ];

	sItemWidth				= MAP_INV_ITEM_ROW_WIDTH - 1;
	sOffSetY				= 120;

	// Set some globals
	guiCurrentItemDescriptionScreen = guiCurrentScreen;
	gsItemPopupInvX			= sInvX;
	gsItemPopupInvY			= sInvY;
	gsItemPopupInvWidth		= sInvWidth;
	gsItemPopupInvHeight	= sInvHeight;
	// Set soldier
	gpItemPopupSoldier		= pSoldier;
	// Determine # of items
	gpItemPopupObject		= &(pInventoryPoolList->object );
	gubNumItemPopups		= ItemSlotLimit( gpItemPopupObject, STACK_SIZE_LIMIT );

	// Return false if #objects not >1
	if ( gubNumItemPopups <1 )
		return( FALSE );

	// Load graphics
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\extra_inventory.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiItemPopupBoxes) );

	// Get size
	GetVideoObject( &hVObject, guiItemPopupBoxes );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usPopupWidth = 75;
	//usPopupWidth = pTrav->usWidth;
	usPopupHeight = pTrav->usHeight;

	// Get Width, Height
	gsItemPopupWidth		= gubNumItemPopups * usPopupWidth;
	gsItemPopupHeight		= pTrav->usHeight;
	gsItemPopupX			= gsItemPopupInvX;
	gsItemPopupY			= gsItemPopupInvY;

	for ( cnt = 0; cnt < gubNumItemPopups; cnt++ )
	{
		// Build a mouse region here that is over any others.....
		MSYS_DefineRegion( &gItemPopupRegions[cnt],
				(INT16)( gsItemPopupInvX + ( cnt % sItemWidth * usPopupWidth ) + sOffSetX ), // top left
				(INT16)( sInvY + sOffSetY +( cnt / sItemWidth * usPopupHeight ) ), // top right
				(INT16)( gsItemPopupInvX + ( ( cnt % sItemWidth ) + 1 ) * usPopupWidth + sOffSetX ), // bottom left
				(INT16)( sInvY + ( (cnt / sItemWidth + 1) * usPopupHeight ) + sOffSetY ), // bottom right
				MSYS_PRIORITY_HIGHEST,
				MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemPopupRegionCallback );
		// Add region
		MSYS_AddRegion( &gItemPopupRegions[cnt]);
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 0, cnt );
		//CHRISL: Include the pockets capacity as UserData 1
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 1, gubNumItemPopups );
		//CHRISL: Let's also include the ubID for this merc as UserData so we can find the merc again if needed
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 2, pSoldier->ubID);
		// Flag this as a sectory item
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 3, -1);
		
		//OK, for each item, set dirty text if applicable!
		//CHRISL:
		if(cnt < gpItemPopupObject->ubNumberOfObjects && gpItemPopupObject->exists() == true){
			GetHelpTextForItem( pStr, gpItemPopupObject, 0, cnt );
			SetRegionFastHelpText( &(gItemPopupRegions[ cnt ]), pStr );
		}
		else{
			SetRegionFastHelpText( &(gItemPopupRegions[ cnt ]), ItemNames[ gpItemPopupObject->usItem ] );
		}
		SetRegionHelpEndCallback( &(gItemPopupRegions[ cnt ]), HelpTextDoneCallback );
		gfItemPopupRegionCallbackEndFix = FALSE;
	}


	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &gItemPopupRegion, sInvX, sInvY ,(INT16)(sInvX + sInvWidth), (INT16)(sInvY + sInvHeight), MSYS_PRIORITY_HIGH,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemPopupFullRegionCallback );
	// Add region
	MSYS_AddRegion( &gItemPopupRegion);

	//Disable all faces
	SetAllAutoFacesInactive( );

	fInterfacePanelDirty = DIRTYLEVEL2;

	gfInSectorStackPopup = TRUE;
	fShowInventoryFlag = TRUE;

	//Reserict mouse cursor to panel
	aRect.iLeft = sInvX + sOffSetX;
	aRect.iTop = sInvY + sOffSetY;
	aRect.iRight = aRect.iLeft + sItemWidth * usPopupWidth;
	aRect.iBottom = aRect.iTop + (INT32)(ceil((float)cnt/(float)sItemWidth)+1) * usPopupHeight;

	RestrictMouseCursor( &aRect );

	return( TRUE );
}

BOOLEAN InitItemStackPopup( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight )
{
	VOBJECT_DESC    VObjectDesc;
	SGPRect			aRect;
	ETRLEObject		*pTrav;
	HVOBJECT		hVObject;
	INT32			cnt;
	UINT16			usPopupWidth, usPopupHeight;
	INT16			sOffSetY = 0, sOffSetX = 0;
	INT16			sItemWidth = 0;
	static CHAR16	pStr[ 512 ];

	//sOffSetX = xResOffset;

	RenderBackpackButtons(DEACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	if( guiCurrentScreen == MAP_SCREEN )
	{
		sItemWidth						= MAP_INV_ITEM_ROW_WIDTH;
		sOffSetY						= 120;
	}
	else
	{
		sItemWidth						= INV_ITEM_ROW_WIDTH;
	}

	// Set some globals
	gsItemPopupInvX					= sInvX + xResOffset;
	gsItemPopupInvY					= sInvY;
	gsItemPopupInvWidth				= sInvWidth;
	gsItemPopupInvHeight			= sInvHeight;

	gpItemPopupSoldier = pSoldier;

	// Determine # of items
	gpItemPopupObject = &(pSoldier->inv[ ubPosition ] );
	// CHRISL:
	gubNumItemPopups = ItemSlotLimit( gpItemPopupObject, ubPosition, pSoldier );

	// Return false if #objects not >1
	if ( gubNumItemPopups <1 )
	{
		return( FALSE );
	}

	// Load graphics
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\extra_inventory.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiItemPopupBoxes) );

	//if(UsingNewInventorySystem() == true)
	//{
	//	/*VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	//	strcpy( VObjectDesc.ImageFile, "INTERFACE\\infobox_background.sti" );
	//	CHECKF( AddVideoObject( &VObjectDesc, &guiItemDescBoxBackground) );*/
	//}
	//else
	//	guiItemDescBoxBackground = 0;

	// Get size
	GetVideoObject( &hVObject, guiItemPopupBoxes );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usPopupWidth = pTrav->usWidth;
	usPopupHeight = pTrav->usHeight;

	//CHRISL: resize usPopupWidth based on popup stack location
	if(UsingNewInventorySystem() == true)
	{
		if((ubPosition >=BIGPOCKSTART && ubPosition < BIGPOCKFINAL) || (gGameExternalOptions.fVehicleInventory && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)))
		{
			usPopupWidth = 75;
			if(guiCurrentScreen != MAP_SCREEN)
				sItemWidth -= 2;
			else if(guiCurrentScreen == MAP_SCREEN)
				sItemWidth -= 1;
		}
		else if(ubPosition >=MEDPOCKSTART && ubPosition < MEDPOCKFINAL)
		{
			usPopupWidth = 58;
			if(guiCurrentScreen != MAP_SCREEN)
				sItemWidth -= 1;
		}
	}

	// Determine position, height and width of mouse region, area
	//GetSlotInvXY( ubPosition, &sX, &sY );
	//GetSlotInvHeightWidth( ubPosition, &sItemSlotWidth, &sItemSlotHeight );

	// Get Width, Height
	gsItemPopupWidth	= gubNumItemPopups * usPopupWidth;
	gsItemPopupHeight = pTrav->usHeight;

	gsItemPopupX	= gsItemPopupInvX;
	gsItemPopupY	= gsItemPopupInvY;

	for ( cnt = 0; cnt < gubNumItemPopups; cnt++ )
	{
//				(INT16)( gsItemPopupInvX + ( cnt % gsItemPopupInvWidth * usPopupWidth ) + sOffSetX ), // top left
//				(INT16)( sInvY + sOffSetY +( cnt / gsItemPopupInvWidth * usPopupHeight ) ), // top right
//				(INT16)( gsItemPopupInvX + ( ( cnt % gsItemPopupInvWidth ) + 1 ) * usPopupWidth + sOffSetX ), // bottom left
//				(INT16)( sInvY + ( (cnt / gsItemPopupInvWidth + 1) * usPopupHeight ) + sOffSetY ), // bottom right
//		sX = (INT16)(gsItemPopupX + ( cnt % sItemWidth * usWidth ));
//		sY = (INT16)(gsItemPopupY + sOffSetY + ( cnt / sItemWidth * usHeight ));
		// Build a mouse region here that is over any others.....
		MSYS_DefineRegion( &gItemPopupRegions[cnt],
				(INT16)( gsItemPopupInvX + ( cnt % sItemWidth * usPopupWidth ) + sOffSetX ), // top left
				(INT16)( sInvY + sOffSetY +( cnt / sItemWidth * usPopupHeight ) ), // top right
				(INT16)( gsItemPopupInvX + ( ( cnt % sItemWidth ) + 1 ) * usPopupWidth + sOffSetX ), // bottom left
				(INT16)( sInvY + ( (cnt / sItemWidth + 1) * usPopupHeight ) + sOffSetY ), // bottom right
				MSYS_PRIORITY_HIGHEST,
				MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemPopupRegionCallback );
		// Add region
		MSYS_AddRegion( &gItemPopupRegions[cnt]);
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 0, cnt );
		//CHRISL: Include the pockets capacity as UserData 1
		int cap = ItemSlotLimit( &pSoldier->inv[ubPosition], ubPosition, pSoldier );
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 1, cap );
		//CHRISL: Let's also include the ubID for this merc as UserData so we can find the merc again if needed
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 2, pSoldier->ubID);
		//CHRISL: Also include the pocket we're looking at so we can display the right graphic
		MSYS_SetRegionUserData( &gItemPopupRegions[cnt], 3, ubPosition);
		
		//OK, for each item, set dirty text if applicable!
		//CHRISL:
		if(cnt < pSoldier->inv[ubPosition].ubNumberOfObjects && pSoldier->inv[ubPosition].exists() == true){
			GetHelpTextForItem( pStr, &( pSoldier->inv[ ubPosition ] ), pSoldier, cnt );
			SetRegionFastHelpText( &(gItemPopupRegions[ cnt ]), pStr );
		}
		else{
			SetRegionFastHelpText( &(gItemPopupRegions[ cnt ]), ItemNames[ pSoldier->inv[ ubPosition ].usItem ] );
		}
		SetRegionHelpEndCallback( &(gItemPopupRegions[ cnt ]), HelpTextDoneCallback );
		gfItemPopupRegionCallbackEndFix = FALSE;
	}


	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &gItemPopupRegion, sInvX, sInvY ,(INT16)(sInvX + sInvWidth), (INT16)(sInvY + sInvHeight), MSYS_PRIORITY_HIGH,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemPopupFullRegionCallback );
	// Add region
	MSYS_AddRegion( &gItemPopupRegion);


	//Disable all faces
	SetAllAutoFacesInactive( );


	fInterfacePanelDirty = DIRTYLEVEL2;

	//guiTacticalInterfaceFlags |= INTERFACE_NORENDERBUTTONS;


//	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	if( guiCurrentItemDescriptionScreen != MAP_SCREEN )
	{
		EnableSMPanelButtons( FALSE, FALSE );
	}

	gfInItemStackPopup = TRUE;

	//Reserict mouse cursor to panel
	aRect.iLeft = sInvX + xResOffset;
	aRect.iTop = sInvY + sOffSetY;
	aRect.iRight = aRect.iLeft + sItemWidth * usPopupWidth;
	aRect.iBottom = aRect.iTop + (INT32)(ceil((float)cnt/(float)sItemWidth)+1) * usPopupHeight;
	//aRect.iTop = sInvY;
	//aRect.iLeft = sInvX;
	//aRect.iBottom = sInvY + sInvHeight;
	//aRect.iRight = sInvX + sInvWidth;

	RestrictMouseCursor( &aRect );

	return( TRUE );
}

void EndItemStackPopupWithItemInHand( )
{
	if ( gpItemPointer != NULL )
	{
		DeleteItemStackPopup( );
	}
}

void ShadowNIVPanel()
{
	// WANNE: Instead of blitting the sti overlap interface, shade the inventory. Looks much better and is the same than in OIV
	UINT16 startX1, startX2, startX3;
	UINT16 startY1, startY2, startY3;
	UINT16 endX1, endX2, endX3;
	UINT16 endY1, endY2, endY3;
	
	// First rectangle
	startX1 = gsItemPopupInvX;
	startY1 = gsItemPopupInvY;
	endX1 = SCREEN_WIDTH - xResOffset;
	endY1 = startY1 + 109;
	ShadowVideoSurfaceRect( FRAME_BUFFER, startX1, startY1, endX1 , endY1  );

	// Second rectangle
	startX2 = startX1;
	startY2 = endY1;
	endX2 = SCREEN_WIDTH - xResOffset - 155;
	endY2 = SCREEN_HEIGHT;
	ShadowVideoSurfaceRect( FRAME_BUFFER, startX2, startY2, endX2 , endY2  );

	// Third rectangle
	startX3 = startX1 - 140;
	startY3 = startY1 + 65;
	endX3 = startX3 + 140;
	endY3 = SCREEN_HEIGHT;
	ShadowVideoSurfaceRect( FRAME_BUFFER, startX3, startY3, endX3 , endY3  );
}

void RenderItemStackPopup( BOOLEAN fFullRender )
{
	ETRLEObject						*pTrav;
	UINT32								usHeight, usWidth;
	HVOBJECT							hVObject;
	UINT32								cnt;
	INT16									sX, sY, sNewX, sNewY;
	INT16			sItemWidth = 0, sOffSetY = 0, sWidth = 29;
	INT8			ubPosition;
	UINT8			image = 0;
	int				sID;

	// CHRISL: Setup witdh and offset to layer inventory boxes if necessary
	if( guiCurrentScreen == MAP_SCREEN )
	{
		sItemWidth						= MAP_INV_ITEM_ROW_WIDTH;
		sOffSetY						= 120;
	}
	else
	{
		sItemWidth						= INV_ITEM_ROW_WIDTH;
	}

	if ( gfInItemStackPopup )
	{

		//Disable all faces
		SetAllAutoFacesInactive( );

		// Shadow Area
		if ( fFullRender )
		{
			if(UsingNewInventorySystem() == false || guiCurrentScreen == MAP_SCREEN)
				ShadowVideoSurfaceRect( FRAME_BUFFER, gsItemPopupInvX, gsItemPopupInvY, gsItemPopupInvX + gsItemPopupInvWidth , gsItemPopupInvY + gsItemPopupInvHeight  );
			else if(UsingNewInventorySystem() == true && iResolution >= _800x600 /* && guiItemDescBoxBackground != 0 */ && guiCurrentScreen != MAP_SCREEN)
			{
				ShadowNIVPanel();
				

				/*
				if(iResolution >= _640x480 && iResolution < _800x600)
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 0, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				else if (iResolution < _1024x768)
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 1, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				else
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 2, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );				
				*/
			}
		}

	}
	//CHRISL: Get ubPosition from mouse region
	sID = MSYS_GetRegionUserData(&gItemPopupRegions[0], 2);
	ubPosition = MSYS_GetRegionUserData(&gItemPopupRegions[0], 3);
	// TAKE A LOOK AT THE VIDEO OBJECT SIZE ( ONE OF TWO SIZES ) AND CENTER!
	GetVideoObject( &hVObject, guiItemPopupBoxes );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;

	//CHRISL: resize usPopupWidth based on popup stack location
	if(UsingNewInventorySystem() == true || ubPosition == -1)
	{
		if(ubPosition == -1 || (ubPosition >=BIGPOCKSTART && ubPosition < BIGPOCKFINAL) || (gGameExternalOptions.fVehicleInventory && (MercPtrs[sID]->flags.uiStatusFlags & SOLDIER_VEHICLE)))
		{
			if(guiCurrentScreen != MAP_SCREEN)
				sItemWidth -= 2;
			else if(guiCurrentScreen == MAP_SCREEN)
				sItemWidth -= 1;
			usWidth = 75;
			sWidth = 60;
			image = 2;
		}
		else if(ubPosition >=MEDPOCKSTART && ubPosition < MEDPOCKFINAL)
		{
			if(guiCurrentScreen != MAP_SCREEN)
				sItemWidth -= 1;
			usWidth = 58;
			sWidth = 43;
			image = 1;	
		}
	}

	for ( cnt = 0; cnt < gubNumItemPopups; cnt++ )
	{
		// CHRISL: Layer item boxes if we have too many to display in a single row
		//BltVideoObjectFromIndex( FRAME_BUFFER, guiItemPopupBoxes, 0, gsItemPopupX + ( cnt * usWidth ), gsItemPopupY, VO_BLT_SRCTRANSPARENCY, NULL );
		sX = (INT16)(gsItemPopupX + ( cnt % sItemWidth * usWidth ));
		sY = (INT16)(gsItemPopupY + sOffSetY + ( cnt / sItemWidth * usHeight ));
		BltVideoObjectFromIndex( FRAME_BUFFER, guiItemPopupBoxes, image, (INT16)sX, ( INT16 )sY, VO_BLT_SRCTRANSPARENCY, NULL );
	
		if ( cnt < gpItemPopupObject->ubNumberOfObjects )
		{	
			// CHRISL: Coord updates to work with mutliple rows
			//sX = (INT16)(gsItemPopupX + ( cnt * usWidth ) + 11);
			//sY = (INT16)( gsItemPopupY + 3 );

			INVRenderItem( FRAME_BUFFER, NULL, gpItemPopupObject, sX+11, sY+3, sWidth, 23, DIRTYLEVEL2, NULL, (UINT8)RENDER_ITEM_NOSTATUS, FALSE, 0, cnt );

			// CHRISL: Coord updates to work with mutliple rows
			// Do status bar here...
			sNewX = (INT16)( gsItemPopupX + ( cnt * usWidth ) + 7 );
			sNewY = gsItemPopupY + INV_BAR_DY + 3;
			DrawItemUIBarEx( gpItemPopupObject, (UINT8)cnt, sX+7, sY+INV_BAR_DY+3, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT, Get16BPPColor( STATUS_BAR ), Get16BPPColor( STATUS_BAR_SHADOW ), TRUE , FRAME_BUFFER, cnt );

		}
	}

	//RestoreExternBackgroundRect( gsItemPopupInvX, gsItemPopupInvY, gsItemPopupInvWidth, gsItemPopupInvHeight );
	InvalidateRegion( gsItemPopupInvX, INV_REGION_Y, gsItemPopupInvX + gsItemPopupInvWidth, INV_REGION_Y + gsItemPopupInvHeight );

}

void HandleItemStackPopup( )
{

}


void DeleteItemStackPopup( )
{
	INT32			cnt;

	//CHRISL: if neither item or sector stack popups are open, just return.
	if(!gfInItemStackPopup && !gfInSectorStackPopup)
		return;
	//Remove
	DeleteVideoObjectFromIndex( guiItemPopupBoxes );

	MSYS_RemoveRegion( &gItemPopupRegion);


	gfInItemStackPopup = FALSE;
	gfInSectorStackPopup = FALSE;

	for ( cnt = 0; cnt < gubNumItemPopups; cnt++ )
	{
		MSYS_RemoveRegion( &gItemPopupRegions[cnt]);
	}


	fInterfacePanelDirty = DIRTYLEVEL2;

	fMapScreenBottomDirty = TRUE;

	//guiTacticalInterfaceFlags &= (~INTERFACE_NORENDERBUTTONS);

//	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	if( guiCurrentItemDescriptionScreen != MAP_SCREEN )
	{
		EnableSMPanelButtons( TRUE, FALSE );
	}

	FreeMouseCursor( );

}


BOOLEAN InitKeyRingPopup( SOLDIERTYPE *pSoldier, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight )
{
	VOBJECT_DESC    VObjectDesc;
	SGPRect			aRect;
	ETRLEObject		*pTrav;
	HVOBJECT		hVObject;
	INT32			cnt;
	UINT16			usPopupWidth, usPopupHeight;
	INT16			sOffSetY = 0, sOffSetX = 0;
	INT16			sKeyRingItemWidth = 0;

	RenderBackpackButtons(DEACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	if( guiCurrentScreen == MAP_SCREEN )
	{
		gsKeyRingPopupInvX	= xResOffset;
		sKeyRingItemWidth	= MAP_KEY_RING_ROW_WIDTH;
		sOffSetX = 40;
		sOffSetY = 15;
	}
	else
	{
		// Set some globals	
		gsKeyRingPopupInvX				= xResOffset + sInvX + TACTICAL_INVENTORY_KEYRING_GRAPHIC_OFFSET_X;
		sKeyRingItemWidth				= KEY_RING_ROW_WIDTH;
		sOffSetY = 8;
	}

	gsKeyRingPopupInvY					= sInvY;
	gsKeyRingPopupInvWidth				= sInvWidth;
	gsKeyRingPopupInvHeight				= sInvHeight;

	gpItemPopupSoldier = pSoldier;

	// Load graphics
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\extra_inventory.STI" );
	CHECKF( AddVideoObject( &VObjectDesc, &guiItemPopupBoxes) );

	if(UsingNewInventorySystem() == true)
	{
		/*VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\infobox_background.sti" );
		CHECKF( AddVideoObject( &VObjectDesc, &guiItemDescBoxBackground) );*/
	}
	else
		guiItemDescBoxBackground = 0;

	// Get size
	GetVideoObject( &hVObject, guiItemPopupBoxes );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usPopupWidth = pTrav->usWidth;
	usPopupHeight = pTrav->usHeight;

	// Determine position, height and width of mouse region, area
	//GetSlotInvHeightWidth( ubSlotSimilarToKeySlot, &sItemSlotWidth, &sItemSlotHeight );

	for ( cnt = 0; cnt < NUMBER_KEYS_ON_KEYRING; cnt++ )
	{
		// Build a mouse region here that is over any others.....
		MSYS_DefineRegion( &gKeyRingRegions[cnt],
				(INT16)( gsKeyRingPopupInvX + ( cnt % sKeyRingItemWidth * usPopupWidth ) + sOffSetX ), // top left
				(INT16)( sInvY + sOffSetY +( cnt / sKeyRingItemWidth * usPopupHeight ) ), // top right
				(INT16)( gsKeyRingPopupInvX + ( ( cnt % sKeyRingItemWidth ) + 1 ) * usPopupWidth + sOffSetX ), // bottom left
				(INT16)( sInvY + ( (cnt / sKeyRingItemWidth + 1) * usPopupHeight ) + sOffSetY ), // bottom right
				MSYS_PRIORITY_HIGHEST,
				MSYS_NO_CURSOR, MSYS_NO_CALLBACK, KeyRingSlotInvClickCallback );
		// Add region
		MSYS_AddRegion( &gKeyRingRegions[cnt]);
		MSYS_SetRegionUserData( &gKeyRingRegions[cnt], 0, cnt );
		//gfItemPopupRegionCallbackEndFix = FALSE;
	}


	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &gItemPopupRegion, sInvX, sInvY ,(INT16)(sInvX + sInvWidth), (INT16)(sInvY + sInvHeight), MSYS_PRIORITY_HIGH,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, ItemPopupFullRegionCallback );

	// Add region
	MSYS_AddRegion( &gItemPopupRegion);


	//Disable all faces
	SetAllAutoFacesInactive( );


	fInterfacePanelDirty = DIRTYLEVEL2;

	//guiTacticalInterfaceFlags |= INTERFACE_NORENDERBUTTONS;


//	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	if( guiCurrentItemDescriptionScreen != MAP_SCREEN )
	{
		EnableSMPanelButtons( FALSE , FALSE );
	}

	gfInKeyRingPopup = TRUE;

	//Reserict mouse cursor to panel
	aRect.iLeft = gsKeyRingPopupInvX + sOffSetX;
	aRect.iTop = sInvY + sOffSetY;
	aRect.iRight = aRect.iLeft + sKeyRingItemWidth * usPopupWidth;
	aRect.iBottom = aRect.iTop + (INT32)(ceil((float)cnt/(float)sKeyRingItemWidth)+1) * usPopupHeight;

	RestrictMouseCursor( &aRect );

	return( TRUE );
}


void RenderKeyRingPopup( BOOLEAN fFullRender )
{
  ETRLEObject						*pTrav;
	UINT32								usHeight, usWidth;
	HVOBJECT							hVObject;
	UINT32								cnt;
	INT16 sKeyRingItemWidth = 0;
	INT16 sOffSetY = 0, sOffSetX = 0;

	if( guiCurrentScreen != MAP_SCREEN )
	{
		sOffSetY = 8;
	}
	else
	{
		sOffSetX = 40;
		sOffSetY = 15;
	}

	if ( gfInKeyRingPopup )
	{

		//Disable all faces
		SetAllAutoFacesInactive( );

		// Shadow Area
		if ( fFullRender )
		{
			if(UsingNewInventorySystem() == false || guiCurrentScreen == MAP_SCREEN)
				ShadowVideoSurfaceRect( FRAME_BUFFER, 0, gsKeyRingPopupInvY, gsKeyRingPopupInvX + gsKeyRingPopupInvWidth , gsKeyRingPopupInvY + gsKeyRingPopupInvHeight );
			else if(UsingNewInventorySystem() == true && iResolution >= _800x600 /* && guiItemDescBoxBackground != 0 */ && guiCurrentScreen != MAP_SCREEN)
			{
				ShadowNIVPanel();

				/*
				if (iResolution >= _640x480 && iResolution < _800x600)
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 0, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				else if (iResolution < _1024x768)
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 1, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );		
				else	
					BltVideoObjectFromIndex( FRAME_BUFFER, guiItemDescBoxBackground, 2, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				*/
			}
		}
	}

	gTempObject.usItem = KEY_1;
	gTempObject[0]->data.objectStatus = 100;

	// TAKE A LOOK AT THE VIDEO OBJECT SIZE ( ONE OF TWO SIZES ) AND CENTER!
	GetVideoObject( &hVObject, guiItemPopupBoxes );
	pTrav = &(hVObject->pETRLEObject[ 0 ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;

	if( guiCurrentScreen == MAP_SCREEN )
	{
		sKeyRingItemWidth						= MAP_KEY_RING_ROW_WIDTH;
	}
	else
	{
		// Set some globals
		sKeyRingItemWidth						= KEY_RING_ROW_WIDTH;
	}

	for ( cnt = 0; cnt < NUMBER_KEYS_ON_KEYRING; cnt++ )
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, guiItemPopupBoxes, 0, (INT16)(gsKeyRingPopupInvX + ( cnt % sKeyRingItemWidth * usWidth ) + sOffSetX ), ( INT16 )( gsKeyRingPopupInvY + sOffSetY + ( cnt / sKeyRingItemWidth * usHeight ) ), VO_BLT_SRCTRANSPARENCY, NULL );

		// will want to draw key here.. if there is one
		if( ( gpItemPopupSoldier->pKeyRing[ cnt ].ubKeyID != INVALID_KEY_NUMBER ) && ( gpItemPopupSoldier->pKeyRing[ cnt ].ubNumber > 0 ) )
		{
			gTempObject.ubNumberOfObjects = gpItemPopupSoldier->pKeyRing[ cnt ].ubNumber;

			// show 100% status for each
			DrawItemUIBarEx( &gTempObject, 0, (INT16)( gsKeyRingPopupInvX + sOffSetX + ( cnt % sKeyRingItemWidth * usWidth ) + 7 ), ( INT16 )( gsKeyRingPopupInvY + sOffSetY + ( cnt / sKeyRingItemWidth * usHeight ) + 24 )
			, ITEM_BAR_WIDTH, ITEM_BAR_HEIGHT, 	Get16BPPColor( STATUS_BAR ), Get16BPPColor( STATUS_BAR_SHADOW ), TRUE , FRAME_BUFFER );

			// set item type
			gTempObject.usItem = FIRST_KEY + LockTable[ gpItemPopupSoldier->pKeyRing[ cnt].ubKeyID ].usKeyItem;

			// render the item
			INVRenderItem( FRAME_BUFFER, NULL, &gTempObject, (INT16)(gsKeyRingPopupInvX + sOffSetX +( cnt % sKeyRingItemWidth * usWidth ) + 8), ( INT16 )( gsKeyRingPopupInvY + sOffSetY + ( cnt / sKeyRingItemWidth * usHeight ) ),
				( UINT16 )( usWidth - 8 ), ( UINT16 )( usHeight - 2 ) , DIRTYLEVEL2,  NULL, 0, 0, 0 );
		}

		//BltVideoObjectFromIndex( FRAME_BUFFER, guiItemPopupBoxes, 0, (INT16)(gsKeyRingPopupInvX + ( cnt % KEY_RING_ROW_WIDTH * usWidth ) ), ( INT16 )( gsKeyRingPopupInvY + ( cnt / KEY_RING_ROW_WIDTH * usHeight ) ), VO_BLT_SRCTRANSPARENCY, NULL );


	}

	//RestoreExternBackgroundRect( gsItemPopupInvX, gsItemPopupInvY, gsItemPopupInvWidth, gsItemPopupInvHeight );
	InvalidateRegion( gsKeyRingPopupInvX, gsKeyRingPopupInvY, gsKeyRingPopupInvX + gsKeyRingPopupInvWidth, gsKeyRingPopupInvY + gsKeyRingPopupInvHeight );

}


void DeleteKeyRingPopup( )
{
	INT32			cnt;

	if( gfInKeyRingPopup == FALSE )
	{
		// done,
		return;
	}

	//Remove
	DeleteVideoObjectFromIndex( guiItemPopupBoxes );

	MSYS_RemoveRegion( &gItemPopupRegion);


	gfInKeyRingPopup = FALSE;

	for ( cnt = 0; cnt < NUMBER_KEYS_ON_KEYRING; cnt++ )
	{
		MSYS_RemoveRegion( &gKeyRingRegions[cnt]);
	}


	fInterfacePanelDirty = DIRTYLEVEL2;

	//guiTacticalInterfaceFlags &= (~INTERFACE_NORENDERBUTTONS);

//	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	if( guiCurrentItemDescriptionScreen != MAP_SCREEN )
	{
		EnableSMPanelButtons( TRUE, FALSE );
	}

	FreeMouseCursor( );
}

UINT32 GetInterfaceGraphicForItem( INVTYPE *pItem )
{
	UINT32 id;
	UINT8 ubGraphicType = pItem->ubGraphicType;

	// CHECK SUBCLASS
	if ( ubGraphicType == 0 )
	{
		SGP_TRYCATCH_RETHROW( id = g_bUsePngItemImages ? g_oGUNSM.getVObjectForItem(pItem->ubGraphicNum) : guiGUNSM,
			L"Failed to retrieve gun image" );
	}
	else 
	{
		SGP_TRYCATCH_RETHROW( id = g_bUsePngItemImages ? g_oPITEMS[ubGraphicType-1].getVObjectForItem(pItem->ubGraphicNum) : guiPITEMS[ubGraphicType-1],
			String("Failed to retrieve interface image, graphic type = %d",ubGraphicType) );
	}

	return id;
}


UINT16 GetTileGraphicForItem( INVTYPE *pItem )
{
	UINT16 usIndex;

	if ( pItem->ubClassIndex >= M900 )
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GetTileGraphicForItem: item %d graphic %d",pItem->ubClassIndex, pItem->ubGraphicNum));
	// CHECK SUBCLASS
	if ( pItem->ubGraphicType == 0 )
	{
		GetTileIndexFromTypeSubIndex( GUNS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	}
	else if ( pItem->ubGraphicType == 1 )
	{
		GetTileIndexFromTypeSubIndex( P1ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	}
	else if ( pItem->ubGraphicType == 2 )
	{
		GetTileIndexFromTypeSubIndex( P2ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	}
	else if ( pItem->ubGraphicType == 3 )
	{
		GetTileIndexFromTypeSubIndex( P3ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	}
	////MM: New item tileslots start here
	//else if ( pItem->ubGraphicType == 4 )
	//{
	//	GetTileIndexFromTypeSubIndex( P4ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}
	//else if ( pItem->ubGraphicType == 5 )
	//{
	//	GetTileIndexFromTypeSubIndex( P5ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}
	//else if ( pItem->ubGraphicType == 6 )
	//{
	//	GetTileIndexFromTypeSubIndex( P6ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}
	//else if ( pItem->ubGraphicType == 7 )
	//{
	//	GetTileIndexFromTypeSubIndex( P7ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}
	//else if ( pItem->ubGraphicType == 8 )
	//{
	//	GetTileIndexFromTypeSubIndex( P8ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}
	//else
	//{
	//	GetTileIndexFromTypeSubIndex( P9ITEMS, (INT16)(pItem->ubGraphicNum+1), &usIndex );
	//}


	if ( pItem->ubClassIndex >= M900  )
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GetTileGraphicForItem: db index %d",usIndex));

	return( usIndex );
}


BOOLEAN LoadTileGraphicForItem( INVTYPE *pItem, UINT32 *puiVo )
{
	CHAR8	 zName[ 100 ];
	UINT32	uiVo;
	VOBJECT_DESC    VObjectDesc;
	UINT16		ubGraphic;
	UINT8 ubGraphicType;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LoadTileGraphicForItem"));

	const char* ext = g_bUsePngItemImages ? ".png" : ".sti";

	// CHECK SUBCLASS
	ubGraphic = pItem->ubGraphicNum;
	ubGraphicType = pItem->ubGraphicType;

	if ( pItem->ubGraphicType == 0 )
	{
		// CHECK SUBCLASS
		//ubGraphic++;

		if ( ubGraphic < 10 )
		{
			sprintf( zName, "gun0%d", ubGraphic );
		}
		else
		{
			sprintf( zName, "gun%d", ubGraphic );
		}
	}
	else 
	{
		if ( ubGraphic < 10 )
		{
			sprintf( zName, "p%ditem0%d", ubGraphicType, ubGraphic );
		}
		else
		{
			sprintf( zName, "p%ditem%d", ubGraphicType, ubGraphic );
		}
	}


	//Load item
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	sprintf( VObjectDesc.ImageFile, "BIGITEMS\\%s%s", zName, ext );
	CHECKF( AddVideoObject( &VObjectDesc, &uiVo) );

	*puiVo = uiVo;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LoadTileGraphicForItem: done"));

	return( TRUE );
}

void ItemDescMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
}

void ItemDescCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	static BOOLEAN fRightDown = FALSE, fLeftDown = FALSE;

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		fLeftDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if ( fLeftDown )
		{
			fLeftDown = FALSE;

			//Only exit the screen if we are NOT in the money interface.  Only the DONE button should exit the money interface.
			if( gpItemDescObject->usItem != MONEY )
			{
				OBJECTTYPE *pTemp = gpItemDescPrevObject;
				DeleteItemDescriptionBox( );
				if (pTemp != NULL)
				{
					InternalInitItemDescriptionBox( pTemp, gsInvDescX, gsInvDescY, 0, gpItemDescSoldier );
				}	

			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if ( fRightDown )
		{
			fRightDown = FALSE;

			//Only exit the screen if we are NOT in the money interface.  Only the DONE button should exit the money interface.
//			if( gpItemDescObject->usItem != MONEY )
			{
				OBJECTTYPE *pTemp = gpItemDescPrevObject;
				DeleteItemDescriptionBox( );
				if (pTemp != NULL)
				{
					InternalInitItemDescriptionBox( pTemp, gsInvDescX, gsInvDescY, 0, gpItemDescSoldier );
				}	
			}
		}
	}
}

void ItemDescDoneButtonCallback( GUI_BUTTON *btn, INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	  btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
  {
    if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);

			if( gpItemDescObject->usItem == MONEY )
			{
				RemoveMoney();
			}

		  DeleteItemDescriptionBox( );
		}
	}

	if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
	  btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
  {
    if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
		  DeleteItemDescriptionBox( );
		}
	}
}


void ItemPopupRegionCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32					uiItemPos;
	UINT32					iItemCap;
	INT32					ubID;
	CHAR16					sString[ 128 ];

	uiItemPos = MSYS_GetRegionUserData( pRegion, 0 );
	iItemCap = MSYS_GetRegionUserData( pRegion, 1 );
	ubID = MSYS_GetRegionUserData( pRegion, 2 );

	// TO ALLOW ME TO DELETE REGIONS IN CALLBACKS!
	if ( gfItemPopupRegionCallbackEndFix )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if( ( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX != sSelMapX ) ||
				( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY != sSelMapY ) ||
				( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ != iCurrentMapSectorZ ) ||
				( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].flags.fBetweenSectors ) )
		{
			if ( gpItemPointer == NULL )
			{
				swprintf( sString, pMapInventoryErrorString[ 2 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
			}
			else
			{
				swprintf( sString, pMapInventoryErrorString[ 5 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
			}
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
			return;
		}
		//If something in our hand, see if it's ammo and if we are trying to reload a gun
		if ( gpItemPointer != NULL )
		{
			//CHRISL: Since we can now stack guns, let's make it so we can reload guns in a stack
			if(CompatibleAmmoForGun(gpItemPointer, gpItemPopupObject) || ValidLaunchable(gpItemPointer->usItem, gpItemPopupObject->usItem))
			{
				switch (Item[gpItemPopupObject->usItem].usItemClass)
				{
					case IC_GUN:
						if (Item[gpItemPointer->usItem].usItemClass == IC_AMMO) {
							if (Weapon[gpItemPopupObject->usItem].ubCalibre == Magazine[Item[gpItemPointer->usItem].ubClassIndex].ubCalibre) {
								ReloadGun( MercPtrs[ubID], gpItemPopupObject, gpItemPointer, uiItemPos );
							}
						}
						break;
					case IC_LAUNCHER:
						if ( ValidLaunchable( gpItemPointer->usItem, gpItemPopupObject->usItem ) ) {
							ReloadGun( MercPtrs[ubID], gpItemPopupObject, gpItemPointer, uiItemPos );
						}
						break;
				}
			}
		}
		//If one in our hand, place it
		if ( gpItemPointer != NULL )
		{
			if ( !PlaceObjectAtObjectIndex( gpItemPointer, gpItemPopupObject, (UINT8)uiItemPos, iItemCap ) || gpItemPointer->ubNumberOfObjects == 0 )
			{
				if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
				{
					MAPEndItemPointer( );
				}
				else
				{
					gpItemPointer = NULL;
					MSYS_ChangeRegionCursor( &gSMPanelRegion , CURSOR_NORMAL );
					SetCurrentCursorFromDatabase( CURSOR_NORMAL );

					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						gMoveingItem.initialize();
						SetSkiCursor( CURSOR_NORMAL );
					}
				}

				// re-evaluate repairs
				gfReEvaluateEveryonesNothingToDo = TRUE;
			}
			else
			{
				guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
				gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

				MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
				MSYS_SetCurrentCursor( EXTERN_CURSOR );
				fMapInventoryItem=TRUE;
				fTeamPanelDirty=TRUE;

				// remember which gridno the object came from
				sObjectSourceGridNo = MercPtrs[ubID]->sGridNo;
				// and who owned it last
				gpItemPointerSoldier = MercPtrs[ubID];

				ReevaluateItemHatches( MercPtrs[ubID], FALSE );
			}

				//Dirty interface
				//fInterfacePanelDirty = DIRTYLEVEL2;
				//RenderItemStackPopup( FALSE );
		}
		else
		{
			if ( uiItemPos < gpItemPopupObject->ubNumberOfObjects )
			{
				// Here, grab an item and put in cursor to swap
				//RemoveObjFrom( OBJECTTYPE * pObj, UINT8 ubRemoveIndex )

    			if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
				{
					// pick it up
					gpItemPopupObject->RemoveObjectAtIndex( uiItemPos, &gItemPointer );
					InternalMAPBeginItemPointer( gpItemPopupSoldier );
				}
				else
				{
					gpItemPopupObject->RemoveObjectAtIndex( uiItemPos, &gItemPointer );
					gpItemPointer = &gItemPointer;
					gpItemPointerSoldier = gpItemPopupSoldier;
				}

				//if we are in the shop keeper interface
				if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
				{
					// pick up stacked item into cursor and try to sell it ( unless CTRL is held down )
					BeginSkiItemPointer( PLAYERS_INVENTORY, -1, ( BOOLEAN )!gfKeyState[ CTRL ] );

				}
				// if we've just removed the last one there
				if ( gpItemPopupObject->exists() == false )
				{
					// we must immediately get out of item stack popup, because the item has been deleted (memset to 0), and
					// errors like a right bringing up an item description for item 0 could happen then.  ARM.
					DeleteItemStackPopup( );
				}

				// re-evaluate repairs
				gfReEvaluateEveryonesNothingToDo = TRUE;

				//Dirty interface
				//RenderItemStackPopup( FALSE );
				//fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}

		UpdateItemHatches();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// Get Description....
		// Some global stuff here - for esc, etc
		//Remove
		gfItemPopupRegionCallbackEndFix = TRUE;


		DeleteItemStackPopup( );

		if ( gpItemPopupObject->ubNumberOfObjects <= uiItemPos )
			fTeamPanelDirty = TRUE;
		else if ( !InItemDescriptionBox( ) )
		{
			// RESTORE BACKGROUND
			RestoreExternBackgroundRect( gsItemPopupInvX, gsItemPopupInvY, gsItemPopupInvWidth, gsItemPopupInvHeight );
			if ( guiCurrentItemDescriptionScreen == MAP_SCREEN )
			{
				// HEADROCK HAM 5: Sector Inventory Item Desc Box no longer accessible during combat.
				
				if( gTacticalStatus.uiFlags & INCOMBAT )
				{
					DoScreenIndependantMessageBox( New113HAMMessage[ 22 ], MSG_BOX_FLAG_OK, NULL );
					return;
				}
				else
				{
					if ( _KeyDown(SHIFT) && gpItemPointer == NULL && Item[gpItemPopupObject->usItem].usItemClass == IC_GUN )
					{
						EmptyWeaponMagazine( gpItemPopupObject, &gItemPointer, uiItemPos );
						InternalMAPBeginItemPointer( gpItemPopupSoldier );
					}
					else
						MAPInternalInitItemDescriptionBox( gpItemPopupObject, (UINT8)uiItemPos, gpItemPopupSoldier );
				}
			}
			else
			{
				if ( _KeyDown(SHIFT) && gpItemPointer == NULL && Item[gpItemPopupObject->usItem].usItemClass == IC_GUN && !(Item[gpItemPopupObject->usItem].singleshotrocketlauncher) && !( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
				{
					EmptyWeaponMagazine( gpItemPopupObject, &gItemPointer, uiItemPos );
					gpItemPointer = &gItemPointer;
					gpItemPointerSoldier = gpItemPopupSoldier;
				}
				else
					InternalInitItemDescriptionBox( gpItemPopupObject, (INT16) ITEMDESC_START_X, (INT16) ITEMDESC_START_Y, (UINT8)uiItemPos, gpItemPopupSoldier );
			}
		}
	}
}

void ItemPopupFullRegionCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32					uiItemPos;

	uiItemPos = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if ( InItemStackPopup( ) )
		{
			// End stack popup and retain pointer
			EndItemStackPopupWithItemInHand( );
		}
		else if( InKeyRingPopup() )
		{
			// end pop up with key in hand
			DeleteKeyRingPopup( );
			fTeamPanelDirty = TRUE;

		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if ( InItemStackPopup( ) || InSectorStackPopup( ) )
		{
			DeleteItemStackPopup( );
			fTeamPanelDirty = TRUE;
		}
		else
		{
			DeleteKeyRingPopup( );
			fTeamPanelDirty = TRUE;
		}
	}
}

#define NUM_PICKUP_SLOTS				6

#define SIZEOF_ITEM_PICKUP_MENU_STRUCT_POD offsetof(ITEM_PICKUP_MENU_STRUCT, endOfPod)
class ITEM_PICKUP_MENU_STRUCT
{
public:
	ITEM_PICKUP_MENU_STRUCT() {initialize();};
	void	initialize() {memset(this, 0, SIZEOF_ITEM_PICKUP_MENU_STRUCT_POD); CompAmmoObject.initialize();};
	ITEM_POOL			*pItemPool;
	INT16					sX;
	INT16					sY;
	INT16					sWidth;
	INT16					sHeight;
	INT8					bScrollPage;
	INT32			  	ubScrollAnchor;
	INT32		  		ubTotalItems;
	INT32		  		bCurSelect;
	UINT8					bNumSlotsPerPage;
	UINT32				uiPanelVo;
	INT32					iUpButtonImages;
	INT32					iDownButtonImages;
	INT32					iAllButtonImages;
	INT32					iCancelButtonImages;
	INT32					iOKButtonImages;
	INT32					iUpButton;
	INT32					iDownButton;
	INT32					iAllButton;
	INT32					iOKButton;
	INT32					iCancelButton;
	BOOLEAN				fCanScrollUp;
	BOOLEAN				fCanScrollDown;
	BOOLEAN				fDirtyLevel;
	INT32					iDirtyRect;
	BOOLEAN				fHandled;
	INT32 sGridNo;
	INT8					bZLevel;
	INT16					sButtomPanelStartY;
	SOLDIERTYPE		*pSoldier;
	ITEM_POOL			*ItemPoolSlots[ NUM_PICKUP_SLOTS ];
	MOUSE_REGION	Regions[ NUM_PICKUP_SLOTS ];
	MOUSE_REGION	BackRegions;
	MOUSE_REGION	BackRegion;
	BOOLEAN				*pfSelectedArray;
	BOOLEAN				fAtLeastOneSelected;
	BOOLEAN				fAllSelected;
	char	endOfPod;
	OBJECTTYPE		CompAmmoObject;
};

#define					ITEMPICK_UP_X				55
#define					ITEMPICK_UP_Y				5
#define					ITEMPICK_DOWN_X			111
#define					ITEMPICK_DOWN_Y			5
#define					ITEMPICK_ALL_X			79
#define					ITEMPICK_ALL_Y			6
#define					ITEMPICK_OK_X				16
#define					ITEMPICK_OK_Y				6
#define					ITEMPICK_CANCEL_X		141
#define					ITEMPICK_CANCEL_Y		6

#define					ITEMPICK_START_X_OFFSET		10
#define					ITEMPICK_START_Y_OFFSET		20

#define					ITEMPICK_GRAPHIC_X			10
#define					ITEMPICK_GRAPHIC_Y			12
#define					ITEMPICK_GRAPHIC_YSPACE	26

#define					ITEMPICK_TEXT_X					56
#define					ITEMPICK_TEXT_Y					22
#define					ITEMPICK_TEXT_YSPACE		26
#define					ITEMPICK_TEXT_WIDTH			109
#define					ITEMPICK_TEXT_HEIGHT		17




ITEM_PICKUP_MENU_STRUCT			gItemPickupMenu;
BOOLEAN											gfInItemPickupMenu = FALSE;

void ItemPickupScrollUp( GUI_BUTTON *btn, INT32 reason );
void ItemPickupScrollDown( GUI_BUTTON *btn, INT32 reason );
void ItemPickupAll( GUI_BUTTON *btn, INT32 reason );
void ItemPickupOK( GUI_BUTTON *btn, INT32 reason );
void ItemPickupCancel( GUI_BUTTON *btn, INT32 reason );
void SetupPickupPage( INT8 bPage );
void ItemPickMenuMouseMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void ItemPickMenuMouseClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CalculateItemPickupMenuDimensions( );
void ItemPickupBackgroundClick( MOUSE_REGION * pRegion, INT32 iReason );


// STUFF FOR POPUP ITEM INFO BOX
void SetItemPickupMenuDirty( BOOLEAN fDirtyLevel )
{
	gItemPickupMenu.fDirtyLevel = fDirtyLevel;
}


BOOLEAN InitializeItemPickupMenu( SOLDIERTYPE *pSoldier, INT32 sGridNo, ITEM_POOL *pItemPool, INT16 sScreenX, INT16 sScreenY, INT8 bZLevel )
{
  VOBJECT_DESC    VObjectDesc;
	CHAR8						ubString[48];
	ITEM_POOL				*pTempItemPool;
	INT32						cnt;
	INT16						sCenX, sCenY, sX, sY, sCenterYVal;

	gfStealing=FALSE;
	gpOpponent=NULL;

	// Erase other menus....
	EraseInterfaceMenus( TRUE );

	// Make sure menu is located if not on screen
	LocateSoldier( pSoldier->ubID, FALSE );

	// memset values
	gItemPickupMenu.initialize();

	//Set item pool value
	gItemPickupMenu.pItemPool		= pItemPool;

	InterruptTime();
	PauseGame();
	LockPauseState( 18 );
	// Pause timers as well....
	PauseTime( TRUE );


	// Alrighty, cancel lock UI if we havn't done so already
	UnSetUIBusy( pSoldier->ubID );

	// Change to INV panel if not there already...
	gfSwitchPanel = TRUE;
	gbNewPanel = SM_PANEL;
	gubNewPanelParam = (UINT8)pSoldier->ubID;

	//Determine total #
	cnt = 0;
	pTempItemPool = pItemPool;
	while( pTempItemPool != NULL )
	{
		if ( ItemPoolOKForDisplay( pTempItemPool, bZLevel ) )
		{
			cnt++;
		}

		pTempItemPool = pTempItemPool->pNext;
	}
	gItemPickupMenu.ubTotalItems = (UINT8)cnt;

	// Determine # of slots per page
	if ( gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
	{
		gItemPickupMenu.bNumSlotsPerPage = NUM_PICKUP_SLOTS;
	}
	else
	{
		gItemPickupMenu.bNumSlotsPerPage = gItemPickupMenu.ubTotalItems;
	}

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\itembox.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(gItemPickupMenu.uiPanelVo) ) );

	// Memalloc selection array...
	 gItemPickupMenu.pfSelectedArray = (BOOLEAN *) MemAlloc(( sizeof( UINT8 ) * gItemPickupMenu.ubTotalItems ) );
	// seto to 0
	memset( gItemPickupMenu.pfSelectedArray, 0, ( sizeof( UINT8 ) * gItemPickupMenu.ubTotalItems ) );

	// Calcualate dimensions
	CalculateItemPickupMenuDimensions( );

	// Get XY
	{
		// First get mouse xy screen location		
		if(!TileIsOutOfBounds(sGridNo))
		{
			sX = gusMouseXPos;
			sY = gusMouseYPos;
		}
		else
		{
			 sX = sScreenX;
			 sY = sScreenY;
		}

		// CHECK FOR LEFT/RIGHT
		if ( ( sX + gItemPickupMenu.sWidth ) > SCREEN_WIDTH )
		{
			sX = SCREEN_WIDTH - gItemPickupMenu.sWidth - ITEMPICK_START_X_OFFSET;
		}
		else
		{
			sX = sX + ITEMPICK_START_X_OFFSET;
		}

		// Now check for top
		// Center in the y
		sCenterYVal = gItemPickupMenu.sHeight / 2;

		sY -= sCenterYVal;

		if ( sY < gsVIEWPORT_WINDOW_START_Y )
		{
			sY = gsVIEWPORT_WINDOW_START_Y;
		}

		// Check for bottom
		if ( ( sY + gItemPickupMenu.sHeight ) > INV_INTERFACE_START_Y )
		{
			sY = INV_INTERFACE_START_Y - gItemPickupMenu.sHeight;
		}

	}

	// Set some values
	gItemPickupMenu.sX					= sX;
	gItemPickupMenu.sY					= sY;
	gItemPickupMenu.bCurSelect	= 0;
	gItemPickupMenu.pSoldier		= pSoldier;
	gItemPickupMenu.fHandled		= FALSE;
	gItemPickupMenu.sGridNo			= sGridNo;
	gItemPickupMenu.bZLevel			= bZLevel;
	gItemPickupMenu.fAtLeastOneSelected = FALSE;
	gItemPickupMenu.fAllSelected	= FALSE;

	//Load images for buttons
	FilenameForBPP("INTERFACE\\itembox.sti", ubString );
	gItemPickupMenu.iUpButtonImages			= LoadButtonImage( ubString, -1,5,-1,10,-1 );
	gItemPickupMenu.iDownButtonImages		=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 7, -1, 12, -1 );
	gItemPickupMenu.iAllButtonImages		=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 6, -1,11, -1 );
	gItemPickupMenu.iCancelButtonImages	=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 8, -1, 13, -1 );
	gItemPickupMenu.iOKButtonImages			=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 4, -1, 9, -1 );

/* This region definition must be checked and recalculated!!!
 * i didnt have time for that sorry
 * any questions? joker
 */

	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &(gItemPickupMenu.BackRegion ), (INT16)( SCREEN_WIDTH - 108 ), (INT16)( SCREEN_HEIGHT - 133 ), (INT16)( SCREEN_WIDTH  ),(INT16)( SCREEN_HEIGHT ), MSYS_PRIORITY_HIGHEST,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	// Add region
	MSYS_AddRegion( &(gItemPickupMenu.BackRegion ) );


	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &(gItemPickupMenu.BackRegions ), (INT16)(gItemPickupMenu.sX ), (INT16)(gItemPickupMenu.sY), (INT16)(gItemPickupMenu.sX + gItemPickupMenu.sWidth ),(INT16)( gItemPickupMenu.sY + gItemPickupMenu.sHeight ), MSYS_PRIORITY_HIGHEST,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	// Add region
	MSYS_AddRegion( &(gItemPickupMenu.BackRegions ) );


	// Create buttons
	if ( gItemPickupMenu.bNumSlotsPerPage == NUM_PICKUP_SLOTS && gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
	{
		gItemPickupMenu.iUpButton	= QuickCreateButton( gItemPickupMenu.iUpButtonImages, (INT16)(sX + ITEMPICK_UP_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_UP_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupScrollUp );

		SetButtonFastHelpText( gItemPickupMenu.iUpButton, ItemPickupHelpPopup[ 1 ] );


		gItemPickupMenu.iDownButton	= QuickCreateButton( gItemPickupMenu.iDownButtonImages, (INT16)(sX + ITEMPICK_DOWN_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_DOWN_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupScrollDown );

		SetButtonFastHelpText( gItemPickupMenu.iDownButton, ItemPickupHelpPopup[ 3 ] );

	}


	gItemPickupMenu.iOKButton	= QuickCreateButton( gItemPickupMenu.iOKButtonImages, (INT16)(sX + ITEMPICK_OK_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_OK_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupOK );
	SetButtonFastHelpText( gItemPickupMenu.iOKButton, ItemPickupHelpPopup[ 0 ] );


	gItemPickupMenu.iAllButton	= QuickCreateButton( gItemPickupMenu.iAllButtonImages, (INT16)(sX + ITEMPICK_ALL_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_ALL_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupAll );
	SetButtonFastHelpText( gItemPickupMenu.iAllButton, ItemPickupHelpPopup[ 2 ] );

	gItemPickupMenu.iCancelButton	= QuickCreateButton( gItemPickupMenu.iCancelButtonImages, (INT16)(sX + ITEMPICK_CANCEL_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_CANCEL_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupCancel );
	SetButtonFastHelpText( gItemPickupMenu.iCancelButton, ItemPickupHelpPopup[ 4 ] );


	DisableButton( gItemPickupMenu.iOKButton );



	// Create regions...
	sCenX = gItemPickupMenu.sX;
	sCenY = gItemPickupMenu.sY + ITEMPICK_GRAPHIC_Y;

	for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
	{
		// Build a mouse region here that is over any others.....
		MSYS_DefineRegion( &(gItemPickupMenu.Regions[cnt]), (INT16)(sCenX ), (INT16)(sCenY + 1), (INT16)(sCenX + gItemPickupMenu.sWidth ),(INT16)( sCenY + ITEMPICK_GRAPHIC_YSPACE ), MSYS_PRIORITY_HIGHEST,
							 CURSOR_NORMAL, ItemPickMenuMouseMoveCallback, ItemPickMenuMouseClickCallback );
		// Add region
		MSYS_AddRegion( &(gItemPickupMenu.Regions[cnt]) );
		MSYS_SetRegionUserData( &(gItemPickupMenu.Regions[cnt]), 0, cnt );

		sCenY += ITEMPICK_GRAPHIC_YSPACE;
	}

	//Save dirty rect
	//gItemPickupMenu.iDirtyRect = RegisterBackgroundRect( BGND_FLAG_PERMANENT | BGND_FLAG_SAVERECT, NULL, gItemPickupMenu.sX, gItemPickupMenu.sY, (INT16)(gItemPickupMenu.sX + gItemPickupMenu.sWidth ) , (INT16)(gItemPickupMenu.sY + gItemPickupMenu.sHeight ) );


	SetupPickupPage( 0 );

	gfInItemPickupMenu = TRUE;

	// Ignore scrolling
	gfIgnoreScrolling = TRUE;

	HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
	gubSelectSMPanelToMerc = pSoldier->ubID;
	ReEvaluateDisabledINVPanelButtons( );
	DisableTacticalTeamPanelButtons( TRUE );

	//gfSMDisableForItems = TRUE;
	return( TRUE );

}


void SetupPickupPage( INT8 bPage )
{
	INT32 cnt, iStart, iEnd;
	ITEM_POOL				*pTempItemPool;
  INT16           sValue;
	OBJECTTYPE  *pObject;
	static CHAR16 pStr[ 200 ];


	// Zero out page slots
	memset( gItemPickupMenu.ItemPoolSlots, 0, sizeof( gItemPickupMenu.ItemPoolSlots )  );

	// Zero page flags
	gItemPickupMenu.fCanScrollUp		= FALSE;
	gItemPickupMenu.fCanScrollDown  = FALSE;

	// Get lower bound
	iStart = bPage * NUM_PICKUP_SLOTS;
	if ( iStart > gItemPickupMenu.ubTotalItems )
	{
		return;
	}

	if ( bPage > 0 )
	{
		gItemPickupMenu.fCanScrollUp = TRUE;
	}


	iEnd   = iStart + NUM_PICKUP_SLOTS;
	if ( iEnd >= gItemPickupMenu.ubTotalItems )
	{
		iEnd = gItemPickupMenu.ubTotalItems;
	}
	else
	{
		// We can go for more!
		gItemPickupMenu.fCanScrollDown = TRUE;
	}

	// Setup slots!
	// These slots contain an inventory pool pointer for each slot...
	pTempItemPool = gItemPickupMenu.pItemPool;

  // ATE: Patch fix here for crash :(
  // Clear help text!
	for ( cnt = 0; cnt < NUM_PICKUP_SLOTS; cnt++ )
	{
  	SetRegionFastHelpText( &(gItemPickupMenu.Regions[cnt]), L"" );
  }

	for ( cnt = 0; cnt < iEnd; )
	{
		// Move to the closest one that can be displayed....
		if (!gfStealing)
		{
			while( !ItemPoolOKForDisplay( pTempItemPool, gItemPickupMenu.bZLevel ) )
			{
				pTempItemPool = pTempItemPool->pNext;
			}
		}

		if ( cnt >= iStart )
		{
			gItemPickupMenu.ItemPoolSlots[ cnt - iStart ] = pTempItemPool;

			pObject = (gfStealing)? &gpOpponent->inv[pTempItemPool->iItemIndex]
				:&(gWorldItems[ pTempItemPool->iItemIndex ].object );

		  sValue = (*pObject)[0]->data.objectStatus;

	    // Adjust for ammo, other thingys..
	    if( Item[ pObject->usItem ].usItemClass & IC_AMMO || Item[ pObject->usItem ].usItemClass & IC_KEY )
	    {
        swprintf( pStr, L"" );
	    }
      else
      {
		#ifdef CHINESE
			swprintf( pStr, ChineseSpecString3, sValue );
		#else
			swprintf( pStr, L"%d%%", sValue );
		#endif	
      }

    	SetRegionFastHelpText( &(gItemPickupMenu.Regions[ cnt - iStart ]), pStr );
		}

		cnt++;

		pTempItemPool = pTempItemPool->pNext;
	}

	gItemPickupMenu.bScrollPage		= bPage;
	gItemPickupMenu.ubScrollAnchor	= (UINT8)iStart;

	if ( gItemPickupMenu.bNumSlotsPerPage == NUM_PICKUP_SLOTS && gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
	{
		// Setup enabled/disabled buttons
		if ( gItemPickupMenu.fCanScrollUp )
		{
			EnableButton( gItemPickupMenu.iUpButton );
		}
		else
		{
			DisableButton( gItemPickupMenu.iUpButton );
		}

		// Setup enabled/disabled buttons
		if ( gItemPickupMenu.fCanScrollDown )
		{
			EnableButton( gItemPickupMenu.iDownButton );
		}
		else
		{
			DisableButton( gItemPickupMenu.iDownButton );
		}
	}
	SetItemPickupMenuDirty( DIRTYLEVEL2 );

}


void CalculateItemPickupMenuDimensions( )
{
	INT32			cnt;
	INT16			sX, sY;
	UINT16			usSubRegion, usHeight, usWidth;

	// Build background
	sX = 0;
	sY = 0;

	for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
	{
		if ( cnt == 0 )
		{
			usSubRegion = 0;
		}
		else
		{
			usSubRegion = 1;
		}

		// Add hieght of object
		GetVideoObjectETRLESubregionProperties( gItemPickupMenu.uiPanelVo, usSubRegion, &usWidth, &usHeight );

		sY += usHeight;

	}
	gItemPickupMenu.sButtomPanelStartY = sY;

	// Do end
	GetVideoObjectETRLESubregionProperties( gItemPickupMenu.uiPanelVo, 2, &usWidth, &usHeight );

	sY += usHeight;

	// Set height, width
	gItemPickupMenu.sHeight				= sY;
	gItemPickupMenu.sWidth				= usWidth;

}


// set pick up menu dirty level
void SetPickUpMenuDirtyLevel( BOOLEAN fDirtyLevel )
{
	gItemPickupMenu.fDirtyLevel = fDirtyLevel;

	return;
}


void RenderItemPickupMenu( )
{
	INT32			cnt;
	UINT16		usItemTileIndex;
	INT16			sX, sY, sCenX, sCenY, sFontX, sFontY, sNewX, sNewY;
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;
	CHAR16				pStr[ 100 ];
	UINT16			usSubRegion, usHeight, usWidth;
	INVTYPE   *pItem;
	OBJECTTYPE  *pObject;
	UINT16			uiStringLength;


	if ( !gfInItemPickupMenu )
	{
		return;
	}


	// Do everything!
	if ( gItemPickupMenu.fDirtyLevel == DIRTYLEVEL2 )
	{
		MarkButtonsDirty( );

		// Build background
		sX = gItemPickupMenu.sX;
		sY = gItemPickupMenu.sY;

		for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
		{
			if ( cnt == 0 )
			{
				usSubRegion = 0;
			}
			else
			{
				usSubRegion = 1;
			}

			BltVideoObjectFromIndex( FRAME_BUFFER, gItemPickupMenu.uiPanelVo, usSubRegion, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );

			// Add hieght of object
			GetVideoObjectETRLESubregionProperties( gItemPickupMenu.uiPanelVo, usSubRegion, &usWidth, &usHeight );

			sY += usHeight;

		}

		// Do end
		if ( gItemPickupMenu.bNumSlotsPerPage == NUM_PICKUP_SLOTS && gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
		{
			BltVideoObjectFromIndex( FRAME_BUFFER, gItemPickupMenu.uiPanelVo, 2, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			BltVideoObjectFromIndex( FRAME_BUFFER, gItemPickupMenu.uiPanelVo, 3, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// Render items....
		sX = ITEMPICK_GRAPHIC_X + gItemPickupMenu.sX;
		sY = ITEMPICK_GRAPHIC_Y + gItemPickupMenu.sY;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetFont( ITEMDESC_FONT );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontShadow( ITEMDESC_FONTSHADOW2 );

		for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
		{
			if ( gItemPickupMenu.ItemPoolSlots[ cnt ] != NULL )
			{
				// Get item to render
				pObject = (gfStealing)? &gpOpponent->inv[gItemPickupMenu.ItemPoolSlots[ cnt ]->iItemIndex]
					:&(gWorldItems[ gItemPickupMenu.ItemPoolSlots[ cnt ]->iItemIndex ].object );
				pItem = &( Item[ pObject->usItem ] );

				usItemTileIndex = GetTileGraphicForItem( pItem );

				// Render
				sX = ITEMPICK_GRAPHIC_X + gItemPickupMenu.sX;

				sCenX = sX;
				sCenY = sY;

        // ATE: Adjust to basic shade.....
    		gTileDatabase[ usItemTileIndex ].hTileSurface->pShadeCurrent=gTileDatabase[ usItemTileIndex ].hTileSurface->pShades[4];

				//else
				{
					if ( gItemPickupMenu.pfSelectedArray[  cnt + gItemPickupMenu.ubScrollAnchor ] )
					{
						//SetFontForeground( FONT_MCOLOR_LTYELLOW );
						//SetFontShadow( ITEMDESC_FONTSHADOW2 );
						Blt8BPPDataTo16BPPBufferOutline( (UINT16*)pDestBuf, uiDestPitchBYTES, gTileDatabase[ usItemTileIndex ].hTileSurface, sCenX, sCenY, gTileDatabase[ usItemTileIndex ].usRegionIndex, Get16BPPColor( FROMRGB( 255, 255, 0 ) ), TRUE );
					}
					else
					{
						//SetFontForeground( FONT_BLACK );
						//SetFontShadow( ITEMDESC_FONTSHADOW2 );
						Blt8BPPDataTo16BPPBufferOutline( (UINT16*)pDestBuf, uiDestPitchBYTES, gTileDatabase[ usItemTileIndex ].hTileSurface, sCenX, sCenY, gTileDatabase[ usItemTileIndex ].usRegionIndex, 0, FALSE );
					}
        }

        // Draw text.....
      	SetFont( ITEM_FONT );
				if ( pObject->ubNumberOfObjects > 1 )
				{
					SetFontForeground( FONT_GRAY4 );
					SetFontShadow( DEFAULT_SHADOW );

				  sCenX = sX - 4;
				  sCenY = sY + 14;

				  swprintf( pStr, L"%d", pObject->ubNumberOfObjects );

				  VarFindFontRightCoordinates( sCenX, sCenY, 42, 1 , ITEM_FONT, &sFontX, &sFontY, pStr );
				  mprintf_buffer( pDestBuf, uiDestPitchBYTES, ITEM_FONT, sFontX, sFontY, pStr );
				}
      	SetFont( ITEMDESC_FONT );


        // Render attachment symbols
			  if ( ItemHasAttachments( pObject ) )
			  {
				  SetFontForeground( FONT_GREEN );
					SetFontShadow( DEFAULT_SHADOW );

				  sNewY = sCenY + 2;
				  swprintf( pStr, L"*" );

				  // Get length of string
				  uiStringLength=StringPixLength( pStr, ITEM_FONT );

				  sNewX = sCenX + 43 - uiStringLength - 4;

  				mprintf_buffer( pDestBuf, uiDestPitchBYTES, ITEMDESC_FONT, sNewX, sNewY, pStr );
				  //gprintfinvalidate( sNewX, sNewY, pStr );
			  }

			  if((UsingNewInventorySystem() == true))
			  {
				  // CHRISL: Show astrisk for active LBENODE
				  if ( pObject->IsActiveLBE(0))
				  {
					  SetFontForeground( FONT_BLUE );
						SetFontShadow( DEFAULT_SHADOW );

					  sNewY = sCenY + 2;
					  swprintf( pStr, L"*" );

					  // Get length of string
					  uiStringLength=StringPixLength( pStr, ITEM_FONT );

					  sNewX = sCenX + 43 - uiStringLength - 4;

  					mprintf_buffer( pDestBuf, uiDestPitchBYTES, ITEMDESC_FONT, sNewX, sNewY, pStr );
					  //gprintfinvalidate( sNewX, sNewY, pStr );
				  }
			  }

				if ( gItemPickupMenu.bCurSelect == ( cnt + gItemPickupMenu.ubScrollAnchor ) )
				{
					//SetFontForeground( ITEMDESC_FONTSHADOW2 );
					//if ( gItemPickupMenu.pfSelectedArray[  cnt + gItemPickupMenu.ubScrollAnchor ] )
					//{
					//	SetFontForeground( FONT_MCOLOR_LTYELLOW );
					//	SetFontShadow( ITEMDESC_FONTSHADOW2 );
					//}
					//else
					//{
						SetFontForeground( FONT_WHITE );
						SetFontShadow( DEFAULT_SHADOW );
					//}
					// Blt8BPPDataTo16BPPBufferOutline( (UINT16*)pDestBuf, uiDestPitchBYTES, gTileDatabase[ usItemTileIndex ].hTileSurface, sCenX, sCenY, gTileDatabase[ usItemTileIndex ].usRegionIndex, Get16BPPColor( FROMRGB( 255, 0, 0 ) ), TRUE );
					// Blt8BPPDataTo16BPPBufferOutline( (UINT16*)pDestBuf, uiDestPitchBYTES, gTileDatabase[ usItemTileIndex ].hTileSurface, sCenX, sCenY, gTileDatabase[ usItemTileIndex ].usRegionIndex, Get16BPPColor( FROMRGB( 255, 0, 0 ) ), TRUE );
				}
				else
				{
					SetFontForeground( FONT_BLACK );
					SetFontShadow( ITEMDESC_FONTSHADOW2 );
				}

				// Render name
				sCenX = ITEMPICK_TEXT_X + gItemPickupMenu.sX;
				sCenY = ITEMPICK_TEXT_Y + gItemPickupMenu.sY + ( ITEMPICK_TEXT_YSPACE * (INT16)cnt );

				// If we are money...
				if ( Item[ pObject->usItem ].usItemClass == IC_MONEY )
				{
					CHAR16		pStr2[20];
					swprintf( pStr2, L"%ld", (*pObject)[0]->data.money.uiMoneyAmount );
					InsertCommasForDollarFigure( pStr2 );
					InsertDollarSignInToString( pStr2 );

					swprintf( pStr, L"%s (%ls)", ItemNames[ pObject->usItem ], pStr2 );
				}
				else
				{
					swprintf( pStr, L"%s", ShortItemNames[ pObject->usItem ] );
				}
				VarFindFontCenterCoordinates( sCenX, sCenY, ITEMPICK_TEXT_WIDTH, 1 , ITEMDESC_FONT, &sFontX, &sFontY, pStr );
				mprintf_buffer( pDestBuf, uiDestPitchBYTES, ITEMDESC_FONT, sFontX, sFontY, pStr );

				sY += ITEMPICK_GRAPHIC_YSPACE;
			}
		}

		SetFontShadow( DEFAULT_SHADOW );


		UnLockVideoSurface( FRAME_BUFFER );

		InvalidateRegion( gItemPickupMenu.sX, gItemPickupMenu.sY, gItemPickupMenu.sX + gItemPickupMenu.sWidth, gItemPickupMenu.sY + gItemPickupMenu.sHeight );

		gItemPickupMenu.fDirtyLevel = 0;

	}
}


void RemoveItemPickupMenu( )
{
	INT32 cnt;

	if ( gfInItemPickupMenu )
	{
		gfSMDisableForItems = FALSE;

		HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );

		UnLockPauseState();
		UnPauseGame();
		// UnPause timers as well....
		PauseTime( FALSE );

		// Unfreese guy!
		gItemPickupMenu.pSoldier->flags.fPauseAllAnimation = FALSE;

		// Remove graphics!
		DeleteVideoObjectFromIndex( gItemPickupMenu.uiPanelVo );

		// Remove buttons
		if ( gItemPickupMenu.bNumSlotsPerPage == NUM_PICKUP_SLOTS && gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
		{
			RemoveButton( gItemPickupMenu.iUpButton );
			RemoveButton( gItemPickupMenu.iDownButton );
		}
		RemoveButton( gItemPickupMenu.iAllButton );
		RemoveButton( gItemPickupMenu.iOKButton );
		RemoveButton( gItemPickupMenu.iCancelButton );

		// Remove button images
		UnloadButtonImage( gItemPickupMenu.iUpButtonImages );
		UnloadButtonImage( gItemPickupMenu.iDownButtonImages );
		UnloadButtonImage( gItemPickupMenu.iAllButtonImages );
		UnloadButtonImage( gItemPickupMenu.iCancelButtonImages );
		UnloadButtonImage( gItemPickupMenu.iOKButtonImages );

		MSYS_RemoveRegion( &(gItemPickupMenu.BackRegions ) );
		MSYS_RemoveRegion( &(gItemPickupMenu.BackRegion ) );

		// Remove regions
		for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
		{
			MSYS_RemoveRegion( &(gItemPickupMenu.Regions[cnt]));
		}

		// Remove register rect
		if ( gItemPickupMenu.iDirtyRect != -1 )
		{
			//FreeBackgroundRect( gItemPickupMenu.iDirtyRect );
		}

		// Free selection list...
		MemFree( gItemPickupMenu.pfSelectedArray );
		gItemPickupMenu.pfSelectedArray = NULL;


		// Set cursor back to normal mode...
		guiPendingOverrideEvent = A_CHANGE_TO_MOVE;

		// Rerender world
		SetRenderFlags( RENDER_FLAG_FULL );

		gfInItemPickupMenu = FALSE;

		//gfSMDisableForItems = FALSE;
		//EnableButtonsForInItemBox( TRUE );
		EnableSMPanelButtons( TRUE , TRUE );
		gfSMDisableForItems = FALSE;

		fInterfacePanelDirty = DIRTYLEVEL2;

		// Turn off Ignore scrolling
		gfIgnoreScrolling = FALSE;
		DisableTacticalTeamPanelButtons( FALSE );
		gubSelectSMPanelToMerc = gpSMCurrentMerc->ubID;

	}
}


void ItemPickupScrollUp( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		SetupPickupPage( (UINT8)( gItemPickupMenu.bScrollPage - 1 ) );
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}


void ItemPickupScrollDown( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		SetupPickupPage( (UINT8)( gItemPickupMenu.bScrollPage + 1 ) );
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void ItemPickupAll( GUI_BUTTON *btn, INT32 reason )
{
	INT32 cnt;


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gItemPickupMenu.fAllSelected = !gItemPickupMenu.fAllSelected;


		// OK, pickup item....
		//gItemPickupMenu.fHandled = TRUE;
		// Tell our soldier to pickup this item!
		//SoldierGetItemFromWorld( gItemPickupMenu.pSoldier, ITEM_PICKUP_ACTION_ALL, gItemPickupMenu.sGridNo, gItemPickupMenu.bZLevel, NULL );
		for ( cnt = 0; cnt < gItemPickupMenu.ubTotalItems; cnt++ )
		{
			gItemPickupMenu.pfSelectedArray[ cnt ] = gItemPickupMenu.fAllSelected;
		}

		if ( gItemPickupMenu.fAllSelected )
		{
			EnableButton( gItemPickupMenu.iOKButton );
		}
		else
		{
			DisableButton( gItemPickupMenu.iOKButton );
		}

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}


void ItemPickupOK( GUI_BUTTON *btn, INT32 reason )
{
	UINT16 usLastItem;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// OK, pickup item....
		gItemPickupMenu.fHandled = TRUE;

		if (gfStealing)	//jackaians modif
		{
			usLastItem=gpOpponent->inv[HANDPOS].usItem;
			SoldierStealItemFromSoldier( gItemPickupMenu.pSoldier,gpOpponent,gItemPickupMenu.pItemPool, ITEM_PICKUP_SELECTION, gItemPickupMenu.sGridNo, gItemPickupMenu.bZLevel, gItemPickupMenu.pfSelectedArray );
			DeletePool(gItemPickupMenu.pItemPool);
			if ((gpOpponent->inv[HANDPOS].exists() == false ) && (usLastItem!=NOTHING))
				gpOpponent->ReLoadSoldierAnimationDueToHandItemChange( usLastItem, NOTHING );

//			PreventFromTheFreezingBug(gItemPickupMenu.pSoldier);
		}
		else
		{
			// Tell our soldier to pickup this item!
			SoldierGetItemFromWorld( gItemPickupMenu.pSoldier, ITEM_PICKUP_SELECTION, gItemPickupMenu.sGridNo, gItemPickupMenu.bZLevel, gItemPickupMenu.pfSelectedArray );
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void ItemPickupCancel( GUI_BUTTON *btn, INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// OK, pickup item....
		gItemPickupMenu.fHandled = TRUE;

		// Nonomori: Fix crash-on-cancel by doing this here instead of on the ...LBUTTON_DWN event
		if (gfStealing)		//jackaians modif
		{
			DeletePool(gItemPickupMenu.pItemPool);

//			PreventFromTheFreezingBug(gItemPickupMenu.pSoldier);
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}


void ItemPickMenuMouseMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32					uiItemPos;
	//ITEM_POOL				*pTempItemPool;
	INT32						bPos;
	static					BOOLEAN	bChecked = FALSE;

	uiItemPos = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		bPos = ( uiItemPos + gItemPickupMenu.ubScrollAnchor );

		if ( bPos < gItemPickupMenu.ubTotalItems )
		{
			// Set current selected guy....
			gItemPickupMenu.bCurSelect = bPos;

			if ( !bChecked )
			{
				// WANNE: There is a big bug in the stealing routine.
				/*
				// Show compatible ammo...
				pTempItemPool = gItemPickupMenu.ItemPoolSlots[ gItemPickupMenu.bCurSelect - gItemPickupMenu.ubScrollAnchor ];

				// Nonomori: Fix crash caused by stealing ammo in a sector with no items loaded.
				// gItemPickupMenu.CompAmmoObject = gWorldItems[ pTempItemPool->iItemIndex ].object;
				if ( gWorldItems && gWorldItems[ pTempItemPool->iItemIndex ].fExists )
				{
					gItemPickupMenu.CompAmmoObject = gWorldItems[ pTempItemPool->iItemIndex ].object ;
				}
				else
				{
					OBJECTTYPE pCompAmmoObject;
					CreateItem( pTempItemPool->iItemIndex, 100, &pCompAmmoObject);
					gItemPickupMenu.CompAmmoObject = pCompAmmoObject;
				}

				// Turn off first...
				HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
				InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, &( gItemPickupMenu.CompAmmoObject ), TRUE );

				// Nonomori: Fix crash caused by stealing ammo in a sector with no items loaded.
				// HandleAnyMercInSquadHasCompatibleStuff( (INT8)CurrentSquad( ), &(gWorldItems[ pTempItemPool->iItemIndex ].object ), FALSE );
				HandleAnyMercInSquadHasCompatibleStuff( (INT8)CurrentSquad( ), pCompAmmoObject, FALSE );
				*/


				SetItemPickupMenuDirty( DIRTYLEVEL2 );

				bChecked = TRUE;
			}
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		gItemPickupMenu.bCurSelect = 255;

		InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, &( gItemPickupMenu.CompAmmoObject ), FALSE );
		HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );

		SetItemPickupMenuDirty( DIRTYLEVEL2 );

		bChecked = FALSE;
	}


}


void ItemPickupBackgroundClick( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// OK, goto team panel....
		ToggleTacticalPanels();
	}
}



void ItemPickMenuMouseClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32				  	uiItemPos;
	UINT8						cnt;
	BOOLEAN					fEnable = FALSE;

	uiItemPos = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if ( uiItemPos + gItemPickupMenu.ubScrollAnchor < gItemPickupMenu.ubTotalItems )
		{
			// Toggle selection... ONLY IF LEGAL!!
			gItemPickupMenu.pfSelectedArray[ uiItemPos + gItemPickupMenu.ubScrollAnchor ] = !gItemPickupMenu.pfSelectedArray[  uiItemPos + gItemPickupMenu.ubScrollAnchor ];

			// OK, pickup item....
			//gItemPickupMenu.fHandled = TRUE;

			//pTempItemPool = gItemPickupMenu.ItemPoolSlots[ gItemPickupMenu.bCurSelect - gItemPickupMenu.ubScrollAnchor ];

			// Tell our soldier to pickup this item!
			//SoldierGetItemFromWorld( gItemPickupMenu.pSoldier, pTempItemPool->iItemIndex, gItemPickupMenu.sGridNo, gItemPickupMenu.bZLevel );
		}

		// Loop through all and set /unset OK
		for ( cnt = 0; cnt < gItemPickupMenu.ubTotalItems; cnt++ )
		{
			if ( gItemPickupMenu.pfSelectedArray[ cnt ] )
			{
				fEnable = TRUE;
				break;
			}
		}

		if ( fEnable )
		{
			EnableButton( gItemPickupMenu.iOKButton );
		}
		else
		{
			DisableButton( gItemPickupMenu.iOKButton );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{

	}
}

BOOLEAN HandleItemPickupMenu( )
{

	if ( !gfInItemPickupMenu )
	{
		return( FALSE );
	}

	if ( gItemPickupMenu.fHandled )
	{
		RemoveItemPickupMenu( );
	}

	return( gItemPickupMenu.fHandled );
}


void BtnMoneyButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8	i;
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		switch( ubButton )
		{
			case M_1000:
				if( gRemoveMoney.uiMoneyRemaining >= 1000 )
				{
					//if the player is removing money from their account, and they are removing more then $20,000
					if( gfAddingMoneyToMercFromPlayersAccount &&  ( gRemoveMoney.uiMoneyRemoving + 1000 ) > MAX_MONEY_PER_SLOT )
					{
						if( guiCurrentScreen == SHOPKEEPER_SCREEN )
							DoMessageBox( MSG_BOX_BASIC_STYLE, gzMoneyWithdrawMessageText[ MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM ], SHOPKEEPER_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
						else
							DoMessageBox( MSG_BOX_BASIC_STYLE, gzMoneyWithdrawMessageText[ MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
						return;
					}

					gRemoveMoney.uiMoneyRemaining -= 1000;
					gRemoveMoney.uiMoneyRemoving += 1000;
				}
				break;
			case M_100:
				if( gRemoveMoney.uiMoneyRemaining >= 100 )
				{
					//if the player is removing money from their account, and they are removing more then $20,000
					if( gfAddingMoneyToMercFromPlayersAccount &&  ( gRemoveMoney.uiMoneyRemoving + 100 ) > MAX_MONEY_PER_SLOT )
					{
						DoMessageBox( MSG_BOX_BASIC_STYLE, gzMoneyWithdrawMessageText[ MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
						return;
					}

					gRemoveMoney.uiMoneyRemaining -= 100;
					gRemoveMoney.uiMoneyRemoving += 100;
				}
				break;
			case M_10:
				if( gRemoveMoney.uiMoneyRemaining >= 10 )
				{
					//if the player is removing money from their account, and they are removing more then $20,000
					if( gfAddingMoneyToMercFromPlayersAccount &&  ( gRemoveMoney.uiMoneyRemoving + 10 ) > MAX_MONEY_PER_SLOT )
					{
						DoMessageBox( MSG_BOX_BASIC_STYLE, gzMoneyWithdrawMessageText[ MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
						return;
					}

					gRemoveMoney.uiMoneyRemaining -= 10;
					gRemoveMoney.uiMoneyRemoving += 10;
				}
				break;
			case M_DONE:
			{
				RemoveMoney();

				DeleteItemDescriptionBox( );
			}
			break;
		}
		if( ubButton != M_DONE )
		{
			RenderItemDescriptionBox( );
			for( i=0; i<OLD_MAX_ATTACHMENTS_101; i++ )
			{
				MarkAButtonDirty( guiMoneyButtonBtn[ i ] );
			}
			// HEADROCK HAM 4: Mark buttons dirty for UDB
			if (UsingEDBSystem())
			{
				MarkAButtonDirty( giInvDescTabButton[ 0 ] );
				MarkAButtonDirty( giInvDescTabButton[ 1 ] );
				MarkAButtonDirty( giInvDescTabButton[ 2 ] );
			}
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}


	if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		btn->uiFlags &= (~BUTTON_CLICKED_ON );


		switch( ubButton )
		{
			case M_1000:
				if( gRemoveMoney.uiMoneyRemoving >= 1000 )
				{
					gRemoveMoney.uiMoneyRemaining += 1000;
					gRemoveMoney.uiMoneyRemoving -= 1000;
				}
				break;
			case M_100:
				if( gRemoveMoney.uiMoneyRemoving >= 100 )
				{
					gRemoveMoney.uiMoneyRemaining += 100;
					gRemoveMoney.uiMoneyRemoving -= 100;
				}
				break;
			case M_10:
				if( gRemoveMoney.uiMoneyRemoving >= 10 )
				{
					gRemoveMoney.uiMoneyRemaining += 10;
					gRemoveMoney.uiMoneyRemoving -= 10;
				}
				break;
		}

		RenderItemDescriptionBox( );
		for( i=0; i<OLD_MAX_ATTACHMENTS_101; i++ )
		{
			MarkAButtonDirty( guiMoneyButtonBtn[ i ] );
		}
		// HEADROCK HAM 4: Mark Tab Buttons Dirty for UDB
		if (UsingEDBSystem())
		{
			MarkAButtonDirty( giInvDescTabButton[ 0 ] );
			MarkAButtonDirty( giInvDescTabButton[ 1 ] );
			MarkAButtonDirty( giInvDescTabButton[ 2 ] );
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void RemoveMoney()
{
	if( gRemoveMoney.uiMoneyRemoving != 0 )
	{
		//CHRISL: If what we have in the cursor is more money, just add to what we have in the cursor.
		if (gpItemPointer != NULL && gpItemPointer->exists() == true && gpItemPointer->usItem != MONEY) {
			//ADB oops, let's not overwrite what's on the cursor!
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Unable to split money due to having an item on your cursor." );

			//after this returns it will call DeleteItemDescriptionBox which will totally destroy the remaining money
			//if the uiMoneyRemaining is 0, so reset it, because we aren't moving anything!
			gRemoveMoney.uiMoneyRemaining = gRemoveMoney.uiTotalAmount;
			gRemoveMoney.uiMoneyRemoving = 0;

			return;
		}
		//if we are in the shop keeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			INVENTORY_IN_SLOT InvSlot;

			InvSlot.fActive = TRUE;
			InvSlot.sItemIndex = MONEY;
			InvSlot.bSlotIdInOtherLocation = -1;

			//Create an item to get the money that is being removed
			CreateMoney(gRemoveMoney.uiMoneyRemoving, &InvSlot.ItemObject );

			//Set the amount thast is being removed
			InvSlot.ubIdOfMercWhoOwnsTheItem = gpItemDescSoldier->ubProfile;

			//if we are removing money from the players account
			if( gfAddingMoneyToMercFromPlayersAccount )
			{
				(*gpItemDescObject)[0]->data.money.uiMoneyAmount = gRemoveMoney.uiMoneyRemoving;

				//take the money from the player
				AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, gpSMCurrentMerc->ubProfile, GetWorldTotalMin() , -(INT32)( gRemoveMoney.uiMoneyRemoving ) );
			}
			else {
				//Remove the money from the money in the pocket
				(*gpItemDescObject)[0]->data.money.uiMoneyAmount = gRemoveMoney.uiMoneyRemaining;
			}

			gMoveingItem = InvSlot;

			gItemPointer = InvSlot.ItemObject;
			gpItemPointer = &gItemPointer;
			gpItemPointerSoldier = gpSMCurrentMerc;

			// Set mouse
			SetSkiCursor( EXTERN_CURSOR );

			//Restrict the cursor to the proper area
			RestrictSkiMouseCursor();
		}
		else
		{
			if(gpItemPointer->exists() == true)
			{
				CreateMoney( gRemoveMoney.uiMoneyRemoving, &gTempObject );
				gItemPointer.AddObjectsToStack(gTempObject, -1, 0, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
				gpItemPointer = &gItemPointer;
			}
			else
			{
				CreateMoney( gRemoveMoney.uiMoneyRemoving, &gItemPointer );
				gpItemPointer = &gItemPointer;
			}
			//Asign the soldier to be the currently selected soldier
			gpItemPointerSoldier = gpItemDescSoldier;

			//Remove the money from the money in the pocket
			//if we are removing money from the players account
			if( gfAddingMoneyToMercFromPlayersAccount )
			{
				(*gpItemDescObject)[0]->data.money.uiMoneyAmount = gRemoveMoney.uiMoneyRemoving;

				//take the money from the player
				AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, gpSMCurrentMerc->ubProfile, GetWorldTotalMin() , -(INT32)(gRemoveMoney.uiMoneyRemoving) );
			}
			else
				(*gpItemDescObject)[0]->data.money.uiMoneyAmount = gRemoveMoney.uiMoneyRemaining;



			if( guiCurrentItemDescriptionScreen == MAP_SCREEN )
			{
				// Set mouse
				guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
				gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

				MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
				MSYS_SetCurrentCursor( EXTERN_CURSOR );
				fMapInventoryItem=TRUE;
				fTeamPanelDirty=TRUE;
			}

		}
	}

//	if( gfAddingMoneyToMercFromPlayersAccount )
//		gfAddingMoneyToMercFromPlayersAccount = FALSE;
}


void GetHelpTextForItem( STR16 pzStr, OBJECTTYPE *pObject, SOLDIERTYPE *pSoldier, int subObject )
{
	CHAR16	pStr[ 500 ];
	UINT16	usItem = pObject->usItem;
	INT32	iNumAttachments = 0;
	INT16	sValue;
	FLOAT	fWeight;

	if( pSoldier != NULL )
	{
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
		{
			swprintf( pStr, L"" );
			swprintf( pzStr, L"%s", pStr );
			return;
		}
	}

/* 2007-05-27, Sergeant_Kolja: code temporarily added for tracking the 
   SKI Tony inventory crash.
   Remove when fixed!
 */
# if defined( _DEBUG )
	if ( ((*pObject)[0]->data.gun.ubGunAmmoType >= MAXITEMS) )
  {
    DebugMsg(TOPIC_JA2, DBG_LEVEL_1, String( "corrupted pObject (%S) found in GetHelpTextForItem()", (usItem<MAXITEMS) ? Item[usItem].szItemName : L"???" ));
  	ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"corrupted pObject (%s) found in GetHelpTextForItem()"    , (usItem<MAXITEMS) ? Item[usItem].szItemName : L"???" );
    DebugBreak();
    AssertMsg( 0, "GetHelpTextForItem() would crash" );
  }
#endif
    
	if ( pObject->exists() == true )
	{
		// Retrieve the status of the items
		//CHRISL: If looking at an item in stack popup, show just that item
		//CHRISL: Also, determine weight differently if we're looking at a single item in a stack
		if(subObject == -1){
			// Find the minimum status value - not just the first one
			if(Item[pObject->usItem].usItemClass != IC_AMMO)
				(*pObject)[0]->data.objectStatus = __min((*pObject)[0]->data.objectStatus, 100);
			sValue = (*pObject)[0]->data.objectStatus;

			for(INT16 i = 1; i < pObject->ubNumberOfObjects; i++){
				if(Item[pObject->usItem].usItemClass != IC_AMMO)
					(*pObject)[ i ]->data.objectStatus = __min((*pObject)[ i ]->data.objectStatus, 100);
				if((*pObject)[ i ]->data.objectStatus < sValue)
				{
					sValue = (*pObject)[ i ]->data.objectStatus;
				}
			}
			//get item weight
			fWeight = (float)(CalculateObjectWeight( pObject )) / 10;
			subObject = 0;
		}
		else {
			if(Item[pObject->usItem].usItemClass != IC_AMMO)
				(*pObject)[subObject]->data.objectStatus = __min((*pObject)[subObject]->data.objectStatus, 100);
			sValue = (*pObject)[subObject]->data.objectStatus;
			fWeight = (float)(pObject->GetWeightOfObjectInStack(subObject)) / 10;
		}

		if ( !gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ] ) // metric units not enabled
		{
			fWeight = fWeight * 2.2f;
		}

		if ( fWeight < 0.1 )
		{
			fWeight = 0.1f;
		}

		switch( Item[ usItem ].usItemClass )
		{
		case MONEY:
			//Money
			{
				swprintf( pStr, L"%ld", (*pObject)[subObject]->data.money.uiMoneyAmount );
				InsertCommasForDollarFigure( pStr );
				InsertDollarSignInToString( pStr );
			}
			break;

		case IC_MONEY:
			// alternate money like silver or gold
			//if ( Item[ usItem ].usItemClass == IC_MONEY )
			{
				CHAR16		pStr2[20];
				swprintf( pStr2, L"%ld", (*pObject)[subObject]->data.money.uiMoneyAmount );
				InsertCommasForDollarFigure( pStr2 );
				InsertDollarSignInToString( pStr2 );

				swprintf( pStr, L"%s (%ls)", ItemNames[ usItem ], pStr2 );
			}
			break;

		case IC_GUN:
			{
				//Calculate AP's
				CHAR16 apStr[20];
				CHAR16 apStr2[20];
				INT16 ubAttackAPs = BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], pObject, pSoldier );

				if ( Weapon[ usItem ].NoSemiAuto )
				{
					swprintf( apStr, L"-" );
				}
				else
				{
					swprintf( apStr, L"%d", ubAttackAPs );
				}

				if (GetShotsPerBurst(pObject) > 0)
				{
					swprintf( apStr2, L" / %d", ubAttackAPs + CalcAPsToBurst( APBPConstants[DEFAULT_APS], pObject, NULL ) );
					wcscat( apStr, apStr2 );
				}
				else
				{
					wcscat( apStr, L" / -" );
				}

				if (GetAutofireShotsPerFiveAPs(pObject) > 0)
				{
					swprintf( apStr2, L" / %d", ubAttackAPs + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], pObject, 3, NULL ) );
					wcscat( apStr, apStr2 );
				}
				else
				{
					wcscat( apStr, L" / -" );
				}

				// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
				FLOAT accuracyheatmultiplicator = 1.0;
				if ( gGameOptions.fWeaponOverheating )
				{
					FLOAT overheatdamagepercentage = GetGunOverheatDamagePercentage( pObject );
					FLOAT accuracymalus = (FLOAT)((max(1.0, overheatdamagepercentage) - 1.0) * 0.1);
					accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
				}

				//Info for weapons
				//swprintf( pStr, L"%s (%s) [%d%%]\n%s %d\n%s %d\n%s %d (%d)\n%s %s\n%s %1.1f %s",

					INT8 accuracy = (UsingNewCTHSystem()==true?Weapon[ usItem ].nAccuracy:Weapon[ usItem ].bAccuracy);
					accuracy *= accuracyheatmultiplicator;
					#ifdef CHINESE
						swprintf( pStr, ChineseSpecString4,
					#else
						swprintf( pStr, L"%s (%s) [%d%%]\n%s %d\n%s %d\n%s %d (%d)\n%s (%d) %s\n%s %1.1f %s",
					#endif
					
					
					ItemNames[ usItem ],
					AmmoCaliber[ Weapon[ usItem ].ubCalibre ],
					sValue,
					gWeaponStatsDesc[ 9 ],		//Accuracy String
					accuracy,
					gWeaponStatsDesc[ 11 ],		//Damage String
					GetDamage(pObject),
					gWeaponStatsDesc[ 10 ],		//Range String
					gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ] ? GunRange( pObject, NULL )/10 : GunRange( pObject, NULL ),	 // SANDRO - added argument		//Modified Range
					gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ] ? GetModifiedGunRange(usItem)/10 : GetModifiedGunRange(usItem),	//Gun Range
					gWeaponStatsDesc[ 6 ],		//AP String
					(Weapon[ usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction( pObject )) / 100),    // Ready AP's
					apStr,						//AP's
					gWeaponStatsDesc[ 12 ],		//Weight String
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);
			}
			break;

		case IC_LAUNCHER:
			{
				//Calculate AP's
				CHAR16 apStr[20];
				CHAR16 apStr2[20];
				INT16 ubAttackAPs = BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], pObject, pSoldier );

				swprintf( apStr, L"%d", ubAttackAPs );

				if (GetShotsPerBurst(pObject) > 0)
				{
					swprintf( apStr2, L" / %d", ubAttackAPs + CalcAPsToBurst( APBPConstants[DEFAULT_APS], pObject, NULL ) );
					wcscat( apStr, apStr2 );
				}
				else
				{
					wcscat( apStr, L" / -" );
				}

				if (GetAutofireShotsPerFiveAPs(pObject) > 0)
				{
					swprintf( apStr2, L" / %d", ubAttackAPs + CalcAPsToAutofire( APBPConstants[DEFAULT_APS], pObject, 3, NULL ) );
					wcscat( apStr, apStr2 );
				}
				else
				{
					wcscat( apStr, L" / -" );
				}

				// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
				FLOAT accuracyheatmultiplicator = 1.0;
				if ( gGameOptions.fWeaponOverheating )
				{
					FLOAT overheatdamagepercentage = GetGunOverheatDamagePercentage( pObject );
					FLOAT accuracymalus = (FLOAT)((max(1.0, overheatdamagepercentage) - 1.0) * 0.1);
					accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
				}

				//Info for weapons
					INT8 accuracy = (UsingNewCTHSystem()==true?Weapon[ usItem ].nAccuracy:Weapon[ usItem ].bAccuracy);
					accuracy *= accuracyheatmultiplicator;

					#ifdef CHINESE
						swprintf( pStr, L"%s [%d%ге]\n%s %d\n%s %d\n%s %d (%d)\n%s %s\n%s %1.1f %s",
					#else
						swprintf( pStr, L"%s [%d%%]\n%s %d\n%s %d\n%s %d (%d)\n%s %s\n%s %1.1f %s",
					#endif

					ItemNames[ usItem ],
					sValue,
					gWeaponStatsDesc[ 9 ],		//Accuracy String
					accuracy,
					gWeaponStatsDesc[ 11 ],		//Damage String
					GetDamage(pObject),
					gWeaponStatsDesc[ 10 ],		//Range String
					gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ] ? GunRange( pObject, NULL )/10 : GunRange( pObject, NULL ),	 // SANDRO - added argument		//Modified Range
					gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ] ? GetModifiedGunRange(usItem)/10 : GetModifiedGunRange(usItem),	//Gun Range
					gWeaponStatsDesc[ 6 ],		//AP String
					apStr,						//AP's
					gWeaponStatsDesc[ 12 ],		//Weight String
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);
			}
			break;

		case IC_BLADE:
		case IC_THROWING_KNIFE:
		case IC_PUNCH:
			{
				#ifdef CHINESE
					swprintf( pStr, ChineseSpecString5,
				#else
					swprintf( pStr, L"%s [%d%%]\n%s %d\n%s %d\n%s %1.1f %s",
				#endif

					ItemNames[ usItem ],
					sValue,
					gWeaponStatsDesc[ 11 ],					//Damage String
					GetDamage(pObject), 					//Melee damage
					gWeaponStatsDesc[ 6 ],					//AP String
					BaseAPsToShootOrStab( APBPConstants[DEFAULT_APS], APBPConstants[DEFAULT_AIMSKILL], pObject, pSoldier ), //AP's
					gWeaponStatsDesc[ 12 ],					//Weight String
					fWeight,								//Weight
					GetWeightUnitString()					//Weight units
					);
			}
			break;

		case IC_AMMO:
			{
				// The next is for ammunition which gets the measurement 'rnds'
				swprintf( pStr, New113Message[MSG113_AMMO_SPEC_STRING],
					ItemNames[ usItem ],		//Item long name
					(*pObject)[subObject]->data.ubShotsLeft,	//Shots left
					gWeaponStatsDesc[ 12 ],		//Weight String
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);

				//Lal: do not delete, commented out for next version
				//swprintf( pStr, L"%s %s %s %d [%d rnds]\n%s %1.1f %s",
				//	AmmoCaliber[ Magazine[ Item[usItem].ubClassIndex ].ubCalibre ],			//Ammo calibre
				//	AmmoTypes[Magazine[ Item[usItem].ubClassIndex ].ubAmmoType].ammoName,	//Ammo type
				//	MagNames[Magazine[ Item[usItem].ubClassIndex ].ubMagType],				//Magazine type
				//	Magazine[ Item[usItem].ubClassIndex ].ubMagSize,						//Magazine capacity
				//	(*pObject)[0]->data.ubShotsLeft,	//Shots left
				//	gWeaponStatsDesc[ 12 ],		//Weight String
				//	fWeight,					//Weight
				//	GetWeightUnitString()		//Weight units
				//	);
			}
			break;

		case IC_GRENADE:
		case IC_BOMB:
			// explosives
			{
				// HEADROCK HAM 3.6: Can now use negative modifier.
				//UINT16 explDamage = (UINT16)( Explosive[Item[ usItem ].ubClassIndex].ubDamage + ( (double) Explosive[Item[ usItem ].ubClassIndex].ubDamage / 100) * gGameExternalOptions.ubExplosivesDamageMultiplier );
				//UINT16 explStunDamage = (UINT16)( Explosive[Item[ usItem ].ubClassIndex].ubStunDamage + ( (double) Explosive[Item[ usItem ].ubClassIndex].ubStunDamage / 100) * gGameExternalOptions.ubExplosivesDamageMultiplier );
				UINT16 explDamage = (UINT16) GetModifiedExplosiveDamage( Explosive[Item[ usItem ].ubClassIndex].ubDamage );
				UINT16 explStunDamage = (UINT16) GetModifiedExplosiveDamage( Explosive[Item[ usItem ].ubClassIndex].ubStunDamage );

				#ifdef CHINESE
					swprintf( pStr, ChineseSpecString5,
				#else
					swprintf( pStr, L"%s [%d%%]\n%s %d\n%s %d\n%s %1.1f %s",
				#endif

					ItemNames[ usItem ],
					sValue,
					gWeaponStatsDesc[ 11 ],		//Damage String
					explDamage,
					gWeaponStatsDesc[ 13 ],		//Stun Damage String
					explStunDamage,				//Stun Damage
					gWeaponStatsDesc[ 12 ],		//Weight String
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);
			}
			break;

		case IC_ARMOUR:
			{
				//Armor
				INT32 iProtection = EffectiveArmour( pObject );

				switch( Armour[ Item[ usItem ].ubClassIndex ].ubArmourClass )
				{
				case( ARMOURCLASS_HELMET ):
					iProtection = 15 * iProtection / Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection;
					break;

				case( ARMOURCLASS_VEST ):
					iProtection = 65 * iProtection / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
					break;

				case( ARMOURCLASS_LEGGINGS ):
					iProtection = 25 * iProtection / Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection;
					break;

				case( ARMOURCLASS_PLATE ):
					iProtection = 65 * iProtection / ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
					break;
				}

				#ifdef CHINESE
					swprintf( pStr, ChineseSpecString6,
				#else
					swprintf( pStr, L"%s [%d%%]\n%s %d%% (%d/%d)\n%s %d%%\n%s %1.1f %s",
				#endif

				
					ItemNames[ usItem ],		//Item long name
					sValue,						//Item condition
					pInvPanelTitleStrings[ 4 ],	//Protection string
					iProtection,				//Protection rating in % based on best armor
					Armour[ Item[ usItem ].ubClassIndex ].ubProtection, //Protection (raw data)
					Armour[ Item[ usItem ].ubClassIndex ].ubProtection * sValue / 100,
					pInvPanelTitleStrings[ 3 ],	//Camo string
					GetCamoBonus(pObject)+GetUrbanCamoBonus(pObject)+GetDesertCamoBonus(pObject)+GetSnowCamoBonus(pObject),	//Camo bonus
					gWeaponStatsDesc[ 12 ],		//Weight string
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);
			}
			break;


		case IC_MISC:
		case IC_MEDKIT:
		case IC_KIT:
		case IC_FACE:
		default:
			{
				// The final, and typical case, is that of an item with a percent status
				#ifdef CHINESE
					swprintf( pStr, ChineseSpecString7,
				#else
					swprintf( pStr, L"%s [%d%%]\n%s %1.1f %s",
				#endif
					ItemNames[ usItem ],		//Item long name
					sValue,						//Item condition
					gWeaponStatsDesc[ 12 ],		//Weight String
					fWeight,					//Weight
					GetWeightUnitString()		//Weight units
					);
			}
			break;
		}


		// Fingerprint ID (Soldier Name)
		if ( ( Item[pObject->usItem].fingerprintid ) && (*pObject)[subObject]->data.ubImprintID < NO_PROFILE )
		{
			CHAR16		pStr2[20];
			swprintf( pStr2, L" [%s]", gMercProfiles[ (*pObject)[subObject]->data.ubImprintID ].zNickname );
			wcscat( pStr, pStr2 );
		}


		// Add attachment string....
		CHAR16	attachString[ 300 ];
		memset(attachString,0,sizeof(attachString));
		for (attachmentList::iterator iter = (*pObject)[subObject]->attachments.begin(); iter != (*pObject)[subObject]->attachments.end(); ++iter) {
			if(iter->exists()){

				//Break off if it's too long.
				if(wcslen(attachString)>270){
					wcscat( attachString, L"\n...." );
					break;
				}

				iNumAttachments++;
				
				if ( iNumAttachments == 1 )
				{
					swprintf( attachString, L"\n \n%s:\n", Message[ STR_ATTACHMENTS ] );
				}
				else
				{
					wcscat( attachString, L"\n" );
				}

				wcscat( attachString, ItemNames[ iter->usItem ] );
			}
		}


		if ( iNumAttachments > 0 )
		{
			wcscat( pStr, attachString);
			//wcscat( pStr, pMessageStrings[ MSG_END_ATTACHMENT_LIST ] );
		}

	}
	else
	{
		swprintf( pStr, L"" );
	}

	// Copy over...
	swprintf( pzStr, L"%s", pStr );
}






void CancelItemPointer( )
{
	// ATE: If we have an item pointer end it!
	if ( gpItemPointer != NULL )
	{
		if ( gbItemPointerSrcSlot != NO_SLOT )
		{
			// Place it back in our hands!
			PlaceObject( gpItemPointerSoldier, gbItemPointerSrcSlot, gpItemPointer );

			// ATE: This could potnetially swap!
			// Make sure # of items is 0, if not, auto place somewhere else...
			if ( gpItemPointer->exists() == true )
			{
				if ( !AutoPlaceObject( gpItemPointerSoldier, gpItemPointer, FALSE ) )
				{
					// Alright, place of the friggen ground!
					AddItemToPool( gpItemPointerSoldier->sGridNo, gpItemPointer, 1, gpItemPointerSoldier->pathing.bLevel, 0 , -1 );
					NotifySoldiersToLookforItems( );
				}
			}
		}
		else
		{
			// We drop it here.....
			AddItemToPool( gpItemPointerSoldier->sGridNo, gpItemPointer, 1, gpItemPointerSoldier->pathing.bLevel, 0 , -1 );
			NotifySoldiersToLookforItems( );
		}
		EndItemPointer( );
	}
}


BOOLEAN LoadItemCursorFromSavedGame( HWFILE hFile )
{
	ITEM_CURSOR_SAVE_INFO		SaveStruct;

	// Load structure
	if ( !SaveStruct.Load(hFile) )
	{
		return( FALSE );
	}

	// Now set things up.....
	// Copy object
	gItemPointer = (SaveStruct.ItemPointerInfo);

	// Copy soldier ID
	if ( SaveStruct.ubSoldierID == NOBODY )
	{
		gpItemPointerSoldier = NULL;
	}
	else
	{
		gpItemPointerSoldier = MercPtrs[ SaveStruct.ubSoldierID ];
	}

	// Inv slot
	gbItemPointerSrcSlot = SaveStruct.ubInvSlot;

	// Boolean
	if ( SaveStruct.fCursorActive )
	{
		gpItemPointer = &( gItemPointer );
		ReEvaluateDisabledINVPanelButtons( );
	}
	else
	{
		gpItemPointer = NULL;
	}

	return( TRUE );
}

BOOLEAN SaveItemCursorToSavedGame( HWFILE hFile )
{
	ITEM_CURSOR_SAVE_INFO		SaveStruct;

	// Setup structure;
	SaveStruct.ItemPointerInfo = gItemPointer;

	// Soldier
	if ( gpItemPointerSoldier != NULL )
	{
		SaveStruct.ubSoldierID = gpItemPointerSoldier->ubID;
	}
	else
	{
		SaveStruct.ubSoldierID = NOBODY;
	}

	// INv slot
	SaveStruct.ubInvSlot = gbItemPointerSrcSlot;

	// Boolean
	if ( gpItemPointer != NULL )
	{
		SaveStruct.fCursorActive = TRUE;
	}
	else
	{
		SaveStruct.fCursorActive = FALSE;
	}

	// save locations of watched points
	if( !SaveStruct.Save(hFile) )
	{
		return( FALSE );
	}

	// All done...

	return( TRUE );
}



void UpdateItemHatches()
{
	SOLDIERTYPE *pSoldier = NULL;

  if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
  {
		if ( fShowInventoryFlag && bSelectedInfoChar >= 0 )
		{
			pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
		}
	}
	else
	{
		pSoldier = gpSMCurrentMerc;
	}

	if ( pSoldier != NULL )
	{
		ReevaluateItemHatches( pSoldier, ( BOOLEAN ) ( gpItemPointer == NULL ) );
	}
}

/**
 * jackaians: copied from InitializeItemPickupMenu
 */
BOOLEAN InitializeStealItemPickupMenu( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, ITEM_POOL *pItemPool, UINT8 ubCount)
{
  VOBJECT_DESC		VObjectDesc;
	CHAR8			ubString[48];
	INT16			sCenX, sCenY, sX, sY, sCenterYVal;
	INT8 bZLevel	=pOpponent->pathing.bLevel;
	INT32 sGridNo	=pOpponent->sGridNo;
	INT32			cnt;
	gpOpponent		=pOpponent;
	gfStealing		=TRUE;

	// Erase other menus....
	EraseInterfaceMenus( TRUE );

	// Make sure menu is located if not on screen
	LocateSoldier( pOpponent->ubID, FALSE );

	gItemPickupMenu.initialize();

	//Set item pool value
	gItemPickupMenu.pItemPool	= pItemPool;

	InterruptTime();
	PauseGame();
	LockPauseState( 18 );
	// Pause timers as well....
	PauseTime( TRUE );

	// Alrighty, cancel lock UI if we havn't done so already
	UnSetUIBusy( pSoldier->ubID );

	// Change to INV panel if not there already...
	gfSwitchPanel = TRUE;
	gbNewPanel = SM_PANEL;
	gubNewPanelParam = (UINT8)pSoldier->ubID;

	gItemPickupMenu.ubTotalItems = ubCount;

	// Determine # of slots per page
	if ( gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
	{
		gItemPickupMenu.bNumSlotsPerPage = NUM_PICKUP_SLOTS;
	}
	else
	{
		gItemPickupMenu.bNumSlotsPerPage = gItemPickupMenu.ubTotalItems;
	}

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\itembox.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &(gItemPickupMenu.uiPanelVo) ) );

	// Memalloc selection array...
	 gItemPickupMenu.pfSelectedArray = (BOOLEAN *)MemAlloc(( sizeof( UINT8 ) * gItemPickupMenu.ubTotalItems ) );
	// seto to 0
	memset( gItemPickupMenu.pfSelectedArray, 0, ( sizeof( UINT8 ) * gItemPickupMenu.ubTotalItems ) );

	// Calcualate dimensions
	CalculateItemPickupMenuDimensions( );

	// Get XY
	{
		// First get mouse xy screen location		
		if(!TileIsOutOfBounds(sGridNo))
		{
			sX = gusMouseXPos;
			sY = gusMouseYPos;
		}
		else
		{
			 sX = 0;
			 sY = 0;
		}

		// CHECK FOR LEFT/RIGHT
		if ( ( sX + gItemPickupMenu.sWidth ) > SCREEN_WIDTH )
		{
			sX = SCREEN_WIDTH - gItemPickupMenu.sWidth - ITEMPICK_START_X_OFFSET;
		}
		else
		{
			sX = sX + ITEMPICK_START_X_OFFSET;
		}

		// Now check for top
		// Center in the y
		sCenterYVal = gItemPickupMenu.sHeight / 2;

		sY -= sCenterYVal;

		if ( sY < gsVIEWPORT_WINDOW_START_Y )
		{
			sY = gsVIEWPORT_WINDOW_START_Y;
		}

		// Check for bottom
		if ( ( sY + gItemPickupMenu.sHeight ) > (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT) )
		{
			sY = (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT) - gItemPickupMenu.sHeight;
		}

	}

	// Set some values
	gItemPickupMenu.sX				= sX;
	gItemPickupMenu.sY				= sY;
	gItemPickupMenu.bCurSelect		= 0;
	gItemPickupMenu.pSoldier		= pSoldier;
	gItemPickupMenu.fHandled		= FALSE;
	gItemPickupMenu.sGridNo			= sGridNo;
	gItemPickupMenu.bZLevel			= bZLevel;
	gItemPickupMenu.fAtLeastOneSelected = FALSE;
	gItemPickupMenu.fAllSelected	= FALSE;

	//Load images for buttons
	FilenameForBPP("INTERFACE\\itembox.sti", ubString );
	gItemPickupMenu.iUpButtonImages			= LoadButtonImage( ubString, -1,5,-1,10,-1 );
	gItemPickupMenu.iDownButtonImages		=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 7, -1, 12, -1 );
	gItemPickupMenu.iAllButtonImages		=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 6, -1,11, -1 );
	gItemPickupMenu.iCancelButtonImages		=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 8, -1, 13, -1 );
	gItemPickupMenu.iOKButtonImages			=	UseLoadedButtonImage( gItemPickupMenu.iUpButtonImages, -1, 4, -1, 9, -1 );


	// Build a mouse region here that is over any others.....
	/*MSYS_DefineRegion( &(gItemPickupMenu.BackRegion ), (INT16)( 532 ), (INT16)( 367 ), (INT16)( 640 ),(INT16)( 480 ), MSYS_PRIORITY_HIGHEST,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK ); */

	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &(gItemPickupMenu.BackRegion ), (INT16)( iScreenWidthOffset + 532 ), (INT16)( iScreenHeightOffset + 367 ), (INT16)( SCREEN_WIDTH ),(INT16)( SCREEN_HEIGHT ), MSYS_PRIORITY_HIGHEST,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );

	// Add region
	MSYS_AddRegion( &(gItemPickupMenu.BackRegion ) );


	// Build a mouse region here that is over any others.....
	MSYS_DefineRegion( &(gItemPickupMenu.BackRegions ), (INT16)(gItemPickupMenu.sX ), (INT16)(gItemPickupMenu.sY), (INT16)(gItemPickupMenu.sX + gItemPickupMenu.sWidth ),(INT16)( gItemPickupMenu.sY + gItemPickupMenu.sHeight ), MSYS_PRIORITY_HIGHEST,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	// Add region
	MSYS_AddRegion( &(gItemPickupMenu.BackRegions ) );


	// Create buttons
	if ( gItemPickupMenu.bNumSlotsPerPage == NUM_PICKUP_SLOTS && gItemPickupMenu.ubTotalItems > NUM_PICKUP_SLOTS )
	{
		gItemPickupMenu.iUpButton	= QuickCreateButton( gItemPickupMenu.iUpButtonImages, (INT16)(sX + ITEMPICK_UP_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_UP_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupScrollUp );

		SetButtonFastHelpText( gItemPickupMenu.iUpButton, ItemPickupHelpPopup[ 1 ] );


		gItemPickupMenu.iDownButton	= QuickCreateButton( gItemPickupMenu.iDownButtonImages, (INT16)(sX + ITEMPICK_DOWN_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_DOWN_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupScrollDown );

		SetButtonFastHelpText( gItemPickupMenu.iDownButton, ItemPickupHelpPopup[ 3 ] );

	}


	gItemPickupMenu.iOKButton	= QuickCreateButton( gItemPickupMenu.iOKButtonImages, (INT16)(sX + ITEMPICK_OK_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_OK_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupOK );
	SetButtonFastHelpText( gItemPickupMenu.iOKButton, ItemPickupHelpPopup[ 0 ] );


	gItemPickupMenu.iAllButton	= QuickCreateButton( gItemPickupMenu.iAllButtonImages, (INT16)(sX + ITEMPICK_ALL_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_ALL_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupAll );
	SetButtonFastHelpText( gItemPickupMenu.iAllButton, ItemPickupHelpPopup[ 2 ] );

	gItemPickupMenu.iCancelButton	= QuickCreateButton( gItemPickupMenu.iCancelButtonImages, (INT16)(sX + ITEMPICK_CANCEL_X), (INT16)(sY + gItemPickupMenu.sButtomPanelStartY + ITEMPICK_CANCEL_Y),
	 									 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
	 									 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)ItemPickupCancel );
	SetButtonFastHelpText( gItemPickupMenu.iCancelButton, ItemPickupHelpPopup[ 4 ] );


	DisableButton( gItemPickupMenu.iOKButton );



	// Create regions...
	sCenX = gItemPickupMenu.sX;
	sCenY = gItemPickupMenu.sY + ITEMPICK_GRAPHIC_Y;

	for ( cnt = 0; cnt < gItemPickupMenu.bNumSlotsPerPage; cnt++ )
	{
		// Build a mouse region here that is over any others.....
		MSYS_DefineRegion( &(gItemPickupMenu.Regions[cnt]), (INT16)(sCenX ), (INT16)(sCenY + 1), (INT16)(sCenX + gItemPickupMenu.sWidth ),(INT16)( sCenY + ITEMPICK_GRAPHIC_YSPACE ), MSYS_PRIORITY_HIGHEST,
							 CURSOR_NORMAL, ItemPickMenuMouseMoveCallback, ItemPickMenuMouseClickCallback );
		// Add region
		MSYS_AddRegion( &(gItemPickupMenu.Regions[cnt]) );
		MSYS_SetRegionUserData( &(gItemPickupMenu.Regions[cnt]), 0, cnt );

		sCenY += ITEMPICK_GRAPHIC_YSPACE;
	}

	//Save dirty rect
	//gItemPickupMenu.iDirtyRect = RegisterBackgroundRect( BGND_FLAG_PERMANENT | BGND_FLAG_SAVERECT, NULL, gItemPickupMenu.sX, gItemPickupMenu.sY, (INT16)(gItemPickupMenu.sX + gItemPickupMenu.sWidth ) , (INT16)(gItemPickupMenu.sY + gItemPickupMenu.sHeight ) );


	SetupPickupPage(0);
	gfInItemPickupMenu = TRUE;

	// Ignore scrolling
	gfIgnoreScrolling = TRUE;

	HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
	gubSelectSMPanelToMerc = pSoldier->ubID;
	ReEvaluateDisabledINVPanelButtons( );
	DisableTacticalTeamPanelButtons( TRUE );

	//gfSMDisableForItems = TRUE;
	return( TRUE );

}
void DeletePool(ITEM_POOL *pItemPool)
{
	ITEM_POOL *pItemPoolToDelete;
	while(pItemPool!=NULL)
	{
		pItemPoolToDelete=pItemPool;
		pItemPool=pItemPool->pNext;
		MemFree(pItemPoolToDelete);
	}
}

void ItemDescTabButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	switch (btn->UserData[0])
	{
		case 0:
			gubDescBoxPage = 0;
			InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );
			RenderItemDescriptionBox();

			if (giItemDescAmmoButton > -1)
				MarkAButtonDirty( giItemDescAmmoButton ); // Required for tactical screen
			
			ItemDescTabButtonOn( 0 );
			ItemDescTabButtonOff( 1 );
			ItemDescTabButtonOff( 2 );
			break;
		case 1:
			gubDescBoxPage = 1;
			InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );
			RenderItemDescriptionBox();

			if (giItemDescAmmoButton > -1)
				MarkAButtonDirty( giItemDescAmmoButton ); // Required for tactical screen

			ItemDescTabButtonOff( 0 );
			ItemDescTabButtonOn( 1 );
			ItemDescTabButtonOff( 2 );
			break;
		case 2:
			gubDescBoxPage = 2;
			InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );
			RenderItemDescriptionBox();

			if (giItemDescAmmoButton > -1)
				MarkAButtonDirty( giItemDescAmmoButton ); // Required for tactical screen

			ItemDescTabButtonOff( 0 );
			ItemDescTabButtonOff( 1 );
			ItemDescTabButtonOn( 2 );
			break;
	}
}

void ItemDescTabButtonOn( UINT8 ubItemDescTabButtonIndex )
{
	Assert( ubItemDescTabButtonIndex < 3 );

	// if button doesn't exist, return
	if ( giInvDescTabButton[ ubItemDescTabButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giInvDescTabButton[ ubItemDescTabButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giInvDescTabButton[ ubItemDescTabButtonIndex ] ]->uiFlags |= BUTTON_CLICKED_ON;

	MarkAButtonDirty( giInvDescTabButton[ ubItemDescTabButtonIndex ] );
}

void ItemDescTabButtonOff( UINT8 ubItemDescTabButtonIndex )
{
	Assert( ubItemDescTabButtonIndex < 3 );

	// if button doesn't exist, return
	if ( giInvDescTabButton[ ubItemDescTabButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giInvDescTabButton[ ubItemDescTabButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giInvDescTabButton[ ubItemDescTabButtonIndex ] ]->uiFlags &= ~(BUTTON_CLICKED_ON);

	MarkAButtonDirty( giInvDescTabButton[ ubItemDescTabButtonIndex ] );
}

void ItemDescAdvButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if (reason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		switch (btn->UserData[0])
		{
			case 0:
				gubDescBoxLine--;
				InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );
				RenderItemDescriptionBox();
				// Required for tactical screen

				if (giItemDescAmmoButton > -1)
					MarkAButtonDirty( giItemDescAmmoButton ); 

				for (INT8 x = 0; x < 3; x++)
				{
					if (giInvDescTabButton[x] != -1)
					{
						// HEADROCK HAM 4: Makes sure the Tab Buttons are always redrawn on top.
						MarkAButtonDirty( giInvDescTabButton[ x ] );
					}
				}
				ItemDescAdvButtonOff( 0 );
				ItemDescTabButtonOff( 1 );
				ItemDescAdvButtonCheck();
				break;
			case 1:
				gubDescBoxLine++;
				InternalInitEDBTooltipRegion( gpItemDescObject, guiCurrentItemDescriptionScreen );
				RenderItemDescriptionBox();
				 // Required for tactical screen
				if (giItemDescAmmoButton > -1)
					MarkAButtonDirty( giItemDescAmmoButton );

				for (INT8 x = 0; x < 3; x++)
				{
					if (giInvDescTabButton[x] != -1)
					{
						// HEADROCK HAM 4: Makes sure the Tab Buttons are always redrawn on top.
						MarkAButtonDirty( giInvDescTabButton[ x ] );
					}
				}
				ItemDescTabButtonOff( 0 );
				ItemDescTabButtonOff( 1 );
				ItemDescAdvButtonCheck();
				break;
		}
	}
}

void ItemDescAdvButtonOn( UINT8 ubItemDescAdvButtonIndex )
{
	Assert( ubItemDescAdvButtonIndex < 2 );

	// if button doesn't exist, return
	if ( giInvDescAdvButton[ ubItemDescAdvButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giInvDescAdvButton[ ubItemDescAdvButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giInvDescAdvButton[ ubItemDescAdvButtonIndex ] ]->uiFlags |= BUTTON_CLICKED_ON;

	MarkAButtonDirty( giInvDescAdvButton[ 0 ] );
	MarkAButtonDirty( giInvDescAdvButton[ 1 ] );
}

void ItemDescAdvButtonOff( UINT8 ubItemDescAdvButtonIndex )
{
	Assert( ubItemDescAdvButtonIndex < 2 );

	// if button doesn't exist, return
	if ( giInvDescAdvButton[ ubItemDescAdvButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giInvDescAdvButton[ ubItemDescAdvButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giInvDescAdvButton[ ubItemDescAdvButtonIndex ] ]->uiFlags &= ~(BUTTON_CLICKED_ON);

	MarkAButtonDirty( giInvDescAdvButton[ 0 ] );
	MarkAButtonDirty( giInvDescAdvButton[ 1 ] );
}

void ItemDescAdvButtonCheck( void )
{
	if (gubDescBoxLine <= 0)
	{
		DisableButton( giInvDescAdvButton[0] );
	}
	else
	{
		EnableButton( giInvDescAdvButton[0] );
	}

	if (NUM_UDB_ADV_LINES < gubDescBoxTotalAdvLines - gubDescBoxLine)
	{
		EnableButton( giInvDescAdvButton[1] );
	}
	else
	{
		DisableButton( giInvDescAdvButton[1] );
	}
}

// HEADROCK HAM 5: Item Transformations callback. The user has clicked on the Big Item
// picture, possibly indicating that he wishes to transform this item into something else.
void ItemDescTransformRegionCallback( MOUSE_REGION *pRegion, INT32 reason )
{
	if (reason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// A left-click triggers the Transformation Menu. The game scans memory for any Transformations
		// that can be performed on this item, and creates a menu with all the options listed.

		// HEADROCK HAM 5: Disable shopkeeper item transformations entirely. 
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			return;
		}

		///////////////////////////
		// CONSTRUCT POPUP MENU
		///////////////////////////
		// If we already have a popup, destroy it first. This ensures we get a fresh menu each time.
		if ( gfItemDescTransformPopupInitialized )
		{
			delete(gItemDescTransformPopup);
			gItemDescTransformPopup = NULL;
			gfItemDescTransformPopupInitialized = FALSE;
			gfItemDescTransformPopupVisible = FALSE;
			gfSkipDestroyTransformPopup = FALSE;
		}
		// create a popup
		gItemDescTransformPopup = new POPUP("TRANSFORMATION MENU POPUP");	// at this point the name is used mainly for debug output
		
		// add a callback that lets the keyboard handler know we're done (and ready to pop up again)
		gItemDescTransformPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,void>( &TransformationMenuPopup_Hide ) );
		
		BOOLEAN fFoundTransformations = false;

		/*
		// Test the item for Gun Jams. If it's a gun and is jammed, add a Transformation Menu option to unjam it.
		if ( Item[gpItemDescObject->usItem].usItemClass == IC_GUN && !EXPLOSIVE_GUN( gpItemDescObject->usItem ) ) 
		{ 
			// Check ammo status
			if ((*gpItemDescObject)[0]->data.gun.bGunAmmoStatus < 0) 
			{
				// Add option
				POPUP_OPTION *pOption = new POPUP_OPTION(&std::wstring( L"Unjam" ), new popupCallbackFunction<void,void>( &TransformationMenuPopup_Unjam ));
				gItemDescTransformPopup->addOption( *pOption );
				fFoundTransformations = true;
			}
		}
		*/

		// Ammocrates may be split into magazines of any size available in the game. But not in combat.
		if ( Item[gpItemDescObject->usItem].usItemClass == IC_AMMO && Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubMagType >= AMMO_BOX && !(gTacticalStatus.uiFlags & INCOMBAT) )
		{
			BOOLEAN fCrateInPool = FALSE;

			// Before we continue, lets check whether our object is in the sector inventory.
			// Is the sector inventory open?
			if (fShowMapInventoryPool)
			{
				// Is our object currently in the pool?
				for (UINT32 x = 0; x < pInventoryPoolList.size(); x++)
				{
					if (pInventoryPoolList[x].object.exists())
					{
						if (&(pInventoryPoolList[x].object) == gpItemDescObject)
						{
							// Aha! In that case, all transformations will be done directly at the sector pool,
							// with multiple results ending on the ground rather than in the inventory.
							fCrateInPool = TRUE;
							break;
						}
					}
				}
			}

			if (fCrateInPool)
			{
				for (UINT16 x = 0; x < MAXITEMS; x++)
				{
					if ( Item[x].usItemClass & IC_AMMO )
					{
						// If this magazine has the same caliber and ammotype as the crate, has a smaller size, and is
						// not an ammo crate itself...
						if ( Magazine[Item[x].ubClassIndex].ubCalibre == Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubCalibre &&
							Magazine[Item[x].ubClassIndex].ubAmmoType == Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubAmmoType &&
							Magazine[Item[x].ubClassIndex].ubMagSize < Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubMagSize &&
							Magazine[Item[x].ubClassIndex].ubMagType < AMMO_BOX )
						{
							UINT16 usMagSize = Magazine[Item[x].ubClassIndex].ubMagSize;

							CHAR16 MenuRowText[300];
							swprintf( MenuRowText, gzTransformationMessage[ 7 ], usMagSize );
							// Generate a new option for the menu
							POPUP_OPTION *pOption = new POPUP_OPTION(&std::wstring( MenuRowText ), new popupCallbackFunction<void,UINT16>( TransformationMenuPopup_SplitCrate, x ) );
							// Add the option to the menu.
							gItemDescTransformPopup->addOption( *pOption );
							// Set this flag so we know we have at least one Transformation available.
							fFoundTransformations = true;
						}
					}
				}
			}
			else
			{
				POPUP_OPTION *pOption = new POPUP_OPTION(&std::wstring( gzTransformationMessage[ 6 ] ), new popupCallbackFunction<void,void>( TransformationMenuPopup_SplitCrateInInventory ) );
				gItemDescTransformPopup->addOption( *pOption );
				fFoundTransformations = true;
			}
		}

		// Scan the Transformation list for the current item. Create pop-up options as required.
		INT32 iTransformIndex = -1;
		for (INT32 x = 0; x < MAXITEMS; x++)
		{
			if (Transform[x].usItem == -1)
			{
				break;
			}
			if (Transform[x].usItem == gpItemDescObject->usItem)
			{
				iTransformIndex++;

				CHAR16 MenuRowText[300];
				if ( Transform[x].usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED )
				{
					swprintf (MenuRowText, L"%s (%d AP)", Transform[x].szMenuRowText, Transform[x].usAPCost );
				}
				else
				{
					swprintf (MenuRowText, Transform[x].szMenuRowText);
				}

				// Generate a new option for the menu
				POPUP_OPTION *pOption = new POPUP_OPTION(&std::wstring( MenuRowText ), new popupCallbackFunction<void,TransformInfoStruct*>( &TransformationMenuPopup_Transform, &Transform[x] ) );
				// Set the function that tests whether it's valid at the moment.
				pOption->setAvail(new popupCallbackFunction<bool,TransformInfoStruct*>( &TransformationMenuPopup_TestValid, &Transform[x] ));
				// Add the option to the menu.
				gItemDescTransformPopup->addOption( *pOption );
				// Set this flag so we know we have at least one Transformation available.
				fFoundTransformations = true;
			}
		}
		if (!fFoundTransformations)
		{
			POPUP_OPTION * pOption = new POPUP_OPTION( &std::wstring( gzTransformationMessage[ 0 ] ), new popupCallbackFunction<void,TransformInfoStruct*>( &TransformationMenuPopup_Transform, NULL ) );
			pOption->setAvail(new popupCallbackFunction<bool,TransformInfoStruct*>( &TransformationMenuPopup_TestValid, NULL ));
			gItemDescTransformPopup->addOption( *pOption );
		}

		UINT16 usPosX = (UINT16)((ITEMDESC_ITEM_X + (ITEMDESC_ITEM_X + ITEMDESC_ITEM_WIDTH)) / 2);
		UINT16 usPosY = (UINT16)((ITEMDESC_ITEM_Y + (ITEMDESC_ITEM_Y + ITEMDESC_ITEM_HEIGHT)) / 2);
		if (pRegion->MouseXPos > ITEMDESC_ITEM_X && pRegion->MouseXPos < (ITEMDESC_ITEM_X + ITEMDESC_ITEM_WIDTH))
		{
			usPosX = pRegion->MouseXPos;
		}
		if (pRegion->MouseYPos > ITEMDESC_ITEM_Y && pRegion->MouseYPos < (ITEMDESC_ITEM_Y + ITEMDESC_ITEM_HEIGHT))
		{
			usPosY = pRegion->MouseYPos;
		}
		gItemDescTransformPopup->setPosition( usPosX, usPosY );

		gfItemDescTransformPopupInitialized = TRUE;
		gfItemDescTransformPopupVisible = TRUE;

		// In compliance with current bugs, if there's a bullet icon, hide it to prevent overlaps.
		if (giItemDescAmmoButton > -1)	
		{
			HideButton(giItemDescAmmoButton);
		}

		gItemDescTransformPopup->show();

		// Now that the popup is initialized, lets set the help text for each line. Note that we have to do it here
		// (rather than before) because only now are the MOUSE_REGIONs ready to receive help text at all!!
		INT32 iNumOptions = 0;
		for (INT32 x = 0; x < MAXITEMS; x++)
		{
			if (Transform[x].usItem == -1)
			{
				break;
			}
			if (Transform[x].usItem == gpItemDescObject->usItem)
			{
				SetRegionFastHelpText( &(gItemDescTransformPopup->MenuRegion[iNumOptions]), Transform[x].szTooltipText );
				iNumOptions++;
			}
		}

	}
	else if (reason == MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		// Behave like the background region, closing the box.
		OBJECTTYPE *pTemp = gpItemDescPrevObject;
		DeleteItemDescriptionBox( );
		if (pTemp != NULL)
		{
			InternalInitItemDescriptionBox( pTemp, gsInvDescX, gsInvDescY, 0, gpItemDescSoldier );
		}	
	}
}

//////////////////////////////////////////////////////////////////////
// HEADROCK HAM 5: Callback Functions for the Transformation Menu
//////////////////////////////////////////////////////////////////////

// This function handles hiding the menu.
void TransformationMenuPopup_Hide(void)
{
	// If an Eject Ammo button exists for the current DB item, then restore it to view.
	if (giItemDescAmmoButton > -1)	
	{
		ShowButton(giItemDescAmmoButton);
	}

	// Signal the renderer to stop drawing this menu.
	gfItemDescTransformPopupVisible = FALSE;
	fMapPanelDirty = TRUE;
}

// This function handles callback when one of the options on the Transformation menu is clicked.
// This function takes care of all activity that is not directly related to the transformation data, the transformed
// object, or the soldier performing the transformation.
void TransformationMenuPopup_Transform( TransformInfoStruct * Transform)
{
	// If the item is in a stack, ask for confirmation.
	if (gpItemDescObject->ubNumberOfObjects > 1)
	{
		//Ask for confirmation
		gTransformInProgress = Transform;
		guiTransformInProgressPrevScreen = guiCurrentScreen;
		CHAR16 pStr[500];
		swprintf( pStr, gzTransformationMessage[ 5 ], gpItemDescObject->ubNumberOfObjects );
		DoScreenIndependantMessageBox( pStr, MSG_BOX_FLAG_YESNO, ConfirmTransformationMessageBoxCallBack );
	}
	else
	{
		TransformFromItemDescBox( Transform );
	}
}

BOOLEAN TransformationMenuPopup_TestValid(TransformInfoStruct * Transform)
{
	if (Transform == NULL)
	{
		return false;
	}
	else
	{
		UINT16 usAPCost = Transform->usAPCost;
		INT32 iBPCost = Transform->iBPCost;

		if (EnoughPoints( gpItemDescSoldier, usAPCost, iBPCost, false ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// HEADROCK HAM 5: This Transformation Menu Callback will attempt to unjam a jammed gun. If the
// soldier lacks APs to do so, it will fail with a screen message. Otherwise the gun is unjammed,
// causing the normal effects.
void TransformationMenuPopup_Unjam()
{
	/*
	if ( Item[gpItemDescObject->usItem].usItemClass != IC_GUN || EXPLOSIVE_GUN( gpItemDescObject->usItem ) || (*gpItemDescObject)[0]->data.gun.bGunAmmoStatus > 0) 
	{ 
		AssertMsg( 0, "Transformation Menu allowed us to attempt to unjam an unjammed gun. This is illegal!" );
		return;
	}

	if(EnoughPoints(gpItemDescSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM], FALSE))
	{
		DeductPoints(gpItemDescSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM]);
		INT8 bChanceMod;
		
		if ( Weapon[gpItemDescObject->usItem].EasyUnjam )
			bChanceMod = 100;
		else
			bChanceMod = (INT8) (GetReliability( gpItemDescObject )* 4);
		
		int iResult = SkillCheck( gpItemDescSoldier, UNJAM_GUN_CHECK, bChanceMod); 
		
		if (iResult > 0) 
		{ 
			// yay! unjammed the gun 
			(*gpItemDescObject)[0]->data.gun.bGunAmmoStatus *= -1; 
		 
			// MECHANICAL/DEXTERITY GAIN: Unjammed a gun 
			
			if (bChanceMod < 100) // don't give exp for unjamming an easily unjammable gun
			{
				StatChange( gpItemDescSoldier, MECHANAMT, 5, FALSE ); 
				StatChange( gpItemDescSoldier, DEXTAMT, 5, FALSE ); 
			}
		 
			RenderItemDescriptionBox();
		 
			return;
		} 
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s does not have enough APs to unjam this weapon.", gpItemDescSoldier->name);
	}
	*/
}

// HEADROCK HAM 5: This is a handler for transforming an ammo crate into magazines. We get the itemnumber of the
// magazine to create, and proceed to create one magazine after the other. They are placed either into the
// carrying soldier's inventory, or into the sector inventory if it is there.
void TransformationMenuPopup_SplitCrate( UINT16 usMagazineItem )
{
	if (gItemDescTransformPopup != NULL && gfItemDescTransformPopupInitialized == TRUE)
	{
		gItemDescTransformPopup->hide();
	}

	OBJECTTYPE MagazineObject;
	UINT16 usShotsLeft = (*gpItemDescObject)[gubItemDescStatusIndex]->data.ubShotsLeft;
	UINT16 usMagazineSize = Magazine[Item[usMagazineItem].ubClassIndex].ubMagSize;

	UINT8 ubAmmoType = Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubAmmoType;
	UINT8 ubCaliber = Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubCalibre;

	// Simple. Just create as many clips as you can, drop them all to the sector inventory.
	UINT32 uiNumMagazinesToCreate = (usShotsLeft / usMagazineSize) + ((usShotsLeft % usMagazineSize) > 0);

	for (UINT32 x = 0; x < uiNumMagazinesToCreate; x++)
	{
		UINT16 usBulletsInMag = __min( usShotsLeft, usMagazineSize );
		
		MagazineObject.initialize();
		CreateAmmo(usMagazineItem, &MagazineObject, usBulletsInMag);
		AutoPlaceObjectToWorld( gpItemDescSoldier, &MagazineObject, true );

		if (&MagazineObject != NULL)
		{
			DeleteObj( &MagazineObject );
		}

		usShotsLeft -= usBulletsInMag;
	}		

	CHAR16 pStr[500];
	swprintf( pStr, gzTransformationMessage[ 8 ], Item[gpItemDescObject->usItem].szItemName, uiNumMagazinesToCreate, usMagazineSize );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr );

	OBJECTTYPE *gTempCrate = gpItemDescObject;

	gfSkipDestroyTransformPopup = TRUE;
	DeleteItemDescriptionBox();
	gfSkipDestroyTransformPopup = FALSE;

	gTempCrate->RemoveObjectAtIndex(gubItemDescStatusIndex);
}

void TransformationMenuPopup_SplitCrateInInventory( )
{
	if (gItemDescTransformPopup != NULL && gfItemDescTransformPopupInitialized == TRUE)
	{
		gItemDescTransformPopup->hide();
	}

	OBJECTTYPE MagazineObject;
	UINT16 usShotsLeft = (*gpItemDescObject)[gubItemDescStatusIndex]->data.ubShotsLeft;
	UINT16 usOrigShotsLeft = usShotsLeft;

	UINT8 ubAmmoType = Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubAmmoType;
	UINT8 ubCaliber = Magazine[Item[gpItemDescObject->usItem].ubClassIndex].ubCalibre;

	// Drop into the soldier's inventory.
	if (gpItemDescSoldier == NULL)
	{
		return;
	}
	for (INT16 sPocket = BIGPOCK1POS; sPocket < NUM_INV_SLOTS && usShotsLeft > 0; sPocket++)
	{
		// To cut down on processing time, skip any pockets containing an item that will not appear on the
		// possible splits list.
		BOOLEAN fEmptyPocketOk = FALSE;
		BOOLEAN fMagazineInPocketOk = FALSE;
		BOOLEAN fHalfEmptyMagazineInPocketOk = FALSE; //MM: changed this so that half empty magazines will be topped off.  TODO: eventually apply similar logic to when a crate is clicked on a gun as well.

		UINT16 usMagazineToCreate = 0;

		OBJECTTYPE *pObjInPocket = &(gpItemDescSoldier->inv[sPocket]);
		if ( pObjInPocket->exists() == true )
		{
			if (Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubAmmoType == ubAmmoType &&
				Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubCalibre == ubCaliber &&
				Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubMagType < AMMO_BOX)
			{
				fMagazineInPocketOk = TRUE;
				usMagazineToCreate = pObjInPocket->usItem;
				for (INT8 cnt = 0; cnt < pObjInPocket->ubNumberOfObjects; ++cnt)
				{
					if ((*pObjInPocket)[cnt]->data.ubShotsLeft < Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubMagSize)
					{
						fHalfEmptyMagazineInPocketOk = TRUE;
						break;
					}
				}
			}
		}
		else
		{
			fEmptyPocketOk = TRUE;
		}

		if (fMagazineInPocketOk)
		{
			UINT8 ubCapacity = ItemSlotLimit(pObjInPocket, sPocket, gpItemDescSoldier);
			ubCapacity -= pObjInPocket->ubNumberOfObjects;

			for ( UINT16 x = 0; x < ubCapacity && usShotsLeft > 0; x++)
			{
				UINT16 usBulletsInMag = __min( usShotsLeft, Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubMagSize );
		
				MagazineObject.initialize();
				CreateAmmo(usMagazineToCreate, &MagazineObject, usBulletsInMag);
				PlaceObject( gpItemDescSoldier, (INT8)sPocket, &MagazineObject );

				if (&MagazineObject != NULL)
				{
					DeleteObj( &MagazineObject );
				}

				usShotsLeft -= usBulletsInMag;
			}

			if (fHalfEmptyMagazineInPocketOk && usShotsLeft > 0)
			{
				for (INT8 cnt = 0; cnt < pObjInPocket->ubNumberOfObjects ;++cnt)
				{
				 	if ((*pObjInPocket)[cnt]->data.ubShotsLeft < Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubMagSize)
					{
						UINT16 bulletsToAdd = Magazine[Item[pObjInPocket->usItem].ubClassIndex].ubMagSize - (*pObjInPocket)[cnt]->data.ubShotsLeft;
						
						if ( usShotsLeft < bulletsToAdd )
							bulletsToAdd = usShotsLeft;

						(*pObjInPocket)[cnt]->data.ubShotsLeft += bulletsToAdd;
						usShotsLeft -= bulletsToAdd;
					}
				}
			}
		}
		else if (fEmptyPocketOk)
		{
			// Check whether any magazine of any size can fit into this slot. Find the type that can fit the
			// most rounds in per slot.

			UINT8 ubBestStackCapacity = 0;
			UINT32 uiBestRoundCapacity = 0;
			UINT16 usBestItem = 0;

			for (UINT16 x = 0; x < MAXITEMS; x++)
			{
				if (Item[x].usItemClass & IC_AMMO)
				{
					if (Magazine[Item[x].ubClassIndex].ubAmmoType == ubAmmoType &&
						Magazine[Item[x].ubClassIndex].ubCalibre == ubCaliber &&
						Magazine[Item[x].ubClassIndex].ubMagType < AMMO_BOX )
					{
						UINT16 usTempMagazineSize = Magazine[Item[x].ubClassIndex].ubMagSize;
						OBJECTTYPE TempMagObject;
						TempMagObject.initialize();
						CreateAmmo(x, &TempMagObject, usTempMagazineSize);

						UINT8 ubCapacity = ItemSlotLimit(&TempMagObject, sPocket, gpItemDescSoldier);
						
						if ((UINT32)(ubCapacity * usTempMagazineSize) > uiBestRoundCapacity)
						{
							uiBestRoundCapacity = ubCapacity * usTempMagazineSize;
							ubBestStackCapacity = ubCapacity;
							usBestItem = x;
						}

						DeleteObj( &TempMagObject );
					}
				}
			}

			if (usBestItem > 0)
			{
				UINT16 usMagazineSize = Magazine[Item[usBestItem].ubClassIndex].ubMagSize;
				for (UINT16 y = 0; y < ubBestStackCapacity && usShotsLeft > 0; y++)
				{
					UINT16 usBulletsInMag = __min( usShotsLeft, usMagazineSize );

					MagazineObject.initialize();
					CreateAmmo( usBestItem, &MagazineObject, usBulletsInMag );
					PlaceObject( gpItemDescSoldier, (INT8)sPocket, &MagazineObject );

					if (&MagazineObject != NULL)
					{
						DeleteObj( &MagazineObject );
					}

					usShotsLeft -= usBulletsInMag;
				}
			}
		}
	}
	
	if (usOrigShotsLeft > usShotsLeft)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzTransformationMessage[ 9 ], Item[gpItemDescObject->usItem].szItemName, gpItemDescSoldier->name );
	}
	else
	{
		// Until we determine whether a box would be better...
		ScreenMsg( FONT_ORANGE, MSG_INTERFACE, gzTransformationMessage[ 10 ], gpItemDescSoldier->name );
		//CHAR16 pStr[300];
		//swprintf( pStr, gzTransformationMessage[ 10 ], gpItemDescSoldier->name );
		//DoScreenIndependantMessageBox( pStr, MSG_BOX_FLAG_OK, NULL );
	}

	if (usShotsLeft > 0)
	{
		(*gpItemDescObject)[gubItemDescStatusIndex]->data.ubShotsLeft = usShotsLeft;
		RenderItemDescriptionBox();
	}
	else
	{
		OBJECTTYPE *gTempCrate = gpItemDescObject;

		gfSkipDestroyTransformPopup = TRUE;
		DeleteItemDescriptionBox();
		gfSkipDestroyTransformPopup = FALSE;

		gTempCrate->RemoveObjectAtIndex(gubItemDescStatusIndex);
		//DeleteObj( gTempCrate );
	}
}


void TransformFromItemDescBox( TransformInfoStruct * Transform)
{

	// Hide the Transform Popup menu, we don't need it anymore. It will be destroyed and rebuilt next time we click the
	// Transformation region anyway.
	if (gItemDescTransformPopup != NULL && gfItemDescTransformPopupInitialized == TRUE)
	{
		gItemDescTransformPopup->hide();
	}

	// Record the item's original class. If it's a gun item, we may need to manually delete the 
	// eject ammo button!
	UINT32 uiOrigClass = Item[gpItemDescObject->usItem].usItemClass;
	BOOLEAN fWasAttachment = gfItemDescObjectIsAttachment;

	// Carry out the transformation on this item, using the data we've received.
	gpItemDescObject->TransformObject( gpItemDescSoldier, gubItemDescStatusIndex, Transform, gpItemDescPrevObject );

	// Check to see if we need to manually erase the ammo button.
	UINT32 uiNewClass = Item[gpItemDescObject->usItem].usItemClass;
	BOOLEAN fEraseAmmoButton = FALSE;
	if (uiOrigClass & IC_GUN && !(uiNewClass & IC_GUN))
	{
		fEraseAmmoButton = TRUE;
	}

	// We're going to shut down and reinitialize the DescBox now, to ensure that we get the DescBox for the resulting
	// item instead of the old one.

	// Save previous ItemDesc-related parameters, before deleting the box (they get erased...)
	OBJECTTYPE *pTemp = gpItemDescObject;
	OBJECTTYPE *pTempParent = NULL;
	BOOLEAN fTempIsAttachment = FALSE;
	OBJECTTYPE *pTempAttachment = NULL;
	// If the object was attached and still is, we'll need to save the attachment details now before we destroy the
	// description box. That way we can reopen it with all the data intact.
	if (fWasAttachment && gfItemDescObjectIsAttachment)
	{
		pTempParent = gpItemDescPrevObject;
		fTempIsAttachment = gfItemDescObjectIsAttachment;
		pTempAttachment = gpItemDescOrigAttachmentObject;
	}

	// This flag tells the deletion function not to destroy the Transform Popup Menu - we need it to stay alive through
	// this.
	gfSkipDestroyTransformPopup = TRUE;

	// DELETE THE BOX!
	DeleteItemDescriptionBox( );

	/*if (fEraseAmmoButton)
	{
		// Transformation from gun to non-gun. Erase ammo button manually.
		UnloadButtonImage( giItemDescAmmoButtonImages );
		RemoveButton( giItemDescAmmoButton );
	}*/

	// Unflag. Next closure of the DescBox will destroy the menu as normal.
	gfSkipDestroyTransformPopup = FALSE;

	if (pTemp->usItem > 0 && // Open only if the item is still valid.
		!(fWasAttachment && !fTempIsAttachment)) // Do not reopen the box if the item was an attachment and has been removed from its gun.
	{
		// Restore previous settings
		gpItemDescObject = pTemp;
		gpItemDescPrevObject = pTempParent;
		gfItemDescObjectIsAttachment = fTempIsAttachment;
		gpItemDescOrigAttachmentObject = pTempAttachment;

		// RESTART THE BOX!
		InternalInitItemDescriptionBox( gpItemDescObject, gsInvDescX, gsInvDescY, 0, gpItemDescSoldier );
	}
}

// This is a callback function for the box that asks you whether or not you want to transform all items
// in a stack.
void ConfirmTransformationMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		UINT32 iTempScreen = guiCurrentScreen;
		guiCurrentScreen = guiTransformInProgressPrevScreen;
		TransformFromItemDescBox( gTransformInProgress );
		guiCurrentScreen = iTempScreen;
		guiTransformInProgressPrevScreen = 0;
	}
}