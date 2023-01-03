#ifndef __VSURFACE_PRIVATE_
#define __VSURFACE_PRIVATE_

// ***********************************************************************
// 
// PRIVATE, INTERNAL Header used by other SGP Internal modules
//
// Allows direct access to underlying Direct Draw Implementation
//
// ***********************************************************************

LPDIRECTDRAWSURFACE2 GetVideoSurfaceDDSurface( HVSURFACE hVSurface );
LPDIRECTDRAWSURFACE	GetVideoSurfaceDDSurfaceOne( HVSURFACE hVSurface );
LPDIRECTDRAWPALETTE	GetVideoSurfaceDDPalette( HVSURFACE hVSurface );

HVSURFACE CreateVideoSurfaceFromDDSurface( LPDIRECTDRAWSURFACE2 lpDDSurface );

#endif