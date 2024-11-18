#ifndef __OVERHEAD_TYPES_H
#define __OVERHEAD_TYPES_H
// GLOBAL HEADER FOR DATA, TYPES FOR TACTICAL ENGINE

#include "Types.h"

#define REFINE_AIM_1					0
#define REFINE_AIM_MID1				1
#define REFINE_AIM_2					2
#define REFINE_AIM_MID2				3
#define REFINE_AIM_3					4
#define REFINE_AIM_MID3				5
#define REFINE_AIM_4					6
#define REFINE_AIM_MID4				7
#define REFINE_AIM_5					8
#define REFINE_AIM_BURST			-1	// SANDRO - I've changed this from 10 to -1, so when the code asks about how much we aim, it does not return this high value when we don't aim at all
										// hopefully, it will not cause any troubles anywhere .. it shouldn't 

#define AIM_SHOT_RANDOM				0
#define AIM_SHOT_HEAD					1
#define AIM_SHOT_TORSO				2
#define AIM_SHOT_LEGS					3
#define AIM_SHOT_GLAND				4

#define MIN_AMB_LEVEL_FOR_MERC_LIGHTS		9


#define		MAXTEAMS_SP				6 // WANNE: for singleplayer
#define     MAXTEAMS				11 //hayden
#define		MAXMERCS				MAX_NUM_SOLDIERS

//TACTICAL OVERHEAD STUFF
//#define			NO_SOLDIER			TOTAL_SOLDIERS // SAME AS NOBODY
//#define			NOBODY				NO_SOLDIER
//#define				NOBODY				TOTAL_SOLDIERS




// TACTICAL ENGINE STATUS FLAGS
#define			REALTIME						0x000000002
#define			TURNBASED						0x000000004
#define			IN_ENDGAME_SEQUENCE			 		0x000000008
#define			SHOW_ALL_ITEMS											0x000000010
#define			SHOW_AP_LEFT												0x000000020
#define			SHOW_ALL_MERCS											0x000000040
#define			TRANSLUCENCY_TYPE										0x000000080
#define			GODMODE															0x000000100
#define			DEMOMODE														0x000000200
#define			PLAYER_TEAM_DEAD										0x000000400
#define			NPC_TEAM_DEAD												0x000000800
#define			DISALLOW_SIGHT											0x000001000
#define			CHECK_SIGHT_AT_END_OF_ATTACK				0x000002000
#define	 IN_CREATURE_LAIR					0x000004000
#define			HIDE_TREES													0x000008000
#define			NOHIDE_REDUNDENCY										0x000010000
#define			DEBUGCLIFFS													0x000020000
#define			INCOMBAT														0x000040000
#define			WANT_MILITIA_REINFORCEMENTS										0x000080000
#define			ACTIVE															0x000100000
#define			SHOW_Z_BUFFER												0x000200000
#define			SLOW_ANIMATION											0x000400000
#define			ENGAGED_IN_CONV											0x000800000
#define			LOADING_SAVED_GAME									0x001000000
#define			OUR_MERCS_AUTO_MOVE									0x002000000
#define			SHOW_ALL_ROOFS											0x004000000
#define			NEWLY_ENTERED_SECTOR								0x008000000
#define			RED_ITEM_GLOW_ON										0x010000000				
#define			IGNORE_ENGAGED_IN_CONV_UI_UNLOCK		0x020000000
#define			IGNORE_ALL_OBSTACLES								0x040000000
#define			IN_DEIDRANNA_ENDGAME								0x080000000
#define			DONE_DEIDRANNA_ENDGAME							0x100000000
	
#define			OKBREATH						10
#define			OKLIFE							15
#define			CONSCIOUSNESS				10

// VIEWRANGE DEFINES
#define	 NORMAL_VIEW_RANGE					13
#define			MIN_RANGE_FOR_BLOWNAWAY		40

// MODIFIERS FOR AP COST FOR MOVEMENT 
#define RUNDIVISOR				1.8
#define WALKCOST					-1
#define SWATCOST					0
#define CRAWLCOST					1

// CHRISL: AP Cost Movement modifiers when wearing a backpack
#define RUNDIVISORBPACK				1.5
#define WALKCOSTBPACK					0
#define SWATCOSTBPACK					1
#define CRAWLCOSTBPACK					2

// defines
// ######################################################
#define MAX_PATH_LIST_SIZE		30
#define NUM_SOLDIER_SHADES		48
#define NUM_SOLDIER_EFFECTSHADES		2

// TIMER DELAYS
#define	DAMAGE_DISPLAY_DELAY		250
#define	FADE_DELAY							150
#define	FLASH_SELECTOR_DELAY		4000
#define	BLINK_SELECTOR_DELAY		250

#define PTR_OURTEAM	 (pSoldier->bTeam == gbPlayerNum)


#define DONTLOOK				0
#define LOOK						1


#define NOLOCATE		0
#define LOCATE			1

#define DONTSETLOCATOR									0
#define SETLOCATOR											1
#define SETANDREMOVEPREVIOUSLOCATOR			2
#define SETLOCATORFAST									3

#define NOCENTERING	 0


#define NOUPDATE		0
#define UPDATE			1


// ORDERS
enum
{
	STATIONARY = 0,		// moves max 1 sq., no matter what's going on
	ONGUARD,					// moves max 2 sqs. until alerted by something
	CLOSEPATROL,			// patrols within 5 spaces until alerted
	FARPATROL,				// patrols within 15 spaces
	POINTPATROL,			// patrols using patrolGrids
	ONCALL,						// helps buddies anywhere within the sector
	SEEKENEMY,				// not tied down to any one particular spot
	RNDPTPATROL,			// patrols randomly using patrolGrids
	SNIPER,					// snipes
	MAXORDERS
};

// ATTITUDES
enum
{
	DEFENSIVE = 0,
	BRAVESOLO,
	BRAVEAID,
	CUNNINGSOLO,
	CUNNINGAID,
	AGGRESSIVE,
	MAXATTITUDES,
	ATTACKSLAYONLY // special hyperaggressive vs Slay only value for Carmen the bounty hunter
};

// alert status types
enum
{
	STATUS_GREEN = 0,	// everything's OK, no suspicion
	STATUS_YELLOW,		// he or his friend heard something
	STATUS_RED,				// has definite evidence of opponent
	STATUS_BLACK,			// currently sees an active opponent
	NUM_STATUS_STATES
};

enum
{
	MORALE_HOPELESS = 0,
	MORALE_WORRIED,
	MORALE_NORMAL,
	MORALE_CONFIDENT,
	MORALE_FEARLESS,
	NUM_MORALE_STATES
};

// DEFINES FOR WEAPON HIT EVENT SPECIAL PARAM
#define		FIRE_WEAPON_NO_SPECIAL				0
#define		FIRE_WEAPON_BURST_SPECIAL			1
#define		FIRE_WEAPON_HEAD_EXPLODE_SPECIAL	2
#define		FIRE_WEAPON_CHEST_EXPLODE_SPECIAL	3
#define		FIRE_WEAPON_LEG_FALLDOWN_SPECIAL	4
#define		FIRE_WEAPON_HIT_BY_KNIFE_SPECIAL	5
#define		FIRE_WEAPON_SLEEP_DART_SPECIAL		6
#define		FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL 7
#define		FIRE_WEAPON_TOSSED_OBJECT_SPECIAL   8
#define		FIRE_WEAPON_BLINDED					9
#define		FIRE_WEAPON_DEAFENED				10
#define		FIRE_WEAPON_BLINDED_AND_DEAFENED	11
#define		FIRE_WEAPON_BLINDED_SPECIAL			12	// Flugente: works like FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL but without the damage dependency
#define		FIRE_WEAPON_VEHICLE_TRAUMA			13


#define NO_INTERRUPTS			0
#define ALLOW_INTERRUPTS		1


#define SIGHT_LOOK		0x1
//#define SIGHT_SEND		0x2	// no longer needed using LOCAL OPPLISTs
#define SIGHT_RADIO	 0x4
#define SIGHT_INTERRUPT 0x8
#define SIGHT_ALL		0xF


// CHANGE THIS VALUE TO AFFECT TOTAL SIGHT RANGE
//#define STRAIGHT_RANGE gGameExternalOptions.ubStraightSightRange

// CHANGE THESE VALUES TO ADJUST VARIOUS FOV ANGLES
#define STRAIGHT_RATIO	1 //* 1.5
#define ANGLE_RATIO			0.857 //* 1.5
#define SIDE_RATIO			0.571 //* 1.5
// CJC: Changed SBEHIND_RATIO (side-behind ratio) to be 0 to make stealth attacks easier
// Changed on September 21, 1998
//#define SBEHIND_RATIO		0.142
#define SBEHIND_RATIO		0
#define BEHIND_RATIO		0

// looking distance defines
//#define BEHIND			(INT8)( BEHIND_RATIO * STRAIGHT_RANGE )
//#define SBEHIND		 (INT8)( SBEHIND_RATIO * STRAIGHT_RANGE )
//#define SIDE			(INT8)( SIDE_RATIO * STRAIGHT_RANGE )
//#define ANGLE			(INT8)( ANGLE_RATIO * STRAIGHT_RANGE )
//#define STRAIGHT		(INT8)( STRAIGHT_RATIO * STRAIGHT_RANGE )
 

// opplist value constants
#define HEARD_3_TURNS_AGO		-4
#define HEARD_2_TURNS_AGO		-3
#define HEARD_LAST_TURN		 -2
#define HEARD_THIS_TURN		 -1
#define NOT_HEARD_OR_SEEN		0
#define SEEN_CURRENTLY			1
#define SEEN_THIS_TURN			2
#define SEEN_LAST_TURN			3
#define SEEN_2_TURNS_AGO		 4
#define SEEN_3_TURNS_AGO		 5

#define OLDEST_SEEN_VALUE			SEEN_3_TURNS_AGO
#define OLDEST_HEARD_VALUE	 HEARD_3_TURNS_AGO

#define UNDER_FIRE							2
#define UNDER_FIRE_LAST_TURN		1


#define		MAX_DISTANCE_FOR_PROXIMITY_SIGHT		15		




// DEFINES FOR BODY TYPE SUBSTITUTIONS
#define			SUB_ANIM_BIGGUYSHOOT2							0x00000001
#define			SUB_ANIM_BIGGUYTHREATENSTANCE			0x00000002


// Enumerate directions
enum WorldDirections
{
	NORTH = 0,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST,
	NUM_WORLD_DIRECTIONS,
	DIRECTION_IRRELEVANT,
	DIRECTION_EXITGRID = 255,
};


// Starting Sector
// HEADROCK HAM 3.5: Externalized.
//UINT8 startingX = 9;
//UINT8 startingY = 1;
const int startingZ = 0;


// ENUMERATION OF SOLDIER POSIITONS IN GLOBAL SOLDIER LIST
// WDS - make number of mercenaries, etc. be configurable
#define CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS 254
extern UINT16	giMAXIMUM_NUMBER_OF_PLAYER_MERCS;
#define CODE_MAXIMUM_NUMBER_OF_PLAYER_VEHICLES 6
extern UINT16	giMAXIMUM_NUMBER_OF_PLAYER_VEHICLES;
#define CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS+CODE_MAXIMUM_NUMBER_OF_PLAYER_VEHICLES
extern UINT16	giMAXIMUM_NUMBER_OF_PLAYER_SLOTS;
#define CODE_MAXIMUM_NUMBER_OF_ENEMIES 256
extern UINT16	giMAXIMUM_NUMBER_OF_ENEMIES;
#define CODE_MAXIMUM_NUMBER_OF_CREATURES 256
extern UINT16	giMAXIMUM_NUMBER_OF_CREATURES;
#define CODE_MAXIMUM_NUMBER_OF_REBELS 256
extern UINT16	giMAXIMUM_NUMBER_OF_REBELS;
#define CODE_MAXIMUM_NUMBER_OF_CIVS 256
extern UINT16	giMAXIMUM_NUMBER_OF_CIVS;

// The following should be the largest from the above set of constants.
//   Note: Is there any way to compute this via the preprocessor?
#define LARGEST_NUMBER_IN_ANY_GROUP 260

#define	MAX_NUM_SOLDIERS		(CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS+CODE_MAXIMUM_NUMBER_OF_PLAYER_VEHICLES+CODE_MAXIMUM_NUMBER_OF_ENEMIES+CODE_MAXIMUM_NUMBER_OF_CREATURES+CODE_MAXIMUM_NUMBER_OF_REBELS+CODE_MAXIMUM_NUMBER_OF_CIVS)
#define	NUM_PLANNING_MERCS	8
//#define	TOTAL_SOLDIERS			( NUM_PLANNING_MERCS + MAX_NUM_SOLDIERS )
#define	TOTAL_SOLDIERS			(MAX_NUM_SOLDIERS )

// If there are more than (one less the size of the field used to store IDs (currently a char)) then fail
#if TOTAL_SOLDIERS > 2048
#error Too many!
#endif
#if CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS > LARGEST_NUMBER_IN_ANY_GROUP
#error _LARGEST_NUMBER_IN_ANY_GROUP is too small
#endif
#if CODE_MAXIMUM_NUMBER_OF_ENEMIES > LARGEST_NUMBER_IN_ANY_GROUP
#error _LARGEST_NUMBER_IN_ANY_GROUP is too small
#endif
#if CODE_MAXIMUM_NUMBER_OF_CREATURES > LARGEST_NUMBER_IN_ANY_GROUP
#error _LARGEST_NUMBER_IN_ANY_GROUP is too small
#endif
#if CODE_MAXIMUM_NUMBER_OF_REBELS > LARGEST_NUMBER_IN_ANY_GROUP
#error _LARGEST_NUMBER_IN_ANY_GROUP is too small
#endif
#if CODE_MAXIMUM_NUMBER_OF_CIVS > LARGEST_NUMBER_IN_ANY_GROUP
#error _LARGEST_NUMBER_IN_ANY_GROUP is too small
#endif

// DEFINE TEAMS
#define OUR_TEAM				0
#define ENEMY_TEAM			1
#define CREATURE_TEAM		2
#define MILITIA_TEAM		3
#define CIV_TEAM				4
#define LAST_TEAM				LAN_TEAM_FOUR
#define	PLAYER_PLAN			5

#define LAN_TEAM_ONE 6
#define LAN_TEAM_TWO 7
#define LAN_TEAM_THREE 8
#define LAN_TEAM_FOUR 9
//hayden

// boxing state
enum BoxingStates
{
	NOT_BOXING = 0,
	BOXING_WAITING_FOR_PLAYER,
	PRE_BOXING,
	BOXING,
	DISQUALIFIED,
	WON_ROUND,
	LOST_ROUND
} ;

// PALETTE SUBSITUTION TYPES
typedef struct 
{
	UINT8		ubStart;
	UINT8		ubEnd;

} PaletteSubRangeType;

typedef CHAR8 PaletteRepID[ 30 ];

typedef struct 
{
	UINT8					ubType;
	PaletteRepID				ID;
	UINT8					ubPaletteSize;
	UINT8					*r;
	UINT8					*g;
	UINT8					*b;

} PaletteReplacementType;

// MACROS
// This will set an animation ID
#define			SET_PALETTEREP_ID( a, b )				( strcpy( a, b ) )
																						// strcmp returns 0 if true!		
#define	 COMPARE_PALETTEREP_ID( a, b )		( strcmp( a, b ) ? FALSE : TRUE )


struct SOLDIERTYPE;
extern SOLDIERTYPE *MercPtrs[TOTAL_SOLDIERS];

typedef struct SoldierID
{
	UINT16 i;

	// Implicit conversion and constructor to provide compatibility with unchanged code
	// TODO: Remove once SoldierID is used everywhere and these are no longer needed
	inline operator UINT16() const { return i; }

	SoldierID(const UINT16 val = TOTAL_SOLDIERS) : i(val)
	{
		// Limit the maximum value to TOTAL_SOLDIERS. Anything beyond that is invalid
		if ( i > TOTAL_SOLDIERS )
			i = TOTAL_SOLDIERS;
	}
	SoldierID ( const UINT32 val ) : i ( val )
	{
		// Limit the maximum value to TOTAL_SOLDIERS. Anything beyond that is invalid
		if ( i > TOTAL_SOLDIERS )
			i = TOTAL_SOLDIERS;
	}
	SoldierID ( const INT32 val ) : i ( val )
	{
		// Limit the maximum value to TOTAL_SOLDIERS. Anything beyond that is invalid
		if ( i > TOTAL_SOLDIERS )
			i = TOTAL_SOLDIERS;
	}

	// No conversions from 8-bit integers!
	SoldierID ( const UINT8 ) = delete;
	SoldierID ( const INT8 ) = delete;

	
	SOLDIERTYPE* operator->() { return MercPtrs[i]; }
	const SOLDIERTYPE* operator->() const { return MercPtrs[i]; }
	inline operator SOLDIERTYPE* () { return MercPtrs[i]; }
	inline operator const SOLDIERTYPE*() const { return MercPtrs[i]; }
} SoldierID;

inline bool operator==(const SoldierID lhs, const SoldierID rhs) { return lhs.i == rhs.i; }
inline bool operator==(const SoldierID lhs, const int rhs) { return lhs.i == rhs; }
inline bool operator==(const SoldierID lhs, const UINT32 rhs) { return lhs.i == rhs; }
inline bool operator==(const SoldierID lhs, const UINT16 rhs) { return lhs.i == rhs; }
inline bool operator==(const SoldierID lhs, const INT16 rhs) { return lhs.i == rhs; }
inline bool operator==(const int lhs, const SoldierID rhs) { return lhs == rhs.i; }
inline bool operator==(const UINT16 lhs, const SoldierID rhs) { return lhs == rhs.i; }
inline bool operator==(const INT16 lhs, const SoldierID rhs) { return lhs == rhs.i; }

inline bool operator!=(const SoldierID lhs, const SoldierID rhs) { return lhs.i != rhs.i; }
inline bool operator!=(const SoldierID lhs, const UINT32 rhs) { return lhs.i != rhs; }
inline bool operator!=(const SoldierID lhs, const UINT16 rhs) { return lhs.i != rhs; }
inline bool operator!=(const int lhs, const SoldierID rhs) { return lhs != rhs.i; }
inline bool operator!=(const UINT32 lhs, const SoldierID rhs) { return lhs != rhs.i; }
inline bool operator!=(const UINT16 lhs, const SoldierID rhs) { return lhs != rhs.i; }
inline bool operator!=(const INT16 lhs, const SoldierID rhs) { return lhs != rhs.i; }

inline bool operator<(const SoldierID lhs, const SoldierID rhs) { return lhs.i < rhs.i; }
inline bool operator<(const SoldierID lhs, const int rhs) { return lhs.i < rhs; }
inline bool operator<(const SoldierID lhs, UINT16 rhs) { return lhs.i < rhs; }
inline bool operator<(const int lhs, const SoldierID rhs) { return lhs < rhs.i; }
inline bool operator<(const unsigned int lhs, const SoldierID rhs) { return lhs < rhs.i; }
inline bool operator<(const UINT16 lhs, const SoldierID rhs) { return lhs < rhs.i; }

inline bool operator>(const SoldierID lhs, const SoldierID rhs) { return lhs.i > rhs.i; }
inline bool operator>(const SoldierID lhs, const int rhs) { return lhs.i > rhs; }
inline bool operator>(const int lhs, const SoldierID rhs) { return lhs > rhs.i; }
inline bool operator>(const UINT16 lhs, const SoldierID rhs) { return lhs > rhs.i; }

inline bool operator<=(const SoldierID lhs, const SoldierID rhs) { return lhs.i <= rhs.i; }
inline bool operator<=(const SoldierID lhs, const int rhs) { return lhs.i <= rhs; }
inline bool operator<=(const SoldierID lhs, const UINT16 rhs) { return lhs.i <= rhs; }
inline bool operator<=(const int lhs, const SoldierID rhs) { return lhs <= rhs.i; }
inline bool operator<=(const UINT32 lhs, const SoldierID rhs) { return lhs <= rhs.i; }
inline bool operator<=(const UINT16 lhs, const SoldierID rhs) { return lhs <= rhs.i; }
inline bool operator<=(const INT16 lhs, const SoldierID rhs) { return lhs <= rhs.i; }

inline bool operator>=(const SoldierID lhs, const SoldierID rhs) { return lhs.i >= rhs.i; }
inline bool operator>=(const SoldierID lhs, const int rhs) { return lhs.i >= rhs; }
inline bool operator>=(const SoldierID lhs, const UINT16 rhs) { return lhs.i >= rhs; }
inline bool operator>=(const int lhs, const SoldierID rhs) { return lhs >= rhs.i; }
inline bool operator>=(const unsigned int lhs, const SoldierID rhs) { return lhs >= rhs.i; }
inline bool operator>=(const UINT16 lhs, const SoldierID rhs) { return lhs >= rhs.i; }

inline SoldierID operator-(const SoldierID lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs.i - rhs.i) }; }
inline SoldierID operator-(const SoldierID lhs, const int rhs) { return SoldierID{ static_cast<UINT16>(lhs.i - rhs) }; }
inline SoldierID operator-(const SoldierID lhs, const unsigned int rhs) { return SoldierID{ static_cast<UINT16>(lhs.i - rhs) }; }
inline SoldierID operator-(const SoldierID lhs, const UINT16 rhs) { return SoldierID{ static_cast<UINT16>(lhs.i - rhs) }; }
inline SoldierID operator-(const int lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs - rhs.i) }; }
inline SoldierID operator-(const unsigned int lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs - rhs.i) }; }

inline SoldierID operator+(const SoldierID lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs.i + rhs.i) }; }
inline SoldierID operator+(const SoldierID lhs, const int rhs) { return SoldierID{ static_cast<UINT16>(lhs.i + rhs) }; }
inline SoldierID operator+(const SoldierID lhs, const unsigned int rhs) { return SoldierID{ static_cast<UINT16>(lhs.i + rhs) }; }
inline SoldierID operator+(const SoldierID lhs, const UINT16 rhs) { return SoldierID{ static_cast<UINT16>(lhs.i + rhs) }; }
inline SoldierID operator+(const unsigned int lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs + rhs.i) }; }
inline SoldierID operator+(const INT16 lhs, const SoldierID rhs) { return SoldierID{ static_cast<UINT16>(lhs + rhs.i) }; }

inline SoldierID operator--(SoldierID lhs)
{
	lhs.i -= 1;
	return lhs;
}

inline SoldierID operator++(SoldierID lhs)
{
	lhs.i += 1;
	return lhs;
}

//TODO: Change this to constexpr after SoldierID is ready and the user defined constructor from uint16 is not needed anymore.
inline const SoldierID NOBODY{ TOTAL_SOLDIERS };
#endif
