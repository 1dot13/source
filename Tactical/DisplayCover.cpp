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

enum COVER_VALUES
{
	INVALID_COVER=-1,
	NO_COVER=0,
	MIN_COVER=1,
	MED_COVER=2,
	MAX_COVER=3
};

// Flugente: mines display - stuff needs to be here
enum MINES_VALUES
{
	MINES_ALL = MAX_COVER + 1,
	MINE_BOMB,
	MINE_WIRE,
	MINE_BOMB_AND_WIRE,
	MINES_NET_1,
	MINES_NET_2,
	MINES_NET_3,
	MINES_NET_4,
	MINES_LVL_1,
	MINES_LVL_2,
	MINES_LVL_3,
	MINES_LVL_4,
	MAX_MINES
};

// Flugente: trait display
enum TRAIT_VALUES
{
	TRAIT_ALL = MAX_MINES + 1,
	TRAIT_1,
	TRAIT_2,
	MAX_TRAIT
};


struct CoverCell
{
	INT32	sGridNo;
	INT8	bCover;
	BOOLEAN fInverseColor;

	INT8	bMines;
	INT8	bTrait;

	CoverCell() : sGridNo(NOWHERE), bCover(INVALID_COVER), fInverseColor(FALSE), bMines(MINES_ALL), bTrait(TRAIT_ALL) {}

};

enum COVER_DRAW_MODE {
	COVER_DRAW_OFF,
	COVER_DRAW_MERC_VIEW,
	COVER_DRAW_ENEMY_VIEW
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


// Flugente: mines display - stuff needs to be here
enum MINES_DRAW_MODE {
	MINES_DRAW_OFF,
	MINES_DRAW_DETECT_ENEMY,
	MINES_DRAW_PLAYERTEAM_NETWORKS,
	MINES_DRAW_NETWORKCOLOURING,
	MINES_DRAW_NET_A,
	MINES_DRAW_NET_B,
	MINES_DRAW_NET_C,
	MINES_DRAW_NET_D,
	MINES_DRAW_MAX
};

// Flugente: trait draw mode
enum TRAIT_DRAW_MODE {
	TRAIT_DRAW_OFF,
	TRAIT_DRAW_RANGE,
	TRAIT_DRAW_MAX
};


//******	Local Variables	*********************************************

INT16 gsMinCellX, gsMinCellY, gsMaxCellX, gsMaxCellY = -1;

CoverCell gCoverViewArea[ COVER_X_CELLS ][ COVER_Y_CELLS ][ COVER_Z_CELLS ];

DWORD guiCoverNextUpdateTime = 0;

COVER_DRAW_MODE gubDrawMode = COVER_DRAW_OFF;

MINES_DRAW_MODE gubDrawModeMine = MINES_DRAW_OFF;	// Flugente: mines display

TRAIT_DRAW_MODE gubDrawModeTrait = TRAIT_DRAW_OFF;	// Flugente: mines display


//*******	Local Function Prototypes ***********************************

CHAR16* GetTerrainName( const UINT8& ubTerrainType );

TileDefines GetTileCoverIndex( const INT8& bCover );

void	AddCoverObjectToWorld( const INT32& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof );
void	RemoveCoverObjectFromWorld( const INT32 sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof );

void	AddCoverObjectsToViewArea();
void	RemoveCoverObjectsFromViewArea();

void	RemoveMinesObjectsFromViewArea();	// added by Flugente, has to be declared here
void	RemoveTraitObjectsFromViewArea();

void	CalculateCover();
void	CalculateCoverForSoldier( SOLDIERTYPE* pForSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover );
void	CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover, SOLDIERTYPE* pToSoldier=NULL );

void	GetGridNoForViewPort( const INT32& ubX, const INT32& ubY, INT32& sGridNo );

BOOLEAN GridNoOnScreenAndAround( const INT32& sGridNo, const UINT8& ubRadius=2 );

BOOLEAN IsTheRoofVisible( const INT32& sGridNo );
BOOLEAN HasAdjTile( const INT32& ubX, const INT32& ubY );

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

void SwitchCoverDrawMode()
{
	switch(gubDrawMode)
	{
	case COVER_DRAW_OFF:
		SwitchToMercView();
		break;
	case COVER_DRAW_MERC_VIEW:
		SwitchToEnemyView();
		break;
	default:
		SwitchViewOff();
		break;
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

TileDefines GetTileCoverIndex( const INT8& bCover )
{
	switch(bCover) {
		case NO_COVER:
			return SPECIALTILE_COVER_1; // red
		case MIN_COVER:
			return SPECIALTILE_COVER_2; // orange
		case MED_COVER:
			return SPECIALTILE_COVER_3; // yellow
		case MAX_COVER:
			return SPECIALTILE_COVER_5; // green
		default:
			return SPECIALTILE_COVER_4; // light green, can be used to denote that you just don't know.
	}
}

void AddCoverObjectToWorld( const INT32& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof )
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

	if( NightTime() )
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

			INT8& bCover = gCoverViewArea[ ubTX ][ ubTY ][ ubZ ].bCover;

			if ( bCover > -1 && bCover < MAX_COVER )
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
				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;

				if ( bCover != -1 && ( bCover != MAX_COVER || HasAdjTile( ubX, ubY, ubZ ) ) )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
					BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

					TileDefines tile = GetTileCoverIndex(!fInverseColor ? bCover : MAX_COVER - bCover);
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN) ubZ );
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
				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;

				if ( bCover != -1 )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
					BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

					TileDefines tile = GetTileCoverIndex(!fInverseColor ? bCover : MAX_COVER - bCover);
					RemoveCoverObjectFromWorld( sGridNo, tile, (BOOLEAN) ubZ );
					bCover = -1;
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

void DisplayCover( const BOOLEAN& forceUpdate )
{
	if ( gGameExternalOptions.ubCoverDisplayUpdateWait == (UINT16)-1 )
	{
		return;
	}

	if ( forceUpdate || ( !gfScrollPending && !gfScrollInertia && GetTickCount() > guiCoverNextUpdateTime ) )
	{
		CalculateCover();
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

	// at we're here, we want to display cover, so remove the mines display
	if ( gubDrawModeMine != MINES_DRAW_OFF )
		RemoveMinesObjectsFromViewArea();

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
				{
					continue;
				}
								
				// do not show stuff on ground if roof is shown
				if ( IsTheRoofVisible( sGridNo ) )
				{
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				// do not show stuff on roofs if ground is shown
				else if ( ubZ == I_ROOF_LEVEL )
					continue;

				if ( !NewOKDestination( pSoldier, sGridNo, false, ubZ ) )
				{
					continue;
				}

				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;
				BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

				if ( gubDrawMode == COVER_DRAW_ENEMY_VIEW ) // view of enemies against your selected merc
				{
					// reset cover value
					bCover = MAX_COVER;
					CalculateCoverForSoldier( pSoldier, sGridNo, ubZ, bCover );
					fInverseColor = FALSE;
				}
				else if ( gubDrawMode == COVER_DRAW_MERC_VIEW )
				{
					// reset cover value
					bCover = MAX_COVER;
					if ( gTacticalStatus.fAtLeastOneGuyOnMultiSelect ) // view of selected mercs
					{
						// OK, loop through all guys who are 'multi-selected' and
						INT32	cnt;
						cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
						for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++ )
						{
							if ( pSoldier->bActive && pSoldier->bInSector )
							{
								if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
								{
									CalculateCoverFromSoldier( pSoldier, sGridNo, ubZ, bCover );
								}
							}
						}
					}
					else // single view from your merc
						CalculateCoverFromSoldier( pSoldier, sGridNo, ubZ, bCover );
					
					fInverseColor = TRUE; // we don't want to show the cover for the enemy
				}
			}
		}
	}

	AddCoverObjectsToViewArea();
}

void CalculateCoverForSoldier( SOLDIERTYPE* pSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover )
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

		CalculateCoverFromSoldier( pOpponent, sTargetGridNo, fRoof, bCover, pSoldier );

		// if we got no cover, just return, can't get any worse than that. Helldorados lets you count
		if (bCover == NO_COVER)
		{
			return;
		}
	}
}

void CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover, SOLDIERTYPE* pToSoldier )
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
			if (bCover > i) bCover = i;
			break; // we go from prone to stand, if soldier can see someone crouching, he can also see someone standing
		}
	}
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
		INT8 ubTerrainType = GetTerrainTypeForGrid(sTargetGridNo, gsInterfaceLevel);

		INT8 ubCover = - GetSightAdjustment(pSoldier, sTargetGridNo, gsInterfaceLevel);

		//display a string with cover value of current selected merc and brightness
		//swprintf( zOutputString, gzDisplayCoverText[DC_MSG__COVER_INFORMATION], ubCover, GetTerrainName(ubTerrainType), ubBrightness );

		//Display the msg
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
			gzDisplayCoverText[DC_MSG__COVER_INFORMATION],
			ubCover, GetTerrainName(ubTerrainType), ubBrightness );
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

//******	Local Variables	*********************************************

INT16 gsMineMinCellX, gsMineMinCellY, gsMineMaxCellX, gsMineMaxCellY = -1;

DWORD guiMinesNextUpdateTime = 0;

//*******	Local Function Prototypes ***********************************

TileDefines GetTileMinesIndex( const INT8& bMines );

void	AddMinesObjectsToViewArea();

void	CalculateMines();
void	DetermineMineDisplayInTile( INT32 sGridNo, INT8 bLevel, INT8& bMines, BOOLEAN fWithMineDetector = FALSE );

BOOLEAN MineTileHasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ );

//*******	Functions **************************************************

///BEGIN key binding functions
void SwitchToTrapNetworkView()
{
	if (gubDrawModeMine == MINES_DRAW_PLAYERTEAM_NETWORKS)
		return;

	gubDrawModeMine = MINES_DRAW_PLAYERTEAM_NETWORKS;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network");
	DisplayMines(TRUE);
}

void SwitchToHostileTrapsView()
{
	if (gubDrawModeMine == MINES_DRAW_DETECT_ENEMY)
		return;

	gubDrawModeMine = MINES_DRAW_DETECT_ENEMY;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display nearby traps");
	DisplayMines(TRUE);
}

void SwitchMineViewOff()
{
	if (gubDrawModeMine == MINES_DRAW_OFF)
		return;

	gubDrawModeMine = MINES_DRAW_OFF;
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Turning off trap display");
	DisplayMines(TRUE);
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
			DisplayMines(TRUE);
			break;

		case MINES_DRAW_PLAYERTEAM_NETWORKS:
			gubDrawModeMine = MINES_DRAW_NETWORKCOLOURING;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network colouring");
			DisplayMines(TRUE);
			break;
		case MINES_DRAW_NETWORKCOLOURING:
			gubDrawModeMine = MINES_DRAW_NET_A;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network A");
			DisplayMines(TRUE);
			break;
		case MINES_DRAW_NET_A:
			gubDrawModeMine = MINES_DRAW_NET_B;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network B");
			DisplayMines(TRUE);
			break;
		case MINES_DRAW_NET_B:
			gubDrawModeMine = MINES_DRAW_NET_C;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network C");
			DisplayMines(TRUE);
			break;
		case MINES_DRAW_NET_C:
			gubDrawModeMine = MINES_DRAW_NET_D;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trap network D");
			DisplayMines(TRUE);
			break;
		case MINES_DRAW_NET_D:
		case MINES_DRAW_MAX:
		default:
			gubDrawModeMine = MINES_DRAW_OFF;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Turning off trap display");
			DisplayMines(TRUE);
			break;
	}
}
///END key binding functions

TileDefines GetTileMinesIndex( const INT8& bMines )
{
	switch(bMines) 
	{
		case MINES_ALL:
		case MINE_BOMB:
		case MINES_NET_1:
		case MINES_LVL_4:
			return SPECIALTILE_COVER_1; // red
		case MINE_BOMB_AND_WIRE:
		case MINES_NET_2:
		case MINES_LVL_3:		
			return SPECIALTILE_COVER_2; // orange
		case MINE_WIRE:
		case MINES_NET_3:
		case MINES_LVL_2:	
			return SPECIALTILE_COVER_3; // yellow
		case MINES_NET_4:
		case MINES_LVL_1:
			return SPECIALTILE_COVER_4; // green
		case MAX_MINES:
		default:
			return SPECIALTILE_COVER_5; // light green, can be used to denote that you just don't know.	looks just like normal green, though...
	}
}

BOOLEAN MineTileHasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ )
{
	// no search for adjacent tiles wehn looking at a specific network (we have only 4 colours, need them all :-)
	if ( gubDrawModeMine == MINES_DRAW_NETWORKCOLOURING || gubDrawModeMine == MINES_DRAW_NET_A || gubDrawModeMine == MINES_DRAW_NET_B || gubDrawModeMine == MINES_DRAW_NET_C || gubDrawModeMine == MINES_DRAW_NET_D )
		return FALSE;

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

			INT8& bMines = gCoverViewArea[ ubTX ][ ubTY ][ ubZ ].bMines;

			if ( bMines > MINES_ALL && bMines < MAX_MINES )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void AddMinesObjectsToViewArea()
{
	if (gsMineMaxCellY == -1)
	{
		return;
	}
	INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bMines = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bMines;

				if ( bMines != MINES_ALL && ( bMines != MAX_MINES || MineTileHasAdjTile( ubX, ubY, ubZ ) ) )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;

					TileDefines tile = GetTileMinesIndex( bMines );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN) ubZ );
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

void RemoveMinesObjectsFromViewArea()
{
	if (gsMineMaxCellY == -1)
	{
		return;
	}
	INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT8& bMines = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bMines;

				if ( bMines != MINES_ALL )
				{
					INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;

					TileDefines tile = GetTileMinesIndex( bMines );
					RemoveCoverObjectFromWorld( sGridNo, tile, (BOOLEAN) ubZ );
					bMines = MINES_ALL;
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

void DisplayMines( const BOOLEAN& forceUpdate )
{
	if ( gGameExternalOptions.ubCoverDisplayUpdateWait == (UINT16)-1 )
	{
		return;
	}

	if ( forceUpdate || ( !gfScrollPending && !gfScrollInertia && GetTickCount() > guiMinesNextUpdateTime ) )
	{
		CalculateMines();
		guiMinesNextUpdateTime = GetTickCount() + gGameExternalOptions.ubCoverDisplayUpdateWait;
	}
}

void CalculateMines()
{
	INT32 ubX, ubY, ubZ;
	SOLDIERTYPE* pSoldier;
			
	RemoveMinesObjectsFromViewArea();

	if( gubDrawModeMine == MINES_DRAW_OFF )
	{
		return;
	}

	if( gusSelectedSoldier == NOBODY )
	{
		return;
	}

	// at we're here, we want to display mines, so remove the cover display
	if ( gubDrawMode != COVER_DRAW_OFF )
		RemoveCoverObjectsFromViewArea();

	GetSoldier( &pSoldier, gusSelectedSoldier );

	// if we want to detect hostile mines and we have an metal detector in our hands, allow seeking
	BOOLEAN fWithMineDetector = FALSE;
	if ( pSoldier && gubDrawModeMine == MINES_DRAW_DETECT_ENEMY )
	{
		if ( FindMetalDetectorInHand(pSoldier) )
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
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMineMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMineMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMineMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMineMaxCellY );
	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				
				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if( !GridNoOnScreenAndAround( sGridNo, 2 ) )
				{
					continue;
				}

				if ( IsTheRoofVisible( sGridNo ) )
				{
					if ( ubZ == I_GROUND_LEVEL )
						continue;
				}
				else if ( ubZ == I_ROOF_LEVEL )
				{
					continue;
				}
																
				// if we are looking for hostile mines, but the tile is out of our' detectors range, skip looking for mines
				if ( gubDrawModeMine == MINES_DRAW_DETECT_ENEMY && fWithMineDetector )
				{
					if ( PythSpacesAway(sSelectedSoldierGridNo, sGridNo) > 4 )
						continue;
				}

				if ( !NewOKDestination( pSoldier, sGridNo, false, (INT8) ubZ ) )
				{
					continue;
				}

				INT8& bMines = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bMines;

				bMines = MAX_MINES;

				DetermineMineDisplayInTile( sGridNo, ubZ, bMines, fWithMineDetector );
			}
		}
	}

	AddMinesObjectsToViewArea();
}


void DetermineMineDisplayInTile( INT32 sGridNo, INT8 bLevel, INT8& bMines, BOOLEAN fWithMineDetector )
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
					bMines = MINE_BOMB;
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
										if ( bMines == MINE_BOMB )
											bMines = MINE_BOMB_AND_WIRE;
										else if ( bMines == MINE_BOMB_AND_WIRE )
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
												bMines = MINE_BOMB_AND_WIRE;
											else
												bMines = MINE_WIRE;
										}
									}
									else
									{
										// if we're already marked as MINE_WIRE, switch to MINE_BOMB_AND_WIRE
										if ( bMines == MINE_WIRE )
											bMines = MINE_BOMB_AND_WIRE;
										else
											bMines = MINE_BOMB;
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
												bMines = MINES_NET_1;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_2 ) != 0 )
												bMines = MINES_NET_2;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_3 ) != 0 )
												bMines = MINES_NET_3;
											else if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_4 ) != 0 )
												bMines = MINES_NET_4;
										}
									}
								}
								break;

							case MINES_DRAW_NET_A:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// correct network?
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER ) != 0 && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_1 ) != 0 )
										{
											bMines = MINES_LVL_1;

											if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_2 ) ) != 0 )
												bMines = MINES_LVL_2;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_3 ) ) != 0 )
												bMines = MINES_LVL_3;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_4 ) ) != 0 )
												bMines = MINES_LVL_4;
										}
									}
								}
								break;

							case MINES_DRAW_NET_B:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// correct network?
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER ) != 0 && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_2 ) != 0 )
										{
											bMines = MINES_LVL_1;

											if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_2 ) ) != 0 )
												bMines = MINES_LVL_2;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_3 ) ) != 0 )
												bMines = MINES_LVL_3;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_4 ) ) != 0 )
												bMines = MINES_LVL_4;
										}
									}
								}
								break;

							case MINES_DRAW_NET_C:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// correct network?
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER ) != 0 && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_3 ) != 0 )
										{
											bMines = MINES_LVL_1;

											if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_2 ) ) != 0 )
												bMines = MINES_LVL_2;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_3 ) ) != 0 )
												bMines = MINES_LVL_3;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_4 ) ) != 0 )
												bMines = MINES_LVL_4;
										}
									}
								}
								break;

							case MINES_DRAW_NET_D:
								{
									if ( Item[pObj->usItem].tripwire == 1 )
									{
										// determine if wire is of the network we're searching for
										// determine this tripwire's flag
										UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

										// correct network?
										if ( (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER ) != 0 && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_4 ) != 0 )
										{
											bMines = MINES_LVL_1;

											if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_2 ) ) != 0 )
												bMines = MINES_LVL_2;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_3 ) ) != 0 )
												bMines = MINES_LVL_3;
											else if ( (ubWireNetworkFlag & ( TRIPWIRE_NETWORK_LVL_4 ) ) != 0 )
												bMines = MINES_LVL_4;
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

TileDefines GetTileTraitIndex( const INT8& bTrait );

void	AddTraitObjectsToViewArea();

void	CalculateTraitRange();

BOOLEAN TraitTileHasAdjTile( const INT32& ubX, const INT32& ubY, const INT32& ubZ );

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
	}
	else
	{
		gubDrawModeTrait = TRAIT_DRAW_OFF;
		SetTraitToDisplay( NO_SKILLTRAIT_NT );
		SetGridNoForTraitDisplay( NOWHERE );
	}

	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Display trait ranges");
	DisplayTraitRange(TRUE);
}

void SetTraitToDisplay( UINT32 aTrait )
{
	usDisplayTrait = aTrait;
}

void SetGridNoForTraitDisplay( INT32 sGridNo )
{
	sTraitgridNo = sGridNo;
}

void DisplayTraitRange( const BOOLEAN& forceUpdate )
{
	if ( gGameExternalOptions.ubCoverDisplayUpdateWait == -1 )
	{
		return;
	}

	if ( forceUpdate || ( !gfScrollPending && !gfScrollInertia && GetTickCount() > guiMinesNextUpdateTime ) )
	{
		CalculateTraitRange();
		guiMinesNextUpdateTime = GetTickCount() + gGameExternalOptions.ubCoverDisplayUpdateWait;
	}
}

void CalculateTraitRange()
{
	INT32 ubX, ubY, ubZ;
	SOLDIERTYPE* pSoldier;

	RemoveTraitObjectsFromViewArea();

	if ( gubDrawModeTrait == TRAIT_DRAW_OFF )
		return;
	
	if( gusSelectedSoldier == NOBODY || !GetSoldier( &pSoldier, gusSelectedSoldier ) )
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

	default:
		return;
	}

	//remove other displays
	if( gubDrawModeMine != MINES_DRAW_OFF )
		RemoveMinesObjectsFromViewArea();
		
	if ( gubDrawMode != COVER_DRAW_OFF )
		RemoveCoverObjectsFromViewArea();
			
	const INT32& sSelectedSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;
	
	INT16 usTmp;
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, &gsMineMinCellX, &usTmp );
	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_END_Y, &gsMineMaxCellX, &usTmp );

	GetScreenXYWorldCell( gsVIEWPORT_END_X, gsVIEWPORT_START_Y, &usTmp, &gsMineMinCellY );
	GetScreenXYWorldCell( gsVIEWPORT_START_X, gsVIEWPORT_END_Y, &usTmp, &gsMineMaxCellY );
	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;

				GetGridNoForViewPort( ubX, ubY, sGridNo );

				if( !GridNoOnScreenAndAround( sGridNo, 2 ) )
				{
					continue;
				}

				if ( !NewOKDestination( pSoldier, sGridNo, false, (INT8) ubZ ) )
				{
					continue;
				}

				// do not show stuff on roofs if ground is shown
				if ( ubZ == I_ROOF_LEVEL && !IsTheRoofVisible( sGridNo ) )
				{
					continue;
				}

				// do not show stuff on ground if roof is shown
				if ( ubZ == I_GROUND_LEVEL && IsTheRoofVisible( sGridNo ) )
				{
					continue;
				}
				
				if ( range1 && PythSpacesAway(sSelectedSoldierGridNo, sGridNo) == range1 )
					gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait = TRAIT_1;
				else if ( range2 && PythSpacesAway(sSelectedSoldierGridNo, sGridNo) == range2 )
					gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait = TRAIT_2;
				// mark the gridno we are targetting
				else if ( PythSpacesAway(sTraitgridNo, sGridNo) == 1 )
					gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait = TRAIT_1;
				else
					gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait = MAX_TRAIT;
			}
		}
	}

	AddTraitObjectsToViewArea();
}

void AddTraitObjectsToViewArea()
{
	if (gsMineMaxCellY == -1)
	{
		return;
	}
	INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				INT8& bTrait = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait;

				if ( bTrait != -1 && ( bTrait == TRAIT_1 || bTrait == TRAIT_2 ) )
				{
					TileDefines tile = GetTileTraitIndex( bTrait );
					AddCoverObjectToWorld( sGridNo, tile, (BOOLEAN) ubZ );
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

TileDefines GetTileTraitIndex( const INT8& bTrait )
{
	switch(bTrait) 
	{
		case TRAIT_1:
			return SPECIALTILE_COVER_5; // green
		case TRAIT_2:
			return SPECIALTILE_COVER_2; // orange
		case TRAIT_ALL:
			return SPECIALTILE_COVER_3; // yellow
		case MAX_TRAIT:
		default:
			return SPECIALTILE_COVER_1; // red
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

			INT8& bTrait = gCoverViewArea[ ubTX ][ ubTY ][ ubZ ].bTrait;

			if ( bTrait == TRAIT_1 || bTrait == TRAIT_2 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void RemoveTraitObjectsFromViewArea()
{
	if (gsMineMaxCellY == -1)
	{
		return;
	}
	INT32 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=gsMineMinCellX; ubX<=gsMineMaxCellX; ++ubX )
	{
		for ( ubY=gsMineMinCellY; ubY<=gsMineMaxCellY; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT32& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				INT8& bTrait = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bTrait;

				if ( bTrait != -1 || TraitTileHasAdjTile( ubX, ubY, ubZ ))
				{
					TileDefines tile = GetTileTraitIndex( bTrait );
					RemoveCoverObjectFromWorld( sGridNo, tile, (BOOLEAN) ubZ );
					bTrait = -1;
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