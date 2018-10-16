#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
#else
	#include "sgp.h"
	#include "air raid.h"
	#include "game event hook.h"
	#include "game clock.h"
	#include "strategicmap.h"
	#include "PreBattle Interface.h"
	#include "screenids.h"
	#include "jascreens.h"
	#include "random.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "timer control.h"
	#include "dialogue control.h"
	#include "overhead.h"
	#include "message.h"
	#include "isometric utils.h"
	#include "soldier macros.h"
	#include "los.h"
	#include "math.h"
	#include "explosion control.h"
	#include "interface.h"
	#include "music control.h"
	#include "Campaign Types.h"
	#include "GameSettings.h"
	#include "text.h"
	#include "Morale.h"
	#include "Map screen helicopter.h"
	#include "structure wrap.h"
	#include "meanwhile.h"
#endif

#include "GameInitOptionsScreen.h"

#define		SCRIPT_DELAY													10
#define		AIR_RAID_SAY_QUOTE_TIME								3000
#define		AIR_RAID_DIVE_INTERVAL								10000
#define		RAID_DELAY														40
#define		TIME_FROM_DIVE_SOUND_TO_ATTACK_DELAY	8000
#define		TIME_FROM_BOMB_SOUND_TO_ATTACK_DELAY	3000
#define		MOVE_X																5
#define		MOVE_Y																5
#define		STRAFE_DIST														80
#define	BOMB_DIST															150



// BEGIN SERALIZATION
extern INT32		giTimerAirRaidQuote;
extern INT32		giTimerAirRaidDiveStarted;
extern INT32		giTimerAirRaidUpdate;

BOOLEAN				gfInAirRaid					= FALSE;
BOOLEAN				gfAirRaidScheduled			= FALSE;
UINT8				gubAirRaidMode;
UINT32				guiSoundSample;
UINT32				guiRaidLastUpdate;
BOOLEAN				gfFadingRaidIn				= FALSE;
BOOLEAN				gfQuoteSaid					= FALSE;
INT8				gbNumDives					= 0;
INT8				gbMaxDives					= 0;
BOOLEAN				gfFadingRaidOut				= FALSE;
INT16				gsDiveX;
INT16				gsDiveY;
INT16				gsDiveTargetLocation;
UINT8				gubDiveDirection;
INT16				gsNumGridNosMoved;
INT32				giNumTurnsSinceLastDive;
INT32				giNumTurnsSinceDiveStarted;
INT32				giNumGridNosMovedThisTurn;
BOOLEAN				gfAirRaidHasHadTurn			= FALSE;
UINT8				gubBeginTeamTurn			= 0;
BOOLEAN				gfHaveTBBatton				= FALSE;
INT16				gsNotLocatedYet				= FALSE;
extern INT32		giNumFrames; // symbol already defined in video.cpp (jonathanl)

AIR_RAID_DEFINITION	gAirRaidDef;


typedef	struct
{
	BOOLEAN				fInAirRaid;
	BOOLEAN				fAirRaidScheduled;
	UINT8					ubAirRaidMode;
	UINT32				uiSoundSample;
	UINT32				uiRaidLastUpdate;
	BOOLEAN				fFadingRaidIn;
	BOOLEAN				fQuoteSaid;
	INT8					bNumDives;
	INT8					bMaxDives;
	BOOLEAN				fFadingRaidOut;
	INT16					sDiveX;
	INT16					sDiveY;
	INT16					sDiveTargetLocation;
	UINT8					ubDiveDirection;
	INT16					sNumGridNosMoved;
	INT32					iNumTurnsSinceLastDive;
	INT32					iNumTurnsSinceDiveStarted;
	INT32					iNumGridNosMovedThisTurn;
	BOOLEAN				fAirRaidHasHadTurn;
	UINT8					ubBeginTeamTurn;
	BOOLEAN				fHaveTBBatton;
	AIR_RAID_DEFINITION	AirRaidDef;
	INT16					sRaidSoldierID;

	INT16					sNotLocatedYet;
	INT32					iNumFrames;

	INT8					bLevel;
	INT8					bTeam;
	INT8					bSide;
	UINT8					ubAttackerID;
	UINT16				usAttackingWeapon;
	FLOAT					dXPos;
	FLOAT					dYPos;
	INT16					sX;
	INT16					sY;
	INT32 sGridNo;


	UINT8					ubFiller[ 32 ];

} AIR_RAID_SAVE_STRUCT;


// END SERIALIZATION
SOLDIERTYPE		*gpRaidSoldier;


typedef struct
{
	INT8 bDir1;
	INT8 bDir2;

} AIR_RAID_DIR;

typedef struct
{
	INT8 bX;
	INT8 bY;

} AIR_RAID_POS;


AIR_RAID_DIR	ubPerpDirections[ ] =
{
	2, 6,
	3, 7,
	0, 4,
	1, 5,
	2, 6,
	3, 7,
	0, 4,
	1, 5
};

AIR_RAID_POS	ubXYTragetInvFromDirection[ ] =
{
	0, -1,
	1, -1,
	1, 0,
	1, 1,
	0, 1,
	-1, 1,
	-1, 0,
	-1, -1
};



void ScheduleAirRaid( AIR_RAID_DEFINITION *pAirRaidDef )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"ScheduleAirRaid");
	// Make sure only one is cheduled...
	//if ( gfAirRaidScheduled )
	//{
	//	return;
	//}

	// Copy definiaiotn structure into global struct....
	memcpy( &gAirRaidDef, pAirRaidDef, sizeof( AIR_RAID_DEFINITION ) );

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ScheduleAirRaid: raid scheduled for %d",GetWorldMinutesInDay() + pAirRaidDef->ubNumMinsFromCurrentTime ));
	AddSameDayStrategicEvent( EVENT_BEGIN_AIR_RAID, ( GetWorldMinutesInDay() + pAirRaidDef->ubNumMinsFromCurrentTime ), 0 );

	gfAirRaidScheduled = TRUE;
}


BOOLEAN BeginAirRaid( )
{
	INT32 cnt=0;
	BOOLEAN fOK = FALSE;
	SOLDIERTYPE *pSoldier;
	gfQuoteSaid = FALSE;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid");

	// OK, we have been told to start.....

	// First remove scheduled flag...
	gfAirRaidScheduled = FALSE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: UpdateAirspaceControl");
	UpdateAirspaceControl ();

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: WillAirRaidBeStopped");
	if( WillAirRaidBeStopped( gAirRaidDef.sSectorX, gAirRaidDef.sSectorY ) )
	{
		return( FALSE );
	}

	// CHECK IF WE CURRENTLY HAVE THIS SECTOR OPEN....
	/*if (	gAirRaidDef.sSectorX == gWorldSectorX &&
				gAirRaidDef.sSectorY == gWorldSectorY &&
				gAirRaidDef.sSectorZ == gbWorldSectorZ )
	*/
		// Do we have any guys in here...

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginAirRaid: check for mercs: first id = %d, last id = %d ",gTacticalStatus.Team[ gbPlayerNum ].bFirstID, gTacticalStatus.Team[ gbPlayerNum ].bLastID));
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( cnt = 0, pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++)
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginAirRaid: soldier id = %d, active = %d",pSoldier->ubID,pSoldier->bActive));
			if ( pSoldier->bActive	)
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginAirRaid: soldier sectors: x = %d, y = %d, z = %d",pSoldier->sSectorX,pSoldier->sSectorY,pSoldier->bSectorZ ));
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginAirRaid: air raid sectors: x = %d, y = %d, z = %d",gAirRaidDef.sSectorX,gAirRaidDef.sSectorY,gAirRaidDef.sSectorZ ));
				if( pSoldier->sSectorX == gAirRaidDef.sSectorX && pSoldier->sSectorY == gAirRaidDef.sSectorY && pSoldier->bSectorZ == 	gAirRaidDef.sSectorZ && !pSoldier->flags.fBetweenSectors && pSoldier->stats.bLife && pSoldier->bAssignment != IN_TRANSIT )
				{
					fOK = TRUE;
					break;
				}
			}
		}


		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: do we have some guys in this sector?");
		if ( !fOK )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: no we don't");
			return( FALSE );
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: in pre-battle interface?");
		// ( unless we are in prebattle interface, then ignore... )
		if ( gfPreBattleInterfaceActive )
		{
			return( FALSE );
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: change to the right sector");
		ChangeSelectedMapSector( gAirRaidDef.sSectorX, gAirRaidDef.sSectorY, ( INT8 )gAirRaidDef.sSectorZ );
		gfQuoteSaid	= FALSE;

		if (	gAirRaidDef.sSectorX != gWorldSectorX ||
				gAirRaidDef.sSectorY != gWorldSectorY ||
				gAirRaidDef.sSectorZ != gbWorldSectorZ || guiCurrentScreen == MAP_SCREEN )
		{
			// sector not loaded
			// Set flag for handling raid....
			gubAirRaidMode = AIR_RAID_TRYING_TO_START;
			gfQuoteSaid = TRUE;
			SayQuoteFromAnyBodyInThisSector( gAirRaidDef.sSectorX, gAirRaidDef.sSectorY, ( INT8 )gAirRaidDef.sSectorZ, QUOTE_AIR_RAID );
			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_EXIT_MAP_SCREEN, gAirRaidDef.sSectorX, gAirRaidDef.sSectorY, gAirRaidDef.sSectorZ, 0,0 );
		}
		else
		{
			gubAirRaidMode = AIR_RAID_TRYING_TO_START;
			gfQuoteSaid				= FALSE;
		}

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Incoming Air Strike!" );
		LockPauseState ( 22 );
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginAirRaid: setting flags, quote said = %d",gfQuoteSaid));
		// Set flag for handling raid....
		gfInAirRaid = TRUE;
		giNumFrames = 0;



		guiRaidLastUpdate = GetJA2Clock( );


		gbNumDives				= 0;
		gfAirRaidHasHadTurn = FALSE;

		gpRaidSoldier = MercPtrs[ MAX_NUM_SOLDIERS - 1 ];
		gpRaidSoldier->initialize();
		gpRaidSoldier->pathing.bLevel = 0;
		gpRaidSoldier->bTeam = 1;
		gpRaidSoldier->bSide = 1;
		gpRaidSoldier->ubID	= MAX_NUM_SOLDIERS - 1;
		gpRaidSoldier->ubAttackerID = NOBODY;
		gpRaidSoldier->usAttackingWeapon = HK21E;
		CreateItem(HK21E, 100, &gpRaidSoldier->inv[HANDPOS]);

		// Determine how many dives this one will be....
		gbMaxDives				= (INT8)( gAirRaidDef.bIntensity + Random( gAirRaidDef.bIntensity - 1 ) );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Begin Air Raid." );


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"BeginAirRaid: done");
	return(TRUE);
}


INT32 PickLocationNearAnyMercInSector( )
{
	UINT8	ubMercsInSector[ 20 ] = { 0 };
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt=0;

	// Loop through all our guys and randomly say one from someone in our sector
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"PickLocationNearAnyMercInSector");

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickLocationNearAnyMercInSector: total guys = %d", gTacticalStatus.Team[ gbPlayerNum ].bLastID));
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickLocationNearAnyMercInSector: looping %d",cnt));
		if ( OK_INSECTOR_MERC( pTeamSoldier ) )
		{
			ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
			ubNumMercs++;
		}
	}

	// If we are > 0
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickLocationNearAnyMercInSector: number of guys %d",ubNumMercs));
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickLocationNearAnyMercInSector: chosen guy = %d",ubChosenMerc));
		return( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ]->sGridNo );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickLocationNearAnyMercInSector: no target"));
	return( NOWHERE );
}

INT32 PickRandomLocationAtMinSpacesAway( INT32 sGridNo, INT16 sMinValue, INT16 sRandomVar )
{
	INT32 sNewGridNo = NOWHERE;
	INT16 sX, sY, sNewX, sNewY;
	INT32 cnt = 0;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"PickRandomLocationAtMinSpacesAway");

	sX = CenterX( sGridNo );
	sY = CenterY( sGridNo );
	
	while(TileIsOutOfBounds(sNewGridNo))
	{
		sNewX = sX + sMinValue + (INT16)Random( sRandomVar );
		sNewY = sY + sMinValue + (INT16)Random( sRandomVar );

		if ( Random( 2 ) )
		{
			sNewX = -1 * sNewX;
		}

		if ( Random( 2 ) )
		{
			sNewY = -1 * sNewY;
		}

		// Make gridno....
		sNewGridNo = GETWORLDINDEXFROMWORLDCOORDS( sNewY, sNewX );

		// Check if visible on screen....
		if(!GridNoOnVisibleWorldTile(sNewGridNo))//dnl ch56 141009
			sNewGridNo = NOWHERE;
		cnt++;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickRandomLocationAtMinSpacesAway: grid no = %d",sNewGridNo));
	return( sNewGridNo );
}

void TryToStartRaid( )
{
	// OK, check conditions,

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("TryToStartRaid"));
	// Some are:

	// Cannot be in battle ( this is handled by the fact of it begin shceduled in the first place...

	// Cannot be auto-bandaging?
	if ( gTacticalStatus.fAutoBandageMode )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("TryToStartRaid: auto-bandaging"));
		return;
	}

	// Cannot be in conversation...
	if ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("TryToStartRaid: in conversation"));
		return;
	}

	// Cannot be traversing.....

	// Ok, go...
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("TryToStartRaid: go!"));

	gubAirRaidMode = AIR_RAID_START;

}

void AirRaidStart( )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AirRaidStart"));
	// Begin ambient sound....
	guiSoundSample = PlayJA2Sample( S_RAID_AMBIENT, RATE_11025, 0, 10000, MIDDLEPAN );

	gfFadingRaidIn = TRUE;

	// Setup start time....
	RESETTIMECOUNTER( giTimerAirRaidQuote, AIR_RAID_SAY_QUOTE_TIME );

	gubAirRaidMode = AIR_RAID_LOOK_FOR_DIVE;

	// If we are not in combat, change music mode...
	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		SetMusicMode( MUSIC_TACTICAL_BATTLE );
	}
}

void AirRaidLookForDive( )
{
	BOOLEAN	fDoDive = FALSE;
	BOOLEAN	fDoQuote = FALSE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AirRaidLookForDive: quote said? = %d",gfQuoteSaid));

	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if ( !gfQuoteSaid )
		{
			if ( TIMECOUNTERDONE( giTimerAirRaidQuote, AIR_RAID_SAY_QUOTE_TIME ) )
			{
				fDoQuote = TRUE;
			}
		}
	}
	else
	{
		if ( giNumTurnsSinceLastDive > 1 && !gfQuoteSaid )
		{
			fDoQuote = TRUE;
		}
	}

	// OK, check if we should say something....
	if ( fDoQuote )
	{
		gfQuoteSaid = TRUE;

		// Someone in group say quote...
		SayQuoteFromAnyBodyInSector( QUOTE_AIR_RAID );

		// Update timer
		RESETTIMECOUNTER( giTimerAirRaidDiveStarted, AIR_RAID_DIVE_INTERVAL );

		giNumTurnsSinceLastDive = 0;

		// Do morale hit on our guys
		HandleMoraleEvent( NULL, MORALE_AIRSTRIKE, gAirRaidDef.sSectorX, gAirRaidDef.sSectorY, ( INT8 ) gAirRaidDef.sSectorZ );
	}


	// If NOT in combat....
	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// OK, for now on, all we try to do is look for dives to make...
		if ( gfQuoteSaid )
		{
			if ( TIMECOUNTERDONE( giTimerAirRaidDiveStarted, AIR_RAID_DIVE_INTERVAL ) )
			{
				// IN realtime, give a bit more leeway for time....
				//if ( Random( 2 ) )
				{
					fDoDive = TRUE;
				}
			}
		}
	}
	else
	{
		// How many turns have gone by?
		if ( (UINT32)giNumTurnsSinceLastDive > ( Random( 2 ) + 1 ) )
		{
			fDoDive = TRUE;
		}
	}

	if ( fDoDive )
	{
		// If we are are beginning game, only do gun dives..
		
		if ( gAirRaidDef.uiFlags & AIR_RAID_BEGINNING_GAME && !zDiffSetting[gGameOptions.ubDifficultyLevel].bAirRaidLookForDive )
		{
			if ( gbNumDives == 0 )
			{
				gubAirRaidMode = AIR_RAID_BEGIN_DIVE;
			}
			else if ( gbNumDives == 1 )
			{
				gubAirRaidMode = AIR_RAID_BEGIN_BOMBING;
			}
			else
			{
				gubAirRaidMode = AIR_RAID_BEGIN_DIVE;
			}
		}
		else
		{
			// Randomly do dive...
			if ( Random( 2 ) && gAirRaidDef.uiFlags & AIR_RAID_CAN_RANDOMIZE_TEASE_DIVES )
			{
				gubAirRaidMode = AIR_RAID_BEGIN_DIVE;
			}
			else
			{
				gubAirRaidMode = AIR_RAID_BEGIN_BOMBING;
			}
		}
		gbNumDives++;
		return;
	}
	else
	{
		if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			if ( giNumGridNosMovedThisTurn == 0 )
			{
				// Free up attacker...
				ReduceAttackBusyCount( );
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker AIR RAID NO DIVE, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
			}
		}
	}

	// End if we have made desired # of dives...
	if ( gbNumDives == gbMaxDives )
	{
		// Air raid is over....
		gubAirRaidMode = AIR_RAID_START_END;
	}
}


void 	AirRaidStartEnding( )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AirRaidStartEnding"));
	// Fade out sound.....
	gfFadingRaidOut = TRUE;
}


void BeginBombing( )
{
	INT32 sGridNo;
	UINT32	iSoundStartDelay;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginBombing"));

	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// Start diving sound...
		PlayJA2Sample( S_RAID_WHISTLE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
	}

	gubAirRaidMode = AIR_RAID_BOMBING;

	// Pick location...
	gsDiveTargetLocation = PickLocationNearAnyMercInSector( );
	
	if (TileIsOutOfBounds(gsDiveTargetLocation))
	{
		// TODO.WANNE: Hardcoded grid number
		gsDiveTargetLocation = 10234;
	}

	// Get location of aircraft....
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginBombing: pick location, gsDiveTargetLocation = %d",gsDiveTargetLocation));
	sGridNo = PickRandomLocationAtMinSpacesAway( gsDiveTargetLocation , 300, 200 );

	// Save X, y:
	gsDiveX = CenterX( sGridNo );
	gsDiveY = CenterY( sGridNo );

	RESETTIMECOUNTER( giTimerAirRaidUpdate, RAID_DELAY );

	if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		iSoundStartDelay = 0;
	}
	else
	{
		iSoundStartDelay = TIME_FROM_BOMB_SOUND_TO_ATTACK_DELAY;
	}
	RESETTIMECOUNTER( giTimerAirRaidDiveStarted, iSoundStartDelay );


	giNumTurnsSinceDiveStarted = 0;

	// Get direction....
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginBombing: get direction"));
	gubDiveDirection = (INT8)GetDirectionToGridNoFromGridNo( sGridNo, gsDiveTargetLocation );

	gsNumGridNosMoved = 0;
	gsNotLocatedYet		= TRUE;

}



void BeginDive( )
{
	INT32 sGridNo;
	UINT32	iSoundStartDelay;


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginDive"));
	// Start diving sound...
	PlayJA2Sample( S_RAID_DIVE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

	gubAirRaidMode = AIR_RAID_DIVING;

	// Increment attacker bust count....
	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack BEGIN DIVE %d", gTacticalStatus.ubAttackBusyCount) );
	DebugAttackBusy( String("!!!!!!! Starting attack BEGIN DIVE %d", gTacticalStatus.ubAttackBusyCount) );

	// Pick location...
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginDive: pick location"));
	gsDiveTargetLocation = PickLocationNearAnyMercInSector( );
	
	if (TileIsOutOfBounds(gsDiveTargetLocation))
	{
		// TODO.WANNE: Hardcoded grid number
		gsDiveTargetLocation = 10234;
	}

	// Get location of aircraft....
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginDive: pick location at min spaces away, gsDiveTargetLocation = %d",gsDiveTargetLocation));
	sGridNo = PickRandomLocationAtMinSpacesAway( gsDiveTargetLocation, 300, 200 );

	// Save X, y:
	gsDiveX = CenterX( sGridNo );
	gsDiveY = CenterY( sGridNo );

	RESETTIMECOUNTER( giTimerAirRaidUpdate, RAID_DELAY );
	giNumTurnsSinceDiveStarted = 0;

	if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		iSoundStartDelay = 0;
	}
	else
	{
		iSoundStartDelay = TIME_FROM_DIVE_SOUND_TO_ATTACK_DELAY;
	}
	RESETTIMECOUNTER( giTimerAirRaidDiveStarted, iSoundStartDelay );


	// Get direction....
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("BeginDive: get direction"));
	gubDiveDirection = (INT8)GetDirectionToGridNoFromGridNo( sGridNo, gsDiveTargetLocation );

	gsNumGridNosMoved = 0;
	gsNotLocatedYet		= TRUE;

}


void MoveDiveAirplane( FLOAT dAngle )
{
	FLOAT					dDeltaPos;

	// Find delta Movement for X pos
	dDeltaPos = MOVE_X * (FLOAT)sin( dAngle );

	// Find new position
	gsDiveX = (INT16)( gsDiveX + dDeltaPos );

	// Find delta Movement for Y pos
	dDeltaPos = MOVE_X * (FLOAT)cos( dAngle );

	// Find new pos
	gsDiveY = (INT16)( gsDiveY + dDeltaPos );


}


void DoDive(	)
{
	INT16		sRange;
	INT32 sGridNo, sOldGridNo;

	INT16		sTargetX, sTargetY;
	INT16		sStrafeX, sStrafeY;
	FLOAT		dDeltaX, dDeltaY, dAngle, dDeltaXPos, dDeltaYPos;
	INT16		sX, sY;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive");

	// Delay for a specific perion of time to allow sound to Q up...
	if ( TIMECOUNTERDONE( giTimerAirRaidDiveStarted, 0 ) )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: tease or not");
		// OK, rancomly decide to not do this dive...
		if ( gAirRaidDef.uiFlags & AIR_RAID_CAN_RANDOMIZE_TEASE_DIVES )
		{
			//if ( Random( 10 ) == 0 )
			//{
			//	// Finish....
			//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: just teasing");
			//	gubAirRaidMode = AIR_RAID_END_DIVE;
			//	return;
			//}
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: locate target");
		if ( gsNotLocatedYet && !( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			gsNotLocatedYet = FALSE;
			LocateGridNo( gsDiveTargetLocation );
		}

		sOldGridNo = GETWORLDINDEXFROMWORLDCOORDS( gsDiveY, gsDiveX );

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: dive until we are a certain range to target");
		// Dive until we are a certain range to target....
		sRange = PythSpacesAway( sOldGridNo, gsDiveTargetLocation );

		// If sRange
		if ( sRange < 3 )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: end dive");
			// Finish....
			gubAirRaidMode = AIR_RAID_END_DIVE;
			return;
		}

		if ( TIMECOUNTERDONE( giTimerAirRaidUpdate, RAID_DELAY ) )
		{
			RESETTIMECOUNTER( giTimerAirRaidUpdate, RAID_DELAY );

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: move towards target");
			// Move Towards target....
			sTargetX = CenterX( gsDiveTargetLocation );
			sTargetY = CenterY( gsDiveTargetLocation );

			// Determine deltas
			dDeltaX = (FLOAT)( sTargetX - gsDiveX );
			dDeltaY = (FLOAT)( sTargetY - gsDiveY );

			// Determine angle
			dAngle = (FLOAT)atan2( dDeltaX, dDeltaY );

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: move plane");
			MoveDiveAirplane( dAngle );

			gpRaidSoldier->dXPos	= gsDiveX;
			gpRaidSoldier->sX			= gsDiveX;
			gpRaidSoldier->dYPos	= gsDiveY;
			gpRaidSoldier->sY			= gsDiveY;

			// Figure gridno....
			sGridNo = GETWORLDINDEXFROMWORLDCOORDS( gsDiveY, gsDiveX );
			gpRaidSoldier->sGridNo = sGridNo;

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DoDive: figure out grid number, sgridno = %d, soldgridno = %d", sGridNo, sOldGridNo));
			if ( sOldGridNo != sGridNo )
			{
				gsNumGridNosMoved++;

				giNumGridNosMovedThisTurn++;

				// OK, shoot bullets....
				// Get positions of guns...

				// Get target.....
				dDeltaXPos = STRAFE_DIST * (FLOAT)sin( dAngle );
				sStrafeX = (INT16)( gsDiveX + dDeltaXPos );

				// Find delta Movement for Y pos
				dDeltaYPos = STRAFE_DIST * (FLOAT)cos( dAngle );
				sStrafeY = (INT16)( gsDiveY + dDeltaYPos );

				if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
				{
					LocateGridNo( sGridNo );
				}

				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoDive: Fire bullets");
				if ( GridNoOnVisibleWorldTile( GETWORLDINDEXFROMWORLDCOORDS( sStrafeY, sStrafeX ) ) )
				{
					//if ( gsNotLocatedYet && !( gTacticalStatus.uiFlags & INCOMBAT ) )
				//	{
					//	gsNotLocatedYet = FALSE;
				//		LocateGridNo( sGridNo );
				//	}

					//if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// Increase attacker busy...
						//gTacticalStatus.ubAttackBusyCount++;
						//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack AIR RAID ( fire gun ), attack count now %d", gTacticalStatus.ubAttackBusyCount) );

						// INcrement bullet fired...
						// gpRaidSoldier->bBulletsLeft++;
					}

					// For now use first position....


					gpRaidSoldier->ubTargetID = NOBODY;
					FireBulletGivenTarget( gpRaidSoldier, sStrafeX, sStrafeY, 0, gpRaidSoldier->usAttackingWeapon, 10, FALSE, FALSE );
				}

				// Do second one.... ( ll )
				sX = (INT16)( gsDiveX + ( (FLOAT)sin( dAngle + ( PI/2) ) * 40 ) );
				sY = (INT16)( gsDiveY + ( (FLOAT)cos( dAngle + ( PI/2) ) * 40 ) );

				gpRaidSoldier->dXPos	= sX;
				gpRaidSoldier->sX			= sX;
				gpRaidSoldier->dYPos	= sY;
				gpRaidSoldier->sY			= sY;
				gpRaidSoldier->sGridNo = GETWORLDINDEXFROMWORLDCOORDS( sY, sX );

				// Get target.....
				sStrafeX = (INT16)( sX + dDeltaXPos );

				// Find delta Movement for Y pos
				sStrafeY = (INT16)( sY + dDeltaYPos );

				if ( GridNoOnVisibleWorldTile( GETWORLDINDEXFROMWORLDCOORDS( sStrafeY, sStrafeX ) ) )
				{
					//if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// Increase attacker busy...
						//gTacticalStatus.ubAttackBusyCount++;
						//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack AIR RAID ( second one ), attack count now %d", gTacticalStatus.ubAttackBusyCount) );

						// INcrement bullet fired...
						// gpRaidSoldier->bBulletsLeft++;

					}

					// For now use first position....
					FireBulletGivenTarget( gpRaidSoldier, sStrafeX, sStrafeY, 0, gpRaidSoldier->usAttackingWeapon, 10, FALSE, FALSE );
				}

			}

			if ( giNumGridNosMovedThisTurn >= 6 )
			{
				if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
				{
					// Free up attacker...
					ReduceAttackBusyCount( );
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker AIR RAID DIVE DONE FOR THIS TURN, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
				}
			}

		}
	}
}


void DoBombing(	)
{
	INT16		sRange;
	INT32		sGridNo, sOldGridNo, sBombGridNo;

	INT16		sTargetX, sTargetY;
	UINT16	usItem;
	INT16		sStrafeX, sStrafeY;
	FLOAT		dDeltaX, dDeltaY, dAngle, dDeltaXPos, dDeltaYPos;
	BOOLEAN	fLocate = FALSE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoBombing");

	// Delay for a specific perion of time to allow sound to Q up...
	if ( TIMECOUNTERDONE( giTimerAirRaidDiveStarted, 0 ) )
	{
		// OK, rancomly decide to not do this dive...
		if ( gAirRaidDef.uiFlags & AIR_RAID_CAN_RANDOMIZE_TEASE_DIVES )
		{
			//if ( Random( 10 ) == 0 )
			//{
			//	// Finish....
			//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"DoBombing: just teasing");
			//	gubAirRaidMode = AIR_RAID_END_BOMBING;
			//	return;
			//}
		}

		if ( gsNotLocatedYet && !( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			gsNotLocatedYet = FALSE;
			LocateGridNo( gsDiveTargetLocation );
		}

		sOldGridNo = GETWORLDINDEXFROMWORLDCOORDS( gsDiveY, gsDiveX );

		// Dive until we are a certain range to target....
		sRange = PythSpacesAway( sOldGridNo, gsDiveTargetLocation );

		// If sRange
		if ( sRange < 3 )
		{
			// Finish....
			gubAirRaidMode = AIR_RAID_END_BOMBING;
			return;
		}

		if ( TIMECOUNTERDONE( giTimerAirRaidUpdate, RAID_DELAY ) )
		{
			RESETTIMECOUNTER( giTimerAirRaidUpdate, RAID_DELAY );

			// Move Towards target....
			sTargetX = CenterX( gsDiveTargetLocation );
			sTargetY = CenterY( gsDiveTargetLocation );

			// Determine deltas
			dDeltaX = (FLOAT)( sTargetX - gsDiveX );
			dDeltaY = (FLOAT)( sTargetY - gsDiveY );

			// Determine angle
			dAngle = (FLOAT)atan2( dDeltaX, dDeltaY );

			MoveDiveAirplane( dAngle );

			gpRaidSoldier->dXPos	= gsDiveX;
			gpRaidSoldier->sX			= gsDiveX;
			gpRaidSoldier->dYPos	= gsDiveY;
			gpRaidSoldier->sY			= gsDiveY;

			// Figure gridno....
			sGridNo = GETWORLDINDEXFROMWORLDCOORDS( gsDiveY, gsDiveX );
			gpRaidSoldier->sGridNo = sGridNo;

			if ( sOldGridNo != sGridNo )
			{
				// Every once and a while, drop bomb....
				gsNumGridNosMoved++;

				giNumGridNosMovedThisTurn++;

				if ( ( gsNumGridNosMoved % 4 ) == 0 )
				{
					// Get target.....
					dDeltaXPos = BOMB_DIST * (FLOAT)sin( dAngle );
					sStrafeX = (INT16)( gsDiveX + dDeltaXPos );

					// Find delta Movement for Y pos
					dDeltaYPos = BOMB_DIST * (FLOAT)cos( dAngle );
					sStrafeY = (INT16)( gsDiveY + dDeltaYPos );

					if ( GridNoOnVisibleWorldTile( GETWORLDINDEXFROMWORLDCOORDS( sStrafeY, sStrafeX ) ) )
					{
						//if ( gsNotLocatedYet && !( gTacticalStatus.uiFlags & INCOMBAT ) )
						//{
						//	gsNotLocatedYet = FALSE;
						//	LocateGridNo( sGridNo );
						//}


						if ( Random( 2 ) )
						{
							usItem = HAND_GRENADE;
						}
						else
						{
							usItem = RDX;
						}

						// Pick random gridno....
						sBombGridNo = PickRandomLocationAtMinSpacesAway( GETWORLDINDEXFROMWORLDCOORDS( sStrafeY, sStrafeX ), 40, 40 );

						if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
						{
							fLocate = TRUE;
							// Increase attacker busy...
							gTacticalStatus.ubAttackBusyCount++;
							DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack AIR RAID ( bombs away ), attack count now %d", gTacticalStatus.ubAttackBusyCount) );
							DebugAttackBusy( String("!!!!!!! Starting attack AIR RAID ( bombs away ), attack count now %d", gTacticalStatus.ubAttackBusyCount) );
						}

						// Drop bombs...
						InternalIgniteExplosion( NOBODY, CenterX( sBombGridNo ), CenterY( sBombGridNo ), 0, sBombGridNo, usItem, fLocate , (UINT8)IsRoofPresentAtGridNo( sBombGridNo ) );

					}

				}

				if ( giNumGridNosMovedThisTurn >= 6 )
				{
					if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// Free up attacker...
						ReduceAttackBusyCount( );
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker AIR RAID BOMB ATTACK DONE FOR THIS TURN, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
					}
				}

			}
		}
	}
}


void HandleAirRaid( )
{
	INT32 iVol;
	UINT32 uiClock;

	// OK,
	if ( gfInAirRaid )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: air raid mode = %d, quote said = %d",gubAirRaidMode,gfQuoteSaid));

		// are we in meanwhile?

		if ( AreInMeanwhile() )
			return;

		// Are we in TB?
		if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			// Do we have the batton?
			if ( !gfHaveTBBatton )
			{
				// Don;t do anything else!
				return;
			}
		}

		if( WillAirRaidBeStopped( gAirRaidDef.sSectorX, gAirRaidDef.sSectorY ) )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"HandleAirRaid: SAM just taken over");
			gfFadingRaidOut=TRUE;
			gfFadingRaidIn=FALSE;
		}

		// Madd - handle running out of the sector
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: check for mercs: first id = %d, last id = %d ",gTacticalStatus.Team[ gbPlayerNum ].bFirstID, gTacticalStatus.Team[ gbPlayerNum ].bLastID));
		SOLDIERTYPE * pSoldier;
		BOOLEAN fOK = FALSE;
		int cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( cnt = 0, pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++)
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: soldier id = %d, active = %d",pSoldier->ubID,pSoldier->bActive));
			if ( pSoldier->bActive	)
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: soldier sectors: x = %d, y = %d, z = %d",pSoldier->sSectorX,pSoldier->sSectorY,pSoldier->bSectorZ ));
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: air raid sectors: x = %d, y = %d, z = %d",gAirRaidDef.sSectorX,gAirRaidDef.sSectorY,gAirRaidDef.sSectorZ ));
				if( pSoldier->sSectorX == gAirRaidDef.sSectorX && pSoldier->sSectorY == gAirRaidDef.sSectorY && pSoldier->bSectorZ == 	gAirRaidDef.sSectorZ && !pSoldier->flags.fBetweenSectors && pSoldier->stats.bLife && pSoldier->bAssignment != IN_TRANSIT )
				{
					fOK = TRUE;
					break;
				}
			}
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"HandleAirRaid: do we have some guys in this sector?");
		if ( !fOK )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"HandleAirRaid: no we don't");
			gfFadingRaidOut=TRUE;
			gfFadingRaidIn=FALSE;
		}

		uiClock = GetJA2Clock( );

		if ( ( uiClock - guiRaidLastUpdate ) >	SCRIPT_DELAY )
		{
			giNumFrames++;

			guiRaidLastUpdate = uiClock;

			if( gfFadingRaidIn )
			{
				if( guiSoundSample!=NO_SAMPLE )
				{
					if ( ( giNumFrames % 10 ) == 0 )
					{
						iVol=SoundGetVolume( guiSoundSample );
						iVol=__min( HIGHVOLUME, iVol+1);
						SoundSetVolume(guiSoundSample, iVol);
						if(iVol==HIGHVOLUME)
								gfFadingRaidIn=FALSE;
					}
				}
				else
				{
					gfFadingRaidIn=FALSE;
				}
			}
			else if( gfFadingRaidOut )
			{
				if( guiSoundSample!=NO_SAMPLE )
				{
					if ( ( giNumFrames % 10 ) == 0 )
					{
						iVol=SoundGetVolume(guiSoundSample);

						iVol=__max( 0, iVol-1);

						SoundSetVolume(guiSoundSample, iVol);
						if(iVol==0)
						{
							gfFadingRaidOut=FALSE;

							gubAirRaidMode = AIR_RAID_END;
						}
					}
				}
				else
				{
						gfFadingRaidOut=FALSE;
						gubAirRaidMode = AIR_RAID_END;
				}
			}

			switch( gubAirRaidMode )
			{
				case AIR_RAID_TRYING_TO_START:

					TryToStartRaid( );
					break;

				case AIR_RAID_START:

					AirRaidStart( );
					break;

				case AIR_RAID_LOOK_FOR_DIVE:

					AirRaidLookForDive( );
					break;

				case AIR_RAID_START_END:

					AirRaidStartEnding( );
					break;

				case AIR_RAID_END:

					EndAirRaid( );
					break;

				case AIR_RAID_BEGIN_DIVE:

					BeginDive( );
					break;

				case AIR_RAID_DIVING:

					// If in combat, check if we have reached our max...
					if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						if ( giNumGridNosMovedThisTurn < 6 )
						{
							DoDive( );
						}
					}
					else
					{
						DoDive( );
					}
					break;

				case AIR_RAID_END_DIVE:

					giNumTurnsSinceLastDive = 0;
					RESETTIMECOUNTER( giTimerAirRaidDiveStarted, AIR_RAID_DIVE_INTERVAL );

					if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// Free up attacker...
						ReduceAttackBusyCount( );
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker AIR RAID ENDING DIVE, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
					}

					gubAirRaidMode = AIR_RAID_LOOK_FOR_DIVE;
					break;

				case AIR_RAID_END_BOMBING:

					RESETTIMECOUNTER( giTimerAirRaidDiveStarted, AIR_RAID_DIVE_INTERVAL );
					giNumTurnsSinceLastDive = 0;

					if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// Free up attacker...
						ReduceAttackBusyCount( );
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker AIR RAID ENDING DIVE, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
					}

					gubAirRaidMode = AIR_RAID_LOOK_FOR_DIVE;
					break;

				case AIR_RAID_BEGIN_BOMBING:
					BeginBombing( );
					break;

				case AIR_RAID_BOMBING:
					DoBombing( );
					break;
			}
		}

		if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: in combat"));
			// Do we have the batton?
			if ( gfHaveTBBatton )
			{
				// Are we through with attacker busy count?
				if ( gTacticalStatus.ubAttackBusyCount == 0 )
				{
					// Relinquish control....
					gfAirRaidHasHadTurn = TRUE;
					gfHaveTBBatton = FALSE;
					BeginTeamTurn( gubBeginTeamTurn );
				}
			}
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleAirRaid: done"));
	}
}


BOOLEAN InAirRaid( )
{
	return FALSE;
//	return( gfInAirRaid );
}


BOOLEAN HandleAirRaidEndTurn( UINT8 ubTeam )
{
	return( TRUE ); // lal bugfix air raid hangs (remove to enable air raid)

	if ( !gfInAirRaid )
	{
		return( TRUE );
	}

	if ( gfAirRaidHasHadTurn )
	{
		gfAirRaidHasHadTurn = FALSE;
		return( TRUE );
	}

	giNumTurnsSinceLastDive++;
	giNumTurnsSinceDiveStarted++;
	giNumGridNosMovedThisTurn = 0;
	gubBeginTeamTurn = ubTeam;
	gfHaveTBBatton = TRUE;

	// ATE: Even if we have an attacker busy problem.. init to 0 now
	//gTacticalStatus.ubAttackBusyCount = 0;

	// Increment attacker bust count....
	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack AIR RAID, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
	DebugAttackBusy( String("!!!!!!! Starting attack AIR RAID, attack count now %d\n", gTacticalStatus.ubAttackBusyCount) );

	AddTopMessage( AIR_RAID_TURN_MESSAGE, TacticalStr[ AIR_RAID_TURN_STR ] );

	// OK, handle some sound effects, depending on the mode we are in...
	if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
			switch( gubAirRaidMode )
			{
				case AIR_RAID_BOMBING:

					// Start diving sound...
					PlayJA2Sample( S_RAID_TB_BOMB, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					break;

				case AIR_RAID_BEGIN_DIVE:

					PlayJA2Sample( S_RAID_TB_DIVE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					break;
			}
	}

	return( FALSE );
}

BOOLEAN SaveAirRaidInfoToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	AIR_RAID_SAVE_STRUCT sAirRaidSaveStruct;

	// Put all the globals into the save struct
	sAirRaidSaveStruct.fInAirRaid = gfInAirRaid;
	sAirRaidSaveStruct.fAirRaidScheduled = gfAirRaidScheduled;
	sAirRaidSaveStruct.ubAirRaidMode = gubAirRaidMode;
	sAirRaidSaveStruct.uiSoundSample = guiSoundSample;
	sAirRaidSaveStruct.uiRaidLastUpdate = guiRaidLastUpdate;
	sAirRaidSaveStruct.fFadingRaidIn = gfFadingRaidIn;
	sAirRaidSaveStruct.fQuoteSaid = gfQuoteSaid;
	sAirRaidSaveStruct.bNumDives = gbNumDives;
	sAirRaidSaveStruct.bMaxDives = gbMaxDives;
	sAirRaidSaveStruct.fFadingRaidOut = gfFadingRaidOut;
	sAirRaidSaveStruct.sDiveX = gsDiveX;
	sAirRaidSaveStruct.sDiveY = gsDiveY;
	sAirRaidSaveStruct.sDiveTargetLocation = gsDiveTargetLocation;
	sAirRaidSaveStruct.ubDiveDirection = gubDiveDirection;
	sAirRaidSaveStruct.sNumGridNosMoved = gsNumGridNosMoved;
	sAirRaidSaveStruct.iNumTurnsSinceLastDive = giNumTurnsSinceLastDive;
	sAirRaidSaveStruct.iNumTurnsSinceDiveStarted = giNumTurnsSinceDiveStarted;
	sAirRaidSaveStruct.iNumGridNosMovedThisTurn = giNumGridNosMovedThisTurn;
	sAirRaidSaveStruct.fAirRaidHasHadTurn = gfAirRaidHasHadTurn;
	sAirRaidSaveStruct.ubBeginTeamTurn = gubBeginTeamTurn;
	sAirRaidSaveStruct.fHaveTBBatton = gfHaveTBBatton;

	sAirRaidSaveStruct.sNotLocatedYet = gsNotLocatedYet;
	sAirRaidSaveStruct.iNumFrames = giNumFrames;


//	if( gpRaidSoldier )
//	{
//		sAirRaidSaveStruct.bLevel = gpRaidSoldier->pathing.bLevel;
//		sAirRaidSaveStruct.bTeam = gpRaidSoldier->bTeam;
//		sAirRaidSaveStruct.bSide = gpRaidSoldier->bSide;
//		sAirRaidSaveStruct.ubAttackerID = gpRaidSoldier->ubAttackerID;
//		sAirRaidSaveStruct.usAttackingWeapon = gpRaidSoldier->usAttackingWeapon;
//		sAirRaidSaveStruct.dXPos = gpRaidSoldier->dXPos;
//		sAirRaidSaveStruct.dYPos = gpRaidSoldier->dYPos;
//		sAirRaidSaveStruct.sX = gpRaidSoldier->sX;
//		sAirRaidSaveStruct.sY = gpRaidSoldier->sY;
//		sAirRaidSaveStruct.sGridNo = gpRaidSoldier->sGridNo;
//
//		sAirRaidSaveStruct.sRaidSoldierID = MAX_NUM_SOLDIERS - 1;
////		sAirRaidSaveStruct.sRaidSoldierID = gpRaidSoldier->ubID;
//	}
//	else
		sAirRaidSaveStruct.sRaidSoldierID = -1;


	memcpy( &sAirRaidSaveStruct.AirRaidDef, &gAirRaidDef, sizeof( AIR_RAID_DEFINITION	) );


	//Save the Air Raid Save Struct
	FileWrite( hFile, &sAirRaidSaveStruct, sizeof( AIR_RAID_SAVE_STRUCT ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( AIR_RAID_SAVE_STRUCT ) )
	{
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN LoadAirRaidInfoFromSaveGameFile( HWFILE hFile )
{
	AIR_RAID_SAVE_STRUCT sAirRaidSaveStruct;
	UINT32	uiNumBytesRead;


	//Load the number of REAL_OBJECTs in the array
	FileRead( hFile, &sAirRaidSaveStruct, sizeof( AIR_RAID_SAVE_STRUCT ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( AIR_RAID_SAVE_STRUCT ) )
	{
		return( FALSE );
	}


	// Put all the globals into the save struct
	gfInAirRaid = sAirRaidSaveStruct.fInAirRaid;
	gfAirRaidScheduled = sAirRaidSaveStruct.fAirRaidScheduled;
	gubAirRaidMode = sAirRaidSaveStruct.ubAirRaidMode;
	guiSoundSample = sAirRaidSaveStruct.uiSoundSample;
	guiRaidLastUpdate = sAirRaidSaveStruct.uiRaidLastUpdate;
	gfFadingRaidIn = sAirRaidSaveStruct.fFadingRaidIn;
	gfQuoteSaid = sAirRaidSaveStruct.fQuoteSaid;
	gbNumDives = sAirRaidSaveStruct.bNumDives;
	gbMaxDives = sAirRaidSaveStruct.bMaxDives;
	gfFadingRaidOut = sAirRaidSaveStruct.fFadingRaidOut;
	gsDiveX = sAirRaidSaveStruct.sDiveX;
	gsDiveY = sAirRaidSaveStruct.sDiveY;
	gsDiveTargetLocation = sAirRaidSaveStruct.sDiveTargetLocation;
	gubDiveDirection = sAirRaidSaveStruct.ubDiveDirection;
	gsNumGridNosMoved = sAirRaidSaveStruct.sNumGridNosMoved;
	giNumTurnsSinceLastDive = sAirRaidSaveStruct.iNumTurnsSinceLastDive;
	giNumTurnsSinceDiveStarted = sAirRaidSaveStruct.iNumTurnsSinceDiveStarted;
	giNumGridNosMovedThisTurn = sAirRaidSaveStruct.iNumGridNosMovedThisTurn;
	gfAirRaidHasHadTurn = sAirRaidSaveStruct.fAirRaidHasHadTurn;
	gubBeginTeamTurn = sAirRaidSaveStruct.ubBeginTeamTurn;
	gfHaveTBBatton = sAirRaidSaveStruct.fHaveTBBatton;

	gsNotLocatedYet = sAirRaidSaveStruct.sNotLocatedYet;
	giNumFrames = sAirRaidSaveStruct.iNumFrames;


	if( sAirRaidSaveStruct.sRaidSoldierID != -1 )
	{
		gpRaidSoldier = &Menptr[ sAirRaidSaveStruct.sRaidSoldierID ];

		gpRaidSoldier->pathing.bLevel = sAirRaidSaveStruct.bLevel;
		gpRaidSoldier->bTeam = sAirRaidSaveStruct.bTeam;
		gpRaidSoldier->bSide = sAirRaidSaveStruct.bSide;
		gpRaidSoldier->ubAttackerID = sAirRaidSaveStruct.ubAttackerID;
		gpRaidSoldier->usAttackingWeapon = sAirRaidSaveStruct.usAttackingWeapon;
		gpRaidSoldier->dXPos = sAirRaidSaveStruct.dXPos;
		gpRaidSoldier->dYPos = sAirRaidSaveStruct.dYPos;
		gpRaidSoldier->sX = sAirRaidSaveStruct.sX;
		gpRaidSoldier->sY = sAirRaidSaveStruct.sY;
		gpRaidSoldier->sGridNo = sAirRaidSaveStruct.sGridNo;
	}
	else
		gpRaidSoldier = NULL;

	memcpy( &gAirRaidDef, &sAirRaidSaveStruct.AirRaidDef, sizeof( AIR_RAID_DEFINITION	) );

	return( TRUE );
}



void EndAirRaid( )
{
	gfInAirRaid = FALSE;

	// Stop sound
	SoundStop( guiSoundSample );

	// Change music back...
	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		SetMusicMode( MUSIC_TACTICAL_NOTHING );

		if ( !gTacticalStatus.Team[ ENEMY_TEAM ].bTeamActive && !gTacticalStatus.Team[ CREATURE_TEAM ].bTeamActive )
		{
			SOLDIERTYPE * pTeamSoldier;
			INT32	cnt;

			// Loop through all militia and restore them to peaceful status
			cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++,pTeamSoldier++)
			{
				if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
				{
					pTeamSoldier->aiData.bAlertStatus = STATUS_GREEN;
				}
			}
			gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition = FALSE;

			cnt = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
			// Loop through all civs and restore them to peaceful status
			for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; cnt++,pTeamSoldier++)
			{
				if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
				{
					pTeamSoldier->aiData.bAlertStatus = STATUS_GREEN;
				}
			}
			gTacticalStatus.Team[ CIV_TEAM ].bAwareOfOpposition = FALSE;
		}

	}

	// OK, look at flags...
	if ( gAirRaidDef.uiFlags & AIR_RAID_BEGINNING_GAME )
	{	// OK, make enemy appear in Omerta
		// Talk to strategic AI for this...
		//GROUP *pGroup;
		//Create a patrol group originating from sector B9
		//pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_B9, (UINT8)(2 + Random( 2 ) + gGameOptions.ubDifficultyLevel), 0 );
		//Move the patrol group north to attack Omerta
		//AddWaypointToPGroup( pGroup, 9, 1 ); //A9
		//Because we want them to arrive right away, we will toast the arrival event.	The information
		//is already set up though.
		//DeleteStrategicEvent( EVENT_GROUP_ARRIVAL, pGroup->ubGroupID );
		//Simply reinsert the event, but the time is now.
		//AddStrategicEvent( EVENT_GROUP_ARRIVAL, GetWorldTotalMin(), pGroup->ubGroupID );
	}

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Ending Air Raid." );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"The skies have cleared..." );
	UnLockPauseState();
}

// Madd
void CheckForAndSetupAirRaid ()
{
	//INT16 sSectorX;
	//INT16 sSectorY;
	//BOOLEAN fDoAirRaid=FALSE;
	//SECTORINFO * pSector;
	//INT16 sMenInSector;
	//INT16 sMinMenForRaid;
	//SOLDIERTYPE * pSoldier;
	//INT16 cnt=0;

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"CheckForAndSetupAirRaid");

	//switch ( gGameOptions.ubDifficultyLevel )
	//{
	//	case DIF_LEVEL_EASY:
	//		sMinMenForRaid = 10;
	//		break;
	//	case DIF_LEVEL_MEDIUM:
	//		sMinMenForRaid = 8;
	//		break;
	//	case DIF_LEVEL_HARD:
	//		sMinMenForRaid = 6;
	//		break;
	//	case DIF_LEVEL_INSANE:
	//		sMinMenForRaid = 4;
	//		break;
	//}

	//for ( sSectorY = 0;sSectorY < MAP_WORLD_Y;sSectorY++ )
	//{
	//	for ( sSectorX = 0;sSectorX < MAP_WORLD_X;sSectorX++ )
	//	{
	//		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	//		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CheckForAndSetupAirRaid: sector (%d,%d)",sSectorX,sSectorY));

	//		//only attack towns and sam sites
	//		if( StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].bNameId == BLANK_SECTOR && !IsThisSectorASAMSector( sSectorX, sSectorY, 0 ))
	//			continue;

	//		sMenInSector = 0;
	//		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	//		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++ )
	//		{
	//			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CheckForAndSetupAirRaid: soldier id = %d, (x,y,z) = (%d,%d,%d)",pSoldier->bActive,pSoldier->sSectorX,pSoldier->sSectorY,pSoldier->bSectorZ ));
	//			if ( pSoldier->sSectorX == sSectorX && pSoldier->sSectorY == sSectorY && pSoldier->bSectorZ == 0 && pSoldier->bActive )
	//				sMenInSector++;
	//		}
	//
	//		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CheckForAndSetupAirRaid: men %d, min men %d",sMenInSector,sMinMenForRaid));
	//		if ( !WillAirRaidBeStopped ( sSectorX, sSectorY ) && sMenInSector >= sMinMenForRaid && Random(4) < gGameOptions.ubDifficultyLevel )
	//		{
	//			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CheckForAndSetupAirRaid: do a raid"));
	//			fDoAirRaid = TRUE;
	//			break;
	//		}
	//	}
	//	if( fDoAirRaid )
	//		break;
	//}

	//if ( fDoAirRaid )
	//{
	//	AIR_RAID_DEFINITION	AirRaidDef;

	//	AirRaidDef.sSectorX		= sSectorX;
	//	AirRaidDef.sSectorY		= sSectorY;
	//	AirRaidDef.sSectorZ		= 0;
	//	AirRaidDef.bIntensity	= max(1,gGameOptions.ubDifficultyLevel-1) ;
	//	AirRaidDef.uiFlags		=	AIR_RAID_CAN_RANDOMIZE_TEASE_DIVES;
	//	AirRaidDef.ubNumMinsFromCurrentTime	= Random(840) + 1;

	//	ScheduleAirRaid( &AirRaidDef );

		return;
	//}
}

void EnemyCallInAirStrike (INT16 sSectorX, INT16 sSectorY)
{
	//AIR_RAID_DEFINITION	AirRaidDef;

	//AirRaidDef.sSectorX		= sSectorX;
	//AirRaidDef.sSectorY		= sSectorY;
	//AirRaidDef.sSectorZ		= 0;
	//AirRaidDef.bIntensity	= max(1,gGameOptions.ubDifficultyLevel-1);
	//AirRaidDef.uiFlags		=	AIR_RAID_BOMBS_ONLY;
	//AirRaidDef.ubNumMinsFromCurrentTime	= Random(2);

	//if ( AirRaidDef.bIntensity > 0 )
	//	ScheduleAirRaid( &AirRaidDef );

	return;
}
