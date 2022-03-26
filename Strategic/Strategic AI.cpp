#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "types.h"
	#include "Strategic Movement.h"
	#include "Queen Command.h"
	#include "Strategic AI.h"
	#include "Game Event Hook.h"
	#include "GameSettings.h"
	#include "Campaign Types.h"
	#include "Random.h"
	#include "Strategic Pathing.h"
	#include "message.h"
	#include "strategicmap.h"
	#include "Game Clock.h"
	#include "strategic.h"
	#include "Explosion Control.h"
	#include "Town Militia.h"
	#include "Overhead.h"
	#include "Squads.h"
	#include "jascreens.h"
	#include "screenids.h"
	#include "MessageBoxScreen.h"
	#include "Quests.h"
	#include "Player Command.h"
	#include "Campaign.h"
	#include "Campaign Init.h"
	#include "Game Init.h"
	#include "PreBattle Interface.h"
	#include "Quests.h"
	#include "Soldier Profile.h"
	#include "Scheduling.h"
	#include "Map Information.h"
	#include "interface dialogue.h"
	#include "ASD.h"		// added by Flugente
#endif

#include "GameInitOptionsScreen.h"

#define SAI_VERSION		29

/*
STRATEGIC AI -- UNDERLYING PHILOSOPHY
The most fundamental part of the strategic AI which takes from reality and gives to gameplay is the manner
the queen attempts to take her towns back.	Finances and owning mines are the most important way
to win the game.	As the player takes more mines over, the queen will focus more on quality and defense.	In
the beginning of the game, she will focus more on offense than mid-game or end-game.

REALITY
The queen owns the entire country, and the player starts the game with a small lump of cash, enough to hire
some mercenaries for about a week.	In that week, the queen may not notice what is going on, and the player
would believably take over one of the towns before she could feasibly react.	As soon as her military was
aware of the situation, she would likely proceed to send 300-400 troops to annihilate the opposition, and the
game would be over relatively quickly.	If the player was a prodigy, and managed to hold the town against such
a major assault, he would probably lose in the long run being forced into a defensive position and running out
of money quickly while the queen could continue to pump out the troops.	On the other hand, if the player
somehow managed to take over most of the mines, he would be able to casually walk over the queen eventually
just from the sheer income allowing him to purchase several of the best mercs.	That would have the effect of
making the game impossibly difficult in the beginning of the game, and a joke at the end (this is very much
like Master Of Orion II on the more difficult settings )

GAMEPLAY
Because we want the game to be like a normal game and make it fun, we need to make the game easy in the
beginning and harder at the end.	In order to accomplish this, I feel that pure income shouldn't be the factor
for the queen, because she would likely crucify a would-be leader in his early days.	So, in the beginning of
the game, the forces would already be situated with the majority of forces being the administrators in the towns,
and army troops and elites in the more important sectors.	Restricting the queen's offensive
abilities using a distance penalty would mean that the furthest sectors from the queen's palace would be
much easier to defend because she would only be allowed to send x number of troops.	As you get closer to the
queen, she would be allowed to send larger forces to attack those towns in question.	Also, to further
increase the games difficulty as the campaign progresses in the player's favor, we could also increase the
quality of the queen's troops based purely on the peek progress percentage.	This is calculated using a formula
that determines how well the player is doing by combining loyalty of towns owned, income generated, etc.	So,
in the beginning of the game, the quality is at the worst, but once you capture your first mines/towns, it
permanently	increase the queen's quality rating, effectively bumping up the stakes.	By the time you capture
four or five mines, the queen is going to focus more (but not completely) on quality defense as she prepares
for your final onslaught.	This quality rating will augment the experience level, equipment rating, and/or
attribute ratings of the queen's troops.	I would maintain a table of these enhancements based on the current
quality rating hooking into the difficulty all along.

//EXPLANATION OF THE WEIGHT SYSTEM:
The strategic AI has two types of groups:	garrisons and patrol groups.	Each of these groups contain
information of it's needs, mainly desired population.	If the current population is greater than the
desired population, and the group will get a negative weight assigned to it, which means that it is willing
to give up troops to areas that need them more.	On the other hand, if a group has less than the desired population,
then the weight will be positive, meaning they are requesting reinforcements.

The weight generated will range between -100 and +100.	The calculated weight is modified by the priority
of the group.	If the priority of the group is high, they
*/


//Kaiden: While the values below were Defines that we built off of, the actuall
// values have been externalized into an INI file. Not all of them at this point,
// but more will be externalized as time goes on.

//#define BASIC_POOL_INCREMENT gGameExternalOptions.guiBaseQueenPoolIncrement

//Modifies the number of troops the queen has at the beginning of the game on top
//of all of the garrison and patrol groups.	Additionally, there are a total of
//16 sectors that are LEVEL 1, 2, or 3 garrison groups.	The lower the level, the more
//troops stay in that sector, and the rest will also be used as a secondary pool when
//the primary pool runs dry.	So basically, this number is only part of the equation.
//#define EASY_QUEENS_POOL_OF_TROOPS					150
//#define NORMAL_QUEENS_POOL_OF_TROOPS				200
//#define HARD_QUEENS_POOL_OF_TROOPS					400
//#define INSANE_QUEENS_POOL_OF_TROOPS				8000

//Modifies the starting values as well as the desired values for all of the garrisons.
//#define EASY_INITIAL_GARRISON_PERCENTAGES		70
//#define NORMAL_INITIAL_GARRISON_PERCENTAGES	100
//#define HARD_INITIAL_GARRISON_PERCENTAGES		150 //125
//#define INSANE_INITIAL_GARRISON_PERCENTAGES		200

//Madd - sets % of extra elite troops in army compositions
//#define INSANE_ELITE_BONUS					50//25
//#define EXPERT_ELITE_BONUS					25//0

//#define EASY_MIN_ENEMY_GROUP_SIZE						3
//#define NORMAL_MIN_ENEMY_GROUP_SIZE					4
//#define HARD_MIN_ENEMY_GROUP_SIZE						6
//#define INSANE_MIN_ENEMY_GROUP_SIZE						8

//Sets the starting alert chances.	Everytime an enemy arrives in a new sector, or the player,
//this is the chance the enemy will detect the player in adjacent sectors.	This chance is associated
//with each side checked.	Stationary groups do this check periodically.
//#define EASY_ENEMY_STARTING_ALERT_LEVEL			20
//#define	NORMAL_ENEMY_STARTING_ALERT_LEVEL		50
//#define HARD_ENEMY_STARTING_ALERT_LEVEL			70
//#define INSANE_ENEMY_STARTING_ALERT_LEVEL		90

//When an enemy spots and chases a player group, the alertness value decrements by this value.	The
//higher the value, the less of a chance the enemy will spot and attack subsequent groups.	This
//minimizes the aggressiveness of the enemy.	Ranges from 1-100 (but recommend 20-60).
//#define EASY_ENEMY_STARTING_ALERT_DECAY			75
//#define	NORMAL_ENEMY_STARTING_ALERT_DECAY		50
//#define HARD_ENEMY_STARTING_ALERT_DECAY			25
//#define INSANE_ENEMY_STARTING_ALERT_DECAY		10

//The base time that the queen can think about reinforcements for refilling lost patrol groups,
//town garrisons, etc. She only is allowed one action per 'turn'.
//#define EASY_TIME_EVALUATE_IN_MINUTES				480
//#define NORMAL_TIME_EVALUATE_IN_MINUTES			360
//#define HARD_TIME_EVALUATE_IN_MINUTES				180
//#define INSANE_TIME_EVALUATE_IN_MINUTES				90

//The variance added on.
//#define EASY_TIME_EVALUATE_VARIANCE					240
//#define NORMAL_TIME_EVALUATE_VARIANCE				180
//#define HARD_TIME_EVALUATE_VARIANCE					120
//#define INSANE_TIME_EVALUATE_VARIANCE				60

//When a player takes control of a sector, don't allow any enemy reinforcements to enter the sector for a
//limited amount of time.	This essentially dumbs down the AI, making it less aggressive.
//#define EASY_GRACE_PERIOD_IN_HOURS					144		// 6 days
//#define NORMAL_GRACE_PERIOD_IN_HOURS				96		// 4 days
//#define HARD_GRACE_PERIOD_IN_HOURS					48		// 2 days
//#define INSANE_GRACE_PERIOD_IN_HOURS				6		// hours!

//Defines how many days must pass before the queen is willing to refill a defeated patrol group.
//#define EASY_PATROL_GRACE_PERIOD_IN_DAYS		16
//#define NORMAL_PATROL_GRACE_PERIOD_IN_DAYS		12
//#define HARD_PATROL_GRACE_PERIOD_IN_DAYS		8
//#define INSANE_PATROL_GRACE_PERIOD_IN_DAYS		2

//Certain conditions can cause the queen to go into a "full alert" mode.	This means that temporarily, the queen's
//forces will automatically succeed adjacent checks until x number of enemy initiated battles occur.	The same variable
//is what is used to determine the free checks.
//#define EASY_NUM_AWARE_BATTLES							1
//#define NORMAL_NUM_AWARE_BATTLES						2
//#define HARD_NUM_AWARE_BATTLES							3
//#define INSANE_NUM_AWARE_BATTLES						4

BOOLEAN gfAutoAIAware = FALSE;

// original values to start a clean new game
INT32	iOrigGarrisonArraySize;
INT32	iOrigPatrolArraySize;

//Saved vars
INT8	gbPadding2[3]			= {0, 0, 0};	//NOT USED
BOOLEAN gfExtraElites			= 0;	//Set when queen compositions are augmented with bonus elites.
INT32	giGarrisonArraySize		= 0;
INT32	giPatrolArraySize		= 0;
INT32	giForcePercentage		= 0;	//Modifies the starting group sizes relative by percentage
INT32	giArmyAlertness			= 0;	//The chance the group will spot an adjacent player/militia
INT32	giArmyAlertnessDecay	= 0;	//How much the spotting chance decreases when spot check succeeds
UINT8	gubNumAwareBattles		= 0;	//When non-zero, this means the queen is very aware and searching for players.	Every time
										//there is an enemy initiated battle, this counter decrements until zero.	Until that point,
										//all adjacent sector checks automatically succeed.
BOOLEAN gfQueenAIAwake					= FALSE;	//This flag turns on/off the strategic decisions.	If it's off, no reinforcements
																					//or assaults will happen.
																					//@@@Alex, this flag is ONLY set by the first meanwhile scene which calls an action.	If this
																					//action isn't called, the AI will never turn on.	It is completely dependant on this action.	It can
																					//be toggled at will in the AIViewer for testing purposes.
INT32		giReinforcementPool			= 0;	//How many troops the queen has in reserve in noman's land.	These guys are spawned as needed in P3.
INT32	giReinforcementPoints		= 0;	//the entire army's capacity to provide reinforcements.
INT32		giRequestPoints					= 0;	//the entire army's need for reinforcements.
UINT8	gubSAIVersion						= SAI_VERSION;	//Used for adding new features to be saved.
UINT8		gubQueenPriorityPhase		= 0;	//Defines how far into defence the queen is -- abstractly related to defcon index ranging from 0-10.

BOOLEAN		gfUnlimitedTroops	= FALSE;
BOOLEAN		gfAggressiveQueen	= FALSE;

//10 is the most defensive
//Used for authorizing the use of the first battle meanwhile scene AFTER the battle is complete.	This is the case used when
//the player attacks a town, and is set once militia are sent to investigate.
BOOLEAN gfFirstBattleMeanwhileScenePending = FALSE;

//After the first battle meanwhile scene is finished, this flag is set, and the queen orders patrol groups to immediately fortify all towns.
BOOLEAN gfMassFortificationOrdered = FALSE;

UINT8		gubMinEnemyGroupSize			= 0;
UINT8		gubHoursGracePeriod		= 0;
UINT16	gusPlayerBattleVictories = 0;
BOOLEAN gfUseAlternateQueenPosition = FALSE;
INT32 giTotalRecruitsInTraining = 0;

//padding for generic globals
#define SAI_PADDING_BYTES				93
INT8		gbPadding[SAI_PADDING_BYTES];
//patrol group info plus padding
#define SAVED_PATROL_GROUPS			MAX_PATROL_GROUPS
PATROL_GROUP *gPatrolGroup			= NULL;
//army composition info plus padding
#define SAVED_ARMY_COMPOSITIONS		MAX_ARMY_COMPOSITIONS
ARMY_COMPOSITION gArmyComp[ MAX_ARMY_COMPOSITIONS ];
//garrison info plus padding
#define SAVED_GARRISON_GROUPS		MAX_GARRISON_GROUPS
GARRISON_GROUP *gGarrisonGroup	= NULL;

//
UINT8 NUM_ARMY_COMPOSITIONS;

extern UINT8 gubNumGroupsArrivedSimultaneously;

//This refers to the number of force points that are *saved* for the AI to use.	This is basically an array of each
//group.	When the queen wants to send forces to attack a town that is defended, the initial number of forces that
//she would send would be considered too weak.	So, instead, she will send that force to the sector's adjacent sector,
//and stage, while
UINT8 *gubGarrisonReinforcementsDenied = NULL;
UINT8 *gubPatrolReinforcementsDenied = NULL;

//Unsaved vars
BOOLEAN gfDisplayStrategicAILogs = FALSE;

void ValidatePendingGroups();
void ValidateWeights( INT32 iID );
void ValidateGroup( GROUP *pGroup );
void ValidateLargeGroup( GROUP *pGroup );

extern BOOLEAN TeleportSoldier( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fForce );


//The army composition defines attributes for the various garrisons.	The priority reflects how important the sector is
//to the queen, the elite/troop percentages refer to the desired composition of the group.	The admin percentage has recently been
//changed to reflect the starting percentage of the garrison that are administrators.	Note that elite% + troop% = 100, and the admin% is
//not related in this effect.	If the admin% is non-zero, then that garrison is assigned only x% of the force as admins, with NO troops or elites.
//All reinforcements use the composition of the troop/elite for refilling.
//@@@Alex, the send reinforcement composition isn't complete.	Either sends all troops or troops based off of the composition of the source garrison.
//	It is my intention to add this.

//If you change the MAX_STRATEGIC_TEAM_SIZE, then all the garrison sizes (start, desired) will have to be changed accordingly.

ARMY_COMPOSITION gOrigArmyComp[ MAX_ARMY_COMPOSITIONS ];// =
//{	//COMPOSITION				PRIORITY		ELITE%		TROOP%		ADMIN 		DESIRED#		START#		PADDING
//	//																								START%
//	QUEEN_DEFENCE,				100,			100,		0,			0,			32,				32,				{0,0,0,0,0,0,0,0,0,0},
//	MEDUNA_DEFENCE,				95,				55,			45,			0,			16,				20,				{0,0,0,0,0,0,0,0,0,0},
//	MEDUNA_SAMSITE,				96,				65,			35,			0,			20,				20,				{0,0,0,0,0,0,0,0,0,0},
//	LEVEL1_DEFENCE,				40,				20,			80,			0,			12,				20,				{0,0,0,0,0,0,0,0,0,0},
//	LEVEL2_DEFENCE,				30,				10,			90,			0,			10,				20,				{0,0,0,0,0,0,0,0,0,0},
//	LEVEL3_DEFENCE,				20,				5,			95,			0,			8,				20,				{0,0,0,0,0,0,0,0,0,0},
//	ORTA_DEFENCE,				90,				50,			50,			0,			18,				19,				{0,0,0,0,0,0,0,0,0,0},
//	EAST_GRUMM_DEFENCE,			80,				20,			80,			0,			15,				15,				{0,0,0,0,0,0,0,0,0,0},
//	WEST_GRUMM_DEFENCE,			70,				0,			100,		40,			15,				15,				{0,0,0,0,0,0,0,0,0,0},
//	GRUMM_MINE,					85,				25,			75,			45,			15,				15,				{0,0,0,0,0,0,0,0,0,0},
//	OMERTA_WELCOME_WAGON,		0,				0,			100,		0,			0,				3,				{0,0,0,0,0,0,0,0,0,0},
//	BALIME_DEFENCE,				60,				45,			55,			20,			10,				4,				{0,0,0,0,0,0,0,0,0,0},
//	TIXA_PRISON,				80,				10,			90,			15,			15,				15,				{0,0,0,0,0,0,0,0,0,0},
//	TIXA_SAMSITE,				85,				10,			90,			0,			12,				12,				{0,0,0,0,0,0,0,0,0,0},
//	ALMA_DEFENCE,				74,				15,			85,			0,			11,				20,				{0,0,0,0,0,0,0,0,0,0},
//	ALMA_MINE,					80,				20,			80,			45,			15,				20,				{0,0,0,0,0,0,0,0,0,0},
//	CAMBRIA_DEFENCE,			50,				0,			100,		30,			10,				6,				{0,0,0,0,0,0,0,0,0,0},
//	CAMBRIA_MINE,				60,				15,			90,			40,			11,				6,				{0,0,0,0,0,0,0,0,0,0},
//	CHITZENA_DEFENCE,			30,				0,			100,		75,			12,				10,				{0,0,0,0,0,0,0,0,0,0},
//	CHITZENA_MINE,				40,				0,			100,		75,			10,				10,				{0,0,0,0,0,0,0,0,0,0},
//	CHITZENA_SAMSITE,			75,				10,			90,			0,			9,				9,				{0,0,0,0,0,0,0,0,0,0},
//	DRASSEN_AIRPORT,			30,				0,			100,		85,			12,				10,				{0,0,0,0,0,0,0,0,0,0},
//	DRASSEN_DEFENCE,			20,				0,			100,		80,			10,				8,				{0,0,0,0,0,0,0,0,0,0},
//	DRASSEN_MINE,				35,				0,			100,		75,			11,				9,				{0,0,0,0,0,0,0,0,0,0},
//	DRASSEN_SAMSITE,			50,				0,			100,		0,			10,				10,				{0,0,0,0,0,0,0,0,0,0},
//	ROADBLOCK,					20,				2,			98,			0,			8,				0,				{0,0,0,0,0,0,0,0,0,0},
//	SANMONA_SMALL,				0,				0,			0,			0,			0,				0,				{0,0,0,0,0,0,0,0,0,0},
//};

//Patrol definitions
//NOTE:	A point containing 0 is actually the same as SEC_A1, but because nobody is using SEC_A1 in any
//				of the patrol groups, I am coding 0 to be ignored.
//NOTE:		Must have at least two points.
PATROL_GROUP gOrigPatrolGroup[MAX_PATROL_GROUPS];// =
//{ //SIZE	PRIORITY	POINT1		POINT2		POINT3		POINT4		MOD 	GROUPID	WEIGHT	PENDING
//	//																	DAY100					GROUP ID
//	8,		40,			SEC_B1,		SEC_C1,		SEC_C3,		SEC_A3,		-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	6,		35,			SEC_B4,		SEC_B7,		SEC_C7,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	6,		25,			SEC_A8,		SEC_B8,		SEC_B9,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	6,		30,			SEC_B10,	SEC_B12,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	7,		45,			SEC_A11,	SEC_A14,	SEC_D14,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//5
//	6,		50,			SEC_C8,		SEC_C9,		SEC_D9,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		55,			SEC_D3,		SEC_G3,		0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	10,		50,			SEC_D6,		SEC_D7,		SEC_F7,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	10,		55,			SEC_E8,		SEC_E11,	SEC_F11,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	10,		60,			SEC_E12,	SEC_E15,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//10
//	12,		60,			SEC_G4,		SEC_G7,		0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		65,			SEC_G10,	SEC_G12,	SEC_F12,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		65,			SEC_G13,	SEC_G15,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	10,		65,			SEC_H15,	SEC_J15,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	14,		65,			SEC_H12,	SEC_J12,	SEC_J13,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//15
//	13,		70,			SEC_H9,		SEC_I9,		SEC_I10,	SEC_J10,	-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	11,		70,			SEC_K11,	SEC_K14,	SEC_J14,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		75,			SEC_J2,		SEC_K2,		0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		80,			SEC_I3,		SEC_J3,		0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		80,			SEC_J6,		SEC_K6,		0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//20
//	13,		85,			SEC_K7,		SEC_K10,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		90,			SEC_L10,	SEC_M10,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		90,			SEC_N9,		SEC_N10,	0,			0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		80,			SEC_L7,		SEC_L8,		SEC_M8,		SEC_M9,		-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	14,		80,			SEC_H4,		SEC_H5,		SEC_I5,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//25
//	7,		40,			SEC_D4,		SEC_E4,		SEC_E5,		0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	7,		50,			SEC_C10,	SEC_C11,	SEC_D11,	SEC_D12,	-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	8,		40,			SEC_A15,	SEC_C15,	SEC_C16,	0,			-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	12,		30,			SEC_L13,	SEC_M13,	SEC_M14,	SEC_L14,	-1,		0,		0,		0,				{0,0,0,0,0,0,0,0,0,0},
//	//29
//};
//#define PATROL_GROUPS 29



GARRISON_GROUP gOrigGarrisonGroup[MAX_GARRISON_GROUPS];// =
//{	//SECTOR	MILITARY						WEIGHT					UNUSED
//	//			COMPOSITION						GROUP ID
//	SEC_P3,		QUEEN_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_O3,		MEDUNA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_O4,		MEDUNA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_N3,		MEDUNA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_N4,		MEDUNA_SAMSITE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//5
//	SEC_N5,		MEDUNA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_M3,		LEVEL1_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_M4,		LEVEL1_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_M5,		LEVEL1_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_N6,		LEVEL1_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//10
//	SEC_M2,		LEVEL2_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L3,		LEVEL2_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L4,		LEVEL2_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L5,		LEVEL2_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_M6,		LEVEL2_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//15
//	SEC_N7,		LEVEL1_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L2,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_K3,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_K5,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L6,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//20
//	SEC_M7,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_N8,		LEVEL3_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_K4,		ORTA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G1,		WEST_GRUMM_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G2,		EAST_GRUMM_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//25
//	SEC_H1,		WEST_GRUMM_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_H2,		EAST_GRUMM_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_H3,		GRUMM_MINE,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_A9,		OMERTA_WELCOME_WAGON,			0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_L11,	BALIME_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//30
//	SEC_L12,	BALIME_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_J9,		TIXA_PRISON,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_I8,		TIXA_SAMSITE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_H13,	ALMA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_H14,	ALMA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//35
//	SEC_I13,	ALMA_DEFENCE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_I14,	ALMA_MINE,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_F8,		CAMBRIA_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_F9,		CAMBRIA_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G8,		CAMBRIA_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//40
//	SEC_G9,		CAMBRIA_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_H8,		CAMBRIA_MINE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_A2,		CHITZENA_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_B2,		CHITZENA_MINE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_D2,		CHITZENA_SAMSITE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//45
//	SEC_B13,	DRASSEN_AIRPORT,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_C13,	DRASSEN_DEFENCE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_D13,	DRASSEN_MINE,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_D15,	DRASSEN_SAMSITE,				0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G12,	ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//50
//	SEC_M10,	ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G6,		ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_C9,		ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_K10,	ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_G7,		ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//55
//	SEC_G3,		ROADBLOCK,						0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	SEC_C5,		SANMONA_SMALL,					0,			0,			{0,0,0,0,0,0,0,0,0,0},
//	//57
//};

//Various decision functions and utils to help make those decisions.
BOOLEAN AdjacentSectorIsImportantAndUndefended( UINT8 ubSectorID );
BOOLEAN HandleEmptySectorNoticedByPatrolGroup( GROUP *pGroup, UINT8 ubEmptySectorID );
void HandleEmptySectorNoticedByGarrison( UINT8 ubGarrisonSectorID, UINT8 ubEmptySectorID );

UINT16 GetOffensePoints(const ENEMYGROUP* pEnemyGroup);
UINT16 GetOffensePoints(const SECTORINFO* pSectorInfo);
BOOLEAN PlayerForceTooStrong( UINT8 ubSectorID, UINT16 usOffensePoints, UINT16 *pusDefencePoints );
void RequestAttackOnSector( UINT8 ubSectorID, UINT16 usDefencePoints );
void RequestHighPriorityStagingGroupReinforcements( GROUP *pGroup );
void RequestHighPriorityGarrisonReinforcements( INT32 iGarrisonID, UINT8 ubSoldiersRequested );

BOOLEAN GarrisonCanProvideMinimumReinforcements( INT32 iGarrisonID );
BOOLEAN GarrisonRequestingMinimumReinforcements( INT32 iGarrisonID );
BOOLEAN PatrolRequestingMinimumReinforcements( INT32 iPatrolID );

//These are the chance functions that the AI uses to see player's/militia in adjacent sectors.
BOOLEAN AttemptToNoticeAdjacentGroupSucceeds();
BOOLEAN AttemptToNoticeEmptySectorSucceeds();

void EliminateSurplusTroopsForGarrison( GROUP *pGroup, SECTORINFO *pSector );
void ReinitializeUnvisitedGarrisons();

//Records any decisions that I deem important enough into an automatically appending AI log file called
//"Strategic Decisions.txt" in the JA2\Data directory.	This also records the time that each log entry
//was made.
#ifdef JA2BETAVERSION

void LogStrategicEvent( STR8	str, ... ); //adds a timestamp.
void LogStrategicMsg( STR8	str, ... ); //doesn't use the time stamp

void ClearStrategicLog();
#endif

//Recalculates a group's weight based on any changes.
//@@@Alex, this is possibly missing in some areas.	It is hard to ensure it is
//everywhere with all the changes I've made.	I'm sure you could probably find some missing calls.
void RecalculatePatrolWeight( INT32 iPatrolID );
void RecalculateGarrisonWeight( INT32 iGarrisonID );

extern INT8 GetTownAssociatedWithMine( INT8 bMineIndex );
extern INT8 GetMineIndexForSector( INT16 sX, INT16 sY );


INT32 GarrisonReinforcementsRequested( INT32 iGarrisonID, UINT8 *pubExtraReinforcements );
INT32 PatrolReinforcementsRequested( INT32 iPatrolID );
INT32 ReinforcementsAvailable( INT32 iGarrisonID );
BOOLEAN ReinforcementsApproved( INT32 iGarrisonID, UINT16 *pusDefencePoints );
BOOLEAN SendReinforcementsForGarrison( INT32 iDstGarrisonID, UINT16 usDefencePoints, GROUP **pOptionalGroup );
BOOLEAN SendReinforcementsForPatrol( INT32 iPatrolID, GROUP **pOptionalGroup );

void ClearPreviousAIGroupAssignment( GROUP *pGroup );

void CalcNumTroopsBasedOnComposition( UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 ubTotal, INT32 iCompositionID );
void ConvertGroupTroopsToComposition( GROUP *pGroup, INT32 iCompositionID );
void RemoveSoldiersFromGarrisonBasedOnComposition( INT32 iGarrisonID, UINT8 ubSize );

//If there are any enemy groups that will be moving through this sector due, they will have to repath which
//will cause them to avoid the sector.	Returns the number of redirected groups.
UINT8 RedirectEnemyGroupsMovingThroughSector( UINT8 ubSectorX, UINT8 ubSectorY );



//As the player's progress changes in the game, the queen will adjust her priorities accordingly.
//Basically, increasing priorities and numbers for sectors she owns, and lowering them.
//@@@Alex, this is tweakable.	My philosophies could be incorrect.	It might be better if instead of lowering
//priorities and numbers for towns the queen has lost, to instead lower the priority but increase the numbers so
//she would send larger attack forces.	This is questionable.
void EvolveQueenPriorityPhase( BOOLEAN fForceChange );

extern INT16 sWorldSectorLocationOfFirstBattle;

void ReassignAIGroup( GROUP **pGroup );
void TransferGroupToPool( GROUP **pGroup );
void SendGroupToPool( GROUP **pGroup );

//Simply orders all garrisons to take troops from the patrol groups and send the closest troops from them.	Any garrison,
//whom there request isn't fulfilled (due to lack of troops), will recieve their reinforcements from the queen (P3).
void MassFortifyTowns();

void UpgradeAdminsToTroops();


#ifdef JA2BETAVERSION
void SAIReportError( STR16 wErrorString );
#else
#define SAIReportError( a ) //define it out
#endif

/* This is only a dirty fix to prevent CTD:
 * when loading a game, sometimes I found that
 * 'gGarrisonGroup' was not allocated.
 * so I did:
 * - added NULL check,
 * - added some AssertMsg,
 * - added new allocation on different places
 * - condensed this paces to the following function
 * ---------------------------
 * 2007-03-03, Sgt. Kolja
 */

/* PUBLIC */
void Ensure_RepairedGarrisonGroup( GARRISON_GROUP **ppGarrison, INT32 *pGarraySize )
{
	{
	GARRISON_GROUP *pG;

	pG = *ppGarrison;

	if( !pG )
	{
	//AssertMsg( 0, "invalid global Array in" __FILE__ );
	//return;
		pG = (GARRISON_GROUP*)MemAlloc( sizeof( gOrigGarrisonGroup ) );
	Assert( pG );
	memcpy( pG, gOrigGarrisonGroup, sizeof( gOrigGarrisonGroup ) );
	*pGarraySize = sizeof( gOrigGarrisonGroup ) / sizeof(GARRISON_GROUP);
	*ppGarrison = pG;
	}
	return;
}}



//returns the number of reinforcements permitted to be sent.	Will increased if the denied counter is non-zero.
INT32 GarrisonReinforcementsRequested( INT32 iGarrisonID, UINT8 *pubExtraReinforcements )
{
	INT32 iExtraReinforcements;
	
	INT32 iReinforcementsRequested;
	INT32 iExistingForces;
	SECTORINFO *pSector;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
 DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic1");

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	iExistingForces = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	iReinforcementsRequested = gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bDesiredPopulation - iExistingForces;

	//Record how many of the reinforcements are additionally provided due to being denied in the past.	This will grow
	//until it is finally excepted or an absolute max is made.
	
	if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		iExtraReinforcements = 1;
	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
		iExtraReinforcements = 2;
	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
		iExtraReinforcements = 3;
	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
		iExtraReinforcements = 4;
	else
	{
		iExtraReinforcements = Random (4);
		if (iExtraReinforcements == 0) iExtraReinforcements = 1;
	}
		
	*pubExtraReinforcements = (UINT8)(gubGarrisonReinforcementsDenied[ iGarrisonID ] / (6 - iExtraReinforcements));
	//*pubExtraReinforcements = (UINT8)(gubGarrisonReinforcementsDenied[ iGarrisonID ] / (6 - gGameOptions.ubDifficultyLevel));
	
	//Make sure the number of extra reinforcements don't bump the force size past the max of MAX_STRATEGIC_TEAM_SIZE.
	*pubExtraReinforcements = (UINT16)min( (INT32)*pubExtraReinforcements, min( (INT32)(*pubExtraReinforcements), iMaxEnemyGroupSize - iReinforcementsRequested ) );

	iReinforcementsRequested = min( iMaxEnemyGroupSize, iReinforcementsRequested );

	return iReinforcementsRequested;
}

INT32 PatrolReinforcementsRequested( INT32 iPatrolID )
{
	GROUP *pGroup;
	pGroup = GetGroup( gPatrolGroup[ iPatrolID ].ubGroupID );
	if( !pGroup )
	{
		return gPatrolGroup[ iPatrolID ].bSize;
	}
	else
	{
		return gPatrolGroup[ iPatrolID ].bSize - pGroup->ubGroupSize;
	}
}

INT32 ReinforcementsAvailable( INT32 iGarrisonID )
{
	SECTORINFO *pSector;
	INT32 iReinforcementsAvailable;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	iReinforcementsAvailable = pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumAdmins + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	iReinforcementsAvailable -= gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bDesiredPopulation;

	switch( gGarrisonGroup[ iGarrisonID ].ubComposition )
	{
		case LEVEL1_DEFENCE:
		case LEVEL2_DEFENCE:
		case LEVEL3_DEFENCE:
		case ALMA_DEFENCE:
		case ALMA_MINE:
			//Legal spawning locations
			break;
		default:
			//No other sector permitted to send surplus troops
			return 0;
	}

	return iReinforcementsAvailable;
}

UINT16 GetOffensePoints(const ENEMYGROUP* pEnemyGroup)
{
	return pEnemyGroup->ubNumAdmins * ADMIN_POINTS_MODIFIER +
			pEnemyGroup->ubNumTroops * REGULAR_POINTS_MODIFIER +
			pEnemyGroup->ubNumElites * ELITE_POINTS_MODIFIER +
			pEnemyGroup->ubNumRobots * ROBOT_POINTS_MODIFIER +
			pEnemyGroup->ubNumTanks * TANK_POINTS_MODIFIER + 
			pEnemyGroup->ubNumJeeps * JEEP_POINTS_MODIFIER;
}

UINT16 GetOffensePoints(const SECTORINFO* pSectorInfo)
{
	return pSectorInfo->ubNumAdmins * ADMIN_POINTS_MODIFIER +
			pSectorInfo->ubNumTroops * REGULAR_POINTS_MODIFIER +
			pSectorInfo->ubNumElites * ELITE_POINTS_MODIFIER +
			pSectorInfo->ubNumRobots * ROBOT_POINTS_MODIFIER +
			pSectorInfo->ubNumTanks * TANK_POINTS_MODIFIER + 
			pSectorInfo->ubNumJeeps * JEEP_POINTS_MODIFIER;
}

BOOLEAN PlayerForceTooStrong( UINT8 ubSectorID, UINT16 usOffensePoints, UINT16 *pusDefencePoints )
{
	SECTORINFO *pSector;
	UINT8 ubSectorX, ubSectorY;

	ubSectorX = (UINT8)SECTORX( ubSectorID );
	ubSectorY = (UINT8)SECTORY( ubSectorID );
	pSector = &SectorInfo[ ubSectorID ];

	// SANDRO - EVALUATE THE STRENGTH OF MILITIA BASED ON INI SETTING
	*pusDefencePoints = PlayerMercsInSector( ubSectorX, ubSectorY, 0 ) * 5;
	*pusDefencePoints += (MilitiaInSectorOfRank( ubSectorX, ubSectorY, GREEN_MILITIA ) * GREEN_MILITIA_POINTS_MODIFIER * (100 + gGameExternalOptions.sGreenMilitiaAutoresolveStrength) / 100);
	*pusDefencePoints += (MilitiaInSectorOfRank( ubSectorX, ubSectorY, REGULAR_MILITIA ) * REGULAR_MILITIA_POINTS_MODIFIER * (100 + gGameExternalOptions.sRegularMilitiaAutoresolveStrength) / 100);
	*pusDefencePoints += (MilitiaInSectorOfRank( ubSectorX, ubSectorY, ELITE_MILITIA ) * ELITE_MILITIA_POINTS_MODIFIER * (100 + gGameExternalOptions.sVeteranMilitiaAutoresolveStrength) / 100);

	if( *pusDefencePoints > usOffensePoints )
	{
		return TRUE;
	}
	return FALSE;
}

void RequestAttackOnSector( UINT8 ubSectorID, UINT16 usDefencePoints )
{
	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for ( INT32 i = 0; i < giGarrisonArraySize; ++i )
	{
		if( gGarrisonGroup[ i ].ubSectorID == ubSectorID && !gGarrisonGroup[ i ].ubPendingGroupID )
		{
#ifdef JA2BETAVERSION
			LogStrategicEvent( "An attack has been requested in sector %c%d.",
				SECTORY( ubSectorID ) + 'A' - 1, SECTORX( ubSectorID ) );
#endif
			if ( SendReinforcementsForGarrison( i, usDefencePoints, NULL ) )
				return;
		}
	}
}



BOOLEAN AdjacentSectorIsImportantAndUndefended( UINT8 ubSectorID )
{
	SECTORINFO *pSector;

	switch( ubSectorID )
	{
		//case SEC_A9:	case SEC_A10:					//Omerta
		case SEC_C5:	case SEC_C6:	case SEC_D5:	//San Mona
		case SEC_I6:									//Estoni

		//These sectors aren't important. //Lalien: changed this, need to externalize
		return FALSE;
	}

	pSector = &SectorInfo[ ubSectorID ];

	if ( pSector->ubNumTroops || pSector->ubNumElites || pSector->ubNumAdmins || pSector->ubNumRobots || pSector->ubNumTanks || pSector->ubNumJeeps )
	{
		return FALSE;
	}

	if( pSector->ubTraversability[ 4 ] == TOWN )
	{
		if( !PlayerSectorDefended( ubSectorID ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void ValidatePendingGroups()
{
	#ifdef JA2BETAVERSION
		GROUP *pGroup;
		INT32 i, iErrorsForInvalidPendingGroup = 0;
		UINT8 ubGroupID;

		Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

		for( i = 0; i < giPatrolArraySize; i++ )
		{
			ubGroupID = gPatrolGroup[ i ].ubPendingGroupID;
			if( ubGroupID )
			{
				pGroup = GetGroup( ubGroupID );
				if( !pGroup || pGroup->usGroupTeam == OUR_TEAM )
				{
					iErrorsForInvalidPendingGroup++;
					gPatrolGroup[ i ].ubPendingGroupID = 0;
				}
			}
		}
		for( i = 0; i < giGarrisonArraySize; i++ )
		{
			ubGroupID = gGarrisonGroup[ i ].ubPendingGroupID;
			if( ubGroupID )
		{
			pGroup = GetGroup( ubGroupID );
			if ( !pGroup || pGroup->usGroupTeam == OUR_TEAM )
				{
					iErrorsForInvalidPendingGroup++;
				gGarrisonGroup[ i ].ubPendingGroupID = 0;
			}
		}
		}
		if( iErrorsForInvalidPendingGroup )
		{
			CHAR16 str[256];
			swprintf( str, L"Strategic AI: Internal error -- %d pending groups were discovered to be invalid. Please report error and send save."
										L"You can continue playing, as this has been auto-corrected. No need to send any debug files.", iErrorsForInvalidPendingGroup );
			SAIReportError( str );
		}
	#endif
}

void ValidateWeights( INT32 iID )
{
	#ifdef JA2BETAVERSION
		INT32 i;
		INT32 iSumRequestPoints = 0;
		INT32 iSumReinforcementPoints = 0;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( i = 0; i < giPatrolArraySize; i++ )
 	{
 		iSumRequestPoints += gPatrolGroup[ i ].bWeight;
 	}
 	for( i = 0; i < giGarrisonArraySize; i++ )
 	{
 		if( gGarrisonGroup[ i ].bWeight > 0 )
 		{
 			iSumRequestPoints += gGarrisonGroup[ i ].bWeight;
 		}
 		else if( gGarrisonGroup[ i ].bWeight < 0 )
 		{
 			iSumReinforcementPoints -= gGarrisonGroup[ i ].bWeight; //double negative is positive!
 		}
 	}

		if( giReinforcementPoints != iSumReinforcementPoints || giRequestPoints != iSumRequestPoints )
		{
			CHAR16 str[256];
			swprintf( str, L"Strategic AI: Internal error #%02d (total request/reinforcement points). Please report error including error#."
										L"You can continue playing, as the points have been auto-corrected. No need to send any save/debug files.", iID );
			//Correct the misalignment.
			giReinforcementPoints = iSumReinforcementPoints;
			giRequestPoints = iSumRequestPoints;
			SAIReportError( str );
		}
	#endif
}

void ValidateGroup( GROUP *pGroup )
{
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic2");

	if (pGroup == NULL)
	{
		return;
	}

	if( !pGroup->ubSectorX || !pGroup->ubSectorY || pGroup->ubSectorX > 16 || pGroup->ubSectorY > 16 )
	{
		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			#ifdef JA2BETAVERSION
			{
				CHAR16 str[256];
				swprintf( str, L"Strategic AI: Internal error (invalid enemy group #%d location at %c%d, destination %c%d). Please send PRIOR save file and Strategic Decisions.txt.",
											pGroup->ubGroupID, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX, pGroup->ubNextY + 'A' - 1, pGroup->ubNextX );
				SAIReportError( str );
			}
			#endif
			ClearPreviousAIGroupAssignment( pGroup );
			RemovePGroup( pGroup );
			return;
		}
	}
	if( !pGroup->ubNextX || !pGroup->ubNextY )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && pGroup->pEnemyGroup->ubIntention != STAGING
												&& pGroup->pEnemyGroup->ubIntention != REINFORCEMENTS )
		{
			#ifdef JA2BETAVERSION
				SAIReportError( L"Strategic AI:	Internal error (floating group).	Please send PRIOR save file and Strategic Decisions.txt." );
			#endif
			if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
			{
				ClearPreviousAIGroupAssignment( pGroup );
				ReassignAIGroup( &pGroup );
				return;
			}
		}
	}
	#ifdef JA2BETAVERSION
	if ( pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots != pGroup->ubGroupSize ||
				pGroup->ubGroupSize > iMaxEnemyGroupSize )
		{
			SAIReportError( L"Strategic AI: Internal error (bad group populations). Please send PRIOR save file and Strategic Decisions.txt." );
		}
	#endif
}

void ValidateLargeGroup( GROUP *pGroup )
{
	#ifdef JA2BETAVERSION
	/* WDS - this code is no longer applicable with changes to group sizes
		if( pGroup->ubGroupSize > 25 )
		{
			CHAR16 str[ 512 ];
			swprintf( str, L"Strategic AI warning: Enemy group containing %d soldiers "
									L"(%d admins, %d troops, %d elites) in sector %c%d. This message is a temporary test message "
									L"to evaluate a potential problems with very large enemy groups.",
									pGroup->ubGroupSize, pGroup->pEnemyGroup->ubNumAdmins, pGroup->pEnemyGroup->ubNumTroops, pGroup->pEnemyGroup->ubNumElites,
									pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
			SAIReportError( str );
		}
    */
	#endif
}

#ifdef JA2BETAVERSION
void RemovePlayersFromAllMismatchGroups( SOLDIERTYPE *pSoldier )
{
	GROUP *pGroup, *pTempGroup = NULL;
	PLAYERGROUP *pPlayer;
	BOOLEAN fRemoveSoldierFromThisGroup = FALSE;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam == OUR_TEAM )
		{
			pPlayer = pGroup->pPlayerList;
			while( pPlayer )
			{
				if( pPlayer->pSoldier == pSoldier )
				{
					if( pSoldier->ubGroupID != pGroup->ubGroupID )
					{
						fRemoveSoldierFromThisGroup = TRUE;
						pTempGroup = pGroup;
					}
					break;
				}
				pPlayer = pPlayer->next;
			}
		}
		pGroup = pGroup->next;
		if( fRemoveSoldierFromThisGroup )
		{
			fRemoveSoldierFromThisGroup = FALSE;
			RemovePlayerFromPGroup( pTempGroup, pSoldier );
		}
	}
}
#endif

#ifdef JA2BETAVERSION
void ValidatePlayersAreInOneGroupOnly()
{
	INT32 i;
	INT32 iGroups;
	INT32 iMismatches;
	INT32 iNumErrors;
	GROUP *pGroup, *pOtherGroup;
	PLAYERGROUP *pPlayer;
	SOLDIERTYPE *pSoldier;
	CHAR16 str[ 1024 ];
	UINT8 ubGroupID;
	//Go through each merc slot in the player team
	iNumErrors = 0;
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
	{ //check to see if the merc has a group ID

		pSoldier = MercPtrs[ i ];

		if( !pSoldier || !pSoldier->bActive || !pSoldier->stats.bLife || !pSoldier->ubGroupID )
		{ //non-existant, dead, or in no group (don't care, skip to next merc)
			continue;
		}

		//Record the merc's group ID, as we may have to restore this later if the merc is found to exist
		//in multiple groups.
		ubGroupID = pSoldier->ubGroupID;

		iGroups = 0;
		iMismatches = 0;
		//Go through each group and determine if the player exists in multiple groups
		//iGroups ------ counts the number of groups the merc is in.
		//iMismatches -- counts the cases where the merc's ubGroupID doesn't match the ubGroupID of the group
		//				the merc exists in.
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->usGroupTeam == OUR_TEAM )
			{
				pPlayer = pGroup->pPlayerList;
				while( pPlayer )
				{
					if( pPlayer->pSoldier == pSoldier )
					{
						if( pSoldier->ubGroupID != pGroup->ubGroupID )
						{
							iMismatches++;
						}
						iGroups++;
						break;
					}
					pPlayer = pPlayer->next;
				}
			}
			pGroup = pGroup->next;
		}

		if( iMismatches || !iGroups )
		{ //If we have any mismatches or a merc not in a group, then there is definately an error.
			//We need to record and report the "first" error in detail
			iNumErrors++; //keeps track of the total errors (this number will be reported)

			if( iNumErrors == 1 )
			{ //This is the first error, so we will provide detailed debug information to help fix the bug(s).
				if( iGroups == 1 && iMismatches == 1 )
				{ //We have a very serious problem, as we have no way to know which group this merc is supposed to be in.
					//This problem cannot be corrected (so we will assign the merc to his own unique squad) and definately report it.

					//Get a pointer to the group that contains the merc
					iMismatches = 0;
					iGroups = 0;
					pGroup = gpGroupList;
					pOtherGroup = NULL;
					while( pGroup )
					{
						if ( pGroup->usGroupTeam == OUR_TEAM )
						{
							pPlayer = pGroup->pPlayerList;
							while( pPlayer )
							{
								if( pPlayer->pSoldier == pSoldier )
								{
									if( pSoldier->ubGroupID != pGroup->ubGroupID )
									{
										pOtherGroup = pGroup;
										iMismatches++;
									}
									iGroups++;
									break;
								}
								pPlayer = pPlayer->next;
							}
						}
						if( iMismatches == 1 )
						{
							break;
						}
						pGroup = pGroup->next;
					}
					pGroup = GetGroup( pSoldier->ubGroupID );
					Assert( pGroup );
					Assert( pOtherGroup );
					swprintf( str, L"%s in %c%d thinks he/she is in group %d in %c%d but isn't. "
												L"Group %d in %c%d thinks %s is in the group but isn't. %s will be assigned to a unique squad. "
												L"Please send screenshot, PRIOR save (corrected by time you read this), and any theories.",
												pSoldier->name, pSoldier->sSectorY + 'A' - 1, pSoldier->sSectorX,
												pSoldier->ubGroupID, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX,
												pOtherGroup->ubGroupID, pOtherGroup->ubSectorY + 'A' - 1, pOtherGroup->ubSectorX, pSoldier->name,
												pSoldier->name );
				}
				else if( iGroups > 1 && iMismatches == iGroups - 1 )
				{ //This is the error that is being targetted.	This means that the merc exists in multiple groups though the merc
					//knows what group he is supposed to be in.	This error can be corrected, by manually removing the merc from all
					//mismatch groups.	This is indicative of a merc being reassigned to another group without removing him first.

					//Get a pointer to the first mismatch group that contains the merc
					iMismatches = 0;
					iGroups = 0;
					pGroup = gpGroupList;
					pOtherGroup = NULL;
					while( pGroup )
					{
						if ( pGroup->usGroupTeam == OUR_TEAM )
						{
							pPlayer = pGroup->pPlayerList;
							while( pPlayer )
							{
								if( pPlayer->pSoldier == pSoldier )
								{
									if( pSoldier->ubGroupID != pGroup->ubGroupID )
									{
										pOtherGroup = pGroup;
										iMismatches++;
									}
									iGroups++;
									break;
								}
								pPlayer = pPlayer->next;
							}
						}
						pGroup = pGroup->next;
					}
					pGroup = GetGroup( pSoldier->ubGroupID );
					Assert( pGroup );
					Assert( pOtherGroup );

					swprintf( str, L"%s in %c%d has been found in multiple groups. The group he/she is supposed "
												L"to be in is group %d in %c%d, but %s was also found to be in group %d in %c%d. %s was found in %d groups "
												L"total. Please send screenshot, PRIOR save (corrected by time you read this), and any theories.",
												pSoldier->name, pSoldier->sSectorY + 'A' - 1, pSoldier->sSectorX,
												pGroup->ubGroupID, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX,
												pSoldier->name, pOtherGroup->ubGroupID, pOtherGroup->ubSectorY + 'A' - 1, pOtherGroup->ubSectorX,
												pSoldier->name, iGroups );
				}
				else if( !iGroups )
				{ //The merc cannot be found in any group!	This should never happen!	We will assign the merc into his
					//own unique squad as a correction.
					swprintf( str, L"%s in %c%d cannot be found in any group. %s will be assigned to a unique group/squad. "
												L"Please provide details on how you think this may have happened. Send screenshot and PRIOR save. Do not send a save "
												L"you create after this point as the info will have been corrected by then.",
												pSoldier->name, pSoldier->sSectorY + 'A' - 1, pSoldier->sSectorX, pSoldier->name );
				}
			}

			//CORRECT THE ERRORS NOW
			if( iGroups == 1 && iMismatches == 1 )
			{ //We have a very serious problem, as we have no way to know which group this merc is supposed to be in.
				//This problem cannot be corrected (so we will assign the merc to his own unique squad).
				RemovePlayersFromAllMismatchGroups( pSoldier );
				AddCharacterToUniqueSquad( pSoldier );
			}
			else if( iGroups > 1 && iMismatches == iGroups - 1 )
			{ //This is the error that is being targetted.	This means that the merc exists in multiple groups though the merc
				//knows what group he is supposed to be in.	This error can be corrected, by manually removing the merc from all
				//mismatch groups.	This is indicative of a merc being reassigned to another group without removing him first.
				RemovePlayersFromAllMismatchGroups( pSoldier );
				pSoldier->ubGroupID = ubGroupID;
			}
			else if( !iGroups )
			{ //The merc cannot be found in any group!	This should never happen!	We will assign the merc into his
				//own unique squad as a correction.
				AddCharacterToUniqueSquad( pSoldier );
			}
		}
	}
	if( iNumErrors )
	{ //The first error to be detected is the one responsible for building the strings.	We will simply append another string containing
		//the total number of detected errors.
		CHAR16 tempstr[ 128 ];
		swprintf( tempstr, L" A total of %d related errors have been detected.", iNumErrors );
		wcscat( str, tempstr );
		SAIReportError( str );
	}
}
#endif

#ifdef JA2BETAVERSION
void SAIReportError( STR16 wErrorString )
{
	// runtime static only, don't save
	#ifdef JA2TESTVERSION
		static BOOLEAN fReportedAlready = FALSE;
	#else
		BOOLEAN fReportedAlready = FALSE; //so it can loop
	#endif

	if ( !fReportedAlready )
	{
		StopTimeCompression();

		// report the error
		if( guiCurrentScreen != SAVE_LOAD_SCREEN )
		{
			DoScreenIndependantMessageBox( wErrorString, MSG_BOX_FLAG_OK, NULL );
		}
		else
		{
			ScreenMsg( FONT_LTBLUE, MSG_BETAVERSION, wErrorString );
		}
		if( guiCurrentScreen == AIVIEWER_SCREEN )
		{
			CHAR8 str[ 512 ];
			sprintf( str, "%S\n", wErrorString );
			OutputDebugString( (LPCSTR) str );
		}

		// this should keep it from repeating endlessly and allow player to save/bail
		fReportedAlready = TRUE;
	}
}
#endif



void InitStrategicAI()
{
	INT32 i, cnt, iRandom;
	INT32 iEliteChance, iTroopChance, iAdminChance;
	INT32 iWeight;
	INT32 iStartPop, iDesiredPop, iPriority;
	SECTORINFO *pSector = NULL;
	GROUP *pGroup;
	UINT8 ubNumTroops;
	UINT8 ubNumRobots, ubNumTanks, ubNumJeeps;
	INT32 iPercentElitesBonus;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic3");

	//Initialize the basic variables.

	gbPadding2[0]						= 0;
	gbPadding2[1]						= 0;
	gbPadding2[2]						= 0;
	gfExtraElites						= FALSE;
	//giGarrisonArraySize			= 0;
	//giPatrolArraySize				= 0;
	giForcePercentage				= 0;
	giArmyAlertness					= 0;
	giArmyAlertnessDecay		= 0;
	gubNumAwareBattles			= 0;
	gfQueenAIAwake					= FALSE;
	giReinforcementPool			= 0;
	giReinforcementPoints		= 0;
	giRequestPoints					= 0;
	gubSAIVersion						= SAI_VERSION;
	gubQueenPriorityPhase		= 0;
	gfFirstBattleMeanwhileScenePending = FALSE;
	gfMassFortificationOrdered = FALSE;
	gubMinEnemyGroupSize		= 0;
	gubHoursGracePeriod			= 0;
	gusPlayerBattleVictories = 0;
	gfUseAlternateQueenPosition = FALSE;

	gfAggressiveQueen = FALSE;
	gfUnlimitedTroops = FALSE;

	#ifdef JA2BETAVERSION
		ClearStrategicLog();
	#endif

	// Flugente: enemy generals can speed up the AI decision process (justification: the generals are the ones planning the military operations, so if theay are around, the army is more efficient)
	FLOAT dEnemyGeneralsSpeedupFactor = 1.0f;
	if ( gGameExternalOptions.fEnemyRoles && gGameExternalOptions.fEnemyGenerals )
	{
		dEnemyGeneralsSpeedupFactor = max( 0.5f, dEnemyGeneralsSpeedupFactor - gStrategicStatus.usVIPsLeft * gGameExternalOptions.fEnemyGeneralStrategicDecisionSpeedBonus );
	}

	giReinforcementPool		= zDiffSetting[gGameOptions.ubDifficultyLevel].iQueensInitialPoolOfTroops; 
	giForcePercentage		= zDiffSetting[gGameOptions.ubDifficultyLevel].iInitialGarrisonPercentages; 
	giArmyAlertness			= zDiffSetting[gGameOptions.ubDifficultyLevel].iEnemyStartingAlertLevel; 
	giArmyAlertnessDecay	= zDiffSetting[gGameOptions.ubDifficultyLevel].iEnemyAlertDecay; 
	gubMinEnemyGroupSize	= zDiffSetting[gGameOptions.ubDifficultyLevel].iMinEnemyGroupSize; 
	gubHoursGracePeriod		= zDiffSetting[gGameOptions.ubDifficultyLevel].iGracePeriodInHoursAfterSectorLiberation; 
	gfUnlimitedTroops		= zDiffSetting[gGameOptions.ubDifficultyLevel].bUnlimitedPoolOfTroops; 
	gfAggressiveQueen		= zDiffSetting[gGameOptions.ubDifficultyLevel].bAggressiveQueenAi;

	// 475 is 7:55am in minutes since midnight, the time the game starts on day 1
	AddStrategicEvent( EVENT_EVALUATE_QUEEN_SITUATION, 475 + dEnemyGeneralsSpeedupFactor * (zDiffSetting[gGameOptions.ubDifficultyLevel].iBaseDelayInMinutesBetweenEvaluations + Random( zDiffSetting[gGameOptions.ubDifficultyLevel].iEvaluationDelayVariance )), 0 );
	
	//Initialize the sectorinfo structure so all sectors don't point to a garrisonID.
	for( i = 0; i <= 255; ++i )
	{
		SectorInfo[ i ].ubGarrisonID = NO_GARRISON;
	}

	//copy over the original army composition as it does get modified during the campaign.	This
	//bulletproofs starting the game over again.
	memcpy( gArmyComp, gOrigArmyComp, sizeof( gArmyComp ) );

	//Eliminate more perimeter defenses on the easier levels.
	if (zDiffSetting[gGameOptions.ubDifficultyLevel].iDesiredPopulationL2 == FALSE) 
	{
		gArmyComp[ LEVEL2_DEFENCE ].bDesiredPopulation = 0;
		gArmyComp[ LEVEL2_DEFENCE ].bStartPopulation = 0;
	}
	
	if (zDiffSetting[gGameOptions.ubDifficultyLevel].iDesiredPopulationL3 == FALSE) 
	{
		gArmyComp[ LEVEL3_DEFENCE ].bDesiredPopulation = 0;
		gArmyComp[ LEVEL3_DEFENCE ].bStartPopulation = 0;
	}
	
	iPercentElitesBonus = zDiffSetting[gGameOptions.ubDifficultyLevel].iPercentElitesBonus;

	// if we allow additional elites
	if ( iPercentElitesBonus > 0 )
	{
		for( i = 0; i < NUM_ARMY_COMPOSITIONS; ++i )
		{
			if ( i != OMERTA_WELCOME_WAGON )
			{
				gArmyComp[i].bElitePercentage = min(100, gArmyComp[i].bElitePercentage + iPercentElitesBonus);
				if (gArmyComp[i].bTroopPercentage >= iPercentElitesBonus)
				{
					gArmyComp[i].bTroopPercentage = gArmyComp[i].bTroopPercentage - iPercentElitesBonus;
				}
				else
				{
					INT8 diff = iPercentElitesBonus - gArmyComp[i].bTroopPercentage;
					gArmyComp[i].bTroopPercentage = 0;
					gArmyComp[i].bAdminPercentage = max(0, gArmyComp[i].bAdminPercentage - diff);
				}
			}
		}
	}

	/*
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			gArmyComp[ LEVEL2_DEFENCE ].bDesiredPopulation = 0;
			gArmyComp[ LEVEL2_DEFENCE ].bStartPopulation = 0;
			gArmyComp[ LEVEL3_DEFENCE ].bDesiredPopulation = 0;
			gArmyComp[ LEVEL3_DEFENCE ].bStartPopulation = 0;
			break;
			// Kaiden: original Experienced troop compositions:
		//case DIF_LEVEL_MEDIUM:
			//gArmyComp[ LEVEL3_DEFENCE ].bDesiredPopulation = 0;
			//gArmyComp[ LEVEL3_DEFENCE ].bStartPopulation = 0;
			//break;

		//Kaiden: Added to allow an elite bonus for experienced.
		//Kaiden: Fixed to add the old way of doing this if the
		//bonus is set to 0 percent.
		case DIF_LEVEL_MEDIUM:
			if (gGameExternalOptions.iPercentElitesBonusExperienced == 0)
			{
				gArmyComp[ LEVEL3_DEFENCE ].bDesiredPopulation = 0;
				gArmyComp[ LEVEL3_DEFENCE ].bStartPopulation = 0;
			}
			else
			{
				for( i = 0; i < NUM_ARMY_COMPOSITIONS; i++ )
				{
					if ( i != OMERTA_WELCOME_WAGON )
					{
						iPercentElitesBonus = gGameExternalOptions.iPercentElitesBonusExperienced;
						gArmyComp[ i ].bElitePercentage = min(100,gArmyComp[ i ].bElitePercentage + iPercentElitesBonus);
						gArmyComp[ i ].bTroopPercentage = max(0,gArmyComp[ i ].bTroopPercentage - iPercentElitesBonus);
						gArmyComp[ i ].bAdminPercentage = 0;
					}
				}
			}
			break;
		case DIF_LEVEL_HARD:
			for( i = 0; i < NUM_ARMY_COMPOSITIONS; i++ )
			{
				if ( i != OMERTA_WELCOME_WAGON )
				{
					iPercentElitesBonus = gGameExternalOptions.iPercentElitesBonusExpert;
					gArmyComp[ i ].bElitePercentage = min(100,gArmyComp[ i ].bElitePercentage + iPercentElitesBonus);
					gArmyComp[ i ].bTroopPercentage = max(0,gArmyComp[ i ].bTroopPercentage - iPercentElitesBonus);
					gArmyComp[ i ].bAdminPercentage = 0;
				}
			}
			break;
		case DIF_LEVEL_INSANE:
			for( i = 0; i < NUM_ARMY_COMPOSITIONS; i++ )
			{
				if ( i != OMERTA_WELCOME_WAGON )
				{
					iPercentElitesBonus = gGameExternalOptions.iPercentElitesBonusInsane;
					gArmyComp[ i ].bElitePercentage = min(100,gArmyComp[ i ].bElitePercentage + iPercentElitesBonus);
					gArmyComp[ i ].bTroopPercentage = max(0,gArmyComp[ i ].bTroopPercentage - iPercentElitesBonus);
					gArmyComp[ i ].bAdminPercentage = 0;
				}
			}
			break;
	}
	*/
	
	//initialize the patrol group definitions
	giPatrolArraySize = iOrigPatrolArraySize;
	if( gPatrolGroup )
	{
		MemFree( gPatrolGroup );
	}
	gPatrolGroup = (PATROL_GROUP*)MemAlloc( sizeof( gOrigPatrolGroup ) );
	Assert( gPatrolGroup );
	memcpy( gPatrolGroup, gOrigPatrolGroup, sizeof( gOrigPatrolGroup ) );

	if( gubPatrolReinforcementsDenied )
	{
		MemFree( gubPatrolReinforcementsDenied );
	}
	gubPatrolReinforcementsDenied = (UINT8*)MemAlloc( giPatrolArraySize );
	memset( gubPatrolReinforcementsDenied, 0, giPatrolArraySize );

	//initialize the garrison group definitions
	giGarrisonArraySize = iOrigGarrisonArraySize;
	if( gGarrisonGroup )
	{
		MemFree( gGarrisonGroup );
	}
	gGarrisonGroup = (GARRISON_GROUP*)MemAlloc( sizeof( gOrigGarrisonGroup ) );
	Assert( gGarrisonGroup );
	memcpy( gGarrisonGroup, gOrigGarrisonGroup, sizeof( gOrigGarrisonGroup ) );

	if ( gubGarrisonReinforcementsDenied )
	{
		MemFree( gubGarrisonReinforcementsDenied );
	}
	gubGarrisonReinforcementsDenied = (UINT8*)MemAlloc( giGarrisonArraySize );
	memset( gubGarrisonReinforcementsDenied, 0, giGarrisonArraySize );

	//Modify initial force sizes?
	if( giForcePercentage != 100 )
	{ //The initial force sizes are being modified, so go through each of the army compositions
		//and adjust them accordingly.
		for( i = 0; i < NUM_ARMY_COMPOSITIONS; i++ )
		{
			if( i != QUEEN_DEFENCE )
			{
				gArmyComp[ i ].bDesiredPopulation = (INT8)min( iMaxEnemyGroupSize, (gArmyComp[ i ].bDesiredPopulation * giForcePercentage / 100) );
				if( gArmyComp[ i ].bStartPopulation != iMaxEnemyGroupSize )
				{ //if the value is MAX_STRATEGIC_TEAM_SIZE, then that means the particular sector is a spawning location.
					//Don't modify the value if it is MAX_STRATEGIC_TEAM_SIZE.	Everything else is game.
					gArmyComp[ i ].bStartPopulation = (INT8)min( iMaxEnemyGroupSize, (gArmyComp[ i ].bStartPopulation * giForcePercentage / 100) );
				}
			}
			else
			{
				gArmyComp[ i ].bDesiredPopulation = (INT8)min( 32, (gArmyComp[ i ].bDesiredPopulation * giForcePercentage / 100) );
				gArmyComp[ i ].bStartPopulation = gArmyComp[ i ].bDesiredPopulation;
			}
		}
		for( i = 0; i < giPatrolArraySize; i++ )
		{ //force modified range within 1-MAX_STRATEGIC_TEAM_SIZE.
			gPatrolGroup[ i ].bSize = (INT8)max( gubMinEnemyGroupSize, min( iMaxEnemyGroupSize, (gPatrolGroup[ i ].bSize * giForcePercentage / 100 ) ) );
		}
	}


	//Now, initialize the garrisons based on the initial sizes (all variances are plus or minus 1).
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		pSector = &SectorInfo[ gGarrisonGroup[ i ].ubSectorID ];
		pSector->ubGarrisonID = (UINT8)i;
		iStartPop = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bStartPopulation;
		iDesiredPop = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bDesiredPopulation;
		iPriority = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bPriority;
		iEliteChance = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bElitePercentage;
		iTroopChance = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bTroopPercentage + iEliteChance;
		iAdminChance = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bAdminPercentage;

		switch( gGarrisonGroup[ i ].ubComposition )
		{
			case ROADBLOCK:
				pSector->uiFlags |= SF_ENEMY_AMBUSH_LOCATION;
				if( Chance( 20 ) )
					iStartPop = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bDesiredPopulation;
				else
					iStartPop = 0;
				break;
			case SANMONA_SMALL:
				iStartPop = 0; //not appropriate until Kingpin is killed.
				break;
		}

		if( iStartPop )
		{
			if( gGarrisonGroup[ i ].ubSectorID != SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) )
			{
				// if population is less than maximum
				if( iStartPop != iMaxEnemyGroupSize )
				{
					// then vary it a bit (+/- 25%)
					iStartPop = iStartPop * ( 100 + ( Random ( 51 ) - 25 ) ) / 100;
				}

				iStartPop = max( gubMinEnemyGroupSize, min( iMaxEnemyGroupSize, iStartPop ) );
			}
			cnt = iStartPop;

			if( iAdminChance )
			{
				pSector->ubNumAdmins = iAdminChance * iStartPop / 100;
				cnt -= pSector->ubNumAdmins;
			}
			
			while( cnt-- )
			{ //for each person, randomly determine the types of each soldier.
				{
					iRandom = Random( 100 );
					if( iRandom < iEliteChance )
					{
						pSector->ubNumElites++;
					}
					else if( iRandom < iTroopChance )
					{
						pSector->ubNumTroops++;
					}
				}
			}

			switch( gGarrisonGroup[ i ].ubComposition )
			{
				case CAMBRIA_DEFENCE:
				case CAMBRIA_MINE:
				case ALMA_MINE:
				case GRUMM_MINE:
					//Fill up extra start slots with troops
					pSector->ubNumTroops = (UINT8)(iStartPop -= pSector->ubNumAdmins);
					break;
				case DRASSEN_AIRPORT:
				case DRASSEN_DEFENCE:
				case DRASSEN_MINE:
					pSector->ubNumAdmins = (UINT8)max( 5, pSector->ubNumAdmins );
					break;
				case TIXA_PRISON:
					pSector->ubNumAdmins = (UINT8)max( 8, pSector->ubNumAdmins );
					if (gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsRobots)
						pSector->ubNumRobots = (UINT8)max( Random(gGameOptions.ubDifficultyLevel - 1), pSector->ubNumRobots );
					break;
				case QUEEN_DEFENCE:
				case MEDUNA_DEFENCE:
				case MEDUNA_SAMSITE:
					if (gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsRobots)
						pSector->ubNumRobots = (UINT8)max( 1 + Random(gGameOptions.ubDifficultyLevel), pSector->ubNumRobots );
					break;
				case ORTA_DEFENCE:
				case LEVEL1_DEFENCE:
				case LEVEL2_DEFENCE:
				case LEVEL3_DEFENCE:
					if (gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsRobots)
						pSector->ubNumRobots = (UINT8)max( Random(gGameOptions.ubDifficultyLevel), pSector->ubNumRobots );
					break;

			}
		}
		
		// fill up deficit with admins if total enemies less than min enemy group size
		if ( pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps < gubMinEnemyGroupSize )
		{
			pSector->ubNumAdmins = gubMinEnemyGroupSize - pSector->ubNumTroops - pSector->ubNumElites - pSector->ubNumRobots - pSector->ubNumTanks - pSector->ubNumJeeps;
		}

		//Calculate weight (range is -20 to +20 before multiplier).
		//The multiplier of 3 brings it to a range of -96 to +96 which is
		//close enough to a plus/minus 100%.	The resultant percentage is then
		//converted based on the priority.
		iWeight = (iDesiredPop - iStartPop) * 3;
		if( iWeight > 0 )
		{ //modify it by it's priority.
			//generates a value between 2 and 100
			iWeight = iWeight * iPriority / 96;
			iWeight = max( iWeight, 2 );
			giRequestPoints += iWeight;
		}
		else if( iWeight < 0 )
		{ //modify it by it's reverse priority
			//generates a value between -2 and -100
			iWeight = iWeight * (100-iPriority) / 96;
			iWeight = min( iWeight, -2 );
			giReinforcementPoints -= iWeight;
		}
		gGarrisonGroup[ i ].bWeight = (INT8)iWeight;

		//Now post an event which allows them to check adjacent sectors periodically.
		//Spread them out so that they process at different times.
			UINT32 val;
			if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
				val = 1;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
				val = 2;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
				val = 3;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
				val = 4;
			else
			{
				val = Random( 4 );
				if (val == 0) val = 1;
			}
				
		AddPeriodStrategicEventWithOffset( EVENT_CHECK_ENEMY_CONTROLLED_SECTOR, 140 - 20 * val + Random( 4 ), 475 + i, gGarrisonGroup[ i ].ubSectorID );
		//AddPeriodStrategicEventWithOffset( EVENT_CHECK_ENEMY_CONTROLLED_SECTOR, 140 - 20 * gGameOptions.ubDifficultyLevel + Random( 4 ), 475 + i, gGarrisonGroup[ i ].ubSectorID );

	}

	//Now, initialize each of the patrol groups
	for( i = 0; i < giPatrolArraySize; ++i )
	{	// IGNORE COMMENT, FEATURE REMOVED!
		//Some of the patrol groups aren't there at the beginning of the game.	This is
		//based on the difficulty settings in the above patrol table.
		//if( gPatrolGroup[ i ].ubUNUSEDStartIfDifficulty <= gGameOptions.ubDifficultyLevel )
		{
			UINT8 troopCount = (UINT8)(gPatrolGroup[ i ].bSize + Random( 3 ) - 1);
			troopCount = (UINT8)max( gubMinEnemyGroupSize, min( iMaxEnemyGroupSize, troopCount) );
			UINT8 ubNumElites;
			InitializeGroup(GROUP_TYPE_PATROL, troopCount, ubNumTroops, ubNumElites, ubNumRobots, ubNumJeeps, ubNumTanks, Random(10) < gGameOptions.ubDifficultyLevel && i != 3 && i != 4);

			//Note on adding patrol groups...
			//The patrol group can't actually start on the first waypoint, so we set it to the second way
			//point for initialization, and then add the waypoints from 0 up
			pGroup = CreateNewEnemyGroupDepartingFromSector( gPatrolGroup[ i ].ubSectorID[ 1 ], 0, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps );
			Assert( pGroup );

			if( i == 3 || i == 4 )
			{ //Special case:	Two patrol groups are administrator groups -- rest are troops
				pGroup->pEnemyGroup->ubNumAdmins = pGroup->pEnemyGroup->ubNumTroops;
				pGroup->pEnemyGroup->ubNumTroops = 0;
			}
			gPatrolGroup[ i ].ubGroupID = pGroup->ubGroupID;
			pGroup->pEnemyGroup->ubIntention = PATROL;
			pGroup->ubMoveType = ENDTOEND_FORWARDS;
			AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 0 ] );
			AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 1 ] );
			if( gPatrolGroup[ i ].ubSectorID[ 2 ] )
			{ //Add optional waypoints if included.
				AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 2 ] );
				if( gPatrolGroup[ i ].ubSectorID[ 3 ] )
					AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 3 ] );
			}
			RandomizePatrolGroupLocation( pGroup );
			ValidateGroup( pGroup );
		}
		//else
		//{ //we aren't creating this patrol group at the beginning of the game, so we
			//need to set up the weighting values to prioritize it's reinforcement request so that
			//it gets filled up later in the game.
		//	iWeight = gPatrolGroup[ i ].bSize * 3 * gPatrolGroup[ i ].bPriority / 96;
		//	gPatrolGroup[ i ].bWeight = (INT8)iWeight;
		//	giRequestPoints += iWeight;
		//}
	}

	// Lesh: new declaration of sam sites
	for (i=0; i < NUMBER_OF_SAMS; i++)
	{
		SectorInfo[ pSamList[i] ].uiFlags |= SF_SAM_SITE;
	}

	//Setup the flags for the four sam sites.
	//SectorInfo[SEC_D2].uiFlags |= SF_SAM_SITE;
	//SectorInfo[SEC_D15].uiFlags |= SF_SAM_SITE;
	//SectorInfo[SEC_I8].uiFlags |= SF_SAM_SITE;
	//SectorInfo[SEC_N4].uiFlags |= SF_SAM_SITE;

	//final thing to do is choose 1 cache map out of 5 possible maps.	Simply select the sector randomly,
	//set up the flags to use the alternate map, then place 8-12 regular troops there (no ai though).
	//changing MAX_STRATEGIC_TEAM_SIZE may require changes to to the defending force here.
	if ( !gGameExternalOptions.fEnableAllWeaponCaches ) // SANDRO - weapon caches settings changed
	{
		// added a chance to have more than one weapon caches in game
		INT8 ubPicked[2] = { 0,0 };
		UINT8 cnt, cnt2, ubNumberCaches, ubSector;
		
		ubNumberCaches = ( Chance( 50 ) ? (Chance( 25 ) ?  3 : 2 ) : 1);
		cnt2 = 0;

		for (cnt = 0; cnt < ubNumberCaches; cnt++)
		{
			ubSector = (1 + Random ( 5 ));
			switch( ubSector )
			{
				case 1:	
					if( ubPicked[0] != ubSector && ubPicked[1] != ubSector )
					{
						pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache1X, gModSettings.ubWeaponCache1Y ) ]; //SEC_E11
						pSector->uiFlags |= SF_USE_ALTERNATE_MAP;					
						pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops1 * 2);	
						if (ubPicked[0] == 0)
							ubPicked[0] = ubSector;
						else if (ubPicked[1] == 0)
							ubPicked[1] = ubSector; 
						else 
							cnt2 += 25;
					}
					else 
					{
						cnt--; 
					}
					break;
				case 2:	
					if( ubPicked[0] != ubSector && ubPicked[1] != ubSector )
					{
						pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache2X, gModSettings.ubWeaponCache2Y ) ]; //SEC_H5
						pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
						pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops2 * 2);
						if (ubPicked[0] == 0)
							ubPicked[0] = ubSector;
						else if (ubPicked[1] == 0)
							ubPicked[1] = ubSector; 
						else 
							cnt2 += 25;
					}
					else 
					{
						cnt--; 
					}
					break;
				case 3:	
					if( ubPicked[0] != ubSector && ubPicked[1] != ubSector )
					{
						pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache3X, gModSettings.ubWeaponCache3Y ) ]; //SEC_H10
						pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
						pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops3 * 2);
						if (ubPicked[0] == 0)
							ubPicked[0] = ubSector;
						else if (ubPicked[1] == 0)
							ubPicked[1] = ubSector; 
						else 
							cnt2 += 25;
					}
					else 
					{
						cnt--; 
					}
					break;
				case 4:	
					if( ubPicked[0] != ubSector && ubPicked[1] != ubSector )
					{
						pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache4X, gModSettings.ubWeaponCache4Y ) ]; //SEC_J12
						pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
						pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops4 * 2);
						if (ubPicked[0] == 0)
							ubPicked[0] = ubSector;
						else if (ubPicked[1] == 0)
							ubPicked[1] = ubSector; 
						else 
							cnt2 += 25;
					}
					else 
					{
						cnt--; 
					}
					break;
				case 5:	
					if( ubPicked[0] != ubSector && ubPicked[1] != ubSector )
					{
						pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache5X, gModSettings.ubWeaponCache5Y ) ]; //SEC_M9
						pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
						pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops5 * 2);
						if (ubPicked[0] == 0)
							ubPicked[0] = ubSector;
						else if (ubPicked[1] == 0)
							ubPicked[1] = ubSector; 
						else 
							cnt2 += 25;
					}
					else 
					{
						cnt--; 
					}
					break;
			}

			cnt2++;
			if (cnt2 > 25)
				break;
		}
	}
	else
	{
		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache1X, gModSettings.ubWeaponCache1Y ) ]; //SEC_E11
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops1 * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache2X, gModSettings.ubWeaponCache2Y ) ]; //SEC_H5
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops2 * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache3X, gModSettings.ubWeaponCache3Y ) ]; //SEC_H10
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops3 * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache4X, gModSettings.ubWeaponCache4Y ) ]; //SEC_J12
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops4 * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache5X, gModSettings.ubWeaponCache5Y ) ]; //SEC_M9
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + zDiffSetting[gGameOptions.ubDifficultyLevel].iWeaponCacheTroops5 * 2);		
		
		/*
		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache1X, gModSettings.ubWeaponCache1Y ) ]; //SEC_E11
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache2X, gModSettings.ubWeaponCache2Y ) ]; //SEC_H5
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache3X, gModSettings.ubWeaponCache3Y ) ]; //SEC_H10
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache4X, gModSettings.ubWeaponCache4Y ) ]; //SEC_J12
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2);

		pSector = &SectorInfo[ SECTOR( gModSettings.ubWeaponCache5X, gModSettings.ubWeaponCache5Y ) ]; //SEC_M9
		pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		pSector->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2);
		*/
	}

	ValidateWeights( 1 );
}

void KillStrategicAI()
{
	if( gPatrolGroup )
	{
		MemFree( gPatrolGroup );
		gPatrolGroup = NULL;
		giPatrolArraySize = 0;
	}
	if( gGarrisonGroup )
	{
		MemFree( gGarrisonGroup );
		gGarrisonGroup = NULL;
		giGarrisonArraySize = 0;
	}
	if( gubPatrolReinforcementsDenied )
	{
		MemFree( gubPatrolReinforcementsDenied );
		gubPatrolReinforcementsDenied = NULL;
	}
	if( gubGarrisonReinforcementsDenied )
	{
		MemFree( gubGarrisonReinforcementsDenied );
		gubGarrisonReinforcementsDenied = NULL;
	}
	DeleteAllStrategicEventsOfType( EVENT_EVALUATE_QUEEN_SITUATION );
}

BOOLEAN OkayForEnemyToMoveThroughSector( UINT8 ubSectorID )
{
	SECTORINFO *pSector;
	pSector = &SectorInfo[ ubSectorID ];
	if( pSector->uiTimeLastPlayerLiberated && pSector->uiTimeLastPlayerLiberated + (gubHoursGracePeriod * 3600) > GetWorldTotalSeconds() )
	{
		return FALSE;
	}
	return TRUE;
}

BOOLEAN EnemyPermittedToAttackSector( GROUP **pGroup, UINT8 ubSectorID )
{
	SECTORINFO *pSector;
	BOOLEAN fPermittedToAttack = TRUE;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	pSector = &SectorInfo[ ubSectorID ];
	fPermittedToAttack = OkayForEnemyToMoveThroughSector( ubSectorID );
	if( pGroup && *pGroup && pSector->ubGarrisonID != NO_GARRISON )
	{
		if( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
		{
			GROUP *pPendingGroup;
			pPendingGroup = GetGroup( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID );
			if( pPendingGroup == *pGroup )
			{
				if( fPermittedToAttack )
				{
					if( GroupAtFinalDestination( *pGroup ) )
					{ //High priority reinforcements have arrived.	This overrides most other situations.
						return TRUE;
					}
				}
				else
				{ //Reassign the group
					ReassignAIGroup( pGroup );
				}
			}
		}
	}
	if( !fPermittedToAttack )
	{
		return FALSE;
	}
	//If Hill-billies are alive, then enemy won't attack the sector.
	switch( ubSectorID )
	{
		case SEC_F10:
			//Hill-billy farm -- not until hill billies are dead.
			if( CheckFact( 273, FALSE ) )
				return FALSE;
			break;

		//Lalien: deactivated
		//case SEC_A9:
		//case SEC_A10:
		//	//Omerta -- not until Day 2 at 7:45AM.
		//	if( GetWorldTotalMin() < 3345 )
		//		return FALSE;
		//	break;
		//case SEC_B13:
		//case SEC_C13:
		//case SEC_D13:
		//	//Drassen -- not until Day 3 at 6:30AM.
		//	if( GetWorldTotalMin() < 4710 )
		//		return FALSE;
		//	break;

		case SEC_C5:
		case SEC_C6:
		case SEC_D5:
			//San Mona -- not until Kingpin is dead.
			if ( CheckFact( FACT_KINGPIN_DEAD, 0 ) == FALSE )
				return FALSE;
		case SEC_G1:
			if( PlayerSectorDefended( SEC_G2 ) && (PlayerSectorDefended( SEC_H1 ) || PlayerSectorDefended( SEC_H2 )) )
			{
				return FALSE;
			}
			break;
		case SEC_H2:
			if( PlayerSectorDefended( SEC_H2 ) && (PlayerSectorDefended( SEC_G1 ) || PlayerSectorDefended( SEC_G2 )) )
			{
				return FALSE;
			}
			break;
	}
	return TRUE;
}

BOOLEAN HandlePlayerGroupNoticedByPatrolGroup( GROUP *pPlayerGroup, GROUP *pEnemyGroup )
{
	UINT16 usDefencePoints;
	UINT16 usOffensePoints;
	UINT8 ubSectorID;

	ubSectorID = (BOOLEAN)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY );
	usOffensePoints = GetOffensePoints(pEnemyGroup->pEnemyGroup);
	if( PlayerForceTooStrong( ubSectorID, usOffensePoints, &usDefencePoints ) )
	{
		RequestAttackOnSector( ubSectorID, usDefencePoints );
		return FALSE;
	}
	//For now, automatically attack.
	if( pPlayerGroup->ubNextX )
	{
		MoveSAIGroupToSector( &pEnemyGroup, (UINT8)SECTOR( pPlayerGroup->ubNextX, pPlayerGroup->ubNextY ), DIRECT, PURSUIT );

#ifdef JA2BETAVERSION
		if ( pEnemyGroup )
			LogStrategicEvent( "Enemy group at %c%d detected player group at %c%d and is moving to intercept them at %c%d.",
				pEnemyGroup->ubSectorY + 'A' - 1, pEnemyGroup->ubSectorX,
				pPlayerGroup->ubSectorY + 'A' - 1, pPlayerGroup->ubSectorX,
				pPlayerGroup->ubNextY + 'A' - 1, pPlayerGroup->ubNextX );
#endif
	}
	else
	{
		MoveSAIGroupToSector( &pEnemyGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY ), DIRECT, PURSUIT );

#ifdef JA2BETAVERSION
		if ( pEnemyGroup )
			LogStrategicEvent( "Enemy group at %c%d detected player group at %c%d and is moving to intercept them at %c%d.",
				pEnemyGroup->ubSectorY + 'A' - 1, pEnemyGroup->ubSectorX,
				pPlayerGroup->ubSectorY + 'A' - 1, pPlayerGroup->ubSectorX,
				pPlayerGroup->ubSectorY + 'A' - 1, pPlayerGroup->ubSectorX );
#endif
	}
	return TRUE;
}

void HandlePlayerGroupNoticedByGarrison( GROUP *pPlayerGroup, UINT8 ubSectorID )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	INT32 iReinforcementsApproved;
	UINT16 usOffensePoints, usDefencePoints;
	UINT8 ubEnemies;
	pSector = &SectorInfo[ ubSectorID ];
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
 DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic4");

	//First check to see if the player is at his final destination.
	if( !GroupAtFinalDestination( pPlayerGroup ) )
	{
		return;
	}
	usOffensePoints = GetOffensePoints(pSector);
	if( PlayerForceTooStrong( ubSectorID, usOffensePoints, &usDefencePoints ) )
	{
		RequestAttackOnSector( ubSectorID, usDefencePoints );
		return;
	}

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( pSector->ubGarrisonID != NO_GARRISON )
	{
		//Decide whether or not they will attack them with some of the troops.
		ubEnemies = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps);
		if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
			iReinforcementsApproved = (ubEnemies - gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bDesiredPopulation );
		else
			iReinforcementsApproved = (ubEnemies - gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bDesiredPopulation / 2);

		if( iReinforcementsApproved*2 > pPlayerGroup->ubGroupSize*3 && iReinforcementsApproved > gubMinEnemyGroupSize )
		{ //Then enemy's available outnumber the player by at least 3:2, so attack them.
			pGroup = CreateNewEnemyGroupDepartingFromSector( ubSectorID, 0, (UINT8)iReinforcementsApproved, 0, 0, 0, 0 );

			ConvertGroupTroopsToComposition( pGroup, gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition );

			MoveSAIGroupToSector( &pGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY ), DIRECT, REINFORCEMENTS );

			if ( pGroup )
				RemoveSoldiersFromGarrisonBasedOnComposition( pSector->ubGarrisonID, pGroup->ubGroupSize );

			if ( pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumAdmins + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps > iMaxEnemyGroupSize )
			{
				#ifdef JA2BETAVERSION
					LogStrategicEvent( "ERROR:	Sector %c%d now has %d enemies (max %d).",
						gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID / 16 + 'A' , gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID % 16,
						pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumAdmins + pSector->ubNumTanks + pSector->ubNumJeeps + pSector->ubNumRobots, iMaxEnemyGroupSize );
				#endif
			}

			#ifdef JA2BETAVERSION
				LogStrategicEvent( "Enemy garrison at %c%d detected stopped player group at %c%d and is sending %d troops to attack.",
					gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID / 16 + 'A' , gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID % 16,
					pPlayerGroup->ubSectorY + 'A' - 1, pPlayerGroup->ubSectorX,
					pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumTroops );
			#endif
		}
	}
}

BOOLEAN HandleMilitiaNoticedByPatrolGroup( UINT8 ubSectorID, GROUP *pEnemyGroup )
{
	//For now, automatically attack.
	UINT16 usOffensePoints, usDefencePoints;
	UINT8 ubSectorX = SECTORX( ubSectorID );
	UINT8 ubSectorY = SECTORY( ubSectorID );
	usOffensePoints = GetOffensePoints(pEnemyGroup->pEnemyGroup);
	if( PlayerForceTooStrong( ubSectorID, usOffensePoints, &usDefencePoints ) )
	{
		RequestAttackOnSector( ubSectorID, usDefencePoints );
		return FALSE;
	}

	MoveSAIGroupToSector( &pEnemyGroup, (UINT8)SECTOR( ubSectorX, ubSectorY ), DIRECT, REINFORCEMENTS );

#ifdef JA2BETAVERSION
	if ( pEnemyGroup )
		LogStrategicEvent( "Enemy group at %c%d detected militia at %c%d and is moving to attack them.", pEnemyGroup->ubSectorY + 'A' - 1, pEnemyGroup->ubSectorX, ubSectorY + 'A' - 1, ubSectorX );
#endif

	return FALSE;
}

BOOLEAN AttemptToNoticeEmptySectorSucceeds()
{
	if( gubNumAwareBattles || gfAutoAIAware )
	{ //The queen is in high-alert and is searching for players.	All adjacent checks will automatically succeed.
		return TRUE;
	}
	if( DayTime() )
	{ //Day time chances are normal
		if( Chance( giArmyAlertness ) )
		{
			giArmyAlertness -= giArmyAlertnessDecay;
			//Minimum alertness should always be at least 0.
			giArmyAlertness = max( 0, giArmyAlertness );
			return TRUE;
		}
		giArmyAlertness++;
		return FALSE;
	}
	//Night time chances are one third of normal.
	if( Chance( giArmyAlertness/3 ) )
	{
		giArmyAlertness -= giArmyAlertnessDecay;
		//Minimum alertness should always be at least 0.
		giArmyAlertness = max( 0, giArmyAlertness );
		return TRUE;
	}
	if( Chance( 33 ) )
	{
		giArmyAlertness++;
	}
	return FALSE;
}

//Calling the function assumes that a player group is found to be adjacent to an enemy group.
//This uses the alertness rating to emulate the chance that the group will notice.	If it does
//notice, then the alertness drops accordingly to simulate a period of time where the enemy would
//not notice as much.	If it fails, the alertness gradually increases until it succeeds.
BOOLEAN AttemptToNoticeAdjacentGroupSucceeds()
{
	if( gubNumAwareBattles || gfAutoAIAware )
	{ //The queen is in high-alert and is searching for players.	All adjacent checks will automatically succeed.
		return TRUE;
	}
	if( DayTime() )
	{ //Day time chances are normal
		if( Chance( giArmyAlertness ) )
		{
			giArmyAlertness -= giArmyAlertnessDecay;
			//Minimum alertness should always be at least 0.
			giArmyAlertness = max( 0, giArmyAlertness );
			return TRUE;
		}
		giArmyAlertness++;
		return FALSE;
	}
	//Night time chances are one third of normal.
	if( Chance( giArmyAlertness/3 ) )
	{
		giArmyAlertness -= giArmyAlertnessDecay;
		//Minimum alertness should always be at least 0.
		giArmyAlertness = max( 0, giArmyAlertness );
		return TRUE;
	}
	if( Chance( 33 ) )
	{
		giArmyAlertness++;
	}
	return FALSE;
}

BOOLEAN HandleEmptySectorNoticedByPatrolGroup( GROUP *pGroup, UINT8 ubEmptySectorID )
{
	UINT8 ubGarrisonID;
	UINT8 ubSectorX = SECTORX( ubEmptySectorID );
	UINT8 ubSectorY = SECTORY( ubEmptySectorID );

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	ubGarrisonID = SectorInfo[ ubEmptySectorID ].ubGarrisonID;
	if( ubGarrisonID != NO_GARRISON )
	{
		if( gGarrisonGroup[ ubGarrisonID ].ubPendingGroupID )
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	//Clear the patrol group's previous orders.
	ClearPreviousAIGroupAssignment( pGroup );

	gGarrisonGroup[ ubGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
	MoveSAIGroupToSector( &pGroup, (UINT8)SECTOR( ubSectorX, ubSectorY ), DIRECT, REINFORCEMENTS );

#ifdef JA2BETAVERSION
	if ( pGroup )
		LogStrategicEvent( "Enemy group at %c%d detected undefended sector at %c%d and is moving to retake it.", pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX, ubSectorY + 'A' - 1, ubSectorX );
#endif

	return TRUE;
}

void HandleEmptySectorNoticedByGarrison( UINT8 ubGarrisonSectorID, UINT8 ubEmptySectorID )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT8 ubAvailableTroops;
	UINT8 ubSrcGarrisonID = 255, ubDstGarrisonID = 255;

	//Make sure that the destination sector doesn't already have a pending group.
	pSector = &SectorInfo[ ubEmptySectorID ];

	ubSrcGarrisonID = SectorInfo[ ubGarrisonSectorID ].ubGarrisonID;
	ubDstGarrisonID = SectorInfo[ ubEmptySectorID ].ubGarrisonID;

	if( ubSrcGarrisonID == NO_GARRISON || ubDstGarrisonID == NO_GARRISON )
	{ //Bad logic
		return;
	}

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( gGarrisonGroup[ ubDstGarrisonID ].ubPendingGroupID )
	{ //A group is already on-route, so don't send anybody from here.
		return;
	}

	//An opportunity has arisen, where the enemy has noticed an important sector that is undefended.
	pSector = &SectorInfo[ ubGarrisonSectorID ];
	ubAvailableTroops = pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumAdmins + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;

	if( ubAvailableTroops >= gubMinEnemyGroupSize * 2 )
	{ //split group into two groups, and move one of the groups to the next sector.
		pGroup = CreateNewEnemyGroupDepartingFromSector( ubGarrisonSectorID, 0, (UINT8)(ubAvailableTroops / 2), 0, 0, 0, 0 );
		ConvertGroupTroopsToComposition( pGroup, gGarrisonGroup[ ubDstGarrisonID ].ubComposition );
		RemoveSoldiersFromGarrisonBasedOnComposition( ubSrcGarrisonID, pGroup->ubGroupSize );
		gGarrisonGroup[ ubDstGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
		MoveSAIGroupToSector( &pGroup, ubEmptySectorID, DIRECT, REINFORCEMENTS );
	}
}

BOOLEAN ReinforcementsApproved( INT32 iGarrisonID, UINT16 *pusDefencePoints )
{
	SECTORINFO *pSector;
	UINT16 usOffensePoints;
	UINT16 usDefencePoints;
	UINT8 ubSectorX, ubSectorY;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	ubSectorX = (UINT8)SECTORX( gGarrisonGroup[ iGarrisonID ].ubSectorID );
	ubSectorY = (UINT8)SECTORY( gGarrisonGroup[ iGarrisonID ].ubSectorID );

	// SANDRO - EVALUATE THE MILITIA STRENGTH REGARDING THE GAME SETTINGS
	usOffensePoints = gArmyComp[gGarrisonGroup[iGarrisonID].ubComposition].bAdminPercentage * 2 +
		gArmyComp[gGarrisonGroup[iGarrisonID].ubComposition].bTroopPercentage * 3 +
		gArmyComp[gGarrisonGroup[iGarrisonID].ubComposition].bElitePercentage * 4 +
		gubGarrisonReinforcementsDenied[iGarrisonID];
	usOffensePoints = usOffensePoints * gArmyComp[gGarrisonGroup[iGarrisonID].ubComposition].bDesiredPopulation / 100;

	if ( PlayerForceTooStrong( gGarrisonGroup[iGarrisonID].ubSectorID, usOffensePoints, &usDefencePoints ) )
	{
		*pusDefencePoints = usDefencePoints;

		return TRUE;
	}

	*pusDefencePoints = usDefencePoints;

	//Before returning false, determine if reinforcements have been denied repeatedly.	If so, then
	//we might send an augmented force to take it back.
	if ( gubGarrisonReinforcementsDenied[iGarrisonID] + usOffensePoints > usDefencePoints )
	{
		#ifdef JA2BETAVERSION
			LogStrategicEvent( "Sector %c%d will now recieve an %d extra troops due to multiple denials for reinforcements in the past for strong player presence.",
				ubSectorY + 'A' - 1, ubSectorX, gubGarrisonReinforcementsDenied[ iGarrisonID ] / 3 );
		#endif
		return TRUE;
	}
	//Reinforcements will have to wait.	For now, increase the reinforcements denied.	The amount increase is 20 percent
	//of the garrison's priority.
	gubGarrisonReinforcementsDenied[ iGarrisonID ] += (UINT8)(gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bPriority / 2);
	
	return FALSE;
}

//if the group has arrived in a sector, and doesn't have any particular orders, then
//send him back where they came from.
//RETURNS TRUE if the group is deleted or told to move somewhere else.
//This is important as the calling function will need
//to abort processing of the group for obvious reasons.
BOOLEAN EvaluateGroupSituation( GROUP *pGroup )
{
	SECTORINFO *pSector;
	GROUP *pPatrolGroup;
	INT32 i;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic5");

	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic5");
	ValidateWeights( 2 );

	if( !gfQueenAIAwake )
	{
		return FALSE;
	}
	Assert( pGroup->usGroupTeam != OUR_TEAM );

	// for now, nothing to do for militia groups
	if ( pGroup->usGroupTeam == MILITIA_TEAM )
		return FALSE;

	if( pGroup->pEnemyGroup->ubIntention == PURSUIT )
	{
		//Lost the player group that he was going to attack.	Return to original position.
		SetThisSectorAsEnemyControlled( pGroup->ubSectorX, pGroup->ubSectorY, 0, TRUE );
		ReassignAIGroup( &pGroup );
		return TRUE;
	}
	else if( pGroup->pEnemyGroup->ubIntention == REINFORCEMENTS )
	{
		Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
		//The group has arrived at the location where he is supposed to reinforce.
		//Step 1 -- Check for matching garrison location
		for( i = 0; i < giGarrisonArraySize; i++ )
		{
			if( gGarrisonGroup[ i ].ubSectorID == SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) &&
					gGarrisonGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
			{
				pSector = &SectorInfo[ SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) ];

				if( gGarrisonGroup[ i ].ubSectorID != SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) )
				{
					EliminateSurplusTroopsForGarrison( pGroup, pSector );
					pSector->ubNumAdmins = (UINT8)(pSector->ubNumAdmins + pGroup->pEnemyGroup->ubNumAdmins);
					pSector->ubNumTroops = (UINT8)(pSector->ubNumTroops + pGroup->pEnemyGroup->ubNumTroops);
					pSector->ubNumElites = (UINT8)(pSector->ubNumElites + pGroup->pEnemyGroup->ubNumElites);
					pSector->ubNumRobots = (UINT8)(pSector->ubNumRobots + pGroup->pEnemyGroup->ubNumRobots);
					pSector->ubNumTanks = (UINT8)(pSector->ubNumTanks + pGroup->pEnemyGroup->ubNumTanks);
					pSector->ubNumJeeps = (UINT8)(pSector->ubNumJeeps + pGroup->pEnemyGroup->ubNumJeeps);

#ifdef JA2BETAVERSION
					LogStrategicEvent( "%d reinforcements have arrived to garrison sector %c%d",
						pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTroops +
						pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
#endif

					if( IsThisSectorASAMSector( pGroup->ubSectorX, pGroup->ubSectorY, 0 ) )
					{
						UINT16 stratsector = CALCULATE_STRATEGIC_INDEX( pGroup->ubSectorX, pGroup->ubSectorY );
						if ((StrategicMap[stratsector].usFlags & SAMSITE_REPAIR_ORDERED) == 0 && (StrategicMap[stratsector].bSAMCondition < 100 || StrategicMap[stratsector].sSamHackStatus < 100))
						{
							StrategicMap[stratsector].usFlags |= SAMSITE_REPAIR_ORDERED;
							AddStrategicEvent(EVENT_SAMSITE_REPAIRED, GetWorldTotalMin() + 24 * 60, stratsector);
						}
							
					}
				}
				else
				{ //The group was sent back to the queen's palace (probably because they couldn't be reassigned
					//anywhere else, but it is possible that the queen's sector is requesting the reinforcements.	In
					//any case, if the queen's sector is less than full strength, fill it up first, then
					//simply add the rest to the global pool.
					if( pSector->ubNumElites < iMaxEnemyGroupSize )
					{
						if( pSector->ubNumElites + pGroup->ubGroupSize >= iMaxEnemyGroupSize )
						{ //Fill up the queen's guards, then apply the rest to the reinforcement pool

							//Madd: unlimited reinforcements?, if so never increase/decrease it
							if ( !gfUnlimitedTroops )
								giReinforcementPool += iMaxEnemyGroupSize - pSector->ubNumElites;

							pSector->ubNumElites = iMaxEnemyGroupSize;
							#ifdef JA2BETAVERSION
								LogStrategicEvent( "%d reinforcements have arrived to garrison queen's sector.	The excess troops will be relocated to the reinforcement pool.",
									pGroup->ubGroupSize );
							#endif
						}
						else
						{ //Add all the troops to the queen's guard.
							pSector->ubNumElites += pGroup->ubGroupSize;
							#ifdef JA2BETAVERSION
								LogStrategicEvent( "%d reinforcements have arrived to garrison queen's sector.",
									pGroup->ubGroupSize, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
							#endif
						}
					}
					else
					{ //Add all the troops to the reinforcement pool as the queen's guard is at full strength.
						//Madd: unlimited reinforcements?, if so never increase/decrease it
						if ( !gfUnlimitedTroops )
							giReinforcementPool += pGroup->ubGroupSize;
						#ifdef JA2BETAVERSION
							LogStrategicEvent( "%d reinforcements have arrived at queen's sector and have been added to the reinforcement pool.",
								pGroup->ubGroupSize, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
						#endif
					}
				}

				SetThisSectorAsEnemyControlled( pGroup->ubSectorX, pGroup->ubSectorY, 0, TRUE );
				RemovePGroup( pGroup );
				RecalculateGarrisonWeight( i );
				
				return TRUE;
			}
		}
		//Step 2 -- Check for Patrol groups matching waypoint index.
		for( i = 0; i < giPatrolArraySize; i++ )
		{
			if( gPatrolGroup[ i ].ubSectorID[ 1 ] == SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) &&
					gPatrolGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
			{
				gPatrolGroup[ i ].ubPendingGroupID = 0;
				if( gPatrolGroup[ i ].ubGroupID && gPatrolGroup[ i ].ubGroupID != pGroup->ubGroupID )
				{ //cheat, and warp our reinforcements to them!
					pPatrolGroup = GetGroup( gPatrolGroup[ i ].ubGroupID );
					pPatrolGroup->pEnemyGroup->ubNumTroops += pGroup->pEnemyGroup->ubNumTroops;
					pPatrolGroup->pEnemyGroup->ubNumElites += pGroup->pEnemyGroup->ubNumElites;
					pPatrolGroup->pEnemyGroup->ubNumAdmins += pGroup->pEnemyGroup->ubNumAdmins;
					pPatrolGroup->pEnemyGroup->ubNumRobots += pGroup->pEnemyGroup->ubNumRobots;
					pPatrolGroup->pEnemyGroup->ubNumTanks += pGroup->pEnemyGroup->ubNumTanks;
					pPatrolGroup->pEnemyGroup->ubNumJeeps += pGroup->pEnemyGroup->ubNumJeeps;
					pPatrolGroup->ubGroupSize += (UINT8)(pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins 
														  + pGroup->pEnemyGroup->ubNumRobots + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps);
					#ifdef JA2BETAVERSION
						LogStrategicEvent( "%d reinforcements have joined patrol group at sector %c%d (new size: %d)",
										   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots,
							pPatrolGroup->ubSectorY + 'A' - 1, pPatrolGroup->ubSectorX, pPatrolGroup->ubGroupSize );
					#endif
					if( pPatrolGroup->ubGroupSize > iMaxEnemyGroupSize )
					{
						UINT8 ubCut;
						#ifdef JA2BETAVERSION
						CHAR16 str[512];
						swprintf( str, L"Patrol group #%d in %c%d received too many reinforcements from group #%d that was created in %c%d. Size truncated from %d to %d."
													L"Please send Strategic Decisions.txt and PRIOR save.",
													pPatrolGroup->ubGroupID, pPatrolGroup->ubSectorY + 'A' - 1, pPatrolGroup->ubSectorX,
													pGroup->ubGroupID, SECTORY( pGroup->ubCreatedSectorID ) + 'A' - 1, SECTORX( pGroup->ubCreatedSectorID ),
													pPatrolGroup->ubGroupSize, iMaxEnemyGroupSize );
						SAIReportError( str );
						#endif
						//truncate the group size.
						ubCut = pPatrolGroup->ubGroupSize - iMaxEnemyGroupSize;
						while( ubCut-- )
						{
							if( pGroup->pEnemyGroup->ubNumAdmins )
							{
								pGroup->pEnemyGroup->ubNumAdmins--;
								pPatrolGroup->pEnemyGroup->ubNumAdmins--;
							}
							else if( pGroup->pEnemyGroup->ubNumTroops )
							{
								pGroup->pEnemyGroup->ubNumTroops--;
								pPatrolGroup->pEnemyGroup->ubNumTroops--;
							}
							else if( pGroup->pEnemyGroup->ubNumElites )
							{
								pGroup->pEnemyGroup->ubNumElites--;
								pPatrolGroup->pEnemyGroup->ubNumElites--;
							}
							else if( pGroup->pEnemyGroup->ubNumRobots )
							{
								pGroup->pEnemyGroup->ubNumRobots--;
								pPatrolGroup->pEnemyGroup->ubNumRobots--;
							}
							else if( pGroup->pEnemyGroup->ubNumTanks )
							{
								pGroup->pEnemyGroup->ubNumTanks--;
								pPatrolGroup->pEnemyGroup->ubNumTanks--;
							}
							else if ( pGroup->pEnemyGroup->ubNumJeeps )
							{
								pGroup->pEnemyGroup->ubNumJeeps--;
								pPatrolGroup->pEnemyGroup->ubNumJeeps--;
							}
						}
						pPatrolGroup->ubGroupSize = iMaxEnemyGroupSize;
						Assert( pPatrolGroup->pEnemyGroup->ubNumAdmins +
										pPatrolGroup->pEnemyGroup->ubNumTroops +
										pPatrolGroup->pEnemyGroup->ubNumElites +
										pPatrolGroup->pEnemyGroup->ubNumRobots +
										pPatrolGroup->pEnemyGroup->ubNumTanks + 
										pPatrolGroup->pEnemyGroup->ubNumJeeps == iMaxEnemyGroupSize );
					}
					RemovePGroup( pGroup );
					RecalculatePatrolWeight( i );
					ValidateLargeGroup( pPatrolGroup );
				}
				else
				{ //the reinforcements have become the new patrol group (even if same group)
					gPatrolGroup[ i ].ubGroupID = pGroup->ubGroupID;
					pGroup->pEnemyGroup->ubIntention = PATROL;
					pGroup->ubMoveType = ENDTOEND_FORWARDS;
					RemovePGroupWaypoints( pGroup );
					AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 0 ] );
					AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 1 ] );
					if( gPatrolGroup[ i ].ubSectorID[ 2 ] )
					{ //Add optional waypoints if included.
						AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 2 ] );
						if( gPatrolGroup[ i ].ubSectorID[ 3 ] )
							AddWaypointIDToPGroup( pGroup, gPatrolGroup[ i ].ubSectorID[ 3 ] );
					}

					//Otherwise, the engine assumes they are being deployed.
					//pGroup->fWaypointsCancelled = FALSE;

					#ifdef JA2BETAVERSION
						LogStrategicEvent( "%d soldiers have arrived to patrol area near sector %c%d",
										   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots,
							pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
					#endif
					RecalculatePatrolWeight( i );
				}
				return TRUE;
			}
		}
	}
	else
	{	//This is a floating group at his final destination...
		if( pGroup->pEnemyGroup->ubIntention != STAGING && pGroup->pEnemyGroup->ubIntention != REINFORCEMENTS )
		{
			ReassignAIGroup( &pGroup );
			return TRUE;
		}
	}
	ValidateWeights( 3 );
	return FALSE;
}

//returns TRUE if the group was deleted.
BOOLEAN StrategicAILookForAdjacentGroups( GROUP *pGroup )
{
    Assert(pGroup);

	SECTORINFO *pSector;
	GROUP *pEnemyGroup, *pPlayerGroup;
	UINT8 ubNumEnemies;
	UINT8 ubSectorID;
	if( !gfQueenAIAwake )
	{//The queen isn't aware the player's presence yet, so she is oblivious to any situations.

		if ( pGroup->usGroupTeam == ENEMY_TEAM )
		{
			//Exception case!
			//In the beginning of the game, a group is sent to A9 after the first battle.	If you leave A9, when they arrive,
			//they will stay there indefinately because the AI isn't awake.	What we do, is if this is a group in A9, then
			//send them home.
			if( GroupAtFinalDestination( pGroup ) )
			{
				//Wake up the queen now, if she hasn't woken up already.
				WakeUpQueen();
				if( pGroup->ubSectorX == gGameExternalOptions.ubDefaultArrivalSectorX && pGroup->ubSectorY == gGameExternalOptions.ubDefaultArrivalSectorY ||
						pGroup->ubSectorX == gModSettings.ubSAISpawnSectorX && pGroup->ubSectorY == gModSettings.ubSAISpawnSectorY )
				{
					SendGroupToPool( &pGroup );
					if( !pGroup )
					{ //Group was transferred to the pool
						return TRUE;
					}
				}
			}
		}

		if( !gfQueenAIAwake )
		{
			return FALSE;
		}
	}

	if ( pGroup->usGroupTeam == ENEMY_TEAM )
	{
		//The enemy group has arrived at a new sector and now controls it.
		//Look in each of the four directions, and the alertness rating will
		//determine the chance to detect any players that may exist in that sector.
		pEnemyGroup = pGroup;
		if( GroupAtFinalDestination( pEnemyGroup ) )
		{
			return EvaluateGroupSituation( pEnemyGroup );
		}
		ubSectorID = (UINT8)SECTOR( pEnemyGroup->ubSectorX, pEnemyGroup->ubSectorY );

		if( pEnemyGroup && pEnemyGroup->ubSectorY > 1 && EnemyPermittedToAttackSector( &pEnemyGroup, (UINT8)(ubSectorID - 16) ) )
		{
			pPlayerGroup = FindMovementGroupInSector( pEnemyGroup->ubSectorX, (UINT8)(pEnemyGroup->ubSectorY - 1), OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
			}
			else if ( NumNonPlayerTeamMembersInSector( pEnemyGroup->ubSectorX, (UINT8)(pEnemyGroup->ubSectorY - 1), MILITIA_TEAM ) &&
							AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandleMilitiaNoticedByPatrolGroup( (UINT8)SECTOR( pEnemyGroup->ubSectorX, pEnemyGroup->ubSectorY-1 ), pEnemyGroup );
			}
			else if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID-16) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				return HandleEmptySectorNoticedByPatrolGroup( pEnemyGroup, (UINT8)(ubSectorID-16) );
			}
		}

		if( pEnemyGroup && pEnemyGroup->ubSectorX > 1 && EnemyPermittedToAttackSector( &pEnemyGroup, (UINT8)(ubSectorID - 1) ) )
		{
			pPlayerGroup = FindMovementGroupInSector( (UINT8)(pEnemyGroup->ubSectorX - 1), pEnemyGroup->ubSectorY, OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
			}
			else if ( NumNonPlayerTeamMembersInSector( (UINT8)(pEnemyGroup->ubSectorX - 1), pEnemyGroup->ubSectorY, MILITIA_TEAM ) &&
							AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandleMilitiaNoticedByPatrolGroup( (UINT8)SECTOR( pEnemyGroup->ubSectorX-1, pEnemyGroup->ubSectorY ), pEnemyGroup );
			}
			else if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID-1) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				return HandleEmptySectorNoticedByPatrolGroup( pEnemyGroup, (UINT8)(ubSectorID-1) );
			}
		}

		if( pEnemyGroup && pEnemyGroup->ubSectorY < 16 && EnemyPermittedToAttackSector( &pEnemyGroup, (UINT8)(ubSectorID + 16) ) )
		{
			pPlayerGroup = FindMovementGroupInSector( pEnemyGroup->ubSectorX, (UINT8)(pEnemyGroup->ubSectorY + 1), OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
			}
			else if ( NumNonPlayerTeamMembersInSector( pEnemyGroup->ubSectorX, (UINT8)(pEnemyGroup->ubSectorY + 1), MILITIA_TEAM ) &&
							AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandleMilitiaNoticedByPatrolGroup( (UINT8)SECTOR( pEnemyGroup->ubSectorX, pEnemyGroup->ubSectorY+1 ), pEnemyGroup );
			}
			else if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID+16) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				return HandleEmptySectorNoticedByPatrolGroup( pEnemyGroup, (UINT8)(ubSectorID+16) );
			}
		}

		if( pEnemyGroup && pEnemyGroup->ubSectorX < 16 && EnemyPermittedToAttackSector( &pEnemyGroup, (UINT8)(ubSectorID + 1) ) )
		{
			pPlayerGroup = FindMovementGroupInSector( (UINT8)(pEnemyGroup->ubSectorX + 1), pEnemyGroup->ubSectorY, OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
			}
			else if ( NumNonPlayerTeamMembersInSector( (UINT8)(pEnemyGroup->ubSectorX + 1), pEnemyGroup->ubSectorY, MILITIA_TEAM ) &&
							AttemptToNoticeAdjacentGroupSucceeds() )
			{
				return HandleMilitiaNoticedByPatrolGroup( (UINT8)SECTOR( pEnemyGroup->ubSectorX+1, pEnemyGroup->ubSectorY ), pEnemyGroup );
			}
			else if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID+1) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				return HandleEmptySectorNoticedByPatrolGroup( pEnemyGroup, (UINT8)(ubSectorID+1) );
			}
		}

		if( !pEnemyGroup )
		{
			//group deleted.
			return TRUE;
		}
	}
	else if ( pGroup->usGroupTeam == OUR_TEAM || pGroup->usGroupTeam == MILITIA_TEAM )
	{
		//The player group has arrived at a new sector and now controls it.
		//Look in each of the four directions, and the enemy alertness rating will
		//determine if the enemy notices that the player is here.
		//Additionally, there are also stationary enemy groups that may also notice the
		//player's new presence.
		//NOTE:	Always returns false because it is the player group that we are handling.	We
		//		don't mess with the player group here!
		pPlayerGroup = pGroup;
		if( pPlayerGroup->ubSectorZ )
			return FALSE;
		if( !EnemyPermittedToAttackSector( NULL, (UINT8)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY ) ) )
			return FALSE;
		// WDS BAD BUG FIX - 12/25/2008 - some of the +1/-1 in the following were goofed up
		if( pPlayerGroup->ubSectorY > 1 )
		{
			pEnemyGroup = FindMovementGroupInSector( pPlayerGroup->ubSectorX, (UINT8)(pPlayerGroup->ubSectorY - 1), ENEMY_TEAM );
			if( pEnemyGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
				return FALSE;
			}
			pSector = &SectorInfo[ SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY-1 ) ];
			ubNumEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
			if( ubNumEnemies && pSector->ubGarrisonID != NO_GARRISON && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY-1 ) );
				return FALSE;
			}
		}
		if( pPlayerGroup->ubSectorX < 16 )
		{
			pEnemyGroup = FindMovementGroupInSector( (UINT8)(pPlayerGroup->ubSectorX + 1), pPlayerGroup->ubSectorY, ENEMY_TEAM );
			if( pEnemyGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
				return FALSE;
			}
			pSector = &SectorInfo[ SECTOR( pPlayerGroup->ubSectorX+1, pPlayerGroup->ubSectorY ) ];
			ubNumEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
			if( ubNumEnemies && pSector->ubGarrisonID != NO_GARRISON && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX+1, pPlayerGroup->ubSectorY ) );
				return FALSE;
			}
		}
		if( pPlayerGroup->ubSectorY < 16 )
		{
			pEnemyGroup = FindMovementGroupInSector( pPlayerGroup->ubSectorX, (UINT8)(pPlayerGroup->ubSectorY + 1), ENEMY_TEAM );
			if( pEnemyGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
				return FALSE;
			}
			pSector = &SectorInfo[ SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY+1 ) ];
			ubNumEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
			if( ubNumEnemies && pSector->ubGarrisonID != NO_GARRISON && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX, pPlayerGroup->ubSectorY+1 ) );
				return FALSE;
			}
		}
		if( pPlayerGroup->ubSectorX > 1 )
		{
			pEnemyGroup = FindMovementGroupInSector( (UINT8)(pPlayerGroup->ubSectorX-1), pPlayerGroup->ubSectorY, ENEMY_TEAM );
			if( pEnemyGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByPatrolGroup( pPlayerGroup, pEnemyGroup );
				return FALSE;
			}
			pSector = &SectorInfo[ SECTOR( pPlayerGroup->ubSectorX-1, pPlayerGroup->ubSectorY ) ];
			ubNumEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
			if( ubNumEnemies && pSector->ubGarrisonID != NO_GARRISON && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, (UINT8)SECTOR( pPlayerGroup->ubSectorX-1, pPlayerGroup->ubSectorY ) );
				return FALSE;
			}
		}
	}
	return FALSE;
}

//This is called periodically for each enemy occupied sector containing garrisons.
void CheckEnemyControlledSector( UINT8 ubSectorID )
{
	SECTORINFO *pSector;
	UINT8 ubSectorX, ubSectorY;
	if( !gfQueenAIAwake )
	{
		return;
	}

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	//First, determine if the sector is still owned by the enemy.
	pSector = &SectorInfo[ ubSectorID ];
	if( pSector->ubGarrisonID != NO_GARRISON )
	{
		if( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
		{ //Look for a staging group.
			GROUP *pGroup;
			pGroup = GetGroup( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID );
			if( pGroup )
			{ //We have a staging group
				if( GroupAtFinalDestination( pGroup ) )
				{
					if( pGroup->pEnemyGroup->ubPendingReinforcements )
					{
						if( pGroup->pEnemyGroup->ubPendingReinforcements > 4 )
						{
							UINT8 ubNum = (UINT8)(3 + Random( 3 ));
							pGroup->pEnemyGroup->ubNumTroops += ubNum;
							pGroup->ubGroupSize += ubNum;
							pGroup->pEnemyGroup->ubPendingReinforcements -= ubNum;
							RecalculateGroupWeight( pGroup );
							ValidateLargeGroup( pGroup );
						}
						else
						{
							pGroup->pEnemyGroup->ubNumTroops += pGroup->pEnemyGroup->ubPendingReinforcements;
							pGroup->ubGroupSize += pGroup->pEnemyGroup->ubPendingReinforcements;
							pGroup->pEnemyGroup->ubPendingReinforcements = 0;
							ValidateLargeGroup( pGroup );
						}
						//RequestHighPriorityStagingGroupReinforcements( pGroup );
					}
					else if( SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) != gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID )
					{
						MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ pSector->ubGarrisonID ].ubSectorID, DIRECT, pGroup->pEnemyGroup->ubIntention );
					}
				}
				//else the group is on route to stage hopefully...
			}
		}
	}

	if ( pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps )
	{

		//The sector is still controlled, so look around to see if there are any players nearby.
		ubSectorX = (UINT8)SECTORX( ubSectorID );
		ubSectorY = (UINT8)SECTORY( ubSectorID );
		if( ubSectorY > 1 && EnemyPermittedToAttackSector( NULL, (UINT8)(ubSectorID - 16) ) )
		{
			/*
			pPlayerGroup = FindMovementGroupInSector( ubSectorX, (UINT8)(ubSectorY-1), OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, ubSectorID );
				return;
			}
			else
			*/
			if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID-16) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				HandleEmptySectorNoticedByGarrison( ubSectorID, (UINT8)(ubSectorID-16) );
				return;
			}
		}
		if( ubSectorX < 16 && EnemyPermittedToAttackSector( NULL, (UINT8)(ubSectorID + 1) ) )
		{
			/*
			pPlayerGroup = FindMovementGroupInSector( (UINT8)(ubSectorX+1), ubSectorY, OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, ubSectorID );
				return;
			}
			else
			*/
			if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID+1) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				HandleEmptySectorNoticedByGarrison( ubSectorID, (UINT8)(ubSectorID+1) );
				return;
			}
		}
		if( ubSectorY < 16 && EnemyPermittedToAttackSector( NULL, (UINT8)(ubSectorID + 16) ) )
		{
			/*
			pPlayerGroup = FindMovementGroupInSector( ubSectorX, (UINT8)(ubSectorY+1), OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, ubSectorID );
				return;
			}
			else
			*/
			if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID+16) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				HandleEmptySectorNoticedByGarrison( ubSectorID, (UINT8)(ubSectorID+16) );
				return;
			}
		}
		if( ubSectorX > 1 && EnemyPermittedToAttackSector( NULL, (UINT8)(ubSectorID - 1) ) )
		{
			/*
			pPlayerGroup = FindMovementGroupInSector( (UINT8)(ubSectorX-1), ubSectorY, OUR_TEAM );
			if( pPlayerGroup && AttemptToNoticeAdjacentGroupSucceeds() )
			{
				HandlePlayerGroupNoticedByGarrison( pPlayerGroup, ubSectorID );
				return;
			}
			else
			*/
			if( AdjacentSectorIsImportantAndUndefended( (UINT8)(ubSectorID-1) ) && AttemptToNoticeEmptySectorSucceeds() )
			{
				HandleEmptySectorNoticedByGarrison( ubSectorID, (UINT8)(ubSectorID-1) );
				return;
			}
		}
	}
}


void RemoveGroupFromStrategicAILists( UINT8 ubGroupID )
{
	INT32 i;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( i = 0; i < giPatrolArraySize; i++ )
	{
		if( gPatrolGroup[ i ].ubGroupID == ubGroupID )
		{ //Patrol group was destroyed.
			gPatrolGroup[ i ].ubGroupID = 0;
			RecalculatePatrolWeight( i );
			return;
		}
		if( gPatrolGroup[ i ].ubPendingGroupID == ubGroupID )
		{ //Group never arrived to reinforce.
			gPatrolGroup[ i ].ubPendingGroupID = 0;
			return;
		}
	}
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		if( gGarrisonGroup[ i ].ubPendingGroupID == ubGroupID )
		{ //Group never arrived to reinforce.
			gGarrisonGroup[ i ].ubPendingGroupID = 0;
			return;
		}
	}
}

void RecalculatePatrolWeight( INT32 iPatrolID )
{
	GROUP *pGroup;
	INT32 iWeight, iPrevWeight;
	INT32 iNeedPopulation;

	ValidateWeights( 4 );

	//First, remove the previous weight from the applicable field.
	iPrevWeight = gPatrolGroup[ iPatrolID ].bWeight;
	if( iPrevWeight > 0 )
		giRequestPoints -= iPrevWeight;

	if( gPatrolGroup[ iPatrolID ].ubGroupID )
	{
		pGroup = GetGroup( gPatrolGroup[ iPatrolID ].ubGroupID );
		iNeedPopulation = gPatrolGroup[ iPatrolID ].bSize - pGroup->ubGroupSize;
		if( iNeedPopulation < 0 )
		{
			gPatrolGroup[ iPatrolID ].bWeight = 0;
			ValidateWeights( 27 );
			return;
		}
	}
	else
	{
		iNeedPopulation = gPatrolGroup[ iPatrolID ].bSize;
	}
	iWeight = iNeedPopulation * 3 * gPatrolGroup[ iPatrolID ].bPriority / 96;
	iWeight = min( 2, iWeight );
	gPatrolGroup[ iPatrolID ].bWeight = (INT8)iWeight;
	giRequestPoints += iWeight;

	ValidateWeights( 5 );
}

void RecalculateGarrisonWeight( INT32 iGarrisonID )
{
	SECTORINFO *pSector;
	INT32 iWeight, iPrevWeight;
	INT32 iDesiredPop, iCurrentPop, iPriority;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	ValidateWeights( 6 );

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	iDesiredPop = gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bDesiredPopulation;
	iCurrentPop = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	iPriority = gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bPriority;

	//First, remove the previous weight from the applicable field.
	iPrevWeight = gGarrisonGroup[ iGarrisonID ].bWeight;
	if( iPrevWeight > 0 )
		giRequestPoints -= iPrevWeight;
	else if( iPrevWeight < 0 )
		giReinforcementPoints += iPrevWeight;

	//Calculate weight (range is -20 to +20 before multiplier).
	//The multiplier of 3 brings it to a range of -96 to +96 which is
	//close enough to a plus/minus 100%.	The resultant percentage is then
	//converted based on the priority.
	iWeight = (iDesiredPop - iCurrentPop) * 3;
	if( iWeight > 0 )
	{ //modify it by it's priority.
		//generates a value between 2 and 100
		iWeight = iWeight * iPriority / 96;
		iWeight = max( iWeight, 2 );
		giRequestPoints += iWeight;
	}
	else if( iWeight < 0 )
	{ //modify it by it's reverse priority
		//generates a value between -2 and -100
		iWeight = iWeight * (100-iPriority) / 96;
		iWeight = min( iWeight, -2 );
		giReinforcementPoints -= (INT8)iWeight;
	}

	gGarrisonGroup[ iGarrisonID ].bWeight = (INT8)iWeight;

	ValidateWeights( 7 );
}

void RecalculateSectorWeight( UINT8 ubSectorID )
{
	INT32 i;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		if( gGarrisonGroup[ i ].ubSectorID == ubSectorID )
		{
			RecalculateGarrisonWeight( i );
			return;
		}
	}
}

void RecalculateGroupWeight( GROUP *pGroup )
{
	INT32 i;
	for( i = 0; i < giPatrolArraySize; i++ )
	{
		if( gPatrolGroup[ i ].ubGroupID == pGroup->ubGroupID || gPatrolGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
		{
			if( !pGroup->ubGroupSize )
			{
				TagSAIGroupWithGracePeriod( pGroup );
				gPatrolGroup[ i ].ubGroupID = 0;
			}
			RecalculatePatrolWeight( i );
			return;
		}
	}

}

INT32 ChooseSuitableGarrisonToProvideReinforcements( INT32 iDstGarrisonID, INT32 iReinforcementsRequested )
{
	INT32 iSrcGarrisonID, iBestGarrisonID = NO_GARRISON;
	INT32 iReinforcementsAvailable;
	INT32 i, iRandom, iWeight;
	INT8 bBestWeight;
	UINT8 ubSectorID;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	//Check to see if we could send reinforcements from Alma.	Only Drassen/Cambria get preferred
	//service from Alma, due to it's proximity and Alma's purpose as a forward military base.
	ubSectorID = gGarrisonGroup[ iDstGarrisonID ].ubSectorID;
	switch( ubSectorID )
	{
		case SEC_B13: case SEC_C13: case SEC_D13: case SEC_D15:								//Drassen + nearby SAM site
		case SEC_F8:	case SEC_F9:	case SEC_G8:	case SEC_G9:	case SEC_H8:	//Cambria
			//reinforcements will be primarily sent from Alma whenever possible.

			//find which the first sector that contains Alma soldiers.
			for( i = 0; i < giGarrisonArraySize; i++ )
			{
				if( gGarrisonGroup[ i ].ubComposition == ALMA_DEFENCE )
					break;
			}
			iSrcGarrisonID = i;
			//which of these 4 Alma garrisons have the most reinforcements available?	It is
			//possible that none of these garrisons can provide any reinforcements.
			bBestWeight = 0;
			for( i = iSrcGarrisonID; i < iSrcGarrisonID + 4; i++ )
			{
				RecalculateGarrisonWeight( i );
				if( bBestWeight > gGarrisonGroup[ i ].bWeight && GarrisonCanProvideMinimumReinforcements( i ) )
				{
					bBestWeight = gGarrisonGroup[ i ].bWeight;
					iBestGarrisonID = i;
				}
			}
			//If we can provide reinforcements from Alma, then make sure that it can provide at least 67% of
			//the requested reinforcements.
			if( bBestWeight < 0 )
			{
				iReinforcementsAvailable = ReinforcementsAvailable( iBestGarrisonID );
				if( iReinforcementsAvailable * 100 >= iReinforcementsRequested * 67 )
				{ //This is the approved group to provide the reinforcements.
					return iBestGarrisonID;
				}
			}
			break;
	}

	//The Alma case either wasn't applicable or failed to have the right reinforcements.	Do a general weighted search.
	iRandom = Random( giReinforcementPoints );
	for( iSrcGarrisonID = 0; iSrcGarrisonID < giGarrisonArraySize; iSrcGarrisonID++ )
	{ //go through the garrisons
		RecalculateGarrisonWeight( iSrcGarrisonID );
		iWeight = -gGarrisonGroup[ iSrcGarrisonID ].bWeight;
		if( iWeight > 0 )
		{ //if group is able to provide reinforcements.
			if( iRandom < iWeight && GarrisonCanProvideMinimumReinforcements( iSrcGarrisonID ) )
			{
				iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );
				if( iReinforcementsAvailable * 100 >= iReinforcementsRequested * 67 )
				{ //This is the approved group to provide the reinforcements.
					return iSrcGarrisonID;
				}
			}
			iRandom -= iWeight;
		}
	}

	//So far we have failed on all accounts.	Now, simply process all the garrisons, and return the first garrison that can
	//provide the reinforcements.
	for( iSrcGarrisonID = 0; iSrcGarrisonID < giGarrisonArraySize; iSrcGarrisonID++ )
	{ //go through the garrisons
		RecalculateGarrisonWeight( iSrcGarrisonID );
		iWeight = -gGarrisonGroup[ iSrcGarrisonID ].bWeight;
		if( iWeight > 0 && GarrisonCanProvideMinimumReinforcements( iSrcGarrisonID ) )
		{ //if group is able to provide reinforcements.
			iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );
			if( iReinforcementsAvailable * 100 >= iReinforcementsRequested * 67 )
			{ //This is the approved group to provide the reinforcements.
				return iSrcGarrisonID;
			}
		}
	}

	//Well, if we get this far, the queen must be low on troops.	Send whatever we can.
	iRandom = Random( giReinforcementPoints );
	for( iSrcGarrisonID = 0; iSrcGarrisonID < giGarrisonArraySize; iSrcGarrisonID++ )
	{ //go through the garrisons
		RecalculateGarrisonWeight( iSrcGarrisonID );
		iWeight = -gGarrisonGroup[ iSrcGarrisonID ].bWeight;
		if( iWeight > 0 && GarrisonCanProvideMinimumReinforcements( iSrcGarrisonID ) )
		{ //if group is able to provide reinforcements.
			if( iRandom < iWeight )
			{
				iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );
				return iSrcGarrisonID;
			}
			iRandom -= iWeight;
		}
	}

	//Failed completely.
	return -1;
}

BOOLEAN SendReinforcementsForGarrison( INT32 iDstGarrisonID, UINT16 usDefencePoints, GROUP **pOptionalGroup )
{
	SECTORINFO *pSector;
	INT32 iChance, iRandom, iSrcGarrisonID;
	INT32 iMaxReinforcementsAllowed, iReinforcementsAvailable, iReinforcementsRequested, iReinforcementsApproved;
	GROUP *pGroup;
	UINT8 ubSrcSectorX, ubSrcSectorY, ubDstSectorX, ubDstSectorY;
	UINT8 ubNumExtraReinforcements;
	UINT8 ubGroupSize;
	BOOLEAN fLimitMaxTroopsAllowable = FALSE;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	ValidateWeights( 8 );

	/*if( gGarrisonGroup[ iDstGarrisonID ].ubSectorID == SEC_B13 ||
		gGarrisonGroup[ iDstGarrisonID ].ubSectorID == SEC_C13 ||
		gGarrisonGroup[ iDstGarrisonID ].ubSectorID == SEC_D13 )
	{
		pSector = NULL;
	}*/

	pSector = &SectorInfo[ gGarrisonGroup[ iDstGarrisonID ].ubSectorID ];
	//Determine how many units the garrison needs.
	iReinforcementsRequested = GarrisonReinforcementsRequested( iDstGarrisonID, &ubNumExtraReinforcements );

	//The maximum number of reinforcements can't be offsetted past a certain point based on the
	//priority of the garrison.
	iMaxReinforcementsAllowed = //from 1 to 3 times the desired size of the normal force.
		gArmyComp[ gGarrisonGroup[ iDstGarrisonID ].ubComposition ].bDesiredPopulation +
		gArmyComp[ gGarrisonGroup[ iDstGarrisonID ].ubComposition ].bDesiredPopulation *
		gArmyComp[ gGarrisonGroup[ iDstGarrisonID ].ubComposition ].bPriority / 50;

	if( iReinforcementsRequested + ubNumExtraReinforcements > iMaxReinforcementsAllowed )
	{
		//adjust the extra reinforcements so that it doesn't exceed the maximum allowed.
		fLimitMaxTroopsAllowable = TRUE;
		ubNumExtraReinforcements = (UINT16)(max(0, iMaxReinforcementsAllowed - iReinforcementsRequested));
	}

	// Flugente: this is stupid. why would we limit the groupsize by the desired population? If that is smaller than the minimum group size, then we'll create a group smaller than that
	// this would forbid us from properly moving it. Why does gGameExternalOptions.iMaxEnemyGroupSize exist if it isn't properly used?
	ubNumExtraReinforcements = (UINT16)(max(0, gubMinEnemyGroupSize - iReinforcementsRequested));
	
	iReinforcementsRequested += ubNumExtraReinforcements;

	if( iReinforcementsRequested <= 0 )
	{
		ValidateWeights( 9 );
		return FALSE;
	}

	ubDstSectorX = (UINT8)SECTORX( gGarrisonGroup[ iDstGarrisonID ].ubSectorID );
	ubDstSectorY = (UINT8)SECTORY( gGarrisonGroup[ iDstGarrisonID ].ubSectorID );

	if( pOptionalGroup && *pOptionalGroup )
	{
		//This group will provide the reinforcements
		pGroup = *pOptionalGroup;

		#ifdef JA2BETAVERSION
			LogStrategicEvent( "%d troops have been reassigned from %c%d to garrison sector %c%d",
							   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots,
				pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX,
				ubDstSectorY + 'A' - 1, ubDstSectorX );
		#endif

		gGarrisonGroup[ iDstGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
		ConvertGroupTroopsToComposition( pGroup, gGarrisonGroup[ iDstGarrisonID ].ubComposition );
		MoveSAIGroupToSector( pOptionalGroup, gGarrisonGroup[ iDstGarrisonID ].ubSectorID, STAGE, REINFORCEMENTS );

		ValidateWeights( 10 );

		return TRUE;
	}
	iRandom = Random( giReinforcementPoints + giReinforcementPool );
	if( iRandom < giReinforcementPool )
	{ //use the pool and send the requested amount from SECTOR P3 (queen's palace)
		QUEEN_POOL:

		//KM : Sep 9, 1999
		//If the player owns sector P3, any troops that spawned there were causing serious problems, seeing battle checks
		//were not performed!
		if( !StrategicMap[ CALCULATE_STRATEGIC_INDEX( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) ].fEnemyControlled )
		{
			//Queen can no longer send reinforcements from the palace if she doesn't control it!
			return FALSE;
		}

		if( !giReinforcementPool )
		{
			ValidateWeights( 11 );
			return FALSE;
		}

		iReinforcementsApproved = min( iReinforcementsRequested, giReinforcementPool );

		if( iReinforcementsApproved * 3 < usDefencePoints )
		{
			//The enemy force that would be sent would likely be decimated by the player forces.
			gubGarrisonReinforcementsDenied[ iDstGarrisonID ] += (UINT8)(gArmyComp[ gGarrisonGroup[ iDstGarrisonID ].ubComposition ].bPriority / 2);
			ValidateWeights( 12 );
			return FALSE;
		}
		else
		{
			//The force is strong enough to be able to take the sector.
			gubGarrisonReinforcementsDenied[ iDstGarrisonID ] = 0;
		}

		//The chance she will send them is related with the strength difference between the
		//player's force and the queen's.
		if( ubNumExtraReinforcements && fLimitMaxTroopsAllowable && iReinforcementsApproved == iMaxReinforcementsAllowed )
		{
			iChance = (iReinforcementsApproved + ubNumExtraReinforcements) * 100 / max(usDefencePoints, 1); // lal bugfix: max 1 added to prevent division by 0
			if( !Chance( iChance ) )
			{
				ValidateWeights( 13 );
				return FALSE;
			}
		}		

		pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, (UINT8)iReinforcementsApproved, 0, 0, 0, 0 );
		ConvertGroupTroopsToComposition( pGroup, gGarrisonGroup[ iDstGarrisonID ].ubComposition );
		pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
		if (gGameExternalOptions.fASDActive && Random(10) < gGameOptions.ubDifficultyLevel)
		{
			ASDInitializePatrolGroup(pGroup);
		}
		//Madd: unlimited reinforcements?
		if ( !gfUnlimitedTroops )
			giReinforcementPool -= iReinforcementsApproved;

		pGroup->ubMoveType = ONE_WAY;
		gGarrisonGroup[ iDstGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;

		ubGroupSize = (UINT8)(pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumRobots + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps);

		if( ubNumExtraReinforcements )
		{
			#ifdef JA2BETAVERSION
				LogStrategicEvent( "%d troops have been sent from palace to stage assault near sector %c%d",
					ubGroupSize, ubDstSectorY + 'A' - 1, ubDstSectorX );
			#endif

			MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iDstGarrisonID ].ubSectorID, STAGE, STAGING );
		}
		else
		{
			#ifdef JA2BETAVERSION
				LogStrategicEvent( "%d troops have been sent from palace to garrison sector %c%d",
					ubGroupSize, ubDstSectorY + 'A' - 1, ubDstSectorX );
			#endif

			MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iDstGarrisonID ].ubSectorID, STAGE, REINFORCEMENTS );
		}

		ValidateWeights( 14 );

		return TRUE;
	}
	else
	{
		iSrcGarrisonID = ChooseSuitableGarrisonToProvideReinforcements( iDstGarrisonID, iReinforcementsRequested );
		// WDS 08/01/2008 - Fix 0 reinforcement problem
		iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );
		if( (iSrcGarrisonID == -1) || (iReinforcementsAvailable == 0) )
		{
			ValidateWeights( 15 );
			goto QUEEN_POOL;
		}

		ubSrcSectorX = SECTORX( gGarrisonGroup[iSrcGarrisonID].ubSectorID );
		ubSrcSectorY = SECTORY( gGarrisonGroup[iSrcGarrisonID].ubSectorID );
		if( ubSrcSectorX != gWorldSectorX || ubSrcSectorY != gWorldSectorY || gbWorldSectorZ > 0 )
		{
			//The reinforcements aren't coming from the currently loaded sector!
			iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );
			if( iReinforcementsAvailable <= 0)
			{
				SAIReportError( L"Attempting to send reinforcements from a garrison that doesn't have any! -- KM:0 (with prior saved game and strategic decisions.txt)" );
				return FALSE;
			}
			//Send the lowest of the two:	number requested or number available

			iReinforcementsApproved = min( iReinforcementsRequested, iReinforcementsAvailable );
			if( iReinforcementsApproved > iMaxReinforcementsAllowed - ubNumExtraReinforcements )
			{
				//The force isn't strong enough, but the queen isn't willing to apply extra resources
				iReinforcementsApproved = iMaxReinforcementsAllowed - ubNumExtraReinforcements;
			}
			else if( (iReinforcementsApproved + ubNumExtraReinforcements) * 3 < usDefencePoints )
			{
				//The enemy force that would be sent would likely be decimated by the player forces.
				gubGarrisonReinforcementsDenied[ iDstGarrisonID ] += (UINT8)(gArmyComp[ gGarrisonGroup[ iDstGarrisonID ].ubComposition ].bPriority / 2);
				ValidateWeights( 17 );
				return FALSE;
			}
			else
			{
				//The force is strong enough to be able to take the sector.
				gubGarrisonReinforcementsDenied[ iDstGarrisonID ] = 0;
			}

			//The chance she will send them is related with the strength difference between the
			//player's force and the queen's.
			if( iReinforcementsApproved + ubNumExtraReinforcements == iMaxReinforcementsAllowed && usDefencePoints )
			{
				iChance = (iReinforcementsApproved + ubNumExtraReinforcements) * 100 / max(usDefencePoints, 1); // lal bugfix: max 1 added to prevent division by 0
				if( !Chance( iChance ) )
				{
					ValidateWeights( 18 );
					return FALSE;
				}
			}

			pGroup = CreateNewEnemyGroupDepartingFromSector( gGarrisonGroup[ iSrcGarrisonID ].ubSectorID, 0, (UINT8)iReinforcementsApproved, 0, 0, 0, 0 );
			ConvertGroupTroopsToComposition( pGroup, gGarrisonGroup[ iDstGarrisonID ].ubComposition );
			if (gGameExternalOptions.fASDActive && Random(10) < gGameOptions.ubDifficultyLevel)
			{
				ASDInitializePatrolGroup(pGroup);
			}
			RemoveSoldiersFromGarrisonBasedOnComposition( iSrcGarrisonID, pGroup->ubGroupSize );
			pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
			pGroup->ubMoveType = ONE_WAY;
			gGarrisonGroup[ iDstGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
			ubGroupSize = (UINT8)(pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumRobots + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps);

			if( ubNumExtraReinforcements )
			{
				pGroup->pEnemyGroup->ubPendingReinforcements = ubNumExtraReinforcements;

				#ifdef JA2BETAVERSION
					LogStrategicEvent( "%d troops have been sent from sector %c%d to stage assault near sector %c%d",
						ubGroupSize, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX, ubDstSectorY + 'A' - 1, ubDstSectorX );
				#endif

				MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iDstGarrisonID ].ubSectorID, STAGE, STAGING );
			}
			else
			{
				#ifdef JA2BETAVERSION
					LogStrategicEvent( "%d troops have been sent from sector %c%d to garrison sector %c%d",
						ubGroupSize, pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX, ubDstSectorY + 'A' - 1, ubDstSectorX );
				#endif

				MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iDstGarrisonID ].ubSectorID, STAGE, REINFORCEMENTS );
			}

			#ifdef JA2BETAVERSION
				LogStrategicEvent( "%d troops have been sent from garrison sector %c%d to patrol area near sector %c%d",
					ubGroupSize, ubSrcSectorY + 'A' - 1, ubSrcSectorX, ubDstSectorY + 'A' - 1, ubDstSectorX );
			#endif

			ValidateWeights( 19 );
			return TRUE;
		}
	}

	ValidateWeights( 20 );

	return FALSE;
}

BOOLEAN SendReinforcementsForPatrol( INT32 iPatrolID, GROUP **pOptionalGroup )
{
	GROUP *pGroup;
	INT32 iRandom, iSrcGarrisonID, iWeight;
	INT32 iReinforcementsAvailable, iReinforcementsRequested, iReinforcementsApproved;
	UINT8 ubSrcSectorX, ubSrcSectorY, ubDstSectorX, ubDstSectorY;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	ValidateWeights( 21 );

	//Determine how many units the patrol group needs.
	iReinforcementsRequested = PatrolReinforcementsRequested( iPatrolID );

	if( iReinforcementsRequested <= 0)
		return FALSE;

	ubDstSectorX = SECTORX( gPatrolGroup[iPatrolID].ubSectorID[1] );
	ubDstSectorY = SECTORY( gPatrolGroup[iPatrolID].ubSectorID[1] );

	if( pOptionalGroup && *pOptionalGroup )
	{
		//This group will provide the reinforcements
		pGroup = *pOptionalGroup;

		gPatrolGroup[ iPatrolID ].ubPendingGroupID = pGroup->ubGroupID;

#ifdef JA2BETAVERSION
		LogStrategicEvent( "%d troops have been reassigned from %c%d to reinforce patrol group covering sector %c%d",
						   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots,
			pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX,
			ubDstSectorY + 'A' - 1, ubDstSectorX );
#endif

		MoveSAIGroupToSector( pOptionalGroup, gPatrolGroup[ iPatrolID ].ubSectorID[1], EVASIVE, REINFORCEMENTS );

		ValidateWeights( 22 );

		return TRUE;
	}

	iRandom = Random( giReinforcementPoints + giReinforcementPool );

	if( iRandom < giReinforcementPool )
	{
		//use the pool and send the requested amount from SECTOR P3 (queen's palace)
		iReinforcementsApproved = min( iReinforcementsRequested, giReinforcementPool );
		pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, (UINT8)iReinforcementsApproved, 0, 0, 0, 0 );
		pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
		InitializeGroup(GROUP_TYPE_PATROL, iReinforcementsApproved, *pGroup->pEnemyGroup, Random(10) < gGameOptions.ubDifficultyLevel);

		//Madd: unlimited reinforcements?
		if ( !gfUnlimitedTroops )
			giReinforcementPool -= iReinforcementsApproved;

		gPatrolGroup[ iPatrolID ].ubPendingGroupID = pGroup->ubGroupID;

		#ifdef JA2BETAVERSION
			LogStrategicEvent( "%d troops have been sent from palace to patrol area near sector %c%d",
							   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps,
				ubDstSectorY + 'A' - 1, ubDstSectorX );
		#endif

		MoveSAIGroupToSector( &pGroup, gPatrolGroup[ iPatrolID ].ubSectorID[1], EVASIVE, REINFORCEMENTS );

		ValidateWeights( 23 );

		return TRUE;
	}
	else
	{
		iRandom -= giReinforcementPool;

		for( iSrcGarrisonID = 0; iSrcGarrisonID < giGarrisonArraySize; ++iSrcGarrisonID )
		{
			//go through the garrisons
			RecalculateGarrisonWeight( iSrcGarrisonID );
			iWeight = -gGarrisonGroup[ iSrcGarrisonID ].bWeight;
			if( iWeight > 0 )
			{
				//if group is able to provide reinforcements.
				if( iRandom < iWeight )
				{
					//This is the group that gets the reinforcements!
					ubSrcSectorX = (UINT8)SECTORX(gGarrisonGroup[ iSrcGarrisonID ].ubSectorID );
					ubSrcSectorY = (UINT8)SECTORY(gGarrisonGroup[ iSrcGarrisonID ].ubSectorID );

					if( ubSrcSectorX != gWorldSectorX || ubSrcSectorY != gWorldSectorY || gbWorldSectorZ > 0 )
					{
						//The reinforcements aren't coming from the currently loaded sector!
						iReinforcementsAvailable = ReinforcementsAvailable( iSrcGarrisonID );

						//Send the lowest of the two:	number requested or number available
						iReinforcementsApproved = min( iReinforcementsRequested, iReinforcementsAvailable );

						// if there are no reinforcements, then there is no reason to send an empty group
						if ( !iReinforcementsApproved )
							continue;

						pGroup = CreateNewEnemyGroupDepartingFromSector( gGarrisonGroup[ iSrcGarrisonID ].ubSectorID, 0, (UINT8)iReinforcementsApproved, 0, 0, 0, 0 );
						pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
						gPatrolGroup[ iPatrolID ].ubPendingGroupID = pGroup->ubGroupID;
						InitializeGroup(GROUP_TYPE_PATROL, iReinforcementsApproved, *pGroup->pEnemyGroup, Random(10) < gGameOptions.ubDifficultyLevel);

						RemoveSoldiersFromGarrisonBasedOnComposition( iSrcGarrisonID, pGroup->ubGroupSize );

						#ifdef JA2BETAVERSION
							LogStrategicEvent( "%d troops have been sent from garrison sector %c%d to patrol area near sector %c%d",
											   pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps + pGroup->pEnemyGroup->ubNumRobots,
								ubSrcSectorY + 'A' - 1, ubSrcSectorX,
								ubDstSectorY + 'A' - 1, ubDstSectorX );
						#endif

						MoveSAIGroupToSector( &pGroup, gPatrolGroup[ iPatrolID ].ubSectorID[1], EVASIVE, REINFORCEMENTS );

						ValidateWeights( 24 );

						return TRUE;
					}
				}

				iRandom -= iWeight;
			}
		}
	}

	ValidateWeights( 25 );

	return FALSE;
}

//Periodically does a general poll and check on each of the groups and garrisons, determines
//reinforcements, new patrol groups, planned assaults, etc.
void EvaluateQueenSituation()
{
	INT32 i, iRandom;
	INT32 iWeight;
	UINT32 uiOffset;
	UINT16 usDefencePoints = 0;
	INT32 iOrigRequestPoints;
	INT32 iSumOfAllWeights = 0;

	INT32 iApplicableRequestPoints = 0;
	INT32 iApplicableGarrisons = 0;
	INT32 iApplicableGarrisonIds[ MAX_GARRISON_GROUPS ];
	INT32 iApplicablePatrols = 0;
	INT32 iApplicablePatrolIds[ MAX_PATROL_GROUPS ];

	ValidateWeights( 26 );

	// figure out how long it shall be before we call this again

	// The more work to do there is (request points the queen's army is asking for), the more often she will make decisions
	// This can increase the decision intervals by up to 500 extra minutes (> 8 hrs)
	uiOffset = max( 100 - giRequestPoints, 0);
	uiOffset = uiOffset + Random( uiOffset * 4 );

	// Flugente: enemy generals can speed up the AI decision process (justification: the generals are the ones planning the military operations, so if they are around, the army is more efficient)
	FLOAT dEnemyGeneralsSpeedupFactor = 1.0f;
	if ( gGameExternalOptions.fEnemyRoles && gGameExternalOptions.fEnemyGenerals )
	{
		dEnemyGeneralsSpeedupFactor = max( 0.5f, dEnemyGeneralsSpeedupFactor - gStrategicStatus.usVIPsLeft * gGameExternalOptions.fEnemyGeneralStrategicDecisionSpeedBonus );
	}
	
	uiOffset += dEnemyGeneralsSpeedupFactor * (zDiffSetting[gGameOptions.ubDifficultyLevel].iBaseDelayInMinutesBetweenEvaluations + Random( zDiffSetting[gGameOptions.ubDifficultyLevel].iEvaluationDelayVariance ));
	
	// Check/update reinforcements pool if old behavior is enabled
	if ( !gfUnlimitedTroops && zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolIncrementDaysPerDifficultyLevel == 0 )
	{
		// sevenfm: allow recruiting when pool size drops below iQueenPoolBaseIncrementSizePerDifficultyLevel, this should result in more stable strategic AI behavior
		if ( giReinforcementPool < zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolBaseIncrementSizePerDifficultyLevel )
		{
			//Queen has run out of reinforcements. Simulate recruiting and training new troops
			uiOffset *= 10;
			giReinforcementPool += ( zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolBaseIncrementSizePerDifficultyLevel ) * ( 100 + CurrentPlayerProgressPercentage() ) / 100;
			AddStrategicEvent( EVENT_EVALUATE_QUEEN_SITUATION, GetWorldTotalMin() + uiOffset, 0 );
			return;
		}
	}

	//Re-post the event
	AddStrategicEvent( EVENT_EVALUATE_QUEEN_SITUATION, GetWorldTotalMin() + uiOffset, 0 );

	// if the queen hasn't been alerted to player's presence yet
	if( !gfQueenAIAwake )
	{ //no decisions can be made yet.
		return;
	}

	// Adjust queen's disposition based on player's progress
	EvolveQueenPriorityPhase( FALSE );

	// Gradually promote any remaining admins into troops
	UpgradeAdminsToTroops();

	if( ( giRequestPoints <= 0 ) || ( ( giReinforcementPoints <= 0 ) && ( giReinforcementPool <= 0 ) ) )
	{ //we either have no reinforcements or request for reinforcements.
		return;
	}

	// anv: only consider garrisons and patrols that can be reinforced
	// otherwise unreinforcable groups will stall the rest, effectively breaking entire system

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		RecalculateGarrisonWeight( i );
		iWeight = gGarrisonGroup[ i ].bWeight;
		if( iWeight > 0 )
		{
			if( !gGarrisonGroup[ i ].ubPendingGroupID &&
					EnemyPermittedToAttackSector( NULL, gGarrisonGroup[ i ].ubSectorID ) &&
					GarrisonRequestingMinimumReinforcements( i ) )
			{
				if( ReinforcementsApproved( i, &usDefencePoints ) )
				{
					iApplicableGarrisonIds[iApplicableGarrisons] = i;
					iApplicableGarrisons++;
					iApplicableRequestPoints += gGarrisonGroup[ i ].bWeight;
				}
			}
		}
	}
	for( i = 0; i < giPatrolArraySize; i++ )
	{
		RecalculatePatrolWeight( i );
		iWeight = gPatrolGroup[ i ].bWeight;
		if( iWeight > 0 )
		{
			if( !gPatrolGroup[ i ].ubPendingGroupID && PatrolRequestingMinimumReinforcements( i ) )
			{
				iApplicablePatrolIds[iApplicablePatrols] = i;
				iApplicablePatrols++;
				iApplicableRequestPoints += gPatrolGroup[ i ].bWeight;
			}
		}
	}

	if( !iApplicableRequestPoints )
	{
		return;
	}

	//now randomly choose who gets the reinforcements.
	// giRequestPoints is the combined sum of all the individual weights of all garrisons and patrols requesting reinforcements
	//iRandom = Random( giRequestPoints );
	iRandom = Random( iApplicableRequestPoints );

	iOrigRequestPoints = giRequestPoints;	// debug only!

	//go through garrisons first
	for( i = 0; i < iApplicableGarrisons; i++ )
	{
		iSumOfAllWeights += iWeight;	// debug only!
		iWeight = gGarrisonGroup[ iApplicableGarrisonIds[i] ].bWeight;
		if( iRandom < iWeight )
		{
			//This is the group that gets the reinforcements!
			if ( SendReinforcementsForGarrison( iApplicableGarrisonIds[i] , usDefencePoints, NULL ) )
				return;
		}
		iRandom -= iWeight;
	}

	//go through the patrol groups
	for( i = 0; i < iApplicablePatrols; i++ )
	{
		iSumOfAllWeights += iWeight;	// debug only!
		iWeight = gPatrolGroup[ iApplicablePatrolIds[i] ].bWeight;
		if( iRandom < iWeight )
		{
			//This is the group that gets the reinforcements!
			if ( SendReinforcementsForPatrol( iApplicablePatrolIds[i], NULL ) )
				return;
		}
		iRandom -= iWeight;
	}

	ValidateWeights( 27 );
}


BOOLEAN SaveStrategicAI( HWFILE hFile )
{
	GARRISON_GROUP gTempGarrisonGroup;
	PATROL_GROUP gTempPatrolGroup;
	ARMY_COMPOSITION gTempArmyComp;
	UINT32 uiNumBytesWritten;
	INT32 i;

	memset( &gTempPatrolGroup, 0, sizeof( PATROL_GROUP ) );
	memset( &gTempArmyComp, 0, sizeof( ARMY_COMPOSITION ) );

	FileWrite( hFile, gbPadding2,							3, &uiNumBytesWritten );
	if( uiNumBytesWritten != 3 )
		return FALSE;
	FileWrite( hFile, &gfExtraElites,					1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &giGarrisonArraySize,		4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giPatrolArraySize,			4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giReinforcementPool,		4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giForcePercentage,			4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giArmyAlertness,				4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giArmyAlertnessDecay,	4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &gfQueenAIAwake,				1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &giReinforcementPoints, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &giRequestPoints,				4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
		return FALSE;
	FileWrite( hFile, &gubNumAwareBattles,		1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gubSAIVersion,					1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gubQueenPriorityPhase,	1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gfFirstBattleMeanwhileScenePending,	1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gfMassFortificationOrdered,	1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gubMinEnemyGroupSize,	1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gubHoursGracePeriod, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &gusPlayerBattleVictories, 2, &uiNumBytesWritten );
	if( uiNumBytesWritten != 2 )
		return FALSE;
	FileWrite( hFile, &gfUseAlternateQueenPosition, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
		return FALSE;
	FileWrite( hFile, &giTotalRecruitsInTraining, sizeof( giTotalRecruitsInTraining ), &uiNumBytesWritten );
	if ( uiNumBytesWritten != sizeof( giTotalRecruitsInTraining ) )
		return FALSE; 
	FileWrite( hFile, gbPadding, SAI_PADDING_BYTES, &uiNumBytesWritten );
	if( uiNumBytesWritten != SAI_PADDING_BYTES )
		return FALSE;
	

	//Save the army composition (which does get modified)
	FileWrite( hFile, gArmyComp, NUM_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ), &uiNumBytesWritten );
	if( uiNumBytesWritten != NUM_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ) )
		return FALSE;
	i = SAVED_ARMY_COMPOSITIONS - NUM_ARMY_COMPOSITIONS;
	while( i-- )
	{
		FileWrite( hFile, &gTempArmyComp, sizeof( ARMY_COMPOSITION ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( ARMY_COMPOSITION ) )
			return FALSE;
	}
	//Save the patrol group definitions
	FileWrite( hFile, gPatrolGroup, giPatrolArraySize * sizeof( PATROL_GROUP ), &uiNumBytesWritten );
	if( uiNumBytesWritten != giPatrolArraySize * sizeof( PATROL_GROUP ) )
		return FALSE;
	i = SAVED_PATROL_GROUPS - giPatrolArraySize;
	while( i-- )
	{
		FileWrite( hFile, &gTempPatrolGroup, sizeof( PATROL_GROUP ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( PATROL_GROUP ) )
			return FALSE;
	}
	//Save the garrison information!
	/* Step 1: fill an EMPTY GARRISON for later padding */
	memset( &gTempGarrisonGroup, 0, sizeof( GARRISON_GROUP ) );
	if( giGarrisonArraySize > SAVED_GARRISON_GROUPS )
	{
	giGarrisonArraySize = SAVED_GARRISON_GROUPS;
	}

	/* Step 2: write current Garrison Block of <giGarrisonArraySize> Elements */
	FileWrite( hFile, gGarrisonGroup, giGarrisonArraySize * sizeof( GARRISON_GROUP ), &uiNumBytesWritten );
	if( uiNumBytesWritten != giGarrisonArraySize * sizeof( GARRISON_GROUP ) )
		return FALSE;

	/* Step 3: after doing well, calculate how many Garrisons kept empty (and therefore are not saved)
	* in <strategic AI.h> it is defined as MAX_GARRISON_GROUPS 100. To have the same offset for the next field
	* every time, we have to pad the unused garrisons until we have stored 100
	*/
	i = SAVED_GARRISON_GROUPS - giGarrisonArraySize;
	while( i-- > 0 )
	{
	/* write an empty garison as a padding filling block */
		FileWrite( hFile, &gTempGarrisonGroup, sizeof( GARRISON_GROUP ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( GARRISON_GROUP ) )
			return FALSE;
	}

	FileWrite( hFile, gubPatrolReinforcementsDenied, giPatrolArraySize, &uiNumBytesWritten );
	if( uiNumBytesWritten != (UINT32)giPatrolArraySize )
	{
		return FALSE;
	}

	FileWrite( hFile, gubGarrisonReinforcementsDenied, giGarrisonArraySize, &uiNumBytesWritten );
	if( uiNumBytesWritten != (UINT32)giGarrisonArraySize )
	{
		return FALSE;
	}

	#ifdef JA2BETAVERSION
		ValidatePlayersAreInOneGroupOnly();
	#endif

	return TRUE;
}

BOOLEAN LoadStrategicAI( HWFILE hFile )
{
	GROUP *pGroup, *next;
	GARRISON_GROUP gTempGarrisonGroup;
	PATROL_GROUP gTempPatrolGroup;
	ARMY_COMPOSITION gTempArmyComp;
	UINT32 uiNumBytesRead;
	INT32 i;
	UINT8 ubSAIVersion;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;

	gfUnlimitedTroops = zDiffSetting[gGameOptions.ubDifficultyLevel].bUnlimitedPoolOfTroops;
	gfAggressiveQueen = zDiffSetting[gGameOptions.ubDifficultyLevel].bAggressiveQueenAi;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic6");
	FileRead( hFile, gbPadding2,							3, &uiNumBytesRead );
	if( uiNumBytesRead != 3 )
		return FALSE;
	FileRead( hFile, &gfExtraElites,					1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &giGarrisonArraySize,		4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giPatrolArraySize,			4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giReinforcementPool,		4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giForcePercentage,			4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giArmyAlertness,				4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giArmyAlertnessDecay,		4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &gfQueenAIAwake,					1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &giReinforcementPoints,	4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &giRequestPoints,				4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
		return FALSE;
	FileRead( hFile, &gubNumAwareBattles,			1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &ubSAIVersion,						1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gubQueenPriorityPhase,	1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gfFirstBattleMeanwhileScenePending,	1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gfMassFortificationOrdered,	1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gubMinEnemyGroupSize,	1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gubHoursGracePeriod, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &gusPlayerBattleVictories, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
		return FALSE;
	FileRead( hFile, &gfUseAlternateQueenPosition, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
		return FALSE;
	FileRead( hFile, &giTotalRecruitsInTraining, sizeof( giTotalRecruitsInTraining ), &uiNumBytesRead );
	if ( uiNumBytesRead != sizeof( giTotalRecruitsInTraining ) )
		return FALSE;
	FileRead( hFile, gbPadding, SAI_PADDING_BYTES, &uiNumBytesRead );
	if( uiNumBytesRead != SAI_PADDING_BYTES )
		return FALSE;
	
	//Restore the army composition
	FileRead( hFile, gArmyComp,	NUM_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ), &uiNumBytesRead );
	if( uiNumBytesRead != NUM_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ) )
		return FALSE;
	i = SAVED_ARMY_COMPOSITIONS - NUM_ARMY_COMPOSITIONS;
	while( i-- )
	{
		FileRead( hFile, &gTempArmyComp, sizeof( ARMY_COMPOSITION ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( ARMY_COMPOSITION ) )
			return FALSE;
	}

	//Restore the patrol group definitions
	if( gPatrolGroup )
	{
		MemFree( gPatrolGroup );
	}
	gPatrolGroup = (PATROL_GROUP*)MemAlloc( giPatrolArraySize * sizeof( PATROL_GROUP ) );
	FileRead( hFile, gPatrolGroup, giPatrolArraySize * sizeof( PATROL_GROUP ), &uiNumBytesRead );
	if( uiNumBytesRead != giPatrolArraySize * sizeof( PATROL_GROUP ) )
		return FALSE;
	i = SAVED_PATROL_GROUPS - giPatrolArraySize;
	while( i-- )
	{
		FileRead( hFile, &gTempPatrolGroup, sizeof( PATROL_GROUP ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( PATROL_GROUP ) )
			return FALSE;
	}

	gubSAIVersion = SAI_VERSION;
	//Load the garrison information!
	if( gGarrisonGroup )
	{
		MemFree( gGarrisonGroup );
	}

	/* giGarrisonArraySize has been zero here. Found it several times when loading
	* a differnet build of JA2.1.13. Now fixed a little bit, I hope.
	*/
	if( giGarrisonArraySize<iOrigGarrisonArraySize || giGarrisonArraySize<1 )
	{
	giGarrisonArraySize = iOrigGarrisonArraySize;
	}
	if( giGarrisonArraySize > SAVED_GARRISON_GROUPS )
	{
	giGarrisonArraySize = SAVED_GARRISON_GROUPS;
	}

	gGarrisonGroup = (GARRISON_GROUP*)MemAlloc( giGarrisonArraySize * sizeof( GARRISON_GROUP ) );
	Assert( gGarrisonGroup );
	FileRead( hFile, gGarrisonGroup, giGarrisonArraySize * sizeof( GARRISON_GROUP ), &uiNumBytesRead );
	if( uiNumBytesRead != giGarrisonArraySize * sizeof( GARRISON_GROUP ) )
	{
		return FALSE;
	}
	/* if the Savegame File contains more Garrisons then our Array, toss the remaining ones into the saucer */
	i = SAVED_GARRISON_GROUPS - giGarrisonArraySize;
	while( i-- > 0 ) /* prevent underrun, 2007-03-03, Sgt. Kolja	*/
	{
		FileRead( hFile, &gTempGarrisonGroup, sizeof( GARRISON_GROUP ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( GARRISON_GROUP ) )
		{
			return FALSE;
		}
	}

	//Load the list of reinforcement patrol points.
	if( gubPatrolReinforcementsDenied )
	{
		MemFree( gubPatrolReinforcementsDenied );
		gubPatrolReinforcementsDenied = NULL;
	}
	gubPatrolReinforcementsDenied = (UINT8*)MemAlloc( giPatrolArraySize );
		FileRead( hFile, gubPatrolReinforcementsDenied, giPatrolArraySize, &uiNumBytesRead );
		if( uiNumBytesRead != (UINT32)giPatrolArraySize )
		{
			return FALSE;
		}

	//Load the list of reinforcement garrison points.
	if( gubGarrisonReinforcementsDenied )
	{
		MemFree( gubGarrisonReinforcementsDenied );
		gubGarrisonReinforcementsDenied = NULL;
	}
	gubGarrisonReinforcementsDenied = (UINT8*)MemAlloc( giGarrisonArraySize );
		FileRead( hFile, gubGarrisonReinforcementsDenied, giGarrisonArraySize, &uiNumBytesRead );
		if( uiNumBytesRead != (UINT32)giGarrisonArraySize )
		{
			return FALSE;
		}

	#ifdef JA2BETAVERSION
		InitStrategicMovementCosts();
	#endif

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( ubSAIVersion < 6 )
	{ //Reinitialize the costs since they have changed.

		//Recreate the compositions
		memcpy( gArmyComp, gOrigArmyComp, MAX_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ) );
		EvolveQueenPriorityPhase( TRUE );

		//Recreate the patrol desired sizes
		for( i = 0; i < giPatrolArraySize; i++ )
		{
			gPatrolGroup[ i ].bSize = gOrigPatrolGroup[ i ].bSize;
		}
	}
	if( ubSAIVersion < 7 )
	{
		BuildUndergroundSectorInfoList();
	}
	if( ubSAIVersion < 8 )
	{
		ReinitializeUnvisitedGarrisons();
	}
	if( ubSAIVersion < 10 )
	{
		for( i = 0; i < giPatrolArraySize; i++ )
		{
			if( gPatrolGroup[ i ].bSize >= 16 )
			{
				gPatrolGroup[ i ].bSize = 10;
			}
		}
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->usGroupTeam == ENEMY_TEAM && pGroup->ubGroupSize >= 16 )
			{
				//accident in patrol groups being too large
				UINT8	ubGetRidOfXTroops = pGroup->ubGroupSize - 10;
				if( gbWorldSectorZ || pGroup->ubSectorX != gWorldSectorX || pGroup->ubSectorY != gWorldSectorY )
				{ //don't modify groups in the currently loaded sector.
					if( pGroup->pEnemyGroup->ubNumTroops >= ubGetRidOfXTroops )
					{
						pGroup->pEnemyGroup->ubNumTroops -= ubGetRidOfXTroops;
						pGroup->ubGroupSize -= ubGetRidOfXTroops;
					}
				}
			}
			pGroup = pGroup->next;
		}
	}

	if( ubSAIVersion < 13 )
	{
		for( i = 0; i < 255; i++ )
		{
			SectorInfo[ i ].bBloodCatPlacements = 0;
			SectorInfo[ i ].bBloodCats = -1;
		}
		InitBloodCatSectors();
		//This info is used to clean up the two new codes inserted into the
		//middle of the enumeration for battle codes.
		if( GetEnemyEncounterCode() > CREATURE_ATTACK_CODE )
		{
			SetEnemyEncounterCode( GetEnemyEncounterCode() + 2 );
		}

		if( GetExplicitEnemyEncounterCode() > CREATURE_ATTACK_CODE )
		{
			SetExplicitEnemyEncounterCode( GetExplicitEnemyEncounterCode() + 2 );
		}

	}
	if( ubSAIVersion < 14 )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( 4, 11, 1 );
		if( pSector->ubNumTroops + pSector->ubNumElites > 20 )
		{
			pSector->ubNumTroops -= 2;
		}
		pSector = FindUnderGroundSector( 3, 15, 1 );
		if( pSector->ubNumTroops + pSector->ubNumElites > 20 )
		{
			pSector->ubNumTroops -= 2;
		}
	}
	if( ubSAIVersion < 17 )
	{ //Patch all groups that have this flag set
		gubNumGroupsArrivedSimultaneously = 0;
		{
			pGroup = gpGroupList;
			while( pGroup )
			{
				if( pGroup->uiFlags & GROUPFLAG_GROUP_ARRIVED_SIMULTANEOUSLY )
				{
					pGroup->uiFlags &= ~GROUPFLAG_GROUP_ARRIVED_SIMULTANEOUSLY;
				}
				pGroup = pGroup->next;
			}
		}
	}
	if( ubSAIVersion < 18 )
	{ //adjust down the number of bloodcats based on difficulty in the two special bloodcat levels
		// HEADROCK HAM 3.6: Run the Init function instead.
		InitBloodCatSectors();
		/*
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY: //50%
				SectorInfo[ SEC_I16	].bBloodCatPlacements = 14;
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 13;
				SectorInfo[ SEC_I16	].bBloodCats = 14;
				SectorInfo[ SEC_N5	].bBloodCats = 13;
				break;
			case DIF_LEVEL_MEDIUM: //75%
				SectorInfo[ SEC_I16	].bBloodCatPlacements = 19;
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 18;
				SectorInfo[ SEC_I16	].bBloodCats = 19;
				SectorInfo[ SEC_N5	].bBloodCats = 18;
			case DIF_LEVEL_HARD: //100%
				SectorInfo[ SEC_I16	].bBloodCatPlacements = 26;
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 25;
				SectorInfo[ SEC_I16	].bBloodCats = 26;
				SectorInfo[ SEC_N5	].bBloodCats = 25;
				break;
			case DIF_LEVEL_INSANE: //150%
				SectorInfo[ SEC_I16	].bBloodCatPlacements = 39;
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 37;
				SectorInfo[ SEC_I16	].bBloodCats = 39;
				SectorInfo[ SEC_N5	].bBloodCats = 37;
				break;
		}*/
	}
	if( ubSAIVersion < 19 )
	{
		//Clear the garrison in C5
		gArmyComp[ gGarrisonGroup[ SectorInfo[ SEC_C5 ].ubGarrisonID ].ubComposition ].bPriority = 0;
		gArmyComp[ gGarrisonGroup[ SectorInfo[ SEC_C5 ].ubGarrisonID ].ubComposition ].bDesiredPopulation = 0;
	}
	if( ubSAIVersion < 20 )
	{
		gArmyComp[ QUEEN_DEFENCE ].bDesiredPopulation = 32;
		SectorInfo[ SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) ].ubNumElites = 32;
	}
	if( ubSAIVersion < 21 )
	{
		pGroup = gpGroupList;
		while( pGroup )
		{
			pGroup->uiFlags = 0;
			pGroup = pGroup->next;
		}
	}
	if( ubSAIVersion < 22 )
	{ //adjust down the number of bloodcats based on difficulty in the two special bloodcat levels
		// HEADROCK HAM 3.6: Run the Init function instead.
		InitBloodCatSectors();
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY: //50%
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 8;
				SectorInfo[ SEC_N5	].bBloodCats = 10;
				break;
			case DIF_LEVEL_MEDIUM: //75%
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 8;
				SectorInfo[ SEC_N5	].bBloodCats = 10;
			case DIF_LEVEL_HARD: //100%
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 8;
				SectorInfo[ SEC_N5	].bBloodCats = 10;
				break;
			case DIF_LEVEL_INSANE: //150%
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 12;
				SectorInfo[ SEC_N5	].bBloodCats = 15;
				break;
			default:
				SectorInfo[ SEC_N5	].bBloodCatPlacements = 8;
				SectorInfo[ SEC_N5	].bBloodCats = 10;		
			break;
		}
	}
	if( ubSAIVersion < 23 )
	{
		if( gWorldSectorX != gModSettings.ubSAISpawnSectorX || gWorldSectorY != gModSettings.ubSAISpawnSectorY || !gbWorldSectorZ )
		{
			SectorInfo[ SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) ].ubNumElites = 32;
		}
	}
	if( ubSAIVersion < 24 )
	{
		//If the queen has escaped to the basement, do not use the profile insertion info
		//when we finally go down there, otherwise she will end up in the wrong spot, possibly inside
		//the walls.
		if( !gubFact[ FACT_QUEEN_DEAD ] && gMercProfiles[ QUEEN ].bSectorZ == 1 )
		if( gbWorldSectorZ != 1 || gWorldSectorX != 16 || gWorldSectorY != 3 )
		{ //We aren't in the basement sector
			gMercProfiles[ QUEEN ].fUseProfileInsertionInfo = FALSE;
		}
		else
		{ //We are in the basement sector, relocate queen to proper position.
			INT32 i;
			for( i = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; i++ )
			{
				if( MercPtrs[ i ]->ubProfile == QUEEN )
				{ //Found queen, relocate her to 16866
					BumpAnyExistingMerc( 16866 );
					TeleportSoldier( MercPtrs[ i ], 16866, TRUE );
					break;
				}
			}
		}
	}
	if( ubSAIVersion < 25 )
	{
		if( gubFact[ FACT_SKYRIDER_CLOSE_TO_CHOPPER ] )
		{
			gMercProfiles[ SKYRIDER ].fUseProfileInsertionInfo = FALSE;
		}
	}

	//KM : July 21, 1999 patch fix
	if( ubSAIVersion < 26 )
	{
		INT32 i;
		for( i = 0; i < 255; ++i )
		{
			if( SectorInfo[ i ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] +
					SectorInfo[ i ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] +
					SectorInfo[ i ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] > 20 )
			{
				SectorInfo[ i ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] = 0;
				SectorInfo[ i ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] = 20;
				SectorInfo[ i ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] = 0;
			}
		}
	}

	//KM : August 4, 1999 patch fix
	//	 This addresses the problem of not having any soldiers in sector N7 when playing the game under easy difficulty.
	//		If captured and interrogated, the player would find no soldiers defending the sector.	This changes the composition
	//	 so that it will always be there, and adds the soldiers accordingly if the sector isn't loaded when the update is made.
	if( ubSAIVersion < 27 )
	{
		if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		{
			if( gWorldSectorX != 7 || gWorldSectorY != 14 || gbWorldSectorZ )
			{
				INT32 cnt, iRandom;
				INT32 iEliteChance, iTroopChance, iAdminChance;
				INT32 iStartPop, iDesiredPop, iPriority;
				SECTORINFO *pSector = NULL;

				//Change the garrison composition to LEVEL1_DEFENCE from LEVEL2_DEFENCE
				pSector = &SectorInfo[ SEC_N7 ];
				gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition = LEVEL1_DEFENCE;

				iStartPop = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bStartPopulation;
				iDesiredPop = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bDesiredPopulation;
				iPriority = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bPriority;
				iEliteChance = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bElitePercentage;
				iTroopChance = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bTroopPercentage + iEliteChance;
				iAdminChance = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bAdminPercentage;

				if( iStartPop )
				{
					// if population is less than maximum
					if( iStartPop != iMaxEnemyGroupSize )
					{
						// then vary it a bit (+/- 25%)
						iStartPop = iStartPop * ( 100 + ( Random ( 51 ) - 25 ) ) / 100;
					}

					iStartPop = max( gubMinEnemyGroupSize, min( iMaxEnemyGroupSize, iStartPop ) );
					cnt = iStartPop;

					if( iAdminChance )
					{
						pSector->ubNumAdmins = iAdminChance * iStartPop / 100;
					}
					else while( cnt-- )
					{ //for each person, randomly determine the types of each soldier.
						{
							iRandom = Random( 100 );
							if( iRandom < iEliteChance )
							{
								pSector->ubNumElites++;
							}
							else if( iRandom < iTroopChance )
							{
								pSector->ubNumTroops++;
							}
						}
					}
				}
			}
		}
	}

	if( ubSAIVersion < 28 )
	{
		GROUP *pNext;
		if( !StrategicMap[ CALCULATE_STRATEGIC_INDEX( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ) ].fEnemyControlled )
		{ //Eliminate all enemy groups in this sector, because the player owns the sector, and it is not
			//possible for them to spawn there!
			pGroup = gpGroupList;
			while( pGroup )
			{
				pNext = pGroup->next;
				if ( pGroup->usGroupTeam != OUR_TEAM )
				{
					if( pGroup->ubSectorX == gModSettings.ubSAISpawnSectorX && pGroup->ubSectorY == gModSettings.ubSAISpawnSectorY && !pGroup->ubPrevX && !pGroup->ubPrevY )
					{
						ClearPreviousAIGroupAssignment( pGroup );
						RemovePGroup( pGroup );
					}
				}
				pGroup = pNext;
			}
		}
	}
	if( ubSAIVersion < 29 )
	{
		InitStrategicMovementCosts();
	}

	//KM : Aug 11, 1999 -- Patch fix:	Blindly update the airspace control.	There is a bug somewhere
	//		that is failing to keep this information up to date, and I failed to find it.	At least this
	//		will patch saves.
	UpdateAirspaceControl( );

	EvolveQueenPriorityPhase( TRUE );

	//Count and correct the floating groups
	pGroup = gpGroupList;
	while( pGroup )
	{
		next = pGroup->next; //store the next node as pGroup could be deleted!
		if ( pGroup->usGroupTeam != OUR_TEAM )
		{
			if( !pGroup->fBetweenSectors )
			{
				if( pGroup->ubSectorX != gWorldSectorX ||
						pGroup->ubSectorY != gWorldSectorY ||
						gbWorldSectorZ )
				{
					UINT8 groupid = pGroup->ubGroupID;

					RepollSAIGroup( pGroup );

					// it is possible that the group gets destroyed inbetween, so recheck on that
					pGroup = GetGroup( groupid );

					if ( pGroup )
						ValidateGroup( pGroup );
				}
			}
		}
		pGroup = next; //advance the node
	}

	#ifdef JA2BETAVERSION
		LogStrategicMsg( "" );
		LogStrategicMsg( "-------------------------------------------------" );
		LogStrategicMsg( "Restoring saved game at Day %02d, %02d:%02d ", GetWorldDay(), GetWorldHour(), GetWorldMinutesInDay()%60 );
		LogStrategicMsg( "-------------------------------------------------" );
		LogStrategicMsg( "" );
	#endif

	//Update the version number to the most current.
	gubSAIVersion = SAI_VERSION;

	ValidateWeights( 28 );
	ValidatePendingGroups();
	#ifdef JA2BETAVERSION
		ValidatePlayersAreInOneGroupOnly();
	#endif

	return TRUE;
}

void EvolveQueenPriorityPhase( BOOLEAN fForceChange )
{
	INT32 i, index, num, iFactor;
	INT32 iChange, iNew, iNumSoldiers, iNumPromotions;
	SECTORINFO *pSector;
	UINT8 ubOwned[ MAX_ARMY_COMPOSITIONS ];
	UINT8 ubTotal[ MAX_ARMY_COMPOSITIONS ];
	UINT8 ubNewPhase;
	ubNewPhase = CurrentPlayerProgressPercentage() / 10;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic7");

	if( !fForceChange && ubNewPhase == gubQueenPriorityPhase )
	{
		return;
	}

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( gubQueenPriorityPhase > ubNewPhase )
	{
		#ifdef JA2BETAVERSION
			LogStrategicEvent( "The queen's defence priority has decreased from %d0%% to %d0%%.", gubQueenPriorityPhase, ubNewPhase );
		#endif
	}
	else if( gubQueenPriorityPhase < ubNewPhase )
	{
		#ifdef JA2BETAVERSION
			LogStrategicEvent( "The queen's defence priority has increased from %d0%% to %d0%%.", gubQueenPriorityPhase, ubNewPhase );
		#endif
	}
	else
	{
		#ifdef JA2BETAVERSION
			LogStrategicEvent( "The queen's defence priority is the same (%d0%%), but has been forced to update.", gubQueenPriorityPhase );
		#endif
	}

	gubQueenPriorityPhase = ubNewPhase;

	//The phase value refers to the deviation percentage she will apply to original garrison values.
	//All sector values are evaluated to see how many of those sectors are enemy controlled.	If they
	//are controlled by her, the desired number will be increased as well as the priority.	On the other
	//hand, if she doesn't own those sectors, the values will be decreased instead.	All values are based off of
	//the originals.
	memset( ubOwned, 0, MAX_ARMY_COMPOSITIONS );
	memset( ubTotal, 0, MAX_ARMY_COMPOSITIONS );

	//Record the values required to calculate the percentage of each composition type that the queen controls.
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		index = gGarrisonGroup[ i ].ubComposition;
		if( StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( gGarrisonGroup[ i ].ubSectorID ) ].fEnemyControlled )
		{
			ubOwned[ index ]++;
		}
		ubTotal[ index ]++;
	}

	//Go through the *majority* of compositions and modify the priority/desired values.
	for( i = 0; i < NUM_ARMY_COMPOSITIONS; i++ )
	{
		switch( i )
		{
			case QUEEN_DEFENCE:
			case MEDUNA_DEFENCE:
			case MEDUNA_SAMSITE:
			case LEVEL1_DEFENCE:
			case LEVEL2_DEFENCE:
			case LEVEL3_DEFENCE:
			case OMERTA_WELCOME_WAGON:
			case ROADBLOCK:
			//case SANMONA_SMALL:
				//don't consider these compositions
				continue;
		}
		//If the queen owns them ALL, then she gets the maximum defensive bonus.	If she owns NONE,
		//then she gets a maximum defensive penalty.	Everything else lies in the middle.	The legal
		//range is +-50.
		if( ubTotal[ i ] )
		{
			iFactor = (ubOwned[ i ] * 100 / ubTotal[ i ]) - 50;
		}
		else
		{
			iFactor = -50;
		}

		if ( gfAggressiveQueen )
			iFactor = abs(iFactor); // Madd: The Queen never gets a defensive penalty.	The range for iFactor becomes 0-50.	This should increase the priorities for sectors outside Meduna as the queen loses sectors and keep up the attacks.	This is only going to work well if the reinforcement pool is unlimited, as sectors priorities won't ever go down really low.

		iFactor = iFactor * gubQueenPriorityPhase / 10;

		//modify priority by + or - 25% of original
		if( gArmyComp[ i ].bPriority )
		{
			num = gOrigArmyComp[ i ].bPriority + iFactor / 2;
			num = min( max( 0, num ), 100 );
			gArmyComp[ i ].bPriority = (INT8)num;
		}

		//modify desired population by + or - 50% of original population
		num = gOrigArmyComp[ i ].bDesiredPopulation * (100 + iFactor) / 100;
		num = min( max( 6, num ), iMaxEnemyGroupSize );
		gArmyComp[ i ].bDesiredPopulation = (INT8)num;

		//if gfExtraElites is set, then augment the composition sizes
		if( gfExtraElites && iFactor >= 15 && gArmyComp[ i ].bElitePercentage )
		{
		
		INT32 val;
			if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
				val = 1;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
				val = 2;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
				val = 3;
			else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
				val = 4;
			else
			{
				val = Random (4);
				if (val == 0) val = 1;
			}		
			iChange = val * 5;

			//iChange = gGameOptions.ubDifficultyLevel * 5;
		
			//increase elite % (max 100)
			iNew = gArmyComp[ i ].bElitePercentage + iChange;
			iNew = (INT8)min( 100, iNew );
			gArmyComp[ i ].bElitePercentage = (INT8)iNew;

			//decrease troop % (min 0)
			iNew = gArmyComp[ i ].bTroopPercentage - iChange;
			iNew = (INT8)max( 0, iNew );
			gArmyComp[ i ].bTroopPercentage = (INT8)iNew;
		}
	}
	if( gfExtraElites )
	{
		//Turn off the flag so that this doesn't happen everytime this function is called!
		gfExtraElites = FALSE;

		for( i = 0; i < giGarrisonArraySize; i++ )
		{
			//if we are dealing with extra elites, then augment elite compositions (but only if they exist in the sector).
			//If the queen still owns the town by more than 65% (iFactor >= 15), then upgrade troops to elites in those sectors.
			index = gGarrisonGroup[ i ].ubComposition;
			switch( index )
			{
				case QUEEN_DEFENCE:
				case MEDUNA_DEFENCE:
				case MEDUNA_SAMSITE:
				case LEVEL1_DEFENCE:
				case LEVEL2_DEFENCE:
				case LEVEL3_DEFENCE:
				case OMERTA_WELCOME_WAGON:
				case ROADBLOCK:
				//case SANMONA_SMALL:
					//don't consider these compositions
					continue;
			}
			pSector = &SectorInfo[ gGarrisonGroup[ i ].ubSectorID ];
			if( ubTotal[ index ] )
			{
				iFactor = (ubOwned[ index ] * 100 / ubTotal[ index ]) - 50;
			}
			else
			{
				iFactor = -50;
			}
			if( iFactor >= 15 )
			{ //Make the actual elites in sector match the new garrison percentage
				if( !gfWorldLoaded || gbWorldSectorZ ||
						gWorldSectorX != SECTORX( gGarrisonGroup[ i ].ubSectorID ) ||
						gWorldSectorY != SECTORY( gGarrisonGroup[ i ].ubSectorID ) )
				{ //Also make sure the sector isn't currently loaded!
					iNumSoldiers = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites;
					iNumPromotions = gArmyComp[ index ].bElitePercentage * iNumSoldiers / 100 - pSector->ubNumElites;

					if( iNumPromotions > 0 )
					{
						while( iNumPromotions-- )
						{
							if( pSector->ubNumAdmins )
							{
								pSector->ubNumAdmins--;
							}
							else if( pSector->ubNumTroops )
							{
								pSector->ubNumTroops--;
							}
							else
							{
								Assert( 0 );
							}
							pSector->ubNumElites++;
						}
						Assert( iNumSoldiers == pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites );
					}
				}
			}
		}
	}
	//Recalculate all of the weights.
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		RecalculateGarrisonWeight( i );
	}
}

void ExecuteStrategicAIAction( UINT16 usActionCode, INT16 sSectorX, INT16 sSectorY, 
							   INT32 option1, INT32 option2 )
{
	GROUP *pGroup, *pGroup0, *pGroup1, *pGroup2, *pGroup3, *pPendingGroup = NULL;
	SECTORINFO *pSector;
	UINT8 ubSectorID;
	UINT8 ubSourceSectorID;
	UINT8 ubTargetSectorID;
	UINT8 stagesector0, stagesector1, stagesector2, stagesector3;
	UINT8 assaultsector0, assaultsector1, assaultsector2, assaultsector3;
	unsigned ubNumSoldiers;
	UINT8 ubNumRobots = 0;
	UINT8 ubNumTanks = 0;
	UINT8 ubNumJeeps = 0;
	UINT32 totalusedsoldiers = 0;
	UINT8 grouptroops[4] = {0,0,0,0};
	UINT8 groupelites[4] = {0,0,0,0};
	UINT8 grouprobots[4] = {0,0,0,0};
	UINT8 grouptanks[4] = {0,0,0,0};
	UINT8 groupjeeps[4] = {0, 0, 0, 0};

	UINT32 difficultyMod;
	switch (gGameOptions.ubDifficultyLevel)
	{
	case DIF_LEVEL_EASY:	difficultyMod = 1; break;
	case DIF_LEVEL_MEDIUM:	difficultyMod = 2; break;
	case DIF_LEVEL_HARD:	difficultyMod = 3; break;
	case DIF_LEVEL_INSANE:	difficultyMod = 4; break;
	default:				difficultyMod = 1; break;
	}

	switch( usActionCode )
	{
		case STRATEGIC_AI_ACTION_WAKE_QUEEN:
			WakeUpQueen();
			break;

		case STRATEGIC_AI_ACTION_QUEEN_DEAD:
			gfQueenAIAwake = FALSE;
			break;

		case STRATEGIC_AI_ACTION_KINGPIN_DEAD:
			//Immediate send a small garrison to C5 (to discourage access to Tony the dealer)
			/*
	 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	* added NULL fix, 2007-03-03, Sgt. Kolja *

			for( i = 0; i < giGarrisonArraySize; i++ )
			{
				if( gGarrisonGroup[ i ].ubComposition == SANMONA_SMALL )
				{
					//Setup the composition so from now on the queen will consider this an important sector
					//to hold.
					gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bPriority = 65;
					gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bTroopPercentage = 100;
					gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bDesiredPopulation = 5;
					RequestHighPriorityGarrisonReinforcements( i, (UINT8)(2 + Random( 4 )) ); //send 2-5 soldiers now.
					break;
				}
			}
			*/
			break;

		case NPC_ACTION_SEND_SOLDIERS_TO_DRASSEN:
			//Send 6, 9, or 12 troops (based on difficulty) one of the Drassen sectors.	If nobody is there when they arrive,
			//those troops will get reassigned.

			//if( Chance( 50 ) )
			//{
			//	ubSectorID = SEC_D13;
			//}
			//else if( Chance( 60 ) )
			//{
			//	ubSectorID = SEC_B13;
			//}
			//else
			//{
			//	ubSectorID = SEC_C13;
			//}
			//ubNumSoldiers = (UINT8)(3 + gGameOptions.ubDifficultyLevel * 3);
			//pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_P3, 0, ubNumSoldiers, 0 );

			//if( !gGarrisonGroup[ SectorInfo[ ubSectorID ].ubGarrisonID ].ubPendingGroupID )
			//{
			//	pGroup->pEnemyGroup->ubIntention = STAGE;
			//	gGarrisonGroup[ SectorInfo[ ubSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
			//}
			//else
			//{ //this should never happen (but if it did, then this is the best way to deal with it).
			//	pGroup->pEnemyGroup->ubIntention = PURSUIT;
			//}

			//Madd: unlimited reinforcements?
			//if ( !gfUnlimitedTroops )
			//	giReinforcementPool -= ubNumSoldiers;

			//giReinforcementPool = max( giReinforcementPool, 0 );

			//MoveSAIGroupToSector( &pGroup, ubSectorID, EVASIVE, pGroup->pEnemyGroup->ubIntention );

			if ( !gGameExternalOptions.ubSendTroopsToDrassen )
				break;

			ubSourceSectorID = SEC_H13;
			ubTargetSectorID = SEC_D13;

			if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
				ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

			// make sure that we use at least the min group size
			ubNumSoldiers = (UINT8)( __max(zDiffSetting[gGameOptions.ubDifficultyLevel].iCounterAttackGroupSize, zDiffSetting[gGameOptions.ubDifficultyLevel].iMinEnemyGroupSize) );

			// anv: replace one of soldiers with tank
			if ( ubNumSoldiers )
			{
				for( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
				{
					InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], Random(10) < gGameOptions.ubDifficultyLevel);
					totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
				}
			}

			pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
			pGroup1 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
			pGroup2 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
			pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

			if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
			{
				pGroup0->pEnemyGroup->ubIntention = STAGE;
				pGroup1->pEnemyGroup->ubIntention = STAGE;
				pGroup2->pEnemyGroup->ubIntention = STAGE;
				pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

				gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;

			}
			else
			{ //this should never happen (but if it did, then this is the best way to deal with it).
				pGroup0->pEnemyGroup->ubIntention = PURSUIT;
				pGroup1->pEnemyGroup->ubIntention = PURSUIT;
				pGroup2->pEnemyGroup->ubIntention = PURSUIT;
				pGroup3->pEnemyGroup->ubIntention = PURSUIT;
			}

			//Madd: unlimited reinforcements?
			if ( !gfUnlimitedTroops )
			{
				giReinforcementPool -= totalusedsoldiers;

				giReinforcementPool = max( giReinforcementPool, 0 );
			}

			MoveSAIGroupToSector( &pGroup0, SEC_D14, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
			MoveSAIGroupToSector( &pGroup1, SEC_E13, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
			MoveSAIGroupToSector( &pGroup2, SEC_D12, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
			MoveSAIGroupToSector( &pGroup3, SEC_D14, EVASIVE, pGroup3->pEnemyGroup->ubIntention );

			break;

		case NPC_ACTION_SEND_SOLDIERS_TO_CHITZENA:
		case NPC_ACTION_SEND_SOLDIERS_TO_GRUMM:
		case NPC_ACTION_SEND_SOLDIERS_TO_CAMBRIA:
		case NPC_ACTION_SEND_SOLDIERS_TO_ALMA:
		case NPC_ACTION_SEND_SOLDIERS_TO_BALIME:
			{
				if ( gGameExternalOptions.ubAgressiveStrategicAI < 1)
				{
					ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, sSectorX, sSectorY );
					break;
				}

				// make sure that we use at least the min group size
				ubNumSoldiers = (UINT8)( __max(zDiffSetting[gGameOptions.ubDifficultyLevel].iCounterAttackGroupSize, zDiffSetting[gGameOptions.ubDifficultyLevel].iMinEnemyGroupSize) );

				// anv: replace one of soldiers with tank
				for( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
				{
					InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
					totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
				}
				
				switch( usActionCode )
				{
					case NPC_ACTION_SEND_SOLDIERS_TO_CHITZENA:
						ubSourceSectorID = SEC_H3;
						ubTargetSectorID = SEC_B2;
						
						if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
							ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						stagesector0 = ubSourceSectorID;
						stagesector1 = ubSourceSectorID;
						stagesector2 = ubSourceSectorID;
						stagesector3 = ubSourceSectorID;

						assaultsector0 = SEC_B1;
						assaultsector1 = SEC_C2;
						assaultsector2 = SEC_B3;
						assaultsector3 = SEC_B1;
																						
						break;
					case NPC_ACTION_SEND_SOLDIERS_TO_GRUMM:
						ubSourceSectorID = SEC_M3;
						ubTargetSectorID = SEC_H3;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
							ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						stagesector0 = SEC_M2;
						stagesector1 = ubSourceSectorID;
						stagesector2 = ubSourceSectorID;
						stagesector3 = SEC_M6;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( stagesector0 ), SECTORY( stagesector0 ), ENEMY_TEAM ) )
							stagesector0 = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( stagesector3 ), SECTORY( stagesector3 ), ENEMY_TEAM ) )
							stagesector3 = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						assaultsector0 = SEC_H4;
						assaultsector1 = SEC_I3;
						assaultsector2 = SEC_I3;
						assaultsector3 = SEC_G3;

						break;
					case NPC_ACTION_SEND_SOLDIERS_TO_CAMBRIA:
						ubSourceSectorID = SEC_M6;
						ubTargetSectorID = SEC_H8;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
							ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						stagesector0 = ubSourceSectorID;
						stagesector1 = ubSourceSectorID;
						stagesector2 = ubSourceSectorID;
						stagesector3 = SEC_L11;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( stagesector3 ), SECTORY( stagesector3 ), ENEMY_TEAM ) )
						{
							stagesector3 = SEC_N9;

							if ( !NumNonPlayerTeamMembersInSector( SECTORX( stagesector3 ), SECTORY( stagesector3 ), ENEMY_TEAM ) )
								stagesector3 = ubSourceSectorID;
						}

						assaultsector0 = SEC_I8;
						assaultsector1 = SEC_I8;
						assaultsector2 = SEC_H7;
						assaultsector3 = SEC_H9;

						break;
					case NPC_ACTION_SEND_SOLDIERS_TO_ALMA:
						ubSourceSectorID = SEC_G9;
						ubTargetSectorID = SEC_H13;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
						{
							ubSourceSectorID = SEC_J9;
							ubTargetSectorID = SEC_I13;

							if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
								ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );
						}

						stagesector0 = ubSourceSectorID;
						stagesector1 = ubSourceSectorID;
						stagesector2 = ubSourceSectorID;
						stagesector3 = ubSourceSectorID;

						if ( ubSourceSectorID == SEC_G9 )
						{
							assaultsector0 = SEC_H12;
							assaultsector1 = SEC_H12;
							assaultsector2 = SEC_G13;
							assaultsector3 = SEC_G13;
						}
						else
						{
							assaultsector0 = SEC_J13;
							assaultsector1 = SEC_J13;
							assaultsector2 = SEC_I12;
							assaultsector3 = SEC_I12;
						}

						break;
					case NPC_ACTION_SEND_SOLDIERS_TO_BALIME:
						ubSourceSectorID = SEC_N5;
						ubTargetSectorID = SEC_L11;

						if ( !NumNonPlayerTeamMembersInSector( SECTORX( ubSourceSectorID ), SECTORY( ubSourceSectorID ), ENEMY_TEAM ) )
							ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

						stagesector0 = ubSourceSectorID;
						stagesector1 = ubSourceSectorID;
						stagesector2 = ubSourceSectorID;
						stagesector3 = ubSourceSectorID;

						assaultsector0 = SEC_K11;
						assaultsector1 = SEC_L10;
						assaultsector2 = SEC_L10;
						assaultsector3 = SEC_K11;

						break;
				}

				pGroup0 = CreateNewEnemyGroupDepartingFromSector( stagesector0, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
				pGroup1 = CreateNewEnemyGroupDepartingFromSector( stagesector1, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
				pGroup2 = CreateNewEnemyGroupDepartingFromSector( stagesector2, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
				pGroup3 = CreateNewEnemyGroupDepartingFromSector( stagesector3, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

				if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
				{
					pGroup0->pEnemyGroup->ubIntention = STAGE;
					pGroup1->pEnemyGroup->ubIntention = STAGE;
					pGroup2->pEnemyGroup->ubIntention = STAGE;
					pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

					gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;
				}
				else
				{
					//this should never happen (but if it did, then this is the best way to deal with it).
					pGroup0->pEnemyGroup->ubIntention = PURSUIT;
					pGroup1->pEnemyGroup->ubIntention = PURSUIT;
					pGroup2->pEnemyGroup->ubIntention = PURSUIT;
					pGroup3->pEnemyGroup->ubIntention = PURSUIT;
				}

				MoveSAIGroupToSector( &pGroup0, assaultsector0, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
				MoveSAIGroupToSector( &pGroup1, assaultsector1, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
				MoveSAIGroupToSector( &pGroup2, assaultsector2, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
				MoveSAIGroupToSector( &pGroup3, assaultsector3, EVASIVE, pGroup3->pEnemyGroup->ubIntention );

				//Madd: unlimited reinforcements?
				if ( !gfUnlimitedTroops )
				{
					giReinforcementPool -= totalusedsoldiers;

					giReinforcementPool = max( giReinforcementPool, 0 );
				}
			}
			break;

		case NPC_ACTION_GLOBAL_OFFENSIVE_1:
		case NPC_ACTION_GLOBAL_OFFENSIVE_2:		// for now, just a copy...
			{
				if ( gGameExternalOptions.ubAgressiveStrategicAI < 2 )
				{
					break;
				}
								
				// depending on which cities the player currently holds, we send out attack on multiple cities. We try to make these attacks occur simultaneously, so the player will have to fend off
				// multiple gigantic attacks on different cities. Ideally, the attacks will be timed so well that the player cannot use a sqaud in both battles, even with use of the helicopter
				// we first have to check which cities we have to attack. For that, we simply check wether there are troops in the target sector. If not, we will attack here
				BOOLEAN fAttack_Grumm		= !(SectorInfo[ SEC_H3 ].ubNumTroops > 0);
				BOOLEAN fAttack_Cambria		= !(SectorInfo[ SEC_H8 ].ubNumTroops > 0);
				BOOLEAN fCambriaSAMOccupied	= !(SectorInfo[ SEC_I8 ].ubNumTroops > 0);
				BOOLEAN fAttack_Alma		= !(SectorInfo[ SEC_I13 ].ubNumTroops > 0);
				BOOLEAN fAttack_Balime		= !(SectorInfo[ SEC_L11 ].ubNumTroops > 0);
				BOOLEAN fAttack_Chitzena	= !(SectorInfo[ SEC_L11 ].ubNumTroops > 0);
			
				ubNumSoldiers = (UINT8)( gubMinEnemyGroupSize + difficultyMod * 3);
								
				if ( fAttack_Grumm )
				{
					// anv: replace one of soldiers with tank
					for ( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
					{
						InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
						totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
					}

					ubSourceSectorID = SEC_M6;
					ubTargetSectorID = SEC_H3;

					if ( !(SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
						ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

					pGroup0 = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
					pGroup1 = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
					pGroup2 = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
					pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
					{
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						pGroup1->pEnemyGroup->ubIntention = STAGE;
						pGroup2->pEnemyGroup->ubIntention = STAGE;
						pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

						gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;

					}
					else
					{
						//this should never happen (but if it did, then this is the best way to deal with it).
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
						pGroup1->pEnemyGroup->ubIntention = PURSUIT;
						pGroup2->pEnemyGroup->ubIntention = PURSUIT;
						pGroup3->pEnemyGroup->ubIntention = PURSUIT;
					}

					MoveSAIGroupToSector( &pGroup0, SEC_H4, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup1, SEC_I3, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup2, SEC_I3, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup3, SEC_G3, EVASIVE, pGroup3->pEnemyGroup->ubIntention );
				}

				if ( fAttack_Cambria )
				{
					// anv: replace one of soldiers with tank
					for ( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
					{
						InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
						totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
					}

					ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

					// if the SAM was liberated by the player, we cannot stage there... 
					// however, the SAM itself is also a very promising target... If we conquer that, the player cannot move his forces via helicopter, thus he cannot reinforce his other cities against simultaneous attacks
					if ( fCambriaSAMOccupied )
					{
						ubTargetSectorID = SEC_I8;

						assaultsector0 = SEC_J8;
						assaultsector1 = SEC_J8;
						assaultsector2 = SEC_I7;
						assaultsector3 = SEC_I9;
					}
					else
					{
						ubTargetSectorID = SEC_H8;

						assaultsector0 = SEC_I8;
						assaultsector1 = SEC_I8;
						assaultsector2 = SEC_H7;
						assaultsector3 = SEC_H9;
					}

					pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
					pGroup1 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
					pGroup2 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
					pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );
					
					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
					{
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						pGroup1->pEnemyGroup->ubIntention = STAGE;
						pGroup2->pEnemyGroup->ubIntention = STAGE;
						pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

						gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;

					}
					else
					{
						//this should never happen (but if it did, then this is the best way to deal with it).
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
						pGroup1->pEnemyGroup->ubIntention = PURSUIT;
						pGroup2->pEnemyGroup->ubIntention = PURSUIT;
						pGroup3->pEnemyGroup->ubIntention = PURSUIT;
					}

					MoveSAIGroupToSector( &pGroup0, assaultsector0, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup1, assaultsector1, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup2, assaultsector2, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup3, assaultsector3, EVASIVE, pGroup3->pEnemyGroup->ubIntention );
				}

				if ( fAttack_Alma )
				{
					// anv: replace one of soldiers with tank
					for ( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
					{
						InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
						totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
					}

					ubSourceSectorID = SEC_M6;
					ubTargetSectorID = SEC_I13;

					if ( !(SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
						ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

					pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
					pGroup1 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
					pGroup2 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
					pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
					{
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						pGroup1->pEnemyGroup->ubIntention = STAGE;
						pGroup2->pEnemyGroup->ubIntention = STAGE;
						pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

						gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;
					}
					else
					{
						//this should never happen (but if it did, then this is the best way to deal with it).
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
						pGroup1->pEnemyGroup->ubIntention = PURSUIT;
						pGroup2->pEnemyGroup->ubIntention = PURSUIT;
						pGroup3->pEnemyGroup->ubIntention = PURSUIT;
					}

					MoveSAIGroupToSector( &pGroup0, SEC_J13, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup1, SEC_J13, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup2, SEC_I12, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup3, SEC_I12, EVASIVE, pGroup3->pEnemyGroup->ubIntention );
				}

				if ( fAttack_Balime )
				{
					// anv: replace one of soldiers with tank
					for ( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
					{
						InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
						totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
					}

					ubSourceSectorID = SEC_M2;
					ubTargetSectorID = SEC_L11;

					if ( !(SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
						ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );

					pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
					pGroup1 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
					pGroup2 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
					pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
					{
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						pGroup1->pEnemyGroup->ubIntention = STAGE;
						pGroup2->pEnemyGroup->ubIntention = STAGE;
						pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

						gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;

					}
					else
					{
						//this should never happen (but if it did, then this is the best way to deal with it).
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
						pGroup1->pEnemyGroup->ubIntention = PURSUIT;
						pGroup2->pEnemyGroup->ubIntention = PURSUIT;
						pGroup3->pEnemyGroup->ubIntention = PURSUIT;
					}

					MoveSAIGroupToSector( &pGroup0, SEC_K11, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup1, SEC_L10, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup2, SEC_L10, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
					MoveSAIGroupToSector( &pGroup3, SEC_K11, EVASIVE, pGroup3->pEnemyGroup->ubIntention );
				}

				if ( fAttack_Chitzena )
				{
					// anv: replace one of soldiers with tank
					for ( UINT8 ubCounter = 0; ubCounter < 4; ++ubCounter )
					{
						InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[ubCounter], groupelites[ubCounter], grouprobots[ubCounter], groupjeeps[ubCounter], grouptanks[ubCounter], ubNumSoldiers > 0);
						totalusedsoldiers += grouptroops[ubCounter] + groupelites[ubCounter] + grouprobots[ubCounter] + grouptanks[ubCounter] + groupjeeps[ubCounter];
					}

					ubSourceSectorID = SEC_H2;
					ubTargetSectorID = SEC_B2;

					// we only attack chitzena if our staging area in grumm is still not liberated. 
					// If we were to use our usual staging points in Meduna, the Chitzena and Grumm attacks would very likely combine in Grumm Mine. 2 double attacks in the same sector are a 'bit' too much
					if ( (SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
					{
						pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
						pGroup1 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[1], groupelites[1], grouprobots[1], grouptanks[1], groupjeeps[1] );
						pGroup2 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[2], groupelites[2], grouprobots[2], grouptanks[2], groupjeeps[2] );
						pGroup3 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, 0, grouptroops[3], groupelites[3], grouprobots[3], grouptanks[3], groupjeeps[3] );

						if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID )
						{
							pGroup0->pEnemyGroup->ubIntention = STAGE;
							pGroup1->pEnemyGroup->ubIntention = STAGE;
							pGroup2->pEnemyGroup->ubIntention = STAGE;
							pGroup3->pEnemyGroup->ubIntention = REINFORCEMENTS;

							gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup3->ubGroupID;

						}
						else
						{
							//this should never happen (but if it did, then this is the best way to deal with it).
							pGroup0->pEnemyGroup->ubIntention = PURSUIT;
							pGroup1->pEnemyGroup->ubIntention = PURSUIT;
							pGroup2->pEnemyGroup->ubIntention = PURSUIT;
							pGroup3->pEnemyGroup->ubIntention = PURSUIT;
						}

						MoveSAIGroupToSector( &pGroup0, SEC_B1, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
						MoveSAIGroupToSector( &pGroup1, SEC_C2, EVASIVE, pGroup1->pEnemyGroup->ubIntention );
						MoveSAIGroupToSector( &pGroup2, SEC_B3, EVASIVE, pGroup2->pEnemyGroup->ubIntention );
						MoveSAIGroupToSector( &pGroup3, SEC_B1, EVASIVE, pGroup3->pEnemyGroup->ubIntention );
					}
				}

				//Madd: unlimited reinforcements?
				if ( !gfUnlimitedTroops )
				{
					giReinforcementPool -= totalusedsoldiers;

					giReinforcementPool = max( giReinforcementPool, 0 );
				}

				if ( usActionCode == NPC_ACTION_GLOBAL_OFFENSIVE_1 )
					SetFactTrue( FACT_GLOBAL_OFFENSIVE_1_ORDERED );
				else if ( usActionCode == NPC_ACTION_GLOBAL_OFFENSIVE_2 )
					SetFactTrue( FACT_GLOBAL_OFFENSIVE_2_ORDERED );
			}
			break;

		case NPC_ACTION_SEND_SOLDIERS_TO_BATTLE_LOCATION:
			//Send 4, 8, or 12 troops (based on difficulty) to the location of the first battle.	If nobody is there when they arrive,
			//those troops will get reassigned.
			ubSectorID = (UINT8)STRATEGIC_INDEX_TO_SECTOR_INFO( sWorldSectorLocationOfFirstBattle );
			pSector = &SectorInfo[ ubSectorID ];

			ubNumSoldiers = (UINT8)(gubMinEnemyGroupSize + difficultyMod * 4);
			
			// anv: replace one of soldiers with tank	
			if ( ubNumSoldiers )
			{
				InitializeGroup(GROUP_TYPE_ATTACK, ubNumSoldiers, grouptroops[0], groupelites[0], grouprobots[0], groupjeeps[0], grouptanks[0], Random(10) < difficultyMod);
				totalusedsoldiers += grouptroops[0] + groupelites[0] + grouprobots[0], grouptanks[0] + groupjeeps[0];
			}

			pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );

			//Madd: unlimited reinforcements?
			if ( !gfUnlimitedTroops )
			{
				giReinforcementPool -= totalusedsoldiers;

				giReinforcementPool = max( giReinforcementPool, 0 );
			}

			//Determine if the battle location actually has a garrison assignment.	If so, and the following
			//checks succeed, the enemies will be sent to attack and reinforce that sector.	Otherwise, the
			//enemies will simply check it out, then leave.
			if( pSector->ubGarrisonID != NO_GARRISON )
			{ //sector has a garrison
				if ( !NumNonPlayerTeamMembersInSector( (INT16)SECTORX( ubSectorID ), (INT16)SECTORY( ubSectorID ), ENEMY_TEAM ) )
				{ //no enemies are here
					if( gArmyComp[ !gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bPriority )
					{ //the garrison is important
						if( !gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
						{ //the garrison doesn't have reinforcements already on route.
							gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
							MoveSAIGroupToSector( &pGroup, ubSectorID, STAGE, REINFORCEMENTS );
							break;
						}
					}
				}
			}
			else
			{
				MoveSAIGroupToSector( &pGroup, ubSectorID, EVASIVE, PURSUIT );
			}
			break;

		// WDS - New AI
		case NPC_ACTION_SEND_WEIGHTED_ASSAULT_TO_LOCATION:  // USES OPTIONAL PARAMETERS
			if (gGameExternalOptions.useNewAI)
			{
				// Send troops to assault the specified location.

				// **  1st optional parm is a "direness" rating from the queens point of view 
				//  that ranges from 0 to 100.  It it used to determine the number of troops and
				//  their composition
				// ** If 2nd optional parm is specified that is used as the groups "intention"
				unsigned direness = (unsigned) option1;
				Assert(direness >= 0 && direness <= 100);
				unsigned intention = (unsigned) option2;
				Assert(intention < NUM_ENEMY_INTENTIONS);
				
				ubNumSoldiers = ( gubMinEnemyGroupSize + 
								  difficultyMod * 3 + 
								  Random(HighestPlayerProgressPercentage() / 5) +
								  Random(option1 / 5)
								 );

				if ( !gfUnlimitedTroops )
					giReinforcementPool -= ubNumSoldiers;

				giReinforcementPool = max( giReinforcementPool, 0 );

				ubSourceSectorID = SEC_H13;
				ubTargetSectorID = (UINT8)SECTOR( sSectorX, sSectorY );

				if ( !(SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
				{
					ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );
				}

				int groupCnt = 0;
				while (ubNumSoldiers != 0) {
					++groupCnt;
					unsigned soldiersThisSquad = ubNumSoldiers;
					if (soldiersThisSquad > (unsigned)gGameExternalOptions.iMaxEnemyGroupSize) {
						soldiersThisSquad = (unsigned)gGameExternalOptions.iMaxEnemyGroupSize;
					}
					ubNumSoldiers -= soldiersThisSquad;
					UINT8 adminsThisSquad = 0;
					UINT8 troopsThisSquad = 0;
					UINT8 elitesThisSquad = 0;
					UINT8 robotsThisSquad = 0;
					UINT8 tanksThisSquad = 0;
					UINT8 jeepsThisSquad = 0;
					// this is an attack group, but set it to patrol since we'll set the admin/regular/elite composition by hand here
					InitializeGroup(GROUP_TYPE_PATROL, soldiersThisSquad, troopsThisSquad, elitesThisSquad, robotsThisSquad, jeepsThisSquad, tanksThisSquad, Random(10 * 100) < difficultyMod * direness);
					if (direness < 25) {
						adminsThisSquad = soldiersThisSquad * (40 + Random(20)) / 100;	// 40-60% of total
						troopsThisSquad = soldiersThisSquad - adminsThisSquad;			// The rest
					} else if (direness < 50) {
						adminsThisSquad = soldiersThisSquad * (20 + Random(10)) / 100;	// 20-30% of total
						troopsThisSquad = soldiersThisSquad * (30 + Random(20)) / 100;	// 30-50% of total
						elitesThisSquad = soldiersThisSquad - troopsThisSquad - adminsThisSquad; // The rest
					} else if (direness < 75) {
						adminsThisSquad = soldiersThisSquad * (10 + Random(10)) / 100;	// 10-20% of total
						troopsThisSquad = soldiersThisSquad * (20 + Random(20)) / 100;	// 20-40% of total
						elitesThisSquad = soldiersThisSquad - troopsThisSquad - adminsThisSquad; // The rest
					} else /* direness >=75  */ {
						adminsThisSquad = 0;											// 0% of total
						troopsThisSquad = soldiersThisSquad * (30 + Random(20)) / 100;	// 30-50% of total
						elitesThisSquad = soldiersThisSquad - troopsThisSquad - adminsThisSquad; // The rest
					}
					Assert(adminsThisSquad + troopsThisSquad + elitesThisSquad == soldiersThisSquad);
					pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, adminsThisSquad, soldiersThisSquad, elitesThisSquad, robotsThisSquad, tanksThisSquad, jeepsThisSquad );
					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID ) {
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						if (groupCnt > 2) {
							pGroup0->pEnemyGroup->ubIntention = REINFORCEMENTS;
							gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup0->ubGroupID;
						}
					} else {
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
					}
					MoveSAIGroupToSector( &pGroup0, ubTargetSectorID, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
				} // while
			} // if (gGameExternalOptions.useNewAI)
			break;

		// WDS - New AI
		case NPC_ACTION_SEND_SPECIFIC_ASSAULT_TO_LOCATION:  // USES OPTIONAL PARAMETERS
			if (gGameExternalOptions.useNewAI) {
				// Send specific troops to assauat the specified location.

				// Note:  1st optional parm is a count of troops to send
				//        2nd optional parm is the composition of troops
				if (option1 > 0 && option1 < 256) {
					ubNumSoldiers = (UINT8)(option1);
				} else {
					// Something is goofy, just send a default sized squad
					ubNumSoldiers = gubMinEnemyGroupSize;
				}
				unsigned adminsPercentage = 0;
				unsigned troopsPercentage = 0;
				unsigned elitesPercentage = 0;
				// option2: value is eettaa
				if (option2 > 0 && option2 < 99+99*100+99*100*100) {
					adminsPercentage = option2 % 100;
					troopsPercentage = (option2 / 100) % 100;
					elitesPercentage = (option2 / 10000) % 100;
				} else {
					// Something is goofy, just use a default composition
					adminsPercentage = 25;
					troopsPercentage = 50;
					elitesPercentage = 25;
				}
				Assert(adminsPercentage + troopsPercentage + elitesPercentage == 100);

				if ( !gfUnlimitedTroops )
					giReinforcementPool -= ubNumSoldiers;
				giReinforcementPool = max( giReinforcementPool, 0 );

				ubSourceSectorID = SEC_H13;
				ubTargetSectorID = (UINT8)SECTOR( sSectorX, sSectorY );

				if ( !(SectorInfo[ ubSourceSectorID ].ubNumTroops > 0) )
				{
					ubSourceSectorID = SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY );
				}

				int groupCnt = 0;
				while (ubNumSoldiers != 0) {
					++groupCnt;
					unsigned soldiersThisSquad = ubNumSoldiers;
					if (soldiersThisSquad > (unsigned)gGameExternalOptions.iMaxEnemyGroupSize) {
						soldiersThisSquad = (unsigned)gGameExternalOptions.iMaxEnemyGroupSize;
					}
					ubNumSoldiers -= soldiersThisSquad;
					unsigned adminsThisSquad = soldiersThisSquad * adminsPercentage / 100;
					unsigned troopsThisSquad = soldiersThisSquad * troopsPercentage / 100;
					unsigned elitesThisSquad = soldiersThisSquad - troopsThisSquad - adminsThisSquad; // The rest

					Assert(adminsThisSquad + troopsThisSquad + elitesThisSquad == soldiersThisSquad);
					pGroup0 = CreateNewEnemyGroupDepartingFromSector( ubSourceSectorID, adminsThisSquad, soldiersThisSquad, elitesThisSquad, 0, 0, 0 );
					if( !gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID ) {
						pGroup0->pEnemyGroup->ubIntention = STAGE;
						if (groupCnt > 2) {
							pGroup0->pEnemyGroup->ubIntention = REINFORCEMENTS;
							gGarrisonGroup[ SectorInfo[ ubTargetSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup0->ubGroupID;
						}
					} else {
						pGroup0->pEnemyGroup->ubIntention = PURSUIT;
					}
					MoveSAIGroupToSector( &pGroup0, ubTargetSectorID, EVASIVE, pGroup0->pEnemyGroup->ubIntention );
				} // while
			} // if (gGameExternalOptions.useNewAI)
			break;

		case NPC_ACTION_SEND_SOLDIERS_TO_OMERTA:
			ubNumSoldiers = (UINT8)( gubMinEnemyGroupSize + difficultyMod * 6); //6, 12, or 18 based on difficulty.	
			pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, ubNumSoldiers, (UINT8)(ubNumSoldiers/7), 0, 0, 0 ); //add 1 elite to normal, and 2 for hard
			ubNumSoldiers = (UINT8)(ubNumSoldiers + ubNumSoldiers / 7);

			//Madd: unlimited reinforcements
			if ( !gfUnlimitedTroops )
				giReinforcementPool -= ubNumSoldiers;

			giReinforcementPool = max( giReinforcementPool, 0 );
			if( PlayerMercsInSector( 9, 1, 1 ) && !PlayerMercsInSector( 10, 1, 1 ) && !PlayerMercsInSector( 10, 1, 2 ) )
			{ //send to A9 (if mercs in A9, but not in A10 or A10 basement)
				ubSectorID = SEC_A9;
			}
			else
			{ //send to A10
				ubSectorID = SEC_A10;
			}

			MoveSAIGroupToSector( &pGroup, ubSectorID, EVASIVE, PURSUIT );

			ValidateGroup( pGroup );
			break;

		case NPC_ACTION_SEND_TROOPS_TO_SAM:
			ubSectorID = (UINT8)SECTOR( sSectorX, sSectorY );
			ubNumSoldiers = (UINT8)( gubMinEnemyGroupSize + difficultyMod + HighestPlayerProgressPercentage() / 15 );
			//Madd: unlimited reinforcements?
			if ( !gfUnlimitedTroops )
				giReinforcementPool -= ubNumSoldiers;

			UINT8 ubNumTroops, ubNumElites;
			// set the type to patrol as we're manually tuning the group composition here (100% elites + ASD upgrades)
			InitializeGroup(GROUP_TYPE_PATROL, ubNumSoldiers, ubNumTroops, ubNumElites, ubNumRobots, ubNumJeeps, ubNumTanks, Random(10*100) < difficultyMod * HighestPlayerProgressPercentage());

			giReinforcementPool = max( giReinforcementPool, 0 );
			pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, 0, ubNumSoldiers, ubNumRobots, ubNumTanks, ubNumJeeps );
			MoveSAIGroupToSector( &pGroup, ubSectorID, STAGE, REINFORCEMENTS );

			// WANNE: This should fix the assertion in UC in the cutscene!
			if (SectorInfo[ ubSectorID ].ubGarrisonID != 255)
			{
				if( gGarrisonGroup[ SectorInfo[ ubSectorID ].ubGarrisonID ].ubPendingGroupID )
				{	//Clear the pending group's assignment.
					pPendingGroup = GetGroup( gGarrisonGroup[ SectorInfo[ ubSectorID ].ubGarrisonID ].ubPendingGroupID );
					Assert( pPendingGroup );
					ClearPreviousAIGroupAssignment( pPendingGroup );
				}

				//Assign the elite squad to attack the SAM site
				if ( pGroup )
				{
					pGroup->pEnemyGroup->ubIntention = REINFORCEMENTS;
					gGarrisonGroup[ SectorInfo[ ubSectorID ].ubGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
				}

				if( pPendingGroup )
				{ //Reassign the pending group
					ReassignAIGroup( &pPendingGroup );
				}
			}
			break;

		case NPC_ACTION_ADD_MORE_ELITES:
			gfExtraElites = TRUE;
			EvolveQueenPriorityPhase( TRUE );
			break;

		case NPC_ACTION_GIVE_KNOWLEDGE_OF_ALL_MERCS:
			//temporarily make the queen's forces more aware (high alert)
			gubNumAwareBattles = zDiffSetting[gGameOptions.ubDifficultyLevel].iNumAwareBattles;
			break;

		default:
			ScreenMsg( FONT_RED, MSG_DEBUG, L"QueenAI failed to handle action code %d.", usActionCode );
			break;
	}
}


// WDS - New AI
void HourlyCheckStrategicAI()
{
	// Nothing (yet!)
}


#ifdef JA2BETAVERSION
#include "sgp_logger.h"

static struct StratDecLog {
	sgp::Logger_ID id;
	StratDecLog() {
		id = sgp::Logger::instance().createLogger();
		sgp::Logger::instance().connectFile(id, L"Strategic Decisions.txt", false, sgp::Logger::FLUSH_ON_DELETE);
	}
} s_stratD;

void LogStrategicMsg( STR8	str, ... )
{
	va_list argptr;
	CHAR8	string[512];
#ifndef USE_VFS
	FILE *fp;

	fp = fopen( "Strategic Decisions.txt", "a" );
	if( !fp )
		return;
#endif
	va_start(argptr, str );
	vsprintf( string, str, argptr);
	va_end(argptr);

#ifndef USE_VFS
	fprintf( fp, "%s\n", string );
#else
	SGP_LOG(s_stratD.id, string);
#endif

	if( gfDisplayStrategicAILogs )
	{
		ScreenMsg( FONT_LTKHAKI, MSG_DIALOG, L"%S", string );
	}
	if( guiCurrentScreen == AIVIEWER_SCREEN )
	{
		OutputDebugString( (LPCSTR)String( "%s\n", string ) );
	}

#ifndef USE_VFS
	fclose( fp );
#endif
}

void LogStrategicEvent( STR8	str, ... )
{
	va_list argptr;
	CHAR8	string[512];
#ifndef USE_VFS
	FILE *fp;

	fp = fopen( "Strategic Decisions.txt", "a" );
	if( !fp )
		return;
#endif
	va_start(argptr, str );
	vsprintf( string, str, argptr);
	va_end(argptr);

#ifndef USE_VFS
	fprintf( fp, "\n%S:\n", WORLDTIMESTR );
	fprintf( fp, "%s\n", string );
#else
	SGP_LOG(s_stratD.id) << sgp::endl << WORLDTIMESTR << ":" << sgp::endl << string << sgp::endl;
#endif
	if( gfDisplayStrategicAILogs )
	{
		ScreenMsg( FONT_LTKHAKI, MSG_DIALOG, L"%S", string );
	}
	if( guiCurrentScreen == AIVIEWER_SCREEN )
	{
		OutputDebugString( (LPCSTR)String( "%s\n", string ) );
	}
#ifndef USE_VFS
	fclose( fp );
#endif
}

void ClearStrategicLog()
{
#ifndef USE_VFS
	FILE *fp;
	fp = fopen( "Strategic Decisions.txt", "w" );
	if( !fp )
		return;

	fprintf( fp, "STRATEGIC LOG\n" );

	fclose( fp );
#else
	SGP_LOG(s_stratD.id) << sgp::flush;
#endif
}
#endif

void InvestigateSector( UINT8 ubSectorID )
{
	//Lalien: enabled
	if ( gGameExternalOptions.gfInvestigateSector == TRUE )
	{
		INT32 i;
		SECTORINFO *pSector;
		INT16 sSectorX, sSectorY;
		UINT8 ubAdmins[4], ubTroops[4], ubElites[4], ubNumToSend, ubTotal;

		//Lalien: enabled again
		//@@@ Disabled!	Also commented out the posting of the event
		//return;

		sSectorX = (INT16)SECTORX( ubSectorID );
		sSectorY = (INT16)SECTORY( ubSectorID );

		if( guiCurrentScreen != GAME_SCREEN )
		{ //If we aren't in tactical, then don't do this.	It is strictly added flavour and would be irritating if
			//you got the prebattle interface in mapscreen while compressing time (right after clearing it...)
			return;
		}

		if( sSectorX != gWorldSectorX || sSectorY != gWorldSectorY || gbWorldSectorZ )
		{ //The sector isn't loaded, so don't bother...
			return;
		}

		//Now, we will investigate this sector if there are forces in adjacent towns.	For each
		//sector that applies, we will add 1-2 soldiers.

		ubTotal = 0;
		for( i = 0; i < 4; i++ )
		{
			ubAdmins[i] = ubTroops[i] = ubElites[i] = 0;
			switch( i )
			{
			case 0: //NORTH
				if( sSectorY == 1 )
					continue;
				pSector = &SectorInfo[ ubSectorID - 16 ];
				break;
			case 1: //EAST
				if( sSectorX == 16 )
					continue;
				pSector = &SectorInfo[ ubSectorID + 1 ];
				break;
			case 2: //SOUTH
				if( sSectorY == 16 )
					continue;
				pSector = &SectorInfo[ ubSectorID + 16 ];
				break;
			case 3: //WEST
				if( sSectorX == 1 )
					continue;
				pSector = &SectorInfo[ ubSectorID - 1 ];
				break;
			}

			if( pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites > 4 )
			{
				ubNumToSend = (UINT8)(Random( 2 ) + 1);
				while( ubNumToSend )
				{
					if( pSector->ubNumAdmins )
					{
						pSector->ubNumAdmins--;
						ubNumToSend--;
						ubAdmins[i]++;
						ubTotal++;

						// Flugente: turncoats might move as well
						if ( pSector->ubNumAdmins_Turncoat > pSector->ubNumAdmins )
						{
							SectorInfo[ubSectorID].ubNumAdmins_Turncoat += pSector->ubNumAdmins_Turncoat - pSector->ubNumAdmins;
							pSector->ubNumAdmins_Turncoat = pSector->ubNumAdmins;
						}
					}
					else if( pSector->ubNumTroops )
					{
						pSector->ubNumTroops--;
						ubNumToSend--;
						ubTroops[i]++;
						ubTotal++;

						// Flugente: turncoats might move as well
						if ( pSector->ubNumTroops_Turncoat > pSector->ubNumTroops )
						{
							SectorInfo[ubSectorID].ubNumTroops_Turncoat += pSector->ubNumTroops_Turncoat - pSector->ubNumTroops;
							pSector->ubNumTroops_Turncoat = pSector->ubNumTroops;
						}
					}
					else if( pSector->ubNumElites )
					{
						pSector->ubNumElites--;
						ubNumToSend--;
						ubElites[i]++;
						ubTotal++;

						// Flugente: turncoats might move as well
						if ( pSector->ubNumElites_Turncoat > pSector->ubNumElites )
						{
							SectorInfo[ubSectorID].ubNumElites_Turncoat += pSector->ubNumElites_Turncoat - pSector->ubNumElites;
							pSector->ubNumElites_Turncoat = pSector->ubNumElites;
						}
					}
					else
					{
						break; //???
					}
				}
			}
		}

		if( !ubTotal )
		{ //Nobody is available to investigate
			return;
		}

		//Now we have decided who to send, so send them.
		for( i = 0; i < 4; i++ )
		{
			if( ubAdmins[i] + ubTroops[i] + ubElites[i] )
			{
				switch( i )
				{
				case 0: //NORTH
					AddEnemiesToBattle( NULL, INSERTION_CODE_NORTH, ubAdmins[i], ubTroops[i], ubElites[i], 0, 0, 0, TRUE );
					break;
				case 1: //EAST
					AddEnemiesToBattle( NULL, INSERTION_CODE_EAST, ubAdmins[i], ubTroops[i], ubElites[i], 0, 0, 0, TRUE );
					break;
				case 2: //SOUTH
					AddEnemiesToBattle( NULL, INSERTION_CODE_SOUTH, ubAdmins[i], ubTroops[i], ubElites[i], 0, 0, 0, TRUE );
					break;
				case 3: //WEST
					AddEnemiesToBattle( NULL, INSERTION_CODE_WEST, ubAdmins[i], ubTroops[i], ubElites[i], 0, 0, 0, TRUE );
					break;
				}
			}
		}

		if( !gfQueenAIAwake )
		{
			gfFirstBattleMeanwhileScenePending = TRUE;
		}
	}
}

void StrategicHandleQueenLosingControlOfSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	SECTORINFO *pSector;
	UINT8 ubSectorID;
	if( sSectorZ )
	{ //The queen doesn't care about anything happening under the ground.
		return;
	}

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( StrategicMap[CALCULATE_STRATEGIC_INDEX(sSectorX, sSectorY)].fEnemyControlled )
	{
		//If the sector doesn't belong to the player, then we shouldn't be calling this function!
		SAIReportError( L"StrategicHandleQueenLosingControlOfSector() was called for a sector that is internally considered to be enemy controlled." );
		return;
	}

	ubSectorID = SECTOR( sSectorX, sSectorY );
	pSector = &SectorInfo[ ubSectorID ];

	//Keep track of victories and wake up the queen after x number of battles.
	gusPlayerBattleVictories++;
	
		UINT8 value8;
		if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
			value8 = 1;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
			value8 = 2;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
			value8 = 3;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
			value8 = 4;
		else
		{
			value8 = Random (4);
			if (value8 == 0) value8 = 1;
		}
	if( gusPlayerBattleVictories == 5 - value8 )
	//if( gusPlayerBattleVictories == 5 - gGameOptions.ubDifficultyLevel )
	{ //4 victories for easy, 3 for normal, 2 for hard
		WakeUpQueen();
	}

	if( pSector->ubGarrisonID == NO_GARRISON )
	{ //Queen doesn't care if the sector lost wasn't a garrison sector.
		return;
	}
	else
	{
		//Lalien
		if ( gGameExternalOptions.gfReassignPendingReinforcements == TRUE )
		{
			//check to see if there are any pending reinforcements.	If so, then cancel their orders and have them
			//reassigned, so the player doesn't get pestered.	This is a feature that *dumbs* down the AI, and is done
			//for the sake of gameplay.	We don't want the game to be tedious.
			if( !pSector->uiTimeLastPlayerLiberated )
			{
				pSector->uiTimeLastPlayerLiberated = GetWorldTotalSeconds();
			}
			else
			{
				//convert hours to seconds and subtract up to half of it randomly "seconds - (hours*3600 / 2)"
				pSector->uiTimeLastPlayerLiberated = GetWorldTotalSeconds() - Random( gubHoursGracePeriod * 1800 );
			}

			if( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
			{
				GROUP *pGroup;
				pGroup = GetGroup( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID );

				if( pGroup )
				{
					ReassignAIGroup( &pGroup );
				}
				gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID = 0;
			}
		}
	}

	//If there are any enemy groups that will be moving through this sector due, they will have to repath which
	//will cause them to avoid the sector.	Returns the number of redirected groups.
	RedirectEnemyGroupsMovingThroughSector( (UINT8)sSectorX, (UINT8)sSectorY );

	//For the purposes of a town being lost, we shall check to see if the queen wishes to investigate quickly after losing.
	//This is done in town sectors when the character first takes it.
	//Only town size greater than 1 will be investigated from other sectors of the same town
	UINT8 ubTownId = GetTownIdForSector( sSectorX, sSectorY );
	
	if( ubTownId != BLANK_SECTOR && GetTownSectorSize( ubTownId ) != 1 )
	{
		for( INT32 i = 0; i < MAP_WORLD_X * MAP_WORLD_Y; i++ )
		{
			if( StrategicMap[ i ].bNameId == ubTownId )
				SectorInfo[ STRATEGIC_INDEX_TO_SECTOR_INFO( i ) ].ubInvestigativeState++;
		}	
	}
	else
		return;

	//Madd: tweaked to increase attacks for experienced and expert to 8 and 12, and unlimited for insane
		UINT8 value9;
		if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
			value9 = 1;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
			value9 = 2;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
			value9 = 3;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
			value9 = 4;
		else
		{
			value9 = Random (4);
			if (value9 == 0) value9 = 1;
		}
	
	if( pSector->ubInvestigativeState >= ( 4 * value9 ) && !zDiffSetting[gGameOptions.ubDifficultyLevel].bQueenLosingControlOfSector )
	{
		//This is the 4th time the player has conquered this sector.	We won't pester the player with probing attacks here anymore.
		return;
	}

	if( sSectorX != gWorldSectorX || sSectorY != gWorldSectorY )
	{ //The sector isn't loaded, so don't probe attack it.	Otherwise, autoresolve would get them smoked!
		return;
	}

	//@@@ disabled
	//Lalien: enabled investigation of lost sector by enemy
	if ( gGameExternalOptions.gfInvestigateSector == TRUE)
	{
		AddStrategicEventUsingSeconds( EVENT_INVESTIGATE_SECTOR, GetWorldTotalSeconds() + 45 * pSector->ubInvestigativeState + Random( 60 ), SECTOR( sSectorX, sSectorY ) );
	}
}


void RequestHighPriorityStagingGroupReinforcements( GROUP *pGroup )
{
//	GROUP *pClosestGroup;
	if( !pGroup->pEnemyGroup->ubPendingReinforcements )
	{
		return;
	}
	//pClosestGroup = SearchForClosestGroup( pGroup );
}


UINT8 SectorDistance( UINT8 ubSectorID1, UINT8 ubSectorID2 )
{
	UINT8 ubSectorX1, ubSectorX2, ubSectorY1, ubSectorY2;
	UINT8 ubDist;
	ubSectorX1 = (UINT8)SECTORX( ubSectorID1 );
	ubSectorX2 = (UINT8)SECTORX( ubSectorID2 );
	ubSectorY1 = (UINT8)SECTORY( ubSectorID1 );
	ubSectorY2 = (UINT8)SECTORY( ubSectorID2 );

	ubDist = (UINT8)( abs( ubSectorX1 - ubSectorX2 ) + abs( ubSectorY1 - ubSectorY2 ) );

	return ubDist;
}

void RequestHighPriorityGarrisonReinforcements( INT32 iGarrisonID, UINT8 ubSoldiersRequested )
{
	INT32 i, iBestIndex;
	GROUP *pGroup;
	UINT8 ubBestDist, ubDist;
	UINT8 ubDstSectorX, ubDstSectorY;
	//AssertMsg( giPatrolArraySize == PATROL_GROUPS && giGarrisonArraySize == GARRISON_GROUPS, "Strategic AI -- Patrol and/or garrison group definition mismatch." );
	ubBestDist = 255;
	iBestIndex = -1;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( i = 0; i < giPatrolArraySize; i++ )
	{
		if( gPatrolGroup[ i ].ubGroupID )
		{
			pGroup = GetGroup( gPatrolGroup[ i ].ubGroupID );
			if( pGroup && pGroup->ubGroupSize >= ubSoldiersRequested )
			{
				ubDist = SectorDistance( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), gGarrisonGroup[ iGarrisonID ].ubSectorID );
				if( ubDist < ubBestDist )
				{
					ubBestDist = ubDist;
					iBestIndex = i;
				}
			}
		}
	}
	ubDstSectorX = (UINT8)SECTORX( gGarrisonGroup[ iGarrisonID ].ubSectorID );
	ubDstSectorY = (UINT8)SECTORY( gGarrisonGroup[ iGarrisonID ].ubSectorID );
	
	/* // Comment code calling reinforcements from patrol groups as they will not be replenished subsequently
	if( iBestIndex != -1 )
	{ //Send the group to the garrison
		pGroup = GetGroup( gPatrolGroup[ iBestIndex ].ubGroupID );
		if( pGroup->ubGroupSize > ubSoldiersRequested && pGroup->ubGroupSize - ubSoldiersRequested >= gubMinEnemyGroupSize )
		{ //Split the group, and send to location
			GROUP *pNewGroup;
			pNewGroup = CreateNewEnemyGroupDepartingFromSector( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), 0, 0, 0, 0, 0 );
			//Transfer the troops from group to new group
			if( pGroup->pEnemyGroup->ubNumTroops >= ubSoldiersRequested )
			{ //All of them are troops, so do it in one shot.
				pGroup->pEnemyGroup->ubNumTroops -= ubSoldiersRequested;
				pGroup->ubGroupSize -= ubSoldiersRequested;
				pNewGroup->pEnemyGroup->ubNumTroops = ubSoldiersRequested;
				pNewGroup->ubGroupSize += ubSoldiersRequested;
				ValidateLargeGroup( pGroup );
				ValidateLargeGroup( pNewGroup );
			}
			else while( ubSoldiersRequested )
			{ //There aren't enough troops, so transfer other types when we run out of troops, prioritizing admins, then elites.
				if( pGroup->pEnemyGroup->ubNumTroops )
				{
					pGroup->pEnemyGroup->ubNumTroops--;
					pGroup->ubGroupSize--;
					pNewGroup->pEnemyGroup->ubNumTroops++;
					pNewGroup->ubGroupSize++;
					ubSoldiersRequested--;
					ValidateLargeGroup( pGroup );
					ValidateLargeGroup( pNewGroup );
				}
				else if( pGroup->pEnemyGroup->ubNumAdmins )
				{
					pGroup->pEnemyGroup->ubNumAdmins--;
					pGroup->ubGroupSize--;
					pNewGroup->pEnemyGroup->ubNumAdmins++;
					pNewGroup->ubGroupSize++;
					ubSoldiersRequested--;
					ValidateLargeGroup( pGroup );
					ValidateLargeGroup( pNewGroup );
				}
				else if( pGroup->pEnemyGroup->ubNumElites )
				{
					pGroup->pEnemyGroup->ubNumElites--;
					pGroup->ubGroupSize--;
					pNewGroup->pEnemyGroup->ubNumElites++;
					pNewGroup->ubGroupSize++;
					ubSoldiersRequested--;
					ValidateLargeGroup( pGroup );
					ValidateLargeGroup( pNewGroup );
				}
				else if( pGroup->pEnemyGroup->ubNumTanks )
				{
					pGroup->pEnemyGroup->ubNumTanks--;
					pGroup->ubGroupSize--;
					pNewGroup->pEnemyGroup->ubNumTanks++;
					pNewGroup->ubGroupSize++;
					ubSoldiersRequested--;
					ValidateLargeGroup( pGroup );
					ValidateLargeGroup( pNewGroup );
				}
				else
				{
					AssertMsg( 0, "Strategic AI group transfer error.	KM : 0" );
					return;
				}
			}
			pNewGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
			gGarrisonGroup[ iGarrisonID ].ubPendingGroupID = pNewGroup->ubGroupID;
			RecalculatePatrolWeight( iBestIndex );

			MoveSAIGroupToSector( &pNewGroup, gGarrisonGroup[ iGarrisonID ].ubSectorID, EVASIVE, REINFORCEMENTS );
		}
		else
		{ //Send the whole group and kill it's patrol assignment.
			gPatrolGroup[ iBestIndex ].ubGroupID = 0;
			gGarrisonGroup[ iGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
			pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
			RecalculatePatrolWeight( iBestIndex );
			//The ONLY case where the group is told to move somewhere else when they could be BETWEEN sectors.	The movegroup functions
			//don't work if this is the case.	Teleporting them to their previous sector is the best and easiest way to deal with this.
			SetEnemyGroupSector( pGroup, (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) );

			MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iGarrisonID ].ubSectorID, EVASIVE, REINFORCEMENTS );
			ValidateGroup( pGroup );
		}
	}
	else
	*/
	{ //There are no groups that have enough troops. Send a new force from the palace instead.
		if ( giReinforcementPool > 0 )
		{
			pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 
				0, min( ubSoldiersRequested, giReinforcementPool ), 0, 0, 0, 0 );
			pGroup->ubMoveType = ONE_WAY;
			pGroup->pEnemyGroup->ubIntention = REINFORCEMENTS;
			gGarrisonGroup[ iGarrisonID ].ubPendingGroupID = pGroup->ubGroupID;
			pGroup->ubOriginalSector = (UINT8)SECTOR( ubDstSectorX, ubDstSectorY );
			InitializeGroup(GROUP_TYPE_PATROL, pGroup->pEnemyGroup->ubNumTroops, *pGroup->pEnemyGroup, Random(10) < gGameOptions.ubDifficultyLevel );

			//Madd: unlimited reinforcements?
			if ( !gfUnlimitedTroops )
				giReinforcementPool -= (INT32)min( ubSoldiersRequested, giReinforcementPool );

			MoveSAIGroupToSector( &pGroup, gGarrisonGroup[ iGarrisonID ].ubSectorID, EVASIVE, REINFORCEMENTS );
			ValidateGroup( pGroup );
		}
	}
}

void WakeUpQueen()
{
	gfQueenAIAwake = TRUE;
	if( !gfMassFortificationOrdered )
	{
		gfMassFortificationOrdered = TRUE;
		MassFortifyTowns();
	}
}

void MassFortifyTowns()
{
	INT32 i;
	SECTORINFO *pSector;
	//GROUP *pGroup;
	UINT8 ubNumTroops, ubDesiredTroops;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		pSector = &SectorInfo[ gGarrisonGroup[ i ].ubSectorID ];
		ubNumTroops = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
		ubDesiredTroops = (UINT8)gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bDesiredPopulation;

		if( ubNumTroops < ubDesiredTroops	)
		{
			if( !gGarrisonGroup[ i ].ubPendingGroupID &&
				gGarrisonGroup[ i ].ubComposition != ROADBLOCK &&
				EnemyPermittedToAttackSector( NULL, gGarrisonGroup[ i ].ubSectorID ) )
			{
				RequestHighPriorityGarrisonReinforcements( i, (UINT8)(ubDesiredTroops - ubNumTroops) );
			}
		}
	}

	//Lalien: removed, the queen should attack and stay in Omerta

	//Convert the garrison sitting in Omerta (if alive), and reassign them
	//pSector = &SectorInfo[ SEC_A9 ];
	//if( pSector->ubNumTroops )
	//{
	//	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_A9, 0, pSector->ubNumTroops, 0 );
	//	Assert( pGroup );
	//	pSector->ubNumTroops = 0;
	//	pGroup->pEnemyGroup->ubIntention = PATROL;
	//	pGroup->ubMoveType = ONE_WAY;
	//	ReassignAIGroup( &pGroup );
	//	ValidateGroup( pGroup );
	//	RecalculateSectorWeight( SEC_A9 );
	//}
}

void RenderAIViewerGarrisonInfo( INT32 x, INT32 y, SECTORINFO *pSector )
{
	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( pSector->ubGarrisonID != NO_GARRISON )
	{
		INT32 iDesired, iSurplus;
		iDesired = gArmyComp[ gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition ].bDesiredPopulation;
		iSurplus = pSector->ubNumTroops + pSector->ubNumAdmins + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps - iDesired;
		SetFontForeground( FONT_WHITE );
		if( iSurplus >= 0 )
		{
			mprintf( x, y, L"%d desired, %d surplus troops", iDesired, iSurplus );
		}
		else
		{
			mprintf( x, y, L"%d desired, %d reinforcements requested", iDesired, -iSurplus );
		}
		if( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
		{
			GROUP *pGroup;
			pGroup = GetGroup( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID );
			mprintf( x, y+10, L"%d reinforcements on route from group %d in %c%d", pGroup->ubGroupSize, pGroup->ubGroupID,
				pGroup->ubSectorY + 'A' - 1, pGroup->ubSectorX );
		}
		else
		{
			mprintf( x, y+10, L"No pending reinforcements for this sector." );
		}
	}
	else
	{
		SetFontForeground( FONT_GRAY2 );
		mprintf( x, y, L"No garrison information for this sector." );
	}
}

void StrategicHandleMineThatRanOut( UINT8 ubSectorID )
{
	// Queen will then have little interest in the town
	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	UINT8 ubTownId = GetTownAssociatedWithMine( GetMineIndexForSector( SECTORX( ubSectorID ), SECTORY( ubSectorID ) ) );

	if( ubTownId != BLANK_SECTOR )
	{
		for( INT32 i = 0; i < MAP_WORLD_X * MAP_WORLD_Y; i++ )
		{
			if( StrategicMap[ i ].bNameId == ubTownId )
				gArmyComp[ gGarrisonGroup[ SectorInfo[ STRATEGIC_INDEX_TO_SECTOR_INFO( i ) ].ubGarrisonID ].ubComposition ].bPriority /= 4;
		}	
	}
}

BOOLEAN GarrisonCanProvideMinimumReinforcements( INT32 iGarrisonID )
{
	INT32 iAvailable;
	INT32 iDesired;
	SECTORINFO *pSector;
	UINT8 ubSectorX, ubSectorY;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];

	iAvailable = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	iDesired = gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bDesiredPopulation;

	if( iAvailable - iDesired >= gubMinEnemyGroupSize )
	{
		//Do a more expensive check first to determine if there is a player presence here (combat in progress)
		//If so, do not provide reinforcements from here.
		ubSectorX = (UINT8)SECTORX( gGarrisonGroup[ iGarrisonID ].ubSectorID );
		ubSectorY = (UINT8)SECTORY( gGarrisonGroup[ iGarrisonID ].ubSectorID );
		if ( PlayerMercsInSector( ubSectorX, ubSectorY, 0 ) || NumNonPlayerTeamMembersInSector( ubSectorX, ubSectorY, MILITIA_TEAM ) )
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOLEAN GarrisonRequestingMinimumReinforcements( INT32 iGarrisonID )
{
	INT32 iAvailable;
	INT32 iDesired;
	SECTORINFO *pSector;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( gGarrisonGroup[ iGarrisonID ].ubPendingGroupID )
	{
		return FALSE;
	}

	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	iAvailable = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	iDesired = gArmyComp[ gGarrisonGroup[ iGarrisonID ].ubComposition ].bDesiredPopulation;

	if( iDesired - iAvailable >= gubMinEnemyGroupSize )
	{
		return TRUE;
	}

	// Flugente: The above check is insufficient. if we increase gubMinEnemyGroupSize (to, say, make the game harder), this check will never evaluate to true. Congratulations, you've broken the AI!
	// Instead, if we have nobody here, but want someone to be here, then a minimum-sized group is in order.
	if( iDesired > 0 && iAvailable == 0 )
		return TRUE;

	return FALSE;
}

BOOLEAN PatrolRequestingMinimumReinforcements( INT32 iPatrolID )
{
	GROUP *pGroup;

	if( gPatrolGroup[ iPatrolID ].ubPendingGroupID )
	{
		return FALSE;
	}
	if( !PermittedToFillPatrolGroup( iPatrolID ) )
	{ //if the group was defeated, it won't be considered for reinforcements again for several days
		return FALSE;
	}
	pGroup = GetGroup( gPatrolGroup[ iPatrolID ].ubGroupID );
	if( pGroup )
	{
		if( gPatrolGroup[ iPatrolID ].bSize - pGroup->ubGroupSize >= gubMinEnemyGroupSize )
		{
			return TRUE;
		}
	}
	else
	{
		// anv: we want to refill totally defeated patrols too
		return TRUE;
	}
	return FALSE;
}


void EliminateSurplusTroopsForGarrison( GROUP *pGroup, SECTORINFO *pSector )
{
	INT32 iTotal;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Strategic8");

	iTotal = pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites + pGroup->pEnemyGroup->ubNumAdmins + pGroup->pEnemyGroup->ubNumRobots + pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps +
		pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumAdmins + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps;
	if( iTotal <= iMaxEnemyGroupSize )
	{
		return;
	}
	iTotal -= iMaxEnemyGroupSize;
	while( iTotal )
	{
		if( pGroup->pEnemyGroup->ubNumAdmins )
		{
			if( pGroup->pEnemyGroup->ubNumAdmins < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumAdmins;
				pGroup->pEnemyGroup->ubNumAdmins = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumAdmins -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pSector->ubNumAdmins )
		{
			if( pSector->ubNumAdmins < iTotal )
			{
				iTotal -= pSector->ubNumAdmins;
				pSector->ubNumAdmins = 0;
			}
			else
			{
				pSector->ubNumAdmins -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pGroup->pEnemyGroup->ubNumTroops )
		{
			if( pGroup->pEnemyGroup->ubNumTroops < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumTroops;
				pGroup->pEnemyGroup->ubNumTroops = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumTroops -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pSector->ubNumTroops )
		{
			if( pSector->ubNumTroops < iTotal )
			{
				iTotal -= pSector->ubNumTroops;
				pSector->ubNumTroops = 0;
			}
			else
			{
				pSector->ubNumTroops -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pGroup->pEnemyGroup->ubNumElites )
		{
			if( pGroup->pEnemyGroup->ubNumElites < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumElites;
				pGroup->pEnemyGroup->ubNumElites = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumElites -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pSector->ubNumElites )
		{
			if( pSector->ubNumElites < iTotal )
			{
				iTotal -= pSector->ubNumElites;
				pSector->ubNumElites = 0;
			}
			else
			{
				pSector->ubNumElites -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pGroup->pEnemyGroup->ubNumTanks )
		{
			if( pGroup->pEnemyGroup->ubNumTanks < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumTanks;
				pGroup->pEnemyGroup->ubNumTanks = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumTanks -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if( pSector->ubNumTanks )
		{
			if( pSector->ubNumTanks < iTotal )
			{
				iTotal -= pSector->ubNumTanks;
				pSector->ubNumTanks = 0;
			}
			else
			{
				pSector->ubNumTanks -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if ( pGroup->pEnemyGroup->ubNumRobots )
		{
			if ( pGroup->pEnemyGroup->ubNumRobots < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumRobots;
				pGroup->pEnemyGroup->ubNumRobots = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumRobots -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if ( pSector->ubNumRobots )
		{
			if ( pSector->ubNumRobots < iTotal )
			{
				iTotal -= pSector->ubNumRobots;
				pSector->ubNumRobots = 0;
			}
			else
			{
				pSector->ubNumRobots -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if ( pGroup->pEnemyGroup->ubNumJeeps )
		{
			if ( pGroup->pEnemyGroup->ubNumJeeps < iTotal )
			{
				iTotal -= pGroup->pEnemyGroup->ubNumJeeps;
				pGroup->pEnemyGroup->ubNumJeeps = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumJeeps -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
		else if ( pSector->ubNumJeeps )
		{
			if ( pSector->ubNumJeeps < iTotal )
			{
				iTotal -= pSector->ubNumJeeps;
				pSector->ubNumJeeps = 0;
			}
			else
			{
				pSector->ubNumJeeps -= (UINT8)iTotal;
				iTotal = 0;
			}
		}
	}
}



// once Queen is awake, she'll gradually begin replacing admins with regular troops.	This is mainly to keep player from
// fighting many more admins once they are no longer any challenge for him.	Eventually all admins will vanish off map.
void UpgradeAdminsToTroops()
{
	INT32 i;
	SECTORINFO *pSector;
	INT8 bPriority;
	UINT8 ubAdminsToCheck;
	GROUP *pGroup;
	INT16 sPatrolIndex;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	// on normal, AI evaluates approximately every 10 hrs.	There are about 130 administrators seeded on the map.
	// Some of these will be killed by the player.

	UINT8 highestplayerprogress = HighestPlayerProgressPercentage( );

	// check all garrisons for administrators
	if ( zDiffSetting[gGameOptions.ubDifficultyLevel].bUpgradeGarrisonsAdminsToTroops || highestplayerprogress >= zDiffSetting[gGameOptions.ubDifficultyLevel].usAlwaysUpGradeAdminsToTroopsProgress )
	{
		for( i = 0; i < giGarrisonArraySize; ++i )
		{
			// skip sector if it's currently loaded, we'll never upgrade guys in those
			if ( (gWorldSectorX != 0) && (gWorldSectorY != 0) &&
				 (SECTOR( gWorldSectorX, gWorldSectorY ) == gGarrisonGroup[ i ].ubSectorID) )
			{
				continue;
			}

			pSector = &SectorInfo[ gGarrisonGroup[ i ].ubSectorID ];

			// if there are any admins currently in this garrison
			if ( pSector->ubNumAdmins > 0 )
			{
				bPriority = gArmyComp[ gGarrisonGroup[ i ].ubComposition ].bPriority;

				// highest priority sectors are upgraded first. Each 1% of progress lower the
				// priority threshold required to start triggering upgrades by 10%.
				if ( (100 - (10 * highestplayerprogress)) < bPriority )
				{
					ubAdminsToCheck = pSector->ubNumAdmins;

					while ( ubAdminsToCheck > 0)
					{
						// chance to upgrade at each check is random, and also dependant on the garrison's priority
						if ( Chance ( bPriority ) ) 
						{
							pSector->ubNumAdmins--;
							pSector->ubNumTroops++;
						}

						--ubAdminsToCheck;
					}

					// Flugente: make sure turncoat numbers are plausible
					if ( pSector->ubNumAdmins_Turncoat > pSector->ubNumAdmins )
					{
						pSector->ubNumTroops_Turncoat += pSector->ubNumAdmins_Turncoat - pSector->ubNumAdmins;
						pSector->ubNumAdmins_Turncoat = pSector->ubNumAdmins;
					}
				}
			}
		}
	}
	
	// check all moving enemy groups for administrators
	if ( zDiffSetting[gGameOptions.ubDifficultyLevel].bUpgradeAdminsToTroops || highestplayerprogress >= zDiffSetting[gGameOptions.ubDifficultyLevel].usAlwaysUpGradeAdminsToTroopsProgress )
	{
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->ubGroupSize && pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle )
			{
				Assert ( pGroup->pEnemyGroup );

				// skip sector if it's currently loaded, we'll never upgrade guys in those
				if ( ( pGroup->ubSectorX == gWorldSectorX ) && ( pGroup->ubSectorY == gWorldSectorY ) )
				{
					pGroup = pGroup->next;
					continue;
				}

				// if there are any admins currently in this group
				if ( pGroup->pEnemyGroup->ubNumAdmins > 0 )
				{
					// if it's a patrol group
					if ( pGroup->pEnemyGroup->ubIntention == PATROL )
					{
						sPatrolIndex = FindPatrolGroupIndexForGroupID( pGroup->ubGroupID );
						Assert( sPatrolIndex != -1 );

						// use that patrol's priority
						bPriority = gPatrolGroup[ sPatrolIndex ].bPriority;
					}
					else	// not a patrol group
					{
						// use a default priority
						bPriority = 50;
					}

					// highest priority groups are upgraded first. Each 1% of progress lower the
					// priority threshold required to start triggering upgrades by 10%.
					if ( (100 - (10 * highestplayerprogress)) < bPriority )
					{
						ubAdminsToCheck = pGroup->pEnemyGroup->ubNumAdmins;

						while ( ubAdminsToCheck > 0)
						{
							// chance to upgrade at each check is random, and also dependant on the group's priority
							if ( Chance ( bPriority ) ) 
							{
								pGroup->pEnemyGroup->ubNumAdmins--;
								pGroup->pEnemyGroup->ubNumTroops++;
							}

							ubAdminsToCheck--;
						}

						// Flugente: make sure turncoat numbers are plausible
						if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat > pGroup->pEnemyGroup->ubNumAdmins )
						{
							pGroup->pEnemyGroup->ubNumTroops_Turncoat += pGroup->pEnemyGroup->ubNumAdmins_Turncoat - pGroup->pEnemyGroup->ubNumAdmins;
							pGroup->pEnemyGroup->ubNumAdmins_Turncoat = pGroup->pEnemyGroup->ubNumAdmins;
						}
					}
				}
			}

			pGroup = pGroup->next;
		}
	}
}


INT16 FindPatrolGroupIndexForGroupID( UINT8 ubGroupID )
{
	INT16 sPatrolIndex;

	for( sPatrolIndex = 0; sPatrolIndex < giPatrolArraySize; sPatrolIndex++ )
	{
		if ( gPatrolGroup[ sPatrolIndex ].ubGroupID == ubGroupID )
		{
			// found it
			return( sPatrolIndex );
		}
	}

	// not there!
	return( -1 );
}


INT16 FindPatrolGroupIndexForGroupIDPending( UINT8 ubGroupID )
{
	INT16 sPatrolIndex;

	for( sPatrolIndex = 0; sPatrolIndex < giPatrolArraySize; sPatrolIndex++ )
	{
		if ( gPatrolGroup[ sPatrolIndex ].ubPendingGroupID == ubGroupID )
		{
			// found it
			return( sPatrolIndex );
		}
	}

	// not there!
	return( -1 );
}


INT16 FindGarrisonIndexForGroupIDPending( UINT8 ubGroupID )
{
	INT16 sGarrisonIndex;
 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	for( sGarrisonIndex = 0; sGarrisonIndex < giGarrisonArraySize; sGarrisonIndex++ )
	{
		if ( gGarrisonGroup[ sGarrisonIndex ].ubPendingGroupID == ubGroupID )
		{
			// found it
			return( sGarrisonIndex );
		}
	}

	// not there!
	return( -1 );
}

void TransferGroupToPool( GROUP **pGroup )
{
	//Madd: unlimited reinforcements?
	if ( !gfUnlimitedTroops )
		giReinforcementPool += (*pGroup)->ubGroupSize;

	RemovePGroup( *pGroup );
	*pGroup = NULL;
}

//NOTE:	Make sure you call SetEnemyGroupSector() first if the group is between sectors!!	See example in ReassignAIGroup()...
void SendGroupToPool( GROUP **pGroup )
{
	if( (*pGroup)->ubSectorX == gModSettings.ubSAISpawnSectorX && (*pGroup)->ubSectorY == gModSettings.ubSAISpawnSectorY )
	{
		TransferGroupToPool( pGroup );
	}
	else
	{
		(*pGroup)->ubSectorIDOfLastReassignment = (UINT8)SECTOR( (*pGroup)->ubSectorX, (*pGroup)->ubSectorY );
		MoveSAIGroupToSector( pGroup, SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), EVASIVE, REINFORCEMENTS );
	}
}

void ReassignAIGroup( GROUP **pGroup )
{
	INT32 i, iRandom;
	INT32 iWeight;
	UINT16 usDefencePoints = 0;
	INT32 iReloopLastIndex = -1;
	UINT8 ubSectorID;

	ubSectorID = (UINT8)SECTOR( (*pGroup)->ubSectorX, (*pGroup)->ubSectorY );

	(*pGroup)->ubSectorIDOfLastReassignment = ubSectorID;

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	ClearPreviousAIGroupAssignment( *pGroup );

	//First thing to do, is teleport the group to be AT the sector he is currently moving from.	Otherwise, the
	//strategic pathing can break if the group is between sectors upon reassignment.
	SetEnemyGroupSector( *pGroup, ubSectorID );

	if( giRequestPoints <= 0	)
	{ //we have no request for reinforcements, so send the group to Meduna for reassignment in the pool.
		SendGroupToPool( pGroup );
		return;
	}

	//now randomly choose who gets the reinforcements.
	// giRequestPoints is the combined sum of all the individual weights of all garrisons and patrols requesting reinforcements
	iRandom = Random( giRequestPoints );

	//go through garrisons first and begin considering where the random value dictates.	If that garrison doesn't require
	//reinforcements, it'll continue on considering all subsequent garrisons till the end of the array.	If it fails at that
	//point, it'll restart the loop at zero, and consider all garrisons to the index that was first considered by the random value.
	for( i = 0; i < giGarrisonArraySize; ++i )
	{
		RecalculateGarrisonWeight( i );
		iWeight = gGarrisonGroup[ i ].bWeight;
		if( iWeight > 0 )
		{	//if group is requesting reinforcements.
			if( iRandom < iWeight )
			{
				if( !gGarrisonGroup[ i ].ubPendingGroupID &&
						EnemyPermittedToAttackSector( NULL, gGarrisonGroup[ i ].ubSectorID ) &&
						GarrisonRequestingMinimumReinforcements( i ) )
				{
					//This is the group that gets the reinforcements!
					if( ReinforcementsApproved( i, &usDefencePoints ) )
					{
						if ( SendReinforcementsForGarrison( i, usDefencePoints, pGroup ) )
							return;
					}
				}

				if( iReloopLastIndex == -1 )
				{ //go to the next garrison and clear the iRandom value so it attempts to use all subsequent groups.
					iReloopLastIndex = i - 1;
					iRandom = 0;
				}
			}
			//Decrease the iRandom value until it hits 0.	When that happens, all garrisons will get considered until
			//we either have a match or process all of the garrisons.
			iRandom -= iWeight;
		}
	}
	if( iReloopLastIndex >= 0 )
	{
		//Process the loop again to the point where the original random slot started considering, and consider
		//all of the garrisons.	If this fails, all patrol groups will be considered next.
		for( i = 0; i <= iReloopLastIndex; ++i )
		{
			RecalculateGarrisonWeight( i );
			iWeight = gGarrisonGroup[ i ].bWeight;
			if( iWeight > 0 )
			{
				//if group is requesting reinforcements.
				if( !gGarrisonGroup[ i ].ubPendingGroupID &&
						EnemyPermittedToAttackSector( NULL, gGarrisonGroup[ i ].ubSectorID ) &&
						GarrisonRequestingMinimumReinforcements( i ) )
				{
					//This is the group that gets the reinforcements!
					if( ReinforcementsApproved( i, &usDefencePoints ) )
					{
						if ( SendReinforcementsForGarrison( i, usDefencePoints, pGroup ) )
							return;
					}
				}
			}
		}
	}

	if( iReloopLastIndex == -1 )
	{
		//go through the patrol groups
		for( i = 0; i < giPatrolArraySize; ++i )
		{
			RecalculatePatrolWeight( i );
			iWeight = gPatrolGroup[ i ].bWeight;
			if( iWeight > 0 )
			{
				if( iRandom < iWeight )
				{
					if( !gPatrolGroup[ i ].ubPendingGroupID && PatrolRequestingMinimumReinforcements( i ) )
					{
						//This is the group that gets the reinforcements!
						if ( SendReinforcementsForPatrol( i, pGroup ) )
							return;
					}
				}
				if( iReloopLastIndex == -1 )
				{
					iReloopLastIndex = i - 1;
					iRandom = 0;
				}
				iRandom -= iWeight;
			}
		}
	}
	else
	{
		iReloopLastIndex = giPatrolArraySize - 1;
	}

	for( i = 0; i <= iReloopLastIndex; ++i )
	{
		RecalculatePatrolWeight( i );
		iWeight = gPatrolGroup[ i ].bWeight;
		if( iWeight > 0 )
		{
			if( !gPatrolGroup[ i ].ubPendingGroupID && PatrolRequestingMinimumReinforcements( i ) )
			{
				//This is the group that gets the reinforcements!
				if ( SendReinforcementsForPatrol( i, pGroup ) )
					return;
			}
		}
	}

	// Flugente: at least have them walk back correctly instead of just having them vanish
	SendGroupToPool( pGroup );
}

//When an enemy AI group is eliminated by the player, apply a grace period in which the
//group isn't allowed to be filled for several days.
void TagSAIGroupWithGracePeriod( GROUP *pGroup )
{
	INT32 iPatrolID;
	if( pGroup )
	{
		iPatrolID = FindPatrolGroupIndexForGroupID( pGroup->ubGroupID );
	
		// check for on the way patrol group
		if( iPatrolID == -1 )
			iPatrolID = FindPatrolGroupIndexForGroupIDPending( pGroup->ubGroupID );

		if( iPatrolID != -1 )
		{
			gPatrolGroup[ iPatrolID ].bFillPermittedAfterDayMod100 = (UINT8)((GetWorldDay() + zDiffSetting[gGameOptions.ubDifficultyLevel].iGracePeriodInDaysAfterPatrolDestroyed) % 100);
		}
	}
}

BOOLEAN PermittedToFillPatrolGroup( INT32 iPatrolID )
{
	INT32 iDay;
	INT32 iDayAllowed;
	iDay = GetWorldDay();
	iDayAllowed = gPatrolGroup[ iPatrolID ].bFillPermittedAfterDayMod100 + (iDay / 100) * 100;
	return iDay >= iDayAllowed;
}

void RepollSAIGroup( GROUP *pGroup )
{
	INT32 i;
	Assert( pGroup->usGroupTeam != OUR_TEAM );

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	if( GroupAtFinalDestination( pGroup ) )
	{
		EvaluateGroupSituation( pGroup );
		return;
	}
	for( i = 0; i < giPatrolArraySize; i++ )
	{
		if( gPatrolGroup[ i ].ubGroupID == pGroup->ubGroupID )
		{
			RecalculatePatrolWeight( i ); //in case there are any dead enemies
			CalculateNextMoveIntention( pGroup );
			return;
		}
	}
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		//KM : August 6, 1999 Patch fix
		//	 Ack, wasn't checking for the matching group to garrison
		if( gGarrisonGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
		//end
		{
			RecalculateGarrisonWeight( i ); //in case there are any dead enemies
			CalculateNextMoveIntention( pGroup );
			return;
		}
	}
}

void ClearPreviousAIGroupAssignment( GROUP *pGroup )
{
	INT32 i;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	for( i = 0; i < giPatrolArraySize; i++ )
	{
		if( gPatrolGroup[ i ].ubGroupID == pGroup->ubGroupID )
		{
			gPatrolGroup[ i ].ubGroupID = 0;
			RecalculatePatrolWeight( i );
			return;
		}
		if( gPatrolGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
		{
			gPatrolGroup[ i ].ubPendingGroupID = 0;
			return;
		}
	}
	//Also check if this group was a garrison's pending group
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		if( gGarrisonGroup[ i ].ubPendingGroupID == pGroup->ubGroupID )
		{
			gGarrisonGroup[ i ].ubPendingGroupID = 0;
			return;
		}
	}
}

void CalcNumTroopsBasedOnComposition( UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 ubTotal, INT32 iCompositionID )
{
	*pubNumTroops = gArmyComp[ iCompositionID ].bTroopPercentage * ubTotal / 100;
	*pubNumElites = gArmyComp[ iCompositionID ].bElitePercentage * ubTotal / 100;

	//Due to low roundoff, it is highly possible that we will be short one soldier.
	while( *pubNumTroops + *pubNumElites < ubTotal )
	{
		if( Chance( gArmyComp[ iCompositionID ].bTroopPercentage ) )
		{
			(*pubNumTroops)++;
		}
		else
		{
			(*pubNumElites)++;
		}
	}
	//CHRISL: For some reason, it's possible for pubNumTroops + pubNumElites to be greater then ubTotal.  It seems that in some
	//	cases, bTroopPercentage and bElitePercentage can add up to more then 100%.  So we need an extra condition that makes
	//	sure we don't have more then ubTotal
	while( *pubNumTroops + *pubNumElites > ubTotal)
	{
		if( Chance( gArmyComp[ iCompositionID ].bTroopPercentage ) )
		{
			(*pubNumTroops)--;
		}
		else
		{
			(*pubNumElites)--;
		}
	}
	Assert( *pubNumTroops + *pubNumElites == ubTotal );
}

void ConvertGroupTroopsToComposition( GROUP *pGroup, INT32 iCompositionID )
{
	Assert( pGroup );
	Assert( pGroup->usGroupTeam == ENEMY_TEAM );
	CalcNumTroopsBasedOnComposition( &pGroup->pEnemyGroup->ubNumTroops, &pGroup->pEnemyGroup->ubNumElites, pGroup->ubGroupSize, iCompositionID );
	pGroup->pEnemyGroup->ubNumAdmins = 0;
	pGroup->ubGroupSize = pGroup->pEnemyGroup->ubNumTroops + pGroup->pEnemyGroup->ubNumElites;
	ValidateLargeGroup( pGroup );
}

void RemoveSoldiersFromGarrisonBasedOnComposition( INT32 iGarrisonID, UINT8 ubSize )
{
	SECTORINFO *pSector;
	INT32 iCompositionID;
	UINT8 ubNumTroops, ubNumElites;

	//debug stuff
	UINT8 ubOrigSectorAdmins;
	UINT8 ubOrigSectorTroops;
	UINT8 ubOrigSectorElites;
	UINT8 ubOrigNumElites;
	UINT8 ubOrigNumTroops;
	UINT8 ubOrigSize;

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */
	iCompositionID = gGarrisonGroup[ iGarrisonID ].ubComposition;

	CalcNumTroopsBasedOnComposition( &ubNumTroops, &ubNumElites, ubSize, iCompositionID );
	pSector = &SectorInfo[ gGarrisonGroup[ iGarrisonID ].ubSectorID ];
	//if there are administrators in this sector, remove them first.

	ubOrigNumElites = ubNumElites;
	ubOrigNumTroops = ubNumTroops;
	ubOrigSectorAdmins = pSector->ubNumAdmins;
	ubOrigSectorTroops = pSector->ubNumTroops;
	ubOrigSectorElites = pSector->ubNumElites;
	ubOrigSize = ubSize;

	while( ubSize && pSector->ubNumAdmins )
	{
		pSector->ubNumAdmins--;
		ubSize--;
		if( ubNumTroops )
		{
			ubNumTroops--;
		}
		else
		{
			ubNumElites--;
		}
	}
	//No administrators are left.

	//Eliminate the troops
	while( ubNumTroops )
	{
		if( pSector->ubNumTroops )
		{
			pSector->ubNumTroops--;
		}
		else if( pSector->ubNumElites )
		{
			pSector->ubNumElites--;
		}
		else
		{
			Assert( 0 );
		}
		ubNumTroops--;
	}

	//Eliminate the elites
	while( ubNumElites )
	{
		if( pSector->ubNumElites )
		{
			pSector->ubNumElites--;
		}
		else if( pSector->ubNumTroops )
		{
			pSector->ubNumTroops--;
		}
		else
		{
			Assert( 0 );
		}
		ubNumElites--;
	}

	RecalculateGarrisonWeight( iGarrisonID );
}

void MoveSAIGroupToSector( GROUP **pGroup, UINT8 ubSectorID, UINT32 uiMoveCode, UINT8 ubIntention )
{
	UINT8 ubDstSectorX, ubDstSectorY;

	ubDstSectorX = (UINT8)SECTORX( ubSectorID );
	ubDstSectorY = (UINT8)SECTORY( ubSectorID );

	if( (*pGroup)->fBetweenSectors )
	{
		SetEnemyGroupSector( *pGroup, (UINT8)SECTOR( (*pGroup)->ubSectorX, (*pGroup)->ubSectorY ) );
	}

	(*pGroup)->pEnemyGroup->ubIntention = ubIntention;
	(*pGroup)->ubMoveType = ONE_WAY;

	if( (*pGroup)->ubSectorX == ubDstSectorX && (*pGroup)->ubSectorY == ubDstSectorY )
	{
		//The destination sector is the current location.	Instead of causing code logic problems,
		//simply process them as if they just arrived.
		if( EvaluateGroupSituation( *pGroup ) )
		{
			//The group was deleted.
			*pGroup = NULL;
			return;
		}
	}

	switch( uiMoveCode )
	{
		case STAGE:
			MoveGroupFromSectorToSectorButAvoidPlayerInfluencedSectorsAndStopOneSectorBeforeEnd( (*pGroup)->ubGroupID, (*pGroup)->ubSectorX, (*pGroup)->ubSectorY, ubDstSectorX, ubDstSectorY );
			break;
		case EVASIVE:
			MoveGroupFromSectorToSectorButAvoidPlayerInfluencedSectors( (*pGroup)->ubGroupID, (*pGroup)->ubSectorX, (*pGroup)->ubSectorY, ubDstSectorX, ubDstSectorY );
			break;
		case DIRECT:
		default:
			MoveGroupFromSectorToSector( (*pGroup)->ubGroupID, (*pGroup)->ubSectorX, (*pGroup)->ubSectorY, ubDstSectorX, ubDstSectorY );
			break;
	}
	//Make sure that the group is moving.	If this fails, then the pathing may have failed for some reason.
	ValidateGroup( *pGroup );
}

//If there are any enemy groups that will be moving through this sector due, they will have to repath which
//will cause them to avoid the sector.	Returns the number of redirected groups.
UINT8 RedirectEnemyGroupsMovingThroughSector( UINT8 ubSectorX, UINT8 ubSectorY )
{
	GROUP *pGroup;
	UINT8 ubNumGroupsRedirected = 0;
	WAYPOINT *pWaypoint;
	UINT8 ubDestSectorID;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && pGroup->ubMoveType == ONE_WAY )
		{
			//check the waypoint list
			if( GroupWillMoveThroughSector( pGroup, ubSectorX, ubSectorY ) )
			{
				//extract the group's destination.
				pWaypoint = GetFinalWaypoint( pGroup );
				Assert( pWaypoint );
				ubDestSectorID = (UINT8)SECTOR( pWaypoint->x, pWaypoint->y );
				SetEnemyGroupSector( pGroup, (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ) );
				MoveSAIGroupToSector( &pGroup, ubDestSectorID, EVASIVE, pGroup->pEnemyGroup->ubIntention );
				ubNumGroupsRedirected++;
			}
		}
		pGroup = pGroup->next;
	}
	if( ubNumGroupsRedirected )
	{
		ScreenMsg( FONT_LTBLUE, MSG_BETAVERSION, L"Test message for new feature:	%d enemy groups were redirected away from moving through sector %c%d.	Please don't report unless this number is greater than 5.",
			ubNumGroupsRedirected, ubSectorY + 'A' - 1, ubSectorX );
	}
	return ubNumGroupsRedirected;
}

//when the SAI compositions change, it is necessary to call this function upon version load,
//to reflect the changes of the compositions to the sector that haven't been visited yet.
void ReinitializeUnvisitedGarrisons()
{
	SECTORINFO *pSector;
	ARMY_COMPOSITION *pArmyComp;
	GROUP *pGroup;
	INT32 i, cnt, iEliteChance, iAdminChance;

	//Recreate the compositions
	memcpy( gArmyComp, gOrigArmyComp, NUM_ARMY_COMPOSITIONS * sizeof( ARMY_COMPOSITION ) );
	EvolveQueenPriorityPhase( TRUE );
 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	//Go through each unvisited sector and recreate the garrison forces based on
	//the desired population.
	for( i = 0; i < giGarrisonArraySize; i++ )
	{
		if( gGarrisonGroup[ i ].ubComposition >= LEVEL1_DEFENCE && gGarrisonGroup[ i ].ubComposition <= LEVEL3_DEFENCE )
		{ //These 3 compositions make up the perimeter around Meduna.	The existance of these are based on the
			//difficulty level, and we don't want to reset these anyways, due to the fact that many of the reinforcements
			//come from these sectors, and it could potentially add upwards of 150 extra troops which would seriously
			//unbalance the difficulty.
			continue;
		}
		pSector = &SectorInfo[ gGarrisonGroup[ i ].ubSectorID ];
		pArmyComp = &gArmyComp[ gGarrisonGroup[ i ].ubComposition ];
		if( !(pSector->uiFlags & SF_ALREADY_VISITED) )
		{
			pSector->ubNumAdmins = 0;
			pSector->ubNumTroops = 0;
			pSector->ubNumElites = 0;
			if( gfQueenAIAwake )
			{
				cnt = pArmyComp->bDesiredPopulation;
			}
			else
			{
				cnt = pArmyComp->bStartPopulation;
			}

			if( gGarrisonGroup[ i ].ubPendingGroupID )
			{ //if the garrison has reinforcements on route, then subtract the number of
				//reinforcements from the value we reset the size of the garrison.	This is to
				//prevent overfilling the group.
				pGroup = GetGroup( gGarrisonGroup[ i ].ubPendingGroupID );
				if( pGroup )
				{
					cnt -= pGroup->ubGroupSize;
					cnt = max( cnt, 0 );
				}
			}

			iEliteChance = pArmyComp->bElitePercentage;
			iAdminChance = pArmyComp->bAdminPercentage;
			if( iAdminChance && !gfQueenAIAwake && cnt )
			{
				pSector->ubNumAdmins = iAdminChance * cnt / 100;
			}
			else while( cnt-- )
			{ //for each person, randomly determine the types of each soldier.
				if( Chance( iEliteChance ) )
				{
					pSector->ubNumElites++;
				}
				else
				{
					pSector->ubNumTroops++;
				}
			}
		}
	}
}

GROUP* FindPendingGroupForGarrisonSector( UINT8 ubSectorID )
{
	GROUP *pGroup;
	SECTORINFO *pSector;
	pSector = &SectorInfo[ ubSectorID ];

 Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	/* added NULL fix, 2007-03-03, Sgt. Kolja */

	if( pSector->ubGarrisonID != NO_GARRISON )
	{
		if( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID )
		{
			pGroup = GetGroup( gGarrisonGroup[ pSector->ubGarrisonID ].ubPendingGroupID );
			Assert( pGroup );
			return pGroup;
		}
	}
	return NULL;
}

//Shadooow: new function to upgrade patrols groups with ASD to avoid overriding default patrol team composition
void ASDInitializePatrolGroup(GROUP *pGroup)
{
	if (pGroup->ubGroupSize > 0 && pGroup->pEnemyGroup->ubNumElites > 0)
	{
		if (gGameExternalOptions.fASDAssignsTanks && ASDSoldierUpgradeToTank())
		{
			pGroup->pEnemyGroup->ubNumElites--;
			pGroup->pEnemyGroup->ubNumTanks++;
		}

		if (pGroup->pEnemyGroup->ubNumElites > 0 && gGameExternalOptions.fASDAssignsJeeps && ASDSoldierUpgradeToJeep())
		{
			pGroup->pEnemyGroup->ubNumElites--;
			pGroup->pEnemyGroup->ubNumJeeps++;
		}

		if (pGroup->pEnemyGroup->ubNumTroops > 0 && gGameExternalOptions.fASDAssignsRobots && ASDSoldierUpgradeToRobot())
		{
			int numRobots = 1 + Random(gGameOptions.ubDifficultyLevel);
			if (numRobots > pGroup->pEnemyGroup->ubNumTroops)
			{
				pGroup->pEnemyGroup->ubNumRobots += pGroup->pEnemyGroup->ubNumTroops;
				pGroup->pEnemyGroup->ubNumTroops = 0;
			}
			else
			{
				pGroup->pEnemyGroup->ubNumRobots += numRobots;
				pGroup->pEnemyGroup->ubNumTroops -= numRobots;
			}
		}
	}
}

void InitializeGroup( const GROUP_TYPE groupType, const UINT8 groupSize, ENEMYGROUP& enemyGroup, const BOOLEAN asdUpgrade )
{
	InitializeGroup( groupType, groupSize, enemyGroup.ubNumTroops, enemyGroup.ubNumElites, enemyGroup.ubNumRobots, enemyGroup.ubNumJeeps, enemyGroup.ubNumTanks, asdUpgrade );
}

void InitializeGroup( const GROUP_TYPE groupType, const UINT8 groupSize, UINT8 &troopCount, UINT8 &eliteCount, UINT8 &robotCount, UINT8 &jeepCount, UINT8 &tankCount, const BOOLEAN asdUpgrade )
{
	troopCount = groupSize;
	eliteCount = 0;
	robotCount = 0;
	jeepCount = 0;
	tankCount = 0;

	UINT32 difficultyMod = 0;
	switch (gGameOptions.ubDifficultyLevel)
	{
	case DIF_LEVEL_EASY:	difficultyMod = 1; break;
	case DIF_LEVEL_MEDIUM:	difficultyMod = 2; break;
	case DIF_LEVEL_HARD:	difficultyMod = 3; break;
	case DIF_LEVEL_INSANE:	difficultyMod = 4; break;
	default:				difficultyMod = 1; break;
	}
	
	if ( asdUpgrade && groupSize > 0 )
	{
		if ( gGameExternalOptions.fASDAssignsTanks && ASDSoldierUpgradeToTank( ) )
		{
			troopCount--;
			tankCount++;
		}

		if ( gGameExternalOptions.fASDAssignsJeeps && ASDSoldierUpgradeToJeep( ) )
		{
			troopCount--;
			jeepCount++;
		}

		if ( gGameExternalOptions.fASDAssignsRobots && ASDSoldierUpgradeToRobot() )
		{
			const int numRobots = 1 + Random(difficultyMod);
			troopCount -= numRobots;
			robotCount += numRobots;
		}
	}

	// replace some soldiers with elites for attack groups
	if ( groupType == GROUP_TYPE_ATTACK && difficultyMod > 0 )
	{
		eliteCount += troopCount - troopCount / difficultyMod;
		troopCount -= eliteCount;
	}

}
