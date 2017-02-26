#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "handle UI.h"
#include "mousesystem.h"
#include "structure.h"
#include "Assignments.h"		// added by Flugente for the stat-enums

#define		MAX_UICOMPOSITES				4

enum
{
	WALK_ICON,
	SNEAK_ICON,
	RUN_ICON,
	CRAWL_ICON,
	LOOK_ICON,
	ACTIONC_ICON,
	TALK_ICON,
	HAND_ICON,

	OPEN_DOOR_ICON,
	EXAMINE_DOOR_ICON,
	LOCKPICK_DOOR_ICON,
	BOOT_DOOR_ICON,
	UNTRAP_DOOR_ICON,
	USE_KEY_ICON,
	USE_KEYRING_ICON,
	EXPLOSIVE_DOOR_ICON,
	USE_CROWBAR_ICON,

	CANCEL_ICON,
	NUM_ICONS
};

#define	ACTIVATE_BUTTON		0
#define	DEACTIVATE_BUTTON	1
#define	ENABLE_BUTTON		2
#define	DISABLE_BUTTON		3
#define	UNLOAD_BUTTON		4

#define MAX_ENEMY_NAMES_CHARS		128

typedef struct
{
	UINT16		uiIndex; // add
	BOOLEAN		Hidden; //add

} HIDDEN_NAMES_VALUES;

extern HIDDEN_NAMES_VALUES zHiddenNames[500];

typedef struct
{
	UINT16		uiIndex; 
	BOOLEAN		Enabled; 
	UINT16 		ExpLevel; 
	UINT16 		Stats; 
	UINT16		StatsMin;
	UINT16		StatsMax;
	CHAR16	szCurRank[MAX_ENEMY_NAMES_CHARS];

} ENEMY_RANK_VALUES;

extern ENEMY_RANK_VALUES zEnemyRank[20];

typedef struct
{
	UINT16		uiIndex; // add
	BOOLEAN		Enabled; //add
	UINT16 		SectorX; //add
	UINT16 		SectorY; //add
	CHAR16	szCurGroup[MAX_ENEMY_NAMES_CHARS];

} ENEMY_NAMES_VALUES;

extern ENEMY_NAMES_VALUES zEnemyName[500];

typedef struct
{
	UINT16		uiIndex; // add
	BOOLEAN		Enabled; //add
	BOOLEAN Loyalty;
	BOOLEAN AddToBattle;
	BOOLEAN		fCanBeCaptured;		// Flugente: if TRUE, members of this faction can be captured
	CHAR16	szCurGroup[MAX_ENEMY_NAMES_CHARS];

} CIV_NAMES_VALUES;

extern CIV_NAMES_VALUES zCivGroupName[NUM_CIV_GROUPS];

// Flugente: soldier profiles
typedef struct
{
	UINT16		uiIndex;
	CHAR16		szName[MAX_ENEMY_NAMES_CHARS];
	UINT8		uiBodyType;
	UINT8		uiHair;
	UINT8		uiSkin;
	UINT8		uiTrait[3];	
	UINT32		uiFlags;
} SOLDIER_PROFILE_VALUES;

#define NUM_SOLDIER_PROFILES 300

extern SOLDIER_PROFILE_VALUES zSoldierProfile[6][NUM_SOLDIER_PROFILES];

extern UINT16 num_found_soldier_profiles[6];	// the correct number is set on reading the xml
extern UINT16 num_found_profiles;		// a helper variable during reading xmls

// Flugente: backgrounds
// enums for backgrounds
enum {
	// sector dependent AP modifiers
	BG_POLAR,		// not used ingame (yet?)
	BG_DESERT,
	BG_SWAMP,
	BG_URBAN,
	BG_FOREST,
	BG_PLAINS,
	BG_RIVER,
	BG_TROPICAL,
	BG_COASTAL,
	BG_MOUNTAIN,

	// stat effectiveness modifiers
	BG_AGILITY,
	BG_DEXTERITY,
	BG_STRENGTH,
	BG_LEADERSHIP,
	BG_MARKSMANSHIP,
	BG_MECHANICAL,
	BG_EXPLOSIVE_ASSIGN,
	BG_MEDICAL,
	BG_WISDOM,

	// ap modifiers for non-sector related tasks
	BG_HEIGHT,
	BG_SWIMMING,
	BG_FORTIFY,
	BG_ARTILLERY,
	BG_INVENTORY,
	BG_AIRDROP,
	BG_ASSAULT,

	// travel speed modifiers
	BG_TRAVEL_FOOT,
	BG_TRAVEL_CAR,
	BG_TRAVEL_AIR,
	BG_TRAVEL_BOAT,

	// resistances
	BG_RESI_FEAR,
	BG_RESI_SUPPRESSION,
	BG_RESI_PHYSICAL,
	BG_RESI_ALCOHOL,
	BG_RESI_DISEASE,

	// various
	BG_PERC_INTERROGATION,
	BG_PERC_GUARD,
	BG_PERC_PRICES_GUNS,
	BG_PERC_PRICES,
	BG_PERC_CAPITULATION,
	BG_PERC_SPEED_RUNNING,
	BG_PERC_BANDAGING,
	BG_PERC_REGEN_ENERGY,
	BG_PERC_CARRYSTRENGTH,
	BG_PERC_FOOD,
	BG_PERC_WATER,
	BG_PERC_SLEEP,
	BG_PERC_DAMAGE_MELEE,
	BG_PERC_CTH_BLADE,
	BG_PERC_CAMO,
	BG_PERC_STEALTH,
	BG_PERC_CTH_MAX,
	BG_PERC_HEARING_NIGHT,
	BG_PERC_HEARING_DAY,
	BG_PERC_DISARM,
	BG_PERC_SAM_CTH,

	// approaches
	BG_PERC_APPROACH_FRIENDLY,
	BG_PERC_APPROACH_DIRECT,
	BG_PERC_APPROACH_THREATEN,
	BG_PERC_APPROACH_RECRUIT,

	// various
	BG_BONUS_BREACHINGCHARGE,
	BG_PERC_CTH_CREATURE,
	BG_PERC_INSURANCE,
	BG_PERC_SPOTTER,
	BG_PERC_DISEASE_DIAGNOSE,
	BG_PERC_DISEASE_TREAT,
	BG_TRACKER_ABILITY,
	BG_AMBUSH_RADIUS,
	BG_SNAKEDEFENSE,
	
	BG_DISLIKEBG,			// dislike any other background that has the negative of this value set
	BG_SMOKERTYPE,			// 0: doesnt care about smoking 1: will consume cigarettes, dislikes non-smokers 2: will refuse to smoke, dislikes smokers
	BG_CROUCHEDDEFENSE,		// lowers enemy cth if they fire at us while we are crouched against cover in the direction the shots come from
	BG_FORTIFY_ASSIGNMENT,	// modifies effectivity of 'FORTIFICATION' assignment
	BG_HACKERSKILL,			// hacking skill from 0 to 100, > 0 means hacking is possible

	BG_MAX,
};

typedef struct
{
	UINT16		uiIndex;
	CHAR16		szName[MAX_ENEMY_NAMES_CHARS];	// name of a background etc.
	CHAR16		szShortName[20];				// abbreviated name for laptop display
	CHAR16		szDescription[256];				// description of background, should explain the abilities

	UINT64		uiFlags;						// this flagmask defines what special properties this background has (on/off behaviour)	
	INT16		value[BG_MAX];					// property values
} BACKGROUND_VALUES;

#define NUM_BACKGROUND 500

extern BACKGROUND_VALUES zBackground[NUM_BACKGROUND];

extern UINT16 num_found_background;		// the correct number is set on reading the xml

// anv: enums for externalised taunts
enum TAUNTPROPERTY{

	// progress
	TAUNT_PROGRESS_LT,
	TAUNT_PROGRESS_GT,

	// taunting person's stats
	TAUNT_EXP_LEVEL_LT,
	TAUNT_EXP_LEVEL_GT,
	TAUNT_HEALTH_LT,
	TAUNT_HEALTH_GT,
	TAUNT_HEALTH_MAX_LT,
	TAUNT_HEALTH_MAX_GT,
	TAUNT_ENERGY_LT,
	TAUNT_ENERGY_GT,
	TAUNT_ENERGY_MAX_LT,
	TAUNT_ENERGY_MAX_GT,
	TAUNT_MORALE_LT,
	TAUNT_MORALE_GT,

	// enemy/milita profiles
	TAUNT_PROFILE_ADMIN,
	TAUNT_PROFILE_ARMY,
	TAUNT_PROFILE_ELITE,
	TAUNT_PROFILE_GREEN,
	TAUNT_PROFILE_REGULAR,
	TAUNT_PROFILE_VETERAN,

	// facts
	TAUNT_FACT_TRUE,
	TAUNT_FACT_FALSE,

	// taunt target's stats
	TAUNT_TARGET_TYPE,
	TAUNT_TARGET_EXP_LEVEL_LT,
	TAUNT_TARGET_EXP_LEVEL_GT,
	TAUNT_TARGET_HEALTH_LT,
	TAUNT_TARGET_HEALTH_GT,
	TAUNT_TARGET_HEALTH_MAX_LT,
	TAUNT_TARGET_HEALTH_MAX_GT,
	TAUNT_TARGET_ENERGY_LT,
	TAUNT_TARGET_ENERGY_GT,
	TAUNT_TARGET_ENERGY_MAX_LT,
	TAUNT_TARGET_ENERGY_MAX_GT,
	TAUNT_TARGET_MORALE_LT,
	TAUNT_TARGET_MORALE_GT,
	TAUNT_TARGET_APPEARANCE,

	// taunt target's profile
	TAUNT_TARGET_MERC_PROFILE,

	// which merc quote should be riposted
	TAUNT_RIPOSTE_QUOTE,

	TAUNT_MAX,
};

typedef struct
{
	UINT16		uiIndex;
	CHAR16		szText[320];
	CHAR16		szCensoredText[320];
	UINT64		uiFlags;
	UINT64		uiFlags2;
	INT16		value[TAUNT_MAX];

} TAUNT_VALUES;

#define NUM_TAUNT 2048

extern TAUNT_VALUES zTaunt[NUM_TAUNT];

extern UINT16 num_found_taunt;		// the correct number is set on reading the xml

extern		int INTERFACE_WIDTH;		//			640
extern		int INTERFACE_HEIGHT;		//			120
extern		int INV_INTERFACE_HEIGHT;	//			140

extern		int INTERFACE_START_X;
extern		int INTERFACE_START_Y;		//			( SCREEN_HEIGHT - INTERFACE_HEIGHT )
extern		int INV_INTERFACE_START_Y;	//			( SCREEN_HEIGHT - INV_INTERFACE_HEIGHT )
// FLASH PORTRAIT CODES
#define		FLASH_PORTRAIT_STOP				0
#define		FLASH_PORTRAIT_START			1
#define		FLASH_PORTRAIT_WAITING			2
#define		FLASH_PORTRAIT_DELAY			150

// FLASH PORTRAIT PALETTE IDS
#define		FLASH_PORTRAIT_NOSHADE			0	
#define		FLASH_PORTRAIT_STARTSHADE		1
#define		FLASH_PORTRAIT_ENDSHADE			2
#define		FLASH_PORTRAIT_DARKSHADE		3
#define		FLASH_PORTRAIT_GRAYSHADE		4
#define		FLASH_PORTRAIT_LITESHADE		5


// GLOBAL DEFINES FOR SOME UI FLAGS
#define		ARROWS_HIDE_UP					0x00000002
#define		ARROWS_HIDE_DOWN				0x00000004
#define		ARROWS_SHOW_UP_BESIDE			0x00000008
#define		ARROWS_SHOW_DOWN_BESIDE			0x00000020
#define		ARROWS_SHOW_UP_ABOVE_Y			0x00000040
#define		ARROWS_SHOW_DOWN_BELOW_Y		0x00000080
#define		ARROWS_SHOW_DOWN_BELOW_G		0x00000200
#define		ARROWS_SHOW_DOWN_BELOW_YG		0x00000400
#define		ARROWS_SHOW_DOWN_BELOW_GG		0x00000800
#define		ARROWS_SHOW_UP_ABOVE_G			0x00002000
#define		ARROWS_SHOW_UP_ABOVE_YG			0x00004000
#define		ARROWS_SHOW_UP_ABOVE_GG			0x00008000
#define		ARROWS_SHOW_UP_ABOVE_YY			0x00020000
#define		ARROWS_SHOW_DOWN_BELOW_YY		0x00040000
#define		ARROWS_SHOW_UP_ABOVE_CLIMB		0x00080000
#define		ARROWS_SHOW_UP_ABOVE_CLIMB2		0x00400000
#define		ARROWS_SHOW_UP_ABOVE_CLIMB3		0x00800000
#define		ARROWS_SHOW_DOWN_CLIMB			0x02000000

#define		ROOF_LEVEL_HEIGHT				50

#define		LOCATEANDSELECT_MERC			1
#define		LOCATE_MERC_ONCE				2


// Interface level enums
enum
{
	I_GROUND_LEVEL,
	I_ROOF_LEVEL,
	I_NUMLEVELS
};

#define		DRAW_RED_BAR		1
#define		DRAW_BLUE_BAR		2
#define		DRAW_ERASE_BAR	3


extern BOOLEAN		gfSwitchPanel;
extern BOOLEAN		gfUIStanceDifferent;
extern UINT8		gbNewPanel;
extern UINT8		gubNewPanelParam;
extern INT16		gsCurInterfacePanel;

extern UINT32		guiRENDERBUFFER;
extern UINT32		guiCLOSE;
extern UINT32		guiDEAD;
extern UINT32		guiHATCH;
extern UINT32		guiINTEXT;

extern UINT32		guiSILHOUETTE;
extern UINT32		guiCOMPANEL;
extern UINT32		guiCOMPANELB;
extern UINT32		guiRADIO;
extern UINT32		guiPORTRAITICONS;		// Default JA2 portrait icons (+ additional headgear icons from legion)
extern UINT32		guiASSIGNMENTICONS;		// Flugente: icons for assignments
extern UINT32		guiBURSTACCUM;
extern UINT32		guiITEMPOINTERHATCHES;


//legion
//extern UINT32		guiPORTRAITICONS_NV;			//legion: NV-Face Graphic
//extern UINT32		guiPORTRAITICONS_GAS_MASK;		//legion: GASMASK-Face Graphic

// WANNE: Face gear for IMPs in additional file
//extern UINT32		guiPORTRAITICONS_NV_IMP;
//extern UINT32		guiPORTRAITICONS_GAS_MASK_IMP;

extern MOUSE_REGION		gViewportRegion;
extern MOUSE_REGION		gRadarRegion;
extern MOUSE_REGION	gBottomPanalRegion;

#define				MOVEMENT_MENU_LOOK			1
#define				MOVEMENT_MENU_ACTIONC		2
#define				MOVEMENT_MENU_HAND			3
#define				MOVEMENT_MENU_TALK			4
#define				MOVEMENT_MENU_RUN			5
#define				MOVEMENT_MENU_WALK			6
#define				MOVEMENT_MENU_SWAT			7
#define				MOVEMENT_MENU_PRONE			8


#define				DIRTYLEVEL0					0
#define				DIRTYLEVEL1					1
#define				DIRTYLEVEL2					2


typedef enum
{
	SM_PANEL,
	TEAM_PANEL,
	NUM_UI_PANELS

} InterfacePanelDefines;

BOOLEAN InitializeTacticalPortraits( );
BOOLEAN InitializeFaceGearGraphics( );
BOOLEAN InitializeTacticalInterface( );
BOOLEAN ShutdownTacticalInterface( );
extern BOOLEAN	fInterfacePanelDirty;
extern BOOLEAN	gfPausedTacticalRenderFlags;
extern BOOLEAN	gfPausedTacticalRenderInterfaceFlags;
extern INT16	gsInterfaceLevel;
extern BOOLEAN	gfInMovementMenu;



void PopupPositionMenu( UI_EVENT *pUIEvent );
void PopDownPositionMenu( );
void PopupMovementMenu( UI_EVENT *pUIEvent );
void PopDownMovementMenu( );
void RenderMovementMenu( );
void CancelMovementMenu( );


void PopDownOpenDoorMenu( );
void RenderOpenDoorMenu( );
BOOLEAN InitDoorOpenMenu( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection, BOOLEAN fClosingDoor );
BOOLEAN HandleOpenDoorMenu( );
void CancelOpenDoorMenu( );

void HandleInterfaceBackgrounds( );

void BeginOverlayMessage( UINT32 uiFont, STR16 pFontString, ... );
void EndOverlayMessage( );

void DrawSelectedUIAboveGuy( UINT16 usSoldierID );

// HEADROCK HAM 4: Draw the CTH indicator, using values grabbed from the global CTH array
BOOLEAN DrawCTHIndicator();

void CreateCurrentTacticalPanelButtons( );
void RemoveCurrentTacticalPanelButtons( );
void SetCurrentTacticalPanelCurrentMerc( UINT8 ubID );
void SetCurrentInterfacePanel( UINT8 ubNewPanel );
BOOLEAN IsMercPortraitVisible( UINT8 ubSoldierID );

BOOLEAN InitializeCurrentPanel( );
void ShutdownCurrentPanel( );

void ClearInterface( );
void RestoreInterface( );

void RenderArrows( );
void EraseRenderArrows( );
void GetArrowsBackground( );
void HandleUpDownArrowBackgrounds( );

void EndDeadlockMsg( );

void HandleLocateSelectMerc( UINT8 ubID, INT8 bFlag );

void DirtyMercPanelInterface( SOLDIERTYPE *pSoldier, UINT8 ubDirtyLevel );


void EndUIMessage( );
void BeginUIMessage( STR16 pFontString, ... );
void InternalBeginUIMessage( BOOLEAN fUseSkullIcon, STR16 pFontString, ... );


// map screen version, for centering over the map area
void BeginMapUIMessage( UINT8 fPosition, STR16 pFontString, ... );


extern UINT16	gusUIOldSelectedSoldier;

extern INT32	giUIMessageOverlay;
extern UINT32	guiUIMessageTime;

enum MESSAGE_TYPES
{
	NO_MESSAGE,
	COMPUTER_TURN_MESSAGE,
	COMPUTER_INTERRUPT_MESSAGE,
	PLAYER_INTERRUPT_MESSAGE,
	MILITIA_INTERRUPT_MESSAGE,
	AIR_RAID_TURN_MESSAGE,
	PLAYER_TURN_MESSAGE,

} ;

void HandleTopMessages( );

BOOLEAN AddTopMessage( UINT8 ubType, STR16 pzString );

BOOLEAN InTopMessageBarAnimation( );
void EndTopMessage( );

void PauseRT( BOOLEAN fPause );

void InitEnemyUIBar( UINT8 ubNumEnemies, UINT8 ubDoneEnemies );

STR16 GetSoldierHealthString( SOLDIERTYPE *pSoldier );


void GetLaunchItemParamsFromUI( );
void RenderAimCubeUI( );
void ResetAimCubeAI( );
void SetupAimCubeAI( );
void IncrementAimCubeUI( );
void EndAimCubeUI( );
void BeginAimCubeUI( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 ubLevel, UINT8 bStartPower, INT8 bStartHeight );
BOOLEAN AimCubeUIClick( );

void ResetPhysicsTrajectoryUI( );
void SetupPhysicsTrajectoryUI( );
void EndPhysicsTrajectoryUI( );
void BeginPhysicsTrajectoryUI( INT32 sGridNo, INT8 bLevel, BOOLEAN fBadCTGT );

void InitPlayerUIBar( BOOLEAN fInterrupt );

void ToggleTacticalPanels( );

void DirtyTopMessage( );

void BeginMultiPurposeLocator( INT32 sGridNo, INT8 bLevel, BOOLEAN fSlideTo );
void HandleMultiPurposeLocator( );
void RenderTopmostMultiPurposeLocator( );

// sevenfm: draw lines in health bar
void DrawBar( INT32 x, INT32 y, INT32 width, INT32 height, UINT16 color8, UINT16 color16, UINT8 *pDestBuf );
// draw health bar over enemy
void DrawEnemyHealthBar( SOLDIERTYPE* pSoldier, INT32 sX, INT32 sY, UINT8 ubLines, INT32 iBarWidth, INT32 iBarHeight );

// Flugente: show enemy role
BOOLEAN ShowSoldierRoleSymbol(SOLDIERTYPE* pSoldier);

//sevenfm: draw additional info for NCTH indicator
void DrawItemPic( INVTYPE *pItem, INT16 sX, INT16 sY );
void GetItemDimensions( INVTYPE *pItem, INT16 &sWidth, INT16 &sHeight );
BOOLEAN ShowExactInfo( SOLDIERTYPE* pSoldier, SOLDIERTYPE* pTargetSoldier );
void DrawNCTHCursorItemPics( INT16 sStartScreenX, INT16 sStartScreenY  );

void GetEnemyInfoString( SOLDIERTYPE* pSelectedSoldier, SOLDIERTYPE* pTargetSoldier, BOOLEAN showExactInfo, CHAR16 *NameStr );

void ShowEnemyWeapon( INT16 sX, INT16 sY, SOLDIERTYPE* pTargetSoldier );
void ShowEnemyHealthBar( INT16 sX, INT16 sY, SOLDIERTYPE* pSoldier );

void NCTHImprovedAPColor( SOLDIERTYPE* pSoldier, OBJECTTYPE* pWeapon );
void NCTHDrawLaserDot( UINT16* ptrBuf, UINT32 uiPitch, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, INT16 sStartScreenX, INT16 sStartScreenY, FLOAT fLaserBonus, FLOAT fBrightnessModifier, FLOAT fEffectiveLaserRatio, FLOAT zOffset );
void NCTHCorrectMaxAperture( FLOAT& iAperture, FLOAT& iDistanceAperture, UINT16& usCApertureBar );
void NCTHDrawScopeModeIcon( SOLDIERTYPE *pSoldier, INT16 sNewX, INT16 sNewY );
void NCTHShowAimLevels( SOLDIERTYPE* pSoldier, INT16 curX, INT16 curY );
void NCTHShowMounted( SOLDIERTYPE* pSoldier, UINT16* ptrBuf, UINT32 uiPitch, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, INT16 sStartScreenX, INT16 sStartScreenY, INT16 zOffset );

// Flugente: check a profile for a background flag without using SOLDIERTYPE
BOOLEAN		HasBackgroundFlag( UINT8 usProfile, UINT64 aFlag );
INT16		GetBackgroundValue( UINT8 usProfile, UINT16 aNr );

#endif