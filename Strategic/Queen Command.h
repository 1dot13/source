#ifndef __QUEEN_COMMAND_H
#define __QUEEN_COMMAND_H

#include "types.h"
#include "Campaign Types.h"
#include "Soldier Control.h"
#include "FileMan.h"
#include "Strategic Movement.h"



extern BOOLEAN gfPendingNonPlayerTeam[PLAYER_PLAN];


UINT16 NumFreeSlots( UINT8 ubTeam );

//Counts enemies and crepitus, but not bloodcats.
UINT16 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

UINT16 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

// returns how many members of a team are in a sector - not intended for OUR_TEAM!
UINT16 NumNonPlayerTeamMembersInSector( INT16 sSectorX, INT16 sSectorY, UINT8 ubTeam );

// returns how many members of a team are in a sector - only intended for OUR_TEAM! POWs not included
UINT16 NumPlayerTeamMembersInSector( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ );

UINT16 NumEnemyArmedVehiclesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 usTeam );
UINT16 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
UINT16 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
void GetNumberOfMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps );
void GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( INT16 sSectorX, INT16 sSectorY, UINT16 usTeam, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps );
void GetNumberOfStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps );
void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps );

//Called when entering a sector so the campaign AI can automatically insert the
//correct number of troops of each type based on the current number in the sector
//in global focus (gWorldSectorX/Y)
BOOLEAN PrepareEnemyForSectorBattle();
BOOLEAN PrepareEnemyForUndergroundBattle();

void AddEnemiesToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT16 ubNumAdmins, UINT16 ubNumTroops, UINT16 ubNumElites, UINT16 ubNumRobots, UINT16 ubNumTanks, UINT16 abNumJeeps, BOOLEAN fMagicallyAppeared );
void AddMilitiaToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT16 ubNumGreens, UINT16 ubNumRegulars, UINT16 ubNumElites, BOOLEAN fMagicallyAppeared );
void AddPossiblePendingEnemiesToBattle();
void EndTacticalBattleForEnemy();

void ProcessQueenCmdImplicationsOfDeath( SOLDIERTYPE *pSoldier );

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

// Flugente check whether a team has nearby members that can be added to the current battle
// Note that this requires separate counts of teammembers in the sector and those that are actually fighting
// This currently does not exist for MILITIA_TEAM, making such a check relatively useless
BOOLEAN CheckPendingNonPlayerTeam(UINT8 usTeam);

// Flugente: turncoats
UINT16 NumTurncoatsOfClassInSector( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass );
BOOLEAN TurncoatsInSector( INT16 sSectorX, INT16 sSectorY );
bool RemoveOneTurncoat( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass, BOOLEAN alsoRemoveFromGroup );
bool AddOneTurncoat( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass );
void CorrectTurncoatCount( INT16 sSectorX, INT16 sSectorY );

extern UINT32 guiTurnCnt, guiReinforceTurn, guiMilitiaReinforceTurn;//dnl ch68 080913

#endif
