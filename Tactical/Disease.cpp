/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "Disease.h"

#include "Overhead.h"
#include "random.h"
#include "Assignments.h"
#include "message.h"
#include "Soldier Profile.h"
#include "Map Screen Interface Map.h"
#include "Queen Command.h"
#include "Quests.h"
#include "finances.h"
#include "Game Clock.h"
#include "LaptopSave.h"
#include "strategic.h"
#include "DynamicDialogue.h"
#include <math.h>
#include "Drugs And Alcohol.h"	// for DoesMercHaveDisability( ... )
#include "environment.h"

//GLOBALS
DISEASE Disease[NUM_DISEASES];

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// if disease in a sector is high enough, there is a chance that neighbouring sectors can be affected too
#define DISEASE_STRATEGIC_ADJACENTINFECTION		0.3

#define MAPROWS		(MAP_WORLD_X - 2)
#define	MAPCOLS		(MAP_WORLD_Y - 2)

// percentage of population that has at least some medical skill
FLOAT GetCivPopulationDoctorRate( )			{ return 0.1f; }

// percentage of army population that has at least some medical skill
FLOAT GetMilitaryPopulationDoctorRate( )	{ return 0.34f; }

// healing points of a population doctor
FLOAT GetPopulationDoctorPoints( )			{ return 20; }

// if this fraction of a sector's population is infected, outbreak is declared regardless of infection severity
// Otherwise infection would be untreatable at this point
FLOAT GetSectorDiseaseOverFlowThreshold( )
{
	/* disease in a sector is automatically revealed if the mean severity breaches the outbreak threshold
	* however, we add a second condition: if a high enough percentage of the population is infected, disease is also revealed
	* the reason is that otherwise the AI might have no chance at all to combat an outbreak
	* It thus follows that
	* population * percentage of doctors * healing points per doctor == population * disease overflow threshold * infection gain per hour
	* which leads to
	* disease overflow threshold = percentage of doctors * healing points per doctor / infection gain per hour
	*/
	if ( Disease[0].sInfectionPtsGainPerHour > 0 )
		return GetCivPopulationDoctorRate( ) * GetPopulationDoctorPoints( ) / (FLOAT)Disease[0].sInfectionPtsGainPerHour;

	return 0.3f;
}

void HandleDisease()
{
	if ( !gGameExternalOptions.fDisease )
		return;

	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;

	// increase existing diseases
	for ( uiCnt = 0, pSoldier = MercPtrs[uiCnt]; uiCnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++uiCnt, ++pSoldier )
	{
		if ( pSoldier->bActive )
		{
			for ( int i = 0; i < NUM_DISEASES; ++i )
			{
				if ( pSoldier->sDiseasePoints[i] > 0 )
				{
					// add disease points - some diseases can reverse on certain states
					if ( pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_REVERSEAL )
						pSoldier->AddDiseasePoints( i, -Disease[i].sInfectionPtsGainPerHour );
					else
						pSoldier->AddDiseasePoints( i, Disease[i].sInfectionPtsGainPerHour );
				}
			}
		}
	}

	// chance for infection due to proximity to other mercs
	for ( uiCnt = 0, pSoldier = MercPtrs[uiCnt]; uiCnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++uiCnt, ++pSoldier )
	{
		if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )
		{
			// loop over all other soldiers and determine the chance that they will infect us
			SOLDIERTYPE *pTeamSoldier = NULL;
			UINT32 uiCnt2 = 0;
						
			// chance for infection due to proximity to other mercs
			for ( uiCnt2 = 0, pTeamSoldier = MercPtrs[uiCnt2]; uiCnt2 <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++uiCnt2, ++pTeamSoldier )
			{
				if ( pTeamSoldier->bActive && pTeamSoldier != pSoldier 
					 && pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ
					 && pTeamSoldier->stats.bLife > 0 )
				{
					// infection chance gets modified depending on assignments
					FLOAT modifier = 1.0f;

					// if both are in the same squad or vehicle, chance is increased
					if (pTeamSoldier->bAssignment == pSoldier->bAssignment &&
						 (pSoldier->bAssignment < ON_DUTY ||
						 (pSoldier->bAssignment == VEHICLE && pSoldier->iVehicleId == pTeamSoldier->iVehicleId)))
					{
						modifier = 1.5;
					}
					// doctor-patient relations increase chance even more
					else if (IS_PATIENT(pTeamSoldier->bAssignment) && IS_PATIENT(pSoldier->bAssignment))
					{
						modifier = 4;
					}

					// we might get a disease from this...
					HandlePossibleInfection( pSoldier, pTeamSoldier, INFECTION_TYPE_CONTACT_HUMAN, modifier );
				}
			}

			// we can also be infected by the disease left from corpses
			if ( gGameExternalOptions.fDiseaseStrategic )
			{
				SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )] );

				if ( pSectorInfo && pSectorInfo->fDiseasePoints )
				{
					HandlePossibleInfection( pSoldier, NULL, INFECTION_TYPE_CONTACT_CORPSE, ( pSectorInfo->fDiseasePoints / DISEASE_MAX_SECTOR ) );
				}
			}

			// if we are infected, this might disgust other people, but only if it's visisble to them (we assume doctors dont mention this to others)
			for ( int i = 0; i < NUM_DISEASES; ++i )
			{
				if ( (Disease[i].usDiseaseProperties & DISEASE_PROPERTY_DISGUSTING) && 
					 (pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_DIAGNOSED) && 
					 (pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_OUTBREAK) )
				{
					HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_DISEASE_DISGUSTING, TRUE, TRUE );
					break;
				}
			}
		}
	}

	// chance for infection due sector
	for ( uiCnt = 0, pSoldier = MercPtrs[uiCnt]; uiCnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++uiCnt, ++pSoldier )
	{
		if ( pSoldier->bActive )
		{
			UINT8 ubSector = (UINT8)SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
			UINT8 ubTraverseType = SectorInfo[ubSector].ubTraversability[THROUGH_STRATEGIC_MOVE];

			switch ( ubTraverseType )
			{
			case TROPICS_SAM_SITE:
			case TROPICS:
			case TROPICS_ROAD:
				{
					HandlePossibleInfection( pSoldier, NULL, INFECTION_TYPE_TROPICS );
				}
				break;
			case SWAMP:
			case SWAMP_ROAD:
				{
					HandlePossibleInfection( pSoldier, NULL, INFECTION_TYPE_SWAMP );
				}
				break;
			default:
				break;
			}
		}
	}
}

// chance gets modified by aModifier (contextual modifier)
void HandlePossibleInfection( SOLDIERTYPE *pSoldier, SOLDIERTYPE* pOtherSoldier, UINT8 aInfectionType, FLOAT aModifier, BOOLEAN fStrategicOnly )
{
	if ( !gGameExternalOptions.fDisease )
		return;
	
	// only for living mercs with a profile with a valid infection method
	if ( !pSoldier || pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE || pSoldier->ubProfile == NO_PROFILE || aInfectionType >= INFECTION_TYPE_MAX )
		return;

	int max = fStrategicOnly ? 1 : NUM_DISEASES;
	for ( int i = 0; i < max; ++i )
	{
		// do not infect us if we are already infected
		if ( !(Disease[i].usDiseaseProperties & DISEASE_PROPERTY_CANREINFECT) && pSoldier->sDiseasePoints[i] > 0 )
			continue;

		// chance of infection by insects
		FLOAT dChance = Disease[i].dInfectionChance[aInfectionType];

		// alter chance by modifier and disease resistance
		dChance = dChance * aModifier * (100 - pSoldier->GetDiseaseResistance( )) / 100;

		if ( aInfectionType == INFECTION_TYPE_TROPICS || aInfectionType == INFECTION_TYPE_SWAMP )
		{
			// if we are afraid of insects, we will be more careful around them - lower chance to be infected
			if ( DoesMercHaveDisability( pSoldier, FEAR_OF_INSECTS ) )
				dChance *= 0.7f;
		}
		else if ( aInfectionType == INFECTION_TYPE_CONTACT_HUMAN )
		{
			// if we check a specific soldier, he must have the disease himself
			if ( pOtherSoldier && pOtherSoldier->sDiseasePoints[i] <= 0 )
				dChance = 0;

			// if we wear face or hand protection, lower chance of infection
			dChance *= (1.0f - pSoldier->GetDiseaseContactProtection( ));
		}
		else if ( aInfectionType == INFECTION_TYPE_CONTACT_CORPSE )
		{
			// if we wear face or hand protection, lower chance of infection
			dChance *= (1.0f - pSoldier->GetDiseaseContactProtection( ));
		}

		// chances ca be smaller than 1%, so we use a trick here by altering ou 'chance function'. This allows to have much smaller chances, as for diseases, 1% can be way too high.
		if ( Random( 10000 ) < dChance * 100 )
		{
			// infect us
			pSoldier->Infect( i );
		}
	}
}

// return number of adjacent sector or -1 if there is none
INT16 GetAdjacentSector( UINT8 sector, UINT8 spdir )
{
	switch ( spdir )
	{
	case SP_UP:
		if ( SECTORY( sector ) > 0 )
			return sector - MAPCOLS;
		break;

	case SP_LEFT:
		if ( SECTORX( sector ) > 0 )
			return sector - 1;
		break;

	case SP_RIGHT:
		if ( SECTORX( sector ) < MAPCOLS )
			return sector + 1;
		break;

	case SP_DOWN:
		if ( SECTORY( sector ) < MAPCOLS )
			return sector + MAPCOLS;
		break;
	}

	return -1;
}


// get a sector population (not the tactical one - we use an xml estimation + troops present)
UINT16 GetSectorPopulation( INT16 sX, INT16 sY, BOOLEAN fWithMilitary )
{
	UINT8 sector = SECTOR( sX, sY );

	UINT16 population = SectorExternalData[sector][0].usCivilianPopulation;

	if ( fWithMilitary )
	{
		// add number of enemies...
		population += NumNonPlayerTeamMembersInSector( sX, sY, ENEMY_TEAM );
	}

	return population;
}

void HandleDiseaseDailyRefresh()
{
	if ( gGameExternalOptions.fDisease && gGameExternalOptions.fDiseaseStrategic && gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED] && LaptopSaveInfo.iCurrentBalance > gGameExternalOptions.sDiseaseWHOSubscriptionCost )
	{
		gubFact[FACT_DISEASE_WHODATA_ACCESS] = TRUE;

		AddTransactionToPlayersBook( WHO_SUBSCRIPTION, 0, GetWorldTotalMin( ), (-1) * (INT32)gGameExternalOptions.sDiseaseWHOSubscriptionCost );
	}
	else
	{
		gubFact[FACT_DISEASE_WHODATA_ACCESS] = FALSE;
	}
}

// heal sector population, return number of points used
UINT32 HealSectorPopulation( INT16 sX, INT16 sY, UINT32 uHealpts )
{
	// how many points are needed to cure all disease?
	UINT8 sector = SECTOR( sX, sY );

	SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

	if ( !pSectorInfo )
		return 0;

	UINT32 ptsused = uHealpts;
	
	if ( uHealpts >= pSectorInfo->fDiseasePoints )
	{
		uHealpts -= pSectorInfo->fDiseasePoints;
		pSectorInfo->fDiseasePoints = 0;
	}
	else
	{
		pSectorInfo->fDiseasePoints -= uHealpts;
		uHealpts = 0;
	}
	
	// if there are no more infected, or infection is at 0, remove all traces of disease
	if ( pSectorInfo->fDiseasePoints <= 0 )
	{
		pSectorInfo->usInfectionFlag &= ~(SECTORDISEASE_DIAGNOSED_PLAYER);
	}

	ptsused -= uHealpts;

	return ptsused;
}

// Flugente: get workforce effectiveness with regards to strategic disease
FLOAT GetWorkforceEffectivenessWithDisease( INT8 bTownId, UINT8 usTeam )
{
	if ( !gGameExternalOptions.fDisease || !gGameExternalOptions.fDiseaseStrategic )
		return 1.0f;

	FLOAT val = 0.0f;
	UINT16 population_healthy = 0;
	UINT16 population_total = 0;

	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
		{
			UINT16 strategicsector = CALCULATE_STRATEGIC_INDEX( sX, sY );

			if ( StrategicMap[strategicsector].bNameId == bTownId )
			{
				// if we check for the player team, we look for sectors free of the enemy
				if ( OUR_TEAM == usTeam && StrategicMap[strategicsector].fEnemyControlled )
					continue;
				else if ( ENEMY_TEAM == usTeam && !StrategicMap[strategicsector].fEnemyControlled )
					continue;

				UINT8 sector = SECTOR( sX, sY );
				SECTORINFO *pSector = &(SectorInfo[sector]);

				if ( !pSector )
					continue;

				// at least 1 to be safe
				UINT16 population = max(1, GetSectorPopulation( sX, sY, FALSE ) );

				population_total += population;

				// only civilian population is relevant here
				if ( population )
				{
					// workforce effectivity is linearly dependent on disease
					population = max( 0, ( ( DISEASE_MAX_SECTOR - pSector->fDiseasePoints) / DISEASE_MAX_SECTOR ) *  population );
				}

				population_healthy += population;
			}
		}
	}

	val = (FLOAT)(population_healthy) / (FLOAT)(population_total);

	return val;
}
