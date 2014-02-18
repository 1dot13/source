#ifndef __STRATEGIC_H
#define __STRATEGIC_H
// header for strategic structure
#include "Types.h"
#include "mapscreen.h"
#include "soldier control.h"


struct strategicmapelement{
	UINT8 UNUSEDuiFootEta[4];			// eta/mvt costs for feet 
	UINT8 UNUSEDuiVehicleEta[4];		// eta/mvt costs for vehicles 
	UINT8 uiBadFootSector[4];			// blocking mvt for foot
	UINT8 uiBadVehicleSector[4];		// blocking mvt from vehicles
	INT8	bNameId;
	BOOLEAN fEnemyControlled;			// enemy controlled or not
	BOOLEAN fEnemyAirControlled;
	BOOLEAN UNUSEDfLostControlAtSomeTime;
	INT8 bSAMCondition;					// SAM Condition .. 0 - 100, just like an item's status

	INT8 bFillupforPadding[ 3 ];		// necessary for padding
	UINT32	usFlags;					// Flugente: various flags
	INT8 bPadding[ 20 ];
};

enum
{
	INSERTION_CODE_NORTH,
	INSERTION_CODE_EAST,
	INSERTION_CODE_SOUTH,
	INSERTION_CODE_WEST,
	INSERTION_CODE_GRIDNO,
	INSERTION_CODE_ARRIVING_GAME,
	INSERTION_CODE_CHOPPER,
	INSERTION_CODE_PRIMARY_EDGEINDEX,
	INSERTION_CODE_SECONDARY_EDGEINDEX,
	INSERTION_CODE_CENTER,
};

// -------- added by Flugente: various item flags --------
// flags used for various properties (easier than adding 32 differently named variables). DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define MILITIA_MOVE_NORTH				0x00000001	//1			// movement directions for ordering militia on the strategic map
#define MILITIA_MOVE_WEST				0x00000002	//2
#define MILITIA_MOVE_EAST				0x00000004	//4
#define MILITIA_MOVE_SOUTH				0x00000008	//8

#define MILITIA_MOVE_ALLDIRS			0x0000000F	// 15
// -------------------------------------------------------

//PLEASE USE CALCULATE_STRATEGIC_INDEX() macro instead (they're identical).
//#define			GETWORLDMAPNO( x, y )		( x+(MAP_WORLD_X*y) )

typedef struct strategicmapelement StrategicMapElement;
extern StrategicMapElement StrategicMap[MAP_WORLD_X*MAP_WORLD_Y];

BOOLEAN InitStrategicEngine( );

void HandleSoldierDeadComments( SOLDIERTYPE *pSoldier );

BOOLEAN HandleStrategicDeath( SOLDIERTYPE *pSoldier );

BOOLEAN SetMilitiaMovementOrder(INT16 sX, INT16 sY, INT8 sZ, UINT32 dir);

#endif