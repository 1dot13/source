#ifndef __PREBATTLE_INTERFACE_H
#define __PREBATTLE_INTERFACE_H

#include "types.h"
#include "Strategic Movement.h"

void InitPreBattleInterface( GROUP *pBattleGroup, BOOLEAN fPersistantPBI );
void KillPreBattleInterface();
void RenderPreBattleInterface();
void ScrollPreBattleInterface( BOOLEAN fUp );

extern BOOLEAN gfPreBattleInterfaceActive;
extern BOOLEAN gfDisplayPotentialRetreatPaths;
extern BOOLEAN gfAutomaticallyStartAutoResolve;
extern BOOLEAN fDisableMapInterfaceDueToBattle;

extern GROUP *gpBattleGroup;

extern BOOLEAN gfTacticalTraversal;
extern GROUP *gpTacticalTraversalGroup;
extern SOLDIERTYPE *gpTacticalTraversalChosenSoldier;

extern BOOLEAN gfGotoSectorTransition;
extern BOOLEAN gfEnteringMapScreenToEnterPreBattleInterface;

enum
{
	//General encounter codes (gubEnemyEncounterCode)
	NO_ENCOUNTER_CODE,			//when there is no encounter
	ENEMY_INVASION_CODE,
	ENEMY_ENCOUNTER_CODE,
	ENEMY_AMBUSH_CODE,
	ENTERING_ENEMY_SECTOR_CODE,
	CREATURE_ATTACK_CODE,

	BLOODCAT_AMBUSH_CODE,
	ENTERING_BLOODCAT_LAIR_CODE,

	//Explicit encounter codes only (gubExplicitEnemyEncounterCode -- a superset of gubEnemyEncounterCode)
	FIGHTING_CREATURES_CODE,
	HOSTILE_CIVILIANS_CODE,
	HOSTILE_BLOODCATS_CODE,

	ENEMY_AMBUSH_DEPLOYMENT_CODE,	// Flugente: an ambush in which we can deploy our mercs (somewhat)
	ENEMY_INVASION_AIRDROP_CODE,	// Flugente: enemy airdrop
	CONCEALINSERTION_CODE,			// Flugente: a spy is no longer concealed, this causes us to drop into combat

	BLOODCAT_ATTACK_CODE,			// Flugente: like CREATURE_ATTACK_CODE, but with cats
	ZOMBIE_ATTACK_CODE,				// Flugente: like CREATURE_ATTACK_CODE, but with zombies
	BANDIT_ATTACK_CODE,				// Flugente: like CREATURE_ATTACK_CODE, but with bandits

	TRANSPORT_INTERCEPT_CODE,		// rftr: like ENEMY_ENCOUNTER_CODE
};

extern BOOLEAN gfAutoAmbush;
extern BOOLEAN gfHighPotentialForAmbush;

extern void ActivatePreBattleAutoresolveAction();
extern void ActivatePreBattleEnterSectorAction();
extern void ActivatePreBattleRetreatAction();
extern void ActivateAutomaticAutoResolveStart();

void CalculateNonPersistantPBIInfo();
void ClearNonPersistantPBIInfo();

//SAVE START

//Using the ESC key in the PBI will get rid of the PBI and go back to mapscreen, but 
//only if the PBI isn't persistant (!gfPersistantPBI).
extern BOOLEAN gfPersistantPBI;

//Contains general information about the type of encounter the player is faced with.	This
//determines whether or not you can autoresolve the battle or even retreat.	This code
//dictates the header that is used at the top of the PBI.
UINT8 GetEnemyEncounterCode();
void SetEnemyEncounterCode( UINT8 aCode );

//The autoresolve during tactical battle option needs more detailed information than the 
//gubEnemyEncounterCode can provide.	The explicit version contains possibly unique codes
//for reasons not normally used in the PBI.	For example, if we were fighting the enemy
//in a normal situation, then shot at a civilian, the civilians associated with the victim
//would turn hostile, which would disable the ability to autoresolve the battle.
UINT8 GetExplicitEnemyEncounterCode();
void SetExplicitEnemyEncounterCode( UINT8 aCode );

//Location of the current battle (determines where the animated icon is blitted) and if the
//icon is to be blitted.
extern BOOLEAN gfBlitBattleSectorLocator;

extern UINT8 gubPBSectorX;
extern UINT8 gubPBSectorY;
extern UINT8 gubPBSectorZ;

extern BOOLEAN gfCantRetreatInPBI;

extern BOOLEAN gubSpecialEncounterCodeForEnemyHeli;
//SAVE END

void WakeUpAllMercsInSectorUnderAttack( void );

void ClearMovementForAllInvolvedPlayerGroups( void );
void RetreatAllInvolvedPlayerGroups( void );
void RetreatAllInvolvedMilitiaGroups( void );

BOOLEAN PlayerGroupInvolvedInThisCombat( GROUP *pGroup );
BOOLEAN MilitiaGroupInvolvedInThisCombat( GROUP *pGroup );
BOOLEAN PlayerMercInvolvedInThisCombat( SOLDIERTYPE *pSoldier );

BOOLEAN CurrentBattleSectorIs( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

enum
{
	LOG_DEFEAT,
	LOG_VICTORY,
};
void LogBattleResults( UINT8 ubVictoryCode);

#endif
