#ifndef _TILE_SURFACE_H
#define _TILE_SURFACE_H

#include "worlddef.h"

extern TILE_IMAGERY		*gTileSurfaceArray[ NUMBEROFTILETYPES ];
extern UINT8			gbDefaultSurfaceUsed[ NUMBEROFTILETYPES ];
extern UINT8			gbSameAsDefaultSurfaceUsed[ NUMBEROFTILETYPES ];

TILE_IMAGERY *LoadTileSurface(  char * cFilename );

void DeleteTileSurface( PTILE_IMAGERY	pTileSurf );

void SetRaisedObjectFlag( char *cFilename, TILE_IMAGERY *pTileSurf );


#endif