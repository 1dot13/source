#ifndef _MILITIA_SQUADS_H
#define _MILITIA_SQUADS_H

// header for town militia strategic control module

#include "Types.h"
#include "Soldier Control.h"

//#define MAXIMUM_MILITIA_SQUAD_SIZE 30 

extern UINT8 gpAttackDirs[5][4];
extern UINT8 guiDirNumber;

// Updates movement orders for militia squads
void UpdateMilitiaSquads(INT16 sMapX, INT16 sMapY );
// Kaiden: creates militia squad in an adjacent sector
// to the coordinates you pass to it.
void CreateMilitiaSquads(INT16 sMapX, INT16 sMapY );
void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY );

void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors );
BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY);
void MilitiaFollowPlayer( INT16 sMapX, INT16 sMapY, INT16 sDMapX, INT16 sDMapY );

// HEADROCK HAM B1: Changes the allowed militia sectors
extern void AdjustRoamingRestrictions();
// HEADROCK HAM B1: Alternate array keeps track of dynamically unrestricted sectors
extern BOOLEAN gDynamicRestrictMilitia[ 256 ];
// HEADROCK HAM B2.7: Wonder why this function wasn't declared here. It is now, to allow Town Militia.cpp to
// access it.

BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors );

// HEADROCK HAM B2.7: Generates possible movement directions - this is used only in CreateMilitiaGroup(), and
// makes sure that the target sector has room for more militia.
void GenerateDirectionInfosWithCapacityCheck( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors );
#endif