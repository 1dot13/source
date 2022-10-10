#ifndef _SOLDIER_ADD_H
#define _SOLDIER_ADD_H

#include "Soldier Control.h"


// Finds a gridno given a sweet spot
// Returns a good direction too!
INT32	FindGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

// Ensures a good path.....
INT32 FindGridNoFromSweetSpotThroughPeople( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

// Returns a good sweetspot but not the swetspot!
INT32 FindGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

INT32 FindGridNoFromSweetSpotExcludingSweetSpotInQuardent( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, INT8 ubQuardentDir );

// Finds a gridno near a sweetspot but a random one!
INT32 FindRandomGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

// Finds a sweetspot but excluding this one!
// Flugente: I've altered this function in two ways:
// 1. The gridno is now drawn from the entirety of the circle - not just for sX and sY being positive
// 2. The direction now points to sSweetGridNo, center of the circle, instead of the map center
INT32 FindRandomGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

// Flugente: returns random gridno in a circle around sCenterGridNo with radius uOuterRadius that is not inside the circle with radius uInnerRadius
INT32 FindRandomGridNoBetweenCircles( INT32 sCenterGridNo, UINT8 uInnerRadius, UINT8 uOuterRadius, UINT8& urDirection );

// Adds a soldier ( already created in mercptrs[] array )! 
// Finds a good placement based on data in the loaded sector and if they are enemy's or not, etc...
BOOLEAN AddSoldierToSector( UINT16 ubID );

BOOLEAN AddSoldierToSectorNoCalculateDirection( UINT16 ubID );

BOOLEAN AddSoldierToSectorNoCalculateDirectionUseAnimation( UINT16 ubID, UINT16 usAnimState, UINT16 usAnimCode );

// IsMercOnTeam() checks to see if the passed in Merc Profile ID is currently on the player's team
BOOLEAN IsMercOnTeam( UINT8 ubMercID, BOOLEAN aAlreadyInCountry, BOOLEAN aAlive );



// GetSoldierIDFromMercID() Gets the Soldier ID from the Merc Profile ID, else returns -1
INT16 GetSoldierIDFromMercID(UINT8 ubMercID);

INT32 FindGridNoFromSweetSpotWithStructData( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc );

/*
void SoldierInSectorSleep( SOLDIERTYPE *pSoldier, INT16 sGridNo );
*/

INT32 FindGridNoFromSweetSpotWithStructDataFromSoldier( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc, SOLDIERTYPE *pSrcSoldier, BOOLEAN fAllowSoldierCurrentGrid = FALSE );

void SoldierInSectorPatient( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void SoldierInSectorDoctor( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void SoldierInSectorRepair( SOLDIERTYPE *pSoldier, INT32 sGridNo );

BOOLEAN CanSoldierReachGridNoInGivenTileLimit( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 sMaxTiles, INT8 bLevel );


#endif
