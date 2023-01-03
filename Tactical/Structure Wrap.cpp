#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include "debug.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "structure wrap.h"
	#include "isometric utils.h"
	#include "worldman.h"
	#include "overhead.h"
	#include "renderworld.h"
	#include "strategicmap.h"
	#include "rotting corpses.h"
	#include "WorldDat.h"	// added by Flugente
#endif

extern BOOLEAN DoesSAMExistHere( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo );



//----------------legion by Jazz

BOOLEAN	IsJumpableWindowPresentAtGridNo( INT32 sGridNo, INT8 direction2, BOOLEAN fIntactWindowsAlso )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLNWINDOW );
	
	if ( pStructure )
	{
		// anv: additional tile properties hook
		if(gGameExternalOptions.fAdditionalTileProperties)
		{
			LEVELNODE *pNode = FindLevelNodeBasedOnStructure( sGridNo, pStructure );
			if(pNode != NULL)
			{
				if( gTileDatabase[ pNode->usIndex ].uiAdditionalFlags & ADDITIONAL_TILE_FLAG_BLOCKED_WINDOW )
				// special tag disables jumping through specific windows (e.g. barred windows in Tixa)
					return( FALSE );
			}
		}
		
		if ( ( direction2 == SOUTH || direction2 == NORTH ) && (pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT || pStructure->ubWallOrientation == INSIDE_TOP_LEFT ) && pStructure->fFlags & STRUCTURE_WALLNWINDOW && !(pStructure->fFlags & STRUCTURE_SPECIAL) )
	    {
			if ( fIntactWindowsAlso || ( pStructure->fFlags & STRUCTURE_OPEN ) )
         		return( TRUE );
	    }
	                            	
        if ( ( direction2 == EAST || direction2 == WEST ) && ( pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT || pStructure->ubWallOrientation == INSIDE_TOP_RIGHT ) && pStructure->fFlags & STRUCTURE_WALLNWINDOW && !(pStructure->fFlags & STRUCTURE_SPECIAL) )
		{
			if ( fIntactWindowsAlso || ( pStructure->fFlags & STRUCTURE_OPEN ) )
				return( TRUE );
	    }
	}

	return( FALSE );
}


BOOLEAN	IsOknoFencePresentAtGridno( INT32 sGridNo )
{

	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLNWINDOW );

	if ( pStructure )
	{
		if ( pStructure->fFlags & STRUCTURE_WALLNWINDOW && !(pStructure->fFlags & STRUCTURE_SPECIAL) && ( pStructure->fFlags & STRUCTURE_OPEN ) )
		{
			return( TRUE );
		}
	}
	
/*	STRUCTURE * pStructure;
	STRUCTURE * pStructure2;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLNWINDOW );

	if ( pStructure )
	{
	//	pStructure2 = FindStructure( sGridNo, STRUCTURE_WALL );
              // 	if ( !pStructure2 )
              //	{
        	if ( pStructure->fFlags & STRUCTURE_WALLNWINDOW && !(pStructure->fFlags & STRUCTURE_SPECIAL)  && ( pStructure->fFlags & STRUCTURE_OPEN ))
		{
			return( TRUE );
		}
		
	//	}
		
	}
	
	*/
	return( FALSE );
}

BOOLEAN	IsLegionWallPresentAtGridno( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_FENCE );

	if ( pStructure )
	{
		
		if ( pStructure->fFlags & STRUCTURE_FENCE && pStructure->fFlags & STRUCTURE_SPECIAL && pStructure->fFlags & STRUCTURE_WALL ) 
		{
			return( TRUE );
		}
	}
	return( FALSE );
}
//-------------------------------------------------------------------------------------

BOOLEAN	IsFencePresentAtGridNo( INT32 sGridNo )
{
	if ( FindStructure( sGridNo, STRUCTURE_ANYFENCE ) != NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN	IsRoofPresentAtGridNo( INT32 sGridNo )
{
	if ( FindStructure( sGridNo, STRUCTURE_ROOF ) != NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN	IsJumpableFencePresentAtGridNo( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_OBSTACLE );

	if ( pStructure )
	{
		if ( pStructure->fFlags & STRUCTURE_FENCE && !(pStructure->fFlags & STRUCTURE_SPECIAL) )
		{
			return( TRUE );
		}
		if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_SANDBAG && StructureHeight( pStructure ) <= 2 )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


BOOLEAN	IsDoorPresentAtGridNo( INT32 sGridNo )
{
	if ( FindStructure( sGridNo, STRUCTURE_ANYDOOR ) != NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN	IsTreePresentAtGridNo( INT32 sGridNo )
{
	if ( FindStructure( sGridNo, STRUCTURE_TREE ) != NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}


LEVELNODE *IsWallPresentAtGridNo( INT32 sGridNo )
{
	LEVELNODE *pNode = NULL;
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLSTUFF );

	if ( pStructure != NULL )
	{
		pNode = FindLevelNodeBasedOnStructure( sGridNo, pStructure );
	}

	return( pNode );
}

LEVELNODE	*GetWallLevelNodeOfSameOrientationAtGridNo( INT32 sGridNo, INT8 ubOrientation )
{
	LEVELNODE *pNode = NULL;
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLSTUFF );

	while ( pStructure != NULL )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == ubOrientation )
		{
			pNode = FindLevelNodeBasedOnStructure( sGridNo, pStructure );
			return( pNode );
		}
		pStructure = FindNextStructure( pStructure, STRUCTURE_WALLSTUFF );
	}

	return( NULL );
}


LEVELNODE	*GetWallLevelNodeAndStructOfSameOrientationAtGridNo( INT32 sGridNo, INT8 ubOrientation, STRUCTURE **ppStructure )
{
	LEVELNODE *pNode = NULL;
	STRUCTURE * pStructure, * pBaseStructure;

	(*ppStructure) = NULL;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLSTUFF );

	while ( pStructure != NULL )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == ubOrientation )
		{
			pBaseStructure = FindBaseStructure( pStructure );
			if (pBaseStructure)
			{
				pNode = FindLevelNodeBasedOnStructure( pBaseStructure->sGridNo, pBaseStructure );
				(*ppStructure) = pBaseStructure;
				return( pNode );
			}
		}
		pStructure = FindNextStructure( pStructure, STRUCTURE_WALLSTUFF );
	}

	return( NULL );
}


BOOLEAN IsDoorVisibleAtGridNo( INT32 sGridNo )
{
	STRUCTURE * pStructure;
	INT32 sNewGridNo;

	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );

	if ( pStructure != NULL )
	{
		// Check around based on orientation
		switch( pStructure->ubWallOrientation )
		{
			case INSIDE_TOP_LEFT:
			case OUTSIDE_TOP_LEFT:

				// Here, check north direction
				sNewGridNo = NewGridNo( sGridNo, DirectionInc( NORTH ) );

				if ( IsRoofVisible2( sNewGridNo ) )
				{
					// OK, now check south, if true, she's not visible
					sNewGridNo = NewGridNo( sGridNo, DirectionInc( SOUTH ) );

					if ( IsRoofVisible2( sNewGridNo ) )
					{
						return( FALSE );
					}
				}
				break;

			case INSIDE_TOP_RIGHT:
			case OUTSIDE_TOP_RIGHT:

				// Here, check west direction
				sNewGridNo = NewGridNo( sGridNo, DirectionInc( WEST ) );

				if ( IsRoofVisible2( sNewGridNo ) )
				{
					// OK, now check south, if true, she's not visible
					sNewGridNo = NewGridNo( sGridNo, DirectionInc( EAST ) );

					if ( IsRoofVisible2( sNewGridNo ) )
					{
						return( FALSE );
					}
				}
				break;

		}

	}

	// Return true here, even if she does not exist
	return( TRUE );
}


BOOLEAN DoesGridNoContainHiddenStruct( INT32 sGridNo, BOOLEAN *pfVisible )
{
	// ATE: These are ignored now - always return false

	//STRUCTURE *pStructure;

	//pStructure = FindStructure( sGridNo, STRUCTURE_HIDDEN );

	//if ( pStructure != NULL )
	//{
	//	if ( !(gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS)	)
	//	{
	//		*pfVisible = FALSE;
	//	}
	//	else
	//	{
	//		*pfVisible = TRUE;
	//	}//
//
	//	return( TRUE );
	//}

	return( FALSE );
}


BOOLEAN IsHiddenStructureVisible( INT32 sGridNo, UINT16 usIndex )
{
	// Check if it's a hidden struct and we have not revealed anything!
	if ( gTileDatabase[ usIndex ].uiFlags & HIDDEN_TILE )
	{
		if ( !(gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS) )
		{
			// Return false
			return( FALSE );
		}
	}

	return( TRUE );
}


BOOLEAN	WallExistsOfTopLeftOrientation( INT32 sGridNo )
{
	// CJC: changing to search only for normal walls, July 16, 1998
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALL );

	while ( pStructure != NULL )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
		{
			return( TRUE );
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_WALL );

	}

	return( FALSE );
}

BOOLEAN	WallExistsOfTopRightOrientation( INT32 sGridNo )
{
	// CJC: changing to search only for normal walls, July 16, 1998
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALL );

	while ( pStructure != NULL )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
		{
			return( TRUE );
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_WALL );

	}

	return( FALSE );
}

BOOLEAN WallOrClosedDoorExistsOfTopLeftOrientation( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLSTUFF );

	while ( pStructure != NULL )
	{
		// skip it if it's an open door
		if ( ! ( ( pStructure->fFlags & STRUCTURE_ANYDOOR ) && ( pStructure->fFlags & STRUCTURE_OPEN ) ) )
		{
			// Check orientation
			if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
			{
				return( TRUE );
			}
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_WALLSTUFF );

	}

	return( FALSE );
}

BOOLEAN WallOrClosedDoorExistsOfTopRightOrientation( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WALLSTUFF );

	while ( pStructure != NULL )
	{
		// skip it if it's an open door
		if ( ! ( ( pStructure->fFlags & STRUCTURE_ANYDOOR ) && ( pStructure->fFlags & STRUCTURE_OPEN ) ) )
		{
			// Check orientation
			if ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
			{
				return( TRUE );
			}
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_WALLSTUFF );

	}

	return( FALSE );
}

BOOLEAN OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );

	while ( pStructure != NULL && (pStructure->fFlags & STRUCTURE_OPEN) )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
		{
			if ( (pStructure->fFlags & STRUCTURE_DOOR) || (pStructure->fFlags & STRUCTURE_DDOOR_RIGHT) )
			{
				return( TRUE );
			}
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_ANYDOOR );

	}

	return( FALSE );
}

BOOLEAN OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );

	while ( pStructure != NULL && (pStructure->fFlags & STRUCTURE_OPEN) )
	{
		// Check orientation
		if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
		{
			if ( (pStructure->fFlags & STRUCTURE_DOOR) || (pStructure->fFlags & STRUCTURE_DDOOR_LEFT) )
			{
				return( TRUE );
			}
		}

		pStructure = FindNextStructure( pStructure, STRUCTURE_ANYDOOR );

	}

	return( FALSE );
}

STRUCTURE	* FindCuttableWireFenceAtGridNo( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WIREFENCE );
	if (pStructure != NULL && pStructure->ubWallOrientation != NO_ORIENTATION && !(pStructure->fFlags & STRUCTURE_OPEN) )
	{
		return( pStructure );
	}
	return( NULL );
}

BOOLEAN CutWireFence( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindCuttableWireFenceAtGridNo( sGridNo );
	if (pStructure)
	{
		pStructure = SwapStructureForPartnerAndStoreChangeInMap( sGridNo, pStructure );
		if (pStructure)
		{
			RecompileLocalMovementCosts( sGridNo );
			SetRenderFlags( RENDER_FLAG_FULL );
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN IsCuttableWireFenceAtGridNo( INT32 sGridNo )
{
	return( FindCuttableWireFenceAtGridNo( sGridNo ) != NULL );
}


BOOLEAN IsRepairableStructAtGridNo( INT32 sGridNo, UINT8 *pubID )
{
	UINT8 ubMerc;

	// OK, first look for a vehicle....
	ubMerc = WhoIsThere2( sGridNo, 0 );

	if ( pubID != NULL )
	{
		(*pubID) = ubMerc;
	}

	if ( ubMerc != NOBODY )
	{
		if (MercPtrs[ubMerc]->flags.uiStatusFlags & SOLDIER_ROBOT)
		{
			return( 1 );
		}
		else if ( MercPtrs[ ubMerc ]->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			return( 2 );
		}
	}
	// Then for over a robot....

	// then for SAM site....
	if ( DoesSAMExistHere( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, sGridNo ) )
	{
		return( 3 );
	}


	return( FALSE );
}


BOOLEAN IsRefuelableStructAtGridNo( INT32 sGridNo, UINT8 *pubID )
{
	UINT8 ubMerc;

	// OK, first look for a vehicle....
	ubMerc = WhoIsThere2( sGridNo, 0 );

	if ( pubID != NULL )
	{
		(*pubID) = ubMerc;
	}

	if ( ubMerc != NOBODY )
	{
		if ( MercPtrs[ ubMerc ]->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}


// Flugente: determine wether a fortification can be built on this position
BOOLEAN IsFortificationPossibleAtGridNo( INT32 sGridNo )
{
	INT8 bOverTerrainType = GetTerrainType( sGridNo );
	if( bOverTerrainType == MED_WATER || bOverTerrainType == DEEP_WATER || bOverTerrainType == LOW_WATER )
		return FALSE;

	STRUCTURE * pStructure = NULL;

	pStructure = FindStructure( sGridNo, (STRUCTURE_OBSTACLE|STRUCTURE_PERSON) );

	return( pStructure == NULL );
}

UINT16 InteractiveActionPossibleAtGridNo( INT32 sGridNo, UINT8 usLevel, UINT16& arusStructIndex )
{
	arusStructIndex = 0;

	if ( TileIsOutOfBounds( sGridNo ) )
		return INTERACTIVE_STRUCTURE_NO_ACTION; 
	
	INT32 tmpgridno = 0;
	UINT16 structurenumber = 0;
	INT32 structuregridno = NOWHERE;
	UINT32 uiTileType = 0;
	bool properdatafound( false );

	STRUCTURE* pStruct = FindStructure( sGridNo, ( STRUCTURE_GENERIC ) );

	// Wall decorations are on the gridno of the wall. This can be problematic if this gridno is 'behind' the wall, because then we might not find a path.
	// Thus we need to check for decorations on these walls
	if ( !pStruct )
	{
		tmpgridno = NewGridNo( sGridNo, DirectionInc( NORTH ) );

		pStruct = GetStructForLevelNodeOfTypeRange( tmpgridno, FIRSTDECORATIONS, FOURTHDECORATIONS );

		// if struct is found but has wrong orientation, ignore it
		if ( pStruct
			&& pStruct->ubWallOrientation != OUTSIDE_TOP_LEFT
			&& pStruct->ubWallOrientation != INSIDE_TOP_LEFT )
			pStruct = NULL;
	}

	if ( !pStruct )
	{
		tmpgridno = NewGridNo( sGridNo, DirectionInc( WEST ) );

		pStruct = GetStructForLevelNodeOfTypeRange( tmpgridno, FIRSTDECORATIONS, FOURTHDECORATIONS );

		// if struct is found but has wrong orientation, ignore it
		if ( pStruct
			&& pStruct->ubWallOrientation != OUTSIDE_TOP_RIGHT
			&& pStruct->ubWallOrientation != INSIDE_TOP_RIGHT )
			pStruct = NULL;
	}

	if ( pStruct )
	{
		// if this is a multi-tile structure, be sure to use the base gridno
		if ( !( pStruct->fFlags & STRUCTURE_BASE_TILE ) )
		{
			pStruct = FindBaseStructure( pStruct );

			if ( !pStruct )
				return INTERACTIVE_STRUCTURE_NO_ACTION;
		}

		structuregridno = pStruct->sGridNo;
		structurenumber = pStruct->pDBStructureRef->pDBStructure->usStructureNumber;

		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode
			&& GetTileType( pNode->usIndex, &uiTileType ) )
		{
			properdatafound = true;
		}
	}
	
	// With Decals we don't have structuredata for the node. Thus we have to work around that by getting the data from the TileElem
	if ( !properdatafound )
	{
		tmpgridno = NewGridNo( sGridNo, DirectionInc( NORTH ) );

		if ( GetTypeRegionIndexForLevelNodeOfTypeRange( tmpgridno, FIRSTWALLDECAL, FOURTHWALLDECAL, uiTileType, structurenumber )
			|| GetTypeRegionIndexForLevelNodeOfTypeRange( tmpgridno, FIFTHWALLDECAL, EIGTHWALLDECAL, uiTileType, structurenumber ) )
		{
			properdatafound = true;
		}
	}

	if ( !properdatafound )
	{
		tmpgridno = NewGridNo( sGridNo, DirectionInc( WEST ) );

		if ( GetTypeRegionIndexForLevelNodeOfTypeRange( tmpgridno, FIRSTWALLDECAL, FOURTHWALLDECAL, uiTileType, structurenumber )
			|| GetTypeRegionIndexForLevelNodeOfTypeRange( tmpgridno, FIFTHWALLDECAL, EIGTHWALLDECAL, uiTileType, structurenumber ) )
		{
			properdatafound = true;
		}
	}

	if ( properdatafound )
	{
		// we loop over each action and determine whether it fits
		// multiple actions can fit, we chose whatever fits best
		// So, for example, we can set computers everywhere to be hackable with a generic result, and then define more precise results on specific computers
		// Other example: With just oen entry, with neither sector nor location set, we can make all water taps drinkable
		BOOLEAN foundmatch_precise	= FALSE;
		BOOLEAN foundmatch_sector	= FALSE;
		BOOLEAN foundmatch_any		= FALSE;

		UINT8 sector = SECTOR( gWorldSectorX, gWorldSectorY );

		int foundindex = 0;
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType][0] )
		{
			for ( int i = 0; i < gMaxInteractiveStructureRead; ++i )
			{
				if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType], gInteractiveStructure[i].szTileSetName, 11 ) )
				{
					std::vector<UINT16> tmpvec = gInteractiveStructure[i].tileindexvector;

					std::vector<UINT16>::iterator it = std::find( tmpvec.begin( ), tmpvec.end( ), structurenumber );

					if ( it != tmpvec.end( ) )
					{
						if ( gInteractiveStructure[i].sector == sector && gInteractiveStructure[i].sectorlevel == gbWorldSectorZ )
						{
							foundmatch_sector = TRUE;
							foundindex = i;

							if ( gInteractiveStructure[i].sLevel == usLevel || gInteractiveStructure[i].sLevel == -1 )
							{
								std::vector<INT32> tmpgridnovec = gInteractiveStructure[i].gridnovector;

								std::vector<INT32>::iterator it_gridno = std::find( tmpgridnovec.begin( ), tmpgridnovec.end( ), structuregridno );

								if ( it_gridno != tmpgridnovec.end( ) )
								{
									foundmatch_precise = TRUE;
									break;
								}
							}
						}
						// some actions cen be defined everywhere
						else if ( !foundmatch_sector && gInteractiveStructure[i].sector == -1 && gInteractiveStructure[i].sectorlevel == -1 )
						{
							foundmatch_any = TRUE;
							foundindex = i;
						}
					}
				}
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[uiTileType][0] )
		{
			for ( int i = 0; i < gMaxInteractiveStructureRead; ++i )
			{
				if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[uiTileType], gInteractiveStructure[i].szTileSetName, 11 ) )
				{
					std::vector<UINT16> tmpvec = gInteractiveStructure[i].tileindexvector;

					std::vector<UINT16>::iterator it = std::find( tmpvec.begin( ), tmpvec.end( ), structurenumber );

					if ( it != tmpvec.end() )
					{
						if ( gInteractiveStructure[i].sector == sector && gInteractiveStructure[i].sectorlevel == gbWorldSectorZ )
						{
							foundmatch_sector = TRUE;
							foundindex = i;

							if ( gInteractiveStructure[i].sLevel == usLevel || gInteractiveStructure[i].sLevel == -1 )
							{
								std::vector<INT32> tmpgridnovec = gInteractiveStructure[i].gridnovector;

								std::vector<INT32>::iterator it_gridno = std::find( tmpgridnovec.begin( ), tmpgridnovec.end( ), structuregridno );

								if ( it_gridno != tmpgridnovec.end() )
								{
									foundmatch_precise = TRUE;
									break;
								}
							}
						}
						else if ( !foundmatch_sector && gInteractiveStructure[i].sector == -1 && gInteractiveStructure[i].sectorlevel == -1 )
						{
							foundmatch_any = TRUE;
							foundindex = i;
						}
					}
				}
			}
		}

		if ( foundmatch_any || foundmatch_sector || foundmatch_precise )
		{
			arusStructIndex = foundindex;

			return gInteractiveStructure[foundindex].sActionType;
		}
	}

	return INTERACTIVE_STRUCTURE_NO_ACTION;
}


BOOLEAN IsCutWireFenceAtGridNo( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_WIREFENCE );
	if (pStructure != NULL && (pStructure->ubWallOrientation != NO_ORIENTATION) && (pStructure->fFlags & STRUCTURE_OPEN) )
	{
		return( TRUE );
	}
	return( FALSE );
}



INT32 FindDoorAtGridNoOrAdjacent( INT32 sGridNo )
{
	STRUCTURE * pStructure;
	STRUCTURE * pBaseStructure;
	INT32				sTestGridNo;

	sTestGridNo = sGridNo;
	pStructure = FindStructure( sTestGridNo, STRUCTURE_ANYDOOR );
	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
		return( pBaseStructure->sGridNo );
	}

	sTestGridNo = sGridNo + DirectionInc( NORTH );
	pStructure = FindStructure( sTestGridNo, STRUCTURE_ANYDOOR );
	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
		return( pBaseStructure->sGridNo );
	}

	sTestGridNo = sGridNo + DirectionInc( WEST );
	pStructure = FindStructure( sTestGridNo, STRUCTURE_ANYDOOR );
	if (pStructure)
	{
		pBaseStructure = FindBaseStructure( pStructure );
		return( pBaseStructure->sGridNo );
	}

	return( NOWHERE );
}



BOOLEAN IsCorpseAtGridNo( INT32 sGridNo, UINT8 ubLevel )
{
	if ( GetCorpseAtGridNo( sGridNo , ubLevel ) != NULL )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN SetOpenableStructureToClosed( INT32 sGridNo, UINT8 ubLevel )
{
	STRUCTURE *		pStructure;
	STRUCTURE *		pNewStructure;

	pStructure = FindStructure( sGridNo, STRUCTURE_OPENABLE );
	if ( !pStructure )
	{
		return( FALSE );
	}

	if ( pStructure->fFlags & STRUCTURE_OPEN )
	{
		pNewStructure = SwapStructureForPartner( sGridNo, pStructure );
		if ( pNewStructure != NULL)
		{
			RecompileLocalMovementCosts( sGridNo );
			SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
	// else leave it as is!
	return( TRUE );
}
