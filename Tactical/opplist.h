#ifndef _OPPLIST_H
#define _OPPLIST_H


#define EVERYBODY		MAXMERCS

#define MAX_MISC_NOISE_DURATION 12		// max dur for VERY loud NOBODY noises

#define DOOR_NOISE_VOLUME		2
#define WINDOW_CRACK_VOLUME			4
#define WINDOW_SMASH_VOLUME			8
#define MACHETE_VOLUME			9
#define TRIMMER_VOLUME			18
#define CHAINSAW_VOLUME		 30
#define SMASHING_DOOR_VOLUME		6
#define CROWBAR_DOOR_VOLUME			4
#define ITEM_THROWN_VOLUME			2

#define TIME_BETWEEN_RT_OPPLIST_DECAYS 20

// this is a fake "level" value (0 on ground, 1 on roof) for
// HearNoise to ignore the effects of lighting(?)
#define LIGHT_IRRELEVANT 127

#define AUTOMATIC_INTERRUPT 100
#define NO_INTERRUPT 127

#define MOVEINTERRUPT	0
#define SIGHTINTERRUPT	1
#define NOISEINTERRUPT	2

// SANDRO - enum for improved interrupt system
enum
{
	DISABLED_INTERRUPT = 0,
	UNTRIGGERED_INTERRUPT,
	UNDEFINED_INTERRUPT,
	MOVEMENT_INTERRUPT,
	SP_MOVEMENT_INTERRUPT,
	BEFORESHOT_INTERRUPT,
	AFTERSHOT_INTERRUPT,
	AFTERACTION_INTERRUPT,
	INSTANT_INTERRUPT,
	MAX_INTERRUPT_TYPES
};

// noise type constants
enum
{
	NOISE_UNKNOWN = 0,
	NOISE_MOVEMENT,
	NOISE_CREAKING,
	NOISE_SPLASHING,
	NOISE_BULLET_IMPACT,
	NOISE_GUNFIRE,
	NOISE_EXPLOSION,
	NOISE_SCREAM,
	NOISE_ROCK_IMPACT,
	NOISE_GRENADE_IMPACT,
	NOISE_WINDOW_SMASHING,
	NOISE_DOOR_SMASHING,
	NOISE_SILENT_ALARM, // only heard by enemies
	NOISE_VOICE, // anv: for enemy taunts
	MAX_NOISES
};

enum
{
	EXPECTED_NOSEND,	// other nodes expecting noise & have all info
	EXPECTED_SEND,		// other nodes expecting noise, but need info
	UNEXPECTED				// other nodes are NOT expecting this noise
};

#define NUM_WATCHED_LOCS 3

extern INT8 gbPublicOpplist[MAXTEAMS][ TOTAL_SOLDIERS ];
extern INT8 gbSeenOpponents[TOTAL_SOLDIERS][TOTAL_SOLDIERS];
extern INT32 gsLastKnownOppLoc[TOTAL_SOLDIERS][TOTAL_SOLDIERS];		// merc vs. merc
extern INT8 gbLastKnownOppLevel[TOTAL_SOLDIERS][TOTAL_SOLDIERS];
extern INT32 gsPublicLastKnownOppLoc[MAXTEAMS][TOTAL_SOLDIERS];	// team vs. merc
extern INT8 gbPublicLastKnownOppLevel[MAXTEAMS][TOTAL_SOLDIERS];
extern UINT8 gubPublicNoiseVolume[MAXTEAMS];
extern INT32 gsPublicNoiseGridNo[MAXTEAMS];
extern INT8 gbPublicNoiseLevel[MAXTEAMS];
extern UINT8 gubKnowledgeValue[10][10];
extern INT8 gbLookDistance[8][8];
extern INT8 gfKnowAboutOpponents;

extern BOOLEAN	gfPlayerTeamSawJoey;
extern BOOLEAN	gfMikeShouldSayHi;

#ifdef JA2UB
extern BOOLEAN   gfMorrisShouldSayHi; // JA25 UB
#endif

extern INT32			gsWatchedLoc[ TOTAL_SOLDIERS ][ NUM_WATCHED_LOCS ];
extern INT8				gbWatchedLocLevel[ TOTAL_SOLDIERS ][ NUM_WATCHED_LOCS ];
extern UINT8			gubWatchedLocPoints[ TOTAL_SOLDIERS ][ NUM_WATCHED_LOCS ];
extern BOOLEAN		gfWatchedLocReset[ TOTAL_SOLDIERS ][ NUM_WATCHED_LOCS ];
extern INT8 STRAIGHT;

#define BEST_SIGHTING_ARRAY_SIZE 6
#define BEST_SIGHTING_ARRAY_SIZE_ALL_TEAMS_LOOK_FOR_ALL 6
#define BEST_SIGHTING_ARRAY_SIZE_NONCOMBAT 3
#define BEST_SIGHTING_ARRAY_SIZE_INCOMBAT 0
extern UINT8 gubBestToMakeSightingSize;

INT16 AdjustMaxSightRangeForEnvEffects( SOLDIERTYPE *pSoldier, INT8 bLightLevel, INT16 sDistVisible );
INT16 ManLooksForMan(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, UINT8 ubCaller);
void HandleSight(SOLDIERTYPE *pSoldier, UINT8 ubSightFlags);
void AllTeamsLookForAll(UINT8 ubAllowInterrupts);
void GloballyDecideWhoSeesWho(void);
UINT16 GetClosestMerc( UINT16 usSoldierIndex );
void ManLooksForOtherTeams(SOLDIERTYPE *pSoldier);
void OtherTeamsLookForMan(SOLDIERTYPE *pOpponent);
void ManSeesMan(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, INT32 sOppGridNo, INT8 bOppLevel, UINT8 ubCaller, UINT8 ubCaller2);
void DecideTrueVisibility(SOLDIERTYPE *pSoldier, UINT8 ubLocate);
void AddOneOpponent(SOLDIERTYPE *pSoldier);
void RemoveOneOpponent(SOLDIERTYPE *pSoldier);
void UpdatePersonal(SOLDIERTYPE *pSoldier, UINT8 ubID, INT8 bNewOpplist, INT32 sGridNo, INT8 bLevel);
INT16 MaxNormalDistanceVisible( void );
INT16 DistanceVisible( SOLDIERTYPE *pSoldier, INT8 bFacingDir, INT8 bSubjectDir, INT32 sSubjectGridNo, INT8 bLevel, const BOOLEAN& isCowering, const UINT8& tunnelVision);
void ResetLastKnownLocs(SOLDIERTYPE *ptr);
void RecalculateOppCntsDueToNoLongerNeutral( SOLDIERTYPE * pSoldier );
void ReevaluateBestSightingPosition( SOLDIERTYPE * pSoldier, INT8 bInterruptDuelPts );


void InitOpponentKnowledgeSystem(void);
void InitSoldierOppList(SOLDIERTYPE *pSoldier);
void BetweenTurnsVisibilityAdjustments(void);
void RemoveManAsTarget(SOLDIERTYPE *pSoldier);
void UpdatePublic(UINT8 ubTeam, UINT8 ubID, INT8 bNewOpplist, INT32 sGridNo, INT8 bLevel );
void RadioSightings(SOLDIERTYPE *pSoldier, UINT8 ubAbout, UINT8 ubTeamToRadioTo );
void OurTeamRadiosRandomlyAbout(UINT8 ubAbout);
void DebugSoldierPage1( );
void DebugSoldierPage2( );
void DebugSoldierPage3( );
void DebugSoldierPage4( );

UINT8 MovementNoise( SOLDIERTYPE *pSoldier );
UINT8 DoorOpeningNoise( SOLDIERTYPE *pSoldier );
void MakeNoise(UINT8 ubNoiseMaker, INT32 sGridNo, INT8 bLevel, UINT8 ubTerrType, UINT8 ubVolume, UINT8 ubNoiseType, STR16 zNoiseMessage = NULL );
void OurNoise( UINT8 ubNoiseMaker, INT32 sGridNo, INT8 bLevel, UINT8 ubTerrType, UINT8 ubVolume, UINT8 ubNoiseType, STR16 zNoiseMessage = NULL );

void ResolveInterruptsVs( SOLDIERTYPE * pSoldier, UINT8 ubInterruptType);

void VerifyAndDecayOpplist(SOLDIERTYPE *pSoldier);
void DecayIndividualOpplist(SOLDIERTYPE *pSoldier);
void VerifyPublicOpplistDueToDeath( SOLDIERTYPE * pSoldier );
void NoticeUnseenAttacker( SOLDIERTYPE * pAttacker, SOLDIERTYPE * pDefender, INT8 bReason );

BOOLEAN MercSeesCreature( SOLDIERTYPE * pSoldier );

INT8 GetWatchedLocPoints( UINT8 ubID, INT32 sGridNo, INT8 bLevel );
INT8 GetHighestVisibleWatchedLoc( UINT8 ubID );
INT8 GetHighestWatchedLocPoints( UINT8 ubID );

void TurnOffEveryonesMuzzleFlashes( void );
void TurnOffTeamsMuzzleFlashes( UINT8 ubTeam );
void EndMuzzleFlash( SOLDIERTYPE * pSoldier );
void NonCombatDecayPublicOpplist( UINT32 uiTime );

void CheckHostileOrSayQuoteList( void );
void InitOpplistForDoorOpening( void );
UINT8 DoorOpeningNoise( SOLDIERTYPE * pSoldier );

void AddToShouldBecomeHostileOrSayQuoteList( UINT8 ubID );

//extern INT8 gbLightSighting[1][16];

BOOLEAN SoldierHasLimitedVision(SOLDIERTYPE * pSoldier);


#ifdef JA2UB
INT32 MaxDistanceVisible( void );
#endif

// HEADROCK HAM 3.6: Moved here from cpp
void MakeBloodcatsHostile( void );
#endif