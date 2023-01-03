#ifndef _HANDLE_UI_
#define _HANDLE_UI_

#include "input.h"
#include "Soldier Control.h"

// DEFINES
#define	UIEVENT_SINGLEEVENT			0x00000002
#define	UIEVENT_SNAPMOUSE			0x00000008

#define	NO_GUY_SELECTION			0
#define SELECTED_GUY_SELECTION		1
#define NONSELECTED_GUY_SELECTION	2
#define ENEMY_GUY_SELECTION			3

#define MOUSE_MOVING_IN_TILE		0x00000001
#define MOUSE_MOVING				0x00000002
#define MOUSE_MOVING_NEW_TILE		0x00000004
#define MOUSE_STATIONARY			0x00000008

#define	MOVEUI_TARGET_INTTILES		1
#define	MOVEUI_TARGET_ITEMS			2
#define	MOVEUI_TARGET_MERCS			3
#define	MOVEUI_TARGET_MERCSFORAID	5
#define	MOVEUI_TARGET_WIREFENCE		6
#define	MOVEUI_TARGET_BOMB			7
#define	MOVEUI_TARGET_STEAL			8
#define	MOVEUI_TARGET_REPAIR		9
#define	MOVEUI_TARGET_JAR			10
#define	MOVEUI_TARGET_CAN			11
#define MOVEUI_TARGET_REFUEL		12
#define MOVEUI_TARGET_FORTIFICATION	13
#define MOVEUI_TARGET_HANDCUFF		14
#define MOVEUI_TARGET_APPLYITEM		15
#define MOVEUI_TARGET_INTERACTIVEACTION	16
#define MOVEUI_TARGET_BLOODBAG		17
#define MOVEUI_TARGET_CAMERA		18

#define	MOVEUI_RETURN_ON_TARGET_MERC	1

typedef enum
{
	DONT_CHANGEMODE,
	IDLE_MODE,
	MOVE_MODE,
	ACTION_MODE,
	MENU_MODE,
	POPUP_MODE,
	CONFIRM_MOVE_MODE,
	ADJUST_STANCE_MODE,
	CONFIRM_ACTION_MODE,
	HANDCURSOR_MODE,
	GETTINGITEM_MODE,
	ENEMYS_TURN_MODE,
	LOOKCURSOR_MODE,
	TALKINGMENU_MODE,
	TALKCURSOR_MODE,
	LOCKUI_MODE,
	OPENDOOR_MENU_MODE,
	LOCKOURTURN_UI_MODE,
	EXITSECTORMENU_MODE,
	RUBBERBAND_MODE,
	JUMPOVER_MODE,
	RADIOCURSOR_MODE,

}	UI_MODE;

typedef UINT32 (*UI_HANDLEFNC)( struct TAG_UI_EVENT* );

typedef struct TAG_UI_EVENT
{
	UINT32					uiFlags;
	UI_MODE					ChangeToUIMode;
	UI_HANDLEFNC		HandleEvent;
	BOOLEAN					fFirstTime;
	BOOLEAN					fDoneMenu;
	UINT32					uiMenuPreviousMode;
	UINT32					uiParams[3];

} UI_EVENT;


// EVENT ENUMERATION
typedef enum
{
	I_DO_NOTHING,
	I_EXIT,
	I_NEW_MERC,
	I_NEW_BADMERC,
	I_SELECT_MERC,
	I_ENTER_EDIT_MODE,
	I_ENTER_PALEDIT_MODE,
	I_ENDTURN,
	I_TESTHIT,
	I_CHANGELEVEL,
	I_ON_TERRAIN,
	I_CHANGE_TO_IDLE,
	I_LOADLEVEL,
	I_SOLDIERDEBUG,
	I_LOSDEBUG,
	I_LEVELNODEDEBUG,
	I_GOTODEMOMODE,
	I_LOADFIRSTLEVEL,
	I_LOADSECONDLEVEL,
	I_LOADTHIRDLEVEL,
	I_LOADFOURTHLEVEL,
	I_LOADFIFTHLEVEL,

	ET_ON_TERRAIN,
	ET_ENDENEMYS_TURN,

	M_ON_TERRAIN,
	M_CHANGE_TO_ACTION,
	M_CHANGE_TO_HANDMODE,
	M_CYCLE_MOVEMENT,
	M_CYCLE_MOVE_ALL,
	M_CHANGE_TO_ADJPOS_MODE,

	POPUP_DOMESSAGE,

	A_ON_TERRAIN,
	A_CHANGE_TO_MOVE,
	A_CHANGE_TO_CONFIM_ACTION,
	A_END_ACTION,
	U_MOVEMENT_MENU,
	U_POSITION_MENU,

	C_WAIT_FOR_CONFIRM,
	C_MOVE_MERC,
	C_ON_TERRAIN,

	PADJ_ADJUST_STANCE,

	CA_ON_TERRAIN,
	CA_MERC_SHOOT,
	CA_END_CONFIRM_ACTION,

	HC_ON_TERRAIN,

	G_GETTINGITEM,	

	LC_ON_TERRAIN,
	LC_CHANGE_TO_LOOK,
	LC_LOOK,

	TA_TALKINGMENU,	

	T_ON_TERRAIN,
	T_CHANGE_TO_TALKING,

	LU_ON_TERRAIN,
	LU_BEGINUILOCK,
	LU_ENDUILOCK,

	OP_OPENDOORMENU,

	LA_ON_TERRAIN,
	LA_BEGINUIOURTURNLOCK,
	LA_ENDUIOUTURNLOCK,

	EX_EXITSECTORMENU,

	RB_ON_TERRAIN,

	JP_ON_TERRAIN,
	JP_JUMP,

	R_CHANGE_TO_RADIO,

	NUM_UI_EVENTS

} UI_EVENT_DEFINES;

typedef BOOLEAN (*UIKEYBOARD_HOOK)( InputAtom *pInputEvent );	


// GLOBAL EVENT STRUCT
extern UI_EVENT gEvents[ NUM_UI_EVENTS ];

// GLOBAL STATUS VARS
extern UI_MODE		gCurrentUIMode;
extern UI_MODE		gOldUIMode;
extern UINT32		guiCurrentEvent;
extern INT16		gsSelectedLevel;
extern BOOLEAN		gfPlotNewMovement;
extern UINT32		guiPendingOverrideEvent;


// GLOBALS
extern BOOLEAN		gfUIDisplayActionPoints;
extern BOOLEAN		gfUIDisplayActionPointsInvalid;
extern BOOLEAN		gfUIDisplayActionPointsBlack;
extern BOOLEAN		gfUIDisplayActionPointsCenter;
extern INT16		gUIDisplayActionPointsOffY;
extern INT16		gUIDisplayActionPointsOffX;
extern BOOLEAN		gfUIDoNotHighlightSelMerc;
extern UINT32		guiShowUPDownArrows;
extern BOOLEAN		gfUIHandleSelection;
extern BOOLEAN		gfUIHandleSelectionAboveGuy;
extern INT32		gsSelectedGridNo;
extern INT16		gsSelectedGuy;
extern BOOLEAN		gfUIInDeadlock;
extern UINT8		gUIDeadlockedSoldier;

extern BOOLEAN		gfUIMouseOnValidCatcher;
extern UINT8		gubUIValidCatcherID;
extern BOOLEAN		gUIUseReverse;


extern BOOLEAN		gfUIHandleShowMoveGrid;
extern INT32		gsUIHandleShowMoveGridLocation;

extern BOOLEAN		gfUIDisplayDamage;
extern INT8			gbDamage;
extern INT32		gsDamageGridNo;

extern BOOLEAN		gfFontPopupDo;

extern BOOLEAN		gUITargetReady;
extern BOOLEAN		gUITargetShotWaiting;
extern INT32		gsUITargetShotGridNo;

extern CHAR16		gzLocation[ 20 ];
extern BOOLEAN		gfUIBodyHitLocation;

extern UINT16		gsBulletCount;
extern UINT16		gsTotalBulletCount;
extern BOOLEAN		gTintBulletCounts;
extern BOOLEAN		gfUIAutofireBulletCount;

extern BOOLEAN		gfUICtHBar;
// HEADROCK HAM 4: Now a STRUCT containing several variables for CTH display
typedef struct
{
	UINT8 MuzzleSwayPercentage;
	FLOAT ScopeMagFactor;
	FLOAT ProjectionFactor;
	INT16 iBestLaserRange;
	FLOAT FinalMagFactor;
	INT32 iShooterGridNo;
	INT32 iTargetGridNo;
	FLOAT dTargetZ;				// CellZ of target (not tile coordinates!)
	BOOLEAN	fMaxAimReached;
} CTHDISPLAY;

extern CTHDISPLAY gCTHDisplay;
// HEADROCK HAM B1/2: Multiple-bullet CTH bar array (replaces old single integer)
extern UINT8		gbCtH[ 10 ];
extern UINT8		gbCtHBurstCount;
// HEADROCK HAM B2: Track autofire on/off
extern BOOLEAN		gbCtHAutoFire;

extern CHAR16		gzIntTileLocation[ 20 ];
extern BOOLEAN		gfUIIntTileLocation;

extern CHAR16		gzIntTileLocation2[ 20 ];
extern BOOLEAN		gfUIIntTileLocation2;

extern BOOLEAN		gfUIWaitingForUserSpeechAdvance;
extern BOOLEAN		gfUIKeyCheatModeOn;

extern BOOLEAN		gfUIAllMoveOn;
extern BOOLEAN		gfUICanBeginAllMoveCycle;

extern BOOLEAN		gfUIRefreshArrows;

extern BOOLEAN		gfUIHandlePhysicsTrajectory;


// GLOBALS FOR FAST LOOKUP FOR FINDING MERCS FROM THE MOUSE
extern BOOLEAN	gfUISelectiveTargetFound;
extern UINT16	gusUISelectiveTargetID;
extern UINT32	guiUISelectiveTargetFlags;

extern BOOLEAN	gfUIFullTargetFound;
extern UINT16	gusUIFullTargetID;
extern UINT32	guiUIFullTargetFlags;

extern BOOLEAN	gfUIConfirmExitArrows;
extern INT32	gsJumpOverGridNo;

extern UINT32	HandleTacticalUI(void);
extern UINT32	UIHandleEndTurn( UI_EVENT *pUIEvent );

extern BOOLEAN	gfUIShowCurIntTile;

extern SGPRect	gRubberBandRect;
extern BOOLEAN	gRubberBandActive;

void EndMenuEvent( UINT32	uiEvent );
void SetUIKeyboardHook( UIKEYBOARD_HOOK KeyboardHookFnc );
void HandleObjectHighlighting( );

extern BOOLEAN	gfUIForceReExamineCursorData;

extern INT16	guiCreateGuyIndex;
extern INT16	guiCreateBadGuyIndex;

// WANNE: Calculate the APs to turn around
INT16 APsToTurnAround(SOLDIERTYPE *pSoldier, INT32 sAdjustedGridNo);

// FUNCTIONS IN INPUT MODULES
void GetKeyboardInput( UINT32 *puiNewEvent );
void GetPolledKeyboardInput( UINT32 *puiNewEvent );

void GetTBMouseButtonInput( UINT32 *puiNewEvent );
void GetTBMousePositionInput( UINT32 *puiNewEvent );
void QueryTBLeftButton( UINT32 *puiNewEvent );
void QueryTBRightButton( UINT32 *puiNewEvent );
void HandleStanceChangeFromUIKeys( UINT8 ubAnimHeight );
void HandleKeyInputOnEnemyTurn(	);


void GetRTMouseButtonInput( UINT32 *puiNewEvent );
void GetRTMousePositionInput( UINT32 *puiNewEvent );
void QueryRTLeftButton( UINT32 *puiNewEvent );
void QueryRTRightButton( UINT32 *puiNewEvent );

void AdjustSoldierCreationStartValues( );

BOOLEAN SelectedMercCanAffordAttack( );
BOOLEAN SelectedMercCanAffordMove(	);
void GetMercClimbDirection( UINT8 ubSoldierID, BOOLEAN *pfGoDown, BOOLEAN *pfGoUp );

void ToggleHandCursorMode( UINT32 *puiNewEvent );
void ToggleTalkCursorMode( UINT32 *puiNewEvent );
void ToggleLookCursorMode( UINT32 *puiNewEvent );

void UIHandleSoldierStanceChange( UINT8 ubSoldierID, INT8	bNewStance );
void GetCursorMovementFlags( UINT32 *puiCursorFlags );

BOOLEAN HandleUIMovementCursor( SOLDIERTYPE *pSoldier, UINT32 uiCursorFlags, INT32 usMapPos, UINT32 uiFlags );
BOOLEAN UIMouseOnValidAttackLocation( SOLDIERTYPE *pSoldier );

BOOLEAN UIOkForItemPickup( SOLDIERTYPE *pSoldier, INT32 sGridNo );

BOOLEAN IsValidTalkableNPCFromMouse( UINT8 *pubSoldierID, BOOLEAN fGive, BOOLEAN fAllowMercs, BOOLEAN fCheckCollapsed );
BOOLEAN IsValidTalkableNPC( UINT8 ubSoldierID, BOOLEAN fGive, BOOLEAN fAllowMercs, BOOLEAN fCheckCollapsed );

BOOLEAN HandleTalkInit(	);

BOOLEAN HandleCheckForExitArrowsInput( BOOLEAN fAdjustForConfirm );

void SetUIBusy( UINT8 ubID );
void UnSetUIBusy( UINT8 ubID );

UINT32 UIHandleLUIEndLock( UI_EVENT *pUIEvent );

void BeginDisplayTimedCursor( UINT32 uiCursorID, UINT32 uiDelay );

void HandleHandCursorClick( INT32 usMapPos, UINT32 *puiNewEvent );
void HandleHandCursorRightClick( INT32 usMapPos, UINT32 *puiNewEvent );
void HandleRadioCursorClick(INT32 usMapPos, UINT32 *puiNewEvent);
INT8 HandleMoveModeInteractiveClick( INT32 usMapPos, UINT32 *puiNewEvent );

BOOLEAN HandleUIReloading( SOLDIERTYPE *pSoldier );

UINT32	UIHandleChangeLevel( UI_EVENT *pUIEvent );
BOOLEAN UIHandleOnMerc( BOOLEAN fMovementMode );

void ChangeInterfaceLevel( INT16 sLevel );

void EndRubberBanding( );
void ResetMultiSelection( );
void EndMultiSoldierSelection( BOOLEAN fAcknowledge );
BOOLEAN StopRubberBandedMercFromMoving(void);

BOOLEAN SelectedGuyInBusyAnimation( );

void GotoLowerStance( SOLDIERTYPE *pSoldier );
void GotoHeigherStance( SOLDIERTYPE *pSoldier );

BOOLEAN IsValidJumpLocation( SOLDIERTYPE *pSoldier, INT32 sGridNo , BOOLEAN fCheckForPath);

void PopupAssignmentMenuInTactical( SOLDIERTYPE *pSoldier );
void PopupMilitiaControlMenu( SOLDIERTYPE *pSoldier ); //lal

void PreventFromTheFreezingBug(SOLDIERTYPE* pSoldier);

void GetGridNoScreenXY( INT32 sGridNo, INT16 *pScreenX, INT16 *pScreenY );

//Legion by Jazz
void GetMercOknoDirection( UINT8 ubSoldierID, BOOLEAN *pfGoDown, BOOLEAN *pfGoUp );

#endif