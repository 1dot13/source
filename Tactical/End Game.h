#ifndef __ENDGAME_H
#define __ENDGAME_H


BOOLEAN DoesO3SectorStatueExistHere( INT32 sGridNo );
void ChangeO3SectorStatue( BOOLEAN fFromExplosion );

void HandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
void BeginHandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );

void HandleDoneLastKilledQueenQuote( );

void EndQueenDeathEndgameBeginEndCimenatic( );
void EndQueenDeathEndgame( );

void HandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
void BeginHandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );

void HandleDoneLastEndGameQuote( );


#endif