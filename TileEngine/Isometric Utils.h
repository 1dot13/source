#ifndef __ISOMETRIC_UTILSH
#define __ISOMETRIC_UTILSH

#include "worlddef.h"

//DEFINES
#define MAXCOL					WORLD_COLS
#define MAXROW					WORLD_ROWS
#define GRIDSIZE		(MAXCOL * MAXROW)
#define RIGHTMOSTGRID	(MAXCOL - 1)
#define LASTROWSTART	(GRIDSIZE - MAXCOL)
//SB: NOWHERE must be constant
//#define NOWHERE         (GRIDSIZE + 1) //Lalien: old definition, replaced with -1
//#define NOWHERE         MAXLONG
#define NOWHERE         -1
//#define	NO_MAP_POS			NOWHERE //Lalien: replaced with NOWHERE
#define	MAX_MAP_POS			(GRIDSIZE)  //MAX_MAP_POS will be used only to track the changes made for the big map project, should be replaced with GRIDSIZE later

//#define MAPWIDTH			(WORLD_COLS)	//Lalien: replaced with WORLD_COLS
//#define MAPHEIGHT			(WORLD_ROWS)	//Lalien: replaced with WORLD_ROWS
//#define MAPLENGTH			(MAPHEIGHT*MAPWIDTH) //Lalien: replaced with WORLD_MAX


#define	ADJUST_Y_FOR_HEIGHT( pos, y )				( y -= gpWorldLevelData[ pos ].sHeight )					


extern UINT8 gOppositeDirection[ NUM_WORLD_DIRECTIONS ];
extern UINT8 gTwoCCDirection[ NUM_WORLD_DIRECTIONS ];
extern UINT8 gTwoCDirection[ NUM_WORLD_DIRECTIONS ];
extern UINT8 gOneCDirection[ NUM_WORLD_DIRECTIONS ];
extern UINT8 gOneCCDirection[ NUM_WORLD_DIRECTIONS ];

extern UINT8 gPurpendicularDirection[ NUM_WORLD_DIRECTIONS ][ NUM_WORLD_DIRECTIONS ];

// Macros


//												|Check for map bounds------------------------------------------|	|Invalid-|	|Valid-------------------|
#define MAPROWCOLTOPOS( r, c )									( ( (r < 0) || (r >= WORLD_ROWS) || (c < 0) || (c >= WORLD_COLS) ) ? ( 0xFFFFFFFF ) : ( (INT32)(r) * WORLD_COLS + (c) ) )

#define GETWORLDINDEXFROMWORLDCOORDS( y, x )		( (INT32) ( x / CELL_X_SIZE ) ) + WORLD_COLS * ( (INT32) ( y / CELL_Y_SIZE ) ) 

void ConvertGridNoToXY( INT32 sGridNo, INT16 *sXPos, INT16 *sYPos );
void ConvertGridNoToCellXY( INT32 sGridNo, INT16 *sXPos, INT16 *sYPos );
void ConvertGridNoToCenterCellXY( INT32 sGridNo, INT16 *sXPos, INT16 *sYPos );


// GRID NO MANIPULATION FUNCTIONS
INT32 NewGridNo(INT32 sGridNo, INT16 sDirInc);
INT16 DirectionInc(UINT8 ubDirection);
INT32 OutOfBounds(INT32 sGridNo, INT32 sProposedGridNo);
BOOLEAN TileIsOutOfBounds(INT32 sGridNo);

 
// Functions
BOOLEAN GetMouseCell( INT32 *piMouseMapPos );
BOOLEAN GetMouseXY( INT16 *psMouseX, INT16 *psMouseY );
BOOLEAN GetMouseWorldCoords( INT16 *psMouseX, INT16 *psMouseY );
BOOLEAN GetMouseMapPos( INT32	*psMapPos );
BOOLEAN GetMouseWorldCoordsInCenter( INT16 *psMouseX, INT16 *psMouseY );
BOOLEAN GetMouseXYWithRemainder( INT16 *psMouseX, INT16 *psMouseY, INT16 *psCellX, INT16 *psCellY );



void GetScreenXYWorldCoords( INT16 sScreenX, INT16 sScreenY, INT16 *pWorldX, INT16 *psWorldY );
void GetScreenXYWorldCell( INT16 sScreenX, INT16 sScreenY, INT16 *psWorldCellX, INT16 *psWorldCellY );
void GetScreenXYGridNo( INT16 sScreenX, INT16 sScreenY, INT32	*psMapPos );
void GetWorldXYAbsoluteScreenXY( INT32 sWorldCellX, INT32 sWorldCellY, INT16 *psWorldScreenX, INT16 *psWorldScreenY );
void GetFromAbsoluteScreenXYWorldXY( INT32 *psWorldCellX, INT32* psWorldCellY, INT16 sWorldScreenX, INT16 sWorldScreenY );


void FromCellToScreenCoordinates( INT16 sCellX, INT16 sCellY, INT16 *psScreenX, INT16 *psScreenY );
void FromScreenToCellCoordinates( INT16 sScreenX, INT16 sScreenY, INT16 *psCellX, INT16 *psCellY );

// Higher resolution convertion functions
void FloatFromCellToScreenCoordinates( FLOAT dCellX, FLOAT dCellY, FLOAT *pdScreenX, FLOAT *pdScreenY );
void FloatFromScreenToCellCoordinates( FLOAT dScreenX, FLOAT dScreenY, FLOAT *pdCellX, FLOAT *pdCellY );

BOOLEAN GridNoOnVisibleWorldTile( INT32 sGridNo );
BOOLEAN GridNoOnVisibleWorldTileGivenYLimits( INT32 sGridNo );
BOOLEAN GridNoOnEdgeOfMap( INT32 sGridNo, INT8 * pbDirection );

BOOLEAN ConvertMapPosToWorldTileCenter( INT32 usMapPos, INT16 *psXPos, INT16 *psYPos );

BOOLEAN CellXYToScreenXY(INT16 sCellX, INT16 sCellY, INT16 *sScreenX, INT16 *sScreenY);

INT32 GetRangeFromGridNoDiff( INT32 sGridNo1, INT32 sGridNo2 );
INT32 GetRangeInCellCoordsFromGridNoDiff( INT32 sGridNo1, INT32 sGridNo2 );

BOOLEAN IsPointInScreenRect( INT16 sXPos, INT16 sYPos, SGPRect *pRect );
BOOLEAN IsPointInScreenRectWithRelative( INT16 sXPos, INT16 sYPos, SGPRect *pRect, INT16 *sXRel, INT16 *sRelY );


INT16 PythSpacesAway(INT32 sOrigin, INT32 sDest);
INT16 SpacesAway(INT32 sOrigin, INT32 sDest);
INT16 CardinalSpacesAway(INT32 sOrigin, INT32 sDest);
INT8 FindNumTurnsBetweenDirs( INT8 sDir1, INT8 sDir2 );
BOOLEAN FindHeigherLevel( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );
BOOLEAN FindLowerLevel( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );
	
INT16 QuickestDirection(INT16 origin, INT16 dest);
INT16 ExtQuickestDirection(INT16 origin, INT16 dest);


// Returns the (center ) cell coordinates in X
INT16 CenterX( INT32 sGridNo );

// Returns the (center ) cell coordinates in Y
INT16 CenterY( INT32 sGridNo );

INT16 MapX( INT32 sGridNo );
INT16 MapY( INT32 sGridNo );
BOOLEAN FindFenceJumpDirection( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );

//Simply chooses a random gridno within valid boundaries (for dropping things in unloaded sectors)
INT32 RandomGridNo();

extern UINT32 guiForceRefreshMousePositionCalculation;


//ADB I'm tired of seeing a 5 digit number when looking at something's gridno.
//I need to see an x and y.	I created this class for the AStar,
//but moved it here as you can convert a regular INT16 gridno to a GridNode then print it out for debugging.
//Don't switch between the 2 types too often, IntToGridNode is especially slow
//0verhaul:  It's not good to give the computer extra work just for readability's sake.  So this should be a 
//good compromise.  Possibly even more useful.  Just add a watch variable for GridNode.MapXY[ mygridvar ] and
//see its X,Y in the watch window.

class GridNode
{
public:
	// WANNE - BMP: DONE!
	//typedef GridNode MapXY_t[WORLD_MAX];
	typedef GridNode MapXY_t[MAX_ALLOWED_WORLD_MAX];
	static MapXY_t MapXY;

	INT16		x;
	INT16		y;

	static MapXY_t *initGridNodes() { for (INT32 i=0; i<WORLD_MAX; i++){ConvertGridNoToXY(i, &MapXY[i].x, &MapXY[i].y); } return &MapXY; };
};

//Legion by JAzz	
BOOLEAN FindHeigherLevelOkno( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );
BOOLEAN FindHeigherLevelFence( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );
BOOLEAN FindFenceDirection( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );
BOOLEAN FindLowerLevelFence( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection ); 
BOOLEAN FindWindowJumpDirection( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bStartingDir, INT8 *pbDirection );


#endif