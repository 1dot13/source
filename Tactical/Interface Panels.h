#ifndef __INTERFACE_PANELS
#define __INTERFACE_PANELS


typedef enum
{
	STANCEUP_BUTTON = 0,
	UPDOWN_BUTTON,
	CLIMB_BUTTON,
	STANCEDOWN_BUTTON,
	HANDCURSOR_BUTTON,
	PREVMERC_BUTTON,
	NEXTMERC_BUTTON,
	OPTIONS_BUTTON,
	BURSTMODE_BUTTON,
	LOOK_BUTTON,
	TALK_BUTTON,
	MUTE_BUTTON,
	SM_DONE_BUTTON,
	SM_MAP_SCREEN_BUTTON,
	NUM_SM_BUTTONS
};


typedef enum
{
	TEAM_DONE_BUTTON = 0,
	TEAM_MAP_SCREEN_BUTTON,
	CHANGE_SQUAD_BUTTON,
	NUM_TEAM_BUTTONS
};


#define NEW_ITEM_CYCLE_COUNT				19
#define NEW_ITEM_CYCLES						4
#define	NUM_TEAM_SLOTS						6


#define	PASSING_ITEM_DISTANCE_OKLIFE		3
#define	PASSING_ITEM_DISTANCE_NOTOKLIFE		2

#define	SHOW_LOCATOR_NORMAL					1
#define SHOW_LOCATOR_FAST					2

BOOLEAN CreateSMPanelButtons( );
void	RemoveSMPanelButtons( );
BOOLEAN InitializeSMPanel(	);
BOOLEAN ShutdownSMPanel( );
void RenderSMPanel( BOOLEAN *pfDirty );
void EnableSMPanelButtons( BOOLEAN fEnable, BOOLEAN fFromItemPickup );


/* In this function i will initialize all coords for SingleMercPanel
 * If i will success i will return true, i think that could be usefull when
 * we will have our panes more customizable for ex. we can read coords from file
 * and when we will read all values we return TRUE if not we will retunf FALSE
 * any questions? joker
 */
//BOOLEAN InitializeSMPanelCoords( ); 
BOOLEAN InitializeSMPanelCoordsNew();
BOOLEAN InitializeSMPanelCoordsOld(); 


BOOLEAN CreateTEAMPanelButtons( );
void	RemoveTEAMPanelButtons( );
BOOLEAN InitializeTEAMPanel(	);
BOOLEAN ShutdownTEAMPanel( );
void RenderTEAMPanel( BOOLEAN fDirty );
void UpdateTEAMPanel( );

/*
 * Just like b4 we will initialize all coords but now for TEAMPanel
 */
BOOLEAN InitializeTEAMPanelCoords( ); 

void SetSMPanelCurrentMerc( UINT8 ubNewID );
void SetTEAMPanelCurrentMerc( UINT8 ubNewID );
UINT16 GetSMPanelCurrentMerc(	);
void UpdateSMPanel( );

BOOLEAN InitTEAMSlots( );
void AddPlayerToInterfaceTeamSlot( UINT8 ubID );
BOOLEAN RemovePlayerFromInterfaceTeamSlot( UINT8 ubID );
BOOLEAN GetPlayerIDFromInterfaceTeamSlot( UINT8 ubPanelSlot, UINT8 *pubID );
void RemoveAllPlayersFromSlot( );
BOOLEAN PlayerExistsInSlot( UINT8 ubID );
BOOLEAN RemovePlayerFromTeamSlotGivenMercID( UINT8 ubMercID );
void CheckForAndAddMercToTeamPanel( SOLDIERTYPE *pSoldier );
void CleanUpStack( OBJECTTYPE * pObj, OBJECTTYPE * pCursorObj );

void DisableTacticalTeamPanelButtons( BOOLEAN fDisable );
// CHRISL: Add X,Y coords as parameters so we can control the placement of TownID string on the team and inventory panels seperately
void RenderTownIDString( INT16 sX, INT16 sY );
void KeyRingSlotInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
// CHRISL: New functions to handle initialization of inventory systems
void InitNewInventorySystem( );
void InitOldInventorySystem( );



// ATE TO BE MOVED TO INTERFACE_ITEMS.C
extern INT8		gbNewItem[ NUM_INV_SLOTS ];
extern INT8		gbNewItemCycle[ NUM_INV_SLOTS ];
extern UINT8	gubNewItemMerc;

void ShowRadioLocator( UINT8 ubID, UINT8 ubLocatorSpeed );
void EndRadioLocator( UINT8 ubID );


extern MOUSE_REGION	gSMPanelRegion;
extern UINT32		guiSecItemHiddenVO;
extern BOOLEAN		gfDisableTacticalPanelButtons;


typedef struct
{
	UINT8	ubID;
	BOOLEAN	fOccupied;

} TEAM_PANEL_SLOTS_TYPE;

extern TEAM_PANEL_SLOTS_TYPE	gTeamPanel[ NUM_TEAM_SLOTS ];



//Used when the shop keeper interface is active
void DisableSMPpanelButtonsWhenInShopKeeperInterface( BOOLEAN fDontDrawButtons );
//void DisableSMPpanelButtonsWhenInShopKeeperInterface( );

//
void ReEvaluateDisabledINVPanelButtons( );
void CheckForReEvaluateDisabledINVPanelButtons( );

void CheckForDisabledForGiveItem( );
void ReevaluateItemHatches( SOLDIERTYPE *pSoldier, BOOLEAN fEnable );
//CHRISL: New function to deal with rendering backpack buttons
void RenderBackpackButtons(int bpAction);

void HandlePanelFaceAnimations( SOLDIERTYPE *pSoldier );

void GoToMapScreenFromTactical( void );

// Moved to weapons.h by ADB, rev 1513
//void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem );

void FinishAnySkullPanelAnimations( );

UINT8 FindNextMercInTeamPanel( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs );

// CHRISL: New callback functions for backpack buttons
BOOLEAN ChangeDropPackStatus(SOLDIERTYPE *pSoldier, BOOLEAN newStatus);

#endif