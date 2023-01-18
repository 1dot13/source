#include "ai.h"
#include "AIInternals.h"
#include "Isometric utils.h"
#include "Points.h"
#include "overhead.h"
#include "opplist.h"
#include "items.h"
#include "Weapons.h"
#include "NPC.h"
#include "Soldier Functions.h"
#include "worldman.h"
#include "Scheduling.h"
#include "Message.h"
#include "Structure Wrap.h"
#include "Keys.h"
#include "pathai.h"
#include "Render Fun.h"
#include "Boxing.h"
//	#include "Air Raid.h"
#include "Soldier Profile.h"
#include "soldier profile type.h"
#include "Soldier macros.h"
#include "los.h"
#include "Buildings.h"
#include "strategicmap.h"
#include "Quests.h"
#include "Map Screen Interface Map.h"
#include "soldier ani.h"
#include "rotting corpses.h"
#include "GameSettings.h"
#include "Dialogue Control.h"
#include "connect.h"
#include "Text.h"

extern BOOLEAN gfHiddenInterrupt;
extern void LogDecideInfo(SOLDIERTYPE *pSoldier);

extern STR8 gStr8AlertStatus[];
extern STR8 gStr8Attitude[];
extern STR8 gStr8Orders[];
extern STR8 gStr8Team[];
extern STR8 gStr8Class[];

// ------------------------------ ZOMBIE AI --------------------------
INT8 ZombieDecideActionGreen(SOLDIERTYPE *pSoldier)
{
	DOUBLE iChance, iSneaky = 10;
	INT8  bInWater;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen, orders = %d",pSoldier->aiData.bOrders));

	DebugAI( AI_MSG_START, pSoldier, String("[Green Zombie]"));
	LogDecideInfo(pSoldier);
	//DebugAI( AI_MSG_INFO, pSoldier, String("AP=%d/%d %s %s %s %s %s", pSoldier->bActionPoints, pSoldier->bInitialActionPoints, gStr8AlertStatus[pSoldier->aiData.bAlertStatus], gStr8Orders[pSoldier->aiData.bOrders], gStr8Attitude[pSoldier->aiData.bAttitude], gStr8Team[pSoldier->bTeam], gStr8Class[pSoldier->ubSoldierClass]) );
	//DebugAI( AI_MSG_INFO, pSoldier, String("Health %d/%d Breath %d/%d Shock %d Tolerance %d AI Morale %d", pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pSoldier->aiData.bShock, CalcSuppressionTolerance(pSoldier), pSoldier->aiData.bAIMorale) );

	gubNPCPathCount = 0;

	bInWater = Water( pSoldier->sGridNo, pSoldier->pathing.bLevel );

	if ( bInWater && PreRandom( 2 ) )
	{
		// don't do nuttin!
		return( AI_ACTION_NONE );
	}

	// if real-time, and not in the way, do nothing 90% of the time (for GUARDS!)
	// unless in water (could've started there), then we better swim to shore!

	////////////////////////////////////////////////////////////////////////////
	// POINT PATROL: move towards next point unless getting a bit winded
	////////////////////////////////////////////////////////////////////////////

	// this takes priority over water/gas checks, so that point patrol WILL work
	// from island to island, and through gas covered areas, too
	if ((pSoldier->aiData.bOrders == POINTPATROL) && (pSoldier->bBreath >= 25))
	{
		if (PointPatrolAI(pSoldier))
		{
			if (!gfTurnBasedAI)
			{
				// wait after this...
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
			}
			return(AI_ACTION_POINT_PATROL);
		}
		else
		{
			// Reset path count to avoid dedlok
			gubNPCPathCount = 0;
		}
	}

	if ((pSoldier->aiData.bOrders == RNDPTPATROL) && (pSoldier->bBreath >=25))
	{
		if (RandomPointPatrolAI(pSoldier))
		{
			if (!gfTurnBasedAI)
			{
				// wait after this...
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
			}
			return(AI_ACTION_POINT_PATROL);
		}
		else
		{
			// Reset path count to avoid dedlok
			gubNPCPathCount = 0;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// WHEN LEFT IN WATER OR GAS, GO TO NEAREST REACHABLE SPOT OF UNGASSED LAND
	////////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ZombieDecideActionGreen: get out of water and gas"));

	if ( bInWater )
	{
		pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

		if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
		{
			return(AI_ACTION_LEAVE_WATER_GAS);
		}
	}

	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ZombieDecideActionGreen: rest if running out of breath"));
	// if our breath is running a bit low, and we're not in the way or in water
	if ((pSoldier->bBreath < 25) && !bInWater)
	{
		// take a breather for gods sake!
		// for realtime, AI will use a standard wait set outside of here
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}


	////////////////////////////////////////////////////////////////////////////
	// CLIMB A BUILDING
	////////////////////////////////////////////////////////////////////////////

	// sevenfm: disable roof climbing for zombies in Green AI
	/*
	if ( gGameExternalOptions.fZombieCanClimb && pSoldier->aiData.bLastAction != AI_ACTION_CLIMB_ROOF && !is_networked )
	{
		iChance = 10 + pSoldier->aiData.bBypassToGreen;

		// set base chance and maximum seeking distance according to orders
		switch (pSoldier->aiData.bOrders)
		{
		case STATIONARY:     iChance *= 0; break;
		case ONGUARD:        iChance += 10; break;
		case ONCALL:                         break;
		case CLOSEPATROL:    iChance += -20; break;
		case RNDPTPATROL:
		case POINTPATROL:    iChance  = -30; break;
		case FARPATROL:      iChance += -40; break;
		case SEEKENEMY:      iChance += -30; break;
		case SNIPER:		 iChance += 70; break;
		}

		// modify for attitude
		switch (pSoldier->aiData.bAttitude)
		{
		case DEFENSIVE:      iChance *= 1.5;  break;
		case BRAVESOLO:      iChance /= 2;    break;
		case BRAVEAID:       iChance /= 2;   break;
		case CUNNINGSOLO:    iChance *= 1;    break;
		case CUNNINGAID:     iChance /= 1;   break;
		case AGGRESSIVE:     iChance /= 3;    break;
		case ATTACKSLAYONLY:									 break;
		}

		// This is the chance that we want to be on the roof.  If already there, invert the chance to see if we want back
		// down
		if (pSoldier->pathing.bLevel > 0)
		{
			iChance = 100 - iChance;
		}

		if ((INT16) PreRandom(100) < iChance)
		{
			BOOLEAN fUp = FALSE;
			if ( pSoldier->pathing.bLevel == 0 )
			{
				fUp = TRUE;
			}
			else if (pSoldier->pathing.bLevel > 0 )
			{
				fUp = FALSE;
			}

			if ( CanClimbFromHere ( pSoldier ) )
			{
				DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Zombie %d is climbing roof",pSoldier->ubID) );
				return( AI_ACTION_CLIMB_ROOF );
			}
			else
			{
				pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier, fUp );
				// Added the check here because sniper militia who are locked inside of a building without keys
				// will still have a >100% chance to want to climb, which means an infinite loop.  In fact, any
				// time a move is desired, there probably also will be a need to check for a path.				
				if ( !TileIsOutOfBounds(pSoldier->aiData.usActionData) &&
					LegalNPCDestination(pSoldier,pSoldier->aiData.usActionData,ENSURE_PATH,WATEROK, 0 ))
				{
					return( AI_ACTION_MOVE_TO_CLIMB  );
				}
			}
		}
	}
	*/


	////////////////////////////////////////////////////////////////////////////
	// RANDOM PATROL:  determine % chance to start a new patrol route
	////////////////////////////////////////////////////////////////////////////
	if (!gubNPCPathCount) // try to limit pathing in Green AI
	{
		iChance = 25 + pSoldier->aiData.bBypassToGreen;

		// sevenfm: limit chance
		iChance = iChance * pSoldier->bBreath / 100;

		// if we're in water with land miles (> 25 tiles) away,
		// OR if we roll under the chance calculated
		if (bInWater || ((INT16) PreRandom(100) < iChance))
		{
			pSoldier->aiData.usActionData = RandDestWithinRange(pSoldier);

			if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
			{
				pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards( pSoldier, pSoldier->aiData.usActionData, AI_ACTION_RANDOM_PATROL );
			}

			if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
			{
				if (!gfTurnBasedAI)
				{
					// wait after this...
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
				}
				return(AI_ACTION_RANDOM_PATROL);
			}
		}
	}

	if (!gubNPCPathCount) // try to limit pathing in Green AI
	{
		////////////////////////////////////////////////////////////////////////////
		// SEEK FRIEND: determine %chance for man to pay a friendly visit
		////////////////////////////////////////////////////////////////////////////

		iChance = 25 + pSoldier->aiData.bBypassToGreen;

		// set base chance and maximum seeking distance according to orders
		switch (pSoldier->aiData.bOrders)
		{
		case STATIONARY:     iChance += -20; break;
		case ONGUARD:        iChance += -15; break;
		case ONCALL:                         break;
		case CLOSEPATROL:    iChance += +10; break;
		case RNDPTPATROL:
		case POINTPATROL:    iChance  = -10; break;
		case FARPATROL:      iChance += +20; break;
		case SEEKENEMY:      iChance += -10; break;
		case SNIPER:		 iChance += -10; break;
		}

		// modify for attitude
		switch (pSoldier->aiData.bAttitude)
		{
		case DEFENSIVE:                       break;
		case BRAVESOLO:      iChance /= 2;    break;  // loners
		case BRAVEAID:       iChance += 10;   break;  // friendly
		case CUNNINGSOLO:    iChance /= 2;    break;  // loners
		case CUNNINGAID:     iChance += 10;   break;  // friendly
		case AGGRESSIVE:                      break;
		case ATTACKSLAYONLY:				  break;
		}

		if ((INT16) PreRandom(100) < iChance)
		{
			if (RandomFriendWithin(pSoldier))
			{
				if ( pSoldier->aiData.usActionData == GoAsFarAsPossibleTowards( pSoldier, pSoldier->aiData.usActionData, AI_ACTION_SEEK_FRIEND ) )
				{
					if ( !gfTurnBasedAI)
					{
						// pause at the end of the walk!
						pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
						pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CIV_AI_DELAY;
					}

					return(AI_ACTION_SEEK_FRIEND);
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND: determine %chance for man to turn in place
	////////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ZombieDecideActionGreen: Soldier deciding to turn"));
	if (GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		// avoid 2 consecutive random turns in a row
		if (pSoldier->aiData.bLastAction != AI_ACTION_CHANGE_FACING)
		{
			iChance = 25 + pSoldier->aiData.bBypassToGreen;

			// set base chance according to orders
			if (pSoldier->aiData.bOrders == STATIONARY || pSoldier->aiData.bOrders == SNIPER)
				iChance += 25;

			if (pSoldier->aiData.bOrders == ONGUARD)
				iChance += 20;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 25;

			if ( pSoldier->aiData.bOrders == SNIPER && pSoldier->pathing.bLevel == 1)
				iChance += 35;

			if ((INT16)PreRandom(100) < iChance)
			{
				// roll random directions (stored in actionData) until different from current
				do
				{
					// if man has a LEGAL dominant facing, and isn't facing it, he will turn
					// back towards that facing 50% of the time here (normally just enemies)
					if ((pSoldier->aiData.bDominantDir >= 0) && (pSoldier->aiData.bDominantDir <= 8) &&
						(pSoldier->ubDirection != pSoldier->aiData.bDominantDir) && PreRandom(2) && pSoldier->aiData.bOrders != SNIPER )
					{
						pSoldier->aiData.usActionData = pSoldier->aiData.bDominantDir;
					}
					else
					{
						INT32 iNoiseValue;
						BOOLEAN fClimb;
						BOOLEAN fReachable;
						INT32 sNoiseGridNo = MostImportantNoiseHeard(pSoldier,&iNoiseValue, &fClimb, &fReachable);
						UINT8 ubNoiseDir;

						if (TileIsOutOfBounds(sNoiseGridNo) || (ubNoiseDir = AIDirection(pSoldier->sGridNo, sNoiseGridNo)) == pSoldier->ubDirection )
						{
							pSoldier->aiData.usActionData = PreRandom(8);
						}
						else
						{
							pSoldier->aiData.usActionData = ubNoiseDir;
						}
					}
				} while (pSoldier->aiData.usActionData == pSoldier->ubDirection);

				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Trying to turn - checking stance validity"));
				if ( pSoldier->InternalIsValidStance( (INT8) pSoldier->aiData.usActionData, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
				{

					if ( !gfTurnBasedAI )
					{
						// wait after this...
						pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
						pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
					}

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Soldier is turning"));
					return(AI_ACTION_CHANGE_FACING);
				}
			}
		}
	}

	// by default, if everything else fails, just stands in place without turning
	// for realtime, regular AI guys will use a standard wait set outside of here
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

INT8 ZombieDecideActionYellow(SOLDIERTYPE *pSoldier)
{
	UINT8 ubNoiseDir;
	INT32 sNoiseGridNo;
	INT32 iNoiseValue;
	INT32 iChance;
	BOOLEAN fClimb;
	BOOLEAN fReachable;
	
	DebugAI( AI_MSG_START, pSoldier, String("[Yellow Zombie]"));
	LogDecideInfo(pSoldier);
	//DebugAI( AI_MSG_INFO, pSoldier, String("AP=%d/%d %s %s %s %s %s", pSoldier->bActionPoints, pSoldier->bInitialActionPoints, gStr8AlertStatus[pSoldier->aiData.bAlertStatus], gStr8Orders[pSoldier->aiData.bOrders], gStr8Attitude[pSoldier->aiData.bAttitude], gStr8Team[pSoldier->bTeam], gStr8Class[pSoldier->ubSoldierClass]) );
	//DebugAI( AI_MSG_INFO, pSoldier, String("Health %d/%d Breath %d/%d Shock %d Tolerance %d AI Morale %d", pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pSoldier->aiData.bShock, CalcSuppressionTolerance(pSoldier), pSoldier->aiData.bAIMorale) );

	// determine the most important noise heard, and its relative value
	sNoiseGridNo = MostImportantNoiseHeard(pSoldier,&iNoiseValue, &fClimb, &fReachable);

	if (TileIsOutOfBounds(sNoiseGridNo))
	{
		// then we have no business being under YELLOW status any more!

#ifdef BETAVERSION
		NumMessage("DecideActionYellow: ERROR - No important noise known by guynum ",pSoldier->ubID);
#endif

		return(AI_ACTION_NONE);
	}	

	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD NOISE: determine %chance for man to turn towards noise
	////////////////////////////////////////////////////////////////////////////

	// determine direction from this soldier in which the noise lies
	ubNoiseDir = AIDirection(pSoldier->sGridNo, sNoiseGridNo);

	// if soldier is not already facing in that direction,
	// and the noise source is close enough that it could possibly be seen
	if ( GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
	{
		if ((pSoldier->ubDirection != ubNoiseDir) && PythSpacesAway(pSoldier->sGridNo,sNoiseGridNo) <= pSoldier->GetMaxDistanceVisible(sNoiseGridNo) )
		{
			// set base chance according to orders
			if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD) )
				iChance = 50;
			else           // all other orders
				iChance = 25;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 15;


			if ((INT16)PreRandom(100) < iChance && pSoldier->InternalIsValidStance( ubNoiseDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = ubNoiseDir;

				return(AI_ACTION_CHANGE_FACING);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// RADIO YELLOW ALERT: determine %chance to call others and report noise
	////////////////////////////////////////////////////////////////////////////

	// if we have the action points remaining to RADIO
	// (we never want NPCs to choose to radio if they would have to wait a turn)
	if ( (pSoldier->bActionPoints >= APBPConstants[AP_RADIO]) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) )
	{
		// base chance depends on how much new info we have to radio to the others
		iChance = 5 * WhatIKnowThatPublicDont(pSoldier,FALSE);   // use 5 * for YELLOW alert

		// if I actually know something they don't and I ain't swimming (deep water)
		if (iChance && !DeepWater( pSoldier->sGridNo, pSoldier->pathing.bLevel ))
		{
			// CJC: this addition allows for varying difficulty levels for soldier types
			iChance += gbDiff[ DIFF_RADIO_RED_ALERT ][ SoldierDifficultyLevel( pSoldier ) ] / 2;

			// Alex: this addition replaces the sectorValue/2 in original JA
			//iChance += gsDiff[DIFF_RADIO_RED_ALERT][GameOption[ENEMYDIFFICULTY]] / 2;

			// modify base chance according to orders
			switch (pSoldier->aiData.bOrders)
			{
			case STATIONARY: iChance +=  20;  break;
			case ONGUARD:    iChance +=  15;  break;
			case ONCALL:     iChance +=  10;  break;
			case CLOSEPATROL:                 break;
			case RNDPTPATROL:
			case POINTPATROL:                 break;
			case FARPATROL:  iChance += -10;  break;
			case SEEKENEMY:  iChance += -20;  break;
			case SNIPER:	 iChance += -10;  break; //Madd: sniper contacts are supposed to be automatically reported
			}

			// modify base chance according to attitude
			switch (pSoldier->aiData.bAttitude)
			{
			case DEFENSIVE:  iChance +=  20;  break;
			case BRAVESOLO:  iChance += -10;  break;
			case BRAVEAID:                    break;
			case CUNNINGSOLO:iChance +=  -5;  break;
			case CUNNINGAID:                  break;
			case AGGRESSIVE: iChance += -20;  break;
			case ATTACKSLAYONLY: iChance = 0; break;
			}

			if ((INT16)PreRandom(100) < iChance)
			{
				return(AI_ACTION_YELLOW_ALERT);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	// if our breath is running a bit low, and we're not in water
	if ((pSoldier->bBreath < 25) && !pSoldier->MercInWater())
	{
		// take a breather for gods sake!
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sNoiseGridNo,AI_ACTION_SEEK_NOISE);

	if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
	{
		if ( fClimb )
		{
			if ( pSoldier->sGridNo == sNoiseGridNo)
			{
				if (IsActionAffordable(pSoldier, pSoldier->aiData.bAction))
				{
					return( AI_ACTION_CLIMB_ROOF );
				}
				else if ( gfTurnBasedAI && pSoldier->bActionPoints < pSoldier->bInitialActionPoints )
				{
					return AI_ACTION_NONE;
				}
			}
			else
			{
				pSoldier->aiData.usActionData = sNoiseGridNo;							
				return( AI_ACTION_MOVE_TO_CLIMB  );
			}
		}
		else
		{
			return(AI_ACTION_SEEK_NOISE);
		}					
	}

	////////////////////////////////////////////////////////////////////////////
	// SWITCH TO GREEN: determine if soldier acts as if nothing at all was wrong
	////////////////////////////////////////////////////////////////////////////
	if ((INT16)PreRandom(100) < 50 )
	{
		// Skip YELLOW until new situation, 15% extra chance to do GREEN actions
		pSoldier->aiData.bBypassToGreen = 15;
		return(ZombieDecideActionGreen(pSoldier));
	}

	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////

	// by default, if everything else fails, just stands in place without turning
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}


INT8 ZombieDecideActionRed(SOLDIERTYPE *pSoldier)
{
	INT32	iDummy;
	INT32	sClosestOpponent,sClosestFriend;
	INT32	sClosestDisturbance, sDistVisible;
	INT32	sMostImportantNoise;
	INT32	sClosestThreat = NOWHERE;
	UINT8	ubCanMove,ubOpponentDir;
	INT8	bInWater, bInDeepWater;
	INT8	bSeekPts = 0, bHelpPts = 0, bWatchPts = 0;
	BOOLEAN	fClimb;	
	UINT8	ubThreatID = NOBODY;
	INT32	sThreatSpot = NOWHERE;
	INT8	bThreatLevel = 0;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ZombieDecideActionRed: soldier orders = %d",pSoldier->aiData.bOrders));

	DebugAI( AI_MSG_START, pSoldier, String("[Red Zombie]"));
	LogDecideInfo(pSoldier);
	//DebugAI( AI_MSG_INFO, pSoldier, String("AP=%d/%d %s %s %s %s %s", pSoldier->bActionPoints, pSoldier->bInitialActionPoints, gStr8AlertStatus[pSoldier->aiData.bAlertStatus], gStr8Orders[pSoldier->aiData.bOrders], gStr8Attitude[pSoldier->aiData.bAttitude], gStr8Team[pSoldier->bTeam], gStr8Class[pSoldier->ubSoldierClass]) );
	//DebugAI( AI_MSG_INFO, pSoldier, String("Health %d/%d Breath %d/%d Shock %d Tolerance %d AI Morale %d", pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pSoldier->aiData.bShock, CalcSuppressionTolerance(pSoldier), pSoldier->aiData.bAIMorale) );

	// if we have absolutely no action points, we can't do a thing under RED!
	if (!pSoldier->bActionPoints)
	{
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	// can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
	ubCanMove = (pSoldier->bActionPoints >= MinPtsToMove(pSoldier));

	// determine if we happen to be in water (in which case we're in BIG trouble!)
	bInWater = Water( pSoldier->sGridNo, pSoldier->pathing.bLevel );
	bInDeepWater = DeepWater( pSoldier->sGridNo, pSoldier->pathing.bLevel );
	
	DebugAI( AI_MSG_INFO, pSoldier, String("CanMove %d InWater %d InDeepWater %d", ubCanMove, bInWater, bInDeepWater));

	// calculate our morale
	pSoldier->aiData.bAIMorale = CalcMorale(pSoldier);

	////////////////////////////////////////////////////////////////////////
	// CROUCH & REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("[crouch and rest]"));

	// if our breath is running a bit low, and we're not in water or under fire
	if ((pSoldier->bBreath < 25) && !bInWater && !pSoldier->aiData.bUnderFire)
	{		
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: main red ai");
	////////////////////////////////////////////////////////////////////////////
	// MAIN RED AI
	////////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("Main Red AI"));

	// get the location of the closest reachable opponent
	//sClosestDisturbance = ClosestReachableDisturbance(pSoldier, &fClimb, ubThreatID, sThreatSpot, bThreatLevel);
	sClosestDisturbance = ClosestReachableDisturbance(pSoldier, &fClimb);
	sClosestOpponent = ClosestKnownOpponent(pSoldier, NULL, NULL);
	sMostImportantNoise = MostImportantNoiseHeard( pSoldier, NULL, NULL, NULL );

	DebugAI( AI_MSG_INFO, pSoldier, String("Closest Disturbance %d Closest Opponent %d Most Important Noise %d", sClosestDisturbance, sClosestOpponent, sMostImportantNoise));

	sClosestThreat = sClosestDisturbance;
	if(TileIsOutOfBounds( sClosestThreat ))
	{
		sClosestThreat = sClosestOpponent;
	}
	if( TileIsOutOfBounds( sClosestThreat ) )
	{
		sClosestThreat = sMostImportantNoise;
	}

	DebugAI( AI_MSG_INFO, pSoldier, String("Closest Threat %d", sClosestThreat));

	if( ubCanMove &&
		pSoldier->bActionPoints > APBPConstants[MAX_AP_CARRIED] &&
		!gfHiddenInterrupt )
	{		
		DebugAI( AI_MSG_INFO, pSoldier, String("[seek opponent]"));
		// if there is an opponent reachable					
		if (!TileIsOutOfBounds(sClosestDisturbance) )
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("found disturbance %d climb %d", sClosestDisturbance, fClimb));

			//////////////////////////////////////////////////////////////////////
			// SEEK CLOSEST DISTURBANCE: GO DIRECTLY TOWARDS CLOSEST KNOWN OPPONENT
			//////////////////////////////////////////////////////////////////////

			if (fClimb && pSoldier->sGridNo == sClosestDisturbance)
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("at climb point"));

				// wait for next turn if turnbased (to climb with max APs)
				if( gfTurnBasedAI &&
					pSoldier->bActionPoints < pSoldier->bInitialActionPoints )
				{
					pSoldier->aiData.bNextAction = AI_ACTION_NONE;
					return( AI_ACTION_END_TURN );
				}

				// climb
				if (IsActionAffordable(pSoldier, AI_ACTION_CLIMB_ROOF))
				{
					pSoldier->aiData.bNextAction = AI_ACTION_NONE;
					return( AI_ACTION_CLIMB_ROOF );
				}
				// cannot climb at all
			}

			// try to move towards him
			pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sClosestDisturbance, 0, AI_ACTION_SEEK_OPPONENT, 0);
			DebugAI( AI_MSG_INFO, pSoldier, String("found seek spot %d", pSoldier->aiData.usActionData));

			// if it's possible						
			if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("[move to seek spot]"));

				// sevenfm: try to advance using sight cover
				if( gfTurnBasedAI &&
					PythSpacesAway(pSoldier->sGridNo, sClosestDisturbance) < TACTICAL_RANGE * 2 &&
					PythSpacesAway(pSoldier->sGridNo, sClosestDisturbance) > TACTICAL_RANGE / 4 &&
					!SightCoverAtSpot(pSoldier, pSoldier->aiData.usActionData, TRUE))
				{
					INT32 sAdvanceSpot = FindAdvanceSpot(pSoldier, sClosestDisturbance, AI_ACTION_SEEK_OPPONENT, ADVANCE_SPOT_SIGHT_COVER, TRUE);
					DebugAI( AI_MSG_INFO, pSoldier, String("found advance spot %d", sAdvanceSpot));

					if( !TileIsOutOfBounds(sAdvanceSpot) )
					{
						DebugAI( AI_MSG_INFO, pSoldier, String("move to advance spot"));

						pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sAdvanceSpot, 0, AI_ACTION_SEEK_OPPONENT, 0);

						if( !TileIsOutOfBounds(pSoldier->aiData.usActionData) )
						{
							return(AI_ACTION_SEEK_OPPONENT);
						}						
					}

					// if we are in cover and new spot has no cover - wait for next turn
					if( SightCoverAtSpot(pSoldier, pSoldier->sGridNo, TRUE) &&
						gfTurnBasedAI && pSoldier->bActionPoints != pSoldier->bInitialActionPoints ) //&&
						//CountFriendsBlack(pSoldier, sClosestDisturbance) == 0 )
					{
						DebugAI( AI_MSG_INFO, pSoldier, String("check turn to closest disturbance before ending turn"));
						// first turn to closest threat
						if( GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
						{
							// determine direction from this soldier to the closest opponent
							ubOpponentDir = AIDirection(pSoldier->sGridNo, sClosestDisturbance);

							DebugAI( AI_MSG_INFO, pSoldier, String("soldier direction %d disturbance direction %d", pSoldier->ubDirection, ubOpponentDir));

							sDistVisible = pSoldier->GetMaxDistanceVisible(sClosestThreat, 0, CALC_FROM_ALL_DIRS );

							if( pSoldier->ubDirection != ubOpponentDir &&
								PythSpacesAway(pSoldier->sGridNo,sClosestThreat) <= sDistVisible &&
								pSoldier->InternalIsValidStance( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
							{
								pSoldier->aiData.usActionData = ubOpponentDir;
								return(AI_ACTION_CHANGE_FACING);								
							}
							DebugAI( AI_MSG_INFO, pSoldier, String("cannot turn"));
						}

						DebugAI( AI_MSG_INFO, pSoldier, String("wait for next turn"));
						//ScreenMsg(FONT_GREEN, MSG_INTERFACE, L"[%d] wait for next turn", pSoldier->ubID);
						return AI_ACTION_NONE;
					}
				}

				return(AI_ACTION_SEEK_OPPONENT);
			}
		}

		DebugAI( AI_MSG_INFO, pSoldier, String("[seek friend]"));
		// if no opponent - go to closest friend
		sClosestFriend = ClosestReachableFriendInTrouble(pSoldier, &fClimb );

		if (!TileIsOutOfBounds(sClosestFriend) )
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("found friend %d climb %d", sClosestFriend, fClimb));

			//////////////////////////////////////////////////////////////////////
			// GO DIRECTLY TOWARDS CLOSEST FRIEND UNDER FIRE OR WHO LAST RADIOED
			//////////////////////////////////////////////////////////////////////			

			if ( fClimb && pSoldier->sGridNo == sClosestFriend )
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("at climb spot"));

				// wait for next turn to have full APs
				if( gfTurnBasedAI && pSoldier->bActionPoints < pSoldier->bInitialActionPoints )
				{
					return( AI_ACTION_END_TURN);
				}
				// climb
				if( IsActionAffordable(pSoldier, AI_ACTION_CLIMB_ROOF) )
				{
					return( AI_ACTION_CLIMB_ROOF );
				}
				// don't have enough AP for climbing
			}
			else
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("move to friend %d", sClosestFriend));
				// try to move towards him
				pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sClosestFriend, 0, AI_ACTION_SEEK_FRIEND, 0);
				DebugAI( AI_MSG_INFO, pSoldier, String("found move spot %d", pSoldier->aiData.usActionData));

				// if it's possible						
				if ( !TileIsOutOfBounds(pSoldier->aiData.usActionData) )
				{
					return(AI_ACTION_SEEK_FRIEND);
				}
				else
				{
					DebugAI( AI_MSG_INFO, pSoldier, String("cannot move!"));
				}
			}			
		}

		DebugAI( AI_MSG_INFO, pSoldier, String("[hide]"));

		// if cannot seek or help - hide
		if( gfTurnBasedAI &&
			pSoldier->bActionPoints == pSoldier->bInitialActionPoints &&
			(!SightCoverAtSpot(pSoldier, pSoldier->sGridNo, TRUE) || pSoldier->aiData.bUnderFire) )
		{
			pSoldier->aiData.usActionData = FindBestNearbyCover(pSoldier, pSoldier->aiData.bAIMorale, &iDummy);			

			if( !TileIsOutOfBounds(pSoldier->aiData.usActionData) )
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("found cover spot %d", pSoldier->aiData.usActionData));
				return AI_ACTION_TAKE_COVER;
			}
			else if (pSoldier->aiData.bUnderFire)
			{
				pSoldier->aiData.usActionData = FindRetreatSpot(pSoldier);

				if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
				{
					DebugAI(AI_MSG_INFO, pSoldier, String("found retreat spot %d", pSoldier->aiData.usActionData));
					return AI_ACTION_TAKE_COVER;
				}
			}
		}
	}

	DebugAI( AI_MSG_INFO, pSoldier, String("[look around towards opponent]"));
	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD CLOSEST KNOWN OPPONENT, IF KNOWN
	////////////////////////////////////////////////////////////////////////////

	if( GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints &&
		!TileIsOutOfBounds(sClosestThreat) )
	{
		// determine direction from this soldier to the closest opponent
		ubOpponentDir = AIDirection(pSoldier->sGridNo, sClosestThreat);

		DebugAI( AI_MSG_INFO, pSoldier, String("soldier direction %d opponent direction %d", pSoldier->ubDirection, ubOpponentDir));

		// if soldier is not already facing in that direction,
		// and the opponent is close enough that he could possibly be seen
		// note, have to change this to use the level returned from ClosestKnownOpponent
		sDistVisible = pSoldier->GetMaxDistanceVisible(sClosestThreat, 0, CALC_FROM_ALL_DIRS );

		if( pSoldier->ubDirection != ubOpponentDir &&
			PythSpacesAway(pSoldier->sGridNo,sClosestThreat) <= sDistVisible &&
			pSoldier->InternalIsValidStance( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
		{
			pSoldier->aiData.usActionData = ubOpponentDir;
			return(AI_ACTION_CHANGE_FACING);			
		}
		DebugAI( AI_MSG_INFO, pSoldier, String("cannot turn"));
	}

	////////////////////////////////////////////////////////////////////////////
	// SWITCH TO GREEN: soldier does ordinary regular patrol, seeks friends
	////////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("[switch to green]"));

	// if not in combat or under fire, and we COULD have moved, just chose not to	
	if( (pSoldier->aiData.bAlertStatus != STATUS_BLACK) &&
		!pSoldier->aiData.bUnderFire &&
		ubCanMove &&
		(!gfTurnBasedAI || pSoldier->bActionPoints == pSoldier->bInitialActionPoints) &&
		TileIsOutOfBounds(sClosestDisturbance) )
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("decide green"));

		// Skip RED until new situation/next turn, 30% extra chance to do GREEN actions
		pSoldier->aiData.bBypassToGreen = 30;
		return(ZombieDecideActionGreen(pSoldier) );
	}

	////////////////////////////////////////////////////////////////////////////
	// IF UNDER FIRE, FACE THE MOST IMPORTANT NOISE WE KNOW
	////////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("[face the most important noise]"));

	if( pSoldier->aiData.bUnderFire &&
		pSoldier->CheckInitialAP() &&
		//pSoldier->bActionPoints == pSoldier->bInitialActionPoints &&
		GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints &&
		!TileIsOutOfBounds( sMostImportantNoise ) )
	{
		ubOpponentDir = AIDirection(pSoldier->sGridNo, sMostImportantNoise);
		DebugAI( AI_MSG_INFO, pSoldier, String("soldier direction %d noise direction %d", pSoldier->ubDirection, ubOpponentDir));

		if ( pSoldier->ubDirection != ubOpponentDir &&
			pSoldier->InternalIsValidStance( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ))
		{
			pSoldier->aiData.usActionData = ubOpponentDir;
			return( AI_ACTION_CHANGE_FACING );
		}
		DebugAI( AI_MSG_INFO, pSoldier, String("cannot turn"));
	}

	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////
	DebugAI( AI_MSG_INFO, pSoldier, String("nothing to do!"));

	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

INT8 ZombieDecideActionBlack(SOLDIERTYPE *pSoldier)
{
	INT32	sClosestOpponent;
	INT32	sClosestDisturbance;
	INT16	ubMinAPCost;
	UINT8	ubCanMove;
	INT8	bInWater,bInDeepWater;
	INT8	bDirection;
	UINT8	ubBestAttackAction = AI_ACTION_NONE;
	INT8	bCanAttack;
	INT8	bWeaponIn;
	BOOLEAN	fClimb;
	INT32	iDummy;
	UINT8	ubThreatID = NOBODY;
	INT32	sThreatSpot = NOWHERE;
	INT8	bThreatLevel = 0;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionBlack: soldier = %d, orders = %d, attitude = %d",pSoldier->ubID,pSoldier->aiData.bOrders,pSoldier->aiData.bAttitude));

	DebugAI( AI_MSG_START, pSoldier, String("[Black Zombie]"));
	LogDecideInfo(pSoldier);
	//DebugAI( AI_MSG_INFO, pSoldier, String("AP=%d/%d %s %s %s %s %s", pSoldier->bActionPoints, pSoldier->bInitialActionPoints, gStr8AlertStatus[pSoldier->aiData.bAlertStatus], gStr8Orders[pSoldier->aiData.bOrders], gStr8Attitude[pSoldier->aiData.bAttitude], gStr8Team[pSoldier->bTeam], gStr8Class[pSoldier->ubSoldierClass]) );
	//DebugAI( AI_MSG_INFO, pSoldier, String("Health %d/%d Breath %d/%d Shock %d Tolerance %d AI Morale %d", pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pSoldier->aiData.bShock, CalcSuppressionTolerance(pSoldier), pSoldier->aiData.bAIMorale) );

	ATTACKTYPE BestStab, BestAttack;
	BOOLEAN fAllowCoverCheck = FALSE;

	// if we have absolutely no action points, we can't do a thing under BLACK!
	if (pSoldier->bActionPoints <= 0)
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("no AP! nothing to do"));
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	// can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
	ubCanMove = (pSoldier->bActionPoints >= MinPtsToMove(pSoldier));

	// determine if we happen to be in water (in which case we're in BIG trouble!)
	bInWater = Water( pSoldier->sGridNo, pSoldier->pathing.bLevel );
	bInDeepWater = WaterTooDeepForAttacks( pSoldier->sGridNo, pSoldier->pathing.bLevel );

	// calculate our morale
	pSoldier->aiData.bAIMorale = CalcMorale(pSoldier);

	DebugAI( AI_MSG_INFO, pSoldier, String("CanMove %d InWater %d InDeepWater %d", ubCanMove, bInWater, bInDeepWater));
	////////////////////////////////////////////////////////////////////////////
	// STUCK IN WATER OR GAS, NO COVER, GO TO NEAREST SPOT OF UNGASSED LAND
	////////////////////////////////////////////////////////////////////////////

	// if soldier in water/gas has enough APs left to move at least 1 square
	if ( bInDeepWater && ubCanMove)
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("in deep water: find nearest land"));

		pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

		if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("leave deep water: move to %d", pSoldier->aiData.usActionData));
			return(AI_ACTION_LEAVE_WATER_GAS);
		}

		DebugAI( AI_MSG_INFO, pSoldier, String("couldn't find ANY land within 25 tiles"));
		// couldn't find ANY land within 25 tiles(!), this should never happen...
		// look for best place to RUN AWAY to (farthest from the closest threat)
		pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

		if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("move to %d", pSoldier->aiData.usActionData));
			return(AI_ACTION_RUN_AWAY);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// SOLDIER CAN ATTACK IF NOT IN WATER/GAS AND NOT DOING SOMETHING TOO FUNKY
	////////////////////////////////////////////////////////////////////////////

	// zombie can always attack
	bCanAttack = TRUE;

	// NPCs in deep water cannot attack
	if ((pSoldier->bActionPoints < 2) || bInDeepWater)
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("low AP or in deep water - cannot attack"));
		bCanAttack = FALSE;
	}

	BestStab.ubPossible  = FALSE;	// by default, assume Stabbing isn't possible

	// if we are able attack
	if (bCanAttack)
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("[attack]"));
		pSoldier->bAimShotLocation = AIM_SHOT_RANDOM;

		// nothing (else) to attack with so let's try hand-to-hand
		bWeaponIn = FindObj( pSoldier, NOTHING, HANDPOS, NUM_INV_SLOTS );

		if (bWeaponIn != NO_SLOT)
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("found weapon in slot %d", bWeaponIn));

			BestStab.bWeaponIn = bWeaponIn;
			// if it's in his holster, swap it into his hand temporarily
			if (bWeaponIn != HANDPOS)
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("rearrange pocket temporarily"));
				RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
			}

			// get the minimum cost to attack by HTH
			ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,DONTADDTURNCOST,0,0);
			DebugAI( AI_MSG_INFO, pSoldier, String("min AP to attack %d", ubMinAPCost));

			// if we can afford the minimum AP cost to use HTH combat
			if (pSoldier->bActionPoints >= ubMinAPCost)
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("[find target for attack]"));

				// then look around for a worthy target (which sets BestStab.ubPossible)
				CalcBestStab(pSoldier,&BestStab, FALSE);

				if (BestStab.ubPossible && pSoldier->ubSoldierClass == SOLDIER_CLASS_ZOMBIE)
				{
					//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"[%d] can attack %s", pSoldier->ubID, MercPtrs[BestStab.ubOpponent]->GetName());
					// now we KNOW FOR SURE that we will do something (stab, at least)
					NPCDoesAct(pSoldier);
					ubBestAttackAction = AI_ACTION_KNIFE_MOVE;
					DebugAI( AI_MSG_INFO, pSoldier, String("NPC decided to move to stab"));
				}
			}

			// if it was in his holster, swap it back into his holster for now
			if (bWeaponIn != HANDPOS)
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("rearrange pocket back"));
				RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// CHOOSE THE BEST TYPE OF ATTACK OUT OF THOSE FOUND TO BE POSSIBLE
		//////////////////////////////////////////////////////////////////////////

		// copy the information on the best action selected into BestAttack struct		
		DebugAI( AI_MSG_INFO, pSoldier, String("copy the information on the best action selected into BestAttack struct"));
		switch (ubBestAttackAction)
		{
		case AI_ACTION_KNIFE_MOVE:
			DebugAI( AI_MSG_INFO, pSoldier, String("best attack = stab"));
			memcpy(&BestAttack,&BestStab,sizeof(BestAttack));
			break;

		default:
			// set to empty
			DebugAI( AI_MSG_INFO, pSoldier, String("best attack = no good attack"));
			memset( &BestAttack, 0, sizeof( BestAttack ) );
			break;
		}
	}

	// if attack is still desirable (meaning it's also preferred to taking cover)
	if (ubBestAttackAction != AI_ACTION_NONE)
	{
		DebugAI( AI_MSG_INFO, pSoldier, String("prepare attack, AP cost %d CTH %d", BestAttack.ubAPCost, BestAttack.ubChanceToReallyHit));

		pSoldier->aiData.bAimTime = BestAttack.ubAimTime;

		// sevenfm: dynamically decide stab location
		if( BestAttack.ubOpponent != NOBODY )
		{
			UINT32	uiRoll;
			UINT8	ubChanceHead = 0;
			UINT16	usRealCTH = BestAttack.ubChanceToReallyHit;

			// attack to head randomly
			if( gAnimControl[ MercPtrs[BestAttack.ubOpponent]->usAnimState ].ubEndHeight != ANIM_PRONE )
			{	
				ubChanceHead = 6;
				ubChanceHead += usRealCTH / 2;
				ubChanceHead = ubChanceHead * usRealCTH / 100;
			}
			DebugAI( AI_MSG_INFO, pSoldier, String("ubRealCTH %d", usRealCTH));
			DebugAI( AI_MSG_INFO, pSoldier, String("ubChanceHead %d", ubChanceHead));

			// randomly decide hit location
			uiRoll = PreRandom( 100 );					

			if (uiRoll > 100 - ubChanceHead)
			{
				pSoldier->bAimShotLocation = AIM_SHOT_HEAD;
				DebugAI( AI_MSG_INFO, pSoldier, String("aim at head"));
			}
			else
			{
				pSoldier->bAimShotLocation = AIM_SHOT_TORSO;
				DebugAI( AI_MSG_INFO, pSoldier, String("aim at torso"));
			}
		}

		DebugAI( AI_MSG_INFO, pSoldier, String("AimTime %d AimShotLocation %d", pSoldier->aiData.bAimTime, pSoldier->bAimShotLocation));

		// swap weapon to hand if necessary
		if (BestAttack.bWeaponIn != HANDPOS)
		{
			DebugAI( AI_MSG_INFO, pSoldier, String("rearrange pocket for attack"));
			RearrangePocket(pSoldier,HANDPOS,BestAttack.bWeaponIn,FOREVER);
		}

		pSoldier->aiData.usActionData = BestAttack.sTarget;
		pSoldier->bTargetLevel = BestAttack.bTargetLevel;

		DebugAI( AI_MSG_INFO, pSoldier, String("attack opponent %d at gridno %d level %d", BestAttack.ubOpponent, pSoldier->aiData.usActionData, pSoldier->bTargetLevel));

		return(ubBestAttackAction);
	}

	DebugAI( AI_MSG_INFO, pSoldier, String("[go to closest deisturbance]"));
	if( ubCanMove &&
		!gfHiddenInterrupt &&
		pSoldier->bActionPoints > APBPConstants[MAX_AP_CARRIED] )
	{
		//sClosestDisturbance = ClosestReachableDisturbance(pSoldier, &fClimb, ubThreatID, sThreatSpot, bThreatLevel);		
		sClosestDisturbance = ClosestReachableDisturbance(pSoldier, &fClimb);
		DebugAI( AI_MSG_INFO, pSoldier, String("found disturbance %d climb %d", sClosestDisturbance, fClimb));

		// if there is an opponent reachable					
		if (!TileIsOutOfBounds(sClosestDisturbance))
		{
			//////////////////////////////////////////////////////////////////////
			// SEEK CLOSEST DISTURBANCE: GO DIRECTLY TOWARDS CLOSEST KNOWN OPPONENT
			//////////////////////////////////////////////////////////////////////
			
			if (fClimb && pSoldier->sGridNo == sClosestDisturbance)
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("at climb point"));

				// wait for next turn if turnbased (to climb with max APs)
				if( gfTurnBasedAI &&
					pSoldier->bActionPoints < pSoldier->bInitialActionPoints )
				{
					pSoldier->aiData.bNextAction = AI_ACTION_NONE;
					return( AI_ACTION_END_TURN );
				}

				// climb
				if (IsActionAffordable(pSoldier, AI_ACTION_CLIMB_ROOF))
				{
					pSoldier->aiData.bNextAction = AI_ACTION_NONE;
					return( AI_ACTION_CLIMB_ROOF );
				}
				// cannot climb at all
			}
			else
			{
				DebugAI( AI_MSG_INFO, pSoldier, String("try to move towards disturbance"));

				// try to move towards him
				pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sClosestDisturbance, 0, AI_ACTION_SEEK_OPPONENT, 0);
				DebugAI( AI_MSG_INFO, pSoldier, String("found spot %d", pSoldier->aiData.usActionData));

				// if it's possible
				if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
				{
					// sevenfm: try to advance using sight cover
					if( gfTurnBasedAI &&
						PythSpacesAway(pSoldier->sGridNo, sClosestDisturbance) < TACTICAL_RANGE * 2 &&
						PythSpacesAway(pSoldier->sGridNo, sClosestDisturbance) > TACTICAL_RANGE / 4 &&
						!SightCoverAtSpot(pSoldier, pSoldier->aiData.usActionData, TRUE))
					{
						DebugAI( AI_MSG_INFO, pSoldier, String("[advance spot]"));

						INT32 sAdvanceSpot = FindAdvanceSpot(pSoldier, sClosestDisturbance, AI_ACTION_SEEK_OPPONENT, ADVANCE_SPOT_SIGHT_COVER, TRUE);
						DebugAI( AI_MSG_INFO, pSoldier, String("found advance spot %d", sAdvanceSpot));

						if( !TileIsOutOfBounds(sAdvanceSpot) )
						{
							DebugAI( AI_MSG_INFO, pSoldier, String("advance to %d", sAdvanceSpot));

							pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sAdvanceSpot, 0, AI_ACTION_SEEK_OPPONENT, 0);

							if( !TileIsOutOfBounds(pSoldier->aiData.usActionData) )
							{
								return(AI_ACTION_SEEK_OPPONENT);
							}							
						}
						else if( SightCoverAtSpot(pSoldier, pSoldier->sGridNo, TRUE) &&
								gfTurnBasedAI &&
								pSoldier->bActionPoints < pSoldier->bInitialActionPoints)
						{
							DebugAI( AI_MSG_INFO, pSoldier, String("cannot find advance spot, wait for next turn in sight cover"));
							return AI_ACTION_NONE;
						}
					}

					return(AI_ACTION_SEEK_OPPONENT);
				}
			}			
		}

		DebugAI( AI_MSG_INFO, pSoldier, String("[hide]"));

		// if cannot seek or help - hide
		if( gfTurnBasedAI &&
			pSoldier->bActionPoints == pSoldier->bInitialActionPoints &&
			(!SightCoverAtSpot(pSoldier, pSoldier->sGridNo, TRUE) || pSoldier->aiData.bUnderFire) )
		{
			pSoldier->aiData.usActionData = FindBestNearbyCover(pSoldier, pSoldier->aiData.bAIMorale, &iDummy);						

			if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
			{
				DebugAI(AI_MSG_INFO, pSoldier, String("found hide spot %d", pSoldier->aiData.usActionData));
				return AI_ACTION_TAKE_COVER;
			}
			else if (pSoldier->aiData.bUnderFire)
			{
				pSoldier->aiData.usActionData = FindRetreatSpot(pSoldier);
				
				if (!TileIsOutOfBounds(pSoldier->aiData.usActionData))
				{
					DebugAI(AI_MSG_INFO, pSoldier, String("found retreat spot %d", pSoldier->aiData.usActionData));
					return AI_ACTION_TAKE_COVER;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// TURN TO FACE CLOSEST KNOWN OPPONENT (IF NOT FACING THERE ALREADY)
	////////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("[turn to closest opponent]"));

	if( GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
	{
		sClosestOpponent = ClosestSeenOpponent(pSoldier, NULL, NULL);
		DebugAI( AI_MSG_INFO, pSoldier, String("closest seen opponent %d", sClosestOpponent));

		// if we have a closest seen opponent
		if (!TileIsOutOfBounds(sClosestOpponent))
		{
			bDirection = AIDirection(pSoldier->sGridNo, sClosestOpponent);
			DebugAI( AI_MSG_INFO, pSoldier, String("soldier direction %d opponent direction %d", pSoldier->ubDirection, bDirection));

			// if we're not facing towards him
			if( pSoldier->ubDirection != bDirection && 
				pSoldier->InternalIsValidStance( bDirection, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = bDirection;

				return(AI_ACTION_CHANGE_FACING);
			}
			DebugAI( AI_MSG_INFO, pSoldier, String("cannot turn"));
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////

	DebugAI( AI_MSG_INFO, pSoldier, String("nothing to do!"));

	// by default, if everything else fails, just stand in place and wait
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

INT8 ZombieDecideAction( SOLDIERTYPE *pSoldier )
{
	INT8 bAction = AI_ACTION_NONE;

	switch (pSoldier->aiData.bAlertStatus)
	{
	case STATUS_GREEN:
		bAction = ZombieDecideActionGreen(pSoldier);
		break;

	case STATUS_YELLOW:
		bAction = ZombieDecideActionYellow(pSoldier);
		break;

	case STATUS_RED:
		bAction = ZombieDecideActionRed(pSoldier);
		break;

	case STATUS_BLACK:
		bAction = ZombieDecideActionBlack(pSoldier);
		break;
	}

#ifdef DEBUGDECISIONS
	STR tempstr;
	sprintf(tempstr, "ZombieDecideAction: selected action %d, actionData %d\n\n", bAction, pSoldier->aiData.usActionData);
	DebugAI(tempstr);
#endif

	return(bAction);
}

/*void ZombieDecideAlertStatus( SOLDIERTYPE *pSoldier )
{
	INT8	bOldStatus;
	INT32	iDummy;
	BOOLEAN	fClimbDummy, fReachableDummy;

	// THE FOUR (4) POSSIBLE ALERT STATUSES ARE:
	// GREEN - No one sensed, no suspicious noise heard, go about doing regular stuff
	// YELLOW - Suspicious noise was heard personally
	// RED - Either saw OPPONENTS in person, or definite contact had been called
	// BLACK - Currently has one or more OPPONENTS in sight

	// save the man's previous status
	bOldStatus = pSoldier->aiData.bAlertStatus;

	// determine the current alert status for this category of man
	if (pSoldier->aiData.bOppCnt > 0)		// opponent(s) in sight
	{		
		pSoldier->aiData.bAlertStatus = STATUS_BLACK;
	}
	else // no opponents are in sight
	{
		switch (bOldStatus)
		{
		case STATUS_BLACK:
			// then drop back to RED status
			pSoldier->aiData.bAlertStatus = STATUS_RED;
			break;

		case STATUS_RED:
			// RED can never go back down below RED, only up to BLACK
			// TODO: perhaps zombies might forget their target?	They are pretty dumb after all...
			break;

		case STATUS_YELLOW:
			// if all enemies have been RED alerted, or we're under fire
			if (gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition || pSoldier->aiData.bUnderFire)
			{
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				// if we are NOT aware of any uninvestigated noises right now
				// and we are not currently in the middle of an action
				// (could still be on his way heading to investigate a noise!)					
				if (( TileIsOutOfBounds(MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy))) && !pSoldier->aiData.bActionInProgress)
				{
					// then drop back to GREEN status
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
				}
			}
			break;

		case STATUS_GREEN:
			// if all enemies have been RED alerted, or we're under fire
			if (gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition || pSoldier->aiData.bUnderFire)
			{
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				// if we ARE aware of any uninvestigated noises right now					
				if ( !TileIsOutOfBounds(MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy)))
				{
					// then move up to YELLOW status
					pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				}
			}
			break;
		}
		// otherwise, RED stays RED, YELLOW stays YELLOW, GREEN stays GREEN
	}

	// if the creatures alert status has changed in any way
	if (pSoldier->aiData.bAlertStatus != bOldStatus)
	{
		// HERE ARE TRYING TO AVOID NPCs SHUFFLING BACK & FORTH BETWEEN RED & BLACK
		// if either status is < RED (ie. anything but RED->BLACK && BLACK->RED)
		if ((bOldStatus < STATUS_RED) || (pSoldier->aiData.bAlertStatus < STATUS_RED))
		{
			// force a NEW action decision on next pass through HandleManAI()
			SetNewSituation( pSoldier );
		}

		// if this guy JUST discovered that there were opponents here for sure...
		if ((bOldStatus < STATUS_RED) && (pSoldier->aiData.bAlertStatus >= STATUS_RED))
		{
			// might want to make custom to let them go anywhere
			CheckForChangingOrders(pSoldier);
		}
	}
	else	// status didn't change
	{
		// if a guy on status GREEN or YELLOW is running low on breath
		if (((pSoldier->aiData.bAlertStatus == STATUS_GREEN)	&& (pSoldier->bBreath < 75)) ||
			((pSoldier->aiData.bAlertStatus == STATUS_YELLOW) && (pSoldier->bBreath < 50)))
		{
			// as long as he's not in water (standing on a bridge is OK)
			if (!pSoldier->MercInWater())
			{
				// force a NEW decision so that he can get some rest
				SetNewSituation( pSoldier );

				// current action will be canceled. if noise is no longer important				
				if ((pSoldier->aiData.bAlertStatus == STATUS_YELLOW) &&
					(TileIsOutOfBounds(MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy))))
				{
					// then drop back to GREEN status
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
					CheckForChangingOrders(pSoldier);
				}
			}
		}
	}
}*/
