#ifndef __SOLDIER_TILE_H
#define __SOLDIER_TILE_H

#include "soldier control.h"

#define			MOVE_TILE_CLEAR										1
#define			MOVE_TILE_TEMP_BLOCKED					-1
#define			MOVE_TILE_STATIONARY_BLOCKED		-2



INT8 TileIsClear( SOLDIERTYPE *pSoldier, INT8 bDirection, INT32 sGridNo, INT8 bLevel );

void MarkMovementReserved( SOLDIERTYPE *pSoldier, INT32 sGridNo );

void UnMarkMovementReserved( SOLDIERTYPE *pSoldier );

BOOLEAN HandleNextTile( SOLDIERTYPE *pSoldier, INT8 bDirection, INT32 sGridNo, INT32 sFinalDestTile );//dnl ch53 111009

BOOLEAN HandleNextTileWaiting( SOLDIERTYPE *pSoldier );

BOOLEAN TeleportSoldier( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fForce );

void SwapMercPositions( SOLDIERTYPE *pSoldier1, SOLDIERTYPE *pSoldier2 );

void SetDelayedTileWaiting( SOLDIERTYPE *pSoldier, INT32 sCauseGridNo, INT8 bValue );

BOOLEAN CanExchangePlaces( SOLDIERTYPE *pSoldier1, SOLDIERTYPE *pSoldier2, BOOLEAN fShow );

#endif