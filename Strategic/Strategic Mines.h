#ifndef __STRATEGIC_MINES_H
#define __STRATEGIC_MINES_H

// the .h to the mine management system

#include "Types.h"
#include "strategicmap.h"
#include "mapscreen.h"

// the mines
enum{
	MINE_SAN_MONA = 0,
		MINE_DRASSEN,
		MINE_ALMA,
		MINE_CAMBRIA,
		MINE_CHITZENA,
		MINE_GRUMM,
	//MAX_NUMBER_OF_MINES,
};

//enum{
//		MINER_FRED = 0,
//		MINER_MATT,
//		MINER_OSWALD,
//		MINER_CALVIN,
//		MINER_CARL,
//	NUM_HEAD_MINERS,
//};

// different types of mines
enum{
	SILVER_MINE=0,
	GOLD_MINE,
	NUM_MINE_TYPES,
};


// monster infestatation level, as percieved by the villagers
enum{
	MINES_NO_MONSTERS = 0,	// nothing there at all
	MINES_TRACE_MONSTERS,	// monsters leave traces behind, but no one has seen them yet
	MINES_SOME_MONSTERS,		// there have been sightings but no one believes the witnesses
	MINES_MODERATE_MONSTERS, // scattered reports of monsters in lower levels, leaves workers uneasy
	MINES_HIGH_MONSTERS,		// workers know they are there and all but a few refuse to work
	MINES_RAMPANT_MONSTERS,	// the few that go down don't seem to return, no one wants to work period
	MINES_MONSTERS_SURFACE,	// monsters are coming out of the mines into the town looking for any edibles
};

// head miner quote types
enum{
	HEAD_MINER_STRATEGIC_QUOTE_RUNNING_OUT = 0,
	HEAD_MINER_STRATEGIC_QUOTE_CREATURES_ATTACK,
	HEAD_MINER_STRATEGIC_QUOTE_CREATURES_GONE,
	HEAD_MINER_STRATEGIC_QUOTE_CREATURES_AGAIN,
	NUM_HEAD_MINER_STRATEGIC_QUOTES
};


struct AssociatedMineSector
{
	UINT8 mineID;
	UINT8 x, y, z;
};

// the strategic mine structures
//typedef struct MINE_LOCATION_TYPE
//{
//	INT16	sSectorX;						// x value of sector mine is in
//	INT16	sSectorY;						// y value of sector mine is in
//	INT8	bAssociatedTown;			// associated town of this mine
//
//} MINE_LOCATION_TYPE;

typedef struct MINE_STATUS_TYPE
{
	UINT8	ubMineType;								// type of mine (silver or gold)
	BYTE		filler1[3];
	UINT32	uiMaxRemovalRate;					// fastest rate we can move ore from this mine in period

	UINT32	uiRemainingOreSupply;			// the total value left to this mine (-1 means unlimited)
	UINT32	uiOreRunningOutPoint;			// when supply drop below this, workers tell player the mine is running out of ore

	BOOLEAN	fEmpty;										// whether no longer minable
	BOOLEAN	fRunningOut;								// whether mine is beginning to run out
	BOOLEAN	fWarnedOfRunningOut;				// whether mine foreman has already told player the mine's running out
	BOOLEAN	fShutDownIsPermanent;			// means will never produce again in the game (head miner was attacked & died/quit)
	BOOLEAN	fShutDown;									// TRUE means mine production has been shut off
	BOOLEAN	fPrevInvadedByMonsters;		// whether or not mine has been previously invaded by monsters
	BOOLEAN	fSpokeToHeadMiner;					// player doesn't receive income from mine without speaking to the head miner first
	BOOLEAN	fMineHasProducedForPlayer;	// player has earned income from this mine at least once

	BOOLEAN	fQueenRetookProducingMine;	// whether or not queen ever retook a mine after a player had produced from it
	BOOLEAN	fAttackedHeadMiner;				// player has attacked the head miner, shutting down mine & decreasing loyalty
	UINT16	usValidDayCreaturesCanInfest; //Creatures will be permitted to spread if the game day is greater than this value.
	UINT32	uiTimePlayerProductionStarted;		// time in minutes when 'fMineHasProducedForPlayer' was first set

	BYTE	 filler[11];					// reserved for expansion

	// merge in MINE_LOCATION_TYPE
	INT16	sSectorX;						// x value of sector mine is in
	INT16	sSectorY;						// y value of sector mine is in
	INT8	bAssociatedTown;			// associated town of this mine
	
	UINT32 StrategicIndex() { return CALCULATE_STRATEGIC_INDEX(sSectorX, sSectorY); };

	BOOLEAN fInfectible;

} MINE_STATUS_TYPE;

typedef struct HEAD_MINER_TYPE
{
	UINT16	usProfileId;
	INT8		bQuoteNum[NUM_HEAD_MINER_STRATEGIC_QUOTES];
	UINT8		ubExternalFaceIndex;
} HEAD_MINER_TYPE;


extern INT32 MAX_NUMBER_OF_MINES;
extern INT32 NUM_HEAD_MINERS;
extern std::vector<MINE_STATUS_TYPE> gMineStatus;
extern std::vector<AssociatedMineSector> associatedMineSectors;
extern std::vector<HEAD_MINER_TYPE> gHeadMinerData;

// init mines
void InitializeMines( void );

void HourlyMinesUpdate(void);

// get total left in this mine
INT32 GetTotalLeftInMine( INT8 bMineIndex );

// get max rates for this mine (per period, per day)
UINT32 GetMaxPeriodicRemovalFromMine( INT8 bMineIndex );
UINT32 GetMaxDailyRemovalFromMine( INT8 bMineIndex );

// which town does this mine belong to?
INT8 GetTownAssociatedWithMine( INT8 bMineIndex );

// posts the actual mine production events daily
void PostEventsForMineProduction(void);

// the periodic checking for income from mines
void HandleIncomeFromMines( void );

// predict income from mines
INT32 PredictIncomeFromPlayerMines( BOOLEAN fIncludeFacilities );

// predict income from a mine
UINT32 PredictDailyIncomeFromAMine( INT8 bMineIndex, BOOLEAN fIncludeFacilities );

// calculate maximum possible daily income from all mines
INT32 CalcMaxPlayerIncomeFromMines( void );

// get index value of this mine in the mine list
INT8 GetMineIndexForSector( INT16 sX, INT16 sY );

// get the index of the mine associated with this town
INT8 GetMineIndexForTown( INT8 bTownId );

// get the sector value for the mine associated with this town
INT16 GetMineSectorForTown( INT8 bTownId );

// is there a mine here?
BOOLEAN IsThereAMineInThisSector( INT16 sX, INT16 sY );

//Save the mine status to the save game file
BOOLEAN SaveMineStatusToSaveGameFile( HWFILE hFile );

//Load the mine status from the saved game file
BOOLEAN LoadMineStatusFromSavedGameFile( HWFILE hFile );

// if the player controls a given mine
BOOLEAN PlayerControlsMine(INT8 bMineIndex);

void ShutOffMineProduction( INT8 bMineIndex );
void RestartMineProduction( INT8 bMineIndex );
void MineShutdownIsPermanent( INT8 bMineIndex );

BOOLEAN IsMineShutDown( INT8 bMineIndex );

UINT8 GetHeadMinerIndexForMine( INT8 bMineIndex );
//UINT16 GetHeadMinerProfileIdForMine( INT8 bMineIndex );

// Find the sector location of a mine
void GetMineSector( UINT8 ubMineIndex, INT16 * psX, INT16 * psY );

void IssueHeadMinerQuote( INT8 bMineIndex, UINT8 ubQuoteType );

UINT8 GetHeadMinersMineIndex(UINT8 ubMinerProfileId);

void PlayerSpokeToHeadMiner(UINT8 ubMinerProfile );

BOOLEAN IsHisMineRunningOut(UINT8 ubMinerProfileId );
BOOLEAN IsHisMineEmpty(UINT8 ubMinerProfileId );
BOOLEAN IsHisMineDisloyal(UINT8 ubMinerProfileId );
BOOLEAN IsHisMineInfested(UINT8 ubMinerProfileId );
BOOLEAN IsHisMineLostAndRegained(UINT8 ubMinerProfileId );
BOOLEAN IsHisMineAtMaxProduction(UINT8 ubMinerProfileId );
void ResetQueenRetookMine( UINT8 ubMinerProfileId );

void QueenHasRegainedMineSector(INT8 bMineIndex);

BOOLEAN HasAnyMineBeenAttackedByMonsters(void);

void PlayerAttackedHeadMiner( UINT8 ubMinerProfileId );

BOOLEAN HasHisMineBeenProducingForPlayerForSomeTime( UINT8 ubMinerProfileId );

// given sector value, get mine id value
INT8 GetIdOfMineForSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

// use this for miner (civilian) quotes when *underground* in a mine
//BOOLEAN PlayerForgotToTakeOverMine( UINT8 ubMineIndex );

// use this to determine whether or not to place miners into a underground mine level
BOOLEAN AreThereMinersInsideThisMine( UINT8 ubMineIndex );

// use this to determine whether or not the player has spoken to a head miner
BOOLEAN SpokenToHeadMiner( UINT8 ubMineIndex );

#endif