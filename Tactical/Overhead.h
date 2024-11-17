#ifndef __OVERHEAD_H
#define __OVERHEAD_H

#include <string.h>
#include <stdio.h>
#include "Soldier Control.h"
#include <EditorMercs.h>
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
    UINT16      bFirstID;
    UINT16      bLastID;
    COLORVAL    RadarColor;
    INT8        bSide;
    INT16       bMenInSector;
    SoldierID   ubLastMercToRadio;
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
    UINT32                  uiFlags;
    TacticalTeamType        Team[ MAXTEAMS ];
    UINT8                   ubCurrentTeam;
    INT32                   sSlideTarget;
    INT16                   sSlideReason_UNUSED;
    UINT32                  uiTimeSinceMercAIStart;
    INT8                    fPanicFlags;
    INT32                   sPanicTriggerGridNoUnused;
    INT16                   sHandGrid;
    UINT16                  ubSpottersCalledForBy;
    UINT16                  ubTheChosenOne;
    UINT32                  uiTimeOfLastInput;
    UINT32                  uiTimeSinceDemoOn;
    UINT32                  uiCountdownToRestart;
    BOOLEAN                 fGoingToEnterDemo;
    BOOLEAN                 fNOTDOLASTDEMO;
    BOOLEAN                 fMultiplayer;
    BOOLEAN                 fCivGroupHostile[ NUM_CIV_GROUPS ];
    UINT8                   ubLastBattleSectorX;                    
    UINT8                   ubLastBattleSectorY;
    BOOLEAN                 fLastBattleWon;
    INT8                    bOriginalSizeOfEnemyForce;
    INT8                    bPanicTriggerIsAlarmUnused;
    BOOLEAN                 fVirginSector;
    BOOLEAN                 fEnemyInSector;
    BOOLEAN                 fInterruptOccurred;
    INT8                    bRealtimeSpeed;
    UINT8                   ubEnemyIntention;
    UINT8                   ubEnemyIntendedRetreatDirection;
    SoldierID               ubEnemySightingOnTheirTurnEnemyID;
    SoldierID               ubEnemySightingOnTheirTurnPlayerID;
    BOOLEAN                 fEnemySightingOnTheirTurn;
    BOOLEAN                 fAutoBandageMode;
    UINT8                   ubAttackBusyCount;
    INT8                    bNumEnemiesFoughtInBattleUnused;
    UINT16                  ubEngagedInConvFromActionMercID;
    UINT16                  usTactialTurnLimitCounter;
    BOOLEAN                 fInTopMessage;
    UINT8                   ubTopMessageType;
    CHAR16                  zTopMessageString[20];
    UINT16                  usTactialTurnLimitMax;
    UINT32                  uiTactialTurnLimitClock;
    BOOLEAN                 fTactialTurnLimitStartedBeep;
    INT8                    bBoxingState;
    INT8                    bConsNumTurnsNotSeen;
    UINT8                   ubArmyGuysKilled;

    INT32                   sPanicTriggerGridNo[ NUM_PANIC_TRIGGERS ];
    INT8                    bPanicTriggerIsAlarm[ NUM_PANIC_TRIGGERS ];
    UINT8                   ubPanicTolerance[ NUM_PANIC_TRIGGERS ];
    BOOLEAN                 fAtLeastOneGuyOnMultiSelect;
    BOOLEAN                 fSaidCreatureFlavourQuote;
    BOOLEAN                 fHaveSeenCreature;
    BOOLEAN                 fKilledEnemyOnAttack;
    UINT16                  ubEnemyKilledOnAttack;
    INT8                    bEnemyKilledOnAttackLevel;
    UINT16                  ubEnemyKilledOnAttackLocation;
    BOOLEAN                 fItemsSeenOnAttack;
    BOOLEAN                 ubItemsSeenOnAttackSoldier;
    BOOLEAN                 fBeenInCombatOnce;
    BOOLEAN                 fSaidCreatureSmellQuote;
    UINT32                  usItemsSeenOnAttackGridNo;
    BOOLEAN                 fLockItemLocators;
    UINT8                   ubLastQuoteSaid;
    UINT8                   ubLastQuoteProfileNUm;
    BOOLEAN                 fCantGetThrough;
    INT32                   sCantGetThroughGridNo;
    INT32                   sCantGetThroughSoldierGridNo;
    UINT16                  ubCantGetThroughID;
    BOOLEAN                 fDidGameJustStart;
    BOOLEAN                 fStatChangeCheatOn;
    UINT8                   ubLastRequesterTargetID;
    BOOLEAN                 fGoodToAllowCrows;
    UINT8                   ubNumCrowsPossible;
    UINT32                  uiTimeCounterForGiveItemSrc;
    BOOLEAN                 fUnLockUIAfterHiddenInterrupt;
    INT8                    bNumFoughtInBattle[ MAXTEAMS ];
    UINT32                  uiDecayBloodLastUpdate;
    UINT32                  uiTimeSinceLastInTactical;
    BOOLEAN                 fHasAGameBeenStarted;
    INT8                    bConsNumTurnsWeHaventSeenButEnemyDoes;
    BOOLEAN                 fSomeoneHit;
    UINT32                  uiTimeSinceLastOpplistDecay;
    INT8                    bMercArrivingQuoteBeingUsed;
    UINT16                  ubEnemyKilledOnAttackKiller;
    BOOLEAN                 fCountingDownForGuideDescription;
    INT8                    bGuideDescriptionCountDown;
    UINT8                   ubGuideDescriptionToUse;
    INT8                    bGuideDescriptionSectorX;
    INT8                    bGuideDescriptionSectorY;
    INT8                    fEnemyFlags;
    BOOLEAN                 fAutoBandagePending;
    BOOLEAN                 fHasEnteredCombatModeSinceEntering;
    BOOLEAN                 fDontAddNewCrows;
    UINT8                   ubMorePadding;
    UINT16                  sCreatureTenseQuoteDelay;
    UINT32                  uiCreatureTenseQuoteLastUpdate;
    // SANDRO - added these
    SoldierID               ubLastRequesterSurgeryTargetID;
    UINT8                   ubInterruptPending;
	// sevenfm: Ctrl+D disable interrupts
	BOOLEAN					ubDisablePlayerInterrupts;
    // PADDING GONE!!!!!

};


extern UINT8    gbPlayerNum;
extern INT8     gbShowEnemies;

extern SoldierID    gusSelectedSoldier;

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
BOOLEAN GetSoldier( SOLDIERTYPE **ppSoldier, SoldierID usSoldierIndex );

UINT32 CountNonVehiclesOnPlayerTeam( );

BOOLEAN NewOKDestination( SOLDIERTYPE * pCurrSoldier, INT32 sGridNo, BOOLEAN fPeopleToo, INT8 bLevel );

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
void LocateSoldier( SoldierID usID, BOOLEAN fSetLocator);

void BeginTeamTurn( UINT8 ubTeam );
void SlideTo(SoldierID usSoldierID, BOOLEAN fSetLocator) ;
void SlideToLocation( INT32 sDestGridNo );

void RebuildAllSoldierShadeTables( );
void HandlePlayerTeamMemberDeath( SOLDIERTYPE *pSoldier );
UINT16 LastActiveTeamMember( UINT16 ubTeam );
BOOLEAN SoldierOnVisibleWorldTile( SOLDIERTYPE *pSoldier );

UINT16 FindNextActiveAndAliveMerc( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs );
UINT16 FindPrevActiveAndAliveMerc( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs );

BOOLEAN CheckForPlayerTeamInMissionExit( );
void HandleNPCTeamMemberDeath( SOLDIERTYPE *pSoldier );

void StopMercAnimation( BOOLEAN fStop );

UINT32 EnterTacticalDemoMode();

BOOLEAN UIOKMoveDestination( SOLDIERTYPE *pSoldier, INT32 usMapPos );

INT32 FindAdjacentGridEx( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 *pubDirection, INT32 *psAdjustedGridNo, BOOLEAN fForceToPerson, BOOLEAN fDoor , bool allow_diagonal = false);
INT32 FindNextToAdjacentGridEx( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 *pubDirection, INT32 *psAdjustedGridNo, BOOLEAN fForceToPerson, BOOLEAN fDoor );


void SelectNextAvailSoldier( SOLDIERTYPE *pSoldier );
BOOLEAN TeamMemberNear(INT8 bTeam, INT32 sGridNo, INT32 iRange);
BOOLEAN IsValidTargetMerc( SoldierID ubSoldierID );

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
void SetActionToDoOnceMercsGetToLocation( UINT8 ubActionCode, UINT8 uiNumMercsWaiting, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 );
void ResetAllMercSpeeds( );
BOOLEAN HandleGotoNewGridNo( SOLDIERTYPE *pSoldier, BOOLEAN *pfKeepMoving, BOOLEAN fInitialMove, UINT16 usAnimState );
SOLDIERTYPE * ReduceAttackBusyCount( );

// HEADROCK HAM B2.6: Made this public so it can be used elsewhere.
INT8 CalcSuppressionTolerance( SOLDIERTYPE * pSoldier );
// HEADROCK HAM 3.2: A new function for checking the condition of nearby friendlies and returning a modifier.
INT8 CheckStatusNearbyFriendlies( SOLDIERTYPE *pSoldier );
// sevenfm: simplified version
INT8 CheckStatusNearbyFriendliesSimple(SOLDIERTYPE *pSoldier);

void CommonEnterCombatModeCode( );
void CheckForPotentialAddToBattleIncrement( SOLDIERTYPE *pSoldier );
void CencelAllActionsForTimeCompression( );
BOOLEAN CheckForEndOfBattle( BOOLEAN fAnEnemyRetreated );
void AddManToTeam( INT8 bTeam );
void RemoveManFromTeam( INT8 bTeam );
void RemoveSoldierFromTacticalSector( SOLDIERTYPE *pSoldier, BOOLEAN fAdjustSelected );
void MakeCivHostile(SOLDIERTYPE *pSoldier);

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
BOOLEAN HostileZombiesPresent( );
BOOLEAN HostileCreaturesPresent();

UINT16 NumPCsInSector( );
void SetSoldierNonNeutral( SOLDIERTYPE * pSoldier );
void SetSoldierNeutral( SOLDIERTYPE * pSoldier );

#ifdef JA2UB
void HandleThePlayerBeNotifiedOfSomeoneElseInSector();
void SetMsgBoxForPlayerBeNotifiedOfSomeoneElseInSector();

void HandleDisplayingOfPlayerLostDialogue( );

// UTILITY FUNCTIONS
UINT16    NumActiveAndConsciousTeamMembers( UINT8 ubTeam );
UINT16 NumEnemyInSector( );
UINT16 NumCapableEnemyInSector( );
UINT16    NumMercsOnPlayerTeam( );
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
extern UINT16 NumEnemyInSector();

// Flugente
extern UINT16 NumZombiesInSector();

// Flugente: offer the enemy the chance to surrender
void HandleSurrenderOffer( SOLDIERTYPE* pSoldier );

// Flugente: attempt to convince a soldier to become a turncoat
void HandleTurncoatAttempt( SOLDIERTYPE* pSoldier );

// Flugente: disguised mercs can distract enemies by talking to them. In order to display that, we sometimes display excerpts of their 'chats'
void CheckChatPartners();

// Flugente: order a team to drop gear, or pick it up
// setting fForce to TRUE allows dropping of gear in combat (used in auto-resolve instances)
void TeamDropAll(UINT8 bTeam, BOOLEAN fForce = FALSE);
void TeamRestock(UINT8 bTeam);

// are we allowed to steal access this guy's inventory?
BOOLEAN AllowedToStealFromTeamMate( UINT16 ubID, UINT16 ubTargetID );

// Flugente: is an soldier profile already used?
BOOLEAN IsProfileInUse(UINT8 usTeam, INT8 aType, UINT16 aNr);

// Flugente: functions altering a sector's prisoners
UINT16 GetNumberOfPrisoners( SECTORINFO *pSectorInfo, INT16 aPrisoners[] );
UINT16 GetNumberOfPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo, INT16 aPrisoners[] );

void ChangeNumberOfPrisoners( SECTORINFO *pSectorInfo, INT16 aPrisoners[] );
void ChangeNumberOfPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo, INT16 aPrisoners[] );

void DeleteAllPrisoners( SECTORINFO *pSectorInfo );
void DeleteAllPrisoners( UNDERGROUND_SECTORINFO *pSectorInfo );

// used when the player kills a prisoner. We kill of high-value prisoners first, to punish this kind of behaviour
void KillOnePrisoner( SECTORINFO *pSectorInfo );

// sevenfm: calc shock level with modifiers (traits, backgrounds, ...)
INT8 CalcEffectiveShockLevel( SOLDIERTYPE * pSoldier );
BOOLEAN CoweringShockLevel( SOLDIERTYPE * pSoldier );
// Flugente: enemy officers
enum {
	OFFICER_NONE = 0,
	OFFICER_LIEUTNANT,
	OFFICER_CAPTAIN,

	OFFICER_MAX,
};

// Flugente: return number of enemy officers and highest rank found
UINT16 HighestEnemyOfficersInSector(UINT8& aType);

// count all soldiers in the current sector that have a specific flag set
UINT16 NumSoldiersWithFlagInSector(UINT8 aTeam, UINT32 aFlag);
UINT16 NumSoldiersofClassWithFlag2InSector( UINT8 aTeam, UINT8 aSoldierClass, UINT32 aFlag );
INT32 GetClosestSoldierWithFlag( UINT8 aTeam, UINT32 aFlag );

// Flugente: VIP targets
// can a new VIP be created?
BOOLEAN VIPSlotFree();

BOOLEAN SectorHasVIP( INT16 sMapX, INT16 sMapY );
BOOLEAN PlayerKnowsAboutVIP( INT16 sMapX, INT16 sMapY );

BOOLEAN TownHasVIP( INT8 bTownId );

BOOLEAN GetRandomEnemyTownSector( INT8 bTownId, UINT16& aSector );

// Get a possible sector for a VIP to spawn in. Note: we do not spawn new VIPs in Meduna - VIPs that escape move there instead
BOOLEAN GetPossibleVIPSector( UINT16& aSector );

// Get the location of a VIP we do not yet know about
BOOLEAN GetRandomUnknownVIPSector( UINT16& aSector );

void DeleteVIP( INT16 sMapX, INT16 sMapY );
void VIPFleesToMeduna();

// Flugente: is any member of a specific civilian faction still alive in the current sector?
BOOLEAN IsCivFactionMemberAliveInSector( UINT8 usCivilianGroup );

BOOLEAN IsFreeSlotAvailable( int aTeam );
void AttemptToCapturePlayerSoldiers();

SOLDIERTYPE* SoldierFromID(const SoldierID id);
SOLDIERTYPE* SoldierFromID(const SoldierID id, SOLDIERTYPE* Array[TOTAL_SOLDIERS]);
#endif

