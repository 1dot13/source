#include "Render Z.h"
#include "renderworld.h"

INT16 gsRScreenCenterX, gsRScreenCenterY;
INT16 gsRDistToCenterY, gsRDistToCenterX;

INT16 GetMapXYWorldY( INT16 sWorldCellX, INT16 sWorldCellY )
{
	gsRDistToCenterX = ( sWorldCellX * CELL_X_SIZE ) - gCenterWorldX;
	gsRDistToCenterY = ( sWorldCellY * CELL_Y_SIZE ) - gCenterWorldY;

	gsRScreenCenterY = gsRDistToCenterX + gsRDistToCenterY;

	return (gsRScreenCenterY + gsCY - gsTLY);
}	

INT16 GetMapXYWorldYFromCellCoords( INT16 sWorldCellX, INT16 sWorldCellY )
{
	gsRDistToCenterX = sWorldCellX	- gCenterWorldX;
	gsRDistToCenterY = sWorldCellY	- gCenterWorldY;

	gsRScreenCenterY = gsRDistToCenterX + gsRDistToCenterY;

	return gsRScreenCenterY + gsCY - gsTLY;
}	


INT16 LandZLevel( INT16 sMapX, INT16 sMapY )
{
	return LAND_Z_LEVEL;
}


INT16 ObjectZLevel( TILE_ELEMENT* TileElem, LEVELNODE* pNode, UINT32 uiTileElemFlags, INT32 sMapX, INT32 sMapY, INT16& sWorldY )
{
	sWorldY = GetMapXYWorldY( sMapX, sMapY );

	if ( uiTileElemFlags & CLIFFHANG_TILE )
	{
		return LAND_Z_LEVEL;
	}
	else if ( uiTileElemFlags & OBJECTLAYER_USEZHEIGHT )
	{
		return ( ( sWorldY ) * Z_SUBLAYERS)+LAND_Z_LEVEL;
	}
	else
	{
		return OBJECT_Z_LEVEL;
	}
}


INT16 RoofZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY )
{
	sWorldY = GetMapXYWorldY( sMapX, sMapY );
	sWorldY += WALL_HEIGHT;

	return (sWorldY*Z_SUBLAYERS)+ROOF_Z_LEVEL;

}

INT16 OnRoofZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY, UINT32 uiLevelNodeFlags )
{
	sWorldY = GetMapXYWorldY( sMapX, sMapY );

	if ( uiLevelNodeFlags & LEVELNODE_ROTTINGCORPSE )
	{
		sWorldY += ( WALL_HEIGHT + 40 );
	}
	if ( uiLevelNodeFlags & LEVELNODE_ROTTINGCORPSE )
	{
		sWorldY += ( WALL_HEIGHT + 40 );
	}
	else
	{
		sWorldY += WALL_HEIGHT;
	}

	return(sWorldY*Z_SUBLAYERS)+ONROOF_Z_LEVEL;

}

INT16 TopmostZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY )
{
	sWorldY = GetMapXYWorldY( sMapX, sMapY );

	return TOPMOST_Z_LEVEL;
}

INT16 ShadowZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY )
{
	sWorldY = GetMapXYWorldY( sMapX, sMapY );

	return __max( ( (sWorldY - 80 ) *Z_SUBLAYERS )+SHADOW_Z_LEVEL, 0 );
}



//#if 0
//		sZOffsetX = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetX;\
////		sZOffsetY = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetY;\
//#endif

