#ifndef __SOLDIER_INIT_LIST_H
#define __SOLDIER_INIT_LIST_H

#include "Soldier Create.h"
#include "FileMan.h"

typedef struct SOLDIERINITNODE
{
	UINT8 ubNodeID;
	UINT8 ubSoldierID;
	BASIC_SOLDIERCREATE_STRUCT *pBasicPlacement;
	SOLDIERCREATE_STRUCT *pDetailedPlacement;
	SOLDIERTYPE *pSoldier;
	struct SOLDIERINITNODE *prev;
	struct SOLDIERINITNODE *next;
}SOLDIERINITNODE;

extern SOLDIERINITNODE *gSoldierInitHead;
extern SOLDIERINITNODE *gSoldierInitTail;

//These serialization functions are assuming the passing of a valid file 
//pointer to the beginning of the save/load area, at the correct part of the
//map file.
BOOLEAN LoadSoldiersFromMap( INT8 **hBuffer, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion	);
BOOLEAN SaveSoldiersToMap(HWFILE fp, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);//dnl ch33 150909

//For the purpose of keeping track of which soldier belongs to which placement within the game,
//the only way we can do this properly is to save the soldier ID from the list and reconnect the
//soldier pointer whenever we load the game.
BOOLEAN SaveSoldierInitListLinks( HWFILE hfile );
BOOLEAN LoadSoldierInitListLinks( HWFILE hfile );
BOOLEAN NewWayOfLoadingEnemySoldierInitListLinks( HWFILE hfile );
BOOLEAN NewWayOfLoadingCivilianInitListLinks( HWFILE hfile );
BOOLEAN LookAtButDontProcessEnemySoldierInitListLinks( HWFILE hfile );

void InitSoldierInitList();
void KillSoldierInitList();
SOLDIERINITNODE* AddBasicPlacementToSoldierInitList( BASIC_SOLDIERCREATE_STRUCT *pBasicPlacement );
void RemoveSoldierNodeFromInitList( SOLDIERINITNODE *pNode );
SOLDIERINITNODE* FindSoldierInitNodeWithID( UINT16 usID );

UINT8 AddSoldierInitListTeamToWorld( INT8 bTeam, UINT8 ubMaxNum );
void AddSoldierInitListEnemyDefenceSoldiers( UINT8 ubTotalAdmin, UINT8 ubTotalTroops, UINT8 ubTotalElite, UINT8 ubTotalRobots, UINT8 ubTotalTanks, UINT8 ubTotalJeeps );
void AddSoldierInitListCreatures( BOOLEAN fQueen, UINT8 ubNumLarvae, UINT8 ubNumInfants,	
																	UINT8 ubNumYoungMales, UINT8 ubNumYoungFemales, UINT8 ubNumAdultMales, 
																	UINT8 ubNumAdultFemales );
void AddSoldierInitListOtherCreatures( UINT8 usNum );
void AddSoldierInitListMilitia( UINT8 ubNumGreen, UINT8 ubNumReg, UINT8 ubNumElites );
void AddSoldierInitListMilitiaOnEdge( UINT8 ubStrategicInsertionCode, UINT8 ubNumGreen, UINT8 ubNumReg, UINT8 ubNumElites );

void AddSoldierInitListBloodcats();

void UseEditorOriginalList();
void UseEditorAlternateList();
void AddPlacementToWorldByProfileID( UINT8 ubProfile );

void EvaluateDeathEffectsToSoldierInitList( SOLDIERTYPE *pSoldier );
void RemoveDetailedPlacementInfo( UINT8 ubNodeID );

void AddProfilesUsingProfileInsertionData();
void AddProfilesNotUsingProfileInsertionData();

// Flugente: decide wether to spawn enemy assassins in this sector (not kingpin's hitmen, they are handled elsewhere)
void SectorAddAssassins( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// Flugente: decide wether to create prisoners of war in a sector. Not to be confused with player POWs
void SectorAddPrisonersofWar( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// Flugente: decide wether to add a downed pilot if a helicopter was shot down here
void SectorAddDownedPilot( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

#endif