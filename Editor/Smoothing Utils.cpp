#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include <stdlib.h>
	#include "worlddef.h"	//for LEVELNODE def
	#include "worldman.h" //for RemoveXXXX()
	#include "Isometric Utils.h" //for GridNoOnVisibleWorldTile()
	#include "SmartMethod.h"
	#include "Smoothing Utils.h"
	#include "Editor Undo.h"
	#include "EditorDefines.h"
	#include "edit_sys.h"
	#include "environment.h"
#endif

extern UINT16 PickAWallPiece( UINT16 usWallPieceType );

//This method isn't foolproof, but because erasing large areas of buildings could result in
//multiple wall types for each building.	When processing the region, it is necessary to
//calculate the roof type by searching for the nearest roof tile.
UINT16 SearchForWallType( INT32 iMapIndex )
{
	UINT32 uiTileType;
	LEVELNODE *pWall;
	INT16 sOffset;
	INT16 x, y, sRadius = 0;
	if( gfBasement )
	{
		UINT16 usWallType;
		usWallType = GetRandomIndexByRange( FIRSTWALL, LASTWALL );
		if( usWallType == 0xffff )
			usWallType = FIRSTWALL;
		return usWallType;
	}
	while( sRadius < 32 )
	{
		//NOTE:	start at the higher y value and go negative because it is possible to have another
		// structure type one tile north, but not one tile south -- so it'll find the correct wall first.
		for( y = sRadius; y >= -sRadius; y-- ) for( x = -sRadius; x <= sRadius; x++ )
		{
			if( abs(x) == abs(sRadius) || abs(y) == abs(sRadius) )
			{
				sOffset = y * WORLD_COLS + x;
				if( !GridNoOnVisibleWorldTile( iMapIndex + sOffset ) )
				{
					continue;
				}
				pWall = gpWorldLevelData[ iMapIndex + sOffset ].pStructHead;
				while( pWall )
				{
					GetTileType( pWall->usIndex, &uiTileType );
					if( uiTileType >= FIRSTWALL && uiTileType <= LASTWALL )
					{	//found a roof, so return its type.
						return (UINT16)uiTileType;
					}
					//if( uiTileType >= FIRSTWINDOW && uiTileType <= LASTWINDOW )
					//{	//Window types can be converted to a wall type.
					//	return (UINT16)(FIRSTWALL + uiTileType - FIRSTWINDOW );
					//}
					pWall = pWall->pNext;
				}
			}
		}
		sRadius++;
	}
	return 0xffff;
}

//This method isn't foolproof, but because erasing large areas of buildings could result in
//multiple roof types for each building.	When processing the region, it is necessary to
//calculate the roof type by searching for the nearest roof tile.
UINT16 SearchForRoofType( INT32 iMapIndex )
{
	UINT32 uiTileType;
	LEVELNODE *pRoof;
	INT16 x, y, sRadius = 0;
	INT16 sOffset;
	while( sRadius < 32 )
	{
		for( y = -sRadius; y <= sRadius; y++ ) for( x = -sRadius; x <= sRadius; x++ )
		{
			if( abs(x) == abs(sRadius) || abs(y) == abs(sRadius) )
			{
				sOffset = y * WORLD_COLS + x;
				if( !GridNoOnVisibleWorldTile( iMapIndex + sOffset ) )
				{
					continue;
				}
				pRoof = gpWorldLevelData[ iMapIndex + sOffset ].pRoofHead;
				while( pRoof )
				{
					GetTileType( pRoof->usIndex, &uiTileType );
					if( uiTileType >= FIRSTROOF && uiTileType <= LASTROOF )
					{	//found a roof, so return its type.
						return (UINT16)uiTileType;
					}
					pRoof = pRoof->pNext;
				}
			}
		}
		sRadius++;
	}
	return 0xffff;
}

BOOLEAN RoofAtGridNo( INT32 iMapIndex )
{
	LEVELNODE	*pRoof;
	UINT32 uiTileType;
	pRoof = gpWorldLevelData[ iMapIndex ].pRoofHead;
	// Look through all objects and Search for type
	while( pRoof )
	{
		if ( pRoof->usIndex != NO_TILE )
		{
			GetTileType( pRoof->usIndex, &uiTileType );
			if ( uiTileType >= FIRSTROOF && uiTileType <= SECONDSLANTROOF )
				return TRUE;
			pRoof = pRoof->pNext;
		}
	}
	return FALSE;
}

BOOLEAN BuildingAtGridNo( INT32 iMapIndex )
{
	if( RoofAtGridNo( iMapIndex ) )
		return TRUE;
	if( FloorAtGridNo( iMapIndex ) )
		return TRUE;
	return FALSE;
}

BOOLEAN ValidDecalPlacement( INT32 iMapIndex )
{
	if( GetVerticalWall( iMapIndex ) || GetHorizontalWall( iMapIndex )
			|| GetVerticalFence( iMapIndex ) || GetHorizontalFence( iMapIndex ) )
		return TRUE;
	return FALSE;
}

LEVELNODE* GetVerticalWall( INT32 iMapIndex )
{
	LEVELNODE *pStruct;
	UINT32 uiTileType;
	UINT16 usWallOrientation;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructHead;
	while( pStruct )
	{
		if ( pStruct->usIndex != NO_TILE )
		{
			GetTileType( pStruct->usIndex, &uiTileType );
			if ( uiTileType >= FIRSTWALL && uiTileType <= LASTWALL ||
					uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			{
				GetWallOrientation( pStruct->usIndex, &usWallOrientation );
				if( usWallOrientation == INSIDE_TOP_RIGHT || usWallOrientation == OUTSIDE_TOP_RIGHT )
				{
					return pStruct;
				}
			}
		}
		pStruct = pStruct->pNext;
	}
	return NULL;
}

LEVELNODE* GetHorizontalWall( INT32 iMapIndex )
{
	LEVELNODE *pStruct;
	UINT32 uiTileType;
	UINT16 usWallOrientation;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructHead;
	while( pStruct )
	{
		if ( pStruct->usIndex != NO_TILE )
		{
			GetTileType( pStruct->usIndex, &uiTileType );
			if ( uiTileType >= FIRSTWALL && uiTileType <= LASTWALL ||
					uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			{
				GetWallOrientation( pStruct->usIndex, &usWallOrientation );
				if( usWallOrientation == INSIDE_TOP_LEFT || usWallOrientation == OUTSIDE_TOP_LEFT )
				{
					return pStruct;
				}
			}
		}
		pStruct = pStruct->pNext;
	}
	return NULL;
}

UINT16 GetVerticalWallType( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	UINT32 uiTileType;
	pWall = GetVerticalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		if( uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			uiTileType = SearchForWallType( iMapIndex );
		return (UINT16)uiTileType;
	}
	return 0;
}

UINT16 GetHorizontalWallType( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	UINT32 uiTileType;
	pWall = GetHorizontalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		if( uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			uiTileType = SearchForWallType( iMapIndex );
		return (UINT16)uiTileType;
	}
	return 0;
}

LEVELNODE* GetVerticalFence( INT32 iMapIndex )
{
	LEVELNODE *pStruct;
	UINT32 uiTileType;
	UINT16 usWallOrientation;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructHead;
	while( pStruct )
	{
		if ( pStruct->usIndex != NO_TILE )
		{
			GetTileType( pStruct->usIndex, &uiTileType );
			if ( uiTileType == FENCESTRUCT )
			{
				GetWallOrientation( pStruct->usIndex, &usWallOrientation );
				if( usWallOrientation == INSIDE_TOP_RIGHT || usWallOrientation == OUTSIDE_TOP_RIGHT )
				{
					return pStruct;
				}
			}
		}
		pStruct = pStruct->pNext;
	}
	return NULL;
}

LEVELNODE* GetHorizontalFence( INT32 iMapIndex )
{
	LEVELNODE *pStruct;
	UINT32 uiTileType;
	UINT16 usWallOrientation;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructHead;
	while( pStruct )
	{
		if ( pStruct->usIndex != NO_TILE )
		{
			GetTileType( pStruct->usIndex, &uiTileType );
			if ( uiTileType == FENCESTRUCT )
			{
				GetWallOrientation( pStruct->usIndex, &usWallOrientation );
				if( usWallOrientation == INSIDE_TOP_LEFT || usWallOrientation == OUTSIDE_TOP_LEFT )
				{
					return pStruct;
				}
			}
		}
		pStruct = pStruct->pNext;
	}
	return NULL;
}

void EraseHorizontalWall( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	pWall = GetHorizontalWall( iMapIndex );
	if( pWall )
	{
		AddToUndoList( iMapIndex );
		RemoveStruct( iMapIndex, pWall->usIndex );
		RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
	}
}

void EraseVerticalWall( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	pWall = GetVerticalWall( iMapIndex );
	if( pWall )
	{
		AddToUndoList( iMapIndex );
		RemoveStruct( iMapIndex, pWall->usIndex );
		RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
	}
}

void ChangeHorizontalWall( INT32 iMapIndex, UINT16 usNewPiece )
{
	LEVELNODE *pWall;
	UINT32 uiTileType;
	UINT16 usTileIndex;
	INT16 sIndex;
	pWall = GetHorizontalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		if( uiTileType >= FIRSTWALL && uiTileType <= LASTWALL )
		{ //Okay, we have the wall, now change it's type.
			sIndex = PickAWallPiece( usNewPiece );
			AddToUndoList( iMapIndex );
			GetTileIndexFromTypeSubIndex( uiTileType, sIndex, &usTileIndex );
			ReplaceStructIndex( iMapIndex, pWall->usIndex, usTileIndex );
		}
	}
}

void ChangeVerticalWall( INT32 iMapIndex, UINT16 usNewPiece )
{
	LEVELNODE *pWall;
	UINT32 uiTileType;
	UINT16 usTileIndex;
	INT16 sIndex;
	pWall = GetVerticalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		if( uiTileType >= FIRSTWALL && uiTileType <= LASTWALL )
		{ //Okay, we have the wall, now change it's type.
			sIndex = PickAWallPiece( usNewPiece );
			AddToUndoList( iMapIndex );
			GetTileIndexFromTypeSubIndex( uiTileType, sIndex, &usTileIndex );
			ReplaceStructIndex( iMapIndex, pWall->usIndex, usTileIndex );
		}
	}
}

void RestoreWalls( INT32 iMapIndex )
{
	LEVELNODE *pWall = NULL;
	UINT32 uiTileType;
	UINT16 usWallType;
	UINT16 usWallOrientation;
	UINT8 ubSaveWallUIValue;
	BOOLEAN fDone = FALSE;

	pWall = GetHorizontalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		usWallType = (UINT16)uiTileType;
		if( uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			usWallType = SearchForWallType( iMapIndex );
		GetWallOrientation( pWall->usIndex, &usWallOrientation );
		AddToUndoList( iMapIndex );
		RemoveStruct( iMapIndex, pWall->usIndex );
		RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
		switch( usWallOrientation )
		{
			case OUTSIDE_TOP_LEFT:
				BuildWallPiece( iMapIndex, INTERIOR_BOTTOM, usWallType );
				break;
			case INSIDE_TOP_LEFT:
				BuildWallPiece( iMapIndex, EXTERIOR_BOTTOM, usWallType );
				break;
		}
		fDone = TRUE;
	}
	pWall = GetVerticalWall( iMapIndex );
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		usWallType = (UINT16)uiTileType;
		if( uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			usWallType = SearchForWallType( iMapIndex );
		GetWallOrientation( pWall->usIndex, &usWallOrientation );
		AddToUndoList( iMapIndex );
		RemoveStruct( iMapIndex, pWall->usIndex );
		RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
		switch( usWallOrientation )
		{
			case OUTSIDE_TOP_RIGHT:
				BuildWallPiece( iMapIndex, INTERIOR_RIGHT, usWallType );
				break;
			case INSIDE_TOP_RIGHT:
				BuildWallPiece( iMapIndex, EXTERIOR_RIGHT, usWallType );
				break;
		}
		fDone = TRUE;
	}
	if( fDone )
	{
		return;
	}
	//we are in a special case here.	The user is attempting to restore a wall, though nothing
	//is here.	We will hook into the smart wall method by tricking it into using the local wall
	//type, but only if we have adjacent walls.
	fDone = FALSE;
	if( pWall = GetHorizontalWall( iMapIndex - 1 ) )
	fDone = TRUE;
	if( !fDone && (pWall = GetHorizontalWall( iMapIndex + 1 )) )
		fDone = TRUE;
	if( !fDone && (pWall = GetVerticalWall( iMapIndex - WORLD_COLS )) )
		fDone = TRUE;
	if( !fDone && (pWall = GetVerticalWall( iMapIndex + WORLD_COLS )) )
		fDone = TRUE;
	if( !fDone )
		return;
	//found a wall.	Let's back up the current wall value, and restore it after pasting a smart wall.
	if( pWall )
	{
		GetTileType( pWall->usIndex, &uiTileType );
		usWallType = (UINT16)uiTileType;
		if( uiTileType >= FIRSTDOOR && uiTileType <= LASTDOOR )
			usWallType = SearchForWallType( iMapIndex );
		if( usWallType != 0xffff )
		{
			ubSaveWallUIValue = gubWallUIValue; //save the wall UI value.
			gubWallUIValue = (UINT8)usWallType;	//trick the UI value
			PasteSmartWall( iMapIndex );				//paste smart wall with fake UI value
			gubWallUIValue = ubSaveWallUIValue;	//restore the real UI value.
		}
	}
}

UINT16 GetWallClass( LEVELNODE *pWall )
{
	UINT16 row, col, rowVariants;
	UINT16 usWallIndex;
	if( !pWall )
		return 0xffff;
	GetSubIndexFromTileIndex( pWall->usIndex, &usWallIndex );
	for ( row = 0; row < NUM_WALL_TYPES; row++ )
	{
		rowVariants = gbWallTileLUT[row][0];
		for( col = 1; col <= rowVariants; col++ )
		{
			if( usWallIndex == gbWallTileLUT[row][col] )
			{
				return row;	//row is the wall class
			}
		}
	}
	return 0xffff;
}

UINT16 GetVerticalWallClass( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	if( pWall = GetVerticalWall( iMapIndex ) )
		return GetWallClass( pWall );
	return 0xffff;
}

UINT16 GetHorizontalWallClass( INT32 iMapIndex )
{
	LEVELNODE *pWall;
	if( pWall = GetVerticalWall( iMapIndex ) )
		return GetWallClass( pWall );
	return 0xffff;
}

#endif
