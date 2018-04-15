/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#include "CampaignStats.h"
#include "message.h"
#include "SaveLoadGame.h"
#include "GameVersion.h"
#include "Campaign Types.h"
#include "Game Clock.h"
#include "Soldier macros.h"
#include "Text.h"
#include "Laptop.h"
#include "LaptopSave.h"
#include "email.h"
#include "DynamicDialogue.h"

Campaign_Stats	gCampaignStats;
Incident_Stats	gCurrentIncident;		// we might save during an incident, thus we have to store the ongoing incident

CAMPAIGNSTATSEVENT zCampaignStatsEvent[NUM_CAMPAIGNSTATSEVENTS];

void
Incident_Stats::clear()
{
	usID				= 0;
	usTime				= 0;
	usSector			= 0;
	usLevel				= 0;
	usInterestRating	= 0;
	usNPCDied			= 0;
	usCivFactionFought	= 0;
	usIncidentFlags		= 0;
	usOneTimeEventFlags = 0;
		
	for (UINT8 i = 0; i < CAMPAIGNHISTORY_SD_MAX; ++i)
	{
		usKills[i]			= 0;
		usWounds[i]			= 0;
		usPrisoners[i]		= 0;
		usShots[i]			= 0;
		usParticipants[i]	= 0;
		usPromotions[i]		= 0;
	}
}

// determine how 'interesting' an incident is - the more happens, the more interesting it is
void
Incident_Stats::CalcInterestRating()
{
	usInterestRating = 0;

	for (UINT8 i = 0; i < CAMPAIGNHISTORY_SD_MAX; ++i)
	{
		usInterestRating +=	 100 * usKills[i];
		usInterestRating +=	  50 * usWounds[i];
		usInterestRating +=	  75 * usPrisoners[i];
		usInterestRating +=	       usShots[i] / 30;
		usInterestRating +=	  20 * usParticipants[i];
		usInterestRating +=	   5 * usPromotions[i];
	}

	if ( usNPCDied )
		usInterestRating += 500;

	if ( usCivFactionFought )
		usInterestRating += 1000;

	// evaluate flags
	if ( usIncidentFlags & INCIDENT_ARTILLERY_ENEMY )				usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_ARTILLERY_PLAYERSIDE )			usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_TANKS_ENEMY )					usInterestRating += 1000;
	if ( usIncidentFlags & INCIDENT_TANKS_PLAYERSIDE )				usInterestRating += 2000;
	if ( usIncidentFlags & INCIDENT_MUSTARDGAS_ENEMY )				usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_MUSTARDGAS_PLAYERSIDE )			usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_ATTACK_ENEMY )					usInterestRating +=  200;
	if ( usIncidentFlags & INCIDENT_ATTACK_PLAYERSIDE )				usInterestRating +=  100;
	if ( usIncidentFlags & INCIDENT_REINFORCEMENTS_ENEMY )			usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_REINFORCEMENTS_PLAYERSIDE )		usInterestRating +=  500;
	if ( usIncidentFlags & INCIDENT_SNIPERS_ENEMY )					usInterestRating +=  200;
	if ( usIncidentFlags & INCIDENT_SNIPERS_PLAYERSIDE )			usInterestRating +=  100;
	if ( usIncidentFlags & INCIDENT_AMBUSH )						usInterestRating +=  100;
	if ( usIncidentFlags & INCIDENT_AIRDROP )						usInterestRating +=  100;
	if ( usIncidentFlags & INCIDENT_SURGERY )						usInterestRating +=   50;
	if ( usIncidentFlags & INCIDENT_BUILDINGS_DAMAGED )				usInterestRating +=  100;
	if ( usIncidentFlags & INCIDENT_WIN )							usInterestRating +=  100;
	else															usInterestRating +=  200;
	if ( usIncidentFlags & INCIDENT_SAMSITE_SABOTAGED )				usInterestRating += 1500;
	
	if ( usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_OMERTA )			usInterestRating += 1000;
	if ( usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_DEATH_KINGPIN )	usInterestRating += 500;
	if ( usOneTimeEventFlags & (INCIDENT_ONETIMEEVENT_MASSACRE_HICKS|INCIDENT_ONETIMEEVENT_MASSACRE_BLOODCATS) )	usInterestRating += 800;
	if ( usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_CITY_RETAKEN )		usInterestRating += 2000;
	if ( usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_CITY_LIBERATED )	usInterestRating += 1000;
}

BOOLEAN	
Incident_Stats::Save( HWFILE hFile )
{
	UINT32 uiNumBytesWritten = 0;

	if ( !FileWrite( hFile, this, SIZEOF_INCIDENT_STATS_POD, &uiNumBytesWritten ) )
	{
		return(FALSE);
	}

	return( TRUE );
}

BOOLEAN	
Incident_Stats::Load( HWFILE hwFile )
{
	if(guiCurrentSaveGameVersion >= CAMPAIGNSTATS)
	{
		UINT32 numBytesRead = 0;
		
		numBytesRead = ReadFieldByField(hwFile, &usID,					sizeof(usID),				sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usTime,				sizeof(usTime),				sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usSector,				sizeof(usSector),			sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usLevel,				sizeof(usLevel),			sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usInterestRating,		sizeof(usInterestRating),	sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usNPCDied,				sizeof(usNPCDied),			sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usCivFactionFought,	sizeof(usCivFactionFought), sizeof(UINT8), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usIncidentFlags,		sizeof(usIncidentFlags),	sizeof(UINT64), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usOneTimeEventFlags,	sizeof(usOneTimeEventFlags),sizeof(UINT64), numBytesRead);
		
		numBytesRead = ReadFieldByField(hwFile, &usKills,				sizeof(usKills),			sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usWounds,				sizeof(usWounds),			sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usPrisoners,			sizeof(usPrisoners),		sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usShots,				sizeof(usShots),			sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usParticipants,		sizeof(usParticipants),		sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usPromotions,			sizeof(usPromotions),		sizeof(UINT16), numBytesRead);

		numBytesRead = ReadFieldByField(hwFile, &usFiller,				sizeof(usFiller),			sizeof(UINT8), numBytesRead);
		
		if( numBytesRead != SIZEOF_INCIDENT_STATS_POD )
			return(FALSE);
	}

	return( TRUE );
}

void
Incident_Stats::AddStat( SOLDIERTYPE* pSoldier, UINT8 aType )
{
	UINT8 group = CAMPAIGNHISTORY_SD_MERC;

	switch ( pSoldier->bTeam )
	{
	case OUR_TEAM:
		group = CAMPAIGNHISTORY_SD_MERC;
		break;

	case ENEMY_TEAM:
		{
			if ( ARMED_VEHICLE( pSoldier ) )
				group = CAMPAIGNHISTORY_SD_ENEMY_TANK;
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR )
				group = CAMPAIGNHISTORY_SD_ENEMY_ADMIN;
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE )
				group = CAMPAIGNHISTORY_SD_ENEMY_ELITE;
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY )
				group = CAMPAIGNHISTORY_SD_ENEMY_ARMY;
		}
		break;

	case CREATURE_TEAM:
		if ( pSoldier->IsZombie() )
			group = CAMPAIGNHISTORY_SD_ZOMBIE;
		else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_BANDIT )
			group = CAMPAIGNHISTORY_SD_ENEMY_ADMIN;
		else
			group = CAMPAIGNHISTORY_SD_CREATURE;
		break;

	case MILITIA_TEAM:
		{
			if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA )
				group = CAMPAIGNHISTORY_SD_MILITIA_GREEN;
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
				group = CAMPAIGNHISTORY_SD_MILITIA_REGULAR;
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA )
				group = CAMPAIGNHISTORY_SD_MILITIA_ELITE;
		}
		break;

	case CIV_TEAM:
	default:
		group = CAMPAIGNHISTORY_SD_CIV;
		break;
	}

	switch ( aType )
	{
	case CAMPAIGNHISTORY_TYPE_KILL:
		{
			usKills[group]++;

			if ( pSoldier->ubProfile == KINGPIN )
				usOneTimeEventFlags |= INCIDENT_ONETIMEEVENT_DEATH_KINGPIN;
			else if ( pSoldier->ubProfile == DARREL )
				usOneTimeEventFlags |= INCIDENT_ONETIMEEVENT_MASSACRE_HICKS;
		}
		break;

	case CAMPAIGNHISTORY_TYPE_WOUND:
		usWounds[group]++;
		break;

	case CAMPAIGNHISTORY_TYPE_PRISONER:
		usPrisoners[group]++;
		break;

	case CAMPAIGNHISTORY_TYPE_SHOT:
		usShots[group]++;
		break;

	case CAMPAIGNHISTORY_TYPE_PARTICIPANT:
		usParticipants[group]++;
		break;

	case CAMPAIGNHISTORY_TYPE_PROMOTION:
	default:
		usPromotions[group]++;
		break;
	}

	if ( ARMED_VEHICLE( pSoldier ) )
	{
		if ( pSoldier->bSide == 0 )
			usIncidentFlags |= INCIDENT_TANKS_PLAYERSIDE;
		else
			usIncidentFlags |= INCIDENT_TANKS_ENEMY;
	}
}

UINT8 tmpnr = 0;
static CHAR16	tmpdirstring[2][ 50 ];	// we need 2 arrays, in case we need 2 name pointers in one string
STR16
Incident_Stats::GetAttackerDirString( BOOLEAN fAttacker )
{
	CHAR16	helperstr[ 50 ];
	swprintf(helperstr, L"" );

	++tmpnr;
	if ( tmpnr > 1 )
		tmpnr = 0;

	tmpdirstring[tmpnr][0] =  '\0' ;

	UINT8 dirs = 0;
	CHAR16	dir1[ 10 ], dir2[ 10 ], dir3[ 10 ], dir4[ 10 ];
	swprintf(dir1, L"" );
	swprintf(dir2, L"" );
	swprintf(dir3, L"" );
	swprintf(dir4, L"" );

	if ( (fAttacker && usIncidentFlags & INCIDENT_ATTACK_ENEMY) || (!fAttacker && usIncidentFlags & INCIDENT_ATTACK_PLAYERSIDE) )
	{
		if ( usIncidentFlags & INCIDENT_ATTACKDIR_NORTH_ENEMY )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_NORTH] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_WEST_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_WEST] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH_ENEMY )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
					++dirs;

					if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
					{
						swprintf(dir4, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
						++dirs;
					}
				}
				else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_WEST_ENEMY )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_WEST] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH_ENEMY )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST_ENEMY )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
			++dirs;
		}
	}
	else
	{
		if ( usIncidentFlags & INCIDENT_ATTACKDIR_NORTH )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_NORTH] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_WEST )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_WEST] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
					++dirs;

					if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
					{
						swprintf(dir4, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
						++dirs;
					}
				}
				else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_WEST )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_WEST] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
				++dirs;

				if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
				{
					swprintf(dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
					++dirs;
				}
			}
			else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_SOUTH )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH] );
			++dirs;

			if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
			{
				swprintf(dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
				++dirs;
			}
		}
		else if ( usIncidentFlags & INCIDENT_ATTACKDIR_EAST )
		{
			swprintf(dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_EAST] );
			++dirs;
		}
	}

	switch ( dirs )
	{
	case 4:
		swprintf(helperstr, L"%s, %s, %s %s %s", dir1, dir2, dir3, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_AND], dir4 );
		break;

	case 3:
		swprintf(helperstr, L"%s, %s %s %s", dir1, dir2, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_AND], dir3 );
		break;

	case 2:
		swprintf(helperstr, L"%s %s %s", dir1, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_AND], dir2 );
		break;

	case 1:
		swprintf(helperstr, L"%s", dir1 );
		break;

	case 0:
		swprintf(helperstr, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_UNKNOWNLOCATION] );
		break;

	default:
		break;
	}

	wcscat( tmpdirstring[tmpnr], helperstr );

	return tmpdirstring[tmpnr];
}

void
Incident_Stats::GetTerrainandType(UINT8& arTerrain, UINT8& arType)
{
	if ( usLevel > 0 )
		arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_UNDERGROUND;
	else
	{
		SECTORINFO *pSector = &SectorInfo[ usSector ];

		switch( pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] )
		{
			case COASTAL:
			case COASTAL_ROAD:
			case WATER:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_COASTAL;
				break;

			case SAND:
			case SAND_ROAD:
			case SAND_SAM_SITE:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_DESERT;
				break;

			case DENSE:
			case DENSE_ROAD:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_FOREST;
				break;

			case GROUNDBARRIER:
			case EDGEOFWORLD:
			case HILLS:
			case HILLS_ROAD:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MOUNTAIN;
				break;

			case NS_RIVER:
			case EW_RIVER:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_RIVER;
				break;

			case SWAMP:
			case SWAMP_ROAD:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_SWAMP;
				break;

			case TROPICS:
			case TROPICS_ROAD:
			case TROPICS_SAM_SITE:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_TROPICAL;
				break;

			case TOWN:
			case MEDUNA_SAM_SITE:
			case CAMBRIA_HOSPITAL_SITE:
			case DRASSEN_AIRPORT_SITE:
			case MEDUNA_AIRPORT_SITE:
			case SAM_SITE:
			case REBEL_HIDEOUT:
			case TIXA_DUNGEON:
			case CREATURE_LAIR:
			case ORTA_BASEMENT:
			case TUNNEL:
			case SHELTER:
			case ABANDONED_MINE:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_URBAN;
				break;

			case ROAD:
			case PLAINS:			
			case SPARSE:													
			case FARMLAND:
			case PLAINS_ROAD:
			case SPARSE_ROAD:
			case FARMLAND_ROAD:			
			case SPARSE_SAM_SITE:		
			default:
				arTerrain = CAMPAINGHISTORY_PICLIBRARY_TERRAIN_SPARSE;
				break;
		}
	}

	if ( usIncidentFlags & INCIDENT_AIRDROP )
		arType = CAMPAINGHISTORY_PICLIBRARY_TYPE_AIRDROP;
	else if ( usIncidentFlags & (INCIDENT_TANKS_ENEMY|INCIDENT_TANKS_PLAYERSIDE) )
		arType = CAMPAINGHISTORY_PICLIBRARY_TYPE_TANKS;
	else if ( usIncidentFlags & INCIDENT_WIN )
		arType = CAMPAINGHISTORY_PICLIBRARY_TYPE_WIN;	
	else
		arType = CAMPAINGHISTORY_PICLIBRARY_TYPE_LOSS;
}

Campaign_Stats::Campaign_Stats()
{
	//*mspSelf = *this;
}

Campaign_Stats::~Campaign_Stats()
{
	//if ( mspSelf )
		//delete mspSelf;
}

/*Campaign_Stats*
Campaign_Stats::GetObject()
{
	if ( !mspSelf )
		mspSelf = new Campaign_Stats;

	return mspSelf;
}*/

void
Campaign_Stats::clear()
{
	memset(this, 0, SIZEOF_CAMPAIGN_STATS_POD);

	mIncidentVector.clear();
}

BOOLEAN
Campaign_Stats::Save( HWFILE hFile )
{
	if(guiCurrentSaveGameVersion >= CAMPAIGNSTATS)
	{
		UINT32 uiNumBytesWritten = 0;

		if ( !FileWrite( hFile, this, SIZEOF_CAMPAIGN_STATS_POD, &uiNumBytesWritten ) )
			return(FALSE);

		std::vector<Incident_Stats>::iterator itend = mIncidentVector.end();
		for (std::vector<Incident_Stats>::iterator it = mIncidentVector.begin(); it != itend; ++it)
		{
			if ( !(*it).Save ( hFile ) )
				return(FALSE);
		}
	}

	return( TRUE );
}

BOOLEAN
Campaign_Stats::Load( HWFILE hwFile )
{
	if(guiCurrentSaveGameVersion >= CAMPAIGNSTATS)
	{
		clear();

		UINT32 numBytesRead = 0;

		numBytesRead = ReadFieldByField(hwFile, &usKills,			sizeof(usKills),			sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usWounds,			sizeof(usWounds),			sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usPrisoners,		sizeof(usPrisoners),		sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usShots,			sizeof(usShots),			sizeof(UINT32), numBytesRead);
		
		numBytesRead = ReadFieldByField(hwFile, &sMoneyEarned,		sizeof(sMoneyEarned),		sizeof(INT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usConsumed,		sizeof(usConsumed),			sizeof(FLOAT), numBytesRead);
				
		numBytesRead = ReadFieldByField(hwFile, &usNumIncidents,	sizeof(usNumIncidents),		sizeof(UINT32), numBytesRead);
		numBytesRead = ReadFieldByField(hwFile, &usHighestID,		sizeof(usHighestID),		sizeof(UINT32), numBytesRead);

		numBytesRead = ReadFieldByField(hwFile, &usFiller,			sizeof(usFiller),			sizeof(UINT8), numBytesRead);

		if( numBytesRead != SIZEOF_CAMPAIGN_STATS_POD )
			return(FALSE);

		for (UINT32 i = 0; i < usNumIncidents; ++i)
		{
			Incident_Stats incident;
			if ( !incident.Load ( hwFile ) )
				return(FALSE);
			
			// if option is active, only read the last reports and forget the rest
			if ( gGameExternalOptions.usReportsToLoad < 0 || i >= usNumIncidents - gGameExternalOptions.usReportsToLoad )
				mIncidentVector.push_back(incident);
		}

		// re-evaluate the vector size - we may have been ordered to forget some
		usNumIncidents = mIncidentVector.size();
	}
	
	return( TRUE );
}

// Warning! As this also updates the overall kill counters, don't use this function when loading incidents
void
Campaign_Stats::AddNewIncident(Incident_Stats arIncident)
{
	// make sure interest rating is up to date
	arIncident.CalcInterestRating();

	mIncidentVector.push_back(arIncident);
	++usNumIncidents;

	// update kills/wounds/prisoner numbers
	for (UINT16 i = 0; i < CAMPAIGNHISTORY_SD_MAX; ++i)
	{
		usKills[i]		+= arIncident.usKills[i];
		usWounds[i]		+= arIncident.usWounds[i];
		usPrisoners[i]	+= arIncident.usPrisoners[i];
		usShots[i]	    += arIncident.usShots[i];
	}
}

void
Campaign_Stats::AddMoneyEarned(UINT8 aType, INT32 aVal)
{
	if ( aType >= CAMPAIGN_MONEY_MAX )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Error: Unknown money type!" );
		return;
	}

	sMoneyEarned[aType] += aVal;
}

void
Campaign_Stats::AddConsumption(UINT8 aType, FLOAT aVal)
{
	if ( aType >= CAMPAIGN_CONSUMED_MAX )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Error: Unknown consumption type!" );
		return;
	}

	usConsumed[aType] += aVal;
}

// add this incident to the campaign stats and then clear it
void FinishIncident(INT16 sX, INT16 sY, INT8 sZ)
{
	// it is possible to enter a combat without any real fighting. 
	// For now, lets just assume this happens with undetected spies. If nothing of interest happened, don't add this incident (it would be boring to read anyway)
	if ( gCurrentIncident.usIncidentFlags & (INCIDENT_SPYACTION_ENEMY|INCIDENT_SPYACTION_PLAYERSIDE) && !(gCurrentIncident.usIncidentFlags & INCIDENT_SPYACTION_UNCOVERED) )
	{
		// if noting of interest happened...
		if ( !(gCurrentIncident.usIncidentFlags & INCIDENT_EVENT) && gCurrentIncident.usOneTimeEventFlags == 0 )
		{
			UINT16 i = 0;

			// if nobody was harmed...
			for (i = 0; i < CAMPAIGNHISTORY_SD_MAX; ++i)
			{
				if ( gCurrentIncident.usKills[i] || gCurrentIncident.usWounds[i] || gCurrentIncident.usPrisoners[i] )
					break;
			}

			if ( i == CAMPAIGNHISTORY_SD_MAX )
			{
				// totally boring. Don't add this, just clean it and exit
				gCurrentIncident.clear();

				return;
			}
		}
	}
	
	// due to odd coding, we do not know when an incident starts
	// (checking for entering combat isn't enough, as we do that multiple times per battle)
	// we thus set the relevant data when finishing an incident
	gCampaignStats.usHighestID++;
	
	gCurrentIncident.usID = gCampaignStats.usHighestID;
	gCurrentIncident.usTime = GetWorldTotalSeconds();
	gCurrentIncident.usSector = SECTOR(sX, sY);
	gCurrentIncident.usLevel = sZ;

	// add flags depending on quests etc.
	// if this is incident 1, we just landed - special text then
	if ( gCampaignStats.usHighestID == 1 )
		gCurrentIncident.usOneTimeEventFlags |= INCIDENT_ONETIMEEVENT_OMERTA;

	// dynamic opinions
	HandleDynamicOpinionBattleFinished( (gCurrentIncident.usIncidentFlags & INCIDENT_WIN) != 0 );

	gCampaignStats.AddNewIncident(gCurrentIncident);
	
	// reset incident after adding it
	gCurrentIncident.clear();
}

// start a new incident
void StartIncident(INT16 sX, INT16 sY, INT8 sZ)
{
	gCampaignStats.usHighestID++;

	gCurrentIncident.usID = gCampaignStats.usHighestID;
	gCurrentIncident.usTime = GetWorldTotalSeconds();
	gCurrentIncident.usSector = SECTOR(sX, sY);
	gCurrentIncident.usLevel = sZ;
}

INT32 GetPositionOfIncident( UINT32 aIncidentId )
{
	INT32 cnt = 0;

	std::vector<Incident_Stats>::iterator itend = gCampaignStats.mIncidentVector.end( );
	for ( std::vector<Incident_Stats>::iterator it = gCampaignStats.mIncidentVector.begin( ); it != itend; ++it )
	{
		if ( (*it).usID == aIncidentId )
			return cnt;

		++cnt;
	}

	return -1;
}

static CHAR16	gIncidentNameText[100];
STR16	GetIncidentName( UINT32 aIncidentId )
{
	swprintf( gIncidentNameText, L"NAME NOT FOUND" );

	std::vector<Incident_Stats>::iterator itend = gCampaignStats.mIncidentVector.end( );
	for ( std::vector<Incident_Stats>::iterator it = gCampaignStats.mIncidentVector.begin( ); it != itend; ++it )
	{
		if ( (*it).usID == aIncidentId )
		{
			Incident_Stats incident = (*it);

			UINT32 prefix = (incident.usTime + incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_ARMY] + incident.usID) % CAMPAIGNSTATS_OPERATION_NUM_PREFIX;
			UINT32 suffix = (incident.usTime + incident.usShots[CAMPAIGNHISTORY_SD_MERC] + 7 * incident.usID) % CAMPAIGNSTATS_OPERATION_NUM_SUFFIX;

			swprintf( gIncidentNameText, szCampaignStatsOperationPrefix[prefix], szCampaignStatsOperationSuffix[suffix] );

			break;
		}
	}

	return gIncidentNameText;
}

UINT32 GetIdOfCurrentlyOngoingIncident()
{
	return gCampaignStats.usHighestID + 1;
}
