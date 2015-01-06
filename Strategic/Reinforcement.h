#ifndef __REINFORCEMENT_H__
#define __REINFORCEMENT_H__

//For Autoresolve (mostly)
void GetNumberOfEnemiesInFiveSectors( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumTanks );
BOOLEAN IsGroupInARightSectorToReinforce( GROUP *pGroup, INT16 sSectorX, INT16 sSectorY );
UINT8 GetAdjacentSectors( UINT8 pSectors[4], INT16 sSectorX, INT16 sSectorY );
UINT8 CountAllMilitiaInFiveSectors(INT16 sMapX, INT16 sMapY);
UINT8 NumEnemiesInFiveSectors( INT16 sMapX, INT16 sMapY );

//For Tactical
UINT8 DoReinforcementAsPendingEnemy( INT16 sMapX, INT16 sMapY );
void AddPossiblePendingMilitiaToBattle();
GROUP* GetEnemyGroupInSector( INT16 sMapX, INT16 sMapY );

#endif