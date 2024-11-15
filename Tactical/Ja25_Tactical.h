#ifndef _JA25_TACTICAL__H_
#define _JA25_TACTICAL__H_

#ifdef JA2UB

#include "MapScreen Quotes.h"

#define		PGF__FAN_EXIT_GRID_GRIDNO																	10979

#define		NUM_MERCS_WITH_NEW_QUOTES												20//7


#define		DQ__NORMAL_DELAYED_QUOTE														0xff00
#define		DQ__JERRY_BROKE_TRANSMITTER													0xffff
#define		DQ__MORRIS_NOTE_NEW_MERC_DELAY											0xfffe
#define		DQ__MORRIS_NOTE_DISPLAY_NOTE_1											0xfffd
#define		DQ__MORRIS_NOTE_DISPLAY_NOTE_2											0xfffc
#define		DQ__SHOW_RADIO_LOCATOR															0xfffb
#define		DQ__NEW_MERC_SAY_NOTE_QUOTES												0xfffa
#define		DQ__START_EVERYONE_TALKING_AT_END_OF_GAME						0xfff0
#define		DQ__FADE_OUT_TO_END_GAME_CINEMATIC									0xfff9

enum
{
	JM_SE__HEADING_TO_TRACONA =MILO_QUOTE__HEADED_TO_TRACONA_QUOTE,
	JM_SE__SHOW_RADIO_LOCATOR = MILO_NO_QUOTE-10,
};

/*
// things that need to be saved in the exp in tactical
typedef struct
{

} JA25_TACTICAL_SAVE_STRUCT;

*/
extern	BOOLEAN		gfFirstTimeInGameHeliCrash;

INT16		RandomProfileIdFromNewMercsOnPlayerTeam();
INT16		RandomSoldierIdFromNewMercsOnPlayerTeam();
UINT8		GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( UINT8 *pSoldierIdArray, UINT8 *pProfileIdArray );
BOOLEAN	IsSoldierQualifiedMerc( SOLDIERTYPE *pSoldier );
BOOLEAN	IsSoldierQualifiedInitialHireMerc( SOLDIERTYPE *pSoldier );
BOOLEAN	IsSoldierQualifiedMercForSeeingPowerGenFan( SOLDIERTYPE *pSoldier );

BOOLEAN SayQuoteFromAllNewHiredMercButDoGastonLast( UINT8 ubProfile, UINT32 uiQuoteNum );

//Pass in an array of size NUM_MERCS_WITH_NEW_QUOTES, array will be filled with
//random soldier IDs of the qualified mercs
UINT8 RandomArrayOfQualifiedMercs( UINT8 *pRandomSoldierIdArray );


extern UINT8 Get3RandomQualifiedMercs( SoldierID *pSoldierId1, SoldierID *pSoldierId2, SoldierID *pSoldierId3 );

void HandleWhenCertainPercentageOfEnemiesDie();

void StartFanBackUpAgain();

void AddExitGridForFanToPowerGenSector();

void HandleAddingPowerGenFanSound();
void HandleRemovingPowerGenFanSound();

void HandleStartingFanBackUp();

void HandlePowerGenAlarm();


BOOLEAN HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( SOLDIERTYPE *pSoldier, BOOLEAN fSayQuoteOnlyOnce );

extern void HandleHowPlayerGotThroughFan();

//void RevealAllDroppedEnemyItems();

void HandlePlayingQuoteWhenHiringNpc( UINT8 ubProfile );


BOOLEAN LoadJa25TacticalInfoFromSavedGame( HWFILE hFile );
BOOLEAN SaveJa25TacticalInfoToSaveGame( HWFILE hFile );

BOOLEAN HandleNewGunComment( SOLDIERTYPE *pSoldier, INT32 iItemIndex, BOOLEAN fFromGround );
void HandleDeathInPowerGenSector( SOLDIERTYPE *pSoldier );

void		HandleJa25EnemyExpLevelModifier( );

INT8		RandomSoldierIdForAnyMercInSector();

void		HandleFanStartingAtEndOfCombat();

extern void		HandleInitialEventsInHeliCrash();

void		HandleCommanderMorrisNewMercWantsNoteDelayedSpeech();

void		DisplayCommanderMorrisNote( SOLDIERTYPE *pSoldier );

void		HandleShowingRadioLocatorsInMorrisArea();

void		HandleNewMercSayingContentsOfMorrisNote( SOLDIERTYPE *pSoldier );

void		HandlePlayerHittingSwitchToLaunchMissles();

void		HandleStartingEveryoneTalkingAtEndOfGame();

void		HandleFadingOutToEndGameCinematics();

void		HandleOpenControlPanelToRevealSwitchInMorrisArea();

void		InitNewGunArray();

extern BOOLEAN useOldJA2Inventory;
extern UINT32	TEX_MOVIE_ATTACK_CLYDESDALES;
extern UINT32	TEX_MOVIE_WILD_EAST;
extern UINT32	TEX_MOVIE_HAVE_HONDA;
extern UINT32	LAPTOP_TRANSMITTER;
extern UINT32	CHE_GUEVARA_CANTEEN;
extern UINT32	MERC_WRISTWATCH;
extern UINT32	SAM_GARVER_COMBAT_KNIFE;
extern UINT32	MERC_UMBRELLA_OLD;
extern UINT32	MORRIS_INSTRUCTION_NOTE;
extern UINT32	HAND_CANNON;
extern UINT32	HARTFORD_6_SHOOTER;
extern UINT32	MERC_UMBRELLA;
extern UINT32	CLIP_CANNON_BALL;
extern UINT32	BARRETT_UB;
extern UINT32	CALICO_960_UB;
extern UINT32	PSG1_UB;
extern UINT32	L85_UB;
extern UINT32	TAR21_UB;
extern UINT32	VAL_SILENT_UB;
extern UINT32	MICRO_UZI_UB;
extern UINT32	CALICO_950_UB;
extern UINT32	CALICO_900_UB;

extern UINT8 MANUEL_UB;
extern UINT8 BIGGENS_UB;
extern UINT8 JOHN_K_UB;
extern UINT8 TEX_UB;
extern UINT8 GASTON_UB;
extern UINT8 STOGIE_UB;
extern UINT8 JERRY_MILO_UB;
extern UINT8 PGMALE4_UB;
extern UINT8 BETTY_UB;
extern UINT8 RAUL_UB;
extern UINT8 MORRIS_UB;
extern UINT8 RUDY_UB;

extern void Old_UB_Inventory ();
extern void New_UB_Inventory ();

extern void InitGridNoUB();

#endif

#endif
