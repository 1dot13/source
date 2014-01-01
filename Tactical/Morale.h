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
	MORALE_PLAYER_INACTIVE_DAYS,
	MORALE_PLAYER_INACTIVE_DAYS_AGGRESSIVE_BONUS,
	MORALE_PLAYER_INACTIVE_DAYS_PACIFIST_BONUS,

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

extern void HandleMoraleEvent( SOLDIERTYPE * pSoldier, INT8 bMoraleEvent, INT16 sMapX, INT16 sMapY, INT8 bMapZ );
extern void RefreshSoldierMorale( SOLDIERTYPE * pSoldier );
extern INT8 GetMoraleModifier( SOLDIERTYPE * pSoldier );

void HourlyMoraleUpdate( void );
void HandleSnitchCheck( void );
void HandleSnitchesReports( SnitchEvent *SnitchEvents, UINT8 ubSnitchEventsCounter );
INT8 RememberSnitchableEvent( UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile, BOOLEAN fSameGroupOnly, UINT8 ubEventType, SnitchEvent SnitchEvents[], INT8 ubSnitchEventsCounter );
void DailyMoraleUpdate( SOLDIERTYPE *pSoldier );

void DecayTacticalMoraleModifiers( void );

// Flugente: calculate A's opinion of B
INT8	SoldierRelation( SOLDIERTYPE* pSoldierA, SOLDIERTYPE* pSoldierB);

#endif