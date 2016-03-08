#ifndef _UB_CONFIG_CFG_H
#define _UB_CONFIG_CFG_H

#include "soldier profile type.h"

extern void LoadGameUBOptions();

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
	BOOLEAN AutoResolve;
	BOOLEAN AddRandomEnemyToSector;
	BOOLEAN StrategicMovementCostsXML;
	BOOLEAN MakeStrategicMovementCosts;
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
	
	BOOLEAN fTexAndJohn;
	
	BOOLEAN fRandomManuelText;
	
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
	
	UINT8 ubMANUEL_UB; 
	UINT8 ubBIGGENS_UB; 
	UINT8 ubJOHN_K_UB; 
	UINT8 ubTEX_UB;
	UINT8 ubGASTON_UB; 
	UINT8 ubSTOGIE_UB; 
	UINT8 ubJERRY_MILO_UB; 
	UINT8 ubPGMALE4_UB; 
	UINT8 ubBETTY_UB; 
	UINT8 ubRAUL_UB; 
	UINT8 ubMORRIS_UB;
	UINT8 ubRUDY_UB;
	
	UINT32 ubTEX_MOVIE_ATTACK_CLYDESDALES;
	UINT32 ubTEX_MOVIE_WILD_EAST;
	UINT32 ubTEX_MOVIE_HAVE_HONDA;
	UINT32 ubLAPTOP_TRANSMITTER;
	UINT32 ubCHE_GUEVARA_CANTEEN;
	UINT32 ubMERC_WRISTWATCH;
	UINT32 ubSAM_GARVER_COMBAT_KNIFE;
	UINT32 ubMERC_UMBRELLA_OLD;
	UINT32 ubMORRIS_INSTRUCTION_NOTE;
	UINT32 ubHAND_CANNON;
	UINT32 ubHARTFORD_6_SHOOTER;
	UINT32 ubMERC_UMBRELLA;
	UINT32 ubCLIP_CANNON_BALL;
	UINT32 ubBARRETT_UB;
	UINT32 ubCALICO_960_UB;
	UINT32 ubPSG1_UB;
	UINT32 ubL85_UB;
	UINT32 ubTAR21_UB;
	UINT32 ubVAL_SILENT_UB;
	UINT32 ubMICRO_UZI_UB;
	UINT32 ubCALICO_950_UB;
	UINT32 ubCALICO_900_UB;
	
	//L15-3
	UINT32 SectorLaunchMisslesX;
	UINT32 SectorLaunchMisslesY; 
	UINT32 SectorLaunchMisslesZ;
	
	//J13-0
	UINT32 SectorFanX;
	UINT32 SectorFanY; 
	UINT32 SectorFanZ; 
	
	//K14-1
	UINT32 SectorOpenGateInTunnelX;
	UINT32 SectorOpenGateInTunnelY; 
	UINT32 SectorOpenGateInTunnelZ; 
	
	//J14-1
	UINT32 ExitForFanToPowerGenSectorX;
	UINT32 ExitForFanToPowerGenSectorY; 
	UINT32 ExitForFanToPowerGenSectorZ; 
	
	UINT32 MineSectorX;
	UINT32 MineSectorY; 
	UINT32 MineSectorZ; 
	
	UINT32 MineEntranceGridno; 
	UINT32 MineGridnoAddStructToHead; 
	
	UINT32 MineRemoveExitGridFromWorld1; 
	UINT32 MineRemoveExitGridFromWorld2; 

	UINT32 MineSectorUndergroundX;
	UINT32 MineSectorUndergroundY; 
	UINT32 MineSectorUndergroundZ; 
	
	UINT32 MineSectorUndergroundGridno1;
	UINT32 MineSectorUndergroundGridno2;

	UINT32 SectorDoorInTunnelX;
	UINT32 SectorDoorInTunnelY; 
	UINT32 SectorDoorInTunnelZ; 
	
	UINT32 SectorGuardPostX;
	UINT32 SectorGuardPostZ; 
	UINT32 SectorGuardPostY; 
	
	UINT32 H9MoneyGridNo; 	
	UINT32 H9MoneyEasy; 
	UINT32 H9MoneyMedium; 
	UINT32 H9MoneyHard; 
	
	UINT32 FristSectorTownX; 	
	UINT32 FristSectorTownY; 
	UINT32 FristSectorTownZ; 
	
	UINT32 I10MoneyGridNo1; 	
	UINT32 I10MoneyEasy1; 
	UINT32 I10MoneyMedium1; 
	UINT32 I10MoneyHard1; 
	
	UINT32 I10MoneyGridNo2; 	
	UINT32 I10MoneyEasy2; 
	UINT32 I10MoneyMedium2; 
	UINT32 I10MoneyHard2; 

	UINT32 SectorTown2X; 	
	UINT32 SectorTown2Y; 
	UINT32 SectorTown2Z; 

	UINT32 SectorTown3X; 	
	UINT32 SectorTown3Y; 
	UINT32 SectorTown3Z;
	
	UINT32 SectorTownGridNo1a; 
	UINT32 SectorTownGridNo1b; 

	UINT32 SectorTownGridNo2a; 
	UINT32 SectorTownGridNo2b; 
	
	UINT32 SectorTownGridNo3a; 
	UINT32 SectorTownGridNo3b; 
	
	//I9
	UINT32 I9SectorPlayerQuoteX; 	
	UINT32 I9SectorPlayerQuoteY; 
	UINT32 I9SectorPlayerQuoteZ; 
	
	//H10
	UINT32 H10SectorPlayerQuoteX; 	
	UINT32 H10SectorPlayerQuoteY; 
	UINT32 H10SectorPlayerQuoteZ; 
	
	UINT32 SectorDoorInTunnelGridNo;
	
	UINT8 MaxNumberOfMercs;
       
} GAME_UB_OPTIONS;

extern GAME_UB_OPTIONS gGameUBOptions; 

#ifdef JA2UB
extern void RandomAddEnemy( UINT8 SectorX, UINT8 SectorY, UINT8 Level );
extern void RandomStats ();
#endif

extern void RandomStats ();
#endif