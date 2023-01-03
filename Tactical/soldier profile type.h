#ifndef __SOLDER_PROFILE_TYPE_H
#define __SOLDER_PROFILE_TYPE_H

#include "overhead types.h"
#include "Soldier Control.h"
#include "mapscreen.h"

#define NUM_PROFILES		255 //170 new profiles by Jazz
#define NUM_PROFILES_v111	170
//tais: maximum amount of starting gear kits
#define NUM_MERCSTARTINGGEAR_KITS	5

#ifdef JA2UB
#define FIRST_RPC 60	//JA25 was 59
#else
#define FIRST_RPC 57
#endif

#define FIRST_NPC 75

#define NAME_LENGTH			30
#define NICKNAME_LENGTH	10


//ONLY HAVE 8 MISC FLAGS.. SHOULD BE ENOUGH
#define	PROFILE_MISC_FLAG_RECRUITED									0x01
#define	PROFILE_MISC_FLAG_HAVESEENCREATURE							0x02
#define	PROFILE_MISC_FLAG_FORCENPCQUOTE								0x04
#define PROFILE_MISC_FLAG_WOUNDEDBYPLAYER							0x08
#define	PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS				0x10
#define PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE						0x20
#define	PROFILE_MISC_FLAG_EPCACTIVE									0x40
#define	PROFILE_MISC_FLAG_ALREADY_USED_ITEMS						0x80				//The player has already purchased the mercs items.

#define PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR						0x01
#define PROFILE_MISC_FLAG2_LEFT_COUNTRY								0x02
#define PROFILE_MISC_FLAG2_BANDAGED_TODAY							0x04
#define PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE						0x08
#define PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE				0x10
#define PROFILE_MISC_FLAG2_MARRIED_TO_HICKS							0x20
#define PROFILE_MISC_FLAG2_ASKED_BY_HICKS							0x40
//JMich_MMG: Flag to see if gearkit cost is still unpaid for
#define	PROFILE_MISC_FLAG2_MERC_GEARKIT_UNPAID						0x80

#define	PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM			0x01		// In the aimscreen, the merc was away and the player left a message
#define PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE					0x02
#define PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE				0x04		// player's had a chance to hire this merc
#define PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL					0x08

#define PROFILE_MISC_FLAG3_NPC_PISSED_OFF							0x10
#define	PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID			0x20		// In the merc site, the merc has died and Speck quote for the dead merc has been said

#define PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH				0x40
#define PROFILE_MISC_FLAG3_GOODGUY									0x80
//
// The following variables are used with the 'bMercStatus' variable
//
//

//Merc is ready
#define MERC_OK																0

//if the merc doesnt have a EDT file
#define MERC_HAS_NO_TEXT_FILE									-1

//used in the aim video conferencing screen
#define	MERC_ANNOYED_BUT_CAN_STILL_CONTACT		-2
#define	MERC_ANNOYED_WONT_CONTACT							-3
#define	MERC_HIRED_BUT_NOT_ARRIVED_YET				-4

//self explanatory
#define	MERC_IS_DEAD													-5

//set when the merc is returning home.	A delay for 1,2 or 3 days
#define	MERC_RETURNING_HOME										-6

// used when merc starts game on assignment, goes on assignment later, or leaves to go on another contract
#define	MERC_WORKING_ELSEWHERE								-7

//When the merc was fired, they were a POW, make sure they dont show up in AIM, or MERC as available
#define	MERC_FIRED_AS_A_POW										-8


// the values for categories of stats
#define SUPER_STAT_VALUE 80
#define NEEDS_TRAINING_STAT_VALUE 50
#define NO_CHANCE_IN_HELL_STAT_VALUE 40

#define SUPER_SKILL_VALUE 80
#define NEEDS_TRAINING_SKILL_VALUE 50
#define NO_CHANCE_IN_HELL_SKILL_VALUE 0

//
// WDS 05/13/2009
// This is a temporary fix and should be REALLY fixed someday.
// The bMercOpinion array in MERCPROFILESTRUCT below only contains 75 entries.  That
// means profile ID#s of 75 and beyond are not valid to be used for checking morale.
//
// Flugente 16/04/2013: Done
#define NUMBER_OF_OPINIONS_OLD	75		// for old structures, we need to keep this value. Do NOT change, or you'll be in a world of pain. I warned you.
#define NUMBER_OF_OPINIONS		255	

	
inline bool OKToCheckOpinion(int profileNumber) {
	return (profileNumber < NUMBER_OF_OPINIONS);
}

// SANDRO - replaced this list to represent the old traits only
typedef enum
{
	NO_SKILLTRAIT_OT = 0,
	LOCKPICKING_OT,
	HANDTOHAND_OT,
	ELECTRONICS_OT,
	NIGHTOPS_OT,
	THROWING_OT,
	TEACHING_OT,
	HEAVY_WEAPS_OT,
	AUTO_WEAPS_OT,
	STEALTHY_OT,
	AMBIDEXT_OT,
	THIEF_OT,
	MARTIALARTS_OT,
	KNIFING_OT,
	PROF_SNIPER_OT,
	CAMOUFLAGED_OT,
	// SANDRO - I dared to remove these for it is really useless,
	//   with the camo removing/repainting feature you only need the basic camouflage trait anyway
	//CAMOUFLAGED_URBAN,
	//CAMOUFLAGED_DESERT,
	//CAMOUFLAGED_SNOW,
	EXPERT,
	NUM_SKILLTRAITS_OT
} SkillTrait;

//////////////////////////////////////////////////////////////////////
// SANDRO - the list for new traits
typedef enum
{
	NO_SKILLTRAIT_NT = 0,
	// MAJOR TRAITS
	AUTO_WEAPONS_NT, // 1
	HEAVY_WEAPONS_NT, // 2
	SNIPER_NT, // 3
	RANGER_NT, // 4
	GUNSLINGER_NT, // 5
	MARTIAL_ARTS_NT, // 6
	SQUADLEADER_NT, // 7
	TECHNICIAN_NT, // 8
	DOCTOR_NT, // 9
	// MINOR TRAITS
	AMBIDEXTROUS_NT, // 10
	MELEE_NT, // 11
	THROWING_NT, // 12
	NIGHT_OPS_NT, // 13
	STEALTHY_NT, // 14
	ATHLETICS_NT, // 15
	BODYBUILDING_NT, // 16
	DEMOLITIONS_NT, // 17
	TEACHING_NT, // 18
	SCOUTING_NT, // 19

	// Flugente: new traits have to go here, even if they are major traits, as otherwise the existing traits in profiles get mixed up
	COVERT_NT, // 20	// a major trait
	RADIO_OPERATOR_NT, //21		// a minor trait
	SNITCH_NT, //22		// a minor trait 
	SURVIVAL_NT, //23	// a minor trait

	NUM_SKILLTRAITS_NT
} SkillTraitNew;

#define NUM_MAJOR_TRAITS 10
#define NUM_MINOR_TRAITS 13

#define NUM_ORIGINAL_MAJOR_TRAITS 9

// Flugente: I've had it with this hardcoding madness. Without this, adding or removing a new trait would crash anything related to a bubblehelp display of traits
// always check every use of these enums and every use of the skill-strings if you add a new trait
#define NEWTRAIT_MERCSKILL_EXPERTOFFSET	(NUM_MAJOR_TRAITS + NUM_MINOR_TRAITS)
#define NEWTRAIT_MERCSKILL_OFFSET_ALL	(NEWTRAIT_MERCSKILL_EXPERTOFFSET + NUM_MAJOR_TRAITS)

// Flugente: these aren't really traits, but it is convenient to pretend so
#define INTEL	(2 * NEWTRAIT_MERCSKILL_EXPERTOFFSET + 2)
#define DISGUISE		(INTEL + 1)
#define VARIOUSSKILLS	(DISGUISE + 1)
// sevenfm: add autobandage to skills menu
#define AUTOBANDAGESKILLS (VARIOUSSKILLS + 1)

// SANDRO - new set of character traits
typedef enum
{
	CHAR_TRAIT_NORMAL = 0,
	CHAR_TRAIT_SOCIABLE,
	CHAR_TRAIT_LONER,
	CHAR_TRAIT_OPTIMIST,
	CHAR_TRAIT_ASSERTIVE,
	CHAR_TRAIT_INTELLECTUAL,
	CHAR_TRAIT_PRIMITIVE,
	CHAR_TRAIT_AGGRESSIVE,
	CHAR_TRAIT_PHLEGMATIC,
	CHAR_TRAIT_DAUNTLESS,
	CHAR_TRAIT_PACIFIST,
	CHAR_TRAIT_MALICIOUS,
	CHAR_TRAIT_SHOWOFF,
	CHAR_TRAIT_COWARD,
	NUM_CHAR_TRAITS
} CharacterTraits;

// SANDRO - appearances
typedef enum
{
	APPEARANCE_AVERAGE = 0,
	APPEARANCE_UGLY,
	APPEARANCE_HOMELY,
	APPEARANCE_ATTRACTIVE,
	APPEARANCE_BABE,
	NUM_APPEARANCES
} Appearances;

// Flugente - care levels
typedef enum
{
	CARELEVEL_NONE = 0,
	CARELEVEL_SOME = 1,
	CARELEVEL_EXTREME = 2,

	NUM_CARELEVELS
} CareLevels;

// Flugente - refinements
typedef enum
{
	REFINEMENT_AVERAGE = 0,
	REFINEMENT_SLOB = 1,
	REFINEMENT_SNOB = 2,

	NUM_REFINEMENT
} Refinements;

// Flugente - nationalities
typedef enum
{
	// old Sirtech values
	AMERICAN_NAT = 0,		// take that, Murica! Sirtech knew you have no value :-)
	ARAB_NAT = 1,
	AUSTRALIAN_NAT = 2,
	BRITISH_NAT = 3,
	CANADIAN_NAT = 4,
	CUBAN_NAT = 5,
	DANISH_NAT = 6,
	FRENCH_NAT = 7,
	RUSSIAN_NAT = 8,
	NIGERIAN_NAT = 9,		// this was simply not defined in original data (I did not find any country), but we can't have any holes in here, so just invented soemthing
	SWISS_NAT = 10,
	JAMAICAN_NAT = 11,
	POLISH_NAT = 12,
	CHINESE_NAT = 13,
	IRISH_NAT = 14,
	SOUTH_AFRICAN_NAT = 15,
	HUNGARIAN_NAT = 16,
	SCOTTISH_NAT = 17,
	ARULCAN_NAT = 18,
	GERMAN_NAT = 19,
	AFRICAN_NAT = 20,
	ITALIAN_NAT = 21,
	DUTCH_NAT = 22,
	ROMANIAN_NAT = 23,
	METAVIRAN_NAT = 24,

	// new values, because why not
	GREEK_NAT,		// 25
	ESTONIAN_NAT,
	VENEZUELAN_NAT ,
	JAPANESE_NAT,
	TURKISH_NAT,
	INDIAN_NAT,		// 30
	MEXICAN_NAT,
	NORWEGIAN_NAT,
	SPANISH_NAT,
	BRASILIAN_NAT,
	FINNISH_NAT,	// 35
	IRANIAN_NAT,
	ISRAELI_NAT,
	BULGARIAN_NAT,
	SWEDISH_NAT,
	IRAQI_NAT,		// 40
	SYRIAN_NAT,
	BELGIAN_NAT,
	PORTOGUESE_NAT,
	BELARUSIAN_NAT,
	SERBIAN_NAT,	// 45
	PAKISTANI_NAT,
	ALBANIAN_NAT,
	ARGENTINIAN_NAT,
	ARMENIAN_NAT,
	AZERBAIJANI_NAT, // 50
	BOLIVIAN_NAT,
	CHILEAN_NAT,
	CIRCASSIAN_NAT,
	COLUMBIAN_NAT,
	EGYPTIAN_NAT, // 55
	ETHIOPIAN_NAT,
	GEORGIAN_NAT,
	JORDANIAN_NAT,
	KAZAKHSTANI_NAT,
	KENYAN_NAT, // 60
	KOREAN_NAT,
	KYRGYZSTANI_NAT,
	MONGOLIAN_NAT,
	PALESTINIAN_NAT,
	PANAMANIAN_NAT, // 65
	RHODESIAN_NAT,
	SALVADORAN_NAT,
	SAUDI_NAT,
	SOMALI_NAT,
	THAI_NAT, // 70
	UKRAINIAN_NAT,
	UZBEKISTANI_NAT,
	WELSH_NAT,
	YAZIDI_NAT,
	ZIMBABWEAN_NAT, // 75

	NUM_NATIONALITIES
} Nationalities;

// Flugente - races
typedef enum
{
	WHITE = 0,
	BLACK = 1,
	ASIAN = 2,
	ESKIMO = 3,
	HISPANIC = 4,

	NUM_RACES
} Races;

// Flugente - racist levels
typedef enum
{
	RACIST_NONE = 0,
	RACIST_SOME = 1,
	RACIST_VERY = 2,

	NUM_RACIST
} RacistLevels;

// Flugente: dynamic opinons: number of profile flagmasks
#define OPINION_FLAGMASKS_NUMBER	6

//////////////////////////////////////////////////////////////////////

typedef enum
{
	NO_DISABILITY = 0,
	HEAT_INTOLERANT,
	NERVOUS,
	CLAUSTROPHOBIC,
	NONSWIMMER,
	FEAR_OF_INSECTS,
	FORGETFUL,
	PSYCHO,
	// Flugente: more disabilities
	DEAF,
	SHORTSIGHTED,
	HEMOPHILIAC,
	AFRAID_OF_HEIGHTS,
	SELF_HARM,

	NUM_DISABILITIES
} PersonalityTrait;

#define NERVOUS_RADIUS 10

typedef enum
{
	ATT_NORMAL = 0,
	ATT_FRIENDLY,
	ATT_LONER,
	ATT_OPTIMIST,
	ATT_PESSIMIST,
	ATT_AGGRESSIVE,
	ATT_ARROGANT,
	ATT_BIG_SHOT,
	ATT_ASSHOLE,
	ATT_COWARD,
	NUM_ATTITUDES
} Attitudes;

typedef enum
{
	MALE = 0,
	FEMALE
} Sexes;

typedef enum
{
	NOT_SEXIST = 0,
	SOMEWHAT_SEXIST,
	VERY_SEXIST,
	GENTLEMAN,
	NUM_SEXIST
} SexistLevels;



// training defines for evolution, no stat increase, stat decrease( de-evolve )
typedef enum
{
	NORMAL_EVOLUTION =0,
	NO_EVOLUTION,
	DEVOLVE,
	THREEQUARTER_EVOLUTION,
	HALF_EVOLUTION,
	ONEQUARTER_EVOLUTION,
} CharacterEvolution;

#define BUDDY_MERC( prof, bud ) ((prof)->bBuddy[0] == (bud) || (prof)->bBuddy[1] == (bud) || (prof)->bBuddy[2] == (bud) || (prof)->bBuddy[3] == (bud) || (prof)->bBuddy[4] == (bud) || ( (prof)->bLearnToLike == (bud) && (prof)->bLearnToLikeCount == 0 ) )
#define HATED_MERC( prof, hat ) ((prof)->bHated[0] == (hat) || (prof)->bHated[1] == (hat) || (prof)->bHated[2] == (hat) || (prof)->bHated[3] == (hat) || (prof)->bHated[4] == (hat) || ( (prof)->bLearnToHate == (hat) && (prof)->bLearnToHateCount == 0 ) )

#define BUDDY_OPINION +25
#define HATED_OPINION -25

///////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - Let's add a nice new struct for records, as there are just too many of them 
class STRUCT_Records 
{
public:

	UINT16	usKillsElites;
	UINT16	usKillsRegulars;
	UINT16	usKillsAdmins;
	UINT16  usKillsHostiles;
	UINT16	usKillsCreatures;
	UINT16  usKillsZombies;
	UINT16	usKillsTanks;
	UINT16	usKillsOthers;

	UINT16	usAssistsMercs;
	UINT16	usAssistsMilitia;
	UINT16	usAssistsOthers;

	UINT16	usShotsFired;
	UINT16  usMissilesLaunched;
	UINT16  usGrenadesThrown;
	UINT16  usKnivesThrown;
	UINT16  usBladeAttacks;
	UINT16  usHtHAttacks;
	UINT16	usShotsHit;

	UINT16	usBattlesTactical;
	UINT16	usBattlesAutoresolve;
	UINT16	usBattlesRetreated;
	UINT16  usAmbushesExperienced;
	UINT16	usLargestBattleFought;

	UINT16	usTimesWoundedShot;
	UINT16	usTimesWoundedStabbed;
	UINT16	usTimesWoundedPunched;
	UINT16	usTimesWoundedBlasted;
	UINT16	usTimesStatDamaged;
	UINT16	usTimesSurgeryUndergoed;
	UINT16	usFacilityAccidents;
	
	UINT16	usLocksPicked;
	UINT16	usLocksBreached;
	UINT16	usTrapsRemoved;
	UINT16	usExpDetonated;
	UINT16	usItemsRepaired;
	UINT16	usItemsCombined;
	UINT16  usItemsStolen;
	UINT16	usMercsBandaged;
	UINT16	usSurgeriesMade;
	UINT16  usMilitiaTrained;
	UINT16	usNPCsDiscovered;
	UINT16	usSectorsDiscovered;
	UINT8   ubQuestsHandled;
	
	UINT16  usInterrogations;
	UINT32  usDamageTaken;
	UINT32  usDamageDealt;
	UINT16  usTimesInfected;
	UINT16	zFiller7;
	UINT32  usPointsHealed;		// we store the healed hundreth, because we can

}; // RECORDS
//////////////////////////////////////////////////////////////////////////////

#define mNameLength 80
#define mGearKitNameLength 80

// CHRISL: Class to handle profile equipment from XML_MercStartingGear
class MERCPROFILEGEAR {
public:
	// Constructor
	MERCPROFILEGEAR();
	// Assignment operator
	MERCPROFILEGEAR& operator=(const MERCPROFILEGEAR&);
	// Copy constructor
	MERCPROFILEGEAR(const MERCPROFILEGEAR& src);
	// Destructor
	~MERCPROFILEGEAR();

	// Initialize the mercenary profile gear
	void initialize();

	//  Note that the constructor does this automatically.
	void clearInventory();
public:
	UINT16		mIndex;
	INT16		PriceModifier;
	INT16		AbsolutePrice;
	CHAR16		mGearKitName[mGearKitNameLength];
	CHAR8		mName[mNameLength];
	char		endOfPOD;	// marker for end of POD (plain old data)
	std::vector<int>	inv;
	std::vector<int>	iStatus;
	std::vector<int>	iDrop;
	std::vector<int>	iNumber;
	std::vector<int>	lbe;
	std::vector<int>	lStatus;
private:
	int			invCnt;
	int			lbeCnt;
};
#define SIZEOF_MERCPROFILEGEAR_POD offsetof( MERCPROFILEGEAR, endOfPOD )

class OLD_MERCPROFILESTRUCT_101
{
public:
	// Constructor
	OLD_MERCPROFILESTRUCT_101();
	CHAR16	zName[ NAME_LENGTH ];
	CHAR16	zNickname[ NICKNAME_LENGTH ];
	UINT32	uiAttnSound;
	UINT32	uiCurseSound;
	UINT32	uiDieSound;
	UINT32	uiGoodSound;
	UINT32	uiGruntSound;
	UINT32	uiGrunt2Sound;
	UINT32	uiOkSound;
	UINT8		ubFaceIndex;
	PaletteRepID		PANTS;
	PaletteRepID		VEST;
	PaletteRepID		SKIN;
	PaletteRepID		HAIR;
	INT8		bSex;
	INT8		bArmourAttractiveness;
	UINT8		ubMiscFlags2;
	INT8		bEvolution;
	UINT8		ubMiscFlags;
	UINT8		bSexist;
	UINT8		bLearnToHate;

	// skills
	INT8		bStealRate;
	INT8		bVocalVolume;
	UINT8		ubQuoteRecord;
	INT8		bDeathRate;
	INT8		bScientific;

	INT16		sExpLevelGain;
	INT16		sLifeGain;
	INT16		sAgilityGain;
	INT16		sDexterityGain;
	INT16		sWisdomGain;
	INT16		sMarksmanshipGain;
	INT16		sMedicalGain;
	INT16		sMechanicGain;
	INT16		sExplosivesGain;

	UINT8		ubBodyType;
	INT8		bMedical;

	UINT16	usEyesX;
	UINT16	usEyesY;
	UINT16	usMouthX;
	UINT16	usMouthY;
	UINT32	uiEyeDelay;
	UINT32	uiMouthDelay;
	UINT32	uiBlinkFrequency;
	UINT32	uiExpressionFrequency;
	UINT16	sSectorX;
	UINT16	sSectorY;

	UINT32	uiDayBecomesAvailable;			//day the merc will be available.	used with the bMercStatus

	INT8		bStrength;

	INT8		bLifeMax;
	INT8		bExpLevelDelta;
	INT8		bLifeDelta;
	INT8		bAgilityDelta;
	INT8		bDexterityDelta;
	INT8		bWisdomDelta;
	INT8		bMarksmanshipDelta;
	INT8		bMedicalDelta;
	INT8		bMechanicDelta;
	INT8		bExplosivesDelta;
	INT8	bStrengthDelta;
	INT8	bLeadershipDelta;
	UINT16	usKills;
	UINT16	usAssists;
	UINT16	usShotsFired;
	UINT16	usShotsHit;
	UINT16	usBattlesFought;
	UINT16	usTimesWounded;
	UINT16	usTotalDaysServed;

	INT16		sLeadershipGain;
	INT16		sStrengthGain;



	// BODY TYPE SUBSITUTIONS
	UINT32	uiBodyTypeSubFlags;

	INT16	sSalary;
	INT8	bLife;
	INT8		bDexterity;		// dexterity (hand coord) value
	INT8	bDisability;
	INT8	bSkillTrait;

	INT8	bReputationTolerance;
	INT8	bExplosive;
	INT8	bSkillTrait2;
	INT8	bLeadership;

	INT8	bBuddy[5];
	INT8	bHated[5];
	INT8	bExpLevel;		// general experience level

	INT8	bMarksmanship;
	UINT8	bMinService;
	INT8	bWisdom;
	UINT8	bResigned;
	UINT8	bActive;

	UINT8	DO_NOT_USE_bInvStatus[OldInventory::NUM_INV_SLOTS];
	UINT8	DO_NOT_USE_bInvNumber[OldInventory::NUM_INV_SLOTS];
	UINT16 usApproachFactor[4];

	INT8	bMainGunAttractiveness;
	INT8		bAgility;			// agility (speed) value

	BOOLEAN	fUseProfileInsertionInfo;				// Set to various flags, ( contained in TacticalSave.h )
	INT16		sGridNo;												// The Gridno the NPC was in before leaving the sector
	UINT8		ubQuoteActionID;
	INT8		bMechanical;

	UINT8	ubInvUndroppable;
	UINT8	ubRoomRangeStart[2];
	UINT16 DO_NOT_USE_inv[OldInventory::NUM_INV_SLOTS];
	INT8 bMercTownReputation[ 20 ];

	UINT16 usStatChangeChances[ 12 ];		// used strictly for balancing, never shown!
	UINT16 usStatChangeSuccesses[ 12 ];	// used strictly for balancing, never shown!

	UINT8	ubStrategicInsertionCode;

	UINT8	ubRoomRangeEnd[2];

	INT8 bPadding[ 4 ];

	UINT8 ubLastQuoteSaid;
	
	INT8 bRace;
	INT8 bNationality;
	INT8 bAppearance;
	INT8 bAppearanceCareLevel;
	INT8 bRefinement;
	INT8 bRefinementCareLevel;
	INT8 bHatedNationality;
	INT8 bHatedNationalityCareLevel;
	INT8 bRacist;
	UINT32 uiWeeklySalary;
	UINT32 uiBiWeeklySalary;
	INT8 bMedicalDeposit;
	INT8 bAttitude;
	INT8 bBaseMorale;
	UINT16 sMedicalDepositAmount;
	
	UINT8 bLearnToLike;
	UINT8 ubApproachVal[4];
	UINT8 ubApproachMod[3][4];
	INT8 bTown;
	INT8 bTownAttachment;
	UINT16 usOptionalGearCost;
	
	INT8 bMercOpinion[NUMBER_OF_OPINIONS_OLD];	// Flugente: old value has to be kept for compatibility. Do not change, or you'll be in a world of pain

	INT8 bApproached;
	INT8 bMercStatus;								//The status of the merc.	If negative, see flags at the top of this file.	Positive:	The number of days the merc is away for.	0:	Not hired but ready to be.
	INT8 bHatedTime[5];
	INT8 bLearnToLikeTime;
	INT8 bLearnToHateTime;
	INT8 bHatedCount[5];
	INT8 bLearnToLikeCount;
	INT8 bLearnToHateCount;
	UINT8 ubLastDateSpokenTo;
	UINT8 bLastQuoteSaidWasSpecial;
	INT8	bSectorZ;
	UINT16 usStrategicInsertionData;
	INT8 bFriendlyOrDirectDefaultResponseUsedRecently;
	INT8 bRecruitDefaultResponseUsedRecently;
	INT8 bThreatenDefaultResponseUsedRecently;
	INT8 bNPCData;			// NPC specific
	INT32	iBalance;
	INT16 sTrueSalary; // for use when the person is working for us for free but has a positive salary value
	UINT8	ubCivilianGroup;
	UINT8	ubNeedForSleep;
	UINT32	uiMoney;
	INT8	bNPCData2;		// NPC specific

	UINT8	ubMiscFlags3;

	UINT8 ubDaysOfMoraleHangover;		// used only when merc leaves team while having poor morale
	UINT8	ubNumTimesDrugUseInLifetime;		// The # times a drug has been used in the player's lifetime...

	// Flags used for the precedent to repeating oneself in Contract negotiations.	Used for quote 80 -	~107.	Gets reset every day
	UINT32	uiPrecedentQuoteSaid;
	UINT32	uiProfileChecksum;
	INT16		sPreCombatGridNo;
	UINT8		ubTimeTillNextHatedComplaint;
	UINT8		ubSuspiciousDeath;

	INT32	iMercMercContractLength;		//Used for MERC mercs, specifies how many days the merc has gone since last page

	UINT32	uiTotalCostToDate;			// The total amount of money that has been paid to the merc for their salary
	UINT8		ubBuffer[4];

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//

	char endOfPOD;	// marker for end of POD (plain old data)
};
#define SIZEOF_OLD_MERCPROFILESTRUCT_101_POD offsetof( OLD_MERCPROFILESTRUCT_101, endOfPOD )

class MERCPROFILESTRUCT {
public:
	// Constructor
	MERCPROFILESTRUCT();
	// Copy Constructor
	MERCPROFILESTRUCT(const MERCPROFILESTRUCT&);
	// Conversion operator
	MERCPROFILESTRUCT& operator=(const OLD_MERCPROFILESTRUCT_101& src);
	// Assignment operator
	MERCPROFILESTRUCT& operator=(const MERCPROFILESTRUCT&);
	// Destructor
	~MERCPROFILESTRUCT();

	void	CopyOldInventoryToNew(const OLD_MERCPROFILESTRUCT_101& src);
	// Changed by ADB, rev 1513
	//BOOLEAN Load(HWFILE hFile, bool forceLoadOldVersion);
	BOOLEAN Load(HWFILE hFile, bool forceLoadOldVersion, bool forceLoadOldEncryption, bool wasSavedWithEncryption);
	BOOLEAN Save(HWFILE hFile);
	UINT32	GetChecksum();

	// Initialize the mercenary profile.	
	//	Use this instead of the old method of calling memset.
	//	Note that the constructor does this automatically.
	void initialize();

	// Clear out the mercenary profile's invetory
	//	Use this instead of the old method of calling memset.
	//	Note that the constructor does this automatically.
	void clearInventory();

	// Note: Place all non-POD items at the end (after endOfPOD)
	// The format of this structure affects what is written into and read from various
	// files (maps, save files, etc.).	If you change it then that code will not work 
	// properly until it is all fixed and the files updated.
public:
	CHAR16	zName[ NAME_LENGTH ];
	CHAR16	zNickname[ NICKNAME_LENGTH ];
	UINT32	uiAttnSound;
	UINT32	uiCurseSound;
	UINT32	uiDieSound;
	UINT32	uiGoodSound;
	UINT32	uiGruntSound;
	UINT32	uiGrunt2Sound;
	UINT32	uiOkSound;
	UINT8		ubFaceIndex;
	PaletteRepID		PANTS;
	PaletteRepID		VEST;
	PaletteRepID		SKIN;
	PaletteRepID		HAIR;
	INT8		bSex;
	INT8		bArmourAttractiveness;
	UINT8		ubMiscFlags2;
	INT8		bEvolution;
	UINT8		ubMiscFlags;
	UINT8		bSexist;
	UINT8		bLearnToHate;

	// skills
	INT8		bStealRate;
	INT8		bVocalVolume;
	UINT8		ubQuoteRecord;
	INT8		bDeathRate;
	INT8		bScientific;

	INT16		sExpLevelGain;
	INT16		sLifeGain;
	INT16		sAgilityGain;
	INT16		sDexterityGain;
	INT16		sWisdomGain;
	INT16		sMarksmanshipGain;
	INT16		sMedicalGain;
	INT16		sMechanicGain;
	INT16		sExplosivesGain;

	UINT8		ubBodyType;
	INT8		bMedical;

	UINT16	usEyesX;
	UINT16	usEyesY;
	UINT16	usMouthX;
	UINT16	usMouthY;
	UINT32	uiEyeDelay;
	UINT32	uiMouthDelay;
	UINT32	uiBlinkFrequency;
	UINT32	uiExpressionFrequency;
	UINT16	sSectorX;
	UINT16	sSectorY;

	UINT32	uiDayBecomesAvailable;			//day the merc will be available.	used with the bMercStatus

	INT8		bStrength;

	INT8		bLifeMax;
	INT8		bExpLevelDelta;
	INT8		bLifeDelta;
	INT8		bAgilityDelta;
	INT8		bDexterityDelta;
	INT8		bWisdomDelta;
	INT8		bMarksmanshipDelta;
	INT8		bMedicalDelta;
	INT8		bMechanicDelta;
	INT8		bExplosivesDelta;
	INT8	bStrengthDelta;
	INT8	bLeadershipDelta;
	/////////////////////////////////////////////////////////////
	// SANDRO - converted to it's own structure
	//UINT16	usKills;
	//UINT16	usAssists;
	//UINT16	usShotsFired;
	//UINT16	usShotsHit;
	//UINT16	usBattlesFought;
	//UINT16	usTimesWounded;
	/////////////////////////////////////////////////////////////

	UINT16	usTotalDaysServed;

	INT16		sLeadershipGain;
	INT16		sStrengthGain;



	// BODY TYPE SUBSITUTIONS
	UINT32	uiBodyTypeSubFlags;

	INT16	sSalary;
	INT8	bLife;
	INT8		bDexterity;		// dexterity (hand coord) value
	INT8	bDisability;
	INT8	bSkillTraits[30];

	INT8	bReputationTolerance;
	INT8	bExplosive;
	//INT8	bSkillTrait2;
	//INT8	bSkillTrait3; // added by SANDRO
	INT8	bLeadership;

	UINT8	bBuddy[5];
	UINT8	bHated[5];
	INT8	bExpLevel;		// general experience level

	INT8	bMarksmanship;
	UINT8	bMinService;
	INT8	bWisdom;
	UINT8	bResigned;
	UINT8	bActive;

	UINT16 usApproachFactor[4];

	INT8	bMainGunAttractiveness;
	INT8		bAgility;			// agility (speed) value

	BOOLEAN	fUseProfileInsertionInfo;				// Set to various flags, ( contained in TacticalSave.h )
//	INT16		sGridNo;												// The Gridno the NPC was in before leaving the sector
	INT16 _old_sGridNo;				// WANNE - BMP: DONE! (Structure)						// The Gridno the NPC was in before leaving the sector
	UINT8		ubQuoteActionID;
	INT8		bMechanical;

	UINT8	ubInvUndroppable;
	//DBrot: More Rooms
	//UINT8	ubRoomRangeStart[2];
	UINT16 usRoomRangeStart[2];
	INT8 bMercTownReputation[ 20 ];

	UINT16 usStatChangeChances[ 12 ];		// used strictly for balancing, never shown!
	UINT16 usStatChangeSuccesses[ 12 ];	// used strictly for balancing, never shown!

	UINT8	ubStrategicInsertionCode;

	//UINT8	ubRoomRangeEnd[2];
	UINT16 usRoomRangeEnd[2];

	UINT8 ubLastQuoteSaid;
	
	INT8 bRace;
	INT8 bNationality;
	INT8 bAppearance;
	INT8 bAppearanceCareLevel;
	INT8 bRefinement;
	INT8 bRefinementCareLevel;
	INT8 bHatedNationality;
	INT8 bHatedNationalityCareLevel;
	INT8 bRacist;
	UINT32 uiWeeklySalary;
	UINT32 uiBiWeeklySalary;
	INT8 bMedicalDeposit;
	INT8 bAttitude;
	INT8 bCharacterTrait; // added by SANDRO
	INT8 bBaseMorale;
	UINT16 sMedicalDepositAmount;
	
	UINT8 bLearnToLike;
	UINT8 ubApproachVal[4];
	UINT8 ubApproachMod[3][4];
	INT8 bTown;
	INT8 bTownAttachment;
	UINT16 usOptionalGearCost;
	INT8 bMercOpinion[NUMBER_OF_OPINIONS];
	INT8 bApproached;
	INT8 bMercStatus;								//The status of the merc.	If negative, see flags at the top of this file.	Positive:	The number of days the merc is away for.	0:	Not hired but ready to be.
	INT8 bHatedTime[5];
	INT8 bLearnToLikeTime;
	INT8 bLearnToHateTime;
	INT8 bHatedCount[5];
	INT8 bLearnToLikeCount;
	INT8 bLearnToHateCount;
	UINT8 ubLastDateSpokenTo;
	UINT8 bLastQuoteSaidWasSpecial;
	INT8	bSectorZ;
	UINT16 usStrategicInsertionData;
	INT8 bFriendlyOrDirectDefaultResponseUsedRecently;
	INT8 bRecruitDefaultResponseUsedRecently;
	INT8 bThreatenDefaultResponseUsedRecently;
	INT8 bNPCData;			// NPC specific
	INT32	iBalance;
	INT16 sTrueSalary; // for use when the person is working for us for free but has a positive salary value
	UINT8	ubCivilianGroup;
	UINT8	ubNeedForSleep;
	UINT32	uiMoney;
	INT8	bNPCData2;		// NPC specific

	UINT8	ubMiscFlags3;

	UINT8 ubDaysOfMoraleHangover;		// used only when merc leaves team while having poor morale
	UINT8	ubNumTimesDrugUseInLifetime;		// The # times a drug has been used in the player's lifetime...

	// Flags used for the precedent to repeating oneself in Contract negotiations.	Used for quote 80 -	~107.	Gets reset every day
	UINT32	uiPrecedentQuoteSaid;
	UINT32	uiProfileChecksum;
//	INT16		sPreCombatGridNo;
	INT16		_old_sPreCombatGridNo;		// WANNE - BMP: DONE! (Structure)
	UINT8		ubTimeTillNextHatedComplaint;
	UINT8		ubSuspiciousDeath;

	INT32	iMercMercContractLength;		//Used for MERC mercs, specifies how many days the merc has gone since last page

	UINT32	uiTotalCostToDate;			// The total amount of money that has been paid to the merc for their salary

	//SB: extended fields
	INT32	sGridNo;			// WANNE - BMP: DONE! (Structure)
	INT32	sPreCombatGridNo;	// WANNE - BMO: DONE! (Structure)
	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//

	char endOfPOD;	// marker for end of POD (plain old data)

	std::vector<int>	inv;
	std::vector<int>	bInvStatus;
	std::vector<int>	bInvNumber;

	// SANDRO - merc records
	STRUCT_Records		records;

	// Flugente: background
	UINT16 usBackground;

	// anv: for snitches, remember if he was exposed and by how many people (could be used for covert purposes too)
	//UINT8	ubExposedInSector[MAP_WORLD_X * MAP_WORLD_Y];

	// anv: simplified ubExposedInSector, hours until snitch can go undercover after exposition
	UINT8 ubSnitchExposedCooldown;

	// Flugente: flagmasks for dynamic opinions on each and every other person. Yes, this is significant in size
	UINT32 usDynamicOpinionFlagmask[NUM_PROFILES][OPINION_FLAGMASKS_NUMBER];

	INT8	sDynamicOpinionLongTerm[NUM_PROFILES];

	// Flugente: voice set used
	UINT32 usVoiceIndex;

	// Flugente: type of profile
	UINT32 Type;
}; // MERCPROFILESTRUCT;

// WANNE - BMP: DONE!
class _OLD_MERCPROFILESTRUCT {
public:
	CHAR16	zName[ NAME_LENGTH ];
	CHAR16	zNickname[ NICKNAME_LENGTH ];
	UINT32	uiAttnSound;
	UINT32	uiCurseSound;
	UINT32	uiDieSound;
	UINT32	uiGoodSound;
	UINT32	uiGruntSound;
	UINT32	uiGrunt2Sound;
	UINT32	uiOkSound;
	UINT8		ubFaceIndex;
	PaletteRepID		PANTS;
	PaletteRepID		VEST;
	PaletteRepID		SKIN;
	PaletteRepID		HAIR;
	INT8		bSex;
	INT8		bArmourAttractiveness;
	UINT8		ubMiscFlags2;
	INT8		bEvolution;
	UINT8		ubMiscFlags;
	UINT8		bSexist;
	INT8		bLearnToHate;

	// skills
	INT8		bStealRate;
	INT8		bVocalVolume;
	UINT8		ubQuoteRecord;
	INT8		bDeathRate;
	INT8		bScientific;

	INT16		sExpLevelGain;
	INT16		sLifeGain;
	INT16		sAgilityGain;
	INT16		sDexterityGain;
	INT16		sWisdomGain;
	INT16		sMarksmanshipGain;
	INT16		sMedicalGain;
	INT16		sMechanicGain;
	INT16		sExplosivesGain;

	UINT8		ubBodyType;
	INT8		bMedical;

	UINT16	usEyesX;
	UINT16	usEyesY;
	UINT16	usMouthX;
	UINT16	usMouthY;
	UINT32	uiEyeDelay;
	UINT32	uiMouthDelay;
	UINT32	uiBlinkFrequency;
	UINT32	uiExpressionFrequency;
	UINT16	sSectorX;
	UINT16	sSectorY;

	UINT32	uiDayBecomesAvailable;			//day the merc will be available.	used with the bMercStatus

	INT8		bStrength;

	INT8		bLifeMax;
	INT8		bExpLevelDelta;
	INT8		bLifeDelta;
	INT8		bAgilityDelta;
	INT8		bDexterityDelta;
	INT8		bWisdomDelta;
	INT8		bMarksmanshipDelta;
	INT8		bMedicalDelta;
	INT8		bMechanicDelta;
	INT8		bExplosivesDelta;
	INT8	bStrengthDelta;
	INT8	bLeadershipDelta;
	UINT16	usKills;
	UINT16	usAssists;
	UINT16	usShotsFired;
	UINT16	usShotsHit;
	UINT16	usBattlesFought;
	UINT16	usTimesWounded;
	UINT16	usTotalDaysServed;

	INT16		sLeadershipGain;
	INT16		sStrengthGain;



	// BODY TYPE SUBSITUTIONS
	UINT32	uiBodyTypeSubFlags;

	INT16	sSalary;
	INT8	bLife;
	INT8		bDexterity;		// dexterity (hand coord) value
	INT8	bDisability;
	INT8	bSkillTrait;

	INT8	bReputationTolerance;
	INT8	bExplosive;
	INT8	bSkillTrait2;
	INT8	bLeadership;

	INT8	bBuddy[5];
	INT8	bHated[5];
	INT8	bExpLevel;		// general experience level

	INT8	bMarksmanship;
	UINT8	bMinService;
	INT8	bWisdom;
	UINT8	bResigned;
	UINT8	bActive;

	UINT16 usApproachFactor[4];

	INT8	bMainGunAttractiveness;
	INT8		bAgility;			// agility (speed) value

	BOOLEAN	fUseProfileInsertionInfo;				// Set to various flags, ( contained in TacticalSave.h )
	INT16		sGridNo;												// The Gridno the NPC was in before leaving the sector
	UINT8		ubQuoteActionID;
	INT8		bMechanical;

	UINT8	ubInvUndroppable;
	UINT8	ubRoomRangeStart[2];
	INT8 bMercTownReputation[ 20 ];

	UINT16 usStatChangeChances[ 12 ];		// used strictly for balancing, never shown!
	UINT16 usStatChangeSuccesses[ 12 ];	// used strictly for balancing, never shown!

	UINT8	ubStrategicInsertionCode;

	UINT8	ubRoomRangeEnd[2];

	UINT8 ubLastQuoteSaid;
	
	INT8 bRace;
	INT8 bNationality;
	INT8 bAppearance;
	INT8 bAppearanceCareLevel;
	INT8 bRefinement;
	INT8 bRefinementCareLevel;
	INT8 bHatedNationality;
	INT8 bHatedNationalityCareLevel;
	INT8 bRacist;
	UINT32 uiWeeklySalary;
	UINT32 uiBiWeeklySalary;
	INT8 bMedicalDeposit;
	INT8 bAttitude;
	INT8 bBaseMorale;
	UINT16 sMedicalDepositAmount;
	
	INT8 bLearnToLike;
	UINT8 ubApproachVal[4];
	UINT8 ubApproachMod[3][4];
	INT8 bTown;
	INT8 bTownAttachment;
	UINT16 usOptionalGearCost;
	
	INT8 bMercOpinion[NUMBER_OF_OPINIONS_OLD];	// Flugente: old value has to be kept for compatibility. Do not change, or you'll be in a world of pain

	INT8 bApproached;
	INT8 bMercStatus;								//The status of the merc.	If negative, see flags at the top of this file.	Positive:	The number of days the merc is away for.	0:	Not hired but ready to be.
	INT8 bHatedTime[5];
	INT8 bLearnToLikeTime;
	INT8 bLearnToHateTime;
	INT8 bHatedCount[5];
	INT8 bLearnToLikeCount;
	INT8 bLearnToHateCount;
	UINT8 ubLastDateSpokenTo;
	UINT8 bLastQuoteSaidWasSpecial;
	INT8	bSectorZ;
	UINT16 usStrategicInsertionData;
	INT8 bFriendlyOrDirectDefaultResponseUsedRecently;
	INT8 bRecruitDefaultResponseUsedRecently;
	INT8 bThreatenDefaultResponseUsedRecently;
	INT8 bNPCData;			// NPC specific
	INT32	iBalance;
	INT16 sTrueSalary; // for use when the person is working for us for free but has a positive salary value
	UINT8	ubCivilianGroup;
	UINT8	ubNeedForSleep;
	UINT32	uiMoney;
	INT8	bNPCData2;		// NPC specific

	UINT8	ubMiscFlags3;

	UINT8 ubDaysOfMoraleHangover;		// used only when merc leaves team while having poor morale
	UINT8	ubNumTimesDrugUseInLifetime;		// The # times a drug has been used in the player's lifetime...

	// Flags used for the precedent to repeating oneself in Contract negotiations.	Used for quote 80 -	~107.	Gets reset every day
	UINT32	uiPrecedentQuoteSaid;
	UINT32	uiProfileChecksum;
	INT16		sPreCombatGridNo;
	UINT8		ubTimeTillNextHatedComplaint;
	UINT8		ubSuspiciousDeath;

	INT32	iMercMercContractLength;		//Used for MERC mercs, specifies how many days the merc has gone since last page

	UINT32	uiTotalCostToDate;			// The total amount of money that has been paid to the merc for their salary

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//

	char endOfPOD;	// marker for end of POD (plain old data)

	std::vector<int>	inv;
	std::vector<int>	bInvStatus;
	std::vector<int>	bInvNumber;
}; // _OLD_MERCPROFILESTRUCT;

#define SIZEOF_MERCPROFILESTRUCT_POD offsetof( MERCPROFILESTRUCT, endOfPOD )
#define _OLD_SIZEOF_MERCPROFILESTRUCT_POD offsetof( _OLD_MERCPROFILESTRUCT, endOfPOD )

#define TIME_BETWEEN_HATED_COMPLAINTS 24

#define SUSPICIOUS_DEATH				1
#define VERY_SUSPICIOUS_DEATH		2


#ifdef WFconvertUB

typedef struct
{
	UINT16	zName[ NAME_LENGTH ];
	UINT16	zNickname[ NICKNAME_LENGTH ];
	UINT32	uiAttnSound;
	UINT32	uiCurseSound;
	UINT32	uiDieSound;
	UINT32	uiGoodSound;
	UINT32	uiGruntSound;
	UINT32	uiGrunt2Sound;
	UINT32  uiOkSound;
	UINT8		ubFaceIndex;
	PaletteRepID		PANTS;
	PaletteRepID		VEST;
	PaletteRepID		SKIN;
	PaletteRepID		HAIR;
	INT8		bSex;
	INT8		bArmourAttractiveness;
	UINT8		ubMiscFlags2;
	INT8		bEvolution;
	UINT8		ubMiscFlags;
	UINT8		bSexist;
	INT8		bLearnToHate;

	// skills
	INT8		bStealRate;
	INT8		bVocalVolume;
	UINT8		ubQuoteRecord;
	INT8		bDeathRate;
	INT8		bScientific;

	INT16		sExpLevelGain;
	INT16		sLifeGain;
	INT16		sAgilityGain;
	INT16		sDexterityGain;
	INT16		sWisdomGain;
	INT16		sMarksmanshipGain;
	INT16		sMedicalGain;
	INT16		sMechanicGain;
	INT16		sExplosivesGain;

	UINT8		ubBodyType;
	INT8		bMedical;

	UINT16	usEyesX;
	UINT16	usEyesY;
	UINT16	usMouthX;
	UINT16	usMouthY;
	UINT32	uiEyeDelay;
	UINT32	uiMouthDelay;
	UINT32	uiBlinkFrequency;
	UINT32	uiExpressionFrequency;
	UINT16	sSectorX;
	UINT16	sSectorY;

	UINT32	uiDayBecomesAvailable;			//day the merc will be available.  used with the bMercStatus

	INT8		bStrength;

	INT8		bLifeMax;
	INT8		bExpLevelDelta;
	INT8		bLifeDelta;
	INT8		bAgilityDelta;
	INT8		bDexterityDelta;
	INT8		bWisdomDelta;
	INT8		bMarksmanshipDelta;
	INT8		bMedicalDelta;
	INT8		bMechanicDelta;
	INT8		bExplosivesDelta;
	INT8    bStrengthDelta;
	INT8    bLeadershipDelta;
	UINT16  usKills;
	UINT16  usAssists;
	UINT16  usShotsFired;
	UINT16  usShotsHit;
	UINT16  usBattlesFought;
	UINT16  usTimesWounded;
	UINT16  usTotalDaysServed;

	INT16		sLeadershipGain;
	INT16		sStrengthGain;



	// BODY TYPE SUBSITUTIONS
	UINT32	uiBodyTypeSubFlags;

	INT16	sSalary;
	INT8	bLife;
	INT8		bDexterity;		// dexterity (hand coord) value
	INT8	bPersonalityTrait;
	INT8	bSkillTrait;

	INT8	bReputationTolerance;
	INT8	bExplosive;
	INT8	bSkillTrait2;
	INT8	bLeadership;

	INT8	bBuddy[5];
	INT8	bHated[5];
	INT8	bExpLevel;		// general experience level

	INT8	bMarksmanship;
	UINT8	bMinService;
	INT8	bWisdom;
	UINT8	bResigned;
	UINT8	bActive;

	UINT8	bInvStatus[19];
	UINT8 bInvNumber[19];
	UINT16 usApproachFactor[4];

	INT8	bMainGunAttractiveness;
	INT8		bAgility;			// agility (speed) value

	BOOLEAN	fUseProfileInsertionInfo;				// Set to various flags, ( contained in TacticalSave.h )
	INT16		sGridNo;												// The Gridno the NPC was in before leaving the sector
	UINT8		ubQuoteActionID;
	INT8		bMechanical;

	UINT8	ubInvUndroppable;
	UINT8	ubRoomRangeStart[2];
	UINT16 inv[19];
	INT8 bMercTownReputation[ 20 ];

	UINT16 usStatChangeChances[ 12 ];		// used strictly for balancing, never shown!
	UINT16 usStatChangeSuccesses[ 12 ];	// used strictly for balancing, never shown!

	UINT8	ubStrategicInsertionCode;

	UINT8	ubRoomRangeEnd[2];

	INT8 bPadding[ 4 ];

	UINT8 ubLastQuoteSaid;

	INT8 bRace;
	INT8 bNationality;
	INT8 bAppearance;
	INT8 bAppearanceCareLevel;
	INT8 bRefinement;
	INT8 bRefinementCareLevel;
	INT8 bHatedNationality;
	INT8 bHatedNationalityCareLevel;
	INT8 bRacist;
	UINT32 uiWeeklySalary;
	UINT32 uiBiWeeklySalary;
	INT8 bMedicalDeposit;
	INT8 bAttitude;
	INT8 bBaseMorale;
	UINT16 sMedicalDepositAmount;

	INT8 bLearnToLike;
	UINT8 ubApproachVal[4];
	UINT8 ubApproachMod[3][4];
	INT8 bTown;
	INT8 bTownAttachment;
	UINT16 usOptionalGearCost;
	INT8 bMercOpinion[NUMBER_OF_OPINIONS];
	INT8 bApproached;
	INT8 bMercStatus;								//The status of the merc.  If negative, see flags at the top of this file.  Positive:  The number of days the merc is away for.  0:  Not hired but ready to be.
	INT8 bHatedTime[5];
	INT8 bLearnToLikeTime;
	INT8 bLearnToHateTime;
	INT8 bHatedCount[5];
	INT8 bLearnToLikeCount;
	INT8 bLearnToHateCount;
	UINT8 ubLastDateSpokenTo;
	UINT8 bLastQuoteSaidWasSpecial;
	INT8	bSectorZ;
	UINT16 usStrategicInsertionData;
	INT8 bFriendlyOrDirectDefaultResponseUsedRecently;
	INT8 bRecruitDefaultResponseUsedRecently;
	INT8 bThreatenDefaultResponseUsedRecently;
	INT8 bNPCData;			// NPC specific
	INT32	iBalance;
	INT16 sTrueSalary; // for use when the person is working for us for free but has a positive salary value
	UINT8	ubCivilianGroup;
	UINT8	ubNeedForSleep;
	UINT32	uiMoney;
	INT8	bNPCData2;		// NPC specific

	UINT8	ubMiscFlags3;

	UINT8 ubDaysOfMoraleHangover;		// used only when merc leaves team while having poor morale
	UINT8	ubNumTimesDrugUseInLifetime;		// The # times a drug has been used in the player's lifetime...

	// Flags used for the precedent to repeating oneself in Contract negotiations.  Used for quote 80 -  ~107.  Gets reset every day
	UINT32	uiPrecedentQuoteSaid;
	UINT32	uiProfileChecksum;
	INT16		sPreCombatGridNo;
	UINT8		ubTimeTillNextHatedComplaint;
	UINT8		ubSuspiciousDeath;

	INT32	iMercMercContractLength;		//Used for MERC mercs, specifies how many days the merc has gone since last page

	UINT32	uiTotalCostToDate;			// The total amount of money that has been paid to the merc for their salary

	UINT16	usTotalKills;						// total kills the player has had ( only used if the player imported the mercs from ja2 )
	UINT16	usTotalAssists;					// total assists the player has had ( only used if the player imported the mercs from ja2 )
} MERCPROFILESTRUCT_OLD_WF;

#endif

#endif
