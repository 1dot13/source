#ifndef _MILITIA_INDIVIDUAL_H
#define _MILITIA_INDIVIDUAL_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// These functions are used for the individual militia feature

#include "Assignments.h"
#include "soldier profile type.h"
#include "Animation Data.h"
#include "Soldier Create.h"

// not used atm
/*typedef struct
{
	UINT16	item;			// item id, refers to the ones in Items.xml
	UINT16	status;			// status of item. In case of ammo items, this instead refers to the ammo count. No bullets lost here.
} MILITIA_ITEM;*/

/*enum
{
WISDOM = NUM_TRAINABLE_STATS,
LEVEL,
HEALTH_CURRENT,
NUM_TOTAL_STATS
};*/

#define MILITIA_BATTLEREPORT_FLAG_RECRUITED				0x00000001	// this was not a battle, but we were recruited - use id as timestamp in hours
#define MILITIA_BATTLEREPORT_FLAG_PROMOTED				0x00000002	// this was not a battle, but we were promoted - use id as timestamp in hours
#define MILITIA_BATTLEREPORT_FLAG_DIED					0x00000004	// militia died in this battle
#define MILITIA_BATTLEREPORT_FLAG_WOUNDED_SMALL			0x00000008	// was lightly wounded

#define MILITIA_BATTLEREPORT_FLAG_WOUNDED_HEAVY			0x00000010	// was heavily wounded
#define MILITIA_BATTLEREPORT_FLAG_WOUNDED_COMA			0x00000020	// wounds were so heavy that we fell into a coma
#define MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY			0x00000040	// killed an enemy
#define	MILITIA_BATTLEREPORT_FLAG_FIRED					0x00000080	// militia was fired

#define MILITIA_BATTLEREPORT_FLAG_RECRUITED_TURNCOAT	0x00000100	// defected to our side

struct MILITIA_BATTLEREPORT
{
	MILITIA_BATTLEREPORT( ) : id( 0 ), flagmask( 0 )	{}

	UINT32	id;				// id of battle militia took part in. Ties to campaign history, thus we know with more detail what happened there.
	UINT32	flagmask;		// we can note up to 16 different properties here - perhaps wether we were a reinforcement, or were wounded and such
};

// a militia's origin affects the chances of his makeout
enum MilitiaOrigin
{
	MO_ARULCO,			// standard, militia was an aruclan volunteer that has been trained by the player
	MO_PMC,				// militia was hired from a PMC by the player
	MO_DEFECTOR,		// militia was an enemy captured by the player and switched to the player side during interrogation

	MO_MAX
};

// This struct defines necessary data for a pool of militia origins.
// For example, if you want PMC militia to resemble eastern european buff guys, you only need to define the specific fore- and surnames, 
// set the chance for huge male bodytpes high, and give according skin colour chances
typedef struct
{
	// possible name pool
	std::vector<std::wstring> szMale_Forename;
	std::vector<std::wstring> szMale_Surname;
	std::vector<std::wstring> szFemale_Forename;
	std::vector<std::wstring> szFemale_Surname;

	// chances for bodytypes and skin colours (hair is chosen auomatically according to coded scheme)
	UINT8 chance_bodytype[ADULTFEMALEMONSTER];
	UINT8 chance_skin[NUMSKINS];
	UINT16 dailycost[MAX_MILITIA_LEVELS];
} MilitiaOriginData;

extern MilitiaOriginData gMilitiaOriginData[MO_MAX];

#define MILITIAFLAG_DEAD					0x00000001	// this guy is dead
#define MILITIAFLAG_FIRED					0x00000002	// this guy no longer works for us - we fired him, or didn't have the money to keep them
#define MILITIAFLAG_NEEDS_SECTOR			0x00000004	// as the player currently resets militia in a town's sector in the map, this militia's sector is not clear
#define MILITIAFLAG_DESERTION				0x00000008	// this guy deserted

class MILITIA
{
public: 
	MILITIA();

	BOOLEAN		Save( HWFILE hFile );
	BOOLEAN		Load( HWFILE hwFile );

	STR16	GetName( );
	STR16	GetSector( );
	STR16	GetOriginSector( );
	void	AddKills( UINT16 aKills, UINT16 aAssists );
	UINT16	GetWage();
	void	Fire();			// release militia from our service. Does not take care of actually removing a militia from the sector!

	UINT32	id;				// number of militia. Tied with one or more separate tables of names, this gives us a name
	UINT32	flagmask;		// individual flags - we are dead etc.

	// origin
	UINT8	origin;			// one of MilitiaOrigin
	UINT8	originsector;	// sector militia was first created in
	UINT8	sector;			// sector militia currently is in
	UINT8	age;
	
	// visuals
	UINT8	militiarank;	// green/veteran/elite
	UINT8	bodytype;		// male/bigmale1/bigmale2/female
	UINT8	skin;
	UINT8	hair;

	// name
	UINT16	forename;
	UINT16	surname;
	UINT16	callsign;
		
	// As stats are highly dependent of game progress and current sector, storing the stats themselves is ill-advised.
	// We thus only store the current HP percentage.
	FLOAT	healthratio;

	UINT16	kills;
	UINT16	assists;

	// we don't store the experience level, we store the amount of points in regard to promotion. 
	// While this is made up of kills and assists, we need this, as we have to account for militia initially starting at regular level
	FLOAT	promotionpoints;

	char endOfPOD;	// marker for end of POD (plain old data)
	
	std::vector<MILITIA_BATTLEREPORT>	history;	// reports on all battles this militia took part in
};

#define SIZEOF_MILITIA_POD offsetof( MILITIA, endOfPOD )

// later on, make this an array with a vector for each sector
extern std::vector<MILITIA> gIndividualMilitiaVector;

void InitIndividualMilitiaData();
BOOLEAN SaveIndividualMilitiaData( HWFILE hwFile );
BOOLEAN LoadIndividualMilitiaData( HWFILE hwFile );

// wrapper functions to retrieve militia
UINT32 TotalNumberIndividualMilitia( );
UINT32 GetFreeIndividualMilitiaId();
BOOLEAN IsLegalMilitiaId( UINT32 aId );

// return militia with a certain id
BOOLEAN GetMilitia( UINT32 aId, MILITIA* apMilitia );

// update data
void UpdateMilitia( MILITIA aMilitia );

SOLDIERTYPE* GetUsedSoldierToIndividualMilitia( UINT32 aMilitiaId );

// apply tactical life ratio to militia health ratio
void ApplyTacticalLifeRatioToMilitia();

// apply health ratios to militia in tactical
void ApplyMilitiaHealthRatioToTactical();

void HandleHourlyMilitiaHealing( );
UINT32 MilitiaIndividual_Heal(UINT32 points, UINT8 aSector );

UINT32 CreateRandomIndividualMilitia( UINT8 aMilitiaRank, UINT8 aOrigin, UINT8 aSector );

UINT32 CreateNewIndividualMilitia( UINT8 aMilitiaRank, UINT8 aOrigin, UINT8 aSector );

UINT32 CreateNewIndividualMilitiaFromSoldier( SOLDIERTYPE* pSoldier, UINT8 aOrigin );

UINT32 GetIdOfUnusedIndividualMilitia( UINT8 aSoldierClass, UINT8 aSector );

BOOLEAN GetIdOfIndividualMilitiaWithClassSector( UINT8 aSoldierClass, UINT8 aSector, UINT32& arId );

FLOAT PromoteIndividualMilitiaInSector( UINT8 aSector, FLOAT aPointsToAdd );

// handle possible militia promotion and individual militia update
void HandlePossibleMilitiaPromotion( SOLDIERTYPE* pSoldier, BOOLEAN aAutoResolve );

void MoveIndividualMilitiaProfiles( UINT8 aSourceSector, UINT8 aTargetSector, UINT8 usGreens, UINT8 usRegulars, UINT8 usElites );

// militia are disbanded - we have to take note of that
void DisbandIndividualMilitia( UINT8 aSector, UINT8 usGreens, UINT8 usRegulars, UINT8 usElites );

void PromoteIndividualMilitia( UINT8 aSector, UINT8 aSoldierClass );

void PossiblyPromoteIndividualMilitia( MILITIA& aMilitia );

// return the sum of daily wages, and the number of individual militia per class
UINT32 GetDailyUpkeep_IndividualMilitia( UINT32& arNumGreen, UINT32& arNumRegular, UINT32& arNumElite );

// if the player repositions militia in a town in strategic, we need to reposition militia as well
void PickIndividualMilitia( UINT8 aSector, UINT8 ubType, UINT16 aNumber );
void DropIndividualMilitia( UINT8 aSector, UINT8 ubType, UINT16 aNumber );

#endif
