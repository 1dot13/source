#ifndef _MILITIA_SQUADS_H
#define _MILITIA_SQUADS_H

// header for town militia strategic control module

#include "Types.h"
#include "Soldier Control.h"
#include "campaign types.h"

//#define MAXIMUM_MILITIA_SQUAD_SIZE 30 

extern UINT8 gpAttackDirs[5][4];
extern UINT8 guiDirNumber;

// Flugente: militia in this sector is ordered to move according to flags that were applied to it prior
void MilitiaMovementOrder(UINT8 sector);

// Kaiden: creates militia squad in an adjacent sector
// to the coordinates you pass to it.
void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY );

// HEADROCK HAM 3.4: Now using different arguments for this function. All relevent instances have also been altered.
void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fForTraining, BOOLEAN fForBattle );
BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY);

// HEADROCK HAM B2.7: Wonder why this function wasn't declared here. It is now, to allow Town Militia.cpp to
// access it.

BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fForTraining, BOOLEAN fForBattle );

// HEADROCK HAM B2.7: Generates possible movement directions - this is used only in CreateMilitiaGroup(), and
// makes sure that the target sector has room for more militia.
void GenerateDirectionInfosForTraining( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3] );

// HEADROCK HAM 3.6: New upgrade check returns the amount of militia that can be upgraded at target sector, in
// "upgrade points"
UINT16 MilitiaUpgradeSlotsCheck( INT16 sMapX, INT16 sMapY );

// HEADROCK HAM 3.6: This needs to be accessible.
void AddToBlockMoveList(INT16 sMapX, INT16 sMapY);

// Flugente: militia movement
BOOLEAN SaveMilitiaMovementInformationToSaveGameFile( HWFILE hFile );
BOOLEAN LoadMilitiaMovementInformationFromSavedGameFile( HWFILE hFile, UINT32 uiSavedGameVersion );

// delete a militia group and transfer its militia to the sector they are in
void DissolveMilitiaGroup( UINT8 uGroupId );

BOOLEAN MilitiaGroupEntersCurrentSector( UINT8 usGroupId, INT16 sMapX, INT16 sMapY );

#endif