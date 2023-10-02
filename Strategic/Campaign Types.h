#ifndef __CAMPAIGN_TYPES_H
#define __CAMPAIGN_TYPES_H

#include "types.h"
#include "DEBUG.H"
#include <map>

const int MAXIMUM_VALID_X_COORDINATE = 16;
const int MINIMUM_VALID_X_COORDINATE = 1;
const int MAXIMUM_VALID_Y_COORDINATE = 16;
const int MINIMUM_VALID_Y_COORDINATE = 1;
const int MAXIMUM_VALID_Z_COORDINATE = 3;
const int MINIMUM_VALID_Z_COORDINATE = 0;

//Macro to convert sector coordinates (1-16,1-16) to 0-255
#ifdef USE_AGGRESSIVE_ASSERTIONS
#define SECTOR(x,y)			(UINT8)(AssertGE(x, MINIMUM_VALID_X_COORDINATE), \
	                                AssertLE(x, MAXIMUM_VALID_X_COORDINATE), \
	                                AssertGE(y, MINIMUM_VALID_Y_COORDINATE), \
	                                AssertLE(y, MAXIMUM_VALID_Y_COORDINATE), \
									((y-1)*16+x-1))
#define SECTORX(SectorID)	(UINT8)(AssertLE(SectorID, MAXIMUM_VALID_X_COORDINATE*MAXIMUM_VALID_Y_COORDINATE), \
	                                ((SectorID % 16) + 1))
#define SECTORY(SectorID)	(UINT8)(AssertLE(SectorID, MAXIMUM_VALID_X_COORDINATE*MAXIMUM_VALID_Y_COORDINATE), \
	                                ((SectorID / 16) + 1))
#else
#define SECTOR(x,y)			(UINT8)(((y-1)*16+x-1))
#define SECTORX(SectorID)	(UINT8)(((SectorID % 16) + 1))
#define SECTORY(SectorID)	(UINT8)(((SectorID / 16) + 1))
#endif

//Sector enumerations
//
//NOTE: These use the 0-255 SectorInfo[] numbering system, and CAN'T be used as indexes into the StrategicMap[] array
//Use SECTOR_INFO_TO_STRATEGIC_INDEX() macro to convert...
enum
{
	SEC_A1,	SEC_A2,	SEC_A3, SEC_A4,	SEC_A5, SEC_A6,	SEC_A7,	SEC_A8,	SEC_A9, SEC_A10, SEC_A11, SEC_A12, SEC_A13, SEC_A14, SEC_A15, SEC_A16,
	SEC_B1,	SEC_B2,	SEC_B3, SEC_B4,	SEC_B5, SEC_B6,	SEC_B7,	SEC_B8,	SEC_B9, SEC_B10, SEC_B11, SEC_B12, SEC_B13, SEC_B14, SEC_B15, SEC_B16,
	SEC_C1,	SEC_C2,	SEC_C3, SEC_C4,	SEC_C5, SEC_C6,	SEC_C7,	SEC_C8,	SEC_C9, SEC_C10, SEC_C11, SEC_C12, SEC_C13, SEC_C14, SEC_C15, SEC_C16,
	SEC_D1,	SEC_D2,	SEC_D3, SEC_D4,	SEC_D5, SEC_D6, SEC_D7,	SEC_D8,	SEC_D9, SEC_D10, SEC_D11, SEC_D12, SEC_D13, SEC_D14, SEC_D15, SEC_D16,
	SEC_E1,	SEC_E2,	SEC_E3, SEC_E4,	SEC_E5, SEC_E6,	SEC_E7,	SEC_E8,	SEC_E9, SEC_E10, SEC_E11, SEC_E12, SEC_E13, SEC_E14, SEC_E15, SEC_E16,
	SEC_F1,	SEC_F2,	SEC_F3, SEC_F4,	SEC_F5, SEC_F6,	SEC_F7,	SEC_F8,	SEC_F9, SEC_F10, SEC_F11, SEC_F12, SEC_F13, SEC_F14, SEC_F15, SEC_F16,
	SEC_G1,	SEC_G2,	SEC_G3, SEC_G4,	SEC_G5, SEC_G6,	SEC_G7,	SEC_G8,	SEC_G9, SEC_G10, SEC_G11, SEC_G12, SEC_G13, SEC_G14, SEC_G15, SEC_G16,
	SEC_H1,	SEC_H2,	SEC_H3, SEC_H4,	SEC_H5, SEC_H6,	SEC_H7,	SEC_H8,	SEC_H9, SEC_H10, SEC_H11, SEC_H12, SEC_H13, SEC_H14, SEC_H15, SEC_H16,
	SEC_I1,	SEC_I2,	SEC_I3, SEC_I4,	SEC_I5, SEC_I6,	SEC_I7,	SEC_I8,	SEC_I9, SEC_I10, SEC_I11, SEC_I12, SEC_I13, SEC_I14, SEC_I15, SEC_I16,
	SEC_J1,	SEC_J2,	SEC_J3, SEC_J4,	SEC_J5, SEC_J6,	SEC_J7,	SEC_J8,	SEC_J9, SEC_J10, SEC_J11, SEC_J12, SEC_J13, SEC_J14, SEC_J15, SEC_J16,
	SEC_K1,	SEC_K2,	SEC_K3, SEC_K4,	SEC_K5, SEC_K6,	SEC_K7,	SEC_K8,	SEC_K9, SEC_K10, SEC_K11, SEC_K12, SEC_K13, SEC_K14, SEC_K15, SEC_K16,
	SEC_L1,	SEC_L2,	SEC_L3, SEC_L4,	SEC_L5, SEC_L6,	SEC_L7,	SEC_L8,	SEC_L9, SEC_L10, SEC_L11, SEC_L12, SEC_L13, SEC_L14, SEC_L15, SEC_L16,
	SEC_M1,	SEC_M2,	SEC_M3, SEC_M4,	SEC_M5, SEC_M6,	SEC_M7,	SEC_M8,	SEC_M9, SEC_M10, SEC_M11, SEC_M12, SEC_M13, SEC_M14, SEC_M15, SEC_M16,
	SEC_N1,	SEC_N2,	SEC_N3, SEC_N4,	SEC_N5, SEC_N6,	SEC_N7,	SEC_N8,	SEC_N9, SEC_N10, SEC_N11, SEC_N12, SEC_N13, SEC_N14, SEC_N15, SEC_N16,
	SEC_O1,	SEC_O2,	SEC_O3, SEC_O4,	SEC_O5, SEC_O6,	SEC_O7,	SEC_O8,	SEC_O9, SEC_O10, SEC_O11, SEC_O12, SEC_O13, SEC_O14, SEC_O15, SEC_O16,
	SEC_P1,	SEC_P2,	SEC_P3, SEC_P4,	SEC_P5, SEC_P6,	SEC_P7,	SEC_P8,	SEC_P9, SEC_P10, SEC_P11, SEC_P12, SEC_P13, SEC_P14, SEC_P15, SEC_P16
};

enum //group types
{
	NOGROUP,
	MOBILE,
	DEFENCE
};

enum //strategic values for each sector
{
	NO_VALUE,
	LOW_VALUE,
	FAIR_VALUE,
	AVG_VALUE,
	GOOD_VALUE,
	HI_VALUE,
	GREAT_VALUE
};


//Various flag definitions

#define SF_USE_MAP_SETTINGS							0x00000001
#define SF_ENEMY_AMBUSH_LOCATION					0x00000002

//Special case flag used when players encounter enemies in a sector, then retreat.	The number of enemies
//will display on mapscreen until time is compressed.	When time is compressed, the flag is cleared, and
//a question mark is displayed to reflect that the player no longer knows.
#define SF_PLAYER_KNOWS_ENEMIES_ARE_HERE			0x00000004	

#define SF_SAM_SITE									0x00000008
#define SF_MINING_SITE								0x00000010
#define SF_ALREADY_VISITED							0x00000020
#define SF_USE_ALTERNATE_MAP						0x00000040
#define SF_PENDING_ALTERNATE_MAP					0x00000080
#define SF_ALREADY_LOADED							0x00000100
#define SF_HAS_ENTERED_TACTICAL						0x00000200
#define SF_SKYRIDER_NOTICED_ENEMIES_HERE			0x00000400
#define SF_HAVE_USED_GUIDE_QUOTE					0x00000800

#ifdef JA2UB
#define	SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR		0x00001000
#endif

#define SF_ASSIGN_NOTICED_ENEMIES_HERE				0x00002000		// Flugente: flag deleted after 1 hour, info from assigment: enemies were found
#define SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER		0x00004000		// Flugente: flag deleted after 1 hour, info from assigment: enemies were found, and we know their number

#define SF_ASSIGN_PERMAINFO_ENEMIES_HERE			0x00008000		// Flugente: permanent flag, info from buying info: enemies were found
#define SF_ASSIGN_PERMAINFO_ENEMIES_KNOW_NUMBER		0x00010000		// Flugente: permanent flag, info from buying info: enemies were found, and we know their number

#define SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS			0x00100000		//Temp File starts with sm_
#define SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS		0x00200000		//Temp File starts with l_
#define SF_REVEALED_STATUS_TEMP_FILE_EXISTS			0x01000000		//Temp File starts with v_
#define SF_DOOR_STATUS_TEMP_FILE_EXISTS				0x02000000		//Temp File starts with ds_
#define SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS			0x04000000		//Temp File starts with e_
#define SF_CIV_PRESERVED_TEMP_FILE_EXISTS			0x08000000		//Temp File starts with c_
#define SF_ITEM_TEMP_FILE_EXISTS					0x10000000		//Temp File starts with i_
#define SF_ROTTING_CORPSE_TEMP_FILE_EXISTS			0x20000000		//Temp File starts with r_
#define SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS		0x40000000		//Temp File starts with m_
#define SF_DOOR_TABLE_TEMP_FILES_EXISTS				0x80000000		//Temp File starts with d_


// town militia experience categories
enum
{
	GREEN_MILITIA = 0,
	REGULAR_MILITIA,
	ELITE_MILITIA,
	MAX_MILITIA_LEVELS
};

// HEADROCK HAM 3.5: No longer needed. Facility types externalized.
/*// facilities flags
#define SFCF_HOSPITAL	0x00000001
#define SFCF_INDUSTRY	0x00000002
#define SFCF_PRISON		0x00000004
#define SFCF_MILITARY	0x00000008
#define SFCF_AIRPORT	0x00000010
#define SFCF_GUN_RANGE	0x00000020*/

// HEADROCK HAM 3.4: These are no longer needed anywhere in the code.
/*// coordinates of shooting range sector
#define GUN_RANGE_X		13
#define GUN_RANGE_Y		MAP_ROW_H
#define GUN_RANGE_Z		0

// HEADROCK HAM 3.2: New values for the location of an Industrial sector in Grumm H2. This sector boosts repair rates.
#define INDUSTRIAL_X	2
#define INDUSTRIAL_Y	MAP_ROW_H
#define INDUSTRIAL_Z	0*/

// HEADROCK HAM 3.4: Facility Locations
typedef struct FACILITYLOCATIONS
{
	BOOLEAN fFacilityHere;
	UINT8 ubHidden;
}FACILITYLOCATIONS;

enum
{
	// Enumerate the different assignments that a facility can emulate.
	FAC_AMBIENT = 0,
	FAC_STAFF,
	FAC_FOOD,
	FAC_REST,
	FAC_REPAIR_ITEMS,
	FAC_REPAIR_VEHICLE,
	FAC_REPAIR_ROBOT,
	FAC_DOCTOR,
	FAC_PATIENT,
	FAC_PRACTICE_STRENGTH,
	FAC_PRACTICE_DEXTERITY,
	FAC_PRACTICE_AGILITY,
	FAC_PRACTICE_HEALTH,
	FAC_PRACTICE_MARKSMANSHIP,
	FAC_PRACTICE_MEDICAL,
	FAC_PRACTICE_MECHANICAL,
	FAC_PRACTICE_LEADERSHIP,
	FAC_PRACTICE_EXPLOSIVES,
	FAC_STUDENT_STRENGTH,
	FAC_STUDENT_DEXTERITY,
	FAC_STUDENT_AGILITY,
	FAC_STUDENT_HEALTH,
	FAC_STUDENT_MARKSMANSHIP,
	FAC_STUDENT_MEDICAL,
	FAC_STUDENT_MECHANICAL,
	FAC_STUDENT_LEADERSHIP,
	FAC_STUDENT_EXPLOSIVES,
	FAC_TRAINER_STRENGTH,
	FAC_TRAINER_DEXTERITY,
	FAC_TRAINER_AGILITY,
	FAC_TRAINER_HEALTH,
	FAC_TRAINER_MARKSMANSHIP,
	FAC_TRAINER_MEDICAL,
	FAC_TRAINER_MECHANICAL,
	FAC_TRAINER_LEADERSHIP,
	FAC_TRAINER_EXPLOSIVES,	
	FAC_INTERROGATE_PRISONERS,
	FAC_PRISON_SNITCH,
	FAC_SPREAD_PROPAGANDA,
	FAC_SPREAD_PROPAGANDA_GLOBAL,
	FAC_GATHER_RUMOURS,
	FAC_STRATEGIC_MILITIA_MOVEMENT,
	NUM_FACILITY_ASSIGNMENTS,
};


// Enumerated list of risk types
enum
{
	RISK_STRENGTH = 0,
	RISK_HEALTH,
	RISK_AGILITY,
	RISK_DEXTERITY,
	RISK_WISDOM,
	RISK_MARKSMANSHIP,
	RISK_LEADERSHIP,
	RISK_EXPLOSIVES,
	RISK_MECHANICAL,
	RISK_MEDICAL,

	RISK_INJURY,
	RISK_MORALE,
	RISK_FATIGUE,
	RISK_DRUNK,

	RISK_LOYALTY_LOCAL,
	RISK_LOYALTY_GLOBAL,

	NUM_RISKS,
};

enum class FacilityRiskVectorTypes
{
	RISK_DRUG_ITEMS,
};

typedef struct FACILITYRISKTYPE
{
	// The risks involved with perfoming an assignment at a specific facility.

	UINT16 usChance;			// Chance out of X that this risk will trigger every hour. X is INI-defined and is normally 1000.
	INT8 bBaseEffect;			// Base result. If negative, result will always be negative. If positive, result will always be positive.
								// If 0, result can be either negative or positive.
	UINT8 ubRange;				// Range of deviation for the base effect.
	std::map<FacilityRiskVectorTypes, std::vector<INT16>> valueVectors;		// Optional additional data

} FACILITYRISKTYPE;

typedef struct FACILITYASSIGNMENTTYPE
{
	// This contains data about performing a specific assignment at a specific facility.

	UINT8 ubStaffLimit;				// Maximum number of people who can perform this assignment simultaneously
	CHAR16 szTooltipText[300];		// Tooltip explaining the assignment in general terms.
	INT16 sCostPerHour;				// Cost per hour. Can be negative to indicate FLAT PROFIT per hour.

	UINT16 usPerformance;			// Percentage effectiveness of this assignment compared to non-emulated assignment (100 = normal)
	UINT16 usFatigue;				// Percentage speed of losing Max Breath (100 = normal)
	UINT16 usSleep;					// Percentage effectiveness of sleeping on the job (100 = normal)
	UINT16 usKitDegrade;			// Percentage speed of kit degrade (100 = normal)

	UINT8 ubMaximumMorale;			// Maximum morale achievable while performing this assignment
	UINT8 ubMaximumBreath;			// Maximum breath achievable while performing this assignment

	UINT8 ubDetectEnemiesImmediate;	// Detection of enemies in adjacent sectors, value = range of detection.
	BOOLEAN fDetectEnemiesLongrange;	// Detection of enemies in explored territory
	BOOLEAN fDetectEnemiesDynamic;		// Detection of enemies in Mobile Militia areas
	BOOLEAN fDetectEnemiesAnywhere;		// Detection of enemies all over the map
	BOOLEAN fCountEnemiesInWild;		// Count number of enemies in wilderness sectors
	BOOLEAN fCountEnemiesInCities;		// Count number of enemies in city sectors

	INT16 sSkyriderCostModifier;	// Flat modifier for Skyrider's Cost Per Tile
	UINT16 usMineIncomeModifier;	// Percentage income adjustment
	BOOLEAN fOnlyLocalMineAffected;		// Determines whether income modifier applies to local mine only, or all mines.
	INT16 sCantinaFoodModifier;		// how many food points do we get when spending time in a cantina?
	UINT16 usPrisonBaseLimit;		// for how many prisoners was this prison built?
	
	UINT8 ubMinimumStrength;		// Minimum STR Requirement to begin this assignment
	UINT8 ubMinimumHealth;			// Minimum HLT Requirement to begin this assignment
	UINT8 ubMinimumWisdom;			// Minimum WIS Requirement to begin this assignment
	UINT8 ubMinimumAgility;			// Minimum AGI Requirement to begin this assignment
	UINT8 ubMinimumDexterity;		// Minimum DEX Requirement to begin this assignment
	UINT8 ubMinimumMarksmanship;	// Minimum MRK Requirement to begin this assignment
	UINT8 ubMinimumLeadership;		// Minimum LDR Requirement to begin this assignment
	UINT8 ubMinimumExplosives;		// Minimum EXP Requirement to begin this assignment
	UINT8 ubMinimumMechanical;		// Minimum MEC Requirement to begin this assignment
	UINT8 ubMinimumMedical;			// Minimum MED Requirement to begin this assignment
	UINT8 ubMinimumLevel;			// Minimum LVL Requirement to begin this assignment

	UINT8 ubMinimumLoyaltyHere;		// Minimum loyalty required in the town where the facility is placed (if any)
	UINT8 ubMinimumBreath;			// Minimum Breath Requirement to begin this assignment
	UINT8 ubMinimumMorale;			// Minimum Morale Requirement to begin this assignment

	FACILITYRISKTYPE Risk[ NUM_RISKS ];		// Data about risks/chances possible when doing this assignment

} FACILITYASSIGNMENTTYPE;

// struct needed during production and laptop display
typedef struct PRODUCTION_LINE_PREPRODUCT
{
	UINT16 item;
	UINT32 requiredforonecreation;
} PRODUCTION_LINE_PREPRODUCT;

// -------- added by Flugente: various flags for factories --------
#define REQUIRES_STAFFING		0x00000001	//1			// the factory can only be activated if the facility is staffed

typedef struct PRODUCTION_LINE
{
	// determines which items can be produced here with what requirement
	CHAR16 szProductionName[30];
	CHAR16 szAdditionalRequirementTips[80];
	UINT16 usItemToCreate;		// item that shall be created
	INT32 sMinutesRequired;		// time required to create one item
	INT32 sGridNo_Creation;		// the location where the item is dropped at

	UINT32 usProductionFlags;	// flagmask for various properties

	INT32 sHourlyCost;			// cost for every hour of work (NOT per item). Negative values mean we gain money.
	UINT8 usOptional_LoyaltyRequired;	// loyalty in the sector must be at least this value. Only applicable in town sectors.

	// this vector contains all the items we need to create what we want in what quantity
	std::vector<PRODUCTION_LINE_PREPRODUCT> usOptional_PreProducts;
} PRODUCTION_LINE;

// Flugente: facilitytype properties
#define FACILITYTYPE_PMCENTRYPOINT		0x00000001				// militia hired from the PMC can only enter Arulco in sectors with facilites with this property

// HEADROCK HAM 3.5: Facility Type
typedef struct FACILITYTYPE
{
	// Basic data about the facility

	CHAR16 szFacilityName[30];				// Long facility name
	CHAR16 szFacilityShortName[15];			// Short facility name
	UINT8 ubTotalStaffLimit;				// Total number of people who can work here simultaneously

	UINT8 ubMilitiaTrainersAllowed;			// Number of Militia trainers allowed simultaneously
	UINT16 usMilitiaTraining;				// Percentage effectiveness of Militia training (100 = normal)

	UINT32 usFacilityFlags;					// flagmask for various facility properties

	FACILITYASSIGNMENTTYPE AssignmentData[ NUM_FACILITY_ASSIGNMENTS ];		// Data about possible assignments that can be done here

	std::vector<PRODUCTION_LINE> ProductionData;

} FACILITYTYPE;

// HEADROCK HAM 3.5: Maximum number of different facility types
#define MAX_NUM_FACILITY_TYPES 255
// HEADROCK HAM 3.5: Number of facilities placed on the map
extern UINT16 NUM_FACILITIES;
// HEADROCK HAM 3.5: Number of facility types read from XML
extern UINT16 NUM_FACILITY_TYPES;
// HEADROCK HAM 3.4: Array to hold facilities in each sector
extern FACILITYLOCATIONS gFacilityLocations[256][MAX_NUM_FACILITY_TYPES];
// HEADROCK HAM 3.5: Array to hold all different facility types
extern FACILITYTYPE gFacilityTypes[MAX_NUM_FACILITY_TYPES];

// HEADROCK HAM 3.6: Bloodcat Placement and Encounter data.
typedef struct BLOODCATPLACEMENT
{
	UINT8 PlacementType;
	UINT8 ubMinBloodcats;
	UINT8 ubMaxBloodcats;
	UINT8 ubAmbushChance;
	BOOLEAN ubFactionAffiliation;
	BOOLEAN fRespawn;
}BLOODCATPLACEMENT;

// HEADROCK HAM 3.6: Array to hold bloodcat placements and user-set bloodcat encounter data. There are 256 of these
// (one for each sector), with 4 sublevels (one for each difficulty)
extern BLOODCATPLACEMENT gBloodcatPlacements[256][4];
// Location of the Bloodcat Lair. There can be only one, as there's a quest that relies on its location. This is a
// sectorID, 0-255.
extern UINT8 gubBloodcatLairSectorId;

// HEADROCK HAM 3.6: Enumerated Bloodcat Placement types
enum
{
	BLOODCAT_PLACEMENT_AMBUSH = 0,
	BLOODCAT_PLACEMENT_STATIC,
	BLOODCAT_PLACEMENT_LAIR,
	NUM_BLOODCAT_PLACEMENT_TYPES
};

//Vehicle types
#define FOOT			0x01 //anywhere
#define CAR				0x02 //roads
#define TRUCK			0x04 //roads, plains, sparse
#define TRACKED			0x08 //roads, plains, sand, sparse
#define AIR				0x10 //can traverse all terrains at 100%

//Traversability ratings
enum
{
	TOWN,				//instant
	ROAD,				//everything travels at 100%
	PLAINS,				//foot 90%, truck 75%, tracked 100%
	SAND,				//foot 70%, tracked 60%
	SPARSE,				//foot 70%, truck 50%, tracked 60%
	DENSE,				//foot 50% 
	SWAMP,				//foot 20%
	WATER,				//foot 15%
	HILLS,				//foot 50%, truck 50%, tracked 50%
	GROUNDBARRIER,		//only air (super dense forest, ocean, etc.)
	NS_RIVER,			//river from north to south
	EW_RIVER,			//river from east to west
	EDGEOFWORLD,		//nobody can traverse.

	//NEW (not used for border values -- traversal calculations)
	TROPICS,
	FARMLAND,
	PLAINS_ROAD,
	SPARSE_ROAD,
	FARMLAND_ROAD,
	TROPICS_ROAD,
	DENSE_ROAD,
	COASTAL,
	HILLS_ROAD,
	COASTAL_ROAD,
	SAND_ROAD,
	SWAMP_ROAD,

	//only used for text purposes and not assigned to areas (SAM sites are hard coded throughout the code)
	SPARSE_SAM_SITE,		//D15 near Drassen
	SAND_SAM_SITE,			//I8 near Tixa
	TROPICS_SAM_SITE,		//D2 near Chitzena
	MEDUNA_SAM_SITE,		//N4 in Meduna
	CAMBRIA_HOSPITAL_SITE,
	DRASSEN_AIRPORT_SITE,
	MEDUNA_AIRPORT_SITE,
	SAM_SITE,
	REFUEL_SITE,

	REBEL_HIDEOUT,
	TIXA_DUNGEON,
	CREATURE_LAIR,
	ORTA_BASEMENT,
	TUNNEL,
	SHELTER,
	ABANDONED_MINE,

#ifdef JA2UB	
//Ja25: New
	FINAL_COMPLEX,
	GUARD_POST_TYPE,
	CRASH_SITE_TYPE,
	POWER_PLANT_TYPE,
	MOUNTAINS_TYPE,
	UNKNOWN_TYPE,
#endif

	NUM_TRAVTERRAIN_TYPES
};
extern UINT8 gszTerrain[NUM_TRAVTERRAIN_TYPES][15];

#define TRAVELRATING_NONE			0
#define TRAVELRATING_LOW			25
#define TRAVELRATING_NORMAL			50
#define TRAVELRATING_HIGH			75
#define TRAVELRATING_EXTREME		100

//Used by ubGarrisonID when a sector doesn't point to a garrison.	Used by strategic AI only.
#define NO_GARRISON					255

// Flugente: types of prisoners
typedef enum
{
	PRISONER_ADMIN = 0,
	PRISONER_REGULAR,
	PRISONER_ELITE,
	PRISONER_OFFICER,

	PRISONER_GENERAL,	// generals can be captured too, interogating them gives special rewards
	PRISONER_CIVILIAN,	// captured civilians (Kingpin, Hicks...)
	PRISONER_SECRET1,	// tbA
	PRISONER_SECRET2,	// tbA
	PRISONER_MAX,
} PrisonerType;

// -------- added by Flugente: sector info flags --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define SECTORINFO_VOLUNTEERS_RECENTLY_RECRUITED		0x01	//1				// we recruited volunteers here. Until this flag is removed, newly created civilians wont be potential volunteers anymore
#define SECTORINFO_ENEMYHELI_SHOTDOWN					0x02	//2				// an enemy helicopter was shot down here. The first time after that we enter this sector, there is a chance to find a downed pilot here

typedef struct SECTORINFO
{
	//information pertaining to this sector
	UINT32	uiFlags;					//various special conditions
	UINT8	ubInvestigativeState;		//When the sector is attacked by the player, the state increases by 1 permanently.
										//This value determines how quickly it is investigated by the enemy.
	UINT8	ubGarrisonID;				//IF the sector has an ID for this (non 255), then the queen values this sector and it
										//indexes the garrison group.
	INT8	ubPendingReinforcements;	//when the enemy owns this sector, this value will keep track of HIGH priority reinforcements -- not regular.
	BOOLEAN fMilitiaTrainingPaid;
	UINT8	ubMilitiaTrainingPercentDone;
	UINT8	ubMilitiaTrainingHundredths;
	//enemy military presence
	BOOLEAN	fPlayer[ 4 ];				//whether the player THINKS the sector is unde his control or not. array is for sublevels
	//enemy only info
	UINT8	ubNumTroops;				//the actual number of troops here.
	UINT8	ubNumElites;				//the actual number of elites here.
	UINT8	ubNumAdmins;				//the actual number of admins here.
	UINT8	ubNumCreatures;				//only set when immediately before ground attack made!
	UINT8	ubTroopsInBattle, ubElitesInBattle, ubAdminsInBattle, ubCreaturesInBattle;

	INT8	bLastKnownEnemies;			// -1 means never been there, no idea, otherwise it's what we'd observed most recently
										// while this is being maintained (partially, surely buggy), nothing uses it anymore. ARM

	UINT32	ubDayOfLastCreatureAttack;
	UINT32	uiFacilitiesFlags;			// the flags for various facilities

	UINT8	ubTraversability[5];		//determines the traversability ratings to adjacent sectors.
										//The last index represents the traversability if travelling
										//throught the sector without entering it.
	INT8	bNameId;
	INT8	bUSUSED;	
	INT8	bBloodCats;
	INT8	bBloodCatPlacements;
	INT8	UNUSEDbSAMCondition;

	UINT8	ubTravelRating;				//Represents how travelled a sector is.	Typically, the higher the travel rating,
										//the more people go near it.	A travel rating of 0 means there are never people
										//around.	This value is used for determining how often items would "vanish" from
										//a sector (nice theory, except it isn't being used that way.	Stealing is only in towns.	ARM)
	UINT8	ubNumberOfCivsAtLevel[ MAX_MILITIA_LEVELS ]; // town militia per experience class, 0/1/2 is GREEN/REGULAR/ELITE
	// HEADROCK HAM 3.6: Adding separate training percentage for MOBILES.
	UINT8	usFiller3;
	UINT8	usFiller1;
	BOOLEAN fFiller2;
	// Replacing these variables with the ones above. They really are unused.
	//UINT16	usUNUSEDMilitiaLevels;					// unused (ARM)
	//UINT8	ubUNUSEDNumberOfJoeBlowCivilians;		// unused (ARM)
	UINT32	uiTimeCurrentSectorWasLastLoaded;		//in game minutes, Specifies the last time the player was in the sector.
	UINT8	ubUNUSEDNumberOfEnemiesThoughtToBeHere;	// using bLastKnownEnemies instead
	UINT32	uiTimeLastPlayerLiberated;				//in game seconds (used to prevent the queen from attacking for awhile)

	BOOLEAN fSurfaceWasEverPlayerControlled;

	// HEADROCK HAM 3.6: Flag to determine whether enemy units in this sector can be seen, and whether their numbers
	// should be displayed. One filler was replaced to make room.
	UINT8	ubDetectionLevel;
	UINT8	usSectorInfoFlag;			// a flagmask
	UINT8	bFiller3;

	UINT32	uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer;
#ifdef JA2UB
	BOOLEAN	fValidSector; //ja25 UB
	BOOLEAN	fCustomSector;
	BOOLEAN	fCampaignSector;
#endif
	
	UINT8	uiNumberOfPrisonersOfWar[PRISONER_MAX];	
	UINT8	uiInterrogationHundredsLeft[PRISONER_MAX];

	UINT32	uiTimeAIArtillerywasOrdered;			// Flugente: updated every time an artillery strike is ordered from the militia
	
	UINT8	ubNumTanks;
	UINT8	ubTanksInBattle;

	// Flugente: disease
	UINT16	usNumCorpses;				// number of corpses in this sector
	FLOAT	fDiseasePoints;				// disease points
	UINT8	bFiller4;
	UINT8	usInfectionFlag;

	// Flugente: fortification
	FLOAT	dFortification_MaxPossible;	// the amount of fortification that can still be done in this sector, given the current layout plans. Is updated every time we unload a sector
	FLOAT	dFortification_UnappliedProgress;	// progress done via assignment work. As we cannot update unloaded sectors, update happens once sector is loaded

	// Flugente: enemy jeeps
	UINT8	ubNumJeeps;
	UINT8	ubJeepsInBattle;
	
	UINT8	usWorkers;
	UINT8	ubWorkerTrainingHundredths;

	// Flugente: localized weather
	UINT32	usWeather;

	// Flugente: burial assignment
	FLOAT	dBurial_UnappliedProgress;	// progress done via assignment work. This way no work gets lost

	// Flugente: number of turncoats
	UINT8	ubNumAdmins_Turncoat;
	UINT8	ubNumTroops_Turncoat;
	UINT8	ubNumElites_Turncoat;	
	UINT8	usExplorationProgress;

	UINT8	ubNumRobots;
	UINT8	ubRobotsInBattle;
	INT8	bPadding[ 6 ];

}SECTORINFO;

#define NO_ADJACENT_SECTOR		0x00
#define NORTH_ADJACENT_SECTOR	0x01
#define EAST_ADJACENT_SECTOR	0x02
#define SOUTH_ADJACENT_SECTOR	0x04
#define WEST_ADJACENT_SECTOR	0x08


typedef struct UNDERGROUND_SECTORINFO
{
	UINT32	uiFlags;
	UINT8	ubSectorX, ubSectorY, ubSectorZ;
	UINT8	ubNumElites, ubNumTroops, ubNumAdmins, ubNumCreatures;
	UINT8	fVisited;
	INT8	ubTravelRating;				//Represents how travelled a sector is.	Typically, the higher the travel rating,
										//the more people go near it.	A travel rating of 0 means there are never people
										//around.	This value is used for determining how often items would "vanish" from
										//a sector.
	UINT32	uiTimeCurrentSectorWasLastLoaded;		//Specifies the last time the player was in the sector
	struct	UNDERGROUND_SECTORINFO *next;

	UINT8	ubNumBloodcats;				// Bloodcat population
	UINT8	ubCreatureHabitat;			//determines how creatures live in this sector (see creature spreading.c)
	UINT8	ubElitesInBattle, ubTroopsInBattle, ubAdminsInBattle, ubCreaturesInBattle;

	// adding these (should not change struct layout due to padding)
	UINT8	ubMusicMode, ubUnsed;

	UINT32	uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer;
#ifdef JA2UB	
	BOOLEAN	fCustomSector;
	BOOLEAN	fCampaignSector;
#endif
	
	UINT8	uiNumberOfPrisonersOfWar[PRISONER_MAX];

	UINT8	ubNumTanks;
	UINT8	ubTanksInBattle;

	// Flugente: fortification
	FLOAT	dFortification_MaxPossible;	// the amount of fortification that can still be done in this sector, given the current layout plans. Is updated every time we unload a sector
	FLOAT	dFortification_UnappliedProgress;	// progress done via assignment work. As we cannot update unloaded sectors, update happens once sector is loaded

	UINT8	ubNumJeeps;
	UINT8	ubJeepsInBattle;
	UINT8	usExplorationProgress;
	UINT8	ubNumRobots;
	UINT8	ubRobotsInBattle;

	INT8	bPadding[10];
	//no padding left!
}UNDERGROUND_SECTORINFO;

//The sector information required for the strategic AI.	Contains the number of enemy troops,
//as well as intentions, etc.
//extern SECTORINFO SectorInfo[256];
extern std::vector<SECTORINFO> SectorInfo;

extern UNDERGROUND_SECTORINFO *gpUndergroundSectorInfoHead;

enum CreateMusic
{
	CM_COMPAT,
	CM_AUTO,
	CM_NEVER,
	CM_ALWAYS,
};

#endif