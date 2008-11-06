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
#endif


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//*******	Local Defines **************************************************


#define		DC_MAX_COVER_RANGE					43 //31
#define		DC__SOLDIER_VISIBLE_RANGE			43 //31

#define		DC__MIN_SIZE						4
#define		DC__MAX_SIZE						21 //11

typedef struct
{
	INT16	sGridNo;
	INT8	bCover;				//% chance that the gridno is fully covered.	ie 100 if safe, 0	is has no cover
//	BOOLEAN fRoof;
} BEST_COVER_STRUCT;


typedef struct
{
	INT16 sGridNo;
	INT8	bVisibleToSoldier;
	BOOLEAN	fRoof;
} VISIBLE_TO_SOLDIER_STRUCT;


/*
#define	DC__PRONE				(INT8)( 0x01 )
#define DC__CROUCH			(INT8)( 0x02 )
#define DC__STAND				(INT8)( 0x04 )
*/
enum
{
	DC__SEE_NO_STANCES,
	DC__SEE_1_STANCE,
	DC__SEE_2_STANCE,
	DC__SEE_3_STANCE,
};

//******	Global Variables	*****************************************


BEST_COVER_STRUCT gCoverRadius[ DC_MAX_COVER_RANGE ][ DC_MAX_COVER_RANGE ];
INT16	gsLastCoverGridNo=NOWHERE;
INT16	gsLastSoldierGridNo=NOWHERE;
INT8	gbLastStance=-1;


VISIBLE_TO_SOLDIER_STRUCT gVisibleToSoldierStruct[ DC__SOLDIER_VISIBLE_RANGE ][ DC__SOLDIER_VISIBLE_RANGE ];
INT16	gsLastVisibleToSoldierGridNo=NOWHERE;

//*******	Function Prototypes ***************************************

INT8	CalcCoverForGridNoBasedOnTeamKnownEnemies( SOLDIERTYPE *pSoldier, INT16 sTargetGridno, INT8 bStance );
void	CalculateCoverInRadiusAroundGridno( INT16 sTargetGridNo, INT8	bSearchRange );
void	AddCoverTileToEachGridNo();
void	AddCoverObjectToWorld( INT16 sGridNo, UINT16 usGraphic, BOOLEAN fRoof );
void	RemoveCoverObjectFromWorld( INT16 sGridNo, UINT16 usGraphic, BOOLEAN fRoof );
INT8	GetCurrentMercForDisplayCoverStance();
SOLDIERTYPE *GetCurrentMercForDisplayCover();

void	CalculateVisibleToSoldierAroundGridno( INT16 sGridNo, INT8 bSearchRange );
void	AddVisibleToSoldierToEachGridNo();
INT8	CalcIfSoldierCanSeeGridNo( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, BOOLEAN fRoof );
BOOLEAN IsTheRoofVisible( INT16 sGridNo );

//ppp

//*******	Functions **************************************************


void DisplayCoverOfSelectedGridNo( )
{
	INT16 sGridNo;
	INT8	bStance;

	GetMouseMapPos( &sGridNo );

	//Only allowed in if there is someone selected
	if( gusSelectedSoldier == NOBODY )
	{
		return;
	}

	//if the cursor is in a the tactical map
	if( sGridNo != NOWHERE && sGridNo != 0 )
	{
		bStance = GetCurrentMercForDisplayCoverStance();

		//if the gridno is different then the last one that was displayed
		if( sGridNo != gsLastCoverGridNo ||
				gbLastStance != bStance ||
				MercPtrs[ gusSelectedSoldier ]->sGridNo != gsLastSoldierGridNo )
		{
			//if the cover is currently being displayed
			if( gsLastCoverGridNo != NOWHERE || gbLastStance != -1 || gsLastSoldierGridNo != NOWHERE )
			{
				//remove the gridnos
				RemoveCoverOfSelectedGridNo();
			}
			else
			{
				//if it is the first time in here

				//pop up a message to say we are in the display cover routine
#ifdef JA2TESTVERSION
				{
					CHAR16	zString[512];
					swprintf( zString, L"%s, (%d)", zNewTacticalMessages[ TCTL_MSG__DISPLAY_COVER ], gGameSettings.ubSizeOfDisplayCover );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zString );
				}
#else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zNewTacticalMessages[ TCTL_MSG__DISPLAY_COVER ] );
#endif

				//increment the display cover counter ( just seeing how many times people use it )
				//gJa25SaveStruct.uiDisplayCoverCounter++;
			}

			gbLastStance = bStance;
			gsLastCoverGridNo = sGridNo;
			gsLastSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;

			//Fill the array of gridno and cover values
			CalculateCoverInRadiusAroundGridno( sGridNo, gGameSettings.ubSizeOfDisplayCover );

			//Add the graphics to each gridno
			AddCoverTileToEachGridNo();

			// Re-render the scene!
			SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
}

void AddCoverTileToEachGridNo()
{
	UINT32 uiCntX, uiCntY;
	BOOLEAN fRoof = ( gsInterfaceLevel != I_GROUND_LEVEL );


	//loop through all the gridnos
	for(uiCntY=0; uiCntY<DC_MAX_COVER_RANGE ;uiCntY++)
	{
		for(uiCntX=0; uiCntX<DC_MAX_COVER_RANGE ;uiCntX++)
		{
			//if there is a valid cover at this gridno
			if( gCoverRadius[ uiCntX ][ uiCntY ].bCover != -1 )
			{
				//if the tile provides 80-100% cover
				if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 100 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 80 )
				{
					AddCoverObjectToWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_5, fRoof );
				}

				//else if the tile provides 60-80% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 80 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 60 )
				{
					AddCoverObjectToWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_4, fRoof );
				}

				//else if the tile provides 40-60% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 60 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 40 )
				{
					AddCoverObjectToWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_3, fRoof );
				}

				//else if the tile provides 20-40% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 40 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 20 )
				{
					AddCoverObjectToWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_2, fRoof );
				}

				//else if the tile provides 0-20% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 20 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover >= 0 )
				{
					AddCoverObjectToWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_1, fRoof );
				}

				//should never get in here
				else
				{
					Assert( 0 );
				}
			}
		}
	}
}

void RemoveCoverOfSelectedGridNo()
{
	UINT32 uiCntX, uiCntY;
	BOOLEAN fRoof = ( gsInterfaceLevel != I_GROUND_LEVEL );


	if( gsLastCoverGridNo == NOWHERE )
	{
		return;
	}

	//loop through all the gridnos
	for(uiCntY=0; uiCntY<DC_MAX_COVER_RANGE ;uiCntY++)
	{
		for(uiCntX=0; uiCntX<DC_MAX_COVER_RANGE ;uiCntX++)
		{
			//if there is a valid cover at this gridno
			if( gCoverRadius[ uiCntX ][ uiCntY ].bCover != -1 )
			{
//				fRoof = gCoverRadius[ uiCntX ][ uiCntY ].fRoof;

				//if the tile provides 80-100% cover
				if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 100 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 80 )
				{
					RemoveCoverObjectFromWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_5, fRoof );
				}

				//else if the tile provides 60-80% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 80 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 60 )
				{
					RemoveCoverObjectFromWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_4, fRoof );
				}

				//else if the tile provides 40-60% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 60 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 40 )
				{
					RemoveCoverObjectFromWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_3, fRoof );
				}

				//else if the tile provides 20-40% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 40 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover > 20 )
				{
					RemoveCoverObjectFromWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_2, fRoof );
				}

				//else if the tile provides 0-20% cover
				else if( gCoverRadius[ uiCntX ][ uiCntY ].bCover <= 20 &&
						gCoverRadius[ uiCntX ][ uiCntY ].bCover >= 0 )
				{
					RemoveCoverObjectFromWorld( gCoverRadius[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_1, fRoof );
				}

				//should never get in here
				else
				{
					Assert( 0 );
				}
			}
		}
	}

	// Re-render the scene!
	SetRenderFlags( RENDER_FLAG_FULL );

	gsLastCoverGridNo = NOWHERE;
	gbLastStance = -1;
	gsLastSoldierGridNo = NOWHERE;
}


void CalculateCoverInRadiusAroundGridno( INT16 sTargetGridNo, INT8	bSearchRange )
{
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	SOLDIERTYPE *pSoldier=NULL;
	INT16	sGridNo;
	INT16	sCounterX, sCounterY;
	UINT8	ubID;
	INT8 bStance;
//	BOOLEAN fRoof;

	//clear out the array first
//	memset( gCoverRadius, -1, DC_MAX_COVER_RANGE * DC_MAX_COVER_RANGE );
	//loop through all the gridnos that we are interested in
	for (sCounterY = 0; sCounterY < DC_MAX_COVER_RANGE; sCounterY++)
	{
		for (sCounterX = 0; sCounterX < DC_MAX_COVER_RANGE; sCounterX++)
		{
			gCoverRadius[ sCounterX ][ sCounterY ].sGridNo = -1;
			gCoverRadius[ sCounterX ][ sCounterY ].bCover = -1;
		}
	}

	if( bSearchRange > ( DC_MAX_COVER_RANGE / 2 ) )
		bSearchRange = ( DC_MAX_COVER_RANGE / 2 );

	// determine maximum horizontal limits
	sMaxLeft	= min( bSearchRange,( sTargetGridNo % MAXCOL ));
	sMaxRight = min( bSearchRange,MAXCOL - (( sTargetGridNo % MAXCOL ) + 1));

	// determine maximum vertical limits
	sMaxUp	= min( bSearchRange,( sTargetGridNo / MAXROW ));
	sMaxDown = min( bSearchRange,MAXROW - (( sTargetGridNo / MAXROW ) + 1));


	//Find out which tiles around the location are reachable
	LocalReachableTest( sTargetGridNo, bSearchRange );

	pSoldier = GetCurrentMercForDisplayCover();

//	sCounterX = sCounterY = 0;

	//Determine the stance to use
	bStance = GetCurrentMercForDisplayCoverStance();

	//loop through all the gridnos that we are interested in
	for (sCounterY = 0, sCounterX = 0, sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++, sCounterY++, sCounterX = 0)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++, sCounterX++)
		{
			sGridNo = sTargetGridNo + sXOffset + (MAXCOL * sYOffset);

			//record the gridno
			gCoverRadius[ sCounterX ][ sCounterY ].sGridNo = sGridNo;

/*
			fRoof = FALSE;

			//is there a roof above this gridno
			if( FlatRoofAboveGridNo( sGridNo ) )
			{
				if( IsTheRoofVisible( sGridNo ) )
				{
					fRoof = TRUE;
				}
			}
*/
			//if the gridno is NOT on screen
			if( !GridNoOnScreen( sGridNo ) )
			{
				continue;
			}

			//if we are to display cover for the roofs, and there is a roof above us
			if( gsInterfaceLevel == I_ROOF_LEVEL && !FlatRoofAboveGridNo( sGridNo ) )
			{
				continue;
			}

			//if the gridno cant be reached
			if ( !(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE) )
			{
				//skip to the next gridno
//				sCounterX++;
				continue;
			}

			// if someone (visible) is there, skip
			//Check both bottom level, and top level
			ubID = WhoIsThere2( sGridNo, 0 );
			if( ubID == NOBODY )
			{
				ubID = WhoIsThere2( sGridNo, 1 );
			}
			//if someone is here, and they are an enemy, skip over them
			if ( ubID != NOBODY && Menptr[ ubID ].bVisible == TRUE && Menptr[ ubID ].bTeam != pSoldier->bTeam )
			{
				continue;
			}

			//Calculate the cover for this gridno
			gCoverRadius[ sCounterX ][ sCounterY ].bCover = CalcCoverForGridNoBasedOnTeamKnownEnemies( pSoldier, sGridNo, bStance );
//			gCoverRadius[ sCounterX ][ sCounterY ].fRoof = fRoof;


//			sCounterX++;
		}
//		sCounterY++;
//		sCounterX = 0;
	}
}



INT8	CalcCoverForGridNoBasedOnTeamKnownEnemies( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, INT8 bStance )
{
	INT32		iTotalCoverPoints=0;
	INT8		bNumEnemies=0;
	INT8		bPercentCoverForGridno=0;
	UINT32	uiLoop;
	SOLDIERTYPE *pOpponent;
	INT8		*pbPersOL;
	INT8		*pbPublOL;
	INT32		iGetThrough=0;
	INT32		iBulletGetThrough=0;
	INT32		iHighestValue=0;
	INT32		iCover=0;
	UINT16	usMaxRange;
	UINT16	usRange;

	//loop through all the enemies and determine the cover
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
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

		// if actual LOS check fails, then chance to hit is 0, ignore this guy
		if( SoldierToVirtualSoldierLineOfSightTest( pOpponent, sTargetGridNo, pSoldier->pathing.bLevel, bStance, TRUE, CALC_FROM_WANTED_DIR ) == 0 )
		{
			continue;
		}

		iGetThrough = SoldierToLocationChanceToGetThrough( pOpponent, sTargetGridNo, pSoldier->pathing.bLevel, bStance, NOBODY );
//	iBulletGetThrough = CalcChanceToHitGun( pOpponent, sTargetGridNo, APBPConstants[AP_MAX_AIM_ATTACK], AIM_SHOT_TORSO );

		if( WeaponInHand( pOpponent ) )
		{
			usMaxRange = GunRange( &pOpponent->inv[ HANDPOS ] );
		}
		else
		{
			usMaxRange = Weapon[ GLOCK_18 ].usRange;
		}

		usRange = (UINT16)GetRangeInCellCoordsFromGridNoDiff( pOpponent->sGridNo, sTargetGridNo );
		iBulletGetThrough = __min( __max( (INT32)( ( ( ( ( usMaxRange - usRange ) / (FLOAT)( usMaxRange ) ) + .3 ) * 100 ) ), 0 ), 100 );

		if( iBulletGetThrough > 5 && iGetThrough > 0 )
		{
			iCover = (iGetThrough * iBulletGetThrough / 100);

			if( iCover > iHighestValue )
				iHighestValue = iCover;

			iTotalCoverPoints += iCover;
			bNumEnemies++;
		}
	}

	if( bNumEnemies == 0 )
	{
		bPercentCoverForGridno = 100;
	}
	else
	{
		INT32 iTemp;

		bPercentCoverForGridno = ( iTotalCoverPoints / bNumEnemies );

		iTemp = bPercentCoverForGridno - ( iHighestValue / bNumEnemies );

		iTemp = iTemp + iHighestValue;

		bPercentCoverForGridno = 100 - ( __min( iTemp, 100 ) );
	}


	return( bPercentCoverForGridno );
}


void AddCoverObjectToWorld( INT16 sGridNo, UINT16 usGraphic, BOOLEAN fRoof )
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

void RemoveCoverObjectFromWorld( INT16 sGridNo, UINT16 usGraphic, BOOLEAN fRoof )
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

SOLDIERTYPE *GetCurrentMercForDisplayCover()
{
	SOLDIERTYPE *pSoldier=NULL;
	//Get a soldier that is on the player team
	if( gusSelectedSoldier != NOBODY )
	{
		GetSoldier( &pSoldier, gusSelectedSoldier );
	}
	else
	{
		Assert( 0 );
	}
	return( pSoldier );
}

INT8 GetCurrentMercForDisplayCoverStance()
{
	INT8	bStance;
	SOLDIERTYPE *pSoldier = NULL;

	pSoldier = GetCurrentMercForDisplayCover();

	switch( pSoldier->usUIMovementMode )
	{
		case PRONE:
		case CRAWLING:
			bStance = ANIM_PRONE;
			break;

		case KNEEL_DOWN:
		case SWATTING:
		case CROUCHING:
			bStance = ANIM_CROUCH;
			break;

		case WALKING:
		case RUNNING:
		case STANDING:
			bStance = ANIM_STAND;
			break;

		default:
			bStance = ANIM_CROUCH;
			break;
	}

	return( bStance );
}


void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	UINT16 usRange=0;
	CHAR16	zOutputString[512];

	if( sTargetGridNo == NOWHERE || sTargetGridNo == 0 )
	{
		return;
	}

	//Get the range to the target location
	usRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo );

	usRange = usRange / 10;

	// Snap: get target brightness:
	UINT8 bLightLevel = LightTrueLevel(sTargetGridNo, gsInterfaceLevel);

	//display a string with the range to target and target brightness (Snap)
	swprintf( zOutputString, zNewTacticalMessages[ TCTL_MSG__RANGE_TO_TARGET ], usRange, SHADE_MIN - bLightLevel, SHADE_MIN );

	//Display the msg
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );

	//if the soldier has a weapon in hand, display gun range and chance to hit
	if( WeaponInHand( pSoldier ) )
	{
		UINT32 uiHitChance;
		//AXP 30.03.2007: Fix CtH calculation for first shot after changing aim level (roof/ground)
		INT8 bTempTargetLevel = pSoldier->bTargetLevel;
		pSoldier->bTargetLevel = (INT8)gsInterfaceLevel;
		uiHitChance = CalcChanceToHitGun( pSoldier, sTargetGridNo, (pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
		pSoldier->bTargetLevel = bTempTargetLevel;

		swprintf( zOutputString, zNewTacticalMessages[ TCTL_MSG__GUN_RANGE_AND_CTH ], Weapon[ pSoldier->inv[HANDPOS].usItem ].usRange / 10, uiHitChance );

		//Display the msg
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
	}


	//if the target is out of the mercs gun range or knife
	if( !InRange( pSoldier, sTargetGridNo ) &&
		( Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass == IC_GUN || Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass == IC_THROWING_KNIFE	) )
	{
		// Display a warning saying so
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ OUT_OF_RANGE_STRING ] );
	}

	//increment the display gun range counter ( just seeing how many times people use it )
	//gJa25SaveStruct.uiDisplayGunRangeCounter++;
}



void DisplayGridNoVisibleToSoldierGrid( )
{
	INT16 sGridNo;
//	INT8	bStance;

	GetMouseMapPos( &sGridNo );

	//Only allowed in if there is someone selected
	if( gusSelectedSoldier == NOBODY )
	{
		return;
	}

	//if the cursor is in a the tactical map
	if( sGridNo != NOWHERE && sGridNo != 0 )
	{
		//if the gridno is different then the last one that was displayed
		if( sGridNo != gsLastVisibleToSoldierGridNo || MercPtrs[ gusSelectedSoldier ]->sGridNo != gsLastSoldierGridNo )
		{
			//if the cover is currently being displayed
			if( gsLastVisibleToSoldierGridNo != NOWHERE || gsLastSoldierGridNo != NOWHERE )
			{
				//remove the gridnos
				RemoveVisibleGridNoAtSelectedGridNo();
			}
			else
			{
#ifdef JA2TESTVERSION
				{
					CHAR16	zString[512];
					swprintf( zString, L"%s, (%d)", zNewTacticalMessages[ TCTL_MSG__LOS ], gGameSettings.ubSizeOfLOS );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zString );
				}
#else
				//pop up a message to say we are in the display cover routine
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zNewTacticalMessages[ TCTL_MSG__LOS ] );
#endif
				//increment the display LOS counter ( just seeing how many times people use it )
				//gJa25SaveStruct.uiDisplayLosCounter++;
			}

			gsLastVisibleToSoldierGridNo = sGridNo;
			gsLastSoldierGridNo = MercPtrs[ gusSelectedSoldier ]->sGridNo;


			//Fill the array of gridno and cover values
			CalculateVisibleToSoldierAroundGridno( sGridNo, gGameSettings.ubSizeOfLOS );

			//Add the graphics to each gridno
			AddVisibleToSoldierToEachGridNo();

			// Re-render the scene!
			SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
}


void CalculateVisibleToSoldierAroundGridno( INT16 sTargetGridNo, INT8 bSearchRange )
{
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	SOLDIERTYPE *pSoldier=NULL;
	INT16	sGridNo;
	INT16	sCounterX, sCounterY;
	BOOLEAN	fRoof=FALSE;

	//clear out the struct
	memset( gVisibleToSoldierStruct, 0, sizeof( VISIBLE_TO_SOLDIER_STRUCT ) * DC__SOLDIER_VISIBLE_RANGE * DC__SOLDIER_VISIBLE_RANGE );

	if( bSearchRange > ( DC_MAX_COVER_RANGE / 2 ) )
		bSearchRange = ( DC_MAX_COVER_RANGE / 2 );


	// determine maximum horizontal limits
	sMaxLeft	= min( bSearchRange,( sTargetGridNo % MAXCOL ));
	sMaxRight = min( bSearchRange,MAXCOL - (( sTargetGridNo % MAXCOL ) + 1));

	// determine maximum vertical limits
	sMaxUp	= min( bSearchRange,( sTargetGridNo / MAXROW ));
	sMaxDown = min( bSearchRange,MAXROW - (( sTargetGridNo / MAXROW ) + 1));

	pSoldier = GetCurrentMercForDisplayCover();

	sCounterX=0;
	sCounterY=0;

	//loop through all the gridnos that we are interested in
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		sCounterX = 0;
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = sTargetGridNo + sXOffset + (MAXCOL * sYOffset);
			fRoof = FALSE;

			//record the gridno
			gVisibleToSoldierStruct[ sCounterX ][ sCounterY ].sGridNo = sGridNo;

			//if the gridno is NOT on screen
			if( !GridNoOnScreen( sGridNo ) )
			{
				continue;
			}

			//is there a roof above this gridno
			if( FlatRoofAboveGridNo( sGridNo ) )
			{
				if( IsTheRoofVisible( sGridNo ) && gbWorldSectorZ == 0 )
				{
					fRoof = TRUE;
				}

				//if wer havent explored the area yet and we are underground, dont show cover
				else if( !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) && gbWorldSectorZ != 0 )
				{
					continue;
				}
			}

/*
			//if we are to display cover for the roofs, and there is a roof above us
			if( gsInterfaceLevel == I_ROOF_LEVEL && !FlatRoofAboveGridNo( sGridNo ) )
			{
				continue;
			}
*/
/*
			// if someone (visible) is there, skip
			//Check both bottom level, and top level
			ubID = WhoIsThere2( sGridNo, 0 );
			if( ubID == NOBODY )
			{
				ubID = WhoIsThere2( sGridNo, 1 );
			}
			//if someone is here, and they are an enemy, skip over them
			if ( ubID != NOBODY && Menptr[ ubID ].bVisible == TRUE && Menptr[ ubID ].bTeam != pSoldier->bTeam )
			{
				continue;
			}

			//Calculate the cover for this gridno
			gCoverRadius[ sCounterX ][ sCounterY ].bCover = CalcCoverForGridNoBasedOnTeamKnownEnemies( pSoldier, sGridNo, bStance );
*/

			gVisibleToSoldierStruct[ sCounterX ][ sCounterY ].bVisibleToSoldier = CalcIfSoldierCanSeeGridNo( pSoldier, sGridNo, fRoof );
			gVisibleToSoldierStruct[ sCounterX ][ sCounterY ].fRoof = fRoof;
			sCounterX++;
		}

		sCounterY++;
	}
}


void AddVisibleToSoldierToEachGridNo()
{
	UINT32 uiCntX, uiCntY;
	INT8	bVisibleToSoldier=0;
	BOOLEAN fRoof;
	INT16 sGridNo;

	//loop through all the gridnos
	for(uiCntY=0; uiCntY<DC_MAX_COVER_RANGE ;uiCntY++)
	{
		for(uiCntX=0; uiCntX<DC_MAX_COVER_RANGE ;uiCntX++)
		{
			bVisibleToSoldier = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].bVisibleToSoldier;
			if( bVisibleToSoldier == -1 )
			{
				continue;
			}

			fRoof = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].fRoof;
			sGridNo = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo;

			//if the soldier can easily see this gridno.	Can see all 3 positions
			if( bVisibleToSoldier == DC__SEE_3_STANCE )
			{
				AddCoverObjectToWorld( sGridNo, SPECIALTILE_COVER_5, fRoof );
			}

			//cant see a thing
			else if( bVisibleToSoldier == DC__SEE_NO_STANCES )
			{
				AddCoverObjectToWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_1, fRoof );
			}

			//can only see prone
			else if( bVisibleToSoldier == DC__SEE_1_STANCE )
			{
				AddCoverObjectToWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_2, fRoof );
			}

			//can see crouch or prone
			else if( bVisibleToSoldier == DC__SEE_2_STANCE )
			{
				AddCoverObjectToWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_3, fRoof );
			}

			else
			{
				Assert( 0 );
			}
		}
	}
}


void RemoveVisibleGridNoAtSelectedGridNo()
{
	UINT32 uiCntX, uiCntY;
	INT8	bVisibleToSoldier;
	INT16 sGridNo;
	BOOLEAN fRoof;

	//make sure to only remove it when its right
	if( gsLastVisibleToSoldierGridNo == NOWHERE )
	{
		return;
	}

	//loop through all the gridnos
	for(uiCntY=0; uiCntY<DC_MAX_COVER_RANGE ;uiCntY++)
	{
		for(uiCntX=0; uiCntX<DC_MAX_COVER_RANGE ;uiCntX++)
		{
			bVisibleToSoldier = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].bVisibleToSoldier;
			fRoof = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].fRoof;
			sGridNo = gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo;

			//if there is a valid cover at this gridno
			if( bVisibleToSoldier == DC__SEE_3_STANCE )
			{
				RemoveCoverObjectFromWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_5, fRoof );
			}

			//cant see a thing
			else if( bVisibleToSoldier == DC__SEE_NO_STANCES )
			{
				RemoveCoverObjectFromWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_1, fRoof );
			}

			//can only see prone
			else if( bVisibleToSoldier == DC__SEE_1_STANCE )
			{
				RemoveCoverObjectFromWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_2, fRoof );
			}

			//can see crouch or prone
			else if( bVisibleToSoldier == DC__SEE_2_STANCE )
			{
				RemoveCoverObjectFromWorld( gVisibleToSoldierStruct[ uiCntX ][ uiCntY ].sGridNo, SPECIALTILE_COVER_3, fRoof );
			}

			else
			{
				Assert( 0 );
			}
		}
	}

	// Re-render the scene!
	SetRenderFlags( RENDER_FLAG_FULL );

	gsLastVisibleToSoldierGridNo = NOWHERE;
	gsLastSoldierGridNo = NOWHERE;
}


INT8 CalcIfSoldierCanSeeGridNo( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, BOOLEAN fRoof )
{
	INT8	bRetVal=0;
	INT32 iLosForGridNo=0;
	UINT16	usSightLimit=0;
	INT8	*pPersOL,*pbPublOL;
	UINT8 ubID;
	BOOLEAN	bAware=FALSE;

	if( fRoof )
	{
		ubID = WhoIsThere2( sTargetGridNo, 1 );
	}
	else
	{
		ubID = WhoIsThere2( sTargetGridNo, 0 );
	}

	if( ubID != NOBODY )
	{
		pPersOL = &(pSoldier->aiData.bOppList[ubID]);
		pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][ubID]);

		// if soldier is known about (SEEN or HEARD within last few turns)
		if (*pPersOL || *pbPublOL)
		{
			bAware = TRUE;
		}
	}

    // Lesh: changed 2-nd parameter in DistanceVisible function call
	usSightLimit = pSoldier->GetMaxDistanceVisible(sTargetGridNo, fRoof, CALC_FROM_WANTED_DIR);


	//
	// Prone
	//
	iLosForGridNo = SoldierToVirtualSoldierLineOfSightTest( pSoldier, sTargetGridNo, fRoof, ANIM_PRONE, bAware, usSightLimit );
	if( iLosForGridNo != 0 )
	{
		bRetVal++;
	}

	//
	// Crouch
	//
	iLosForGridNo = SoldierToVirtualSoldierLineOfSightTest( pSoldier, sTargetGridNo, fRoof, ANIM_CROUCH, bAware, usSightLimit );
	if( iLosForGridNo != 0 )
	{
		bRetVal++;
	}

	//
	// Standing
	//
	iLosForGridNo = SoldierToVirtualSoldierLineOfSightTest( pSoldier, sTargetGridNo, fRoof, ANIM_STAND, bAware, usSightLimit );
	if( iLosForGridNo != 0 )
	{
		bRetVal++;
	}

	return( bRetVal );
}

BOOLEAN IsTheRoofVisible( INT16 sGridNo )
{
	UINT8 ubRoom;

	if( InARoom( sGridNo, &ubRoom ) )
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

#ifdef JA2TESTVERSION
/*
void DisplayLosAndDisplayCoverUsageScreenMsg()
{
	CHAR16	zString[512];

	swprintf( zString, L"Display Cover: %d", gJa25SaveStruct.uiDisplayCoverCounter );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zString );

	swprintf( zString, L"LOS: %d", gJa25SaveStruct.uiDisplayLosCounter );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zString );

	swprintf( zString, L"Gun Range: %d", gJa25SaveStruct.uiDisplayGunRangeCounter );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zString );
}
*/
#endif

void ChangeSizeOfDisplayCover( INT32 iNewSize )
{
	//if the new size is smaller or greater, scale it
	if( iNewSize < DC__MIN_SIZE )
	{
		iNewSize = DC__MIN_SIZE;
	}
	else if( iNewSize > DC__MAX_SIZE )
	{
		iNewSize = DC__MAX_SIZE;
	}

	//Set new size
	gGameSettings.ubSizeOfDisplayCover = (UINT8)iNewSize;

	//redisplay the cover
	RemoveCoverOfSelectedGridNo();
	DisplayCoverOfSelectedGridNo( );
}

void ChangeSizeOfLOS( INT32 iNewSize )
{
	//if the new size is smaller or greater, scale it
	if( iNewSize < DC__MIN_SIZE )
	{
		iNewSize = DC__MIN_SIZE;
	}
	else if( iNewSize > DC__MAX_SIZE )
	{
		iNewSize = DC__MAX_SIZE;
	}

	//Set new size
	gGameSettings.ubSizeOfLOS = (UINT8)iNewSize;

	//ReDisplay the los
	RemoveVisibleGridNoAtSelectedGridNo();
	DisplayGridNoVisibleToSoldierGrid( );
}
