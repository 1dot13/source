#ifndef __ENDGAME_H
#define __ENDGAME_H


BOOLEAN DoesO3SectorStatueExistHere( INT32 sGridNo );
void ChangeO3SectorStatue( BOOLEAN fFromExplosion );

void HandleDoneLastKilledQueenQuote( );
void HandleDoneLastEndGameQuote( );

#ifdef JA2UB
void DoneFadeOutEndCinematic( void );
void HandleJa25EndGameAndGoToCreditsScreen( BOOLEAN fFromTactical );
void HandleEveryoneDoneTheirEndGameQuotes();
void EndGameEveryoneSayTheirGoodByQuotes( void );
void EnterTacticalInFinalSector();
extern	BOOLEAN			gfPlayersLaptopWasntWorkingAtEndOfGame;
#else
void HandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
void BeginHandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
void EndQueenDeathEndgameBeginEndCimenatic( );
void EndQueenDeathEndgame( );

void HandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
void BeginHandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel );
#endif

#endif
