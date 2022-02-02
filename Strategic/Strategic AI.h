#ifndef __STRATEGIC_AI_H
#define __STRATEGIC_AI_H

#include "FileMan.h"
#include "Strategic Movement.h"

void InitStrategicAI();
void KillStrategicAI();

BOOLEAN SaveStrategicAI( HWFILE hFile );
BOOLEAN LoadStrategicAI( HWFILE hFile );

//NPC ACTION TRIGGERS SPECIAL CASE AI
enum
{
	STRATEGIC_AI_ACTION_WAKE_QUEEN = 1,
	STRATEGIC_AI_ACTION_KINGPIN_DEAD,
	STRATEGIC_AI_ACTION_QUEEN_DEAD,

};

// WDS - New AI
void ExecuteStrategicAIAction(
							  UINT16 usActionCode,  // See list in "interface Dialogue.h"
							  INT16 sSectorX,		// Target X location of strategic action
							  INT16 sSectorY,		// Target Y location of strategic action
							  INT32 option1 = 0,	// Option 1, vaies with action
							  INT32 option2 = 0		// Option 2, vaies with action
							  );

void CheckEnemyControlledSector( UINT8 ubSectorID );
void EvaluateQueenSituation();

extern BOOLEAN gfUseAlternateQueenPosition;

//returns TRUE if the group was deleted.
BOOLEAN StrategicAILookForAdjacentGroups( GROUP *pGroup );
void RemoveGroupFromStrategicAILists( UINT8 ubGroupID );
void RecalculateSectorWeight( UINT8 ubSectorID );
void RecalculateGroupWeight( GROUP *pGroup );

BOOLEAN OkayForEnemyToMoveThroughSector( UINT8 ubSectorID );
BOOLEAN EnemyPermittedToAttackSector( GROUP **pGroup, UINT8 ubSectorID );

void StrategicHandleQueenLosingControlOfSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

void InvestigateSector( UINT8 ubSectorID );

// WDS - New AI
void HourlyCheckStrategicAI();

void WakeUpQueen();

void StrategicHandleMineThatRanOut( UINT8 ubSectorID );

INT16 FindPatrolGroupIndexForGroupID( UINT8 ubGroupID );
INT16 FindPatrolGroupIndexForGroupIDPending( UINT8 ubGroupID );
INT16 FindGarrisonIndexForGroupIDPending( UINT8 ubGroupID );

GROUP* FindPendingGroupInSector( UINT8 ubSectorID );

enum SAIMOVECODE
{
	DIRECT,
	EVASIVE,
	STAGE,
};
void MoveSAIGroupToSector( GROUP **pGroup, UINT8 ubSectorID, UINT32 uiMoveCode, UINT8 ubIntention );

void RepollSAIGroup( GROUP *pGroup );

//When an enemy AI group is eliminated by the player, apply a grace period in which the 
//group isn't allowed to be filled for several days.
void TagSAIGroupWithGracePeriod( GROUP *pGroup );

BOOLEAN PermittedToFillPatrolGroup( INT32 iPatrolID );

enum GROUP_TYPE
{
	GROUP_TYPE_ATTACK,
	GROUP_TYPE_PATROL
};

void ASDInitializePatrolGroup(GROUP *pGroup);
void InitializeGroup(const GROUP_TYPE groupType, const UINT8 groupSize, ENEMYGROUP& enemyGroup, const BOOLEAN asdUpgrade);
void InitializeGroup(const GROUP_TYPE groupType, const UINT8 groupSize, UINT8 &troopCount, UINT8 &eliteCount, UINT8 &robotCount, UINT8 &jeepCount, UINT8 &tankCount, const BOOLEAN asdUpgrade);

extern BOOLEAN gfDisplayStrategicAILogs;
extern BOOLEAN gfFirstBattleMeanwhileScenePending;

extern UINT8 gubSAIVersion;
extern BOOLEAN gfUnlimitedTroops;
extern INT32 giReinforcementPool;
extern INT32 giTotalRecruitsInTraining;


//These enumerations define all of the various types of stationary garrison
//groups, and index their compositions for forces, etc.
enum
{
	QUEEN_DEFENCE,			//The most important sector, the queen's palace.
	MEDUNA_DEFENCE,			//The town surrounding the queen's palace.
	MEDUNA_SAMSITE,			//A sam site within Meduna (higher priority)
	LEVEL1_DEFENCE,			//The sectors immediately adjacent to Meduna (defence and spawning area)
	LEVEL2_DEFENCE,			//Two sectors away from Meduna (defence and spawning area)
	LEVEL3_DEFENCE,			//Three sectors away from Meduna (defence and spawning area)
	ORTA_DEFENCE,				//The top secret military base containing lots of elites
	EAST_GRUMM_DEFENCE,	//The most-industrial town in Arulco (more mine income)
	WEST_GRUMM_DEFENCE,	//The most-industrial town in Arulco (more mine income)
	GRUMM_MINE,
	OMERTA_WELCOME_WAGON,//Small force that greets the player upon arrival in game.
	BALIME_DEFENCE,			//Rich town, paved roads, close to Meduna (in queen's favor)
	TIXA_PRISON,				//Prison, well defended, but no point in retaking
	TIXA_SAMSITE,				//The central-most sam site (important for queen to keep)
	ALMA_DEFENCE,				//The military town of Meduna.	Also very important for queen.
	ALMA_MINE,					//Mine income AND administrators
	CAMBRIA_DEFENCE,		//Medical town, large, central.
	CAMBRIA_MINE,
	CHITZENA_DEFENCE,		//Small town, small mine, far away.
	CHITZENA_MINE,
	CHITZENA_SAMSITE,		//Sam site near Chitzena.
	DRASSEN_AIRPORT,		//Very far away, a supply depot of little importance.
	DRASSEN_DEFENCE,		//Medium town, normal.
	DRASSEN_MINE,
	DRASSEN_SAMSITE,		//Sam site near Drassen (least importance to queen of all samsites)
	ROADBLOCK,					//General outside city roadblocks -- enhance chance of ambush?
	SANMONA_SMALL
};

extern UINT8 NUM_ARMY_COMPOSITIONS;

typedef struct ARMY_COMPOSITION
{
	INT32 iReadability;					//contains the enumeration which is useless, but helps readability.
	INT8	bPriority;
	INT8	bElitePercentage;
	INT8	bTroopPercentage;
	INT8	bAdminPercentage;
	INT8	bDesiredPopulation;
	INT8	bStartPopulation;
	INT8	bPadding[10];
}ARMY_COMPOSITION;

//Defines the patrol groups -- movement groups.
typedef struct PATROL_GROUP
{ 
	INT8	bSize;
	INT8	bPriority;
	UINT8 ubSectorID[4];
	INT8	bFillPermittedAfterDayMod100;
	UINT8 ubGroupID;
	INT8	bWeight;
	UINT8 ubPendingGroupID;
	INT8	bPadding[10];
}PATROL_GROUP;

//Defines all stationary defence forces. 
typedef struct GARRISON_GROUP
{
	UINT8 ubSectorID;
	UINT8 ubComposition;
	INT8	bWeight;
	UINT8 ubPendingGroupID;
	INT8	bPadding[10];
}GARRISON_GROUP;

#define MAX_GARRISON_GROUPS	256	// WANNE - MP: Changed from 100 to 256
#define MAX_PATROL_GROUPS	 50
#define MAX_ARMY_COMPOSITIONS 60

// attack/defense point modifiers, used to determine if the AI should attack a sector
#define ADMIN_POINTS_MODIFIER 2
#define REGULAR_POINTS_MODIFIER 4
#define ELITE_POINTS_MODIFIER 6
#define ROBOT_POINTS_MODIFIER 15
#define JEEP_POINTS_MODIFIER 12
#define TANK_POINTS_MODIFIER 20
#define GREEN_MILITIA_POINTS_MODIFIER 1
#define REGULAR_MILITIA_POINTS_MODIFIER 2
#define ELITE_MILITIA_POINTS_MODIFIER 3

#endif