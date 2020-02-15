#ifndef __RENDERWORLD_H
#define __RENDERWORLD_H

#include "worlddef.h"
#include "LogicalBodyTypes/Layers.h"
#include "LogicalBodyTypes/BodyTypeDB.h"

// TEMP SELECT STUFF
#define	NO_SELECT		0
#define	FULL_SELECT		1
#define SELECT_WIDTH	2
#define SELECT_HEIGHT	3


extern BOOLEAN		gfDoVideoScroll;
extern BOOLEAN		gfDoSubtileScroll;
extern UINT8		gubCurScrollSpeedID;

// RENDERING FLAGS
#define RENDER_FLAG_FULL		0x00000001
#define RENDER_FLAG_SHADOWS		0x00000002
#define RENDER_FLAG_MARKED		0x00000004
#define RENDER_FLAG_SAVEOFF		0x00000008
#define RENDER_FLAG_NOZ			0x00000010
#define RENDER_FLAG_ROOMIDS		0x00000020
#define RENDER_FLAG_CHECKZ		0x00000040
#define RENDER_FLAG_ONLYLAND	0x00000080
#define RENDER_FLAG_ONLYSTRUCT	0x00000100
#define	RENDER_FLAG_FOVDEBUG	0x00000200

#define	SCROLL_UP				0x00000001
#define	SCROLL_DOWN				0x00000002
#define	SCROLL_RIGHT			0x00000004
#define	SCROLL_LEFT				0x00000008
#define	SCROLL_UPLEFT			0x00000020
#define	SCROLL_UPRIGHT			0x00000040
#define	SCROLL_DOWNLEFT			0x00000080
#define	SCROLL_DOWNRIGHT		0x00000200


#define	Z_SUBLAYERS				8
#define	LAND_Z_LEVEL			0
#define OBJECT_Z_LEVEL			1
#define SHADOW_Z_LEVEL			2
#define MERC_Z_LEVEL			3
#define STRUCT_Z_LEVEL			4
#define ROOF_Z_LEVEL			5
#define ONROOF_Z_LEVEL			6
#define FOG_Z_LEVEL				7
#define TOPMOST_Z_LEVEL			32767


// highest bit value is rendered first!
#define	TILES_STATIC_LAND			0x00040000
#define TILES_STATIC_OBJECTS		0x00020000
#define TILES_STATIC_SHADOWS		0x00008000
#define TILES_STATIC_STRUCTURES		0x00004000
#define TILES_STATIC_ROOF			0x00002000
#define TILES_STATIC_ONROOF			0x00001000
#define TILES_STATIC_TOPMOST		0x00000800

						
// highest bit value is rendered first!
#define TILES_ALL_DYNAMICS					0x00000fff
#define TILES_DYNAMIC_CHECKFOR_INT_TILE		0x00000400
#define	TILES_DYNAMIC_LAND					0x00000200
#define TILES_DYNAMIC_OBJECTS				0x00000100
#define TILES_DYNAMIC_SHADOWS				0x00000080
#define TILES_DYNAMIC_STRUCT_MERCS			0x00000040
#define TILES_DYNAMIC_MERCS					0x00000020
#define TILES_DYNAMIC_STRUCTURES			0x00000010
#define TILES_DYNAMIC_ROOF					0x00000008
#define TILES_DYNAMIC_HIGHMERCS				0x00000004
#define TILES_DYNAMIC_ONROOF				0x00000002
#define TILES_DYNAMIC_TOPMOST				0x00000001




extern BOOLEAN	gfRenderScroll;
extern INT16	gsScrollXIncrement;
extern INT16	gsScrollYIncrement;
extern INT32	guiScrollDirection;
extern BOOLEAN	gfScrollStart;
extern INT16	gsRenderHeight;


// Distance around mercs to pixelate walls
#define REVEAL_WALLS_RADIUS	3

// GLOBAL VARIABLES
extern INT16	SCROLL_X_STEP;
extern INT16	SCROLL_Y_STEP;

extern INT16	gsVIEWPORT_START_X;		
extern INT16	gsVIEWPORT_START_Y;		
extern INT16	gsVIEWPORT_WINDOW_START_Y;			
extern INT16	gsVIEWPORT_END_Y;			
extern INT16	gsVIEWPORT_WINDOW_END_Y;			
extern INT16	gsVIEWPORT_END_X;		
	
extern INT16	gsRenderCenterX;
extern INT16	gsRenderCenterY;
extern INT16	gsRenderWorldOffsetX;
extern INT16	gsRenderWorldOffsetY;

// CURRENT VIEWPORT IN WORLD COORDS
extern INT16 gsTopLeftWorldX;
extern INT16 gsTopLeftWorldY;
extern INT16 gsTopRightWorldX;
extern INT16 gsTopRightWorldY;
extern INT16 gsBottomLeftWorldX;
extern INT16 gsBottomLeftWorldY;
extern INT16 gsBottomRightWorldX;
extern INT16 gsBottomRightWorldY;

// WANNE: If we are talking?
// This check is used, to prevent scrolling in small maps (e.g: Rebel Basement) in higher resolution (1024x768) [2007-05-14]
extern BOOLEAN gfDialogControl;


extern SGPRect	gSelectRegion;
extern SGPPoint	gSelectAnchor;
extern UINT32	fSelectMode;


// GLOBAL COORDINATES
extern INT16 gTopLeftWorldLimitX;
extern INT16 gTopLeftWorldLimitY;
extern INT16 gTopRightWorldLimitX;
extern INT16 gTopRightWorldLimitY;
extern INT16 gBottomLeftWorldLimitX;
extern INT16 gBottomLeftWorldLimitY;
extern INT16 gBottomRightWorldLimitX;
extern INT16 gBottomRightWorldLimitY;
extern INT16 gCenterWorldX;
extern INT16 gCenterWorldY;
extern INT16 gsTLX;
extern INT16 gsTLY;
extern INT16 gsTRX; 
extern INT16 gsTRY;
extern INT16 gsBLX;
extern INT16 gsBLY;
extern INT16 gsBRX;
extern INT16 gsBRY;
extern INT16 gsCX;
extern INT16 gsCY;
extern DOUBLE gdScaleX;
extern DOUBLE gdScaleY;

extern BOOLEAN fLandLayerDirty;

extern BOOLEAN	gfIgnoreScrollDueToCenterAdjust;

//dnl ch45 051009
#define MAPWIDTH (gsTRX - gsTLX)// World Screen Width
#define MAPHEIGHT (gsBRY - gsTRY)// World Screen Height


// FUNCTIONS
void ScrollWorld( );
void InitRenderParams( UINT8 ubRestrictionID );
void RenderWorld( );

/*	This procedure will initialize gsVIEWPORT_xxx variables
 *	they ware declared with static initializer 
 *	any question? joker
 */
void InitializeViewPort();
void ResetLayerOptimizing(void);
void ResetSpecificLayerOptimizing( UINT32 uiRowFlag );

// Routines of RenderWorld
extern void RenderStaticWorld(	);
extern void RenderDynamicWorld(	);
void CopyRenderBuffer( );

void SetRenderFlags(UINT32 uiFlags);
UINT32 GetRenderFlags(void);
void ClearRenderFlags(UINT32 uiFlags);

void RenderSetShadows(BOOLEAN fShadows);

extern UINT16 *gpZBuffer;
extern UINT32 gRenderFlags;
extern BOOLEAN gfIgnoreScrolling;

extern BOOLEAN		gfScrollInertia;
extern BOOLEAN		gfScrollPending;

// Definitions for dirty rectangle uploads
void ReRenderWorld(INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom);

BOOLEAN ConcealWalls(INT16 sX, INT16 sY, INT16 sRadius);
BOOLEAN RevealWalls(INT16 sX, INT16 sY, INT16 sRadius);
void ConcealAllWalls(void);

BOOLEAN ApplyScrolling( INT16 sTempRenderCenterX, INT16 sTempRenderCenterY, BOOLEAN fForceAdjust, BOOLEAN fCheckOnly );

BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncClip( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion);

void RenderStaticWorldRect(INT16, INT16, INT16, INT16, BOOLEAN );
void RenderMarkedWorld(void);
void RenderDynamicMercWorld( void );

void ExamineZBufferRect( INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom);

void InvalidateWorldRedundency(void);
void InvalidateWorldRedundencyRadius(INT16 sX, INT16 sY, INT16 sRadius);
void DirtyWorldRender( );

// These two functions will setup the glow frame script to use then glowing enemy mercs....
void SetMercGlowFast( );
void SetMercGlowNormal( );

void SetRenderCenter( INT16 sNewX, INT16 sNewY );

#ifdef _DEBUG
void RenderFOVDebug( );
#endif

BOOLEAN Zero8BPPDataTo16BPPBufferTransparent( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );
BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncClipZSameZBurnsThrough( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZStripIndex );

BOOLEAN Blt8BPPDataTo16BPPBufferTransInvZ( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncClip(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows = FALSE);
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncClipAlpha(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, HVOBJECT hAlphaVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows = FALSE);
BOOLEAN Blt8BPPDataTo16BPPBufferTransZIncObscureClip(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion);
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClip(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows = FALSE);
BOOLEAN Blt8BPPDataTo16BPPBufferTransZTransShadowIncObscureClipAlpha(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, HVOBJECT hAlphaVObject, INT32 iX, INT32 iY, UINT16 usIndex, SGPRect *clipregion, INT16 sZIndex, UINT16 *p16BPPPalette, BOOLEAN fIgnoreShadows = FALSE);

#endif