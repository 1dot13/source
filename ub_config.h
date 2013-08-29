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

	UINT8 MaxNumberOfMercs;
       
} GAME_UB_OPTIONS;

extern GAME_UB_OPTIONS gGameUBOptions; 

#ifdef JA2UB
extern void RandomAddEnemy( UINT8 SectorX, UINT8 SectorY, UINT8 Level );
extern void RandomStats ();
#endif

extern void RandomStats ();
#endif