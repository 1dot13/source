#ifdef PRECOMPILEDHEADERS
#include "AI All.h"
#else
#include "ai.h"
#include "Weapons.h"
#include "opplist.h"
#include "AIInternals.h"
#include "LOS.h"
#include "Physics.h"
#include "Items.h"
#include "Weapons.h"
#include "Spread Burst.h"
#include "overhead.h"
#include "SkillCheck.h"
#include "Soldier Profile.h"
#include "Isometric Utils.h"
#include "Soldier macros.h"
#include "PATHAI.H"
#include "GameSettings.h"
#include "strategicmap.h"
#include "environment.h"
#include "lighting.h"
#include "Sound Control.h"
#endif

extern INT16 DirIncrementer[8];

//
// CJC DG->JA2 conversion notes
//
// Still commented out:
//
// EstimateShotDamage - stuff related to legs?
// EstimateStabDamage - stuff related to armour
// EstimateThrowDamage - waiting for grenade, armour definitions
// CheckIfTossPossible - waiting for grenade definitions

// this define should go in soldier control.h


void LoadWeaponIfNeeded(SOLDIERTYPE *pSoldier)
{
	UINT16 usInHand;
	INT8 bPayloadPocket;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("LoadWeaponIfNeeded"));

	usInHand = pSoldier->inv[HANDPOS].usItem;

	if ( IsGrenadeLauncherAttached(&pSoldier->inv[HANDPOS]) )
		usInHand = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS]);

	// if he's got a MORTAR in his hand, make sure he has a MORTARSHELL avail.
	if (Item[usInHand].mortar )
	{
		//		bPayloadPocket = FindObj( pSoldier, MORTAR_SHELL );
		bPayloadPocket = FindLaunchable( pSoldier, usInHand );
		if (bPayloadPocket == NO_SLOT)
		{
#ifdef BETAVERSION
			NumMessage("LoadWeaponIfNeeded: ERROR - no mortar shells found to load MORTAR!	Guynum",pSoldier->ubID);
#endif
			return;	// no shells, can't fire the MORTAR
		}
	}
	// if he's got a GL in his hand, make sure he has some type of GRENADE avail.
	else if (Item[usInHand].grenadelauncher )
	{
		bPayloadPocket = FindGLGrenade( pSoldier );
		if (bPayloadPocket == NO_SLOT || FindNonSmokeLaunchableAttachment( &pSoldier->inv[HANDPOS],usInHand ) != 0 )
		{
#ifdef BETAVERSION
			NumMessage("LoadWeaponIfNeeded: ERROR - no grenades found to load GLAUNCHER!	Guynum",pSoldier->ubID);
#endif
			return;	// no grenades, can't fire the GLAUNCHER... or the launcher has a magsize > 1
		}
	}
	// if he's got a RPG7 in his hand, make sure he has some type of RPG avail.
	else if (Item[usInHand].rocketlauncher && !Item[usInHand].singleshotrocketlauncher )
	{
		bPayloadPocket = FindLaunchable (pSoldier, usInHand );
		if (bPayloadPocket == NO_SLOT)
		{
			return;	// no grenades, can't fire
		}
	}
	else if (Item[usInHand].cannon )
	{
		bPayloadPocket = FindLaunchable( pSoldier, usInHand );
		if (bPayloadPocket == NO_SLOT)
		{
			return;
		}
	}
	else
	{
		// regular hand-thrown grenade in hand, nothing to load!
		return;
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("LoadWeaponIfNeeded: remove payload from its pocket, and add it as the hand weapon's first attachment"));
	// remove payload from its pocket, and add it as the hand weapon's first attachment

	if ( TANK( pSoldier ) )
	{
		// don't remove ammo
		gTempObject = pSoldier->inv[bPayloadPocket];
		if (gTempObject.ubNumberOfObjects > 1) {
			gTempObject.RemoveObjectsFromStack(gTempObject.ubNumberOfObjects - 1);
		}
		pSoldier->inv[HANDPOS].AttachObject(pSoldier,&gTempObject,FALSE);
	}
	else if (pSoldier->inv[bPayloadPocket].MoveThisObjectTo(gTempObject, 1) == 0) {
		pSoldier->inv[HANDPOS].AttachObject( pSoldier,&gTempObject,FALSE);
	}
}

void CalcBestShot(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestShot, BOOLEAN shootUnseen)
{
	UINT32 uiLoop;
	INT32 iAttackValue;
	INT32 iThreatValue;
	INT32 iHitRate,iBestHitRate,iPercentBetter;
	INT32 iEstDamage;
	UINT8 ubMaxPossibleAimTime;
	INT16 ubAimTime,ubMinAPcost,ubRawAPCost;
	UINT8 ubChanceToReallyHit = 0;
	INT16 ubChanceToHit,ubBestAimTime,ubChanceToGetThrough,ubBestChanceToHit;
	SOLDIERTYPE *pOpponent;
	INT16 ubBurstAPs;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalcBestShot");

	ubBestChanceToHit = ubBestAimTime = ubChanceToHit = 0;

	pSoldier->usAttackingWeapon = pSoldier->inv[HANDPOS].usItem;
	pSoldier->bWeaponMode = WM_NORMAL;

	ubBurstAPs = CalcAPsToBurst( pSoldier->CalcActionPoints( ), &(pSoldier->inv[HANDPOS]) );

	InitAttackType(pBestShot);		// set all structure fields to defaults

	// hang a pointer into active soldier's personal opponent list
	//pbPersOL = &(pSoldier->aiData.bOppList[0]);

	// determine which attack against which target has the greatest attack value
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpponent || !pOpponent->stats.bLife)
			continue;			// next merc

		// if this man is neutral / on the same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
			continue;			// next merc

		// if this opponent is not currently in sight (ignore known but unseen!)
		if ((pSoldier->aiData.bOppList[pOpponent->ubID] != SEEN_CURRENTLY && !shootUnseen)) //guys we can't see
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CalcBestShot: soldier = %d, target = %d, skip guys we can't see, shootUnseen = %d, personal opplist = %d",pSoldier->ubID, pOpponent->ubID, shootUnseen, pSoldier->aiData.bOppList[pOpponent->ubID]));
			continue;	// next opponent
		}
		if ((pSoldier->aiData.bOppList[pOpponent->ubID] != SEEN_CURRENTLY && gbPublicOpplist[pSoldier->bTeam][pOpponent->ubID] != SEEN_CURRENTLY)) // guys nobody sees
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CalcBestShot: soldier = %d, target = %d, skip guys nobody sees, shootUnseen = %d, public opplist = %d",pSoldier->ubID, pOpponent->ubID, shootUnseen,gbPublicOpplist[pSoldier->bTeam][pOpponent->ubID]));
			continue;	// next opponent
		}


		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != SLAY)
			continue;	// next opponent

#ifdef DEBUGATTACKS
		DebugAI( String( "%s sees %s at gridno %d\n",pSoldier->name,ExtMen[pOpponent->ubID].name,pOpponent->sGridNo ) );
#endif

		// calculate minimum action points required to shoot at this opponent
		//	if ( !Weapon[pSoldier->usAttackingWeapon].NoSemiAuto )
		ubMinAPcost = MinAPsToAttack(pSoldier,pOpponent->sGridNo,ADDTURNCOST);
		// What the.... The APs to attack on the HandleItem side does not make a test like this.	It always uses the MinAPs as a base.
		//	else
		//		ubMinAPcost = CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[HANDPOS]), 3 );

		//NumMessage("MinAPcost to shoot this opponent = ",ubMinAPcost);

		// if we don't have enough APs left to shoot even a snap-shot at this guy
		if (ubMinAPcost > pSoldier->bActionPoints)
			continue;			// next opponent

		//KeepInterfaceGoing();

		// calculate chance to get through the opponent's cover (if any)

		ubChanceToGetThrough = AISoldierToSoldierChanceToGetThrough( pSoldier, pOpponent );

		//	ubChanceToGetThrough = ChanceToGetThrough(pSoldier,pOpponent->sGridNo,NOTFAKE,ACTUAL,TESTWALLS,9999,M9PISTOL,NOT_FOR_LOS);

		//NumMessage("Chance to get through = ",ubChanceToGetThrough);

		// if we can't possibly get through all the cover
		if (ubChanceToGetThrough == 0)
			continue;			// next opponent

		if ( (pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER) && (pSoldier->ubBodyType != QUEENMONSTER ) )
		{
			STRUCTURE_FILE_REF *	pStructureFileRef;
			UINT16								usAnimSurface;

			usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, pSoldier->usUIMovementMode );
			pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface,pSoldier->usUIMovementMode	);

			if ( pStructureFileRef )
			{
				UINT16		usStructureID;
				INT8			bDir;

				// must make sure that structure data can be added in the direction of the target
				bDir = (INT8) GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, pOpponent->sGridNo );

				// ATE: Only if we have a levelnode...
				if ( pSoldier->pLevelNode != NULL && pSoldier->pLevelNode->pStructureData != NULL )
				{
					usStructureID = pSoldier->pLevelNode->pStructureData->usStructureID;
				}
				else
				{
					usStructureID = INVALID_STRUCTURE_ID;
				}

				if ( ! OkayToAddStructureToWorld( pSoldier->sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[ gOneCDirection[ bDir ] ]), usStructureID ) )
				{
					// can't turn in that dir.... next opponent
					continue;
				}
			}
		}

		// calc next attack's minimum shooting cost (excludes readying & turning)
		ubRawAPCost = MinAPsToShootOrStab(pSoldier,pOpponent->sGridNo,FALSE);

		if (pOpponent->sGridNo == pSoldier->sLastTarget)
		{
			// raw AP cost calculation included cost of changing target!
			// Not unless we really needed to change targets!
			//ubRawAPCost -= APBPConstants[AP_CHANGE_TARGET];
		}

		iBestHitRate = 0;					 // reset best hit rate to minimum

		// calculate the maximum possible aiming time

		if ( TANK( pSoldier ) )
		{
			ubMaxPossibleAimTime = pSoldier->bActionPoints - ubMinAPcost;

			// always burst
			if ( ubMaxPossibleAimTime < ubBurstAPs )
			{
				// should this be a return instead?
				continue;
			}
			// bursts aren't aimed
			ubMaxPossibleAimTime = 0;

		}
		else
		{
			ubMaxPossibleAimTime = min(AllowedAimingLevels(pSoldier),pSoldier->bActionPoints - ubMinAPcost);
		}

		// consider the various aiming times
		if ( !Weapon[pSoldier->usAttackingWeapon].NoSemiAuto )
		{
			for (ubAimTime = APBPConstants[AP_MIN_AIM_ATTACK]; ubAimTime <= ubMaxPossibleAimTime; ubAimTime++)
			{
				//HandleMyMouseCursor(KEYBOARDALSO);

				//NumMessage("ubAimTime = ",ubAimTime);

				ubChanceToHit = (INT16) AICalcChanceToHitGun(pSoldier,pOpponent->sGridNo,ubAimTime, AIM_SHOT_TORSO);
				// ExtMen[pOpponent->ubID].haveStats = TRUE;
				//NumMessage("chance to Hit = ",ubChanceToHit);

				//sprintf(tempstr,"Vs. %s, at AimTime %d, ubChanceToHit = %d",ExtMen[pOpponent->ubID].name,ubAimTime,ubChanceToHit);
				//PopMessage(tempstr);

				iHitRate = (pSoldier->bActionPoints * ubChanceToHit) / (ubRawAPCost + ubAimTime);
				//NumMessage("hitRate = ",iHitRate);

				// if aiming for this amount of time produces a better hit rate
				if (iHitRate > iBestHitRate)
				{
					iBestHitRate = iHitRate;
					ubBestAimTime = ubAimTime;
					ubBestChanceToHit = ubChanceToHit;
				}
			}
		}
		else
		{
			ubAimTime = APBPConstants[AP_MIN_AIM_ATTACK];
			ubChanceToHit = (INT16) AICalcChanceToHitGun(pSoldier,pOpponent->sGridNo,ubAimTime, AIM_SHOT_TORSO);
			Assert( ubRawAPCost > 0);
			iHitRate = (pSoldier->bActionPoints * ubChanceToHit) / (ubRawAPCost + ubAimTime);

			iBestHitRate = iHitRate;
			ubBestAimTime = ubAimTime;
			ubBestChanceToHit = ubChanceToHit;
		}

		// if we can't get any kind of hit rate at all
		if (iBestHitRate == 0)
			continue;			// next opponent

		// calculate chance to REALLY hit: shoot accurately AND get past cover
		ubChanceToReallyHit = (ubBestChanceToHit * ubChanceToGetThrough) / 100;

		// if we can't REALLY hit at all
		if (ubChanceToReallyHit == 0)
			continue;			// next opponent

		// really limit knife throwing so it doesn't look wrong
		if ( Item[ pSoldier->usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE && (ubChanceToReallyHit < 30 || ( PythSpacesAway( pSoldier->sGridNo, pOpponent->sGridNo ) > CalcMaxTossRange( pSoldier, pSoldier->usAttackingWeapon, FALSE ) )))// Madd / 2 ) ) )
			continue; // don't bother... next opponent

		// calculate this opponent's threat value (factor in my cover from him)
		iThreatValue = CalcManThreatValue(pOpponent,pSoldier->sGridNo,TRUE,pSoldier);

		// estimate the damage this shot would do to this opponent
		iEstDamage = EstimateShotDamage(pSoldier,pOpponent,ubBestChanceToHit);
		//NumMessage("SHOT EstDamage = ",iEstDamage);

		// calculate the combined "attack value" for this opponent
		// highest possible value before division should be about 1.8 billion...
		// normal value before division should be about 5 million...
		iAttackValue = (iEstDamage * iBestHitRate * ubChanceToReallyHit * iThreatValue) / 1000;
		//NumMessage("SHOT AttackValue = ",iAttackValue / 1000);

		// special stuff for assassins to ignore militia more
		if ( pSoldier->ubProfile >= JIM && pSoldier->ubProfile <= TYRONE && pOpponent->bTeam == MILITIA_TEAM )
		{
			iAttackValue /= 2;
		}

#ifdef DEBUGATTACKS
		DebugAI( String( "CalcBestShot: best AttackValue vs %d = %d\n",uiLoop,iAttackValue ) );
#endif

		// if we can hurt the guy, OR probably not, but at least it's our best
		// chance to actually hit him and maybe scare him, knock him down, etc.
		if ((iAttackValue > 0) || (ubChanceToReallyHit > pBestShot->ubChanceToReallyHit))
		{
			// if there already was another viable target
			if (pBestShot->ubChanceToReallyHit > 0)
			{
				// OK, how does our chance to hit him compare to the previous best one?
				iPercentBetter = ((ubChanceToReallyHit * 100) / pBestShot->ubChanceToReallyHit) - 100;

				// if this chance to really hit is more than 50% worse, and the other
				// guy is conscious at all
				if ((iPercentBetter < -PERCENT_TO_IGNORE_THREAT) && (Menptr[pBestShot->ubOpponent].stats.bLife >= OKLIFE))
					// then stick with the older guy as the better target
					continue;

				// if this chance to really hit between 50% worse to 50% better
				if (iPercentBetter < PERCENT_TO_IGNORE_THREAT)
				{
					// then the one with the higher ATTACK VALUE is the better target
					if (iAttackValue < pBestShot->iAttackValue)
						// the previous guy is more important since he's more dangerous
						continue;			// next opponent
				}
			}

			// OOOF!	That was a lot of work!	But we've got a new best target!
			pBestShot->ubPossible			= TRUE;
			pBestShot->ubOpponent			= pOpponent->ubID;
			pBestShot->ubAimTime			= ubBestAimTime;
			pBestShot->ubChanceToReallyHit = ubChanceToReallyHit;
			pBestShot->sTarget			 = pOpponent->sGridNo;
			pBestShot->bTargetLevel				= pOpponent->pathing.bLevel;
			pBestShot->iAttackValue		= iAttackValue;
			pBestShot->ubAPCost			= ubMinAPcost;
		}
	}
}

// JA2Gold: added
BOOLEAN CloseEnoughForGrenadeToss( INT16 sGridNo, INT16 sGridNo2 )
{
	INT16	sTempGridNo;
	UINT8	ubDirection;
	INT16	sXPos, sYPos, sXPos2, sYPos2;
	UINT8	ubMovementCost;

	if (sGridNo == sGridNo2 )
	{
		// checking the same space; if there is a closed door next to location in ANY direction then forget it
		// (could be the player closed a door on us)
		for (ubDirection = 0; ubDirection < NUM_WORLD_DIRECTIONS; ubDirection++)
		{
			sTempGridNo = NewGridNo( sGridNo, DirectionInc( ubDirection ) );
			ubMovementCost = gubWorldMovementCosts[ sTempGridNo ][ ubDirection ][ 0 ];
			if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
			{
				ubMovementCost = DoorTravelCost( NULL, sTempGridNo, ubMovementCost, FALSE, NULL );
			}
			if ( ubMovementCost >= TRAVELCOST_BLOCKED)
			{
				return( FALSE );
			}
		}
	}
	else
	{
		if ( CardinalSpacesAway( sGridNo, sGridNo2 ) > 2 )
		{
			return( FALSE );
		}

		// we are within 1 space diagonally or at most 2 horizontally or vertically,
		// so we can now do a loop safely

		sTempGridNo = sGridNo;

		sXPos = CenterX( sGridNo );
		sYPos = CenterY( sGridNo );
		sXPos2 = CenterX( sGridNo2 );
		sYPos2 = CenterY( sGridNo2 );
		ubDirection = atan8( sXPos, sYPos, sXPos2, sYPos2 );

		// For each step of the loop, we are checking for door or obstacle movement costs.	If we
		// find we're blocked, then this is no good for grenade tossing!
		do
		{
			sTempGridNo = NewGridNo( sTempGridNo, DirectionInc( ubDirection ) );
			ubMovementCost = gubWorldMovementCosts[ sTempGridNo ][ ubDirection ][ 0 ];
			if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
			{
				ubMovementCost = DoorTravelCost( NULL, sTempGridNo, ubMovementCost, FALSE, NULL );
			}
			if ( ubMovementCost >= TRAVELCOST_BLOCKED)
			{
				return( FALSE );
			}
		} while( sTempGridNo != sGridNo2 );
	}

	return( TRUE );
}

void CalcBestThrow(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestThrow)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow");
	// September 9, 1998: added code for LAWs (CJC)
	UINT8 ubLoop, ubLoop2;
	INT32 iAttackValue;
	INT32 iHitRate, iThreatValue, iTotalThreatValue,iOppThreatValue[MAXMERCS];
	INT16 sGridNo, sEndGridNo, sFriendTile[MAXMERCS], sOpponentTile[MAXMERCS];
	INT8	bFriendLevel[MAXMERCS], bOpponentLevel[MAXMERCS];
	INT32 iEstDamage;
	UINT8 ubFriendCnt = 0,ubOpponentCnt = 0, ubOpponentID[MAXMERCS];
	UINT8 ubMaxPossibleAimTime;
	INT16 ubRawAPCost,ubMinAPcost;
	UINT8 ubChanceToHit,ubChanceToGetThrough,ubChanceToReallyHit;
	UINT32 uiPenalty;
	UINT8 ubSearchRange;
	UINT16 usOppDist;
	BOOLEAN fFriendsNearby;
	UINT16 usInHand, usGrenade;
	UINT8 ubOppsInRange, ubOppsAdjacent;
	BOOLEAN fSkipLocation;
	INT8	bPayloadPocket;
	INT8	bMaxLeft,bMaxRight,bMaxUp,bMaxDown,bXOffset,bYOffset;
	INT8	bPersOL, bPublOL;
	SOLDIERTYPE *pOpponent, *pFriend;
	static INT16	sExcludeTile[100]; // This array is for storing tiles that we have
	UINT8 ubNumExcludedTiles = 0;		// already considered, to prevent duplication of effort
	INT32	iTossRange;
	UINT8	ubSafetyMargin = 0;
	UINT8	ubDiff;
	INT8	bEndLevel;

	usInHand = pSoldier->inv[HANDPOS].usItem;
	usGrenade = NOTHING;

	if ( IsGrenadeLauncherAttached(&pSoldier->inv[HANDPOS]) )
		usInHand = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS]);

	if ( EXPLOSIVE_GUN( usInHand ) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"explosive gun");
		iTossRange = Weapon[ usInHand ].usRange / CELL_X_SIZE;
	}
	else
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"about to calcmaxtossrange");
		iTossRange = CalcMaxTossRange( pSoldier, usInHand, TRUE );
	}

	// if he's got a MORTAR in his hand, make sure he has a MORTARSHELL avail.
	if (Item[usInHand].mortar )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: buddy's got a mortar");
		//	bPayloadPocket = FindObj( pSoldier, MORTAR_SHELL );
		bPayloadPocket = FindLaunchable ( pSoldier, usInHand );
		if (bPayloadPocket == NO_SLOT)
		{
			return;	// no shells, can't fire the MORTAR
		}
		ubSafetyMargin = (UINT8)Explosive[ Item[ pSoldier->inv[bPayloadPocket].usItem ].ubClassIndex ].ubRadius;
	}
	// if he's got a GL in his hand, make sure he has some type of GRENADE avail.
	else if (Item[usInHand].grenadelauncher )
	{
		// use up pocket 2 first, they get left as drop items
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: buddy's got a GL");
		bPayloadPocket = FindGLGrenade( pSoldier );
		if (bPayloadPocket == NO_SLOT)
		{
			return;	// no grenades, can't fire the GLAUNCHER
		}
		ubSafetyMargin = (UINT8)Explosive[ Item[ pSoldier->inv[ bPayloadPocket ].usItem ].ubClassIndex ].ubRadius;
		usGrenade = pSoldier->inv[ bPayloadPocket ].usItem;
	}
	else if ( Item[usInHand].rocketlauncher )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: buddy's got a rocket launcher");
		// put in hand
		if ( Item[usInHand].singleshotrocketlauncher )
			// as C1
			ubSafetyMargin = (UINT8)Explosive[ Item[ C1 ].ubClassIndex ].ubRadius;
		else
		{
			bPayloadPocket = FindLaunchable ( pSoldier, usInHand );
			if (bPayloadPocket == NO_SLOT)
			{
				return;	// no ammo, can't fire
			}
			ubSafetyMargin = (UINT8)Explosive[ Item[ pSoldier->inv[bPayloadPocket].usItem ].ubClassIndex ].ubRadius;
		}
	}
	else if (Item[usInHand].cannon )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: buddy's got a tank");
		bPayloadPocket = FindLaunchable ( pSoldier, usInHand );
		if (bPayloadPocket == NO_SLOT)
		{
			return;	// no ammo, can't fire
		}
		ubSafetyMargin = (UINT8)Explosive[ Item[ pSoldier->inv[bPayloadPocket].usItem ].ubClassIndex ].ubRadius;


		//bPayloadPocket = FindObj( pSoldier, TANK_SHELL );
		//if (bPayloadPocket == NO_SLOT)
		//{
		//	return;	// no grenades, can't fire the GLAUNCHER
		//}
		//ubSafetyMargin = Explosive[ Item[ TANK_SHELL ].ubClassIndex ].ubRadius;

	}
	else
	{
		// else it's a plain old grenade, now in his hand
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: buddy's got a grenade");
		bPayloadPocket = HANDPOS;
		ubSafetyMargin = (UINT8)Explosive[ Item[ pSoldier->inv[ bPayloadPocket ].usItem ].ubClassIndex ].ubRadius;
		usGrenade = pSoldier->inv[ bPayloadPocket ].usItem;

		if ( Item[usGrenade].flare )
		{
			// JA2Gold: light isn't as nasty as explosives
			ubSafetyMargin /= 2;
		}
	}

	ubDiff = SoldierDifficultyLevel( pSoldier );

	// make a list of tiles one's friends are positioned in
	for (ubLoop = 0; ubLoop < guiNumMercSlots; ubLoop++)
	{

		pFriend = MercSlots[ubLoop];

		if ( !pFriend )
		{
			continue; // next soldier
		}

		if (pFriend->stats.bLife == 0)
		{
			continue;
		}

		/*
		// if this soldier is inactive, at base, on assignment, or dead
		if (!Menptr[ubLoop].bActive || !Menptr[ubLoop].bInSector || !Menptr[ubLoop].bLife)
		continue;			// next soldier
		*/

		// if this man is neutral / NOT on the same side, he's not a friend
		if (pFriend->aiData.bNeutral || (pSoldier->bSide != pFriend->bSide))
		{
			continue;			// next soldier
		}

		// active friend, remember where he is so that we DON'T blow him up!
		// this includes US, since we don't want to blow OURSELVES up either
		sFriendTile[ubFriendCnt] = pFriend->sGridNo;
		bFriendLevel[ubFriendCnt] = pFriend->pathing.bLevel;
		ubFriendCnt++;
	}

	//NumMessage("ubFriendCnt = ",ubFriendCnt);

	// make a list of tiles one's CURRENTLY SEEN opponents are positioned in
	for (ubLoop = 0; ubLoop < guiNumMercSlots; ubLoop++)
	{
		pOpponent = MercSlots[ubLoop];

		if (!pOpponent)
		{
			// inactive or not in sector
			continue;			// next soldier
		}

		if (!pOpponent->stats.bLife)
		{
			continue;			// next soldier
		}

		/*
		// if this soldier is inactive, at base, on assignment, or dead
		if (!pOpponent->bActive || !pOpponent->bInSector || !pOpponent->stats.bLife)
		continue;			// next soldier
		*/

		// if this man is neutral / on the same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
		{
			continue;			// next soldier
		}

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != 64)
		{
			continue;	// next opponent
		}


		bPersOL = pSoldier->aiData.bOppList[pOpponent->ubID];

		if ((Item[usInHand].mortar ) || (Item[usInHand].grenadelauncher ) )
		{
			bPublOL = gbPublicOpplist[pSoldier->bTeam][pOpponent->ubID];
			// allow long range firing, where target doesn't PERSONALLY see opponent
			if ((bPersOL != SEEN_CURRENTLY) && (bPublOL != SEEN_CURRENTLY))
			{
				continue;			// next soldier
			}
			// active KNOWN opponent, remember where he is so that we DO blow him up!
			sOpponentTile[ubOpponentCnt] = pOpponent->sGridNo;
			bOpponentLevel[ubOpponentCnt] = pOpponent->pathing.bLevel;
		}
		else
		{
			/*
			if (bPersOL != SEEN_CURRENTLY && bPersOL != SEEN_LAST_TURN)	 // if not in sight right now
			{
			continue;			// next soldier
			}
			*/
			if (bPersOL == SEEN_CURRENTLY)
			{
				// active KNOWN opponent, remember where he is so that we DO blow him up!
				sOpponentTile[ubOpponentCnt] = pOpponent->sGridNo;
				bOpponentLevel[ubOpponentCnt] = pOpponent->pathing.bLevel;
			}
			else if (bPersOL == SEEN_LAST_TURN)
			{
				// cheat; only allow throw if person is REALLY within 2 tiles of where last seen

				// JA2Gold: UB checks were screwed up
				/*
				if ( pOpponent->sGridNo == gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ] )
				{
				continue;
				}
				else */
				if ( !CloseEnoughForGrenadeToss( pOpponent->sGridNo, gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ] ) )
				{
					continue;
				}

				sOpponentTile[ubOpponentCnt] = gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ];
				bOpponentLevel[ubOpponentCnt] = gbLastKnownOppLevel[ pSoldier->ubID ][ pOpponent->ubID ];

				// JA2Gold: commented out
				/*
				if ( SpacesAway( pOpponent->sGridNo, gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ] ) > 2 )
				{
				continue;
				}
				sOpponentTile[ubOpponentCnt] = gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ];
				bOpponentLevel[ubOpponentCnt] = gbLastKnownOppLevel[ pSoldier->ubID ][ pOpponent->ubID ];
				*/
			}
			else if (bPersOL == HEARD_LAST_TURN )
			{
				// cheat; only allow throw if person is REALLY within 2 tiles of where last seen

				// screen out some ppl who have thrown
				if ( PreRandom( 3 ) == 0 )
				{
					continue;
				}

				// Weird detail: if the opponent is in the same location then they may have closed a door on us.
				// In which case, don't throw!

				// JA2Gold: UB checks were screwed up
				/*
				if ( pOpponent->sGridNo == gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ] )
				{
				continue;
				}
				else
				*/
				if ( !CloseEnoughForGrenadeToss( pOpponent->sGridNo, gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ] ) )
				{
					continue;
				}
				if ( !Item[usGrenade].flare && !pSoldier->aiData.bUnderFire && pSoldier->aiData.bShock == 0 )
				{
					continue;
				}
				sOpponentTile[ubOpponentCnt] = gsLastKnownOppLoc[ pSoldier->ubID ][ pOpponent->ubID ];
				bOpponentLevel[ubOpponentCnt] = gbLastKnownOppLevel[ pSoldier->ubID ][ pOpponent->ubID ];
			}
			else
			{
				continue;
			}

		}


		// also remember who he is (which soldier #)
		ubOpponentID[ubOpponentCnt] = pOpponent->ubID;

		// remember how relatively dangerous this opponent is (ignore my cover)
		iOppThreatValue[ubOpponentCnt] = CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier);

		ubOpponentCnt++;
	}

	//NumMessage("ubOpponentCnt = ",ubOpponentCnt);


	// this is try to minimize enemies wasting their (limited) toss attacks, with the exception of break lights
	if ( !Item[usGrenade].flare )
	{
		switch( ubDiff )
		{
		case 0:
		case 1:
			// they won't use them until they have 2+ opponents as long as half life left
			if ((ubOpponentCnt < 2) && (pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 2)))
			{
				return;
			}
			break;
		case 2:
			// they won't use them until they have 2+ opponents as long as 3/4 life left
			if ((ubOpponentCnt < 2) && (pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 4) * 3 ))
			{
				return;
			}
			break;
			break;
		default:
			break;
		}
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: about to initattacktype");
	InitAttackType(pBestThrow);	 // set all structure fields to defaults

	// look at the squares near each known opponent and try to find the one
	// place where a tossed projectile would do the most harm to the opponents
	// while avoiding one's friends
	for (ubLoop = 0; ubLoop < ubOpponentCnt; ubLoop++)
	{
		//NumMessage("Checking Guy#",ubOpponentID[ubLoop]);

		// search all tiles within 2 squares of this opponent
		ubSearchRange = MAX_TOSS_SEARCH_DIST;

		// determine maximum horizontal limits
		//bMaxLeft	= min(ubSearchRange,(sOpponentTile[ubLoop] % MAXCOL));
		bMaxLeft = ubSearchRange;
		//NumMessage("bMaxLeft = ",bMaxLeft);
		//bMaxRight = min(ubSearchRange,MAXCOL - ((sOpponentTile[ubLoop] % MAXCOL) + 1));
		bMaxRight = ubSearchRange;
		//NumMessage("bMaxRight = ",bMaxRight);

		// determine maximum vertical limits
		bMaxUp	= ubSearchRange;
		//NumMessage("bMaxUp = ",bMaxUp);
		bMaxDown = ubSearchRange;
		//NumMessage("bMaxDown = ",bMaxDown);

		// evaluate every tile for its opponent-damaging potential
		for (bYOffset = -bMaxUp; bYOffset <= bMaxDown; bYOffset++)
		{
			for (bXOffset = -bMaxLeft; bXOffset <= bMaxRight; bXOffset++)
			{
				//HandleMyMouseCursor(KEYBOARDALSO);

				// calculate the next potential gridno near this opponent
				sGridNo = sOpponentTile[ubLoop] + bXOffset + (MAXCOL * bYOffset);
				//NumMessage("Testing gridno #",sGridNo);

				// this shouldn't ever happen
				if ((sGridNo < 0) || (sGridNo >= GRIDSIZE))
				{
#ifdef BETAVERSION
					NumMessage("CalcBestThrow: ERROR - invalid gridno being tested ",sGridNo);
#endif
					continue;
				}

				if ( PythSpacesAway( pSoldier->sGridNo, sGridNo ) > iTossRange )
				{
					// can't throw there!
					return;
				}

				// if considering a gas/smoke grenade, check to see if there is such stuff already there!
				if ( usGrenade )
				{
					//					if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_SMOKE ||
					//						gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS) ||
					//						gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_MUSTARDGAS)
					if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_SMOKE ||
						gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS | MAPELEMENT_EXT_BURNABLEGAS) ||
						gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_MUSTARDGAS || gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_BURNABLEGAS)
					{
						continue;
					}

					//switch( usGrenade )
					//{
					//	case SMOKE_GRENADE:
					//	case GL_SMOKE_GRENADE:
					//		// skip smoke
					//		if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_SMOKE )
					//		{
					//			continue;
					//		}
					//		break;
					//	case TEARGAS_GRENADE:
					//		// skip tear and mustard gas
					//		if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS) )
					//		{
					//			continue;
					//		}
					//		break;
					//	case MUSTARD_GRENADE:
					//		// skip mustard gas
					//		if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bOpponentLevel[ubLoop] ] & MAPELEMENT_EXT_MUSTARDGAS )
					//		{
					//			continue;
					//		}
					//		break;
					//	default:
					//		break;
					//}
				}

				fSkipLocation = FALSE;
				// Check to see if we have considered this tile before:
				for (ubLoop2 = 0; ubLoop2 < ubNumExcludedTiles; ubLoop2++)
				{
					if (sExcludeTile[ubLoop2] == sGridNo)
					{
						// already checked!
						fSkipLocation = TRUE;
						break;
					}
				}
				if (fSkipLocation)
				{
					continue;
				}

				// calculate minimum action points required to throw at this gridno
				ubMinAPcost = MinAPsToAttack(pSoldier,sGridNo,ADDTURNCOST);
				DebugMsg(TOPIC_JA2 , DBG_LEVEL_3 , String("MinAPcost to attack = %d",ubMinAPcost));

				// if we don't have enough APs left to throw even without aiming
				DebugMsg(TOPIC_JA2 , DBG_LEVEL_3 , String("Soldier's action points = %d",pSoldier->bActionPoints ));
				if (ubMinAPcost > pSoldier->bActionPoints)
					continue;				// next gridno

				// check whether there are any friends standing near this gridno
				fFriendsNearby = FALSE;

				for (ubLoop2 = 0; ubLoop2 < ubFriendCnt; ubLoop2++)
				{
					if ( (bFriendLevel[ubLoop2] == bOpponentLevel[ubLoop]) && ( PythSpacesAway(sFriendTile[ubLoop2],sGridNo) <= ubSafetyMargin ) )
					{
						//NumMessage("Friend too close: at gridno",sFriendTile[ubLoop2]);
						fFriendsNearby = TRUE;
						break;		// don't bother checking any other friends
					}
				}

				if (fFriendsNearby)
					continue;		// this location is no good, move along now

				// Well this place shows some promise, evaluate its "damage potential"
				iTotalThreatValue = 0;
				ubOppsInRange = 0;
				ubOppsAdjacent = 0;
				// skip this location unless it's right on top of an enemy or
				// adjacent to more than 1
				fSkipLocation = TRUE;

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: checking opponents");
				for (ubLoop2 = 0; ubLoop2 < ubOpponentCnt; ubLoop2++)
				{
					usOppDist = PythSpacesAway( sOpponentTile[ubLoop2], sGridNo );

					// if this opponent is close enough to the target gridno
					if (usOppDist <= 3)
					{
						// start with this opponents base threat value
						iThreatValue = iOppThreatValue[ubLoop2];

						// estimate how much damage this tossed item would do to him
						iEstDamage = EstimateThrowDamage(pSoldier,bPayloadPocket,MercPtrs[ubOpponentID[ubLoop2]],sGridNo);
						//NumMessage("THROW EstDamage = ",iEstDamage);

						if (usOppDist)
						{
							// reduce the estimated damage for his distance from gridno
							// use 100% at range 0, 80% at range 1, and 60% at range 2, etc.
							iEstDamage = (iEstDamage * (100 - (20 * usOppDist))) / 100;
							//NumMessage("THROW reduced usEstDamage = ",usEstDamage);
						}
						else
						{
							// throwing right on top of someone... always consider this
							fSkipLocation = FALSE;
						}

						// add the product of his threat value & damage caused to total
						iTotalThreatValue += (iThreatValue * iEstDamage);

						// only count opponents still standing worth shooting at (in range)
						if (Menptr[ ubOpponentID[ubLoop2] ].stats.bLife >= OKLIFE)
						{
							ubOppsInRange++;
							if (usOppDist < 2)
							{
								ubOppsAdjacent++;
								if (ubOppsAdjacent > 1 || Item[usGrenade].flare )
								{
									fSkipLocation = FALSE;
									// add to exclusion list so we don't consider it again
								}
							}
						}
					}
				}

				// JA2Gold
				if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
				{
					if (fSkipLocation)
					{
						continue;
					}
				}
				else
				{
					if( ubOppsInRange == 0 )
					{
						continue;
					}

					//Only use it if we are in a surface sector ( basement will be hard enough, plus more chances of mercs being clumped together )
					else if( gbWorldSectorZ > 0 && fSkipLocation )
					{
						continue;
					}
				}

				// JA2Gold change to >=
				if (ubOppsAdjacent >= 1 && ubNumExcludedTiles < 100)
				{
					// add to exclusion list so we don't calculate for this location twice
					sExcludeTile[ubNumExcludedTiles] = sGridNo;
					ubNumExcludedTiles++;
				}

				// calculate chance to get through any cover to this gridno
				//ubChanceToGetThrough = ChanceToGetThrough(pSoldier,sGridNo,NOTFAKE,ACTUAL,TESTWALLS,9999,M9PISTOL,NOT_FOR_LOS);

				if ( EXPLOSIVE_GUN( usInHand ) )
				{
					ubChanceToGetThrough = AISoldierToLocationChanceToGetThrough( pSoldier, sGridNo, bOpponentLevel[ubLoop], 0 );
					if ( ubChanceToGetThrough == 0)
					{
						continue; // next gridno
					}
				}
				else
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: checking chance for launcher to beat cover");
					ubChanceToGetThrough = 100 * CalculateLaunchItemChanceToGetThrough( pSoldier, &(pSoldier->inv[ bPayloadPocket ] ), sGridNo, bOpponentLevel[ubLoop], 0, &sEndGridNo, TRUE, &bEndLevel, FALSE );				//NumMessage("Chance to get through = ",ubChanceToGetThrough);
					// if we can't possibly get through all the cover
					if (ubChanceToGetThrough == 0 )
					{
						if ( bEndLevel == bOpponentLevel[ubLoop] && ubSafetyMargin > 1 )
						{
							// rate "chance of hitting" according to how far away this is from the target
							// but keeping in mind that we don't want to hit far, subtract 1 from the radius here
							// to penalize being far from the target
							uiPenalty = 100 * PythSpacesAway( sGridNo, sEndGridNo ) / (ubSafetyMargin - 1);
							if ( uiPenalty < 100 )
							{
								ubChanceToGetThrough = 100 - (UINT8) uiPenalty;
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
				}

				//NumMessage("Total Threat Value = ",iTotalThreatValue);
				//NumMessage("Opps in Range = ",ubOppsInRange);

				// this is try to minimize enemies wasting their (few) mortar shells or LAWs
				// they won't use them on less than 2 targets as long as half life left
				if ((Item[usInHand].mortar || Item[usInHand].rocketlauncher ) && (ubOppsInRange < 2) &&
					(pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 2)))
				{
					continue;				// next gridno
				}

				// calculate the maximum possible aiming time
				ubMaxPossibleAimTime = min(AllowedAimingLevels(pSoldier),pSoldier->bActionPoints - ubMinAPcost);
				DebugMsg(TOPIC_JA2 , DBG_LEVEL_3 , String("Max Possible Aim Time = %d",ubMaxPossibleAimTime ));

				// calc next attack's minimum AP cost (excludes readying & turning)

				// since grenades & shells are far too valuable to waste, ALWAYS
				// aim for the longest time possible!

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: checking chance to hit");
				if ( EXPLOSIVE_GUN( usInHand ) )
				{
					ubRawAPCost = MinAPsToShootOrStab( pSoldier, sGridNo, FALSE);
					ubChanceToHit = (UINT8) AICalcChanceToHitGun(pSoldier, sGridNo, ubMaxPossibleAimTime, AIM_SHOT_TORSO );
				}
				else
				{
					// NB grenade launcher is NOT a direct fire weapon!
					ubRawAPCost = (UINT8) MinAPsToThrow( pSoldier, sGridNo, FALSE );
					DebugMsg(TOPIC_JA2 , DBG_LEVEL_3 , String("Raw AP Cost = %d",ubRawAPCost ));
					ubChanceToHit = (UINT8) CalcThrownChanceToHit( pSoldier, sGridNo, ubMaxPossibleAimTime, AIM_SHOT_TORSO );
					DebugMsg(TOPIC_JA2 , DBG_LEVEL_3 , String("Chance to hit = %d",ubChanceToHit ));
				}

				// mortars are inherently quite inaccurate, don't get proximity bonus
				// rockets can go right past people too so...
				if (!Item[usInHand].mortar && EXPLOSIVE_GUN( usInHand ) )
				{
					// special 50% to Hit bonus: this reflects that even if a tossed item
					// misses by a bit, it's still likely to affect the intended target(s)
					ubChanceToHit += (ubChanceToHit / 2);

					// still can't let it go over 100% chance, though
					if (ubChanceToHit > 100)
					{
						ubChanceToHit = 100;
					}
				}

				if (ubRawAPCost < 1)
					ubRawAPCost = ubMinAPcost;

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("calcbestthrow: checking hit rate: ubRawAPCost %d, ubMaxPossibleAimTime %d", ubRawAPCost, ubMaxPossibleAimTime ));
				iHitRate = (pSoldier->bActionPoints * ubChanceToHit) / (ubRawAPCost + ubMaxPossibleAimTime);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: checked hit rate");
				//NumMessage("iHitRate = ",iHitRate);

				// calculate chance to REALLY hit: throw accurately AND get past cover
				ubChanceToReallyHit = (ubChanceToHit * ubChanceToGetThrough) / 100;

				// if we can't REALLY hit at all
				if (ubChanceToReallyHit == 0)
					continue;				// next gridno

				// calculate the combined "attack value" for this opponent
				// maximum possible attack value here should be about 140 million
				// typical attack value here should be about 500 thousand
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow: checking attack value");
				iAttackValue = (iHitRate * ubChanceToReallyHit * iTotalThreatValue) / 1000;
				//NumMessage("THROW AttackValue = ",iAttackValue / 1000);

				// unlike SHOOTing and STABbing, find strictly the highest attackValue
				if (iAttackValue > pBestThrow->iAttackValue)
				{
#ifdef DEBUGATTACKS
					DebugAI( String( "CalcBestThrow: new best attackValue vs %d = %d\n",ubOpponentID[ubLoop],iAttackValue ) );
#endif

					// OOOF!	That was a lot of work!	But we've got a new best target!
					pBestThrow->ubPossible			= TRUE;
					pBestThrow->ubOpponent			= ubOpponentID[ubLoop];
					pBestThrow->ubAimTime			= ubMaxPossibleAimTime;
					pBestThrow->ubChanceToReallyHit = ubChanceToReallyHit;
					pBestThrow->sTarget			 = sGridNo;
					pBestThrow->iAttackValue		= iAttackValue;
					pBestThrow->ubAPCost			= ubMinAPcost + ubMaxPossibleAimTime;
					pBestThrow->bTargetLevel				= bOpponentLevel[ubLoop];

					//sprintf(tempstr,"new best THROW AttackValue = %d at grid #%d",iAttackValue/100000,gridno);
					//PopMessage(tempstr);
				}
			}
		}
	}

	// this is try to minimize enemies wasting their (limited) toss attacks:
	switch( ubDiff )
	{
	case 0:
	case 1:
		// don't use unless have a 70% chance to hit
		if (pBestThrow->ubChanceToReallyHit < 70)
		{
			pBestThrow->ubPossible = FALSE;
		}
		break;
	case 2:
	case 3:
	case 4:
		// don't use unless have a 50% chance to hit
		if (pBestThrow->ubChanceToReallyHit < 50)
		{
			pBestThrow->ubPossible = FALSE;
		}
		break;
	default:
		break;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcbestthrow done");
}

void CalcBestStab(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestStab, BOOLEAN fBladeAttack )
{
	UINT32 uiLoop;
	INT32 iAttackValue;
	INT32 iThreatValue,iHitRate,iBestHitRate,iPercentBetter, iEstDamage;
	BOOLEAN fSurpriseStab;
	INT16 ubRawAPCost,ubMinAPCost,ubMaxPossibleAimTime = 0;
	INT16 ubChanceToReallyHit = 0;
	INT16 ubAimTime,ubChanceToHit,ubBestAimTime;
	SOLDIERTYPE *pOpponent;
	UINT16 usTrueMovementMode;
	INT16 ubBestChanceToHit;
	InitAttackType(pBestStab);		// set all structure fields to defaults

	pSoldier->usAttackingWeapon = pSoldier->inv[HANDPOS].usItem;

	// temporarily make this guy run so we get a proper AP cost value
	// from CalcTotalAPsToAttack
	usTrueMovementMode = pSoldier->usUIMovementMode;
	pSoldier->usUIMovementMode = RUNNING;

	// determine which attack against which target has the greatest attack value

	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpponent || !pOpponent->stats.bLife)
			continue;			// next merc

		// if this man is neutral / on the same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide) )
			continue;			// next merc

		// if this opponent is not currently in sight (ignore known but unseen!)
		if (pSoldier->aiData.bOppList[pOpponent->ubID] != SEEN_CURRENTLY)
			continue;			// next merc

		// if this opponent is not on the same level
		if (pSoldier->pathing.bLevel != pOpponent->pathing.bLevel)
			continue;			// next merc

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != 64)
			continue;	// next opponent

#ifdef DEBUGATTACKS
		DebugAI( String( "%s can see %s\n",pSoldier->name,ExtMen[pOpponent->ubID].name ) );
#endif

		// calculate minimum action points required to stab at this opponent
		ubMinAPCost = CalcTotalAPsToAttack( pSoldier,pOpponent->sGridNo,ADDTURNCOST, 0 );

		//ubMinAPCost = MinAPsToAttack(pSoldier,pOpponent->sGridNo,ADDTURNCOST);
		//NumMessage("MinAPcost to stab this opponent = ",ubMinAPCost);

		// Human: if I don't have enough APs left to get there & stab at this guy, skip 'im.
		// Monster:	I'll do an extra check later on to see if I can reach the guy this turn.

		// if 0 is returned then no path!
		if ( ubMinAPCost > pSoldier->bActionPoints || ubMinAPCost == 0 )
		{
			continue;
			/*
			if ( CREATURE_OR_BLOODCAT( pSoldier ) )
			{
			// hardcode ubMinAPCost so that aiming time is 0 and can start move to stab
			// at any time
			ubMinAPCost = pSoldier->bActionPoints;
			}
			else
			{
			continue;			// next merc
			}
			*/
		}

		//KeepInterfaceGoing();

		// calc next attack's minimum stabbing cost (excludes movement & turning)
		//ubRawAPCost = MinAPsToShootOrStab(pSoldier,pOpponent->sGridNo, FALSE) - APBPConstants[AP_CHANGE_TARGET];
		ubRawAPCost = MinAPsToAttack(pSoldier,pOpponent->sGridNo, FALSE) - APBPConstants[AP_CHANGE_TARGET];
		//NumMessage("ubRawAPCost to stab this opponent = ",ubRawAPCost);


		// determine if this is a surprise stab (must be next to opponent & unseen)
		fSurpriseStab = FALSE;		// assume it is not a surprise stab

		// if opponent doesn't see the attacker
		if (pOpponent->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY)
		{
			// and he's only one space away from attacker
			if (SpacesAway(pSoldier->sGridNo,pOpponent->sGridNo) == 1)
			{
				fSurpriseStab = TRUE;	// we got 'im lined up where we want 'im!
			}
		}


		iBestHitRate = 0;					 // reset best hit rate to minimum

		// calculate the maximum possible aiming time
		ubMaxPossibleAimTime = min(AllowedAimingLevels(pSoldier),pSoldier->bActionPoints - ubMinAPCost);
		//NumMessage("Max Possible Aim Time = ",ubMaxPossibleAimTime);

		// consider the various aiming times
		for (ubAimTime = APBPConstants[AP_MIN_AIM_ATTACK]; ubAimTime <= ubMaxPossibleAimTime; ubAimTime++)
		{
			//HandleMyMouseCursor(KEYBOARDALSO);

			//NumMessage("ubAimTime = ",ubAimTime);

			if (!fSurpriseStab)
			{
				if (fBladeAttack)
				{
					ubChanceToHit = (INT16) CalcChanceToStab(pSoldier,pOpponent,ubAimTime);
				}
				else
				{
					ubChanceToHit = (INT16) CalcChanceToPunch(pSoldier,pOpponent,ubAimTime);
				}
			}
			else
				// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
				// ubChanceToHit = MAXCHANCETOHIT;
				ubChanceToHit = gGameExternalOptions.ubMaximumCTH;
			//NumMessage("chance to Hit = ",ubChanceToHit);

			//sprintf(tempstr,"Vs. %s, at AimTime %d, ubChanceToHit = %d",ExtMen[pOpponent->ubID].name,ubAimTime,ubChanceToHit);
			//PopMessage(tempstr);

			if (ubRawAPCost < 1)
				ubRawAPCost = ubMinAPCost;

			iHitRate = (pSoldier->bActionPoints * ubChanceToHit) / (ubRawAPCost + ubAimTime);
			//NumMessage("hitRate = ",iHitRate);

			// if aiming for this amount of time produces a better hit rate
			if (iHitRate > iBestHitRate)
			{
				iBestHitRate = iHitRate;
				ubBestAimTime = ubAimTime;
				ubBestChanceToHit = ubChanceToHit;
			}
		}


		// if we can't get any kind of hit rate at all
		if (iBestHitRate == 0)
			continue;			// next opponent

		// stabs are not affected by cover, so the chance to REALLY hit is the same
		ubChanceToReallyHit = ubBestChanceToHit;

		// calculate this opponent's threat value
		// NOTE: ignore my cover!	By the time I run beside him I won't have any!
		iThreatValue = CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier);


		// estimate the damage this stab would do to this opponent
		iEstDamage = EstimateStabDamage(pSoldier,pOpponent,ubBestChanceToHit, fBladeAttack );
		//NumMessage("STAB EstDamage = ", iEstDamage);

		// calculate the combined "attack value" for this opponent
		// highest possible value before division should be about 1 billion...
		// normal value before division should be about 5 million...
		iAttackValue = ( iEstDamage * iBestHitRate * ubChanceToReallyHit * iThreatValue) / 1000;
		//NumMessage("STAB AttackValue = ",iAttackValue / 1000);

#ifdef DEBUGATTACKS
		DebugAI( String( "CalcBestStab: best AttackValue vs %d = %d\n",ubLoop,iAttackValue ) );
#endif

		// if we can hurt the guy, OR probably not, but at least it's our best
		// chance to actually hit him and maybe scare him, knock him down, etc.
		if ((iAttackValue > 0) || (ubChanceToReallyHit > pBestStab->ubChanceToReallyHit))
		{
			// if there already was another viable target
			if (pBestStab->ubChanceToReallyHit > 0)
			{
				// OK, how does our chance to hit him compare to the previous best one?
				iPercentBetter = ((ubChanceToReallyHit * 100) / pBestStab->ubChanceToReallyHit) - 100;

				// if this chance to really hit is more than 50% worse, and the other
				// guy is conscious at all
				if ((iPercentBetter < -PERCENT_TO_IGNORE_THREAT) && (Menptr[pBestStab->ubOpponent].stats.bLife >= OKLIFE))
					// then stick with the older guy as the better target
					continue;

				// if this chance to really hit between 50% worse to 50% better
				if (iPercentBetter < PERCENT_TO_IGNORE_THREAT)
				{
					// then the one with the higher ATTACK VALUE is the better target
					if (iAttackValue < pBestStab->iAttackValue)
						// the previous guy is more important since he's more dangerous
						continue;			// next opponent
				}
			}

			// OOOF!	That was a lot of work!	But we've got a new best target!
			pBestStab->ubPossible			= TRUE;
			pBestStab->ubOpponent			= pOpponent->ubID;
			pBestStab->ubAimTime			= ubBestAimTime;
			pBestStab->ubChanceToReallyHit = ubChanceToReallyHit;
			pBestStab->sTarget			 = pOpponent->sGridNo;
			pBestStab->bTargetLevel		= pOpponent->pathing.bLevel;
			pBestStab->iAttackValue		= iAttackValue;
			pBestStab->ubAPCost			= ubMinAPCost + ubBestAimTime;
		}
	}

	pSoldier->usUIMovementMode = usTrueMovementMode;
}

void CalcTentacleAttack(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestStab )
{
	UINT32 uiLoop;
	INT32 iAttackValue;
	INT32 iThreatValue,iHitRate,iBestHitRate, iEstDamage;
	BOOLEAN fSurpriseStab;
	UINT8 ubMaxPossibleAimTime = 0;
	INT16 ubBestChanceToHit,ubAimTime,ubMinAPCost,ubChanceToHit,ubBestAimTime,ubRawAPCost;
	INT16 ubChanceToReallyHit = 0;
	SOLDIERTYPE *pOpponent;


	InitAttackType(pBestStab);		// set all structure fields to defaults

	pSoldier->usAttackingWeapon = pSoldier->inv[HANDPOS].usItem;

	// determine which attack against which target has the greatest attack value

	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpponent || !pOpponent->stats.bLife)
			continue;			// next merc

		// if this man is neutral / on the same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
			continue;			// next merc

		// if this opponent is not currently in sight (ignore known but unseen!)
		if (pSoldier->aiData.bOppList[pOpponent->ubID] != SEEN_CURRENTLY)
			continue;			// next merc

		// if this opponent is not on the same level
		if (pSoldier->pathing.bLevel != pOpponent->pathing.bLevel)
			continue;			// next merc

		// if this opponent is outside the range of our tentacles
		if ( GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, pOpponent->sGridNo ) > Weapon[ CREATURE_QUEEN_TENTACLES].usRange )
		{
			continue; // next merc
		}

#ifdef DEBUGATTACKS
		DebugAI( String( "%s can see %s\n",pSoldier->name,ExtMen[pOpponent->ubID].name ) );
#endif

		// calculate minimum action points required to stab at this opponent
		ubMinAPCost = CalcTotalAPsToAttack( pSoldier,pOpponent->sGridNo,ADDTURNCOST, 0 );
		//ubMinAPCost = MinAPsToAttack(pSoldier,pOpponent->sGridNo,ADDTURNCOST);
		//NumMessage("MinAPcost to stab this opponent = ",ubMinAPCost);


		// calc next attack's minimum stabbing cost (excludes movement & turning)
		//ubRawAPCost = MinAPsToShootOrStab(pSoldier,pOpponent->sGridNo, FALSE) - APBPConstants[AP_CHANGE_TARGET];
		ubRawAPCost = MinAPsToAttack(pSoldier,pOpponent->sGridNo, FALSE) - APBPConstants[AP_CHANGE_TARGET];
		//NumMessage("ubRawAPCost to stab this opponent = ",ubRawAPCost);

		// determine if this is a surprise stab (for tentacles, enemy must not see us, no dist limit)
		fSurpriseStab = FALSE;		// assume it is not a surprise stab

		// if opponent doesn't see the attacker
		if (pOpponent->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY)
		{
			fSurpriseStab = TRUE;	// we got 'im lined up where we want 'im!
		}

		iBestHitRate = 0;					 // reset best hit rate to minimum

		// calculate the maximum possible aiming time

		//ubMaxPossibleAimTime = min(AllowedAimingLevels(pSoldier),pSoldier->bActionPoints - ubMinAPCost);
		ubMaxPossibleAimTime = 0;
		//NumMessage("Max Possible Aim Time = ",ubMaxPossibleAimTime);

		// consider the various aiming times
		for (ubAimTime = APBPConstants[AP_MIN_AIM_ATTACK]; ubAimTime <= ubMaxPossibleAimTime; ubAimTime++)
		{
			//HandleMyMouseCursor(KEYBOARDALSO);

			//NumMessage("ubAimTime = ",ubAimTime);

			if (!fSurpriseStab)
			{
				ubChanceToHit = (INT16) CalcChanceToStab(pSoldier,pOpponent,ubAimTime);
			}
			else
				// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
				// ubChanceToHit = MAXCHANCETOHIT;
				ubChanceToHit = gGameExternalOptions.ubMaximumCTH;
			//NumMessage("chance to Hit = ",ubChanceToHit);

			//sprintf(tempstr,"Vs. %s, at AimTime %d, ubChanceToHit = %d",ExtMen[pOpponent->ubID].name,ubAimTime,ubChanceToHit);
			//PopMessage(tempstr);

			if (ubRawAPCost < 1)
				ubRawAPCost = ubMinAPCost;

			iHitRate = (pSoldier->bActionPoints * ubChanceToHit) / (ubRawAPCost + ubAimTime);
			//NumMessage("hitRate = ",iHitRate);

			// if aiming for this amount of time produces a better hit rate
			if (iHitRate > iBestHitRate)
			{
				iBestHitRate = iHitRate;
				ubBestAimTime = ubAimTime;
				ubBestChanceToHit = ubChanceToHit;
			}
		}

		// if we can't get any kind of hit rate at all
		if (iBestHitRate == 0)
			continue;			// next opponent

		// stabs are not affected by cover, so the chance to REALLY hit is the same
		ubChanceToReallyHit = ubBestChanceToHit;

		// calculate this opponent's threat value
		// NOTE: ignore my cover!	By the time I run beside him I won't have any!
		iThreatValue = CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier);

		// estimate the damage this stab would do to this opponent
		iEstDamage = EstimateStabDamage(pSoldier,pOpponent,ubBestChanceToHit, TRUE );
		//NumMessage("STAB EstDamage = ", iEstDamage);

		// calculate the combined "attack value" for this opponent
		// highest possible value before division should be about 1 billion...
		// normal value before division should be about 5 million...
		iAttackValue = ( iEstDamage * iBestHitRate * ubChanceToReallyHit * iThreatValue) / 1000;
		//NumMessage("STAB AttackValue = ",iAttackValue / 1000);

#ifdef DEBUGATTACKS
		DebugAI( String( "CalcBestStab: best AttackValue vs %d = %d\n",ubLoop,iAttackValue ) );
#endif

		// if we can hurt the guy, OR probably not, but at least it's our best
		// chance to actually hit him and maybe scare him, knock him down, etc.
		if (iAttackValue > 0)
		{
			// OOOF!	That was a lot of work!	But we've got a new best target!
			pBestStab->ubPossible			= TRUE;
			pBestStab->ubOpponent			= pOpponent->ubID;
			pBestStab->ubAimTime			= ubBestAimTime;
			pBestStab->ubChanceToReallyHit = ubChanceToReallyHit;
			pBestStab->sTarget			 = pOpponent->sGridNo;
			pBestStab->bTargetLevel		= pOpponent->pathing.bLevel;

			// ADD this target's attack value to our TOTAL...
			pBestStab->iAttackValue				+= iAttackValue;

			pBestStab->ubAPCost			= ubMinAPCost + ubBestAimTime;

		}
	}
}

UINT8 NumMercsCloseTo( INT16 sGridNo, UINT8 ubMaxDist )
{
	INT8						bNumber = 0;
	UINT32					uiLoop;
	SOLDIERTYPE *		pSoldier;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE )
		{
			if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= ubMaxDist)
			{
				bNumber++;
			}
		}
	}

	return( bNumber );
}

INT32 EstimateShotDamage(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, INT16 ubChanceToHit)
{
	INT32 iRange,iMaxRange,iPowerLost;
	INT32 iDamage;
	UINT8 ubBonus;
	INT32 iHeadProt = 0, iTorsoProt = 0, iLegProt = 0;
	INT32 iTotalProt;
	//INT8 bPlatePos;
	UINT8	ubAmmoType;
	UINT16 usItem;
	OBJECTTYPE *pObj;

	/*
	if ( pOpponent->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
	// only thing that can damage vehicles is HEAP rounds?
	return( 0 );
	}
	*/

	pObj = &(pSoldier->inv[pSoldier->ubAttackingHand]);
	usItem = pObj->usItem;

	if ( Item[ usItem ].usItemClass & IC_THROWING_KNIFE )
	{
		ubAmmoType = AMMO_KNIFE;
	}
	else
	{
		ubAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;
	}

	// calculate distance to target, obtain his gun's maximum range rating

	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, pOpponent->sGridNo );
	iMaxRange = Weapon[usItem].usRange;

	// bullet loses speed and penetrating power, 50% loss per maximum range
	iPowerLost = ((50 * iRange) / iMaxRange);

	// up to 50% extra impact for making particularly accurate successful shots
	ubBonus = ubChanceToHit / 4;		// /4 is really /2 and /2 again

	iDamage = ((GetDamage(pObj)) * (100 - iPowerLost + ubBonus) / 100) ;

	//NumMessage("Pre-protection damage: ",damage);

	// if opponent is wearing a helmet
	if (pOpponent->inv[HELMETPOS].usItem)
	{
		iHeadProt += (INT32) Armour[Item[pOpponent->inv[HELMETPOS].usItem].ubClassIndex].ubProtection *
			(INT32) pOpponent->inv[HELMETPOS][0]->data.objectStatus / 100;
	}

	// if opponent is wearing a protective vest
	if ( !AmmoTypes[ubAmmoType].ignoreArmour )
	{
		// monster spit and knives ignore kevlar vests
		if (pOpponent->inv[VESTPOS].usItem)
		{
			iTorsoProt += (INT32) Armour[Item[pOpponent->inv[VESTPOS].usItem].ubClassIndex].ubProtection *
				(INT32) pOpponent->inv[VESTPOS][0]->data.objectStatus / 100;
		}
	}

	// check for ceramic plates; these do affect monster spit
	for (attachmentList::iterator iter = pOpponent->inv[VESTPOS][0]->attachments.begin(); iter != pOpponent->inv[VESTPOS][0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
		{
			iTorsoProt += (INT32) Armour[Item[iter->usItem].ubClassIndex].ubProtection *
				(INT32) (*iter)[0]->data.objectStatus / 100;
		}
	}

	// if opponent is wearing armoured leggings (LEGPOS)
	if ( !AmmoTypes[ubAmmoType].ignoreArmour )
	{	// monster spit and knives ignore kevlar leggings
		if (pOpponent->inv[LEGPOS].usItem)
		{
			iLegProt += (INT32) Armour[Item[pOpponent->inv[LEGPOS].usItem].ubClassIndex].ubProtection *
				(INT32) pOpponent->inv[LEGPOS][0]->data.objectStatus / 100;
		}
	}

	// 15% of all shots are to the head, 80% are to the torso.	Calc. avg. prot.
	// NB: make AI guys shoot at head 15% of time, 5% of time at legs

	iTotalProt = ((15 * iHeadProt) + (75 * iTorsoProt) + 5 * iLegProt) / 100;
	iTotalProt = (INT32) (iTotalProt * AmmoTypes[ubAmmoType].armourImpactReductionMultiplier / max(1,AmmoTypes[ubAmmoType].armourImpactReductionDivisor) );
	//switch (ubAmmoType)
	//{
	//	case AMMO_HP:
	//		iTotalProt = AMMO_ARMOUR_ADJUSTMENT_HP( iTotalProt );
	//		break;
	//	case AMMO_AP:
	//		iTotalProt = AMMO_ARMOUR_ADJUSTMENT_AP( iTotalProt );
	//		break;
	//	case AMMO_SUPER_AP:
	//		iTotalProt = AMMO_ARMOUR_ADJUSTMENT_SAP( iTotalProt );
	//		break;
	//	default:
	//		break;
	//}

	iDamage -= iTotalProt;
	//NumMessage("After-protection damage: ",damage);

	//if (ubAmmoType == AMMO_HP)
	//{
	//	// increase after-armour damage
	//	iDamage = AMMO_DAMAGE_ADJUSTMENT_HP( iDamage );
	//}
	iDamage = (INT32)(iDamage * AmmoTypes[ubAmmoType].afterArmourDamageMultiplier / max(1,AmmoTypes[ubAmmoType].afterArmourDamageDivisor) ) ;

	if (AmmoTypes[ubAmmoType].monsterSpit )
	{
		// cheat and emphasize shots
		//iDamage = (iDamage * 15) / 10;
		switch( usItem )
		{
			// explosive damage is 100-200% that of the rated, so multiply by 3/2s here
		case CREATURE_QUEEN_SPIT: //TODO: Madd - remove the hardcoding here
			iDamage += ( 3 * Explosive[ Item[ LARGE_CREATURE_GAS ].ubClassIndex ].ubDamage * NumMercsCloseTo( pOpponent->sGridNo, (UINT8)Explosive[ Item[ LARGE_CREATURE_GAS ].ubClassIndex ].ubRadius ) ) / 2;
			break;
		case CREATURE_OLD_MALE_SPIT:
			iDamage += ( 3 * Explosive[ Item[ SMALL_CREATURE_GAS ].ubClassIndex ].ubDamage * NumMercsCloseTo( pOpponent->sGridNo, (UINT8)Explosive[ Item[ SMALL_CREATURE_GAS ].ubClassIndex	].ubRadius ) ) / 2;
			break;
		default:
			iDamage += ( 3 * Explosive[ Item[ VERY_SMALL_CREATURE_GAS ].ubClassIndex ].ubDamage * NumMercsCloseTo( pOpponent->sGridNo, (UINT8)Explosive[ Item[ VERY_SMALL_CREATURE_GAS ].ubClassIndex	].ubRadius ) ) / 2;
			break;
		}
	}

	if (iDamage < 1)
		iDamage = 1;	// assume we can do at LEAST 1 pt minimum damage

	return( iDamage );
}

INT32 EstimateThrowDamage( SOLDIERTYPE *pSoldier, UINT8 ubItemPos, SOLDIERTYPE *pOpponent, INT16 sGridno )
{
	UINT16	ubExplosiveIndex;
	INT32	iExplosDamage, iBreathDamage, iArmourAmount, iDamage = 0;
	INT8	bSlot;


	if( pSoldier == NULL || pOpponent == NULL || ubItemPos > pSoldier->inv.size() || sGridno > NUMBEROFTILES )
		return 0;

	if( pSoldier->inv[ubItemPos].exists() == false )
		return 0;


	//switch ( pSoldier->inv[ ubItemPos ].usItem )
	//{
	//case GL_SMOKE_GRENADE:
	//case SMOKE_GRENADE:
	//	// Don't want to value throwing smoke very much.	This value is based relative
	//	// to the value for knocking somebody down, which was giving values that were
	//	// too high
	//	return( 5 );
	//case RPG7:
	//case ROCKET_LAUNCHER:
	//	ubExplosiveIndex = Item[ C1 ].ubClassIndex;
	//	break;
	//default:
	if ( Item[pSoldier->inv[ ubItemPos ].usItem].singleshotrocketlauncher )
		ubExplosiveIndex = Item[ C1 ].ubClassIndex;
	else if ( Item[pSoldier->inv[ ubItemPos ].usItem].rocketlauncher || Item[pSoldier->inv[ ubItemPos ].usItem].grenadelauncher || Item[pSoldier->inv[ ubItemPos ].usItem].mortar )
	{
		OBJECTTYPE* pAttachment = FindLaunchableAttachment(&pSoldier->inv[ ubItemPos ],pSoldier->inv[ ubItemPos ].usItem ) ;
		if ( pAttachment )
			ubExplosiveIndex = Item[pAttachment->usItem].ubClassIndex;
		else
			return 0;
	}
	else if ( Explosive[Item[pSoldier->inv[ ubItemPos ].usItem].ubClassIndex].ubType == EXPLOSV_SMOKE )
		return 5;
	else
		ubExplosiveIndex = Item[ pSoldier->inv[ubItemPos].usItem ].ubClassIndex;

	//		break;
	//}
	// JA2Gold: added
	if ( Item[pSoldier->inv[ubItemPos].usItem].flare )
	{
		return( 5 * ( LightTrueLevel( pOpponent->sGridNo, pOpponent->pathing.bLevel ) - NORMAL_LIGHTLEVEL_DAY ) );
	}


	iExplosDamage = ( ( (INT32) Explosive[ ubExplosiveIndex ].ubDamage ) * 3) / 2;
	iBreathDamage = ( ( (INT32) Explosive[ ubExplosiveIndex ].ubStunDamage ) * 5) / 4;

	if ( Explosive[ ubExplosiveIndex ].ubType == EXPLOSV_TEARGAS || Explosive[ ubExplosiveIndex ].ubType == EXPLOSV_MUSTGAS )
	{
		// if target gridno is outdoors (where tear gas lasts only 1-2 turns)
		if (gpWorldLevelData[sGridno].ubTerrainID != FLAT_FLOOR)
			iBreathDamage /= 2;		// reduce effective breath damage by 1/2

		bSlot = FindObj( pOpponent, GASMASK );
		if (bSlot == HEAD1POS || bSlot == HEAD2POS)
		{
			// take condition of the gas mask into account - it could be leaking
			iBreathDamage = (iBreathDamage * (100 - pOpponent->inv[bSlot][0]->data.objectStatus)) / 100;
			//NumMessage("damage after GAS MASK: ",iBreathDamage);
		}

	}
	if ( Explosive[ ubExplosiveIndex ].ubType == EXPLOSV_BURNABLEGAS )
	{
		// if target gridno is outdoors (where tear gas lasts only 1-2 turns)
		if (gpWorldLevelData[sGridno].ubTerrainID != FLAT_FLOOR)
			iBreathDamage /= 2;		// reduce effective breath damage by 1/2
	}
	else if (iExplosDamage)
	{
		// EXPLOSION DAMAGE is spread amongst locations
		iArmourAmount = ArmourVersusExplosivesPercent( pSoldier );
		iExplosDamage -= iExplosDamage * iArmourAmount / 100;

		if (iExplosDamage < 1)
			iExplosDamage = 1;
	}

	// if this opponent is standing
	if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
	{
		// 15 pt. flat bonus for knocking him down (for ANY type of explosion)
		iDamage += 15;
	}

	if ( pOpponent->bBreath < OKBREATH || AM_A_ROBOT( pOpponent ) )
	{
		// don't bother to count breath damage against people already down
		iBreathDamage = 0;
	}

	// estimate combined "damage" value considering combined health/breath damage
	iDamage += iExplosDamage + (iBreathDamage / 3);

	// approximate chance of the grenade going off (Ian's formulas are too funky)
	// then use that to reduce the expected damage because thing may not blow!
	iDamage = (iDamage * pSoldier->inv[ubItemPos][0]->data.objectStatus) / 100;

	// if the target gridno is in water, grenade may not blow (guess 50% of time)
	/*
	if (TTypeList[Grid[gridno].land] >= LAKETYPE)
	iDamage /= 2;
	*/

	return( iDamage);
}

INT32 EstimateStabDamage( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent,
						INT16 ubChanceToHit, BOOLEAN fBladeAttack )
{
	INT32 iImpact, iFluke, iBonus;

	UINT16	usItem;

	usItem = pSoldier->inv[HANDPOS].usItem;

	if (fBladeAttack)
	{
		iImpact = GetDamage(&pSoldier->inv[HANDPOS]);
		iImpact += EffectiveStrength( pSoldier ) / 20; // 0 to 5 for strength, adjusted by damage taken
	}
	else
	{
		// NB martial artists don't get a bonus for using brass knuckles!
		if (pSoldier->usAttackingWeapon && !( HAS_SKILL_TRAIT( pSoldier, MARTIALARTS ) ) )
		{
			iImpact = GetDamage(&pSoldier->inv[HANDPOS]);
		}
		else
		{
			// base HTH damage
			iImpact = 5;
		}
		iImpact += EffectiveStrength( pSoldier ) / 5; // 0 to 20 for strength, adjusted by damage taken
	}


	iImpact += (pSoldier->stats.bExpLevel / 2); // 0 to 4 for level

	iFluke = 0;
	iBonus = ubChanceToHit / 4;				// up to 50% extra impact for accurate attacks

	iImpact = iImpact * (100 + iFluke + iBonus) / 100;

	if (!fBladeAttack)
	{
		// add bonuses for hand-to-hand and martial arts
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIALARTS ) )
		{
			iImpact = iImpact * (100 + gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pSoldier, MARTIALARTS ) ) / 100;
		}
		if ( HAS_SKILL_TRAIT( pSoldier, HANDTOHAND ) )
		{
			iImpact = iImpact * (100 + gbSkillTraitBonus[HANDTOHAND] * NUM_SKILL_TRAITS( pSoldier, HANDTOHAND ) ) / 100;
		}
		// Here, if we're doing a bare-fisted attack,
		// we want to pay attention just to wounds inflicted
		iImpact = iImpact / PUNCH_REAL_DAMAGE_PORTION;
	}

	if (iImpact < 1)
	{
		iImpact = 1;
	}

	return( iImpact );
}

INT8 TryToReload( SOLDIERTYPE * pSoldier )
{
	INT8		bSlot;
	WEAPONTYPE *pWeapon;
	OBJECTTYPE *pObj, *pObj2;

	// HEADROCK HAM 3.3: Attempt to reload now takes magazine type into account. Prefernace will be given to magazines of similar type.
	pObj = &(pSoldier->inv[HANDPOS]);
	pWeapon = &(Weapon[pSoldier->inv[HANDPOS].usItem]);
	bSlot = FindAmmo( pSoldier, pWeapon->ubCalibre, pWeapon->ubMagSize, GetAmmoType(pObj), NO_SLOT );

	//if (bSlot != NO_SLOT)
	//{
	//	if (ReloadGun( pSoldier, &(pSoldier->inv[HANDPOS]), &(pSoldier->inv[bSlot]) ))
	//	{
	//		return( TRUE );
	//	}
	//}

	//<SB> manual recharge
	//pObj = &(pSoldier->inv[HANDPOS]);

	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
	{
		(*pObj)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;

		DeductPoints(pSoldier, Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually, 0);

		PlayJA2Sample( Weapon[ Item[pObj->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );


		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
			}
		}

		return TRUE;
	}
	else
	{
		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;

				DeductPoints(pSoldier, Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually, 0);

				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

				return TRUE;
			}
		}
	}
	//</SB>

	if (bSlot != NO_SLOT && ReloadGun( pSoldier, &(pSoldier->inv[HANDPOS]), &(pSoldier->inv[bSlot]) ))
	{
		return( TRUE );
	}

	return( NOSHOOT_NOAMMO );
}

/*
INT8 TryToReloadLauncher( SOLDIERTYPE * pSoldier )
{
UINT16	usWeapon;
INT8		bSlot;

usWeapon = pSoldier->inv[HANDPOS].usItem;

if ( usWeapon == TANK_CANNON )
{
bSlot = FindObj( pSoldier, TANK_SHELL );
}
else
{
bSlot = FindLaunchable( pSoldier, usWeapon );
}

if (bSlot != NO_SLOT)
{
}
return( NOSHOOT_NOAMMO );
}
*/

INT8 CanNPCAttack(SOLDIERTYPE *pSoldier)
{
	INT8		bCanAttack;
	INT8		bWeaponIn;

	// NEUTRAL civilians are not allowed to attack, but those that are not
	// neutral (KILLNPC mission guynums, escorted guys) can, if they're armed
	if (PTR_CIVILIAN && pSoldier->aiData.bNeutral)
	{
		return(FALSE);
	}

	// test if if we are able to attack (in general, not at any specific target)
	bCanAttack = OKToAttack(pSoldier,NOWHERE);

	// if soldier can't attack because he doesn't have a weapon or is out of ammo
	// or his weapon isn't loaded
	if ( bCanAttack == NOSHOOT_NOAMMO ) // || NOLOAD
	{
		// try to reload it
		bCanAttack = TryToReload( pSoldier );
	}
	else if (bCanAttack == NOSHOOT_NOWEAPON)
	{
		// look for another weapon
		bWeaponIn = FindAIUsableObjClass( pSoldier, IC_WEAPON );
		if (bWeaponIn != NO_SLOT)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"cannpcattack: swapping weapon into hand");
			RearrangePocket( pSoldier, HANDPOS, bWeaponIn, FOREVER );
			// look for another weapon if this one is 1-handed
			//			if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_GUN) && !(Item[ pSoldier->inv[ HANDPOS ].usItem ].fFlags & ITEM_TWO_HANDED ) )
			if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_GUN) && !(Item[ pSoldier->inv[ HANDPOS ].usItem ].twohanded ) )
			{
				// look for another pistol/SMG if available
				// CHRISL: Change final parameter to use dynamic pocket definition
				bWeaponIn = FindAIUsableObjClassWithin( pSoldier, IC_WEAPON, BIGPOCKSTART, NUM_INV_SLOTS );
				//				if (bWeaponIn != NO_SLOT && (Item[ pSoldier->inv[ bWeaponIn ].usItem ].usItemClass == IC_GUN) && !(Item[ pSoldier->inv[ bWeaponIn ].usItem ].fFlags & ITEM_TWO_HANDED ) )
				if (bWeaponIn != NO_SLOT && (Item[ pSoldier->inv[ bWeaponIn ].usItem ].usItemClass == IC_GUN) && !(Item[ pSoldier->inv[ bWeaponIn ].usItem ].twohanded ) )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"cannpcattack: swapping weapon into holster");
					RearrangePocket( pSoldier, SECONDHANDPOS, bWeaponIn, FOREVER );
				}
			}
			// might need to reload
			bCanAttack = CanNPCAttack( pSoldier );
		}
	}

#ifdef DEBUGDECISIONS
	if (bCanAttack != TRUE) // if for any reason we can't attack right now
	{
		//LocateMember(pSoldier->ubID,SETLOCATOR); // locate to this NPC, don't center
		STR16 tempstr;
		sprintf(tempstr,"%s can't attack! (not OKToAttack, Reason code = %d)",pSoldier->name,bCanAttack);
		AIPopMessage(tempstr);
	}
#endif

	return( bCanAttack );
}

void CheckIfTossPossible(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestThrow)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CheckIfTossPossible");
	INT16 ubMinAPcost;

	pSoldier->bWeaponMode = WM_NORMAL;

	if ( TANK( pSoldier ) )
	{
		pBestThrow->bWeaponIn = FindCannon(pSoldier);//FindObj( pSoldier, TANK_CANNON );
	}
	else
	{
		pBestThrow->bWeaponIn = FindAIUsableObjClass( pSoldier, IC_LAUNCHER );

		if ( pBestThrow->bWeaponIn == NO_SLOT || ( !EnoughAmmo( pSoldier, FALSE, pBestThrow->bWeaponIn) && FindAmmoToReload( pSoldier, pBestThrow->bWeaponIn, NO_SLOT) == NO_SLOT) )
		{
			// Consider rocket launcher/cannon
			pBestThrow->bWeaponIn = FindRocketLauncherOrCannon( pSoldier );
			if ( pBestThrow->bWeaponIn == NO_SLOT	|| ( !EnoughAmmo( pSoldier, FALSE, pBestThrow->bWeaponIn) && FindAmmoToReload( pSoldier, pBestThrow->bWeaponIn, NO_SLOT) == NO_SLOT) )
			{
				//no rocket launcher (or empty) -- let's look for an underslung/attached GL
				// AND a launchable grenade!
				INT8 bOldWeaponMode = pSoldier->bWeaponMode;
				pSoldier->bWeaponMode = WM_ATTACHED_GL; // So that EnoughAmmo will check for a grenade not a bullet
				INT8 bGunSlot = FindAIUsableObjClass( pSoldier, IC_GUN );
				if ( bGunSlot != NO_SLOT &&
					IsGrenadeLauncherAttached(&pSoldier->inv[bGunSlot]) &&
					( EnoughAmmo( pSoldier, FALSE, bGunSlot) || FindGLGrenade( pSoldier) == NO_SLOT) )
				{
					pBestThrow->bWeaponIn = bGunSlot;
				}
				else
				{
					// no rocket launcher or attached GL, consider grenades
					pSoldier->bWeaponMode = bOldWeaponMode;
					pBestThrow->bWeaponIn = FindThrowableGrenade( pSoldier );
				}
			}
			else
			{
				// Have rocket launcher... maybe have grenades as well.	which one to use?
				if ( pSoldier->aiData.bAIMorale > MORALE_WORRIED && PreRandom( 2 ) )
				{
					pBestThrow->bWeaponIn = FindThrowableGrenade( pSoldier );
				}
			}
		}
	}

	// if the soldier does have a tossable item somewhere
	if (pBestThrow->bWeaponIn != NO_SLOT)
	{
		// if it's in his holster, swap it into his hand temporarily
		if (pBestThrow->bWeaponIn != HANDPOS)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"checkiftosspossible: swapping item into hand");
			RearrangePocket(pSoldier, HANDPOS, pBestThrow->bWeaponIn, TEMPORARILY);
		}


		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"checkiftosspossible: get minapstoattack");
		// get the minimum cost to attack with this tossable item
		ubMinAPcost = MinAPsToAttack( pSoldier, pSoldier->sLastTarget, DONTADDTURNCOST);

		// if we can afford the minimum AP cost to throw this tossable item
		if (pSoldier->bActionPoints >= ubMinAPcost)
		{
			// then look around for a worthy target (which sets bestThrow.ubPossible)
			CalcBestThrow( pSoldier, pBestThrow );
		}

		// if it was in his holster, swap it back into his holster for now
		if (pBestThrow->bWeaponIn != HANDPOS)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"checkiftosspossible: swapping item into holster");
			RearrangePocket( pSoldier, HANDPOS, pBestThrow->bWeaponIn, TEMPORARILY );
		}
	}
}

INT8 CountAdjacentSpreadTargets( SOLDIERTYPE * pSoldier, INT16 sFirstTarget, INT8 bTargetLevel )
{
	// return the number of people next to this guy for burst-spread purposes

	INT8	bDirLoop, bDir, bCheckDir, bTargetIndex, bTargets;
	INT16	sTarget;
	SOLDIERTYPE * pTarget, * pTargets[5] = {NULL};

	bTargetIndex = -1;
	bCheckDir = -1;

	pTargets[2] = SimpleFindSoldier( sFirstTarget, bTargetLevel );
	if (pTargets[2] == NULL)
	{
		return( 0 );
	}
	bTargets = 1;

	bDir = (INT8) GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, sFirstTarget );

	for (bDirLoop = 0; bDirLoop < 8; bDirLoop++)
	{
		if (bDir % 2)
		{
			// odd direction = diagonal direction
			switch( bDirLoop )
			{
			case 0:
				bCheckDir = (bDir + 6) % 8;
				bTargetIndex = 0;
				break;
			case 1:
				bCheckDir = (bDir + 5) % 8;
				bTargetIndex = 1;
				break;
			case 2:
				bCheckDir = (bDir + 7) % 8;
				bTargetIndex = 1;
				break;
			case 3:
				bCheckDir = (bDir + 3) % 8;
				bTargetIndex = 3;
				break;
			case 4:
				bCheckDir = (bDir + 1) % 8;
				bTargetIndex = 3;
				break;
			case 5:
				bCheckDir = (bDir + 2) % 8;
				bTargetIndex = 4;
				break;
			case 6:
				// check in front
				bCheckDir = (bDir + 4) % 8;
				bTargetIndex = 1;
				break;
			case 7:
				// check behind
				bCheckDir = (bDir) % 8;
				bTargetIndex = 3;
				break;
			}
		}
		else
		{
			// even = straight
			switch( bDirLoop )
			{
			case 0:
				bCheckDir = (bDir + 5) % 8;
				bTargetIndex = 1;
				break;
			case 1:
				bCheckDir = (bDir + 6) % 8;
				bTargetIndex = 1;
				break;
			case 2:
				bCheckDir = (bDir + 7) % 8;
				bTargetIndex = 1;
				break;
			case 3:
				bCheckDir = (bDir + 3) % 8;
				bTargetIndex = 3;
				break;
			case 4:
				bCheckDir = (bDir + 2) % 8;
				bTargetIndex = 3;
				break;
			case 5:
				bCheckDir = (bDir + 1) % 8;
				bTargetIndex = 3;
				break;
			case 6:
				// check in front
				bCheckDir = (bDir + 4) % 8;
				bTargetIndex = 1;
				break;
			case 7:
				// check behind
				bCheckDir = (bDir) % 8;
				bTargetIndex = 3;
				break;

			}
		}
		if (bDirLoop == 6 && bTargets > 1)
		{
			// we're done!	otherwise we continue and try to find people in front/behind
			break;
		}
		else if (pTargets[bTargetIndex] != NULL)
		{
			continue;
		}
		sTarget = sFirstTarget + DirIncrementer[bCheckDir];
		pTarget = SimpleFindSoldier( sTarget, bTargetLevel );
		if (pTarget)
		{
			// check to see if guy is visible
			if (pSoldier->aiData.bOppList[ pTarget->ubID ] == SEEN_CURRENTLY)
			{
				pTargets[bTargetIndex] = pTarget;
				bTargets++;
			}
		}
	}
	return( bTargets - 1 );
}

INT16 CalcSpreadBurst( SOLDIERTYPE * pSoldier, INT16 sFirstTarget, INT8 bTargetLevel )
{
	INT8	bDirLoop, bDir, bCheckDir, bTargetIndex = 0, bLoop, bTargets;
	INT16	sTarget;
	SOLDIERTYPE * pTarget, * pTargets[5] = {NULL};
	INT8 bAdjacents, bOtherAdjacents;


	bCheckDir = -1;

	pTargets[2] = SimpleFindSoldier( sFirstTarget, bTargetLevel );
	if (pTargets[2] == NULL)
	{
		return( sFirstTarget );
	}
	bTargets = 1;
	bAdjacents = CountAdjacentSpreadTargets( pSoldier, sFirstTarget, bTargetLevel );

	bDir = (INT8) GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, sFirstTarget );

	for (bDirLoop = 0; bDirLoop < 8; bDirLoop++)
	{
		if (bDir % 2)
		{
			// odd direction = diagonal direction
			switch( bDirLoop )
			{
			case 0:
				bCheckDir = (bDir + 6) % 8;
				bTargetIndex = 0;
				break;
			case 1:
				bCheckDir = (bDir + 5) % 8;
				bTargetIndex = 1;
				break;
			case 2:
				bCheckDir = (bDir + 7) % 8;
				bTargetIndex = 1;
				break;
			case 3:
				bCheckDir = (bDir + 3) % 8;
				bTargetIndex = 3;
				break;
			case 4:
				bCheckDir = (bDir + 1) % 8;
				bTargetIndex = 3;
				break;
			case 5:
				bCheckDir = (bDir + 2) % 8;
				bTargetIndex = 4;
				break;
			case 6:
				// check in front
				bCheckDir = (bDir + 4) % 8;
				bTargetIndex = 1;
				break;
			case 7:
				// check behind
				bCheckDir = (bDir) % 8;
				bTargetIndex = 3;
				break;
			}
		}
		else
		{
			// even = straight
			switch( bDirLoop )
			{
			case 0:
				bCheckDir = (bDir + 5) % 8;
				bTargetIndex = 1;
				break;
			case 1:
				bCheckDir = (bDir + 6) % 8;
				bTargetIndex = 1;
				break;
			case 2:
				bCheckDir = (bDir + 7) % 8;
				bTargetIndex = 1;
				break;
			case 3:
				bCheckDir = (bDir + 3) % 8;
				bTargetIndex = 3;
				break;
			case 4:
				bCheckDir = (bDir + 2) % 8;
				bTargetIndex = 3;
				break;
			case 5:
				bCheckDir = (bDir + 1) % 8;
				bTargetIndex = 3;
				break;
			case 6:
				// check in front
				bCheckDir = (bDir + 4) % 8;
				bTargetIndex = 1;
				break;
			case 7:
				// check behind
				bCheckDir = (bDir) % 8;
				bTargetIndex = 3;
				break;

			}
		}
		if (bDirLoop == 6 && bTargets > 1)
		{
			// we're done!	otherwise we continue and try to find people in front/behind
			break;
		}
		else if (pTargets[bTargetIndex] != NULL)
		{
			continue;
		}
		sTarget = sFirstTarget + DirIncrementer[bCheckDir];
		pTarget = SimpleFindSoldier( sTarget, bTargetLevel );
		if (pTarget && pSoldier->aiData.bOppList[ pTarget->ubID ] == SEEN_CURRENTLY)
		{
			bOtherAdjacents = CountAdjacentSpreadTargets( pSoldier, sTarget, bTargetLevel );
			if (bOtherAdjacents > bAdjacents)
			{
				// we should do a spread-burst there instead!
				return( CalcSpreadBurst( pSoldier, sTarget, bTargetLevel ) );
			}
			pTargets[bTargetIndex] = pTarget;
			bTargets++;
		}
	}

	if (bTargets > 1)
	{
		// Move all the locations down in the array if necessary
		// Check the 4th position
		if (pTargets[3] == NULL && pTargets[4] != NULL)
		{
			pTargets[3] = pTargets[4];
			pTargets[4] = NULL;
		}
		// Check the first two positions; we know the 3rd value is set because
		// it's our initial target
		if (pTargets[1] == NULL)
		{
			pTargets[1] = pTargets[2];
			pTargets[2] = pTargets[3];
			pTargets[3] = pTargets[4];
			pTargets[4] = NULL;
		}
		if (pTargets[0] == NULL)
		{
			pTargets[0] = pTargets[1];
			pTargets[1] = pTargets[2];
			pTargets[2] = pTargets[3];
			pTargets[3] = pTargets[4];
			pTargets[4] = NULL;
		}
		// now 50% chance to reorganize to fire in reverse order
		if (Random( 2 ))
		{
			for( bLoop = 0; bLoop < bTargets / 2; bLoop++)
			{
				pTarget = pTargets[bLoop];
				pTargets[bLoop] = pTargets[bTargets - 1 - bLoop];
				pTargets[bTargets - 1 - bLoop] = pTarget;
			}
		}
		AIPickBurstLocations( pSoldier, bTargets, pTargets );
		pSoldier->flags.fDoSpread = TRUE;
	}
	return( sFirstTarget );
}

INT16 AdvanceToFiringRange( SOLDIERTYPE * pSoldier, INT16 sClosestOpponent )
{
	// see how far we can go down a path and still shoot

	INT16		bAttackCost, bTrueActionPoints;
	UINT16	usActionData;

	bAttackCost = MinAPsToAttack(pSoldier, sClosestOpponent, ADDTURNCOST);

	if (bAttackCost >= pSoldier->bActionPoints)
	{
		// probably want to go as far as possible!
		// return( NOWHERE );
		return( GoAsFarAsPossibleTowards( pSoldier, sClosestOpponent, AI_ACTION_SEEK_OPPONENT ) );
	}

	bTrueActionPoints = pSoldier->bActionPoints;

	pSoldier->bActionPoints -= bAttackCost;

	usActionData = GoAsFarAsPossibleTowards( pSoldier, sClosestOpponent, AI_ACTION_SEEK_OPPONENT );
	//POSSIBLE STRUCTURE PROBLEM HERE.  GOTTHARD 7/15/08
	pSoldier->bActionPoints = bTrueActionPoints;

	return( usActionData );

}



void CheckIfShotPossible(SOLDIERTYPE *pSoldier, ATTACKTYPE *pBestShot, BOOLEAN suppressionFire)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CheckIfShotPossible");
	INT16 ubMinAPcost;
	pBestShot->ubPossible = FALSE;
	pBestShot->bWeaponIn = NO_SLOT;

	if ( !TANK( pSoldier ) )
	{
		// AIs never have more than one gun anyway
		pBestShot->bWeaponIn = FindAIUsableObjClass( pSoldier, IC_GUN );
	}

	// if the soldier does have a long range rifle
	if (pBestShot->bWeaponIn != NO_SLOT)
	{
		OBJECTTYPE * pObj = &pSoldier->inv[pBestShot->bWeaponIn];
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CheckIfShotPossible: found a gun item # %d",pObj->usItem ));

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CheckIfShotPossible: weapon type %d",Weapon [pObj->usItem ].ubWeaponType ));

		// if it's in his holster, swap it into his hand temporarily
		if (pBestShot->bWeaponIn != HANDPOS)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CheckIfShotPossible: Rearranging pocket");
			RearrangePocket(pSoldier, HANDPOS, pBestShot->bWeaponIn, TEMPORARILY);
		}

		//if ( (!suppressionFire && ( (IsScoped(pObj) && GunRange(pObj) > pSoldier->MaxDistanceVisible(pBestShot->sTarget, pBestShot->bTargetLevel) ) || pSoldier->bOrders == SNIPER ) ) ||
		// HEADROCK HAM B2.4: Changed this again - weapons are no longer checked for larger magazine to allow suppressive fire, due to
		// suppressive fire revamp.
//		if ( (!suppressionFire && ( (IsScoped(pObj) && GunRange(pObj) > MaxNormalDistanceVisible() ) || pSoldier->aiData.bOrders == SNIPER ) ) ||
//			(suppressionFire  && IsGunAutofireCapable(&pSoldier->inv[pBestShot->bWeaponIn] ) && GetMagSize(pObj) > 30 && (*pObj)[0]->data.gun.ubGunShotsLeft > 20 ))
		BOOLEAN fEnableAISuppression = FALSE;

		if ( gGameExternalOptions.fIncreaseAISuppressionFire && ((!suppressionFire && ( (IsScoped(pObj) && GunRange(pObj) > MaxNormalDistanceVisible() ) || pSoldier->aiData.bOrders == SNIPER ) ) ||
			(suppressionFire  && IsGunAutofireCapable(&pSoldier->inv[pBestShot->bWeaponIn]))) )
			fEnableAISuppression = TRUE;
		else if ( !gGameExternalOptions.fIncreaseAISuppressionFire && ( (!suppressionFire && ( (IsScoped(pObj) && GunRange(pObj) > MaxNormalDistanceVisible() ) || pSoldier->aiData.bOrders == SNIPER ) ) ||
			(suppressionFire  && IsGunAutofireCapable(&pSoldier->inv[pBestShot->bWeaponIn] ) && GetMagSize(pObj) > 30 && (*pObj)[0]->data.gun.ubGunShotsLeft > 20 )))
			fEnableAISuppression = TRUE;

		if (fEnableAISuppression)
		{
			// get the minimum cost to attack with this item
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CheckIfShotPossible: getting min aps");
			ubMinAPcost = MinAPsToAttack( pSoldier, pSoldier->sLastTarget, ADDTURNCOST);
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CheckIfShotPossible: min AP cost: %d", ubMinAPcost));

			// if we can afford the minimum AP cost
			if (pSoldier->bActionPoints >= ubMinAPcost)
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CheckIfShotPossible: CalcBestShot"));
				// then look around for a worthy target (which sets bestThrow.ubPossible)
				CalcBestShot( pSoldier, pBestShot,TRUE);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("CheckIfShotPossible: CalcBestShot done"));
			}
		}
		// if it was in his holster, swap it back into his holster for now
		if (pBestShot->bWeaponIn != HANDPOS)
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CheckIfShotPossible: Rearranging pocket");
			RearrangePocket( pSoldier, HANDPOS, pBestShot->bWeaponIn, TEMPORARILY );
		}
	}
}
