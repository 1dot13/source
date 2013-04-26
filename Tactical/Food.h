#ifndef __FOOD_H
#define __FOOD_H

#include "soldier control.h"

#define FOOD_MIN									- 20000
#define FOOD_MAX									20000

#define FOOD_RANGE									(FOOD_MAX - FOOD_MIN)
#define FOOD_HALF_RANGE								(FOOD_RANGE / 2)

typedef enum
{
	FOOD_STUFFED,
	FOOD_EXTREMELY_FULL,
	FOOD_VERY_FULL,
	FOOD_FULL,
	FOOD_SLIGHTLY_FULL,
	FOOD_NORMAL,
	FOOD_LOW,
	FOOD_EVEN_LOWER,
	FOOD_VERY_LOW,
	FOOD_DANGER,
	FOOD_DESPERATE,
	FOOD_STARVING,
	NUM_FOOD_MORALE_TYPES
} FoodMoraleModType;

#define FOOD_MERC_START_CONSUME					FOOD_LOW
#define FOOD_MERC_START_SHOW_HUNGER_SYMBOL		FOOD_EVEN_LOWER
#define FOOD_MERC_STOP_FACILITY					FOOD_FULL
#define FOOD_MERC_REFUSAL						FOOD_VERY_FULL

typedef struct
{
	INT32	bThreshold;
	INT8	bMoraleModifier;				// absolute modifier to max morale
	INT8	bSleepModifier;					// absolute modifier
	INT8	bBreathRegenModifier;			// percentage modifier
	INT8	bAssignmentEfficiencyModifier;	// percentage modifier
	UINT8	ubStatDamageChance;				// percentual chance to receive damage to life and strength
} FoodMoraleMod;

extern FoodMoraleMod FoodMoraleMods[NUM_FOOD_MORALE_TYPES];

extern	UINT8	gbPlayerNum;

typedef struct
{
	UINT16		uiIndex;	
	CHAR16		szName[80];				// name of this food
	INT32		bFoodPoints;			// points that will be added to our drink level
	INT32		bDrinkPoints;			// points that will be added to our drink level
	UINT16		ubPortionSize;			// how much is 'eaten' as a portion (percentage)
	INT8		bMoraleMod;				// morale modificator for eating this
	FLOAT		usDecayRate;			// rate at which food decays
} FOODTYPE;

//GLOBALS
extern FOODTYPE Food[FOOD_TYPE_MAX];

// for determining the type of water source a sector has
typedef enum
{
	WATER_NONE,				// no water at all
	WATER_DRINKABLE,		// drinkable water (rivers, water supply from houses)
	WATER_SALTWATER,		// salt water, can't drink that (until something special for this gets implemented)
	WATER_POISONOUS			// there is water, but it is poisoned (swamps and polluted sectors)
} FoodSectorWaterSupply;

// eat a food object. if fForce = FALSE, the soldier can reject to eat this once
// if fForceFromDrugs = TRUE, we HAVE to eat this, because we already got the effects from the ApplyDrugs-function (item is both drug and food)
BOOLEAN ApplyFood( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject, BOOLEAN fForce, BOOLEAN fForceFromDrugs );

void GetFoodSituation( SOLDIERTYPE *pSoldier, UINT8* pFoodSituation, UINT8* pWaterSituation );
void FoodMaxMoraleModifiy( SOLDIERTYPE *pSoldier, UINT8* pubMaxMorale );
void FoodNeedForSleepModifiy( SOLDIERTYPE *pSoldier, UINT8* pubNeedForSleep );
void ReducePointsForHunger( SOLDIERTYPE *pSoldier, UINT32 *pusPoints );
void ReduceBPRegenForHunger( SOLDIERTYPE *pSoldier, INT32 *psPoints );

void HourlyFoodSituationUpdate( SOLDIERTYPE *pSoldier );
void HourlyFoodAutoDigestion( SOLDIERTYPE *pSoldier );

// eat stuff from the inventory. if fcanteensonly = TRUE, only drink from canteen items
void EatFromInventory( SOLDIERTYPE *pSoldier, BOOLEAN fcanteensonly = FALSE );

void HourlyFoodUpdate( void );

UINT8 GetWaterQuality(INT16 asMapX, INT16 asMapY, INT8 asMapZ);

// a function that tries to fill up all canteens in this sector
void SectorFillCanteens( void );

OBJECTTYPE* GetUsableWaterDrumInSector( void );

#endif
