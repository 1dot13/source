	#include "ai.h"
	#include "AIInternals.h"
	#include "opplist.h"
	#include "Soldier Profile.h"
	#include "los.h"
	#include "NPC.h"
	#include "Quests.h"
	#include "Render Fun.h"
	#include "Soldier macros.h"

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

void CallAvailableEnemiesTo( INT32 sGridNo )
{
	// All enemy teams become aware of a very important "noise" coming from here!
	for ( INT32 iLoop = 0; iLoop < LAST_TEAM; ++iLoop )
	{
		CallAvailableTeamEnemiesTo( sGridNo, iLoop );
	}
}

void CallAvailableTeamEnemiesTo( INT32 sGridNo, INT8 bTeam )
{
	SOLDIERTYPE * pSoldier;

	// All enemy teams become aware of a very important "noise" coming from here!
	// if this team is active
	if (gTacticalStatus.Team[bTeam].bTeamActive)
	{
		// if this team is computer-controlled, and isn't the CIVILIAN "team"
		if (!(gTacticalStatus.Team[bTeam].bHuman) && (bTeam != CIV_TEAM))
		{
			// make this team (publicly) aware of the "noise"
			gsPublicNoiseGridNo[bTeam] = sGridNo;
			gubPublicNoiseVolume[bTeam] = MAX_MISC_NOISE_DURATION;

			// new situation for everyone;
			SoldierID iLoop2 = gTacticalStatus.Team[ bTeam ].bFirstID;
			for ( ; iLoop2 <= gTacticalStatus.Team[ bTeam ].bLastID; ++iLoop2 )
			{
				pSoldier = iLoop2;
				if (pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE)
				{
					SetNewSituation( pSoldier );
					WearGasMaskIfAvailable( pSoldier );
				}
			}
		}
	}
}

void CallAvailableKingpinMenTo( INT32 sGridNo )
{
	// like call all enemies, but only affects civgroup KINGPIN guys with
	// NO PROFILE

	SOLDIERTYPE * pSoldier;

	// All enemy teams become aware of a very important "noise" coming from here!
	// if this team is active
	if (gTacticalStatus.Team[CIV_TEAM].bTeamActive)
	{
		// make this team (publicly) aware of the "noise"
		gsPublicNoiseGridNo[CIV_TEAM] = sGridNo;
		gubPublicNoiseVolume[CIV_TEAM] = MAX_MISC_NOISE_DURATION;

		// new situation for everyone...

		SoldierID iLoop2 = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
		for ( ; iLoop2 <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ++iLoop2 )
		{
			pSoldier = iLoop2;
			if (pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE && pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP && pSoldier->ubProfile == NO_PROFILE)
			{
				SetNewSituation( pSoldier );
			}
		}
	}
}

void CallEldinTo( INT32 sGridNo )
{
	// like call all enemies, but only affects Eldin
	SOLDIERTYPE * pSoldier;

	// Eldin becomes aware of a very important "noise" coming from here!
	// So long as he hasn't already heard a noise a sec ago...
	if (gTacticalStatus.Team[CIV_TEAM].bTeamActive)
	{
		// new situation for Eldin
		pSoldier = FindSoldierByProfileID( ELDIN, FALSE );
		if ( pSoldier && pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE && (pSoldier->aiData.bAlertStatus == STATUS_GREEN || pSoldier->aiData.ubNoiseVolume < (MAX_MISC_NOISE_DURATION / 2) ) )
		{
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, 0, 0, TRUE ) )
			{
				// sees the player now!
				TriggerNPCWithIHateYouQuote( ELDIN );
				SetNewSituation( pSoldier );
			}
			else
			{
				pSoldier->aiData.sNoiseGridno = sGridNo;
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
				if ( (pSoldier->aiData.bAction != AI_ACTION_GET_CLOSER) || CheckFact( FACT_MUSEUM_ALARM_WENT_OFF, 0 ) == FALSE )
				{
					DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: CallEldinTo"));
					CancelAIAction( pSoldier, TRUE );
					pSoldier->aiData.bNextAction = AI_ACTION_GET_CLOSER;
					pSoldier->aiData.usNextActionData = sGridNo;
					RESETTIMECOUNTER( pSoldier->timeCounters.AICounter, 100 );
				}
				// otherwise let AI handle this normally
//				SetNewSituation( pSoldier );
				// reduce any delay to minimal
			}
			SetFactTrue( FACT_MUSEUM_ALARM_WENT_OFF );
		}
	}
}


INT32 MostImportantNoiseHeard( SOLDIERTYPE *pSoldier, INT32 *piRetValue, BOOLEAN * pfClimbingNecessary, BOOLEAN * pfReachable )
{
	UINT32 uiLoop;
	INT8 * pbPersOL, * pbPublOL;
	INT32 *psLastLoc,*psNoiseGridNo;
	INT8 * pbNoiseLevel;
	INT8 *pbLastLevel;
	UINT8 *pubNoiseVolume;
	INT32 iDistAway;
	INT32	iNoiseValue, iBestValue = -10000;
	INT32 sBestGridNo = NOWHERE;
	INT8	bBestLevel = 0;
	INT32 sClimbingGridNo;
	BOOLEAN fClimbingNecessary = FALSE;
	SOLDIERTYPE * pTemp;

	pubNoiseVolume = &gubPublicNoiseVolume[pSoldier->bTeam];
	psNoiseGridNo = &gsPublicNoiseGridNo[pSoldier->bTeam];
	pbNoiseLevel = &gbPublicNoiseLevel[pSoldier->bTeam];

	psLastLoc = gsLastKnownOppLoc[pSoldier->ubID];

	// hang pointers at start of this guy's personal and public opponent opplists
	pbPersOL = pSoldier->aiData.bOppList;
	pbPublOL = gbPublicOpplist[pSoldier->bTeam];

	// sevenfm: sector information
	UINT8 ubSectorId = SECTOR( gWorldSectorX, gWorldSectorY );
	UINT8 sectordata = SectorExternalData[ubSectorId][0].usCurfewValue;

	// look through this man's personal & public opplists for opponents heard
	for (uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop)
	{
		pTemp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pTemp || !pTemp->stats.bLife)
			continue;			// next merc

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pTemp ) || (pSoldier->bSide == pTemp->bSide))
			continue;			// next merc

		// Flugente: chance to ignore the noise if the ceator is covert
		if ( pTemp->usSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER) )
		{
			// green  AI state: always ignore
			// yellow AI state: 50% chance to ignore
			if ( pSoldier->aiData.bAlertStatus == STATUS_GREEN || (pSoldier->aiData.bAlertStatus == STATUS_YELLOW && Random(2) ) )
				continue;

			// sevenfm: ignore noise if some friends already see opponent
			if ( CountTeamSeeSoldier( pSoldier->bTeam, pTemp ) > sectordata )
				continue;
		}

		pbPersOL = pSoldier->aiData.bOppList + pTemp->ubID;
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pTemp->ubID;
		psLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pTemp->ubID;
		pbLastLevel = gbLastKnownOppLevel[pSoldier->ubID] + pTemp->ubID;

		// if this guy's been personally heard within last 3 turns
		if (*pbPersOL < NOT_HEARD_OR_SEEN)
		{
			// calculate how far this noise was, and its relative "importance"
			iDistAway = SpacesAway(pSoldier->sGridNo,*psLastLoc);
			iNoiseValue = (*pbPersOL) * iDistAway;				// always a negative number!

			if (iNoiseValue > iBestValue)
			{
				iBestValue = iNoiseValue;
				sBestGridNo = *psLastLoc;
				bBestLevel = *pbLastLevel;
			}
		}

		// if this guy's been publicly heard within last 3 turns
		if (*pbPublOL < NOT_HEARD_OR_SEEN)
		{
			// calculate how far this noise was, and its relative "importance"
			iDistAway = SpacesAway(pSoldier->sGridNo, gsPublicLastKnownOppLoc[pSoldier->bTeam][pTemp->ubID]);
			iNoiseValue = (*pbPublOL) * iDistAway;				// always a negative number!

			if (iNoiseValue > iBestValue)
			{
				iBestValue = iNoiseValue;
				sBestGridNo = gsPublicLastKnownOppLoc[pSoldier->bTeam][pTemp->ubID];
				bBestLevel = gbPublicLastKnownOppLevel[pSoldier->bTeam][pTemp->ubID];
			}
		}
	}

	// if any "misc. noise" was also heard recently	
	if (!TileIsOutOfBounds(pSoldier->aiData.sNoiseGridno))
	{
		if ( pSoldier->bNoiseLevel != pSoldier->pathing.bLevel || PythSpacesAway( pSoldier->sGridNo, pSoldier->aiData.sNoiseGridno ) >= 6 || SoldierTo3DLocationLineOfSightTest( pSoldier, pSoldier->aiData.sNoiseGridno, pSoldier->bNoiseLevel, 0, FALSE, NO_DISTANCE_LIMIT ) == 0 )
		{
			// calculate how far this noise was, and its relative "importance"
			iDistAway = SpacesAway(pSoldier->sGridNo,pSoldier->aiData.sNoiseGridno);
			iNoiseValue = ((pSoldier->aiData.ubNoiseVolume / 2) - 6) * iDistAway;

			if (iNoiseValue > iBestValue)
			{
				iBestValue = iNoiseValue;
				sBestGridNo = pSoldier->aiData.sNoiseGridno;
				bBestLevel = pSoldier->bNoiseLevel;
			}
		}
		else
		{
			// we are there or near
			pSoldier->aiData.sNoiseGridno = NOWHERE;		// wipe it out, not useful anymore
			pSoldier->aiData.ubNoiseVolume = 0;
		}
	}
	
	// if any recent PUBLIC "misc. noise" is also known
	if ( (pSoldier->bTeam != CIV_TEAM) || ( pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP ) )
	{		
		if (!TileIsOutOfBounds(*psNoiseGridNo))
		{
			// if we are NOT there (at the noise gridno)
			if ( *pbNoiseLevel != pSoldier->pathing.bLevel || PythSpacesAway( pSoldier->sGridNo, *psNoiseGridNo ) >= 6 || SoldierTo3DLocationLineOfSightTest( pSoldier, *psNoiseGridNo, *pbNoiseLevel, 0, FALSE, NO_DISTANCE_LIMIT ) == 0 )
			{
				// calculate how far this noise was, and its relative "importance"
				iDistAway = SpacesAway(pSoldier->sGridNo,*psNoiseGridNo);
				iNoiseValue = ((*pubNoiseVolume / 2) - 6) * iDistAway;

				if (iNoiseValue > iBestValue)
				{
					iBestValue = iNoiseValue;
					sBestGridNo = *psNoiseGridNo;
					bBestLevel = *pbNoiseLevel;
				}
			}
		}
	}
	
	if (!TileIsOutOfBounds(sBestGridNo) && pfReachable )
	{
		*pfReachable = TRUE;

		// make civs not walk to noises outside their room if on close patrol/onguard
		if ( pSoldier->aiData.bOrders <= CLOSEPATROL && (pSoldier->bTeam == CIV_TEAM || pSoldier->ubProfile != NO_PROFILE ) )
		{
			//DBrot: More Rooms
			//UINT8	ubRoom, ubNewRoom;
			UINT16 usRoom, usNewRoom;

			// any other combo uses the default of ubRoom == 0, set above
			if ( InARoom( pSoldier->aiData.sPatrolGrid[0], &usRoom ) )
			{
				if ( !InARoom( pSoldier->aiData.sPatrolGrid[0], &usNewRoom ) || usRoom != usNewRoom )
				{
					*pfReachable = FALSE;
				}
			}
		}

		if ( *pfReachable )
		{
			// if there is a climb involved then we should store the location
			// of where we have to climb to instead
			sClimbingGridNo = GetInterveningClimbingLocation( pSoldier, sBestGridNo, bBestLevel, &fClimbingNecessary );
			if ( fClimbingNecessary )
			{				
				if (TileIsOutOfBounds(sClimbingGridNo))
				{
					// can't investigate!
					*pfReachable = FALSE;
				}
				else
				{
					sBestGridNo = sClimbingGridNo;
					fClimbingNecessary = TRUE;
				}
			}
			else
			{
				fClimbingNecessary = FALSE;
			}
		}
	}

	if ( piRetValue )
	{
		*piRetValue = iBestValue;
	}

	if ( pfClimbingNecessary )
	{
		*pfClimbingNecessary = fClimbingNecessary;
	}

#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sBestGridNo))
		AINumMessage("MOST IMPORTANT NOISE HEARD FROM GRID #",sBestGridNo);
#endif

	return(sBestGridNo);
}


INT16 WhatIKnowThatPublicDont(SOLDIERTYPE *pSoldier, UINT8 ubInSightOnly)
{
	UINT8 ubTotal = 0;
	UINT32 uiLoop;
	INT8 *pbPersOL,*pbPublOL;
	SOLDIERTYPE * pTemp;

	// if merc knows of a more important misc. noise than his team does
	if (!(CREATURE_OR_BLOODCAT( pSoldier )) && (pSoldier->aiData.ubNoiseVolume > gubPublicNoiseVolume[pSoldier->bTeam]))
	{
		// the difference in volume is added to the "new info" total
		ubTotal += pSoldier->aiData.ubNoiseVolume - gubPublicNoiseVolume[pSoldier->bTeam];
	}

	// hang pointers at start of this guy's personal and public opponent opplists
	pbPersOL = &(pSoldier->aiData.bOppList[0]);
	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);

	// for every opponent
//	for (iLoop = 0; iLoop < MAXMERCS; iLoop++,pbPersOL++,pbPublOL++)
//	{
//	pTemp = &(Menptr[iLoop]);


	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pTemp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pTemp)
		{
			continue;			// next merc
		}

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pTemp ) || (pSoldier->bSide == pTemp->bSide))
		{
			continue;			// next merc
		}

		pbPersOL = pSoldier->aiData.bOppList + pTemp->ubID;
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pTemp->ubID;


		// if we're only interested in guys currently is sight, and he's not
		if (ubInSightOnly)
		{
			if ((*pbPersOL == SEEN_CURRENTLY) && (*pbPublOL != SEEN_CURRENTLY))
			{
				// just count the number of them
				ubTotal++;
			}
		}
		else
		{
			// add value of personal knowledge compared to public knowledge to total
			ubTotal += gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE];
		}
	}

#ifdef DEBUGDECISIONS
	if (ubTotal > 0)
	{
		AINumMessage("WHAT I KNOW THAT PUBLIC DON'T = ",ubTotal);
	}
#endif

	return(ubTotal);
}
