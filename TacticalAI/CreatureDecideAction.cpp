#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "types.h"
	//#include "soldier control.h"
	#include "ai.h"
	#include "AIInternals.h"
	#include "opplist.h"
	#include "Items.h"
	#include "Rotting Corpses.h"
	#include "soldier add.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


extern INT8 STRAIGHT; //lal

#define CAN_CALL( s ) (s->ubBodyType != BLOODCAT && s->ubBodyType != LARVAE_MONSTER && s->ubBodyType != INFANT_MONSTER)
#define CAN_LISTEN_TO_CALL( s ) (s->ubBodyType != BLOODCAT && s->ubBodyType != LARVAE_MONSTER)

enum
{
	CALLER_FEMALE	= 0,
	CALLER_MALE,
	CALLER_INFANT,
	CALLER_QUEEN,
	NUM_CREATURE_CALLERS
} CreatureCaller;

enum
{
	CREATURE_MOBILE = 0,
	CREATURE_CRAWLER,
	CREATURE_IMMOBILE
} CreatureMobility;

#define FRENZY_THRESHOLD 8
#define MAX_EAT_DIST 5

INT8 gbCallPriority[NUM_CREATURE_CALLS][NUM_CREATURE_CALLERS] =
{
	{0, 0, 0 },//CALL_NONE
	{3, 5, 12},//CALL_1_PREY
	{5, 9, 12},//CALL_MULTIPLE_PREY
	{4, 7, 12},//CALL_ATTACKED
	{6, 9, 12},//CALL_CRIPPLED
};

INT8 gbHuntCallPriority[NUM_CREATURE_CALLS] =
{
	4, //CALL_1_PREY
	5, //CALL_MULTIPLE_PREY
	7, //CALL_ATTACKED
	8	//CALL_CRIPPLED
};

#define PRIORITY_DECR_DISTANCE 30

#define CALL_1_OPPONENT CALL_1_PREY
#define CALL_MULTIPLE_OPPONENT CALL_MULTIPLE_PREY

void CreatureCall( SOLDIERTYPE * pCaller )
{
	UINT8		ubCallerType=0;
	UINT8		ubReceiver;
	INT8		bFullPriority;
	INT8		bPriority;
	SOLDIERTYPE * pReceiver;
	UINT16	usDistToCaller;
	// communicate call to all creatures on map through ultrasonics

	gTacticalStatus.Team[pCaller->bTeam].bAwareOfOpposition = TRUE;
	// bAction should be AI_ACTION_CREATURE_CALL (new)
	// usActionData is call enum #
	switch (pCaller->ubBodyType)
	{
		case ADULTFEMALEMONSTER:
		case YAF_MONSTER:
			ubCallerType = CALLER_FEMALE;
			break;
		case QUEENMONSTER:
			ubCallerType = CALLER_QUEEN;
			break;
		// need to add male
		case AM_MONSTER:
		case YAM_MONSTER:
			ubCallerType = CALLER_MALE;
			break;
		default:
			ubCallerType = CALLER_FEMALE;
			break;
	}
	if (pCaller->aiData.bHunting) // which should only be set for females outside of the hive
	{
		bFullPriority = gbHuntCallPriority[pCaller->aiData.usActionData];
	}
	else
	{
		bFullPriority = gbCallPriority[pCaller->aiData.usActionData][ubCallerType];
	}

	// OK, do animation based on body type...
	switch (pCaller->ubBodyType)
	{
		case ADULTFEMALEMONSTER:
		case YAF_MONSTER:
		case AM_MONSTER:
		case YAM_MONSTER:

			pCaller->EVENT_InitNewSoldierAnim( MONSTER_UP, 0 , FALSE );
			break;

		case QUEENMONSTER:

			pCaller->EVENT_InitNewSoldierAnim( QUEEN_CALL, 0 , FALSE );
			break;
	}


	for (ubReceiver = gTacticalStatus.Team[ pCaller->bTeam ].bFirstID; ubReceiver <= gTacticalStatus.Team[ pCaller->bTeam ].bLastID; ubReceiver++)
	{
		pReceiver = MercPtrs[ubReceiver];
		if (pReceiver->bActive && pReceiver->bInSector && (pReceiver->stats.bLife >= OKLIFE) && (pReceiver != pCaller) && (pReceiver->aiData.bAlertStatus < STATUS_BLACK))
		{
			if (pReceiver->ubBodyType != LARVAE_MONSTER && pReceiver->ubBodyType != INFANT_MONSTER && pReceiver->ubBodyType != QUEENMONSTER)
			{
				usDistToCaller = PythSpacesAway( pReceiver->sGridNo, pCaller->sGridNo );
				bPriority = bFullPriority - (INT8) (usDistToCaller / PRIORITY_DECR_DISTANCE);
				if (bPriority > pReceiver->aiData.bCallPriority)
				{
					pReceiver->aiData.bCallPriority = bPriority;
					pReceiver->aiData.bAlertStatus = STATUS_RED; // our status can't be more than red to begin with
					pReceiver->aiData.ubCaller = pCaller->ubID;
					pReceiver->aiData.sCallerGridNo = pCaller->sGridNo;
					pReceiver->aiData.bCallActedUpon = FALSE;
					CancelAIAction(pReceiver, FORCE);
					if ((bPriority > FRENZY_THRESHOLD) && (pReceiver->ubBodyType == ADULTFEMALEMONSTER || pReceiver->ubBodyType == YAF_MONSTER))
					{
						// go berzerk!
						pReceiver->aiData.bFrenzied = TRUE;
					}
				}
			}
		}
	}
}

INT8 CreatureDecideActionGreen( SOLDIERTYPE * pSoldier )
{
	INT32		iChance, iSneaky = 10;
	//INT8		bInWater;
	INT8		bInGas;

	//bInWater = pSoldier->MercInWater();

	// NB creatures would ignore smoke completely :-)

	if ( pSoldier->aiData.bMobility == CREATURE_CRAWLER && pSoldier->bActionPoints < pSoldier->bInitialActionPoints)
	{
		return( AI_ACTION_NONE );
	}

	bInGas = InGas( pSoldier, pSoldier->sGridNo );

	if (pSoldier->aiData.bMobility == CREATURE_MOBILE)
	{

		if (TrackScent( pSoldier ))
		{
			return( AI_ACTION_TRACK );
		}

		////////////////////////////////////////////////////////////////////////////
		// POINT PATROL: move towards next point unless getting a bit winded
		////////////////////////////////////////////////////////////////////////////

		// this takes priority over water/gas checks, so that point patrol WILL work
		// from island to island, and through gas covered areas, too
		if ((pSoldier->aiData.bOrders == POINTPATROL) && (pSoldier->bBreath >= 50))
		{
			if (PointPatrolAI(pSoldier))
			{
				if (!gfTurnBasedAI)
				{
					// pause at the end of the walk!
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
				}

				return(AI_ACTION_POINT_PATROL);
			}
		}

		if ((pSoldier->aiData.bOrders == RNDPTPATROL) && (pSoldier->bBreath >=50))
		{
			if (RandomPointPatrolAI(pSoldier))
			{
				if (!gfTurnBasedAI)
				{
					// pause at the end of the walk!
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
				}

				return(AI_ACTION_POINT_PATROL);
			}
		}

		////////////////////////////////////////////////////////////////////////////
		// WHEN LEFT IN WATER OR GAS, GO TO NEAREST REACHABLE SPOT OF UNGASSED LAND
		////////////////////////////////////////////////////////////////////////////

		if ( /*bInWater || */ bInGas)
		{
			pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
	#ifdef DEBUGDECISIONS
				STR16 tempstr;
				sprintf(tempstr,"%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
				AIPopMessage(tempstr);
	#endif

				return(AI_ACTION_LEAVE_WATER_GAS);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	// if our breath is running a bit low, and we're not in the way or in water
	if ((pSoldier->bBreath < 75) /*&& !bInWater*/)
	{
		// take a breather for gods sake!
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	////////////////////////////////////////////////////////////////////////////
	// RANDOM PATROL:	determine % chance to start a new patrol route
	////////////////////////////////////////////////////////////////////////////

	if (pSoldier->aiData.bMobility != CREATURE_IMMOBILE )
	{
		iChance = 25;

		// set base chance according to orders
		switch (pSoldier->aiData.bOrders)
		{
			case STATIONARY:	 iChance += -20;	break;
			case ONGUARD:		iChance += -15;	break;
			case ONCALL:							break;
			case CLOSEPATROL:	iChance += +15;	break;
			case RNDPTPATROL:
			case POINTPATROL:	iChance	=	0;	break;
			case FARPATROL:		iChance += +25;	break;
			case SEEKENEMY:		iChance += -10;	break;
		}

		// modify chance of patrol (and whether it's a sneaky one) by attitude
		switch (pSoldier->aiData.bAttitude)
		{
			case DEFENSIVE:		iChance += -10;				 break;
			case BRAVESOLO:		iChance +=	5;				 break;
			case BRAVEAID:										break;
			case CUNNINGSOLO:	iChance +=	5;	iSneaky += 10; break;
			case CUNNINGAID:						iSneaky +=	5; break;
			case AGGRESSIVE:	 iChance +=	10;	iSneaky += -5; break;
		}

		// reduce chance for any injury, less likely to wander around when hurt
		iChance -= (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);

		// reduce chance if breath is down, less likely to wander around when tired
		iChance -= (100 - pSoldier->bBreath);

		// if we're in water with land miles (> 25 tiles) away,
		// OR if we roll under the chance calculated
		if ( /*bInWater ||*/ ((INT16) PreRandom(100) < iChance))
		{
			pSoldier->aiData.usActionData = RandDestWithinRange(pSoldier);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
	#ifdef DEBUGDECISIONS
			STR16 tempstr;
			sprintf(tempstr,"%s - RANDOM PATROL to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			 AIPopMessage(tempstr);
	#endif

				if (!gfTurnBasedAI)
				{
					// pause at the end of the walk!
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
					if (pSoldier->aiData.bMobility == CREATURE_CRAWLER)
					{
						pSoldier->aiData.usNextActionData *= 2;
					}
				}

				return(AI_ACTION_RANDOM_PATROL);
			}
		}

		/*
		if (pSoldier->aiData.bMobility == CREATURE_MOBILE)
		{
			////////////////////////////////////////////////////////////////////////////
			// SEEK FRIEND: determine %chance for man to pay a friendly visit
			////////////////////////////////////////////////////////////////////////////
			iChance = 25;

			// set base chance and maximum seeking distance according to orders
			switch (pSoldier->aiData.bOrders)
			{
				case STATIONARY:	 iChance += -20; break;
				case ONGUARD:		iChance += -15; break;
				case ONCALL:						 break;
				case CLOSEPATROL:	iChance += +10; break;
				case RNDPTPATROL:
				case POINTPATROL:	iChance	= -10; break;
				case FARPATROL:		iChance += +20; break;
				case SEEKENEMY:		iChance += -10; break;
			}

			// modify for attitude
			switch (pSoldier->aiData.bAttitude)
			{
				case DEFENSIVE:						break;
				case BRAVESOLO:		iChance /= 2;	break;	// loners
				case BRAVEAID:		iChance += 10;	break;	// friendly
				case CUNNINGSOLO:	iChance /= 2;	break;	// loners
				case CUNNINGAID:	 iChance += 10;	break;	// friendly
				case AGGRESSIVE:						break;
			}

			// reduce chance for any injury, less likely to wander around when hurt
			iChance -= (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);

			// reduce chance if breath is down
			iChance -= (100 - pSoldier->bBreath);		 // very likely to wait when exhausted

			if ((INT16) PreRandom(100) < iChance)
			{
				if (RandomFriendWithin(pSoldier))
				{
		#ifdef DEBUGDECISIONS
				sprintf(tempstr,"%s - SEEK FRIEND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
				 AIPopMessage(tempstr);
		#endif

					if (!gfTurnBasedAI)
					{
						// pause at the end of the walk!
						pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
						pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
					}

					return(AI_ACTION_SEEK_FRIEND);
				}
			}
		}
		*/

		////////////////////////////////////////////////////////////////////////////
		// LOOK AROUND: determine %chance for man to turn in place
		////////////////////////////////////////////////////////////////////////////

		// avoid 2 consecutive random turns in a row
		if (pSoldier->aiData.bLastAction != AI_ACTION_CHANGE_FACING && (GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints))
		{
			iChance = 25;

			// set base chance according to orders
			if (pSoldier->aiData.bOrders == STATIONARY)
				iChance += 25;

			if (pSoldier->aiData.bOrders == ONGUARD)
				iChance += 20;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 25;

			if ((INT16)PreRandom(100) < iChance)
			{
				// roll random directions (stored in actionData) until different from current
				do
				{
					// if man has a LEGAL dominant facing, and isn't facing it, he will turn
					// back towards that facing 50% of the time here (normally just enemies)
					if ((pSoldier->aiData.bDominantDir >= 0) && (pSoldier->aiData.bDominantDir <= 8) &&
						(pSoldier->ubDirection != pSoldier->aiData.bDominantDir) && PreRandom(2))
					{
						pSoldier->aiData.usActionData = pSoldier->aiData.bDominantDir;
					}
					else
					{
						pSoldier->aiData.usActionData = (UINT16)PreRandom(8);
					}
				} while (pSoldier->aiData.usActionData == pSoldier->ubDirection);

	#ifdef DEBUGDECISIONS
				STR16 tempstr;
				sprintf(tempstr,"%s - TURNS to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				AIPopMessage(tempstr);
	#endif

				if ( ValidCreatureTurn( pSoldier, (INT8) pSoldier->aiData.usActionData ) )

				//pSoldier->InternalIsValidStance( (INT8) pSoldier->usActionData, ANIM_STAND ) )
				{
					if (!gfTurnBasedAI)
					{
						// pause at the end of the turn!
						pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
						pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
					}

					return(AI_ACTION_CHANGE_FACING);
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// NONE:
	////////////////////////////////////////////////////////////////////////////

	// by default, if everything else fails, just stands in place without turning
	pSoldier->aiData.usActionData = NOWHERE;

	return(AI_ACTION_NONE);
}

INT8 CreatureDecideActionYellow( SOLDIERTYPE * pSoldier )
{
	// monster AI - heard something
	UINT8 ubNoiseDir;
	INT16 sNoiseGridNo;
	INT32 iNoiseValue;
	INT32 iChance, iSneaky;
	BOOLEAN fClimb;
	BOOLEAN fReachable;
//	INT16 sClosestFriend;

	if ( pSoldier->aiData.bMobility == CREATURE_CRAWLER && pSoldier->bActionPoints < pSoldier->bInitialActionPoints)
	{
		return( AI_ACTION_NONE );
	}

	// determine the most important noise heard, and its relative value
	sNoiseGridNo = MostImportantNoiseHeard(pSoldier,&iNoiseValue,&fClimb, &fReachable);
	//NumMessage("iNoiseValue = ",iNoiseValue);

	if (sNoiseGridNo == NOWHERE)
	{
		// then we have no business being under YELLOW status any more!
#ifdef RECORDNET
		fprintf(NetDebugFile,"\nDecideActionYellow: ERROR - No important noise known by guynum %d\n\n",pSoldier->ubID);
#endif

#ifdef BETAVERSION
		NumMessage("DecideActionYellow: ERROR - No important noise known by guynum ",pSoldier->ubID);
#endif

		return(AI_ACTION_NONE);
	}

	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD NOISE: determine %chance for man to turn towards noise
	////////////////////////////////////////////////////////////////////////////

	if (pSoldier->aiData.bMobility != CREATURE_IMMOBILE)
	{
		// determine direction from this soldier in which the noise lies
		ubNoiseDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sNoiseGridNo),CenterY(sNoiseGridNo));

		// if soldier is not already facing in that direction,
		// and the noise source is close enough that it could possibly be seen
		if ((GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints) && (pSoldier->ubDirection != ubNoiseDir) && PythSpacesAway(pSoldier->sGridNo,sNoiseGridNo) <= STRAIGHT)
		{
			// set base chance according to orders
			if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD))
				iChance = 60;
			else			// all other orders
				iChance = 35;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 15;

			if ((INT16)PreRandom(100) < iChance)
			{
				pSoldier->aiData.usActionData = ubNoiseDir;
	#ifdef DEBUGDECISIONS
				STR16 tempstr;
				sprintf(tempstr,"%s - TURNS TOWARDS NOISE to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				AIPopMessage(tempstr);
	#endif
				//if ( pSoldier->InternalIsValidStance( (INT8) pSoldier->usActionData, ANIM_STAND ) )
				if ( ValidCreatureTurn( pSoldier, (INT8) pSoldier->aiData.usActionData ) )
				{
					return(AI_ACTION_CHANGE_FACING);
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	// if our breath is running a bit low, and we're not in water
	if ((pSoldier->bBreath < 25) /*&& !pSoldier->MercInWater() */ )
	{
		// take a breather for gods sake!
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	if (pSoldier->aiData.bMobility != CREATURE_IMMOBILE && fReachable)
	{
		////////////////////////////////////////////////////////////////////////////
		// SEEK NOISE
		////////////////////////////////////////////////////////////////////////////

		// remember that noise value is negative, and closer to 0 => more important!
		iChance = 75 + iNoiseValue;
		iSneaky = 30;

		// set base chance according to orders
		switch (pSoldier->aiData.bOrders)
		{
			case STATIONARY:	 iChance += -20;	break;
			case ONGUARD:		iChance += -15;	break;
			case ONCALL:							break;
			case CLOSEPATROL:	iChance += -10;	break;
			case RNDPTPATROL:
			case POINTPATROL:					 break;
			case FARPATROL:		iChance +=	10;	break;
			case SEEKENEMY:		iChance +=	25;	break;
		}

		// modify chance of patrol (and whether it's a sneaky one) by attitude
		switch (pSoldier->aiData.bAttitude)
		{
			case DEFENSIVE:		iChance += -10;	iSneaky +=	15;	break;
			case BRAVESOLO:		iChance +=	10;					break;
			case BRAVEAID:		iChance +=	5;					break;
			case CUNNINGSOLO:	iChance +=	5;	iSneaky +=	30;	break;
			case CUNNINGAID:						iSneaky +=	30;	break;
			case AGGRESSIVE:	 iChance +=	20;	iSneaky += -10;	break;
		}

		// reduce chance if breath is down, less likely to wander around when tired
		iChance -= (100 - pSoldier->bBreath);

		if ((INT16) PreRandom(100) < iChance)
		{
			pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sNoiseGridNo, AI_ACTION_SEEK_NOISE);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
	#ifdef DEBUGDECISIONS
				STR16 tempstr;
				sprintf(tempstr,"%s - INVESTIGATING NOISE at grid %d, moving to %d",
					pSoldier->name,sNoiseGridNo,pSoldier->aiData.usActionData);
				AIPopMessage(tempstr);
	#endif

				return(AI_ACTION_SEEK_NOISE);
			}
		}
		// Okay, we're not following up on the noise... but let's follow any
		// scent trails available
		if (TrackScent( pSoldier ))
		{
			return( AI_ACTION_TRACK );
		}
	}



	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////

#ifdef DEBUGDECISIONS
	AINameMessage(pSoldier,"- DOES NOTHING (YELLOW)",1000);
#endif

	// by default, if everything else fails, just stands in place without turning
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

INT8 CreatureDecideActionRed(SOLDIERTYPE *pSoldier, UINT8 ubUnconsciousOK)
{
 // monster AI - hostile mammals somewhere around!
 INT16 iChance, sClosestOpponent /*,sClosestOpponent,sClosestFriend*/;
 INT16 sClosestDisturbance;
 INT16 sDistVisible;
 UINT8 ubCanMove,ubOpponentDir;
 //INT8 bInWater;
 INT8 bInGas;
 INT8 bSeekPts = 0, bHelpPts = 0, bHidePts = 0;
 INT16 sAdjustedGridNo;
 BOOLEAN fChangeLevel;

 // if we have absolutely no action points, we can't do a thing under RED!
 if (!pSoldier->bActionPoints)
	{
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
	}

	if ( pSoldier->aiData.bMobility == CREATURE_CRAWLER && pSoldier->bActionPoints < pSoldier->bInitialActionPoints)
	{
		return( AI_ACTION_NONE );
	}


 // can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
 ubCanMove = ((pSoldier->aiData.bMobility != CREATURE_IMMOBILE) && (pSoldier->bActionPoints >= MinPtsToMove(pSoldier)));

 // determine if we happen to be in water (in which case we're in BIG trouble!)
 //bInWater = pSoldier->MercInWater();

 // check if standing in tear gas without a gas mask on
 bInGas = InGas( pSoldier, pSoldier->sGridNo );


 ////////////////////////////////////////////////////////////////////////////
 // WHEN IN GAS, GO TO NEAREST REACHABLE SPOT OF UNGASSED LAND
 ////////////////////////////////////////////////////////////////////////////

 if (bInGas && ubCanMove)
	{
	pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

	if (pSoldier->aiData.usActionData != NOWHERE)
	{
#ifdef DEBUGDECISIONS
	STR16 tempstr;
		sprintf(tempstr,"%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
     AIPopMessage(tempstr);
#endif

	 return(AI_ACTION_LEAVE_WATER_GAS);
	}
	}

	////////////////////////////////////////////////////////////////////////////
	// CALL FOR AID IF HURT
	////////////////////////////////////////////////////////////////////////////
	if ( CAN_CALL( pSoldier ) )
	{
		if ((pSoldier->bActionPoints >= AP_RADIO) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1))
		{
			if (pSoldier->stats.bLife < pSoldier->bOldLife)
			{
				// got injured, maybe call
				if ((pSoldier->bOldLife == pSoldier->stats.bLifeMax) && (pSoldier->bOldLife - pSoldier->stats.bLife > 10))
				{
					// hurt for first time!
					pSoldier->aiData.usActionData = CALL_CRIPPLED;
					pSoldier->bOldLife = pSoldier->stats.bLife;	// don't want to call more than once
					return(AI_ACTION_CREATURE_CALL);
				}
				else if (pSoldier->stats.bLifeMax / pSoldier->stats.bLife > 2)
				{
					// crippled, 1/3 or less health!
					pSoldier->aiData.usActionData = CALL_ATTACKED;
					pSoldier->bOldLife = pSoldier->stats.bLife;	// don't want to call more than once
					return(AI_ACTION_CREATURE_CALL);
				}
			}
		}
	}


 ////////////////////////////////////////////////////////////////////////
 // CROUCH & REST IF RUNNING OUT OF BREATH
 ////////////////////////////////////////////////////////////////////////

 // if our breath is running a bit low, and we're not in water or under fire
 if ((pSoldier->bBreath < 25) /*&& !bInWater*/ && !pSoldier->aiData.bUnderFire)
	{
#ifdef DEBUGDECISIONS
	STR16 tempstr;
	  sprintf(tempstr,"%s RESTS (STATUS RED), breath = %d",pSoldier->name,pSoldier->bBreath);
   AIPopMessage(tempstr);
#endif

	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
	}

	////////////////////////////////////////////////////////////////////////////
	// CALL IN SIGHTING: determine %chance to call others and report contact
	////////////////////////////////////////////////////////////////////////////

	// if we're a computer merc, and we have the action points remaining to RADIO
	// (we never want NPCs to choose to radio if they would have to wait a turn)
	if ( CAN_CALL( pSoldier ) && (!gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition) )
	{
		if ((pSoldier->bActionPoints >= AP_RADIO) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1))
		{
			// if there hasn't been a general sighting call sent yet

			// might want to check the specifics of who we see
			iChance = 20;

			if (iChance)
			{
				#ifdef DEBUGDECISIONS
					AINumMessage("Chance to call sighting = ",iChance);
				#endif

				if ((INT16) PreRandom(100) < iChance)
				{
					#ifdef DEBUGDECISIONS
						AINameMessage(pSoldier,"decides to call an alert!",1000);
					#endif
					pSoldier->aiData.usActionData = CALL_1_PREY;
					return(AI_ACTION_CREATURE_CALL);
				}
			}
		}
	}

	if ( pSoldier->aiData.bMobility != CREATURE_IMMOBILE )
	{
		if ( FindAIUsableObjClass( pSoldier, IC_WEAPON ) == ITEM_NOT_FOUND )
		{
			// probably a baby bug... run away! run away!
		// look for best place to RUN AWAY to (farthest from the closest threat)
		pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents( pSoldier );

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
			return(AI_ACTION_RUN_AWAY);
		}
		else
		{
			return( AI_ACTION_NONE );
		}

		}

		// Respond to call if any
		if ( CAN_LISTEN_TO_CALL( pSoldier ) && pSoldier->aiData.ubCaller != NOBODY )
		{
			if ( PythSpacesAway( pSoldier->sGridNo, pSoldier->aiData.sCallerGridNo ) <= STOPSHORTDIST )
			{
				// call completed... hmm, nothing found
				pSoldier->aiData.ubCaller = NOBODY;
			}
			else
			{
				pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, pSoldier->aiData.sCallerGridNo, -1, AI_ACTION_SEEK_FRIEND, FLAG_STOPSHORT);

				if (pSoldier->aiData.usActionData != NOWHERE)
				{
					#ifdef DEBUGDECISIONS
					STR16 tempstr;
					sprintf(tempstr,"%s - SEEKING FRIEND at %d, MOVING to %d",
						pSoldier->name,sClosestFriend,pSoldier->aiData.usActionData);
						AIPopMessage(tempstr);
					#endif
					return(AI_ACTION_SEEK_FRIEND);
				}
			}
		}

		// get the location of the closest reachable opponent
		sClosestDisturbance = ClosestReachableDisturbance(pSoldier,ubUnconsciousOK, &fChangeLevel);
		// if there is an opponent reachable
		if (sClosestDisturbance != NOWHERE)
		{
			//////////////////////////////////////////////////////////////////////
			// SEEK CLOSEST DISTURBANCE: GO DIRECTLY TOWARDS CLOSEST KNOWN OPPONENT
			//////////////////////////////////////////////////////////////////////

			// try to move towards him
			pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sClosestDisturbance,AI_ACTION_SEEK_OPPONENT);

			// if it's possible
			if (pSoldier->aiData.usActionData != NOWHERE)
			{
				#ifdef DEBUGDECISIONS
					// do it!
					STR16 tempstr;
					sprintf(tempstr,"%s - SEEKING OPPONENT at grid %d, MOVING to %d",
					pSoldier->name,sClosestDisturbance,pSoldier->aiData.usActionData);
					AIPopMessage(tempstr);
				#endif

				return(AI_ACTION_SEEK_OPPONENT);
			}
		}

	////////////////////////////////////////////////////////////////////////////
	// TAKE A BITE, PERHAPS
	////////////////////////////////////////////////////////////////////////////
	if (pSoldier->aiData.bHunting)
	{
	pSoldier->aiData.usActionData = FindNearestRottingCorpse( pSoldier );
		// need smell/visibility check?
		if (PythSpacesAway( pSoldier->sGridNo, pSoldier->aiData.usActionData) < MAX_EAT_DIST )
		{
			INT16 sGridNo;

			sGridNo = FindAdjacentGridEx( pSoldier, pSoldier->aiData.usActionData, &ubOpponentDir, &sAdjustedGridNo, FALSE, FALSE );

			if ( sGridNo != -1 )
			{
					pSoldier->aiData.usActionData = sGridNo;
					return( AI_ACTION_APPROACH_MERC );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// TRACK A SCENT, IF ONE IS PRESENT
	////////////////////////////////////////////////////////////////////////////
	if (TrackScent( pSoldier ))
		{
			return( AI_ACTION_TRACK );
		}


	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD CLOSEST KNOWN OPPONENT, IF KNOWN
	////////////////////////////////////////////////////////////////////////////
	if (GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		// determine the location of the known closest opponent
		// (don't care if he's conscious, don't care if he's reachable at all)
		sClosestOpponent = ClosestKnownOpponent(pSoldier, NULL, NULL);

		if (sClosestOpponent != NOWHERE)
			{
			// determine direction from this soldier to the closest opponent
			ubOpponentDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sClosestOpponent),CenterY(sClosestOpponent));

			 // if soldier is not already facing in that direction,
			 // and the opponent is close enough that he could possibly be seen
			 // note, have to change this to use the level returned from ClosestKnownOpponent
			 sDistVisible = pSoldier->GetMaxDistanceVisible(sClosestOpponent, 0 );

			if ((pSoldier->ubDirection != ubOpponentDir) && (PythSpacesAway(pSoldier->sGridNo,sClosestOpponent) <= sDistVisible))
				{
				// set base chance according to orders
				if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD))
					iChance = 50;
				else			// all other orders
					iChance = 25;

				if (pSoldier->aiData.bAttitude == DEFENSIVE)
					iChance += 25;

				 //if ( (INT16)PreRandom(100) < iChance && pSoldier->InternalIsValidStance( ubOpponentDir, ANIM_STAND ) )
				if ( (INT16)PreRandom(100) < iChance && ValidCreatureTurn( pSoldier, ubOpponentDir ) )
				{
					pSoldier->aiData.usActionData = ubOpponentDir;

		#ifdef DEBUGDECISIONS
					STR16 tempstr;
					 sprintf(tempstr,"%s - TURNS TOWARDS CLOSEST ENEMY to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
					 AIPopMessage(tempstr);
		#endif

					return(AI_ACTION_CHANGE_FACING);
					}
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// LEAVE THE SECTOR
	////////////////////////////////////////////////////////////////////////////

	// NOT IMPLEMENTED

 ////////////////////////////////////////////////////////////////////////////
 // DO NOTHING: Not enough points left to move, so save them for next turn
 ////////////////////////////////////////////////////////////////////////////

#ifdef DEBUGDECISIONS
 AINameMessage(pSoldier,"- DOES NOTHING (RED)",1000);
#endif

 pSoldier->aiData.usActionData = NOWHERE;

 return(AI_ACTION_NONE);
}


INT8 CreatureDecideActionBlack( SOLDIERTYPE * pSoldier )
{
	// monster AI - hostile mammals in sense range
 INT16		sClosestOpponent,sBestCover = NOWHERE;
 INT16		sClosestDisturbance;
 UINT8		ubMinAPCost,ubCanMove/*,bInWater*/,bInGas;
 INT8			bDirection;
 UINT8		ubBestAttackAction;
 INT8			bCanAttack;
 INT8			bSpitIn, bWeaponIn;
 UINT32		uiChance;
 ATTACKTYPE BestShot = {}, BestStab = {}, BestAttack = {}, CurrStab = {};
 BOOLEAN	fRunAway = FALSE;
 BOOLEAN	fChangeLevel;

 // if we have absolutely no action points, we can't do a thing under BLACK!
 if (!pSoldier->bActionPoints)
	{
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
	}

	if ( pSoldier->aiData.bMobility == CREATURE_CRAWLER && pSoldier->bActionPoints < pSoldier->bInitialActionPoints)
	{
		return( AI_ACTION_NONE );
	}

	////////////////////////////////////////////////////////////////////////////
	// CALL FOR AID IF HURT OR IF OTHERS ARE UNAWARE
	////////////////////////////////////////////////////////////////////////////

	if ( CAN_CALL( pSoldier ) )
	{
		if ((pSoldier->bActionPoints >= AP_RADIO) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1))
		{
			if (pSoldier->stats.bLife < pSoldier->bOldLife)
			{
				// got injured, maybe call
				/*
				// don't call when crippled and have target... save breath for attacking!
				if ((pSoldier->bOldLife == pSoldier->stats.bLifeMax) && (pSoldier->bOldLife - pSoldier->stats.bLife > 10))
				{
					// hurt for first time!
					pSoldier->aiData.usActionData = CALL_CRIPPLED;
					pSoldier->bOldLife = pSoldier->stats.bLife;	// don't want to call more than once
					return(AI_ACTION_CREATURE_CALL);
				}
				else
				*/
				if (pSoldier->stats.bLifeMax / pSoldier->stats.bLife > 2)
				{
					// crippled, 1/3 or less health!
					pSoldier->aiData.usActionData = CALL_ATTACKED;
					pSoldier->bOldLife = pSoldier->stats.bLife;	// don't want to call more than once
					return(AI_ACTION_CREATURE_CALL);
				}
			}
			else
			{
				if (!(gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition))
				{
					if (pSoldier->ubBodyType == QUEENMONSTER)
					{
						uiChance = 100;
					}
					else
					{
						uiChance = 20 * pSoldier->aiData.bOppCnt;
					}
					if ( Random( 100 ) < uiChance )
					{
						// alert! alert!
						if (pSoldier->aiData.bOppCnt > 1)
						{
							pSoldier->aiData.usActionData = CALL_MULTIPLE_PREY;
						}
						else
						{
							pSoldier->aiData.usActionData = CALL_1_PREY;
						}
						return(AI_ACTION_CREATURE_CALL);
					}
				}
			}
		}
	}

 // can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
 ubCanMove = ((pSoldier->aiData.bMobility != CREATURE_IMMOBILE) && (pSoldier->bActionPoints >= MinPtsToMove(pSoldier)));

 // determine if we happen to be in water (in which case we're in BIG trouble!)
 //bInWater = pSoldier->MercInWater();

 // check if standing in tear gas without a gas mask on
	bInGas = InGas( pSoldier, pSoldier->sGridNo );


 ////////////////////////////////////////////////////////////////////////////
 // IF GASSED, OR REALLY TIRED (ON THE VERGE OF COLLAPSING), TRY TO RUN AWAY
 ////////////////////////////////////////////////////////////////////////////

 // if we're desperately short on breath (it's OK if we're in water, though!)
 if (bInGas || (pSoldier->bBreath < 5))
	{
	// if soldier has enough APs left to move at least 1 square's worth
	if (ubCanMove)
	{
	 // look for best place to RUN AWAY to (farthest from the closest threat)
	 pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

	 if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
		STR16 tempstr;
		  sprintf(tempstr,"%s - GASSED or LOW ON BREATH (%d), RUNNING AWAY to grid %d",pSoldier->name,pSoldier->bBreath,pSoldier->aiData.usActionData);
       AIPopMessage(tempstr);
#endif

		return(AI_ACTION_RUN_AWAY);
		}
	}

	}


 ////////////////////////////////////////////////////////////////////////////
 // STUCK IN WATER OR GAS, NO COVER, GO TO NEAREST SPOT OF UNGASSED LAND
 ////////////////////////////////////////////////////////////////////////////

 // if soldier in water/gas has enough APs left to move at least 1 square
 if ((/*bInWater ||*/ bInGas) && ubCanMove)
	{
	pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

	if (pSoldier->aiData.usActionData != NOWHERE)
	{
#ifdef DEBUGDECISIONS
		STR16 tempstr;
		sprintf(tempstr,"%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
     AIPopMessage(tempstr);
#endif

	 return(AI_ACTION_LEAVE_WATER_GAS);
	}
	}

 ////////////////////////////////////////////////////////////////////////////
 // SOLDIER CAN ATTACK IF NOT IN WATER/GAS AND NOT DOING SOMETHING TOO FUNKY
 ////////////////////////////////////////////////////////////////////////////

	// NPCs in water/tear gas without masks are not permitted to shoot/stab/throw
	if ((pSoldier->bActionPoints < 2) /*|| bInWater*/ || bInGas)
	{
		bCanAttack = FALSE;
	}
	else
	{
		bCanAttack = CanNPCAttack(pSoldier);
		if (bCanAttack != TRUE)
		{
			if ( bCanAttack == NOSHOOT_NOAMMO )
			{
				pSoldier->inv[HANDPOS].fFlags |= OBJECT_AI_UNUSABLE;

				// try to find a bladed weapon
				if (pSoldier->ubBodyType == QUEENMONSTER)
				{
					bWeaponIn = FindObjClass( pSoldier, IC_TENTACLES );
				}
				else
				{
					bWeaponIn = FindObjClass( pSoldier, IC_BLADE );
				}

				if ( bWeaponIn != NO_SLOT )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping item into hand");
					RearrangePocket(pSoldier,HANDPOS,bWeaponIn,FOREVER);
					bCanAttack = TRUE;
				}
				else
				{
					// infants who exhaust their spit should flee!
					fRunAway = TRUE;
					bCanAttack = FALSE;
				}

			}
			else
			{
				bCanAttack = FALSE;
			}

		}
	}


 BestShot.ubPossible	= FALSE;	// by default, assume Shooting isn't possible
 BestStab.ubPossible	= FALSE;	// by default, assume Stabbing isn't possible

 BestAttack.ubChanceToReallyHit = 0;

 bSpitIn = NO_SLOT;


 // if we are able attack
 if (bCanAttack)
 {
	//////////////////////////////////////////////////////////////////////////
	// FIRE A GUN AT AN OPPONENT
	//////////////////////////////////////////////////////////////////////////

	pSoldier->bAimShotLocation = AIM_SHOT_RANDOM;

	bWeaponIn = FindObjClass( pSoldier, IC_GUN );

	if (bWeaponIn != NO_SLOT)
	{
		if (Item[pSoldier->inv[bWeaponIn].usItem].usItemClass == IC_GUN && pSoldier->inv[bWeaponIn][0]->data.gun.bGunStatus >= USABLE)
		{
			if (pSoldier->inv[bWeaponIn][0]->data.gun.ubGunShotsLeft > 0)
			{
				bSpitIn = bWeaponIn;
				// if it's in another pocket, swap it into his hand temporarily
				if (bWeaponIn != HANDPOS)
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping gun into hand");
					RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
				}

				// now it better be a gun, or the guy can't shoot (but has other attack(s))

				// get the minimum cost to attack the same target with this gun
				ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,DONTADDTURNCOST);

				// if we have enough action points to shoot with this gun
				if (pSoldier->bActionPoints >= ubMinAPCost)
				{
					// look around for a worthy target (which sets BestShot.ubPossible)
					CalcBestShot(pSoldier,&BestShot,FALSE);

					if (BestShot.ubPossible)
					{
						BestShot.bWeaponIn = bWeaponIn;

						// if the selected opponent is not a threat (unconscious & !serviced)
						// (usually, this means all the guys we see our unconscious, but, on
						//	rare occasions, we may not be able to shoot a healthy guy, too)
						if ((Menptr[BestShot.ubOpponent].stats.bLife < OKLIFE) &&
							!Menptr[BestShot.ubOpponent].bService)
						{
							// if our attitude is NOT aggressive
							if (pSoldier->aiData.bAttitude != AGGRESSIVE)
							{
								// get the location of the closest CONSCIOUS reachable opponent
								sClosestDisturbance = ClosestReachableDisturbance(pSoldier,FALSE,&fChangeLevel);

								// if we found one
								if (sClosestDisturbance != NOWHERE)
								{
									// don't bother checking GRENADES/KNIVES, he can't have conscious targets
									#ifdef RECORDNET
										fprintf(NetDebugFile,"\tDecideActionBlack: all visible opponents unconscious, switching to RED AI...\n");
									#endif
									// then make decision as if at alert status RED, but make sure
									// we don't try to SEEK OPPONENT the unconscious guy!
									return(DecideActionRed(pSoldier,FALSE));
								}
								// else kill the guy, he could be the last opponent alive in this sector
							}
							// else aggressive guys will ALWAYS finish off unconscious opponents
						}

						// now we KNOW FOR SURE that we will do something (shoot, at least)
						NPCDoesAct(pSoldier);

					}
				}
				// if it was in his holster, swap it back into his holster for now
				if (bWeaponIn != HANDPOS)
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping gun into holster");
					RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
				}
			}
			else
			{
				// out of ammo! reload if possible!
			}

		}

	}

	//////////////////////////////////////////////////////////////////////////
	// GO STAB AN OPPONENT WITH A KNIFE
	//////////////////////////////////////////////////////////////////////////

	// if soldier has a knife in his hand
	if (pSoldier->ubBodyType == QUEENMONSTER)
	{
		bWeaponIn = FindObjClass( pSoldier, IC_TENTACLES );
	}
	else if ( pSoldier->ubBodyType == BLOODCAT )
	{
		// 1 in 3 attack with teeth, otherwise with claws
		if ( PreRandom( 3 ) )
		{
			bWeaponIn = FindObj( pSoldier, BLOODCAT_CLAW_ATTACK );
		}
		else
		{
			bWeaponIn = FindObj( pSoldier, BLOODCAT_BITE );
		}
	}
	else
	{
		if (bSpitIn != NO_SLOT && Random( 4 ) )
		{
			// spitters only consider a blade attack 1 time in 4
			bWeaponIn = NO_SLOT;
		}
		else
		{
			bWeaponIn = FindObjClass( pSoldier, IC_BLADE );
		}
	}



	BestStab.iAttackValue = 0;

	// if the soldier does have a usable knife somewhere

	// spitters don't always consider using their claws
	if ( bWeaponIn != NO_SLOT )
	{
		// if it's in his holster, swap it into his hand temporarily
		if (bWeaponIn != HANDPOS)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping knife into hand");
			RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
		}

		// get the minimum cost to attack with this knife
		ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,DONTADDTURNCOST);

		//sprintf(tempstr,"%s - ubMinAPCost = %d",pSoldier->name,ubMinAPCost);
		//PopMessage(tempstr);

		// if we can afford the minimum AP cost to stab with this knife weapon
		if (pSoldier->bActionPoints >= ubMinAPCost)
		{
			// then look around for a worthy target (which sets BestStab.ubPossible)

			if (pSoldier->ubBodyType == QUEENMONSTER)
			{
				CalcTentacleAttack( pSoldier, &CurrStab );
			}
			else
			{
				CalcBestStab(pSoldier, &CurrStab, TRUE);
			}

			if (CurrStab.ubPossible)
			{
				// now we KNOW FOR SURE that we will do something (stab, at least)
				NPCDoesAct(pSoldier);
		}

			// if it was in his holster, swap it back into his holster for now
			if (bWeaponIn != HANDPOS)
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping knife into holster");
				RearrangePocket(pSoldier,HANDPOS,bWeaponIn,TEMPORARILY);
			}

			if (CurrStab.iAttackValue > BestStab.iAttackValue)
			{
				CurrStab.bWeaponIn = bWeaponIn;
				memcpy(&BestStab,&CurrStab,sizeof(BestStab));
			}

		}

	}

	//////////////////////////////////////////////////////////////////////////
	// CHOOSE THE BEST TYPE OF ATTACK OUT OF THOSE FOUND TO BE POSSIBLE
	//////////////////////////////////////////////////////////////////////////
	if (BestShot.ubPossible)
	{
		BestAttack.iAttackValue = BestShot.iAttackValue;
		ubBestAttackAction = AI_ACTION_FIRE_GUN;
	}
	else
	{
		BestAttack.iAttackValue = 0;
		ubBestAttackAction = AI_ACTION_NONE;
	}
	if (BestStab.ubPossible && BestStab.iAttackValue > (BestAttack.iAttackValue * 12) / 10 )
	{
		BestAttack.iAttackValue = BestStab.iAttackValue;
		ubBestAttackAction = AI_ACTION_KNIFE_MOVE;
	}

	// if attack is still desirable (meaning it's also preferred to taking cover)
	if (ubBestAttackAction != AI_ACTION_NONE)
	{
		// copy the information on the best action selected into BestAttack struct
		switch (ubBestAttackAction)
		{
			case AI_ACTION_FIRE_GUN:
				memcpy(&BestAttack,&BestShot,sizeof(BestAttack));
				break;

		case AI_ACTION_KNIFE_MOVE:
			memcpy(&BestAttack,&BestStab,sizeof(BestAttack));
			break;

		}

		// if necessary, swap the weapon into the hand position
		 if (BestAttack.bWeaponIn != HANDPOS)
		 {
		// IS THIS NOT BEING SET RIGHT?????
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"creaturedecideactionblack: swapping best weapon into hand");
				RearrangePocket(pSoldier,HANDPOS,BestAttack.bWeaponIn,FOREVER);
		 }

		//////////////////////////////////////////////////////////////////////////
		// GO AHEAD & ATTACK!
		//////////////////////////////////////////////////////////////////////////

		pSoldier->aiData.usActionData = BestAttack.sTarget;
		pSoldier->aiData.bAimTime			= BestAttack.ubAimTime;

		if ( ubBestAttackAction == AI_ACTION_FIRE_GUN && BestAttack.ubChanceToReallyHit > 50 )
		{
			pSoldier->bAimShotLocation = AIM_SHOT_HEAD;
		}
		else
		{
			pSoldier->bAimShotLocation = AIM_SHOT_RANDOM;
		}

#ifdef DEBUGDECISIONS
		STR tempstr;
		sprintf( tempstr, "%d(%s) %s %d(%s) at gridno %d (%d APs aim)\n",
			pSoldier->ubID,pSoldier->name,
			(ubBestAttackAction == AI_ACTION_FIRE_GUN)?"SHOOTS":((ubBestAttackAction == AI_ACTION_TOSS_PROJECTILE)?"TOSSES AT":"STABS"),
			BestAttack.ubOpponent,ExtMen[BestAttack.ubOpponent].name,
			BestAttack.sTarget,BestAttack.ubAimTime) ;
		DebugAI( tempstr );
#endif

		return(ubBestAttackAction);

	}
 }



 ////////////////////////////////////////////////////////////////////////////
 // CLOSE ON THE CLOSEST KNOWN OPPONENT or TURN TO FACE HIM
 ////////////////////////////////////////////////////////////////////////////

 if ( !fRunAway )
 {
	if ( (GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints) )
	{
		// determine the location of the known closest opponent
		// (don't care if he's conscious, don't care if he's reachable at all)
		sClosestOpponent = ClosestKnownOpponent(pSoldier, NULL, NULL);
		// if we have a closest reachable opponent
		if (sClosestOpponent != NOWHERE)
		{
				if ( ubCanMove && PythSpacesAway( pSoldier->sGridNo, sClosestOpponent ) > 2 )
				{
					if ( bSpitIn != NO_SLOT )
					{
						pSoldier->aiData.usActionData = AdvanceToFiringRange( pSoldier, sClosestOpponent );
						if (pSoldier->aiData.usActionData == NOWHERE)
						{
							pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sClosestOpponent,AI_ACTION_SEEK_OPPONENT);
						}
					}
					else
					{
						pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sClosestOpponent,AI_ACTION_SEEK_OPPONENT);
					}
				}
				else
				{
					pSoldier->aiData.usActionData = NOWHERE;
				}

				if (pSoldier->aiData.usActionData != NOWHERE) // charge!
				{
					return( AI_ACTION_SEEK_OPPONENT );
				}
				else if (GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints) // turn to face enemy
				{
				bDirection = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sClosestOpponent),CenterY(sClosestOpponent));

				// if we're not facing towards him
				if (pSoldier->ubDirection != bDirection && ValidCreatureTurn( pSoldier, bDirection ) )
				{
					pSoldier->aiData.usActionData = bDirection;

					#ifdef DEBUGDECISIONS
					STR16 tempstr;
					 sprintf(tempstr,"%s - TURNS to face CLOSEST OPPONENT in direction %d",pSoldier->name,pSoldier->aiData.usActionData);
					 AIPopMessage(tempstr);
					#endif

					return(AI_ACTION_CHANGE_FACING);
				}
				}
		}
	}
 }
 else
 {
	// run away!
	if ( ubCanMove )
	{
		// look for best place to RUN AWAY to (farthest from the closest threat)
		//pSoldier->aiData.usActionData = RunAway( pSoldier );
		pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents( pSoldier );

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
			return(AI_ACTION_RUN_AWAY);
		}
	}

 }
 ////////////////////////////////////////////////////////////////////////////
 // DO NOTHING: Not enough points left to move, so save them for next turn
 ////////////////////////////////////////////////////////////////////////////

#ifdef DEBUGDECISIONS
 AINameMessage(pSoldier,"- DOES NOTHING (BLACK)",1000);
#endif

 // by default, if everything else fails, just stand in place and wait
 pSoldier->aiData.usActionData = NOWHERE;
 return(AI_ACTION_NONE);

}




INT8 CreatureDecideAction( SOLDIERTYPE *pSoldier )
{
	INT8 bAction = AI_ACTION_NONE;

	switch (pSoldier->aiData.bAlertStatus)
	{
		case STATUS_GREEN:
			#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = GREEN");
			#endif
			bAction = CreatureDecideActionGreen(pSoldier);
			break;

		case STATUS_YELLOW:
			#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = YELLOW");
			#endif
			bAction = CreatureDecideActionYellow(pSoldier);
		break;

		case STATUS_RED:
			#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = RED");
			#endif
			bAction = CreatureDecideActionRed(pSoldier, TRUE);
			break;

		case STATUS_BLACK:
			#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = BLACK");
			#endif
			bAction = CreatureDecideActionBlack(pSoldier);
			break;
	}

#ifdef DEBUGDECISIONS
	STR tempstr;
	sprintf( tempstr, "DecideAction: selected action %d, actionData %d\n\n",bAction,pSoldier->aiData.usActionData );
	DebugAI(tempstr);
#endif

	return(bAction);
}

void CreatureDecideAlertStatus( SOLDIERTYPE *pSoldier )
{
	INT8	bOldStatus;
	INT32	iDummy;
	BOOLEAN	fClimbDummy, fReachableDummy;

	// THE FOUR (4) POSSIBLE ALERT STATUSES ARE:
	// GREEN - No one sensed, no suspicious noise heard, go about doing regular stuff
	// YELLOW - Suspicious noise was heard personally
	// RED - Either saw OPPONENTS in person, or definite contact had been called
	// BLACK - Currently has one or more OPPONENTS in sight

	// set mobility
	switch (pSoldier->ubBodyType)
	{
		case ADULTFEMALEMONSTER:
		case YAF_MONSTER:
		case AM_MONSTER:
		case YAM_MONSTER:
		case INFANT_MONSTER:
			pSoldier->aiData.bMobility = CREATURE_MOBILE;
			break;
		case QUEENMONSTER:
			pSoldier->aiData.bMobility = CREATURE_IMMOBILE;
			break;
		case LARVAE_MONSTER:
			pSoldier->aiData.bMobility = CREATURE_CRAWLER;
			break;
	}


	if (pSoldier->ubBodyType == LARVAE_MONSTER)
	{
		// larvae never do anything much!
		pSoldier->aiData.bAlertStatus = STATUS_GREEN;
		return;
	}

	// save the man's previous status
	bOldStatus = pSoldier->aiData.bAlertStatus;

	// determine the current alert status for this category of man
	if (pSoldier->aiData.bOppCnt > 0)		// opponent(s) in sight
	{
		// must search through list of people to see if any of them have
		// attacked us, or do some check to see if we have been attacked
		switch (bOldStatus)
		{
			case STATUS_GREEN:
			case STATUS_YELLOW:
				pSoldier->aiData.bAlertStatus = STATUS_BLACK;
				break;
			case STATUS_RED:
			case STATUS_BLACK:
				pSoldier->aiData.bAlertStatus = STATUS_BLACK;
		}

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
					if ((MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) == NOWHERE) && !pSoldier->aiData.bActionInProgress)
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
					if (MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) != NOWHERE)
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

#ifdef DEBUGDECISIONS
		// don't report status changes for human-controlled mercs
//		if (!pSoldier->human)
//		{
			STR16 tempstr;
			sprintf(tempstr,"%s's Alert Status changed from %d to %d",
				pSoldier->name ,bOldStatus,pSoldier->aiData.bAlertStatus);
			AIPopMessage(tempstr);
//		}
#endif

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
					(MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) == NOWHERE))
				{
					// then drop back to GREEN status
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
					CheckForChangingOrders(pSoldier);
				}
			}
	}
	}
}




INT8 CrowDecideActionRed( SOLDIERTYPE * pSoldier )
{
	// OK, Fly away!
	//HandleCrowFlyAway( pSoldier );
	if (!gfTurnBasedAI)
	{
		pSoldier->aiData.usActionData = 30000;
		return( AI_ACTION_WAIT );
	}
	else
	{
		return( AI_ACTION_NONE );
	}
}


INT8 CrowDecideActionGreen( SOLDIERTYPE * pSoldier )
{
	INT16 sCorpseGridNo;
	UINT8 ubDirection;
	INT16 sFacingDir;

	// Look for a corse!
	sCorpseGridNo = FindNearestRottingCorpse( pSoldier );

	if ( sCorpseGridNo != NOWHERE )
	{
		// Are we close, if so , peck!
		if ( SpacesAway( pSoldier->sGridNo, sCorpseGridNo ) < 2 )
		{
			// Change facing
			sFacingDir = GetDirectionFromGridNo( sCorpseGridNo, pSoldier );

				if ( sFacingDir != pSoldier->ubDirection )
				{
					pSoldier->aiData.usActionData = sFacingDir;
				return(AI_ACTION_CHANGE_FACING);
				}
				else if (!gfTurnBasedAI)
				{
					pSoldier->aiData.usActionData = 30000;
					return( AI_ACTION_WAIT );
				}
				else
				{
					return( AI_ACTION_NONE );
				}
		}
		else
		{
			// Walk to nearest one!
			pSoldier->aiData.usActionData = FindGridNoFromSweetSpot( pSoldier, sCorpseGridNo, 4, &ubDirection );
			if ( pSoldier->aiData.usActionData != NOWHERE )
			{
				return( AI_ACTION_GET_CLOSER );
			}
		}
	}

	return( AI_ACTION_NONE );
}

INT8 CrowDecideAction( SOLDIERTYPE * pSoldier )
{
	if ( pSoldier->usAnimState == CROW_FLY )
	{
		return( AI_ACTION_NONE );
	}

	switch( pSoldier->aiData.bAlertStatus )
	{
		case STATUS_GREEN:
		case STATUS_YELLOW:
			return( CrowDecideActionGreen( pSoldier ) );

		case STATUS_RED:
		case STATUS_BLACK:
			return( CrowDecideActionRed( pSoldier ) );

		default:
			Assert( FALSE );
			return( AI_ACTION_NONE );
	}
}
