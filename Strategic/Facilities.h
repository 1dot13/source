#ifndef _FACILITIES_H
#define _FACILITIES_H

#include "campaign types.h"
#include "Soldier Control.h"

// Get a specific modifier for a specific assignment at a specific facility. Includes Ambient (non-assignment based) 
// effects, if any.
INT16 GetFacilityModifier( UINT8 ubModifierType, UINT8 ubFacilityType, UINT8 ubAssignmentType );
// Get a specific modifier from all facilities in the soldier's sector. This utilizes the above function.
INT16 GetSectorModifier( SOLDIERTYPE *pSoldier, UINT8 ubModifierType );

// Level of awareness to enemy movements, bestowed by facilities
extern void UpdateStrategicDetectionLevel( ); 

// Adjustment to Skyrider's cost-per-tile, bestowed by facilities
extern void UpdateSkyriderCostModifier( );
extern INT16 gsSkyriderCostModifier; // This variable is SAVED/LOADED

// HEADROCK HAM 3.6: Strategic info variable, total of costs accumulated for the use of facilities today. Deducted
// from account and reset at midnight, unless player can't pay (in which case, carries over to the next day).
extern INT32 giTotalOwedForFacilityOperationsToday; // This variable is SAVED and LOADED.
// Do we have outstanding debts for facility operation?
extern BOOLEAN gfOutstandingFacilityDebt; // This variable is SAVED and LOADED.

// Place this here for facility staffing checks.
extern UINT32 GetWorldTotalMin( );

// These are all the different modifiers we could be checking for.
enum
{
	FACILITY_PERFORMANCE_MOD = 0,
	FACILITY_SLEEP_MOD,
	FACILITY_FATIGUE_MOD,
	FACILITY_KIT_DEGRADE_MOD,
	
	FACILITY_MAX_MORALE,
	FACILITY_MAX_BREATH,

	FACILITY_DETECT_IMMEDIATE,
	FACILITY_DETECT_DYNAMIC,
	FACILITY_DETECT_LONGRANGE,
	FACILITY_DETECT_ANYWHERE,
	FACILITY_COUNT_INWILD,
	FACILITY_COUNT_INCITIES,
	
	FACILITY_MINE_INCOME_MOD,
	FACILITY_SKYRIDER_COST_MOD,

	FACILITY_CANTINA_MOD
};

// HEADROCK HAM 3.6: Different enemy detection and counting levels
enum
{
	DETECT_ENEMIES_DYNAMIC = 0,
	DETECT_ENEMIES_LONGRANGE,
	DETECT_ENEMIES_ANYWHERE,

	COUNT_ENEMIES_IN_WILD,
	COUNT_ENEMIES_IN_CITIES,
};

void UpdateGlobalVariablesFromFacilities( void );
void UpdateFacilityUsageCosts( void );

void HandleDailyPaymentFacilityDebt( void );
void HandleManualPaymentFacilityDebt( void );
void HandleHourlyRisks( void );

INT32 MineIncomeModifierFromFacility( UINT8 ubMine );
INT8 GetSoldierFacilityAssignmentIndex( SOLDIERTYPE *pSoldier );
void HandleRisksForSoldier( SOLDIERTYPE *pSoldier );
void HandleRisksForSoldierFacilityAssignment( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType );
INT16 FacilityRiskResult( SOLDIERTYPE *pSoldier, UINT8 ubRiskType, UINT8 ubFacilityType, UINT8 ubAssignmentType );
INT32 GetTotalFacilityHourlyCosts( BOOLEAN fPositive );

void InitFacilities();

#endif