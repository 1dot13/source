#ifndef _MILITIA_SQUADS_H
#define _MILITIA_SQUADS_H

// header for town militia strategic control module

#include "Types.h"
#include "Soldier Control.h"

//#define MAXIMUM_MILITIA_SQUAD_SIZE 30 

extern UINT8 gpAttackDirs[5][4];
extern UINT8 guiDirNumber;

void UpdateMilitiaSquads(INT16 sMapX, INT16 sMapY );
void CreateMilitiaSquads(INT16 sMapX, INT16 sMapY );
void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY );

void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors );
BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY);
void MilitiaFollowPlayer( INT16 sMapX, INT16 sMapY, INT16 sDMapX, INT16 sDMapY );

#endif