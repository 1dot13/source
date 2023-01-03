#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __EDITORITEMS_H
#define __EDITORITEMS_H

typedef struct{
	BOOLEAN fGameInit;	//Used for initializing save variables the first time.
	BOOLEAN fActive;		//currently active
	UINT16 *pusItemIndex;		//a dynamic array of Item indices
	UINT32	uiItemType; //Weapons, ammo, armour, explosives, equipment
	INT16	sNumItems;	//total number of items in the current class of item.
	INT16		sSelItemIndex;	//currently selected item index.
	INT16	sHilitedItemIndex;
	INT16	sScrollIndex;	//current scroll index (0 is far left, 1 is next tile to the right, ...)
	INT16	sSaveSelWeaponsIndex, sSaveSelAmmoIndex, sSaveSelArmourIndex, sSaveSelLBEIndex, sSaveSelExplosivesIndex,
					sSaveSelEquipment1Index, sSaveSelEquipment2Index, sSaveSelEquipment3Index,
					sSaveSelTriggersIndex, sSaveSelKeysIndex, sSaveSelRandomItemIndex;
	INT16	sSaveWeaponsScrollIndex, sSaveAmmoScrollIndex, sSaveArmourScrollIndex, sSaveLBEScrollIndex,
					sSaveExplosivesScrollIndex, sSaveEquipment1ScrollIndex, sSaveEquipment2ScrollIndex,
					sSaveEquipment3ScrollIndex, sSaveTriggersScrollIndex, sSaveKeysScrollIndex, sSaveRandomItemScrollIndex;
	INT16	sNumWeapons, sNumAmmo, sNumArmour, sLBEGear, sNumExplosives, sNumEquipment1, sNumEquipment2, sNumEquipment3, 
					sNumTriggers, sNumKeys, sNumRandomItems;
}EditorItemsInfo;

extern EditorItemsInfo eInfo;

void InitEditorItemsToolbar();
void EntryInitEditorItemsInfo();
void InitEditorItemsInfo(UINT32 uiItemType);
void RenderEditorItemsInfo();
void ClearEditorItemsInfo();
void DisplayItemStatistics();
void DetermineItemsScrolling();
void ScrollEditorItemsInfo(BOOLEAN fForward);//dnl ch80 011213

//User actions
void AddSelectedItemToWorld( INT32 sGridNo );
void HandleRightClickOnItem( INT32 sGridNo );
void DeleteSelectedItem();
void ShowSelectedItem();
void HideSelectedItem();
void SelectNextItemPool();
void SelectNextItemInPool();
void SelectPrevItemInPool();

void KillItemPoolList();
void BuildItemPoolList();

void HideItemCursor( INT32 iMapIndex );
void ShowItemCursor( INT32 iMapIndex );

void SetEditorItemsTaskbarMode( UINT16 usNewMode );

void HandleItemsPanel( UINT16 usScreenX, UINT16 usScreenY, INT8 bEvent );

extern INT32 giDefaultExistChance;

//dnl ch86 120214
typedef struct TAG_ITEM_POOL ITEM_POOL;
BOOLEAN ItemPoolListMove(INT32 iOldGridNo, INT32 iNewGridNo, ITEM_POOL *pItemPool);
BOOLEAN DeleteItemNode(INT32 iMapIndex);

#endif
#endif
