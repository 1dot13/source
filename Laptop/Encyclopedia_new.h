#ifndef __Encyclopedia_new_H
#define __Encyclopedia_new_H

/// For switching between data sub pages.
enum ENC_SUBPAGE_T {
	ENC_MAINPAGE = 0,
	ENC_LOCATIONS,
	ENC_CHARACTERS,
	ENC_ITEMS,
	ENC_QUESTS//4
};
#define ENC_NUM_SUBPAGES ENC_QUESTS
extern ENC_SUBPAGE_T geENC_SubPage;//current subpage

/// values for \ref gbEncyclopediaData_ItemVisible, can be set by using \ref EncyclopediaSetItemAsVisible(). During campain those values can only be increased, never decreased. If you want to reset to 0 use \ref EncyclopediaInitItemsVisibility instead.
typedef enum ENC_ITEM_VISIBILITY_T {
ENC_ITEM_NOT_DISCOVERED = 0,			///< item is not visible in encyclopedia
ENC_ITEM_DISCOVERED_NOT_REACHABLE,		///< item is visible in encyclopedia if external option allows not reachable items to be added as discovered. example: item in sector stash but is not reachable, item has been seen on enemy soldier.
ENC_ITEM_DISCOVERED_NOT_INSPECTABLE,	///< item is visible in encyclopedia if external option allows not inspectable items to be added as discovered. Item is potentially reachable, but can not be inspected. example: item got displayed in bobby rays screen, item got displayed in merc/aim laptop screen.
ENC_ITEM_DISCOVERED_INSPECTABLE,		///< item is visible in encyclopedia if external option allows inspectable items to be added as discovered. Item is potentially reachable and can be inspected. example: discovering from a dealers inventory, discovering from sector stash.
ENC_ITEM_DISCOVERED						///< item is visible in encyclopedia. Item can be fully inspected. example: item is in merc inventory.
};
extern UINT8 gbEncyclopediaData_ItemVisible[];
extern void EncyclopediaInitItemsVisibility();
extern void EncyclopediaSetItemAsVisible( UINT16 itemIndex, ENC_ITEM_VISIBILITY_T visibility );

extern BOOLEAN LoadEncyclopediaItemVisibility( HWFILE hFile );
extern BOOLEAN SaveEncyclopediaItemVisibility( HWFILE hFile );

extern void GameInitEncyclopedia_NEW(  );
extern void HandleEncyclopedia_NEW(  );
extern void RenderEncyclopedia_NEW(  );
extern BOOLEAN EnterEncyclopedia_NEW(  );
extern BOOLEAN ExitEncyclopedia_NEW(  );


#endif
