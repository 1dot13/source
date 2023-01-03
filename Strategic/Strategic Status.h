#ifndef __STRATEGIC_STATUS_H
#define __STRATEGIC_STATUS_H

#include "types.h"
//#include "Item Types.h"
#include "Soldier Control.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


//Enemy is allowed to capture the player after certain day
#define STARTDAY_ALLOW_PLAYER_CAPTURE_FOR_RESCUE		4

#define STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE		0x00000001
#define STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE		0x00000002

#define ARMY_GUN_LEVELS		11

// anv: externalised, now these serve as enums below
// player reputation modifiers
//#define REPUTATION_LOW_DEATHRATE				+5
//#define REPUTATION_HIGH_DEATHRATE				-5
//#define REPUTATION_GREAT_MORALE					+3
//#define REPUTATION_POOR_MORALE					-3
//#define REPUTATION_BATTLE_WON						+2
//#define REPUTATION_BATTLE_LOST					-2
//#define REPUTATION_TOWN_WON							+5
//#define REPUTATION_TOWN_LOST						-5
//#define REPUTATION_SOLDIER_DIED					-2			// per exp. level
//#define REPUTATION_SOLDIER_CAPTURED			-1
//#define REPUTATION_KILLED_CIVILIAN			-5
//#define REPUTATION_EARLY_FIRING					-3
//#define REPUTATION_KILLED_MONSTER_QUEEN	+15
//#define REPUTATION_KILLED_DEIDRANNA			+25

typedef enum
{
	REPUTATION_LOW_DEATHRATE = 0,
	REPUTATION_HIGH_DEATHRATE,
	REPUTATION_GREAT_MORALE,
	REPUTATION_POOR_MORALE,
	REPUTATION_BATTLE_WON,
	REPUTATION_BATTLE_LOST,			// 5
	REPUTATION_TOWN_WON,
	REPUTATION_TOWN_LOST,
	REPUTATION_SOLDIER_DIED,
	REPUTATION_SOLDIER_CAPTURED,
	REPUTATION_KILLED_CIVILIAN,		// 10
	REPUTATION_EARLY_FIRING,
	REPUTATION_KILLED_MONSTER_QUEEN,
	REPUTATION_KILLED_DEIDRANNA,
	REPUTATION_MERC_COMPLAIN_EQUIPMENT,
	REPUTATION_MERC_OWED_MONEY,		// 15
	REPUTATION_PLAYER_IS_INACTIVE,
	REPUTATION_MERC_FIRED_ON_BAD_TERMS,

	NUM_REPUTATION_EVENTS,
} ReputationEventNames;

// flags to remember whether a certain E-mail has already been sent out
#define ENRICO_EMAIL_SENT_SOME_PROGRESS		0x0001
#define ENRICO_EMAIL_SENT_ABOUT_HALFWAY		0x0002
#define ENRICO_EMAIL_SENT_NEARLY_DONE			0x0004
#define ENRICO_EMAIL_SENT_MINOR_SETBACK		0x0008
#define ENRICO_EMAIL_SENT_MAJOR_SETBACK		0x0010
#define ENRICO_EMAIL_SENT_CREATURES				0x0020
#define ENRICO_EMAIL_FLAG_SETBACK_OVER		0x0040
#define ENRICO_EMAIL_SENT_LACK_PROGRESS1	0x0080
#define ENRICO_EMAIL_SENT_LACK_PROGRESS2	0x0100
#define ENRICO_EMAIL_SENT_LACK_PROGRESS3	0x0200
#define ENRICO_EMAIL_SENT_PMC				0x0400
#define ENRICO_EMAIL_SENT_MILITIAROSTER		0x0800

// progress threshold that control Enrico E-mail timing
#define SOME_PROGRESS_THRESHOLD						20
#define ABOUT_HALFWAY_THRESHOLD						55
#define NEARLY_DONE_THRESHOLD							80
#define MINOR_SETBACK_THRESHOLD						5
#define MAJOR_SETBACK_THRESHOLD						15

#define NEW_SECTORS_EQUAL_TO_ACTIVITY 4

// enemy ranks
enum
{
	ENEMY_RANK_ADMIN,
	ENEMY_RANK_TROOP,
	ENEMY_RANK_ELITE,
	//ENEMY_RANK_TANK,
	NUM_ENEMY_RANKS
};

// ways enemies can be killed
enum
{
	ENEMY_KILLED_IN_TACTICAL,
	ENEMY_KILLED_IN_AUTO_RESOLVE,
	ENEMY_KILLED_TOTAL,
	NUM_WAYS_ENEMIES_KILLED
};


typedef struct STRATEGIC_STATUS
{
	UINT32		uiFlags;
	UINT8			ubNumCapturedForRescue;

	UINT8			ubHighestProgress;			// the highest level of progress player has attained thus far in the game (0-100)

	UINT8			ubStandardArmyGunIndex[ARMY_GUN_LEVELS];		// type of gun in each group that Queen's army is using this game
	BOOLEAN		fWeaponDroppedAlready[MAXITEMS];				// flag that tracks whether this weapon type has been dropped before 

	UINT8			ubMercDeaths;						// how many soldiers have bit it while in the player's employ (0-100)
	UINT32		uiManDaysPlayed;				// once per day, # living mercs on player's team is added to this running total

	UINT8			ubBadReputation;				// how bad a reputation player has earned through his actions, performance, etc. (0-100)

	UINT16		usEnricoEmailFlags;		// bit flags that control progress-related E-mails from Enrico

	UINT8			ubInsuranceInvestigationsCnt;		// how many times merc has been investigated for possible insurance fraud

	UINT8			ubUnhiredMercDeaths;		// how many mercs have died while NOT working for the player

	UINT16		usPlayerKills;					// kills achieved by all mercs controlled by player together.	*Excludes* militia kills!

	UINT16		usEnemiesKilled[NUM_WAYS_ENEMIES_KILLED][NUM_ENEMY_RANKS];	// admin/troop/elite.	Includes kills by militia, too
	UINT16		usLastDayOfPlayerActivity;
	UINT8			ubNumNewSectorsVisitedToday;
	UINT8			ubNumberOfDaysOfInactivity;

	// Flugente: VIPs
	UINT8		usVIPsTotal;
	UINT8		usVIPsLeft;

	INT8 bPadding[68];

} STRATEGIC_STATUS;


void InitStrategicStatus();

extern STRATEGIC_STATUS	gStrategicStatus;


BOOLEAN SaveStrategicStatusToSaveGameFile( HWFILE hFile );

BOOLEAN LoadStrategicStatusFromSaveGameFile( HWFILE hFile );

UINT8 CalcDeathRate(void);

void ModifyPlayerReputation(INT8 bRepChange);

BOOLEAN MercThinksDeathRateTooHigh( UINT8 ubProfileID );
BOOLEAN MercThinksBadReputationTooHigh( UINT8 ubProfileID );
BOOLEAN MercThinksHisMoraleIsTooLow( SOLDIERTYPE *pSoldier );
BOOLEAN MercIsOwedTooMuchMoney( UINT8 ubProfileID );
BOOLEAN MercThinksPlayerIsInactiveTooLong( UINT8 ubProfileID );

void HandleEnricoEmail(void);

void TrackEnemiesKilled( UINT8 ubKilledHow, UINT8 ubSoldierClass );

INT8 SoldierClassToRankIndex( UINT8 ubSoldierClass );
UINT8 RankIndexToSoldierClass( UINT8 ubRankIndex );

void UpdateLastDayOfPlayerActivity( UINT16 usDay );

#endif