#ifndef _ENEMY_ITEM_DROPS__H_
#define	_ENEMY_ITEM_DROPS__H_

#define MAX_DROP_ITEMS		100

typedef struct
{
	UINT8		ubWeaponType;
	UINT8		ubEnemyDropRate;
	UINT8		ubMilitiaDropRate;
	UINT32		uiIndex;
} WEAPON_DROPS;

typedef struct
{
	UINT32		uiType;
	UINT8		ubEnemyDropRate;
	UINT8		ubMilitiaDropRate;
	UINT32		uiIndex;
} AMMO_DROPS;

typedef struct
{
	UINT8		ubType;
	UINT8		ubEnemyDropRate;
	UINT8		ubMilitiaDropRate;
	UINT32		uiIndex;
} EXPLOSIVE_DROPS;

typedef struct
{
	UINT8		ubArmourClass;
	UINT8		ubEnemyDropRate;
	UINT8		ubMilitiaDropRate;
	UINT32		uiIndex;
} ARMOUR_DROPS;

typedef struct
{
	UINT32		usItemClass;
	UINT8		ubEnemyDropRate;
	UINT8		ubMilitiaDropRate;
	UINT32		uiIndex;
} MISC_DROPS;

extern WEAPON_DROPS gEnemyWeaponDrops[MAX_DROP_ITEMS]; 
extern AMMO_DROPS gEnemyAmmoDrops[MAX_DROP_ITEMS]; 
extern EXPLOSIVE_DROPS gEnemyExplosiveDrops[MAX_DROP_ITEMS];
extern ARMOUR_DROPS gEnemyArmourDrops[MAX_DROP_ITEMS];
extern MISC_DROPS gEnemyMiscDrops[MAX_DROP_ITEMS];

#endif