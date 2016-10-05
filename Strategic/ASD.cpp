/**
* @file
* @author Flugente (bears-pit.com)
*/

#ifdef PRECOMPILEDHEADERS
#include "Laptop All.h"
#else
#include <math.h>
#include "ASD.h"
#include "strategic.h"
#include "WCheck.h"
#include "Utilities.h"
#include "WordWrap.h"
#include "Cursors.h"
#include "Line.h"
#include "Insurance Text.h"
#include "Encrypted File.h"
#include "Text.h"
#include "Multi Language Graphic Utils.h"
#include "random.h"
#include "Interface.h"
#include "Soldier Add.h"
#include "Soldier Profile.h"
#include "Overhead.h"
#include "Map Screen Interface.h"
#include "finances.h"
#include "Game Clock.h"
#include "SaveLoadGame.h"
#include "GameVersion.h"
#include "message.h"
#include "Game Event Hook.h"
#include "militia control.h"
#include "Town Militia.h"
#include "Strategic Town Loyalty.h"
#include "map information.h"

#include "Campaign.h"
#include "random.h"
#include "Explosion Control.h"
#include "Strategic AI.h"
#include "Strategic Mines.h"
#include "Reinforcement.h"
#include "Player Command.h"
#include "Queen Command.h"
#include "Map Screen Helicopter.h"
#include "Points.h"
#include "worldman.h"
#include "Map Information.h"
#include "opplist.h"
#include "Dialogue Control.h"
#include "Sound Control.h"
#include "renderworld.h"
#include "Isometric Utils.h"
#endif


extern INT32 giReinforcementPool;

extern BOOLEAN gfTacticalDoHeliRun;

// Arulco special division decision code

// ASD savegame variables

// resources
INT32 gASDResource[ASD_RESOURCE_MAX];

// remember which resources we've already bought but haven't received yet
INT32 gASDResource_Ordered[ASD_RESOURCE_MAX];

UINT32 gASD_Flags = 0;

// enemy heli constants
#define	ENEMYHELI_DESTROYED					0x00000001
#define	ENEMYHELI_ORDER_GOTODESTINATION		0x00000002
#define	ENEMYHELI_KNOWNTOPLAYER				0x00000008			// the player knows about this heli (it will this be displayed on the map)

#define	ENEMYHELI_ORDER_DROPTROOPS			0x00000010
#define	ENEMYHELI_ORDER_PICKUPTROOPS		0x00000020
#define	ENEMYHELI_REPAIRING					0x00000040			// we are currently repairing
#define	ENEMYHELI_REFUELING					0x00000080

#define ENEMYHELI_SHOTDOWN					0x00000100			// heli was destroyed by a SAM (instead of being bombed on the ground etc.)

#define	ENEMYHELI_ORDERS					(ENEMYHELI_ORDER_GOTODESTINATION|ENEMYHELI_ORDER_DROPTROOPS|ENEMYHELI_ORDER_PICKUPTROOPS|ENEMYHELI_REPAIRING|ENEMYHELI_REFUELING)

#define ENEMYHELI_MAX				2

// for now, this stays fixed
UINT32 gEnemyHeliTravelTimePerSector = 10;
UINT8 gEnemyHeliMaxTroops = 6;

std::vector<ENEMY_HELI>	gEnemyHeliVector;

void InitASD( )
{
	gASDResource[ASD_MONEY] = 200000;
	gASDResource[ASD_FUEL] = 0;
	gASDResource[ASD_HELI] = 0;
	gASDResource[ASD_JEEP] = 2;
	gASDResource[ASD_TANK] = 2;

	gASDResource_Ordered[ASD_MONEY] = 0;
	gASDResource_Ordered[ASD_FUEL] = 0;
	gASDResource_Ordered[ASD_HELI] = 0;
	gASDResource_Ordered[ASD_JEEP] = 0;
	gASDResource_Ordered[ASD_TANK] = 0;

	gASD_Flags = 0;

	EnemyHeliInit( );
}

BOOLEAN SaveASDData( HWFILE hwFile )
{
	UINT32 uiNumBytesWritten = 0;

	// ASD data
	for ( int i = ASD_MONEY; i < ASD_RESOURCE_MAX; ++i )
	{
		if ( !FileWrite( hwFile, &gASDResource[i], sizeof(INT32), &uiNumBytesWritten ) )	return(FALSE);
	}

	for ( int i = ASD_MONEY; i < ASD_RESOURCE_MAX; ++i )
	{
		if ( !FileWrite( hwFile, &gASDResource_Ordered[i], sizeof(INT32), &uiNumBytesWritten ) )	return(FALSE);
	}

	if ( !FileWrite( hwFile, &gASD_Flags, sizeof(UINT32), &uiNumBytesWritten ) )		return(FALSE);

	// in order to save a vector, we first save its size and then its content
	UINT32 size = gEnemyHeliVector.size( );
	if ( !FileWrite( hwFile, &size, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	for ( UINT32 i = 0; i < size; ++i )
	{
		if ( !FileWrite( hwFile, &gEnemyHeliVector[i], sizeof(ENEMY_HELI), &uiNumBytesWritten ) )
			return(FALSE);
	}

	return TRUE;
}

BOOLEAN LoadASDData( HWFILE hwFile )
{
	UINT32 numBytesRead = 0;

	// ASD data
	for ( int i = ASD_MONEY; i < ASD_RESOURCE_MAX; ++i )
	{
		numBytesRead = ReadFieldByField( hwFile, &gASDResource[i], sizeof(gASDResource[i]), sizeof(INT32), numBytesRead );
	}

	for ( int i = ASD_MONEY; i < ASD_RESOURCE_MAX; ++i )
	{
		numBytesRead = ReadFieldByField( hwFile, &gASDResource_Ordered[i], sizeof(gASDResource_Ordered[i]), sizeof(INT32), numBytesRead );
	}

	numBytesRead = ReadFieldByField( hwFile, &gASD_Flags, sizeof(gASD_Flags), sizeof(UINT32), numBytesRead );

	// clear any artefacts prior to loading
	EnemyHeliInit( );

	UINT32 size = 0;
	numBytesRead = ReadFieldByField( hwFile, &size, sizeof(size), sizeof(UINT32), numBytesRead );

	for ( UINT32 i = 0; i < size; ++i )
	{
		ENEMY_HELI data;
		numBytesRead = ReadFieldByField( hwFile, &data, sizeof(data), sizeof(ENEMY_HELI), 0 );

		gEnemyHeliVector.push_back( data );
	}

	return TRUE;
}

// when called, evaluate the current situation, and perform actions to harm the player if necessary
void UpdateASD( )
{
	// if feature is not active, nothing will happen
	if ( !gGameExternalOptions.fASDActive )
		return;

	if ( DidGameJustStart( ) )
		return;

	// enemy helis are unlocked either by a cutscene OR if a minimum progress threshold has been reached
	if ( !(gASD_Flags & ASDFACT_HELI_UNLOCKED) && HighestPlayerProgressPercentage( ) >= gGameExternalOptions.usEnemyHeliMinimumProgress )
		SetASDFlag( ASDFACT_HELI_UNLOCKED );

	if ( !(gASD_Flags & ASDFACT_JEEP_UNLOCKED) && HighestPlayerProgressPercentage( ) >= gGameExternalOptions.usJeepMinimumProgress )
		SetASDFlag( ASDFACT_JEEP_UNLOCKED );

	if ( !(gASD_Flags & ASDFACT_TANK_UNLOCKED) && HighestPlayerProgressPercentage( ) >= gGameExternalOptions.usTankMinimumProgress )
		SetASDFlag( ASDFACT_TANK_UNLOCKED );

	
	// determine whether we need to buy new toys
	ASDDecideOnPurchases();

	// decide how to use helicopters
	ASDDecideHeliOperations();	
}

void ASDDecideOnPurchases()
{
	INT32 needed_fuel = 0;
	INT32 needed_heli = 0;
	INT32 needed_tank = 0;
	INT32 needed_jeep = 0;

	UINT16 highestplayerprogress = HighestPlayerProgressPercentage( );

	// do we need new helis?
	if ( gASD_Flags & ASDFACT_HELI_UNLOCKED && gGameExternalOptions.fEnemyHeliActive )
	{
		// determine number of SAMs under AI control
		UINT16 numenemysams = 0;

		for ( UINT16 x = 0; x < MAX_NUMBER_OF_SAMS; ++x )
		{
			StrategicMapElement *pSAMStrategicMap = &(StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[x] )]);

			if ( pSamList[x] && pSAMStrategicMap && pSAMStrategicMap->fEnemyControlled )
				++numenemysams;
		}

		// only acquire/replace helis if there is still airspace the AI controls
		if ( numenemysams )
		{
			UINT32 activehelis = 0;

			for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != gEnemyHeliVector.end( ); ++it )
			{
				if ( !((*it).flagmask & ENEMYHELI_DESTROYED) )
					++activehelis;
			}

			needed_heli = ENEMYHELI_MAX - activehelis;

			// for any heli, we need fuel
			needed_fuel += (activehelis + needed_heli) * gGameExternalOptions.gEnemyHeliMaxFuel;
		}
	}

	// do we need new jeeps?
	if ( gASD_Flags & ASDFACT_JEEP_UNLOCKED )
	{
		// how many would we like to have? Note that this refers to the ones in our pool, the vehicles awarded to enemy groups are no longer accounted for
		needed_jeep = min( 10, 2 + highestplayerprogress / 10 );

		// if we already unlocked tanks, we no longer want jeeps - they are replaced by tanks
		if ( gASD_Flags & ASDFACT_TANK_UNLOCKED )
		{
			needed_jeep = 0;
		}

		// how many are needed?
		needed_jeep = max( 0, needed_jeep - gASDResource[ASD_JEEP] );

		needed_fuel += needed_jeep * ASDResourceCostFuel( ASD_JEEP );
	}

	// do we need new tanks?
	if ( gASD_Flags & ASDFACT_TANK_UNLOCKED )
	{
		// how many would we like to have? Note that this refers to the ones in our pool, the vehicles awarded to enemy groups are no longer accounted for
		needed_tank = min( 5, highestplayerprogress / 20 );

		// how many are needed?
		needed_tank = max( 0, needed_tank - gASDResource[ASD_TANK] );

		needed_fuel += needed_tank * ASDResourceCostFuel( ASD_TANK );
	}

	// how much new fuel do we need?
	needed_fuel = max( 0, needed_fuel - gASDResource[ASD_FUEL] );

	// reduce order by what we'e alreay ordered
	needed_fuel = max( 0, needed_fuel - gASDResource_Ordered[ASD_FUEL] );
	needed_jeep = max( 0, needed_jeep - gASDResource_Ordered[ASD_JEEP] );
	needed_tank = max( 0, needed_tank - gASDResource_Ordered[ASD_TANK] );
	needed_heli = max( 0, needed_heli - gASDResource_Ordered[ASD_HELI] );

	// check whether we can afford all new purchases. If we can't, lower your expectations
	while ( needed_fuel * ASDResourceCostMoney( ASD_FUEL ) +
			needed_jeep * ASDResourceCostMoney( ASD_JEEP ) +
			needed_tank * ASDResourceCostMoney( ASD_TANK ) +
			needed_heli * ASDResourceCostMoney( ASD_HELI ) > gASDResource[ASD_MONEY] )
	{
		if ( needed_tank ) --needed_tank;
		else if ( needed_heli ) --needed_heli;
		else if ( needed_jeep ) --needed_jeep;
		else needed_fuel = max( 0, needed_fuel - 10 );
	}

	// purchase things
	if ( needed_fuel )
	{
		AddStrategicAIResources( ASD_MONEY, -needed_fuel * ASDResourceCostMoney( ASD_FUEL ) );

		gASDResource_Ordered[ASD_FUEL] += needed_fuel;

		AddStrategicEvent( EVENT_ASD_PURCHASE_FUEL, GetWorldTotalMin( ) + ASDResourceDeliveryTime( ASD_FUEL ), needed_fuel );
	}

	if ( needed_jeep )
	{
		AddStrategicAIResources( ASD_MONEY, -needed_jeep * ASDResourceCostMoney( ASD_JEEP ) );
		AddStrategicAIResources( ASD_FUEL,  -needed_jeep * ASDResourceCostFuel( ASD_JEEP ) );

		gASDResource_Ordered[ASD_JEEP] += needed_jeep;

		AddStrategicEvent( EVENT_ASD_PURCHASE_JEEP, GetWorldTotalMin( ) + ASDResourceDeliveryTime( ASD_JEEP ), needed_jeep );
	}

	if ( needed_tank )
	{
		AddStrategicAIResources( ASD_MONEY, -needed_tank * ASDResourceCostMoney( ASD_TANK ) );
		AddStrategicAIResources( ASD_FUEL,  -needed_tank * ASDResourceCostFuel( ASD_TANK ) );

		gASDResource_Ordered[ASD_TANK] += needed_tank;

		AddStrategicEvent( EVENT_ASD_PURCHASE_TANK, GetWorldTotalMin( ) + ASDResourceDeliveryTime( ASD_TANK ), needed_tank );
	}

	if ( needed_heli )
	{
		AddStrategicAIResources( ASD_MONEY, -needed_heli * ASDResourceCostMoney( ASD_HELI ) );

		gASDResource_Ordered[ASD_HELI] += needed_heli;

		AddStrategicEvent( EVENT_ASD_PURCHASE_HELI, GetWorldTotalMin( ) + ASDResourceDeliveryTime( ASD_HELI ), needed_heli );
	}
}

void ASDDecideHeliOperations()
{
	if ( !gGameExternalOptions.fEnemyHeliActive )
		return;

	INT16 id = -1;

	// loop over all helis and, if they are idle, give them orders
	std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
	for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
	{
		++id;

		if ( (*it).flagmask & ENEMYHELI_DESTROYED )
			continue;

		if ( (*it).flagmask & ENEMYHELI_ORDERS )
			continue;

		// safety: make sure heli has fuel and is repaired (player might have sabotaged us...)
		UpdateEnemyHeli( id );

		if ( (*it).flagmask & ENEMYHELI_ORDERS )
			continue;

		// if we dont have enough soldiers left to staff a helicopter, don't send one out
		if ( giReinforcementPool < gEnemyHeliMaxTroops )
			continue;

		ENEMY_HELI& heli = (*it);

		UINT32 totalrating = 0;
		std::vector< std::pair<UINT8, INT16> > ratedhelioperations;

		// build a list of all possible locations that require us to drop troops in
		for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
		{
			for ( UINT8 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
			{
				// not if the enemy controls this sector
				if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].fEnemyControlled )
					continue;

				UINT8 sector = SECTOR( sX, sY );

				// if the player has never been here (or if the grace period is not over), this is no valid target
				if ( !OkayForEnemyToMoveThroughSector( sector ) )
					continue;

				// if this is already the target of another helicopter, ignore it
				BOOLEAN fOtherHeliAlreadyAssigned = FALSE;
				std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
				for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
				{
					if ( (*it).flagmask & ENEMYHELI_DESTROYED )
						continue;

					if ( (*it).flagmask & (ENEMYHELI_ORDER_GOTODESTINATION) && (*it).sector_destination == sector )
					{
						fOtherHeliAlreadyAssigned = TRUE;
						break;
					}
				}

				if ( fOtherHeliAlreadyAssigned )
					continue;

				SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

				if ( !pSectorInfo )
					continue;

				// if we would run out of fuel while doing this, don't
				if ( 2 * (abs( SECTORX( heli.sector_current ) - SECTORX( sector ) ) + abs( SECTORY( heli.sector_current ) - SECTORY( sector ) )) > heli.fuel )
					continue;

				// if number of SAM contacts is too high, don't consider this sector
				if ( 2 * heli.SetHeliFlightPath( SECTOR( sX, sY ) ) > gGameExternalOptions.gEnemyHeliAllowedSAMContacts )
					continue;

				INT16 rating = 0;					// how important we deem attacking this sector

				// on a road, we could set up a roadblock.
				if ( IsSectorRoad( sX, sY ) )
					rating += 2;

				// if volunteer feature is on, farm sectors are a valid target, as they increase the rate with which the player gains volunteers
				if ( gGameExternalOptions.fMilitiaVolunteerPool && IsSectorFarm( sX, sY ) )
					rating += 5;

				// town sectors are worth more
				if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].bNameId != BLANK_SECTOR )
					rating += 10;

				// if we take a mine, we cut the player's income - very juicy
				if ( IsThereAMineInThisSector( sX, sY ) )
					rating += 20;

				// if this is a player-owned SAM, it is very important that we take this. Should the SAM be operational, we will likely have ignore this tdue to the earlier check for SAM contacts
				for ( UINT16 x = 0; x < MAX_NUMBER_OF_SAMS; ++x )
				{
					if ( pSamList[x] == sector )
					{
						rating += 20;

						break;
					}
				}

				// lower a sector's rating for the amount of militia and mercs here - no sense in dropping a squad into a killing field
				// for now, the AI simply knows the player's amount of troops - later on, try to rely on espionage
				UINT16 num_militia = CountAllMilitiaInFiveSectors( sX, sY );
				UINT16 num_mercs = PlayerMercsInSector( sX, sY, 0 );
				if ( num_militia + num_mercs )
				{
					if ( num_militia + num_mercs > 2 * gEnemyHeliMaxTroops )
						rating = 0;
					else if ( num_militia + num_mercs > gEnemyHeliMaxTroops )
						rating /= 2;
					// only a very small garrison - we can wipe them out with no risk and add losses to the player. We should totally do that!
					else if ( num_militia + num_mercs < gEnemyHeliMaxTroops )
						rating *= 1.5f;
				}

				if ( rating > 0 )
				{
					totalrating += rating;
					ratedhelioperations.push_back( std::make_pair( sector, rating ) );
				}
			}
		}

		// for safety, delete set flight data
		heli.sector_destination = 0;
		heli.sector_waypoint = 0;
		heli.flagmask &= ~ENEMYHELI_ORDER_GOTODESTINATION;

		// if there are viable operations, pick one at random (weighted by rating) and start that operation
		if ( totalrating && !ratedhelioperations.empty( ) )
		{
			UINT32 pickedrating = Random( totalrating );
			UINT32 ratingsofar = 0;

			for ( std::vector< std::pair<UINT8, INT16> >::iterator it = ratedhelioperations.begin( ); it != ratedhelioperations.end( ); ++it )
			{
				if ( pickedrating <= ratingsofar + (*it).second )
				{
					// attack this sector!
					heli.SetHeliFlightPath( (*it).first );

					heli.troopcount = gEnemyHeliMaxTroops;
					giReinforcementPool -= gEnemyHeliMaxTroops;
					heli.flagmask |= ENEMYHELI_ORDER_DROPTROOPS;

					AddStrategicEvent( EVENT_ENEMY_HELI_UPDATE, GetWorldTotalMin( ) + gEnemyHeliTravelTimePerSector, id );

					break;
				}

				ratingsofar += (*it).second;
			}
		}
	}
}

void SetASDFlag( UINT32 aFlag )
{
	gASD_Flags |= aFlag;
}

UINT32 ASDResourceDeliveryTime( UINT8 aType )
{
	return gGameExternalOptions.gASDResource_BuyTime[aType];
}

UINT32 ASDResourceCostMoney( UINT8 aType )
{
	return gGameExternalOptions.gASDResource_Cost[aType];
}

// add resources to the AIs resource pool
void AddStrategicAIResources( UINT8 aType, INT32 aAmount )
{
	gASDResource[aType] = max( 0, gASDResource[aType] + aAmount );

	if ( aType == ASD_HELI )
		AddNewHeli( );
}

void ASDReduceOrderedStrategicResources( UINT8 aType, INT32 aAmount )
{
	gASDResource_Ordered[aType] = max( 0, gASDResource_Ordered[aType] - aAmount );
}

//////////////////////////////////////////// enemy helis /////////////////////////////////////////////////////////

// determine best heli flight path (minimising contact to hostile SAM sectors) and return number of possible SAM contacts
UINT8 ENEMY_HELI::SetHeliFlightPath( UINT8 aDest )
{
	sector_destination = aDest;
	flagmask |= ENEMYHELI_ORDER_GOTODESTINATION;

	// determine best waypoint
	// standard way of pathing is to minimise the max deviation of x and y
	// we are allowed to set one intermediate waypoint
	// For now, we try 3 methods:
	// 1. no waypoint (waypoint equals destination)
	// 2. waypoint is x_start, y_dest
	// 3. waypoint is x_dest, y_start
	// we pick the waypoint whose path has the least amount of hostile SAM sectors.

	UINT8 best_samcontacts = 0;
	UINT8 tmpsect = sector_current;
	UINT8 tmpcontacts = 0;

	// 1. no waypoint (waypoint equals destination)
	best_samcontacts = NumPlayerAirSpaceOnHeliPath( sector_current, sector_destination );
	sector_waypoint = sector_destination;

	// 2. waypoint is x_start, y_dest
	tmpsect = SECTOR( SECTORX( sector_current ), SECTORY( sector_destination ) );
	tmpcontacts = NumPlayerAirSpaceOnHeliPath( sector_current, tmpsect ) + NumPlayerAirSpaceOnHeliPath( tmpsect, sector_destination );
	if ( tmpcontacts < best_samcontacts )
	{
		best_samcontacts = tmpcontacts;
		sector_waypoint = tmpsect;
	}

	// 3. waypoint is x_dest, y_start
	tmpsect = SECTOR( SECTORX( sector_destination ), SECTORY( sector_current ) );
	tmpcontacts = NumPlayerAirSpaceOnHeliPath( sector_current, tmpsect ) + NumPlayerAirSpaceOnHeliPath( tmpsect, sector_destination );
	if ( tmpcontacts < best_samcontacts )
	{
		best_samcontacts = tmpcontacts;
		sector_waypoint = tmpsect;
	}

	return best_samcontacts;
}

void ENEMY_HELI::Destroy( )
{
	hp = 0;
	
	flagmask |= ENEMYHELI_DESTROYED;

	// if the player knows about this heli, play a sound and leave a message
	if ( flagmask & ENEMYHELI_KNOWNTOPLAYER )
	{
		CHAR16 pStr2[128];
		GetSectorIDString( SECTORX( sector_current ), SECTORY( sector_current ), 0, pStr2, FALSE );

		MapScreenMessage( FONT_MCOLOR_LTRED, MSG_INTERFACE, szEnemyHeliText[0], pStr2 );

		// if heli was shot down by a SAM, play a crashing sound
		if ( flagmask & ENEMYHELI_SHOTDOWN )
		{
			PlayJA2StreamingSampleFromFile( "stsounds\\blah2.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN, NULL );
		}
	}

	// if a helicopter was shot down, there might be survivors or wreckage
	if ( flagmask & ENEMYHELI_SHOTDOWN )
	{
		// remember that a heli crashed here. When entering the sector the next time, we might spawn a downed pilot or wreckage
		SectorInfo[sector_current].usSectorInfoFlag |= SECTORINFO_ENEMYHELI_SHOTDOWN;

		// if the heli had soldiers aboard, they might have survived the crash. Add them to the sector in that case
		// don't do this in sectors that are impassable
		if ( troopcount && !SectorIsImpassable( sector_current ) )
		{
			// it is possible that we shoot down a helicopter while in a fight. In this case, adding soldiers to a sector would either cause a second battle or a state where a fight should start but doesn't
			// both is bad, so we simply don't add soldiers in that case. The player will hardly notice, as there is only a chance that soldiers survive anyway
			if ( !gTacticalStatus.fEnemyInSector && !(gTacticalStatus.uiFlags & INCOMBAT) )
			{
				// chance that they survived at all
				if ( Chance( 75 ) )
				{
					// not all might have made it
					SectorInfo[sector_current].ubNumElites = min( 255, SectorInfo[sector_current].ubNumElites + 1 + Random(troopcount) );

					// if no militia or mercs are here, enemy takes this sector
					if ( PlayerMercsInSector( SECTORX( sector_current ), SECTORY( sector_current ), 0 ) + NumNonPlayerTeamMembersInSector( SECTORX( sector_current ), SECTORY( sector_current ), MILITIA_TEAM ) <= 0 )
					{
						SetThisSectorAsEnemyControlled( SECTORX( sector_current ), SECTORY( sector_current ), 0, TRUE );
					}

					CheckCombatInSectorDueToUnusualEnemyArrival( ENEMY_TEAM, SECTORX( sector_current ), SECTORY( sector_current ), 0 );
				}
			}
		}
	}

	// reduce these at the end, as we might do sth based on them
	fuel = 0;
	troopcount = 0;
}

void EnemyHeliInit( )
{
	gEnemyHeliVector.clear( );
}


void AddNewHeli( )
{
	while ( gASDResource[ASD_HELI] > 0 )
	{
		--gASDResource[ASD_HELI];

		BOOLEAN fHeliReplaced = FALSE;

		INT16 id = -1;

		// if a heli was destroyed, replace it
		std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
		for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
		{
			++id;

			if ( (*it).flagmask & ENEMYHELI_DESTROYED )
			{
				(*it).flagmask = 0;
				(*it).sector_home = SECTOR( gModSettings.usEnemyHeliBaseSectorX, gModSettings.usEnemyHeliBaseSectorY );
				(*it).sector_destination = 0;
				(*it).sector_waypoint = 0;
				(*it).sector_current = (*it).sector_home;
				(*it).troopcount = 0;
				(*it).hp = gGameExternalOptions.gEnemyHeliMaxHP;
				(*it).fuel = 0;

				UpdateEnemyHeli( id );

				fHeliReplaced = TRUE;

				break;
			}
		}

		// otherwise, create a new one
		if ( !fHeliReplaced )
		{
			ENEMY_HELI data;

			data.flagmask = 0;
			data.sector_home = SECTOR( gModSettings.usEnemyHeliBaseSectorX, gModSettings.usEnemyHeliBaseSectorY );
			data.sector_destination = 0;
			data.sector_waypoint = 0;
			data.sector_current = data.sector_home;
			data.troopcount = 0;
			data.hp = gGameExternalOptions.gEnemyHeliMaxHP;
			data.fuel = 0;

			gEnemyHeliVector.push_back( data );

			++id;

			UpdateEnemyHeli( id );
		}
	}
}

// all enemy helis carrying troops in this sector drop these into combat, and get the order to head home
void EnemyHeliTroopDrop( UINT8 aSector )
{
	INT16 id = -1;

	std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
	for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
	{
		++id;

		if ( (*it).sector_current != aSector )
			continue;

		if ( (*it).flagmask & ENEMYHELI_DESTROYED )
			continue;

		if ( !(*it).troopcount )
			continue;

		SectorInfo[aSector].ubNumElites = min( 255, SectorInfo[aSector].ubNumElites + (*it).troopcount );

		(*it).troopcount = 0;

		(*it).flagmask &= ~ENEMYHELI_ORDER_DROPTROOPS;

		UpdateEnemyHeli( id );
	}
}

std::set<UINT8> GetEnemyHeliSectors( BOOLEAN afKnownToPlayer )
{
	std::set<UINT8> set;

	std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
	for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
	{
		if ( (*it).flagmask & ENEMYHELI_DESTROYED )
			continue;

		// if the player doesn't know about this heli, don't add this
		if ( afKnownToPlayer && !((*it).flagmask & ENEMYHELI_KNOWNTOPLAYER) )
			continue;

		set.insert( (*it).sector_current );
	}

	return set;
}

void UpdateEnemyHeliRepair( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		// update
		if ( heli.flagmask & ENEMYHELI_REPAIRING )
		{
			heli.flagmask &= ~ENEMYHELI_REPAIRING;

			if ( heli.sector_current == heli.sector_home )
			{
				INT32 cost = (gGameExternalOptions.gEnemyHeliMaxHP - heli.hp) * gGameExternalOptions.gEnemyHeliCostPerRepair1HP;

				AddStrategicAIResources( ASD_MONEY, -cost );

				heli.hp = gGameExternalOptions.gEnemyHeliMaxHP;
			}
		}

		UpdateEnemyHeli( id );
	}
}

void UpdateEnemyHeliRefuel( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		// update
		if ( heli.flagmask & ENEMYHELI_REFUELING )
		{
			heli.flagmask &= ~ENEMYHELI_REFUELING;

			if ( heli.sector_current == heli.sector_home )
			{
				UINT8 addedfuel = min( gASDResource[ASD_FUEL], gGameExternalOptions.gEnemyHeliMaxFuel - heli.fuel );

				AddStrategicAIResources( ASD_FUEL, -addedfuel );

				heli.fuel += addedfuel;

				// if heli isn't full, try again
				if ( heli.fuel < gGameExternalOptions.gEnemyHeliMaxFuel )
				{
					heli.flagmask |= ENEMYHELI_REFUELING;

					AddStrategicEvent( EVENT_ENEMY_HELI_REFUEL, GetWorldTotalMin( ) + (gGameExternalOptions.gEnemyHeliMaxFuel - heli.fuel) * gGameExternalOptions.gEnemyHeliTimePerFuelRefuel, id );

					return;
				}
			}
		}

		UpdateEnemyHeli( id );
	}
}

void UpdateEnemyHeli( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		if ( heli.flagmask & ENEMYHELI_ORDER_GOTODESTINATION )
		{
			if ( heli.sector_current == heli.sector_waypoint )
				heli.sector_waypoint = heli.sector_destination;
			
			heli.sector_current = GetNextEnemyHeliSector( heli.sector_current, heli.sector_waypoint );

			if ( heli.fuel )
				--heli.fuel;
			else
			{
				// if the heli is in the air without fuel, it will crash!
				heli.flagmask |= ENEMYHELI_SHOTDOWN;

				heli.Destroy();

				return;
			}

			// whenever we arrive at a new sector, SAM batteries get a chance to shoot at a heli
			EnemyHeliCheckPlayerKnowledge( id );
			EnemyHeliSAMCheck( id );

			if ( heli.sector_current == heli.sector_destination )
			{
				heli.sector_waypoint = 0;
				heli.flagmask &= ~ENEMYHELI_ORDER_GOTODESTINATION;
			}
		}

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		// each time we update a heli, check whether we need repairs or refueling
		BOOLEAN needrepair = FALSE;
		BOOLEAN needrefuel = FALSE;
		if ( heli.hp < gGameExternalOptions.gEnemyHeliMaxHP )
		{
			needrepair = TRUE;

			if ( !heli.hp )
			{
				heli.Destroy( );

				return;
			}
		}

		if ( heli.fuel < gGameExternalOptions.gEnemyHeliMaxFuel )
		{
			needrefuel = TRUE;
		}

		// if we need repair or refueling and we are at the base, do that
		if ( needrepair || needrefuel )
		{
			if ( heli.sector_current == heli.sector_home )
			{
				if ( needrepair )
				{
					// determine how long this will take
					INT32 time = (gGameExternalOptions.gEnemyHeliMaxHP - heli.hp) * gGameExternalOptions.gEnemyHeliTimePerHPRepair;

					// are we able to fund this?
					if ( gGameExternalOptions.gEnemyHeliMaxHP - heli.hp * gGameExternalOptions.gEnemyHeliCostPerRepair1HP <= gASDResource[ASD_MONEY] )
					{
						heli.flagmask |= ENEMYHELI_REPAIRING;

						AddStrategicEvent( EVENT_ENEMY_HELI_REPAIR, GetWorldTotalMin( ) + time, id );

						return;
					}
				}

				if ( needrefuel )
				{
					// determine how long this will take
					INT32 time = (gGameExternalOptions.gEnemyHeliMaxFuel - heli.fuel) * gGameExternalOptions.gEnemyHeliTimePerFuelRefuel;

					// are we able to fund this?
					if ( gGameExternalOptions.gEnemyHeliMaxFuel - heli.fuel <= gASDResource[ASD_FUEL] )
					{
						heli.flagmask |= ENEMYHELI_REFUELING;

						AddStrategicEvent( EVENT_ENEMY_HELI_REFUEL, GetWorldTotalMin( ) + time, id );

						return;
					}
				}
			}
			else
			{
				// if we are not at the base and are not ordered to drop or pick up troops, head home
				if ( !(heli.flagmask & (ENEMYHELI_ORDER_GOTODESTINATION | ENEMYHELI_ORDER_DROPTROOPS | ENEMYHELI_ORDER_PICKUPTROOPS)) && heli.sector_current != heli.sector_home )
				{
					// we don't care about number of SAM contacts here - just get us home!
					heli.SetHeliFlightPath( heli.sector_home );
				}
			}
		}

		// if we still have to move, well, do so
		if ( heli.flagmask & (ENEMYHELI_ORDER_GOTODESTINATION) )
		{
			AddStrategicEvent( EVENT_ENEMY_HELI_UPDATE, GetWorldTotalMin( ) + gEnemyHeliTravelTimePerSector, id );

			return;
		}

		// if we have to drop troops, do so
		if ( heli.flagmask & ENEMYHELI_ORDER_DROPTROOPS && heli.sector_current == heli.sector_destination )
		{
			// for now, we park if there is an ongoing battle here
			if ( gTacticalStatus.fEnemyInSector || (gTacticalStatus.uiFlags & INCOMBAT) )
			{
				AddStrategicEvent( EVENT_ENEMY_HELI_UPDATE, GetWorldTotalMin( ) + gEnemyHeliTravelTimePerSector, id );

				return;
			}

			SectorInfo[heli.sector_current].ubNumElites = min( 255, SectorInfo[heli.sector_current].ubNumElites + heli.troopcount );

			heli.troopcount = 0;

			heli.flagmask &= ~ENEMYHELI_ORDER_DROPTROOPS;

			// if no militia or mercs are here, enemy takes this sector
			if ( PlayerMercsInSector( SECTORX( heli.sector_current ), SECTORY( heli.sector_current ), 0 ) + NumNonPlayerTeamMembersInSector( SECTORX( heli.sector_current ), SECTORY( heli.sector_current ), MILITIA_TEAM ) <= 0 )
			{
				SetThisSectorAsEnemyControlled( SECTORX( heli.sector_current ), SECTORY( heli.sector_current ), 0, TRUE );
			}

			CheckCombatInSectorDueToUnusualEnemyArrival( ENEMY_TEAM, SECTORX( heli.sector_current ), SECTORY( heli.sector_current ), 0 );

			// get new orders
			return UpdateEnemyHeli( id );
		}

		// if we have to pick up troops, do so
		if ( heli.flagmask & ENEMYHELI_ORDER_PICKUPTROOPS && heli.sector_current == heli.sector_destination )
		{
			// for now, not if the player is here
			if ( !PlayerMercsInSector( SECTORX( heli.sector_current ), SECTORY( heli.sector_current ), 0 ) )
			{
				if ( heli.troopcount < gEnemyHeliMaxTroops )
				{
					UINT16 possibletroops = SectorInfo[heli.sector_current].ubNumElites;

					UINT8 picked_up = min( possibletroops, gEnemyHeliMaxTroops - heli.troopcount );

					heli.troopcount += picked_up;

					SectorInfo[heli.sector_current].ubNumElites -= picked_up;

					heli.flagmask &= ~ENEMYHELI_ORDER_PICKUPTROOPS;

					// get new orders
					return UpdateEnemyHeli( id );
				}
			}
		}
	}
}

void EnemyHeliSAMCheck( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		INT8 heli_x = SECTORX( heli.sector_current );
		INT8 heli_y = SECTORY( heli.sector_current );

		// is the airspace in our current sector player-controlled and operational?
		if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( heli_x, heli_y )].usAirType & AIRSPACE_PLAYER_ACTIVE )
		{
			for ( int i = 0; i < NUMBER_OF_SAMS; ++i )
			{
				BOOLEAN samworking = FALSE;
				if ( DoesSamCoverSector( i, heli.sector_current, &samworking ) && samworking )
				{
					StrategicMapElement *pSAMStrategicMap = &(StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[i] )]);

					// also check whether this sector is not enemy-controlled - we can't have the AI shooting at its own helis
					if ( pSAMStrategicMap && !pSAMStrategicMap->fEnemyControlled )
					{
						UINT16 ubBestSAMOperatorID = NOBODY;
						FLOAT samcth = GetBestSAMOperatorCTH_Player( gpSamSectorX[i], gpSamSectorY[i], 0, &ubBestSAMOperatorID );

						// cth is reduced if SAM is damaged, even if it can still operate
						samcth = (samcth * pSAMStrategicMap->bSAMCondition) / 100.0f;

						// determine distance from SAM to heli
						FLOAT distance = sqrt( FLOAT( abs( heli_x - gpSamSectorX[i] ) * abs( heli_x - gpSamSectorX[i] ) + abs( heli_y - gpSamSectorY[i] ) * abs( heli_y - gpSamSectorY[i] ) ) );

						// distance penalty
						samcth = (samcth * (100.0f - 8 * distance)) / 100.0f;

						// if cth is too low, we don't fire
						if ( samcth > 0.01 )
						{
							CHAR16 pStrSectorName[128];
							GetSectorIDString( heli_x, heli_y, 0, pStrSectorName, FALSE );

							CHAR16 pStrSectorName_SAM[128];
							GetSectorIDString( gpSamSectorX[i], gpSamSectorY[i], 0, pStrSectorName_SAM, FALSE );

							MapScreenMessage( FONT_MCOLOR_LTRED, MSG_INTERFACE, szEnemyHeliText[8], pStrSectorName_SAM, pStrSectorName );

							// if we hit, damage the heli
							BOOLEAN fHit = FALSE;
							BOOLEAN fDestroyed = FALSE;

							if ( Chance( (UINT32)(samcth) ) )
							{
								fHit = TRUE;

								UINT8 damage = gGameExternalOptions.gEnemyHeliSAMDamage_Base + Random( gGameExternalOptions.gEnemyHeliSAMDamage_Var );

								MapScreenMessage( FONT_MCOLOR_LTRED, MSG_INTERFACE, szEnemyHeliText[6], pStrSectorName );

								heli.hp = max( 0, heli.hp - damage );

								if ( !heli.hp )
								{
									fDestroyed = TRUE;

									heli.flagmask |= ENEMYHELI_SHOTDOWN;

									heli.Destroy( );
								}
							}

							// depending on our result, award experience to the operator
							if ( ubBestSAMOperatorID != NOBODY )
							{
								if ( fHit )
									StatChange( MercPtrs[ubBestSAMOperatorID], EXPERAMT, fDestroyed ? 30 : 10, TRUE );

								StatChange( MercPtrs[ubBestSAMOperatorID], MECHANAMT, 5, TRUE );
							}
						}
					}
				}
			}
		}

		EnemyHeliMANPADSCheck( id );
	}
}

void EnemyHeliMANPADSCheck( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		// if the heli is already destroyed, nothing to do here
		if ( heli.flagmask & ENEMYHELI_DESTROYED )
			return;

		// if the player doesn't know about this heli, he can't shoot it down
		if ( !(heli.flagmask & ENEMYHELI_KNOWNTOPLAYER) )
			return;

		INT8 heli_x = SECTORX( heli.sector_current );
		INT8 heli_y = SECTORY( heli.sector_current );

		CHAR16 pStrSectorName[128];
		GetSectorIDString( heli_x, heli_y, 0, pStrSectorName, FALSE );

		// each merc can fire one missile at the heli (break if heli is shot down)
		// this requires an armed SAM in the first place. The mercs have to be in the heli's sector or adjacent to it
		// sleeping or travelling mercs are excluded
		UINT16 uiCnt = 0;
		SOLDIERTYPE* pSoldier = NULL;

		for ( uiCnt = 0, pSoldier = MercPtrs[uiCnt]; uiCnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++uiCnt, ++pSoldier )
		{
			if ( pSoldier && pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && !pSoldier->bSectorZ &&
				 (pSoldier->sSectorX == heli_x && pSoldier->sSectorY == heli_y) ||
				 (pSoldier->sSectorX == heli_x + 1 && pSoldier->sSectorY == heli_y) ||
				 (pSoldier->sSectorX == heli_x - 1 && pSoldier->sSectorY == heli_y) ||
				 (pSoldier->sSectorX == heli_x && pSoldier->sSectorY == heli_y + 1) ||
				 (pSoldier->sSectorX == heli_x && pSoldier->sSectorY == heli_y - 1)
				 )
			{
				// look for a SAM in our inventory
				OBJECTTYPE* pObj = pSoldier->GetObjectWithFlag( MANPAD );

				if ( pObj )
				{
					// abort if this is a launcher without ammo
					if ( Item[pObj->usItem].rocketlauncher && !Item[pObj->usItem].singleshotrocketlauncher )
					{
						OBJECTTYPE* pAttachment = FindAttachmentByClass( pObj, IC_GRENADE );
						if ( !pAttachment->exists( ) )
							pAttachment = FindAttachmentByClass( pObj, IC_BOMB );

						if ( !pAttachment->exists( ) )
							continue;
					}

					// determine cth
					INT16 samcth = pSoldier->stats.bMarksmanship;

					samcth = (FLOAT)((samcth * (100.0f + pSoldier->GetTraitCTHModifier( pObj->usItem, 2, NO_PROFILE ))) / 100.0f);

					samcth = (samcth * (100.0f + pSoldier->GetBackgroundValue( BG_PERC_SAM_CTH ))) / 100.0f;

					// cth is reduced if SAM is damaged, even if it can still operate
					samcth = (samcth * (*pObj)[0]->data.objectStatus) / 100.0f;

					// determine distance from SAM to heli
					FLOAT distance = sqrt(FLOAT(abs( heli_x - pSoldier->sSectorX ) * abs( heli_x - pSoldier->sSectorX ) + abs( heli_y - pSoldier->sSectorY ) * abs( heli_y - pSoldier->sSectorY ) ));
					
					// distance penalty
					samcth = (samcth * (100.0f - 20 * distance)) / 100.0f;

					MapScreenMessage( FONT_MCOLOR_LTRED, MSG_INTERFACE, szEnemyHeliText[7], pSoldier->GetName( ), Item[pObj->usItem].szItemName, pStrSectorName );

					// 'fire' (remove shot)
					if ( Item[pObj->usItem].singleshotrocketlauncher )
					{
						CreateItem( Item[pObj->usItem].discardedlauncheritem, (*pObj)[0]->data.objectStatus, pObj );

						DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
					}
					else
					{
						DeductAmmo( pSoldier, pObj );
					}

					// if we hit, damage the heli
					BOOLEAN fHit = FALSE;
					BOOLEAN fDestroyed = FALSE;

					if ( Chance( (UINT32)(samcth) ) )
					{
						fHit = TRUE;

						UINT8 damage = gGameExternalOptions.gEnemyHeliMANPADSDamage_Base + Random( gGameExternalOptions.gEnemyHeliMANPADSDamage_Var );

						MapScreenMessage( FONT_MCOLOR_LTRED, MSG_INTERFACE, szEnemyHeliText[6], pStrSectorName );

						heli.hp = max( 0, heli.hp - damage );

						if ( !heli.hp )
						{
							fDestroyed = TRUE;

							heli.flagmask |= ENEMYHELI_SHOTDOWN;

							heli.Destroy( );
						}
					}

					// depending on our result, award experience to the operator
					if ( fHit )
						StatChange( pSoldier, EXPERAMT, fDestroyed ? 30 : 10, TRUE );

					StatChange( pSoldier, MARKAMT, 5, TRUE );
				}
			}
		}
	}
}

void EnemyHeliCheckPlayerKnowledge( INT16 id )
{
	if ( id >= 0 && id < gEnemyHeliVector.size( ) )
	{
		ENEMY_HELI& heli = gEnemyHeliVector[id];

		INT8 current_x = SECTORX( heli.sector_current );
		INT8 current_y = SECTORY( heli.sector_current );

		// if there are mercs in this sector, they spot the heli and let us know
		if ( PlayerMercsInSector( current_x, current_y, 0 ) )
		{
			// if we previously did not know of the heli, our mercs will alert us
			if ( !(heli.flagmask & ENEMYHELI_KNOWNTOPLAYER) )
			{
				SayQuoteFromAnyBodyInThisSector( current_x, current_y, 0, QUOTE_AIR_RAID );
			}

			heli.flagmask |= ENEMYHELI_KNOWNTOPLAYER;

			return;
		}

		// scouts our militia might be able to detect a heli from nearby sectors
		if ( CanSomeoneNearbyScoutThisSector( current_x, current_y, TRUE ) ) // scouting trait check
		{
			if ( gSkillTraitValues.fSCCanDetectEnemyPresenseAround )
			{
				heli.flagmask |= ENEMYHELI_KNOWNTOPLAYER;

				return;
			}
		}

		// is the airspace in our current sector player-controlled and operational?
		if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( current_x, current_y )].usAirType & AIRSPACE_PLAYER_ACTIVE )
		{
			for ( int i = 0; i < NUMBER_OF_SAMS; ++i )
			{
				if ( DoesSamCoverSector( i, heli.sector_current ) )
				{
					// if mercs man the SAM, they will let us know of the new SAM
					if ( PlayerMercsInSector( gpSamSectorX[i], gpSamSectorY[i], 0 ) )
					{
						// if we previously did not know of the heli, our mercs will alert us
						if ( !(heli.flagmask & ENEMYHELI_KNOWNTOPLAYER) )
						{
							SayQuoteFromAnyBodyInThisSector( gpSamSectorX[i], gpSamSectorY[i], 0, QUOTE_AIR_RAID );

							break;
						}
					}
				}
			}

			heli.flagmask |= ENEMYHELI_KNOWNTOPLAYER;

			return;
		}

		// if we haven't exited this function already, nobody notices the heli - delete knowledge
		heli.flagmask &= ~ENEMYHELI_KNOWNTOPLAYER;
	}
}

void RepairSamSite( UINT8 aSector )
{
	if ( IsThisSectorASAMSector( SECTORX( aSector ), SECTORY( aSector ), 0 ) )
	{
		StrategicMap[aSector].bSAMCondition = 100;
		StrategicMap[aSector].usFlags &= ~SAMSITE_REPAIR_ORDERED;

		UpdateSAMDoneRepair( SECTORX( aSector ), SECTORY( aSector ), 0 );
	}
}

UINT8 NumPlayerAirSpaceOnHeliPath( UINT8 aStart, UINT8 aEnd )
{
	UINT8 samcontacts = 0;

	UINT8 tmpsect = aStart;

	while ( tmpsect != aEnd )
	{
		tmpsect = GetNextEnemyHeliSector( tmpsect, aEnd );

		if ( StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( tmpsect )].usAirType & AIRSPACE_PLAYER_ACTIVE )
			++samcontacts;
	}

	return samcontacts;
}

// get the next sector in an enemy heli flight path.
UINT8 GetNextEnemyHeliSector( UINT8 aStart, UINT8 aDest )
{
	INT8 start_x = SECTORX( aStart );
	INT8 start_y = SECTORY( aStart );

	INT8 dest_x = SECTORX( aDest );
	INT8 dest_y = SECTORY( aDest );

	if ( abs( start_x - dest_x ) > abs( start_y - dest_y ) )
	{
		if ( start_x > dest_x )
			return SECTOR( start_x - 1, start_y );
		else
			return SECTOR( start_x + 1, start_y );
	}
	else
	{
		if ( start_y > dest_y )
			return SECTOR( start_x, start_y - 1 );
		else
			return SECTOR( start_x, start_y + 1 );
	}

	return aStart;
}


void HandleEnemyHelicopterOnGroundGraphic( )
{
	// no worries if underground
	if ( gbWorldSectorZ )
		return;

	UINT8 currentsector = SECTOR( gWorldSectorX, gWorldSectorY );

	// only relevant if this is the enemies' heli hub
	if ( currentsector != SECTOR( gModSettings.usEnemyHeliBaseSectorX, gModSettings.usEnemyHeliBaseSectorY ) )
		return;

	INT16 id = -1;

	std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
	for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
	{
		++id;

		// we can only provide graphics for ENEMYHELI_MAX helis
		if ( id >= ENEMYHELI_MAX )
			break;

		AddEnemyHelicopterToMaps( ((*it).sector_current == currentsector), ((*it).flagmask & ENEMYHELI_DESTROYED), gModSettings.sEnemyHeliBaseParkGridno[id], gModSettings.sEnemyHeliBaseParkTileIndex );
	}

	// Invalidate rendering (if we have any helis at all)
	if ( id )
		InvalidateWorldRedundency( );
}

void UpdateAndDamageEnemyHeliIfFound( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo, UINT8 ubDamage, BOOLEAN aDestroyed )
{
	// no worries if underground
	if ( gbWorldSectorZ )
		return;

	UINT8 currentsector = SECTOR( gWorldSectorX, gWorldSectorY );

	// only relevant if this is the enemies' heli hub
	if ( currentsector != SECTOR( gModSettings.usEnemyHeliBaseSectorX, gModSettings.usEnemyHeliBaseSectorY ) )
		return;

	// helicopter tiles are multi-tiled. Search for the base gridno
	STRUCTURE			*pCurrent, *pBaseStructure;
	INT32				sBaseGridNo = NOWHERE;

	pCurrent = gpWorldLevelData[sGridNo].pStructureHead;

	while ( pCurrent != NULL )
	{
		pBaseStructure = FindBaseStructure( pCurrent );

		if ( pBaseStructure && pBaseStructure->ubHitPoints )
		{
			sBaseGridNo = pBaseStructure->sGridNo;

			break;
		}

		pCurrent = pCurrent->pNext;
	}

	if ( sBaseGridNo == NOWHERE )
		return;

	INT16 id = -1;

	std::vector<ENEMY_HELI>::iterator itend = gEnemyHeliVector.end( );
	for ( std::vector<ENEMY_HELI>::iterator it = gEnemyHeliVector.begin( ); it != itend; ++it )
	{
		++id;

		// we can only provide graphics for ENEMYHELI_MAX helis
		if ( id >= ENEMYHELI_MAX )
			continue;

		// if the heli is already destroyed, nothing to do here
		if ( (*it).flagmask & ENEMYHELI_DESTROYED )
			continue;

		if ( (*it).sector_current == currentsector )
		{
			if ( gModSettings.sEnemyHeliBaseParkGridno[id] == sBaseGridNo )
			{
				if ( aDestroyed )
				{
					(*it).Destroy( );
				}
				// if the structure is not destroyed, the heli stays
				else
				{
					(*it).hp = max( 1, (*it).hp - ubDamage );
				}
			}
		}
	}
}

//////////////////////////////////////////// enemy tanks /////////////////////////////////////////////////////////
UINT32 ASDResourceCostFuel( UINT8 aType )
{
	switch ( aType )
	{
	case ASD_TANK:
		return gGameExternalOptions.gASDResource_Fuel_Tank;
		break;

	case ASD_JEEP:
		return gGameExternalOptions.gASDResource_Fuel_Jeep;
		break;
	}

	return 0;
}

// if ASD has tanks, it can allow the queen to upgrade soldiers to tanks
BOOLEAN ASDSoldierUpgradeToTank( )
{
	if ( gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsTanks )
	{
		if ( gASD_Flags & ASDFACT_TANK_UNLOCKED )
		{
			if ( gASDResource[ASD_TANK] )
			{
				gASDResource[ASD_TANK] = max( 0, gASDResource[ASD_TANK] - 1 );
				return TRUE;
			}
		}
	}
	else
	{
		gASDResource[ASD_TANK] = max( 0, gASDResource[ASD_TANK] - 1 );
		return TRUE;
	}

	return FALSE;
}

BOOLEAN ASDSoldierUpgradeToJeep( )
{
	if ( gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsJeeps )
	{
		if ( gASD_Flags & ASDFACT_JEEP_UNLOCKED )
		{
			if ( gASDResource[ASD_JEEP] )
			{
				gASDResource[ASD_JEEP] = max( 0, gASDResource[ASD_JEEP] - 1 );
				return TRUE;
			}
		}
	}
	else
	{
		gASDResource[ASD_JEEP] = max( 0, gASDResource[ASD_JEEP] - 1 );
		return TRUE;
	}

	return FALSE;
}
