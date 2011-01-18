#ifndef __HANDLE_ITEMS_H
#define __HANDLE_ITEMS_H

#include "World Items.h"
#include "structure.h"
#include "Soldier Control.h"

#define ITEM_HANDLE_OK													1
#define ITEM_HANDLE_RELOADING										-1
#define ITEM_HANDLE_UNCONSCIOUS									-2
#define ITEM_HANDLE_NOAPS												-3
#define ITEM_HANDLE_NOAMMO											-4
#define ITEM_HANDLE_CANNOT_GETTO_LOCATION				-5
#define ITEM_HANDLE_BROKEN											-6
#define ITEM_HANDLE_NOROOM											-7
#define ITEM_HANDLE_REFUSAL											-8

// Define for code to try and pickup all items....
#define ITEM_PICKUP_ACTION_ALL									32000
#define ITEM_PICKUP_SELECTION										31000

#define ITEM_IGNORE_Z_LEVEL											-1

#define ITEMLIST_INIT_HANDLE			1
#define ITEMLIST_DISPLAY					2
#define ITEMLIST_HANDLE						3
#define ITEMLIST_END_HANDLE				4
#define ITEMLIST_HANDLE_SELECTION	5

// visibility defines
#define ANY_VISIBILITY_VALUE	-10
#define HIDDEN_ITEM -4
#define BURIED -3
#define HIDDEN_IN_OBJECT -2
#define INVISIBLE -1
#define VISIBLE 1

#define	ITEM_LOCATOR_DELAY	0x01
#define	ITEM_LOCATOR_LOCKED 0x02



// MACRO FOR DEFINING OF ITEM IS VISIBLE
#define ITEMPOOL_VISIBLE( pItemPool )			( ( pItemPool->bVisible >= 1 ) || (gTacticalStatus.uiFlags&SHOW_ALL_ITEMS) )

typedef void ( *ITEM_POOL_LOCATOR_HOOK )( void );	


class WORLDITEM;
struct LEVELNODE;

typedef struct TAG_ITEM_POOL
{	
	struct TAG_ITEM_POOL	*pNext;
	struct TAG_ITEM_POOL	*pPrev;
	
	INT32		iItemIndex;
	INT8		bVisible;
	INT8		bFlashColor;
	UINT32		uiTimerID;
	INT32		sGridNo;
	UINT8		ubLevel;
	UINT16		usFlags;
	INT8		bRenderZHeightAboveLevel;
	LEVELNODE	*pLevelNode;

} ITEM_POOL;

typedef struct 
{
	ITEM_POOL								*pItemPool;

	// Additional info for locators
	INT8										bRadioFrame;
	UINT32									uiLastFrameUpdate;
	ITEM_POOL_LOCATOR_HOOK	Callback;
	BOOLEAN									fAllocated;
	UINT8										ubFlags;

} ITEM_POOL_LOCATOR;

class SOLDIERTYPE;
INT32 HandleItem( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel, UINT16 usHandItem, BOOLEAN fFromUI );
void SoldierPickupItem( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel );
void HandleSoldierPickupItem( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel );
void HandleFlashingItems( );

BOOLEAN SoldierDropItem( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj );

void HandleSoldierThrowItem( SOLDIERTYPE *pSoldier, INT32 sGridNo );
BOOLEAN VerifyGiveItem( SOLDIERTYPE *pSoldier, SOLDIERTYPE **ppTargetSoldier );
void SoldierGiveItemFromAnimation( SOLDIERTYPE *pSoldier );
void SoldierGiveItem( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTargetSoldier, OBJECTTYPE *pObject, INT8 bInvPos );


void NotifySoldiersToLookforItems( );
void AllSoldiersLookforItems( BOOLEAN RevealRoofsAndItems );


void SoldierGetItemFromWorld( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel, BOOLEAN *pfSelectionList );

OBJECTTYPE* AddItemToPool( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID = -1 );
OBJECTTYPE* AddItemToPoolAndGetIndex( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID, INT32 * piItemIndex );
OBJECTTYPE* InternalAddItemToPool( INT32 *psGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID, INT32 * piItemIndex );

INT32 AdjustGridNoForItemPlacement( SOLDIERTYPE *pSoldier, INT32 sGridNo );
BOOLEAN	GetItemPool( INT32 usMapPos, ITEM_POOL **ppItemPool, UINT8 ubLevel );
BOOLEAN	GetItemPoolFromGround( INT32 sMapPos, ITEM_POOL **ppItemPool );
BOOLEAN DrawItemPoolList( ITEM_POOL *pItemPool, INT32 sGridNo, UINT8 bCommand, INT8 bZLevel, INT16 sXPos, INT16 sYPos );
BOOLEAN RemoveItemFromPool( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel );
BOOLEAN ItemExistsAtLocation( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel );
BOOLEAN MoveItemPools( INT32 sStartPos, INT32 sEndPos, INT8 bStartLevel, INT8 bEndLevel );

void SetItemPoolLocator( ITEM_POOL *pItemPool );
void SetItemPoolLocatorWithCallback( ITEM_POOL *pItemPool, ITEM_POOL_LOCATOR_HOOK Callback );
BOOLEAN SetItemPoolVisibilityOn( ITEM_POOL *pItemPool, INT8 bAllGreaterThan, BOOLEAN fSetLocator );
void AdjustItemPoolVisibility( ITEM_POOL *pItemPool );

void SetItemPoolVisibilityHiddenInObject( ITEM_POOL *pItemPool );
void SetItemPoolVisibilityHidden( ITEM_POOL *pItemPool );

INT32 GetItemOfClassTypeInPool( INT32 sGridNo, UINT32 uiItemClass, UINT8 ubLevel );
void RemoveItemPool( INT32 sGridNo, UINT8 ubLevel );
void RenderTopmostFlashingItems( );

void RemoveAllUnburiedItems( INT32 sGridNo, UINT8 ubLevel );


BOOLEAN DoesItemPoolContainAnyHiddenItems( ITEM_POOL *pItemPool );
BOOLEAN DoesItemPoolContainAllHiddenItems( ITEM_POOL *pItemPool );


void HandleSoldierDropBomb( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void HandleSoldierUseRemote( SOLDIERTYPE *pSoldier, INT32 sGridNo );

BOOLEAN ItemPoolOKForDisplay( ITEM_POOL *pItemPool, INT8 bZLevel );
INT16 GetNumOkForDisplayItemsInPool( ITEM_POOL *pItemPool, INT8 bZLevel );

void SoldierHandleDropItem( SOLDIERTYPE *pSoldier );

BOOLEAN LookForHiddenItems( INT32 sGridNo, INT8 ubLevel, BOOLEAN fSetLocator, INT8 bZLevel );

INT8 GetZLevelOfItemPoolGivenStructure( INT32 sGridNo, UINT8 ubLevel, STRUCTURE *pStructure );

INT8 GetLargestZLevelOfItemPool( ITEM_POOL *pItemPool );

BOOLEAN NearbyGroundSeemsWrong( SOLDIERTYPE * pSoldier, INT32 sGridNo, BOOLEAN fCheckAroundGridNo, INT32 * psProblemGridNo );
void MineSpottedDialogueCallBack( void );

extern INT32 gsBoobyTrapGridNo;
extern SOLDIERTYPE * gpBoobyTrapSoldier;
void AddBlueFlag( INT32 sGridNo, INT8 bLevel );
void RemoveBlueFlag( INT32 sGridNo, INT8 bLevel  );

// check if item is booby trapped
BOOLEAN ContinuePastBoobyTrapInMapScreen( OBJECTTYPE *pObject, SOLDIERTYPE *pSoldier );

// set off the booby trap in mapscreen
void SetOffBoobyTrapInMapScreen( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject );

void RefreshItemPools( WORLDITEM* pItemList, INT32 iNumberOfItems );

BOOLEAN HandItemWorks( SOLDIERTYPE *pSoldier, INT8 bSlot );

BOOLEAN ItemTypeExistsAtLocation( INT32 sGridNo, UINT16 usItem, UINT8 ubLevel, INT32 * piItemIndex );

INT32 FindNearestAvailableGridNoForItem( INT32 sSweetGridNo, INT8 ubRadius );

BOOLEAN CanPlayerUseRocketRifle( SOLDIERTYPE *pSoldier, BOOLEAN fDisplay );

void MakeNPCGrumpyForMinorOffense( SOLDIERTYPE * pSoldier, SOLDIERTYPE *pOffendingSoldier );

void SoldierStealItemFromSoldier( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent,ITEM_POOL *pItemPool, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel, BOOLEAN *pfSelectionList );
UINT8 StealItems(SOLDIERTYPE* pSoldier,SOLDIERTYPE* pOpponent, UINT8* ubIndexRet);

BOOLEAN MarblesExistAtLocation( INT32 sGridNo, UINT8 ubLevel, INT32 * piItemIndex );

extern ITEM_POOL *gpItemPool;//dnl ch26 210909

#endif