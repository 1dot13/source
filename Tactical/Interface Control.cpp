	#include <stdio.h>
	#include <time.h>
	#include <math.h>
	#include "sgp.h"
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "sysutil.h"
	#include "overhead.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "interface.h"
	#include "vsurface.h"
	#include "input.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "renderworld.h"
	#include "cursors.h"
	#include "radar screen.h"
	#include "worldman.h"
	#include "Font Control.h"
	#include "render dirty.h"
	#include "utilities.h"
	#include "interface cursors.h"
	#include "lighting.h"
	#include "Interface Panels.h"
	#include "pathai.h"
	#include "vobject_blitters.h"
	#include "faces.h"
	#include "Handle UI Plan.h"
	#include "interface control.h"
	#include "interface items.h"
	#include "interface dialogue.h"
	#include "interactive tiles.h"
	#include "Game Clock.h"
	#include "Strategic Exit GUI.h"
	#include "PopUpBox.h"
	#include "Assignments.h"
	#include "spread burst.h"
	#include "squads.h"
	#include "line.h"
	#include "Militia Control.h"
	#include "Map Screen Interface.h"
	#include "civ quotes.h"
	#include "GameSettings.h"
	#include "Explosion Control.h"		// added by Flugente

#include "connect.h"
#include "Text.h"

#ifdef JA2UB
#include "Dialogue Control.h"
#endif

/*	I deleted here declaration of clock coords for tactical screen i will declare them
 *	in functions that Initialize coord for SMPanet and TEAMPanel
 *	any questions? joker
 */
SGPRect				gOldClippingRect, gOldDirtyClippingRect;

UINT32		guiTacticalInterfaceFlags;

UINT16		gusUICurIntTileEffectIndex;
INT32			gsUICurIntTileEffectGridNo;
UINT8			gsUICurIntTileOldShade;

BOOLEAN		gfRerenderInterfaceFromHelpText = FALSE;

MOUSE_REGION		gLockPanelOverlayRegion;

// CHRISL: Change function definition to allow passing of X,Y coords to control placement of TownID string
extern void			RenderTownIDString( INT16 sX, INT16 sY );
extern BOOLEAN	gfUIOverItemPool;
extern INT32		gfUIOverItemPoolGridNo;
extern BOOLEAN	gfInMovementMenu;
extern BOOLEAN	gfInItemPickupMenu;
extern BOOLEAN	gfInOpenDoorMenu;
extern UINT32		guiUIMessageTimeDelay;


BOOLEAN	gfPausedTacticalRenderInterfaceFlags = FALSE;
BOOLEAN	gfPausedTacticalRenderFlags					= FALSE;

// which assignment menu can be shown?
extern void DetermineWhichAssignmentMenusCanBeShown( void );
extern void DetermineWhichMilitiaControlMenusCanBeShown( void ); //lal
extern void HandleAnyMercInSquadHasCompatibleStuff( UINT8 ubSquad, OBJECTTYPE *pObject, BOOLEAN fReset );
extern BOOLEAN RemoveFlashItemSlot( ITEM_POOL *pItemPool );


void SetTacticalInterfaceFlags( UINT32 uiFlags )
{
	guiTacticalInterfaceFlags = uiFlags;
}

void HandleTacticalPanelSwitch( )
{
	if ( gfSwitchPanel )
	{
		SetCurrentInterfacePanel( gbNewPanel );
		SetCurrentTacticalPanelCurrentMerc( gubNewPanelParam );
		gfSwitchPanel = FALSE;

		if ( !(guiTacticalInterfaceFlags & INTERFACE_NORENDERBUTTONS ) && !( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
		{
			RenderButtons( );
		}
	}
}

// Flugente: handle flashlights
void HandleFlashLights()
{
	if ( !fInterfacePanelDirty )
		return;

	SOLDIERTYPE* pSoldier = NULL;

	for (UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop )
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier != NULL && pSoldier->usSoldierFlagMask & SOLDIER_REDOFLASHLIGHT )
		{
			pSoldier->HandleFlashLights();
		}
	}
}

void RenderTacticalInterface( )
{

	// handle paused render of tactical
	HandlePausedTacticalRender( );

	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		HandleFlashingItems( );

	HandleMultiPurposeLocator( );

	}

	// Handle degrading new items...
	DegradeNewlyAddedItems( );

	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			RenderSMPanel( &fInterfacePanelDirty );
			break;

		case TEAM_PANEL:
			RenderTEAMPanel( fInterfacePanelDirty );
			break;
	}

	// Handle faces
	if( !(guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
		HandleAutoFaces( );

	// Flugente: handle flashlights
	HandleFlashLights();
}


void HandlePausedTacticalRender( void )
{

	// for a one frame paused render of tactical
	if( gfPausedTacticalRenderFlags )
	{
		gRenderFlags|=gfPausedTacticalRenderFlags;

		gfPausedTacticalRenderFlags = FALSE;
	}

	if ( gfPausedTacticalRenderInterfaceFlags )
	{
		fInterfacePanelDirty = gfPausedTacticalRenderInterfaceFlags;
		gfPausedTacticalRenderInterfaceFlags = FALSE;
	}

	return;
}


void RenderTacticalInterfaceWhileScrolling( )
{
	RenderButtons( );

	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			RenderSMPanel( &fInterfacePanelDirty );
			break;

		case TEAM_PANEL:
			RenderTEAMPanel( fInterfacePanelDirty );
			break;
	}

	// Handle faces
	HandleAutoFaces( );

}

void SetUpInterface( )
{
	SOLDIERTYPE								*pSoldier;
	LEVELNODE									*pIntTile;

	if ( ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	DrawUICursor( );

	SetupPhysicsTrajectoryUI( );


	if ( giUIMessageOverlay != -1 )
	{
		if ( ( GetJA2Clock( ) - guiUIMessageTime ) > guiUIMessageTimeDelay )
		{
			EndUIMessage( );
		}
	}

	if ( gusSelectedSoldier != NOBODY )
	{
			GetSoldier( &pSoldier, gusSelectedSoldier );
	}


	if ( gCurrentUIMode == OPENDOOR_MENU_MODE )
	{
		HandleOpenDoorMenu( );
	}

	HandleTalkingMenu( );

	if ( gCurrentUIMode == EXITSECTORMENU_MODE )
	{
		HandleSectorExitMenu( );
	}

	// FOR THE MOST PART - SHUTDOWN INTERFACE WHEN IT'S THE ENEMY'S TURN
	if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum )
	{
		return;
	}

	HandleInterfaceBackgrounds( );


	if ( gfUIHandleSelection == NONSELECTED_GUY_SELECTION )
	{
		if ( gsSelectedLevel > 0 )
		{
			AddRoofToHead( gsSelectedGridNo, GOODRING1 );
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;

		}
		else
		{
			AddObjectToHead( gsSelectedGridNo, GOODRING1 );
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
		}

	}

	if ( gfUIHandleSelection == SELECTED_GUY_SELECTION )
	{
		if ( gsSelectedLevel > 0 )
		{
			//AddRoofToHead( gsSelectedGridNo, SELRING1 );
			AddRoofToHead( gsSelectedGridNo, FIRSTPOINTERS2 );
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;

		}
		else
		{
			//AddObjectToHead( gsSelectedGridNo, SELRING1 );
			AddObjectToHead( gsSelectedGridNo, FIRSTPOINTERS2 );
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
		}

	}


	if ( gfUIHandleSelection == ENEMY_GUY_SELECTION )
	{
		if ( gsSelectedLevel > 0 )
		{
			AddRoofToHead( gsSelectedGridNo, FIRSTPOINTERS2 );
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pRoofHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;

		}
		else
		{
			AddObjectToHead( gsSelectedGridNo, FIRSTPOINTERS2 );
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[gsSelectedGridNo].pObjectHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
		}

	}

	if ( gfUIHandleShowMoveGrid )
	{
		if ( gusSelectedSoldier != NOBODY )
		{
			if ( MercPtrs[ gusSelectedSoldier ]->sGridNo != gsUIHandleShowMoveGridLocation )
			{
				if ( gfUIHandleShowMoveGrid == 2 )
				{
					AddTopmostToHead( gsUIHandleShowMoveGridLocation, GetSnapCursorIndex( FIRSTPOINTERS4 ) );
					gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
					gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
				}
				else
				{
					if ( MercPtrs[ gusSelectedSoldier ]->bStealthMode )
					{
						AddTopmostToHead( gsUIHandleShowMoveGridLocation, GetSnapCursorIndex( FIRSTPOINTERS9 ) );
						gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}
					else
					{
						AddTopmostToHead( gsUIHandleShowMoveGridLocation, GetSnapCursorIndex( FIRSTPOINTERS2 ) );
						gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						gpWorldLevelData[gsUIHandleShowMoveGridLocation].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}
				}
			}
		}
	}

	// Check if we are over an interactive tile...
	if ( gfUIShowCurIntTile )
	{
		pIntTile = GetCurInteractiveTileGridNo( &gsUICurIntTileEffectGridNo );

		if ( pIntTile != NULL )
		{
			gusUICurIntTileEffectIndex = pIntTile->usIndex;

			// Shade green
			gsUICurIntTileOldShade = pIntTile->ubShadeLevel;
			pIntTile->ubShadeLevel = 0;
			pIntTile->uiFlags |= LEVELNODE_DYNAMIC;
		}
	}
}


void ResetInterface( )
{
	LEVELNODE *pNode;

	if ( ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	// find out if we need to show any menus
	DetermineWhichAssignmentMenusCanBeShown();
	CreateDestroyAssignmentPopUpBoxes( );

	DetermineWhichMilitiaControlMenusCanBeShown( ); //lal
	CreateDestroyMilitiaControlPopUpBoxes( );


	HideUICursor( );

	ResetPhysicsTrajectoryUI( );

	if ( gfUIHandleSelection )
	{
		if ( gsSelectedLevel > 0 )
		{
			RemoveRoof( gsSelectedGridNo, GOODRING1 );
			RemoveRoof( gsSelectedGridNo, FIRSTPOINTERS2 );
		}
		else
		{
			RemoveObject( gsSelectedGridNo, FIRSTPOINTERS2 );
			RemoveObject( gsSelectedGridNo, GOODRING1 );

		}
	}

	if ( gfUIHandleShowMoveGrid )
	{
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS4	);
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS9	);
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS2 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS13 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS15 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS19 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS20 );
	}

	if ( fInterfacePanelDirty )
	{
		fInterfacePanelDirty = FALSE;
	}


	// Reset int tile cursor stuff
	if ( gfUIShowCurIntTile )
	{		
		if (!TileIsOutOfBounds(gsUICurIntTileEffectGridNo))
		{
			//Find our tile!
			pNode = gpWorldLevelData[ gsUICurIntTileEffectGridNo].pStructHead;

			while( pNode != NULL )
			{
				if ( pNode->usIndex == gusUICurIntTileEffectIndex )
				{
					pNode->ubShadeLevel = gsUICurIntTileOldShade;
					pNode->uiFlags &= (~LEVELNODE_DYNAMIC);
					break;
				}

				pNode = pNode->pNext;
			}
		}
	}

}


extern BOOLEAN AnyItemsVisibleOnLevel( ITEM_POOL *pItemPool, INT8 bZLevel );


UINT32	guiColors[ 12 ] =
{
	FROMRGB( 198, 163, 0 ),
	FROMRGB( 185, 150, 0 ),
	FROMRGB( 172, 136, 0 ),
	FROMRGB( 159, 123, 0 ),
	FROMRGB( 146, 110, 0 ),
	FROMRGB( 133, 96, 0 ),
	FROMRGB( 120, 83, 0 ),
	FROMRGB( 133, 96, 0 ),
	FROMRGB( 146, 110, 0 ),
	FROMRGB( 159, 123, 0 ),
	FROMRGB( 172, 136, 0 ),
	FROMRGB( 185, 150, 0 )
};

void RenderRubberBanding( )
{
	UINT16										usLineColor;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	INT16											iLeft, iRight, iTop, iBottom;
	INT32											iBack = -1;
	static INT32							iFlashColor = 0;
	static INT32							uiTimeOfLastUpdate = 0;

	if ( !gRubberBandActive )
		return;

	iLeft		= (INT16)gRubberBandRect.iLeft;
	iRight	= (INT16)gRubberBandRect.iRight;
	iTop		= (INT16)gRubberBandRect.iTop;
	iBottom	= (INT16)gRubberBandRect.iBottom;

	if ( iLeft == iRight && iTop == iBottom )
	{
		return;
	}

	if ( ( GetJA2Clock( ) - uiTimeOfLastUpdate ) > 60 )
	{
		uiTimeOfLastUpdate = GetJA2Clock( );
		iFlashColor++;

		if ( iFlashColor == 12 )
		{
			iFlashColor = 0;
		}
	}

	// Draw rectangle.....
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );

	usLineColor = Get16BPPColor( guiColors[ iFlashColor ] );

	if ( ( iRight - iLeft ) > 0 )
	{
		LineDraw( TRUE, iLeft, iTop, iRight, iTop, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iLeft, iTop, (INT16)( iRight + 1 ), (INT16)( iTop + 1 ) );
	}
	else if ( ( iRight - iLeft ) < 0 )
	{
		LineDraw( TRUE, iLeft, iTop, iRight, iTop, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iRight, iTop, (INT16)( iLeft + 1 ), (INT16)( iTop + 1 ) );
	}

	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}


	iBack = -1;

	if ( ( iRight - iLeft ) > 0 )
	{
		LineDraw( TRUE, iLeft, iBottom, iRight, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iLeft, iBottom, (INT16)( iRight +1 ), (INT16)( iBottom + 1 ) );
	}
	else if ( ( iRight - iLeft ) < 0 )
	{
		LineDraw( TRUE, iLeft, iBottom, iRight, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iRight, iBottom, (INT16)( iLeft + 1 ), (INT16)( iBottom + 1 ) );
	}

	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}



	iBack = -1;

	if ( ( iBottom - iTop ) > 0 )
	{
		LineDraw( TRUE, iLeft, iTop, iLeft, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iLeft, iTop, (INT16)( iLeft +1 ), iBottom );
	}
	else if ( ( iBottom - iTop ) < 0 )
	{
		LineDraw( TRUE, iLeft, iTop, iLeft, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iLeft, iBottom, (INT16)( iLeft + 1 ), iTop );
	}

	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}


	iBack = -1;

	if ( ( iBottom - iTop ) > 0 )
	{
		LineDraw( TRUE, iRight, iTop, iRight, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iRight, iTop, (INT16)( iRight +1 ), iBottom );
	}
	else if ( ( iBottom - iTop ) < 0 )
	{
		LineDraw( TRUE, iRight, iTop, iRight, iBottom, usLineColor, pDestBuf );
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, iRight, iBottom, (INT16)( iRight + 1 ), iTop );
	}

	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}



	UnLockVideoSurface( FRAME_BUFFER );
}

// Flugente: draw moving circles around a gridno. This is used to warn the player of impending explosions
void DrawExplosionWarning( INT32 sGridno, INT8 sLevel, INT8 sDelay )
{
	if ( TileIsOutOfBounds( sGridno ) )
		return;

	if ( sDelay <= 0 )
		return;

	UINT32					uiDestPitchBYTES;
	UINT8*					pDestBuf;
	INT32					iBack = -1;
	INT16					periods = 120 / sDelay;
	static INT32			uiTimeOfLastUpdate = 0;	
	static INT32			updatecounter = 0;
	INT16					sScreenX, sScreenY;
		
	// Get screen pos of gridno......
	GetGridNoScreenXY( sGridno, &sScreenX, &sScreenY );

	// ATE: If we are on a higher interface level, substract....
	if ( sLevel == 1 )
		sScreenY -= 50;
		
	if ( (GetJA2Clock( ) - uiTimeOfLastUpdate) > 30 )
	{
		uiTimeOfLastUpdate = GetJA2Clock( );

		if ( ++updatecounter >= 120 )
			updatecounter = 0;
	}

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	// make sure to check for these boundaries later on, and only draw inside them
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );

	UINT16 red = Get16BPPColor( FROMRGB( 255, 0, 0 ) );

	INT16 numcircles = max( 1, 4 - sDelay);

	// we determine the biggest rectangle we have to reserve
	INT32 best_xl = 99999;
	INT32 best_xr = 0;
	INT32 best_yl = 99999;
	INT32 best_yr = 0;
	BOOLEAN sthdrawn = FALSE;

	for ( int i = 0; i < numcircles; ++i )
	{
		INT32 radiusvar = (updatecounter + i * periods / numcircles) % periods;
		INT32 radius = min(8, max( 1, radiusvar / 8 ) );

		INT16 radius_inner = max( 2, radiusvar );
		INT16 radius_outer = radius_inner + radius;
		
		// determine area of where the circle will be drawn in, take into account what part of the sector we actually see
		INT32 xl = max( gsVIEWPORT_START_X, sScreenX - radius_outer );
		INT32 xr = min( gsVIEWPORT_END_X, sScreenX + radius_outer );
		INT32 yl = max( gsVIEWPORT_WINDOW_START_Y, sScreenY - radius_outer );
		INT32 yr = min( gsVIEWPORT_WINDOW_END_Y, sScreenY + radius_outer );

		best_xl = min( best_xl, xl );
		best_xr = max( best_xr, xr );
		best_yl = min( best_yl, yl );
		best_yr = max( best_yr, yr );
		
		for ( INT32 x = xl; x <= xr; ++x )
		{
			FLOAT xdiffsquared = (FLOAT)((sScreenX - x) * (sScreenX - x));

			for ( INT32 y = yl; y <= yr; ++y )
			{
				FLOAT diff = sqrt( (FLOAT)(xdiffsquared + 4 * (sScreenY - y) * (sScreenY - y)) );

				if ( radius_inner <= diff && diff <= radius_outer )
				{
					// we alter the colour of existing pixels instead of fully replacing the colour. As a result, one can still see the map regions we draw over, which looks a lot better
					PixelAlterColour( FALSE, x, y, red, pDestBuf );

					sthdrawn = TRUE;
				}
			}
		}
	}

	// redraw background to stop weird graphic remnants remaining
	// but don't do so while scrolling, because that looks weird
	if ( !gfScrollPending && !gfScrollInertia )
	{
		// only do this once
		if ( sthdrawn )
		{
			iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, best_xl, best_yl, (INT16)(best_xr + 1), (INT16)(best_yr + 1) );

			if ( iBack != -1 )
			{
				SetBackgroundRectFilled( iBack );
			}
		}
	}

	UnLockVideoSurface( FRAME_BUFFER );
}

// Flugente: draw a circle around a gridno
// For now, we aren't using usColour, but that will likely change in the future
void DrawTraitRadius(INT32 sGridno, INT8 sLevel, INT32 sRadius, INT16 sThickness, UINT16 usColour)
{
	if (TileIsOutOfBounds(sGridno))
		return;

	if (sRadius <= 0)
		return;

	UINT32					uiDestPitchBYTES;
	UINT8*					pDestBuf;
	INT32					iBack = -1;
	INT16					sScreenX, sScreenY;

	// Get screen pos of gridno......
	GetGridNoScreenXY(sGridno, &sScreenX, &sScreenY);

	// ATE: If we are on a higher interface level, subtract....
	if (sLevel == 1)
		sScreenY -= 50;

	pDestBuf = LockVideoSurface(FRAME_BUFFER, &uiDestPitchBYTES);

	// make sure to check for these boundaries later on, and only draw inside them
	SetClippingRegionAndImageWidth(uiDestPitchBYTES, gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y);

	// we determine the biggest rectangle we have to reserve
	INT32 best_xl = 99999;
	INT32 best_xr = 0;
	INT32 best_yl = 99999;
	INT32 best_yr = 0;
	BOOLEAN sthdrawn = FALSE;

	INT16 radius_inner = sRadius - sThickness / 2;
	INT16 radius_outer = sRadius + sThickness / 2;

	// determine area of where the circle will be drawn in, take into account what part of the sector we actually see
	INT32 xl = max(gsVIEWPORT_START_X, sScreenX - radius_outer);
	INT32 xr = min(gsVIEWPORT_END_X, sScreenX + radius_outer);
	INT32 yl = max(gsVIEWPORT_WINDOW_START_Y, sScreenY - radius_outer);
	INT32 yr = min(gsVIEWPORT_WINDOW_END_Y, sScreenY + radius_outer);

	best_xl = min(best_xl, xl);
	best_xr = max(best_xr, xr);
	best_yl = min(best_yl, yl);
	best_yr = max(best_yr, yr);

	for (INT32 x = xl; x <= xr; ++x)
	{
		FLOAT xdiffsquared = (FLOAT)((sScreenX - x) * (sScreenX - x));

		for (INT32 y = yl; y <= yr; ++y)
		{
			FLOAT diff = sqrt((FLOAT)(xdiffsquared + 4 * (sScreenY - y) * (sScreenY - y)));

			if (radius_inner <= diff && diff <= radius_outer)
			{
				// we alter the colour of existing pixels instead of fully replacing the colour. As a result, one can still see the map regions we draw over, which looks a lot better
				PixelAlterColour(FALSE, x, y, usColour, pDestBuf);

				sthdrawn = TRUE;
			}
		}
	}

	// redraw background to stop weird graphic remnants remaining
	// but don't do so while scrolling, because that looks weird
	if (!gfScrollPending && !gfScrollInertia)
	{
		// only do this once
		if (sthdrawn)
		{
			iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, best_xl, best_yl, (INT16)(best_xr + 1), (INT16)(best_yr + 1));

			if (iBack != -1)
			{
				SetBackgroundRectFilled(iBack);
			}
		}
	}

	UnLockVideoSurface(FRAME_BUFFER);
}

void RenderTopmostTacticalInterface( )
{
	SOLDIERTYPE								*pSoldier;
	UINT32											cnt;
	static UINT32				uiBogTarget = 0;
	VOBJECT_DESC	VObjectDesc;
	INT16			sX, sY;
	INT16			sOffsetX, sOffsetY, sTempY_S, sTempX_S;
	INT32 usMapPos;
	ITEM_POOL					*pItemPool;


	if ( gfRerenderInterfaceFromHelpText == TRUE )
	{
		fInterfacePanelDirty = DIRTYLEVEL2;

		switch( gsCurInterfacePanel )
		{
			case SM_PANEL:
				RenderSMPanel( &fInterfacePanelDirty );
				break;

			case TEAM_PANEL:
				RenderTEAMPanel( fInterfacePanelDirty );
				break;
		}
		gfRerenderInterfaceFromHelpText = FALSE;
	}

	if ( ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if ( !(guiTacticalInterfaceFlags & INTERFACE_NORENDERBUTTONS ) )
		{
			// If we want to rederaw whole screen, dirty all buttons!
			if ( fInterfacePanelDirty == DIRTYLEVEL2 )
			{
				MarkButtonsDirty( );
			}


			RenderButtons( );
		}

		return;
	}

	if ( InItemStackPopup( ) || InSectorStackPopup( ) )
	{
		RenderItemStackPopup( ( BOOLEAN )( fInterfacePanelDirty == DIRTYLEVEL2 ) );
	}

	if( ( InKeyRingPopup() ) && ( !InItemDescriptionBox( ) ) )
	{
		RenderKeyRingPopup( ( BOOLEAN )( fInterfacePanelDirty == DIRTYLEVEL2 ) );
	}

	if ( gfInMovementMenu )
	{
		RenderMovementMenu( );
	}



	// if IN PLAN MODE AND WE HAVE TARGETS, draw black targets!
	if ( InUIPlanMode( ) )
	{
		// Zero out any planned soldiers
		for( cnt = MAX_NUM_SOLDIERS; cnt < TOTAL_SOLDIERS; cnt++ )
		{
			if ( MercPtrs[ cnt ]->bActive )
			{
				if ( MercPtrs[ cnt ]->sPlannedTargetX != -1 )
				{
					// Blit bogus target
					if ( uiBogTarget == 0 )
					{
						//Loadup cursor!
						VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
						FilenameForBPP( "CURSORS\\targblak.sti", VObjectDesc.ImageFile);
						AddVideoObject( &VObjectDesc, &uiBogTarget );
					}

					if ( GridNoOnScreen( MAPROWCOLTOPOS( ( MercPtrs[ cnt ]->sPlannedTargetY/CELL_Y_SIZE), ( MercPtrs[ cnt ]->sPlannedTargetX / CELL_X_SIZE ) ) ) )
					{
						// GET SCREEN COORDINATES
						sOffsetX = (MercPtrs[ cnt ]->sPlannedTargetX - gsRenderCenterX);
						sOffsetY = (MercPtrs[ cnt ]->sPlannedTargetY - gsRenderCenterY);

						FromCellToScreenCoordinates( sOffsetX, sOffsetY, &sTempX_S, &sTempY_S );

						sX = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) + sTempX_S;
						sY = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) + sTempY_S;

						// Adjust for offset position on screen
						sX -= gsRenderWorldOffsetX;
						sY -= gsRenderWorldOffsetY;

						sX -= 10;
						sY -= 10;

						BltVideoObjectFromIndex(	FRAME_BUFFER, uiBogTarget, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
						InvalidateRegion( sX, sY, sX + 20, sY + 20 );
					}
				}
			}
		}
	}


/*
	if ( gfUIInDeadlock )
	{
		SetFont( LARGEFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );
		gprintfdirty( 0, 300, L"OPPONENT %d DEADLOCKED - 'Q' TO DEBUG, <ALT><ENTER> END OPP TURN", gUIDeadlockedSoldier	);
		mprintf( 0, 300, L"OPPONENT %d DEADLOCKED - 'Q' TO DEBUG, <ALT><ENTER> END OPP TURN", gUIDeadlockedSoldier );

	}
*/

	// Syncronize for upcoming soldier counters
	SYNCTIMECOUNTER( );

	
	// Setup system for video overlay ( text and blitting ) Sets clipping rects, etc
	StartViewportOverlays( );

	HandleExplosionWarningAnimations( );

	RenderTopmostFlashingItems( );

	RenderTopmostMultiPurposeLocator( );

	RenderAccumulatedBurstLocations( );

	// Loop through all mercs and make go
	for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
	{
		pSoldier = MercSlots[ cnt ];

		if (pSoldier != NULL )
		{
			if ( pSoldier->ubID == gsSelectedGuy && gfUIHandleSelectionAboveGuy )
			{

			}
			else
			{
				DrawSelectedUIAboveGuy((UINT16)pSoldier->ubID);
			}

			DrawCounters( pSoldier );
		}
	}

	if ( gusSelectedSoldier != NOBODY )
	{
		DrawSelectedUIAboveGuy( gusSelectedSoldier );
	}

	if ( gfUIHandleSelectionAboveGuy && gsSelectedGuy != NOBODY )
	{
		DrawSelectedUIAboveGuy((UINT16)gsSelectedGuy );
	}

	// HEADROCK HAM 4: The NCTH indicator is now drawn to the FRAMEBUFFER instead of the MOUSEBUFFER. This is
	// due to size constraints related to drawing directly on the cursor. This indicator follows the mouse, and
	// is a mouse-cursor in everything but name. See the function itself for more details.
	if (UsingNewCTHSystem() == true && gfUICtHBar)
	{
		DrawCTHIndicator();
	}

	// FOR THE MOST PART, DISABLE INTERFACE STUFF WHEN IT'S ENEMY'S TURN
	if ( gTacticalStatus.ubCurrentTeam == gbPlayerNum )
	{
		RenderArrows( );
	}

	RenderAimCubeUI( );

	EndViewportOverlays( );
		
	RenderRubberBanding( );

	if ( !gfInItemPickupMenu && gpItemPointer == NULL )
	{
		HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
	}

	// CHECK IF OUR CURSOR IS OVER AN INV POOL
	if( GetMouseMapPos( &usMapPos) )
	{
		if ( gfUIOverItemPool )
		{
			if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
			{
				// Check if we are over an item pool
				if ( GetItemPool( gfUIOverItemPoolGridNo, &pItemPool, pSoldier->pathing.bLevel ) )
				{
					STRUCTURE					*pStructure = NULL;
					INT32 sIntTileGridNo;
					INT8							bZLevel = 0;
					INT32 sActionGridNo = usMapPos;

					// Get interactive tile...
					if ( ConditionalGetCurInteractiveTileGridNoAndStructure( &sIntTileGridNo , &pStructure, FALSE ) )
					{
						sActionGridNo = sIntTileGridNo;
					}

					bZLevel = GetLargestZLevelOfItemPool( pItemPool );

					if ( AnyItemsVisibleOnLevel( pItemPool, bZLevel ) )
					{
						DrawItemPoolList( pItemPool, gfUIOverItemPoolGridNo	, ITEMLIST_DISPLAY, bZLevel, gusMouseXPos, gusMouseYPos );

						// ATE: If over items, remove locator....
						RemoveFlashItemSlot( pItemPool );
					}
				}
				else
				{
					INT8 bCheckLevel;

					// ATE: Allow to see list if a different level....
					if ( pSoldier->pathing.bLevel == 0 )
					{
						bCheckLevel = 1;
					}
					else
					{
						bCheckLevel = 0;
					}

					// Check if we are over an item pool
					if ( GetItemPool( gfUIOverItemPoolGridNo, &pItemPool, bCheckLevel ) )
					{
						STRUCTURE					*pStructure = NULL;
						INT32 sIntTileGridNo;
						INT8							bZLevel = 0;
						INT32 sActionGridNo = usMapPos;

						// Get interactive tile...
						if ( ConditionalGetCurInteractiveTileGridNoAndStructure( &sIntTileGridNo , &pStructure, FALSE ) )
						{
							sActionGridNo = sIntTileGridNo;
						}
					
						bZLevel = GetLargestZLevelOfItemPool( pItemPool );

						if ( AnyItemsVisibleOnLevel( pItemPool, bZLevel ) )
						{
							DrawItemPoolList( pItemPool, gfUIOverItemPoolGridNo	, ITEMLIST_DISPLAY, bZLevel, gusMouseXPos, gusMouseYPos );

							// ATE: If over items, remove locator....
							RemoveFlashItemSlot( pItemPool );
						}
					}
				}
			}
		}
	}

	// Check if we should render item selection window
	if ( gCurrentUIMode == GETTINGITEM_MODE )
	{
		SetItemPickupMenuDirty( DIRTYLEVEL2 );
		// Handle item pickup will return true if it's been closed
		RenderItemPickupMenu( );
	}

	// Check if we should render item selection window
	if ( gCurrentUIMode == OPENDOOR_MENU_MODE )
	{
		RenderOpenDoorMenu( );
	}

	if ( gfInTalkPanel )
	{
		SetTalkingMenuDirty( DIRTYLEVEL2 );
		// Handle item pickup will return true if it's been closed
		RenderTalkingMenu( );
	}

	if ( gfInSectorExitMenu )
	{
		RenderSectorExitMenu( );
	}

	if( fRenderRadarScreen == TRUE )
	{
	// Render clock
/*	Here we was rendering closk and position string now i moved it to functions
 *	that render SM or TEAM panel it will be usefull when RenderTownIDString will
 *	take position where render i will fix that
 *	any questions?? joker
 */
//		RenderClock( CLOCK_X, CLOCK_Y );
//		RenderTownIDString( );
//		CreateMouseRegionForPauseOfClock( CLOCK_X, CLOCK_Y );
	}
	else
	{
		RemoveMouseRegionForPauseOfClock( );
	}
	
	if ( !(guiTacticalInterfaceFlags & INTERFACE_NORENDERBUTTONS ) )
	{
		// If we want to rederaw whole screen, dirty all buttons!
		if ( fInterfacePanelDirty == DIRTYLEVEL2 )
		{
			MarkButtonsDirty( );
		}
		
		RenderButtons( );
		RenderPausedGameBox( );
	}

	// mark all pop ups as dirty
	MarkAllBoxesAsAltered( );

	HandleShowingOfTacticalInterfaceFastHelpText( );
	HandleShadingOfLinesForAssignmentMenus( );
	DetermineBoxPositions( );
	DisplayBoxes( FRAME_BUFFER );
}



void StartViewportOverlays( )
{

	// Set Clipping Rect to be the viewscreen
	// Save old one
	memcpy( &gOldClippingRect, &ClippingRect, sizeof( gOldClippingRect ) );

	// Save old dirty clipping rect
//	memcpy( &gOldDirtyClippingRect, &ClippingRect, sizeof( gOldDirtyClippingRect ) );
	memcpy( &gOldDirtyClippingRect, &gDirtyClipRect, sizeof( gOldDirtyClippingRect ) );

	// Set bottom clipping value for blitter clipping rect
	ClippingRect.iLeft = 0;
	ClippingRect.iTop = gsVIEWPORT_WINDOW_START_Y;
	ClippingRect.iRight = SCREEN_WIDTH;
	ClippingRect.iBottom = gsVIEWPORT_WINDOW_END_Y;

	// Set values for dirty rect clipping rect
	gDirtyClipRect.iLeft = 0;
	gDirtyClipRect.iTop = gsVIEWPORT_WINDOW_START_Y;
	gDirtyClipRect.iRight = SCREEN_WIDTH;
	gDirtyClipRect.iBottom = gsVIEWPORT_WINDOW_END_Y;

	SaveFontSettings( );
	SetFontDestBuffer( FRAME_BUFFER, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, gsVIEWPORT_WINDOW_END_Y, FALSE );

}

void EndViewportOverlays( )
{
	// Reset clipping rect
	memcpy( &ClippingRect, &gOldClippingRect, sizeof( gOldClippingRect ) );
	memcpy( &gDirtyClipRect, &gOldDirtyClippingRect, sizeof( gOldDirtyClippingRect ) );

if(gDirtyClipRect.iBottom < SCREEN_HEIGHT)
	gDirtyClipRect.iBottom = SCREEN_HEIGHT;

	RestoreFontSettings( );

}

void LockTacticalInterface( )
{
	// OK, check and see if we are not locked, if so
	// 1) create a mouse region over the entrie interface panel
	// 2) set flag for use in tactical to indicate we are locked
	if ( !(guiTacticalInterfaceFlags & INTERFACE_LOCKEDLEVEL1 ) )
	{
		MSYS_DefineRegion( &gLockPanelOverlayRegion, 0, gsVIEWPORT_WINDOW_END_Y ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST,
							CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
		// Add region
		MSYS_AddRegion( &gLockPanelOverlayRegion);

		guiTacticalInterfaceFlags |= INTERFACE_LOCKEDLEVEL1;
	}
}


void UnLockTacticalInterface( )
{
	if ( (guiTacticalInterfaceFlags & INTERFACE_LOCKEDLEVEL1 ) )
	{
		// Remove region
		MSYS_RemoveRegion( &gLockPanelOverlayRegion);

		guiTacticalInterfaceFlags &= (~INTERFACE_LOCKEDLEVEL1);
	}

}

void EraseInterfaceMenus( BOOLEAN fIgnoreUIUnLock )
{
	// ATE: If we are currently talking, setup this flag so that the
	// automatic handler in handledialogue doesn't adjust the UI setting
	if ( ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) && fIgnoreUIUnLock )
	{
		gTacticalStatus.uiFlags |= IGNORE_ENGAGED_IN_CONV_UI_UNLOCK;
	}

	// Remove item pointer if one active
	CancelItemPointer( );

	ShutDownQuoteBoxIfActive( );
	PopDownMovementMenu( );
	PopDownOpenDoorMenu( );
	DeleteTalkingMenu( );
#ifdef JA2UB
	RemoveJerryMiloBrokenLaptopOverlay();
#endif
}


BOOLEAN AreWeInAUIMenu( )
{
	if ( gfInMovementMenu || gfInOpenDoorMenu || gfInItemPickupMenu || gfInSectorExitMenu || gfInTalkPanel )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


void ResetInterfaceAndUI( )
{
	// Erase menus
	EraseInterfaceMenus( FALSE );

	EraseRenderArrows( );

	EndRubberBanding( );

	//ResetMultiSelection( );

	if ( giUIMessageOverlay != -1	)
	{
		RemoveVideoOverlay( giUIMessageOverlay );
		giUIMessageOverlay = -1;
	}

	// Set UI back to movement...
	guiPendingOverrideEvent = M_ON_TERRAIN;
	HandleTacticalUI( );
}

BOOLEAN InterfaceOKForMeanwhilePopup()
{
	if ( gfSwitchPanel )
	{
		return( FALSE );
	}

	return( TRUE );
}

void PrintCounter( INT16 x, INT16 y, INT16 data, UINT16 &width, UINT8 ubForegound, UINT8 scale )
{
	CHAR16 pStr[100];
	UINT8 num = 0;
	UINT8 i;

	if( data == 0)
		return;

	switch ( scale )
	{
	case PRINT_SCALE_ASTERISK_SUPPRESSION:
		num = CalcScaleSuppression( abs(data) );
		break;
	case PRINT_SCALE_ASTERISK_DAMAGE:
		num = CalcScaleDamage( abs(data) );
		break;
	case PRINT_SCALE_ASTERISK_MORALE:
		num = CalcScaleMorale( abs(data) );
		break;
	case PRINT_SCALE_ASTERISK_AP:
		num = CalcScaleAP( abs(data) );
		break;
	case PRINT_SCALE_ASTERISK_SHOCK:
		num = CalcScaleShock( abs(data) );
		break;
	default:
		num = 1;
	}
	if ( !gGameExternalOptions.ubShowSuppressionScaleAsterisk )
		num = 1;

	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( ubForegound );
	// scale = 0 - print numbers
	// scale = 1 - print one asterisk
	// scale > 1 - print num asterisks
	if( scale > PRINT_SCALE_PLAIN_NUMBER )
	{
		swprintf( pStr, L"*" );
		for ( i = 1; i<num; i++)
			wcscat( pStr, L"*" );
	}
	else
	{
		swprintf( pStr, L"%d ", data );
	}
	gprintfdirty( x+width, y, pStr );
	mprintf( x+width, y, pStr );
	width += StringPixLength ( pStr, TINYFONT1 );
	SetFontForeground( FONT_MCOLOR_WHITE );
}

UINT8 CalcScaleSuppression( INT16 data )
{
	if( data > 20 ) return 5;
	if( data > 15 ) return 4;
	if( data > 10 ) return 3;
	if( data > 5 ) return 2;
	return 1;
}

UINT8 CalcScaleDamage( INT16 data )
{
	if( data > 100 ) return 5;
	if( data > 60 ) return 4;
	if( data > 30 ) return 3;
	if( data > 10 ) return 2;
	return 1;
}

UINT8 CalcScaleMorale( INT16 data )
{
	return __min( 5, data );
}
UINT8 CalcScaleAP( INT16 data )
{
	if( data > 100 ) return 5;
	if( data > 60 ) return 4;
	if( data > 30 ) return 3;
	if( data > 10 ) return 2;
	return 1;
}
UINT8 CalcScaleShock( INT16 data )
{
	if( data > 20 ) return 5;
	if( data > 12 ) return 4;
	if( data > 7 ) return 3;
	if( data > 3 ) return 2;
	return 1;
}

void DrawCounters( SOLDIERTYPE *pSoldier )
{
	// Use world coordinates!
	INT16 sMercScreenX, sMercScreenY, sOffsetX, sOffsetY, sDamageX, sDamageY;

	// sevenfm: added for suppression counters
	INT16 sSuppressionX, sSuppressionY;				
	UINT16 widthDamage = 0;							// print damage counters one after another in one line
	UINT16 widthSuppression = 0;					// print suppression counters (not moving) one after another in one line above soldier
	BOOLEAN printSuppression = FALSE;               // determines if any value is printed above soldier (only for new suppression counters)
	UINT8 hitCount;
	CHAR16 pStr[100];

	if (    gGameExternalOptions.ubShowSuppressionCountAlt ||
		gGameExternalOptions.ubShowSuppressionCount == 2 ||
		gGameExternalOptions.ubShowShockCount == 2 ||
		gGameExternalOptions.ubShowAPCount == 2 ||
		gGameExternalOptions.ubShowMoraleCount == 2 )
		printSuppression = TRUE;

	if (!TileIsOutOfBounds(pSoldier->sGridNo) && pSoldier->bVisible != -1 )
	{
		GetSoldierScreenPos( pSoldier, &sMercScreenX, &sMercScreenY );
		GetSoldierAnimOffsets( pSoldier, &sOffsetX, &sOffsetY );
		// coordinates for suppression counter

		if ( pSoldier->ubBodyType == QUEENMONSTER )
		{
			sSuppressionX = sMercScreenX - pSoldier->sBoundingBoxOffsetX;
			sSuppressionY = sMercScreenY - pSoldier->sBoundingBoxOffsetY;
			sSuppressionX += 25;
			sSuppressionY += 10;
		}
		else
		{
			sSuppressionX = (INT16)(sMercScreenX + ( 2 * 30 / 3 ) );
			sSuppressionY = (INT16)(sMercScreenY - 5 );

			if ( sSuppressionY < gsVIEWPORT_WINDOW_START_Y )
				sSuppressionY = ( sMercScreenY - sOffsetY );
		}		
		// coordinates for damage counter
		if ( pSoldier->ubBodyType == QUEENMONSTER )
		{
			sDamageX = sMercScreenX + pSoldier->sDamageX - pSoldier->sBoundingBoxOffsetX;
			sDamageY = sMercScreenY + pSoldier->sDamageY - pSoldier->sBoundingBoxOffsetY;
			sDamageX += 25;
			sDamageY += 10;
		}
		else
		{
			sDamageX = pSoldier->sDamageX + (INT16)(sMercScreenX + ( 2 * 30 / 3 )	);
			sDamageY = pSoldier->sDamageY + (INT16)(sMercScreenY - 5 );
			sDamageX -= sOffsetX;
			sDamageY -= sOffsetY;
			// if showing suppression - move damage counter higher on screen
			if( printSuppression)
//				sDamageY -= 2*WFGetFontHeight ( TINYFONT1 );
				sDamageY -= WFGetFontHeight ( TINYFONT1 );
			if ( sSuppressionY < gsVIEWPORT_WINDOW_START_Y )
				sSuppressionY = ( sMercScreenY - sOffsetY );
		}
		// print current ubSuppressionPoints counter
		if( gGameExternalOptions.ubShowSuppressionCountAlt && pSoldier->stats.bLife >= OKLIFE )
		{                               
			if( pSoldier->ubSuppressionPoints >0 )
			{
				PrintCounter( sSuppressionX, sSuppressionY, pSoldier->ubSuppressionPoints, widthSuppression, FONT_MCOLOR_LTGRAY,
					gGameExternalOptions.ubShowSuppressionUseAsterisks ? PRINT_SCALE_ASTERISK_SUPPRESSION : PRINT_SCALE_PLAIN_NUMBER );
			}
		}

		if ( pSoldier->flags.fDisplayDamage )
		{
			// Display damage
			SetFont( TINYFONT1 );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_WHITE );

			if( pSoldier->bTeam == OUR_TEAM )
				hitCount = gGameExternalOptions.ubPlayerHitCount;
			else
				hitCount = gGameExternalOptions.ubEnemyHitCount;

			if( pSoldier->sDamage < 0 )
			{								
				// Flugente: it is possible that someone might regain negative damage as zombies can regenerate health through bleeding
				SetFontForeground( FONT_MCOLOR_LTGREEN );
				swprintf( pStr, L"+%d ", -pSoldier->sDamage );
				gprintfdirty( sDamageX, sDamageY, pStr );
				mprintf( sDamageX, sDamageY, pStr );
				widthDamage += StringPixLength ( pStr, TINYFONT1 );
				SetFontForeground( FONT_MCOLOR_WHITE );
			}
			else							// normal soldier
			{
				switch (hitCount)
				{
				case 0:				// show damage as usual
					if( pSoldier->sDamage > 0 )
					{
						//PrintCounter( sDamageX, sDamageY, pSoldier->sDamage, widthDamage, FONT_MCOLOR_WHITE, PRINT_SCALE_PLAIN_NUMBER );
						swprintf( pStr, L"-%d ", pSoldier->sDamage );
						gprintfdirty( sDamageX, sDamageY, pStr );
						mprintf( sDamageX, sDamageY, pStr );
						widthDamage += StringPixLength ( pStr, TINYFONT1 );
					}
				break;
				case 1:				// show ? indicator
					if( pSoldier->sDamage != 0 )
					{
						swprintf( pStr, L"%s ", gzHiddenHitCountStr[0] );
						gprintfdirty( sDamageX, sDamageY, pStr );
						mprintf( sDamageX, sDamageY, pStr );
						widthDamage += StringPixLength ( pStr, TINYFONT1 );
					}
					break;
				case 2:				// do not show anything
					break;
				case 3:				// show white asterisks
					PrintCounter( sDamageX, sDamageY, pSoldier->sDamage, widthDamage, FONT_MCOLOR_WHITE, PRINT_SCALE_ASTERISK_DAMAGE );
					break;
				case 4:				// show red asterisks
					PrintCounter( sDamageX, sDamageY, pSoldier->sDamage, widthDamage, FONT_MCOLOR_DKRED, PRINT_SCALE_ASTERISK_DAMAGE );
					break;
				default:
					break;
				}
				// show additional hit info
				if( gGameExternalOptions.fShowHitInfo && ( pSoldier->iLastBulletImpact > 0 || pSoldier->iLastArmourProtection ) )
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( COLOR_ORANGE );

					//swprintf( pStr, L"%d/%d ", pSoldier->iLastBulletImpact, pSoldier->iLastArmourProtection );
					swprintf( pStr, L"%d ", pSoldier->iLastArmourProtection );
					gprintfdirty( sDamageX + widthDamage, sDamageY, pStr );
					mprintf( sDamageX + widthDamage, sDamageY, pStr );
					widthDamage += StringPixLength ( pStr, TINYFONT1 );

					SetFontForeground( FONT_MCOLOR_WHITE );
			}
			}

			// do not show suppression info for dying soldier
			if ( pSoldier->stats.bLife >= OKLIFE )
			{
				// replace ubLastSuppression count on screen
				if( gGameExternalOptions.ubShowSuppressionCountAlt && gGameExternalOptions.ubShowSuppressionCount == 2 && pSoldier->ubLastSuppression > 0 )
					widthSuppression = 0;
				// display suppression from last attack
				PrintSuppressionCounter( sDamageX, sDamageY, sSuppressionX, sSuppressionY, pSoldier->ubLastSuppression, widthDamage, widthSuppression, 
										FONT_MCOLOR_LTGRAY, PRINT_SCALE_ASTERISK_SUPPRESSION, gGameExternalOptions.ubShowSuppressionCount );
				// display shock from last attack
				PrintSuppressionCounter( sDamageX, sDamageY, sSuppressionX, sSuppressionY, pSoldier->ubLastShock + pSoldier->ubLastShockFromHit, widthDamage, widthSuppression,
										FONT_MCOLOR_LTYELLOW, PRINT_SCALE_ASTERISK_SHOCK,  gGameExternalOptions.ubShowShockCount );
				// display morale hit from last attack
				PrintSuppressionCounter( sDamageX, sDamageY, sSuppressionX, sSuppressionY, pSoldier->ubLastMorale + pSoldier->ubLastMoraleFromHit, widthDamage, widthSuppression,
										FONT_MCOLOR_LTGREEN, PRINT_SCALE_ASTERISK_MORALE, gGameExternalOptions.ubShowMoraleCount );
				// display AP loss from last attack
				PrintSuppressionCounter( sDamageX, sDamageY, sSuppressionX, sSuppressionY, pSoldier->ubLastAP + pSoldier->ubLastAPFromHit, widthDamage, widthSuppression, 
										FONT_MCOLOR_LTBLUE, PRINT_SCALE_ASTERISK_AP, gGameExternalOptions.ubShowAPCount );
			}
		} 
	}
}

void PrintSuppressionCounter( INT16 x, INT16 y, INT16 sX, INT16 sY, UINT8 data, UINT16 &widthDamage, UINT16 &widthSuppression, UINT8 ubForeground, UINT8 scale, UINT8 option)
{
	if( option == 1 )  // show after damage counter
		PrintCounter( x, y, data, widthDamage, ubForeground, gGameExternalOptions.ubShowSuppressionUseAsterisks ? scale : PRINT_SCALE_PLAIN_NUMBER );
	if( option == 2 )  // show after suppression counter
		PrintCounter( sX, sY, data, widthSuppression, ubForeground, gGameExternalOptions.ubShowSuppressionUseAsterisks ? scale : PRINT_SCALE_PLAIN_NUMBER );
}
