#ifndef _MORALE_H
#define _MORALE_H

#include "Types.h"
#include "Soldier Control.h"

//#define	DEFAULT_MORALE		50

typedef enum
{
	MORALE_KILLED_ENEMY = 0,
	MORALE_SQUADMATE_DIED,
	MORALE_SUPPRESSED,
	MORALE_AIRSTRIKE,
	MORALE_DID_LOTS_OF_DAMAGE,
	MORALE_TOOK_LOTS_OF_DAMAGE, // 5
	MORALE_KILLED_CIVILIAN,
	MORALE_BATTLE_WON,
	MORALE_RAN_AWAY,
	MORALE_HEARD_BATTLE_WON,
	MORALE_HEARD_BATTLE_LOST,		// 10
	MORALE_TOWN_LIBERATED,
	MORALE_TOWN_LOST,
	MORALE_MINE_LIBERATED,
	MORALE_MINE_LOST,
	MORALE_SAM_SITE_LIBERATED,	// 15
	MORALE_SAM_SITE_LOST,
	MORALE_BUDDY_DIED,
	MORALE_HATED_DIED,
	MORALE_TEAMMATE_DIED,
	MORALE_LOW_DEATHRATE,				// 20
	MORALE_HIGH_DEATHRATE,
	MORALE_GREAT_MORALE,
	MORALE_POOR_MORALE,
	MORALE_DRUGS_CRASH,
	MORALE_ALCOHOL_CRASH,				// 25
	MORALE_MONSTER_QUEEN_KILLED,
	MORALE_DEIDRANNA_KILLED,
	MORALE_CLAUSTROPHOBE_UNDERGROUND,
	MORALE_INSECT_PHOBIC_SEES_CREATURE,
	MORALE_NERVOUS_ALONE, // 30
	MORALE_MERC_CAPTURED,
	MORALE_MERC_MARRIED,
	MORALE_QUEEN_BATTLE_WON,
	MORALE_SEX,
	// added by SANDRO
	MORALE_HEAT_INTOLERANT_IN_DESERT, // 35
	MORALE_PSYCHO_UNABLE_TO_PSYCHO,
	MORALE_PACIFIST_GAIN_NONCOMBAT,
	MORALE_MALICIOUS_HIT,

	// added by Flugente
	MORALE_FOOD,
	MORALE_GOOD_FOOD, // 40
	MORALE_BAD_FOOD,
	MORALE_LOATHSOME_FOOD,

	// added by anv
	MORALE_BUDDY_FIRED,
	MORALE_BUDDY_FIRED_EARLY,
	MORALE_BUDDY_FIRED_ON_BAD_TERMS, //45
	MORALE_BAD_EQUIPMENT,
	MORALE_OWED_MONEY,
	MORALE_OWED_MONEY_DAYS,
	MORALE_PLAYER_INACTIVE,
	MORALE_PLAYER_INACTIVE_DAYS,  //50
	MORALE_PLAYER_INACTIVE_DAYS_AGGRESSIVE_BONUS,
	MORALE_PLAYER_INACTIVE_DAYS_PACIFIST_BONUS,
	MORALE_PREVENTED_MISBEHAVIOUR,

	NUM_MORALE_EVENTS,
} MoraleEventNames;

typedef enum
{
	MORALE_MOD_MAX = 0,
	PHOBIC_LIMIT,
	MORALE_MODIFIER_SOCIABLE_ONE_MERC_NEARBY,
	MORALE_MODIFIER_SOCIABLE_NO_MERCS_NEARBY,
	MORALE_MODIFIER_LONER_ONE_MERC_NEARBY,
	MORALE_MODIFIER_LONER_MORE_MERCS_NEARBY,
	MORALE_MODIFIER_AGRESSIVE_VIOLENT_ACTION,
	MORALE_MODIFIER_PACIFIST_VIOLENT_ACTION,
	MORALE_MODIFIER_OPTIMIST,
	MORALE_MODIFIER_SHOWOFF_AROUND,
	MORALE_MODIFIER_DAUNTLESS_SQUADMATE_DIED,
	MORALE_MODIFIER_DAUNTLESS_SUPPRESSED,
	MORALE_MODIFIER_DAUNTLESS_TOOK_LOTS_OF_DAMAGE,
	MORALE_MODIFIER_DAUNTLESS_TEAMMATE_DIED,
	MORALE_MODIFIER_OT_OPTIMIST_GOOD_EVENT,
	MORALE_MODIFIER_OT_AGGRESSIVE_GOOD_EVENT,
	MORALE_MODIFIER_OT_PESSIMIST_GOOD_EVENT,
	MORALE_MODIFIER_OT_OPTIMIST_BAD_EVENT,
	MORALE_MODIFIER_OT_PESSIMIST_BAD_EVENT,
	MORALE_MODIFIER_OT_COWARD_BAD_EVENT,
	MORALE_MODIFIER_MALICIOUS_HOURLY_DECAY,

	NUM_MORALE_MODIFIERS,
} MoraleModifierNames;

typedef enum
{
	DRUG_EFFECT_MORALE_MOD = 0,
	ALCOHOL_EFFECT_MORALE_MOD,

	NUM_DRUG_AND_ALCOHOL_MODIFIERS,
} DrugAndAlcoholModifierNames;

typedef enum
{
	TACTICAL_MORALE_EVENT = 0,
	STRATEGIC_MORALE_EVENT
} MoraleEventType;

typedef struct MoraleEvent
{
	UINT8 ubType;
	INT8	bChange;
} MoraleEvent;

#define MAX_SNITCH_EVENTS			64;
#define MAX_SNITCHES				16;


typedef enum 
{
	SNITCH_INTRODUCTION= 0,
	SNITCH_DEATH_RATE, // current death rate higher than preferable
	SNITCH_REPUTATION, // current reputation worse than preferable
	SNITCH_LOW_MORALE, // merc's morale worse than preferable
	SNITCH_HATED_PERSON, // merc dislikes another person (not necessarily hates him)
	SNITCH_GONNA_QUIT, // merc's contract going to end and he won't renew
	SNITCH_PROGRESS,  // merc thinks player is inactive too long
	SNITCH_OWED_MONEY, // merc is owed money
	
	SNITCH_TEAM_ROTATION, // unused at the moment (will have to add specific rotation tolerance to mercs or something)
						// maybe with personal opinion on rotation depending on opinions (so they get pissed faster when liked mercs are fired)

	NUM_SNITCH_EVENTS,

} SnitchEventType;

typedef struct SnitchEvent
{
	UINT8 ubEventType;
	UINT8 ubSnitchID;
	UINT8 ubTargetProfile;
	UINT8 ubSecondaryTargetProfile;
} SnitchEvent;

// -------- added by Flugente: flags for opinions --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// if someone influences our opinion, STAGE1 gets set. Every day, stages move up one step, until we 'forget' them
// these flags describe what the other guy did to us....
// flagmask 1
#define OPINIONFLAG_STAGE1_FRIENDLYFIRE								0x00000001	//1			// fired on us
#define OPINIONFLAG_STAGE2_FRIENDLYFIRE								0x00000002	//2
#define OPINIONFLAG_STAGE3_FRIENDLYFIRE								0x00000004	//4
#define OPINIONFLAG_STAGE4_FRIENDLYFIRE								0x00000008	//8

#define OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT							0x00000010	//16		// has been snitching to the player about us
#define OPINIONFLAG_STAGE2_SNITCHSOLDMEOUT							0x00000020	//32
#define OPINIONFLAG_STAGE3_SNITCHSOLDMEOUT   						0x00000040	//64
#define OPINIONFLAG_STAGE4_SNITCHSOLDMEOUT							0x00000080	//128

#define OPINIONFLAG_STAGE1_INTERFERENCE								0x00000100	//256		// is interfering with us (stopped us from taking drugs or stealing)
#define OPINIONFLAG_STAGE2_INTERFERENCE								0x00000200	//512
#define OPINIONFLAG_STAGE3_INTERFERENCE								0x00000400	//1024
#define OPINIONFLAG_STAGE4_INTERFERENCE								0x00000800	//2048

#define OPINIONFLAG_STAGE1_FRIENDSWITHHATED							0x00001000	//4096		// is friends with someone we hate
#define OPINIONFLAG_STAGE2_FRIENDSWITHHATED							0x00002000	//8192
#define OPINIONFLAG_STAGE3_FRIENDSWITHHATED							0x00004000	//16384
#define OPINIONFLAG_STAGE4_FRIENDSWITHHATED							0x00008000	//32768

#define OPINIONFLAG_STAGE1_CONTRACTEXTENSION						0x00010000	//65536		// got his contract extended before we did, even though we had less time left
#define OPINIONFLAG_STAGE2_CONTRACTEXTENSION						0x00020000	//131072
#define OPINIONFLAG_STAGE3_CONTRACTEXTENSION						0x00040000	//262144
#define OPINIONFLAG_STAGE4_CONTRACTEXTENSION						0x00080000	//524288

#define OPINIONFLAG_STAGE1_ORDEREDRETREAT							0x00100000	//1048576	// was commander (most senior merc around) and ordered retreat
#define OPINIONFLAG_STAGE2_ORDEREDRETREAT							0x00200000	//2097152
#define OPINIONFLAG_STAGE3_ORDEREDRETREAT							0x00400000	//4194304
#define OPINIONFLAG_STAGE4_ORDEREDRETREAT							0x00800000	//8388608

#define OPINIONFLAG_STAGE1_CIVKILLER								0x01000000	//16777216	// killed an unarmed civilian
#define OPINIONFLAG_STAGE2_CIVKILLER								0x02000000	//33554432
#define OPINIONFLAG_STAGE3_CIVKILLER								0x04000000	//67108864
#define OPINIONFLAG_STAGE4_CIVKILLER								0x08000000	//134217728

#define OPINIONFLAG_STAGE1_SLOWSUSDOWN								0x10000000	//268435456	// is overloaded and thus slows us down
#define OPINIONFLAG_STAGE2_SLOWSUSDOWN								0x20000000	//536870912
#define OPINIONFLAG_STAGE3_SLOWSUSDOWN								0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_SLOWSUSDOWN								0x80000000	//2147483648

// flagmask 2
#define OPINIONFLAG_STAGE1_NOSHARINGFOOD							0x00000001	//1			// had water/food when we needed it. So he did not share!
#define OPINIONFLAG_STAGE2_NOSHARINGFOOD							0x00000002	//2
#define OPINIONFLAG_STAGE3_NOSHARINGFOOD							0x00000004	//4
#define OPINIONFLAG_STAGE4_NOSHARINGFOOD							0x00000008	//8

#define OPINIONFLAG_STAGE1_ANNOYINGDISABILITY						0x00000010	//16		// disability kicked in at a bad time, putting the team at risk (forgeful, nervous, psycho...)
#define OPINIONFLAG_STAGE2_ANNOYINGDISABILITY						0x00000020	//32
#define OPINIONFLAG_STAGE3_ANNOYINGDISABILITY  						0x00000040	//64
#define OPINIONFLAG_STAGE4_ANNOYINGDISABILITY						0x00000080	//128

#define OPINIONFLAG_STAGE1_ADDICT									0x00000100	//256		// is taking drugs 
#define OPINIONFLAG_STAGE2_ADDICT									0x00000200	//512
#define OPINIONFLAG_STAGE3_ADDICT									0x00000400	//1024
#define OPINIONFLAG_STAGE4_ADDICT									0x00000800	//2048

#define OPINIONFLAG_STAGE1_THIEF									0x00001000	//4096		// is stealing items
#define OPINIONFLAG_STAGE2_THIEF									0x00002000	//8192
#define OPINIONFLAG_STAGE3_THIEF									0x00004000	//16384
#define OPINIONFLAG_STAGE4_THIEF									0x00008000	//32768

#define OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER						0x00010000	//65536		// was commander (most senior merc around) when a lot of our people (including militia) died
#define OPINIONFLAG_STAGE2_WORSTCOMMANDEREVER						0x00020000	//131072
#define OPINIONFLAG_STAGE3_WORSTCOMMANDEREVER						0x00040000	//262144
#define OPINIONFLAG_STAGE4_WORSTCOMMANDEREVER						0x00080000	//524288

#define OPINIONFLAG_STAGE1_RICHGUY									0x00100000	//1048576	// gets paid a lot more than we do
#define OPINIONFLAG_STAGE2_RICHGUY									0x00200000	//2097152
#define OPINIONFLAG_STAGE3_RICHGUY									0x00400000	//4194304
#define OPINIONFLAG_STAGE4_RICHGUY									0x00800000	//8388608

#define OPINIONFLAG_STAGE1_BETTERGEAR								0x01000000	//16777216	// has a lot better gear than we do
#define OPINIONFLAG_STAGE2_BETTERGEAR								0x02000000	//33554432
#define OPINIONFLAG_STAGE3_BETTERGEAR								0x04000000	//67108864
#define OPINIONFLAG_STAGE4_BETTERGEAR								0x08000000	//134217728

#define OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS				0x10000000	//268435456	// used our body to rest his gun on
#define OPINIONFLAG_STAGE2_YOUMOUNTEDAGUNONMYBREASTS				0x20000000	//536870912
#define OPINIONFLAG_STAGE3_YOUMOUNTEDAGUNONMYBREASTS				0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_YOUMOUNTEDAGUNONMYBREASTS				0x80000000	//2147483648

// flagmask 3
#define OPINIONFLAG_STAGE1_BANDAGED									0x00000001	//1			// bandaged our wounds
#define OPINIONFLAG_STAGE2_BANDAGED									0x00000002	//2
#define OPINIONFLAG_STAGE3_BANDAGED									0x00000004	//4
#define OPINIONFLAG_STAGE4_BANDAGED									0x00000008	//8

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD						0x00000010	//16		// had a drink with him, he's okay.
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_GOOD						0x00000020	//32
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_GOOD  						0x00000040	//64
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_GOOD						0x00000080	//128

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER						0x00000100	//256		// had a drink with him, he's super okay
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_SUPER						0x00000200	//512
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_SUPER						0x00000400	//1024
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_SUPER						0x00000800	//2048

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD							0x00001000	//4096		// had a drink with him, don't like him
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_BAD							0x00002000	//8192
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_BAD							0x00004000	//16384
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_BAD							0x00008000	//32768

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE						0x00010000	//65536		// had a drink with him, and I REALLY don't like him now
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_WORSE						0x00020000	//131072
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_WORSE						0x00040000	//262144
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_WORSE						0x00080000	//524288

/*#define OPINIONFLAG_STAGE1_RICHGUY									0x00100000	//1048576	// gets paid a lot more than we do
#define OPINIONFLAG_STAGE2_RICHGUY									0x00200000	//2097152
#define OPINIONFLAG_STAGE3_RICHGUY									0x00400000	//4194304
#define OPINIONFLAG_STAGE4_RICHGUY									0x00800000	//8388608

#define OPINIONFLAG_STAGE1_BETTERGEAR								0x01000000	//16777216	// has a lot better gear than we do
#define OPINIONFLAG_STAGE2_BETTERGEAR								0x02000000	//33554432
#define OPINIONFLAG_STAGE3_BETTERGEAR								0x04000000	//67108864
#define OPINIONFLAG_STAGE4_BETTERGEAR								0x08000000	//134217728

#define OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS				0x10000000	//268435456	// used our body to rest his gun on
#define OPINIONFLAG_STAGE2_YOUMOUNTEDAGUNONMYBREASTS				0x20000000	//536870912
#define OPINIONFLAG_STAGE3_YOUMOUNTEDAGUNONMYBREASTS				0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_YOUMOUNTEDAGUNONMYBREASTS				0x80000000	//2147483648*/

#define OPINIONFLAG_STAGE4_ALL										0x88888888				// flags of all final stages, used when rolling over
// ----------------------------------------------------------------

extern void HandleMoraleEvent( SOLDIERTYPE * pSoldier, INT8 bMoraleEvent, INT16 sMapX, INT16 sMapY, INT8 bMapZ );
extern void RefreshSoldierMorale( SOLDIERTYPE * pSoldier );
extern INT8 GetMoraleModifier( SOLDIERTYPE * pSoldier );

void HourlyMoraleUpdate( void );
void HandleSnitchCheck( void );
void HandleSnitchesReports( std::vector<SnitchEvent>& aVec );
void RememberSnitchableEvent( UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile, BOOLEAN fSameGroupOnly, UINT8 ubEventType, std::vector<SnitchEvent>& aVec );
void DailyMoraleUpdate( SOLDIERTYPE *pSoldier );

void DecayTacticalMoraleModifiers( void );

// Flugente: calculate A's opinion of B
INT8	SoldierRelation( SOLDIERTYPE* pSoldierA, SOLDIERTYPE* pSoldierB);

// Flugente: dynamic opinions
//  modify usProfileA's opinion of usProfileB because of usEvent
void AddOpinionEvent( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent );

// get usProfileA's opinion of usProfileB concerning usEvent
INT8 GetDynamicOpinion( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent );

// daily rollover of opinions
void HandleDynamicOpinions();

// a day has passed, 'age' opinions 
void RolloverDynamicOpinions( UINT8 usProfileA );

// check wether other people are friends with someone else we hate. All persons must be in Arulco
void CheckForFriendsofHated( SOLDIERTYPE* pSoldier );

void HandleDynamicOpinionOnContractExtension( UINT8 ubCode, UINT8 usProfile );
void HandleDynamicOpinionCivKill( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionSlowdown( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionWageJealousy( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionDisability( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionAddict( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionThief( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionFoodSharing( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionGear( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionBattleLosses();
void HandleDynamicOpinionRetreat();
void HandleDynamicOpinionTeamDrinking( SOLDIERTYPE* pSoldier );

UINT8 HighestInventoryCoolness( SOLDIERTYPE* pSoldier );

void HandleDynamicOpinionDialogue();

#endif