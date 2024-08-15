	#include "ai.h"
	#include "AIInternals.h"
	#include "Isometric Utils.h"
	#include "overhead.h"
	#include "worldman.h"
	#include "pathai.h"
	//#include "points.h"
	#include "message.h"
	#include "Smell.h"
	#include "mapscreen.h"
	#include "strategic.h"
	#include "Strategic Pathing.h"

	#include "Soldier macros.h"
	#include "Render Fun.h"
	#include "Soldier Functions.h"		// added by Flugente
#include "connect.h"
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


// from strategic
extern INT16 DirXIncrementer[8];
extern INT16 DirYIncrementer[8];
//
// CJC's DG->JA2 conversion notes
//
// LegalNPCDestination - mode hardcoded to walking; C.O. tear gas related stuff commented out
// TryToResumeMovement - C.O. EscortedMoveCanceled call
// GoAsFarAsPossibleTowards - C.O. stuff related to current animation esp first aid
// SetCivilianDestination - C.O. stuff for if we don't control the civ

int LegalNPCDestination(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubPathMode, UINT8 ubWaterOK, UINT8 fFlags)
{
	BOOLEAN fSkipTilesWithMercs;

	if ((sGridNo < 0) || (sGridNo >= GRIDSIZE))
	{
#ifdef RECORDNET
		fprintf(NetDebugFile,"LegalNPC->pathing.sDestination: ERROR - rcvd invalid gridno %d",GridNo);
#endif

#ifdef BETAVERSION
		NumMessage("LegalNPC->pathing.sDestination: ERROR - rcvd invalid gridno ",GridNo);
#endif

		return(FALSE);
	}

	//dnl ch53 121009 Return false if gridno on different level from merc or inside unvisible area
	if ((GridNoOnVisibleWorldTile(pSoldier->sGridNo) && gpWorldLevelData[pSoldier->sGridNo].sHeight != gpWorldLevelData[sGridNo].sHeight) || !GridNoOnVisibleWorldTile(sGridNo))
		return(FALSE);

	// skip mercs if turnbased and adjacent AND not doing an IGNORE_PATH check (which is used almost exclusively by GoAsFarAsPossibleTowards)
	fSkipTilesWithMercs = (gfTurnBasedAI && ubPathMode != IGNORE_PATH && SpacesAway( pSoldier->sGridNo, sGridNo ) == 1 );

	// if this gridno is an OK destination
	// AND the gridno is NOT in a tear-gassed tile when we have no gas mask
	// AND someone is NOT already standing there
	// AND we're NOT already standing at that gridno
	// AND the gridno hasn't been black-listed for us
	
	// Nov 28 98: skip people in destination tile if in turnbased
	// sevenfm: disabled additional checks to prevent AI deadlock
	if (	NewOKDestination(pSoldier, sGridNo, fSkipTilesWithMercs, pSoldier->pathing.bLevel ) &&
			//!InGas( pSoldier, sGridNo ) &&
			//!FindBombNearby(pSoldier, sGridNo, DAY_VISION_RANGE/8 ) &&
			sGridNo != pSoldier->sGridNo &&
			sGridNo != pSoldier->pathing.sBlackList )
	{
		// if water's a problem, and gridno is in a water tile (bridges are OK)
		if (!ubWaterOK && Water(sGridNo, pSoldier->pathing.bLevel))
			return(FALSE);

		//Madd: added to prevent people from running into gas and fire
		/*if ( (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->pathing.bLevel] & (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
			 !DoesSoldierWearGasMask( pSoldier ) )
		{
			return( FALSE );
		}

		if ( gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->pathing.bLevel] & MAPELEMENT_EXT_BURNABLEGAS )
		{
			return( FALSE );
		}*/

		// passed all checks, now try to make sure we can get there!
		switch (ubPathMode)
		{
			// if finding a path wasn't asked for (could have already been done,
			// for example), don't bother
			case IGNORE_PATH	 :	return(TRUE);

			case ENSURE_PATH	 :	if ( FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, COPYROUTE, fFlags ) )
									{
										return(TRUE);		// legal destination
									}
									else // got this far, but found no clear path,
									{
										// so test fails
										return(FALSE);
									}

			// *** NOTE: movement mode hardcoded to WALKING !!!!!
			case ENSURE_PATH_COST:	return(PlotPath(pSoldier,sGridNo,FALSE,FALSE,FALSE,WALKING,FALSE,FALSE,0));

			default				:
#ifdef BETAVERSION
				NumMessage("LegalNPC->pathing.sDestination: ERROR - illegal pathMode = ",ubPathMode);
#endif
				return(FALSE);
		}
	}
	
	 // something failed - didn't even have to test path
	return(FALSE);			// illegal destination
}

int TryToResumeMovement(SOLDIERTYPE *pSoldier, INT32 sGridNo)
{
	UINT8 ubGottaCancel = FALSE;
	UINT8 ubSuccess = FALSE;


	// have to make sure the old destination is still legal (somebody may
	// have occupied the destination gridno in the meantime!)
	if (LegalNPCDestination(pSoldier,sGridNo,ENSURE_PATH,WATEROK,0))
	{
#ifdef DEBUGDECISIONS
		STR tempstr;
		sprintf(tempstr, "%d CONTINUES MOVEMENT to gridno %d...\n", pSoldier->ubID,sGridno );
		DebugAI( tempstr );
#endif

		pSoldier->pathing.bPathStored = TRUE;	// optimization - Ian

		// make him go to it (needed to continue movement across multiple turns)
		NewDest(pSoldier,sGridNo);

		ubSuccess = TRUE;

		// make sure that it worked (check that pSoldier->pathing.sDestination == pSoldier->sGridNo)
		if (pSoldier->pathing.sDestination == sGridNo)
		{
			ubSuccess = TRUE;
		}
		else
		{
#ifdef BETAVERSION
			sprintf(tempstr,"TryToResumeMovement: ERROR - NewDest failed for %s, action CANCELED",pSoldier->name);

#ifdef RECORDNET
			fprintf(NetDebugFile,"\n\t%s\n",tempstr);
#endif

			PopMessage(tempstr);
			SaveGame(ERROR_SAVE);
#endif

			// must work even for escorted civs, can't just set the flag
			DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: could not resume movement"));
			CancelAIAction(pSoldier,FORCE);
		}

	}
 else
	{
		// don't black-list anything here, this situation can come up quite
		// legally if another soldier gets in the way between turns

#ifdef BETAVERSION
		sprintf(tempstr,"TryToResumeMovement: %d can't continue to gridno %d, no longer legal!",pSoldier->ubID,gridno);

#ifdef RECORDNET
		fprintf(NetDebugFile,"\n\t%s\n",tempstr);
#endif

#ifdef DEBUGDECISIONS
		AIPopMessage(tempstr);
#endif

#endif


		if (!pSoldier->aiData.bUnderEscort)
		{
			DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: not under escort"));
			CancelAIAction(pSoldier,DONTFORCE);	// no need to force this
		}
		else
		{
			// this is an escorted NPC, don't want to just completely stop
			// moving, try to find a nearby "next best" destination if possible
			pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sGridNo,pSoldier->aiData.bAction);

			// if it's not possible to get any closer			
			if (TileIsOutOfBounds(pSoldier->aiData.usActionData))
			{
				ubGottaCancel = TRUE;
			}
			else
			{
				// change his desired destination to this new one
				sGridNo = pSoldier->aiData.usActionData;

				// GoAsFar... sets pathStored TRUE only if he could go all the way

				// make him go to it (needed to continue movement across multiple turns)
				NewDest(pSoldier,sGridNo);


				// make sure that it worked (check that pSoldier->pathing.sDestination == pSoldier->sGridNo)
				if (pSoldier->pathing.sDestination == sGridNo)
					ubSuccess = TRUE;
				else
					ubGottaCancel = TRUE;
			}

			if (ubGottaCancel)
			{
				// can't get close, gotta abort the movement!
				DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: can't get close, abort movement"));
				CancelAIAction(pSoldier,FORCE);

				// tell the player doing the escorting that civilian has stopped
				//EscortedMoveCanceled(pSoldier,COMMUNICATE);
			}
		}
	}

	return(ubSuccess);
}



INT32 NextPatrolPoint(SOLDIERTYPE *pSoldier)
{
 // patrol slot 0 is UNUSED, so max patrolCnt is actually only 9
 if ((pSoldier->aiData.bPatrolCnt < 1) || (pSoldier->aiData.bPatrolCnt >= MAXPATROLGRIDS))
	{
#ifdef BETAVERSION
	sprintf(tempstr,"NextPatrolPoint: ERROR: Invalid patrol count = %d for %s",pSoldier->aiData.bPatrolCnt,pSoldier->name);
	PopMessage(tempstr);
#endif

	return(NOWHERE);
	}


 pSoldier->aiData.bNextPatrolPnt++;


 // if there are no more patrol points, return back to the first one
 if (pSoldier->aiData.bNextPatrolPnt > pSoldier->aiData.bPatrolCnt)
	pSoldier->aiData.bNextPatrolPnt = 1;	// ZERO is not used!

 return(pSoldier->aiData.sPatrolGrid[pSoldier->aiData.bNextPatrolPnt]);
}



INT8 PointPatrolAI(SOLDIERTYPE *pSoldier)
{
 INT32 sPatrolPoint;
 INT8	bOldOrders;
#ifdef DEBUGDECISIONS
 STR16 tempstr;
#endif

 sPatrolPoint = pSoldier->aiData.sPatrolGrid[pSoldier->aiData.bNextPatrolPnt];

 // if we're already there, advance next patrol point
 if (pSoldier->sGridNo == sPatrolPoint || pSoldier->aiData.bNextPatrolPnt == 0)
	{
	// find next valid patrol point
	do
	{
	 sPatrolPoint = NextPatrolPoint(pSoldier);
	}	
	while (( !TileIsOutOfBounds(sPatrolPoint)) && !NewOKDestination(pSoldier,sPatrolPoint,IGNOREPEOPLE, pSoldier->pathing.bLevel) );

	// if we're back where we started, then ALL other patrol points are junk!
	if (pSoldier->sGridNo == sPatrolPoint)
	{
#ifdef BETAVERSION
	 NumMessage("PROBLEM WITH SCENARIO: All other patrol points are invalid for guynum ",pSoldier->ubID);
#endif
	 // force change of orders & an abort
	 sPatrolPoint = NOWHERE;
	}
	}

 // if we don't have a legal patrol point 
 if (TileIsOutOfBounds(sPatrolPoint))
	{
#ifdef BETAVERSION
	NumMessage("PointPatrolAI: ERROR - no legal patrol point for %d",pSoldier->ubID);
#endif

	// over-ride orders to something safer
	pSoldier->aiData.bOrders = FARPATROL;
	return(FALSE);
	}


 // make sure we can get there from here at this time, if we can't get all
 // the way there, at least do our best to get close
 if (LegalNPCDestination(pSoldier,sPatrolPoint,ENSURE_PATH,WATEROK,0))
	{
	pSoldier->pathing.bPathStored = TRUE;	 // optimization - Ian
	pSoldier->aiData.usActionData = sPatrolPoint;
	}
 else
	{
	// temporarily extend roaming range to infinity by changing orders, else
	// this won't work if the next patrol point is > 10 tiles away!
	bOldOrders					= pSoldier->aiData.bOrders;
	pSoldier->aiData.bOrders	= ONCALL;

	pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sPatrolPoint,pSoldier->aiData.bAction);

	pSoldier->aiData.bOrders = bOldOrders;

	// if it's not possible to get any closer, that's OK, but fail this call	
	if (TileIsOutOfBounds(pSoldier->aiData.usActionData))
	 return(FALSE);
	}


 // passed all tests - start moving towards next patrol point
#ifdef DEBUGDECISIONS
 sprintf(tempstr,"%s - POINT PATROL to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
 AIPopMessage(tempstr);
#endif

 return(TRUE);
}

INT8 RandomPointPatrolAI(SOLDIERTYPE *pSoldier)
{
#ifdef DEBUGDECISIONS
 STR16 tempstr;
#endif
	INT32 sPatrolPoint;
	INT8	bOldOrders, bPatrolIndex;
	INT8	bCnt;

	sPatrolPoint = pSoldier->aiData.sPatrolGrid[pSoldier->aiData.bNextPatrolPnt];

	// if we're already there, advance next patrol point
	if (pSoldier->sGridNo == sPatrolPoint || pSoldier->aiData.bNextPatrolPnt == 0)
	{
		// find next valid patrol point
		// we keep a count of the # of times we are in here to make sure we don't get into an endless
		//loop
		bCnt = 0;
		do
		{
			// usPatrolGrid[0] gets used for centre of close etc patrols, so we have to add 1 to the Random #
			bPatrolIndex = (INT8) PreRandom( pSoldier->aiData.bPatrolCnt ) + 1;
			sPatrolPoint = pSoldier->aiData.sPatrolGrid[ bPatrolIndex];
			bCnt++;
		}		
		while ( (sPatrolPoint == pSoldier->sGridNo) || ( (!TileIsOutOfBounds(sPatrolPoint)) && (bCnt < pSoldier->aiData.bPatrolCnt) && !NewOKDestination(pSoldier,sPatrolPoint,IGNOREPEOPLE, pSoldier->pathing.bLevel )) );

		if (bCnt == pSoldier->aiData.bPatrolCnt)
		{
			// ok, we tried doing this randomly, didn't work well, so now do a linear search
			pSoldier->aiData.bNextPatrolPnt = 0;
			do
			{
				sPatrolPoint = NextPatrolPoint(pSoldier);
			}			
			while ((!TileIsOutOfBounds(sPatrolPoint)) && !NewOKDestination(pSoldier,sPatrolPoint,IGNOREPEOPLE, pSoldier->pathing.bLevel) );
		}

		// do nothing this time around
		if (pSoldier->sGridNo == sPatrolPoint)
		{
			return( FALSE );
		}
	}

	// if we don't have a legal patrol point	
	if (TileIsOutOfBounds(sPatrolPoint))
	{
#ifdef BETAVERSION
		NumMessage("PointPatrolAI: ERROR - no legal patrol point for %d",pSoldier->ubID);
#endif

		// over-ride orders to something safer
		pSoldier->aiData.bOrders = FARPATROL;
		return(FALSE);
	}

	// make sure we can get there from here at this time, if we can't get all
	// the way there, at least do our best to get close
	if (LegalNPCDestination(pSoldier,sPatrolPoint,ENSURE_PATH,WATEROK,0))
	{
		pSoldier->pathing.bPathStored = TRUE;	 // optimization - Ian
		pSoldier->aiData.usActionData = sPatrolPoint;
	}
	else
	{
		// temporarily extend roaming range to infinity by changing orders, else
		// this won't work if the next patrol point is > 10 tiles away!
		bOldOrders					= pSoldier->aiData.bOrders;
		pSoldier->aiData.bOrders	= SEEKENEMY;

		pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sPatrolPoint,pSoldier->aiData.bAction);

		pSoldier->aiData.bOrders = bOldOrders;

		// if it's not possible to get any closer, that's OK, but fail this call		
		if (TileIsOutOfBounds(pSoldier->aiData.usActionData))
			return(FALSE);
	}


	// passed all tests - start moving towards next patrol point
#ifdef DEBUGDECISIONS
	sprintf(tempstr,"%s - POINT PATROL to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
	AIPopMessage(tempstr);
#endif

	return(TRUE);
}



INT32 InternalGoAsFarAsPossibleTowards(SOLDIERTYPE *pSoldier, INT32 sDesGrid, INT16 bReserveAPs, INT8 bAction, INT8 fFlags )
{
#ifdef DEBUGDECISIONS
	STR16 tempstr;
#endif
	INT32 sLoop,sAPCost;
	INT32 sTempDest,sGoToGrid;
	INT32 sOrigin;
	UINT16 usMaxDist;
	UINT8 ubDirection,ubDirsLeft,ubDirChecked[8],fFound = FALSE;
	// anv: changed to INT16 since Soldier->bActionPoints are INT16 too
	INT16 bAPsLeft, fPathFlags;
	//DBrot: More Rooms
	//UINT8 ubRoomRequired = 0, ubTempRoom;
	UINT16 usRoomRequired = 0, usTempRoom;
	BOOLEAN fAllowDest = FALSE;

	// 0verhaul:	Make sure to clear the stored path since this always calculates a new one.
	// This is needed if the path cannot be found.
	pSoldier->pathing.bPathStored = FALSE;

	if ( bReserveAPs == -1 )
	{
		// default reserve points
		if ( CREATURE_OR_BLOODCAT( pSoldier ) )
		{
			bReserveAPs = 0;
		}
		else
		{
			bReserveAPs = APBPConstants[MAX_AP_CARRIED];
		}
	}

	sTempDest = -1;

	// obtain maximum roaming distance from soldier's sOrigin
	usMaxDist = RoamingRange(pSoldier,&sOrigin);

	if ( pSoldier->aiData.bOrders <= CLOSEPATROL && (pSoldier->bTeam == CIV_TEAM || pSoldier->ubProfile != NO_PROFILE ) )
	{
		if ( InARoom( pSoldier->aiData.sPatrolGrid[0], &usRoomRequired ) )
		{
			// make sure this doesn't interfere with pathing for scripts			
			if (!TileIsOutOfBounds(pSoldier->sAbsoluteFinalDestination))
			{
				usRoomRequired = 0;
			}
		}
	}

	pSoldier->usUIMovementMode = DetermineMovementMode(pSoldier, bAction );
	if ( pSoldier->usUIMovementMode == RUNNING && fFlags & FLAG_CAUTIOUS )
	{
		pSoldier->usUIMovementMode = WALKING;
	}

#ifdef DEBUGDECISIONS
	sprintf(tempstr,"%s wants to go towards %d (has range %d)",pSoldier->name,sDesGrid,usMaxDist);
	AIPopMessage(tempstr);
#endif

	// if soldier is ALREADY at the desired destination, quit right away
	if (sDesGrid == pSoldier->sGridNo)
	{
		return(NOWHERE);
	}

	// don't try to approach go after noises or enemies actually in water
	// would be too easy to throw rocks in water, etc. & distract the AI
	if (Water(sDesGrid, pSoldier->pathing.bLevel))
	{
		return(NOWHERE);
	}

	fPathFlags = 0;
	if ( CREATURE_OR_BLOODCAT( pSoldier ) )
	{
			fPathFlags = PATH_CLOSE_GOOD_ENOUGH;
	}

	// first step: try to find an OK destination at or near the desired gridno
	if (!LegalNPCDestination(pSoldier,sDesGrid,ENSURE_PATH,NOWATER,fPathFlags))
	{
#ifdef DEBUGDECISIONS
		AIPopMessage("destination Grid # itself not valid, looking around it");
#endif
		if ( CREATURE_OR_BLOODCAT( pSoldier ) )
		{
			// we tried to get close, failed; abort!
			pSoldier->pathing.usPathIndex = pSoldier->pathing.usPathDataSize = 0;
			return( NOWHERE );
		}
		else
		{
			// else look at the 8 nearest gridnos to sDesGrid for a valid destination

			// clear ubDirChecked flag for all 8 directions
			for ( ubDirection = 0; ubDirection < NUM_WORLD_DIRECTIONS; ++ubDirection )
				ubDirChecked[ubDirection] = FALSE;

			ubDirsLeft = NUM_WORLD_DIRECTIONS;

			// examine all 8 spots around 'sDesGrid'
			// keep looking while directions remain and a satisfactory one not found
			for ( ubDirsLeft = NUM_WORLD_DIRECTIONS; ubDirsLeft != 0; --ubDirsLeft )
			{
				if (fFound)
				{
					break;
				}
				// randomly select a direction which hasn't been 'checked' yet
				do
				{
					ubDirection = (UINT8) Random(8);
				}
				while (ubDirChecked[ubDirection]);

				ubDirChecked[ubDirection] = TRUE;

				// determine the gridno 1 tile away from current friend in this direction
				sTempDest = NewGridNo(sDesGrid,DirectionInc( ubDirection ));

				// if that's out of bounds, ignore it & check next direction
				if (sTempDest == sDesGrid)
					continue;

				if (LegalNPCDestination(pSoldier,sTempDest,ENSURE_PATH,NOWATER,0))
				{
					fFound = TRUE;			// found a spot

#ifdef DEBUGDECISIONS
					AINumMessage("Found a spot!	ubDirection = ",ubDirection + 1);
#endif

					break;					// stop checking in other directions
				}
			}

			if (!fFound)
			{
#ifdef DEBUGDECISIONS
				AINumMessage("Couldn't find OK destination around grid #",sDesGrid);
#endif

				pSoldier->pathing.usPathIndex = pSoldier->pathing.usPathDataSize = 0;
				return(NOWHERE);
			}

			// found a good grid #, this becomes our actual desired grid #
			sDesGrid = sTempDest;
		}
	}

	// HAVE FOUND AN OK destination AND PLOTTED A VALID BEST PATH TO IT


#ifdef DEBUGDECISIONS
	AINumMessage("Chosen legal destination is gridno ",sDesGrid);
	AINumMessage("Tracing along path, pathRouteToGo = ",pSoldier->pathing.usPathIndex);
#endif

	 sGoToGrid = pSoldier->sGridNo;		// start back where soldier is standing now
	 sAPCost = 0;			// initialize path cost counter

	// 0verhaul:  If the destination is within the patrol route, allow it.  This is necessary to allow an errant soldier
	// to return to his patrol route if flanking or other actions have pulled him beyond his allowed range from origin.
	if (SpacesAway(sOrigin,sDesGrid) <= usMaxDist)
	{
		fAllowDest = TRUE;
	}

	// we'll only go as far along the plotted route as is within our
	// permitted roaming range, and we'll stop as soon as we're down to <= 5 APs

	sTempDest = pSoldier->sGridNo;

	for (sLoop = 0; sLoop < (pSoldier->pathing.usPathDataSize - pSoldier->pathing.usPathIndex); sLoop++)
	{
		// what is the next gridno in the path?

		//sTempDest = NewGridNo( sGoToGrid,DirectionInc( (INT16) (pSoldier->pathing.usPathingData[sLoop] + 1) ) );
		//sTempDest = NewGridNo( sGoToGrid,DirectionInc( (UINT8) (pSoldier->pathing.usPathingData[sLoop]) ) );
		sTempDest = NewGridNo( sTempDest,DirectionInc( (UINT8) (pSoldier->pathing.usPathingData[sLoop]) ) );		

		// this should NEVER be out of bounds
		if (sTempDest == sGoToGrid)
		{
#ifdef BETAVERSION
			sprintf(tempstr,"GoAsFarAsPossibleTowards: ERROR - gridno along valid route is invalid!	guynum %d, sTempDest = %d",pSoldier->ubID,sTempDest);

#ifdef RECORDNET
			fprintf(NetDebugFile,"\n\t%s\n",tempstr);
#endif

			PopMessage(tempstr);
			SaveGame(ERROR_SAVE);
#endif
			break;			// quit here, sGoToGrid is where we are going
		}

		// if this takes us beyond our permitted "roaming range"
		// but if it brings us closer, then allow it!
		if (SpacesAway(sOrigin,sTempDest) > usMaxDist && !fAllowDest)
			break;			// quit here, sGoToGrid is where we are going


		if ( usRoomRequired )
		{
			if ( !( InARoom( sTempDest, &usTempRoom ) && usTempRoom == usRoomRequired ) )
			{
				// quit here, limited by room!
				break;
			}
		}

		if ( (fFlags & FLAG_STOPSHORT) && SpacesAway( sDesGrid, sTempDest ) <= STOPSHORTDIST )
		{
			break;			// quit here, sGoToGrid is where we are going
		}

		// CAN'T CALL PathCost() HERE! IT CALLS findBestPath() and overwrites
		//		pathRouteToGo !!!	Gotta calculate the cost ourselves - Ian
		//
		//ubAPsLeft = pSoldier->bActionPoints - PathCost(pSoldier,sTempDest,FALSE,FALSE,FALSE,FALSE,FALSE);

		if (gfTurnBasedAI)
		{
			// if we're just starting the "costing" process (first gridno)
			if (sLoop == 0)
			{
				if (pSoldier->usUIMovementMode == RUNNING)
				{
					sAPCost += GetAPsStartRun( pSoldier ); // changed by SANDRO
				}
			}

			// ATE: Direction here?
			sAPCost += EstimateActionPointCost( pSoldier, sTempDest, (INT8) pSoldier->pathing.usPathingData[sLoop], pSoldier->usUIMovementMode, (INT8) sLoop, (INT8) pSoldier->pathing.usPathDataSize );
			bAPsLeft = pSoldier->bActionPoints - sAPCost;
		}

		// if this gridno is NOT a legal NPC destination
		// DONT'T test path again - that would replace the traced path! - Ian
		// NOTE: It's OK to go *THROUGH* water to try and get to the destination!
		// sevenfm: jump over fence code - if current gridno is not legal destination, check next gridno
		if (!LegalNPCDestination(pSoldier,sTempDest,IGNORE_PATH,WATEROK,0))
		{
			// break;
			continue;
		}

		// if after this, we have <= 5 APs remaining, that's far enough, break out
		// (the idea is to preserve APs so we can crouch or react if
		// necessary, and benefit from the carry-over next turn if not needed)
		// This routine is NOT used by any GREEN AI, so such caution is warranted!

		if ( gfTurnBasedAI && (bAPsLeft < bReserveAPs) )
			break;
		else
		{
			sGoToGrid = sTempDest;	// we're OK up to here

			// if exactly 5 APs left, don't bother checking any further
			if ( gfTurnBasedAI && (bAPsLeft == bReserveAPs) )
				break;
		}
	}


	// if it turned out we couldn't go even 1 tile towards the desired gridno
	if (sGoToGrid == pSoldier->sGridNo)
	{
#ifdef DEBUGDECISIONS
		sprintf(tempstr,"%s will go NOWHERE, path doesn't meet criteria",pSoldier->name);
		AIPopMessage(tempstr);
#endif

		pSoldier->pathing.usPathIndex = pSoldier->pathing.usPathDataSize = 0;
		return(NOWHERE);			 // then go nowhere
	}
	else
	{
		// possible optimization - stored path IS good if we're going all the way
		if (sGoToGrid == sDesGrid)
		{
			pSoldier->pathing.bPathStored = TRUE;
			pSoldier->pathing.sFinalDestination = sGoToGrid;
		}
		else if ( pSoldier->pathing.usPathIndex == 0 )
		{
			// we can hack this surely! -- CJC
			pSoldier->pathing.bPathStored = TRUE;
			pSoldier->pathing.sFinalDestination = sGoToGrid;
			pSoldier->pathing.usPathDataSize = sLoop + 1;
		}

#ifdef DEBUGDECISIONS
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"%d to %d with %d APs left", pSoldier->ubID, sGoToGrid, pSoldier->bActionPoints );
 #endif
		return( sGoToGrid );
	}
}

INT32 GoAsFarAsPossibleTowards(SOLDIERTYPE *pSoldier, INT32 sDesGrid, INT8 bAction)
{
	return( InternalGoAsFarAsPossibleTowards( pSoldier, sDesGrid, -1, bAction, 0 ) );
}

void SoldierTriesToContinueAlongPath(SOLDIERTYPE *pSoldier)
{
	INT32 usNewGridNo,bAPCost;


	// turn off the flag now that we're going to do something about it...
	// ATE: USed to be redundent, now if called befroe NewDest can cause some side efects...
	// pSoldier->AdjustNoAPToFinishMove( FALSE );

	if (pSoldier->aiData.bNewSituation == IS_NEW_SITUATION)
	{
		DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: IS_NEW_SITUATION"));
		CancelAIAction(pSoldier,DONTFORCE);
		return;
	}

	if (TileIsOutOfBounds(pSoldier->aiData.usActionData))
	{
		DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: bad usActionData"));
		CancelAIAction(pSoldier,DONTFORCE);
		return;
	}

	if (!NewOKDestination( pSoldier,pSoldier->aiData.usActionData, TRUE, pSoldier->pathing.bLevel ))
	{
		DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: !NewOKDestination"));
		CancelAIAction(pSoldier,DONTFORCE);
		return;
	}

	// WANNE: Disabled the following lines to fix the bug, that Fatima will not leave the sector!
	// When Fatima wants to leave the sectur, the condition is met, the method returns and Fatima would stay in the sector!
	/*
	// SANDRO - hack! interrupt issue - we don't want to recalculate our path if no new situation and we are already on move
	// i.e. in case we interrupted a soldier who has no idea about us seeing him, he should move along as if nothing is happening
	if ( pSoldier->aiData.bNewSituation == NOT_NEW_SITUATION && pSoldier->aiData.bActionInProgress && !TileIsOutOfBounds(pSoldier->pathing.sFinalDestination ))
	{
		// just set our path to previously decided final destination
		NewDest(pSoldier,pSoldier->pathing.sFinalDestination);
		return;	
	}
	*/

	if (IsActionAffordable(pSoldier))
	{
		if (pSoldier->aiData.bActionInProgress == FALSE)
		{
			// start a move that didn't even get started before...
			// hope this works...
			NPCDoesAct(pSoldier);

			// perform the chosen action
			pSoldier->aiData.bActionInProgress = ExecuteAction(pSoldier); // if started, mark us as busy
		}
		else
		{
			// otherwise we shouldn't have to do anything(?)
		}
	}
	else
	{
		DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: !IsActionAffordable"));
		CancelAIAction(pSoldier,DONTFORCE);
#ifdef TESTAI
		DebugMsg( TOPIC_JA2AI, DBG_LEVEL_3,
						String("Soldier (%d) HAS NOT ENOUGH AP to continue along path",pSoldier->ubID) );
#endif
	}

	usNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( (UINT8)pSoldier->pathing.usPathingData[ pSoldier->pathing.usPathIndex ] ) );

	// Find out how much it takes to move here!
	bAPCost = EstimateActionPointCost( pSoldier, usNewGridNo, (INT8)pSoldier->pathing.usPathingData[ pSoldier->pathing.usPathIndex ], pSoldier->usUIMovementMode, (INT8) pSoldier->pathing.usPathIndex, (INT8) pSoldier->pathing.usPathDataSize );

	if (pSoldier->bActionPoints >= bAPCost)
	{
		// seems to have enough points...
		NewDest(pSoldier,usNewGridNo);
		// maybe we didn't actually start the action last turn...
		pSoldier->aiData.bActionInProgress = TRUE;
#ifdef TESTAI
		DebugMsg( TOPIC_JA2AI, DBG_LEVEL_3,
						String("Soldier (%d) continues along path",pSoldier->ubID) );
#endif
	}
	else
	{
		DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: not enough AP"));
		CancelAIAction(pSoldier,DONTFORCE);
#ifdef TESTAI
		DebugMsg( TOPIC_JA2AI, DBG_LEVEL_3,
						String("Soldier (%d) HAS NOT ENOUGH AP to continue along path",pSoldier->ubID) );
#endif
	}
}

void HaltMoveForSoldierOutOfPoints(SOLDIERTYPE *pSoldier)
{
	// If a special move, ignore this!
	if ( ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_SPECIALMOVE ) )
	{
		return;
	}

	if (is_networked)
	{
		EV_S_STOP_MERC				SStopMerc;

		SStopMerc.sGridNo					= pSoldier->sGridNo;
		SStopMerc.ubDirection			= pSoldier->ubDirection;
		SStopMerc.usSoldierID			= pSoldier->ubID;
		SStopMerc.fset=TRUE;
		SStopMerc.sXPos=pSoldier->sX;
		SStopMerc.sYPos=pSoldier->sY;

		//AddGameEvent( S_STOP_MERC, 0, &SStopMerc ); //hayden.
		if(pSoldier->ubID>=120 || (!is_server && pSoldier->ubID>=20)) 
			return;//hayden

		if(is_client)
			send_stop(&SStopMerc);
	}
	// record that this merc can no longer animate and why...
	pSoldier->AdjustNoAPToFinishMove( TRUE );

	// We'll keep his action intact though...
	DebugAI( String("NO AP TO FINISH MOVE for %d (%d APs left)",pSoldier->ubID, pSoldier->bActionPoints) );

	// if this dude is under AI right now, then pass the baton to someone else
	if (pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL)
	{
		#ifdef TESTAICONTROL
			DebugAI( String("Ending turn for %d because out of APs for movement", pSoldier->ubID ) );
		#endif

		EndAIGuysTurn(pSoldier);
	}
}

void SetCivilianDestination(UINT8 ubWho, INT32 sGridNo)
{
 SOLDIERTYPE *pSoldier;


 pSoldier = MercPtrs[ubWho];

/*
 // if we control the civilian
 if (PTR_OURCONTROL)
	{
*/
	// if the destination is different from what he has now
	if (pSoldier->aiData.usActionData != sGridNo)
	{
	 // store his new destination
	 pSoldier->aiData.usActionData = sGridNo;

	 // and cancel any movement in progress that he was still engaged in
	 pSoldier->aiData.bAction = AI_ACTION_NONE;
	 pSoldier->aiData.bActionInProgress = FALSE;
	}

	// only set the underEscort flag once you give him a destination
	// (that way AI can keep him appearing to act on his own until you
	// give him orders).
	//
	// Either way, once set, it should stay that way, preventing AI from
	// doing anything other than advance him towards destination.
	pSoldier->aiData.bUnderEscort = TRUE;

	// change orders to maximize roaming range so he can Go As Far As Possible
	pSoldier->aiData.bOrders = ONCALL;
/*
	}

 else
	{
	NetSend.msgType = NET_CIV_DEST;
	NetSend.ubID	= pSoldier->ubID;
	NetSend.gridno	= gridno;

	// only the civilian's controller needs to know this
	SendNetData(pSoldier->controller);
	}
*/
}


#define RADIUS 3

INT32 TrackScent( SOLDIERTYPE * pSoldier )
{
	// This function returns the best gridno to go to based on the scent being followed,
	// and the soldier (creature/animal)'s current direction (which is used to resolve
	// ties.
	INT32						iXDiff, iYDiff, iXIncr;
	INT32						iStart, iXStart, iYStart;
	INT32						iGridNo;
	INT8						bDir;
	INT32						iBestGridNo = NOWHERE;
	UINT8						ubBestDirDiff = 5, ubBestStrength = 0;
	UINT8						ubDirDiff, ubStrength;
	UINT8						ubSoughtSmell;
	MAP_ELEMENT *		pMapElement;

	iStart = pSoldier->sGridNo;
	iXStart = iStart % WORLD_COLS;
	iYStart = iStart / WORLD_COLS;

	if (CREATURE_OR_BLOODCAT( pSoldier ) ) // or bloodcats
	{
		// tracking humans; search the edges of a 7x7 square for the
		// most promising tile
		ubSoughtSmell = HUMAN;
		for (iYDiff = -RADIUS; iYDiff < (RADIUS + 1); iYDiff++)
		{
			if (iYStart + iYDiff < 0)
			{
				// outside of map! might be on map further down...
				continue;
			}
			else if (iYStart + iYDiff > WORLD_ROWS)
			{
				// outside of bottom of map! abort!
				break;
			}
			if (iYDiff == -RADIUS || iYDiff == RADIUS)
			{
				iXIncr = 1;
			}
			else
			{
				// skip over the spots in the centre of the square
				iXIncr = RADIUS * 2;
			}
			for (iXDiff = -RADIUS; iXDiff < (RADIUS + 1); iXDiff += iXIncr)
			{
				iGridNo = iStart + iXDiff + iYDiff * WORLD_ROWS;
				if (abs( iGridNo % WORLD_ROWS - iXStart ) > RADIUS)
				{
					// wrapped across map!
					continue;
				}
				if (LegalNPCDestination(pSoldier,pSoldier->aiData.usActionData,ENSURE_PATH,WATEROK,0))
				{
					// check this location out
					pMapElement = &(gpWorldLevelData[iGridNo]);
					if (pMapElement->ubSmellInfo && (SMELL_TYPE( pMapElement->ubSmellInfo ) == ubSoughtSmell))
					{
						ubStrength = SMELL_STRENGTH( pMapElement->ubSmellInfo );
						if (ubStrength > ubBestStrength )
						{
							iBestGridNo = iGridNo;
							ubBestStrength = ubStrength;
							bDir = atan8( (INT16) iXStart, (INT16) iYStart, (INT16) (iXStart + iXDiff), (INT16) (iYStart + iYDiff) );
							// now convert it into a difference in degree between it and our current dir
							ubBestDirDiff = abs( pSoldier->ubDirection - bDir );
							if (ubBestDirDiff > 4 ) // dir 0 compared with dir 6, for instance
							{
								ubBestDirDiff = 8 - ubBestDirDiff;
							}
						}
						else if (ubStrength == ubBestStrength)
						{							
							if (TileIsOutOfBounds(iBestGridNo))
							{
								// first place we've found with the same strength
								iBestGridNo = iGridNo;
								ubBestStrength = ubStrength;
							}
							else
							{
								// use directions to decide between the two
								// start by calculating direction to the new gridno
								bDir = atan8( (INT16) iXStart, (INT16) iYStart, (INT16) (iXStart + iXDiff), (INT16) (iYStart + iYDiff) );
								// now convert it into a difference in degree between it and our current dir
								ubDirDiff = abs( pSoldier->ubDirection - bDir );
								if (ubDirDiff > 4 ) // dir 0 compared with dir 6, for instance
								{
									ubDirDiff = 8 - ubDirDiff;
								}
								if (ubDirDiff < ubBestDirDiff || ((ubDirDiff == ubBestDirDiff) && Random( 2 )))
								{
									// follow this trail as its closer to the one we're following!
									// (in the case of a tie, we tossed a coin)
									ubBestDirDiff = ubDirDiff;

								}
							}
						}
					}
				}
			}
			// go on to next tile
		}
		// go on to next row
	}
	else
	{
		// who else can track?
	}	
	if (!TileIsOutOfBounds(iBestGridNo))
	{
		pSoldier->aiData.usActionData = iBestGridNo;
		return( iBestGridNo );
	}
	return( 0 );
}

/*
UINT16 RunAway( SOLDIERTYPE * pSoldier )
{
	// "Run away! Run away!!!"
	// This code should figure out which directions are safe for the enemy
	// to run in.	They shouldn't try to run off in directions which will
	// take them into enemy territory.	We must presume that they inform each
	// other by radio when sectors are taken by the player! :-)
	// The second wrinkle would be to look at the directions to known player
	// mercs and use that to influence the direction in which we run.

	// we can only flee in the cardinal directions (NESW) so start with an
	// alternating pattern of true/false
	INT8 bOkayDir[8] = {TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE};
	UINT8 ubLoop, ubBestDir, ubDistToEdge, ubBestDistToEdge = WORLD_COLS;
	INT32	iSector, iSectorX, iSectorY;
	INT32 iNewSectorX, iNewSectorY, iNewSector;
	INT32	iRunX, iRunY, iRunGridNo;
	SOLDIERTYPE * pOpponent;

	iSector = pSoldier->sSectorX + pSoldier->sSectorY * MAP_WORLD_X;

	// first start by scanning through opposing mercs and find out what directions are blocked.
	for (ubLoop = 0,pOpponent = Menptr; ubLoop < MAXMERCS; ubLoop++,pOpponent++)
	{
		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpponent->bActive || !pOpponent->bInSector || !pOpponent->stats.bLife)
		{
			continue;			// next merc
		}

		// if this man is neutral / on the same side, he's not an opponent
		if (pOpponent->aiData.bNeutral || (pSoldier->bSide == pOpponent->bSide))
		{
			continue;			// next merc
		}

		// we don't want to run in that direction!
		bOkayDir[ atan8( pSoldier->sX, pSoldier->sY, pOpponent->sX, pOpponent->sY ) ] = FALSE;
	}

	for (ubLoop = 0; ubLoop < NUM_WORLD_DIRECTIONS; ubLoop += 2)
	{
		if (bOkayDir[ubLoop])
		{
			// figure out sector # in that direction
			iNewSectorX = pSoldier->sSectorX + DirXIncrementer[ubLoop];
			iNewSectorY = pSoldier->sSectorY + DirYIncrementer[ubLoop];
			iNewSector = iSectorX + iSectorY * MAP_WORLD_X;
			// check movement
			if (TravelBetweenSectorsIsBlockedFromFoot( (UINT16) iSector, (UINT16) iNewSector ) || StrategicMap[iSector].fEnemyControlled)
			{
				// sector inaccessible or controlled by the player; skip it!
				continue;
			}
			switch( ubLoop )
			{
				case 0:
					ubDistToEdge = pSoldier->sGridNo / WORLD_COLS;
					break;
				case 2:
					ubDistToEdge = WORLD_COLS - pSoldier->sGridNo % WORLD_COLS;
					break;
				case 4:
					ubDistToEdge = WORLD_ROWS - pSoldier->sGridNo / WORLD_COLS;
					break;
				case 6:
					ubDistToEdge = pSoldier->sGridNo % WORLD_COLS;
					break;
			}
			if (ubDistToEdge < ubBestDistToEdge)
			{
				ubBestDir = ubLoop;
				ubBestDistToEdge = ubDistToEdge;
			}
		}
	}
	if (ubBestDistToEdge < WORLD_COLS)
	{
		switch( ubBestDir )
		{
			case 0:
				iRunX = pSoldier->sX + Random( 9 ) - 4;
				iRunY = 0;
				if (iRunX < 0)
				{
					iRunX = 0;
				}
				else if (iRunX >= WORLD_COLS)
				{
					iRunX = WORLD_COLS - 1;
				}
				break;
			case 2:
				iRunX = WORLD_COLS;
				iRunY = pSoldier->sY + Random( 9 ) - 4;
				if (iRunY < 0)
				{
					iRunY = 0;
				}
				else if (iRunY >= WORLD_COLS)
				{
					iRunY = WORLD_ROWS - 1;
				}
				break;
			case 4:
				iRunX = pSoldier->sX + Random( 9 ) - 4;
				iRunY = WORLD_ROWS;
				if (iRunX < 0)
				{
					iRunX = 0;
				}
				else if (iRunX >= WORLD_COLS)
				{
					iRunX = WORLD_COLS - 1;
				}
				break;
			case 6:
				iRunX = 0;
				iRunY = pSoldier->sY + Random( 9 ) - 4;
				if (iRunY < 0)
				{
					iRunY = 0;
				}
				else if (iRunY >= WORLD_COLS)
				{
					iRunY = WORLD_ROWS - 1;
				}
				break;
		}
		iRunGridNo = iRunX + iRunY * WORLD_COLS;
		if (LegalNPCDestination( pSoldier, (UINT16) iRunGridNo, ENSURE_PATH, TRUE,0))
		{
			return( (UINT16) iRunGridNo );
		}
		// otherwise we'll try again another time
	}
	return( NOWHERE );
}
*/
