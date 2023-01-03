#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef EDITSCREEN_H
#define EDITSCREEN_H

#include "Button System.h"

#define EDITOR_LIGHT_MAX		(SHADE_MIN + SHADE_MAX)
#define EDITOR_LIGHT_FAKE		(EDITOR_LIGHT_MAX - SHADE_MAX - 2)

extern BOOLEAN gfFakeLights;
extern GUI_BUTTON *gpPersistantButton;

extern UINT16 GenericButtonFillColors[40];

// WANNE: 05.10.2010
extern BOOLEAN fNewMapSaved;

void DisplayWayPoints(void);

BOOLEAN CheckForSlantRoofs( void );
BOOLEAN CheckForFences( void );

void ShowLightPositionHandles( void );
void RemoveLightPositionHandles( void );

extern BOOLEAN DoWindowSelection( void );
extern void RemoveTempMouseCursorObject(void);
extern BOOLEAN DrawTempMouseCursorObject(void);
extern void ShowCurrentDrawingMode( void );
extern void ShowCurrentSlotImage( HVOBJECT hVObj, INT32 iWindow );
extern void ShowCurrentSlotSurface( UINT32 hSurface, INT32 iWindow );

void CreateNewMap();

void DisplayTilesetPage(void);
void FindTilesetComments(void);
void GetMasterList(void);

void HandleJA2ToolbarSelection( void );
void HandleKeyboardShortcuts(	);
UINT32 PerformSelectedAction( void );
UINT32 WaitForSelectionWindowResponse( void );
UINT32 WaitForMessageBoxResponse( UINT32 uiCurrentScreen );
UINT32 WaitForHelpScreenResponse( void );

//dnl ch86 210214
BOOLEAN FindLight(INT32 iGridNo, INT8 bLightType, UINT8 *pubLightRadius, UINT8 *pubLightId, INT32 *piSprite=NULL);
BOOLEAN PlaceLight(INT16 sRadius, INT16 iMapX, INT16 iMapY, INT16 sType, INT8 bLightType);
BOOLEAN RemoveLight(INT16 iMapX, INT16 iMapY, INT8 bLightType);

extern BOOLEAN gfMercResetUponEditorEntry;

void ShowEditorToolbar( INT32 iNewTaskMode );
void HideEditorToolbar( INT32 iOldTaskMode );

void ProcessSelectionArea();

void MapOptimize(void);

extern UINT16 GenericButtonFillColors[40];

//These go together.	The taskbar has a specific color scheme.
extern UINT16 gusEditorTaskbarColor;
extern UINT16 gusEditorTaskbarHiColor;
extern UINT16 gusEditorTaskbarLoColor;

extern INT32 iOldTaskMode;
extern INT32 iCurrentTaskbar;
extern INT32 iTaskMode;

void ProcessAreaSelection( BOOLEAN fWithLeftButton );

void ShowEntryPoints();
void HideEntryPoints();

extern BOOLEAN			gfConfirmExitFirst;
extern BOOLEAN			gfConfirmExitPending;
extern BOOLEAN			gfIntendOnEnteringEditor;

extern BOOLEAN gfResizeMapOnLoading;
extern BOOLEAN gfVanillaMode;//dnl ch33 160909
extern BOOLEAN fRaiseWorld;//dnl ch3 210909
extern BOOLEAN fShowHighGround;//dnl ch2 210909
void ShowHighGround(INT32 iShowHighGroundCommand);//dnl ch2 210909

extern BOOLEAN fEditorCreateItemFromKeyboard;
extern UINT16 usEditorTempItem;
extern UINT16 gusSavedLightLevel;//dnl ch80 011213

#endif
#endif



 


