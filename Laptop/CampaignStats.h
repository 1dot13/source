#ifndef __CAMPAIGNSTATS_H
#define __CAMPAIGNSTATS_H

/** 
 * @file
 * @author Flugente (bears-pit.com)
 * @brief Define data structures for campaign statistics. These statistics are updated throughout the campaign and can be viewed in the laptop websites.
 */

#include "types.h"
#include "Soldier Control.h"

// -------- added by Flugente: various flags for incidents --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define INCIDENT_ARTILLERY_ENEMY				0x00000001	//1			// side used artillery
#define INCIDENT_ARTILLERY_PLAYERSIDE			0x00000002	//2			// 
#define INCIDENT_TANKS_ENEMY					0x00000004	//4			// side used tanks
#define INCIDENT_TANKS_PLAYERSIDE				0x00000008	//8			// 

#define INCIDENT_MUSTARDGAS_ENEMY				0x00000010	//16		// side used mustard gas
#define INCIDENT_MUSTARDGAS_PLAYERSIDE			0x00000020	//32		// 
#define INCIDENT_ATTACK_ENEMY   				0x00000040	//64		// side began the attack
#define INCIDENT_ATTACK_PLAYERSIDE				0x00000080	//128		// 

#define INCIDENT_REINFORCEMENTS_ENEMY			0x00000100	//256		// side was reinforced
#define INCIDENT_REINFORCEMENTS_PLAYERSIDE		0x00000200	//512		// 
#define INCIDENT_SNIPERS_ENEMY					0x00000400	//1024		// side used snipers
#define INCIDENT_SNIPERS_PLAYERSIDE				0x00000800	//2048		// 

#define INCIDENT_AMBUSH							0x00001000	//4096		// player was ambushed
#define INCIDENT_AIRDROP						0x00002000	//8192		// player airdropped
#define INCIDENT_SURGERY						0x00004000	//16384		// player performed surgery
#define INCIDENT_BUILDINGS_DAMAGED				0x00008000	//32768		// buildings were damaged in the fighting

#define INCIDENT_ATTACKDIR_NORTH				0x00010000	//65536		// direction attackers came from
#define INCIDENT_ATTACKDIR_WEST					0x00020000	//131072	// 
#define INCIDENT_ATTACKDIR_SOUTH				0x00040000	//262144	// 
#define INCIDENT_ATTACKDIR_EAST					0x00080000	//524288	//

#define INCIDENT_ATTACKDIR_NORTH_ENEMY			0x00100000	//1048576	//
#define INCIDENT_ATTACKDIR_WEST_ENEMY			0x00200000	//2097152	//
#define INCIDENT_ATTACKDIR_SOUTH_ENEMY			0x00400000	//4194304	//
#define INCIDENT_ATTACKDIR_EAST_ENEMY			0x00800000	//8388608	//

#define INCIDENT_WIN							0x01000000	//16777216	// player won battle (otherwise he lost)
#define INCIDENT_OMERTA_LANDING					0x02000000	//33554432	// omerta landing
#define INCIDENT_FIRST_LIBERATION				0x04000000	//67108864	// first time we free a sector
/*#define PLAYER_NET_4_LVL_3		0x08000000	//134217728

#define PLAYER_NET_1_LVL_4		0x10000000	//268435456
#define PLAYER_NET_2_LVL_4		0x20000000	//536870912
#define WH40K_SOLDIER_ILLUSION				0x40000000	//1073741824	// Soldier is an Illusion
#define WH40K_SOLDIER_KILLTHISTURN			0x80000000	//2147483648	// Soldier is on a kill streak*/

#define INCIDENT_PLAYER_ALLDIRS		(INCIDENT_ATTACKDIR_NORTH|INCIDENT_ATTACKDIR_WEST|INCIDENT_ATTACKDIR_SOUTH|INCIDENT_ATTACKDIR_EAST)
#define INCIDENT_ENEMY_ALLDIRS		(INCIDENT_ATTACKDIR_NORTH_ENEMY|INCIDENT_ATTACKDIR_WEST_ENEMY|INCIDENT_ATTACKDIR_SOUTH_ENEMY|INCIDENT_ATTACKDIR_EAST_ENEMY)
// ----------------------------------------------------------------

// -------- added by Flugente: flags for one time events that commence an individual report --------
// the numbering of these flags is important. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define INCIDENT_ONETIMEEVENT_OMERTA							0x00000001	// mercenary forces land in Omerta, the campaign begins
#define INCIDENT_ONETIMEEVENT_DEATH_KINGPIN						0x00000002	// kingpin was killed in battle
#define INCIDENT_ONETIMEEVENT_MASSACRE_HICKS					0x00000004	// hicks clan has been massacred
#define INCIDENT_ONETIMEEVENT_MASSACRE_BLOODCATS				0x00000008	// bloodcat lair was raided

#define INCIDENT_ONETIMEEVENT_CITY_RETAKEN						0x00000010	// the army has retaken a city
#define INCIDENT_ONETIMEEVENT_CITY_LIBERATED					0x00000020	// we liberated a city for the first time
/*#define CORPSE_HAIR_WHITE   	0x00000040	//64
#define CORPSE_HAIR_BLOND		0x00000080	//128

#define CORPSE_NO_ZOMBIE_RISE	0x00000100	//256		// no zombie can rise from this corpse
#define CORPSE_SKIN_PINK		0x00000200	//512
#define CORPSE_SKIN_TAN			0x00000400	//1024
#define CORPSE_SKIN_DARK		0x00000800	//2048

#define CORPSE_SKIN_BLACK		0x00001000	//4096
#define CORPSE_VEST_BROWN       0x00002000	//8192
#define CORPSE_VEST_grey 		0x00004000	//16384
#define CORPSE_VEST_GREEN		0x00008000	//32768

#define CORPSE_VEST_JEAN		0x00010000	//65536
#define CORPSE_VEST_RED			0x00020000	//131072
#define CORPSE_VEST_BLUE		0x00040000	//262144
#define CORPSE_VEST_YELLOW		0x00080000	//524288

#define CORPSE_VEST_WHITE		0x00100000	//1048576
#define CORPSE_VEST_BLACK		0x00200000	//2097152
#define CORPSE_VEST_GYELLOW		0x00400000	//4194304
#define CORPSE_VEST_PURPLE		0x00800000	//8388608

#define CORPSE_PANTS_GREEN		0x01000000	//16777216
#define CORPSE_PANTS_JEAN		0x02000000	//33554432
#define CORPSE_PANTS_TAN		0x04000000	//67108864
#define CORPSE_PANTS_BLACK		0x08000000	//134217728

#define CORPSE_PANTS_BLUE		0x10000000	//268435456
#define CORPSE_PANTS_BEIGE		0x20000000	//536870912
#define CORPSE_NO_VEST			0x40000000	//1073741824		// corpse has no vest item (it has been either taken or been destroyed)
#define CORPSE_NO_PANTS			0x80000000	//2147483648		// corpse has no pants item/for tripwire activation (gets set and unset when activating tripwire)*/
// ----------------------------------------------------------------

#define MAX_CAMPAIGNSTATSEVENTS_TEXTS	10
#define NUM_CAMPAIGNSTATSEVENTS			20

typedef struct
{
	UINT16		uiIndex;
	CHAR16		szText[MAX_CAMPAIGNSTATSEVENTS_TEXTS][300];			// Up to 10 sentences can be defined

	UINT8		usCityTaken;										// if > 0, this is a text for the liberation of a city (defined in Cities.xml)
} CAMPAIGNSTATSEVENT;

extern CAMPAIGNSTATSEVENT zCampaignStatsEvent[NUM_CAMPAIGNSTATSEVENTS];

enum
{
	CAMPAIGNHISTORY_SD_MERC,
	CAMPAIGNHISTORY_SD_MILITIA_GREEN,
	CAMPAIGNHISTORY_SD_MILITIA_REGULAR,
	CAMPAIGNHISTORY_SD_MILITIA_ELITE,

	CAMPAIGNHISTORY_SD_CIV,

	CAMPAIGNHISTORY_SD_ENEMY_ADMIN,
	CAMPAIGNHISTORY_SD_ENEMY_ARMY,
	CAMPAIGNHISTORY_SD_ENEMY_ELITE,
	CAMPAIGNHISTORY_SD_ENEMY_TANK,

	CAMPAIGNHISTORY_SD_CREATURE,
	CAMPAIGNHISTORY_SD_ZOMBIE,

	CAMPAIGNHISTORY_SD_MAX,
};

enum
{
	CAMPAIGNHISTORY_TYPE_KILL,
	CAMPAIGNHISTORY_TYPE_WOUND,
	CAMPAIGNHISTORY_TYPE_PRISONER,
	CAMPAIGNHISTORY_TYPE_SHOT,
	CAMPAIGNHISTORY_TYPE_PARTICIPANT,
	CAMPAIGNHISTORY_TYPE_PROMOTION,

	CAMPAIGNHISTORY_TYPE_MAX,
};

enum {
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_COASTAL,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_DESERT,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_FOREST,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MOUNTAIN,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_POLAR,	
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_RIVER,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_SPARSE,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_SWAMP,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_TROPICAL,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_UNDERGROUND,
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_URBAN,
	
	CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MAX,
};

enum {
	CAMPAINGHISTORY_PICLIBRARY_TYPE_WIN,
	CAMPAINGHISTORY_PICLIBRARY_TYPE_LOSS,
	CAMPAINGHISTORY_PICLIBRARY_TYPE_INSTALLATION,
	CAMPAINGHISTORY_PICLIBRARY_TYPE_AIRDROP,
	CAMPAINGHISTORY_PICLIBRARY_TYPE_TANKS,

	CAMPAINGHISTORY_PICLIBRARY_TYPE_MAX,
};

class Incident_Stats
{
public:
	Incident_Stats() {}
	~Incident_Stats()	{}

	void clear();

	// determine how 'interesting' an incident is - the more happens, the more interesting it is
	void	CalcInterestRating();

	BOOLEAN		Save( HWFILE hFile );
	BOOLEAN		Load( HWFILE hwFile );

	void	AddStat( SOLDIERTYPE* pSoldier, UINT8 aType );

	STR16	GetAttackerDirString( BOOLEAN fAttacker );

	void	GetTerrainandType(UINT8& arTerrain, UINT8& arType);

public:
	// unique data of incident
	UINT32				usID;
	UINT32				usTime;
	UINT8				usSector;
	UINT8				usLevel;

	// every incident has a rating, which signifies how 'interesting' it is. We use this to show
	// the most interesting incidents of the entire campaign on the website - 'milestones' of the war
	UINT32				usInterestRating;

	// NPC involvement
	UINT8				usNPCDied;
	UINT8				usCivFactionFought;

	// various notable properties of this incident
	UINT64				usIncidentFlags;

	// some events happen only once and get special coverage (like the initial Omerta landing, the Drassenn lieration, Kingpin#s death etc.).
	// In those cases, we use special text and picture defined in an xml.
	UINT64				usOneTimeEventFlags;

	// total number of fallen
	UINT16				usKills[CAMPAIGNHISTORY_SD_MAX];

	// total number of wounded
	UINT16				usWounds[CAMPAIGNHISTORY_SD_MAX];

	// total number of prisoners
	UINT16				usPrisoners[CAMPAIGNHISTORY_SD_MAX];

	// shots fired
	UINT16				usShots[CAMPAIGNHISTORY_SD_MAX];

	// total number of participants
	UINT16				usParticipants[CAMPAIGNHISTORY_SD_MAX];

	// total number of promotions
	UINT16				usPromotions[CAMPAIGNHISTORY_SD_MAX];

	// reserve
	UINT8				usFiller[40];

	char endOfPOD;	// marker for end of POD (plain old data)
};

#define SIZEOF_INCIDENT_STATS_POD offsetof( Incident_Stats, endOfPOD )

enum {
	CAMPAIGN_MONEY_START,
	CAMPAIGN_MONEY_MINES,
	CAMPAIGN_MONEY_TRADE,
	CAMPAIGN_MONEY_ETC,

	CAMPAIGN_MONEY_MAX,
};

enum {
	CAMPAIGN_CONSUMED_AMMO,
	CAMPAIGN_CONSUMED_EXPLOSIVES,
	CAMPAIGN_CONSUMED_FOOD,
	CAMPAIGN_CONSUMED_MEDICAL,
	CAMPAIGN_CONSUMED_REPAIR,

	CAMPAIGN_CONSUMED_MAX,
};

#define CAMPAIGNSTATS_NUM_LAST_INCIDENTS	20
#define CAMPAIGNSTATS_NUM_MOST_IMPORTANT	20

class Campaign_Stats
{
public:
	Campaign_Stats();
	~Campaign_Stats();

	void clear();

	//Campaign_Stats* GetObject();	// TODO: ist das überhaupt nötig?

	BOOLEAN		Save( HWFILE hFile );
	BOOLEAN		Load( HWFILE hwFile );

	void	AddNewIncident(Incident_Stats arIncident);
	void	AddMoneyEarned(UINT8 aType, INT32 aVal);
	void	AddConsumption(UINT8 aType, FLOAT aVal);

public:
	// total number of fallen
	UINT32				usKills[CAMPAIGNHISTORY_SD_MAX];

	// total number of wounded
	UINT32				usWounds[CAMPAIGNHISTORY_SD_MAX];

	// total number of prisoners
	UINT32				usPrisoners[CAMPAIGNHISTORY_SD_MAX];

	// shots fired
	UINT32				usShots[CAMPAIGNHISTORY_SD_MAX];

	// money earned
	INT32				sMoneyEarned[CAMPAIGN_MONEY_MAX];

	// amount of items consumed (e.g. how many kilos of ammo did we fire?)
	FLOAT				usConsumed[CAMPAIGN_CONSUMED_MAX];

	// number of incidents
	UINT32				usNumIncidents;

	// highest ID
	UINT32				usHighestID;

	// reserve
	UINT8				usFiller[40];

	char endOfPOD;	// marker for end of POD (plain old data)

	// vector of all incidents
	std::vector<Incident_Stats>	mIncidentVector;
	
private:
	//Campaign_Stats*		mspSelf;
};

#define SIZEOF_CAMPAIGN_STATS_POD offsetof( Campaign_Stats, endOfPOD )

extern Campaign_Stats	gCampaignStats;
extern Incident_Stats	gCurrentIncident;		// we might save during an incident, thus we have to store the ongoing incident

// add this incident to the campaign stats and then clear it
void FinishIncident(INT16 sX, INT16 sY, INT8 sZ);

// start a new incident
void StartIncident(INT16 sX, INT16 sY, INT8 sZ);

#endif	// __CAMPAIGNSTATS_H