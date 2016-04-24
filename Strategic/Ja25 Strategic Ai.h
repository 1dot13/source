#ifndef JA25__STRATEGIC_AI
#define JA25__STRATEGIC_AI

#ifdef JA2UB
typedef struct
{
	UINT32		uiJerryMiloQuotesFlags;										//used to keep track of the quotes Jerry says.
	BOOLEAN		fImportCharactersFromOldJa2Save;					// True if we have loaded in merc profiles from JA2

	BOOLEAN		fInCustomMap;

	BOOLEAN		fJohnKulbaIsInGame;												// If true Kulba is in the game, otherwise tex is

	BOOLEAN		fMorrisToSayHurtPlayerQuoteNextTurn;
	UINT8			ubPlayerMorrisHurt;

	BOOLEAN		fBiggensUsedDetonator;

	BOOLEAN		fSendEmail_10_NextSector;									// True if we are to send email #10 next time the player loads up a sector

	UINT8			ubEmailFromSectorFlag;

	BOOLEAN		fNpcHasBeenAdded;													// True if manuel has been added to sector H10 or i9

	BOOLEAN		fShowMercDestinationDialogWhenHiringMerc;	// 

	BOOLEAN		fHaveAimandMercOffferItems;								// TRUE when MERC and AIM are giving away 'gifts' to the mercs that get hired

	BOOLEAN		fBiffWasKilledWhenImportingSave;					//TRUE is biff was killed when we imported the save

	UINT8			ubImportantSpeckQuotesSaidBefore;					

	BOOLEAN		fShouldMsgBoxComeUpSayingSomeoneImportantIsInSector;		

	UINT8			ubStateOfFanInPowerGenSector;

	UINT8			ubHowPlayerGotThroughFan;

	UINT32		uiJa25TacticalFlags;

	INT32			iPowerGenFanPositionSndID;

	UINT32		uiTacticalTurnCounter;

	UINT32		uiTurnPowerGenFanWentDown;

	UINT32		uiJa25GeneralFlags;

	UINT8			ubStatusOfFortifiedDoor;

	UINT32		uiDisplayCoverCounter;
	UINT32		uiDisplayLosCounter;
	UINT32		uiDisplayGunRangeCounter;

	UINT8			ubLoadScreenStairTraversal;

	UINT8			ubMorrisNoteState;

	UINT8			ubJohnKulbaInitialSectorX;
	UINT8			ubJohnKulbaInitialSectorY;

	UINT8			ubDisplayPlayerLostMsgBox;

	BOOLEAN		fJerryBreakingLaptopOccuring;

	BOOLEAN		fEnemyShouldImmediatelySeekThePlayer;
	INT8			bSectorTheEnemyWillSeekEnemy;

	INT8			bNewMercProfileIDForSayingMorrisNote;

	UINT8			ubDisplayCommanderMorrisNote;

	UINT16		usFiller1;

	UINT32		uiTurnLastSaidSeeEnemyQuote;

	UINT8			ubFiller[928];	//Struct is originally 1000 bytes in size

} JA25_SAVE_INFO;

extern	JA25_SAVE_INFO	gJa25SaveStruct;

//following flags use uiEmailFromSectorFlag
#define		SECTOR_EMAIL__ANOTHER_SECTOR					0x01
#define		SECTOR_EMAIL__J11_J12									0x02
#define		SECTOR_EMAIL__POWER_GEN								0x04
#define		SECTOR_EMAIL__TUNNEL									0x08
#define		SECTOR_EMAIL__POWER_GEN_LVL_1					0x10


//following flags use fNpcHasBeenAdded
#define		SECTOR_ADDED_NPC__MANUEL							0x01
#define		SECTOR_ADDED_NPC__TEX									0x02
#define		SECTOR_ADDED_NPC__JOHN_K							0x04


//Following flags use ubImportantSpeckQuotesSaidBefore
#define		SPECK__ENCOURAGE_POAYER_TO_KEEP_SHOPPING			1<<0
#define		SPECK__SECOND_INTRO_BEEN_SAID									1<<1
#define		SPECK__BETTER_EQUIPMENT												1<<2


//following flags are used with ubStateOfFanInPowerGenSector
enum
{
	PGF__RUNNING_NORMALLY,
	PGF__STOPPED,
	PGF__BLOWN_UP,
};


//following flags are used with uiJa25TacticalFlags



//following flags are used with ubHowPlayerGotThroughFan
enum
{
	PG__PLAYER_NOT_GONE_THROUGH_YET,
	PG__PLAYER_BLEW_UP_FAN_TO_GET_THROUGH,
	PG__PLAYER_STOPPED_FAN_TO_GET_THROUGH,
};

//The following flags are used with uiJa25GeneralFlags.  
#define		JA_GF__POWER_GEN_FAN_HAS_BEEN_STOPPED																			0x00000001
#define		JA_GF__BIGGENS_SAID_QUOTE_117																							0x00000002
#define		JA_GF__DID_PLAYER_MAKE_SOUND_GOING_THROUGH_TUNNEL_GATE										0x00000004
#define		JA_GF__PLAYER_HAS_SEEN_FORTIFIED_DOOR																			0x00000008

#define		JA_GF__ALL_DEAD_TOP_LEVEL_OF_COMPLEX																			0x00000010
#define		JA_GF__ALREADY_MOVED_ENEMIES_IN_COMPLEX																		0x00000020
#define		JA_GF__PICKED_UP_MONEY_IN_GUARD_POST																			0x00000040
#define		JA_GF__PICKED_UP_MONEY_IN_FIRST_TOWN																			0x00000080

#define		JA_GF__PLAYER_SAID_LAPTOP_FIXED_QUOTE																			0x00000100
#define		JA_GF__RAUL_BLOW_HIMSELF_UP																								0x00000200
#define		JA_GF__QUOTE_105_HAS_BEEN_SAID																						0x00000400
#define		JA_GF__BARRETT_IS_HALF_PRICE																							0x00000800

#define		JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_1																		0x00001000
#define		JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_2																		0x00002000
#define		JA_GF__PLAYER_SEEN_FAN_BEFORE																							0x00004000
#define		JA_GF__PLAYER_SAID_GATE_LOCKED_QUOTE																			0x00008000


//The following is used with ubStatusOfFortifiedDoor
enum
{
	FD__CLOSED,
	FD__OPEN,
};


//the following is used with ubLoadScreenStairTraversal
enum
{
	LS__NOT_GOING_UP_STAIRS,
	LS__GOING_UP_STAIRS,
	LS__GOING_DOWN_STAIRS,
};



//the following is used with ubMorrisNoteState
enum
{
	MN__NOT_PICKED_UP_YET,
	MN__PICKED_UP_BY_OLD_MERC_NEVER_SAID_QUOTE,
	MN__PICKED_UP_BY_OLD_MERC_SAID_QUOTE_ALREADY,
	MN__PICKED_UP_BY_NEW_MERC,
	MN__FINISHED = MN__PICKED_UP_BY_NEW_MERC,
};


//used with ubDisplayCommanderMorrisNote
enum
{
	DMN__NOT_TO_DISPLAY_IT,
	DMN__DISPLAY_PART_1,
	DMN__DISPLAY_PART_2,
	DMN__FINISHED,
};


BOOLEAN	AreAnyPlayerMercsStillInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

BOOLEAN ShouldEnemiesBeAddedToInitialSector();

void InitJa25StrategicAi();

void SetNumberJa25EnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubJeeps );

void InitJa25SaveStruct();

void InitJa25StrategicAiBloodcats( );
void SetNumberJa25EnemiesInSurfaceSector( INT32 iSectorID, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps );

BOOLEAN SaveJa25SaveInfoToSaveGame( HWFILE hFile );
BOOLEAN LoadJa25SaveInfoFromSavedGame( HWFILE hFile );


void AddEnemiesToJa25TunnelMaps();
void HandleAddingEnemiesToTunnelMaps();

void SetJa25GeneralFlag( UINT32 uiFlagToSet );
void ClearJa25GeneralFlag( UINT32 uiFlagToClear );
BOOLEAN IsJa25GeneralFlagSet( UINT32 uiFlagToCheck );








typedef struct
{
	UINT32	uiTimeOfLastBattleInMinutes;
	UINT32	uiTimeOfLastUpdate;

	INT16		iLastBattleSectorID;
	INT8		bLastBattleSectorZ;

	INT8		bFiller1;

	UINT32	uiAttackedSameSectorCounter;

	INT8		bFiller[16];

} JA25_SECTOR_AI_MANAGER;




typedef struct
{
	INT16		iSectorID;
	INT8		bSectorZ;

	INT8		bBaseNumEnemies;										// The base number of enemies that can present in the sector
	INT8		bRandomNumEnemies;									// The random amount of enemies that can be present

	INT8		bProbabilityOfAttacking;						// The probability Increase of attacking every hour of time compressing

	INT8		bMaxProbabilityForAttackingSector;	// The max amount of probabilty the enemy will attack enemy

	INT8		bCurrentProbability;								// Gets updated every hour when there can be an attack, reset when attack occurs

	BOOLEAN fPlayerControlled;								// Set when the player controls the sector

	UINT8		ubMinimumProbabiltyBeforeAttack;

	BOOLEAN	fPlayerHasLiberatedSectorBefore;		//Set when the player has a victory in the sector, never gets cleared after that

	BOOLEAN	fAutoDirection;											//Calculate direction enemy are coming in from

	UINT8		ubInsertionDirection;								// For custom maps, used to describe which direction enemies are from

	UINT32		sInsertionGridNo;										// For custom maps, the insertion gridno

	BOOLEAN	fCustomSector;											// Ah, a flag so the AI doesn't need to lookup sector info

	UINT8		ubLoadingScreenID;									// For a custom map, which loading screen to use

	BOOLEAN	fEnemyEnrouteToAttack;							// To help things so that the enemy wioll not continually send dudes

	INT8		bFiller[21];

} JA25_SECTOR_AI;

#define CUSTOMSECTOR 1020

extern	JA25_SECTOR_AI	gJa25AiSectorStruct[CUSTOMSECTOR];

enum
{
	JA25_H7,
	JA25_H8,
	JA25_H9,
	JA25_I9,
	JA25_H10,
	JA25_I10,	//5
	JA25_I11,
	JA25_J11,
	JA25_I12,
	JA25_J12,
	JA25_I13,		//10
	JA25_J13,
	JA25_I13_1,
	JA25_J13_1,
	JA25_J14_1,
	JA25_K14_1,		//15
	JA25_K15,
	JA25_K15_1,
	JA25_K15_2,
	JA25_L15_2,
	JA25_L15_3,

	NUM_CAMPAIGN_JA25_SECTORS
};




extern	BOOLEAN	gfEnemyShouldImmediatelySeekThePlayer;
//extern  INT32	giNumJA25Sectors;

void		SetJa25SectorOwnedStatus( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fPlayerOwned );
INT8		GetTheFurthestSectorPlayerOwns();
void		Ja25_UpdateTimeOfEndOfLastBattle( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
BOOLEAN Ja25_AreEnemiesAbleToAttackSector( INT16 iSectorID, INT8 bSectorZ );
BOOLEAN InitJa25SectorAi();
INT16		GetJA25SectorID( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
void		JA25_HandleUpdateOfStrategicAi();

void		SetEnemiesToFindThePlayerMercs();

void HandleSayingDontStayToLongWarningInSectorH8();

#ifdef JA2BETAVERSION
	BOOLEAN InitJa25StrategicAiDecisions( BOOLEAN fLoadedGame );
#endif

void HandleEnricosUnderstandingEmail();

void HandleRemovingEnemySoldierInitLinksIfPlayerEverWonInSector();

// -1 if enemy isnt supposed to seek player, otherwise SAI sectgor ID get returned
INT8 GetSectorEnemyIsToImmediatelySeekEnemyIn();

void AddJA25AIDataToSector( JA25_SECTOR_AI *pSectorAIInfo );
extern void	SetNumberOfJa25BloodCatsInSector( INT32 iSectorID, INT8 bNumBloodCats, INT8 bBloodCatPlacements  );

#endif

#endif