#ifndef __SOLDIER_INIT_LIST_H
#define __SOLDIER_INIT_LIST_H

#include "Soldier Create.h"
#include "FileMan.h"

typedef struct SOLDIERINITNODE
{
	UINT16 ubNodeID;
	UINT16 ubSoldierID;
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

UINT16 AddSoldierInitListTeamToWorld( INT8 bTeam, UINT16 ubMaxNum );
void AddSoldierInitListEnemyDefenceSoldiers( UINT16 ubTotalAdmin, UINT16 ubTotalTroops, UINT16 ubTotalElite, UINT16 ubTotalRobots, UINT16 ubTotalTanks, UINT16 ubTotalJeeps );
void AddSoldierInitListCreatures( BOOLEAN fQueen, UINT16 ubNumLarvae, UINT16 ubNumInfants,	
																	UINT16 ubNumYoungMales, UINT16 ubNumYoungFemales, UINT16 ubNumAdultMales, 
																	UINT16 ubNumAdultFemales );
void AddSoldierInitListOtherCreatures( UINT16 usNum );
void AddSoldierInitListMilitia( UINT16 ubNumGreen, UINT16 ubNumReg, UINT16 ubNumElites );
void AddSoldierInitListMilitiaOnEdge( UINT8 ubStrategicInsertionCode, UINT16 ubNumGreen, UINT16 ubNumReg, UINT16 ubNumElites );

void AddSoldierInitListBloodcats();

void UseEditorOriginalList();
void UseEditorAlternateList();
void AddPlacementToWorldByProfileID( UINT8 ubProfile );

void EvaluateDeathEffectsToSoldierInitList( SOLDIERTYPE *pSoldier );
void RemoveDetailedPlacementInfo( UINT16 ubNodeID );

void AddProfilesUsingProfileInsertionData();
void AddProfilesNotUsingProfileInsertionData();

// Flugente: decide wether to spawn enemy assassins in this sector (not kingpin's hitmen, they are handled elsewhere)
void SectorAddAssassins( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// Flugente: decide wether to create prisoners of war in a sector. Not to be confused with player POWs
void SectorAddPrisonersofWar( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// Flugente: decide wether to add a downed pilot if a helicopter was shot down here
void SectorAddDownedPilot( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

#endif
