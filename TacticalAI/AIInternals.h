#include "types.h"
#include "Isometric Utils.h"
#include "Animation Control.h"
#include "Overhead.h"
#include "random.h"
#include "Points.h"


extern BOOLEAN gfTurnBasedAI;

#define MAX_DIST 160
// THIS IS AN ITEM #	- AND FOR NOW JUST COMPLETELY FAKE...

#define MAX_TOSS_SEARCH_DIST	1		// must throw within this of opponent
#define NPC_TOSS_SAFETY_MARGIN	4		// all friends must be this far away

#define ACTING_ON_SCHEDULE( p ) ( (p)->aiData.fAIFlags & AI_CHECK_SCHEDULE )

// the AI should try to have this many APs before climbing a roof, if possible
//THIS IS NOT EXTERNALIZED!!!  CHECK THIS!  GOTTHARD 7/2/08
#define AI_AP_CLIMBROOF 15

#define TEMPORARILY	 0
#define FOREVER		 1

#define IGNORE_PATH			 0
#define ENSURE_PATH			 1
#define ENSURE_PATH_COST		2

//Kris:	November 10, 1997
//Please don't change this value from 10.	It will invalidate all of the maps and soldiers.
//#define MAXPATROLGRIDS	10//dnl ch33 200909 already defined in Soldier Control.h

#define NOWATER		 0
#define WATEROK		 1

#define DONTADDTURNCOST 0
#define ADDTURNCOST	 1

enum
{
	URGENCY_LOW = 0,
	URGENCY_MED,
	URGENCY_HIGH,
	NUM_URGENCY_STATES
};

#define NOWATER		 0
#define WATEROK		 1

#define IGNORE_PATH			 0
#define ENSURE_PATH			 1
#define ENSURE_PATH_COST		2

#define DONTFORCE		0
#define FORCE			1

#define MAX_ROAMING_RANGE		WORLD_COLS

#define PTR_CIV_OR_MILITIA ( PTR_CIVILIAN || (pSoldier->bTeam == MILITIA_TEAM) )

#define REALTIME_AI_DELAY (10000 + Random( 1000 ))
#define REALTIME_CIV_AI_DELAY ( 1000 * (gTacticalStatus.Team[ MILITIA_TEAM ].bMenInSector + gTacticalStatus.Team[ CIV_TEAM ].bMenInSector) + 5000 + 2000 * Random( 3 ) )
#define REALTIME_CREATURE_AI_DELAY ( 10000 + 1000 * Random( 3 ) )

//#define PLAYINGMODE			 0
//#define CAMPAIGNLENGTH			1
//#define LASTUSABLESLOT			2
//#define RANDOMMERCS			 3
//#define AVAILABLEMERCS			4
//#define HIRINGKNOWLEDGE		 5
//#define EQUIPMENTLEVEL			6
//#define ENEMYTEAMSIZE			7
#define ENEMYDIFFICULTY		 8	// this is being used in this module
//#define FOG_OF_WAR				9
//#define TURNLENGTH				10
//#define INCREASEDAP			 11
//#define BLOODSTAINS			 12
//#define STARTINGBALANCE		 13
#define MAXGAMEOPTIONS			14

#define NOSHOOT_WAITABIT		-1
#define NOSHOOT_WATER			-2
#define NOSHOOT_MYSELF			-3
#define NOSHOOT_HURT			-4
#define NOSHOOT_NOAMMO			-5
#define NOSHOOT_NOLOAD			-6
#define NOSHOOT_NOWEAPON		-7

#define PERCENT_TO_IGNORE_THREAT		50		// any less, use threat value
#define ACTION_TIMEOUT_CYCLES	50	// # failed cycles through AI
#define MAX_THREAT_RANGE		400	// 30 tiles worth
#define MIN_PERCENT_BETTER	5	// 5% improvement in cover is good


#define TOSSES_PER_10TURNS		18		// max # of grenades tossable in 10 turns
#define SHELLS_PER_10TURNS		13		// max # of shells	firable	in 10 turns

#define SEE_THRU_COVER_THRESHOLD		5		// min chance to get through

#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))

#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))

typedef struct
{
 SOLDIERTYPE *	pOpponent;
 INT32 sGridNo;
 INT32					iValue;
 INT32					iAPs;
 INT32					iCertainty;
 INT32					iOrigRange;
} THREATTYPE;

// define for bAimTime for bursting
#define BURSTING 20
#define AUTOFIRING 21

typedef struct
{
 UINT8 ubPossible;			// is this attack form possible?	T/F
 UINT8 ubOpponent;			// which soldier is the victim?
 //CHANGED STRUCTURE VALUE BY GOTTHARD 7/14/07
 INT16 ubAimTime;							// how many extra APs to spend on aiming
 INT16 ubChanceToReallyHit;	// chance to hit * chance to get through cover
 //END STRUCTURE CHANGE BY GOTTHARD 7/14/07
 INT32 iAttackValue;			// relative worthiness of this type of attack
 INT32 sTarget;								// target gridno of this attack
 INT8	bTargetLevel;					// target level of this attack
 INT16 ubAPCost;							// how many APs the attack will use up
 INT8	bWeaponIn;							// the inv slot of the weapon in question
} ATTACKTYPE;

extern THREATTYPE Threat[MAXMERCS];
extern int ThreatPercent[10];
extern UINT8 SkipCoverCheck;
extern INT8 GameOption[MAXGAMEOPTIONS];

typedef enum
{
	SEARCH_GENERAL_ITEMS,
	SEARCH_AMMO,
	SEARCH_WEAPONS
} ItemSearchReasons;

// go as far as possible flags
#define FLAG_CAUTIOUS		0x01
#define FLAG_STOPSHORT	0x02

#define STOPSHORTDIST 5

INT16 AdvanceToFiringRange( SOLDIERTYPE * pSoldier, INT16 sClosestOpponent );

BOOLEAN AimingGun(SOLDIERTYPE *pSoldier);
void CalcBestShot(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestShot, BOOLEAN shootUnseen);
void CalcBestStab(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestStab, BOOLEAN fBladeAttack);
void CalcBestThrow(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestThrow);
void CalcTentacleAttack(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestStab );

INT16 CalcSpreadBurst( SOLDIERTYPE * pSoldier, INT16 sFirstTarget, INT8 bTargetLevel );
INT32 CalcManThreatValue(SOLDIERTYPE *pSoldier, INT32 sMyGrid, UINT8 ubReduceForCover, SOLDIERTYPE * pMe );
INT8 CanNPCAttack(SOLDIERTYPE *pSoldier);
void CheckIfTossPossible(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestThrow);
BOOLEAN ClimbingNecessary( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel );
INT8 ClosestPanicTrigger( SOLDIERTYPE * pSoldier );
INT32 ClosestReachableDisturbance(SOLDIERTYPE *pSoldier, UINT8 ubUnconsciousOK, BOOLEAN * pfChangeLevel );
INT32 ClosestReachableFriendInTrouble(SOLDIERTYPE *pSoldier, BOOLEAN * pfClimbingNecessary);
INT32 ClosestSeenOpponent(SOLDIERTYPE *pSoldier, INT32 * psGridNo, INT8 * pbLevel);
void CreatureCall( SOLDIERTYPE * pCaller );
INT8 CreatureDecideAction( SOLDIERTYPE * pCreature );
void CreatureDecideAlertStatus( SOLDIERTYPE *pCreature );
INT8 CrowDecideAction( SOLDIERTYPE * pSoldier );
void DecideAlertStatus( SOLDIERTYPE *pSoldier );
INT8 DecideAutoBandage( SOLDIERTYPE * pSoldier );
UINT16 DetermineMovementMode( SOLDIERTYPE * pSoldier, INT8 bAction );

INT32 EstimateShotDamage(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, INT16 ubChanceToHit);
INT32 EstimateStabDamage(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent,	INT16 ubChanceToHit, BOOLEAN fBladeAttack);
INT32 EstimateThrowDamage(SOLDIERTYPE *pSoldier, UINT8 ubItemPos, SOLDIERTYPE *pOpponent, INT32 sGridNo);
INT16 EstimatePathCostToLocation( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel, BOOLEAN fAddCostAfterClimbingUp, BOOLEAN * pfClimbingNecessary, INT32 * psClimbGridNo );

BOOLEAN FindBetterSpotForItem( SOLDIERTYPE * pSoldier, INT8 bSlot );
INT32 FindClosestClimbPointAvailableToAI( SOLDIERTYPE * pSoldier, INT32 sStartGridNo, INT32 sDesiredGridNo, BOOLEAN fClimbUp );
INT32 FindRouteBackOntoMap( SOLDIERTYPE * pSoldier, INT32 sDestGridNo );
INT32 FindClosestBoxingRingSpot( SOLDIERTYPE * pSoldier, BOOLEAN fInRing );
INT32 GetInterveningClimbingLocation( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel, BOOLEAN * pfClimbingNecessary );
UINT8 GetTraversalQuoteActionID( INT8 bDirection );
INT32 GoAsFarAsPossibleTowards(SOLDIERTYPE *pSoldier, INT32 sDesGrid, INT8 bAction);

INT8 HeadForTheStairCase( SOLDIERTYPE * pSoldier );

BOOLEAN InGas( SOLDIERTYPE *pSoldier, INT32 sGridNo );
BOOLEAN InGasOrSmoke( SOLDIERTYPE *pSoldier, INT32 sGridNo );
BOOLEAN InWaterGasOrSmoke( SOLDIERTYPE *pSoldier, INT32 sGridNo );

void InitAttackType(ATTACKTYPE *pAttack);

INT32 InternalGoAsFarAsPossibleTowards(SOLDIERTYPE *pSoldier, INT32 sDesGrid, INT16 bReserveAPs, INT8 bAction, INT8 fFlags );

int LegalNPCDestination(SOLDIERTYPE *pSoldier, INT32 sGridno, UINT8 ubPathMode, UINT8 ubWaterOK, UINT8 fFlags);
void LoadWeaponIfNeeded(SOLDIERTYPE *pSoldier);
INT32 MostImportantNoiseHeard( SOLDIERTYPE *pSoldier, INT32 *piRetValue, BOOLEAN * pfClimbingNecessary, BOOLEAN * pfReachable );
INT32 NPCConsiderInitiatingConv( SOLDIERTYPE * pNPC, UINT8 * pubDesiredMerc );
void NPCDoesAct(SOLDIERTYPE *pSoldier);
void NPCDoesNothing(SOLDIERTYPE *pSoldier);
INT8 OKToAttack(SOLDIERTYPE *ptr, int target);
BOOLEAN NeedToRadioAboutPanicTrigger( void );
INT8 PointPatrolAI(SOLDIERTYPE *pSoldier);
void PossiblyMakeThisEnemyChosenOne( SOLDIERTYPE * pSoldier );
INT8 RandomPointPatrolAI(SOLDIERTYPE *pSoldier);
INT32 RangeChangeDesire( SOLDIERTYPE * pSoldier );
UINT16 RealtimeDelay( SOLDIERTYPE * pSoldier );
void RearrangePocket(SOLDIERTYPE *pSoldier, INT8 bPocket1, INT8 bPocket2, UINT8 bPermanent);
void RTHandleAI( SOLDIERTYPE * pSoldier );
UINT16 RunAway( SOLDIERTYPE * pSoldier );
INT8	SearchForItems( SOLDIERTYPE * pSoldier, INT8 bReason, UINT16 usItem );
UINT8 ShootingStanceChange( SOLDIERTYPE * pSoldier, ATTACKTYPE * pAttack, INT8 bDesiredDirection );
UINT8 StanceChange( SOLDIERTYPE * pSoldier, INT16 ubAttackAPCost );
INT32 TrackScent( SOLDIERTYPE * pSoldier );
void RefreshAI(SOLDIERTYPE *pSoldier);
BOOLEAN InLightAtNight( INT32 sGridNo, INT8 bLevel );
INT16 FindNearbyDarkerSpot( SOLDIERTYPE *pSoldier );

BOOLEAN ArmySeesOpponents( void );

void CheckIfShotPossible(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestShot, BOOLEAN suppressionFire);

INT32 FindBestCoverNearTheGridNo(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubSearchRadius );

INT8 FindDirectionForClimbing( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel);

// HEADROCK HAM B2.7: Functions to assist group AI
// WANNE: Headrock informed me that I should disable these 3 functions in code, because they need a lot of CPU during AI calculation.
INT16 AssessTacticalSituation( INT8 bSide );
BOOLEAN TeamSeesOpponent( INT8 bSide, SOLDIERTYPE * pOpponent );
INT32 CalcStraightThreatValue( SOLDIERTYPE *pEnemy );

// SANDRO - added following functions
BOOLEAN DoctorIsPresent( SOLDIERTYPE * pPatient, BOOLEAN fOnDoctorAssignmentCheck );
BOOLEAN AIDetermineStealingWeaponAttempt( SOLDIERTYPE * pSoldier, SOLDIERTYPE * ubOpponent );

// HEADROCK HAM 4: Calculate average recoil offset for a shot.
FLOAT AICalcRecoilForShot( SOLDIERTYPE *pSoldier, OBJECTTYPE *pWeapon, UINT8 ubShotNum);


// HEADROCK HAM 4: Calculate average recoil offset for a shot.
FLOAT AICalcRecoilForShot( SOLDIERTYPE *pSoldier, OBJECTTYPE *pWeapon, UINT8 ubShotNum);
