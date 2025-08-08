#ifndef __REINFORCEMENT_H__
#define __REINFORCEMENT_H__

//For Autoresolve (mostly)
void GetNumberOfEnemiesInFiveSectors( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps );
void ActivateTurncoatsForAutoResolve( INT16 sSectorX, INT16 sSectorY );
BOOLEAN IsGroupInARightSectorToReinforce( GROUP *pGroup, INT16 sSectorX, INT16 sSectorY );
UINT8 GetAdjacentSectors( UINT8 pSectors[4], INT16 sSectorX, INT16 sSectorY );
UINT16 CountAllMilitiaInFiveSectors(INT16 sMapX, INT16 sMapY);
UINT16 NumEnemiesInFiveSectors( INT16 sMapX, INT16 sMapY );

//For Tactical
UINT8 DoReinforcementAsPendingNonPlayer( INT16 sMapX, INT16 sMapY, UINT8 usTeam );
void AddPossiblePendingMilitiaToBattle();
GROUP* GetNonPlayerGroupInSectorForReinforcement( INT16 sMapX, INT16 sMapY, UINT8 usTeam );

#endif
