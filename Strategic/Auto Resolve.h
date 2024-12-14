#ifndef __AUTO_RESOLVE_H
#define __AUTO_RESOLVE_H

#include "types.h"

//generic face images
enum
{
	ADMIN_FACE,
	TROOP_FACE,
	ELITE_FACE,
	TANK_FACE,
	MILITIA1_FACE,
	MILITIA2_FACE,
	MILITIA3_FACE,
	YM_CREATURE_FACE,
	AM_CREATURE_FACE,
	YF_CREATURE_FACE,
	AF_CREATURE_FACE,
	HUMAN_SKULL,
	TANK_WRECK,
	CREATURE_SKULL,
	ADMINF_FACE,
	TROOPF_FACE,
	ELITEF_FACE,
	MILITIA1F_FACE,
	MILITIA2F_FACE,
	MILITIA3F_FACE,
	JEEP_FACE,
	JEEP_WRECK,
	ZOMBIE_FACE,
	BANDIT1_FACE,
	BANDIT1_F_FACE = BANDIT1_FACE + 4,
	BLOODCAT_FACE = BANDIT1_F_FACE + 4,
	BLOODCAT_SKULL,

	AUTORESOLVEFACES_MAX
};

void EnterAutoResolveMode( UINT8 ubSectorX, UINT8 ubSectorY );

// is the autoresolve active?
BOOLEAN IsAutoResolveActive( void );

void EliminateAllEnemies( UINT8 ubSectorX, UINT8 ubSectorY );

void ConvertTacticalBattleIntoStrategicAutoResolveBattle();

UINT8 GetAutoResolveSectorID();

extern BOOLEAN gfTransferTacticalOppositionToAutoResolve;

//Returns TRUE if autoresolve is active or a sector is loaded.
BOOLEAN GetCurrentBattleSectorXYZ( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ );

void CheckForSoldiersWhoRetreatedIntoMilitiaHeldSectors();

// Flugente: have all militia in autoresolve drop their gear and be promoted in necessary
void AutoResolveMilitiaDropAndPromote();

BOOLEAN IndividualMilitiaInUse_AutoResolve( UINT32 aMilitiaId );

#endif
