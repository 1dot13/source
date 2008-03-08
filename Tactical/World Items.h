#ifndef __WORLD_ITEMS
#define __WORLD_ITEMS

#include "items.h"
#include "FileMan.h"


#define	WORLD_ITEM_DONTRENDER												0x0001
#define	WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO				0x0002
#define WORLD_ITEM_ARMED_BOMB												0x0040
#define WORLD_ITEM_SCIFI_ONLY												0x0080
#define WORLD_ITEM_REALISTIC_ONLY										0x0100
#define WORLD_ITEM_REACHABLE												0x0200
#define WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT		0x0400
//Kaiden: This constant is to flag items that an enemy drops when they die.
#define	WORLD_ITEM_DROPPED_FROM_ENEMY								0x0800

class OLD_WORLDITEM_101
{
public:
	BOOLEAN					fExists;
	INT16					sGridNo;
	UINT8					ubLevel;
	OLD_OBJECTTYPE_101		oldObject;
	UINT16					usFlags;
	INT8					bRenderZHeightAboveLevel;
	INT8					bVisible;
	UINT8					ubNonExistChance;	
};


class WORLDITEM
{
public:
	WORLDITEM() {initialize();};
	WORLDITEM&	operator=(OLD_WORLDITEM_101& src);
	WORLDITEM&	operator=(const WORLDITEM& src);
	bool		operator<(WORLDITEM& compare);
	BOOLEAN		Save( HWFILE hFile, bool fSavingMap );
	BOOLEAN		Load( HWFILE hFile );
	BOOLEAN		Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	void		initialize();
	BOOLEAN					fExists;
	INT16					sGridNo;
	UINT8					ubLevel;
	UINT16					usFlags;
	INT8					bRenderZHeightAboveLevel;
	INT8					bVisible;

	//This is the chance associated with an item or a trap not-existing in the world.	The reason why 
	//this is reversed (10 meaning item has 90% chance of appearing, is because the order that the map 
	//is saved, we don't know if the version is older or not until after the items are loaded and added.
	//Because this value is zero in the saved maps, we can't change it to 100, hence the reversal method.
	//This check is only performed the first time a map is loaded.	Later, it is entirely skipped.
	UINT8					ubNonExistChance;	
	INT8					soldierID;

	char					endOfPod;
	OBJECTTYPE				object;
};
#define SIZEOF_WORLDITEM_POD (offsetof(WORLDITEM, endOfPod))

extern WORLDITEM		*gWorldItems;
extern UINT32				guiNumWorldItems;

INT32 AddItemToWorld( INT16 sGridNo, OBJECTTYPE *pObject, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 bVisible, INT8 soldierID );
void RemoveItemFromWorld( INT32 iItemIndex );
INT32 FindWorldItem( UINT16 usItem );

void LoadWorldItemsFromMap( INT8 **hBuffer, float dMajorMapVersion, int ubMinorMapVersion );
void SaveWorldItemsToMap( HWFILE fp );

void TrashWorldItems();

typedef struct
{
	BOOLEAN				fExists;
	INT32					iItemIndex;
} WORLDBOMB;

extern WORLDBOMB * gWorldBombs;
extern UINT32 guiNumWorldBombs;

extern INT32 AddBombToWorld( INT32 iItemIndex );
extern void FindPanicBombsAndTriggers( void );
extern INT32 FindWorldItemForBombInGridNo( INT16 sGridNo, INT8 bLevel);

void RefreshWorldItemsIntoItemPools( WORLDITEM * pItemList, INT32 iNumberOfItems );

#endif