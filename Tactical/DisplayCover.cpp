#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
	#include "DisplayCover.h"
	#include "Interface.h"
	#include "opplist.h"
	#include "_Ja25Englishtext.h"
	//#include "Ja25 Strategic Ai.h"
#else
#include "builddefines.h"
#include "Types.h"
#include "Isometric Utils.h"
//#include "Soldier Control.h"
#include "Overhead.h"
#include "displaycover.h"
#include "Font Control.h"
#include "_Ja25EnglishText.h"
#include "message.h"
#include "GameSettings.h"
#include "renderworld.h"
#include "Interface.h"
#include "Debug.h"
#include "PATHAI.H"
#include "worldman.h"
#include "opplist.h"
#include "los.h"
#include "weapons.h"
#include "Game Clock.h"
#include "Animation Control.h"
#include "lighting.h"
#include "Text.h"
#include "strategicmap.h"
#include "Render Fun.h"
// HEADROCK HAM B2.7: Allow calling a CTH approximation function for the CTH display ("F" key)
#include "UI Cursors.h"
#include "soldier profile type.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//*******	Local Defines **************************************************


struct CoverCell
{
	INT32	sGridNo;
	INT8	bOverlayType;

	CoverCell( ) : sGridNo( NOWHERE ), bOverlayType( INVALID_COVER ) {}
};

const UINT8 animArr[3] = {
	ANIM_PRONE,
	ANIM_CROUCH,
	ANIM_STAND
};

// yea way too big... but some bytes more memory is cheap
#define COVER_X_CELLS WORLD_COLS_MAX
#define COVER_Y_CELLS WORLD_ROWS_MAX
#define COVER_Z_CELLS 2 // roof or no roof

//******	Local Variables	*********************************************

INT16 gsMinCellX, gsMinCellY, gsMaxCellX, gsMaxCellY = -1;

CoverCell gCoverViewArea[ COVER_X_CELLS ][ COVER_Y_CELLS ][ COVER_Z_CELLS ];

DWORD guiCoverNextUpdateTime = 0;

COVER_DRAW_MODE gubDrawMode = COVER_DRAW_OFF;

MINES_DRAW_MODE gubDrawModeMine = MINES_DRAW_OFF;	// Flugente: mines display

TRAIT_DRAW_MODE gubDrawModeTrait = TRAIT_DRAW_OFF;	// Flugente: mines display

TRACKER_DRAW_MODE gubDrawModeTracker = TRACKER_DRAW_OFF;	// Flugente: tracker display

//*******	Local Function Prototypes ***********************************

CHAR16* GetTerrainName( const UINT8& ubTerrainType );

// anv: additional tile properties
CHAR16* GetDetailedTerrainName( ADDITIONAL_TILE_PROPERTIES_VALUES zGivenTileProperties );

void	AddCoverObjectToWorld( const INT32& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof, BOOLEAN fNightTime );
void	RemoveCoverObjectFromWorld( const INT32 sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof );

void	AddCoverObjectsToViewArea();
void	RemoveCoverObjectsFromViewArea();

void	CalculateCover();
void	CalculateMines();
void	CalculateTraitRange();
void	CalculateTrackerRange();

void	GetGridNoForViewPort( const INT32& ubX, const INT32& ubY, INT32& sGridNo );

BOOLEAN GridNoOnScreenAndAround( const INT32& sGridNo, const UINT8& ubRadius=2 );

BOOLEAN IsTheRoofVisible( const INT32& sGridNo );
BOOLEAN HasAdjTile( const INT32& ubX, const INT32& ubY );

// resets the overlay modes so that everything will be reset at the NEXT display cycle
void ResetOverlayModes()
{
	gubDrawMode = COVER_DRAW_OFF;
	gubDrawModeMine = MINES_DRAW_OFF;
	gubDrawModeTrait = TRAIT_DRAW_OFF;
	gubDrawModeTracker = TRACKER_DRAW_OFF;
}

TileDefines GetOverlayIndex( const INT8& bOverlayType )
{
	switch ( bOverlayType )
	{
	case NO_COVER:

	case NO_SEE:

	case MINES_ALL:
	case MINE_BOMB:
	case MINES_NET_1:
	case MINES_LVL_4:

	case TRACKS_BLOOD:
		return SPECIALTILE_COVER_1; // red

	case MIN_COVER:

	case MIN_SEE:

	case MINE_BOMB_AND_WIRE:
	case MINES_NET_2:
	case MINES_LVL_3:

	case TRAIT_2:

	case TRACKS_VERYOLD:
		return SPECIALTILE_COVER_2; // orange

	case MED_COVER:

	case MED_SEE:

	case MINE_WIRE:
	case MINES_NET_3:
	case MINES_LVL_2:

	case TRAIT_ALL:

	case TRACKS_OLD:
		return SPECIALTILE_COVER_3; // yellow

	case MAX_COVER:

	case MAX_SEE:

	case MINES_NET_4:
	case MINES_LVL_1:

	case TRAIT_1:

	case TRACKS_RECENT:
		return SPECIALTILE_COVER_5; // green

	case MAX_MINES:
	case MAX_TRAIT:
	default:
		return SPECIALTILE_COVER_4; // light green, can be used to denote that you just don't know.
	}
}

//*******	Functions **************************************************

///BEGIN key binding functions
void SwitchToMercView()
{
	if (gubDrawMode == COVER_DRAW_MERC_VIEW)
		return;

	gubDrawMode = COVER_DRAW_MERC_VIEW;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzDisplayCoverText[DC_MSG__COVER_DRAW_MERC_VIEW]);
	DisplayCover(TRUE);
}

void SwitchToEnemyView()
{
	if (gubDrawMode == COVER_DRAW_ENEMY_VIEW)
		return;

	gubDrawMode = COVER_DRAW_ENEMY_VIEW;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzDisplayCoverText[DC_MSG__COVER_DRAW_ENEMY_VIEW]);
	DisplayCover(TRUE);
}

void SwitchViewOff()
{
	if (gubDrawMode == COVER_DRAW_OFF)
		return;

	gubDrawMode = COVER_DRAW_OFF;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzDisplayCoverText[DC_MSG__COVER_DRAW_OFF]);
	DisplayCover(TRUE);
}

void ToggleEnemyView()
{
	if (gubDrawMode == COVER_DRAW_ENEMY_VIEW) {
		SwitchViewOff();
	} else {
		SwitchToEnemyView();
	}
}

void ToggleMercView()
{
	if (gubDrawMode == COVER_DRAW_MERC_VIEW) {
		SwitchViewOff();
	} else {
		SwitchToMercView();
	}
}
///END key binding functions

void GetGridNoForViewPort( const INT32& ubX, const INT32& ubY, INT32& sGridNo )
{
	sGridNo = MAPROWCOLTOPOS( ubY, ubX );
}

CHAR16* GetTerrainName( const UINT8& ubTerrainType )
{
	switch(ubTerrainType) {
		case LOW_GRASS:
		case HIGH_GRASS:
			return gzDisplayCoverText[DC_TTI__WOOD];
		case FLAT_FLOOR:
		case PAVED_ROAD:
			return gzDisplayCoverText[DC_TTI__URBAN];
		case DIRT_ROAD:
		case TRAIN_TRACKS:
			return gzDisplayCoverText[DC_TTI__DESERT];
		case FLAT_GROUND:
			return gzDisplayCoverText[DC_TTI__WOOD_AND_DESERT];
		default:
			return gzDisplayCoverText[DC_TTI__UNKNOWN];
	}
}

CHAR16* GetDetailedTerrainName( ADDITIONAL_TILE_PROPERTIES_VALUES zGivenTileProperties )
{
	if(zGivenTileProperties.bWoodCamoAffinity >= 50)
	{
		if(zGivenTileProperties.bDesertCamoAffinity >= 50)
		{
			return gzDisplayCoverText[DC_TTI__WOOD_AND_DESERT];
		}
		else
		{
			if(zGivenTileProperties.bUrbanCamoAffinity >= 50)
			{
				return gzDisplayCoverText[DC_TTI__WOOD_AND_URBAN];
			}
			else
			{
				if(zGivenTileProperties.bSnowCamoAffinity >= 50)
				{
					return gzDisplayCoverText[DC_TTI__WOOD_AND_SNOW];
				}
				else
				{
					return gzDisplayCoverText[DC_TTI__WOOD];
				}
			}
		}
	}
	else if(zGivenTileProperties.bDesertCamoAffinity >= 50)
	{
		if(zGivenTileProperties.bUrbanCamoAffinity >= 50)
		{
			return gzDisplayCoverText[DC_TTI__DESERT_AND_URBAN];
		}
		else
		{
			if(zGivenTileProperties.bSnowCamoAffinity >= 50)
			{
				return gzDisplayCoverText[DC_TTI__DESERT_AND_SNOW];
			}
			else
			{
				return gzDisplayCoverText[DC_TTI__DESERT];
			}
		}
	}
	else if(zGivenTileProperties.bUrbanCamoAffinity >= 50)
	{
		if(zGivenTileProperties.bSnowCamoAffinity >= 50)
		{
			return gzDisplayCoverText[DC_TTI__URBAN_AND_SNOW];
		}
		else
		{
			return gzDisplayCoverText[DC_TTI__URBAN];
		}
	}
	else if(zGivenTileProperties.bSnowCamoAffinity >= 50)
	{
		return gzDisplayCoverText[DC_TTI__SNOW];
	}
	else
	{
		return gzDisplayCoverText[DC_TTI__UNKNOWN];
	}
}

void AddCoverObjectToWorld( const INT32& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof, BOOLEAN fNightTime )
{
	LEVELNODE *pNode;

	if( fRoof )
	{
		AddOnRoofToHead( sGridNo, usGraphic );
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
	}
	else
	{
		AddObjectToHead( sGridNo, usGraphic );
		pNode = gpWorldLevelData[ sGridNo ].pObjectHead;
	}

	pNode->uiFlags |= LEVELNODE_REVEAL;

	if ( fNightTime )
	{
		pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
		pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
	}
}

void RemoveCoverObjectFromWorld( const INT32 sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof )
{
	if( fRoof )
	{
		RemoveOnRoof( sGridNo, usGraphic );
	}
	else
	{
		RemoveObject( sGridNo, usGraphic );
	}
}

BOOLEAN HasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ )
{
	INT32 ubTX, ubTY;

	for ( ubTX = ubX-1; ubTX <= ubX+1; ++ubTX )
	{
		if ( ubTX < 0 || ubTX > WORLD_COLS )
		{
			continue;
		}

		for ( ubTY = ubY-1; ubTY <= ubY+1; ++ubTY )
		{
			if ( ubTY < 0 || ubTY > WORLD_ROWS )
			{
				continue;
			}
			
			if ( gCoverViewArea[ubTX][ubTY][ubZ].bOverlayType > INVALID_COVER && gCoverViewArea[ubTX][ubTY][ubZ].bOverlayType != MAX_COVER && gCoverViewArea[ubTX][ubTY][ubZ].bOverlayType != NO_SEE )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void AddCoverObjectsToViewArea()
{
	if (gsMaxCellY == -1)
		return;

	register INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	BOOLEAN fNightTime = NightTime( );
	
	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;
				
				if ( bOverlayType != INVALID_COVER && ((bOverlayType != MAX_COVER && bOverlayType != NO_SEE) || HasAdjTile( ubX, ubY, ubZ )) )
				{
					INT32& sGridNo = gCoverViewArea[ubX][ubY][ubZ].sGridNo;
					
					TileDefines tile = GetOverlayIndex( bOverlayType );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN)ubZ, fNightTime );
					fChanged = TRUE;
				}
			}
		}
	}

	// Re-render the scene!
	if ( fChanged )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

void RemoveCoverObjectsFromViewArea()
{
	if (gsMaxCellY == -1)
	{
		return;
	}

	register INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				if ( bOverlayType != INVALID_COVER )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
					
					TileDefines tile = GetOverlayIndex( bOverlayType );
					RemoveCoverObjectFromWorld( sGridNo, tile, (BOOLEAN) ubZ );
					bOverlayType = INVALID_COVER;
					fChanged = TRUE;
				}
			}
		}
	}
	
	// Re-render the scene!
	if ( fChanged )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

// ubRadius in times of y or x cell sizes
BOOLEAN GridNoOnScreenAndAround( const INT32& sGridNo, const UINT8& ubRadius )
{
	INT16 sNewCenterWorldX, sNewCenterWorldY;
	INT16 sWorldX;
	INT16 sWorldY;

	UINT16 usXDiff = ubRadius*CELL_X_SIZE;
	UINT16 usYDiff = ubRadius*CELL_Y_SIZE;

	ConvertGridNoToXY( sGridNo, &sNewCenterWorldX, &sNewCenterWorldY );

	// Get screen coordinates for current position of soldier
	GetWorldXYAbsoluteScreenXY( (INT16)(sNewCenterWorldX), (INT16)(sNewCenterWorldY), &sWorldX, &sWorldY);

	if (    sWorldX >= ( gsTopLeftWorldX		- usXDiff )
		 && sWorldX <= ( gsBottomRightWorldX	+ usXDiff )
		 &&	sWorldY >= ( gsTopLeftWorldY		- usYDiff + gsVIEWPORT_WINDOW_START_Y )
		 && sWorldY <= ( gsBottomRightWorldY	+ usYDiff ) )
	{
		return( TRUE );
	}
	return( FALSE );
}

void DisplayCover( BOOLEAN forceUpdate )
{
	if ( gGameExternalOptions.ubCoverDisplayUpdateWait == (UINT16)-1 )
		return;

	if ( forceUpdate || ( !gfScrollPending && !gfScrollInertia && GetTickCount() > guiCoverNextUpdateTime ) )
	{
		if ( gubDrawMode != COVER_DRAW_OFF )
			CalculateCover();
		else if ( gubDrawModeMine != MINES_DRAW_OFF )
			CalculateMines( );
		else if ( gubDrawModeTrait != TRAIT_DRAW_OFF )
			CalculateTraitRange( );
		else //if ( gubDrawModeTracker != TRAIT_DRAW_OFF )
			CalculateTrackerRange( );

		guiCoverNextUpdateTime = GetTickCount() + gGameExternalOptions.ubCoverDisplayUpdateWait;
	}
}

void CalculateCover()
{
	register INT32 ubX, ubY;
	register INT8 ubZ;
	SOLDIERTYPE* pSoldier;

	RemoveCoverObjectsFromViewArea();

	if( gubDrawMode == COVER_DRAW_OFF )
	{
		return;
	}

	if( gusSelectedSoldier == NOBODY )
	{
		return;
	}
	
	GetSoldier( &pSoldier, gusSelectedSoldier );
	
	INT16 usTmp;
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMaxCellY );
	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				
				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if( !GridNoOnScreenAndAround( sGridNo, 2 ) )
					continue;
								
				if ( IsTheRoofVisible( sGridNo ) )
				{
					// do not show stuff on ground if roof is shown
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				else
				{
					// do not show stuff on roofs if ground is shown
					if ( ubZ == I_ROOF_LEVEL )
						continue;
				}

				if ( !NewOKDestination( pSoldier, sGridNo, false, ubZ ) )
					continue;

				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				if ( gubDrawMode == COVER_DRAW_ENEMY_VIEW ) // view of enemies against your selected merc
				{
					// reset cover value
					bOverlayType = MAX_COVER;
					CalculateCoverForSoldier( pSoldier, sGridNo, ubZ, bOverlayType );
				}
				else if ( gubDrawMode == COVER_DRAW_MERC_VIEW )
				{
					// reset cover value
					bOverlayType = MAX_COVER;
					if ( gTacticalStatus.fAtLeastOneGuyOnMultiSelect ) // view of selected mercs
					{
						// OK, loop through all guys who are 'multi-selected' and
						INT32 cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;
						for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++cnt, ++pSoldier )
						{
							if ( pSoldier->bActive && pSoldier->bInSector )
							{
								if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
								{
									CalculateCoverFromSoldier( pSoldier, sGridNo, ubZ, bOverlayType );

									// if the tile is already NO_COVER, there's no need to continue
									if ( NO_COVER == bOverlayType )
										break;
								}
							}
						}
					}
					else // single view from your merc
						CalculateCoverFromSoldier( pSoldier, sGridNo, ubZ, bOverlayType );

					// we use different enums for our merc's sight to avoid confusing inverse sight
					bOverlayType = MAX_SEE - bOverlayType;
				}
			}
		}
	}

	AddCoverObjectsToViewArea();
}

void CalculateCoverForSoldier( SOLDIERTYPE* pSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bOverlayType )
{
	UINT32		uiLoop;
	SOLDIERTYPE *pOpponent;
	INT8		*pbPersOL;
	INT8		*pbPublOL;

	//loop through all the enemies and determine the cover
	for (uiLoop = 0; uiLoop<guiNumMercSlots; ++uiLoop)
	{
		pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, dead, unconscious
		if (!pOpponent || pOpponent->stats.bLife < OKLIFE)
		{
			continue;			// next merc
		}

		// if this man is neutral / on the same side, he's not an opponent
 		if( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
		{
			continue;			// next merc
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpponent->ubID;
		pbPublOL = gbPublicOpplist[ OUR_TEAM ] + pOpponent->ubID;

		// if this opponent is unknown personally and publicly
		if( *pbPersOL != SEEN_CURRENTLY && *pbPublOL != SEEN_CURRENTLY )
		{
			continue;			// next merc
		}

		CalculateCoverFromSoldier( pOpponent, sTargetGridNo, fRoof, bOverlayType, pSoldier );

		// if we got no cover, just return, can't get any worse than that. Helldorados lets you count
		if ( bOverlayType == NO_COVER )
		{
			return;
		}
	}
}

void CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bOverlayType, SOLDIERTYPE* pToSoldier )
{
	UINT16 usSightLimit = pFromSoldier->GetMaxDistanceVisible(sTargetGridNo, (INT8)fRoof, CALC_FROM_WANTED_DIR);

	for ( int i = 0; i<sizeof(animArr); ++i )
	{
		const UINT8& ubStance = animArr[i];

		INT32 usAdjustedSight;

		if (pToSoldier == NULL) {
			usAdjustedSight = usSightLimit;
		} else {
			usAdjustedSight = usSightLimit + usSightLimit * GetSightAdjustment( pToSoldier, sTargetGridNo, (INT8) fRoof, ubStance ) /100;
		}

		if ( SoldierToVirtualSoldierLineOfSightTest( pFromSoldier, sTargetGridNo, (INT8) fRoof, ubStance, FALSE, usAdjustedSight ) != 0 )
		{
			if ( bOverlayType > i ) bOverlayType = i;
			break; // we go from prone to stand, if soldier can see someone crouching, he can also see someone standing
		}
	}
}

BOOLEAN CanSoldierSeeFloor( SOLDIERTYPE* pSoldier, INT32 sGridNo, INT8 bLevel )
{
	if ( !pSoldier )
		return FALSE;

	UINT16 usSightLimit = pSoldier->GetMaxDistanceVisible( sGridNo, bLevel, CALC_FROM_WANTED_DIR );
	
	if ( SoldierToVirtualSoldierLineOfSightTest( pSoldier, sGridNo, bLevel, ANIM_PRONE, FALSE, usSightLimit ) != 0 )
		return TRUE;

	return FALSE;
}

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo )
{
	UINT16 usRange=0;
	CHAR16	zOutputString[512];
	UINT8	title = (UsingNewCTHSystem() == true ? DC_MSG__NCTH_GUN_RANGE_INFORMATION : DC_MSG__GUN_RANGE_INFORMATION);

	if( sTargetGridNo == NOWHERE || sTargetGridNo == 0 )
	{
		return;
	}

	{
		UINT8 ubLightLevel = LightTrueLevel(sTargetGridNo, gsInterfaceLevel);
		UINT8 ubBrightness = 100 - 100 * (ubLightLevel-SHADE_MAX)/(SHADE_MIN-SHADE_MAX); // percentage
		
		UINT8 ubTerrainType = NO_TERRAIN; 

		// anv: additional tile properties
		ADDITIONAL_TILE_PROPERTIES_VALUES zGivenTileProperties;
		memset(&zGivenTileProperties,0,sizeof(zGivenTileProperties));
		if(gGameExternalOptions.fAdditionalTileProperties)
		{
			zGivenTileProperties = GetAllAdditonalTilePropertiesForGrid(sTargetGridNo, gsInterfaceLevel);
		}
		else
		{
			ubTerrainType = GetTerrainTypeForGrid(sTargetGridNo, gsInterfaceLevel);
		}

		INT8 ubCover = - GetSightAdjustment(pSoldier, sTargetGridNo, gsInterfaceLevel);

		//display a string with cover value of current selected merc and brightness
		//swprintf( zOutputString, gzDisplayCoverText[DC_MSG__COVER_INFORMATION], ubCover, GetTerrainName(ubTerrainType), ubBrightness );

		//Display the msg
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );

		if(gGameExternalOptions.fAdditionalTileProperties)
		{
			if(!gGameExternalOptions.fCoverTooltipDetailedTileProperties)
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
					gzDisplayCoverText[DC_MSG__COVER_INFORMATION],
					ubCover, GetDetailedTerrainName(zGivenTileProperties), ubBrightness );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
					gzDisplayCoverText[DC_MSG__COVER_INFORMATION_WITH_DETAILED_CAMO],
					ubCover, ubBrightness );

				UINT8 ubApplicableProperties = 0;
				swprintf( zOutputString, L"" );
				if(zGivenTileProperties.bWoodCamoAffinity > 0)
				{
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__WOOD]);
					swprintf( zOutputString + wcslen(zOutputString), L": %d/100", zGivenTileProperties.bWoodCamoAffinity);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bDesertCamoAffinity > 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__DESERT]);
					swprintf( zOutputString + wcslen(zOutputString), L": %d/100", zGivenTileProperties.bDesertCamoAffinity);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bUrbanCamoAffinity > 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__URBAN]);
					swprintf( zOutputString + wcslen(zOutputString), L": %d/100", zGivenTileProperties.bUrbanCamoAffinity);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bSnowCamoAffinity > 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__SNOW]);
					swprintf( zOutputString + wcslen(zOutputString), L": %d/100", zGivenTileProperties.bSnowCamoAffinity);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bSoundModifier != 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__DETAILED_SOUND]);
					if(zGivenTileProperties.bSoundModifier > 0)
						swprintf( zOutputString + wcslen(zOutputString), L": +%d", zGivenTileProperties.bSoundModifier);
					else
						swprintf( zOutputString + wcslen(zOutputString), L": %d", zGivenTileProperties.bSoundModifier);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bStealthDifficultyModifer != 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__DETAILED_STEALTH]);
					if(zGivenTileProperties.bStealthDifficultyModifer > 0)
						swprintf( zOutputString + wcslen(zOutputString), L": +%d/100", zGivenTileProperties.bStealthDifficultyModifer);
					else
						swprintf( zOutputString + wcslen(zOutputString), L": %d/100", zGivenTileProperties.bStealthDifficultyModifer);
					ubApplicableProperties++;
				}
				if(zGivenTileProperties.bTrapBonus != 0)
				{
					if(ubApplicableProperties)
						swprintf( zOutputString + wcslen(zOutputString), L", ");
					swprintf( zOutputString + wcslen(zOutputString), gzDisplayCoverText[DC_TTI__DETAILED_TRAP_LEVEL]);
					if(zGivenTileProperties.bTrapBonus > 0)
						swprintf( zOutputString + wcslen(zOutputString), L": +%d", zGivenTileProperties.bTrapBonus);
					else
						swprintf( zOutputString + wcslen(zOutputString), L": %d", zGivenTileProperties.bTrapBonus);
					ubApplicableProperties++;
				}
				if( wcslen(zOutputString) > 0 )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
			}
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
				gzDisplayCoverText[DC_MSG__COVER_INFORMATION],
				ubCover, GetTerrainName(ubTerrainType), ubBrightness );
		}
	}

	//Get the range to the target location
	usRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo );

	//if the soldier has a weapon in hand, display gun range and chance to hit
	if( WeaponInHand( pSoldier ) )
	{
		UINT32 uiHitChance;
		//AXP 30.03.2007: Fix CtH calculation for first shot after changing aim level (roof/ground)
		INT8 bTempTargetLevel = pSoldier->bTargetLevel;
		pSoldier->bTargetLevel = (INT8)gsInterfaceLevel;
		uiHitChance = CalcChanceToHitGun( pSoldier, sTargetGridNo, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
		// HEADROCK HAM B2.7: CTH approximation?
		if (gGameExternalOptions.fApproximateCTH)
		{	
			uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
		}
		pSoldier->bTargetLevel = bTempTargetLevel;

		// HEADROCK HAM 3.6: Calculate Gun Range using formula.
		// Flugente: we might be equipped with an underbarrel gun....
		OBJECTTYPE* pObjhand = pSoldier->GetUsedWeapon(&pSoldier->inv[HANDPOS]);
		UINT16 usGunRange = GunRange(pObjhand, pSoldier ); // SANDRO - added argument

		swprintf( zOutputString, gzDisplayCoverText[title], usRange / 10, usGunRange / 10, uiHitChance );
		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
	}
	else
	{
		swprintf( zOutputString, gzDisplayCoverText[title], usRange / 10, 0, 0 );

		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
	}

	//increment the display gun range counter ( just seeing how many times people use it )
	//gJa25SaveStruct.uiDisplayGunRangeCounter++;
}

BOOLEAN IsTheRoofVisible( const INT32& sGridNo )
{
	if( FlatRoofAboveGridNo( sGridNo ) )
	{
		if( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED )
		{
			if( gTacticalStatus.uiFlags & SHOW_ALL_ROOFS )
				return( TRUE );
			else
				return( FALSE );
		}
		else
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


// ----------------------------- Mines display after this ----------------------------------------
// added by Flugente

//*******	Local Function Prototypes ***********************************

void	AddMinesObjectsToViewArea();

void	DetermineMineDisplayInTile( INT32 sGridNo, INT8 bLevel, INT8& bOverlayType, BOOLEAN fWithMineDetector = FALSE );

//*******	Functions **************************************************

///BEGIN key binding functions
void SwitchToTrapNetworkView()
{
	if (gubDrawModeMine == MINES_DRAW_PLAYERTEAM_NETWORKS)
		return;

	gubDrawModeMine = MINES_DRAW_PLAYERTEAM_NETWORKS;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network");
	DisplayCover( TRUE );
}

void SwitchToHostileTrapsView()
{
	if (gubDrawModeMine == MINES_DRAW_DETECT_ENEMY)
		return;

	gubDrawModeMine = MINES_DRAW_DETECT_ENEMY;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display nearby traps");
	DisplayCover( TRUE );
}

void SwitchMineViewOff()
{
	if (gubDrawModeMine == MINES_DRAW_OFF)
		return;

	gubDrawModeMine = MINES_DRAW_OFF;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Turning off trap display");
	DisplayCover( TRUE );
}

void ToggleHostileTrapsView()
{
	if (gubDrawModeMine == MINES_DRAW_DETECT_ENEMY) {
		SwitchMineViewOff();
	} else {
		SwitchToHostileTrapsView();
	}
}

void ToggleTrapNetworkView()
{
	SwitchMinesDrawModeForNetworks();
}

void SwitchMinesDrawModeForNetworks()
{
	switch ( gubDrawModeMine )
	{
		case MINES_DRAW_OFF:
		case MINES_DRAW_DETECT_ENEMY:
			gubDrawModeMine = MINES_DRAW_PLAYERTEAM_NETWORKS;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network");
			break;
		case MINES_DRAW_PLAYERTEAM_NETWORKS:
			gubDrawModeMine = MINES_DRAW_NETWORKCOLOURING;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network colouring");
			break;
		case MINES_DRAW_NETWORKCOLOURING:
			gubDrawModeMine = MINES_DRAW_NET_A;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network A");
			break;
		case MINES_DRAW_NET_A:
			gubDrawModeMine = MINES_DRAW_NET_B;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network B");
			break;
		case MINES_DRAW_NET_B:
			gubDrawModeMine = MINES_DRAW_NET_C;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network C");
			break;
		case MINES_DRAW_NET_C:
			gubDrawModeMine = MINES_DRAW_NET_D;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network D");
			break;
		case MINES_DRAW_NET_D:
		case MINES_DRAW_MAX:
		default:
			gubDrawModeMine = MINES_DRAW_OFF;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Turning off trap display");
			break;
	}

	DisplayCover( TRUE );
}
///END key binding functions

void AddMinesObjectsToViewArea()
{
	if (gsMaxCellY == -1)
		return;
	
	register INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	BOOLEAN fNightTime = NightTime( );

	BOOLEAN fSearchAdjTile = TRUE;
	// no search for adjacent tiles when looking at a specific network (we have only 4 colours, need them all :-)
	if ( gubDrawModeMine == MINES_DRAW_NETWORKCOLOURING || gubDrawModeMine == MINES_DRAW_NET_A || gubDrawModeMine == MINES_DRAW_NET_B || gubDrawModeMine == MINES_DRAW_NET_C || gubDrawModeMine == MINES_DRAW_NET_D )
		fSearchAdjTile = FALSE;

	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				if ( bOverlayType != INVALID_COVER && (bOverlayType != MAX_MINES || (fSearchAdjTile && HasAdjTile( ubX, ubY, ubZ ))) )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;

					TileDefines tile = GetOverlayIndex( bOverlayType );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN)ubZ, fNightTime );
					fChanged = TRUE;
				}
			}
		}
	}

	// Re-render the scene!
	if ( fChanged )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

void CalculateMines()
{
	INT32 ubX, ubY;
	INT8  ubZ;
	SOLDIERTYPE* pSoldier;
			
	RemoveCoverObjectsFromViewArea( );

	if( gubDrawModeMine == MINES_DRAW_OFF )
	{
		return;
	}

	if ( gusSelectedSoldier == NOBODY || !GetSoldier( &pSoldier, gusSelectedSoldier ) || !pSoldier->bInSector )
		return;

	// if we want to detect hostile mines and we have an metal detector in our hands, allow seeking
	BOOLEAN fWithMineDetector = FALSE;
	if ( pSoldier && gubDrawModeMine == MINES_DRAW_DETECT_ENEMY )
	{
		if ( FindMetalDetectorInHand(pSoldier) != NO_SLOT )
			fWithMineDetector = TRUE;

		// TODO: perhaps even consume batteries one day...
	}

	// if we are looking for mines via mine detector, but don't have one equipped, return, we won't detect anything
	if ( gubDrawModeMine == MINES_DRAW_DETECT_ENEMY && !fWithMineDetector )
	{
		return;
	}
	
	const INT32& sSelectedSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;
	
	INT16 usTmp;
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMaxCellY );
	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				
				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if( !GridNoOnScreenAndAround( sGridNo, 2 ) )
					continue;

				if ( IsTheRoofVisible( sGridNo ) )
				{
					// do not show stuff on ground if roof is shown
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				else
				{
					// do not show stuff on roofs if ground is shown
					if ( ubZ == I_ROOF_LEVEL )
						continue;
				}
																
				// if we are looking for hostile mines, but the tile is out of our' detectors range, skip looking for mines
				if ( gubDrawModeMine == MINES_DRAW_DETECT_ENEMY && fWithMineDetector )
				{
					if ( PythSpacesAway(sSelectedSoldierGridNo, sGridNo) > 4 )
						continue;
				}

				if ( !NewOKDestination( pSoldier, sGridNo, false, ubZ ) )
					continue;

				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				bOverlayType = INVALID_COVER;

				DetermineMineDisplayInTile( sGridNo, ubZ, bOverlayType, fWithMineDetector );
			}
		}
	}

	AddMinesObjectsToViewArea();
}


void DetermineMineDisplayInTile( INT32 sGridNo, INT8 bLevel, INT8& bOverlayType, BOOLEAN fWithMineDetector )
{
	// if there is a bomb at that grid and level, and it isn't disabled
	for (UINT32 uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; ++uiWorldBombIndex)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sGridNo && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].ubLevel == bLevel )
		{
			OBJECTTYPE* pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if (!((*pObj).fFlags & OBJECT_DISABLED_BOMB))
			{
				// we are looking for hostile mines and have got an detector equipped
				// some bombs cannot be found via metal detector
				if ( gubDrawModeMine == MINES_DRAW_DETECT_ENEMY && fWithMineDetector && !( HasItemFlag(pObj->usItem, NO_METAL_DETECTION) || HasItemFlag((*pObj)[0]->data.misc.usBombItem, NO_METAL_DETECTION) ) )
				{
					// display all mines
					bOverlayType = MINE_BOMB;
				}
				else
				{
					// look for mines from our own team
					if ( (*pObj)[0]->data.misc.ubBombOwner > 1 )
					{
						switch ( gubDrawModeMine )
						{
							case MINES_DRAW_PLAYERTEAM_NETWORKS:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// if we're already marked as MINE_BOMB, switch to MINE_BOMB_AND_WIRE
										if ( bOverlayType == MINE_BOMB )
											bOverlayType = MINE_BOMB_AND_WIRE;
										else if ( bOverlayType == MINE_BOMB_AND_WIRE )
											;
										else
										{
											// check if the tripwire has a gun attached
											BOOLEAN fgunfound = FALSE;
											attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
											for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
											{
												if ( iter->exists() && Item[iter->usItem].usItemClass == IC_GUN )
												{
													fgunfound = TRUE;
													break;
												}
											}

											if ( fgunfound )
												bOverlayType = MINE_BOMB_AND_WIRE;
											else
												bOverlayType = MINE_WIRE;
										}
									}
									else
									{
										// if we're already marked as MINE_WIRE, switch to MINE_BOMB_AND_WIRE
										if ( bOverlayType == MINE_WIRE )
											bOverlayType = MINE_BOMB_AND_WIRE;
										else
											bOverlayType = MINE_BOMB;
									}
								}
								break;

							case MINES_DRAW_NETWORKCOLOURING:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// only if its one of our networks
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER ) != 0 )
										{
											// correct network?
											if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_1 ) != 0 )
												bOverlayType = MINES_NET_1;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_2 ) != 0 )
												bOverlayType = MINES_NET_2;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_3 ) != 0 )
												bOverlayType = MINES_NET_3;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_4 ) != 0 )
												bOverlayType = MINES_NET_4;
										}
									}
								}
								break;

							case MINES_DRAW_NET_A:
							case MINES_DRAW_NET_B:
							case MINES_DRAW_NET_C:
							case MINES_DRAW_NET_D:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										UINT32 specificnet = 0;
										switch ( gubDrawModeMine )
										{
										case MINES_DRAW_NET_A: specificnet = TRIPWIRE_NETWORK_NET_1; break;
										case MINES_DRAW_NET_B: specificnet = TRIPWIRE_NETWORK_NET_2; break;
										case MINES_DRAW_NET_C: specificnet = TRIPWIRE_NETWORK_NET_3; break;
										case MINES_DRAW_NET_D: specificnet = TRIPWIRE_NETWORK_NET_4; break;
										}

										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// correct network?
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER) != 0 && (ubWireNetworkFlag & specificnet) != 0 )
										{
											bOverlayType = MINES_LVL_1;

											if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_2 ) ) != 0 )
												bOverlayType = MINES_LVL_2;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_3 ) ) != 0 )
												bOverlayType = MINES_LVL_3;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_4 ) ) != 0 )
												bOverlayType = MINES_LVL_4;
										}
									}
								}
								break;

							case MINES_DRAW_DETECT_ENEMY:
							case MINES_DRAW_MAX:
							default:
								break;
						}
					}
				}
			}
		}
	}
}

// ----------------------------- trait range display after this ----------------------------------------
// added by Flugente

//*******	Local Function Prototypes ***********************************

void	AddTraitObjectsToViewArea();

UINT32 usDisplayTrait = NO_SKILLTRAIT_NT;
INT32	sTraitgridNo = NOWHERE;

///BEGIN key binding functions
void ToggleTraitRangeView(BOOLEAN fOn)
{
	if ( fOn )
	{
		gubDrawModeTrait = TRAIT_DRAW_RANGE;
		gubDrawMode		 = COVER_DRAW_OFF;
		gubDrawModeMine  = MINES_DRAW_OFF;
		gubDrawModeTracker = TRACKER_DRAW_OFF;
	}
	else
	{
		gubDrawModeTrait = TRAIT_DRAW_OFF;
		SetTraitToDisplay( NO_SKILLTRAIT_NT );
		SetGridNoForTraitDisplay( NOWHERE );
	}

	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trait ranges");
	DisplayCover( TRUE );
}

void SetTraitToDisplay( UINT32 aTrait )
{
	usDisplayTrait = aTrait;
}

void SetGridNoForTraitDisplay( INT32 sGridNo )
{
	sTraitgridNo = sGridNo;
}


void CalculateTraitRange()
{
	INT32 ubX, ubY;
	INT8  ubZ;
	SOLDIERTYPE* pSoldier;

	RemoveCoverObjectsFromViewArea( );

	if ( gubDrawModeTrait == TRAIT_DRAW_OFF )
		return;
	
	if ( gusSelectedSoldier == NOBODY || !GetSoldier( &pSoldier, gusSelectedSoldier ) || !pSoldier->bInSector )
		return;

	UINT16 range1 = 0;
	UINT16 range2 = 0;
	switch ( usDisplayTrait )
	{
	case RADIO_OPERATOR_NT:
		{
			// we 'mark' the map position with which we called the menu, so that the player sees where he is targetting

		}
		break;

	case VARIOUSSKILLS:
		{
			range1 = gGameExternalOptions.usSpotterRange;
			range2 = gGameExternalOptions.usSpotterRange * 2;
		}
		break;

	default:
		return;
	}
				
	const INT32& sSelectedSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;
	
	INT16 usTmp;
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMaxCellY );
	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;

				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if( !GridNoOnScreenAndAround( sGridNo, 2 ) )
					continue;
				
				if ( IsTheRoofVisible( sGridNo ) )
				{
					// do not show stuff on ground if roof is shown
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				else
				{
					// do not show stuff on roofs if ground is shown
					if ( ubZ == I_ROOF_LEVEL )
						continue;
				}

				if ( !NewOKDestination( pSoldier, sGridNo, false, ubZ ) )
					continue;
				
				if ( range1 && PythSpacesAway(sSelectedSoldierGridNo, sGridNo) == range1 )
					gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRAIT_1;
				else if ( range2 && PythSpacesAway(sSelectedSoldierGridNo, sGridNo) == range2 )
					gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRAIT_2;
				// mark the gridno we are targetting
				else if ( PythSpacesAway(sTraitgridNo, sGridNo) == 1 )
					gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRAIT_1;
				else
					gCoverViewArea[ubX][ubY][ubZ].bOverlayType = MAX_TRAIT;
			}
		}
	}

	AddTraitObjectsToViewArea();
}

void AddTraitObjectsToViewArea()
{
	if (gsMaxCellY == -1)
		return;
	
	register INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	BOOLEAN fNightTime = NightTime( );

	for ( ubX=gsMinCellX; ubX<=gsMaxCellX; ++ubX )
	{
		for ( ubY=gsMinCellY; ubY<=gsMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				if ( bOverlayType == TRAIT_1 || bOverlayType == TRAIT_2 )
				{
					INT32& sGridNo = gCoverViewArea[ubX][ubY][ubZ].sGridNo;
					TileDefines tile = GetOverlayIndex( bOverlayType );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN)ubZ, fNightTime );
					fChanged = TRUE;
				}
			}
		}
	}

	// Re-render the scene!
	if ( fChanged )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

BOOLEAN TraitTileHasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ )
{
	INT32 ubTX, ubTY;

	for ( ubTX = ubX-1; ubTX <= ubX+1; ++ubTX )
	{
		if ( ubTX < 0 || ubTX > WORLD_COLS )
		{
			continue;
		}

		for ( ubTY = ubY-1; ubTY <= ubY+1; ++ubTY )
		{
			if ( ubTY < 0 || ubTY > WORLD_ROWS )
			{
				continue;
			}

			INT8& bOverlayType = gCoverViewArea[ubTX][ubTY][ubZ].bOverlayType;

			if ( bOverlayType == TRAIT_1 || bOverlayType == TRAIT_2 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}



// ----------------------------- tracker display after this ----------------------------------------
// added by Flugente

// idea: have smell and blood displayed as 'tracks'
// Hunter/Ranger-Trait and backgrounds add a tracker-strength. This sets a max range up to which we can display these tracks
// TODO: separate displays for smell and blood, or have the colour red symbolise blood and the rest be smell?
// toggle this on and off in the function menu
// if turned on, this will be displayed whenever the currently selected merc can track.

//*******	Local Function Prototypes ***********************************

void	AddTrackerObjectsToViewArea( );

void CalculateTrackerRange( )
{
	INT32 ubX, ubY;
	INT8 ubZ;
	SOLDIERTYPE* pSoldier;

	RemoveCoverObjectsFromViewArea( );

	if ( gubDrawModeTracker == TRACKER_DRAW_OFF )
		return;

	if ( gusSelectedSoldier == NOBODY || !GetSoldier( &pSoldier, gusSelectedSoldier ) ||  !pSoldier->bInSector )
		return;

	FLOAT trackerskill = (FLOAT)(NUM_SKILL_TRAITS( pSoldier, RANGER_NT ) * gSkillTraitValues.usTrackerAbility + pSoldier->GetBackgroundValue( BG_TRACKER_ABILITY )) / 100.0f;
	
	if ( trackerskill < 0.01f )
		return;

	UINT16 range = gSkillTraitValues.usTrackerMaxRange * trackerskill;

	const INT32& sSelectedSoldierGridNo = MercPtrs[gusSelectedSoldier]->sGridNo;

	INT16 usTmp;
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMaxCellY );
	for ( ubX = gsMinCellX; ubX <= gsMaxCellX; ++ubX )
	{
		for ( ubY = gsMinCellY; ubY <= gsMaxCellY; ++ubY )
		{
			for ( ubZ = 0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ubX][ubY][ubZ].sGridNo;

				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if ( !GridNoOnScreenAndAround( sGridNo, 2 ) )
					continue;
								
				if ( IsTheRoofVisible( sGridNo ) )
				{
					// do not show stuff on ground if roof is shown
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				else
				{
					// do not show stuff on roofs if ground is shown
					if ( ubZ == I_ROOF_LEVEL )
						continue;
				}

				// no tracks in water or on roads
				if ( (!ubZ && GetTerrainType( sGridNo ) == PAVED_ROAD || GetTerrainType( sGridNo ) == FLAT_FLOOR) || TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
					continue;

				if ( !NewOKDestination( pSoldier, sGridNo, false, ubZ ) )
					continue;
								
				if ( PythSpacesAway( sSelectedSoldierGridNo, sGridNo ) <= range )
				{
					if ( gpWorldLevelData[sGridNo].ubBloodInfo )
						gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRACKS_BLOOD;
					// blood can be seen on a roof, but other tracks can't
					else if ( !ubZ && gpWorldLevelData[sGridNo].ubSmellInfo )
					{
						if ( gpWorldLevelData[sGridNo].ubSmellInfo < 10 )
							gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRACKS_VERYOLD;
						else if ( gpWorldLevelData[sGridNo].ubSmellInfo < 20 )
							gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRACKS_OLD;
						else //if ( gpWorldLevelData[sGridNo].ubSmellInfo < 24 )
							gCoverViewArea[ubX][ubY][ubZ].bOverlayType = TRACKS_RECENT;
					}

					// we need to be able to see the floor
					// this check is relatively expensive, we don't want to do this unless we have to. 
					// Thus we check afterwards, and only if a display value was found we reset it
					if ( gCoverViewArea[ubX][ubY][ubZ].bOverlayType != INVALID_COVER && !CanSoldierSeeFloor( pSoldier, sGridNo, ubZ ) )
						gCoverViewArea[ubX][ubY][ubZ].bOverlayType = INVALID_COVER;
				}
			}
		}
	}

	AddTrackerObjectsToViewArea( );
}

void AddTrackerObjectsToViewArea( )
{
	if ( gsMaxCellY == -1 )
		return;

	register INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	BOOLEAN fNightTime = NightTime();

	for ( ubX = gsMinCellX; ubX <= gsMaxCellX; ++ubX )
	{
		for ( ubY = gsMinCellY; ubY <= gsMaxCellY; ++ubY )
		{
			for ( ubZ = 0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bOverlayType = gCoverViewArea[ubX][ubY][ubZ].bOverlayType;

				if ( bOverlayType == TRACKS_VERYOLD || bOverlayType == TRACKS_OLD || bOverlayType == TRACKS_RECENT || bOverlayType == TRACKS_BLOOD )
				{
					INT32& sGridNo = gCoverViewArea[ubX][ubY][ubZ].sGridNo;
					TileDefines tile = GetOverlayIndex( bOverlayType );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN)ubZ, fNightTime );
					fChanged = TRUE;
				}
			}
		}
	}

	// Re-render the scene!
	if ( fChanged )
	{
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

BOOLEAN TrackerTileHasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ )
{
	INT32 ubTX, ubTY;

	for ( ubTX = ubX - 1; ubTX <= ubX + 1; ++ubTX )
	{
		if ( ubTX < 0 || ubTX > WORLD_COLS )
		{
			continue;
		}

		for ( ubTY = ubY - 1; ubTY <= ubY + 1; ++ubTY )
		{
			if ( ubTY < 0 || ubTY > WORLD_ROWS )
			{
				continue;
			}

			INT8& bOverlayType = gCoverViewArea[ubTX][ubTY][ubZ].bOverlayType;

			if ( bOverlayType == TRACKS_VERYOLD || bOverlayType == TRACKS_OLD || bOverlayType == TRACKS_RECENT || bOverlayType == TRACKS_BLOOD )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
