#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include <string.h>
	#include "wcheck.h"
	#include "stdlib.h"
	#include "debug.h"
	#include "soldier control.h"
	#include "weapons.h"
	#include "cursor control.h"
	#include "cursors.h"
	#include "soldier find.h"
	#include "isometric utils.h"
	#include "renderworld.h"
	#include "render dirty.h"
	#include "interface.h"
	#include "spread burst.h"
	#include "points.h"
#endif


BURST_LOCATIONS			gsBurstLocations[ MAX_BURST_LOCATIONS ];
INT8					gbNumBurstLocations = 0;

extern BOOLEAN gfBeginBurstSpreadTracking;


void ResetBurstLocations( )
{
	gbNumBurstLocations = 0;
}


void InternalAccumulateBurstLocation( INT16 sGridNo )
{
	INT32 cnt;
	if ( gbNumBurstLocations < MAX_BURST_LOCATIONS )
	{
		// Check if it already exists!
		for ( cnt = 0; cnt < gbNumBurstLocations; cnt++ )
		{
			if ( gsBurstLocations[ cnt ].sGridNo == sGridNo )
			{
				return;
			}
		}

		gsBurstLocations[ gbNumBurstLocations ].sGridNo = sGridNo;

		// Get cell X, Y from mouse...
		GetMouseWorldCoords( &( gsBurstLocations[ gbNumBurstLocations ].sX ), &( gsBurstLocations[ gbNumBurstLocations ].sY ) );

		gbNumBurstLocations++;
	}
}

//Madd: to add a bit more usefulness to spread fire, I'm making it so that 
//it will automatically latch onto enemies within iSearchRange tiles of the mouse drag.
void AccumulateBurstLocation( INT16 sGridNo )
{
	SOLDIERTYPE* pTarget;
	int iSearchRange = 2; // number of tiles beside the mouse drag to look at
	INT16	sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset, sAdjacentGridNo;
	BOOLEAN foundTarget = FALSE;

		//first see if we can find a guy standing right on the spot
		pTarget = SimpleFindSoldier(sGridNo,0);
		if (!pTarget)
			pTarget = SimpleFindSoldier(sGridNo, 1); //try on a roof

		if (pTarget)
		{
			InternalAccumulateBurstLocation(sGridNo);			
			foundTarget = TRUE;	
		}
		//let's now look around this square - maybe there are some adjacent enemies we can latch onto

		// stay away from the edges

		// determine maximum horizontal limits
		sMaxLeft  = min( iSearchRange, (sGridNo % MAXCOL));
		sMaxRight = min( iSearchRange, MAXCOL - ((sGridNo % MAXCOL) + 1));

		// determine maximum vertical limits
		sMaxUp   = min( iSearchRange, (sGridNo / MAXROW));
		sMaxDown = min( iSearchRange, MAXROW - ((sGridNo / MAXROW) + 1));

		// reset the "reachable" flags in the region we're looking at
		for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
		{
			for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
			{
				sAdjacentGridNo = sGridNo + sXOffset + (MAXCOL * sYOffset);
				if ( !(sAdjacentGridNo >=0 && sAdjacentGridNo < WORLD_MAX) )
				{
					continue;
				}

				pTarget = SimpleFindSoldier(sAdjacentGridNo,0); // look for a guy in that gridno

				if (!pTarget)
					pTarget = SimpleFindSoldier(sAdjacentGridNo, 1); //try on a roof

				if (pTarget)
				{		
					InternalAccumulateBurstLocation(sAdjacentGridNo); //there's somebody there! let's latch onto him
					foundTarget = TRUE;	
				}
			}
		}


		if ( !foundTarget )
		{
			//didn't find anyone nearby, but we should target this space anyway to prevent players from abusing this feature
			InternalAccumulateBurstLocation(sGridNo);
		}
}


void PickBurstLocations( SOLDIERTYPE *pSoldier )
{
	UINT8		ubShotsPerBurst;
	FLOAT		dAccululator = 0;
	FLOAT		dStep = 0;
	INT32		cnt;
	UINT8		ubLocationNum;

	// OK, using the # of locations, spread them evenly between our current weapon shots per burst value

	// Get shots per burst
	//DIGICRAB: Burst UnCap
	//if we fire more than MAX_BURST_SPREAD_TARGETS bullets, make sure there's no buffer overflow
	if(pSoldier->bDoAutofire)
	{
		INT16	sAPCosts;

		if ( pSoldier->bDoAutofire <= gbNumBurstLocations )
		{
			pSoldier->bDoAutofire = 1;
			do
			{
				pSoldier->bDoAutofire++;
				sAPCosts = CalcTotalAPsToAttack( pSoldier, gsBurstLocations[0].sGridNo, TRUE, 0);
			}
			while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && pSoldier->inv[ pSoldier->ubAttackingHand ].ubGunShotsLeft >= pSoldier->bDoAutofire && gbNumBurstLocations >= pSoldier->bDoAutofire);
			pSoldier->bDoAutofire--;

			ubShotsPerBurst = __min(pSoldier->bDoAutofire,MAX_BURST_SPREAD_TARGETS);
		}
		else if ( gbNumBurstLocations > 0 )
			ubShotsPerBurst = pSoldier->bDoAutofire / gbNumBurstLocations;

	}
	else
	{
		if ( pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST )
			ubShotsPerBurst = __min(Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS])].ubShotsPerBurst,MAX_BURST_SPREAD_TARGETS);	
		else
			ubShotsPerBurst = __min(GetShotsPerBurst(&pSoldier->inv[ HANDPOS ]),MAX_BURST_SPREAD_TARGETS);
	}

	// Use # gridnos accululated and # burst shots to determine accululator
	dStep = gbNumBurstLocations / (FLOAT)ubShotsPerBurst;

	//Loop through our shots!
	for ( cnt = 0; cnt < ubShotsPerBurst; cnt++ )
	{
		// Get index into list
		ubLocationNum = (UINT8)( dAccululator );

		// Add to merc location
		pSoldier->sSpreadLocations[ cnt ] = gsBurstLocations[ ubLocationNum ].sGridNo;
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickBurstLocations: loc#%d = %d", cnt, pSoldier->sSpreadLocations[ cnt ]));
		// Acculuate index value
		dAccululator += dStep;
	}

	// OK, they have been added
}

void AIPickBurstLocations( SOLDIERTYPE *pSoldier, INT8 bTargets, SOLDIERTYPE *pTargets[5] )
{
	UINT8		ubShotsPerBurst;
	FLOAT		dAccululator = 0;
	FLOAT		dStep = 0;
	INT32		cnt;
	UINT8		ubLocationNum;

	// OK, using the # of locations, spread them evenly between our current weapon shots per burst value

	// Get shots per burst
	//DIGICRAB: Burst UnCap
	//if we fire more than MAX_BURST_SPREAD_TARGETS bullets, make sure there's no buffer overflow
	if(pSoldier->bDoAutofire)
		ubShotsPerBurst = __min(pSoldier->bDoAutofire,MAX_BURST_SPREAD_TARGETS);
	else
		ubShotsPerBurst = __min(GetShotsPerBurst (&pSoldier->inv[ HANDPOS ]),MAX_BURST_SPREAD_TARGETS);

	if ( ubShotsPerBurst <= 0 )
		ubShotsPerBurst = 1;

	// Use # gridnos accululated and # burst shots to determine accululator
	//dStep = gbNumBurstLocations / (FLOAT)ubShotsPerBurst;
	// CJC: tweak!
	dStep = bTargets / (FLOAT)ubShotsPerBurst;

	//Loop through our shots!
	for ( cnt = 0; cnt < ubShotsPerBurst; cnt++ )
	{
		// Get index into list
		ubLocationNum = (UINT8)( dAccululator );

		// Add to merc location
		pSoldier->sSpreadLocations[ cnt ] = pTargets[ubLocationNum]->sGridNo;

		// Acculuate index value
		dAccululator += dStep;
	}

	// OK, they have been added
}


extern HVOBJECT GetCursorFileVideoObject( UINT32 uiCursorFile );


void RenderAccumulatedBurstLocations( )
{
	INT32			cnt;
	INT16			sGridNo;
	HVOBJECT	hVObject;

	if ( !gfBeginBurstSpreadTracking )
	{
		return;
	}

	if ( gbNumBurstLocations == 0 )
	{
		return;
	}

	// Loop through each location...
	GetVideoObject( &hVObject, guiBURSTACCUM );

	// If on screen, render

	// Check if it already exists!
	for ( cnt = 0; cnt < gbNumBurstLocations; cnt++ )
	{
		sGridNo = gsBurstLocations[ cnt ].sGridNo;

		if ( GridNoOnScreen( sGridNo ) )
		{
			FLOAT				dOffsetX, dOffsetY;
			FLOAT				dTempX_S, dTempY_S;
			INT16				sXPos, sYPos;
			INT32				iBack;

			dOffsetX = (FLOAT)( gsBurstLocations[ cnt ].sX - gsRenderCenterX );
			dOffsetY = (FLOAT)( gsBurstLocations[ cnt ].sY - gsRenderCenterY );

			// Calculate guy's position
			FloatFromCellToScreenCoordinates( dOffsetX, dOffsetY, &dTempX_S, &dTempY_S );

			sXPos = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) + (INT16)dTempX_S;
			sYPos = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) + (INT16)dTempY_S - gpWorldLevelData[ sGridNo ].sHeight;

			// Adjust for offset position on screen
			sXPos -= gsRenderWorldOffsetX;
			sYPos -= gsRenderWorldOffsetY;

			// Adjust for render height
			sYPos += gsRenderHeight;

			//sScreenY -= gpWorldLevelData[ sGridNo ].sHeight;

			// Center circle!
			//sXPos -= 10;
			//sYPos -= 10;

			iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos +40 ), (INT16)(sYPos + 40 ) ); 
			if ( iBack != -1 )
			{
				SetBackgroundRectFilled( iBack );
			}

			BltVideoObject(  FRAME_BUFFER, hVObject, 1, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}
}


