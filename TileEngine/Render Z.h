#include "worlddef.h"
#include "renderworld.h"
#include "Isometric Utils.h"


INT16 ObjectZLevel( TILE_ELEMENT* TileElem, LEVELNODE* pNode, UINT32 uiTileElemFlags, INT32 sMapX, INT32 sMapY, INT16& sWorldY );
INT16 GetMapXYWorldY( INT16 sWorldCellX, INT16 sWorldCellY );
INT16 GetMapXYWorldYFromCellCoords( INT16 sWorldCellX, INT16 sWorldCellY );
INT16 LandZLevel( INT16 sMapX, INT16 sMapY );
INT16 RoofZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY );
INT16 OnRoofZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY, UINT32 uiLevelNodeFlags );
INT16 TopmostZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY );
INT16 ShadowZLevel( INT16 sMapX, INT16 sMapY, INT16& sWorldY );



#define	StructZLevel( sMapX, sMapY )\
{\
\
	sWorldY = GetMapXYWorldY( sMapX, sMapY );\
\
	if ( ( uiLevelNodeFlags & LEVELNODE_ROTTINGCORPSE ) )\
	{\
		if ( pCorpse->def.usFlags & ROTTING_CORPSE_VEHICLE )\
		{\
\
			if ( pNode->pStructureData != NULL )\
			{\
				sZOffsetX = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetX;\
				sZOffsetY = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetY;\
			}\
	\
			sWorldY = GetMapXYWorldY( (sMapX + sZOffsetX), (sMapY + sZOffsetY) );\
	\
			sWorldY = GetMapXYWorldY( (sMapX + sZOffsetX), (sMapY + sZOffsetY) );\
	\
			sZLevel=( ( sWorldY ) * Z_SUBLAYERS)+STRUCT_Z_LEVEL;\
	\
		}\
		else\
		{\
			sZOffsetX = -1;\
			sZOffsetY = -1;\
			sWorldY = GetMapXYWorldY( (sMapX + sZOffsetX), (sMapY + sZOffsetY) );\
	\
		sWorldY += 20;\
\
			sZLevel=( ( sWorldY ) * Z_SUBLAYERS)+LAND_Z_LEVEL;\
		}\
	}\
	else if ( uiLevelNodeFlags & LEVELNODE_PHYSICSOBJECT )\
	{\
		sWorldY += pNode->sRelativeZ;\
 \
		sZLevel=(sWorldY*Z_SUBLAYERS)+ONROOF_Z_LEVEL;\
\
	}\
	else if ( uiLevelNodeFlags & LEVELNODE_ITEM )\
	{\
		if ( pNode->pItemPool->bRenderZHeightAboveLevel > 0 )\
		{\
			sZLevel=(sWorldY*Z_SUBLAYERS)+STRUCT_Z_LEVEL;\
			sZLevel+=( pNode->pItemPool->bRenderZHeightAboveLevel );\
		}\
		else\
		{\
			sZLevel=(sWorldY*Z_SUBLAYERS)+OBJECT_Z_LEVEL;\
		}\
	}\
	else if ( uiAniTileFlags & ANITILE_SMOKE_EFFECT )\
	{\
		sZLevel=(sWorldY*Z_SUBLAYERS)+OBJECT_Z_LEVEL;\
	}\
	else if ( ( uiLevelNodeFlags & LEVELNODE_USEZ ) )\
	{\
\
		if ( ( uiLevelNodeFlags & LEVELNODE_NOZBLITTER ) )\
		{\
			sWorldY += 40;\
		}\
		else\
		{\
			sWorldY += pNode->sRelativeZ;\
		}\
\
		sZLevel=(sWorldY*Z_SUBLAYERS)+ONROOF_Z_LEVEL;\
	}\
	else\
	{\
		sZLevel=(sWorldY*Z_SUBLAYERS)+STRUCT_Z_LEVEL;\
	}\
\
}


#define SoldierZLevel( pSoldier, sMapX, sMapY )\
{\
\
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE ) )\
	{\
\
		if ( pNode->pStructureData != NULL )\
		{\
			sZOffsetX = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetX;\
			sZOffsetY = pNode->pStructureData->pDBStructureRef->pDBStructure->bZTileOffsetY;\
		}\
\
		sWorldY = GetMapXYWorldY( (sMapX + sZOffsetX), (sMapY + sZOffsetY) );\
\
	}\
	else\
	{\
		sWorldY = GetMapXYWorldY( sMapX, sMapY );\
	}\
\
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )\
	{\
		sZLevel=(sWorldY*Z_SUBLAYERS)+STRUCT_Z_LEVEL;\
	}\
	else\
	{\
		if ( pSoldier->aiData.dHeightAdjustment > 0 )\
		{\
			sWorldY += ( WALL_HEIGHT + 20 );\
\
			sZLevel=(sWorldY*Z_SUBLAYERS)+ONROOF_Z_LEVEL;\
		}\
		else\
		{\
			if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )\
			{\
				sZLevel=(sWorldY*Z_SUBLAYERS)+MERC_Z_LEVEL;\
			}\
			else\
			{\
				sZLevel=(sWorldY*Z_SUBLAYERS)+MERC_Z_LEVEL;\
			}\
		}\
	\
		if ( pSoldier->sZLevelOverride != -1 )\
		{\
			sZLevel = pSoldier->sZLevelOverride;\
		}\
	\
		if ( gsForceSoldierZLevel != 0 )\
		{\
			sZLevel = gsForceSoldierZLevel;\
		}\
	}\
\
}
