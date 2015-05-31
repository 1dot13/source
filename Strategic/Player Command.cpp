#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Player Command.h"
	#include "Queen Command.h"
	#include "Campaign Types.h"
	#include "Strategic.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "text.h"
	#include "laptopsave.h"
	#include "quests.h"
	#include "Morale.h"
	#include "Overhead.h"
	#include "strategicmap.h"
	#include "Map Screen Helicopter.h"
	#include "Game Clock.h"
	#include "meanwhile.h"
	#include "email.h"
	#include "GameSettings.h"
	#include "Strategic Status.h"
	#include "Strategic AI.h"
	#include "PreBattle Interface.h"
	#include "Map Screen Interface.h"
	#include "Tactical Save.h"
	#include "interface Dialogue.h"
	#include "Random.h"
	// HEADROCK HAM 3.6: Added for facility string printing...
	#include "PopUpBox.h"
	#include "CampaignStats.h"	// added by Flugente
	#include "Town Militia.h"	// added by Flugente
	#include "LuaInitNPCs.h"	// added by Flugente
#endif

#include "postalservice.h"

#include "GameInitOptionsScreen.h"

extern BOOLEAN fMapScreenBottomDirty;
extern CPostalService gPostalService;


void AddFacilitiesToBox( INT16 sMapX, INT16 sMapY, UINT32 *uiHandle, BOOLEAN fCityInfoBox )
{
	// will build a string stating current facilities present in sector

	// HEADROCK HAM 3.5: Facilities are now read from XMLs, including their names.

	UINT32 cnt = 0;
	UINT32 uiNumFacilities = 0;
	BOOLEAN fHeaderAdded = FALSE;

	CHAR16 szFacilityString[30];

	Assert(sMapX > 0 && sMapY > 0 && sMapX < 17 && sMapY < 17);

	for ( cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++ )
	{
		if (fCityInfoBox && uiNumFacilities == 0 && !fHeaderAdded )
		{
			// For a city info box, always show this line on the left side if not hidden
			if( gfHiddenTown[ GetTownIdForSector( sMapX, sMapY ) ] )
			{
				swprintf( szFacilityString, L"%s:", pwTownInfoStrings[ 8 ] );
				AddMonoString( uiHandle, szFacilityString );
				fHeaderAdded = TRUE;
			}
		}
		// Facility type exists at this location?
		if (gFacilityLocations[SECTOR(sMapX,sMapY)][cnt].fFacilityHere)
		{
			if (gFacilityLocations[SECTOR(sMapX,sMapY)][cnt].ubHidden == 0 ||
				(gFacilityLocations[SECTOR(sMapX,sMapY)][cnt].ubHidden == 1 && SectorInfo[SECTOR(sMapX,sMapY)].fSurfaceWasEverPlayerControlled) )
			{
				if (!fCityInfoBox && uiNumFacilities == 0 && !fHeaderAdded )
				{
					// For a non-city info box, show this line on the left side only if there's at least one facility.
					swprintf( szFacilityString, L"%s:", pwTownInfoStrings[ 8 ] );
					AddMonoString( uiHandle, szFacilityString );
				}
				else if (uiNumFacilities > 0)
				{
					// Add empty line on the left size to make room for text on the right.
					AddMonoString( uiHandle, L"" );
				}

				AddSecondColumnMonoString( uiHandle, gFacilityTypes[cnt].szFacilityName );
				uiNumFacilities++;
			}
		}
	}

	if( uiNumFacilities == 0 && fCityInfoBox && gfHiddenTown[ GetTownIdForSector( sMapX, sMapY ) ])
	{
		// Add "NONE" on the right side. Only happens when the sector is a non-hidden city.
		AddSecondColumnMonoString( uiHandle, sFacilitiesStrings[0] );
		return;
	}

	/* HEADROCK HAM 3.4: Hardcodes removed.
	// hospital
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_HOSPITAL) )
	{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ FACILITY_HOSPITAL ] );
	}

	// industry
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_INDUSTRY) )
	{
		if( wcslen( sFacilitiesString ) == 0 )
		{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ 2 ] );
		}
		else
		{
			wcscat( sFacilitiesString, L", ");
			wcscat( sFacilitiesString, sFacilitiesStrings[ 2 ]);
		}
	}

	// prison
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_PRISON) )
	{
		if( wcslen( sFacilitiesString ) == 0 )
		{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ 3 ] );
		}
		else
		{
			wcscat( sFacilitiesString, L", ");
			wcscat( sFacilitiesString, sFacilitiesStrings[ 3 ]);
		}
	}

	// HEADROCK HAM 3.4: What about the military base!
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_MILITARY) )
	{
		if( wcslen( sFacilitiesString ) == 0 )
		{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ 4 ] );
		}
		else
		{
			wcscat( sFacilitiesString, L", ");
			wcscat( sFacilitiesString, sFacilitiesStrings[ 4 ]);
		}
	}

	// airport
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_AIRPORT) )
	{
		if( wcslen( sFacilitiesString ) == 0 )
		{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ 5 ] );
		}
		else
		{
			wcscat( sFacilitiesString, L", ");
			wcscat( sFacilitiesString, sFacilitiesStrings[ 5 ]);
		}
	}

	// gun range
	if( SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFacilitiesFlags & (1 << FACILITY_GUNRANGE) )
	{
		if( wcslen( sFacilitiesString ) == 0 )
		{
		swprintf( sFacilitiesString, L"%s", sFacilitiesStrings[ 6 ] );
		}
		else
		{
			wcscat( sFacilitiesString, L", ");
			wcscat( sFacilitiesString, sFacilitiesStrings[ 6 ]);
		}
	}
	*/

	return;
}


// ALL changes of control to player must be funneled through here!
BOOLEAN SetThisSectorAsPlayerControlled( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fContested )
{
	// NOTE: MapSector must be 16-bit, cause MAX_WORLD_X is actually 18, so the sector numbers exceed 256 although we use only 16x16
	UINT16 usMapSector = 0;
	BOOLEAN fWasEnemyControlled = FALSE;
	INT8 bTownId = 0;
	UINT8 ubSectorID;

#ifdef JA2UB
//Ja25: No meanwhiles
#else
	if( AreInMeanwhile( ) )
	{
		return FALSE;
	}
#endif


	if( bMapZ == 0 )
	{
		usMapSector = sMapX + ( sMapY * MAP_WORLD_X );

/*
		// if enemies formerly controlled this sector
		if (StrategicMap[ usMapSector ].fEnemyControlled)
		{
			// remember that the enemies have lost it
			StrategicMap[ usMapSector ].fLostControlAtSomeTime = TRUE;
		}
*/
		if( NumHostilesInSector( sMapX, sMapY, bMapZ ) )
		{ //too premature:	enemies still in sector.
			return FALSE;
		}

		if (LaptopSaveInfo.fBobbyRSiteCanBeAccessed == FALSE)
		{
			// Check if the sector is a shipment sector
			BOOLEAN isShipmentSector = gPostalService.IsSectorAShipmentSector((UINT8)sMapX, (UINT8)sMapY, (UINT8)bMapZ);

			if (isShipmentSector)
				LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
		}

		// WANNE: Get Airport sector from ShippingDestination.xml file!
		// check if we ever grabbed drassen airport, if so, set fact we can go to BR's
		//if( ( sMapX == BOBBYR_SHIPPING_DEST_SECTOR_X ) && ( sMapY == BOBBYR_SHIPPING_DEST_SECTOR_Y ) )
		if (LaptopSaveInfo.fBobbyRSiteCanBeAccessed)
		{
			//LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;

			//If the player has been to Bobbyr when it was down, and we havent already sent email, send him an email
			if( LaptopSaveInfo.ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction == BOBBYR_BEEN_TO_SITE_ONCE &&	LaptopSaveInfo.ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction != BOBBYR_ALREADY_SENT_EMAIL )
			{
#ifdef JA2UB
// no UB
#else
				AddEmail( BOBBYR_NOW_OPEN, BOBBYR_NOW_OPEN_LENGTH, BOBBY_R, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
#endif
				LaptopSaveInfo.ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction = BOBBYR_ALREADY_SENT_EMAIL;
			}
		}

		fWasEnemyControlled = StrategicMap[ usMapSector ].fEnemyControlled;

		StrategicMap[ usMapSector ].fEnemyControlled = FALSE;
		SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] = TRUE;

		bTownId = StrategicMap[ usMapSector ].bNameId;

		// check if there's a town in the sector
		if ((bTownId >= FIRST_TOWN) && (bTownId < NUM_TOWNS))
		{
			// yes, start tracking (& displaying) this town's loyalty if not already doing so
			StartTownLoyaltyIfFirstTime( bTownId );
		}


		// if player took control away from enemy 
		if( fWasEnemyControlled )
		{
			// WDS - New AI
			// Hook in here for some stuff

			// and it's a town
			if ((bTownId >= FIRST_TOWN) && (bTownId < NUM_TOWNS))
			{
				// don't do these for takeovers of Omerta sectors at the beginning of the game
				if ((bTownId != OMERTA) || (GetWorldDay() != 1))
				{
					ubSectorID = (UINT8)SECTOR( sMapX, sMapY );
					if( !bMapZ )
					{
						HandleMoraleEvent( NULL, MORALE_TOWN_LIBERATED, sMapX, sMapY, bMapZ );

						// single sector towns does not give global loyalty bonus
						if( GetTownSectorSize( bTownId ) != 1 )
						{
#ifdef JA2UB
//Ja25:	no loyalty
#else
						HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_GAIN_TOWN_SECTOR, sMapX, sMapY, bMapZ );
#endif
						}
					// liberation by definition requires that the place was enemy controlled in the first place
					CheckIfEntireTownHasBeenLiberated( bTownId, sMapX, sMapY );
					}
				}
			}

			// if it's a mine that's still worth something
			if ( IsThereAMineInThisSector( sMapX, sMapY ) )
			{
				if ( GetTotalLeftInMine( GetMineIndexForSector( sMapX, sMapY ) ) > 0)
				{
					HandleMoraleEvent( NULL, MORALE_MINE_LIBERATED, sMapX, sMapY, bMapZ );
#ifdef JA2UB
// Ja25:	no loyalty
#else
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_GAIN_MINE, sMapX, sMapY, bMapZ );
#endif
				}
			}

			// if it's a SAM site sector
			if( IsThisSectorASAMSector( sMapX, sMapY, bMapZ ) )
			{
				if ( 1 /*!GetSectorFlagStatus( sMapX, sMapY, bMapZ, SF_SECTOR_HAS_BEEN_LIBERATED_ONCE ) */)
				{
					// SAM site liberated for first time, schedule meanwhile
#ifdef JA2UB
//JA25 No meanwhiles
#else
					HandleMeanWhileEventPostingForSAMLiberation( GetSAMIdFromSector( sMapX, sMapY, bMapZ ) );
#endif
				}

				HandleMoraleEvent( NULL, MORALE_SAM_SITE_LIBERATED, sMapX, sMapY, bMapZ );
#ifdef JA2UB
// Ja25:	no loyalty
#else
				HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_GAIN_SAM, sMapX, sMapY, bMapZ );
				UpdateAirspaceControl( );
#endif
				// if Skyrider has been delivered to chopper, and already mentioned Drassen SAM site, but not used this quote yet
				if ( IsHelicopterPilotAvailable( ) && ( guiHelicopterSkyriderTalkState >= 1 ) && ( !gfSkyriderSaidCongratsOnTakingSAM ) )
				{
					SkyRiderTalk( SAM_SITE_TAKEN );
					gfSkyriderSaidCongratsOnTakingSAM = TRUE;
				}

				if ( !SectorInfo[ SECTOR( sMapX, sMapY ) ].fSurfaceWasEverPlayerControlled )
				{
					// grant grace period
					UpdateLastDayOfPlayerActivity( ( UINT16 ) ( GetWorldDay() + zDeffSetting[gGameOptions.ubDifficultyLevel].iUpdateLastDayOfPlayerActivity ) );
				}
			}

			// if it's a helicopter refueling site sector
			if( IsRefuelSiteInSector( sMapX, sMapY) )
			{
				UpdateRefuelSiteAvailability( );
			}

//			SetSectorFlag( sMapX, sMapY, bMapZ, SF_SECTOR_HAS_BEEN_LIBERATED_ONCE );
			if ( bMapZ == 0 && ( ( ( sMapX >= gModSettings.ubMeanwhileMedunaOutskirtsRowMinX && sMapX <= gModSettings.ubMeanwhileMedunaOutskirtsRowMaxX ) && //row x range
				sMapY == gModSettings.ubMeanwhileMedunaOutskirtsRowY ) || //row y
				( sMapX == gModSettings.ubMeanwhileMedunaOutskirtsColX && // col x
				( sMapY >= gModSettings.ubMeanwhileMedunaOutskirtsColMinY && sMapY <= gModSettings.ubMeanwhileMedunaOutskirtsColMaxY ) ) ) ) //col y range
			{

#ifdef JA2UB
//Ja25 No meanwhiles
#else
				HandleOutskirtsOfMedunaMeanwhileScene();
#endif

			}
		}
#ifdef JA2UB
//Ja25 No strategic ai
#else
		if( fContested )
		{
			StrategicHandleQueenLosingControlOfSector( (UINT8)sMapX, (UINT8)sMapY, (UINT8)bMapZ );
		}
#endif
	}
	else
	{
#ifdef JA2UB
//Ja25 No strategic ai
#else

		if( sMapX == gModSettings.ubQueenBasementSectorX && sMapY == gModSettings.ubQueenBasementSectorY 
			&& bMapZ == gModSettings.ubQueenBasementSectorZ )
		{ //Basement sector
			gfUseAlternateQueenPosition = TRUE;
		}
#endif
	}

	// also set fact the player knows they own it
	SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] = TRUE;

	// Flugente: campaign stats
	if ( !SectorInfo[ SECTOR( sMapX, sMapY ) ].fSurfaceWasEverPlayerControlled )
	{
		UINT8 townid = GetTownIdForSector( sMapX, sMapY );
		if ( townid != BLANK_SECTOR )
		{
			// first liberation of a town sector -> special texts
			gCurrentIncident.usIncidentFlags |= INCIDENT_FIRST_LIBERATION;

			// liberating a town sector for the first time grants us an initial wave of volunteers
			UINT16 population = GetSectorPopulation( sMapX, sMapY );

			FLOAT loyalpopulation = gTownLoyalty[townid].ubRating * population / 100;
			
			AddVolunteers( loyalpopulation * gGameExternalOptions.dMilitiaVolunteerGainFactorLiberation );
		}
	}

	// Flugente: if we take the surface sector for the first time, script-defined actions might happen
	LuaHandleSectorLiberation( sMapX, sMapY, bMapZ, !SectorInfo[SECTOR( sMapX, sMapY )].fSurfaceWasEverPlayerControlled );

	if ( bMapZ == 0 )
	{
		SectorInfo[ SECTOR( sMapX, sMapY ) ].fSurfaceWasEverPlayerControlled = TRUE;
	}

	//KM : Aug 11, 1999 -- Patch fix:	Relocated this check so it gets called everytime a sector changes hands,
	//	 even if the sector isn't a SAM site.	There is a bug _somewhere_ that fails to update the airspace,
	//	 even though the player controls it.
	//UpdateAirspaceControl( );

	// redraw map/income if in mapscreen
	fMapPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;

	return fWasEnemyControlled;
}


// ALL changes of control to enemy must be funneled through here!
BOOLEAN SetThisSectorAsEnemyControlled( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fContested )
{
	UINT16 usMapSector = 0;
	BOOLEAN fWasPlayerControlled = FALSE;
	INT8 bTownId = 0;
	UINT8 ubTheftChance;
	UINT8 ubSectorID;

	//KM : August 6, 1999 Patch fix
	//	 This check was added because this function gets called when player mercs retreat from an unresolved
	//	 battle between militia and enemies.	It will get called again AFTER autoresolve is finished.
	if( gfAutomaticallyStartAutoResolve )
	{
		return( FALSE );
	}

	if( bMapZ == 0 )
	{
		usMapSector = sMapX + ( sMapY * MAP_WORLD_X );

		fWasPlayerControlled = !StrategicMap[ usMapSector ].fEnemyControlled;

		StrategicMap[ usMapSector ].fEnemyControlled = TRUE;

		// if player lost control to the enemy
		if ( fWasPlayerControlled )
		{
			if( PlayerMercsInSector( (UINT8)sMapX, (UINT8)sMapY, (UINT8)bMapZ ) )
			{ //too premature:	Player mercs still in sector.
				return FALSE;
			}

			// check if there's a town in the sector
			bTownId = StrategicMap[ usMapSector ].bNameId;

			SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] = FALSE;

			// and it's a town
			if ((bTownId >= FIRST_TOWN) && (bTownId < NUM_TOWNS))
			{
				ubSectorID = (UINT8)SECTOR( sMapX, sMapY );
				if( !bMapZ && ubSectorID != SEC_J9 && ubSectorID != SEC_K4 )
				{
					HandleMoraleEvent( NULL, MORALE_TOWN_LOST, sMapX, sMapY, bMapZ );
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_LOSE_TOWN_SECTOR, sMapX, sMapY, bMapZ );

					CheckIfEntireTownHasBeenLost( bTownId, sMapX, sMapY );
				}
			}

			// if the sector has a mine which is still worth something
			if ( IsThereAMineInThisSector( sMapX, sMapY ) )
			{
				// if it isn't empty
				if ( GetTotalLeftInMine( GetMineIndexForSector( sMapX, sMapY ) ) > 0)
				{
					QueenHasRegainedMineSector(GetMineIndexForSector (sMapX, sMapY));
					HandleMoraleEvent( NULL, MORALE_MINE_LOST, sMapX, sMapY, bMapZ );
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_LOSE_MINE, sMapX, sMapY, bMapZ );
				}
			}

			// if it's a SAM site sector
			if( IsThisSectorASAMSector( sMapX, sMapY, bMapZ ) )
			{
				HandleMoraleEvent( NULL, MORALE_SAM_SITE_LOST, sMapX, sMapY, bMapZ );
				HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_LOSE_SAM, sMapX, sMapY, bMapZ );
				UpdateAirspaceControl( );
			}

			// if it's a helicopter refueling site sector
			if( IsRefuelSiteInSector( sMapX, sMapY) )
			{
				UpdateRefuelSiteAvailability( );
			}

			// ARM: this must be AFTER all resulting loyalty effects are resolved, or reduced mine income shown won't be accurate
			NotifyPlayerWhenEnemyTakesControlOfImportantSector( sMapX, sMapY, 0, fContested );
		}

		// NOTE: Stealing is intentionally OUTSIDE the fWasPlayerControlled branch.	This function gets called if new
		// enemy reinforcements arrive, and they deserve another crack at stealing what the first group missed! :-)

		// stealing should fail anyway 'cause there shouldn't be a temp file for unvisited sectors, but let's check anyway
		if ( GetSectorFlagStatus( sMapX, sMapY, ( UINT8 ) bMapZ, SF_ALREADY_VISITED ) == TRUE )
		{
			// enemies can steal items left lying about (random chance).	The more there are, the more they take!
			ubTheftChance = 5 * NumEnemiesInAnySector( sMapX, sMapY, bMapZ );
			// max 90%, some stuff may just simply not get found
			if (ubTheftChance > 90 )
			{
				ubTheftChance = 90;
			}
			RemoveRandomItemsInSector( sMapX, sMapY, bMapZ, ubTheftChance );
		}

		// don't touch fPlayer flag for a surface sector lost to the enemies!
		// just because player has lost the sector doesn't mean he realizes it - that's up to our caller to decide!
	}
	else
	{
		// underground sector control is always up to date, because we don't track control down there
		SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] = FALSE;
	}

	//KM : Aug 11, 1999 -- Patch fix:	Relocated this check so it gets called everytime a sector changes hands,
	//	 even if the sector isn't a SAM site.	There is a bug _somewhere_ that fails to update the airspace,
	//	 even though the player controls it.
	//UpdateAirspaceControl( );

	// redraw map/income if in mapscreen
	fMapPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;

	return fWasPlayerControlled;
}


#ifdef JA2TESTVERSION
void ClearMapControlledFlags( void )
{
	INT32 iCounterA = 0, iCounterB = 0;
	UINT16 usMapSector = 0;


	for( iCounterA = 1; iCounterA < MAP_WORLD_X - 1; iCounterA++ )
	{
		for( iCounterB = 1; iCounterB < MAP_WORLD_Y - 1; iCounterB++ )
		{
			usMapSector = iCounterA + ( iCounterB * MAP_WORLD_X );
			StrategicMap[ usMapSector ].fEnemyControlled = FALSE;
			SectorInfo[ SECTOR( iCounterA, iCounterB ) ].fPlayer[ 0 ] = TRUE;
		}
	}
	UpdateAirspaceControl( );
}
#endif


/*
BOOLEAN IsTheSectorPerceivedToBeUnderEnemyControl( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{

	// are we in battle in this sector?
	if( ( sMapX == gWorldSectorX ) && ( sMapY == gWorldSectorY ) && ( bMapZ == gbWorldSectorZ ) && ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		return( TRUE );
	}


	// does the player believe this sector is under enemy control?
	return( !( SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] ) );
}


void MakePlayerPerceptionOfSectorControlCorrect( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	if (bMapZ == 0)
	{
		SectorInfo[ SECTOR( sMapX, sMapY ) ].fPlayer[ bMapZ ] = !( StrategicMap[ CALCULATE_STRATEGIC_INDEX( sMapX, sMapY ) ].fEnemyControlled );
	}
	// else nothing, underground sector control is always up to date, because we don't track control down there

	fMapPanelDirty = TRUE;
}
*/

void ReplaceSoldierProfileInPlayerGroup( UINT8 ubGroupID, UINT8 ubOldProfile, UINT8 ubNewProfile )
{
	GROUP *pGroup;
	PLAYERGROUP *curr;

	pGroup = GetGroup( ubGroupID );

	if ( !pGroup )
	{
		return;
	}

	curr = pGroup->pPlayerList;

	while( curr )
	{
		if( curr->ubProfileID == ubOldProfile )
		{
			// replace and return!
			curr->ubProfileID = ubNewProfile;
			return;
		}
		curr = curr->next;
	}

}
