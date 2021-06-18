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

// silversurfer: added this to prevent tons of compiler warnings in VS2010 because of new behaviour for array initialization used in below structs
// see https://msdn.microsoft.com/en-us/library/1ywe7hcy.aspx
#pragma warning( disable : 4351 )

// Flugente: we can construct and deconstruct map structures via items.
typedef struct STRUCTURE_DECONSTRUCT {
	STRUCTURE_DECONSTRUCT()
	: usDeconstructItem( 0 ), usItemToCreate( 0 ), usCreatedItemStatus( 0 ), szTileSetDisplayName( ), szTileSetName( ), dCreationCost( 0 ) {}

	UINT16 usDeconstructItem;					// the item that has to be used to deconstruct the structure
	UINT16 usItemToCreate;						// the item that will be created when we deconstruct a structure
	UINT8  usCreatedItemStatus;					// status of the item to create
	char szTileSetDisplayName[20];				// name of this structure to the player (tileset names are obscure)
	char szTileSetName[20];						// name of the tileset
	FLOAT	dCreationCost;						// assignment cost per structure built
	std::vector<UINT8> tilevector;				// structures in the tileset that we can deconstruct
} STRUCTURE_DECONSTRUCT;

#define STRUCTURE_DECONSTRUCT_MAX		200

extern STRUCTURE_DECONSTRUCT gStructureDeconstruct[STRUCTURE_DECONSTRUCT_MAX];

typedef struct STRUCTURE_CONSTRUCT {
	STRUCTURE_CONSTRUCT()
	: usCreationItem( 0 ), usItemStatusLoss( 0 ), szTileSetDisplayName(), szTileSetName(), dCreationCost( 1.0 ), fFortifyAdjacentAdjustment( TRUE ) {}

	UINT16	usCreationItem;						// the item that will be consumed when creating the structure
	UINT8	usItemStatusLoss;					// item will lose this number of status points
	char	szTileSetDisplayName[20];			// name of this structure to the player (tileset names are obscure)
	char	szTileSetName[20];					// name of the tileset
	FLOAT	dCreationCost;						// assignment cost per structure built
	BOOLEAN fFortifyAdjacentAdjustment;			// if true, try to fit adjacent fortifications
	std::vector<UINT8> northtilevector;			// structures in the tileset we can create while facing north
	std::vector<UINT8> southtilevector;			// structures in the tileset we can create while facing south
	std::vector<UINT8> easttilevector;			// structures in the tileset we can create while facing east
	std::vector<UINT8> westtilevector;			// structures in the tileset we can create while facing west
} STRUCTURE_CONSTRUCT;

#define STRUCTURE_CONSTRUCT_MAX		200

extern STRUCTURE_CONSTRUCT gStructureConstruct[STRUCTURE_CONSTRUCT_MAX];

// Flugente: we can define what structures we can interact with (example: we can hack computers or retrieve information from a file cabinet)
typedef enum
{
	INTERACTIVE_STRUCTURE_NO_ACTION,

	INTERACTIVE_STRUCTURE_HACKABLE,
	INTERACTIVE_STRUCTURE_READFILE,
	INTERACTIVE_STRUCTURE_WATERTAP,
	INTERACTIVE_STRUCTURE_SODAMACHINE,
	INTERACTIVE_STRUCTURE_MINIGAME,

	INTERACTIVE_STRUCTURE_TYPE_MAX,
} INTERACTIVE_STRUCTURE_TYPE;

typedef struct INTERACTIVE_STRUCTURE {
	INTERACTIVE_STRUCTURE()
	: sector( -1 ), sectorlevel( -1 ), sLevel( -1 ), sActionType( INTERACTIVE_STRUCTURE_NO_ACTION ), difficulty( 0 ), luaactionid( -1 ) {}
		
	INT16	sector;
	INT8	sectorlevel;						// whether this is on the surface (0) or below (1-3)
	char	szTileSetName[20];					// name of the tileset
	std::vector<UINT16>	tileindexvector;		// possible indizes in tileset

	// We can either specify a specific location, or simply leave this empty
	// in that case, this action will apply to all locations fitting the above criteria, but a specific location definition always takes preference
	std::vector<INT32> gridnovector;			// possible gridnos
	INT8	sLevel;

	UINT16	sActionType;						// one of INTERACTIVE_STRUCTURE_TYPE
	INT32	difficulty;
	INT32	luaactionid;						// call lua with this ID after performing the usual action - perhaps something special has to happen
} INTERACTIVE_STRUCTURE;

#define INTERACTIVE_STRUCTURE_MAX		200

extern INTERACTIVE_STRUCTURE gInteractiveStructure[INTERACTIVE_STRUCTURE_MAX];
extern UINT32 gMaxInteractiveStructureRead;

// Flugente: we need to define what structures can be moved. Otherwise the player will drag around parts of the scenery.
typedef struct STRUCTURE_MOVEPOSSIBLE {
	STRUCTURE_MOVEPOSSIBLE()
		: szTileSetDisplayName(), szTileSetName() {}

	char szTileSetDisplayName[20];				// name of this structure to the player (tileset names are obscure)
	char szTileSetName[20];						// name of the tileset
	std::vector<UINT8> tilevector;				// structures in the tileset that we can deconstruct
} STRUCTURE_MOVEPOSSIBLE;

#define STRUCTURE_MOVEPOSSIBLE_MAX		200

extern STRUCTURE_MOVEPOSSIBLE gStructureMovePossible[STRUCTURE_MOVEPOSSIBLE_MAX];

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
BOOLEAN	GetItemPoolFromRoof( INT32 sMapPos, ITEM_POOL **ppItemPool );
BOOLEAN DrawItemPoolList( ITEM_POOL *pItemPool, INT32 sGridNo, UINT8 bCommand, INT8 bZLevel, INT16 sXPos, INT16 sYPos );
BOOLEAN RemoveItemFromPool( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel );
BOOLEAN ItemExistsAtLocation( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel );
BOOLEAN MoveItemPools( INT32 sStartPos, INT32 sEndPos, INT8 bStartLevel, INT8 bEndLevel );
BOOLEAN MoveItemPools_ForDragging( INT32 sStartPos, INT32 sEndPos, INT8 bStartLevel, INT8 bEndLevel );

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
void RevealAllUnburiedItems( INT32 sGridNo, UINT8 ubLevel );

BOOLEAN DoesItemPoolContainAnyHiddenItems( ITEM_POOL *pItemPool );
BOOLEAN DoesItemPoolContainAllHiddenItems( ITEM_POOL *pItemPool );


void HandleSoldierDropBomb( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void HandleSoldierUseRemote( SOLDIERTYPE *pSoldier, INT32 sGridNo );

void HandleTacticalFunctionSelection( SOLDIERTYPE *pSoldier, INT32 sGridNo );	// Flugente: in tactical, open a selection box for different actions

void HandleSoldierUseCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel );	// Flugente: handle corpses
void HandleSoldierDefuseTripwire( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT32 sItem );	// Flugente: defuse tripwire

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

// Flugente: build and deconstruct structures
INT16 GetTileSetTindexToTileSetName( INT32 asTileSetId, std::string aTileSetName );
BOOLEAN IsStructureConstructItem( UINT16 usItem, INT32 sGridNo, SOLDIERTYPE* pSoldier );	// can we construct a structure with this item?
BOOLEAN IsStructureDeconstructItem( UINT16 usItem, INT32 sGridNo, SOLDIERTYPE* pSoldier );	// can we remove a structure with this item?
BOOLEAN BuildFortification( INT32 sGridNo, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );		// build a structure, return true if sucessful
BOOLEAN RemoveFortification( INT32 sGridNo, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );		// remove a structure, return true if sucessful

// Flugente: functions for fortification
UINT8	CheckBuildFortification( INT32 sGridNo, INT8 sLevel, UINT8 usIndex, UINT32 usStructureconstructindex );
BOOLEAN	BuildFortification( INT32 sGridNo, INT8 sLevel, UINT8 usIndex, UINT32 usStructureconstructindex );

BOOLEAN	CanRemoveFortification( INT32 sGridNo, INT8 sLevel, UINT32 usStructureconstructindex );
BOOLEAN	RemoveFortification( INT32 sGridNo, INT8 sLevel, UINT32 usStructureconstructindex );

BOOLEAN	IsDragStructurePresent( INT32 sGridNo, INT8 sLevel, UINT32& arusTileType, UINT16& arusStructureNumber );
void	GetDragStructureXmlEntry( UINT32 ausTileType, UINT16 ausStructureNumber, int& arXmlVectorEntry );
BOOLEAN	RemoveStructDrag( INT32 sGridNo, INT8 sLevel, UINT32 uiTileType );
BOOLEAN	BuildStructDrag( INT32 sGridNo, INT8 sLevel, UINT32 uiTileType, UINT8 usIndex, UINT16 usSoldierID );

void UpdateFortificationPossibleAmount();
void HandleFortificationUpdate();

// get a vector of all tilesets the current sector has
std::vector<std::pair<INT16, STR16> > GetCurrentSectorTileSetVector();

// get a vector of all tilesets that are allowed to be built in this sector (the above filtered by structure construct/deconstruct basically)
std::vector<std::pair<INT16, STR16> > GetCurrentSectorAllowedFortificationTileSetVector( INT32 asTileSetId );

// get all allowed indizes for a specific tileset. 'Allowed' as in: used in our structure construct entries
std::vector<std::pair<INT16, STR16> > GetTileSetIndexVector( INT16 aKey );

std::string GetNameToTileSet( UINT8 aIndex );
INT16 GetStructureConstructIndexToTileset( INT16 aTileset );
INT16 GetStructureDeConstructIndexToTileset( INT16 aTileset );
std::set<UINT8> GetStructureConstructDirectionIndizes( INT16 aEntry, BOOLEAN afNorth, BOOLEAN afEast, BOOLEAN afSouth, BOOLEAN afWest );
void AddFortificationPlanNode( INT32 sGridNo, INT8 sLevel, INT16 sFortificationStructure, UINT8 usFortificationTileLibraryIndex, BOOLEAN fAdd );

void LoadSectorFortificationPlan( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ );
void SaveSectorFortificationPlan( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ );

std::vector< std::pair<INT16, std::pair<UINT8, INT8> > > GetAllForticationGridNo( );

INT32 GetFirstObjectInSectorPosition( UINT16 ausItem );

extern ITEM_POOL *gpItemPool;//dnl ch26 210909

// Flugente: interactive actions
void DoInteractiveAction( INT32 sGridNo, SOLDIERTYPE *pSoldier );
void DoInteractiveActionDefaultResult( INT32 sGridNo, UINT8 ubID, BOOLEAN aSuccess );
BOOLEAN SpendMoney( SOLDIERTYPE *pSoldier, UINT32 aAmount );			// character spends money - either from inventory or the account

// Flugente: intel
void TakePhoto( SOLDIERTYPE* pSoldier, INT32 sGridNo, INT8 bLevel );

#endif