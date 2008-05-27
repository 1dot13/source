#pragma once

//time for a new header :)

extern int readyteamreg[10];

extern bool NOGEAR;

typedef struct
{
	UINT16 soldier_id;
	UINT16 attacker_id;
}death_struct;

typedef struct
{
	UINT16 ubID;
	INT16 sGridNo;
	BOOLEAN fNoAnimations;
}doors;

typedef struct
{
UINT16 ubID;
INT8 bLife;
INT8 bBleeding;
}heal;

typedef struct
{
	UINT16 ubID;
	INT8 bteam;

}AIint;

BOOLEAN DisplayMercsInventory(UINT8 ubMercID);
void send_door ( SOLDIERTYPE *pSoldier, INT16 sGridNo, BOOLEAN fNoAnimations );

void send_changestate (EV_S_CHANGESTATE * SChangeState);

void send_death( SOLDIERTYPE *pSoldier);

void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit);
void send_hitwindow(EV_S_WINDOWHIT * SWindowHit);
void send_miss(EV_S_MISS * SMiss);
					
void cheat_func(void);
//void start_tt(void);
void unlock (void);

void UpdateSoldierToNetwork ( SOLDIERTYPE *pSoldier );

extern BOOLEAN gfUIInterfaceSetBusy;
void TrashAllSoldiers( );

void kick_player (void);
void overide_turn (void);

extern bool DISABLE_MORALE;

void send_fireweapon ( EV_S_FIREWEAPON * SFireWeapon);

void end_interrupt ( BOOLEAN fMarkInterruptOccurred );
void EndInterrupt( BOOLEAN fMarkInterruptOccurred );
void sendRT (void);

extern int numreadyteams;//for realtime

extern bool requested;
extern bool getReal;

extern UINT8			gubCheatLevel;

void startCombat(UINT8 ubStartingTeam);

extern int WEAPON_READIED_BONUS;

void intAI (SOLDIERTYPE *pSoldier );

void teamwiped (void);

BOOLEAN check_status (void);
extern UINT8 NumEnemyInSector( );
extern INT8	NumActiveAndConsciousTeamMembers( UINT8 ubTeam );

void send_heal (SOLDIERTYPE *pSoldier );

extern int DISABLE_SPEC_MODE;

void requestAIint(SOLDIERTYPE *pSoldier );