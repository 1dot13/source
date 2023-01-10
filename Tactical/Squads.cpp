	#include "Types.h"
	#include "Squads.h"
	#include "Strategic Pathing.h"
	#include "Strategicmap.h"
	#include "Faces.h"
	#include "Strategic Movement.h"
	#include "Assignments.h"
	#include "OverHead.h"
	#include "Interface.h"
	#include "Vehicles.h"
	#include "Map Screen Helicopter.h"
	#include "Soldier Profile.h"
	#include "Debug.h"
	#include "jascreens.h"
	#include "screenids.h"
	#include "Soldier macros.h"
	#include "GameSettings.h"
#include <vector>


typedef struct
{
	INT16	uiID;						// The soldiers ID
	INT16 sPadding[ 5 ];
//	INT8	bSquadValue;		// The squad id

} SAVE_SQUAD_INFO_STRUCT;

// squad array
SOLDIERTYPE *Squad[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];

// list of dead guys for squads...in id values->-1 means no one home
INT16 sDeadMercs[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];

// the movement group ids
INT8 SquadMovementGroups[ NUMBER_OF_SQUADS ];

BOOLEAN fExitingVehicleToSquad = FALSE;

extern void CheckForAndAddMercToTeamPanel( SOLDIERTYPE *pSoldier );
extern void RemoveAllPlayersFromSlot( );
extern INT32 iHelicopterVehicleId;


// update current merc selected in tactical
void UpdateCurrentlySelectedMerc( SOLDIERTYPE *pSoldier, INT8 bSquadValue );

// is the passed squad between sectors?
void RebuildSquad( INT8 bSquadValue );

BOOLEAN AddDeadCharacterToSquadDeadGuys( SOLDIERTYPE *pSoldier, INT32 iSquadValue );
BOOLEAN IsDeadGuyOnAnySquad( SOLDIERTYPE *pSoldier );

INT32 iCurrentTacticalSquad = FIRST_SQUAD;

void InitSquads( void )
{
	// init the squad lists to NULL ptrs.
	INT32 iCounterB = 0;
	INT32 iCounter =0;
	GROUP		*pGroup	=	NULL;

	// null each list of ptrs.
	for( iCounter = 0; iCounter <	NUMBER_OF_SQUADS; iCounter++ )
	{
		for( iCounterB =0; iCounterB < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounterB++ )
		{

		// squad, soldier
		Squad[ iCounter ][ iCounterB ]= NULL;

		}

		// create mvt groups
		SquadMovementGroups[ iCounter ] = CreateNewPlayerGroupDepartingFromSector( 1, 1 );

		// Set persistent....
		pGroup = GetGroup( SquadMovementGroups[ iCounter ] );
		pGroup->fPersistant = TRUE;

	}

	memset( sDeadMercs, -1, sizeof( INT16 ) * NUMBER_OF_SQUADS * NUMBER_OF_SOLDIERS_PER_SQUAD );

	return;
}

BOOLEAN IsThisSquadFull( INT8 bSquadValue )
{
	//SQUAD10 FIX:  Number of taken slots should not exceed 6/8/10 depending on resolution
	if ( NumberOfPeopleInSquad( bSquadValue ) >= gGameOptions.ubSquadSize )
	{
		return( true );
	}

	return ( false );

/*
	INT32 iCounter = 0;

	// run through entries in the squad list, make sure there is a free entry
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		// check this slot
		if( Squad[ bSquadValue ][ iCounter ] == NULL )
		{
			// a free slot found - not full
			return( FALSE );
		}
	}

	// no free slots - it's full
	return( TRUE );
*/
}

INT8 GetFirstEmptySquad( void )
{
	UINT8 ubCounter = 0;

	for( ubCounter = 0; ubCounter < NUMBER_OF_SQUADS; ubCounter++ )
	{
		if( SquadIsEmpty( ubCounter ) == TRUE )
		{
			// empty squad, return value
			return( ubCounter );
		}
	}

	// not found - none are completely empty (shouldn't ever happen!)
	Assert( FALSE );
	return( -1 );
}

void FixOversizedSquadsInSector( void )
{
	// SQUAD10: Fix any squads in current tactical map with more people than allowed at current resolution
	INT32 iCountSquad = 0;
	INT8  iCountSoldier = 0;
	INT8  iPeopleInSquad = 0;
	
	// loop through all squads
	for( iCountSquad = 0; iCountSquad < NUMBER_OF_SQUADS; iCountSquad++ )
	{
		// but deal only with squads in current sector
		if( IsSquadOnCurrentTacticalMap( iCountSquad ) == TRUE )
		{
			// found a squad in current sector...check size
			iPeopleInSquad = NumberOfPeopleInSquad( iCountSquad );
			while ( iPeopleInSquad > gGameOptions.ubSquadSize )
			{
				// oversized squad found -- move some people to another squad
				// 
				// loop backwards through the squad in order to remove the last person
				for( iCountSoldier = NUMBER_OF_SOLDIERS_PER_SQUAD - 1; iCountSoldier >= 0 ; iCountSoldier-- )
				{
					// if squad is still oversized, and we have found a squad member
					if( (Squad[ iCountSquad ][ iCountSoldier ] != NULL) && (iPeopleInSquad > gGameOptions.ubSquadSize) )
					{
						// ... attempt to move him to another squad						
						if ( AddCharacterToAnySquad( Squad[ iCountSquad ][ iCountSoldier ] ) == TRUE )
						{
							// success: we got rid of one person in this squad
							iPeopleInSquad--;
						}
					}
				}
				// we've looped through all members off the squad
				// reset iPeopleInSquad as there is nothing more we can do
				iPeopleInSquad = 0;
			}
		}
	}
}

BOOLEAN AddCharacterToSquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{
	INT8 bCounter =0;
	INT16 sX, sY;
	INT8	bZ;
//	BOOLEAN fBetweenSectors = FALSE;
	GROUP	*pGroup;
	BOOLEAN fNewSquad;


	// add character to squad...return success or failure
	// run through list of people in squad, find first free slo

	if( fExitingVehicleToSquad )
	{
		return( FALSE );
	}


	// ATE: If any vehicle exists in this squad AND we're not set to
	// a driver or or passenger, when return false
	if ( DoesVehicleExistInSquad( bSquadValue ) )
	{
		// We're not allowing anybody to go on a vehicle if they are not passengers!
		// NB: We obviously need to make sure that REAL passengers have their
		// flags set before adding them to a squad!
		if ( !( pCharacter->flags.uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER | SOLDIER_VEHICLE ) ) )
		{
			return( FALSE );
		}
	}



	// if squad is on the move, can't add someone
	if( IsThisSquadOnTheMove( bSquadValue ) == TRUE )
	{
		// nope, go away now
		return( FALSE );
	}



	for( bCounter =0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; bCounter++ )
	{
		// check if on current squad and current slot?
		if( Squad[ bSquadValue ][ bCounter ] == pCharacter )
		{
			// 'successful of sorts, if there, then he's 'added'
			return ( TRUE );
		}
	}

	if ( NumberOfPeopleInSquad( bSquadValue ) >= gGameOptions.ubSquadSize )
	{
		// SQUAD10: too many people in this squad
		return( FALSE );
	}

	for( bCounter =0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; bCounter++ )
	{
		// free slot, add here
		if( Squad[ bSquadValue ][ bCounter ] == NULL )
		{
			// check if squad empty, if not check sector x,y,z are the same as this guys
			if( SquadIsEmpty( bSquadValue ) == FALSE )
			{
				GetLocationOfSquad( &sX, &sY, &bZ, bSquadValue );

				// if not same, return false
				if( ( pCharacter->sSectorX != sX ) || ( pCharacter->sSectorY != sY ) ||( pCharacter->bSectorZ != bZ) )
				{
					return ( FALSE );
				}

				// remove them
				RemoveCharacterFromSquads( pCharacter );

//				fBetweenSectors =	Squad[ bSquadValue ][ 0 ]->flags.fBetweenSectors;
			}
			else
			{
				// remove them
				RemoveCharacterFromSquads( pCharacter );
			}

/*
			if( fBetweenSectors == TRUE )
			{
				pCharacter->flags.fBetweenSectors = TRUE;
			}
*/

			// copy path of squad to this char
			CopyPathOfSquadToCharacter( pCharacter, bSquadValue );

			// check if old mvt group
			if( pCharacter->ubGroupID != 0 )
			{
				// in valid group, remove from that group
				RemovePlayerFromGroup(	pCharacter->ubGroupID , pCharacter );

				// character not on a reserved group
				if( ( pCharacter->bAssignment >= ON_DUTY ) && ( pCharacter->bAssignment != VEHICLE ) )
				{
					// get the group from the character
					pGroup = GetGroup( pCharacter->ubGroupID );

					// if valid group, delete it
					if( pGroup )
					{
						RemoveGroupFromList( pGroup );
					}
				}

			}




			if( ( pCharacter->bAssignment == VEHICLE ) && ( pCharacter->iVehicleId == iHelicopterVehicleId ) && ( pCharacter->iVehicleId != -1 ) )
			{
				// if creating a new squad from guys exiting the chopper
				fNewSquad = SquadIsEmpty( bSquadValue );

				RemoveSoldierFromHelicopter( pCharacter );

				AddPlayerToGroup( SquadMovementGroups[ bSquadValue ], pCharacter	);
				SetGroupSectorValue( pCharacter->sSectorX, pCharacter->sSectorY, pCharacter->bSectorZ, SquadMovementGroups[ bSquadValue ] );
				pCharacter->ubGroupID = SquadMovementGroups[ bSquadValue ];

				// if we've just started a new squad
				if ( fNewSquad )
				{
					// set mvt group for
					GROUP *pGroup;

					// grab group
					pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );
					Assert( pGroup );

					if( pGroup )
					{
						// set where it is and where it's going, then make it arrive there.	Don't check for battle
						PlaceGroupInSector( SquadMovementGroups[ bSquadValue ], pGroup->ubPrevX, pGroup->ubPrevY, pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubSectorZ, FALSE );
					}
				}
			}
			else if( ( pCharacter->bAssignment == VEHICLE ) && ( pCharacter->iVehicleId != -1 ) )
			{
				fExitingVehicleToSquad = TRUE;
				// remove from vehicle
				TakeSoldierOutOfVehicle( pCharacter );
				fExitingVehicleToSquad = FALSE;


				AddPlayerToGroup( SquadMovementGroups[ bSquadValue ], pCharacter	);
				SetGroupSectorValue( pCharacter->sSectorX, pCharacter->sSectorY, pCharacter->bSectorZ, SquadMovementGroups[ bSquadValue ] );
				pCharacter->ubGroupID = SquadMovementGroups[ bSquadValue ];
			}
			else if( IsVehicle(pCharacter) )
			{
				INT32 iCounter = 0;
				for( iCounter = 0; iCounter < ubNumberOfVehicles ; iCounter++ )
				{
					if(pVehicleList[ iCounter ].ubProfileID == pCharacter->ubProfile)
						break;
				}
				//AddPlayerToGroup( pVehicleList[iCounter].ubMovementGroup, pCharacter	);
				SetGroupSectorValue( pCharacter->sSectorX, pCharacter->sSectorY, pCharacter->bSectorZ, pVehicleList[iCounter].ubMovementGroup );
				pCharacter->ubGroupID = pVehicleList[iCounter].ubMovementGroup;
			}
			else
			{
				AddPlayerToGroup( SquadMovementGroups[ bSquadValue ], pCharacter	);
				SetGroupSectorValue( pCharacter->sSectorX, pCharacter->sSectorY, pCharacter->bSectorZ, SquadMovementGroups[ bSquadValue ] );
				pCharacter->ubGroupID = SquadMovementGroups[ bSquadValue ];
			}


			// assign here
			Squad[ bSquadValue ][ bCounter ] = pCharacter;

			if( ( pCharacter->bAssignment != bSquadValue ) )
			{
				// check to see if we should wake them up
				if ( pCharacter->flags.fMercAsleep )
				{
					// try to wake him up
					SetMercAwake( pCharacter, FALSE, FALSE );
				}
				SetTimeOfAssignmentChangeForMerc( pCharacter );
			}

			// set squad value
			ChangeSoldiersAssignment( pCharacter, bSquadValue );
			if ( pCharacter->bOldAssignment < ON_DUTY )
			{
				pCharacter->bOldAssignment = bSquadValue;
			}

			// if current tactical squad...update panel
			if( NumberOfPeopleInSquad( ( INT8 )iCurrentTacticalSquad ) == 0 )
			{
				SetCurrentSquad( bSquadValue, TRUE );
			}

			// silversurfer: We need to sort the squad here. Otherwise it will have a random sort order when we force exit a vehicle in an enemy sector.
			SortSquadByID(bSquadValue);

			if( bSquadValue == ( INT8 ) iCurrentTacticalSquad )
			{
				CheckForAndAddMercToTeamPanel( pCharacter );
			}

			if ( pCharacter->ubID == gusSelectedSoldier )
			{
				SetCurrentSquad( bSquadValue, TRUE );
			}

			if ( SPY_LOCATION( pCharacter->bOldAssignment ) )
			{
				pCharacter->usSoldierFlagMask2 |= SOLDIER_CONCEALINSERTION;
			}

			return ( TRUE );
		}
	}

	return ( FALSE );
}


// find the first slot we can fit the guy in
BOOLEAN AddCharacterToAnySquad( SOLDIERTYPE *pCharacter )
{
	// add character to any squad, if character is assigned to a squad, returns TRUE
	INT8 bCounter = 0;
	INT8 bFirstEmptySquad = -1;


	// remove them from current squad
	RemoveCharacterFromSquads( pCharacter );

	// first look for a compatible NON-EMPTY squad (don't start new squad if we don't have to)
	for( bCounter = 0; bCounter < NUMBER_OF_SQUADS; bCounter++ ) {
		if( !SquadIsEmpty( bCounter ) )
		{
			if( AddCharacterToSquad( pCharacter, bCounter ) == TRUE )
			{
				return ( TRUE );
			}
		}
		else
		{
			if ( bFirstEmptySquad == -1 )
			{
				bFirstEmptySquad = bCounter;
			}
		}
	}

	// no non-empty compatible squads were found

	// try the first empty one (and there better be one)
	if ( bFirstEmptySquad != -1 )
	{
		if( AddCharacterToSquad( pCharacter, bFirstEmptySquad ) == TRUE )
		{
			return ( TRUE );
		}
	}

	// should never happen!
	//Assert( FALSE );

	return( FALSE );
}

// find the first slot we can fit the guy in
INT8 AddCharacterToUniqueSquad( SOLDIERTYPE *pCharacter )
{
	// add character to any squad, if character is assigned to a squad, returns TRUE
	INT8 bCounter = 0;

	// check if character on a squad

		// remove them
	RemoveCharacterFromSquads( pCharacter );

	for( bCounter = 0; bCounter < NUMBER_OF_SQUADS; bCounter++ )
	{
		if( SquadIsEmpty( bCounter ) == TRUE )
		{
			if( AddCharacterToSquad( pCharacter, bCounter ) == TRUE )
			{
				return ( bCounter );
			}
		}
	}

	return( -1 );
}

// WDS - make number of mercenaries, etc. be configurable
void SortSquadByID( INT8 bSquadValue )
{
	for(unsigned idx1 = 0; idx1 < NUMBER_OF_SOLDIERS_PER_SQUAD-1; ++idx1) {
		if (Squad[bSquadValue][idx1]) {
			for(unsigned idx2 = idx1+1; idx2 < NUMBER_OF_SOLDIERS_PER_SQUAD; ++idx2) {
				if (Squad[bSquadValue][idx2] &&
					(Squad[bSquadValue][idx2]->ubID < Squad[bSquadValue][idx1]->ubID)) {
					SOLDIERTYPE *temp = Squad[bSquadValue][idx1];
					Squad[bSquadValue][idx1] = Squad[bSquadValue][idx2];
					Squad[bSquadValue][idx2] = temp;
				}
			}
		}
	}
}

BOOLEAN SquadIsEmpty( INT8 bSquadValue )
{
	// run through this squad's slots and find if they ALL are empty
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; iCounter++ )
	{
		if( Squad[ bSquadValue ][ iCounter ] != NULL )
		{
			return ( FALSE );
		}
	}

	return( TRUE );
}



// find and remove characters from any squad
BOOLEAN RemoveCharacterFromSquads( SOLDIERTYPE *pCharacter )
{
	INT32 iCounterA = 0;
	INT32 iCounter = 0;
	UINT8 ubGroupId = 0;
	// find character and remove.. check characters in all squads


	// squad?
	for( iCounterA = 0; iCounterA < NUMBER_OF_SQUADS ; iCounterA++ )
	{
		// slot?
		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; iCounter++ )
		{

			// check if on current squad and current slot?
			if( Squad[ iCounterA ][ iCounter ] == pCharacter )
			{


				// found and nulled
				Squad[ iCounterA ][ iCounter ] = NULL;

				// Release memory for his personal path, BUT DON'T CLEAR HIS GROUP'S PATH/WAYPOINTS (pass in groupID -1).
				// Just because one guy leaves a group is no reason to cancel movement for the rest of the group.
				pCharacter->pMercPath = ClearStrategicPathList( pCharacter->pMercPath, -1 );

				// remove character from mvt group
				RemovePlayerFromGroup( SquadMovementGroups[ iCounterA ], pCharacter	);

				// reset player mvt group id value
				pCharacter->ubGroupID = 0;

				if( ( pCharacter->flags.fBetweenSectors )&&( pCharacter->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
				{
					ubGroupId = CreateNewPlayerGroupDepartingFromSector( ( INT8 ) ( pCharacter->sSectorX ) , ( INT8 ) ( pCharacter->sSectorY ) );

					// assign to a group
					AddPlayerToGroup( ubGroupId, pCharacter );
				}

				RebuildSquad( ( INT8 )iCounterA );

				if( pCharacter->stats.bLife == 0 )
				{
					AddDeadCharacterToSquadDeadGuys( pCharacter, iCounterA );
				}

				//if we are not loading a saved game
				// OJW - 20090427 - fix bug leaving gamescree/tactical for MAIN_MENU
				if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) && guiCurrentScreen == GAME_SCREEN && guiPendingScreen != MAINMENU_SCREEN )
				{
					UpdateCurrentlySelectedMerc( pCharacter, ( INT8 )iCounterA );
				}

				return ( TRUE );
			}
		}
	}

	// not found
	return ( FALSE );
}

BOOLEAN RemoveCharacterFromASquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{

	INT32 iCounter =0, iCounterA = 0;

	// remove character from particular squad..return if successful
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; iCounter++ )
	{
		// check if on current squad and current slot?
		if( Squad[ bSquadValue ][ iCounter ] == pCharacter )
		{

			UpdateCurrentlySelectedMerc( pCharacter, bSquadValue );

			// found and nulled
			Squad[ bSquadValue ][ iCounter ] = NULL;

			// remove character from mvt group
			RemovePlayerFromGroup( SquadMovementGroups[ bSquadValue ], pCharacter );


			if( pCharacter->stats.bLife == 0 )
			{
				AddDeadCharacterToSquadDeadGuys( pCharacter, iCounterA );
			}

			RebuildSquad( bSquadValue );


			// found
			return ( TRUE );
		}
	}

	// not found
	return ( FALSE );
}



BOOLEAN IsCharacterInSquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{
	INT32 iCounter =0;
		// find character in particular squad..return if successful
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; iCounter++ )
	{
		// check if on current squad and current slot?
		if( Squad[ bSquadValue ][ iCounter ] == pCharacter )
		{
			// found
			return ( TRUE );
		}
	}

	// not found
	return ( FALSE );
}

INT8 SlotCharacterIsInSquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{
	INT8 bCounter =0;

	// find character in particular squad..return slot if successful, else -1
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; bCounter++ )
	{
		// check if on current squad and current slot?
		if( Squad[ bSquadValue ][ bCounter ] == pCharacter )
		{
			// found
			return ( bCounter );
		}
	}

	// not found
	return ( -1 );
}

INT8 SquadCharacterIsIn( SOLDIERTYPE *pCharacter )
{
	// returns which squad character is in, -1 if none found
	INT8 iCounterA =0, iCounter = 0;

	// squad?
	for( iCounterA = 0; iCounterA < NUMBER_OF_SQUADS ; iCounterA++ )
	{
		// slot?
		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; iCounter++ )
		{

			// check if on current squad and current slot?
			if( Squad[ iCounterA ][ iCounter ] == pCharacter )
			{
				// return value
				return ( iCounterA );
			}
		}
	}

	// return failure
	return ( -1 );
}

INT8 NumberOfPeopleInSquad( INT8 bSquadValue )
{
	INT8 bCounter = 0;
	INT8 bSquadCount = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( 0 );
	}

	// find number of characters in particular squad.
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; ++bCounter )
	{
		// valid slot?
		if( Squad[ bSquadValue ][ bCounter ] != NULL )
		{
			// yep
			bSquadCount++;
		}
	}

	// return number found
	return( bSquadCount );
}

INT8 NumberOfNonEPCsInSquad( INT8 bSquadValue )
{
	INT8 bCounter = 0;
	INT8 bSquadCount = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( 0 );
	}

	// find number of characters in particular squad.
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; ++bCounter )
	{
		// valid slot?
		if( Squad[ bSquadValue ][ bCounter ] != NULL && !AM_AN_EPC( Squad[ bSquadValue ][ bCounter ] ) )
		{
			// yep
			++bSquadCount;
		}
	}

	// return number found
	return( bSquadCount );
}

BOOLEAN IsRobotControllerInSquad( INT8 bSquadValue )
{
	INT8 bCounter = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( 0 );
	}

	// find number of characters in particular squad.
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; ++bCounter )
	{
		// valid slot?
		if ( ( Squad[ bSquadValue ][ bCounter ] != NULL ) && Squad[ bSquadValue ][ bCounter ]->ControllingRobot( ) )
		{
			// yep
			return( TRUE );
		}
	}

	// return number found
	return( FALSE );
}

BOOLEAN SectorSquadIsIn(INT8 bSquadValue, INT16 *sMapX, INT16 *sMapY, INT16 *sMapZ )
{
	// returns if there is anyone on the squad and what sector ( strategic ) they are in
	INT8 bCounter =0;

	Assert( bSquadValue < ON_DUTY );

	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; ++bCounter )
	{
		// if valid soldier, get current sector and return
		if( Squad[ bSquadValue ][ bCounter ] != NULL )
		{
			*sMapX = Squad[ bSquadValue ][ bCounter ]->sSectorX;
			*sMapY = Squad[ bSquadValue ][ bCounter ]->sSectorY;
			*sMapZ = ( INT16 )Squad[ bSquadValue ][ bCounter ]->bSectorZ;

			return ( TRUE );
		}
	}

	// return there is no squad
	return ( FALSE );
}


BOOLEAN CopyPathOfSquadToCharacter(	SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{
	// copy path from squad to character
	INT8 bCounter = 0;

	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; ++bCounter )
	{
		if( ( Squad[ bSquadValue ][ bCounter ] != pCharacter ) &&( Squad[ bSquadValue ][ bCounter ] != NULL ) )
		{
			// valid character, copy paths
			pCharacter->pMercPath = CopyPaths(	Squad[ bSquadValue ][ bCounter ]->pMercPath, pCharacter->pMercPath);

			// return success
			return ( TRUE );
		}
	}

	// return failure
	return ( FALSE );
}


BOOLEAN CopyPathOfCharacterToSquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue )
{
	// copy path of this character to members of squad
	BOOLEAN fSuccess = FALSE;
	INT8 bCounter =0;

	// anyone else on squad?
	if( NumberOfPeopleInSquad( bSquadValue ) < 2)
	{
		// nope

		// return failure
		return ( FALSE );
	}

	// copy each person on squad, skip this character
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; bCounter++ )
	{
		if( ( Squad[ bSquadValue ][ bCounter ] != pCharacter ) &&( Squad[ bSquadValue ][ bCounter ] != NULL ) )
		{
			// valid character, copy paths

			// first empty path
			Squad[ bSquadValue ][ bCounter ]->pMercPath = ClearStrategicPathList( Squad[ bSquadValue ][ bCounter ]->pMercPath, -1 );

			// then copy
			Squad[ bSquadValue ][ bCounter ]->pMercPath = CopyPaths( pCharacter->pMercPath, Squad[ bSquadValue ][ bCounter ]->pMercPath );

			// successful at least once
			fSuccess = TRUE;
		}
	}

	// return success?
	return ( fSuccess );
}



INT32 CurrentSquad( void )
{
	// returns which squad is current squad

	return( iCurrentTacticalSquad );
}

BOOLEAN SetCurrentSquad( INT32 iCurrentSquad, BOOLEAN fForce )
{
	// set the current tactical squad
	INT32 iCounter = 0;


	// ARM: can't call SetCurrentSquad() in mapscreen, it calls SelectSoldier(), that will initialize interface panels!!!
	// ATE: Adjusted conditions a bit ( sometimes were not getting selected )
	if ( guiCurrentScreen == LAPTOP_SCREEN || guiCurrentScreen == MAP_SCREEN )
	{
		return( FALSE );
	}

	// ATE; Added to allow us to have NO current squad
	if ( iCurrentSquad == NO_CURRENT_SQUAD )
	{
		// set current squad and return success
		iCurrentTacticalSquad = iCurrentSquad;

		// cleat list
		RemoveAllPlayersFromSlot( );

		// set all auto faces inactive
		SetAllAutoFacesInactive( );

		return( FALSE );
	}


	// check if valid value passed
	if( ( iCurrentSquad >= NUMBER_OF_SQUADS )||( iCurrentSquad < 0 ) )
	{
		// no
		return ( FALSE );
	}

	// check if squad is current
	if( iCurrentSquad == iCurrentTacticalSquad && !fForce )
	{
		return ( TRUE );
	}

	// set current squad and return success
	iCurrentTacticalSquad = iCurrentSquad;

	// cleat list
	RemoveAllPlayersFromSlot( );

	// set all auto faces inactive
	SetAllAutoFacesInactive( );

	if( iCurrentTacticalSquad != NO_CURRENT_SQUAD )
	{
		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
		{
			if(	Squad[ iCurrentTacticalSquad ][ iCounter ] != NULL )
			{
				// squad set, now add soldiers in
				CheckForAndAddMercToTeamPanel( Squad[ iCurrentTacticalSquad ][ iCounter ] );
			}
		}
		SOLDIERTYPE *pDeadSoldier = NULL;
		for (iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++)
		{
			if (sDeadMercs[iCurrentTacticalSquad][iCounter] != -1)
			{
				pDeadSoldier = FindSoldierByProfileID((UINT8)(sDeadMercs[iCurrentTacticalSquad][iCounter]), TRUE);

				if (pDeadSoldier)
				{
					// squad set, now add soldiers in
					CheckForAndAddMercToTeamPanel(pDeadSoldier);
				}
			}
		}
	}

	// check if the currently selected guy is on this squad, if not, get the first one on the new squad
	if ( gusSelectedSoldier != NOBODY )
	{
		if( Menptr[ gusSelectedSoldier ].bAssignment != iCurrentTacticalSquad )
		{
			// ATE: Changed this to FALSE for ackoledgement sounds.. sounds bad if just starting/entering sector..
			SelectSoldier( Squad[ iCurrentTacticalSquad ][ 0 ]->ubID, FALSE, TRUE );
		}
	}
	else
	{
		// ATE: Changed this to FALSE for ackoledgement sounds.. sounds bad if just starting/entering sector..
		SelectSoldier( Squad[ iCurrentTacticalSquad ][ 0 ]->ubID, FALSE, TRUE );
	}

	return ( TRUE );
}

void RebuildCurrentSquad( void )
{
	// rebuilds current squad to reset faces in tactical
	INT32 iCounter = 0;
	SOLDIERTYPE *pDeadSoldier = NULL;

	// check if valid value passed
	if( ( iCurrentTacticalSquad >= NUMBER_OF_SQUADS ) || ( iCurrentTacticalSquad < 0 ) )
	{
		// no
		return;
	}

	// set default squad..just inc ase we no longer have a valid squad
	SetDefaultSquadOnSectorEntry( TRUE );

	// cleat list
	RemoveAllPlayersFromSlot( );

	// set all auto faces inactive
	SetAllAutoFacesInactive( );

	gfPausedTacticalRenderInterfaceFlags = DIRTYLEVEL2;

	if( iCurrentTacticalSquad != NO_CURRENT_SQUAD )
	{
		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
		{
			if(	Squad[ iCurrentTacticalSquad ][ iCounter ] != NULL )
			{
				// squad set, now add soldiers in
				CheckForAndAddMercToTeamPanel( Squad[ iCurrentTacticalSquad ][ iCounter ] );
			}
		}

		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
		{
			if(	sDeadMercs[ iCurrentTacticalSquad ][ iCounter ] != -1 )
			{
				pDeadSoldier = FindSoldierByProfileID( ( UINT8 )( sDeadMercs[ iCurrentTacticalSquad ][ iCounter ] ), TRUE );

				if( pDeadSoldier )
				{
					// squad set, now add soldiers in
					CheckForAndAddMercToTeamPanel( pDeadSoldier );
				}
			}
		}
	}
}


void ExamineCurrentSquadLights( void )
{
	// rebuilds current squad to reset faces in tactical
	UINT8 ubLoop;

	// OK, we should add lights for any guy currently bInSector who is not bad OKLIFE...
	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( ; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++)
	{
		if ( MercPtrs[ ubLoop ]->bInSector && MercPtrs[ ubLoop ]->stats.bLife >= OKLIFE )
		{
			MercPtrs[ ubLoop ]->PositionSoldierLight(	);
		}
	}


	// check if valid value passed
	//if( ( iCurrentTacticalSquad >= NUMBER_OF_SQUADS ) || ( iCurrentTacticalSquad < 0 ) )
	//{
		// no
	//	return;
	//}

	//for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	//{
	//	if(	Squad[ iCurrentTacticalSquad ][ iCounter ] != NULL )
	//	{
	//		PositionSoldierLight( Squad[ iCurrentTacticalSquad ][ iCounter ] );
	//	}
	//}
}



BOOLEAN GetSoldiersInSquad( INT32 iCurrentSquad, SOLDIERTYPE *pSoldierArray[] )
{
	INT32 iCounter = 0;
	// will get the soldiertype pts for every merc in this squad

	// check if valid value passed
	if( ( iCurrentSquad >= NUMBER_OF_SQUADS ) || ( iCurrentSquad < 0 ) )
	{
		// no
		return ( FALSE );
	}

	// copy pts values over
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		pSoldierArray[ iCounter ] = Squad[ iCurrentSquad ][ iCounter ];
	}

	return ( TRUE );
}


BOOLEAN IsSquadOnCurrentTacticalMap( INT32 iCurrentSquad )
{
	INT32 iCounter = 0;
	// check to see if this squad is on the current map

	// check if valid value passed
	if( ( iCurrentSquad >= NUMBER_OF_SQUADS ) || ( iCurrentSquad < 0 ) )
	{
		// no
		return ( FALSE );
	}

	// go through memebrs of squad...if anyone on this map, return true
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if(	Squad[ iCurrentSquad ][ iCounter ] != NULL )
		{
			// ATE; Added more checks here for being in sector ( fBetweenSectors and SectorZ )
			if( ( Squad[ iCurrentSquad ][ iCounter ]->sSectorX == gWorldSectorX ) && ( Squad[ iCurrentSquad ][ iCounter ]->sSectorY == gWorldSectorY ) && Squad[ iCurrentSquad ][ iCounter ]->bSectorZ == gbWorldSectorZ && Squad[ iCurrentSquad ][ iCounter ]->flags.fBetweenSectors != TRUE )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


void SetDefaultSquadOnSectorEntry( BOOLEAN fForce )
{
	INT32 iCounter = 0;
	// check if selected squad is in current sector, if so, do nothing, if not...first first case that they are

	if( IsSquadOnCurrentTacticalMap( iCurrentTacticalSquad ) == TRUE )
	{
		// is in sector, leave
		return;
	}

	//otherwise...

	// find first squad availiable
	for( iCounter = 0; iCounter < NUMBER_OF_SQUADS; iCounter++ )
	{
		if( IsSquadOnCurrentTacticalMap( iCounter ) == TRUE )
		{
			// squad in sector...set as current
			SetCurrentSquad( iCounter, fForce );

			return;
		}
	}

	// If here, set to no current squad
	SetCurrentSquad( NO_CURRENT_SQUAD, FALSE );

	return;
}

INT32 GetLastSquadActive( void )
{
	// find id of last squad in the list with active mercs in it
	INT32 iCounter =0, iCounterB = 0;
	INT32 iLastSquad = 0;

	for( iCounter = 0; iCounter < NUMBER_OF_SQUADS; iCounter++ )
	{
		for( iCounterB = 0; iCounterB < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounterB++ )
		{
			if( Squad[ iCounter ][ iCounterB ] != NULL )
			{
				iLastSquad = iCounter;
			}
		}
	}

	return ( iLastSquad );
}


void GetSquadPosition( UINT8 *ubNextX, UINT8 *ubNextY, UINT8 *ubPrevX, UINT8 *ubPrevY, UINT32 *uiTraverseTime, UINT32 *uiArriveTime, UINT8 ubSquadValue )
{
	// grab the mvt group for this squad and find all this information

	if( SquadMovementGroups[ ubSquadValue ] == 0 )
	{
		*ubNextX = 0;
		*ubNextY = 0;
		*ubPrevX = 0;
		*ubPrevY = 0;
		*uiTraverseTime = 0;
		*uiArriveTime = 0;
		return;
	}

	// grab this squads mvt position
	GetGroupPosition( ubNextX, ubNextY, ubPrevX, ubPrevY, uiTraverseTime, uiArriveTime, SquadMovementGroups[ ubSquadValue ] );

	return;
}


void SetSquadPositionBetweenSectors( UINT8 ubNextX, UINT8 ubNextY, UINT8 ubPrevX, UINT8 ubPrevY, UINT32 uiTraverseTime, UINT32 uiArriveTime, UINT8 ubSquadValue	)
{
	// set mvt group position for squad for

	if( SquadMovementGroups[ ubSquadValue ] == 0 )
	{
		return;
	}
	SetGroupPosition(	ubNextX,	ubNextY,	ubPrevX,	ubPrevY,	uiTraverseTime,	uiArriveTime,	SquadMovementGroups[ ubSquadValue ] );

	return;
}



BOOLEAN SaveSquadInfoToSavedGameFile( HWFILE hFile )
{
	SAVE_SQUAD_INFO_STRUCT sSquadSaveStruct[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];
	UINT32	uiNumBytesWritten = 0;
	UINT32	uiSaveSize=0;
	//Reset the current squad info
	INT32 iCounterB = 0;
	INT32 iCounter =0;


	for( iCounter = 0; iCounter <	NUMBER_OF_SQUADS; iCounter++ )
	{
		for( iCounterB =0; iCounterB < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounterB++ )
		{
			if( Squad[ iCounter ][ iCounterB ] )
				sSquadSaveStruct[ iCounter ][ iCounterB ].uiID	= Squad[ iCounter ][ iCounterB ]->ubID;
			else
				sSquadSaveStruct[ iCounter ][ iCounterB ].uiID = -1;
		}
	}

	//Save the squad info to the Saved Game File
	uiSaveSize = sizeof( SAVE_SQUAD_INFO_STRUCT ) * NUMBER_OF_SQUADS * NUMBER_OF_SOLDIERS_PER_SQUAD;

	FileWrite( hFile, sSquadSaveStruct, uiSaveSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSaveSize )
	{
		return(FALSE);
	}


	//Save all the squad movement id's
	FileWrite( hFile, SquadMovementGroups, sizeof( INT8 ) * NUMBER_OF_SQUADS, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( INT8 ) * NUMBER_OF_SQUADS )
	{
		return(FALSE);
	}



	return( TRUE );
}



BOOLEAN LoadSquadInfoFromSavedGameFile( HWFILE hFile )
{
	SAVE_SQUAD_INFO_STRUCT sSquadSaveStruct[ NUMBER_OF_SQUADS ][ NUMBER_OF_SOLDIERS_PER_SQUAD ];
	UINT32	uiNumBytesRead=0;
	UINT32	uiSaveSize=0;

	//Reset the current squad info
	INT32 iCounterB = 0;
	INT32 iCounter =0;

	// null each list of ptrs.
	for( iCounter = 0; iCounter <	NUMBER_OF_SQUADS; iCounter++ )
	{
		for( iCounterB =0; iCounterB < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounterB++ )
		{
		// squad, soldier
		Squad[ iCounter ][ iCounterB ]= NULL;
		}
	}


	// Load in the squad info
	uiSaveSize = sizeof( SAVE_SQUAD_INFO_STRUCT ) * NUMBER_OF_SQUADS * NUMBER_OF_SOLDIERS_PER_SQUAD;

	FileRead( hFile, sSquadSaveStruct, uiSaveSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiSaveSize )
	{
		return(FALSE);
	}



	// Loop through the array loaded in
	for( iCounter = 0; iCounter <	NUMBER_OF_SQUADS; iCounter++ )
	{
		for( iCounterB =0; iCounterB < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounterB++ )
		{
			if( sSquadSaveStruct[ iCounter ][ iCounterB ].uiID != -1 )
				Squad[ iCounter ][ iCounterB ]	= &Menptr[ sSquadSaveStruct[ iCounter ][ iCounterB ].uiID ];
			else
				Squad[ iCounter ][ iCounterB ] = NULL;
		}
	}


	//Load in the Squad movement id's
	FileRead( hFile, SquadMovementGroups, sizeof( INT8 ) * NUMBER_OF_SQUADS, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( INT8 ) * NUMBER_OF_SQUADS )
	{
		return(FALSE);
	}

	return( TRUE );
}


void GetLocationOfSquad( INT16 *sX, INT16 *sY, INT8 *bZ, INT8 bSquadValue )
{
	// run through list of guys, once valid merc found, get his sector x and y and z
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if( Squad[ bSquadValue ][ iCounter ] )
		{
			// valid guy
			*sX = Squad[ bSquadValue ][ iCounter ]->sSectorX;
			*sY = Squad[ bSquadValue ][ iCounter ]->sSectorY;
			*bZ = Squad[ bSquadValue ][ iCounter ]->bSectorZ;
		}
	}

	return;
}

BOOLEAN IsThisSquadOnTheMove( INT8 bSquadValue )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if( Squad[ bSquadValue ][ iCounter ] )
		{
			return( Squad[ bSquadValue ][ iCounter ]->flags.fBetweenSectors );
		}
	}

	return( FALSE );
}

// rebuild this squad after someone has been removed, to 'squeeze' together any empty spots
void RebuildSquad( INT8 bSquadValue )
{
	INT32 iCounter = 0, iCounterB = 0;

	for( iCounterB = 0; iCounterB <NUMBER_OF_SOLDIERS_PER_SQUAD - 1; iCounterB++ )
	{
		for( iCounter = 0 ;iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD - 1; iCounter++ )
		{
			if( Squad[ bSquadValue ][ iCounter ] == NULL )
			{
				if( Squad[ bSquadValue ][ iCounter + 1 ] != NULL )
				{
					Squad[ bSquadValue ][ iCounter ] = Squad[ bSquadValue ][ iCounter + 1 ];
					Squad[ bSquadValue ][ iCounter + 1 ] = NULL;
				}
			}
		}
	}

	return;
}

void UpdateCurrentlySelectedMerc( SOLDIERTYPE *pSoldier, INT8 bSquadValue )
{
	UINT8	ubID;

	// if this squad is the current one and and the psoldier is the currently selected soldier, get rid of 'em
	if( bSquadValue != iCurrentTacticalSquad )
	{
		return;
	}

	// Are we the selected guy?
	if( gusSelectedSoldier == pSoldier->ubID )
	{
		ubID = FindNextActiveAndAliveMerc( pSoldier, FALSE, FALSE );

		if ( ubID != NOBODY && ubID != gusSelectedSoldier )
		{
			SelectSoldier( ubID, FALSE, FALSE );
		}
		else
		{
			gusSelectedSoldier = NOBODY;

		// ATE: Make sure we are in TEAM panel at this point!
			SetCurrentInterfacePanel( TEAM_PANEL );
		}
	}

	return;
}


BOOLEAN IsSquadInSector( SOLDIERTYPE *pSoldier, UINT8 ubSquad )
{

	if( pSoldier == NULL )
	{
		return( FALSE );
	}

	if( pSoldier->flags.fBetweenSectors == TRUE )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == IN_TRANSIT )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_MINIEVENT )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_REBELCOMMAND)
	{
		return( FALSE );
	}

	if( SquadIsEmpty( ubSquad ) == TRUE )
	{
		return( TRUE );
	}

	if( ( pSoldier->sSectorX != Squad[ ubSquad ][ 0 ]->sSectorX ) ||( pSoldier->sSectorY != Squad[ ubSquad ][ 0 ]->sSectorY ) || ( pSoldier->bSectorZ != Squad[ ubSquad ][ 0 ]->bSectorZ ) )
	{
		return( FALSE );
	}

	if( Squad[ ubSquad ][ 0 ]->flags.fBetweenSectors == TRUE )
	{
		return( FALSE );
	}


	return( TRUE );
}


BOOLEAN IsAnyMercOnSquadAsleep( UINT8 ubSquadValue )
{
	INT32 iCounter = 0;

	if( SquadIsEmpty( ubSquadValue ) == TRUE )
	{
		return( FALSE );
	}

	for( iCounter = 0 ;iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD - 1; iCounter++ )
	{
		if( Squad[ ubSquadValue ][ iCounter ] != NULL )
		{
			if( Squad[ ubSquadValue ][ iCounter ]->flags.fMercAsleep )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

BOOLEAN AddDeadCharacterToSquadDeadGuys( SOLDIERTYPE *pSoldier, INT32 iSquadValue )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pTempSoldier = NULL;



	// is dead guy in any squad
	if( IsDeadGuyOnAnySquad( pSoldier ) == TRUE )
	{
		return( TRUE );
	}

	// first find out if the guy is in the list
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		// valid soldier?
		if( sDeadMercs[ iSquadValue ][ iCounter ] != -1 )
		{
			pTempSoldier = FindSoldierByProfileID( ( UINT8 )( sDeadMercs[ iSquadValue ][ iCounter ] ), TRUE );

			if( pSoldier == pTempSoldier )
			{
				return( TRUE );
			}
		}
	}


	// now insert the guy
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		// valid soldier?
		if( sDeadMercs[ iSquadValue ][ iCounter ] != -1 )
		{
			// yep
			pTempSoldier = FindSoldierByProfileID( ( UINT8 )( sDeadMercs[ iSquadValue ][ iCounter ] ), TRUE );

			// valid soldier?
			if( pTempSoldier == NULL )
			{
				// nope
				sDeadMercs[ iSquadValue ][ iCounter ] = pSoldier->ubProfile;
				return( TRUE );
			}
		}
		else
		{
			// nope
			sDeadMercs[ iSquadValue ][ iCounter ] = pSoldier->ubProfile;
			return( TRUE );
		}
	}

	// no go
	return( FALSE );
}

BOOLEAN IsDeadGuyOnAnySquad( SOLDIERTYPE *pSoldier )
{
	INT32 iCounterA = 0, iCounter = 0;

	// squad?
	for( iCounterA = 0; iCounterA < NUMBER_OF_SQUADS; iCounterA++ )
	{
		// slot?
		for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
		{
			if( sDeadMercs[ iCounterA ][ iCounter ] == pSoldier->ubProfile )
			{
				return( TRUE );
			}
		}
	}

	return ( FALSE );
}

BOOLEAN IsDeadGuyInThisSquadSlot( INT8 bSlotId, INT8 bSquadValue , INT8 *bNumberOfDeadGuysSoFar )
{
	INT32 iCounter = 0, iCount = 0;

	// see if we have gone too far?
	if( bSlotId < *bNumberOfDeadGuysSoFar )
	{
		// reset
		*bNumberOfDeadGuysSoFar = 0;
	}

	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if( sDeadMercs[ bSquadValue ][ iCounter ] != -1 )
		{
			// not gone far enough yet
			if( *bNumberOfDeadGuysSoFar > iCounter )
			{
				iCount++;
			}
			else
			{
				// far enough, start checking
				bNumberOfDeadGuysSoFar++;

				return( TRUE );
			}
		}
	}

	return( FALSE );
}


BOOLEAN SoldierIsDeadAndWasOnSquad( SOLDIERTYPE *pSoldier, INT8 bSquadValue )
{
	INT32 iCounter = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( FALSE );
	}

	// check if guy is on squad
	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if( pSoldier->ubProfile == sDeadMercs[ bSquadValue ][ iCounter ] )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

BOOLEAN ResetDeadSquadMemberList( INT32 iSquadValue )
{
	memset( sDeadMercs[ iSquadValue ], -1, sizeof( INT16 ) * NUMBER_OF_SOLDIERS_PER_SQUAD );

	return( TRUE );
}


// this passed	soldier on the current squad int he tactical map
BOOLEAN IsMercOnCurrentSquad( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter = 0;

	// valid soldier?
	if( pSoldier == NULL )
	{
		// no
		return( FALSE );
	}

	// active grunt?
	if( pSoldier->bActive == FALSE )
	{
		// no
		return( FALSE );
	}

	// current squad valid?
	if( iCurrentTacticalSquad >= NUMBER_OF_SQUADS )
	{
		// no
		return( FALSE );
	}


	for( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
	{
		if( Squad[ iCurrentTacticalSquad ][ iCounter ] == pSoldier )
		{
			// found him
			return( TRUE );
		}
	}

	return( FALSE );
}

INT8 NumberOfPlayerControllableMercsInSquad( INT8 bSquadValue )
{
	SOLDIERTYPE *pSoldier;
	INT8 bCounter = 0;
	INT8 bSquadCount = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( 0 );
	}

	// find number of characters in particular squad.
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; bCounter++ )
	{

		// valid slot?
		if( Squad[ bSquadValue ][ bCounter ] != NULL )
		{
			// yep
			pSoldier = Squad[ bSquadValue ][ bCounter ] ;

			//Kris:	This breaks the CLIENT of this function, tactical traversal.	Do NOT check for EPCS or ROBOT here.
			//if ( !AM_AN_EPC( pSoldier ) && !AM_A_ROBOT( pSoldier ) &&
			if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				++bSquadCount;
			}
		}
	}

	// return number found
	return( bSquadCount );
}


BOOLEAN DoesVehicleExistInSquad( INT8 bSquadValue )
{
	SOLDIERTYPE *pSoldier;
	INT8 bCounter = 0;

	if( bSquadValue >= NO_CURRENT_SQUAD )
	{
		return( FALSE );
	}

	// find number of characters in particular squad.
	for( bCounter = 0; bCounter < NUMBER_OF_SOLDIERS_PER_SQUAD ; bCounter++ )
	{
		// valid slot?
		if( Squad[ bSquadValue ][ bCounter ] != NULL )
		{
			// yep
			pSoldier = Squad[ bSquadValue ][ bCounter ] ;

			// If we are an EPC or ROBOT, don't allow this
			if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				return( TRUE );
			}
		}
	}

	return(FALSE );
}

void CheckSquadMovementGroups( void )
{
	GROUP *		pGroup;

	for(INT32 iSquad = 0; iSquad < NUMBER_OF_SQUADS; ++iSquad )
	{
		pGroup = GetGroup( SquadMovementGroups[ iSquad ] );
		if ( pGroup == NULL )
		{
			// recreate group
			SquadMovementGroups[ iSquad ] = CreateNewPlayerGroupDepartingFromSector( 1, 1 );

			// Set persistent....
			pGroup = GetGroup( SquadMovementGroups[ iSquad ] );
			Assert( pGroup );
			pGroup->fPersistant = TRUE;
		}

		// Bob: propagate the group id to the squad members
		for (INT8 iSoldier = 0; iSoldier < NUMBER_OF_SOLDIERS_PER_SQUAD; iSoldier++) {
			if (Squad[iSquad][iSoldier] != NULL)
			{
				Squad[iSquad][iSoldier]->ubGroupID = pGroup->ubGroupID;
			}
		}
	}
}
