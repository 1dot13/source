#ifndef _STRATEGIC_EVENT_HANDLER_H_
#define _STRATEGIC_EVENT_HANDLER_H_

#include "Types.h"

#define KINGPIN_MONEY_SECTOR_X	gModSettings.ubKingpinMoneySectorX //5
#define KINGPIN_MONEY_SECTOR_Y	gModSettings.ubKingpinMoneySectorY //MAP_ROW_D
#define KINGPIN_MONEY_SECTOR_Z	gModSettings.ubKingpinMoneySectorZ //1

#define HOSPITAL_SECTOR_X	gModSettings.ubHospitalSectorX //8
#define HOSPITAL_SECTOR_Y	gModSettings.ubHospitalSectorY //MAP_ROW_F
#define HOSPITAL_SECTOR_Z	gModSettings.ubHospitalSectorZ //0

extern UINT8 gubCambriaMedicalObjects;

void CheckForKingpinsMoneyMissing( BOOLEAN fFirstCheck );
void CheckForMissingHospitalSupplies( void );

//void BobbyRayPurchaseEventCallback( UINT8 ubOrderID );

void HandleStolenItemsReturned( void );

void AddSecondAirportAttendant( void );

void SetPabloToUnbribed( void );
void HandleNPCSystemEvent( UINT32 uiEvent );
void HandleEarlyMorningEvents( void );

void MakeCivGroupHostileOnNextSectorEntrance( UINT8 ubCivGroup );

void RemoveAssassin( UINT8 ubProfile );

#endif