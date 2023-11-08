#include "builddefines.h"
///////////////////////////
// C file include here
#include "Render Z.h"
///////////////////////////

	#include "renderworld.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "debug.h"
	#include "wcheck.h"
	#include "worldman.h"
	#include "Radar Screen.h"
	#include "Render Dirty.h"
	#include "ai.h"
	#include "render fun.h"
	#include "interactive tiles.h"
	#include "tile cache.h"
	#include "English.h"
	#include "interface control.h"
	#include "Sound Control.h"
	#include "LogicalBodyTypes/Layers.h"
	#include "LogicalBodyTypes/BodyTypeDB.h"

#include "Utilities.h"

UINT32 guiShieldGraphic = 0;
BOOLEAN fShieldGraphicInit = FALSE;
#define WALLDAMAGEGRAPHICS_MAX		2
UINT32 guiWallDamageGraphic[WALLDAMAGEGRAPHICS_MAX] = { 0 };
BOOLEAN fWallDamageGraphicInit[WALLDAMAGEGRAPHICS_MAX] = { FALSE };
UINT32 guiWallBloodGraphic = 0;
BOOLEAN fWallBloodGraphicInit = FALSE;

extern	CHAR8	gDebugStr[128];
extern	BOOLEAN fLandLayerDirty	= TRUE;

extern	INT16	gsVIEWPORT_START_X;
extern	INT16	gsVIEWPORT_START_Y;
extern	INT16	gsVIEWPORT_END_Y;
extern	INT16	gsVIEWPORT_WINDOW_END_Y;
extern	INT16	gsVIEWPORT_WINDOW_START_Y;
extern	INT16	gsVIEWPORT_END_X;

UINT16	*gpZBuffer				= NULL;
BOOLEAN gfTagAnimatedTiles		= TRUE;

INT16	gsCurrentGlowFrame		= 0;
INT16	gsCurrentItemGlowFrame	= 0;

extern	BOOLEAN	gfUIShowExitEast;
extern	BOOLEAN	gfUIShowExitWest;
extern	BOOLEAN	gfUIShowExitNorth;
extern	BOOLEAN	gfUIShowExitSouth;
extern	BOOLEAN	gfTopMessageDirty;

// VIEWPORT OFFSET VALUES
// NOTE:
// THESE VALUES MUST BE MULTIPLES OF TILE SIZES!
#define	VIEWPORT_XOFFSET_S					WORLD_TILE_X*1
#define	VIEWPORT_YOFFSET_S					WORLD_TILE_Y*2
#define LARGER_VIEWPORT_XOFFSET_S			( VIEWPORT_XOFFSET_S * 3 )
#define LARGER_VIEWPORT_YOFFSET_S			( VIEWPORT_YOFFSET_S * 5 )

#define TILES_TYPE_BITMASK					0x00040000
#define TILES_TYPE_MASK						0x07ffffff
#define TILES_DIRTY							0x80000000
#define TILES_DYNAMIC						0x40000000
#define TILES_NOZWRITE						0x20000000
#define TILES_MARKED						0x10000000
#define TILES_NOZ							0x04000000
#define TILES_DOALL							0x02000000
#define TILES_OBSCURED						0x01000000

//#define TILES_MERC						0x00000400
//#define TILES_Z_BLITTER					0x00000200
//#define TILES_Z_WRITE						0x00000100
//#define TILES_SHADOW						0x00000080
//#define TILES_BACKWARDS					0x00000040

#define	MAX_RENDERED_ITEMS					3

// RENDERER FLAGS FOR DIFFERENT RENDER LEVELS
enum
{
	RENDER_STATIC_LAND,						RENDER_STATIC_OBJECTS,
	RENDER_STATIC_SHADOWS,					RENDER_STATIC_STRUCTS,
	RENDER_STATIC_ROOF,						RENDER_STATIC_ONROOF,
	RENDER_STATIC_TOPMOST,					RENDER_DYNAMIC_LAND,
	RENDER_DYNAMIC_OBJECTS,					RENDER_DYNAMIC_SHADOWS,
	RENDER_DYNAMIC_STRUCT_MERCS,			RENDER_DYNAMIC_MERCS,
	RENDER_DYNAMIC_STRUCTS,					RENDER_DYNAMIC_ROOF,
	RENDER_DYNAMIC_HIGHMERCS,				RENDER_DYNAMIC_ONROOF,
	RENDER_DYNAMIC_TOPMOST,					NUM_RENDER_FX_TYPES
};

#define	SCROLL_INTERTIA_STEP1				3//6
#define	SCROLL_INTERTIA_STEP2				4//8

//#define SHORT_ROUND( x ) ( (INT16)( ( x * 1000 ) / 1000 ) )
#define SHORT_ROUND( x ) ( x )

#define	NUM_ITEM_CYCLE_COLORS				60

UINT16	us16BPPItemCycleWhiteColors[ NUM_ITEM_CYCLE_COLORS ];
UINT16	us16BPPItemCycleRedColors[ NUM_ITEM_CYCLE_COLORS ];
UINT16	us16BPPItemCycleYellowColors[ NUM_ITEM_CYCLE_COLORS ];

INT16	gsLobOutline;
INT16	gsThrowOutline;
INT16	gsGiveOutline;
INT16	gusNormalItemOutlineColor;
INT16	gusYellowItemOutlineColor;
INT16	gsRenderHeight				= 0;
BOOLEAN	gfRenderFullThisFrame		= 0;

INT16 gCenterWorldX;

//UINT8		gubIntTileCheckFlags	 = INTILE_CHECK_FULL;
UINT8		gubIntTileCheckFlags	 = INTILE_CHECK_SELECTIVE;

UINT8		ubRGBItemCycleWhiteColors[] =
{
	25,		25,		25,
	50,		50,		50,
	75,		75,		75,
	100,	100,	100,
	125,	125,	125,
	150,	150,	150,
	175,	175,	175,
	200,	200,	200,
	225,	225,	225,
	250,	250,	250,

	250,	250,	250,
	225,	225,	225,
	200,	200,	200,
	175,	175,	175,
	150,	150,	150,
	125,	125,	125,
	100,	100,	100,
	75,		75,		75,
	50,		50,		50,
	25,		25,		25,

	25,		25,		25,
	50,		50,		50,
	75,		75,		75,
	100,	100,	100,
	125,	125,	125,
	150,	150,	150,
	175,	175,	175,
	200,	200,	200,
	225,	225,	225,
	250,	250,	250,

	250,	250,	250,
	225,	225,	225,
	200,	200,	200,
	175,	175,	175,
	150,	150,	150,
	125,	125,	125,
	100,	100,	100,
	75,		75,		75,
	50,		50,		50,
	25,		25,		25,

	25,		25,		25,
	50,		50,		50,
	75,		75,		75,
	100,	100,	100,
	125,	125,	125,
	150,	150,	150,
	175,	175,	175,
	200,	200,	200,
	225,	225,	225,
	250,	250,	250,

	250,	250,	250,
	225,	225,	225,
	200,	200,	200,
	175,	175,	175,
	150,	150,	150,
	125,	125,	125,
	100,	100,	100,
	75,		75,		75,
	50,		50,		50,
	25,		25,		25
};

UINT8		ubRGBItemCycleRedColors[] =
{
	25,		0,		0,
	50,		0,		0,
	75,		0,		0,
	100,	0,		0,
	125,	0,		0,
	150,	0,		0,
	175,	0,		0,
	200,	0,		0,
	225,	0,		0,
	250,	0,		0,

	250,	0,		0,
	225,	0,		0,
	200,	0,		0,
	175,	0,		0,
	150,	0,		0,
	125,	0,		0,
	100,	0,		0,
	75,		0,		0,
	50,		0,		0,
	25,		0,		0,

	25,		0,		0,
	50,		0,		0,
	75,		0,		0,
	100,	0,		0,
	125,	0,		0,
	150,	0,		0,
	175,	0,		0,
	200,	0,		0,
	225,	0,		0,
	250,	0,		0,

	250,	0,		0,
	225,	0,		0,
	200,	0,		0,
	175,	0,		0,
	150,	0,		0,
	125,	0,		0,
	100,	0,		0,
	75,		0,		0,
	50,		0,		0,
	25,		0,		0,

	25,		0,		0,
	50,		0,		0,
	75,		0,		0,
	100,	0,		0,
	125,	0,		0,
	150,	0,		0,
	175,	0,		0,
	200,	0,		0,
	225,	0,		0,
	250,	0,		0,

	250,	0,		0,
	225,	0,		0,
	200,	0,		0,
	175,	0,		0,
	150,	0,		0,
	125,	0,		0,
	100,	0,		0,
	75,		0,		0,
	50,		0,		0,
	25,		0,		0,
};

UINT8		ubRGBItemCycleYellowColors[] =
{
	25,		25,		0,
	50,		50,		0,
	75,		75,		0,
	100,	100,	0,
	125,	125,	0,
	150,	150,	0,
	175,	175,	0,
	200,	200,	0,
	225,	225,	0,
	250,	250,	0,

	250,	250,	0,
	225,	225,	0,
	200,	200,	0,
	175,	175,	0,
	150,	150,	0,
	125,	125,	0,
	100,	100,	0,
	75,		75,		0,
	50,		50,		0,
	25,		25,		0,

	25,		25,		0,
	50,		50,		0,
	75,		75,		0,
	100,	100,	0,
	125,	125,	0,
	150,	150,	0,
	175,	175,	0,
	200,	200,	0,
	225,	225,	0,
	250,	250,	0,

	250,	250,	0,
	225,	225,	0,
	200,	200,	0,
	175,	175,	0,
	150,	150,	0,
	125,	125,	0,
	100,	100,	0,
	75,		75,		0,
	50,		50,		0,
	25,		25,		0,

	25,		25,		0,
	50,		50,		0,
	75,		75,		0,
	100,	100,	0,
	125,	125,	0,
	150,	150,	0,
	175,	175,	0,
	200,	200,	0,
	225,	225,	0,
	250,	250,	0,

	250,	250,	0,
	225,	225,	0,
	200,	200,	0,
	175,	175,	0,
	150,	150,	0,
	125,	125,	0,
	100,	100,	0,
	75,		75,		0,
	50,		50,		0,
	25,		25,		0,
};

#define NUMSPEEDS		5

UINT8	gubNewScrollXSpeeds[2][ NUMSPEEDS ]	=
{
	40, 80, 100, 180, 200,		// Non-video mode scroll
	20, 40, 80, 80, 80			// Video mode scroll
};

UINT8	gubNewScrollYSpeeds[2][ NUMSPEEDS ]	=
{
	40, 80, 100, 180, 200,		// Non-video mode scroll
	10, 20, 60, 80, 80			// Video mode scroll
};

// These speeds are only an indication of how long to do each subtile step until moving on to another
UINT8	gubNewScrollIDSpeeds[ ]		= { 10, 10, 20, 20,  20 };
UINT8	gubScrollSpeedStartID		= 2;
UINT8	gubScrollSpeedEndID			= 4;
UINT8	gubCurScrollSpeedID			= 1;
BOOLEAN	gfDoVideoScroll				= TRUE;
BOOLEAN	gfDoSubtileScroll			= FALSE;
BOOLEAN	gfScrollPending				= FALSE;
UINT32	uiLayerUsedFlags			= 0xffffffff;
UINT32	uiAdditiveLayerUsedFlags	= 0xffffffff;

// Array of shade values to use.....
#define NUM_GLOW_FRAMES				30


INT16	gsGlowFrames[] =
{
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	2,	4,	6,	8,	9,	8,	6,	4,	2,	0,
};

// This has to be the same # of frames as NUM_GLOW_FRAMES
INT16	gsFastGlowFrames[] =
{
	0,	0,	6,	7,	8,	9,	8,	7,	6,	5,
	0,	0,	6,	7,	8,	9,	8,	7,	6,	5,
	0,	0,	6,	7,	8,	9,	8,	7,	6,	5,
};

// The glow frame pointer can be adjusted to use a faster/ slower glow
INT16	*gpGlowFramePointer	= gsGlowFrames;

UINT32	gScrollDirectionFlags[ NUM_WORLD_DIRECTIONS ] =
{
	SCROLL_UP | SCROLL_RIGHT,
	SCROLL_RIGHT,
	SCROLL_DOWN | SCROLL_RIGHT,
	SCROLL_DOWN,
	SCROLL_DOWN | SCROLL_LEFT,
	SCROLL_LEFT,
	SCROLL_UP | SCROLL_LEFT,
	SCROLL_UP,
};

INT16 SCROLL_X_STEP	 =		( WORLD_TILE_X );
INT16 SCROLL_Y_STEP	 =		( WORLD_TILE_Y * 2);

/*
 *	ViewPort starting values
 *	What is viewport?
 *	So viewport are coords of map screen (tactical screen)
 *	gsViewport_xxx this variables are related with mouse regions, current rendering region ...
 *	this variables ware unnecessary with this GUI becouse they can be calculated from INTERFACE coords
 *	but i think that they can be realy usefull while we will change GUI for more customisable
 *  any questions? joker
 */
/*
INT16 gsVIEWPORT_START_X				= 0;
INT16 gsVIEWPORT_START_Y				= 0;
INT16 gsVIEWPORT_WINDOW_START_Y			= 0;
INT16 gsVIEWPORT_WINDOW_END_Y			= INTERFACE_START_Y;//do zmiany
INT16 gsVIEWPORT_END_X					= SCREEN_WIDTH;
INT16 gsVIEWPORT_END_Y					= INTERFACE_START_Y;

i changed for dynamic initialization joker
*/
// still need the global object container defined atleast once outside a header (jonathanl)
INT16 gsVIEWPORT_START_X					= 0;
INT16 gsVIEWPORT_START_Y				= 0;
INT16 gsVIEWPORT_WINDOW_START_Y			= 0;
INT16 gsVIEWPORT_WINDOW_END_Y			= INTERFACE_START_Y;
INT16 gsVIEWPORT_END_X					= SCREEN_WIDTH;
INT16 gsVIEWPORT_END_Y					= INTERFACE_START_Y;

INT16 gsTopLeftWorldX, gsTopLeftWorldY;
INT16 gsTopRightWorldX, gsTopRightWorldY;
INT16 gsBottomLeftWorldX, gsBottomLeftWorldY;
INT16 gsBottomRightWorldX, gsBottomRightWorldY;
BOOLEAN gfIgnoreScrolling = FALSE;

// WANNE: If we are talking?
// This check is used, to prevent scrolling in small maps (e.g: Rebel Basement) in higher resolution (1024x768) [2007-05-14]
BOOLEAN gfDialogControl = FALSE;

BOOLEAN	gfIgnoreScrollDueToCenterAdjust = FALSE;

// GLOBAL SCROLLING PARAMS
INT16	gTopLeftWorldLimitX,		gTopLeftWorldLimitY;
INT16	gTopRightWorldLimitX,		gTopRightWorldLimitY;
INT16	gBottomLeftWorldLimitX,		gBottomLeftWorldLimitY;
INT16	gBottomRightWorldLimitX,	gBottomRightWorldLimitY;
INT16	Slide,						gCenterWorldY;
INT16	gsTLX,		gsTLY,			gsTRX,		gsTRY;
INT16	gsBLX,		gsBLY,			gsBRX,		gsBRY;
INT16	gsCX,		gsCY;
DOUBLE	gdScaleX,	gdScaleY;

#define FASTMAPROWCOLTOPOS( r, c )		( (r) * WORLD_COLS + (c) )

BOOLEAN	gfScrollInertia = FALSE;

// GLOBALS FOR CALCULATING STARTING PARAMETERS
INT16	gsStartPointX_W,	gsStartPointY_W;
INT16	gsStartPointX_S,	gsStartPointY_S;
INT16	gsStartPointX_M,	gsStartPointY_M;
INT16	gsEndXS,			gsEndYS;
// LARGER OFFSET VERSION FOR GIVEN LAYERS
INT16	gsLStartPointX_W,	gsLStartPointY_W;
INT16	gsLStartPointX_S,	gsLStartPointY_S;
INT16	gsLStartPointX_M,	gsLStartPointY_M;
INT16	gsLEndXS,			gsLEndYS;

BOOLEAN	gfRenderScroll = FALSE;
BOOLEAN	gfScrollStart  = FALSE;
INT16	gsScrollXIncrement;
INT16	gsScrollYIncrement;
INT32	guiScrollDirection;

// Rendering flags (full, partial, etc.)
UINT32		gRenderFlags=0;

/*  You know it was static and now it will be dynamic :P
 *  any questions? joker
 */
SGPRect		gClippingRect;//			= { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - INTERFACE_HEIGHT};
//SGPRect		gOldClipRect = { 0, 0, 2560, 1600 };
SGPRect		gOldClipRect = { 0, 0, 0, 0 };
INT16		gsRenderCenterX;
INT16		gsRenderCenterY;
INT16		gsRenderWorldOffsetX	= 0;  //lal was -1 : bugfix for merc screen position in tactical on high resolution
INT16		gsRenderWorldOffsetY	= 10; //lal was -1
SGPRect		gSelectRegion;
UINT32		fSelectMode				= NO_SELECT;
SGPPoint	gSelectAnchor;

typedef struct
{
	BOOLEAN	fDynamic;
	BOOLEAN	fZWrite;
	BOOLEAN	fZBlitter;
	BOOLEAN fShadowBlitter;
	BOOLEAN fLinkedListDirection;
	BOOLEAN fMerc;
	BOOLEAN	fCheckForRedundency;
	BOOLEAN	fMultiZBlitter;
	BOOLEAN fConvertTo16;
	BOOLEAN	fObscured;
} RenderFXType;

RenderFXType RenderFX[] =
{
	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE, 	// STATIC LAND
	FALSE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	// STATIC OBJECTS
	FALSE,	TRUE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// STATIC SHADOWS
	FALSE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	// STATIC STRUCTS
	FALSE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// STATIC ROOF
	FALSE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	// STATIC ONROOF
	FALSE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// STATIC TOPMOST
	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	// DYNAMIC LAND
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	// DYNAMIC OBJECT
	TRUE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC SHADOW
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC STRUCT MERCS
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC MERCS
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC STRUCT
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC ROOF
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC HIGHMERCS
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	// DYNAMIC ONROOF
	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE	// DYNAMIC TOPMOST
};

UINT8 RenderFXStartIndex[] =
{
	LAND_START_INDEX,		// STATIC LAND
	OBJECT_START_INDEX,		// STATIC OBJECTS
	SHADOW_START_INDEX,		// STATIC SHADOWS
	STRUCT_START_INDEX,		// STATIC STRUCTS
	ROOF_START_INDEX,		// STATIC ROOF
	ONROOF_START_INDEX,		// STATIC ONROOF
	TOPMOST_START_INDEX,	// STATIC TOPMOST
	LAND_START_INDEX,		// DYNAMIC LAND
	OBJECT_START_INDEX,		// DYNAMIC OBJECT
	SHADOW_START_INDEX,		// DYNAMIC SHADOW
	MERC_START_INDEX,		// DYNAMIC STRUCT MERCS
	MERC_START_INDEX,		// DYNAMIC MERCS
	STRUCT_START_INDEX,		// DYNAMIC STRUCT
	ROOF_START_INDEX,		// DYNAMIC ROOF
	MERC_START_INDEX,		// DYNAMIC HIGHMERCS
	ONROOF_START_INDEX,		// DYNAMIC ONROOF
	TOPMOST_START_INDEX,	// DYNAMIC TOPMOST
};


//INT16 gsCoordArray[ 500 ][ 500 ][ 4 ];
//INT16 gsCoordArrayX;
//INT16 gsCoordArrayY;
//void SetRenderGlobals( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
//void TempRenderTiles(UINT32 uiFlags, INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
//void TempRenderTiles(UINT32 uiFlags, INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS, UINT8 ubNumLevels, UINT32 *puiLevels );

void ExamineZBufferForHiddenTiles( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );

//void ReRenderWorld(INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom);
void ClearMarkedTiles(void);
void CorrectRenderCenter( INT16 sRenderX, INT16 sRenderY, INT16 *pSNewX, INT16 *pSNewY );
void ScrollBackground(UINT32 uiDirection, INT16 sScrollXIncrement, INT16 sScrollYIncrement );
void CalcRenderParameters(INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom );
void ResetRenderParameters(  );


void RenderRoomInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );


#ifdef _DEBUG
//extern UINT8 gubFOVDebugInfoInfo[ WORLD_MAX ];
//extern UINT8 gubGridNoMarkers[ WORLD_MAX ];
extern UINT8 * gubFOVDebugInfoInfo;
extern UINT8 * gubGridNoMarkers;
extern UINT8 gubGridNoValue;
extern BOOLEAN gfDisplayCoverValues;
extern BOOLEAN gfDisplayGridNoVisibleValues = 0;
//extern INT16	gsCoverValue[ WORLD_MAX ];
extern INT16 *	gsCoverValue;
extern INT16	gsBestCover;
void RenderFOVDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
void RenderCoverDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
void RenderGridNoVisibleDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
#endif

void DeleteFromWorld( UINT16 usTileIndex, UINT32 uiRenderTiles, UINT16 usIndex );

void RenderHighlight( INT16 sMouseX_M, INT16 sMouseY_M, INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS );
BOOLEAN CheckRenderCenter( INT16 sNewCenterX, INT16 sNewCenterY );

// Flugente: display a riot shield
void ShowRiotShield( SOLDIERTYPE* pSoldier, UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue )
{
	if (pSoldier)
	{
		if (!fShieldGraphicInit)
		{
			VOBJECT_DESC	VObjectDesc;

			// Flugente: enemy role symbols
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			FilenameForBPP("Tilecache\\riotshield.sti", VObjectDesc.ImageFile);
			if (!AddVideoObject(&VObjectDesc, &guiShieldGraphic))
				AssertMsg(0, "Missing Tilecache\\riotshield.sti");

			fShieldGraphicInit = TRUE;
		}
		
		HVOBJECT hSrcVObject;
		if ( !GetVideoObject( &hSrcVObject, guiShieldGraphic ) )
			return;

		OBJECTTYPE* pObj = pSoldier->GetEquippedRiotShield();

		if ( !pObj )
			return;

		UINT16 offset = Item[pObj->usItem].usRiotShieldGraphic;

		// Get screen pos of gridno......
		INT16					sScreenX, sScreenY;
		GetGridNoScreenXY(pSoldier->sGridNo, &sScreenX, &sScreenY);

		// take height level into account
		if ( pSoldier->pathing.bLevel == 1 )
			sScreenY -= 50;

		// try to keep the shield 'moving' alongside the soldier. This won't work perfectly, but it's better than nothing		
		INT16 base_x = 0;
		INT16 base_y = 0;
		ConvertGridNoToCenterCellXY( pSoldier->sGridNo, &base_x, &base_y );

		INT16 dx = pSoldier->sX - base_x;
		INT16 dy = pSoldier->sY - base_y;

		INT16 offset_x = (dx - dy);
		INT16 offset_y = (dx + dy);

		// redraw background to stop weird graphic remnants remaining
		// but don't do so while scrolling, because that looks weird
		if ( !gfScrollPending && !gfScrollInertia )
		{
			// We can get graphical glitches here if we reserve too many background rectangles, as the entire array will be filled rapidly.
			// To prevent this, each soldier can have their own reserved rectangle, which we free first
			static INT32 soldierbackgroundrectangle[TOTAL_SOLDIERS] = { 0 };

			if ( soldierbackgroundrectangle[pSoldier->ubID] != 0 && soldierbackgroundrectangle[pSoldier->ubID] != -1 )
			{
				FreeBackgroundRect( soldierbackgroundrectangle[pSoldier->ubID] );
				soldierbackgroundrectangle[pSoldier->ubID] = 0;
			}

			soldierbackgroundrectangle[pSoldier->ubID] = RegisterBackgroundRect( BGND_FLAG_ANIMATED, NULL, sScreenX - 50, sScreenY - 60, sScreenX + 50, sScreenY + 35 );

			if ( soldierbackgroundrectangle[pSoldier->ubID] != -1 )
			{
				SetBackgroundRectFilled( soldierbackgroundrectangle[pSoldier->ubID] );
			}
		}
		
		Blt8BPPDataTo16BPPBufferTransZNB( pBuffer, uiDestPitchBYTES, pZBuffer, usZValue, hSrcVObject, sScreenX - 20 + offset_x, sScreenY - 60 + offset_y, offset * 8 + pSoldier->ubDirection );
	}
}

// We can get graphical glitches here if we reserve too many background rectangles, as the entire array will be filled rapidly.
// Additionally we get graphic glitches if we never free these background rectangles.
// To prevent this, we reserve a limited amount and free them at the beginning of every rendering
#define MAX_DECALS_ONSCREEN		200
INT32 gDecalBackgroundRectangle[MAX_DECALS_ONSCREEN] = { 0 };
int gDecalBackgroundRectableCounter = 0;

void ClearBackgroundRectanglesForDecal()
{
	for ( int i = 0; i < gDecalBackgroundRectableCounter; ++i )
	{
		if (gDecalBackgroundRectangle[i] != -1 )
		{
			FreeBackgroundRect( gDecalBackgroundRectangle[i] );
			gDecalBackgroundRectangle[i] = -1;
		}
	}

	gDecalBackgroundRectableCounter = 0;
}

bool SetupBackgroundRectanglesForDecal( UINT32 uiFlags, INT16 *pSaveArea, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom )
{
	if ( gDecalBackgroundRectableCounter < MAX_DECALS_ONSCREEN )
	{
		gDecalBackgroundRectangle[gDecalBackgroundRectableCounter] = RegisterBackgroundRect( uiFlags, pSaveArea, sLeft, sTop, sRight, sBottom );
		
		if ( gDecalBackgroundRectangle[gDecalBackgroundRectableCounter] != -1 )
		{
			SetBackgroundRectFilled( gDecalBackgroundRectangle[gDecalBackgroundRectableCounter] );

			++gDecalBackgroundRectableCounter;

			return true;
		}
	}

	return false;
}

// Flugente: display decal
void ShowDecal( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, INT32 sGridNo )
{
	// we mark locations with decals with the 'DAMAGED' flag for easier filtering
	if ( gGameExternalOptions.fAdditionalDecals 
		&& gGameSettings.fOptions[TOPTION_BLOOD_N_GORE]
		&& !TileIsOutOfBounds( sGridNo )
		&& ( gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_STRUCTURE_DAMAGED ) )
	{
		for ( int i = 0; i < WALLDAMAGEGRAPHICS_MAX; ++i )
		{
			if ( !fWallDamageGraphicInit[i] )
			{
				VOBJECT_DESC	VObjectDesc;
				VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

				CHAR8	filename[128];
				sprintf( filename, "Tilecache\\walldamage%d.sti", i );

				FilenameForBPP( filename, VObjectDesc.ImageFile );
				if ( !AddVideoObject( &VObjectDesc, &guiWallDamageGraphic[i] ) )
					AssertMsg( 0, String( "Missing %s", VObjectDesc.ImageFile ) );

				fWallDamageGraphicInit[i] = TRUE;
			}
		}

		if ( !fWallBloodGraphicInit )
		{
			VOBJECT_DESC	VObjectDesc;
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			FilenameForBPP( "Tilecache\\bloodspatter_wall.sti", VObjectDesc.ImageFile );
			if ( !AddVideoObject( &VObjectDesc, &guiWallBloodGraphic ) )
				AssertMsg( 0, String( "Missing %s", VObjectDesc.ImageFile ) );

			fWallBloodGraphicInit = TRUE;
		}
		
		//  rendering while scrolling is a pain in the ass, so we skip that
		if ( gfScrollPending || gfScrollInertia )
			return;

		STRUCTURE* pStructure = FindStructure( sGridNo, STRUCTURE_WALL );
		if ( !pStructure )
			return;

		// Get screen pos of gridno......
		INT16					sScreenX, sScreenY;
		GetGridNoScreenXY( sGridNo, &sScreenX, &sScreenY );

		// For ease of use, the pics from the libraries are without an offset, we handle this here. Of course this assumes the painter starts with the proper template to begin with.
		INT32 displayX = sScreenX - WORLD_TILE_X / 2;
		INT32 displayY = sScreenY - WORLD_TILE_Y / 2 - 41;
		
		// we need to reserve a background if we draw decals, but we only need to do that once
		bool backgroundrectregistered = false;

		//loop through all the structures and add all that are damaged
		while ( pStructure )
		{
			// for now only for full walls
			if ( StructureHeight(pStructure) == 4
				&& ( pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT
					|| pStructure->ubWallOrientation == INSIDE_TOP_LEFT
					|| pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT
					|| pStructure->ubWallOrientation == INSIDE_TOP_RIGHT ))
			{
				//if the structure has been damaged
				if ( pStructure->ubHitPoints < pStructure->pDBStructureRef->pDBStructure->ubHitPoints )
				{
					int graphiclib = sGridNo % WALLDAMAGEGRAPHICS_MAX;

					HVOBJECT hSrcVObject;
					if ( GetVideoObject( &hSrcVObject, guiWallDamageGraphic[graphiclib] ) )
					{
						// redraw background to stop weird graphic remnants remaining
						// but don't do so while scrolling, because that looks weird
						if ( !backgroundrectregistered )
						{
							if ( !SetupBackgroundRectanglesForDecal( BGND_FLAG_ANIMATED, NULL, displayX - 50, displayY - 60, displayX + 50, displayY + 60 ) )
							{
								return;
							}

							backgroundrectregistered = true;
						}

						FLOAT ratio = (FLOAT)( pStructure->ubHitPoints ) / (FLOAT)( pStructure->pDBStructureRef->pDBStructure->ubHitPoints );

						// The first half of the pics are for the left side, the second half for the right side
						UINT16 picsperside = hSrcVObject->usNumberOfObjects / 2;

						UINT16 graphic = 0;
						displayX = sScreenX - WORLD_TILE_X / 2;

						switch ( pStructure->ubWallOrientation )
						{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:
						{
							graphic = ratio * picsperside;
						}
						break; 
						
						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:
						{
							graphic = ratio * picsperside + picsperside;
							displayX += 16;
						}
						break;						
						}

						if ( BltIsClippedOrOffScreen( hSrcVObject, displayX, displayY, graphic, &gClippingRect ) )
							return;

						Blt8BPPDataTo16BPPBufferTransZNB( pBuffer, uiDestPitchBYTES, pZBuffer, usZValue, hSrcVObject, displayX, displayY, graphic );
					}
				}

				if ( pStructure->ubDecalFlag & STRUCTURE_DECALFLAG_BLOOD )
				{
					HVOBJECT hSrcVObject;
					if ( GetVideoObject( &hSrcVObject, guiWallBloodGraphic ) )
					{
						// redraw background to stop weird graphic remnants remaining
						// but don't do so while scrolling, because that looks weird
						if ( !backgroundrectregistered )
						{
							if ( !SetupBackgroundRectanglesForDecal( BGND_FLAG_ANIMATED, NULL, displayX - 50, displayY - 60, displayX + 50, displayY + 60 ) )
							{
								return;
							}

							backgroundrectregistered = true;
						}

						// The first half of the pics are for the left side, the second half for the right side
						UINT16 picsperside = hSrcVObject->usNumberOfObjects / 2;

						UINT16 graphic = 0;
						displayX = sScreenX - WORLD_TILE_X / 2;

						switch ( pStructure->ubWallOrientation )
						{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:
							// It is advised that the number of pics per side does not divide the map length per side - 160 or 360 - without remainder.
							// Otherwise the spatters along a '/' wall will all be the same
							graphic = ( sGridNo ) % picsperside;
							break; 
						
						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:
							graphic = ( sGridNo ) % picsperside + picsperside;
							displayX += 16;
							break;
						}

						if ( BltIsClippedOrOffScreen( hSrcVObject, displayX, displayY, graphic, &gClippingRect ) )
							return;

						Blt8BPPDataTo16BPPBufferTransZNB( pBuffer, uiDestPitchBYTES, pZBuffer, usZValue, hSrcVObject, displayX, displayY, graphic );
					}
				}
			}

			pStructure = FindNextStructure( pStructure, STRUCTURE_WALL );
		}
	}
}


BOOLEAN RevealWalls(INT16 sX, INT16 sY, INT16 sRadius)
{
LEVELNODE *pStruct;
INT16 sCountX, sCountY;
UINT32 uiTile;
BOOLEAN fRerender=FALSE;
TILE_ELEMENT *TileElem;

	for(sCountY=sY-sRadius; sCountY < (sY+sRadius+2); sCountY++)
		for(sCountX=sX-sRadius; sCountX < (sX+sRadius+2); sCountX++)
		{
			uiTile=FASTMAPROWCOLTOPOS(sCountY, sCountX);
			pStruct=gpWorldLevelData[uiTile].pStructHead;

			while(pStruct!=NULL)
			{
				TileElem = &(gTileDatabase[pStruct->usIndex]);
				switch(TileElem->usWallOrientation)
				{
					case NO_ORIENTATION:
						break;

					case INSIDE_TOP_RIGHT:
					case OUTSIDE_TOP_RIGHT:
						if(sCountX >= sX)
						{
							pStruct->uiFlags|=LEVELNODE_REVEAL;
							fRerender=TRUE;
						}
						break;

					case INSIDE_TOP_LEFT:
					case OUTSIDE_TOP_LEFT:
						if(sCountY >= sY)
						{
							pStruct->uiFlags|=LEVELNODE_REVEAL;
							fRerender=TRUE;
						}
						break;
				}
				pStruct=pStruct->pNext;
			}
		}

/*
	if(fRerender)
		SetRenderFlags(RENDER_FLAG_FULL);
*/

	return(TRUE);
}

BOOLEAN ConcealWalls(INT16 sX, INT16 sY, INT16 sRadius)
{
LEVELNODE *pStruct;
INT16 sCountX, sCountY;
UINT32 uiTile;
BOOLEAN fRerender=FALSE;
TILE_ELEMENT *TileElem;


	for(sCountY=sY-sRadius; sCountY < (sY+sRadius+2); sCountY++)
		for(sCountX=sX-sRadius; sCountX < (sX+sRadius+2); sCountX++)
		{
			uiTile=FASTMAPROWCOLTOPOS(sCountY, sCountX);
			pStruct=gpWorldLevelData[uiTile].pStructHead;

			while(pStruct!=NULL)
			{
				TileElem = &(gTileDatabase[pStruct->usIndex]);
				switch(TileElem->usWallOrientation)
				{
					case NO_ORIENTATION:
						break;

					case INSIDE_TOP_RIGHT:
					case OUTSIDE_TOP_RIGHT:
						if(sCountX >= sX)
						{
							pStruct->uiFlags&=(~LEVELNODE_REVEAL);
							fRerender=TRUE;
						}
						break;

					case INSIDE_TOP_LEFT:
					case OUTSIDE_TOP_LEFT:
						if(sCountY >= sY)
						{
							pStruct->uiFlags&=(~LEVELNODE_REVEAL);
							fRerender=TRUE;
						}
						break;
				}
				pStruct=pStruct->pNext;
			}
		}

/*
	if(fRerender)
		SetRenderFlags(RENDER_FLAG_FULL);
*/

	return(TRUE);

}

void ConcealAllWalls(void)
{
LEVELNODE *pStruct;
INT32 uiCount;

	for(uiCount=0; uiCount < WORLD_MAX; uiCount++)
	{
		pStruct=gpWorldLevelData[uiCount].pStructHead;
		while(pStruct!=NULL)
		{
			pStruct->uiFlags&=(~LEVELNODE_REVEAL);
			pStruct=pStruct->pNext;
		}
	}
}


void ResetLayerOptimizing(void)
{
	uiLayerUsedFlags = 0xffffffff;
	uiAdditiveLayerUsedFlags = 0;
}

void ResetSpecificLayerOptimizing( UINT32 uiRowFlag )
{
	uiLayerUsedFlags |= uiRowFlag;
}


void SumAddiviveLayerOptimization( void )
{
	uiLayerUsedFlags = uiAdditiveLayerUsedFlags;
}

void SetRenderFlags(UINT32 uiFlags)
{
	gRenderFlags|=uiFlags;
}

void ClearRenderFlags(UINT32 uiFlags)
{
	gRenderFlags&=(~uiFlags);
}

UINT32 GetRenderFlags(void)
{
	return(gRenderFlags);
}

void RenderSetShadows(BOOLEAN fShadows)
{
	if(fShadows)
	{
		gRenderFlags|=RENDER_FLAG_SHADOWS;
	}
	else
	{
		gRenderFlags&=(~RENDER_FLAG_SHADOWS);
}
}

inline UINT16 * GetShadeTable(LEVELNODE * pNode, SOLDIERTYPE * pSoldier, SOLDIERTYPE * pPaletteTable, UINT32 uiFlags, INT16 * gsForceSoldierZLevel)
{
	UINT16 * pShadeTable;
	// Shade guy always lighter than scene default!
	{
		const auto GridNo = pSoldier->sGridNo;
		if (GridNo == NOWHERE)
		{
			pShadeTable = pPaletteTable->pCurrentShade = pPaletteTable->pShades[DEFAULT_SHADE_LEVEL];
			return pShadeTable;
		}
		UINT8 ubShadeLevel = gpWorldLevelData[GridNo].pLandHead->ubShadeLevel;
		// If merc is on a roof, shade according to roof brightness
		if (pSoldier->pathing.bLevel > 0 && gpWorldLevelData[GridNo].pRoofHead != NULL)
		{
			ubShadeLevel = gpWorldLevelData[GridNo].pRoofHead->ubShadeLevel;
		}

		ubShadeLevel = (ubShadeLevel & 0x0f);
		ubShadeLevel = __max(ubShadeLevel - 2, DEFAULT_SHADE_LEVEL);
		ubShadeLevel |= (ubShadeLevel & 0x30);


		if (pSoldier->flags.fBeginFade)
		{
			pShadeTable = pPaletteTable->pCurrentShade = pPaletteTable->pShades[pSoldier->ubFadeLevel];
		}
		else
		{
			pShadeTable = pPaletteTable->pCurrentShade = pPaletteTable->pShades[ubShadeLevel];
		}
	}
	if (!pSoldier->flags.fBeginFade)
	{
		// Special effect - draw ghost if is seen by a guy in player's team but not current guy
		// ATE: Todo: setup flag for 'bad-guy' - can releive some checks in renderer
		if (!pSoldier->aiData.bNeutral && (pSoldier->bSide != gbPlayerNum))
		{
			SOLDIERTYPE * pSelSoldier;
			if (gusSelectedSoldier != NOBODY)
			{
				pSelSoldier = MercPtrs[gusSelectedSoldier];
			}
			else
			{
				pSelSoldier = NULL;
			}
			INT8 bGlowShadeOffset;
			bGlowShadeOffset = 0;
			if (gTacticalStatus.ubCurrentTeam == gbPlayerNum)
			{
				// Shade differently depending on visiblity
				if (pSoldier->bLastRenderVisibleValue == 0)
				{
					bGlowShadeOffset = 10;
				}
				if (pSelSoldier != NULL)
				{
					if (pSelSoldier->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY)
					{
						if (pSoldier->usAnimState != CHARIOTS_OF_FIRE && pSoldier->usAnimState != BODYEXPLODING)
						{
							bGlowShadeOffset = 10;
						}
					}
				}
			}
			INT16 ** pShadeStart;
			if (pSoldier->pathing.bLevel == 0)
			{
				pShadeStart = (INT16 **) & (pPaletteTable->pGlowShades[0]);
			}
			else
			{
				pShadeStart = (INT16 **) & (pPaletteTable->pShades[20]);
			}
			// Set shade
			// If a bad guy is highlighted
			if (gfUIHandleSelectionAboveGuy == TRUE && MercPtrs[gsSelectedGuy]->bSide != gbPlayerNum)
			{
				if (gsSelectedGuy == pSoldier->ubID)
				{
					pShadeTable = (UINT16 *)pShadeStart[gsGlowFrames[gsCurrentGlowFrame] + bGlowShadeOffset];
					*gsForceSoldierZLevel = TOPMOST_Z_LEVEL;
				}
				else
				{
					// Are we dealing with a not-so visible merc?
					if (bGlowShadeOffset == 10)
					{
						pShadeTable = pPaletteTable->pEffectShades[0];
					}
				}
			}
			else
			{
				// OK,not highlighted, but maybe we are in enemy's turn and they have the baton
				// AI's turn?
				if (gTacticalStatus.ubCurrentTeam != OUR_TEAM)
				{
					// Do he have baton?
					if ((pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL))
					{
						pShadeTable = (UINT16 *)pShadeStart[gpGlowFramePointer[gsCurrentGlowFrame] + bGlowShadeOffset];
						if (gpGlowFramePointer[gsCurrentGlowFrame] >= 7)
						{
							*gsForceSoldierZLevel = TOPMOST_Z_LEVEL;
						}
					}
				}
				else
				{
					pShadeTable = (UINT16 *)pShadeStart[gpGlowFramePointer[gsCurrentGlowFrame] + bGlowShadeOffset];
					if (gpGlowFramePointer[gsCurrentGlowFrame] >= 7)
					{
						*gsForceSoldierZLevel = TOPMOST_Z_LEVEL;
					}
				}
			}
			//if ( gusSelectedSoldier != NOBODY )
			//{
			//  pSelSoldier = MercPtrs[ gusSelectedSoldier ];
			// Shade differently depending on visiblity
			//  if ( pSoldier->bVisible == 0 || ( pSelSoldier->aiData.bOppList[ pSoldier->ubID ] == 0  ) )
			//  {
			// Shade gray
			//      pShadeTable = pSoldier->pGlowShades[ gpGlowFramePointer[ gsCurrentGlowFrame ] + 10 ];
			//  }
			//}
		}
	}
	if (!(uiFlags & TILES_DIRTY))
	{
		if (pSoldier->flags.fForceShade)
		{
			pShadeTable = pSoldier->pForcedShade;
		}
	}
	// check if we are a merc duplicate, if so, only do minimal stuff!
	if (pSoldier->ubID >= MAX_NUM_SOLDIERS)
	{
		// Shade gray
		pShadeTable = pPaletteTable->pEffectShades[1];
	}

	// Flugente: frozen soldiers appear to be in ice, which we simulate by having the soldier be fully white
	if (pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO])
	{
		pShadeTable = White16BPPPalette;
	}
	return pShadeTable;
}



/* 
MONSTERS BE HERE!
*/
void RenderTiles(UINT32 uiFlags, INT32 iStartPointX_M, INT32 iStartPointY_M, INT32 iStartPointX_S, INT32 iStartPointY_S, INT32 iEndXS, INT32 iEndYS, UINT8 ubNumLevels, UINT32 *puiLevels, UINT16 *psLevelIDs)
{

	//#if 0

	LEVELNODE		*pNode; //, *pLand, *pStruct; //*pObject, *pTopmost, *pMerc;
	SOLDIERTYPE	*pSoldier;
	HVOBJECT		hVObject = NULL;
	ETRLEObject *pTrav;
	TILE_ELEMENT *TileElem = NULL;
	UINT32			uiDestPitchBYTES = 0;
	UINT8				*pDestBuf = NULL;
	UINT16			usAnimSurface;
	INT8				bXOddFlag = 0;
	INT32				iAnchorPosX_M, iAnchorPosY_M;
	INT32				iAnchorPosX_S, iAnchorPosY_S;
	INT32				iTempPosX_M, iTempPosY_M;
	INT32				iTempPosX_S, iTempPosY_S;
	FLOAT				dOffsetX, dOffsetY;
	FLOAT				dTempX_S, dTempY_S;
	INT32			uiTileIndex;
	UINT16			usImageIndex, *pShadeTable, *pDirtyBackPtr;
	UINT32			uiBrushWidth, uiBrushHeight, uiDirtyFlags;
	INT16				sTileHeight, sXPos, sYPos, sZLevel;
	INT16				sMouseX_M, sMouseY_M;
	BOOLEAN			fShadowBlitter = FALSE;
	BOOLEAN			fZBlitter = FALSE;
	BOOLEAN			fZWrite = FALSE;
	BOOLEAN			fLinkedListDirection = TRUE;
	BOOLEAN			fRenderTile = TRUE;
	BOOLEAN			fMerc = FALSE;
	BOOLEAN			fCheckForRedundency = FALSE;
	UINT32			uiRowFlags;
	BOOLEAN			fDynamic = TRUE;
	BOOLEAN			fEndRenderRow = FALSE;
	BOOLEAN			fEndRenderCol = FALSE;
	BOOLEAN			fPixelate = FALSE;
	BOOLEAN			fMultiZBlitter = FALSE;
	BOOLEAN			fWallTile = FALSE;
	BOOLEAN			fMultiTransShadowZBlitter = FALSE;
	INT16				sMultiTransShadowZBlitterIndex = -1;
	BOOLEAN			fTranslucencyType = FALSE;
	INT16				sX, sY;
	BOOLEAN			fTileInvisible = FALSE;
	BOOLEAN			fConvertTo16 = FALSE;
	UINT32			cnt;
	static			UINT8				ubLevelNodeStartIndex[NUM_RENDER_FX_TYPES];
	BOOLEAN			bItemOutline;
	UINT16			usOutlineColor = 0;

	static			INT32				iTileMapPos[500];
	INT32			uiMapPosIndex;
	UINT8				bBlitClipVal;
	INT8				bItemCount, bVisibleItemCount;
	//UINT16			us16BPPIndex;
	RenderFXType  RenderingFX;
	BOOLEAN				fCheckForMouseDetections = FALSE;
	static				RenderFXType  RenderFXList[NUM_RENDER_FX_TYPES];
	BOOLEAN				fSaveZ;
	INT16					sWorldY;
	INT16					sZOffsetX = -1;
	INT16					sZOffsetY = -1;
	BOOLEAN				fIntensityBlitter;
	INT16						gsForceSoldierZLevel;
	ROTTING_CORPSE	*pCorpse = NULL;
	BOOLEAN				fUseTileElem;
	UINT32				uiLevelNodeFlags;
	UINT32				uiTileElemFlags = 0;
	BOOLEAN				fObscured;
	BOOLEAN				fObscuredBlitter;
	INT16					sModifiedTileHeight;
	BOOLEAN				fDoRow;

	UINT32				uiSaveBufferPitchBYTES;
	UINT8					*pSaveBuf;
	ITEM_POOL			*pItemPool = NULL;
	BOOLEAN				fHiddenTile = FALSE;
	UINT32        uiAniTileFlags = 0;
	INT16					sZStripIndex;

	//Init some variables
	usImageIndex = 0;
	sZLevel = 0;
	uiDirtyFlags = 0;
	pShadeTable = NULL;

	// Begin Render Loop
	iAnchorPosX_M = iStartPointX_M;
	iAnchorPosY_M = iStartPointY_M;
	iAnchorPosX_S = iStartPointX_S;
	iAnchorPosY_S = iStartPointY_S;

	// Flugente: clear rectangles reserved for decals
	ClearBackgroundRectanglesForDecal();

	if (!(uiFlags&TILES_DIRTY))
		pDestBuf = LockVideoSurface(FRAME_BUFFER, &uiDestPitchBYTES);


	if (uiFlags & TILES_DYNAMIC_CHECKFOR_INT_TILE)
	{
		if (ShouldCheckForMouseDetections())
		{
			BeginCurInteractiveTileCheck(gubIntTileCheckFlags);
			fCheckForMouseDetections = TRUE;

			// If we are in edit mode, don't do this...
			if (gfEditMode)
			{
				fCheckForMouseDetections = FALSE;
			}
		}
	}

	//if((uiFlags&TILES_TYPE_MASK)==TILES_STATIC_LAND)
	GetMouseXY(&sMouseX_M, &sMouseY_M);

	pDirtyBackPtr = NULL;

	if (gTacticalStatus.uiFlags&TRANSLUCENCY_TYPE)
		fTranslucencyType = TRUE;

	for (cnt = 0; cnt < ubNumLevels; cnt++)
	{
		ubLevelNodeStartIndex[cnt] = RenderFXStartIndex[psLevelIDs[cnt]];
		RenderFXList[cnt] = RenderFX[psLevelIDs[cnt]];
	}

	do
	{

		iTempPosX_M = iAnchorPosX_M;
		iTempPosY_M = iAnchorPosY_M;
		iTempPosX_S = iAnchorPosX_S;
		iTempPosY_S = iAnchorPosY_S;

		uiMapPosIndex = 0;

		// Build tile index list
		do
		{
			iTileMapPos[uiMapPosIndex] = FASTMAPROWCOLTOPOS(iTempPosY_M, iTempPosX_M);

			iTempPosX_S += 40;
			iTempPosX_M++;
			iTempPosY_M--;

			uiMapPosIndex++;

		} while (iTempPosX_S < iEndXS);



		for (cnt = 0; cnt < ubNumLevels; cnt++)
		{

			uiRowFlags = puiLevels[cnt];
			fDoRow = TRUE;

			if ((uiRowFlags & TILES_ALL_DYNAMICS) && !(uiLayerUsedFlags & uiRowFlags) && !(uiFlags & TILES_DYNAMIC_CHECKFOR_INT_TILE))
			{
				fDoRow = FALSE;
			}

			if (fDoRow)
			{
				iTempPosX_M = iAnchorPosX_M;
				iTempPosY_M = iAnchorPosY_M;
				iTempPosX_S = iAnchorPosX_S;
				iTempPosY_S = iAnchorPosY_S;

				fEndRenderRow = FALSE;
				uiMapPosIndex = 0;


				if (bXOddFlag > 0)
					iTempPosX_S += 20;

				do
				{

					uiTileIndex = iTileMapPos[uiMapPosIndex];
					uiMapPosIndex++;

					//if ( 0 )					
					if (!TileIsOutOfBounds(uiTileIndex))
					{
						// OK, we're searching through this loop anyway, might as well check for mouse position
						// over objects...
						// Experimental!
						if (uiFlags & TILES_DYNAMIC_CHECKFOR_INT_TILE)
						{
							if (fCheckForMouseDetections && gpWorldLevelData[uiTileIndex].pStructHead != NULL)
							{
								LogMouseOverInteractiveTile(uiTileIndex);
							}
						}

						if ((uiFlags&TILES_MARKED) && !(gpWorldLevelData[uiTileIndex].uiFlags&MAPELEMENT_REDRAW))
						{
							pNode = NULL;
						}
						else
						{
							//pNode = gpWorldLevelData[ uiTileIndex ].pLevelNodes[ RenderFXStartIndex[ psLevelIDs[ cnt ] ] ];
							//pNode = gpWorldLevelData[ uiTileIndex ].pLevelNodes[ 0 ];
							//pNode=NULL;
							pNode = gpWorldLevelData[uiTileIndex].pLevelNodes[ubLevelNodeStartIndex[cnt]];
						}

						bItemCount = 0;
						bVisibleItemCount = 0;
						pItemPool = NULL;

						while (pNode != NULL)
						{

							RenderingFX = RenderFXList[cnt];

							fObscured = RenderingFX.fObscured;
							fDynamic = RenderingFX.fDynamic;

							fMerc = RenderingFX.fMerc;
							fZWrite = RenderingFX.fZWrite;
							fZBlitter = RenderingFX.fZBlitter;
							fShadowBlitter = RenderingFX.fShadowBlitter;
							fLinkedListDirection = RenderingFX.fLinkedListDirection;
							fCheckForRedundency = RenderingFX.fCheckForRedundency;
							fMultiZBlitter = RenderingFX.fMultiZBlitter;
							fConvertTo16 = RenderingFX.fConvertTo16;
							fIntensityBlitter = FALSE;
							fSaveZ = FALSE;
							fWallTile = FALSE;
							gsForceSoldierZLevel = FALSE;
							pSoldier = NULL;
							fUseTileElem = FALSE;
							fMultiTransShadowZBlitter = FALSE;
							fObscuredBlitter = FALSE;
							fTranslucencyType = TRUE;
							uiAniTileFlags = 0;
							sZStripIndex = -1;

							uiLevelNodeFlags = pNode->uiFlags;

							if (fCheckForRedundency)
							{
								if ((gpWorldLevelData[uiTileIndex].uiFlags & MAPELEMENT_REDUNDENT))
								{
									// IF WE DONOT WANT TO RE-EVALUATE FIRST
									if (!(gpWorldLevelData[uiTileIndex].uiFlags & MAPELEMENT_REEVALUATE_REDUNDENCY) && !(gTacticalStatus.uiFlags & NOHIDE_REDUNDENCY))
									{
										pNode = NULL;
										break;
									}
								}
							}

							// Force z-buffer blitting for marked tiles ( even ground!)
							if ((uiFlags&TILES_MARKED))
							{
								fZBlitter = TRUE;
							}


							//Looking up height every time here is alot better than doing it above!
							sTileHeight = gpWorldLevelData[uiTileIndex].sHeight;

							sModifiedTileHeight = (((sTileHeight / 80) - 1) * 80);

							if (sModifiedTileHeight < 0)
							{
								sModifiedTileHeight = 0;
							}


							fRenderTile = TRUE;
							pDirtyBackPtr = NULL;
							if (uiLevelNodeFlags&LEVELNODE_REVEAL)
							{
								if (!fDynamic)
									fRenderTile = FALSE;
								else
									fPixelate = TRUE;
							}
							else
								fPixelate = FALSE;

							// non-type specific setup
							sXPos = (INT16)iTempPosX_S;
							sYPos = (INT16)iTempPosY_S;


							// setup for any tile type except mercs
							if (!fMerc)
							{
								if (!(uiLevelNodeFlags & (LEVELNODE_ROTTINGCORPSE | LEVELNODE_CACHEDANITILE)))
								{
									if ((uiLevelNodeFlags & LEVELNODE_REVEALTREES))
									{
										TileElem = &(gTileDatabase[pNode->usIndex + 2]);
									}
									else
									{
										TileElem = &(gTileDatabase[pNode->usIndex]);
									}

									// HANDLE INDEPENDANT-PER-TILE ANIMATIONS ( IE: DOORS, EXPLODING THINGS, ETC )
									if (fDynamic)
									{
										if ((uiLevelNodeFlags & LEVELNODE_ANIMATION))
										{
											if (pNode->sCurrentFrame != -1)
											{
												Assert(TileElem->pAnimData != NULL);
												TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[pNode->sCurrentFrame]];
											}
										}
									}
								}

								// Check for best translucency
								if (uiLevelNodeFlags & LEVELNODE_USEBESTTRANSTYPE)
								{
									fTranslucencyType = FALSE;
								}

								if ((uiLevelNodeFlags & (LEVELNODE_ROTTINGCORPSE | LEVELNODE_CACHEDANITILE)))
								{
									if (fDynamic)
									{
										if (!(uiLevelNodeFlags & (LEVELNODE_DYNAMIC)) && !(uiLevelNodeFlags & LEVELNODE_LASTDYNAMIC))
											fRenderTile = FALSE;
									}
									else if ((uiLevelNodeFlags & (LEVELNODE_DYNAMIC)))
										fRenderTile = FALSE;
								}
								else
								{
									// Set Tile elem flags here!
									uiTileElemFlags = TileElem->uiFlags;
									// Set valid tile elem!
									fUseTileElem = TRUE;

									if (fDynamic || fPixelate)
									{
										if (!fPixelate)
										{
											if (!(uiTileElemFlags & ANIMATED_TILE) && !(uiTileElemFlags & DYNAMIC_TILE) && !(uiLevelNodeFlags & LEVELNODE_DYNAMIC) && !(uiLevelNodeFlags & LEVELNODE_LASTDYNAMIC))
												fRenderTile = FALSE;
											else if (!(uiTileElemFlags&DYNAMIC_TILE) && !(uiLevelNodeFlags&LEVELNODE_DYNAMIC) && !(uiLevelNodeFlags&LEVELNODE_LASTDYNAMIC))
												//	else if((TileElem->uiFlags&ANIMATED_TILE) )
											{
												Assert(TileElem->pAnimData != NULL);
												TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[TileElem->pAnimData->bCurrentFrame]];
												uiTileElemFlags = TileElem->uiFlags;
											}
										}

									}
									else if ((uiTileElemFlags & ANIMATED_TILE) || (uiTileElemFlags & DYNAMIC_TILE) || (uiLevelNodeFlags & LEVELNODE_DYNAMIC))
									{
										if (!(uiFlags & TILES_OBSCURED) || (uiTileElemFlags & ANIMATED_TILE))
										{
											fRenderTile = FALSE;
										}
									}
								}

								// OK, ATE, CHECK FOR AN OBSCURED TILE AND MAKE SURE IF LEVELNODE IS SET
								// WE DON'T RENDER UNLESS WE HAVE THE RENDER FLAG SET!
								if (fObscured)
								{
									if ((uiFlags & TILES_OBSCURED))
									{
										if (uiLevelNodeFlags & LEVELNODE_SHOW_THROUGH)
										{
											fObscuredBlitter = TRUE;

											// ATE: Check if this is a levelnode, and what frame we are on
											// turn off......
											//if ( ( uiLevelNodeFlags & LEVELNODE_ITEM ) && gsCurrentItemGlowFrame < 25 )
											//{
											//	fRenderTile = FALSE;
											//}
										}
										else
										{
											// Don;t render if we are not on this render loop!
											fRenderTile = FALSE;
										}
									}
									else
									{
										if (uiLevelNodeFlags & LEVELNODE_SHOW_THROUGH)
										{
											fRenderTile = FALSE;

											// ATE: Check if this is a levelnode, and what frame we are on
											// turn off......
											//if ( ( uiLevelNodeFlags & LEVELNODE_ITEM ) && gsCurrentItemGlowFrame < 25 )
											//{
											//	fRenderTile = TRUE;
											//}
										}
									}
								}


								// If flag says to do dynamic as well, render!
								if ((uiFlags & TILES_DOALL))
								{
									fRenderTile = TRUE;
								}

								if (fRenderTile)
								{
									// Set flag to set layer as used
									if (fDynamic || fPixelate)
									{
										uiAdditiveLayerUsedFlags |= uiRowFlags;
									}

									if (uiLevelNodeFlags & LEVELNODE_DYNAMICZ)
									{
										fSaveZ = TRUE;
										fZWrite = TRUE;
									}

									if ((uiLevelNodeFlags & LEVELNODE_CACHEDANITILE))
									{
										hVObject = gpTileCache[pNode->pAniTile->sCachedTileID].pImagery->vo;
										usImageIndex = pNode->pAniTile->sCurrentFrame;
										uiAniTileFlags = pNode->pAniTile->uiFlags;

										// Position corpse based on its float position
										if ((uiLevelNodeFlags & LEVELNODE_ROTTINGCORPSE))
										{
											pCorpse = &(gRottingCorpse[pNode->pAniTile->uiUserData]);

											pShadeTable = pCorpse->pShades[pNode->ubShadeLevel];

											//pShadeTable = pCorpse->p16BPPPalette;

											dOffsetX = pCorpse->def.dXPos - gsRenderCenterX;
											dOffsetY = pCorpse->def.dYPos - gsRenderCenterY;

											// OK, if this is a corpse.... stop if not visible
											if (pCorpse->def.bVisible != 1 && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS))
											{
												//CONTINUE, DONOT RENDER
												if (!fLinkedListDirection)
													pNode = pNode->pPrevNode;
												else
													pNode = pNode->pNext;

												continue;
											}
										}
										else
										{
											dOffsetX = (FLOAT)(pNode->pAniTile->sRelativeX - gsRenderCenterX);
											dOffsetY = (FLOAT)(pNode->pAniTile->sRelativeY - gsRenderCenterY);
										}

										// Calculate guy's position
										FloatFromCellToScreenCoordinates(dOffsetX, dOffsetY, &dTempX_S, &dTempY_S);

										sXPos = ((gsVIEWPORT_END_X - gsVIEWPORT_START_X) / 2) + (INT16)dTempX_S;
										sYPos = ((gsVIEWPORT_END_Y - gsVIEWPORT_START_Y) / 2) + (INT16)dTempY_S - sTileHeight;

										// Adjust for offset position on screen
										sXPos -= gsRenderWorldOffsetX;
										sYPos -= gsRenderWorldOffsetY;

									}
									else
									{
										hVObject = TileElem->hTileSurface;
										usImageIndex = TileElem->usRegionIndex;

										// ADJUST FOR WORLD MAPELEM HIEGHT
										sYPos -= TileElem->sOffsetHeight;

										if ((TileElem->uiFlags&IGNORE_WORLD_HEIGHT))
										{
											sYPos = sYPos - sModifiedTileHeight;
											//sYPos -= sTileHeight;
										}

										if (!(uiLevelNodeFlags&LEVELNODE_IGNOREHEIGHT) && !(TileElem->uiFlags&IGNORE_WORLD_HEIGHT))
											sYPos -= sTileHeight;

										if (!(uiFlags&TILES_DIRTY))
										{
											hVObject->pShadeCurrent = hVObject->pShades[pNode->ubShadeLevel];
											hVObject->pShade8 = ubColorTables[pNode->ubShadeLevel];
										}

									}


									//ADJUST FOR RELATIVE OFFSETS
									if (uiLevelNodeFlags & LEVELNODE_USERELPOS)
									{
										sXPos += pNode->sRelativeX;
										sYPos += pNode->sRelativeY;
									}

									if (uiLevelNodeFlags& LEVELNODE_USEZ)
									{
										sYPos -= pNode->sRelativeZ;
									}

									//ADJUST FOR ABSOLUTE POSITIONING
									if (uiLevelNodeFlags& LEVELNODE_USEABSOLUTEPOS)
									{
										dOffsetX = (FLOAT)(pNode->sRelativeX - gsRenderCenterX);
										dOffsetY = (FLOAT)(pNode->sRelativeY - gsRenderCenterY);

										// OK, DONT'T ASK... CONVERSION TO PROPER Y NEEDS THIS...
										dOffsetX -= CELL_Y_SIZE;

										FloatFromCellToScreenCoordinates(dOffsetX, dOffsetY, &dTempX_S, &dTempY_S);

										sXPos = ((gsVIEWPORT_END_X - gsVIEWPORT_START_X) / 2) + (INT16)SHORT_ROUND(dTempX_S);
										sYPos = ((gsVIEWPORT_END_Y - gsVIEWPORT_START_Y) / 2) + (INT16)SHORT_ROUND(dTempY_S);

										// Adjust for offset position on screen
										sXPos -= gsRenderWorldOffsetX;
										sYPos -= gsRenderWorldOffsetY;


										sYPos -= pNode->sRelativeZ;

									}

								}

								// COUNT # OF ITEMS AT THIS LOCATION
								if (uiLevelNodeFlags & LEVELNODE_ITEM)
								{
									// OK set item pool for this location....
									if (bItemCount == 0)
									{
										pItemPool = pNode->pItemPool;
									}
									else
									{
										pItemPool = pItemPool->pNext;
									}

									if (bItemCount < MAX_RENDERED_ITEMS)
									{
										bItemCount++;

										if (gWorldItems[pItemPool->iItemIndex].bVisible == VISIBLE)
										{
											bVisibleItemCount++;
										}
									}

									// LIMIT RENDERING OF ITEMS TO ABOUT 7, DO NOT RENDER HIDDEN ITEMS TOO!
									if (bVisibleItemCount == MAX_RENDERED_ITEMS || (gWorldItems[pItemPool->iItemIndex].bVisible != VISIBLE) || (pItemPool->usFlags & WORLD_ITEM_DONTRENDER))
									{
										if (!(gTacticalStatus.uiFlags&SHOW_ALL_ITEMS))
										{
											//CONTINUE, DONOT RENDER
											if (!fLinkedListDirection)
												pNode = pNode->pPrevNode;
											else
												pNode = pNode->pNext;
											continue;
										}
									}

									if (guiCurrentScreen == EDIT_SCREEN)
									{
										// ATE: If in the editor, change this to a little higher value
										if (bItemCount == MAX_RENDERED_ITEMS)
										{
											//CONTINUE, DONOT RENDER
											if (!fLinkedListDirection)
												pNode = pNode->pPrevNode;
											else
												pNode = pNode->pNext;
											continue;
										}
									}
									if (pItemPool->bRenderZHeightAboveLevel > 0)
									{
										sYPos -= pItemPool->bRenderZHeightAboveLevel;
									}

								}

								// If render tile is false...
								if (!fRenderTile)
								{
									if (!fLinkedListDirection)
										pNode = pNode->pPrevNode;
									else
										pNode = pNode->pNext;

									continue;
								}
							}

							// specific code for node types on a per-tile basis
							switch (uiRowFlags)
							{
							case TILES_STATIC_LAND:

								sZLevel = LandZLevel(iTempPosX_M, iTempPosY_M);
								break;

							case TILES_STATIC_OBJECTS:

								// ATE: Modified to use constant z level, as these are same level as land items
								sZLevel = ObjectZLevel(TileElem, pNode, uiTileElemFlags, iTempPosX_M, iTempPosY_M, sWorldY);
								break;

							case TILES_STATIC_STRUCTURES:

								StructZLevel(iTempPosX_M, iTempPosY_M);

								if (fUseTileElem && (TileElem->uiFlags & MULTI_Z_TILE))
								{
									fMultiZBlitter = TRUE;
								}

								// ATE: if we are a wall, set flag
								if (fUseTileElem && (TileElem->uiFlags & WALL_TILE))
								{
									fWallTile = TRUE;
								}

								break;

							case TILES_STATIC_ROOF:

								sZLevel = RoofZLevel(iTempPosX_M, iTempPosY_M, sWorldY);

								// Automatically adjust height!
								sYPos -= WALL_HEIGHT;

								// ATE: Added for shadows on roofs
								if (fUseTileElem && (TileElem->uiFlags & ROOFSHADOW_TILE))
								{
									fShadowBlitter = TRUE;
								}
								break;
							case TILES_STATIC_ONROOF:

								sZLevel = OnRoofZLevel(iTempPosX_M, iTempPosY_M, sWorldY, uiLevelNodeFlags);
								// Automatically adjust height!
								sYPos -= WALL_HEIGHT;
								break;

							case TILES_STATIC_TOPMOST:

								sZLevel = TopmostZLevel(iTempPosX_M, iTempPosY_M, sWorldY);
								break;

							case TILES_STATIC_SHADOWS:

								sZLevel = ShadowZLevel(iTempPosX_M, iTempPosY_M, sWorldY);

								if (uiLevelNodeFlags & LEVELNODE_EXITGRID)
								{
									fIntensityBlitter = TRUE;
									fShadowBlitter = FALSE;
								}
								break;

							case TILES_DYNAMIC_LAND:

								sZLevel = LandZLevel(iTempPosX_M, iTempPosY_M);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								break;
							case TILES_DYNAMIC_SHADOWS:

								sZLevel = ShadowZLevel(iTempPosX_M, iTempPosY_M, sWorldY);
								//sZLevel=SHADOW_Z_LEVEL;
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								break;
							case TILES_DYNAMIC_OBJECTS:

								sZLevel = ObjectZLevel(TileElem, pNode, uiTileElemFlags, iTempPosX_M, iTempPosY_M, sWorldY);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								break;

							case TILES_DYNAMIC_STRUCTURES:

								StructZLevel(iTempPosX_M, iTempPosY_M);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;

								if (uiTileElemFlags & Z_AWARE_DYNAMIC_TILE)
								{
									fMultiZBlitter = TRUE;
									fZBlitter = TRUE;
									fWallTile = TRUE;
									sZStripIndex = 0;
								}
								break;
							case TILES_DYNAMIC_ROOF:

								sYPos -= WALL_HEIGHT;

								sZLevel = RoofZLevel(iTempPosX_M, iTempPosY_M, sWorldY);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								// For now, adjust to hieght of a wall ( 50 temp, make define )
								//if ( TileElem->fType > FOOTPRINTS )
								//{
								//	sYPos -= 58;
								//}
								break;

							case TILES_DYNAMIC_ONROOF:

								sZLevel = OnRoofZLevel(iTempPosX_M, iTempPosY_M, sWorldY, uiLevelNodeFlags);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								// Automatically adjust height!
								sYPos -= WALL_HEIGHT;
								break;

							case TILES_DYNAMIC_TOPMOST:
								sZLevel = TopmostZLevel(iTempPosX_M, iTempPosY_M, sWorldY);
								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED;
								break;

							case TILES_DYNAMIC_MERCS:
							case TILES_DYNAMIC_HIGHMERCS:
							case TILES_DYNAMIC_STRUCT_MERCS:

								// Set flag to set layer as used
								uiAdditiveLayerUsedFlags |= uiRowFlags;

								pSoldier = pNode->pSoldier;

								if (uiRowFlags == TILES_DYNAMIC_MERCS)
								{
									// If we are multi-tiled, ignore here
									if (pSoldier->flags.uiStatusFlags & (SOLDIER_MULTITILE_Z | SOLDIER_Z))
									{
										pNode = pNode->pNext;
										continue;
									}

									// If we are at a higher level, no not do anything unless we are at the highmerc stage
									if (pSoldier->pathing.bLevel > 0)
									{
										pNode = pNode->pNext;
										continue;
									}
								}

								if (uiRowFlags == TILES_DYNAMIC_HIGHMERCS)
								{
									// If we are multi-tiled, ignore here
									if (pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE_Z)
									{
										pNode = pNode->pNext;
										continue;
									}

									// If we are at a lower level, no not do anything unless we are at the highmerc stage
									if (pSoldier->pathing.bLevel == 0)
									{
										pNode = pNode->pNext;
										continue;
									}
								}


								if (uiRowFlags == TILES_DYNAMIC_STRUCT_MERCS)
								{
									// If we are not multi-tiled, ignore here
									if (!(pSoldier->flags.uiStatusFlags & (SOLDIER_MULTITILE_Z | SOLDIER_Z)))
									{
										// If we are at a low level, no not do anything unless we are at the merc stage
										if (pSoldier->pathing.bLevel == 0)
										{
											pNode = pNode->pNext;
											continue;
										}
									}

									if (pSoldier->flags.uiStatusFlags & (SOLDIER_MULTITILE_Z | SOLDIER_Z))
									{
										fSaveZ = TRUE;
										fZBlitter = TRUE;

										if (pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE_Z)
										{
											fMultiTransShadowZBlitter = TRUE;
											// ATE: Use one direction for queen!
											if (pSoldier->ubBodyType == QUEENMONSTER)
											{
												sMultiTransShadowZBlitterIndex = 0;
											}
											else
											{
												sMultiTransShadowZBlitterIndex = gOneCDirection[pSoldier->ubDirection];
											}
										}
										else
										{
											fZWrite = TRUE;
										}
									}

								}

								// IF we are not active, or are a placeholder for multi-tile animations do nothing
								//if ( !pSoldier->bActive  )
								if (!pSoldier->bActive || (uiLevelNodeFlags & LEVELNODE_MERCPLACEHOLDER))
								{
									pNode = pNode->pNext;
									continue;
								}

								// Skip if we cannot see the guy!
								if (pSoldier->bLastRenderVisibleValue == -1 && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS))
								{
									pNode = pNode->pNext;
									continue;
								}


								// Get animation surface....
								usAnimSurface = GetSoldierAnimationSurface(pSoldier, pSoldier->usAnimState);

								if (usAnimSurface == INVALID_ANIMATION_SURFACE)
								{
									pNode = pNode->pNext;
									continue;
								}


								// Position guy based on guy's position
								dOffsetX = pSoldier->dXPos - gsRenderCenterX;
								dOffsetY = pSoldier->dYPos - gsRenderCenterY;


								// Calculate guy's position
								FloatFromCellToScreenCoordinates(dOffsetX, dOffsetY, &dTempX_S, &dTempY_S);

								sXPos = ((gsVIEWPORT_END_X - gsVIEWPORT_START_X) / 2) + (INT16)dTempX_S;
								sYPos = ((gsVIEWPORT_END_Y - gsVIEWPORT_START_Y) / 2) + (INT16)dTempY_S - sTileHeight;

								// Adjust for offset position on screen
								sXPos -= gsRenderWorldOffsetX;
								sYPos -= gsRenderWorldOffsetY;

								// Adjust for soldier height
								sYPos -= pSoldier->sHeightAdjustment;

								// Handle shade stuff....
								// bio: put the logic for determining the right shade table into inline fuction (called again for logical bodytype implementation)
								// TODO: in case of a logical bodytype being used, the function does not have to be called here (if determining of log. bodytype is moved to the render entry fun.
								pShadeTable = GetShadeTable(pNode, pSoldier, pSoldier, uiFlags, &gsForceSoldierZLevel);

								// Calculate Z level
								SoldierZLevel(pSoldier, iTempPosX_M, iTempPosY_M);

								hVObject = gAnimSurfaceDatabase[usAnimSurface].hVideoObject;

								if (hVObject == NULL)
								{
									pNode = pNode->pNext;
									continue;
								}

								// ATE: If we are in a gridno that we should not use obscure blitter, set!
								if (!(gpWorldLevelData[uiTileIndex].ubExtFlags[0] & MAPELEMENT_EXT_NOBURN_STRUCT))
								{
									fObscuredBlitter = TRUE;
								}
								else
								{
									// ATE: Artificially increase z=level...
									sZLevel += 2;
								}

								usImageIndex = pSoldier->usAniFrame;

								// Flugente: frozen soldiers don't move
								if (pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] && pSoldier->stats.bLife > 0)
								{
									usImageIndex = pSoldier->CryoAniFrame();
								}

								uiDirtyFlags = BGND_FLAG_SINGLE | BGND_FLAG_ANIMATED | BGND_FLAG_MERC;
								break;
							}

							// Adjust for interface level
							sYPos += gsRenderHeight;

							// OK, check for LEVELNODE HIDDEN...
							fHiddenTile = FALSE;

							if (uiLevelNodeFlags & LEVELNODE_HIDDEN)
							{
								fHiddenTile = TRUE;

								if (TileElem != NULL)
								{
									// If we are a roof and have SHOW_ALL_ROOFS on, turn off hidden tile check!
									//if ( ( TileElem->uiFlags & ROOF_TILE ) && ( gTacticalStatus.uiFlags&SHOW_ALL_ROOFS ) )
									// anv: commented out TileElem->uiFlags & ROOF_TILE because:
									// 1. only roof tiles get hidden in this manner anyway
									// 2. we might want to his and reveal again other stuff (most importantly - on roof structures)
									if ((gTacticalStatus.uiFlags&SHOW_ALL_ROOFS))
									{
										// Turn off
										fHiddenTile = FALSE;
									}
								}
							}


							if (fRenderTile && !fHiddenTile)
							{
								fTileInvisible = FALSE;

								if ((uiLevelNodeFlags & LEVELNODE_ROTTINGCORPSE))
								{
									// Set fmerc flag!
									fMerc = TRUE;
									fZWrite = TRUE;

									//if ( hVObject->ppZStripInfo != NULL )
									{
										sMultiTransShadowZBlitterIndex = GetCorpseStructIndex(&(pCorpse->def), TRUE);
										fMultiTransShadowZBlitter = TRUE;
									}
								}

								if ((uiLevelNodeFlags & LEVELNODE_LASTDYNAMIC) && !(uiFlags&TILES_DIRTY))
								{
									// Remove flags!
									pNode->uiFlags &= (~LEVELNODE_LASTDYNAMIC);
									fZWrite = TRUE;
								}

								if (uiLevelNodeFlags & LEVELNODE_NOWRITEZ)
								{
									fZWrite = FALSE;
								}


								if (uiFlags&TILES_NOZWRITE)
									fZWrite = FALSE;

								if (uiFlags & TILES_NOZ)
								{
									fZBlitter = FALSE;
								}



								if ((uiLevelNodeFlags & LEVELNODE_WIREFRAME))
								{
									if (!gGameSettings.fOptions[TOPTION_TOGGLE_WIREFRAME])
									{
										fTileInvisible = TRUE;
									}
								}

								using namespace LogicalBodyTypes;
								static BodyTypeDB * bodyTypeDB = &BodyTypeDB::Instance();
								static Layers::LayerPropertiesVector::size_type numLayers = max(1, Layers::Instance().GetCount()); //defaults to 1 in case the layers.xml file could not be read
								// TODO: should find a better place to determine the logical body type. This function gets called like a gazillion times
								// every render cycle and the find provedure will get quite expensive at later stages (lots of logical body types, animations and filters!)
								// Should only be done once every cycle, or better yet, only and directly after a SOLDIERTYPE object has been altered (if at all possible)!
								BodyType * bt = NULL;
								if ((gGameSettings.fOptions[TOPTION_USE_LOGICAL_BODYTYPES] == TRUE)
									&& (uiRowFlags == TILES_DYNAMIC_MERCS || uiRowFlags == TILES_DYNAMIC_HIGHMERCS || uiRowFlags == TILES_DYNAMIC_STRUCT_MERCS))
								{
									bt = bodyTypeDB->Find(pSoldier);
								}
								UINT16 * pDefaultShadeTable = pShadeTable;
								RECT backRect; // Actually only needed if uiFlags & TILES_DIRTY, but must be initialized with values that make sense for the comparisons.
								backRect.left = SCREEN_WIDTH;
								backRect.top = SCREEN_HEIGHT;
								backRect.right = 0;
								backRect.bottom = 0;
								
								Layers::LayerGraphIterator layerIter;
								Layers::LayerGraphIterator layerEnd;
								if (bt != NULL)
								{
									layerIter = Layers::Instance().GetIterator(pSoldier->bMovementDirection);
									layerEnd = Layers::Instance().GetIterationEnd(pSoldier->bMovementDirection);
								}

								HVOBJECT hVObjectAlpha = NULL;
								while ((bt == NULL) || (layerIter != layerEnd))
								{
									bool fIgnoreShadows = false;
									if (bt != NULL)
									{
										BodyType::LogicalSurfaceType * logSurfaceType = bt->GetLogicalSurfaceType(layerIter->index, pSoldier);
										if (logSurfaceType != NULL)
										{
											const Layers::LayerProperties * layerProperties = bt->GetLayerProperties(layerIter->index);
											fIgnoreShadows = !layerProperties->renderShadows;
											if (!layerProperties->render)
											{
												layerIter++;
												continue;
											}
											hVObject = logSurfaceType->physicalSurfaceType->hVideoObject;
											if (hVObject == NULL)
											{
												layerIter++;
												continue;
											}
											if (logSurfaceType->alphaSurfaceType != NULL) {
												hVObjectAlpha = logSurfaceType->alphaSurfaceType->hVideoObject;
											}
											if (logSurfaceType->paletteTable != NULL)
											{
												pShadeTable = GetShadeTable(pNode, pSoldier, (SOLDIERTYPE *)logSurfaceType->paletteTable, uiFlags, &gsForceSoldierZLevel);
											}
											else
											{
												pShadeTable = pDefaultShadeTable;
											}
										}
										else
										{
											layerIter++;
											continue;
										}
									}

								// RENDER
								if (fTileInvisible)
								{

								}
								else if (uiLevelNodeFlags & LEVELNODE_DISPLAY_AP && !(uiFlags&TILES_DIRTY))
								{
									pTrav = &(hVObject->pETRLEObject[usImageIndex]);
									sXPos += pTrav->sOffsetX;
									sYPos += pTrav->sOffsetY;

									if (gfUIDisplayActionPointsInvalid)
									{
										SetFontBackground(FONT_MCOLOR_BLACK);
										SetFontForeground(FONT_MCOLOR_WHITE);
									}
									else
									{
										SetFontBackground(FONT_MCOLOR_BLACK);
										SetFontForeground(FONT_MCOLOR_WHITE);
									}

									if (gfUIDisplayActionPointsBlack)
									{
										SetFontBackground(FONT_MCOLOR_BLACK);
										SetFontForeground(FONT_MCOLOR_BLACK);
									}

									SetFont(TINYFONT1);
									SetFontDestBuffer(guiSAVEBUFFER, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, gsVIEWPORT_WINDOW_END_Y, FALSE);
									VarFindFontCenterCoordinates(sXPos, sYPos, 1, 1, TINYFONT1, &sX, &sY, L"%d", pNode->uiAPCost);
									mprintf_buffer(pDestBuf, uiDestPitchBYTES, TINYFONT1, sX, sY, L"%d", pNode->uiAPCost);
									SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);
								}
								else if ((uiLevelNodeFlags  & LEVELNODE_ERASEZ) && !(uiFlags&TILES_DIRTY))
								{
									Zero8BPPDataTo16BPPBufferTransparent((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex);
									//Zero8BPPDataTo16BPPBufferTransparent( (UINT16*)gpZBuffer, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex );
								}
								else if ((uiLevelNodeFlags  & LEVELNODE_ITEM) && !(uiFlags&TILES_DIRTY))
								{
									BOOLEAN fZBlit = FALSE;

									if (uiRowFlags == TILES_STATIC_ONROOF || uiRowFlags == TILES_DYNAMIC_ONROOF)
									{
										usOutlineColor = gusYellowItemOutlineColor;
										bItemOutline = TRUE;
										fZBlit = TRUE;
									}
									else
									{
										usOutlineColor = gusNormalItemOutlineColor;
										bItemOutline = TRUE;
										fZBlit = TRUE;
									}

									if (gGameSettings.fOptions[TOPTION_GLOW_ITEMS])
									{
										if (uiRowFlags == TILES_STATIC_ONROOF || uiRowFlags == TILES_DYNAMIC_ONROOF)
										{
											usOutlineColor = us16BPPItemCycleYellowColors[gsCurrentItemGlowFrame];
											bItemOutline = TRUE;
										}
										else
										{
											if (gTacticalStatus.uiFlags & RED_ITEM_GLOW_ON)
											{
												usOutlineColor = us16BPPItemCycleRedColors[gsCurrentItemGlowFrame];
												bItemOutline = TRUE;
											}
											else
											{
												usOutlineColor = us16BPPItemCycleWhiteColors[gsCurrentItemGlowFrame];
												bItemOutline = TRUE;
											}
										}
									}

									//else
										//{
										//	usOutlineColor = us16BPPItemCycleWhiteColors[ pItemPool->bFlashColor ];
										//	bItemOutline = TRUE;
										//}

									bBlitClipVal = BltIsClippedOrOffScreen(hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

									if (bBlitClipVal == FALSE)
									{
										if (fZBlit)
										{
											if (fObscuredBlitter)
											{
												Blt8BPPDataTo16BPPBufferOutlineZPixelateObscured((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline);
											}
											else
											{
												Blt8BPPDataTo16BPPBufferOutlineZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline);
											}
										}
										else
										{
											Blt8BPPDataTo16BPPBufferOutline((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline);
										}
									}
									else if (bBlitClipVal == TRUE)
									{
										if (fZBlit)
										{
											if (fObscuredBlitter)
											{
												Blt8BPPDataTo16BPPBufferOutlineZPixelateObscuredClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline, &gClippingRect);
											}
											else
											{
												Blt8BPPDataTo16BPPBufferOutlineZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline, &gClippingRect);
											}
										}
										else
										{
											Blt8BPPDataTo16BPPBufferOutlineClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline, &gClippingRect);
										}
									}
								}
								// ATE: Check here for a lot of conditions!
								else if ( (uiLevelNodeFlags & LEVELNODE_PHYSICSOBJECT) && !(uiFlags&TILES_DIRTY) )
								{
									bItemOutline = FALSE;

									bBlitClipVal = BltIsClippedOrOffScreen(hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

									if (fShadowBlitter)
									{
										if (bBlitClipVal == FALSE)
										{
											Blt8BPPDataTo16BPPBufferShadowZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
										}
										else
										{
											Blt8BPPDataTo16BPPBufferShadowZNBClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
										}
									}
									else
									{
										if (bBlitClipVal == FALSE)
										{
											Blt8BPPDataTo16BPPBufferOutlineZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline);
										}
										else if (bBlitClipVal == TRUE)
										{
											Blt8BPPDataTo16BPPBufferOutlineClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, usOutlineColor, bItemOutline, &gClippingRect);
										}
									}
								}
								else if (uiFlags&TILES_DIRTY)
								{

									if (!(uiLevelNodeFlags & LEVELNODE_LASTDYNAMIC))
									{
										// changed the logic to determine the maximum dimensions of the needed back rect
										// in case of multi layering. Actual registering of back rect done after layer loop
										pTrav = &(hVObject->pETRLEObject[usImageIndex]);
										uiBrushHeight = (UINT32)pTrav->usHeight;
										uiBrushWidth = (UINT32)pTrav->usWidth;
										INT32 rLeft = sXPos + pTrav->sOffsetX;
										INT32 rTop = sYPos + pTrav->sOffsetY;

										backRect.left = min(backRect.left, rLeft);
										backRect.top = min(backRect.top, rTop);
										backRect.right = max(backRect.right, rLeft + (INT32)uiBrushWidth);
										backRect.bottom = max(backRect.bottom, rTop + (INT32)uiBrushHeight);
									}
								}
								else
								{
									if (gbPixelDepth == 16)
									{
										/*if(fConvertTo16)
										{
											ConvertVObjectRegionTo16BPP(hVObject, usImageIndex, 4);
											if(CheckFor16BPPRegion(hVObject, usImageIndex, 4, &us16BPPIndex))
											{
												Blt16BPPDataTo16BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES,  hVObject, sXPos, sYPos, us16BPPIndex, &gClippingRect);
											}
										}*/

										if (fMultiTransShadowZBlitter)
										{
											if (fZBlitter)
											{
												if (fObscuredBlitter)
												{
													if (hVObjectAlpha == NULL) {
														Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect, sMultiTransShadowZBlitterIndex, pShadeTable, fIgnoreShadows);
													}
													else {
														Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, hVObjectAlpha, sXPos, sYPos, usImageIndex, &gClippingRect, sMultiTransShadowZBlitterIndex, pShadeTable, fIgnoreShadows);
													}
												}
												else
												{
													if (hVObjectAlpha == NULL) {
														Blt8BPPDataTo16BPPBufferTransZTransShadowIncClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect, sMultiTransShadowZBlitterIndex, pShadeTable, fIgnoreShadows);
													}
													else {
														Blt8BPPDataTo16BPPBufferTransZTransShadowIncClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, hVObjectAlpha, sXPos, sYPos, usImageIndex, &gClippingRect, sMultiTransShadowZBlitterIndex, pShadeTable, fIgnoreShadows);
													}
												}
											}
											else
											{
												//Blt8BPPDataTo16BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect );
											}
										}
										else if (fMultiZBlitter)
										{
											if (fZBlitter)
											{
												if (fObscuredBlitter)
												{
													Blt8BPPDataTo16BPPBufferTransZIncObscureClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
												}
												else
												{
													if (fWallTile)
													{
														if (sZStripIndex == -1)
														{
															Blt8BPPDataTo16BPPBufferTransZIncClipZSameZBurnsThrough((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect, usImageIndex);
														}
														else
														{
															Blt8BPPDataTo16BPPBufferTransZIncClipZSameZBurnsThrough((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect, sZStripIndex);
														}
													}
													else
													{
														Blt8BPPDataTo16BPPBufferTransZIncClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
												}
											}
											else
											{
												Blt8BPPDataTo16BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
											}
										}
										else
										{
											bBlitClipVal = BltIsClippedOrOffScreen(hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

											if (bBlitClipVal == TRUE)
											{
												if (fPixelate)
												{
													if (fTranslucencyType)
													{
														//if(fZWrite)
														//	Blt8BPPDataTo16BPPBufferTransZClipTranslucent((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														//else
														Blt8BPPDataTo16BPPBufferTransZNBClipTranslucent((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
													else
													{
														//if(fZWrite)
														//	Blt8BPPDataTo16BPPBufferTransZClipPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														//else
														Blt8BPPDataTo16BPPBufferTransZNBClipPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
												}
												else if (fMerc)
												{
													if (fZBlitter)
													{
														if (fZWrite)
														{
															if (hVObjectAlpha != NULL)
															{
																Blt8BPPDataTo16BPPBufferTransShadowZClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																	hVObject,
																	hVObjectAlpha,
																	sXPos, sYPos,
																	usImageIndex,
																	&gClippingRect,
																	pShadeTable,
																	fIgnoreShadows);
															}
															else
															{
																Blt8BPPDataTo16BPPBufferTransShadowZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																	hVObject,
																	sXPos, sYPos,
																	usImageIndex,
																	&gClippingRect,
																	pShadeTable,
																	fIgnoreShadows);
															}
														}
														else
														{
															if (fObscuredBlitter)
															{
																if (hVObjectAlpha != NULL)
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNBObscuredClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		hVObjectAlpha,
																		sXPos, sYPos,
																		usImageIndex,
																		&gClippingRect,
																		pShadeTable,
																		fIgnoreShadows);
																}
																else
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNBObscuredClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		sXPos, sYPos,
																		usImageIndex,
																		&gClippingRect,
																		pShadeTable,
																		fIgnoreShadows);
																}
															}
															else
															{
																if (hVObjectAlpha != NULL)
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNBClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		hVObjectAlpha,
																		sXPos, sYPos,
																		usImageIndex,
																		&gClippingRect,
																		pShadeTable,
																		fIgnoreShadows);
																}
																else
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNBClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		sXPos, sYPos,
																		usImageIndex,
																		&gClippingRect,
																		pShadeTable,
																		fIgnoreShadows);
																}
															}
														}

														if ((uiLevelNodeFlags & LEVELNODE_UPDATESAVEBUFFERONCE))
														{
															pSaveBuf = LockVideoSurface(guiSAVEBUFFER, &uiSaveBufferPitchBYTES);

															// BLIT HERE
															if (hVObjectAlpha != NULL)
															{
																Blt8BPPDataTo16BPPBufferTransShadowClipAlpha((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES,
																	hVObject,
																	hVObjectAlpha,
																	sXPos, sYPos,
																	usImageIndex,
																	&gClippingRect,
																	pShadeTable,
																	fIgnoreShadows);
															}
															else
															{
																Blt8BPPDataTo16BPPBufferTransShadowClip((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES,
																	hVObject,
																	sXPos, sYPos,
																	usImageIndex,
																	&gClippingRect,
																	pShadeTable,
																	fIgnoreShadows);
															}

															UnLockVideoSurface(guiSAVEBUFFER);

															// Turn it off!
															pNode->uiFlags &= (~LEVELNODE_UPDATESAVEBUFFERONCE);
														}

													}
													else
													{
														if (hVObjectAlpha != NULL)
														{
															Blt8BPPDataTo16BPPBufferTransShadowClipAlpha((UINT16*)pDestBuf, uiDestPitchBYTES,
																hVObject,
																hVObjectAlpha,
																sXPos, sYPos,
																usImageIndex,
																&gClippingRect,
																pShadeTable,
																fIgnoreShadows);
														}
														else 
														{
															Blt8BPPDataTo16BPPBufferTransShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES,
																hVObject,
																sXPos, sYPos,
																usImageIndex,
																&gClippingRect,
																pShadeTable,
																fIgnoreShadows);
														}
													}
												}
												else if (fShadowBlitter)
												{
													if (fZBlitter)
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferShadowZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														else
															Blt8BPPDataTo16BPPBufferShadowZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
													else
													{
														Blt8BPPDataTo16BPPBufferShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
												}
												else if (fIntensityBlitter)
												{
													if (fZBlitter)
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferIntensityZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														else
															Blt8BPPDataTo16BPPBufferIntensityZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
													else
													{
														Blt8BPPDataTo16BPPBufferIntensityClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}
												}
												else if (fZBlitter)
												{
													if (fZWrite)
													{
														if (fObscuredBlitter)
														{
															Blt8BPPDataTo16BPPBufferTransZClipPixelateObscured((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														}
														else
														{
															Blt8BPPDataTo16BPPBufferTransZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
														}
													}
													else
													{
														Blt8BPPDataTo16BPPBufferTransZNBClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
													}

													if ((uiLevelNodeFlags & LEVELNODE_UPDATESAVEBUFFERONCE))
													{
														pSaveBuf = LockVideoSurface(guiSAVEBUFFER, &uiSaveBufferPitchBYTES);

														// BLIT HERE
														Blt8BPPDataTo16BPPBufferTransZClip((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

														UnLockVideoSurface(guiSAVEBUFFER);
													}

												}
												else
													Blt8BPPDataTo16BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

											}
											else if (bBlitClipVal == FALSE)
											{
												if (fPixelate)
												{
													if (fTranslucencyType)
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferTransZTranslucent((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														else
															Blt8BPPDataTo16BPPBufferTransZNBTranslucent((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
													}
													else
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferTransZPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														else
															Blt8BPPDataTo16BPPBufferTransZNBPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
													}
												}
												else if (fMerc)
												{
													// Flugente: draw riot shield UNDER the soldier
													if ( pSoldier &&
														pSoldier->bVisible != -1 &&
														( pSoldier->ubDirection == NORTH ||
															pSoldier->ubDirection == NORTHWEST ||
															pSoldier->ubDirection == WEST )
														&& pSoldier->IsRiotShieldEquipped() )
													{
														ShowRiotShield( pSoldier, (UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel );
													}

													if (fZBlitter)
													{
														if (fZWrite)
														{
															if (hVObjectAlpha != NULL)
															{
																Blt8BPPDataTo16BPPBufferTransShadowZAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																	hVObject,
																	hVObjectAlpha,
																	sXPos, sYPos,
																	usImageIndex,
																	pShadeTable,
																	fIgnoreShadows);
															}
															else
															{
																Blt8BPPDataTo16BPPBufferTransShadowZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																	hVObject,
																	sXPos, sYPos,
																	usImageIndex,
																	pShadeTable,
																	fIgnoreShadows);
															}
														}
														else
														{
															if (fObscuredBlitter)
															{
																if (hVObjectAlpha != NULL) {
																	Blt8BPPDataTo16BPPBufferTransShadowZNBObscuredAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		hVObjectAlpha,
																		sXPos, sYPos,
																		usImageIndex,
																		pShadeTable,
																		fIgnoreShadows);
																}
																else {
																	Blt8BPPDataTo16BPPBufferTransShadowZNBObscured((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		sXPos, sYPos,
																		usImageIndex,
																		pShadeTable,
																		fIgnoreShadows);
																}
															}
															else
															{
																if (hVObjectAlpha != NULL)
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNBAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		hVObjectAlpha,
																		sXPos, sYPos,
																		usImageIndex,
																		pShadeTable,
																		fIgnoreShadows);
																}
																else
																{
																	Blt8BPPDataTo16BPPBufferTransShadowZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
																		hVObject,
																		sXPos, sYPos,
																		usImageIndex,
																		pShadeTable,
																		fIgnoreShadows);
																}
															}
														}

														if ((uiLevelNodeFlags & LEVELNODE_UPDATESAVEBUFFERONCE))
														{
															pSaveBuf = LockVideoSurface(guiSAVEBUFFER, &uiSaveBufferPitchBYTES);

															// BLIT HERE
															if (hVObjectAlpha != NULL)
															{
																Blt8BPPDataTo16BPPBufferTransShadowAlpha((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES,
																	hVObject,
																	hVObjectAlpha,
																	sXPos, sYPos,
																	usImageIndex,
																	pShadeTable,
																	fIgnoreShadows);
															}
															else
															{
																Blt8BPPDataTo16BPPBufferTransShadow((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES,
																	hVObject,
																	sXPos, sYPos,
																	usImageIndex,
																	pShadeTable,
																	fIgnoreShadows);
															}

															UnLockVideoSurface(guiSAVEBUFFER);

														}

													}
													else
													{
														if (hVObjectAlpha != NULL)
														{
															Blt8BPPDataTo16BPPBufferTransShadowAlpha((UINT16*)pDestBuf, uiDestPitchBYTES,
																hVObject,
																hVObjectAlpha,
																sXPos, sYPos,
																usImageIndex,
																pShadeTable,
																fIgnoreShadows);
														}
														else
														{
															Blt8BPPDataTo16BPPBufferTransShadow((UINT16*)pDestBuf, uiDestPitchBYTES,
																hVObject,
																sXPos, sYPos,
																usImageIndex,
																pShadeTable,
																fIgnoreShadows);
														}

													}

													// Flugente: draw riot shield OVER the soldier
													if ( pSoldier &&
														pSoldier->bVisible != -1 &&
														( pSoldier->ubDirection == EAST ||
															pSoldier->ubDirection == SOUTHEAST ||
															pSoldier->ubDirection == SOUTH ||
															pSoldier->ubDirection == SOUTHWEST ||
															pSoldier->ubDirection == NORTHEAST )
														&& pSoldier->IsRiotShieldEquipped() )
													{
														ShowRiotShield( pSoldier, (UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel );
													}
												}
												else if (fShadowBlitter)
												{
													if (fZBlitter)
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferShadowZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														else
															Blt8BPPDataTo16BPPBufferShadowZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
													}
													else
													{
														Blt8BPPDataTo16BPPBufferShadow((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex);
													}
												}
												else if (fIntensityBlitter)
												{
													if (fZBlitter)
													{
														if (fZWrite)
															Blt8BPPDataTo16BPPBufferIntensityZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														else
															Blt8BPPDataTo16BPPBufferIntensityZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
													}
													else
													{
														Blt8BPPDataTo16BPPBufferIntensity((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex);
													}
												}
												else if (fZBlitter)
												{
													if (fZWrite)
													{
														// TEST
														//Blt8BPPDataTo16BPPBufferTransZPixelate( (UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);

														if (fObscuredBlitter)
														{
															Blt8BPPDataTo16BPPBufferTransZPixelateObscured((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														}
														else
														{
															Blt8BPPDataTo16BPPBufferTransZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
														}
													}
													else
														Blt8BPPDataTo16BPPBufferTransZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);


													if ((uiLevelNodeFlags & LEVELNODE_UPDATESAVEBUFFERONCE))
													{
														pSaveBuf = LockVideoSurface(guiSAVEBUFFER, &uiSaveBufferPitchBYTES);

														// BLIT HERE
														Blt8BPPDataTo16BPPBufferTransZ((UINT16*)pSaveBuf, uiSaveBufferPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);

														UnLockVideoSurface(guiSAVEBUFFER);
													}

												}
												else
													Blt8BPPDataTo16BPPBufferTransparent((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex);
											}
										}
									}
									else // 8bpp section
									{
										if (fPixelate)
										{
											if (fZWrite)
												Blt8BPPDataTo8BPPBufferTransZClipPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
											else
												Blt8BPPDataTo8BPPBufferTransZNBClipPixelate((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
										}
										else if (BltIsClipped(hVObject, sXPos, sYPos, usImageIndex, &gClippingRect))
										{
											if (fMerc)
											{
												Blt8BPPDataTo8BPPBufferTransShadowZNBClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
													hVObject,
													sXPos, sYPos,
													usImageIndex,
													&gClippingRect,
													pShadeTable);
											}
											else if (fShadowBlitter)
												if (fZWrite)
													Blt8BPPDataTo8BPPBufferShadowZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
												else
													Blt8BPPDataTo8BPPBufferShadowZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);

											else if (fZBlitter)
											{
												if (fZWrite)
													Blt8BPPDataTo8BPPBufferTransZClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
												else
													Blt8BPPDataTo8BPPBufferTransZNBClip((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
											}
											else
												Blt8BPPDataTo8BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex, &gClippingRect);
										}
										else
										{
											if (fMerc)
											{

												// why to 16BPP here??
												if (hVObjectAlpha != NULL)
												{
													Blt8BPPDataTo16BPPBufferTransShadowZNBObscuredAlpha((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
														hVObject,
														hVObjectAlpha,
														sXPos, sYPos,
														usImageIndex,
														pShadeTable,
														fIgnoreShadows);
												}
												else
												{
													Blt8BPPDataTo16BPPBufferTransShadowZNBObscured((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
														hVObject,
														sXPos, sYPos,
														usImageIndex,
														pShadeTable,
														fIgnoreShadows);
												}

												//	Blt8BPPDataTo8BPPBufferTransShadowZNB( (UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel,
												//																							hVObject,
												//																							sXPos, sYPos,
												//																							usImageIndex,
												//																							pShadeTable);
											}
											else if (fShadowBlitter)
											{
												if (fZWrite)
													Blt8BPPDataTo8BPPBufferShadowZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
												else
													Blt8BPPDataTo8BPPBufferShadowZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
											}
											else if (fZBlitter)
											{
												if (fZWrite)
													Blt8BPPDataTo8BPPBufferTransZ((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
												else
													Blt8BPPDataTo8BPPBufferTransZNB((UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, hVObject, sXPos, sYPos, usImageIndex);
											}
											else
												Blt8BPPDataTo8BPPBufferTransparent((UINT16*)pDestBuf, uiDestPitchBYTES, hVObject, sXPos, sYPos, usImageIndex);
										}
									}

									// Flugente: additional decals
									if ( fWallTile )
									{
										ShowDecal( (UINT16*)pDestBuf, uiDestPitchBYTES, gpZBuffer, sZLevel, uiTileIndex );
									}
								}


								if (bt == NULL)
								{
									break;    // no logical body type found, no layers to render
								}
								layerIter++;
								}
								// background rect registration. Have to do this here, can't do it in the loop
								// as we don't know what the last rendered layer is upfront
								if (uiFlags & TILES_DIRTY)
								{
									if (!(uiLevelNodeFlags & LEVELNODE_LASTDYNAMIC))
									{
										sXPos = (INT16)backRect.left;
										sYPos = (INT16)backRect.top;
										RegisterBackgroundRect(uiDirtyFlags, NULL, sXPos, sYPos, (INT16)backRect.right, (INT16)(__min((INT16)(backRect.bottom), gsVIEWPORT_WINDOW_END_Y)));
										if (fSaveZ)
										{
											RegisterBackgroundRect(uiDirtyFlags | BGND_FLAG_SAVE_Z, NULL, sXPos, sYPos, (INT16)backRect.right, (INT16)(__min((INT16)(backRect.bottom), gsVIEWPORT_WINDOW_END_Y)));
										}
									}
								}
								// unset save buffer once flag after all layers have been rendered          }
								if ((uiLevelNodeFlags & LEVELNODE_UPDATESAVEBUFFERONCE))
								{
									pNode->uiFlags &= (~LEVELNODE_UPDATESAVEBUFFERONCE);
								}


								// RENDER APS ONTOP OF PLANNED MERC GUY
								if (fRenderTile && !(uiFlags&TILES_DIRTY))
								{
									if (fMerc)
									{
										if (pSoldier != NULL && pSoldier->ubID >= MAX_NUM_SOLDIERS)
										{
											SetFont(TINYFONT1);
											SetFontDestBuffer(guiSAVEBUFFER, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, gsVIEWPORT_WINDOW_END_Y, FALSE);
											VarFindFontCenterCoordinates(sXPos, sYPos, 1, 1, TINYFONT1, &sX, &sY, L"%d", pSoldier->ubPlannedUIAPCost);
											mprintf_buffer(pDestBuf, uiDestPitchBYTES, TINYFONT1, sX, sY, L"%d", pSoldier->ubPlannedUIAPCost);
											SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);
										}
									}
								}

							}

							if (!fLinkedListDirection)
								pNode = pNode->pPrevNode;
							else
								pNode = pNode->pNext;

							//pNode = NULL;
						}
					}
					else
					{
						if (gfEditMode)
						{
							//ATE: Used here in the editor to denote then an area is not in the world

							//Kris:  Fixed a couple things here...
							//  First, there was a problem with the FRAME_BUFFER already being locked which caused failures,
							//	and eventual crashes, so if it reaches this code, the buffer needs to be unlocked first, as
							//  it gets locked and unlocked internally within ColorFillVideoSurfaceArea().  I'm surprised
							//	this problem didn't surface a long time ago.  Anyway, it seems that scrolling to the bottom
							//	right hand corner of the map, would cause the end of the world to be drawn.  Now, this would
							//	only crash on my computer and not Emmons, so this should work.  Also, I changed the color
							//	from fluorescent green to black, which is easier on the eyes, and prevent the drawing of the
							//	end of the world if it would be drawn on the editor's taskbar.
							if (iTempPosY_S < INTERFACE_START_Y)
							{
								if (!(uiFlags&TILES_DIRTY))
									UnLockVideoSurface(FRAME_BUFFER);
								ColorFillVideoSurfaceArea(FRAME_BUFFER, iTempPosX_S, iTempPosY_S, (iTempPosX_S + 40),
									(min(iTempPosY_S + 20, INTERFACE_START_Y)), Get16BPPColor(FROMRGB(0, 0, 0)));
								if (!(uiFlags&TILES_DIRTY))
									pDestBuf = LockVideoSurface(FRAME_BUFFER, &uiDestPitchBYTES);
							}
						}
					}

					iTempPosX_S += 40;
					iTempPosX_M++;
					iTempPosY_M--;

					if (iTempPosX_S >= iEndXS)
					{
						fEndRenderRow = TRUE;
					}

				} while (!fEndRenderRow);

			}

		}
		//	} while( FALSE );

		if (bXOddFlag > 0)
		{
			iAnchorPosY_M++;
		}
		else
		{
			iAnchorPosX_M++;
		}

		bXOddFlag = !bXOddFlag;
		iAnchorPosY_S += 10;

		if (iAnchorPosY_S >= iEndYS)
		{
			fEndRenderCol = TRUE;
		}
	}
	while (!fEndRenderCol);

	if (!(uiFlags&TILES_DIRTY))
		UnLockVideoSurface(FRAME_BUFFER);

	if (uiFlags & TILES_DYNAMIC_CHECKFOR_INT_TILE)
	{
		EndCurInteractiveTileCheck();
	}


}


void DeleteFromWorld( UINT16 usTileIndex, UINT32 uiRenderTiles, UINT16 usIndex )
{
	switch( uiRenderTiles )
	{
		case TILES_DYNAMIC_LAND:
		case TILES_STATIC_LAND:
			RemoveLand( usTileIndex, usIndex );
			break;
		case TILES_DYNAMIC_OBJECTS:
		case TILES_STATIC_OBJECTS:
			RemoveObject( usTileIndex, usIndex );
			break;
		case TILES_STATIC_STRUCTURES:
		case TILES_DYNAMIC_STRUCTURES:
			RemoveStruct( usTileIndex, usIndex );
			break;
		case TILES_DYNAMIC_ROOF:
		case TILES_STATIC_ROOF:
			RemoveRoof( usTileIndex, usIndex );
			break;
		case TILES_STATIC_ONROOF:
			RemoveOnRoof( usTileIndex, usIndex );
			break;

		case TILES_DYNAMIC_TOPMOST:
		case TILES_STATIC_TOPMOST:
			RemoveTopmost( usTileIndex, usIndex );
			break;

	}

}



// memcpy's the background to the new scroll position, and renders the missing strip
// via the RenderStaticWorldRect. Dynamic stuff will be updated on the next frame
// by the normal render cycle
void ScrollBackground(UINT32 uiDirection, INT16 sScrollXIncrement, INT16 sScrollYIncrement )
{

	//RestoreBackgroundRects();

	if ( !gfDoVideoScroll )
	{
		// Clear z-buffer
		memset(gpZBuffer, LAND_Z_LEVEL, SCREEN_WIDTH*2* gsVIEWPORT_END_Y );

		RenderStaticWorldRect( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y, FALSE );

		FreeBackgroundRectType(BGND_FLAG_ANIMATED);
	}
	else
	{
		if ( gfRenderScroll == FALSE )
		{
			guiScrollDirection   = uiDirection;
			gfScrollStart				 = TRUE;
			gsScrollXIncrement	 = 0;
			gsScrollYIncrement	 = 0;
		}
		else
		{
			guiScrollDirection   |= uiDirection;
			gfScrollStart = FALSE;

		}

		gfRenderScroll =	TRUE;
		gsScrollXIncrement	+= sScrollXIncrement;
		gsScrollYIncrement	+= sScrollYIncrement;
	}


}

// Render routine takes center X, Y and Z coordinate and gets world
// Coordinates for the window from that using the following functions
// For coordinate transformations

void RenderWorld( )
{
TILE_ELEMENT					*TileElem;
TILE_ANIMATION_DATA		*pAnimData;
UINT32 cnt = 0;

	gfRenderFullThisFrame = FALSE;

	// If we are testing renderer, set background to pink!
	if ( gTacticalStatus.uiFlags & DEBUGCLIFFS )
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH,	gsVIEWPORT_WINDOW_END_Y, Get16BPPColor( FROMRGB( 0, 255, 0 ) ) );
		SetRenderFlags(RENDER_FLAG_FULL);
	}

	if ( gTacticalStatus.uiFlags & SHOW_Z_BUFFER )
	{
		SetRenderFlags(RENDER_FLAG_FULL);
	}


//	SetRenderFlags(RENDER_FLAG_FULL);



	// FOR NOW< HERE, UPDATE ANIMATED TILES
	if ( COUNTERDONE( ANIMATETILES ) )
	{
		RESETCOUNTER( ANIMATETILES );

		while( cnt < gusNumAnimatedTiles )
		{
			TileElem = &(gTileDatabase[ gusAnimatedTiles[ cnt ] ] );

			pAnimData = TileElem->pAnimData;

			Assert( pAnimData != NULL );

			pAnimData->bCurrentFrame++;

			if ( pAnimData->bCurrentFrame >= pAnimData->ubNumFrames )
					pAnimData->bCurrentFrame = 0;
			cnt++;
		}
	}

	// HERE, UPDATE GLOW INDEX
	if ( COUNTERDONE( GLOW_ENEMYS ) )
	{
		RESETCOUNTER( GLOW_ENEMYS );

		gsCurrentGlowFrame++;

		if ( gsCurrentGlowFrame == NUM_GLOW_FRAMES )
		{
			gsCurrentGlowFrame = 0;
		}

		gsCurrentItemGlowFrame++;

		if ( gsCurrentItemGlowFrame == NUM_ITEM_CYCLE_COLORS )
		{
			gsCurrentItemGlowFrame = 0;
		}

	}


	//RenderStaticWorldRect( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );
	//AddBaseDirtyRect(gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );
	//return;


//#if 0

	if(gRenderFlags&RENDER_FLAG_FULL)
	{
		gfRenderFullThisFrame = TRUE;

		gfTopMessageDirty = TRUE;

		// Dirty the interface...
		fInterfacePanelDirty = DIRTYLEVEL2;

		// Apply scrolling sets some world variables
		ApplyScrolling( gsRenderCenterX, gsRenderCenterY, TRUE, FALSE );
		ResetLayerOptimizing();

		if ( (gRenderFlags&RENDER_FLAG_NOZ ) )
		{
			RenderStaticWorldRect( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y, FALSE );

		}
		else
		{
			RenderStaticWorld();

		}


		if(!(gRenderFlags&RENDER_FLAG_SAVEOFF))
			UpdateSaveBuffer();


	}
	else if(gRenderFlags&RENDER_FLAG_MARKED)
	{
		ResetLayerOptimizing();
		RenderMarkedWorld();
		if(!(gRenderFlags&RENDER_FLAG_SAVEOFF))
			UpdateSaveBuffer();

	}

	if ( gfScrollInertia == FALSE || (gRenderFlags&RENDER_FLAG_NOZ ) || (gRenderFlags&RENDER_FLAG_FULL ) || (gRenderFlags&RENDER_FLAG_MARKED ) )
	{
		RenderDynamicWorld( );

///////////////////////////////////////////////////////////

//	ColorFillVideoSurfaceArea( guiSAVEBUFFER, 0, gsVIEWPORT_WINDOW_END_Y, BP_SCREEN_WIDTH_CENTERED, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 16, 8, 0 ) ) );

///////////////////////////////////////////////////////////
	}

	if ( gfScrollInertia )
	{
  	EmptyBackgroundRects( );
  }

	if( gRenderFlags&RENDER_FLAG_ROOMIDS )
	{
		RenderRoomInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
	}

#ifdef _DEBUG
	if( gRenderFlags&RENDER_FLAG_FOVDEBUG )
	{
		RenderFOVDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
	}
	else if (gfDisplayCoverValues)
	{
		RenderCoverDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
	}
	else if (gfDisplayGridNoVisibleValues)
	{
		RenderGridNoVisibleDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
	}

#endif

//#endif

	//RenderStaticWorldRect( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );
	//AddBaseDirtyRect(gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );


	if(gRenderFlags&RENDER_FLAG_MARKED)
		ClearMarkedTiles();

	if ( gRenderFlags&RENDER_FLAG_CHECKZ && !(gTacticalStatus.uiFlags & NOHIDE_REDUNDENCY) )
	{
		ExamineZBufferRect( gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );
	}


	gRenderFlags&=(~(RENDER_FLAG_FULL|RENDER_FLAG_MARKED|RENDER_FLAG_ROOMIDS|RENDER_FLAG_CHECKZ));

	if ( gTacticalStatus.uiFlags & SHOW_Z_BUFFER )
	{
		// COPY Z BUFFER TO FRAME BUFFER
		UINT32	uiDestPitchBYTES;
		UINT16		*pDestBuf;
		UINT32	cnt;
		INT16		zVal;

		pDestBuf = (UINT16 *)LockVideoSurface(guiRENDERBUFFER, &uiDestPitchBYTES);

		for ( cnt = 0; cnt < (UINT32)( SCREEN_WIDTH * SCREEN_HEIGHT ); cnt++ )
		{
			// Get Z value
			zVal = gpZBuffer[ cnt ];
			pDestBuf[cnt] = zVal;
		}

		UnLockVideoSurface(guiRENDERBUFFER);
	}

}

// Start with a center X,Y,Z world coordinate and render direction
// Determine WorldIntersectionPoint and the starting block from these
// Then render away!

//ok tutaj jest renderowane przy przesuwaniu
void RenderStaticWorldRect(INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, BOOLEAN fDynamicsToo )
{
	UINT32		uiLevelFlags[10];
	UINT16		sLevelIDs[10];
	UINT32		uiDestPitchBYTES;
	UINT8		*pDestBuf=NULL;

	// Calculate render starting parameters
	CalcRenderParameters( sLeft, sTop, sRight, sBottom );

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	Assert( pDestBuf);
	memset(gpZBuffer, LAND_Z_LEVEL, uiDestPitchBYTES * gsVIEWPORT_END_Y );
	UnLockVideoSurface( FRAME_BUFFER);


	// Reset layer optimizations
	ResetLayerOptimizing( );
//#if 0

	// rendering land
	uiLevelFlags[0]		=	TILES_STATIC_LAND;
	sLevelIDs	[0]		=	RENDER_STATIC_LAND;
	RenderTiles( 0 , gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags, sLevelIDs );
	
	//rendering objects
	uiLevelFlags[0]		=	TILES_STATIC_OBJECTS;
	sLevelIDs	[0]		=	RENDER_STATIC_OBJECTS;
	RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags, sLevelIDs );

	if(gRenderFlags&RENDER_FLAG_SHADOWS )
	{
		// If we have to we render shadows
		uiLevelFlags[0]	=	TILES_STATIC_SHADOWS;
		sLevelIDs	[0]	=	RENDER_STATIC_SHADOWS;
		RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags , sLevelIDs );
	}

	uiLevelFlags[0]		=	TILES_STATIC_STRUCTURES;
	sLevelIDs	[0]		=	RENDER_STATIC_STRUCTS;
	uiLevelFlags[1]		=	TILES_STATIC_ROOF;
	sLevelIDs	[1]		=	RENDER_STATIC_ROOF;
	uiLevelFlags[2]		=	TILES_STATIC_ONROOF;
	sLevelIDs	[2]		=	RENDER_STATIC_ONROOF;
	uiLevelFlags[3]		=	TILES_STATIC_TOPMOST;
	sLevelIDs	[3]		=	RENDER_STATIC_TOPMOST;
	RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 4, uiLevelFlags, sLevelIDs );

	//ATE: Do obsucred layer!
	uiLevelFlags[0] = TILES_STATIC_STRUCTURES;
	sLevelIDs[0]		= RENDER_STATIC_STRUCTS;
	uiLevelFlags[1] = TILES_STATIC_ONROOF;
	sLevelIDs[1]		= RENDER_STATIC_ONROOF;
	RenderTiles( TILES_OBSCURED,  gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 2, uiLevelFlags, sLevelIDs );


	uiLevelFlags[0] = TILES_DYNAMIC_MERCS;
	//uiLevelFlags[1] = TILES_DYNAMIC_HIGHMERCS;

	sLevelIDs[0]    = RENDER_DYNAMIC_MERCS;
	//sLevelIDs[1]		= RENDER_DYNAMIC_HIGHMERCS;
	RenderTiles( 0, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );


	if ( fDynamicsToo )
	{
		// DYNAMICS
		uiLevelFlags[0] = TILES_DYNAMIC_LAND;
		uiLevelFlags[1] = TILES_DYNAMIC_OBJECTS;
		uiLevelFlags[2] = TILES_DYNAMIC_SHADOWS;
		uiLevelFlags[3] = TILES_DYNAMIC_STRUCT_MERCS;
		uiLevelFlags[4] = TILES_DYNAMIC_MERCS;
		uiLevelFlags[5] = TILES_DYNAMIC_STRUCTURES;
		uiLevelFlags[6] = TILES_DYNAMIC_ROOF;
		uiLevelFlags[7] = TILES_DYNAMIC_HIGHMERCS;
		uiLevelFlags[8] = TILES_DYNAMIC_ONROOF;

		sLevelIDs[0]    = RENDER_DYNAMIC_LAND;
		sLevelIDs[1]    = RENDER_DYNAMIC_OBJECTS;
		sLevelIDs[2]    = RENDER_DYNAMIC_SHADOWS;
		sLevelIDs[3]    = RENDER_DYNAMIC_STRUCT_MERCS;
		sLevelIDs[4]    = RENDER_DYNAMIC_MERCS;
		sLevelIDs[5]    = RENDER_DYNAMIC_STRUCTS;
		sLevelIDs[6]    = RENDER_DYNAMIC_ROOF;
		sLevelIDs[7]    = RENDER_DYNAMIC_HIGHMERCS;
		sLevelIDs[8]    = RENDER_DYNAMIC_ONROOF;
		RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 9, uiLevelFlags , sLevelIDs );


		SumAddiviveLayerOptimization( );
	}

	ResetRenderParameters( );

	if ( !gfDoVideoScroll )
	{
		//AddBaseDirtyRect(gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );
		AddBaseDirtyRect(sLeft, sTop, sRight, sBottom );
	}

//#endif
}


// to jest wykonywane przy kazdym takcie zegara
void RenderStaticWorld(  )
{
	UINT32	uiLevelFlags[9];
	UINT16	sLevelIDs[9];
	UINT32	uiDestPitchBYTES;
	UINT8	*pDestBuf=NULL;

	// Calculate render starting parameters
	CalcRenderParameters( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );

	// Clear z-buffer
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	Assert( pDestBuf);
	memset(gpZBuffer, LAND_Z_LEVEL, uiDestPitchBYTES * gsVIEWPORT_END_Y );
	UnLockVideoSurface( FRAME_BUFFER);

	FreeBackgroundRectType(BGND_FLAG_ANIMATED);
	InvalidateBackgroundRects();

	// rendering land tiles
	uiLevelFlags[0]		=	TILES_STATIC_LAND;
	sLevelIDs	[0]		=	RENDER_STATIC_LAND;
	RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags, sLevelIDs );

	//rendering objects
	uiLevelFlags[0]		=	TILES_STATIC_OBJECTS;
	sLevelIDs	[0]		=	RENDER_STATIC_OBJECTS;
	RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags, sLevelIDs );

	// If we have to we render shadows
	if(gRenderFlags&RENDER_FLAG_SHADOWS )
	{
		uiLevelFlags[0]	=	TILES_STATIC_SHADOWS;
		sLevelIDs	[0]	=	RENDER_STATIC_SHADOWS;
		RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 1, uiLevelFlags , sLevelIDs );
	}

	uiLevelFlags[0]		=	TILES_STATIC_STRUCTURES;
	sLevelIDs	[0]		=	RENDER_STATIC_STRUCTS;
	uiLevelFlags[1]		=	TILES_STATIC_ROOF;
	sLevelIDs	[1]		=	RENDER_STATIC_ROOF;
	uiLevelFlags[2]		=	TILES_STATIC_ONROOF;
	sLevelIDs	[2]		=	RENDER_STATIC_ONROOF;
	uiLevelFlags[3]		=	TILES_STATIC_TOPMOST;
	sLevelIDs	[3]		=	RENDER_STATIC_TOPMOST;
	RenderTiles( 0, gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 4, uiLevelFlags, sLevelIDs );

	//ATE: Do obsucred layer!
	uiLevelFlags[0] = TILES_STATIC_STRUCTURES;
	sLevelIDs[0]		= RENDER_STATIC_STRUCTS;
	uiLevelFlags[1] = TILES_STATIC_ONROOF;
	sLevelIDs[1]		= RENDER_STATIC_ONROOF;
	RenderTiles( TILES_OBSCURED,  gsLStartPointX_M, gsLStartPointY_M, gsLStartPointX_S, gsLStartPointY_S, gsLEndXS, gsLEndYS, 2, uiLevelFlags, sLevelIDs );


	AddBaseDirtyRect(gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );
	ResetRenderParameters( );

}

void RenderMarkedWorld(void)
{
	UINT32 uiLevelFlags[4];
	UINT16		sLevelIDs[4];

	CalcRenderParameters( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );



	RestoreBackgroundRects();
	FreeBackgroundRectType(BGND_FLAG_ANIMATED);
	InvalidateBackgroundRects();

	ResetLayerOptimizing();

	uiLevelFlags[0] = TILES_STATIC_LAND;
	uiLevelFlags[1] = TILES_STATIC_OBJECTS;
	sLevelIDs[0]		= RENDER_STATIC_LAND;
	sLevelIDs[1]		= RENDER_STATIC_OBJECTS;
	RenderTiles(TILES_MARKED,  gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 2, uiLevelFlags, sLevelIDs );

	if(gRenderFlags&RENDER_FLAG_SHADOWS)
	{
		uiLevelFlags[0] = TILES_STATIC_SHADOWS;
		sLevelIDs[0]		= RENDER_STATIC_SHADOWS;
		RenderTiles(TILES_MARKED, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );
	}

	uiLevelFlags[0] = TILES_STATIC_STRUCTURES;
	sLevelIDs[0]		= RENDER_STATIC_STRUCTS;
	RenderTiles(TILES_MARKED, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );

	uiLevelFlags[0] = TILES_STATIC_ROOF;
	sLevelIDs[0]		= RENDER_STATIC_ROOF;
	RenderTiles(TILES_MARKED, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );

	uiLevelFlags[0] = TILES_STATIC_ONROOF;
	sLevelIDs[0]		= RENDER_STATIC_ONROOF;
	RenderTiles(TILES_MARKED, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );

	uiLevelFlags[0] = TILES_STATIC_TOPMOST;
	sLevelIDs[0]		= RENDER_STATIC_TOPMOST;
	RenderTiles(TILES_MARKED, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );


	 AddBaseDirtyRect(gsVIEWPORT_START_X, gsVIEWPORT_WINDOW_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );


	ResetRenderParameters( );

}

void RenderDynamicWorld(  )
{
	UINT8		ubNumLevels;
	UINT32	uiLevelFlags[ 10 ];
	UINT16	sLevelIDs[ 10 ];

	CalcRenderParameters( gsVIEWPORT_START_X, gsVIEWPORT_START_Y, gsVIEWPORT_END_X, gsVIEWPORT_END_Y );

	RestoreBackgroundRects();

	if(!gfTagAnimatedTiles)
	{
		uiLevelFlags[0] = TILES_DYNAMIC_OBJECTS;
		uiLevelFlags[1] = TILES_DYNAMIC_SHADOWS;
		uiLevelFlags[2] = TILES_DYNAMIC_STRUCT_MERCS;
		uiLevelFlags[3] = TILES_DYNAMIC_MERCS;
		uiLevelFlags[4] = TILES_DYNAMIC_STRUCTURES;
		uiLevelFlags[5] = TILES_DYNAMIC_HIGHMERCS;
		uiLevelFlags[6] = TILES_DYNAMIC_ROOF;
		uiLevelFlags[7] = TILES_DYNAMIC_ONROOF;
		uiLevelFlags[8] = TILES_DYNAMIC_TOPMOST;

		sLevelIDs[0]		= RENDER_DYNAMIC_OBJECTS;
		sLevelIDs[1]		= RENDER_DYNAMIC_SHADOWS;
		sLevelIDs[2]		= RENDER_DYNAMIC_STRUCT_MERCS;
		sLevelIDs[3]		= RENDER_DYNAMIC_MERCS;
		sLevelIDs[4]		= RENDER_DYNAMIC_STRUCTS;
		sLevelIDs[5]		= RENDER_DYNAMIC_MERCS;
		sLevelIDs[6]		= RENDER_DYNAMIC_ROOF;
		sLevelIDs[7]		= RENDER_DYNAMIC_ONROOF;
		sLevelIDs[8]		= RENDER_DYNAMIC_TOPMOST;

		ubNumLevels = 9;
	}
	else
	{
		gfTagAnimatedTiles=FALSE;
		//uiLevelFlags[0] = TILES_DYNAMIC_LAND;
		uiLevelFlags[0] = TILES_DYNAMIC_OBJECTS;
		uiLevelFlags[1] = TILES_DYNAMIC_SHADOWS;
		uiLevelFlags[2] = TILES_DYNAMIC_STRUCT_MERCS;
		uiLevelFlags[3] = TILES_DYNAMIC_MERCS;
		uiLevelFlags[4] = TILES_DYNAMIC_STRUCTURES;
		uiLevelFlags[5] = TILES_DYNAMIC_HIGHMERCS;
		uiLevelFlags[6] = TILES_DYNAMIC_ROOF;
		uiLevelFlags[7] = TILES_DYNAMIC_ONROOF;
		uiLevelFlags[8] = TILES_DYNAMIC_TOPMOST;

		//sLevelIDs[0]		= RENDER_DYNAMIC_LAND;
		sLevelIDs[0]		= RENDER_DYNAMIC_OBJECTS;
		sLevelIDs[1]		= RENDER_DYNAMIC_SHADOWS;
		sLevelIDs[2]		= RENDER_DYNAMIC_STRUCT_MERCS;
		sLevelIDs[3]		= RENDER_DYNAMIC_MERCS;
		sLevelIDs[4]		= RENDER_DYNAMIC_STRUCTS;
		sLevelIDs[5]		= RENDER_DYNAMIC_MERCS;
		sLevelIDs[6]		= RENDER_DYNAMIC_ROOF;
		sLevelIDs[7]		= RENDER_DYNAMIC_ONROOF;
		sLevelIDs[8]		= RENDER_DYNAMIC_TOPMOST;

		ubNumLevels = 9;
	}

	RenderTiles(TILES_DIRTY, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, ubNumLevels, uiLevelFlags, sLevelIDs );

	#ifdef JA2EDITOR
	if( !gfEditMode && !gfAniEditMode )
	#endif
	{
		RenderTacticalInterface( );
	}

	SaveBackgroundRects();

	//uiLevelFlags[0] = TILES_DYNAMIC_LAND;
	uiLevelFlags[0] = TILES_DYNAMIC_OBJECTS;
	uiLevelFlags[1] = TILES_DYNAMIC_SHADOWS;
	uiLevelFlags[2] = TILES_DYNAMIC_STRUCT_MERCS;
	uiLevelFlags[3] = TILES_DYNAMIC_MERCS;
	uiLevelFlags[4] = TILES_DYNAMIC_STRUCTURES;

	//sLevelIDs[0]    = RENDER_DYNAMIC_LAND;
	sLevelIDs[0]    = RENDER_DYNAMIC_OBJECTS;
	sLevelIDs[1]    = RENDER_DYNAMIC_SHADOWS;
	sLevelIDs[2]    = RENDER_DYNAMIC_STRUCT_MERCS;
	sLevelIDs[3]    = RENDER_DYNAMIC_MERCS;
	sLevelIDs[4]    = RENDER_DYNAMIC_STRUCTS;

	RenderTiles( 0, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 5, uiLevelFlags , sLevelIDs );


	uiLevelFlags[0] = TILES_DYNAMIC_ROOF;
	uiLevelFlags[1] = TILES_DYNAMIC_HIGHMERCS;
	uiLevelFlags[2] = TILES_DYNAMIC_ONROOF;

	sLevelIDs[0]    = RENDER_DYNAMIC_ROOF;
	sLevelIDs[1]    = RENDER_DYNAMIC_HIGHMERCS;
	sLevelIDs[2]    = RENDER_DYNAMIC_ONROOF;

	RenderTiles(0 , gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 3, uiLevelFlags, sLevelIDs );

	uiLevelFlags[0] = TILES_DYNAMIC_TOPMOST;
	sLevelIDs[0]    = RENDER_DYNAMIC_TOPMOST;

	// ATE: check here for mouse over structs.....
	RenderTiles( TILES_DYNAMIC_CHECKFOR_INT_TILE, gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS, 1, uiLevelFlags, sLevelIDs );

	SumAddiviveLayerOptimization( );

	ResetRenderParameters( );

}




BOOLEAN HandleScrollDirections( UINT32 ScrollFlags, INT16 sScrollXStep, INT16 sScrollYStep, INT16 *psTempRenderCenterX, INT16 *psTempRenderCenterY, BOOLEAN fCheckOnly )
{
	BOOLEAN fAGoodMove = FALSE, fMovedPos = FALSE;
	INT16		sTempX_W, sTempY_W;
	BOOLEAN fUpOK, fLeftOK;
	BOOLEAN fDownOK, fRightOK;
	INT16		sTempRenderCenterX, sTempRenderCenterY;

	sTempRenderCenterX = sTempRenderCenterY = 0;

	// This checking sequence just validates the values!
	if ( ScrollFlags & SCROLL_LEFT )
	{
		FromScreenToCellCoordinates( (INT16)-sScrollXStep, 0, &sTempX_W, &sTempY_W );
		sTempRenderCenterX = gsRenderCenterX + sTempX_W;
		sTempRenderCenterY = gsRenderCenterY + sTempY_W;

		fMovedPos=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );
		if ( fMovedPos )
		{
			fAGoodMove = TRUE;
		}

		if ( !fCheckOnly )
		{
			ScrollBackground(SCROLL_LEFT, sScrollXStep, sScrollYStep );
		}

	}

	if ( ScrollFlags & SCROLL_RIGHT )
	{
			FromScreenToCellCoordinates( sScrollXStep, 0, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fMovedPos=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );
			if ( fMovedPos )
			{
				fAGoodMove = TRUE;
			}

			if ( !fCheckOnly )
			{
				ScrollBackground(SCROLL_RIGHT, sScrollXStep, sScrollYStep );
			}
	}

	if ( ScrollFlags & SCROLL_UP )
	{
			FromScreenToCellCoordinates( 0, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fMovedPos=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );
			if ( fMovedPos )
			{
				fAGoodMove = TRUE;
			}

			if ( !fCheckOnly )
			{
				ScrollBackground(SCROLL_UP, sScrollXStep, sScrollYStep );
			}
	}

	if ( ScrollFlags & SCROLL_DOWN )
	{
			FromScreenToCellCoordinates( 0, sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fMovedPos=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );
			if ( fMovedPos )
			{
				fAGoodMove = TRUE;
			}

			if ( !fCheckOnly )
			{
				ScrollBackground(SCROLL_DOWN, sScrollXStep, sScrollYStep );
			}

	}

	if ( ScrollFlags & SCROLL_UPLEFT )
	{
			// Check up
			FromScreenToCellCoordinates( 0, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fUpOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			// Check left
			FromScreenToCellCoordinates( (INT16)-sScrollXStep, 0, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fLeftOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );


			if ( fLeftOK && fUpOK )
			{
				FromScreenToCellCoordinates( (INT16)-sScrollXStep, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;
				fAGoodMove = TRUE;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_UPLEFT, sScrollXStep, sScrollYStep );
				}

			}
			else if ( fUpOK )
			{
				fAGoodMove = TRUE;

				FromScreenToCellCoordinates( 0, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_UP, sScrollXStep, sScrollYStep );
				}

			}
			else if ( fLeftOK )
			{
				fAGoodMove = TRUE;

				FromScreenToCellCoordinates( (INT16)-sScrollXStep, 0, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_LEFT, sScrollXStep, sScrollYStep );
				}
			}
	}

	if ( ScrollFlags & SCROLL_UPRIGHT )
	{

			// Check up
			FromScreenToCellCoordinates( 0, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fUpOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			// Check right
			FromScreenToCellCoordinates( sScrollXStep, 0, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fRightOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			if ( fUpOK && fRightOK )
			{
				FromScreenToCellCoordinates( (INT16)sScrollXStep, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;
				fAGoodMove = TRUE;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_UPRIGHT, sScrollXStep, sScrollYStep );
				}

			}
			else if ( fUpOK )
			{
				fAGoodMove = TRUE;

				FromScreenToCellCoordinates( 0, (INT16)-sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_UP, sScrollXStep, sScrollYStep );
				}
			}
			else if ( fRightOK )
			{
				fAGoodMove = TRUE;

				FromScreenToCellCoordinates( sScrollXStep, 0, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_RIGHT, sScrollXStep, sScrollYStep );
				}
			}
	}

	if ( ScrollFlags & SCROLL_DOWNLEFT )
	{
			// Check down......
			FromScreenToCellCoordinates( 0, sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fDownOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			// Check left.....
			FromScreenToCellCoordinates( (INT16)-sScrollXStep, 0, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fLeftOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			if ( fLeftOK && fDownOK )
			{
				fAGoodMove = TRUE;
				FromScreenToCellCoordinates( (INT16)-sScrollXStep, (INT16)sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;

				if ( !fCheckOnly )
				{
					ScrollBackground(SCROLL_DOWNLEFT, sScrollXStep, sScrollYStep );
				}

			}
			else if ( fLeftOK )
			{
					FromScreenToCellCoordinates( (INT16)-sScrollXStep, 0, &sTempX_W, &sTempY_W );
					sTempRenderCenterX = gsRenderCenterX + sTempX_W;
					sTempRenderCenterY = gsRenderCenterY + sTempY_W;
					fAGoodMove = TRUE;

					if ( !fCheckOnly )
					{
						ScrollBackground(SCROLL_LEFT, sScrollXStep, sScrollYStep );
					}
			}
			else if ( fDownOK )
			{
					FromScreenToCellCoordinates( 0, sScrollYStep, &sTempX_W, &sTempY_W );
					sTempRenderCenterX = gsRenderCenterX + sTempX_W;
					sTempRenderCenterY = gsRenderCenterY + sTempY_W;
					fAGoodMove = TRUE;

					if ( !fCheckOnly )
					{
						ScrollBackground(SCROLL_DOWN, sScrollXStep, sScrollYStep );
					}
			}
	}

	if ( ScrollFlags & SCROLL_DOWNRIGHT )
	{
			// Check right
			FromScreenToCellCoordinates( sScrollXStep, 0, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fRightOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			// Check down
			FromScreenToCellCoordinates( 0, sScrollYStep, &sTempX_W, &sTempY_W );
			sTempRenderCenterX = gsRenderCenterX + sTempX_W;
			sTempRenderCenterY = gsRenderCenterY + sTempY_W;
			fDownOK=ApplyScrolling( sTempRenderCenterX, sTempRenderCenterY, FALSE, fCheckOnly );

			if ( fDownOK && fRightOK )
			{
				FromScreenToCellCoordinates( (INT16)sScrollXStep, (INT16)sScrollYStep, &sTempX_W, &sTempY_W );
				sTempRenderCenterX = gsRenderCenterX + sTempX_W;
				sTempRenderCenterY = gsRenderCenterY + sTempY_W;
				fAGoodMove = TRUE;

				if ( !fCheckOnly )
				{
					ScrollBackground( SCROLL_DOWNRIGHT, sScrollXStep, sScrollYStep );
				}

			}
			else if ( fDownOK )
			{
					FromScreenToCellCoordinates( 0, sScrollYStep, &sTempX_W, &sTempY_W );
					sTempRenderCenterX = gsRenderCenterX + sTempX_W;
					sTempRenderCenterY = gsRenderCenterY + sTempY_W;
					fAGoodMove = TRUE;

					if ( !fCheckOnly )
					{
						ScrollBackground( SCROLL_DOWN, sScrollXStep, sScrollYStep );
					}
			}
			else if ( fRightOK )
			{
					FromScreenToCellCoordinates( sScrollXStep, 0, &sTempX_W, &sTempY_W );
					sTempRenderCenterX = gsRenderCenterX + sTempX_W;
					sTempRenderCenterY = gsRenderCenterY + sTempY_W;
					fAGoodMove = TRUE;

					if ( !fCheckOnly )
					{
						ScrollBackground( SCROLL_RIGHT, sScrollXStep, sScrollYStep );
					}
			}

	}

	( *psTempRenderCenterX ) = sTempRenderCenterX;
	( *psTempRenderCenterY ) = sTempRenderCenterY;


	return( fAGoodMove );
}

extern BOOLEAN gfNextRefreshFullScreen;

void ScrollWorld( )
{
	UINT32		ScrollFlags = 0;
	BOOLEAN		fDoScroll = FALSE, fAGoodMove = FALSE;
	INT16	sTempRenderCenterX, sTempRenderCenterY;
	INT8	bDirection;
	INT16	sScrollXStep=-1;
	INT16 sScrollYStep=-1;
	BOOLEAN		fIgnoreInput	= FALSE;
  static    UINT8   ubOldScrollSpeed = 0;
  static    BOOLEAN fFirstTimeInSlideToMode = TRUE;


	if ( gfIgnoreScrollDueToCenterAdjust )
	{
		//	gfIgnoreScrollDueToCenterAdjust = FALSE;
		return;
	}

	if ( gfIgnoreScrolling == 1 )
	{
		return;
	}

	if ( gfIgnoreScrolling == 2 )
	{
		fIgnoreInput = TRUE;
	}

	if ( gCurrentUIMode == LOCKUI_MODE )
	{
		fIgnoreInput = TRUE;
	}


	// If in editor, ignore scrolling if any of the shift keys pressed with arrow keys
	if ( gfEditMode && ( _KeyDown(CTRL) || _KeyDown(ALT) ) )
		return;

	// Ignore if ALT DONW
	// silversurfer: also ignore CTRL because this is now used for merc portrait switching
	if ( _KeyDown(ALT) || _KeyDown(CTRL) )
		return;


	do
	{

		if ( gfIgnoreScrolling != 3 )
		{
			// Check for sliding			
			if (!TileIsOutOfBounds(gTacticalStatus.sSlideTarget))
			{
				// Ignore all input...
				// Check if we have reached out dest!
				if ( fFirstTimeInSlideToMode )
				{
					ubOldScrollSpeed = gubCurScrollSpeedID;
					fFirstTimeInSlideToMode = FALSE;
				}

				// Make faster!
				//gubCurScrollSpeedID = 2;

				ScrollFlags = 0;
				fDoScroll = FALSE;
				//
				if ( SoldierLocationRelativeToScreen( gTacticalStatus.sSlideTarget, gTacticalStatus.sSlideReason, &bDirection, &ScrollFlags ) && GridNoOnVisibleWorldTile( gTacticalStatus.sSlideTarget ) )
				{
					ScrollFlags = gScrollDirectionFlags[ bDirection ];
					fDoScroll			= TRUE;
					fIgnoreInput	= TRUE;
				}
				else
				{
					// We've stopped!
					gTacticalStatus.sSlideTarget = NOWHERE;
				}
			}
			else
			{
				// Restore old scroll speed
				if ( !fFirstTimeInSlideToMode )
				{
					gubCurScrollSpeedID = ubOldScrollSpeed;
				}
				fFirstTimeInSlideToMode = TRUE;
			}
		}

		if ( !fIgnoreInput )
		{
			// Check keys
			if ( _KeyDown( UPARROW )  )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_UP;
			}


			if ( _KeyDown( DNARROW )  )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_DOWN;
			}


			if ( _KeyDown( RIGHTARROW )  )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_RIGHT;
			}


			if ( _KeyDown( LEFTARROW )  )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_LEFT;
			}


			//hayden - disable mouse scrolling for window dev ease
			if(iDisableMouseScrolling)break;


			// Do mouse - PUT INTO A TIMER!
			// Put a counter on starting from mouse, if we have not started already!
			if ( !gfScrollInertia && gfScrollPending == FALSE )
			{
				if ( !COUNTERDONE( STARTSCROLL ) )
				{
					break;
				}
				RESETCOUNTER( STARTSCROLL );

			}

			if ( gusMouseYPos == 0 )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_UP;
			}


			if ( gusMouseYPos >= SCREEN_HEIGHT - 1 )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_DOWN;
			}


			if ( gusMouseXPos >= SCREEN_WIDTH - 1 )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_RIGHT;
			}


			if ( gusMouseXPos == 0 )
			{
				fDoScroll = TRUE;
				ScrollFlags |= SCROLL_LEFT;
			}
		}

	} while( FALSE );


	if ( fDoScroll )
	{
			if ( gfDoSubtileScroll )
			{
				if ( gfScrollInertia > gubNewScrollIDSpeeds[ gubCurScrollSpeedID ] )
				{
					gubCurScrollSpeedID++;

					if ( gubCurScrollSpeedID > gubScrollSpeedEndID )
					{
						gubCurScrollSpeedID = gubScrollSpeedEndID;
					}
				}
			}

			//if ( !gfDoVideoScroll )
			//{
			//	gubCurScrollSpeedID = 4;
			//}


			// Adjust speed based on whether shift is down
			if ( _KeyDown( SHIFT ) )
			{
				sScrollXStep = gubNewScrollXSpeeds[ gfDoVideoScroll ][ 3 ];
				sScrollYStep = gubNewScrollYSpeeds[ gfDoVideoScroll ][ 3 ];
			}
			else
			{
				sScrollXStep = gubNewScrollXSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ];
				sScrollYStep = gubNewScrollYSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ];
			}


			// Set diagonal flags!
			if ( ( ScrollFlags & SCROLL_LEFT ) && ( ScrollFlags & SCROLL_UP ) )
			{
				ScrollFlags = SCROLL_UPLEFT;
			}
			if ( ( ScrollFlags & SCROLL_RIGHT ) && ( ScrollFlags & SCROLL_UP ) )
			{
				ScrollFlags = SCROLL_UPRIGHT;
			}
			if ( ( ScrollFlags & SCROLL_LEFT ) && ( ScrollFlags & SCROLL_DOWN ) )
			{
				ScrollFlags = SCROLL_DOWNLEFT;
			}
			if ( ( ScrollFlags & SCROLL_RIGHT ) && ( ScrollFlags & SCROLL_DOWN ) )
			{
				ScrollFlags = SCROLL_DOWNRIGHT;
			}

			fAGoodMove = HandleScrollDirections( ScrollFlags, sScrollXStep, sScrollYStep, &sTempRenderCenterX, &sTempRenderCenterY, TRUE );
	}

	// Has this been an OK scroll?
	if ( fAGoodMove )
	{
		if ( COUNTERDONE( NEXTSCROLL ) )
		{
			RESETCOUNTER( NEXTSCROLL );

			// Are we starting a new scroll?
			if ( gfScrollInertia == 0 && gfScrollPending == FALSE )
			{
					// We are starting to scroll - setup scroll pending
					gfScrollPending = TRUE;

					// Remove any interface stuff
					ClearInterface( );

					// Return so that next frame things will be erased!
					return;
			}

			// If here, set scroll pending to false
			gfScrollPending = FALSE;

			// INcrement scroll intertia
			gfScrollInertia++;
			if(gfScrollInertia == 1)//dnl ch78 271113
				RestoreBackgroundRects();

			// Now we actually begin our scrolling
			HandleScrollDirections( ScrollFlags, sScrollXStep, sScrollYStep, &sTempRenderCenterX, &sTempRenderCenterY, FALSE );

			if( gfNextRefreshFullScreen ) SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
	else
	{
		// ATE: Also if scroll pending never got to scroll....
		if ( gfScrollPending == TRUE )
		{
			// Do a complete rebuild!
			gfScrollPending = FALSE;

			// Restore Interface!
			RestoreInterface( );

			// Delete Topmost blitters saved areas
			DeleteVideoOverlaysArea( );

		}

		// Check if we have just stopped scrolling!
		if ( gfScrollInertia != FALSE  )
		{
			SetRenderFlags( RENDER_FLAG_FULL | RENDER_FLAG_CHECKZ );

			// Restore Interface!
			RestoreInterface( );

			// Delete Topmost blitters saved areas
			DeleteVideoOverlaysArea( );

		}

		gfScrollInertia = FALSE;
		gfScrollPending = FALSE;

		if ( gfDoSubtileScroll )
		{
			gubCurScrollSpeedID = gubScrollSpeedStartID;
		}

	}
}


/* Initialization
 *
 * any questions? joker
 */

void InitializeViewPort()
{
	gOldClipRect.iLeft = 0;
	gOldClipRect.iTop = 0;
	gOldClipRect.iRight = SCREEN_WIDTH;
	gOldClipRect.iBottom = SCREEN_HEIGHT;

	gsVIEWPORT_START_X			= 0;
	gsVIEWPORT_START_Y			= 0;
	gsVIEWPORT_WINDOW_START_Y	= 0;
	gsVIEWPORT_WINDOW_END_Y		= INTERFACE_START_Y;
	gsVIEWPORT_END_X			= SCREEN_WIDTH;
	gsVIEWPORT_END_Y			= INTERFACE_START_Y;

	gClippingRect.iLeft		= 0;
	gClippingRect.iTop		= 0;
	gClippingRect.iRight	= SCREEN_WIDTH;
	gClippingRect.iBottom	= SCREEN_HEIGHT - INTERFACE_HEIGHT;
}

void InitRenderParams( UINT8 ubRestrictionID )
{
		INT16 gsTilesX, gsTilesY;
		UINT32 cnt, cnt2;
		DOUBLE  dWorldX, dWorldY;

		switch( ubRestrictionID )
		{
			case 0:		//Default!

				gTopLeftWorldLimitX = CELL_X_SIZE;
				gTopLeftWorldLimitY = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;

				gTopRightWorldLimitX = ( WORLD_COLS / 2 ) * CELL_Y_SIZE;
				gTopRightWorldLimitY = CELL_X_SIZE;

				gBottomLeftWorldLimitX = ( ( WORLD_COLS / 2 ) * CELL_Y_SIZE );
				gBottomLeftWorldLimitY = ( WORLD_ROWS * CELL_Y_SIZE );

				gBottomRightWorldLimitX = ( WORLD_COLS * CELL_Y_SIZE );
				gBottomRightWorldLimitY = ( ( WORLD_ROWS / 2 ) * CELL_X_SIZE );
				break;

			/* // Buggler: Commented out restricted Scroll ID/viewport as viewport for A10_B1 map shifts around in non-640x480 resolutions
			// no issue on having the usual scrollable viewport both during gameplay and in mapeditor
			// code may served as reference for fixing viewport issues larger than standard-sized maps on very high resolutions, e.g. UHD.
			case 1:		// BASEMENT LEVEL 1

				gTopLeftWorldLimitX = ( 3 * WORLD_ROWS / 10 ) * CELL_X_SIZE;
				gTopLeftWorldLimitY = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;

				gTopRightWorldLimitX = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;
				gTopRightWorldLimitY = ( 3 * WORLD_COLS / 10 ) * CELL_X_SIZE;

				gBottomLeftWorldLimitX = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;
				gBottomLeftWorldLimitY = ( 7 * WORLD_COLS / 10 ) * CELL_X_SIZE;

				gBottomRightWorldLimitX = ( 7 * WORLD_ROWS / 10 ) * CELL_X_SIZE;
				gBottomRightWorldLimitY = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;
				break;
			*/

			default:		//Default!

				gTopLeftWorldLimitX = CELL_X_SIZE;
				gTopLeftWorldLimitY = ( WORLD_ROWS / 2 ) * CELL_X_SIZE;

				gTopRightWorldLimitX = ( WORLD_COLS / 2 ) * CELL_Y_SIZE;
				gTopRightWorldLimitY = CELL_X_SIZE;

				gBottomLeftWorldLimitX = ( ( WORLD_COLS / 2 ) * CELL_Y_SIZE );
				gBottomLeftWorldLimitY = ( WORLD_ROWS * CELL_Y_SIZE );

				gBottomRightWorldLimitX = ( WORLD_COLS * CELL_Y_SIZE );
				gBottomRightWorldLimitY = ( ( WORLD_ROWS / 2 ) * CELL_X_SIZE );
				break;

		}

		gCenterWorldX = ( WORLD_ROWS ) / 2 * CELL_X_SIZE;
		gCenterWorldY = ( WORLD_COLS ) / 2 * CELL_Y_SIZE;

		// Convert Bounding box into screen coords
		FromCellToScreenCoordinates( gTopLeftWorldLimitX, gTopLeftWorldLimitY, &gsTLX, &gsTLY );
		FromCellToScreenCoordinates( gTopRightWorldLimitX, gTopRightWorldLimitY, &gsTRX, &gsTRY );
		FromCellToScreenCoordinates( gBottomLeftWorldLimitX, gBottomLeftWorldLimitY, &gsBLX, &gsBLY );
		FromCellToScreenCoordinates( gBottomRightWorldLimitX, gBottomRightWorldLimitY, &gsBRX, &gsBRY );
		FromCellToScreenCoordinates( gCenterWorldX , gCenterWorldY, &gsCX, &gsCY );

		// Adjust for interface height tabbing!
		gsTLY += ROOF_LEVEL_HEIGHT;
		gsTRY += ROOF_LEVEL_HEIGHT;
		gsCY  += ( ROOF_LEVEL_HEIGHT / 2 );

		// Take these spaning distances and determine # tiles spaning
		gsTilesX = ( gsTRX - gsTLX ) / WORLD_TILE_X;
		gsTilesY = ( gsBRY - gsTRY ) / WORLD_TILE_Y;

		DebugMsg(TOPIC_JA2, DBG_LEVEL_0, String("World Screen Width %d Height %d", ( gsTRX - gsTLX ), ( gsBRY - gsTRY )));


		// Determine scale factors
		// First scale world screen coords for VIEWPORT ratio
		dWorldX = (DOUBLE)( gsTRX - gsTLX );
		dWorldY = (DOUBLE)( gsBRY - gsTRY );

		gdScaleX = (DOUBLE)RADAR_WINDOW_WIDTH /  dWorldX;
		gdScaleY = (DOUBLE)RADAR_WINDOW_HEIGHT / dWorldY;

		for ( cnt = 0, cnt2 = 0; cnt2 < NUM_ITEM_CYCLE_COLORS; cnt+=3, cnt2++ )
		{
			us16BPPItemCycleWhiteColors[ cnt2 ] = Get16BPPColor( FROMRGB( ubRGBItemCycleWhiteColors[ cnt ], ubRGBItemCycleWhiteColors[ cnt + 1 ], ubRGBItemCycleWhiteColors[ cnt + 2] ) );
			us16BPPItemCycleRedColors[ cnt2 ] = Get16BPPColor( FROMRGB( ubRGBItemCycleRedColors[ cnt ], ubRGBItemCycleRedColors[ cnt + 1 ], ubRGBItemCycleRedColors[ cnt + 2] ) );
			us16BPPItemCycleYellowColors[ cnt2 ] = Get16BPPColor( FROMRGB( ubRGBItemCycleYellowColors[ cnt ], ubRGBItemCycleYellowColors[ cnt + 1 ], ubRGBItemCycleYellowColors[ cnt + 2] ) );
		}

		gsLobOutline = Get16BPPColor( FROMRGB( 10, 200, 10 ) );
		gsThrowOutline = Get16BPPColor( FROMRGB( 253, 212, 10 ) );
		gsGiveOutline = Get16BPPColor( FROMRGB( 253, 0, 0 ) );

		gusNormalItemOutlineColor = Get16BPPColor( FROMRGB( 255, 255, 255 ) );
		gusYellowItemOutlineColor = Get16BPPColor( FROMRGB( 255, 255, 0 ) );

		// NOW GET DISTANCE SPANNING WORLD LIMITS IN WORLD COORDS
		//FromScreenToCellCoordinates( ( gTopRightWorldLimitX - gTopLeftWorldLimitX ), ( gTopRightWorldLimitY - gTopLeftWorldLimitY ), &gsWorldSpanX, &gsWorldSpanY );

		// CALCULATE 16BPP COLORS FOR ITEMS

}

// WANNE: Scrolling: Only scroll, if the map is larger than the radar map
// For example: Do not allow scrolling in Rebel Basement.
// Appy? HEahehahehahehae.....
BOOLEAN ApplyScrolling( INT16 sTempRenderCenterX, INT16 sTempRenderCenterY, BOOLEAN fForceAdjust, BOOLEAN fCheckOnly )
{
	BOOLEAN		fScrollGood = FALSE;
	BOOLEAN		fOutLeft = FALSE;
	BOOLEAN		fOutRight = FALSE;
	BOOLEAN		fOutTop = FALSE;
	BOOLEAN		fOutBottom = FALSE;


	double	dOpp, dAdj, dAngle;

	INT16 sTopLeftWorldX, sTopLeftWorldY;
	INT16 sTopRightWorldX, sTopRightWorldY;
	INT16 sBottomLeftWorldX, sBottomLeftWorldY;
	INT16 sBottomRightWorldX, sBottomRightWorldY;

	INT16 sTempPosX_W, sTempPosY_W;


	// For debug text for all 4 angles
	double at1, at2, at3, at4;

	INT16	sX_S, sY_S;
	INT16 sScreenCenterX, sScreenCenterY;
	INT16 sDistToCenterY, sDistToCenterX;
	INT16 sNewScreenX, sNewScreenY;
	INT16	sMult;

	INT16 sRadarTLX, sRadarTLY;
	INT16 sRadarBRX, sRadarBRY;
	INT16 sRadarCX, sRadarCY;
	INT16 sHeight, sWidth, sX, sY;
	// CHRISL:
	INT16			gsRadarX;
	INT16			gsRadarY;
	BOOLEAN			fAllowScrollingHorizontal = FALSE;
	BOOLEAN			fAllowScrollingVertical = FALSE;


	//Makesure it's a multiple of 5
	sMult = sTempRenderCenterX / CELL_X_SIZE;
	sTempRenderCenterX = ( sMult * CELL_X_SIZE ) + ( CELL_X_SIZE / 2 );

	//Makesure it's a multiple of 5
	sMult = sTempRenderCenterY / CELL_Y_SIZE;
	sTempRenderCenterY = ( sMult * CELL_Y_SIZE ) + ( CELL_Y_SIZE / 2 );


	// Find the diustance from render center to true world center
	sDistToCenterX = sTempRenderCenterX - gCenterWorldX;
	sDistToCenterY = sTempRenderCenterY - gCenterWorldY;

	// From render center in world coords, convert to render center in "screen" coords
	FromCellToScreenCoordinates( sDistToCenterX , sDistToCenterY, &sScreenCenterX, &sScreenCenterY );

	// Subtract screen center
	sScreenCenterX += gsCX;
	sScreenCenterY += gsCY;

	// Adjust for offset position on screen
	sScreenCenterX -= 0;
	sScreenCenterY -= 10;


	// Get corners in screen coords
	// TOP LEFT
	sX_S = ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2;
	sY_S = ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2;

	sTopLeftWorldX = sScreenCenterX  - sX_S;
	sTopLeftWorldY = sScreenCenterY  - sY_S;

	sTopRightWorldX = sScreenCenterX  + sX_S;
	sTopRightWorldY = sScreenCenterY  - sY_S;

	sBottomLeftWorldX = sScreenCenterX  - sX_S;
	sBottomLeftWorldY = sScreenCenterY  + sY_S;

	sBottomRightWorldX = sScreenCenterX  + sX_S;
	sBottomRightWorldY = sScreenCenterY  + sY_S;


	// WANNE: Scrolling bugfix, if we are talking in a small tactical map (e.g: Rebel Basement) in high resolution (1024x768)[2007-05-14]
	// Determine radar coordinates
	sRadarCX	= (INT16)( gsCX * gdScaleX );
	sRadarCY	= (INT16)( gsCY * gdScaleY );

	if( guiCurrentScreen == MAP_SCREEN )
	{
		gsRadarX = RADAR_WINDOW_STRAT_X;
		gsRadarY = RADAR_WINDOW_STRAT_Y;
	}
	else if ( gsCurInterfacePanel == SM_PANEL )
	{
		gsRadarX = RADAR_WINDOW_SM_X;
		gsRadarY = RADAR_WINDOW_SM_Y;
	}
	else
	{
		gsRadarX = RADAR_WINDOW_TM_X;
		gsRadarY = RADAR_WINDOW_TM_Y;
	}


	sWidth		= ( RADAR_WINDOW_WIDTH );
	sHeight		= ( RADAR_WINDOW_HEIGHT );
	sX				= gsRadarX;
	sY				= gsRadarY;


	sRadarTLX = (INT16)( ( sTopLeftWorldX * gdScaleX ) - sRadarCX  + sX + ( sWidth /2 ) );
	sRadarTLY = (INT16)( ( sTopLeftWorldY * gdScaleY ) - sRadarCY + gsRadarY + ( sHeight /2 ) );
	sRadarBRX = (INT16)( ( sBottomRightWorldX * gdScaleX ) - sRadarCX + sX + ( sWidth /2 ) );
	sRadarBRY = (INT16)( ( sBottomRightWorldY * gdScaleY ) - sRadarCY + gsRadarY + ( sHeight /2 ) );

	// WANNE: Scrolling bugfix, if we are talking in a small tactical map (e.g: Rebel Basement) in high resolution (1024x768)[2007-05-14]
	if ((sRadarBRX - sRadarTLX) <= RADAR_WINDOW_WIDTH)
	{
		fAllowScrollingHorizontal = TRUE;
	}

	if ((sRadarBRY - sRadarTLY) <= RADAR_WINDOW_HEIGHT)
	{
		fAllowScrollingVertical = TRUE;
	}

	if ((fAllowScrollingHorizontal == FALSE || fAllowScrollingVertical == FALSE) && (gfDialogControl == TRUE))
	{
		gfDialogControl = FALSE;
		return (FALSE);
	}

	// WANNE: Scrolling bugfix for small maps in high resolution (eg: Rebel Basement)
	//if ((fAllowScrollingHorizontal || fAllowScrollingVertical) && gfDialogControl)
	//{

	// Get angles
	// TOP LEFT CORNER FIRST
	dOpp = sTopLeftWorldY - gsTLY;
	dAdj = sTopLeftWorldX - gsTLX;

	dAngle = (double)atan2( dAdj, dOpp );
	at1 = dAngle * 180 / PI;

	if ( dAngle < 0 )
	{
		fOutLeft = TRUE;
	}
	else	if ( dAngle > PI/2 )
	{
		fOutTop = TRUE;
	}

	// TOP RIGHT CORNER
	dOpp = sTopRightWorldY - gsTRY;
	dAdj = gsTRX - sTopRightWorldX;

	dAngle = (double)atan2( dAdj, dOpp );
	at2 = dAngle * 180 / PI;

	if ( dAngle < 0 )
	{
		fOutRight = TRUE;
	}
	else if ( dAngle > PI/2 )
	{
		fOutTop = TRUE;
	}


	// BOTTOM LEFT CORNER
	dOpp = gsBLY - sBottomLeftWorldY;
	dAdj = sBottomLeftWorldX - gsBLX;

	dAngle = (double)atan2( dAdj, dOpp );
	at3 = dAngle * 180 / PI;

	if ( dAngle < 0 )
	{
		fOutLeft = TRUE;
	}
	else if ( dAngle > PI/2 )
	{
		fOutBottom = TRUE;
	}

	// BOTTOM RIGHT CORNER
	dOpp = gsBRY - sBottomRightWorldY;
	dAdj = gsBRX - sBottomRightWorldX;

	dAngle = (double)atan2( dAdj, dOpp );
	at4 = dAngle * 180 / PI;

	if ( dAngle < 0 )
	{
		fOutRight = TRUE;
	}
	else if ( dAngle > PI/2 )
	{
		fOutBottom = TRUE;
	}

	sprintf( gDebugStr, "Angles: %d %d %d %d", (int)at1, (int)at2, (int)at3, (int)at4 );

	if ( !fOutRight && !fOutLeft && !fOutTop && !fOutBottom )
	{
		fScrollGood = TRUE;
	}

	// If in editor, anything goes
	if ( gfEditMode && _KeyDown( SHIFT ) )
	{
		fScrollGood = TRUE;
	}

	// Reset some UI flags
	gfUIShowExitEast								= FALSE;
	gfUIShowExitWest								= FALSE;
	gfUIShowExitNorth								= FALSE;
	gfUIShowExitSouth								= FALSE;


	if ( !fScrollGood )
	{
		// Force adjustment, if true
		if ( fForceAdjust )
		{
			if ( fOutRight )
			{
				CorrectRenderCenter( (INT16)( gsTRX - sX_S ) , sScreenCenterY , &sNewScreenX, &sNewScreenY );
				FromScreenToCellCoordinates( sNewScreenX, sNewScreenY , &sTempPosX_W, &sTempPosY_W );

				sTempRenderCenterX = sTempPosX_W;
				sTempRenderCenterY = sTempPosY_W;
				fScrollGood = TRUE;
			}
			
			if ( fOutTop )
			{
				// Adjust screen coordinates on the Y!
				CorrectRenderCenter( sScreenCenterX, (INT16)(gsTLY + sY_S ), &sNewScreenX, &sNewScreenY );
				FromScreenToCellCoordinates( sNewScreenX, sNewScreenY , &sTempPosX_W, &sTempPosY_W );

				sTempRenderCenterX = sTempPosX_W;
				sTempRenderCenterY = sTempPosY_W;

				fScrollGood = TRUE;
			}

			if ( fOutBottom )
			{
				// OK, Ajust this since we get rounding errors in our two different calculations.
				CorrectRenderCenter( sScreenCenterX, (INT16)(gsBLY - sY_S - 50 ), &sNewScreenX, &sNewScreenY );
				FromScreenToCellCoordinates( sNewScreenX, sNewScreenY , &sTempPosX_W, &sTempPosY_W );

				sTempRenderCenterX = sTempPosX_W;
				sTempRenderCenterY = sTempPosY_W;
				fScrollGood = TRUE;
			}

			if ( fOutLeft )
			{
				CorrectRenderCenter( (INT16)( gsTLX + sX_S ) , sScreenCenterY , &sNewScreenX, &sNewScreenY );
				FromScreenToCellCoordinates( sNewScreenX, sNewScreenY , &sTempPosX_W, &sTempPosY_W );

				sTempRenderCenterX = sTempPosX_W;
				sTempRenderCenterY = sTempPosY_W;

				fScrollGood = TRUE;
			}
		}
		else
		{
			if ( fOutRight )
			{
				// Check where our cursor is!
				if ( gusMouseXPos >= SCREEN_WIDTH - 1 )
				{
					gfUIShowExitEast = TRUE;
				}
			}

			if ( fOutLeft )
			{
				// Check where our cursor is!
				if ( gusMouseXPos == 0 )
				{
					gfUIShowExitWest = TRUE;
				}
			}

			if ( fOutTop )
			{
				// Check where our cursor is!
				if ( gusMouseYPos == 0 )
				{
					gfUIShowExitNorth = TRUE;
				}
			}

			if ( fOutBottom )
			{
				// Check where our cursor is!
				if ( gusMouseYPos >= SCREEN_HEIGHT - 1 )
				{
					gfUIShowExitSouth = TRUE;
				}
			}

		}
	}


	if ( fScrollGood )
	{
		if ( !fCheckOnly )
		{
				sprintf( gDebugStr, "Center: %d %d ", (int)gsRenderCenterX, (int)gsRenderCenterY );

				//Makesure it's a multiple of 5
				sMult = sTempRenderCenterX / CELL_X_SIZE;

				gsRenderCenterX = ( sMult * CELL_X_SIZE ) + ( CELL_X_SIZE / 2 );

				//Makesure it's a multiple of 5
					sMult = sTempRenderCenterY / CELL_Y_SIZE;

				gsRenderCenterY = ( sMult * CELL_Y_SIZE ) + ( CELL_Y_SIZE / 2 );

				//gsRenderCenterX = sTempRenderCenterX;
				//gsRenderCenterY = sTempRenderCenterY;


				gsTopLeftWorldX = sTopLeftWorldX - gsTLX;
					gsTopRightWorldX = sTopRightWorldX - gsTLX;
					gsBottomLeftWorldX = sBottomLeftWorldX - gsTLX;
					gsBottomRightWorldX = sBottomRightWorldX - gsTLX;

				gsTopLeftWorldY = sTopLeftWorldY - gsTLY;
				gsTopRightWorldY = sTopRightWorldY - gsTLY;
				gsBottomLeftWorldY = sBottomLeftWorldY - gsTLY;
				gsBottomRightWorldY = sBottomRightWorldY - gsTLY;

        SetPositionSndsVolumeAndPanning( );
		}

		return( TRUE );
	}

	return( FALSE );
	//}

	//return ( FALSE );
}


void ClearMarkedTiles(void)
{
	INT32 uiCount;

	for(uiCount=0; uiCount < WORLD_MAX; uiCount++)
		gpWorldLevelData[uiCount].uiFlags&=(~MAPELEMENT_REDRAW);

}

// @@ATECLIP TO WORLD!
void InvalidateWorldRedundencyRadius(INT16 sX, INT16 sY, INT16 sRadius)
{
	INT16 sCountX, sCountY;
	UINT32 uiTile;

	SetRenderFlags( RENDER_FLAG_CHECKZ );

	for(sCountY=sY-sRadius; sCountY < (sY+sRadius+2); sCountY++)
	{
		for(sCountX=sX-sRadius; sCountX < (sX+sRadius+2); sCountX++)
		{
			uiTile=FASTMAPROWCOLTOPOS(sCountY, sCountX);

			gpWorldLevelData[uiTile].uiFlags |= MAPELEMENT_REEVALUATE_REDUNDENCY;

		}
	}
}


void InvalidateWorldRedundency( )
{
	INT32 uiCount;

	SetRenderFlags( RENDER_FLAG_CHECKZ );

	for(uiCount=0; uiCount < WORLD_MAX; uiCount++)
		gpWorldLevelData[uiCount].uiFlags |= MAPELEMENT_REEVALUATE_REDUNDENCY;

}

#define	Z_STRIP_DELTA_Y  ( Z_SUBLAYERS * 10 )

/**********************************************************************************************
 Blt8BPPDataTo16BPPBufferTransZIncClip

	Blits an image into the destination buffer, using an ETRLE brush as a source, and a 16-bit
	buffer as a destination. As it is blitting, it checks the Z value of the ZBuffer, and if the
	pixel's Z level is below that of the current pixel, it is written on, and the Z value is
	updated to the current value,	for any non-transparent pixels. The Z-buffer is 16 bit, and
	must be the same dimensions (including Pitch) as the destination.

**********************************************************************************************/
BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncClip( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion)
{
	UINT16 *p16BPPPalette;
	UINT32 uiOffset;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
  ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if(clipregion==NULL)
	{
		ClipX1=ClippingRect.iLeft;
		ClipY1=ClippingRect.iTop;
		ClipX2=ClippingRect.iRight;
		ClipY2=ClippingRect.iBottom;
	}
	else
	{
		ClipX1=clipregion->iLeft;
		ClipY1=clipregion->iTop;
		ClipX2=clipregion->iRight;
		ClipY2=clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip=__min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip=__min(max(ClipX2, (iTempX+(INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip=__min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip=__min(__max(ClipY2, (iTempY+(INT32)usHeight)) - ClipY2, (INT32)usHeight);

	// calculate the remaining rows and columns to blit
	BlitLength=((INT32)usWidth-LeftSkip-RightSkip);
	BlitHeight=((INT32)usHeight-TopSkip-BottomSkip);

	// check if whole thing is clipped
	if((LeftSkip >=(INT32)usWidth) || (RightSkip >=(INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if((TopSkip >=(INT32)usHeight) || (BottomSkip >=(INT32)usHeight))
		return(TRUE);

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	p16BPPPalette = hSrcVObject->pShadeCurrent;
	LineSkip=(uiDestPitchBYTES-(BlitLength*2));

	if(hSrcVObject->ppZStripInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo=hSrcVObject->ppZStripInfo[usIndex];
	if(pZInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel=(UINT16)((INT16)usZValue+((INT16)pZInfo->bInitialZChange*Z_STRIP_DELTA_Y));
	// set to odd number of pixels for first column

	if(LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols=(LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols=20-(usZStartCols%20);
	}
	else if(LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols=(UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols=20;

	usZColsToGo=usZStartCols;

	pZArray=pZInfo->pbZChange;

	if(LeftSkip >= pZInfo->ubFirstZStripWidth)
	{
		// Index into array after doing left clipping
		usZStartIndex=1 + ((LeftSkip-pZInfo->ubFirstZStripWidth)/20);

		//calculates the Z-value after left-side clipping
		if(usZStartIndex)
		{
			for(usCount=0; usCount < usZStartIndex; usCount++)
			{
				switch(pZArray[usCount])
				{
					case -1:	usZStartLevel-=Z_STRIP_DELTA_Y;
										break;
					case 0:		//no change
										break;
					case 1:		usZStartLevel+=Z_STRIP_DELTA_Y;
										break;
				}
			}
		}
	}
	else
		usZStartIndex=0;

	usZLevel=usZStartLevel;
	usZIndex=usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


// Skips the number of lines clipped at the top
TopSkipLoop:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		TopSkipLoop
		jz		TSEndLine

		add		esi, ecx
		jmp		TopSkipLoop

TSEndLine:
		dec		TopSkip
		jnz		TopSkipLoop


// Start of line loop

// Skips the pixels hanging outside the left-side boundry
LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
		mov		eax, LeftSkip					// after we have skipped enough left-side pixels
		mov		LSCount, eax					// LSCount counts how many pixels skipped so far
		or		eax, eax
		jz		BlitLineSetup					// check for nothing to skip

LeftSkipLoop:

		mov		cl, [esi]
		inc		esi

		or		cl, cl
		js		LSTrans

		cmp		ecx, LSCount
		je		LSSkip2								// if equal, skip whole, and start blit with new run
		jb		LSSkip1								// if less, skip whole thing

		add		esi, LSCount					// skip partial run, jump into normal loop for rest
		sub		ecx, LSCount
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitNTL1							// *** jumps into non-transparent blit loop

LSSkip2:
		add		esi, ecx							// skip whole run, and start blit with new run
		jmp		BlitLineSetup


LSSkip1:
		add		esi, ecx							// skip whole run, continue skipping
		sub		LSCount, ecx
		jmp		LeftSkipLoop


LSTrans:
		and		ecx, 07fH
		cmp		ecx, LSCount
		je		BlitLineSetup					// if equal, skip whole, and start blit with new run
		jb		LSTrans1							// if less, skip whole thing

		sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitTransparent				// *** jumps into transparent blit loop


LSTrans1:
		sub		LSCount, ecx					// skip whole run, continue skipping
		jmp		LeftSkipLoop

//-------------------------------------------------
// setup for beginning of line

BlitLineSetup:
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0

BlitDispatch:

		cmp		LSCount, 0							// Check to see if we're done blitting
		je		RightSkipLoop

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		RSLoop2

//--------------------------------
// blitting non-transparent pixels

		and		ecx, 07fH

BlitNTL1:
		mov		ax, [ebx]								// check z-level of pixel
		cmp		ax, usZLevel
		jae		BlitNTL2

		mov		ax, usZLevel						// update z-level of pixel
		mov		[ebx], ax

		xor		eax, eax
		mov		al, [esi]								// copy pixel
		mov		ax, [edx+eax*2]
		mov		[edi], ax

BlitNTL2:
		inc		esi
		add		edi, 2
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitNTL6

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitNTL5								// dir = 0 no change
		js		BlitNTL4								// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitNTL5

BlitNTL4:
		sub		dx, Z_STRIP_DELTA_Y

BlitNTL5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitNTL6:
		dec		LSCount									// decrement pixel length to blit
		jz		RightSkipLoop						// done blitting the visible line

		dec		ecx
		jnz		BlitNTL1								// continue current run

		jmp		BlitDispatch						// done current run, go for another


//----------------------------
// skipping transparent pixels

BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

BlitTrans2:

		add		edi, 2									// move up the destination pointer
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitTrans1

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitTrans5							// dir = 0 no change
		js		BlitTrans4							// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitTrans5

BlitTrans4:
		sub		dx, Z_STRIP_DELTA_Y

BlitTrans5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitTrans1:

		dec		LSCount									// decrement the pixels to blit
		jz		RightSkipLoop						// done the line

		dec		ecx
		jnz		BlitTrans2

		jmp		BlitDispatch

//---------------------------------------------
// Scans the ETRLE until it finds an EOL marker

RightSkipLoop:


RSLoop1:
		mov		al, [esi]
		inc		esi
		or		al, al
		jnz		RSLoop1

RSLoop2:

		dec		BlitHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip

// reset all the z-level stuff for a new line

		mov		ax, usZStartLevel
		mov		usZLevel, ax
		mov		ax, usZStartIndex
		mov		usZIndex, ax
		mov		ax, usZStartCols
		mov		usZColsToGo, ax


		jmp		LeftSkipSetup


BlitDone:
	}

	return(TRUE);
}


/**********************************************************************************************
 Blt8BPPDataTo16BPPBufferTransZIncClipSaveZBurnsThrough

	Blits an image into the destination buffer, using an ETRLE brush as a source, and a 16-bit
	buffer as a destination. As it is blitting, it checks the Z value of the ZBuffer, and if the
	pixel's Z level is below that of the current pixel, it is written on, and the Z value is
	updated to the current value,	for any non-transparent pixels. The Z-buffer is 16 bit, and
	must be the same dimensions (including Pitch) as the destination.

**********************************************************************************************/
BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncClipZSameZBurnsThrough( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 usZStripIndex )
{
	UINT16 *p16BPPPalette;
	UINT32 uiOffset;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
  ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if(clipregion==NULL)
	{
		ClipX1=ClippingRect.iLeft;
		ClipY1=ClippingRect.iTop;
		ClipX2=ClippingRect.iRight;
		ClipY2=ClippingRect.iBottom;
	}
	else
	{
		ClipX1=clipregion->iLeft;
		ClipY1=clipregion->iTop;
		ClipX2=clipregion->iRight;
		ClipY2=clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip=__min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip=__min(max(ClipX2, (iTempX+(INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip=__min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip=__min(__max(ClipY2, (iTempY+(INT32)usHeight)) - ClipY2, (INT32)usHeight);

	// calculate the remaining rows and columns to blit
	BlitLength=((INT32)usWidth-LeftSkip-RightSkip);
	BlitHeight=((INT32)usHeight-TopSkip-BottomSkip);

	// check if whole thing is clipped
	if((LeftSkip >=(INT32)usWidth) || (RightSkip >=(INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if((TopSkip >=(INT32)usHeight) || (BottomSkip >=(INT32)usHeight))
		return(TRUE);

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	p16BPPPalette = hSrcVObject->pShadeCurrent;
	LineSkip=(uiDestPitchBYTES-(BlitLength*2));

	if(hSrcVObject->ppZStripInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo=hSrcVObject->ppZStripInfo[ usZStripIndex ];
	if(pZInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel=(UINT16)((INT16)usZValue+((INT16)pZInfo->bInitialZChange*Z_STRIP_DELTA_Y));
	// set to odd number of pixels for first column

	if(LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols=(LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols=20-(usZStartCols%20);
	}
	else if(LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols=(UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols=20;

	usZColsToGo=usZStartCols;

	pZArray=pZInfo->pbZChange;

	if(LeftSkip >= pZInfo->ubFirstZStripWidth)
	{
		// Index into array after doing left clipping
		usZStartIndex=1 + ((LeftSkip-pZInfo->ubFirstZStripWidth)/20);

		//calculates the Z-value after left-side clipping
		if(usZStartIndex)
		{
			for(usCount=0; usCount < usZStartIndex; usCount++)
			{
				switch(pZArray[usCount])
				{
					case -1:	usZStartLevel-=Z_STRIP_DELTA_Y;
										break;
					case 0:		//no change
										break;
					case 1:		usZStartLevel+=Z_STRIP_DELTA_Y;
										break;
				}
			}
		}
	}
	else
		usZStartIndex=0;

	usZLevel=usZStartLevel;
	usZIndex=usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


// Skips the number of lines clipped at the top
TopSkipLoop:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		TopSkipLoop
		jz		TSEndLine

		add		esi, ecx
		jmp		TopSkipLoop

TSEndLine:
		dec		TopSkip
		jnz		TopSkipLoop


// Start of line loop

// Skips the pixels hanging outside the left-side boundry
LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
		mov		eax, LeftSkip					// after we have skipped enough left-side pixels
		mov		LSCount, eax					// LSCount counts how many pixels skipped so far
		or		eax, eax
		jz		BlitLineSetup					// check for nothing to skip

LeftSkipLoop:

		mov		cl, [esi]
		inc		esi

		or		cl, cl
		js		LSTrans

		cmp		ecx, LSCount
		je		LSSkip2								// if equal, skip whole, and start blit with new run
		jb		LSSkip1								// if less, skip whole thing

		add		esi, LSCount					// skip partial run, jump into normal loop for rest
		sub		ecx, LSCount
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitNTL1							// *** jumps into non-transparent blit loop

LSSkip2:
		add		esi, ecx							// skip whole run, and start blit with new run
		jmp		BlitLineSetup


LSSkip1:
		add		esi, ecx							// skip whole run, continue skipping
		sub		LSCount, ecx
		jmp		LeftSkipLoop


LSTrans:
		and		ecx, 07fH
		cmp		ecx, LSCount
		je		BlitLineSetup					// if equal, skip whole, and start blit with new run
		jb		LSTrans1							// if less, skip whole thing

		sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitTransparent				// *** jumps into transparent blit loop


LSTrans1:
		sub		LSCount, ecx					// skip whole run, continue skipping
		jmp		LeftSkipLoop

//-------------------------------------------------
// setup for beginning of line

BlitLineSetup:
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0

BlitDispatch:

		cmp		LSCount, 0							// Check to see if we're done blitting
		je		RightSkipLoop

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		RSLoop2

//--------------------------------
// blitting non-transparent pixels

		and		ecx, 07fH

BlitNTL1:
		mov		ax, [ebx]								// check z-level of pixel
		cmp		ax, usZLevel
		ja		BlitNTL2

		mov		ax, usZLevel						// update z-level of pixel
		mov		[ebx], ax

		xor		eax, eax
		mov		al, [esi]								// copy pixel
		mov		ax, [edx+eax*2]
		mov		[edi], ax

BlitNTL2:
		inc		esi
		add		edi, 2
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitNTL6

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitNTL5								// dir = 0 no change
		js		BlitNTL4								// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitNTL5

BlitNTL4:
		sub		dx, Z_STRIP_DELTA_Y

BlitNTL5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitNTL6:
		dec		LSCount									// decrement pixel length to blit
		jz		RightSkipLoop						// done blitting the visible line

		dec		ecx
		jnz		BlitNTL1								// continue current run

		jmp		BlitDispatch						// done current run, go for another


//----------------------------
// skipping transparent pixels

BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

BlitTrans2:

		add		edi, 2									// move up the destination pointer
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitTrans1

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitTrans5							// dir = 0 no change
		js		BlitTrans4							// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitTrans5

BlitTrans4:
		sub		dx, Z_STRIP_DELTA_Y

BlitTrans5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitTrans1:

		dec		LSCount									// decrement the pixels to blit
		jz		RightSkipLoop						// done the line

		dec		ecx
		jnz		BlitTrans2

		jmp		BlitDispatch

//---------------------------------------------
// Scans the ETRLE until it finds an EOL marker

RightSkipLoop:


RSLoop1:
		mov		al, [esi]
		inc		esi
		or		al, al
		jnz		RSLoop1

RSLoop2:

		dec		BlitHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip

// reset all the z-level stuff for a new line

		mov		ax, usZStartLevel
		mov		usZLevel, ax
		mov		ax, usZStartIndex
		mov		usZIndex, ax
		mov		ax, usZStartCols
		mov		usZColsToGo, ax


		jmp		LeftSkipSetup


BlitDone:
	}

	return(TRUE);
}


/**********************************************************************************************
 Blt8BPPDataTo16BPPBufferTransZIncObscureClip

	Blits an image into the destination buffer, using an ETRLE brush as a source, and a 16-bit
	buffer as a destination. As it is blitting, it checks the Z value of the ZBuffer, and if the
	pixel's Z level is below that of the current pixel, it is written on, and the Z value is
	updated to the current value,	for any non-transparent pixels. The Z-buffer is 16 bit, and
	must be the same dimensions (including Pitch) as the destination.

	//ATE: This blitter makes the values that are =< z value pixellate rather than not
	// render at all

**********************************************************************************************/
BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncObscureClip( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion)
{
	UINT16 *p16BPPPalette;
	UINT32 uiOffset, uiLineFlag;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
  ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;


	if(clipregion==NULL)
	{
		ClipX1=ClippingRect.iLeft;
		ClipY1=ClippingRect.iTop;
		ClipX2=ClippingRect.iRight;
		ClipY2=ClippingRect.iBottom;
	}
	else
	{
		ClipX1=clipregion->iLeft;
		ClipY1=clipregion->iTop;
		ClipX2=clipregion->iRight;
		ClipY2=clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip=__min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip=__min(max(ClipX2, (iTempX+(INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip=__min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip=__min(__max(ClipY2, (iTempY+(INT32)usHeight)) - ClipY2, (INT32)usHeight);

	uiLineFlag=(iTempY&1);

	// calculate the remaining rows and columns to blit
	BlitLength=((INT32)usWidth-LeftSkip-RightSkip);
	BlitHeight=((INT32)usHeight-TopSkip-BottomSkip);

	// check if whole thing is clipped
	if((LeftSkip >=(INT32)usWidth) || (RightSkip >=(INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if((TopSkip >=(INT32)usHeight) || (BottomSkip >=(INT32)usHeight))
		return(TRUE);

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	p16BPPPalette = hSrcVObject->pShadeCurrent;
	LineSkip=(uiDestPitchBYTES-(BlitLength*2));

	if(hSrcVObject->ppZStripInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo=hSrcVObject->ppZStripInfo[usIndex];
	if(pZInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel=(UINT16)((INT16)usZValue+((INT16)pZInfo->bInitialZChange*Z_STRIP_DELTA_Y));
	// set to odd number of pixels for first column

	if(LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols=(LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols=20-(usZStartCols%20);
	}
	else if(LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols=(UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols=20;

	usZColsToGo=usZStartCols;

	pZArray=pZInfo->pbZChange;

	if(LeftSkip >= pZInfo->ubFirstZStripWidth)
	{
		// Index into array after doing left clipping
		usZStartIndex=1 + ((LeftSkip-pZInfo->ubFirstZStripWidth)/20);

		//calculates the Z-value after left-side clipping
		if(usZStartIndex)
		{
			for(usCount=0; usCount < usZStartIndex; usCount++)
			{
				switch(pZArray[usCount])
				{
					case -1:	usZStartLevel-=Z_STRIP_DELTA_Y;
										break;
					case 0:		//no change
										break;
					case 1:		usZStartLevel+=Z_STRIP_DELTA_Y;
										break;
				}
			}
		}
	}
	else
		usZStartIndex=0;

	usZLevel=usZStartLevel;
	usZIndex=usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


// Skips the number of lines clipped at the top
TopSkipLoop:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		TopSkipLoop
		jz		TSEndLine

		add		esi, ecx
		jmp		TopSkipLoop

TSEndLine:

		xor		uiLineFlag, 1
		dec		TopSkip
		jnz		TopSkipLoop


// Start of line loop

// Skips the pixels hanging outside the left-side boundry
LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
		mov		eax, LeftSkip					// after we have skipped enough left-side pixels
		mov		LSCount, eax					// LSCount counts how many pixels skipped so far
		or		eax, eax
		jz		BlitLineSetup					// check for nothing to skip

LeftSkipLoop:

		mov		cl, [esi]
		inc		esi

		or		cl, cl
		js		LSTrans

		cmp		ecx, LSCount
		je		LSSkip2								// if equal, skip whole, and start blit with new run
		jb		LSSkip1								// if less, skip whole thing

		add		esi, LSCount					// skip partial run, jump into normal loop for rest
		sub		ecx, LSCount
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitNTL1							// *** jumps into non-transparent blit loop

LSSkip2:
		add		esi, ecx							// skip whole run, and start blit with new run
		jmp		BlitLineSetup


LSSkip1:
		add		esi, ecx							// skip whole run, continue skipping
		sub		LSCount, ecx
		jmp		LeftSkipLoop


LSTrans:
		and		ecx, 07fH
		cmp		ecx, LSCount
		je		BlitLineSetup					// if equal, skip whole, and start blit with new run
		jb		LSTrans1							// if less, skip whole thing

		sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitTransparent				// *** jumps into transparent blit loop


LSTrans1:
		sub		LSCount, ecx					// skip whole run, continue skipping
		jmp		LeftSkipLoop

//-------------------------------------------------
// setup for beginning of line

BlitLineSetup:
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0

BlitDispatch:

		cmp		LSCount, 0							// Check to see if we're done blitting
		je		RightSkipLoop

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		RSLoop2

//--------------------------------
// blitting non-transparent pixels

		and		ecx, 07fH

BlitNTL1:
		mov		ax, [ebx]								// check z-level of pixel
		cmp		ax, usZLevel
		jae		BlitPixellate1
		jmp   BlitPixel1

BlitPixellate1:

		// OK, DO PIXELLATE SCHEME HERE!
		test	uiLineFlag, 1
		jz		BlitSkip1

		test	edi, 2
		jz		BlitNTL2
		jmp		BlitPixel1

BlitSkip1:
		test	edi, 2
		jnz		BlitNTL2

BlitPixel1:

		mov		ax, usZLevel						// update z-level of pixel
		mov		[ebx], ax

		xor		eax, eax
		mov		al, [esi]								// copy pixel
		mov		ax, [edx+eax*2]
		mov		[edi], ax

BlitNTL2:
		inc		esi
		add		edi, 2
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitNTL6

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitNTL5								// dir = 0 no change
		js		BlitNTL4								// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitNTL5

BlitNTL4:
		sub		dx, Z_STRIP_DELTA_Y

BlitNTL5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitNTL6:
		dec		LSCount									// decrement pixel length to blit
		jz		RightSkipLoop						// done blitting the visible line

		dec		ecx
		jnz		BlitNTL1								// continue current run

		jmp		BlitDispatch						// done current run, go for another


//----------------------------
// skipping transparent pixels

BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

BlitTrans2:

		add		edi, 2									// move up the destination pointer
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitTrans1

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitTrans5							// dir = 0 no change
		js		BlitTrans4							// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_STRIP_DELTA_Y
		jmp		BlitTrans5

BlitTrans4:
		sub		dx, Z_STRIP_DELTA_Y

BlitTrans5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitTrans1:

		dec		LSCount									// decrement the pixels to blit
		jz		RightSkipLoop						// done the line

		dec		ecx
		jnz		BlitTrans2

		jmp		BlitDispatch

//---------------------------------------------
// Scans the ETRLE until it finds an EOL marker

RightSkipLoop:


RSLoop1:
		mov		al, [esi]
		inc		esi
		or		al, al
		jnz		RSLoop1

RSLoop2:

		xor		uiLineFlag, 1
		dec		BlitHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip

// reset all the z-level stuff for a new line

		mov		ax, usZStartLevel
		mov		usZLevel, ax
		mov		ax, usZStartIndex
		mov		usZIndex, ax
		mov		ax, usZStartCols
		mov		usZColsToGo, ax


		jmp		LeftSkipSetup


BlitDone:
	}

	return(TRUE);
}


// Blitter Specs
// 1 ) 8 to 16 bpp
// 2 ) strip z-blitter
// 3 ) clipped
// 4 ) trans shadow - if value is 254, makes a shadow
//
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClip(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows) 
{
	UINT32 uiOffset, uiLineFlag;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
  ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if(clipregion==NULL)
	{
		ClipX1=ClippingRect.iLeft;
		ClipY1=ClippingRect.iTop;
		ClipX2=ClippingRect.iRight;
		ClipY2=ClippingRect.iBottom;
	}
	else
	{
		ClipX1=clipregion->iLeft;
		ClipY1=clipregion->iTop;
		ClipX2=clipregion->iRight;
		ClipY2=clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip=__min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip=__min(max(ClipX2, (iTempX+(INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip=__min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip=__min(__max(ClipY2, (iTempY+(INT32)usHeight)) - ClipY2, (INT32)usHeight);

	uiLineFlag=(iTempY&1);

	// calculate the remaining rows and columns to blit
	BlitLength=((INT32)usWidth-LeftSkip-RightSkip);
	BlitHeight=((INT32)usHeight-TopSkip-BottomSkip);

	// check if whole thing is clipped
	if((LeftSkip >=(INT32)usWidth) || (RightSkip >=(INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if((TopSkip >=(INT32)usHeight) || (BottomSkip >=(INT32)usHeight))
		return(TRUE);

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	LineSkip=(uiDestPitchBYTES-(BlitLength*2));

	if(hSrcVObject->ppZStripInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo=hSrcVObject->ppZStripInfo[sZIndex];
	if(pZInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel=(UINT16)((INT16)usZValue+((INT16)pZInfo->bInitialZChange*Z_SUBLAYERS*10));

	if(LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols=(LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols=20-(usZStartCols%20);
	}
	else if(LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols=(UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols=20;

	// set to odd number of pixels for first column
	usZColsToGo=usZStartCols;

	pZArray=pZInfo->pbZChange;

	if(LeftSkip >= usZColsToGo)
	{
		// Index into array after doing left clipping
		usZStartIndex=1 + ((LeftSkip-pZInfo->ubFirstZStripWidth)/20);

		//calculates the Z-value after left-side clipping
		if(usZStartIndex)
		{
			for(usCount=0; usCount < usZStartIndex; usCount++)
			{
				switch(pZArray[usCount])
				{
					case -1:	usZStartLevel-=Z_SUBLAYERS;
										break;
					case 0:		//no change
										break;
					case 1:		usZStartLevel+=Z_SUBLAYERS;
										break;
				}
			}
		}
	}
	else
		usZStartIndex=0;

	usZLevel=usZStartLevel;
	usZIndex=usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


// Skips the number of lines clipped at the top
TopSkipLoop:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		TopSkipLoop
		jz		TSEndLine

		add		esi, ecx
		jmp		TopSkipLoop

TSEndLine:

		xor		uiLineFlag, 1
		dec		TopSkip
		jnz		TopSkipLoop


// Start of line loop

// Skips the pixels hanging outside the left-side boundry
LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
		mov		eax, LeftSkip					// after we have skipped enough left-side pixels
		mov		LSCount, eax					// LSCount counts how many pixels skipped so far
		or		eax, eax
		jz		BlitLineSetup					// check for nothing to skip

LeftSkipLoop:

		mov		cl, [esi]
		inc		esi

		or		cl, cl
		js		LSTrans

		cmp		ecx, LSCount
		je		LSSkip2								// if equal, skip whole, and start blit with new run
		jb		LSSkip1								// if less, skip whole thing

		add		esi, LSCount					// skip partial run, jump into normal loop for rest
		sub		ecx, LSCount
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitNTL1							// *** jumps into non-transparent blit loop

LSSkip2:
		add		esi, ecx							// skip whole run, and start blit with new run
		jmp		BlitLineSetup


LSSkip1:
		add		esi, ecx							// skip whole run, continue skipping
		sub		LSCount, ecx
		jmp		LeftSkipLoop


LSTrans:
		and		ecx, 07fH
		cmp		ecx, LSCount
		je		BlitLineSetup					// if equal, skip whole, and start blit with new run
		jb		LSTrans1							// if less, skip whole thing

		sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
		mov		eax, BlitLength
		mov		LSCount, eax

		mov		Unblitted, 0
		jmp		BlitTransparent				// *** jumps into transparent blit loop


LSTrans1:
		sub		LSCount, ecx					// skip whole run, continue skipping
		jmp		LeftSkipLoop

//-------------------------------------------------
// setup for beginning of line

BlitLineSetup:
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0

BlitDispatch:

		cmp		LSCount, 0							// Check to see if we're done blitting
		je		RightSkipLoop

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		RSLoop2

//--------------------------------
// blitting non-transparent pixels

		and		ecx, 07fH

BlitNTL1:
		mov		ax, [ebx]								// check z-level of pixel
		cmp		ax, usZLevel
		//jae		BlitPixellate1 // Original comparison
		ja		BlitPixellate1 // Due to lobot layers having the same z-level, the comparison would pixelate merc's layers against each other. Now we pixelate ONLY if the comparison is above.
		jmp		BlitPixel1

BlitPixellate1:

		// OK, DO PIXELLATE SCHEME HERE!
		test	uiLineFlag, 1
		jz		BlitSkip1

		test	edi, 2
		jz		BlitNTL2
		jmp		BlitPixel1

BlitSkip1:
		test	edi, 2
		jnz		BlitNTL2

BlitPixel1:

		mov		ax, usZLevel						// update z-level of pixel
		mov		[ebx], ax

		// Check for shadow...
		xor		eax, eax
		mov		al, [esi]
		cmp		al, 254
		jne		BlitNTL66

		mov		al, fIgnoreShadows
		cmp		al, 0
		jne		BlitNTL2

		mov		ax, [edi]
		mov		ax, ShadeTable[eax*2]
		mov		[edi], ax
		jmp		BlitNTL2

BlitNTL66:

		mov		ax, [edx+eax*2]					// Copy pixel
		mov		[edi], ax

BlitNTL2:
		inc		esi
		add		edi, 2
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitNTL6

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitNTL5								// dir = 0 no change
		js		BlitNTL4								// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_SUBLAYERS
		jmp		BlitNTL5

BlitNTL4:
		sub		dx, Z_SUBLAYERS

BlitNTL5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitNTL6:
		dec		LSCount									// decrement pixel length to blit
		jz		RightSkipLoop						// done blitting the visible line

		dec		ecx
		jnz		BlitNTL1								// continue current run

		jmp		BlitDispatch						// done current run, go for another


//----------------------------
// skipping transparent pixels

BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

BlitTrans2:

		add		edi, 2									// move up the destination pointer
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitTrans1

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitTrans5							// dir = 0 no change
		js		BlitTrans4							// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_SUBLAYERS
		jmp		BlitTrans5

BlitTrans4:
		sub		dx, Z_SUBLAYERS

BlitTrans5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitTrans1:

		dec		LSCount									// decrement the pixels to blit
		jz		RightSkipLoop						// done the line

		dec		ecx
		jnz		BlitTrans2

		jmp		BlitDispatch

//---------------------------------------------
// Scans the ETRLE until it finds an EOL marker

RightSkipLoop:


RSLoop1:
		mov		al, [esi]
		inc		esi
		or		al, al
		jnz		RSLoop1

RSLoop2:

		xor		uiLineFlag, 1
		dec		BlitHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip

// reset all the z-level stuff for a new line

		mov		ax, usZStartLevel
		mov		usZLevel, ax
		mov		ax, usZStartIndex
		mov		usZIndex, ax
		mov		ax, usZStartCols
		mov		usZColsToGo, ax


		jmp		LeftSkipSetup


BlitDone:
	}

	return(TRUE);
}


// Blitter Specs
// 1 ) 8 to 16 bpp
// 2 ) strip z-blitter
// 3 ) clipped
// 4 ) trans shadow - if value is 254, makes a shadow
//
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClipAlpha(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, HVOBJECT hAlphaVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows)
{
	UINT32 uiOffset, uiLineFlag;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr, *AlphaPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert(hSrcVObject != NULL);
	Assert(hAlphaVObject != NULL);
	Assert(pBuffer != NULL);

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[usIndex]);
	usHeight = (UINT32)pTrav->usHeight;
	usWidth = (UINT32)pTrav->usWidth;
	uiOffset = pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if (clipregion == NULL)
	{
		ClipX1 = ClippingRect.iLeft;
		ClipY1 = ClippingRect.iTop;
		ClipX2 = ClippingRect.iRight;
		ClipY2 = ClippingRect.iBottom;
	}
	else
	{
		ClipX1 = clipregion->iLeft;
		ClipY1 = clipregion->iTop;
		ClipX2 = clipregion->iRight;
		ClipY2 = clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip = __min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip = __min(max(ClipX2, (iTempX + (INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip = __min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip = __min(__max(ClipY2, (iTempY + (INT32)usHeight)) - ClipY2, (INT32)usHeight);

	uiLineFlag = (iTempY & 1);

	// calculate the remaining rows and columns to blit
	BlitLength = ((INT32)usWidth - LeftSkip - RightSkip);
	BlitHeight = ((INT32)usHeight - TopSkip - BottomSkip);

	// check if whole thing is clipped
	if ((LeftSkip >= (INT32)usWidth) || (RightSkip >= (INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if ((TopSkip >= (INT32)usHeight) || (BottomSkip >= (INT32)usHeight))
		return(TRUE);

	SrcPtr = (UINT8 *)hSrcVObject->pPixData + uiOffset;
	AlphaPtr = (UINT8 *)hAlphaVObject->pPixData + (hAlphaVObject->pETRLEObject[usIndex]).uiDataOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY + TopSkip)) + ((iTempX + LeftSkip) * 2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY + TopSkip)) + ((iTempX + LeftSkip) * 2);
	LineSkip = (uiDestPitchBYTES - (BlitLength * 2));

	if (hSrcVObject->ppZStripInfo == NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo = hSrcVObject->ppZStripInfo[sZIndex];
	if (pZInfo == NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel = (UINT16)((INT16)usZValue + ((INT16)pZInfo->bInitialZChange*Z_SUBLAYERS * 10));

	if (LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols = (LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols = 20 - (usZStartCols % 20);
	}
	else if (LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols = (UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols = 20;

	// set to odd number of pixels for first column
	usZColsToGo = usZStartCols;

	pZArray = pZInfo->pbZChange;

	if (LeftSkip >= usZColsToGo)
	{
		// Index into array after doing left clipping
		usZStartIndex = 1 + ((LeftSkip - pZInfo->ubFirstZStripWidth) / 20);

		//calculates the Z-value after left-side clipping
		if (usZStartIndex)
		{
			for (usCount = 0; usCount < usZStartIndex; usCount++)
			{
				switch (pZArray[usCount])
				{
				case -1:	usZStartLevel -= Z_SUBLAYERS;
					break;
				case 0:		//no change
					break;
				case 1:		usZStartLevel += Z_SUBLAYERS;
					break;
				}
			}
		}
	}
	else
		usZStartIndex = 0;

	usZLevel = usZStartLevel;
	usZIndex = usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


		// Skips the number of lines clipped at the top
		TopSkipLoop :

		mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		TopSkipLoop
			jz		TSEndLine

			add		esi, ecx

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			jmp		TopSkipLoop

			TSEndLine :

		xor		uiLineFlag, 1
			dec		TopSkip
			jnz		TopSkipLoop


			// Start of line loop

			// Skips the pixels hanging outside the left-side boundry
		LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
			mov		eax, LeftSkip					// after we have skipped enough left-side pixels
			mov		LSCount, eax					// LSCount counts how many pixels skipped so far
			or eax, eax
			jz		BlitLineSetup					// check for nothing to skip

			LeftSkipLoop :

		mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		LSTrans

			cmp		ecx, LSCount
			je		LSSkip2								// if equal, skip whole, and start blit with new run
			jb		LSSkip1								// if less, skip whole thing

			add		esi, LSCount					// skip partial run, jump into normal loop for rest

			push	esi
			mov		esi, AlphaPtr
			add		esi, LSCount
			mov		AlphaPtr, esi
			pop		esi

			sub		ecx, LSCount
			mov		eax, BlitLength
			mov		LSCount, eax
			mov		Unblitted, 0
			jmp		BlitNTL1							// *** jumps into non-transparent blit loop

			LSSkip2 :
		add		esi, ecx							// skip whole run, and start blit with new run

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			jmp		BlitLineSetup


			LSSkip1 :
		add		esi, ecx							// skip whole run, continue skipping

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			sub		LSCount, ecx
			jmp		LeftSkipLoop


			LSTrans :
		and		ecx, 07fH
			cmp		ecx, LSCount
			je		BlitLineSetup					// if equal, skip whole, and start blit with new run
			jb		LSTrans1							// if less, skip whole thing

			sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
			mov		eax, BlitLength
			mov		LSCount, eax

			mov		Unblitted, 0
			jmp		BlitTransparent				// *** jumps into transparent blit loop


			LSTrans1 :
		sub		LSCount, ecx					// skip whole run, continue skipping
			jmp		LeftSkipLoop

			//-------------------------------------------------
			// setup for beginning of line

			BlitLineSetup :
		mov		eax, BlitLength
			mov		LSCount, eax
			mov		Unblitted, 0

			BlitDispatch :

			cmp		LSCount, 0							// Check to see if we're done blitting
			je		RightSkipLoop

			mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		BlitTransparent
			jz		RSLoop2

			//--------------------------------
			// blitting non-transparent pixels

			and		ecx, 07fH

			BlitNTL1 :
		mov		ax, [ebx]								// check z-level of pixel
			cmp		ax, usZLevel
			jae		BlitPixellate1
			jmp		BlitPixel1

			BlitPixellate1 :

		// OK, DO PIXELLATE SCHEME HERE!
		test	uiLineFlag, 1
			jz		BlitSkip1

			test	edi, 2
			jz		BlitNTL2
			jmp		BlitPixel1

			BlitSkip1 :
		test	edi, 2
			jnz		BlitNTL2

			BlitPixel1 :

		mov		ax, usZLevel						// update z-level of pixel
			mov[ebx], ax

			// Check for shadow...
			xor		eax, eax
			mov		al, [esi]
			cmp		al, 254
			jne		BlitNTL66

			mov		al, fIgnoreShadows
			cmp		al, 0
			jne		BlitNTL2

			mov		ax, [edi]
			mov		ax, ShadeTable[eax * 2]
			mov[edi], ax
			jmp		BlitNTL2

			BlitNTL66 :

		mov		ax, [edx + eax * 2]					// Copy pixel

			push	edx
			push	ecx
			push	ebx
			push	esi
			mov		esi, AlphaPtr
			xor		ebx, ebx
			mov		bl, [esi]
			pop		esi
			push	ebx
			push[edi]
			push	eax
			call    blendWithAlpha
			add     esp, 12
			pop		ebx
			pop		ecx
			pop		edx

			mov[edi], ax

			BlitNTL2 :
		inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			add		edi, 2
			add		ebx, 2

			dec		usZColsToGo
			jnz		BlitNTL6

			// update the z-level according to the z-table

			push	edx
			mov		edx, pZArray						// get pointer to array
			xor		eax, eax
			mov		ax, usZIndex						// pick up the current array index
			add		edx, eax
			inc		eax											// increment it
			mov		usZIndex, ax						// store incremented value

			mov		al, [edx]								// get direction instruction
			mov		dx, usZLevel						// get current z-level

			or al, al
			jz		BlitNTL5								// dir = 0 no change
			js		BlitNTL4								// dir < 0 z-level down
																		// dir > 0 z-level up (default)
			add		dx, Z_SUBLAYERS
			jmp		BlitNTL5

			BlitNTL4 :
		sub		dx, Z_SUBLAYERS

			BlitNTL5 :
		mov		usZLevel, dx						// store the now-modified z-level
			mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
			pop		edx

			BlitNTL6 :
		dec		LSCount									// decrement pixel length to blit
			jz		RightSkipLoop						// done blitting the visible line

			dec		ecx
			jnz		BlitNTL1								// continue current run

			jmp		BlitDispatch						// done current run, go for another


	//----------------------------
	// skipping transparent pixels

		BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

			BlitTrans2 :

		add		edi, 2									// move up the destination pointer
			add		ebx, 2

			dec		usZColsToGo
			jnz		BlitTrans1

			// update the z-level according to the z-table

			push	edx
			mov		edx, pZArray						// get pointer to array
			xor		eax, eax
			mov		ax, usZIndex						// pick up the current array index
			add		edx, eax
			inc		eax											// increment it
			mov		usZIndex, ax						// store incremented value

			mov		al, [edx]								// get direction instruction
			mov		dx, usZLevel						// get current z-level

			or al, al
			jz		BlitTrans5							// dir = 0 no change
			js		BlitTrans4							// dir < 0 z-level down
																		// dir > 0 z-level up (default)
			add		dx, Z_SUBLAYERS
			jmp		BlitTrans5

			BlitTrans4 :
		sub		dx, Z_SUBLAYERS

			BlitTrans5 :
		mov		usZLevel, dx						// store the now-modified z-level
			mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
			pop		edx

			BlitTrans1 :

		dec		LSCount									// decrement the pixels to blit
			jz		RightSkipLoop						// done the line

			dec		ecx
			jnz		BlitTrans2

			jmp		BlitDispatch

			//---------------------------------------------
			// Scans the ETRLE until it finds an EOL marker

			RightSkipLoop :


	RSLoop1:
		mov		al, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or al, al
			jnz		RSLoop1

			RSLoop2 :

		xor		uiLineFlag, 1
			dec		BlitHeight
			jz		BlitDone
			add		edi, LineSkip
			add		ebx, LineSkip

			// reset all the z-level stuff for a new line

			mov		ax, usZStartLevel
			mov		usZLevel, ax
			mov		ax, usZStartIndex
			mov		usZIndex, ax
			mov		ax, usZStartCols
			mov		usZColsToGo, ax


			jmp		LeftSkipSetup


			BlitDone :
	}

	return(TRUE);
}



void CorrectRenderCenter( INT16 sRenderX, INT16 sRenderY, INT16 *pSNewX, INT16 *pSNewY )
{
	INT16 sScreenX, sScreenY;
	INT16 sNumXSteps, sNumYSteps;

	// Use radar scale values to get screen values, then convert ot map values, rounding to nearest middle tile
	sScreenX = (INT16) sRenderX;
	sScreenY = (INT16) sRenderY;

	// Adjust for offsets!
	sScreenX += 0;
	sScreenY += 10;

	// Adjust to viewport start!
	sScreenX -= ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 );
	sScreenY -= ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 );

	//Make sure these coordinates are multiples of scroll steps
	sNumXSteps = sScreenX  / gubNewScrollXSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ];
	sNumYSteps = sScreenY  / gubNewScrollYSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ];


	sScreenX = ( sNumXSteps * gubNewScrollXSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ] );
	sScreenY = ( sNumYSteps * gubNewScrollYSpeeds[ gfDoVideoScroll ][ gubCurScrollSpeedID ]);

	// Adjust back
	sScreenX += ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 );
	sScreenY += ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 );

	*pSNewX = sScreenX;
	*pSNewY = sScreenY;

}



// Blitter Specs
// 1 ) 8 to 16 bpp
// 2 ) strip z-blitter
// 3 ) clipped
// 4 ) trans shadow - if value is 254, makes a shadow
//
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncClip(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows) 
{
	UINT32 uiOffset;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
  ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if(clipregion==NULL)
	{
		ClipX1=ClippingRect.iLeft;
		ClipY1=ClippingRect.iTop;
		ClipX2=ClippingRect.iRight;
		ClipY2=ClippingRect.iBottom;
	}
	else
	{
		ClipX1=clipregion->iLeft;
		ClipY1=clipregion->iTop;
		ClipX2=clipregion->iRight;
		ClipY2=clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip=__min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip=__min(max(ClipX2, (iTempX+(INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip=__min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip=__min(__max(ClipY2, (iTempY+(INT32)usHeight)) - ClipY2, (INT32)usHeight);

	// calculate the remaining rows and columns to blit
	BlitLength=((INT32)usWidth-LeftSkip-RightSkip);
	BlitHeight=((INT32)usHeight-TopSkip-BottomSkip);

	// check if whole thing is clipped
	if((LeftSkip >=(INT32)usWidth) || (RightSkip >=(INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if((TopSkip >=(INT32)usHeight) || (BottomSkip >=(INT32)usHeight))
		return(TRUE);

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY+TopSkip)) + ((iTempX+LeftSkip)*2);
	LineSkip=(uiDestPitchBYTES-(BlitLength*2));

	if(hSrcVObject->ppZStripInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo=hSrcVObject->ppZStripInfo[sZIndex];
	if(pZInfo==NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel=(UINT16)((INT16)usZValue+((INT16)pZInfo->bInitialZChange*Z_SUBLAYERS*10));

	if(LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols=(LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols=20-(usZStartCols%20);
	}
	else if(LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols=(UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols=20;

	// set to odd number of pixels for first column
	usZColsToGo=usZStartCols;

	pZArray=pZInfo->pbZChange;

	if(LeftSkip >= usZColsToGo)
	{
		// Index into array after doing left clipping
		usZStartIndex=1 + ((LeftSkip-pZInfo->ubFirstZStripWidth)/20);

		//calculates the Z-value after left-side clipping
		if(usZStartIndex)
		{
			for(usCount=0; usCount < usZStartIndex; usCount++)
			{
				switch(pZArray[usCount])
				{
					case -1:	usZStartLevel-=Z_SUBLAYERS;
										break;
					case 0:		//no change
										break;
					case 1:		usZStartLevel+=Z_SUBLAYERS;
										break;
				}
			}
		}
	}
	else
		usZStartIndex=0;

	usZLevel=usZStartLevel;
	usZIndex=usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


// Skips the number of lines clipped at the top
TopSkipLoop:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		TopSkipLoop
		jz		TSEndLine

		add		esi, ecx
		jmp		TopSkipLoop

TSEndLine:
		dec		TopSkip
		jnz		TopSkipLoop


// Start of line loop

// Skips the pixels hanging outside the left-side boundry
LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
		mov		eax, LeftSkip					// after we have skipped enough left-side pixels
		mov		LSCount, eax					// LSCount counts how many pixels skipped so far
		or		eax, eax
		jz		BlitLineSetup					// check for nothing to skip

LeftSkipLoop:

		mov		cl, [esi]
		inc		esi

		or		cl, cl
		js		LSTrans

		cmp		ecx, LSCount
		je		LSSkip2								// if equal, skip whole, and start blit with new run
		jb		LSSkip1								// if less, skip whole thing

		add		esi, LSCount					// skip partial run, jump into normal loop for rest
		sub		ecx, LSCount
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0
		jmp		BlitNTL1							// *** jumps into non-transparent blit loop

LSSkip2:
		add		esi, ecx							// skip whole run, and start blit with new run
		jmp		BlitLineSetup


LSSkip1:
		add		esi, ecx							// skip whole run, continue skipping
		sub		LSCount, ecx
		jmp		LeftSkipLoop


LSTrans:
		and		ecx, 07fH
		cmp		ecx, LSCount
		je		BlitLineSetup					// if equal, skip whole, and start blit with new run
		jb		LSTrans1							// if less, skip whole thing

		sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
		mov		eax, BlitLength
		mov		LSCount, eax

		mov		Unblitted, 0
		jmp		BlitTransparent				// *** jumps into transparent blit loop


LSTrans1:
		sub		LSCount, ecx					// skip whole run, continue skipping
		jmp		LeftSkipLoop

//-------------------------------------------------
// setup for beginning of line

BlitLineSetup:
		mov		eax, BlitLength
		mov		LSCount, eax
		mov		Unblitted, 0

BlitDispatch:

		cmp		LSCount, 0							// Check to see if we're done blitting
		je		RightSkipLoop

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		RSLoop2

//--------------------------------
// blitting non-transparent pixels

		and		ecx, 07fH

BlitNTL1:
		mov		ax, [ebx]								// check z-level of pixel
		cmp		ax, usZLevel
		ja		BlitNTL2

		mov		ax, usZLevel						// update z-level of pixel
		mov		[ebx], ax

		// Check for shadow...
		xor		eax, eax
		mov		al, [esi]
		cmp		al, 254
		jne		BlitNTL66

		mov		al, fIgnoreShadows
		cmp		al, 0
		jne		BlitNTL2

		mov		ax, [edi]
		mov		ax, ShadeTable[eax*2]
		mov		[edi], ax
		jmp		BlitNTL2

BlitNTL66:

		mov		ax, [edx+eax*2]					// Copy pixel
		mov		[edi], ax

BlitNTL2:
		inc		esi
		add		edi, 2
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitNTL6

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitNTL5								// dir = 0 no change
		js		BlitNTL4								// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_SUBLAYERS
		jmp		BlitNTL5

BlitNTL4:
		sub		dx, Z_SUBLAYERS

BlitNTL5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitNTL6:
		dec		LSCount									// decrement pixel length to blit
		jz		RightSkipLoop						// done blitting the visible line

		dec		ecx
		jnz		BlitNTL1								// continue current run

		jmp		BlitDispatch						// done current run, go for another


//----------------------------
// skipping transparent pixels

BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

BlitTrans2:

		add		edi, 2									// move up the destination pointer
		add		ebx, 2

		dec		usZColsToGo
		jnz		BlitTrans1

// update the z-level according to the z-table

		push	edx
		mov		edx, pZArray						// get pointer to array
		xor		eax, eax
		mov		ax, usZIndex						// pick up the current array index
		add		edx, eax
		inc		eax											// increment it
		mov		usZIndex, ax						// store incremented value

		mov		al, [edx]								// get direction instruction
		mov		dx, usZLevel						// get current z-level

		or		al, al
		jz		BlitTrans5							// dir = 0 no change
		js		BlitTrans4							// dir < 0 z-level down
																	// dir > 0 z-level up (default)
		add		dx, Z_SUBLAYERS
		jmp		BlitTrans5

BlitTrans4:
		sub		dx, Z_SUBLAYERS

BlitTrans5:
		mov		usZLevel, dx						// store the now-modified z-level
		mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
		pop		edx

BlitTrans1:

		dec		LSCount									// decrement the pixels to blit
		jz		RightSkipLoop						// done the line

		dec		ecx
		jnz		BlitTrans2

		jmp		BlitDispatch

//---------------------------------------------
// Scans the ETRLE until it finds an EOL marker

RightSkipLoop:


RSLoop1:
		mov		al, [esi]
		inc		esi
		or		al, al
		jnz		RSLoop1

RSLoop2:

		dec		BlitHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip

// reset all the z-level stuff for a new line

		mov		ax, usZStartLevel
		mov		usZLevel, ax
		mov		ax, usZStartIndex
		mov		usZIndex, ax
		mov		ax, usZStartCols
		mov		usZColsToGo, ax


		jmp		LeftSkipSetup


BlitDone:
	}

	return(TRUE);
}


// Blitter Specs
// 1 ) 8 to 16 bpp
// 2 ) strip z-blitter
// 3 ) clipped
// 4 ) trans shadow - if value is 254, makes a shadow
//
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncClipAlpha(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, HVOBJECT hAlphaVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows)
{
	UINT32 uiOffset;
	UINT32 usHeight, usWidth, Unblitted;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr, *AlphaPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	INT32	 iTempX, iTempY, LeftSkip, RightSkip, TopSkip, BottomSkip, BlitLength, BlitHeight, LSCount;
	INT32  ClipX1, ClipY1, ClipX2, ClipY2;
	UINT16 usZLevel, usZStartLevel, usZColsToGo, usZStartIndex, usCount, usZIndex, usZStartCols;
	INT8 *pZArray;
	ZStripInfo *pZInfo;

	// Assertions
	Assert(hSrcVObject != NULL);
	Assert(hAlphaVObject != NULL);
	Assert(pBuffer != NULL);

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[usIndex]);
	usHeight = (UINT32)pTrav->usHeight;
	usWidth = (UINT32)pTrav->usWidth;
	uiOffset = pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	if (clipregion == NULL)
	{
		ClipX1 = ClippingRect.iLeft;
		ClipY1 = ClippingRect.iTop;
		ClipX2 = ClippingRect.iRight;
		ClipY2 = ClippingRect.iBottom;
	}
	else
	{
		ClipX1 = clipregion->iLeft;
		ClipY1 = clipregion->iTop;
		ClipX2 = clipregion->iRight;
		ClipY2 = clipregion->iBottom;
	}

	// Calculate rows hanging off each side of the screen
	LeftSkip = __min(ClipX1 - min(ClipX1, iTempX), (INT32)usWidth);
	RightSkip = __min(max(ClipX2, (iTempX + (INT32)usWidth)) - ClipX2, (INT32)usWidth);
	TopSkip = __min(ClipY1 - __min(ClipY1, iTempY), (INT32)usHeight);
	BottomSkip = __min(__max(ClipY2, (iTempY + (INT32)usHeight)) - ClipY2, (INT32)usHeight);

	// calculate the remaining rows and columns to blit
	BlitLength = ((INT32)usWidth - LeftSkip - RightSkip);
	BlitHeight = ((INT32)usHeight - TopSkip - BottomSkip);

	// check if whole thing is clipped
	if ((LeftSkip >= (INT32)usWidth) || (RightSkip >= (INT32)usWidth))
		return(TRUE);

	// check if whole thing is clipped
	if ((TopSkip >= (INT32)usHeight) || (BottomSkip >= (INT32)usHeight))
		return(TRUE);

	SrcPtr = (UINT8 *)hSrcVObject->pPixData + uiOffset;
	AlphaPtr = (UINT8 *)hAlphaVObject->pPixData + (hAlphaVObject->pETRLEObject[usIndex]).uiDataOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*(iTempY + TopSkip)) + ((iTempX + LeftSkip) * 2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*(iTempY + TopSkip)) + ((iTempX + LeftSkip) * 2);
	LineSkip = (uiDestPitchBYTES - (BlitLength * 2));

	if (hSrcVObject->ppZStripInfo == NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}
	// setup for the z-column blitting stuff
	pZInfo = hSrcVObject->ppZStripInfo[sZIndex];
	if (pZInfo == NULL)
	{
		DebugMsg(TOPIC_VIDEOOBJECT, DBG_LEVEL_0, String("Missing Z-Strip info on multi-Z object"));
		return(FALSE);
	}

	usZStartLevel = (UINT16)((INT16)usZValue + ((INT16)pZInfo->bInitialZChange*Z_SUBLAYERS * 10));

	if (LeftSkip > pZInfo->ubFirstZStripWidth)
	{
		usZStartCols = (LeftSkip - pZInfo->ubFirstZStripWidth);
		usZStartCols = 20 - (usZStartCols % 20);
	}
	else if (LeftSkip < pZInfo->ubFirstZStripWidth)
		usZStartCols = (UINT16)(pZInfo->ubFirstZStripWidth - LeftSkip);
	else
		usZStartCols = 20;

	// set to odd number of pixels for first column
	usZColsToGo = usZStartCols;

	pZArray = pZInfo->pbZChange;

	if (LeftSkip >= usZColsToGo)
	{
		// Index into array after doing left clipping
		usZStartIndex = 1 + ((LeftSkip - pZInfo->ubFirstZStripWidth) / 20);

		//calculates the Z-value after left-side clipping
		if (usZStartIndex)
		{
			for (usCount = 0; usCount < usZStartIndex; usCount++)
			{
				switch (pZArray[usCount])
				{
				case -1:	usZStartLevel -= Z_SUBLAYERS;
					break;
				case 0:		//no change
					break;
				case 1:		usZStartLevel += Z_SUBLAYERS;
					break;
				}
			}
		}
	}
	else
		usZStartIndex = 0;

	usZLevel = usZStartLevel;
	usZIndex = usZStartIndex;

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

		cmp		TopSkip, 0							// check for nothing clipped on top
		je		LeftSkipSetup


		// Skips the number of lines clipped at the top
		TopSkipLoop :

		mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		TopSkipLoop
			jz		TSEndLine

			add		esi, ecx

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			jmp		TopSkipLoop

			TSEndLine :
		dec		TopSkip
			jnz		TopSkipLoop


			// Start of line loop

			// Skips the pixels hanging outside the left-side boundry
		LeftSkipSetup:

		mov		Unblitted, 0					// Unblitted counts any pixels left from a run
			mov		eax, LeftSkip					// after we have skipped enough left-side pixels
			mov		LSCount, eax					// LSCount counts how many pixels skipped so far
			or eax, eax
			jz		BlitLineSetup					// check for nothing to skip

			LeftSkipLoop :

		mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		LSTrans

			cmp		ecx, LSCount
			je		LSSkip2								// if equal, skip whole, and start blit with new run
			jb		LSSkip1								// if less, skip whole thing

			add		esi, LSCount					// skip partial run, jump into normal loop for rest

			push	esi
			mov		esi, AlphaPtr
			add		esi, LSCount
			mov		AlphaPtr, esi
			pop		esi

			sub		ecx, LSCount
			mov		eax, BlitLength
			mov		LSCount, eax
			mov		Unblitted, 0
			jmp		BlitNTL1							// *** jumps into non-transparent blit loop

			LSSkip2 :
		add		esi, ecx							// skip whole run, and start blit with new run

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			jmp		BlitLineSetup


			LSSkip1 :
		add		esi, ecx							// skip whole run, continue skipping

			push	esi
			mov		esi, AlphaPtr
			add		esi, ecx
			mov		AlphaPtr, esi
			pop		esi

			sub		LSCount, ecx
			jmp		LeftSkipLoop


			LSTrans :
		and		ecx, 07fH
			cmp		ecx, LSCount
			je		BlitLineSetup					// if equal, skip whole, and start blit with new run
			jb		LSTrans1							// if less, skip whole thing

			sub		ecx, LSCount							// skip partial run, jump into normal loop for rest
			mov		eax, BlitLength
			mov		LSCount, eax

			mov		Unblitted, 0
			jmp		BlitTransparent				// *** jumps into transparent blit loop


			LSTrans1 :
		sub		LSCount, ecx					// skip whole run, continue skipping
			jmp		LeftSkipLoop

			//-------------------------------------------------
			// setup for beginning of line

			BlitLineSetup :
		mov		eax, BlitLength
			mov		LSCount, eax
			mov		Unblitted, 0

			BlitDispatch :

			cmp		LSCount, 0							// Check to see if we're done blitting
			je		RightSkipLoop

			mov		cl, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or cl, cl
			js		BlitTransparent
			jz		RSLoop2

			//--------------------------------
			// blitting non-transparent pixels

			and		ecx, 07fH

			BlitNTL1 :
		mov		ax, [ebx]								// check z-level of pixel
			cmp		ax, usZLevel
			ja		BlitNTL2

			mov		ax, usZLevel						// update z-level of pixel
			mov[ebx], ax

			// Check for shadow...
			xor		eax, eax
			mov		al, [esi]
			cmp		al, 254
			jne		BlitNTL66

			mov		al, fIgnoreShadows
			cmp		al, 0
			jne		BlitNTL2

			mov		ax, [edi]
			mov		ax, ShadeTable[eax * 2]
			mov[edi], ax
			jmp		BlitNTL2

			BlitNTL66 :

		mov		ax, [edx + eax * 2]					// Copy pixel

			push	edx
			push	ecx
			push	ebx
			push	esi
			mov		esi, AlphaPtr
			xor		ebx, ebx
			mov		bl, [esi]
			pop		esi
			push	ebx
			push[edi]
			push	eax
			call    blendWithAlpha
			add     esp, 12
			pop		ebx
			pop		ecx
			pop		edx

			mov[edi], ax

			BlitNTL2 :
		inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			add		edi, 2
			add		ebx, 2

			dec		usZColsToGo
			jnz		BlitNTL6

			// update the z-level according to the z-table

			push	edx
			mov		edx, pZArray						// get pointer to array
			xor		eax, eax
			mov		ax, usZIndex						// pick up the current array index
			add		edx, eax
			inc		eax											// increment it
			mov		usZIndex, ax						// store incremented value

			mov		al, [edx]								// get direction instruction
			mov		dx, usZLevel						// get current z-level

			or al, al
			jz		BlitNTL5								// dir = 0 no change
			js		BlitNTL4								// dir < 0 z-level down
																		// dir > 0 z-level up (default)
			add		dx, Z_SUBLAYERS
			jmp		BlitNTL5

			BlitNTL4 :
		sub		dx, Z_SUBLAYERS

			BlitNTL5 :
		mov		usZLevel, dx						// store the now-modified z-level
			mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
			pop		edx

			BlitNTL6 :
		dec		LSCount									// decrement pixel length to blit
			jz		RightSkipLoop						// done blitting the visible line

			dec		ecx
			jnz		BlitNTL1								// continue current run

			jmp		BlitDispatch						// done current run, go for another


	//----------------------------
	// skipping transparent pixels

		BlitTransparent:									// skip transparent pixels

		and		ecx, 07fH

			BlitTrans2 :

		add		edi, 2									// move up the destination pointer
			add		ebx, 2

			dec		usZColsToGo
			jnz		BlitTrans1

			// update the z-level according to the z-table

			push	edx
			mov		edx, pZArray						// get pointer to array
			xor		eax, eax
			mov		ax, usZIndex						// pick up the current array index
			add		edx, eax
			inc		eax											// increment it
			mov		usZIndex, ax						// store incremented value

			mov		al, [edx]								// get direction instruction
			mov		dx, usZLevel						// get current z-level

			or al, al
			jz		BlitTrans5							// dir = 0 no change
			js		BlitTrans4							// dir < 0 z-level down
																		// dir > 0 z-level up (default)
			add		dx, Z_SUBLAYERS
			jmp		BlitTrans5

			BlitTrans4 :
		sub		dx, Z_SUBLAYERS

			BlitTrans5 :
		mov		usZLevel, dx						// store the now-modified z-level
			mov		usZColsToGo, 20					// reset the next z-level change to 20 cols
			pop		edx

			BlitTrans1 :

		dec		LSCount									// decrement the pixels to blit
			jz		RightSkipLoop						// done the line

			dec		ecx
			jnz		BlitTrans2

			jmp		BlitDispatch

			//---------------------------------------------
			// Scans the ETRLE until it finds an EOL marker

			RightSkipLoop :


	RSLoop1:
		mov		al, [esi]
			inc		esi

			push	esi
			mov		esi, AlphaPtr
			inc		esi
			mov		AlphaPtr, esi
			pop		esi

			or al, al
			jnz		RSLoop1

			RSLoop2 :

		dec		BlitHeight
			jz		BlitDone
			add		edi, LineSkip
			add		ebx, LineSkip

			// reset all the z-level stuff for a new line

			mov		ax, usZStartLevel
			mov		usZLevel, ax
			mov		ax, usZStartIndex
			mov		usZIndex, ax
			mov		ax, usZStartCols
			mov		usZColsToGo, ax


			jmp		LeftSkipSetup


			BlitDone :
	}

	return(TRUE);
}



void RenderRoomInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS )
{
	INT8				bXOddFlag = 0;
	INT16				sAnchorPosX_M, sAnchorPosY_M;
	INT16				sAnchorPosX_S, sAnchorPosY_S;
	INT16				sTempPosX_M, sTempPosY_M;
	INT16				sTempPosX_S, sTempPosY_S;
	BOOLEAN			fEndRenderRow = FALSE, fEndRenderCol = FALSE;
	INT16				sX, sY;
	INT32			usTileIndex;//dnl ch56 141009
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;


	// Begin Render Loop
	sAnchorPosX_M = sStartPointX_M;
	sAnchorPosY_M = sStartPointY_M;
	sAnchorPosX_S = sStartPointX_S;
	sAnchorPosY_S = sStartPointY_S;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	do
	{

		fEndRenderRow = FALSE;
		sTempPosX_M = sAnchorPosX_M;
		sTempPosY_M = sAnchorPosY_M;
		sTempPosX_S = sAnchorPosX_S;
		sTempPosY_S = sAnchorPosY_S;

		if(bXOddFlag > 0)
			sTempPosX_S += 20;


		do
		{

			usTileIndex=FASTMAPROWCOLTOPOS( sTempPosY_M, sTempPosX_M );

			if ( usTileIndex < GRIDSIZE	)
			{
				sX = sTempPosX_S + ( WORLD_TILE_X / 2 ) - 5;
				sY = sTempPosY_S + ( WORLD_TILE_Y / 2 ) - 5;

				// THIS ROOM STUFF IS ONLY DONE IN THE EDITOR...
				// ADJUST BY SHEIGHT
				sY -= gpWorldLevelData[ usTileIndex ].sHeight;
				sY += gsRenderHeight;//dnl ch85 030214

				if ( gusWorldRoomInfo[ usTileIndex ] != NO_ROOM )
				{
					SetFont( SMALLCOMPFONT );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, gsVIEWPORT_END_Y, FALSE );
					switch( gusWorldRoomInfo[ usTileIndex ] % 5 )
					{
						case 0:		SetFontForeground( FONT_GRAY3 );	break;
						case 1:		SetFontForeground( FONT_YELLOW );	break;
						case 2:		SetFontForeground( FONT_LTRED );	break;
						case 3:		SetFontForeground( FONT_LTBLUE );	break;
						case 4:   SetFontForeground( FONT_LTGREEN );break;
					}
					mprintf_buffer( pDestBuf, uiDestPitchBYTES, TINYFONT1, sX,  sY , L"%d", gusWorldRoomInfo[ usTileIndex ] );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				}
			}

			sTempPosX_S += 40;
			sTempPosX_M ++;
			sTempPosY_M --;

			if ( sTempPosX_S >= sEndXS )
			{
				fEndRenderRow = TRUE;
			}

		} while( !fEndRenderRow );

		if ( bXOddFlag > 0 )
		{
			sAnchorPosY_M ++;
		}
		else
		{
			sAnchorPosX_M ++;
		}


		bXOddFlag = !bXOddFlag;
		sAnchorPosY_S += 10;

		if ( sAnchorPosY_S >= sEndYS )
		{
			fEndRenderCol = TRUE;
		}

	}
	while( !fEndRenderCol );

	UnLockVideoSurface( FRAME_BUFFER );

}


#ifdef _DEBUG

void RenderFOVDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS )
{
	INT8				bXOddFlag = 0;
	INT16				sAnchorPosX_M, sAnchorPosY_M;
	INT16				sAnchorPosX_S, sAnchorPosY_S;
	INT16				sTempPosX_M, sTempPosY_M;
	INT16				sTempPosX_S, sTempPosY_S;
	BOOLEAN			fEndRenderRow = FALSE, fEndRenderCol = FALSE;
	UINT16			usTileIndex;
	INT16				sX, sY;
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;


	// Begin Render Loop
	sAnchorPosX_M = sStartPointX_M;
	sAnchorPosY_M = sStartPointY_M;
	sAnchorPosX_S = sStartPointX_S;
	sAnchorPosY_S = sStartPointY_S;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	do
	{

		fEndRenderRow = FALSE;
		sTempPosX_M = sAnchorPosX_M;
		sTempPosY_M = sAnchorPosY_M;
		sTempPosX_S = sAnchorPosX_S;
		sTempPosY_S = sAnchorPosY_S;

		if(bXOddFlag > 0)
			sTempPosX_S += 20;


		do
		{

			usTileIndex=FASTMAPROWCOLTOPOS( sTempPosY_M, sTempPosX_M );

			if ( usTileIndex < GRIDSIZE	)
			{
				sX = sTempPosX_S + ( WORLD_TILE_X / 2 ) - 5;
				sY = sTempPosY_S + ( WORLD_TILE_Y / 2 ) - 5;

				// Adjust for interface level
				sY -= gpWorldLevelData[ usTileIndex ].sHeight;
				sY += gsRenderHeight;

				if ( gubFOVDebugInfoInfo[ usTileIndex ] != 0 )
				{
					SetFont( SMALLCOMPFONT );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, gsVIEWPORT_END_Y, FALSE );
					SetFontForeground( FONT_GRAY3 );
					mprintf_buffer( pDestBuf, uiDestPitchBYTES, TINYFONT1, sX,  sY , L"%d", gubFOVDebugInfoInfo[ usTileIndex ] );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );


					Blt8BPPDataTo16BPPBufferTransparentClip((UINT16*)pDestBuf, uiDestPitchBYTES, gTileDatabase[0].hTileSurface, sTempPosX_S, sTempPosY_S, 0, &gClippingRect );

				}

			  if ( gubGridNoMarkers[ usTileIndex ] == gubGridNoValue )
				{
					SetFont( SMALLCOMPFONT );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, gsVIEWPORT_END_Y, FALSE );
					SetFontForeground( FONT_FCOLOR_YELLOW );
					mprintf_buffer( pDestBuf, uiDestPitchBYTES, TINYFONT1, sX,  sY + 4 , L"x" );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				}

			}

			sTempPosX_S += 40;
			sTempPosX_M ++;
			sTempPosY_M --;

			if ( sTempPosX_S >= sEndXS )
			{
				fEndRenderRow = TRUE;
			}

		} while( !fEndRenderRow );

		if ( bXOddFlag > 0 )
		{
			sAnchorPosY_M ++;
		}
		else
		{
			sAnchorPosX_M ++;
		}


		bXOddFlag = !bXOddFlag;
		sAnchorPosY_S += 10;

		if ( sAnchorPosY_S >= sEndYS )
		{
			fEndRenderCol = TRUE;
		}

	}
	while( !fEndRenderCol );

	UnLockVideoSurface( FRAME_BUFFER );

}

void RenderCoverDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS )
{
	INT8				bXOddFlag = 0;
	INT16				sAnchorPosX_M, sAnchorPosY_M;
	INT16				sAnchorPosX_S, sAnchorPosY_S;
	INT16				sTempPosX_M, sTempPosY_M;
	INT16				sTempPosX_S, sTempPosY_S;
	BOOLEAN			fEndRenderRow = FALSE, fEndRenderCol = FALSE;
	UINT16			usTileIndex;
	INT16				sX, sY;
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;


	// Begin Render Loop
	sAnchorPosX_M = sStartPointX_M;
	sAnchorPosY_M = sStartPointY_M;
	sAnchorPosX_S = sStartPointX_S;
	sAnchorPosY_S = sStartPointY_S;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	do
	{

		fEndRenderRow = FALSE;
		sTempPosX_M = sAnchorPosX_M;
		sTempPosY_M = sAnchorPosY_M;
		sTempPosX_S = sAnchorPosX_S;
		sTempPosY_S = sAnchorPosY_S;

		if(bXOddFlag > 0)
			sTempPosX_S += 20;


		do
		{

			usTileIndex=FASTMAPROWCOLTOPOS( sTempPosY_M, sTempPosX_M );

			if ( usTileIndex < GRIDSIZE	)
			{
				sX = sTempPosX_S + ( WORLD_TILE_X / 2 ) - 5;
				sY = sTempPosY_S + ( WORLD_TILE_Y / 2 ) - 5;

				// Adjust for interface level
				sY -= gpWorldLevelData[ usTileIndex ].sHeight;
				sY += gsRenderHeight;

				if (gsCoverValue[ usTileIndex] != 0x7F7F)
				{
					SetFont( SMALLCOMPFONT );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, gsVIEWPORT_END_Y, FALSE );
					if (usTileIndex == gsBestCover)
					{
						SetFontForeground( FONT_MCOLOR_RED );
					}
					else if (gsCoverValue[ usTileIndex ] < 0)
					{
						SetFontForeground( FONT_MCOLOR_WHITE );
					}
					else
					{
						SetFontForeground( FONT_GRAY3 );
					}
					mprintf_buffer( pDestBuf, uiDestPitchBYTES, TINYFONT1, sX,  sY , L"%d", gsCoverValue[ usTileIndex ] );
					SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				}

			}

			sTempPosX_S += 40;
			sTempPosX_M ++;
			sTempPosY_M --;

			if ( sTempPosX_S >= sEndXS )
			{
				fEndRenderRow = TRUE;
			}

		} while( !fEndRenderRow );

		if ( bXOddFlag > 0 )
		{
			sAnchorPosY_M ++;
		}
		else
		{
			sAnchorPosX_M ++;
		}


		bXOddFlag = !bXOddFlag;
		sAnchorPosY_S += 10;

		if ( sAnchorPosY_S >= sEndYS )
		{
			fEndRenderCol = TRUE;
		}

	}
	while( !fEndRenderCol );

	UnLockVideoSurface( FRAME_BUFFER );

}


void RenderGridNoVisibleDebugInfo( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS )
{
	INT8				bXOddFlag = 0;
	INT16				sAnchorPosX_M, sAnchorPosY_M;
	INT16				sAnchorPosX_S, sAnchorPosY_S;
	INT16				sTempPosX_M, sTempPosY_M;
	INT16				sTempPosX_S, sTempPosY_S;
	BOOLEAN			fEndRenderRow = FALSE, fEndRenderCol = FALSE;
	INT16				sX, sY;
	INT32			usTileIndex;//dnl ch56 141009
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;


	// Begin Render Loop
	sAnchorPosX_M = sStartPointX_M;
	sAnchorPosY_M = sStartPointY_M;
	sAnchorPosX_S = sStartPointX_S;
	sAnchorPosY_S = sStartPointY_S;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	do
	{

		fEndRenderRow = FALSE;
		sTempPosX_M = sAnchorPosX_M;
		sTempPosY_M = sAnchorPosY_M;
		sTempPosX_S = sAnchorPosX_S;
		sTempPosY_S = sAnchorPosY_S;

		if(bXOddFlag > 0)
			sTempPosX_S += 20;


		do
		{

			usTileIndex=FASTMAPROWCOLTOPOS( sTempPosY_M, sTempPosX_M );

			if ( usTileIndex < GRIDSIZE	)
			{
				sX = sTempPosX_S + ( WORLD_TILE_X / 2 ) - 5;
				sY = sTempPosY_S + ( WORLD_TILE_Y / 2 ) - 5;

				// Adjust for interface level
				sY -= gpWorldLevelData[ usTileIndex ].sHeight;
				sY += gsRenderHeight;

				SetFont( SMALLCOMPFONT );
				SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, gsVIEWPORT_END_Y, FALSE );

				if ( !GridNoOnVisibleWorldTile( usTileIndex ) )
				{
					SetFontForeground( FONT_MCOLOR_RED );
				}
				else
				{
					SetFontForeground( FONT_GRAY3 );
				}
				mprintf_buffer( pDestBuf, uiDestPitchBYTES, TINYFONT1, sX,  sY , L"%d", usTileIndex );
				SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			}

			sTempPosX_S += 40;
			sTempPosX_M ++;
			sTempPosY_M --;

			if ( sTempPosX_S >= sEndXS )
			{
				fEndRenderRow = TRUE;
			}

		} while( !fEndRenderRow );

		if ( bXOddFlag > 0 )
		{
			sAnchorPosY_M ++;
		}
		else
		{
			sAnchorPosX_M ++;
		}


		bXOddFlag = !bXOddFlag;
		sAnchorPosY_S += 10;

		if ( sAnchorPosY_S >= sEndYS )
		{
			fEndRenderCol = TRUE;
		}

	}
	while( !fEndRenderCol );

	UnLockVideoSurface( FRAME_BUFFER );

}


#endif


void ExamineZBufferRect( INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom)
{
	CalcRenderParameters( sLeft, sTop, sRight, sBottom );

	ExamineZBufferForHiddenTiles( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );

}


void ExamineZBufferForHiddenTiles( INT16 sStartPointX_M, INT16 sStartPointY_M, INT16 sStartPointX_S, INT16 sStartPointY_S, INT16 sEndXS, INT16 sEndYS )
{
	INT8				bXOddFlag = 0;
	INT16				sAnchorPosX_M, sAnchorPosY_M;
	INT16				sAnchorPosX_S, sAnchorPosY_S;
	INT16				sTempPosX_M, sTempPosY_M;
	INT16				sTempPosX_S, sTempPosY_S;
	BOOLEAN			fEndRenderRow = FALSE, fEndRenderCol = FALSE;
	UINT16			usTileIndex;
	INT16				sX, sY, sWorldX, sZLevel;
	UINT32			uiDestPitchBYTES;
	UINT8				*pDestBuf;
	TILE_ELEMENT *TileElem;
	INT8			bBlitClipVal;
	LEVELNODE		*pObject;


	// Begin Render Loop
	sAnchorPosX_M = sStartPointX_M;
	sAnchorPosY_M = sStartPointY_M;
	sAnchorPosX_S = sStartPointX_S;
	sAnchorPosY_S = sStartPointY_S;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	// Get VObject for first land peice!
	TileElem = &(gTileDatabase[ FIRSTTEXTURE1 ] );


	do
	{

		fEndRenderRow = FALSE;
		sTempPosX_M = sAnchorPosX_M;
		sTempPosY_M = sAnchorPosY_M;
		sTempPosX_S = sAnchorPosX_S;
		sTempPosY_S = sAnchorPosY_S;

		if(bXOddFlag > 0)
			sTempPosX_S += 20;


		do
		{

			usTileIndex=FASTMAPROWCOLTOPOS( sTempPosY_M, sTempPosX_M );

			if ( usTileIndex < GRIDSIZE	)
			{
				// ATE: Don;t let any vehicle sit here....
				if ( FindStructure( usTileIndex, ( STRUCTURE_MOBILE ) ) )
				{
					// Continue...
					goto ENDOFLOOP;
				}

				sX = sTempPosX_S;
				sY = sTempPosY_S - gpWorldLevelData[usTileIndex].sHeight;

				// Adjust for interface level
				sY += gsRenderHeight;

				// Caluluate zvalue
				// Look for anything less than struct layer!
				GetWorldXYAbsoluteScreenXY( sTempPosX_M, sTempPosY_M, &sWorldX, &sZLevel);

				sZLevel += gsRenderHeight;

				sZLevel=(sZLevel*Z_SUBLAYERS)+STRUCT_Z_LEVEL;

				if ( gpWorldLevelData[usTileIndex].uiFlags & MAPELEMENT_REEVALUATE_REDUNDENCY )
				{
					bBlitClipVal = BltIsClippedOrOffScreen(TileElem->hTileSurface, sX, sY, TileElem->usRegionIndex, &gClippingRect);

					if ( bBlitClipVal == FALSE )
					{
						// Set flag to not evaluate again!
						gpWorldLevelData[usTileIndex].uiFlags &= (~MAPELEMENT_REEVALUATE_REDUNDENCY );

						// OK, first do some rules with exceptions
						// Don't let this happen for roads!
						pObject = gpWorldLevelData[usTileIndex ].pObjectHead;

						if ( IsTileRedundent( uiDestPitchBYTES, gpZBuffer, sZLevel, TileElem->hTileSurface, sX, sY, TileElem->usRegionIndex ) )
						{
							// Mark in the world!
							gpWorldLevelData[ usTileIndex ].uiFlags |= MAPELEMENT_REDUNDENT;
						}
						else
						{
							// Un Mark in the world!
							gpWorldLevelData[ usTileIndex ].uiFlags &= (~MAPELEMENT_REDUNDENT);
						}

					}
				}
			}

ENDOFLOOP:

			sTempPosX_S += 40;
			sTempPosX_M ++;
			sTempPosY_M --;

			if ( sTempPosX_S >= sEndXS )
			{
				fEndRenderRow = TRUE;
			}

		} while( !fEndRenderRow );

		if ( bXOddFlag > 0 )
		{
			sAnchorPosY_M ++;
		}
		else
		{
			sAnchorPosX_M ++;
		}


		bXOddFlag = !bXOddFlag;
		sAnchorPosY_S += 10;

		if ( sAnchorPosY_S >= sEndYS )
		{
			fEndRenderCol = TRUE;
		}

	}
	while( !fEndRenderCol );

	UnLockVideoSurface( FRAME_BUFFER );

}



void CalcRenderParameters(INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom )
{
	INT16 sTempPosX_W, sTempPosY_W;
	INT16 sRenderCenterX_W, sRenderCenterY_W;
	INT16 sOffsetX_W, sOffsetY_W, sOffsetX_S, sOffsetY_S;

	gOldClipRect = gClippingRect;

	// Set new clipped rect
	gClippingRect.iLeft = __max( gsVIEWPORT_START_X, sLeft);
	gClippingRect.iRight = __min( gsVIEWPORT_END_X, sRight);
	gClippingRect.iTop = __max( gsVIEWPORT_WINDOW_START_Y, sTop);
	gClippingRect.iBottom = __min(gsVIEWPORT_WINDOW_END_Y, sBottom);

	gsEndXS = sRight + VIEWPORT_XOFFSET_S;
	gsEndYS = sBottom + VIEWPORT_YOFFSET_S;

	sRenderCenterX_W = gsRenderCenterX;
	sRenderCenterY_W = gsRenderCenterY;

	// STEP THREE - determine starting point in world coords
	// a) Determine where in screen coords to start rendering
	gsStartPointX_S = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) - (sLeft - VIEWPORT_XOFFSET_S);
	gsStartPointY_S = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) - (sTop - VIEWPORT_YOFFSET_S);


	// b) Convert these distances into world distances
	FromScreenToCellCoordinates( gsStartPointX_S, gsStartPointY_S, &sTempPosX_W, &sTempPosY_W );

	// c) World start point is Render center minus this distance
	gsStartPointX_W = sRenderCenterX_W - sTempPosX_W;
	gsStartPointY_W = sRenderCenterY_W - sTempPosY_W;

	// NOTE: Increase X map value by 1 tile to offset where on screen we are...
	if ( gsStartPointX_W > 0 )
		gsStartPointX_W += CELL_X_SIZE;

	// d) screen start point is screen distances minus screen center
	gsStartPointX_S = sLeft - VIEWPORT_XOFFSET_S;
	gsStartPointY_S = sTop - VIEWPORT_YOFFSET_S;

	// STEP FOUR - Determine Start block
	// a) Find start block
	gsStartPointX_M = ( gsStartPointX_W  ) / CELL_X_SIZE;
	gsStartPointY_M = ( gsStartPointY_W  ) / CELL_Y_SIZE;

	// STEP 5 - Determine Deltas for center and find screen values
	//Make sure these coordinates are multiples of scroll steps
	sOffsetX_W = abs( gsStartPointX_W ) - ( abs( ( gsStartPointX_M * CELL_X_SIZE ) ) );
	sOffsetY_W = abs( gsStartPointY_W ) - ( abs( ( gsStartPointY_M * CELL_Y_SIZE ) ) );

	FromCellToScreenCoordinates( sOffsetX_W, sOffsetY_W, &sOffsetX_S, &sOffsetY_S );

	if ( gsStartPointY_W < 0 )
	{
		gsStartPointY_S	+= 0;//(sOffsetY_S/2);
	}
	else
	{
		gsStartPointY_S	-= sOffsetY_S;
	}
	gsStartPointX_S	-= sOffsetX_S;


	// Set globals for render offset
	if ( gsRenderWorldOffsetX == -1 )
	{
		gsRenderWorldOffsetX = sOffsetX_S;
	}

	if ( gsRenderWorldOffsetY == -1 )
	{
		gsRenderWorldOffsetY = sOffsetY_S;
	}

	/////////////////////////////////////////
	//ATE: CALCULATE LARGER OFFSET VALUES
	gsLEndXS = sRight + LARGER_VIEWPORT_XOFFSET_S;
	gsLEndYS = sBottom + LARGER_VIEWPORT_YOFFSET_S;

		// STEP THREE - determine starting point in world coords
	// a) Determine where in screen coords to start rendering
	gsLStartPointX_S = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) - (sLeft - LARGER_VIEWPORT_XOFFSET_S);
	gsLStartPointY_S = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) - (sTop - LARGER_VIEWPORT_YOFFSET_S);


	// b) Convert these distances into world distances
	FromScreenToCellCoordinates( gsLStartPointX_S, gsLStartPointY_S, &sTempPosX_W, &sTempPosY_W );

	// c) World start point is Render center minus this distance
	gsLStartPointX_W = sRenderCenterX_W - sTempPosX_W;
	gsLStartPointY_W = sRenderCenterY_W - sTempPosY_W;

	// NOTE: Increase X map value by 1 tile to offset where on screen we are...
	if ( gsLStartPointX_W > 0 )
		gsLStartPointX_W += CELL_X_SIZE;

	// d) screen start point is screen distances minus screen center
	gsLStartPointX_S = sLeft - LARGER_VIEWPORT_XOFFSET_S;
	gsLStartPointY_S = sTop - LARGER_VIEWPORT_YOFFSET_S;

	// STEP FOUR - Determine Start block
	// a) Find start block
	gsLStartPointX_M = ( gsLStartPointX_W  ) / CELL_X_SIZE;
	gsLStartPointY_M = ( gsLStartPointY_W  ) / CELL_Y_SIZE;

	// Adjust starting screen coordinates
	gsLStartPointX_S	-= sOffsetX_S;

	if ( gsLStartPointY_W < 0 )
	{
		gsLStartPointY_S	+= 0;
		gsLStartPointX_S	-= 20;
	}
	else
	{
		gsLStartPointY_S	-= sOffsetY_S;
	}

}

void ResetRenderParameters(  )
{
	// Restore clipping rect
	gClippingRect =	gOldClipRect;
}




BOOLEAN Zero8BPPDataTo16BPPBufferTransparent( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex )
{
	UINT32 uiOffset;
	UINT32 usHeight, usWidth;
	UINT8	 *SrcPtr, *DestPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	INT32	 iTempX, iTempY;


	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	// Validations
	CHECKF( iTempX >= 0 );
	CHECKF( iTempY >= 0 );


	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*iTempY) + (iTempX*2);
	LineSkip=(uiDestPitchBYTES-(usWidth*2));

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		xor		eax, eax
		xor		ebx, ebx
		xor		ecx, ecx

BlitDispatch:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		BlitDoneLine

//BlitNonTransLoop:

		clc
		rcr		cl, 1
		jnc		BlitNTL2

		mov		[edi], ax

		inc		esi
		add		edi, 2

BlitNTL2:
		clc
		rcr		cl, 1
		jnc		BlitNTL3

		mov		[edi], ax

		mov		[edi+2], ax

		add		esi, 2
		add		edi, 4

BlitNTL3:

		or		cl, cl
		jz		BlitDispatch

		xor		ebx, ebx

BlitNTL4:

		mov		[edi], ax

		mov		[edi+2], ax

		mov		[edi+4], ax

		mov		[edi+6], ax

		add		esi, 4
		add		edi, 8
		dec		cl
		jnz		BlitNTL4

		jmp		BlitDispatch

BlitTransparent:

		and		ecx, 07fH
//		shl		ecx, 1
		add   ecx, ecx
		add		edi, ecx
		jmp		BlitDispatch


BlitDoneLine:

		dec		usHeight
		jz		BlitDone
		add		edi, LineSkip
		jmp		BlitDispatch


BlitDone:
	}

	return(TRUE);

}


BOOLEAN Blt8BPPDataTo16BPPBufferTransInvZ( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex )
{
	UINT16 *p16BPPPalette;
	UINT32 uiOffset;
	UINT32 usHeight, usWidth;
	UINT8	 *SrcPtr, *DestPtr, *ZPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	INT32	 iTempX, iTempY;


	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	// Validations
	CHECKF( iTempX >= 0 );
	CHECKF( iTempY >= 0 );


	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*iTempY) + (iTempX*2);
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*iTempY) + (iTempX*2);
	p16BPPPalette = hSrcVObject->pShadeCurrent;
	LineSkip=(uiDestPitchBYTES-(usWidth*2));

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

BlitDispatch:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		BlitDoneLine

//BlitNonTransLoop:

		xor		eax, eax

BlitNTL4:

		mov		ax, usZValue
		cmp		ax, [ebx]
		jne		BlitNTL5

		//mov		[ebx], ax

		xor		ah, ah
		mov		al, [esi]
		mov		ax, [edx+eax*2]
		mov		[edi], ax

BlitNTL5:
		inc		esi
		inc		edi
		inc		ebx
		inc		edi
		inc		ebx

		dec		cl
		jnz		BlitNTL4

		jmp		BlitDispatch


BlitTransparent:

		and		ecx, 07fH
//		shl		ecx, 1
		add   ecx, ecx
		add		edi, ecx
		add		ebx, ecx
		jmp		BlitDispatch


BlitDoneLine:

		dec		usHeight
		jz		BlitDone
		add		edi, LineSkip
		add		ebx, LineSkip
		jmp		BlitDispatch


BlitDone:
	}

	return(TRUE);

}



BOOLEAN IsTileRedundent( UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex )
{
	UINT16 *p16BPPPalette;
	UINT32 uiOffset;
	UINT32 usHeight, usWidth;
	UINT8	 *SrcPtr, *ZPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	INT32	 iTempX, iTempY;
	BOOLEAN		fHidden = TRUE;


	// Assertions
	Assert( hSrcVObject != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	// Validations
	CHECKF( iTempX >= 0 );
	CHECKF( iTempY >= 0 );


	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;
	ZPtr = (UINT8 *)pZBuffer + (uiDestPitchBYTES*iTempY) + (iTempX*2);
	p16BPPPalette = hSrcVObject->pShadeCurrent;
	LineSkip=(uiDestPitchBYTES-(usWidth*2));

	__asm {

		mov		esi, SrcPtr
		mov		edx, p16BPPPalette
		xor		eax, eax
		mov		ebx, ZPtr
		xor		ecx, ecx

BlitDispatch:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		BlitDoneLine

//BlitNonTransLoop:

		xor		eax, eax

BlitNTL4:

		mov		ax, usZValue
		cmp		ax, [ebx]
		jle		BlitNTL5


		//    Set false, flag
		mov   fHidden, 0
		jmp		BlitDone


BlitNTL5:
		inc		esi
		inc		ebx
		inc		ebx

		dec		cl
		jnz		BlitNTL4

		jmp		BlitDispatch


BlitTransparent:

		and		ecx, 07fH
//		shl		ecx, 1
		add   ecx, ecx
		add		ebx, ecx
		jmp		BlitDispatch


BlitDoneLine:

		dec		usHeight
		jz		BlitDone
		add		ebx, LineSkip
		jmp		BlitDispatch


BlitDone:
	}

	return(fHidden);

}

void SetMercGlowFast( )
{
	//gpGlowFramePointer	= gsFastGlowFrames;
}

void SetMercGlowNormal( )
{
	gpGlowFramePointer	= gsGlowFrames;
}





void SetRenderCenter( INT16 sNewX, INT16 sNewY )
{
	if ( gfIgnoreScrolling == 1 )
	{
		return;
	}

	// Apply these new coordinates to the renderer!
	ApplyScrolling( sNewX, sNewY, TRUE , FALSE );

	// Set flag to ignore scrolling this frame
	gfIgnoreScrollDueToCenterAdjust = TRUE;

	// Set full render flag!
	// DIRTY THE WORLD!
	SetRenderFlags(RENDER_FLAG_FULL);

	gfPlotNewMovement = TRUE;

	if ( gfScrollPending == TRUE )
	{
		// Do a complete rebuild!
		gfScrollPending = FALSE;

		// Restore Interface!
		RestoreInterface( );

		// Delete Topmost blitters saved areas
		DeleteVideoOverlaysArea( );

	}

	gfScrollInertia = FALSE;

}

#ifdef _DEBUG
void RenderFOVDebug( )
{
	RenderFOVDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
}

void RenderCoverDebug( )
{
	RenderCoverDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
}

void RenderGridNoVisibleDebug( )
{
	RenderGridNoVisibleDebugInfo( gsStartPointX_M, gsStartPointY_M, gsStartPointX_S, gsStartPointY_S, gsEndXS, gsEndYS );
}

#endif
