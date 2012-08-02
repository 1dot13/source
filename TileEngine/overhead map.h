#ifndef __OVERHEADMAP_H
#define __OVERHEADMAP_H


void InitNewOverheadDB( UINT8 ubTilesetID );
void RenderOverheadMap( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS, BOOLEAN fFromMapUtility );


void HandleOverheadMap( );
BOOLEAN InOverheadMap( );
void GoIntoOverheadMap( );
void HandleOverheadUI( );
void KillOverheadMap();

void ClickOverheadRegionCallback( MOUSE_REGION *reg, INT32 reason );
void MoveInOverheadRegionCallback( MOUSE_REGION *reg, INT32 reason );

void CalculateRestrictedMapCoords( INT8 bDirection, INT16 *psX1, INT16 *psY1, INT16 *psX2, INT16 *psY2, INT16 sEndXS, INT16 sEndYS );
void CalculateRestrictedScaleFactors( INT16 *pScaleX, INT16 *pScaleY );

void TrashOverheadMap( );

//dnl ch45 031009
void ScrollOverheadMap(void);
void ResetScrollOverheadMap(void);

// WANNE - BMP: I THINK THIS NEEDS TO CHANGE FOR BIG MAPS!
// OK, these are values that are calculated in InitRenderParams( ) with normal view settings.
// These would be different if we change ANYTHING about the game worlkd map sizes...
#define	NORMAL_MAP_SCREEN_WIDTH		3160
#define	NORMAL_MAP_SCREEN_HEIGHT	1540
#define	NORMAL_MAP_SCREEN_X			1580
#define	NORMAL_MAP_SCREEN_BY		2400
#define	NORMAL_MAP_SCREEN_TY		860

#define FASTMAPROWCOLTOPOS( r, c )		( (r) * WORLD_COLS + (c) )

extern BOOLEAN gfUseBiggerOverview;
#endif
