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
	#include "Food.h"		// added by Flugente
	#include "AIInternals.h"//dnl ch69 150913
#endif
#include "connect.h"
//rain
//#define BREATH_GAIN_REDUCTION_PER_RAIN_INTENSITY 25
//end rain

extern BOOLEAN IsValidSecondHandShot( SOLDIERTYPE *pSoldier );
extern UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady, BOOLEAN fHipStance );


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

	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	UINT8 ubTerrainID = gpWorldLevelData[ sGridNo ].ubTerrainID;

	if ( TERRAIN_IS_WATER( ubTerrainID) && pSoldier->pathing.bLevel > 0 )
	{
		ubTerrainID = FLAT_GROUND;

		// WANNE.WATER.BUGFIX: If we "walk on water", take the travecost_flat BPs instead of the high swimming BPs ...
		ubMovementCost = TRAVELCOST_FLAT;	
	}
	
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

	if ( ubMovementCost == WALKING_WEAPON_RDY || ubMovementCost == WALKING_DUAL_RDY )
	{
		iPoints += APBPConstants[BP_MOVEMENT_READY];
	}

	iPoints = iPoints * BreathPointAdjustmentForCarriedWeight( pSoldier ) / 100;

	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( ubTerrainID) )
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
		case SIDE_STEP_WEAPON_RDY:
		case SIDE_STEP_DUAL_RDY:
		case SIDE_STEP_ALTERNATIVE_RDY:
		case WALK_BACKWARDS:
		case BLOODCAT_WALK_BACKWARDS:
		case MONSTER_WALK_BACKWARDS:
		case WALKING_WEAPON_RDY:
		case WALKING_DUAL_RDY:
		case WALKING_ALTERNATIVE_RDY:
		case WALKING :
			iPoints *= APBPConstants[BP_WALK_ENERGYCOSTFACTOR];	break;
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

	// Flugente: scuba fins reduce movement cost in water, but increase cost on land
	if ( pSoldier->inv[LEGPOS].exists() && HasItemFlag( pSoldier->inv[LEGPOS].usItem, SCUBA_FINS ) )
	{
		if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
		{
			iPoints /= 2;
		}
		else
		{
			iPoints *= 2;
		}
	}

	// Flugente: backgrounds
	if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
		iPoints = (iPoints * (100 + pSoldier->GetBackgroundValue(BG_SWIMMING))) / 100;
	
	// ATE: Adjust these by realtime movement
	 if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	 {
		// ATE: ADJUST FOR RT - MAKE BREATH GO A LITTLE FASTER!
	 	// silversurfer: now externalized to APBPConstants.ini
		//iPoints	= (INT32)( iPoints * TB_BREATH_DEDUCT_MODIFIER );
		iPoints	= (INT32)( iPoints * APBPConstants[BP_RT_BREATH_DEDUCT_MODIFIER] / 100 );
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

	
	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	UINT8 ubTerrainID = gpWorldLevelData[ sGridNo ].ubTerrainID;

	if ( TERRAIN_IS_WATER( ubTerrainID) && pSoldier->pathing.bLevel > 0 )
		ubTerrainID = FLAT_GROUND;


	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( ubTerrainID) )
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
			case WALKING_ALTERNATIVE_RDY:
			case SIDE_STEP_ALTERNATIVE_RDY:
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK];
				if ( usMovementMode == WALKING && !(pSoldier->MercInWater()) && ( (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE ) ))
				{
					sPoints += APBPConstants[AP_MODIFIER_READY];	
				}
				break;
			case SIDE_STEP_WEAPON_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALKING_WEAPON_RDY:
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

		// Flugente: scuba fins reduce movement cost in water, but increase cost on land
		if ( pSoldier->inv[LEGPOS].exists() && HasItemFlag( pSoldier->inv[LEGPOS].usItem, SCUBA_FINS ) )
		{
			if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
			{
				sPoints /= 2;
			}
			else
			{
				sPoints *= 2;
			}
		}

		// Flugente: swimming background
		if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
			sPoints = (sPoints * (100 + pSoldier->GetBackgroundValue(BG_SWIMMING))) / 100;

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
		// Moa: apply penalty for heavily packed backpack (wobble penalty)
		if ( UsingNewInventorySystem() == true && gGameExternalOptions.fBackPackWeightLowersAP )
		{
			INT8 bSlot= FindBackpackOnSoldier( pSoldier );
			if ( bSlot != ITEM_NOT_FOUND )
			{
				UINT16 usBPPenalty = APBPConstants[ AP_MODIFIER_PACK ];
				if ( bSlot == BPACKPOCKPOS ) //Backpack caried on back
				{
					OBJECTTYPE * pObj = &( pSoldier->inv[ BPACKPOCKPOS ] );
					UINT16 usBackPackWeight = CalculateObjectWeight( pObj );
					// CalculateObjectWeight checks for active LBE gear. Unfortunatly our backpack is not active since we are carying it.
					// Sounds not intuitive at all, active means the LBE caries items (marked with blue *), but when put on the LBE adittional slots of our soldier
					// are activated where something can be carried. So we have to add the weights of those slots as well.
					std::vector<INT8> vbLBESlots;
					GetLBESlots( BPACKPOCKPOS, vbLBESlots );
					for ( UINT8 i = 0; i < vbLBESlots.size() ; i++ )
					{
						pObj = &( pSoldier->inv[ vbLBESlots[ i ] ] );
						usBackPackWeight += CalculateObjectWeight( pObj );
					}
					usBPPenalty = min( ( usBackPackWeight / 50 ), usBPPenalty ); //1 AP penalty for each 5kg of weight up to the penalty defined by AP_MODIFIER_PACK (default = 4)
				}
				else //Backpack caried not on back (maybe somewhere inside another LBE or in Hand?)
				{
					//apply full penalty
				}
				sPoints += usBPPenalty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if (sSwitchValue == TRAVELCOST_NOT_STANDING)
	{
		switch(usMovementMode)
		{
			case RUNNING:
			case WALKING :
			case WALKING_WEAPON_RDY:
			case WALKING_DUAL_RDY:
			case LARVAE_WALK:
			case SIDE_STEP:
			case SIDE_STEP_WEAPON_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
			case WALKING_ALTERNATIVE_RDY:
			case SIDE_STEP_ALTERNATIVE_RDY:
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


	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	UINT8 ubTerrainID = gpWorldLevelData[ sGridNo ].ubTerrainID;

	if ( TERRAIN_IS_WATER( ubTerrainID) && pSoldier->pathing.bLevel > 0 )
		ubTerrainID = FLAT_GROUND;

	// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
	if ( TERRAIN_IS_WATER( ubTerrainID) )
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
			case WALKING_ALTERNATIVE_RDY :
			case SIDE_STEP_ALTERNATIVE_RDY :
				sPoints = sTileCost + APBPConstants[AP_MODIFIER_WALK];
				if (!(pSoldier->MercInWater()) && ( (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE ) ) && !(gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ALT_WEAPON_HOLDING ) )
				{
					sPoints += APBPConstants[AP_MODIFIER_READY];	
				}
				break;
			case SIDE_STEP_WEAPON_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALKING_WEAPON_RDY:
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
			case SIDE_STEP_WEAPON_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
			case RUNNING:
			case WALKING :
			case WALKING_WEAPON_RDY:
			case WALKING_DUAL_RDY:
			case WALKING_ALTERNATIVE_RDY :
			case SIDE_STEP_ALTERNATIVE_RDY:

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
			case WALKING_WEAPON_RDY:
			case WALKING_DUAL_RDY:
			case SIDE_STEP:
			case SIDE_STEP_WEAPON_RDY:
			case SIDE_STEP_DUAL_RDY:
			case WALK_BACKWARDS:
			case WALKING_ALTERNATIVE_RDY :
			case SIDE_STEP_ALTERNATIVE_RDY:
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
		// Flugente: if we GAIN breath points, adjust them - if we are hungry, we get fewer points back
		// Flugente: ubMaxMorale can now be influenced by our food situation
		if ( iBPCost < 0 && gGameOptions.fFoodSystem )
			ReduceBPRegenForHunger(pSoldier, &iBPCost);

		// Flugente: backgrounds
		iBPCost = (INT32) (iBPCost * (100 + pSoldier->GetBackgroundValue(BG_PERC_REGEN_ENERGY)) / 100);

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
	if( gGameOptions.fImprovedInterruptSystem && sAPCost > 0 && ubInterruptType != DISABLED_INTERRUPT )
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
				INT16 iDistVisible = pOpponent->GetMaxDistanceVisible(pOpponent->sGridNo, pOpponent->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE; // -1% registered by 4% of the difference of how far we can see and how far is the target	
				iDistVisible = max(iDistVisible, CELL_X_SIZE);
				ubPointsRegistered -= min( 25, ( 25 * GetRangeInCellCoordsFromGridNoDiff( pOpponent->sGridNo, pSoldier->sGridNo ) / iDistVisible ));
				
				if ( gGameOptions.fNewTraitSystem )
				{
					// without Night Ops, we get small penalty for interrupting a target in dark
					if ( !(HAS_SKILL_TRAIT( pOpponent, NIGHT_OPS_NT )) && pOpponent->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY)
					{
						INT8 bLightLevel = LightTrueLevel(pSoldier->sGridNo, pSoldier->pathing.bLevel);
						if ( bLightLevel > 6) // 7+ lightlevel is darkness
						{
							ubPointsRegistered -= bLightLevel;	// -7 to -12%
						}
					}

					// Martial Arts trait reduces the points registered when moving
					if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && (ubInterruptType == SP_MOVEMENT_INTERRUPT || ubInterruptType == MOVEMENT_INTERRUPT ) &&
						Item[pSoldier->inv[HANDPOS].usItem].usItemClass & (IC_PUNCH|IC_BLADE|IC_NONE)) // only get this if we have no weapon or blade in hands
					{
						// check how far we are, we only get this bonus at certain range
						INT16 sTileDistance = PythSpacesAway(pSoldier->sGridNo, pOpponent->sGridNo);
						if ( (sTileDistance <= 9) && (pOpponent->bTargetLevel == pSoldier->bTargetLevel)) // we must be on the same ground level as well
						{
							UINT8 ubBonus = gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT );
							if (sTileDistance > 6) // make it gradually effective based on distance 
								ubBonus = ubBonus * (10 - sTileDistance) / 4; // at 9th tile it is only 25% of the trait bonus, from 7th tile it is 100%

							// check direction, we get full bonus if running towards the victim, but others around watching the scene have somehow better chance to interfere
							if ( pSoldier->ubDirection != GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, pOpponent->sGridNo ) )
							{	
								ubBonus = ubBonus *2/3; // two thirds only sound reasonable
							}
							ubPointsRegistered -= ubBonus;	
						}					
					}
					// Stealhty trait reduced the points registered on all actions
					if ( HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ) )
					{
						ubPointsRegistered -= gSkillTraitValues.ubSTReducedAPsRegistered;							
					}
				}

				// ALRIGHT! Get final value
				if (pOpponent->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY )
					ubPointsRegistered = max( 20, min( 100, ubPointsRegistered ) ); // 20% is minimum on seeing
				else
					ubPointsRegistered = max( 10, min( 100, ubPointsRegistered ) ); // 10% is minimum on hearing
				ubPointsRegistered = min( 100, ubPointsRegistered ); // 100% is maximum ofc
				ubPointsRegistered = (UINT8)((sAPCost * ubPointsRegistered / 100) + 0.5); // now calc how many APs we will award

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




INT32 AdjustBreathPts( SOLDIERTYPE * pSoldier , INT32 iBPCost )
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
 sBreathFactor += (100 - pSoldier->bBreath);

 // adjust breath factor for current life deficiency (but add 1/2 bandaging)
 ubBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
 //sBreathFactor += (pSoldier->stats.bLifeMax - (pSoldier->stats.bLife + (ubBandaged / 2)));
 sBreathFactor += 100 * (pSoldier->stats.bLifeMax - (pSoldier->stats.bLife + (ubBandaged / 2))) / pSoldier->stats.bLifeMax;

 if ( pSoldier->stats.bStrength > 80 )
 {
	// give % reduction to breath costs for high strength mercs
	sBreathFactor -= (pSoldier->stats.bStrength - 80) / 2;
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
	if ( MercIsHot( pSoldier ) )
	{
		sBreathFactor += 100;
	}
	// SANDRO - Less breath consumption for Primitive chars
	if ( gGameOptions.fNewTraitSystem && gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_PRIMITIVE )
	{
		sBreathFactor -= 15; // -15% breath consumption
	}

 // if a non-swimmer type is thrashing around in deep water
	if ( (pSoldier->ubProfile != NO_PROFILE ) && (gMercProfiles[ pSoldier->ubProfile ].bDisability == NONSWIMMER) )
	{
		if ( pSoldier->usAnimState == DEEP_WATER_TRED || pSoldier->usAnimState == DEEP_WATER_SWIM)
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



void UnusedAPsToBreath( SOLDIERTYPE * pSoldier )
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


	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if ( !( gTacticalStatus.uiFlags & TURNBASED ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// ALRIGHT, GIVE A FULL AMOUNT BACK, UNLES MODIFIED BY WHAT ACTIONS WE WERE DOING
		sBreathPerAP = GetBreathPerAP( pSoldier, pSoldier->usAnimState );

		// adjust for carried weight
		sBreathPerAP = sBreathPerAP * 100 / BreathPointAdjustmentForCarriedWeight( pSoldier );

		// If this value is -ve, we have a gain, else we have a loos which we should not really do
		// We just want to limit this to no gain if we were doing stuff...
		sBreathChange = 3 * sBreathPerAP;

		// Adjust for on drugs
		HandleBPEffectDueToDrugs( pSoldier, &sBreathChange );

		if ( sBreathChange > 0 )
		{
			sBreathChange = 0;
		}
		else
		{
			// We have a gain, now limit this depending on what we were doing...
			// OK for RT, look at how many tiles we have moved, our last move anim
			if ( pSoldier->ubTilesMovedPerRTBreathUpdate > 0 )
			{
				// How long have we done this for?
				// And what anim were we doing?
				sBreathPerAP = GetBreathPerAP( pSoldier, pSoldier->usLastMovementAnimPerRTBreathUpdate );

				sRTBreathMod = sBreathPerAP * pSoldier->ubTilesMovedPerRTBreathUpdate;

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
		// silversurfer: now externalized to APBPConstants.ini
		//sBreathChange *= TB_BREATH_RECOVER_MODIFIER;
		sBreathChange = INT16 ( sBreathChange * APBPConstants[BP_RT_BREATH_RECOVER_MODIFIER] / 100 );

		// SANDRO: get BP *cost* for weapon holding (reduce breath gain, or even make it breath loss)
		if ( gGameExternalOptions.ubEnergyCostForWeaponWeight ) 
		{
			sBreathChange += 3 * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;

			// if we are actually losing breath by holding a very heavy gun up, lower it if breath already at critical line
			if ( sBreathChange > 0 && pSoldier->bBreath < OKBREATH )
			{	
				// ok, if this gun is rather heavy, and cost us at least 3 energy points per turn
				if ( (GetBPCostPer10APsForGunHolding( pSoldier ) * 10) >= (300 * gGameExternalOptions.ubEnergyCostForWeaponWeight / 100) ) 
				{
					// throw quote
					if ( !(pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LOW_BREATH ) )
					{
						TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_BREATH );
						pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_LOW_BREATH;
					}
					// Put gun down....
					pSoldier->InternalSoldierReadyWeapon(pSoldier->ubDirection, TRUE, FALSE );
				}
			}
		}

		// adjust breath only, don't touch action points!
		DeductPoints(pSoldier,0,(INT16)sBreathChange );

		// Reset value for RT breath update
		pSoldier->ubTilesMovedPerRTBreathUpdate = 0;

	}
	else
	{
		// if merc has any APs left unused this turn (that aren't carrying over)
		if (pSoldier->bActionPoints > APBPConstants[MAX_AP_CARRIED])
		{
			// SANDRO - don't reduce the APs if collapsed
			if ( pSoldier->bCollapsed || pSoldier->bBreathCollapsed )
				sUnusedAPs = pSoldier->bActionPoints;
			else
				sUnusedAPs = pSoldier->bActionPoints - APBPConstants[MAX_AP_CARRIED];

			sBreathPerAP = GetBreathPerAP( pSoldier, pSoldier->usAnimState );

			if (sBreathPerAP < 0)
			{
				// can't gain any breath when we've just been gassed, OR
				// if standing in tear gas without a gas mask on
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
				{
					return;		// can't breathe here, so get no breath back!
				}
			}

			// adjust for carried weight
			sBreathPerAP = sBreathPerAP * 100 / BreathPointAdjustmentForCarriedWeight( pSoldier );

			// SANDRO: what the hell? This calculation is not correct, it means the less APs you have the more Breath you gain,
			// when the exact opposite should happen according to the function of this entire procedure!
			//if ( pSoldier->bTeam != CIV_TEAM && pSoldier->bTeam != gbPlayerNum)
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
			// SANDRO: if we want to make difference between difficulty levels, then just make it so with higher difficulty the enemy regenerate faster:
			if ( pSoldier->bTeam == ENEMY_TEAM )
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
			// SANDRO: this simple thing is the correct calculation
			sBreathChange = (sUnusedAPs * sBreathPerAP);

			// SANDRO: get BP *cost* for weapon holding (reduce breath gain, or even make it breath loss)
			if ( gGameExternalOptions.ubEnergyCostForWeaponWeight ) 
			{
				sBreathChange += sUnusedAPs * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;

				// if we are actually losing breath by holding a very heavy gun up, lower it if breath already at critical line
				if ( sBreathChange > 0 && pSoldier->bBreath < OKBREATH )
				{	
					// ok, if this gun is rather heavy, and cost us at least 3 energy points per turn
					if ( (GetBPCostPer10APsForGunHolding( pSoldier ) * 10) >= (300 * gGameExternalOptions.ubEnergyCostForWeaponWeight / 100) ) 
					{
						// throw quote
						if ( !(pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LOW_BREATH ) )
						{
							TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_BREATH );
							pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_LOW_BREATH;
						}
						// Put gun down....
						pSoldier->InternalSoldierReadyWeapon(pSoldier->ubDirection, TRUE, FALSE );
					}
				}
			}
		}
		else
		{
			sBreathChange = 0;
		}
		// Adjust for on drugs
		HandleBPEffectDueToDrugs( pSoldier, &sBreathChange );

		// SANDRO - Penalty for breath regain for being hit by Martial artist
		if ((pSoldier->lUnregainableBreath > 0) && (sBreathChange < 0) && gGameOptions.fNewTraitSystem)
		{
			if (pSoldier->lUnregainableBreath >= (0 - sBreathChange))
			{
				pSoldier->lUnregainableBreath += sBreathChange;
				sBreathChange = 0;
			}
			else
			{
				sBreathChange += (INT16)(pSoldier->lUnregainableBreath);
				pSoldier->lUnregainableBreath = 0;
			}
		}

		// adjust breath only, don't touch action points!
		DeductPoints(pSoldier,0,(INT16)sBreathChange );

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
				case WALKING_WEAPON_RDY:
				case WALKING_DUAL_RDY:
				case WALKING_ALTERNATIVE_RDY:

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
INT16 CalcAPsToBurst( INT16 bBaseActionPoints, OBJECTTYPE * pObj, SOLDIERTYPE* pSoldier )
{
	INT32 aps;

	// base APs is what you'd get from CalcActionPoints();
	// NB round UP, so 21-25 APs pay full
	aps =	( Weapon[ pObj->usItem ].bBurstAP * bBaseActionPoints + (APBPConstants[AP_MAXIMUM] - 1) ) / APBPConstants[AP_MAXIMUM];

	/*if ( GetPercentBurstFireAPReduction(pObj)>0 )
	{
		aps = (aps * 100) / (100 + GetBurstFireAPReductionStatus(pObj) / (100/GetPercentBurstFireAPReduction(pObj)));
	}

	if ( GetPercentAPReduction(pSoldier, pObj)>0 )
	{
		aps = (aps * 100) / (100 + GetAPReductionStatus(pObj) / (100/GetPercentAPReduction(pSoldier, pObj)));
	}*/

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToBurst: before bonus aps = %d, std bonus = %d, burst bonus = %d", aps,GetPercentAPReduction(pSoldier, pObj),GetPercentBurstFireAPReduction(pObj)));
	// Snap: do this a little differently: % reduction means
	// aps <- aps * ( 100 - red ) / 100
	aps = ( aps	* ( 100 - GetPercentAPReduction(pSoldier, pObj) ) ) / 100;

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

INT16 CalcAPsToAutofire( INT16 bBaseActionPoints, OBJECTTYPE * pObj, UINT8 bDoAutofire, SOLDIERTYPE* pSoldier )
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

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcAPsToAutoFire: before bonus aps = %d, std bonus = %d, auto bonus = %d", aps,GetPercentAPReduction(pSoldier, pObj),GetPercentAutofireAPReduction(pObj)));
		aps = ( aps	* ( 100 - GetPercentAPReduction(pSoldier, pObj) ) ) / 100;

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
	// Flugente: check for underbarrel weapons and use that object if necessary
	OBJECTTYPE* AttackingWeapon = pSoldier->GetUsedWeapon( &(pSoldier->inv[HANDPOS]) );
	UINT16 usUBItemNum = pSoldier->GetUsedWeaponNumber( &(pSoldier->inv[HANDPOS]) );

	usItemNum = pSoldier->inv[HANDPOS].usItem;
	uiItemClass = Item[ usUBItemNum ].usItemClass;

	if ( uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER || uiItemClass == IC_TENTACLES || uiItemClass == IC_THROWING_KNIFE )
	{
		sAPCost = MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost, bAimTime );

		if ( pSoldier->bDoBurst )
		{
			if(pSoldier->bDoAutofire && GetAutofireShotsPerFiveAPs(AttackingWeapon) > 0 )
				sAPCost += CalcAPsToAutofire( pSoldier->CalcActionPoints( ), AttackingWeapon, pSoldier->bDoAutofire, pSoldier );
			else
				sAPCost += CalcAPsToBurst( pSoldier->CalcActionPoints( ), AttackingWeapon, pSoldier );
		}
		//else //ddd comment for aimed burst
		{
			if (gGameExternalOptions.fIncreasedAimingCost )
			{
				// HEADROCK HAM B2.6: Changed the number of APs to attack when aiming.
				if (bAimTime > 0)
				{
					GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sGridNo, ubAddTurningCost, &fAddingTurningCost, &fAddingRaiseGunCost, bAimTime );
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

						usWeaponReadyTime = Weapon[ pSoldier->inv[HANDPOS].usItem ].ubReadyTime * (100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[HANDPOS])) / 100;
						ubReadyTimeDivisor = gGameExternalOptions.ubFirstAimReadyCostDivisor;
						sAPCost += usWeaponReadyTime / ubReadyTimeDivisor;
					}
					
					// If the weapon has a scope, and the target is within eligible range for scope use
					
					if ( (UsingNewCTHSystem() == false && IsScoped(&pSoldier->inv[HANDPOS]) && GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo ) >= GetMinRangeForAimBonus( pSoldier, &pSoldier->inv[HANDPOS]) && !pSoldier->IsValidAlternativeFireMode(bAimTime,sGridNo))
						|| (UsingNewCTHSystem() == true && GetBestScopeMagnificationFactor(pSoldier, &pSoldier->inv[HANDPOS], (FLOAT)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo ) > 1.0 ) && !pSoldier->IsValidAlternativeFireMode(bAimTime,sGridNo)) )
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
		sAPCost = MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost, bAimTime, 0 );

		//sAPCost = 5;
	}

	if ( uiItemClass == IC_PUNCH || uiItemClass == IC_BLADE )//dnl ch73 031013
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
			sAPCost += MinAPsToAttack(pSoldier, sAdjustedGridNo, sAPCost>0?FALSE:ubAddTurningCost, bAimTime, 0);//dnl ch73 031013
			//sAPCost += APsToTurnAround(pSoldier, sAdjustedGridNo);
		}
		else
		{
			// Add points to attack
			// Use our gridno
			sAPCost += MinAPsToAttack( pSoldier, sGridNo, ubAddTurningCost, bAimTime, 0 );
		}

		// Add aim time...
		sAPCost += (bAimTime*APBPConstants[AP_CLICK_AIM]);

	}

	return( sAPCost );
}

INT16 MinAPsToAttack(SOLDIERTYPE *pSoldier, INT32 sGridno, UINT8 ubAddTurningCost, INT16 bAimTime, UINT8 ubForceRaiseGunCost )
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
		UINT16 undbarItem = pSoldier->GetUsedWeaponNumber( &(pSoldier->inv[ HANDPOS ]) );

		// LOOK IN BUDDY'S HAND TO DETERMINE WHAT TO DO HERE
		uiItemClass = Item[ undbarItem ].usItemClass;
	}
#if 0//dnl ch73 290913
	// bare fist or with brass knuckles
	if ( !(pSoldier->inv[HANDPOS].exists()) || Item[pSoldier->inv[HANDPOS].usItem].brassknuckles ) 
	{
		sAPCost = MinAPsToPunch( pSoldier, sGridno, ubAddTurningCost );
	}
	else if ( uiItemClass & ( IC_PUNCH | IC_BLADE | IC_GUN | IC_LAUNCHER | IC_TENTACLES | IC_THROWING_KNIFE ) )
	{
		sAPCost = MinAPsToShootOrStab( pSoldier, sGridno, bAimTime, ubAddTurningCost, ubForceRaiseGunCost );
	}	
	// thrown items
	else if ( uiItemClass & ( IC_GRENADE | IC_THROWN ) )
	{
		sAPCost = MinAPsToThrow( pSoldier, sGridno, ubAddTurningCost );
	}
	// for exceptions
	else 
		sAPCost = MinAPsToPunch( pSoldier, sGridno, ubAddTurningCost );
#else
	if(uiItemClass & (IC_GUN | IC_LAUNCHER | IC_THROWING_KNIFE))
		sAPCost = MinAPsToShootOrStab(pSoldier, sGridno, bAimTime, ubAddTurningCost, ubForceRaiseGunCost);
	else if(uiItemClass & (IC_GRENADE | IC_THROWN))
		sAPCost = MinAPsToThrow(pSoldier, sGridno, ubAddTurningCost);
	else 
		sAPCost = MinAPsToPunch(pSoldier, sGridno, ubAddTurningCost);
#endif
	return sAPCost;
}

INT16 CalcAimSkill( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	INT16 bAimSkill;

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
		bAimSkill = ( EffectiveDexterity( pSoldier, FALSE ) + EffectiveAgility( pSoldier, FALSE ) ) / 2;
		//return( 4 );
	}
	return( bAimSkill );
}

INT16 BaseAPsToShootOrStab( INT16 bAPs, INT16 bAimSkill, OBJECTTYPE * pObj, SOLDIERTYPE* pSoldier )
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
	// Formula: AP to Shoot% = (maxAP_soldier) / ((MAXAIM*0.5 + aimSkill_soldier*0.5) * ROF_PER4TURNS / 4)
	//          AP to Shoot = (AP to Shoot%) * MAXAP

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
		Top *= ( 100 - GetPercentAPReduction(pSoldier, pObj) );
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

void GetAPChargeForShootOrStabWRTGunRaises( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost, BOOLEAN *pfChargeTurning, BOOLEAN *pfChargeRaise, INT16 bAimTime )
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
			if ( ubDirection != pSoldier->ubDirection && !(ubDirection == pSoldier->pathing.bDesiredDirection && pSoldier->aiData.bLastAction == AI_ACTION_CHANGE_FACING) )//dnl ch64 310813 sometimes turning is in progress and APs already deducted
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
		// SANDRO - alternative weapon holding feature
		if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
		{
			if ( pSoldier->bScopeMode == USE_ALT_WEAPON_HOLD && !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_ALT_WEAPON_HOLDING ) ))
			{
				fAddingRaiseGunCost = TRUE;
			}
			else if ( pSoldier->bScopeMode != USE_ALT_WEAPON_HOLD && ( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_ALT_WEAPON_HOLDING ) || !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) ))
			{
				fAddingRaiseGunCost = TRUE;
			}
		}
		else if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 2 )
		{
			if ( !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) || 
				((gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_ALT_WEAPON_HOLDING )) && 
				(bAimTime > GetNumberAltFireAimLevels( pSoldier, sGridNo )) ))
			{
				fAddingRaiseGunCost = TRUE;
			}
		}
		else if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 1 )
		{
			if ( !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) || 
				((gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_ALT_WEAPON_HOLDING )) && 
				(bAimTime > 0) ))
			{
				fAddingRaiseGunCost = TRUE;
			}
		}
		else 
		{
			if ( !( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) )
			{
				fAddingRaiseGunCost = TRUE;
			}	
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

UINT16 CalculateTurningCost(SOLDIERTYPE *pSoldier, UINT16 usItem, BOOLEAN fAddingTurningCost, INT8 bDesiredHeight)//dnl ch72 190913
{
	UINT16 usTrueAnimState, usTurningCost = 0;

	if(fAddingTurningCost)
	{
		usTrueAnimState = pSoldier->usAnimState;
		switch(bDesiredHeight)
		{
		case ANIM_STAND:
			pSoldier->usAnimState = STANDING;
			break;
		case ANIM_CROUCH:
			pSoldier->usAnimState = CROUCHING;
			break;
		case ANIM_PRONE:
			pSoldier->usAnimState = PRONE;
			break;
		default:
			break;
		}
		if(Item[usItem].usItemClass == IC_THROWING_KNIFE)
			pSoldier->usAnimState = STANDING;
		usTurningCost = GetAPsToLook(pSoldier);
		pSoldier->usAnimState = usTrueAnimState;
	}
	return(usTurningCost);
}

UINT16 CalculateRaiseGunCost(SOLDIERTYPE *pSoldier, BOOLEAN fAddingRaiseGunCost, INT32 iTargetGridNum, INT16 bAimTime )
{
	UINT16	usRaiseGunCost = 0, usAnimState;

	if (fAddingRaiseGunCost )
	{
		BOOLEAN fAltFireMode = FALSE;
		if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding )
		{
			if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )//dnl ch69 130913 condition (&& pSoldier->bTeam == gbPlayerNum) must be out because prevent AI from doing alternate weapon holding
			{
				if ( pSoldier->bScopeMode == -1 )
					fAltFireMode = TRUE;
			}
			else if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 1 )
			{
				if ( bAimTime == 0 )
					fAltFireMode = TRUE;
			}
			else
			{
				if ( bAimTime <= GetNumberAltFireAimLevels( pSoldier, iTargetGridNum ) )
					fAltFireMode = TRUE;
			}
		}
		usAnimState = PickSoldierReadyAnimation( pSoldier, FALSE, fAltFireMode );
		usRaiseGunCost = GetAPsToReadyWeapon( pSoldier, usAnimState );
		pSoldier->flags.fDontChargeReadyAPs = FALSE;
	}

	return usRaiseGunCost;
}

INT16 MinAPsToShootOrStab(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 bAimTime, UINT8 ubAddTurningCost, UINT8 ubForceRaiseGunCost )
{
	UINT32	uiMercFlags;
	UINT16	usTargID;
	INT16	bFullAPs;
	INT16 bAimSkill;
	INT16	bAPCost = APBPConstants[AP_MIN_AIM_ATTACK];
	BOOLEAN	fAddingTurningCost = FALSE;
	BOOLEAN	fAddingRaiseGunCost = FALSE;
	UINT16 usItem;
	UINT16 usRaiseGunCost = 0;
	UINT16 usTurningCost = 0;
	//UINT16 usRange;

	UINT16 usUBItem = 0;
	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		usItem = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS] );//UNDER_GLAUNCHER;
		usUBItem = usItem;
	}
	else
	{		
		usItem = pSoldier->inv[ HANDPOS ].usItem;

		// Flugente: we need a secon temnr in case we are using an underbarrel weapon. Not all checks should apply for that one, as aiming is still done with the main weapon
		usUBItem = pSoldier->GetUsedWeaponNumber(&pSoldier->inv[HANDPOS]);
	}
	if(Item[usUBItem].usItemClass == IC_PUNCH || Item[usUBItem].usItemClass == IC_BLADE || Item[usUBItem].usItemClass == IC_TENTACLES)//dnl ch73 021013 punch and stub generally use identical logic so put all necessary stuff to MinAPsToPunch
		return(MinAPsToPunch(pSoldier, sGridNo, ubAddTurningCost));

	OBJECTTYPE* pObjUsed = pSoldier->GetUsedWeapon( &(pSoldier->inv[HANDPOS]) );

	GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sGridNo, ubAddTurningCost, &fAddingTurningCost, &fAddingRaiseGunCost, bAimTime );

	// Snap: reversed DIGICRAB's change.
	// bFullAPs are BASE APs, which do not include APs caried over from the previous turn.
	bFullAPs = pSoldier->CalcActionPoints( );

	// aim skill is the same whether we are using 1 or 2 guns
	bAimSkill = CalcAimSkill( pSoldier, usItem );

	if (!TileIsOutOfBounds(sGridNo))
	{
		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if ( FindSoldier( sGridNo, &usTargID, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
		{
				sGridNo = MercPtrs[ usTargID ]->sGridNo;
		}
		//usRange = GetRangeFromGridNoDiff( pSoldier->sGridNo, sGridNo );
	}

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
			bAPCost += (BaseAPsToShootOrStab( bFullAPs, bAimSkill, &GrenadeLauncher, pSoldier ) * (100 - GetBipodBonus(&GrenadeLauncher)) / 100);
		} else {
			bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, &GrenadeLauncher, pSoldier );
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
			INT16 bcst1 = BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]), pSoldier );
			INT16 bcst2 = BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[SECONDHANDPOS]), pSoldier );
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
				BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[HANDPOS]), pSoldier ),
				BaseAPsToShootOrStab( bFullAPs, bAimSkill, &(pSoldier->inv[SECONDHANDPOS]), pSoldier ) );
		}
	}
	else
	{
		//CHRISL: When prone and using a bipod, bipod should help compensate for recoil.  To reflect this, our shot AP cost should be minimially reduced
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier > 0 && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && GetBipodBonus(&(pSoldier->inv[HANDPOS])) > 0)
		{
			bAPCost += (BaseAPsToShootOrStab( bFullAPs, bAimSkill, pObjUsed, pSoldier ) * (100 - GetBipodBonus(&(pSoldier->inv[HANDPOS]))) / 100);
		} 
		else 
		{
			bAPCost += BaseAPsToShootOrStab( bFullAPs, bAimSkill, pObjUsed, pSoldier );
		}
		// If we are using alternative weapon holding (from hip rifle/one-handed pistol), the shot can be faster if set in the ini
		if ( gGameExternalOptions.ubAltWeaponHoldingFireSpeedBonus > 0 && pSoldier->IsValidAlternativeFireMode( bAimTime, sGridNo ))
		{
			bAPCost = (INT16)((bAPCost * (100 - gGameExternalOptions.ubAltWeaponHoldingFireSpeedBonus ) / 100)+ 0.5);
		}
		/////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - STOMP traits
		////////////////////////////////////////////////////
		if ( gGameOptions.fNewTraitSystem )
		{
			// Decreased APs needed for LMG - Auto Weapons
			if (Weapon[usUBItem].ubWeaponType == GUN_LMG && (pSoldier->bDoBurst || pSoldier->bDoAutofire) && ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubAWFiringSpeedBonusLMGs ) / 100)+ 0.5);
			}
#if 0//dnl ch73 021013 relocate this to MinAPsToPunch
			// Decreased APs needed for melee attacks - Melee (Blade only)
			else if ( Item[ usUBItem ].usItemClass == IC_BLADE && ( HAS_SKILL_TRAIT( pSoldier, MELEE_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubMEBladesAPsReduction ) / 100)+ 0.5);
			}
#endif
			// Decreased APs needed for throwing knives - Throwing
			else if ( Item[ usUBItem ].usItemClass == IC_THROWING_KNIFE && ( HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubTHBladesAPsReduction ) / 100)+ 0.5);
			}
			// Decreased APs needed for launchers - Heavy Weapons
			else if ( (Item[ usUBItem ].usItemClass == IC_LAUNCHER || Item[usUBItem].grenadelauncher ) && !(Item[usUBItem].rocketlauncher) && !(Item[usUBItem].mortar) && ( HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) ))
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5);
			}
			// Decreased APs needed for launchers - Heavy Weapons
			else if (( Item[usUBItem].rocketlauncher || Item[usUBItem].singleshotrocketlauncher ) && !(Item[usUBItem].mortar) && ( HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ) ))
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWRocketLaunchersAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) ) / 100)+ 0.5);
			}
			// Decreased APs needed for mortar - Heavy Weapons
			else if ( Item[usUBItem].mortar )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubHWMortarAPsReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ) + pSoldier->GetBackgroundValue(BG_ARTILLERY) ) / 100)+ 0.5);
			}
			// Decreased APs needed for pistols and machine pistols - Gunslinger
			else if (Weapon[ usUBItem ].ubWeaponType == GUN_PISTOL && HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubGSFiringSpeedBonusPistols * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ) ) / 100)+ 0.5);
			}
			// Decrease APs needed for Shotguns - Ranger
			else if (Weapon[ usUBItem ].ubWeaponType == GUN_SHOTGUN && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
			{
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubRAFiringSpeedBonusShotguns * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ) ) / 100)+ 0.5);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////
	}

	if (ubForceRaiseGunCost == TRUE )//dnl ch69 140913
	{
		fAddingRaiseGunCost = TRUE;
	}
	
	if (!TileIsOutOfBounds(sGridNo))
	{
		// throwing knifes
		if ( Item[ usUBItem ].usItemClass == IC_THROWING_KNIFE )
		{
			// Do we need to stand up?
			bAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
		}
#if 0//dnl ch73 021013 relocate this to MinAPsToPunch
		// blunt weapons & blades
		else if ( Item[ usUBItem ].usItemClass == IC_PUNCH || Item[ usUBItem ].usItemClass == IC_BLADE )
		{
			if ( usTargID != NOBODY  )
			{
				// Check if target is prone, if so, calc cost...
				if ( gAnimControl[ MercPtrs[ usTargID ]->usAnimState ].ubEndHeight == ANIM_PRONE )
					bAPCost += GetAPsToChangeStance( pSoldier, ANIM_CROUCH );
				else
					bAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );
			}
		}
#endif
		else if(Item[usItem].rocketlauncher || Item[usItem].grenadelauncher || Item[usItem].mortar)//dnl ch72 260913 move this here from bottom, need to change as rocketlaucher could be fired from crouch too
		{
			if(gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE || Item[usItem].mortar && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)
				bAPCost += GetAPsToChangeStance(pSoldier, ANIM_CROUCH);
			else
				bAPCost += GetAPsToChangeStance(pSoldier, gAnimControl[pSoldier->usAnimState].ubEndHeight);
		}
	}

	if ( AM_A_ROBOT( pSoldier ) || TANK( pSoldier ) || ubForceRaiseGunCost == 2 )//dnl ch64 300813 robots and tanks cannot do this //dnl ch69 150913 need option to override raise gun cost
	{
		fAddingRaiseGunCost = FALSE;
	}

	//Calculate usTurningCost
	if (!TileIsOutOfBounds(sGridNo))
	{
#if 0//dnl ch73 021013 relocate this to MinAPsToPunch
		// Buggler: actual melee ap deduction for turning applies only when target is 1 tile away
		if ( !( ( Item[ usUBItem ].usItemClass == IC_PUNCH || Item[ usUBItem ].usItemClass == IC_BLADE ) && usRange > 1 ) )
#endif
		{
			if(Item[usItem].rocketlauncher || Item[usItem].grenadelauncher || Item[usItem].mortar)//dnl ch72 260913
			{
				if(gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE || Item[usItem].mortar && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)
					usTurningCost = CalculateTurningCost(pSoldier, usItem, fAddingTurningCost, ANIM_CROUCH);
				else
					usTurningCost = CalculateTurningCost(pSoldier, usItem, fAddingTurningCost);
			}
			else
				usTurningCost = CalculateTurningCost(pSoldier, usItem, fAddingTurningCost);
		}
	}

	//Calculate usRaiseGunCost
	usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost, sGridNo, bAimTime );

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

	// if attacking a new target (or if the specific target is uncertain)
	// Added check if the weapon is throwing knife/melee weapons - otherwise it would add APs for change target on cursor but not actually deduct them afterwards - SANDRO
	if ( ubForceRaiseGunCost == TRUE || (( sGridNo != pSoldier->sLastTarget ) && !Item[usUBItem].rocketlauncher && ( Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )/* && ( Item[ usUBItem ].usItemClass != IC_PUNCH ) && ( Item[ usUBItem ].usItemClass != IC_BLADE )*/ ) )//dnl ch69 140913 //dnl ch73 290913
	{
		if ( pSoldier->IsValidAlternativeFireMode( bAimTime, sGridNo ) )
			bAPCost += (APBPConstants[AP_CHANGE_TARGET] / 2);
		else
			bAPCost += APBPConstants[AP_CHANGE_TARGET];
	}
#if 0//dnl ch63 240813 this seems very wrong, in most case (pSoldier->bActionPoints > bFullAps) and this will return less points then is actually required and could cancel some AI actions, like throwing grenades
	// the minimum AP cost of ANY shot can NEVER be more than merc's maximum APs!
	if ( bAPCost > bFullAPs )
		bAPCost = bFullAPs;
#endif
	// this SHOULD be impossible, but nevertheless...
	if ( bAPCost < 1 )
		bAPCost = 1;

	return bAPCost;
}

INT16 MinAPsToPunch(SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubAddTurningCost)//dnl ch73 290913
{
	INT16 bAPCost = APBPConstants[AP_MIN_AIM_ATTACK];
	if(!TileIsOutOfBounds(sGridNo))
	{
		OBJECTTYPE *pObjUsed = pSoldier->GetUsedWeapon(&pSoldier->inv[HANDPOS]);
		UINT16 usItem = pObjUsed->usItem;
		INT16 bFullAPs = pSoldier->CalcActionPoints();
		INT16 bAimSkill = CalcAimSkill(pSoldier, pSoldier->inv[HANDPOS].usItem);
		if(usItem == NONE/* || Item[usItem].brassknuckles*/)
			bAPCost += ApsToPunch(pSoldier);// SANDRO - changed this to direct us to specific calc function
		else
		{
			bAPCost += BaseAPsToShootOrStab(bFullAPs, bAimSkill, pObjUsed, pSoldier);
			// Decreased APs needed for melee attacks - Melee (Blade only)
			if(Item[usItem].usItemClass == IC_BLADE && (HAS_SKILL_TRAIT(pSoldier, MELEE_NT)))
				bAPCost = (INT16)((bAPCost * (100 - gSkillTraitValues.ubMEBladesAPsReduction) / 100) + 0.5);
		}
		UINT16 usTargID = WhoIsThere2(sGridNo, pSoldier->bTargetLevel);
		// Given a gridno here, check if we are on a guy - if so - get his gridno
		if(usTargID != NOBODY)
		{
			// Check if target is prone, if so, calc cost...
			if(gAnimControl[MercPtrs[usTargID]->usAnimState].ubEndHeight == ANIM_PRONE)
				bAPCost += GetAPsToChangeStance(pSoldier, ANIM_CROUCH);
			else
				bAPCost += GetAPsToChangeStance(pSoldier, ANIM_STAND);
		}
		if(ubAddTurningCost)
		{
			// ATE: Use standing turn cost....
			UINT8 ubDirection = GetDirectionFromGridNo(sGridNo, pSoldier);
			INT32 sSpot = sGridNo;
			if(usTargID != NOBODY && gAnimControl[MercPtrs[usTargID]->usAnimState].ubEndHeight == ANIM_PRONE)
				for(INT8 sCnt=0; sCnt<NUM_WORLD_DIRECTIONS; sCnt++)
				{
					sSpot = NewGridNo(pSoldier->sGridNo, DirectionInc(sCnt));
					if(gubWorldMovementCosts[sSpot][sCnt][pSoldier->bTargetLevel] >= TRAVELCOST_BLOCKED)
						continue;
					if(WhoIsThere2(sSpot, pSoldier->bTargetLevel) == usTargID)
					{
						ubDirection = (UINT8)sCnt;
						break;
					}
				}
			// Is it the same as he's facing?
			// Buggler: actual melee ap deduction for turning applies only when target is 1 tile away
			//UINT16 usRange = GetRangeFromGridNoDiff(pSoldier->sGridNo, sSpot);
			if(/*usRange == 1 && */ubDirection != pSoldier->ubDirection)
			{
				if(gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE)
					bAPCost += CalculateTurningCost(pSoldier, usItem, TRUE, ANIM_CROUCH);
				else
					bAPCost += CalculateTurningCost(pSoldier, usItem, TRUE);
			}
		}
	}
	return(bAPCost);
}

// SANDRO - added function
INT16 ApsToPunch( SOLDIERTYPE *pSoldier )
{
	if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
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
	INT16	sLowest=32767;//dnl ch64 290813 100AP made return of INT8 or 127 obsolete as other variables are INT16
	INT16	sCost;
	INT32	sGridNo;

	if ( TANK( pSoldier ) )
	{
		return(sLowest);//dnl ch64 290813 100AP made INT8 return obsolete
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
	return(sLowest);//dnl ch64 290813 100AP made INT8 return obsolete
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

	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	if ( TERRAIN_IS_WATER( bOverTerrainType) && pSoldier->pathing.bLevel > 0 )
		bOverTerrainType = FLAT_GROUND;

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
		case WALKING_WEAPON_RDY:
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
			OBJECTTYPE* pObjUsed = pSoldier->GetUsedWeapon( &(pSoldier->inv[bInvPos]) );
			UINT16 usItemUsed    = pSoldier->GetUsedWeaponNumber( &(pSoldier->inv[bInvPos]) );

			if ( Item[usItemUsed].singleshotrocketlauncher	)
			{
				// hack... they turn empty afterwards anyways
				return( TRUE );
			}

			if (Item[ usItemUsed ].usItemClass == IC_LAUNCHER || Item[usItemUsed].cannon )
			{
				if ( FindAttachmentByClass( pObjUsed, IC_GRENADE ) != 0 )
				{
					return( TRUE );
				}

				
				if (Item[usItemUsed].usItemClass == IC_BOMB)
				{
					return (TRUE);
				}

				// ATE: Did an else if here...
				if ( FindAttachmentByClass( pObjUsed, IC_BOMB ) != 0 )
				{
					return( TRUE );
				}

				// WANNE: If there is a tank, it always have ammo to shoot, no check!
				if (Item[usItemUsed].cannon)
				{
					return ( TRUE );
				}

				if ( fDisplay )
				{
					TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
				}

				return( FALSE );
			}
			else if (Item[ usItemUsed ].usItemClass == IC_GUN)
			{
				if ( (*pObjUsed)[0]->data.gun.ubGunShotsLeft == 0 )
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
					if ( !(	(*pObjUsed)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER ) )
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
			// Flugente: check for underbarrel weapons and use that object if necessary
			OBJECTTYPE* pObjUsed = pSoldier->GetUsedWeapon( &(pSoldier->inv[bInvPos]) );

			// Flugente: external feeding allows us to take ammo from somewhere other than our magazine, like a belt in our inventory our even another mercs
			if ( gGameExternalOptions.ubExternalFeeding > 0 )
			{
				if ( !DeductBulletViaExternalFeeding(pSoldier, pObj) )
				{
					// bullet couldn't be fed externally -> remove bullet from the weapon's magazine
					// OK, let's see, don't overrun...
					if ( (*pObjUsed)[0]->data.gun.ubGunShotsLeft != 0 )
					{
						(*pObjUsed)[0]->data.gun.ubGunShotsLeft--;
					}	
				}
			}
			else
			{
				// OK, let's see, don't overrun...
				if ( (*pObjUsed)[0]->data.gun.ubGunShotsLeft != 0 )
				{
					(*pObjUsed)[0]->data.gun.ubGunShotsLeft--;
				}	
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
					if ( (*pAttachment)[0]->data.objectStatus < (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) ) ))
					{
						pObj->RemoveAttachment( pAttachment);
					}
				}
				else if ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO ) && GetExpMagSize(pAttachment) > 1 )
				{
					(*pAttachment)[0]->data.objectStatus -= (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) )) ;

					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DeductAmmo: deducting for OICW GL: resulting status: %d, remove? = %d",(*pAttachment)[0]->data.objectStatus,((*pAttachment)[0]->data.objectStatus <= (INT8) ( 100 / GetExpMagSize(pAttachment) ))));
					if ( (*pAttachment)[0]->data.objectStatus < (INT8) ceil((double)( 100 / GetExpMagSize(pAttachment) ) ))
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

	// Flugente: check for underbarrel weapons and use that object if necessary
	pObj = pSoldier->GetUsedWeapon( &(pSoldier->inv[HANDPOS]) );

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
			// Flugente: check for underbarrel weapons and use that object if necessary
			pObj = pSoldier->GetUsedWeapon( &(pSoldier->inv[SECONDHANDPOS]) );
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
		sAPCost = GetAPsCrouch(pSoldier, TRUE*2);//dnl ch70 160913
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_STAND )
	{
		sAPCost = GetAPsProne(pSoldier, TRUE*2) + GetAPsCrouch(pSoldier, TRUE*2);//dnl ch70 160913
	}
	if ( bCurrentHeight == ANIM_PRONE && bDesiredHeight == ANIM_CROUCH )
	{
		sAPCost = GetAPsProne(pSoldier, TRUE*2);//dnl ch70 160913
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
			//dnl ch70 160913 because of backpack cost for going up is 2
			if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				return( max( 1, (INT16)((APBPConstants[AP_LOOK_PRONE] * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100) + 0.5)) + GetAPsProne(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE*2) );
			else 
				return( APBPConstants[AP_LOOK_PRONE] + GetAPsProne(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE*2) );
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
	UINT8 ubReadyAPs = 0;

	if(pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO)//dnl ch72 250913
		usItem = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS]);
	else
		usItem = pSoldier->inv[HANDPOS].usItem;

	// If this is a dwel pistol anim
	// ATE: What was I thinking, hooking into animations like this....
	//if ( usAnimState == READY_DUAL_STAND || usAnimState == READY_DUAL_CROUCH )
	//{
		//return( APBPConstants[AP_READY_DUAL] );
	//}
	if ( AM_A_ROBOT( pSoldier ) && gGameExternalOptions.fRobotNoReadytime){
		return 0;
	}
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
		ubReadyAPs = ( max(rt1,rt2) + APBPConstants[AP_READY_DUAL] );
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
			ubReadyAPs = Weapon[ usItem ].ubReadyTime;

			//ADB if 0, which it normally is, avoid the function call and division
			if (ubReadyAPs == 0)
			{
				return 0;
			}
			else 
			{		
				// Get bonus to draw speed from attachments
				ubReadyAPs = (( ubReadyAPs * ( 100 - GetPercentReadyTimeAPReduction(&pSoldier->inv[ HANDPOS ]) ) ) / 100);

				// alternative weapon holding?
				if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding )
				{
					// If we are told to go to the alt weapon holding mode
					if ( gAnimControl[ usAnimState ].uiFlags & ( ANIM_ALT_WEAPON_HOLDING ) || (pSoldier->bScopeMode == USE_ALT_WEAPON_HOLD && usAnimState == INVALID_ANIMATION) )//dnl ch71 180913
					{
						if ( Item[ usItem ].twohanded )
						{
							// Raising only to hip, either charge no APs or a portion of them
							ubReadyAPs = ((ubReadyAPs * gGameExternalOptions.ubToAltWeaponHoldReadyAPsPerc) + 99) / 100 ; // round up for rifles
						}
						else
						{
							// raising pistol one-handed, make it faster
							ubReadyAPs = (ubReadyAPs * gGameExternalOptions.ubToAltWeaponHoldReadyAPsPerc) / 100 ; // round down for pistols
						}
					}
					// If we are told to go from alternative to standard weapon holding
					else if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_ALT_WEAPON_HOLDING ) )
					{
						if ( Item[ usItem ].twohanded )
						{
							ubReadyAPs = ubReadyAPs * gGameExternalOptions.ubFromAltWeaponHoldReadyAPsPerc / 100; // round down for rifles
						}
						else
						{
							ubReadyAPs = ((ubReadyAPs * gGameExternalOptions.ubFromAltWeaponHoldReadyAPsPerc)+ 99) / 100; // round up for pistols
						}
					}
				}

				// STOMP traits
				// Decrease APs to ready LMG if having Auto Weapon trait
				if ( (Weapon[usItem].ubWeaponType == GUN_LMG) && HAS_SKILL_TRAIT(pSoldier, AUTO_WEAPONS_NT) && ( gGameOptions.fNewTraitSystem ))
				{
					ubReadyAPs = (( ubReadyAPs * max(0, (100 - gSkillTraitValues.ubAWPercentReadyLMGReduction * NUM_SKILL_TRAITS(pSoldier, AUTO_WEAPONS_NT) ) ) / 100)); // -10% per trait
				}
				// Decrease APs to ready pistols and revolvers if having Gunslinger trait
				else if ( (Weapon[usItem].ubWeaponType == GUN_PISTOL) && HAS_SKILL_TRAIT(pSoldier, GUNSLINGER_NT) && ( gGameOptions.fNewTraitSystem ))
				{
					ubReadyAPs = (( ubReadyAPs * max(0, (100 - gSkillTraitValues.ubGSPercentReadyPistolsReduction * NUM_SKILL_TRAITS(pSoldier, GUNSLINGER_NT) ) ) / 100)); // -15% per trait
				}
			}
		}
	}

	return( (INT16)max( 0, ubReadyAPs ) );
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
#if 0//dnl ch72 180913
	UINT16 usTargID;
	UINT32 uiMercFlags;
	UINT8 ubDirection;

	if(pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO)//dnl ch63 240813
		usInHand = GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS]);
	else
#endif
		// make sure the guy's actually got a throwable item in his hand!
		usInHand = pSoldier->inv[HANDPOS].usItem;
#if 0//dnl ch72 180913 this goes down because of new trait system	
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
#endif

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

	// moved from above - SANDRO
	//dnl ch72 180913 someone create throwing grenades from crouch so ANIM_STAND is not only stance for throwing, and as throwing object disappears from our hand when is thrown we need to determine desired height for turning cost
	if(ubAddTurningCost && !TileIsOutOfBounds(sGridNo) && GetDirectionFromGridNo(sGridNo, pSoldier) != pSoldier->ubDirection)
		ubAddTurningCost = TRUE;
	else
		ubAddTurningCost = FALSE;
	if(gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE)
	{
		iAPCost += GetAPsToChangeStance(pSoldier, ANIM_CROUCH);
		iAPCost += CalculateTurningCost(pSoldier, usInHand, ubAddTurningCost, ANIM_CROUCH);
	}
	else
	{
		iAPCost += GetAPsToChangeStance(pSoldier, gAnimControl[pSoldier->usAnimState].ubEndHeight);
		iAPCost += CalculateTurningCost(pSoldier, usInHand, ubAddTurningCost);
	}

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

INT16 GetAPsToHandcuff( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	INT16						sAPCost = 0;

	sAPCost = PlotPath( pSoldier, usMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
		
	sAPCost += APBPConstants[AP_HANDCUFF];

	return sAPCost;
}

INT16 GetAPsToApplyItem( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	INT16 sAPCost = 0;

	sAPCost = PlotPath( pSoldier, usMapPos, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
		
	sAPCost += APBPConstants[AP_APPLYITEM];

	return sAPCost;
}

// added by Flugente
INT16 GetAPsForMultiTurnAction( SOLDIERTYPE *pSoldier, UINT8 usActionType )
{
	INT16 sAPCost = 0;

	switch ( usActionType )
	{
	case MTA_FORTIFY:
		sAPCost += APBPConstants[AP_FORTIFICATION];
		break;
	case MTA_REMOVE_FORTIFY:
		sAPCost += APBPConstants[AP_REMOVE_FORTIFICATION];
		break;
	case MTA_FILL_SANDBAG:
		sAPCost += APBPConstants[AP_FILL_SANDBAG];
		break;
	}

	if ( usActionType == MTA_FORTIFY || usActionType == MTA_REMOVE_FORTIFY || usActionType == MTA_FILL_SANDBAG )
	{
		sAPCost = (sAPCost * (100 + pSoldier->GetBackgroundValue(BG_FORTIFY))) / 100;
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
#ifndef dnlCALCBESTSHOT//dnl ch69 150913 if this is turn on we get incorrect APs as MinAPsToShootOrStab always include raise gun cost
	if (!TANK(pSoldier) && !( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_FIREREADY | ANIM_FIRE )))//dnl ch64 310813
	{
		// Weapon not ready, check aiming from hip, else add raise gun cost
		//if (!pSoldier->IsValidShotFromHip(bAimTime,sTargetGridNo))
		{
			sAPCost += CalculateRaiseGunCost( pSoldier, TRUE, sTargetGridNo, bAimTime );
		}
	}
#endif
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
			
			if ( (UsingNewCTHSystem() == false && IsScoped(&pSoldier->inv[HANDPOS]) && GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo ) >= GetMinRangeForAimBonus(pSoldier, &pSoldier->inv[HANDPOS]) && !pSoldier->IsValidAlternativeFireMode(bAimTime,sTargetGridNo))
				|| (UsingNewCTHSystem() == true && GetBestScopeMagnificationFactor(pSoldier, &pSoldier->inv[HANDPOS], (FLOAT)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo ) > 1.0 ) && !pSoldier->IsValidAlternativeFireMode(bAimTime,sTargetGridNo)))
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
		iFinalAPsToCrouch += fBackpackCheck;//dnl ch70 160913 was 1

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
		iFinalAPsToLieDown += fBackpackCheck;//dnl ch70 160913 was 1

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

INT32 GetBPCostPer10APsForGunHolding( SOLDIERTYPE * pSoldier, BOOLEAN fEstimate )
{
	if ( !gGameExternalOptions.ubEnergyCostForWeaponWeight )
		return 0;
	if ( pSoldier == NULL )
		return 0;
	if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass != IC_GUN && Item[pSoldier->inv[HANDPOS].usItem].usItemClass != IC_LAUNCHER )
		return 0;
	if ( !(gAnimControl[ pSoldier->usAnimState ].uiFlags & (ANIM_FIRE | ANIM_FIREREADY) ) && !fEstimate ) // don't if weapon not raised, but do if we are gonna estimate the cost
		return 0;
	if ( TANK( pSoldier ) || AM_A_ROBOT( pSoldier ) )
		return 0;
	//////////////////////////////////////////////////////////////////////////////
	// THE BASIC COST FOR HOLDING THE GUN RAISED (per AP)
	//////////////////////////////////////////////////////////////////////////////
	// This somehow represents the effort we must make to keep the gun shouldered and pointing on a target.
	// The cost is not directly generated by firing, but is applied on the fire, as we spend APs.. it is also applied
	// on shouldering the gun and keeping the gun shouldered over the turns, as well as walking with it up, or turning around.

	INT32 iBPcost = 0;

	// Get weapon total weight
	INT16 sWeaponWeight = CalculateObjectWeight(&pSoldier->inv[pSoldier->ubAttackingHand]);

	// Make the weight impact exponential, so lightweight guns cost very little or nearly no energy, 
	// but as the gun weight goes up, the impact raises dramatically	

	// this divisor controls the final value and is adjusted by our strength
	INT8 bDivisor = 30;
	double dModifier = 0;
	// +0.25 per strength between 70 and 80, +0.5 per strength between 80 and 90, +0.75 per strength over 90 
	// --> max modifier +15 at 100 strength, which equals cca -33% energy cost compared to a merc with 60-70 strength
	INT16 sOurStrength = EffectiveStrength( pSoldier, FALSE );
	if ( sOurStrength > 70 )
	{
		dModifier += (sOurStrength - 70)/4;
		if ( sOurStrength > 80 )
		{
			dModifier += (sOurStrength - 80)/4;
			if ( sOurStrength > 90 )
				dModifier += (sOurStrength - 90)/4;
		}
	}
	// -0.25 per strength below 60, further -0.25 per strength below 50, and further -0.25 per strength below 40 
	// --> max modifier -15 at 40- strength, which equals doubled energy cost compared to a merc with 60-70 strength
	else if ( sOurStrength < 60 )
	{
		dModifier -= (60 - sOurStrength)/4;
		if ( sOurStrength < 50 )
		{
			dModifier -= (50 - sOurStrength)/4;
			if ( sOurStrength < 40 )
				dModifier -= (40 - sOurStrength)/4;
		}
	} 
	// Round the number properly
	if (dModifier)
		bDivisor += (INT8)(dModifier + 0.5);
	else
		bDivisor += (INT8)(dModifier - 0.5);

	// Calculate the basic cost 
	// Note: we are multiplying it by 10 here because the final cost is per 10 APs, and because we need some precision in the calculation
	iBPcost = 10 * sWeaponWeight * sWeaponWeight / bDivisor;

	// ------------------------------------
	// APPLY MODIFIERS FOR STANCE AND BIPOD
	dModifier = 0;
	// Alternative weapon holding?
	if (( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ALT_WEAPON_HOLDING ) || (fEstimate && pSoldier->bScopeMode == USE_ALT_WEAPON_HOLD) )
	{
		if ( Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded ) // firing from hip is not nearly бn effort
			dModifier += 80; // only 20% cost if on hip
		else // holding pistol in one hand is worse in this case							
			dModifier -= 25; // increased cost by 25%
	}
	// If weapon is rested on something or we are prone, reduce the power
	if ( gGameExternalOptions.fWeaponResting && pSoldier->IsWeaponMounted() || ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE ) )
	{
		dModifier += 90; // only 10% of the regular cost if prone or rested	
		if ( GetBipodBonus(&pSoldier->inv[pSoldier->ubAttackingHand]) > 0)
			iBPcost = 0; // with bipod on top, there is no bp cost
	}
	else if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH )
		dModifier += 50; // only 50% of the cost if crouching

	if ( dModifier != 0 )
	{
		dModifier = min( 99, dModifier );
		iBPcost = (INT32)(iBPcost * (100 - dModifier) / 100);
	}
	iBPcost = max(0, iBPcost);

	// What the hell, can't shoulder these, return some crazy breath cost
	if ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].HeavyGun && gGameExternalOptions.ubAllowAlternativeWeaponHolding && (!(gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ALT_WEAPON_HOLDING) || (fEstimate && pSoldier->bScopeMode != USE_ALT_WEAPON_HOLD )) )
	{
		iBPcost = (iBPcost + 25) * 5;
	}

	// For enemies, modify by difficulty
	if ( pSoldier->bTeam == ENEMY_TEAM )
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				break;
			case DIF_LEVEL_MEDIUM:
				iBPcost = (iBPcost * 9)/10; // -10%
				break;
			case DIF_LEVEL_HARD:
				iBPcost = (iBPcost * 3)/4; // -25%
				break;
			case DIF_LEVEL_INSANE:
				iBPcost /= 2; // -50%
				break;
		}
	}

	// Adjust by predefined ini setting
	iBPcost = (iBPcost * gGameExternalOptions.ubEnergyCostForWeaponWeight / 100);	

	// Now we have a number, but we must convert this number to represent BP cost PER 1 AP
	// This is roughly "a guess"
	iBPcost /= 25;
	// this can make the cost zero, but that's what we actully want - with very light guns, there is no effort at all

	// ::: overview :::
	// a merc with 85 strength (+5 modifier to divisor)...		
	// 1) a gun weighting 3 kilos (lightweight ARs), generates cca 25 points														 = 1 BP per AP = 100 BPs per full turn = pretty much nothing
	// 2) a gun weighting 5 kilos (most ARs with some attachments), generates cca 71 points											 = 3 BPs per AP = 300 BPs per full turn = still ok, similar to climb down a roof once
	// 3) a gun weighting 8 kilos ("lighter" LMGs like PKM, or heavier ARs with lots of attachments), generates cca 183 points		 = 7 BPs per AP = 700 BPs per full turn = you can handle it, its just like climb roof up and then down once
	// 4) a gun weighting 11 kilos (Rheinmetall MG3, FN MAG), generates cca 346 points												 = 13 BPs per AP = 1300 BPs per full turn = starts to feel rather heavy soon
	// 5) s gun weighting 15 kilos (you trying to shoulder Barrett? or Browning M1919?), generates cca 643 points					 = 26 BPs per AP = 2600 BPs per turn = uhh, 

	// after testing, a gun weighting 12 kilos, shouldered in standing position, with 85 strength, together with recoil kick (small one on this one though)... 
	// took 5-6 whole turns to totaly exhaust the merc, if one turn is equal 20-30 seconds, then it is 2-3 minutes... I think that's reasonable

	return ( iBPcost );
}

INT32 GetBPCostForRecoilkick( SOLDIERTYPE * pSoldier )
{
	if ( !gGameExternalOptions.ubEnergyCostForWeaponRecoilKick )
		return 0;
	if ( pSoldier == NULL )
		return 0;
	if ( Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usItemClass != IC_GUN && Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].usItemClass != IC_LAUNCHER )
		return 0;
	if ( TANK( pSoldier ) || AM_A_ROBOT( pSoldier ) )
		return 0;
	//////////////////////////////////////////////////////////////////////////////
	// THE GUN RECOIL KICK ENERGY COST
	//////////////////////////////////////////////////////////////////////////////
	// This is not about how heavy is the gun, but how much it kicks our shoulder.
	// Single shots may not prove much effect, but on autofire this may incure some nasty energy loss with certain guns.

	// Impact indirectly speaks of the size of the bullet, and thus strength of the backforce, theoretically.. so take it to the account.
	// (Other than that, we have nothing to base our calculation on by now, the solution would be to make a new tag in weapons.xml, but that's modder-unfriendly.)
	INT32 iKickPower = (INT32)(Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubImpact);

	// get weapon total weight
	// Weapon weight here actually helps us, since it absorbs the power of the backforce.
	INT16 sWeaponWeight = CalculateObjectWeight(&pSoldier->inv[pSoldier->ubAttackingHand]);
	
	// --------------------------
	// CALCULATE BASIC KICK POWER
	// "Standard weapon" impact is around 30, "standard weapon" weight plus some attachments is 3-5 kilos,
	// so for 30 and less impact make the kick power equal to it, then exponentially rise, so 40 impact makes double the amount.
	// Some crazy weapons with 50 (or more) impact then have more than 5x more kick power than "reasonable" guns.
	if ( iKickPower > 30 )
		iKickPower = 30 + ((iKickPower - 30) * (iKickPower - 30) / 3);

	// reduce it by 1% per 0,2 kilo of weapon weight
	sWeaponWeight = min(99,(sWeaponWeight / 2));
	iKickPower = iKickPower * (100 - sWeaponWeight) / 100;

	// If one-handed gun, reduce it a bit, since the whole thing is somewhat different.
	if ( !Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded )
		iKickPower = iKickPower * 3 / 4; // -25%

	// ::: overview :::
	// Glock 17 generates cca 18 points
	// Colt M4 (with some attachemnts) generates cca 25 points
	// .50 Beowulf (with some attachments) generates cca 40 points
	// OSV-96 generates cca 115 points

	double dModifier = 0;

	// ----------------------------
	// APPLY REDUCTION FOR STRENGTH 
	// Furthermore reduce the kicking power by our strength (with "exceptionally" low strength, we get a penalty though). 
	// -1,5% per strength over 90, -1% per strength between 80 and 90, and -0.5% per strength between 70 and 80 
	// --> max reduction -30% at 100 strength
	INT16 sOurStrength = EffectiveStrength( pSoldier, FALSE );
	if ( sOurStrength < 55 )
	{
		dModifier -= (55 - sOurStrength) / 2; //-0.5% per strength below 55 
		if ( sOurStrength < 35 )
			dModifier -= (35 - sOurStrength) / 2; //-1% total per strength below 35 
	}
	else if ( sOurStrength > 70 )
	{
		dModifier += (sOurStrength - 70) / 2; //-0.5% per strength over 70 
		if ( sOurStrength > 80 )
		{
			dModifier += (sOurStrength - 80) / 2; //-1% total per strength over 80
			if ( sOurStrength > 90 )
				dModifier += (sOurStrength - 90) / 2; //-1,5% total per strength over 90
		}
	} 
	if ( dModifier != 0 )
	{
		dModifier = min( 99, dModifier );
		iKickPower = (INT32)(iKickPower * (100 - dModifier) / 100);
	}

	// ------------------------------------
	// APPLY MODIFIERS FOR STANCE AND BIPOD
	dModifier = 0;
	// Alternative weapon holding?
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ALT_WEAPON_HOLDING )
	{
		if ( Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].twohanded ) // firing from hip makes the kicking rather diminishing
			dModifier += 80; // only 20% of the regular kick power 
		else // holding pistol in one hand is worse in this case							
			dModifier -= 33; // plus 33% power
	}
	// If weapon is rested on something or we are prone, reduce the power
	if ( gGameExternalOptions.fWeaponResting && pSoldier->IsWeaponMounted() || ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE ) )
	{
		dModifier += 40; // only 60% of the regular kick power if prone		
		dModifier += (2 * GetBipodBonus(&pSoldier->inv[pSoldier->ubAttackingHand])); // minus up to 20% for bipod	
	}
	else if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH )
		dModifier += 20; // only 80% of the regular kick power if crouching

	if ( dModifier != 0 )
	{
		dModifier = min( 99, dModifier );
		iKickPower = (INT32)(iKickPower * (100 - dModifier) / 100);
	}

	// ---------------------------
	// APPLY MODIFIER FOR AUTOFIRE
	// Now, on autofire this becomes real thing.. the kick power evolves by the length of the autofire
	// Depending somehow on the weapon autofire penalty, the 4th-6th bullet can already deal double the amount of energy loss.
	if ( pSoldier->bDoBurst > 1 )
	{					
		// per every shot after the first one, the power increases by an amount based on the weapon steadiness (autopenalty - reduction of it)
		// the additional kick factor is 5% base + half the auto penalty, but is itself also reduced by the above adjustments
		INT16 sSteadiness = Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].AutoPenalty - GetAutoToHitBonus(&pSoldier->inv[pSoldier->ubAttackingHand], FALSE );
		dModifier = (INT32)((3 + (sSteadiness / 2)) * (100 - dModifier) / 100); 
		if ( dModifier > 0 )
		{
			for ( INT16 i = 1; i < pSoldier->bDoBurst; i++ )
			{
				iKickPower = (INT32)(iKickPower * ( 100 + dModifier ) / 100);
			}
		}
	}

	// For enemies, modify by difficulty
	if ( pSoldier->bTeam == ENEMY_TEAM )
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				break;
			case DIF_LEVEL_MEDIUM:
				iKickPower = (iKickPower * 9)/10; // -10%
				break;
			case DIF_LEVEL_HARD:
				iKickPower = (iKickPower * 3)/4; // -25%
				break;
			case DIF_LEVEL_INSANE:
				iKickPower /= 2; // -50%
				break;
		}
	}
	
	// adjust by predefined ini setting
	iKickPower = (iKickPower * gGameExternalOptions.ubEnergyCostForWeaponRecoilKick / 100);
	iKickPower = max(0, iKickPower);

	return ( iKickPower );
}