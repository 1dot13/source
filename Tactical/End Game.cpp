#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
	#include "Intro.h"
#else

	#include "Overhead.h"
	#include "Render Fun.h"
	#include "Random.h"
	#include "Worldman.h"
	#include "Soldier Profile.h"
	#include "NPC.h"
	#include "ai.h"
	#include "Dialogue Control.h"
	#include "Handle UI.h"
	#include "end game.h"
	#include "Intro.h"
	#include "Exit Grids.h"
	#include "strategicmap.h"
	#include "Quests.h"
	#include "SaveLoadMap.h"
	#include "Sound Control.h"
	#include "renderworld.h"
	#include "Isometric Utils.h"
	#include "Music Control.h"
	#include "Soldier macros.h"
	#include "qarray.h"
	#include "los.h"
	#include "Strategic AI.h"
	#include "Squads.h"
	#include "PreBattle Interface.h"
	#include "Strategic Movement.h"
	#include "strategic.h"
	#include "Morale.h"
	#include "Queen Command.h"
	#include "Strategic Town Loyalty.h"
	#include "Player Command.h"
	#include "Campaign Types.h"
	#include "Tactical Save.h"
	#include "screenids.h"
#endif

#ifdef JA2UB
#include "email.h"
#include "Game Clock.h"
#include "Ja25_Tactical.h"
#include "Game Init.h"
#include "interface Dialogue.h"
#include "ub_config.h"

void HandleAddingTheEndGameEmails();
void EndFadeToCredits( void );
void FadeToCredits( void );
void InFinalSectorAfterFadeIn( void );
void FadeOutToLaptopOnEndGame( void );

BOOLEAN			gfPlayersLaptopWasntWorkingAtEndOfGame;
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//victory ending scene sector
#define VICTORY_X	gModSettings.ubEndGameVictorySectorX
#define VICTORY_Y	gModSettings.ubEndGameVictorySectorY

INT32 sStatueGridNos[] = { 13829, 13830, 13669, 13670 };

SOLDIERTYPE *gpKillerSoldier = NULL;
INT32				 gsGridNo;
INT8				gbLevel;


// This function checks if our statue exists in the current sector at given gridno
BOOLEAN DoesO3SectorStatueExistHere( INT32 sGridNo )
{
	INT32 cnt;
	EXITGRID								ExitGrid;

	// First check current sector......
	if ( gWorldSectorX == 3 && gWorldSectorY == MAP_ROW_O && gbWorldSectorZ == 0 )
	{
		// Check for exitence of and exit grid here...
		// ( if it doesn't then the change has already taken place )
		if ( !GetExitGrid( 13669, &ExitGrid ) )
		{
			for ( cnt = 0; cnt < 4; cnt++ )
			{
				if ( sStatueGridNos[ cnt ] == sGridNo )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}

// This function changes the graphic of the statue and adds the exit grid...
void ChangeO3SectorStatue( BOOLEAN fFromExplosion )
{
	EXITGRID								ExitGrid;
	UINT16									usTileIndex;
	INT16 sX, sY;

	// Remove old graphic
	ApplyMapChangesToMapTempFile( TRUE );
	// Remove it!
	// Get index for it...
	GetTileIndexFromTypeSubIndex( EIGHTOSTRUCT, (INT8)( 5 ), &usTileIndex );
	RemoveStruct( 13830, usTileIndex );

	// Add new one...
	if ( fFromExplosion )
	{
		// Use damaged peice
		GetTileIndexFromTypeSubIndex( EIGHTOSTRUCT, (INT8)( 7 ), &usTileIndex );
	}
	else
	{
		GetTileIndexFromTypeSubIndex( EIGHTOSTRUCT, (INT8)( 8 ), &usTileIndex );
		// Play sound...

	PlayJA2Sample( OPEN_STATUE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

	}
	AddStructToHead( 13830, usTileIndex );

	// Add exit grid
	ExitGrid.ubGotoSectorX = 3;
	ExitGrid.ubGotoSectorY = MAP_ROW_O;
	ExitGrid.ubGotoSectorZ = 1;
	ExitGrid.usGridNo = 13037;	 //dnl!!!

	AddExitGridToWorld( 13669, &ExitGrid );
	gpWorldLevelData[ 13669 ].uiFlags |= MAPELEMENT_REVEALED;

	// Turn off permenant changes....
	ApplyMapChangesToMapTempFile( FALSE );

	// Re-render the world!
	gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;
	// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
	InvalidateWorldRedundency( );
	SetRenderFlags(RENDER_FLAG_FULL);

	// Redo movement costs....
	ConvertGridNoToXY( 13830, &sX, &sY );

	RecompileLocalMovementCostsFromRadius( 13830, 5 );

}
#ifdef JA2UB
//Ja25 no queen
#else
void DeidrannaTimerCallback( void )
{
	HandleDeidrannaDeath( gpKillerSoldier, gsGridNo, gbLevel );
}


void BeginHandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
	gpKillerSoldier = pKillerSoldier;
	gsGridNo = sGridNo;
	gbLevel	= bLevel;

	// Lock the UI.....
	gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
	// Increment refrence count...
	giNPCReferenceCount = 1;

	gTacticalStatus.uiFlags |= IN_DEIDRANNA_ENDGAME;

	SetCustomizableTimerCallbackAndDelay( 2000, DeidrannaTimerCallback, FALSE );

}

void HandleDeidrannaDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;
	UINT8		ubKillerSoldierID = NOBODY;

	// Start victory music here...
	SetMusicMode( MUSIC_TACTICAL_VICTORY );
	
	if ( pKillerSoldier )
	{
		TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLING_DEIDRANNA );
		ubKillerSoldierID = pKillerSoldier->ubID;
	}

	// STEP 1 ) START ALL QUOTES GOING!
	// OK - loop through all witnesses and see if they want to say something abou this...
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++cnt,pTeamSoldier++ )
	{
		if ( cnt != ubKillerSoldierID )
		{
			if ( OK_INSECTOR_MERC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !AM_AN_EPC( pTeamSoldier ) )
			{
				if ( QuoteExp[ pTeamSoldier->ubProfile ].QuoteExpWitnessDeidrannaDeath )
				{
					if ( SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo,  bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
					{
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_KILLING_DEIDRANNA );
					}
				}
			}
		}
	}

	// Set fact that she is dead!
	SetFactTrue( FACT_QUEEN_DEAD );

	ExecuteStrategicAIAction( STRATEGIC_AI_ACTION_QUEEN_DEAD, 0, 0 );

	// AFTER LAST ONE IS DONE - PUT SPECIAL EVENT ON QUEUE TO BEGIN FADE< ETC
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE, MULTIPURPOSE_SPECIAL_EVENT_DONE_KILLING_DEIDRANNA, 0,0,0,0 );
}

void DoneFadeInKilledQueen( void )
{
	SOLDIERTYPE *pNPCSoldier;

	// Locate gridno.....

	// Run NPC script
	pNPCSoldier = FindSoldierByProfileID( 136, FALSE );
	if ( !pNPCSoldier )
	{
		return;
	}

	// Converse!
	//InitiateConversation( pNPCSoldier, pSoldier, 0, 1 );
	TriggerNPCRecordImmediately( pNPCSoldier->ubProfile, 6 );

}

void DoneFadeOutKilledQueen( void )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier, *pTeamSoldier;

	// For one, loop through our current squad and move them over
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		// Are we in this sector, On the current squad?
		if ( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && pSoldier->bInSector && pSoldier->bAssignment == CurrentSquad( ) )
		{
			gfTacticalTraversal = TRUE;
			SetGroupSectorValue( VICTORY_X, VICTORY_Y, 0, pSoldier->ubGroupID );

			// Set next sectore
			pSoldier->sSectorX = VICTORY_X;
			pSoldier->sSectorY = VICTORY_Y;
			pSoldier->bSectorZ = 0;

			// Set gridno
			pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
			pSoldier->usStrategicInsertionData = gModSettings.iEndGameVictoryGridNo; //5687 dnl!!!
			// Set direction to face....
			pSoldier->ubInsertionDirection		= 100 + NORTHWEST;
		}
	}

	// Kill all enemies in world.....
	cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++,pTeamSoldier++)
	{
		// Are we active and in sector.....
		if ( pTeamSoldier->bActive	)
		{
			// For sure for flag thet they are dead is not set
			// Check for any more badguys
			// ON THE STRAGETY LAYER KILL BAD GUYS!
			if ( !pTeamSoldier->aiData.bNeutral && (pTeamSoldier->bSide != gbPlayerNum ) )
			{
				ProcessQueenCmdImplicationsOfDeath( pTeamSoldier );
			}
		}
	}

	// 'End' battle
	ExitCombatMode();
	gTacticalStatus.fLastBattleWon = TRUE;
	// Set enemy presence to false
	gTacticalStatus.fEnemyInSector = FALSE;

	SetMusicMode( MUSIC_TACTICAL_VICTORY );

	HandleMoraleEvent( NULL, MORALE_QUEEN_BATTLE_WON, VICTORY_X, VICTORY_Y, 0 );
	HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_QUEEN_BATTLE_WON, VICTORY_X, VICTORY_Y, 0 );

	SetMusicMode( MUSIC_TACTICAL_VICTORY );

	SetThisSectorAsPlayerControlled( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, TRUE );

	// ATE: Force change of level set z to 1 to allow reloading of same sector
	gbWorldSectorZ = 1;

	// Clear out dudes.......
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubNumAdmins = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubNumTroops = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubNumElites = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubNumTanks = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubAdminsInBattle = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubTroopsInBattle = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubElitesInBattle = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubRobotsInBattle = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubTanksInBattle = 0;
	SectorInfo[ SECTOR( VICTORY_X, VICTORY_Y ) ].ubJeepsInBattle = 0;

	// ATE: banish elliot... dead or alive
	gMercProfiles[ ELLIOT ].sSectorX = 0;
	gMercProfiles[ ELLIOT ].sSectorY = 0;
	gMercProfiles[ ELLIOT ].bSectorZ = 0;

	ChangeNpcToDifferentSector( DEREK, VICTORY_X, VICTORY_Y, 0 );
	ChangeNpcToDifferentSector( OLIVER, VICTORY_X, VICTORY_Y, 0 );


	// OK, insertion data found, enter sector!
	SetCurrentWorldSector( VICTORY_X, VICTORY_Y, 0 );

	// OK, once down here, adjust the above map with crate info....
	gfTacticalTraversal = FALSE;
	gpTacticalTraversalGroup = NULL;
	gpTacticalTraversalChosenSoldier = NULL;

	gFadeInDoneCallback = DoneFadeInKilledQueen;

	FadeInGameScreen( );
}

// Called after all player quotes are done....
void HandleDoneLastKilledQueenQuote( )
{
	gFadeOutDoneCallback = DoneFadeOutKilledQueen;

	FadeOutGameScreen( );
}


void EndQueenDeathEndgameBeginEndCimenatic( )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier;

	// Start end cimimatic....
	gTacticalStatus.uiFlags |= IN_ENDGAME_SEQUENCE;

	// first thing is to loop through team and say end quote...
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		// Are we in this sector, On the current squad?
		if ( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && !AM_AN_EPC( pSoldier ) )
		{
			TacticalCharacterDialogue( pSoldier, QUOTE_END_GAME_COMMENT );
		}
	}

	// Add queue event to proceed w/ smacker cimimatic
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE, MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING, 0,0,0,0 );

}

void EndQueenDeathEndgame( )
{
	// Unset flags...
	gTacticalStatus.uiFlags &= (~ENGAGED_IN_CONV );
	// Increment refrence count...
	giNPCReferenceCount = 0;

	gTacticalStatus.uiFlags &= (~IN_DEIDRANNA_ENDGAME);
}


void DoneFadeOutEndCinematic( void )
{
	// DAVE PUT SMAKER STUFF HERE!!!!!!!!!!!!
	// :)
	gTacticalStatus.uiFlags &= (~IN_ENDGAME_SEQUENCE);


	// For now, just quit the freaken game...
//	InternalLeaveTacticalScreen( MAINMENU_SCREEN );

	InternalLeaveTacticalScreen( INTRO_SCREEN );
//	guiCurrentScreen = INTRO_SCREEN;


	SetIntroType( INTRO_ENDING );
}
#endif
// OK, end death UI - fade to smaker....
void HandleDoneLastEndGameQuote( )
{
#ifdef JA2UB
//Ja25 No queen	
#else
EndQueenDeathEndgame( );
#endif
	gFadeOutDoneCallback = DoneFadeOutEndCinematic;

	FadeOutGameScreen( );
}



void QueenBitchTimerCallback( void )
{
#ifdef JA2UB
//no Ub
#else
	HandleQueenBitchDeath( gpKillerSoldier, gsGridNo, gbLevel );
#endif
}

#ifdef JA2UB
void EndGameEveryoneSayTheirGoodByQuotes( void )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier;

	// Start end cimimatic....
  gTacticalStatus.uiFlags |= IN_ENDGAME_SEQUENCE;

	//lock the interface
	guiPendingOverrideEvent = LU_BEGINUILOCK;
	HandleTacticalUI( );

	//
	// first thing is to loop through team and look for QUALIFIED mercs on the team to say special end game quote
	//
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{       
		// Are we in this sector, On the current squad?
		if( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && !AM_AN_EPC( pSoldier ) && IsSoldierQualifiedMerc( pSoldier ) )
		{
			TacticalCharacterDialogue( pSoldier, QUOTE_RENEWING_CAUSE_BUDDY_2_ON_TEAM );	
		}
	}

	//
	// Next is to loop through ENTIRE team and say end quote...
	//
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{       
		// Are we in this sector, On the current squad?
		if ( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && !AM_AN_EPC( pSoldier ) )
		{
			TacticalCharacterDialogue( pSoldier, QUOTE_END_GAME_COMMENT );	
		}
	}

	// Add queue event to proceed w/ smacker cimimatic
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE, MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING, 0,0,0,0 );
}


void HandleAddingTheEndGameEmails()
{
	BOOLEAN				fMiguelAlive=FALSE;
	BOOLEAN				fManuelAlive=FALSE;
	BOOLEAN				fManuelHired=FALSE;


	//Miguel alive
	if( gubFact[ FACT_PLAYER_IMPORTED_SAVE_MIGUEL_DEAD ] )
		fMiguelAlive = FALSE;
	else
		fMiguelAlive = TRUE;

	//manuel alive
	if( gMercProfiles[ MANUEL_UB ].bMercStatus == MERC_IS_DEAD )
		fManuelAlive = FALSE;
	else
		fManuelAlive = TRUE;

	//manuel hired
	if( gMercProfiles[ MANUEL_UB ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
		fManuelHired = TRUE;
	else
		fManuelHired = FALSE;

	//
	// Determine the EMAIL to be sent out to the player
	//

	#ifdef JA113DEMO
	//no demo
	#else
	// email # 12a - Miguel dead, Manuel never recruited
	if( !fMiguelAlive && !fManuelHired )
	{
		AddEmail( EMAIL_CONGRATS, EMAIL_CONGRATS_LENGTH, MAIL_ENRICO,  GetWorldTotalMin(),-1 ,-1, TYPE_EMAIL_EMAIL_EDT);
	}
		
	// email # 12b - Miguel alive, Manuel never recruited
	else if( fMiguelAlive && !fManuelHired )
	{
		AddEmail( EMAIL_CONGRATSICK, EMAIL_CONGRATSICK_LENGTH, MAIL_ENRICO,  GetWorldTotalMin(),-1 ,-1, TYPE_EMAIL_EMAIL_EDT);
	}

	// email # 12c - Miguel alive, Manuel dead
	else if( fMiguelAlive && !fManuelAlive )
	{
		AddEmail( EMAIL_CONGRATMIGMANUELDEAD, EMAIL_CONGRATMIGMANUELDEAD_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() , -1, -1, TYPE_EMAIL_EMAIL_EDT);
	}

	// email # 12d - Miguel alive, Manuel recruited and alive
	else if( fMiguelAlive && fManuelAlive && fManuelHired )
	{
		AddEmail( EMAIL_CONGRATMIGMANUELALIVE, EMAIL_CONGRATMIGMANUELALIVE_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() , -1, -1, TYPE_EMAIL_EMAIL_EDT);
	}

	// email # 12e - Miguel dead, Manuel dead
	else if( !fMiguelAlive && !fManuelAlive )
	{
		AddEmail( EMAIL_CONGRATMANUELDEAD, EMAIL_CONGRATMANUELDEAD_LENGTH, MAIL_ENRICO,  GetWorldTotalMin(), -1 , -1, TYPE_EMAIL_EMAIL_EDT);
	}

	// email # 12f -  Miguel dead, Manuel recruited and alive
	else if( !fMiguelAlive && fManuelAlive && fManuelHired )
	{
		AddEmail( EMAIL_CONGRATMANUELALIVE, EMAIL_CONGRATMANUELALIVE_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() , -1, -1, TYPE_EMAIL_EMAIL_EDT);
	}

	else
	{
		Assert( 0 );
	}
	#endif
}


void HandleEveryoneDoneTheirEndGameQuotes()
{
	// UnLock UI!
	guiPendingOverrideEvent = LU_ENDUILOCK;
	HandleTacticalUI( );

	//if laptop is still BROKEN
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTINPROGRESS && gGameUBOptions.LaptopQuestEnabled == TRUE )
	{
		gfPlayersLaptopWasntWorkingAtEndOfGame = TRUE;

		// otherwise, go to the credits screen
		HandleJa25EndGameAndGoToCreditsScreen( TRUE );
	}
	else
	{
		gfPlayersLaptopWasntWorkingAtEndOfGame = FALSE;

		gFadeOutDoneCallback = FadeOutToLaptopOnEndGame;

		FadeOutGameScreen( );	
	}
}


void HandleJa25EndGameAndGoToCreditsScreen( BOOLEAN fFromTactical )
{
	if( fFromTactical )
	{
		FadeToCredits( );
	}
	else
	{
		//Reset flag indicating we are in the end game sequence
		gTacticalStatus.uiFlags &= ~IN_ENDGAME_SEQUENCE;

		//We want to reinitialize the game
		ReStartingGame();	
	}
}

void EnterTacticalInFinalSector()
{
	gFadeInDoneCallback = InFinalSectorAfterFadeIn;

	FadeInGameScreen( );
}

void InFinalSectorAfterFadeIn( void )
{
	//Have everyone start talking
	DelayedMercQuote( NOBODY, DQ__START_EVERYONE_TALKING_AT_END_OF_GAME, GetWorldTotalSeconds() + 2 );
}

void FadeToCredits( void )
{
	gFadeOutDoneCallback = EndFadeToCredits;

	FadeOutGameScreen( );	
}

void EndFadeToCredits( void )
{
	//then we can go strait to the laptop screen
	InternalLeaveTacticalScreen( CREDIT_SCREEN );

	//Reset flag indicating we are in the end game sequence
	gTacticalStatus.uiFlags &= ~IN_ENDGAME_SEQUENCE;

	//We want to reinitialize the game
	ReStartingGame();	
}

void FadeOutToLaptopOnEndGame( void )
{

	//then we can go strait to the laptop screen
	InternalLeaveTacticalScreen( LAPTOP_SCREEN );

	//Add the end Game Emails
	HandleAddingTheEndGameEmails();
}
#else  //Ja25 no queen

void BeginHandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;


	gpKillerSoldier = pKillerSoldier;
	gsGridNo = sGridNo;
	gbLevel	= bLevel;

	// Lock the UI.....
	gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
	// Increment refrence count...
	giNPCReferenceCount = 1;

	// gTacticalStatus.uiFlags |= IN_DEIDRANNA_ENDGAME;

	SetCustomizableTimerCallbackAndDelay( 3000, QueenBitchTimerCallback, FALSE );


	// Kill all enemies in creature team.....
	cnt = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; cnt++,pTeamSoldier++)
	{
		// Are we active and ALIVE and in sector.....
		if ( pTeamSoldier->bActive && pTeamSoldier->stats.bLife > 0 )
		{
			// For sure for flag thet they are dead is not set
			// Check for any more badguys
			// ON THE STRAGETY LAYER KILL BAD GUYS!

			// HELLO!	THESE ARE CREATURES!	THEY CAN'T BE NEUTRAL!
			//if ( !pTeamSoldier->aiData.bNeutral && (pTeamSoldier->bSide != gbPlayerNum ) )
			{
//	 		gTacticalStatus.ubAttackBusyCount++;
				DebugAttackBusy( "Killing off a queen ally.\n");
				pTeamSoldier->EVENT_SoldierGotHit( 0, 10000, 0, pTeamSoldier->ubDirection, 320, NOBODY , FIRE_WEAPON_NO_SPECIAL, pTeamSoldier->bAimShotLocation, 0, NOWHERE );
			}
		}
	}


}

void HandleQueenBitchDeath( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;
	UINT8		ubKillerSoldierID = NOBODY;

	// Start victory music here...
	SetMusicMode( MUSIC_TACTICAL_VICTORY );

	if ( pKillerSoldier )
	{
		TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLING_QUEEN );
		ubKillerSoldierID = pKillerSoldier->ubID;
	}

	// STEP 1 ) START ALL QUOTES GOING!
	// OK - loop through all witnesses and see if they want to say something abou this...
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		if ( cnt != ubKillerSoldierID )
		{
			if ( OK_INSECTOR_MERC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !AM_AN_EPC( pTeamSoldier ) )
			{
				if ( QuoteExp[ pTeamSoldier->ubProfile ].QuoteExpWitnessQueenBugDeath )
				{
					if ( SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo,  bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
					{
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_KILLING_QUEEN );
					}
				}
			}
		}
	}


	// Set fact that she is dead!
	if ( CheckFact( FACT_QUEEN_DEAD, 0 ) )
	{
	 EndQueenDeathEndgameBeginEndCimenatic( );
	}
	else
	{
	// Unset flags...
	gTacticalStatus.uiFlags &= (~ENGAGED_IN_CONV );
	// Increment refrence count...
	giNPCReferenceCount = 0;
	}
}
#endif

#ifdef JA2UB
//JA25UB
void DoneFadeOutEndCinematic( void )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier;

	//Change the currently selecter sector in mapscreen
	//ChangeSelectedMapSector( 16, 11, 0 );
	  ChangeSelectedMapSector( gGameUBOptions.ubEndDefaultSectorX, gGameUBOptions.ubEndDefaultSectorY, gGameUBOptions.ubEndDefaultSectorZ );
	//
	// Loop through all the soldiers and move any of them that are in the complex to be in the safe sector near ther
	//
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{       
		// if the soldier was in the complex
		if( pSoldier->bActive && 
				pSoldier->sSectorX == 15 && ( pSoldier->sSectorY == 11 || pSoldier->sSectorY == 12 ) )
		{
			if ( GetGroup( pSoldier->ubGroupID ) )
			{
				//move them to the 'fake' sector
				//PlaceGroupInSector( pSoldier->ubGroupID, 15, 11, 16, 11, 0, FALSE );
				  PlaceGroupInSector( pSoldier->ubGroupID, 15, 11, gGameUBOptions.ubEndDefaultSectorX, gGameUBOptions.ubEndDefaultSectorY, gGameUBOptions.ubEndDefaultSectorZ, FALSE );
			}
			else
			{
				pSoldier->sSectorX = gGameUBOptions.ubEndDefaultSectorX; //16;
				pSoldier->sSectorY = gGameUBOptions.ubEndDefaultSectorY; //11;
				pSoldier->bSectorZ = gGameUBOptions.ubEndDefaultSectorZ; //0;
			}
		}
	}

	//
	// Go watch the movies
	//
	InternalLeaveTacticalScreen( INTRO_SCREEN );
	SetIntroType( INTRO_ENDING );
}
#endif
