//////////////////////////////////////////////////////////////////
// HEADROCK HAM 3.5: Facilities
//
// This file contains functions that test facility data (read
// from XML) whenever necessary. See XML_Facilities.cpp and
// XML_FacilityTypes.cpp for the reading XML functions.
//////////////////////////////////////////////////////////////////

#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "Campaign Types.h"
	#include "Facilities.h"
	#include "Soldier Control.h"
	#include "Map Screen Interface.h"
	#include "Overhead.h"
	#include "Assignments.h"
	#include "GameSettings.h"
	#include "LaptopSave.h"
	#include "finances.h"
	#include "Text.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "SkillCheck.h"
	#include "Random.h"
	#include "strategicmap.h"
	#include "Game Clock.h"
	#include "Campaign.h"
	#include "Drugs And Alcohol.h"
	#include "Interface.h"
	#include "message.h"
	#include "Morale.h"
	#include "Points.h"
	#include "Soldier Control.h"
	#include "Isometric Utils.h"
	#include "MilitiaSquads.h"
	#include "Tactical Save.h"
#endif

INT16 gsSkyriderCostModifier;
// HEADROCK HAM 3.6: Strategic info variable, total of income/costs accumulated for the use of facilities today.
// Account settle and reset at midnight, unless player can't pay (in which case, carries over to the next day).
// This variable is SAVED and LOADED.
INT32 giTotalEarnedForFacilityOperationsToday = 0;
INT32 giTotalOwedForFacilityOperationsToday = 0;
BOOLEAN gfOutstandingFacilityDebt = TRUE;

// This function returns a specific modifier out of the FacilityTypes array. Modifiers exist inside an assignment
// sub-array, so the final result depends on the assignment the soldier is performing. The difference between
// this and simply reading data from the facility array is that in this case, "AMBIENT" effects are also included,
// which take effect regardless of the assignment.
INT16 GetFacilityModifier( UINT8 ubModifierType, UINT8 ubFacilityType, UINT8 ubAssignmentType )
{
	INT16 sAssignmentModifier;
	INT16 sAmbientModifier;

	// Are we checking an actual assignment at a specific facility?
	if (ubAssignmentType != FAC_AMBIENT &&
		ubFacilityType > 0 )
	{
		switch (ubModifierType)
		{
			case FACILITY_PERFORMANCE_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usPerformance;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usPerformance;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			case FACILITY_FATIGUE_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usFatigue;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usFatigue;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			case FACILITY_SLEEP_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usSleep;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usSleep;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			case FACILITY_KIT_DEGRADE_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usKitDegrade;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usKitDegrade;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			case FACILITY_MAX_MORALE:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMaximumMorale;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubMaximumMorale;
				sAssignmentModifier = __min(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_MAX_BREATH:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMaximumBreath;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubMaximumBreath;
				sAssignmentModifier = __min(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_DETECT_IMMEDIATE:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubDetectEnemiesImmediate;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubDetectEnemiesImmediate;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_DETECT_DYNAMIC:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fDetectEnemiesDynamic;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesDynamic;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_DETECT_LONGRANGE:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fDetectEnemiesLongrange;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesLongrange;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_DETECT_ANYWHERE:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fDetectEnemiesAnywhere;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesAnywhere;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_COUNT_INWILD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fCountEnemiesInWild;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fCountEnemiesInWild;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_COUNT_INCITIES:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fCountEnemiesInCities;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fCountEnemiesInCities;
				sAssignmentModifier = __max(sAssignmentModifier, sAmbientModifier);
				return (sAssignmentModifier);

			case FACILITY_SKYRIDER_COST_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sSkyriderCostModifier;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].sSkyriderCostModifier;
				sAssignmentModifier = sAssignmentModifier + sAmbientModifier;
				return (sAssignmentModifier);

			case FACILITY_MINE_INCOME_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usMineIncomeModifier;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usMineIncomeModifier;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			// Flugente: not needed anywhere atm
			case FACILITY_CANTINA_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCantinaFoodModifier;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].sCantinaFoodModifier;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			case FACILITY_PRISON_MOD:
				sAssignmentModifier = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usPrisonBaseLimit;
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usPrisonBaseLimit;
				sAssignmentModifier = 100 + ((sAssignmentModifier-100) + (sAmbientModifier-100));
				return (sAssignmentModifier);

			default:
				return (0);
		}
	}
	// Only ambient effect please
	else
	{
		switch (ubModifierType)
		{
			case FACILITY_PERFORMANCE_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usPerformance;
				return (sAmbientModifier);

			case FACILITY_FATIGUE_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usFatigue;
				return (sAmbientModifier);

			case FACILITY_SLEEP_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usSleep;
				return (sAmbientModifier);

			case FACILITY_KIT_DEGRADE_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usKitDegrade;
				return (sAmbientModifier);

			case FACILITY_MAX_MORALE:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubMaximumMorale;
				return (sAmbientModifier);

			case FACILITY_MAX_BREATH:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubMaximumBreath;
				return (sAmbientModifier);

			case FACILITY_DETECT_IMMEDIATE:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].ubDetectEnemiesImmediate;
				return (sAmbientModifier);

			case FACILITY_DETECT_DYNAMIC:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesDynamic;
				return (sAmbientModifier);

			case FACILITY_DETECT_LONGRANGE:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesLongrange;
				return (sAmbientModifier);

			case FACILITY_DETECT_ANYWHERE:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fDetectEnemiesAnywhere;
				return (sAmbientModifier);

			case FACILITY_COUNT_INWILD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fCountEnemiesInWild;
				return (sAmbientModifier);

			case FACILITY_COUNT_INCITIES:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].fCountEnemiesInCities;
				return (sAmbientModifier);

			case FACILITY_SKYRIDER_COST_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].sSkyriderCostModifier;
				return (sAmbientModifier);

			case FACILITY_MINE_INCOME_MOD:
				sAmbientModifier = gFacilityTypes[ubFacilityType].AssignmentData[0].usMineIncomeModifier;
				return (sAmbientModifier);

			default:
				return (0);
		}
	}
}

// This is a handler function that runs through every facility in the soldier's sector, retrieving one type of
// modifier from each facility.
INT16 GetSectorModifier( SOLDIERTYPE *pSoldier, UINT8 ubModifierType )
{
	INT16 sCurrent;
	INT16 Result;
	UINT8 ubSectorID = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
	INT8 bAssignmentType = GetSoldierFacilityAssignmentIndex(pSoldier);

	// Set "default" value.
	switch (ubModifierType)
	{
		case FACILITY_PERFORMANCE_MOD:
		case FACILITY_FATIGUE_MOD:
		case FACILITY_SLEEP_MOD:
		case FACILITY_KIT_DEGRADE_MOD:
		case FACILITY_MINE_INCOME_MOD:
			Result = 100;
			break;

		case FACILITY_MAX_MORALE:
		case FACILITY_MAX_BREATH:
			Result = 100;
			break;

		case FACILITY_DETECT_IMMEDIATE:
		case FACILITY_DETECT_DYNAMIC:
		case FACILITY_DETECT_LONGRANGE:
		case FACILITY_DETECT_ANYWHERE:
		case FACILITY_COUNT_INWILD:
		case FACILITY_COUNT_INCITIES:
			Result = 0;
			break;

		case FACILITY_SKYRIDER_COST_MOD:
			Result = 0;
			break;
	}

	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		if (gFacilityLocations[ubSectorID][cnt].fFacilityHere)
		{
			if ( bAssignmentType != -1)
			{
				// Soldier has a facility-based assignment. Is he working this type of facility?
				if ((UINT8)pSoldier->sFacilityTypeOperated == (UINT8)cnt)
				{
					sCurrent = GetFacilityModifier( ubModifierType, (UINT8)cnt, (UINT8)bAssignmentType );
				}
				else // He's operating another type of facility. Get only the ambient effect.
				{
					sCurrent = GetFacilityModifier( ubModifierType, (UINT8)cnt, FAC_AMBIENT );
				}
			}
			else
			{
				// Soldier is not doing facility-specific work. Get only the ambient effect.
				sCurrent = GetFacilityModifier( ubModifierType, (UINT8)cnt, FAC_AMBIENT );
			}

			// Calculate overall result so far. Calculation method is based on the modifier we're looking for.
			switch (ubModifierType)
			{
				case FACILITY_PERFORMANCE_MOD:
				case FACILITY_FATIGUE_MOD:
				case FACILITY_SLEEP_MOD:
				case FACILITY_KIT_DEGRADE_MOD:
				case FACILITY_MINE_INCOME_MOD:
					Result = 100 + ((Result-100) + (sCurrent-100));
					break;

				case FACILITY_MAX_MORALE:
				case FACILITY_MAX_BREATH:
					Result = __min(Result, sCurrent);
					break;

				case FACILITY_DETECT_IMMEDIATE:
				case FACILITY_DETECT_DYNAMIC:
				case FACILITY_DETECT_LONGRANGE:
				case FACILITY_DETECT_ANYWHERE:
				case FACILITY_COUNT_INWILD:
				case FACILITY_COUNT_INCITIES:
					Result = __max(Result, sCurrent);
					break;

				case FACILITY_SKYRIDER_COST_MOD:
					Result += sCurrent;
					break;
			}
		}
	}

	return (Result);
}

//////////////////////////////////////////////////
// This function reads the status of intelligence gathering currently being done by your mercs. They work at special
// facilities that can enable enemy detection in certain sectors.
//
// There are several detection methods, and they allow/disallow detection in different groups of sectors.
// DETECT_ENEMIES_IMMEDIATE: Detection in sectors near to where the merc is working.
// DETECT_ENEMIES_DYNAMIC: Detection in sectors where Mobile Militia are allowed to roam.
// DETECT_ENEMIES_LONGRANGE: Detection in all explored sectors.
// DETECT_ENEMIES_ANYWHERE: Detect enemy units all over the map.
// Also there are two Counting methods, which allow reading the exact number of troops in specific sectors.
// COUNT_ENEMIES_IN_WILD: Can count enemy numbers in non-city sectors only.
// COUNT_ENEMIES_IN_CITIES: Can counter enemy numbers in city sectors only.
//
// This function runs through each working merc, and flags all the appropriate sectors. Later on, the program reads
// these flags when it wants to find out whether an enemy unit is visible on the map, and whether the exact number of
// enemies in that unit is also shown.
void UpdateStrategicDetectionLevel( )
{
	SOLDIERTYPE * pSoldier;
	UINT8 ubCounter = 0;

	// Reset the detection flag in SectorInfo for all sectors.
	for (UINT16 X = 0; X < 256; X++)
	{
		// Reset sector detection flags.
		SectorInfo[X].ubDetectionLevel = 0;
	}

	// Flag to hold the current detection level before calculating its effects on the detection mask.
	UINT8 ubStrategicDetectionLevel = 0;

	// Run through list of player-characters
	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
			UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;

			if (GetSoldierFacilityAssignmentIndex( pSoldier ) == -1)
			{
				// Skip this soldier, he is not performing a facility assignment.
				ubCounter++;
				continue;
			}

			UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );

			// Test to see whether the character is properly set up at this facility.
			if ( CanCharacterFacility( pSoldier, ubFacilityType, ubAssignmentType ) &&
				GetWorldTotalMin() - pSoldier->uiLastAssignmentChangeMin >= (UINT32)gGameExternalOptions.ubMinutesForAssignmentToCount )
			{
				// Make sure facility type is valid.
				Assert(ubFacilityType < MAX_NUM_FACILITY_TYPES);

				if (!(gFacilityLocations[ubSector][ubFacilityType].fFacilityHere))
				{
					// Somehow, the character is working in a nonexistant facility. Odd. Skip him.
					ubCounter++;
					continue;
				}

				/////////////////////////////////////////////////////////
				// Begin testing for facility detection bonuses.

				// Does game allow Dynamic Roaming Militia?
				if (gGameExternalOptions.fDynamicRestrictRoaming)
				{
					// Does facility allow Dynamic Detection?
					if (GetFacilityModifier(FACILITY_DETECT_DYNAMIC, ubFacilityType, ubAssignmentType))
					{
						ubStrategicDetectionLevel |= (1 << DETECT_ENEMIES_DYNAMIC);
					}
				}

				// Does facility allow Long-Range Detection?
				if (GetFacilityModifier(FACILITY_DETECT_LONGRANGE, ubFacilityType, ubAssignmentType))
				{
					ubStrategicDetectionLevel |= (1 << DETECT_ENEMIES_LONGRANGE);
				}

				// Does facility allow Omniscient Detection?
				if (GetFacilityModifier(FACILITY_DETECT_ANYWHERE, ubFacilityType, ubAssignmentType))
				{
					ubStrategicDetectionLevel |= (1 << DETECT_ENEMIES_ANYWHERE);
				}

				// Does facility allow Counting Enemies in the Wilderness?
				if (GetFacilityModifier(FACILITY_COUNT_INWILD, ubFacilityType, ubAssignmentType))
				{
					ubStrategicDetectionLevel |= (1 << COUNT_ENEMIES_IN_WILD);
				}

				// Does facility allow Counting Enemies in the Wilderness?
				if (GetFacilityModifier(FACILITY_COUNT_INCITIES, ubFacilityType, ubAssignmentType))
				{
					ubStrategicDetectionLevel |= (1 << COUNT_ENEMIES_IN_CITIES);
				}

				// "Immediate" detection allows spotting enemies within a certain radius of the facility.
				UINT16 usImmediateDetectionRange = (UINT16)GetFacilityModifier(FACILITY_DETECT_IMMEDIATE, ubFacilityType, ubAssignmentType);
				if (usImmediateDetectionRange)
				{
					// Loop through all nearby sectors.
					UINT8 sMapX;
					UINT8 sMapY;
					for (sMapX = pSoldier->sSectorX - usImmediateDetectionRange; sMapX <= pSoldier->sSectorX + usImmediateDetectionRange; sMapX++)
					{
						for (sMapY = pSoldier->sSectorY - usImmediateDetectionRange; sMapY <= pSoldier->sSectorY + usImmediateDetectionRange; sMapY++)
						{
							if (sMapX <= 0 || sMapX >= MAP_WORLD_X - 1 ||
								sMapY <= 0 || sMapY >= MAP_WORLD_Y - 1 )
							{
								// Sector out of bounds.
								continue;
							}

							if (abs(pSoldier->sSectorX - sMapX) <= usImmediateDetectionRange &&
								abs(pSoldier->sSectorY - sMapY) <= usImmediateDetectionRange)
							{
								// Sector within range. Update its detection flag.
								SectorInfo[SECTOR(sMapX, sMapY)].ubDetectionLevel |= 1;
							}
						}
					}
				}
			}
		}
		ubCounter++;
	}
	// Some flags have been set. Run through all sectors and apply this.
	for (UINT16 X = 0; X < 256; X++)
	{
		// Reset detection level first
		SectorInfo[X].ubDetectionLevel = 0;

		if (ubStrategicDetectionLevel & (1<<DETECT_ENEMIES_DYNAMIC))
		{
			// Is sector allowed for militia roaming?
			if (IsSectorRoamingAllowed( X ) )
			{
				// Enemy is in a militia-allowed area, and is always visible.
				SectorInfo[X].ubDetectionLevel |= 1;
			}
		}
		if (ubStrategicDetectionLevel & (1<<DETECT_ENEMIES_LONGRANGE))
		{
			if( GetSectorFlagStatus( SECTORX(X), SECTORY(X), 0, SF_ALREADY_VISITED ) == TRUE )
			{
				SectorInfo[X].ubDetectionLevel |= 1;
			}
		}
		if (ubStrategicDetectionLevel & (1<<DETECT_ENEMIES_ANYWHERE))
		{
			SectorInfo[X].ubDetectionLevel |= 1;
		}

		// ENEMY COUNTING:
		if (ubStrategicDetectionLevel & (1<<COUNT_ENEMIES_IN_WILD)||
			ubStrategicDetectionLevel & (1<<COUNT_ENEMIES_IN_CITIES))
		{
			if (GetTownIdForSector( SECTORX(X), SECTORY(X) ) == BLANK_SECTOR &&
				ubStrategicDetectionLevel & (1<<COUNT_ENEMIES_IN_WILD))
			{
				SectorInfo[X].ubDetectionLevel |= (1<<2);
			}
			else if (GetTownIdForSector( SECTORX(X), SECTORY(X) ) != BLANK_SECTOR &&
				ubStrategicDetectionLevel & (1<<COUNT_ENEMIES_IN_CITIES))
			{
				SectorInfo[X].ubDetectionLevel |= (1<<2);
			}
		}
	}
}

// This function runs hourly and on gameload. It recalculates a modifier to Skyrider's per-sector flight costs,
// for every awake+resting character in a sector with a facility that modifies the cost.
void UpdateSkyriderCostModifier()
{
	SOLDIERTYPE * pSoldier;
	UINT8 ubCounter = 0;

	// Start by resetting the Skyrider Cost variable.
	gsSkyriderCostModifier = 0;

	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
			UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
			if (GetSoldierFacilityAssignmentIndex( pSoldier ) == -1)
			{
				// Skip this soldier, he is not performing a facility assignment.
				ubCounter++;
				continue;
			}
			UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );

			// Is character using a facility?
			if ( CanCharacterFacility( pSoldier, ubFacilityType, ubAssignmentType ) &&
				GetWorldTotalMin() - pSoldier->uiLastAssignmentChangeMin >= (UINT32)gGameExternalOptions.ubMinutesForAssignmentToCount )
			{

				// Make sure facility type is valid.
				Assert(ubFacilityType < MAX_NUM_FACILITY_TYPES);

				if (!(gFacilityLocations[ubSector][ubFacilityType].fFacilityHere))
				{
					// Somehow, the character is working in a nonexistant facility. Odd. Skip him.
					ubCounter++;
					continue;
				}

				// Does facility change Skyrider Costs at all?
				gsSkyriderCostModifier += GetFacilityModifier( FACILITY_SKYRIDER_COST_MOD, ubFacilityType, ubAssignmentType );
			}
		}
		ubCounter++;
	}
}

// HEADROCK HAM 3.6: Goes through all characters "STAFFING" a facility, and checks to see whether they alter any
// strategic variables.
void UpdateGlobalVariablesFromFacilities( void )
{
	UpdateStrategicDetectionLevel( );

	UpdateSkyriderCostModifier( );

	UpdateFacilityUsageCosts( );
}

// This function runs hourly. It calculates the total cost of all facilities being operated currently,
// and adds them to a global "debt" variable that tracks expenditure. At the end of the day, the debt is deducted from
// the player's account.
void UpdateFacilityUsageCosts( )
{
	SOLDIERTYPE * pSoldier;
	UINT8 ubCounter = 0;
	INT16 sCost = 0;

	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
			UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
			if (GetSoldierFacilityAssignmentIndex( pSoldier ) == -1)
			{
				// Skip this soldier, he is not performing a facility assignment.
				ubCounter++;
				continue;
			}
			UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );

			// Is character using a facility?
			if ( CanCharacterFacility( pSoldier, ubFacilityType, ubAssignmentType ) &&
				GetWorldTotalMin() - pSoldier->uiLastAssignmentChangeMin >= (UINT32)gGameExternalOptions.ubMinutesForAssignmentToCount )
			{
				// Make sure facility type is valid.
				Assert(ubFacilityType < MAX_NUM_FACILITY_TYPES);

				if (!(gFacilityLocations[ubSector][ubFacilityType].fFacilityHere))
				{
					// Somehow, the character is working in a nonexistant facility. Odd. Skip him.
					ubCounter++;
					continue;
				}

				sCost = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour;

				if (sCost > 0)
				{
					/////////////////////////////////////////////////////////
					// Increase debt for operating this facility

					giTotalOwedForFacilityOperationsToday += gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour;
				}
				else if (sCost < 0)
				{
					/////////////////////////////////////////////////////////
					// Increase income for operating this facility

					giTotalEarnedForFacilityOperationsToday += (-1 * gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour);
				}

			}
		}
		ubCounter++;
	}
}

void HandleDailyPaymentFacilityIncome( void )
{
	// Pay total income.
	AddTransactionToPlayersBook( FACILITY_OPERATIONS, 0, GetWorldTotalMin(), giTotalEarnedForFacilityOperationsToday );
	giTotalEarnedForFacilityOperationsToday = 0;
}

// HEADROCK HAM 3.6: This function runs once at the end of each day.
// It handles debt accrued by operation facilities. If there is enough money to pay the debt off, this
// is done automatically. If only part of the money is available, or none of it, your account is emptied and all
// "expensive" facility work ends immediately. You also suffer a loyalty hit across Arulco based on how much money
// you owe. Facility work cannot continue before the debt is paid off!
void HandleDailyPaymentFacilityDebt( void )
{
	CHAR16 sString[256];
	INT32 iToPay = 0;

	if (LaptopSaveInfo.iCurrentBalance >= giTotalOwedForFacilityOperationsToday)
	{
		// Pay total debt.
		iToPay = giTotalOwedForFacilityOperationsToday;
		gfOutstandingFacilityDebt = FALSE;
	}
	else if (LaptopSaveInfo.iCurrentBalance < giTotalOwedForFacilityOperationsToday)
	{
		if (LaptopSaveInfo.iCurrentBalance > 0)
		{
			// Still have some money in the bank. Pay it.
			iToPay = LaptopSaveInfo.iCurrentBalance;
			swprintf( sString, New113HAMMessage[14], iToPay, giTotalOwedForFacilityOperationsToday - iToPay );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			HandleFacilityDebtLoyaltyHit();
			ResetAllExpensiveFacilityAssignments();
			gfOutstandingFacilityDebt = TRUE;
		}
		else if (LaptopSaveInfo.iCurrentBalance <= 0)
		{
			// No money to pay!
			iToPay = 0;
			swprintf( sString, New113HAMMessage[15], giTotalOwedForFacilityOperationsToday);
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			HandleFacilityDebtLoyaltyHit();
			ResetAllExpensiveFacilityAssignments();
			gfOutstandingFacilityDebt = TRUE;
		}
	}

	if (iToPay > 0)
	{
		AddTransactionToPlayersBook( FACILITY_OPERATIONS, 0, GetWorldTotalMin(), -( iToPay ) );
		giTotalOwedForFacilityOperationsToday -= iToPay;
	}
}

// HEADROCK HAM 3.6: This functionruns a player-requested payoff of facility debt. It is only possible if the player
// has enough money in his account to pay off the ENTIRE debt. The function is called when a player tries to assign
// a merc to facility duty while an outstanding debt exists. Debts are only outstanding if they failed to be paid off
// during the previous midnight.
void HandleManualPaymentFacilityDebt( void )
{
	INT32 iToPay = 0;

	// Is there any debt at all?
	if (giTotalOwedForFacilityOperationsToday > 0)
	{
		// This function should ONLY be called if there's enough money in the bank to pay off the entire debt.
		Assert( LaptopSaveInfo.iCurrentBalance >= giTotalOwedForFacilityOperationsToday );

		// Pay total debt.
		iToPay = giTotalOwedForFacilityOperationsToday;
		AddTransactionToPlayersBook( FACILITY_OPERATIONS, 0, GetWorldTotalMin(), -( iToPay ) );
		giTotalOwedForFacilityOperationsToday = 0;
		gfOutstandingFacilityDebt = FALSE;
	}
}

// HEADROCK HAM 3.6: Apply income bonuses from Facility Work to a specific mine.
INT32 MineIncomeModifierFromFacility( UINT8 ubMine )
{
	Assert (ubMine > 0 && ubMine < MAX_NUMBER_OF_MINES);

	SOLDIERTYPE *pSoldier;
	INT32 iModifier = 0;

	UINT8 ubCounter = 0;
	while (gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
			UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
			if (GetSoldierFacilityAssignmentIndex( pSoldier ) == -1)
			{
				// Skip this soldier, he is not performing a facility assignment.
				ubCounter++;
				continue;
			}
			UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );

			// Is character using a facility?
			if ( CanCharacterFacility( pSoldier, ubFacilityType, ubAssignmentType ) &&
				GetWorldTotalMin() - pSoldier->uiLastAssignmentChangeMin >= (UINT32)gGameExternalOptions.ubMinutesForAssignmentToCount )
			{
				if (GetFacilityModifier( FACILITY_MINE_INCOME_MOD, ubFacilityType, ubAssignmentType ) && // Facility adjusts mine income
					(gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].fOnlyLocalMineAffected == 0 || // All mines affected
					GetMineIndexForSector( pSoldier->sSectorX, pSoldier->sSectorY ) == ubMine ) ) // This mine is here
				{
					iModifier += (gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].usMineIncomeModifier - 100);
				}
			}
		}
		ubCounter++;
	}

	return iModifier;
}

// This function converts a soldier's current assignment into a single Facility Assignment Type Index.
// The Assignment Type Index is used for referencing various data and effects associated with performing a
// SPECIFIC assignment with the help of a SPECIFIC facility. It's basically a reference number for the gFacilityTypes
// array.
INT8 GetSoldierFacilityAssignmentIndex( SOLDIERTYPE *pSoldier )
{
	// Read soldier's assignment
	INT8 bAssignment = pSoldier->bAssignment;
	INT8 bAssignmentIndex = -1; // Return variable. -1 = Non-facility assignment.

	if (bAssignment <= ON_DUTY ||
		bAssignment == PATIENT ||
		bAssignment == VEHICLE ||
		bAssignment == IN_TRANSIT ||
		bAssignment == TRAIN_TOWN ||
		bAssignment == TRAIN_MOBILE ||
		( bAssignment > FACILITY_REST && bAssignment != FACILITY_STRATEGIC_MILITIA_MOVEMENT && ( bAssignment < FACILITY_PRISON_SNITCH || bAssignment > FACILITY_GATHER_RUMOURS ) ))
	{
		// Soldier is performing a distinctly NON-FACILITY assignment.
		return (-1);
	}

	if (pSoldier->sFacilityTypeOperated == -1)
	{
		// Soldier is not set to work at a facility...
		return (-1);
	}

	switch (bAssignment)
	{
		case DOCTOR:
			bAssignmentIndex = FAC_DOCTOR;
			break;
		case PATIENT:
			bAssignmentIndex = FAC_PATIENT;
			break;
		case REPAIR:
			// Determine which kind of repair is he performing
			if ( pSoldier->bVehicleUnderRepairID != -1 )
			{
				bAssignmentIndex = FAC_REPAIR_VEHICLE;
			}
			else if ( pSoldier->flags.fFixingRobot )
			{
				bAssignmentIndex = FAC_REPAIR_ROBOT;
			}
			else
			{
				bAssignmentIndex = FAC_REPAIR_ITEMS;
			}
			break;
		case TRAIN_SELF:
			switch (pSoldier->bTrainStat)
			{
				case STRENGTH:
					bAssignmentIndex = FAC_PRACTICE_STRENGTH;
					break;
				case DEXTERITY:
					bAssignmentIndex = FAC_PRACTICE_DEXTERITY;
					break;
				case AGILITY:
					bAssignmentIndex = FAC_PRACTICE_AGILITY;
					break;
				case HEALTH:
					bAssignmentIndex = FAC_PRACTICE_HEALTH;
					break;
				case MARKSMANSHIP:
					bAssignmentIndex = FAC_PRACTICE_MARKSMANSHIP;
					break;
				case MEDICAL:
					bAssignmentIndex = FAC_PRACTICE_MEDICAL;
					break;
				case MECHANICAL:
					bAssignmentIndex = FAC_PRACTICE_MECHANICAL;
					break;
				case LEADERSHIP:
					bAssignmentIndex = FAC_PRACTICE_LEADERSHIP;
					break;
				case EXPLOSIVE_ASSIGN:
					bAssignmentIndex = FAC_PRACTICE_EXPLOSIVES;
					break;
				default:
					bAssignmentIndex = -1;
					break;
			}
			break;
		case TRAIN_TEAMMATE:
			switch (pSoldier->bTrainStat)
			{
				case STRENGTH:
					bAssignmentIndex = FAC_TRAINER_STRENGTH;
					break;
				case DEXTERITY:
					bAssignmentIndex = FAC_TRAINER_DEXTERITY;
					break;
				case AGILITY:
					bAssignmentIndex = FAC_TRAINER_AGILITY;
					break;
				case HEALTH:
					bAssignmentIndex = FAC_TRAINER_HEALTH;
					break;
				case MARKSMANSHIP:
					bAssignmentIndex = FAC_TRAINER_MARKSMANSHIP;
					break;
				case MEDICAL:
					bAssignmentIndex = FAC_TRAINER_MEDICAL;
					break;
				case MECHANICAL:
					bAssignmentIndex = FAC_TRAINER_MECHANICAL;
					break;
				case LEADERSHIP:
					bAssignmentIndex = FAC_TRAINER_LEADERSHIP;
					break;
				case EXPLOSIVE_ASSIGN:
					bAssignmentIndex = FAC_TRAINER_EXPLOSIVES;
					break;
				default:
					bAssignmentIndex = -1;
					break;
			}
			break;
		case TRAIN_BY_OTHER:
			switch (pSoldier->bTrainStat)
			{
				case STRENGTH:
					bAssignmentIndex = FAC_STUDENT_STRENGTH;
					break;
				case DEXTERITY:
					bAssignmentIndex = FAC_STUDENT_DEXTERITY;
					break;
				case AGILITY:
					bAssignmentIndex = FAC_STUDENT_AGILITY;
					break;
				case HEALTH:
					bAssignmentIndex = FAC_STUDENT_HEALTH;
					break;
				case MARKSMANSHIP:
					bAssignmentIndex = FAC_STUDENT_MARKSMANSHIP;
					break;
				case MEDICAL:
					bAssignmentIndex = FAC_STUDENT_MEDICAL;
					break;
				case MECHANICAL:
					bAssignmentIndex = FAC_STUDENT_MECHANICAL;
					break;
				case LEADERSHIP:
					bAssignmentIndex = FAC_STUDENT_LEADERSHIP;
					break;
				case EXPLOSIVE_ASSIGN:
					bAssignmentIndex = FAC_STUDENT_EXPLOSIVES;
					break;
				default:
					bAssignmentIndex = -1;
					break;
			}
			break;
		case FACILITY_STAFF:
			bAssignmentIndex = FAC_STAFF;
			break;
		case FACILITY_EAT:
			bAssignmentIndex = FAC_FOOD;
			break;
		case FACILITY_REST:
			bAssignmentIndex = FAC_REST;
			break;
		case FACILITY_INTERROGATE_PRISONERS:
			bAssignmentIndex = FAC_INTERROGATE_PRISONERS;
			break;
		case FACILITY_PRISON_SNITCH:
			bAssignmentIndex = FAC_PRISON_SNITCH;
			break;
		case FACILITY_SPREAD_PROPAGANDA:
			bAssignmentIndex = FAC_SPREAD_PROPAGANDA;
			break;
		case FACILITY_SPREAD_PROPAGANDA_GLOBAL:
			bAssignmentIndex = FAC_SPREAD_PROPAGANDA;
			break;
		case FACILITY_GATHER_RUMOURS:
			bAssignmentIndex = FAC_GATHER_RUMOURS;
			break;
		case FACILITY_STRATEGIC_MILITIA_MOVEMENT:
			bAssignmentIndex = FAC_STRATEGIC_MILITIA_MOVEMENT;
			break;
		default:
			bAssignmentIndex = -1;
			break;
	}

	return (bAssignmentIndex);

}

// This function runs a specific kind of RISK associated with a facility. The character's stats may affect the result.
INT16 FacilityRiskResult( SOLDIERTYPE *pSoldier, UINT8 ubRiskType, UINT8 ubFacilityType, UINT8 ubAssignmentType )
{
	INT16 Result = 0;

	INT32 iChance;
	INT16 bBaseEffect;
	UINT16 ubRange;

	// Read risk data directly from facility array.
	iChance = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].Risk[ubRiskType].usChance;
	bBaseEffect = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].Risk[ubRiskType].bBaseEffect;
	ubRange = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].Risk[ubRiskType].ubRange;

	// For now, only these variables can effect risk outcome. In the future there may be more effects, like traits
	// and personality, or even sex.
	INT16 ubStrength = EffectiveStrength( pSoldier, FALSE );
	INT16 ubAgility = EffectiveAgility( pSoldier, FALSE );
	INT16 ubDexterity = EffectiveDexterity( pSoldier, FALSE );
	UINT8 ubHealth = __min(pSoldier->stats.bLife, pSoldier->stats.bLifeMax);
	INT16 ubWisdom = EffectiveWisdom( pSoldier );
	UINT8 ubLeadership = EffectiveLeadership( pSoldier );
	UINT8 ubExplosives = EffectiveExplosive( pSoldier );
	UINT8 ubExpLevel = EffectiveExpLevel( pSoldier );
	UINT8 ubLocalLoyalty = 0;
	if (GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY ) != BLANK_SECTOR )
	{
		ubLocalLoyalty = gTownLoyalty[ GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY ) ].ubRating;
	}
	else
	{
		ubLocalLoyalty = 0;
	}

	///////////////
	INT16 sAbsoluteMaxResult;
	INT16 sAbsoluteMinResult;

	// Calculate absolute maximum variance. The result can never be outside these.
	if (bBaseEffect < 0)
	{
		sAbsoluteMaxResult = __min((bBaseEffect + ubRange), -1);
		sAbsoluteMinResult = bBaseEffect - ubRange;
	}
	else if (bBaseEffect > 0)
	{
		sAbsoluteMaxResult = bBaseEffect + ubRange;
		sAbsoluteMinResult = __max(1, (bBaseEffect - ubRange));
	}
	else
	{
		sAbsoluteMaxResult = bBaseEffect + ubRange;
		sAbsoluteMinResult = bBaseEffect - ubRange;
	}

	INT16 bCombinedStats;

	// Begin calculating the effect of skills on increasing/reducing final result. Use a mix of stats to reach a range
	// of 0 to 100.

	switch (ubRiskType)
	{
		case RISK_STRENGTH:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubAgility * 0.3) + (ubExpLevel * 5));
			break;
		case RISK_DEXTERITY:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.4) + (ubExpLevel * 6));
			break;
		case RISK_AGILITY:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubExpLevel * 8));
			break;
		case RISK_HEALTH:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.5) + (ubExpLevel * 5));
			break;
		case RISK_WISDOM:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.6) + (ubExpLevel * 4));
			break;
		case RISK_MARKSMANSHIP:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubAgility * 0.4) + (ubExpLevel * 4));
			break;
		case RISK_MEDICAL:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.3) + (ubDexterity * 0.4) + (ubExpLevel * 3));
			break;
		case RISK_MECHANICAL:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.3) + (ubDexterity * 0.5) + (ubExpLevel * 2));
			break;
		case RISK_LEADERSHIP:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.4) + (ubLeadership * 0.3) + (ubExpLevel * 3));
			break;
		case RISK_EXPLOSIVES:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubAgility * 0.3) + (ubDexterity * 0.3) + (ubExpLevel * 2));
			break;
		case RISK_INJURY:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubDexterity * 0.2) + (ubAgility * 0.4) + (ubExpLevel * 2));
			break;
		case RISK_MORALE:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubLeadership * 0.4) + (ubExpLevel * 4));
			break;
		case RISK_FATIGUE:
			bCombinedStats = (INT16)__min(100, (ubAgility * 0.1) + (ubStrength * 0.3) + (ubHealth * 0.2) + (ubExpLevel * 4));
			break;
		case RISK_DRUNK:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.5) + (ubHealth * 0.3) + (ubExpLevel * 2));
			break;
		case RISK_LOYALTY_LOCAL:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.2) + (ubLeadership * 0.3) + (ubStrength * 0.1) + (ubExpLevel * 2) + (ubLocalLoyalty * 0.2) );
			break;
		case RISK_LOYALTY_GLOBAL:
			bCombinedStats = (INT16)__min(100, (ubWisdom * 0.3) + (ubLeadership * 0.3) + (ubExpLevel * 2) + (ubLocalLoyalty * 0.2));
			break;
	}

	if (bCombinedStats >= 100)
	{
		// Character is as skilled as possible. Automatic "best" result.
		return (sAbsoluteMaxResult);
	}
	if (bCombinedStats <= 0)
	{
		// Character is completely inept? Strange. Automatic "worst" result.
		return (sAbsoluteMinResult);
	}

	// we now have a range of 0 to 100. Let's apply an externalized modifier of 0 to -100.
	bCombinedStats -= gGameExternalOptions.ubFacilityDangerRate;

	// The chance of triggering an event is also based on stats.
	if (bBaseEffect > 0 )
	{
		// Chance increases with good stats, decreased with bad stats.
		iChance = iChance + ((bCombinedStats * iChance) / 100 );
	}
	else if (bBaseEffect < 0 )
	{
		// Chance decreases with good stats, increases with bad stats.
		iChance = iChance - ((bCombinedStats * iChance) / 100 );
	}
	else
	{
		// Chance is unaffected
	}

        // Limit iChance into a 0 to 50000 range.
        iChance = __max(0, __min(iChance, 50000));

	// Alright, let's see if the event is triggered.
	if (PreRandom(gGameExternalOptions.usFacilityEventRarity)>(UINT32)iChance)
	{
		// Chance failed to trigger.
		return (0);
	}
	else
	{
		// Let's find out how bad/good the result is.

		// By now, the Combined Stats variable is anywhere between -100 and +100.
		// We use this to move the Base Effect point from its original location, effectively giving us a better
		// or worse result based on our stats.
		bBaseEffect += (bCombinedStats * (ubRange+1)) / 100;
		bBaseEffect = __min(bBaseEffect, sAbsoluteMaxResult);
		bBaseEffect = __max(bBaseEffect, sAbsoluteMinResult);

		// The deviation is also affected by our stats.
		ubRange -= abs((bCombinedStats * (ubRange+1)) / 100); // If the Base Effect has moved any, then the range shrinks accordingly.

		// Effectively what happens is that skilled characters will get better results more consistently, and avoid
		// bad results consistently as well.
		// The opposite is also true - characters with insufficient skills tend to get worse results consistently,
		// and rarely (if ever) get the best results possible.

		//The final result is:
		Result = bBaseEffect + ((PreRandom((ubRange*2)+1)) - ubRange);

		// Naturally, the character can't ever breach the range of possible results as defined by the facility data.
		Result = __min(Result, sAbsoluteMaxResult);
		Result = __max(Result, sAbsoluteMinResult);

		return (Result);
	}
}


void HandleHourlyRisks()
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier;


	while (gCharactersList[iCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			// Run this character's risks.
			HandleRisksForSoldier( pSoldier );
		}
		iCounter++;
	}
}

void HandleRisksForSoldier( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter = 0;
	INT32 iCounterB = 0;
	UINT8 ubFacilityType = 0;
	UINT8 ubAssignmentType = 0;

	if (pSoldier->sFacilityTypeOperated != -1 && GetSoldierFacilityAssignmentIndex( pSoldier ) != -1)
	{
		// Soldier is working at a specific facility. Risks associated with this specific facility and assignment
		// have priority.
		ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );
		ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
		HandleRisksForSoldierFacilityAssignment( pSoldier, ubFacilityType, ubAssignmentType );
	}

	// Test for triggers related to any AMBIENT facility in the same sector.
	for (iCounterB; iCounterB < NUM_FACILITY_TYPES; iCounterB++)
	{
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][iCounterB].fFacilityHere)
		{
			// Test this facility for ambient effects.
			HandleRisksForSoldierFacilityAssignment( pSoldier, iCounterB, FAC_AMBIENT );
		}
	}
}

void HandleRisksForSoldierFacilityAssignment( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType )
{
	INT32 iCounter = 0;
	INT32 Result = 0;
	INT32 iNewValue = 0;

	CHAR16 sString[256];
	CHAR16 szSectorGrid[10];
	SGPRect pCenteringRect= {0, 0, 640, INV_INTERFACE_START_Y };

	UINT8 ubTownID = 0;
	BOOLEAN fOperatingFacility = (GetSoldierFacilityAssignmentIndex( pSoldier ) == -1) ? FALSE : TRUE;

	BOOLEAN fBadResult = FALSE;

	UINT16 usColor = FONT_MCOLOR_LTRED;

	for (iCounter; iCounter < NUM_RISKS; iCounter++)
	{
		fBadResult = FALSE;

		if (gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].Risk[iCounter].usChance > 0)
		{
			if (iCounter == RISK_LOYALTY_LOCAL)
			{
				ubTownID = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
				if (ubTownID == BLANK_SECTOR)
				{
					// No town here, so this risk doesn't have any effect anyway...
					continue;
				}
			}

			Result = FacilityRiskResult( pSoldier, iCounter, ubFacilityType, ubAssignmentType);
			if (Result != 0)
			{
				switch (iCounter)
				{
					case RISK_STRENGTH:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usStrengthSubpointsToImprove;
							StatChange( pSoldier, STRAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bStrength - 1))
								Result = -1*(pSoldier->stats.bStrength - 1);
							pSoldier->stats.bStrength += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_STRENGTH ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, STRAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[0], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[0], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_AGILITY:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usAgilitySubpointsToImprove;
							StatChange( pSoldier, AGILAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bAgility - 1))
								Result = -1*(pSoldier->stats.bAgility - 1);
							pSoldier->stats.bAgility += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_AGILITY ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeAgilityTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( AGIL_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bAgility	= pSoldier->stats.bAgility;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, AGILAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[1], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[1], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_DEXTERITY:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usDexteritySubpointsToImprove;
							StatChange( pSoldier, DEXTAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bDexterity - 1))
								Result = -1*(pSoldier->stats.bDexterity - 1);
							pSoldier->stats.bDexterity += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_DEXTERITY ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( DEX_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bDexterity	= pSoldier->stats.bDexterity;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->name, FALSE, Result, DEXTAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[2], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[2], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_WISDOM:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usWisdomSubpointsToImprove;
							StatChange( pSoldier, WISDOMAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bWisdom - 1))
								Result = -1*(pSoldier->stats.bWisdom - 1);
							pSoldier->stats.bWisdom += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_WISDOM ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeWisdomTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( WIS_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bWisdom	= pSoldier->stats.bWisdom;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, WISDOMAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[3], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[3], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_HEALTH:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usHealthSubpointsToImprove;
							StatChange( pSoldier, HEALTHAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bLifeMax - 1))
								Result = -1*(pSoldier->stats.bLifeMax - 1);
							pSoldier->stats.bLifeMax += Result;
							pSoldier->stats.bLife += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_HEALTH ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeHealthTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( HEALTH_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bLifeMax	= pSoldier->stats.bLifeMax;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							if (pSoldier->stats.bLife < OKLIFE)
							{
								HandleTakeDamageDeath( pSoldier, 0, TAKE_DAMAGE_BLOODLOSS );
							}
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, HEALTHAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							if (pSoldier->stats.bLife >= OKLIFE)
							{
								// Do Screen Message and stop time.
								GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
								if (!fOperatingFacility)
									swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[4], szSectorGrid );
								else
									swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[4], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
								DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
								StopTimeCompression();
							}
							else if(pSoldier->stats.bLife < OKLIFE && pSoldier->stats.bLife > 0)
							{
								// Soldier's health is driven very low. Automatically removed from duty.
								// Do Screen Message and stop time.
								GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
								if (!fOperatingFacility)
									swprintf( sString, gzFacilityErrorMessage[28], pSoldier->GetName(), szSectorGrid );
								else
									swprintf( sString, gzFacilityErrorMessage[29], pSoldier->GetName(), gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
								AddCharacterToAnySquad( pSoldier );
								DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
								StopTimeCompression();
							}
						}
						break;
					case RISK_MARKSMANSHIP:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usMarksmanshipSubpointsToImprove;
							StatChange( pSoldier, MARKAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bMarksmanship - 1))
								Result = -1*(pSoldier->stats.bMarksmanship - 1);
							pSoldier->stats.bMarksmanship += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_MARKSMANSHIP ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeMarksmanshipTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( MRK_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bMarksmanship	= pSoldier->stats.bMarksmanship;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, MARKAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[5], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[5], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_LEADERSHIP:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usLeadershipSubpointsToImprove;
							StatChange( pSoldier, LDRAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bLeadership - 1))
								Result = -1*(pSoldier->stats.bLeadership - 1);
							pSoldier->stats.bLeadership += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_LEADERSHIP ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeLeadershipTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( LDR_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bLeadership	= pSoldier->stats.bLeadership;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, LDRAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[6], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[6], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_MECHANICAL:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usMechanicalSubpointsToImprove;
							StatChange( pSoldier, MECHANAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bMechanical - 1))
								Result = -1*(pSoldier->stats.bMechanical - 1);
							pSoldier->stats.bMechanical += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_MECHANICAL ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeMechanicalTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( MECH_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bMechanical	= pSoldier->stats.bMechanical;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, MECHANAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[7], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[7], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_MEDICAL:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usMedicalSubpointsToImprove;
							StatChange( pSoldier, MEDICALAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bMedical - 1))
								Result = -1*(pSoldier->stats.bMedical - 1);
							pSoldier->stats.bMedical += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_MEDICAL ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeMedicalTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( MED_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bMedical	= pSoldier->stats.bMedical;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, MEDICALAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
														if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[8], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[8], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_EXPLOSIVES:
						if (Result > 0)
						{
							// Soldier doesn't gain stat points directly, instead he gets a number of chances
							// equal to Result*SubPointsPerLevel. Optimally this leads to gaining a number of
							// levels equal to result, but this isn't always the case.
							Result *= gGameExternalOptions.usExplosivesSubpointsToImprove;
							StatChange( pSoldier, EXPLODEAMT, Result, TRUE );
						}
						else
						{
							// Soldier is suffering direct stat damage.
							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							if ( (-1*Result) > (pSoldier->stats.bExplosive - 1))
								Result = -1*(pSoldier->stats.bExplosive - 1);
							pSoldier->stats.bExplosive += Result;
							if ( gGameOptions.fNewTraitSystem && (Result <= 0) )
							{
								pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_EXPLOSIVES ] -= Result;
							}
							else
							{
								// make this stat RED for a while...
								pSoldier->timeChanges.uiChangeExplosivesTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( EXP_INCREASE );
							}
							// Update Profile
							gMercProfiles[ pSoldier->ubProfile ].bExplosive	= pSoldier->stats.bExplosive;

							// merc records - stat damaged
							if( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							///////////////////////////////////////////////////////////////////////////////////////////
							fBadResult = TRUE;

							// Log message
							BuildStatChangeString( sString, pSoldier->GetName(), FALSE, Result, EXPLODEAMT );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// Do Screen Message and stop time.
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[17], pSoldier->GetName(), gzFacilityRiskResultStrings[9], szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[18], pSoldier->GetName(), gzFacilityRiskResultStrings[9], gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
							DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
							StopTimeCompression();
						}
						break;
					case RISK_INJURY:
						// This can only be a negative effect... for now.
						if (Result < 0)
						{
							// Soldier is suffering direct unbandaged injury.
							pSoldier->SoldierTakeDamage( 0, abs(Result), 0, abs(Result), TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, FALSE );
							fBadResult = TRUE;

							// SANDRO - add to merc records - facility accidents counter
							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;

							if (pSoldier->stats.bLife > 0)
							{
								if (pSoldier->bBleeding > MIN_BLEEDING_THRESHOLD)
								{
									// Log message
									GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
									swprintf( sString, gzFacilityErrorMessage[31], pSoldier->GetName(), szSectorGrid );
									ScreenMsg( usColor, MSG_INTERFACE, sString );

									// Do Screen Message, stop time, and take character off duty immediately.
									GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
									if (!fOperatingFacility)
										swprintf( sString, gzFacilityErrorMessage[19], pSoldier->GetName(), szSectorGrid );
									else
										swprintf( sString, gzFacilityErrorMessage[20], pSoldier->GetName(), gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
									AddCharacterToAnySquad( pSoldier );
									DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
									StopTimeCompression();
								}
								else
								{
									// Log message
									GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
									swprintf( sString, gzFacilityErrorMessage[30], pSoldier->GetName(), szSectorGrid );
									ScreenMsg( usColor, MSG_INTERFACE, sString );

									// Soldier isn't bleeding too bad. Let the player know, but don't take any action.
									// Do Screen Message and stop time.
									GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
									if (!fOperatingFacility)
										swprintf( sString, gzFacilityErrorMessage[21], pSoldier->GetName(), szSectorGrid );
									else
										swprintf( sString, gzFacilityErrorMessage[22], pSoldier->GetName(), gFacilityTypes[ubFacilityType].szFacilityName, szSectorGrid );
									DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
									StopTimeCompression();
								}
							}
						}
						break;
					case RISK_MORALE:
						pSoldier->aiData.bMorale = __min(100,__max(0, pSoldier->aiData.bMorale + Result));
						pSoldier->aiData.bStrategicMoraleMod = __min(50,__max(-50, pSoldier->aiData.bStrategicMoraleMod + Result));
						RefreshSoldierMorale( pSoldier );
						// SANDRO - add to merc records - facility accidents counter
						if ( Result < 0 )
							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
						break;
					case RISK_FATIGUE:
						pSoldier->bBreathMax = __min(100,__max(0, pSoldier->bBreathMax + Result));
						pSoldier->bBreath = pSoldier->bBreathMax;
						// SANDRO - add to merc records - facility accidents counter
						if ( Result < 0 )
							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
						break;
					case RISK_DRUNK:

						// Has soldier just drunk enough to make him drunk off his head?
						if (GetDrunkLevel(pSoldier) == BORDERLINE)
						{
							// Log message
							swprintf( sString, gzFacilityErrorMessage[27], pSoldier->GetName() );
							ScreenMsg( usColor, MSG_INTERFACE, sString );
						}

						// Add effects
						pSoldier->AddDrugValues( DRUG_TYPE_ALCOHOL, Drug[DRUG_TYPE_ALCOHOL].ubDrugEffect, Drug[DRUG_TYPE_ALCOHOL].ubDrugTravelRate, Drug[DRUG_TYPE_ALCOHOL].ubDrugSideEffect );

						// ATE: Make guy collapse from heart attack if too much stuff taken....
						if ( pSoldier->drugs.bDrugSideEffectRate[ DRUG_TYPE_ALCOHOL ] > (  Drug[DRUG_TYPE_ALCOHOL].ubDrugSideEffect * 3 ) )
						{
							if ( pSoldier->ubProfile == LARRY_NORMAL )
							{
								pSoldier = SwapLarrysProfiles( pSoldier );
							}
							else if ( pSoldier->ubProfile == LARRY_DRUNK )
							{
								gMercProfiles[ LARRY_DRUNK ].bNPCData = 0;
							}

							// Keel over...
							DeductPoints( pSoldier, 0, 10000 );

							///////////////////////////////////////////////////////////////////////////////////////////
							// SANDRO - if our stat is damaged through facility event, make it healable
							// Permanently lower certain stats...
							if ( gGameOptions.fNewTraitSystem )
							{
								// WISDOM decrease
								if ( pSoldier->stats.bWisdom > 5 )
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_WISDOM ] += 5;
									pSoldier->stats.bWisdom -= 5;
								}
								else
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_WISDOM ] += (pSoldier->stats.bWisdom - 1);
									pSoldier->stats.bWisdom = 1;
								}
								// DEXTERITY decrease
								if ( pSoldier->stats.bDexterity > 5 )
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_DEXTERITY ] += 5;
									pSoldier->stats.bDexterity -= 5;
								}
								else
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_DEXTERITY ] += (pSoldier->stats.bDexterity - 1);
									pSoldier->stats.bDexterity = 1;
								}
								// STRENGTH decrease
								if ( pSoldier->stats.bStrength > 5 )
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_STRENGTH ] += 5;
									pSoldier->stats.bStrength -= 5;
								}
								else
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_STRENGTH ] += (pSoldier->stats.bStrength - 1);
									pSoldier->stats.bStrength = 1;
								}
								// AGILITY decrease
								if ( pSoldier->stats.bAgility > 5 )
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_AGILITY ] += 5;
									pSoldier->stats.bAgility -= 5;
								}
								else
								{
									pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_AGILITY ] += (pSoldier->stats.bAgility - 1);
									pSoldier->stats.bAgility = 1;
								}
							}
							else // old system
							{
								pSoldier->stats.bAgility	= __max(1, pSoldier->stats.bWisdom-5);
								pSoldier->stats.bDexterity	= __max(1, pSoldier->stats.bDexterity-5);
								pSoldier->stats.bStrength	= __max(1, pSoldier->stats.bStrength-5);
								pSoldier->stats.bAgility	= __max(1, pSoldier->stats.bAgility-5);

								// make those stats RED for a while...
								// SANDRO - we don't need to do this with new system, as we simply show all damaged stats in red until healed
								pSoldier->timeChanges.uiChangeWisdomTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( WIS_INCREASE );
								pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( DEX_INCREASE );
								pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );
								pSoldier->timeChanges.uiChangeAgilityTime = GetJA2Clock();
								pSoldier->usValueGoneUp &= ~( AGIL_INCREASE );
							}

							// export stat changes to profile
							gMercProfiles[ pSoldier->ubProfile ].bWisdom	= pSoldier->stats.bWisdom;
							gMercProfiles[ pSoldier->ubProfile ].bDexterity = pSoldier->stats.bDexterity;
							gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;
							gMercProfiles[ pSoldier->ubProfile ].bAgility	= pSoldier->stats.bAgility;

							fBadResult = TRUE; // stop the time, call a doctor, we had a heart attack!

							// merc records - stat damaged
							gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;
							gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}
						break;
					case RISK_LOYALTY_LOCAL:
						Result *= GAIN_PTS_PER_LOYALTY_PT; // Transform points into actual loyalty.
						if (Result > 0)
						{
							IncrementTownLoyalty( ubTownID, Result );
						}
						else
						{
							DecrementTownLoyalty( ubTownID, abs(Result) );
							// Log message
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[23], pTownNames[ubTownID], pSoldier->GetName() );
							else
								swprintf( sString, gzFacilityErrorMessage[24], pTownNames[ubTownID], pSoldier->GetName(), gFacilityTypes[ubFacilityType].szFacilityName );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// SANDRO - add to merc records - facility accidents counter
							if ( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
						}
						break;
					case RISK_LOYALTY_GLOBAL:
						Result *= GAIN_PTS_PER_LOYALTY_PT; // Transform points into actual loyalty.
						if (Result > 0)
						{
							IncrementTownLoyaltyEverywhere( Result );
						}
						else
						{
							DecrementTownLoyaltyEverywhere( abs(Result) );
							// Log message
							GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, szSectorGrid );
							if (!fOperatingFacility)
								swprintf( sString, gzFacilityErrorMessage[25], pSoldier->GetName(), szSectorGrid );
							else
								swprintf( sString, gzFacilityErrorMessage[26], pSoldier->GetName(), szSectorGrid, gFacilityTypes[ubFacilityType].szFacilityName );
							ScreenMsg( usColor, MSG_INTERFACE, sString );

							// SANDRO - add to merc records - facility accidents counter
							if ( Result < 0 )
								gMercProfiles[ pSoldier->ubProfile ].records.usFacilityAccidents++;
						}
						break;
					default:
						break;
				}
				if (fBadResult)
				{
					StopTimeCompression();
				}
			}
		}
	}
}

INT32 GetTotalFacilityHourlyCosts( BOOLEAN fPositive )
{
	SOLDIERTYPE *pSoldier;
	INT32 iTotal = 0;

	UINT8 ubCounter = 0;

	// Run through list of player-characters
	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character truly valid?
		if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
				pSoldier->bSectorZ == 0  && pSoldier != NULL &&
				pSoldier->stats.bLife >= OKLIFE &&
				!(pSoldier->flags.fMercAsleep) )
		{
			INT8 ubAssignmentType = GetSoldierFacilityAssignmentIndex( pSoldier );
			if (ubAssignmentType == -1)
			{
				// Skip this character.
				ubCounter++;
				continue;
			}

			//UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
			INT16 ubFacilityType = pSoldier->sFacilityTypeOperated;

			if (!fPositive && ubFacilityType != -1 && // We want facilities that cost money to operate
				gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour > 0) // This facility costs money
			{
				iTotal += gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour;
			}
			else if (fPositive && ubFacilityType != -1 && // We want facilities that GENERATE money
				gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour < 0)
			{
				// Flip the value to positive
				iTotal += (-1 * gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour);
			}
		}
		ubCounter++;
	}

	return (iTotal);
}

void InitFacilities()
{
	// Initialize facility accounts.
	giTotalEarnedForFacilityOperationsToday = 0;
	giTotalOwedForFacilityOperationsToday = 0;
	gfOutstandingFacilityDebt = FALSE;
}
