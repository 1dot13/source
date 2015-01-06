#ifndef __QUEEN_COMMAND_H
#define __QUEEN_COMMAND_H

#include "types.h"
#include "Campaign Types.h"
#include "Soldier Control.h"
#include "FileMan.h"
#include "Strategic Movement.h"



extern BOOLEAN gfPendingEnemies;


UINT8 NumFreeSlots( UINT8 ubTeam );

//Counts enemies and crepitus, but not bloodcats.
UINT8 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

UINT8 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

// returns how many members of a team are in a sector - not intended for OUR_TEAM!
UINT8 NumNonPlayerTeamMembersInSector( INT16 sSectorX, INT16 sSectorY, UINT8 ubTeam );

UINT16 NumEnemyTanksInSector( INT16 sSectorX, INT16 sSectorY );
UINT8 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
UINT8 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
void GetNumberOfMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumTanks );
void GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumTanks );
void GetNumberOfStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumTanks );
void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumTanks );

//Called when entering a sector so the campaign AI can automatically insert the
//correct number of troops of each type based on the current number in the sector
//in global focus (gWorldSectorX/Y)
BOOLEAN PrepareEnemyForSectorBattle();
BOOLEAN PrepareEnemyForUndergroundBattle();

void AddEnemiesToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, BOOLEAN fMagicallyAppeared );
void AddPossiblePendingEnemiesToBattle();
void EndTacticalBattleForEnemy();

void ProcessQueenCmdImplicationsOfDeath( SOLDIERTYPE *pSoldier );

void HandleEnemyStatusInCurrentMapBeforeLoadingNewMap();
BOOLEAN SaveUnderGroundSectorInfoToSaveGame( HWFILE hFile );
BOOLEAN LoadUnderGroundSectorInfoFromSavedGame( HWFILE hFile );

//Finds and returns the specified underground structure ( DONT MODIFY IT ).	Else returns NULL
UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );

void EnemyCapturesPlayerSoldier( SOLDIERTYPE *pSoldier );
void BeginCaptureSquence( );
void EndCaptureSequence( );

BOOLEAN PlayerSectorDefended( UINT8 ubSectorID );

BOOLEAN OnlyHostileCivsInSector();

//Buggler: Externalized to gModSettings.iMeanwhileInterrogatePOWGridNo
//extern INT32 gsInterrogationGridNo[3];

#ifdef JA2UB
extern	INT32		gsGridNoForMapEdgePointInfo;
#endif

BOOLEAN CheckPendingEnemies();

extern UINT32 guiTurnCnt, guiReinforceTurn, guiMilitiaReinforceTurn;//dnl ch68 080913

#endif
