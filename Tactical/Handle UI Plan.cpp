#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "Handle UI Plan.h"
	#include "overhead.h"
	#include "Isometric Utils.h"
	#include "pathai.h"
	#include "Handle UI.h"
	#include "points.h"
	#include "weapons.h"
	#include "renderworld.h"
	#include "Animation Control.h"
	#include "message.h"
	#include "soldier create.h"
	#include "interface.h"
#endif

UINT8						gubNumUIPlannedMoves			= 0;
SOLDIERTYPE			*gpUIPlannedSoldier			= NULL;
SOLDIERTYPE			*gpUIStartPlannedSoldier = NULL;
BOOLEAN					gfInUIPlanMode					= FALSE;


void SelectPausedFireAnimation( SOLDIERTYPE *pSoldier );


BOOLEAN BeginUIPlan( SOLDIERTYPE *pSoldier )
{
	gubNumUIPlannedMoves = 0;
	gpUIPlannedSoldier				= pSoldier;
	gpUIStartPlannedSoldier		= pSoldier;
	gfInUIPlanMode			= TRUE;

	gfPlotNewMovement	= TRUE;

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Entering Planning Mode" );

	return( TRUE );
}

BOOLEAN AddUIPlan( INT32 sGridNo, UINT8 ubPlanID )
{
	SOLDIERTYPE				*pPlanSoldier;
	INT16							sXPos, sYPos;
	INT16							sAPCost = 0;
	INT8							bDirection;
	INT32							iLoop;
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	UINT8							ubNewIndex;

	// Depeding on stance and direction facing, add guy!

	// If we have a planned action here, ignore!


	// If not OK Dest, ignore!
	if ( !NewOKDestination( gpUIPlannedSoldier, sGridNo, FALSE, (INT8)gsInterfaceLevel ) )
	{
		return( FALSE );
	}


	if ( ubPlanID == UIPLAN_ACTION_MOVETO )
	{
		// Calculate cost to move here
		sAPCost = PlotPath( gpUIPlannedSoldier, sGridNo, COPYROUTE, NO_PLOT, TEMPORARY, (UINT16) gpUIPlannedSoldier->usUIMovementMode, NOT_STEALTH, FORWARD,	gpUIPlannedSoldier->bActionPoints );
		// Adjust for running if we are not already running
		if (	gpUIPlannedSoldier->usUIMovementMode == RUNNING )
		{
			sAPCost += GetAPsStartRun( gpUIPlannedSoldier ); // changed by SANDRO
		}

		if ( EnoughPoints( gpUIPlannedSoldier, sAPCost, 0, FALSE ) )
		{
			MercCreateStruct.initialize();
			MercCreateStruct.bTeam				= SOLDIER_CREATE_AUTO_TEAM;
			MercCreateStruct.ubProfile		= NO_PROFILE;
			MercCreateStruct.fPlayerPlan	= TRUE;
			MercCreateStruct.ubBodyType		= gpUIPlannedSoldier->ubBodyType;
			MercCreateStruct.sInsertionGridNo		= sGridNo;

			// Get Grid Corrdinates of mouse
			if ( TacticalCreateSoldier( &MercCreateStruct, &ubNewIndex ) )
			{
				// Get pointer to soldier
				GetSoldier( &pPlanSoldier, (UINT16)ubNewIndex );

				pPlanSoldier->sPlannedTargetX = -1;
				pPlanSoldier->sPlannedTargetY = -1;

				// Compare OPPLISTS!
				// Set ones we don't know about but do now back to old ( ie no new guys )
				for (iLoop = 0; iLoop < MAX_NUM_SOLDIERS; iLoop++ )
				{
					if ( gpUIPlannedSoldier->aiData.bOppList[ iLoop ] < 0 )
					{
							pPlanSoldier->aiData.bOppList[ iLoop ] = gpUIPlannedSoldier->aiData.bOppList[ iLoop ];
					}
				}

				// Get XY from Gridno
				ConvertGridNoToCenterCellXY( sGridNo, &sXPos, &sYPos );

				pPlanSoldier->EVENT_SetSoldierPosition( sXPos, sYPos );
				pPlanSoldier->EVENT_SetSoldierDestination( (UINT8) sGridNo ); // Hopefully this code is never used anymore because the second param is now direction, not grid
				pPlanSoldier->bVisible = 1;
				pPlanSoldier->usUIMovementMode = gpUIPlannedSoldier->usUIMovementMode;


				pPlanSoldier->bActionPoints = gpUIPlannedSoldier->bActionPoints - sAPCost;

				pPlanSoldier->ubPlannedUIAPCost = (UINT8)pPlanSoldier->bActionPoints;

				// Get direction
				bDirection = (INT8)gpUIPlannedSoldier->pathing.usPathingData[ gpUIPlannedSoldier->pathing.usPathDataSize - 1 ];

				// Set direction
				pPlanSoldier->ubDirection = bDirection;
				pPlanSoldier->pathing.bDesiredDirection = bDirection;

				// Set walking animation
				pPlanSoldier->ChangeSoldierState( pPlanSoldier->usUIMovementMode, 0, FALSE );

				// Change selected soldier
				gusSelectedSoldier = (UINT16)pPlanSoldier->ubID;

				// Change global planned mode to this guy!
				gpUIPlannedSoldier = pPlanSoldier;

				gubNumUIPlannedMoves++;

				gfPlotNewMovement	= TRUE;

				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Adding Merc Move to Plan" );

			}
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Merc will not have enough action points" );
		}
	}
	else if ( ubPlanID == UIPLAN_ACTION_FIRE )
	{
 	sAPCost = CalcTotalAPsToAttack( gpUIPlannedSoldier, sGridNo, TRUE, (INT16)(gpUIPlannedSoldier->aiData.bShownAimTime ) );

		// Get XY from Gridno
		ConvertGridNoToCenterCellXY( sGridNo, &sXPos, &sYPos );


		// If this is a player guy, show message about no APS
		if ( EnoughPoints( gpUIPlannedSoldier, sAPCost, 0, FALSE ) )
		{
			// CHECK IF WE ARE A PLANNED SOLDIER OR NOT< IF SO< CREATE!
			if ( gpUIPlannedSoldier->ubID < MAX_NUM_SOLDIERS )
			{
				MercCreateStruct.initialize();
				MercCreateStruct.bTeam				= SOLDIER_CREATE_AUTO_TEAM;
				MercCreateStruct.ubProfile		= NO_PROFILE;
				MercCreateStruct.fPlayerPlan	= TRUE;
				MercCreateStruct.ubBodyType		= gpUIPlannedSoldier->ubBodyType;
				MercCreateStruct.sInsertionGridNo		= sGridNo;

				// Get Grid Corrdinates of mouse
				if ( TacticalCreateSoldier( &MercCreateStruct, &ubNewIndex ) )
				{
					// Get pointer to soldier
					GetSoldier( &pPlanSoldier, (UINT16)ubNewIndex );

					pPlanSoldier->sPlannedTargetX = -1;
					pPlanSoldier->sPlannedTargetY = -1;

					// Compare OPPLISTS!
					// Set ones we don't know about but do now back to old ( ie no new guys )
					for (iLoop = 0; iLoop < MAX_NUM_SOLDIERS; iLoop++ )
					{
						if ( gpUIPlannedSoldier->aiData.bOppList[ iLoop ] < 0 )
						{
								pPlanSoldier->aiData.bOppList[ iLoop ] = gpUIPlannedSoldier->aiData.bOppList[ iLoop ];
						}
					}

					pPlanSoldier->EVENT_SetSoldierPosition( gpUIPlannedSoldier->dXPos, gpUIPlannedSoldier->dYPos );
					pPlanSoldier->EVENT_SetSoldierDestination( (UINT8) gpUIPlannedSoldier->sGridNo );
					pPlanSoldier->bVisible = 1;
					pPlanSoldier->usUIMovementMode = gpUIPlannedSoldier->usUIMovementMode;


					pPlanSoldier->bActionPoints = gpUIPlannedSoldier->bActionPoints - sAPCost;

					pPlanSoldier->ubPlannedUIAPCost = (UINT8)pPlanSoldier->bActionPoints;

					// Get direction
					bDirection = (INT8)gpUIPlannedSoldier->pathing.usPathingData[ gpUIPlannedSoldier->pathing.usPathDataSize - 1 ];

					// Set direction
					pPlanSoldier->ubDirection = bDirection;
					pPlanSoldier->pathing.bDesiredDirection = bDirection;

					// Set walking animation
					pPlanSoldier->ChangeSoldierState( pPlanSoldier->usUIMovementMode, 0, FALSE );

					// Change selected soldier
					gusSelectedSoldier = (UINT16)pPlanSoldier->ubID;

					// Change global planned mode to this guy!
					gpUIPlannedSoldier = pPlanSoldier;

					gubNumUIPlannedMoves++;
				}


			}

			gpUIPlannedSoldier->bActionPoints = gpUIPlannedSoldier->bActionPoints - sAPCost;

			gpUIPlannedSoldier->ubPlannedUIAPCost = (UINT8)gpUIPlannedSoldier->bActionPoints;

			// Get direction from gridno
			bDirection = GetDirectionFromGridNo( sGridNo, gpUIPlannedSoldier );

			// Set direction
			gpUIPlannedSoldier->ubDirection = bDirection;
			gpUIPlannedSoldier->pathing.bDesiredDirection = bDirection;

			// Set to shooting animation
			SelectPausedFireAnimation( gpUIPlannedSoldier );

			gpUIPlannedSoldier->sPlannedTargetX = sXPos;
			gpUIPlannedSoldier->sPlannedTargetY = sYPos;

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Adding Merc Shoot to Plan" );

		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Merc will not have enough action points" );
		}
	}
	return( TRUE );
}


void EndUIPlan(	)
{
	int				cnt;
	SOLDIERTYPE *pSoldier;

	// Zero out any planned soldiers
	for( cnt = MAX_NUM_SOLDIERS; cnt < TOTAL_SOLDIERS; cnt++ )
	{
		pSoldier = MercPtrs[ cnt ];

		if ( pSoldier->bActive )
		{
			if ( pSoldier->sPlannedTargetX != -1 )
			{
				SetRenderFlags(RENDER_FLAG_FULL );
			}
			TacticalRemoveSoldier( pSoldier->ubID );
		}


	}
	gfInUIPlanMode			= FALSE;
	gusSelectedSoldier	= gpUIStartPlannedSoldier->ubID;

	gfPlotNewMovement	= TRUE;

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Leaving Planning Mode" );

}

BOOLEAN InUIPlanMode( )
{
	return( gfInUIPlanMode );
}


void SelectPausedFireAnimation( SOLDIERTYPE *pSoldier )
{
	// Determine which animation to do...depending on stance and gun in hand...

	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
		case ANIM_STAND:

			if ( pSoldier->bDoBurst > 0 )
			{
				if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_ALT_WEAPON_HOLDING ) )
				{
					pSoldier->ChangeSoldierState( BURST_ALTERNATIVE_STAND, 2 , FALSE );
				}
				else
				{
					pSoldier->ChangeSoldierState( STANDING_BURST, 2 , FALSE );
				}
			}
			else
			{
				if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_ALT_WEAPON_HOLDING ) )
				{
					pSoldier->ChangeSoldierState( SHOOT_ALTERNATIVE_STAND, 2 , FALSE );
				}
				else
				{
					pSoldier->ChangeSoldierState( SHOOT_RIFLE_STAND, 2 , FALSE );
				}
			}
			break;

		case ANIM_PRONE:
			pSoldier->ChangeSoldierState( SHOOT_RIFLE_PRONE, 2 , FALSE );
			break;

		case ANIM_CROUCH:
			pSoldier->ChangeSoldierState( SHOOT_RIFLE_CROUCH, 2 , FALSE );
			break;

	}

}
