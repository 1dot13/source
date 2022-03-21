#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Game Clock.h"
	#include "Font Control.h"
	#include "Timer Control.h"
	#include "overhead.h"
	#include "Game Clock.h"
	#include "message.h"
	#include "worlddef.h"
	#include "rotting corpses.h"
	#include "soldier create.h"
	#include "soldier add.h"
	#include "isometric utils.h"
	#include "animation data.h"
	#include "animation control.h"
	#include "Tactical Turns.h"
	#include "smell.h"
	#include "opplist.h"
	#include "Queen Command.h"
	#include "dialogue control.h"
	#include "smokeeffects.h"
	#include "lighteffects.h"
	#include "campaign.h"
	#include "Soldier macros.h"
	#include "strategicmap.h"
	#include "Random.h"
	#include "Explosion Control.h"
	#include "Dialogue Control.h"	// added by Flugente
#endif

#include "Reinforcement.h"

extern void DecayPublicOpplist( INT8 bTeam );

//not in overhead.h!
extern UINT8 NumEnemyInSector();

#ifdef JA2UB

//no uB
#else

void HandleRPCDescription(	)
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
//	UINT8	ubMercsInSector[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ] = { 0 };
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc = 0;
	SOLDIERTYPE *pTeamSoldier;
	INT32		cnt2;
	BOOLEAN fSAMSite = FALSE;


	if ( !gTacticalStatus.fCountingDownForGuideDescription )
	{
		return;
	}

	// ATE: postpone if we are not in tactical
	if ( guiCurrentScreen != GAME_SCREEN )
	{
	return;
	}

	if ( ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
	{
	return;
	}

	// Are we a SAM site?
	if ( gTacticalStatus.ubGuideDescriptionToUse == 27 ||
		gTacticalStatus.ubGuideDescriptionToUse == 30 ||
		gTacticalStatus.ubGuideDescriptionToUse == 32 ||
		gTacticalStatus.ubGuideDescriptionToUse == 25 ||
		gTacticalStatus.ubGuideDescriptionToUse == 31 )
	{
	 fSAMSite = TRUE;
	 gTacticalStatus.bGuideDescriptionCountDown = 1;
	}

	// ATE; Don't do in combat
	if ( ( gTacticalStatus.uiFlags & INCOMBAT ) && !fSAMSite )
	{
		return;
	}

	// Don't do if enemy in sector
	if ( NumEnemyInSector( ) && !fSAMSite )
	{
		return;
	}
	
	gTacticalStatus.bGuideDescriptionCountDown--;

	if ( gTacticalStatus.bGuideDescriptionCountDown == 0 )
	{
		gTacticalStatus.fCountingDownForGuideDescription = FALSE;

		// OK, count how many rpc guys we have....
		// set up soldier ptr as first element in mercptrs list
		cnt2 = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		if (gTacticalStatus.ubGuideDescriptionToUse != 100)
		{
			// run through list
			for ( pTeamSoldier = MercPtrs[cnt2]; cnt2 <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt2, pTeamSoldier++ )
			{
				// Add guy if he's a candidate...
				if ( RPC_RECRUITED( pTeamSoldier ) )
				{
					if ( pTeamSoldier->stats.bLife >= OKLIFE && pTeamSoldier->bActive &&
						pTeamSoldier->sSectorX == gTacticalStatus.bGuideDescriptionSectorX && pTeamSoldier->sSectorY == gTacticalStatus.bGuideDescriptionSectorY &&
						pTeamSoldier->bSectorZ == gbWorldSectorZ &&
						!pTeamSoldier->flags.fBetweenSectors )
					{
						if ( pTeamSoldier->ubProfile == IRA ||
							pTeamSoldier->ubProfile == MIGUEL ||
							pTeamSoldier->ubProfile == CARLOS ||
							pTeamSoldier->ubProfile == DIMITRI )
						{
							ubMercsInSector[ubNumMercs] = (UINT8)cnt2;
							++ubNumMercs;
						}
					}
				}
			}

			// If we are > 0
			if ( ubNumMercs > 0 )
			{
				ubChosenMerc = (UINT8)Random( ubNumMercs );

				TacticalCharacterDialogueWithSpecialEvent( MercPtrs[ubMercsInSector[ubChosenMerc]], gTacticalStatus.ubGuideDescriptionToUse, DIALOGUE_SPECIAL_EVENT_USE_ALTERNATE_FILES, 0, 0 );
			}
		}

		// Flugente: special value signifies lua-based quotes
		if ( !ubNumMercs || gTacticalStatus.ubGuideDescriptionToUse == 100 )
		{
			// run through list
			cnt2 = gTacticalStatus.Team[gbPlayerNum].bFirstID;
			for ( pTeamSoldier = MercPtrs[cnt2]; cnt2 <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt2, pTeamSoldier++ )
			{
				if ( pTeamSoldier->stats.bLife >= OKLIFE && pTeamSoldier->bActive &&
					pTeamSoldier->sSectorX == gTacticalStatus.bGuideDescriptionSectorX && pTeamSoldier->sSectorY == gTacticalStatus.bGuideDescriptionSectorY &&
					pTeamSoldier->bSectorZ == gbWorldSectorZ &&
					!pTeamSoldier->flags.fBetweenSectors )
				{
					AdditionalTacticalCharacterDialogue_CallsLua( pTeamSoldier, ADE_SECTOR_COMMENTARY );
				}
			}
		}
	}
}
#endif

void HandleTacticalEndTurn( )
{
	UINT32 cnt;
	SOLDIERTYPE		*pSoldier;
	UINT32				uiTime;
	static UINT32 uiTimeSinceLastStrategicUpdate = 0;

	// OK, Do a number of things here....
	// Every few turns......

	SetFastForwardMode(FALSE); // Cancel FF at end of battle
	SetClockSpeedPercent(gGameExternalOptions.fClockSpeedPercent);	// sevenfm: set default clock speed

	// Get time elasped
	uiTime = GetWorldTotalSeconds( );

	if ( ( uiTimeSinceLastStrategicUpdate - uiTime ) > 1200 )
	{
		HandleRottingCorpses( );
		//DecayTacticalMoraleModifiers();

	uiTimeSinceLastStrategicUpdate = uiTime;
	}

	DecayBombTimers( );
	
	DecayLightEffects( uiTime );
	DecaySmokeEffects( uiTime );

	// Decay smells
	//DecaySmells();

	// Decay blood
	DecayBloodAndSmells( uiTime );

	// decay AI warning values from corpses
	DecayRottingCorpseAIWarnings();

	HandleEnvironmentHazard( );

	if(gGameExternalOptions.gfAllowReinforcements)//dnl ch68 100913
	{
		if ( gTacticalStatus.Team[ENEMY_TEAM].bTeamActive || gfPendingNonPlayerTeam[ENEMY_TEAM] || 
			 gTacticalStatus.Team[MILITIA_TEAM].bTeamActive || gfPendingNonPlayerTeam[MILITIA_TEAM] )
			 ++guiTurnCnt;
		else
			guiTurnCnt = 0;

		//Check for enemy pooling (add enemies if there happens to be more than the max in the
		//current battle.	If one or more slots have freed up, we can add them now.
		AddPossiblePendingEnemiesToBattle();
		AddPossiblePendingMilitiaToBattle();
	}

	// Loop through each active team and decay public opplist...
	// May want this done every few times too
	NonCombatDecayPublicOpplist( uiTime );
	/*
	for( cnt = 0; cnt < MAXTEAMS; cnt++ )
	{
		if ( gTacticalStatus.Team[ cnt ].bMenInSector > 0 )
		{
			// decay team's public opplist
			DecayPublicOpplist( (INT8)cnt );
		}
	}
*/

	// First pass:
	// Loop through our own mercs:
	//	Check things like ( even if not in our sector )
	//		1 ) All updates of breath, shock, bleeding, etc
	//	2 ) Updating First AID, etc
	//	( If in our sector: )
	//		3 ) Update things like decayed opplist, etc

	// Second pass:
	//	Loop through all mercs in tactical engine
	//	If not a player merc ( ubTeam ) , do things like 1 , 2 , 3 above


	// First exit if we are not in realtime combat or realtime noncombat
	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{

		BeginLoggingForBleedMeToos( TRUE );

		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
		{
			if ( pSoldier->bActive && pSoldier->stats.bLife > 0 && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				// Handle everything from getting breath back, to bleeding, etc
				pSoldier->EVENT_BeginMercTurn( TRUE, 0 );

				if (!AM_A_ROBOT(pSoldier))
				{
					// Handle Player services
					HandlePlayerServices( pSoldier );

					// if time is up, turn off xray
					if ( pSoldier->uiXRayActivatedTime && uiTime > pSoldier->uiXRayActivatedTime + XRAY_TIME )
					{
						TurnOffXRayEffects( pSoldier );
					}

					// Handle stat changes if ness.
					//if ( fCheckStats )
					//{
					////	UpdateStats( pSoldier );
					//}
									
					// Flugente: update multi-turn actions
					pSoldier->UpdateMultiTurnAction();
				}
			}
		}

		BeginLoggingForBleedMeToos( FALSE );

		// OK, loop through the mercs to perform 'end turn' events on each...
		// We're looping through only mercs in tactical engine, ignoring our mercs
		// because they were done earilier...
		for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
		{
			pSoldier = MercSlots[ cnt ];

			if ( pSoldier != NULL )
			{
				if ( pSoldier->bTeam != gbPlayerNum )
				{
					// Handle everything from getting breath back, to bleeding, etc
					pSoldier->EVENT_BeginMercTurn( TRUE, 0 );

					// Handle Player services
					HandlePlayerServices( pSoldier );
				}
			}
		}
	}
#ifdef JA2UB
//	HandleRPCDescription( );
#else
	HandleRPCDescription( );
#endif

	// Flugente: Cool down/decay all items not in a soldier's inventory
	CoolDownWorldItems();

	// Flugente: raise zombies if in gamescreen and option set
	if ( guiCurrentScreen == GAME_SCREEN )
	{
		RaiseZombies();
	}
}



