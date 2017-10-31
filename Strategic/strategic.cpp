#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Strategic.h"
	#include "Types.h"
	#include "Squads.h"
	#include "jascreens.h"
	#include "Assignments.h"
	#include "Interface Control.h"
	#include "Overhead.h"
	#include "Soldier Profile.h"
	#include "Dialogue Control.h"
	#include "personnel.h"
	#include "Tactical Save.h"
	#include "Isometric Utils.h"
	#include "Vehicles.h"
	#include "Game Clock.h"
	#include "CampaignStats.h"		// added by Flugente
	#include "militiasquads.h"		// added by Flugente
	#include "Game Event Hook.h"	// added by Flugente
	#include "message.h"			// added by Flugente
	#include "Text.h"				// added by Flugente
	#include "Queen Command.h"		// added by Flugente
#endif

#ifdef JA2UB
#else
	// anv: for playable Speck
	#include "mercs.h"
#endif

StrategicMapElement StrategicMap[MAP_WORLD_X*MAP_WORLD_Y];

extern BOOLEAN fReDrawFace;



BOOLEAN HandleStrategicDeath( SOLDIERTYPE *pSoldier )
{
	// add the guy to the dead list
	//AddCharacterToDeadList( pSoldier );

	// If in a vehicle, remove them!
	if( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId != -1 ) )
	{
		// remove from vehicle
		TakeSoldierOutOfVehicle( pSoldier );
	}

	// if not in mapscreen
	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		// ATE; At least make them dead!
		if( ( pSoldier->bAssignment != ASSIGNMENT_DEAD ) )
		{
			SetTimeOfAssignmentChangeForMerc( pSoldier );
		}

		ChangeSoldiersAssignment( pSoldier, ASSIGNMENT_DEAD );
	}
	else if( ( pSoldier->stats.bLife == 0 )&&( pSoldier->bAssignment != ASSIGNMENT_DEAD ) )
	{
		// died in mapscreen

		fReDrawFace = TRUE;

		// dead
		if( ( pSoldier->bAssignment != ASSIGNMENT_DEAD ) )
		{
			SetTimeOfAssignmentChangeForMerc( pSoldier );
		}

		ChangeSoldiersAssignment( pSoldier, ASSIGNMENT_DEAD );

		//s et breath and breath max to 0
		pSoldier->bBreath = pSoldier->bBreathMax = 0;

		// rebuild list
		ReBuildCharactersList( );

		// ste merc as dead
		// pSoldier->flags.fUIdeadMerc = TRUE;

		// attempt to remove character from squad
		RemoveCharacterFromSquads( pSoldier );

		// handle any passign comments by grunts
		HandleSoldierDeadComments( pSoldier );

		// put the dead guys down
		AddDeadSoldierToUnLoadedSector( ( UINT8 ) ( pSoldier->sSectorX ), ( UINT8 )( pSoldier->sSectorY ), pSoldier->bSectorZ, pSoldier, RandomGridNo(), ADD_DEAD_SOLDIER_TO_SWEETSPOT );

		fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;

		StopTimeCompression();
	}
	
	return( TRUE );
}


void HandleSoldierDeadComments( SOLDIERTYPE *pSoldier )
{
	INT32 cnt = 0;
	SOLDIERTYPE *pTeamSoldier;
	INT8 bBuddyIndex;

	// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
	cnt = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID;


	// see if this was the friend of a living merc
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->stats.bLife >= OKLIFE && pTeamSoldier->bActive )
		{
			bBuddyIndex = WhichBuddy( pTeamSoldier->ubProfile, pSoldier->ubProfile );
			switch( bBuddyIndex )
			{
				case 0:
					// buddy #1 died!
					TacticalCharacterDialogue( pTeamSoldier, QUOTE_BUDDY_1_KILLED );
					break;
				case 1:
					// buddy #2 died!
					TacticalCharacterDialogue( pTeamSoldier, QUOTE_BUDDY_2_KILLED );
					break;
				case 2:
					// buddy #3 died!
					if( pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_AIM_BUDDY_3_KILLED );
					else
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_NON_AIM_BUDDY_3_KILLED );
					break;
				case 3:
					// buddy #4 died!
					if( pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_AIM_BUDDY_4_KILLED );
					else
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_NON_AIM_BUDDY_4_KILLED );
					break;
				case 4:
					// buddy #5 died!
					if( pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_AIM_BUDDY_5_KILLED );
					else
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_NON_AIM_BUDDY_5_KILLED );
					break;
				case 5:
					// learn to like buddy died!
					TacticalCharacterDialogue( pTeamSoldier, QUOTE_LEARNED_TO_LIKE_MERC_KILLED );
					break;
				default:
					break;
				}
			}

#ifdef JA2UB
#else
			// anv: handle Speck witnessing his employee death
			if( pTeamSoldier->ubProfile == SPECK_PLAYABLE && pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC )
				HandleSpeckWitnessingEmployeeDeath( pSoldier );
#endif

		}
}

BOOLEAN SetMilitiaMovementOrder(INT16 sX, INT16 sY, INT8 sZ, UINT32 dir)
{
	// militia can only move on the surface
	if ( sZ )
		return FALSE;
	
	// militia needs to be here...
	if ( !NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ) )
		return FALSE;

	// in order to give movement orders, we need a merc to staff a military headquarter
	if ( !MercStaffsMilitaryHQ() )
		return FALSE;

	INT16 newX = sX;
	INT16 newY = sY;
	UINT8 movetype = THROUGH_STRATEGIC_MOVE;

	switch (dir)
	{
	case MILITIA_MOVE_NORTH:
		--newY;
		movetype = NORTH_STRATEGIC_MOVE;
		break;

	case MILITIA_MOVE_WEST:
		--newX;
		movetype = WEST_STRATEGIC_MOVE;
		break;

	case MILITIA_MOVE_EAST:
		++newX;
		movetype = EAST_STRATEGIC_MOVE;
		break;

	case MILITIA_MOVE_SOUTH:
		++newY;
		movetype = SOUTH_STRATEGIC_MOVE;
		break;

	default:
		return FALSE;
		break;
	}

	if ( gGameExternalOptions.gflimitedRoaming && !IsSectorRoamingAllowed( SECTOR(newX, newY) ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gpStrategicString[STR_MILITIAMOVEMENT_NO_LIMITEDROAMING] );
		return FALSE;
	}

	UINT8 ubTraverseType = SectorInfo[ SECTOR(sX, sY) ].ubTraversability[ movetype ];

	if( ubTraverseType == GROUNDBARRIER || ubTraverseType == EDGEOFWORLD )
		return FALSE;

	UINT32 ubTraverseMod = 0;

	switch( ubTraverseType )
	{
		case TOWN:						ubTraverseMod = 1780;break;
		case ROAD:						ubTraverseMod = 100;break;
		case PLAINS:					ubTraverseMod = 85; break;
		case SAND:						ubTraverseMod = 50;	break;
		case SPARSE:					ubTraverseMod = 70;	break;
		case DENSE:						ubTraverseMod = 60;	break;
		case SWAMP:						ubTraverseMod = 35;	break;
		case WATER:						ubTraverseMod = 25;	break;
		case HILLS:						ubTraverseMod = 50;	break;
		case NS_RIVER:					ubTraverseMod = 25;	break;
		case EW_RIVER:					ubTraverseMod = 25;	break;
	}

	if( ubTraverseMod )
	{
		UINT32 timeneeded = FOOT_TRAVEL_TIME * 100 / ubTraverseMod;
		
		SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sX, sY ) ] );

		StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags &= ~MILITIA_MOVE_ALLDIRS;
		StrategicMap[ sX + ( sY * MAP_WORLD_X ) ].usFlags |= dir;

		AddStrategicEvent( EVENT_MILITIA_MOVEMENT_ORDER, GetWorldTotalMin() + timeneeded, SECTOR( sX, sY ) );
			
		fMapPanelDirty = TRUE;

		return TRUE;
	}

	return FALSE;
}