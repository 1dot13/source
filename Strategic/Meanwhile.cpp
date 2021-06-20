#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "sgp.h"
	#include "meanwhile.h"
	#include "PreBattle Interface.h"
	#include "messageboxscreen.h"
	#include "strategicmap.h"
	#include "fade screen.h"
	#include "screenids.h"
	#include "jascreens.h"
	#include "npc.h"
	#include "game event hook.h"
	#include "game clock.h"
	#include "tactical save.h"
	#include "soldier profile.h"
	#include "overhead.h"
	#include "dialogue control.h"
	#include "gameloop.h"
	#include "npc.h"
	#include "assignments.h"
	#include "strategic.h"
	#include "text.h"
	#include "interface control.h"
	#include "Interface Items.h"
	#include "Map Information.h"
	#include "Map Screen Interface Map.h"
	#include "Map Screen Interface.h"
	#include "Music Control.h"
	#include "Interface.h"
	#include "Game Events.h"
	#include "GameSettings.h"
	#include "Strategic AI.h"
	#include "interface dialogue.h"
	#include "Quests.h"
	#include "Campaign Types.h"
	#include "Squads.h"
	#include "Random.h"
#endif

#include "GameInitOptionsScreen.h"

#define MAX_MEANWHILE_PROFILES	10

INT8 gzMeanwhileStr[][30] =
{
	"End of player's first battle",
	"Drassen Lib. ",
	"Cambria Lib.",
	"Alma Lib.",
	"Grumm lib.",
	"Chitzena Lib.",
	"NW SAM",
	"NE SAM",
	"Central SAM",
	"Flowers",
	"Lost town",
	"Interrogation",
	"Creatures",
	"Kill Chopper",
	"AWOL Madlab",
	"Outskirts Meduna",
	"Balime Lib.",
};


// the snap to grid nos for meanwhile scenes
INT32 gusMeanWhileGridNo[]=
{
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	12248,
	8075,
	12248,
	12248,
	12248,
	12248,
	12248,
};


typedef struct
{
	UINT8		ubProfile;
	INT16		sX;
	INT16		sY;
	INT16		sZ;
	INT32 sGridNo;

} NPC_SAVE_INFO;


// BEGIN SERALIZATION
MEANWHILE_DEFINITION	gCurrentMeanwhileDef;
MEANWHILE_DEFINITION	gMeanwhileDef[NUM_MEANWHILES];
BOOLEAN								gfMeanwhileTryingToStart = FALSE;
BOOLEAN								gfInMeanwhile = FALSE;
// END SERIALIZATION
INT16									gsOldSectorX;
INT16									gsOldSectorY;
INT16									gsOldSectorZ;
INT16									gsOldSelectedSectorX;
INT16									gsOldSelectedSectorY;
INT16									gsOldSelectedSectorZ;

UINT32								guiOldScreen;
NPC_SAVE_INFO					gNPCSaveData[ MAX_MEANWHILE_PROFILES ];
UINT32								guiNumNPCSaves = 0;
BOOLEAN								gfReloadingScreenFromMeanwhile = FALSE;
INT16									gsOldCurInterfacePanel = 0;
BOOLEAN								gfWorldWasLoaded = FALSE;
UINT8									ubCurrentMeanWhileId = 0;

void BeginMeanwhileCallBack( UINT8 bExitValue );
void DoneFadeOutMeanwhile( void );
void DoneFadeInMeanwhile( void );
void DoneFadeOutMeanwhileOnceDone( void );
void DoneFadeInMeanwhileOnceDone( void );
void LocateMeanWhileGrid( void );

UINT32 uiMeanWhileFlags = 0;

// meanwhile flag defines
#define END_OF_PLAYERS_FIRST_BATTLE_FLAG			0x00000001
#define	DRASSEN_LIBERATED_FLAG								0x00000002
#define	CAMBRIA_LIBERATED_FLAG								0x00000004
#define	ALMA_LIBERATED_FLAG										0x00000008
#define	GRUMM_LIBERATED_FLAG									0x00000010
#define	CHITZENA_LIBERATED_FLAG								0x00000020
#define	NW_SAM_FLAG														0x00000040
#define	NE_SAM_FLAG														0x00000080
#define	CENTRAL_SAM_FLAG											0x00000100
#define	FLOWERS_FLAG													0x00000200
#define	LOST_TOWN_FLAG												0x00000400
#define	CREATURES_FLAG												0x00000800
#define	KILL_CHOPPER_FLAG											0x00001000
#define	AWOL_SCIENTIST_FLAG										0x00002000
#define	OUTSKIRTS_MEDUNA_FLAG									0x00004000
#define INTERROGATION_FLAG										0x00008000
#define BALIME_LIBERATED_FLAG									0x00010000

extern void InternalLocateGridNo( INT32 sGridNo, BOOLEAN fForce );

void ProcessImplicationsOfMeanwhile( void );

// set flag for this event
void SetMeanWhileFlag( UINT8 ubMeanwhileID )
{
	switch( ubMeanwhileID )
	{
		case END_OF_PLAYERS_FIRST_BATTLE:
			uiMeanWhileFlags |= END_OF_PLAYERS_FIRST_BATTLE_FLAG;
			break;
		case DRASSEN_LIBERATED:
			uiMeanWhileFlags |= DRASSEN_LIBERATED_FLAG;
			break;
		case CAMBRIA_LIBERATED:
			uiMeanWhileFlags |= CAMBRIA_LIBERATED_FLAG;
			break;
		case ALMA_LIBERATED:
			uiMeanWhileFlags |= ALMA_LIBERATED_FLAG;
			break;
		case GRUMM_LIBERATED:
			uiMeanWhileFlags |= GRUMM_LIBERATED_FLAG;
			break;
		case CHITZENA_LIBERATED:
			uiMeanWhileFlags |= CHITZENA_LIBERATED_FLAG;
			break;
		case BALIME_LIBERATED:
			uiMeanWhileFlags |= BALIME_LIBERATED_FLAG;
			break;
		case NW_SAM:
			uiMeanWhileFlags |= NW_SAM_FLAG;
			break;
		case NE_SAM:
			uiMeanWhileFlags |= NE_SAM_FLAG;
			break;
		case CENTRAL_SAM:
			uiMeanWhileFlags |= CENTRAL_SAM_FLAG;
			break;
		case FLOWERS:
			uiMeanWhileFlags |= FLOWERS_FLAG;
			break;
		case LOST_TOWN:
			uiMeanWhileFlags |= LOST_TOWN_FLAG;
			break;
		case CREATURES:
			uiMeanWhileFlags |= CREATURES_FLAG;
			break;
		case KILL_CHOPPER:
			uiMeanWhileFlags |= KILL_CHOPPER_FLAG;
			break;
		case AWOL_SCIENTIST:
			uiMeanWhileFlags |= AWOL_SCIENTIST_FLAG;
			break;
		case OUTSKIRTS_MEDUNA:
			uiMeanWhileFlags |= OUTSKIRTS_MEDUNA_FLAG;
			break;
		case INTERROGATION:
			uiMeanWhileFlags |= INTERROGATION_FLAG;
			break;
	}
}

// is this flag set?
BOOLEAN GetMeanWhileFlag( UINT8 ubMeanwhileID )
{
	UINT32 uiTrue = FALSE;
	switch( ubMeanwhileID )
	{
		case END_OF_PLAYERS_FIRST_BATTLE:
			uiTrue = ( uiMeanWhileFlags & END_OF_PLAYERS_FIRST_BATTLE_FLAG );
			break;
		case DRASSEN_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & DRASSEN_LIBERATED_FLAG );
			break;
		case CAMBRIA_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & CAMBRIA_LIBERATED_FLAG );
			break;
		case ALMA_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & ALMA_LIBERATED_FLAG );
			break;
		case GRUMM_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & GRUMM_LIBERATED_FLAG );
			break;
		case CHITZENA_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & CHITZENA_LIBERATED_FLAG );
			break;
		case BALIME_LIBERATED:
			uiTrue = ( uiMeanWhileFlags & BALIME_LIBERATED_FLAG );
			break;
		case NW_SAM:
			uiTrue = ( uiMeanWhileFlags & NW_SAM_FLAG );
			break;
		case NE_SAM:
			uiTrue = ( uiMeanWhileFlags & NE_SAM_FLAG );
			break;
		case CENTRAL_SAM:
			uiTrue = ( uiMeanWhileFlags & CENTRAL_SAM_FLAG );
			break;
		case FLOWERS:
			uiTrue = ( uiMeanWhileFlags & FLOWERS_FLAG );
			break;
		case LOST_TOWN:
			uiTrue = ( uiMeanWhileFlags & LOST_TOWN_FLAG );
			break;
		case CREATURES:
			uiTrue = ( uiMeanWhileFlags & CREATURES_FLAG );
			break;
		case KILL_CHOPPER:
			uiTrue = ( uiMeanWhileFlags & KILL_CHOPPER_FLAG );
			break;
		case AWOL_SCIENTIST:
			uiTrue = ( uiMeanWhileFlags & AWOL_SCIENTIST_FLAG );
			break;
		case OUTSKIRTS_MEDUNA:
			uiTrue = ( uiMeanWhileFlags & OUTSKIRTS_MEDUNA_FLAG );
			break;
		case INTERROGATION:
			uiTrue = ( uiMeanWhileFlags & INTERROGATION_FLAG );
			break;
	}

	if( uiTrue )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


INT32 GetFreeNPCSave( void )
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumNPCSaves; uiCount++)
	{
		if(( gNPCSaveData[uiCount].ubProfile == NO_PROFILE ) )
			return( (INT32)uiCount );
	}

	if( guiNumNPCSaves < MAX_MEANWHILE_PROFILES )
		return( (INT32) guiNumNPCSaves++ );

	return( -1 );
}

void RecountNPCSaves( void )
{
	INT32 uiCount;

	for(uiCount=guiNumNPCSaves-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gNPCSaveData[uiCount].ubProfile != NO_PROFILE ) )
		{
			guiNumNPCSaves=(UINT32)(uiCount+1);
			break;
		}
	}
}



void ScheduleMeanwhileEvent( MEANWHILE_DEFINITION *pMeanwhileDef, UINT32 uiTime )
{
	// event scheduled to happen before, ignore
	if( GetMeanWhileFlag( pMeanwhileDef->ubMeanwhileID )== TRUE )
	{
		return;
	}

	// set the meanwhile flag for this event
	SetMeanWhileFlag( pMeanwhileDef->ubMeanwhileID );

	// set the id value
	ubCurrentMeanWhileId = pMeanwhileDef->ubMeanwhileID;

	// Copy definiaiotn structure into position in global array....
	memcpy( &(gMeanwhileDef[pMeanwhileDef->ubMeanwhileID]), pMeanwhileDef, sizeof( MEANWHILE_DEFINITION ) );

	// A meanwhile.. poor elliot!
	// increment his slapped count...

	// We need to do it here 'cause they may skip it...
	if ( gMercProfiles[ ELLIOT ].bNPCData != 17 )
	{
	gMercProfiles[ ELLIOT ].bNPCData++;
	}

	// enable/disable meanwhile cutscene
	if( gModSettings.AllMeanwhileCutscene )
		AddStrategicEvent( EVENT_MEANWHILE, uiTime, pMeanwhileDef->ubMeanwhileID );
	else
	{// simulate that we saw cutscene and handle implications right away
		memcpy( &gCurrentMeanwhileDef, &(gMeanwhileDef[pMeanwhileDef->ubMeanwhileID]), sizeof( MEANWHILE_DEFINITION ) );
		ProcessImplicationsOfMeanwhile();
	}
}


BOOLEAN BeginMeanwhile( UINT8 ubMeanwhileID )
{
	INT32 cnt;

	// copy meanwhile data from array to structure for current
	memcpy( &gCurrentMeanwhileDef, &(gMeanwhileDef[ubMeanwhileID]), sizeof( MEANWHILE_DEFINITION ) );

	gfMeanwhileTryingToStart = TRUE;
	PauseGame();
	// prevent anyone from messing with the pause!
	LockPauseState( 6 );

	// Set NO_PROFILE info....
	for ( cnt = 0; cnt < MAX_MEANWHILE_PROFILES; cnt++ )
	{
		gNPCSaveData[ cnt ].ubProfile = NO_PROFILE;
	}

	return( TRUE );
}


void BringupMeanwhileBox( )
{
	CHAR16 zStr[256];

#ifdef JA2TESTVERSION
	swprintf( zStr, L"Meanwhile..... ( %S : Remember to make sure towns are controlled if required by script )", gzMeanwhileStr[ gCurrentMeanwhileDef.ubMeanwhileID ] );
#else
	swprintf( zStr, L"%s.....", pMessageStrings[ MSG_MEANWHILE ] );
#endif

#ifdef JA2TESTVERSION
	if ( gCurrentMeanwhileDef.ubMeanwhileID != INTERROGATION )
#else
	if ( gCurrentMeanwhileDef.ubMeanwhileID != INTERROGATION && MeanwhileSceneSeen( gCurrentMeanwhileDef.ubMeanwhileID ) )
#endif
	{
		DoMessageBox( MSG_BOX_BASIC_STYLE, zStr, guiCurrentScreen, MSG_BOX_FLAG_OKSKIP, BeginMeanwhileCallBack, NULL );
	}
	else
	{
		//DoMessageBox( MSG_BOX_BASIC_STYLE, zStr, guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_OK, BeginMeanwhileCallBack, NULL );
		DoMessageBox( MSG_BOX_BASIC_STYLE, zStr, guiCurrentScreen, MSG_BOX_FLAG_OKSKIP, BeginMeanwhileCallBack, NULL );
	}
}

void CheckForMeanwhileOKStart( )
{
	if ( gfMeanwhileTryingToStart )
	{
		// Are we in prebattle interface?
		if ( gfPreBattleInterfaceActive )
		{
			return;
		}

		if ( !InterfaceOKForMeanwhilePopup() )
		{
			return;
		}

		if ( !DialogueQueueIsEmptyOrSomebodyTalkingNow( ) )
		{
			return;
		}

		//shadooow: prevent the interrogation meanwhile to start if the mercs were fired from team
		if (gCurrentMeanwhileDef.ubMeanwhileID == INTERROGATION)
		{
			SOLDIERTYPE *pSoldier;
			BOOLEAN fFoundSoldierToInterrogate = FALSE;
			UINT32 uiCount = 0;
			for (pSoldier = MercPtrs[gCharactersList[uiCount].usSolID]; gCharactersList[uiCount].fValid; uiCount++, pSoldier++)
			{
				if (pSoldier->sSectorX == gModSettings.ubMeanwhileInterrogatePOWSectorX && pSoldier->sSectorY == gModSettings.ubMeanwhileInterrogatePOWSectorY &&
					pSoldier->bSectorZ == 0 && pSoldier->bAssignment == ASSIGNMENT_POW && pSoldier->stats.bLife > 0 && gMercProfiles[pSoldier->ubProfile].bMercStatus != MERC_FIRED_AS_A_POW)
				{
					fFoundSoldierToInterrogate = TRUE;
					break;
				}
			}

			if (!fFoundSoldierToInterrogate)
			{
				gfMeanwhileTryingToStart = FALSE;
				ProcessImplicationsOfMeanwhile();
				UnLockPauseState();
				UnPauseGame();
				return;
			}
		}

		gfMeanwhileTryingToStart = FALSE;

		guiOldScreen = guiCurrentScreen;

		if ( guiCurrentScreen == GAME_SCREEN )
		{
			LeaveTacticalScreen( GAME_SCREEN );
		}

		// We need to make sure we have no item - at least in tactical
		// In mapscreen, time is paused when manipulating items...
		CancelItemPointer( );

		BringupMeanwhileBox( );
	}
}

void StartMeanwhile( )
{
	INT32 iIndex;

	// OK, save old position...
	if ( gfWorldLoaded )
	{
		gsOldSectorX = gWorldSectorX;
		gsOldSectorY = gWorldSectorY;
		gsOldSectorZ = gbWorldSectorZ;
	}

	gsOldSelectedSectorX = sSelMapX;
	gsOldSelectedSectorY = sSelMapY;
	gsOldSelectedSectorZ = (INT16) iCurrentMapSectorZ;

	gfInMeanwhile = TRUE;

	// ATE: Change music before load
	SetMusicMode( MUSIC_MAIN_MENU );


	gfWorldWasLoaded = gfWorldLoaded;

	// OK, we have been told to start.....
	SetCurrentInterfacePanel( (UINT8)TEAM_PANEL );

	// Setup NPC locations, depending on meanwhile type...
	switch( gCurrentMeanwhileDef.ubMeanwhileID )
	{
		case	END_OF_PLAYERS_FIRST_BATTLE:
		case	DRASSEN_LIBERATED:
		case	CAMBRIA_LIBERATED:
		case	ALMA_LIBERATED:
		case	GRUMM_LIBERATED:
		case	CHITZENA_LIBERATED:
		case	BALIME_LIBERATED:
		case	NW_SAM:
		case	NE_SAM:
		case	CENTRAL_SAM:
		case	FLOWERS:
		case	LOST_TOWN:
		case	CREATURES:
		case	KILL_CHOPPER:
		case	AWOL_SCIENTIST:
		case	OUTSKIRTS_MEDUNA:

				// SAVE QUEEN!
				iIndex = GetFreeNPCSave( );
				if ( iIndex != -1 )
				{
					gNPCSaveData[ iIndex ].ubProfile = QUEEN;
					gNPCSaveData[ iIndex ].sX				= gMercProfiles[ QUEEN ].sSectorX;
					gNPCSaveData[ iIndex ].sY				= gMercProfiles[ QUEEN ].sSectorY;
					gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ QUEEN ].bSectorZ;
					gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ QUEEN ].sGridNo;

					// Force reload of NPC files...
					ReloadQuoteFile( QUEEN );

					ChangeNpcToDifferentSector( QUEEN, gModSettings.ubMeanwhilePalaceSectorX, gModSettings.ubMeanwhilePalaceSectorY, 0 ); //3, 16
				}

				// SAVE MESSANGER!
				iIndex = GetFreeNPCSave( );
				if ( iIndex != -1 )
				{
					gNPCSaveData[ iIndex ].ubProfile = ELLIOT;
					gNPCSaveData[ iIndex ].sX				= gMercProfiles[ ELLIOT ].sSectorX;
					gNPCSaveData[ iIndex ].sY				= gMercProfiles[ ELLIOT ].sSectorY;
					gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ ELLIOT ].bSectorZ;
					gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ ELLIOT ].sGridNo;

					// Force reload of NPC files...
					ReloadQuoteFile( ELLIOT );

					ChangeNpcToDifferentSector( ELLIOT, gModSettings.ubMeanwhilePalaceSectorX, gModSettings.ubMeanwhilePalaceSectorY, 0 );
				}

				if ( gCurrentMeanwhileDef.ubMeanwhileID == OUTSKIRTS_MEDUNA )
				{
					// SAVE JOE!
					iIndex = GetFreeNPCSave( );
					if ( iIndex != -1 )
					{
						gNPCSaveData[ iIndex ].ubProfile = JOE;
						gNPCSaveData[ iIndex ].sX				= gMercProfiles[ JOE ].sSectorX;
						gNPCSaveData[ iIndex ].sY				= gMercProfiles[ JOE ].sSectorY;
						gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ JOE ].bSectorZ;
						gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ JOE ].sGridNo;

						// Force reload of NPC files...
						ReloadQuoteFile( JOE );

						ChangeNpcToDifferentSector( JOE, gModSettings.ubMeanwhilePalaceSectorX, gModSettings.ubMeanwhilePalaceSectorY, 0 );
					}
				}

			break;


		case	INTERROGATION:

				// SAVE QUEEN!
				iIndex = GetFreeNPCSave( );
				if ( iIndex != -1 )
				{
					gNPCSaveData[ iIndex ].ubProfile = QUEEN;
					gNPCSaveData[ iIndex ].sX				= gMercProfiles[ QUEEN ].sSectorX;
					gNPCSaveData[ iIndex ].sY				= gMercProfiles[ QUEEN ].sSectorY;
					gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ QUEEN ].bSectorZ;
					gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ QUEEN ].sGridNo;

					// Force reload of NPC files...
					ReloadQuoteFile( QUEEN );

					ChangeNpcToDifferentSector( QUEEN, gModSettings.ubMeanwhileInterrogatePOWSectorX, gModSettings.ubMeanwhileInterrogatePOWSectorY, 0 ); // 7, 14
				}

				// SAVE MESSANGER!
				iIndex = GetFreeNPCSave( );
				if ( iIndex != -1 )
				{
					gNPCSaveData[ iIndex ].ubProfile = ELLIOT;
					gNPCSaveData[ iIndex ].sX				= gMercProfiles[ ELLIOT ].sSectorX;
					gNPCSaveData[ iIndex ].sY				= gMercProfiles[ ELLIOT ].sSectorY;
					gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ ELLIOT ].bSectorZ;
					gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ ELLIOT ].sGridNo;

					// Force reload of NPC files...
					ReloadQuoteFile( ELLIOT );

					ChangeNpcToDifferentSector( ELLIOT, gModSettings.ubMeanwhileInterrogatePOWSectorX, gModSettings.ubMeanwhileInterrogatePOWSectorY, 0 );
				}

				// SAVE JOE!
				iIndex = GetFreeNPCSave( );
				if ( iIndex != -1 )
				{
					gNPCSaveData[ iIndex ].ubProfile = JOE;
					gNPCSaveData[ iIndex ].sX				= gMercProfiles[ JOE ].sSectorX;
					gNPCSaveData[ iIndex ].sY				= gMercProfiles[ JOE ].sSectorY;
					gNPCSaveData[ iIndex ].sZ				= gMercProfiles[ JOE ].bSectorZ;
					gNPCSaveData[ iIndex ].sGridNo	= gMercProfiles[ JOE ].sGridNo;

					// Force reload of NPC files...
					ReloadQuoteFile( JOE );

					ChangeNpcToDifferentSector( JOE, gModSettings.ubMeanwhileInterrogatePOWSectorX, gModSettings.ubMeanwhileInterrogatePOWSectorY, 0 );
				}

			break;


	}

	// fade out old screen....
	FadeOutNextFrame( );

	// Load new map....
	gFadeOutDoneCallback = DoneFadeOutMeanwhile;


}


void DoneFadeOutMeanwhile( )
{
	// OK, insertion data found, enter sector!

	SetCurrentWorldSector( gCurrentMeanwhileDef.sSectorX, gCurrentMeanwhileDef.sSectorY, 0 );

	//LocateToMeanwhileCharacter( );
	LocateMeanWhileGrid( );

	gFadeInDoneCallback = DoneFadeInMeanwhile;

	FadeInNextFrame( );
}


void DoneFadeInMeanwhile( )
{
	// ATE: double check that we are in meanwhile
	// this is if we cancel right away.....
	if ( gfInMeanwhile )
	{
		giNPCReferenceCount = 1;

		if ( gCurrentMeanwhileDef.ubMeanwhileID != INTERROGATION )
		{
			gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;
		}

		TriggerNPCRecordImmediately( gCurrentMeanwhileDef.ubNPCNumber, (UINT8)gCurrentMeanwhileDef.usTriggerEvent );
	}
}




void BeginMeanwhileCallBack( UINT8 bExitValue )
{
	if ( bExitValue == MSG_BOX_RETURN_OK || bExitValue == MSG_BOX_RETURN_YES )
	{
		gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
		// Increment reference count...
		giNPCReferenceCount = 1;

		StartMeanwhile( );
	}
	else
	{
		// skipped scene!
		ProcessImplicationsOfMeanwhile();
		UnLockPauseState();
		UnPauseGame();
	}
}


BOOLEAN AreInMeanwhile( )
{
	STRATEGICEVENT *curr;

	//KM:	April 6, 1999
	//Tactical traversal needs to take precedence over meanwhile events.	When tactically traversing, we
	//expect to make it to the other side without interruption.
	if( gfTacticalTraversal )
	{
		return FALSE;
	}

	if( gfInMeanwhile )
	{
		return TRUE;
	}
	//Check to make sure a meanwhile scene isn't in the event list occurring at the exact same time as this call.	Meanwhile
	//scenes have precedence over a new battle if they occur in the same second.
	curr = gpEventList;
	while( curr )
	{
		if( curr->uiTimeStamp == GetWorldTotalSeconds() )
		{
			if( curr->ubCallbackID == EVENT_MEANWHILE )
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
		curr = curr->next;
	}

	return( FALSE );
}

void ProcessImplicationsOfMeanwhile( void )
{
	switch( gCurrentMeanwhileDef.ubMeanwhileID )
	{
		case END_OF_PLAYERS_FIRST_BATTLE:
		
			if( zDiffSetting[gGameOptions.ubDifficultyLevel].bStrategicAiActionWakeQueen )
			{
				//Wake up the queen earlier to punish the good players!
				ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			}
			//HandleNPCDoAction( QUEEN, NPC_ACTION_SEND_SOLDIERS_TO_BATTLE_LOCATION, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_BATTLE_LOCATION, 0, 0 );
			break;

		case CAMBRIA_LIBERATED:
			//ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, 8, 7 ); //lalien: The queen should try to get the city back

			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_CAMBRIA, 8, 8 );
			break;

		case ALMA_LIBERATED:
			//ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, 14, 9 );//lalien: The queen should try to get the city back

			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_ALMA, 14, 9 );
			break;

		case GRUMM_LIBERATED:
			//ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, 3, 8 );//lalien: The queen should try to get the city back

			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_GRUMM, 3, 8 );
			break;

		case CHITZENA_LIBERATED:
			//ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, 2, 2 );//lalien: The queen should try to get the city back

			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_CHITZENA, 2, 2 );
			break;

		case BALIME_LIBERATED:
			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			//ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, 11, 12 );//lalien: The queen should try to get the city back

			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_BALIME, 12, 12 );
			break;

		case DRASSEN_LIBERATED:
			ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_WAKE_QUEEN, 0, 0 );
			//HandleNPCDoAction( QUEEN, NPC_ACTION_SEND_SOLDIERS_TO_DRASSEN, 0 );
			ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_DRASSEN, 13, 4 );
			break;

		case CREATURES:
			// add Rat
			HandleNPCDoAction( QUEEN, NPC_ACTION_ADD_RAT, 0 );
			break;

		case AWOL_SCIENTIST:
			{
				INT16	sSectorX, sSectorY;

				StartQuest( QUEST_FIND_SCIENTIST, -1, -1 );
				// place Madlab and robot!
				if ( SectorInfo[ SECTOR( gModSettings.ubMeanwhileAddMadlabSector1X, gModSettings.ubMeanwhileAddMadlabSector1Y ) ].uiFlags & SF_USE_ALTERNATE_MAP ) //(7, 8)
				{
					sSectorX = gModSettings.ubMeanwhileAddMadlabSector1X;
					sSectorY = gModSettings.ubMeanwhileAddMadlabSector1Y;
				}
				else if ( SectorInfo[ SECTOR( gModSettings.ubMeanwhileAddMadlabSector2X, gModSettings.ubMeanwhileAddMadlabSector2Y ) ].uiFlags & SF_USE_ALTERNATE_MAP ) //(16, 8)
				{
					sSectorX = gModSettings.ubMeanwhileAddMadlabSector2X;
					sSectorY = gModSettings.ubMeanwhileAddMadlabSector2Y;
				}
				else if ( SectorInfo[ SECTOR( gModSettings.ubMeanwhileAddMadlabSector3X, gModSettings.ubMeanwhileAddMadlabSector3Y ) ].uiFlags & SF_USE_ALTERNATE_MAP ) //(11, 9)
				{
					sSectorX = gModSettings.ubMeanwhileAddMadlabSector3X;
					sSectorY = gModSettings.ubMeanwhileAddMadlabSector3Y;
				}
				else if ( SectorInfo[ SECTOR( gModSettings.ubMeanwhileAddMadlabSector4X, gModSettings.ubMeanwhileAddMadlabSector4Y ) ].uiFlags & SF_USE_ALTERNATE_MAP ) //(4, 5)
				{
					sSectorX = gModSettings.ubMeanwhileAddMadlabSector4X;
					sSectorY = gModSettings.ubMeanwhileAddMadlabSector4Y;
				}
				else
				{
					Assert( 0 );
					return;
				}
				gMercProfiles[ MADLAB ].sSectorX = sSectorX;
				gMercProfiles[ MADLAB ].sSectorY = sSectorY;
				gMercProfiles[ MADLAB ].bSectorZ = 0;

				gMercProfiles[ ROBOT ].sSectorX = sSectorX;
				gMercProfiles[ ROBOT ].sSectorY = sSectorY;
				gMercProfiles[ ROBOT ].bSectorZ = 0;
			}
			break;
		case NW_SAM:
			ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, gpSamSectorX[0], gpSamSectorY[0] );
			break;
		case NE_SAM:
			ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, gpSamSectorX[1], gpSamSectorY[1] );
			break;
		case CENTRAL_SAM:
			ExecuteStrategicAIAction( NPC_ACTION_SEND_TROOPS_TO_SAM, gpSamSectorX[2], gpSamSectorY[2] );
			break;

		default:
			break;
	}
}

void EndMeanwhile( )
{
	UINT32		cnt;
	UINT8		ubProfile;

	EmptyDialogueQueue();
	ProcessImplicationsOfMeanwhile();
	SetMeanwhileSceneSeen( gCurrentMeanwhileDef.ubMeanwhileID );

	gfInMeanwhile = FALSE;
	giNPCReferenceCount = 0;

	gTacticalStatus.uiFlags &= (~ENGAGED_IN_CONV );

	UnLockPauseState();
	UnPauseGame();

	// ATE: Make sure!
	TurnOffSectorLocator();

	if ( gCurrentMeanwhileDef.ubMeanwhileID != INTERROGATION )
	{
		gTacticalStatus.uiFlags &= (~SHOW_ALL_MERCS);

		// OK, load old sector again.....
		FadeOutNextFrame( );

		// Load new map....
		gFadeOutDoneCallback = DoneFadeOutMeanwhileOnceDone;
	}
	else
	{
		// We leave this sector open for our POWs to escape!
		// Set music mode to enemy present!
		UseCreatureMusic(HostileZombiesPresent());

		SetMusicMode( MUSIC_TACTICAL_ENEMYPRESENT );

	// ATE: Restore people to saved positions...
	// OK, restore NPC save info...
	for ( cnt = 0; cnt < guiNumNPCSaves; cnt++ )
	{
		ubProfile = gNPCSaveData[ cnt ].ubProfile;

		if ( ubProfile != NO_PROFILE )
		{
			gMercProfiles[ ubProfile ].sSectorX = gNPCSaveData[ cnt ].sX;
			gMercProfiles[ ubProfile ].sSectorY = gNPCSaveData[ cnt ].sY;
			gMercProfiles[ ubProfile ].bSectorZ = (INT8)gNPCSaveData[ cnt ].sZ;
			gMercProfiles[ ubProfile ].sGridNo	= (INT32)gNPCSaveData[ cnt ].sGridNo;

			// Ensure NPC files loaded...
			ReloadQuoteFile( ubProfile );
		}
	}

	}

}

void DoneFadeOutMeanwhileOnceDone( )
{
	UINT32		cnt;
	UINT8		ubProfile;

	// OK, insertion data found, enter sector!
	gfReloadingScreenFromMeanwhile = TRUE;


	if( gfWorldWasLoaded )
	{
		SetCurrentWorldSector( gsOldSectorX, gsOldSectorY, (INT8)gsOldSectorZ );

		ExamineCurrentSquadLights( );
	}
	else
	{
		TrashWorld( );
		// NB no world is loaded!
		gWorldSectorX = 0;
		gWorldSectorY = 0;
		gbWorldSectorZ = -1;
	}

	ChangeSelectedMapSector( gsOldSelectedSectorX, gsOldSelectedSectorY, (INT8) gsOldSelectedSectorZ );

	gfReloadingScreenFromMeanwhile = FALSE;

	// OK, restore NPC save info...
	for ( cnt = 0; cnt < guiNumNPCSaves; cnt++ )
	{
		ubProfile = gNPCSaveData[ cnt ].ubProfile;

		if ( ubProfile != NO_PROFILE )
		{
			gMercProfiles[ ubProfile ].sSectorX = gNPCSaveData[ cnt ].sX;
			gMercProfiles[ ubProfile ].sSectorY = gNPCSaveData[ cnt ].sY;
			gMercProfiles[ ubProfile ].bSectorZ = (INT8)gNPCSaveData[ cnt ].sZ;
			gMercProfiles[ ubProfile ].sGridNo	= (INT32)gNPCSaveData[ cnt ].sGridNo;

			// Ensure NPC files loaded...
			ReloadQuoteFile( ubProfile );
		}
	}

	gFadeInDoneCallback = DoneFadeInMeanwhileOnceDone;

	// OK, based on screen we were in....
	switch( guiOldScreen )
	{
		case MAP_SCREEN:
			InternalLeaveTacticalScreen( MAP_SCREEN );
			//gfEnteringMapScreen = TRUE;
			break;

		case GAME_SCREEN:
			// restore old interface panel flag
			SetCurrentInterfacePanel( (UINT8)TEAM_PANEL );
			break;
	}

	FadeInNextFrame( );

}

void DoneFadeInMeanwhileOnceDone( )
{

}

void LocateMeanWhileGrid( void )
{
	INT32 sGridNo = 0;

	// go to the approp. gridno
	sGridNo = gusMeanWhileGridNo[ ubCurrentMeanWhileId ];

	InternalLocateGridNo( sGridNo, TRUE );

	return;
}

void LocateToMeanwhileCharacter( )
{
	SOLDIERTYPE *pSoldier;

	if ( gfInMeanwhile )
	{
		pSoldier = FindSoldierByProfileID( gCurrentMeanwhileDef.ubNPCNumber, FALSE );

		if ( pSoldier != NULL )
		{
			LocateSoldier( pSoldier->ubID, FALSE );
		}
	}
}


BOOLEAN AreReloadingFromMeanwhile( )
{
	return( gfReloadingScreenFromMeanwhile );
}

UINT8 GetMeanwhileID( )
{
	return( gCurrentMeanwhileDef.ubMeanwhileID );
}


void HandleCreatureRelease( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	MeanwhileDef.ubMeanwhileID = CREATURES;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}


void HandleMeanWhileEventPostingForTownLiberation( UINT8 bTownId )
{
	// post event for meanwhile whithin the next 6 hours if it still will be daylight, otherwise the next morning
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	UINT8 ubId = 0;
	BOOLEAN fHandled = FALSE;

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	// which town iberated?
	switch( bTownId )
	{
		case DRASSEN:
			ubId = DRASSEN_LIBERATED;
			fHandled = TRUE;
			break;
		case CAMBRIA:
			ubId = CAMBRIA_LIBERATED;
			fHandled = TRUE;
			break;
		case ALMA:
			ubId = ALMA_LIBERATED;
			fHandled = TRUE;
			break;
		case GRUMM:
			ubId = GRUMM_LIBERATED;
			fHandled = TRUE;
			break;
		case CHITZENA:
			ubId = CHITZENA_LIBERATED;
			fHandled = TRUE;
			break;
		case BALIME:
			ubId = BALIME_LIBERATED;
			fHandled = TRUE;
			break;
	}

	if( fHandled )
	{
		MeanwhileDef.ubMeanwhileID = ubId;

		// schedule the event
		ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
	}
}

void HandleMeanWhileEventPostingForTownLoss( UINT8 bTownId )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;

	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( LOST_TOWN ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	MeanwhileDef.ubMeanwhileID = LOST_TOWN;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleMeanWhileEventPostingForSAMLiberation( INT8 bSamId )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	UINT8 ubId = 0;
	BOOLEAN fHandled = FALSE;

	if ( bSamId == -1 )
	{
		// invalid parameter!
		return;
	}
	else if ( bSamId == 3 )
	{
		// no meanwhile scene for this SAM site
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	// which SAM iberated?
	switch( bSamId )
	{
		case 0:
			ubId = NW_SAM;
			fHandled = TRUE;
			break;
		case 1:
			ubId = NE_SAM;
			fHandled = TRUE;
			break;
		case 2:
			ubId = CENTRAL_SAM;
			fHandled = TRUE;
			break;
		default:
			// wtf?
			break;
	}

	if( fHandled )
	{
		MeanwhileDef.ubMeanwhileID = ubId;

		// schedule the event
		ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
	}


}

void HandleFlowersMeanwhileScene( INT8 bTimeCode )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( FLOWERS ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	// time delay should be based on time code, 0 next day, 1 seeral days (random)
	if ( bTimeCode == 0 )
	{
		// 20-24 hours later
		uiTime = GetWorldTotalMin() + 60 * ( 20 + Random( 5 ) );
	}
	else
	{
		// 2-4 days later
		uiTime = GetWorldTotalMin() + 60 * ( 24 + Random( 48 ) );
	}

	MeanwhileDef.ubMeanwhileID = FLOWERS;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleOutskirtsOfMedunaMeanwhileScene( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( OUTSKIRTS_MEDUNA ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	MeanwhileDef.ubMeanwhileID = OUTSKIRTS_MEDUNA;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleKillChopperMeanwhileScene( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( KILL_CHOPPER ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 55 + Random( 10 );

	MeanwhileDef.ubMeanwhileID = KILL_CHOPPER;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleScientistAWOLMeanwhileScene( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( AWOL_SCIENTIST ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	MeanwhileDef.ubMeanwhileID = AWOL_SCIENTIST;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleInterrogationMeanwhileScene( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	// make sure scene hasn't been used before
	if ( GetMeanWhileFlag( INTERROGATION ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhileInterrogatePOWSectorX; // interrogate sector
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhileInterrogatePOWSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 60;

	MeanwhileDef.ubMeanwhileID = INTERROGATION;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );
}

void HandleFirstBattleVictory( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	UINT8 ubId = 0;

	if ( GetMeanWhileFlag( END_OF_PLAYERS_FIRST_BATTLE ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	uiTime = GetWorldTotalMin() + 5;

	ubId = END_OF_PLAYERS_FIRST_BATTLE;

	MeanwhileDef.ubMeanwhileID = ubId;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );

}


void HandleDelayedFirstBattleVictory( void )
{
	UINT32 uiTime = 0;
	MEANWHILE_DEFINITION MeanwhileDef;
	UINT8 ubId = 0;

	if ( GetMeanWhileFlag( END_OF_PLAYERS_FIRST_BATTLE ) )
	{
		return;
	}

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX;
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY;
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;

	/*
	//It is theoretically impossible to liberate a town within 60 minutes of the first battle (which is supposed to
	//occur outside of a town in this scenario).	The delay is attributed to the info taking longer to reach the queen.
	uiTime = GetWorldTotalMin() + 60;
	*/
	uiTime = GetWorldTotalMin() + 5;

	ubId = END_OF_PLAYERS_FIRST_BATTLE;

	MeanwhileDef.ubMeanwhileID = ubId;

	// schedule the event
	ScheduleMeanwhileEvent( &MeanwhileDef, uiTime );

}


void HandleFirstBattleEndingWhileInTown( INT16 sSectorX, INT16 sSectorY, INT16 bSectorZ, BOOLEAN fFromAutoResolve )
{
	INT8 bTownId = 0;
	INT16 sSector = 0;

	if ( GetMeanWhileFlag( END_OF_PLAYERS_FIRST_BATTLE ) )
	{
		return;
	}

	// if this is in fact a town and it is the first battle, then set gfFirstBattleMeanwhileScenePending true
	// if	is true then this is the end of the second battle, post the first meanwhile OR, on call to trash world, that
	// means player is leaving sector

	// grab sector value
	sSector = CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY );

	// get town name id
	bTownId = StrategicMap[ sSector ].bNameId;

	if ( bTownId == BLANK_SECTOR )
	{
		// invalid town
		HandleDelayedFirstBattleVictory( );
		gfFirstBattleMeanwhileScenePending = FALSE;
	}
	else if ( gfFirstBattleMeanwhileScenePending || fFromAutoResolve )
	{
		HandleFirstBattleVictory( );
		gfFirstBattleMeanwhileScenePending = FALSE;
	}
	else
	{
		gfFirstBattleMeanwhileScenePending = TRUE;
	}

	return;
}


void HandleFirstMeanWhileSetUpWithTrashWorld( void )
{

	// exiting sector after first battle fought
	if( gfFirstBattleMeanwhileScenePending )
	{
		HandleFirstBattleVictory( );
		gfFirstBattleMeanwhileScenePending = FALSE;
	}

}








