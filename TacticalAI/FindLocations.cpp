#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include <stdlib.h>
	#include "Isometric Utils.h"
	#include "ai.h"
	#include "AIInternals.h"
	#include "los.h"
	#include "Weapons.h"
	#include "Opplist.h"
	#include "pathai.h"
	#include "items.h"
	#include "World Items.h"
	#include "Points.h"
	#include "message.h"
	#include "Map Edgepoints.h"
	#include "renderworld.h"
	#include "Render Fun.h"
	#include "Boxing.h"
	#include "Text.h"
	#ifdef _DEBUG
		#include "renderworld.h"
		#include "video.h"
	#endif
	#include "worldman.h"
	#include "strategicmap.h"
	#include "environment.h"
	#include "lighting.h"
	#include "Buildings.h"
	#include "GameSettings.h"
	#include "Soldier Profile.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - all "APBPConstants[AP_CROUCH]" and "APBPConstants[AP_PRONE]" here    
//           were changed to GetAPsCrouch() and GetAPsProne()					 
//		  - also all "APBPConstants[AP_PICKUP_ITEM]" were replaced by GetBasicAPsToPickupItem()
////////////////////////////////////////////////////////////////////////////////////////////////////////

	INT16 * gsCoverValue = NULL;
#ifdef _DEBUG

	INT16	gsBestCover;
	#ifndef PATHAI_VISIBLE_DEBUG
		// NB Change this to true to get visible cover debug -- CJC
		BOOLEAN gfDisplayCoverValues = FALSE;
	#endif
	extern void RenderCoverDebug( void );
#endif

INT16	gubAIPathCosts[19][19];

// FindBestNearbyCover - "Net" related stuff commented out
extern UINT8 gubAICounter;
extern BOOLEAN gfTurnBasedAI;

INT32 CalcPercentBetter(INT32 iOldValue, INT32 iNewValue, INT32 iOldScale, INT32 iNewScale)
{
 INT32 iValueChange,iScaleSum,iPercentBetter;//,loopCnt,tempInt;

 // calcalate how much better the new cover would be than the current cover
 iValueChange = iNewValue - iOldValue;


 // here, the change in cover HAS to be an improvement over current cover
 if (iValueChange <= 0)
	{
#ifdef BETAVERSION
	sprintf(tempstr,"CalcPercentBetter: ERROR - invalid valueChange = %d",valueChange);

#ifdef RECORDNET
	fprintf(NetDebugFile,"\n\t%s\n\n",tempstr);
#endif

	PopMessage(tempstr);
#endif

	return(NOWHERE);
	}


 iScaleSum = iOldScale + iNewScale;

 // here, the change in cover HAS to be an improvement over current cover
 if (iScaleSum <= 0)
	{
#ifdef BETAVERSION
	sprintf(tempstr,"CalcPercentBetter: ERROR - invalid scaleSum = %d",iScaleSum);

#ifdef RECORDNET
	fprintf(NetDebugFile,"\n\t%s\n\n",tempstr);
#endif

	PopMessage(tempstr);
#endif

	return(NOWHERE);
	}



 iPercentBetter = (iValueChange * 100) / iScaleSum;

#ifdef DEBUGCOVER
 DebugAI( String( "CalcPercentBetter: %%Better %ld, old %ld, new %ld, change %ld\n\t\toldScale %ld, newScale %ld, scaleSum %ld\n",
	iPercentBetter,iOldValue,iNewValue,iValueChange,iOldScale,iNewScale,iScaleSum ) );
#endif


 return(iPercentBetter);
}

void AICenterXY( INT32 sGridNo, FLOAT * pdX, FLOAT * pdY )
{
	INT16		sXPos, sYPos;

	sXPos = sGridNo % WORLD_COLS;
	sYPos = sGridNo / WORLD_COLS;

	*pdX = (FLOAT) (sXPos * CELL_X_SIZE + CELL_X_SIZE / 2);
	*pdY = (FLOAT) (sYPos * CELL_Y_SIZE + CELL_Y_SIZE / 2);
}

INT8 CalcWorstCTGTForPosition( SOLDIERTYPE * pSoldier, UINT8 ubOppID, INT32 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft )
{
	// When considering a gridno for cover, we want to take into account cover if we
	// lie down, so we return the LOWEST chance to get through for that location.
	INT8		bCubeLevel, bThisCTGT,bWorstCTGT = 100;

	for (bCubeLevel = 1; bCubeLevel <= 3; bCubeLevel++)
	{
		switch (bCubeLevel)
		{
			case 1:
				if (iMyAPsLeft < GetAPsCrouch(pSoldier,TRUE) + GetAPsProne(pSoldier,TRUE))
				{
					continue;
				}
				break;
			case 2:
				if (iMyAPsLeft < GetAPsCrouch(pSoldier,TRUE))
				{
					continue;
				}
				break;
			default:
				break;
		}

		bThisCTGT = SoldierToLocationChanceToGetThrough( pSoldier, sOppGridNo, bLevel, bCubeLevel, ubOppID );
		if (bThisCTGT < bWorstCTGT)
		{
			bWorstCTGT = bThisCTGT;
			// if there is perfect cover
			if (bWorstCTGT == 0)
				// then bail from the for loop, it can't possible get any better
				break;
		}
	}
	return( bWorstCTGT );
}

INT8 CalcAverageCTGTForPosition( SOLDIERTYPE * pSoldier, UINT8 ubOppID, INT32 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft )
{
	// When considering a gridno for cover, we want to take into account cover if we
	// lie down, so we return the LOWEST chance to get through for that location.
	INT8		bCubeLevel;
	INT32		iTotalCTGT = 0, bValidCubeLevels = 0;

	for (bCubeLevel = 1; bCubeLevel <= 3; bCubeLevel++)
	{
			switch (bCubeLevel)
		{
			case 1:
				if (iMyAPsLeft < GetAPsCrouch(pSoldier,TRUE) + GetAPsProne(pSoldier,TRUE))
				{
					continue;
				}
				break;
			case 2:
				if (iMyAPsLeft < GetAPsCrouch(pSoldier,TRUE))
				{
					continue;
				}
				break;
			default:
				break;
		}
		iTotalCTGT += SoldierToLocationChanceToGetThrough( pSoldier, sOppGridNo, bLevel, bCubeLevel, ubOppID );
		bValidCubeLevels++;
	}
	iTotalCTGT /= bValidCubeLevels;
	return( (INT8) iTotalCTGT );
}


INT8 CalcBestCTGT( SOLDIERTYPE *pSoldier, UINT8 ubOppID, INT32 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft )
{
	// NOTE: CTGT stands for "ChanceToGetThrough..."

	// using only ints for maximum execution speed here
	// CJC: Well, so much for THAT idea!
	INT32 sCentralGridNo, sAdjSpot, sNorthGridNo, sSouthGridNo, sCheckSpot, sOKTest;

	INT8 bThisCTGT, bBestCTGT = 0;

	sCheckSpot = -1;

	sCentralGridNo = pSoldier->sGridNo;

	// precalculate these for speed
	// what was struct for?
	sOKTest = NewOKDestination( pSoldier, sCentralGridNo, IGNOREPEOPLE , bLevel );
	sNorthGridNo = NewGridNo( sCentralGridNo, DirectionInc(NORTH) );
	sSouthGridNo = NewGridNo( sCentralGridNo, DirectionInc(SOUTH) );

	// look into all 8 adjacent tiles & determine where the cover is the worst
	// Lalien: shouldn't this start at 0 than?
	for (UINT8 sDir = 0; sDir < NUM_WORLD_DIRECTIONS; sDir++)
	{
		// get the gridno of the adjacent spot lying in that direction
		sAdjSpot = NewGridNo( sCentralGridNo, DirectionInc( sDir ) );

		// if it wasn't out of bounds
		if (sAdjSpot != sCentralGridNo)
		{
			// if the adjacent spot can we walked on and isn't in water or gas
			if ((NewOKDestination( pSoldier, sAdjSpot, IGNOREPEOPLE, bLevel ) > 0) && !InWaterOrGas( pSoldier, sAdjSpot ))
			{
				switch (sDir)
				{
					case NORTH:
					case EAST:
					case SOUTH:
					case WEST:
						sCheckSpot = sAdjSpot;
						break;
					case NORTHEAST:
					case NORTHWEST:
						// spot to the NORTH is guaranteed to be in bounds since NE/NW was
						sCheckSpot = sNorthGridNo;
						break;
					case SOUTHEAST:
					case SOUTHWEST:
						// spot to the SOUTH is guaranteed to be in bounds since SE/SW was
						sCheckSpot = sSouthGridNo;
						break;
				}

				// ATE: OLD STUFF
				// if the adjacent gridno is reachable from the starting spot
				if ( NewOKDestination( pSoldier, sCheckSpot, FALSE, bLevel ) )
				{
					// the dude could move to this adjacent gridno, so put him there
					// "virtually" so we can calculate what our cover is from there

					// NOTE: GOTTA SET THESE 3 FIELDS *BACK* AFTER USING THIS FUNCTION!!!
					pSoldier->sGridNo = sAdjSpot;	 // pretend he's standing at 'sAdjSpot'
					AICenterXY( sAdjSpot, &(pSoldier->dXPos), &(pSoldier->dYPos) );
					bThisCTGT = CalcWorstCTGTForPosition( pSoldier, ubOppID, sOppGridNo, bLevel, iMyAPsLeft );
					if (bThisCTGT > bBestCTGT)
					{
						bBestCTGT = bThisCTGT;
						// if there is no cover
						if (bBestCTGT == 100)
							// then bail from the for loop, it can't possible get any better
							break;
					}
				}
			}
		}
	}

	return( bBestCTGT );
}


INT32 CalcCoverValue(SOLDIERTYPE *pMe, INT32 sMyGridNo, INT32 iMyThreat, INT32 iMyAPsLeft,
					UINT32 uiThreatIndex, INT32 iRange, INT32 morale, INT32 *iTotalScale)
{
	DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("CalcCoverValue"));

	// all 32-bit integers for max. speed
	INT32	iMyPosValue, iHisPosValue, iCoverValue;
	INT32	iReductionFactor, iThisScale;
	INT32	sHisGridNo, sMyRealGridNo = NOWHERE, sHisRealGridNo = NOWHERE;
	INT16 sTempX, sTempY;
	FLOAT dMyX, dMyY, dHisX, dHisY;
	INT8	bHisBestCTGT, bHisActualCTGT, bHisCTGT, bMyCTGT;
	INT32	iRangeChange, iRangeFactor, iRangeFactorMultiplier;
	SOLDIERTYPE *pHim;

	dMyX = dMyY = dHisX = dHisY = -1.0;

	pHim = Threat[uiThreatIndex].pOpponent;
	sHisGridNo = Threat[uiThreatIndex].sGridNo;

	// THE FOLLOWING STUFF IS *VEERRRY SCAARRRY*, BUT SHOULD WORK.	IF YOU REALLY
	// HATE IT, THEN CHANGE ChanceToGetThrough() TO WORK FROM A GRIDNO TO GRIDNO

	// if this is theoretical, and I'm not actually at sMyGridNo right now
	if (pMe->sGridNo != sMyGridNo)
	{
		sMyRealGridNo = pMe->sGridNo;		// remember where I REALLY am
		dMyX = pMe->dXPos;
		dMyY = pMe->dYPos;

		pMe->sGridNo = sMyGridNo;				// but pretend I'm standing at sMyGridNo
		ConvertGridNoToCenterCellXY( sMyGridNo, &sTempX, &sTempY );
		pMe->dXPos = (FLOAT) sTempX;
		pMe->dYPos = (FLOAT) sTempY;
	}

	// if this is theoretical, and he's not actually at hisGrid right now
	if (pHim->sGridNo != sHisGridNo)
	{
		sHisRealGridNo = pHim->sGridNo;		// remember where he REALLY is
		dHisX = pHim->dXPos;
		dHisY = pHim->dYPos;

		pHim->sGridNo = sHisGridNo;			// but pretend he's standing at sHisGridNo
		ConvertGridNoToCenterCellXY( sHisGridNo, &sTempX, &sTempY );
		pHim->dXPos = (FLOAT) sTempX;
		pHim->dYPos = (FLOAT) sTempY;
	}


	if (InWaterOrGas(pHim,sHisGridNo))
	{
		bHisActualCTGT = 0;
	}
	else
	{
		// optimistically assume we'll be behind the best cover available at this spot

		//bHisActualCTGT = ChanceToGetThrough(pHim,sMyGridNo,FAKE,ACTUAL,TESTWALLS,9999,M9PISTOL,NOT_FOR_LOS); // assume a gunshot
		bHisActualCTGT = CalcWorstCTGTForPosition( pHim, pMe->ubID, sMyGridNo, pMe->pathing.bLevel, iMyAPsLeft );
	}

	// normally, that will be the cover I'll use, unless worst case over-rides it
	bHisCTGT = bHisActualCTGT;

	// only calculate his best case CTGT if there is room for improvement!
	if (bHisActualCTGT < 100)
	{
		// if we didn't remember his real gridno earlier up above, we got to now,
		// because calculating worst case is about to play with it in a big way!		
		if (TileIsOutOfBounds(sHisRealGridNo))
		{
			sHisRealGridNo = pHim->sGridNo;		// remember where he REALLY is
			dHisX = pHim->dXPos;
			dHisY = pHim->dYPos;
		}

		// calculate where my cover is worst if opponent moves just 1 tile over
		bHisBestCTGT = CalcBestCTGT(pHim, pMe->ubID, sMyGridNo, pMe->pathing.bLevel, iMyAPsLeft);

		// if he can actually improve his CTGT by moving to a nearby gridno
		if (bHisBestCTGT > bHisActualCTGT)
		{
			// he may not take advantage of his best case, so take only 2/3 of best
			bHisCTGT = ((2 * bHisBestCTGT) + bHisActualCTGT) / 3;
		}
	}

	// if my intended gridno is in water or gas, I can't attack at all from there
	// here, for smoke, consider bad
	if (InWaterGasOrSmoke(pMe,sMyGridNo))
	{
		bMyCTGT = 0;
	}
	else
	{
		// put him at sHisGridNo if necessary!
		if (pHim->sGridNo != sHisGridNo )
		{
			pHim->sGridNo = sHisGridNo;
			ConvertGridNoToCenterCellXY( sHisGridNo, &sTempX, &sTempY );
			pHim->dXPos = (FLOAT) sTempX;
			pHim->dYPos = (FLOAT) sTempY;
		}
		// bMyCTGT = ChanceToGetThrough(pMe,sHisGridNo,FAKE,ACTUAL,TESTWALLS,9999,M9PISTOL,NOT_FOR_LOS); // assume a gunshot
		// bMyCTGT = SoldierToLocationChanceToGetThrough( pMe, sHisGridNo, pMe->bTargetLevel, pMe->bTargetCubeLevel );

		// let's not assume anything about the stance the enemy might take, so take an average
		// value... no cover give a higher value than partial cover
		bMyCTGT = CalcAverageCTGTForPosition( pMe, pHim->ubID, sHisGridNo, pHim->pathing.bLevel, iMyAPsLeft );

		// since NPCs are too dumb to shoot "blind", ie. at opponents that they
		// themselves can't see (mercs can, using another as a spotter!), if the
		// cover is below the "see_thru" threshold, it's equivalent to perfect cover!
		if (bMyCTGT < SEE_THRU_COVER_THRESHOLD)
		{
			bMyCTGT = 0;
		}
	}

	// UNDO ANY TEMPORARY "DAMAGE" DONE ABOVE	
	if (!TileIsOutOfBounds(sMyRealGridNo))
	{
		pMe->sGridNo = sMyRealGridNo;		// put me back where I belong!
		pMe->dXPos = dMyX;						// also change the 'x'
		pMe->dYPos = dMyY;						// and the 'y'
	}
	
	if (!TileIsOutOfBounds(sHisRealGridNo))
	{
		pHim->sGridNo = sHisRealGridNo;		// put HIM back where HE belongs!
		pHim->dXPos = dHisX;					// also change the 'x'
		pHim->dYPos = dHisY;					// and the 'y'
	}


	// these value should be < 1 million each
	iHisPosValue = bHisCTGT * Threat[uiThreatIndex].iValue * Threat[uiThreatIndex].iAPs;
	iMyPosValue =	bMyCTGT *	iMyThreat * iMyAPsLeft;


	// try to account for who outnumbers who: the side with the advantage thus
	// (hopefully) values offense more, while those in trouble will play defense
	if (pHim->aiData.bOppCnt > 1)
	{
		iHisPosValue /= pHim->aiData.bOppCnt;
	}

	if (pMe->aiData.bOppCnt > 1)
	{
		iMyPosValue /= pMe->aiData.bOppCnt;
	}


 // if my positional value is worth something at all here
 if (iMyPosValue > 0)
	{
	// if I CAN'T crouch when I get there, that makes it significantly less
	// appealing a spot (how much depends on range), so that's a penalty to me
	if (iMyAPsLeft < GetAPsCrouch(pMe,TRUE))
	 // subtract another 1 % penalty for NOT being able to crouch per tile
	 // the farther away we are, the bigger a difference crouching will make!
	 iMyPosValue -= ((iMyPosValue * (AIM_PENALTY_TARGET_CROUCHED + (iRange / CELL_X_SIZE))) / 100);
	}


	// high morale prefers decreasing the range (positive factor), while very
	// low morale (HOPELESS) prefers increasing it

//	if (bHisCTGT < 100 || (morale - 1 < 0))
	{

		iRangeFactorMultiplier = RangeChangeDesire( pMe );

		if (iRangeFactorMultiplier)
		{
			iRangeChange = Threat[uiThreatIndex].iOrigRange - iRange;

			if (iRangeChange)
			{
				//iRangeFactor = (iRangeChange * (morale - 1)) / 4;
				iRangeFactor = (iRangeChange * iRangeFactorMultiplier) / 2;

	#ifdef DEBUGCOVER
				DebugAI( String( "CalcCoverValue: iRangeChange %d, iRangeFactor %d\n", iRangeChange, iRangeFactor ) );
	#endif

				// aggression booster for stupider enemies
				iMyPosValue += 100 * iRangeFactor * ( 5 - SoldierDifficultyLevel( pMe ) ) / 5 ;

				// if factor is positive increase positional value, else decrease it
				// change both values, since one or the other could be 0
				if (iRangeFactor > 0)
				{

					iMyPosValue = (iMyPosValue * (100 + iRangeFactor)) / 100;
					iHisPosValue = (100 * iHisPosValue) / (100 + iRangeFactor);
				}
				else if (iRangeFactor < 0)
				{
					iMyPosValue = (100 * iMyPosValue) / (100 - iRangeFactor);
					iHisPosValue = (iHisPosValue * (100 - iRangeFactor)) / 100;
				}
			}
		}
	}

	// the farther apart we are, the less important the cover differences are
	// the less certain his position, the less important cover differences are
	iReductionFactor = ((MAX_THREAT_RANGE - iRange) * Threat[uiThreatIndex].iCertainty) /
		 MAX_THREAT_RANGE;

	// divide by a 100 to make the numbers more managable and avoid 32-bit limit
	iThisScale = max( iMyPosValue, iHisPosValue) / 100;
	iThisScale = (iThisScale * iReductionFactor) / 100;
	*iTotalScale += iThisScale;
	// this helps to decide the percent improvement later

	// POSITIVE COVER VALUE INDICATES THE COVER BENEFITS ME, NEGATIVE RESULT
	// MEANS IT BENEFITS THE OTHER GUY.
	// divide by a 100 to make the numbers more managable and avoid 32-bit limit
	iCoverValue = (iMyPosValue - iHisPosValue) / 100;
	iCoverValue = (iCoverValue * iReductionFactor) / 100;

#ifdef DEBUGCOVER
	DebugAI( String( "CalcCoverValue: iCoverValue %d, sMyGridNo %d, sHisGrid %d, iRange %d, morale %d\n",iCoverValue,sMyGridNo,sHisGridNo,iRange,morale) );
	DebugAI( String( "CalcCoverValue: iCertainty %d, his bOppCnt %d, my bOppCnt %d\n",Threat[uiThreatIndex].iCertainty,pHim->aiData.bOppCnt,pMe->aiData.bOppCnt) );
	DebugAI( String( "CalcCoverValue: bHisCTGT = %d, hisThreat = %d, hisFullAPs = %d\n",bHisCTGT,Threat[uiThreatIndex].iValue,Threat[uiThreatIndex].iAPs) );
	DebugAI( String( "CalcCoverValue: bMyCTGT = %d,	iMyThreat = %d,	iMyAPsLeft = %d\n", bMyCTGT, iMyThreat,iMyAPsLeft) );
	DebugAI( String( "CalcCoverValue: hisPosValue = %d, myPosValue = %d\n",iHisPosValue,iMyPosValue) );
	DebugAI( String( "CalcCoverValue: iThisScale = %d, iTotalScale = %d, iReductionFactor %d\n\n",iThisScale,*iTotalScale, iReductionFactor) );
#endif

	return( iCoverValue );
}


UINT8 NumberOfTeamMatesAdjacent( SOLDIERTYPE * pSoldier, INT32 sGridNo )
{
	UINT8	ubLoop, ubCount, ubWhoIsThere;
	INT32	sTempGridNo;

	ubCount = 0;

	for( ubLoop = 0; ubLoop < NUM_WORLD_DIRECTIONS; ubLoop++ )
	{
		sTempGridNo = NewGridNo( sGridNo, DirectionInc( ubLoop ) );
		if ( sTempGridNo != sGridNo )
		{
			ubWhoIsThere = WhoIsThere2( sGridNo, pSoldier->pathing.bLevel );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != pSoldier->ubID && MercPtrs[ ubWhoIsThere ]->bTeam == pSoldier->bTeam )
			{
				ubCount++;
			}
		}
	}

	return( ubCount );
}

INT32 FindBestNearbyCover(SOLDIERTYPE *pSoldier, INT32 morale, INT32 *piPercentBetter)
{
	DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("FindBestNearbyCover"));

	// all 32-bit integers for max. speed
	UINT32 uiLoop;
	INT32 iCurrentCoverValue, iCoverValue, iBestCoverValue;
	INT32	iCurrentScale = -1, iCoverScale = -1, iBestCoverScale = -1;
	INT32	iDistFromOrigin, iDistCoverFromOrigin, iThreatCertainty;
	INT32 sGridNo, sBestCover = NOWHERE;
	INT32 iPathCost;
	INT32	iThreatRange, iClosestThreatRange = 1500;
//	INT16 sClosestThreatGridno = NOWHERE;
	INT32	iMyThreatValue;
	INT32	sThreatLoc;
	INT32 iMaxThreatRange;
	UINT32	uiThreatCnt = 0;
	INT32 iMaxMoveTilesLeft, iSearchRange, iRoamRange;
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32	sOrigin;	// has to be a short, need a pointer
	INT32	*		pusLastLoc;
	INT8 *		pbPersOL;
	INT8 *		pbPublOL;
	SOLDIERTYPE *pOpponent;
	UINT16 usMovementMode;

	UINT8	ubBackgroundLightLevel;
	UINT8	ubBackgroundLightPercent = 0;
	UINT8	ubLightPercentDifference;
	BOOLEAN		fNight;

	// There's no cover when boxing!
	if (gTacticalStatus.bBoxingState == BOXING)
	{
		return (NOWHERE);
	}

	if ( gbWorldSectorZ > 0 )
	{
		fNight = FALSE;
	}
	else
	{
		ubBackgroundLightLevel = GetTimeOfDayAmbientLightLevel();

		if ( ubBackgroundLightLevel < NORMAL_LIGHTLEVEL_DAY + 2 )
		{
			fNight = FALSE;
		}
		else
		{
			fNight = TRUE;
			ubBackgroundLightPercent = gGameExternalOptions.ubBrightnessVisionMod[ ubBackgroundLightLevel ];
			//ubBackgroundLightPercent = gbLightSighting[ 0 ][ ubBackgroundLightLevel ];
		}
	}


	iBestCoverValue = -1;

#if defined( _DEBUG ) && defined( COVER_DEBUG )
	if (gfDisplayCoverValues)
	{
		memset( gsCoverValue, 0x7F, sizeof( INT16 ) * WORLD_MAX );
	}
#endif

	//NameMessage(pSoldier,"looking for some cover...");

	// BUILD A LIST OF THREATENING GRID #s FROM PERSONAL & PUBLIC opplists

	pusLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

	// hang a pointer into personal opplist
	pbPersOL = &(pSoldier->aiData.bOppList[0]);
	// hang a pointer into public opplist
	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);

	// decide how far we're gonna be looking
	iSearchRange = gbDiff[DIFF_MAX_COVER_RANGE][ SoldierDifficultyLevel( pSoldier ) ];

/*
	switch (pSoldier->aiData.bAttitude)
	{
		case DEFENSIVE:		iSearchRange += 2; break;
		case BRAVESOLO:		iSearchRange -= 4; break;
		case BRAVEAID:		iSearchRange -= 4; break;
		case CUNNINGSOLO:	iSearchRange += 4; break;
		case CUNNINGAID:	iSearchRange += 4; break;
		case AGGRESSIVE:	iSearchRange -= 2; break;
	}*/


	// maximum search range is 1 tile / 8 pts of wisdom
	if (iSearchRange > (pSoldier->stats.bWisdom / 8))
	{
		iSearchRange = (pSoldier->stats.bWisdom / 8);
	}

	if (!gfTurnBasedAI)
	{
		// don't search so far in realtime
		iSearchRange /= 2;
	}

	usMovementMode = DetermineMovementMode( pSoldier, AI_ACTION_TAKE_COVER );

	if (pSoldier->aiData.bAlertStatus >= STATUS_RED)			// if already in battle
	{
		// must be able to reach the cover, so it can't possibly be more than
		// action points left (rounded down) tiles away, since minimum
		// cost to move per tile is 1 points.
		// HEADROCK HAM 3.6: This doesn't take into account the 100AP system. Adjusting.
		// Please note, I used a calculation that may have a better representation in some global variable.
		//iMaxMoveTilesLeft = __max( 0, pSoldier->bActionPoints - MinAPsToStartMovement( pSoldier, usMovementMode ) );
		// WarmSteel - Bugfix:  wrong parentheses
		iMaxMoveTilesLeft = __max( 0, (pSoldier->bActionPoints - MinAPsToStartMovement( pSoldier, usMovementMode )) / (APBPConstants[AP_MAXIMUM] / 25) );

		//NumMessage("In BLACK, maximum tiles to move left = ",maxMoveTilesLeft);

		// if we can't go as far as the usual full search range
		if (iMaxMoveTilesLeft < iSearchRange)
		{
			// then limit the search range to only as far as we CAN go
			iSearchRange = iMaxMoveTilesLeft;
		}
	}

	if (iSearchRange <= 0)
	{
		return(NOWHERE);
	}

	// those within 20 tiles of any tile we'll CONSIDER as cover are important
	iMaxThreatRange = MAX_THREAT_RANGE + (CELL_X_SIZE * iSearchRange);

	// calculate OUR OWN general threat value (not from any specific location)
	iMyThreatValue = CalcManThreatValue(pSoldier,NOWHERE,FALSE,pSoldier);

	// look through all opponents for those we know of
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, dead, unconscious
		if (!pOpponent || pOpponent->stats.bLife < OKLIFE)
		{
			continue;			// next merc
		}

		// if this man is neutral / on the same side, he's not an opponent
 		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
		{
			continue;			// next merc
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpponent->ubID;
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpponent->ubID;
		pusLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpponent->ubID;

		// if this opponent is unknown personally and publicly
		if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN))
		{
			continue;			// next merc
		}

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

		// if personal knowledge is more up to date or at least equal
		if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
			(*pbPersOL == *pbPublOL))
		{
			// using personal knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = *pusLastLoc;
			iThreatCertainty = ThreatPercent[*pbPersOL - OLDEST_HEARD_VALUE];
		}
		else
		{
			// using public knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpponent->ubID];
			iThreatCertainty = ThreatPercent[*pbPublOL - OLDEST_HEARD_VALUE];
		}

		// calculate how far away this threat is (in adjusted pixels)
		//iThreatRange = AdjPixelsAway(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sThreatLoc),CenterY(sThreatLoc));
		iThreatRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sThreatLoc );

		//NumMessage("Threat Range = ",iThreatRange);

#ifdef DEBUGCOVER
//		DebugAI( String( "FBNC: Opponent %d believed to be at gridno %d (mine %d, public %d)\n",iLoop,sThreatLoc,*pusLastLoc,PublicLastKnownOppLoc[pSoldier->bTeam][iLoop] ) );
#endif

		// if this opponent is believed to be too far away to really be a threat
		if (iThreatRange > iMaxThreatRange)
		{
#ifdef DEBUGCOVER
//			AINameMessage(pOpponent,"is too far away to be a threat",1000);
#endif

			continue;			// check next opponent
		}

		// remember this opponent as a current threat, but DON'T REDUCE FOR COVER!
		Threat[uiThreatCnt].iValue = CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier);

		// if the opponent is no threat at all for some reason
		if (Threat[uiThreatCnt].iValue == -999)
		{
			//NameMessage(pOpponent,"is thought to be no threat");
			continue;			// check next opponent
		}

		//NameMessage(pOpponent,"added to the list of threats");
		//NumMessage("His/Her threat value = ",threatValue[uiThreatCnt]);

		Threat[uiThreatCnt].pOpponent		= pOpponent;
		Threat[uiThreatCnt].sGridNo			= sThreatLoc;
		Threat[uiThreatCnt].iCertainty	= iThreatCertainty;
		Threat[uiThreatCnt].iOrigRange	= iThreatRange;

		// calculate how many APs he will have at the start of the next turn
		Threat[uiThreatCnt].iAPs = pOpponent->CalcActionPoints();

		if (iThreatRange < iClosestThreatRange)
		{
			iClosestThreatRange = iThreatRange;
			//NumMessage("New Closest Threat Range = ",iClosestThreatRange);
//			sClosestThreatGridNo = sThreatLoc;
			//NumMessage("Closest Threat Gridno = ",sClosestThreatGridNo);
		}

		uiThreatCnt++;
	}

	// if no known opponents were found to threaten us, can't worry about cover
	if (!uiThreatCnt)
	{
		//NameMessage(pSoldier,"has no threats - WON'T take cover");
		return(sBestCover);
	}

	// calculate our current cover value in the place we are now, since the
	// cover we are searching for must be better than what we have now!
	iCurrentCoverValue = 0;
	iCurrentScale = 0;

	// for every opponent that threatens, consider this spot's cover vs. him
	for (uiLoop = 0; uiLoop < uiThreatCnt; uiLoop++)
	{
		// if this threat is CURRENTLY within 20 tiles
		if (Threat[uiLoop].iOrigRange <= MAX_THREAT_RANGE)
		{
			// add this opponent's cover value to our current total cover value
			iCurrentCoverValue += CalcCoverValue(pSoldier,pSoldier->sGridNo,iMyThreatValue,pSoldier->bActionPoints,uiLoop,Threat[uiLoop].iOrigRange,morale,&iCurrentScale);
		}
		//sprintf(tempstr,"iCurrentCoverValue after opponent %d is now %d",iLoop,iCurrentCoverValue);
		//PopMessage(tempstr);
	}

	// reduce cover for each person adjacent to this gridno who is on our team,
	// by 10% (so locations next to several people will be very much frowned upon
	if ( iCurrentCoverValue >= 0 )
	{
		iCurrentCoverValue -= (iCurrentCoverValue / 10) * NumberOfTeamMatesAdjacent( pSoldier, pSoldier->sGridNo );
	}
	else
	{
		// when negative, must add a negative to decrease the total
		iCurrentCoverValue += (iCurrentCoverValue / 10) * NumberOfTeamMatesAdjacent( pSoldier, pSoldier->sGridNo );
	}

#ifdef DEBUGCOVER
//	AINumMessage("Search Range = ",iSearchRange);
#endif

	// determine maximum horizontal limits
	sMaxLeft  = min(iSearchRange,(pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min(iSearchRange,MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min(iSearchRange,(pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min(iSearchRange,MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
	//NumMessage("sMaxDown = ",sMaxDown);

	iRoamRange = RoamingRange(pSoldier,&sOrigin);

	// if status isn't black (life & death combat), and roaming range is limited	
	if ((pSoldier->aiData.bAlertStatus != STATUS_BLACK) && (iRoamRange < MAX_ROAMING_RANGE) &&
		(!TileIsOutOfBounds(sOrigin)))
	{
		// must try to stay within or return to the point of origin
		iDistFromOrigin = SpacesAway(sOrigin,pSoldier->sGridNo);
	}
	else
	{
		// don't care how far from origin we go
		iDistFromOrigin = -1;
	}


#ifdef DEBUGCOVER
	DebugAI( String( "FBNC: iRoamRange %d, sMaxLeft %d, sMaxRight %d, sMaxUp %d, sMaxDown %d\n",iRoamRange,sMaxLeft,sMaxRight,sMaxUp,sMaxDown) );
#endif

	// the initial cover value to beat is our current cover value
	iBestCoverValue = iCurrentCoverValue;

#ifdef DEBUGDECISIONS
	STR tempstr="";
	sprintf( tempstr, "FBNC: CURRENT iCoverValue = %d\n",iCurrentCoverValue );
	DebugAI( tempstr );
#endif

	if (pSoldier->aiData.bAlertStatus >= STATUS_RED)			// if already in battle
	{
		// to speed this up, tell PathAI to cancel any paths beyond our AP reach!
		gubNPCAPBudget = pSoldier->bActionPoints;
	}
	else
	{
		// even if not under pressure, limit to 1 turn's travelling distance
		// hope this isn't too expensive...
		gubNPCAPBudget = pSoldier->CalcActionPoints( );
		//gubNPCAPBudget = pSoldier->bInitialAPs;
	}

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = (UINT8) iSearchRange;
	gusNPCMovementMode = usMovementMode;

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}
			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, AI_ACTION_TAKE_COVER ), COPYREACHABLE_AND_APS, 0 );//dnl ch50 071009

	// Turn off the "reachable" flag for his current location
	// so we don't consider it
	gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);

	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			//HandleMyMouseCursor(KEYBOARDALSO);

			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			//NumMessage("Testing gridno #",sGridNo);

			// if we are limited to staying/returning near our place of origin
			if (iDistFromOrigin != -1)
			{
				iDistCoverFromOrigin = SpacesAway(sOrigin,sGridNo);

				// if this is outside roaming range, and doesn't get us closer to it
				if ((iDistCoverFromOrigin > iRoamRange) &&
					(iDistFromOrigin <= iDistCoverFromOrigin))
				{
					continue;	// then we can't go there
				}
		}

/*
			if (Net.pnum != Net.turnActive)
			{
				KeepInterfaceGoing(1);
			}
*/
			if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
			{
				continue;
			}

			if ( InGas( pSoldier, sGridNo ) )
			{
				continue;
			}

			// ignore blacklisted spot
			if ( sGridNo == pSoldier->pathing.sBlackList )
			{
				continue;
			}

			iPathCost = gubAIPathCosts[AI_PATHCOST_RADIUS + sXOffset][AI_PATHCOST_RADIUS + sYOffset];
			/*
			// water is OK, if the only good hiding place requires us to get wet, OK
			iPathCost = LegalNPCDestination(pSoldier,sGridNo,ENSURE_PATH_COST,WATEROK);

			if (!iPathCost)
			{
				continue;		// skip on to the next potential grid
			}

			// CJC: This should be a redundent check because the path code is given an
			// AP limit to begin with!
			if (pSoldier->aiData.bAlertStatus == STATUS_BLACK)		// in battle
			{
				// must be able to afford the APs to get to this cover this turn
				if (iPathCost > pSoldier->bActionPoints)
				{
					//NumMessage("In BLACK, and can't afford to get there, cost = ",iPathCost);
					continue;		// skip on to the next potential grid
				}
			}
			*/

			// OK, this place shows potential.	How useful is it as cover?
			// EVALUATE EACH GRID #, remembering the BEST PROTECTED ONE
			iCoverValue = 0;
			iCoverScale = 0;

			// for every opponent that threatens, consider this spot's cover vs. him
			for (uiLoop = 0; uiLoop < uiThreatCnt; uiLoop++)
			{
				// calculate the range we would be at from this opponent
				iThreatRange = GetRangeInCellCoordsFromGridNoDiff( sGridNo, Threat[uiLoop].sGridNo );
				// if this threat would be within 20 tiles, count it
				if (iThreatRange <= MAX_THREAT_RANGE)
				{
					iCoverValue += CalcCoverValue(pSoldier,sGridNo,iMyThreatValue,
						(pSoldier->bActionPoints - iPathCost),
						uiLoop,iThreatRange,morale,&iCoverScale);
				}

				//sprintf(tempstr,"iCoverValue after opponent %d is now %d",iLoop,iCoverValue);
				//PopMessage(tempstr);
			}

			// reduce cover for each person adjacent to this gridno who is on our team,
			// by 10% (so locations next to several people will be very much frowned upon
			if ( iCoverValue >= 0 )
			{
				iCoverValue -= (iCoverValue / 10) * NumberOfTeamMatesAdjacent( pSoldier, sGridNo );
			}
			else
			{
				// when negative, must add a negative to decrease the total
				iCoverValue += (iCoverValue / 10) * NumberOfTeamMatesAdjacent( pSoldier, sGridNo );
			}

			if ( fNight && !( InARoom( sGridNo, NULL ) ) ) // ignore in buildings in case placed there
			{
				// reduce cover at nighttime based on how bright the light is at that location
				// using the difference in sighting distance between the background and the
				// light for this tile
				//ubLightPercentDifference = (gbLightSighting[ 0 ][ LightTrueLevel( sGridNo, pSoldier->pathing.bLevel ) ] - ubBackgroundLightPercent );
				ubLightPercentDifference = (gGameExternalOptions.ubBrightnessVisionMod[ LightTrueLevel( sGridNo, pSoldier->pathing.bLevel ) ] - ubBackgroundLightPercent );
				
				if ( iCoverValue >= 0 )
				{
					iCoverValue -= (iCoverValue / 100) * ubLightPercentDifference;
				}
				else
				{
					iCoverValue += (iCoverValue / 100) * ubLightPercentDifference;
				}
			}


#ifdef DEBUGCOVER
			// if there ARE multiple opponents
			if (uiThreatCnt > 1)
			{
				DebugAI( String( "FBNC: Total iCoverValue at gridno %d is %d\n\n",sGridNo,iCoverValue ) );
			}
#endif

#if defined( _DEBUG ) && defined( COVER_DEBUG )
			if (gfDisplayCoverValues)
			{
				gsCoverValue[sGridNo] = (INT16) (iCoverValue / 100);
			}
#endif

			// if this is better than the best place found so far

			if (iCoverValue > iBestCoverValue)
			{
				// ONLY DO THIS CHECK HERE IF WE'RE WAITING FOR OPPCHANCETODECIDE,
				// OTHERWISE IT WOULD USUALLY BE A WASTE OF TIME
				// ok to comment out for now?
				/*
				if (Status.team[Net.turnActive].allowOppChanceToDecide)
				{
					// if this cover value qualifies as "better" enough to get used
					if (CalcPercentBetter( iCurrentCoverValue,iCoverValue,iCurrentScale,iCoverScale) >= MIN_PERCENT_BETTER)
					{
						// then we WILL do something (take this cover, at least)
						NPCDoesAct(pSoldier);
					}
				}
				*/

#ifdef DEBUGDECISIONS
				STR tempstr;
				sprintf( tempstr,"FBNC: NEW BEST iCoverValue at gridno %d is %d\n",sGridNo,iCoverValue );
				DebugAI( tempstr );
#endif
				// remember it instead
				sBestCover = sGridNo;
				iBestCoverValue = iCoverValue;
				iBestCoverScale = iCoverScale;
			}
		}
	}

	gubNPCAPBudget = 0;
	gubNPCDistLimit = 0;

	#if defined( _DEBUG ) && !defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues)
	{
		// do a locate?
		LocateSoldier( pSoldier->ubID, SETLOCATORFAST );
		gsBestCover = sBestCover;
		SetRenderFlags( RENDER_FLAG_FULL );
		RenderWorld();
		RenderCoverDebug( );
		InvalidateScreen( );
		EndFrameBufferRender();
		RefreshScreen( NULL );
		/*
	iLoop = GetJA2Clock();
	do
	{

	} while( ( GetJA2Clock( ) - iLoop ) < 2000 );
	*/
	}
	#endif

	// if a better cover location was found	
	if (!TileIsOutOfBounds(sBestCover))
	{
		#if defined( _DEBUG ) && !defined( PATHAI_VISIBLE_DEBUG )
		gsBestCover = sBestCover;
		#endif
		// cover values already take the AP cost of getting there into account in
		// a BIG way, so no need to worry about that here, even small improvements
		// are actually very significant once we get our APs back (if we live!)
		*piPercentBetter = CalcPercentBetter(iCurrentCoverValue,iBestCoverValue,iCurrentScale,iBestCoverScale);

		// if best cover value found was at least 5% better than our current cover
		if (*piPercentBetter >= MIN_PERCENT_BETTER)
		{
#ifdef DEBUGDECISIONS
			STR tempstr;
			sprintf( tempstr,"Found Cover: current %ld, best %ld, %%%%Better %ld\n", iCurrentCoverValue,iBestCoverValue,*piPercentBetter  );
			DebugAI( tempstr );
#endif

#ifdef BETAVERSION
			SnuggleDebug(pSoldier,"Found Cover");
#endif

			return(sBestCover);       // return the gridno of that cover
		}
	}
	return(NOWHERE);		// return that no suitable cover was found
}

INT32 FindSpotMaxDistFromOpponents(SOLDIERTYPE *pSoldier)
{
	INT32 sGridNo;
	INT32 sBestSpot = NOWHERE;
	UINT32 uiLoop;
	INT32 iThreatRange,iClosestThreatRange = 1500, iSpotClosestThreatRange;
	INT32 sThreatLoc, sThreatGridNo[MAXMERCS];
	UINT32 uiThreatCnt = 0;
	INT32 iSearchRange;
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT8 * pbPersOL,*pbPublOL, bEscapeDirection, bBestEscapeDirection = -1;
	SOLDIERTYPE *pOpponent;
	INT32	sOrigin;
	INT32	iRoamRange;

	// BUILD A LIST OF THREATENING GRID #s FROM PERSONAL & PUBLIC opplistS

	// look through all opponents for those we know of
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, dead, unconscious
		if (!pOpponent || (pOpponent->stats.bLife < OKLIFE))
		{
			continue;			// next merc
		}

		// if this man is neutral / on the same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
		{
			continue;			// next merc
		}

		pbPersOL = &(pSoldier->aiData.bOppList[pOpponent->ubID]);
		pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][pOpponent->ubID]);

		// if this opponent is unknown personally and publicly
		if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN))
		{
			continue;			// check next opponent
		}

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

		// if the opponent is no threat at all for some reason
		if (CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier) == -999)
		{
			continue;			// check next opponent
		}

		// if personal knowledge is more up to date or at least equal
		if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
		(*pbPersOL == *pbPublOL))
		{
			// using personal knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = gsLastKnownOppLoc[pSoldier->ubID][pOpponent->ubID];
		}
		else
		{
			// using public knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpponent->ubID];
		}

		// calculate how far away this threat is (in adjusted pixels)
		iThreatRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sThreatLoc );

		if (iThreatRange < iClosestThreatRange)
		{
			iClosestThreatRange = iThreatRange;
			//NumMessage("New Closest Threat Range = ",iClosestThreatRange);
		}

		// remember this threat's gridno
		sThreatGridNo[uiThreatCnt] = sThreatLoc;
		uiThreatCnt++;
	}

	// if no known opponents were found to threaten us, can't worry about them
	if (!uiThreatCnt)
	{
		//NameMessage(pSoldier,"has no known threats - WON'T run away");
		return( sBestSpot );
	}

	// get roaming range here; for civilians, running away is limited by roam range
	if ( pSoldier->bTeam == CIV_TEAM )
	{
		iRoamRange = RoamingRange( pSoldier, &sOrigin );
		if ( iRoamRange == 0 )
		{
			return( sBestSpot );
		}
	}
	else
	{
		// dummy values
		iRoamRange = 100;
		sOrigin = pSoldier->sGridNo;
	}

	// DETERMINE CO-ORDINATE LIMITS OF SQUARE AREA TO BE CHECKED
	// THIS IS A LOT QUICKER THAN COVER, SO DO A LARGER AREA, NOT AFFECTED BY
	// DIFFICULTY SETTINGS...

	if (pSoldier->aiData.bAlertStatus == STATUS_BLACK)			// if already in battle
	{
		iSearchRange = pSoldier->bActionPoints / 2;

		// to speed this up, tell PathAI to cancel any paths beyond our AP reach!
		gubNPCAPBudget = pSoldier->bActionPoints;
	}
	else
	{
		// even if not under pressure, limit to 1 turn's travelling distance
		gubNPCAPBudget = __min( pSoldier->bActionPoints / 2, pSoldier->CalcActionPoints( ) );

		iSearchRange = gubNPCAPBudget / 2;
	}

	if (!gfTurnBasedAI)
	{
		// search only half as far in realtime
		// but always allow a certain minimum!

		if ( iSearchRange > 4 )
		{
			iSearchRange /= 2;
			gubNPCAPBudget /= 2;
		}
	}


	// assume we have to stand up!
	// use the min macro here to make sure we don't wrap the UINT8 to 255...
	// Lesh: for some reason this code still allows to wrap UINT8 at low values of gubNPCAPBudget
	//gubNPCAPBudget = 	gubNPCAPBudget = __min( gubNPCAPBudget, gubNPCAPBudget - GetAPsToChangeStance( pSoldier, ANIM_STAND ) );
	// Lesh: will be using this form
	if ( gubNPCAPBudget > GetAPsToChangeStance( pSoldier, ANIM_STAND ) )
		gubNPCAPBudget = gubNPCAPBudget - (UINT8) GetAPsToChangeStance( pSoldier, ANIM_STAND );
	//NumMessage("Search Range = ",iSearchRange);
	//NumMessage("gubNPCAPBudget = ",gubNPCAPBudget);

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
	//NumMessage("sMaxDown = ",sMaxDown);

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = (UINT8) iSearchRange;

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, AI_ACTION_RUN_AWAY ), COPYREACHABLE, 0 );//dnl ch50 121009

	// Turn off the "reachable" flag for his current location
	// so we don't consider it
	gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
	//dnl ch58 170813 also don't use last two locations to avoid looping same decisions per turn
	if(!TileIsOutOfBounds(pSoldier->sLastTwoLocations[0]))
		gpWorldLevelData[pSoldier->sLastTwoLocations[0]].uiFlags &= ~(MAPELEMENT_REACHABLE);
	else if(!TileIsOutOfBounds(pSoldier->sLastTwoLocations[1]))
		gpWorldLevelData[pSoldier->sLastTwoLocations[1]].uiFlags &= ~(MAPELEMENT_REACHABLE);

	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			//NumMessage("Testing gridno #",gridno);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
			{
				continue;
			}

			if ( sGridNo == pSoldier->pathing.sBlackList )
			{
				continue;
			}

			if ( pSoldier->bTeam == CIV_TEAM )
			{
				iRoamRange = RoamingRange( pSoldier, &sOrigin );
				if ( PythSpacesAway( sOrigin, sGridNo ) > iRoamRange )
				{
					continue;
				}
			}

			// exclude locations with tear/mustard gas (at this point, smoke is cool!)
			if ( InGas( pSoldier, sGridNo ) )
			{
				continue;
			}

			//Madd: skip lighted spots
			if ( InLightAtNight( sGridNo, pSoldier->pathing.bLevel ) )
				continue;

			// OK, this place shows potential.	How useful is it as cover?
			//NumMessage("Promising seems gridno #",gridno);

			iSpotClosestThreatRange = 1500;

			if ( pSoldier->bTeam == ENEMY_TEAM && GridNoOnEdgeOfMap( sGridNo, &bEscapeDirection ) && EscapeDirectionIsValid( &bEscapeDirection ) )
			{
				// We can escape!	This is better than anything else except a closer spot which we can
				// cross over from.

				// Subtract the straight-line distance from our location to this one as an estimate of
				// path cost and for looks...

				// The edge spot closest to us which is on the edge will have the highest value, so
				// it will be picked over locations further away.
				// Only reachable gridnos will be picked so this should hopefully look okay
				iSpotClosestThreatRange -= PythSpacesAway( pSoldier->sGridNo, sGridNo );

			}
			else
			{
				bEscapeDirection = -1;
				// for every opponent that threatens, consider this spot's cover vs. him
				for (uiLoop = 0; uiLoop < uiThreatCnt; uiLoop++)
				{
					//iThreatRange = AdjPixelsAway(CenterX(sGridNo),CenterY(sGridNo), CenterX(sThreatGridNo[iLoop]),CenterY(sThreatGridNo[iLoop]));
					iThreatRange = GetRangeInCellCoordsFromGridNoDiff( sGridNo, sThreatGridNo[uiLoop] );
					if (iThreatRange < iSpotClosestThreatRange)
					{
						iSpotClosestThreatRange = iThreatRange;
					}
				}
			}

			// if this is better than the best place found so far
			// (i.e. the closest guy would be farther away than previously)
			if (iSpotClosestThreatRange > iClosestThreatRange)
			{
				// remember it instead
				iClosestThreatRange = iSpotClosestThreatRange;
				//NumMessage("New best range = ",iClosestThreatRange);
				sBestSpot = sGridNo;
				bBestEscapeDirection = bEscapeDirection;
				//NumMessage("New best grid = ",bestSpot);
			}
		}

	}

	gubNPCAPBudget = 0;
	gubNPCDistLimit = 0;

	if (bBestEscapeDirection != -1)
	{
		// Woohoo!	We can escape!	Fake some stuff with the quote-related actions
		pSoldier->ubQuoteActionID = GetTraversalQuoteActionID( bBestEscapeDirection );
	}

	return( sBestSpot );
}

INT32 FindNearestUngassedLand(SOLDIERTYPE *pSoldier)
{
	INT32 sGridNo,sClosestLand = NOWHERE,sPathCost,sShortestPath = 1000;
	INT16 sMaxLeft,sMaxRight,sMaxUp,sMaxDown,sXOffset,sYOffset;
	INT32 iSearchRange;

	//NameMessage(pSoldier,"looking for nearest reachable land");

	// start with a small search area, and expand it if we're unsuccessful
	// this should almost never need to search farther than 5 or 10 squares...
	for (iSearchRange = 5; iSearchRange <= 25; iSearchRange += 5)
	{
		//NumMessage("Trying iSearchRange = ", iSearchRange);

		// determine maximum horizontal limits
		sMaxLeft  = min(iSearchRange,(pSoldier->sGridNo % MAXCOL));
		//NumMessage("sMaxLeft = ",sMaxLeft);
		sMaxRight = min(iSearchRange,MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
		//NumMessage("sMaxRight = ",sMaxRight);

		// determine maximum vertical limits
		sMaxUp   = min(iSearchRange,(pSoldier->sGridNo / MAXROW));
		//NumMessage("sMaxUp = ",sMaxUp);
		sMaxDown = min(iSearchRange,MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
		//NumMessage("sMaxDown = ",sMaxDown);

		// Call FindBestPath to set flags in all locations that we can
		// walk into within range.	We have to set some things up first...

		// set the distance limit of the square region
		gubNPCDistLimit = (UINT8) iSearchRange;

		// reset the "reachable" flags in the region we're looking at
		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
				if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
				{
					continue;
				}

				gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
			}
		}

		FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, AI_ACTION_LEAVE_WATER_GAS ), COPYREACHABLE, 0 );//dnl ch50 071009

		// Turn off the "reachable" flag for his current location
		// so we don't consider it
		gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);

		// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				// calculate the next potential gridno
				sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
				//NumMessage("Testing gridno #",gridno);
				if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
				{
					continue;
				}

				if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
				{
					continue;
				}

				// ignore blacklisted spot
				if ( sGridNo == pSoldier->pathing.sBlackList )
				{
					continue;
				}

				// CJC: here, unfortunately, we must calculate a path so we have an AP cost

				// obviously, we're looking for LAND, so water is out!
				sPathCost = LegalNPCDestination(pSoldier,sGridNo,ENSURE_PATH_COST,NOWATER,0);

				if (!sPathCost)
				{
					continue;		// skip on to the next potential grid
				}

				// if this path is shorter than the one to the closest land found so far
				if (sPathCost < sShortestPath)
				{
					// remember it instead
					sShortestPath = sPathCost;
					//NumMessage("New shortest route = ",shortestPath);

					sClosestLand = sGridNo;
					//NumMessage("New closest land at grid = ",closestLand);
				}
			}
		}

		// if we found a piece of land in this search area		
		if (!TileIsOutOfBounds(sClosestLand))	// quit now, no need to look any farther
			break;
	}

	//NumMessage("closestLand = ",closestLand);
	return(sClosestLand);
}

INT16 FindNearbyDarkerSpot( SOLDIERTYPE *pSoldier )
{
	INT32 sGridNo, sClosestSpot = NOWHERE, sPathCost;
	INT32	iSpotValue, iBestSpotValue = 1000;
	INT16 sMaxLeft,sMaxRight,sMaxUp,sMaxDown,sXOffset,sYOffset;
	INT32 iSearchRange;
	INT8 bLightLevel, bCurrLightLevel, bLightDiff;
	INT32 iRoamRange;
	INT32 sOrigin;

	bCurrLightLevel = LightTrueLevel( pSoldier->sGridNo, pSoldier->pathing.bLevel );

	iRoamRange = RoamingRange( pSoldier, &sOrigin );

	// start with a small search area, and expand it if we're unsuccessful
	// this should almost never need to search farther than 5 or 10 squares...
	for (iSearchRange = 5; iSearchRange <= 15; iSearchRange += 5)
	{
		// determine maximum horizontal limits
		sMaxLeft  = min(iSearchRange,(pSoldier->sGridNo % MAXCOL));
		//NumMessage("sMaxLeft = ",sMaxLeft);
		sMaxRight = min(iSearchRange,MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
		//NumMessage("sMaxRight = ",sMaxRight);

		// determine maximum vertical limits
		sMaxUp   = min(iSearchRange,(pSoldier->sGridNo / MAXROW));
		//NumMessage("sMaxUp = ",sMaxUp);
		sMaxDown = min(iSearchRange,MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
		//NumMessage("sMaxDown = ",sMaxDown);

		// Call FindBestPath to set flags in all locations that we can
		// walk into within range.	We have to set some things up first...

		// set the distance limit of the square region
		gubNPCDistLimit = (UINT8) iSearchRange;

		// reset the "reachable" flags in the region we're looking at
		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
				if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
				{
					continue;
				}

				gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
			}
		}

		FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, AI_ACTION_LEAVE_WATER_GAS ), COPYREACHABLE, 0 );//dnl ch50 071009

		// Turn off the "reachable" flag for his current location
		// so we don't consider it
		gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);

		// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				// calculate the next potential gridno
				sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
				//NumMessage("Testing gridno #",gridno);
				if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
				{
					continue;
				}

				if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
				{
					continue;
				}

				// ignore blacklisted spot
				if ( sGridNo == pSoldier->pathing.sBlackList )
				{
					continue;
				}

				// require this character to stay within their roam range
				if ( PythSpacesAway( sOrigin, sGridNo ) > iRoamRange )
				{
					continue;
				}

				// screen out anything brighter than our current best spot
				bLightLevel = LightTrueLevel( sGridNo, pSoldier->pathing.bLevel );

				//bLightDiff = gbLightSighting[0][ bCurrLightLevel ] - gbLightSighting[0][ bLightLevel ];
				bLightDiff = gGameExternalOptions.ubBrightnessVisionMod[ bCurrLightLevel ] - gGameExternalOptions.ubBrightnessVisionMod[ bLightLevel ];
				// if the spot is darker than our current location, then bLightDiff > 0
				// plus ignore differences of just 1 light level
				if ( bLightDiff <= 1 )
				{
					continue;
				}

				// CJC: here, unfortunately, we must calculate a path so we have an AP cost

				sPathCost = LegalNPCDestination(pSoldier,sGridNo,ENSURE_PATH_COST,NOWATER,0);

				if (!sPathCost)
				{
					continue;		// skip on to the next potential grid
				}

				// decrease the "cost" of the spot by the amount of light/darkness
				iSpotValue = sPathCost * 2 - bLightDiff;

				if ( iSpotValue < iBestSpotValue )
				{
					// remember it instead
					iBestSpotValue = iSpotValue;
					//NumMessage("New shortest route = ",shortestPath);

					sClosestSpot = sGridNo;
					//NumMessage("New closest land at grid = ",closestLand);
				}
			}
		}

		// if we found a piece of land in this search area		
		if (!TileIsOutOfBounds(sClosestSpot))	// quit now, no need to look any farther
		{
			break;
		}
	}

	return(sClosestSpot);
}

#define MINIMUM_REQUIRED_STATUS 70

INT8 SearchForItems( SOLDIERTYPE * pSoldier, INT8 bReason, UINT16 usItem )
{
	DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("SearchForItems"));

	INT32					iSearchRange;
	INT16					sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32 sGridNo;
	INT32					sBestSpot = NOWHERE;
	INT32					iTempValue, iValue, iBestValue = 0;
	ITEM_POOL *				pItemPool;
	OBJECTTYPE *			pObj;
	INVTYPE *				pItem;
	INT32					iItemIndex, iBestItemIndex;
	BOOLEAN					fDumbEnoughtoPickup = FALSE;

	iTempValue = -1;
	iItemIndex = iBestItemIndex = -1;

	// No fair picking up weapons while boxing!
	if (gTacticalStatus.bBoxingState == BOXING)
	{
		return AI_ACTION_NONE;
	}

	if (pSoldier->bActionPoints < GetBasicAPsToPickupItem( pSoldier ))
	{
		return( AI_ACTION_NONE );
	}

	if ( !IS_MERC_BODY_TYPE( pSoldier ) )
	{
		return( AI_ACTION_NONE );
	}

	iSearchRange = gbDiff[DIFF_MAX_COVER_RANGE][ SoldierDifficultyLevel( pSoldier ) ];

	switch (pSoldier->aiData.bAttitude)
	{
		case DEFENSIVE:		iSearchRange --;	break;
		case BRAVESOLO:		iSearchRange += 2; break;
		case BRAVEAID:		iSearchRange += 2; break;
		case CUNNINGSOLO:	iSearchRange -= 2; break;
		case CUNNINGAID:	iSearchRange -= 2; break;
		case AGGRESSIVE:	iSearchRange ++;	break;
	}

	// maximum search range is 1 tile / 10 pts of wisdom
	if (iSearchRange > (pSoldier->stats.bWisdom / 10))
	{
		iSearchRange = (pSoldier->stats.bWisdom / 10);
	}

	if (!gfTurnBasedAI)
	{
		// don't search so far in realtime
		iSearchRange /= 2;
	}

	// don't search so far for items
	iSearchRange /= 2;

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
	//NumMessage("sMaxDown = ",sMaxDown);

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = (UINT8) iSearchRange;

	// set an AP limit too, to our APs less the cost of picking up an item
	// and less the cost of dropping an item since we might need to do that
	gubNPCAPBudget = pSoldier->bActionPoints - GetBasicAPsToPickupItem( pSoldier );

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, AI_ACTION_PICKUP_ITEM ), COPYREACHABLE, 0 );//dnl ch50 071009

	// Flugente: if the soldier is 'dumb enough', he may pick up certain items... which can be used to lure the AI into traps
	if ( pSoldier->stats.bWisdom < 70 )
		fDumbEnoughtoPickup = TRUE;

	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			// exclude locations with tear/mustard gas (at this point, smoke is cool!)
			if ( InGasOrSmoke( pSoldier, sGridNo ) )
			{
				continue;
			}

			if ( (gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_ITEMPOOL_PRESENT)
					&& (gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE) )
			{

				// ignore blacklisted spot
				if ( sGridNo == pSoldier->pathing.sBlackList )
				{
					continue;
				}

				iValue = 0;
				GetItemPool( sGridNo, &pItemPool, pSoldier->pathing.bLevel );
				switch( bReason )
				{
					case SEARCH_AMMO:
						// we are looking for ammo to match the gun in usItem
						while( pItemPool )
						{
							pObj = &(gWorldItems[ pItemPool->iItemIndex ].object);
							pItem = &(Item[pObj->usItem]);
							if ( pItem->usItemClass == IC_GUN && (*pObj)[0]->data.objectStatus >= MINIMUM_REQUIRED_STATUS )
							{
								// maybe this gun has ammo (adjust for whether it is better than ours!)
								if ( (*pObj)[0]->data.gun.bGunAmmoStatus < 0 || (*pObj)[0]->data.gun.ubGunShotsLeft == 0 || (Item[pObj->usItem].fingerprintid && (*pObj)[0]->data.ubImprintID != NOBODY && (*pObj)[0]->data.ubImprintID != pSoldier->ubID) )
								{
									iTempValue = 0;
								}
								else
								{
									iTempValue = (*pObj)[0]->data.gun.ubGunShotsLeft * Weapon[pObj->usItem].ubDeadliness / Weapon[usItem].ubDeadliness;
								}
							}
							else if (ValidAmmoType( usItem, pObj->usItem ) )
							{
								iTempValue = TotalPoints( pObj );
							}
							else
							{
								iTempValue = 0;
							}
							if (iTempValue > iValue )
							{
								iValue = iTempValue;
								iItemIndex = pItemPool->iItemIndex;
							}
							pItemPool = pItemPool->pNext;
						}
						break;
					case SEARCH_WEAPONS:
						while( pItemPool )
						{
							pObj = &(gWorldItems[ pItemPool->iItemIndex ].object);
							pItem = &(Item[pObj->usItem]);
							if (pItem->usItemClass & IC_WEAPON && (*pObj)[0]->data.objectStatus >= MINIMUM_REQUIRED_STATUS )
							{
								if ( (pItem->usItemClass & IC_GUN) && ((*pObj)[0]->data.gun.bGunAmmoStatus < 0 || (*pObj)[0]->data.gun.ubGunShotsLeft == 0 || (( Item[pObj->usItem].fingerprintid ) && (*pObj)[0]->data.ubImprintID != NOBODY && (*pObj)[0]->data.ubImprintID != pSoldier->ubID) ) )
								{
									// jammed or out of ammo, skip it!
									iTempValue = 0;
								}
								else if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass & IC_WEAPON )
								{
									if (Weapon[pObj->usItem].ubDeadliness > Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness)
									{
										iTempValue = 100 * Weapon[pObj->usItem].ubDeadliness / Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness;
									}
									else
									{
										iTempValue = 0;
									}
								}
								else
								{
									iTempValue = 200 + Weapon[pObj->usItem].ubDeadliness;
								}
							}
							else
							{
								iTempValue = 0;
							}
							if (iTempValue > iValue )
							{
								iValue = iTempValue;
								iItemIndex = pItemPool->iItemIndex;
							}
							pItemPool = pItemPool->pNext;
						}
						break;
					default:
						while( pItemPool )
						{
							pObj = &(gWorldItems[ pItemPool->iItemIndex ].object);
							pItem = &(Item[pObj->usItem]);
							if ( pItem->usItemClass & IC_WEAPON && (*pObj)[0]->data.objectStatus >= MINIMUM_REQUIRED_STATUS )
							{
								if ( (pItem->usItemClass & IC_GUN) && ((*pObj)[0]->data.gun.bGunAmmoStatus < 0 || (*pObj)[0]->data.gun.ubGunShotsLeft == 0 || (( Item[pObj->usItem].fingerprintid ) && (*pObj)[0]->data.ubImprintID != NOBODY && (*pObj)[0]->data.ubImprintID != pSoldier->ubID) ) )
								{
									// jammed or out of ammo, skip it!
									iTempValue = 0;
								}
								else if ( (Item[pSoldier->inv[HANDPOS].usItem].usItemClass & IC_WEAPON)	)
								{
									if (Weapon[pObj->usItem].ubDeadliness > Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness)
									{
										if ((Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness != NULL) && (Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness > 0))
										{
											iTempValue = 100 * Weapon[pObj->usItem].ubDeadliness / Weapon[pSoldier->inv[HANDPOS].usItem].ubDeadliness;
										}
										else
										{
											iTempValue = 100 * Weapon[pObj->usItem].ubDeadliness;
										}
									}
									else
									{
										iTempValue = 0;
									}
								}
								else
								{
									iTempValue = 200 + Weapon[pObj->usItem].ubDeadliness;
								}
							}
							else if	(pItem->usItemClass == IC_ARMOUR && (*pObj)[0]->data.objectStatus >= MINIMUM_REQUIRED_STATUS )
							{
								switch( Armour[pItem->ubClassIndex].ubArmourClass )
								{
									case ARMOURCLASS_HELMET:
										if (pSoldier->inv[HELMETPOS].exists() == false)
										{
											iTempValue = 200 + EffectiveArmour( pObj );
										}
										else if ( EffectiveArmour( &(pSoldier->inv[HELMETPOS]) ) > EffectiveArmour( pObj ) )
										{
											iTempValue = 100 * EffectiveArmour( pObj ) / EffectiveArmour( &(pSoldier->inv[HELMETPOS]) );
										}
										else
										{
											iTempValue = 0;
										}
										break;
									case ARMOURCLASS_VEST:
										if (pSoldier->inv[VESTPOS].exists() == false)
										{
											iTempValue = 200 + EffectiveArmour( pObj );
										}
										else if ( EffectiveArmour( &(pSoldier->inv[HELMETPOS]) ) > EffectiveArmour( pObj ) )
										{
											iTempValue = 100 * EffectiveArmour( pObj ) / EffectiveArmour( &(pSoldier->inv[VESTPOS]) );
										}
										else
										{
											iTempValue = 0;
										}
										break;
									case ARMOURCLASS_LEGGINGS:
										if (pSoldier->inv[LEGPOS].exists() == false)
										{
											iTempValue = 200 + EffectiveArmour( pObj );
										}
										else if ( EffectiveArmour( &(pSoldier->inv[HELMETPOS]) ) > EffectiveArmour( pObj ) )
										{
											iTempValue = 100 * EffectiveArmour( pObj ) / EffectiveArmour( &(pSoldier->inv[LEGPOS]) );
										}
										else
										{
											iTempValue = 0;
										}
										break;
									default:
										// WANNE: Fix a vanilla bug: When an enemy soldier is looking for items and finds a non-helmet/vest/leggings piece of armour it was incorrectly considered for pickup.
										// Fixed by Tron (Stracciatella): Revision: 5719
										// break;
										// continue; <- silversurfer: bad idea, this causes the game to hang
										// to make sure that the item isn't considered set iTempValue to zero and get out
										{
											iTempValue = 0;
											break;
										}
								}
							}
							// Flugente: if the soldier is 'dumb enough', he may pick up 'interesting items'. This can be used to lure him into traps (a certain scene in FMJ comes to mind)
							else if ( fDumbEnoughtoPickup && pItem->usItemClass == IC_MISC && HasItemFlag(pObj->usItem, ATTENTION_ITEM) )
							{
								// oooh... shiny!
								iTempValue = 1000;
							}
							else
							{
								iTempValue = 0;
							}

							if (iTempValue > iValue )
							{
								iValue = iTempValue;
								iItemIndex = pItemPool->iItemIndex;
							}
							pItemPool = pItemPool->pNext;
						}
						break;
				}
				iValue = (3 * iValue) / (3 + PythSpacesAway( sGridNo, pSoldier->sGridNo ));
				if (iValue > iBestValue )
				{
					sBestSpot = sGridNo;
					iBestValue = iValue;
					iBestItemIndex = iItemIndex;
				}
			}
		}
	}
	
	if (!TileIsOutOfBounds(sBestSpot))
	{
		DebugAI( String( "%d decides to pick up %S", pSoldier->ubID, ItemNames[ gWorldItems[ iBestItemIndex ].object.usItem ] ) );
		if (Item[gWorldItems[ iBestItemIndex ].object.usItem].usItemClass == IC_GUN)
		{
			//CHRISL: This is the line from ADB's code but I removed it, for now, to match what 0verhaul has been working on
			//if (pSoldier->inv[HANDPOS].exists() == true && PlaceInAnyPocket(pSoldier, &pSoldier->inv[HANDPOS], false) == false)
			if (FindBetterSpotForItem( pSoldier, HANDPOS ) == FALSE)
			{
				if (pSoldier->bActionPoints < GetBasicAPsToPickupItem( pSoldier ) + GetBasicAPsToPickupItem( pSoldier ))
				{
					return( AI_ACTION_NONE );
				}
				if (pSoldier->inv[HANDPOS].fFlags & OBJECT_UNDROPPABLE)
				{
					// destroy this item!
					DebugAI( String( "%d decides he must drop %S first so destroys it", pSoldier->ubID, ItemNames[ pSoldier->inv[HANDPOS].usItem ] ) );
					DeleteObj( &(pSoldier->inv[HANDPOS]) );
					DeductPoints( pSoldier, GetBasicAPsToPickupItem( pSoldier ), 0, AFTERACTION_INTERRUPT );
				}
				else
				{
					// we want to drop this item!
					DebugAI( String( "%d decides he must drop %S first", pSoldier->ubID, ItemNames[ pSoldier->inv[HANDPOS].usItem ] ) );

					pSoldier->aiData.bNextAction = AI_ACTION_PICKUP_ITEM;
					pSoldier->aiData.usNextActionData = sBestSpot;
					pSoldier->iNextActionSpecialData = iBestItemIndex;
					return( AI_ACTION_DROP_ITEM );
				}
			}
		}
		pSoldier->aiData.uiPendingActionData1 = iBestItemIndex;
		pSoldier->aiData.usActionData = sBestSpot;
		return( AI_ACTION_PICKUP_ITEM );
	}

	return( AI_ACTION_NONE );
}

INT32 FindClosestDoor( SOLDIERTYPE * pSoldier )
{
	INT32		sClosestDoor = NOWHERE;
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32 sGridNo;
	INT32		iDist, iClosestDist = 10;

	iSearchRange = 5;

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
	//NumMessage("sMaxDown = ",sMaxDown);
	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if (FindStructure( sGridNo, STRUCTURE_ANYDOOR ) != NULL)
			{
				iDist = PythSpacesAway( pSoldier->sGridNo, sGridNo );
				if (iDist < iClosestDist)
				{
					iClosestDist = iDist;
					sClosestDoor = sGridNo;
				}
			}
		}
	}

	return( sClosestDoor );
}

INT32 FindNearestEdgepointOnSpecifiedEdge( INT32 sGridNo, INT8 bEdgeCode )
{
	INT32			iLoop;
	INT32			*psEdgepointArray;
	INT32			iEdgepointArraySize;
	INT32			sClosestSpot = NOWHERE, sClosestDist = 0x7FFF, sTempDist;

	switch( bEdgeCode )
	{
		case NORTH_EDGEPOINT_SEARCH:
			psEdgepointArray = gps1stNorthEdgepointArray;
			iEdgepointArraySize = gus1stNorthEdgepointArraySize;
			break;
		case EAST_EDGEPOINT_SEARCH:
			psEdgepointArray = gps1stEastEdgepointArray;
			iEdgepointArraySize = gus1stEastEdgepointArraySize;
			break;
		case SOUTH_EDGEPOINT_SEARCH:
			psEdgepointArray = gps1stSouthEdgepointArray;
			iEdgepointArraySize = gus1stSouthEdgepointArraySize;
			break;
		case WEST_EDGEPOINT_SEARCH:
			psEdgepointArray = gps1stWestEdgepointArray;
			iEdgepointArraySize = gus1stWestEdgepointArraySize;
			break;
		default:
			// WTF???
			return( NOWHERE );
	}

	// Do a 2D search to find the closest map edgepoint and
	// try to create a path there

	for ( iLoop = 0; iLoop < iEdgepointArraySize; iLoop++ )
	{
		sTempDist = PythSpacesAway( sGridNo, psEdgepointArray[ iLoop ] );
		if ( sTempDist < sClosestDist )
		{
			sClosestDist = sTempDist;
			sClosestSpot = psEdgepointArray[ iLoop ];
		}
	}

	return( sClosestSpot );
}

INT32 FindNearestEdgePoint( INT32 sGridNo )
{
	INT16			sGridX, sGridY;
	INT16			sScreenX, sScreenY, sMaxScreenX, sMaxScreenY;
	INT16			sDist[5], sMinDist;
	INT32			iLoop;
	INT8			bMinIndex;
	INT32 *		psEdgepointArray;
	INT32			iEdgepointArraySize;
	INT32			sClosestSpot = NOWHERE, sClosestDist = 0x7FFF, sTempDist;

	ConvertGridNoToXY( sGridNo, &sGridX, &sGridY );
	GetWorldXYAbsoluteScreenXY( sGridX, sGridY, &sScreenX, &sScreenY );

	sMaxScreenX = gsBRX - gsTLX;
	sMaxScreenY = gsBRY - gsTLY;

	sDist[0] = 0x7FFF;
	sDist[1] = sScreenX;									// west
	sDist[2] = sMaxScreenX - sScreenX;		// east
	sDist[3] = sScreenY;									// north
	sDist[4] = sMaxScreenY - sScreenY;		// south

	sMinDist = sDist[0];
	bMinIndex = 0;
	for( iLoop = 1; iLoop < 5; iLoop++)
	{
		if ( sDist[ iLoop ] < sMinDist )
		{
			sMinDist = sDist[ iLoop ];
			bMinIndex = (INT8) iLoop;
		}
	}

	switch( bMinIndex )
	{
		case 1:
			psEdgepointArray = gps1stWestEdgepointArray;
			iEdgepointArraySize = gus1stWestEdgepointArraySize;
			break;
		case 2:
			psEdgepointArray = gps1stEastEdgepointArray;
			iEdgepointArraySize = gus1stEastEdgepointArraySize;
			break;
		case 3:
			psEdgepointArray = gps1stNorthEdgepointArray;
			iEdgepointArraySize = gus1stNorthEdgepointArraySize;
			break;
		case 4:
			psEdgepointArray = gps1stSouthEdgepointArray;
			iEdgepointArraySize = gus1stSouthEdgepointArraySize;
			break;
		default:
			// WTF???
			return( NOWHERE );
	}

	// Do a 2D search to find the closest map edgepoint and
	// try to create a path there

	for ( iLoop = 0; iLoop < iEdgepointArraySize; iLoop++ )
	{
		sTempDist = PythSpacesAway( sGridNo, psEdgepointArray[ iLoop ] );
		if ( sTempDist < sClosestDist )
		{
			sClosestDist = sTempDist;
			sClosestSpot = psEdgepointArray[ iLoop ];
		}
	}

	return( sClosestSpot );
}

#define EDGE_OF_MAP_SEARCH 5

INT32 FindNearbyPointOnEdgeOfMap( SOLDIERTYPE * pSoldier, INT8 * pbDirection )
{
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;

	INT32 sGridNo, sClosestSpot = NOWHERE;
	INT8	bDirection, bClosestDirection;
	INT32 iPathCost, iClosestPathCost = 1000;

	bClosestDirection = -1;

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = EDGE_OF_MAP_SEARCH;

	iSearchRange = EDGE_OF_MAP_SEARCH;

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, WALKING, COPYREACHABLE, 0 );//dnl ch50 071009

	// Turn off the "reachable" flag for his current location
	// so we don't consider it
	gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);

	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
			{
				continue;
			}

			if (GridNoOnEdgeOfMap( sGridNo, &bDirection ) )
			{
				iPathCost = PythSpacesAway( pSoldier->sGridNo, sGridNo );

				if (iPathCost < iClosestPathCost)
				{
					// this place is closer
					sClosestSpot = sGridNo;
					iClosestPathCost = iPathCost;
					bClosestDirection = bDirection;
				}
			}
		}
	}

	*pbDirection = bClosestDirection;
	return( sClosestSpot );
}

INT32 FindRouteBackOntoMap( SOLDIERTYPE * pSoldier, INT32 sDestGridNo )
{
	// the first thing to do is restore the soldier's gridno from the X and Y
	// values

	// well, let's TRY just taking a path to the place we're supposed to go...
	if ( FindBestPath( pSoldier, sDestGridNo, pSoldier->pathing.bLevel, WALKING, COPYROUTE, 0 ) )
	{
		pSoldier->pathing.bPathStored = TRUE;
		return( sDestGridNo );
	}
	else
	{
		return( NOWHERE );
	}

}

INT32 FindClosestBoxingRingSpot( SOLDIERTYPE * pSoldier, BOOLEAN fInRing )
{
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;

	INT32 sGridNo, sClosestSpot = NOWHERE;
	INT32 iDistance, iClosestDistance = 9999;
	//DBrot: More Rooms
	//UINT8	ubRoom;
	UINT16 usRoom;

	// set the distance limit of the square region
	iSearchRange = 7;

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	if ( (pSoldier->bTeam == gbPlayerNum) && (fInRing == FALSE) )
	{
		// have player not go to the left of the ring
		sMaxLeft = 0;
	}

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));

	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( InARoom( sGridNo, &usRoom ))
			{
				if ( (fInRing && usRoom == BOXING_RING) || (!fInRing && usRoom != BOXING_RING ) && LegalNPCDestination(pSoldier,sGridNo,IGNORE_PATH,NOWATER,0) )
				{
					iDistance = abs( sXOffset ) + abs( sYOffset );
					if ( iDistance < iClosestDistance && WhoIsThere2( sGridNo, 0 ) == NOBODY )
					{
						sClosestSpot = sGridNo;
						iClosestDistance = iDistance;
					}
				}
			}

		}
	}

	return( sClosestSpot );
}

INT32 FindNearestOpenableNonDoor( INT32 sStartGridNo )
{
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;

	INT32 sGridNo, sClosestSpot = NOWHERE;
	INT32		iDistance, iClosestDistance = 9999;
	STRUCTURE * pStructure;

	// set the distance limit of the square region
	iSearchRange = 7;

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (sStartGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((sStartGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (sStartGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((sStartGridNo / MAXROW) + 1));

	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = sStartGridNo + sXOffset + (MAXCOL * sYOffset);
			pStructure = FindStructure( sGridNo, STRUCTURE_OPENABLE );
			if (pStructure)
			{
				// skip any doors
				while ( pStructure && ( pStructure->fFlags & STRUCTURE_ANYDOOR ) )
				{
					pStructure = FindNextStructure( pStructure, STRUCTURE_OPENABLE );
				}
				// if we still have a pointer, then we have found a valid non-door openable structure
				if ( pStructure )
				{
					iDistance = CardinalSpacesAway( sGridNo, sStartGridNo );
					if ( iDistance < iClosestDistance )
					{
						sClosestSpot = sGridNo;
						iClosestDistance = iDistance;
					}
				}
			}
		}
	}

	return( sClosestSpot );

}








INT32 FindFlankingSpot(SOLDIERTYPE *pSoldier, INT32 sPos, INT8 bAction )
{
	INT32 sGridNo;
	INT32 sBestSpot = NOWHERE;
	INT32 iSearchRange = 4;
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;

	DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("FindFlankingSpot: orig loc = %d, loc to flank = %d", pSoldier->sGridNo , sPos));

	// hit the edge of the map
	if ( FindNearestEdgePoint ( pSoldier->sGridNo ) == pSoldier->sGridNo	)
		return NOWHERE;


	if ( gfTurnBasedAI )
	{
		//if (pSoldier->aiData.bAlertStatus == STATUS_BLACK)			// if already in battle
		//{

		//	iSearchRange = pSoldier->bActionPoints - ( MinAPsToAttack( pSoldier, sPos, ADDTURNCOST));

		//	// to speed this up, tell PathAI to cancel any paths beyond our AP reach!
		//	gubNPCAPBudget = iSearchRange; //pSoldier->bActionPoints;
		//}
		//else
		//{
		//	// even if not under pressure, limit to 1 turn's travelling distance
		//	iSearchRange = pSoldier->bActionPoints - ( MinAPsToAttack( pSoldier, sPos, ADDTURNCOST));

		//	gubNPCAPBudget = iSearchRange; //__min( pSoldier->bActionPoints / 2, pSoldier->CalcActionPoints( ) );

			//iSearchRange = gubNPCAPBudget;
		//}
	}
	//if (!gfTurnBasedAI)
	//{
	//	// search only half as far in realtime
	//	// but always allow a certain minimum!

	//	if ( iSearchRange > 20 )
	//	{
	//		iSearchRange /= 2;
	//		gubNPCAPBudget /= 2;
	//	}
	//}

	gubNPCAPBudget=(UINT8) (iSearchRange*3);

	// assume we have to stand up and turn
	// use the min macro here to make sure we don't wrap the UINT8 to 255...

	//gubNPCAPBudget = 	gubNPCAPBudget = __min( gubNPCAPBudget, gubNPCAPBudget - GetAPsToChangeStance( pSoldier, ANIM_STAND ) -1 ); //-1 for turning cost while standing
	//NumMessage("Search Range = ",iSearchRange);
	//NumMessage("gubNPCAPBudget = ",gubNPCAPBudget);

	// stay away from the edges

	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
	//NumMessage("sMaxDown = ",sMaxDown);

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = (UINT8) iSearchRange;

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, DetermineMovementMode( pSoldier, bAction ), COPYREACHABLE, 0 );//dnl ch50 071009

	// Turn off the "reachable" flag for his current location
	// so we don't consider it
	gpWorldLevelData[pSoldier->sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);


	// get direction of position to flank from soldier's position
	INT16 sDir = GetDirectionFromGridNo ( sPos, pSoldier) ;
	INT16 sDesiredDir;
	INT16 sTempDir;
	INT16 sTempDist, sBestDist=0;

	switch ( bAction )
	{
		case AI_ACTION_FLANK_LEFT:
			sDesiredDir = sDir - 2;
			break;
		case AI_ACTION_FLANK_RIGHT:
			sDesiredDir = sDir + 2;
			break;
		case AI_ACTION_WITHDRAW:
			sDesiredDir = sDir + 4;
			break;
		default:
			sDesiredDir = sDir;
	}

	if ( sDesiredDir < 0 )
		sDesiredDir += NUM_WORLD_DIRECTIONS;

	if ( sDesiredDir > NUM_WORLD_DIRECTIONS )
		sDesiredDir -= NUM_WORLD_DIRECTIONS;

	DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("FindFlankingSpot: direction to loc = %d, dir to flank = %d", sDir , sDesiredDir ));

	for (sYOffset = -sMaxUp + 1; sYOffset <= sMaxDown - 1; ++sYOffset)
	{
		for (sXOffset = -sMaxLeft + 1; sXOffset <= sMaxRight - 1; ++sXOffset)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			sTempDir = GetDirectionFromGridNo ( sGridNo, pSoldier );
			sTempDist = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo , sGridNo );

			//NumMessage("Testing gridno #",gridno);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}

			if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
			{
				continue;
			}

			if ( sGridNo == pSoldier->pathing.sBlackList )
			{
				continue;
			}

			// exclude locations with tear/mustard gas (at this point, smoke is cool!)
			if ( InGas( pSoldier, sGridNo ) )
			{
				continue;
			}

			//Madd: skip lighted spots
			if ( InLightAtNight( sGridNo, pSoldier->pathing.bLevel ) )
				continue;

			// allow an extra direction for flanking
			if ( bAction == AI_ACTION_FLANK_LEFT )
			{
				if ( sTempDir != sDesiredDir && sTempDir != ( sDesiredDir + 1 ) )
					continue;
			}
			else if ( bAction == AI_ACTION_FLANK_RIGHT )
			{
				if ( sTempDir != sDesiredDir && sTempDir != ( sDesiredDir - 1 ) )
					continue;
			}
			else
			{
				if ( sTempDir != sDesiredDir ) //&& sTempDir != ( sDesiredDir + 1 ) && sTempDir != ( sDesiredDir - 1 ))
					continue;
			}

			// if this is better than the best place found so far
			if ( sTempDist > sBestDist )
			{
				// remember it instead
				sBestDist = sTempDist;
				sBestSpot = sGridNo;
			}
		}

	}

	DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("FindFlankingSpot: return grid no %d", sBestSpot ));

	gubNPCAPBudget = 0;
	gubNPCDistLimit = 0;

	return( sBestSpot );
}



INT32 FindClosestClimbPoint (SOLDIERTYPE *pSoldier, BOOLEAN fClimbUp )
{
	INT32 sBestSpot = NOWHERE;

	//DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , "FindClosestClimbPoint");

		//gubNPCAPBudget = 	gubNPCAPBudget = __min( gubNPCAPBudget, gubNPCAPBudget - GetAPsToChangeStance( pSoldier, ANIM_STAND ) -1 ); //-1 for turning cost while standing
	//NumMessage("Search Range = ",iSearchRange);
	//NumMessage("gubNPCAPBudget = ",gubNPCAPBudget);

	// 0verhaul:  This function is optimized to take advantage of the new climb point info.

	if (fClimbUp)
	{
		// For the climb up case, search the nearby limits for a climb up point and take the closest.
		INT32 sGridNo;
		static const INT32 iSearchRange = 20;
		INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
		UINT8 ubTestDir;

		// determine maximum horizontal limits
		sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
		//NumMessage("sMaxLeft = ",sMaxLeft);
		sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
		//NumMessage("sMaxRight = ",sMaxRight);

		// determine maximum vertical limits
		sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
		//NumMessage("sMaxUp = ",sMaxUp);
		sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));
		//NumMessage("sMaxDown = ",sMaxDown);

		//DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , String("Max: Left %d Right %d Up %d Down %d", sMaxLeft, sMaxRight, sMaxUp, sMaxDown ) );

		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				// calculate the next potential gridno
				sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
				//DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , String("Checking grid %d" , sGridNo ));

				//NumMessage("Testing gridno #",gridno);
				if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
				{
					continue;
				}


				if ( sGridNo == pSoldier->pathing.sBlackList )
				{
					continue;
				}			

				// exclude locations with tear/mustard gas (at this point, smoke is cool!)
				if ( InGas( pSoldier, sGridNo ) )
				{
					continue;
				}

				if (gpWorldLevelData[ sGridNo].ubExtFlags[0] & MAPELEMENT_EXT_CLIMBPOINT)
				{
					// Search for the destination climb point on the roof
					for ( ubTestDir = 0; ubTestDir < NUM_WORLD_DIRECTIONS; ubTestDir += 2 )
					{
						INT32 sTestGridNo = NewGridNo( sGridNo, DirectionInc( ubTestDir));
						// And see if it or the ground location is occupied
						if (gpWorldLevelData[ sTestGridNo ].ubExtFlags[1] & MAPELEMENT_EXT_CLIMBPOINT &&
							(WhoIsThere2( sTestGridNo, 1 ) == NOBODY) &&
							(WhoIsThere2( sGridNo, 0 ) == NOBODY) )
						{
							// Good climb point.  Is it closer than the previous point?
							if ( GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo , sGridNo ) < 
								GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo , sBestSpot ))
							{
								// If not, we have a new winnar!
								sBestSpot = sGridNo;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// Climbing down is easier.  Just find the nearest climb point ;)
		sBestSpot = FindClosestClimbPoint( pSoldier, pSoldier->sGridNo, pSoldier->sGridNo, fClimbUp);
	}

	//	DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , String("FindClosestClimbPoint Returning %d", sBestSpot ));
	return( sBestSpot );
}



BOOLEAN CanClimbFromHere (SOLDIERTYPE * pSoldier, BOOLEAN fUp )
{
#if 1
	return FindDirectionForClimbing( pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel) != DIRECTION_IRRELEVANT;
#else
	BUILDING * pBuilding;
	INT32 i;
	INT32 iSearchRange = 1;
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;

	//DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , "CanClimbFromHere");


	// determine maximum horizontal limits
	sMaxLeft  = min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));

	// determine maximum vertical limits
	sMaxUp   = min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));

	INT32 sGridNo=NOWHERE;

	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = pSoldier->sGridNo + sXOffset + (MAXCOL * sYOffset);
			//DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , String("Checking grid %d" , sGridNo ));

			//NumMessage("Testing gridno #",gridno);
			if ( !(sGridNo >=0 && sGridNo < WORLD_MAX) )
			{
				continue;
			}


			if ( sGridNo == pSoldier->pathing.sBlackList )
			{
				continue;
			}


			// OK, this place shows potential.	How useful is it as cover?
			//NumMessage("Promising seems gridno #",gridno);

			// Kaiden: From this point down I've removed an unneccessary call to
			// FindBuilding, The original code that was from this point till the
			// end of the function is now commented out AFTER the function.

			pBuilding = FindBuilding ( sGridNo );

			if ( pBuilding != NULL)
			{
				if ( fUp )
				{

					for (i = 0 ; i < pBuilding->ubNumClimbSpots; i++)
					{
						if (pBuilding->sUpClimbSpots[ i ] == pSoldier->sGridNo &&
							(WhoIsThere2( pBuilding->sUpClimbSpots[ i ], 0 ) == NOBODY)
							&& (WhoIsThere2( pBuilding->sDownClimbSpots[ i ], 1 ) == NOBODY) )
							return TRUE;
					}
				}
				else
				{
					for (i = 0 ; i < pBuilding->ubNumClimbSpots; i++)
					{
						if (pBuilding->sDownClimbSpots[ i ] == pSoldier->sGridNo &&
							(WhoIsThere2( pBuilding->sUpClimbSpots[ i ], 0 ) == NOBODY)
							&& (WhoIsThere2( pBuilding->sDownClimbSpots[ i ], 1 ) == NOBODY) )
							return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
#endif
}
			// OK, this place shows potential.	How useful is it as cover?
			//NumMessage("Promising seems gridno #",gridno);

/*			if ( FindBuilding ( sGridNo ) != NULL )
				pBuilding = FindBuilding ( sGridNo );
		}
	}

	DebugMsg( TOPIC_JA2AI , DBG_LEVEL_3 , String("Adjacent Building climb spots = %d" , pBuilding->ubNumClimbSpots ));
	if ( pBuilding != NULL)
	{
		if ( fUp )
		{
			for (i = 0 ; i < pBuilding->ubNumClimbSpots; i++)
			{
				if (pBuilding->sUpClimbSpots[ i ] == pSoldier->sGridNo &&
					(WhoIsThere2( pBuilding->sUpClimbSpots[ i ], 0 ) == NOBODY)
					&& (WhoIsThere2( pBuilding->sDownClimbSpots[ i ], 1 ) == NOBODY) )
					return TRUE;
			}
		}
		else
		{
			for (i = 0 ; i < pBuilding->ubNumClimbSpots; i++)
			{
				if (pBuilding->sDownClimbSpots[ i ] == pSoldier->sGridNo &&
					(WhoIsThere2( pBuilding->sUpClimbSpots[ i ], 0 ) == NOBODY)
					&& (WhoIsThere2( pBuilding->sDownClimbSpots[ i ], 1 ) == NOBODY) )
					return TRUE;
			}
		}
	}
	return FALSE;
}
*/
extern BUILDING gBuildings[ MAX_BUILDINGS ];
extern UINT8 gubNumberOfBuildings;


INT32 FindBestCoverNearTheGridNo(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubSearchRadius )
{
	INT32 iPercentBetter;
//	INT16 sTrueGridNo;
	INT16 sResultGridNo;
	INT8 bRealWisdom = pSoldier->stats.bWisdom;

//	sTrueGridNo = pSoldier->sGridNo;
//	pSoldier->sGridNo = sGridNo;
	pSoldier->stats.bWisdom = 8 * ubSearchRadius;// 5 tiles

	sResultGridNo = FindBestNearbyCover( pSoldier, MORALE_NORMAL, &iPercentBetter);

	pSoldier->stats.bWisdom = bRealWisdom;
//	pSoldier->sGridNo = sTrueGridNo;
	
	if(!TileIsOutOfBounds(sResultGridNo))
		return sResultGridNo;
	else
		return sGridNo;

}

INT8 FindDirectionForClimbing( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel )
{
	UINT8 ubClimbDir;
	INT32 sClimbSpot;

	if (bLevel == 0)
	{
		if (gpWorldLevelData[ sGridNo].ubExtFlags[0] & MAPELEMENT_EXT_CLIMBPOINT)
		{
			for ( ubClimbDir = 0; ubClimbDir < NUM_WORLD_DIRECTIONS; ubClimbDir += 2 )
			{
				sClimbSpot = NewGridNo( sGridNo, DirectionInc( ubClimbDir));
				if (gpWorldLevelData[ sClimbSpot].ubExtFlags[1] & MAPELEMENT_EXT_CLIMBPOINT &&
					WhoIsThere2( sClimbSpot, 1 ) == NOBODY )
				{
					return ubClimbDir;
				}
			}
		}
	}
	else if (bLevel == 1)
	{
		//CHRISL: If NewInv and wearing a backpack, don't allow climbing
		if (UsingNewInventorySystem() == true && pSoldier->inv[BPACKPOCKPOS].exists() == true
			//JMich.BackpackClimb
			&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
			&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
			return DIRECTION_IRRELEVANT;
		if (gpWorldLevelData[ sGridNo].ubExtFlags[1] & MAPELEMENT_EXT_CLIMBPOINT)
		{
			for ( ubClimbDir = 0; ubClimbDir< NUM_WORLD_DIRECTIONS; ubClimbDir += 2 )
			{
				sClimbSpot = NewGridNo( sGridNo, DirectionInc( ubClimbDir));
				if (gpWorldLevelData[ sClimbSpot].ubExtFlags[0] & MAPELEMENT_EXT_CLIMBPOINT &&
					WhoIsThere2( sClimbSpot, 0 ) == NOBODY )
				{
					return ubClimbDir;
				}
			}
		}
	}
	return DIRECTION_IRRELEVANT;
}

INT32 FindNearestPassableSpot( INT32 sGridNo, UINT8 usSearchRadius )
{
	INT32	sNearestSpot = NOWHERE;
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32	sTestGridNo;
	INT32	iDist, iClosestDist = 10;

	// determine maximum horizontal limits
	sMaxLeft  = min( usSearchRadius, (sGridNo % MAXCOL));
	sMaxRight = min( usSearchRadius, MAXCOL - ((sGridNo % MAXCOL) + 1));

	// determine maximum vertical limits
	sMaxUp   = min( usSearchRadius, (sGridNo / MAXROW));
	sMaxDown = min( usSearchRadius, MAXROW - ((sGridNo / MAXROW) + 1));

	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sTestGridNo = sGridNo + sXOffset + (MAXCOL * sYOffset);
			// is this an empty tile (no structure on it) or is the structure passable?
			if (gpWorldLevelData[sTestGridNo].pStructureHead == NULL || FindStructure( sTestGridNo, STRUCTURE_PASSABLE ) != NULL)
			{
				iDist = PythSpacesAway( sGridNo, sTestGridNo );
				if (iDist < iClosestDist)
				{
					iClosestDist = iDist;
					sNearestSpot = sTestGridNo;
				}
			}
		}
	}

	return( sNearestSpot );
}
