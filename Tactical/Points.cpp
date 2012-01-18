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
	#include "Soldier Control.h"	// added by SANDRO 
	#include "opplist.h"	// added by SANDRO 
	#include "lighting.h"	// added by SANDRO 
#endif
#include "connect.h"
//rain
//#define BREATH_GAIN_REDUCTION_PER_RAIN_INTENSITY 25
//end rain

extern BOOLEAN IsValidSecondHandShot( SOLDIERTYPE *pSoldier );

//////////////////////////////////////////////////////////////////////////////
// SANDRO - In this file, all APBPConstants[AP_CROUCH] and APBPConstants[AP_PRONE] were changed to GetAPsCrouch() and GetAPsProne()
//			On the bottom here, there are these functions made
//////////////////////////////////////////////////////////////////////

INT16 GetBreathPerAP( SOLDIERTYPE *pSoldier, UINT16 usAnimState );

INT16 TerrainActionPoints( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bDir, INT8 bLevel )
{
	// SANDRO - Note: this procedure was changed a bit
		INT16	sAPCost = 0;
	INT16	sSwitchValue;
		BOOLEAN		fHiddenStructVisible;				// Used for hidden struct visiblity

	// SANDRO - removed AP cost of moving stealthy or reversed outside this procedure

 //if (GridCost[gridno] == NPCMINECOST)
 //	switchValue = BackupGridCost[gridno];
 //else

  sSwitchValue = gubWorldMovementCosts[sGridNo][bDir][ bLevel ];

	// Check reality vs what the player knows....
	if ( pSoldier->bTeam == gbPlayerNum )
	{
		// Is this obstcale a hidden tile that has not been revealed yet?
		if( DoesGridNoContainHiddenStruct( sGridNo, &fHiddenStructVisible ) )
		{
			// Are we not visible, if so use terrain costs!
			if ( !fHiddenStructVisible )
			{
				// Set cost of terrain!
				sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridNo ].ubTerrainID ];			
			}
		}
	}
	//dddokno 
	if ( sSwitchValue == TRAVELCOST_JUMPABLEWINDOW
		|| sSwitchValue == TRAVELCOST_JUMPABLEWINDOW_N
		|| sSwitchValue == TRAVELCOST_JUMPABLEWINDOW_W)
	{
		if (bDir & 1)
				return -1;

	}	
	//ddd ^^^^^^если не ставить здесь условие, то при подходе к клетке с окном ,когда расст до клетки с окном 1
	//получим милое сообщение, что путь заблокирован 
	if ( sSwitchValue == TRAVELCOST_NOT_STANDING)// || sSwitchValue == TRAVELCOST_JUMPABLEWINDOW)
	{
		// use the cost of the terrain!
		sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridNo ].ubTerrainID ];
	}
	else if (IS_TRAVELCOST_DOOR( sSwitchValue ))
	{
		// Can't travel diagonally through a door
		if (bDir & 1)
		{
			return -1;
		}
		sSwitchValue = DoorTravelCost( pSoldier, sGridNo, (UINT8) sSwitchValue, (BOOLEAN) (pSoldier->bTeam == gbPlayerNum), NULL );
	}
	else if (gfPlotPathToExitGrid && sSwitchValue == TRAVELCOST_EXITGRID)
	{
		sSwitchValue = gTileTypeMovementCost[ gpWorldLevelData[ sGridNo ].ubTerrainID ];
	}

	if (sSwitchValue >= TRAVELCOST_BLOCKED && sSwitchValue != TRAVELCOST_DOOR )
	{
		return(-1);	// Cost too much to be considered!
	}

	//CHRISL: We can't jump a fence while wearing a backpack, to consider fences as impassible
	// SANDRO - Headrocks change to backpack check implemented
	if(sSwitchValue == TRAVELCOST_FENCE && UsingNewInventorySystem() == true && FindBackpackOnSoldier( pSoldier ) != ITEM_NOT_FOUND)
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
		// SANDRO - opening door cost will be added after Athletics check
	case TRAVELCOST_DOOR: sAPCost += APBPConstants[AP_MOVEMENT_FLAT];
		if (pSoldier->bTeam == gbPlayerNum)
		{
			return -1;
		}
		//sAPCost += APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_OPEN_DOOR] + APBPConstants[AP_OPEN_DOOR]; // Include open and close costs!
		break;

		// cost for jumping a fence REPLACES all other AP costs!
	case TRAVELCOST_FENCE		: 
		//dddokno{
	case TRAVELCOST_JUMPABLEWINDOW:
	case TRAVELCOST_JUMPABLEWINDOW_N:
	case TRAVELCOST_JUMPABLEWINDOW_W:
		//dddokno}
		if (!IS_MERC_BODY_TYPE( pSoldier ))
		{
			return -1;
		}
		// CHRISL: 
		// SANDRO - some little changes here
		if((UsingNewInventorySystem() == true) && FindBackpackOnSoldier( pSoldier ) != ITEM_NOT_FOUND)
			return( GetAPsToJumpFence( pSoldier, TRUE ) );
		else
			return( GetAPsToJumpFence( pSoldier, FALSE ) );

	case TRAVELCOST_NONE			: return( 0 );

	default:

		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Calc AP: Unrecongnized MP type %d in %d, direction %d", sSwitchValue, sGridno, bDir ) );
			break;


	}

	// SANDRO - note: as it is, the modifiers for walking/running/etc are not multiplied for diagonal movement, but is it intentional?
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


INT16 TerrainBreathPoints(SOLDIERTYPE * pSoldier, INT32 sGridNo, INT8 bDir, UINT16 usMovementMode)
{
 INT32 iPoints=0;
 UINT8 ubMovementCost;

	ubMovementCost = gubWorldMovementCosts[sGridNo][bDir][0];

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

	if ( ubMovementCost == WALKING_PISTOL_RDY || ubMovementCost == WALKING_RIFLE_RDY || ubMovementCost == WALKING_DUAL_RDY )
	{
		iPoints += APBPConstants[BP_MOVEMENT_READY];
	}

	iPoints = iPoints * BreathPointAdjustmentForCarriedWeight( pSoldier ) / 100;

	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( gpWorldLevelData[ sGridNo ].ubTerrainID) )
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
		case SIDE_STEP_PISTOL_RDY:
		case SIDE_STEP_RIFLE_RDY:
		case SIDE_STEP_DUAL_RDY:
		case WALK_BACKWARDS:
		case BLOODCAT_WALK_BACKWARDS:
		case MONSTER_WALK_BACKWARDS:
		case WALKING_PISTOL_RDY:
		case WALKING_RIFLE_RDY:
		case WALKING_DUAL_RDY:
		case WALKING :	iPoints *= APBPConstants[BP_WALK_ENERGYCOSTFACTOR];	break;

		case START_SWAT:
		case SWATTING:
		case SWAT_BACKWARDS:
			iPoints *= APBPConstants[BP_SWAT_ENERGYCOSTFACTOR];	break;
		case CRAWLING:	iPoints *= APBPConstants[BP_CRAWL_ENERGYCOSTFACTOR];	break;


	}

	// SANDRO - STOMP traits - Athletics reduce breath points spent for moving
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
	{
		iPoints = max(1, (INT16)((iPoints * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100) + 0.5));
	}

	// ATE: Adjust these by realtime movement
 if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
 {
		// ATE: ADJUST FOR RT - MAKE BREATH GO A LITTLE FASTER!
		iPoints	= (INT32)( iPoints * TB_BREATH_DEDUCT_MODIFIER );
 }


	return( (INT16) iPoints);
}


INT16 ActionPointCost( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bDir, UINT16 usMovementMode )
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
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - This part have been modified "a bit" 
		// Check movement modifiers
		switch( usMovementMode )
		{
			case RUNNING:
			case ADULTMONSTER_WALKING:
			case BLOODCAT_RUN:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_RUN];
				break;
			case CROW_FLY:
			case SIDE_STEP:
			case WALK_BACKWARDS:
			case ROBOT_WALK:
			case BLOODCAT_WALK_BACKWARDS:
			case MONSTER_WALK_BACKWARDS:
			case LARVAE_WALK:
			case WALKING :
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK];
				if ( usMovementMode == WALKING && !(pSoldier->MercInWater()) && ( (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE ) ))
				{
					sPoints += APBPConstants[AP_MODIFIER_READY];	
				}
				break;
			case SIDE_STEP_PISTOL_RDY:
			case SIDE_STEP_RIFLE_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALKING_PISTOL_RDY: 
			case WALKING_RIFLE_RDY:
			case WALKING_DUAL_RDY:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK] + APBPConstants[AP_MODIFIER_READY];
				break;
			case START_SWAT:
			case SWAT_BACKWARDS:
			case SWATTING:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_SWAT];
				break;
			case CRAWLING:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_CRAWL];
				break;

			default:

				// Invalid movement mode
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Invalid movement mode %d used in ActionPointCost", usMovementMode	) );
				sPoints = sTileCost;
				break;
		}
		// Check for reverse mode
		if ( pSoldier->bReverse || gUIUseReverse )
			sPoints += APBPConstants[AP_REVERSE_MODIFIER];

		// STOMP traits - Athletics trait decreases movement cost
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
		{
			sPoints = max(1, (INT16)((sPoints * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100) + 0.5));
		}
		// Check if doors if not player's merc (they have to open them manually)
		if ( sSwitchValue == TRAVELCOST_DOOR && pSoldier->bTeam != gbPlayerNum )
		{
			sPoints += GetAPsToOpenDoor( pSoldier ) + GetAPsToOpenDoor( pSoldier ); // Include open and close costs!
		}
		// Check for stealth mode
		if ( pSoldier->bStealthMode )
		{
			 // STOMP traits - Stealthy trait decreases stealth AP modifier
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ))
			{
				 sPoints += (max(0, (INT16)((APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100) + 0.5)));
			}
			else
			{
				sPoints += APBPConstants[AP_STEALTH_MODIFIER];
			}
		}
		// Check for backpack
		if((UsingNewInventorySystem() == true) && FindBackpackOnSoldier( pSoldier ) != ITEM_NOT_FOUND )
			sPoints += APBPConstants[AP_MODIFIER_PACK];

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if (sSwitchValue == TRAVELCOST_NOT_STANDING)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case WALKING :
			case WALKING_PISTOL_RDY:
			case WALKING_RIFLE_RDY:
			case WALKING_DUAL_RDY:
			case LARVAE_WALK:
			case SIDE_STEP:
			case SIDE_STEP_PISTOL_RDY:
			case SIDE_STEP_RIFLE_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
				// charge crouch APs for ducking head!
				sPoints += GetAPsCrouch(pSoldier, TRUE); // SANDRO changed
				break;

			default:
				break;
		}
	}

	return( sPoints );
}

INT16 EstimateActionPointCost( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bDir, UINT16 usMovementMode, INT8 bPathIndex, INT8 bPathLength )
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
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - This part have been modified "a bit" 
		// Check movement modifiers
		switch( usMovementMode )
		{
			case RUNNING:
			case ADULTMONSTER_WALKING:
			case BLOODCAT_RUN:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_RUN];
				break;
			case CROW_FLY:
			case SIDE_STEP:
			case WALK_BACKWARDS:
			case ROBOT_WALK:
			case BLOODCAT_WALK_BACKWARDS:
			case MONSTER_WALK_BACKWARDS:
			case LARVAE_WALK:
			case WALKING :
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK];
				if (!(pSoldier->MercInWater()) && ( (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE ) ))
				{
					sPoints += APBPConstants[AP_MODIFIER_READY];	
				}
				break;
			case SIDE_STEP_PISTOL_RDY:
			case SIDE_STEP_RIFLE_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALKING_PISTOL_RDY: 
			case WALKING_RIFLE_RDY:
			case WALKING_DUAL_RDY:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK] + APBPConstants[AP_MODIFIER_READY];
				break;
			case START_SWAT:
			case SWAT_BACKWARDS:
			case SWATTING:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_SWAT];
				break;
			case CRAWLING:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_CRAWL];
				break;

			default:

				// Invalid movement mode
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Invalid movement mode %d used in ActionPointCost", usMovementMode	) );
				sPoints = sTileCost;
				break;
		}
				
		// Check for reverse mode
		if ( pSoldier->bReverse || gUIUseReverse )
			sPoints += APBPConstants[AP_REVERSE_MODIFIER];

		// STOMP traits - Athletics trait decreases movement cost
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
		{
			sPoints = max(1, (INT16)((sPoints * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100) + 0.5));
		}
		// Check if doors if not player's merc (they have to open them manually)
		if ( sSwitchValue == TRAVELCOST_DOOR && pSoldier->bTeam != gbPlayerNum )
		{
			sPoints += GetAPsToOpenDoor( pSoldier ) + GetAPsToOpenDoor( pSoldier ); // Include open and close costs!
		}
		// Check for stealth mode
		if ( pSoldier->bStealthMode )
		{
			 // STOMP traits - Stealthy trait decreases stealth AP modifier
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ))
			{
				 sPoints += (max(0, (INT16)((APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100) + 0.5)));
			}
			else
			{
				sPoints += APBPConstants[AP_STEALTH_MODIFIER];
			}
		}
		// Check for backpack
		if((UsingNewInventorySystem() == true) && FindBackpackOnSoldier( pSoldier ) != ITEM_NOT_FOUND )
			sPoints += APBPConstants[AP_MODIFIER_PACK];

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			case SIDE_STEP_PISTOL_RDY:
			case SIDE_STEP_RIFLE_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
			case RUNNING:
			case WALKING :
			case WALKING_PISTOL_RDY:
			case WALKING_RIFLE_RDY:
			case WALKING_DUAL_RDY:

				// Add here cost to go from crouch to stand AFTER fence hop....
				// Since it's AFTER.. make sure we will be moving after jump...
				if ( ( bPathIndex + 2 ) < bPathLength )
				{
					sPoints += GetAPsCrouch(pSoldier, TRUE); // SANDRO changed..
				}
				break;

			case SWATTING:
			case START_SWAT:
			case SWAT_BACKWARDS:

				// Add cost to stand once there BEFORE....
				sPoints += GetAPsCrouch(pSoldier, TRUE); // SANDRO changed..
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
			case WALKING_PISTOL_RDY:
			case WALKING_RIFLE_RDY:
			case WALKING_DUAL_RDY:
			case SIDE_STEP:
			case SIDE_STEP_PISTOL_RDY:
			case SIDE_STEP_RIFLE_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
				// charge crouch APs for ducking head!
				sPoints += GetAPsCrouch(pSoldier, TRUE); // SANDRO changed..
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


void DeductPoints( SOLDIERTYPE *pSoldier, INT16 sAPCost, INT32 iBPCost, UINT8 ubInterruptType )
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

	// WANNE: negative APs didn't actually have any effect on the next turn because they were being reset to 0 before they could have any effect (by Headrock)
	// If we cannot deduct points, return FALSE
	/*
	if ( sNewAP < 0 )
	{
		sNewAP = 0;
	}
	*/

	pSoldier->bActionPoints = sNewAP;

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
		if ( PTR_OURTEAM && iBPCost >= APBPConstants[BP_MOVEMENT_GRASS] && ubInterruptType != DISABLED_INTERRUPT
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
			//CHRISL: Need to adjust this so that AP lose is based on AP_MAXIMUM
			pSoldier->bActionPoints -= (INT16)( (float)( pSoldier->sBreathRed + iBPCost - BREATH_RED_MAX )
									/ (float)( 5 * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL])
									* (float)(4 * (float)APBPConstants[AP_MAXIMUM] / 100));
			// HEADROCK HAM 3.1: This may be the problem with suppression - it limits the lower APs to 0, which breaks
			// suppression's negative values. Changed instances of "0" to the APBP constant.
			if ( pSoldier->bActionPoints < APBPConstants[AP_MIN_LIMIT] )
			{
				pSoldier->bActionPoints = APBPConstants[AP_MIN_LIMIT];
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Interrupt counter
	if( gGameExternalOptions.fImprovedInterruptSystem && sAPCost > 0 && ubInterruptType != DISABLED_INTERRUPT )
	{
		UINT8 ubPointsRegistered = 0;
		UINT16 uCnt = 0;
		SOLDIERTYPE *pOpponent;
		BOOLEAN fFoundInterrupter = FALSE;

		for ( uCnt = 0; uCnt <= MAX_NUM_SOLDIERS; uCnt++ )
		{
			// first find all guys who watch us
			pOpponent = MercPtrs[ uCnt ];
			if ( pOpponent == NULL)
				continue;			// not here or not even breathing -> next!
			if ( pOpponent->stats.bLife < OKLIFE || pOpponent->bCollapsed || !pOpponent->bActive )
				continue;			// not here or not even breathing -> next!
			if ( pSoldier->bTeam == pOpponent->bTeam )
				continue;			// same team? -> next!
			if ( pSoldier->bSide == pOpponent->bSide )
				continue;			// not enemy
			if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) )
				continue;			// neutral

			// if we see or hear him
			// dunno if this is the best solution yet, probably yes
			if ( pOpponent->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY ||
				 pOpponent->aiData.bOppList[pSoldier->ubID] == HEARD_THIS_TURN)
			//if (SoldierToSoldierLineOfSightTest( pOpponent, pSoldier, TRUE, CALC_FROM_WANTED_DIR ) != 0)
			{
				// calculate how much points do we "register" (let's try to avoid chance-based calc to not inspire save-load mania)
				// Experience says how well is the observer able to notice and percieve the environment around him, i.e. gives us the actual chance per AP
				if ( pOpponent->aiData.bOppList[pSoldier->ubID] == HEARD_THIS_TURN )
				{
					// if we only heard him, keep it lower
					ubPointsRegistered = (gGameExternalOptions.ubBasicPercentRegisterValueIIS - 20) 
										+ (gGameExternalOptions.ubPercentRegisterValuePerLevelIIS * EffectiveExpLevel( pOpponent )); // base 40% + 4% per level
				}
				else
				{
					ubPointsRegistered = gGameExternalOptions.ubBasicPercentRegisterValueIIS 
										+ (gGameExternalOptions.ubPercentRegisterValuePerLevelIIS * EffectiveExpLevel( pOpponent )); // base 60% + 4% per level
				}
				// adjust by range to target
				//INT32 iRange = GetRangeInCellCoordsFromGridNoDiff( pOpponent->sGridNo, pSoldier->sGridNo );		// calculate actual range
				//INT16 iDistVisible = (pOpponent->GetMaxDistanceVisible(pOpponent->sGridNo, pOpponent->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE); // how far do we see
				ubPointsRegistered -= ( 25 * GetRangeInCellCoordsFromGridNoDiff( pOpponent->sGridNo, pSoldier->sGridNo ) / 
										(pOpponent->GetMaxDistanceVisible(pOpponent->sGridNo, pOpponent->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE) ); // -1% registered by 4% of the difference of how far we can see and how far is the target	
				
				if ( gGameOptions.fNewTraitSystem )
				{
					// Martial Arts trait reduces the points registered when moving
					if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && (ubInterruptType == SP_MOVEMENT_INTERRUPT || ubInterruptType == MOVEMENT_INTERRUPT ))
					{
						ubPointsRegistered -= gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT );							
					}
					// Stealhty trait reduced the points registered on all actions
					if ( HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ) )
					{
						ubPointsRegistered -= gSkillTraitValues.ubSTReducedAPsRegistered;							
					}
				}

				// ALRIGHT! Get final value
				ubPointsRegistered = max( 5, min( 100, ubPointsRegistered ) ); // 5% is minimum, 100% maximum
				ubPointsRegistered = (UINT8)((sAPCost * ubPointsRegistered / 100) + 0.5); // now calc how many APs we will award and store it in ubPointsRegistered

				// increase the counter
				if ( ubPointsRegistered )
				{
					pOpponent->aiData.ubInterruptCounter[pSoldier->ubID] += ubPointsRegistered;
					fFoundInterrupter = TRUE;
				}
			}	
		}
		// if interrupted, freeze our guy and trigger interrupt situation
		if (fFoundInterrupter)
		{
			// if we've got the special movement flag type here, pass it to after-action type
			// it's only meant for martial arts bonuses
			if ( ubInterruptType == SP_MOVEMENT_INTERRUPT )
			{
				ubInterruptType = AFTERACTION_INTERRUPT;
			}
			// OK, something happened, set the interrupt pending flag
			gTacticalStatus.ubInterruptPending = ubInterruptType;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	// SANDRO - added heat intolerant penalty
	if ( MercIsHot( pSold ) )
	{
		sBreathFactor += 100;
	}
	// SANDRO - Less breath consumption for Primitive chars
	if ( gGameOptions.fNewTraitSystem && gMercProfiles[pSold->ubProfile].bCharacterTrait == CHAR_TRAIT_PRIMITIVE )
	{
		sBreathFactor -= 15; // -15% breath consumption
	}

 // if a non-swimmer type is thrashing around in deep water
	if ( (pSold->ubProfile != NO_PROFILE ) && (gMercProfiles[ pSold->ubProfile ].bDisability == NONSWIMMER) )
	{
		if ( pSold->usAnimState == DEEP_WATER_TRED || pSold->usAnimState == DEEP_WATER_SWIM)
		{
			sBreathFactor *= 7;		// lose breath 5 times faster in deep water!
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
			// SANDRO - don't reduce the APs if collapsed
			if ( pSold->bCollapsed || pSold->bBreathCollapsed )
				sUnusedAPs = pSold->bActionPoints;
			else
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

			// SANDRO - what the hell!?! This calculation is not correct, it means the less APs you have the more Breath you gain,
			// when the exact opposite should happen according to the function of this entire procedure!!!
			//if ( pSold->bTeam != CIV_TEAM && pSold->bTeam != gbPlayerNum)
			//{
			//	switch( gGameOptions.ubDifficultyLevel )
			//	{
			//		case DIF_LEVEL_EASY:
			//			sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iEasyAPBonus)- sUnusedAPs) * sBreathPerAP;
			//			break;

			//		case DIF_LEVEL_MEDIUM:
			//			sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iExperiencedAPBonus)-sUnusedAPs) * sBreathPerAP;
			//			break;

			//		case DIF_LEVEL_HARD:
			//			sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iExpertAPBonus)- sUnusedAPs) * sBreathPerAP;
			//			break;

			//		case DIF_LEVEL_INSANE:
			//			sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iInsaneAPBonus)- sUnusedAPs) * sBreathPerAP;
			//			break;

			//		default:
			//			sBreathChange = (APBPConstants[AP_MAXIMUM] - sUnusedAPs) * sBreathPerAP;
			//	}
			//}
			//else
			//{
			//	sBreathChange = ((APBPConstants[AP_MAXIMUM] + gGameExternalOptions.iPlayerAPBonus)- sUnusedAPs) * sBreathPerAP;
			//}
			// SANDRO - if we want to make difference between difficulty levels, the just make it so with higher difficulty the enemy regenerate faster:
			if ( pSold->bTeam == ENEMY_TEAM )
			{
				switch( gGameOptions.ubDifficultyLevel )
				{
					case DIF_LEVEL_EASY:
						sBreathPerAP *= 9/10; // -10%
						break;
					case DIF_LEVEL_MEDIUM:
						sBreathPerAP *= 1; // normal
						break;
					case DIF_LEVEL_HARD:
						sBreathPerAP *= 11/10; // +10%
						break;
					case DIF_LEVEL_INSANE:
						sBreathPerAP *= 6/5; // +20%
						break;
				}
			}
			// SANDRO - this simple thing is the correct calculation
			sBreathChange = (sUnusedAPs * sBreathPerAP);
		}
		else
		{
			sBreathChange = 0;
		}
		// Adjust for on drugs
		HandleBPEffectDueToDrugs( pSold, &sBreathChange );

		// SANDRO - Penalty for breath regain for being hit by Martial artist
		if ((pSold->lUnregainableBreath > 0) && (sBreathChange < 0) && gGameOptions.fNewTraitSystem)
		{
			if (pSold->lUnregainableBreath >= (0 - sBreathChange))
			{
				pSold->lUnregainableBreath += sBreathChange;
				sBreathChange = 0;
			}
			else
			{
				sBreathChange += (INT16)(pSold->lUnregainableBreath);
				pSold->lUnregainableBreath = 0;
			}
		}

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
				case WALKING_PISTOL_RDY:
				case WALKING_RIFLE_RDY:
				case WALKING_DUAL_RDY:

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
			// Added a feature to reduce rain effect on regaining breath with Ranger trait - SANDRO
			if( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) && ( gGameOptions.fNewTraitSystem ))
			{
				INT16 sBreathGainPenalty = 0;
				sBreathGainPenalty = (INT16)((gGameExternalOptions.ubBreathGainReductionPerRainIntensity * (100 - gSkillTraitValues.ubRAWeatherPenaltiesReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ))) / 100);
				sBreathGainPenalty = min( max( 0, sBreathGainPenalty ), 100); // keep it 0-100%
				sBreathPerAP -= (INT16)( sBreathPerAP * gbCurrentRainIntensity * sBreathGainPenalty /100 );
			}
			else
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

// HEADROCK HAM 4: Same as above, without percent modifiers.
INT16 CalcAPsToBurstNoModifier( INT16 bBaseActionPoints, OBJECTTYPE * pObj )
{
	INT32 aps;

	aps =	( Weapon[ pObj->usItem ].bBurstAP * bBaseActionPoints + (APBPConstants[AP_MAXIMUM] - 1) ) / APBPConstants[AP_MAXIMUM];

	aps = __max( aps, ( Weapon[ pObj->usItem ].bBurstAP + 1 ) / 2 );

	if ( aps < 0 ) aps = 0;
	else if ( aps > APBPConstants[AP_MAXIMUM] ) aps = APBPConstants[AP_MAXIMUM];

	return (UINT8) aps;
}

INT16 CalcAPsToAutofire( INT16 bBaseActionPoints, OBJECTTYPE * pObj, UINT8 bDoAutofire )
{
	//CHRISL: We send the actual number of rounds being fired in the bDoAutofire paramter.  But that implies that it would take longer to fire the first round
	//	in an autofire sequence then it would take to fire a single round during a single fire sequence.  That doesn't make any sense.  It should only cost
	//	additional APs to fire rounds beyond the first in an autofire sequence.  But rather then mess with all the locations that call this function, I'm going
	//	to simply adjust the parameter once we get to the function.
	if(bDoAutofire > 1)
		bDoAutofire -= 1;
	else
		return 0;
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
		//CHRISL: This can cause problem if a player uses any of the xxx_AP_BONUS values in the INI file.
		//else if ( aps > APBPConstants[AP_MAXIMUM] ) aps = APBPConstants[AP_MAXIMUM];
		// HEADROCK HAM 4: But we NEED an upper cap, so here it is:
		else
			aps = __min(255,aps);
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutoFire: return aps = %d", aps));
	return (UINT8) aps;
}

// HEADROCK HAM 4: Same as above, without modifiers
INT16 CalcAPsToAutofireNoModifier( INT16 bBaseActionPoints, OBJECTTYPE * pObj, UINT8 bDoAutofire )
{
 
	INT32 aps=APBPConstants[AP_MAXIMUM] + 1;
	if ( GetAutofireShotsPerFiveAPs (pObj) )
	{
		const INT32 autofireaps =
		aps = ( ( APBPConstants[AUTOFIRE_SHOTS_AP_VALUE] * bDoAutofire * bBaseActionPoints ) / GetAutofireShotsPerFiveAPs(pObj) + (APBPConstants[AP_MAXIMUM] - 1) ) / APBPConstants[AP_MAXIMUM];

		aps = __max( aps, ( autofireaps + 1 ) / 2 );

		if ( aps < 0 ) aps = 0;
	}

	return (UINT8) aps;
}

INT16 CalcTotalAPsToAttack( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost, INT16 bAimTime )
{
	UINT16						sAPCost = 0;
	UINT16						usItemNum;
	INT32 sActionGridNo;
	UINT8							ubDirection;
	INT32 sAdjustedGridNo;
	UINT32						uiItemClass;
	BOOLEAN	fAddingTurningCost = FALSE;
	BOOLEAN	fAddingRaiseGunCost = FALSE;

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
		//else //ddd comment for aimed burst
		{
			if (gGameExternalOptions.fIncreasedAimingCost )
			{
				// HEADROCK HAM B2.6: Changed the number of APs to attack when aiming.
				if (bAimTime > 0)
				{
					GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sGridNo, ubAddTurningCost, &fAddingTurningCost, &fAddingRaiseGunCost );
					if(fAddingRaiseGunCost == TRUE)
					{
						// HEADROCK HAM 3: No idea what should come here... For now I've put my extra gun-raise costs
						// outside this IF (see below).
					}
					
					// HEADROCK HAM 3: One-time penalty: Add part of the weapon's Ready AP cost. Reinstated because
					// it's now externalized.
					if (gGameExternalOptions.ubFirstAimReadyCostDivisor > 0)
					{
						UINT16 usWeaponReadyTime;
						UINT8 ubReadyTimeDivisor;

						usWeaponReadyTime = Weapon[ usItemNum ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[HANDPOS])) / 100;
						ubReadyTimeDivisor = gGameExternalOptions.ubFirstAimReadyCostDivisor;
						sAPCost += usWeaponReadyTime / ubReadyTimeDivisor;
					}
					
					// If the weapon has a scope, and the target is within eligible range for scope use
					
					if ( (UsingNewCTHSystem() == false && IsScoped(&pSoldier->inv[HANDPOS]) && GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo ) >= GetMinRangeForAimBonus(&pSoldier->inv[HANDPOS]))
						|| (UsingNewCTHSystem() == true && GetBestScopeMagnificationFactor(pSoldier, &pSoldier->inv[HANDPOS], (FLOAT)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo ) > 1.0 )) )
					{
						// Add an individual cost for EACH click, as necessary.

						sAPCost += APBPConstants[AP_FIRST_CLICK_AIM_SCOPE];

						if (bAimTime > 1)
							sAPCost += APBPConstants[AP_SECOND_CLICK_AIM_SCOPE];
						if (bAimTime > 2)
							sAPCost += APBPConstants[AP_THIRD_CLICK_AIM_SCOPE];
						if (bAimTime > 3)
							sAPCost += APBPConstants[AP_FOURTH_CLICK_AIM_SCOPE];
						if (bAimTime > 4)
							sAPCost += APBPConstants[AP_FIFTH_CLICK_AIM_SCOPE];
						if (bAimTime > 5)
							sAPCost += APBPConstants[AP_SIXTH_CLICK_AIM_SCOPE];
						if (bAimTime > 6)
							sAPCost += APBPConstants[AP_SEVENTH_CLICK_AIM_SCOPE];
						if (bAimTime > 7)
							sAPCost += APBPConstants[AP_EIGHTH_CLICK_AIM_SCOPE];
					}
					
					// Weapon has no scope or not within requried range. Apply regular AP costs.
					else
					{
						sAPCost += bAimTime * APBPConstants[AP_CLICK_AIM];
					}
							
				}
			}

			// Regular cost system
			else
			{
				sAPCost += bAimTime * APBPConstants[AP_CLICK_AIM];
			}
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
				INT32		sGotLocation = NOWHERE;
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
				
				if (TileIsOutOfBounds(sGotLocation) && pSoldier->ubBodyType != BLOODCAT )
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
				
				if (!TileIsOutOfBounds(sGotLocation))
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

	return( sAPCost );
}

INT16 MinAPsToAttack(SOLDIERTYPE *pSoldier, INT32 sGridno, UINT8 ubAddTurningCost, UINT8 ubForceRaiseGunCost)
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

	// WANNE : Fixed CTD that occurs when trowing item (grenade, throwing knife, ...)
	// with open description box in tactical
	INT16 baseAPsToShootOrStab = -1;
	if (rof > 0)
	{
		Bottom = (INT32)(( 100 + bAimSkill ) * rof);
		baseAPsToShootOrStab = ( Top + Bottom / 2 ) / Bottom;
	}

	// (this will round to the nearest integer)
	return baseAPsToShootOrStab;
}

// HEADROCK HAM 4: Same function as above, except no modifier.
INT16 BaseAPsToShootOrStabNoModifier( INT16 bAPs, INT16 bAimSkill, OBJECTTYPE * pObj )
{
	INT32	Top, Bottom;
	FLOAT	rof;

	rof = Weapon[ pObj->usItem ].ubShotsPer4Turns;

	Top = 8 * bAPs * 100;

	INT16 baseAPsToShootOrStab = -1;
	if (rof > 0)
	{
		Bottom = (INT32)(( 100 + bAimSkill ) * rof);
		baseAPsToShootOrStab = ( Top + Bottom / 2 ) / Bottom;
	}

	return baseAPsToShootOrStab;
}

void GetAPChargeForShootOrStabWRTGunRaises( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost, BOOLEAN *pfChargeTurning, BOOLEAN *pfChargeRaise )
{
	UINT8 ubDirection;
	UINT32	uiMercFlags;
	UINT16	usTargID;
	BOOLEAN	fAddingTurningCost = FALSE;
	BOOLEAN	fAddingRaiseGunCost = FALSE;
	
	if (!TileIsOutOfBounds(sGridNo))
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
		if(fAddingTurningCost && !fAddingRaiseGunCost)
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
		{
			// SANDRO - Athletics trait check added
			if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				usTurningCost = max( 1, (INT16)((APBPConstants[AP_LOOK_STANDING] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/100) + 0.5));
			else
				usTurningCost = APBPConstants[AP_LOOK_STANDING];
		}
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

INT16 MinAPsToShootOrStab(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost, UINT8 ubForceRaiseGunCost)
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

	// THE LINES FROM HERE HAVE BEEN MOVED DOWN - SANDRO

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

		//CHRISL: When prone and using a bipod, bipod should help compensate for recoil.  To reflect this, our shot AP cost should be minimially reduced
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier > 0 && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && GetBipodBonus(&GrenadeLauncher) > 0){
			bAPCost += (BaseAPsToShootOrStab( bFullAPs, bAimSkill, &GrenadeLauncher ) * (100 - GetBipodBonus(&GrenadeLauncher)) / 100);
		} else {
			bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, &GrenadeLauncher );
		}

		// SANDRO - STOMP traits - reduced APs needed to fire underbarrel grenade launchers for Heavy Weapons trait
		if ( HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) && gGameOptions.fNewTraitSystem )
		{
			bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5); 
		}
	}
	else if ( pSoldier->IsValidSecondHandShot( ) )
	{
		// SANDRO - gunslinger check for firing speed
		if ( HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) && gGameOptions.fNewTraitSystem )
		{
			INT16 bcst1 = BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) );
			INT16 bcst2 = BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[SECONDHANDPOS]) );
			if ( Weapon[ usItem ].ubWeaponType == GUN_PISTOL )
				bcst1 = (INT16)((bcst1 * ( 100 - gSkillTraitValues.ubGSFiringSpeedBonusPistols) / 100)+ 0.5);
			if ( Weapon[ pSoldier->inv[SECONDHANDPOS].usItem ].ubWeaponType == GUN_PISTOL ) 
				bcst2 = (INT16)((bcst2 * ( 100 - gSkillTraitValues.ubGSFiringSpeedBonusPistols) / 100)+ 0.5);

			bAPCost += __max( bcst1, bcst2 );
		}
		else
		{		
			// charge the maximum of the two	
			bAPCost += __max(
				BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) ),
				BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[SECONDHANDPOS]) ) );
		}
	}
	else
	{
		//CHRISL: When prone and using a bipod, bipod should help compensate for recoil.  To reflect this, our shot AP cost should be minimially reduced
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier > 0 && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && GetBipodBonus(&(pSoldier->inv[HANDPOS])) > 0){
			bAPCost += (BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) ) * (100 - GetBipodBonus(&(pSoldier->inv[HANDPOS]))) / 100);
		} else {
			bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]) );
		}
		
		/////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - STOMP traits
		////////////////////////////////////////////////////
		if ( gGameOptions.fNewTraitSystem )
		{
			// Decreased APs needed for LMG - Auto Weapons
			if (Weapon[usItem].ubWeaponType == GUN_LMG && (pSoldier->bDoBurst || pSoldier->bDoAutofire) && ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubAWFiringSpeedBonusLMGs ) / 100)+ 0.5);
			}
			// Decreased APs needed for melee attacks - Melee
			if ( Item[ usItem ].usItemClass == IC_BLADE && ( HAS_SKILL_TRAIT( pSoldier, MELEE_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubMEBladesAPsReduction ) / 100)+ 0.5);
			}
			// Decreased APs needed for throwing knives - Throwing
			else if ( Item[ usItem ].usItemClass == IC_THROWING_KNIFE && ( HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubTHBladesAPsReduction ) / 100)+ 0.5);
			}
			// Decreased APs needed for launchers - Heavy Eeapons
			else if ( (Item[ usItem ].usItemClass == IC_LAUNCHER || Item[usItem].grenadelauncher ) && !(Item[usItem].rocketlauncher) && !(Item[usItem].mortar) && ( HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) ))
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5);
			}
			// Decreased APs needed for launchers - Heavy Eeapons
			else if (( Item[usItem].rocketlauncher || Item[usItem].singleshotrocketlauncher ) && !(Item[usItem].mortar) && ( HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) ))
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWRocketLaunchersAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5);
			}
			// Decreased APs needed for mortar - Heavy Eeapons
			else if ( Item[usItem].mortar && HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWMortarAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5);
			}
			// Decreased APs needed for pistols and machine pistols - Gunslinger
			else if (Weapon[ usItem ].ubWeaponType == GUN_PISTOL && HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubGSFiringSpeedBonusPistols * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ) ) / 100)+ 0.5);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////
	}

	/////////////////////////////////////////////////////////////////////////
	// THESE MODIFIERS HAVE BEEN JUST MOVED HERE FROM ABOVE - SANDRO

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
	// silversurfer: this was different from HandleItem() and therefore it displayed different values
	// on the cursor than those deducted for real
	// bAPCost += (usTurningCost + usRaiseGunCost);
	if ( gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
	{
		if ( fAddingTurningCost && fAddingRaiseGunCost )
		{
			if(usRaiseGunCost > usTurningCost)
				bAPCost += usRaiseGunCost;
			else
				bAPCost += usTurningCost;
		}
		else
			bAPCost += (usTurningCost + usRaiseGunCost);
	}
	else
		bAPCost += (usTurningCost + usRaiseGunCost);	
	
	if (!TileIsOutOfBounds(sGridNo))
	{
		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if ( FindSoldier( sGridNo, &usTargID, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
		{
				sGridNo = MercPtrs[ usTargID ]->sGridNo;
		}
	}

	// if attacking a new target (or if the specific target is uncertain)
	// Added check if the weapon is a throwing knife - otherwise it would add APs for change taregt on cursor but not actually deduct them afterwards - SANDRO
	if (ubForceRaiseGunCost || (( sGridNo != pSoldier->sLastTarget ) && !Item[usItem].rocketlauncher && (Item[ usItem ].usItemClass != IC_THROWING_KNIFE) )) 
	{
		bAPCost += APBPConstants[AP_CHANGE_TARGET];
	}
	/////////////////////////////////////////////////////////////////////////

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


INT16 MinAPsToPunch(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost)
{
	UINT8	bAPCost = 0;
	UINT16 usTargID;
	UINT8	ubDirection;

	//	bAimSkill = ( pSoldier->stats.bDexterity + pSoldier->stats.bAgility) / 2;	
	if (!TileIsOutOfBounds(sGridNo))
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
					// SANDRO - Athletics trait check added
					if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
						bAPCost += max( 1, (INT16)((APBPConstants[AP_LOOK_STANDING] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/100) + 0.5 ));
					else
						bAPCost += APBPConstants[AP_LOOK_STANDING];
				}
			}
		}

	}

	bAPCost += ApsToPunch( pSoldier ); // SANDRO - changed this to direct us to specific calc function

	return ( bAPCost );
}

// SANDRO - added function
INT16 ApsToPunch( SOLDIERTYPE *pSoldier )
{
	if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && (!(pSoldier->inv[HANDPOS].exists()) || Item[pSoldier->inv[HANDPOS].usItem].brassknuckles) )
	{
		return ( max( 1, (INT16)((APBPConstants[AP_PUNCH] * (100 - gSkillTraitValues.ubMAPunchAPsReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/ 100) + 0.5)) );
	}
	else
	{
		return ( APBPConstants[AP_PUNCH] );
	}
}


INT16 MinPtsToMove(SOLDIERTYPE *pSoldier)
{
	// look around all 8 directions and return lowest terrain cost
	UINT8	cnt;
	INT16	sLowest=127;
	INT16	sCost;
	INT32	sGridNo;

	if ( TANK( pSoldier ) )
	{
		return( (INT8)sLowest);
	}

	// WANNE - BMP: FIX: Valid directions are only from 0-7!!
	//for (cnt=0; cnt <= 8; cnt++)
	for (cnt=0; cnt < 8; cnt++)
	{
    sGridNo = NewGridNo(pSoldier->sGridNo,DirectionInc(cnt));
    if (sGridNo != pSoldier->sGridNo)
			{
       if ( (sCost=ActionPointCost( pSoldier, sGridNo, cnt , pSoldier->usUIMovementMode ) ) < sLowest )
			{
					sLowest = sCost;
			}
		}
	}
	return( (INT8)sLowest);
}

INT8	PtsToMoveDirection(SOLDIERTYPE *pSoldier, INT8 bDirection )
{
	INT16	sCost;
	INT32	sGridNo;
	INT8	bOverTerrainType;
	UINT16	usMoveModeToUse;

	sGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( bDirection ) );

	usMoveModeToUse = pSoldier->usUIMovementMode;

	// ATE: Check if the new place is watter and we were tying to run....
	bOverTerrainType = GetTerrainType( sGridNo );

	if ( TERRAIN_IS_WATER( bOverTerrainType) )
	{
		usMoveModeToUse = WALKING;
	}

  sCost = ActionPointCost( pSoldier, sGridNo, bDirection , usMoveModeToUse );

	if ( gubWorldMovementCosts[ sGridNo ][ bDirection ][ pSoldier->pathing.bLevel ] != TRAVELCOST_FENCE )
	{
		if ( usMoveModeToUse == RUNNING && pSoldier->usAnimState != RUNNING )
		{
			sCost += GetAPsStartRun( pSoldier ); // changed by SANDRO
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
		case WALKING_PISTOL_RDY:
		case WALKING_RIFLE_RDY:
		case WALKING_DUAL_RDY:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				bAPs += GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE);
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH)
			{
				bAPs += GetAPsCrouch(pSoldier, TRUE);
			}
			break;
		case SWATTING:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				bAPs += GetAPsProne(pSoldier, TRUE);
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
			{
				bAPs += GetAPsCrouch(pSoldier, TRUE);
			}
			break;
		case CRAWLING:
			if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
			{
				bAPs += GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE);
			}
			else if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH)
			{
				bAPs += GetAPsCrouch(pSoldier, TRUE);
			}
			break;
		default:
			break;
	}

	if (usMovementMode == RUNNING && pSoldier->usAnimState != RUNNING )
	{
		bAPs += GetAPsStartRun( pSoldier ); // changed by SANDRO
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
			if ( pAttachment->exists() )
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

				
				if (Item[pSoldier->inv[bInvPos].usItem].usItemClass == IC_BOMB)
				{
					return (TRUE);
				}

				// ATE: Did an else if here...
				if ( FindAttachmentByClass( &(pSoldier->inv[ bInvPos ]), IC_BOMB ) != 0 )
				{
					return( TRUE );
				}

				// WANNE: If there is a tank, it always have ammo to shoot, no check!
				if (Item[pSoldier->inv[bInvPos].usItem].cannon)
				{
					return ( TRUE );
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
			if ( !pAttachment->exists() )
			{
				pAttachment = FindAttachmentByClass( pObj, IC_BOMB );
			}

			//CHRISL: Added a magsize value to Explosives.xml which is checked by GetExpMagSize.  This will allow OAS to load individual grenades into
			//	multi-launch grenades and fire correctly
			if (pAttachment->exists())
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for GL: found attachment, about to decide whether to remove it"));
				if ( Item[ pObj->usItem ].usItemClass == IC_LAUNCHER && GetExpMagSize(pAttachment) > 1 )
				{
					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for milkor"));
					(*pAttachment)[0]->data.objectStatus -= (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) )) ;

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for milkor: resulting status: %d, remove? = %d",(*pAttachment)[0]->data.objectStatus,((*pAttachment)[0]->data.objectStatus <= (INT8) ( 100 / GetExpMagSize(pAttachment) ))));
					if ( (*pAttachment)[0]->data.objectStatus <= (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) ) ))
					{
						pObj->RemoveAttachment( pAttachment);
					}
				}
				else if ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO ) && GetExpMagSize(pAttachment) > 1 )
				{
					(*pAttachment)[0]->data.objectStatus -= (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) )) ;

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for OICW GL: resulting status: %d, remove? = %d",(*pAttachment)[0]->data.objectStatus,((*pAttachment)[0]->data.objectStatus <= (INT8) ( 100 / GetExpMagSize(pAttachment) ))));
					if ( (*pAttachment)[0]->data.objectStatus <= (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) ) ))
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


UINT16 GetAPsToPickupItem( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	ITEM_POOL					*pItemPool;
	UINT16						sAPCost = 0;
	INT32 sActionGridNo;

	// Check if we are over an item pool
	if ( GetItemPool( usMapPos, &pItemPool, pSoldier->pathing.bLevel ) )
	{
		// If we are in the same tile, just return pickup cost
		sActionGridNo = AdjustGridNoForItemPlacement( pSoldier, usMapPos );

		if ( pSoldier->sGridNo != sActionGridNo )
		{
			sAPCost = PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (INT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

			// If point cost is zero, return 0
			if ( sAPCost != 0 )
			{
				// ADD APS TO PICKUP
				sAPCost += GetBasicAPsToPickupItem( pSoldier ); // changed by SANDRO
			}
		}
		else
		{
				sAPCost += GetBasicAPsToPickupItem( pSoldier ); // changed by SANDRO
		}
	}

	return( sAPCost );
}


UINT16 GetAPsToGiveItem( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	UINT16						sAPCost = 0;

	sAPCost = PlotPath( pSoldier, usMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 || pSoldier->sGridNo == usMapPos )
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
		// SANDRO - added a feature here to reduce APs needed to reload if having GUNSLINGER skill
		if ( gGameOptions.fNewTraitSystem )
		{
			// if pistol or revolver
			if ( HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) && ( Weapon[ pGun->usItem ].ubWeaponType == GUN_PISTOL || Weapon[ pGun->usItem ].ubWeaponType == GUN_M_PISTOL ) )
				return(INT16)(( GetAPsToReload(pGun) * max(0,(100 - gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ) - (HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) ? gSkillTraitValues.ubAMReloadSpeedMagazines : 0 )))/100 ) + 0.5);
			// other weapons for ambidextrous
			else if (HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
				return(INT16)(( GetAPsToReload(pGun) * max(0,(100 - gSkillTraitValues.ubAMReloadSpeedMagazines ))/100) + 0.5);
			// normal case
			else
				return GetAPsToReload(pGun);
		}
		else
			return GetAPsToReload(pGun);
	}
	else if(Weapon[pGun->usItem].swapClips == 1)
	{
		// wrong size ammo item
		if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) ) // SANDRO - added Ambidextrous check
			return (INT8)((((GetAPsToReload(pGun) * APBPConstants[AP_WRONG_MAG])/10)*(100 - gSkillTraitValues.ubAMReloadSpeedLoose)/100) + 0.5);
		else
			return (INT8)((GetAPsToReload(pGun) * APBPConstants[AP_WRONG_MAG])/10);
	}
	else
	{
		// individually loaded guns
		int reload = 0;
		int rounds = __min((GetMagSize(pGun)-(*pGun)[0]->data.gun.ubGunShotsLeft), Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize);
		if(!((*pGun)[0]->data.gun.ubGunState & GS_WEAPON_BEING_RELOADED))
		{
			reload = GetAPsToReload(pGun);
			// SANDRO - added Ambidextrous check
			if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
			{
				reload = (INT16)(reload * (100 - gSkillTraitValues.ubAMReloadSpeedMagazines)/100);
			}
		}
		if(usAllAPs == 1)
		{
			for(int i = 1; i <= rounds; i++)
			{
				// SANDRO - added Ambidextrous check
				if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
				{
					if(EnoughPoints(pSoldier,(reload + (i * (INT16)((APBPConstants[AP_RELOAD_LOOSE]*(100 - gSkillTraitValues.ubAMReloadSpeedLoose)/100) + 0.5))),0,FALSE) == FALSE)
					{
						rounds = i-1;
						break;
					}
				}
				else
				{
					if(EnoughPoints(pSoldier,(reload + (i * APBPConstants[AP_RELOAD_LOOSE])),0,FALSE) == FALSE)
					{
						rounds = i-1;
						break;
					}
				}
			}
		}
		else if (usAllAPs == 2)
			rounds = 1;

		// SANDRO - added Ambidextrous check
		if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
			return (reload + ((INT16) (rounds * (APBPConstants[AP_RELOAD_LOOSE]*(100 - gSkillTraitValues.ubAMReloadSpeedLoose)/100) + 0.5)));
		else
			return (reload + (rounds * APBPConstants[AP_RELOAD_LOOSE]));
	}

	return GetAPsToReload(pGun); // added by SANDRO - safety check
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
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// STOMP traits - SANDRO
		if ( gGameOptions.fNewTraitSystem )
		{
			// Sniper trait makes chambering a round faster
			if (( Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_SN_RIFLE || Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_RIFLE ) && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ))
				return ( (INT16)(((Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )))/100) + 0.5));
			// Ranger trait makes pumping shotguns faster
			else if (( Weapon[Item[(pObj)->usItem].ubClassIndex].ubWeaponType == GUN_SHOTGUN ) && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ))
				return ( (INT16)(((Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT )))/100) + 0.5));
			else
				return (Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually);
		}
		else
		{
			return (Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	INT32 sActionGridNo;
	UINT8			 ubDirection;
	INT32 sAdjustedGridNo;

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
		return(0); // no need to got through the code below here - SANDRO
	}

	// SANDRO - changed this here to call specific functions
	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_PRONE )
	{
		sAPCost = GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE);
	}
	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_CROUCH )
	{
		sAPCost = GetAPsCrouch(pSoldier, TRUE);
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_PRONE )
	{
		sAPCost = GetAPsProne(pSoldier, TRUE);
	}
	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_STAND )
	{
		sAPCost = GetAPsCrouch(pSoldier, TRUE);
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_STAND )
	{
		sAPCost = GetAPsProne(pSoldier, TRUE) + GetAPsCrouch(pSoldier, TRUE);
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_CROUCH )
	{
		sAPCost = GetAPsProne(pSoldier, TRUE);
	}

	return( sAPCost );
}

// SANDRO - actually this procedure is  nowhere used in the entire JA2 code - commented out
//INT16 GetBPsToChangeStance( SOLDIERTYPE *pSoldier, INT8 bDesiredHeight )
//{
//	UINT16						sBPCost = 0;
//	INT8							bCurrentHeight;
//
//	bCurrentHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;
//
//	if ( bCurrentHeight == bDesiredHeight )
//	{
//		sBPCost = 0;
//	}
//
//	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_PRONE )
//	{
//		sBPCost = APBPConstants[BP_CROUCH] + APBPConstants[BP_PRONE];
//	}
//	if ( bCurrentHeight == ANIM_STAND && bDesiredHeight == ANIM_CROUCH )
//	{
//		sBPCost = APBPConstants[BP_CROUCH];
//	}
//	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_PRONE )
//	{
//		sBPCost = APBPConstants[BP_PRONE];
//	}
//	if ( bCurrentHeight == ANIM_CROUCH && bDesiredHeight == ANIM_STAND )
//	{
//		sBPCost = APBPConstants[BP_CROUCH];
//	}
//	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_STAND )
//	{
//		sBPCost = APBPConstants[BP_PRONE] + APBPConstants[BP_CROUCH];
//	}
//	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_CROUCH )
//	{
//		sBPCost = APBPConstants[BP_PRONE];
//	}
//
//	return( sBPCost );
//}


INT16 GetAPsToLook( SOLDIERTYPE *pSoldier )
{
	// Set # of APs
	switch( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
		// Now change to appropriate animation
		case ANIM_STAND:
			if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				return( max( 1, (INT16)((APBPConstants[AP_LOOK_STANDING] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 )));
			else
				return( APBPConstants[AP_LOOK_STANDING] );
			break;

		case ANIM_CROUCH:
			if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				return( max( 1, (INT16)((APBPConstants[AP_LOOK_CROUCHED] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 )));
			else
				return( APBPConstants[AP_LOOK_CROUCHED] );
			break;

		case ANIM_PRONE:
			// APBPConstants[AP_PRONE] is the AP cost to go to or from the prone stance.	To turn while prone, your merc has to get up to
			// crouched, turn, and then go back down.	Hence you go up (APBPConstants[AP_PRONE]), turn (APBPConstants[AP_LOOK_PRONE]) and down (APBPConstants[AP_PRONE]).
			if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				return( max( 1, (INT16)((APBPConstants[AP_LOOK_PRONE] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)) + GetAPsProne(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE) );
			else 
				return( APBPConstants[AP_LOOK_PRONE] + GetAPsProne(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE) );
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
	INT32 sGridNo;

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
		INT16 rt1 = 0, rt2 = 0;
		// SANDRO - STOMP traits - decrease APs to ready pistols and revolvers if having Gunslinger trait
		if ( HAS_SKILL_TRAIT(pSoldier, GUNSLINGER_NT) && ( gGameOptions.fNewTraitSystem ))
		{
			if (Weapon[usItem].ubWeaponType == GUN_PISTOL)
				rt1 = (INT16)(Weapon[usItem].ubReadyTime * max(0, (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(pSoldier, GUNSLINGER_NT) ) ) / 100);
			if (Weapon[pSoldier->inv[SECONDHANDPOS].usItem].ubWeaponType == GUN_PISTOL)
				rt2 = (INT16)(Weapon[pSoldier->inv[SECONDHANDPOS].usItem].ubReadyTime * max(0, (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(pSoldier, GUNSLINGER_NT) ) ) / 100);
		}
		else 
		{
			rt1 = Weapon[usItem].ubReadyTime * ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) ) / 100;
			rt2 = Weapon[pSoldier->inv[SECONDHANDPOS].usItem].ubReadyTime * ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ SECONDHANDPOS ]) ) / 100;
		}
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
			else 
			{
				// SANDRO - STOMP traits
				// Decrease APs to ready LMG if having Auto Weapon trait
				if ( (Weapon[usItem].ubWeaponType == GUN_LMG) && HAS_SKILL_TRAIT(pSoldier, AUTO_WEAPONS_NT) && ( gGameOptions.fNewTraitSystem ))
				{
					return (INT16)(( Weapon[ usItem ].ubReadyTime 
						* max(0, (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) - gSkillTraitValues.ubAWPercentReadyLMGReduction * NUM_SKILL_TRAITS(pSoldier, AUTO_WEAPONS_NT) ) ) / 100)); // -10% per trait
				}
				// Decrease APs to ready pistols and revolvers if having Gunslinger trait
				else if ( (Weapon[usItem].ubWeaponType == GUN_PISTOL) && HAS_SKILL_TRAIT(pSoldier, GUNSLINGER_NT) && ( gGameOptions.fNewTraitSystem ))
				{
					return (INT16)(( Weapon[ usItem ].ubReadyTime 
						* max(0, (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(pSoldier, GUNSLINGER_NT) ) ) / 100)); // -15% per trait
				}
				//Normal ready time
				else
					return (INT16)(( Weapon[ usItem ].ubReadyTime
						* ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) ) ) / 100);
			}
		}
	}

	return( 0 );
}


INT16 GetAPsToClimbRoof( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	// SANDRO - STOMP traits - added a feature to reduce APs needed to climb on or off roof for Martial Arts trait
	UINT16 iAPsToClimb = 0;

	if ( !fClimbDown )
	{
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
			iAPsToClimb = max( 1, (INT16)((APBPConstants[AP_CLIMBROOF] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)); // -25% per trait
		else
			iAPsToClimb = APBPConstants[AP_CLIMBROOF];

		// OK, add aps to goto stand stance...
		iAPsToClimb += GetAPsToChangeStance( pSoldier, ANIM_STAND );
	}
	else
	{
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
			iAPsToClimb = max( 1, (INT16)((APBPConstants[AP_CLIMBOFFROOF] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)); // -25% per trait
		else
			iAPsToClimb = APBPConstants[AP_CLIMBOFFROOF];

		// Add aps to goto crouch
		iAPsToClimb += GetAPsToChangeStance( pSoldier, ANIM_CROUCH );
	}

	// A check if we don't go to zero somehow - SANDRO
	if( iAPsToClimb < 1 )
		iAPsToClimb = 1;

	// return our value
	return(iAPsToClimb);
}

INT16 GetAPsToJumpWall( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	// SANDRO - STOMP traits - added a feature to reduce APs needed to climb on or off roof for Martial Arts trait
	UINT16 iAPsToClimb = 0;

	if ( !fClimbDown )
	{
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
			iAPsToClimb = max( 1, (INT16)((APBPConstants[AP_JUMPWALL] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)); // -25% per trait
		else
			iAPsToClimb = APBPConstants[AP_JUMPWALL];

		// OK, add aps to goto stand stance...
		iAPsToClimb += GetAPsToChangeStance( pSoldier, ANIM_STAND );
	}
	else
	{
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
			iAPsToClimb = max( 1, (INT16)((APBPConstants[AP_JUMPOFFWALL] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)); // -25% per trait
		else
			iAPsToClimb = APBPConstants[AP_JUMPOFFWALL];

		// Add aps to goto crouch
		iAPsToClimb += GetAPsToChangeStance( pSoldier, ANIM_CROUCH );
	}

	// A check if we don't go to zero somehow - SANDRO
	if( iAPsToClimb < 1 )
		iAPsToClimb = 1;

	// return our value
	return(iAPsToClimb);
}

INT16 GetAPsToJumpThroughWindows( SOLDIERTYPE *pSoldier, BOOLEAN fWithBackpack )
{
	// STOMP traits - Martial Arts reduce APs spent for jumping obstacles
	if ( !fWithBackpack )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
			return( max( 1, (INT16)((APBPConstants[AP_JUMPWINDOW] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 ))); // -25% per trait
		else
			return( APBPConstants[AP_JUMPWINDOW] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
			return( max( 1, (INT16)((APBPConstants[AP_JUMPWINDOW] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 ))); // -25% per trait
		else
			return( APBPConstants[AP_JUMPWINDOW] );
	}
}

INT16 GetBPsToClimbRoof( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	// SANDRO - STOMP traits - Athletics reduce breath points spent for moving
	if ( !fClimbDown )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_CLIMBROOF] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_CLIMBROOF] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_CLIMBOFFROOF] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_CLIMBOFFROOF] );
	}
}


// SANDRO - added function to calculate APs for jumping over fence
INT16 GetAPsToJumpFence( SOLDIERTYPE *pSoldier, BOOLEAN fWithBackpack )
{
	// STOMP traits - Martial Arts reduce APs spent for jumping obstacles
	if ( !fWithBackpack )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
			return( max( 1, (INT16)((APBPConstants[AP_JUMPFENCE] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 ))); // -25% per trait
		else
			return( APBPConstants[AP_JUMPFENCE] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
			return( max( 1, (INT16)((APBPConstants[AP_JUMPFENCEBPACK] * ( 100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 ))); // -25% per trait
		else
			return( APBPConstants[AP_JUMPFENCEBPACK] );
	}
}

INT16 GetBPsToJumpWall( SOLDIERTYPE *pSoldier, BOOLEAN fClimbDown )
{
	// SANDRO - STOMP traits - Athletics reduce breath points spent for moving
	if ( !fClimbDown )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPWALL] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPWALL] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPOFFWALL] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPOFFWALL] );
	}
}

// SANDRO - added function to calculate BPs for jumping over fence
INT16 GetBPsToJumpFence( SOLDIERTYPE *pSoldier, BOOLEAN fWithBackpack )
{
	// STOMP traits - Athletics reduce breath points spent for moving
	if ( !fWithBackpack )
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPFENCE] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPFENCE] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPFENCEBPACK] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPFENCEBPACK] );
	}
}

// SANDRO - added function to calculate BPs for jumping over fence
INT16 GetBPsToJumpThroughWindows( SOLDIERTYPE *pSoldier, BOOLEAN fWithBackpack )
{
	// STOMP traits - Athletics reduce breath points spent for moving
	if ( !fWithBackpack )
	{
		// WANNE: Yes, we use the jumpfence BPs, it is simpler for now :)
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPFENCE] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPFENCE] );

	}
	else
	{
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
			return( APBPConstants[BP_JUMPFENCEBPACK] * (100 - gSkillTraitValues.ubATBPsMovementReduction) / 100 );
		else
			return( APBPConstants[BP_JUMPFENCEBPACK] );
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


INT16 MinAPsToThrow( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost )
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

	if ( ( !(Item[ usInHand ].usItemClass & IC_GRENADE) &&
		   !(Item[ usInHand ].usItemClass & IC_LAUNCHER)) )
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
	
	if (!TileIsOutOfBounds(sGridNo))
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

	//iAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND ); // moved lower - SANDRO


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

	// SANDRO - STOMP traits - reduce APs needed to throw grenades if having Demolitions skill
	if( HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ) && gGameOptions.fNewTraitSystem )
	{
		if ( Item[ usInHand ].usItemClass & IC_GRENADE ) 
		{
			iAPCost = max( 1, (INT32)(iAPCost * (100 - gSkillTraitValues.ubDEAPsNeededToThrowGrenadesReduction) / 100));
		}
		// if we've thrown the object for real, it disapears from our hands and is converted to TempObject
		else if ( pSoldier->pTempObject != NULL && pSoldier->pThrowParams != NULL &&
			pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM && (Item[ pSoldier->pTempObject->usItem ].usItemClass & IC_GRENADE) )
		{
			iAPCost = max( 1, (INT32)(iAPCost * (100 - gSkillTraitValues.ubDEAPsNeededToThrowGrenadesReduction) / 100));
		}
	}

	iAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND ); // moved from above - SANDRO

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
	// SANDRO - STOMP traits - Ambidextrous bonus
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_DROP_BOMB] * (100 - gSkillTraitValues.ubAMHandleBombsAPsReduction)/100 ) + 0.5)) );
	else
		return( APBPConstants[AP_DROP_BOMB] );
}

INT16 GetAPsToPlantMine( SOLDIERTYPE *pSoldier )
{
	// SANDRO - STOMP traits - Ambidextrous bonus
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_BURY_MINE] * (100 - gSkillTraitValues.ubAMHandleBombsAPsReduction)/100 ) + 0.5)) );
	else
		return( APBPConstants[AP_BURY_MINE] );
}

UINT16 GetTotalAPsToDropBomb( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	INT16 sAPs = 0;

	sAPs = PlotPath( pSoldier, sGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	if ( sAPs > 0 )
	{
		if(Item[pSoldier->inv[HANDPOS].usItem].mine == 1)
			sAPs += GetAPsToPlantMine( pSoldier ); // changed by SANDRO
		else
			sAPs += GetAPsToDropBomb( pSoldier ); // changed by SANDRO
	}

	return( sAPs );
}



INT16 GetAPsToUseRemote( SOLDIERTYPE *pSoldier )
{
	return( APBPConstants[AP_USE_REMOTE] );
}


INT16 GetAPsToStealItem( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTargetSoldier, INT16 sMapPos ) // SANDRO - added target
{
	INT16	sAPCost = 0;

	if (sMapPos != -1)
	{
		sAPCost = PlotPath( pSoldier, sMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
	}

	// ADD APS TO PICKUP
	/////////////////////////////////////////////////////////////////////////////////////////
	// CHANGED BY SANDRO - REDUCE AP COST TO STEAL FOR MARTIAL ARTS AND HAND TO HAND
	if (pTargetSoldier != NULL && pTargetSoldier->bCollapsed && gGameExternalOptions.fEnhancedCloseCombatSystem)
	{
		sAPCost += (GetBasicAPsToPickupItem( pSoldier )); // stealing from collapsed soldiers is treated differently
	}
	else if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && ( gGameOptions.fNewTraitSystem ))
	{
		sAPCost += max( 1, (INT16)((APBPConstants[AP_STEAL_ITEM] *  (100 - gSkillTraitValues.ubMAReducedAPsToSteal * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/ 100) + 0.5));
	}
	else
	{
		sAPCost += APBPConstants[AP_STEAL_ITEM];
	}
	/////////////////////////////////////////////////////////////////////////////////////////

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


INT16 GetAPsToUseJar( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	INT16						sAPCost = 0;

	sAPCost = PlotPath( pSoldier, usMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

	// If point cost is zero, return 0
	if ( sAPCost != 0 )
	{
		// ADD APS TO PICKUP
		sAPCost += APBPConstants[AP_TAKE_BLOOD];
	}

	return sAPCost;

}

INT16 GetAPsToUseCan( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	INT16	sAPCost = 0;

	sAPCost = PlotPath( pSoldier, usMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

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
	// -25% APs needed to for MA traits
	if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
		return( GetAPsToChangeStance( pSoldier, ANIM_STAND ) + max( 1, (INT16)((APBPConstants[AP_JUMP_OVER] * (100 - gSkillTraitValues.ubMAAPsClimbOrJumpReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/100) + 0.5)));
	else
		return(	GetAPsToChangeStance( pSoldier, ANIM_STAND ) + APBPConstants[AP_JUMP_OVER] );
}

// HEADROCK HAM 3.6: Calculate the actual AP cost to add this many Extra Aiming levels, taking into account
// APBP Constants and extra game features.
INT32 CalcAPCostForAiming( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo, INT8 bAimTime )
{
	INT16 sAPCost = 0;
	UINT16 usItemNum = pSoldier->inv[HANDPOS].usItem;

	Assert(pSoldier != NULL);
	Assert(&pSoldier->inv[HANDPOS] != NULL);

	if (gGameExternalOptions.fIncreasedAimingCost )
	{
		// HEADROCK HAM B2.6: Changed the number of APs to attack when aiming.
		// HEADROCK HAM 3.1: Externalized the entire function to allow customization of each detail.
		if (bAimTime > 0)
		{
			// HEADROCK HAM 3: One-time penalty: Add part of the weapon's Ready AP cost. Reinstated because
			// it's now externalized.
			if (gGameExternalOptions.ubFirstAimReadyCostDivisor > 0)
			{
				UINT16 usWeaponReadyTime;
				UINT8 ubReadyTimeDivisor;

				usWeaponReadyTime = Weapon[ usItemNum ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[HANDPOS])) / 100;
				ubReadyTimeDivisor = gGameExternalOptions.ubFirstAimReadyCostDivisor;
				sAPCost += usWeaponReadyTime / ubReadyTimeDivisor;
			}
			
			// If the weapon has a scope, and the target is within eligible range for scope use
			
			if ( (UsingNewCTHSystem() == false && IsScoped(&pSoldier->inv[HANDPOS]) && GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo ) >= GetMinRangeForAimBonus(&pSoldier->inv[HANDPOS]))
				|| (UsingNewCTHSystem() == true && GetBestScopeMagnificationFactor(pSoldier, &pSoldier->inv[HANDPOS], (FLOAT)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo ) > 1.0 )))
			{
				// Add an individual cost for EACH click, as necessary.

				sAPCost += APBPConstants[AP_FIRST_CLICK_AIM_SCOPE];

				if (bAimTime > 1)
					sAPCost += APBPConstants[AP_SECOND_CLICK_AIM_SCOPE];
				if (bAimTime > 2)
					sAPCost += APBPConstants[AP_THIRD_CLICK_AIM_SCOPE];
				if (bAimTime > 3)
					sAPCost += APBPConstants[AP_FOURTH_CLICK_AIM_SCOPE];
				if (bAimTime > 4)
					sAPCost += APBPConstants[AP_FIFTH_CLICK_AIM_SCOPE];
				if (bAimTime > 5)
					sAPCost += APBPConstants[AP_SIXTH_CLICK_AIM_SCOPE];
				if (bAimTime > 6)
					sAPCost += APBPConstants[AP_SEVENTH_CLICK_AIM_SCOPE];
				if (bAimTime > 7)
					sAPCost += APBPConstants[AP_EIGHTH_CLICK_AIM_SCOPE];
			}
			
			// Weapon has no scope or not within requried range. Apply regular AP costs.
			else
			{
				sAPCost += bAimTime * APBPConstants[AP_CLICK_AIM];
			}
		}
		else
		{
			return(0);
		}
	}

	// Regular cost system
	else
	{
		sAPCost += bAimTime * APBPConstants[AP_CLICK_AIM];
	}

	return sAPCost;
}

// HEADROCK HAM 3.6: Calculate how many Aiming Levels we can get with a given amount of APs.
INT8 CalcAimingLevelsAvailableWithAP( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo, INT8 bAPsLeft )
{
	INT8 bAllowedLevels = 0;

	for (INT16 x = APBPConstants[AP_MIN_AIM_ATTACK]; x <= AllowedAimingLevels(pSoldier, sTargetGridNo); x++)
	{
		if (CalcAPCostForAiming( pSoldier, sTargetGridNo, (INT8)x ) <= (INT32)bAPsLeft)
		{
			bAllowedLevels = (INT8)x;
		}
		else
		{
			break;
		}
	}
	return (bAllowedLevels);
}


// SANDRO - Added feature to reduce APs for change states, if having MA trait
INT16 GetAPsCrouch( SOLDIERTYPE *pSoldier, BOOLEAN fBackpackCheck )
{
	UINT16 iFinalAPsToCrouch;

	iFinalAPsToCrouch = APBPConstants[AP_CROUCH];

	// if backpack and new inventory
	if ( fBackpackCheck && (UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true && !pSoldier->flags.ZipperFlag)
		iFinalAPsToCrouch += 1;

	// -x% APs needed to change stance for MA trait
	if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && ( gGameOptions.fNewTraitSystem ))
		iFinalAPsToCrouch = (INT16)((iFinalAPsToCrouch * ( 100 - gSkillTraitValues.ubMAAPsChangeStanceReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 );

	if (iFinalAPsToCrouch < 1 )
		iFinalAPsToCrouch = 1;

	return(	iFinalAPsToCrouch );
}
// SANDRO - Added feature to reduce APs for change states, if having MA trait
INT16 GetAPsProne( SOLDIERTYPE *pSoldier, BOOLEAN fBackpackCheck )
{
	UINT16 iFinalAPsToLieDown;

	iFinalAPsToLieDown = APBPConstants[AP_PRONE];

	// if backpack and new inventory
	if ( fBackpackCheck && (UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true && !pSoldier->flags.ZipperFlag)
		iFinalAPsToLieDown += 1;

	// -x% APs needed to change stance for MA trait
	if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && ( gGameOptions.fNewTraitSystem ))
		iFinalAPsToLieDown = (INT16)((iFinalAPsToLieDown * ( 100 - gSkillTraitValues.ubMAAPsChangeStanceReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5 );

	if (iFinalAPsToLieDown < 1 )
		iFinalAPsToLieDown = 1;

	return(	iFinalAPsToLieDown );
}
// SANDRO - Added feature to calculate start run cost (for Athletics trait)
INT16 GetAPsStartRun( SOLDIERTYPE *pSoldier )
{
	// Athletics trait
	if ( HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ) && gGameOptions.fNewTraitSystem )
		return( max( 1, (INT16)((APBPConstants[AP_START_RUN_COST] * (100 - gSkillTraitValues.ubATAPsMovementReduction)/100) + 0.5 )) );
	else
		return(	APBPConstants[AP_START_RUN_COST] );
}

// SANDRO - Added feature to calculate APs for handling doors (for Ambidextrous trait)
INT16 GetAPsToOpenDoor( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_OPEN_DOOR] * (100 - gSkillTraitValues.ubAMHandleDoorsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_OPEN_DOOR] );
}

// SANDRO - Added feature to calculate APs for handling doors (for Ambidextrous trait)
INT16 GetAPsToPicklock( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_PICKLOCK] * (100 - gSkillTraitValues.ubAMHandleDoorsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_PICKLOCK] );
}

// SANDRO - Added feature to calculate APs for handling doors (for Ambidextrous trait)
INT16 GetAPsToBombDoor( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_EXPLODE_DOOR] * (100 - gSkillTraitValues.ubAMHandleDoorsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_EXPLODE_DOOR] );
}

// SANDRO - Added feature to calculate APs for handling doors (for Ambidextrous trait)
INT16 GetAPsToUntrapDoor( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_UNTRAP_DOOR] * (100 - gSkillTraitValues.ubAMHandleDoorsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_UNTRAP_DOOR] );
}

// SANDRO - Added feature to calculate APs for pickup items (for Ambidextrous trait)
INT16 GetBasicAPsToPickupItem( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_PICKUP_ITEM] * (100 - gSkillTraitValues.ubAMPickItemsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_PICKUP_ITEM] );
}

// SANDRO - Added feature to calculate APs for disarming mine (for Ambidextrous trait)
INT16 GetAPsToDisarmMine( SOLDIERTYPE *pSoldier )
{
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		return( max( 1, (INT16)((APBPConstants[AP_DISARM_MINE] * (100 - gSkillTraitValues.ubAMHandleBombsAPsReduction)/100) + 0.5 )) );
	else
		return( APBPConstants[AP_DISARM_MINE] );
}