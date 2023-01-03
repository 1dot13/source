#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __SMOOTH_H
#define __SMOOTH_H

#define ONELEVELTYPEONEROOF		1
#define ONELEVELTYPETWOROOF			2


//	Area (pointer to SGP rect) +							
//		Location to check-+--|	|		|---- Check left and right edges -----|	|---- Check top and bottom edges -----|
#define IsLocationInArea( x, y, r )		( ((x) >= r->iLeft) && ((x) <= r->iRight) && ((y) >= r->iTop) && ((y) <= r->iBottom) )


void SmoothAllTerrainWorld( void );
void SmoothTerrain(INT32 GridNo, INT32 origType, UINT16 *piNewTile, BOOLEAN fForceSmooth  );

void SmoothTerrainRadius( INT32 iMapIndex, UINT32 uiCheckType, UINT8 ubRadius, BOOLEAN fForceSmooth );
void SmoothTerrainWorld( UINT32 uiCheckType );
void SmoothWaterTerrain( INT32 GridNo, INT32 origType, UINT16 *piNewTile, BOOLEAN fForceSmooth );
void SmoothAllTerrainTypeRadius( INT32 iMapIndex, UINT8 ubRadius, BOOLEAN fForceSmooth );

void SmoothExitGrid(INT32 GridNo, UINT16 *piNewTile, BOOLEAN fForceSmooth );
void SmoothExitGridRadius( INT32 sMapIndex, UINT8 ubRadius );

#endif
#endif



 


