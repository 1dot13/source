#ifndef _MERC_ENTRING_H
#define _MERC_ENTRING_H


void ResetHeliSeats( );
void AddMercToHeli( UINT8 ubID );

// Flugente: we might want to set the helicopter dropoff point without starting it at the same time
void SetHelicopterDroppoint( INT32 sGridNo );
void SetHelicopterDropDirection( UINT8 usDirection );

void StartHelicopterRun();

void HandleHeliDrop( );

extern BOOLEAN		gfIngagedInDrop;

#endif

