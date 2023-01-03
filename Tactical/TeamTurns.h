#ifndef TEAMTURNS_H
#define TEAMTURNS_H

#include "Soldier Control.h"

extern UINT8 gubOutOfTurnPersons;
extern UINT8 gubOutOfTurnOrder[MAXMERCS] ;
extern BOOLEAN gfHiddenInterrupt;
extern BOOLEAN gfHiddenTurnbased;

#define INTERRUPT_QUEUED (gubOutOfTurnPersons > 0)

extern BOOLEAN StandardInterruptConditionsMet( SOLDIERTYPE * pSoldier, UINT8 ubOpponentID, INT8 bOldOppList);
extern INT8 CalcInterruptDuelPts( SOLDIERTYPE * pSoldier, UINT8 ubOpponentID, BOOLEAN fUseWatchSpots );
extern void EndAITurn( void );
extern void DisplayHiddenInterrupt( SOLDIERTYPE * pSoldier );
extern BOOLEAN InterruptDuel( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOpponent);
extern void AddToIntList( UINT8 ubID, BOOLEAN fGainControl, BOOLEAN fCommunicate );
extern void DoneAddingToIntList( SOLDIERTYPE * pSoldier, BOOLEAN fChange, UINT8 ubInterruptType);

void FreezeInterfaceForEnemyTurn( void );
void ClearIntList( void );

BOOLEAN	SaveTeamTurnsToTheSaveGameFile( HWFILE hFile );

BOOLEAN	LoadTeamTurnsFromTheSavedGameFile( HWFILE hFile );

void EndAllAITurns( void );
void EndTurnEvents( void );

BOOLEAN NPCFirstDraw( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTargetSoldier );

#endif