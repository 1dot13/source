#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <time.h>
	#include "sgp.h"
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "sysutil.h"
	#include "overhead.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "interface.h"
	#include "vsurface.h"
	#include "wcheck.h"
	#include "input.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "renderworld.h"
	#include "sys globals.h"
	#include "cursors.h"
	#include "radar screen.h"
	#include "worldman.h"
	#include "Font Control.h"
	#include "render dirty.h"
	#include "utilities.h"
	#include "interface cursors.h"
	#include "Sound Control.h"
	#include "lighting.h"
	#include "Interface Panels.h"
	#include "pathai.h"
	#include "vobject_blitters.h"
	#include "faces.h"
	#include "Handle UI Plan.h"
	#include "interface control.h"
	#include "interface items.h"
	#include "soldier profile.h"
	#include "MercTextBox.h"
	#include "soldier functions.h"
	#include "cursor control.h"
	#include "handle doors.h"
	#include "keys.h"
	#include "text.h"
	#include "points.h"
	#include "soldier macros.h"
	#include "game clock.h"
	#include "physics.h"
	#include "Map Screen Interface Map.h"
	#include "line.h"
	#include "Vehicles.h"
	#include "messageboxscreen.h"
	#include "gamesettings.h"
	#include "Squads.h"
	#include "message.h"
    #include "strategicmap.h"
	#include "Queen Command.h"
	// HEADROCK HAM 4: Included for new CTH indicator
	#include "weapons.h"
	#include "Map Screen Interface.h"	// added by Flugente for SquadNames
	#include "environment.h"
	// sevenfm: needed for _KeyDown(SHIFT)
	#include "english.h"
	#include "DisplayCover.h"
	#include "SkillCheck.h"				// added by Flugente
#endif

#include "InterfaceItemImages.h"
#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "mercs.h"
#include "ub_config.h"
#endif

#include "connect.h"
//const UINT32 INTERFACE_START_X			= 0;
//const UINT32 INTERFACE_START_Y			= ( SCREEN_HEIGHT - INTERFACE_HEIGHT );
//const UINT32 INV_INTERFACE_START_Y		= ( SCREEN_HEIGHT - INV_INTERFACE_HEIGHT );

int INTERFACE_WIDTH;//			= 640;
int INTERFACE_HEIGHT;//		= 120;
int INV_INTERFACE_HEIGHT;//	= 140;

int INTERFACE_START_X;//		= 0;
int INTERFACE_START_Y;//		= ( SCREEN_HEIGHT - INTERFACE_HEIGHT );
int INV_INTERFACE_START_Y;//	= ( SCREEN_HEIGHT - INV_INTERFACE_HEIGHT );

#define ARROWS_X_OFFSET				10
#define	ARROWS_HEIGHT				20
#define	ARROWS_WIDTH				20
#define UPARROW_Y_OFFSET			-30
#define DOWNARROW_Y_OFFSET			-10

#define	BUTTON_PANEL_WIDTH			78
#define	BUTTON_PANEL_HEIGHT			76

HIDDEN_NAMES_VALUES zHiddenNames[500]; //legion2 Jazz
ENEMY_NAMES_VALUES zEnemyName[500];
ENEMY_RANK_VALUES zEnemyRank[20];				// Flugente: set this to 20, which should be way enough, as there are only 10 exp levels
CIV_NAMES_VALUES zCivGroupName[NUM_CIV_GROUPS];

// Flugente: soldier profiles
SOLDIER_PROFILE_VALUES zSoldierProfile[6][NUM_SOLDIER_PROFILES];
UINT16 num_found_soldier_profiles[6];	// the correct number is set on reading the xml

BACKGROUND_VALUES zBackground[NUM_BACKGROUND];				// Flugente: backgrounds

TAUNT_VALUES zTaunt[NUM_TAUNT];	// anv: externalised taunts

BOOLEAN	gfInMovementMenu = FALSE;
INT32		giMenuAnchorX, giMenuAnchorY;

//*ddd
//#define PROG_BAR_START_Y			2

//**ddd{ 
//assign 0 to define if using large progress bar
//#define fSmallSizeProgressbar 1
INT32 HEIGHT_PROGRESSBAR, PROG_BAR_START_Y;
//ddd}

extern BOOLEAN gfCannotGetThrough;	// sevenfm: will use it when showing red laser dot

BOOLEAN	gfProgBarActive		= FALSE;
UINT8		gubProgNumEnemies		= 0;
UINT8		gubProgCurEnemy			= 0;


UINT32		guiPORTRAITICONS;
UINT32		guiASSIGNMENTICONS;		// Flugente: icons for assignments

//UINT32		guiPORTRAITICONS_NV; //legion
//UINT32		guiPORTRAITICONS_GAS_MASK; //legion

// WANNE: Additional face gear file for IMPs
//UINT32		guiPORTRAITICONS_NV_IMP;
//UINT32		guiPORTRAITICONS_GAS_MASK_IMP;

typedef struct
{
	UINT32			uiSurface;
	INT8				bCurrentMessage;
	UINT32			uiTimeOfLastUpdate;
	UINT32			uiTimeSinceLastBeep;
	INT8				bAnimate;
	INT8				bYPos;
	BOOLEAN			fCreated;
	INT16				sWorldRenderX;
	INT16				sWorldRenderY;

} TOP_MESSAGE;


TOP_MESSAGE		gTopMessage;
BOOLEAN				gfTopMessageDirty = FALSE;

void CreateTopMessage( UINT32 uiSurface, UINT8 ubType, STR16 psString );

extern UINT16 GetAnimStateForInteraction( SOLDIERTYPE *pSoldier, BOOLEAN fDoor, UINT16 usAnimState );


MOUSE_REGION	gMenuOverlayRegion;


MOUSE_REGION	gBottomPanalRegion;

UINT16				gusOldSelectedSoldier		= NOBODY;

// OVerlay ID
INT32					giPopupSlideMessageOverlay = -1;
UINT16				gusOverlayPopupBoxWidth, gusOverlayPopupBoxHeight;
MercPopUpBox	gpOverrideMercBox;

INT32					giUIMessageOverlay = -1;
UINT16				gusUIMessageWidth, gusUIMessageHeight;
MercPopUpBox	gpUIMessageOverrideMercBox;
UINT32				guiUIMessageTime = 0;
INT32					iOverlayMessageBox = -1;
INT32					iUIMessageBox = -1;
UINT32				guiUIMessageTimeDelay = 0;
BOOLEAN				gfUseSkullIconMessage = FALSE;

// Overlay callback
void BlitPopupText( VIDEO_OVERLAY *pBlitter );

BOOLEAN	gfPanelAllocated = FALSE;

extern MOUSE_REGION	gDisableRegion;
extern MOUSE_REGION gUserTurnRegion;
extern BOOLEAN gfUserTurnRegionActive;
extern UINT8 gubSelectSMPanelToMerc;
extern BOOLEAN gfIgnoreOnSelectedGuy;

typedef enum
{
	WALK_IMAGES = 0,
	SNEAK_IMAGES,
	RUN_IMAGES,
	CRAWL_IMAGES,
	LOOK_IMAGES,
	TALK_IMAGES,
	HAND_IMAGES,
	CANCEL_IMAGES,

	TARGETACTIONC_IMAGES,
	KNIFEACTIONC_IMAGES,
	AIDACTIONC_IMAGES,
	PUNCHACTIONC_IMAGES,
	BOMBACTIONC_IMAGES,

	OPEN_DOOR_IMAGES,
	EXAMINE_DOOR_IMAGES,
	LOCKPICK_DOOR_IMAGES,
	BOOT_DOOR_IMAGES,
	CROWBAR_DOOR_IMAGES,
	USE_KEY_IMAGES,
	USE_KEYRING_IMAGES,
	EXPLOSIVE_DOOR_IMAGES,

	TOOLKITACTIONC_IMAGES,
	WIRECUTACTIONC_IMAGES,

	NUM_ICON_IMAGES
};

INT32					iIconImages[ NUM_ICON_IMAGES ];

INT32					iActionIcons[ NUM_ICONS ];

// GLOBAL INTERFACE SURFACES
extern UINT32			guiRENDERBUFFER; // symbol already defined in sysutil.cpp (jonathanl)
UINT32					guiINTEXT;
UINT32					guiCLOSE;
UINT32					guiDEAD;
UINT32					guiHATCH;

UINT32					guiBUTTONBORDER;
UINT32					guiSILHOUETTE;
UINT32					guiRADIO;
UINT32					guiRADIO2;
UINT32					guiCOMPANEL;
UINT32					guiCOMPANELB;
UINT32					guiAIMCUBES;
UINT32					guiAIMBARS;
UINT32					guiVEHINV;
UINT32					guiBURSTACCUM;
UINT32					guiITEMPOINTERHATCHES;

UINT32					guiUNDERWATER;	// added by Flugente
UINT32					guiENEMYROLES;	// added by Flugente

// UI Globals
MOUSE_REGION	gViewportRegion;
MOUSE_REGION	gRadarRegion;


UINT16 gsUpArrowX;
UINT16 gsUpArrowY;
UINT16 gsDownArrowX;
UINT16 gsDownArrowY;

UINT32 giUpArrowRect;
UINT32 giDownArrowRect;

// HEADROCK HAM 4: Rectangles for the various CTH displays.
RECT MagRect;
RECT AimRect;
RECT ModeRect;
RECT APRect;
// sevenfm: additional rectangles
RECT RightRect;
RECT LeftRect;
RECT LeftRect2;
// HEADROCK HAM 4: Externed this value which tracks whether we've clicked out final aiming click yet.
extern BOOLEAN gfDisplayFullCountRing;

void DrawBarsInUIBox( SOLDIERTYPE *pSoldier , INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, INT16 interval);
void PopupDoorOpenMenu( BOOLEAN fClosingDoor );


BOOLEAN fFirstTimeInGameScreen	= TRUE;
BOOLEAN	fInterfacePanelDirty	= DIRTYLEVEL2;
INT16		gsInterfaceLevel			= I_GROUND_LEVEL;
INT32		gsCurrentSoldierGridNo	= 0;
INT16		gsCurInterfacePanel			= TEAM_PANEL;

// LOCAL FUCTIONS
void BtnPositionCallback( GUI_BUTTON *btn, INT32 reason );
void BtnMovementCallback( GUI_BUTTON *btn, INT32 reason );
void BtnDoorMenuCallback( GUI_BUTTON *btn, INT32 reason );
void MovementMenuBackregionCallback( MOUSE_REGION * pRegion, INT32 iReason );
void DoorMenuBackregionCallback( MOUSE_REGION * pRegion, INT32 iReason );

UINT32 CalcUIMessageDuration( STR16 wString );

extern FLOAT Distance2D( FLOAT dDeltaX, FLOAT dDeltaY );

// Flugente:  toggle display of enemy role indicators
BOOLEAN gDisplayEnemyRoles = TRUE;

#define SCSTI ".STI"
#define SCSTI_IMP "_IMP.STI"

BOOLEAN InitializeFaceGearGraphics()
{
	VOBJECT_DESC	VObjectDesc;

	char fileName[500];

	for ( UINT32 iCounter2 = 1; iCounter2 < gMAXITEMS_READ; ++iCounter2 )
	{		
		if ( zNewFaceGear[iCounter2].Type > 0 )
		{
			{
				VObjectDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE;
				strcpy(fileName, zNewFaceGear[iCounter2].szFile);
				strcat(fileName, SCSTI);
				strcpy(VObjectDesc.ImageFile, fileName);
				CHECKF(AddVideoObject(&VObjectDesc,&zNewFaceGear[iCounter2].uiIndex));
			}

			//IMP
			{
				VObjectDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE;
				strcpy( fileName, zNewFaceGear[iCounter2].szFile );
				strcat( fileName, SCSTI_IMP );
				strcpy( VObjectDesc.ImageFile, fileName );
				CHECKF( AddVideoObject( &VObjectDesc, &zNewFaceGearIMP[iCounter2].uiIndex ) );
			}
		}
	}

	return ( TRUE );
}

BOOLEAN InitializeTacticalPortraits(	)
{
	VOBJECT_DESC	VObjectDesc;

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	//CHRISL: Moved from InitializeTacticalInterface so we can reinitialize portait graphics as we need to
	//legion 2 jazz
	if (gGameSettings.fOptions[ TOPTION_SHOW_TACTICAL_FACE_ICONS ] == TRUE) 
	{
		//additional face icons (legion 2)
		if (gGameExternalOptions.bTacticalFaceIconStyle == 0) 
		{	
			FilenameForBPP("INTERFACE\\portraiticons_a.sti", VObjectDesc.ImageFile);

			if( !AddVideoObject( &VObjectDesc, &guiPORTRAITICONS ) )
				AssertMsg(0, "Missing INTERFACE\\portraiticons_a.sti" );
		}	
		else if (gGameExternalOptions.bTacticalFaceIconStyle == 1) 
		{	
			FilenameForBPP("INTERFACE\\portraiticons_b.sti", VObjectDesc.ImageFile);

			if( !AddVideoObject( &VObjectDesc, &guiPORTRAITICONS ) )
				AssertMsg(0, "Missing INTERFACE\\portraiticons_b.sti" );
		}	
		else if (gGameExternalOptions.bTacticalFaceIconStyle == 2) 
		{
			FilenameForBPP("INTERFACE\\portraiticons_c.sti", VObjectDesc.ImageFile);

			if( !AddVideoObject( &VObjectDesc, &guiPORTRAITICONS ) )
				AssertMsg(0, "Missing INTERFACE\\portraiticons_c.sti" );
		}
		else if (gGameExternalOptions.bTacticalFaceIconStyle == 3) 
		{
			FilenameForBPP("INTERFACE\\portraiticons_d.sti", VObjectDesc.ImageFile);

			if( !AddVideoObject( &VObjectDesc, &guiPORTRAITICONS ) )
				AssertMsg(0, "Missing INTERFACE\\portraiticons_d.sti" );
		}		
	}
	else 
	{
		// JA2 classic face icons
		FilenameForBPP("INTERFACE\\portraiticons.sti", VObjectDesc.ImageFile);

		if( !AddVideoObject( &VObjectDesc, &guiPORTRAITICONS ) )
			AssertMsg(0, "Missing INTERFACE\\portraiticons.sti" );
	}

	// Flugente: icons for assignments
	FilenameForBPP("INTERFACE\\AssignmentIcons.sti", VObjectDesc.ImageFile);

	if( !AddVideoObject( &VObjectDesc, &guiASSIGNMENTICONS ) )
		AssertMsg(0, "Missing INTERFACE\\AssignmentIcons.sti" );

	return ( TRUE );
}

BOOLEAN InitializeTacticalInterface(	)
{
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC	VObjectDesc;
	
	//UINT32 iCounter2;
	
	// CHRISL: Setup default interface coords based on inventory system in use
	if((UsingNewInventorySystem() == true))
	{
		InitNewInventorySystem();
		InitializeSMPanelCoordsNew();
	}
	else
	{
		InitOldInventorySystem();
		InitializeSMPanelCoordsOld();
	}

//*ddd{ 
	if( gGameExternalOptions.fSmallSizeProgressbar )
	{
		HEIGHT_PROGRESSBAR	= 7;
		PROG_BAR_START_Y	= 0;
	}
	else{
		HEIGHT_PROGRESSBAR	= 20;
		PROG_BAR_START_Y	= 2;
	}
//*ddd}

/*	OK i need to initialize coords here
 *	Isnt it cool
 *	any questions? joker
 */
	InitializeViewPort( );
	InitializeTEAMPanelCoords( );
	
	// Load button Interfaces
	iIconImages[ WALK_IMAGES	]			= LoadButtonImage("INTERFACE\\newicons3.sti", -1,3,4,5,-1 );
	iIconImages[ SNEAK_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 6, 7, 8, -1 );
	iIconImages[ RUN_IMAGES ]				= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 0, 1, 2, -1 );
	iIconImages[ CRAWL_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 9, 10, 11, -1 );
	iIconImages[ LOOK_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 12, 13, 14, -1 );
	iIconImages[ TALK_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 21, 22, 23, -1 );
	iIconImages[ HAND_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 18, 19, 20, -1 );
	iIconImages[ CANCEL_IMAGES ]		= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 15, 16, 17, -1 );

	iIconImages[ TARGETACTIONC_IMAGES ]		= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 24, 25, 26, -1 );
	iIconImages[ KNIFEACTIONC_IMAGES ]		= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 27, 28, 29, -1 );
	iIconImages[ AIDACTIONC_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 30, 31, 32, -1 );
	iIconImages[ PUNCHACTIONC_IMAGES ]		= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 33, 34, 35, -1 );
	iIconImages[ BOMBACTIONC_IMAGES ]			= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 36, 37, 38, -1 );
	iIconImages[ TOOLKITACTIONC_IMAGES ]	= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 39, 40, 41, -1 );
	iIconImages[ WIRECUTACTIONC_IMAGES ]	= UseLoadedButtonImage(iIconImages[ WALK_IMAGES	], -1, 42, 43, 44, -1 );

	iIconImages[ OPEN_DOOR_IMAGES ]				= LoadButtonImage("INTERFACE\\door_op2.sti", -1,9,10,11,-1 );
	iIconImages[ EXAMINE_DOOR_IMAGES ]		= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 12, 13, 14, -1 );
	iIconImages[ LOCKPICK_DOOR_IMAGES ]		= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 21, 22, 23, -1 );
	iIconImages[ BOOT_DOOR_IMAGES ]				= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 25, 26, 27, -1 );
	iIconImages[ CROWBAR_DOOR_IMAGES ]		= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 0, 1, 2, -1 );
	iIconImages[ USE_KEY_IMAGES ]					= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 3, 4, 5, -1 );
	iIconImages[ USE_KEYRING_IMAGES ]			= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 6, 7, 8, -1 );
	iIconImages[ EXPLOSIVE_DOOR_IMAGES ]	= UseLoadedButtonImage(iIconImages[ OPEN_DOOR_IMAGES	], -1, 15, 16, 17, -1 );

	// Load interface panels
	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

	// failing the CHECKF after this will cause you to lose your mouse

	if ( GETPIXELDEPTH() == 8 )
	{
		strcpy( vs_desc.ImageFile, "INTERFACE\\IN_TEXT_8.pcx" );
	}
	else if ( GETPIXELDEPTH() == 16 )
	{
		strcpy( vs_desc.ImageFile, "INTERFACE\\IN_TEXT.STI" );
	}

	if( !AddVideoSurface( &vs_desc, &guiINTEXT ) )
		AssertMsg( 0, "Missing INTERFACE\\In_text.sti");
	SetVideoSurfaceTransparency( guiINTEXT, FROMRGB( 255, 0, 0 ) );


	// LOAD CLOSE ANIM
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\p_close.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiCLOSE ) )
		AssertMsg(0, "Missing INTERFACE\\p_close.sti" );

	// LOAD DEAD ANIM
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\p_dead.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiDEAD ) )
		AssertMsg(0, "Missing INTERFACE\\p_dead.sti" );

	// LOAD HATCH
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\hatch.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiHATCH ) )
		AssertMsg(0, "Missing INTERFACE\\hatch.sti" );

	SGP_THROW_IFFALSE( RegisterItemImages(), L"Registering Item Images failed" );

	// CHRISL:
	// LOAD INTERFACE POCKET SILHOUETTES
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Silhouettes.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiSILHOUETTE ) )
		AssertMsg(0, "Missing INTERFACE\\Silhouettes.sti" );

	// LOAD INTERFACE BUTTON BORDER
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\button_frame.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiBUTTONBORDER ) )
		AssertMsg(0, "Missing INTERFACE\\button_frame.sti" );

	// LOAD AIM CUBES
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\aimcubes.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiAIMCUBES ) )
		AssertMsg(0, "Missing INTERFACE\\aimcubes.sti" );

	// LOAD AIM BARS
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\aimbars.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiAIMBARS ) )
		AssertMsg(0, "Missing INTERFACE\\aimbars.sti" );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventor.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiVEHINV ) )
		AssertMsg(0, "Missing INTERFACE\\inventor.sti" );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\burst1.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiBURSTACCUM ) )
		AssertMsg(0, "Missing INTERFACE\\burst1.sti" );

	// Flugente: load fish symbol (signifies that a merc is diving until someone comes up with a proper animation)
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\fish.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiUNDERWATER ) )
		AssertMsg(0, "Missing INTERFACE\\fish.sti" );

	// Flugente: enemy role symbols
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\RoleIcons.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiENEMYROLES ) )
		AssertMsg(0, "Missing INTERFACE\\RoleIcons.sti" );

	//CHRISL: Moved to seperate function so we can call seperately
	InitializeTacticalPortraits();

	InitializeFaceGearGraphics();
	
	// LOAD RADIO
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\radio.sti", VObjectDesc.ImageFile);

	if( !AddVideoObject( &VObjectDesc, &guiRADIO ) )	
		AssertMsg(0, "Missing INTERFACE\\radio.sti" );

	// LOAD RADIO2
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\radio2.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiRADIO2 ) )
		AssertMsg(0, "Missing INTERFACE\\radio2.sti" );

	// LOAD com panel 2
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\communicationpopup.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiCOMPANEL ) )
		AssertMsg(0, "Missing INTERFACE\\communicationpopup.sti" );

	// LOAD ITEM GRIDS....
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\itemgrid.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiITEMPOINTERHATCHES ) )
		AssertMsg(0, "Missing INTERFACE\\itemgrid.sti" );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\communicationpopup_2.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &guiCOMPANELB ) )
		AssertMsg(0, "Missing INTERFACE\\communicationpopup_2.sti" );


	// Alocate message surfaces
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = SCREEN_WIDTH;
	vs_desc.usHeight = 20;
	vs_desc.ubBitDepth = 16;
	CHECKF( AddVideoSurface( &vs_desc, &(gTopMessage.uiSurface) ) );


	InitItemInterface( );

	InitRadarScreen( );

	InitTEAMSlots( );

	// Init popup box images
//	CHECKF( LoadTextMercPopupImages( BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER ) );

	return( TRUE );
}


BOOLEAN ShutdownTacticalInterface( )
{
	ShutdownCurrentPanel( );

	return( TRUE );
}

BOOLEAN InitializeCurrentPanel( )
{
	BOOLEAN		fOK = FALSE;

	MoveRadarScreen( );

	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			// Set new viewport
			gsVIEWPORT_WINDOW_END_Y =	INV_INTERFACE_START_Y;

			// Render full
			SetRenderFlags(RENDER_FLAG_FULL);
			fOK = InitializeSMPanel( );
			break;

		case TEAM_PANEL:
			gsVIEWPORT_WINDOW_END_Y = INTERFACE_START_Y;
			// Render full
			SetRenderFlags(RENDER_FLAG_FULL);
			fOK = InitializeTEAMPanel( );
			break;
	}

//	RefreshMouseRegions( );
	gfPanelAllocated = TRUE;

	return( fOK );
}

void ShutdownCurrentPanel( )
{
	if ( gfPanelAllocated )
	{

		switch( gsCurInterfacePanel )
		{
			case SM_PANEL:
				ShutdownSMPanel( );
				break;

			case TEAM_PANEL:
				ShutdownTEAMPanel( );
				break;
		}

		gfPanelAllocated = FALSE;

	}
}






void SetCurrentTacticalPanelCurrentMerc( UINT8 ubID )
{
	SOLDIERTYPE *pSoldier;

	// Disable faces
	SetAllAutoFacesInactive( );

	if ( gsCurInterfacePanel == SM_PANEL )
	{
		// If we are not of merc bodytype, or am an epc, and going into inv, goto another....
		pSoldier = MercPtrs[ ubID ];

		if ( !IS_MERC_BODY_TYPE( pSoldier ) || AM_AN_EPC( pSoldier ) )
		{
			SetCurrentInterfacePanel( TEAM_PANEL );
		}
	}

	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			//SetSMPanelCurrentMerc( ubID );
			gubSelectSMPanelToMerc = ubID;
			break;

		case TEAM_PANEL:
			SetTEAMPanelCurrentMerc( (UINT8)gusSelectedSoldier );
			break;
	}


}

void CreateCurrentTacticalPanelButtons( )
{
	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			CreateSMPanelButtons( /*fInterfacePanelDirty*/ ); // doesn't take parameters (jonathanl)
			break;

		case TEAM_PANEL:
			CreateTEAMPanelButtons( /*fInterfacePanelDirty*/ ); // doesn't take parameters (jonathanl)
			break;
	}

}

void SetCurrentInterfacePanel( UINT8 ubNewPanel )
{
	ShutdownCurrentPanel( );

	// INit new panel
	gsCurInterfacePanel = ubNewPanel;

	InitializeCurrentPanel( );


}


void ToggleTacticalPanels( )
{
	gfSwitchPanel			= TRUE;
	gubNewPanelParam	= (UINT8)gusSelectedSoldier;

	if ( gsCurInterfacePanel == SM_PANEL )
	{
		gbNewPanel = TEAM_PANEL;
	}
	else
	{
		gbNewPanel = SM_PANEL;
		// silversurfer: This needs to be reset to TRUE because inventory view only allows radar screen to be displayed and not the squad list.
		fRenderRadarScreen = TRUE;
	}
}


void RemoveCurrentTacticalPanelButtons( )
{
	switch( gsCurInterfacePanel )
	{
		case SM_PANEL:
			RemoveSMPanelButtons( /*fInterfacePanelDirty*/ ); // doesn't take parameters (jonathanl)
			break;

		case TEAM_PANEL:
			RemoveTEAMPanelButtons( /*fInterfacePanelDirty*/ ); // doesn't take parameters (jonathanl)
			break;
	}
}


BOOLEAN IsMercPortraitVisible( UINT8 ubSoldierID )
{
	if ( gsCurInterfacePanel == TEAM_PANEL )
	{
		return( TRUE );
	}

	if ( gsCurInterfacePanel == SM_PANEL )
	{
		if ( GetSMPanelCurrentMerc() == ubSoldierID )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void HandleInterfaceBackgrounds( )
{
	HandleUpDownArrowBackgrounds( );
}


void PopupPositionMenu( UI_EVENT *pUIEvent )
{

}

void PopDownPositionMenu( )
{

}


void BtnPositionCallback(GUI_BUTTON *btn,INT32 reason)
{


}



void PopupMovementMenu( UI_EVENT *pUIEvent )
{
	SOLDIERTYPE					*pSoldier = NULL;
	INT32								iMenuAnchorX, iMenuAnchorY;
	UINT32							uiActionImages;
	CHAR16								zActionString[ 50 ];
	BOOLEAN							fDisableAction = FALSE;

	// Erase other menus....
	EraseInterfaceMenus( TRUE );

	giMenuAnchorX = gusMouseXPos - 18;
	giMenuAnchorY = gusMouseYPos - 18;

	// ATE: OK loser, let's check if we're going off the screen!
	if ( giMenuAnchorX < 0 )
	{
		giMenuAnchorX = 0;
	}

	if ( giMenuAnchorY < 0 )
	{
		giMenuAnchorY = 0;
	}


	// Create mouse region over all area to facilitate clicking to end
	MSYS_DefineRegion( &gMenuOverlayRegion, 0, 0 ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST-1,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, MovementMenuBackregionCallback );
	// Add region
	MSYS_AddRegion( &gMenuOverlayRegion);


	// OK, CHECK FOR BOUNDARIES!
	if ( ( giMenuAnchorX + BUTTON_PANEL_WIDTH ) > SCREEN_WIDTH )
	{
		giMenuAnchorX = ( SCREEN_WIDTH - BUTTON_PANEL_WIDTH );
	}
	if ( ( giMenuAnchorY + BUTTON_PANEL_HEIGHT ) > gsVIEWPORT_WINDOW_END_Y )
	{
		giMenuAnchorY = ( gsVIEWPORT_WINDOW_END_Y - BUTTON_PANEL_HEIGHT );
	}



	if ( gusSelectedSoldier != NOBODY )
	{
		pSoldier = MercPtrs[ gusSelectedSoldier ];
	}

	// Blit background!
	//BltVideoObjectFromIndex( FRAME_BUFFER, guiBUTTONBORDER, 0, iMenuAnchorX, iMenuAnchorY, VO_BLT_SRCTRANSPARENCY, NULL );

	iMenuAnchorX = giMenuAnchorX + 9;
	iMenuAnchorY = giMenuAnchorY + 8;

	iActionIcons[ RUN_ICON ] = QuickCreateButton( iIconImages[ RUN_IMAGES ], (INT16)(iMenuAnchorX + 20 ), (INT16)(iMenuAnchorY ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ RUN_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	SetButtonFastHelpText( iActionIcons[ RUN_ICON ], pTacticalPopupButtonStrings[ RUN_ICON ] );
	//SetButtonSavedRect( iActionIcons[ RUN_ICON ] );
	ButtonList[ iActionIcons[ RUN_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( pSoldier->MercInWater( ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
	{
		DisableButton( iActionIcons[ RUN_ICON ] );
	}

	iActionIcons[ WALK_ICON ] = QuickCreateButton( iIconImages[ WALK_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY	),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ WALK_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	//SetButtonSavedRect( iActionIcons[ WALK_ICON ] );

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		SetButtonFastHelpText( iActionIcons[ WALK_ICON ], TacticalStr[ DRIVE_POPUPTEXT ] );
	}
	else
	{
		SetButtonFastHelpText( iActionIcons[ WALK_ICON ], pTacticalPopupButtonStrings[ WALK_ICON ] );
	}

	ButtonList[ iActionIcons[ WALK_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		if ( !pSoldier->CanRobotBeControlled( ) )
		{
			DisableButton( iActionIcons[ WALK_ICON ] );
		}
	}


	iActionIcons[ SNEAK_ICON ] = QuickCreateButton( iIconImages[ SNEAK_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ SNEAK_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	SetButtonFastHelpText( iActionIcons[ SNEAK_ICON ], pTacticalPopupButtonStrings[ SNEAK_ICON ] );
	//SetButtonSavedRect( iActionIcons[ SNEAK_ICON ] );
	ButtonList[ iActionIcons[ SNEAK_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	// Check if this is a valid stance, diable if not!
	if ( !IsValidStance( pSoldier, ANIM_CROUCH ) )
	{
		DisableButton( iActionIcons[ SNEAK_ICON ]	);
	}

	iActionIcons[ CRAWL_ICON ] = QuickCreateButton( iIconImages[ CRAWL_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ CRAWL_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	SetButtonFastHelpText( iActionIcons[ CRAWL_ICON ], pTacticalPopupButtonStrings[ CRAWL_ICON ] );
	//SetButtonSavedRect( iActionIcons[ CRAWL_ICON ] );
	ButtonList[ iActionIcons[ CRAWL_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	// Check if this is a valid stance, diable if not!
	if ( !IsValidStance( pSoldier, ANIM_PRONE ) )
	{
		DisableButton( iActionIcons[ CRAWL_ICON ]	);
	}


	iActionIcons[ LOOK_ICON ] = QuickCreateButton( iIconImages[ LOOK_IMAGES ], (INT16)(iMenuAnchorX ), (INT16)(iMenuAnchorY ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ LOOK_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	SetButtonFastHelpText( iActionIcons[ LOOK_ICON ], TacticalStr[ LOOK_CURSOR_POPUPTEXT ] );
	//SetButtonSavedRect( iActionIcons[ LOOK_ICON ] );
	ButtonList[ iActionIcons[ LOOK_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		DisableButton( iActionIcons[ LOOK_ICON ]	);
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		if ( !pSoldier->CanRobotBeControlled( ) )
		{
			DisableButton( iActionIcons[ LOOK_ICON ] );
		}
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// Until we get mounted weapons...
		uiActionImages = CANCEL_IMAGES;
		swprintf( zActionString, TacticalStr[ NOT_APPLICABLE_POPUPTEXT ] );
		fDisableAction = TRUE;
	}
	else
	{
		if ( Item[pSoldier->inv[ HANDPOS ].usItem].toolkit )
		{
			uiActionImages = TOOLKITACTIONC_IMAGES;
			swprintf( zActionString, TacticalStr[ NOT_APPLICABLE_POPUPTEXT ] );
		}
		else if ( Item[pSoldier->inv[ HANDPOS ].usItem].wirecutters )
		{
			uiActionImages = WIRECUTACTIONC_IMAGES;
			swprintf( zActionString, TacticalStr[ NOT_APPLICABLE_POPUPTEXT ] );
		}
		else
		{
			// Create button based on what is in our hands at the moment!
			switch( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass )
			{
				case IC_PUNCH:

					uiActionImages = PUNCHACTIONC_IMAGES;
					swprintf( zActionString, TacticalStr[ USE_HANDTOHAND_POPUPTEXT ] );
					break;

				case IC_GUN:

					uiActionImages = TARGETACTIONC_IMAGES;
					swprintf( zActionString, TacticalStr[ USE_FIREARM_POPUPTEXT ] );
					break;

				case IC_BLADE:

					uiActionImages = KNIFEACTIONC_IMAGES;
					swprintf( zActionString, TacticalStr[ USE_BLADE_POPUPTEXT ] );
					break;

				case IC_GRENADE:
				case IC_BOMB:

					uiActionImages = BOMBACTIONC_IMAGES;
					swprintf( zActionString, TacticalStr[ USE_EXPLOSIVE_POPUPTEXT ] );
					break;

				case IC_MEDKIT:

					uiActionImages = AIDACTIONC_IMAGES;
					swprintf( zActionString, TacticalStr[ USE_MEDKIT_POPUPTEXT ] );
					break;

				default:

					uiActionImages = CANCEL_IMAGES;
					swprintf( zActionString, TacticalStr[ NOT_APPLICABLE_POPUPTEXT ] );
					fDisableAction = TRUE;
					break;
			}
		}
	}

	if ( AM_AN_EPC( pSoldier ) )
	{
		fDisableAction = TRUE;
	}

	// anv: don't switch if passengers are blocked from attacking
	if( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
	{
		SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
		INT8 bSeatIndex = GetSeatIndexFromSoldier( pSoldier );
		if( gNewVehicle[ pVehicleList[ pSoldier->iVehicleId ].ubVehicleType ].VehicleSeats[bSeatIndex].fBlockedShots == TRUE )
		{
			fDisableAction = TRUE;
		}
	}

	iActionIcons[ ACTIONC_ICON ] = QuickCreateButton( iIconImages[ uiActionImages ], (INT16)(iMenuAnchorX	), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ ACTIONC_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	//SetButtonSavedRect( iActionIcons[ ACTIONC_ICON ] );
	SetButtonFastHelpText( iActionIcons[ ACTIONC_ICON ], zActionString );
	ButtonList[ iActionIcons[ ACTIONC_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( fDisableAction )
	{
		DisableButton( iActionIcons[ ACTIONC_ICON ]	);
	}


	iActionIcons[ TALK_ICON ] = QuickCreateButton( iIconImages[ TALK_IMAGES ], (INT16)(iMenuAnchorX	), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ TALK_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	//SetButtonSavedRect( iActionIcons[ TALK_ICON ] );
	SetButtonFastHelpText( iActionIcons[ TALK_ICON ], pTacticalPopupButtonStrings[ TALK_ICON ] );
	ButtonList[ iActionIcons[ TALK_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( AM_AN_EPC( pSoldier ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		DisableButton( iActionIcons[ TALK_ICON ]	);
	}


	iActionIcons[ HAND_ICON ] = QuickCreateButton( iIconImages[ HAND_IMAGES ], (INT16)(iMenuAnchorX + 20	), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ HAND_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	//SetButtonSavedRect( iActionIcons[ HAND_ICON ] );
	SetButtonFastHelpText( iActionIcons[ HAND_ICON ], pTacticalPopupButtonStrings[ HAND_ICON ] );
	ButtonList[ iActionIcons[ HAND_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	if ( AM_AN_EPC( pSoldier ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		DisableButton( iActionIcons[ HAND_ICON ]	);
	}

	iActionIcons[ CANCEL_ICON ] = QuickCreateButton( iIconImages[ CANCEL_IMAGES ], (INT16)(iMenuAnchorX + 20	), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMovementCallback );
	if ( iActionIcons[ CANCEL_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	//SetButtonSavedRect( iActionIcons[ CANCEL_ICON ] );
	SetButtonFastHelpText( iActionIcons[ CANCEL_ICON ], pTacticalPopupButtonStrings[ CANCEL_ICON ] );
	ButtonList[ iActionIcons[ CANCEL_ICON ] ]->UserData[0] = (UINT32)pUIEvent;

	//LockTacticalInterface( );

	gfInMovementMenu = TRUE;

	// Ignore scrolling
	gfIgnoreScrolling = TRUE;

}

void PopDownMovementMenu( )
{
	if ( gfInMovementMenu )
	{
		RemoveButton( iActionIcons[ WALK_ICON	] );
		RemoveButton( iActionIcons[ SNEAK_ICON	] );
		RemoveButton( iActionIcons[ RUN_ICON	] );
		RemoveButton( iActionIcons[ CRAWL_ICON	] );
		RemoveButton( iActionIcons[ LOOK_ICON	] );
		RemoveButton( iActionIcons[ ACTIONC_ICON	] );
		RemoveButton( iActionIcons[ TALK_ICON	] );
		RemoveButton( iActionIcons[ HAND_ICON	] );
		RemoveButton( iActionIcons[ CANCEL_ICON	] );

		// Turn off Ignore scrolling
		gfIgnoreScrolling = FALSE;

		// Rerender world
		SetRenderFlags( RENDER_FLAG_FULL );

		fInterfacePanelDirty = DIRTYLEVEL2;

		//UnLockTacticalInterface( );
		MSYS_RemoveRegion( &gMenuOverlayRegion );

	}

	gfInMovementMenu = FALSE;

}

void RenderMovementMenu( )
{
	if ( gfInMovementMenu )
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, guiBUTTONBORDER, 0, giMenuAnchorX, giMenuAnchorY, VO_BLT_SRCTRANSPARENCY, NULL );

		// Mark buttons dirty!
		MarkAButtonDirty( iActionIcons[ WALK_ICON	] );
		MarkAButtonDirty( iActionIcons[ SNEAK_ICON	] );
		MarkAButtonDirty( iActionIcons[ RUN_ICON	] );
		MarkAButtonDirty( iActionIcons[ CRAWL_ICON	] );
		MarkAButtonDirty( iActionIcons[ LOOK_ICON	] );
		MarkAButtonDirty( iActionIcons[ ACTIONC_ICON	] );
		MarkAButtonDirty( iActionIcons[ TALK_ICON	] );
		MarkAButtonDirty( iActionIcons[ HAND_ICON	] );
		MarkAButtonDirty( iActionIcons[ CANCEL_ICON	] );

		InvalidateRegion( giMenuAnchorX, giMenuAnchorY, giMenuAnchorX + BUTTON_PANEL_WIDTH, giMenuAnchorY + BUTTON_PANEL_HEIGHT );

	}
}

void CancelMovementMenu( )
{
	// Signal end of event
	PopDownMovementMenu( );
	guiPendingOverrideEvent		= A_CHANGE_TO_MOVE;
}

void BtnMovementCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32		uiBtnID;
	UI_EVENT	*pUIEvent;

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;

		uiBtnID = btn->IDNum;

		pUIEvent = (UI_EVENT*)( btn->UserData[0] );

		if ( uiBtnID == iActionIcons[ WALK_ICON	] )
		{
			pUIEvent->uiParams[0] = MOVEMENT_MENU_WALK;
		}
		else if ( uiBtnID == iActionIcons[ RUN_ICON	] )
		{
			pUIEvent->uiParams[0] = MOVEMENT_MENU_RUN;
		}
		else if ( uiBtnID == iActionIcons[ SNEAK_ICON	] )
		{
			pUIEvent->uiParams[0] = MOVEMENT_MENU_SWAT;
		}
		else if ( uiBtnID == iActionIcons[ CRAWL_ICON	] )
		{
			pUIEvent->uiParams[0] = MOVEMENT_MENU_PRONE;
		}
		else if ( uiBtnID == iActionIcons[ LOOK_ICON	] )
		{
			pUIEvent->uiParams[2] = MOVEMENT_MENU_LOOK;
		}
		else if ( uiBtnID == iActionIcons[ ACTIONC_ICON	] )
		{
			pUIEvent->uiParams[2] = MOVEMENT_MENU_ACTIONC;
		}
		else if ( uiBtnID == iActionIcons[ TALK_ICON	] )
		{
			pUIEvent->uiParams[2] = MOVEMENT_MENU_TALK;
		}
		else if ( uiBtnID == iActionIcons[ HAND_ICON	] )
		{
			pUIEvent->uiParams[2] = MOVEMENT_MENU_HAND;
		}
		else if ( uiBtnID == iActionIcons[ CANCEL_ICON	] )
		{
			// Signal end of event
			EndMenuEvent( U_MOVEMENT_MENU );
			pUIEvent->uiParams[1] = FALSE;
			return;
		}
		else
		{
			return;
		}

		// Signal end of event
		EndMenuEvent( U_MOVEMENT_MENU );
		pUIEvent->uiParams[1] = TRUE;

	}

}

void HandleUpDownArrowBackgrounds( )
{
	static	UINT32						uiOldShowUpDownArrows = ARROWS_HIDE_UP | ARROWS_HIDE_DOWN;

	// Check for change in mode
	if ( guiShowUPDownArrows != uiOldShowUpDownArrows || gfUIRefreshArrows )
	{
		gfUIRefreshArrows = FALSE;

		// Hide position of new ones
		GetArrowsBackground( );

		uiOldShowUpDownArrows = guiShowUPDownArrows;

	}

}

void RenderArrows( )
{
	TILE_ELEMENT							TileElem;

	if ( guiShowUPDownArrows & ARROWS_HIDE_UP && guiShowUPDownArrows & ARROWS_HIDE_DOWN )
	{
		return;
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_BESIDE )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_G )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS1 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_Y )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_YG )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			TileElem = gTileDatabase[ SECONDPOINTERS1 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_GG )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS1 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_YY )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS8 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB2 )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
			TileElem = gTileDatabase[ SECONDPOINTERS8 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB3 )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS3 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			TileElem = gTileDatabase[ SECONDPOINTERS8 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsUpArrowX, gsUpArrowY + 40, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BESIDE )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS4 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_G )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS2 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_Y )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS4 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_CLIMB )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS7 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_YG )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS2 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			TileElem = gTileDatabase[ SECONDPOINTERS4 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_GG )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS2 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_YY )
	{
			TileElem = gTileDatabase[ SECONDPOINTERS4 ];
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, gsDownArrowX, gsDownArrowY + 20, VO_BLT_SRCTRANSPARENCY, NULL );
	}


}

void EraseRenderArrows( )
{
	if ( giUpArrowRect != 0 )
	{
		if ( giUpArrowRect != -1 )
		{
			FreeBackgroundRect( giUpArrowRect );
		}
	}
	giUpArrowRect = 0;

	if ( giDownArrowRect != 0 )
	{
		if ( giDownArrowRect != -1 )
		{
			FreeBackgroundRect( giDownArrowRect );
		}
	}
	giDownArrowRect = 0;
}

void GetArrowsBackground( )
{
	SOLDIERTYPE								*pSoldier;
	INT16											sMercScreenX, sMercScreenY;
	UINT16										sArrowHeight = ARROWS_HEIGHT, sArrowWidth = ARROWS_WIDTH;

	if ( guiShowUPDownArrows & ARROWS_HIDE_UP && guiShowUPDownArrows & ARROWS_HIDE_DOWN )
	{
		return;
	}

	if ( gusSelectedSoldier != NOBODY )
	{
		// Get selected soldier
		GetSoldier( &pSoldier, gusSelectedSoldier );

		// Get screen position of our guy
		GetSoldierTRUEScreenPos( pSoldier, &sMercScreenX, &sMercScreenY );

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_BESIDE )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX + ARROWS_X_OFFSET;
			gsUpArrowY = sMercScreenY + UPARROW_Y_OFFSET;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_G || guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_Y )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX - 10;
			gsUpArrowY = sMercScreenY - 50;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_YG || guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_GG || guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_YY )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX - 10;
			gsUpArrowY = sMercScreenY - 70;
			sArrowHeight = 3 * ARROWS_HEIGHT;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX - 10;
			gsUpArrowY = sMercScreenY - 70;
			sArrowHeight = 2 * ARROWS_HEIGHT;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB2 )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX - 10;
			gsUpArrowY = sMercScreenY - 80;
			sArrowHeight = 3 * ARROWS_HEIGHT;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_UP_ABOVE_CLIMB3 )
		{
			// Setup blt rect
			gsUpArrowX = sMercScreenX - 10;
			gsUpArrowY = sMercScreenY - 900;
			sArrowHeight = 5 * ARROWS_HEIGHT;
		}


		if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BESIDE )
		{
			gsDownArrowX = sMercScreenX + ARROWS_X_OFFSET;
			gsDownArrowY = sMercScreenY + DOWNARROW_Y_OFFSET;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_Y || guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_G )
		{
			gsDownArrowX = sMercScreenX -10;
			gsDownArrowY = sMercScreenY + 10;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_CLIMB )
		{
			gsDownArrowX = sMercScreenX - 10;
			gsDownArrowY = sMercScreenY + 10;
			sArrowHeight = 3 * ARROWS_HEIGHT;
		}

		if ( guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_YG || guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_GG || guiShowUPDownArrows & ARROWS_SHOW_DOWN_BELOW_YY )
		{
			gsDownArrowX = sMercScreenX -10;
			gsDownArrowY = sMercScreenY + 10;
			sArrowHeight = 3 * ARROWS_HEIGHT;
		}

		// Adjust arrows based on level
		if ( gsInterfaceLevel == I_ROOF_LEVEL )
		{
		//	gsDownArrowY -= ROOF_LEVEL_HEIGHT;
		//	gsUpArrowY	-= ROOF_LEVEL_HEIGHT;
		}

		//Erase prevois ones...
		EraseRenderArrows( );

		// Register dirty rects
		giDownArrowRect = RegisterBackgroundRect(BGND_FLAG_PERMANENT, NULL, gsDownArrowX, gsDownArrowY, (INT16)(gsDownArrowX + sArrowWidth), (INT16)(gsDownArrowY + sArrowHeight ) );
		giUpArrowRect = RegisterBackgroundRect(BGND_FLAG_PERMANENT, NULL, gsUpArrowX, gsUpArrowY, (INT16)(gsUpArrowX + sArrowWidth), (INT16)(gsUpArrowY + sArrowHeight ) );
	}
}


void GetSoldierAboveGuyPositions( SOLDIERTYPE *pSoldier, INT16 *psX, INT16 *psY, BOOLEAN fRadio )
{
	INT16 sMercScreenX, sMercScreenY;
	INT16 sOffsetX, sOffsetY;
	UINT8	ubAnimUseHeight;
	INT16		sStanceOffset = 0;
	INT16		sTextBodyTypeYOffset = 62;

	// Find XY, dims, offsets
	GetSoldierScreenPos( pSoldier, &sMercScreenX, &sMercScreenY );
	GetSoldierAnimOffsets( pSoldier, &sOffsetX, &sOffsetY );

	// OK, first thing to do is subtract offsets ( because GetSoldierScreenPos adds them... )
	sMercScreenX -= sOffsetX;
	sMercScreenY -= sOffsetY;

	// Adjust based on stance
	if ( ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_NOMOVE_MARKER) )
	{
		ubAnimUseHeight = gAnimControl[ pSoldier->usAnimState ].ubHeight;
	}
	else
	{
		ubAnimUseHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;
	}
	switch ( ubAnimUseHeight )
	{
		case ANIM_STAND:
			break;

		case ANIM_PRONE:
			sStanceOffset = 25;
			break;

		case ANIM_CROUCH:
			sStanceOffset = 10;
			break;
	}

	// Adjust based on body type...
	switch( pSoldier->ubBodyType )
	{
		case CROW:

			sStanceOffset = 30;
			break;

		case ROBOTNOWEAPON:

			sStanceOffset = 30;
			break;

	}

	// anv: passengers sit, but we want to display their names above vehicle
	if( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
	{
		sStanceOffset = 0;
	}

	//sStanceOffset -= gpWorldLevelData[ pSoldier->sGridNo ].sHeight;

	// Adjust based on level
	if ( pSoldier->pathing.bLevel == 1 && gsInterfaceLevel == 0 )
	{
		//sStanceOffset -= ROOF_LEVEL_HEIGHT;
	}
	if ( pSoldier->pathing.bLevel == 0 && gsInterfaceLevel == 1 )
	{
		//sStanceOffset += ROOF_LEVEL_HEIGHT;
	}

	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		if ( fRadio )
		{
			*psX = sMercScreenX - ( 80 / 2 ) - pSoldier->sLocatorOffX;
			*psY = sMercScreenY - sTextBodyTypeYOffset + sStanceOffset;
		}
		else
		{
			*psX = sMercScreenX - ( 80 / 2 ) - pSoldier->sLocatorOffX;
			*psY = sMercScreenY - sTextBodyTypeYOffset + sStanceOffset;

			// OK, Check if we need to go below....
			// Can do this 1) if displaying damge or 2 ) above screen

			// If not a radio position, adjust if we are getting hit, to be lower!
			// If we are getting hit, lower them!
			if ( pSoldier->flags.fDisplayDamage || *psY < gsVIEWPORT_WINDOW_START_Y )
			{
				*psX = sMercScreenX - ( 80 / 2 ) - pSoldier->sLocatorOffX;
				*psY = sMercScreenY;
			}
		}

	}
	else
	{
		//Display Text!
		*psX = sMercScreenX - ( 80 / 2 ) - pSoldier->sLocatorOffX;
		*psY = sMercScreenY - sTextBodyTypeYOffset + sStanceOffset;
	}
}

void DrawCTHPixelToBuffer( UINT16 *pBuffer, UINT32 uiPitch, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, INT16 sPixelX, INT16 sPixelY, UINT16 usColor )
{
	///////////////////////////////////////////////////////////
	// HEADROCK HAM 4:
	//
	// This is a specialized drawing function that's used for the CTH indicator. It makes sure that the
	// pixel is drawn only within the allowed region (the Tactical Viewport), and never drawn in regions
	// reserved for other parts of the CTH indicator.
 
	if (sPixelX < sLeft || sPixelX > sRight || sPixelY < sTop || sPixelY > (sBottom-1))
	{
		return;
	}
	// Do not draw in other CTH rects!
	if ((sPixelX >= MagRect.left && sPixelX <= MagRect.right) && (sPixelY >= MagRect.top && sPixelY <= MagRect.bottom))
	{
		return;
	}
	if ((sPixelX >= AimRect.left && sPixelX <= AimRect.right) && (sPixelY >= AimRect.top && sPixelY <= AimRect.bottom))
	{
		return;
	}
	if ((sPixelX >= ModeRect.left && sPixelX <= ModeRect.right) && (sPixelY >= ModeRect.top && sPixelY <= ModeRect.bottom))
	{
		return;
	}
	if ((sPixelX >= APRect.left && sPixelX <= APRect.right) && (sPixelY >= APRect.top && sPixelY <= APRect.bottom))
	{
		return;
	}
	// sevenfm: added additional right, left rectangles
	if ((sPixelX >= RightRect.left && sPixelX <= RightRect.right) && (sPixelY >= RightRect.top && sPixelY <= RightRect.bottom))
	{
		return;
	}
	if ((sPixelX >= LeftRect.left && sPixelX <= LeftRect.right) && (sPixelY >= LeftRect.top && sPixelY <= LeftRect.bottom))
	{
		return;
	}
	if ((sPixelX >= LeftRect2.left && sPixelX <= LeftRect2.right) && (sPixelY >= LeftRect2.top && sPixelY <= LeftRect2.bottom))
	{
		return;
	}
	
	pBuffer[sPixelX + uiPitch*sPixelY] = usColor;
}

//QUOTE_SYSTEM_STRUCT	soldierTTInfo;

void DrawSelectedUIAboveGuy( UINT16 usSoldierID )
{
	SOLDIERTYPE		*pSoldier;
	INT16			sXPos, sYPos;
	INT16			sX, sY;
	INT32			iBack;
	TILE_ELEMENT	TileElem;
	CHAR16			*pStr;
	CHAR16			NameStr[ MAX_ENEMY_NAMES_CHARS ];
	UINT16			usGraphicToUse = THIRDPOINTERS1;
	BOOLEAN		 fRaiseName = FALSE;
	BOOLEAN		 fDoName = TRUE;
	UINT16 iCounter2;
	INT16			maxWidth = 40;		// sevenfm: maximum string length (in pixels) is used to determine x coordinate for additional info

	GetSoldier( &pSoldier, usSoldierID );

	if ( !pSoldier || (pSoldier->bVisible == -1 && !(gTacticalStatus.uiFlags&SHOW_ALL_MERCS) ) )
	{
		return;
	}
	
	if (TileIsOutOfBounds(pSoldier->sGridNo))
	{
		return;
	}

	if ( pSoldier->flags.fFlashLocator )
	{
		if ( pSoldier->bVisible == -1 )
		{
			pSoldier->flags.fFlashLocator = FALSE;
		}
		else
		{
			if ( TIMECOUNTERDONE( pSoldier->timeCounters.BlinkSelCounter, 80 ) )
			{
				RESETTIMECOUNTER( pSoldier->timeCounters.BlinkSelCounter, 80 );

			//	pSoldier->flags.fShowLocator = !pSoldier->flags.fShowLocator;

				pSoldier->flags.fShowLocator = TRUE;

				// Update frame
				pSoldier->sLocatorFrame++;

				if ( pSoldier->sLocatorFrame == 5 )
				{
					// Update time we do this
					pSoldier->flags.fFlashLocator++;
					pSoldier->sLocatorFrame = 0;
				}
			}

			//if ( TIMECOUNTERDONE( pSoldier->timeCounters.FlashSelCounter, 5000 ) )
			//{
			//	RESETTIMECOUNTER( pSoldier->timeCounters.FlashSelCounter, 5000 );

			//	pSoldier->flags.fFlashLocator = FALSE;
			//	pSoldier->flags.fShowLocator = FALSE;

			//}
			if ( pSoldier->flags.fFlashLocator == pSoldier->ubNumLocateCycles )
			{
				pSoldier->flags.fFlashLocator = FALSE;
				pSoldier->flags.fShowLocator = FALSE;
			}

			//if ( pSoldier->flags.fShowLocator )
			{
				// Render the beastie
				// anv: if soldier is in vehicle, draw above middle of the vehicle
				if( !(pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
				{
					GetSoldierAboveGuyPositions( pSoldier, &sXPos, &sYPos, TRUE );
				}
				else
				{
					SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
					GetSoldierAboveGuyPositions( pVehicle, &sXPos, &sYPos, TRUE );
				}

				// Adjust for bars!
				sXPos += 25;
				sYPos += 25;

				//sXPos += 15;
				//sYPos += 21;


				// Add bars
				//iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 55 ), (INT16)(sYPos + 80 ) );
				iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos +40 ), (INT16)(sYPos + 40 ) );

				if ( iBack != -1 )
				{
					SetBackgroundRectFilled( iBack );
				}

				if ( !pSoldier->aiData.bNeutral && ( pSoldier->bSide != gbPlayerNum ) )
				{
					BltVideoObjectFromIndex(	FRAME_BUFFER, guiRADIO2, pSoldier->sLocatorFrame, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );
				}
				else
				{
					BltVideoObjectFromIndex(	FRAME_BUFFER, guiRADIO, pSoldier->sLocatorFrame, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );

					//BltVideoObjectFromIndex(	FRAME_BUFFER, guiRADIO, 0, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}
		}
	}
	// Flugente: show a small animation hat signifies that a merc is underwater (hopefully temporary until someone comes up with a proper animation)
	else if ( pSoldier->UsesScubaGear() )
	{
		if ( TIMECOUNTERDONE( pSoldier->timeCounters.BlinkSelCounter, 320 ) )
		{
			RESETTIMECOUNTER( pSoldier->timeCounters.BlinkSelCounter, 320 );
			
			// Update frame
			pSoldier->sLocatorFrame++;

			if ( pSoldier->sLocatorFrame == 5 )
			{
				// Update time we do this
				pSoldier->sLocatorFrame = 0;
			}
		}

		if ( pSoldier->flags.fFlashLocator == pSoldier->ubNumLocateCycles )
		{
				pSoldier->flags.fShowLocator = FALSE;
		}
		
		if ( gGameExternalOptions.ubShowHealthBarsOnHead )
		{
			// Render the beastie
			// anv: if soldier is in vehicle, draw above middle of the vehicle
			if( !(pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
			{
				GetSoldierAboveGuyPositions( pSoldier, &sXPos, &sYPos, TRUE );
			}
			else
			{
				SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
				GetSoldierAboveGuyPositions( pVehicle, &sXPos, &sYPos, TRUE );
			}

			// Adjust for bars!
			sXPos += 25;
			sYPos += 25;

			// Add bars
			iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos +40 ), (INT16)(sYPos + 40 ) );

			if ( iBack != -1 )
			{
				SetBackgroundRectFilled( iBack );
			}

			BltVideoObjectFromIndex( FRAME_BUFFER, guiUNDERWATER, pSoldier->sLocatorFrame, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	if ( gDisplayEnemyRoles && pSoldier->bTeam == ENEMY_TEAM || pSoldier->bTeam == CIV_TEAM )
	{
		ShowSoldierRoleSymbol(pSoldier);
	}

	if ( !pSoldier->flags.fShowLocator )
	{
		// RETURN IF MERC IS NOT SELECTED
		if ( gfUIHandleSelectionAboveGuy && pSoldier->ubID == gsSelectedGuy && pSoldier->ubID != gusSelectedSoldier && !gfIgnoreOnSelectedGuy )
		{

		}
		else if ( pSoldier->ubID == gusSelectedSoldier && !gRubberBandActive )
		{
			usGraphicToUse = THIRDPOINTERS2;
		}
		// show all people's names if !
		//else if ( pSoldier->ubID >= 20 && pSoldier->bVisible != -1 )
		//{

		//}
		else if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
		{

		}
		else
		{
			return;
		}
	}
	else
	{
		if ( pSoldier->ubID == gusSelectedSoldier && !gRubberBandActive )
		{
			usGraphicToUse = THIRDPOINTERS2;
		}
	}

	// If he is in the middle of a certain animation, ignore!
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_NOSHOW_MARKER )
	{
		return;
	}

	// Donot show if we are dead
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		return;
	}
	
	// anv: if soldier is in vehicle, draw above middle of the vehicle
	if( !(pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
	{
		GetSoldierAboveGuyPositions( pSoldier, &sXPos, &sYPos, FALSE );
	}
	else
	{
		SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
		GetSoldierAboveGuyPositions( pVehicle, &sXPos, &sYPos, FALSE );
	}
	
	// Display name
	SetFont( TINYFONT1 );
	SetFontBackground( FONT_MCOLOR_BLACK );
	
	BOOLEAN bInCombat = gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT;
	BOOLEAN bStealth = pSoldier->bStealthMode || pSoldier->usSoldierFlagMask & ( SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER );
		// sevenfm: for mercs use name color as cover indicator
	// only use color when there is enemy in sector
	if( pSoldier->bTeam == OUR_TEAM &&
		(NumEnemyInSector() != 0) &&
		! ( pSoldier->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_ROBOT) ) &&
		( ( gGameExternalOptions.ubShowCoverIndicator == 1 && ( bInCombat || bStealth ) ) ||
		( gGameExternalOptions.ubShowCoverIndicator == 2 && bStealth ) ) )
	{
		INT8 cover = 3;		// MAX_COVER
		BOOLEAN showCover;
		INT32 usMapPos;
		INT16 color8, color16;

		if( GetMouseMapPos(&usMapPos) && _KeyDown(SHIFT) )
		{
			// target tile cover
			CalculateCoverForSoldier( pSoldier, usMapPos, gsInterfaceLevel, cover );
		}
		else
		{
			// merc's cover
			CalculateCoverForSoldier( pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel, cover );			
		}

		showCover = CoverColorCode( cover, color8, color16 );
		SetFontForeground( (UINT8)color8 );
	}
	else
	{
		SetFontForeground( FONT_MCOLOR_WHITE );
	}

	if ( pSoldier->ubProfile != NO_PROFILE || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		if ( gfUIMouseOnValidCatcher == 1 && pSoldier->ubID == gubUIValidCatcherID )
		{
			swprintf( NameStr, TacticalStr[ CATCH_STR ] );
			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}
		else if ( gfUIMouseOnValidCatcher == 3 && pSoldier->ubID == gubUIValidCatcherID )
		{
			swprintf( NameStr, TacticalStr[ RELOAD_STR ] );
			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}
		else if ( gfUIMouseOnValidCatcher == 4 && pSoldier->ubID == gubUIValidCatcherID )
		{
			swprintf( NameStr, pMessageStrings[ MSG_PASS ] );
			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}
		// anv: if soldier is in vehicle, write its name instead of "vehicle"
		else if ( pSoldier->bAssignment == VEHICLE )
		{
			swprintf( NameStr, L"(%s)", GetSoldierStructureForVehicle( pSoldier->iVehicleId )->GetName() );
			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}
		else if ( pSoldier->bAssignment >= ON_DUTY )
		{
			SetFontForeground( FONT_YELLOW );
			swprintf( NameStr, L"(%s)", pAssignmentStrings[ pSoldier->bAssignment ] );
			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}
		else if ( pSoldier->bTeam == gbPlayerNum &&	pSoldier->bAssignment < ON_DUTY && pSoldier->bAssignment != CurrentSquad() && !( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED ) )
		{
			if ( gGameExternalOptions.fUseXMLSquadNames )
				swprintf( NameStr, SquadNames[ pSoldier->bAssignment ].squadname );
			else
				swprintf( NameStr, gzLateLocalizedString[ 34 ], ( pSoldier->bAssignment + 1 ) );

			FindFontCenterCoordinates( sXPos, (INT16)(sYPos ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
			fRaiseName = TRUE;
		}

		// If not in a squad....
		if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			if ( GetNumberInVehicle( pSoldier->bVehicleID ) == 0 )
			{
				SetFontForeground( FONT_GRAY4 );
			}
		}
		else
		{
			if ( pSoldier->bAssignment >= ON_DUTY && pSoldier->bAssignment != VEHICLE )
			{
				SetFontForeground( FONT_YELLOW );
			}
		}

		if ( fDoName )
		{
			//legion2 jazz
			if ( pSoldier->ubBodyType == ROBOTNOWEAPON && pSoldier->bTeam == ENEMY_TEAM )
			{
				swprintf( NameStr, zGrod[0] );
			}
			//Legion	
			else if ( TANK( pSoldier ) )
			{
				swprintf( NameStr, gNewVehicle[164].NewVehicleStrings );
			}
			else if ( zHiddenNames[pSoldier->ubProfile].Hidden == TRUE )
			{
				swprintf( NameStr, L"???" );
			}
			else
			{
				swprintf( NameStr, L"%s", pSoldier->name );
			}

			INT16 sNameY = fRaiseName ? sYPos - 10 : sYPos;
			FindFontCenterCoordinates( sXPos, sNameY, (INT16)(80), 1, NameStr, TINYFONT1, &sX, &sY );
			gprintfdirty( sX, sY, NameStr );
			mprintf( sX, sY, NameStr );
		}

//		if ( pSoldier->ubProfile < FIRST_RPC || pSoldier->ubProfile >= GASTON || RPC_RECRUITED( pSoldier ) || AM_AN_EPC( pSoldier ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		//new profiles by Jazz	
		if ( gProfilesIMP[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesAIM[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesMERC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || RPC_RECRUITED( pSoldier ) || AM_AN_EPC( pSoldier ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ))			
		{
			if ( gGameExternalOptions.ubShowHealthBarsOnHead )
			{
				if( gGameExternalOptions.ubShowHealthBarsOnHead > 1 )
				{
					INT16 interval = gGameExternalOptions.ubShowHealthBarsOnHead;
					//if( interval > 3 )
					//	interval -= 2;
					sXPos += 28;
					sYPos += 7;
					iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, sXPos, sYPos-1, (INT16)(sXPos + 24 ), (INT16)(sYPos + 3 + interval*3 ) );
					if ( iBack != -1 )
						SetBackgroundRectFilled( iBack );
					DrawBarsInUIBox( pSoldier,	(INT16)(sXPos), (INT16)(sYPos), 20, 1, 2 );

					// show AP only in turnbased combat, only during our turn
					if( gGameExternalOptions.ubShowHealthBarsOnHead > 2 && 
						(gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) &&
						gTacticalStatus.ubCurrentTeam == OUR_TEAM)
					{
						INT16 len;
						swprintf( NameStr,L"%d", pSoldier->bActionPoints );
						len = StringPixLength ( NameStr, TINYFONT1 );
						SetFont( TINYFONT1 );
						if(pSoldier->bActionPoints > APBPConstants[MIN_APS_TO_INTERRUPT])
							SetFontForeground( FONT_MCOLOR_LTGRAY );
						else if(pSoldier->bActionPoints > 10)
							SetFontForeground( FONT_YELLOW );
						else
							SetFontForeground( FONT_MCOLOR_DKRED );
						gprintfdirty( sXPos-2-len, sYPos-2, NameStr );
						mprintf( sXPos-2-len, sYPos-2, NameStr );
					}
				}
				else
				{
					// Adjust for bars!
					if ( pSoldier->ubID == gusSelectedSoldier )
					{
						sXPos += 28;
						sYPos += 5;
					}
					else
					{
						sXPos += 30;
						sYPos += 7;
					}

					// Add bars
					iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 34 ), (INT16)(sYPos + 11 ) );

					if ( iBack != -1 )
					{
						SetBackgroundRectFilled( iBack );
					}
					TileElem = gTileDatabase[ usGraphicToUse ];
					BltVideoObject(	FRAME_BUFFER, TileElem.hTileSurface, TileElem.usRegionIndex, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );

					// Draw life, breath
					// Only do this when we are a vehicle but on our team
					if ( pSoldier->ubID == gusSelectedSoldier )
					{
						DrawBarsInUIBox( pSoldier,	(INT16)(sXPos +1), (INT16)(sYPos + 2), 16, 1, 3);
					}
					else
					{
						DrawBarsInUIBox( pSoldier,	(INT16)(sXPos -1), (INT16)(sYPos ), 16, 1, 3 );
					}
				}
			}
		}
		else // ( pSoldier->ubProfile < FIRST_RPC || pSoldier->ubProfile >= GASTON ||
		{	// RPC_RECRUITED( pSoldier ) || AM_AN_EPC( pSoldier ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			if ( gfUIMouseOnValidCatcher == 2 && pSoldier->ubID == gubUIValidCatcherID )
			{
				SetFont( TINYFONT1 );
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_MCOLOR_WHITE );

				swprintf( NameStr, TacticalStr[ GIVE_STR ] );
				FindFontCenterCoordinates( sXPos, (INT16)(sYPos + 10 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
				gprintfdirty( sX, sY, NameStr );
				mprintf( sX, sY, NameStr );
			}
			else
			{
				SetFont( TINYFONT1 );
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_MCOLOR_DKRED );

				pStr = GetSoldierHealthString( pSoldier );

				FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 10 ), (INT16)(80 ), 1, pStr, TINYFONT1, &sX, &sY );
				gprintfdirty( sX, sY, pStr );
				mprintf( sX, sY, pStr );
			
				//-----------------	
				if ( pSoldier->bInSector && pSoldier->ubProfile == NO_PROFILE )
				{
					if ( pSoldier->bTeam == ENEMY_TEAM )
					{
						// Flugente: soldier profiles
						if ( gGameExternalOptions.fSoldierProfiles_Enemy && pSoldier->usSoldierProfile )
						{					
							swprintf(NameStr, pSoldier->GetName());

							SetFontForeground( FONT_YELLOW );

							FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
							gprintfdirty( sX, sY, NameStr );
							mprintf( sX, sY, NameStr );
						}
						else if (gGameExternalOptions.fEnemyNames == TRUE && gGameExternalOptions.fEnemyRank == FALSE)
						{
							for( iCounter2 = 0; iCounter2 < 500; ++iCounter2 )
							{
								if (zEnemyName[iCounter2].Enabled == 1)
								{
									if ( pSoldier->sSectorX == zEnemyName[iCounter2].SectorX && pSoldier->sSectorY == zEnemyName[iCounter2].SectorY )
									{
										swprintf(NameStr, zEnemyName[iCounter2].szCurGroup);

										SetFontForeground( FONT_YELLOW );
					
										//legion2
										FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
										gprintfdirty( sX, sY, NameStr );
										mprintf( sX, sY, NameStr );
				
										break;
									}
								}
							}
						}
						else if (gGameExternalOptions.fEnemyNames == FALSE && gGameExternalOptions.fEnemyRank == TRUE)
						{
							for( iCounter2 = 1; iCounter2 < 11; ++iCounter2 )
							{
								if (zEnemyRank[iCounter2].Enabled == 1)
								{		
									if ( zEnemyRank[iCounter2].Stats == 0 && pSoldier->stats.bExpLevel == zEnemyRank[iCounter2].ExpLevel )  
									{
										swprintf(NameStr, zEnemyRank[iCounter2].szCurRank);

										SetFontForeground( FONT_YELLOW );
								
										//legion2
										FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
										gprintfdirty( sX, sY, NameStr );
										mprintf( sX, sY, NameStr );

										break;
									}
								}
							}				
						}
					}
					// Flugente: soldier profiles
					else if ( pSoldier->bTeam == MILITIA_TEAM && gGameExternalOptions.fSoldierProfiles_Militia && pSoldier->usSoldierProfile )
					{
						// get a proper chaos name							
						swprintf(NameStr, pSoldier->GetName());

						SetFontForeground( FONT_YELLOW );

						FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
						gprintfdirty( sX, sY, NameStr );
						mprintf( sX, sY, NameStr );
					}
					else if (gGameExternalOptions.fCivGroupName == TRUE && pSoldier->ubCivilianGroup > 0 )
					{
						if (zCivGroupName[pSoldier->ubCivilianGroup].Enabled == 1)
						{	
							swprintf(NameStr, zCivGroupName[pSoldier->ubCivilianGroup].szCurGroup);

							SetFontForeground( FONT_YELLOW );
								
							//legion2
							FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
							gprintfdirty( sX, sY, NameStr );
							mprintf( sX, sY, NameStr );
						}
					}
				}
			//------------
			}
		}
	}
	else //pSoldier->ubProfile != NO_PROFILE || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// show (roof) text
		if ( pSoldier->pathing.bLevel != 0 )
		{
			// sevenfm: fix for overlapping with SHOW_ENEMY_WEAPON feature
			if( !gGameExternalOptions.fShowEnemyWeapon || !gfUIFullTargetFound )
			{
				SetFont( TINYFONT1 );
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_YELLOW );

				swprintf( NameStr, gzLateLocalizedString[ 15 ] );
				FindFontCenterCoordinates( sXPos, (INT16)(sYPos + 10), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
				gprintfdirty( sX, sY, NameStr );
				mprintf( sX, sY, NameStr );
			}
		}

		pStr = GetSoldierHealthString( pSoldier );

		//jones
		extern void SoldierTooltip(SOLDIERTYPE*);
		//if ( gGameExternalOptions.gfAllowSoldierToolTips ) // changed by SANDRO
		if ( gGameSettings.fOptions[TOPTION_ALLOW_SOLDIER_TOOLTIPS] )
			SoldierTooltip(pSoldier);

		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKRED );

		FindFontCenterCoordinates( sXPos, sYPos, (INT16)(80 ), 1, pStr, TINYFONT1, &sX, &sY );
		gprintfdirty( sX, sY, pStr );
		mprintf( sX, sY, pStr );
		
		//-----------------	
		if ( pSoldier->bInSector && pSoldier->ubProfile == NO_PROFILE )
		{
			if ( pSoldier->IsZombie() )
			{
				swprintf(NameStr, pSoldier->name);
							
				// Display name
				SetFontForeground( FONT_MCOLOR_WHITE );
								
				//legion2
				FindFontCenterCoordinates( sXPos, (INT16)( sYPos -10 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
				gprintfdirty( sX, sY, NameStr );
				mprintf( sX, sY, NameStr );
			}
			else if ( pSoldier->bTeam == ENEMY_TEAM )
			{
				// Flugente: soldier profiles
				if ( gGameExternalOptions.fSoldierProfiles_Enemy && pSoldier->usSoldierProfile )
				{
					// get a proper chaos name							
					swprintf(NameStr, pSoldier->GetName());

					SetFontForeground( FONT_YELLOW );

					FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
					gprintfdirty( sX, sY, NameStr );
					mprintf( sX, sY, NameStr );	
				}
				else if (gGameExternalOptions.fEnemyNames == TRUE)
				{
					for( iCounter2 = 0; iCounter2 < 500; ++iCounter2 )
					{
						if (zEnemyName[iCounter2].Enabled == 1 )
						{
							if ( pSoldier->sSectorX == zEnemyName[iCounter2].SectorX && pSoldier->sSectorY == zEnemyName[iCounter2].SectorY )
							{
								swprintf(NameStr, zEnemyName[iCounter2].szCurGroup);

								SetFontForeground( FONT_YELLOW );
								
								//legion2
								FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
								gprintfdirty( sX, sY, NameStr );
								mprintf( sX, sY, NameStr );
		
								break;
							}
						}
					}
				}

				if (gGameExternalOptions.fEnemyRank == TRUE)
				{
					for( iCounter2 = 1; iCounter2 < 11; ++iCounter2 )
					{
						if (zEnemyRank[iCounter2].Enabled == 1)
						{
							if ( zEnemyRank[iCounter2].Stats == 0 && pSoldier->stats.bExpLevel == zEnemyRank[iCounter2].ExpLevel )  
							{
								swprintf(NameStr, zEnemyRank[iCounter2].szCurRank);

								SetFontForeground( FONT_YELLOW );

								// need to adjust sYPos because default position already occupied by the name
								if ( gGameExternalOptions.fSoldierProfiles_Enemy && pSoldier->usSoldierProfile || gGameExternalOptions.fEnemyNames )
									FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 10 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
								// use default position for text
								else
									FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );

								//legion2
								gprintfdirty( sX, sY, NameStr );
								mprintf( sX, sY, NameStr );

								break;
							}
						}
					}
				}

				//FindFontCenterCoordinates( sXPos, (INT16)( sYPos ), (INT16)(80 ), 1, L"", TINYFONT1, &sX, &sY );

				// sevenfm: show weapon name and additional info
				ShowEnemyWeapon( sX, sY, pSoldier );
				ShowEnemyHealthBar( sX, sY, pSoldier );				
				ShowAdditionalInfo( sX, sY, pSoldier );
				ShowRankIcon( sXPos, sYPos, pSoldier );
			}
			// Flugente: soldier profiles
			else if ( pSoldier->bTeam == MILITIA_TEAM && gGameExternalOptions.fSoldierProfiles_Militia && pSoldier->usSoldierProfile )
			{
				// get a proper name							
				swprintf(NameStr, pSoldier->GetName());

				SetFontForeground( FONT_YELLOW );

				FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
				gprintfdirty( sX, sY, NameStr );
				mprintf( sX, sY, NameStr );	
			}
			else if (gGameExternalOptions.fCivGroupName == TRUE && pSoldier->ubCivilianGroup > 0 )
			{
				if (zCivGroupName[pSoldier->ubCivilianGroup].Enabled == 1)
				{	
					swprintf(NameStr, zCivGroupName[pSoldier->ubCivilianGroup].szCurGroup);

					SetFontForeground( FONT_YELLOW );
								
					//legion2
					FindFontCenterCoordinates( sXPos, (INT16)( sYPos + 20 ), (INT16)(80 ), 1, NameStr, TINYFONT1, &sX, &sY );
					gprintfdirty( sX, sY, NameStr );
					mprintf( sX, sY, NameStr );
				}

				// sevenfm: show weapon name and additional info
				FindFontCenterCoordinates( sXPos, (INT16)( sYPos ), (INT16)(80 ), 1, L"", TINYFONT1, &sX, &sY );
				ShowEnemyWeapon( sX, sY, pSoldier );
			}
		}
	}
}

BOOLEAN DrawCTHIndicator()
{
	////////////////////////////////////////////
	// HEADROCK HAM 4: NCTH Indicator
	//
	// The NCTH shooting system is radically different from the old CTH, and as a result it requires a completely
	// new indicator to show hit-probability. The "Hit Percentage" bar we've all gotten used to in JA2 1.13 is not
	// doable in NCTH since the hit probability is unknown before the bullet reaches the target distance. Instead,
	// we use a sort of "reticle" to show the margin of error for our current shot.
	//
	// The CTH indicator is made up of several parts:
	//   1. Outer Circle: This circle shows the margin of error that we would have if Muzzle Accuracy was 0%. In other
	//      words, it indicates the "worst" possible shot with the gun we're holding, given the target's distance.
	//      This circle only gets larger or smaller if the shooter moves closer or further from the target (or
	//      changes weapons...).
	//   2. Inner Circle+Crosshairs: This indicator shows the margin of error for the shot given all the bonuses from
	//      Base CTH and extra aiming clicks. It is a visual representation of where our bullet may end up once fired.
	//      The bullet can end up anywhere within this smaller circle. The circle gets larger or smaller as we change
	//      the number of applied aiming clicks, add magnification bonuses from a scope, change stance, etcetera.
	//   3. Additional indicators: These are floating graphical and numerical indicators showing various important
	//      data, such as how many aiming clicks we've applied (and how many we can apply), how many bullets we're
	//      going to fire (in burst/auto mode), and how many APs it's going to cost us. These are generally just
	//      relocation and redesign of existing indicators on the OCTH cursor.

	if (gCTHDisplay.MuzzleSwayPercentage < 0 || gCTHDisplay.MuzzleSwayPercentage > 99 || gCTHDisplay.FinalMagFactor < 1.0)
	{
		// Obviously Uninitialized.
		return 0;
	}

	INT32			iBack;

	// Find the shooter.
	SOLDIERTYPE *pSoldier;
	GetSoldier( &pSoldier, gusSelectedSoldier );

	OBJECTTYPE* pWeapon = pSoldier->GetUsedWeapon( &pSoldier->inv[ pSoldier->ubAttackingHand ] );

	// Create a Background Rect for us to draw our indicator on. With NCTH, the size and position of this rectangle
	// is equal exactly to the size of the tactical screen viewport. Unlike the OCTH indicator, the NCTH one can grow
	// so large as to fill the entire screen.
	INT16 sLeft = gsVIEWPORT_START_X;
	INT16 sTop = gsVIEWPORT_WINDOW_START_Y;
	INT16 sRight = gsVIEWPORT_END_X;
	INT16 sBottom = gsVIEWPORT_WINDOW_END_Y;

	iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, sLeft, sTop, sRight, sBottom);

	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: New CTH, Aperture Size
	//
	// As explained above, the most important part of the new indicator is the Aperture circle/crosshairs.
	// These indicate the size of the "margin of error" for our current shot. The more accurate we are,
	// the smaller the circle will be.
	// 
	// The player uses this to determine whether the shot has a good likelyhood to hit the target.
	// If the circle is the same size or smaller than the target itself, then hit probability is 100%.
	// The larger it gets, the less hit probability there will be. When adding extra aiming clicks,
	// the circle gets proportionally smaller. The idea is to try to make it as small as the target
	// (if possible) to ensure a hit.
	//
	// Shot aperture is calculated using the CTH formula (CalcChanceToHitGun). Scopes and lasers
	// can help decrease the size of the aperture further.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////
	// Define colors
	UINT8 ColorsRed[10] = { 255, 255, 255, 255, 255, 255, 255, 255, 173, 0 };
	UINT8 ColorsGreen[10] = { 0, 50, 81, 101, 133, 174, 211, 243, 255, 255 };

	// Select the color of the indicator circles
	UINT16 usCApertureBar;

	// CHRISL: Instead of shigting the color based on MuzzleSwayPercent, let's move this line and base it on the final iAperture value
	// Indicator changes color from red to yellow to green, based on how well the gun is aimed.
	//usCApertureBar		= Get16BPPColor( FROMRGB( ColorsRed[gCTHDisplay.MuzzleSwayPercentage/10], ColorsGreen[gCTHDisplay.MuzzleSwayPercentage/10], 0 ) ); // Crosshair color shifts from red (longshot) to green (Accurate shot)

	UINT16 usCApertureBorder	= Get16BPPColor( FROMRGB( 10, 10, 10 ) ); // Boundaries in dark color.

	//////////////////////////////////
	// Calculate Aperture

	// Calculate the center point of the shooter, in world coordinates.
	FLOAT dStartX = (FLOAT) CenterX( gCTHDisplay.iShooterGridNo );
	FLOAT dStartY = (FLOAT) CenterY( gCTHDisplay.iShooterGridNo );

	// Calculate the center point of the target, in world coordinates.
	FLOAT dEndX = (FLOAT) CenterX( gCTHDisplay.iTargetGridNo );
	FLOAT dEndY = (FLOAT) CenterY( gCTHDisplay.iTargetGridNo );

	// Calculate a delta: the difference between the shooter and target.
	FLOAT dDeltaX = dEndX - dStartX;
	FLOAT dDeltaY = dEndY - dStartY;

	// Calculate the distance of the shot, using Pythagorean Theorem
	DOUBLE d2DDistance = Distance2D( dDeltaX, dDeltaY );
	// Round it upwards.
	INT32 iDistance = (INT32) d2DDistance;
	if ( d2DDistance != iDistance )
	{
		iDistance += 1;
		d2DDistance = (FLOAT) ( iDistance);
	}

	///////////////////////////////////////////////////////////
	// Now we calculate the Aperture size. This is done using the same method as the shooting formula uses.

	// Calculate the size of a "normal" aperture. This is how wide a shot can go at 1x Normal Distance.
	FLOAT iBasicAperture = CalcBasicAperture() * 2; // The *2 compensates for the difference between CellXY and ScreenXY 

	// sevenfm: moved declarations out of codeblock, will use them later for laser dot plotting
	FLOAT fLaserBonus = 0;	
	FLOAT fBrightnessModifier = 1;	
	FLOAT fEffectiveLaserRatio = 1;

	// when using the reworked NCTH code we do additional calculations for iron sights and lasers
	if (gGameExternalOptions.fUseNewCTHCalculation)
	{
		// silversurfer: New functionality for iron sights - There have been many complaints that iron sights lose their usefulness
		// very fast the farther the target is away. Setting IRON_SIGHT_PERFORMANCE_BONUS too high makes them overly powerful at
		// close range. This experimental formula implements a curve that lowers iBasicAperture the farther the target is away.
		// At 1 tile distance iBasicAperture will be the same as before. That's the common start.
		if ( gGameCTHConstants.IRON_SIGHTS_MAX_APERTURE_USE_GRADIENT && gCTHDisplay.ScopeMagFactor <= 1.0 && !pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, gCTHDisplay.iTargetGridNo ) )

			iBasicAperture = iBasicAperture * ( 1 / sqrt( d2DDistance / FLOAT(CELL_X_SIZE) ) / gGameCTHConstants.IRON_SIGHTS_MAX_APERTURE_MODIFIER
							+ (gGameCTHConstants.IRON_SIGHTS_MAX_APERTURE_MODIFIER - 1) / gGameCTHConstants.IRON_SIGHTS_MAX_APERTURE_MODIFIER );

		// iron sights can get a percentage bonus to make them overall better but only when not shooting from hip
		if ( gCTHDisplay.ScopeMagFactor <= 1.0 && !pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, gCTHDisplay.iTargetGridNo ) )

			iBasicAperture = iBasicAperture * (FLOAT)( (100 - gGameCTHConstants.IRON_SIGHT_PERFORMANCE_BONUS) / 100);

		// laser pointers can provide a percentage bonus to base aperture
		if ( gCTHDisplay.iBestLaserRange > 0 
			&& ( gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0) )
		{
			INT8 bLightLevel = LightTrueLevel(gCTHDisplay.iTargetGridNo, gsInterfaceLevel );
			INT32 iMaxLaserRange = ( gCTHDisplay.iBestLaserRange*( 2*bLightLevel + 3*NORMAL_LIGHTLEVEL_NIGHT - 5*NORMAL_LIGHTLEVEL_DAY ) ) / ( 2 * ( NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY ) );

			// laser only has effect when in range
			if ( iMaxLaserRange > d2DDistance )
			{
				// which bonus do we want to apply?
				if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, gCTHDisplay.iTargetGridNo ) )
					// shooting from hip
					fLaserBonus = gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP;
				else if ( gCTHDisplay.ScopeMagFactor <= 1.0 )
					// using iron sights or other 1x sights
					fLaserBonus = gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON;
				else
					// must be using a scope
					fLaserBonus = gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE;

				// light level influences how easy it is to spot the laser dot on the target
				fBrightnessModifier = (FLOAT)(bLightLevel) / (FLOAT)(NORMAL_LIGHTLEVEL_NIGHT);

				// laser fully efficient
				if ( gCTHDisplay.iBestLaserRange > d2DDistance )
					// apply full bonus
					iBasicAperture = iBasicAperture * (FLOAT)( (100 - (fLaserBonus * fBrightnessModifier)) / 100);
				else
				{
					// beyond BestLaserRange laser bonus drops linearly to 0
					fEffectiveLaserRatio = (FLOAT)(iMaxLaserRange - d2DDistance) / (FLOAT)(iMaxLaserRange - gCTHDisplay.iBestLaserRange);
					// apply partial bonus
					iBasicAperture = iBasicAperture * (FLOAT)( (100 - (fLaserBonus * fBrightnessModifier * fEffectiveLaserRatio)) / 100);
				}
			}
		}
	}

	// Next, find out how large the aperture can be around the target, given range. The further the target is, the
	// larger the aperture can be.
	FLOAT iDistanceAperture = iBasicAperture * (d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);

	///////////////////////////////////////////////////////////////
	// To make things easier for us, we now calculate the Magnification Factor for this shot. A Mag Factor
	// is a divisor to the sway of the muzzle. It's about the same as multiplying CTH by a certain amount.
	// Note that both optical magnification devices (like scopes) and dot-projection devices (like lasers and 
	// reflex sights) provide this sort of bonus.
	FLOAT iMagFactor = CalcMagFactor( pSoldier, pWeapon, d2DDistance, gCTHDisplay.iTargetGridNo, (UINT8)pSoldier->aiData.bAimTime );

	// Get effective mag factor for this shooter. This represents his ability to use scopes.
	FLOAT fEffectiveMagFactor = CalcEffectiveMagFactor( pSoldier, iMagFactor );

	// Next step is to apply scope/projection factor to decrease the size of the aperture. This gives us the "Max
	// Aperture" value - the size of the shooting circle if the gun is as unstable as possible.
	FLOAT iMaxAperture = iDistanceAperture / fEffectiveMagFactor;

	// We now use the Muzzle Sway value, calculated by the CTH formula, to decrease the size of the shot aperture.
	// It is used as a percentage: a 50% muzzle sway value gives a cone with half the maximum radius. A cone with
	// 0% Muzzle Sway is a single line with no width (meaning all shots will fly right down the center, and all will
	// hit the target), while a cone with 100% muzzle sway is as wide as possible.
	FLOAT iAperture = ( iMaxAperture * (100.0f - (FLOAT)gCTHDisplay.MuzzleSwayPercentage) / 100.0f);

	/////////////////////////////////////////////
	// Factor in Weapon "Effective Range".
	UINT16 sEffRange = Weapon[Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubClassIndex].usRange + GetRangeBonus(&(pSoldier->inv[ pSoldier->ubAttackingHand ]));
	FLOAT iRangeRatio = __max(1.0f, (FLOAT)(d2DDistance / sEffRange));
	FLOAT iDistanceRatio = (FLOAT)(d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);
	
	/////////////////////////////////////////////
	// Factor in Gun Accuracy.
	
	INT16 sAccuracy = GetGunAccuracy( &(pSoldier->inv[ pSoldier->ubAttackingHand ]) );

	sAccuracy = __max(0, sAccuracy);
	sAccuracy = __min(100, sAccuracy);

	FLOAT iBulletDev = (gGameCTHConstants.MAX_BULLET_DEV * (100-sAccuracy)) / 100;
	if( gGameCTHConstants.RANGE_EFFECTS_DEV == TRUE )
		iBulletDev *= iRangeRatio;
	iBulletDev *= iDistanceRatio;

	// Since bullet dev is only affected by distance, we add it last as a flat modifier.
	iBasicAperture += iBulletDev;
	iDistanceAperture += iBulletDev;
	iAperture += iBulletDev;

	// CHRISL: Moved here so we can base the cursor color on the iAperture value
	UINT8 iColorCode = 0;
	if(iAperture < 3.0)
		iColorCode = 9;
	else if(iAperture < 6.0)
		iColorCode = 8;
	else if(iAperture < 9.0)
		iColorCode = 7;
	else if(iAperture < 12.0)
		iColorCode = 6;
	else if(iAperture < 16.0)
		iColorCode = 5;
	else if(iAperture < 20.0)
		iColorCode = 4;
	else if(iAperture < 25.0)
		iColorCode = 3;
	else if(iAperture < 30.0)
		iColorCode = 2;
	else if(iAperture < 40.0)
		iColorCode = 1;
	usCApertureBar		= Get16BPPColor( FROMRGB( ColorsRed[iColorCode], ColorsGreen[iColorCode], 0 ) ); // Crosshair color shifts from red (longshot) to green (Accurate shot)

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: Calculate Screen Positions
	//
	// Aperture Size has to be translated into on-screen size. We also want to track the position of the
	// mouse so that our indicator moves with it like a cursor.

	// Z Offset is used to move the indicator up or down. It's currently set to 0 since it's not required ATM.
	FLOAT zOffset = 0;

	// Calculate the position of the mouse cursor and store it.
	INT16 sStartScreenX;
	INT16 sStartScreenY;

	POINT	MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!	

	sStartScreenX = (INT16)MousePos.x - 1;	// sevenfm: fix (-1) for cursor mismatch
	sStartScreenY = (INT16)MousePos.y - 1;

	// Define regions for the various indicators. We need to do this now because the Aperture Circles will want to
	// avoid being drawn within these regions, otherwise they might make them harder to see.

	MagRect.left = sStartScreenX-50;	MagRect.top = sStartScreenY-15;		MagRect.right = sStartScreenX-20;		MagRect.bottom = sStartScreenY-5;
	APRect.left = sStartScreenX+22;		APRect.top = sStartScreenY-15;		APRect.right = sStartScreenX+52;		APRect.bottom = sStartScreenY-5;
	AimRect.left = sStartScreenX-35;	AimRect.top = sStartScreenY+22;		AimRect.right = sStartScreenX+35;		AimRect.bottom = sStartScreenY+30;
	ModeRect.left = sStartScreenX-28;	ModeRect.top = sStartScreenY+31;	ModeRect.right = sStartScreenX+28;		ModeRect.bottom = sStartScreenY+41;

	////////////////////////////////////////////////////////////////////////////////////////
	// DRAW ADDITIONAL COMPONENTS:
	//
	// We now draw additional indicators on top of the circles. There are four indicators:
	//
	// 1. AP Cost: The current AP cost for the shot has been moved from the center of the
	//    targeting cursor to avoid obscuring the meeting point of the aperture crosshairs.
	//    It is now displayed in the top right, together with the word "AP".
	// 2. MAG Factor: The current Magnification or Projection factor for the shot is displayed
	//    in the top-left, followed by an "x". Its color indicates whether we are using the
	//    full potential of our aiming device.
	// 3. Aiming Pips: The circles on the old indicator shows how many aiming clicks we've
	//    placed into the shot. Now that the circles are used to indicate shot probability,
	//    the aiming clicks are displayed with a series of pips below the targeting cursor
	//    which fill up as we add levels.
	// 4. Bullet Count: This indicator, now below the Aiming Pips indicator, shows the number
	//    of bullets queues up for a Burst/Autofire volley. For autofire, it starts out by
	//    displaying grey bullets showing how many we CAN add, then fills up with colored
	//    bullets as we add them to the volley. A "+" sign is added if space for bullets runs out.

	CHAR16 pStr[10];

	INT16 curX = 0;
	INT16 curY = 0;

	/////////////////////////////////////////
	// Load image STI
	UINT32	guiCTHImage;
	VOBJECT_DESC	VObjectDesc;

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	if(UsingNewCTHSystem() == true)
		FilenameForBPP("CURSORS\\CUR_CTH_NCTH.sti", VObjectDesc.ImageFile);
	else
		FilenameForBPP("CURSORS\\CUR_CTH.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCTHImage));

	/////////////// AP COST FOR CURRENT SHOT
	// sevenfm: only if in turnbased!
	if( ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) ) )
	{
		// Create a pointer to the Frame Buffer which we are going to draw directly into.
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		// Find coordinates, using the full string ("XX AP")
		swprintf( pStr, L"%d %s", gsCurrentActionPoints, gzNCTHlabels[ 1 ] );
		FindFontCenterCoordinates( (INT16)APRect.left, (INT16)APRect.top, (INT16)APRect.right-(INT16)APRect.left, 10, pStr, TINYFONT1, &curX, &curY);
		// Find width of this string.
		UINT16 usTotalWidth = StringPixLength ( pStr, TINYFONT1 );

		// Cut string to just the number of APs
		swprintf( pStr, L"%d", gsCurrentActionPoints );
		// Find out how wide this string is
		UINT16 usWidth = StringPixLength( pStr, TINYFONT1 );
		// Draw to screen
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		// Draw the "AP" label 4 pixels forward.
		SetFontForeground(FONT_MCOLOR_LTYELLOW);
		swprintf( pStr, L"%s", gzNCTHlabels[ 1 ] );
		curX += usWidth + 4;

		// sevenfm: improved NCTH indicator - change color of 'AP' depending on the remaining APs after shooting
		NCTHImprovedAPColor( pSoldier, pWeapon );
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		// Redefine area size based on total width plus a margin of 2 pixels.
		usTotalWidth = usTotalWidth + 6;
		INT16 sCenter = (INT16)((APRect.right + APRect.left) / 2);
		APRect.left = sCenter - (usTotalWidth / 2);
		APRect.right = sCenter + (usTotalWidth / 2);
	}
	else
	{
		APRect.top = APRect.bottom = 0;
		APRect.left = APRect.right = 0;
	}

	// sevenfm: draw item pics
	DrawNCTHCursorItemPics( sStartScreenX, sStartScreenY );

	/////////////// MAG FACTOR
	{
		SetFont( TINYFONT1 );

		// Find coordinates, using full string ("X.X x")
		swprintf( pStr, L"%1.1f x", gCTHDisplay.FinalMagFactor );
		FindFontCenterCoordinates( (INT16)MagRect.left, (INT16)MagRect.top, (INT16)MagRect.right-(INT16)MagRect.left, 10, pStr, TINYFONT1, &curX, &curY);
		// Find width of this string.
		UINT16 usTotalWidth = StringPixLength ( pStr, TINYFONT1 );

		// Cut string to just the float
		swprintf( pStr, L"%1.1f", gCTHDisplay.FinalMagFactor );
		// Record the width of the float
		UINT16 usWidth = StringPixLength( pStr, TINYFONT1 );

		////////////////////////////
		// We change the color of this indicator based on whether or not our optical devices are working
		// at full potential. Remember that scopes below their best range give out penalties!

		// If using a scope below its optimal range...
		if (gCTHDisplay.ScopeMagFactor > 1.0 && gCTHDisplay.ScopeMagFactor > gCTHDisplay.FinalMagFactor)
		{
			// If within projection range...
			if (gCTHDisplay.ProjectionFactor > 1.0 && gCTHDisplay.ProjectionFactor >= iDistanceRatio )
			{
				SetFontForeground( FONT_ORANGE );
			}
			// If only using a scope...
			else
			{
				SetRGBFontForeground( 255, 150, 150 ); // Bright Pink
			}
		}
		// If using a scope above its range, or a laser below its range
		else if ((gCTHDisplay.ScopeMagFactor > 1.0 && gCTHDisplay.ScopeMagFactor <= iDistanceRatio) ||
				(gCTHDisplay.ProjectionFactor > 1.0 && gCTHDisplay.ProjectionFactor >= iDistanceRatio) )
		{
			SetRGBFontForeground( 100,255,100 ); // True Light Green
		}
		// In all other circumstances...
		else
		{
			SetFontForeground( FONT_MCOLOR_WHITE );
		}

		// Print the float
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		if( gGameExternalOptions.ubImprovedNCTHCursor > 0 && Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN )
		{
			// sevenfm: show scope mode icon
			NCTHDrawScopeModeIcon( pSoldier, curX + usWidth + 2, curY );
		}
		else
		{
			// Add an "x" 4 pixels forward.
			SetFontForeground( FONT_MCOLOR_LTYELLOW );
			swprintf( pStr, L"x" );
			curX += usWidth + 4;
			gprintfdirty( curX, curY, pStr );
			mprintf( curX, curY, pStr);
		}

		// Redefine area size based on total width plus a margin of 2 pixels.
		usTotalWidth = usTotalWidth + 6;
		INT16 sCenter = (INT16)((MagRect.right + MagRect.left) / 2);
		MagRect.left = sCenter - (usTotalWidth / 2);
		MagRect.right = sCenter + (usTotalWidth / 2);
	}
	
#ifdef JA2BETAVERSION
	/////////////// SHOT APERTURE SIZE
	{
		// Create a pointer to the Frame Buffer which we are going to draw directly into.
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		// Find coordinates, using the full string ("XX AP")
		swprintf( pStr, L"%3.2f", iAperture );
		FindFontCenterCoordinates( (INT16)APRect.left, (INT16)APRect.top-5, (INT16)APRect.right-(INT16)APRect.left, 5, pStr, TINYFONT1, &curX, &curY);
		// Find width of this string.
		UINT16 usTotalWidth = StringPixLength ( pStr, TINYFONT1 );

		// Draw to screen
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		// Redefine area size based on total width plus a margin of 2 pixels.
		usTotalWidth = usTotalWidth + 6;
		INT16 sCenter = (INT16)((APRect.right + APRect.left) / 2);
		APRect.left = sCenter - (usTotalWidth / 2);
		APRect.right = sCenter + (usTotalWidth / 2);
	}

	/////////////// DISTANCE APERTURE SIZE
	{
		SetFont( TINYFONT1 );

		// Find coordinates, using full string ("X.X x")
		swprintf( pStr, L"%3.2f", iDistanceAperture );
		FindFontCenterCoordinates( (INT16)MagRect.left, (INT16)MagRect.top-5, (INT16)MagRect.right-(INT16)MagRect.left, 5, pStr, TINYFONT1, &curX, &curY);
		// Find width of this string.
		UINT16 usTotalWidth = StringPixLength ( pStr, TINYFONT1 );

		// Draw to screen
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		// Redefine area size based on total width plus a margin of 2 pixels.
		usTotalWidth = usTotalWidth + 6;
		INT16 sCenter = (INT16)((MagRect.right + MagRect.left) / 2);
		MagRect.left = sCenter - (usTotalWidth / 2);
		MagRect.right = sCenter + (usTotalWidth / 2);
	}
#endif

	//////////////////// BURST/AUTO LABEL AND BULLETS
	if (pSoldier->bDoBurst)
	{
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		////////////////////////////////////////////////////////////////////////////////
		// We need to calculate how many bullets to display, and how much space they
		// take, so that we can center them horizontally on the CTH indicator.
		// This works slightly differently for Autofire and Burst.

		UINT8 ubNumBullets = 0;
		UINT8 ubBulletWidth = 4;
		UINT8 ubTotalWidth = 0;
		UINT32 uiMaxAutofire = 0;
		UINT32 uiMaxBulletsToDisplay = 0;
		UINT32 uiSpacesToDisplay = 0;

		if (pSoldier->bDoAutofire)
		{
			// Number of COLORED bullets to display is the number of bullets we want to fire.
			ubNumBullets = pSoldier->bDoAutofire;

			// How many bullets are left in the gun?
			UINT32 uiBulletsLeft = pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft;
			if (pSoldier->IsValidSecondHandBurst()) 
			{
				uiBulletsLeft = min( (pSoldier->inv[ SECONDHANDPOS ][0]->data.gun.ubGunShotsLeft), uiBulletsLeft );
			}

			UINT32 uiCurBullet = uiBulletsLeft;
			while( uiCurBullet > 0 )
			{
				// We work backwards from the total number of bullets left, to find out the number of bullets
				// that can be fired given the current number of APs left.

				// Store Soldier Autofire variable into a temp. We do this to fool the AP Calculation formula.
				UINT8 TempDoAutofire = pSoldier->bDoAutofire;
				pSoldier->bDoAutofire = uiCurBullet;

				//Get AP cost to fire this many bullets
				INT16 sAPCosts = CalcTotalAPsToAttack( pSoldier, gCTHDisplay.iTargetGridNo, TRUE, pSoldier->aiData.bShownAimTime);

				// Switch back.
				pSoldier->bDoAutofire = TempDoAutofire;

				// Have we reached a number of bullets that can be fired with the current APs?
				if( !uiMaxAutofire && EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					// Set it.
					uiMaxAutofire = uiCurBullet;
				}

				//Decrement until zero.
				uiCurBullet--;
			}

			// Display no more than X bullets on screen!
			// sevenfm: limit displayed number of bullets to 10
			if( gGameExternalOptions.ubImprovedNCTHCursor > 1 )	
				uiMaxBulletsToDisplay = __min(uiMaxAutofire, 10); 
			else
				uiMaxBulletsToDisplay = __min(uiMaxAutofire, 15);

			// Calculate how many spaces to display. Currently, a space is displayed after every 5 bullets.
			uiSpacesToDisplay = (uiMaxBulletsToDisplay-1) / 5;

			// Calculate the total width of this display in pixels. Spaces and bullets have the same width.
			ubTotalWidth = ubBulletWidth * (uiMaxBulletsToDisplay + uiSpacesToDisplay);
		}
		else
		{
			// Number of COLORED bullets to display is the number of bullets we want to fire.
			ubNumBullets = GetShotsPerBurst( &(pSoldier->inv[ pSoldier->ubAttackingHand ]) );

			// Display no more than X bullets on screen!
			uiMaxBulletsToDisplay = __min(ubNumBullets, 15);

			// Calculate how many spaces to display. Currently, a space is displayed after every 5 bullets.
			uiSpacesToDisplay = (uiMaxBulletsToDisplay-1) / 5;

			// Calculate the total width of this display in pixels. Spaces and bullets have the same width.
			ubTotalWidth = ubBulletWidth * (uiMaxBulletsToDisplay + uiSpacesToDisplay);
		}

		// Tracks the number of spaces we've drawn on screen so far.
		UINT16 usNumSpacesShown = 0;

		// AUTO FIRE
		if (pSoldier->bDoAutofire)
		{
			// Draw greyed-out bullets, indicating how many bullets CAN be added to the volley given our
			// current remaining APs.
			for (UINT8 x = 0; x < (uiMaxBulletsToDisplay); x++)
			{
				// Find starting point for drawing
				INT16 sLeft = (((INT16)ModeRect.left + (INT16)ModeRect.right)/2) - (ubTotalWidth/2);
				INT16 sTop = (INT16)ModeRect.top;

				// If we've drawn 5,10,15,etc. bullets, add a space.
				if (x > 0 && x%5 == 0)
				{
					usNumSpacesShown++;
				}

				// Offset adds up the number of bullets and spaces we've already drawn.
				INT16 sCurOffset = ubBulletWidth * (x+usNumSpacesShown);
				
				// Draw a bullet here!
				BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 8, sLeft + sCurOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );

				// If the number of bullets that can be fired is greater than the number of bullets we can display,
				// and we've just now drawn the last displayable bullet, then...
				if (x == uiMaxBulletsToDisplay-1 && uiMaxBulletsToDisplay < uiMaxAutofire)
				{
					// Draw a shaded plus sign at the end.
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 9, sLeft + sCurOffset + ubBulletWidth, sTop+2, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}

			// Draw colored bullets. These show how many bullets we've set to fire with the current volley.
			usNumSpacesShown = 0;
			for (UINT8 x = 0; x < ubNumBullets; x++)
			{
				// Set start point for drawing
				INT16 sLeft = (((INT16)ModeRect.left + (INT16)ModeRect.right)/2) - (ubTotalWidth/2);
				INT16 sTop = (INT16)ModeRect.top;

				// Calculate offset based on the number of bullets and spaces already drawn.
				INT16 sCurOffset = ubBulletWidth * (x+usNumSpacesShown);

				// If we're drawing more bullets than can be displayed on-screen...
				if (x == uiMaxBulletsToDisplay)
				{
					// Draw a colored plus sign at the end.
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 10, sLeft + sCurOffset, sTop+2, VO_BLT_SRCTRANSPARENCY, NULL );
					// Set x to NumBullets to end the loop
					x = ubNumBullets;
				}
				else
				{
					// If we've drawn 5,10,15,etc. bullets, increase the number of spaces drawn.
					if (x > 0 && x%5 == 0)
					{
						usNumSpacesShown++;
					}
					// Recalculate offset to take any new spaces into account.
					INT16 sCurOffset = ubBulletWidth * (x+usNumSpacesShown);
					// Draw a colored bullet here.
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 7, sLeft + sCurOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}

			// Redefine area size based on total width plus a margin of 2 pixels.
			ubTotalWidth = ubTotalWidth + 6;
			INT16 sCenter = (INT16)((ModeRect.right + ModeRect.left) / 2);
			ModeRect.left = sCenter - (ubTotalWidth / 2);
			ModeRect.right = sCenter + (ubTotalWidth / 2);

		}
		// BURST FIRE
		else
		{
			// Draw colored bullets. These show how many bullets we've set to fire with the current burst.
			usNumSpacesShown = 0;
			UINT16 usTotalWidth = 0;
			UINT8 x;
			for (x = 0; x < ubNumBullets; x++)
			{
				// Set start point for drawing
				INT16 sLeft = (((INT16)ModeRect.left + (INT16)ModeRect.right)/2) - (ubTotalWidth/2);
				INT16 sTop = (INT16)ModeRect.top;

				// Calculate offset based on the number of bullets and spaces already drawn.
				INT16 sCurOffset = ubBulletWidth * (x+usNumSpacesShown);

				// If we're drawing more bullets than can be displayed on-screen...
				if (x == uiMaxBulletsToDisplay)
				{
					// Draw a colored plus sign at the end.
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 10, sLeft + sCurOffset, sTop+2, VO_BLT_SRCTRANSPARENCY, NULL );
					// Set x to NumBullets to end the loop
					x = ubNumBullets;
					usTotalWidth += 5;
				}
				else
				{
					// If we've drawn 5,10,15,etc. bullets, increase the number of spaces drawn.
					if (x > 0 && x%5 == 0)
					{
						usNumSpacesShown++;
					}
					// Recalculate offset to take any new spaces into account.
					INT16 sCurOffset = ubBulletWidth * (x+usNumSpacesShown);
					// Draw a colored bullet here.
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 7, sLeft + sCurOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
				}
			}
			usTotalWidth += ubBulletWidth * ((x+1)+usNumSpacesShown)+4;
			INT16 sCenter = (INT16)((ModeRect.right + ModeRect.left) / 2);
			ModeRect.left = sCenter - (ubTotalWidth / 2);
			ModeRect.right = sCenter + (ubTotalWidth / 2);
		}
	}

	/////////////// SINGLE-SHOT.
	else
	{
		// Draw a label that says "SINGLE" instead of bullets.
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		swprintf( pStr, gzNCTHlabels[ 0 ] );
		FindFontCenterCoordinates( (INT16)ModeRect.left, (INT16)ModeRect.top, (INT16)ModeRect.right-(INT16)ModeRect.left, 10, pStr, TINYFONT1, &curX, &curY);
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		UINT16 usTotalWidth = StringPixLength( pStr, TINYFONT1 ) + 6;
		INT16 sCenter = (INT16)((ModeRect.right + ModeRect.left) / 2);
		ModeRect.left = sCenter - (usTotalWidth / 2);
		ModeRect.right = sCenter + (usTotalWidth / 2);
	}

	/////////////// AIM LEVELS
	{
		// Calculate number of allowed aiming levels on this gun.
		INT8 ubAllowedLevels = AllowedAimingLevels( pSoldier, gCTHDisplay.iTargetGridNo );

		// Number of ticks to display, and the number of spaces between them.
		UINT8 ubNumTicks = ((ubAllowedLevels-1)*2)+1;
		UINT8 ubNumSpaces = ubNumTicks-1;

		// Width of the tick image plus (or minus) a mandatory margin.
		UINT8 ubAimTickWidth = 5;

		// Total width of the aiming display if using no extra spacing and MAXIMUM allowed aiming levels (currently 8)
		UINT8 ubAimMaxTotalWidth = ubAimTickWidth * ((8 * 2)-1);

		// Total width of the aiming display if using no extra spacing and as many levels as allowed by the gun.
		UINT8 ubAimPlainWidth = ubAimTickWidth * ubNumTicks;

		// Find the largest space width possible without exceeding the maximum width. We do this by
		// finding the difference between MaxTotal width and Plain width, and dividing it by the number of spaces
		// we've got.
		UINT8 ubAimFinalOffset = 0;
		UINT8 ubAimFinalWidth = 0;
		if (ubNumSpaces > 0)
		{
			UINT8 ubAimBestOffset = (ubAimMaxTotalWidth - ubAimPlainWidth) / ubNumSpaces;

			// Final offset is whichever is smallest - the largest width, or the width of a single tick.
			ubAimFinalOffset = __min(ubAimBestOffset, ubAimTickWidth);
		}
		// else leave at 0

		ubAimFinalOffset += ubAimTickWidth;

		// Final width of the entire display, taking all ticks and spaces into account.
		ubAimFinalWidth = ubAimTickWidth + (ubAimFinalOffset * ubNumSpaces);

		// Find offset from center.
		UINT8 ubAimLeftOffset = ubAimFinalWidth / 2;

		// Find starting coordinates for this display
		sTop = (INT16)AimRect.top;
		sLeft = (INT16)((AimRect.left+AimRect.right)/2) - ubAimLeftOffset;

		if( gGameExternalOptions.ubImprovedNCTHCursor > 1 )
		{		// sevenfm: show aim levels as numbers
			NCTHShowAimLevels( pSoldier, curX, curY );
		}
		else	// draw ticks
		{
			// Draw empty ticks.
			for (UINT8 x = 0; x < (ubAllowedLevels*2)-1; x++)
			{
				INT16 sOffset = x * ubAimFinalOffset;
				if (gfDisplayFullCountRing)
				{
					if (ubAllowedLevels - abs((ubAllowedLevels-(x+1))) >= pSoldier->aiData.bShownAimTime)
					{
						// Red empty tick - unusable aim level with current AP!
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 6, sLeft + sOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						// Orange empty tick - Used aim tick.
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 4, sLeft + sOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
				else
				{
					if (pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bShownAimTime, gCTHDisplay.iTargetGridNo ) && 
						ubAllowedLevels - abs((ubAllowedLevels-(x+1))) <= GetNumberAltFireAimLevels( pSoldier, gCTHDisplay.iTargetGridNo ) )
					{
						// yellow empty tick
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 2, sLeft + sOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						// grey empty tick
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 0, sLeft + sOffset, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
			}

			// Draw Filled Ticks
			if (pSoldier->aiData.bShownAimTime)
			{
				// Tick on the left
				INT16 sNewLeft = sLeft + (ubAimFinalOffset * (pSoldier->aiData.bShownAimTime-1));
				if (gfDisplayFullCountRing)
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bShownAimTime, gCTHDisplay.iTargetGridNo ) )
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 3, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 5, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
				else
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bShownAimTime, gCTHDisplay.iTargetGridNo ) )
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 3, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 1, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}

				// Tick on the right
				sNewLeft = sLeft + (ubAimFinalOffset * (ubNumSpaces-(pSoldier->aiData.bShownAimTime-1)));
				if (gfDisplayFullCountRing)
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bShownAimTime, gCTHDisplay.iTargetGridNo ) )
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 3, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 5, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
				else
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bShownAimTime, gCTHDisplay.iTargetGridNo ) )
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 3, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
					else
					{
						BltVideoObjectFromIndex( FRAME_BUFFER, guiCTHImage, 1, sNewLeft, sTop, VO_BLT_SRCTRANSPARENCY, NULL );
					}
				}
			}

			ubAimFinalWidth += 6;
			INT16 sCenter = (INT16)((AimRect.right + AimRect.left) / 2);
			AimRect.left = sCenter - (ubAimFinalWidth / 2);
			AimRect.right = sCenter + (ubAimFinalWidth / 2);
		}

	}

	// Delete CTH indicator STIs from memory.
	DeleteVideoObjectFromIndex( guiCTHImage );

	///////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: Drawing the CTH indicator to the Frame Buffer
	//
	// Early on I ran into issues concerning drawing this indicator on the screen. At first I naturally
	// wanted to draw it on the Cursor Buffer, the same way as the old indicator. Unfortunately,
	// the size of the Cursor Buffer is quite limited (~35x35 pixels) and is governed by the size of the STIs used
	// for the mouse cursor itself. After much trial and error, it became obvious that it was impossible to draw
	// a large indicator (as required by the NCTH system) directly onto the mouse buffer.
	// 
	// Fortunately, I found out that it was possible to draw into the Frame Buffer instead, without leaving
	// graphical glitches behind. I could also dynamically define the area to draw into as I pleased. Furthermore,
	// I managed to make this drawing behave the same as a mouse cursor would, by having it follow the mouse
	// position.
	//
	// The end result is a bit of an ugly workaround: We're drawing what is essentially a mouse cursor without referring
	// to ANY part of the mouse cursor system (except the location of the mouse). This is the only way I could make it
	// work, and so far it works flawlessly.

	// Create a pointer to the Frame Buffer which we are going to draw directly into.
	UINT32 uiPitch;
	UINT16 *ptrBuf = (UINT16*)LockVideoSurface( FRAME_BUFFER, &uiPitch );
	uiPitch >>= 1;

	///////////////////////////////////////////////////////////////////////////////
	// Begin drawing

	sLeft = gsVIEWPORT_START_X;
	sTop = gsVIEWPORT_WINDOW_START_Y;
	sRight = gsVIEWPORT_END_X;
	sBottom = gsVIEWPORT_WINDOW_END_Y;

	// sevenfm: draw a line under the cursor indicating that gun is 'mounted' on surface
	NCTHShowMounted( pSoldier, ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, sStartScreenY, zOffset );

//	INT16 lastY = 0;
//	INT16 diffY = 0;
//	UINT32 uiAperture = __max(2,(UINT32)iAperture);
//	UINT32 uiMaxAperture = __max(2,(UINT32)iDistanceAperture);
	UINT32 uiApertureBarLength = 10;

	INT32 cnt = 0;

	// Vertical bias makes the circle "flatter" and "wider" in prone and crouched stance.
	FLOAT dVerticalBias = gGameCTHConstants.VERTICAL_BIAS;
	switch (gAnimControl[ pSoldier->usAnimState ].ubEndHeight)
	{
		case ANIM_STAND:
			dVerticalBias = 1.0;
			break;
		case ANIM_PRONE:
			// no change
			break;
		case ANIM_CROUCH:
			dVerticalBias = 1.0f + ((dVerticalBias - 1.0f) * 0.66f);
			break;
	}

	// Circumference tells us how many pixels to draw, hopefully.
	FLOAT RADIANS_IN_CIRCLE = (FLOAT)(PI * 2);
	INT32 Circ = 0;

	// sevenfm: limit maximum shown aperture
	NCTHCorrectMaxAperture( iAperture, iDistanceAperture, usCApertureBar );

	Circ = (INT32)((iDistanceAperture * RADIANS_IN_CIRCLE) * dVerticalBias);
	if(gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
	{
		// Draw outer circle
		for (INT32 iCurPoint = 0; iCurPoint < Circ; iCurPoint++)
		{
			curX = (INT16)(iDistanceAperture * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
			curY = (INT16)((iDistanceAperture * dVerticalBias) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
			INT16 firstX = curX;
			INT16 firstY = curY;

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBar );

			// Draw a border circle which is 1 point wider
//			curX = (INT16)((iDistanceAperture+1) * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
//			curY = (INT16)(((iDistanceAperture * dVerticalBias)+1) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));

//			if (curX != firstX || curY != firstY)
//				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBorder );

			// Draw a border circle which is 1 point narrower
//			curX = (INT16)((iDistanceAperture-1) * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
//			curY = (INT16)(((iDistanceAperture * dVerticalBias)-1) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));

//			if (curX != firstX || curY != firstY)
//				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBorder );
		}

		Circ = (INT32)((iAperture * RADIANS_IN_CIRCLE) * dVerticalBias);
		// Draw inner circle
		for (INT32 iCurPoint = 0; iCurPoint < Circ; iCurPoint++)
		{
			curX = (INT16)(iAperture * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
			curY = (INT16)((iAperture * dVerticalBias) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
			INT16 firstX = curX;
			INT16 firstY = curY;

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBar );

			// Draw a border circle which is 1 point wider
//			curX = (INT16)((iAperture+1) * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
//			curY = (INT16)(((iAperture * dVerticalBias)+1) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));

//			if (curX != firstX || curY != firstY)
//				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBorder );

			// Draw a border circle which is 1 point narrower
//			curX = (INT16)((iAperture-1) * cos((iCurPoint * RADIANS_IN_CIRCLE)/Circ));
//			curY = (INT16)(((iAperture * dVerticalBias)-1) * sin((iCurPoint * RADIANS_IN_CIRCLE)/Circ));

//			if (curX != firstX || curY != firstY)
//				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+curX, sStartScreenY+curY+(INT16)zOffset, usCApertureBorder );
		}

		// Aperture Crosshairs
		for (INT16 cnt = (INT16)(iAperture); cnt <= (INT16)(iAperture + uiApertureBarLength); cnt++)
		{
			// Horizontal Aperture Crosshairs
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, sStartScreenY+(INT16)zOffset, usCApertureBar );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, sStartScreenY+(INT16)zOffset, usCApertureBar );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY+1)+(INT16)zOffset, usCApertureBar );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY+1)+(INT16)zOffset, usCApertureBar );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY-1)+(INT16)zOffset, usCApertureBar );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY-1)+(INT16)zOffset, usCApertureBar );

			// Darker borders
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY+2)+(INT16)zOffset, usCApertureBorder );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY+2)+(INT16)zOffset, usCApertureBorder );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY-2)+(INT16)zOffset, usCApertureBorder );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY-2)+(INT16)zOffset, usCApertureBorder );

			// Darker borders
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY+1)+(INT16)zOffset, usCApertureBorder );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY+1)+(INT16)zOffset, usCApertureBorder );

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, (sStartScreenY-1)+(INT16)zOffset, usCApertureBorder );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-cnt, (sStartScreenY-1)+(INT16)zOffset, usCApertureBorder );

		}
		for (INT16 cnt = (INT16)(iAperture * dVerticalBias); cnt <= (INT16)((iAperture * dVerticalBias) + uiApertureBarLength); cnt++)
		{
			// Vertical Aperture Crosshairs
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBar );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBar );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBar );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBar );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBar );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBar );

			// Darker borders
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+2, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBorder );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+2, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBorder );

//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-2, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBorder );
//			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-2, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBorder );

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBorder );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBorder );

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, (sStartScreenY+cnt)+(INT16)zOffset, usCApertureBorder );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, (sStartScreenY-cnt)+(INT16)zOffset, usCApertureBorder );

		}
	}

	// sevenfm: draw laser dot
	NCTHDrawLaserDot( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, sStartScreenY, fLaserBonus, fBrightnessModifier, fEffectiveLaserRatio, zOffset );
	
	// Unlock the Frame Buffer.
	UnLockVideoSurface( FRAME_BUFFER );

	return (TRUE);
}

void RenderOverlayMessage( VIDEO_OVERLAY *pBlitter )
{
	// Override it!
	OverrideMercPopupBox( &gpOverrideMercBox );

	RenderMercPopupBox( pBlitter->sX, pBlitter->sY,	pBlitter->uiDestBuff );

	// Set it back!
	ResetOverrideMercPopupBox( );

	InvalidateRegion( pBlitter->sX, pBlitter->sY, pBlitter->sX + gusOverlayPopupBoxWidth, pBlitter->sY + gusOverlayPopupBoxHeight );
}


void BeginOverlayMessage( UINT32 uiFont, STR16 pFontString, ... )
{
	va_list argptr;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	CHAR16	SlideString[512];


	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(SlideString, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	// Override it!
	OverrideMercPopupBox( &gpOverrideMercBox );

	SetPrepareMercPopupFlags( MERC_POPUP_PREPARE_FLAGS_TRANS_BACK | MERC_POPUP_PREPARE_FLAGS_MARGINS );

	// Prepare text box
	iOverlayMessageBox = PrepareMercPopupBox( iOverlayMessageBox, BASIC_MERC_POPUP_BACKGROUND, RED_MERC_POPUP_BORDER, SlideString, 200, 50, 0, 0, &gusOverlayPopupBoxWidth, &gusOverlayPopupBoxHeight );

	// Set it back!
	ResetOverrideMercPopupBox( );

	if ( giPopupSlideMessageOverlay == -1	)
	{
		// WDS - bug fix: VideoOverlayDesc must be initialized! - 07/16/2007
		memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	 // Set Overlay
		VideoOverlayDesc.sLeft			= ( SCREEN_WIDTH - gusOverlayPopupBoxWidth ) / 2;
		VideoOverlayDesc.sTop				= 100;
		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusOverlayPopupBoxWidth;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusOverlayPopupBoxHeight;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderOverlayMessage;

		giPopupSlideMessageOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );
	}

}

void EndOverlayMessage( )
{
	if ( giPopupSlideMessageOverlay != -1 )
	{

//		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Removing Overlay message") );

		RemoveVideoOverlay( giPopupSlideMessageOverlay );

		giPopupSlideMessageOverlay = -1;

	}
}


void DrawBarsInUIBox( SOLDIERTYPE *pSoldier , INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, INT16 interval )
{
	FLOAT											dWidth, dPercentage;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	INT8											bBandage;
	INT16		color8;
	INT16		color16;

	// Draw new size
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	// region rysowania barow nad najemnikiem
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, ( gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y ) );

	// sevenfm: draw background for alt bar
	if( gGameExternalOptions.ubShowHealthBarsOnHead > 1 )
	{
		DrawBar( sXPos+2, sYPos, sWidth + 2, 1 + interval*3, COLOR_BLACK, Get16BPPColor( FROMRGB( 0, 0, 0	) ), pDestBuf );
		// draw border: light brown for stealth mode, grey for regular		
		if(pSoldier->bStealthMode)
		{
			color8 = COLOR_BROWN;
			color16 = Get16BPPColor( FROMRGB( 180, 140, 20 ) );
		}
		else{
			color8 = COLOR_DKGREY;
			color16 = Get16BPPColor( FROMRGB( 120, 120, 120 ) );
		}
		if ( pSoldier->ubID == gusSelectedSoldier )
		{
			if(gbPixelDepth==16)
				RectangleDraw( TRUE, sXPos+1, sYPos-1, sXPos+sWidth+3, sYPos+1+interval*3, color16, pDestBuf);
			else
				RectangleDraw8( TRUE, sXPos+1, sYPos-1, sXPos+sWidth+3, sYPos+1+interval*3, color8, pDestBuf);
		}
	}

	// get amt bandaged
	bBandage = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
	
	// NOW DO BLEEDING
	if ( pSoldier->bBleeding )
	{
		dPercentage = (FLOAT)( pSoldier->bBleeding +	pSoldier->stats.bLife + bBandage )/ (FLOAT)100;
		dWidth			=	dPercentage * sWidth;
		DrawBar( sXPos + 3, sYPos + 1, (INT32)dWidth, sHeight, COLOR_RED, Get16BPPColor( FROMRGB( 240,	240, 20	) ), pDestBuf );
	}
		
	if( bBandage )
	{
		dPercentage = (FLOAT)( pSoldier->stats.bLife + bBandage ) / (FLOAT)100;
		dWidth			=	dPercentage * sWidth;
		DrawBar( sXPos + 3, sYPos + 1, (INT32)dWidth, sHeight, COLOR_RED, Get16BPPColor( FROMRGB( 222, 132, 132	) ), pDestBuf );
	}
		
	dPercentage = (FLOAT)pSoldier->stats.bLife / (FLOAT)100;
	dWidth			=	dPercentage * sWidth;
	DrawBar( sXPos + 3, sYPos + 1, (INT32)dWidth, sHeight, COLOR_RED, Get16BPPColor( FROMRGB( 200, 0, 0	) ), pDestBuf );
		
	dPercentage = (FLOAT)( pSoldier->bBreathMax ) / (FLOAT)100;
	dWidth			=	dPercentage * sWidth;
	DrawBar( sXPos + 3, sYPos + 1 + interval, (INT32)dWidth, sHeight, COLOR_BLUE, Get16BPPColor( FROMRGB( 20, 20, 150	) ), pDestBuf );

	dPercentage = (FLOAT)( pSoldier->bBreath ) / (FLOAT)100;
	dWidth			=	dPercentage * sWidth;
	DrawBar( sXPos + 3, sYPos + 1 + interval, (INT32)dWidth, sHeight, COLOR_BLUE, Get16BPPColor( FROMRGB( 100, 100, 220 ) ), pDestBuf );	

	/*
	// morale
	dPercentage = (FLOAT)pSoldier->aiData.bMorale / (FLOAT)100;
	dWidth			=	dPercentage * sWidth;
	if(gbPixelDepth==16)
	{
		usLineColor = Get16BPPColor( FROMRGB( 0, 250, 0 ) );
		RectangleDraw( TRUE, sXPos + 1, sYPos + 7, (INT32)( sXPos + dWidth + 1 ), sYPos + 7, usLineColor, pDestBuf );
	}
	else if(gbPixelDepth==8)
	{
	// DB Need to change this to a color from the 8-bit standard palette
		usLineColor = COLOR_GREEN;
		RectangleDraw8( TRUE, sXPos + 1, sYPos + 7, (INT32)( sXPos + dWidth + 1 ), sYPos + 7, usLineColor, pDestBuf );
	}

	*/

	// draw suppression shock bar
	if( gGameExternalOptions.ubShowHealthBarsOnHead > 1 )
	{
		dPercentage = (FLOAT)( __min(100, ( (FLOAT)100 * CalcEffectiveShockLevel( pSoldier ) ) / CalcSuppressionTolerance( pSoldier ) ) ) / (FLOAT)100;
		dWidth		=	dPercentage * sWidth;
		DrawBar( sXPos+3, sYPos+1+2*interval, (INT32)dWidth, sHeight, COLOR_ORANGE, Get16BPPColor( FROMRGB( 220, 140, 0 ) ), pDestBuf );
	}

	/*
	BOOLEAN bDisguised = pSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER);
	BOOLEAN bStealth = pSoldier->bStealthMode;
	// draw cover indicators
	if( ( gGameExternalOptions.ubShowHealthBarsOnHead > 1 ) && 
		( (gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) || bStealth || bDisguised ) )
	{		
		INT8 cover = 3;		// MAX_COVER
		BOOLEAN showCover;
		INT32 usMapPos;

		if( gGameExternalOptions.ubShowCoverIndicator ==1 ||
			( gGameExternalOptions.ubShowCoverIndicator ==3 && (bStealth || bDisguised) ) )
		{
			if( GetMouseMapPos(&usMapPos) && _KeyDown(SHIFT) )
			{
				// draw target tile cover indicator
				CalculateCoverForSoldier( pSoldier, usMapPos, gsInterfaceLevel, cover );
				showCover = CoverColorCode( cover, color8, color16 );
			}
			else
			{
				// draw cover indicator
				CalculateCoverForSoldier( pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel, cover );
				showCover = CoverColorCode( cover, color8, color16 );
				if ( color8 == COLOR_GREEN)
					showCover = FALSE;
			}

			if(showCover)
				DrawBar( sXPos+33, sYPos+1, 1+sHeight , (interval==3 ? 2 : 3 )*interval-1, color8, color16, pDestBuf );
		}
	}
	*/

	UnLockVideoSurface( FRAME_BUFFER );

}

void EndDeadlockMsg( )
{
	// Reset gridlock
	gfUIInDeadlock = FALSE;
}


void ClearInterface( )
{
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	// Stop any UI menus!
	if ( gCurrentUIMode == MENU_MODE )
	{
		EndMenuEvent( guiCurrentEvent );
	}

	if ( gfUIHandleShowMoveGrid )
	{
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS4	);
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS2 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS13 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS15 );
	}

	// Remove any popup menus
	if ( gCurrentUIMode == GETTINGITEM_MODE )
	{
		RemoveItemPickupMenu( );
	}

	// Remove any popup menus
	if ( gCurrentUIMode == OPENDOOR_MENU_MODE )
	{
		PopDownOpenDoorMenu( );
	}

	// Remove UP/DOWN arrows...
	//EraseRenderArrows( );
	// Don't render arrows this frame!
	guiShowUPDownArrows	= ARROWS_HIDE_UP | ARROWS_HIDE_DOWN;
	gTintBulletCounts				= FALSE;
	gfUIAutofireBulletCount			= FALSE;

	ResetPhysicsTrajectoryUI( );

	// Remove any paths, cursors
	ErasePath( FALSE );

	//gfPlotNewMovement = TRUE;

	// Erase Interface cursors
	HideUICursor( );

	MSYS_ChangeRegionCursor( &gViewportRegion , VIDEO_NO_CURSOR );

	// Hide lock UI cursors...
	MSYS_ChangeRegionCursor( &gDisableRegion , VIDEO_NO_CURSOR );
	MSYS_ChangeRegionCursor( &gUserTurnRegion , VIDEO_NO_CURSOR );

	// Remove special thing for south arrow...
	if ( gsGlobalCursorYOffset == ( SCREEN_HEIGHT - gsVIEWPORT_WINDOW_END_Y ) )
	{
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
	}

}

void RestoreInterface( )
{
	// Once we are done, plot path again!
	gfPlotNewMovement = TRUE;

	// OK, reset arrows too...
	gfUIRefreshArrows = TRUE;

	// SHow lock UI cursors...
	MSYS_ChangeRegionCursor( &gDisableRegion , CURSOR_WAIT );
	MSYS_ChangeRegionCursor( &gUserTurnRegion , CURSOR_WAIT );

}


void BlitPopupText( VIDEO_OVERLAY *pBlitter )
{
	UINT8	*pDestBuf;
	UINT32 uiDestPitchBYTES;

	BltVideoSurface( pBlitter->uiDestBuff, guiINTEXT, 0, pBlitter->pBackground->sLeft, pBlitter->pBackground->sTop, VS_BLT_FAST | VS_BLT_USECOLORKEY, NULL );


	pDestBuf = LockVideoSurface( pBlitter->uiDestBuff, &uiDestPitchBYTES);

	SetFont( pBlitter->uiFontID );
	SetFontBackground( pBlitter->ubFontBack );
	SetFontForeground( pBlitter->ubFontFore );

	mprintf_buffer( pDestBuf, uiDestPitchBYTES, pBlitter->uiFontID, pBlitter->sX, pBlitter->sY, pBlitter->zText );

	UnLockVideoSurface( pBlitter->uiDestBuff );

}

void DirtyMercPanelInterface( SOLDIERTYPE *pSoldier, UINT8 ubDirtyLevel )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DirtyMercPanelInterface"));
	if ( pSoldier->bTeam == gbPlayerNum )
	{
		// ONly set to a higher level!
		if ( fInterfacePanelDirty < ubDirtyLevel )
		{
			fInterfacePanelDirty = ubDirtyLevel;
		}
	}

}

typedef struct
{
	SOLDIERTYPE *pSoldier;
	STRUCTURE		*pStructure;
	UINT8				ubDirection;
	INT16				sX;
	INT16				sY;
	BOOLEAN			fMenuHandled;
	BOOLEAN			fClosingDoor;

} OPENDOOR_MENU;

OPENDOOR_MENU	gOpenDoorMenu;
BOOLEAN				gfInOpenDoorMenu = FALSE;

BOOLEAN InitDoorOpenMenu( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection, BOOLEAN fClosingDoor )
{
	INT16 sHeight, sWidth;
	INT16	sScreenX, sScreenY;

	// Can't bring up this menu if in autobandage mode
	if (gTacticalStatus.fAutoBandageMode)
	{
		return FALSE;
	}

	// Erase other menus....
	EraseInterfaceMenus( TRUE );

	InterruptTime();
	PauseGame();
	LockPauseState( 19 );
	// Pause timers as well....
	PauseTime( TRUE );


	gOpenDoorMenu.pSoldier		= pSoldier;
	gOpenDoorMenu.pStructure	= pStructure;
	gOpenDoorMenu.ubDirection	= ubDirection;
	gOpenDoorMenu.fClosingDoor	= fClosingDoor;

	// OK, Determine position...
	// Center on guy
	// Locate to guy first.....
	LocateSoldier( pSoldier->ubID, FALSE );
	GetSoldierAnimDims( pSoldier, &sHeight, &sWidth );
	GetSoldierScreenPos( pSoldier, &sScreenX, &sScreenY );
	gOpenDoorMenu.sX = sScreenX - ( ( BUTTON_PANEL_WIDTH - sWidth ) / 2 );
	gOpenDoorMenu.sY = sScreenY - ( ( BUTTON_PANEL_HEIGHT - sHeight ) / 2 );

	// Alrighty, cancel lock UI if we havn't done so already
	UnSetUIBusy( pSoldier->ubID );


	// OK, CHECK FOR BOUNDARIES!
	if ( ( gOpenDoorMenu.sX + BUTTON_PANEL_WIDTH ) > SCREEN_WIDTH )
	{
		gOpenDoorMenu.sX = ( SCREEN_WIDTH - BUTTON_PANEL_WIDTH );
	}
	if ( ( gOpenDoorMenu.sY + BUTTON_PANEL_HEIGHT ) > gsVIEWPORT_WINDOW_END_Y )
	{
		gOpenDoorMenu.sY = ( gsVIEWPORT_WINDOW_END_Y - BUTTON_PANEL_HEIGHT );
	}
	if ( gOpenDoorMenu.sX < 0 )
	{
		gOpenDoorMenu.sX = 0;
	}
	if ( gOpenDoorMenu.sY < 0 )
	{
		gOpenDoorMenu.sY = 0;
	}


	gOpenDoorMenu.fMenuHandled	= FALSE;

	guiPendingOverrideEvent		= OP_OPENDOORMENU;
	HandleTacticalUI( );

	PopupDoorOpenMenu( fClosingDoor );

	return( TRUE );
}

void PopupDoorOpenMenu( BOOLEAN fClosingDoor )
{
	INT32								iMenuAnchorX, iMenuAnchorY;
	CHAR16								zDisp[ 100 ];

	//iMenuAnchorX = gOpenDoorMenu.sX;
	//iMenuAnchorY = gOpenDoorMenu.sY;

	// Blit background!
	//BltVideoObjectFromIndex( FRAME_BUFFER, guiBUTTONBORDER, 0, iMenuAnchorX, iMenuAnchorY, VO_BLT_SRCTRANSPARENCY, NULL );
	iMenuAnchorX = gOpenDoorMenu.sX + 9;
	iMenuAnchorY = gOpenDoorMenu.sY + 8;


	// Create mouse region over all area to facilitate clicking to end
	MSYS_DefineRegion( &gMenuOverlayRegion, 0, 0 ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST-1,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, DoorMenuBackregionCallback );
	// Add region
	MSYS_AddRegion( &gMenuOverlayRegion);


	iActionIcons[ USE_KEYRING_ICON ] = QuickCreateButton( iIconImages[ USE_KEYRING_IMAGES ], (INT16)(iMenuAnchorX + 20 ), (INT16)(iMenuAnchorY ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ USE_KEYRING_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ USE_KEYRING_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ USE_KEYRING_ICON ], APBPConstants[AP_UNLOCK_DOOR] );
	}
	SetButtonFastHelpText( iActionIcons[ USE_KEYRING_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_UNLOCK_DOOR], APBPConstants[BP_UNLOCK_DOOR], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) )
	{
		DisableButton( iActionIcons[ USE_KEYRING_ICON ] );
	}

	// Greyout if no keys found...
	if ( !SoldierHasKey( gOpenDoorMenu.pSoldier, ANYKEY ) )
	{
		DisableButton( iActionIcons[ USE_KEYRING_ICON ] );
	}


	iActionIcons[ USE_CROWBAR_ICON ] = QuickCreateButton( iIconImages[ CROWBAR_DOOR_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY	),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ USE_CROWBAR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ USE_CROWBAR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ USE_CROWBAR_ICON ], APBPConstants[AP_USE_CROWBAR] );
	}
	SetButtonFastHelpText( iActionIcons[ USE_CROWBAR_ICON ], zDisp );

	// Greyout if no crowbar found...
	if ( FindUsableCrowbar ( gOpenDoorMenu.pSoldier ) == NO_SLOT	|| fClosingDoor )
	{
		DisableButton( iActionIcons[ USE_CROWBAR_ICON ] );
	}

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_USE_CROWBAR], APBPConstants[BP_USE_CROWBAR], FALSE ) )
	{
		DisableButton( iActionIcons[ USE_CROWBAR_ICON ] );
	}

	iActionIcons[ LOCKPICK_DOOR_ICON ] = QuickCreateButton( iIconImages[ LOCKPICK_DOOR_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ LOCKPICK_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ LOCKPICK_DOOR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ LOCKPICK_DOOR_ICON ], GetAPsToPicklock( gOpenDoorMenu.pSoldier ) ); // SANDRO
	}
	SetButtonFastHelpText( iActionIcons[ LOCKPICK_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToPicklock( gOpenDoorMenu.pSoldier ), APBPConstants[BP_PICKLOCK], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) ) // SANDRO
	{
		DisableButton( iActionIcons[ LOCKPICK_DOOR_ICON ] );
	}


	// Grayout if no lockpick found....
	if ( FindLocksmithKit( gOpenDoorMenu.pSoldier ) == NO_SLOT )
	{
		DisableButton( iActionIcons[ LOCKPICK_DOOR_ICON ] );
	}


	iActionIcons[ EXPLOSIVE_DOOR_ICON ] = QuickCreateButton( iIconImages[ EXPLOSIVE_DOOR_IMAGES ], (INT16)(iMenuAnchorX + 40 ), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ EXPLOSIVE_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ EXPLOSIVE_DOOR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ EXPLOSIVE_DOOR_ICON ], GetAPsToBombDoor( gOpenDoorMenu.pSoldier ) ); // SANDRO
	}
	SetButtonFastHelpText( iActionIcons[ EXPLOSIVE_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToBombDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_EXPLODE_DOOR], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) ) // SANDRO
	{
		DisableButton( iActionIcons[ EXPLOSIVE_DOOR_ICON ] );
	}

	// Grayout if no lock explosive found....
	// For no use bomb1 until we get a special item for this
	if ( FindLockBomb ( gOpenDoorMenu.pSoldier ) == NO_SLOT )
	{
		DisableButton( iActionIcons[ EXPLOSIVE_DOOR_ICON ] );
	}


	iActionIcons[ OPEN_DOOR_ICON ] = QuickCreateButton( iIconImages[ OPEN_DOOR_IMAGES ], (INT16)(iMenuAnchorX ), (INT16)(iMenuAnchorY ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ OPEN_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if ( fClosingDoor )
	{
		if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			swprintf( zDisp, pTacticalPopupButtonStrings[ CANCEL_ICON + 1 ] );
		}
		else
		{
			swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ CANCEL_ICON + 1 ], GetAPsToOpenDoor( gOpenDoorMenu.pSoldier ) ); // SANDRO
		}
	}
	else
	{
		if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			swprintf( zDisp, pTacticalPopupButtonStrings[ OPEN_DOOR_ICON ] );
		}
		else
		{
			swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ OPEN_DOOR_ICON ], GetAPsToOpenDoor( gOpenDoorMenu.pSoldier ) ); // SANDRO
		}
	}
	SetButtonFastHelpText( iActionIcons[ OPEN_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToOpenDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_OPEN_DOOR], FALSE ) ) // SANDRO
	{
		DisableButton( iActionIcons[ OPEN_DOOR_ICON ] );
	}


	// Create button based on what is in our hands at the moment!
	iActionIcons[ EXAMINE_DOOR_ICON ] = QuickCreateButton( iIconImages[ EXAMINE_DOOR_IMAGES ], (INT16)(iMenuAnchorX	), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ EXAMINE_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ EXAMINE_DOOR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ EXAMINE_DOOR_ICON ], APBPConstants[AP_EXAMINE_DOOR] );
	}
	SetButtonFastHelpText( iActionIcons[ EXAMINE_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_EXAMINE_DOOR], APBPConstants[BP_EXAMINE_DOOR], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) )
	{
		DisableButton( iActionIcons[ EXAMINE_DOOR_ICON ] );
	}

	iActionIcons[ BOOT_DOOR_ICON ] = QuickCreateButton( iIconImages[ BOOT_DOOR_IMAGES ], (INT16)(iMenuAnchorX	), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ BOOT_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ BOOT_DOOR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ BOOT_DOOR_ICON ], APBPConstants[AP_BOOT_DOOR] );
	}
	SetButtonFastHelpText( iActionIcons[ BOOT_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_BOOT_DOOR], APBPConstants[BP_BOOT_DOOR], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) )
	{
		DisableButton( iActionIcons[ BOOT_DOOR_ICON ] );
	}


	iActionIcons[ UNTRAP_DOOR_ICON ] = QuickCreateButton( iIconImages[ UNTRAP_DOOR_ICON ], (INT16)(iMenuAnchorX + 20	), (INT16)(iMenuAnchorY + 40 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ UNTRAP_DOOR_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}

	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		swprintf( zDisp, pTacticalPopupButtonStrings[ UNTRAP_DOOR_ICON ] );
	}
	else
	{
		swprintf( zDisp, L"%s ( %d )", pTacticalPopupButtonStrings[ UNTRAP_DOOR_ICON ], GetAPsToUntrapDoor( gOpenDoorMenu.pSoldier ) ); // SANDRO
	}
	SetButtonFastHelpText( iActionIcons[ UNTRAP_DOOR_ICON ], zDisp );

	if ( !EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToUntrapDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_UNTRAP_DOOR], FALSE ) || fClosingDoor || AM_AN_EPC( gOpenDoorMenu.pSoldier ) ) // SANDRO
	{
		DisableButton( iActionIcons[ UNTRAP_DOOR_ICON ] );
	}

	iActionIcons[ CANCEL_ICON ] = QuickCreateButton( iIconImages[ CANCEL_IMAGES ], (INT16)(iMenuAnchorX + 20	), (INT16)(iMenuAnchorY + 20 ),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDoorMenuCallback );
	if ( iActionIcons[ CANCEL_ICON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return;
	}
	SetButtonFastHelpText( iActionIcons[ CANCEL_ICON ], pTacticalPopupButtonStrings[ CANCEL_ICON ] );

	//LockTacticalInterface( );

	gfInOpenDoorMenu = TRUE;

	// Ignore scrolling
	gfIgnoreScrolling = TRUE;
}


void PopDownOpenDoorMenu( )
{
	if ( gfInOpenDoorMenu )
	{
		UnLockPauseState();
		UnPauseGame();
		// UnPause timers as well....
		PauseTime( FALSE );

		RemoveButton( iActionIcons[ USE_KEYRING_ICON	] );
		RemoveButton( iActionIcons[ USE_CROWBAR_ICON	] );
		RemoveButton( iActionIcons[ LOCKPICK_DOOR_ICON	] );
		RemoveButton( iActionIcons[ EXPLOSIVE_DOOR_ICON	] );
		RemoveButton( iActionIcons[ OPEN_DOOR_ICON	] );
		RemoveButton( iActionIcons[ EXAMINE_DOOR_ICON	] );
		RemoveButton( iActionIcons[ BOOT_DOOR_ICON	] );
		RemoveButton( iActionIcons[ UNTRAP_DOOR_ICON	] );
		RemoveButton( iActionIcons[ CANCEL_ICON	] );

		// Turn off Ignore scrolling
		gfIgnoreScrolling = FALSE;

		// Rerender world
		SetRenderFlags( RENDER_FLAG_FULL );

		fInterfacePanelDirty = DIRTYLEVEL2;

		//UnLockTacticalInterface( );
		MSYS_RemoveRegion( &gMenuOverlayRegion);

	}

	gfInOpenDoorMenu = FALSE;

}

void RenderOpenDoorMenu( )
{
	if ( gfInOpenDoorMenu )
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, guiBUTTONBORDER, 0, gOpenDoorMenu.sX, gOpenDoorMenu.sY, VO_BLT_SRCTRANSPARENCY, NULL );

		// Mark buttons dirty!
		MarkAButtonDirty( iActionIcons[ USE_KEYRING_ICON	] );
		MarkAButtonDirty( iActionIcons[ USE_CROWBAR_ICON	] );
		MarkAButtonDirty( iActionIcons[ LOCKPICK_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ EXPLOSIVE_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ OPEN_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ EXAMINE_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ BOOT_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ UNTRAP_DOOR_ICON	] );
		MarkAButtonDirty( iActionIcons[ CANCEL_ICON	] );

		RenderButtons( );

		// if game is paused, then render paused game text
		RenderPausedGameBox( );

		InvalidateRegion( gOpenDoorMenu.sX, gOpenDoorMenu.sY, gOpenDoorMenu.sX + BUTTON_PANEL_WIDTH, gOpenDoorMenu.sY + BUTTON_PANEL_HEIGHT );

	}
}

void CancelOpenDoorMenu( )
{
	// Signal end of event
	gOpenDoorMenu.fMenuHandled = 2;
}

void BtnDoorMenuCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32		uiBtnID;

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;

		uiBtnID = btn->IDNum;

		// Popdown menu
		gOpenDoorMenu.fMenuHandled = TRUE;

		if ( uiBtnID == iActionIcons[ CANCEL_ICON ] )
		{
			// OK, set cancle code!
			gOpenDoorMenu.fMenuHandled = 2;
#ifdef JA2UB			
			//Handle someone trying to open the door in the tunnel gate`
			HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( gOpenDoorMenu.pSoldier, FALSE ); //Ja25 UB
#endif
		}

		// Switch on command....
		if ( uiBtnID == iActionIcons[ OPEN_DOOR_ICON ] )
		{
#ifdef JA2UB		
			//Handle someone trying to open the door in the tunnel gate`
			if( HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( gOpenDoorMenu.pSoldier, TRUE ) ) //Ja25 UB
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
			else
			{
				// Open door normally...
				// Check APs
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_OPEN_DOOR], APBPConstants[BP_OPEN_DOOR], FALSE ) )
				{
				// Set UI
					SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

					if ( gOpenDoorMenu.fClosingDoor )
					{
						gOpenDoorMenu.pSoldier->ChangeSoldierState( GetAnimStateForInteraction( gOpenDoorMenu.pSoldier, TRUE, CLOSE_DOOR ), 0 , FALSE );
					}
					else
					{
						InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_OPEN );
					}
				}
				else
				{
					// OK, set cancel code!
					gOpenDoorMenu.fMenuHandled = 2;
				}
			}
#else
			// Open door normally...
			// Check APs
			// SANDRO - changed APs for opening dorrs calc
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToOpenDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_OPEN_DOOR], FALSE ) )
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				if ( gOpenDoorMenu.fClosingDoor )
				{
					gOpenDoorMenu.pSoldier->ChangeSoldierState( GetAnimStateForInteraction( gOpenDoorMenu.pSoldier, TRUE, CLOSE_DOOR ), 0 , FALSE );
				}
				else
				{
					InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_OPEN );
				}
			}
			else
			{
				// OK, set cancel code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
#endif
		}

		if ( uiBtnID == iActionIcons[ BOOT_DOOR_ICON ] )
		{
			// Boot door
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_BOOT_DOOR], APBPConstants[BP_BOOT_DOOR], FALSE ) )
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_FORCE );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
		}

		if ( uiBtnID == iActionIcons[ USE_KEYRING_ICON ] )
		{
			// Unlock door
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_UNLOCK_DOOR], APBPConstants[BP_UNLOCK_DOOR], FALSE ) )
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_UNLOCK );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
		}

		if ( uiBtnID == iActionIcons[ LOCKPICK_DOOR_ICON ] )
		{
			// Lockpick
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToPicklock( gOpenDoorMenu.pSoldier ), APBPConstants[BP_PICKLOCK], FALSE ) ) // SANDRO
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_LOCKPICK );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
		}

		if ( uiBtnID == iActionIcons[ EXAMINE_DOOR_ICON ] )
		{
			// Lockpick
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_EXAMINE_DOOR], APBPConstants[BP_EXAMINE_DOOR], FALSE ) )
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_EXAMINE );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
		}

		if ( uiBtnID == iActionIcons[ EXPLOSIVE_DOOR_ICON ] )
		{
#ifdef JA2UB
			//Handle someone trying to open the door in the tunnel gate`
			if( HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( gOpenDoorMenu.pSoldier, TRUE ) ) //Ja25 UB
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
			else
			{
					// Explode
				if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_EXPLODE_DOOR], APBPConstants[BP_EXPLODE_DOOR], FALSE ) )
				{
					// Set UI
					SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

					InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_EXPLODE );
				}
				else
				{
					// OK, set cancle code!
					gOpenDoorMenu.fMenuHandled = 2;
				}
			}
#else
			// Explode
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToBombDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_EXPLODE_DOOR], FALSE ) ) // SANDRO
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_EXPLODE );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
#endif
		}

		if ( uiBtnID == iActionIcons[ UNTRAP_DOOR_ICON ] )
		{
			// Explode
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, GetAPsToUntrapDoor( gOpenDoorMenu.pSoldier ), APBPConstants[BP_UNTRAP_DOOR], FALSE ) ) // SANDRO
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_UNTRAP );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
		}

		if ( uiBtnID == iActionIcons[ USE_CROWBAR_ICON ] )
		{
#ifdef JA2UB		
			//Handle someone trying to open the door in the tunnel gate`
			if( HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( gOpenDoorMenu.pSoldier, TRUE ) ) //JA25 UB
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
			else
			{
				// Explode
				if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_USE_CROWBAR], APBPConstants[BP_USE_CROWBAR], FALSE ) )
				{
					// Set UI
					SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

					InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_CROWBAR );
				}
				else
				{
					// OK, set cancle code!
					gOpenDoorMenu.fMenuHandled = 2;
				}
			}
#else
			// Explode
			if ( EnoughPoints(	gOpenDoorMenu.pSoldier, APBPConstants[AP_USE_CROWBAR], APBPConstants[BP_USE_CROWBAR], FALSE ) )
			{
				// Set UI
				SetUIBusy( (UINT8)gOpenDoorMenu.pSoldier->ubID );

				InteractWithClosedDoor( gOpenDoorMenu.pSoldier, HANDLE_DOOR_CROWBAR );
			}
			else
			{
				// OK, set cancle code!
				gOpenDoorMenu.fMenuHandled = 2;
			}
#endif
		}

		HandleOpenDoorMenu( );

	}
}


BOOLEAN HandleOpenDoorMenu( )
{
	if ( gOpenDoorMenu.fMenuHandled )
	{
		PopDownOpenDoorMenu( );
		return( gOpenDoorMenu.fMenuHandled );
	}

	return( FALSE );
}


void RenderUIMessage( VIDEO_OVERLAY *pBlitter )
{
	// Shade area first...
	ShadowVideoSurfaceRect( pBlitter->uiDestBuff, pBlitter->sX, pBlitter->sY, pBlitter->sX + gusUIMessageWidth - 2, pBlitter->sY + gusUIMessageHeight - 2 );

	RenderMercPopUpBoxFromIndex( iUIMessageBox, pBlitter->sX, pBlitter->sY,	pBlitter->uiDestBuff );

	InvalidateRegion( pBlitter->sX, pBlitter->sY, pBlitter->sX + gusUIMessageWidth, pBlitter->sY + gusUIMessageHeight );
}


void InternalBeginUIMessage( BOOLEAN fUseSkullIcon, STR16 pFontString, ... )
{
	va_list argptr;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	CHAR16	MsgString[512];


	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(MsgString, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	guiUIMessageTime = GetJA2Clock( );
	guiUIMessageTimeDelay = CalcUIMessageDuration(MsgString);

	// Override it!
	OverrideMercPopupBox( &gpUIMessageOverrideMercBox );

	//SetPrepareMercPopupFlags( MERC_POPUP_PREPARE_FLAGS_TRANS_BACK | MERC_POPUP_PREPARE_FLAGS_MARGINS );

	if ( fUseSkullIcon )
	{
		SetPrepareMercPopupFlags( MERC_POPUP_PREPARE_FLAGS_MARGINS | MERC_POPUP_PREPARE_FLAGS_SKULLICON );
	}
	else
	{
		SetPrepareMercPopupFlags( MERC_POPUP_PREPARE_FLAGS_MARGINS | MERC_POPUP_PREPARE_FLAGS_STOPICON );
	}

	// Prepare text box
	iUIMessageBox = PrepareMercPopupBox( iUIMessageBox ,BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, MsgString, 200, 10, 0, 0, &gusUIMessageWidth, &gusUIMessageHeight );

	// Set it back!
	ResetOverrideMercPopupBox( );

	if ( giUIMessageOverlay != -1	)
	{
		RemoveVideoOverlay( giUIMessageOverlay );

		giUIMessageOverlay = -1;
	}

	if ( giUIMessageOverlay == -1	)
	{
		memset( &VideoOverlayDesc, 0, sizeof( VideoOverlayDesc ) );

		// Set Overlay
		VideoOverlayDesc.sLeft			= ( SCREEN_WIDTH - gusUIMessageWidth ) / 2;
		VideoOverlayDesc.sTop				= 150;
		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusUIMessageWidth;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusUIMessageHeight;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderUIMessage;

		giUIMessageOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );
	}

	gfUseSkullIconMessage = fUseSkullIcon;
}

void BeginUIMessage( STR16 pFontString, ... )
{
	va_list argptr;
	CHAR16	MsgString[512];

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(MsgString, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	InternalBeginUIMessage( FALSE, MsgString );
}


void BeginMapUIMessage( UINT8 ubPosition, STR16 pFontString, ... )
{
	va_list argptr;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	CHAR16	MsgString[512];

	memset( &VideoOverlayDesc, 0, sizeof( VideoOverlayDesc ) );

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(MsgString, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	guiUIMessageTime = GetJA2Clock( );
	guiUIMessageTimeDelay = CalcUIMessageDuration(MsgString);

	// Override it!
	OverrideMercPopupBox( &gpUIMessageOverrideMercBox );

	SetPrepareMercPopupFlags( MERC_POPUP_PREPARE_FLAGS_TRANS_BACK | MERC_POPUP_PREPARE_FLAGS_MARGINS );

	// Prepare text box
	iUIMessageBox = PrepareMercPopupBox( iUIMessageBox ,BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, MsgString, 200, 10, 0, 0, &gusUIMessageWidth, &gusUIMessageHeight );

	// Set it back!
	ResetOverrideMercPopupBox( );

	if ( giUIMessageOverlay == -1	)
	{
		// WDS - bug fix: VideoOverlayDesc must be initialized! - 07/16/2007
		memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );
		// Set Overlay
		VideoOverlayDesc.sLeft	= 20 + MAP_VIEW_START_X + ( MAP_VIEW_WIDTH - gusUIMessageWidth ) / 2;

		VideoOverlayDesc.sTop	= MAP_VIEW_START_Y + ( MAP_VIEW_HEIGHT - gusUIMessageHeight ) / 2;

		if( ubPosition == MSG_MAP_UI_POSITION_UPPER )
		{
			VideoOverlayDesc.sTop	-= 100;
		}
		else if ( ubPosition == MSG_MAP_UI_POSITION_LOWER )
		{
			VideoOverlayDesc.sTop	+= 100;
		}

		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusUIMessageWidth;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusUIMessageHeight;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderUIMessage;

		giUIMessageOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );
	}

}

void EndUIMessage( )
{
	UINT32	uiClock = GetJA2Clock();

	if ( giUIMessageOverlay != -1 )
	{
		if ( gfUseSkullIconMessage )
		{
			if ( ( uiClock - guiUIMessageTime ) < 300 )
			{
				return;
			}
		}

//		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Removing Overlay message") );

		RemoveVideoOverlay( giUIMessageOverlay );

	// Remove popup as well....
	if ( iUIMessageBox != -1 )
	{
		RemoveMercPopupBoxFromIndex( iUIMessageBox );
		iUIMessageBox = -1;
	}

		giUIMessageOverlay = -1;

	}
	//iUIMessageBox = -1;
}

#define PLAYER_TEAM_TIMER_INTTERUPT_GRACE		( 15000 / PLAYER_TEAM_TIMER_SEC_PER_TICKS )
#define PLAYER_TEAM_TIMER_GRACE_PERIOD			1000
//#define PLAYER_TEAM_TIMER_SEC_PER_TICKS			100
int PLAYER_TEAM_TIMER_SEC_PER_TICKS = 100;//hayden
#define PLAYER_TEAM_TIMER_TICKS_PER_OK_MERC								( 15000 / PLAYER_TEAM_TIMER_SEC_PER_TICKS )
#define PLAYER_TEAM_TIMER_TICKS_PER_NOTOK_MERC						( 5000 / PLAYER_TEAM_TIMER_SEC_PER_TICKS )
#define PLAYER_TEAM_TIMER_TICKS_FROM_END_TO_START_BEEP		( (5000 / PLAYER_TEAM_TIMER_SEC_PER_TICKS)*1.15 )
#define PLAYER_TEAM_TIMER_TIME_BETWEEN_BEEPS							( 500 )
#define PLAYER_TEAM_TIMER_TICKS_PER_ENEMY									( 2000 / PLAYER_TEAM_TIMER_SEC_PER_TICKS )


BOOLEAN AddTopMessage( UINT8 ubType, STR16 pzString )
{
	UINT32	cnt;
	BOOLEAN	fFound = FALSE;

	// Set time of last update
	gTopMessage.uiTimeOfLastUpdate = GetJA2Clock( );

	// Set flag to animate down...
	//gTopMessage.bAnimate = -1;
	//gTopMessage.bYPos		= 2;

	gTopMessage.bAnimate = 0;
	gTopMessage.bYPos		= 20;
	gTopMessage.fCreated = TRUE;

	fFound = TRUE;
	cnt = 0;

	// WANNE: Disabled AI count on turnbar, because it shows different count on clients
	/*
	if(is_networked && gTacticalStatus.ubCurrentTeam == 1 && ubType == COMPUTER_TURN_MESSAGE){
		//add ai count to turn bar - haydent
		swprintf( pzString, MPClientMessage[80], NumEnemyInSector());
	}
	*/

	if ( fFound )
	{
		gTopMessage.bCurrentMessage = (INT8)cnt;

		gTacticalStatus.ubTopMessageType = ubType;
		gTacticalStatus.fInTopMessage = TRUE;

		// Copy string
		wcsncpy( gTacticalStatus.zTopMessageString, pzString, 19 );

		CreateTopMessage( gTopMessage.uiSurface, ubType, pzString );

		return( TRUE );
	}

	return( FALSE );
}

void CreateTopMessage( UINT32 uiSurface, UINT8 ubType, STR16 psString )
{
	UINT32			uiBAR, uiPLAYERBAR = 0, uiINTBAR = 0;
	VOBJECT_DESC	VObjectDesc;
	INT16			sX, sY;
	INT32			cnt2;
	INT16			sBarX = 0;
	UINT32			uiBarToUseInUpDate=0;
	BOOLEAN			fDoLimitBar = FALSE;
	FLOAT			dNumStepsPerEnemy, dLength, dCurSize;
	INT16			iProgBarLength; 
	STR fn;
	
	memset( &VObjectDesc, 0, sizeof( VObjectDesc ) );
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		iProgBarLength = 640 - 13;
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\rect_Thin.sti";
		else
			fn = "INTERFACE\\rect.sti";		
	}
	else if (iResolution < _1024x768)
	{
		iProgBarLength = 800 - 13;
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\rect_800x600Thin.sti";
		else
			fn = "INTERFACE\\rect_800x600.sti";			
	}
	else
	{
		iProgBarLength = 1024 - 13;
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\rect_1024x768Thin.sti";
		else
			fn = "INTERFACE\\rect_1024x768.sti";
	}

	FilenameForBPP(fn, VObjectDesc.ImageFile);
	
	if( !AddVideoObject( &VObjectDesc, &uiBAR ) )	
	{ 
		sprintf(fn, "Missing %s", fn);
		AssertMsg(0, fn );	
	}
	
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebargreen_Thin.sti";
		else
			fn = "INTERFACE\\timebargreen.sti";
	}
	else if (iResolution < _1024x768)
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebargreen_800x600Thin.sti";
		else
			fn = "INTERFACE\\timebargreen_800x600.sti";
	}
	else
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebargreen_1024x768Thin.sti";
		else
			fn = "INTERFACE\\timebargreen_1024x768.sti";
	}

	FilenameForBPP(fn, VObjectDesc.ImageFile);
	if( !AddVideoObject( &VObjectDesc, &uiPLAYERBAR ) )
	{ 
		sprintf(fn, "Missing %s", fn);
		AssertMsg(0, fn );	
	}
	
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebaryellow_Thin.sti";
		else
			fn = "INTERFACE\\timebaryellow.sti";
	}
	else if (iResolution < _1024x768)
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebaryellow_800x600Thin.sti";
		else
			fn = "INTERFACE\\timebaryellow_800x600.sti";
	}
	else
	{
		if (gGameExternalOptions.fSmallSizeProgressbar)
			fn = "INTERFACE\\timebaryellow_1024x768Thin.sti";
		else
			fn = "INTERFACE\\timebaryellow_1024x768.sti";
	}

	FilenameForBPP(fn, VObjectDesc.ImageFile);
	
	if( !AddVideoObject( &VObjectDesc, &uiINTBAR ) )
	{ 
		sprintf(fn, "Missing %s", fn);
		AssertMsg(0, fn );	
	}		
	
	SetFontDestBuffer( uiSurface , 0, 0, SCREEN_WIDTH , HEIGHT_PROGRESSBAR, FALSE );
	SetFont( TINYFONT1 );

	switch( ubType )
	{
		case COMPUTER_TURN_MESSAGE:
		case COMPUTER_INTERRUPT_MESSAGE:
		case MILITIA_INTERRUPT_MESSAGE:
		case AIR_RAID_TURN_MESSAGE:

			// Render rect into surface	
			BltVideoObjectFromIndex( uiSurface, uiBAR, 0, xResOffset, 0, VO_BLT_SRCTRANSPARENCY, NULL );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_WHITE );
			uiBarToUseInUpDate = uiBAR;
			fDoLimitBar				= TRUE;
			break;

		case PLAYER_INTERRUPT_MESSAGE:

			// Render rect into surface	
			BltVideoObjectFromIndex( uiSurface, uiINTBAR, 0, xResOffset, 0, VO_BLT_SRCTRANSPARENCY, NULL );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_WHITE );
			SetFontShadow( NO_SHADOW );
			uiBarToUseInUpDate = uiINTBAR;
			break;

		case PLAYER_TURN_MESSAGE:

			// Render rect into surface	
			BltVideoObjectFromIndex( uiSurface, uiPLAYERBAR, 0, xResOffset, 0, VO_BLT_SRCTRANSPARENCY, NULL );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_WHITE );
			SetFontShadow( NO_SHADOW );
			uiBarToUseInUpDate = uiPLAYERBAR;
			break;
	}
	
	if ( gGameOptions.fTurnTimeLimit )
	{
		if ( ubType == PLAYER_TURN_MESSAGE || ubType == PLAYER_INTERRUPT_MESSAGE )
		{
			fDoLimitBar = TRUE;
		}
	}

	if ( fDoLimitBar )
	{
		dNumStepsPerEnemy = (FLOAT)( (FLOAT)iProgBarLength / (FLOAT)gTacticalStatus.usTactialTurnLimitMax );

		// Render end peice
		sBarX = (SCREEN_WIDTH - xResSize) / 2 + 5;
		BltVideoObjectFromIndex( uiSurface, uiBarToUseInUpDate, 1, sBarX, PROG_BAR_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );

		// Determine Length
		dLength	= ( gTacticalStatus.usTactialTurnLimitCounter ) * dNumStepsPerEnemy;

		dCurSize = 0;
		cnt2 = 0;

		while( dCurSize < dLength )
		{
			sBarX++;

			// Check sBarX, ( just as a precaution )
			if ( sBarX > SCREEN_WIDTH - 1 )
			{
				break;
			}

			BltVideoObjectFromIndex( uiSurface, uiBarToUseInUpDate, (INT16)( 2 + cnt2 ), sBarX, PROG_BAR_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );

			dCurSize++;
			cnt2++;

			if ( cnt2 == 10 )
			{
				cnt2 = 0;
			}
		}

		// Do end...
		if ( gTacticalStatus.usTactialTurnLimitCounter == gTacticalStatus.usTactialTurnLimitMax )
		{
			sBarX++;
			BltVideoObjectFromIndex( uiSurface, uiBarToUseInUpDate, (INT16)( 2 + cnt2 ), sBarX, PROG_BAR_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			sBarX++;
			BltVideoObjectFromIndex( uiSurface, uiBarToUseInUpDate, (INT16)( 12 ), sBarX, PROG_BAR_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	// Delete rect
	DeleteVideoObjectFromIndex( uiBAR );
	DeleteVideoObjectFromIndex( uiINTBAR );
	DeleteVideoObjectFromIndex( uiPLAYERBAR );

	// Draw text....
	if(gGameExternalOptions.fSmallSizeProgressbar)
		FindFontCenterCoordinates( SCREEN_WIDTH/2, 2, 1, 1, psString, TINYFONT1, &sX, &sY );
	else
		FindFontCenterCoordinates( SCREEN_WIDTH/2, 7, 1, 1, psString, TINYFONT1, &sX, &sY );

	mprintf( sX, sY, psString );

	// Change back...
	SetFontDestBuffer( FRAME_BUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// Done!
	SetFontShadow( DEFAULT_SHADOW );

	gfTopMessageDirty = TRUE;
}

void TurnExpiredCallBack( UINT8 bExitValue )
{
	// End turn...
	UIHandleEndTurn( NULL );
}

void CheckForAndHandleEndPlayerTimeLimit( )
{
	if ( gTacticalStatus.fInTopMessage )
	{
		if ( gGameOptions.fTurnTimeLimit )
		{
			if ( gTacticalStatus.ubTopMessageType == PLAYER_TURN_MESSAGE || gTacticalStatus.ubTopMessageType == PLAYER_INTERRUPT_MESSAGE )
			{
				if ( gTacticalStatus.usTactialTurnLimitCounter == ( gTacticalStatus.usTactialTurnLimitMax - 1 ) )
				{
						// ATE: increase this so that we don't go into here again...
						gTacticalStatus.usTactialTurnLimitCounter++;

						// OK, set message that time limit has expired....
						//DoMessageBox( MSG_BOX_BASIC_STYLE, L"Turn has Expired!", GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, TurnExpiredCallBack, NULL );

						// End turn...
						UIHandleEndTurn( NULL );

				}
			}
		}
	}
}


void HandleTopMessages( )
{
	//UINT32		uiTime;
	blt_vs_fx BltFx;

	// OK, is out current count > 0 ?
		if ( gTacticalStatus.fInTopMessage )
	{
		//gfTopMessageDirty = TRUE;


		// ATE: If we are told to go into top message, but we have not
		// initialized it yet...
		// This is mostly for loading saved games.....
		if ( !gTopMessage.fCreated )
		{
			gfTopMessageDirty = TRUE;
			AddTopMessage( gTacticalStatus.ubTopMessageType, gTacticalStatus.zTopMessageString );
		}

		if ( gTacticalStatus.ubTopMessageType == COMPUTER_TURN_MESSAGE ||
				gTacticalStatus.ubTopMessageType == COMPUTER_INTERRUPT_MESSAGE ||
				gTacticalStatus.ubTopMessageType == MILITIA_INTERRUPT_MESSAGE ||
				gTacticalStatus.ubTopMessageType == AIR_RAID_TURN_MESSAGE )
		{
			// OK, update timer.....
			if ( TIMECOUNTERDONE( giTimerTeamTurnUpdate, PLAYER_TEAM_TIMER_SEC_PER_TICKS ) )
			{
				RESETTIMECOUNTER( giTimerTeamTurnUpdate, PLAYER_TEAM_TIMER_SEC_PER_TICKS );
				if(!is_networked)//hayden - stop clients from updating enemy progress bar...  //could have another param in here when get coop going
				{
				// Update counter....
				if ( gTacticalStatus.usTactialTurnLimitCounter < gTacticalStatus.usTactialTurnLimitMax )
				{
					gTacticalStatus.usTactialTurnLimitCounter++;
				}

				// Check if we have reach limit...
				if ( gTacticalStatus.usTactialTurnLimitCounter >= ( ( gubProgCurEnemy	) * PLAYER_TEAM_TIMER_TICKS_PER_ENEMY ) )
				{
					gTacticalStatus.usTactialTurnLimitCounter = ( ( gubProgCurEnemy ) * PLAYER_TEAM_TIMER_TICKS_PER_ENEMY );
				}
				
				}

				CreateTopMessage( gTopMessage.uiSurface, gTacticalStatus.ubTopMessageType, gTacticalStatus.zTopMessageString );
			}
		}
		else if ( gGameOptions.fTurnTimeLimit )
		{
			if ( gTacticalStatus.ubTopMessageType == PLAYER_TURN_MESSAGE || gTacticalStatus.ubTopMessageType == PLAYER_INTERRUPT_MESSAGE )
			{
				if ( !gfUserTurnRegionActive && !AreWeInAUIMenu() )
				{
						// Check Grace period...
						if ( ( GetJA2Clock( ) - gTacticalStatus.uiTactialTurnLimitClock ) > PLAYER_TEAM_TIMER_GRACE_PERIOD )
						{
							gTacticalStatus.uiTactialTurnLimitClock = 0;

							if ( TIMECOUNTERDONE( giTimerTeamTurnUpdate, PLAYER_TEAM_TIMER_SEC_PER_TICKS ) )
							{
								RESETTIMECOUNTER( giTimerTeamTurnUpdate, PLAYER_TEAM_TIMER_SEC_PER_TICKS );

								if ( gTacticalStatus.fTactialTurnLimitStartedBeep )
								{
									if ( ( GetJA2Clock( ) - gTopMessage.uiTimeSinceLastBeep ) > PLAYER_TEAM_TIMER_TIME_BETWEEN_BEEPS )
									{
										gTopMessage.uiTimeSinceLastBeep = GetJA2Clock( );

										// Start sample....
										PlayJA2SampleFromFile( "SOUNDS\\TURN_NEAR_END.WAV", RATE_11025, HIGHVOLUME, 1 , MIDDLEPAN );
									}
								}

								// OK, have we gone past the time to
								if ( !gTacticalStatus.fTactialTurnLimitStartedBeep && ( gTacticalStatus.usTactialTurnLimitMax - gTacticalStatus.usTactialTurnLimitCounter ) < PLAYER_TEAM_TIMER_TICKS_FROM_END_TO_START_BEEP )
								{
									gTacticalStatus.fTactialTurnLimitStartedBeep = TRUE;

									gTopMessage.uiTimeSinceLastBeep = GetJA2Clock( );

								}

								// Update counter....
								if ( gTacticalStatus.usTactialTurnLimitCounter < gTacticalStatus.usTactialTurnLimitMax )
								{
									gTacticalStatus.usTactialTurnLimitCounter++;
								}

								CreateTopMessage( gTopMessage.uiSurface, gTacticalStatus.ubTopMessageType, gTacticalStatus.zTopMessageString );

								// Have we reached max?
								if ( gTacticalStatus.usTactialTurnLimitCounter == ( gTacticalStatus.usTactialTurnLimitMax - 1) )
								{
									// IF we are not in lock ui mode....
									CheckForAndHandleEndPlayerTimeLimit( );
								}
							}
						}
				}
			}
		}

		// Set redner viewport value
		//* ddd gsVIEWPORT_WINDOW_START_Y = 20;
		gsVIEWPORT_WINDOW_START_Y = HEIGHT_PROGRESSBAR;

		// Check if we have scrolled...
		if ( gTopMessage.sWorldRenderX != gsRenderCenterX || gTopMessage.sWorldRenderY != gsRenderCenterY )
		{
			gfTopMessageDirty = TRUE;
		}

		if ( gfTopMessageDirty )
		{
			gTopMessage.sWorldRenderX = gsRenderCenterX;
			gTopMessage.sWorldRenderY = gsRenderCenterY;

			// Redner!
			BltFx.SrcRect.iLeft = 0;
			//* ddd BltFx.SrcRect.iTop	= 20 - gTopMessage.bYPos;
			if(gGameExternalOptions.fSmallSizeProgressbar)
				BltFx.SrcRect.iTop  = 0;
			else
				BltFx.SrcRect.iTop  = 20 - gTopMessage.bYPos;

			BltFx.SrcRect.iRight = SCREEN_WIDTH ;
			//*ddd BltFx.SrcRect.iBottom = 20;
			BltFx.SrcRect.iBottom = HEIGHT_PROGRESSBAR;

			BltVideoSurface( FRAME_BUFFER, gTopMessage.uiSurface, 0,
																		0, 0,
																		VS_BLT_SRCSUBRECT, &BltFx );

			// Save to save buffer....
			BltFx.SrcRect.iLeft = 0;
			//* ddd BltFx.SrcRect.iTop	= 0;
			if(gGameExternalOptions.fSmallSizeProgressbar)
				BltFx.SrcRect.iTop  = 0;
			else
				BltFx.SrcRect.iTop  = 20 - gTopMessage.bYPos;

			BltFx.SrcRect.iRight = SCREEN_WIDTH;
			//*ddd BltFx.SrcRect.iBottom = 20;
			BltFx.SrcRect.iBottom = HEIGHT_PROGRESSBAR;

			BltVideoSurface( guiSAVEBUFFER, FRAME_BUFFER, 0,
																		0, 0,
																		VS_BLT_SRCSUBRECT, &BltFx );

			//*ddd InvalidateRegion( 0, 0, SCREEN_WIDTH, 20 );
			InvalidateRegion( 0, 0, SCREEN_WIDTH, HEIGHT_PROGRESSBAR );

			gfTopMessageDirty = FALSE;
		}

	}
	else
	{
		// Set redner viewport value
	gsVIEWPORT_WINDOW_START_Y = 0;
	}
}


void EndTopMessage( )
{
//	blt_vs_fx BltFx;


	// OK, end the topmost message!
	if ( gTacticalStatus.fInTopMessage )
	{
		// Are we the last?
		//if ( gTopMessage.bCurrentMessage == 1 )
		{
			// We are....
			// Re-render our strip and then copy to the save buffer...
			gsVIEWPORT_WINDOW_START_Y = 0;
			gTacticalStatus.fInTopMessage = FALSE;

			SetRenderFlags( RENDER_FLAG_FULL );
			//RenderStaticWorldRect( 0, 0, 640, 20, TRUE );
			//gsVIEWPORT_WINDOW_START_Y = 20;

			// Copy into save buffer...
			//BltFx.SrcRect.iLeft = 0;
			//BltFx.SrcRect.iTop	= 0;
			//BltFx.SrcRect.iRight = 640;
			//BltFx.SrcRect.iBottom = 20;

			//BltVideoSurface( guiSAVEBUFFER, FRAME_BUFFER, 0,
			//															0, 0,
			//															VS_BLT_SRCSUBRECT, &BltFx );
		}
		//else
		//{
			// Render to save buffer
		//	CreateTopMessage( guiSAVEBUFFER, gTopMessageTypes[ 0 ], gzTopMessageStrings[ 0 ] );
		//}

		// Animate up...
		//gTopMessage.bAnimate = 1;
		// Set time of last update
		//gTopMessage.uiTimeOfLastUpdate = GetJA2Clock( ) + 150;

		// Handle first frame now...
		// HandleTopMessages( );

	}
}


BOOLEAN InTopMessageBarAnimation( )
{
	if ( gTacticalStatus.fInTopMessage )
	{
		if ( 	gTopMessage.bAnimate != 0 )
		{
			HandleTopMessages( );

			return( TRUE );
		}

	}

	return( FALSE );
}


void PauseRT( BOOLEAN fPause )
{
	//StopMercAnimation( fPause );

	if ( fPause )
	{
	//	PauseGame( );
	}
	else
	{
	//	UnPauseGame( );
	}
}


void InitEnemyUIBar( UINT8 ubNumEnemies, UINT8 ubDoneEnemies )
{
	// OK, set value
	gubProgNumEnemies = ubNumEnemies + ubDoneEnemies;
	gubProgCurEnemy		= ubDoneEnemies;
	gfProgBarActive = TRUE;

	gTacticalStatus.usTactialTurnLimitCounter = ubDoneEnemies * PLAYER_TEAM_TIMER_TICKS_PER_ENEMY;
	gTacticalStatus.usTactialTurnLimitMax = ( (ubNumEnemies + ubDoneEnemies) * PLAYER_TEAM_TIMER_TICKS_PER_ENEMY );

}

void UpdateEnemyUIBar( )
{
	// Are we active?
	if ( gfProgBarActive )
	{
		// OK, update team limit counter....
		gTacticalStatus.usTactialTurnLimitCounter = ( gubProgCurEnemy * PLAYER_TEAM_TIMER_TICKS_PER_ENEMY );

		gubProgCurEnemy++;

	}

	// Do we have an active enemy bar?
	if ( gTacticalStatus.fInTopMessage )
	{
		if ( gTacticalStatus.ubTopMessageType == COMPUTER_TURN_MESSAGE )
		{
			// Update message!
			CreateTopMessage( gTopMessage.uiSurface, COMPUTER_TURN_MESSAGE, gTacticalStatus.zTopMessageString );
		}
	}
}


void InitPlayerUIBar( BOOLEAN fInterrupt )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32				cnt = 0;
	INT8				bNumOK = 0, bNumNotOK = 0;

	if ( !gGameOptions.fTurnTimeLimit )
	{
		if ( fInterrupt == TRUE )
		{
			AddTopMessage( PLAYER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
		}
		else
		{
			//EndTopMessage();
			AddTopMessage( PLAYER_TURN_MESSAGE, TeamTurnString[0] );
		}
		return;
	}

	if (is_networked)
		gTacticalStatus.usTactialTurnLimitMax = 0;//hayden , cheap hack, always calc time...
	
	// OK, calculate time....
	if ( !fInterrupt || gTacticalStatus.usTactialTurnLimitMax == 0 )
	{
		gTacticalStatus.usTactialTurnLimitCounter = 0;

		// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

		// look for all mercs on the same team,
		for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++)
		{
			// Are we active and in sector.....
			if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
			{
				if ( pTeamSoldier->stats.bLife < OKLIFE )
				{
					bNumNotOK++;
				}
				else
				{
					bNumOK++;
				}
			}
		}

		gTacticalStatus.usTactialTurnLimitMax = ( bNumOK * PLAYER_TEAM_TIMER_TICKS_PER_OK_MERC ) + ( bNumNotOK * PLAYER_TEAM_TIMER_TICKS_PER_NOTOK_MERC );
	}
	else
	{
		if ( gTacticalStatus.usTactialTurnLimitCounter > PLAYER_TEAM_TIMER_INTTERUPT_GRACE )
		{
			gTacticalStatus.usTactialTurnLimitCounter -= PLAYER_TEAM_TIMER_INTTERUPT_GRACE;
		}
	}

	gTacticalStatus.uiTactialTurnLimitClock		= 0;
	gTacticalStatus.fTactialTurnLimitStartedBeep = FALSE;

	// RESET COIUNTER...
	RESETTIMECOUNTER( giTimerTeamTurnUpdate, PLAYER_TEAM_TIMER_SEC_PER_TICKS );


	// OK, set value
	if ( fInterrupt != TRUE )
	{
		AddTopMessage( PLAYER_TURN_MESSAGE, TeamTurnString[0] );
	}
	else
	{
		AddTopMessage( PLAYER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
	}

}


void MovementMenuBackregionCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		CancelMovementMenu( );
	}
}

void DoorMenuBackregionCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		CancelOpenDoorMenu( );
	}
}

STR16 GetSoldierHealthString( SOLDIERTYPE *pSoldier )
{
	// sevenfm: show enemy health as text only when SHOW_ENEMY_HEALTH = 1
	if ( ( gGameExternalOptions.ubShowEnemyHealth != 1 ) && (pSoldier->bTeam == ENEMY_TEAM || pSoldier->bTeam == CREATURE_TEAM))
	{
		return L"";
	}
	else	
	{
		// Flugente: display if we are a prisoner of war
		if ( pSoldier->usSoldierFlagMask & SOLDIER_POW )
			return zHealthStr[ 7 ];

		INT32 cnt, cntStart;
		if( pSoldier->stats.bLife == pSoldier->stats.bLifeMax )
		{
			cntStart = 4;
		}
		else
		{
			cntStart = 0;
		}
		//Show health on others.........
		for ( cnt = cntStart; cnt < 6; cnt ++ )
		{
			if ( pSoldier->stats.bLife < bHealthStrRanges[ cnt ] )
			{
				break;
			}
		}
		return zHealthStr[ cnt ];
	}
}


typedef struct
{
	INT8				bHeight;
	INT8				bPower;
	INT32 sGridNo;
	UINT8				ubLevel;
	SOLDIERTYPE	*pSoldier;
	BOOLEAN			fShowHeight;
	BOOLEAN			fShowPower;
	BOOLEAN			fActiveHeightBar;
	BOOLEAN			fActivePowerBar;
	BOOLEAN			fAtEndHeight;
	INT32 sTargetGridNo;
	FLOAT				dInitialForce;
	FLOAT				dForce;
	FLOAT				dDegrees;
	FLOAT				dMaxForce;
	UINT8				ubPowerIndex;

} AIMCUBE_UI_DATA;

static BOOLEAN					gfInAimCubeUI = FALSE;
static AIMCUBE_UI_DATA	gCubeUIData;

#define	GET_CUBES_HEIGHT_FROM_UIHEIGHT( h )	( 32 + ( h * 64 ) )



void CalculateAimCubeUIPhysics( )
{
	UINT8	ubHeight;

	ubHeight = GET_CUBES_HEIGHT_FROM_UIHEIGHT( gCubeUIData.bHeight );

	if ( gCubeUIData.fActiveHeightBar )
	{

		// OK, determine which power to use.....
		// TODO this: take force / max force * 10....
		gCubeUIData.ubPowerIndex = (UINT8)( gCubeUIData.dForce / gCubeUIData.dMaxForce * 10 );

	}

	if ( gCubeUIData.fActivePowerBar )
	{
		gCubeUIData.dForce = ( gCubeUIData.dMaxForce * ( (FLOAT)gCubeUIData.ubPowerIndex / (FLOAT)10 ) );

		// Limit to the max force...
		if ( gCubeUIData.dForce > gCubeUIData.dMaxForce )
		{
			gCubeUIData.dForce = gCubeUIData.dMaxForce;
		}

		gCubeUIData.dDegrees = (FLOAT)CalculateLaunchItemAngle( gCubeUIData.pSoldier, gCubeUIData.sGridNo, ubHeight, gCubeUIData.dForce, &(gCubeUIData.pSoldier->inv[ HANDPOS ] ), &(gCubeUIData.sTargetGridNo ) );
	}

}


INT32 GetInAimCubeUIGridNo( )
{
	return( gCubeUIData.sGridNo );
}

BOOLEAN InAimCubeUI( )
{
	return( gfInAimCubeUI );
}

BOOLEAN AimCubeUIClick( )
{
	if ( !gfInAimCubeUI )
	{
		return( FALSE );
	}

	// If we have clicked, and we are only on height, continue with power
	if ( gCubeUIData.fActiveHeightBar && gCubeUIData.bHeight != 0 )
	{
		gCubeUIData.fShowPower				= TRUE;
		gCubeUIData.fActiveHeightBar	= FALSE;
		gCubeUIData.fActivePowerBar		= TRUE;

		return( FALSE );
	}
	else
	{
		return( TRUE );
	}
}

void BeginAimCubeUI( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 ubLevel, UINT8 bStartPower, INT8 bStartHeight )
{
	gfInAimCubeUI = TRUE;

	gCubeUIData.sGridNo			= sGridNo;
	gCubeUIData.ubLevel			= ubLevel;
	gCubeUIData.pSoldier		= pSoldier;
	gCubeUIData.bPower			= bStartPower;
	gCubeUIData.bHeight			= bStartHeight;
	gCubeUIData.fShowHeight = TRUE;
	gCubeUIData.fShowPower	= FALSE;
	gCubeUIData.fActivePowerBar		= FALSE;
	gCubeUIData.fActiveHeightBar	= TRUE;
	gCubeUIData.fAtEndHeight	= FALSE;
	gCubeUIData.dDegrees		= (float)PI/4;


	// Calculate Iniital force....
	CalculateAimCubeUIPhysics( );

}


void EndAimCubeUI( )
{
	gfInAimCubeUI = FALSE;
}

void IncrementAimCubeUI( )
{
	if ( gCubeUIData.fActiveHeightBar )
	{
		// Cycle the last height yellow once
		if ( gCubeUIData.bHeight == 3 )
		{
			if ( gCubeUIData.fAtEndHeight )
			{
				gCubeUIData.bHeight = 0;
				gCubeUIData.fAtEndHeight = 0;
			}
			else
			{
				gCubeUIData.fAtEndHeight = TRUE;
			}
		}
		else
		{
			gCubeUIData.bHeight++;
		}

		CalculateAimCubeUIPhysics( );

	}

	if ( gCubeUIData.fActivePowerBar )
	{
		if ( gCubeUIData.ubPowerIndex == 10 )
		{
			UINT8	ubHeight;

			ubHeight = GET_CUBES_HEIGHT_FROM_UIHEIGHT( gCubeUIData.bHeight );

			// Start back to basic7
			gCubeUIData.dDegrees = (FLOAT)(PI/4);
			gCubeUIData.dInitialForce = gCubeUIData.dForce;

			// OK, determine which power to use.....
			// TODO this: take force / max force * 10....
			gCubeUIData.ubPowerIndex = (UINT8)( gCubeUIData.dForce / gCubeUIData.dMaxForce * 10 );
		}

		// Cycle the last height yellow once
		gCubeUIData.ubPowerIndex++;

		CalculateAimCubeUIPhysics( );

	}

}


void SetupAimCubeAI( )
{
	if ( gfInAimCubeUI )
	{
		AddTopmostToHead( gCubeUIData.sTargetGridNo, FIRSTPOINTERS2 );
		gpWorldLevelData[ gCubeUIData.sTargetGridNo ].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
		gpWorldLevelData[ gCubeUIData.sTargetGridNo ].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;

		//AddTopmostToHead( gCubeUIData.sGridNo, FIRSTPOINTERS2 );
		//gpWorldLevelData[ gCubeUIData.sGridNo ].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
		//gpWorldLevelData[ gCubeUIData.sGridNo ].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;

	}
}


void ResetAimCubeAI( )
{
	if ( gfInAimCubeUI )
	{
		RemoveTopmost( gCubeUIData.sTargetGridNo, FIRSTPOINTERS2 );
		//RemoveTopmost( gCubeUIData.sGridNo, FIRSTPOINTERS2 );
	}
}

void RenderAimCubeUI( )
{
	INT16 sScreenX = 0, sScreenY = 0;
	INT32	cnt;
	INT16	sBarHeight;
	INT32 iBack;
	INT8	bGraphicNum;

	if ( gfInAimCubeUI )
	{

		// OK, given height
		if ( gCubeUIData.fShowHeight )
		{
			// Determine screen location....
			GetGridNoScreenPos( gCubeUIData.sGridNo, gCubeUIData.ubLevel, &sScreenX, &sScreenY );

			// Save background
			iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sScreenX, (INT16)(sScreenY - 70 ), (INT16)(sScreenX + 40 ), (INT16)(sScreenY + 50 ) );
			if ( iBack != -1 )
			{
				SetBackgroundRectFilled( iBack );
			}

			sBarHeight = 0;
			bGraphicNum = 0;

			if ( gCubeUIData.bHeight == 3 && gCubeUIData.fAtEndHeight )
			{
				bGraphicNum = 1;
			}

			// Do first level....
			BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
			sBarHeight -= 3;
			BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );

			// Loop through height.....
			for ( cnt = 1; cnt <= gCubeUIData.bHeight; cnt++ )
			{
				sBarHeight -= 3;
				BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
				sBarHeight -= 3;
				BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
				sBarHeight -= 3;
				BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
				sBarHeight -= 3;
				BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMCUBES, bGraphicNum, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
			}
		}


		if ( gCubeUIData.fShowPower )
		{
			sBarHeight = -50;

			BltVideoObjectFromIndex( FRAME_BUFFER, guiAIMBARS, gCubeUIData.ubPowerIndex, sScreenX, ( sScreenY + sBarHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
		}

	}

}

void GetLaunchItemParamsFromUI( )
{


}


static BOOLEAN gfDisplayPhysicsUI = FALSE;
INT32	 gsPhysicsImpactPointGridNo = NOWHERE;
static INT8		gbPhysicsImpactPointLevel = 0;
static BOOLEAN gfBadPhysicsCTGT = FALSE;

void BeginPhysicsTrajectoryUI( INT32 sGridNo, INT8 bLevel, BOOLEAN fBadCTGT )
{
	gfDisplayPhysicsUI					= TRUE;
	gsPhysicsImpactPointGridNo	= sGridNo;
	gbPhysicsImpactPointLevel		= bLevel;
	gfBadPhysicsCTGT						= fBadCTGT;
}

void EndPhysicsTrajectoryUI( )
{
	gfDisplayPhysicsUI = FALSE;

}

void SetupPhysicsTrajectoryUI( )
{
	if ( gfDisplayPhysicsUI && gfUIHandlePhysicsTrajectory )
	{
		if ( gbPhysicsImpactPointLevel == 0 )
		{
			if ( gfBadPhysicsCTGT )
			{
				AddTopmostToHead( gsPhysicsImpactPointGridNo, FIRSTPOINTERS12 );
			}
			else
			{
				AddTopmostToHead( gsPhysicsImpactPointGridNo, FIRSTPOINTERS8 );
			}
			gpWorldLevelData[ gsPhysicsImpactPointGridNo ].pTopmostHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[ gsPhysicsImpactPointGridNo ].pTopmostHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
		}
		else
		{
			if ( gfBadPhysicsCTGT )
			{
				AddOnRoofToHead( gsPhysicsImpactPointGridNo, FIRSTPOINTERS12 );
			}
			else
			{
				AddOnRoofToHead( gsPhysicsImpactPointGridNo, FIRSTPOINTERS8 );
			}
			gpWorldLevelData[ gsPhysicsImpactPointGridNo ].pOnRoofHead->ubShadeLevel=DEFAULT_SHADE_LEVEL;
			gpWorldLevelData[ gsPhysicsImpactPointGridNo ].pOnRoofHead->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
		}
	}

}

void ResetPhysicsTrajectoryUI( )
{
	if ( gfDisplayPhysicsUI )
	{
		RemoveTopmost( gsPhysicsImpactPointGridNo, FIRSTPOINTERS8 );
		RemoveTopmost( gsPhysicsImpactPointGridNo, FIRSTPOINTERS12 );
		RemoveOnRoof( gsPhysicsImpactPointGridNo, FIRSTPOINTERS8 );
		RemoveOnRoof( gsPhysicsImpactPointGridNo, FIRSTPOINTERS12 );
	}

}

void DirtyTopMessage( )
{
	gTopMessage.fCreated	= FALSE;
}



UINT32 CalcUIMessageDuration( STR16 wString )
{
	// base + X per letter
	return( 1000 + 50 * wcslen( wString ) );
}


BOOLEAN	gfMultipurposeLocatorOn = FALSE;
UINT32	guiMultiPurposeLocatorLastUpdate;
INT8		gbMultiPurposeLocatorFrame;
INT32     gsMultiPurposeLocatorGridNo;
INT8		gbMultiPurposeLocatorLevel;
INT8		gbMultiPurposeLocatorCycles;

void BeginMultiPurposeLocator( INT32 sGridNo, INT8 bLevel, BOOLEAN fSlideTo )
{
	guiMultiPurposeLocatorLastUpdate = 0;
	gbMultiPurposeLocatorCycles		= 0;
	gbMultiPurposeLocatorFrame		= 0;
	gfMultipurposeLocatorOn = TRUE;

	gsMultiPurposeLocatorGridNo = sGridNo;
	gbMultiPurposeLocatorLevel	= bLevel;

	if ( fSlideTo )
	{
		// FIRST CHECK IF WE ARE ON SCREEN
	if ( GridNoOnScreen( sGridNo ) )
	{
		return;
	}

	// sGridNo here for DG compatibility
	gTacticalStatus.sSlideTarget = sGridNo;
	gTacticalStatus.sSlideReason = NOBODY;

	// Plot new path!
	gfPlotNewMovement = TRUE;
	}
}


void HandleMultiPurposeLocator( )
{
	UINT32			uiClock;

	if ( !gfMultipurposeLocatorOn )
	{
	return;
	}

	// Update radio locator
	uiClock = GetJA2Clock( );

	// Update frame values!
	if ( ( uiClock - guiMultiPurposeLocatorLastUpdate ) > 80 )
	{
		guiMultiPurposeLocatorLastUpdate = uiClock;

		// Update frame
		gbMultiPurposeLocatorFrame++;

		if ( gbMultiPurposeLocatorFrame == 5 )
		{
			gbMultiPurposeLocatorFrame = 0;
		gbMultiPurposeLocatorCycles++;
		}

	if ( gbMultiPurposeLocatorCycles == 8 )
	{
		gfMultipurposeLocatorOn	= FALSE;
	}
	}
}



void RenderTopmostMultiPurposeLocator( )
{
	FLOAT				dOffsetX, dOffsetY;
	FLOAT				dTempX_S, dTempY_S;
	INT16				sX, sY, sXPos, sYPos;
	INT32				iBack;

	if ( !gfMultipurposeLocatorOn )
	{
	return;
	}

	ConvertGridNoToCenterCellXY( gsMultiPurposeLocatorGridNo, &sX, &sY );

	dOffsetX = (FLOAT)( sX - gsRenderCenterX );
	dOffsetY = (FLOAT)( sY - gsRenderCenterY );

	// Calculate guy's position
	FloatFromCellToScreenCoordinates( dOffsetX, dOffsetY, &dTempX_S, &dTempY_S );

	sXPos = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) + (INT16)dTempX_S;
	sYPos = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) + (INT16)dTempY_S - gpWorldLevelData[ gsMultiPurposeLocatorGridNo ].sHeight;

	// Adjust for offset position on screen
	sXPos -= gsRenderWorldOffsetX;
	sYPos -= gsRenderWorldOffsetY;

	// Adjust for render height
	sYPos += gsRenderHeight;

	// Adjust for level height
	if ( gbMultiPurposeLocatorLevel )
	{
		sYPos -= ROOF_LEVEL_HEIGHT;
	}

	// Center circle!
	sXPos -= 20;
	sYPos -= 20;

	iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos +40 ), (INT16)(sYPos + 40 ) );
	if ( iBack != -1 )
	{
		SetBackgroundRectFilled( iBack );
	}

	BltVideoObjectFromIndex(	FRAME_BUFFER, guiRADIO, gbMultiPurposeLocatorFrame, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );
}

void DrawBar( INT32 x, INT32 y, INT32 width, INT32 height, UINT16 color8, UINT16 color16, UINT8 *pDestBuf )
{
	if( width > 0 )
	{
		for( INT32 i=0; i < height; i++ )
		{
			if(gbPixelDepth==16)
				LineDraw( TRUE, x, y+i, x+width-1, y+i, color16, pDestBuf );
			else if(gbPixelDepth==8)
				LineDraw8( TRUE, x, y+i, x+width-1, y+i, color8, pDestBuf );
		}
	}
}

BOOLEAN CoverColorCode( INT8 cover, INT16 &color8, INT16 &color16 ){
	BOOLEAN showCover = TRUE;
	switch(cover)
	{
	case 0:
		//color8 = COLOR_RED;
		color8 = FONT_MCOLOR_RED;
		color16 = Get16BPPColor( FROMRGB( 220, 0, 0 ) );
		break;
	case 1:
		color8 = COLOR_ORANGE;
		color16 = Get16BPPColor( FROMRGB( 220, 140, 0 ) );
		break;
	case 2:
		color8 = COLOR_YELLOW;
		color16 = Get16BPPColor( FROMRGB( 220, 220, 0 ) );
		break;
	case 3:
		color8 = COLOR_GREEN;
		color16 = Get16BPPColor( FROMRGB( 0, 200, 0 ) );
		break;
	default: 
		showCover = FALSE;
	}
	return showCover;
}

void DrawRankIcon( INT8 rank, INT32 baseX, INT32 baseY )
{
	UINT32		uiDestPitchBYTES;
	UINT8		*pDestBuf;
	INT16		color8;
	INT16		color16;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, ( gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y ) );

	if(rank<6)
	{
		color8 = FONT_DKGREEN;
		color16 = Get16BPPColor( FROMRGB( 20, 60, 20 ) );
		//color8 = FONT_ORANGE;
		//color16 = Get16BPPColor( FROMRGB( 160, 80, 0 ) );
	}
	else
	{
		color8 = FONT_DKRED;
		color16 = Get16BPPColor( FROMRGB( 60, 20, 20 ) );
	}

	DrawBar( baseX+2, baseY+2, 7, 8, color8, color16, pDestBuf );
	DrawLine( baseX+3, baseY+10, baseX+7, baseY+10, color8, color16, pDestBuf );
	DrawLine( baseX+4, baseY+11, baseX+6, baseY+11, color8, color16, pDestBuf );

	if(rank<6)
	{
		color8 = COLOR_GREEN;
		color16 = Get16BPPColor( FROMRGB( 20, 80, 20 ) );
		//color8 = FONT_YELLOW;
		//color16 = Get16BPPColor( FROMRGB( 200, 120, 00 ) );
	}
	else
	{
		color8 = COLOR_RED;
		color16 = Get16BPPColor( FROMRGB( 120, 20, 20 ) );
	}

	DrawLine( baseX+2, baseY+1, baseX+8, baseY+1, color8, color16, pDestBuf );
	DrawLine( baseX+1, baseY+2, baseX+1, baseY+9, color8, color16, pDestBuf );
	DrawLine( baseX+9, baseY+2, baseX+9, baseY+9, color8, color16, pDestBuf );
	
	DrawLine( baseX+1, baseY+9, baseX+4, baseY+12, color8, color16, pDestBuf );
	DrawLine( baseX+4, baseY+12, baseX+6, baseY+12, color8, color16, pDestBuf );
	DrawLine( baseX+6, baseY+12, baseX+9, baseY+9, color8, color16, pDestBuf );

	if(rank<6)
	{
		//color8 = FONT_DKGREEN;
		//color16 = Get16BPPColor( FROMRGB( 0, 40, 0 ) );
		//color8 = COLOR_LTGREY;
		//color16 = Get16BPPColor( FROMRGB( 120, 120, 120 ) );
		color8 = COLOR_YELLOW;
		color16 = Get16BPPColor( FROMRGB( 200, 200, 200 ) );
	}
	else
	{
		color8 = FONT_ORANGE;
		color16 = Get16BPPColor( FROMRGB( 220, 140, 20 ) );
	}

	switch( rank )
	{
	case 1:
		DrawLine( baseX+3, baseY+6, baseX+7, baseY+6, color8, color16, pDestBuf );
		break;
	case 2:
		DrawLine( baseX+3, baseY+5, baseX+7, baseY+5, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+7, baseX+7, baseY+7, color8, color16, pDestBuf );
		break;
	case 3:
		DrawLine( baseX+3, baseY+4, baseX+7, baseY+4, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+6, baseX+7, baseY+6, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+8, baseX+7, baseY+8, color8, color16, pDestBuf );
		break;
	case 4:
		DrawLine( baseX+3, baseY+5, baseX+5, baseY+8, color8, color16, pDestBuf );
		DrawLine( baseX+5, baseY+8, baseX+7, baseY+5, color8, color16, pDestBuf );
		break;
	case 5:
		DrawLine( baseX+3, baseY+4, baseX+7, baseY+4, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+6, baseX+5, baseY+9, color8, color16, pDestBuf );
		DrawLine( baseX+5, baseY+9, baseX+7, baseY+6, color8, color16, pDestBuf );
		break;
	case 6:
		DrawLine( baseX+5, baseY+4, baseX+5, baseY+8, color8, color16, pDestBuf );
		break;
	case 7:
		DrawLine( baseX+4, baseY+4, baseX+4, baseY+8, color8, color16, pDestBuf );
		DrawLine( baseX+6, baseY+4, baseX+6, baseY+8, color8, color16, pDestBuf );
		break;
	case 8:
		DrawLine( baseX+5, baseY+4, baseX+5, baseY+8, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+4, baseX+3, baseY+8, color8, color16, pDestBuf );
		DrawLine( baseX+7, baseY+4, baseX+7, baseY+8, color8, color16, pDestBuf );
		break;
	case 9:
		DrawLine( baseX+2, baseY+6, baseX+8, baseY+6, color8, color16, pDestBuf );
		DrawLine( baseX+5, baseY+3, baseX+5, baseY+9, color8, color16, pDestBuf );
		DrawLine( baseX+3, baseY+4, baseX+7, baseY+8, color8, color16, pDestBuf );
		DrawLine( baseX+7, baseY+4, baseX+3, baseY+8, color8, color16, pDestBuf );
	}
	
	UnLockVideoSurface( FRAME_BUFFER );
}

void DrawItemPic(INVTYPE *pItem, INT16 sX, INT16 sY )
{
	UINT16			usGraphicNum;

	usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;	

	HVOBJECT		hVObject;
	ETRLEObject     *pTrav;
	GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );
	pTrav = &(hVObject->pETRLEObject[ usGraphicNum ] );

	BltVideoObjectOutlineFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sX - pTrav->sOffsetX, sY - pTrav->sOffsetY, Get16BPPColor( FROMRGB( 255, 255, 255 ) ), FALSE );
}

void GetItemDimensions( INVTYPE *pItem, INT16 &sWidth, INT16 &sHeight )
{
	HVOBJECT		hVObject;
	ETRLEObject     *pTrav;
	UINT16			usGraphicNum;

	usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;	
	GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );
	pTrav = &(hVObject->pETRLEObject[ usGraphicNum ] );

	sWidth = (UINT32)(pTrav->usWidth);
	sHeight = (UINT32)(pTrav->usHeight);	
}

BOOLEAN ShowExactInfo( SOLDIERTYPE* pSoldier, SOLDIERTYPE* pTargetSoldier )
{
	INT32 range;
	INT32 maxExactWeaponDistance;

	if( pTargetSoldier->bVisible == -1 )
		return FALSE;

	range = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, pTargetSoldier->sGridNo ) / 10;

	// visible distance
	maxExactWeaponDistance = (INT32)( pSoldier->GetMaxDistanceVisible( pTargetSoldier->sGridNo, 0, CALC_FROM_WANTED_DIR) ) / 2 ;

	// apply experience level factor
	maxExactWeaponDistance = (INT32)( maxExactWeaponDistance * ( 1 + FLOAT( EffectiveExpLevel( pSoldier ) ) / 10.0f ) ); 

	if ( maxExactWeaponDistance >= range )
		return TRUE;

	return FALSE;
}

void DrawNCTHCursorItemPics( INT16 sStartScreenX, INT16 sStartScreenY  )
{
	// find target soldier
	SOLDIERTYPE *pSoldier;	
	SOLDIERTYPE		*pTargetSoldier;
	INT32			usItemID = 0;
	INT16			sX, sY, sWidth, sHeight;
	INT16			sXOffset = 0;

	GetSoldier( &pSoldier, gusSelectedSoldier );
	GetSoldier( &pTargetSoldier, gusUIFullTargetID );

	// zero rects
	RightRect.left = 0;		RightRect.top = 0;		RightRect.right = 0;		RightRect.bottom = 0;
	LeftRect.left = 0;		LeftRect.top = 0;		LeftRect.right = 0;			LeftRect.bottom = 0;
	LeftRect2.left = 0;		LeftRect2.top = 0;		LeftRect2.right = 0;		LeftRect2.bottom = 0;
	
	if( gGameExternalOptions.ubAdditionalNCTHCursorInfo &&
		gfUIBodyHitLocation &&
		pSoldier->ubBodyType <= REGFEMALE &&
		_KeyDown( ALT ) &&
		ShowExactInfo( pSoldier, pTargetSoldier) )
	{
		// show armour
		switch( pSoldier->bAimShotLocation )
		{
		case AIM_SHOT_HEAD:
			if( pTargetSoldier->inv[HELMETPOS].exists() )
				usItemID = pTargetSoldier->inv[HELMETPOS].usItem;
			break;
		case AIM_SHOT_TORSO:
			if( pTargetSoldier->inv[VESTPOS].exists() )
				usItemID = pTargetSoldier->inv[VESTPOS].usItem;
			break;
		case AIM_SHOT_LEGS:
			if( pTargetSoldier->inv[LEGPOS].exists() )
				usItemID = pTargetSoldier->inv[LEGPOS].usItem;
			break;
		}
		if( usItemID )
		{
			sX = sStartScreenX+25;
			sY = sStartScreenY-3;
			DrawItemPic( &(Item[ usItemID ] ), sX, sY );
			GetItemDimensions( &(Item[ usItemID ]), sWidth, sHeight );
			RightRect.left = sX;
			RightRect.top = sY;
			RightRect.right = sX + sWidth;
			RightRect.bottom = sY + sHeight;
		}			

		if( gGameExternalOptions.ubAdditionalNCTHCursorInfo > 1 )
		{
			// show item in hands
			if( pSoldier->bAimShotLocation == AIM_SHOT_TORSO )
			{
				if( pTargetSoldier->inv[HANDPOS].exists() )
				{
					usItemID = pTargetSoldier->inv[HANDPOS].usItem;
					GetItemDimensions( &(Item[ usItemID ]), sWidth, sHeight );
					if( sWidth > 30 )
						sXOffset = 10;
					sX = sStartScreenX - 25 - sWidth + sXOffset;
					sY = sStartScreenY - 3;
					DrawItemPic(&(Item[ usItemID ] ), sX, sY );
					LeftRect.left = sX;
					LeftRect.top = sY;
					LeftRect.right = sX + sWidth;
					LeftRect.bottom = sY + sHeight;
				}
			}
			// show items in head slots
			else if( pSoldier->bAimShotLocation == AIM_SHOT_HEAD )
			{
				if( pTargetSoldier->inv[HEAD2POS].exists() )
				{
					usItemID = pTargetSoldier->inv[HEAD2POS].usItem;
					GetItemDimensions( &(Item[ usItemID ]), sWidth, sHeight );
					sX = sStartScreenX - 25 - sWidth;
					sY = sStartScreenY - 3;
					DrawItemPic(&(Item[ usItemID ] ), sX, sY );
					LeftRect.left = sX;
					LeftRect.top = sY;
					LeftRect.right = sX + sWidth;
					LeftRect.bottom = sY + sHeight;
				}

				if( pTargetSoldier->inv[HEAD1POS].exists() )
				{					
					usItemID = pTargetSoldier->inv[HEAD1POS].usItem;
					GetItemDimensions( &(Item[ usItemID ]), sWidth, sHeight );					
					sX = sStartScreenX - 25 - sWidth;
					sY = sStartScreenY - 3;
					if( pTargetSoldier->inv[HEAD2POS].exists() )
						sX -= LeftRect.right - LeftRect.left;
					DrawItemPic(&(Item[ usItemID ] ), sX, sY );
					LeftRect2.left = sX;
					LeftRect2.top = sY;
					LeftRect2.right = sX + sWidth;
					LeftRect2.bottom = sY + sHeight;
				}
			}
		}
	}
}

void GetEnemyInfoString( SOLDIERTYPE* pSelectedSoldier, SOLDIERTYPE* pTargetSoldier, BOOLEAN showExactInfo, CHAR16 *NameStr )
{
	UINT16 usItemID;

	// if ALT pressed - show armour instead of weapon
	if( gfUIBodyHitLocation &&
		_KeyDown( ALT ) )
	{						
		SetFontForeground( FONT_YELLOW );
		// show armour
		usItemID = 0;
		switch( pSelectedSoldier->bAimShotLocation )
		{
		case AIM_SHOT_HEAD:
			if( pTargetSoldier->inv[HELMETPOS].exists() )
				usItemID = pTargetSoldier->inv[HELMETPOS].usItem;
			break;
		case AIM_SHOT_TORSO:
			if( pTargetSoldier->inv[VESTPOS].exists() )
				usItemID = pTargetSoldier->inv[VESTPOS].usItem;
			break;
		case AIM_SHOT_LEGS:
			if( pTargetSoldier->inv[LEGPOS].exists() )
				usItemID = pTargetSoldier->inv[LEGPOS].usItem;
			break;
		}
		if( usItemID )
		{
			if( showExactInfo && gGameExternalOptions.fShowEnemyExtendedInfo )
			{	// in range, show exact armour name
				swprintf( NameStr, L"%s", ItemNames[ usItemID ] );
			}
			else
			{	// show general armour type
				switch( pSelectedSoldier->bAimShotLocation )
				{
				case AIM_SHOT_HEAD:
					swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_HELMET] );
					//swprintf( NameStr, L"%s", L"Helmet" );
					break;
				case AIM_SHOT_TORSO:
					swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_VEST] );
					//swprintf( NameStr, L"%s", L"Vest" );
					break;
				case AIM_SHOT_LEGS:
					swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_LEGGINGS] );
					//swprintf( NameStr, L"%s", L"Leggings" );
					break;
				}								
			}
		}
	}
	else
	{						
		if( gfUIBodyHitLocation &&
			pSelectedSoldier->bAimShotLocation == AIM_SHOT_HEAD )
		{
			// show face items
			SetFontForeground( FONT_ORANGE );
			if( pTargetSoldier->inv[HEAD1POS].exists() )
			{
				if(gGameExternalOptions.fShowEnemyExtendedInfo)
				{	// show exact name
					wcscat( NameStr, ItemNames[ pTargetSoldier->inv[HEAD1POS].usItem ] );
				}
				else
				{	// show general name
					if( Item[pTargetSoldier->inv[HEAD1POS].usItem].gasmask )
						wcscat( NameStr, TacticalStr[ GENERAL_INFO_MASK ] );
					else if( Item[pTargetSoldier->inv[HEAD1POS].usItem].nightvisionrangebonus || Item[pTargetSoldier->inv[HEAD1POS].usItem].cavevisionrangebonus )
						wcscat( NameStr, TacticalStr[ GENERAL_INFO_NVG ] );
				}				
			}
			if( pTargetSoldier->inv[HEAD2POS].exists() )
			{
				if( pTargetSoldier->inv[HEAD1POS].exists() )
					wcscat( NameStr, L", " );
				if(gGameExternalOptions.fShowEnemyExtendedInfo)
				{	// show exact name
					wcscat( NameStr, ItemNames[ pTargetSoldier->inv[HEAD2POS].usItem ] );
				}
				else
				{	// show general name
					if( Item[pTargetSoldier->inv[HEAD1POS].usItem].gasmask )
						wcscat( NameStr, TacticalStr[ GENERAL_INFO_MASK ] );
					else if( Item[pTargetSoldier->inv[HEAD1POS].usItem].nightvisionrangebonus || Item[pTargetSoldier->inv[HEAD1POS].usItem].cavevisionrangebonus )
						wcscat( NameStr, TacticalStr[ GENERAL_INFO_NVG ] );
				}
			}
		}
		else
		{
			// show weapon
			SetFontForeground( FONT_ORANGE );
			if ( WeaponInHand( pTargetSoldier ) )
			{
				if ( showExactInfo )
				{
					swprintf( NameStr, L"%s", ShortItemNames[ pTargetSoldier->inv[ HANDPOS ].usItem ] );
				}
				else
				{
					// display general weapon class
					switch( Weapon[pTargetSoldier->inv[ HANDPOS ].usItem].ubWeaponClass )
					{
					case HANDGUNCLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_HANDGUN] );
						break;
					case SMGCLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_SMG] );
						break;
					case RIFLECLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_RIFLE] );
						break;
					case MGCLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_MG] );
						break;
					case SHOTGUNCLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_SHOTGUN] );
						break;
					case KNIFECLASS:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_KNIFE] );
						break;
					default:
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_HEAVY_WEAPON] );
						break;
					}
				}
			}
			else
			{
				if( showExactInfo )
				{							
					if( pTargetSoldier->inv[ HANDPOS ].usItem )
						swprintf( NameStr, L"%s", Item[ pTargetSoldier->inv[ HANDPOS ].usItem ].szItemName );
					else
						swprintf( NameStr, L"%s", gzTooltipStrings[STR_TT_NO_WEAPON] );
				}
				else
				{
					if( pTargetSoldier->inv[ HANDPOS ].usItem )
						swprintf( NameStr, L"%s", TacticalStr[ GENERAL_INFO_ITEM ] );
					else
						swprintf( NameStr, L"%s", L"" );
				}								
			}
		}
	}
}

void ShowEnemyWeapon( INT16 sX, INT16 sY, SOLDIERTYPE* pTargetSoldier )
{
	SOLDIERTYPE *pSelectedSoldier;
	BOOLEAN		showExactInfo = FALSE;
	CHAR16		NameStr[ MAX_ENEMY_NAMES_CHARS ];
	UINT16 usTotalWidth;
	INT32 iRange;
	INT32 iVisibleDistance;

	if ( gusSelectedSoldier != NOBODY )
		pSelectedSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if( gGameExternalOptions.fEnemyRank || gGameExternalOptions.fEnemyNames )
		return;

	iRange = GetRangeInCellCoordsFromGridNoDiff( pSelectedSoldier->sGridNo, pTargetSoldier->sGridNo ) / 10;
	iVisibleDistance = (INT32)( pSelectedSoldier->GetMaxDistanceVisible( pTargetSoldier->sGridNo, 0, CALC_FROM_WANTED_DIR) ) ;

	if( iRange > iVisibleDistance )
		return;

	if( !pTargetSoldier->bVisible )
		return;

	// calc max range for exact info
	if ( gusSelectedSoldier != NOBODY )
		showExactInfo = ShowExactInfo( pSelectedSoldier, pTargetSoldier );

	// show weapon/armour/items info
	if ( gGameExternalOptions.fShowEnemyWeapon && gTacticalStatus.ubCurrentTeam == OUR_TEAM && pTargetSoldier->ubBodyType <= REGFEMALE )
	{
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );		

		swprintf( NameStr, L"" );
		GetEnemyInfoString( pSelectedSoldier, pTargetSoldier, showExactInfo, NameStr );
		usTotalWidth = StringPixLength ( NameStr, TINYFONT1 );
		sX -= usTotalWidth/2;
		
		// print item name
		gprintfdirty( sX, sY + 10, NameStr );
		mprintf( sX, sY + 10, NameStr );
	}
}

void ShowAdditionalInfo( INT16 sX, INT16 sY, SOLDIERTYPE* pTargetSoldier )
{
	BOOLEAN		showExactInfo;
	CHAR16		NameStr[ MAX_ENEMY_NAMES_CHARS ];
	SOLDIERTYPE *pSelectedSoldier;

	if ( gusSelectedSoldier != NOBODY )
		pSelectedSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if( gGameExternalOptions.fEnemyRank || gGameExternalOptions.fEnemyNames )
		return;	

	// calc max range for exact info
	if ( gusSelectedSoldier != NOBODY )
		showExactInfo = ShowExactInfo( pSelectedSoldier, pTargetSoldier );

	sX -= StringPixLength ( L"*", TINYFONT1 );
	if( gGameExternalOptions.fShowEnemyAwareness && gTacticalStatus.ubCurrentTeam == OUR_TEAM )
	{
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		// show awareness sign only when in stealth mode or disguised
		if( ( gusSelectedSoldier != NOBODY ) &&
			( pTargetSoldier->bTeam == ENEMY_TEAM || pTargetSoldier->bTeam == CIV_TEAM && !pTargetSoldier->aiData.bNeutral ) &&
			( MercPtrs[ gusSelectedSoldier ]->bStealthMode || MercPtrs[ gusSelectedSoldier ]->usSoldierFlagMask & ( SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER ) ) )
		{
			if( pTargetSoldier->aiData.bOppList[ MercPtrs[ gusSelectedSoldier ]->ubID ] == SEEN_CURRENTLY )
				swprintf( NameStr, L"%s", L"*" );
			else
				swprintf( NameStr, L"%s", L"-" );

			SetFontForeground( FONT_GRAY1 );
			switch ( pTargetSoldier->aiData.bAlertStatus )
			{
			case STATUS_GREEN:							// everything's OK, no suspicion
				SetFontForeground( FONT_GREEN );
				break;
			case STATUS_YELLOW:							// he or his friend heard something
				SetFontForeground( FONT_YELLOW );
				break;
			case STATUS_RED:                            // has definite evidence of opponent
				SetFontForeground( FONT_ORANGE );
				break;
			case STATUS_BLACK:							// currently sees an active opponent
				SetFontForeground( FONT_DKRED );
				break;
			}

			if ( showExactInfo )
			{
				// this feature is a part of 'show enemy health bar'
				if( gGameExternalOptions.ubShowEnemyHealth > 1 )
				{
					gprintfdirty( sX - 15, sY, NameStr );
					mprintf( sX - 15, sY, NameStr );	
				}
			}
		}
	}
}

void ShowEnemyHealthBar( INT16 sX, INT16 sY, SOLDIERTYPE* pSoldier )
{
	INT32	iBack;
	UINT8 ubLines = gGameExternalOptions.ubShowEnemyHealth - 1;
	INT32 iBarWidth = 24;
	INT32 iBarHeight;
	SOLDIERTYPE *pSelectedSoldier;

	if ( gusSelectedSoldier != NOBODY )
		pSelectedSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;
	
	if ( pSelectedSoldier->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY )
		return;

	// show enemy health bar
	if( gGameExternalOptions.ubShowEnemyHealth > 1 && gTacticalStatus.ubCurrentTeam == OUR_TEAM )
	{
		iBarHeight = 2 + 3 * ubLines - 1;
		sY += 10;
		sY -= iBarHeight;
		sX -= iBarWidth / 2;

		iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, sX, sY, (INT16)(sX + iBarWidth), (INT16)(sY + iBarHeight ) );
		if ( iBack != -1 )
			SetBackgroundRectFilled( iBack );

		DrawEnemyHealthBar( pSoldier, sX, sY, ubLines, iBarWidth - 2, iBarHeight );
	}
}

void ShowRankIcon( INT16 sXPos, INT16 sYPos, SOLDIERTYPE* pSoldier )
{
	INT32	iBack;
	INT16 sX, sY;
	SOLDIERTYPE *pSelectedSoldier;

	if ( gusSelectedSoldier != NOBODY )
		pSelectedSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;
	
	if ( pSelectedSoldier->aiData.bOppList[pSoldier->ubID] != SEEN_CURRENTLY )
		return;

	if( gGameExternalOptions.ubShowEnemyRankIcon && gTacticalStatus.ubCurrentTeam == OUR_TEAM )
		//&& (!gGameExternalOptions.fEnemyNames) && (!gGameExternalOptions.fEnemyRank))
	{
		sX = sXPos + 50;
		sY = sYPos + 17;
		if(gGameExternalOptions.fEnemyNames)
			sY+=10;
		if(gGameExternalOptions.fEnemyRank)
			sY+=10;

		iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, sX, sY, (INT16)(sX + 11 ), (INT16)(sY + 14 ) );
		if ( iBack != -1 )
			SetBackgroundRectFilled( iBack );

		DrawRankIcon( pSoldier->stats.bExpLevel, sX, sY );
	}
}

void DrawLine( INT32 x1, INT32 y1, INT32 x2, INT32 y2, UINT16 color8, UINT16 color16, UINT8 *pDestBuf )
{
	if(gbPixelDepth==16)
		LineDraw( TRUE, x1, y1, x2, y2, color16, pDestBuf);
	else
		LineDraw8( TRUE, x1, y1, x2, y2, color8, pDestBuf);
}

void DrawEnemyHealthBar( SOLDIERTYPE* pSoldier, INT32 sX, INT32 sY, UINT8 ubLines, INT32 iBarWidth, INT32 iBarHeight )
{
	FLOAT		dWidth, dPercentage;
	UINT32		uiDestPitchBYTES;
	UINT8		*pDestBuf;
	INT32		sHeight = 2;

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, ( gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y ) );

	// sevenfm: draw background
	DrawBar( sX, sY, iBarWidth + 2, iBarHeight, COLOR_BLACK, Get16BPPColor( FROMRGB( 0, 0, 0 ) ), pDestBuf );

	// draw health
	if(ubLines > 0)
	{
		dPercentage = (FLOAT)pSoldier->stats.bLife / (FLOAT) pSoldier->stats.bLifeMax;
		dWidth			=	dPercentage * iBarWidth;
		dWidth = __min (dWidth, iBarWidth);
		DrawBar( sX + 1, sY + 1, (INT32)dWidth, sHeight, COLOR_RED, Get16BPPColor( FROMRGB( 200, 0, 0	) ), pDestBuf );
	}

	// draw ap
	if(ubLines > 1)
	{
		if( pSoldier->bActionPoints >0 )
		{
			//dPercentage = (FLOAT)pSoldier->bActionPoints / (FLOAT) ( pSoldier->bActionPoints + pSoldier->ubAPsLostToSuppression + pSoldier->u);
			dPercentage = (FLOAT)pSoldier->bActionPoints / (FLOAT) ( pSoldier->bInitialActionPoints);
			dWidth = dPercentage * iBarWidth;
			dWidth = __min (dWidth, iBarWidth);
			//DrawBar( sX + 1, sY + 1 + (sHeight+1), (INT32)dWidth, sHeight, COLOR_BLUE, Get16BPPColor( FROMRGB( 20, 20, 220 ) ), pDestBuf );
			DrawBar( sX + 1, sY + 1 + (sHeight+1), (INT32)dWidth, sHeight, COLOR_PURPLE, Get16BPPColor( FROMRGB( 110, 30, 210 ) ), pDestBuf );
		}
		else
		{
			dPercentage = (FLOAT)pSoldier->bActionPoints / (FLOAT) ( APBPConstants[AP_MIN_LIMIT] );
			dWidth	= dPercentage * iBarWidth;
			dWidth = __min (dWidth, iBarWidth);
			DrawBar( sX + 1, sY + 1 + (sHeight+1), (INT32)dWidth, sHeight, COLOR_LTGREY, Get16BPPColor( FROMRGB( 140, 140, 140	) ), pDestBuf );
		}
	}

	// draw suppression shock
	if( ubLines > 2 )
	{
		dPercentage = (FLOAT)( __min(100, ( (FLOAT)100 * CalcEffectiveShockLevel( pSoldier ) ) / CalcSuppressionTolerance( pSoldier ) ) ) / (FLOAT)100;
		dWidth = dPercentage * iBarWidth;
		dWidth = __min (dWidth, iBarWidth);
		DrawBar( sX+1, sY + 1 + 2*(sHeight+1), (INT32)dWidth, sHeight, COLOR_ORANGE, Get16BPPColor( FROMRGB( 200, 120, 0 ) ), pDestBuf );
	}

	// draw morale
	if( ubLines > 3 )
	{
		dPercentage = (FLOAT)( pSoldier->aiData.bAIMorale ) / (FLOAT)( MORALE_FEARLESS );
		//dPercentage = (FLOAT)( pSoldier->aiData.bMorale ) / (FLOAT)( 100 );
		dWidth = dPercentage * iBarWidth;
		dWidth = __min (dWidth, iBarWidth);
		DrawBar( sX+1, sY + 1 + 3*(sHeight+1), (INT32)dWidth, sHeight, COLOR_GREEN, Get16BPPColor( FROMRGB( 0, 140, 0 ) ), pDestBuf );
	}

	// draw BP
	if( ubLines > 4 )
	{
		dPercentage = (FLOAT)( pSoldier->bBreath ) / (FLOAT)( pSoldier->bBreathMax );
		dWidth = dPercentage * iBarWidth;
		dWidth = __min (dWidth, iBarWidth);
		DrawBar( sX+1, sY + 1 + 4*(sHeight+1), (INT32)dWidth, sHeight, COLOR_BLUE, Get16BPPColor( FROMRGB( 0, 0, 200 ) ), pDestBuf );
	}

	UnLockVideoSurface( FRAME_BUFFER );
}

// Flugente: show enemy role
BOOLEAN ShowSoldierRoleSymbol(SOLDIERTYPE* pSoldier)
{
	// this only works on enemy soldiers
	if ( pSoldier->bTeam != ENEMY_TEAM && pSoldier->bTeam != CIV_TEAM )
		return false;

	if ( pSoldier->usSkillCounter[SOLDIER_COUNTER_ROLE_OBSERVED] < gGameExternalOptions.usTurnsToUncover )
		return false;

	INT16 sXPos = 0;
	INT16 sYPos = 0;
	INT32 iBack = 0;

	GetSoldierAboveGuyPositions( pSoldier, &sXPos, &sYPos, TRUE );

	// Adjust for bars!
	sXPos += 50;
	sYPos += 25;

	// are we a VIP? show that only when the player knows a VIP is in this sector. otherwise, don't even show our officer property
	if ( pSoldier->usSoldierFlagMask & SOLDIER_VIP && !pSoldier->bSectorZ )
	{
		if ( PlayerKnowsAboutVIP( pSoldier->sSectorX, pSoldier->sSectorY ) )
		{
			// Add bars
			iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20), (INT16)(sYPos + 20) );

			if ( iBack != -1 )
			{
				SetBackgroundRectFilled( iBack );
			}

			BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 6, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

			sYPos += 20;
		}
	}

	// is this guy an officer?
	if ( pSoldier->usSoldierFlagMask & SOLDIER_ENEMY_OFFICER )
	{
		// Add bars
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20 ), (INT16)(sYPos + 20 ) );

		if ( iBack != -1 )
		{
			SetBackgroundRectFilled( iBack );
		}

		// if we look at this guy long enough, we might even learn that he is an advanced officer
		if ( pSoldier->usSkillCounter[SOLDIER_COUNTER_ROLE_OBSERVED] > 1.5 * gGameExternalOptions.usTurnsToUncover && NUM_SKILL_TRAITS( pSoldier, SQUADLEADER_NT ) > 1 )
			BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 5, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );
		else
			BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 0, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

		sYPos += 20;
	}

	// is this guy radio operator with a radio?
	if ( pSoldier->CanUseRadio(false) )
	{
		// Add bars
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20 ), (INT16)(sYPos + 20 ) );

		if ( iBack != -1 )
		{
			SetBackgroundRectFilled( iBack );
		}

		BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 3, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

		sYPos += 20;
	}

	// is this guy a doctor? we do not check for medical gear, as we would not have that information from a glance
	if ( HAS_SKILL_TRAIT( pSoldier, DOCTOR_NT) )
	{
		// Add bars
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20 ), (INT16)(sYPos + 20 ) );

		if ( iBack != -1 )
		{
			SetBackgroundRectFilled( iBack );
		}

		BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 1, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

		sYPos += 20;
	}

	// is this guy a sniper? Just check for the gun, trait is not necessary
	if ( pSoldier->HasSniper() )
	{
		// Add bars
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20 ), (INT16)(sYPos + 20 ) );

		if ( iBack != -1 )
		{
			SetBackgroundRectFilled( iBack );
		}

		BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 2, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

		sYPos += 20;
	}

	// is this guy a mortar guy? Just check for the gun, trait is not necessary
	if ( pSoldier->HasMortar() )
	{
		// Add bars
		iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos + 20 ), (INT16)(sYPos + 20 ) );

		if ( iBack != -1 )
		{
			SetBackgroundRectFilled( iBack );
		}

		BltVideoObjectFromIndex( FRAME_BUFFER, guiENEMYROLES, 4, sXPos, sYPos, VO_BLT_TRANSSHADOW, NULL );

		sYPos += 20;
	}

	return true;
}


void NCTHImprovedAPColor( SOLDIERTYPE* pSoldier, OBJECTTYPE* pWeapon )
{
	if( gGameExternalOptions.ubImprovedNCTHCursor > 0 )
	{
		// compare Aps needed to shoot with shooter's APs
		INT16 sRemainingAP = pSoldier->bActionPoints - gsCurrentActionPoints;			
		INT16 sModifiedReloadAP = Weapon[Item[pWeapon->usItem].ubClassIndex].APsToReloadManually;
		if( sModifiedReloadAP > 0 )
		{
			if ( Item[ pWeapon->usItem ].usItemClass == IC_GUN )
				sModifiedReloadAP *= gItemSettings.fAPtoReloadManuallyModifierGun[ Weapon[ pWeapon->usItem ].ubWeaponType ];
			else if ( Item[ pWeapon->usItem ].usItemClass == IC_LAUNCHER )
				sModifiedReloadAP *= gItemSettings.fAPtoReloadManuallyModifierLauncher;
			if ( gGameOptions.fNewTraitSystem )
			{
				// Sniper trait makes chambering a round faster
				if (( Weapon[Item[pWeapon->usItem].ubClassIndex].ubWeaponType == GUN_SN_RIFLE || Weapon[Item[pWeapon->usItem].ubClassIndex].ubWeaponType == GUN_RIFLE ) && HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ))
					sModifiedReloadAP = (INT16)(((sModifiedReloadAP * (100 - gSkillTraitValues.ubSNChamberRoundAPsReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )))/100) + 0.5);
				// Ranger trait makes pumping shotguns faster
				else if (( Weapon[Item[pWeapon->usItem].ubClassIndex].ubWeaponType == GUN_SHOTGUN ) && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ))
					sModifiedReloadAP = (INT16)(((sModifiedReloadAP * (100 - gSkillTraitValues.ubRAPumpShotgunsAPsReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT )))/100) + 0.5);
			}
		}
		INT16 sPointsToShoot = CalcTotalAPsToAttack( pSoldier, gCTHDisplay.iTargetGridNo, 0, pSoldier->aiData.bShownAimTime ) + sModifiedReloadAP;

		// we can shoot twice
		if( sRemainingAP >= sPointsToShoot )
		{
			SetFontForeground(FONT_MCOLOR_LTYELLOW);
		}
		// at least we have min ap for interrupt (enough to interrupt somebody or take cover after shooting)
		else if( sRemainingAP >= APBPConstants[MIN_APS_TO_INTERRUPT] )
		{
			SetFontForeground(FONT_ORANGE);
		}
		// too low AP
		else
		{
			SetFontForeground(FONT_MCOLOR_DKRED);
		}
	}
}

void NCTHDrawLaserDot( UINT16* ptrBuf, UINT32 uiPitch, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, INT16 sStartScreenX, INT16 sStartScreenY, FLOAT fLaserBonus, FLOAT fBrightnessModifier, FLOAT fEffectiveLaserRatio, FLOAT zOffset )
{
	if( gGameExternalOptions.ubImprovedNCTHCursor > 0 )
	{
		//UINT16 usLaserBonus = fLaserBonus * fBrightnessModifier * fEffectiveLaserRatio;
		UINT16 usLaserBonus;
		// we want to show relative effectiveness of laser, not it's absolute value
		if( fLaserBonus > 0 )
			usLaserBonus = 100 * fBrightnessModifier * fEffectiveLaserRatio;
		else
			usLaserBonus = 0;
		UINT16 usCLaserDot = Get16BPPColor( FROMRGB( 130 + usLaserBonus, 0, 0 ) );
		UINT16 usCLaserDotHigh = Get16BPPColor( FROMRGB( 150 + usLaserBonus, 0, 0 ) );
		UINT16 usCLaserDotLow = Get16BPPColor( FROMRGB( 100 + usLaserBonus, 0, 0 ) );

		// if laser visibility is at least 10%
		if(!gfCannotGetThrough && usLaserBonus > 10)
		{
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, sStartScreenY+1+(INT16)zOffset, usCLaserDotLow );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, sStartScreenY+1+(INT16)zOffset, usCLaserDot );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, sStartScreenY+1+(INT16)zOffset, usCLaserDotLow );

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, sStartScreenY+(INT16)zOffset, usCLaserDot );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, sStartScreenY+(INT16)zOffset, usCLaserDotHigh );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, sStartScreenY+(INT16)zOffset, usCLaserDot );

			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX-1, sStartScreenY-1+(INT16)zOffset, usCLaserDotLow );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX, sStartScreenY-1+(INT16)zOffset, usCLaserDot );
			DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+1, sStartScreenY-1+(INT16)zOffset, usCLaserDotLow );
		}
	}
}

void NCTHCorrectMaxAperture( FLOAT& iAperture, FLOAT& iDistanceAperture, UINT16& usCApertureBar )
{
	if( gGameExternalOptions.ubImprovedNCTHCursor > 0 )
	{
		INT32 iMaxShownAperture;
		INT32 iMinShownAperture = 5 ;
		if( gGameExternalOptions.ubImprovedNCTHCursor > 1 )
			iMaxShownAperture = 50;
		else
			iMaxShownAperture = 100;
		if( iAperture > iMaxShownAperture )
			usCApertureBar	= Get16BPPColor( FROMRGB( 160, 0, 0 ) );
		iDistanceAperture = __min( iDistanceAperture, iMaxShownAperture );
		iAperture = __min( iAperture, iMaxShownAperture );
		iAperture = __max( iAperture, iMinShownAperture );
	}
}

void NCTHDrawScopeModeIcon( SOLDIERTYPE* pSoldier, INT16 sNewX, INT16 sNewY )
{
	if( gGameExternalOptions.ubImprovedNCTHCursor > 0 )
	{
		if ( gGameExternalOptions.fScopeModes && Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN )
		{
			UINT32 uiItemInfoAdvancedIcon = 0;
			VOBJECT_DESC    VObjectDesc;
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			strcpy( VObjectDesc.ImageFile, "INTERFACE\\ItemInfoAdvancedIcons.STI" );
			AddVideoObject( &VObjectDesc, &uiItemInfoAdvancedIcon);

			std::map<INT8, OBJECTTYPE*> ObjList;
			GetScopeLists(pSoldier, &pSoldier->inv[HANDPOS], ObjList);

			if ( pSoldier->bScopeMode == USE_ALT_WEAPON_HOLD )
			{		
				BltVideoObjectFromIndex( FRAME_BUFFER, uiItemInfoAdvancedIcon, 56, sNewX, sNewY+1, VO_BLT_TRANSSHADOW, NULL );
			}
			else if (ObjList[pSoldier->bScopeMode] != NULL && IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_SCOPE ) )
			{					
				BltVideoObjectFromIndex( FRAME_BUFFER, uiItemInfoAdvancedIcon, 54, sNewX, sNewY+1, VO_BLT_TRANSSHADOW, NULL );
			}
			// improved iron sights are attachable iron sights (the 'normal' iron sight is the gun itself)
			else if (ObjList[pSoldier->bScopeMode] != NULL &&  IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_IRONSIGHT ) )
			{
				BltVideoObjectFromIndex( FRAME_BUFFER, uiItemInfoAdvancedIcon, 52, sNewX, sNewY-1, VO_BLT_TRANSSHADOW, NULL );
			}
			else if (ObjList[pSoldier->bScopeMode] != NULL && IsAttachmentClass(ObjList[pSoldier->bScopeMode]->usItem, AC_SIGHT ) )
			{
				BltVideoObjectFromIndex( FRAME_BUFFER, uiItemInfoAdvancedIcon, 53, sNewX, sNewY, VO_BLT_TRANSSHADOW, NULL );
			}
			else
			{
				BltVideoObjectFromIndex( FRAME_BUFFER, uiItemInfoAdvancedIcon, 52, sNewX, sNewY-1, VO_BLT_TRANSSHADOW, NULL );
			}								
		}
	}
}

void NCTHShowAimLevels( SOLDIERTYPE* pSoldier, INT16 curX, INT16 curY )
{
	CHAR16 pStr[256];
	INT8 ubAllowedLevels = AllowedAimingLevels( pSoldier, gCTHDisplay.iTargetGridNo );

	// only show Aim Levels when cursor is on target
	if( gfUIFullTargetFound )
	{
		SetFont( TINYFONT1 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		if( ubAllowedLevels == 0 )
			SetFontForeground( FONT_MCOLOR_LTGRAY );
		else if ( pSoldier->aiData.bShownAimTime == ubAllowedLevels )		
			SetFontForeground( FONT_MCOLOR_LTYELLOW );
		else
			SetFontForeground( FONT_MCOLOR_LTGRAY );

		if( ubAllowedLevels > 0 )
			swprintf( pStr, L"%d/%d", pSoldier->aiData.bShownAimTime, ubAllowedLevels );
		else
			swprintf( pStr, L"-/-" );
		FindFontCenterCoordinates( (INT16)AimRect.left, (INT16)AimRect.top, (INT16)AimRect.right-(INT16)AimRect.left, 10, pStr, TINYFONT1, &curX, &curY);
		gprintfdirty( curX, curY, pStr );
		mprintf( curX, curY, pStr);

		UINT16 usTotalWidth = StringPixLength( pStr, TINYFONT1 ) + 6;
		INT16 sCenter = (INT16)((AimRect.right + AimRect.left) / 2);
		AimRect.left = sCenter - (usTotalWidth / 2);
		AimRect.right = sCenter + (usTotalWidth / 2);
	}
}

void NCTHShowMounted( SOLDIERTYPE* pSoldier, UINT16* ptrBuf, UINT32 uiPitch, INT16 sLeft, INT16 sTop, INT16 sRight, INT16 sBottom, INT16 sStartScreenX, INT16 sStartScreenY, INT16 zOffset )
{
	if( gGameExternalOptions.ubImprovedNCTHCursor > 2 )
	{
		OBJECTTYPE* pWeapon = pSoldier->GetUsedWeapon( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
		INVTYPE	*pItem = &Item[ pWeapon->usItem ];
		UINT16 usCMountedBar	= Get16BPPColor( FROMRGB( 192, 0, 0 ) );
		UINT16 usCMountedBorder	= Get16BPPColor( FROMRGB( 10, 10, 10 ) );
		if ( gGameExternalOptions.fWeaponResting && !gfCannotGetThrough && pItem->usItemClass & (IC_GUN | IC_LAUNCHER) && pSoldier->IsWeaponMounted() )
		{			
			for(INT32 cnt=-5;cnt<=5;cnt++)
			{
				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, sStartScreenY+8+(INT16)zOffset, usCMountedBorder );
				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, sStartScreenY+9+(INT16)zOffset, usCMountedBar );
				DrawCTHPixelToBuffer( ptrBuf, uiPitch, sLeft, sTop, sRight, sBottom, sStartScreenX+cnt, sStartScreenY+10+(INT16)zOffset, usCMountedBorder );
			}
		}
	}
}

// Flugente: check a profile for a background flag without using SOLDIERTYPE
BOOLEAN	HasBackgroundFlag( UINT8 usProfile, UINT64 aFlag )
{
	if ( gGameOptions.fBackGround && usProfile != NO_PROFILE )
	{
		if ( zBackground[gMercProfiles[usProfile].usBackground].uiFlags & aFlag )
			return TRUE;
	}

	return FALSE;
}

INT16 GetBackgroundValue( UINT8 usProfile, UINT16 aNr )
{
	if ( gGameOptions.fBackGround && usProfile != NO_PROFILE )
	{
		return zBackground[gMercProfiles[usProfile].usBackground].value[aNr];
	}

	return 0;
}