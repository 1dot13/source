#ifndef BUILDING_H

#define BUILDING_H

#include "worlddef.h"
// for what it's worth, 2 bytes, we use roof climb spots as 1-based
// so the 0th entry is always 0 and can be compared with (and not equal)
// NOWHERE or any other location
#define MAX_CLIMBSPOTS_PER_BUILDING 255

// similarly for buildings, only we really want 0 to be invalid index
#define NO_BUILDING 0
#define MAX_BUILDINGS 255

typedef struct BUILDING
{
	INT32			sUpClimbSpots[MAX_CLIMBSPOTS_PER_BUILDING];
	INT32 		sDownClimbSpots[MAX_CLIMBSPOTS_PER_BUILDING];
	UINT8			ubNumClimbSpots;
} BUILDING;

//extern UINT8 gubBuildingInfo[ WORLD_MAX ];
extern UINT8 * gubBuildingInfo;

BOOLEAN InBuilding( INT32 sGridNo );
BUILDING * GenerateBuilding( INT32 sDesiredSpot );
BUILDING * FindBuilding( INT32 sGridNo );
void GenerateBuildings( void );
INT32 FindClosestClimbPoint( SOLDIERTYPE *pSoldier, INT32 sStartGridNo, INT32 sDesiredGridNo, BOOLEAN fClimbUp );
BOOLEAN SameBuilding( INT32 sGridNo1, INT32 sGridNo2 );

#endif