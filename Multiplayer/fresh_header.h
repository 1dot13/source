#pragma once

//time for a new header :)
extern int readyteamreg[10];

typedef struct
{
	SoldierID soldier_id;
	SoldierID attacker_id;
	UINT8 attacker_team;
	UINT8 soldier_team;
}death_struct;

typedef struct
{
	SoldierID ubID;
	INT32 sGridNo;
	BOOLEAN fNoAnimations;
}doors;

typedef struct
{
	SoldierID ubID;
	INT8 bLife;
	INT8 bBleeding;
}heal;

typedef struct
{
	SoldierID ubID;
	INT8 bteam;
}AIint;

extern BOOLEAN gfUIInterfaceSetBusy;
extern int numreadyteams;//for realtime
extern bool requested;
extern bool getReal;
extern UINT8 gubCheatLevel;

// Client variables
extern UINT8 cWeaponReadyBonus;
extern UINT8 cDisableSpectatorMode;
extern UINT8 cDisableMorale;

BOOLEAN DisplayMercsInventory(UINT8 ubMercID);
void send_door ( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fNoAnimations );
void send_changestate (EV_S_CHANGESTATE * SChangeState);
void send_death( SOLDIERTYPE *pSoldier);
void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit);
void send_hitwindow(EV_S_WINDOWHIT * SWindowHit);
void send_miss(EV_S_MISS * SMiss);	
void unlock (void);
void UpdateSoldierToNetwork ( SOLDIERTYPE *pSoldier );
void TrashAllSoldiers( );
void kick_player (void);
void overide_turn (void);
void send_fireweapon ( EV_S_FIREWEAPON * SFireWeapon);
void end_interrupt ( BOOLEAN fMarkInterruptOccurred );
void EndInterrupt( BOOLEAN fMarkInterruptOccurred );
void sendRT (void);
void startCombat(UINT8 ubStartingTeam);
void intAI (SOLDIERTYPE *pSoldier );
void teamwiped (void);
BOOLEAN check_status (void);
extern UINT16 NumEnemyInSector( );
extern UINT16	NumActiveAndConsciousTeamMembers( UINT8 ubTeam );
void send_heal (SOLDIERTYPE *pSoldier );
void requestAIint(SOLDIERTYPE *pSoldier );
