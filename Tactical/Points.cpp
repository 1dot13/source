#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "worlddef.h"
	#include "points.h"
	#include "overhead.h"
	#include "Font control.h"
	#include "interface.h"
	#include "Isometric utils.h"
	#include "pathai.h"
	#include "interface.h"
	#include "message.h"
	#include "Animation Control.h"
	#include "Weapons.h"

	#include "structure wrap.h"
	#include "dialogue control.h"
	#include "items.h"
	#include "rt time defines.h"
	#include "ai.h"
	#include "handle ui.h"
	#include "text.h"
	#include "SkillCheck.h"
	#include "wcheck.h"
	#include "Soldier Profile.h"
	#include "Soldier macros.h"
	#include "Random.h"
	#include "Campaign.h"
	#include "drugs and alcohol.h"
	#include "GameSettings.h"
	#include "worldman.h"
	#include "math.h"
#include "Map Information.h"
#include "Interface Items.h"
#endif
#include "connect.h"
//rain
//#define BREATH_GAIN_REDUCTION_PER_RAIN_INTENSITY 25
//end rain

extern BOOLEAN IsValidSecondHandShot( SOLDIERTYPE *pSoldier );


INT16 GetBreathPerAP( SOLDIERTYPE *pSoldier, UINT16 usAnimState );

INT16 TerrainActionPoints( SOLDIERTYPE *pSoldier, INT16 sGridno, INT8 bDir, INT8 bLevel )
{
		INT16	sAPCost = 0;
	INT16	sSwitchValue;
		BOOLEAN		fHiddenStructVisible;				// Used for hidden struct visiblity

 if ( pSoldier->bStealthMode )
	 sAPCost += APBPConstants[AP_STEALTH_MODIFIER];

 if ( pSoldier->bReverse || gUIUseReverse )
	 sAPCost += APBPConstants[AP_REVERSE_MODIFIER];

 //if (GridCost[gridno] == NPCMINECOST)
 //	switchValue = BackupGridCost[gridno];
 //else

	sSwitchValue = gubWorldMovementCosts[sGridno][bDir][ bLevel ];

	// Check reality vs what the player knows....
	if ( pSoldier->bTeam == gbPlayerNum )
	{
		// Is this obstcale a hidden tile that has not been revealed yet?
		if( DoesGridnoContainHiddenStruct( (INT16)sGridno, &fHiddenStructVisible ) )
		{
			// Are we not visible, if so use terrain costs!
			if ( !fHiddenStructVisible )
			{
				// Set cost of terrain!
				sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridno ].ubTerrainID ];
			}
		}
	}
	if ( sSwitchValue == TRAVELCOST_NOT_STANDING )
	{
		// use the cost of the terrain!
		sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridno ].ubTerrainID ];
	}
	else if (IS_TRAVELCOST_DOOR( sSwitchValue ))
	{
		// Can't travel diagonally through a door
		if (bDir & 1)
		{
			return -1;
		}
		sSwitchValue = DoorTravelCost( pSoldier, sGridno, (UINT8) sSwitchValue, (BOOLEAN) (pSoldier->bTeam == gbPlayerNum), NULL );
	}
	else if (gfPlotPathToExitGrid && sSwitchValue == TRAVELCOST_EXITGRID)
	{
		sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridno ].ubTerrainID ];
	}

	if (sSwitchValue >= TRAVELCOST_BLOCKED && sSwitchValue != TRAVELCOST_DOOR )
	{
		return(-1);	// Cost too much to be considered!
	}

	//CHRISL: We can't jump a fence while wearing a backpack, to consider fences as impassible
	if(sSwitchValue == TRAVELCOST_FENCE && UsingNewInventorySystem() == true && pSoldier->inv[BPACKPOCKPOS].exists() == true)
	{
		return(-1);
	}

	switch( sSwitchValue )
	{
	case TRAVELCOST_DIRTROAD :
	case TRAVELCOST_FLAT			: sAPCost += APBPConstants[AP_MOVEMENT_FLAT]; 
		break;
		//case TRAVELCOST_BUMPY		:		
	case TRAVELCOST_GRASS		: sAPCost += APBPConstants[AP_MOVEMENT_GRASS];
		break;
	case TRAVELCOST_THICK		:	sAPCost += APBPConstants[AP_MOVEMENT_BUSH];
		break;
	case TRAVELCOST_DEBRIS		: sAPCost += APBPConstants[AP_MOVEMENT_RUBBLE];
		break;
	case TRAVELCOST_SHORE		: sAPCost += APBPConstants[AP_MOVEMENT_SHORE]; // wading shallow water
		if (!IS_MERC_BODY_TYPE( pSoldier ))
		{
			return -1;
		}
		break;
	case TRAVELCOST_KNEEDEEP	:	sAPCost += APBPConstants[AP_MOVEMENT_LAKE]; // wading waist/chest deep - very slow
		if (!IS_MERC_BODY_TYPE( pSoldier ))
		{
			return -1;
		}
		break;

	case TRAVELCOST_DEEPWATER: sAPCost += APBPConstants[AP_MOVEMENT_OCEAN]; // can swim, so it's faster than wading
		if (!IS_MERC_BODY_TYPE( pSoldier ))
		{
			return -1;
		}
		break;
		/*
		case TRAVELCOST_VEINEND	:
		case TRAVELCOST_VEINMID	: sAPCost += APBPConstants[AP_MOVEMENT_FLAT];
		break;
		*/
	case TRAVELCOST_DOOR			: 
		if (pSoldier->bTeam == gbPlayerNum)
		{
			return -1;
		}
		sAPCost += APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_OPEN_DOOR] + APBPConstants[AP_OPEN_DOOR]; // Include open and close costs!
		break;

		// cost for jumping a fence REPLACES all other AP costs!
	case TRAVELCOST_FENCE		: 
		if (!IS_MERC_BODY_TYPE( pSoldier ))
		{
			return -1;
		}
		// CHRISL: 
		if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
			return( APBPConstants[AP_JUMPFENCEBPACK] );
		else
			return( APBPConstants[AP_JUMPFENCE] );

	case TRAVELCOST_NONE			: return( 0 );

	default:

		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Calc AP: Unrecongnized MP type %d in %d, direction %d", sSwitchValue, sGridno, bDir ) );
			break;


	}

	if (bDir & 1)
	{
		sAPCost = (sAPCost * 14) / 10;
	}


	return(sAPCost);

}


INT16 BreathPointAdjustmentForCarriedWeight( SOLDIERTYPE * pSoldier )
{
	UINT32	uiCarriedPercent;
	UINT32	uiPercentCost;

	uiCarriedPercent = CalculateCarriedWeight( pSoldier );
	if (uiCarriedPercent < 101)
	{
		// normal BP costs
		uiPercentCost = 100;
	}
	else
	{
		if (uiCarriedPercent < 151)
		{
			// between 101 and 150% of max carried weight, extra BP cost
			// of 1% per % above 100... so at 150%, we pay 150%
			uiPercentCost = 100 + (uiCarriedPercent - 100) * 3;
		}
		else if (uiCarriedPercent < 201)
		{
			// between 151 and 200% of max carried weight, extra BP cost
			// of 2% per % above 150... so at 200%, we pay 250%
			uiPercentCost = 100 + (uiCarriedPercent - 100) * 3 + (uiCarriedPercent - 150);
		}
		else
		{
			// over 200%, extra BP cost of 3% per % above 200
			uiPercentCost = 100 + (uiCarriedPercent - 100) * 3 + (uiCarriedPercent - 150) + (uiCarriedPercent - 200);
			// so at 250% weight, we pay 400% breath!
		}
	}
	return( (INT16) uiPercentCost );
}


INT16 TerrainBreathPoints(SOLDIERTYPE * pSoldier, INT16 sGridno,INT8 bDir, UINT16 usMovementMode)
{
 INT32 iPoints=0;
 UINT8 ubMovementCost;

	ubMovementCost = gubWorldMovementCosts[sGridno][bDir][0];

 switch( ubMovementCost )
 {
	case TRAVELCOST_DIRTROAD	:
	case TRAVELCOST_FLAT			: iPoints = APBPConstants[BP_MOVEMENT_FLAT];		break;
	//case TRAVELCOST_BUMPY			:
	case TRAVELCOST_GRASS			: iPoints = APBPConstants[BP_MOVEMENT_GRASS];	break;
	case TRAVELCOST_THICK			: iPoints = APBPConstants[BP_MOVEMENT_BUSH];		break;
	case TRAVELCOST_DEBRIS		: iPoints = APBPConstants[BP_MOVEMENT_RUBBLE];	break;
	case TRAVELCOST_SHORE			: iPoints = APBPConstants[BP_MOVEMENT_SHORE];	break;	// wading shallow water
	case TRAVELCOST_KNEEDEEP	: iPoints = APBPConstants[BP_MOVEMENT_LAKE];		break;	// wading waist/chest deep - very slow
	case TRAVELCOST_DEEPWATER	: iPoints = APBPConstants[BP_MOVEMENT_OCEAN];	break;	// can swim, so it's faster than wading
//	case TRAVELCOST_VEINEND		:
//	case TRAVELCOST_VEINMID		: iPoints = APBPConstants[BP_MOVEMENT_FLAT];		break;
	default:
		if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
		{
			iPoints = APBPConstants[BP_MOVEMENT_FLAT];
			break;
		}
/*
#ifdef TESTVERSION
	 NumMessage("ERROR: TerrainBreathPoints: Unrecognized grid cost = ",
								GridCost[gridno]);
#endif
*/
			return(0);
	}

	iPoints = iPoints * BreathPointAdjustmentForCarriedWeight( pSoldier ) / 100;

	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( gpWorldLevelData[ sGridno ].ubTerrainID) )
	{
	usMovementMode = WALKING;
	}

	// so, then we must modify it for other movement styles and accumulate
	switch(usMovementMode)
	{
		case RUNNING:
		case ADULTMONSTER_WALKING:
		case BLOODCAT_RUN:

			iPoints *= APBPConstants[BP_RUN_ENERGYCOSTFACTOR];		break;

		case SIDE_STEP:
		case WALK_BACKWARDS:
		case BLOODCAT_WALK_BACKWARDS:
		case MONSTER_WALK_BACKWARDS:
		case WALKING :	iPoints *= APBPConstants[BP_WALK_ENERGYCOSTFACTOR];	break;

		case START_SWAT:
		case SWATTING:
		case SWAT_BACKWARDS:
			iPoints *= APBPConstants[BP_SWAT_ENERGYCOSTFACTOR];	break;
		case CRAWLING:	iPoints *= APBPConstants[BP_CRAWL_ENERGYCOSTFACTOR];	break;


	}

	// ATE: Adjust these by realtime movement
 if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
 {
		// ATE: ADJUST FOR RT - MAKE BREATH GO A LITTLE FASTER!
		iPoints	= (INT32)( iPoints * TB_BREATH_DEDUCT_MODIFIER );
 }


	return( (INT16) iPoints);
}


INT16 ActionPointCost( SOLDIERTYPE *pSoldier, INT16 sGridNo, INT8 bDir, UINT16 usMovementMode )
{
	INT16 sTileCost, sPoints, sSwitchValue;

	sPoints = 0;

	// get the tile cost for that tile based on WALKING
	sTileCost = TerrainActionPoints( pSoldier, sGridNo, bDir, pSoldier->pathing.bLevel );
	if (sTileCost == -1)
	{
		return 100;
	}

	// Get switch value...
	sSwitchValue = gubWorldMovementCosts[ sGridNo ][ bDir ][ pSoldier->pathing.bLevel ];

	// Tile cost should not be reduced based on movement mode...
	if ( sSwitchValue == TRAVELCOST_FENCE )
	{
	return( sTileCost );
	}

	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( gpWorldLevelData[ sGridNo ].ubTerrainID) )
	{
	usMovementMode = WALKING;
	}

	// so, then we must modify it for other movement styles and accumulate
	// CHRISL: Adjusted system to use different move costs while wearing a backpack
	if (sTileCost > 0)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case ADULTMONSTER_WALKING:
			case BLOODCAT_RUN:
				// CHRISL
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (INT16)(DOUBLE)( (sTileCost / RUNDIVISORBPACK) );
				else
					sPoints = (INT16)(DOUBLE)( (sTileCost / RUNDIVISOR) );
				break;

			case CROW_FLY:
			case SIDE_STEP:
			case WALK_BACKWARDS:
			case ROBOT_WALK:
			case BLOODCAT_WALK_BACKWARDS:
			case MONSTER_WALK_BACKWARDS:
      case LARVAE_WALK:
			case WALKING :
				// CHRISL
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + WALKCOSTBPACK);
				else
					sPoints = (sTileCost + WALKCOST);
				break;

			case START_SWAT:
			case SWAT_BACKWARDS:
			// CHRISL
			case SWATTING:
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + SWATCOSTBPACK);
				else
					sPoints = (sTileCost + SWATCOST);
				break;
			case CRAWLING:
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + CRAWLCOSTBPACK);
				else
					sPoints = (sTileCost + CRAWLCOST);
				break;

			default:

				// Invalid movement mode
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Invalid movement mode %d used in ActionPointCost", usMovementMode	) );
				sPoints = 1;
		}
	}

	if (sSwitchValue == TRAVELCOST_NOT_STANDING)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case WALKING :
		case LARVAE_WALK:
			case SIDE_STEP:
			case WALK_BACKWARDS:
				// charge crouch APs for ducking head!
				sPoints += APBPConstants[AP_CROUCH];
				break;

			default:
				break;
		}
	}

	return( sPoints );
}

INT16 EstimateActionPointCost( SOLDIERTYPE *pSoldier, INT16 sGridNo, INT8 bDir, UINT16 usMovementMode, INT8 bPathIndex, INT8 bPathLength )
{
	// This action point cost code includes the penalty for having to change
	// stance after jumping a fence IF our path continues...
	INT16 sTileCost, sPoints, sSwitchValue;
	sPoints = 0;

	// get the tile cost for that tile based on WALKING
	sTileCost = TerrainActionPoints( pSoldier, sGridNo, bDir, pSoldier->pathing.bLevel );
	if (sTileCost == -1)
	{
		return 100;
	}

	// so, then we must modify it for other movement styles and accumulate
	// CHRISL: Adjusted system to use different move costs while wearing a backpack
	if (sTileCost > 0)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case ADULTMONSTER_WALKING:
			case BLOODCAT_RUN:
				// CHRISL
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (INT16)(DOUBLE)( (sTileCost / RUNDIVISORBPACK) );
				else
					sPoints = (INT16)(DOUBLE)( (sTileCost / RUNDIVISOR) );
				break;

			case CROW_FLY:
			case SIDE_STEP:
			case ROBOT_WALK:
			case WALK_BACKWARDS:
			case BLOODCAT_WALK_BACKWARDS:
			case MONSTER_WALK_BACKWARDS:
			case LARVAE_WALK:
			// CHRISL
			case WALKING :
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + WALKCOSTBPACK);
				else
					sPoints = (sTileCost + WALKCOST);
				break;

			case START_SWAT:
			case SWAT_BACKWARDS:
			// CHRISL
			case SWATTING:
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + SWATCOSTBPACK);
				else
					sPoints = (sTileCost + SWATCOST);
				break;
			case CRAWLING:
				if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true)
					sPoints = (sTileCost + CRAWLCOSTBPACK);
				else
					sPoints = (sTileCost + CRAWLCOST);
				break;

			default:

				// Invalid movement mode
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Invalid movement mode %d used in ActionPointCost", usMovementMode	) );
				sPoints = 1;
		}
	}

	// Get switch value...
	sSwitchValue = gubWorldMovementCosts[ sGridNo ][ bDir ][ pSoldier->pathing.bLevel ];

	// ATE: If we have a 'special cost, like jump fence...
	if ( sSwitchValue == TRAVELCOST_FENCE )
	{
		// If we are changeing stance ( either before or after getting there....
		// We need to reflect that...
		switch(usMovementMode)
		{
			case SIDE_STEP:
			case WALK_BACKWARDS:
			case RUNNING:
			case WALKING :

				// Add here cost to go from crouch to stand AFTER fence hop....
				// Since it's AFTER.. make sure we will be moving after jump...
				if ( ( bPathIndex + 2 ) < bPathLength )
				{
					sPoints += APBPConstants[AP_CROUCH];
				}
				break;

			case SWATTING:
			case START_SWAT:
			case SWAT_BACKWARDS:

				// Add cost to stand once there BEFORE....
				sPoints += APBPConstants[AP_CROUCH];
				break;

			case CRAWLING:

				// Can't do it here.....
				break;
		}
	}
	else if (sSwitchValue == TRAVELCOST_NOT_STANDING)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case WALKING :
			case SIDE_STEP:
			case WALK_BACKWARDS:
				// charge crouch APs for ducking head!
				sPoints += APBPConstants[AP_CROUCH];
				break;

			default:
				break;
		}
	}

	return( sPoints );
}



BOOLEAN EnoughPoints( SOLDIERTYPE *pSoldier, INT16 sAPCost, INT32 iBPCost, BOOLEAN fDisplayMsg )
{
	INT16 sNewAP = 0;

	//CHRISL: if we don't have soldier information, assume we have enough points.
	if(pSoldier == NULL)
		return( TRUE );
	// If this guy is on a special move... don't care about APS, OR BPSs!
	if ( pSoldier->ubWaitActionToDo	)
	{
		return( TRUE );
	}

	if ( pSoldier->ubQuoteActionID >=QUOTE_ACTION_ID_TRAVERSE_EAST && pSoldier->ubQuoteActionID <= QUOTE_ACTION_ID_TRAVERSE_NORTH )
	{
		// AI guy on special move off map
		return( TRUE );
	}

	// IN realtime.. only care about BPs
	if ( ( gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		sAPCost = 0;
	}

	#ifdef NETWORKED
	if( !IsTheSolderUnderMyControl( pSoldier->ubID)	)
	{
		return( TRUE );
	}
	#endif

	if (is_networked)
	{
		if(pSoldier->ubID > 119 || (!is_server && pSoldier->ubID > 20))
		{
			return(TRUE);
		}//hayden , if soldier replication, allow.
	}

	// Get New points
	sNewAP = pSoldier->bActionPoints - sAPCost;

	// If we cannot deduct points, return FALSE
	if ( sNewAP < 0 )
	{
		// Display message if it's our own guy
		if ( pSoldier->bTeam == gbPlayerNum && fDisplayMsg )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NOT_ENOUGH_APS_STR ] );
		}
		return( FALSE );
	}

	return( TRUE );
}


void DeductPoints( SOLDIERTYPE *pSoldier, INT16 sAPCost, INT32 iBPCost,BOOLEAN fProactive )
{
	INT16 sNewAP = 0;
	INT8	bNewBreath;


	// in real time, there IS no AP cost, (only breath cost)
	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		sAPCost = 0;
	}

	// Get New points
	sNewAP = pSoldier->bActionPoints - sAPCost;

	// If this is the first time with no action points, set UI flag
	if ( sNewAP <= 0 && pSoldier->bActionPoints > 0 )
	{
		pSoldier->flags.fUIFirstTimeNOAP = TRUE;
		fInterfacePanelDirty = TRUE;
	}

	// If we cannot deduct points, return FALSE
	if ( sNewAP < 0 )
	{
		sNewAP = 0;
	}

	pSoldier->bActionPoints = (INT8)sNewAP;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Deduct Points (%d at %d) %d %d", pSoldier->ubID, pSoldier->sGridNo, sAPCost, iBPCost	) );

	if ( AM_A_ROBOT( pSoldier ) )
	{
		// zap all breath costs for robot
		iBPCost = 0;
	}

	// is there a BREATH deduction/transaction to be made?	(REMEMBER: could be a GAIN!)
	// NB: iBPCost > 0 - breath loss, iBPCost < 0 - breath gain
	if (iBPCost)
	{
		if (is_networked)
		{
			// Adjust breath changes due to spending or regaining of energy
			if(!pSoldier->bActive)
			{
				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"test_func2" );
			}
			else
				iBPCost = AdjustBreathPts(pSoldier, iBPCost);
		}
		else
		{
			iBPCost = AdjustBreathPts(pSoldier, iBPCost);
		}


		// Snap: award some health and strength for exertion
		// Do 4 StatChange rolls for health (2 for strength) per 10 breath points spent?
		// NB: The scale of iBPCost is 100 per breath point (APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL])
		if ( PTR_OURTEAM && iBPCost >= APBPConstants[BP_MOVEMENT_GRASS] && fProactive
			&& (INT32) PreRandom( 10 * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] ) < iBPCost )
		{
			StatChange(pSoldier, HEALTHAMT, 4, FALSE);
			StatChange(pSoldier, STRAMT, 2, FALSE);
		}

		// CJC: moved check for high breathred to below so that negative breath can be detected

		// Snap: kill the overflow bug already!
		// NB: normally, sBreathRed == (100 - bBreath)*APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL]
		const INT16 BREATH_RED_MAX = 100 * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL];
		if ( pSoldier->sBreathRed + iBPCost < 0 )
		{
			pSoldier->sBreathRed = 0;
		}
		else if ( pSoldier->sBreathRed + iBPCost > BREATH_RED_MAX )
		{
			// Snap: moved this up, because it had no effect below
			// Take off 1 AP per 5 (negative) breath...
			pSoldier->bActionPoints -= ( pSoldier->sBreathRed + iBPCost - BREATH_RED_MAX )
									/ ( 5 * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] );
			if ( pSoldier->bActionPoints < 0 )
			{
				pSoldier->bActionPoints = 0;
			}

			pSoldier->sBreathRed = BREATH_RED_MAX;
		}
		else
		{
			pSoldier->sBreathRed += iBPCost;
		}

		// Get new breath
		bNewBreath = ( pSoldier->bBreathMax * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] - pSoldier->sBreathRed )
					/ APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL];

		// Snap: This should never happen...
		if ( bNewBreath > pSoldier->bBreathMax )
		{
			bNewBreath = pSoldier->bBreathMax;
		}
		else if ( bNewBreath < 0 )
		{
			bNewBreath = 0;
		}

		pSoldier->bBreath = bNewBreath;
	}

	// UPDATE BAR
	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL1 );

}




INT32 AdjustBreathPts(SOLDIERTYPE *pSold, INT32 iBPCost)
{
 INT16 sBreathFactor = 100;
 UINT8 ubBandaged;


 //NumMessage("BEFORE adjustments, BREATH PTS = ",breathPts);

 // in real time, there IS no AP cost, (only breath cost)
 /*
 if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
 {
	// ATE: ADJUST FOR RT - MAKE BREATH GO A LITTLE FASTER!
	iBPCost	*= TB_BREATH_DEDUCT_MODIFIER;
 }
 */


 // adjust breath factor for current breath deficiency
 sBreathFactor += (100 - pSold->bBreath);

 // adjust breath factor for current life deficiency (but add 1/2 bandaging)
 ubBandaged = pSold->stats.bLifeMax - pSold->stats.bLife - pSold->bBleeding;
 //sBreathFactor += (pSold->stats.bLifeMax - (pSold->stats.bLife + (ubBandaged / 2)));
 sBreathFactor += 100 * (pSold->stats.bLifeMax - (pSold->stats.bLife + (ubBandaged / 2))) / pSold->stats.bLifeMax;

 if ( pSold->stats.bStrength > 80 )
 {
	// give % reduction to breath costs for high strength mercs
	sBreathFactor -= (pSold->stats.bStrength - 80) / 2;
 }

/*	THIS IS OLD JAGGED ALLIANCE STUFF (left for possible future reference)

 // apply penalty due to high temperature, heat, and hot Metaviran sun
 // if INDOORS, in DEEP WATER, or possessing HEAT TOLERANCE trait
 if ((ptr->terrtype == FLOORTYPE) || (ptr->terr >= OCEAN21) ||
					(ptr->trait == HEAT_TOLERANT))
	breathFactor += (Status.heatFactor / 5);	// 20% of normal heat penalty
 else
	breathFactor += Status.heatFactor;		// not used to this!
*/


 // if a non-swimmer type is thrashing around in deep water
	if ( (pSold->ubProfile != NO_PROFILE ) && (gMercProfiles[ pSold->ubProfile ].bPersonalityTrait == NONSWIMMER) )
	{
		if ( pSold->usAnimState == DEEP_WATER_TRED || pSold->usAnimState == DEEP_WATER_SWIM)
		{
			sBreathFactor *= 5;		// lose breath 5 times faster in deep water!
		}
	}

 if ( sBreathFactor == 0 )
 {
		iBPCost = 0;
 }
 else if (iBPCost > 0)		// breath DECREASE
	// increase breath COST by breathFactor
	iBPCost = ((iBPCost * sBreathFactor) / 100);
 else				// breath INCREASE
	// decrease breath GAIN by breathFactor
	iBPCost = ((iBPCost * 100) / sBreathFactor);


 return(iBPCost);
}



void UnusedAPsToBreath(SOLDIERTYPE *pSold)
{
	INT16 sUnusedAPs, sBreathPerAP = 0, sBreathChange, sRTBreathMod;
	// Note to Andrew (or whomever else it may concern):


	// This function deals with BETWEEN TURN breath/energy gains. The basic concept is:
	//
	//	- look at LAST (current) animation of merc to see what he's now doing
	//	- look at how many AP remain unspent (indicating duration of time doing that anim)
	//
	//	figure out how much breath/energy (if any) he should recover. Obviously if a merc
	//	is STANDING BREATHING and hasn't spent any AP then it means he *stood around* for
	//	the entire duration of one turn (which, instead of spending energy, REGAINS energy)


	// COMMENTED OUT FOR NOW SINCE MOST OF THE ANIMATION DEFINES DO NOT MATCH

	// If we are not in turn-based combat...


	if ( pSold->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if ( !( gTacticalStatus.uiFlags & TURNBASED ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// ALRIGHT, GIVE A FULL AMOUNT BACK, UNLES MODIFIED BY WHAT ACTIONS WE WERE DOING
		sBreathPerAP = GetBreathPerAP( pSold, pSold->usAnimState );

		// adjust for carried weight
		sBreathPerAP = sBreathPerAP * 100 / BreathPointAdjustmentForCarriedWeight( pSold );

		// If this value is -ve, we have a gain, else we have a loos which we should not really do
		// We just want to limit this to no gain if we were doing stuff...
		sBreathChange = 3 * sBreathPerAP;

		// Adjust for on drugs
		HandleBPEffectDueToDrugs( pSold, &sBreathChange );

		if ( sBreathChange > 0 )
		{
			sBreathChange = 0;
		}
		else
		{
			// We have a gain, now limit this depending on what we were doing...
			// OK for RT, look at how many tiles we have moved, our last move anim
			if ( pSold->ubTilesMovedPerRTBreathUpdate > 0 )
			{
				// How long have we done this for?
				// And what anim were we doing?
				sBreathPerAP = GetBreathPerAP( pSold, pSold->usLastMovementAnimPerRTBreathUpdate );

				sRTBreathMod = sBreathPerAP * pSold->ubTilesMovedPerRTBreathUpdate;

				// Deduct some if we were exerting ourselves
				// We add here because to gain breath, sBreathChange needs to be -ve
				if ( sRTBreathMod > 0 )
				{
					sBreathChange += sRTBreathMod;
				}

				if ( sBreathChange < 0 )
				{
					sBreathChange = 0;
				}
			}
		}

		// Divide by a number to adjust that in realtimer we do not want to recover as
		// as fast as the TB values do
		sBreathChange *= TB_BREATH_RECOVER_MODIFIER;


		// adjust breath only, don't touch action points!
		DeductPoints(pSold,0,(INT16)sBreathChange );

		// Reset value for RT breath update
		pSold->ubTilesMovedPerRTBreathUpdate = 0;

	}
	else
	{
		// if merc has any APs left unused this turn (that aren't carrying over)
		if (pSold->bActionPoints > APBPConstants[MAX_AP_CARRIED])
		{

			sUnusedAPs = pSold->bActionPoints - APBPConstants[MAX_AP_CARRIED];

			sBreathPerAP = GetBreathPerAP( pSold, pSold->usAnimState );

			if (sBreathPerAP < 0)
			{
				// can't gain any breath when we've just been gassed, OR
				// if standing in tear gas without a gas mask on
				if ( pSold->flags.uiStatusFlags & SOLDIER_GASSED )
				{
					return;		// can't breathe here, so get no breath back!
				}
			}

			// adjust for carried weight
			sBreathPerAP = sBreathPerAP * 100 / BreathPointAdjustmentForCarriedWeight( pSold );

			if ( pSold->bTeam != CIV_TEAM && pSold->bTeam != gbPlayerNum)
			{
				switch( gGameOptions.ubDifficultyLevel )
				{
					case DIF_LEVEL_EASY:
						sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iEasyAPBonus)- sUnusedAPs) * sBreathPerAP;
						break;

					case DIF_LEVEL_MEDIUM:
						sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iExperiencedAPBonus)-sUnusedAPs) * sBreathPerAP;
						break;

					case DIF_LEVEL_HARD:
						sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iExpertAPBonus)- sUnusedAPs) * sBreathPerAP;
						break;

					case DIF_LEVEL_INSANE:
						sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iInsaneAPBonus)- sUnusedAPs) * sBreathPerAP;
						break;

					default:
						sBreathChange = (APBPConstants[AP_MAXIMUM] - sUnusedAPs) * sBreathPerAP;
				}
			}
			else
			{
				sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iPlayerAPBonus)- sUnusedAPs) * sBreathPerAP;
			}
		}
		else
		{
			sBreathChange = 0;
		}
		// Adjust for on drugs
		HandleBPEffectDueToDrugs( pSold, &sBreathChange );

		// adjust breath only, don't touch action points!
		DeductPoints(pSold,0,(INT16)sBreathChange );

	}
}


//rain
extern INT8 gbCurrentRainIntensity;
//end rain

INT16 GetBreathPerAP( SOLDIERTYPE *pSoldier, UINT16 usAnimState )
{
	INT16 sBreathPerAP = 0;
	BOOLEAN	fAnimTypeFound = FALSE;

	if ( gAnimControl[ usAnimState ].uiFlags & ANIM_VARIABLE_EFFORT )
	{
		// Default effort
		sBreathPerAP = APBPConstants[BP_PER_AP_MIN_EFFORT];

		// OK, check if we are in water and are waling/standing
		if ( pSoldier->MercInWater( ) )
		{
			switch( usAnimState )
			{
				case STANDING:

					sBreathPerAP = APBPConstants[BP_PER_AP_LT_EFFORT];
					break;

				case WALKING:

					sBreathPerAP = APBPConstants[BP_PER_AP_MOD_EFFORT];
					break;
			}
		}
		else
		{

			switch( usAnimState )
			{
				case STANDING:

					sBreathPerAP = APBPConstants[BP_PER_AP_NO_EFFORT];
					break;

				case WALKING:

					sBreathPerAP = APBPConstants[BP_PER_AP_LT_EFFORT];
					break;
			}
		}
		fAnimTypeFound = TRUE;
	}

	switch ( EFFORT( gAnimControl[ usAnimState ].uiFlags) ) {
		case 0: // No effort
			sBreathPerAP = APBPConstants[BP_PER_AP_NO_EFFORT];
			fAnimTypeFound = TRUE;
			break;
		case 1: // Minimal effort
			sBreathPerAP = APBPConstants[BP_PER_AP_MIN_EFFORT];
			fAnimTypeFound = TRUE;
			break;
		case 2: // Light effort
			sBreathPerAP = APBPConstants[BP_PER_AP_LT_EFFORT];
			fAnimTypeFound = TRUE;
			break;
		case 3: // Moderate effort
			sBreathPerAP = APBPConstants[BP_PER_AP_MOD_EFFORT];
			fAnimTypeFound = TRUE;
			break;
	}

	if ( !fAnimTypeFound )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Unknown end-of-turn breath anim: %s", gAnimControl[ usAnimState ].zAnimStr ) );
	}

	//rain
	// Reduce breath gain on 25%/rain intensity
	// Lalien: only for soldiers that are in loaded sector,
	if ( gfWorldLoaded &&  pSoldier->bInSector)
	{
		if( sBreathPerAP < 0 && ( pSoldier->pathing.bLevel  || !FindStructure( pSoldier->sGridNo, STRUCTURE_ROOF )  )  && pSoldier->bBreath > 1)
		{
			sBreathPerAP -= (INT16)( sBreathPerAP * gbCurrentRainIntensity * gGameExternalOptions.ubBreathGainReductionPerRainIntensity  / 100 );
		}
	}
	//end rain

	return( sBreathPerAP );
}

//UINT8 CalcAPsToBurst( INT8 bBaseActionPoints, UINT16 usItem )
INT16 CalcAPsToBurst( INT16 bBaseActionPoints, OBJECTTYPE * pObj )
{
	INT32 aps;

	// base APs is what you'd get from CalcActionPoints();
	// NB round UP, so 21-25 APs pay full
	aps =	( Weapon[ pObj->usItem ].bBurstAP * bBaseActionPoints + (APBPConstants[AP_MAXIMUM] - 1) ) / APBPConstants[AP_MAXIMUM];

	/*if ( GetPercentBurstFireAPReduction(pObj)>0 )
	{
		aps = (aps * 100) / (100 + GetBurstFireAPReductionStatus(pObj) / (100/GetPercentBurstFireAPReduction(pObj)));
	}

	if ( GetPercentAPReduction(pObj)>0 )
	{
		aps = (aps * 100) / (100 + GetAPReductionStatus(pObj) / (100/GetPercentAPReduction(pObj)));
	}*/

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToBurst: before bonus aps = %d, std bonus = %d, burst bonus = %d", aps,GetPercentAPReduction(pObj),GetPercentBurstFireAPReduction(pObj)));
	// Snap: do this a little differently: % reduction means
	// aps <- aps * ( 100 - red ) / 100
	aps = ( aps	* ( 100 - GetPercentAPReduction(pObj) ) ) / 100;

	// Snap: moved this up to allow burst AP reduction up to 100%
	aps = __max( aps, ( Weapon[ pObj->usItem ].bBurstAP + 1 ) / 2 );

	aps = ( aps * ( 100 - GetPercentBurstFireAPReduction(pObj) ) ) / 100;

	if ( aps < 0 ) aps = 0;
	else if ( aps > APBPConstants[AP_MAXIMUM] ) aps = APBPConstants[AP_MAXIMUM];

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToBurst: return aps = %d", aps));
	return (UINT8) aps;
}


INT16 CalcAPsToAutofire( INT16 bBaseActionPoints, OBJECTTYPE * pObj, UINT8 bDoAutofire )
{
//	INT8 bAttachPos;
	INT32 aps=APBPConstants[AP_MAXIMUM] + 1;
	if ( GetAutofireShotsPerFiveAPs (pObj) )
	{
		// Snap: Do this just like in CalcAPsToBurst

		const INT32 autofireaps =
		aps = ( ( APBPConstants[AUTOFIRE_SHOTS_AP_VALUE] * bDoAutofire * bBaseActionPoints ) / GetAutofireShotsPerFiveAPs(pObj) + (APBPConstants[AP_MAXIMUM] - 1) ) / APBPConstants[AP_MAXIMUM];

		//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutofire: base aps = %d, # shots = %d",aps,pSoldier->bDoAutofire ));
		//check for spring and bolt
		//bAttachPos = FindAttachment( &(pSoldier->inv[HANDPOS]), SPRING_AND_BOLT_UPGRADE );
		//if ( bAttachPos != -1 )
		//{
		//	aps = (aps * 100) / (100 + pSoldier->inv[HANDPOS].bAttachStatus[ bAttachPos ] / 5);
		//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutofire: found rod and spring, aps = %d, # shots = %d",aps,pSoldier->bDoAutofire ));
		//}
		//bAttachPos = FindAttachment( &(pSoldier->inv[HANDPOS]), REFLEX_SCOPED );
		//if ( bAttachPos != -1 )
		//{
		//	aps = (aps * 100) / (100 + pSoldier->inv[HANDPOS].bAttachStatus[ bAttachPos ] / 5);
		//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutofire: found reflex scope, aps = %d, # shots = %d",aps,pSoldier->bDoAutofire ));
		//}
		//bAttachPos = FindAttachment( &(pSoldier->inv[HANDPOS]), REFLEX_UNSCOPED );
		//if ( bAttachPos != -1 )
		//{
		//	aps = ( aps * 100 ) / (100 + pSoldier->inv[HANDPOS].bAttachStatus[ bAttachPos ] / 5);
		//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutofire: found reflex sight, aps = %d, # shots = %d",aps,pSoldier->bDoAutofire ));
		//}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutoFire: before bonus aps = %d, std bonus = %d, auto bonus = %d", aps,GetPercentAPReduction(pObj),GetPercentAutofireAPReduction(pObj)));
		aps = ( aps	* ( 100 - GetPercentAPReduction(pObj) ) ) / 100;

		aps = __max( aps, ( autofireaps + 1 ) / 2 );

		// Snap: extend the burst bonus to autofire as well?
		aps = ( aps * ( 100 - GetPercentAutofireAPReduction(pObj) ) ) / 100;

		if ( aps < 0 ) aps = 0;
		else if ( aps > APBPConstants[AP_MAXIMUM] ) aps = APBPConstants[AP_MAXIMUM];
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutoFire: return aps = %d", aps));
	return (UINT8) aps;
}


INT16 CalcTotalAPsToAttack( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost, INT16 bAimTime )
{
	UINT16						sAPCost = 0;
	UINT16						usItemNum;
	INT16							sActionGridNo;
	UINT8							ubDirection;
	INT16							sAdjustedGridNo;
	UINT32						uiItemClass;

	// LOOK IN BUDDY'S HAND TO DETERMINE WHAT TO DO HERE
	usItemNum = pSoldier->inv[HANDPOS].usItem;
	uiItemClass = Item[ usItemNum ].usItemClass;

	if ( uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER || uiItemClass == IC_TENTACLES || uiItemClass == IC_THROWING_KNIFE )
	{
		sAPCost = MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost );

		if ( pSoldier->bDoBurst )
		{
			if(pSoldier->bDoAutofire && GetAutofireShotsPerFiveAPs(&pSoldier->inv[HANDPOS]) > 0 )
				sAPCost += CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[HANDPOS]), pSoldier->bDoAutofire );
			else
				sAPCost += CalcAPsToBurst( pSoldier->CalcActionPoints( ), &(pSoldier->inv[HANDPOS]) );
		}
		else
		{
			sAPCost += (bAimTime*APBPConstants[AP_CLICK_AIM]);
		}
	}

	//ATE: HERE, need to calculate APs!
	if ( uiItemClass & IC_EXPLOSV )
	{
		sAPCost = MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost );

		//sAPCost = 5;
	}

	if ( uiItemClass == IC_PUNCH || ( uiItemClass == IC_BLADE && uiItemClass != IC_THROWING_KNIFE ) )
	{
		// IF we are at this gridno, calc min APs but if not, calc cost to goto this lication
		if ( pSoldier->sGridNo != sGridNo )
		{
			sAdjustedGridNo = NOWHERE;

			// OK, in order to avoid path calculations here all the time... save and check if it's changed!
			if ( pSoldier->sWalkToAttackGridNo == sGridNo )
			{
				sAdjustedGridNo = sGridNo;
				sAPCost += (UINT8)( pSoldier->sWalkToAttackWalkToCost );
			}
			else
			{
				//INT32		cnt;
				//INT16		sSpot;
				UINT8		ubGuyThere;
				INT16		sGotLocation = NOWHERE;
				BOOLEAN	fGotAdjacent = FALSE;
				SOLDIERTYPE	*pTarget;

				ubGuyThere = WhoIsThere2( sGridNo, pSoldier->pathing.bLevel );

				if ( ubGuyThere != NOBODY )
				{

					pTarget = MercPtrs[ ubGuyThere ];

					if ( pSoldier->ubBodyType == BLOODCAT )
					{
						sGotLocation = FindNextToAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
						if ( sGotLocation == -1 )
						{
							sGotLocation = NOWHERE;
						}
					}
					else
					{
						sGotLocation = FindAdjacentPunchTarget( pSoldier, pTarget, &sAdjustedGridNo, &ubDirection );
					}
				}

				if ( sGotLocation == NOWHERE && pSoldier->ubBodyType != BLOODCAT )
				{
					sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

					if ( sActionGridNo == -1 )
					{
						sGotLocation = NOWHERE;
					}
					else
					{
						sGotLocation = sActionGridNo;
					}
					fGotAdjacent = TRUE;
				}

				if ( sGotLocation != NOWHERE )
				{
					if (pSoldier->sGridNo == sGotLocation || !fGotAdjacent )
					{
						pSoldier->sWalkToAttackWalkToCost = 0;
					}
					else
					{
						// Save for next time...
						pSoldier->sWalkToAttackWalkToCost = PlotPath( pSoldier, sGotLocation, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

						if (pSoldier->sWalkToAttackWalkToCost == 0)
						{
							return( 99 );
						}
					}
				}
				else
				{
					return( 0 );
				}
				sAPCost += pSoldier->sWalkToAttackWalkToCost;
			}

			// Save old location!
			pSoldier->sWalkToAttackGridNo = sGridNo;

			// Add points to attack
			sAPCost += MinAPsToAttack( pSoldier, sAdjustedGridNo, ubAddTurningCost );
			//sAPCost += APsToTurnAround(pSoldier, sAdjustedGridNo);
		}
		else
		{
			// Add points to attack
			// Use our gridno
			sAPCost += MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost );
		}

		// Add aim time...
		sAPCost += (bAimTime*APBPConstants[AP_CLICK_AIM]);

	}

	return( (INT8)sAPCost );
}

INT16 MinAPsToAttack(SOLDIERTYPE *pSoldier, INT16 sGridno, UINT8 ubAddTurningCost, UINT8 ubForceRaiseGunCost)
{
	INT16						sAPCost = 0;
	UINT32						uiItemClass;

	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		INT16 glItem;
		// look for an attached grenade launcher

		glItem = GetAttachedGrenadeLauncher( &(pSoldier->inv[ HANDPOS ]) );
		if ( glItem == NONE )
		{
			// default to hand
			// LOOK IN BUDDY'S HAND TO DETERMINE WHAT TO DO HERE
			uiItemClass = Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass;
		}
		else
		{
			uiItemClass = Item[ glItem ].usItemClass;
		}
	}
	else
	{
		// LOOK IN BUDDY'S HAND TO DETERMINE WHAT TO DO HERE
		uiItemClass = Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass;
	}

	if ( uiItemClass == IC_BLADE || uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER || uiItemClass == IC_TENTACLES || uiItemClass == IC_THROWING_KNIFE )
	{
		sAPCost = MinAPsToShootOrStab( pSoldier, sGridno, ubAddTurningCost, ubForceRaiseGunCost );
	}
	else if ( uiItemClass & ( IC_GRENADE | IC_THROWN ) )
	{
		sAPCost = MinAPsToThrow( pSoldier, sGridno, ubAddTurningCost );
	}
	else if ( uiItemClass == IC_PUNCH )
	{
		sAPCost = MinAPsToPunch( pSoldier, sGridno, ubAddTurningCost );
	}

	return sAPCost;
}

INT8 CalcAimSkill( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	INT8 bAimSkill;

	if ( Item[ usWeapon ].usItemClass == IC_GUN || Item[ usWeapon ].usItemClass == IC_LAUNCHER )
	{
		// GUNS: modify aiming cost by shooter's MARKSMANSHIP
		bAimSkill = EffectiveMarksmanship( pSoldier );
	}
	else
	// for now use this for all other weapons
	//if ( Item[ usInHand ].usItemClass == IC_BLADE )
	{
		// KNIVES: modify aiming cost by avg of attacker's DEXTERITY & AGILITY
		bAimSkill = ( EffectiveDexterity( pSoldier ) + EffectiveAgility( pSoldier ) ) / 2;
		//return( 4 );
	}
	return( bAimSkill );
}

INT16 BaseAPsToShootOrStab( INT16 bAPs, INT16 bAimSkill, OBJECTTYPE * pObj )
{
	INT32	Top, Bottom;
	FLOAT	rof;
//	INT8	bAttachPos;

	//CHRISL: By changing rof to a FLOAT value, we resolve some of the rounding that causes APs to be skipped.  For example:
	//	assuming bAPs = 80 (default in a 100Ap system) and bAimSkill = 80 (also a default setting)
	//	If rof = 15, we return an AP cost of 24.  If rof = 16, we return an AP cost of 22.  With an integer value, we can't
	//	get an AP cost of 23.  But if rof = 15.5, we return an AP cost of 23 (22.94 which gets rounded up).  With an
	//	rof = 15.46, we return an AP cost of exactly 23.  So this will allow for fine control over "default" AP costs.

	// Calculate default top & bottom of the magic "aiming" formula!

	// get this man's maximum possible action points (ignoring carryovers)
	// the 2 times is here only to allow rounding off using integer math later
	//Top = 2 * bAPs;//pSoldier->CalcActionPoints( );

	// Shots per turn rating is for max. aimSkill(100), drops down to 1/2 at = 0
	// DIVIDE BY 4 AT THE END HERE BECAUSE THE SHOTS PER TURN IS NOW QUADRUPLED!
	// NB need to define shots per turn for ALL Weapons then.
	// 0verhaul:	Do not calculate rate of fire bonus for stackable items (knives)
	// Their info is an array of item status, not weapon info, and they don't repeat
	// fire anyway.
	rof = Weapon[ pObj->usItem ].ubShotsPer4Turns;
	//CHRISL: Resolves a problem with stackable weapons not using attachments like Reflex sight.  Found by Mugsy
	//if (ItemSlotLimit(pObj, STACK_SIZE_LIMIT) == 1)//NOT STACKABLE!
	{
		rof += GetRateOfFireBonus(pObj);
	}

	//Bottom = ( ( 50 + (bAimSkill / 2) ) * rof ) / 4;

	// add minimum aiming time to the overall minimum AP_cost
	//	 This here ROUNDS UP fractions of 0.5 or higher using integer math
	//	 This works because 'top' is 2x what it really should be throughout
	//return( ( ( ( 100 * Top ) / Bottom ) + 1) / 2);

	// Snap: Refactored the formula to reduce the number of integer divisions
	Top = 8 * bAPs;
	//CHRISL: Resolves a problem with stackable weapons not using attachments like Reflex sight.  Found by Mugsy
	//if (ItemSlotLimit(pObj, STACK_SIZE_LIMIT) == 1)
	{
		Top *= ( 100 - GetPercentAPReduction(pObj) );
	}
	//else
	//{
	//	Top *= 100;
	//}
	Bottom = (INT32)(( 100 + bAimSkill ) * rof);
	// (this will round to the nearest integer)
	return ( Top + Bottom / 2 ) / Bottom;
}

void GetAPChargeForShootOrStabWRTGunRaises( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost, BOOLEAN *pfChargeTurning, BOOLEAN *pfChargeRaise )
{
	UINT8 ubDirection;
	UINT32	uiMercFlags;
	UINT16	usTargID;
	BOOLEAN	fAddingTurningCost = FALSE;
	BOOLEAN	fAddingRaiseGunCost = FALSE;

	if ( sGridNo != NOWHERE )
	{
		// OK, get a direction and see if we need to turn...
		if (ubAddTurningCost)
		{
			// Given a gridno here, check if we are on a guy - if so - get his gridno
			if ( FindSoldier( sGridNo, &usTargID, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
			{
					sGridNo = MercPtrs[ usTargID ]->sGridNo;
			}

			ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, pSoldier );

			// Is it the same as he's facing?
			if ( ubDirection != pSoldier->ubDirection )
			{
					fAddingTurningCost = TRUE;
			}
		}
	}
	else
	{
		if (ubAddTurningCost)
		{
				// Assume we need to add cost!
				fAddingTurningCost = TRUE;
		}
	}


	if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_THROWING_KNIFE )
	{
	}
	else
	{
		// Do we need to ready weapon?
		if ( !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) )
		{
			fAddingRaiseGunCost = TRUE;
		}
		//CHRISL: Handle ready weapon with facing changes based on stance
		if(fAddingTurningCost)
		{
			switch (gAnimControl[ pSoldier->usAnimState ].ubHeight)
			{
				case ANIM_STAND:
					fAddingRaiseGunCost = GetDirectionChangeAmount(sGridNo, pSoldier, 3);
					break;
				case ANIM_CROUCH:
					fAddingRaiseGunCost = GetDirectionChangeAmount(sGridNo, pSoldier, 1);
					break;
				case ANIM_PRONE:
					fAddingRaiseGunCost = TRUE;
					break;
			}
		}
	}

	(*pfChargeTurning) = fAddingTurningCost;
	(*pfChargeRaise )	= fAddingRaiseGunCost;
}

UINT16 CalculateTurningCost(SOLDIERTYPE *pSoldier, UINT16 usItem, BOOLEAN fAddingTurningCost)
{
	UINT16	usTurningCost = 0;

	if ( fAddingTurningCost )
	{
		if ( Item[ usItem ].usItemClass == IC_THROWING_KNIFE )
			usTurningCost = APBPConstants[AP_LOOK_STANDING];
		else
			usTurningCost = GetAPsToLook( pSoldier );
	}

	return usTurningCost;
}

UINT16 CalculateRaiseGunCost(SOLDIERTYPE *pSoldier, BOOLEAN fAddingRaiseGunCost)
{
	UINT16	usRaiseGunCost = 0;

	if (fAddingRaiseGunCost )
	{
		usRaiseGunCost = GetAPsToReadyWeapon( pSoldier, pSoldier->usAnimState );
		pSoldier->flags.fDontChargeReadyAPs = FALSE;
	}

	return usRaiseGunCost;
}

INT16 MinAPsToShootOrStab(SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost, UINT8 ubForceRaiseGunCost)
{
	UINT32	uiMercFlags;
	UINT16	usTargID;
	INT16	bFullAPs;
	INT8 bAimSkill;
	INT16	bAPCost = APBPConstants[AP_MIN_AIM_ATTACK];
	BOOLEAN	fAddingTurningCost = FALSE;
	BOOLEAN	fAddingRaiseGunCost = FALSE;
	UINT16 usItem;
	UINT16 usRaiseGunCost = 0;
	UINT16 usTurningCost = 0;


	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		usItem = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS] );//UNDER_GLAUNCHER;
	}
	else
	{
		usItem = pSoldier->inv[ HANDPOS ].usItem;
	}


	GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sGridNo, ubAddTurningCost, &fAddingTurningCost, &fAddingRaiseGunCost );

	if (ubForceRaiseGunCost)
	{
		fAddingRaiseGunCost = TRUE;
	}

	if ( Item[ usItem ].usItemClass == IC_THROWING_KNIFE )
	{
		// Do we need to stand up?
		bAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
	}

	if ( AM_A_ROBOT( pSoldier ) )
	{
		fAddingRaiseGunCost = FALSE;
	}

	//Calculate usTurningCost
	usTurningCost = CalculateTurningCost(pSoldier, usItem, fAddingTurningCost);

	//Calculate usRaiseGunCost
	usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost);

	//charge for turn and/or raise based on the results of fAddingTurningCost & fAddingRaiseGunCost
	bAPCost += (usTurningCost + usRaiseGunCost);


	if ( sGridNo != NOWHERE )
	{
		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if ( FindSoldier( sGridNo, &usTargID, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
		{
				sGridNo = MercPtrs[ usTargID ]->sGridNo;
		}
	}

	// if attacking a new target (or if the specific target is uncertain)
	if (ubForceRaiseGunCost || (( sGridNo != pSoldier->sLastTarget ) && !Item[usItem].rocketlauncher ))
	{
		bAPCost += APBPConstants[AP_CHANGE_TARGET];
	}

	// Snap: reversed DIGICRAB's change.
	// bFullAPs are BASE APs, which do not include APs caried over from the previous turn.
	bFullAPs = pSoldier->CalcActionPoints( );

	// aim skill is the same whether we are using 1 or 2 guns
	bAimSkill = CalcAimSkill( pSoldier, usItem );

	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		INT16 glItem;
		OBJECTTYPE	GrenadeLauncher;

		// look for an attached grenade launcher
		glItem = GetAttachedGrenadeLauncher( &(pSoldier->inv[ HANDPOS ]));

		// create temporary grenade launcher and use that
		if ( glItem != NONE )
		{
			CreateItem( glItem, GetGrenadeLauncherStatus(&pSoldier->inv[ HANDPOS ]), &GrenadeLauncher );
		}
		else
		{
			// fake it, use a 100 status...
			CreateItem( UNDER_GLAUNCHER, 100, &GrenadeLauncher );
		}

	bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, &GrenadeLauncher );

	}
	else if ( pSoldier->IsValidSecondHandShot( ) )
	{
	// charge the maximum of the two
	bAPCost += __max(
			BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) ),
			BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[SECONDHANDPOS]) ) );
	}
	else
	{
	bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) );
	}

	// the minimum AP cost of ANY shot can NEVER be more than merc's maximum APs!
	if ( bAPCost > bFullAPs )
		bAPCost = bFullAPs;

	// this SHOULD be impossible, but nevertheless...
	if ( bAPCost < 1 )
		bAPCost = 1;

	if ( Item[pSoldier->inv[HANDPOS].usItem].rocketlauncher )
	{
		bAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
	}


	return bAPCost;
}


INT16 MinAPsToPunch(SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost)
{
	UINT8	bAPCost = 0;
	UINT16 usTargID;
	UINT8	ubDirection;

	//	bAimSkill = ( pSoldier->stats.bDexterity + pSoldier->stats.bAgility) / 2;
	if ( sGridNo != NOWHERE )
	{
		usTargID = WhoIsThere2( sGridNo, pSoldier->bTargetLevel );

		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if ( usTargID != NOBODY  )
		{
			sGridNo = MercPtrs[ usTargID ]->sGridNo;

			// Check if target is prone, if so, calc cost...
			if ( gAnimControl[ MercPtrs[ usTargID ]->usAnimState ].ubEndHeight == ANIM_PRONE )
		 {
			 bAPCost += GetAPsToChangeStance( pSoldier, ANIM_CROUCH );
		 }
			else
		 {
			 if ( pSoldier->sGridNo == sGridNo )
			 {
				 bAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
			 }
		 }

		}

		if (ubAddTurningCost)
		{
			if ( pSoldier->sGridNo == sGridNo )
			{
				// ATE: Use standing turn cost....
				ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, pSoldier );

				// Is it the same as he's facing?
				if ( ubDirection != pSoldier->ubDirection )
				{
					bAPCost += APBPConstants[AP_LOOK_STANDING];
				}
			}
		}

	}

	bAPCost += APBPConstants[AP_PUNCH];

	return ( bAPCost );
}



INT16 MinPtsToMove(SOLDIERTYPE *pSoldier)
{
	// look around all 8 directions and return lowest terrain cost
	UINT8	cnt;
	INT16	sLowest=127;
	INT16	sGridno,sCost;

	if ( TANK( pSoldier ) )
	{
		return( (INT8)sLowest);
	}

	for (cnt=0; cnt <= 8; cnt++)
	{
		sGridno = NewGridNo(pSoldier->sGridNo,DirectionInc(cnt));
		if (sGridno != pSoldier->sGridNo)
			{
			if ( (sCost=ActionPointCost( pSoldier, sGridno, cnt , pSoldier->usUIMovementMode ) ) < sLowest )
			{
					sLowest = sCost;
			}
		}
	}
	return( (INT8)sLowest);
}

INT8	PtsToMoveDirection(SOLDIERTYPE *pSoldier, INT8 bDirection )
{
	INT16	sGridno,sCost;
	INT8	bOverTerrainType;
	UINT16	usMoveModeToUse;

	sGridno = NewGridNo( pSoldier->sGridNo, DirectionInc( bDirection ) );

	usMoveModeToUse = pSoldier->usUIMovementMode;

	// ATE: Check if the new place is watter and we were tying to run....
	bOverTerrainType = GetTerrainType( sGridno );

	if ( TERRAIN_IS_WATER( bOverTerrainType) )
	{
		usMoveModeToUse = WALKING;
	}

	sCost = ActionPointCost( pSoldier, sGridno, bDirection , usMoveModeToUse );

	if ( gubWorldMovementCosts[ sGridno ][ bDirection ][ pSoldier->pathing.bLevel ] != TRAVELCOST_FENCE )
	{
		if ( usMoveModeToUse == RUNNING && pSoldier->usAnimState != RUNNING )
		{
			sCost += APBPConstants[AP_START_RUN_COST];
		}
	}

	return( (INT8)sCost );
}

INT8 MinAPsToStartMovement( SOLDIERTYPE * pSoldier, UINT16 usMovementMode )
{
	INT8	bAPs = 0;

	switch( usMovementMode )
	{
		case RUNNING:
		case WALKING:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				bAPs += APBPConstants[AP_CROUCH] + APBPConstants[AP_PRONE];
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH)
			{
				bAPs += APBPConstants[AP_CROUCH];
			}
			break;
		case SWATTING:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				bAPs += APBPConstants[AP_PRONE];
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
			{
				bAPs += APBPConstants[AP_CROUCH];
			}
			break;
		case CRAWLING:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
			{
				bAPs += APBPConstants[AP_CROUCH] + APBPConstants[AP_PRONE];
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH)
			{
				bAPs += APBPConstants[AP_CROUCH];
			}
			break;
		default:
			break;
	}

	if (usMovementMode == RUNNING && pSoldier->usAnimState != RUNNING )
	{
		bAPs += APBPConstants[AP_START_RUN_COST];
	}
	return( bAPs );
}

BOOLEAN EnoughAmmo( SOLDIERTYPE *pSoldier, BOOLEAN fDisplay, INT8 bInvPos )
{
	if ( pSoldier->inv[ bInvPos ].exists() == true )
	{
		if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
		{
			UINT16 glItem = GetAttachedGrenadeLauncher( &(pSoldier->inv[bInvPos]));
			OBJECTTYPE* pAttachment = FindLaunchableAttachment( &(pSoldier->inv[bInvPos]), glItem);
			if ( pAttachment )
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if ( Item[pSoldier->inv[ bInvPos ].usItem].singleshotrocketlauncher	)
			{
				// hack... they turn empty afterwards anyways
				return( TRUE );
			}

			if (Item[ pSoldier->inv[ bInvPos ].usItem ].usItemClass == IC_LAUNCHER || Item[pSoldier->inv[ bInvPos ].usItem].cannon )
			{
				if ( FindAttachmentByClass( &(pSoldier->inv[ bInvPos ]), IC_GRENADE ) != 0 )
				{
					return( TRUE );
				}

				// ATE: Did an else if here...
				if ( FindAttachmentByClass( &(pSoldier->inv[ bInvPos ]), IC_BOMB ) != 0 )
				{
					return( TRUE );
				}

				if ( fDisplay )
				{
					TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
				}

				return( FALSE );
			}
			else if (Item[ pSoldier->inv[ bInvPos ].usItem ].usItemClass == IC_GUN)
			{
				if ( pSoldier->inv[ bInvPos ][0]->data.gun.ubGunShotsLeft == 0 )
				{
					if ( fDisplay )
					{
						TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
					}
					return( FALSE );
				}

				//<SB> manual recharge
				if( pSoldier->bTeam == OUR_TEAM )
				{
					if ( !(	pSoldier->inv[ bInvPos ][0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER ) )
					{
						return( FALSE );
					}
				}
				//</SB>
			}
		}

		return( TRUE );
	}

	return( FALSE );

}


void DeductAmmo( SOLDIERTYPE *pSoldier, INT8 bInvPos )
{
	OBJECTTYPE *		pObj;

	// tanks never run out of MG ammo!
	// unlimited cannon ammo is handled in AI
	if ( TANK( pSoldier ) && !Item[pSoldier->inv[bInvPos].usItem].cannon )
	{
		return;
	}

	pObj = &(pSoldier->inv[ bInvPos ]);
	if ( pObj->exists() == true )
	{
		if ( Item[pObj->usItem].cannon )
		{
		}
		else if ( Item[ pObj->usItem ].usItemClass == IC_GUN && !Item[pObj->usItem].cannon && pSoldier->bWeaponMode != WM_ATTACHED_GL && pSoldier->bWeaponMode != WM_ATTACHED_GL_BURST && pSoldier->bWeaponMode != WM_ATTACHED_GL_AUTO )
		{
			if ( pSoldier->usAttackingWeapon == pObj->usItem)
			{
				// OK, let's see, don't overrun...
				if ( (*pObj)[0]->data.gun.ubGunShotsLeft != 0 )
				{
					(*pObj)[0]->data.gun.ubGunShotsLeft--;
					//Pulmu: Update weight after firing gun to account for bullets fired
					if( gGameExternalOptions.fAmmoDynamicWeight == TRUE)
					{
						//ADB ubWeight has been removed, see comments in OBJECTTYPE
						//pSoldier->inv[HANDPOS].ubWeight = CalculateObjectWeight( &(pSoldier->inv[HANDPOS]));
					}
				}
			}
			else
			{
				// firing an attachment?
			}
		}
		else if ( Item[ pObj->usItem ].usItemClass == IC_LAUNCHER || Item[pObj->usItem].cannon || pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
		{
			OBJECTTYPE* pAttachment = FindAttachmentByClass( pObj, IC_GRENADE );
			if ( pAttachment == 0 )
			{
				pAttachment = FindAttachmentByClass( pObj, IC_BOMB );
			}

			if (pAttachment)
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for GL: found attachment, about to decide whether to remove it"));
				if ( Item[ pObj->usItem ].usItemClass == IC_LAUNCHER && GetMagSize(pObj) > 1 )
				{
					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for milkor"));
					(*pAttachment)[0]->data.objectStatus -= (INT8) ceil((double)( 100 / GetMagSize(pObj) )) ;

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for milkor: resulting status: %d, remove? = %d",(*pAttachment)[0]->data.objectStatus,((*pAttachment)[0]->data.objectStatus <= (INT8) ( 100 / Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize ))));
					if ( (*pAttachment)[0]->data.objectStatus <= (INT8) ceil((double)( 100 / GetMagSize(pObj) ) ))
					{
						pObj->RemoveAttachment( pAttachment);
					}
				}
				else if ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO ) && Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize > 1 )
				{
					(*pAttachment)[0]->data.objectStatus -= (INT8) ceil((double)( 100 / Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize )) ;

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for OICW GL: resulting status: %d, remove? = %d",(*pAttachment)[0]->data.objectStatus,((*pAttachment)[0]->data.objectStatus <= (INT8) ( 100 / Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize ))));
					if ( (*pAttachment)[0]->data.objectStatus <= (INT8) ceil((double)( 100 / Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize ) ))
					{
						pObj->RemoveAttachment( pAttachment);
					}
				}
				else
				{
					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for GL: removing attachment"));
					pObj->RemoveAttachment( pAttachment);
				}
			}
		}

		// Dirty Bars
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL1 );

	}
}


INT16 GetAPsToPickupItem( SOLDIERTYPE *pSoldier, INT16 sMapPos )
{
	ITEM_POOL					*pItemPool;
	UINT16						sAPCost = 0;
	INT16							sActionGridNo;

	// Check if we are over an item pool
	if ( GetItemPool( sMapPos, &pItemPool, pSoldier->pathing.bLevel ) )
	{
		// If we are in the same tile, just return pickup cost
		sActionGridNo = AdjustGridNoForItemPlacement( pSoldier, sMapPos );

		if ( pSoldier->sGridNo != sActionGridNo )
		{
			sAPCost = PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (INT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

			// If point cost is zero, return 0
			if ( sAPCost != 0 )
			{
				// ADD APS TO PICKUP
				sAPCost += APBPConstants[AP_PICKUP_ITEM];
			}
		}
		else
		{
				sAPCost += APBPConstants[AP_PICKUP_ITEM];
		}
	}

	return( sAPCost );
}


INT16 GetAPsToGiveItem( SOLDIERTYPE *pSoldier, INT16 sMapPos )
{
	UINT16						sAPCost = 0;

	sAPCost = PlotPath( pSoldier, sMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 || pSoldier->sGridNo == sMapPos )
	{
		// ADD APS TO PICKUP
		sAPCost += APBPConstants[AP_GIVE_ITEM];
	}

	return( sAPCost );
}


INT16 GetAPsToReloadGunWithAmmo( SOLDIERTYPE *pSoldier, OBJECTTYPE * pGun, OBJECTTYPE * pAmmo, BOOLEAN usAllAPs )
{
	if (Item[ pGun->usItem ].usItemClass == IC_LAUNCHER)
	{
		// always standard AP cost
		//return( APBPConstants[AP_RELOAD_GUN] );
		return GetAPsToReload(pGun);
	}
	if(Weapon[pGun->usItem].swapClips == 1 && GetMagSize(pGun) == Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize)
	{
		// normal situation
		return GetAPsToReload(pGun) ;
	}
	else if(Weapon[pGun->usItem].swapClips == 1)
	{
		// wrong size ammo item
		return (INT8)((GetAPsToReload(pGun) * APBPConstants[AP_WRONG_MAG])/10);
	}
	else
	{
		// individually loaded guns
		int reload = 0;
		int rounds = __min((GetMagSize(pGun)-(*pGun)[0]->data.gun.ubGunShotsLeft), Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize);
		if(!((*pGun)[0]->data.gun.ubGunState & GS_WEAPON_BEING_RELOADED))
			reload = GetAPsToReload(pGun);
		if(usAllAPs == 1)
		{
			for(int i = 1; i <= rounds; i++)
			{
				if(EnoughPoints(pSoldier,(reload + (i * APBPConstants[AP_RELOAD_LOOSE])),0,FALSE) == FALSE)
				{
					rounds = i-1;
					break;
				}
			}
		}
		else if (usAllAPs == 2)
			rounds = 1;
		return (reload + (rounds * APBPConstants[AP_RELOAD_LOOSE]));
	}
}

INT16 GetAPsToAutoReload( SOLDIERTYPE * pSoldier )
{
	OBJECTTYPE *	pObj;
	INT8					bSlot, bSlot2, bExcludeSlot;
	INT16					bAPCost = 0, bAPCost2 = 0;;

	CHECKF( pSoldier );
	pObj = &(pSoldier->inv[HANDPOS]);

//<SB> manual recharge
	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
	{
		return Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually;
	}
//</SB>

	if (Item[pObj->usItem].usItemClass == IC_GUN || Item[pObj->usItem].usItemClass == IC_LAUNCHER)
	{
		bSlot = FindAmmoToReload( pSoldier, HANDPOS, NO_SLOT );
		if (bSlot != NO_SLOT)
		{
			// we would reload using this ammo!
			bAPCost += GetAPsToReloadGunWithAmmo( pSoldier, pObj, &(pSoldier->inv[bSlot] ) );
		}

		if ( pSoldier->IsValidSecondHandShotForReloadingPurposes( ) )
		{
			pObj = &(pSoldier->inv[SECONDHANDPOS]);
			bExcludeSlot = NO_SLOT;
			bSlot2 = NO_SLOT;

			// if the ammo for the first gun is the same we have to do special checks
			if ( ValidAmmoType( pObj->usItem, pSoldier->inv[ bSlot ].usItem ) )
			{
				if ( pSoldier->inv[ bSlot ].ubNumberOfObjects == 1 )
				{
					// we must not consider this slot for reloading!
					bExcludeSlot = bSlot;
				}
				else
				{
					// we can reload the 2nd gun from the same pocket!
					bSlot2 = bSlot;
				}
			}

			if (bSlot2 == NO_SLOT)
			{
				bSlot2 = FindAmmoToReload( pSoldier, SECONDHANDPOS, bExcludeSlot );
			}

			if (bSlot2 != NO_SLOT)
			{
				// we would reload using this ammo!
				bAPCost2 = GetAPsToReloadGunWithAmmo( pSoldier, pObj, &(pSoldier->inv[bSlot2] ) );
				if ( EnoughPoints( pSoldier, (INT16) (bAPCost + bAPCost2), 0, FALSE ) )
				{
					// we can afford to reload both guns; otherwise display just for 1 gun
					bAPCost += bAPCost2;
				}
			}

		}

	}

	return( bAPCost );
}

UINT16 GetAPsToReloadRobot( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pRobot )
{
	UINT16						sAPCost = 0;
	INT16			 sActionGridNo;
	UINT8			 ubDirection;
	INT16			 sAdjustedGridNo;

	sActionGridNo = FindAdjacentGridEx( pSoldier, pRobot->sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

	sAPCost = PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 || sActionGridNo == pSoldier->sGridNo )
	{
		// ADD APS TO RELOAD
		sAPCost += 4;
	}

	return( sAPCost );
}




INT16 GetAPsToChangeStance( SOLDIERTYPE *pSoldier, INT8 bDesiredHeight )
{
	UINT16						sAPCost = 0;
	INT8							bCurrentHeight;

	bCurrentHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;

	if ( bCurrentHeight == bDesiredHeight )
	{
		sAPCost = 0;
	}

	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_PRONE )
	{
		sAPCost = APBPConstants[AP_CROUCH] + APBPConstants[AP_PRONE];
	}
	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_CROUCH )
	{
		sAPCost = APBPConstants[AP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_PRONE )
	{
		sAPCost = APBPConstants[AP_PRONE];
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_STAND )
	{
		sAPCost = APBPConstants[AP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_STAND )
	{
		sAPCost = APBPConstants[AP_PRONE] + APBPConstants[AP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_CROUCH )
	{
		sAPCost = APBPConstants[AP_PRONE];
	}

	return( sAPCost );
}

INT16 GetBPsToChangeStance( SOLDIERTYPE *pSoldier, INT8 bDesiredHeight )
{
	UINT16						sBPCost = 0;
	INT8							bCurrentHeight;

	bCurrentHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;

	if ( bCurrentHeight == bDesiredHeight )
	{
		sBPCost = 0;
	}

	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_PRONE )
	{
		sBPCost = APBPConstants[BP_CROUCH] + APBPConstants[BP_PRONE];
	}
	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_CROUCH )
	{
		sBPCost = APBPConstants[BP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_PRONE )
	{
		sBPCost = APBPConstants[BP_PRONE];
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_STAND )
	{
		sBPCost = APBPConstants[BP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_STAND )
	{
		sBPCost = APBPConstants[BP_PRONE] + APBPConstants[BP_CROUCH];
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_CROUCH )
	{
		sBPCost = APBPConstants[BP_PRONE];
	}

	return( sBPCost );
}


INT16 GetAPsToLook( SOLDIERTYPE *pSoldier )
{
	// Set # of APs
	switch( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
		// Now change to appropriate animation
		case ANIM_STAND:
			return( APBPConstants[AP_LOOK_STANDING] );
			break;

		case ANIM_CROUCH:
			return( APBPConstants[AP_LOOK_CROUCHED] );
			break;

		case ANIM_PRONE:
			// APBPConstants[AP_PRONE] is the AP cost to go to or from the prone stance.	To turn while prone, your merc has to get up to
			// crouched, turn, and then go back down.	Hence you go up (APBPConstants[AP_PRONE]), turn (APBPConstants[AP_LOOK_PRONE]) and down (APBPConstants[AP_PRONE]).
			return( APBPConstants[AP_LOOK_PRONE] + APBPConstants[AP_PRONE] + APBPConstants[AP_PRONE] );
			break;

		// no other values should be possible
		default:
			Assert( FALSE );
			return(0);
			break;
	}
}


BOOLEAN CheckForMercContMove( SOLDIERTYPE *pSoldier )
{
	INT16 sAPCost;
	INT16	sGridNo;

	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		return( FALSE );
	}

	if ( gpItemPointer != NULL )
	{
		return( FALSE );
	}

	if( pSoldier->stats.bLife >= OKLIFE )
	{
		if( pSoldier->sGridNo != pSoldier->pathing.sFinalDestination || pSoldier->bGoodContPath	)
		{
			// OK< check if we are the selected guy!
			if ( pSoldier->ubID == gusSelectedSoldier )
			{
				if (SoldierOnScreen( pSoldier->ubID ) )
				{
					sGridNo = pSoldier->pathing.sFinalDestination;

					if ( pSoldier->bGoodContPath )
					{
						sGridNo = pSoldier->sContPathLocation;
					}

					// Do a check if we can afford move here!

					// get a path to dest...
					if ( FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, pSoldier->usUIMovementMode, NO_COPYROUTE, 0 ) )
					{
						sAPCost = PtsToMoveDirection( pSoldier, (UINT8)guiPathingData[ 0 ] );

						if ( EnoughPoints( pSoldier, sAPCost, 0 , FALSE ) )
						{
							return( TRUE );
						}
					}
					else
					{
						return( FALSE );
					}
				}
			}
		}
	}
	return( FALSE );
}


INT16 GetAPsToReadyWeapon( SOLDIERTYPE *pSoldier, UINT16 usAnimState )
{
	UINT16 usItem;

	usItem = pSoldier->inv[ HANDPOS ].usItem;

	// If this is a dwel pistol anim
	// ATE: What was I thinking, hooking into animations like this....
	//if ( usAnimState == READY_DUAL_STAND || usAnimState == READY_DUAL_CROUCH )
	//{
		//return( APBPConstants[AP_READY_DUAL] );
	//}
	if ( pSoldier->IsValidSecondHandShot( ) )
	{
		//Madd: return the greater of the two weapons + 1:
		UINT8 rt1, rt2;
		rt1 = Weapon[usItem].ubReadyTime * ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) ) / 100;
		rt2 = Weapon[pSoldier->inv[SECONDHANDPOS].usItem].ubReadyTime * ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ SECONDHANDPOS ]) ) / 100;

		return( max(rt1,rt2) + APBPConstants[AP_READY_DUAL] );
	}


	// OK, now check type of weapon

	if ( usItem == NOTHING )
	{
		return( 0 );
	}
	else
	{
		// CHECK FOR RIFLE
		if ( Item[ usItem ].usItemClass == IC_GUN )
		{
			//ADB if 0, which it normally is, avoid the function call and division
			if (Weapon[ usItem ].ubReadyTime == 0) {
				return 0;
			}
			else {
				return ( Weapon[ usItem ].ubReadyTime
					* ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) ) ) / 100;
			}
		}
	}

	return( 0 );
}


INT16 GetAPsToClimbRoof( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	if ( !fClimbDown )
	{
		// OK, add aps to goto stand stance...
		return(( APBPConstants[AP_CLIMBROOF] + GetAPsToChangeStance( pSoldier, ANIM_STAND ) ) );
	}
	else
	{
		// Add aps to goto crouch
		return(( APBPConstants[AP_CLIMBOFFROOF] + GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) ) );
	}
}


INT16 GetBPsToClimbRoof( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	if ( !fClimbDown )
	{
		return( APBPConstants[BP_CLIMBROOF] );
	}
	else
	{
		return( APBPConstants[BP_CLIMBOFFROOF] );
	}
}


INT16 GetAPsToCutFence( SOLDIERTYPE *pSoldier )
{
	// OK, it's normally just cost, but add some if different stance...
	return(	GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) + APBPConstants[AP_USEWIRECUTTERS] );
}

INT16 GetAPsToBeginFirstAid( SOLDIERTYPE *pSoldier )
{
	// OK, it's normally just cost, but add some if different stance...
	return(	GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) + APBPConstants[AP_START_FIRST_AID] );
}

INT16 GetAPsToBeginRepair( SOLDIERTYPE *pSoldier )
{
	// OK, it's normally just cost, but add some if different stance...
	return(	GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) + APBPConstants[AP_START_REPAIR] );
}

INT16 GetAPsToRefuelVehicle( SOLDIERTYPE *pSoldier )
{
	// OK, it's normally just cost, but add some if different stance...
	return( GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) + APBPConstants[AP_REFUEL_VEHICLE] );
}


#define TOSSES_PER_10TURNS		18		// max # of grenades tossable in 10 turns
//COMMENTED OUT, AS THEY WERE
//1. ALREADY DEFINED IN POINTS.H TO BEGIN WITH
//2. EXTERNALIZED BY GOTTHARD 7/2/08
//#define APBPConstants[AP_MIN_AIM_ATTACK]		0		// minimum permitted extra aiming
//#define APBPConstants[AP_MAX_AIM_ATTACK]		4		// maximum permitted extra aiming


INT16 MinAPsToThrow( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost )
{
	INT32 iTop, iBottom;
	INT32	iFullAPs;
	INT32 iAPCost = APBPConstants[AP_MIN_AIM_ATTACK];
	UINT16 usInHand;
	UINT16 usTargID;
	UINT32 uiMercFlags;
	UINT8 ubDirection;

	// make sure the guy's actually got a throwable item in his hand!
	usInHand = pSoldier->inv[ HANDPOS ].usItem;

	if ( ( !(Item[ usInHand ].usItemClass & IC_GRENADE) ) )
	{
		//AXP 25.03.2007: See if we are about to throw grenade (grenade was not in hand, but in temp object)
		if ( pSoldier->pTempObject != NULL && pSoldier->pThrowParams != NULL &&
			pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM && (Item[ pSoldier->pTempObject->usItem ].usItemClass & IC_GRENADE) )
		{
			//nothing here
		}
		else
		{
#ifdef JA2TESTVERSION
			ScreenMsg( MSG_FONT_YELLOW, MSG_DEBUG, L"MinAPsToThrow - Called when in-hand item is %d", usInHand );
#endif
			return(0);
		}
	}

	if ( sGridNo != NOWHERE )
	{
		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if ( FindSoldier( sGridNo, &usTargID, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
		{
			sGridNo = MercPtrs[ usTargID ]->sGridNo;
		}

		// OK, get a direction and see if we need to turn...
		if (ubAddTurningCost)
		{
			ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, pSoldier );

			// Is it the same as he's facing?
			if ( ubDirection != pSoldier->ubDirection )
			{
				//Lalien: disabled it again
				//AXP 25.03.2007: Reenabled look cost
				//iAPCost += GetAPsToLook( pSoldier );
			}
		}
	}
	else
	{
		// Assume we need to add cost!
		//iAPCost += GetAPsToLook( pSoldier );
	}

	// if attacking a new target (or if the specific target is uncertain)
	//AXP 25.03.2007: Aim-at-same-tile AP cost/bonus doesn't make any sense for thrown objects
	//if ( ( sGridNo != pSoldier->sLastTarget ) )
	//{
	//	iAPCost += APBPConstants[AP_CHANGE_TARGET];
	//}

	iAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );


	// Calculate default top & bottom of the magic "aiming" formula)

	// get this man's maximum possible action points (ignoring carryovers)
	iFullAPs = pSoldier->CalcActionPoints( );

	// the 2 times is here only to around rounding off using integer math later
	iTop = 2 * iFullAPs;

	// if it's anything but a mortar
	//	if ( usInHand != MORTAR)
	// tosses per turn is for max dexterity, drops down to 1/2 at dexterity = 0
	// bottom = (TOSSES_PER_10TURNS * (50 + (ptr->dexterity / 2)) / 10);
	//else
	iBottom = ( TOSSES_PER_10TURNS * (50 + ( pSoldier->stats.bDexterity / 2 ) ) / 10 );


	// add minimum aiming time to the overall minimum AP_cost
	//	 This here ROUNDS UP fractions of 0.5 or higher using integer math
	//	 This works because 'top' is 2x what it really should be throughout
	iAPCost += ( ( ( 100 * iTop ) / iBottom) + 1) / 2;


	// the minimum AP cost of ANY throw can NEVER be more than merc has APs!
	if ( iAPCost > iFullAPs )
		iAPCost = iFullAPs;

	// this SHOULD be impossible, but nevertheless...
	if ( iAPCost < 1 )
		iAPCost = 1;


	return ( (INT16)iAPCost );
}

INT16 GetAPsToDropBomb( SOLDIERTYPE *pSoldier )
{
	return( APBPConstants[AP_DROP_BOMB] );
}

INT16 GetAPsToPlantMine( SOLDIERTYPE *pSoldier )
{
	return( APBPConstants[AP_BURY_MINE] );
}

INT16 GetTotalAPsToDropBomb( SOLDIERTYPE *pSoldier, INT16 sGridNo )
{
	INT16 sAPs = 0;

	sAPs = PlotPath( pSoldier, sGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	if ( sAPs > 0 )
	{
		if(Item[pSoldier->inv[HANDPOS].usItem].mine == 1)
			sAPs += APBPConstants[AP_BURY_MINE];
		else
			sAPs += APBPConstants[AP_DROP_BOMB];
	}

	return( sAPs );
}



INT16 GetAPsToUseRemote( SOLDIERTYPE *pSoldier )
{
	return( APBPConstants[AP_USE_REMOTE] );
}


INT16 GetAPsToStealItem( SOLDIERTYPE *pSoldier, INT16 sMapPos )
{
	INT16	sAPCost = 0;

	if (sMapPos != -1)
	{
		sAPCost = PlotPath( pSoldier, sMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
	}

	// ADD APS TO PICKUP
	sAPCost += APBPConstants[AP_STEAL_ITEM];

	// CJC August 13 2002: added cost to stand into equation
	if (!(PTR_STANDING))
	{
		sAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
	}

	return sAPCost;

}

INT16 GetBPsToStealItem( SOLDIERTYPE *pSoldier )
{
	return( APBPConstants[BP_STEAL_ITEM] );
}


INT16 GetAPsToUseJar( SOLDIERTYPE *pSoldier, INT16 sMapPos )
{
	INT16						sAPCost = 0;

	sAPCost = PlotPath( pSoldier, sMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 )
	{
		// ADD APS TO PICKUP
		sAPCost += APBPConstants[AP_TAKE_BLOOD];
	}

	return sAPCost;

}

INT16 GetAPsToUseCan( SOLDIERTYPE *pSoldier, INT16 sMapPos )
{
	INT16	sAPCost = 0;

	sAPCost = PlotPath( pSoldier, sMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 )
	{
		// ADD APS TO PICKUP
		sAPCost += APBPConstants[AP_ATTACH_CAN];
	}

	return sAPCost;

}


INT16 GetAPsToJumpOver( SOLDIERTYPE *pSoldier )
{
	return(	GetAPsToChangeStance( pSoldier, ANIM_STAND ) + APBPConstants[AP_JUMP_OVER] );
}
