#ifndef __SKILL_CHECK_H
#define __SKILL_CHECK_H


#include "types.h"
#include "Soldier Control.h"

extern void ReducePointsForFatigue( SOLDIERTYPE *pSoldier, UINT32 *pusPoints );
extern INT32 GetSkillCheckPenaltyForFatigue( SOLDIERTYPE *pSoldier, INT32 iSkill );
extern INT32 SkillCheck( SOLDIERTYPE *pSoldier, INT8 bReason, INT8 bDifficulty );
extern INT16 CalcTrapDetectLevel( SOLDIERTYPE *pSoldier, BOOLEAN fExamining );


extern	INT16 EffectiveStrength( SOLDIERTYPE *pSoldier, BOOLEAN fTrainer );
extern	INT16 EffectiveWisdom( SOLDIERTYPE *pSoldier );
extern	INT16 EffectiveAgility( SOLDIERTYPE *pSoldier, BOOLEAN fTrainer );
extern	INT8 EffectiveMechanical( SOLDIERTYPE *pSoldier );
extern	INT8 EffectiveExplosive( SOLDIERTYPE *pSoldier );
extern	INT8 EffectiveLeadership( SOLDIERTYPE *pSoldier );
extern	INT8 EffectiveMarksmanship( SOLDIERTYPE *pSoldier );
extern	INT16 EffectiveDexterity( SOLDIERTYPE *pSoldier, BOOLEAN fTrainer );
extern	INT8 EffectiveExpLevel( SOLDIERTYPE *pSoldier );
extern	INT8 EffectiveMedical( SOLDIERTYPE *pSoldier );


enum SkillChecks
{
	NO_CHECK = 0,
	LOCKPICKING_CHECK,
	ELECTRONIC_LOCKPICKING_CHECK,
	ATTACHING_DETONATOR_CHECK,
	ATTACHING_REMOTE_DETONATOR_CHECK,
	PLANTING_BOMB_CHECK,
	PLANTING_REMOTE_BOMB_CHECK,
	OPEN_WITH_CROWBAR,
	SMASH_DOOR_CHECK,
	DISARM_TRAP_CHECK,
	UNJAM_GUN_CHECK,
	NOTICE_DART_CHECK,
	LIE_TO_QUEEN_CHECK,
	ATTACHING_SPECIAL_ITEM_CHECK,
	ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,
	DISARM_ELECTRONIC_TRAP_CHECK,
	ATTACH_POWER_PACK,					// Flugente: attach a power pack to power armor
} ;



#endif