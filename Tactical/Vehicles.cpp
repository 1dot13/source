	#include "Vehicles.h"
	#include "String.h"
	#include "Strategic Pathing.h"
	#include "Assignments.h"
	#include "Strategic Movement.h"
	#include "Squads.h"
	#include "Map Screen Helicopter.h"
	#include "Game Clock.h"
	#include "overhead.h"
	#include "soldier profile.h"
	#include "Sound Control.h"
	#include "soundman.h"
	#include "soldier add.h"
	#include "interface panels.h"
	#include "strategic.h"
	#include "worlddef.h"
	#include "Isometric Utils.h"
	#include "Mapscreen.h"
	#include "message.h"
	#include "interface.h"
	#include "Random.h"
	#include "text.h"
	#include "explosion control.h"
	#include "soldier create.h"
	#include "animation control.h"
	#include "strategicmap.h"
	#include "Interface Control.h"
	#include "Campaign Types.h"
	#include "Map Screen Interface.h"
	#include "jascreens.h"
	#include "screenids.h"
	#include "Quests.h"
	#include "Tactical Save.h"
	#include "Soldier macros.h"
	#include "opplist.h"
	#include "Soldier ani.h"
	#include "GameSettings.h"
	#include "Queen Command.h"

#include "Points.h"
#include "Init.h"

//INT8 gubVehicleMovementGroups[ MAX_VEHICLES ];
std::vector<INT8>	gubVehicleMovementGroups (MAX_VEHICLES, 0);

// the list of vehicles
VEHICLETYPE *pVehicleList = NULL;

// number of vehicle slots on the list
UINT8 ubNumberOfVehicles = 0;

// the sqaud mvt groups
extern INT8 SquadMovementGroups[ ];


NEW_CAR gNewVehicle[NUM_PROFILES];

// Flugente 2013-05-12: saving and loading an array that is read from xml is utterly pointless. The loading function has to remain for compatibility reasons - please remove the next time savegame compatibility is broken
BOOLEAN LoadNewVehiclesToSaveGameFile( HWFILE hFile );

//ATE: These arrays below should all be in a large LUT which contains
// static info for each vehicle....

// the mvt groups associated with vehcile types
/*
INT32 iMvtTypes[]={
	CAR,	// eldorado
	CAR,	// hummer
	CAR,	// ice cream truck
	CAR,	// jeep
	CAR,	// tank

	AIR,	// helicopter

};

INT32 iSeatingCapacities[]={
	6, // eldorado
	6, // hummer
	6, // ice cream truck
	6, // jeep
	6, // tank
	6, // helicopter
};


INT32	iEnterVehicleSndID[] = {

	S_VECH1_INTO,
	S_VECH1_INTO,
	S_VECH1_INTO,
	S_VECH1_INTO,
	S_VECH1_INTO,
	S_VECH1_INTO,

};

INT32	iMoveVehicleSndID[] = {

	S_VECH1_MOVE,
	S_VECH1_MOVE,
	S_VECH1_MOVE,
	S_VECH1_MOVE,
	S_VECH1_MOVE,
	S_VECH1_MOVE,
};

UINT8 ubVehicleTypeProfileID[ ] = {

	PROF_ELDERODO,
	PROF_HUMMER,
	PROF_ICECREAM,
	NPC164,
	NPC164,
	PROF_HELICOPTER
};

*/

/*
// location of crits based on facing
INT8 bInternalCritHitsByLocation[ NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE ][ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE ]={
	{ ENGINE_HIT_LOCATION, ENGINE_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION,CREW_COMPARTMENT_HIT_LOCATION, RF_TIRE_HIT_LOCATION, LF_TIRE_HIT_LOCATION }, // front
	{ ENGINE_HIT_LOCATION, LF_TIRE_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, LR_TIRE_HIT_LOCATION, GAS_TANK_HIT_LOCATION}, // left side
	{ ENGINE_HIT_LOCATION, RF_TIRE_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, RR_TIRE_HIT_LOCATION, GAS_TANK_HIT_LOCATION}, // right side
	{ CREW_COMPARTMENT_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, RR_TIRE_HIT_LOCATION, LR_TIRE_HIT_LOCATION, GAS_TANK_HIT_LOCATION }, // rear
	{ ENGINE_HIT_LOCATION, RF_TIRE_HIT_LOCATION, LF_TIRE_HIT_LOCATION, RR_TIRE_HIT_LOCATION,LR_TIRE_HIT_LOCATION, GAS_TANK_HIT_LOCATION,}, // bottom side
	{ ENGINE_HIT_LOCATION, ENGINE_HIT_LOCATION, ENGINE_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, CREW_COMPARTMENT_HIT_LOCATION, GAS_TANK_HIT_LOCATION }, // top
};
*/

// original armor values for vehicles
/*
	ELDORADO_CAR = 0,
	HUMMER,
	ICE_CREAM_TRUCK,
	JEEP_CAR,
	TANK_CAR,
	HELICOPTER,
*/
/*
INT16 sVehicleArmourType[ NUMBER_OF_TYPES_OF_VEHICLES ] =
{
	KEVLAR_VEST,			// El Dorado
	SPECTRA_VEST,			// Hummer
	KEVLAR_VEST,			// Ice cream truck
	KEVLAR_VEST,			// Jeep
	SPECTRA_VEST,			// Tank - do we want this?
	KEVLAR_VEST,			// Helicopter
};
*/

/*
INT16 sVehicleExternalOrigArmorValues[ NUMBER_OF_TYPES_OF_VEHICLES ][ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE ]={
	{ 100,100,100,100,100,100 }, // helicopter
	{ 500,500,500,500,500,500 }, // hummer
};
*/

/*
// external armor values
INT16 sVehicleInternalOrigArmorValues[ NUMBER_OF_TYPES_OF_VEHICLES ][ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE ]={
	{ 250,250,250,250,250,250 }, // eldorado
	{ 250,250,250,250,250,250 }, // hummer
	{ 250,250,250,250,250,250 }, // ice cream
	{ 250,250,250,250,250,250 }, // feep
	{ 850,850,850,850,850,850 }, // tank
	{ 50,50,50,50,50,50 }, // helicopter
};
*/

// ap cost per crit
#define COST_PER_ENGINE_CRIT 15
#define COST_PER_TIRE_HIT 5
//#define VEHICLE_MAX_INTERNAL 250


// set the driver of the vehicle
void SetDriver( INT32 iID, UINT8 ubID );

//void RemoveSoldierFromVehicleBetweenSectors( pSoldier, iId );

void TeleportVehicleToItsClosestSector( INT32 iVehicleId, UINT8 ubGroupID );

// Flugente 2013-05-12: saving and loading an array that is read from xml is utterly pointless. The loading function has to remain for compatibility reasons - please remove the next time savegame compatibility is broken
BOOLEAN LoadNewVehiclesToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	NEW_CAR tmp[NUM_PROFILES];

	FileRead( hFile, &tmp, sizeof( tmp), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( tmp ) )
	{
		return( FALSE );
	}
	return( TRUE );
}

void InitAVehicle(int index, int x, int y)
{
	gubVehicleMovementGroups[ index ] = CreateNewVehicleGroupDepartingFromSector( x, y, index );

	// Set persistent....
	GROUP *pGroup = GetGroup( gubVehicleMovementGroups[ index ] );
	AssertNotNIL(pGroup);
	pGroup->fPersistant = TRUE;
}


// Loop through and create a few soldier squad ID's for vehicles
void InitAllVehicles( )
{
	for( int cnt = 0; cnt <	MAX_VEHICLES; ++cnt )
		InitAVehicle(cnt, 1, 1);
}


void SetVehicleValuesIntoSoldierType( SOLDIERTYPE *pVehicle )
{
	//wcscpy( pVehicle->name, zVehicleName[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ] );
	//wcscpy( pVehicle->name, gNewVehicle[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ].NewVehicleName );
	wcsncpy( pVehicle->name, gNewVehicle[pVehicleList[pVehicle->bVehicleID].ubVehicleType].NewVehicleName, (sizeof(pVehicle->name) / sizeof(pVehicle->name[0]) - 1) );

	pVehicle->ubProfile = pVehicleList[ pVehicle->bVehicleID ].ubProfileID;

	// Init fuel!
	pVehicle->sBreathRed = 10000;
	pVehicle->bBreath	= 100;

	pVehicle->ubWhatKindOfMercAmI = MERC_TYPE__VEHICLE;
}

INT32 AddVehicleToList( INT16 sMapX, INT16 sMapY, INT32 sGridNo, UINT8 ubType )
{
	// insert this vehicle into the list
	// how many vehicles are there?
	INT32 iVehicleIdValue = -1;
	INT32 iCounter = 0, iCount = 0;
	VEHICLETYPE *pTempList = NULL;
	BOOLEAN fFoundEmpty = FALSE;
	GROUP *pGroup;

	if( pVehicleList != NULL )
	{
		// not the first, add to list
		for( iCounter = 0; iCounter < ubNumberOfVehicles ; ++iCounter )
		{
			// might have an empty slot
			if( pVehicleList[ iCounter ].fValid == FALSE )
			{
				iCount = iCounter;
				iCounter = ubNumberOfVehicles;
				fFoundEmpty = TRUE;
				iVehicleIdValue = iCount;
			}
		}
	}

	if( fFoundEmpty == FALSE )
	{
		iCount = ubNumberOfVehicles;
	}

	if( iCount == 0 )
	{
		pVehicleList = (VEHICLETYPE *) MemAlloc( sizeof( VEHICLETYPE ) );

		// Set!
		memset( pVehicleList, 0, sizeof( VEHICLETYPE ) );

		ubNumberOfVehicles = 1;
		iVehicleIdValue		= 0;
	}
	
	if( ( iVehicleIdValue == -1 ) && ( iCount != 0 ) && ( fFoundEmpty == FALSE ) )
	{

		// no empty slot found, need to realloc
		pTempList = (VEHICLETYPE *) MemAlloc( sizeof( VEHICLETYPE ) * ubNumberOfVehicles );

		// copy to temp
		memcpy( pTempList, pVehicleList, sizeof( VEHICLETYPE ) * ubNumberOfVehicles );

		// now realloc
		pVehicleList = (VEHICLETYPE *) MemRealloc( pVehicleList, ( sizeof( VEHICLETYPE ) * ( ubNumberOfVehicles + 1 ) ) );

		// memset the stuff
		memset( pVehicleList, 0, ( sizeof( VEHICLETYPE ) * ( ubNumberOfVehicles + 1 ) ) );

		// now copy the stuff back
		memcpy( pVehicleList, pTempList, sizeof( VEHICLETYPE ) * ( ubNumberOfVehicles ) );

		// now get rid of crap
		MemFree( pTempList );

		// now get the index value
		iVehicleIdValue = ubNumberOfVehicles;

		++ubNumberOfVehicles;
	}

	// found a slot
	pVehicleList[ iCount ].ubMovementGroup = 0;
	pVehicleList[ iCount ].sSectorX = sMapX;
	pVehicleList[ iCount ].sSectorY = sMapY;
	pVehicleList[ iCount ].sSectorZ = 0;
	pVehicleList[ iCount ].sGridNo = sGridNo;
	memset( pVehicleList[ iCount ].pPassengers, 0, 10 * sizeof( SOLDIERTYPE * ) );
	pVehicleList[ iCount ].fValid = TRUE;
	pVehicleList[ iCount ].ubVehicleType = ubType;
	pVehicleList[ iCount ].pMercPath = NULL;
	pVehicleList[ iCount ].fFunctional = TRUE;
	pVehicleList[ iCount ].fDestroyed = FALSE;
	pVehicleList[ iCount ].iMoveSound			= gNewVehicle[ ubType ].iNewMoveVehicleSndID;
	pVehicleList[ iCount ].iOutOfSound			= gNewVehicle[ ubType ].iNewEnterVehicleSndID;
	pVehicleList[ iCount ].ubProfileID			= ubType; //gNewVehicle[ ubType ].ubNewVehicleTypeProfileID;//gNewVehicle[ ubType ].uiIndex;  //gNewVehicle[ ubType ].ubNewVehicleTypeProfileID;  //
	pVehicleList[ iCount ].ubMovementGroup	= gubVehicleMovementGroups[ iCount ];
	pVehicleList[ iCount ].ubDriver = NOBODY;

	// ATE: Add movement mask to group...
	pGroup = GetGroup( pVehicleList[ iCount ].ubMovementGroup );

	if( !pGroup )
	{
		// WANNE: Commented the following line, because we got a CTD when skyrider sees his heli in drassen!
		//if( gfEditMode )
		{
			//This is okay, no groups exist, so simply return.
			return iVehicleIdValue;
		}
		Assert( 0 );
	}

	// HEADROCK HAM 3.1: An INI setting allows us to turn the Hummer into a true offroad vehicle. It will use the
	// "TRUCK" type movement, which allows it to go into mild non-road terrain. I wish I could come with a more
	// subtle method than this crude override, but this is what I've got at the moment.
	if (gGameExternalOptions.fHumveeOffroad && ubType == HUMMER)
	{
		pGroup->ubTransportationMask = TRUCK;
	}
	else
	{	
		// WANNE: This FIXES the bug (== instead of =), that the heli could only fly on roads!!
		if ( gNewVehicle[ ubType ].iNewMvtTypes == 0 )
			pGroup->ubTransportationMask = FOOT;
		else if ( gNewVehicle[ ubType ].iNewMvtTypes == 1 )
			pGroup->ubTransportationMask = CAR;	
		else if ( gNewVehicle[ ubType ].iNewMvtTypes == 2 )
			pGroup->ubTransportationMask = TRUCK;	
		else if ( gNewVehicle[ ubType ].iNewMvtTypes == 3 )
			pGroup->ubTransportationMask = TRACKED;			
		else if ( gNewVehicle[ ubType ].iNewMvtTypes == 4 )
			pGroup->ubTransportationMask = AIR;				
		else 
			pGroup->ubTransportationMask = CAR;
	}

	// ARM: setup group movement defaults
	pGroup->ubSectorX = ( UINT8 ) sMapX;
	pGroup->ubNextX	= ( UINT8 ) sMapX;
	pGroup->ubSectorY = ( UINT8 ) sMapY;
	pGroup->ubNextY	= ( UINT8 ) sMapY;
	pGroup->uiTraverseTime = 0;
	pGroup->uiArrivalTime	= 0;

	SetUpArmorForVehicle( ( UINT8 )iCount );

	return( iVehicleIdValue );
}


BOOLEAN RemoveVehicleFromList( INT32 iId )
{
	// remove this vehicle from the list

	// error check
	if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
	{
		return ( FALSE );
	}

	// clear remaining path nodes
	if( pVehicleList[ iId ].pMercPath != NULL )
	{
		pVehicleList[ iId ].pMercPath = ClearStrategicPathList( pVehicleList[ iId ].pMercPath, 0 );
	}

	// zero out mem
	memset( &( pVehicleList[ iId ] ), 0, sizeof( VEHICLETYPE ) );

	return( TRUE );
}

void ClearOutVehicleList( void )
{
	INT32	iCounter;

	// empty out the vehicle list
	if( pVehicleList )
	{
		for( iCounter = 0; iCounter < ubNumberOfVehicles ; iCounter++ )
		{
			// if there is a valid vehicle
			if( pVehicleList[ iCounter ].fValid )
			{
				//if the vehicle has a valid path
				if( pVehicleList[ iCounter ].pMercPath )
				{
					//toast the vehicle path
					pVehicleList[ iCounter ].pMercPath = ClearStrategicPathList( pVehicleList[ iCounter ].pMercPath, 0 );
				}
			}
		}

		MemFree( pVehicleList );
		pVehicleList = NULL;
		ubNumberOfVehicles = 0;
	}

/*
	// empty out the vehicle list
	if( pVehicleList )
	{
		MemFree( pVehicleList );
		pVehicleList = NULL;
		ubNumberOfVehicles = 0;
	}
*/
}

BOOLEAN IsThisVehicleAccessibleToSoldier( SOLDIERTYPE *pSoldier, INT32 iId )
{
	if( pSoldier == NULL )
	{
		return( FALSE );
	}

	if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
	{
		return ( FALSE );
	}

	// now check if vehicle is valid
	if( pVehicleList[ iId ].fValid == FALSE || pVehicleList[iId].fDestroyed == TRUE)
	{
		return( FALSE );
	}

	// if the soldier or the vehicle is between sectors
	if( pSoldier->flags.fBetweenSectors || pVehicleList[ iId ].fBetweenSectors )
	{
		return( FALSE );
	}

	// any sector values off?
	if( ( pSoldier->sSectorX != pVehicleList[ iId ].sSectorX ) ||
			( pSoldier->sSectorY != pVehicleList[ iId ].sSectorY ) ||
			( pSoldier->bSectorZ != pVehicleList[ iId ].sSectorZ ) )
	{
		return( FALSE );
	}

	// if vehicle is not ok to use then return false
	if ( !OKUseVehicle( pVehicleList[ iId ].ubProfileID ) )
	{
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN AddSoldierToVehicle( SOLDIERTYPE *pSoldier, INT32 iId, UINT8 ubSeatIndex )
{
	INT32 iCounter = 0;
	UINT32 vCount = 0;
	UINT8 ubFinalSeatIndex = 0;
	BOOLEAN fFoundPlace = FALSE;
	SOLDIERTYPE *pVehicleSoldier = NULL;


	// Add Soldierto Vehicle
	if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
	{
		return ( FALSE );
	}

	// ok now check if any free slots in the vehicle

	// now check if vehicle is valid
	if( pVehicleList[ iId ].fValid == FALSE )
	{
		return( FALSE );
	}

	// get the vehicle soldiertype
	pVehicleSoldier = GetSoldierStructureForVehicle( iId );

	//CHRISL: Get number of vehicles currently in player team
	for(int x = 0; x < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; x++)
	{
		if(MercPtrs[x]->bTeam == OUR_TEAM && (MercPtrs[x]->flags.uiStatusFlags & SOLDIER_VEHICLE) && MercPtrs[x]->bActive == TRUE)
			vCount ++;
	}

	if( pVehicleSoldier )
	{
		if ( pVehicleSoldier->bTeam != pSoldier->bTeam )
		{
			// Can we add a new vehicle
			if( vCount >= gGameExternalOptions.ubGameMaximumNumberOfPlayerVehicles && pSoldier->bTeam == gbPlayerNum )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[VEHICLE_CAN_NOT_BE_ADDED] );
				return( FALSE );
			}

			// Change sides...
			pVehicleSoldier = ChangeSoldierTeam( pVehicleSoldier, pSoldier->bTeam );
			// add it to mapscreen list
			fReBuildCharacterList = TRUE;
		}
	}

	// If vehicle is empty, add to unique squad now that it has somebody in it!
	if ( GetNumberInVehicle( iId ) == 0 && pVehicleSoldier && pSoldier->bTeam == gbPlayerNum )
	{
		// 2 ) Add to unique squad...
		AddCharacterToUniqueSquad( pVehicleSoldier );

		// ATE: OK funcky stuff here!
		// We have now a guy on a squad group, remove him!
		RemovePlayerFromGroup( SquadMovementGroups[ pVehicleSoldier->bAssignment ], pVehicleSoldier	);

		// I really have vehicles.\
		// ONLY add to vehicle group once!
		if ( !DoesPlayerExistInPGroup( pVehicleList[ iId ].ubMovementGroup, pVehicleSoldier ) )
		{
			//NOW.. add guy to vehicle group....
			AddPlayerToGroup( pVehicleList[ iId ].ubMovementGroup, pVehicleSoldier	);
		}
		else
		{
			pVehicleSoldier->ubGroupID = pVehicleList[ iId ].ubMovementGroup;
		}
	}

	// check if the grunt is already here
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] == pSoldier )
		{
			// guy found, no need to add
			return( TRUE );
		}
	}

	if ( pVehicleSoldier )
	{
		// anv: when we enter vehicle in battle, we need to unbusy soldier BEFORE we change selected soldier or else we'll get lock
		UnSetUIBusy( pSoldier->ubID );

		// can't call SelectSoldier in mapscreen, that will initialize interface panels!!!
		if ( guiCurrentScreen == GAME_SCREEN && pSoldier->bTeam == gbPlayerNum )
		{
			SelectSoldier( pVehicleSoldier->ubID, FALSE, TRUE );
		}

		PlayJA2Sample( pVehicleList[ pVehicleSoldier->bVehicleID ].iOutOfSound, RATE_11025, SoundVolume( HIGHVOLUME, pVehicleSoldier->sGridNo ), 1, SoundDir( pVehicleSoldier->sGridNo ) );
	}


	if( pVehicleList[ iId ].pPassengers[ ubSeatIndex ] == NULL )
	{
		ubFinalSeatIndex = ubSeatIndex;
		fFoundPlace = TRUE;
	}
	else
	{
		for( iCounter = 0; !fFoundPlace && iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
		{
			if( pVehicleList[ iId ].pPassengers[ iCounter ] == NULL )
			{
				ubFinalSeatIndex = iCounter;
				fFoundPlace = TRUE;
			}
		}

	}

	if( fFoundPlace )
	{
		// check if slot free
		if( pVehicleList[ iId ].pPassengers[ ubFinalSeatIndex ] == NULL )
		{
			// anv: since now they can shoot it's important to set passenger to proper stance
			SendChangeSoldierStanceEvent( pSoldier, ANIM_CROUCH );

			// add person in
			pVehicleList[ iId ].pPassengers[ ubFinalSeatIndex ] = pSoldier;

			if( pSoldier->bAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
				// NOTE: This will leave the soldier on a squad.	Must be done PRIOR TO and in AS WELL AS the call
				// to RemoveCharacterFromSquads() that's coming up, to permit direct vehicle->vehicle reassignment!
			}

			// if in a squad, remove from squad, if not, check if in vehicle, if so remove, if not, then check if in mvt group..if so, move and destroy group
			if( pSoldier->bTeam == gbPlayerNum && pSoldier->bAssignment < ON_DUTY )
			{
				RemoveCharacterFromSquads( pSoldier );
			}
			else if( pSoldier->bTeam == gbPlayerNum && pSoldier->ubGroupID != 0 )
			{
				// Flugente 2012-08-15: had very weird behaviour here. The outcommented code below failed, because the group size wasn't 0, which then threw an Assert()-error.
				// This happened in r5468 when assinging a merc on repair duty to the truck, switching back and forth between the two
				// I'm fixing it by using RemovePlayerFromGroup(), which seems to be intended just for that. 
				// However, I am at a complete loss as to why this piece of code only throws errors now, seems to me it hasn't changed in ages. Please correct if the error is actually somewhere else
				RemovePlayerFromGroup( pSoldier->ubGroupID, pSoldier );

				// destroy group and set to zero
				//RemoveGroup( pSoldier->ubGroupID );
				//pSoldier->ubGroupID = 0;
			}

			// set vehicle id
			pSoldier->iVehicleId = iId;

			if( pSoldier->bTeam == gbPlayerNum )
			{

				if( ( pSoldier->bAssignment != VEHICLE ) || ( pSoldier->iVehicleId != iId ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				// set thier assignment
				ChangeSoldiersAssignment( pSoldier, VEHICLE );

				// if vehicle is part of mvt group, then add character to mvt group
				if( pVehicleList[ iId ].ubMovementGroup != 0 )
				{
					// add character
					AddPlayerToGroup( pVehicleList[ iId ].ubMovementGroup, pSoldier );
				}

			}

			// Are we the first?
			//if ( GetNumberInVehicle( iId ) == 1 )
			//{
			//	// Set as driver...
			//	pSoldier->flags.uiStatusFlags |= SOLDIER_DRIVER;

			//	SetDriver( iId , pSoldier->ubID );

			//}
			//else
			//{
			//	// Set as driver...
			//	pSoldier->flags.uiStatusFlags |= SOLDIER_PASSENGER;
			//}

			// anv: are we taking driver's seat
			if( gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].VehicleSeats[ubFinalSeatIndex].fDriver )
			{
				SetDriver( iId , pSoldier->ubID );
			}
			else
			{
				pSoldier->flags.uiStatusFlags |= SOLDIER_PASSENGER;
			}

			// Remove soldier's graphic
			pSoldier->RemoveSoldierFromGridNo( );

			// anyone entering a vehicle will be reset to ground level
			pSoldier->SetSoldierHeight(FIRST_LEVEL);

			if ( pVehicleSoldier )
			{
				// bInitialActionPoints - point in time where soldier and vehicle start sharing timeline
				pSoldier->bInitialActionPoints = pSoldier->bActionPoints;
				// set proper initial rotation
				UINT8 ubRotation = gNewVehicle[ pVehicleList[ pVehicleSoldier->bVehicleID ].ubVehicleType ].VehicleSeats[ ubSeatIndex ].ubRotation;
				pSoldier->flags.fDontChargeTurningAPs = TRUE;
				pSoldier->EVENT_SetSoldierDesiredDirection( ( pVehicleSoldier->pathing.bDesiredDirection + ubRotation ) % NUM_WORLD_DIRECTIONS );

				// Set gridno for vehicle.....
				//pSoldier->EVENT_SetSoldierPosition( pVehicleSoldier->dXPos, pVehicleSoldier->dYPos );
				UpdateAllVehiclePassengersGridNo( pVehicleSoldier );

				// Stop from any movement.....
				pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );

				// can't call SetCurrentSquad OR SelectSoldier in mapscreen, that will initialize interface panels!!!
				if ( pSoldier->bTeam == gbPlayerNum && guiCurrentScreen == GAME_SCREEN )
				{
					SetCurrentSquad( pVehicleSoldier->bAssignment, TRUE );
				}
			}

			return( TRUE );
		}
	}

		// no slots, leave
	return( FALSE );
}


void SetSoldierExitVehicleInsertionData( SOLDIERTYPE *pSoldier, INT32 iId, UINT8 iOldGroupID )
{
	GROUP *pGroup;

	if ( iId == iHelicopterVehicleId && !pSoldier->bInSector )
	{
	if( pSoldier->sSectorX	!= AIRPORT_X || pSoldier->sSectorY != AIRPORT_Y || pSoldier->bSectorZ != 0 )
	{
		if( NumHostilesInSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) > 0 && iOldGroupID != 0 && gGameExternalOptions.ubSkyriderHotLZ == 2 )
		{
			pGroup = GetGroup( iOldGroupID );
			if( pGroup->ubSectorX < pGroup->ubPrevX )
			{
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_EAST;
			}
			else if( pGroup->ubSectorX > pGroup->ubPrevX )
			{
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_WEST;
			}
			else if( pGroup->ubSectorY < pGroup->ubPrevY )
			{
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
			}
			else if( pGroup->ubSectorY > pGroup->ubPrevY )
			{
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
			}
			else
			{
				Assert(0);
				return;
			}
		}
		else
		{
			// Not anything different here - just use center gridno......
			pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CENTER;
		}
	}
	else
	{
		// This is drassen, make insertion gridno specific...
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = gModSettings.iHeliSquadDropOff; //10125
	}
	}
}

BOOLEAN RemoveSoldierFromVehicle( SOLDIERTYPE *pSoldier, INT32 iId )
{
	// remove soldier from vehicle
	INT32 iCounter = 0;
	BOOLEAN fSoldierLeft = FALSE;
	BOOLEAN	fSoldierFound = FALSE;
	BOOLEAN	fNewDriverNeeded = FALSE;
	SOLDIERTYPE *pVehicleSoldier;
	UINT8	iOldGroupID=0;


	if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
	{
		return( FALSE );
	}

	// now check if vehicle is valid
	if( pVehicleList[ iId ].fValid == FALSE )
	{
		return( FALSE );
	}

	// now look for the grunt
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] == pSoldier )
		{
			fSoldierFound = TRUE;

			iOldGroupID = pVehicleList[ iId ].pPassengers[ iCounter ]->ubGroupID;
			pVehicleList[ iId ].pPassengers[ iCounter ]->ubGroupID = 0;
			pVehicleList[ iId ].pPassengers[ iCounter ]->sSectorY = pVehicleList[ iId ].sSectorY;
			pVehicleList[ iId ].pPassengers[ iCounter ]->sSectorX = pVehicleList[ iId ].sSectorX;
			pVehicleList[ iId ].pPassengers[ iCounter ]->bSectorZ = ( INT8 )pVehicleList[ iId ].sSectorZ;
			pVehicleList[ iId ].pPassengers[ iCounter ] = NULL;


			// anv: make sure someone else becomes driver if he was the previous one
			if( pSoldier->flags.uiStatusFlags & SOLDIER_DRIVER )
			{
				fNewDriverNeeded = TRUE;
				pVehicleList[ iId ].ubDriver = NOBODY;
			}

			pSoldier->flags.uiStatusFlags &= ( ~( SOLDIER_DRIVER | SOLDIER_PASSENGER ) );

			// check if anyone left in vehicle
			fSoldierLeft = FALSE;
			for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
			{
				if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
				{
					fSoldierLeft = TRUE;
					//if( fNewDriverNeeded )
					//{
					//	SOLDIERTYPE* pNewDriver = pVehicleList[ iId ].pPassengers[ iCounter ];
					//	pNewDriver->flags.uiStatusFlags |= SOLDIER_DRIVER;
					//	pNewDriver->flags.uiStatusFlags &= ~(SOLDIER_PASSENGER);
					//	SetDriver( iId, pNewDriver->ubID );
					//	fNewDriverNeeded = FALSE;
					//}
				}
			}


			if( pVehicleList[ iId ].ubMovementGroup != 0 )
			{
				RemovePlayerFromGroup( pVehicleList[ iId ].ubMovementGroup, pSoldier );

/* ARM 20-01-99, now gonna disallow exiting vehicles between sectors except if merc is leaving (fired, dies, contract runs out)
									in which case we don't need to give them up for anything movement related since they're gone.

				// check if vehicle was between sectors, if so, grunt must go it on foot
				if( pVehicleList[ iId ].fBetweenSectors == TRUE )
				{
					RemoveSoldierFromVehicleBetweenSectors( pSoldier, iId );
				}
*/
			}

			break;
		}
	}


	if ( !fSoldierFound )
	{
		return( FALSE );
	}


	// Are we the last?
	//if ( GetNumberInVehicle( iId ) == 0 )
	if( fSoldierLeft == FALSE )
	{
		// is the vehicle the helicopter?..it can continue moving when no soldiers aboard (Skyrider remains)
		if( iId != iHelicopterVehicleId )
		{
			pVehicleSoldier = GetSoldierStructureForVehicle( iId );
			Assert ( pVehicleSoldier );

			if ( pVehicleSoldier )
			{
				// and he has a route set
				if ( GetLengthOfMercPath( pVehicleSoldier ) > 0 )
				{
					// cancel the entire path (also handles reversing directions)
					CancelPathForVehicle( &( pVehicleList[ iId ] ), FALSE );
				}

				pVehicleSoldier->EVENT_StopMerc( pVehicleSoldier->sGridNo, pVehicleSoldier->ubDirection );

				// if the vehicle was abandoned between sectors
				if ( pVehicleList[ iId ].fBetweenSectors )
				{
					// teleport it to the closer of its current and next sectors (it beats having it arrive empty later)
					TeleportVehicleToItsClosestSector( iId, pVehicleSoldier->ubGroupID );
				}

				// Remove vehicle from squad.....
				RemoveCharacterFromSquads( pVehicleSoldier );
				// ATE: Add him back to vehicle group!
				if ( !DoesPlayerExistInPGroup( pVehicleList[ iId ].ubMovementGroup, pVehicleSoldier ) )
				{
					AddPlayerToGroup( pVehicleList[ iId ].ubMovementGroup, pVehicleSoldier	);
				}
				ChangeSoldiersAssignment( pVehicleSoldier, ASSIGNMENT_EMPTY );


/* ARM Removed Feb. 17, 99 - causes pVehicleSoldier->ubGroupID to become 0, which will cause assert later on
				RemovePlayerFromGroup( pVehicleSoldier->ubGroupID, pVehicleSoldier );
*/

/*
				// Change sides...
				pVehicleSoldier = ChangeSoldierTeam( pVehicleSoldier, CIV_TEAM );
				// subtract it from mapscreen list
				fReBuildCharacterList = TRUE;

				RemoveCharacterFromSquads( pVehicleSoldier );
*/
			}
		}
	}


	// if he got out of the chopper
	if ( iId == iHelicopterVehicleId )
	{
		// and he's alive
		if ( pSoldier->stats.bLife >= OKLIFE )
		{
			// mark the sector as visited (flying around in the chopper doesn't, so this does it as soon as we get off it)
			SetSectorFlag( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, SF_ALREADY_VISITED );
		}

	SetSoldierExitVehicleInsertionData( pSoldier, iId, iOldGroupID );

	// Update in sector if this is the current sector.....
		if ( pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ )
		{
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}

	}

	// soldier successfully removed
	return( TRUE );
}


/*
void RemoveSoldierFromVehicleBetweenSectors( pSoldier, iId )
{
	GROUP *pGroup;
	INT32 iCurrentCostInTime = 0;
	INT8 bDelta = 0;
	UINT8 ubCurrent, ubNext, ubDirection, ubSector;
	UINT8 ubNextX, ubNextY, ubPrevX, ubPrevY;
	UINT32 uiTraverseTime, uiArriveTime;
	UINT8 ubGroupId;
	float flTripFractionCovered;


	// set up a mvt group for the grunt
	pSoldier->flags.fBetweenSectors = TRUE;

	// ok, the guy wasn't in a squad
	// get his mvt groups position and set the squads to this
	GetGroupPosition(&ubNextX, &ubNextY, &ubPrevX, &ubPrevY, &uiTraverseTime, &uiArriveTime, pVehicleList[ iId ].ubMovementGroup );

	ubGroupId = CreateNewPlayerGroupDepartingFromSector( ( INT8 ) ( pSoldier->sSectorX ) , ( INT8 ) ( pSoldier->sSectorY ) );

	// assign to a group
	AddPlayerToGroup( ubGroupId, pSoldier );

	// get the group
	pGroup = GetGroup( ubGroupId );


	// find total time for traversal between sectors for foot mvt type

	// get current and next sector values (DON'T use prevX, prevY, that's the sector BEFORE the current one!!!)
	ubCurrent = CALCULATE_STRATEGIC_INDEX( pGroup->ubSectorX, pGroup->ubSectorY );
	ubNext = CALCULATE_STRATEGIC_INDEX( ubNextX, ubNextY );

	// handle errors
	Assert( ubCurrent != ubNext );
	if ( ubCurrent == ubNext )
		continue;

	// which direction are we moving in?
	bDelta = (INT8) ubNext - ubCurrent;
	if( bDelta > 0 )
	{
		if( bDelta % SOUTH_MOVE == 0 )
		{
			ubDirection = SOUTH_STRATEGIC_MOVE;
		}
		else
		{
			ubDirection = EAST_STRATEGIC_MOVE;
		}
	}
	else
	{
		if( bDelta % NORTH_MOVE == 0 )
		{
			ubDirection = NORTH_STRATEGIC_MOVE;
		}
		else
		{
			ubDirection = WEST_STRATEGIC_MOVE;
		}
	}


	// calculate how long the entire trip would have taken on foot
	ubSector = ( UINT8 ) SECTOR( pGroup->ubSectorX, pGroup->ubSectorY );
	iCurrentCostInTime = GetSectorMvtTimeForGroup( ubSector, ubDirection, pGroup );

	if( iCurrentCostInTime == 0xffffffff )
	{
		AssertMsg( 0, String("Group %d (%s) attempting illegal move from sector %d, dir %d (%s).",
				pGroup->ubGroupID, ( pGroup->fPlayer ) ? "Player" : "AI",
				ubSector, ubDirection,
				gszTerrain[SectorInfo[ubSector].ubTraversability[ubDirection]] ) );
	}

	// figure out what how far along ( percentage ) the vehicle's trip duration we bailed out at
	flTripFractionCovered = ( uiTraverseTime - uiArriveTime + GetWorldTotalMin( ) ) / (float)uiTraverseTime;

	// calculate how much longer we have to go on foot to get there
	uiArriveTime = ( UINT32 )( ( ( 1.0 - flTripFractionCovered ) * ( float )iCurrentCostInTime ) + GetWorldTotalMin( ) );

	SetGroupPosition( ubNextX, ubNextY, ubPrevX, ubPrevY, iCurrentCostInTime, uiArriveTime, pSoldier->ubGroupID );

// ARM: if this is ever reactivated, there seem to be the following additional problems:
	1) The soldier removed isn't showing any DEST.	Must set up his strategic path/destination.
	2) The arrive time seems to be much later than it should have been, suggesting the math above is wrong somehow
	3) Reassigning multiple mercs at once out of a vehicle onroute doesn't work 'cause the group is between sectors so only
			the first merc gets added successfully, the others all fail.
}
*/



BOOLEAN MoveCharactersPathToVehicle( SOLDIERTYPE *pSoldier )
{
	INT32 iId;
	// valid soldier?
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}

	// check if character is in fact in a vehicle
	if( ( pSoldier->bAssignment != VEHICLE ) && ( ! ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ) )
	{
		// now clear soldier's path
		pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, 0 );
		return( FALSE );
	}

	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// grab the id the character is
		iId = pSoldier->bVehicleID;
	}
	else
	{
		// grab the id the character is
		iId = pSoldier->iVehicleId;
	}

	// check if vehicle is valid
	if( iId != -1 )
	{
		// check if vehicle has mvt group, if not, get one for it
		if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
		{
			// now clear soldier's path
			pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, 0 );
			return ( FALSE );
		}

		// now check if vehicle is valid
		if( pVehicleList[ iId ].fValid == FALSE )
		{
			// now clear soldier's path
			pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, 0 );
			return( FALSE );
		}
	}


	// valid vehicle

	// now clear soldier's path
	pVehicleList[ iId ].pMercPath = ClearStrategicPathList( pVehicleList[ iId ].pMercPath, pVehicleList[ iId ].ubMovementGroup );

	// now copy over
	pVehicleList[ iId ].pMercPath = CopyPaths( pSoldier->pMercPath, pVehicleList[ iId ].pMercPath );

	// move to beginning
	pVehicleList[ iId ].pMercPath = MoveToBeginningOfPathList( pVehicleList[ iId ].pMercPath );

	// now clear soldier's path
	pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, 0 );

	return( TRUE );
}

BOOLEAN CopyVehiclePathToSoldier( SOLDIERTYPE *pSoldier )
{
	INT32 iId;

	// valid soldier?
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}

	// check if character is in fact in a vehicle
	if( ( pSoldier->bAssignment != VEHICLE ) && ( ! ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ) )
	{
		return( FALSE );
	}

	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// grab the id the character is
		iId = pSoldier->bVehicleID;
	}
	else
	{
		// grab the id the character is
		iId = pSoldier->iVehicleId;
	}

	// check if vehicle is valid
	if( iId != -1 )
	{
		// check if vehicle has mvt group, if not, get one for it
		if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
		{
			return ( FALSE );
		}

		// now check if vehicle is valid
		if( pVehicleList[ iId ].fValid == FALSE )
		{
			return( FALSE );
		}
	}
	
	// reset mvt group for the grunt
	// ATE: NOT if we are the vehicle
	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		pSoldier->ubGroupID = pVehicleList[ iId ].ubMovementGroup;
	}

	// valid vehicle

	// clear grunt path
	if( pSoldier->pMercPath )
	{
		// clear soldier's path
		pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, 0 );
	}

	// now copy over
	pSoldier->pMercPath = CopyPaths(pVehicleList[ iId ].pMercPath, pSoldier->pMercPath );

	return( TRUE );
}

BOOLEAN IsVehicle(SOLDIERTYPE *pSoldier)
{
	return ( gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_VEHICLE );
}

BOOLEAN SetUpMvtGroupForVehicle( SOLDIERTYPE *pSoldier )
{
	// given this grunt, find out if asscoiated vehicle has a mvt group, if so, set this grunts mvt group tho the vehicle
	// for pathing purposes, will be reset to zero in copying of path
	INT32 iId = 0;
#ifndef RELEASE_WITH_DEBUG_INFO
	//INT32 iCounter = 0;
#endif // RELEASE_WITH_DEBUG_INFO

		// check if character is in fact in a vehicle
	if( ( pSoldier->bAssignment != VEHICLE ) && ( ! ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ) )
	{
		return( FALSE );
	}

	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// grab the id the character is
		iId = pSoldier->bVehicleID;
	}
	else
	{
		// grab the id the character is
		iId = pSoldier->iVehicleId;
	}

	if( pSoldier->pMercPath )
	{
		// clear soldier's path
		pSoldier->pMercPath = ClearStrategicPathList( pSoldier->pMercPath, pSoldier->ubGroupID );
	}

	// if no group, create one for vehicle
	//if( pVehicleList[ iId ].ubMovementGroup == 0 )
	//{
		// get the vehicle a mvt group
		//pVehicleList[ iId ].ubMovementGroup = CreateNewVehicleGroupDepartingFromSector( ( UINT8 )( pVehicleList[ iId ].sSectorX ), ( UINT8 )( pVehicleList[ iId ].sSectorY ), iId );
		//pVehicleList[ iId ].ubMovementGroup = CreateNewVehicleGroupDepartingFromSector( ( UINT8 )( pVehicleList[ iId ].sSectorX ), ( UINT8 )( pVehicleList[ iId ].sSectorY ), iId );


		// add everyone in vehicle to this mvt group
		//for( iCounter = 0; iCounter < gNewVehicle.iNewSeatingCapacities[ pVehicleList[ iId ].ubVehicleType ]; iCounter++ )
		//{
		//	if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		//	{
		//			// add character
		//		AddPlayerToGroup( pVehicleList[ iId ].ubMovementGroup, pVehicleList[ iId ].pPassengers[ iCounter ] );
		//	}
		//}
	//}

	CopyVehiclePathToSoldier( pSoldier );

	// set up mvt group
	pSoldier->ubGroupID = pVehicleList[ iId ].ubMovementGroup;


	return ( TRUE );
}
BOOLEAN VehicleIdIsValid( INT32 iId )
{
	// check if vehicle has mvt group, if not, get one for it
	if( ( iId >= ubNumberOfVehicles ) || ( iId < 0 ) )
	{
		return ( FALSE );
	}
		// now check if vehicle is valid
	if( pVehicleList[ iId ].fValid == FALSE )
	{
		return( FALSE );
	}

	return( TRUE );
}

// get travel time of vehicle
INT32 GetTravelTimeOfVehicle( INT32 iId )
{
	GROUP *pGroup;


	// valid vehicle?
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return( 0 );
	}

	// no mvt group?
	if( pVehicleList[ iId ].ubMovementGroup == 0 )
	{
		return ( 0 );
	}

	pGroup = GetGroup( pVehicleList[ iId ].ubMovementGroup );

	if( pGroup == NULL )
	{
		pVehicleList[ iId ].ubMovementGroup = 0;
		return( 0 );
	}

	return( CalculateTravelTimeOfGroupId( pVehicleList[ iId ].ubMovementGroup ) );
}



void UpdatePositionOfMercsInVehicle( INT32 iId )
{
	INT32 iCounter = 0;

	// update the position of all the grunts in the vehicle
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return;
	}

	// go through list of mercs in vehicle and set all thier states as arrived
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			pVehicleList[ iId ].pPassengers[ iCounter ]->sSectorY = pVehicleList[ iId ].sSectorY;
			pVehicleList[ iId ].pPassengers[ iCounter ]->sSectorX = pVehicleList[ iId ].sSectorX;
			pVehicleList[ iId ].pPassengers[ iCounter ]->flags.fBetweenSectors = FALSE;
		}
	}

	return;
}



INT32 GivenMvtGroupIdFindVehicleId( UINT8 ubGroupId )
{
	INT32 iCounter = 0;

	// given the id of a mvt group, find a vehicle in this group
	if( ubGroupId == 0 )
	{
		return ( -1 );
	}

	for( iCounter = 0; iCounter < ubNumberOfVehicles ; iCounter++ )
	{
		// might have an empty slot
		if( pVehicleList[ iCounter ].fValid == TRUE )
		{
			if( pVehicleList[ iCounter ].ubMovementGroup == ubGroupId )
			{
				return( iCounter );
			}
		}
	}

	return( -1 );
}

// add all people in this vehicle to the mvt group for benifit of prebattle interface
BOOLEAN AddVehicleMembersToMvtGroup( INT32 iId )
{
	INT32 iCounter = 0;

	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return( FALSE );
	}

	// clear the vehicle people list out
	// RemoveAllPlayersFromGroup( pVehicleList[ iId ].ubMovementGroup );

	// go through list of mercs in vehicle and set all thier states as arrived
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			AddPlayerToGroup( pVehicleList[ iId ].ubMovementGroup , pVehicleList[ iId ].pPassengers[ iCounter ] );
		}
	}

	return( TRUE );
}


BOOLEAN InjurePersonInVehicle( INT32 iId, SOLDIERTYPE *pSoldier, UINT8 ubPointsOfDmg )
{
	// find this person, see if they have this many pts left, if not, kill them


	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( FALSE );
	}

	// check if soldier is valid
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}

	// now check hpts of merc
	if( pSoldier->stats.bLife == 0 )
	{
		// guy is dead, leave
		return( FALSE );
	}

	// see if we will infact kill them
	if( ubPointsOfDmg >= pSoldier->stats.bLife )
	{
		return( KillPersonInVehicle( iId, pSoldier ) );
	}

	// otherwise hurt them
	pSoldier->SoldierTakeDamage( 0, ubPointsOfDmg, ubPointsOfDmg, TAKE_DAMAGE_GUNFIRE, NOBODY, NOWHERE, 0, TRUE );

	pSoldier->HandleSoldierTakeDamageFeedback( );

	return( TRUE );
}

BOOLEAN KillPersonInVehicle( INT32 iId, SOLDIERTYPE *pSoldier )
{
	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( FALSE );
	}

	// check if soldier is valid
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}

	// now check hpts of merc
	if( pSoldier->stats.bLife == 0 )
	{
		// guy is dead, leave
		return( FALSE );
	}

	// otherwise hurt them
	pSoldier->SoldierTakeDamage( 0, 100, 100, TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, TRUE );

	return( TRUE );
}

BOOLEAN KillAllInVehicle( INT32 iId )
{
	INT32 iCounter = 0;

	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( FALSE );
	}

	// go through list of occupants and kill them
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			if( KillPersonInVehicle( iId , pVehicleList[ iId ].pPassengers[ iCounter ] ) == FALSE )
			{
				return( FALSE );
			}
		}
	}
	return ( TRUE );
}

// anv: for hurting heli passengers on SAM attack
BOOLEAN HurtPassengersInHelicopter( INT32 iId )
{
	INT32 iCounter = 0;
	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( FALSE );
	}

	// go through list of occupants and hurt them
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			if( PreRandom(100) < gHelicopterSettings.ubHelicopterPassengerHitChance )
			{
				if( InjurePersonInVehicle( iId , pVehicleList[ iId ].pPassengers[ iCounter ], 
					gHelicopterSettings.ubHelicopterPassengerHitMinDamage + PreRandom( gHelicopterSettings.ubHelicopterPassengerHitMaxDamage - gHelicopterSettings.ubHelicopterPassengerHitMinDamage ) ) == FALSE )
				{
					return( FALSE );
				}
			}
		}
	}
	return ( TRUE );
}

INT32 GetNumberInVehicle( INT32 iId )
{
	// go through list of occupants in vehicles and count them
	INT32 iCounter = 0;
	INT32 iCount = 0;

	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( 0 );
	}

	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			iCount++;
		}
	}

	return( iCount );
}

INT32 GetNumberOfNonEPCsInVehicle( INT32 iId )
{
	// go through list of occupants in vehicles and count them
	INT32 iCounter = 0;
	INT32 iCount = 0;

	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( 0 );
	}

	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL && !AM_AN_EPC( pVehicleList[ iId ].pPassengers[ iCounter ] ) )
		{
			iCount++;
		}
	}

	return( iCount );
}

BOOLEAN IsRobotControllerInVehicle( INT32 iId )
{
	// go through list of occupants in vehicles and count them
	INT32 iCounter = 0;
	SOLDIERTYPE * pSoldier;

	// find if vehicle is valid
	if( VehicleIdIsValid( iId ) == FALSE )
	{
		return ( 0 );
	}

	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		pSoldier = pVehicleList[ iId ].pPassengers[ iCounter ];
		if ( pSoldier != NULL && pSoldier->ControllingRobot( ) )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}



BOOLEAN AnyAccessibleVehiclesInSoldiersSector( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < ubNumberOfVehicles ; iCounter++ )
	{
		if ( pVehicleList[ iCounter ].fValid == TRUE )
		{
			if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounter ) )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

SOLDIERTYPE *GetDriver( INT32 iID )
{
	INT32 iCounter;
	if( pVehicleList[ iID ].ubDriver != NOBODY )
	{
		return( MercPtrs[ pVehicleList[ iID ].ubDriver ] );
	}
	else
	{
		// anv: if vehicle has no driver, return any passenger
		for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iID ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
		{
			if( pVehicleList[ iID ].pPassengers[ iCounter ] != NULL )
			{
				return( pVehicleList[ iID ].pPassengers[ iCounter ] );
			}
		}
	}
	return( NULL );
}


void SetDriver( INT32 iID, UINT8 ubID )
{
	// anv: first make sure previous driver won't be driver anymore
	if( pVehicleList[ iID ].ubDriver != NOBODY && MercPtrs[ pVehicleList[ iID ].ubDriver ]->iVehicleId == iID )
	{
		if( MercPtrs[ pVehicleList[ iID ].ubDriver ] )
		{
			MercPtrs[ pVehicleList[ iID ].ubDriver ]->flags.uiStatusFlags &= ~(SOLDIER_DRIVER);
			if( GetSeatIndexFromSoldier( MercPtrs[ pVehicleList[ iID ].ubDriver ] ) != (-1) )
			{
				MercPtrs[ pVehicleList[ iID ].ubDriver ]->flags.uiStatusFlags |= SOLDIER_PASSENGER;
			}
		}
	}
	// set proper flags
	if( ubID != NOBODY )
	{
		MercPtrs[ ubID ]->flags.uiStatusFlags |= SOLDIER_DRIVER;
		MercPtrs[ ubID ]->flags.uiStatusFlags &= ~(SOLDIER_PASSENGER);
	}
	pVehicleList[ iID ].ubDriver = ubID;
}


#ifdef JA2TESTVERSION
void VehicleTest( void )
{
	SetUpHelicopterForPlayer( 9,1, gNewVehicle[ HELICOPTER ].NewPilot, HELICOPTER );
}
#endif


BOOLEAN IsEnoughSpaceInVehicle( INT32 iID )
{
	// find if vehicle is valid
	if( VehicleIdIsValid( iID ) == FALSE )
	{
		return ( FALSE );
	}

	if ( GetNumberInVehicle( iID ) == gNewVehicle[ pVehicleList[ iID ].ubVehicleType ].iNewSeatingCapacities )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN PutSoldierInVehicle( SOLDIERTYPE *pSoldier, INT8 bVehicleId )
{

	SOLDIERTYPE *pVehicleSoldier = NULL;

	if( ( pSoldier->sSectorX != gWorldSectorX ) || ( pSoldier->sSectorY != gWorldSectorY ) || ( pSoldier->bSectorZ != 0 ) || ( bVehicleId	== iHelicopterVehicleId ) )
	{
		// add the soldier
		return( AddSoldierToVehicle( pSoldier, bVehicleId ) );
	}
	else
	{
		// grab the soldier struct for the vehicle
		pVehicleSoldier = GetSoldierStructureForVehicle( bVehicleId );

		// enter the vehicle
		return( EnterVehicle( pVehicleSoldier, pSoldier ) );
	}
}


BOOLEAN TakeSoldierOutOfVehicle( SOLDIERTYPE *pSoldier )
{
	// if not in vehicle, don't take out, not much point, now is there?
	if( pSoldier->bAssignment != VEHICLE )
	{
		return( FALSE );
	}

	if( ( pSoldier->sSectorX != gWorldSectorX ) || ( pSoldier->sSectorY != gWorldSectorY ) || ( pSoldier->bSectorZ != 0 ) || !pSoldier->bInSector )
	{
		// add the soldier
		return( RemoveSoldierFromVehicle( pSoldier, pSoldier->iVehicleId ) );
	}
	else
	{
		// helicopter isn't a soldiertype instance
		if( pSoldier->iVehicleId == iHelicopterVehicleId )
		{
			return( RemoveSoldierFromVehicle( pSoldier, pSoldier->iVehicleId ) );
		}
		else
		{
			// exit the vehicle
			return( ExitVehicle( pSoldier ) );
		}
	}
}



BOOLEAN EnterVehicle( SOLDIERTYPE *pVehicle, SOLDIERTYPE *pSoldier, UINT8 ubSeatIndex )
{

	// TEST IF IT'S VALID...
	if ( pVehicle->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// Is there room...
		if ( IsEnoughSpaceInVehicle( pVehicle->bVehicleID ) )
		{
			// anv: check if preferred seat is already taken, if so automatically choose another
			if ( pVehicleList[ pVehicle->bVehicleID ].pPassengers[ubSeatIndex] != NULL )
			{
				ubSeatIndex = 0;
			}

			INT16	sAPCost = APBPConstants[AP_ENTER_VEHICLE];
			//Are we currently in combat?
			if(gTacticalStatus.uiFlags & INCOMBAT)
			{
				if(EnoughPoints(pSoldier, sAPCost, 0, TRUE))
					DeductPoints(pSoldier, sAPCost, 0);
				else
					return( FALSE );
			}

			// OK, add....
			if ( !AddSoldierToVehicle( pSoldier, pVehicle->bVehicleID, ubSeatIndex) )
				return( FALSE);

			if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
				// Change to team panel if we are not already...
				SetCurrentInterfacePanel( TEAM_PANEL );
			}

			return( TRUE );
		}
	}

	return( FALSE );
}

SOLDIERTYPE *GetVehicleSoldierPointerFromPassenger( SOLDIERTYPE *pSrcSoldier )
{
	UINT32									cnt;
	SOLDIERTYPE			 *pSoldier;

	// End the turn of player charactors
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		if ( pSoldier->bActive && pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			// Check ubID....
			if ( pSoldier->bVehicleID == pSrcSoldier->iVehicleId )
			{
				return( pSoldier );
			}
		}
	}

	return( NULL );
}


BOOLEAN ExitVehicle( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE		*pVehicle;
	UINT8					ubDirection;
	INT32 sGridNo;

	// Get vehicle from soldier...
	pVehicle = GetVehicleSoldierPointerFromPassenger( pSoldier );

	if ( pVehicle == NULL )
	{
		return( FALSE );
	}

	INT16	sAPCost = APBPConstants[AP_EXIT_VEHICLE];
	//Are we currently in combat?
	if(gTacticalStatus.uiFlags & INCOMBAT)
	{
		if(EnoughPoints(pSoldier, sAPCost, 0, TRUE))
			DeductPoints(pSoldier, sAPCost, 0);
		else
			return( FALSE );
	}

	// TEST IF IT'S VALID...
	if ( pVehicle->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		sGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pSoldier, pSoldier->usUIMovementMode, 5, &ubDirection, 3, pVehicle );
		
		if (TileIsOutOfBounds(sGridNo))
		{
			// ATE: BUT we need a place, widen the search
			sGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pSoldier, pSoldier->usUIMovementMode, 20, &ubDirection, 3, pVehicle );
		}

		// OK, remove....
		RemoveSoldierFromVehicle( pSoldier, pVehicle->bVehicleID );

		// Were we the driver, and if so, pick another....
		pSoldier->sInsertionGridNo = sGridNo;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = pSoldier->sInsertionGridNo;
		pSoldier->iVehicleId = -1;

		//AllTeamsLookForAll( FALSE );
		pSoldier->aiData.bOppList[ pVehicle->ubID ] = 1;

		// Add to sector....
		INT16 sX, sY;
		ConvertGridNoToCenterCellXY(pSoldier->sGridNo, &sX, &sY);
		pSoldier->EVENT_SetSoldierPosition( sX, sY );

		// anv: since now they can shoot it's important to set passenger to proper stance
		// namely, back to standing, because we set them to crouching when entering
		SendChangeSoldierStanceEvent( pSoldier, ANIM_STAND );
		pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

		// Update visiblity.....
		HandleSight(pSoldier,SIGHT_LOOK | SIGHT_RADIO );
	
		if( !gGameExternalOptions.fAddPassengerToAnySquad )
		{
			// Add to unique squad....
			AddCharacterToUniqueSquad( pSoldier );
		}
		else
		{
			// anv: optionally add to any squad
			AddCharacterToAnySquad( pSoldier );
		}

		// can't call SetCurrentSquad OR SelectSoldier in mapscreen, that will initialize interface panels!!!
		if ( guiCurrentScreen == GAME_SCREEN )
		{
			if( gGameExternalOptions.fPassengerLeavingSwitchToNewSquad )
			{
				SetCurrentSquad( pSoldier->bAssignment, TRUE );

				SelectSoldier( pSoldier->ubID, FALSE, TRUE );
			}
			else
			{
				UINT16 usTempSelectedSoldier = gusSelectedSoldier;

				SetCurrentSquad( CurrentSquad(), TRUE );

				SelectSoldier( usTempSelectedSoldier, FALSE, FALSE );
			}

		}

		PlayJA2Sample( pVehicleList[ pVehicle->bVehicleID ].iOutOfSound, RATE_11025, SoundVolume( HIGHVOLUME, pVehicle->sGridNo ), 1, SoundDir( pVehicle->sGridNo ) );		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN ChangeVehicleSeat( SOLDIERTYPE *pVehicle, SOLDIERTYPE *pSoldier, UINT8 ubSeatIndex )
{	
	// TEST IF IT'S VALID...
	if ( pVehicle->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// anv: check if preferred seat is available
		if ( pVehicleList[ pVehicle->bVehicleID ].pPassengers[ubSeatIndex] == NULL )
		{

			INT8 bCurrentSeatIndex = GetSeatIndexFromSoldier( pSoldier );
			if( bCurrentSeatIndex == (-1) )
			{
				return( FALSE );
			}

			INT16	sAPCost = APBPConstants[AP_CHANGE_SEAT_VEHICLE];

			//Are we currently in combat?
			if(gTacticalStatus.uiFlags & INCOMBAT)
			{
				if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[bCurrentSeatIndex].ubCompartment !=
					gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[ubSeatIndex].ubCompartment )
				{
					// different compartment!
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pVehicleSeatsStrings[1] );
					return( FALSE );
				}

				if(EnoughPoints(pSoldier, sAPCost, 0, TRUE))
					DeductPoints(pSoldier, sAPCost, 0);
				else
					return( FALSE );
			}

			pVehicleList[ pVehicle->bVehicleID ].pPassengers[ubSeatIndex] = pSoldier;
			pVehicleList[ pVehicle->bVehicleID ].pPassengers[bCurrentSeatIndex] = NULL;

			if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[bCurrentSeatIndex].fDriver )
			{
				SetDriver( pVehicle->bVehicleID, NOBODY );
			}

			if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[ubSeatIndex].fDriver )
			{
				SetDriver( pVehicle->bVehicleID, pSoldier->ubID );
			}

			// set proper initial rotation
			UINT8 ubRotation = gNewVehicle[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ].VehicleSeats[ ubSeatIndex ].ubRotation;
			pSoldier->flags.fDontChargeTurningAPs = TRUE;
			pSoldier->EVENT_SetSoldierDesiredDirection( ( pVehicle->pathing.bDesiredDirection + ubRotation ) % NUM_WORLD_DIRECTIONS );

			UpdateAllVehiclePassengersGridNo( pVehicle );
			
			return( TRUE );
		}
	}

	return( FALSE );
}

BOOLEAN SwapVehicleSeat( SOLDIERTYPE *pVehicle, SOLDIERTYPE *pSoldier, UINT8 ubSeatIndex )
{	
	// TEST IF IT'S VALID...
	if ( pVehicle->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		SOLDIERTYPE *pSoldier2 = pVehicleList[ pVehicle->bVehicleID ].pPassengers[ubSeatIndex];

		if ( pSoldier2 != NULL )
		{
			INT8 bCurrentSeatIndex = GetSeatIndexFromSoldier( pSoldier );
			if( bCurrentSeatIndex == (-1) )
			{
				return( FALSE );
			}

			INT16	sAPCost = APBPConstants[AP_CHANGE_SEAT_VEHICLE];

			//Are we currently in combat?
			if(gTacticalStatus.uiFlags & INCOMBAT)
			{
				if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[bCurrentSeatIndex].ubCompartment !=
					gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[ubSeatIndex].ubCompartment )
				{
					// different compartment!
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pVehicleSeatsStrings[1] );		
					return( FALSE );
				}

				if( EnoughPoints(pSoldier, sAPCost, 0, TRUE) && EnoughPoints(pSoldier2, sAPCost, 0, TRUE) )
				{
					DeductPoints(pSoldier, sAPCost, 0);
					DeductPoints(pSoldier2, sAPCost, 0);
				}
				else
					return( FALSE );
			}

			pVehicleList[ pVehicle->bVehicleID ].pPassengers[ubSeatIndex] = pSoldier;
			pVehicleList[ pVehicle->bVehicleID ].pPassengers[bCurrentSeatIndex] = pSoldier2;

			if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[bCurrentSeatIndex].fDriver )
			{
				SetDriver( pVehicle->bVehicleID, pSoldier2->ubID );
			}

			if( gNewVehicle[pVehicleList[ pVehicle->bVehicleID ].ubVehicleType].VehicleSeats[ubSeatIndex].fDriver )
			{
				SetDriver( pVehicle->bVehicleID, pSoldier->ubID );
			}

			// set proper initial rotations
			UINT8 ubRotation = gNewVehicle[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ].VehicleSeats[ ubSeatIndex ].ubRotation;
			pSoldier->flags.fDontChargeTurningAPs = TRUE;
			pSoldier->EVENT_SetSoldierDesiredDirection( ( pVehicle->pathing.bDesiredDirection + ubRotation ) % NUM_WORLD_DIRECTIONS );

			ubRotation = gNewVehicle[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ].VehicleSeats[ bCurrentSeatIndex ].ubRotation;
			pSoldier2->flags.fDontChargeTurningAPs = TRUE;
			pSoldier2->EVENT_SetSoldierDesiredDirection( ( pVehicle->pathing.bDesiredDirection + ubRotation ) % NUM_WORLD_DIRECTIONS );

			UpdateAllVehiclePassengersGridNo( pVehicle );
			
			return( TRUE );
		}
		else
		{
			return( ChangeVehicleSeat( pVehicle, pSoldier, ubSeatIndex ) );
		}
	}

	return( FALSE );
}

void AddPassangersToTeamPanel( INT32 iId )
{
	INT32 cnt;

	for( cnt = 0; cnt < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; cnt++ )
	{
		if( pVehicleList[ iId ].pPassengers[ cnt ] != NULL )
		{
			// add character
			AddPlayerToInterfaceTeamSlot( pVehicleList[ iId ].pPassengers[ cnt ]->ubID );
		}
	}
}


void VehicleTakeDamage( UINT8 ubID, UINT8 ubReason, INT16 sDamage, INT32 sGridNo, UINT8 ubAttackerID )
{
	if ( ubReason != TAKE_DAMAGE_GAS_FIRE && ubReason != TAKE_DAMAGE_GAS_NOTFIRE )
	{
		PlayJA2Sample( (UINT32)( S_METAL_IMPACT3 ), RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
	}

	// check if there was in fact damage done to the vehicle
	if( ( ubReason == TAKE_DAMAGE_HANDTOHAND ) || ( ubReason == TAKE_DAMAGE_GAS_FIRE ) || ( ubReason == TAKE_DAMAGE_GAS_NOTFIRE ) )
	{
		// nope
		return;
	}

	if( pVehicleList[ ubID ].fDestroyed == FALSE )
	{
		switch( ubReason )
		{
			case( TAKE_DAMAGE_GUNFIRE ):
			case( TAKE_DAMAGE_EXPLOSION):
			case( TAKE_DAMAGE_STRUCTURE_EXPLOSION):

			HandleCriticalHitForVehicleInLocation( ubID, sDamage, sGridNo, ubAttackerID );
			break;
		}
	}
}


void HandleCriticalHitForVehicleInLocation( UINT8 ubID, INT16 sDmg, INT32 sGridNo, UINT8 ubAttackerID )
{
	// check state the armor was s'posed to be in vs. the current state..the difference / orig state is % chance
	// that a critical hit will occur
	SOLDIERTYPE *pSoldier;
	BOOLEAN	fMadeCorpse = FALSE;


	pSoldier = GetSoldierStructureForVehicle( ubID );
    Assert(pSoldier);

	if ( sDmg > pSoldier->stats.bLife )
	{
		pSoldier->stats.bLife = 0;
	}
	else
	{
		// Decrease Health
		pSoldier->stats.bLife -= sDmg;
	}

	if ( pSoldier->stats.bLife < OKLIFE )
	{
		pSoldier->stats.bLife = 0;
	}

	//Show damage
	pSoldier->sDamage += sDmg;

	if ( pSoldier->bInSector && pSoldier->bVisible != -1 )
	{
		// If we are already dead, don't show damage!
		if ( sDmg != 0 )
		{
			// Display damage
			INT16 sMercScreenX, sMercScreenY, sOffsetX, sOffsetY;

			// Set Damage display counter
			pSoldier->flags.fDisplayDamage = TRUE;
			pSoldier->bDisplayDamageCount = 0;

			GetSoldierScreenPos( pSoldier, &sMercScreenX, &sMercScreenY );
			GetSoldierAnimOffsets( pSoldier, &sOffsetX, &sOffsetY );
			pSoldier->sDamageX = sOffsetX;
			pSoldier->sDamageY = sOffsetY;
		}
	}

	if ( pSoldier->stats.bLife == 0 && !pVehicleList[ ubID ].fDestroyed )
	{
		pVehicleList[ ubID ].fDestroyed	= TRUE;

		// Explode vehicle...
		INT16 sX, sY;
		ConvertGridNoToCenterCellXY(sGridNo, &sX, &sY);
		IgniteExplosion( ubAttackerID, sX, sY, 0, sGridNo, GREAT_BIG_EXPLOSION, 0 );

		if ( pSoldier != NULL )
		{
			// Tacticlly remove soldier....
			// pSoldier->EVENT_InitNewSoldierAnim( VEHICLE_DIE, 0, FALSE );
			//TacticalRemoveSoldier( pSoldier->ubID );

			CheckForAndHandleSoldierDeath( pSoldier, &fMadeCorpse );
		}

	// Kill all in vehicle...
	KillAllInVehicle( ubID );

	}

	return;
}



BOOLEAN DoesVehicleNeedAnyRepairs( INT32 iVehicleId )
{
	SOLDIERTYPE		*pVehicleSoldier = NULL;


	// is the vehicle in fact a valid vehicle
	if( VehicleIdIsValid( iVehicleId ) == FALSE )
	{
		// nope
		return( FALSE );
	}

	// Skyrider isn't damagable/repairable
	if ( iVehicleId == iHelicopterVehicleId )
	{
		return( FALSE );
	}

	// get the vehicle soldiertype
	pVehicleSoldier = GetSoldierStructureForVehicle( iVehicleId );

	if ( pVehicleSoldier->stats.bLife != pVehicleSoldier->stats.bLifeMax )
	{
		return( TRUE );
	}

	// everything is in perfect condition
	return( FALSE);
}


INT8 RepairVehicle( INT32 iVehicleId, UINT8 ubRepairPtsLeft, BOOLEAN *pfNothingToRepair )
{
	SOLDIERTYPE		*pVehicleSoldier = NULL;
	INT8					bRepairPtsUsed = 0;
	INT8					bOldLife;

	// is the vehicle in fact a valid vehicle
	if( VehicleIdIsValid( iVehicleId ) == FALSE )
	{
		// nope
		return( bRepairPtsUsed );
	}

	// Skyrider isn't damagable/repairable
	if ( iVehicleId == iHelicopterVehicleId )
	{
		return( bRepairPtsUsed );
	}

	// get the vehicle soldiertype
	pVehicleSoldier = GetSoldierStructureForVehicle( iVehicleId );

	if ( !DoesVehicleNeedAnyRepairs( iVehicleId ) )
	{
		return( bRepairPtsUsed );
	}

	bOldLife = pVehicleSoldier->stats.bLife;

	// Repair
	pVehicleSoldier->stats.bLife += ( ubRepairPtsLeft / VEHICLE_REPAIR_POINTS_DIVISOR );

	// Check
	if ( pVehicleSoldier->stats.bLife > pVehicleSoldier->stats.bLifeMax )
	{
		pVehicleSoldier->stats.bLife = pVehicleSoldier->stats.bLifeMax;
	}

	// Calculate pts used;
	bRepairPtsUsed = ( pVehicleSoldier->stats.bLife - bOldLife ) * VEHICLE_REPAIR_POINTS_DIVISOR;

	// ARM: personally, I'd love to know where in Arulco the mechanic gets the PARTS to do this stuff, but hey, it's a game!
	(*pfNothingToRepair) = !DoesVehicleNeedAnyRepairs( iVehicleId );

	return( bRepairPtsUsed );
}


/*
INT16 GetOrigInternalArmorValueForVehicleInLocation( UINT8 ubID, UINT8 ubLocation )
{
	INT16 sArmorValue = 0;

	sArmorValue = sVehicleInternalOrigArmorValues[ pVehicleList[ ubID ].ubVehicleType ][ ubLocation ];

	// return the armor value
	return( sArmorValue );
}
*/

SOLDIERTYPE * GetSoldierStructureForVehicle( INT32 iId )
{
	if( iId == (-1) )
	{
		return( 0 );
	}

	for( INT32 iCounter = 0; iCounter < TOTAL_SOLDIERS; iCounter++ )
	{
		SOLDIERTYPE *pSoldier = &Menptr[ iCounter ];

		if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			if ( pSoldier->bActive )
			{
				if( pSoldier->bVehicleID == iId )
				{
					return( pSoldier );
				}
			}
		}
	}

	// Didn't find the vehicle
	return( 0 );
}


void SetUpArmorForVehicle( UINT8 ubID )
{
/*
	// set up the internal and external armor for vehicles
	for( iCounter = 0; iCounter < NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE; iCounter++ )
	{
		pVehicleList[ ubID ].sInternalHitLocations[ iCounter ] = sVehicleInternalOrigArmorValues[ pVehicleList[ ubID ].ubVehicleType ][ iCounter ];
	}


	for( iCounter = 0; iCounter < NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE; iCounter++ )
	{
		pVehicleList[ ubID ].sExternalArmorLocationsStatus[ iCounter ] = 100;
	}
	*/

	// for armour type, store the index into the armour table itself
	pVehicleList[ ubID ].sArmourType = Item[ gNewVehicle[ pVehicleList[ ubID ].ubVehicleType ].sNewVehicleArmourType ].ubClassIndex;

	return;
}

void AdjustVehicleAPs( SOLDIERTYPE *pSoldier, INT16 *pubPoints )
{
	INT16 pubDeducations = 0;
	INT32 iCounter = 0;

	// Flugente: this is stupid. Why is the AP calculation for vehicles based on stats? Those are machines, dammit.
	// and why a hardcoded bonus of 35?
	// I've changed this - now a car always gets max AP, which can then be reduced by damage it received.
	//(*pubPoints) += 35;
	(*pubPoints) = gubMaxActionPoints[pSoldier->ubBodyType];

	// check for state of critcals


	// handle for each engine crit
	pubDeducations += pVehicleList[ pSoldier->bVehicleID ].sCriticalHits[ ENGINE_HIT_LOCATION ] * COST_PER_ENGINE_CRIT;

	// handle each tire
	for( iCounter = RF_TIRE_HIT_LOCATION; iCounter < LR_TIRE_HIT_LOCATION; iCounter++ )
	{
		if( pVehicleList[ pSoldier->bVehicleID ].sCriticalHits[ iCounter ] )
		{
			pubDeducations += COST_PER_TIRE_HIT;
		}
	}

	// make sure we don't go too far
	if( pubDeducations > (*pubPoints) )
	{
		pubDeducations = (*pubPoints);
	}

	// now deduct pts
	(*pubPoints) -= pubDeducations;
}



BOOLEAN SaveVehicleInformationToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	PathStPtr	pTempPathPtr;
	UINT32		uiNodeCount=0;
	UINT8		cnt;
	VEHICLETYPE	TempVehicle;
	UINT8			ubPassengerCnt=0;

	//Save the number of elements
	FileWrite( hFile, &ubNumberOfVehicles, sizeof( UINT8 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT8 ) )
	{
		return( FALSE );
	}

	//loop through all the vehicles and save each one
	for( cnt=0; cnt< ubNumberOfVehicles; cnt++ )
	{
		//save if the vehicle spot is valid
		FileWrite( hFile, &pVehicleList[cnt].fValid, sizeof( BOOLEAN ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( BOOLEAN ) )
		{
			return( FALSE );
		}

		if( pVehicleList[cnt].fValid )
		{
			// copy the node into the temp vehicle buffer ( need to do this because we cant save the pointers
			// to the soldier, therefore save the soldier ubProfile
			memcpy( &TempVehicle, &pVehicleList[cnt], sizeof( VEHICLETYPE ) );

			//loop through the passengers
			for( ubPassengerCnt=0; ubPassengerCnt<10; ubPassengerCnt++)
			{
		TempVehicle.pPassengers[ ubPassengerCnt ] = ( SOLDIERTYPE * )NO_PROFILE;

				//if there is a passenger here
				if( pVehicleList[cnt].pPassengers[ ubPassengerCnt ] )
				{
					//assign the passengers profile to the struct
					// ! The pointer to the passenger is converted to a byte so that the Id of the soldier can be saved.
					// ! This means that the pointer contains a bogus pointer, but a real ID for the soldier.
					// ! When reloading, this bogus pointer is converted to a byte to contain the id of the soldier so
					// ! we can get the REAL pointer to the soldier
					TempVehicle.pPassengers[ ubPassengerCnt ] = ( SOLDIERTYPE * ) pVehicleList[cnt].pPassengers[ ubPassengerCnt ]->ubProfile;
				}
			}

			//save the vehicle info
			FileWrite( hFile, &TempVehicle, sizeof( VEHICLETYPE ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( VEHICLETYPE ) )
			{
				return( FALSE );
			}
			//count the number of nodes in the vehicles path
			uiNodeCount=0;
			pTempPathPtr = pVehicleList[cnt].pMercPath;
			while( pTempPathPtr )
			{
				uiNodeCount++;
				pTempPathPtr = pTempPathPtr->pNext;
			}

			//Save the number of nodes
			FileWrite( hFile, &uiNodeCount, sizeof( UINT32 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT32 ) )
			{
				return( FALSE );
			}

			//save all the nodes
			pTempPathPtr = pVehicleList[cnt].pMercPath;
			while( pTempPathPtr )
			{
				//Save the node
				FileWrite( hFile, pTempPathPtr, sizeof( PathSt ), &uiNumBytesWritten );
				if( uiNumBytesWritten != sizeof( PathSt ) )
				{
					return( FALSE );
				}

				pTempPathPtr = pTempPathPtr->pNext;
			}
		}
	}

	return( TRUE );
}


BOOLEAN LoadVehicleInformationFromSavedGameFile( HWFILE hFile, UINT32 uiSavedGameVersion )
{
	UINT32		uiNumBytesRead;
	UINT32		uiTotalNodeCount=0;
	UINT8			cnt;
	UINT32		uiNodeCount=0;
	PathSt		*pPath=NULL;
	UINT8			ubPassengerCnt=0;
	PathSt		*pTempPath;

	//Clear out th vehicle list
	ClearOutVehicleList( );

	//Load the number of elements
	FileRead( hFile, &ubNumberOfVehicles, sizeof( UINT8 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT8 ) )
	{
		return( FALSE );
	}

	if( ubNumberOfVehicles != 0 )
	{
		//allocate memory to hold the vehicle list
		pVehicleList = (VEHICLETYPE *)MemAlloc( sizeof( VEHICLETYPE ) * ubNumberOfVehicles );
		if( pVehicleList == NULL )
			return( FALSE );
		memset( pVehicleList, 0, sizeof( VEHICLETYPE ) * ubNumberOfVehicles );


		//loop through all the vehicles and load each one
		for( cnt=0; cnt< ubNumberOfVehicles; cnt++ )
		{
			//Load if the vehicle spot is valid
			FileRead( hFile, &pVehicleList[cnt].fValid, sizeof( BOOLEAN ), &uiNumBytesRead );
			if( uiNumBytesRead != sizeof( BOOLEAN ) )
			{
				return( FALSE );
			}

			if( pVehicleList[cnt].fValid )
			{
				//load the vehicle info
				FileRead( hFile, &pVehicleList[cnt], sizeof( VEHICLETYPE ), &uiNumBytesRead );
				if( uiNumBytesRead != sizeof( VEHICLETYPE ) )
				{
					return( FALSE );
				}

				//
				// Build the passenger list
				//

				//loop through all the passengers
				for(ubPassengerCnt=0; ubPassengerCnt<10; ubPassengerCnt++)
				{
			if ( uiSavedGameVersion < 86 )
			{
					if( pVehicleList[cnt].pPassengers[ubPassengerCnt] != 0 )
					{
						// ! The id of the soldier was saved in the passenger pointer.	The passenger pointer is converted back
						// ! to a UINT8 so we can get the REAL pointer to the soldier.
						pVehicleList[cnt].pPassengers[ubPassengerCnt] = FindSoldierByProfileID( (UINT8)(pVehicleList[cnt].pPassengers[ ubPassengerCnt ]), FALSE );
						}
			}
			else
			{
					if( pVehicleList[cnt].pPassengers[ubPassengerCnt] != ( SOLDIERTYPE * )NO_PROFILE )
					{
						// ! The id of the soldier was saved in the passenger pointer.	The passenger pointer is converted back
						// ! to a UINT8 so we can get the REAL pointer to the soldier.
						pVehicleList[cnt].pPassengers[ubPassengerCnt] = FindSoldierByProfileID( (UINT8)(pVehicleList[cnt].pPassengers[ ubPassengerCnt ]), FALSE );
						}
			else
			{
				pVehicleList[cnt].pPassengers[ubPassengerCnt] = NULL;
			}
			}
				}


				//Load the number of nodes
				FileRead( hFile, &uiTotalNodeCount, sizeof( UINT32 ), &uiNumBytesRead );
				if( uiNumBytesRead != sizeof( UINT32 ) )
				{
					return( FALSE );
				}

				if( uiTotalNodeCount != 0 )
				{
					pPath = NULL;

					pVehicleList[cnt].pMercPath = NULL;

					//loop through each node
					for( uiNodeCount=0; uiNodeCount<uiTotalNodeCount; uiNodeCount++ )
					{
						//allocate memory to hold the vehicle path
						pTempPath = (PathSt *)MemAlloc( sizeof( PathSt ) );
						if( pTempPath == NULL )
							return( FALSE );
						memset( pTempPath, 0, sizeof( PathSt ) );

						//Load all the nodes
						FileRead( hFile, pTempPath, sizeof( PathSt ), &uiNumBytesRead );
						if( uiNumBytesRead != sizeof( PathSt ) )
						{
							return( FALSE );
						}


						//
						//Setup the pointer info
						//

						if( pVehicleList[cnt].pMercPath == NULL )
							pVehicleList[cnt].pMercPath = pTempPath;


						//if there is a previous node
						if( pPath != NULL )
						{
							pPath->pNext = pTempPath;

							pTempPath->pPrev = pPath;
						}
						else
							pTempPath->pPrev = NULL;


						pTempPath->pNext = NULL;

						pPath = pTempPath;
					}
				}
				else
				{
					pVehicleList[cnt].pMercPath = NULL;
				}
			}

			// WANNE: This should make savegames before the externalized vehicles compatible.
			if (pVehicleList[cnt].ubVehicleType != pVehicleList[cnt].ubProfileID)
				pVehicleList[cnt].ubVehicleType = pVehicleList[cnt].ubProfileID;
			
		}
	}
	return( TRUE );
}


void SetVehicleSectorValues( INT32 iVehId, UINT8 ubSectorX, UINT8 ubSectorY )
{
	pVehicleList[ iVehId ].sSectorX = ubSectorX;
	pVehicleList[ iVehId ].sSectorY = ubSectorY;

	gMercProfiles[ pVehicleList[ iVehId ].ubProfileID ].sSectorX = ubSectorX;
	gMercProfiles[ pVehicleList[ iVehId ].ubProfileID ].sSectorY = ubSectorY;

}

void UpdateAllVehiclePassengersGridNo( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter, iId;
	SOLDIERTYPE *pPassenger;

	// If not a vehicle, ignore!
	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		return;
	}

	iId = pSoldier->bVehicleID;

	// Loop through passengers and update each guy's position
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			pPassenger = pVehicleList[ iId ].pPassengers[ iCounter ];

			INT8 bOffsetX = gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].VehicleSeats[ iCounter ].bOffsetX;
			INT8 bOffsetY = gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].VehicleSeats[ iCounter ].bOffsetY;

			FLOAT dXOffset = 0;
			FLOAT dYOffset = 0;
			INT8 dX, dY;
			INT8 bDiagonalX = 0;
			INT8 bDiagonalY = 0;

			switch( pSoldier->ubBodyType )
			{
				case ELDORADO:
				case HUMVEE:
				case JEEP:
					bDiagonalY = -1;
			}

			switch( pSoldier->ubDirection )
			{
				case 0:
					dYOffset = -10 * ( bOffsetY + bDiagonalY );
					dXOffset = 10 * bOffsetX;
					break;
				case 1:
					if( bOffsetX < 0 )
					{
						dXOffset = - 10 + 10 * bOffsetY;
						dYOffset = - 10 * bOffsetY;
					}
					else
					{
						dYOffset = 10 + - 10 * bOffsetY;
						dXOffset = 10 * bOffsetY;
					}
					break;
				case 2:
					dXOffset = 10 * ( bOffsetY + bDiagonalY);
					dYOffset = 10 * bOffsetX;
					break;
				case 3:
					dX = bOffsetY;
					dY = bOffsetY;
					if( bOffsetX > 0 )
						dX -= 1;
					else if( bOffsetX < 0 )
						dY -= 1;
					dXOffset = 10 * dX;
					dYOffset = 10 * dY;
					break;
				case 4:
					dYOffset = 10 * ( bOffsetY + bDiagonalY );
					dXOffset = -10 * bOffsetX;
					break;
				case 5:
					if( bOffsetX < 0 )
					{
						dXOffset = 10 + - 10 * bOffsetY;
						dYOffset = 10 * bOffsetY;
					}
					else
					{
						dYOffset = - 10 + 10 * bOffsetY;
						dXOffset = - 10 * bOffsetY;
					}
					break;
				case 6:
					dXOffset = -10 * ( bOffsetY + bDiagonalY );
					dYOffset = -10 * bOffsetX;
					break;
				case 7:				
					dX = bOffsetY;
					dY = bOffsetY;
					if( bOffsetX > 0 )
						dX -= 1;
					else if( bOffsetX < 0 )
						dY -= 1;
					dXOffset = - 10 * dX;
					dYOffset = - 10 * dY;
					break;
			}
			// Set gridno.....
			pPassenger->EVENT_SetSoldierPosition( pSoldier->dXPos + dXOffset, pSoldier->dYPos + dYOffset);
		}
	}
}


BOOLEAN SaveVehicleMovementInfoToSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten = 0;
	//Save all the vehicle movement id's
//	FileWrite( hFile, gubVehicleMovementGroups, sizeof( INT8 ) * 5, &uiNumBytesWritten );
//	if( uiNumBytesWritten != sizeof( INT8 ) * 5 )
//	{
//		return(FALSE);
//	}
	for (int cnt=0; cnt<5; ++cnt) {
		FileWrite( hFile, &gubVehicleMovementGroups[cnt], sizeof( INT8 ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( INT8 ) ) {
			return(FALSE);
		}
	}

	return( TRUE );
}


BOOLEAN LoadVehicleMovementInfoFromSavedGameFile( HWFILE hFile )
{
	GROUP		*pGroup	=	NULL;
	UINT32	uiNumBytesRead=0;
	//Load in the Squad movement id's
//	FileRead( hFile, gubVehicleMovementGroups, sizeof( INT8 ) * 5, &uiNumBytesRead );
//	if( uiNumBytesRead != sizeof( INT8 ) * 5 )
//	{
//		return(FALSE);
//	}
	for (int cnt=0; cnt<5; ++cnt) {
		FileRead( hFile, &gubVehicleMovementGroups[cnt], sizeof( INT8 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( INT8 ) ) {
			return(FALSE);
		}
	}

	for( int cnt = 5; cnt <	MAX_VEHICLES; cnt++ )
	{
		// create mvt groups
		gubVehicleMovementGroups[ cnt ] = CreateNewVehicleGroupDepartingFromSector( 1, 1, cnt );

		// Set persistent....
		pGroup = GetGroup( gubVehicleMovementGroups[ cnt ] );
		pGroup->fPersistant = TRUE;
	}

	return( TRUE );
}


BOOLEAN NewSaveVehicleMovementInfoToSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten = 0;
	//Save all the vehicle movement id's
//	FileWrite( hFile, gubVehicleMovementGroups, sizeof( INT8 ) * MAX_VEHICLES, &uiNumBytesWritten );
//	if( uiNumBytesWritten != sizeof( INT8 ) * MAX_VEHICLES )
//	{
//		return(FALSE);
//	}
	for (int cnt=0; cnt<MAX_VEHICLES; ++cnt) {
		FileWrite( hFile, &gubVehicleMovementGroups[cnt], sizeof( INT8 ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( INT8 ) ) {
			return(FALSE);
		}
	}

	return( TRUE );
}


BOOLEAN NewLoadVehicleMovementInfoFromSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead=0;
	//Load in the Squad movement id's
//	FileRead( hFile, gubVehicleMovementGroups, sizeof( INT8 ) * MAX_VEHICLES, &uiNumBytesRead );
//	if( uiNumBytesRead != sizeof( INT8 ) * MAX_VEHICLES )
//	{
//		return(FALSE);
//	}
	for (int cnt=0; cnt<MAX_VEHICLES; ++cnt) {
		FileRead( hFile, &gubVehicleMovementGroups[cnt], sizeof( INT8 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( INT8 ) ) {
			return(FALSE);
		}
	}

	return( TRUE );
}


BOOLEAN OKUseVehicle( UINT8 ubProfile )
{
	if ( ubProfile == PROF_HUMMER )
	{
		return( CheckFact( FACT_OK_USE_HUMMER, NO_PROFILE ) );
	}
	else if ( ubProfile == PROF_ICECREAM )
	{
		return( CheckFact( FACT_OK_USE_ICECREAM, NO_PROFILE ) );
	}
	else if ( ubProfile == PROF_HELICOPTER )
	{
		// don't allow mercs to get inside vehicle if it's grounded (enemy controlled, Skyrider owed money, etc.)
		return( CanHelicopterFly() );
	}
	else
	{
		return( TRUE );
	}
}



void TeleportVehicleToItsClosestSector( INT32 iVehicleId, UINT8 ubGroupID )
{
	GROUP *pGroup = NULL;
	UINT32 uiTimeToNextSector;
	UINT32 uiTimeToLastSector;
	INT16 sPrevX, sPrevY, sNextX, sNextY;


	pGroup = GetGroup( ubGroupID );
	Assert( pGroup );

	Assert( pGroup->uiTraverseTime != -1 );
	Assert( ( pGroup->uiTraverseTime > 0 ) && ( pGroup->uiTraverseTime != 0xffffffff ) );

	Assert( pGroup->uiArrivalTime >= GetWorldTotalMin() );
	uiTimeToNextSector = pGroup->uiArrivalTime - GetWorldTotalMin();

	Assert( pGroup->uiTraverseTime >= uiTimeToNextSector );
	uiTimeToLastSector = pGroup->uiTraverseTime - uiTimeToNextSector;

	if ( uiTimeToNextSector >= uiTimeToLastSector )
	{
		// go to the last sector
		sPrevX = pGroup->ubNextX;
		sPrevY = pGroup->ubNextY;

		sNextX = pGroup->ubSectorX;
		sNextY = pGroup->ubSectorY;
	}
	else
	{
		// go to the next sector
		sPrevX = pGroup->ubSectorX;
		sPrevY = pGroup->ubSectorY;

		sNextX = pGroup->ubNextX;
		sNextY = pGroup->ubNextY;
	}

	// make it arrive immediately, not eventually (it's driverless)
	SetGroupArrivalTime( pGroup, GetWorldTotalMin() );

	// change where it is and where it's going, then make it arrive there.	Don't check for battle
	PlaceGroupInSector( ubGroupID, sPrevX, sPrevY, sNextX, sNextY, 0, FALSE );
}


void AddVehicleFuelToSave( )
{
	INT32		 iCounter;
	SOLDIERTYPE	*pVehicleSoldier = NULL;

	for( iCounter = 0; iCounter < ubNumberOfVehicles ; iCounter++ )
	{
		// might have an empty slot
		if( pVehicleList[ iCounter ].fValid )
	{
	 // get the vehicle soldiertype
	 pVehicleSoldier = GetSoldierStructureForVehicle( iCounter );

	 if( pVehicleSoldier )
		{
		// Init fuel!
		pVehicleSoldier->sBreathRed = 10000;
		pVehicleSoldier->bBreath	= 100;
		}
	}
	}
}


BOOLEAN CanSoldierDriveVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId, BOOLEAN fIgnoreAsleep )
{
	Assert( pSoldier );

	if ( pSoldier->bAssignment != VEHICLE )
	{
		// not in a vehicle!
		return( FALSE );
	}

	if ( pSoldier->iVehicleId != iVehicleId )
	{
		// not in THIS vehicle!
		return( FALSE );
	}

	if ( iVehicleId == iHelicopterVehicleId )
	{
		// only Skyrider can pilot the helicopter
		return( FALSE );
	}

	if( !fIgnoreAsleep && ( pSoldier->flags.fMercAsleep == TRUE ) )
	{
		// asleep!
		return( FALSE );
	}

	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) || AM_AN_EPC( pSoldier ) )
	{
		// vehicles, robot, and EPCs can't drive!
		return (FALSE);
	}

	// too wounded to drive
	if( pSoldier->stats.bLife < OKLIFE )
	{
		return (FALSE);
	}

	// too tired to drive
	if( pSoldier->bBreathMax <= BREATHMAX_ABSOLUTE_MINIMUM )
	{
		return (FALSE);
	}

	// anv: only driver on driver's seat can drive
	INT8 bSeatIndex = GetSeatIndexFromSoldier( pSoldier );
	if( bSeatIndex == (-1) || gNewVehicle[ pVehicleList[ iVehicleId ].ubVehicleType ].VehicleSeats[bSeatIndex].fDriver == FALSE )
	{
		return (FALSE);
	}

	// yup, he could drive this vehicle
	return( TRUE );
}


BOOLEAN SoldierMustDriveVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId, BOOLEAN fTryingToTravel )
{
	Assert( pSoldier );

	// error check
	if( ( iVehicleId >= ubNumberOfVehicles ) || ( iVehicleId < 0 ) )
	{
		return ( FALSE );
	}

	// if vehicle is not going anywhere, then nobody has to be driving it!
	// need the path length check in case we're doing a test while actually in a sector even though we're moving!
	if ( !fTryingToTravel && ( !pVehicleList[ iVehicleId ].fBetweenSectors ) && ( GetLengthOfPath( pVehicleList[ iVehicleId ].pMercPath ) == 0 ) )
	{
		return( FALSE );
	}

	// if he CAN drive it (don't care if he is currently asleep)
	if ( CanSoldierDriveVehicle( pSoldier, iVehicleId, TRUE ) )
	{
		// and he's the ONLY one aboard who can do so
		if ( OnlythisCanDriveVehicle( pSoldier, iVehicleId ) )
		{
			return( TRUE );
		}
		// (if there are multiple possible drivers, than the assumption is that this guy ISN'T driving, so he CAN sleep)
	}

	return( FALSE );
}



BOOLEAN OnlythisCanDriveVehicle( SOLDIERTYPE *pthis, INT32 iVehicleId )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	for( iCounter = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
	{
		// get the current soldier
		pSoldier = &Menptr[ iCounter ];

		// skip checking THIS soldier, we wanna know about everyone else
		if ( pSoldier == pthis )
		{
			continue;
		}

		if( pSoldier->bActive )
		{
			// don't count mercs who are asleep here
			if ( CanSoldierDriveVehicle( pSoldier, iVehicleId, FALSE ) )
			{
				// this guy can drive it, too
				return( FALSE );
			}
		}
	}

	// you're da man!
	return( TRUE );
}



BOOLEAN IsSoldierInThisVehicleSquad( SOLDIERTYPE *pSoldier, INT8 bSquadNumber )
{
	INT32 iVehicleId;
	SOLDIERTYPE *pVehicleSoldier;


	Assert( pSoldier );
	Assert( ( bSquadNumber >= 0 ) && ( bSquadNumber < NUMBER_OF_SQUADS ) );

	// not in a vehicle?
	if( pSoldier->bAssignment != VEHICLE )
	{
		return( FALSE );
	}

	// get vehicle ID
	iVehicleId = pSoldier->iVehicleId;

	// if in helicopter
	if ( iVehicleId == iHelicopterVehicleId )
	{
		// they don't get a squad #
		return( FALSE );
	}

	pVehicleSoldier = GetSoldierStructureForVehicle( iVehicleId );
	Assert( pVehicleSoldier );

	// check squad vehicle is on
	if ( pVehicleSoldier->bAssignment != bSquadNumber )
	{
		return( FALSE );
	}


	// yes, he's in a vehicle assigned to this squad
	return( TRUE );
}


INT8 GetSeatIndexFromSoldier( SOLDIERTYPE *pSoldier )
{
	INT32 iVehicleId;
	SOLDIERTYPE *pVehicleSoldier;
	INT32 iCounter;

	iVehicleId = pSoldier->iVehicleId;

	if( iVehicleId == (-1) )
	{
		return( -1 );
	}

	pVehicleSoldier = GetSoldierStructureForVehicle( iVehicleId );

	if( pVehicleSoldier == NULL )
	{
		return( -1 );
	}

	// Loop through passengers
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iVehicleId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iVehicleId ].pPassengers[ iCounter ] == pSoldier )
		{
			return( iCounter );
		}
	}

	return( -1 );
}

SOLDIERTYPE*	PickRandomPassengerFromVehicle( SOLDIERTYPE *pSoldier )
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
//	UINT8	ubMercsInSector[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ] = { 0 };
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	INT32 iCounter, iId;

	// If not a vehicle, ignore!
	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		return( NULL );
	}

	iId = pSoldier->bVehicleID;

	// Loop through passengers and update each guy's position
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
	{
		if( pVehicleList[ iId ].pPassengers[ iCounter ] != NULL )
		{
			ubMercsInSector[ ubNumMercs ] = (UINT8)iCounter;
			ubNumMercs++;
		}
	}

	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		// If we are air raid, AND red exists somewhere...
	return( pVehicleList[ iId ].pPassengers[ ubChosenMerc ] );
	}

	return( NULL );
}

BOOLEAN DoesVehicleHaveAnyPassengers( INT32 iVehicleID )
{
	if( !GetNumberInVehicle( iVehicleID ) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN DoesVehicleGroupHaveAnyPassengers( GROUP *pGroup )
{
	INT32 iVehicleID;

	iVehicleID = GivenMvtGroupIdFindVehicleId( pGroup->ubGroupID );
	if( iVehicleID == -1 )
	{
		#ifdef JA2BETAVERSION
			AssertMsg( iVehicleID != -1, "DoesVehicleGroupHaveAnyPassengers() for vehicle group.	Invalid iVehicleID." );
		#endif
		return FALSE;
	}

	return DoesVehicleHaveAnyPassengers( iVehicleID );
}

BOOLEAN	IsHelicopterInSector(INT16 sX, INT16 sY)
{
	if ( pVehicleList[ iHelicopterVehicleId ].sSectorX == sX && pVehicleList[ iHelicopterVehicleId ].sSectorY == sY )
	{
		return TRUE;
	}

	return FALSE;
}
