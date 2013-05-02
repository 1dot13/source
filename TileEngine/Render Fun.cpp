#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "math.h"
	#include <stdio.h>
	#include <errno.h>

	#include "Render Fun.h"
	#include "worlddef.h"
	#include "input.h"
	#include "sysutil.h"
	#include "debug.h"
	#include "worldman.h"
	#include "jascreens.h"
	#include "Isometric Utils.h"
	#include "renderworld.h"
	#include "fov.h"
	#include "Fog Of War.h"
	#include "environment.h"
	#include "dialogue control.h"
	#include "Random.h"
	#include "gamesettings.h"
#endif

// Room Information
//UINT8						gubWorldRoomInfo[ WORLD_MAX ];
//DBrot: More Rooms
//UINT8*						gubWorldRoomInfo = NULL;
UINT16*						gusWorldRoomInfo = NULL;
UINT8						gubWorldRoomHidden[ MAX_ROOMS ];


BOOLEAN InitRoomDatabase( )
{
	//memset( gubWorldRoomInfo, NO_ROOM, sizeof( gubWorldRoomInfo ) );
	memset( gubWorldRoomHidden, TRUE, sizeof( gubWorldRoomHidden ) );
	return( TRUE );
}

void ShutdownRoomDatabase( )
{

}
//DBrot: More Rooms
void SetTileRoomNum( INT32 sGridNo, UINT16 usRoomNum )
{
	// Add to global room list
	gusWorldRoomInfo[ sGridNo ] = usRoomNum;
}
//DBrot: More Rooms
void SetTileRangeRoomNum( SGPRect *pSelectRegion, UINT8 usRoomNum )
{
	INT32 cnt1, cnt2;

	for ( cnt1 = pSelectRegion->iTop; cnt1 <= pSelectRegion->iBottom; cnt1++ )
	{
		for ( cnt2 = pSelectRegion->iLeft; cnt2 <= pSelectRegion->iRight; cnt2++ )
		{
			gusWorldRoomInfo[ MAPROWCOLTOPOS( cnt1, cnt2 ) ] = usRoomNum;
		}
	}

}
//DBrot: More Rooms
BOOLEAN InARoom( INT32 sGridNo, UINT16 *pusRoomNo )
{
	// WANNE: Bugzilla #360: Crash when moving via helicopter from museum sector
	if ( sGridNo >= 0 && gusWorldRoomInfo[ sGridNo ] != NO_ROOM )
    {
		if ( pusRoomNo )
		{
			*pusRoomNo = gusWorldRoomInfo[ sGridNo ];
		}
		return( TRUE );
	}

	return( FALSE );
}

//DBrot: More Rooms
BOOLEAN InAHiddenRoom( INT32 sGridNo, UINT16 *pusRoomNo )
{
	if ( sGridNo >= 0 && gusWorldRoomInfo[ sGridNo ] != NO_ROOM )
	{
		if ( (gubWorldRoomHidden[ gusWorldRoomInfo[ sGridNo ] ] ) )
		{
			*pusRoomNo = gusWorldRoomInfo[ sGridNo ];
			return( TRUE );
		}
	}

	return( FALSE );
}


// @@ATECLIP TO WORLD!
void SetRecalculateWireFrameFlagRadius(INT16 sX, INT16 sY, INT16 sRadius)
{
	INT16 sCountX, sCountY;
	UINT32 uiTile;

	for(sCountY=sY-sRadius; sCountY < (sY+sRadius+2); sCountY++)
	{
		for(sCountX=sX-sRadius; sCountX < (sX+sRadius+2); sCountX++)
		{
			uiTile=MAPROWCOLTOPOS(sCountY, sCountX);

			gpWorldLevelData[uiTile].uiFlags |= MAPELEMENT_RECALCULATE_WIREFRAMES;

		}
	}
}


void SetGridNoRevealedFlag( INT32 sGridNo )
{
//	UINT32 cnt;
//	ITEM_POOL					*pItemPool;
//	INT16							sX, sY;
	LEVELNODE					*pNode = NULL;
	STRUCTURE					*pStructure, *pBase;

	// Set hidden flag, for any roofs
	SetRoofIndexFlagsFromTypeRange( sGridNo, FIRSTROOF, FOURTHROOF, LEVELNODE_HIDDEN	);

	// ATE: Do this only if we are in a room...
	if ( gusWorldRoomInfo[ sGridNo ] != NO_ROOM )
	{
		SetStructAframeFlags(	sGridNo, LEVELNODE_HIDDEN );
		// Find gridno one east as well...
		
		if ( ( sGridNo + WORLD_COLS ) < MAX_MAP_POS )
		{
			SetStructAframeFlags(	sGridNo + WORLD_COLS, LEVELNODE_HIDDEN );
		}

		if ( ( sGridNo + 1 ) < MAX_MAP_POS )
		{
			SetStructAframeFlags(	sGridNo + 1, LEVELNODE_HIDDEN );
		}
	}

	// Set gridno as revealed
	gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_REVEALED;
	if( gfCaves )
	{
		RemoveFogFromGridNo( sGridNo );
	}

	// ATE: If there are any structs here, we can render them with the obscured flag!
	// Look for anything but walls pn this gridno!
	pStructure	=  gpWorldLevelData[ sGridNo ].pStructureHead;

	while ( pStructure != NULL )
	{
		if ( pStructure->sCubeOffset == STRUCTURE_ON_GROUND || ( pStructure->fFlags & STRUCTURE_SLANTED_ROOF ) )
		{
			if ( ( (pStructure->fFlags & STRUCTURE_OBSTACLE ) && !( pStructure->fFlags & ( STRUCTURE_PERSON | STRUCTURE_CORPSE ) ) ) || ( pStructure->fFlags & STRUCTURE_SLANTED_ROOF ) )
			{
				pBase = FindBaseStructure( pStructure );
				//Ja25: DEF: 10/12/99		Added because ...
				if( pBase == NULL )
				{
					pStructure = pStructure->pNext;
					Assert( 0 );
					continue;
				}

				// Get LEVELNODE for struct and remove!
				pNode = FindLevelNodeBasedOnStructure( pBase->sGridNo, pBase );

				if( pNode )
					pNode->uiFlags |= LEVELNODE_SHOW_THROUGH;

				if ( pStructure->fFlags & STRUCTURE_SLANTED_ROOF )
				{
					AddSlantRoofFOVSlot( pBase->sGridNo );

					// Set hidden...
					pNode->uiFlags |= LEVELNODE_HIDDEN;

				}
			}
		}

		pStructure = pStructure->pNext;
	}

	gubWorldRoomHidden[ gusWorldRoomInfo[ sGridNo ] ] = FALSE;

}


void ExamineGridNoForSlantRoofExtraGraphic( INT32 sCheckGridNo )
{
	LEVELNODE					*pNode = NULL;
	STRUCTURE					*pStructure, *pBase;
	UINT8 ubLoop;
	DB_STRUCTURE_TILE	**	ppTile;
	INT32 sGridNo;
	UINT16						usIndex;
	BOOLEAN						fChanged = FALSE;

	// CHECK FOR A SLANTED ROOF HERE....
	pStructure = FindStructure( sCheckGridNo, STRUCTURE_SLANTED_ROOF );

	if ( pStructure != NULL )
	{
		// We have a slanted roof here ... find base and remove...
		pBase = FindBaseStructure( pStructure );

		// Get LEVELNODE for struct and remove!
		pNode = FindLevelNodeBasedOnStructure( pBase->sGridNo, pBase );

		// Loop through each gridno and see if revealed....
		for ( ubLoop = 0; ubLoop < pBase->pDBStructureRef->pDBStructure->ubNumberOfTiles; ubLoop++ )
		{
			ppTile = pBase->pDBStructureRef->ppTile;
			sGridNo = pBase->sGridNo + ppTile[ ubLoop ]->sPosRelToBase;

			if (sGridNo < 0 || sGridNo > WORLD_MAX)
			{
				continue;
			}

			// Given gridno,
			// IF NOT REVEALED AND HIDDEN....
			if ( !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) && pNode->uiFlags & LEVELNODE_HIDDEN )
			{
				// Add graphic if one does not already exist....
				if ( !TypeExistsInRoofLayer( sGridNo, SLANTROOFCEILING, &usIndex ) )
				{
					// Add
					AddRoofToHead( sGridNo, SLANTROOFCEILING1 );
					fChanged = TRUE;
				}
			}

			// Revealed?
			if ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED )
			{
				///Remove any slant roof items if they exist
				if ( TypeExistsInRoofLayer( sGridNo, SLANTROOFCEILING, &usIndex ) )
				{
					RemoveRoof( sGridNo, usIndex );
					fChanged = TRUE;
				}
			}

		}
	}

	if ( fChanged )
	{
		// DIRTY THE WORLD!
		InvalidateWorldRedundency();
		SetRenderFlags(RENDER_FLAG_FULL );
	}

}

//DBrot: More Rooms
void RemoveRoomRoof( INT32 sGridNo, UINT16 usRoomNum, SOLDIERTYPE *pSoldier )
{
	INT32			cnt;
	ITEM_POOL					*pItemPool;
	INT16							sX, sY;
	BOOLEAN						fSaidItemSeenQuote = FALSE;

//	STRUCTURE					*pStructure;//, *pBase;

	// LOOP THORUGH WORLD AND CHECK ROOM INFO
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		if ( gusWorldRoomInfo[ cnt ] == usRoomNum )
		{

			SetGridNoRevealedFlag( cnt );//dnl ch56 141009

			RemoveRoofIndexFlagsFromTypeRange( cnt, FIRSTROOF, SECONDSLANTROOF, LEVELNODE_REVEAL );

			// Reveal any items if here!
			if ( GetItemPoolFromGround( cnt, &pItemPool ) )
			{
				// Set visible! ( only if invisible... )
				if ( SetItemPoolVisibilityOn( pItemPool, INVISIBLE, TRUE ) )
				{
					if ( !fSaidItemSeenQuote )
					{
						fSaidItemSeenQuote = TRUE;

						if ( pSoldier != NULL )
						{
						//ddd
							//TacticalCharacterDialogue( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ) );
							if(Chance(gGameExternalOptions.iChanceSayAnnoyingPhrase) )
								TacticalCharacterDialogue( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ) );
								//ddd
						}
					}
				}
			}

			// OK, re-set writeframes ( in a radius )
			// Get XY
			ConvertGridNoToXY( cnt, &sX, &sY );
			SetRecalculateWireFrameFlagRadius( sX, sY, 2 );

		}
	}

	//for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	//{
	//	if ( gubWorldRoomInfo[ cnt ] == bRoomNum )
	//	{
	//		ExamineGridNoForSlantRoofExtraGraphic( (INT16)cnt );
	//	}
	//}

	// DIRTY THE WORLD!
	InvalidateWorldRedundency();
	SetRenderFlags(RENDER_FLAG_FULL );


	CalculateWorldWireFrameTiles( FALSE );

}



BOOLEAN AddSpecialTileRange( SGPRect *pSelectRegion	)
{
	INT32 cnt1, cnt2;

	for ( cnt1 = pSelectRegion->iTop; cnt1 <= pSelectRegion->iBottom; cnt1++ )
	{
		for ( cnt2 = pSelectRegion->iLeft; cnt2 <= pSelectRegion->iRight; cnt2++ )
		{
			AddObjectToHead( MAPROWCOLTOPOS( cnt1, cnt2 ), SPECIALTILE_MAPEXIT ); 
		}
	}

	return( TRUE );
}


BOOLEAN RemoveSpecialTileRange( SGPRect *pSelectRegion	)
{
	INT32 cnt1, cnt2;

	for ( cnt1 = pSelectRegion->iTop; cnt1 <= pSelectRegion->iBottom; cnt1++ )
	{
		for ( cnt2 = pSelectRegion->iLeft; cnt2 <= pSelectRegion->iRight; cnt2++ )
		{
			RemoveObject( MAPROWCOLTOPOS( cnt1, cnt2 ), SPECIALTILE_MAPEXIT ); 
		}
	}

	return( TRUE );
}

