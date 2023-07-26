#include "builddefines.h"

	#include "sgp.h"
	#include "Radar Screen.h"
	#include "sysutil.h"
	#include "line.h"
	#include "renderworld.h"
	#include "lighting.h"
	#include "wcheck.h"
	#include "render dirty.h"
	#include "overhead map.h"
	#include "Squads.h"
	#include "Text.h"
	#include "Utilities.h"
	#include "Interface Control.h"
	#include "Game Clock.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Animation Data.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
extern std::vector<std::wstring> gSquadNameVector;
extern INT8	gbWorldSectorZ;
extern BOOLEAN AreInMeanwhile();
#include "connect.h"

extern BOOLEAN gfTacticalPlacementGUIActive;
extern INT32 iCurrentMapSectorZ;
extern UINT16 UI_BOTTOM_X;
extern UINT16 UI_BOTTOM_Y;
void AdjustWorldCenterFromRadarCoords( INT16 sRadarX, INT16 sRadarY );


// render the squad list
void RenderSquadList( void );

// squad list mvt + btn callback
void TacticalSquadListMvtCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TacticalSquadListBtnCallBack(MOUSE_REGION * pRegion, INT32 iReason );

// the squad list font
#define SQUAD_FONT COMPFONT

#define SQUAD_REGION_HEIGHT 2 * RADAR_WINDOW_HEIGHT
#define SQUAD_WINDOW_TM_Y RADAR_WINDOW_TM_Y + GetFontHeight( SQUAD_FONT )

// subtractor for squad list from size of radar view region height
#define SUBTRACTOR_FOR_SQUAD_LIST 0


INT16			gsRadarX;
INT16			gsRadarY;
UINT32		gusRadarImage;
BOOLEAN		fImageLoaded = FALSE;
BOOLEAN	fRenderRadarScreen = TRUE;
INT16			sSelectedSquadLine = -1;

// CHRISL: Moved radar coords from header file
INT16	RADAR_WINDOW_TM_X;
INT16	RADAR_WINDOW_SM_X;
INT16	RADAR_WINDOW_TM_Y;
INT16	RADAR_WINDOW_SM_Y;
INT16	RADAR_WINDOW_WIDTH;
INT16	RADAR_WINDOW_HEIGHT;
INT16	RADAR_WINDOW_STRAT_X;
INT16	RADAR_WINDOW_STRAT_Y;

BOOLEAN		gfRadarCurrentGuyFlash = FALSE;

// WANNE: Are we allowed to scroll in radar map?
// (E.g: It is not allowed to scroll, if the whole tactical map fits in the radar map) [2007-05-14]
BOOLEAN		fAllowRadarMovementHor = TRUE;
BOOLEAN		fAllowRadarMovementVer = TRUE;


MOUSE_REGION gRadarRegionSquadList[ NUMBER_OF_SQUADS ];

void InitRadarScreenCoords( )
{
	if (isWidescreenUI())
	{
		RADAR_WINDOW_STRAT_X = UI_BOTTOM_X + 1182;
		RADAR_WINDOW_STRAT_Y = UI_BOTTOM_Y + 9;
		RADAR_WINDOW_TM_X = xResOffset + (xResSize - 97) + 223;
		RADAR_WINDOW_SM_X = xResOffset + (xResSize - 97);
	}
	else
	{
		RADAR_WINDOW_STRAT_X 	= xResOffset + (xResSize - 97);
		RADAR_WINDOW_STRAT_Y 	= (SCREEN_HEIGHT - 107);
		RADAR_WINDOW_TM_X 		= xResOffset + (xResSize - 97);
		RADAR_WINDOW_SM_X 		= xResOffset + (xResSize - 97);
	}

	RADAR_WINDOW_TM_Y = (INTERFACE_START_Y + 13);
	RADAR_WINDOW_SM_Y = ((UsingNewInventorySystem() == false)) ? (INV_INTERFACE_START_Y + 33) : (INV_INTERFACE_START_Y + 116);
	RADAR_WINDOW_WIDTH = 88;
	RADAR_WINDOW_HEIGHT = 44;
}

// WANNE.RADAR: This method is called in the main menu, so the mouse region is initialized too early. We should initialize the mouse region later, when reaching
// strategy screen. Take a look at the following methods: CreateMouseRegionForPauseOfClock, RemoveMouseRegionForPauseOfClock
// We should do something similiar for the radar screen mouse region creation/removal
BOOLEAN InitRadarScreen( )
{
		// CHRISL: Move screen coord setup to it's own function
		InitRadarScreenCoords();
		// Add region for radar
		MSYS_DefineRegion( &gRadarRegion, RADAR_WINDOW_TM_X, RADAR_WINDOW_TM_Y, 
											RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH,
											RADAR_WINDOW_TM_Y + RADAR_WINDOW_HEIGHT,
											MSYS_PRIORITY_HIGHEST, 0,
											RadarRegionMoveCallback,
											RadarRegionButtonCallback );

		// Add region
		MSYS_AddRegion( &gRadarRegion );

		//disable the radar map
		MSYS_DisableRegion(&gRadarRegion);

		gsRadarX = RADAR_WINDOW_TM_X;
		gsRadarY = RADAR_WINDOW_TM_Y;

		return( TRUE );
}

BOOLEAN LoadRadarScreenBitmap(CHAR8 * aFilename )
{
	VOBJECT_DESC	VObjectDesc;
	CHAR8			zFilename[ 260 ];
	INT32			cnt;
	HVOBJECT		hVObject;

	strcpy( zFilename, aFilename );

	// If we have loaded, remove old one
	if ( fImageLoaded )
	{
		DeleteVideoObjectFromIndex( gusRadarImage );

		fImageLoaded = FALSE;
	}

/* ARM - Restriction removed Nov.29/98.	Must be able to view different radar maps from map screen while underground!
	// If we are in a cave or basement..... dont get a new one...
	if( !gfBasement && !gfCaves )
*/
	{
		// Remove extension
		for ( cnt = strlen( zFilename )-1; cnt >=0; cnt-- )
		{
			if ( zFilename[ cnt ] == '.' )
			{
				zFilename[ cnt ] = '\0';
			}
		}

		// Grab the Map image
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		sprintf( VObjectDesc.ImageFile, "RADARMAPS\\%s.STI", zFilename );

		CHECKF(AddVideoObject(&VObjectDesc, &gusRadarImage));

		fImageLoaded = TRUE;

		if( GetVideoObject( &hVObject, gusRadarImage ) )
		{
				// ATE: Add a shade table!
				hVObject->pShades[ 0 ]	= Create16BPPPaletteShaded( hVObject->pPaletteEntry, 255, 255, 255, FALSE );
				hVObject->pShades[ 1 ]	= Create16BPPPaletteShaded( hVObject->pPaletteEntry, 100, 100, 100, FALSE );
		}
	}

	// Dirty interface
	fInterfacePanelDirty = TRUE;

	return( TRUE );
}

void ClearOutRadarMapImage( void )
{
	// If we have loaded, remove old one
	if ( fImageLoaded )
	{
	DeleteVideoObjectFromIndex( gusRadarImage );
	fImageLoaded = FALSE;
	}

}

void MoveRadarScreen()
{

	// check if we are allowed to do anything?
	if( fRenderRadarScreen == FALSE )
	{
		return;
	}

	// Remove old region
	MSYS_RemoveRegion( &gRadarRegion );

	// Add new one

	// CHRISL: Reset coords
	InitRadarScreenCoords();
	// Move based on inventory panel
	if( guiCurrentScreen == MAP_SCREEN )
	{
		gsRadarX = RADAR_WINDOW_STRAT_X;
		gsRadarY = RADAR_WINDOW_STRAT_Y;
	}
	else if ( gsCurInterfacePanel == SM_PANEL )
	{
		gsRadarX = RADAR_WINDOW_SM_X;
		gsRadarY = RADAR_WINDOW_SM_Y;
	}
	else
	{
		gsRadarX = RADAR_WINDOW_TM_X;
		gsRadarY = RADAR_WINDOW_TM_Y;
	}

	// Add region for radar
	// CHRISL:
	MSYS_DefineRegion( &gRadarRegion, (UINT16) (gsRadarX), (UINT16)(gsRadarY), 
										 (UINT16) (gsRadarX + RADAR_WINDOW_WIDTH),
										 (UINT16)(gsRadarY + RADAR_WINDOW_HEIGHT),
										 MSYS_PRIORITY_HIGHEST, 0, 
										 RadarRegionMoveCallback,
										 RadarRegionButtonCallback );

	// Add region
	MSYS_AddRegion( &gRadarRegion );

}


void RadarRegionMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT16 sRadarX, sRadarY;

	// check if we are allowed to do anything?
	if( fRenderRadarScreen == FALSE )
	{
		return;
	}

	if (iReason == MSYS_CALLBACK_REASON_MOVE )
	{
		if ( pRegion->ButtonState & MSYS_LEFT_BUTTON )
		{
			// WANNE: If the tactical map fits in the whole radar map, disable scrolling in radar map. [2007-05-14]
			if (fAllowRadarMovementHor)
				// Use relative coordinates to set center of viewport
				sRadarX = pRegion->RelativeXPos - ( RADAR_WINDOW_WIDTH / 2 );
			else
				sRadarX = 0;

			if (fAllowRadarMovementVer)
				sRadarY = pRegion->RelativeYPos - ( RADAR_WINDOW_HEIGHT / 2 );
			else
				sRadarY = 0;

			if (fAllowRadarMovementHor || fAllowRadarMovementVer)
			{
				AdjustWorldCenterFromRadarCoords( sRadarX, sRadarY );
				SetRenderFlags(RENDER_FLAG_FULL);
			}
		}
	}
}

void RadarRegionButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT16 sRadarX, sRadarY;

	// check if we are allowed to do anything?
	if( fRenderRadarScreen == FALSE || gfTacticalPlacementGUIActive)
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( !InOverheadMap( ) )
		{
			// WANNE: If the tactical map fits in the whole radar map, disable scrolling in radar map. [2007-05-14]
			if (fAllowRadarMovementHor)
				// Use relative coordinates to set center of viewport
				sRadarX = pRegion->RelativeXPos - ( RADAR_WINDOW_WIDTH / 2 );
			else
				sRadarX = 0;

			if (fAllowRadarMovementVer)
				sRadarY = pRegion->RelativeYPos - ( RADAR_WINDOW_HEIGHT / 2 );
			else
				sRadarY = 0;

			if (fAllowRadarMovementHor || fAllowRadarMovementVer)
			{
				AdjustWorldCenterFromRadarCoords( sRadarX, sRadarY );

				SetRenderFlags(RENDER_FLAG_FULL);
			}
		}
		else
		{
			KillOverheadMap();
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		if ( !InOverheadMap( ) )
		{
			if (fDisplayOverheadMap == TRUE)
				GoIntoOverheadMap( );
		}
		else
		{
			if (fDisplayOverheadMap == TRUE)
				KillOverheadMap();
		}
	}
}



void RenderRadarScreen( )
{
	INT16 sRadarTLX, sRadarTLY;
	INT16 sRadarBRX, sRadarBRY;
	INT16 sRadarCX, sRadarCY;
	INT32 iItemNumber = 0;

	INT16	sX_S, sY_S;
	INT16 sScreenCenterX, sScreenCenterY;
	INT16 sDistToCenterY, sDistToCenterX;
	INT16 sTopLeftWorldX, sTopLeftWorldY;
	INT16 sTopRightWorldX, sTopRightWorldY;
	INT16 sBottomLeftWorldX, sBottomLeftWorldY;
	INT16 sBottomRightWorldX, sBottomRightWorldY;


	SOLDIERTYPE						*pSoldier;

	INT16	sXSoldPos, sYSoldPos, sXSoldScreen, sYSoldScreen, sXSoldRadar, sYSoldRadar;

	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16										usLineColor;
	UINT32											cnt;
	INT16											sHeight, sWidth, sX, sY;
	INT32											iCounter = 0;


	// create / destroy squad list regions as nessacary
	CreateDestroyMouseRegionsForSquadList( );

	// check if we are allowed to do anything?
	if( fRenderRadarScreen == FALSE )
	{
		RenderSquadList( );
		return;
	}
#ifdef JA2UB
//JA25 No meanwhiles
#else
	if( AreInMeanwhile( ) == TRUE )
	{
		// in a meanwhile, don't render any map
		fImageLoaded = FALSE;
	}
#endif
	if ( fInterfacePanelDirty == DIRTYLEVEL2 && fImageLoaded )
	{
		// Set to default
		SetObjectHandleShade( gusRadarImage, 0 );

		//If night time and on surface, darken the radarmap.

		// WANNE - MP: Only darken the radarmap in single player mode
		if( NightTime() && !is_networked )//hayden
		{
			if( guiCurrentScreen == MAP_SCREEN && !iCurrentMapSectorZ ||
					guiCurrentScreen == GAME_SCREEN && !gbWorldSectorZ )
			{
				SetObjectHandleShade( gusRadarImage, 1 );
			}
		}

		// CHRISL:
		BltVideoObjectFromIndex(  guiSAVEBUFFER, gusRadarImage, 0, gsRadarX, gsRadarY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// FIRST DELETE WHAT'S THERE
	// CHRISL:
	RestoreExternBackgroundRect( gsRadarX, gsRadarY, RADAR_WINDOW_WIDTH + 1 , RADAR_WINDOW_HEIGHT + 1 );

	// Determine scale factors

	// Find the diustance from render center to true world center
	sDistToCenterX = gsRenderCenterX - gCenterWorldX;
	sDistToCenterY = gsRenderCenterY - gCenterWorldY;

	// From render center in world coords, convert to render center in "screen" coords
	FromCellToScreenCoordinates( sDistToCenterX , sDistToCenterY, &sScreenCenterX, &sScreenCenterY );

	// Subtract screen center
	sScreenCenterX += gsCX;
	sScreenCenterY += gsCY;

	// Get corners in screen coords
	// TOP LEFT
	sX_S = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 );
	sY_S = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 );

	sTopLeftWorldX = sScreenCenterX	- sX_S;
	sTopLeftWorldY = sScreenCenterY	- sY_S;

	sTopRightWorldX = sScreenCenterX	+ sX_S;
	sTopRightWorldY = sScreenCenterY	- sY_S;

	sBottomLeftWorldX = sScreenCenterX	- sX_S;
	sBottomLeftWorldY = sScreenCenterY	+ sY_S;

	sBottomRightWorldX = sScreenCenterX	+ sX_S;
	sBottomRightWorldY = sScreenCenterY	+ sY_S;


	// Determine radar coordinates
	sRadarCX	= (INT16)( gsCX * gdScaleX );
	sRadarCY	= (INT16)( gsCY * gdScaleY );


	sWidth		= ( RADAR_WINDOW_WIDTH );
	sHeight		= ( RADAR_WINDOW_HEIGHT );
	sX			= gsRadarX;
	sY			= gsRadarY;


	sRadarTLX = (INT16)( ( sTopLeftWorldX * gdScaleX ) - sRadarCX	+ sX + ( sWidth /2 ) );
	sRadarTLY = (INT16)( ( sTopLeftWorldY * gdScaleY ) - sRadarCY + gsRadarY + ( sHeight /2 ) );
	sRadarBRX = (INT16)( ( sBottomRightWorldX * gdScaleX ) - sRadarCX + sX + ( sWidth /2 ) );
	sRadarBRY = (INT16)( ( sBottomRightWorldY * gdScaleY ) - sRadarCY + gsRadarY + ( sHeight /2 ) );

	// WANNE: Check if the tactical map fits in the whole radar map. If so, disable scrolling in radar map. [2007-05-14]
	fAllowRadarMovementHor = TRUE;
	fAllowRadarMovementVer = TRUE;
	if ((sRadarBRX - sRadarTLX) > RADAR_WINDOW_WIDTH)
	{
		fAllowRadarMovementHor = FALSE;
	}

	if ((sRadarBRY - sRadarTLY) > RADAR_WINDOW_HEIGHT)
	{
		fAllowRadarMovementVer = FALSE;
	}


	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	
	// CHRISL:
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, gsRadarX, gsRadarY, ( gsRadarX + RADAR_WINDOW_WIDTH - 1 ), ( gsRadarY + RADAR_WINDOW_HEIGHT - 1 ) );

	if( !( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if(gbPixelDepth==16)
		{
			// WANNE: Correct radar rectangle size if it is too large to fit in radar screen [2007-05-14]
			if (fAllowRadarMovementHor == FALSE)
			{
				sRadarTLX = RADAR_WINDOW_TM_X;
				sRadarBRX = RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH;
			}

			if (fAllowRadarMovementVer == FALSE)
			{
				sRadarTLY = RADAR_WINDOW_TM_Y;
				sRadarBRY = RADAR_WINDOW_TM_Y + RADAR_WINDOW_HEIGHT;
			}

			usLineColor = Get16BPPColor( FROMRGB( 0, 255, 0 ) );
			RectangleDraw( TRUE, sRadarTLX, sRadarTLY, sRadarBRX, sRadarBRY - 1, usLineColor, pDestBuf );
		}
		else if(gbPixelDepth==8)
		{
			// DB Need to change this to a color from the 8-but standard palette
			usLineColor = COLOR_GREEN;
			RectangleDraw8( TRUE, sRadarTLX + 1, sRadarTLY + 1, sRadarBRX + 1, sRadarBRY + 1, usLineColor, pDestBuf );
		}
	}

	// Cycle fFlash variable
	if ( COUNTERDONE( RADAR_MAP_BLINK ) )
	{
		RESETCOUNTER( RADAR_MAP_BLINK );

		gfRadarCurrentGuyFlash = !gfRadarCurrentGuyFlash;
	}

	if( ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && ( fShowMapInventoryPool == TRUE ) )
	{
		INT32 iNumberOfItems = 0;

		iNumberOfItems = GetTotalNumberOfItems( );

		for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
		{

			iItemNumber = iCounter + iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT;
			// stolen item
			if( ( pInventoryPoolList[ iItemNumber ].object.exists() == false )||( pInventoryPoolList[ iItemNumber ].sGridNo == 0 ) )
			{
				// yep, continue on
				continue;
			}

			ConvertGridNoToXY( pInventoryPoolList[ iItemNumber ].sGridNo, &sXSoldPos, &sYSoldPos );
			GetWorldXYAbsoluteScreenXY( sXSoldPos, sYSoldPos, &sXSoldScreen, &sYSoldScreen );

			// get radar x and y postion
			sXSoldRadar = (INT16)( sXSoldScreen * gdScaleX );
			sYSoldRadar = (INT16)( sYSoldScreen * gdScaleY );


			// Add starting relative to interface
			sXSoldRadar += RADAR_WINDOW_TM_X;
			sYSoldRadar += gsRadarY;

			// if we are in 16 bit mode....kind of redundant
			if(gbPixelDepth==16)
			{
				if( ( fFlashHighLightInventoryItemOnradarMap ) )
				{
					usLineColor = Get16BPPColor( FROMRGB(	0,	255,	0 ) );

				}
				else
				{
					// DB Need to add a radar color for 8-bit
					usLineColor = Get16BPPColor( FROMRGB(	255,	255,	255 ) );
				}

				if( iCurrentlyHighLightedItem == iCounter )
				{
					RectangleDraw( TRUE, sXSoldRadar, sYSoldRadar, sXSoldRadar + 1, sYSoldRadar + 1, usLineColor, pDestBuf );
				}
			}
		}
	}

	if( !( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		// RE-RENDER RADAR
		for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
		{
			pSoldier = MercSlots[ cnt ];

			if ( pSoldier != NULL )
			{
				// Don't place guys in radar until visible!
				if ( pSoldier->bVisible == -1 && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS) && !(pSoldier->ubMiscSoldierFlags & SOLDIER_MISC_XRAYED) )
				{
#ifdef ENABLE_MP_FRIENDLY_PLAYERS_SHARE_SAME_FOV
					continue;// ie dont render
#else
					if(is_networked && pSoldier->bSide==0)
					{
					}
					else
					{
						continue;// ie dont render
					}
#endif
				}

				// Don't render guys if they are dead!
				if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
				{
					continue;
				}

				// Don't render crows
				if ( pSoldier->ubBodyType == CROW )
				{
					continue;
				}

				// Get FULL screen coordinate for guy's position
				// Getxy from gridno
				ConvertGridNoToXY( pSoldier->sGridNo, &sXSoldPos, &sYSoldPos );
				GetWorldXYAbsoluteScreenXY( sXSoldPos, sYSoldPos, &sXSoldScreen, &sYSoldScreen );

				sXSoldRadar = (INT16)( sXSoldScreen * gdScaleX );
				sYSoldRadar = (INT16)( sYSoldScreen * gdScaleY );

				if ( !SoldierOnVisibleWorldTile( pSoldier ) )
				{
					continue;
				}

				// Add starting relative to interface
				sXSoldRadar += gsRadarX;
				sYSoldRadar += gsRadarY;
				
				if(gbPixelDepth==16)
				{
					// DB Need to add a radar color for 8-bit

					// Are we a selected guy?
					if ( pSoldier->ubID == gusSelectedSoldier )
					{
						if ( gfRadarCurrentGuyFlash )
						{
							usLineColor = 0;
						}
						else
						{
							// If on roof, make darker....
							if ( pSoldier->pathing.bLevel > 0 )
							{
								usLineColor = Get16BPPColor( FROMRGB( 150, 150, 0 ) );
							}
							else
							{
								usLineColor = Get16BPPColor( gTacticalStatus.Team[ pSoldier->bTeam ].RadarColor );
							}
						}
					}
					else
					{
						usLineColor = Get16BPPColor( gTacticalStatus.Team[ pSoldier->bTeam ].RadarColor );
						
						if ( pSoldier->bTeam == CIV_TEAM )
						{
							// Override civ team with red if hostile...
							if ( pSoldier->bSide != gbPlayerNum && !pSoldier->aiData.bNeutral )
								usLineColor = Get16BPPColor( FROMRGB( 255, 0, 0 ) );
							// if uncovered, different colour (so the player doesn't have to search for us)
							else if ( gGameExternalOptions.fKnownNPCsUseDifferentColour && pSoldier->ubProfile != NO_PROFILE && !zHiddenNames[pSoldier->ubProfile].Hidden )
								usLineColor = Get16BPPColor( FROMRGB( 0, 0, 255 ) );
						}

						// Flugente 18-04-15: observed an odd bug: if we play with a release build and see a creature for the first time, their overhead/radar map pins do not have the correct colour.
						// Bizarrely enough, the issue seems dependent on the colour value (pink, RGB: 255/0/255) itself.
						// Saving and reloading solves the issue, but I am not sure why. As a fix we now use a slightly dampened pink.
						if ( pSoldier->bTeam == CREATURE_TEAM )
						{
							usLineColor = Get16BPPColor( FROMRGB( 247, 0, 247 ) );
						}

						// Flugente: if we are a (still covert) enemy assassin, colour us like militia, so that the player wont notice us
						if ( pSoldier->usSoldierFlagMask & SOLDIER_ASSASSIN && pSoldier->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER )
							usLineColor = Get16BPPColor( gTacticalStatus.Team[ MILITIA_TEAM ].RadarColor );

						// Render different color if an enemy and he's unconscious
						if ( pSoldier->bTeam != gbPlayerNum && pSoldier->stats.bLife < OKLIFE )
						{
							usLineColor = Get16BPPColor( FROMRGB( 128, 128, 128 ) );
						}

						// If on roof, make darker....
						if ( pSoldier->bTeam == gbPlayerNum && pSoldier->pathing.bLevel > 0 )
						{
							usLineColor = Get16BPPColor( FROMRGB( 150, 150, 0 ) );
						}
					}

					RectangleDraw( TRUE, sXSoldRadar, sYSoldRadar, sXSoldRadar+1, sYSoldRadar+1, usLineColor, pDestBuf );
				}
				else if(gbPixelDepth==8)
				{
					// DB Need to change this to a color from the 8-but standard palette
					usLineColor = COLOR_BLUE;
					RectangleDraw8( TRUE, sXSoldRadar, sYSoldRadar, sXSoldRadar+1, sYSoldRadar+1, usLineColor, pDestBuf );
				}
			}
		}
	}

	UnLockVideoSurface( FRAME_BUFFER );
	
	if( ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && ( fShowMapInventoryPool == TRUE ) )
	{
		// CHRISL:
		InvalidateRegion( gsRadarX, gsRadarY,
										gsRadarX + RADAR_WINDOW_WIDTH,
										gsRadarY + RADAR_WINDOW_HEIGHT );
	}
}

void AdjustWorldCenterFromRadarCoords( INT16 sRadarX, INT16 sRadarY )
{
	INT16 sScreenX, sScreenY;
	INT16	sTempX_W, sTempY_W;
	INT16 sNewCenterWorldX, sNewCenterWorldY;
	INT16 sNumXSteps, sNumYSteps;

	// Use radar scale values to get screen values, then convert ot map values, rounding to nearest middle tile
	sScreenX = (INT16) ( sRadarX / gdScaleX );
	sScreenY = (INT16) ( sRadarY / gdScaleY );

	// Adjust to viewport start!
	sScreenX -= ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 );
	sScreenY -= ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 );

	//Make sure these coordinates are multiples of scroll steps
	sNumXSteps = sScreenX	/ SCROLL_X_STEP;
	sNumYSteps = sScreenY / SCROLL_Y_STEP;

	sScreenX = ( sNumXSteps * SCROLL_X_STEP );
	sScreenY = ( sNumYSteps * SCROLL_Y_STEP );

	// Adjust back
	sScreenX += ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X	) /2 );
	sScreenY += ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y	) /2 );

	// Subtract world center
	//sScreenX += gsCX;
	//sScreenY += gsCY;

	// Convert these into world coordinates
	FromScreenToCellCoordinates( sScreenX, sScreenY, &sTempX_W, &sTempY_W );

	// Adjust these to world center
	sNewCenterWorldX = (INT16)(gCenterWorldX + sTempX_W);
	sNewCenterWorldY = (INT16)(gCenterWorldY + sTempY_W);

	SetRenderCenter( sNewCenterWorldX, sNewCenterWorldY );

}

void DisableRadarScreenRender( void )
{
	fRenderRadarScreen = FALSE;
	return;
}

void EnableRadarScreenRender( void )
{
	fRenderRadarScreen = TRUE;
	return;
}

void ToggleRadarScreenRender( void )
{
	fRenderRadarScreen = ! fRenderRadarScreen;
	return;
}

BOOLEAN CreateDestroyMouseRegionsForSquadList( void )
{
	// will check the state of renderradarscreen flag and decide if we need to create mouse regions for
	static BOOLEAN fCreated = FALSE;
	INT16 sCounter = 0;
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT hHandle;
	UINT32 uiHandle;

	if( ( fRenderRadarScreen == FALSE ) && ( fCreated == FALSE ) )
	{
		// create regions
		// load graphics
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\squadpanel.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &uiHandle));

		GetVideoObject(&hHandle, uiHandle);
		INT32 xCoord = (xResOffset + xResSize - 102 - 1);
		if (isWidescreenUI())
		{
			xCoord += 224;
		}
		BltVideoObject( guiSAVEBUFFER, hHandle, 0, xCoord, gsVIEWPORT_END_Y, VO_BLT_SRCTRANSPARENCY,NULL );
		RestoreExternBackgroundRect(xCoord, gsVIEWPORT_END_Y, 102,( INT16 ) ( SCREEN_HEIGHT - gsVIEWPORT_END_Y ) );

		for( sCounter = 0; sCounter < NUMBER_OF_SQUADS; sCounter++ )
		{
			// run through list of squads and place appropriatly
			if( sCounter < NUMBER_OF_SQUADS / 2 )
			{
				// left half of list
				// CHRISL:
				MSYS_DefineRegion( &gRadarRegionSquadList[ sCounter ], RADAR_WINDOW_TM_X , ( INT16 )( SQUAD_WINDOW_TM_Y + ( sCounter * (  ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / ( NUMBER_OF_SQUADS / 2 ) ) ) ), RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH / 2 - 1, ( INT16 )( SQUAD_WINDOW_TM_Y + ( ( sCounter + 1 ) * ( ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / ( NUMBER_OF_SQUADS / 2 ) ) ) ) ,MSYS_PRIORITY_HIGHEST,
							0, TacticalSquadListMvtCallback, TacticalSquadListBtnCallBack );
			}
			else
			{

				// right half of list
				// CHRISL:
				MSYS_DefineRegion( &gRadarRegionSquadList[ sCounter ], RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH / 2, ( INT16 )( SQUAD_WINDOW_TM_Y + ( ( sCounter - ( NUMBER_OF_SQUADS / 2) ) * ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH  - 1, ( INT16 )( SQUAD_WINDOW_TM_Y + ( ( ( sCounter + 1 ) - ( NUMBER_OF_SQUADS / 2) )* ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), MSYS_PRIORITY_HIGHEST,
						0, TacticalSquadListMvtCallback, TacticalSquadListBtnCallBack );
			}

			// set user data
			MSYS_SetRegionUserData( &gRadarRegionSquadList[ sCounter ],0,sCounter);

		}

		DeleteVideoObjectFromIndex( uiHandle );

		// reset the highlighted line
		sSelectedSquadLine = -1;

		// set fact regions are created
		fCreated = TRUE;
	}
	else if( ( fRenderRadarScreen == TRUE ) && ( fCreated == TRUE ) )
	{
		// destroy regions

		for( sCounter = 0; sCounter < NUMBER_OF_SQUADS; sCounter++ )
		{
		MSYS_RemoveRegion( &gRadarRegionSquadList[ sCounter ] );
		}

		// set fact regions are destroyed
		fCreated = FALSE;


		if ( guiCurrentScreen == GAME_SCREEN )
		{
			// dirty region
			fInterfacePanelDirty = DIRTYLEVEL2;

			MarkButtonsDirty( );

			// re render region
			RenderTacticalInterface( );

			RenderButtons( );

			// if game is paused, then render paused game text
			RenderPausedGameBox( );
		}

	}

	return( TRUE );
}


void RenderSquadList( void )
{
	// show list of squads
	INT16 sCounter = 0;
	INT16 sX, sY;
	CHAR16 sString[40];

	// clear region
	// CHRISL:
	RestoreExternBackgroundRect( gsRadarX, gsRadarY, RADAR_WINDOW_WIDTH , SQUAD_REGION_HEIGHT );

	// fill area
	// CHRISL:
	ColorFillVideoSurfaceArea( FRAME_BUFFER, RADAR_WINDOW_TM_X, RADAR_WINDOW_TM_Y, RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH, RADAR_WINDOW_TM_Y + SQUAD_REGION_HEIGHT , Get16BPPColor( FROMRGB(0,0,0) ) );

	// set font
	SetFont( SQUAD_FONT );

	for( sCounter = 0; sCounter < NUMBER_OF_SQUADS; sCounter++ )
	{
		// run through list of squads and place appropriatly
			if( sCounter < NUMBER_OF_SQUADS / 2 )
			{
				// CHRISL:
				if ( gGameExternalOptions.fUseXMLSquadNames && sCounter < gSquadNameVector.size() )
				{
					swprintf( sString, L"%s", gSquadNameVector[sCounter].c_str() );

					FindFontCenterCoordinates( RADAR_WINDOW_TM_X, (INT16)( SQUAD_WINDOW_TM_Y + ( sCounter * ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), RADAR_WINDOW_WIDTH / 2 - 1, (INT16)( ( ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), sString, SQUAD_FONT, &sX, &sY );
				}
				else
					FindFontCenterCoordinates( RADAR_WINDOW_TM_X , ( INT16 )( SQUAD_WINDOW_TM_Y + ( sCounter * ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), RADAR_WINDOW_WIDTH / 2 - 1, ( INT16 )( (  ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ) , pSquadMenuStrings[ sCounter ] , SQUAD_FONT, &sX, &sY);
			}
			else
			{
				// CHRISL:
				if ( gGameExternalOptions.fUseXMLSquadNames && sCounter < gSquadNameVector.size() )
				{
					swprintf( sString, L"%s", gSquadNameVector[sCounter].c_str() );

					FindFontCenterCoordinates( RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH / 2, (INT16)( SQUAD_WINDOW_TM_Y + ( ( sCounter - ( NUMBER_OF_SQUADS / 2 ) ) * ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), RADAR_WINDOW_WIDTH / 2 - 1, (INT16)( ( ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), sString, SQUAD_FONT, &sX, &sY );
				}
				else
					FindFontCenterCoordinates(RADAR_WINDOW_TM_X + RADAR_WINDOW_WIDTH / 2, ( INT16 )( SQUAD_WINDOW_TM_Y + ( ( sCounter - ( NUMBER_OF_SQUADS / 2) ) * ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), RADAR_WINDOW_WIDTH / 2 - 1, ( INT16 )(   ( ( 2 * ( SQUAD_REGION_HEIGHT - SUBTRACTOR_FOR_SQUAD_LIST ) / NUMBER_OF_SQUADS ) ) ), pSquadMenuStrings[ sCounter ] , SQUAD_FONT, &sX, &sY);
			}

			// highlight line?
			if( sSelectedSquadLine == sCounter)
			{
				SetFontForeground( FONT_WHITE );
			}
			else
			{
				if( IsSquadOnCurrentTacticalMap( ( INT32 ) sCounter ) == TRUE )
				{
					if( CurrentSquad( ) == ( INT32 ) sCounter )
					{
						SetFontForeground( FONT_LTGREEN );
					}
					else
					{
						SetFontForeground(	FONT_DKGREEN);
					}
				}
				else
				{
					SetFontForeground( FONT_BLACK );
				}
			}

			SetFontBackground( FONT_BLACK );

			if( sCounter < NUMBER_OF_SQUADS / 2 )
			{
				sX = RADAR_WINDOW_TM_X + 2; 
			}
			else
			{
				sX = RADAR_WINDOW_TM_X + ( RADAR_WINDOW_WIDTH / 2 ) - 2;
			}

			if ( gGameExternalOptions.fUseXMLSquadNames && sCounter < gSquadNameVector.size() )
			{ 
				swprintf( sString, L"%s", gSquadNameVector[sCounter].c_str() );

				mprintf( sX, sY , sString );
			}
			else
				mprintf( sX, sY , pSquadMenuStrings[ sCounter ]);
	}
}

void TacticalSquadListMvtCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iValue = -1;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( IsSquadOnCurrentTacticalMap( iValue ) == TRUE )
		{
			sSelectedSquadLine = ( INT16 )iValue;
		}
	}
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		sSelectedSquadLine = -1;
	}

	return;

}


void TacticalSquadListBtnCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for team list info region
	INT32 iValue = 0;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// find out if this squad is valid and on this map..if so, set as selected
		if( IsSquadOnCurrentTacticalMap( iValue ) == TRUE )
		{
			// ok, squad is here, set as selected
			SetCurrentSquad( iValue, FALSE );

			// stop showing
			fRenderRadarScreen = TRUE;
		}
	}

	return;
}
