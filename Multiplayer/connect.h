#pragma once


#include "Soldier Init List.h"
#include "Merc Hiring.h"
#include "event pump.h"
#include "Bullets.h"
#include "builddefines.h"

extern bool isOwnTeamWipedOut;

extern bool is_connected;
extern bool is_connecting;
extern bool is_client;
extern bool is_server;
extern bool is_networked;
extern bool is_host; // OJW - added 20081129

// OJW - 20040922
extern bool recieved_settings;
extern bool recieved_transfer_settings;
extern INT16 serverSyncClientsDirectory;

extern int PLAYER_TEAM_TIMER_SEC_PER_TICKS;
extern int CLIENT_NUM;

extern UINT8 gEnemyEnabled;
extern UINT8 gCreatureEnabled;
extern UINT8 gMilitiaEnabled;
extern UINT8 gCivEnabled;

extern UINT8 cMaxClients;
extern UINT8 cSameMercAllowed;
extern UINT8 cGameType;

extern bool allowlaptop;

extern UINT8 netbTeam;
extern UINT8 ubID_prefix;
extern FLOAT cDamageMultiplier;

//OJW - 20081218
extern UINT8 gRandomStartingEdge;
extern UINT8 gRandomMercs;

//OJW - 20090317
extern bool is_game_started;

//OJW - 20090403
extern UINT8 gMaxEnemiesEnabled;

//OJW - 20090405
extern STRING512 gCurrentTransferFilename;
extern INT32 gCurrentTransferBytes;
extern INT32 gTotalTransferBytes;

extern UINT32 crate_usMapPos;

//extern int INTERRUPTS;

extern UINT8 cAllowMercEquipment;

extern UINT8 cMaxMercs;

void lockui (bool unlock);

void start_battle ( void );
void DropOffItemsInSector( UINT8 ubOrderNum );

void mp_help (void);
void mp_help2 (void);
void grid_display ( void);

void send_loaded (void);
void send_donegui ( UINT8 ubResult );


UINT8 numenemyLAN( UINT8 ubSectorX, UINT8 ubSectorY );

void connect_client ( void );
void start_server (void);
void client_packet ( void );
void server_packet ( void );

void NetworkAutoStart(); // OJW - 20081130 - Automate connection functions

void server_disconnect (void);
void client_disconnect (void);

void DialogRemoved( UINT8 ubResult );
void manual_overide(void);

void send_path (  SOLDIERTYPE *pSoldier, INT32 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  );
void send_stance ( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void send_dir ( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection );
void send_fire( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo );
void send_hit(  EV_S_WEAPONHIT *SWeaponHit  );
void send_bullet(  BULLET * pBullet, UINT16 usHandItem); 
void send_hire( UINT8 iNewIndex, UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver);
void send_dismiss( UINT8 ubCurrentSoldier);

void send_gui_pos(SOLDIERTYPE *pSoldier,  FLOAT dNewXPos, FLOAT dNewYPos);
void send_gui_dir(SOLDIERTYPE *pSoldier, UINT16	usNewDirection);

void send_EndTurn( UINT8 ubNextTeam );

void send_AI( SOLDIERCREATE_STRUCT *pCreateStruct, UINT8 *pubID );

void send_stop (EV_S_STOP_MERC *SStopMerc);

void send_interrupt(SOLDIERTYPE *pSoldier);

// OJW - 20091002 - explosives
void send_grenade (OBJECTTYPE *pGameObj, float dLifeLength, float xPos, float yPos, float zPos, float xForce, float yForce, float zForce, UINT32 sTargetGridNo, UINT8 ubOwner, UINT8 ubActionCode, UINT32 uiActionData, INT32 iRealObjectID , bool bIsThrownGrenade);
void send_grenade_result (float xPos, float yPos, float zPos, INT32 sGridNo, UINT8 ubOwnerID, INT32 iRealObjectID, bool bIsDud);
void send_plant_explosive (UINT8 ubID,UINT16 usItem,UINT8 ubItemStatus,UINT16 usFlags, UINT32 sGridNo,UINT8 ubLevel, UINT32 uiWorldIndex);
void send_detonate_explosive (UINT32 uiWorldIndex, UINT8 ubID);
void send_spreadeffect ( INT32 sGridNo, UINT8 ubRadius, UINT16 usItem, UINT8 ubOwner, BOOLEAN fSubsequent, INT8 bLevel, INT32 iSmokeEffectID );
void send_newsmokeeffect(INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT8 ubOwner, INT32 iSmokeEffectID);
void send_gasdamage( SOLDIERTYPE * pSoldier, UINT16 usExplosiveClassID , INT16 sSubsequent, BOOLEAN fRecompileMovementCosts, INT16 sWoundAmt, INT16 sBreathAmt, UINT8 ubOwner );
void send_explosivedamage( UINT8 ubPerson, UINT8 ubOwner, INT32 sBombGridNo, INT16 sWoundAmt, INT16 sBreathAmt, UINT32 uiDist, UINT16 usItem, INT16 sSubsequent );
void send_disarm_explosive(UINT32 sGridNo, UINT32 uiWorldIndex, UINT8 ubID);

void OpenChatMsgBox(void);

INT8 FireBullet( SOLDIERTYPE * pFirer, BULLET * pBullet, BOOLEAN fFake );

void reapplySETTINGS();

BOOLEAN CheckConditionsForBattle( GROUP *pGroup ); // this comes from strategic movement.cpp

extern char client_names[4][30];

extern CHAR16 TeamNameStrings[][30];

// OJW - 20081204
// I need to keep track of all the clients readyness, spawn edge and team
// <TODO> want to change this all to use client_info, but dont want to make sweeping
// changes to the codebase without talking to the other devs
extern int  client_ready[4];
extern int	client_teams[4];
extern int	client_edges[5];
extern int  client_downloading[4];
extern int  client_progress[4];

extern char	cServerName[30];

//OJW - 20081224
#define MAX_CONNECT_RETRIES	5

// WANNE: After some MP-Tests: It seems there are still problems with enemy interupt and if this define is enabled the ALT + E (give turn to client) does not work either. So I disabled this define for now ...
// WANNE: If this define is enabled, it hopefully fixes the "enemy AI got stuck on pure client interrupt". (this "fix" was added in revision 5623)
#define INTERRUPT_MP_DEADLOCK_FIX

// WANNE: This features seems to work without any errors, so it is enabled :)
#define ENABLE_MP_FRIENDLY_PLAYERS_SHARE_SAME_FOV

#define ENABLE_COLLISION (is_server && pBullet->pFirer->ubID<120) || (!is_server && is_client && pBullet->pFirer->ubID<20) || (!is_server && !is_client) 
extern bool auto_retry;
extern int giNumTries;

void send_settings();

void send_mapchange(); // added 20081201 by OJW
void send_teamchange( int newteam ); // send team a change
void send_edgechange( int newedge ); // send an edge change
bool can_teamchange(); // UI Checks
bool can_edgechange(); // UI Checks


void StartInterrupt( void );

// MP GameTypes
enum
{
	MP_TYPE_DEATHMATCH,
	MP_TYPE_TEAMDEATMATCH,
	MP_TYPE_COOP,
	NUM_MP_GAMETYPE
};

typedef struct
{
	int kills;
	int deaths;
	int hits;
	int misses;
} player_stats;

extern player_stats gMPPlayerStats[5];

extern void game_over( void );

// OJW - 20090422
extern BOOLEAN		fClientReceivedAllFiles;

// OJW - 20090507
// Add basic version checking, will only work from now on
// note: this cannot be longer than char[30]
#ifdef JA2UB
#define MPVERSION	"MP v2.0(UB)"
#else
#define MPVERSION	"MP v2.0"
#endif

// OJW - 2009128 - inline funcs for working with soldiers and teams
// sick of confusing myself :)

// this one should be called before passing an ID off the client
inline UINT8 MPEncodeSoldierID( UINT8 ubID )
{
	if ( ubID < 20 )
		return ubID + ubID_prefix; // soldier is ours
	else
		return ubID; // soldier is another teams, dont touch its ID
}

// this one can be called anywhere, even if the ID was not "encoded"
inline UINT8 MPDecodeSoldierID( UINT8 ubID )
{
	if ( ubID >= ubID_prefix && ubID < (ubID_prefix + 7) )
		return ubID - ubID_prefix; // soldier is ours
	else
		return ubID; // soldier is another teams, dont touch its ID
}

inline bool IsOurSoldier (SOLDIERTYPE* pSoldier)
{
	return pSoldier->bTeam == netbTeam || pSoldier->bTeam == 0;
}

inline bool IsOurSoldier (UINT8 ubID)
{
	return (ubID >= ubID_prefix && ubID < (ubID_prefix + 7)) || ubID < 20;
}

