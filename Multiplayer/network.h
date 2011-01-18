#pragma once

#include "random.h"

//this one just for structs, variables and functions used between the client and server scripts...
extern char cClientName[30];
extern char cServerName[30];
extern bool Sawarded;
extern unsigned int setID;

// ja2_mp.ini: File
#define JA2MP_INI_FILENAME				"ja2_mp.ini"

// "ja2_mp.ini: Sections
#define JA2MP_INI_INITIAL_SECTION		"JA2 Multiplayer Initial Settings"
#define	JA2MP_INI_SERVER_SECTION		"JA2 Multiplayer Servers"

// ja2_mp.ini: Properties
#define JA2MP_ALLOW_CUSTOM_NIV			"INVENTORY_ATTACHMENTS"
#define JA2MP_CIV_ENABLED				"ENABLE_CIVILIANS_IN_COOP"
#define JA2MP_CLIENT_NAME				"PLAYER_NAME"
#define JA2MP_DAMAGE_MULTIPLIER			"WEAPON_DAMAGE"
#define JA2MP_DIFFICULT_LEVEL			"DIFFICULTY_LEVEL"
#define JA2MP_DISABLE_BOBBY_RAYS		"BOBBY_RAYS"
#define JA2MP_FILE_TRANSFER_DIRECTORY	"GAME_DATA_SYNC_DIRECTORY"
#define JA2MP_GAME_MODE					"GAME_TYPE"
#define JA2MP_KIT_BAG					"KIT_BAG"
#define JA2MP_MAX_CLIENTS				"MAX_PLAYERS"
#define JA2MP_MAX_MERCS					"MAX_MERCS"
#define JA2MP_OVERRIDE_MAX_AI			"MAX_ENEMIES_IN_COOP"
#define JA2MP_RANDOM_EDGES				"STARTING_SECTOR_EDGE"
#define JA2MP_RANDOM_MERCS				"HIRE_MERCS_METHOD"
#define JA2MP_REPORT_NAME				"REPORT_HIRED_MERC_NAME"
#define JA2MP_SAME_MERC					"SAME_MERC_ALLOWED"
#define JA2MP_SERVER_IP					"SERVER_IP"
#define JA2MP_SERVER_NAME				"SERVER_NAME"
#define JA2MP_SERVER_PORT				"SERVER_PORT"
#define JA2MP_STARTING_BALANCE			"STARTING_CASH"
#define JA2MP_SYNC_CLIENTS_MP_DIR		"SYNC_GAME_DIRECTORY"
#define JA2MP_TIMED_TURN_SECS_PER_TICK	"TIMED_TURNS"
#define JA2MP_TIME						"STARTING_TIME"
#define	JA2MP_NEW_TRAITS				"SKILL_TRAITS"

typedef struct
{
	UINT8 client_num;
	char client_name[30];
	int team;
	int cl_edge;
	char client_version[30];
}client_info;

typedef struct
{
	UINT8	maxClients;
	UINT8	sameMercAllowed;
	float	damageMultiplier;
	INT16	gsMercArriveSectorX;
	INT16	gsMercArriveSectorY;
	UINT8	enemyEnabled;
	UINT8	creatureEnabled;
	UINT8	militiaEnabled;
	UINT8	civEnabled;
	UINT8	gameType;
	INT32	secondsPerTick;
	INT32	startingCash;
	UINT8	disableBobbyRay;
	UINT8	disableMercEquipment;
	BOOLEAN sofGunNut;	
	UINT8	soubGameStyle;
	UINT8	soubDifficultyLevel;
	UINT8	soubSkillTraits;
	BOOLEAN	sofTurnTimeLimit;
	BOOLEAN	sofIronManMode;
	UINT8	soubBobbyRay;	
	UINT8	maxMercs;
	UINT8	client_num;
	char	client_name[30];
	char	client_names[4][30];	
	int		client_edges[5];
	int		client_teams[4];
	char	server_name[30];	
	int		team;	
	char	kitBag[100];
	UINT8	disableMorale;
	UINT8	reportHiredMerc;
	UINT8	startingSectorEdge;
	float	startingTime;
	UINT8	weaponReadyBonus;
	UINT8	inventoryAttachment;
	UINT8	disableSpectatorMode;
	// OJW - added 20081220
	UINT8	randomStartingEdge;
	UINT8	randomMercs;
	int		random_mercs[7];
	char	server_version[30];
	// OJW - added 20091024
	UINT32	random_table[MAX_PREGENERATED_NUMS];
} settings_struct;

// WANNE: FILE TRANSFER
typedef struct
{
	STRING512 fileTransferDirectory;	
	int syncClientsDirectory;			// Does the server want to sync files to the clients
	char serverName[30];				// The name of the server. This is used on the client side as a folder inside the client transfer directory
	long totalTransferBytes;
} filetransfersettings_struct;

// added 080101 by OJW
typedef struct
{
	INT16 gsMercArriveSectorX;
	INT16 gsMercArriveSectorY;
	float startingTime;
} mapchange_struct;

typedef struct
{
	UINT8 client_num;
	UINT8 newedge;
} edgechange_struct;

typedef struct
{
	UINT8 client_num;
	UINT8 newteam;
} teamchange_struct;

typedef struct
{
	UINT8 client_num;
	UINT8 progress;
	UINT8 downloading;
} progress_struct;

typedef struct
{
	BOOLEAN fMarkInterruptOccurred;
	INT8 bteam;
	UINT8 ubid;

}endINT;

typedef struct
{
	INT8 bteam;
}real_struct;

typedef struct
{
	UINT8 ubStartingTeam;
}sc_struct;

namespace ja2
{
	namespace mp
	{
		void InitializeMultiplayerProfile(vfs::Path const& profileRoot);
	}
}