#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __SMOOTHING_UTILS_H
#define __SMOOTHING_UTILS_H

#include "worlddef.h"

//Use these values when specifically replacing a wall with new type.
enum {	//Wall tile types
	INTERIOR_L,										//interior wall with a top left orientation
	INTERIOR_R,										//interior wall with a top right orientation
	EXTERIOR_L,										//exterior wall with a top left orientation
	EXTERIOR_R,										//exterior wall with a top right orientation
	INTERIOR_CORNER,							//special interior end piece with top left orientation.
	//The rest of these walls are special wall tiles for top right orientations.
	INTERIOR_BOTTOMEND,						//interior wall for bottom corner 
	EXTERIOR_BOTTOMEND,						//exterior wall for bottom corner 
	INTERIOR_EXTENDED,						//extended interior wall for top corner
	EXTERIOR_EXTENDED,						//extended exterior wall for top corner
	INTERIOR_EXTENDED_BOTTOMEND,	//extended interior wall for both top and bottom corners.
	EXTERIOR_EXTENDED_BOTTOMEND,	//extended exterior wall for both top and bottom corners.
	NUM_WALL_TYPES 
};

//Use these values when passing a ubWallPiece to BuildWallPieces.
enum { 
	NO_WALLS,
	INTERIOR_TOP,		
	INTERIOR_BOTTOM,		
	INTERIOR_LEFT,		
	INTERIOR_RIGHT,		
	EXTERIOR_TOP,
	EXTERIOR_BOTTOM,
	EXTERIOR_LEFT,
	EXTERIOR_RIGHT,
};

//in newsmooth.c
extern INT8 gbWallTileLUT[NUM_WALL_TYPES][7];
extern void EraseWalls( INT32 iMapIndex );
extern void BuildWallPiece( INT32 iMapIndex, UINT8 ubWallPiece, UINT16 usWallType );
//in Smoothing Utils
void RestoreWalls( INT32 iMapIndex );
UINT16 SearchForRoofType( INT32 iMapIndex );
UINT16 SearchForWallType( INT32 iMapIndex );
BOOLEAN RoofAtGridNo( INT32 iMapIndex );
BOOLEAN BuildingAtGridNo( INT32 iMapIndex );
LEVELNODE* GetHorizontalWall( INT32 iMapIndex );
LEVELNODE* GetVerticalWall( INT32 iMapIndex );
LEVELNODE* GetVerticalFence( INT32 iMapIndex );
LEVELNODE* GetHorizontalFence( INT32 iMapIndex );
UINT16 GetHorizontalWallType( INT32 iMapIndex );
UINT16 GetVerticalWallType( INT32 iMapIndex );
void EraseHorizontalWall( INT32 iMapIndex );
void EraseVerticalWall( INT32 iMapIndex );
void ChangeHorizontalWall( INT32 iMapIndex, UINT16 usNewPiece );
void ChangeVerticalWall( INT32 iMapIndex, UINT16 usNewPiece );
UINT16 GetWallClass( LEVELNODE *pWall );
UINT16 GetVerticalWallClass( INT32 iMapIndex );
UINT16 GetHorizontalWallClass( INT32 iMapIndex );
BOOLEAN ValidDecalPlacement( INT32 iMapIndex );

#endif
#endif