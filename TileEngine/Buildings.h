#ifndef BUILDING_H

#define BUILDING_H

#include "worlddef.h"
// for what it's worth, 2 bytes, we use roof climb spots as 1-based
// so the 0th entry is always 0 and can be compared with (and not equal)
// NOWHERE or any other location
#define MAX_CLIMBSPOTS_PER_BUILDING 21

// similarly for buildings, only we really want 0 to be invalid index
#define NO_BUILDING 0
#define MAX_BUILDINGS 31

// WANNE: If this is defined, this fixes the bug, that soldiers do not
// climb on roofs anymore if ASTAR is disabled.
// The "Bug" was introduced in Revision 1534 (TileEngine\Buildings.cpp)
// Overhaul wanted to improve the climbing on buildings, but introduced this bug
// If the VANILLA_BUILDING_CLIMBING is not defined, it uses Overhaul code,
// where enemies do not climb on buildings!!

// PS: I did not looked in Overhaul's Building code was causes the problem.
// If someone has time to debug and fixes the problem, Overhaul's code should 
// be used -> Disable the define!
#define VANILLA_BUILDING_CLIMBING


typedef struct BUILDING
{
	INT16			sUpClimbSpots[MAX_CLIMBSPOTS_PER_BUILDING];
	INT16 		sDownClimbSpots[MAX_CLIMBSPOTS_PER_BUILDING];
	UINT8			ubNumClimbSpots;
} BUILDING;

extern UINT8 gubBuildingInfo[ WORLD_MAX ];

BOOLEAN InBuilding( INT16 sGridNo );
BUILDING * GenerateBuilding( INT16 sDesiredSpot );
BUILDING * FindBuilding( INT16 sGridNo );
void GenerateBuildings( void );
INT16 FindClosestClimbPoint( SOLDIERTYPE *pSoldier, INT16 sStartGridNo, INT16 sDesiredGridNo, BOOLEAN fClimbUp );
BOOLEAN SameBuilding( INT16 sGridNo1, INT16 sGridNo2 );

#endif