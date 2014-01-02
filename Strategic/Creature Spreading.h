#ifndef __CREATURE_SPREADING_H
#define __CREATURE_SPREADING_H

void InitCreatureQuest();
void SpreadCreatures();
void DecayCreatures();
void ChooseCreatureQuestStartDay();
void ClearCreatureQuest();
void DeleteCreatureDirectives();

BOOLEAN SaveCreatureDirectives( HWFILE hFile );
BOOLEAN LoadCreatureDirectives( HWFILE hFile, UINT32 uiSavedGameVersion );

UINT8 CreaturesInUndergroundSector( UINT8 ubSectorID, UINT8 ubSectorZ );
BOOLEAN PrepareCreaturesForBattle();
void CreatureNightPlanning();
void CreatureAttackTown( UINT8 ubSectorID, BOOLEAN fOverrideTest );

void CheckConditionsForTriggeringCreatureQuest( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void ForceCreaturesToAvoidMineTemporarily( UINT8 ubMineIndex );

//extern BOOLEAN gfUseCreatureMusic;

BOOLEAN MineClearOfMonsters( UINT8 ubMineIndex );

//Returns TRUE if valid and creature quest over, FALSE if creature quest active or not yet started
BOOLEAN GetWarpOutOfMineCodes( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ, INT32 *psInsertionGridNo );

extern INT16 gsCreatureInsertionCode;
extern INT32 gsCreatureInsertionGridNo;
extern UINT8 gubNumCreaturesAttackingTown;
extern UINT8 gubYoungMalesAttackingTown;
extern UINT8 gubYoungFemalesAttackingTown;
extern UINT8 gubAdultMalesAttackingTown;
extern UINT8 gubAdultFemalesAttackingTown;
extern UINT8 gubSectorIDOfCreatureAttack;
enum{
	CREATURE_BATTLE_CODE_NONE,
	CREATURE_BATTLE_CODE_TACTICALLYADD,
	CREATURE_BATTLE_CODE_TACTICALLYADD_WITHFOV,
	CREATURE_BATTLE_CODE_PREBATTLEINTERFACE,
	CREATURE_BATTLE_CODE_AUTORESOLVE,
};
extern UINT8 gubCreatureBattleCode;

void DetermineCreatureTownComposition( UINT8 ubNumCreatures, 
																			UINT8 *pubNumYoungMales, UINT8 *pubNumYoungFemales,
																			UINT8 *pubNumAdultMales, UINT8 *pubNumAdultFemales );

void DetermineCreatureTownCompositionBasedOnTacticalInformation( UINT8 *pubNumCreatures, 
																			UINT8 *pubNumYoungMales, UINT8 *pubNumYoungFemales,
																			UINT8 *pubNumAdultMales, UINT8 *pubNumAdultFemales );


BOOLEAN PlayerGroupIsInACreatureInfestedMine();

extern INT32 giLairID;

// Buggler: creature XML definitions
// actual infectible sites defined in XML
extern UINT8 NUMBER_OF_INFECTIBLE_SITES;
extern UINT8 NUMBER_OF_CREATURE_COMPOSITIONS;

#define MAX_NUMBER_OF_INFECTIBLE_SITES			10
#define MAX_NUMBER_OF_CREATURE_SECTORS			255
#define MAX_NUMBER_OF_CREATURE_COMPOSITIONS		50

typedef struct CREATUREHABITAT
{
	INT16 sX, sY;
	UINT8 ubZ;
	UINT8 ubComposition;
	BOOLEAN fValid;
} CREATUREHABITAT;

typedef struct CREATUREPLACEMENT
{
	INT16 sAttackSourceX, sAttackSourceY;
	INT32 iAttackSourceGridNo;
	INT16 sAltMapX, sAltMapY;
	UINT8 ubAltMapZ;
	INT16 sWarpToX, sWarpToY;
	UINT8 ubWarpToZ;
	INT32 iWarpToGridNo;
	INT16 sQueenX, sQueenY;
	UINT8 ubQueenZ;
	CREATUREHABITAT Habitat[ MAX_NUMBER_OF_CREATURE_SECTORS ];
}CREATUREPLACEMENT;

typedef struct CREATURECOMPOSITION
{
	UINT8 ubLarvaePercent, ubInfantPercent;
	UINT8 ubYoungMalePercent, ubYoungFemalePercent;
	UINT8 ubAdultMalePercent, ubAdultFemalePercent;
	INT32 iMaxPopulation;
}CREATURECOMPOSITION;

// This array stores all XML-read creature placement data, valid giLairID starts at 1
extern CREATUREPLACEMENT gCreaturePlacements[ MAX_NUMBER_OF_INFECTIBLE_SITES + 1 ];
extern CREATURECOMPOSITION gCreatureComposition[ MAX_NUMBER_OF_CREATURE_COMPOSITIONS ];

#endif