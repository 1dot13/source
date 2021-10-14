#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "Soldier Control.h"
	#include "Overhead.h"
	#include "Boxing.h"
	#include "Render Fun.h"
	#include "Random.h"
	#include "Worldman.h"
	#include "Soldier Profile.h"
	#include "NPC.h"
	#include "Opplist.h"
	#include "ai.h"
	#include "Dialogue Control.h"
	#include "Handle UI.h"
	#include "Points.h"
	#include "interface.h"
	#include "interface dialogue.h"
	#include "TeamTurns.h"
	#include "Music Control.h"
	#include "history.h"
	#include "strategicmap.h"
	#include "Game Clock.h"
	#include "Animation Data.h"
	#include "Font Control.h"
	#include "message.h"
	#include "GameSettings.h" // added by SANDRO
#endif

INT32	gsBoxerGridNo[ NUM_BOXERS ] = { 11393, 11233, 11073 };
UINT8 gubBoxerID[ NUM_BOXERS ] = { NOBODY, NOBODY, NOBODY };
BOOLEAN gfBoxerFought[ NUM_BOXERS ] = { FALSE, FALSE, FALSE };
BOOLEAN	gfLastBoxingMatchWonByPlayer = FALSE;
UINT8	gubBoxingMatchesWon = 0;
UINT8	gubBoxersRests = 0;
BOOLEAN gfBoxersResting = FALSE;

extern void RecalculateOppCntsDueToBecomingNeutral( SOLDIERTYPE * pSoldier );

void ExitBoxing( void )
{
	//DBrot: More Rooms
	//UINT8						ubRoom;
	UINT16				usRoom;
	SOLDIERTYPE*		pSoldier;
	UINT32				uiLoop;
	UINT8				ubPass;

	// find boxers and turn them neutral again

	// first time through loop, look for AI guy, then for PC guy.... for stupid
	// oppcnt/alert status reasons
	for( ubPass = 0; ubPass < 2; ++ubPass )
	{
		// because boxer could die, loop through all soldier ptrs
		for ( uiLoop = 0; uiLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ++uiLoop )
		{
			pSoldier = MercPtrs[ uiLoop ];

			if ( pSoldier != NULL )
			{
				if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_BOXER ) && InARoom( pSoldier->sGridNo, &usRoom ) && usRoom == BOXING_RING )
				{
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
					{
						if ( ubPass == 0 ) // pass 0, only handle AI
						{
							continue;
						}
						// put guy under AI control temporarily
						pSoldier->flags.uiStatusFlags |= SOLDIER_PCUNDERAICONTROL;
						//SB: this flag don't allow merc leave the ring
						pSoldier->flags.uiStatusFlags &= ~SOLDIER_ENGAGEDINACTION;
					}
					else
					{
						if ( ubPass == 1 ) // pass 1, only handle PCs
						{
							continue;
						}
						// reset AI boxer to neutral
						SetSoldierNeutral( pSoldier );
						RecalculateOppCntsDueToBecomingNeutral( pSoldier );
					}
					DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: exit boxing"));
					CancelAIAction( pSoldier, TRUE );
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
					pSoldier->aiData.bUnderFire = 0;

					// HEADROCK HAM 3.6: Make sure all boxers' APs have been reset to a reasonable number. Otherwise,
                    // the AI combatant may fail several conditions in subsequent functions, and fail to leave the ring
                    // as a result.
                    if (pSoldier->bActionPoints < (APBPConstants[AP_MAXIMUM]*6)/10)
                    {
                        pSoldier->bActionPoints = (APBPConstants[AP_MAXIMUM]*6)/10;
                    }

					// if necessary, revive boxer so he can leave ring
					if (pSoldier->stats.bLife > 0 && (pSoldier->stats.bLife < OKLIFE || pSoldier->bBreath < OKBREATH ) )
					{
						pSoldier->stats.bLife = __max( OKLIFE * 2, pSoldier->stats.bLife );
						if (pSoldier->bBreath < 100)
						{
							// deduct -ve BPs to grant some BPs back (properly)
							DeductPoints( pSoldier, 0, (INT16) - ( (100 - pSoldier->bBreath) * 100 ) );
						}
						pSoldier->BeginSoldierGetup( );
					}
				}
			}
		}
	}

	DeleteTalkingMenu();

	EndAllAITurns();

	if ( CheckForEndOfCombatMode( FALSE ) )
	{
		EndTopMessage();
		
		#ifdef NEWMUSIC
		GlobalSoundID  = MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ];
		if ( MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] != -1 )
			SetMusicModeID( MUSIC_TACTICAL_NOTHING, MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] );	
		else
		#endif
		SetMusicMode( MUSIC_TACTICAL_NOTHING );

		// Lock UI until we get out of the ring
		guiPendingOverrideEvent = LU_BEGINUILOCK;
	}
}

// in both these cases we're going to want the AI to take over and move the boxers
// out of the ring!
void EndBoxingMatch( SOLDIERTYPE * pLoser )
{
	if (pLoser->bTeam == gbPlayerNum )
	{
		SetBoxingState( LOST_ROUND );
	}
	else
	{
		SetBoxingState( WON_ROUND );
		gfLastBoxingMatchWonByPlayer = TRUE;
		gubBoxingMatchesWon++;
		DebugQuestInfo(String("EndBoxingMatch: gubBoxingMatchesWon %d gfLastBoxingMatchWonByPlayer %d", gubBoxingMatchesWon, gfLastBoxingMatchWonByPlayer));
	}
	TriggerNPCRecord( DARREN, 22 );
}

void BoxingPlayerDisqualified( SOLDIERTYPE * pOffender, INT8 bReason )
{
	if (bReason == BOXER_OUT_OF_RING || bReason == NON_BOXER_IN_RING)
	{
		pOffender->EVENT_StopMerc( pOffender->sGridNo, pOffender->ubDirection );
	}
	SetBoxingState( DISQUALIFIED );
	TriggerNPCRecord( DARREN, 21 );
	//ExitBoxing();
}

void TriggerEndOfBoxingRecord( SOLDIERTYPE * pSoldier )
{
	// unlock UI
	guiPendingOverrideEvent = LU_ENDUILOCK;

	if ( pSoldier )
	{
		switch( gTacticalStatus.bBoxingState )
		{
			case WON_ROUND:
				AddHistoryToPlayersLog( HISTORY_WON_BOXING, pSoldier->ubProfile, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );
				TriggerNPCRecord( DARREN, 23 );
				break;
			case LOST_ROUND:
				// log as lost
				AddHistoryToPlayersLog( HISTORY_LOST_BOXING, pSoldier->ubProfile, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );
				TriggerNPCRecord( DARREN, 24 );
				break;
			case DISQUALIFIED:
				AddHistoryToPlayersLog( HISTORY_DISQUALIFIED_BOXING, pSoldier->ubProfile, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );
				break;
		}
	}

	SetBoxingState( NOT_BOXING );
	ClearAllBoxerFlags();
}

UINT8 CountPeopleInBoxingRing( void )
{
	SOLDIERTYPE * pSoldier;
	UINT32 uiLoop;
	//DBrot: More Rooms
	//UINT8 ubRoom;
	UINT16	usRoom;
	UINT8 ubTotalInRing = 0;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop )
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier != NULL )
		{
			if ( InARoom( pSoldier->sGridNo, &usRoom ) && usRoom == BOXING_RING)
			{
				++ubTotalInRing;
			}
		}
	}

	return( ubTotalInRing );
}

void CountPeopleInBoxingRingAndDoActions( void )
{
	UINT32				uiLoop;
	UINT8				ubTotalInRing = 0;
	//DBrot: More Rooms
	UINT16				usRoom;
	UINT16				ubPlayersInRing = 0;
	SOLDIERTYPE *		pSoldier;
	SOLDIERTYPE *		pInRing[2] = { NULL, NULL };
	SOLDIERTYPE *		pNonBoxingPlayer = NULL;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop )
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier != NULL )
		{
			if ( InARoom( pSoldier->sGridNo, &usRoom ) && usRoom == BOXING_RING)
			{
				if ( ubTotalInRing < 2 )
				{
					pInRing[ ubTotalInRing ] = pSoldier;
				}
				++ubTotalInRing;

				if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
				{
					++ubPlayersInRing;

					if ( !pNonBoxingPlayer && !(pSoldier->flags.uiStatusFlags & SOLDIER_BOXER) )
					{
						pNonBoxingPlayer = pSoldier;
					}
				}
			}
		}
	}

	if ( ubPlayersInRing > 1 )
	{
		// boxing match just became invalid!
		if ( gTacticalStatus.bBoxingState <= PRE_BOXING )
		{
			BoxingPlayerDisqualified( pNonBoxingPlayer, NON_BOXER_IN_RING );
			// set to not in boxing or it won't be handled otherwise
			SetBoxingState( NOT_BOXING );
		}
		else
		{
			BoxingPlayerDisqualified( pNonBoxingPlayer, NON_BOXER_IN_RING );
		}

		return;
	}

	if ( gTacticalStatus.bBoxingState == BOXING_WAITING_FOR_PLAYER )
	{
		if ( ubTotalInRing == 1 && ubPlayersInRing == 1 )
		{
			// time to go to pre-boxing
			SetBoxingState( PRE_BOXING );

			// Flugente: in order for boxing to work while covert, we require the boxer to recognize our covert merc as a boxer, but that flag is not yet set
			// so temporarily set the flag, recognize our merc, and then remove the flag again. it will be properly set later
			pInRing[0]->flags.uiStatusFlags |= SOLDIER_BOXER;

			PickABoxer();

			pInRing[0]->DeleteBoxingFlag();
		}
	}
	else
	{
		// if pre-boxing, check for two people (from different teams!) in the ring
		if (gTacticalStatus.bBoxingState == PRE_BOXING)
		{
			if (ubTotalInRing == 2 && ubPlayersInRing == 1)
			{
				// ladieees and gennleman, we have a fight!
				for (uiLoop = 0; uiLoop < 2; ++uiLoop)
				{
					if (!(pInRing[uiLoop]->flags.uiStatusFlags & SOLDIER_BOXER))
					{
						// set as boxer!
						pInRing[uiLoop]->flags.uiStatusFlags |= SOLDIER_BOXER;
					}
				}
				// start match!
				SetBoxingState(BOXING);
				gfLastBoxingMatchWonByPlayer = FALSE;
				DebugQuestInfo(String("Start Match: gfLastBoxingMatchWonByPlayer %d", gfLastBoxingMatchWonByPlayer));

#ifdef JA2TESTVERSION
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Boxer APs %d %d", pInRing[0]->bActionPoints, pInRing[1]->bActionPoints );
#endif
				// give the first turn to a randomly chosen boxer
				EnterCombatMode(pInRing[Random(2)]->bTeam);
			}
		}
		/*
		else
		{
		// check to see if the player has more than one person in the ring
		if ( ubPlayersInRing > 1 )
		{
		// boxing match just became invalid!
		BoxingPlayerDisqualified( pNonBoxingPlayer, NON_BOXER_IN_RING );
		return;
		}
		}
		*/
	}
}

BOOLEAN CheckOnBoxers( void )
{
	UINT32					uiLoop;
	UINT8						ubID;

	// repick boxer IDs every time
	if ( gubBoxerID[0] == NOBODY )
	{
		// get boxer soldier IDs!
		for( uiLoop = 0; uiLoop < NUM_BOXERS; uiLoop++ )
		{
			ubID = WhoIsThere2( gsBoxerGridNo[ uiLoop ], 0 );

			// WANNE: Safty check!
			if (ubID < TOTAL_SOLDIERS)
			{
				if ( FindObjClass( MercPtrs[ ubID ], IC_WEAPON ) == NO_SLOT &&
					IS_MERC_BODY_TYPE( MercPtrs[ ubID ] ) )
				{
					// no weapon and not a civilian so this guy is a boxer
					gubBoxerID[ uiLoop ] = ubID;
					DebugQuestInfo(String("CheckOnBoxers: set gubBoxerID[%d] %d", ubID, gubBoxerID[uiLoop]));
				}
			}
		}
	}

	if ( gubBoxerID[ 0 ] == NOBODY && gubBoxerID[ 1 ] == NOBODY && gubBoxerID[ 2 ] == NOBODY )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN BoxerExists( void )
{
	UINT32	uiLoop;

	for( uiLoop = 0; uiLoop < NUM_BOXERS; ++uiLoop )
	{
		if ( WhoIsThere2( gsBoxerGridNo[ uiLoop ], 0 ) != NOBODY )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN PickABoxer( void )
{
	UINT32			uiLoop;
	SOLDIERTYPE*	pBoxer;

	for( uiLoop = 0; uiLoop < NUM_BOXERS; ++uiLoop )
	{
		if ( gubBoxerID[ uiLoop ] != NOBODY )
		{
			if ( gfBoxerFought[ uiLoop ] )
			{
				// pathetic attempt to prevent multiple AI boxers
				MercPtrs[ gubBoxerID[ uiLoop ] ]->DeleteBoxingFlag();
			}
			else
			{
				pBoxer = MercPtrs[ gubBoxerID[ uiLoop ] ];
				// pick this boxer!
				if ( pBoxer->bActive && pBoxer->bInSector && pBoxer->stats.bLife >= OKLIFE )
				{
					pBoxer->flags.uiStatusFlags |= SOLDIER_BOXER;
					SetSoldierNonNeutral( pBoxer );
					RecalculateOppCntsDueToNoLongerNeutral( pBoxer );
					DebugAI(AI_MSG_INFO, pBoxer, String("CancelAIAction: pick a boxer"));
					CancelAIAction( pBoxer, TRUE );
					RESETTIMECOUNTER( pBoxer->timeCounters.AICounter, 0 );
					gfBoxerFought[ uiLoop ] = TRUE;
					DebugQuestInfo(String("PickABoxer: set gfBoxerFought[%d] %d", uiLoop, gfBoxerFought[uiLoop]));
					// improve stats based on the # of rests these guys have had
					pBoxer->stats.bStrength = __min( 100, pBoxer->stats.bStrength + gubBoxersRests * 5 );
					pBoxer->stats.bDexterity = __min( 100, pBoxer->stats.bDexterity + gubBoxersRests * 5 );
					pBoxer->stats.bAgility = __min( 100, pBoxer->stats.bAgility + gubBoxersRests * 5 );
					pBoxer->stats.bLifeMax = __min( 100, pBoxer->stats.bLifeMax + gubBoxersRests * 5 );
					// give the 2nd boxer Hand to hand - SANDRO
					if ( (uiLoop == NUM_BOXERS - 2) )
					{
						if (gGameOptions.fNewTraitSystem) // SANDRO - traits
							pBoxer->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
						else
							pBoxer->stats.ubSkillTraits[0] = HANDTOHAND_OT;
					}
					// give the 3rd boxer martial arts
					if ( (uiLoop == NUM_BOXERS - 1) && pBoxer->ubBodyType == REGMALE )
					{
						if (gGameOptions.fNewTraitSystem) // SANDRO - traits
						{
							pBoxer->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
							pBoxer->stats.ubSkillTraits[1] = MARTIAL_ARTS_NT;
						}
						else
							pBoxer->stats.ubSkillTraits[0] = MARTIALARTS_OT;
					}
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}

BOOLEAN BoxerAvailable( void )
{
	// No way around this, BoxerAvailable will have to go find boxer IDs if they aren't set.
	if ( CheckOnBoxers() == FALSE )
	{
		return( FALSE );
	}

	for (UINT8 ubLoop = 0; ubLoop < NUM_BOXERS; ++ubLoop)
	{
		if ( gubBoxerID[ ubLoop ] != NOBODY && !gfBoxerFought[ ubLoop ] )
		{
			if( MercPtrs[ gubBoxerID[ ubLoop ] ]->bActive && MercPtrs[ gubBoxerID[ ubLoop ] ]->bInSector && MercPtrs[ gubBoxerID[ ubLoop ] ]->stats.bLife >= OKLIFE )
				return( TRUE );
		}
	}

	return( FALSE );
}

// NOTE THIS IS NOW BROKEN BECAUSE NPC.C ASSUMES THAT BOXERSAVAILABLE < 3 IS A
// SEQUEL FIGHT.   Maybe we could check Kingpin's location instead!
UINT8 BoxersAvailable( void )
{
	UINT8			ubCount = 0;

	for (UINT8 ubLoop = 0; ubLoop < NUM_BOXERS; ++ubLoop)
	{
		if ( gubBoxerID[ ubLoop ] != NOBODY && !gfBoxerFought[ ubLoop ] )
		{
			if( MercPtrs[ gubBoxerID[ ubLoop ] ]->bActive && MercPtrs[ gubBoxerID[ ubLoop ] ]->bInSector && MercPtrs[ gubBoxerID[ ubLoop ] ]->stats.bLife >= OKLIFE )
				++ubCount;
		}
	}

	return( ubCount );
}

BOOLEAN AnotherFightPossible( void )
{
	// Check that and a boxer is still available and
	// a player has at least OKLIFE + 5 life

	// and at least one fight HAS occurred
	UINT8						ubLoop;
	SOLDIERTYPE *		pSoldier;
	UINT8						ubAvailable;

	ubAvailable = BoxersAvailable();

	if ( ubAvailable == NUM_BOXERS || ubAvailable == 0 )
	{
		return( FALSE );
	}

	// Loop through all mercs on player team
	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	pSoldier = MercPtrs[ ubLoop ];
	for ( ; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++ubLoop, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife > (OKLIFE + 5) && !pSoldier->bCollapsed )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void BoxingMovementCheck( SOLDIERTYPE * pSoldier )
{
	//DBrot: More Rooms
	//UINT8 ubRoom;
	UINT16	usRoom;

	if ( InARoom( pSoldier->sGridNo, &usRoom ) && usRoom == BOXING_RING)
	{
		// someone moving in/into the ring
		CountPeopleInBoxingRingAndDoActions();
	}
	else if ( ( gTacticalStatus.bBoxingState == BOXING ) && ( pSoldier->flags.uiStatusFlags & SOLDIER_BOXER ) )
	{
		// boxer stepped out of the ring!
		BoxingPlayerDisqualified( pSoldier, BOXER_OUT_OF_RING );
		// add the history record here.
		AddHistoryToPlayersLog( HISTORY_DISQUALIFIED_BOXING, pSoldier->ubProfile, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );
		// make not a boxer any more
		pSoldier->DeleteBoxingFlag( );
		pSoldier->flags.uiStatusFlags &= (~SOLDIER_PCUNDERAICONTROL);
	}
}

void SetBoxingState( INT8 bNewState )
{
	DebugQuestInfo(String("SetBoxingState: %d", bNewState));

	if ( gTacticalStatus.bBoxingState == NOT_BOXING )
	{
		if ( bNewState != NOT_BOXING )
		{
			// pause time
			PauseGame();
		}
	}
	else
	{
		if ( bNewState == NOT_BOXING )
		{
			// unpause time
			UnPauseGame();

			if ( BoxersAvailable() == NUM_BOXERS )
			{
				// set one boxer to be set as boxed so that the game will allow another
				// fight to occur
				gfBoxerFought[ 0 ] = TRUE;
				DebugQuestInfo(String("SetBoxingState: set one boxer to be set as boxed so that the game will allow another, gfBoxerFought[ 0 ] %d", gfBoxerFought[0]));
			}
		}
	}
	gTacticalStatus.bBoxingState = bNewState;
	#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Boxing state now %d", bNewState );
	#endif
}

void ClearAllBoxerFlags( void )
{
	for (UINT32 uiSlot = 0; uiSlot < guiNumMercSlots; ++uiSlot)
	{
		if ( MercSlots[ uiSlot ] && MercSlots[ uiSlot ]->flags.uiStatusFlags & SOLDIER_BOXER )
		{
			// Flugente: nuke the entire opponent count, remove boxing flag, reevaluate opponent list
			DecayIndividualOpplist(MercSlots[uiSlot]);
			
			MercSlots[ uiSlot ]->DeleteBoxingFlag();

			ManLooksForOtherTeams(MercSlots[uiSlot]);

			if ( MercSlots[uiSlot]->bTeam == gbPlayerNum )
				MercSlots[uiSlot]->flags.uiStatusFlags &= (~SOLDIER_PCUNDERAICONTROL);
		}
	}
}
