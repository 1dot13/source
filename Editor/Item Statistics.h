#include "BuildDefines.h"

#ifndef __ITEM_STATISTICS_H
#define __ITEM_STATISTICS_H

//enumerations for all of the different action items.	Used by the popup menu for
//changing the type of action item.	When modified, an equivalent text array must be 
//changed as well. 
enum
{
	ACTIONITEM_TRIP_KLAXON,
	ACTIONITEM_FLARE,
	ACTIONITEM_TEARGAS,
	ACTIONITEM_STUN,
	ACTIONITEM_SMOKE,		
	ACTIONITEM_MUSTARD,
	ACTIONITEM_MINE,
	ACTIONITEM_OPEN,
	ACTIONITEM_CLOSE,
	ACTIONITEM_SMPIT,
	ACTIONITEM_LGPIT,
	ACTIONITEM_SMALL,		//grenade	
	ACTIONITEM_MEDIUM,	//TNT
	ACTIONITEM_LARGE,		//C4
	ACTIONITEM_TOGGLE_DOOR,
	ACTIONITEM_TOGGLE_ACTION1,
	ACTIONITEM_TOGGLE_ACTION2,
	ACTIONITEM_TOGGLE_ACTION3,
	ACTIONITEM_TOGGLE_ACTION4,
	ACTIONITEM_ENTER_BROTHEL,
	ACTIONITEM_EXIT_BROTHEL,
	ACTIONITEM_KINGPIN_ALARM,
	ACTIONITEM_SEX,
	ACTIONITEM_REVEAL_ROOM,
	ACTIONITEM_LOCAL_ALARM,
	ACTIONITEM_GLOBAL_ALARM,
	ACTIONITEM_KLAXON,
	ACTIONITEM_UNLOCK_DOOR,
	ACTIONITEM_TOGGLE_LOCK,
	ACTIONITEM_UNTRAP_DOOR,
	ACTIONITEM_TOGGLE_PRESSURE_ITEMS,
	ACTIONITEM_MUSEUM_ALARM,
	ACTIONITEM_BLOODCAT_ALARM,
	ACTIONITEM_BIG_TEAR_GAS,

#ifdef JA2UB
	ACTIONITEM_BIGGENS_BOMBS,
	ACTIONITEM_BIGGENS_WARNING,
	ACTIONITEM_SEE_FORTIFIED_DOOR,
	ACTIONITEM_OPEN_FORTIFED_DOOR,
	ACTIONITEM_SEE_POWER_GEN_FAN,
	ACTIONITEM_NEW = ACTIONITEM_SEE_POWER_GEN_FAN + 216,//dnl ch86 170214
#else
	ACTIONITEM_NEW = ACTIONITEM_BIG_TEAR_GAS + 221,//dnl ch86 170214
#endif

	NUM_ACTIONITEMS
};

#ifdef JA2EDITOR

//#include "Item Types.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


//Handles the dynamic changing of text input fields and button modes depending on the currently edited
//item.	Both the merc's inventory panel, and the items tab use the same code to accomplish this.

//Set if we are editing items from the items tab.	Otherwise, it is assumed that we are 
//editing items from the merc's inventory panel.
extern BOOLEAN gfItemEditingMode;

//Set if we need to update the panel.
extern BOOLEAN gfRenderItemStatsPanel;

void SpecifyItemToEdit( OBJECTTYPE *pItem, INT32 iMapIndex );

void ShowItemStatsPanel();
void HideItemStatsPanel();
void EnableItemStatsPanel();
void DisableItemStatsPanel();

//called from the taskbar renderer.
void UpdateItemStatsPanel();

enum
{
	ITEMSTATS_APPLY,
	ITEMSTATS_CANCEL,
	ITEMSTATS_DEFAULT,
	ITEMSTATS_DELETE,
	ITEMSTATS_HIDE,
	ITEMSTATS_SHOW,
};
void ExecuteItemStatsCmd( /*UINT8*/ UINT16  ubAction );

extern OBJECTTYPE *gpItem;
extern INT32			gsItemGridNo;

extern CHAR16 gszActionItemDesc[ NUM_ACTIONITEMS ][ 30 ];
//Returns a pointer to one of the above string array.
extern const STR16 GetActionItemName( OBJECTTYPE *pItem );
//Called by the popup menu, when a selection is made.
extern void UpdateActionItem( /*INT8*/ INT16 bActionItemIndex );
//Changes an action item into the type specified by the ACTIONITEM enumeration.
extern void ChangeActionItem( OBJECTTYPE *pItem, /*INT8*/ INT16 bActionItemIndex );
extern /*INT8*/ INT16 gbActionItemIndex;
extern INT8 gbDefaultBombTrapLevel;

extern void SetOwnershipGroup( UINT8 ubNewGroup );

//extern CHAR16 gszActionItemDesc[ NUM_ACTIONITEMS ][ 30 ];

#endif
#endif
