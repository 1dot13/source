	#include "builddefines.h"
	#include <stdio.h>
	#include <memory.h>

	#include "types.h"

	#include "Keys.h"
	#include "debug.h"
	#include "Smoothing Utils.h"
	#include "SkillCheck.h"
	#include "opplist.h"
	#include "items.h"
	#include "weapons.h"
	#include "ai.h"
	#include "message.h"
	#include "text.h"
	#include "explosion control.h"
	#include "isometric utils.h"
	#include "StrategicMap.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "los.h"
	#include "opplist.h"
	#include "tiledat.h"
	#include "overhead.h"
	#include "structure.h"
	#include "renderworld.h"
	#include "worldman.h"
	#include "wcheck.h"
	#include "random.h"
	#include "worlddef.h"
	#include "campaign.h"
	#include "Sound Control.h"
	#include "Interface.h"
	#include "Game Clock.h"
	#include "handle doors.h"
	#include "Map Screen Interface.h"
	#include "GameSettings.h" // added by SANDRO
	#include "Dialogue Control.h" // added by Flugente

#include "GameVersion.h"

DOOR_STATUS		*gpDoorStatus = NULL;
UINT8					gubNumDoorStatus=0;
extern INT8 gbMercIsNewInThisSector[ MAX_NUM_SOLDIERS ];


BOOLEAN InternalIsPerceivedDifferentThanReality( DOOR_STATUS *pDoorStatus );
void InternalUpdateDoorGraphicFromStatus( DOOR_STATUS *pDoorStatus, BOOLEAN fUsePerceivedStatus, BOOLEAN fDirty );
void InternalUpdateDoorsPerceivedValue( DOOR_STATUS *pDoorStatus );
BOOLEAN	InternalSetDoorPerceivedOpenStatus( DOOR_STATUS *pDoorStatus, BOOLEAN fPerceivedOpen );


KEY KeyTable[NUM_KEYS] =
{
	// Item #			Flags		Sector, Date Found
	//
	{KEY_1,				0,			0,			0},
	{KEY_2,				0,			0,			0},
	{KEY_3,				0,			0,			0},
	{KEY_4,				0,			0,			0},
	{KEY_5,				0,			0,			0},
	{KEY_6,				0,			0,			0},
	{KEY_7,				0,			0,			0},
	{KEY_8,				0,			0,			0},
	{KEY_9,				0,			0,			0},
	{KEY_10,			0,			0,			0}

};

//Current number of doors in world.
UINT8 gubNumDoors = 0;

//Current max number of doors.	This is only used by the editor.	When adding doors to the
//world, we may run out of space in the DoorTable, so we will allocate a new array with extra slots,
//then copy everything over again.	gubMaxDoors holds the arrays actual number of slots, even though
//the current number (gubNumDoors) will be <= to it.
UINT8 gubMaxDoors = 0;

LOCK LockTable[NUM_LOCKS] = { 0 };

/*
LOCK LockTable[NUM_LOCKS] =
{
	// Keys that will open the lock				Lock type			Pick diff			Smash diff
	{ { NO_KEY, NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,					0,						0},
	{ { 0,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-25,					-25},
	{ { 1,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-60,					-55},
	{ { 2,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-75,					-80},
	{ { 3,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-35,					-45},
	{ { 4,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-45,					-60},
	{ { 5,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-65,					-90},
	{ { 6,			NO_KEY, NO_KEY, NO_KEY},	LOCK_PADLOCK,				-60,					-70},
	{ { 7,			NO_KEY, NO_KEY, NO_KEY},	LOCK_ELECTRONIC,		-50,					-60},
	{ { 8,			NO_KEY, NO_KEY, NO_KEY},	LOCK_ELECTRONIC,		-75,					-80},
	{ { 9,			NO_KEY, NO_KEY, NO_KEY},	LOCK_CARD,					-50,					-40},
	{ { 10,			NO_KEY, NO_KEY, NO_KEY},	LOCK_CARD,					-85,					-80},
	{ { 11,			NO_KEY, NO_KEY, NO_KEY},	LOCK_REGULAR,				-50,					-50}
};
*/

DOORTRAP DoorTrapTable[NUM_DOOR_TRAPS] =
{
	{ 0 },																						// nothing
	{ DOOR_TRAP_STOPS_ACTION },												// explosion
	{ DOOR_TRAP_STOPS_ACTION | DOOR_TRAP_RECURRING},	// electric
	{ DOOR_TRAP_RECURRING },													// siren
	{ DOOR_TRAP_RECURRING | DOOR_TRAP_SILENT},				// silent alarm
	{ DOOR_TRAP_RECURRING },													// brothel siren
	{ DOOR_TRAP_STOPS_ACTION | DOOR_TRAP_RECURRING},	// super electric
};



//Dynamic array of Doors.	For general game purposes, the doors that are locked and/or trapped
//are permanently saved within the map, and are loaded and allocated when the map is loaded.	Because
//the editor allows more doors to be added, or removed, the actual size of the DoorTable may change.
DOOR * DoorTable = NULL;



BOOLEAN LoadLockTable( void )
{
	UINT32	uiNumBytesRead = 0;
	UINT32	uiBytesToRead;
	CHAR8 *	pFileName = "BINARYDATA\\Locks.bin";
	HWFILE	hFile;

	// Load the Lock Table

	hFile = FileOpen( pFileName, FILE_ACCESS_READ, FALSE );
	if( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to LoadLockTable from file %s", pFileName) );
		return(FALSE);
	}

	uiBytesToRead = sizeof( LOCK ) * NUM_LOCKS_OLD;
	FileRead( hFile, LockTable, uiBytesToRead, &uiNumBytesRead );

	FileClose( hFile );

	if ( uiNumBytesRead != uiBytesToRead )
	{
		return( FALSE );
	}

	return( TRUE );

}


BOOLEAN SoldierHasKey( SOLDIERTYPE *pSoldier, UINT8 ubKeyID )
{
	if ( KeyExistsInKeyRing( pSoldier, ubKeyID, NULL ) || KeyExistsInInventory( pSoldier, ubKeyID ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN KeyExistsInKeyRing( SOLDIERTYPE *pSoldier, UINT8 ubKeyID, UINT8 * pubPos )
{
	// returns the index into the key ring where the key can be found
	UINT8 ubLoop;

	if (!(pSoldier->pKeyRing))
	{
		// no key ring!
		return( FALSE );
	}
	for (ubLoop = 0; ubLoop < NUM_KEYS; ubLoop++)
	{
		if (pSoldier->pKeyRing[ubLoop].ubNumber == 0)
		{
			continue;
		}
		if (pSoldier->pKeyRing[ubLoop].ubKeyID == ubKeyID || (ubKeyID == ANYKEY) )
		{
			// found it!
			if (pubPos)
			{
				*pubPos = ubLoop;
			}
			return( TRUE );
		}
	}
	// exhausted key ring
	return( FALSE );
}

BOOLEAN KeyExistsInInventory( SOLDIERTYPE *pSoldier, UINT8 ubKeyID )
{
	UINT8 invsize = pSoldier->inv.size();
	for (UINT8 ubLoop = 0; ubLoop < invsize; ++ubLoop)
	{
		if (Item[pSoldier->inv[ubLoop].usItem].usItemClass == IC_KEY)
		{
			if ( (pSoldier->inv[ubLoop][0]->data.key.ubKeyID == ubKeyID) || (ubKeyID == ANYKEY) )
			{
				// there's the key we want!
				return( TRUE );
			}
		}
	}
	return( FALSE );
}


BOOLEAN ValidKey( DOOR * pDoor, UINT8 ubKeyID )
{
	return (pDoor->ubLockID == ubKeyID);
}

BOOLEAN DoLockDoor( DOOR * pDoor, UINT8 ubKeyID )
{
	// if the door is unlocked and this is the right key, lock the door and
	// return true, otherwise return false
	if (!(pDoor->fLocked) && ValidKey( pDoor, ubKeyID ))
	{
		pDoor->fLocked = TRUE;
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN DoUnlockDoor( DOOR * pDoor, UINT8 ubKeyID )
{
	// if the door is locked and this is the right key, unlock the door and
	// return true, otherwise return false
	if ( (pDoor->fLocked) && ValidKey( pDoor, ubKeyID ))
	{
		// Play lockpicking
		PlayJA2Sample( ( (UINT8)UNLOCK_DOOR_1 ), RATE_11025, SoundVolume( MIDVOLUME, pDoor->sGridNo ), 1, SoundDir( pDoor->sGridNo ) );

		pDoor->fLocked = FALSE;
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN AttemptToUnlockDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	UINT8 ubLoop;
	UINT8		ubKeyID;

	for( ubLoop = 0; ubLoop < MAX_KEYS_PER_LOCK; ubLoop++)
	{
		ubKeyID = pDoor->ubLockID;
		if (KeyExistsInKeyRing( pSoldier, ubKeyID, NULL ))
		{
			// unlock door and move key to front of key ring!
			DoUnlockDoor( pDoor, ubKeyID );
			return( TRUE );
		}
		else if (KeyExistsInInventory( pSoldier, ubKeyID ))
		{
			// unlock door!
			DoUnlockDoor( pDoor, ubKeyID );
			return( TRUE );
		}
	}

	// drat, couldn't find the key
	PlayJA2Sample( KEY_FAILURE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );

	return( FALSE );
}

BOOLEAN AttemptToLockDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	UINT8 ubLoop;
	UINT8		ubKeyID;

	for( ubLoop = 0; ubLoop < MAX_KEYS_PER_LOCK; ubLoop++)
	{
		ubKeyID = pDoor->ubLockID;
		if (KeyExistsInKeyRing( pSoldier, ubKeyID, NULL ))
		{
			// lock door and move key to front of key ring!
			DoLockDoor( pDoor, ubKeyID );
			return( TRUE );
		}
		else if (KeyExistsInInventory( pSoldier, ubKeyID ))
		{
			// lock door!
			DoLockDoor( pDoor, ubKeyID );
			return( TRUE );
		}
	}
	// drat, couldn't find the key
	return( FALSE );
}


BOOLEAN AttemptToCrowbarLock( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	INT32		iResult;
	INT16		bStress;
	INT8		bSlot;

	bSlot = FindUsableCrowbar( pSoldier );
	if ( bSlot == ITEM_NOT_FOUND )
	{
		// error!
		return( FALSE );
	}

	// generate a noise for thumping on the door
	MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, gpWorldLevelData[pSoldier->sGridNo].ubTerrainID, CROWBAR_DOOR_VOLUME, NOISE_DOOR_SMASHING );

	if ( !pDoor->fLocked )
	{
		// auto success but no XP

		// succeeded! door can never be locked again, so remove from door list...
		RemoveDoorInfoFromTable( pDoor->sGridNo );
		// award experience points?

		// Play lock busted sound
		PlayJA2Sample( ( (UINT8)BREAK_LOCK ), RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		return( TRUE );
	}

	if ( pDoor->ubLockID == LOCK_UNOPENABLE )
	{
		// auto failure!
		return( FALSE );
	}

	// possibly damage crowbar
	bStress = __min( EffectiveStrength( pSoldier, FALSE ), LockTable[pDoor->ubLockID].ubSmashDifficulty + 30 );
	// reduce crowbar status by random % between 0 and 5%
	DamageObj( &(pSoldier->inv[ bSlot ]), (INT8) PreRandom( bStress / 20 ) );

	// did we succeed?

	if ( LockTable[pDoor->ubLockID].ubSmashDifficulty == OPENING_NOT_POSSIBLE )
	{
		// do this to get 'can't do this' messages
		iResult = SkillCheck( pSoldier, OPEN_WITH_CROWBAR, (INT8) ( -100 ) );
		iResult = -100;
	}
	else
	{
		iResult = SkillCheck( pSoldier, OPEN_WITH_CROWBAR, (INT8) ( - (INT8) (LockTable[pDoor->ubLockID].ubSmashDifficulty - pDoor->bLockDamage) ) );
	}

	if (iResult > 0)
	{
		// STR GAIN (20) - Pried open a lock
		StatChange( pSoldier, STRAMT, 20, FALSE );

		// SANDRO - merc records - locks breached
		if ( pSoldier->ubProfile != NO_PROFILE )
			gMercProfiles[ pSoldier->ubProfile ].records.usLocksBreached++;

		// succeeded! door can never be locked again, so remove from door list...
		RemoveDoorInfoFromTable( pDoor->sGridNo );

		// Play lock busted sound
		PlayJA2Sample( ( (UINT8)BREAK_LOCK ), RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		return( TRUE );
	}
	else
	{
		if (iResult > -10)
		{
			ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ LOCK_HAS_BEEN_HIT ] );

			// STR GAIN - Damaged a lock by prying
			StatChange( pSoldier, STRAMT, 5, FALSE );

			// we came close... so do some damage to the lock
			pDoor->bLockDamage += (INT8) (10 + iResult);
		}
		else if ( iResult > -40 && pSoldier->sGridNo != pSoldier->sSkillCheckGridNo )
		{
			// give token point for effort :-)
			StatChange( pSoldier, STRAMT, 1, FALSE );
		}

		return( FALSE );
	}

}


BOOLEAN AttemptToSmashDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	INT32		iResult;

	LOCK * pLock;

	// generate a noise for thumping on the door
	MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, gpWorldLevelData[pSoldier->sGridNo].ubTerrainID, SMASHING_DOOR_VOLUME, NOISE_DOOR_SMASHING );

	if ( !pDoor->fLocked )
	{
		// auto success but no XP

		// succeeded! door can never be locked again, so remove from door list...
		RemoveDoorInfoFromTable( pDoor->sGridNo );
		// award experience points?

		// Play lock busted sound
		PlayJA2Sample( ( (UINT8)BREAK_LOCK ), RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		return( TRUE );
	}

	if ( pDoor->ubLockID == LOCK_UNOPENABLE )
	{
		// auto failure!
		return( FALSE );
	}

	pLock = &(LockTable[pDoor->ubLockID]);

	// did we succeed?
	if ( pLock->ubSmashDifficulty == OPENING_NOT_POSSIBLE )
	{
		// do this to get 'can't do this' messages
		iResult = SkillCheck( pSoldier, SMASH_DOOR_CHECK, (INT8) ( -100 ) );
		iResult = -100;
	}
	else
	{
		iResult = SkillCheck( pSoldier, SMASH_DOOR_CHECK, (INT8) ( - (INT8) (LockTable[pDoor->ubLockID].ubSmashDifficulty - pDoor->bLockDamage) ) );
	}
	if (iResult > 0)
	{
		// STR GAIN (20) - Pried open a lock
		StatChange( pSoldier, STRAMT, 20, FALSE );

		// SANDRO - merc records - locks breached
		if ( pSoldier->ubProfile != NO_PROFILE )
			gMercProfiles[ pSoldier->ubProfile ].records.usLocksBreached++;

		// succeeded! door can never be locked again, so remove from door list...
		RemoveDoorInfoFromTable( pDoor->sGridNo );
		// award experience points?

		// Play lock busted sound
		PlayJA2Sample( ( (UINT8)BREAK_LOCK ), RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		return( TRUE );
	}
	else
	{
		if (iResult > -10)
		{
			ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ LOCK_HAS_BEEN_HIT ] );

			// STR GAIN - Damaged a lock by prying
			StatChange( pSoldier, STRAMT, 5, FALSE );

			// we came close... so do some damage to the lock
			pDoor->bLockDamage += (INT8) (10 + iResult);
		}
		else if ( iResult > -40 && pSoldier->sGridNo != pSoldier->sSkillCheckGridNo )
		{
			// give token point for effort :-)
			StatChange( pSoldier, STRAMT, 1, FALSE );
		}
		return( FALSE );
	}
}

BOOLEAN AttemptToPickLock( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	INT32	iResult;
	INT8 bReason;
	LOCK * pLock;

	if ( pDoor->ubLockID == LOCK_UNOPENABLE )
	{
		// auto failure!
		return( FALSE );
	}

	pLock = &(LockTable[pDoor->ubLockID]);

	// look up the type of lock to see if it is electronic or not
	if (pLock->ubLockType == LOCK_CARD || pLock->ubLockType == LOCK_ELECTRONIC )
	{
		bReason = ELECTRONIC_LOCKPICKING_CHECK;
	}
	else
	{
		bReason = LOCKPICKING_CHECK;
	}

	// Play lockpicking
	// ATE: Moved to animation
	//PlayJA2Sample( ( (UINT8)PICKING_LOCK ), RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

	// See if we measure up to the task.
	// The difficulty is negated here to make it a skill adjustment
	if ( pLock->ubPickDifficulty == OPENING_NOT_POSSIBLE )
	{
		// do this to get 'can't do this' messages
		iResult = SkillCheck( pSoldier, bReason, (INT8) ( -100 ) );
		iResult = -100;
	}
	else
	{
		iResult = SkillCheck( pSoldier, bReason, (INT8) ( - (INT8) (pLock->ubPickDifficulty) ) );
	}
	if (iResult > 0)
	{
	// MECHANICAL GAIN:	Picked open a lock
	StatChange( pSoldier, MECHANAMT, ( UINT16 ) ( pLock->ubPickDifficulty / 5 ), FALSE );

	// DEXTERITY GAIN:	Picked open a lock
	StatChange( pSoldier, DEXTAMT, ( UINT16 ) ( pLock->ubPickDifficulty / 10 ), FALSE );

		// WISDOM GAIN:	Picked open a lock (Snap)
		if ( bReason == ELECTRONIC_LOCKPICKING_CHECK )
		 StatChange( pSoldier, WISDOMAMT, ( UINT16 ) ( pLock->ubPickDifficulty / 10 ), FALSE );
		else
		 StatChange( pSoldier, WISDOMAMT, ( UINT16 ) ( pLock->ubPickDifficulty / 15 ), FALSE );

		// SANDRO - merc records - another lock picked
		if ( pSoldier->ubProfile != NO_PROFILE )
			gMercProfiles[ pSoldier->ubProfile ].records.usLocksPicked++;

		// succeeded!
		pDoor->fLocked = FALSE;
		return( TRUE );
	}
	else
	{
		// NOTE: failures are not rewarded, since you can keep trying indefinitely...

		// check for traps
		return( FALSE );
	}
}

BOOLEAN AttemptToUntrapDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	INT32		iResult;

	// See if we measure up to the task.
	if ( pDoor->ubTrapID == EXPLOSION	)
	{
		iResult = SkillCheck( pSoldier, DISARM_TRAP_CHECK, (INT8) (pDoor->ubTrapLevel * 7) );
	}
	else
	{
		iResult = SkillCheck( pSoldier, DISARM_ELECTRONIC_TRAP_CHECK, (INT8) (pDoor->ubTrapLevel * 7) );
	}

	if (iResult > 0)
	{
		// SANDRO - added some exp for removing traps (why was it not here anyway?)
		// MECHANICAL/EXPLOSIVES GAIN
		if ( pDoor->ubTrapID == EXPLOSION	)
			StatChange( pSoldier, EXPLODEAMT, ( UINT16 ) ( pDoor->ubTrapLevel * 5 ), FALSE );
		else
			StatChange( pSoldier, MECHANAMT, ( UINT16 ) ( pDoor->ubTrapLevel * 5 ), FALSE );

		// DEXTERITY GAIN
		StatChange( pSoldier, DEXTAMT, ( UINT16 ) ( pDoor->ubTrapLevel * 5 ), FALSE );

		// WISDOM GAIN
		StatChange( pSoldier, WISDOMAMT, ( UINT16 ) ( pDoor->ubTrapLevel * 2 ), FALSE );

		// SANDRO - merc records - another lock picked
		if ( pSoldier->ubProfile != NO_PROFILE )
			gMercProfiles[ pSoldier->ubProfile ].records.usTrapsRemoved++;

		// succeeded!
		pDoor->ubTrapLevel = 0;
		pDoor->ubTrapID = NO_TRAP;
		return( TRUE );
	}
	else
	{
		// trap should REALLY go off now!
		return( FALSE );
	}
}

BOOLEAN ExamineDoorForTraps( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	// Check to see if there is a trap or not on this door
	INT16 bDetectLevel;

	if (pDoor->ubTrapID == NO_TRAP)
	{
		// No trap!
		pDoor->bPerceivedTrapped = DOOR_PERCEIVED_UNTRAPPED;
	}
	else
	{
		if (pDoor->bPerceivedTrapped == DOOR_PERCEIVED_TRAPPED)
		{
			return( TRUE );
		}
		else
		{
			bDetectLevel = CalcTrapDetectLevel( pSoldier, TRUE );
			if (bDetectLevel < pDoor->ubTrapLevel)
			{
				pDoor->bPerceivedTrapped = DOOR_PERCEIVED_UNTRAPPED;
			}
			else
			{
				pDoor->bPerceivedTrapped = DOOR_PERCEIVED_TRAPPED;
				// SANDRO - add some exp for trap exposion
				StatChange( pSoldier, WISDOMAMT, ( UINT16 ) ( max(1,(pDoor->ubTrapLevel - 1)) ), FALSE );
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOLEAN HasDoorTrapGoneOff( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	// Check to see if the soldier causes the trap to go off
	INT16 bDetectLevel;

	if (pDoor->ubTrapID != NO_TRAP)
	{
		// one quick check to see if the guy sees the trap ahead of time!
		bDetectLevel = CalcTrapDetectLevel( pSoldier, FALSE );
		if (bDetectLevel < pDoor->ubTrapLevel)
		{
			// trap goes off!
			return( TRUE );
		}
	}
	return( FALSE );
}


void HandleDoorTrap( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	if ( !( DoorTrapTable[ pDoor->ubTrapID ].fFlags & DOOR_TRAP_SILENT )	)
	{
		switch( pDoor->ubTrapID )
		{
			case BROTHEL_SIREN:
				ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ LOCK_TRAP_HAS_GONE_OFF_STR ], pDoorTrapStrings[ SIREN ] );
				break;
			case SUPER_ELECTRIC:
				ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ LOCK_TRAP_HAS_GONE_OFF_STR ], pDoorTrapStrings[ ELECTRIC ] );
				break;
			default:
				ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ LOCK_TRAP_HAS_GONE_OFF_STR ], pDoorTrapStrings[ pDoor->ubTrapID ] );
				break;
		}
	}

	// set trap off
	switch( pDoor->ubTrapID )
	{
		case EXPLOSION:
			// cause damage as a regular hand grenade
			IgniteExplosion( NOBODY, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 25, pSoldier->sGridNo, HAND_GRENADE, 0 );
			break;

 		case SIREN:
			// play siren sound effect but otherwise treat as silent alarm, calling
			// available enemies to this location
			PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, pDoor->sGridNo ), 5, SoundDir( pDoor->sGridNo ) );
		case SILENT_ALARM:
			// Get all available enemies running here
			CallAvailableEnemiesTo( pDoor->sGridNo );
			break;

		case BROTHEL_SIREN:
			PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, pDoor->sGridNo ), 5, SoundDir( pDoor->sGridNo ) );
			CallAvailableKingpinMenTo( pDoor->sGridNo );
			// no one is authorized any more!
			gMercProfiles[ MADAME ].bNPCData = 0;
			break;

		case ELECTRIC:
			// insert electrical sound effect here
			PlayJA2Sample( DOOR_ELECTRICITY, RATE_11025, SoundVolume( MIDVOLUME, pDoor->sGridNo ), 1, SoundDir( pDoor->sGridNo ) );

	 // Set attacker's ID
	 pSoldier->ubAttackerID = pSoldier->ubID;
	 // Increment	being attacked count
	 // pSoldier->bBeingAttackedCount++;
		// gTacticalStatus.ubAttackBusyCount++;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Trap gone off %d", gTacticalStatus.ubAttackBusyCount) );
		DebugAttackBusy( "!!!!!!! Trap gone off\n" );

			pSoldier->SoldierTakeDamage( 0, (UINT16) (10 + PreRandom( 10 )), (UINT16) ((3 + PreRandom( 3 ) * 1000)), TAKE_DAMAGE_ELECTRICITY, NOBODY, pDoor->sGridNo, 0, TRUE );
			break;

		case SUPER_ELECTRIC:
			// insert electrical sound effect here
			PlayJA2Sample( DOOR_ELECTRICITY, RATE_11025, SoundVolume( MIDVOLUME, pDoor->sGridNo ), 1, SoundDir( pDoor->sGridNo ) );

	 // Set attacker's ID
	 pSoldier->ubAttackerID = pSoldier->ubID;
	 // Increment	being attacked count
	 // pSoldier->bBeingAttackedCount++;
		//	gTacticalStatus.ubAttackBusyCount++;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Trap gone off %d", gTacticalStatus.ubAttackBusyCount) );
		DebugAttackBusy( "!!!!!!! Trap gone off\n" );

			pSoldier->SoldierTakeDamage( 0, (UINT16) (20 + PreRandom( 20 )), (UINT16) ((6 + PreRandom( 6 ) * 1000)), TAKE_DAMAGE_ELECTRICITY, NOBODY, pDoor->sGridNo, 0, TRUE );
			break;


		default:
			// no trap
			break;
	}

}

BOOLEAN AttemptToBlowUpLock(SOLDIERTYPE * pSoldier, DOOR * pDoor)
{
	INT32	iResult;
	INT8	bSlot;
	BOOLEAN fSuccess = FALSE;

	bSlot = FindLockBomb(pSoldier);
	if (bSlot == NO_SLOT)
	{
		return fSuccess;
	}

	// sevenfm: remember damage as item will be removed
	UINT16 usDamage = Explosive[Item[pSoldier->inv[bSlot].usItem].ubClassIndex].ubDamage;
	UINT16 usItem = pSoldier->inv[bSlot].usItem;
	UINT8 ubVolume = Explosive[Item[pSoldier->inv[bSlot].usItem].ubClassIndex].ubVolume;

	// Remove the explosive.....
	pSoldier->inv[bSlot].RemoveObjectsFromStack(1);
	DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);

	// Flugente: flat bonus to using door breaching charges
	iResult = SkillCheck(pSoldier, PLANTING_BOMB_CHECK, (INT8)pSoldier->GetBackgroundValue(BG_BONUS_BREACHINGCHARGE));
	if (iResult >= -20)
	{
		// Do explosive graphic....
		ANITILE_PARAMS	AniParams;
		INT32 sGridNo;
		INT16						sX, sY, sZ;

		// Get gridno
		sGridNo = pDoor->sGridNo;

		// Get sX, sy;
		sX = CenterX(sGridNo);
		sY = CenterY(sGridNo);

		// Get Z position, based on orientation....
		sZ = 20;

		AniParams.sGridNo = sGridNo;
		AniParams.ubLevelID = ANI_TOPMOST_LEVEL;
		AniParams.sDelay = (INT16)(100);
		AniParams.sStartFrame = 0;
		AniParams.uiFlags = ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
		AniParams.sX = sX;
		AniParams.sY = sY;
		AniParams.sZ = sZ;

		strcpy(AniParams.zCachedFile, "TILECACHE\\MINIBOOM.STI");
		CreateAnimationTile(&AniParams);
		PlayJA2Sample(SMALL_EXPLODE_1, RATE_11025, SoundVolume((INT8)HIGHVOLUME, sGridNo), 1, SoundDir(sGridNo));

		// possibly damage lock
		UINT16 usExplosiveDamage;

		if (!gGameOptions.fNewTraitSystem)
			usExplosiveDamage = usDamage * 2;
		else if (HAS_SKILL_TRAIT(pSoldier, DEMOLITIONS_NT))
			usExplosiveDamage = usDamage * gSkillTraitValues.ubDEShapedChargeDamageMultiplier;
		else
			usExplosiveDamage = usDamage;

		// sevenfm: catch overflow
		if (pDoor->bLockDamage + usExplosiveDamage > 127)
			pDoor->bLockDamage = 127;
		else
			pDoor->bLockDamage += usExplosiveDamage;

		// sevenfm: only damage lock if opening possible, check actual damage in case of reaching max INT8
		if (LockTable[pDoor->ubLockID].ubSmashDifficulty != OPENING_NOT_POSSIBLE &&
			max(pDoor->bLockDamage, usExplosiveDamage) > LockTable[pDoor->ubLockID].ubSmashDifficulty)
		{
			// succeeded! door can never be locked again, so remove from door list...
			RemoveDoorInfoFromTable(pDoor->sGridNo);
			// award experience points? ... SANDRO - sure!
			StatChange(pSoldier, EXPLODEAMT, (10), FALSE);
			// also add to records - door successfully breached
			gMercProfiles[pSoldier->ubProfile].records.usLocksBreached++;

			fSuccess = TRUE;

			// Flugente: additional dialogue
			AdditionalTacticalCharacterDialogue_CallsLua(pSoldier, ADE_LOCKBOMB, 0);
		}

		// sevenfm: also make noise
		MakeNoise(pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_EXPLOSION);
	}
	else
	{
		// OOPS! ... BOOM!
		IgniteExplosion(NOBODY, pSoldier->sX, pSoldier->sY, (INT16)(gpWorldLevelData[pSoldier->sGridNo].sHeight), pSoldier->sGridNo, usItem, 0);
	}	

	return fSuccess;
}

//dnl ch42 250909
DOOR& DOOR::operator=(const _OLD_DOOR& src)
{
	if((void*)this != (void*)&src)
	{
		sGridNo = src.sGridNo;
		fLocked = src.fLocked;
		ubTrapLevel = src.ubTrapLevel;
		ubTrapID = src.ubTrapID;
		ubLockID = src.ubLockID;
		bPerceivedLocked = src.bPerceivedLocked;
		bPerceivedTrapped = src.bPerceivedTrapped;
		bLockDamage = src.bLockDamage;
	}
	return(*this);
}

BOOLEAN DOOR::Load(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	if(dMajorMapVersion < 7.0)
	{
		_OLD_DOOR OldDoorTable;
		LOADDATA(&OldDoorTable, *hBuffer, sizeof(_OLD_DOOR));
		*this = OldDoorTable;
	}
	else
		LOADDATA(this, *hBuffer, sizeof(DOOR));
	return(TRUE);
}

BOOLEAN DOOR::Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	PTR pData = this;
	UINT32 uiBytesToWrite = sizeof(DOOR);
	_OLD_DOOR OldDoor;
	if(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION)
	{
		memset(&OldDoor, 0, sizeof(_OLD_DOOR));
		OldDoor.sGridNo = sGridNo;
		OldDoor.fLocked = fLocked;
		OldDoor.ubTrapLevel = ubTrapLevel;
		OldDoor.ubTrapID = ubTrapID;
		OldDoor.ubLockID = ubLockID;
		OldDoor.bPerceivedLocked = bPerceivedLocked;
		OldDoor.bPerceivedTrapped = bPerceivedTrapped;
		OldDoor.bLockDamage = bLockDamage;
		pData = &OldDoor;
		uiBytesToWrite = sizeof(_OLD_DOOR);
	}
	UINT32 uiBytesWritten = 0;
	FileWrite(hFile, pData, uiBytesToWrite, &uiBytesWritten);
	if(uiBytesToWrite == uiBytesWritten)
		return(TRUE);
	return(FALSE);
}

void LoadDoorTableFromMap(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	TrashDoorTable();
	LOADDATA(&gubNumDoors, *hBuffer, sizeof(gubNumDoors));
	gubMaxDoors = gubNumDoors;
	DoorTable = (DOOR*)MemAlloc(gubMaxDoors*sizeof(DOOR));
	for(int cnt=0; cnt<gubNumDoors; cnt++)
	{
		DoorTable[cnt].Load(hBuffer, dMajorMapVersion);
		gMapTrn.GetTrnCnt(DoorTable[cnt].sGridNo);//dnl ch44 290909 DOOR translation
		// OK, reset perceived values to nothing...
		DoorTable[cnt].bPerceivedLocked = DOOR_PERCEIVED_UNKNOWN;
		DoorTable[cnt].bPerceivedTrapped = DOOR_PERCEIVED_UNKNOWN;
	}
}

void SaveDoorTableToMap(HWFILE fp, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	INT32 i = 0;
	while(i < gubNumDoors)
	{
		if(!OpenableAtGridNo(DoorTable[i].sGridNo))
			RemoveDoorInfoFromTable(DoorTable[i].sGridNo);
		else
			i++;
	}
	UINT32 uiBytesWritten;
	FileWrite(fp, &gubNumDoors, sizeof(gubNumDoors), &uiBytesWritten);
	for(i=0; i<gubNumDoors; i++)
		DoorTable[i].Save(fp, dMajorMapVersion, ubMinorMapVersion);
}

//The editor adds locks to the world.	If the gridno already exists, then the currently existing door
//information is overwritten.
void AddDoorInfoToTable( DOOR *pDoor )
{
	INT32 i;
	for( i = 0; i < gubNumDoors; i++ )
	{
		if( DoorTable[ i ].sGridNo == pDoor->sGridNo )
		{
			memcpy( &DoorTable[ i ], pDoor, sizeof( DOOR ) );
			return;
		}
	}
	//no existing door found, so add a new one.
	if( gubNumDoors < gubMaxDoors )
	{
		memcpy( &DoorTable[ gubNumDoors ], pDoor, sizeof( DOOR ) );
		gubNumDoors++;
	}
	else
	{ //we need to allocate more memory, so add ten more slots.
		DOOR *NewDoorTable;
		gubMaxDoors += 10;
		//Allocate new table with max+10 doors.
		NewDoorTable = (DOOR*)MemAlloc( sizeof( DOOR ) * gubMaxDoors );
		//Copy contents of existing door table to new door table.
		memcpy( NewDoorTable, DoorTable, sizeof( DOOR ) * gubNumDoors );
		//Deallocate the existing door table (possible to not have one).
		if( DoorTable )
			MemFree( DoorTable );
		//Assign the new door table as the existing door table
		DoorTable = NewDoorTable;
		//Add the new door info to the table.
		memcpy( &DoorTable[ gubNumDoors ], pDoor, sizeof( DOOR ) );
		gubNumDoors++;
	}
}

//When the editor removes a door from the world, this function looks for and removes accompanying door
//information.	If the entry is not the last entry, the last entry is move to it's current slot, to keep
//everything contiguous.
void RemoveDoorInfoFromTable( INT32 iMapIndex )
{
	INT32 i;
	INT32 iNumDoorsToCopy;
	for( i = 0; i < gubNumDoors; i++ )
	{
		if( DoorTable[ i ].sGridNo == iMapIndex )
		{
		iNumDoorsToCopy = gubNumDoors - i - 1;
		if( iNumDoorsToCopy )
		{
			memmove( &DoorTable[ i ], &DoorTable[ i+1 ], sizeof( DOOR ) * iNumDoorsToCopy );
		}
			gubNumDoors--;
			return;
		}
	}
}

//This is the link to see if a door exists at a gridno.
DOOR* FindDoorInfoAtGridNo( INT32 iMapIndex )
{
	INT32 i;
	for( i = 0; i < gubNumDoors; i++ )
	{
		if( DoorTable[ i ].sGridNo == iMapIndex )
			return &DoorTable[ i ];
	}
	return NULL;
}

//Upon world deallocation, the door table needs to be deallocated.	Remember, this function
//resets the values, so make sure you do this before you change gubNumDoors or gubMaxDoors.
void TrashDoorTable()
{
	if( DoorTable )
		MemFree( DoorTable );
	DoorTable = NULL;
	gubNumDoors = 0;
	gubMaxDoors = 0;
}

void UpdateDoorPerceivedValue( DOOR *pDoor )
{
	if ( pDoor->fLocked )
	{
		pDoor->bPerceivedLocked = DOOR_PERCEIVED_LOCKED;
	}
	else if ( !pDoor->fLocked )
	{
		pDoor->bPerceivedLocked = DOOR_PERCEIVED_UNLOCKED;
	}

	if (pDoor->ubTrapID != NO_TRAP)
	{
		pDoor->bPerceivedTrapped = DOOR_PERCEIVED_TRAPPED;
	}
	else
	{
		pDoor->bPerceivedTrapped = DOOR_PERCEIVED_UNTRAPPED;
	}

}





BOOLEAN	SaveDoorTableToDoorTableTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	UINT32	uiNumBytesWritten;
	UINT32	uiSizeToSave=0;
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;

//	return( TRUE );

	uiSizeToSave = gubNumDoors * sizeof( DOOR );


	//Convert the current sector location into a file name
//	GetMapFileName( sSectorX, sSectorY, bSectorZ, zTempName, FALSE );

	//add the 'd' for 'Door' to the front of the map name
//	sprintf( zMapName, "%s\\d_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_DOOR_TABLE_TEMP_FILES_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );

	//if the file already exists, delete it
	if( FileExists( zMapName ) )
	{
		//We are going to be overwriting the file
		if( !FileDelete( zMapName ) )
		{
			return(FALSE);
		}
	}



	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}


	//Save the number of doors
	FileWrite( hFile, &gubNumDoors, sizeof( UINT8 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT8 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}


	//if there are doors to save
	if( uiSizeToSave != 0 )
	{
		//Save the door table
		FileWrite( hFile, DoorTable, uiSizeToSave, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSizeToSave )
		{
			FileClose( hFile );
			return( FALSE );
		}
	}


	//Set the sector flag indicating that there is a Door table temp file present
	SetSectorFlag( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, SF_DOOR_TABLE_TEMP_FILES_EXISTS );

	FileClose( hFile );

	return( TRUE );
}



BOOLEAN LoadDoorTableFromDoorTableTempFile( )
{
	UINT32	uiNumBytesRead;
	HWFILE	hFile;
	CHAR8		zMapName[ 128 ];

//	return( TRUE );


	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'd' for 'Door' to the front of the map name
//	sprintf( zMapName, "%s\\d_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_DOOR_TABLE_TEMP_FILES_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Get rid of the existing door table
	TrashDoorTable();

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}

	//Read in the number of doors
	FileRead( hFile, &gubMaxDoors, sizeof( UINT8 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT8 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	gubNumDoors = gubMaxDoors;

	//if there is no doors to load
	if( gubNumDoors != 0 )
	{
		//Allocate space for the door table
		DoorTable = (DOOR *) MemAlloc( sizeof( DOOR ) * gubMaxDoors );
		if( DoorTable == NULL )
		{
			FileClose( hFile );
			return( FALSE );
		}


		//Read in the number of doors
		FileRead( hFile, DoorTable, sizeof( DOOR ) * gubMaxDoors, &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( DOOR ) * gubMaxDoors )
		{
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );

	return( TRUE );
}



static auto ComplainAboutMissingDoorStructure(const INT32 GridNo) {
#if JA2TESTVERSION
	ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Door structure data at %d was not found", GridNo);
#endif
}

// fOpen is True if the door is open, false if it is closed
BOOLEAN ModifyDoorStatus( INT32 sGridNo, BOOLEAN fOpen, BOOLEAN fPerceivedOpen )
{
	UINT8	ubCnt;
	STRUCTURE * pStructure;
	STRUCTURE * pBaseStructure;

	//Set the gridno for the door

	// Find the base tile for the door structure and use that gridno
	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
	}
	else
	{
		pBaseStructure = NULL;
	}

	if ( pBaseStructure == NULL )
	{
		ComplainAboutMissingDoorStructure(sGridNo);
		return( FALSE );
	}

	//if there is an array
	if( gpDoorStatus )
	{
		//Check to see if the user is adding an existing door
		for( ubCnt=0; ubCnt<gubNumDoorStatus; ubCnt++)
		{
			//if the door is already in the array
			if( gpDoorStatus[ ubCnt ].sGridNo == 	pBaseStructure->sGridNo )
			{
				//set the status
				// ATE: Don't set if set to DONTSET
				if ( fPerceivedOpen != DONTSETDOORSTATUS )
				{
					if( fPerceivedOpen )
						gpDoorStatus[ ubCnt ].ubFlags |= DOOR_PERCEIVED_OPEN;
					else
						gpDoorStatus[ ubCnt ].ubFlags &= ~DOOR_PERCEIVED_OPEN;

					// Turn off perceived not set flag....
						gpDoorStatus[ ubCnt ].ubFlags &= ~DOOR_PERCEIVED_NOTSET;

				}

				if ( fOpen != DONTSETDOORSTATUS )
				{
					if( fOpen )
						gpDoorStatus[ ubCnt ].ubFlags |= DOOR_OPEN;
					else
						gpDoorStatus[ ubCnt ].ubFlags &= ~DOOR_OPEN;
				}

				//Dont add it
				return( TRUE );
			}
		}
	}

	// add a new door status structure

	//if there is an array
	if( gpDoorStatus )
	{
		//Increment the number of doors
		gubNumDoorStatus++;

		//reallocate memory to hold the new door
		gpDoorStatus = (DOOR_STATUS *) MemRealloc( gpDoorStatus, sizeof( DOOR_STATUS ) * gubNumDoorStatus );
		if( gpDoorStatus == NULL )
			return( FALSE );

	}
	else
	{
		//Set the initial number of doors
		gubNumDoorStatus = 1;

		gpDoorStatus = (DOOR_STATUS *) MemAlloc( sizeof( DOOR_STATUS ) );
		if( gpDoorStatus == NULL )
			return( FALSE );
	}

	gpDoorStatus[ gubNumDoorStatus-1 ].sGridNo = pBaseStructure->sGridNo;

	//Init the flags
	gpDoorStatus[ gubNumDoorStatus-1 ].ubFlags = 0;

	//If the door is to be initially open
	if( fOpen )
		gpDoorStatus[ gubNumDoorStatus-1 ].ubFlags |= DOOR_OPEN;

	// IF A NEW DOOR, USE SAME AS ACTUAL
	if ( fPerceivedOpen != DONTSETDOORSTATUS )
	{
		if( fOpen )
			gpDoorStatus[ gubNumDoorStatus-1 ].ubFlags |= DOOR_PERCEIVED_OPEN;
	}
	else
	{
		gpDoorStatus[ gubNumDoorStatus-1 ].ubFlags |= DOOR_PERCEIVED_NOTSET;
	}

	// flag the tile as containing a door status
	gpWorldLevelData[ pBaseStructure->sGridNo ].ubExtFlags[0] |= MAPELEMENT_EXT_DOOR_STATUS_PRESENT;

	return( TRUE );
}

void TrashDoorStatusArray( )
{
	if( gpDoorStatus )
	{
		MemFree( gpDoorStatus );
		gpDoorStatus = NULL;
	}

	gubNumDoorStatus = 0;
}


BOOLEAN	IsDoorOpen( INT32 sGridNo )
{
	UINT8	ubCnt;
	STRUCTURE * pStructure;
	STRUCTURE * pBaseStructure;

	// Find the base tile for the door structure and use that gridno
	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
	}
	else
	{
		pBaseStructure = NULL;
	}

	if ( pBaseStructure == NULL )
	{
		ComplainAboutMissingDoorStructure(sGridNo);
		return( FALSE );
	}

	//if there is an array
	if( gpDoorStatus )
	{
		//Check to see if the user is adding an existing door
		for( ubCnt=0; ubCnt<gubNumDoorStatus; ubCnt++)
		{
			//if this is the door
			if( gpDoorStatus[ ubCnt ].sGridNo == pBaseStructure->sGridNo )
			{
				if( gpDoorStatus[ ubCnt ].ubFlags & DOOR_OPEN )
					return( TRUE );
				else
					return( FALSE );
			}
		}
	}

	#ifdef JA2TESTVERSION
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"WARNING! Failed to find the Door Open Status on Gridno %s", sGridNo );
	#endif

	return( FALSE );
}

// Returns a doors status value, NULL if not found
DOOR_STATUS	*GetDoorStatus( INT32 sGridNo )
{
	UINT8	ubCnt;
	STRUCTURE * pStructure;
	STRUCTURE * pBaseStructure;

	//if there is an array
	if( gpDoorStatus )
	{
		// Find the base tile for the door structure and use that gridno
		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			pBaseStructure = FindBaseStructure( pStructure );
		}
		else
		{
			pBaseStructure = NULL;
		}

		if ( pBaseStructure == NULL )
		{
			ComplainAboutMissingDoorStructure(sGridNo);
			return( NULL );
		}

		//Check to see if the user is adding an existing door
		for( ubCnt=0; ubCnt<gubNumDoorStatus; ubCnt++)
		{
			//if this is the door
			if( gpDoorStatus[ ubCnt ].sGridNo == pBaseStructure->sGridNo )
			{
				return( &( gpDoorStatus[ ubCnt ] ) );
			}
		}
	}

	return( NULL );
}


BOOLEAN AllMercsLookForDoor( INT32 sGridNo, BOOLEAN fUpdateValue )
{
	INT32					cnt, cnt2;
	INT8										bDirs[ 8 ] = { NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST };
	SOLDIERTYPE							*pSoldier;
	DOOR_STATUS							*pDoorStatus;
	INT32											usNewGridNo;

	// Get door
	pDoorStatus = GetDoorStatus( sGridNo );

	if ( pDoorStatus == NULL )
	{
		return( FALSE );
	}

	// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++ )
	{
		// ATE: Ok, lets check for some basic things here!		
		if ( pSoldier->stats.bLife >= OKLIFE && !TileIsOutOfBounds(pSoldier->sGridNo) && pSoldier->bActive && pSoldier->bInSector )
		{
			// and we can trace a line of sight to his x,y coordinates?
			// (taking into account we are definitely aware of this guy now)
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, 0, 0, TRUE, CALC_FROM_ALL_DIRS ) )
			{
				// Update status...
				if ( fUpdateValue )
				{
					InternalUpdateDoorsPerceivedValue( pDoorStatus );
				}
				return( TRUE );
			}

			// Now try other adjacent gridnos...
			for ( cnt2 = 0; cnt2 < NUM_WORLD_DIRECTIONS; ++cnt2 )
			{
				usNewGridNo = NewGridNo( sGridNo, DirectionInc( bDirs[ cnt2 ] ) );
				// and we can trace a line of sight to his x,y coordinates?
				// (taking into account we are definitely aware of this guy now)
				if ( SoldierTo3DLocationLineOfSightTest( pSoldier, usNewGridNo, 0, 0, TRUE, CALC_FROM_ALL_DIRS ) )
				{
					// Update status...
					if ( fUpdateValue )
					{
						InternalUpdateDoorsPerceivedValue( pDoorStatus );
					}
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}


BOOLEAN MercLooksForDoors( SOLDIERTYPE *pSoldier, BOOLEAN fUpdateValue )
{
	INT32					cnt, cnt2;
	INT32 sGridNo;
	DOOR_STATUS							*pDoorStatus;
	INT8										bDirs[ 8 ] = { NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST };
	INT32										 usNewGridNo;



	// Loop through all corpses....
	for ( cnt = 0; cnt < gubNumDoorStatus; cnt++ )
	{
		pDoorStatus = &( gpDoorStatus[ cnt ] );

		if ( !InternalIsPerceivedDifferentThanReality( pDoorStatus ) )
		{
			continue;
		}

		sGridNo = pDoorStatus->sGridNo;
		// and we can trace a line of sight to his x,y coordinates?
		// (taking into account we are definitely aware of this guy now)
		if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, 0, 0, TRUE, CALC_FROM_ALL_DIRS ) )
		{
			// OK, here... update perceived value....
			if ( fUpdateValue )
			{
				InternalUpdateDoorsPerceivedValue( pDoorStatus );

				// Update graphic....
				InternalUpdateDoorGraphicFromStatus( pDoorStatus, TRUE, TRUE );
			}
			return( TRUE );
		}

		// Now try other adjacent gridnos...
		for ( cnt2 = 0; cnt2 < NUM_WORLD_DIRECTIONS; ++cnt2 )
		{
			usNewGridNo = NewGridNo( sGridNo, DirectionInc( bDirs[ cnt2 ] ) );
			// and we can trace a line of sight to his x,y coordinates?
			// (taking into account we are definitely aware of this guy now)
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, usNewGridNo, 0, 0, TRUE, CALC_FROM_ALL_DIRS ) )
			{
				// Update status...
				if ( fUpdateValue )
				{
					InternalUpdateDoorsPerceivedValue( pDoorStatus );

					// Update graphic....
					InternalUpdateDoorGraphicFromStatus( pDoorStatus, TRUE, TRUE );

				}
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

void SyncronizeDoorStatusToStructureData( DOOR_STATUS *pDoorStatus )
{
	STRUCTURE *pStructure, *pBaseStructure;
	LEVELNODE * pNode;
	INT32 sBaseGridNo				 = NOWHERE;

	// First look for a door structure here...
	pStructure = FindStructure( pDoorStatus->sGridNo, STRUCTURE_ANYDOOR );

	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
		sBaseGridNo	= pBaseStructure->sGridNo;
	}
	else
	{
		pBaseStructure = NULL;
	}

	if ( pBaseStructure == NULL )
	{
		ComplainAboutMissingDoorStructure(pDoorStatus->sGridNo);
		return;
	}

	pNode = FindLevelNodeBasedOnStructure( sBaseGridNo, pBaseStructure );
	if (!pNode)
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Could not find levelnode from door structure at %d", pDoorStatus->sGridNo );
#endif
		return;
	}

	// ATE: OK let me explain something here:
	// One of the purposes of this function is to MAKE sure the door status MATCHES
	// the struct data value - if not - change ( REGARDLESS of perceived being used or not... )
	//
	// Check for opened...
	if ( pDoorStatus->ubFlags & DOOR_OPEN )
	{
		// IF closed.....
		if ( !( pStructure->fFlags & STRUCTURE_OPEN ) )
		{
			// Swap!
			SwapStructureForPartner( sBaseGridNo, pBaseStructure );
			RecompileLocalMovementCosts( sBaseGridNo );
		}
	}
	else
	{
		if ( ( pStructure->fFlags & STRUCTURE_OPEN ) )
		{
			// Swap!
			SwapStructureForPartner( sBaseGridNo, pBaseStructure );
			RecompileLocalMovementCosts( sBaseGridNo );
		}
	}
}

void UpdateDoorGraphicsFromStatus( BOOLEAN fUsePerceivedStatus, BOOLEAN fDirty )
{
	INT32					cnt;
	DOOR_STATUS							*pDoorStatus;

	for ( cnt = 0; cnt < gubNumDoorStatus; cnt++ )
	{
		pDoorStatus = &( gpDoorStatus[ cnt ] );

		// ATE: Make sure door status flag and struct info are syncronized....
		SyncronizeDoorStatusToStructureData( pDoorStatus );

		InternalUpdateDoorGraphicFromStatus( pDoorStatus, fUsePerceivedStatus, fDirty );
	}
}

void InternalUpdateDoorGraphicFromStatus( DOOR_STATUS *pDoorStatus, BOOLEAN fUsePerceivedStatus, BOOLEAN fDirty )
{
	STRUCTURE *pStructure, *pBaseStructure;
	INT32			cnt;
	BOOLEAN		fOpenedGraphic = FALSE;
	LEVELNODE * pNode;
	BOOLEAN		fWantToBeOpen	= FALSE;
	BOOLEAN		fDifferent	 = FALSE;
	INT32 sBaseGridNo				 = NOWHERE;


	// OK, look at perceived status and adjust graphic
	// First look for a door structure here...
	pStructure = FindStructure( pDoorStatus->sGridNo, STRUCTURE_ANYDOOR );

	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
		sBaseGridNo	= pBaseStructure->sGridNo;
	}
	else
	{
		pBaseStructure = NULL;
	}

	if ( pBaseStructure == NULL )
	{
		ComplainAboutMissingDoorStructure(pDoorStatus->sGridNo);
		return;
	}

	pNode = FindLevelNodeBasedOnStructure( sBaseGridNo, pBaseStructure );
	if (!pNode)
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Could not find levelnode from door structure at %d", pDoorStatus->sGridNo );
#endif
		return;
	}

	// Get status we want to chenge to.....
	if ( fUsePerceivedStatus )
	{
		if ( pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN )
		{
			fWantToBeOpen = TRUE;
		}
	}
	else
	{
		if ( pDoorStatus->ubFlags & DOOR_OPEN )
		{
			fWantToBeOpen = TRUE;
		}
	}

	// First look for an opened door
	// get what it is now...
	cnt = 0;
	while( gClosedDoorList[ cnt ] != -1 )
	{
		// IF WE ARE A SHADOW TYPE
		if ( pNode->usIndex == gClosedDoorList[ cnt ] )
		{
			fOpenedGraphic = TRUE;
			break;
		}
		cnt++;
	};

	// OK, we either have an opened graphic, in which case we want to switch to the closed, or a closed
	// in which case we want to switch to opened...
	// adjust object' graphic


	// OK, we now need to test these things against the true structure data
	// we may need to only adjust the graphic here....
	if ( fWantToBeOpen && ( pStructure->fFlags & STRUCTURE_OPEN ) )
	{
		BOOLEAN fFound = FALSE;
		// Adjust graphic....

		// Loop through and and find opened graphic for the closed one....
		cnt = 0;
		while( gOpenDoorList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( pNode->usIndex == gOpenDoorList[ cnt ] )
			{
				fFound = TRUE;
				break;
			}
			cnt++;
		};

		// OK, now use opened graphic.
		if ( fFound )
		{
			pNode->usIndex = gClosedDoorList[ cnt ];

			if ( fDirty )
			{
				InvalidateWorldRedundency( );
				SetRenderFlags( RENDER_FLAG_FULL );
			}
		}

		return;
	}

	// If we want to be closed but structure is closed
	if ( !fWantToBeOpen && !( pStructure->fFlags & STRUCTURE_OPEN ) )
	{
		BOOLEAN fFound = FALSE;
		// Adjust graphic....

		// Loop through and and find closed graphic for the opend one....
		cnt = 0;
		while( gClosedDoorList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( pNode->usIndex == gClosedDoorList[ cnt ] )
			{
				fFound = TRUE;
				break;
			}
			cnt++;
		};

		// OK, now use opened graphic.
		if ( fFound )
		{
			pNode->usIndex = gOpenDoorList[ cnt ];

			if ( fDirty )
			{
				InvalidateWorldRedundency( );
				SetRenderFlags( RENDER_FLAG_FULL );
			}
		}

		return;
	}


	if ( fOpenedGraphic && !fWantToBeOpen )
	{
		// Close the beast!
		fDifferent = TRUE;
		pNode->usIndex = gOpenDoorList[ cnt ];
	}
	else if ( !fOpenedGraphic && fWantToBeOpen )
	{
		// Find the closed door graphic and adjust....
		cnt = 0;
		while( gOpenDoorList[ cnt ] != -1 )
		{
			// IF WE ARE A SHADOW TYPE
			if ( pNode->usIndex == gOpenDoorList[ cnt ] )
			{
				// Open the beast!
				fDifferent = TRUE;
				pNode->usIndex = gClosedDoorList[ cnt ];
				break;
			}
			cnt++;
		};
	}

	if ( fDifferent )
	{
		SwapStructureForPartner( sBaseGridNo, pBaseStructure );

		RecompileLocalMovementCosts( sBaseGridNo );

		if ( fDirty )
		{
			InvalidateWorldRedundency( );
			SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
}


BOOLEAN InternalIsPerceivedDifferentThanReality( DOOR_STATUS *pDoorStatus )
{
	if ( ( pDoorStatus->ubFlags & DOOR_PERCEIVED_NOTSET ) )
	{
		return( TRUE );
	}

	// Compare flags....
	if ( ( pDoorStatus->ubFlags & DOOR_OPEN && pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN ) ||
			( !( pDoorStatus->ubFlags & DOOR_OPEN ) && !( pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN ) ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

void InternalUpdateDoorsPerceivedValue( DOOR_STATUS *pDoorStatus )
{
	// OK, look at door, set perceived value the same as actual....
	if ( pDoorStatus->ubFlags & DOOR_OPEN )
	{
		InternalSetDoorPerceivedOpenStatus( pDoorStatus, TRUE );
	}
	else
	{
		InternalSetDoorPerceivedOpenStatus( pDoorStatus, FALSE );
	}
}

BOOLEAN UpdateDoorStatusPerceivedValue( INT32 sGridNo )
{
	DOOR_STATUS	*pDoorStatus = NULL;

	pDoorStatus = GetDoorStatus( sGridNo );
	CHECKF( pDoorStatus != NULL );

	InternalUpdateDoorsPerceivedValue( pDoorStatus );

	return( TRUE );
}


BOOLEAN	IsDoorPerceivedOpen( INT32 sGridNo )
{
	DOOR_STATUS	* pDoorStatus;

	pDoorStatus = GetDoorStatus( sGridNo );

	if (pDoorStatus && pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN)
	{
		return( TRUE );
	}
	else
	{
		#ifdef JA2TESTVERSION
			if (!pDoorStatus)
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"WARNING! Failed to find the Perceived Open Door Status on Gridno %s", sGridNo );
			}
		#endif

		return( FALSE );
	}
}


BOOLEAN	InternalSetDoorPerceivedOpenStatus( DOOR_STATUS *pDoorStatus, BOOLEAN fPerceivedOpen )
{
	if( fPerceivedOpen )
		pDoorStatus->ubFlags |= DOOR_PERCEIVED_OPEN;
	else
		pDoorStatus->ubFlags &= ~DOOR_PERCEIVED_OPEN;

	// Turn off perceived not set flag....
	pDoorStatus->ubFlags &= ~DOOR_PERCEIVED_NOTSET;

	return( TRUE );
}


BOOLEAN	SetDoorPerceivedOpenStatus( INT32 sGridNo, BOOLEAN fPerceivedOpen )
{
	DOOR_STATUS	*pDoorStatus = NULL;

	pDoorStatus = GetDoorStatus( sGridNo );

	CHECKF( pDoorStatus != NULL );

	return( InternalSetDoorPerceivedOpenStatus( pDoorStatus, fPerceivedOpen ) );

}


BOOLEAN	SetDoorOpenStatus( INT32 sGridNo, BOOLEAN fOpen )
{
	DOOR_STATUS * pDoorStatus;

	pDoorStatus = GetDoorStatus( sGridNo );

	if ( pDoorStatus )
	{
		if( fOpen )
		{
			pDoorStatus->ubFlags |= DOOR_OPEN;
		}
		else
		{
			pDoorStatus->ubFlags &= ~DOOR_OPEN;
		}
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}

}


BOOLEAN SaveDoorStatusArrayToDoorStatusTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesWritten;
	UINT8		ubCnt;

	// Turn off any DOOR BUSY flags....
	for( ubCnt=0; ubCnt < gubNumDoorStatus; ubCnt++)
	{
		gpDoorStatus[ ubCnt ].ubFlags &= ( ~DOOR_BUSY );
	}


	//Convert the current sector location into a file name
//	GetMapFileName( sSectorX, sSectorY, bSectorZ, zTempName, FALSE );

	//add the 'm' for 'Modifed Map' to the front of the map name
//	sprintf( zMapName, "%s\\ds_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_DOOR_STATUS_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );


	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}


	//Save the number of elements in the door array
	FileWrite( hFile, &gubNumDoorStatus, sizeof( UINT8 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT8 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}

	//if there is some to save
	if( gubNumDoorStatus != 0 )
	{
		//Save the door array
		FileWrite( hFile, gpDoorStatus, ( sizeof( DOOR_STATUS ) * gubNumDoorStatus ), &uiNumBytesWritten );
		if( uiNumBytesWritten != ( sizeof( DOOR_STATUS ) * gubNumDoorStatus ) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );

	//Set the flag indicating that there is a door status array
	SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_DOOR_STATUS_TEMP_FILE_EXISTS );

	return( TRUE );
}


BOOLEAN LoadDoorStatusArrayFromDoorStatusTempFile()
{
	CHAR8		zMapName[ 128 ];
	HWFILE	hFile;
	UINT32	uiNumBytesRead;
	UINT8 ubLoop;

	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'm' for 'Modifed Map' to the front of the map name
//	sprintf( zMapName, "%s\\ds_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_DOOR_STATUS_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Get rid of the existing door array
	TrashDoorStatusArray( );

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}


	// Load the number of elements in the door status array
	FileRead( hFile, &gubNumDoorStatus, sizeof( UINT8 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT8 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	if( gubNumDoorStatus == 0 )
	{
		FileClose( hFile );
		return( TRUE );
	}


	//Allocate space for the door status array
	gpDoorStatus = (DOOR_STATUS *) MemAlloc( sizeof( DOOR_STATUS ) * gubNumDoorStatus );
	if( gpDoorStatus == NULL )
		AssertMsg( 0, "Error Allocating memory for the gpDoorStatus" );
	memset( gpDoorStatus, 0, sizeof( DOOR_STATUS ) * gubNumDoorStatus );


	// Load the number of elements in the door status array
	FileRead( hFile, gpDoorStatus, ( sizeof( DOOR_STATUS ) * gubNumDoorStatus ), &uiNumBytesRead );
	if( uiNumBytesRead != ( sizeof( DOOR_STATUS ) * gubNumDoorStatus ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	FileClose( hFile );

	// the graphics will be updated later in the loading process.

	// set flags in map for containing a door status
	for (ubLoop = 0; ubLoop < gubNumDoorStatus; ubLoop++)
	{
		gpWorldLevelData[ gpDoorStatus[ ubLoop ].sGridNo ].ubExtFlags[0] |= MAPELEMENT_EXT_DOOR_STATUS_PRESENT;
	}

	UpdateDoorGraphicsFromStatus( TRUE, FALSE );

	return( TRUE );
}


BOOLEAN SaveKeyTableToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten=0;


	// Save the KeyTable
	FileWrite( hFile, KeyTable, sizeof( KEY ) * NUM_KEYS, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( KEY ) * NUM_KEYS )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadKeyTableFromSaveedGameFile( HWFILE hFile, UINT32 uiSaveGameVersion )
{
	UINT32	uiNumBytesRead=0;

	if( uiSaveGameVersion < MORE_LOCKS_AND_KEYS )
	{
		// Load the KeyTable
		FileRead( hFile, KeyTable, sizeof( KEY ) * NUM_KEYS_OLD, &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( KEY ) * NUM_KEYS_OLD )
		{
			return( FALSE );
		}
	}
	else
	{
		// Load the KeyTable
		FileRead( hFile, KeyTable, sizeof( KEY ) * NUM_KEYS, &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( KEY ) * NUM_KEYS )
		{
			return( FALSE );
		}
	}

	return( TRUE );
}



void ExamineDoorsOnEnteringSector( )
{
	INT32					cnt;
	DOOR_STATUS							*pDoorStatus;
	SOLDIERTYPE				*pSoldier;
	BOOLEAN									fOK = FALSE;
	INT8										bTownId;

	// OK, only do this if conditions are met....
	// If this is any omerta tow, don't do it...
	bTownId = GetTownIdForSector( gWorldSectorX, gWorldSectorY );

	if ( bTownId == OMERTA )
	{
		return;
	}

	// Check time...
	if ( ( GetWorldTotalMin( ) - gTacticalStatus.uiTimeSinceLastInTactical ) < 30 )
	{
		return;
	}

	// there is at least one human being in that sector.
	// check for civ
	cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ LAST_TEAM ].bLastID; cnt++ ,pSoldier++ )
	{
		if ( pSoldier->bActive )
		{
			if ( pSoldier->bInSector )
			{
				fOK = TRUE;
				break;
			}
		}
	}

	// Let's do it!
	if ( fOK )
	{
		for ( cnt = 0; cnt < gubNumDoorStatus; cnt++ )
		{
			pDoorStatus = &( gpDoorStatus[ cnt ] );

			// Get status of door....
			if ( pDoorStatus->ubFlags & DOOR_OPEN )
			{
				// If open, close!
				HandleDoorChangeFromGridNo( NULL, pDoorStatus->sGridNo, TRUE );
			}
		}
	}
}

void HandleDoorsChangeWhenEnteringSectorCurrentlyLoaded( )
{
	INT32					cnt;
	DOOR_STATUS							*pDoorStatus;
	SOLDIERTYPE				*pSoldier;
	BOOLEAN									fOK = FALSE;
	INT32										iNumNewMercs = 0;
	INT8										bTownId;

	// OK, only do this if conditions are met....

	// If this is any omerta tow, don't do it...
	bTownId = GetTownIdForSector( gWorldSectorX, gWorldSectorY );

	if ( bTownId == OMERTA )
	{
		return;
	}

	// 1 ) there is at least one human being in that sector.
	// check for civ
	cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;

	// Check time...
	if ( ( GetWorldTotalMin( ) - gTacticalStatus.uiTimeSinceLastInTactical ) < 30 )
	{
		return;
	}

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ LAST_TEAM ].bLastID; cnt++ ,pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->bInSector )
		{
			fOK = TRUE;
			break;
		}
	}

	// Loop through our team now....
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++ ,pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->bInSector && gbMercIsNewInThisSector[ cnt ] )
		{
			iNumNewMercs++;
		}
	}

	// ATE: Only do for newly added mercs....
	if ( iNumNewMercs == 0 )
	{
		return;
	}

	// Let's do it!
	if ( fOK )
	{
		for ( cnt = 0; cnt < gubNumDoorStatus; cnt++ )
		{
			pDoorStatus = &( gpDoorStatus[ cnt ] );

			// Get status of door....
			if ( pDoorStatus->ubFlags & DOOR_OPEN )
			{
				// If open, close!
				gfSetPerceivedDoorState = TRUE;

				HandleDoorChangeFromGridNo( NULL, pDoorStatus->sGridNo, TRUE );

				gfSetPerceivedDoorState = FALSE;

				AllMercsLookForDoor( pDoorStatus->sGridNo, TRUE );

				InternalUpdateDoorGraphicFromStatus( pDoorStatus, TRUE, TRUE );

			}
		}
	}
}


void DropKeysInKeyRing( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel, INT8 bVisible, BOOLEAN fAddToDropList, INT32 iDropListSlot, BOOLEAN fUseUnLoaded )
{
	if (!(pSoldier->pKeyRing))
	{
		// no key ring!
		return;
	}
	UINT8 ubLoop;
	UINT8		ubItem;
	for (ubLoop = 0; ubLoop < NUM_KEYS; ubLoop++)
	{
			ubItem = pSoldier->pKeyRing[ ubLoop ].ubKeyID;

		if ( pSoldier->pKeyRing[ubLoop].ubNumber > 0 )
		{
				CreateKeyObject( &gTempObject, pSoldier->pKeyRing[ubLoop].ubNumber, ubItem );

			// Zero out entry
			pSoldier->pKeyRing[ ubLoop ].ubNumber = 0;
			pSoldier->pKeyRing[ ubLoop ].ubKeyID = INVALID_KEY_NUMBER;

			if ( fAddToDropList )
			{
				AddItemToLeaveIndex( &gTempObject, iDropListSlot );
			}
			else
			{
				if( pSoldier->sSectorX != gWorldSectorX || pSoldier->sSectorY != gWorldSectorY || pSoldier->bSectorZ != gbWorldSectorZ || fUseUnLoaded )
				{
					// Set flag for item...
					AddItemsToUnLoadedSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ , sGridNo, 1, &gTempObject , bLevel, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0, bVisible, FALSE );
				}
				else
				{
					// Add to pool
					AddItemToPool( sGridNo, &gTempObject, bVisible, bLevel, 0, 0 );
				}
			}
		}
	}
}
