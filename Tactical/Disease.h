#ifndef __DISEASE_H
#define __DISEASE_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "Types.h"

// number of different diseases. Altering this will alter the size of INT32	sDiseasePoints[NUM_DISEASES]; in SOLDIERTYPE
#define NUM_DISEASES			20

#include "Soldier Control.h"

// infection types - how can we be infected
enum
{
	// infection depending on sector type
	INFECTION_TYPE_SWAMP = 0,
	INFECTION_TYPE_TROPICS,

	// infection by having sex
	INFECTION_TYPE_SEX,

	// infection by contact with lifeforms
	INFECTION_TYPE_CONTACT_HUMAN,			// infection by human interaction
	INFECTION_TYPE_CONTACT_CORPSE,			// infection by interacting with corpses
	
	// infections when taking damage
	INFECTION_TYPE_WOUND_ANIMAL,			// wounded by an animal (bloodcat)
	INFECTION_TYPE_WOUND_OPEN,				// we have a new non-bandaged wound
	INFECTION_TYPE_WOUND_GUNSHOT,			// we were wounded by gunshot

	// wound caused statloss
	INFECTION_TYPE_WOUND_AGI,
	INFECTION_TYPE_WOUND_DEX,
	INFECTION_TYPE_WOUND_STR,
	INFECTION_TYPE_WOUND_WIS,

	INFECTION_TYPE_TRAUMATIC,				// certain events traumatized us

	// infection by bad foodstuff
	INFECTION_TYPE_BADFOOD,
	INFECTION_TYPE_BADWATER,

	INFECTION_TYPE_MAX
};

// stats that can be affected
enum
{
	INFST_AGI = 0,
	INFST_DEX,
	INFST_STR,
	INFST_WIS,
	INFST_EXP,

	INFST_MAX
};

// disase flags for a sector
#define SECTORDISEASE_OUTBREAK			0x01	//1				// disease has officially broken out here - it can now be seen on the map, and the AI will try to remove it
#define SECTORDISEASE_DIAGNOSED_WHO		0x02	//2				// disease has been diagnosed by the WHO. This information can only be seen if the player has a contract with the WHO
#define SECTORDISEASE_DIAGNOSED_PLAYER	0x04	//4				// disease has been diagnosed by the player

// properties of diseases
#define DISEASE_PROPERTY_CANBECURED		0x00000001				// this disease can be healed by doctoring
#define DISEASE_PROPERTY_REVERSEONFULL	0x00000002				// once sInfectionPtsFull are reached, the infection reverses - negative sInfectionPtsGainPerHour is used
#define DISEASE_PROPERTY_CANREINFECT	0x00000004				// infection can be reapplied if already infected
#define DISEASE_PROPERTY_HIDESYMBOL		0x00000008				// do not show a symbol for this disease on the merc's face, even if diagnosed

#define DISEASE_PROPERTY_DISGUSTING		0x00000010				// other merc's will be disgusted by anyone with this disease if broken out

typedef struct
{
	UINT8		uiIndex;
	CHAR16		szName[80];							// name of disease, used for display
	CHAR16		szFatName[80];						// name of disease, used for display
	CHAR16		szDescription[256];					// a short description of the disease. Observe and learn, kids!

	// diseases are measured in disease points
	INT32		sInfectionPtsInitial;				// number of points gained on infection
	INT32		sInfectionPtsOutbreak;				// at this many points, the disease breaks out - effects start and one can see it
	INT32		sInfectionPtsFull;					// the nominal point of a full-blown infection. Effects are scaled towards this
	INT32		sInfectionPtsGainPerHour;			// extra points gained every hour if infected

	// infection chances and behaviour
	FLOAT		dInfectionChance[INFECTION_TYPE_MAX];		// percentage chance of being infected this way if conditions are fulfilled
	UINT32		usDiseaseProperties;				// properties of the disease

	// effects of disease (effect at sInfectionPtsFull points), scaled
	INT8		sEffStat[INFST_MAX];				// stat effectivity is altered
	INT8		sEffAP;								// alter APs
	UINT8		usMaxBreath;						// max Breath is lowered by this percentage
	INT8		sEffCarryStrength;					// carrying strength
	INT16		sLifeRegenHundreds;					// alter hourly life regen by these hundreds
	INT8		sNeedToSleep;						// +/- need to sleep
	INT16		sDrinkModifier;						// +/- % water consumption
	INT16		sFoodModifier;						// +/- % water consumption
	FLOAT		moralemodifier;						// a modifier to total morale
} DISEASE;

//GLOBALS
extern DISEASE Disease[NUM_DISEASES];

void HandleDisease();

// handle possible infection of type aInfectionType to pSoldier. Possible related to pOtherSoldier (can be NULL !)
// chance gets modified by aModifier (contextual modifier)
// if fStrategicOnly == TRUE, then we only handle disease 0
void HandlePossibleInfection( SOLDIERTYPE *pSoldier, SOLDIERTYPE* pOtherSoldier, UINT8 aInfectionType, FLOAT aModifier = 1.0f, BOOLEAN fStrategicOnly = FALSE );

////////////// STRATEGIC DISEASE //////////////////////////

enum
{
	MAPMODE_OFF = 0,
	MAPMODE_DISEASE,

	MAPMODE_MAX,
};

// directions for adjacent sectors
enum {
	SP_UP,
	SP_LEFT,
	SP_RIGHT,
	SP_DOWN,

	SP_DIR_MAX,
};

// return number of adjacent sector or -1 if there is none
INT16 GetAdjacentSector( UINT8 sector, UINT8 spdir );

// get a sector population (not the tactical one - we use an xml estimation + troops present)
UINT16 GetSectorPopulation( INT16 sX, INT16 sY, BOOLEAN fWithMilitary = TRUE );

// handle infection redistribution if people move from A to B (set sXB  and sYB to negative values to simply remove infected people in A)
void PopulationMove( INT16 sXA, INT16 sYA, INT16 sXB, INT16 sYB, UINT16 usAmount );

void HandleDeathDiseaseImplications( SOLDIERTYPE *pSoldier );

void HandleDiseaseDailyRefresh();

// heal sector population, return number of points used
UINT32 HealSectorPopulation( INT16 sX, INT16 sY, UINT32 uHealpts );

// Flugente: get workforce effectiveness with regards to strategic disease
FLOAT GetWorkforceEffectivenessWithDisease( INT8 bTownId, UINT8 usTeam );

// if this fraction of a sector's population is infected, outbreak is declared regardless of infection severity
// Otherwise infection would be untreatable at this point
FLOAT GetSectorDiseaseOverFlowThreshold();

#endif //__DISEASE_H
