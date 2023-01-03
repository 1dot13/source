#ifndef _LEGION_CFG_H
#define _LEGION_CFG_H

#include "soldier profile type.h"

extern void LoadGameLegionOptions();

typedef struct
{
    BOOLEAN BorderTown;
    BOOLEAN BorderMine;
    BOOLEAN BorderTeams;
    BOOLEAN BorderMilitia;
    BOOLEAN BorderAirspace;
    BOOLEAN BorderItem;
	   	
	BOOLEAN EnemyXML;
	BOOLEAN EventAttackInitialSectorIfPlayerStillThere;
	BOOLEAN HandleAddingEnemiesToTunnelMaps;
	BOOLEAN BobbyRayInventory_UB;
	BOOLEAN InitTownLoyalty_UB;
	BOOLEAN AutoResolove;
	BOOLEAN AddRandomEnemyToSector;
	BOOLEAN StrategicMovementCostsXML;
	BOOLEAN MakeStrategicMovementCosts;
	BOOLEAN MercStartingGear25XML;
	UINT32	InitialHeliGridNo[ 7 ];
	INT16	InitalHeliRandomTimes[ 7 ];
	UINT32	LOCATEGRIDNO;
	BOOLEAN LaptopIMPPassJA2;
	BOOLEAN LaptopIMPPassUB;
	
	UINT8 ubDefaultArrivalSectorX;
	UINT8 ubDefaultArrivalSectorY;
	
	INT32	iStartingCashNovice;
	INT32	iStartingCashExperienced;
	INT32	iStartingCashExpert;
	INT32	iStartingCashInsane;
	
	BOOLEAN TEX_AND_JOHN;
	
	BOOLEAN Random_Manuel_Text;
	
	BOOLEAN RISRAPORT;
	
	BOOLEAN LaptopQuestEnabled;
	
	BOOLEAN InGameHeliCrash;
	
	BOOLEAN InGameHeli;
	
	UINT32 LOCATEGRIDNO2;
	
	BOOLEAN JerryQuotes;
	
	BOOLEAN InJerry;
	
	UINT32 JerryGridNo;
	
	UINT32  SwitchInMorrisAreaGridNo;
	UINT32  SwitchToLaunchMisslesGridNo1;
	UINT32  SwitchToLaunchMisslesGridNo2;
	UINT32  RadioLocatorsGridNo1;
	UINT32  RadioLocatorsGridNo2;
	UINT32  PowergenSectorGridNo1;
	UINT32  PowergenSectorGridNo2;
	UINT32  PowergenSectorGridNo3;
	UINT32  PowergenSectorGridNo4;
	UINT32  PowergenSectorExitgridGridNo;
	UINT32  PowergenFanSoundGridNo1;
	UINT32  PowergenFanSoundGridNo2;
	UINT32  StartFanbackupAgainGridNo;
	UINT32  StopPowergenFanGridNo;
	
	BOOLEAN TestUB;
	
	BOOLEAN LaptopLinkInsurance;
	BOOLEAN LaptopLinkFuneral;
	BOOLEAN LaptopLinkBobby;
	
	UINT32 FanGridNo[9];
	
	UINT8 ubEndDefaultSectorX;
	UINT8 ubEndDefaultSectorY;
	UINT8 ubEndDefaultSectorZ;
	
	BOOLEAN pJA2UB;
	
	BOOLEAN fBobbyRSite;
	BOOLEAN fDeadMerc;
	
	BOOLEAN OLDUBInventory;
       
} GAME_LEGION_OPTIONS;

extern GAME_LEGION_OPTIONS gGameLegionOptions; 

#ifdef JA2UB
extern void RandomAddEnemy( UINT8 SectorX, UINT8 SectorY, UINT8 Level );
extern void RandomStats ();
#endif

extern void RandomStats ();
#endif