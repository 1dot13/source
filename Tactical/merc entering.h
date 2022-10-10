#ifndef _MERC_ENTRING_H
#define _MERC_ENTRING_H

extern BOOLEAN		gfIngagedInDrop;

void ResetHeliSeats( );
void AddMercToHeli( UINT16 ubID );

// Flugente: we might want to set the helicopter dropoff point without starting it at the same time
void SetHelicopterDroppoint( INT32 sGridNo );
void SetHelicopterDropDirection( UINT8 usDirection );

void StartHelicopterRun();

void HandleHeliDrop( BOOLEAN fPlayer = TRUE );

UINT16 SpawnAirDropElite( INT32 sGridNo );

void InitiateEnemyAirDropSoldiers( INT32 sGridNo );
void HandleEnemyAirdrop( );

#endif

