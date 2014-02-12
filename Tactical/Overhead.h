#ifndef __OVERHEAD_H
#define __OVERHEAD_H

#include <string.h>
#include <stdio.h>
#include "Soldier Control.h"
#include "overhead types.h"
#include "soldier find.h"
#include "Campaign Types.h"		// added by Flugente for SECTORINFO and UNDERGROUND_SECTORINFO
#define             ADD_SOLDIER_NO_PROFILE_ID       200

#define MAX_REALTIME_SPEED_VAL                      10

// Enums for waiting for mercs to finish codes
enum
{   
    NO_WAIT_EVENT = 0,
    WAIT_FOR_MERCS_TO_WALKOFF_SCREEN,
    WAIT_FOR_MERCS_TO_WALKON_SCREEN,
    WAIT_FOR_MERCS_TO_WALK_TO_GRIDNO
};

// TACTICAL ENGINE STATUS FLAGS
typedef struct
{
    UINT8           bFirstID;
    UINT8           bLastID;
    COLORVAL    RadarColor;
    INT8        bSide;
    INT8            bMenInSector;
    UINT8           ubLastMercToRadio;
    INT8        bTeamActive;
    INT8        bAwareOfOpposition;
    INT8        bHuman;
} TacticalTeamType;

//for use with TacticalStatusType.ubEnemyIntention 
enum
{
    INTENTION_SCOUTING,
    INTENTION_PATROLLING,
    INTENTION_ATTACKING,
    INTENTION_DEFENDING,
    INTENTION_RETREATING
};

//for use with TacticalStatusType.ubEnemyIntendedRetreatDirection 
enum
{
    RETREAT_NORTH,
    RETREAT_EAST,
    RETREAT_SOUTH,
    RETREAT_WEST
};

#define PANIC_BOMBS_HERE        0x01
#define PANIC_TRIGGERS_HERE     0x02
#define NUM_PANIC_TRIGGERS      3
#define ENEMY_OFFERED_SURRENDER 0x01

struct TacticalStatusType
{
    UINT32                          uiFlags;
    TacticalTeamType        Team[ MAXTEAMS ];
    UINT8                               ubCurrentTeam;
    INT32                               sSlideTarget;
    INT16                               sSlideReason;
    UINT32                          uiTimeSinceMercAIStart;
    INT8                                fPanicFlags;
    INT32                               sPanicTriggerGridNoUnused;
    INT16                               sHandGrid;
    UINT8                               ubSpottersCalledForBy;
    UINT8                               ubTheChosenOne;
    UINT32                          uiTimeOfLastInput;
    UINT32                          uiTimeSinceDemoOn;
    UINT32                          uiCountdownToRestart;
    BOOLEAN                         fGoingToEnterDemo;
    BOOLEAN                         fNOTDOLASTDEMO;
    BOOLEAN                         fMultiplayer;
    BOOLEAN                         fCivGroupHostile[ NUM_CIV_GROUPS ];
    UINT8                               ubLastBattleSectorX;                    
    UINT8                               ubLastBattleSectorY;
    BOOLEAN                         fLastBattleWon;
    INT8                                bOriginalSizeOfEnemyForce;
    INT8                                bPanicTriggerIsAlarmUnused;
    BOOLEAN                         fVirginSector;
    BOOLEAN                         fEnemyInSector;
    BOOLEAN                         fInterruptOccurred;
    INT8                                bRealtimeSpeed;
    UINT8                               ubEnemyIntention;
    UINT8                               ubEnemyIntendedRetreatDirection;
    UINT8                               ubEnemySightingOnTheirTurnEnemyID;
    UINT8                               ubEnemySightingOnTheirTurnPlayerID;
    BOOLEAN                         fEnemySightingOnTheirTurn;
    BOOLEAN                         fAutoBandageMode;
    UINT8                               ubAttackBusyCount;
    INT8                                bNumEnemiesFoughtInBattleUnused;
    UINT8                               ubEngagedInConvFromActionMercID;
    UINT16                          usTactialTurnLimitCounter;
    BOOLEAN                         fInTopMessage;
    UINT8                               ubTopMessageType;
    CHAR16                              zTopMessageString[20];
    UINT16                          usTactialTurnLimitMax;
    UINT32                          uiTactialTurnLimitClock;
    BOOLEAN                         fTactialTurnLimitStartedBeep;
    INT8                                bBoxingState;
    INT8                                bConsNumTurnsNotSeen;
    UINT8                               ubArmyGuysKilled;

    INT32                               sPanicTriggerGridNo[ NUM_PANIC_TRIGGERS ];
    INT8                                bPanicTriggerIsAlarm[ NUM_PANIC_TRIGGERS ];
    UINT8                               ubPanicTolerance[ NUM_PANIC_TRIGGERS ];
    BOOLEAN                         fAtLeastOneGuyOnMultiSelect;
    BOOLEAN                         fSaidCreatureFlavourQuote;
    BOOLEAN                         fHaveSeenCreature;
    BOOLEAN                         fKilledEnemyOnAttack;
    UINT8                               ubEnemyKilledOnAttack;
    INT8                                bEnemyKilledOnAttackLevel;
    UINT16                          ubEnemyKilledOnAttackLocation;
    BOOLEAN                         fItemsSeenOnAttack;
    BOOLEAN                         ubItemsSeenOnAttackSoldier;
    BOOLEAN                         fBeenInCombatOnce;
    BOOLEAN                         fSaidCreatureSmellQuote;
    UINT32                          usItemsSeenOnAttackGridNo;
    BOOLEAN                         fLockItemLocators;
    UINT8                               ubLastQuoteSaid;
    UINT8                               ubLastQuoteProfileNUm;
    BOOLEAN                         fCantGetThrough;
    INT32                               sCantGetThroughGridNo;
    INT32                               sCantGetThroughSoldierGridNo;
    UINT8                               ubCantGetThroughID;
    BOOLEAN                         fDidGameJustStart;
    BOOLEAN                         fStatChangeCheatOn;
    UINT8                               ubLastRequesterTargetID;
    BOOLEAN                         fGoodToAllowCrows;
    UINT8                               ubNumCrowsPossible;
    UINT32                          uiTimeCounterForGiveItemSrc;
    BOOLEAN                         fUnLockUIAfterHiddenInterrupt;
    INT8                                bNumFoughtInBattle[ MAXTEAMS ];
    UINT32                          uiDecayBloodLastUpdate;
    UINT32                          uiTimeSinceLastInTactical;
    BOOLEAN                         fHasAGameBeenStarted;
    INT8                                bConsNumTurnsWeHaventSeenButEnemyDoes;
    BOOLEAN                             fSomeoneHit;
    UINT8                               ubPaddingSmall;
    UINT32                          uiTimeSinceLastOpplistDecay;
    INT8                                bMercArrivingQuoteBeingUsed;
    UINT8                               ubEnemyKilledOnAttackKiller;
    BOOLEAN                         fCountingDownForGuideDescription;
    INT8                                bGuideDescriptionCountDown;
    UINT8                               ubGuideDescriptionToUse;
    INT8                                bGuideDescriptionSectorX;
    INT8                                bGuideDescriptionSectorY;
    INT8                                fEnemyFlags;
    BOOLEAN          fAutoBandagePending;
    BOOLEAN          fHasEnteredCombatModeSinceEntering;
    BOOLEAN          fDontAddNewCrows;
    UINT8               ubMorePadding;
    UINT16              sCreatureTenseQuoteDelay;
    UINT32              uiCreatureTenseQuoteLastUpdate;
    // SANDRO - added these
    UINT8                               ubLastRequesterSurgeryTargetID;
    UINT8                               ubInterruptPending;
	// sevenfm: Ctrl+D disable interrupts
	BOOLEAN								ubDisablePlayerInterrupts;
    // PADDING GONE!!!!!

};


extern UINT8    gbPlayerNum;
extern INT8     gbShowEnemies;

extern UINT16   gusSelectedSoldier;
extern UINT16   gusOldSelectedSoldier;

extern CHAR8    gzAlertStr[][ 30 ];
extern CHAR8    gzActionStr[][ 30 ];
extern CHAR8    gzDirectionStr[][ 30 ];

// TEMP FOR E3
extern UINT8    gubCurrentScene;
extern CHAR8    *GetSceneFilename(  );
extern INT8     ubLevelMoveLink[ 10 ];

// Soldier List used for all soldier overhead interaction
extern SOLDIERTYPE  Menptr[ TOTAL_SOLDIERS ];
extern SOLDIERTYPE  *MercPtrs[ TOTAL_SOLDIERS ];

// MERC SLOTS - A LIST OF ALL ACTIVE MERCS
extern SOLDIERTYPE  *MercSlots[ TOTAL_SOLDIERS ];
extern UINT32       guiNumMercSlots;

extern BOOLEAN      gfMovingAnimation;


extern TacticalStatusType   gTacticalStatus;

#define OUR_TEAM_SIZE_NO_VEHICLE (gTacticalStatus.Team[ OUR_TEAM ].bLastID - gTacticalStatus.Team[ OUR_TEAM ].bFirstID + 1 - gGameExternalOptions.ubGameMaximumNumberOfPlayerVehicles)


BOOLEAN InitTacticalEngine( );
void ShutdownTacticalEngine( );


BOOLEAN InitOverhead( );
BOOLEAN ShutdownOverhead( );
BOOLEAN GetSoldier( SOLDIERTYPE **ppSoldier, UINT16 usSoldierIndex );

UINT32 CountNonVehiclesOnPlayerTeam( );

INT16 NewOKDestination( SOLDIERTYPE * pCurrSoldier, INT32 sGridNo, BOOLEAN fPeopleToo, INT8 bLevel );

//Simple check to see if a (one-tiled) soldier can occupy a given location on the ground or roof.
extern BOOLEAN IsLocationSittable( INT32 iMapIndex, BOOLEAN fOnRoof );
extern BOOLEAN IsLocationSittableExcludingPeople( INT32 iMapIndex, BOOLEAN fOnRoof );
extern BOOLEAN FlatRoofAboveGridNo( INT32 iMapIndex );


BOOLEAN ExecuteOverhead( );
BOOLEAN ResetAllAnimationCache( );

void EndTurn( );
void EndTurn( UINT8 ubNextTeam );

void StartPlayerTeamTurn( BOOLEAN fDoBattleSnd, BOOLEAN fEnteringCombatMode );
void EndTacticalDemo( );

void SelectSoldier( UINT16 usSoldierID, BOOLEAN fAcknowledge, BOOLEAN fForceReselect    );

//Kaiden: Function declaration from UB to reveal all items after combat.
void RevealAllDroppedEnemyItems();

void LocateGridNo( INT32 sGridNo );
void LocateSoldier( UINT16 usID, BOOLEAN fSetLocator);

void BeginTeamTurn( UINT8 ubTeam );
void SlideTo(INT32 sGridNo, UINT16 usSoldierID , UINT16 usReasonID, BOOLEAN fSetLocator) ;
void SlideToLocation( UINT16 usReasonID, INT32 sDestGridNo );

void RebuildAllSoldierShadeTables( );
void HandlePlayerTeamMemberDeath( SOLDIERTYPE *pSoldier );
UINT8 LastActiveTeamMember( UINT8 ubTeam );
BOOLEAN SoldierOnVisibleWorldTile( SOLDIERTYPE *pSoldier );

UINT8 FindNextActiveAndAliveMerc( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs );
UINT8 FindPrevActiveAndAliveMerc( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs );

BOOLEAN CheckForPlayerTeamInMissionExit( );
void HandleNPCTeamMemberDeath( SOLDIERTYPE *pSoldier );

void StopMercAnimation( BOOLEAN fStop );

UINT32 EnterTacticalDemoMode();

BOOLEAN UIOKMoveDestination( SOLDIERTYPE *pSoldier, INT32 usMapPos );

INT32 FindAdjacentGridEx( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 *pubDirection, INT32 *psAdjustedGridNo, BOOLEAN fForceToPerson, BOOLEAN fDoor );
INT32 FindNextToAdjacentGridEx( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 *pubDirection, INT32 *psAdjustedGridNo, BOOLEAN fForceToPerson, BOOLEAN fDoor );


void SelectNextAvailSoldier( SOLDIERTYPE *pSoldier );
BOOLEAN TeamMemberNear(INT8 bTeam, INT32 sGridNo, INT32 iRange);
BOOLEAN IsValidTargetMerc( UINT8 ubSoldierID );

// FUNCTIONS FOR MANIPULATING MERC SLOTS - A LIST OF ALL ACTIVE MERCS
INT32 GetFreeMercSlot( );
INT32 AddMercSlot( SOLDIERTYPE *pSoldier );
BOOLEAN RemoveMercSlot( SOLDIERTYPE *pSoldier   );

INT32   AddAwaySlot( SOLDIERTYPE *pSoldier );
BOOLEAN RemoveAwaySlot( SOLDIERTYPE *pSoldier );
INT32   MoveSoldierFromMercToAwaySlot( SOLDIERTYPE * pSoldier );
INT32   MoveSoldierFromAwayToMercSlot( SOLDIERTYPE * pSoldier );

void EnterCombatMode( UINT8 ubStartingTeam );
void ExitCombatMode( );
void HandleTeamServices( UINT8 ubTeamNum );
void HandlePlayerServices( SOLDIERTYPE *pTeamSoldier );
void SetEnemyPresence( );
void CycleThroughKnownEnemies( BOOLEAN backward = FALSE );
BOOLEAN CheckForEndOfCombatMode( BOOLEAN fIncrementTurnsNotSeen );
SOLDIERTYPE * FreeUpAttacker( );
BOOLEAN PlayerTeamFull( );
void SetActionToDoOnceMercsGetToLocation( UINT8 ubActionCode,   INT8 bNumMercsWaiting, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 );
void ResetAllMercSpeeds( );
BOOLEAN HandleGotoNewGridNo( SOLDIERTYPE *pSoldier, BOOLEAN *pfKeepMoving, BOOLEAN fInitialMove, UINT16 usAnimState );
SOLDIERTYPE * ReduceAttackBusyCount( );

// HEADROCK HAM B2.6: Made this public so it can be used elsewhere.
INT8 CalcSuppressionTolerance( SOLDIERTYPE * pSoldier );
// HEADROCK HAM 3.2: A new function for checking the condition of nearby friendlies and returning a modifier.
INT8 CheckStatusNearbyFriendlies( SOLDIERTYPE *pSoldier );

void CommonEnterCombatModeCode( );
void CheckForPotentialAddToBattleIncrement( SOLDIERTYPE *pSoldier );
void CencelAllActionsForTimeCompression( );
BOOLEAN CheckForEndOfBattle( BOOLEAN fAnEnemyRetreated );
void AddManToTeam( INT8 bTeam );
void RemoveManFromTeam( INT8 bTeam );
void RemoveSoldierFromTacticalSector( SOLDIERTYPE *pSoldier, BOOLEAN fAdjustSelected );
void MakeCivHostile( SOLDIERTYPE *pSoldier, INT8 bNewSide );

#define REASON_NORMAL_ATTACK    1
#define REASON_EXPLOSION        2

BOOLEAN ProcessImplicationsOfPCAttack( SOLDIERTYPE * pSoldier, SOLDIERTYPE ** ppTarget, INT8 bReason );
INT32 FindAdjacentPunchTarget( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTargetSoldier, INT32 * psAdjustedTargetGridNo, UINT8 * pubDirection );
SOLDIERTYPE * CivilianGroupMemberChangesSides( SOLDIERTYPE * pAttacked );
void CivilianGroupChangesSides( UINT8 ubCivilianGroup );
void CycleVisibleEnemies( SOLDIERTYPE *pSrcSoldier );
void CycleVisibleEnemiesBackward( SOLDIERTYPE *pSrcSoldier );
UINT8 CivilianGroupMembersChangeSidesWithinProximity( SOLDIERTYPE * pAttacked );
void PauseAITemporarily( );
void PauseAIUntilManuallyUnpaused( );
void UnPauseAI( );
void DoPOWPathChecks( );
BOOLEAN HostileCiviliansWithGunsPresent( );
BOOLEAN HostileCiviliansPresent( );
BOOLEAN HostileBloodcatsPresent( );

#ifdef ENABLE_ZOMBIES
BOOLEAN HostileZombiesPresent( );
#endif

UINT8 NumPCsInSector( );
void SetSoldierNonNeutral( SOLDIERTYPE * pSoldier );
void SetSoldierNeutral( SOLDIERTYPE * pSoldier );

#ifdef JA2UB
void HandleThePlayerBeNotifiedOfSomeoneElseInSector();
void SetMsgBoxForPlayerBeNotifiedOfSomeoneElseInSector();

void HandleDisplayingOfPlayerLostDialogue( );

// UTILITY FUNCTIONS
INT8    NumActiveAndConsciousTeamMembers( UINT8 ubTeam );
UINT8 NumEnemyInSector( );
UINT8 NumCapableEnemyInSector( );
INT8    NumMercsOnPlayerTeam( );
#endif

extern BOOLEAN sniperwarning;
extern BOOLEAN biggunwarning;
extern BOOLEAN gogglewarning;
//extern BOOLEAN airstrikeavailable;

//void CheckForAndSetupAirRaid ();
//void EnemyCallInAirStrike (INT16 sSectorX, INT16 sSectorY);

// will a sam site under the players control shoot down an airraid?
BOOLEAN WillAirRaidBeStopped( INT16 sSectorX, INT16 sSectorY );
// HEADROCK HAM 3.5: Externalized for First Arrival enemy check
extern UINT8 NumEnemyInSector();

#ifdef ENABLE_ZOMBIES
// Flugente
extern UINT8 NumZombiesInSector();
#endif

// Flugente: offer the enemy the chance to surrender
void HandleSurrenderOffer( SOLDIERTYPE* pSoldier );

// Flugente: order a team to drop gear, or pick it up
// setting fForce to TRUE allows dropping of gear in combat (used in auto-resolve instances)
void TeamDropAll(UINT8 bTeam, BOOLEAN fForce = FALSE);
void TeamRestock(UINT8 bTeam);

// are we allowed to steal access this guy's inventory?
BOOLEAN AllowedToStealFromTeamMate( UINT8 aAccessorID, UINT8 aTargetID );

// Flugente: is an soldier profile already used?
BOOLEAN IsProfileInUse(UINT8 usTeam, INT8 aType, UINT16 aNr);

// Flugente: functions altering a sector's prisoners
UINT16 GetNumberOfPrisoners( SECTORINFO *pSectorInfo,				UINT8* apSpecial, UINT8* apElite, UINT8* apRegular, UINT8* apAdmin );
UINT16 GetNumberOfPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo,	UINT8* apSpecial, UINT8* apElite, UINT8* apRegular, UINT8* apAdmin );

void ChangeNumberOfPrisoners( SECTORINFO *pSectorInfo,				INT16 aSpecial, INT16 aElite, INT16 aRegular, INT16 aAdmin, INT16 sX = 0, INT16 sY = 0 );
void ChangeNumberOfPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo,  INT16 aSpecial, INT16 aElite, INT16 aRegular, INT16 aAdmin );

void DeleteAllPrisoners( SECTORINFO *pSectorInfo );
void DeleteAllPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo );

// used when the player kills a prisoner. We kill of high-value prisoners first, to punish this kind of behaviour
void KillOnePrisoner( SECTORINFO *pSectorInfo );

// sevenfm: calc shock level with modifiers (traits, backgrounds, ...)
INT8 CalcEffectiveShockLevel( SOLDIERTYPE * pSoldier );

#endif

