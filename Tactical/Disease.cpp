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

	return 0.3;
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

					// if both are in the same squad, chance is increased
					if ( pTeamSoldier->bAssignment == pSoldier->bAssignment && pSoldier->bAssignment < ON_DUTY )
						modifier = 1.5;
					// doctor-patient relations increase chance even more
					else if ( (pTeamSoldier->bAssignment == DOCTOR || pTeamSoldier->bAssignment == PATIENT) && (pSoldier->bAssignment == DOCTOR || pSoldier->bAssignment == PATIENT) )
						modifier = 4;

					// we might get a disease from this...
					HandlePossibleInfection( pSoldier, pTeamSoldier, INFECTION_TYPE_CONTACT_HUMAN, modifier );
				}
			}

			// we can also be infected by the population
			if ( gGameExternalOptions.fDiseaseStrategic )
			{
				UINT16 population = GetSectorPopulation( pSoldier->sSectorX, pSoldier->sSectorY );

				if ( population )
				{
					UINT8 sector = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );

					SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

					if ( pSectorInfo )
					{
						// if there are infected people, we might get an infection (disease 0 only)
						if ( pSectorInfo->usInfected )
						{
							// infection is also possible by human contact
							UINT32 usChance = Disease[0].usInfectionChance[INFECTION_TYPE_CONTACT_HUMAN];

							// if disease is known, mercs will avoid the infected, lowering the chance of them being infected
							UINT16 max = min(20, pSectorInfo->usInfected);
							if ( (pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_PLAYER) || (gubFact[FACT_DISEASE_WHODATA_ACCESS] && pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_WHO) )
								max /= 4;

							for ( UINT16 i = 0; i < max; ++i )
							{
								if ( Chance( usChance ) )
									HandlePossibleInfection( pSoldier, NULL, INFECTION_TYPE_CONTACT_HUMAN, 1.0f, TRUE );
							}
						}

						// if we are infected, WE might infect other people
						if ( pSoldier->sDiseasePoints[0] > 0 )
						{
							if ( pSectorInfo->usInfected < population )
							{
								if ( Chance( Disease[0].usInfectionChance[INFECTION_TYPE_CONTACT_HUMAN] ) )
								{
									FLOAT infectedseverity = (FLOAT)Disease[0].sInfectionPtsInitial / (FLOAT)Disease[0].sInfectionPtsFull;

									pSectorInfo->fInfectionSeverity = (pSectorInfo->fInfectionSeverity * pSectorInfo->usInfected + infectedseverity * 1) / (pSectorInfo->usInfected + 1);
									++pSectorInfo->usInfected;
								}
							}
						}
					}
				}
			}

			// if we are infected, this might disgust other people
			for ( int i = 0; i < NUM_DISEASES; ++i )
			{
				if ( (Disease[i].usDiseaseProperties & DISEASE_PROPERTY_DISGUSTING) && (pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_DIAGNOSED) )
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
			UINT8 ubTraverseType = SectorInfo[ubSector].ubTraversability[pSoldier->ubDirection];

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

	// now to handle strategic disease
	if ( !gGameExternalOptions.fDiseaseStrategic )
		return;

	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
		{
			UINT8 sector = SECTOR( sX, sY );

			SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

			if ( !pSectorInfo )
				continue;

			// first, existing infections get worse
			if ( pSectorInfo->fInfectionSeverity > 0 )
			{
				pSectorInfo->fInfectionSeverity = min( 1.0f, pSectorInfo->fInfectionSeverity + ( FLOAT )(Disease[0].sInfectionPtsGainPerHour) / (FLOAT)(Disease[0].sInfectionPtsFull) );
			}

			UINT16 population = GetSectorPopulation( sX, sY );

			if ( population )
			{
				// how many people are already infected?
				if ( pSectorInfo->usInfected < population )
				{
					UINT16 lefttoinfect = population - pSectorInfo->usInfected;
					UINT16 newinfected = 0;

					// population might get infected if this a swamp or tropical sector
					switch ( SectorInfo[sector].ubTraversability[THROUGH_STRATEGIC_MOVE] )
					{
						case TROPICS_SAM_SITE:
						case TROPICS:
						case TROPICS_ROAD:
						{
							UINT32 usChance = Disease[0].usInfectionChance[INFECTION_TYPE_TROPICS];
							for ( UINT16 i = 0; i < lefttoinfect; ++i )
							{
								if ( Chance( usChance) )
									++newinfected;
							}
						}
						break;
						case SWAMP:
						case SWAMP_ROAD:
						{
							UINT32 usChance = Disease[0].usInfectionChance[INFECTION_TYPE_SWAMP];
							for ( UINT16 i = 0; i < lefttoinfect; ++i )
							{
								if ( Chance( usChance ) )
									++newinfected;
							}
						}
						break;
					default:
						break;
					}

					if ( lefttoinfect - newinfected > 1 && pSectorInfo->usInfected )
					{
						// infection is also possible by human contact
						UINT32 usChance = Disease[0].usInfectionChance[INFECTION_TYPE_CONTACT_HUMAN];
						UINT16 max = sqrt( (FLOAT) min( lefttoinfect - newinfected, pSectorInfo->usInfected ) );
						for ( UINT16 i = 0; i < max; ++i )
						{
							if ( Chance( usChance ) )
								++newinfected;
						}
					}
					
					if ( lefttoinfect - newinfected > 0 )
					{
						// there is also the chance to be infected by bad food, sex, contact with animals etc.
						// For now, we assume this to be very rare events, so just add a small chance to be infected this way
						FLOAT populationpercentage = (FLOAT)(lefttoinfect - newinfected) / (FLOAT)(population);
						FLOAT basechance = sqrt( (FLOAT) min( lefttoinfect, pSectorInfo->usInfected + newinfected ) ) * 0.5f;

						FLOAT chance_sex = Disease[0].usInfectionChance[INFECTION_TYPE_SEX] * basechance * populationpercentage;
						FLOAT chance_corpse = 0;

						// if there was a fight here in the last 48 hours, then corpses will still be here - increase chance of infection
						if ( pSectorInfo->uiTimeLastPlayerLiberated && pSectorInfo->uiTimeLastPlayerLiberated + (48 * 3600) > GetWorldTotalSeconds( ) )
							chance_corpse = Disease[0].usInfectionChance[INFECTION_TYPE_CONTACT_CORPSE] * populationpercentage;
						
						if ( Chance( chance_sex ) )
							++newinfected;

						if ( Chance( chance_corpse ) )
							++newinfected;
					}

					if ( lefttoinfect - newinfected > 0 )
					{
						// adjacent sectors can infect us too
						for ( UINT8 dir = 0; dir < SP_DIR_MAX; ++dir )
						{
							INT16 othersector = GetAdjacentSector( sector, dir );

							if ( othersector > -1 )
							{
								UINT16 population_other = GetSectorPopulation( SECTORX( othersector ), SECTORY( othersector ) );
								SECTORINFO *pSectorInfo_Other = &(SectorInfo[othersector]);

								if ( pSectorInfo_Other && pSectorInfo_Other->usInfected && population_other )
								{
									FLOAT infectedothersector = (FLOAT)pSectorInfo_Other->usInfected / (FLOAT)(population_other);

									if ( infectedothersector > DISEASE_STRATEGIC_ADJACENTINFECTION )
									{
										FLOAT percentage = (FLOAT)((lefttoinfect - newinfected) * Disease[0].usInfectionChance[INFECTION_TYPE_CONTACT_HUMAN]) / (FLOAT)(100 * population);

										if ( Chance( 100 * percentage ) )
											++newinfected;
									}
								}
							}
						}
					}
					
					if ( pSectorInfo->usInfected + newinfected > 0 && Disease[0].sInfectionPtsFull )
					{
						FLOAT infectedseverity = (FLOAT)Disease[0].sInfectionPtsInitial / (FLOAT)Disease[0].sInfectionPtsFull;

						pSectorInfo->fInfectionSeverity = (pSectorInfo->fInfectionSeverity * pSectorInfo->usInfected + infectedseverity * newinfected) / (pSectorInfo->usInfected + newinfected);
						pSectorInfo->usInfected += newinfected;

						pSectorInfo->usInfected = min( pSectorInfo->usInfected , population);
					}
				}

				// if infection is high enough, disease officially breaks out
				FLOAT infectedpercentage = (FLOAT)pSectorInfo->usInfected / (FLOAT)(population);

				if ( infectedpercentage >= GetSectorDiseaseOverFlowThreshold() || pSectorInfo->fInfectionSeverity > ((FLOAT)Disease[0].sInfectionPtsOutbreak / (FLOAT)Disease[0].sInfectionPtsFull) )
				{
					pSectorInfo->usInfectionFlag |= (SECTORDISEASE_OUTBREAK | SECTORDISEASE_DIAGNOSED_WHO);
				}
				
				// if disease is known and doctoring isn't inhibited, use doctoring to remove disease
				// doctors (civilian and military) can fight the disease
				if ( (pSectorInfo->usInfectionFlag & (SECTORDISEASE_DIAGNOSED_WHO | SECTORDISEASE_DIAGNOSED_PLAYER)) && !pSectorInfo->usDiseaseDoctoringDelay)
				{
					// the country doesn't have many doctors...
					UINT32 doctors = GetSectorPopulation( sX, sY, FALSE ) * GetCivPopulationDoctorRate();

					// if this the hospital sector, the amount of doctoring is increased to marvellous levels
					if ( sX == gModSettings.ubHospitalSectorX && sY == gModSettings.ubHospitalSectorY )
					{
						doctors += 200;
					}

					// the bulk of doctoring will come from the military
					doctors += NumEnemiesInSector( sX, sY ) * GetMilitaryPopulationDoctorRate( );

					UINT32 doctorpower = doctors * GetPopulationDoctorPoints( );

					// if the disease escalates, increase doctor power!
					if ( infectedpercentage > GetSectorDiseaseOverFlowThreshold( ) + 0.1f )
						doctorpower += doctors * 0.3f * GetPopulationDoctorPoints();
					
					HealSectorPopulation( sX, sY, doctorpower );
				}
			}
			else
			{
				// if nobody is there, be sure to set infection data to 0
				pSectorInfo->usInfected = 0;
				pSectorInfo->usInfectionFlag = 0;
				pSectorInfo->fInfectionSeverity = 0;
				pSectorInfo->usDiseaseDoctoringDelay = 0;
			}

			// in any case, one hour has passed, update possible doctor replacements
			pSectorInfo->usDiseaseDoctoringDelay = max( 0, pSectorInfo->usDiseaseDoctoringDelay - 1);
		}
	}
}

// chance gets modified by aModifier (contextual modifier)
void HandlePossibleInfection( SOLDIERTYPE *pSoldier, SOLDIERTYPE* pOtherSoldier, UINT8 aInfectionType, FLOAT aModifier, BOOLEAN fStrategicOnly )
{
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
		UINT32 usChance = Disease[i].usInfectionChance[aInfectionType];

		// alter chance by modifier and disease resistance
		usChance = usChance * aModifier * ( 100 - pSoldier->GetDiseaseResistance( ) ) / 100;

		if ( aInfectionType == INFECTION_TYPE_TROPICS || aInfectionType == INFECTION_TYPE_SWAMP )
		{
			// if we are afraid of insects, we will be more careful around them - lower chance to be infected
			if ( gMercProfiles[pSoldier->ubProfile].bDisability == FEAR_OF_INSECTS )
				usChance *= 0.7f;
		}
		else if ( aInfectionType == INFECTION_TYPE_CONTACT_HUMAN )
		{
			// if we check a specific soldier, he must have the disease himself
			if ( pOtherSoldier && pOtherSoldier->sDiseasePoints[i] <= 0 )
					usChance = 0;

			// if we wear face or hand protection, lower chance of infection
			usChance *= (1.0f - pSoldier->GetDiseaseContactProtection( ));
		}
		else if ( aInfectionType == INFECTION_TYPE_CONTACT_CORPSE )
		{
			// if we wear face or hand protection, lower chance of infection
			usChance *= (1.0f - pSoldier->GetDiseaseContactProtection( ));
		}

		if ( Chance( usChance ) )
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
		UINT16 sNumberOfEnemies = NumEnemiesInSector( sX, sY );
		population += sNumberOfEnemies;
	}

	return population;
}

// colour for a sector on the map
INT32 GetMapColour( INT16 sX, INT16 sY, UINT8 mode )
{
	UINT8 sector = SECTOR( sX, sY );

	switch ( mode )
	{
		case MAPMODE_DISEASE:
		{
			UINT16 population = GetSectorPopulation( sX, sY );

			if ( population )
			{
				SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

				// display sector information only if we know about infection there
				if ( pSectorInfo && ((pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_PLAYER) || (gubFact[FACT_DISEASE_WHODATA_ACCESS] && pSectorInfo->usInfectionFlag & SECTORDISEASE_DIAGNOSED_WHO)) )
				{
					FLOAT infectedpercentage = (FLOAT)pSectorInfo->usInfected / (FLOAT)(population);

					if ( infectedpercentage < 0.2f )
						return MAP_SHADE_DK_GREEN;
					else if ( infectedpercentage < 0.3f )
						return MAP_SHADE_MD_GREEN;
					else if ( infectedpercentage < 0.4f )
						return MAP_SHADE_LT_GREEN;
					else if ( infectedpercentage < 0.5f )
						return MAP_SHADE_DK_YELLOW;
					else if ( infectedpercentage < 0.6f )
						return MAP_SHADE_MD_YELLOW;
					else if ( infectedpercentage < 0.7f )
						return MAP_SHADE_LT_YELLOW;
					else if ( infectedpercentage < 0.8f )
						return MAP_SHADE_DK_RED;
					else if ( infectedpercentage < 0.9f )
						return MAP_SHADE_MD_RED;
					else
						return MAP_SHADE_LT_RED;
				}
			}
		}
		break;
	}

	return MAP_SHADE_DK_GREY;
}

// handle infection redistribution if people move from A to B (set sXB  and sYB to negative values to simply remove infected people in A)
void PopulationMove( INT16 sXA, INT16 sYA, INT16 sXB, INT16 sYB, UINT16 usAmount )
{
	// we have to handle the impact on a sector's disease if people other than mercs move from A to B
	if ( !gGameExternalOptions.fDiseaseStrategic )
		return;

	UINT8 sectorA = SECTOR( sXA, sYA );

	SECTORINFO *pSectorInfoA = &(SectorInfo[sectorA]);

	UINT16 populationA = GetSectorPopulation( sXA, sYA );

	if ( populationA )
	{
		// how many people are already infected?
		if ( pSectorInfoA )
		{
			FLOAT infectionrateA = (FLOAT)(pSectorInfoA->usInfected) / (FLOAT)(populationA);

			UINT16 movinginfected = usAmount * infectionrateA;

			// we have to add the infection of movinginfected new infected with an infection level of pSectorInfoA->usInfectionSeverity
			if ( movinginfected )
			{
				// it is possible that there is no other sector
				if ( sXB > -1 && sYB > -1 )
				{
					UINT8 sectorB = SECTOR( sXB, sYB );

					SECTORINFO *pSectorInfoB = &(SectorInfo[sectorB]);

					if ( pSectorInfoB )
					{
						// new infection severity is the mean of old and new infected times their infection ratios
						pSectorInfoB->fInfectionSeverity = (pSectorInfoB->fInfectionSeverity * pSectorInfoB->usInfected + pSectorInfoA->fInfectionSeverity * movinginfected) / (pSectorInfoB->usInfected + movinginfected);
						pSectorInfoB->usInfected += movinginfected;
					
						// disease moves before population moves, so we have to add those too
						UINT16 populationB = GetSectorPopulation( sXB, sYB ) + movinginfected;

						FLOAT infectedpercentage = 0;
						if ( populationB )
							infectedpercentage = (FLOAT)pSectorInfoB->usInfected / (FLOAT)(populationB);

						if ( infectedpercentage >= GetSectorDiseaseOverFlowThreshold() || pSectorInfoB->fInfectionSeverity > ((FLOAT)Disease[0].sInfectionPtsOutbreak / (FLOAT)Disease[0].sInfectionPtsFull) )
							pSectorInfoB->usInfectionFlag |= (SECTORDISEASE_OUTBREAK | SECTORDISEASE_DIAGNOSED_WHO);
					}
				}

				// reduce the amount of infected in sector A
				pSectorInfoA->usInfected -= movinginfected;

				// if no infected remain, clean up data
				if ( !pSectorInfoA->usInfected )
				{
					pSectorInfoA->fInfectionSeverity = 0;
					pSectorInfoA->usInfectionFlag &= ~(SECTORDISEASE_OUTBREAK | SECTORDISEASE_DIAGNOSED_WHO | SECTORDISEASE_DIAGNOSED_PLAYER);
				}
			}
		}
	}
}

void HandleDeathDiseaseImplications( SOLDIERTYPE *pSoldier )
{
	if ( !gGameExternalOptions.fDisease || !gGameExternalOptions.fDiseaseStrategic || !pSoldier )
		return;

	UINT8 sector = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );

	SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

	if ( pSectorInfo )
	{
		// if the deceased was infected and not a merc, reduce sector number of infected
		if ( pSoldier->sDiseasePoints[0] && pSoldier->bTeam != OUR_TEAM )
		{
			pSectorInfo->usInfected = max( 0, pSectorInfo->usInfected - 1 );
		}

		// if this guy was a medic, then medical personnel was killed - it will take a while before he will be replaced
		if ( HAS_SKILL_TRAIT( pSoldier, DOCTOR_NT ) )
		{
			pSectorInfo->usDiseaseDoctoringDelay = min( 255, pSectorInfo->usDiseaseDoctoringDelay + 12 );
		}
	}
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
	
	// the amount of points needed to cure a single patient
	UINT32 ptsneededforcure = pSectorInfo->fInfectionSeverity * Disease[0].sInfectionPtsFull;

	// if possible, cure people (we prioritise on this, as we want to lower the number os possible infection sources)
	while ( pSectorInfo->usInfected && uHealpts >= ptsneededforcure )
	{
		uHealpts -= ptsneededforcure;
		--pSectorInfo->usInfected;
	}

	// if there are still points let, but we cannot cure someone, at least lower their disease
	if ( pSectorInfo->usInfected && uHealpts > 0 && Disease[0].sInfectionPtsFull )
	{
		UINT32 totaldiseasepoints = pSectorInfo->usInfected * ptsneededforcure;
		totaldiseasepoints = max( 0, totaldiseasepoints - uHealpts );
		
		pSectorInfo->fInfectionSeverity = (FLOAT)totaldiseasepoints / (FLOAT)(pSectorInfo->usInfected * Disease[0].sInfectionPtsFull);
	}

	// if there are no more infected, or infection is at 0, remove all traces of disease
	if ( !pSectorInfo->usInfected || !pSectorInfo->fInfectionSeverity )
	{
		pSectorInfo->usInfected = 0;
		pSectorInfo->fInfectionSeverity = 0;
		pSectorInfo->usInfectionFlag &= ~(SECTORDISEASE_OUTBREAK | SECTORDISEASE_DIAGNOSED_WHO | SECTORDISEASE_DIAGNOSED_PLAYER);
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
	UINT8 sectors = 0;

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
				SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

				if ( !pSectorInfo )
					continue;

				UINT16 population = GetSectorPopulation( sX, sY, FALSE );

				// only civilian population is relevant here
				if ( population )
				{
					// workforce effectivity in a sector is population minus cumulated effectivity loss of all infected, divided by entire population
					val += (population - min( pSectorInfo->usInfected, population ) * pSectorInfo->fInfectionSeverity ) / population;
				}
				// if no population is here, then there is no part of te population that is not working, so retrun 1.0 here :-)
				// this is basically a fix for players using this feature who have faulty xmls with population 0
				else
				{
					val += 1.0f;
				}

				++sectors;
			}
		}
	}

	if ( sectors )
		val /= sectors;

	return val;
}
