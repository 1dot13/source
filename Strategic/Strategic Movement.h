#ifndef __STRATEGIC_MOVEMENT_H
#define __STRATEGIC_MOVEMENT_H

#include "types.h"
#include "Soldier Control.h"
#include "FileMan.h"


enum //enemy intentions,
{
	NO_INTENTIONS,			//enemy intentions are undefined.
	PURSUIT,						//enemy group has spotted a player group and is pursuing them.	If they lose the player group, they
											//will get reassigned.
	STAGING,						//enemy is prepare to assault a town sector, but doesn't have enough troops.
	PATROL,							//enemy is moving around determining safe areas.
	REINFORCEMENTS,			//enemy group has intentions to fortify position at final destination.
	ASSAULT,						//enemy is ready to fight anything they encounter.
	NUM_ENEMY_INTENTIONS
};

enum //move types
{
	ONE_WAY,						//from first waypoint to last, deleting each waypoint as they are reached.
	CIRCULAR,						//from first to last, recycling forever.
	ENDTOEND_FORWARDS,	//from first to last -- when reaching last, change to backwards.
	ENDTOEND_BACKWARDS	//from last to first -- when reaching first, change to forwards.
};

enum
{
	NORTH_STRATEGIC_MOVE,
	EAST_STRATEGIC_MOVE,
	SOUTH_STRATEGIC_MOVE,
	WEST_STRATEGIC_MOVE,
	THROUGH_STRATEGIC_MOVE
};

#define FOOT_TRAVEL_TIME		89

//This structure contains all of the information about a group moving in the strategic
//layer.	This includes all troops, equipment, and waypoints, and location.
//NOTE:	This is used for groups that are initiating a movement to another sector.
typedef struct WAYPOINT
{
	UINT8 x;											//sector x position of waypoint
	UINT8 y;											//sector y position of waypoint
	struct WAYPOINT *next;				//next waypoint in list
}WAYPOINT;

#define PG_INDIVIDUAL_MERGED		0x01

typedef struct PLAYERGROUP
{
	UINT8 ubProfileID;						//SAVE THIS VALUE ONLY.	The others are temp (for quick access)
	UINT8 ubID;										//index in the Menptr array
	SOLDIERTYPE *pSoldier;				//direct access to the soldier pointer
	UINT8 bFlags;									//flags referring to individual player soldiers
	struct PLAYERGROUP *next;			//next player in list
}PLAYERGROUP;


typedef struct ENEMYGROUP
{
	UINT8 ubNumTroops;						//number of regular troops in the group
	UINT8 ubNumElites;						//number of elite troops in the group
	UINT8 ubNumAdmins;						//number of administrators in the group
	UINT8 ubLeaderProfileID;			//could be Mike, maybe the warden... someone new, but likely nobody.
	UINT8 ubPendingReinforcements;//This group is waiting for reinforcements before attacking or attempting to fortify newly aquired sector.
	UINT8 ubAdminsInBattle;				//number of administrators in currently in battle.
	UINT8 ubIntention;						//the type of group this is:	patrol, assault, spies, etc.
	UINT8 ubTroopsInBattle;				//number of soldiers currently in battle.
	UINT8 ubElitesInBattle;				//number of elite soldiers currently in battle.
	// WDS - New AI
	UINT8 ubNumTanks;
	UINT8 ubTanksInBattle;
	UINT8 ubNumJeeps;
	UINT8 ubJeepsInBattle;

	// Flugente: number of turncoats
	UINT8	ubNumAdmins_Turncoat;
	UINT8	ubNumTroops_Turncoat;
	UINT8	ubNumElites_Turncoat;

	UINT8 ubNumRobots;						//number of enemy robots in the group
	UINT8 ubRobotsInBattle;				//number of enemy robots currently in battle.

	INT8	bPadding[11];
}ENEMYGROUP;

//NOTE:	ALL FLAGS ARE CLEARED WHENEVER A GROUP ARRIVES IN A SECTOR, OR ITS WAYPOINTS ARE
//		DELETED!!!
#define GROUPFLAG_SIMULTANEOUSARRIVAL_APPROVED		0x00000001
#define GROUPFLAG_SIMULTANEOUSARRIVAL_CHECKED		0x00000002
//I use this flag when traversing through a list to determine which groups meet whatever conditions,
//then add this marker flag.	The second time I traverse the list, I simply check for this flag,
//apply my modifications to the group, and remove the flag.	If you decide to use it, make sure the 
//flag is cleared.
#define GROUPFLAG_MARKER							0x00000004
//Set whenever a group retreats from battle.	If the group arrives in the next sector and enemies are there
//retreat will not be an option.
#define GROUPFLAG_JUST_RETREATED_FROM_BATTLE		0x00000008
#define GROUPFLAG_HIGH_POTENTIAL_FOR_AMBUSH			0x00000010
#define GROUPFLAG_GROUP_ARRIVED_SIMULTANEOUSLY		0x00000020

#define GROUPFLAGS_TODELETE							0x0000003F

// Flugente: these flags determine whether a group will always be visible on the map
#define GROUPFLAG_KNOWN_POSITION					0x00000040
#define GROUPFLAG_KNOWN_DIRECTION					0x00000080
#define GROUPFLAG_KNOWN_NUMBER						0x00000100

typedef struct GROUP
{
	BOOLEAN fDebugGroup;					//for testing purposes -- handled differently in certain cases.
	UINT8 usGroupTeam;							// the team that controls this group - 0 for OUR_TEAM, 1 for ENEMY_TEAM..
	BOOLEAN fVehicle;							//vehicle controlled group?
	BOOLEAN fPersistant;					//This flag when set prevents the group from being automatically deleted when it becomes empty.
	UINT8 ubGroupID;							//the unique ID of the group (used for hooking into events and SOLDIERTYPE)
	UINT8 ubGroupSize;						//total number of individuals in the group.
	UINT8 ubSectorX, ubSectorY;		//last/curr sector occupied
	UINT8 ubSectorZ;
	UINT8 ubNextX, ubNextY;				//next sector destination
	UINT8 ubPrevX, ubPrevY;				//prev sector occupied (could be same as ubSectorX/Y)
	UINT8 ubOriginalSector;				//sector where group was created.
	BOOLEAN fBetweenSectors;			//set only if a group is between sector.
	UINT8 ubMoveType;							//determines the type of movement (ONE_WAY, CIRCULAR, ENDTOEND, etc.)
	UINT8 ubNextWaypointID;				//the ID of the next waypoint
	UINT8 ubFatigueLevel;					//the fatigue level of the weakest member in group
	UINT8 ubRestAtFatigueLevel;		//when the group's fatigue level <= this level, they will rest upon arrival at next sector.
	UINT8 ubRestToFatigueLevel;		//when resting, the group will rest until the fatigue level reaches this level.
	UINT32 uiArrivalTime;					//the arrival time in world minutes that the group will arrive at the next sector.
	UINT32 uiTraverseTime;				//the total traversal time from the previous sector to the next sector.
	BOOLEAN fRestAtNight;					//set when the group is permitted to rest between 2200 and 0600 when moving
	BOOLEAN fWaypointsCancelled;	//set when groups waypoints have been removed.
	WAYPOINT *pWaypoints;					//a list of all of the waypoints in the groups movement.
	UINT8 ubTransportationMask;		//the mask combining all of the groups transportation methods.
	UINT32 uiFlags;								//various conditions that apply to the group
	UINT8 ubCreatedSectorID;			//used for debugging strategic AI for keeping track of the sector ID a group was created in.
	UINT8 ubSectorIDOfLastReassignment;	//used for debuggin strategic AI.	Records location of any reassignments.
	INT8 bPadding[29];						//***********************************************//

	union
	{
		PLAYERGROUP *pPlayerList;		//list of players in the group
		ENEMYGROUP *pEnemyGroup;		//a structure containing general enemy info
	};
	struct GROUP *next;						//next group
}GROUP;

extern GROUP *gpGroupList;


//General utility functions
void RemoveAllGroups();
GROUP* GetGroup( UINT8 ubGroupID );

//Remove a group from the list.	This removes all of the waypoints as well as the members of the group.
//Calling this function doesn't position them in a sector.	It is up to you to do that.	The event system
//will automatically handle their updating as they arrive in sectors.
void RemoveGroup( UINT8 ubGroupID );//takes a groupID
void RemovePGroup( GROUP *pGroup ); //same function, but takes a GROUP*
void RemoveGroupIdFromList( UINT8 ubId );

//Clears a groups waypoints.	This is necessary when sending new orders such as different routes.
void RemoveGroupWaypoints( UINT8 ubGroupID );
void RemovePGroupWaypoints( GROUP *pGroup ); //same function, but takes a GROUP*

//Player grouping functions
//.........................
//Creates a new player group, returning the unique ID of that group.	This is the first
//step before adding waypoints and members to the player group.
UINT8 CreateNewPlayerGroupDepartingFromSector( UINT8 ubSectorX, UINT8 ubSectorY );
//Allows you to add or remove players from the group.
BOOLEAN AddPlayerToGroup( UINT8 ubGroupID, SOLDIERTYPE *pSoldier);

BOOLEAN RemovePlayerFromGroup( UINT8 ubGroupID, SOLDIERTYPE *pSoldier );
BOOLEAN RemovePlayerFromPGroup( GROUP *pGroup, SOLDIERTYPE *pSoldier );
BOOLEAN RemoveAllPlayersFromGroup( UINT8 ubGroupId );
BOOLEAN RemoveAllPlayersFromPGroup( GROUP *pGroup );

// create a vehicle group, it is by itself, 
UINT8 CreateNewVehicleGroupDepartingFromSector( UINT8 ubSectorX, UINT8 ubSectorY , UINT32 uiVehicleId );


//Appends a waypoint to the end of the list.	Waypoint MUST be on the
//same horizontal xor vertical level as the last waypoint added.
BOOLEAN AddWaypointToGroup( UINT8 ubGroupID, UINT8 ubSectorX, UINT8 ubSectorY );
BOOLEAN AddWaypointToPGroup( GROUP *pGroup, UINT8 ubSectorX, UINT8 ubSectorY );
//Same, but uses a plain sectorID (0-255)
BOOLEAN AddWaypointIDToGroup( UINT8 ubGroupID, UINT8 ubSectorID );
BOOLEAN AddWaypointIDToPGroup( GROUP *pGroup, UINT8 ubSectorID );
//Same, but uses a strategic sectorID
BOOLEAN AddWaypointStrategicIDToGroup( UINT8 ubGroupID, UINT32 uiSectorID );
BOOLEAN AddWaypointStrategicIDToPGroup( GROUP *pGroup, UINT32 uiSectorID );

//Allows you to change any group's orders based on movement type, and when to rest
BOOLEAN SetGroupPatrolParameters( UINT8 ubGroupID, UINT8 ubRestAtFL, UINT8 ubRestToFL, BOOLEAN fRestAtNight );

//Enemy grouping functions -- private use by the strategic AI.
//............................................................
GROUP* CreateNewEnemyGroupDepartingFromSector( UINT32 uiSector, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumRobots, UINT8 ubNumTanks, UINT8 ubNumJeeps );

GROUP* CreateNewMilitiaGroupDepartingFromSector( UINT32 uiSector, UINT8& arusNumAdmins, UINT8& arusNumTroops, UINT8& arusNumElites );

#ifdef JA2UB
GROUP* CreateNewEnemyGroupDepartingFromSectorUsingZLevel( UINT32 uiSector, UINT8 ubSectorZ, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks );
#endif

//ARRIVALCALLBACK -- None of these functions should be called directly.
//...............
//This is called whenever any group arrives in the next sector (player or enemy)
//This function will first check to see if a battle should start, or if they
//aren't at the final destination, they will move to the next sector. 
void GroupArrivedAtSector( UINT8 ubGroupID, BOOLEAN fCheckForBattle, BOOLEAN fNeverLeft );
//Calculates and posts an event to move the group to the next sector.
void InitiateGroupMovementToNextSector( GROUP *pGroup );
void CalculateNextMoveIntention( GROUP *pGroup );


// set current sector of the group..used for player controlled mercs
void SetGroupSectorValue( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, UINT8 ubGroupID );

void SetEnemyGroupSector( GROUP *pGroup, UINT8 ubSectorID );

// set groups next sector x,y value..used ONLY for teleporting groups
void SetGroupNextSectorValue( INT16 sSectorX, INT16 sSectorY, UINT8 ubGroupID );


// calculate the eta time in world total mins of this group
INT32 CalculateTravelTimeOfGroup( GROUP *pGroup );
INT32 CalculateTravelTimeOfGroupId( UINT8 ubId );

INT32 GetSectorMvtTimeForGroup( UINT8 ubSector, UINT8 ubDirection, GROUP *pGroup );

UINT8 PlayerMercsInSector( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );
UINT8 PlayerGroupsInSector( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );

// is the player greoup with this id in motion
BOOLEAN PlayerIDGroupInMotion( UINT8 ubID );

// is this player group in motion?
BOOLEAN PlayerGroupInMotion( GROUP *pGroup );

// find if a path exists?
INT32 GetTravelTimeForFootTeam( UINT8 ubSector, UINT8 ubDirection );

// get travel time for this group?
INT32 GetTravelTimeForGroup( UINT8 ubSector, UINT8 ubDirection, UINT8 ubGroup );

// get number of mercs between sectors
BOOLEAN PlayersBetweenTheseSectors( INT16 sSource, INT16 sDest, INT32 *iCountEnter, INT32 *iCountExit, BOOLEAN *fAboutToArriveEnter );

// set this groups waypoints as cancelled
void SetWayPointsAsCanceled( UINT8 ubGroupID );

void SetGroupPrevSectors( UINT8 ubGroupID, UINT8 ubX, UINT8 ubY );

void MoveAllGroupsInCurrentSectorToSector( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );

// this is for groups that are between sectors, nothing else
// get group position
void GetGroupPosition( UINT8 *ubNextX, UINT8 *ubNextY, UINT8 *ubPrevX, UINT8 *ubPrevY, UINT32 *uiTraverseTime, UINT32 *uiArriveTime, UINT8 ubGroupId );

// set groups postion
void SetGroupPosition( UINT8 ubNextX, UINT8 ubNextY, UINT8 ubPrevX, UINT8 ubPrevY, UINT32 uiTraverseTime, UINT32 uiArriveTime, UINT8 ubGroupId );

//Save the strategic movemnet Group paths to the saved game file
BOOLEAN SaveStrategicMovementGroupsToSaveGameFile( HWFILE hFile );

//Load the strategic movement Group paths from the saved game file
BOOLEAN LoadStrategicMovementGroupsFromSavedGameFile( HWFILE hFile );

// check members of mvt group, if any are bleeding, complain before moving
void CheckMembersOfMvtGroupAndComplainAboutBleeding( SOLDIERTYPE *pSoldier );

void HandleArrivalOfReinforcements( GROUP *pGroup );

void PlanSimultaneousGroupArrivalCallback( UINT8 bMessageValue );

//When groups meet up, then it is possible that they may join up.	This only happens if
//the groups were separated because of singular tactical/exitgrid traversal, and the timing
//expires.
BOOLEAN AttemptToMergeSeparatedGroups( GROUP *pGroup, BOOLEAN fDecrementTraversals );

//Called when all checks have been made for the group (if possible to retreat, etc.)	This function
//blindly determines where to move the group.
void RetreatGroupToPreviousSector( GROUP *pGroup );

GROUP* FindMovementGroupInSector( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubTeam );

BOOLEAN GroupAtFinalDestination( GROUP *pGroup );

// find the travel time between waypts for this group
INT32 FindTravelTimeBetweenWaypoints( WAYPOINT * pSource, WAYPOINT * pDest,	GROUP *pGroup );

BOOLEAN GroupReversingDirectionsBetweenSectors( GROUP *pGroup, UINT8 ubSectorX, UINT8 ubSectorY, BOOLEAN fBuildingWaypoints );
BOOLEAN GroupBetweenSectorsAndSectorXYIsInDifferentDirection( GROUP *pGroup, UINT8 ubSectorX, UINT8 ubSectorY );

void RemoveGroupFromList( GROUP *pGroup );

WAYPOINT *GetFinalWaypoint( GROUP *pGroup );

void ResetMovementForNonPlayerGroupsInLocation( UINT8 ubSectorX, UINT8 ubSectorY );
void ResetMovementForNonPlayerGroup( GROUP *pGroup );

//Determines if any particular group WILL be moving through a given sector given it's current
//position in the route and TREATS the pGroup->ubMoveType as ONE_WAY EVEN IF IT ISN'T.	If the 
//group is currently IN the sector, or just left the sector, it will return FALSE.
BOOLEAN GroupWillMoveThroughSector( GROUP *pGroup, UINT8 ubSectorX, UINT8 ubSectorY );


//Vehicle fuel support functions
INT16 CalculateFuelCostBetweenSectors( UINT8 ubSectorID1, UINT8 ubSectorID2 );
BOOLEAN VehicleHasFuel( SOLDIERTYPE *pSoldier );
INT16 VehicleFuelRemaining( SOLDIERTYPE *pSoldier );
BOOLEAN SpendVehicleFuel( SOLDIERTYPE* pSoldier, INT16 sFuelSpent );
void ReportVehicleOutOfGas( INT32 iVehicleID, UINT8 ubSectorX, UINT8 ubSectorY );


void RandomizePatrolGroupLocation( GROUP *pGroup );

BOOLEAN InitStrategicMovementCosts();

BOOLEAN SectorIsImpassable( INT16 sSector, UINT8 aDirection = THROUGH_STRATEGIC_MOVE );

void PlaceGroupInSector( UINT8 ubGroupID, INT16 sPrevX, INT16 sPrevY, INT16 sNextX, INT16 sNextY, INT8 bZ, BOOLEAN fCheckForBattle );

void SetGroupArrivalTime( GROUP *pGroup, UINT32 uiArrivalTime );

void PlayerGroupArrivedSafelyInSector( GROUP *pGroup, BOOLEAN fCheckForNPCs );

BOOLEAN DoesPlayerExistInPGroup( UINT8 ubGroupID, SOLDIERTYPE *pSoldier );

BOOLEAN GroupHasInTransitDeadOrPOWMercs( GROUP *pGroup );

BOOLEAN ScoutIsPresentInSquad( INT16 ubSectorNumX, INT16 ubSectorNumY ); // added by SANDRO

// Flugente: concealed mercs are in a different sector level, but we pretend they aren't... so we gain scout vision if a concealed merc is 'present'
BOOLEAN ConcealedMercInSector( INT16 ubSectorNumX, INT16 ubSectorNumY, BOOLEAN aScoutsOnly );

void CheckCombatInSectorDueToUnusualEnemyArrival( UINT8 aTeam, INT16 sX, INT16 sY, INT8 sZ );

void GetInfoFromGroupsInSector( INT16 sSectorX, INT16 sSectorY, UINT8 ubTeam, BOOLEAN& arDetection, BOOLEAN& arCount, BOOLEAN& arDirection );

#endif
