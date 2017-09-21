/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "MilitiaIndividual.h"

#include "random.h"
#include "text.h"
#include "Overhead Types.h"
#include "Game Clock.h"
#include "strategicmap.h"
#include "GameVersion.h"
#include "SaveLoadGame.h"
#include "CampaignStats.h"
#include "Town Militia.h"
#include "message.h"
#include "Auto Resolve.h"		// added for IndividualMilitiaInUse_AutoResolve(...)

MilitiaOriginData gMilitiaOriginData[MO_MAX];

// later on, make this an array with a vector for each sector
std::vector<MILITIA> gIndividualMilitiaVector;

MILITIA::MILITIA()
{
	memset( this, 0, sizeof(MILITIA) );
}

BOOLEAN
MILITIA::Save( HWFILE hFile )
{
	UINT32 uiNumBytesWritten = 0;
	
	if ( !FileWrite( hFile, this, SIZEOF_MILITIA_POD, &uiNumBytesWritten ) )
		return(FALSE);

	// in order to save a vector, we first save its size and then its content
	UINT32 size = history.size( );
	if ( !FileWrite( hFile, &size, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	std::vector<MILITIA_BATTLEREPORT>::iterator itend = history.end( );
	for ( std::vector<MILITIA_BATTLEREPORT>::iterator it = history.begin( ); it != itend; ++it )
	{
		if ( !FileWrite( hFile, &(*it), sizeof(MILITIA_BATTLEREPORT), &uiNumBytesWritten ) )
			return(FALSE);
	}

	return(TRUE);
}

BOOLEAN
MILITIA::Load( HWFILE hwFile )
{
	if ( guiCurrentSaveGameVersion >= INDIVIDUAL_MILITIA )
	{
		UINT32 numBytesRead = 0;
				
		numBytesRead = ReadFieldByField( hwFile, &id, sizeof(id), sizeof(UINT32), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &flagmask, sizeof(flagmask), sizeof(UINT32), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &origin, sizeof(origin), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &originsector, sizeof(originsector), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &sector, sizeof(sector), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &age, sizeof(age), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &militiarank, sizeof(militiarank), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &bodytype, sizeof(bodytype), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &skin, sizeof(skin), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &hair, sizeof(hair), sizeof(UINT8), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &forename, sizeof(forename), sizeof(UINT16), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &surname, sizeof(surname), sizeof(UINT16), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &callsign, sizeof(callsign), sizeof(UINT16), numBytesRead );	
		numBytesRead = ReadFieldByField( hwFile, &healthratio, sizeof(healthratio), sizeof(FLOAT), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &kills, sizeof(kills), sizeof(UINT16), numBytesRead);
		numBytesRead = ReadFieldByField( hwFile, &assists, sizeof(assists), sizeof(UINT16), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &promotionpoints, sizeof(promotionpoints), sizeof(UINT16), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &filler1, sizeof(filler1), sizeof(UINT16), numBytesRead );
		
		if ( numBytesRead != SIZEOF_MILITIA_POD )
			return(FALSE);

		UINT32 size = 0;
		numBytesRead = ReadFieldByField( hwFile, &size, sizeof(size), sizeof(UINT32), numBytesRead );

		for ( UINT32 i = 0; i < size; ++i )
		{
			MILITIA_BATTLEREPORT data;
			if ( !FileRead( hwFile, &data, sizeof(MILITIA_BATTLEREPORT), &numBytesRead ) )
				return(FALSE);
			
			history.push_back( data );
		}
	}

	return(TRUE);
}


static CHAR16	gMilitiaNameText[100];
STR16	MILITIA::GetName()
{
	swprintf( gMilitiaNameText, L"NAME NOT FOUND" );

	if ( bodytype == REGFEMALE )
	{
		if ( gMilitiaOriginData[origin].szFemale_Forename.size() > forename && gMilitiaOriginData[origin].szFemale_Surname.size() > surname )
		{
			swprintf( gMilitiaNameText, L"%s %s", gMilitiaOriginData[origin].szFemale_Forename[forename].c_str( ), gMilitiaOriginData[origin].szFemale_Surname[surname].c_str( ) );
		}
	}
	else
	{
		if ( gMilitiaOriginData[origin].szMale_Forename.size( ) > forename && gMilitiaOriginData[origin].szMale_Surname.size( ) > surname )
		{
			swprintf( gMilitiaNameText, L"%s %s", gMilitiaOriginData[origin].szMale_Forename[forename].c_str( ), gMilitiaOriginData[origin].szMale_Surname[surname].c_str( ) );
		}
	}
	
	return gMilitiaNameText;
}

static CHAR16	gMilitiaSectorText[100];
STR16	MILITIA::GetSector( )
{
	swprintf( gMilitiaSectorText, L"NAME NOT FOUND" );
	
	GetShortSectorString( SECTORX( sector ), SECTORY( sector ), gMilitiaSectorText );

	return gMilitiaSectorText;
}

STR16	MILITIA::GetOriginSector( )
{
	swprintf( gMilitiaSectorText, L"NAME NOT FOUND" );

	GetShortSectorString( SECTORX( originsector ), SECTORY( originsector ), gMilitiaSectorText );

	return gMilitiaSectorText;
}

void	MILITIA::AddKills( UINT16 aKills, UINT16 aAssists )
{
	kills += aKills;
	assists += aAssists;

	// important: increase by the NEW points (as militia that start with a higher rank have their points adjusted, promotion points are not always the sum of kills and assists)
	promotionpoints += 2 * aKills + aAssists;

	if ( militiarank == REGULAR_MILITIA )
	{
		promotionpoints = max( promotionpoints, gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Regular );
	}
	else if ( militiarank == ELITE_MILITIA )
	{
		promotionpoints = max( promotionpoints, gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Regular + gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Elite );
	}
}

UINT16	MILITIA::GetWage( )
{	
	return gMilitiaOriginData[origin].dailycost[militiarank];
}

// release militia from our service. Does not take care of actually removing a militia from the sector!
void	MILITIA::Fire( )
{
	flagmask |= MILITIAFLAG_FIRED;

	MILITIA_BATTLEREPORT report;
	report.id = GetWorldTotalMin( );
	report.flagmask |= MILITIA_BATTLEREPORT_FLAG_FIRED;

	history.push_back( report );
}

BOOLEAN SaveIndividualMilitiaData( HWFILE hwFile )
{
	UINT32 uiNumBytesWritten = 0;
		
	// in order to save a vector, we first save its size and then its content
	UINT32 size = gIndividualMilitiaVector.size( );
	if ( !FileWrite( hwFile, &size, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !(*it).Save( hwFile ) )
			return(FALSE);
	}

	return TRUE;
}

void InitIndividualMilitiaData()
{
	gIndividualMilitiaVector.clear();
}

BOOLEAN LoadIndividualMilitiaData( HWFILE hwFile )
{
	InitIndividualMilitiaData();

	UINT32 numBytesRead = 0;
	
	UINT32 size = 0;
	numBytesRead = ReadFieldByField( hwFile, &size, sizeof(size), sizeof(UINT32), numBytesRead );

	for ( UINT32 i = 0; i < size; ++i )
	{
		MILITIA data;
		if ( !data.Load( hwFile ) )
			return(FALSE);

		// we changed variable storing the militia quality from soldierclass to militia rank, so we have to change this here
		if ( guiCurrentSaveGameVersion < INDIVIDUAL_MILITIA_RANKFIX )
			data.militiarank = SoldierClassToMilitiaRank( data.militiarank );

		gIndividualMilitiaVector.push_back(data);
	}

	return TRUE;
}

UINT32 TotalNumberIndividualMilitia( )
{
	return gIndividualMilitiaVector.size( );
}

UINT32 GetFreeIndividualMilitiaId()
{
	return 1 + TotalNumberIndividualMilitia();
}

BOOLEAN IsLegalMilitiaId( UINT32 aId )
{
	return (aId > 0 && aId < GetFreeIndividualMilitiaId( ));
}

BOOLEAN GetMilitia( UINT32 aId, MILITIA* apMilitia )
{
	if ( gGameExternalOptions.fIndividualMilitia && IsLegalMilitiaId( aId ) )
	{
		if ( apMilitia )
			*apMilitia = gIndividualMilitiaVector[aId - 1];

		return TRUE;
	}

	return FALSE;
}

// update data
void UpdateMilitia( MILITIA aMilitia )
{
	if ( gGameExternalOptions.fIndividualMilitia && IsLegalMilitiaId( aMilitia.id ) )
	{
		gIndividualMilitiaVector[aMilitia.id - 1] = aMilitia;
	}
}

SOLDIERTYPE* GetUsedSoldierToIndividualMilitia( UINT32 aMilitiaId )
{
	for ( UINT32 cnt = gTacticalStatus.Team[MILITIA_TEAM].bFirstID; cnt < gTacticalStatus.Team[MILITIA_TEAM].bLastID; ++cnt )
	{
		if ( MercPtrs[cnt] && MercPtrs[cnt]->bActive && MercPtrs[cnt]->usIndividualMilitiaID == aMilitiaId )
		{
			return MercPtrs[cnt];
		}
	}

	return NULL;
}

// update the health values of all militia in tactical
void UpdateAllMilitiaHealthInTactical()
{
	if ( !gGameExternalOptions.fIndividualMilitia )
		return;

	SOLDIERTYPE* pSoldier;
	UINT32 cnt = gTacticalStatus.Team[MILITIA_TEAM].bFirstID;
	INT32 lastid = gTacticalStatus.Team[MILITIA_TEAM].bLastID;
	for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
	{
		MILITIA militia;
		if ( pSoldier && pSoldier->bActive && pSoldier->stats.bLifeMax && GetMilitia( pSoldier->usIndividualMilitiaID, &militia ) )
		{
			militia.healthratio = 100.0f * pSoldier->stats.bLife / pSoldier->stats.bLifeMax;

			// while we're here, update kills and assists too
			militia.AddKills( pSoldier->ubMilitiaKills, pSoldier->ubMilitiaAssists );

			pSoldier->ubMilitiaKills = 0;
			pSoldier->ubMilitiaAssists = 0;

			UpdateMilitia( militia );
		}
	}
}

void HandleHourlyMilitiaHealing( )
{
	if ( !gGameExternalOptions.fIndividualMilitia || !gGameExternalOptions.fIndividualMilitia_ManageHealth )
		return;

	INT32 cnt = gTacticalStatus.Team[MILITIA_TEAM].bFirstID;
	INT32 lastid = gTacticalStatus.Team[MILITIA_TEAM].bLastID;
	SOLDIERTYPE* pSoldier = NULL;
	
	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !((*it).flagmask & MILITIAFLAG_DEAD) )
		{
			(*it).healthratio = min( 100.0f, (*it).healthratio + gGameExternalOptions.dIndividualMilitiaHourlyHealthPercentageGain );

			// if this guy is in the currently loaded sector, heal the soldier instead (and update health ratio while you're there)
			if ( !gbWorldSectorZ && (*it).sector == SECTOR( gWorldSectorX, gWorldSectorY ) )
			{
				for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
				{
					if ( pSoldier && pSoldier->bActive && (*it).id == pSoldier->usIndividualMilitiaID && 
						 gWorldSectorX == pSoldier->sSectorX && gWorldSectorY == pSoldier->sSectorY && !pSoldier->bSectorZ &&
						 pSoldier->stats.bLifeMax > 0  )
					{
						INT8 oldlife = pSoldier->stats.bLife;

						FLOAT currenthealthratio = 100.0f * oldlife / pSoldier->stats.bLifeMax;

						currenthealthratio = min( 100.0f, currenthealthratio + gGameExternalOptions.dIndividualMilitiaHourlyHealthPercentageGain );
						
						pSoldier->stats.bLife = min( pSoldier->stats.bLifeMax, (currenthealthratio / 100.0f) * pSoldier->stats.bLifeMax );

						// healing done will be displayed the next time the player sees this soldier
						pSoldier->flags.fDisplayDamage = TRUE;
						pSoldier->sDamage -= pSoldier->stats.bLife - oldlife;

						// update new health ratio
						(*it).healthratio = currenthealthratio;

						break;
					}
				}
			}
		}
	}
}

UINT32 CreateRandomIndividualMilitia( UINT8 aMilitiaRank, UINT8 aOrigin, UINT8 aSector )
{
	UINT32 militiaid = CreateNewIndividualMilitia(aMilitiaRank, aOrigin, aSector );

	MILITIA newmilitia;
	if ( GetMilitia( militiaid, &newmilitia ) )
	{
		newmilitia.healthratio = (OKLIFE + Random( 85 ));
		
		newmilitia.AddKills( Random( 5 ), Random( 10 ) );

		// make up some history
		UINT16 mineventtime = Random( 70000 );

		int numhistory = Random( 30 );
		for ( int j = 0; j < numhistory; ++j )
		{
			MILITIA_BATTLEREPORT report;
			report.id = Random( 100 );
			switch ( Random( 20 ) )
			{
			case 0:
			case 1:
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_PROMOTED;
				mineventtime = max( mineventtime + 6, report.id );
				report.id = mineventtime;
				break;

			case 2:
			case 3: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_SMALL; break;
			case 4:
			case 5: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_HEAVY; break;
			case 6:
			case 7: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_COMA; break;
			case 8:
			case 9:
			case 10: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY; break;

				// once dead, stop adding more events and kill of the soldier
			case 11: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_DIED;
				j = numhistory;
				newmilitia.healthratio = 0.0f;
				newmilitia.flagmask |= MILITIAFLAG_DEAD;
				break;

			case 12: report.flagmask |= MILITIA_BATTLEREPORT_FLAG_FIRED;
				j = numhistory;
				newmilitia.flagmask |= MILITIAFLAG_FIRED;
				break;
			default: break;
			}

			newmilitia.history.push_back( report );
		}

		UpdateMilitia( newmilitia );
	}

	return militiaid;
}

UINT32 CreateNewIndividualMilitia( UINT8 aMilitiaRank, UINT8 aOrigin, UINT8 aSector )
{
	if ( !gGameExternalOptions.fIndividualMilitia )
		return 0;
	
	MILITIA newmilitia;

	newmilitia.id = GetFreeIndividualMilitiaId( );

	// make sure we use a valid soldier class
	newmilitia.militiarank = aMilitiaRank;
	if ( newmilitia.militiarank >= MAX_MILITIA_LEVELS )
		newmilitia.militiarank = GREEN_MILITIA;

	// according to origin, we choose bodytype, skin, hair, fore- and surname
	newmilitia.origin = aOrigin;

	// age is random
	newmilitia.age = 16 + Random( 2 ) * (1 + Random( 8 ) )
		+ Random( 2 ) * (1 + Random( 8 ))
		+ Random( 2 ) * (1 + Random( 8 ))
		+ Random( 2 ) * (1 + Random( 8 ))
		+ Random( 2 ) * (1 + Random( 8 ))
		+ Random( 2 ) * (1 + Random( 8 ));

	newmilitia.originsector = aSector;
	newmilitia.sector = aSector;

	UINT8 rnd = Random( 100 );

	newmilitia.bodytype = REGMALE;
	for ( int i = 0; i < ADULTFEMALEMONSTER; ++i )
	{
		if ( rnd < gMilitiaOriginData[newmilitia.origin].chance_bodytype[i] )
		{
			newmilitia.bodytype = i;
			break;
		}
		else
		{
			rnd -= gMilitiaOriginData[newmilitia.origin].chance_bodytype[i];
		}
	}

	rnd = Random( 100 );

	newmilitia.skin = PINKSKIN;
	for ( int i = 0; i < NUMSKINS; ++i )
	{
		if ( rnd < gMilitiaOriginData[newmilitia.origin].chance_skin[i] )
		{
			newmilitia.skin = i;
			break;
		}
		else
		{
			rnd -= gMilitiaOriginData[newmilitia.origin].chance_skin[i];
		}
	}

	newmilitia.hair = ChooseHairColor( newmilitia.bodytype, newmilitia.skin );

	if ( newmilitia.age > 50 && Chance( newmilitia.age - 50 ) )
		newmilitia.hair = WHITEHEAD;

	if ( newmilitia.bodytype == REGFEMALE )
	{
		newmilitia.forename = Random( gMilitiaOriginData[newmilitia.origin].szFemale_Forename.size( ) );
		newmilitia.surname = Random( gMilitiaOriginData[newmilitia.origin].szFemale_Surname.size( ) );
	}
	else
	{
		newmilitia.forename = Random( gMilitiaOriginData[newmilitia.origin].szMale_Forename.size( ) );
		newmilitia.surname = Random( gMilitiaOriginData[newmilitia.origin].szMale_Surname.size( ) );
	}

	newmilitia.flagmask = 0;
	
	newmilitia.healthratio = 100.0f;

	newmilitia.kills = 0;
	newmilitia.assists = 0;
	newmilitia.promotionpoints = 0;

	// promotion points are based on kills and assists. For proper accounting, award points for current rank
	newmilitia.AddKills( 0, 0 );

	// make up some history
	MILITIA_BATTLEREPORT report;
	report.flagmask |= MILITIA_BATTLEREPORT_FLAG_RECRUITED;
	report.id = GetWorldTotalMin( );
	
	newmilitia.history.push_back( report );

	gIndividualMilitiaVector.push_back( newmilitia );

	return newmilitia.id;
}

// search for a individual militia that is alive and not currently in use in this sector, and return its id
// if none is found, create new and return that one
UINT32 GetIdOfUnusedindividualMilitia( UINT8 aSoldierClass, UINT8 aSector )
{
	if ( !gGameExternalOptions.fIndividualMilitia )
		return 0;

	UINT8 militialevel = SoldierClassToMilitiaRank( aSoldierClass );

	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end();
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) && (*it).sector == aSector && (*it).militiarank == militialevel )
		{
			// fitting data found - now we have to make sure this one isn't already in use
			BOOLEAN found = FALSE;

			SOLDIERTYPE* pSoldier;

			INT32 cnt = gTacticalStatus.Team[MILITIA_TEAM].bFirstID;
			INT32 lastid = gTacticalStatus.Team[MILITIA_TEAM].bLastID;
			for (pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier)
			{
				if (pSoldier && pSoldier->bActive && (*it).id == pSoldier->usIndividualMilitiaID && IsLegalMilitiaId(pSoldier->usIndividualMilitiaID) )
				{
					found = TRUE;
					break;
				}
			}

			if ( !found && IndividualMilitiaInUse_AutoResolve((*it).id))
			{
				found = TRUE;
			}

			if (!found)
			{
				return (*it).id;
			}
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in GetIdOfUnusedindividualMilitia" );

	// nobody found. That shouldn't really happen, as we are supposed to create data whenever new militia is created. Create new data and use that
	return CreateNewIndividualMilitia( militialevel, MO_ARULCO, aSector );
}

// handle possible militia promotion and individual militia update
void HandlePossibleMilitiaPromotion( SOLDIERTYPE* pSoldier, BOOLEAN aAutoResolve )
{
	// we not only handle promotions here, but basically update this guy
	MILITIA militia;
	if ( GetMilitia( pSoldier->usIndividualMilitiaID, &militia ) )
	{
		militia.AddKills( pSoldier->ubMilitiaKills, pSoldier->ubMilitiaAssists );

		// if we have enough points, promote
		if ( militia.militiarank == GREEN_MILITIA)
		{
			if ( militia.promotionpoints >= gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Regular )
			{
				if ( militia.promotionpoints >= gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Regular + gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Elite )
				{
					++gbGreenToElitePromotions;
					++gbMilitiaPromotions;

					pSoldier->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;

					StrategicPromoteMilitiaInSector( pSoldier->sSectorX, pSoldier->sSectorY, GREEN_MILITIA, 1 );
					StrategicPromoteMilitiaInSector( pSoldier->sSectorX, pSoldier->sSectorY, REGULAR_MILITIA, 1 );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szIdividualMilitiaWebsiteText[18], militia.GetName( ) );
				}
				else
				{
					++gbGreenToRegPromotions;
					++gbMilitiaPromotions;

					pSoldier->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;

					StrategicPromoteMilitiaInSector( pSoldier->sSectorX, pSoldier->sSectorY, GREEN_MILITIA, 1 );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szIdividualMilitiaWebsiteText[17], militia.GetName( ) );
				}
			}
		}
		else if ( militia.militiarank == REGULAR_MILITIA)
		{
			if ( militia.promotionpoints >= gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Regular + gGameExternalOptions.usIndividualMilitia_PromotionPoints_To_Elite )
			{
				++gbRegToElitePromotions;
				++gbMilitiaPromotions;

				pSoldier->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;

				StrategicPromoteMilitiaInSector( pSoldier->sSectorX, pSoldier->sSectorY, REGULAR_MILITIA, 1 );

				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szIdividualMilitiaWebsiteText[18], militia.GetName( ) );
			}
		}

		// add a battle report if there was a battle (if there was a battle there will have been participants
		if ( aAutoResolve || (gCurrentIncident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_GREEN] +
			 gCurrentIncident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_REGULAR] +
			 gCurrentIncident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_ELITE]) )
		{
			MILITIA_BATTLEREPORT report;
			report.id = GetIdOfCurrentlyOngoingIncident( );

			// if we are in autoresolve, then the report has already been created at this point
			if ( aAutoResolve )
				report.id = max( 0, report.id - 1 );

			if ( pSoldier->stats.bLife < OKLIFE )
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_COMA;

			if ( pSoldier->ubMilitiaKills )
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY;

			if ( militia.healthratio - 100.0f * pSoldier->stats.bLife / pSoldier->stats.bLifeMax > 50 )
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_HEAVY;
			else if ( militia.healthratio - 100.0f * pSoldier->stats.bLife / pSoldier->stats.bLifeMax > 5 )
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_WOUNDED_SMALL;

			militia.history.push_back( report );
		}

		// separate report for promotion
		if ( militia.militiarank != SoldierClassToMilitiaRank( pSoldier->ubSoldierClass ) )
		{
			MILITIA_BATTLEREPORT report;
			report.id = GetWorldTotalMin( );
			report.flagmask |= MILITIA_BATTLEREPORT_FLAG_PROMOTED;
			militia.history.push_back( report );
		}

		militia.healthratio = 100.0f * pSoldier->stats.bLife / pSoldier->stats.bLifeMax;
		militia.militiarank = SoldierClassToMilitiaRank( pSoldier->ubSoldierClass );

		UpdateMilitia( militia );
	}
	else if ( pSoldier->ubMilitiaKills + pSoldier->ubMilitiaAssists > 0 )
	{
		UINT8 ubMilitiaRank = SoldierClassToMilitiaRank( pSoldier->ubSoldierClass );
		UINT8 ubPromotions = CheckOneMilitiaForPromotion( pSoldier->sSectorX, pSoldier->sSectorY, ubMilitiaRank, 2 * pSoldier->ubMilitiaKills + pSoldier->ubMilitiaAssists );
		if ( ubPromotions )
		{
			if ( ubPromotions == 2 )
			{
				++gbGreenToElitePromotions;
				++gbMilitiaPromotions;

				pSoldier->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
			}
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA )
			{
				++gbGreenToRegPromotions;
				++gbMilitiaPromotions;

				pSoldier->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
			}
			else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
			{
				++gbRegToElitePromotions;
				++gbMilitiaPromotions;

				pSoldier->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
			}
		}
	}

	pSoldier->ubMilitiaKills = 0;
	pSoldier->ubMilitiaAssists = 0;
}

void MoveIndividualMilitiaProfiles( UINT8 aSourceSector, UINT8 aTargetSector, UINT8 usGreens, UINT8 usRegulars, UINT8 usElites )
{
	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !usGreens && !usRegulars && !usElites )
			return;

		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) && (*it).sector == aSourceSector )
		{
			if ( usGreens && ( *it ).militiarank == GREEN_MILITIA)
			{
				(*it).sector = aTargetSector;
				--usGreens;
			}
			else if ( usRegulars && (*it).militiarank == REGULAR_MILITIA)
			{
				(*it).sector = aTargetSector;
				--usRegulars;
			}
			else if ( usElites && (*it).militiarank == ELITE_MILITIA)
			{
				(*it).sector = aTargetSector;
				--usElites;
			}
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in MoveIndividualMilitiaProfiles" );
}


// militia are disbanded - we have to take note of that
void DisbandIndividualMilitia( UINT8 aSector, UINT8 usGreens, UINT8 usRegulars, UINT8 usElites )
{
	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !usGreens && !usRegulars && !usElites )
			return;

		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) && (*it).sector == aSector )
		{
			if ( usGreens && (*it).militiarank == GREEN_MILITIA)
			{
				--usGreens;
			}
			else if ( usRegulars && (*it).militiarank == REGULAR_MILITIA)
			{
				--usRegulars;
			}
			else if ( usElites && (*it).militiarank == ELITE_MILITIA)
			{
				--usElites;
			}

			(*it).Fire();
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in DisbandIndividualMilitia" );
}

void PromoteIndividualMilitia( UINT8 aSector, UINT8 aSoldierClass )
{
	UINT8 militiarank = SoldierClassToMilitiaRank( aSoldierClass );

	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) && (*it).sector == aSector && (*it).militiarank == militiarank )
		{
			(*it).militiarank++;

			// to properly update promotion points
			(*it).AddKills(0, 0);

			return;
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in PromoteIndividualMilitia" );
}

// return the sum of daily wages, and the number of individual militia per class
UINT32 GetDailyUpkeep_IndividualMilitia( UINT32& arNumGreen, UINT32& arNumRegular, UINT32& arNumElite )
{
	UINT32 cost = 0;

	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) )
		{
			if ( (*it).militiarank == REGULAR_MILITIA )
			{
				++arNumRegular;
			}
			else if ( (*it).militiarank == ELITE_MILITIA )
			{
				++arNumElite;
			}
			else
			{
				++arNumGreen;
			}

			cost += (*it).GetWage();
		}
	}

	return cost;
}

// if the player repositions militia in a town in strategic, we need to reposition militia as well
void PickIndividualMilitia( UINT8 aSector, UINT8 ubType, UINT16 aNumber )
{
	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !aNumber )
			return;

		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED | MILITIAFLAG_NEEDS_SECTOR)) && (*it).sector == aSector )
		{
			if ( ubType == (*it).militiarank )
			{
				(*it).flagmask |= MILITIAFLAG_NEEDS_SECTOR;

				--aNumber;
			}
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( aNumber && gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in PickIndividualMilitia" );
}

void DropIndividualMilitia( UINT8 aSector, UINT8 ubType, UINT16 aNumber )
{
	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( !aNumber )
			return;

		if ( !((*it).flagmask & (MILITIAFLAG_DEAD | MILITIAFLAG_FIRED)) && ((*it).flagmask & MILITIAFLAG_NEEDS_SECTOR) )
		{
			if ( ubType == (*it).militiarank )
			{
				(*it).flagmask &= ~MILITIAFLAG_NEEDS_SECTOR;
				(*it).sector = aSector;

				--aNumber;
			}
		}
	}

	// if this feature is on and we get to this point, then there aren't enough individual militia. This is odd, the player should be informed
	if ( aNumber && gGameExternalOptions.fIndividualMilitia )
		ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"Possible error: Not enough individual militia found in DropIndividualMilitia" );
}
