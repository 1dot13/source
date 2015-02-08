#ifndef _MILITIA_SQUADS_H
#define _MILITIA_SQUADS_H

// header for town militia strategic control module

#include "Types.h"
#include "Soldier Control.h"
#include "campaign types.h"

//#define MAXIMUM_MILITIA_SQUAD_SIZE 30 

extern UINT8 gpAttackDirs[5][4];
extern UINT8 guiDirNumber;

// Updates movement orders for militia squads
void UpdateMilitiaSquads(INT16 sMapX, INT16 sMapY );

// Flugente: militia in this sector is ordered to move according to flags that were applied to it prior
void MilitiaMovementOrder(UINT8 sector);

// Kaiden: creates militia squad in an adjacent sector
// to the coordinates you pass to it.
void CreateMilitiaSquads(INT16 sMapX, INT16 sMapY );
void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY );

// HEADROCK HAM 3.4: Now using different arguments for this function. All relevent instances have also been altered.
void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fForTraining, BOOLEAN fForBattle );
BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY);
void MilitiaFollowPlayer( INT16 sMapX, INT16 sMapY, INT16 sDMapX, INT16 sDMapY );

// HEADROCK HAM B1: Changes the allowed militia sectors
// HEADROCK HAM 5: New flag tells us to also recheck restriced sectors.
extern void AdjustRoamingRestrictions( BOOLEAN fRecheck );
// HEADROCK HAM 4: Yet ANOTHER array, this one holds player-set restrictions. It interacts with the dynamic one below.
extern UINT8 gubManualRestrictMilitia[ 256 ];
// HEADROCK HAM B1: Alternate array keeps track of dynamically unrestricted sectors
extern BOOLEAN gDynamicRestrictMilitia[ 256 ];
// HEADROCK HAM B2.7: Wonder why this function wasn't declared here. It is now, to allow Town Militia.cpp to
// access it.

BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fForTraining, BOOLEAN fForBattle );

// HEADROCK HAM B2.7: Generates possible movement directions - this is used only in CreateMilitiaGroup(), and
// makes sure that the target sector has room for more militia.
void GenerateDirectionInfosForTraining( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3] );

// HEADROCK HAM 3.4: Stores restriction data from XML, including required cities for each sector to be allowed.
typedef struct DYNAMICRESTRICTIONS
{
	INT16 sSectorID;
	UINT32 uiReqTownFlags;
} DYNAMICRESTRICTIONS;

// HEADROCK HAM 3.4: New function for simple Roaming Restriction testing.
BOOLEAN IsSectorRoamingAllowed( UINT32 uiSector );

// HEADROCK HAM 3.6: New upgrade check returns the amount of militia that can be upgraded at target sector, in
// "upgrade points"
UINT16 MilitiaUpgradeSlotsCheck( INT16 sMapX, INT16 sMapY );

// HEADROCK HAM 3.6: This needs to be accessible.
void AddToBlockMoveList(INT16 sMapX, INT16 sMapY);

// HEADROCK HAM 4: Returns whether sector is allowed for militia roaming, taking into account player-set restrictions.
UINT8 ManualMobileMovementAllowed( UINT8 ubSector );

// HEADROCK HAM 4: Initialize all manual restrictions;
void InitManualMobileRestrictions();

//Moa: for mobile militia: militia deserters and maximum allowed
extern void MobileMilitiaDeserters( INT16 sMapX, INT16 sMapY, BOOLEAN fDeleteEquip, BOOLEAN fPrintMessage );
extern UINT8 GetMobileMilitiaQuota( BOOLEAN printMessage );

// Flugente: militia movement
BOOLEAN SaveMilitiaMovementInformationToSaveGameFile( HWFILE hFile );
BOOLEAN LoadMilitiaMovementInformationFromSavedGameFile( HWFILE hFile, UINT32 uiSavedGameVersion );

// delete a militia group and transfer its militia to the sector they are in
void DissolveMilitiaGroup( UINT8 uGroupId );

BOOLEAN MilitiaGroupEntersCurrentSector( UINT8 usGroupId, INT16 sMapX, INT16 sMapY );

#endif