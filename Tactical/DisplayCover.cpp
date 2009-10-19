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
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//*******	Local Defines **************************************************

struct CoverCell
{
	INT16	sGridNo;
	INT8	bCover;
	BOOLEAN fInverseColor;

	CoverCell() : sGridNo(NOWHERE), bCover(-1), fInverseColor(FALSE) {}
};

enum COVER_VALUES
{
	NO_COVER=0,
	MIN_COVER=1,
	MED_COVER=2,
	MAX_COVER=3
};

enum COVER_DRAW_MODE {
	COVER_DRAW_OFF,
	COVER_DRAW_MERC_VIEW,
	COVER_DRAW_ENEMY_VIEW
};

COVER_DRAW_MODE gubDrawMode = COVER_DRAW_OFF;

const UINT8 animArr[3] = {
	ANIM_PRONE,
	ANIM_CROUCH,
	ANIM_STAND
};

//******	Global Variables	*****************************************

// yea way too big... but some bytes more memory is cheap, as well as some extra loops
// PS: brainscells arn't cheap ;)
#define COVER_X_CELLS WORLD_COLS
#define COVER_Y_CELLS WORLD_ROWS
#define COVER_Z_CELLS 2 // roof or no roof

CoverCell gCoverViewArea[ COVER_X_CELLS ][ COVER_Y_CELLS ][ COVER_Z_CELLS ];

DWORD guiCoverNextUpdateTime = 0;

//*******	Function Prototypes ***************************************

void	SwitchCoverDrawMode();

CHAR16* GetTerrainName( const UINT8& ubTerrainType );

TileDefines GetTileCoverIndex( const INT8& bCover );

void	AddCoverObjectToWorld( const INT16& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof );
void	RemoveCoverObjectFromWorld( const INT16 sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof );

void	AddCoverObjectsToViewArea();
void	RemoveCoverObjectsFromViewArea();

void	CalculateCover();
void	CalculateCoverForSoldier( SOLDIERTYPE* pForSoldier, const INT16& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover );
void	CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT16& sTargetGridNo, const BOOLEAN& fRoof, const INT32& iSightAdjustment, INT8& bCover );

void	GetGridNoForViewPort( const UINT8& ubX, const UINT8& ubY, INT16& sGridNo );

BOOLEAN GridNoOnScreenAndAround( const INT16& sGridNo, const UINT8& ubRadius=2 );

BOOLEAN IsTheRoofVisible( const INT16& sGridNo );
BOOLEAN HasAdjTile( const UINT8& ubX, const UINT8& ubY );

//*******	Functions **************************************************

// TODO: internationalize
void SwitchCoverDrawMode()
{
	CHAR16 zDebugString[512];

	switch(gubDrawMode)
	{
	case COVER_DRAW_OFF:
		swprintf( zDebugString, gzDisplayCoverText[DC_MSG__COVER_DRAW_MERC_VIEW] );
		gubDrawMode = COVER_DRAW_MERC_VIEW;
		break;
	case COVER_DRAW_MERC_VIEW:
		swprintf( zDebugString, gzDisplayCoverText[DC_MSG__COVER_DRAW_ENEMY_VIEW] );
		gubDrawMode = COVER_DRAW_ENEMY_VIEW;
		break;
	default:
		swprintf( zDebugString, gzDisplayCoverText[DC_MSG__COVER_DRAW_OFF] );
		gubDrawMode = COVER_DRAW_OFF;
		break;
	}
	
	DisplayCover(TRUE);
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zDebugString );
}

void GetGridNoForViewPort( const UINT8& ubX, const UINT8& ubY, INT16& sGridNo )
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

void AddCoverObjectToWorld( const INT16& sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof )
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

void RemoveCoverObjectFromWorld( const INT16 sGridNo, const UINT16& usGraphic, const BOOLEAN& fRoof )
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

BOOLEAN HasAdjTile( const UINT8& ubX, const UINT8& ubY, const UINT8& ubZ )
{
	UINT8 ubTX, ubTY;

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
	UINT8 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=0; ubX<COVER_X_CELLS; ++ubX )
	{
		for ( ubY=0; ubY<COVER_Y_CELLS; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT16& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;
				BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

				if ( bCover != -1 && ( bCover != MAX_COVER || HasAdjTile( ubX, ubY, ubZ ) ) )
				{
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
	UINT8 ubX, ubY, ubZ;
	BOOLEAN fChanged = FALSE;

	for ( ubX=0; ubX<COVER_X_CELLS; ++ubX )
	{
		for ( ubY=0; ubY<COVER_Y_CELLS; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT16& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;
				BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

				if ( bCover != -1 )
				{
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
BOOLEAN GridNoOnScreenAndAround( const INT16& sGridNo, const UINT8& ubRadius )
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
	if ( gGameExternalOptions.ubCoverDisplayUpdateWait == -1 )
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
	UINT8 ubX, ubY, ubZ;
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

	const INT16& sSelectedSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;

	for ( ubX=0; ubX<COVER_X_CELLS; ++ubX )
	{
		for ( ubY=0; ubY<COVER_Y_CELLS; ++ubY )
		{
			for ( ubZ=0; ubZ<COVER_Z_CELLS; ++ubZ )
			{
				INT16& sGridNo = gCoverViewArea[ ubX ][ ubY ][ ubZ ].sGridNo;
				INT8& bCover = gCoverViewArea[ ubX ][ ubY ][ ubZ ].bCover;
				BOOLEAN& fInverseColor = gCoverViewArea[ ubX ][ ubY ][ ubZ ].fInverseColor;

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

				// if (BehindSoldier( x, y ))


				if ( gubDrawMode == COVER_DRAW_ENEMY_VIEW ) // view of enemies against your selected merc
				{
					CalculateCoverForSoldier( pSoldier, sGridNo, ubZ, bCover );
					fInverseColor = FALSE;
				}
				else if ( gubDrawMode == COVER_DRAW_MERC_VIEW ) // single view from your merc
				{
					CalculateCoverFromSoldier( pSoldier, sGridNo, ubZ, 0, bCover );
					fInverseColor = TRUE; // we don't want to show the cover for the enemy
				}
			}
		}
	}

	AddCoverObjectsToViewArea();
}

void CalculateCoverForSoldier( SOLDIERTYPE* pSoldier, const INT16& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover )
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
		if( *pbPersOL != SEEN_CURRENTLY &&
				*pbPersOL != SEEN_THIS_TURN &&
				*pbPublOL != SEEN_CURRENTLY &&
				*pbPublOL != SEEN_THIS_TURN )
		{
			continue;			// next merc
		}

		INT16 iTileSightLimit = pOpponent->GetMaxDistanceVisible( sTargetGridNo, pSoldier->pathing.bLevel, CALC_FROM_WANTED_DIR );
		INT16 iSightAdjustment = GetSightAdjustment( pSoldier, sTargetGridNo, ANIM_PRONE ) - GetSightAdjustmentThroughStance( ANIM_PRONE );

		CalculateCoverFromSoldier( pOpponent, sTargetGridNo, fRoof, iSightAdjustment, bCover );

		// if we got no cover, just return, can't get any worse than that. Helldorados lets you count
		if (bCover == NO_COVER)
		{
			return;
		}
	}
}

void CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT16& sTargetGridNo, const BOOLEAN& fRoof, const INT32& iSightAdjustment, INT8& bCover )
{
	UINT16 usSightLimit = pFromSoldier->GetMaxDistanceVisible(sTargetGridNo, (INT8)fRoof, CALC_FROM_WANTED_DIR);

	bCover = MAX_COVER;
	for ( int i = 0; i<sizeof(animArr); ++i )
	{
		const UINT8& ubStance = animArr[i];

		UINT16 usAdjustedSight = usSightLimit + usSightLimit * ( GetSightAdjustmentThroughStance(ubStance) + iSightAdjustment )/100;

		if ( SoldierToVirtualSoldierLineOfSightTest( pFromSoldier, sTargetGridNo, (INT8) fRoof, ubStance, FALSE, usAdjustedSight ) != 0 )
		{
			bCover = i;
			break; // we go from prone to stand, if soldier can see someone crouching, he can also see someone standing
		}
	}
}

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	UINT16 usRange=0;
	CHAR16	zOutputString[512];

	if( sTargetGridNo == NOWHERE || sTargetGridNo == 0 )
	{
		return;
	}

	{
		UINT8 ubLightLevel = LightTrueLevel(sTargetGridNo, gsInterfaceLevel);
		UINT8 ubBrightness = 100 * (SHADE_MIN - ubLightLevel)/SHADE_MIN; // percentage

		INT8 ubTerrainType = GetTerrainTypeForGrid(sTargetGridNo);

		INT8 ubCover = - GetSightAdjustment(pSoldier, sTargetGridNo);

		//display a string with cover value of current selected merc and brightness
		swprintf( zOutputString, gzDisplayCoverText[DC_MSG__COVER_INFORMATION], ubCover, GetTerrainName(ubTerrainType), ubBrightness );

		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
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

		swprintf( zOutputString, gzDisplayCoverText[DC_MSG__GUN_RANGE_INFORMATION], usRange / 10, Weapon[ pSoldier->inv[HANDPOS].usItem ].usRange / 10, uiHitChance );
		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
	}
	else
	{
		swprintf( zOutputString, gzDisplayCoverText[DC_MSG__GUN_RANGE_INFORMATION], usRange / 10, 0, 0 );

		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
	}

	//increment the display gun range counter ( just seeing how many times people use it )
	//gJa25SaveStruct.uiDisplayGunRangeCounter++;
}

BOOLEAN IsTheRoofVisible( const INT16& sGridNo )
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
