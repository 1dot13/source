	#include "types.h"
	#include "overhead.h"
	#include "animation control.h"
	#include "points.h"
	#include "opplist.h"
	#include "timer.h"
	#include "event pump.h"
	#include "Sound Control.h"
	#include "interface.h"
	#include "Isometric Utils.h"
	#include "Font Control.H"
	#include "ai.h"
	#include "interface.h"
	#include "message.h"
	#include "text.h"
	#include "TeamTurns.h"
	#include "Smell.h"
	#include "game clock.h"
	#include "Soldier Functions.h"
	#include "cursors.h"
	#include "Queen Command.h"
	#include "Pathai.h"
	#include "lighting.h"
	#include "environment.h"
	#include "Explosion Control.h"
	#include "dialogue control.h"
	#include "Soldier Profile Type.h"
	#include "SmokeEffects.h"
	#include "lighteffects.h"
	#include "air raid.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "AIInternals.h"
	#include "AIList.h"
	#ifdef DEBUG_INTERRUPTS
		#include "debug.h"
	#endif
	#include "renderworld.h"
	#include "rotting corpses.h"
	#include "Squads.h"
	#include "Soldier macros.h"
	#include "Soldier Profile.h"
	#include "NPC.h"
	#include "drugs and alcohol.h"	// added by Flugente

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

// HEADROCK HAM 3.2: Gamesettings.h for external modifications to team turns.
#include "GameSettings.h"
#include "Reinforcement.h"
#include "fresh_header.h"
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
#include "connect.h"

extern INT8 STRAIGHT;
//extern UINT8 gubSpeedUpAnimationFactor;
void SetSoldierAniSpeed( SOLDIERTYPE *pSoldier );

// sevenfm
time_t gtTimeSinceMercAIStart;

void RecalculateSoldiersAniSpeed()
{
	UINT32 uiLoop;
	SOLDIERTYPE *pSoldier;

//	if( gubSpeedUpAnimationFactor == 1 )return;

	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, dead, unconscious
		if (!pSoldier)
		{
			continue;			// next merc
		}

		SetSoldierAniSpeed( pSoldier );
	}

}



extern void DecayPublicOpplist(INT8 bTeam);
extern void VerifyAndDecayOpplist(SOLDIERTYPE *pSoldier);
void EndInterrupt( BOOLEAN fMarkInterruptOccurred );
void DeleteFromIntList( UINT16 ubIndex, BOOLEAN fCommunicate);

#define END_OF_INTERRUPTS 255

UINT16 gubOutOfTurnOrder[MAXMERCS] = { END_OF_INTERRUPTS, 0 };
UINT16 gubOutOfTurnPersons = 0;

#define LATEST_INTERRUPT_GUY (gubOutOfTurnOrder[gubOutOfTurnPersons])
#define REMOVE_LATEST_INTERRUPT_GUY()	(DeleteFromIntList( (gubOutOfTurnPersons), TRUE ))
#define INTERRUPTS_OVER (gubOutOfTurnPersons == 1)

SoldierID	InterruptOnlyGuynum = NOBODY;
BOOLEAN		InterruptsAllowed = TRUE;
BOOLEAN		gfHiddenInterrupt = FALSE;
SoldierID	gubLastInterruptedGuy = NOBODY;

extern UINT16 gsWhoThrewRock;
extern UINT8 gubSightFlags;

typedef struct
{
	UINT16		ubOutOfTurnPersons;

	SoldierID	InterruptOnlyGuynum;
	INT16		sWhoThrewRock;
	BOOLEAN		InterruptsAllowed;
	BOOLEAN		fHiddenInterrupt;
	SoldierID	ubLastInterruptedGuy;

	UINT8	ubFiller[16];
} TEAM_TURN_SAVE_STRUCT;

// WANNE: Moved to APBPConstants
//#define MIN_APS_TO_INTERRUPT 4

void ClearIntList( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"ClearIntList");
	memset( &gubOutOfTurnOrder, 0, MAXMERCS );
	gubOutOfTurnOrder[0] = END_OF_INTERRUPTS;
	gubOutOfTurnPersons = 0;
}

BOOLEAN BloodcatsPresent( void )
{
	SOLDIERTYPE *pSoldier;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"BloodcatsPresent");
	if ( gTacticalStatus.Team[ CREATURE_TEAM ].bTeamActive == FALSE )
	{
		return( FALSE );
	}

	for ( SoldierID iLoop = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID; iLoop <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; ++iLoop )
	{
		pSoldier = iLoop;

		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife > 0 && pSoldier->ubBodyType == BLOODCAT )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

void StartPlayerTeamTurn( BOOLEAN fDoBattleSnd, BOOLEAN fEnteringCombatMode )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"StartPlayerTeamTurn");
//	SOLDIERTYPE		*pSoldier;
//	EV_S_BEGINTURN	SBeginTurn;

	SetFastForwardMode(FALSE);
	SetClockSpeedPercent(gGameExternalOptions.fClockSpeedPercent);	// sevenfm: set default clock speed

	gTacticalStatus.ubDisablePlayerInterrupts = FALSE;

	// Start the turn of player charactors

	//
	// PATCH 1.06:
	//
	// make sure set properly in gTacticalStatus:
	gTacticalStatus.ubCurrentTeam = OUR_TEAM;

	InitPlayerUIBar( FALSE );

	if ( gTacticalStatus.uiFlags & TURNBASED )
	{
		// Are we in combat already?
		if ( gTacticalStatus.uiFlags & INCOMBAT )
		{
			PlayJA2Sample( ENDTURN_1, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
		}

		// Remove deadlock message
		EndDeadlockMsg( );

		// Check for victory conditions

		// ATE: Commented out - looks like this message is called earlier for our team
		// look for all mercs on the same team,
		//SoldierID cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		//for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
		//{
		//	if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )
		//	{
		//		SBeginTurn.usSoldierID		= (UINT16)cnt;
		//		AddGameEvent( S_BEGINTURN, 0, &SBeginTurn );
		//	}
		//}

		// Are we in combat already?
		if ( gTacticalStatus.uiFlags & INCOMBAT )
		{
			if ( gusSelectedSoldier != NOBODY )
			{
				// Check if this guy is able to be selected....
				if ( gusSelectedSoldier->stats.bLife < OKLIFE )
				{
					DebugMsg(TOPIC_JA2INTERRUPT,DBG_LEVEL_3,String("StartPlayerTeamTurn: SelectNextAvailSoldier"));
					SelectNextAvailSoldier( gusSelectedSoldier );
				}
				else
				{
					SelectSoldier( gusSelectedSoldier, FALSE, FALSE);
				}

				// Slide to selected guy...
				if ( gusSelectedSoldier != NOBODY )
				{
					SlideTo(gusSelectedSoldier, SETLOCATOR);

					if ( fDoBattleSnd )
					{
						// Say ATTENTION SOUND...
						gusSelectedSoldier->DoMercBattleSound( BATTLE_SOUND_ATTN1 );
					}

					if ( gsInterfaceLevel == 1 )
					{
						gTacticalStatus.uiFlags |= SHOW_ALL_ROOFS;
						InvalidateWorldRedundency( );
						SetRenderFlags(RENDER_FLAG_FULL);
						ErasePath(FALSE);
					}
				}
			}
		}

		// Dirty panel interface!
		fInterfacePanelDirty = DIRTYLEVEL2;

		// Adjust time now!
		UpdateClock( );

		if ( !fEnteringCombatMode )
		{
			CheckForEndOfCombatMode( TRUE );
		}

	}
	// Signal UI done enemy's turn
	guiPendingOverrideEvent = LU_ENDUILOCK;
	
	if (is_networked)
		guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;

	// ATE: Reset killed on attack variable.. this is because sometimes timing is such
	/// that a baddie can die and still maintain it's attacker ID
	gTacticalStatus.fKilledEnemyOnAttack = FALSE;
	
	gTacticalStatus.ubInterruptPending	= DISABLED_INTERRUPT;

	HandleTacticalUI( );
}

void FreezeInterfaceForEnemyTurn( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"FreezeInterfaceForEnemyTurn");
	// Reset flags
	gfPlotNewMovement = TRUE;

	// Erase path
	ErasePath( TRUE );

	// Setup locked UI
	if(is_client)
	{
		guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
	}
	else 
	{
		guiPendingOverrideEvent = LU_BEGINUILOCK;
	}

	// Remove any UI messages!
	if ( giUIMessageOverlay != -1 )
	{
		EndUIMessage( );
	}
}


void EndTurn( UINT8 ubNextTeam )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EndTurn");
	SOLDIERTYPE * pSoldier;

	//Check for enemy pooling (add enemies if there happens to be more than the max in the
	//current battle.	If one or more slots have freed up, we can add them now.

	EndDeadlockMsg( );

/*
	if ( CheckForEndOfCombatMode( FALSE ) )
	{
		return;
	}
	*/

	if (INTERRUPT_QUEUED)
	{
			if(is_networked)
			{
				end_interrupt( FALSE );//this tells other client to go on from where he was
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Player ended interrupt." );
			}
			EndInterrupt( FALSE );
	}
	else
	{
		if(gGameExternalOptions.gfAllowReinforcements)//dnl ch68 100913 agree with Flugente, put all under one condition
		{
			guiTurnCnt++;
			// Flugente: I'm not really sure why we check here for gGameExternalOptions.ubReinforcementsFirstTurnFreeze, shouldn't that be a check for ALLOW_REINFORCEMENTS?
			// as this inhibits reinforcements during turnbased-mode, I'll check for that instead
			// HEADROCK HAM 3.2: Experimental fix to force reinforcements enter battle with 0 APs.
			//if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze != 1 && gGameExternalOptions.ubReinforcementsFirstTurnFreeze != 2)
			AddPossiblePendingEnemiesToBattle();
			//if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze != 1 && gGameExternalOptions.ubReinforcementsFirstTurnFreeze != 3)
			AddPossiblePendingMilitiaToBattle();
		}

//		InitEnemyUIBar( );

		FreezeInterfaceForEnemyTurn();

		// Loop through all mercs and set to moved
		SoldierID cnt = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID;
		for ( ; cnt <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++cnt )
		{
			pSoldier = cnt;
			if ( pSoldier->bActive )
			{
				pSoldier->aiData.bMoved = TRUE;
			}
		}

		gTacticalStatus.ubCurrentTeam	= ubNextTeam;
		
		gTacticalStatus.ubInterruptPending	= DISABLED_INTERRUPT;

		if(is_server || !is_client) BeginTeamTurn( gTacticalStatus.ubCurrentTeam );

		// WANNE: Disabled Headrocks Experimental fix, because it causes assertion in AddPossiblePendingMilitiaToBattle();
		// HEADROCK HAM 3.2: Experimental fix to force reinforcements enter battle with 0 APs.
		/*
		if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 1 || gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 2)
		{
			AddPossiblePendingEnemiesToBattle();
		}
		if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 1 || gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 3)
		{
			AddPossiblePendingMilitiaToBattle();
		}
		*/

		BetweenTurnsVisibilityAdjustments();
	}
}

void EndAITurn( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EndAITurn");
	SOLDIERTYPE * pSoldier;

	// Remove any deadlock message
	EndDeadlockMsg( );
	if (INTERRUPT_QUEUED)
	{
			if(is_networked)
			{
				end_interrupt( FALSE );//this tells other client to go on from where he was
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"AI ended interrupt." );
			}
		EndInterrupt( FALSE );
	}
	else
	{
		SoldierID cnt = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID;
		for ( ; cnt <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++cnt )
		{
			pSoldier = cnt;
			if ( pSoldier->bActive )
			{
				pSoldier->aiData.bMoved = TRUE;
				// record old life value... for creature AI; the human AI might
				// want to use this too at some point
				pSoldier->bOldLife = pSoldier->stats.bLife;
			}
		}

		gTacticalStatus.ubCurrentTeam++;
		BeginTeamTurn( gTacticalStatus.ubCurrentTeam );
	}
}

void EndAllAITurns( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EndAllAITurns");
	// warp turn to the player's turn
	SOLDIERTYPE * pSoldier;

	// Remove any deadlock message
	EndDeadlockMsg( );
	if (INTERRUPT_QUEUED)
	{
		EndInterrupt( FALSE );
	}

	if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum )
	{
		SoldierID id = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID;
		for ( ; id <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++id )
		{
			pSoldier = id;
			if ( pSoldier->bActive )
			{
				pSoldier->aiData.bMoved = TRUE;
				pSoldier->flags.uiStatusFlags &= (~SOLDIER_UNDERAICONTROL);
				// record old life value... for creature AI; the human AI might
				// want to use this too at some point
				pSoldier->bOldLife = pSoldier->stats.bLife;
			}
		}

		gTacticalStatus.ubCurrentTeam = gbPlayerNum;
		//BeginTeamTurn( gTacticalStatus.ubCurrentTeam );
		
		gTacticalStatus.ubInterruptPending	= DISABLED_INTERRUPT;
	}
}

void EndTurnEvents( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EndTurnEvents");
	// HANDLE END OF TURN EVENTS
	// handle team services like healing
	HandleTeamServices( gbPlayerNum );
	// handle smell and blood decay
	DecaySmells();
	// decay bomb timers and maybe set some off!
	DecayBombTimers();

	DecayLightEffects( GetWorldTotalSeconds( ) );
	DecaySmokeEffects( GetWorldTotalSeconds( ) );

	SOLDIERTYPE* pSoldier = NULL;
	SoldierID  id = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( ; id <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++id )
	{
		pSoldier = id;
		if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )//&& !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !( AM_A_ROBOT( pSoldier ) ) )
		{
			// Flugente: update multi-turn actions
			pSoldier->UpdateMultiTurnAction();
		}
	}

	// Flugente: Cool down/decay all items not in a soldier's inventory
	CoolDownWorldItems( );	

	// Flugente: raise zombies if in gamescreen and option set
	if ( guiCurrentScreen == GAME_SCREEN )
	{
		RaiseZombies();
	}

	// decay AI warning values from corpses
	DecayRottingCorpseAIWarnings();

	HandleEnvironmentHazard( );

#ifdef JA2UB	
	//Ja25 UB
	
	//increment the number of tactical turns that have gone by in turn based mode
	gJa25SaveStruct.uiTacticalTurnCounter++;

	//if the fan should start up
	HandleStartingFanBackUp();
#endif
}

//rain
BOOLEAN LightningEndOfTurn( UINT8 ubTeam );
//end rain

void BeginTeamTurn( UINT8 ubTeam )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"BeginTeamTurn");
	SOLDIERTYPE		*pSoldier;

	//rain
	if( !LightningEndOfTurn( ubTeam ) )return;
	//end rain

	// disable for our turn and enable for other teams
	if ( gGameSettings.fOptions[TOPTION_AUTO_FAST_FORWARD_MODE] )
	{
		if (is_networked)
		{
			// Only allow fast forward mode on enemy team!
			SetFastForwardMode( (ubTeam == ENEMY_TEAM) );
		}
		else
		{
			// Allow fast forward mode on all teams except our team!
			SetFastForwardMode( (ubTeam != OUR_TEAM) );
		}		
	}

	while( 1 )
	{
		if ( ubTeam > LAST_TEAM )
		{
			if ( HandleAirRaidEndTurn( ubTeam ) )
			{
				// End turn!!
				ubTeam = gbPlayerNum;
				gTacticalStatus.ubCurrentTeam = gbPlayerNum;
				EndTurnEvents();
				if(is_server)
				{
					numreadyteams =0;//beginning round 
					memset( &readyteamreg , 0 , sizeof (int) * 10);
				}
			}
			else
			{
				break;
			}
		}
		else if (!(gTacticalStatus.Team[ ubTeam ].bTeamActive))
		{
			// inactive team, skip to the next one
			ubTeam++;
			gTacticalStatus.ubCurrentTeam++;
			// skip back to the top, as we are processing another team now.
			continue;
		}
		if ((!(gTacticalStatus.Team[ ubTeam ].bTeamActive))&& is_networked)
		{
			// inactive team, skip to the next one
			ubTeam++;
			gTacticalStatus.ubCurrentTeam++;
			// skip back to the top, as we are processing another team now.
			continue;
		}

		if ( gTacticalStatus.uiFlags & TURNBASED )
		{
			BeginLoggingForBleedMeToos( TRUE );

			// decay team's public opplist
			DecayPublicOpplist( ubTeam );

			SoldierID id = gTacticalStatus.Team[ ubTeam ].bFirstID;
			for ( ; id <= gTacticalStatus.Team[ ubTeam ].bLastID; ++id )
			{
				pSoldier = id;
				if ( pSoldier->bActive && pSoldier->stats.bLife > 0)
				{
					// decay personal opplist, and refresh APs and BPs
					pSoldier->EVENT_BeginMercTurn( FALSE, 0 );
				}
			}

			if (gTacticalStatus.bBoxingState == LOST_ROUND || gTacticalStatus.bBoxingState == WON_ROUND || gTacticalStatus.bBoxingState == DISQUALIFIED )
			{
				// we have no business being in here any more!
				return;
			}

			BeginLoggingForBleedMeToos( FALSE );

		}

		RecalculateSoldiersAniSpeed();

		if (ubTeam == gbPlayerNum )
		{
			// ATE: Check if we are still in a valid battle...
			// ( they could have blead to death above )
			if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				StartPlayerTeamTurn( TRUE, FALSE );
			/*	if(is_server && !net_turn) send_EndTurn(ubTeam);
				if(net_turn == true) net_turn = false;*/
				if(is_server) 
				{
					numreadyteams =0;//beginning round 
					memset( &readyteamreg , 0 , sizeof (int) * 10);
					send_EndTurn(netbTeam);
				}
			}
			break;
		}
		else if (ubTeam > 4 || (is_client && !is_server )) //hayden
		{
			
			InitEnemyUIBar( 0, 0 );
			fInterfacePanelDirty = DIRTYLEVEL2;
			AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ ubTeam ] );
			/*if(is_server && !net_turn) send_EndTurn(ubTeam);
			if(net_turn == true) net_turn = false;*/
			gTacticalStatus.ubCurrentTeam = ubTeam;
			if(is_server) send_EndTurn(ubTeam);
			
			
			
			//return;
			break;
		}
		else
		{
#ifdef NETWORKED
			// Only the host should do this
			if(!gfAmIHost)
				break;
#endif
				if( is_client && !is_server ) //hayden //disable independant client AI
					break;


			// Set First enemy merc to AI control	
			if ( BuildAIListForTeam( ubTeam ) )
			{

				SoldierID ubID = RemoveFirstAIListEntry();
				if (ubID != NOBODY)
				{
					// Dirty panel interface!
					fInterfacePanelDirty = DIRTYLEVEL2;
					if ( ubTeam == CREATURE_TEAM && BloodcatsPresent() )
					{
						AddTopMessage( COMPUTER_TURN_MESSAGE, Message[ STR_BLOODCATS_TURN ] );
					}
					else
					{
						AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ ubTeam ] );
					}
					StartNPCAI( ubID );
					/*if(is_server && !net_turn) send_EndTurn(ubTeam);
					if(net_turn == true) net_turn = false;*/
					if(is_server) send_EndTurn(ubTeam);
					
					return;
				}
			}

			// This team is dead/inactive/being skipped in boxing
			// skip back to the top to process the next team
			ubTeam++;
			gTacticalStatus.ubCurrentTeam++;
		}
	}
}

void DisplayHiddenInterrupt( SOLDIERTYPE * pSoldier )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"DisplayHiddenInterrupt");
	// If the AI got an interrupt but this has been hidden from the player until this point,
	// this code will display the interrupt

	if (!gfHiddenInterrupt)
	{
		return;
	}
	EndDeadlockMsg( );

	if (pSoldier->bVisible != -1 )
	{
		SlideTo( pSoldier->ubID, SETLOCATOR);
	}

		if(is_client)
	{
		guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
	}
	else 
	{
		guiPendingOverrideEvent = LU_BEGINUILOCK;
	}

	// Dirty panel interface!
	fInterfacePanelDirty = DIRTYLEVEL2;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"about to call ErasePath");
	// Erase path!
	ErasePath( TRUE );

	// Reset flags
	gfPlotNewMovement = TRUE;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"about to call AdjustNoAPToFinishMove");
	// Stop our guy....
	MercPtrs[ LATEST_INTERRUPT_GUY ]->AdjustNoAPToFinishMove( TRUE );
	// Stop him from going to prone position if doing a turn while prone
	MercPtrs[ LATEST_INTERRUPT_GUY ]->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"about to call AddTopMessage");
	// get rid of any old overlay message
	if ( pSoldier->bTeam == MILITIA_TEAM )
	{
		AddTopMessage( MILITIA_INTERRUPT_MESSAGE, Message[ STR_INTERRUPT ] );
	}
	else
	{
		AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, Message[ STR_INTERRUPT ] );
	}

	gfHiddenInterrupt = FALSE;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"DisplayHiddenInterrupt completed");
}

void DisplayHiddenTurnbased( SOLDIERTYPE * pActingSoldier )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"DisplayHiddenTurnBased");
	// This code should put the game in turn-based and give control to the AI-controlled soldier
	// whose pointer has been passed in as an argument (we were in non-combat and the AI is doing
	// something visible, i.e. making an attack)
#ifdef JA2UB
//Ja25 No meanwhiles
#else
	if ( AreInMeanwhile( ) )
	{
		return;
	}
#endif
	if (gTacticalStatus.uiFlags & REALTIME || gTacticalStatus.uiFlags & INCOMBAT)
	{
		// pointless call here; do nothing
		return;
	}

	// Enter combat mode starting with this side's turn
	gTacticalStatus.ubCurrentTeam	= pActingSoldier->bTeam;

	CommonEnterCombatModeCode( );

	//JA2Gold: use function to make sure flags turned off everywhere else
	//pActingSoldier->flags.uiStatusFlags |= SOLDIER_UNDERAICONTROL;
	pActingSoldier->SetSoldierAsUnderAiControl(	);
	DebugAI( String( "Giving AI control to %d", pActingSoldier->ubID ) );
	pActingSoldier->flags.fTurnInProgress = TRUE;
	gTacticalStatus.uiTimeSinceMercAIStart = GetJA2Clock();	
	gtTimeSinceMercAIStart = time(0);	// sevenfm: also remember system time

	if ( gTacticalStatus.ubTopMessageType != COMPUTER_TURN_MESSAGE)
	{
		// Dirty panel interface!
		fInterfacePanelDirty = DIRTYLEVEL2;
		if ( gTacticalStatus.ubCurrentTeam == CREATURE_TEAM && BloodcatsPresent() )
		{
			AddTopMessage( COMPUTER_TURN_MESSAGE, Message[ STR_BLOODCATS_TURN ] );
		}
		else
		{
			AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ gTacticalStatus.ubCurrentTeam ] );
		}

	}


	// freeze the user's interface
	FreezeInterfaceForEnemyTurn();
}

BOOLEAN EveryoneInInterruptListOnSameTeam( void )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EveryoneInInterruptListOnSameTeam");
	UINT16 ubLoop;
	UINT8	ubTeam = 255;

	for (ubLoop = 1; ubLoop <= gubOutOfTurnPersons; ubLoop++)
	{
		if ( ubTeam == 255 )
		{
			ubTeam = MercPtrs[ gubOutOfTurnOrder[ ubLoop ] ]->bTeam;
		}
		else
		{
			if ( MercPtrs[ gubOutOfTurnOrder[ ubLoop ] ]->bTeam != ubTeam )
			{
				return( FALSE );
			}
		}
	}
	return( TRUE );
}

void StartInterrupt( void )
{
	INT8			bTeam;
	SOLDIERTYPE *pSoldier;
	SOLDIERTYPE *pTempSoldier;
	SoldierID 	ubFirstInterrupter;
	SoldierID 	ubInterrupter;
	INT32		cnt;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"StartInterrupt");
	ubFirstInterrupter = LATEST_INTERRUPT_GUY;
	AssertLE(ubFirstInterrupter, MAX_NUM_SOLDIERS);
	pSoldier = MercPtrs[ubFirstInterrupter];
	AssertNotNIL(pSoldier);
	bTeam = pSoldier->bTeam;
	ubInterrupter = ubFirstInterrupter;

#ifdef _DEBUG
	// display everyone on int queue!
	for ( cnt = gubOutOfTurnPersons; cnt > 0; cnt-- )
	{
		DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("STARTINT:	Q position %d: %d", cnt, gubOutOfTurnOrder[ cnt ] ) );
	}
#endif

	//DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: %d is now on top of the interrupt queue", ubFirstInterrupter ) );

	gTacticalStatus.fInterruptOccurred = TRUE;

	for ( SoldierID id = 0; id < MAX_NUM_SOLDIERS; ++id )
	{
		pTempSoldier = id;
		if ( pTempSoldier->bActive )
		{
			pTempSoldier->bMovedPriorToInterrupt = pTempSoldier->aiData.bMoved;
			pTempSoldier->aiData.bMoved = TRUE;
		}
	}

	if (pSoldier->bTeam == OUR_TEAM)
	{
		// start interrupts for everyone on our side at once
		CHAR16		sTemp[ 255 ];
		UINT8		ubInterrupters = 0;
		INT32		iSquad, iCounter;

		// disable ff mode
		SetFastForwardMode(FALSE);
		SetClockSpeedPercent(gGameExternalOptions.fClockSpeedPercent);	// sevenfm: set default clock speed

		// build string for display of who gets interrupt
		//while( 1 )
		for( iCounter = 0; iCounter < MAX_NUM_SOLDIERS; iCounter++ )
		{
			ubInterrupter->aiData.bMoved = FALSE;
			DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: popping %d off of the interrupt queue", ubInterrupter ) );

			REMOVE_LATEST_INTERRUPT_GUY();
			// now LATEST_INTERRUPT_GUY is the guy before the previous
			ubInterrupter = LATEST_INTERRUPT_GUY;

			if (ubInterrupter == NOBODY) // previously emptied slot!
			{
				continue;
			}
			else if (ubInterrupter->bTeam != bTeam)
			{
				break;
			}
		}

		// TODO: check here to see if we really can do anything with available mercs on this team
		//   if not then end interrupt.  Probably should check if actions for for the interrupter but then we
		//   would lose potential XP for being able to radio rest of team about enemy from the interrupt 
		// Theoretically ubInterrupter is enemy causing interrupt


		BOOL handleInterrupt = TRUE;
		if (ubInterrupter != NOBODY)
		{
			handleInterrupt = FALSE;

			// build string in separate loop here, want to linearly process squads...
			SOLDIERTYPE *pInterruptedSoldier = ubInterrupter;
			for ( iSquad = 0; iSquad < NUMBER_OF_SQUADS; iSquad++ )
			{
				for ( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
				{
					pTempSoldier = Squad[ iSquad ][ iCounter ];
					if ( pTempSoldier && pTempSoldier->bActive && pTempSoldier->bInSector && !pTempSoldier->aiData.bMoved )
					{
						INT16 ubMinAPcost = MinAPsToAttack(pSoldier,pInterruptedSoldier->sGridNo,ADDTURNCOST, 0);
						// if we don't have enough APs left to shoot even a snap-shot at this guy
						if (ubMinAPcost < pSoldier->bActionPoints)
						{
							handleInterrupt = TRUE;
						}
					}
				}
			}
		}
		if (!handleInterrupt)
		{
			// no mercs can take even a snapshot at the guy
			EndInterrupt(TRUE);
		}
		else
		{
			wcscpy( sTemp, Message[ STR_INTERRUPT_FOR ] );

			// build string in separate loop here, want to linearly process squads...
			ubInterrupters = 0;
			for ( iSquad = 0; iSquad < NUMBER_OF_SQUADS; iSquad++ )
			{
				for ( iCounter = 0; iCounter < NUMBER_OF_SOLDIERS_PER_SQUAD; iCounter++ )
				{
					pTempSoldier = Squad[ iSquad ][ iCounter ];
					if ( pTempSoldier && pTempSoldier->bActive && pTempSoldier->bInSector && !pTempSoldier->aiData.bMoved )
					{
						// then this guy got an interrupt...
						ubInterrupters++;
						if ( ubInterrupters > 6 )
						{
							// flush... display string, then clear it (we could have 20 names!)
							// add comma to end, we know we have another person after this...
							wcscat( sTemp, L", " );
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,	sTemp );
							wcscpy( sTemp, L"" );
							ubInterrupters = 1;
						}

						if ( ubInterrupters > 1 )
						{
							wcscat( sTemp, L", " );
						}
						wcscat( sTemp, pTempSoldier->name );
					}
				}
			}

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,	sTemp );

			DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: starting interrupt for %d", ubFirstInterrupter ) );
			// gusSelectedSoldier should become the topmost guy on the interrupt list
			//gusSelectedSoldier = ubFirstInterrupter;

			// Remove deadlock message
			EndDeadlockMsg( );

			// Select guy....
			SelectSoldier( ubFirstInterrupter, TRUE, TRUE );

			// ATE; Slide to guy who got interrupted!
			SlideTo( gubLastInterruptedGuy, SETLOCATOR);

			// Dirty panel interface!
			fInterfacePanelDirty						= DIRTYLEVEL2;
			gTacticalStatus.ubCurrentTeam	= pSoldier->bTeam;

			// Signal UI done enemy's turn
			guiPendingOverrideEvent = LU_ENDUILOCK;
			
			if (is_networked)
				guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
			
			HandleTacticalUI( );

			InitPlayerUIBar( TRUE );
			//AddTopMessage( PLAYER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );

			PlayJA2Sample( ENDTURN_1, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );

			// report any close call quotes for us here
			for ( SoldierID id = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; id <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++id )
			{
				SOLDIERTYPE *pSoldier = id;

				if ( OK_INSECTOR_MERC( pSoldier ) )
				{
					if ( pSoldier->flags.fCloseCall )
					{
						if ( pSoldier->bNumHitsThisTurn == 0 && !(pSoldier->usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL) && Random( 3 ) == 0 )
						{
							// say close call quote!
							TacticalCharacterDialogue( pSoldier, QUOTE_CLOSE_CALL );
							pSoldier->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL;
						}
						pSoldier->flags.fCloseCall = FALSE;
					}
				}
			}
		}
	}
	else
	{
		// start interrupts for everyone on that side at once... and start AI with the lowest # guy

		// what we do is set everyone to moved except for people with interrupts at the moment
		/*
		cnt = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID;
		for ( pTempSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTempSoldier++)
		{
			if ( pTempSoldier->bActive )
			{
				pTempSoldier->bMovedPriorToInterrupt = pTempSoldier->aiData.bMoved;
				pTempSoldier->aiData.bMoved = TRUE;
			}
		}
		*/

		//while( 1 )
		UINT16 usCounter;
		for( usCounter = 0; usCounter < MAX_NUM_SOLDIERS; usCounter++ )
		{
			ubInterrupter->aiData.bMoved = FALSE;

			DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: popping %d off of the interrupt queue", ubInterrupter ) );

			REMOVE_LATEST_INTERRUPT_GUY();
			// now LATEST_INTERRUPT_GUY is the guy before the previous
			ubInterrupter = LATEST_INTERRUPT_GUY;
			if (ubInterrupter == NOBODY) // previously emptied slot!
			{
				continue;
			}
			else if (ubInterrupter->bTeam != bTeam)
			{
				break;
			}
			else if (ubInterrupter < ubFirstInterrupter)
			{
				ubFirstInterrupter = ubInterrupter;
			}
		}

		// here we have to rebuilt the AI list!
		BuildAIListForTeam( bTeam );

		// set to the new first interrupter
		SoldierID id = RemoveFirstAIListEntry();

		// sevenfm: RemoveFirstAIListEntry() can return NOBODY
		if( id != NOBODY )
		{
			pTempSoldier = id;

			// sevenfm: don't do anything if pTempSoldier is NULL
			if( pTempSoldier != NULL )
			{
				// SANDRO - we don't use the "hidden interrupt" feature with IIS
				// sevenfm: all interrupts in original interrupt system start as hidden and revealed later if soldier decides something
				if ( !is_networked && pTempSoldier->bTeam != OUR_TEAM && !UsingImprovedInterruptSystem() )
				{
					// we're being interrupted by the computer!
					// we delay displaying any interrupt message until the computer does something...
					gfHiddenInterrupt = TRUE;
					gTacticalStatus.fUnLockUIAfterHiddenInterrupt = FALSE;
				}
				// otherwise it's the AI interrupting another AI team

				gTacticalStatus.ubCurrentTeam	= pTempSoldier->bTeam;

#ifdef JA2BETAVERSION
				if (is_networked)
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Interrupt ( could be hidden )" );
				}
#endif

				// sevenfm: don't show hidden interrupt
				if( !gfHiddenInterrupt )
				{
					// SANDRO - show correct top message
					if (pTempSoldier->bTeam == MILITIA_TEAM )
						AddTopMessage( MILITIA_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
					else
						AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
				}

				// Flugente 12-11-13: I observed an instance where the pTempSoldier was a player merc, leading to a deadlock here. The reason was that during an interrupt by a civilian, he somehow did not win
				// instead the game used the last merc entry... which overflowed, and thus started at merc 0, which is always a player merc
				// I am not sure if this is the best solution... however it seems to work for me.
				// If anybody knows a better solution, feel free to do so
				if ( pTempSoldier->bTeam != OUR_TEAM )
					StartNPCAI( pTempSoldier );
				else
					EndInterrupt(TRUE);
			}
		}
	}

	if ( !gfHiddenInterrupt )
	{
		// Stop this guy....
		if ( LATEST_INTERRUPT_GUY != END_OF_INTERRUPTS // BOB: is this just a blank?
			&& LATEST_INTERRUPT_GUY <= TOTAL_SOLDIERS  // BOB: sanity check
			&& MercPtrs[LATEST_INTERRUPT_GUY]->exists()	//MM: this was crashing if the LATEST_INTERRUPT_GUY wasn't set
			)
		{
			MercPtrs[ LATEST_INTERRUPT_GUY ]->AdjustNoAPToFinishMove( TRUE );
			MercPtrs[ LATEST_INTERRUPT_GUY ]->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
		}
	}

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"StartInterrupt done");
}

void EndInterrupt( BOOLEAN fMarkInterruptOccurred )
{
	SoldierID	ubInterruptedSoldier;
	SOLDIERTYPE *pSoldier;
	SOLDIERTYPE *pTempSoldier;
	BOOLEAN		fFound;
	INT16	ubMinAPsToAttack;

	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"EndInterrupt");

	for ( UINT16 cnt = gubOutOfTurnPersons; cnt > 0; cnt-- )
	{
		DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("ENDINT:	Q position %d: %d", cnt, gubOutOfTurnOrder[ cnt ] ) );
	}

	// ATE: OK, now if this all happended on one frame, we may not have to stop
	// guy from walking... so set this flag to false if so...
	if ( fMarkInterruptOccurred )
	{
		// flag as true if an int occurs which ends an interrupt (int loop)
		gTacticalStatus.fInterruptOccurred = TRUE;
	}
	else
	{
		gTacticalStatus.fInterruptOccurred = FALSE;
	}

	// Loop through all mercs and see if any passed on this interrupt
	SoldierID id = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID;
	for ( ; id <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++id )
	{
		pTempSoldier = id;
		if ( pTempSoldier->bActive && pTempSoldier->bInSector && !pTempSoldier->aiData.bMoved && (pTempSoldier->bActionPoints == pTempSoldier->aiData.bIntStartAPs))
		{
			ubMinAPsToAttack = MinAPsToAttack( pTempSoldier, pTempSoldier->sLastTarget, FALSE, 0 );
			if ( (ubMinAPsToAttack <= pTempSoldier->bActionPoints) && (ubMinAPsToAttack > 0) )
			{
				pTempSoldier->aiData.bPassedLastInterrupt = TRUE;
			}
		}
	}

	if ( !EveryoneInInterruptListOnSameTeam() )
	{
		gfHiddenInterrupt = FALSE;

		// resume interrupted interrupt
		//hayden
		if ( !is_networked )
		{
			StartInterrupt();
		}
		else
		{
			SoldierID 	nubFirstInterrupter;
			INT8			nbTeam;
			SOLDIERTYPE *npSoldier;

			nubFirstInterrupter = LATEST_INTERRUPT_GUY;
			npSoldier = nubFirstInterrupter;
			nbTeam = npSoldier->bTeam;

			//pSoldier is interrupted //but its not available //needs calculating
			//npSoldier,nbTeam is interruptor
			//hayden

#ifdef BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"unchecked interrupt call area:(resume interrupted interrupt)..." );
#endif

			if ( (nbTeam > 0) && (nbTeam < 6) && is_server ) // AI interrupt resume and im server
			{
				send_interrupt( npSoldier );
				StartInterrupt();
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Continuing interrupt with %s and AI", TeamNameStrings[npSoldier->bTeam] );//tried to use pSoldier, but its not available. find another way to get correct team

			}
			else if ( is_server && gTacticalStatus.ubCurrentTeam == 1 )// resume AI interrupted and im server
			{
				//hayden
				send_interrupt( npSoldier );

				if ( nbTeam != 0 )
					intAI( npSoldier );
				else
					StartInterrupt();

				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Continuing interrupt of AI by %s", TeamNameStrings[npSoldier->bTeam] );

			}

#ifdef	INTERRUPT_MP_DEADLOCK_FIX
			//its our turn//else// pure client awarding interrupt resume //its our turn
			else if ( gTacticalStatus.ubCurrentTeam == 0 )
#else
			// pure client awarding interrupt resume
			else
#endif
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Continuing interrupt with %s", TeamNameStrings[npSoldier->bTeam] );//this can be simplified if above comment is implemented
				//ClearIntList();
				//hayden//may need more work.
				StartInterrupt();
				send_interrupt( npSoldier ); //
			}
		}
	}
	else
	{
		ubInterruptedSoldier = LATEST_INTERRUPT_GUY;

		DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: interrupt over, %d's team regains control", ubInterruptedSoldier ) );

		pSoldier = ubInterruptedSoldier;

		for ( SoldierID id = 0; id < MAX_NUM_SOLDIERS; ++id)
		{
			pTempSoldier = id;
			if ( pTempSoldier->bActive )
			{
				// AI guys only here...
				if ( pTempSoldier->bActionPoints == 0 )
				{
					pTempSoldier->aiData.bMoved = TRUE;
				}
				else if ( pTempSoldier->bTeam != gbPlayerNum && pTempSoldier->aiData.bNewSituation == IS_NEW_SITUATION )
				{
					pTempSoldier->aiData.bMoved = FALSE;
				}
				else
				{
					pTempSoldier->aiData.bMoved = pTempSoldier->bMovedPriorToInterrupt;
				}
			}
		}


		// change team
		gTacticalStatus.ubCurrentTeam	= pSoldier->bTeam;

		// switch appropriate messages & flags
		if ( pSoldier->bTeam == OUR_TEAM)
		{
			// set everyone on the team to however they were set moved before the interrupt
			// must do this before selecting soldier...
			/*
			cnt = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID;
			for ( pTempSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; cnt++,pTempSoldier++)
			{
				if ( pTempSoldier->bActive )
				{
					pTempSoldier->aiData.bMoved = pTempSoldier->bMovedPriorToInterrupt;
				}
			}
			*/

			ClearIntList();

			// Select soldier....
			if ( MercPtrs[ ubInterruptedSoldier ]->stats.bLife < OKLIFE )
			{
				DebugMsg(TOPIC_JA2INTERRUPT,DBG_LEVEL_3,String("EndInterrupt: SelectNextAvailSoldier"));
				SelectNextAvailSoldier( MercPtrs[ ubInterruptedSoldier ] );
			}
			else
			{
				SelectSoldier( ubInterruptedSoldier, FALSE, FALSE );
			}

			if (gfHiddenInterrupt)
			{
				// Try to make things look like nothing happened at all.
				gfHiddenInterrupt = FALSE;

				// If we can continue a move, do so!
				if ( gusSelectedSoldier->flags.fNoAPToFinishMove && pSoldier->ubReasonCantFinishMove != REASON_STOPPED_SIGHT )
				{
					// Continue
					gusSelectedSoldier->AdjustNoAPToFinishMove( FALSE );

					if ( gusSelectedSoldier->sGridNo != gusSelectedSoldier->pathing.sFinalDestination )
					{
						gusSelectedSoldier->EVENT_GetNewSoldierPath( gusSelectedSoldier->pathing.sFinalDestination, gusSelectedSoldier->usUIMovementMode );
					}
					else
					{
						UnSetUIBusy( pSoldier->ubID );
					}
				}
				else
				{
					UnSetUIBusy( pSoldier->ubID );
				}

				if ( gTacticalStatus.fUnLockUIAfterHiddenInterrupt )
				{
					gTacticalStatus.fUnLockUIAfterHiddenInterrupt = FALSE;
					UnSetUIBusy( pSoldier->ubID );
				}
			}
			else
			{
				// Signal UI done enemy's turn
				/// ATE: This used to be ablow so it would get done for
				// both hidden interrupts as well - NOT good because
				// hidden interrupts should leave it locked if it was already...
				guiPendingOverrideEvent = LU_ENDUILOCK;
				
				if (is_networked)
					guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
				
				HandleTacticalUI( );

				if ( gusSelectedSoldier != NOBODY )
				{
					SlideTo( gusSelectedSoldier, SETLOCATOR);

					// Say ATTENTION SOUND...
					gusSelectedSoldier->DoMercBattleSound( BATTLE_SOUND_ATTN1 );

					if ( gsInterfaceLevel == 1 )
					{
						gTacticalStatus.uiFlags |= SHOW_ALL_ROOFS;
						InvalidateWorldRedundency( );
						SetRenderFlags(RENDER_FLAG_FULL);
						ErasePath(FALSE);
					}
				}
				// 2 indicates that we're ending an interrupt and going back to
				// normal player's turn without readjusting time left in turn (for
				// timed turns)
				InitPlayerUIBar( 2 );
				
				// SANDRO - shouldn't we unset ui here too?
				UnSetUIBusy( pSoldier->ubID );
			}

		}
		else if (!is_networked || (pSoldier->bTeam < 6))//hayden : is Ai or LAN ?
		{
			// this could be set to true for AI-vs-AI interrupts
			gfHiddenInterrupt = FALSE;

			// Dirty panel interface!
			fInterfacePanelDirty = DIRTYLEVEL2;

			// Erase path!
			ErasePath( TRUE );

			// Reset flags
			gfPlotNewMovement = TRUE;

			// restart AI with first available soldier
			fFound = FALSE;

			// rebuild list for this team if anyone on the team is still available
			SoldierID id = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
			for ( ; id <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++id )
			{
				pTempSoldier = id;
				if ( pTempSoldier->bActive && pTempSoldier->bInSector && pTempSoldier->stats.bLife >= OKLIFE )
				{
					fFound = TRUE;
					break;
				}
			}

			if ( fFound )
			{
				// reset found flag because we are rebuilding the AI list
				fFound = FALSE;

				if ( BuildAIListForTeam( gTacticalStatus.ubCurrentTeam ) )
				{
					// now bubble up everyone left in the interrupt queue, starting
					// at the front of the array
					for (UINT16 cnt = 1; cnt <= gubOutOfTurnPersons; cnt++)
					{
						MoveToFrontOfAIList( gubOutOfTurnOrder[ cnt ] );
					}

					SoldierID id = RemoveFirstAIListEntry();
					if (id != NOBODY)
					{
						fFound = TRUE;
						StartNPCAI( id );
					}
				}

			}

			if (fFound)
			{
				// back to the computer!
				if ( gTacticalStatus.ubCurrentTeam == CREATURE_TEAM && BloodcatsPresent() )
				{
					AddTopMessage( COMPUTER_TURN_MESSAGE, Message[ STR_BLOODCATS_TURN ] );
				}
				else
				{
					AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ gTacticalStatus.ubCurrentTeam ] );
				}

				// Signal UI done enemy's turn
				if(is_client)
				{
					guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
				}
				else 
				{
					guiPendingOverrideEvent = LU_BEGINUILOCK;
				}

				ClearIntList();
			}
			else
			{
				// back to the computer!
				if ( gTacticalStatus.ubCurrentTeam == CREATURE_TEAM && BloodcatsPresent() )
				{
					AddTopMessage( COMPUTER_TURN_MESSAGE, Message[ STR_BLOODCATS_TURN ] );
				}
				else
				{
					AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ gTacticalStatus.ubCurrentTeam ] );
				}

				// Signal UI done enemy's turn
					if(is_client)
					{
						guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
					}
					else 
					{
						guiPendingOverrideEvent = LU_BEGINUILOCK;
					}

				// must clear int list before ending turn
				ClearIntList();
				EndAITurn();
			}
		}

		else if (is_networked) //its going to another Lan client..//hayden
		{
	
			gTacticalStatus.ubCurrentTeam = pSoldier->bTeam;
			AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ gTacticalStatus.ubCurrentTeam ] );
			if(is_client)
			{
				guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
			}
			else 
			{
				guiPendingOverrideEvent = LU_BEGINUILOCK;
			}

			// must clear int list before ending turn
			ClearIntList();

		}
		// Reset our interface!
		fInterfacePanelDirty = DIRTYLEVEL2;

	}

	if ( gGameSettings.fOptions[TOPTION_AUTO_FAST_FORWARD_MODE] )
	{
		if (is_networked)
		{
			// Only allow fast forward mode on enemy team!
			SetFastForwardMode( (gTacticalStatus.ubCurrentTeam == ENEMY_TEAM) );
		}
		else
		{
			// Allow fast forward mode on all teams except our team!
			SetFastForwardMode( (gTacticalStatus.ubCurrentTeam != OUR_TEAM) );
		}	
	}
}


BOOLEAN StandardInterruptConditionsMet( SOLDIERTYPE * pSoldier, SoldierID ubOpponentID, INT8 bOldOppList)
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"StandardInterruptConditionsMet");
//	UINT8 ubAniType;
	INT16						ubMinPtsNeeded;
	INT8						bDir;
	SOLDIERTYPE *		pOpponent;

#ifdef DISABLE_MP_INTERRUPTS_IN_COOP
	
	if (is_networked == TRUE && cGameType == MP_TYPE_COOP)
	{
		return ( FALSE );
	}

#endif

	if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && !(gubSightFlags & SIGHT_INTERRUPT) )
	{
		return( FALSE );
	}

	if ( gTacticalStatus.ubAttackBusyCount > 0 )
	{
		return( FALSE );
	}

	if (ubOpponentID < TOTAL_SOLDIERS)
	{
		/*
		// only the OPPONENT'S controller's decision matters
		if (Menptr[ubOpponentID].controller != Net.pnum)
		{
			return(FALSE);
		}
		*/

		// ALEX
		// if interrupts are restricted to a particular opponent only & he's not it
		if ((InterruptOnlyGuynum != NOBODY) && (ubOpponentID != InterruptOnlyGuynum))
		{
			return(FALSE);
		}

		pOpponent = ubOpponentID;
	}
	else	// no opponent, so controller of 'ptr' makes the call instead
	{
		// ALEX
		if (gsWhoThrewRock >= TOTAL_SOLDIERS)
		{
#ifdef BETAVERSION
			NumMessage("StandardInterruptConditions: ERROR - ubOpponentID is NOBODY, don't know who threw rock, guynum = ",pSoldier->guynum);
#endif

			return(FALSE);
		}

		// the machine that controls the guy who threw the rock makes the decision
		/*
		if (Menptr[WhoThrewRock].controller != Net.pnum)
			return(FALSE);
		*/
		pOpponent = NULL;
	}

	// if interrupts have been disabled for any reason
	if (!InterruptsAllowed)
	{
		return(FALSE);
	}

	// in non-combat allow interrupt points to be calculated freely (everyone's in control!)
	// also allow calculation for storing in AllTeamsLookForAll
	if ( (gTacticalStatus.uiFlags & INCOMBAT) && ( gubBestToMakeSightingSize != BEST_SIGHTING_ARRAY_SIZE_ALL_TEAMS_LOOK_FOR_ALL ) )
	{
		// if his team's already in control
		if (pSoldier->bTeam == gTacticalStatus.ubCurrentTeam )
		{
			// if this is a player's a merc or civilian
			if ((pSoldier->flags.uiStatusFlags & SOLDIER_PC) || PTR_CIVILIAN)
			{
				// then they are not allowed to interrupt their own team
				return(FALSE);
			}
			else
			{
				// enemies, MAY interrupt each other, but NOT themselves!
				//if ( pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL )
				//{
					return(FALSE);
				//}
			}

		// CJC, July 9 1998
			// NO ONE EVER interrupts his own team
			//return( FALSE );
		}
		else if ( gTacticalStatus.bBoxingState != NOT_BOXING )
		{
			// while anything to do with boxing is going on, skip interrupts!
			return( FALSE );
		}

	}

	if ( !(pSoldier->bActive) || !(pSoldier->bInSector ) )
	{
		return( FALSE );
	}

	// soldiers at less than OKLIFE can't perform any actions
	if (pSoldier->stats.bLife < OKLIFE)
	{
		return(FALSE);
	}

	// soldiers out of breath are about to fall over, no interrupt
	if (pSoldier->bBreath < OKBREATH || pSoldier->bCollapsed )
	{
		return(FALSE);
	}

	// if soldier doesn't have enough APs
	if ( pSoldier->bActionPoints < APBPConstants[MIN_APS_TO_INTERRUPT] )
	{
		return( FALSE );
	}

	// soldiers gagging on gas are too busy about holding their cookies down...
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		return(FALSE);
	}

	// a soldier already engaged in a life & death battle is too busy doing his
	// best to survive to worry about "getting the jump" on additional threats
	if (pSoldier->aiData.bUnderFire)
	{
		return(FALSE);
	}

	if (pSoldier->bCollapsed)
	{
		return( FALSE );
	}

	// don't allow neutral folks to get interrupts
	if (pSoldier->aiData.bNeutral)
	{
		return( FALSE );
	}

	// no EPCs allowed to get interrupts
	if ( AM_AN_EPC( pSoldier ) && !AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}


	// don't let mercs on assignment get interrupts
	if ( pSoldier->bTeam == gbPlayerNum && pSoldier->bAssignment >= ON_DUTY)
	{
		return( FALSE );
	}


	// the bare minimum default is enough APs left to TURN
	ubMinPtsNeeded = APBPConstants[AP_CHANGE_FACING];

	// if the opponent is SOMEBODY
	if (ubOpponentID < TOTAL_SOLDIERS)
	{
		// if the soldiers are on the same side
		if (pSoldier->bSide == pOpponent->bSide)
		{
			// human/civilians on same side can't interrupt each other
			if ((pSoldier->flags.uiStatusFlags & SOLDIER_PC) || PTR_CIVILIAN)
			{
				return(FALSE);
			}
			else	// enemy
			{
				// enemies can interrupt EACH OTHER, but enemies and civilians on the
				// same side (but different teams) can't interrupt each other.
				if (pSoldier->bTeam != pOpponent->bTeam)
				{
					return(FALSE);
				}
			}
		}

		// if the interrupted opponent is not the selected character, then the only
		// people eligible to win an interrupt are those on the SAME SIDE AS
		// the selected character, ie. his friends...
		if ( pOpponent->bTeam == gbPlayerNum )
		{
			if ((ubOpponentID != gusSelectedSoldier) && (pSoldier->bSide != gusSelectedSoldier->bSide))
			{
				return( FALSE );
			}
		}
		else
		{
			if (!is_networked) {
				if ( !(pOpponent->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL) && (pSoldier->bSide != pOpponent->bSide))
				{
					return( FALSE );
				}
			} else {
				if ( !(is_client || (pOpponent->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL)) && (pSoldier->bSide != pOpponent->bSide))
				{
					return( FALSE );
				}
			}

		}
		/* old DG code for same:

		if ((ubOpponentID != gusSelectedSoldier) && (pSoldier->bSide != gusSelectedSoldier->bSide))
		{
			return(FALSE);
		}
		*/

		// an non-active soldier can't interrupt a soldier who is also non-active!
		if ((pOpponent->bTeam != gTacticalStatus.ubCurrentTeam) && (pSoldier->bTeam != gTacticalStatus.ubCurrentTeam))
		{
			return(FALSE);
		}


		// if this is a "SEEING" interrupt
		if (pSoldier->aiData.bOppList[ubOpponentID] == SEEN_CURRENTLY)
		{
			// if pSoldier already saw the opponent last "look" or at least this turn
			if ((bOldOppList == SEEN_CURRENTLY) || (bOldOppList == SEEN_THIS_TURN))
			{
				return(FALSE);	 // no interrupt is possible
			}

			// if the soldier is behind him and not very close, forget it
			bDir = atan8( pSoldier->sX, pSoldier->sY, pOpponent->sX, pOpponent->sY );
			if ( gOppositeDirection[ pSoldier->pathing.bDesiredDirection ] == bDir )
			{
				// directly behind; allow interrupts only within # of tiles equal to level
				if ( PythSpacesAway( pSoldier->sGridNo, pOpponent->sGridNo ) > EffectiveExpLevel( pSoldier ) )
				{
					return( FALSE );
				}
			}

			// if the soldier isn't currently crouching
			if (!PTR_CROUCHED)
			{
				ubMinPtsNeeded = GetAPsCrouch(pSoldier, TRUE); // Changed from APBPConstants[AP_CROUCH] - SANDRO
			}
			else
			{
				ubMinPtsNeeded = MinPtsToMove(pSoldier);
			}
		}
		else	// this is a "HEARING" interrupt
		{
			// if the opponent can't see the "interrupter" either, OR
			// if the "interrupter" already has any opponents already in sight, OR
			// if the "interrupter" already heard the active soldier this turn
			if ((pOpponent->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY) || (pSoldier->aiData.bOppCnt > 0) || (bOldOppList == HEARD_THIS_TURN))
			{
				return(FALSE);	 // no interrupt is possible
			}
		}
	}


	// soldiers without sufficient APs to do something productive can't interrupt
	if (pSoldier->bActionPoints < ubMinPtsNeeded)
	{
		return(FALSE);
	}

	// soldier passed on the chance to react during previous interrupt this turn
	if (pSoldier->aiData.bPassedLastInterrupt)
	{
#ifdef RECORDNET
		fprintf(NetDebugFile,"\tStandardInterruptConditionsMet: FAILING because PassedLastInterrupt %d(%s)\n",
			pSoldier->guynum,ExtMen[pSoldier->guynum].name);
#endif

//		return(FALSE);
	}


#ifdef RECORDINTERRUPT
	// this usually starts a new series of logs, so that's why the blank line
	fprintf(InterruptFile,"\nStandardInterruptConditionsMet by %d vs. %d\n",pSoldier->guynum,ubOpponentID);
#endif

	return(TRUE);
}


INT8 CalcInterruptDuelPts( SOLDIERTYPE * pSoldier, SoldierID ubOpponentID, BOOLEAN fUseWatchSpots )
{
	INT32 iPoints;
	INT8 bLightLevel;
	UINT8	ubDistance;
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"CalcInterruptDuelPts");

	// sevenfm: safety check
	Assert(pSoldier);

	// extra check to make sure neutral folks never get interrupts
	if (pSoldier->aiData.bNeutral)
	{
		return( NO_INTERRUPT );
	}

	// Old: BASE is one point for each experience level.
	// Snap: Agility should be a factor, since it is a measure of
	// a person's reactions.	We'll give more weight to experience
	// though, since combat initiative is too important to be left up
	// to an ordinary skill like agility.
	// BASE = (2*lev + agi/10) / 3
	// Robot has interrupt points based on the controller...
	// Controller's interrupt points are reduced by 2 for being distracted...
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT && pSoldier->CanRobotBeControlled( ) )
	{
		//iPoints = EffectiveExpLevel( MercPtrs[ pSoldier->ubRobotRemoteHolderID ] ) - 2;
		// Snap: (do some proper rounding here)
		iPoints = ( 20*EffectiveExpLevel( pSoldier->ubRobotRemoteHolderID )
			+ EffectiveAgility( pSoldier->ubRobotRemoteHolderID, FALSE ) + 15 ) / 30 - 2;
	}
	else
	{
		//iPoints = EffectiveExpLevel( pSoldier );
		// Snap:
		iPoints = ( 20*EffectiveExpLevel( pSoldier ) + EffectiveAgility( pSoldier, FALSE ) + 15 ) / 30;

		/*
		if ( pSoldier->bTeam == ENEMY_TEAM )
		{
			// modify by the difficulty level setting
			iPoints += gbDiff[ DIFF_ENEMY_INTERRUPT_MOD ][ SoldierDifficultyLevel( pSoldier ) ];
			iPoints = __max( iPoints, 9 );
		}
		*/

		if ( pSoldier->ControllingRobot( ) )
		{
			iPoints -= 2;
		}
	}

	// sevenfm: no watch spot bonus when focusing
	if (fUseWatchSpots && !(pSoldier->usSoldierFlagMask2 & SOLDIER_TRAIT_FOCUS))
	{
		// if this is a previously noted spot of enemies, give bonus points!
		iPoints += GetWatchedLocPoints( pSoldier->ubID, ubOpponentID->sGridNo, ubOpponentID->pathing.bLevel );
	}

	// LOSE one point for each 2 additional opponents he currently sees, above 2
	if (pSoldier->aiData.bOppCnt > 2)
	{
		// subtract 1 here so there is a penalty of 1 for seeing 3 enemies
		iPoints -= (pSoldier->aiData.bOppCnt - 1) / 2;
	}

	// LOSE one point if he's trying to interrupt only by hearing
	if (pSoldier->aiData.bOppList[ubOpponentID] == HEARD_THIS_TURN)
	{
		iPoints--;
	}

	//hayden, multiplayer add advantage for a ready'd reapon
	if(is_networked)
	{
		if ( ( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ))
		{
			iPoints=(iPoints + cWeaponReadyBonus);
			
		}
	}

	// if soldier is still in shock from recent injuries, that penalizes him
	iPoints -= pSoldier->aiData.bShock;

	ubDistance = (UINT8) PythSpacesAway( pSoldier->sGridNo, ubOpponentID->sGridNo );

	// if we are in combat mode - thus doing an interrupt rather than determine who gets first turn -
	// then give bonus
	if ( (gTacticalStatus.uiFlags & INCOMBAT) && (pSoldier->bTeam != gTacticalStatus.ubCurrentTeam) )
	{
		// passive player gets penalty due to range
		iPoints -= (ubDistance / 10);
	}
	else
	{
		// either non-combat or the player with the current turn... i.e. active...
		// unfortunately we can't use opplist here to record whether or not we saw this guy before, because at this point
		// the opplist has been updated to seen.	But we can use gbSeenOpponents ...

		// this soldier is moving, so give them a bonus for crawling or swatting at long distances
		if ( !gbSeenOpponents[ ubOpponentID ][ pSoldier->ubID ] )
		{
			if (pSoldier->usAnimState == SWATTING && ubDistance > (MaxNormalDistanceVisible() / 2) ) // more than 1/2 sight distance
			{
				iPoints++;
			}
			else if (pSoldier->usAnimState == CRAWLING && ubDistance > (MaxNormalDistanceVisible() / 4) ) // more than 1/4 sight distance
			{
				iPoints += ubDistance / STRAIGHT;
			}
		}
	}

	// whether active or not, penalize people who are running
	if ( pSoldier->usAnimState == RUNNING && !gbSeenOpponents[ pSoldier->ubID ][ ubOpponentID ] )
	{
		iPoints -= 2;
	}

	if (pSoldier->ubServicePartner != NOBODY)
	{
		// distracted by being bandaged/doing bandaging
		iPoints -= 2;
	}

	if (gGameOptions.fNewTraitSystem) // new/old traits check - SANDRO
	{
		if ( HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
		{
			bLightLevel = LightTrueLevel(pSoldier->sGridNo, pSoldier->pathing.bLevel);
			if (bLightLevel > NORMAL_LIGHTLEVEL_DAY + 3)
			{
				// it's dark, give a bonus for interrupts
				iPoints += gSkillTraitValues.ubNOIterruptsBonusInDark;
			}
		}

		// Phlegmatics get a small penalty to interrupts
		if ( DoesMercHavePersonality( pSoldier, CHAR_TRAIT_PHLEGMATIC ) )
		{
			iPoints -= 1;
		}

		// Flugente: focus skill
		if ( (pSoldier->usSoldierFlagMask2 & SOLDIER_TRAIT_FOCUS) )
		{
			if ( pSoldier->CanUseSkill( SKILLS_FOCUS, FALSE, pSoldier->sFocusGridNo ) )
			{
				// if target is in focus, increase interrupt chance, otherwise lower it
				// radius depends on range
				INT16 range = PythSpacesAway( pSoldier->sFocusGridNo, pSoldier->sGridNo );
				INT16 radius = gSkillTraitValues.ubSNFocusRadius * range / 20;

				INT16 range_opponent = PythSpacesAway( pSoldier->sFocusGridNo, ubOpponentID->sGridNo );

				if ( range_opponent <= radius )
					iPoints += gSkillTraitValues.sSNFocusInterruptBonus;
				else
					iPoints -= 2 * gSkillTraitValues.sSNFocusInterruptBonus;
			}
			else
			{
				pSoldier->usSoldierFlagMask2 &= ~SOLDIER_TRAIT_FOCUS;
				pSoldier->sFocusGridNo = NOWHERE;
			}
		}
	}
	else
	{
		if ( HAS_SKILL_TRAIT( pSoldier, NIGHTOPS_OT ) )
		{
			bLightLevel = LightTrueLevel(pSoldier->sGridNo, pSoldier->pathing.bLevel);
			if (bLightLevel > NORMAL_LIGHTLEVEL_DAY + 3)
			{
				// it's dark, give a bonus for interrupts
				iPoints += 1 * NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT );
			}
		}
	}
		
	// Flugente: interrupt modifier from special stats
	iPoints += pSoldier->GetInterruptModifier( ubDistance );

	// if he's a computer soldier

	// CJC note: this will affect friendly AI as well...

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
	{
		if ( pSoldier->bAssignment >= ON_DUTY )
		{
			// make sure don't get interrupts!
			iPoints = -10;
		}

		// GAIN one point if he's previously seen the opponent
		// check for TRUE because -1 means we JUST saw him (always so here)
		if (gbSeenOpponents[pSoldier->ubID][ubOpponentID] == TRUE)
		{
			iPoints++;	// seen him before, easier to react to him
		}
	}
	else if ( pSoldier->bTeam == ENEMY_TEAM )
	{
		// GAIN one point if he's previously seen the opponent
		// check for TRUE because -1 means we JUST saw him (always so here)
		if (gbSeenOpponents[pSoldier->ubID][ubOpponentID] == TRUE)
		{
			iPoints++;	// seen him before, easier to react to him
		}
		else if (gbPublicOpplist[pSoldier->bTeam][ubOpponentID] != NOT_HEARD_OR_SEEN)
		{
			// GAIN one point if opponent has been recently radioed in by his team
			iPoints++;
		}
	}

	if ( ARMED_VEHICLE( pSoldier ) || ENEMYROBOT( pSoldier ) )
	{
		// reduce interrupt possibilities for tanks!
		iPoints /= 2;
	}

	if (iPoints >= AUTOMATIC_INTERRUPT)
	{
#ifdef BETAVERSION
		NumMessage("CalcInterruptDuelPts: ERROR - Invalid bInterruptDuelPts calculated for soldier ",pSoldier->guynum);
#endif
		iPoints = AUTOMATIC_INTERRUPT - 1;	// hack it to one less than max so its legal
	}

	#ifdef DEBUG_INTERRUPTS
		DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("Calculating int pts for %d vs %d, number is %d", pSoldier->ubID, ubOpponentID, iPoints ) );
	#endif
if(is_networked)
{
	SOLDIERTYPE	*pOpp = ubOpponentID;
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_MPSYSTEM, L"Interrupt: '%s' vs '%s' = %d points.",pSoldier->name,pOpp->name, iPoints );
		#endif
}
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"CalcInterruptDuelPts done");
	return( (INT8)iPoints );
}

BOOLEAN InterruptDuel( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOpponent)
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"InterruptDuel");
	BOOLEAN fResult = FALSE;

	// sevenfm: if Ctrl+D pressed - skip all player interrupts for this turn
	if( !is_networked && !UsingImprovedInterruptSystem() && pSoldier->bTeam == OUR_TEAM && gTacticalStatus.ubDisablePlayerInterrupts )
		return FALSE;

	// if opponent can't currently see us and we can see them
	if ( pSoldier->aiData.bOppList[ pOpponent->ubID ] == SEEN_CURRENTLY && pOpponent->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY )
	{
		fResult = TRUE;		// we automatically interrupt
		// fix up our interrupt duel pts if necessary
		if (pSoldier->aiData.bInterruptDuelPts < pOpponent->aiData.bInterruptDuelPts)
		{
			pSoldier->aiData.bInterruptDuelPts = pOpponent->aiData.bInterruptDuelPts;
		}
	}
	else
	{
		// If our total points is HIGHER, then we interrupt him anyway
		if (pSoldier->aiData.bInterruptDuelPts > pOpponent->aiData.bInterruptDuelPts)
		{
			fResult = TRUE;
		}
	}
//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Interrupt duel %d (%d pts) vs %d (%d pts)", pSoldier->ubID, pSoldier->aiData.bInterruptDuelPts, pOpponent->ubID, pOpponent->aiData.bInterruptDuelPts );
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"InterruptDuel done");
	return( fResult );
}


void DeleteFromIntList( UINT16 ubIndex, BOOLEAN fCommunicate)
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"DeleteFromIntList");
	UINT16 ubLoop;
	UINT16 ubID;

	if ( ubIndex > gubOutOfTurnPersons)
	{
		return;
	}

	// remember who we're getting rid of
	ubID = gubOutOfTurnOrder[ubIndex];

	#ifdef DEBUG_INTERRUPTS
		DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: removing ID %d", ubID ) );
	#endif
//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d removed from int list", ubID );
	// if we're NOT deleting the LAST entry in the int list
	if (ubIndex < gubOutOfTurnPersons)
	{
		// not the last entry, must move all those behind it over to fill the gap
		for (ubLoop = ubIndex; ubLoop < gubOutOfTurnPersons; ubLoop++)
		{
			gubOutOfTurnOrder[ubLoop] = gubOutOfTurnOrder[ubLoop + 1];
		}
	}

	// either way, whack the last entry to NOBODY and decrement the list size
	gubOutOfTurnOrder[gubOutOfTurnPersons] = NOBODY;
	gubOutOfTurnPersons--;

	// once the last interrupted guy gets deleted from the list, he's no longer
	// the last interrupted guy!
	/*
	if (Status.lastInterruptedWas == ubID)
	{
		Status.lastInterruptedWas = NOBODY;
	}
	*/
}

void AddToIntList( UINT16 ubID, BOOLEAN fGainControl, BOOLEAN fCommunicate )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"AddToIntList");
	UINT16 ubLoop;

//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d added to int list", ubID );
	DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: adding ID %d who %s", ubID, fGainControl ? "gains control" : "loses control" ) );

	// check whether 'who' is already anywhere on the queue after the first index
	// which we want to preserve so we can restore turn order
	for (ubLoop = 2; ubLoop <= gubOutOfTurnPersons; ubLoop++)
	{
		if (gubOutOfTurnOrder[ubLoop] == ubID)
		{
			if (!fGainControl)
			{
				// he's LOSING control; that's it, we're done, DON'T add him to the queue again
				gubLastInterruptedGuy = ubID;
				return;
			}
			else
			{
				// GAINING control, so delete him from this slot (because later he'll
				// get added to the end and we don't want him listed more than once!)
				DeleteFromIntList( ubLoop, FALSE );
			}
		}
	}

	// increment total (making index valid) and add him to list
	gubOutOfTurnPersons++;
	gubOutOfTurnOrder[gubOutOfTurnPersons] = ubID;

/*
	// the guy being interrupted HAS to be the currently selected character
	if (Status.lastInterruptedWas != CharacterSelected)
	{
		// if we don't already do so, remember who that was
		Status.lastInterruptedWas = CharacterSelected;
	}
*/

	// if the guy is gaining control
	if (fGainControl)
	{
		// record his initial APs at the start of his interrupt at this time
		// this is not the ideal place for this, but it's the best I could do...
		Menptr[ubID].aiData.bIntStartAPs = Menptr[ubID].bActionPoints;
	}
	else
	{
		gubLastInterruptedGuy = ubID;
		// turn off AI control flag if they lost control
		if (Menptr[ubID].flags.uiStatusFlags & SOLDIER_UNDERAICONTROL)
		{
			DebugAI( String( "Taking away AI control from %d", ubID ) );
			Menptr[ubID].flags.uiStatusFlags &= (~SOLDIER_UNDERAICONTROL);
		}
	}
}

void VerifyOutOfTurnOrderArray()
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"VerifyOutOfTurnOrderArray");
	UINT16		ubTeamHighest[ MAXTEAMS ] = { 0 };
	UINT8		ubTeamsInList;
	UINT16		ubNextInArrayOnTeam, ubNextIndex;
	UINT8		ubTeam;
	UINT16 ubLoop, ubLoop2;
	BOOLEAN	fFoundLoop = FALSE;

	for (ubLoop = 1; ubLoop <= gubOutOfTurnPersons; ubLoop++)
	{
		ubTeam = Menptr[ gubOutOfTurnOrder[ ubLoop ] ].bTeam;
		if (ubTeamHighest[ ubTeam ] > 0)
		{
			// check the other teams to see if any of them are between our last team's mention in
			// the array and this
			for (ubLoop2 = 0; ubLoop2 < MAXTEAMS; ubLoop2++)
			{
				if (ubLoop2 == ubTeam)
				{
					continue;
				}
				else
				{
					if (ubTeamHighest[ ubLoop2 ] > ubTeamHighest[ ubTeam ])
					{
						// there's a loop!! delete it!
						ubNextInArrayOnTeam = gubOutOfTurnOrder[ ubLoop ];
						ubNextIndex = ubTeamHighest[ ubTeam ] + 1;

						while( gubOutOfTurnOrder[ ubNextIndex ] != ubNextInArrayOnTeam )
						{
							// Pause them...
							MercPtrs[ gubOutOfTurnOrder[ ubNextIndex ] ]->AdjustNoAPToFinishMove( TRUE );

							// If they were turning from prone, stop them
							MercPtrs[ gubOutOfTurnOrder[ ubNextIndex ] ]->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;

							DeleteFromIntList( ubNextIndex, FALSE );
						}

						fFoundLoop = TRUE;
						break;
					}
				}
			}

			if (fFoundLoop)
			{
				// at this point we should restart our outside loop (ugh)
				fFoundLoop = FALSE;
				for (ubLoop2 = 0; ubLoop2 < MAXTEAMS; ubLoop2++)
				{
					ubTeamHighest[ ubLoop2 ] = 0;
				}
				ubLoop = 0;
				continue;

			}

		}

		ubTeamHighest[ ubTeam ] = ubLoop;
	}

	// Another potential problem: the player is interrupted by the enemy who is interrupted by
	// the militia.	In this situation the enemy should just lose their interrupt.
	// (Or, the militia is interrupted by the enemy who is interrupted by the player.)

	// Check for 3+ teams in the interrupt queue.	If three exist then abort all interrupts (return
	// control to the first team)
	ubTeamsInList = 0;
	for ( ubLoop = 0; ubLoop < MAXTEAMS; ubLoop++ )
	{
		if ( ubTeamHighest[ ubLoop ] > 0 )
		{
			ubTeamsInList++;
		}
	}
	if ( ubTeamsInList >= 3 )
	{
		// This is bad.	Loop through everyone but the first person in the INT list and remove 'em
		for (ubLoop = 2; ubLoop <= gubOutOfTurnPersons; )
		{
			if ( MercPtrs[ gubOutOfTurnOrder[ ubLoop ] ]->bTeam != MercPtrs[ gubOutOfTurnOrder[ 1 ] ]->bTeam )
			{
				// remove!

				// Pause them...
				MercPtrs[ gubOutOfTurnOrder[ ubLoop ] ]->AdjustNoAPToFinishMove( TRUE );

				// If they were turning from prone, stop them
				MercPtrs[ gubOutOfTurnOrder[ ubLoop ] ]->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;

				DeleteFromIntList( ubLoop, FALSE );

				// since we deleted someone from the list, we want to check the same index in the
				// array again, hence we DON'T increment.
			}
			else
			{
				ubLoop++;
			}
		}
	}

}

void DoneAddingToIntList( SOLDIERTYPE * pSoldier, BOOLEAN fChange, UINT8 ubInterruptType)
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"DoneAddingToIntList");
	if (fChange)
	{
		VerifyOutOfTurnOrderArray();
		if ( EveryoneInInterruptListOnSameTeam() )
		{
			EndInterrupt( TRUE );
		}
		else
		{
			if (!is_networked) 
			{
				StartInterrupt();
			} 
			else 
			{
				UINT16						nubFirstInterrupter;
				INT8						nbTeam;
				SOLDIERTYPE *				npSoldier;
						
				nubFirstInterrupter = LATEST_INTERRUPT_GUY;
				npSoldier = MercPtrs[nubFirstInterrupter];
				nbTeam = npSoldier->bTeam;

				//pSoldier is interrupted
				//npSoldier is interruptor
				//hayden

				// INTERRUPT is calculated on the server
				if ((nbTeam > 0) && (nbTeam <6 ) && is_server) //is for AI and are server
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s is interrupt by AI", TeamNameStrings[pSoldier->bTeam]);
					
					// Only display the top message if we (the server) got interrupted
					if (pSoldier->bTeam == 0)
						AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, TeamTurnString[ nbTeam ] );

					send_interrupt( npSoldier );
					StartInterrupt();

				}
				// INTERRUPT is calculated on the server
				else if(is_server && gTacticalStatus.ubCurrentTeam == 1)//  against ai and are server
				{
					//hayden
					send_interrupt( npSoldier ); //
					if(nbTeam !=0)
						intAI(npSoldier);						
					else 
						StartInterrupt();//
					
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"AI is interrupted by %s", TeamNameStrings[npSoldier->bTeam]);
				}
				// INTERRUPT is calculated on the pure client
				else if(gTacticalStatus.ubCurrentTeam == 0)//its our turn (we are moving)
				{																	
#ifdef	INTERRUPT_MP_DEADLOCK_FIX
					// Do nothing
#else
					if (cGameType == MP_TYPE_COOP)
						ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, MPClientMessage[79]);
#endif

					send_interrupt( npSoldier );


					SOLDIERTYPE* pMerc = gusSelectedSoldier;
					//AdjustNoAPToFinishMove( pMerc, TRUE );	
					pMerc->HaultSoldierFromSighting(TRUE);
					//pMerc->fTurningFromPronePosition = FALSE;// hmmm ??
					FreezeInterfaceForEnemyTurn();
					InitEnemyUIBar( 0, 0 );
					fInterfacePanelDirty = DIRTYLEVEL2;
					AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, TeamTurnString[ nbTeam ] );
					gTacticalStatus.fInterruptOccurred = TRUE;

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"You have interrupted %s", TeamNameStrings[npSoldier->bTeam]);
				}
				else
				{
					ClearIntList();//no interrupt to be awarded, clear generated list.
				}
			}
		}
	}
}

void ResolveInterruptsVs( SOLDIERTYPE * pSoldier, UINT8 ubInterruptType)
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,String("ResolveInterruptsVs: Soldier ID = %d, APs = %d (interrupt type = %d)",pSoldier->ubID,pSoldier->bActionPoints, ubInterruptType));
	UINT8 ubTeam;
	SoldierID ubOpp;
	UINT16 ubIntCnt;
	UINT16 ubIntList[MAXMERCS];
	UINT8 ubIntDiff[MAXMERCS];
	UINT8 ubSmallestDiff;
	UINT16 ubSlot, ubSmallestSlot;
	UINT16 ubLoop;
	BOOLEAN fIntOccurs;
	SOLDIERTYPE * pOpponent;
	BOOLEAN fControlChanged = FALSE;

	AssertNotNIL(pSoldier);

	if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		ubIntCnt = 0;

		for (ubTeam = 0; ubTeam < MAXTEAMS; ubTeam++)
		{
			// WDS fix broken interrupts (I hope...)
			if (/*gTacticalStatus.Team[ubTeam].bTeamActive &&*/ (gTacticalStatus.Team[ubTeam].bSide != pSoldier->bSide) && ubTeam != CIV_TEAM)
			{
				for ( ubOpp = gTacticalStatus.Team[ ubTeam ].bFirstID; ubOpp <= gTacticalStatus.Team[ ubTeam ].bLastID; ++ubOpp)
				{
					pOpponent = ubOpp;
					AssertNotNIL(pOpponent);
					if ( pOpponent->bActive && pOpponent->bInSector && (pOpponent->stats.bLife >= OKLIFE) && (pOpponent->bBreath >= OKBREATH) && !(pOpponent->bCollapsed) )
					{
						if ( ubInterruptType == NOISEINTERRUPT )
						{
							// don't grant noise interrupts at greater than max. visible distance
							if ( PythSpacesAway( pSoldier->sGridNo, pOpponent->sGridNo ) > MaxNormalDistanceVisible() )
							{
								pOpponent->aiData.bInterruptDuelPts = NO_INTERRUPT;

								DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("Resetting int pts for %d - NOISE BEYOND SIGHT DISTANCE!?", pOpponent->ubID ) );

								continue;
							}
						}
						else if ( pOpponent->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY )
						{
							pOpponent->aiData.bInterruptDuelPts = NO_INTERRUPT;

							DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("Resetting int pts for %d - DOESN'T SEE ON SIGHT INTERRUPT!?", pOpponent->ubID ) );


							continue;
						}

						switch (pOpponent->aiData.bInterruptDuelPts)
						{
							case NO_INTERRUPT:		// no interrupt possible, no duel necessary
								DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("ResolveInterruptsVs: No interrupt for opponent %d", pOpponent->ubID ) );
								fIntOccurs = FALSE;
								break;

							case AUTOMATIC_INTERRUPT:	// interrupts occurs automatically
								pSoldier->aiData.bInterruptDuelPts = 0;	// just to have a valid intDiff later
								fIntOccurs = TRUE;

								DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: automatic interrupt on %d by %d", pSoldier->ubID, pOpponent->ubID ) );

								break;

							default:		// interrupt is possible, run a duel
								DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, "Calculating int duel pts for onlooker in ResolveInterruptsVs" );
								pSoldier->aiData.bInterruptDuelPts = CalcInterruptDuelPts(pSoldier, pOpponent->ubID, TRUE);
								fIntOccurs = InterruptDuel(pOpponent,pSoldier);
								#ifdef DEBUG_INTERRUPTS
								if (fIntOccurs)
								{
									DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("INTERRUPT: standard interrupt on %d (%d pts) by %d (%d pts)", pSoldier->ubID, pSoldier->aiData.bInterruptDuelPts, pOpponent->ubID, pOpponent->aiData.bInterruptDuelPts) );
								}
								#endif

								break;
						}

						if (fIntOccurs)
						{
							// remember that this opponent's scheduled to interrupt us
							ubIntList[ubIntCnt] = pOpponent->ubID;

							// and by how much he beat us in the duel
							ubIntDiff[ubIntCnt] = pOpponent->aiData.bInterruptDuelPts - pSoldier->aiData.bInterruptDuelPts;

							// increment counter of interrupts lost
							ubIntCnt++;
						}
						else
						{
						/*
							if (pOpponent->aiData.bInterruptDuelPts != NO_INTERRUPT)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d fails to interrupt %d (%d vs %d pts)", pOpponent->ubID, pSoldier->ubID, pOpponent->aiData.bInterruptDuelPts, pSoldier->aiData.bInterruptDuelPts);
							}
							*/
						}

						// either way, clear out both sides' bInterruptDuelPts field to prepare next one

						if (pSoldier->aiData.bInterruptDuelPts != NO_INTERRUPT)
						{
							DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("Resetting int pts for %d", pSoldier->ubID ) );
						}


						pSoldier->aiData.bInterruptDuelPts = NO_INTERRUPT;


						if (pOpponent->aiData.bInterruptDuelPts != NO_INTERRUPT)
						{
							DebugMsg( TOPIC_JA2INTERRUPT, DBG_LEVEL_3, String("Resetting int pts for %d", pOpponent->ubID ) );
						}

						pOpponent->aiData.bInterruptDuelPts = NO_INTERRUPT;

					}

				}
			}
		}

		// if any interrupts are scheduled to occur (ie. I lost at least once)
		if (ubIntCnt)
		{
			// First add currently active character to the interrupt queue.	This is
			// USUALLY pSoldier->guynum, but NOT always, because one enemy can
			// "interrupt" on another enemy's turn if he hears another team's wound
			// victim's screaming...	the guy screaming is pSoldier here, it's not his turn!
			//AddToIntList( (UINT8) gusSelectedSoldier, FALSE, TRUE);

			if ( (gTacticalStatus.ubCurrentTeam != pSoldier->bTeam) && !(gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bHuman) )
			{
				// if anyone on this team is under AI control, remove
				// their AI control flag and put them on the queue instead of this guy
				for ( SoldierID id = gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bFirstID; id <= gTacticalStatus.Team[ gTacticalStatus.ubCurrentTeam ].bLastID; ++id)
				{
					if ( id->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL)
					{
						// this guy lost control
						id->flags.uiStatusFlags &= (~SOLDIER_UNDERAICONTROL);
						AddToIntList( id, FALSE, TRUE);
						break;
					}
				}
			}
			else
			{
				// this guy lost control
				AddToIntList( pSoldier->ubID, FALSE, TRUE);
			}

			// loop once for each opponent who interrupted
			for (ubLoop = 0; ubLoop < ubIntCnt; ubLoop++)
			{
				// find the smallest intDiff still remaining in the list
				ubSmallestDiff = NO_INTERRUPT;
				ubSmallestSlot = NOBODY;

				for (ubSlot = 0; ubSlot < ubIntCnt; ubSlot++)
				{
					if (ubIntDiff[ubSlot] < ubSmallestDiff)
					{
						ubSmallestDiff = ubIntDiff[ubSlot];
						ubSmallestSlot = ubSlot;
					}
				}

				if (ubSmallestSlot < TOTAL_SOLDIERS)
				{
					// add this guy to everyone's interrupt queue
					AddToIntList(ubIntList[ubSmallestSlot],TRUE,TRUE);
					// SANDRO - for IIS, reset counter if we got here
					if ( UsingImprovedInterruptSystem() )
					{
						// reset the counter
						MercPtrs[ ubIntList[ubSmallestSlot] ]->aiData.ubInterruptCounter[pSoldier->ubID] = 0;						
					}
					if (INTERRUPTS_OVER)
					{
						// a loop was created which removed all the people in the interrupt queue!
						EndInterrupt( TRUE );
						return;
					}

					ubIntDiff[ubSmallestSlot] = NO_INTERRUPT;		// mark slot as been handled
				}
			}

			fControlChanged = TRUE;
		}

		// sends off an end-of-list msg telling everyone whether to switch control,
		// unless it's a MOVEMENT interrupt, in which case that is delayed til later
		DoneAddingToIntList(pSoldier,fControlChanged,ubInterruptType);
	}
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"ResolveInterruptsVs done");
}


BOOLEAN	SaveTeamTurnsToTheSaveGameFile( HWFILE hFile )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"SaveTeamTurnsToTheSaveGameFile");
	UINT32	uiNumBytesWritten;
	TEAM_TURN_SAVE_STRUCT TeamTurnStruct;

	//Save the gubTurn Order Array
	FileWrite( hFile, gubOutOfTurnOrder, sizeof( UINT16 ) * MAXMERCS, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT16 ) * MAXMERCS )
	{
		return( FALSE );
	}


	TeamTurnStruct.ubOutOfTurnPersons = gubOutOfTurnPersons;

	TeamTurnStruct.InterruptOnlyGuynum = InterruptOnlyGuynum;
	TeamTurnStruct.sWhoThrewRock = gsWhoThrewRock;
	TeamTurnStruct.InterruptsAllowed = InterruptsAllowed;
	TeamTurnStruct.fHiddenInterrupt = gfHiddenInterrupt;
	TeamTurnStruct.ubLastInterruptedGuy = gubLastInterruptedGuy;


	//Save the Team turn save structure
	FileWrite( hFile, &TeamTurnStruct, sizeof( TEAM_TURN_SAVE_STRUCT ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( TEAM_TURN_SAVE_STRUCT ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN	LoadTeamTurnsFromTheSavedGameFile( HWFILE hFile )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"LoadTeamTurnsFromTheSavedGameFile");
	UINT32	uiNumBytesRead;
	TEAM_TURN_SAVE_STRUCT TeamTurnStruct;

	//Load the gubTurn Order Array
	FileRead( hFile, gubOutOfTurnOrder, sizeof( UINT16 ) * MAXMERCS, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT16 ) * MAXMERCS )
	{
		return( FALSE );
	}


	//Load the Team turn save structure
	FileRead( hFile, &TeamTurnStruct, sizeof( TEAM_TURN_SAVE_STRUCT ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( TEAM_TURN_SAVE_STRUCT ) )
	{
		return( FALSE );
	}

	gubOutOfTurnPersons = TeamTurnStruct.ubOutOfTurnPersons;

	InterruptOnlyGuynum = TeamTurnStruct.InterruptOnlyGuynum;
	gsWhoThrewRock = TeamTurnStruct.sWhoThrewRock;
	InterruptsAllowed = TeamTurnStruct.InterruptsAllowed;
	gfHiddenInterrupt = TeamTurnStruct.fHiddenInterrupt;
	gubLastInterruptedGuy = TeamTurnStruct.ubLastInterruptedGuy;


	return( TRUE );
}

BOOLEAN NPCFirstDraw( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTargetSoldier )
{
	DebugMsg (TOPIC_JA2INTERRUPT,DBG_LEVEL_3,"NPCFirstDraw");
	// if attacking an NPC check to see who draws first!

	if ( pTargetSoldier->ubProfile != NO_PROFILE && pTargetSoldier->ubProfile != SLAY && pTargetSoldier->aiData.bNeutral && pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == SEEN_CURRENTLY && (	FindAIUsableObjClass( pTargetSoldier, IC_WEAPON ) != NO_SLOT ) )
	{
		UINT8	ubLargerHalf, ubSmallerHalf, ubTargetLargerHalf, ubTargetSmallerHalf;

		// roll the dice!
		// e.g. if level 5, roll Random( 3 + 1 ) + 2 for result from 2 to 5
		// if level 4, roll Random( 2 + 1 ) + 2 for result from 2 to 4
		ubSmallerHalf = EffectiveExpLevel( pSoldier ) / 2;
		ubLargerHalf = EffectiveExpLevel( pSoldier ) - ubSmallerHalf;

		ubTargetSmallerHalf = EffectiveExpLevel( pTargetSoldier ) / 2;
		ubTargetLargerHalf = EffectiveExpLevel( pTargetSoldier ) - ubTargetSmallerHalf;
		if ( gMercProfiles[ pTargetSoldier->ubProfile ].bApproached & gbFirstApproachFlags[ APPROACH_THREATEN - 1 ] )
		{
			// gains 1 to 2 points
			ubTargetSmallerHalf += 1;
			ubTargetLargerHalf += 1;
		}
		if ( Random( ubTargetSmallerHalf + 1) + ubTargetLargerHalf > Random( ubSmallerHalf + 1) + ubLargerHalf )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}
