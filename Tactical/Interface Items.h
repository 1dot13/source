#ifndef __INTERFACE_ITEMS_H
#define __INTERFACE_ITEMS_H

#include "items.h"
#include "Handle Items.h"
#include "popup_class.h"

// DEFINES FOR ITEM SLOT SIZES IN PIXELS
#define		BIG_INV_SLOT_WIDTH				61
#define		BIG_INV_SLOT_HEIGHT				22
#define		SM_INV_SLOT_WIDTH				30
#define		SM_INV_SLOT_HEIGHT				23
#define		ATTACH_SLOT_WIDTH				32
#define		ATTACH_SLOT_BIG_WIDTH			64
#define		VEST_INV_SLOT_WIDTH				43
#define		VEST_INV_SLOT_HEIGHT			24
#define		LEGS_INV_SLOT_WIDTH				43
#define		LEGS_INV_SLOT_HEIGHT			24
#define		HEAD_INV_SLOT_WIDTH				43
#define		HEAD_INV_SLOT_HEIGHT			24

#define		NUM_UDB_GEN_LINES	12
#define		NUM_UDB_ADV_LINES	13			// Flugente FTW 1.1: 12->13
#define		GEN_SEC_REGION_ENTRIES 24		// 6 lines x 4 icons
 
// HEADROCK HAM 4: These rectangle definitions are used in UDB to determine locations for icons, data, and tooltip
// regions. They replace the plethora of region data provided in EDB.
typedef struct
{
	INT16 sLeft;
	INT16 sTop;
	INT16 sRight;
	INT16 sBottom;
} INV_DESC_REGIONS;

extern INV_DESC_REGIONS gItemDescLBEBackground[4]; // Coordinates for displaying LBE background image
extern INV_DESC_REGIONS gItemDescGenHeaderRegions[3]; // Header text regions for various parts of the General Tab
extern INV_DESC_REGIONS gItemDescGenIndexRegions[3][4]; // Index text regions for various parts of the General Tab
extern INV_DESC_REGIONS gItemDescGenRegions[NUM_UDB_GEN_LINES * 2][4]; // Data regions, 4 sub-columns each
extern INV_DESC_REGIONS gItemDescGenSecondaryRegions[GEN_SEC_REGION_ENTRIES]; // Secondary data regions, 6 lines x 4 icons
extern INV_DESC_REGIONS gItemDescTextRegions[9]; // Main description regions
extern INV_DESC_REGIONS gItemDescAdvIndexRegions[1][4];
extern INV_DESC_REGIONS gItemDescAdvRegions[NUM_UDB_ADV_LINES][4]; // Advanced data regions, 4 sub-columns each

// ODB arrays
extern INV_DESC_REGIONS gODBItemDescRegions[4][8]; // Four regions of eight sub-regions each.

//CHRISL: moved from Interface Items.cpp
typedef struct
{
	INT16		sX;
	INT16		sY;
	INT16		sValDx;

} INV_DESC_STATS;
typedef struct
{
	INT16		sX;
	INT16		sY;
	INT16		sHeight;
	INT16		sWidth;
	INT16		sBarDx;
	INT16		sBarDy;

} INV_ATTACHXY;
typedef struct
{
	UINT32	uiTotalAmount;
	UINT32	uiMoneyRemaining;
	UINT32	uiMoneyRemoving;

} REMOVE_MONEY;
typedef struct
{
	UINT16 x;
	UINT16 y;
} MoneyLoc;
void ItemDescCallback( MOUSE_REGION * pRegion, INT32 iReason );
#define		ITEMDESC_FONT					BLOCKFONT2
#define		INV_BAR_DX						5
#define		INV_BAR_DY						21
#define		ITEM_STATS_WIDTH				26
#define		ITEM_STATS_HEIGHT				8
#define		EXCEPTIONAL_GUN_DAMAGE			40
#define		EXCEPTIONAL_MELEE_DAMAGE		40
#define		EXCEPTIONAL_WEIGHT				20
#define		EXCEPTIONAL_RANGE				400
#define		EXCEPTIONAL_MAGAZINE			50
#define		EXCEPTIONAL_AP_COST				5
#define		EXCEPTIONAL_BURST_SIZE			5
#define		EXCEPTIONAL_RELIABILITY			2
#define		EXCEPTIONAL_REPAIR_EASE			2
#define		EXCEPTIONAL_ACCURACY			4
#define		ITEMDESC_FONTHIGHLIGHT			FONT_MCOLOR_WHITE
#define		ITEMDESC_FONTPOSITIVE		FONT_MCOLOR_LTGREEN
#define		ITEMDESC_FONTNEGATIVE		FONT_MCOLOR_LTRED

//CHRISL: extern'd for EDB project
extern INT16	gsInvDescX;
extern INT16	gsInvDescY;
extern UINT8	gubItemDescStatusIndex;

extern INT16	BULLET_SING_X;
extern INT16	BULLET_SING_Y;
extern INT16	BULLET_BURST_X;
extern INT16	BULLET_BURST_Y;
extern INT16	ITEMDESC_CALIBER_X;
extern INT16	ITEMDESC_CALIBER_Y;
extern INT16	ITEMDESC_CALIBER_WIDTH;
extern INT16	ITEMDESC_NAME_X;
extern INT16	ITEMDESC_NAME_Y;
extern INT16	ITEMDESC_ITEM_X;
extern INT16	ITEMDESC_ITEM_Y;
extern INT16	ITEMDESC_ITEM_STATUS_X;
extern INT16	ITEMDESC_ITEM_STATUS_Y;
extern INT16	ITEMDESC_ITEM_STATUS_HEIGHT;
extern INT16	ITEMDESC_AMMO_X;
extern INT16	ITEMDESC_AMMO_Y;
extern INT16	ITEMDESC_DONE_X;
extern INT16	ITEMDESC_DONE_Y;

// A STRUCT USED INTERNALLY FOR INV SLOT REGIONS
typedef struct
{
	BOOLEAN		fBigPocket;
	INT16			sBarDx;
	INT16			sBarDy;
	INT16			sWidth;
	INT16			sHeight;
	INT16			sX;				// starts at 0, gets set via InitInvSlotInterface()
	INT16			sY;				// starts at 0, gets set via InitInvSlotInterface()
} INV_REGIONS;

// USED TO SETUP REGION POSITIONS, ETC
typedef struct
{
	INT16			sX;
	INT16			sY;
} INV_REGION_DESC;


class OLD_ITEM_CURSOR_SAVE_INFO_101
{
public:
	OLD_OBJECTTYPE_101	ItemPointerInfo;
	UINT8				ubSoldierID;
	UINT8				ubInvSlot;
	BOOLEAN				fCursorActive;
	INT8				bPadding[5];

};


class ITEM_CURSOR_SAVE_INFO
{
public:
	ITEM_CURSOR_SAVE_INFO& operator=(OLD_ITEM_CURSOR_SAVE_INFO_101& src)
	{
		this->fCursorActive = src.fCursorActive;
		this->ItemPointerInfo = src.ItemPointerInfo;
		this->ubInvSlot = src.ubInvSlot;
		this->ubSoldierID = src.ubSoldierID;
		return *this;
	}
	//could use a little tidying up
	BOOLEAN Save(HWFILE hFile);
	BOOLEAN Load(HWFILE hFile);

	UINT8				ubSoldierID;
	UINT8				ubInvSlot;
	BOOLEAN				fCursorActive;
	OBJECTTYPE			ItemPointerInfo;
};

// HEADROCK HAM 5: Enums for big-item display attachment asterisks.
enum
{
	ATTACHMENT_NONE,
	ATTACHMENT_GENERAL,
	ATTACHMENT_GL,
	ATTACHMENT_RECOILREDUCTION,
	ATTACHMENT_OPTICAL,
};

// HEADROCK HAM 5: Asterisks for use with big item images
extern UINT32 guiAttachmentAsterisks;

// Itempickup stuff
BOOLEAN InitializeItemPickupMenu( SOLDIERTYPE *pSoldier, INT32 sGridNo, ITEM_POOL *pItemPool, INT16 sScreenX, INT16 sScreenY, INT8 bZLevel );
void RenderItemPickupMenu( );
void RemoveItemPickupMenu( );
void SetItemPickupMenuDirty( BOOLEAN fDirtyLevel );
BOOLEAN HandleItemPickupMenu( );
void SetPickUpMenuDirtyLevel( BOOLEAN fDirtyLevel );


// FUNCTIONS FOR INTERFACEING WITH ITEM PANEL STUFF
void INVRenderINVPanelItem( SOLDIERTYPE *pSoldier, INT16 sPocket, UINT8 fDirtyLevel );
BOOLEAN InitInvSlotInterface( INV_REGION_DESC *pRegionDesc , INV_REGION_DESC *pCamoRegion, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, MOUSE_CALLBACK INVMoveCammoCallback, MOUSE_CALLBACK INVClickCammoCallback, BOOLEAN fSetHighestPrioity );
void ShutdownInvSlotInterface( );
void GetSlotInvXY( UINT8 ubPos, INT16 *psX, INT16 *psY );
void GetSlotInvHeightWidth( UINT8 ubPos, INT16 *psWidth, INT16 *psHeight );
void HandleRenderInvSlots( SOLDIERTYPE *pSoldier, UINT8 fDirtyLevel );
void HandleNewlyAddedItems( SOLDIERTYPE *pSoldier, BOOLEAN *fDirtyLevel );
void RenderInvBodyPanel( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY );
void DisableInvRegions( BOOLEAN fDisable );

void DegradeNewlyAddedItems( );
void CheckForAnyNewlyAddedItems( SOLDIERTYPE *pSoldier );


BOOLEAN HandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, INT8 bInvPos, BOOLEAN fOn );



// THIS FUNCTION IS CALLED TO RENDER AN ITEM.
// uiBuffer - The Dest Video Surface - can only be FRAME_BUFFER or guiSAVEBUFFER
// pSoldier - used for determining whether burst mode needs display
// pObject	- Usually taken from pSoldier->inv[HANDPOS]
// sX, sY, Width, Height,	- Will Center it in the Width
// fDirtyLevel	if == DIRTYLEVEL2 will render everything
//							if == DIRTYLEVEL1 will render bullets and status only
// 
//	pubHighlightCounter - if not null, and == 2 - will display name above item
//											-	if == 1 will only dirty the name space and then set counter to 0
//	Last parameter used mainly for when mouse is over item

void RenderBulletIcon(OBJECTTYPE *pObject, UINT32 ubStatusIndex = 0);
void INVRenderItem( UINT32 uiBuffer, SOLDIERTYPE * pSoldier, OBJECTTYPE	*pObject, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, UINT8 fDirtyLevel, UINT8 *pubHighlightCounter, UINT8 ubStatusIndex, BOOLEAN fOutline, INT16 sOutlineColor, UINT8 iter = 0 );
void INVRenderSteeringWheel( UINT32 uiBuffer, UINT32 uiSteeringWheelIndex, SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, UINT8 fDirtyLevel );
// HEADROCK HAM 5: Drawing a large item pic with extra data.
void MAPINVRenderItem( UINT32 uiBuffer, SOLDIERTYPE * pSoldier, OBJECTTYPE  *pObject, UINT32 uiItemGraphicNum, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, BOOLEAN fOutline, INT16 sOutlineColor );
// CHRISL: Add a new function that will be used to render a pocket silhouette
void INVRenderSilhouette( UINT32 uiBugger, INT16 PocketIndex, INT16 SilIndex, INT16 sX, INT16 sY, INT16 sWideth, INT16 sHeight);
// CHRISL: New function to handle display of inventory quantities based on item current in cursor
void RenderPocketItemCapacity( UINT32 uiWhichBuffer, INT8 pCapacity, INT16 bPos, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT16 sX, INT16 sY );
// CHRISL: New function to display items stored in an LBENODE
void RenderLBENODEItems( OBJECTTYPE *pObj, int subObject );
// CHRISL: New function to setup GSMInvData based on game options
void InitInvData(INV_REGIONS &InvData, BOOLEAN fBigPocket, INT16 sBarDx, INT16 sBarDy, INT16 sWidth, INT16 sHeight, INT16 sX, INT16 sY);
void InitInventoryOld();
void InitInventoryNew();
void InitInventoryVehicle(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity);
void InitInventorySoldier(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity, BOOLEAN fNIVmode);



extern BOOLEAN		gfInItemDescBox;

BOOLEAN InItemDescriptionBox( );
BOOLEAN InitItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex );
BOOLEAN InternalInitItemDescriptionBox( OBJECTTYPE *pObject, INT16 sX, INT16 sY, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier, UINT8 ubPosition = NUM_INV_SLOTS );
void UpdateAttachmentTooltips( OBJECTTYPE *pObject, UINT8 ubStatusIndex );
BOOLEAN InitKeyItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex );
void RenderItemDescriptionBox( );
void HandleItemDescriptionBox( BOOLEAN *pfDirty );
void DeleteItemDescriptionBox( );
extern BOOLEAN UseNASDesc(OBJECTTYPE  *pObject);
INT16 sNASXCorrection(OBJECTTYPE * pObject);
INT16 sNASYCorrection(OBJECTTYPE * pObject);
void HandleItemDescTabButton( );

BOOLEAN InItemStackPopup( );
BOOLEAN InitItemStackPopup( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight );
BOOLEAN InSectorStackPopup( );
BOOLEAN InitSectorStackPopup( SOLDIERTYPE *pSoldier, WORLDITEM *pInventoryPoolList, INT32 ubPosition, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight );
void RenderItemStackPopup( BOOLEAN fFullRender );
void HandleItemStackPopup( );
void DeleteItemStackPopup( );
void EndItemStackPopupWithItemInHand( );


// keyring handlers
BOOLEAN InitKeyRingPopup( SOLDIERTYPE *pSoldier, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight );
void RenderKeyRingPopup( BOOLEAN fFullRender );
void InitKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback );
void InitMapKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback );
void DeleteKeyRingPopup( );


void ShutdownKeyRingInterface( void );
void ShutdownInventoryInterface( void );

BOOLEAN InKeyRingPopup( void );
void BeginKeyRingItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubKeyRingPosition );


extern OBJECTTYPE		*gpItemPointer;
extern OBJECTTYPE		gItemPointer;
extern SOLDIERTYPE		*gpItemPointerSoldier;
extern UINT16				usItemSnapCursor;
extern UINT16				us16BPPItemCyclePlacedItemColors[ 20 ];
extern BOOLEAN				gfItemPointerDifferentThanDefault;


void BeginItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubHandPos );
void InternalBeginItemPointer( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject, INT8 bHandPos );
void EndItemPointer( );
void DrawItemFreeCursor( );
void DrawItemTileCursor( );
void HideItemTileCursor( );
void InitItemInterface( );
BOOLEAN ItemCursorInLobRange( INT32 usMapPos );
BOOLEAN	 HandleItemPointerClick( INT32 usMapPos );
UINT32 GetInterfaceGraphicForItem( INVTYPE *pItem );
UINT16 GetTileGraphicForItem( INVTYPE *pItem );
BOOLEAN LoadTileGraphicForItem( INVTYPE *pItem, UINT32 *puiVo );

void GetHelpTextForItem( STR16 pzStr, OBJECTTYPE *pObject, SOLDIERTYPE *pSoldier, int subObject = -1 );

void CancelItemPointer( );

BOOLEAN LoadItemCursorFromSavedGame( HWFILE hFile );
BOOLEAN SaveItemCursorToSavedGame( HWFILE hFile );

void EnableKeyRing( BOOLEAN fEnable );

// handle compatable items for merc and map inventory
BOOLEAN HandleCompatibleAmmoUIForMapScreen( SOLDIERTYPE *pSoldier, INT32 bInvPos, BOOLEAN fOn, BOOLEAN fFromMerc );
BOOLEAN HandleCompatibleAmmoUIForMapInventory( SOLDIERTYPE *pSoldier, INT32 bInvPos, INT32 iStartSlotNumber, BOOLEAN fOn, BOOLEAN fFromMerc	);
void ResetCompatibleItemArray( );

void CycleItemDescriptionItem( INT16 sX, INT16 sY );
INT16 CycleItems( INT16 usOldItem );

BOOLEAN InitializeStealItemPickupMenu( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, ITEM_POOL *pItemPool, UINT8 ubCount);

// BOB : quick attachment selection popups
extern POPUP* gItemDescAttachmentPopups[MAX_ATTACHMENTS];	// popup list for attachment slots
extern INT32 giActiveAttachmentPopup;	// the attachment popup to display in mapscreen

// BOB : quick equip popups
extern POPUP* gEquipPopups[NUM_INV_SLOTS];	// popup list for attachment slots
extern INT32 giActiveEquipPopup;	// the attachment popup to display in mapscreen

// HEADROCK HAM 5: Item Transformation Popup
extern POPUP* gItemDescTransformPopup;
extern BOOLEAN gfItemDescTransformPopupInitialized;
extern BOOLEAN gfItemDescTransformPopupVisible;

// BOB : duh...
void DoAttachment( UINT8 subObject, INT32 iItemPos );

// THE_BOB (AKA BOB): ammo box/mag popups
void PocketPopupFull( SOLDIERTYPE *pSoldier, INT16 sPocket );	// displays all suported options
void PocketPopupDefault( SOLDIERTYPE *pSoldier, INT16 sPocket );	// displays only pocket-specific (predefined) options

POPUP * createPopupForPocket( SOLDIERTYPE *pSoldier, INT16 sPocket );	// creates a popup positioned next to the pocket

// THE_BOB 
extern UINT16 gsPocketUnderCursor;


void addArmorToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addLBEToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addWeaponsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addWeaponGroupsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addBombsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addFaceGearToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );

void addRifleGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addRocketAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addMiscToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
void addKitsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );

// Flugente: update merc info text on merc body silhouette in strategic view
void UpdateMercBodyRegionHelpText( );

#endif
