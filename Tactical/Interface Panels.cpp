#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
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
	#include "renderworld.h"
	#include "cursors.h"
	#include "radar screen.h"
	#include "Font Control.h"
	#include "render dirty.h"
	#include "utilities.h"
	#include "Sound Control.h"
	#include "Interface Panels.h"
	#include "Animation Control.h"
	#include "Soldier Control.h"
	#include "pathai.h"
	#include "weapons.h"
	#include "lighting.h"
	#include "faces.h"
	#include "mapscreen.h"
	#include "message.h"
	#include "text.h"
	#include "Interface Items.h"
	#include "interface control.h"
	#include "interface utils.h"
	#include "Game Clock.h"
	#include "mapscreen.h"
	#include "Soldier Macros.h"
	#include "strategicmap.h"
	#include "soldier functions.h"
	#include "gamescreen.h"
	#include "Assignments.h"
	#include "points.h"
	#include "Squads.h"
	#include "strategic.h"
	#include "Map Screen Interface Map.h"
	#include "overhead map.h"
	#include "Map Screen Interface.h"
	#include "Options Screen.h"
	#include "ShopKeeper Interface.h"
	#include "english.h"
	#include "keys.h"
	#include "Soldier Add.h"
	#include "vehicles.h"
	#include "gamesettings.h"
	#include "dialogue control.h"
	#include "Items.h"
	#include "drugs and alcohol.h"
	#include "los.h"
	#include "opplist.h"
	#include "vobject_blitters.h"
	#include "Finances.h"
	#include "LaptopSave.h"
	#include "Cursor Control.h"
	#include "MessageBoxScreen.h"
	#include "wordwrap.h"
	#include "Boxing.h"
	#include "personnel.h"
	// HEADROCK HAM 3.6: This is required for Stat Progress Bars
	#include "Campaign.h"
	#include "Food.h"	// added by Flugente
#endif

//legion by Jazz
#include "Interface Utils.h"
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


/*
 *
 *								SINGLE MERC BAR COORDS
 *
 *
 *	My opinion about this bars and GUI
 *	i would be very happy if we had c++ classes then we could made somethis like modules
 *	and peoples could very easly make their ow panels, button, labels and other
 *	i have got some ideas but it will tak some time to implement then.
 *	it will be hard and time consuming but maby i will show ya all direction how i would
 *	do it and peoples will continue that path :) joker
 */

/*	Ok so this array values had a constant initializer INV_INTERFACE_START_Y wich was constant
 *	we need do load SCREEN_WIDTH _HEIGHT from file to make JA more customizable so if we
 *	will make SCREEN_xxx viariable then xxx_INTERFACE_xxx will be also variable.
 *	When we declare static array like here (below) we need constant initializer but we need dynamic
 *	array so i declare only array and i will initielize it in run-time.
 *	any questions?
 *	joker
 */

// CHRISL: Change key value to use dynamically generated NUM_INV_SLOTS instead of static setting
INV_REGION_DESC gSMInvPocketXY[NUM_INV_SLOTS];	// ARRAY FOR INV PANEL INTERFACE ITEM POSITIONS
INV_REGION_DESC gSMCamoXY;			// X, Y Location of cammo region

/*	So this coords buttons, bars etc. also ware declered here as constant and we also need them
 *	more customizable in future so i just declare them and i will initialize them later
 *	any questions? joker
 */
int SM_ITEMDESC_START_X;
int SM_ITEMDESC_START_Y;
int SM_ITEMDESC_HEIGHT;
int SM_ITEMDESC_WIDTH;
extern INT16 ITEMDESC_START_X;
extern INT16 ITEMDESC_START_Y;

int SM_SELMERC_AP_X;
int SM_SELMERC_AP_Y;
int SM_SELMERC_AP_HEIGHT;
int SM_SELMERC_AP_WIDTH;

int SM_SELMERC_HEALTH_X;
int SM_SELMERC_HEALTH_Y;
int SM_SELMERC_HEALTH_WIDTH;
int SM_SELMERC_HEALTH_HEIGHT;

int SM_SELMERC_BREATH_X;
int SM_SELMERC_BREATH_Y;
int SM_SELMERC_BREATH_WIDTH;
int SM_SELMERC_BREATH_HEIGHT;

int SM_SELMERC_MORALE_X;
int SM_SELMERC_MORALE_Y;
int SM_SELMERC_MORALE_WIDTH;
int SM_SELMERC_MORALE_HEIGHT;

int SM_SELMERC_BARS_TIP_X;
int SM_SELMERC_BARS_TIP_Y;
int SM_SELMERC_BARS_TIP_WIDTH;
int SM_SELMERC_BARS_TIP_HEIGHT;

int SM_SELMERCNAME_X;
int SM_SELMERCNAME_Y;
int SM_SELMERCNAME_WIDTH;
int SM_SELMERCNAME_HEIGHT;

int SM_SELMERC_FACE_X;
int SM_SELMERC_FACE_Y;
int SM_SELMERC_FACE_HEIGHT;
int SM_SELMERC_FACE_WIDTH;

int SM_SELMERC_PLATE_X;
int SM_SELMERC_PLATE_Y;
int SM_SELMERC_PLATE_HEIGHT;
int SM_SELMERC_PLATE_WIDTH;

int SM_BODYINV_X;
int SM_BODYINV_Y;

int STATS_TITLE_FONT_COLOR;
int STATS_TEXT_FONT_COLOR;

int SM_TALKB_X;
int SM_TALKB_Y;
int SM_MUTEB_X;
int SM_MUTEB_Y;
int SM_STANCEUPB_X;
int SM_STANCEUPB_Y;
int SM_UPDOWNB_X;
int SM_UPDOWNB_Y;
int SM_CLIMBB_X;
int SM_CLIMBB_Y;
int SM_STANCEDOWNB_X;
int SM_STANCEDOWNB_Y;
int SM_HANDCURSORB_X;
int SM_HANDCURSORB_Y;
int SM_PREVMERCB_X;
int SM_PREVMERCB_Y;
int SM_NEXTMERCB_X;
int SM_NEXTMERCB_Y;
int SM_OPTIONSB_X;
int SM_OPTIONSB_Y;
int SM_BURSTMODEB_X;
int SM_BURSTMODEB_Y;
int SM_LOOKB_X;
int SM_LOOKB_Y;
int SM_STEALTHMODE_X;
int SM_STEALTHMODE_Y;
int SM_DONE_X;
int SM_DONE_Y;
int SM_MAPSCREEN_X;
int SM_MAPSCREEN_Y;
// CHRISL: backpack buttons
int SM_DROPPACK_X;
int SM_DROPPACK_Y;
int SM_ZIPPER_X;
int SM_ZIPPER_Y;
extern int INV_BDROP_X;
extern int INV_BDROP_Y;

int SM_POSITIONB_X;
int SM_POSITIONB_Y;
int SM_POSITIONB_WIDTH;
int SM_POSITIONB_HEIGHT;

int SM_PERCENT_WIDTH;
int SM_PERCENT_HEIGHT;
int SM_ARMOR_X;
int SM_ARMOR_Y;
int SM_ARMOR_LABEL_X;
int SM_ARMOR_LABEL_Y;
int SM_ARMOR_PERCENT_X;
int SM_ARMOR_PERCENT_Y;

int SM_WEIGHT_LABEL_X;
int SM_WEIGHT_LABEL_Y;
int SM_WEIGHT_PERCENT_X;
int SM_WEIGHT_PERCENT_Y;
int SM_WEIGHT_X;
int SM_WEIGHT_Y;

int SM_CAMMO_LABEL_X;
int SM_CAMMO_LABEL_Y;
int SM_CAMMO_PERCENT_X;
int SM_CAMMO_PERCENT_Y;
int SM_CAMMO_X;
int SM_CAMMO_Y;

int SM_STATS_WIDTH;
int SM_STATS_HEIGHT;
int SM_AGI_X;
int SM_AGI_Y;
int SM_DEX_X;
int SM_DEX_Y;
int SM_STR_X;
int SM_STR_Y;
int SM_CHAR_X;
int SM_CHAR_Y;
int SM_WIS_X;
int SM_WIS_Y;
int SM_EXPLVL_X;
int SM_EXPLVL_Y;
int SM_MRKM_X;
int SM_MRKM_Y;
int SM_MECH_X;
int SM_MECH_Y;
int SM_EXPL_X;
int SM_EXPL_Y;
int SM_MED_X;
int SM_MED_Y;

int MONEY_X;
int MONEY_Y;
int MONEY_WIDTH;
int MONEY_HEIGHT;

/*
 *
 *								TEAM BAR COORDS
 *
 */

/*
 *	Now we have only 6 mercs in team but maby we will make it CUSTOMIZABLE hehe i like that word
 *	why we declare array with 12 items? coz we got 2 valuse x and y maby we can work it out
 *	and change INT16 to INV_REGION_DESC for ex.
 */
const int TeamInterfaceMax = NUMBER_OF_SOLDIERS_PER_SQUAD * 2;	// WDS
INT16 sTEAMAPPanelXY[TeamInterfaceMax];
INT16 sTEAMFacesXY[TeamInterfaceMax];
INT16 sTEAMNamesXY[TeamInterfaceMax];
INT16 sTEAMFaceHighlXY[TeamInterfaceMax];
INT16 sTEAMLifeXY[TeamInterfaceMax];
INT16 sTEAMBreathXY[TeamInterfaceMax];
INT16 sTEAMMoraleXY[TeamInterfaceMax];
INT16 sTEAMApXY[TeamInterfaceMax];
INT16 sTEAMBarsXY[TeamInterfaceMax];
INT16 sTEAMHandInvXY[TeamInterfaceMax];

int TM_FACE_WIDTH;
int TM_FACE_HEIGHT;
int TM_APPANEL_HEIGHT;
int TM_APPANEL_WIDTH;

int TM_ENDTURN_X;
int TM_ENDTURN_Y;
int TM_ROSTERMODE_X;
int TM_ROSTERMODE_Y;
int TM_DISK_X;
int TM_DISK_Y;

int TM_NAME_WIDTH;
int TM_NAME_HEIGHT;
int TM_LIFEBAR_WIDTH;
int TM_LIFEBAR_HEIGHT;
int TM_FACEHIGHTL_WIDTH;
int TM_FACEHIGHTL_HEIGHT;
int TM_AP_HEIGHT;
int TM_AP_WIDTH;

int TM_INV_WIDTH;
int TM_INV_HEIGHT;
int TM_INV_HAND1STARTX;
int TM_INV_HAND1STARTY;
int TM_INV_HAND2STARTX;
int TM_INV_HAND2STARTY;
int TM_INV_HAND_SEP;
int TM_INV_HAND_SEPY;

int TM_BARS_REGION_HEIGHT;
int TM_BARS_REGION_WIDTH;

int INDICATOR_BOX_WIDTH;
int INDICATOR_BOX_HEIGHT;

MOUSE_REGION	gSMPanelRegion;
INT8		gbNewItem[ NUM_INV_SLOTS ];
TEAM_PANEL_SLOTS_TYPE	gTeamPanel[ NUM_TEAM_SLOTS ];
/*
 *
 *							VARIOUS PANELS COORDS
 *
 *	now its only clock and location name we will also make them ???
 *	yes yes yes MORE CUSTOMIZABLE :P for moders
 *	i will work with radar screen to allow moving it. or maby someone else will do it
 *	any questions? joker
 */
int	LOCATION_NAME_X;
int	LOCATION_NAME_Y;

/* CHRISL: Added new "TM" variables to allow team and inventory screens to place the location name
independantly of each other */
int	LOCATION_NAME_TM_X;
int	LOCATION_NAME_TM_Y;

// CHRISL: Keyring coords moved from Interface Items.cpp
int KEYRING_X;
int KEYRING_Y;
extern UINT32 guiCurrentItemDescriptionScreen;

extern UINT32 guiCurrentItemDescriptionScreen;

enum
{
	STANCEUP_IMAGES = 0,
	UPDOWN_IMAGES,
	CLIMB_IMAGES,
	STANCEDOWN_IMAGES,
	HANDCURSOR_IMAGES,
	PREVMERC_IMAGES,
	NEXTMERC_IMAGES,
	OPTIONS_IMAGES,
//	BURSTMODE_IMAGES,
	LOOK_IMAGES,
	TALK_IMAGES,
	MUTE_IMAGES,
	STANCE_IMAGES,
	DONE_IMAGES,
	MAPSCREEN_IMAGES,
	BACKPACK_IMAGES,
	//HOP_IMAGES, //legion by Jazz
	NUM_SM_BUTTON_IMAGES
};

enum
{
	ENDTURN_IMAGES = 0,
	ROSTERMODE_IMAGES,
	DISK_IMAGES,
	NUM_TEAM_BUTTON_IMAGES
};

INT32		iSMPanelImages		[ NUM_SM_BUTTON_IMAGES ];
INT32		iBurstButtonImages	[ NUM_WEAPON_MODES ];
INT32		iTEAMPanelImages	[ NUM_TEAM_BUTTON_IMAGES ];

INT32		giSMStealthImages	= -1;
INT32		giSMStealthButton	= -1;

// CHRISL: Backpack buttons
INT32		giSMDropPackImages			= -1;
INT32		giSMDropPackButton			= -1;
INT32		giSMZipperImages			= -1;
INT32		giSMZipperButton			= -1;

BOOLEAN		gfSwitchPanel		= FALSE;
UINT8		gbNewPanel			= SM_PANEL;
UINT8		gubNewPanelParam	= 0;

BOOLEAN		gfUIStanceDifferent						= FALSE;
BOOLEAN		gfAllDisabled							= FALSE;
BOOLEAN		gfSMDisableForItems						= FALSE;
BOOLEAN		gfDisableTacticalPanelButtons			= FALSE;
BOOLEAN		gfAddingMoneyToMercFromPlayersAccount	= FALSE;

BOOLEAN		gfCheckForMouseOverItem				= FALSE;
UINT32		guiMouseOverItemTime				= 0;
INT8		gbCheckForMouseOverItemPos			= 0;
UINT8		gubSelectSMPanelToMerc				= NOBODY;
BOOLEAN		gfReEvaluateDisabledINVPanelButtons = FALSE;

UINT32		guiBrownBackgroundForTeamPanel;
UINT32		guiGoldBackgroundForTeamPanel;

extern	BOOLEAN		gfRerenderInterfaceFromHelpText;
extern	BOOLEAN		gfInKeyRingPopup;
extern	UINT32		guiVEHINV;
extern	INT32		giMapInvDoneButton;
extern	BOOLEAN		gfBeginEndTurn;
extern	SOLDIERTYPE	*gpItemDescSoldier;
extern	BOOLEAN		gfInItemPickupMenu;

extern	void HandleAnyMercInSquadHasCompatibleStuff( UINT8 ubSquad, OBJECTTYPE *pObject, BOOLEAN fReset );
extern	void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem );
extern	BOOLEAN InternalInitItemDescriptionBox( OBJECTTYPE *pObject, INT16 sX, INT16 sY, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier, UINT8 ubPosition );
extern	BOOLEAN InternalHandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, OBJECTTYPE *pTestObject, BOOLEAN fOn	);
extern	BOOLEAN CompatibleAmmoForGun( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject );//dnl ch66 070913

BOOLEAN IsMouseInRegion( MOUSE_REGION *pRegion );
void HandleMouseOverSoldierFaceForContMove( SOLDIERTYPE *pSoldier, BOOLEAN fOn );
void HandlePlayerTeamMemberDeathAfterSkullAnimation( SOLDIERTYPE *pSoldier );
void EnableButtonsForInItemBox( BOOLEAN fDisable );
void ConfirmationToDepositMoneyToPlayersAccount( UINT8 ubExitValue );
void MergeMessageBoxCallBack( UINT8 ubExitValue );

UINT8	gubHandPos;
UINT16	gusOldItemIndex;
UINT16	gusNewItemIndex;
BOOLEAN gfDeductPoints;

// HEADROCK HAM 4: This variable tells the program what "page" of the description box we are currently looking at.
// By saving this in memory, we tell the DescBox which background and values to draw, and make sure that we end up
// on the same page every time we open the description box. UDB buttons allow switching between pages.
UINT8 gubDescBoxPage;
// silversurfer: Now we also have a secondary page on the general tab for weapons so we can look at their secondary attributes.
UINT8 gubDescGenPage;
// Record which line we're looking at. This only applied to the Advanced tab in description boxes.
UINT8 gubDescBoxLine;
// Record how many lines this item has to display. This only applies to the Advanced tab.
UINT8 gubDescBoxTotalAdvLines;

INT32	iSMPanelButtons[ NUM_SM_BUTTONS ];
INT32	iTEAMPanelButtons[ NUM_TEAM_BUTTONS ];

// Video Surface for Single Merc Panel
UINT32	guiSMPanel;
UINT32	guiSMObjects;
UINT32	guiSMObjects2;
UINT32	guiSecItemHiddenVO;
UINT32	guiTEAMPanel;
UINT32	guiTEAMObjects;

// Globals for various mouse regions
MOUSE_REGION	gSM_SELMERCPanelRegion;
MOUSE_REGION	gSM_SELMERCBarsRegion;
MOUSE_REGION	gSM_SELMERCMoneyRegion;
MOUSE_REGION	gSM_SELMERCEnemyIndicatorRegion;
MOUSE_REGION	gTEAM_PanelRegion;
MOUSE_REGION	gTEAM_FaceRegions[ NUMBER_OF_SOLDIERS_PER_SQUAD ];
MOUSE_REGION	gTEAM_BarsRegions[ NUMBER_OF_SOLDIERS_PER_SQUAD ];
MOUSE_REGION	gTEAM_LeftBarsRegions[ NUMBER_OF_SOLDIERS_PER_SQUAD ];
MOUSE_REGION	gTEAM_FirstHandInv[ NUMBER_OF_SOLDIERS_PER_SQUAD ];
MOUSE_REGION	gTEAM_SecondHandInv[ NUMBER_OF_SOLDIERS_PER_SQUAD ];
MOUSE_REGION	gTEAM_EnemyIndicator[ NUMBER_OF_SOLDIERS_PER_SQUAD ];

MOUSE_REGION	gSM_SELMERCWeightRegion; // Added by Flugente
MOUSE_REGION	gSM_SELMERCCamoRegion; // Added by Sandro

BOOLEAN			gfTEAM_HandInvDispText[ NUMBER_OF_SOLDIERS_PER_SQUAD ][ NUM_INV_SLOTS ];
BOOLEAN			gfSM_HandInvDispText[ NUM_INV_SLOTS ];

void HelpTextDoneCallback( void );

// Globals - for one - the current merc here
INT8				gbSMCurStanceObj;
UINT16				gusSMCurrentMerc = 0;
SOLDIERTYPE			*gpSMCurrentMerc = NULL;
// CHRISL:
extern	INT8		gbCompatibleApplyItem;
extern	SOLDIERTYPE *gpItemPopupSoldier;

INT8		gbStanceButPos[2][3][3] =
{
	// NON-STEALTH
	16,		14,		15,
	10,		8,		9,
	22,		20,		21,
	// STEALTH MODE
	13,		11,		12,
	7,		5,		6,
	19,		17,		18
};

//CHRISL: New arrays to deal with backpack button positions
INT8		gbDropPackButPos[2][2] =
{
	2,	3,
	3,	2
};
INT8		gbZipperButPos[2][2] =
{
	0,	1,
	1,	0
};

//Legion by Jazz
//void BtnOknoCallback(GUI_BUTTON *btn,INT32 reason); 
//void BtnFenceCallback(GUI_BUTTON *btn,INT32 reason); 

// Mouse button and region callbacks
//void BtnPositionCallback( GUI_BUTTON *btn, INT32 reason );
//void BtnMovementCallback( GUI_BUTTON *btn, INT32 reason );
void SelectedMercButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SelectedMercButtonMoveCallback( MOUSE_REGION *pRegion, INT32 iReason );
void SelectedMercEnemyIndicatorCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvClickCamoCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoveCammoCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoneyButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );

// SINGLE MERC PANEL BUTTON CALLBACKS
void BtnStealthModeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnStanceUpCallback(GUI_BUTTON *btn,INT32 reason);
void BtnUpdownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnClimbCallback(GUI_BUTTON *btn,INT32 reason);
void BtnStanceDownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnHandCursorCallback(GUI_BUTTON *btn,INT32 reason);
void BtnTalkCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMuteCallback(GUI_BUTTON *btn,INT32 reason);
void BtnSMDoneCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMapScreenCallback(GUI_BUTTON *btn,INT32 reason);

// CHRISL: New callback functions for backpack buttons
void BtnDropPackCallback(GUI_BUTTON *btn,INT32 reason);
void BtnZipperCallback(GUI_BUTTON *btn,INT32 reason);
BOOLEAN ChangeZipperStatus(SOLDIERTYPE *pSoldier, BOOLEAN newStatus);

void BtnPrevMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnNextMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnOptionsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnBurstModeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnLookCallback(GUI_BUTTON *btn,INT32 reason);
void BtnPositionShowCallback(GUI_BUTTON *btn,INT32 reason);
void InvPanelButtonClickCallback( MOUSE_REGION * pRegion, INT32 iReason );

// TEAM PANEL BUTTON CALLBACKS
void BtnEndTurnCallback(GUI_BUTTON *btn,INT32 reason);
void BtnRostermodeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnSquadCallback(GUI_BUTTON *btn,INT32 reason);
void MercFacePanelCallback( MOUSE_REGION * pRegion, INT32 iReason );
void MercFacePanelMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMClickFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMClickSecondHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void EnemyIndicatorClickCallback( MOUSE_REGION * pRegion, INT32 iReason );

void RenderSoldierTeamInv( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY, UINT8 ubPanelNum, BOOLEAN fDirty );

// keyring stuff
void KeyRingItemPanelButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );

void UpdateSelectedSoldier( UINT16 usSoldierID, BOOLEAN fSelect );

void CheckForFacePanelStartAnims( SOLDIERTYPE *pSoldier, INT16 sPanelX, INT16 sPanelY );
void HandleSoldierFaceFlash( SOLDIERTYPE *pSoldier, INT16 sFaceX, INT16 sFaceY );
BOOLEAN PlayerExistsInSlot( UINT8 ubID );
void UpdateStatColor( UINT32 uiTimer, BOOLEAN fIncrease, BOOLEAN fDamaged, BOOLEAN fAugmented ); // SANDRO - added argument // Flugente - me too

extern void UpdateItemHatches();

extern SOLDIERTYPE *FindNextActiveSquad( SOLDIERTYPE *pSoldier );

extern BOOLEAN gfMouseLockedOnBorder;

// Wraps up check for AP-s get from a different soldier for in a vehicle...
INT16 GetUIApsToDisplay( SOLDIERTYPE *pSoldier )
{

	return ( pSoldier->bActionPoints );

}

void CheckForDisabledForGiveItem( )
{
	INT16			sDist;
	INT32 sDestGridNo;
	INT8			bDestLevel;
	INT32			cnt;
	SOLDIERTYPE		*pSoldier;
	UINT8			ubSrcSoldier;

	// CHRISL: If Merc hasn't been set, default to first merc
	if(gpSMCurrentMerc==NULL)
		gpSMCurrentMerc = MercPtrs[ 0 ];
	Assert( gpSMCurrentMerc != NULL);

	if ( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		gfSMDisableForItems = !CanMercInteractWithSelectedShopkeeper( gpSMCurrentMerc );
		return;
	}

	// Default to true
	gfSMDisableForItems = TRUE;

	// ATE: Is the current merc unconscious.....
	if ( gpSMCurrentMerc->stats.bLife < OKLIFE && gpItemPointer != NULL )
	{
		// Go through each merc and see if there is one closeby....
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
		{
			if ( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) && pSoldier->bInSector && IsMercOnCurrentSquad( pSoldier ) )
			{
				sDist = PythSpacesAway( gpSMCurrentMerc->sGridNo, pSoldier->sGridNo );

				// Check LOS....
				if ( SoldierTo3DLocationLineOfSightTest( pSoldier, gpSMCurrentMerc->sGridNo,  gpSMCurrentMerc->pathing.bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
				{
					if ( sDist <= PASSING_ITEM_DISTANCE_NOTOKLIFE )
					{
						gfSMDisableForItems = FALSE;
						break;	// found one, no need to keep looking
					}
				}
			}
		}
	}
	else
	{
		ubSrcSoldier = (UINT8)gusSelectedSoldier;

		if ( gpItemPointer != NULL )
		{
			ubSrcSoldier = gpItemPointerSoldier->ubID;
		}

		// OK buddy, check our currently selected merc and disable/enable if not close enough...
		if ( ubSrcSoldier != NOBODY )
		{
			if ( gusSMCurrentMerc != ubSrcSoldier )
			{
				sDestGridNo = MercPtrs[ gusSMCurrentMerc ]->sGridNo;
				bDestLevel	= MercPtrs[ gusSMCurrentMerc ]->pathing.bLevel;

				// Get distance....
				sDist = PythSpacesAway( MercPtrs[ ubSrcSoldier ]->sGridNo, sDestGridNo );

				// Check LOS....
				if ( SoldierTo3DLocationLineOfSightTest( MercPtrs[ ubSrcSoldier ], sDestGridNo,  bDestLevel, 3, TRUE, CALC_FROM_ALL_DIRS )  )
				{
					// UNCONSCIOUS GUYS ONLY 1 tile AWAY
					if ( MercPtrs[ gusSMCurrentMerc ]->stats.bLife < CONSCIOUSNESS )
					{
						if ( sDist <= PASSING_ITEM_DISTANCE_NOTOKLIFE )
						{
							gfSMDisableForItems = FALSE;
						}
					}
					else if ( sDist <= PASSING_ITEM_DISTANCE_OKLIFE )
					{
						gfSMDisableForItems = FALSE;
					}
				}
				// anv: passengers in the same vehicle can pass items freely
				else if ( MercPtrs[ ubSrcSoldier ]->iVehicleId != -1 && MercPtrs[ ubSrcSoldier ]->iVehicleId == MercPtrs[ gusSMCurrentMerc ]->iVehicleId )
				{
					gfSMDisableForItems = FALSE;
				}
			}
			else
			{
				gfSMDisableForItems = FALSE;
			}
		}
		else
		{
			gfSMDisableForItems = FALSE;
		}
	}
}

void SetSMPanelCurrentMerc( UINT8 ubNewID )
{
	gubSelectSMPanelToMerc = NOBODY;

	gusSMCurrentMerc = ubNewID;

	gpSMCurrentMerc = MercPtrs[ ubNewID ];

	// Set to current guy's interface level
	//if ( gsInterfaceLevel != gpSMCurrentMerc->bUIInterfaceLevel )
	//{
	//	SetRenderFlags(RENDER_FLAG_FULL);
	//	ErasePath(FALSE);
	//	gsInterfaceLevel = gpSMCurrentMerc->bUIInterfaceLevel;
	//}

	// Disable all faces
	SetAllAutoFacesInactive( );

	// Turn off compat ammo....
	HandleCompatibleAmmoUI(gpSMCurrentMerc, NO_SLOT, FALSE);
	if (gpItemPointer != NULL)
	{
		gfCheckForMouseOverItem = TRUE;
	}

	// Remove item desc panel if one up....
	if ( gfInItemDescBox )
	{
		DeleteItemDescriptionBox( );
	}

	// Remove stack popup panel if one up....
	if ( InItemStackPopup( ) || InSectorStackPopup( ) )
	{
		DeleteItemStackPopup( );
	}

	// Remove keyring popup panel if one up....
	if ( InKeyRingPopup( ) )
	{
		DeleteKeyRingPopup( );
	}

	if ( gfInItemPickupMenu )
	{
		gfSMDisableForItems = TRUE;
	}
	else
	{
		if ( ( gpItemPointer != NULL || guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE || gpSMCurrentMerc->stats.bLife < OKLIFE ) )
		{
			CheckForDisabledForGiveItem( );
		}
		else
		{
			gfSMDisableForItems = FALSE;
		}
	}

	if ( gpItemPointer != NULL )
	{
		ReevaluateItemHatches( gpSMCurrentMerc, FALSE );
	}


	DisableInvRegions( gfSMDisableForItems );

	fInterfacePanelDirty = DIRTYLEVEL2;

	gfUIStanceDifferent = TRUE;

	UpdateSMPanel( );

}


void UpdateForContOverPortrait( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	INT32 cnt;

	if ( gsCurInterfacePanel == SM_PANEL )
	{
		if ( gpSMCurrentMerc != NULL )
		{
			// Check if mouse is in region and if so, adjust...
			if ( IsMouseInRegion( &gSM_SELMERCPanelRegion ) )
			{
				HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, fOn );
			}
		}
	}
	else
	{
		for ( cnt = 0; cnt < NUMBER_OF_SOLDIERS_PER_SQUAD; cnt++ )
		{
			if ( gTeamPanel[ cnt ].ubID == pSoldier->ubID )
			{
				if ( IsMouseInRegion( &gTEAM_FaceRegions[ cnt ] ) )
				{
					HandleMouseOverSoldierFaceForContMove( pSoldier, fOn );
				}
			}
		}
	}
}


void UpdateSMPanel( )
{
	BOOLEAN						fNearHeigherLevel;
	BOOLEAN						fNearLowerLevel;
	INT8							bDirection;
	UINT8							ubStanceState;
	
	//jump window by Jazz
	//BOOLEAN						fNearLowerLevelOkno;
	//BOOLEAN						fNearHeigherLevelOkno;

	if (TileIsOutOfBounds(gpSMCurrentMerc->sGridNo))
	{
		return;
	}

	// Stance
	ubStanceState = gpSMCurrentMerc->ubDesiredHeight;

	if ( ubStanceState == NO_DESIRED_HEIGHT )
	{
		ubStanceState = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;
	}


	switch( ubStanceState )
	{
		case ANIM_STAND:

			gbSMCurStanceObj = 0;
			DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );

			// Disable if we cannot do this!
			if ( !IsValidStance( gpSMCurrentMerc, ANIM_CROUCH ) )
			{
				DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			}
			break;

		case ANIM_PRONE:
			gbSMCurStanceObj = 2;
			DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			break;

		case ANIM_CROUCH:
			gbSMCurStanceObj = 1;
			EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );

			// Disable if we cannot do this!
			if ( !IsValidStance( gpSMCurrentMerc, ANIM_PRONE ) )
			{
				DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			}
			break;
	}

	// Stance button done wether we're disabled or not...
	if ( gfUIStanceDifferent )
	{
		//Remove old
		if ( giSMStealthButton != -1 )
		{
				RemoveButton( giSMStealthButton );
		}
		if ( giSMStealthImages != -1 )
		{
			UnloadButtonImage( giSMStealthImages );
		}

		// Make new
		giSMStealthImages = UseLoadedButtonImage( iSMPanelImages[ STANCE_IMAGES	] ,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][0] ,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][1],-1,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][2],-1 );

		giSMStealthButton = QuickCreateButton( giSMStealthImages, SM_STEALTHMODE_X, SM_STEALTHMODE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStealthModeCallback );
		RenderBackpackButtons(ACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */

		SetButtonFastHelpText( giSMStealthButton, TacticalStr[ TOGGLE_STEALTH_MODE_POPUPTEXT ] );

		gfUIStanceDifferent = FALSE;

		if ( gfAllDisabled )
		{
			if ( giSMStealthButton != -1 )
			{
					DisableButton( giSMStealthButton );
			}
			RenderBackpackButtons(DISABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
		}
	}

	if ( gfAllDisabled )
	{
		return;
	}


	CheckForReEvaluateDisabledINVPanelButtons( );

	// Check for any newly added items we need.....
	if ( gpSMCurrentMerc->flags.fCheckForNewlyAddedItems )
	{
		// Startup any newly added items....
		CheckForAnyNewlyAddedItems( gpSMCurrentMerc );

		gpSMCurrentMerc->flags.fCheckForNewlyAddedItems = FALSE;
	}



	// Set Disable /Enable UI based on buddy's stats
	if (ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ImageNum != (UINT32) iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ] )
	{
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ImageNum = iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ];
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_DIRTY;
	}

	/*
	if ( gpSMCurrentMerc->bDoBurst )
	{
		if ( !ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}
	*/


	// Toggle MUTE button...
	if ( gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_MUTE )
	{
		if ( !ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
	
	GetMercClimbDirection( gpSMCurrentMerc->ubID, &fNearLowerLevel, &fNearHeigherLevel );

	if (fNearLowerLevel || fNearHeigherLevel)
	{
		if (IsValidStance(gpSMCurrentMerc, ANIM_CROUCH) && EnoughPoints(gpSMCurrentMerc, GetAPsToClimbRoof(gpSMCurrentMerc, fNearLowerLevel), GetBPsToClimbRoof(gpSMCurrentMerc, fNearLowerLevel), FALSE))
		{
			EnableButton(iSMPanelButtons[CLIMB_BUTTON]);
		}
	}

	if (gGameExternalOptions.fCanClimbOnWalls == TRUE)
	{
		if (FindWallJumpDirection(gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->ubDirection, &bDirection))
		{
			if (IsValidStance(gpSMCurrentMerc, ANIM_CROUCH) && EnoughPoints(gpSMCurrentMerc, GetAPsToJumpWall(gpSMCurrentMerc, FALSE), GetBPsToJumpWall(gpSMCurrentMerc, FALSE), FALSE))
			{
				EnableButton(iSMPanelButtons[CLIMB_BUTTON]);
			}
		}
	}

	if (FindFenceJumpDirection(gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->ubDirection, &bDirection))
	{
		if (IsValidStance(gpSMCurrentMerc, ANIM_CROUCH) && EnoughPoints(gpSMCurrentMerc, GetAPsToJumpFence(gpSMCurrentMerc, FALSE), GetBPsToJumpFence(gpSMCurrentMerc, FALSE), FALSE))
		{
			EnableButton(iSMPanelButtons[CLIMB_BUTTON]);
		}
	}

	if ( (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
	}
	else if ( !gfAllDisabled )
	{
		EnableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
	}

//	if ( gpSMCurrentMerc->bUIInterfaceLevel > 0 )
	if ( gsInterfaceLevel > 0 )
	{
		if ( !ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == HANDCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == TALKCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == LOOKCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	// If not selected ( or dead ), disable/gray some buttons
	if ( gusSelectedSoldier != gpSMCurrentMerc->ubID || ( gpSMCurrentMerc->stats.bLife < OKLIFE ) || (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || gfSMDisableForItems )
	{
		DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
		DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
		DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
		DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
		DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
		DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
		DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
		if ( giSMStealthButton != -1 )
		{
				DisableButton( giSMStealthButton );
		}
		RenderBackpackButtons(DISABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	}
	else
	{
		// Enable some buttons!
		// Changed by ADB, rev 1513
		//if ( ((IsGunAutofireCapable( gpSMCurrentMerc, HANDPOS ) || IsGunBurstCapable( gpSMCurrentMerc, HANDPOS , FALSE )) && !Weapon[gpSMCurrentMerc->inv[HANDPOS].usItem].NoSemiAuto ) || IsGrenadeLauncherAttached ( &(gpSMCurrentMerc->inv[HANDPOS]) ) )
		if ( ( ( IsGunAutofireCapable( &gpSMCurrentMerc->inv[HANDPOS] ) || IsGunBurstCapable( &gpSMCurrentMerc->inv[HANDPOS], FALSE, gpSMCurrentMerc ) ) && !Weapon[gpSMCurrentMerc->inv[HANDPOS].usItem].NoSemiAuto ) 
			|| IsGrenadeLauncherAttached ( &(gpSMCurrentMerc->inv[HANDPOS]) ) 
			|| IsWeaponAttached( &(gpSMCurrentMerc->inv[HANDPOS]), IC_GUN )
			|| IsWeaponAttached( &( gpSMCurrentMerc->inv[HANDPOS] ), IC_BLADE )
			|| HasSeveralBarrelConfigurations( gpSMCurrentMerc->inv[HANDPOS].usItem ) )
		{
			EnableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
		}
		else
		{
			DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
		}
		EnableButton( iSMPanelButtons[ LOOK_BUTTON ] );
		EnableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
		EnableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );

		if ( giSMStealthButton != -1 )
		{
				EnableButton( giSMStealthButton );
		}
		if(!gfInItemDescBox)
			RenderBackpackButtons(ENABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	}

	// CJC Dec 4 2002: or if item pickup menu is up
	if ( (gTacticalStatus.uiFlags & ENGAGED_IN_CONV) || gfInItemPickupMenu)
	{
		DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}

}

extern INT8		gbInvalidPlacementSlot[ NUM_INV_SLOTS ];


void ReevaluateItemHatches( SOLDIERTYPE *pSoldier, BOOLEAN fAllValid )
{
	UINT32	cnt;

	// if there's an item in the cursor and we're not supposed to just make them all valid
	if ( ( gpItemPointer != NULL ) && !fAllValid )
	{
		// check all inventory positions and mark the ones where cursor item won't fit as invalid
		UINT32 invsize = pSoldier->inv.size();
		for ( cnt = 0; cnt < invsize; ++cnt )
		{
			gbInvalidPlacementSlot[ cnt ] = !CanItemFitInPosition( pSoldier, gpItemPointer, (INT8)cnt, FALSE );

			// !!! ATTACHING/MERGING ITEMS IN MAP SCREEN IS NOT SUPPORTED !!!
			// CJC: seems to be supported now...
			//if( guiCurrentScreen != MAP_SCREEN )
			{
				// Check attachments, override to valid placement if valid merge...
				if ( ValidAttachment( gpItemPointer->usItem, &(pSoldier->inv[ cnt ]) ) )
				{
					gbInvalidPlacementSlot[ cnt ] = FALSE;
				}

				if ( ValidMerge( gpItemPointer->usItem, pSoldier->inv[ cnt ].usItem ) )
				{
					gbInvalidPlacementSlot[ cnt ] = FALSE;
				}
			}
		}
	}
	else
	{
		// mark all inventory positions as valid
		for ( cnt = 0; cnt < NUM_INV_SLOTS; ++cnt )
		{
			gbInvalidPlacementSlot[ cnt ] = FALSE;
		}
	}

	fInterfacePanelDirty = DIRTYLEVEL2;
}

//CHRISL: Function to handle backpack button rendering
void RenderBackpackButtons(int bpAction)
{
	// Only run function if we're using new inventory system
	if((UsingNewInventorySystem() == false))
		return;
	// Only run this if we're not on the strategic screen
	if(guiCurrentItemDescriptionScreen == MAP_SCREEN)
		return;
	// Only run if the Tactical Inventory Panel is open
	if(gsCurInterfacePanel != SM_PANEL)
		return;
	// Don't run if keyring is open
	if(gfInKeyRingPopup)
		return;
	// If Merc hasn't been set, default to first merc
	if(gpSMCurrentMerc==NULL)
		gpSMCurrentMerc = MercPtrs[ 0 ];
	Assert(gpSMCurrentMerc!=NULL);
	// Deal with buttons depending on which screen is currently active
	switch (bpAction)
	{
		case ACTIVATE_BUTTON:
			// Activate buttons
			if(giSMZipperButton != -1)
				RemoveButton( giSMZipperButton );
			if(giSMZipperImages != -1)
				UnloadButtonImage( giSMZipperImages );
			if(giSMDropPackButton != -1)
				RemoveButton( giSMDropPackButton );
			if(giSMDropPackImages != -1)
				UnloadButtonImage( giSMDropPackImages );
			gpSMCurrentMerc->flags.DropPackFlag = FALSE;
			if(gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == FALSE)
			{
				for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
				{
					if(gWorldItems[wi].soldierID == gpSMCurrentMerc->ubID && gWorldItems[wi].object.exists() == true)
					{
						gpSMCurrentMerc->flags.DropPackFlag = TRUE;
						break;
					}
				}
			}
			giSMZipperImages	= UseLoadedButtonImage( iSMPanelImages[ BACKPACK_IMAGES  ] ,-1 ,gbZipperButPos[ gpSMCurrentMerc->flags.ZipperFlag ][0],-1,gbZipperButPos[ gpSMCurrentMerc->flags.ZipperFlag ][1],-1 );
			giSMDropPackImages	= UseLoadedButtonImage( iSMPanelImages[ BACKPACK_IMAGES  ] ,-1 ,gbDropPackButPos[ gpSMCurrentMerc->flags.DropPackFlag ][0],-1,gbDropPackButPos[ gpSMCurrentMerc->flags.DropPackFlag ][1],-1 );
			//giSMDropPackImages	= UseLoadedButtonImage( iSMPanelImages[ BACKPACK_IMAGES  ] ,gbDropPackButPos[ gpSMCurrentMerc->flags.DropPackFlag ][0] ,gbDropPackButPos[ gpSMCurrentMerc->flags.DropPackFlag ][0],-1,gbDropPackButPos[ gpSMCurrentMerc->flags.DropPackFlag ][1],-1 );

			giSMZipperButton	= QuickCreateButton( giSMZipperImages, SM_ZIPPER_X, SM_ZIPPER_Y,
													BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnZipperCallback );
			giSMDropPackButton	= QuickCreateButton( giSMDropPackImages, SM_DROPPACK_X, SM_DROPPACK_Y,
													BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnDropPackCallback );
			//SetButtonFastHelpText( giSMZipperButton, TacticalStr[  ] );
			//SetButtonFastHelpText( giSMDropPackButton, TacticalStr[  ] );
			break;
		case DEACTIVATE_BUTTON:
			// Deactivate buttons
			if(giSMZipperButton != -1)
				RemoveButton( giSMZipperButton );
			if(giSMDropPackButton != -1)
				RemoveButton( giSMDropPackButton );
			RenderBackpackButtons(UNLOAD_BUTTON);
			giSMDropPackImages	= -1;
			giSMDropPackButton	= -1;
			giSMZipperImages	= -1;
			giSMZipperButton	= -1;
			break;
		case ENABLE_BUTTON:
			// Enable buttons
			if(giSMDropPackButton == -1 || giSMZipperButton == -1)
			{
				RenderBackpackButtons(DEACTIVATE_BUTTON);
				RenderBackpackButtons(ACTIVATE_BUTTON);
			}
			if(giSMDropPackButton != -1)
				EnableButton( giSMDropPackButton );
			if(giSMZipperButton != -1)
				EnableButton( giSMZipperButton );
			break;
		case DISABLE_BUTTON:
			// Disable buttons
			if(giSMDropPackButton != -1)
				DisableButton( giSMDropPackButton );
			if(giSMZipperButton != -1)
				DisableButton( giSMZipperButton );
			break;
		case UNLOAD_BUTTON:
			if(giSMZipperImages != -1)
				UnloadButtonImage( giSMZipperImages );
			if(giSMDropPackImages != -1)
				UnloadButtonImage( giSMDropPackImages );
			break;
	}
}


void EnableSMPanelButtons( BOOLEAN fEnable , BOOLEAN fFromItemPickup )
{
	if ( fFromItemPickup )
	{
		// If we have the item pointer up...
		// CJC Dec 4 2002: or if item pickup menu is up
		//if ( gpItemPointer != NULL )
		if ( gpItemPointer != NULL || gfInItemPickupMenu )
		{
			DisableTacticalTeamPanelButtons( TRUE );
		}
		else
		{
			DisableTacticalTeamPanelButtons( FALSE );
		}

		fInterfacePanelDirty = DIRTYLEVEL2;
	}


	if ( gsCurInterfacePanel == SM_PANEL )
	{
		if ( fFromItemPickup )
		{
			// If we have the item pointer up...
			if ( gpItemPointer != NULL )
			{
				ReevaluateItemHatches( gpSMCurrentMerc, fEnable );

				// Turn it all false first....
				InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, FALSE );
				InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, TRUE );

				gfCheckForMouseOverItem = FALSE;

				// Highlight guys ....
				HandleAnyMercInSquadHasCompatibleStuff( (INT8)CurrentSquad( ), gpItemPointer, FALSE );

			}
			else
			{
				//InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, FALSE );
				gfCheckForMouseOverItem = FALSE;

				HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
			}

			if ( fEnable )
			{
				ReevaluateItemHatches( gpSMCurrentMerc, fEnable );
			}

			fInterfacePanelDirty = DIRTYLEVEL2;
		}

		if ( fEnable )
		{
			// only enable the following if NOT in shopkeeper's interface
			if ( !(guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
			{
				EnableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
				EnableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
				EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
				EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
				EnableButton( iSMPanelButtons[ LOOK_BUTTON ] );
				EnableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
				EnableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
				if ( giSMStealthButton != -1 )
				{
					EnableButton( giSMStealthButton );
				}
				RenderBackpackButtons(ENABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */

				if ( gfDisableTacticalPanelButtons )
				{
					DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
					DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
					DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
				}
				else
				{
					EnableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
					EnableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
					EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
				}


				//enable the radar map region
				MSYS_EnableRegion(&gRadarRegion);

				gfSMDisableForItems = FALSE;

			DisableInvRegions( gfSMDisableForItems );
			}

			if ( !fFromItemPickup )
			{
				EnableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
				EnableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
			}
		}
		else
		{
			DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
			DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
			DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
			DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
			DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
			if ( giSMStealthButton != -1 )
			{
					DisableButton( giSMStealthButton );
			}
			RenderBackpackButtons(DISABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */

			if ( !fFromItemPickup )
			{
				DisableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
				DisableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
			}

			DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
			DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
			DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );

			//disable the radar map
			MSYS_DisableRegion(&gRadarRegion);
		}

		gfAllDisabled = !fEnable;

	}
}

UINT16 GetSMPanelCurrentMerc( )
{
	return( gusSMCurrentMerc );
}


// CHRISL: New functions to define initial settings for New Inventory system
void InitNewInventorySystem( )
{
	if (iResolution >= _640x480 && iResolution < _800x600)
		INTERFACE_WIDTH			= 640;
	else if (iResolution < _1024x768)
		INTERFACE_WIDTH			= 800;
	else
		INTERFACE_WIDTH			= 1024;

	INTERFACE_START_X		= (SCREEN_WIDTH - INTERFACE_WIDTH)/2;
	INTERFACE_HEIGHT		= 120;
	INV_INTERFACE_HEIGHT	= 200;
	INTERFACE_START_Y		= ( SCREEN_HEIGHT - INTERFACE_HEIGHT );
	INV_INTERFACE_START_Y	= ( SCREEN_HEIGHT - INV_INTERFACE_HEIGHT );
}

// CHRISL: New functions to define initial settings for Old Inventory system
void InitOldInventorySystem( )
{
	if (iResolution >= _640x480 && iResolution < _800x600)
		INTERFACE_WIDTH			= 640;
	else if (iResolution < _1024x768)
		INTERFACE_WIDTH			= 800;
	else
		INTERFACE_WIDTH			= 1024;

	INTERFACE_START_X		= (SCREEN_WIDTH - INTERFACE_WIDTH)/2;
	INTERFACE_HEIGHT		= 120;
	INV_INTERFACE_HEIGHT	= 140;
	INTERFACE_START_Y		= ( SCREEN_HEIGHT - INTERFACE_HEIGHT );
	INV_INTERFACE_START_Y	= ( SCREEN_HEIGHT - INV_INTERFACE_HEIGHT );
}

// CHRISL: These functions will setup inventory panel coords based on game options
BOOLEAN InitializeSMPanelCoordsOld()
{
	// int i; // unused (jonathanl)
	/*
	* now i use standard positions and i add to them starting coords of panel but
	* we can add here function that will read coord in panel from file
	* that will be phase2 of making GUI more customizable
	* any questions? joker
	*/
	// Inventory slots
	gSMInvPocketXY[HELMETPOS].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[HELMETPOS].sY = INV_INTERFACE_START_Y + 6;	// HELMETPOS
	gSMInvPocketXY[VESTPOS].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[VESTPOS].sY = INV_INTERFACE_START_Y + 35;	// VESTPOS
	gSMInvPocketXY[LEGPOS].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[LEGPOS].sY = INV_INTERFACE_START_Y + 95;	// LEGPOS,
	gSMInvPocketXY[HEAD1POS].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[HEAD1POS].sY = INV_INTERFACE_START_Y + 6;	// HEAD1POS
	gSMInvPocketXY[HEAD2POS].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[HEAD2POS].sY = INV_INTERFACE_START_Y + 30;	// HEAD2POS
	gSMInvPocketXY[HANDPOS].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[HANDPOS].sY = INV_INTERFACE_START_Y + 84;	// HANDPOS,
	gSMInvPocketXY[SECONDHANDPOS].sX = INTERFACE_START_X + 226;	gSMInvPocketXY[SECONDHANDPOS].sY = INV_INTERFACE_START_Y + 108;	// SECONDHANDPOS
	gSMInvPocketXY[BIGPOCK1POS].sX = INTERFACE_START_X + 468;	gSMInvPocketXY[BIGPOCK1POS].sY = INV_INTERFACE_START_Y + 5;	// BIGPOCK1
	gSMInvPocketXY[BIGPOCK2POS].sX = INTERFACE_START_X + 468;	gSMInvPocketXY[BIGPOCK2POS].sY = INV_INTERFACE_START_Y + 29;	// BIGPOCK2
	gSMInvPocketXY[BIGPOCK3POS].sX = INTERFACE_START_X + 468;	gSMInvPocketXY[BIGPOCK3POS].sY = INV_INTERFACE_START_Y + 53;	// BIGPOCK3
	gSMInvPocketXY[BIGPOCK4POS].sX = INTERFACE_START_X + 468;	gSMInvPocketXY[BIGPOCK4POS].sY = INV_INTERFACE_START_Y + 77;	// BIGPOCK4
	gSMInvPocketXY[SMALLPOCK1POS].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[SMALLPOCK1POS].sY = INV_INTERFACE_START_Y + 5;	// SMALLPOCK1
	gSMInvPocketXY[SMALLPOCK2POS].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[SMALLPOCK2POS].sY = INV_INTERFACE_START_Y + 29;	// SMALLPOCK2
	gSMInvPocketXY[SMALLPOCK3POS].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[SMALLPOCK3POS].sY = INV_INTERFACE_START_Y + 53;	// SMALLPOCK3
	gSMInvPocketXY[SMALLPOCK4POS].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[SMALLPOCK4POS].sY = INV_INTERFACE_START_Y + 77;	// SMALLPOCK4
	gSMInvPocketXY[SMALLPOCK5POS].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[SMALLPOCK5POS].sY = INV_INTERFACE_START_Y + 5;	// SMALLPOCK5
	gSMInvPocketXY[SMALLPOCK6POS].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[SMALLPOCK6POS].sY = INV_INTERFACE_START_Y + 29;	// SMALLPOCK6
	gSMInvPocketXY[SMALLPOCK7POS].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[SMALLPOCK7POS].sY = INV_INTERFACE_START_Y + 53;	// SMALLPOCK7
	gSMInvPocketXY[SMALLPOCK8POS].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[SMALLPOCK8POS].sY = INV_INTERFACE_START_Y + 77;	// SMALLPOCK8
	
	SM_ITEMDESC_START_X		= ( 214 + INTERFACE_START_X );
	SM_ITEMDESC_START_Y		= ( 1 +	INV_INTERFACE_START_Y );
	SM_ITEMDESC_HEIGHT		= 128;
	SM_ITEMDESC_WIDTH		= 358;

	SM_SELMERC_AP_X			= ( 70 + INTERFACE_START_X );
	SM_SELMERC_AP_Y			= ( 53 + INV_INTERFACE_START_Y );
	SM_SELMERC_AP_HEIGHT	= 12;
	SM_SELMERC_AP_WIDTH		= 11;

	SM_SELMERC_HEALTH_X		= ( 69 + INTERFACE_START_X );
	SM_SELMERC_HEALTH_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_HEALTH_WIDTH = 3;
	SM_SELMERC_HEALTH_HEIGHT = 42;

	SM_SELMERC_BREATH_X		= ( 75 + INTERFACE_START_X );
	SM_SELMERC_BREATH_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_BREATH_WIDTH = 3;
	SM_SELMERC_BREATH_HEIGHT = 42;

	SM_SELMERC_MORALE_X		= ( 81 + INTERFACE_START_X );
	SM_SELMERC_MORALE_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_MORALE_WIDTH = 3;
	SM_SELMERC_MORALE_HEIGHT = 42;

	SM_SELMERC_BARS_TIP_X	= (	SM_SELMERC_HEALTH_X - SM_SELMERC_HEALTH_WIDTH );
	SM_SELMERC_BARS_TIP_Y	= ( SM_SELMERC_HEALTH_Y - SM_SELMERC_HEALTH_HEIGHT );
	SM_SELMERC_BARS_TIP_WIDTH = ((SM_SELMERC_BREATH_WIDTH + 4) *3);
	SM_SELMERC_BARS_TIP_HEIGHT= ( SM_SELMERC_HEALTH_HEIGHT );

	SM_SELMERCNAME_X		= ( 7 + INTERFACE_START_X );
	SM_SELMERCNAME_Y		= ( 55 + INV_INTERFACE_START_Y );
	SM_SELMERCNAME_WIDTH	= 53;
	SM_SELMERCNAME_HEIGHT	= 9;

	SM_SELMERC_FACE_X		= ( 13 + INTERFACE_START_X );
	SM_SELMERC_FACE_Y		= ( 6 + INV_INTERFACE_START_Y );
	SM_SELMERC_FACE_HEIGHT	= 42;
	SM_SELMERC_FACE_WIDTH	= 48;

	SM_SELMERC_PLATE_X		= ( 4 + INTERFACE_START_X );
	SM_SELMERC_PLATE_Y		= ( 2 + INV_INTERFACE_START_Y );
	SM_SELMERC_PLATE_HEIGHT = 65;
	SM_SELMERC_PLATE_WIDTH	= 83;

	SM_BODYINV_X			= ( 244 + INTERFACE_START_X );
	SM_BODYINV_Y			= ( 6 + INV_INTERFACE_START_Y );

	// Cammo
	gSMCamoXY.sX = SM_BODYINV_X; gSMCamoXY.sY = SM_BODYINV_Y;

	SM_TALKB_X				= ( 155 + INTERFACE_START_X );
	SM_TALKB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_MUTEB_X				= ( 91 + INTERFACE_START_X );
	SM_MUTEB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_STANCEUPB_X			= ( 187 + INTERFACE_START_X );
	SM_STANCEUPB_Y			= ( 40 + INV_INTERFACE_START_Y );
	SM_UPDOWNB_X			= ( 91 + INTERFACE_START_X );
	SM_UPDOWNB_Y			= ( 73 + INV_INTERFACE_START_Y );
	SM_CLIMBB_X				= ( 187 + INTERFACE_START_X );
	SM_CLIMBB_Y				= ( 8 + INV_INTERFACE_START_Y );
	SM_STANCEDOWNB_X		= ( 187 + INTERFACE_START_X );
	SM_STANCEDOWNB_Y		= ( 108 + INV_INTERFACE_START_Y );
	SM_HANDCURSORB_X		= ( 123 + INTERFACE_START_X );
	SM_HANDCURSORB_Y		= ( 73 + INV_INTERFACE_START_Y );
	SM_PREVMERCB_X			= ( 9 + INTERFACE_START_X );
	SM_PREVMERCB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_NEXTMERCB_X			= ( 51 + INTERFACE_START_X );
	SM_NEXTMERCB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_OPTIONSB_X			= ( 9 + INTERFACE_START_X );
	SM_OPTIONSB_Y			= ( 105 + INV_INTERFACE_START_Y );
	SM_BURSTMODEB_X			= ( 155 + INTERFACE_START_X );
	SM_BURSTMODEB_Y			= ( 73 + INV_INTERFACE_START_Y );
	SM_LOOKB_X				= ( 123 + INTERFACE_START_X );
	SM_LOOKB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_STEALTHMODE_X		= ( 187 + INTERFACE_START_X );
	SM_STEALTHMODE_Y		= ( 73 + INV_INTERFACE_START_Y );

	SM_DONE_X				=	xResOffset + (xResSize - 97);
	SM_MAPSCREEN_X			=	xResOffset + (xResSize - 51);

//dnl	INTERFACE_CLOCK_X		=	xResOffset + (xResSize - 86);
//dnl	INTERFACE_CLOCK_Y		= ( 119	+ INV_INTERFACE_START_Y );
	INTERFACE_CLOCK_X		= xResOffset + (xResSize - 86);
	INTERFACE_CLOCK_Y		= SCREEN_HEIGHT - 24;
	LOCATION_NAME_X			=	xResOffset + (xResSize - 92);
	LOCATION_NAME_Y			= ( 65	+ INTERFACE_START_Y	);
	
	SM_DONE_Y				= ( 4 + INV_INTERFACE_START_Y );
	SM_MAPSCREEN_Y			= ( 4 + INV_INTERFACE_START_Y );

	SM_POSITIONB_X			= ( 106 + INTERFACE_START_X );
	SM_POSITIONB_Y			= ( 34 + INV_INTERFACE_START_Y );
	SM_POSITIONB_WIDTH		= 19;
	SM_POSITIONB_HEIGHT		= 24;

	SM_PERCENT_WIDTH		= 20;
	SM_PERCENT_HEIGHT		= 10;
	SM_ARMOR_X				= ( 347 + INTERFACE_START_X );
	SM_ARMOR_Y				= ( 79 + INV_INTERFACE_START_Y );
	SM_ARMOR_LABEL_X		= ( 363 + INTERFACE_START_X );
	SM_ARMOR_LABEL_Y		= ( 69 + INV_INTERFACE_START_Y );
	SM_ARMOR_PERCENT_X		= ( 368 + INTERFACE_START_X );
	SM_ARMOR_PERCENT_Y		= ( 79 + INV_INTERFACE_START_Y );

	SM_WEIGHT_LABEL_X		= ( 430 + INTERFACE_START_X );
	SM_WEIGHT_LABEL_Y		= ( 107 + INV_INTERFACE_START_Y );
	SM_WEIGHT_PERCENT_X		= ( 449 + INTERFACE_START_X );
	SM_WEIGHT_PERCENT_Y		= ( 107 + INV_INTERFACE_START_Y );
	SM_WEIGHT_X				= ( 428 + INTERFACE_START_X );
	SM_WEIGHT_Y				= ( 106 + INV_INTERFACE_START_Y );

	SM_CAMMO_LABEL_X		= ( 430 + INTERFACE_START_X );
	SM_CAMMO_LABEL_Y		= ( 122 + INV_INTERFACE_START_Y );
	SM_CAMMO_PERCENT_X		= ( 449 + INTERFACE_START_X );
	SM_CAMMO_PERCENT_Y		= ( 121 + INV_INTERFACE_START_Y );
	SM_CAMMO_X				= ( 428 + INTERFACE_START_X );
	SM_CAMMO_Y				= ( 121 + INV_INTERFACE_START_Y );

	SM_STATS_WIDTH			= 16;
	SM_STATS_HEIGHT			= 8 ;
	SM_AGI_X				= ( 115 + INTERFACE_START_X );
	SM_AGI_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_DEX_X				= ( 115 + INTERFACE_START_X );
	SM_DEX_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_STR_X				= ( 115 + INTERFACE_START_X );
	SM_STR_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_CHAR_X				= ( 115 + INTERFACE_START_X );
	SM_CHAR_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_WIS_X				= ( 115 + INTERFACE_START_X );
	SM_WIS_Y				= ( 47 + INV_INTERFACE_START_Y );
	SM_EXPLVL_X				= ( 163 + INTERFACE_START_X );
	SM_EXPLVL_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_MRKM_X				= ( 163 + INTERFACE_START_X );
	SM_MRKM_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_MECH_X				= ( 163 + INTERFACE_START_X );
	SM_MECH_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_EXPL_X				= ( 163 + INTERFACE_START_X );
	SM_EXPL_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_MED_X				= ( 163 + INTERFACE_START_X );
	SM_MED_Y				= ( 47 + INV_INTERFACE_START_Y );

	MONEY_X					= ( 460 + INTERFACE_START_X );
	MONEY_Y					= ( 105 + INV_INTERFACE_START_Y );
	MONEY_WIDTH				= 30;
	MONEY_HEIGHT			= 22;

	// ow ye font color they can also be customizable :P
	STATS_TITLE_FONT_COLOR	= 6;
	STATS_TEXT_FONT_COLOR	= 5;

	// Keyring	496/106 on Inventory_Bottom_Panel.sti
	KEYRING_X			= xResOffset + 496;
	KEYRING_Y			= (106 + INV_INTERFACE_START_Y);

	// so we got everything "dynamic" now we just return TRUE
	return ( TRUE );
}
BOOLEAN InitializeSMPanelCoordsNew()
{
	// int i; // unused (jonathanl)
	/* 
	 * now i use standard positions and i add to them starting coords of panel but 
	 * we can add here function that will read coord in panel from file
	 * that will be phase2 of making GUI more customizable
	 * any questions? joker
	 */
	/* CHRISL: Inventory slot information was completly altered to allow for new inventory pockets and
	to allow the new inventory interface to function properly. Also altered many of the interface coords
	so that the new interface would line up correctly.*/
	// Inventory slots
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		gSMInvPocketXY[0].sX = INTERFACE_START_X + 239;		gSMInvPocketXY[0].sY = INV_INTERFACE_START_Y + 68;		// HELMETPOS
		gSMInvPocketXY[1].sX = INTERFACE_START_X + 239;		gSMInvPocketXY[1].sY = INV_INTERFACE_START_Y + 96;		// VESTPOS
		gSMInvPocketXY[2].sX = INTERFACE_START_X + 239;		gSMInvPocketXY[2].sY = INV_INTERFACE_START_Y + 157;		// LEGPOS
		gSMInvPocketXY[3].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[3].sY = INV_INTERFACE_START_Y + 68;		// HEAD1POS
		gSMInvPocketXY[4].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[4].sY = INV_INTERFACE_START_Y + 92;		// HEAD2POS
		gSMInvPocketXY[5].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[5].sY = INV_INTERFACE_START_Y + 146;		// HANDPOS
		gSMInvPocketXY[6].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[6].sY = INV_INTERFACE_START_Y + 170;		// SECONDHANDPOS
		gSMInvPocketXY[7].sX = INTERFACE_START_X + 274;		gSMInvPocketXY[7].sY = INV_INTERFACE_START_Y + 13;		// VESTPOCK
		gSMInvPocketXY[8].sX = INTERFACE_START_X + 292;		gSMInvPocketXY[8].sY = INV_INTERFACE_START_Y + 79;		// LTHIGHPOCK
		gSMInvPocketXY[9].sX = INTERFACE_START_X + 340;		gSMInvPocketXY[9].sY = INV_INTERFACE_START_Y + 79;		// RTHIGHPOCK
		gSMInvPocketXY[10].sX = INTERFACE_START_X + 443;	gSMInvPocketXY[10].sY = INV_INTERFACE_START_Y + 79;		// CPACKPOCK
		gSMInvPocketXY[11].sX = INTERFACE_START_X + 486;	gSMInvPocketXY[11].sY = INV_INTERFACE_START_Y + 13;		// BPACKPOCK
		gSMInvPocketXY[12].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[12].sY = INV_INTERFACE_START_Y + 79;		// GUNSLINGPOCKPOS
		gSMInvPocketXY[13].sX = INTERFACE_START_X + 499;	gSMInvPocketXY[13].sY = INV_INTERFACE_START_Y + 79;		// KNIFEPOCKPOS
		gSMInvPocketXY[14].sX = INTERFACE_START_X + 433;	gSMInvPocketXY[14].sY = INV_INTERFACE_START_Y + 116;	// BIGPOCK1
		gSMInvPocketXY[15].sX = INTERFACE_START_X + 433;	gSMInvPocketXY[15].sY = INV_INTERFACE_START_Y + 140;	// BIGPOCK2
		gSMInvPocketXY[16].sX = INTERFACE_START_X + 433;	gSMInvPocketXY[16].sY = INV_INTERFACE_START_Y + 164;	// BIGPOCK3
		gSMInvPocketXY[17].sX = INTERFACE_START_X + 582;	gSMInvPocketXY[17].sY = INV_INTERFACE_START_Y + 10;		// BIGPOCK4
		gSMInvPocketXY[18].sX = INTERFACE_START_X + 582;	gSMInvPocketXY[18].sY = INV_INTERFACE_START_Y + 34;		// BIGPOCK5
		gSMInvPocketXY[19].sX = INTERFACE_START_X + 582;	gSMInvPocketXY[19].sY = INV_INTERFACE_START_Y + 58;		// BIGPOCK6
		gSMInvPocketXY[20].sX = INTERFACE_START_X + 582;	gSMInvPocketXY[20].sY = INV_INTERFACE_START_Y + 82;		// BIGPOCK7
		gSMInvPocketXY[21].sX = INTERFACE_START_X + 439;	gSMInvPocketXY[21].sY = INV_INTERFACE_START_Y + 10;		// MEDPOCK1
		gSMInvPocketXY[22].sX = INTERFACE_START_X + 439;	gSMInvPocketXY[22].sY = INV_INTERFACE_START_Y + 34;		// MEDPOCK2
		gSMInvPocketXY[23].sX = INTERFACE_START_X + 295;	gSMInvPocketXY[23].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK3
		gSMInvPocketXY[24].sX = INTERFACE_START_X + 343;	gSMInvPocketXY[24].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK4
		gSMInvPocketXY[25].sX = INTERFACE_START_X + 324;	gSMInvPocketXY[25].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK1
		gSMInvPocketXY[26].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[26].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK2
		gSMInvPocketXY[27].sX = INTERFACE_START_X + 370;	gSMInvPocketXY[27].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK3
		gSMInvPocketXY[28].sX = INTERFACE_START_X + 393;	gSMInvPocketXY[28].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK4
		gSMInvPocketXY[29].sX = INTERFACE_START_X + 416;	gSMInvPocketXY[29].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK5
		gSMInvPocketXY[30].sX = INTERFACE_START_X + 324;	gSMInvPocketXY[30].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK6
		gSMInvPocketXY[31].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[31].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK7
		gSMInvPocketXY[32].sX = INTERFACE_START_X + 370;	gSMInvPocketXY[32].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK8	
		gSMInvPocketXY[33].sX = INTERFACE_START_X + 393;	gSMInvPocketXY[33].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK9
		gSMInvPocketXY[34].sX = INTERFACE_START_X + 416;	gSMInvPocketXY[34].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK10
		gSMInvPocketXY[35].sX = INTERFACE_START_X + 291;	gSMInvPocketXY[35].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK11
		gSMInvPocketXY[36].sX = INTERFACE_START_X + 314;	gSMInvPocketXY[36].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK12
		gSMInvPocketXY[37].sX = INTERFACE_START_X + 291;	gSMInvPocketXY[37].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK13
		gSMInvPocketXY[38].sX = INTERFACE_START_X + 314;	gSMInvPocketXY[38].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK14
		gSMInvPocketXY[39].sX = INTERFACE_START_X + 339;	gSMInvPocketXY[39].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK15
		gSMInvPocketXY[40].sX = INTERFACE_START_X + 362;	gSMInvPocketXY[40].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK16
		gSMInvPocketXY[41].sX = INTERFACE_START_X + 339;	gSMInvPocketXY[41].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK17
		gSMInvPocketXY[42].sX = INTERFACE_START_X + 362;	gSMInvPocketXY[42].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK18
		gSMInvPocketXY[43].sX = INTERFACE_START_X + 387;	gSMInvPocketXY[43].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK19
		gSMInvPocketXY[44].sX = INTERFACE_START_X + 410;	gSMInvPocketXY[44].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK20
		gSMInvPocketXY[45].sX = INTERFACE_START_X + 387;	gSMInvPocketXY[45].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK21
		gSMInvPocketXY[46].sX = INTERFACE_START_X + 410;	gSMInvPocketXY[46].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK22
		gSMInvPocketXY[47].sX = INTERFACE_START_X + 536;	gSMInvPocketXY[47].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK23
		gSMInvPocketXY[48].sX = INTERFACE_START_X + 536;	gSMInvPocketXY[48].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK24
		gSMInvPocketXY[49].sX = INTERFACE_START_X + 536;	gSMInvPocketXY[49].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK25
		gSMInvPocketXY[50].sX = INTERFACE_START_X + 536;	gSMInvPocketXY[50].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK26
		gSMInvPocketXY[51].sX = INTERFACE_START_X + 559;	gSMInvPocketXY[51].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK27
		gSMInvPocketXY[52].sX = INTERFACE_START_X + 559;	gSMInvPocketXY[52].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK28
		gSMInvPocketXY[53].sX = INTERFACE_START_X + 559;	gSMInvPocketXY[53].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK29
		gSMInvPocketXY[54].sX = INTERFACE_START_X + 559;	gSMInvPocketXY[54].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK30
	}
	else if (iResolution < _1024x768)
	{
		gSMInvPocketXY[0].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[0].sY = INV_INTERFACE_START_Y + 68;		// HELMETPOS
		gSMInvPocketXY[1].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[1].sY = INV_INTERFACE_START_Y + 97;		// VESTPOS
		gSMInvPocketXY[2].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[2].sY = INV_INTERFACE_START_Y + 157;		// LEGPOS
		gSMInvPocketXY[3].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[3].sY = INV_INTERFACE_START_Y + 68;		// HEAD1POS
		gSMInvPocketXY[4].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[4].sY = INV_INTERFACE_START_Y + 92;		// HEAD2POS
		gSMInvPocketXY[5].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[5].sY = INV_INTERFACE_START_Y + 146;		// HANDPOS
		gSMInvPocketXY[6].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[6].sY = INV_INTERFACE_START_Y + 170;		// SECONDHANDPOS
		gSMInvPocketXY[7].sX = INTERFACE_START_X + 284;		gSMInvPocketXY[7].sY = INV_INTERFACE_START_Y + 13;		// VESTPOCK
		gSMInvPocketXY[8].sX = INTERFACE_START_X + 323;		gSMInvPocketXY[8].sY = INV_INTERFACE_START_Y + 79;		// LTHIGHPOCK
		gSMInvPocketXY[9].sX = INTERFACE_START_X + 439;		gSMInvPocketXY[9].sY = INV_INTERFACE_START_Y + 79;		// RTHIGHPOCK
		gSMInvPocketXY[10].sX = INTERFACE_START_X + 586;	gSMInvPocketXY[10].sY = INV_INTERFACE_START_Y + 79;		// CPACKPOCK
		gSMInvPocketXY[11].sX = INTERFACE_START_X + 594;	gSMInvPocketXY[11].sY = INV_INTERFACE_START_Y + 13;		// BPACKPOCK
		gSMInvPocketXY[12].sX = INTERFACE_START_X + 505;	gSMInvPocketXY[12].sY = INV_INTERFACE_START_Y + 79;		// GUNSLINGPOCKPOS
		gSMInvPocketXY[13].sX = INTERFACE_START_X + 388;	gSMInvPocketXY[13].sY = INV_INTERFACE_START_Y + 79;		// KNIFEPOCKPOS
		gSMInvPocketXY[14].sX = INTERFACE_START_X + 565;	gSMInvPocketXY[14].sY = INV_INTERFACE_START_Y + 116;	// BIGPOCK1
		gSMInvPocketXY[15].sX = INTERFACE_START_X + 565;	gSMInvPocketXY[15].sY = INV_INTERFACE_START_Y + 140;	// BIGPOCK2
		gSMInvPocketXY[16].sX = INTERFACE_START_X + 565;	gSMInvPocketXY[16].sY = INV_INTERFACE_START_Y + 164;	// BIGPOCK3
		gSMInvPocketXY[17].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[17].sY = INV_INTERFACE_START_Y + 10;		// BIGPOCK4
		gSMInvPocketXY[18].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[18].sY = INV_INTERFACE_START_Y + 34;		// BIGPOCK5
		gSMInvPocketXY[19].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[19].sY = INV_INTERFACE_START_Y + 58;		// BIGPOCK6
		gSMInvPocketXY[20].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[20].sY = INV_INTERFACE_START_Y + 82;		// BIGPOCK7
		gSMInvPocketXY[21].sX = INTERFACE_START_X + 527;	gSMInvPocketXY[21].sY = INV_INTERFACE_START_Y + 10;		// MEDPOCK1
		gSMInvPocketXY[22].sX = INTERFACE_START_X + 527;	gSMInvPocketXY[22].sY = INV_INTERFACE_START_Y + 34;		// MEDPOCK2
		gSMInvPocketXY[23].sX = INTERFACE_START_X + 333;	gSMInvPocketXY[23].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK3
		gSMInvPocketXY[24].sX = INTERFACE_START_X + 419;	gSMInvPocketXY[24].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK4
		gSMInvPocketXY[25].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[25].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK1
		gSMInvPocketXY[26].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[26].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK2
		gSMInvPocketXY[27].sX = INTERFACE_START_X + 419;	gSMInvPocketXY[27].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK3
		gSMInvPocketXY[28].sX = INTERFACE_START_X + 455;	gSMInvPocketXY[28].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK4
		gSMInvPocketXY[29].sX = INTERFACE_START_X + 491;	gSMInvPocketXY[29].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK5
		gSMInvPocketXY[30].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[30].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK6
		gSMInvPocketXY[31].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[31].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK7
		gSMInvPocketXY[32].sX = INTERFACE_START_X + 419;	gSMInvPocketXY[32].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK8	
		gSMInvPocketXY[33].sX = INTERFACE_START_X + 455;	gSMInvPocketXY[33].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK9
		gSMInvPocketXY[34].sX = INTERFACE_START_X + 491;	gSMInvPocketXY[34].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK10
		gSMInvPocketXY[35].sX = INTERFACE_START_X + 321;	gSMInvPocketXY[35].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK11
		gSMInvPocketXY[36].sX = INTERFACE_START_X + 357;	gSMInvPocketXY[36].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK12
		gSMInvPocketXY[37].sX = INTERFACE_START_X + 321;	gSMInvPocketXY[37].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK13
		gSMInvPocketXY[38].sX = INTERFACE_START_X + 357;	gSMInvPocketXY[38].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK14
		gSMInvPocketXY[39].sX = INTERFACE_START_X + 407;	gSMInvPocketXY[39].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK15
		gSMInvPocketXY[40].sX = INTERFACE_START_X + 443;	gSMInvPocketXY[40].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK16
		gSMInvPocketXY[41].sX = INTERFACE_START_X + 407;	gSMInvPocketXY[41].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK17
		gSMInvPocketXY[42].sX = INTERFACE_START_X + 443;	gSMInvPocketXY[42].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK18
		gSMInvPocketXY[43].sX = INTERFACE_START_X + 493;	gSMInvPocketXY[43].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK19
		gSMInvPocketXY[44].sX = INTERFACE_START_X + 529;	gSMInvPocketXY[44].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK20
		gSMInvPocketXY[45].sX = INTERFACE_START_X + 493;	gSMInvPocketXY[45].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK21
		gSMInvPocketXY[46].sX = INTERFACE_START_X + 529;	gSMInvPocketXY[46].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK22
		gSMInvPocketXY[47].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[47].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK23
		gSMInvPocketXY[48].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[48].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK24
		gSMInvPocketXY[49].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[49].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK25
		gSMInvPocketXY[50].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[50].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK26
		gSMInvPocketXY[51].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[51].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK27
		gSMInvPocketXY[52].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[52].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK28
		gSMInvPocketXY[53].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[53].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK29
		gSMInvPocketXY[54].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[54].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK30
	}
	else
	{
		//1024x768 resolution
		gSMInvPocketXY[0].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[0].sY = INV_INTERFACE_START_Y + 68;		// HELMETPOS
		gSMInvPocketXY[1].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[1].sY = INV_INTERFACE_START_Y + 97;		// VESTPOS
		gSMInvPocketXY[2].sX = INTERFACE_START_X + 258;		gSMInvPocketXY[2].sY = INV_INTERFACE_START_Y + 157;		// LEGPOS
		gSMInvPocketXY[3].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[3].sY = INV_INTERFACE_START_Y + 68;		// HEAD1POS
		gSMInvPocketXY[4].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[4].sY = INV_INTERFACE_START_Y + 92;		// HEAD2POS
		gSMInvPocketXY[5].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[5].sY = INV_INTERFACE_START_Y + 146;		// HANDPOS
		gSMInvPocketXY[6].sX = INTERFACE_START_X + 124;		gSMInvPocketXY[6].sY = INV_INTERFACE_START_Y + 170;		// SECONDHANDPOS
		gSMInvPocketXY[7].sX = INTERFACE_START_X + 284;		gSMInvPocketXY[7].sY = INV_INTERFACE_START_Y + 13;		// VESTPOCK
		gSMInvPocketXY[8].sX = INTERFACE_START_X + 358;		gSMInvPocketXY[8].sY = INV_INTERFACE_START_Y + 79;		// LTHIGHPOCK
		gSMInvPocketXY[9].sX = INTERFACE_START_X + 516;		gSMInvPocketXY[9].sY = INV_INTERFACE_START_Y + 79;		// RTHIGHPOCK
		gSMInvPocketXY[10].sX = INTERFACE_START_X + 594;	gSMInvPocketXY[10].sY = INV_INTERFACE_START_Y + 128;	// CPACKPOCK
		gSMInvPocketXY[11].sX = INTERFACE_START_X + 594;	gSMInvPocketXY[11].sY = INV_INTERFACE_START_Y + 13;		// BPACKPOCK
		gSMInvPocketXY[12].sX = INTERFACE_START_X + 429;	gSMInvPocketXY[12].sY = INV_INTERFACE_START_Y + 79;		// GUNSLINGPOCKPOS
		gSMInvPocketXY[13].sX = INTERFACE_START_X + 594;	gSMInvPocketXY[13].sY = INV_INTERFACE_START_Y + 79;		// KNIFEPOCKPOS
		gSMInvPocketXY[14].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[14].sY = INV_INTERFACE_START_Y + 116;	// BIGPOCK1
		gSMInvPocketXY[15].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[15].sY = INV_INTERFACE_START_Y + 140;	// BIGPOCK2
		gSMInvPocketXY[16].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[16].sY = INV_INTERFACE_START_Y + 164;	// BIGPOCK3
		gSMInvPocketXY[17].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[17].sY = INV_INTERFACE_START_Y + 10;		// BIGPOCK4
		gSMInvPocketXY[18].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[18].sY = INV_INTERFACE_START_Y + 34;		// BIGPOCK5
		gSMInvPocketXY[19].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[19].sY = INV_INTERFACE_START_Y + 58;		// BIGPOCK6
		gSMInvPocketXY[20].sX = INTERFACE_START_X + 729;	gSMInvPocketXY[20].sY = INV_INTERFACE_START_Y + 82;		// BIGPOCK7
		gSMInvPocketXY[21].sX = INTERFACE_START_X + 527;	gSMInvPocketXY[21].sY = INV_INTERFACE_START_Y + 10;		// MEDPOCK1
		gSMInvPocketXY[22].sX = INTERFACE_START_X + 527;	gSMInvPocketXY[22].sY = INV_INTERFACE_START_Y + 34;		// MEDPOCK2
		gSMInvPocketXY[23].sX = INTERFACE_START_X + 359;	gSMInvPocketXY[23].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK3
		gSMInvPocketXY[24].sX = INTERFACE_START_X + 517;	gSMInvPocketXY[24].sY = INV_INTERFACE_START_Y + 164;	// MEDPOCK4
		gSMInvPocketXY[25].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[25].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK1
		gSMInvPocketXY[26].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[26].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK2
		gSMInvPocketXY[27].sX = INTERFACE_START_X + 419;	gSMInvPocketXY[27].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK3
		gSMInvPocketXY[28].sX = INTERFACE_START_X + 455;	gSMInvPocketXY[28].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK4
		gSMInvPocketXY[29].sX = INTERFACE_START_X + 491;	gSMInvPocketXY[29].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK5
		gSMInvPocketXY[30].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[30].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK6
		gSMInvPocketXY[31].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[31].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK7
		gSMInvPocketXY[32].sX = INTERFACE_START_X + 419;	gSMInvPocketXY[32].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK8	
		gSMInvPocketXY[33].sX = INTERFACE_START_X + 455;	gSMInvPocketXY[33].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK9
		gSMInvPocketXY[34].sX = INTERFACE_START_X + 491;	gSMInvPocketXY[34].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK10
		gSMInvPocketXY[35].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[35].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK11
		gSMInvPocketXY[36].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[36].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK12
		gSMInvPocketXY[37].sX = INTERFACE_START_X + 347;	gSMInvPocketXY[37].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK13
		gSMInvPocketXY[38].sX = INTERFACE_START_X + 383;	gSMInvPocketXY[38].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK14
		gSMInvPocketXY[39].sX = INTERFACE_START_X + 505;	gSMInvPocketXY[39].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK15
		gSMInvPocketXY[40].sX = INTERFACE_START_X + 541;	gSMInvPocketXY[40].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK16
		gSMInvPocketXY[41].sX = INTERFACE_START_X + 505;	gSMInvPocketXY[41].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK17
		gSMInvPocketXY[42].sX = INTERFACE_START_X + 541;	gSMInvPocketXY[42].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK18
		gSMInvPocketXY[43].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[43].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK19
		gSMInvPocketXY[44].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[44].sY = INV_INTERFACE_START_Y + 116;	// SMALLPOCK20
		gSMInvPocketXY[45].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[45].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK21
		gSMInvPocketXY[46].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[46].sY = INV_INTERFACE_START_Y + 140;	// SMALLPOCK22
		gSMInvPocketXY[47].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[47].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK23
		gSMInvPocketXY[48].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[48].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK24
		gSMInvPocketXY[49].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[49].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK25
		gSMInvPocketXY[50].sX = INTERFACE_START_X + 657;	gSMInvPocketXY[50].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK26
		gSMInvPocketXY[51].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[51].sY = INV_INTERFACE_START_Y + 10;		// SMALLPOCK27
		gSMInvPocketXY[52].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[52].sY = INV_INTERFACE_START_Y + 34;		// SMALLPOCK28
		gSMInvPocketXY[53].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[53].sY = INV_INTERFACE_START_Y + 58;		// SMALLPOCK29
		gSMInvPocketXY[54].sX = INTERFACE_START_X + 693;	gSMInvPocketXY[54].sY = INV_INTERFACE_START_Y + 82;		// SMALLPOCK30
	}

	SM_ITEMDESC_START_X		= ( 114 + INTERFACE_START_X );	//115
	SM_ITEMDESC_START_Y		= ( 1 +   INV_INTERFACE_START_Y );
	SM_ITEMDESC_HEIGHT		= 128;
	SM_ITEMDESC_WIDTH		= 358;

	SM_SELMERC_AP_X			= ( 71 + INTERFACE_START_X );
	SM_SELMERC_AP_Y			= ( 53 + INV_INTERFACE_START_Y );
	SM_SELMERC_AP_HEIGHT	= 12;
	SM_SELMERC_AP_WIDTH		= 11;
 
	SM_SELMERC_HEALTH_X		= ( 70 + INTERFACE_START_X );
	SM_SELMERC_HEALTH_Y		= ( 48 + INV_INTERFACE_START_Y );
	SM_SELMERC_HEALTH_WIDTH = 3;
	SM_SELMERC_HEALTH_HEIGHT = 42;

	SM_SELMERC_BREATH_X		= ( 76 + INTERFACE_START_X );
	SM_SELMERC_BREATH_Y		= ( 48 + INV_INTERFACE_START_Y );
	SM_SELMERC_BREATH_WIDTH = 3;
	SM_SELMERC_BREATH_HEIGHT = 42;

	SM_SELMERC_MORALE_X		= ( 82 + INTERFACE_START_X );
	SM_SELMERC_MORALE_Y		= ( 48 + INV_INTERFACE_START_Y );
	SM_SELMERC_MORALE_WIDTH = 3;
	SM_SELMERC_MORALE_HEIGHT = 42;

	SM_SELMERC_BARS_TIP_X	= (	SM_SELMERC_HEALTH_X - SM_SELMERC_HEALTH_WIDTH );
	SM_SELMERC_BARS_TIP_Y	= ( SM_SELMERC_HEALTH_Y - SM_SELMERC_HEALTH_HEIGHT );
	SM_SELMERC_BARS_TIP_WIDTH = ((SM_SELMERC_BREATH_WIDTH + 4) *3);
	SM_SELMERC_BARS_TIP_HEIGHT= ( SM_SELMERC_HEALTH_HEIGHT );

	SM_SELMERCNAME_X		= ( 12 + INTERFACE_START_X );
	SM_SELMERCNAME_Y		= ( 55 + INV_INTERFACE_START_Y );
	SM_SELMERCNAME_WIDTH	= 53;
	SM_SELMERCNAME_HEIGHT	= 11;

	SM_SELMERC_FACE_X		= ( 14 + INTERFACE_START_X );
	SM_SELMERC_FACE_Y		= ( 5 + INV_INTERFACE_START_Y );
	SM_SELMERC_FACE_HEIGHT	= 42;
	SM_SELMERC_FACE_WIDTH	= 48;

	SM_SELMERC_PLATE_X		= ( 5 + INTERFACE_START_X );
	SM_SELMERC_PLATE_Y		= ( 2 + INV_INTERFACE_START_Y );
	SM_SELMERC_PLATE_HEIGHT = 65;
	SM_SELMERC_PLATE_WIDTH	= 83;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		SM_BODYINV_X			= ( 141 + INTERFACE_START_X );
		SM_BODYINV_Y			= ( 68 + INV_INTERFACE_START_Y );
	}
	else
	{
		SM_BODYINV_X			= ( 151 + INTERFACE_START_X );
		SM_BODYINV_Y			= ( 68 + INV_INTERFACE_START_Y );
	}

	// Cammo
	gSMCamoXY.sX = SM_BODYINV_X; gSMCamoXY.sY = SM_BODYINV_Y;

	SM_UPDOWNB_X			= ( 8 + INTERFACE_START_X );
	SM_UPDOWNB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_HANDCURSORB_X		= ( 33 + INTERFACE_START_X );
	SM_HANDCURSORB_Y		= ( 70 + INV_INTERFACE_START_Y );
	SM_CLIMBB_X				= ( 58 + INTERFACE_START_X );
	SM_CLIMBB_Y				= ( 70 + INV_INTERFACE_START_Y );
	SM_MUTEB_X				= ( 8 + INTERFACE_START_X );
	SM_MUTEB_Y				= ( 98 + INV_INTERFACE_START_Y );
	SM_LOOKB_X				= ( 33 + INTERFACE_START_X );
	SM_LOOKB_Y				= ( 98 + INV_INTERFACE_START_Y );
	SM_TALKB_X				= ( 58 + INTERFACE_START_X );
	SM_TALKB_Y				= ( 98 + INV_INTERFACE_START_Y );
	SM_STANCEUPB_X			= ( 88 + INTERFACE_START_X );
	SM_STANCEUPB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_STEALTHMODE_X		= ( 88 + INTERFACE_START_X );
	SM_STEALTHMODE_Y		= ( 98 + INV_INTERFACE_START_Y );
	SM_STANCEDOWNB_X		= ( 88 + INTERFACE_START_X );
	SM_STANCEDOWNB_Y		= ( 126 + INV_INTERFACE_START_Y );
	SM_BURSTMODEB_X			= ( 88 + INTERFACE_START_X );
	SM_BURSTMODEB_Y			= ( 167 + INV_INTERFACE_START_Y );
	SM_PREVMERCB_X			= ( 10 + INTERFACE_START_X );
	SM_PREVMERCB_Y			= ( 130 + INV_INTERFACE_START_Y );
	SM_NEXTMERCB_X			= ( 52 + INTERFACE_START_X );
	SM_NEXTMERCB_Y			= ( 130 + INV_INTERFACE_START_Y );
	SM_OPTIONSB_X			= ( 10 + INTERFACE_START_X );
	SM_OPTIONSB_Y			= ( 165 + INV_INTERFACE_START_Y );
	
	// CHRISL: Backpack buttons
	SM_DROPPACK_X			= ( 612 + INTERFACE_START_X );
	SM_DROPPACK_Y			= ( 39 + INV_INTERFACE_START_Y );
	SM_ZIPPER_X				= ( 590 + INTERFACE_START_X );
	SM_ZIPPER_Y				= ( 39 + INV_INTERFACE_START_Y );
	SM_MAPSCREEN_X			= xResOffset + (xResSize - 146);	// 152
	SM_DONE_X				= xResOffset + (xResSize - 146);	// 152

//dnl	INTERFACE_CLOCK_X		= xResOffset + (xResSize - 86);
//dnl	INTERFACE_CLOCK_Y		= ( 119	+ INV_INTERFACE_START_Y );
	INTERFACE_CLOCK_X		= xResOffset + (xResSize - 86);
	INTERFACE_CLOCK_Y		= SCREEN_HEIGHT - 24;
	LOCATION_NAME_X			= xResOffset + (xResSize - 92);
	LOCATION_NAME_Y			= ( 89	+ INTERFACE_START_Y	);

	SM_DONE_Y				= ( 118 + INV_INTERFACE_START_Y );
	SM_MAPSCREEN_Y			= ( 140 + INV_INTERFACE_START_Y );

	SM_POSITIONB_X			= ( 106 + INTERFACE_START_X );
	SM_POSITIONB_Y			= ( 34 + INV_INTERFACE_START_Y );
	SM_POSITIONB_WIDTH		= 19;
	SM_POSITIONB_HEIGHT		= 24;

	SM_PERCENT_WIDTH		= 20;
	SM_PERCENT_HEIGHT		= 10;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		SM_ARMOR_X				= ( 245 + INTERFACE_START_X );
		SM_ARMOR_Y				= ( 141 + INV_INTERFACE_START_Y );
		SM_ARMOR_LABEL_X		= ( 260 + INTERFACE_START_X );
		SM_ARMOR_LABEL_Y		= ( 131 + INV_INTERFACE_START_Y );
		SM_ARMOR_PERCENT_X		= ( 265 + INTERFACE_START_X );
		SM_ARMOR_PERCENT_Y		= ( 141 + INV_INTERFACE_START_Y );
	}
	else
	{
		SM_ARMOR_X				= ( 264 + INTERFACE_START_X );
		SM_ARMOR_Y				= ( 141 + INV_INTERFACE_START_Y );
		SM_ARMOR_LABEL_X		= ( 279 + INTERFACE_START_X );
		SM_ARMOR_LABEL_Y		= ( 131 + INV_INTERFACE_START_Y );
		SM_ARMOR_PERCENT_X		= ( 284 + INTERFACE_START_X );
		SM_ARMOR_PERCENT_Y		= ( 141 + INV_INTERFACE_START_Y );
	}

	SM_WEIGHT_LABEL_X		= ( 220 + INTERFACE_START_X );
	SM_WEIGHT_LABEL_Y		= ( 34 + INV_INTERFACE_START_Y );
	SM_WEIGHT_PERCENT_X		= ( 239 + INTERFACE_START_X );
	SM_WEIGHT_PERCENT_Y		= ( 34 + INV_INTERFACE_START_Y );
	SM_WEIGHT_X				= ( 218 + INTERFACE_START_X );
	SM_WEIGHT_Y				= ( 33 + INV_INTERFACE_START_Y );

	SM_CAMMO_LABEL_X		= ( 220 + INTERFACE_START_X );
	SM_CAMMO_LABEL_Y		= ( 50 + INV_INTERFACE_START_Y );
	SM_CAMMO_PERCENT_X		= ( 239 + INTERFACE_START_X );
	SM_CAMMO_PERCENT_Y		= ( 49 + INV_INTERFACE_START_Y );
	SM_CAMMO_X				= ( 218 + INTERFACE_START_X );
	SM_CAMMO_Y				= ( 49 + INV_INTERFACE_START_Y );

	SM_STATS_WIDTH			= 16;
	SM_STATS_HEIGHT			= 8 ;
	SM_AGI_X				= ( 115 + INTERFACE_START_X );
	SM_AGI_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_DEX_X				= ( 115 + INTERFACE_START_X );
	SM_DEX_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_STR_X				= ( 115 + INTERFACE_START_X );
	SM_STR_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_CHAR_X				= ( 115 + INTERFACE_START_X );
	SM_CHAR_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_WIS_X				= ( 115 + INTERFACE_START_X );
	SM_WIS_Y				= ( 47 + INV_INTERFACE_START_Y );
	SM_EXPLVL_X				= ( 163 + INTERFACE_START_X );
	SM_EXPLVL_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_MRKM_X				= ( 163 + INTERFACE_START_X );
	SM_MRKM_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_MECH_X				= ( 163 + INTERFACE_START_X );
	SM_MECH_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_EXPL_X				= ( 163 + INTERFACE_START_X );
	SM_EXPL_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_MED_X				= ( 163 + INTERFACE_START_X );
	SM_MED_Y				= ( 47 + INV_INTERFACE_START_Y );

	MONEY_X					= ( 185 + INTERFACE_START_X );
	MONEY_Y					= ( 5 + INV_INTERFACE_START_Y );
	MONEY_WIDTH				= 30;
	MONEY_HEIGHT			= 22;
	
	// ow ye font color they can also be customizable :P
	STATS_TITLE_FONT_COLOR	= 6;
	STATS_TEXT_FONT_COLOR	= 5;

	//Keyring 218/5 on Inventory_Bottom_Panel.sti
	KEYRING_X			= xResOffset + 219; //209;
	KEYRING_Y			= (5 + INV_INTERFACE_START_Y);

	// so we got everything "dynamic" now we just return TRUE
	return ( TRUE );
}

BOOLEAN InitializeSMPanel(	)
{
	VOBJECT_DESC	VObjectDesc;

 /*	OK i need to initialize coords here
 *	Isnt it cool
 *	any questions? joker
 */
//	InitializeSMPanelCoords( );

  fDisplayOverheadMap = TRUE;

	// failing the CHECKF after this will cause you to lose your mouse
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel.STI" );
	}
	else if (iResolution < _1024x768)
	{
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel_800x600.STI" );
	}
	else
	{
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel_1024x768.STI" );
	}

	CHECKF( AddVideoObject( &VObjectDesc, &guiSMPanel) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_gold_front.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSMObjects ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inv_frn.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSMObjects2 ) );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\secondary_gun_hidden.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSecItemHiddenVO ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Bars.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );


	// Clear inv display stuff
	memset( gfSM_HandInvDispText, 0, sizeof( gfSM_HandInvDispText ) );

	// INIT viewport region
	// Set global mouse regions for SM panel
	// Define region for viewport
	MSYS_DefineRegion( &gViewportRegion, 0, 0 ,gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y, MSYS_PRIORITY_NORMAL, VIDEO_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gViewportRegion );

	// Create buttons
	CHECKF( CreateSMPanelButtons( ) );

	// Set viewports
	// Define region for panel
	MSYS_DefineRegion( &gSMPanelRegion, 0, INV_INTERFACE_START_Y ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, InvPanelButtonClickCallback );
	// Add region
	MSYS_AddRegion( &gSMPanelRegion);

	//DEfine region for selected guy panel
	MSYS_DefineRegion( &gSM_SELMERCPanelRegion, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, SM_SELMERC_FACE_X + SM_SELMERC_FACE_WIDTH, SM_SELMERC_FACE_Y + SM_SELMERC_FACE_HEIGHT, MSYS_PRIORITY_NORMAL,
						MSYS_NO_CURSOR, SelectedMercButtonMoveCallback, SelectedMercButtonCallback );
	// Add region
	MSYS_AddRegion( &gSM_SELMERCPanelRegion );



	//DEfine region for selected guy panel
	MSYS_DefineRegion( &gSM_SELMERCEnemyIndicatorRegion, SM_SELMERC_FACE_X + 1, SM_SELMERC_FACE_Y + 1, SM_SELMERC_FACE_X + INDICATOR_BOX_WIDTH, SM_SELMERC_FACE_Y + INDICATOR_BOX_HEIGHT, MSYS_PRIORITY_NORMAL,
						MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercEnemyIndicatorCallback );
	// Add region
	MSYS_AddRegion( &gSM_SELMERCEnemyIndicatorRegion );


	//DEfine region for money button
	MSYS_DefineRegion( &gSM_SELMERCMoneyRegion, MONEY_X, MONEY_Y, MONEY_X + MONEY_WIDTH, MONEY_Y + MONEY_HEIGHT, MSYS_PRIORITY_HIGH,
						MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SMInvMoneyButtonCallback );
	// Add region
	MSYS_AddRegion( &gSM_SELMERCMoneyRegion );

	SetRegionFastHelpText( &(gSM_SELMERCMoneyRegion), TacticalStr[ MONEY_BUTTON_HELP_TEXT ] );



	// Check if mouse is in region and if so, adjust...
	if ( IsMouseInRegion( &gSM_SELMERCPanelRegion ) )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, TRUE );
	}


	//DEfine region for selected guy panel
	// THIS IS A REGION OF BAR TIPS
	MSYS_DefineRegion( &gSM_SELMERCBarsRegion, SM_SELMERC_BARS_TIP_X, SM_SELMERC_BARS_TIP_Y,
					SM_SELMERC_BARS_TIP_X + SM_SELMERC_BARS_TIP_WIDTH, SM_SELMERC_BARS_TIP_Y + SM_SELMERC_BARS_TIP_HEIGHT, MSYS_PRIORITY_NORMAL,
						MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercButtonCallback );
	MSYS_AddRegion( &gSM_SELMERCBarsRegion );
	
	MSYS_DefineRegion( &gSM_SELMERCWeightRegion, (SM_WEIGHT_X + 2), (SM_WEIGHT_Y),
					   (SM_WEIGHT_X + 28), (SM_WEIGHT_Y + 10), MSYS_PRIORITY_NORMAL,
					   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercButtonCallback );
	MSYS_AddRegion( &gSM_SELMERCWeightRegion );

	// Added by SANDRO - define region for camo tooltip window
	MSYS_DefineRegion( &gSM_SELMERCCamoRegion, ( SM_CAMMO_X + 2), ( SM_CAMMO_Y ),
					(SM_CAMMO_X + 28), (SM_CAMMO_Y + 10), MSYS_PRIORITY_NORMAL,
						MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercButtonCallback );
	MSYS_AddRegion( &gSM_SELMERCCamoRegion );

	// CHRISL: Setup default coords
	if((UsingNewInventorySystem() == true))
		InitInventoryNew();
	else
		InitInventoryOld();
	
	InitInvSlotInterface( gSMInvPocketXY, &gSMCamoXY, SMInvMoveCallback, SMInvClickCallback, SMInvMoveCammoCallback, SMInvClickCamoCallback, FALSE );

	InitKeyRingInterface( KeyRingItemPanelButtonCallback );


	// this is important! It will disable buttons like SM_MAP_SCREEN_BUTTON when they're supposed to be disabled - the previous
	// disabled state is lost everytime panel is reinitialized, because all the buttons are created from scratch!
	if ( gpItemPointer == NULL )
	{
		// empty cursor - enable, not from item pickup
		EnableSMPanelButtons( TRUE, FALSE );
	}
	else
	{
		// full cursor - disable, from item pickup
		EnableSMPanelButtons( FALSE, TRUE );
	}
	
	if( INTERFACE_START_X > 0 )
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, INV_INTERFACE_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
		ColorFillVideoSurfaceArea( guiSAVEBUFFER, 0, INV_INTERFACE_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	}

	return( TRUE );
}


BOOLEAN CreateSMPanelButtons( )
{
	CHAR8 ubString[48];
  //CHAR8 ubString2[48]; //legion DisableButton
	//CHAR8 ubString3[48]; //legion

	// CHRISL: Reset backpack buttons as well
	giSMStealthImages = giSMZipperImages = giSMDropPackImages = -1;
	giSMStealthButton = giSMZipperButton = giSMDropPackButton = -1;
	gfUIStanceDifferent = TRUE;
	gfAllDisabled	= FALSE;

	FilenameForBPP("INTERFACE\\inventory_buttons.sti", ubString);
	
	//-----------------------
	// Load button Graphics
	iSMPanelImages[ STANCEUP_IMAGES	]			= LoadButtonImage(ubString,-1,0,-1,10,-1 );

	iSMPanelImages[ UPDOWN_IMAGES	]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,9,-1,19,-1 );
	iSMPanelImages[ CLIMB_IMAGES	]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,3,-1,13,-1 );
	iSMPanelImages[ STANCEDOWN_IMAGES	]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,8,-1,18,-1 );
	iSMPanelImages[ HANDCURSOR_IMAGES	]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,1,-1,11,-1 );
	iSMPanelImages[ PREVMERC_IMAGES	]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,20,-1,22,-1 );
	iSMPanelImages[ NEXTMERC_IMAGES	]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,21,-1,23,-1 );
	//iSMPanelImages[ BURSTMODE_IMAGES	]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,7,-1,17,-1 );
	iSMPanelImages[ LOOK_IMAGES	]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,2,-1,12,-1 );
	iSMPanelImages[ TALK_IMAGES	]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,6,-1,16,-1 );
	iSMPanelImages[ MUTE_IMAGES	]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,5,-1,15,-1 );
	
	iSMPanelImages[ OPTIONS_IMAGES	]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	] ,-1,24,-1,25,-1 );
	
	iBurstButtonImages[ WM_NORMAL ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1,  7, -1, -1, -1 );
	iBurstButtonImages[ WM_BURST ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_AUTOFIRE ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL ]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 26, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL_BURST ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL_AUTO ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_UB ]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1,  7, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_UB_BURST ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_UB_AUTO ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_BAYONET ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES	], -1, 17, -1, -1, -1 );

	FilenameForBPP("INTERFACE\\invadd-ons.sti", ubString);
	// Load button Graphics
	iSMPanelImages[ STANCE_IMAGES	]				= LoadButtonImage(ubString,0,0,-1,2,-1 );

	FilenameForBPP("INTERFACE\\inventory_buttons_2.sti", ubString);
	// Load button Graphics
	iSMPanelImages[ DONE_IMAGES	]					= LoadButtonImage(ubString,-1,1,-1,3,-1 );
	iSMPanelImages[ MAPSCREEN_IMAGES	]			= UseLoadedButtonImage( iSMPanelImages[ DONE_IMAGES	] ,-1,0,-1,2,-1 );


	// CHRISL: Needed for new inventory backpack buttons
	FilenameForBPP("INTERFACE\\backpack_buttons.sti", ubString);
	iSMPanelImages[ BACKPACK_IMAGES  ]				= LoadButtonImage(ubString,0,0,-1,2,-1 );

	// Create buttons

	// SET BUTTONS TO -1
	memset( iSMPanelButtons, -1, sizeof( iSMPanelButtons ) );
									
	iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] = QuickCreateButton( iSMPanelImages[ MAPSCREEN_IMAGES ], SM_MAPSCREEN_X, SM_MAPSCREEN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMapScreenCallback );
	SetButtonFastHelpText( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ], TacticalStr[ MAPSCREEN_POPUPTEXT ]);
	SetBtnHelpEndCallback( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ], HelpTextDoneCallback );

	iSMPanelButtons[ SM_DONE_BUTTON ] = CreateIconAndTextButton( iSMPanelImages[ DONE_IMAGES ], L"", FONT12ARIAL,
													FONT_MCOLOR_WHITE, DEFAULT_SHADOW ,
													FONT_MCOLOR_WHITE, DEFAULT_SHADOW ,
													TEXT_CJUSTIFIED,
													SM_DONE_X, SM_DONE_Y, BUTTON_TOGGLE ,MSYS_PRIORITY_HIGH-1,
													DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnSMDoneCallback );
	SetButtonFastHelpText( iSMPanelButtons[ SM_DONE_BUTTON ], TacticalStr[ END_TURN_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ SM_DONE_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ TALK_BUTTON ] = QuickCreateButton( iSMPanelImages[ TALK_IMAGES ], SM_TALKB_X, SM_TALKB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTalkCallback );
	SetButtonFastHelpText( iSMPanelButtons[ TALK_BUTTON ], TacticalStr[ TALK_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ TALK_BUTTON ], HelpTextDoneCallback );



	iSMPanelButtons[ MUTE_BUTTON ] = QuickCreateButton( iSMPanelImages[ MUTE_IMAGES ], SM_MUTEB_X, SM_MUTEB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMuteCallback );
	SetButtonFastHelpText( iSMPanelButtons[ MUTE_BUTTON ], TacticalStr[ TOGGLE_MUTE_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ MUTE_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ STANCEUP_BUTTON ] = QuickCreateButton( iSMPanelImages[ STANCEUP_IMAGES ], SM_STANCEUPB_X, SM_STANCEUPB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStanceUpCallback );
	if ( iSMPanelButtons[ STANCEUP_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ STANCEUP_BUTTON ], TacticalStr[ CHANGE_STANCE_UP_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ STANCEUP_BUTTON ], HelpTextDoneCallback );

	//SetButtonFastHelpText( iSMPanelButtons[ STANCEUP_BUTTON ],L"Change Stance Up");


	iSMPanelButtons[ UPDOWN_BUTTON ] = QuickCreateButton( iSMPanelImages[ UPDOWN_IMAGES ], SM_UPDOWNB_X, SM_UPDOWNB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnUpdownCallback );
	if ( iSMPanelButtons[ UPDOWN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ UPDOWN_BUTTON ],L"Whatever");
	SetButtonFastHelpText( iSMPanelButtons[ UPDOWN_BUTTON ], TacticalStr[ CURSOR_LEVEL_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ UPDOWN_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ CLIMB_BUTTON ] = QuickCreateButton( iSMPanelImages[ CLIMB_IMAGES ], SM_CLIMBB_X, SM_CLIMBB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnClimbCallback );
	if ( iSMPanelButtons[ CLIMB_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ CLIMB_BUTTON ],L"Climb");
	SetButtonFastHelpText( iSMPanelButtons[ CLIMB_BUTTON ], TacticalStr[ JUMPCLIMB_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ CLIMB_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ STANCEDOWN_BUTTON ] = QuickCreateButton( iSMPanelImages[ STANCEDOWN_IMAGES ], SM_STANCEDOWNB_X, SM_STANCEDOWNB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStanceDownCallback );
	if ( iSMPanelButtons[ STANCEDOWN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ STANCEDOWN_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ STANCEDOWN_BUTTON ], TacticalStr[ CHANGE_STANCE_DOWN_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ STANCEDOWN_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ HANDCURSOR_BUTTON ] = QuickCreateButton( iSMPanelImages[ HANDCURSOR_IMAGES ], SM_HANDCURSORB_X, SM_HANDCURSORB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnHandCursorCallback );
	if ( iSMPanelButtons[ HANDCURSOR_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ HANDCURSOR_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ HANDCURSOR_BUTTON ], TacticalStr[ EXAMINE_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ HANDCURSOR_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ PREVMERC_BUTTON ] = QuickCreateButton( iSMPanelImages[ PREVMERC_IMAGES ], SM_PREVMERCB_X, SM_PREVMERCB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnPrevMercCallback );
	if ( iSMPanelButtons[ PREVMERC_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ PREVMERC_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ PREVMERC_BUTTON ], TacticalStr[ PREV_MERC_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ PREVMERC_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ NEXTMERC_BUTTON ] = QuickCreateButton( iSMPanelImages[ NEXTMERC_IMAGES ], SM_NEXTMERCB_X, SM_NEXTMERCB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnNextMercCallback );
	if ( iSMPanelButtons[ NEXTMERC_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ NEXTMERC_BUTTON ], TacticalStr[ NEXT_MERC_POPUPTEXT ] );
	//SetButtonFastHelpText( iSMPanelButtons[ NEXTMERC_BUTTON ],L"Change Stance Down");
	SetBtnHelpEndCallback( iSMPanelButtons[ NEXTMERC_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ OPTIONS_BUTTON ] = QuickCreateButton( iSMPanelImages[ OPTIONS_IMAGES ], SM_OPTIONSB_X, SM_OPTIONSB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnOptionsCallback );
	if ( iSMPanelButtons[ OPTIONS_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ OPTIONS_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ OPTIONS_BUTTON ], TacticalStr[ CHANGE_OPTIONS_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ OPTIONS_BUTTON ], HelpTextDoneCallback );


	/*iSMPanelButtons[ BURSTMODE_BUTTON ] = QuickCreateButton( iSMPanelImages[ BURSTMODE_IMAGES ], SM_BURSTMODEB_X, SM_BURSTMODEB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnBurstModeCallback );*/
	iSMPanelButtons[ BURSTMODE_BUTTON ] = QuickCreateButton( iBurstButtonImages[ WM_NORMAL ], SM_BURSTMODEB_X, SM_BURSTMODEB_Y, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnBurstModeCallback );

	if ( iSMPanelButtons[ BURSTMODE_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ BURSTMODE_BUTTON ], TacticalStr[ TOGGLE_BURSTMODE_POPUPTEXT ] );
	//SetButtonFastHelpText( iSMPanelButtons[ BURSTMODE_BUTTON ],L"Change Stance Down");
	SetBtnHelpEndCallback( iSMPanelButtons[ BURSTMODE_BUTTON ], HelpTextDoneCallback );

	iSMPanelButtons[ LOOK_BUTTON ] = QuickCreateButton( iSMPanelImages[ LOOK_IMAGES ], SM_LOOKB_X, SM_LOOKB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnLookCallback );
	if ( iSMPanelButtons[ LOOK_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ LOOK_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ LOOK_BUTTON ], TacticalStr[ LOOK_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ LOOK_BUTTON ], HelpTextDoneCallback );

	return( TRUE );
}

void	RemoveSMPanelButtons( )
{
	UINT32 cnt;

	for ( cnt = 0; cnt < NUM_SM_BUTTONS; cnt++ )
	{
		if ( iSMPanelButtons[ cnt ] != -1 )
		{
			RemoveButton( iSMPanelButtons[ cnt ] );
		}
	}

	for ( cnt = 0; cnt < NUM_SM_BUTTON_IMAGES; cnt++ )
	{
		if(iSMPanelImages[ cnt ] != -1)
		{
			UnloadButtonImage( iSMPanelImages[ cnt ] );
		}
	}

	if ( giSMStealthButton != -1 )
	{
		RemoveButton( giSMStealthButton );
	}

	RenderBackpackButtons(DEACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */

	if ( giSMStealthImages != -1 )
	{
		UnloadButtonImage( giSMStealthImages );
	}
	RenderBackpackButtons(UNLOAD_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
	
	for (int i = 0; i < NUM_WEAPON_MODES; ++i)
	{
		if ( iBurstButtonImages[i] != -1)
		{
			UnloadButtonImage(iBurstButtonImages[i]);
		}
	}
}




BOOLEAN ShutdownSMPanel( )
{

	// All buttons and regions and video objects and video surfaces will be deleted at shutddown of SGM
	// We may want to delete them at the interm as well, to free up room for other panels
	DeleteVideoObjectFromIndex( guiSMPanel );
	DeleteVideoObjectFromIndex( guiSMObjects );
	DeleteVideoObjectFromIndex( guiSMObjects2 );
	DeleteVideoObjectFromIndex( guiSecItemHiddenVO );
	DeleteVideoObjectFromIndex( guiBrownBackgroundForTeamPanel );

	gubSelectSMPanelToMerc = NOBODY;

	// CJC: delete key ring if open
	DeleteKeyRingPopup(); // function will abort if key ring is not up

	// ATE: Delete desc panel if it was open....
	if ( gfInItemDescBox )
	{
		DeleteItemDescriptionBox( );
	}


	// Shotdown item slot interface
	ShutdownInvSlotInterface( );

	// shutdown keyring interface
	ShutdownKeyRingInterface( );

	ShutdownInventoryInterface();

	MSYS_RemoveRegion( &gSMPanelRegion );
	MSYS_RemoveRegion( &gSM_SELMERCPanelRegion );
	MSYS_RemoveRegion( &gSM_SELMERCBarsRegion );
	MSYS_RemoveRegion( &gSM_SELMERCMoneyRegion );
	MSYS_RemoveRegion( &gSM_SELMERCEnemyIndicatorRegion );
	MSYS_RemoveRegion( &gSM_SELMERCWeightRegion );
	MSYS_RemoveRegion( &gSM_SELMERCCamoRegion ); // added - SANDRO

	HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, FALSE );

	MSYS_RemoveRegion( &gViewportRegion );


	RemoveSMPanelButtons( );

	//CHRISL: Following line is to make sure that team panel buttons activate
	DisableTacticalTeamPanelButtons( FALSE );

	return( TRUE );
}


void RenderSMPanel( BOOLEAN *pfDirty )
{
	INT16 sFontX, sFontY;
	INT16	usX, usY;
	CHAR16 sString[9];
	UINT32	cnt;
	static CHAR16 pStr[ 200 ], pMoraleStr[ 20 ], sTemp[ 20 ];

	if ( gubSelectSMPanelToMerc != NOBODY )
	{
		// Give him the panel!
		SetSMPanelCurrentMerc( gubSelectSMPanelToMerc );
	}


	// ATE: Don't do anything if we are in stack popup and are refreshing stuff....
	if ( ( InItemStackPopup( ) || ( InKeyRingPopup( ) ) ) && (*pfDirty) == DIRTYLEVEL1 )
	{
		return;
	}

	if ( gfCheckForMouseOverItem )
	{
		if ( ( GetJA2Clock( ) - guiMouseOverItemTime ) > 100 )
		{
			if ( HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)gbCheckForMouseOverItemPos, TRUE ) )
			{
				(*pfDirty) = DIRTYLEVEL2;
			}

			gfCheckForMouseOverItem = FALSE;
		}
	}

	HandleNewlyAddedItems( gpSMCurrentMerc, pfDirty );

	if ( InItemDescriptionBox( ) )
	{
		HandleItemDescriptionBox( pfDirty );
	}

	if ( *pfDirty == DIRTYLEVEL2 )
	{
		//if ( InItemStackPopup( ) )
		//{

		//}

		if ( InItemDescriptionBox( ) )
		{
			// CHRISL: Changed 3rd parameter so we can display graphic based on inventory system used
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMPanel, UsingNewInventorySystem(), INTERFACE_START_X, INV_INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			RenderSoldierFace( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, TRUE );


			// ATE: Need these lines here to fix flash bug with face selection box
			if ( gfSMDisableForItems )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
			}
			else
			{
				if ( gusSelectedSoldier == gpSMCurrentMerc->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC(	gpSMCurrentMerc ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
				}
			}

			// HEADROCK HAM 5: Test for active Transform Popup
			if (gfItemDescTransformPopupVisible)
			{
				gItemDescTransformPopup->show();
			}
			else
			{
				RenderItemDescriptionBox( );
			}
		}
		else
		{
			// CHRISL: Changed 3rd parameter so we can display graphic based on inventory system used
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMPanel, UsingNewInventorySystem(), INTERFACE_START_X, INV_INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );

			RenderInvBodyPanel( gpSMCurrentMerc, SM_BODYINV_X, SM_BODYINV_Y );

			// CHECK FOR PANEL STUFF / DEATHS / CLOSURES
			CheckForFacePanelStartAnims( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y );

			// Hitlight

			if ( gfSMDisableForItems )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
			}
			else
			{
				if ( gusSelectedSoldier == gpSMCurrentMerc->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC(	gpSMCurrentMerc ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
				}
			}

			// HEADROCK HAM 3.6: "progress" bars showing how near the character is to "leveling up" in any stat. The bar
			// is displayed behind the current stat value, as see on the character's info panel.
			// This section draws TACTICAL info pages. Another section is in mapscreen.cpp and draws STRATEGIC info pages.
			// The feature is toggled by Options-Menu switch, and its color is determined in the INI files.
			if ( gGameSettings.fOptions[TOPTION_STAT_PROGRESS_BARS] )
			{
				UINT8	*pDestBuf;
				UINT32 uiDestPitchBYTES = 0;
				SGPRect ClipRect;
				UINT8 ubBarWidth;
				UINT16 usColor = Get16BPPColor( FROMRGB( gGameExternalOptions.ubStatProgressBarsRed, gGameExternalOptions.ubStatProgressBarsGreen, gGameExternalOptions.ubStatProgressBarsBlue ) );
				//pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
				pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );

				// AGI
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sAgilityGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sAgilityGain+1)) / SubpointsPerPoint(AGILAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_AGI_Y-1);
					ClipRect.iBottom = (SM_AGI_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_AGI_X;
					ClipRect.iRight = SM_AGI_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// DEX
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sDexterityGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sDexterityGain+1)) / SubpointsPerPoint(DEXTAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_DEX_Y-1);
					ClipRect.iBottom = (SM_DEX_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_DEX_X;
					ClipRect.iRight = SM_DEX_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// STR
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sStrengthGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sStrengthGain+1)) / SubpointsPerPoint(STRAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_STR_Y-1);
					ClipRect.iBottom = (SM_STR_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_STR_X;
					ClipRect.iRight = SM_STR_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// WIS
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sWisdomGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sWisdomGain+1)) / SubpointsPerPoint(WISDOMAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_WIS_Y-1);
					ClipRect.iBottom = (SM_WIS_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_WIS_X;
					ClipRect.iRight = SM_WIS_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// MRK
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMarksmanshipGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMarksmanshipGain+1)) / SubpointsPerPoint(MARKAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_MRKM_Y-1);
					ClipRect.iBottom = (SM_MRKM_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_MRKM_X;
					ClipRect.iRight = SM_MRKM_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// LDR
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sLeadershipGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sLeadershipGain+1)) / SubpointsPerPoint(LDRAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_CHAR_Y-1);
					ClipRect.iBottom = (SM_CHAR_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_CHAR_X;
					ClipRect.iRight = SM_CHAR_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// MECH
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMechanicGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMechanicGain+1)) / SubpointsPerPoint(MECHANAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_MECH_Y-1);
					ClipRect.iBottom = (SM_MECH_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_MECH_X;
					ClipRect.iRight = SM_MECH_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// EXPLO
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sExplosivesGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sExplosivesGain+1)) / SubpointsPerPoint(EXPLODEAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_EXPL_Y-1);
					ClipRect.iBottom = (SM_EXPL_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_EXPL_X;
					ClipRect.iRight = SM_EXPL_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// MED
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMedicalGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sMedicalGain+1)) / SubpointsPerPoint(MEDICALAMT,0);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_MED_Y-1);
					ClipRect.iBottom = (SM_MED_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_MED_X;
					ClipRect.iRight = SM_MED_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				// EXPLEVEL
				if (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sExpLevelGain)
				{
					ubBarWidth = (SM_STATS_WIDTH * (gMercProfiles[ gpSMCurrentMerc->ubProfile ].sExpLevelGain+1)) / SubpointsPerPoint(EXPERAMT, gpSMCurrentMerc->stats.bExpLevel);
					ubBarWidth = __max(0, __min(ubBarWidth, SM_STATS_WIDTH));
					ClipRect.iTop = (SM_EXPLVL_Y-1);
					ClipRect.iBottom = (SM_EXPLVL_Y-1) + SM_STATS_HEIGHT;
					ClipRect.iLeft = SM_EXPLVL_X;
					ClipRect.iRight = SM_EXPLVL_X + ubBarWidth;
					Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
				}

				UnLockVideoSurface( guiSAVEBUFFER );
			}

			// Render faceplate
			//BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 1, SM_SELMERC_NAMEPLATE_X, SM_SELMERC_NAMEPLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			//RestoreExternBackgroundRect( SM_SELMERC_NAMEPLATE_X, SM_SELMERC_NAMEPLATE_Y, SM_SELMERC_NAMEPLATE_WIDTH, SM_SELMERC_NAMEPLATE_HEIGHT );

			// Blit position
			//if(gbPixelDepth==16)
			//{
				//BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, gbSMCurStanceObj, SM_POSITIONB_X, SM_POSITIONB_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			//}
			//RestoreExternBackgroundRect( SM_POSITIONB_X, SM_POSITIONB_Y, SM_POSITIONB_WIDTH , SM_POSITIONB_HEIGHT );


			SetFont( BLOCKFONT2 );


			// Render Values for stats!
			// Set font drawing to saved buffer
			SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( STATS_TITLE_FONT_COLOR );
			for( cnt = 0; cnt < 5; cnt++ )
			{
				mprintf( INTERFACE_START_X + 92, ( INT16 )( INV_INTERFACE_START_Y + 7 + cnt * 10 ), pShortAttributeStrings[ cnt	] );
				mprintf( INTERFACE_START_X + 137, ( INT16 )( INV_INTERFACE_START_Y + 7 + cnt * 10 ), pShortAttributeStrings[ cnt + 5 ] );
			}

			mprintf( SM_ARMOR_LABEL_X - StringPixLength( pInvPanelTitleStrings[0], BLOCKFONT2 ) / 2, SM_ARMOR_LABEL_Y, pInvPanelTitleStrings[ 0 ] );

			#ifdef CHINESE
				mprintf( SM_ARMOR_PERCENT_X, SM_ARMOR_PERCENT_Y, ChineseSpecString1 );
			#else
				mprintf( SM_ARMOR_PERCENT_X, SM_ARMOR_PERCENT_Y, L"%%" );
			#endif

			mprintf( SM_WEIGHT_LABEL_X - StringPixLength( pInvPanelTitleStrings[1], BLOCKFONT2 ), SM_WEIGHT_LABEL_Y, pInvPanelTitleStrings[ 1 ] );

			#ifdef CHINESE
				mprintf( SM_WEIGHT_PERCENT_X, SM_WEIGHT_PERCENT_Y, ChineseSpecString1 );
			#else
				mprintf( SM_WEIGHT_PERCENT_X, SM_WEIGHT_PERCENT_Y, L"%%" );
			#endif
			
			mprintf( SM_CAMMO_LABEL_X - StringPixLength( pInvPanelTitleStrings[2], BLOCKFONT2 ), SM_CAMMO_LABEL_Y, pInvPanelTitleStrings[ 2 ] );

			#ifdef CHINESE
				mprintf( SM_CAMMO_PERCENT_X, SM_CAMMO_PERCENT_Y, ChineseSpecString1 );
			#else
				mprintf( SM_CAMMO_PERCENT_X, SM_CAMMO_PERCENT_Y, L"%%" );
			#endif

				UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeAgilityTime, (BOOLEAN)(gpSMCurrentMerc->usValueGoneUp & AGIL_INCREASE ? TRUE : FALSE), (BOOLEAN)((gGameOptions.fNewTraitSystem && (gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_AGILITY] > 0)) ? TRUE : FALSE), gpSMCurrentMerc->bExtraAgility != 0 ); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bAgility + gpSMCurrentMerc->bExtraAgility );
			FindFontRightCoordinates(SM_AGI_X, SM_AGI_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeDexterityTime, (BOOLEAN)(gpSMCurrentMerc->usValueGoneUp & DEX_INCREASE ? TRUE : FALSE), (BOOLEAN)((gGameOptions.fNewTraitSystem && (gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY] > 0)) ? TRUE : FALSE), gpSMCurrentMerc->bExtraDexterity != 0 ); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bDexterity + gpSMCurrentMerc->bExtraDexterity );
			FindFontRightCoordinates(SM_DEX_X, SM_DEX_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeStrengthTime, (BOOLEAN)(gpSMCurrentMerc->usValueGoneUp & STRENGTH_INCREASE ? TRUE : FALSE), (BOOLEAN)(((gGameOptions.fNewTraitSystem && (gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH] > 0)) || ( UsingFoodSystem() && gpSMCurrentMerc->usStarveDamageStrength > 0)) ? TRUE : FALSE), gpSMCurrentMerc->bExtraStrength != 0 ); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bStrength + gpSMCurrentMerc->bExtraStrength );
			FindFontRightCoordinates(SM_STR_X, SM_STR_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeLeadershipTime, ( BOOLEAN )( gpSMCurrentMerc->usValueGoneUp & LDR_INCREASE? TRUE: FALSE ), ( BOOLEAN ) ( ( gGameOptions.fNewTraitSystem && ( gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_LEADERSHIP] > 0 )) ? TRUE : FALSE), FALSE); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bLeadership );
			FindFontRightCoordinates(SM_CHAR_X, SM_CHAR_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeWisdomTime, (BOOLEAN)(gpSMCurrentMerc->usValueGoneUp & WIS_INCREASE ? TRUE : FALSE), (BOOLEAN)((gGameOptions.fNewTraitSystem && (gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_WISDOM] > 0)) ? TRUE : FALSE), gpSMCurrentMerc->bExtraWisdom != 0 ); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bWisdom + gpSMCurrentMerc->bExtraWisdom );
			FindFontRightCoordinates(SM_WIS_X, SM_WIS_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeLevelTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & LVL_INCREASE? TRUE: FALSE ),  FALSE , FALSE);

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bExpLevel + gpSMCurrentMerc->bExtraExpLevel );
			FindFontRightCoordinates(SM_EXPLVL_X, SM_EXPLVL_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeMarksmanshipTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MRK_INCREASE? TRUE: FALSE ), ( BOOLEAN ) ( ( gGameOptions.fNewTraitSystem && ( gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_MARKSMANSHIP] > 0 )) ? TRUE : FALSE), FALSE); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bMarksmanship );
			FindFontRightCoordinates(SM_MRKM_X, SM_MRKM_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeMechanicalTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MECH_INCREASE ? TRUE: FALSE ), ( BOOLEAN ) ( ( gGameOptions.fNewTraitSystem && ( gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_MECHANICAL] > 0 )) ? TRUE : FALSE), FALSE); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bMechanical );
			FindFontRightCoordinates(SM_MECH_X, SM_MECH_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeExplosivesTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & EXP_INCREASE? TRUE: FALSE ), ( BOOLEAN ) ( ( gGameOptions.fNewTraitSystem && ( gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_EXPLOSIVES] > 0 )) ? TRUE : FALSE), FALSE); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bExplosive );
			FindFontRightCoordinates(SM_EXPL_X, SM_EXPL_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			UpdateStatColor( gpSMCurrentMerc->timeChanges.uiChangeMedicalTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MED_INCREASE? TRUE: FALSE ), ( BOOLEAN ) ( ( gGameOptions.fNewTraitSystem && ( gpSMCurrentMerc->ubCriticalStatDamage[DAMAGED_STAT_MEDICAL] > 0 )) ? TRUE : FALSE), FALSE); // SANDRO

			swprintf( sString, L"%2d", gpSMCurrentMerc->stats.bMedical );
			FindFontRightCoordinates(SM_MED_X, SM_MED_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			if ( gpSMCurrentMerc->stats.bLife >= OKLIFE )
			{
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( STATS_TEXT_FONT_COLOR );
			}
			else
			{
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_MCOLOR_DKGRAY );
			}

			// Display armour value!
			swprintf( sString, L"%3d", ArmourPercent( gpSMCurrentMerc ) );
			FindFontRightCoordinates(SM_ARMOR_X, SM_ARMOR_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			// Display wieght value!
			swprintf( sString, L"%3d", CalculateCarriedWeight( gpSMCurrentMerc ) );
			FindFontRightCoordinates(SM_WEIGHT_X, SM_WEIGHT_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );

			ApplyEquipmentBonuses(gpSMCurrentMerc);

			// Display cammo value!
			swprintf( sString, L"%3d", max(0, min(max((gpSMCurrentMerc->bCamo + gpSMCurrentMerc->wornCamo), max((gpSMCurrentMerc->urbanCamo+gpSMCurrentMerc->wornUrbanCamo), max((gpSMCurrentMerc->desertCamo+gpSMCurrentMerc->wornDesertCamo), (gpSMCurrentMerc->snowCamo+gpSMCurrentMerc->wornSnowCamo)))),100)) );
			FindFontRightCoordinates(SM_CAMMO_X, SM_CAMMO_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString );
			
			// reset to frame buffer!
			SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			RestoreExternBackgroundRect( INTERFACE_START_X, INV_INTERFACE_START_Y, SCREEN_WIDTH - INTERFACE_START_X , INV_INTERFACE_HEIGHT );
			
			RenderSoldierFace( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, TRUE );
		}

		// Render Name!
		SetFont( BLOCKFONT2 );

		if ( gpSMCurrentMerc->bStealthMode )
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_LTYELLOW );
		}
		else
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_LTGRAY );
		}

		RestoreExternBackgroundRect( SM_SELMERCNAME_X, SM_SELMERCNAME_Y, SM_SELMERCNAME_WIDTH, SM_SELMERCNAME_HEIGHT );
		VarFindFontCenterCoordinates( SM_SELMERCNAME_X, SM_SELMERCNAME_Y, SM_SELMERCNAME_WIDTH, SM_SELMERCNAME_HEIGHT, SMALLFONT1, &sFontX, &sFontY, L"%s", gpSMCurrentMerc->name );
		mprintf( sFontX + 5, sFontY, L"%s", gpSMCurrentMerc->name );

	}

	if ( *pfDirty != DIRTYLEVEL0 )
	{
		///////////////////////////////////////////////////////////////////////
		// SANDRO - added a tooltip window showing exact camo percentage for every type
		// *******************************************************************
		CHAR16 pStrCamo[400];
		swprintf( pStrCamo, L"" );
		swprintf( pStr, L"");
		if ((gpSMCurrentMerc->bCamo + gpSMCurrentMerc->wornCamo) > 0 )
		{
			swprintf( pStrCamo, L"\n%d/%d%s %s", gpSMCurrentMerc->bCamo, gpSMCurrentMerc->wornCamo, L"%", gzMiscItemStatsFasthelp[ 21 ]);
			wcscat( pStr, pStrCamo);
			swprintf( pStrCamo, L"" );
		}
		if ((gpSMCurrentMerc->urbanCamo + gpSMCurrentMerc->wornUrbanCamo) > 0 )
		{
			swprintf( pStrCamo, L"\n%d/%d%s %s", gpSMCurrentMerc->urbanCamo, gpSMCurrentMerc->wornUrbanCamo, L"%", gzMiscItemStatsFasthelp[ 22 ]);
			wcscat( pStr, pStrCamo);
			swprintf( pStrCamo, L"" );
		}
		if ((gpSMCurrentMerc->desertCamo + gpSMCurrentMerc->wornDesertCamo) > 0 )
		{
			swprintf( pStrCamo, L"\n%d/%d%s %s", gpSMCurrentMerc->desertCamo, gpSMCurrentMerc->wornDesertCamo, L"%", gzMiscItemStatsFasthelp[ 23 ]);
			wcscat( pStr, pStrCamo);
			swprintf( pStrCamo, L"" );
		}
		if ((gpSMCurrentMerc->snowCamo + gpSMCurrentMerc->wornSnowCamo) > 0 )
		{
			swprintf( pStrCamo, L"\n%d/%d%s %s", gpSMCurrentMerc->snowCamo, gpSMCurrentMerc->wornSnowCamo, L"%", gzMiscItemStatsFasthelp[ 24 ] );
			wcscat( pStr, pStrCamo);
			swprintf( pStrCamo, L"" );
		}
		
		// anv: display stealth together with camo
		INT16 wornstealth = GetWornStealth(gpSMCurrentMerc) - gpSMCurrentMerc->GetBackgroundValue(BG_PERC_STEALTH);
		INT16 bonusstealth = gpSMCurrentMerc->GetBackgroundValue(BG_PERC_STEALTH);
		if ( gpSMCurrentMerc->ubBodyType == BLOODCAT )
		{
			bonusstealth += 50;
		}
		// SANDRO - new/old traits
		else if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( gpSMCurrentMerc, STEALTHY_NT ))
		{
			bonusstealth += gSkillTraitValues.ubSTBonusToMoveQuietly;
		}
		else if ( !gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( gpSMCurrentMerc, STEALTHY_OT ))
		{
			bonusstealth += 25 * NUM_SKILL_TRAITS( gpSMCurrentMerc, STEALTHY_OT );
		}

		if ( bonusstealth != 0 || wornstealth!= 0 )
		{
			CHAR16 pStrBonusStealth[400];
			CHAR16 pStrWornStealth[400];
			if( bonusstealth < 0 )
				swprintf( pStrBonusStealth, L"%d", bonusstealth );
			else
				swprintf( pStrBonusStealth, L"+%d", bonusstealth );
			if( wornstealth < 0 )
				swprintf( pStrWornStealth, L"%d", wornstealth );
			else
				swprintf( pStrWornStealth, L"+%d", wornstealth );
			swprintf( pStrCamo, L"\n%s/%s %s", pStrBonusStealth, pStrWornStealth, gzMiscItemStatsFasthelp[ 25 ] );
			wcscat( pStr, pStrCamo);
			swprintf( pStrCamo, L"" );
		}

		SetRegionFastHelpText( &(gSM_SELMERCCamoRegion), pStr );
		SetRegionHelpEndCallback( &gSM_SELMERCCamoRegion, SkiHelpTextDoneCallBack );

		// Flugente: weight help text
		FLOAT totalweight = (FLOAT)GetTotalWeight( gpSMCurrentMerc );
		swprintf( pStr, gzMiscItemStatsFasthelp[35], totalweight / 10.0 );

		SetRegionFastHelpText( &(gSM_SELMERCWeightRegion), pStr );

		/////////////////////////////////////////////////////////////////////////////////////////

		// UPdate stats!
		if ( gpSMCurrentMerc->stats.bLife != 0 )
		{
		if ( gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			swprintf( pStr, TacticalStr[ VEHICLE_VITAL_STATS_POPUPTEXT ], gpSMCurrentMerc->stats.bLife, gpSMCurrentMerc->stats.bLifeMax, gpSMCurrentMerc->bBreath, gpSMCurrentMerc->bBreathMax );
			SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), pStr );
		}
		else if ( gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_ROBOT )
		{
			swprintf( pStr, gzLateLocalizedString[ 16 ], gpSMCurrentMerc->stats.bLife, gpSMCurrentMerc->stats.bLifeMax );
			SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
		}
		else
		{
			GetMoraleString( gpSMCurrentMerc, pMoraleStr );
			// Flugente: food info if food system is active
			if ( UsingFoodSystem() && gpSMCurrentMerc->ubProfile != ROBOT && !IsVehicle(gpSMCurrentMerc) )
			{
				swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_WITH_FOOD_POPUPTEXT ], gpSMCurrentMerc->stats.bLife, gpSMCurrentMerc->stats.bLifeMax, gpSMCurrentMerc->bBreath, gpSMCurrentMerc->bBreathMax, pMoraleStr, (INT32)(100*(gpSMCurrentMerc->bDrinkLevel - FOOD_MIN) / FOOD_HALF_RANGE), L"%", (INT32)(100*(gpSMCurrentMerc->bFoodLevel - FOOD_MIN) / FOOD_HALF_RANGE), L"%" );
			}
			else
			{
				swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_POPUPTEXT ], gpSMCurrentMerc->stats.bLife, gpSMCurrentMerc->stats.bLifeMax, gpSMCurrentMerc->bBreath, gpSMCurrentMerc->bBreathMax, pMoraleStr );
			}

			{
				CHAR16	atStr[500];
				swprintf( atStr, L"" );

				gpSMCurrentMerc->PrintDiseaseDesc( atStr );

				wcscat( pStr, atStr );
			}

			SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), pStr );

			// Buggler: skills/traits tooltip on merc portrait
			// clear pStr value
			swprintf( pStr, L"");

			if (gGameOptions.fNewTraitSystem) // SANDRO - old/new traits check
			{
				UINT8 ubTempSkillArray[30];
				INT8 bNumSkillTraits = 0;

				// lets rearrange our skills to a temp array
				// we also get the number of lines (skills) to be displayed 
				for ( UINT8 ubCnt = 1; ubCnt < NUM_SKILLTRAITS_NT; ++ubCnt )
				{
					if ( ProfileHasSkillTrait( gpSMCurrentMerc->ubProfile, ubCnt ) == 2 )
					{
						ubTempSkillArray[bNumSkillTraits] = (ubCnt + NEWTRAIT_MERCSKILL_EXPERTOFFSET);
						bNumSkillTraits++;
					}
					else if ( ProfileHasSkillTrait( gpSMCurrentMerc->ubProfile, ubCnt ) == 1 )
					{
						ubTempSkillArray[bNumSkillTraits] = ubCnt;
						bNumSkillTraits++;
					}
				}

				if ( bNumSkillTraits == 0 )
				{
					swprintf( pStr, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
				}
				else
				{
					for ( UINT8 ubCnt = 0; ubCnt < bNumSkillTraits; ++ubCnt )
					{
						swprintf( sTemp, L"%s\n", gzMercSkillTextNew[ ubTempSkillArray[ubCnt] ] );
						wcscat( pStr, sTemp );
					}
				}
			}
			else
			{
				INT8 bSkill1 = 0, bSkill2 = 0; 	
				bSkill1 = gMercProfiles[ gpSMCurrentMerc->ubProfile ].bSkillTraits[0];
				bSkill2 = gMercProfiles[ gpSMCurrentMerc->ubProfile ].bSkillTraits[1];

				if ( bSkill1 == 0 && bSkill2 == 0 )
				{
					swprintf( pStr, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
				}
				else
				{
					//if the 2 skills are the same, add the '(expert)' at the end
					if( bSkill1 == bSkill2 )
					{
						swprintf( pStr, L"%s %s", gzMercSkillText[bSkill1], gzMercSkillText[EXPERT] );
					}
					else
					{
						//Display the first skill
						if( bSkill1 != 0 )
						{
							swprintf( pStr, L"%s\n", gzMercSkillText[bSkill1] );
						}
						if( bSkill2 != 0 )
						{
							swprintf( sTemp, L"%s", gzMercSkillText[bSkill2] );
							wcscat( pStr, sTemp );
						}
					}
				}
			}
			SetRegionFastHelpText( &gSM_SELMERCPanelRegion, pStr );
			}
		}
		else
		{
			SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), L"" );
			SetRegionFastHelpText( &gSM_SELMERCPanelRegion, L"" );
		}

		//if we are in the shop keeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			SetRegionHelpEndCallback( &gSM_SELMERCBarsRegion, SkiHelpTextDoneCallBack );

		// display AP
		if ( !( gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_DEAD ) )
		{
			if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) && gpSMCurrentMerc->stats.bLife >= OKLIFE )
			{
				SetFont( TINYFONT1 );
				//if ( gpSMCurrentMerc->sLastTarget != NOWHERE && !EnoughPoints( gpSMCurrentMerc, MinAPsToAttack( gpSMCurrentMerc, gpSMCurrentMerc->sLastTarget, FALSE ), 0, FALSE ) || GetUIApsToDisplay( gpSMCurrentMerc ) < 0 )
				if ( !EnoughPoints( gpSMCurrentMerc, MinAPsToAttack( gpSMCurrentMerc, gpSMCurrentMerc->sLastTarget, FALSE, 0 ), 0, FALSE ) || GetUIApsToDisplay( gpSMCurrentMerc ) < 0 )
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_DKRED );
				}
				else
				{
					if ( MercDruggedOrDrunk( gpSMCurrentMerc ) )
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						//SetFontForeground( FONT_MCOLOR_LTBLUE );

						// Flugente: new colour for being drugged, as blue on black was hard to see
						SetRGBFontForeground( 250, 5, 250 );
					}
					else if ( gpSMCurrentMerc->bStealthMode )
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						SetFontForeground( FONT_MCOLOR_LTYELLOW );
					}
					else
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						SetFontForeground( FONT_MCOLOR_LTGRAY );
					}
				}
				
				gprintfRestore( SM_SELMERC_AP_X, SM_SELMERC_AP_Y, L"%3d", GetUIApsToDisplay( gpSMCurrentMerc ) );
				VarFindFontCenterCoordinates( SM_SELMERC_AP_X, SM_SELMERC_AP_Y, SM_SELMERC_AP_WIDTH, SM_SELMERC_AP_HEIGHT, TINYFONT1, &sFontX, &sFontY, L"%d", GetUIApsToDisplay( gpSMCurrentMerc ) );
				mprintf( sFontX, SM_SELMERC_AP_Y, L"%d", GetUIApsToDisplay( gpSMCurrentMerc ) );
			}

			// Display bars
			DrawLifeUIBarEx( gpSMCurrentMerc, SM_SELMERC_HEALTH_X, SM_SELMERC_HEALTH_Y, SM_SELMERC_HEALTH_WIDTH, SM_SELMERC_HEALTH_HEIGHT, TRUE , FRAME_BUFFER );

		if ( !(gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_ROBOT ) )
		{
			DrawBreathUIBarEx( gpSMCurrentMerc, SM_SELMERC_BREATH_X, SM_SELMERC_BREATH_Y, SM_SELMERC_HEALTH_WIDTH, SM_SELMERC_HEALTH_HEIGHT, TRUE, FRAME_BUFFER );
			DrawMoraleUIBarEx( gpSMCurrentMerc, SM_SELMERC_MORALE_X, SM_SELMERC_MORALE_Y, SM_SELMERC_MORALE_WIDTH, SM_SELMERC_MORALE_HEIGHT, TRUE, FRAME_BUFFER );
		}
		}

	}

/*	I added this
 *	any questions? joker
 */
	if( fRenderRadarScreen == TRUE )
	{
		// Render clock
		// CHRISL: If we're in NIV mode, don't render the clock on the SM panel
		if(UsingNewInventorySystem() == false)
		{
			RenderClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
			CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		}
		else
		{
			RemoveMouseRegionForPauseOfClock();
		}

		// CHRISL: Change function call to include X,Y coordinates.
		RenderTownIDString( LOCATION_NAME_X, LOCATION_NAME_Y );

	}//XXXj in tactical bug is: NIV pushes town name +Y and boom
	else
	{
		RemoveMouseRegionForPauseOfClock( );
	}

	UpdateSMPanel( );


	//HandlePanelFaceAnimations( gpSMCurrentMerc );

	HandleSoldierFaceFlash( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y );

	// Render items in guy's hand!
	HandleRenderInvSlots( gpSMCurrentMerc, *pfDirty );

	if ( gfSMDisableForItems && (*pfDirty) != DIRTYLEVEL0 )
	{
		UINT8	*pDestBuf;
		UINT32 uiDestPitchBYTES;
		SGPRect ClipRect;

		// CHRISL: Change hatching area based on inventory system
		ClipRect.iLeft = ((UsingNewInventorySystem() == true)) ? 0 : 87;
		ClipRect.iRight = ((UsingNewInventorySystem() == true)) ? SCREEN_WIDTH : 536;
		//ClipRect.iLeft	 = 87;
		//ClipRect.iRight  = 536;
		ClipRect.iTop		= INV_INTERFACE_START_Y;
		ClipRect.iBottom = SCREEN_HEIGHT;
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		Blt16BPPBufferHatchRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
		UnLockVideoSurface( FRAME_BUFFER );
	}

}

void UpdateStatColor( UINT32 uiTimer, BOOLEAN fIncrease, BOOLEAN fDamaged, BOOLEAN fAugmented ) // SANDRO - added argument // Flugente - me too
{
	if ( gpSMCurrentMerc->stats.bLife >= OKLIFE )
	{
		// SANDRO - if damaged stat we could regain, show in red until repaired
		if( fDamaged )
		{
			SetFontForeground( FONT_RED );
		}
		else if( ( GetJA2Clock()	< CHANGE_STAT_RECENTLY_DURATION + uiTimer) && ( uiTimer != 0 ) )
		{
			if( fIncrease )
			{
				SetFontForeground( FONT_LTGREEN );
			}
			else
			{
				SetFontForeground( FONT_RED );
			}
		}
		else if ( fAugmented )
		{
			SetRGBFontForeground( 250, 5, 250 );
		}
		else
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( STATS_TEXT_FONT_COLOR );
		}
	}
	else
	{
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}
}



void SMInvMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	
	UINT32 uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	if ( gpSMCurrentMerc->inv[ uiHandPos ].exists() == false )
		return;

	if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{
	}
	else if (iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if ( gpItemPointer == NULL )
		{
			// Setup a timer....
			guiMouseOverItemTime = GetJA2Clock( );
			gfCheckForMouseOverItem = TRUE;
			gbCheckForMouseOverItemPos = (INT8)uiHandPos;
		}
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//gfSM_HandInvDispText[ uiHandPos ] = 1;
		if ( gpItemPointer == NULL )
		{
			HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)uiHandPos, FALSE );
			gfCheckForMouseOverItem = FALSE;
			fInterfacePanelDirty = DIRTYLEVEL2;
			gbCheckForMouseOverItemPos = NO_SLOT;
		}
	}
}

void InvPanelButtonClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}
}


void SMInvMoveCammoCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	else if (iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if (gpItemPointer == NULL)
		{
			// Setup a timer....
			guiMouseOverItemTime = GetJA2Clock();
			gfCheckForMouseOverItem = TRUE;
			gbCheckForMouseOverItemPos = NO_SLOT;
		}
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		if (gpItemPointer == NULL)
		{
			//gfSM_HandInvDispText[ uiHandPos ] = 1;
			HandleCompatibleAmmoUI(gpSMCurrentMerc, (INT8)NO_SLOT, FALSE);
			gfCheckForMouseOverItem = FALSE;
		}
	}
}


void SMInvClickCamoCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		// Apply camo ( if we have something in cursor... )
		// If we do not have an item in hand, start moving it
		if ( gpItemPointer )
		{
			// We are doing this ourselve, continue
			if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
			{
				if ( ApplyConsumable( gpSMCurrentMerc, gpItemPointer, FALSE, TRUE ) )
				{

				}
				else
				{
					// Send message
					//Heinz: 23.02.09 BUGFIX: Don't send message when SKI is on
					if( !( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANNOT_DO_INV_STUFF_STR ] );
				}

				// Check if it's the same now!
				if ( !gpItemPointer->exists( ) )
				{
					gbCompatibleApplyItem = FALSE;
					EndItemPointer( );
				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}
}


BOOLEAN HandleNailsVestFetish( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, UINT16 usReplaceItem )
{
	BOOLEAN fRefuse = FALSE;

	// OK are we nails?
	if ( pSoldier->ubProfile == 34 )
	{
		// if this the VEST POS?
		if ( uiHandPos == VESTPOS )
		{
			// Are we trying to pick it up?
			if ( usReplaceItem == NOTHING )
			{
				fRefuse = TRUE;
			}
			else
			{
				// Do we have nothing or the leather vest or kevlar leather vest?
				if ( Item[usReplaceItem].leatherjacket ||
						usReplaceItem == COMPOUND18 ||
						usReplaceItem == JAR_QUEEN_CREATURE_BLOOD )
				{
					// This is good....
					fRefuse = FALSE;
				}
				else
				{
					fRefuse = TRUE;
				}
			}

			if ( fRefuse )
			{
				// Say quote!
				TacticalCharacterDialogue( pSoldier, 61 );
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

BOOLEAN UIHandleItemPlacement( UINT8 ubHandPos, UINT16 usOldItemIndex, UINT16 usNewItemIndex, BOOLEAN fDeductPoints )
{
	// Try to place here
	if ( PlaceObject( gpSMCurrentMerc, ubHandPos, gpItemPointer ) )
	{
		if ( fDeductPoints )
		{
			// Deduct points
			if ( gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
			{
				DeductPoints( gpItemPointerSoldier,	2, 0, UNTRIGGERED_INTERRUPT );
			}
			if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
			{
				DeductPoints( gpSMCurrentMerc,	2, 0, UNTRIGGERED_INTERRUPT );
			}
		}

		HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, ubHandPos, usOldItemIndex, usNewItemIndex );

		// Dirty
		fInterfacePanelDirty = DIRTYLEVEL2;

		// Check if cursor is empty now
		if ( gpItemPointer->exists() == false )
		{
			EndItemPointer( );
		}

		if ( gpItemPointerSoldier != gpSMCurrentMerc )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_ITEM_PASSED_TO_MERC ], ShortItemNames[ usNewItemIndex ], gpSMCurrentMerc->name );
		}

		// UPDATE ITEM POINTER.....
		gpItemPointerSoldier = gpSMCurrentMerc;

		if ( gpItemPointer != NULL )
		{
			ReevaluateItemHatches( gpSMCurrentMerc, FALSE );
		}

		// Set cursor back to normal mode...
		guiPendingOverrideEvent = A_CHANGE_TO_MOVE;

		return( TRUE );

	}
	else
	{
		return( FALSE );
	}

}

//Jenilee
INT32 iLastHandPos = -1;

#define SLOT_NONE		0
#define SLOT_HANDS		1
#define SLOT_EQUIPMENT	2
#define SLOT_VEST		3
#define SLOT_RIG		4
#define SLOT_CPACK		5
#define SLOT_BPACK		6
#define SLOT_SLING		7
#define SLOT_KNIFE		8
#define SLOT_FACE		9

STR16 uiSlotTypeName[] = 
{
	L"",				//0
	L"Hands",			//1
	L"Equipment",		//2
	L"Vest",			//3
	L"Leg Rig",			//4
	L"Combat Pack",		//5
	L"Backpack",		//6
	L"Sling",			//7
	L"Knife",			//8
	L"Face",			//9
	L"Large Pocket",	//10
	L"Small Pocket"		//11
};

INT16 uiNIVSlotType[NUM_INV_SLOTS] = { 
						2, 2, 2, //0-2
						9, 9, //3-4
						1, 1, //5-6
						2, 2, 2, 2, 2,  //7-11
						7, //12
						8, //13
						5, 5, 5, //14-16
						6, 6, 6, 6, //17-20
						3, 3, //21-22
						4, 4, //23-24
						3, 3, 3, 3, 3, //25-29
						3, 3, 3, 3, 3, //30-34
						4, 4, 4, 4, //35-38
						4, 4, 4, 4, //39-42
						5, 5, 5, 5, //43-46
						6, 6, 6, 6, //47-50
						6, 6, 6, 6  //51-54
					};

UINT16 uiOIVSlotType[NUM_INV_SLOTS] = {
						2, 2, 2, //0-2
						9, 9, //3-4
						1, 1, //5-6
						0, 0, 0, 0, 0, 0, 0, //7-13
						10, 10, 10, 10,	//14-17
						0, 0, 0, 0, 0, 0, 0, //18-24
						11, 11, 11, 11, 11, 11, 11, 11, //25-32
						0, 0, 0, 0, 0, 0, 0, 0,	//33-40
						0, 0, 0, 0, 0, 0, 0, 0,	//41-48
						0, 0, 0, 0, 0, 0	//49-54
};

UINT16 GetInvMovementCost(OBJECTTYPE* pObj, INT16 old_pos, INT16 new_pos)
{
	if (!(gTacticalStatus.uiFlags & INCOMBAT) || //Not in combat
		(old_pos == -1 || new_pos == -1)||	//Either position is invalid
		(old_pos == new_pos))				//Old position same as new position
		return 0;
	INT16 src_type;
	INT16 dst_type;
	if (UsingNewInventorySystem() == TRUE) 
	{
		src_type = uiNIVSlotType[old_pos];
		dst_type = uiNIVSlotType[new_pos];
	}
	else
	{
		src_type = uiOIVSlotType[old_pos];
		dst_type = uiOIVSlotType[new_pos];
	}
	if (src_type == dst_type)	//Moving to same lbe type
		return 0;

	//If these 2 arrays are initiated outside the function, APBPConstants will not have been initialized, and all values will be 0
	INT16 uiAPCostFromSlot[12] =
	{
		APBPConstants[AP_INV_FROM_NONE],
		APBPConstants[AP_INV_FROM_HANDS],
		APBPConstants[AP_INV_FROM_EQUIPMENT],
		APBPConstants[AP_INV_FROM_VEST],
		APBPConstants[AP_INV_FROM_RIG],
		APBPConstants[AP_INV_FROM_CPACK],
		APBPConstants[AP_INV_FROM_BPACK],
		APBPConstants[AP_INV_FROM_SLING],
		APBPConstants[AP_INV_FROM_KNIFE],
		APBPConstants[AP_INV_FROM_FACE],
		APBPConstants[AP_INV_FROM_BIG_POCKET],
		APBPConstants[AP_INV_FROM_SMALL_POCKET]
	};
	INT16 uiAPCostToSlot[12] =
	{
		APBPConstants[AP_INV_TO_NONE],
		APBPConstants[AP_INV_TO_HANDS],
		APBPConstants[AP_INV_TO_EQUIPMENT],
		APBPConstants[AP_INV_TO_VEST],
		APBPConstants[AP_INV_TO_RIG],
		APBPConstants[AP_INV_TO_CPACK],
		APBPConstants[AP_INV_TO_BPACK],
		APBPConstants[AP_INV_TO_SLING],
		APBPConstants[AP_INV_TO_KNIFE],
		APBPConstants[AP_INV_TO_FACE],
		APBPConstants[AP_INV_TO_BIG_POCKET],
		APBPConstants[AP_INV_TO_SMALL_POCKET]
	};
	FLOAT Weight_Divisor = gGameExternalOptions.uWeightDivisor;
	UINT16 weight_modifier;
	if (Weight_Divisor != 0)
		weight_modifier = DynamicAdjustAPConstants((int)((Item[pObj->usItem].ubWeight) / Weight_Divisor),(int)((Item[pObj->usItem].ubWeight) / Weight_Divisor));
	else
		weight_modifier = 0;

	INT32 cost = 0; 

	cost += uiAPCostFromSlot[src_type];
	cost += uiAPCostToSlot[dst_type];
	cost += weight_modifier;

	// Flugente if we move an item from our hands to the sling, and item has a weapon sling, don't charge any APs
	if ( 1 == src_type && 7 == dst_type )
	{
		// if item has a weapon sling attached
		if ( HasAttachmentOfClass(pObj, AC_SLING) )
			// we simply let go of the item, we can do that because the sling will catch it
			cost = 0;
	}

	if (cost > APBPConstants[AP_INV_MAX_COST]) 
		cost = APBPConstants[AP_INV_MAX_COST];

#ifdef _DEBUG
	CHAR16 szTemp[255];
	swprintf(szTemp, L"Moving item from %s to %s (costs: %d/%d/%d/%d)", uiSlotTypeName[src_type], uiSlotTypeName[dst_type], uiAPCostFromSlot[src_type], uiAPCostToSlot[dst_type], weight_modifier, cost);
	ScreenMsg(FONT_MCOLOR_LTGREEN, MSG_CHAT, szTemp);
#endif
	return cost;
}

void SMInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32 uiHandPos;
	// Copies of values
	UINT16 usOldItemIndex, usNewItemIndex;
	UINT16 usItemPrevInItemPointer;
	UINT16 usCostToMoveItem = 0; //Jenilee
	BOOLEAN fNewItem = FALSE;
	static BOOLEAN	fRightDown = FALSE;
	static BOOLEAN	fLeftDown = FALSE;
	BOOLEAN INV_AP_COST = UsingInventoryCostsAPSystem();

	uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		// and this inventory slot is hatched out
		if( ShouldSoldierDisplayHatchOnItem( gpSMCurrentMerc->ubProfile, (INT16)uiHandPos ) )
		{
			// it means that item is a copy of one in the player's offer area, so we treat it as if the slot was empty (ignore)
			// if the cursor has an item in it, we still ignore the click, because handling swaps in this situation would be
			// ugly, we'd have to the the swap, then make the bOwnerSlot of the item just picked up a -1 in its offer area spot.
			return;
		}
	}


	//if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	//{
	//	fLeftDown = TRUE;
	//}
	//else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLeftDown )
	// CHRISL: Are we in combat, wearing a backpack with the zipper closed?  Don't allow access to backpack items
	if((UsingNewInventorySystem() == true))
		if(icLBE[uiHandPos] == BPACKPOCKPOS && (!(gpSMCurrentMerc->flags.ZipperFlag) || (gpSMCurrentMerc->flags.ZipperFlag && gAnimControl[gpSMCurrentMerc->usAnimState].ubEndHeight == ANIM_STAND)) && (gTacticalStatus.uiFlags & INCOMBAT) && (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ))
			iReason = MSYS_CALLBACK_REASON_NONE;
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLeftDown = FALSE;

		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{

			// Return if empty
			if ( gpSMCurrentMerc->inv[ uiHandPos ].exists() == false )
				return;

			if ( gpSMCurrentMerc->ubID != gusSelectedSoldier )
			{
				SelectSoldier( gpSMCurrentMerc->ubID, FALSE, FALSE );
			}

			// OK, check if this is Nails, and we're in the vest position , don't allow it to come off....
			if ( HandleNailsVestFetish( gpSMCurrentMerc, uiHandPos, NOTHING ) )
			{
				return;
			}

			if ( _KeyDown(CTRL) )
			{
				CleanUpStack( &( gpSMCurrentMerc->inv[ uiHandPos ] ), NULL );
				return;
			}
			
			if((UsingNewInventorySystem() == true))
			{
				/*if we pick up a backpack without reactivating the drop pack button, and we have a
				dropkey, reactivate the button*/
				if(uiHandPos == BPACKPOCKPOS)
				{
					// Deal with the zipper before we do anything
					if(gpSMCurrentMerc->flags.ZipperFlag)
						if(!ChangeZipperStatus(gpSMCurrentMerc, FALSE))
							return;
					// Do we still have a linked backpack?  If so, reset droppackflag
					for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
					{
						if(gWorldItems[wi].soldierID == gpSMCurrentMerc->ubID && gWorldItems[wi].object.exists() == true)
						{
							gpSMCurrentMerc->flags.DropPackFlag = TRUE;
							break;
						}
					}
					RenderBackpackButtons(ACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
				}
			}

			// Turn off new item glow!
			gpSMCurrentMerc->inv.bNewItemCount[ uiHandPos ] = 0;

			usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;

			//Jenilee: remember our last selected slot
			iLastHandPos = uiHandPos;


			// move item into the mouse cursor
			BeginItemPointer( gpSMCurrentMerc, (UINT8)uiHandPos );

			//if we are in the shopkeeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				// pick up item from regular inventory slot into cursor OR try to sell it ( unless CTRL is held down )
				BeginSkiItemPointer( PLAYERS_INVENTORY, ( INT8 ) uiHandPos, ( BOOLEAN )!gfKeyState[ CTRL ] );
			}

			HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, uiHandPos, usOldItemIndex, NOTHING );

			// HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)uiHandPos, FALSE );
		}
		else	// item in cursor
		{
			UINT8			ubSrcID, ubDestID;
			BOOLEAN		fOKToGo = FALSE;
			BOOLEAN		fDeductPoints = FALSE;

			// ATE: OK, get source, dest guy if different... check for and then charge appropriate APs
			ubSrcID	= gpSMCurrentMerc->ubID;
			ubDestID = gpItemPointerSoldier->ubID;

			if ( ubSrcID == ubDestID )
			{
				if(!CanItemFitInPosition(gpSMCurrentMerc, gpItemPointer, (INT8)uiHandPos, FALSE))//dnl ch66 070913
					return;
				if (INV_AP_COST)
					//Jenilee: determine the cost of moving this item around in our inventory
					usCostToMoveItem = GetInvMovementCost(gpItemPointer, iLastHandPos, uiHandPos);
				
				// Flugente: backgrounds
				usCostToMoveItem = (usCostToMoveItem * (100 + gpSMCurrentMerc->GetBackgroundValue(BG_INVENTORY))) / 100;
				
				if ( ( usCostToMoveItem == 0 ) || ( gpSMCurrentMerc->bActionPoints >= usCostToMoveItem ) )
				{
					fOKToGo = TRUE;
					//iLastHandPos = uiHandPos;//dnl ch66 070913 this should be set after we move item
				}
				else //we dont have enough APs to move it to this slot, show a warning message
				{
					// silversurfer: What if our old slot is occupied now (could happen when we swap items)?
					// We will be stuck with an item at the hand cursor and nowhere to put it -> bad. :-(
					// So let's check if our old slot is empty and if it is not allow item placement anyway.
					if ( gpSMCurrentMerc->inv[ iLastHandPos ].usItem == NULL )
					{
						ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[NOT_ENOUGH_APS_STR]);
						fOKToGo = FALSE;
					}
					else
						fOKToGo = TRUE;
				}

				// We are doing this ourselve, continue
//				fOKToGo = TRUE;
			}
			else
			{
				// These guys are different....
				fDeductPoints = TRUE;

				// First check points for src guy
				if ( gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
				{
					if ( EnoughPoints( gpItemPointerSoldier, 3, 0, TRUE ) )
					{
						fOKToGo = TRUE;
					}
				}
				else
				{
					fOKToGo = TRUE;
				}

				// Should we go on?
				if ( fOKToGo )
				{
					if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
					{
						if ( EnoughPoints( gpSMCurrentMerc, 3, 0, TRUE ) )
						{
							fOKToGo = TRUE;
						}
						else
						{
							fOKToGo = FALSE;
						}
					}
				}
			}

			if ( fOKToGo )
			{
				usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
				usNewItemIndex = gpItemPointer->usItem;

				// OK, check if this is Nails, and we're in the vest position , don't allow it to come off....
				if ( HandleNailsVestFetish( gpSMCurrentMerc, uiHandPos, usNewItemIndex ) )
				{
					return;
				}

				if ( _KeyDown(CTRL) )
				{
					if ( gpItemPointer->exists() && gpSMCurrentMerc->inv[ uiHandPos ].exists() )
					{
						if ( gpItemPointer->usItem == gpSMCurrentMerc->inv[ uiHandPos ].usItem )
							CleanUpStack( &( gpSMCurrentMerc->inv[ uiHandPos ] ), gpItemPointer );
						else // Madd: attach / merge object, merge only works on single objects for now
						{
							UINT8 cnt = gpSMCurrentMerc->inv[ uiHandPos ].ubNumberOfObjects;
							if ( gpItemPointer->ubNumberOfObjects < cnt ) 
								cnt = gpItemPointer->ubNumberOfObjects;

							for (UINT8 i = 0; i<cnt;i++)
							{
								if ((gTacticalStatus.uiFlags & INCOMBAT))
								{
									// silversurfer: This didn't cost any AP. Why? CTRL + LeftClick should deduct the same AP as manual attachment in the EDB.
									usCostToMoveItem = AttachmentAPCost(gpItemPointer->usItem, gpSMCurrentMerc->inv[uiHandPos].usItem, gpSMCurrentMerc);
									// Flugente: backgrounds
									usCostToMoveItem = (usCostToMoveItem * (100 + gpSMCurrentMerc->GetBackgroundValue(BG_INVENTORY))) / 100;
									// do we have enough AP?
									if (!EnoughPoints(gpSMCurrentMerc, usCostToMoveItem, 0, TRUE))
										return;
									gpSMCurrentMerc->bActionPoints -= usCostToMoveItem;
								}
								gpSMCurrentMerc->inv[uiHandPos].AttachObject(gpSMCurrentMerc, gpItemPointer, TRUE, i);
							}
						}
					}

					if ( gpItemPointer->exists() == false )
					{
						EndItemPointer( );
					}
					return;
				}

				// hold ALT key to swap valid attachment item instead
				if ( _KeyDown(ALT) )
				{
					// do nothing
				}
				// we allow attaching on items in any slot
				else if ( ValidAttachment( usNewItemIndex, &(gpSMCurrentMerc->inv[uiHandPos]) ) )
				{
					// it's an attempt to attach; bring up the inventory panel
					if ( !InItemDescriptionBox( ) )
					{
						InitItemDescriptionBox( gpSMCurrentMerc, (UINT8)uiHandPos, ITEMDESC_START_X, ITEMDESC_START_Y, 0 );
					}
					return;
				}

				if ( uiHandPos == HANDPOS || uiHandPos == SECONDHANDPOS || uiHandPos == HELMETPOS || uiHandPos == VESTPOS || uiHandPos == LEGPOS )
				{
					if ( ValidMerge( usNewItemIndex, usOldItemIndex ) )
					{
						// bring up merge requestor
						gubHandPos = (UINT8) uiHandPos;
						gusOldItemIndex = usOldItemIndex;
						gusNewItemIndex = usNewItemIndex;
						gfDeductPoints = fDeductPoints;

						if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
						{
							//the only way to merge items is to pick them up.	In SKI when you pick up an item, the cursor is
							//locked in a region, free it up.
							FreeMouseCursor( TRUE );

							DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_MERGE_ITEMS ], SHOPKEEPER_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, MergeMessageBoxCallBack, NULL );
						}
						else
							DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_MERGE_ITEMS ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, MergeMessageBoxCallBack, NULL );
						return;
					}
					// else handle normally
				}


				/* CHRISL: For New Inventory system.  Are we swapping LBE Items?  We'll need to move any
				items in the associated IC group pockets into a new LBENODE.  We'll have to check
				whether we already have an LBENODE for this type of LBE item and, if so, remove
				the items from it, place them in the LBE Items current sector, and delete the existing
				LBENODE.  Then we need to know if the LBE Item in the cursor is an LBENODE
				or just a normal OBJECTTYPE.  If it's an LBENODE, we need to move it's items into
				the appropriate pockets for the soldier and then delete the LBENODE.*/
				if((UsingNewInventorySystem() == true))
				{
					//If we put a new pack in the backpack pocket, turn off the droppack button
					if(uiHandPos == BPACKPOCKPOS && CanItemFitInPosition(gpSMCurrentMerc, gpItemPointer, uiHandPos, FALSE))
					{
						// First, deal with the zipper
						if(gpSMCurrentMerc->flags.ZipperFlag)
							if(!ChangeZipperStatus(gpSMCurrentMerc, FALSE))
								return;
						if(gpSMCurrentMerc->flags.DropPackFlag)
							gpSMCurrentMerc->flags.DropPackFlag = FALSE;
						RenderBackpackButtons(ACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
					}
				}

				// remember the item type currently in the item pointer
				usItemPrevInItemPointer = gpItemPointer->usItem;

				if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
				{
					// If it's just been purchased or repaired, mark it as a "new item"
					fNewItem = ( BOOLEAN ) ( gMoveingItem.uiFlags & ( ARMS_INV_JUST_PURCHASED | ARMS_INV_ITEM_REPAIRED ) );
				}

				// try to place the item in the cursor into this inventory slot
				if ( UIHandleItemPlacement( (UINT8) uiHandPos, usOldItemIndex, usNewItemIndex, fDeductPoints ) )
				{
					iLastHandPos = uiHandPos;//dnl ch66 070913
					//Jenilee: pay the price
					//just make sure to handle that if we are putting it back in the SAME slot, the cost should be 0!!!
					gpSMCurrentMerc->bActionPoints -= usCostToMoveItem;

					RenderBackpackButtons(ACTIVATE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */
					// it worked!	if we're in the SKI...
					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						SetNewItem( gpSMCurrentMerc, ( UINT8 ) uiHandPos, fNewItem );

						// and the cursor is now empty
						if( gpItemPointer == NULL )
						{
							// clean up
							gMoveingItem.initialize();
							SetSkiCursor( CURSOR_NORMAL );
						}
						else
						{
							// if we're holding something else in the pointer now
							if ( usItemPrevInItemPointer != gpItemPointer->usItem )
							{
								// pick up item swapped out of inventory slot into cursor (don't try to sell)
								BeginSkiItemPointer( PLAYERS_INVENTORY, -1, FALSE );
							}
							else
							{
								// otherwise, leave the cursor as is, means more items were picked up at once than can be placed in this slot
								// we deal with this by leaving the remainder in the cursor, to be put down elsewhere using subsequent clicks
							}
						}
					}

					// Setup a timer....
					//guiMouseOverItemTime = GetJA2Clock( );
					//gfCheckForMouseOverItem = TRUE;
					//gbCheckForMouseOverItemPos = (INT8)uiHandPos;
				}

				/*
				// Try to place here
				if ( PlaceObject( gpSMCurrentMerc, (UINT8)uiHandPos, gpItemPointer ) )
				{

					if ( fDeductPoints )
					{
						// Deduct points
						if ( gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpItemPointerSoldier,	2, 0 );
						}
						if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpSMCurrentMerc,	2, 0 );
						}
					}

					HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, uiHandPos, usOldItemIndex, usNewItemIndex );

					// Dirty
					fInterfacePanelDirty = DIRTYLEVEL2;

					// Check if it's the same now!
					if ( gpItemPointer->exists() == false )
					{
						EndItemPointer( );
					}

					// Setup a timer....
					guiMouseOverItemTime = GetJA2Clock( );
					gfCheckForMouseOverItem = TRUE;
					gbCheckForMouseOverItemPos = (INT8)uiHandPos;

				}
				*/
			}
		}

		// Flugente: we have to recheck our flashlights, as we changed items
		//gpSMCurrentMerc->usSoldierFlagMask |= SOLDIER_REDOFLASHLIGHT;
		gpSMCurrentMerc->HandleFlashLights();

		// sevenfm: update morale, as we could add/remove walkman
		RefreshSoldierMorale(gpSMCurrentMerc);
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		// Return if empty
		if ( gpSMCurrentMerc->inv[ uiHandPos ].exists() == false )
			return;

		// CJC: OK, get source, dest guy if different, don't allow panels to be brought up
		/*
		if ( gpItemPointer && gpSMCurrentMerc->ubID != gpItemPointerSoldier->ubID )
		{
			return;
		}
		*/

		// Turn off new item glow!
		gpSMCurrentMerc->inv.bNewItemCount[ uiHandPos ] = 0;

		// Some global stuff here - for esc, etc
		// Check for # of slots in item
		// CHRISL: Use new ItemSlotLimit function if we're using the new inventory system
		UINT8 isLimit = ItemSlotLimit( &gpSMCurrentMerc->inv[ uiHandPos ], uiHandPos, gpSMCurrentMerc );

		// access description box directly if CTRL is pressed for stack items
		if( !( ( gpSMCurrentMerc->inv[ uiHandPos ].ubNumberOfObjects > 1 && isLimit > 0 ) && ( guiCurrentScreen != MAP_SCREEN ) ) || _KeyDown( CTRL ) )
		{
			if ( !InItemDescriptionBox( ) )
			{
				if ( _KeyDown(SHIFT) && gpItemPointer == NULL && Item[gpSMCurrentMerc->inv[ uiHandPos ].usItem].usItemClass == IC_GUN && (gpSMCurrentMerc->inv[ uiHandPos ])[uiHandPos]->data.gun.ubGunShotsLeft > 0 && !(Item[gpSMCurrentMerc->inv[ uiHandPos ].usItem].singleshotrocketlauncher) && !( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
				{
					EmptyWeaponMagazine( &(gpSMCurrentMerc->inv[ uiHandPos ]), &gItemPointer, uiHandPos );
					gpItemPointer = &gItemPointer;
					gpItemPointerSoldier = gpSMCurrentMerc;
				}
				else
					InitItemDescriptionBox( gpSMCurrentMerc, (UINT8)uiHandPos, ITEMDESC_START_X, ITEMDESC_START_Y, 0 );
			}
		}
		else
		{
			if ( !InItemStackPopup( )	)
			{
				//CHRISL: Set popup width based on pocket size
				INT16	invWidth;
				if(uiHandPos >= (UINT32)BIGPOCKSTART && uiHandPos < (UINT32)BIGPOCKFINAL)
					invWidth = 531;
				else if(uiHandPos >= (UINT32)MEDPOCKSTART && uiHandPos < (UINT32)MEDPOCKFINAL)
					invWidth = 412;
				else
					invWidth = 314;

				//InitItemStackPopup( gpSMCurrentMerc, (UINT8)uiHandPos, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, SM_ITEMDESC_WIDTH, SM_ITEMDESC_HEIGHT );
				//CHRISL: In OIV mode, we don't want to offset the keyring popup.
				UINT8 inv_interface_start_x;
				if(UsingNewInventorySystem() == false)
					inv_interface_start_x = 213;
				else
					inv_interface_start_x = 248;
				InitItemStackPopup( gpSMCurrentMerc, (UINT8)uiHandPos, inv_interface_start_x + xResOffset, INV_INTERFACE_START_Y, invWidth, ( SCREEN_HEIGHT - INV_INTERFACE_START_Y ) );

			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fRightDown = FALSE;
		fLeftDown = FALSE;
	}

}
// CHRISL: Function to adjust zipper backpack button status
BOOLEAN  ChangeZipperStatus(SOLDIERTYPE *pSoldier, BOOLEAN newStatus)
{
	INT16	sAPCost;
	INT32	iBPCost = APBPConstants[BP_WORK_ZIPPER];
	INT8	bNewStance;

	//Set AP cost based on what we're doing
	sAPCost = (newStatus) ? APBPConstants[AP_CLOSE_ZIPPER] : APBPConstants[AP_OPEN_ZIPPER];
		// SANDRO - ambidextrous bonus to handle things faster
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) )
		sAPCost = (INT16)(sAPCost * (100 - gSkillTraitValues.ubAMWorkBackpackAPsReduction) / 100.0f + 0.5f);

	//Are we currently in combat?
	if(gTacticalStatus.uiFlags & INCOMBAT)
	{
		// If we have fewer base points then the default cost to unzip a pack, use all remaining points instead
		if(pSoldier->bInitialActionPoints < sAPCost)
			sAPCost = pSoldier->bInitialActionPoints;
		if(EnoughPoints(pSoldier, sAPCost, iBPCost, TRUE))
			DeductPoints(pSoldier, sAPCost, iBPCost);
		else
			return FALSE;
	}

	//Opening a pack?
	if(newStatus)
	{
		// Are we standing?
		bNewStance = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;
		if(bNewStance == ANIM_STAND)
		{
			bNewStance = ANIM_CROUCH;
			UIHandleSoldierStanceChange( pSoldier->ubID, bNewStance );
		}
		pSoldier->flags.ZipperFlag = newStatus;
		gfUIStanceDifferent = TRUE;
	}
	// Closing a pack?
	else
	{
		pSoldier->flags.ZipperFlag = newStatus;
		gfUIStanceDifferent = TRUE;
	}
	fCharacterInfoPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;

	return TRUE;
}

// CHRISL: Function to adjust droppack packpack button status
BOOLEAN ChangeDropPackStatus(SOLDIERTYPE *pSoldier, BOOLEAN newStatus)
{
	INT16	sAPCost = APBPConstants[AP_BACK_PACK];
	INT32	iBPCost = APBPConstants[BP_BACK_PACK];
	INT32	worldKey=1, iRange=0;

	// Are we dropping a pack that has the zipper open?
	if(newStatus && pSoldier->flags.ZipperFlag)
	{
		sAPCost = 0;
		if(!ChangeZipperStatus(pSoldier, FALSE))
			return FALSE;
	}
	
	// Are we currently in combat?
	if((gTacticalStatus.uiFlags & INCOMBAT) || (gTacticalStatus.fEnemyInSector))
	{
		// If we're standing over the backpack that we're trying to pick up, reset the ap cost to 0
		if(!newStatus) {
			for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
			{
				if(gWorldItems[wi].soldierID == pSoldier->ubID && gWorldItems[wi].object.exists() == true)
				{
					for (unsigned int x = 0; x < gWorldItems[wi].object.ubNumberOfObjects; ++x) {
						// Failsafe in case our LBEArray data ever goes missing.
						if(x >= LBEArray.size()){
							break;
						}
						if(gWorldItems[wi].object.IsActiveLBE(x)) {
							LBENODE* pLBE = gWorldItems[wi].object.GetLBEPointer(x);
							if(pLBE && pLBE->lbeIndex != NONE) {
								// Found an associated backpack so figure out how far that pack is from us
								iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, gWorldItems[wi].sGridNo );
								if(gWorldItems[wi].sGridNo == pSoldier->sGridNo)	// standing on pack - pickup regardless
								{
									// Buggler: pickup should cost the same as manually picking it up
									//sAPCost = 0;
									break;
								}
								else if(iRange < 26 && !(gTacticalStatus.uiFlags & INCOMBAT))	// should mean anything within 2 diagonal tiles while not in combat
								{
									// just break because we want to allow this with no changes
									break;
								}
								// If not, we can't pick up the item
								else
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_CAN_NOT_PICKUP] );
									return FALSE;
								}
							}
						}
					}
					break;
				}
			}
		}
		// Do we have enough APs to complete this action?
		if(EnoughPoints(pSoldier, sAPCost, iBPCost, TRUE))
			DeductPoints(pSoldier, sAPCost, iBPCost);
		else
			return FALSE;
	}

	// Dropping a pack?
	if(newStatus)
	{
		// We need to clear any existing WorldItems that are linked with this soldier
		for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
		{
			if(gWorldItems[wi].soldierID == pSoldier->ubID)
			{
				gWorldItems[wi].soldierID = -1;
			}
		}
		MoveItemToLBEItem( pSoldier, BPACKPOCKPOS );
		AddItemToPoolAndGetIndex(pSoldier->sGridNo, &pSoldier->inv[BPACKPOCKPOS], 1, pSoldier->pathing.bLevel, 0 , -1, pSoldier->ubID, &worldKey );
		// Item successfully added to world
		if(worldKey != ITEM_NOT_FOUND)
		{
			NotifySoldiersToLookforItems( );
			DeleteObj(&pSoldier->inv[BPACKPOCKPOS]);
			pSoldier->flags.DropPackFlag = newStatus;
			gfUIStanceDifferent = TRUE;
		}
	}
	// Picking up a pack?
	else
	{
		for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
		{
			//CHRISL: There's the remote chance that a non-LBE item might get associated with a merc.  If that happens, we can
			//	have a CTD, so lets resolve that here.
			if(Item[gWorldItems[wi].object.usItem].usItemClass != IC_LBEGEAR)
				gWorldItems[wi].soldierID = -1;
			if(gWorldItems[wi].soldierID == pSoldier->ubID && gWorldItems[wi].fExists == TRUE && Item[gWorldItems[wi].object.usItem].usItemClass == IC_LBEGEAR && LoadBearingEquipment[Item[gWorldItems[wi].object.usItem].ubClassIndex].lbeClass == BACKPACK)
			{
				for (int x = 0; x < gWorldItems[wi].object.ubNumberOfObjects; ++x) {
					// Is the item we dropped in this sector and does it have an active LBENODE flag?
					//if(gWorldItems[wi].object.IsActiveLBE(x)) {
						// Is the LBENODE we're trying to pick up actually in use?
						//if(gWorldItems[wi].object.GetLBEPointer(x)->lbeIndex != NONE)
						{
							// Try to pickup the LBENODE
							if(AutoPlaceObject(pSoldier, &(gWorldItems[wi].object ), TRUE ))
							{
								RemoveItemFromPool(gWorldItems[wi].sGridNo, wi, gWorldItems[wi].ubLevel);
								pSoldier->flags.DropPackFlag = newStatus;
								gfUIStanceDifferent = TRUE;
								return TRUE;
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_DROP] );
								return TRUE;
							}
						}
					//}
				}
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_PACK] );
				break;
			}
		}
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_PACK] );
	}

	return TRUE;
}


void MergeMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		//ADB see what happens here
		DebugBreakpoint();
		gpSMCurrentMerc->inv[ gubHandPos ].AttachObject( gpItemPointerSoldier, gpItemPointer, FALSE );

		// re-evaluate repairs
		gfReEvaluateEveryonesNothingToDo = TRUE;

		if (gpItemPointer->exists() == false)
		{
			// merge item consumed
			EndItemPointer();
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
	}
	else
	{
		UIHandleItemPlacement( gubHandPos, gusOldItemIndex, gusNewItemIndex, gfDeductPoints );
	}
}

void HandleMouseOverSoldierFaceForContMove( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	FACETYPE *pFace;
	INT32 sGridNo = NOWHERE;

	if ( pSoldier == NULL )
	{
		return;
	}

	if ( fOn )
	{
		// Check if we are waiting to continue move...
		if ( CheckForMercContMove( pSoldier ) )
		{
			// Display 'cont' on face....
			// Get face
			pFace = &gFacesData[ pSoldier->iFaceIndex ];

			pFace->fDisplayTextOver = FACE_DRAW_TEXT_OVER;
			wcscpy( pFace->zDisplayText, TacticalStr[ CONTINUE_OVER_FACE_STR ] );

			sGridNo = pSoldier->pathing.sFinalDestination;

			if ( pSoldier->bGoodContPath )
			{
				sGridNo = pSoldier->sContPathLocation;
			}

			// While our mouse is here, draw a path!
			PlotPath( pSoldier, sGridNo, NO_COPYROUTE, PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
		}
	}
	else
	{
		// Remove 'cont' on face....
		// Get face
		pFace = &gFacesData[ pSoldier->iFaceIndex ];

		pFace->fDisplayTextOver = FACE_ERASE_TEXT_OVER;

		// Erase path!
		ErasePath( TRUE );
	}

	fInterfacePanelDirty = DIRTYLEVEL2;
}

void SelectedMercButtonMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, TRUE );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, FALSE );
	}

}




void SelectedMercButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{

	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// ATE: Don't if this guy can't....
		if ( !gfSMDisableForItems )
		{
			if ( CheckForMercContMove( gpSMCurrentMerc ) )
			{
				// Continue
				ContinueMercMovement( gpSMCurrentMerc );
				ErasePath( TRUE );
			}
			else
			{
				HandleLocateSelectMerc( gpSMCurrentMerc->ubID, 0 );
			}
		}
	}
	//CHRISL: Change this to UP so that we don't inadvertantly switch to action mode when closing Inv panel
	//else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// ATE: Cannot get out by right clicking...
		//if ( gpItemPointer == NULL )
		{
			//if ( !gfSMDisableForItems || ( gfSMDisableForItems && gpItemPointer == NULL ) )
			{
				// Delete desc
				if ( InItemDescriptionBox( ) )
				{
					DeleteItemDescriptionBox( );
				}

				gfSwitchPanel = TRUE;
				gbNewPanel		= TEAM_PANEL;
			}
		}
	}
}


void SelectedMercEnemyIndicatorCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// ATE: Don't if this guy can't....
		if ( !gfSMDisableForItems )
		{
			//if ( gpSMCurrentMerc->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			//{
			//}
			//else
			{
				if ( gpSMCurrentMerc->aiData.bOppCnt > 0 )
				{
					CycleVisibleEnemies( gpSMCurrentMerc );
				}
				else
				{
					SelectedMercButtonCallback( pRegion, iReason );
				}
			}
		}
	}
}



void BtnStanceUpCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8 bNewStance;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		bNewStance = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;

		if ( bNewStance == ANIM_CROUCH )
		{
			bNewStance = ANIM_STAND;
		}
		else if ( bNewStance == ANIM_PRONE )
		{
			bNewStance = ANIM_CROUCH;
		}

		UIHandleSoldierStanceChange( gpSMCurrentMerc->ubID, bNewStance );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{

		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}


// CHRISL: Callback functions for backpack buttons
void BtnDropPackCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		//CHRISL: Added "SHIFT-LMB" option to drop packs for all members of current squad
		if ( _KeyDown( SHIFT ) )
		{
			INT8 bAssignment = gpSMCurrentMerc->bAssignment;
			for( int x = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; x <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; x++ )
			{
				/* Is DropPackFlag currently false and is there something in the backpack pocket?  If so, we haven't
				dropped a pack yet and apparently want to*/
				if(MercPtrs[x]->bAssignment == bAssignment && MercPtrs[x]->inv[BPACKPOCKPOS].exists() == true && !MercPtrs[x]->flags.DropPackFlag)
				{
					ChangeDropPackStatus(MercPtrs[x], TRUE);
				}
				/* Is DropPackFlag currently true, is nothing in the backpack pocket and have we dropped a pack?  If so, we
				must want to retreive a backpack we previously dropped.*/
				else if(MercPtrs[x]->bAssignment == bAssignment && MercPtrs[x]->inv[BPACKPOCKPOS].exists() == false && MercPtrs[x]->flags.DropPackFlag)
				{
					ChangeDropPackStatus(MercPtrs[x], FALSE);
				}
			}
		}
		else
		{
			/* Is DropPackFlag currently false and is there something in the backpack pocket?  If so, we haven't
			dropped a pack yet and apparently want to*/
			if(gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true && !gpSMCurrentMerc->flags.DropPackFlag)
			{
				ChangeDropPackStatus(gpSMCurrentMerc, TRUE);
			}
			/* Is DropPackFlag currently true, is nothing in the backpack pocket and have we dropped a pack?  If so, we
			must want to retreive a backpack we previously dropped.*/
			else if(gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == false && gpSMCurrentMerc->flags.DropPackFlag)
			{
				ChangeDropPackStatus(gpSMCurrentMerc, FALSE);
			}
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void BtnZipperCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		//Are we in combat, do we have a backpack on and is the pack closed? Open it
		if((gTacticalStatus.uiFlags & INCOMBAT) && gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true && !gpSMCurrentMerc->flags.ZipperFlag)
		{
			ChangeZipperStatus(gpSMCurrentMerc, TRUE);
		}
		//Is the pack open?
		else if(gpSMCurrentMerc->flags.ZipperFlag)
		{
			ChangeZipperStatus(gpSMCurrentMerc, FALSE);
		}
		//Are we not in combat?
		else if(!(gTacticalStatus.uiFlags & INCOMBAT))
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_ZIPPER_COMBAT] );
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void BtnUpdownCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//gsInterfaceLevel = gpSMCurrentMerc->bUIInterfaceLevel;

		// Change interface level via HandleUI handler
		UIHandleChangeLevel( NULL );

		// Remember soldier's new value
		gpSMCurrentMerc->bUIInterfaceLevel = (INT8)gsInterfaceLevel;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnClimbCallback(GUI_BUTTON *btn,INT32 reason)
{
	BOOLEAN						fNearHeigherLevel;
	BOOLEAN						fNearLowerLevel;
	INT8							bDirection;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		GetMercClimbDirection( gpSMCurrentMerc->ubID, &fNearLowerLevel, &fNearHeigherLevel );

		if ( fNearLowerLevel )
		{
			if ((UsingNewInventorySystem() == true) && gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true
				//JMich.BackpackClimb
				&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)gpSMCurrentMerc->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
				&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
			{
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB]);
				return;
			}

			gpSMCurrentMerc->BeginSoldierClimbDownRoof();
		}

		if ( fNearHeigherLevel )
		{
			if ((UsingNewInventorySystem() == true) && gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true
				//JMich.BackpackClimb
				&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)gpSMCurrentMerc->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
				&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
			{
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB]);
				return;
			}

			gpSMCurrentMerc->BeginSoldierClimbUpRoof();
		}
		
		//---------------Legion by JAzz-----------
		
		if (gGameExternalOptions.fCanClimbOnWalls == TRUE)
		{
			if ((UsingNewInventorySystem() == true) && gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true
				//JMich.BackpackClimb
				&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)gpSMCurrentMerc->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
				&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
			{
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB]);
				return;
			}

			if ( FindWallJumpDirection( gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->ubDirection, &bDirection ) )
			{
				gpSMCurrentMerc->BeginSoldierClimbWall();
			}
		}
		
		//------------------------------------------

		if ( FindFenceJumpDirection( gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->ubDirection, &bDirection ) )
		{
			if ((UsingNewInventorySystem() == true) && gpSMCurrentMerc->inv[BPACKPOCKPOS].exists() == true
				//JMich.BackpackClimb
				&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)gpSMCurrentMerc->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
				&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[gpSMCurrentMerc->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
			{
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB]);
				return;
			}

			gpSMCurrentMerc->BeginSoldierClimbFence();
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnStanceDownCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8 bNewStance;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		bNewStance = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;

		if ( bNewStance == ANIM_STAND )
		{
			bNewStance = ANIM_CROUCH;
		}
		else if ( bNewStance == ANIM_CROUCH )
		{
			bNewStance = ANIM_PRONE;
		}

		UIHandleSoldierStanceChange( gpSMCurrentMerc->ubID, bNewStance );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnStealthModeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gpSMCurrentMerc->bStealthMode = !	gpSMCurrentMerc->bStealthMode;
		gfUIStanceDifferent		= TRUE;
		gfPlotNewMovement			= TRUE;
		fInterfacePanelDirty	= DIRTYLEVEL2;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}


void BtnHandCursorCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ToggleHandCursorMode( &guiCurrentEvent );
	}

}

void BtnTalkCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
			ToggleTalkCursorMode( &guiCurrentEvent );
	}

}

void BtnMuteCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( gpSMCurrentMerc->flags.uiStatusFlags & SOLDIER_MUTE )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ MUTE_OFF_STR ], gpSMCurrentMerc->name );
			gpSMCurrentMerc->flags.uiStatusFlags &= ( ~SOLDIER_MUTE );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ MUTE_ON_STR ], gpSMCurrentMerc->name );
			gpSMCurrentMerc->flags.uiStatusFlags |= ( SOLDIER_MUTE );
		}
	}

}


void BtnPrevMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT16 sID;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		sID = FindPrevActiveAndAliveMerc( gpSMCurrentMerc, TRUE, TRUE );

		gubSelectSMPanelToMerc = (UINT8)sID;

		if ( !gfInItemPickupMenu )
		{
			//if we are in the shop keeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				LocateSoldier( sID, DONTSETLOCATOR );
				// refresh background for player slots (in case item values change due to Flo's discount)
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
			else
				LocateSoldier( sID, SETLOCATOR );
		}

		//if the user is in the shop keeper interface and is in the item desc
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			if ( InItemDescriptionBox( ) )
			{
				DeleteItemDescriptionBox( );
			}
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnNextMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT16 sID;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		sID = FindNextActiveAndAliveMerc( gpSMCurrentMerc, TRUE, TRUE );

		// Give him the panel!
		gubSelectSMPanelToMerc = (UINT8)sID;

		if ( !gfInItemPickupMenu )
		{
			//if we are in the shop keeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				LocateSoldier( sID, DONTSETLOCATOR );
				// refresh background for player slots (in case item values change due to Flo's discount)
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
			else
				LocateSoldier( sID, SETLOCATOR );
		}

		//if the user is in the shop keeper interface and is in the item desc
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			if ( InItemDescriptionBox( ) )
			{
				DeleteItemDescriptionBox( );
			}
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}


void BtnOptionsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		guiPreviousOptionScreen = guiCurrentScreen;
		LeaveTacticalScreen( OPTIONS_SCREEN );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnSMDoneCallback(GUI_BUTTON *btn,INT32 reason)
{

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gfBeginEndTurn = TRUE;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}


void BtnMapScreenCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// Enter mapscreen...
		//gfEnteringMapScreen = TRUE;
		GoToMapScreenFromTactical();
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

/*
uiTempVObject[0] = LoadButtonImage( "Interface\\InventoryButtons.sti", -1, 7, -1, -1, -1 );
uiTempVObject[1] = UseLoadedButtonImage( uiTempVObject[0], -1, 17, -1, -1, -1 );
uiTempVObject[2] = UseLoadedButtonImage( uiTempVObject[0], -1, 26, -1, -1, -1 );
iButtonID = QuickCreateButton( uiTempVObject[0], 0, 0, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, MSYS_NO_CALLBACK, CycleCallback );

void CycleCallback( GUI_BUTTON *btn, INT32 reason )
{
	static INT32 buttonValue = 0;
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		buttonValue += buttonValue < 2 ? 1 : -2;
		btn->ImageNum = uiTempVObject[ buttonValue ];
		btn->uiFlags |= BUTTON_DIRTY;
	}
}
*/

void BtnBurstModeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ChangeWeaponMode( gpSMCurrentMerc );
//		btn->ImageNum = iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ];
//		btn->uiFlags |= BUTTON_DIRTY;

	}

}

void BtnLookCallback(GUI_BUTTON *btn,INT32 reason)
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		ToggleLookCursorMode( NULL );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnPositionShowCallback(GUI_BUTTON *btn,INT32 reason)
{

}

BOOLEAN InitializeTEAMPanelCoords( )
{
	TM_FACE_WIDTH		= 48;
	TM_FACE_HEIGHT		= 43;
	TM_APPANEL_HEIGHT	= 56;
	TM_APPANEL_WIDTH	= 16;

	TM_ENDTURN_X				=	xResOffset + (xResSize - 131);
	TM_ROSTERMODE_X				=	xResOffset + (xResSize - 131);
	TM_DISK_X					=	xResOffset + (xResSize - 131);
	// CHRISL: New definitions for the team panel clock and location coordinates
	INTERFACE_CLOCK_TM_X	= xResOffset + (xResSize - 86 );
	INTERFACE_CLOCK_TM_Y	= ( 96	+ INTERFACE_START_Y );
	LOCATION_NAME_TM_X		= xResOffset + (xResSize - 92 );
	LOCATION_NAME_TM_Y		= ( 65	+ INTERFACE_START_Y	);
		
	TM_ENDTURN_Y		= ( 9 + INTERFACE_START_Y );
	TM_ROSTERMODE_Y	= ( 45 + INTERFACE_START_Y );
	TM_DISK_Y			= ( 81 + INTERFACE_START_Y );

	TM_NAME_WIDTH		= 60;
	TM_NAME_HEIGHT		= 9;
	TM_LIFEBAR_WIDTH	= 3;
	TM_LIFEBAR_HEIGHT	= 42;
	TM_FACEHIGHTL_WIDTH = 84;
	TM_FACEHIGHTL_HEIGHT = 114;
	TM_AP_HEIGHT		= 10;
	TM_AP_WIDTH		= 15;

	TM_INV_WIDTH		= 58;
	TM_INV_HEIGHT		= 23;
	TM_INV_HAND1STARTX = ( 8 + INTERFACE_START_X );
	TM_INV_HAND1STARTY = ( 67 + INTERFACE_START_Y );
	TM_INV_HAND2STARTX = ( 8 + INTERFACE_START_X );
	TM_INV_HAND2STARTY = ( 93 + INTERFACE_START_Y );
	TM_INV_HAND_SEP	= 83;
	TM_INV_HAND_SEPY	= 24;

	TM_BARS_REGION_HEIGHT	= 47;
	TM_BARS_REGION_WIDTH	= 26;

	INDICATOR_BOX_WIDTH	= 12;
	INDICATOR_BOX_HEIGHT = 10;

	for (int idx = 0; idx < TeamInterfaceMax/2; idx+=1) {
		const int evenIdx = idx * 2;
		Assert(evenIdx < TeamInterfaceMax);
		const int oddIdx = idx*2+1;
		Assert(oddIdx < TeamInterfaceMax);
		sTEAMAPPanelXY[evenIdx] = ( 69+idx*83	+ INTERFACE_START_X );		sTEAMAPPanelXY[oddIdx] = ( 6 + INTERFACE_START_Y );
		sTEAMFacesXY[evenIdx] = ( 13+idx*83+(idx>0) + INTERFACE_START_X );	sTEAMFacesXY[oddIdx] = ( 6 + INTERFACE_START_Y );
		sTEAMNamesXY[evenIdx] = ( 7+idx*83 + INTERFACE_START_X );			sTEAMNamesXY[oddIdx] = ( 55 + INTERFACE_START_Y );
		sTEAMFaceHighlXY[evenIdx] = ( 4+idx*83 + INTERFACE_START_X );		sTEAMFaceHighlXY[oddIdx] = ( 2 + INTERFACE_START_Y );
		sTEAMLifeXY[evenIdx] = ( 69+idx*83 + INTERFACE_START_X );			sTEAMLifeXY[oddIdx] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
		sTEAMBreathXY[evenIdx] = ( 75+idx*83 + INTERFACE_START_X );			sTEAMBreathXY[oddIdx] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
		sTEAMMoraleXY[evenIdx] = ( 81+idx*83 + INTERFACE_START_X );			sTEAMMoraleXY[oddIdx] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
		sTEAMApXY[evenIdx] = ( 68+idx*83 + INTERFACE_START_X );				sTEAMApXY[oddIdx] = ( 52 + INTERFACE_START_Y );
		sTEAMBarsXY[evenIdx] = ( 61+idx*83+(idx>0) + INTERFACE_START_X );	sTEAMBarsXY[oddIdx] = ( 3 + INTERFACE_START_Y );
		sTEAMHandInvXY[evenIdx] = ( TM_INV_HAND1STARTX + ( idx * TM_INV_HAND_SEP ));	sTEAMHandInvXY[oddIdx] = TM_INV_HAND1STARTY;
	}

/*
	sTEAMAPPanelXY[0] = ( 69	+ INTERFACE_START_X );	sTEAMAPPanelXY[1] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[2] = ( 151 + INTERFACE_START_X );	sTEAMAPPanelXY[3] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[4] = ( 234 + INTERFACE_START_X );	sTEAMAPPanelXY[5] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[6] = ( 317 + INTERFACE_START_X );	sTEAMAPPanelXY[7] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[8] = ( 401 + INTERFACE_START_X );	sTEAMAPPanelXY[9] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[10] = ( 484 + INTERFACE_START_X );	sTEAMAPPanelXY[11] = ( 6 + INTERFACE_START_Y );

	sTEAMFacesXY[0] = ( 13 + INTERFACE_START_X );	sTEAMFacesXY[1] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[2] = ( 97 + INTERFACE_START_X );	sTEAMFacesXY[3] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[4] = ( 180 + INTERFACE_START_X );	sTEAMFacesXY[5] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[6] = ( 263 + INTERFACE_START_X );	sTEAMFacesXY[7] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[8] = ( 346 + INTERFACE_START_X );	sTEAMFacesXY[9] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[10] = ( 429 + INTERFACE_START_X );	sTEAMFacesXY[11] = ( 6 + INTERFACE_START_Y );

	sTEAMNamesXY[0] = ( 7 + INTERFACE_START_X );	sTEAMNamesXY[1] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[2] = ( 90 + INTERFACE_START_X );	sTEAMNamesXY[3] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[4] = ( 173 + INTERFACE_START_X );	sTEAMNamesXY[5] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[6] = ( 256 + INTERFACE_START_X );	sTEAMNamesXY[7] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[8] = ( 339 + INTERFACE_START_X );	sTEAMNamesXY[9] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[10] = ( 422 + INTERFACE_START_X );	sTEAMNamesXY[11] = ( 55 + INTERFACE_START_Y );

	sTEAMFaceHighlXY[0] = ( 4 + INTERFACE_START_X );	sTEAMFaceHighlXY[1] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[2] = ( 87 + INTERFACE_START_X );	sTEAMFaceHighlXY[3] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[4] = ( 170 + INTERFACE_START_X );	sTEAMFaceHighlXY[5] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[6] = ( 253 + INTERFACE_START_X );	sTEAMFaceHighlXY[7] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[8] = ( 336 + INTERFACE_START_X );	sTEAMFaceHighlXY[9] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[10] = ( 419 + INTERFACE_START_X );	sTEAMFaceHighlXY[11] = ( 2 + INTERFACE_START_Y );

	sTEAMLifeXY[0] = ( 69 + INTERFACE_START_X );	sTEAMLifeXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[2] = ( 152 + INTERFACE_START_X );	sTEAMLifeXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[4] = ( 235 + INTERFACE_START_X );	sTEAMLifeXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[6] = ( 318 + INTERFACE_START_X );	sTEAMLifeXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[8] = ( 401 + INTERFACE_START_X );	sTEAMLifeXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[10] = ( 484 + INTERFACE_START_X );	sTEAMLifeXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMBreathXY[0] = ( 75 + INTERFACE_START_X );	sTEAMBreathXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[2] = ( 158 + INTERFACE_START_X );	sTEAMBreathXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[4] = ( 241 + INTERFACE_START_X );	sTEAMBreathXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[6] = ( 324 + INTERFACE_START_X );	sTEAMBreathXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[8] = ( 407 + INTERFACE_START_X );	sTEAMBreathXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[10] = ( 490 + INTERFACE_START_X );sTEAMBreathXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMMoraleXY[0] = ( 81 + INTERFACE_START_X );	sTEAMMoraleXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[2] = ( 164 + INTERFACE_START_X );	sTEAMMoraleXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[4] = ( 247 + INTERFACE_START_X );	sTEAMMoraleXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[6] = ( 330 + INTERFACE_START_X );	sTEAMMoraleXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[8] = ( 413 + INTERFACE_START_X );	sTEAMMoraleXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[10] = ( 496 + INTERFACE_START_X );sTEAMMoraleXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMApXY[0] = ( 68 + INTERFACE_START_X );	sTEAMApXY[1] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[2] = ( 152 + INTERFACE_START_X );	sTEAMApXY[3] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[4] = ( 233 + INTERFACE_START_X );	sTEAMApXY[5] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[6] = ( 317 + INTERFACE_START_X );	sTEAMApXY[7] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[8] = ( 400 + INTERFACE_START_X );	sTEAMApXY[9] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[10] = ( 483 + INTERFACE_START_X );sTEAMApXY[11] = ( 52 + INTERFACE_START_Y );

	sTEAMBarsXY[0] = ( 61 + INTERFACE_START_X );	sTEAMBarsXY[1] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[2] = ( 145 + INTERFACE_START_X );	sTEAMBarsXY[3] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[4] = ( 228 + INTERFACE_START_X );	sTEAMBarsXY[5] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[6] = ( 311 + INTERFACE_START_X );	sTEAMBarsXY[7] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[8] = ( 394 + INTERFACE_START_X );	sTEAMBarsXY[9] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[10] = ( 477 + INTERFACE_START_X );	sTEAMBarsXY[11] = ( 3 + INTERFACE_START_Y );

	sTEAMHandInvXY[0] = ( TM_INV_HAND1STARTX + ( 0 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[1] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[2] = ( TM_INV_HAND1STARTX + ( 1 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[3] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[4] = ( TM_INV_HAND1STARTX + ( 2 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[5] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[6] = ( TM_INV_HAND1STARTX + ( 3 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[7] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[8] = ( TM_INV_HAND1STARTX + ( 4 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[9] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[10] = ( TM_INV_HAND1STARTX + ( 5 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[11] = TM_INV_HAND1STARTY;
	// ufff to much copy&paste :D
*/

	return ( TRUE );
}
// TEAM PANEL!!!!!!!!!!!!!!
BOOLEAN InitializeTEAMPanel(	)
{
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC	VObjectDesc;
	UINT32					cnt, posIndex;
	static BOOLEAN	fFirstTime = TRUE;

	/* CHRISL: Add something here to reset the cursor to a pointer so we don't inadvertantly activate
	context sensitive cursor when closing the inventory panel*/
	fDisplayOverheadMap = TRUE;

	// WANNE: Make a black background color for the whole screen
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );


/*	OK i need to initialize coords here
 *	Isnt it cool
 *	any questions? joker
 *	i moved itrk

 */
//	InitializeTEAMPanelCoords( );
	// INit viewport region
	// Set global mouse regions
	// Define region for viewport
	MSYS_DefineRegion( &gViewportRegion, 0, 0 ,gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y, MSYS_PRIORITY_NORMAL, VIDEO_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gViewportRegion );

	// Load interface panels
	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	//SQUAD10 FIX:  Use panels with more slots if SquadSize at current resolution is > 6

	if (iResolution >= _640x480 && iResolution < _800x600)
		FilenameForBPP("INTERFACE\\bottom_bar.sti", VObjectDesc.ImageFile);
	else if (iResolution < _1024x768)
	{
		if (gGameOptions.ubSquadSize > 6)
		{
			FilenameForBPP("INTERFACE\\bottom_bar_8_800x600.sti", VObjectDesc.ImageFile);
		}
		else
		{
			FilenameForBPP("INTERFACE\\bottom_bar_800x600.sti", VObjectDesc.ImageFile);
		}
	}
	else
	{
		if (gGameOptions.ubSquadSize > 8)
		{
			FilenameForBPP("INTERFACE\\bottom_bar_10_1024x768.sti", VObjectDesc.ImageFile);
		}
		else if (gGameOptions.ubSquadSize > 6)
		{
			FilenameForBPP("INTERFACE\\bottom_bar_8_1024x768.sti", VObjectDesc.ImageFile);
		}
		else
		{
			FilenameForBPP("INTERFACE\\bottom_bar_1024x768.sti", VObjectDesc.ImageFile);
		}
	}

	CHECKF( AddVideoObject( &VObjectDesc, &guiTEAMPanel ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\gold_front.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiTEAMObjects ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Bars.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );

	// Clear inv display stuff
	memset( gfTEAM_HandInvDispText, 0, sizeof( gfTEAM_HandInvDispText ) );


	// Create buttons
	CHECKF( CreateTEAMPanelButtons( ) );

	// Set viewports
	// Define region for panel
	MSYS_DefineRegion( &gTEAM_PanelRegion, 0, gsVIEWPORT_END_Y ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	// Add region
	MSYS_AddRegion( &gTEAM_PanelRegion);

	for ( posIndex = 0, cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++, posIndex +=2 )
	{
		MSYS_DefineRegion( &gTEAM_FaceRegions[ cnt ], sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] ,(INT16)(sTEAMFacesXY[ posIndex ] + TM_FACE_WIDTH ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACE_HEIGHT), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MercFacePanelMoveCallback, MercFacePanelCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_FaceRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_FaceRegions[ cnt ], 0, cnt );


		MSYS_DefineRegion( &gTEAM_EnemyIndicator[ cnt ], (INT16)(sTEAMFacesXY[ posIndex ] + 1), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + 1 ) ,(INT16)(sTEAMFacesXY[ posIndex ] + INDICATOR_BOX_WIDTH ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + INDICATOR_BOX_HEIGHT), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, EnemyIndicatorClickCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_EnemyIndicator[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_EnemyIndicator[ cnt ], 0, cnt );



		if ( IsMouseInRegion( &gTEAM_FaceRegions[ cnt ] ) )
		{
			if ( gTeamPanel[ cnt ].ubID != NOBODY )
			{
				HandleMouseOverSoldierFaceForContMove( MercPtrs[ gTeamPanel[ cnt ].ubID ], TRUE );
			}
		}


		MSYS_DefineRegion( &gTEAM_BarsRegions[ cnt ], sTEAMBarsXY[ posIndex ], sTEAMBarsXY[ posIndex + 1 ] ,(INT16)(sTEAMBarsXY[ posIndex ] + TM_BARS_REGION_WIDTH ), (INT16)(sTEAMBarsXY[ posIndex + 1 ] + TM_BARS_REGION_HEIGHT ), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MercFacePanelCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_BarsRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_BarsRegions[ cnt ], 0, cnt );

		MSYS_DefineRegion( &gTEAM_LeftBarsRegions[ cnt ], (INT16)(sTEAMFacesXY[ posIndex ] - 8 ), sTEAMFacesXY[ posIndex + 1 ] ,(INT16)(sTEAMFacesXY[ posIndex ] ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_BARS_REGION_HEIGHT ), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MercFacePanelCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_LeftBarsRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_LeftBarsRegions[ cnt ], 0, cnt );


		MSYS_DefineRegion( &gTEAM_FirstHandInv[ cnt ], sTEAMHandInvXY[ posIndex ], sTEAMHandInvXY[ posIndex + 1 ], (INT16)(sTEAMHandInvXY[ posIndex ] + TM_INV_WIDTH ) ,(INT16)(sTEAMHandInvXY[ posIndex + 1 ] + TM_INV_HEIGHT ), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, TMClickFirstHandInvCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_FirstHandInv[ cnt ] );

		// Add user data
		MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ cnt ], 0, cnt );



		MSYS_DefineRegion( &gTEAM_SecondHandInv[ cnt ], sTEAMHandInvXY[ posIndex ], (INT16)( sTEAMHandInvXY[ posIndex + 1 ] + 24 ), (INT16)(sTEAMHandInvXY[ posIndex ] + TM_INV_WIDTH) ,(INT16)(sTEAMHandInvXY[ posIndex + 1 ] + TM_INV_HEIGHT + 24 ), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, TMClickSecondHandInvCallback );
		// Add region
		MSYS_AddRegion( &gTEAM_SecondHandInv[ cnt ] );

		// Add user data
		MSYS_SetRegionUserData( &gTEAM_SecondHandInv[ cnt ], 0, cnt );

	}


	//DEfine region for selected guy panel
	//MSYS_DefineRegion( &gSM_SELMERCPanalRegion, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, SM_SELMERC_FACE_X + SM_SELMERC_FACE_WIDTH, SM_SELMERC_FACE_Y + SM_SELMERC_FACE_HEIGHT, MSYS_PRIORITY_NORMAL,
	//					CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectedMercButtonCallback );
	// Add region
	//MSYS_AddRegion( &gSM_SELMERCPanalRegion );

	return( TRUE );
}

BOOLEAN ShutdownTEAMPanel( )
{
	UINT32					cnt;

	// All buttons and regions and video objects and video surfaces will be deleted at shutddown of SGM
	// We may want to delete them at the interm as well, to free up room for other panels
	// Delete video objects!
	DeleteVideoObjectFromIndex( guiTEAMPanel );
	DeleteVideoObjectFromIndex( guiTEAMObjects );
	DeleteVideoObjectFromIndex( guiBrownBackgroundForTeamPanel );


	MSYS_RemoveRegion( &gTEAM_PanelRegion);

	// Remove viewport
	MSYS_RemoveRegion( &gViewportRegion );

	for ( cnt = 0; cnt < NUMBER_OF_SOLDIERS_PER_SQUAD; cnt++ )
	{
		MSYS_RemoveRegion( &gTEAM_EnemyIndicator[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_FaceRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_BarsRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_LeftBarsRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_FirstHandInv[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_SecondHandInv[ cnt ] );

		if ( gTeamPanel[ cnt ].ubID != NOBODY )
		{
			HandleMouseOverSoldierFaceForContMove( MercPtrs[ gTeamPanel[ cnt ].ubID ], FALSE );
		}

	}

	// Remove buttons
	RemoveTEAMPanelButtons( );

	// turn off the display of the squad list
	if( fRenderRadarScreen == FALSE )
	{
		// start rendering radar region again,
	fRenderRadarScreen = TRUE;

		// remove squad panel
		CreateDestroyMouseRegionsForSquadList( );
	}
	return( TRUE );
}


void RenderTEAMPanel( BOOLEAN fDirty )
{
	INT16 sFontX, sFontY;
	UINT32				cnt, posIndex;
	SOLDIERTYPE		*pSoldier;
	static				CHAR16		pStr[ 512 ], pMoraleStr[ 20 ], sTemp[ 20 ];

	if ( fDirty == DIRTYLEVEL2 )
	{
		MarkAButtonDirty( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
		MarkAButtonDirty( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
		MarkAButtonDirty( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );


		// Blit video surface
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMPanel, 0, INTERFACE_START_X, INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		RestoreExternBackgroundRect( INTERFACE_START_X, INTERFACE_START_Y, SCREEN_WIDTH - INTERFACE_START_X , INTERFACE_HEIGHT );

		// LOOP THROUGH ALL MERCS ON TEAM PANEL
		for ( cnt = 0, posIndex = 0; cnt < gGameOptions.ubSquadSize; cnt++, posIndex+= 2 )
		{
			// GET SOLDIER
			if ( !gTeamPanel[ cnt ].fOccupied )
			{
				//BLIT CLOSE PANEL
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiCLOSE, 5, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ],	VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ], TM_FACE_WIDTH, TM_FACE_HEIGHT );

				// BLIT AP CLOSE PANEL
				//BltVideoObjectFromIndex( FRAME_BUFFER, guiTEAMObjects, 3, sTEAMAPPanelXY[ posIndex ], sTEAMAPPanelXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
				//RestoreExternBackgroundRect( (INT16)(sTEAMAPPanelXY[ posIndex ]), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ]), TM_APPANEL_WIDTH, TM_APPANEL_HEIGHT );
				//InvalidateRegion( (INT16)(sTEAMAPPanelXY[ posIndex ]), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ]), (INT16)(sTEAMAPPanelXY[ posIndex ] + TM_APPANEL_WIDTH), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ] + TM_APPANEL_HEIGHT ) );

				if ( gTacticalStatus.ubCurrentTeam != OUR_TEAM || INTERRUPT_QUEUED )
				{
					// Hatch out...
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );
				}

			}
			else
			{
				pSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

				if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER ) )
				{
					// Get soldier pointer for vehicle.....
					SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );

					// WDS 07/02/3008 - Fix bug of getting into vehicle you don't control
					Assert (pVehicle != 0);

					//OK, for each item, set dirty text if applicable!
					swprintf( pStr, TacticalStr[ DRIVER_POPUPTEXT ], pVehicle->stats.bLife, pVehicle->stats.bLifeMax, pVehicle->bBreath, pVehicle->bBreathMax );
					SetRegionFastHelpText( &(gTEAM_FirstHandInv[ cnt ]), pStr );
				}
				// Add text for first hand popup
				else
				{
					GetHelpTextForItem( pStr, &( pSoldier->inv[ HANDPOS ] ), pSoldier );

					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_FirstHandInv[ cnt ]), pStr );
				}

				// Add text for seonc hand popup
				if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
				{
					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_SecondHandInv[ cnt ]), TacticalStr[ EXIT_VEHICLE_POPUPTEXT ] );
				}
				else
				{
					GetHelpTextForItem( pStr, &( pSoldier->inv[ SECONDHANDPOS ]	), pSoldier );

					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_SecondHandInv[ cnt ]), pStr );
				}

				// Render Selected guy if selected
				if ( gusSelectedSoldier == pSoldier->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC( pSoldier ) )
				{
					//if(gbPixelDepth==16)
					//{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 0, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
						RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );

						// Render faceplate
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X, sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y, VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( (INT16)(sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y ), TM_FACEPLATE_WIDTH, TM_FACEPLATE_HEIGHT );
					//}
				}
				else if ( gTacticalStatus.ubCurrentTeam != OUR_TEAM || !OK_INTERRUPT_MERC( pSoldier ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );
						// Render non-selection
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 4, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );

						// Render faceplate
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X, sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y, VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( (INT16)(sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y ), TM_FACEPLATE_WIDTH, TM_FACEPLATE_HEIGHT );


				}

				CheckForFacePanelStartAnims( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] );

				RenderSoldierFace( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ], TRUE );

				// Restore AP/LIFE POSIITONS

				// Render name!
				SetFont( BLOCKFONT2 );

				if ( pSoldier->bStealthMode )
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_LTYELLOW );
				}
				else
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_LTGRAY );
				}

				// RENDER ON SAVE BUFFER!
				SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				VarFindFontCenterCoordinates( (INT16)(sTEAMNamesXY[ posIndex ] + 2 ), (INT16)(sTEAMNamesXY[ posIndex + 1 ] ), TM_NAME_WIDTH, TM_NAME_HEIGHT, BLOCKFONT2, &sFontX, &sFontY, L"%s", pSoldier->name );
				mprintf( sFontX, sFontY, L"%s", pSoldier->name );
				gprintfRestore( sFontX, sFontY, L"%s", pSoldier->name );
				// reset to frame buffer!
				SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			}
		}

	}

	// Loop through all mercs and make go
	for ( cnt = 0, posIndex = 0; cnt < gGameOptions.ubSquadSize; cnt++, posIndex+= 2 )
	{
		// GET SOLDIER
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			pSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			// Update animations....
			if ( pSoldier->flags.fClosePanel || pSoldier->flags.fClosePanelToDie )
			{
				pSoldier->sPanelFaceX = gFacesData[ pSoldier->iFaceIndex ].usFaceX;
				pSoldier->sPanelFaceY = gFacesData[ pSoldier->iFaceIndex ].usFaceY;
			}


			if ( fDirty != DIRTYLEVEL0 )
			{
				// UPdate stats!
				if ( fDirty == DIRTYLEVEL2 )
				{
					if ( pSoldier->stats.bLife != 0 )
					{
						if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
						{
							 swprintf( pStr, TacticalStr[ VEHICLE_VITAL_STATS_POPUPTEXT ], pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax );
							 SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
						}
						else if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
						{
							 swprintf( pStr, gzLateLocalizedString[ 16 ], pSoldier->stats.bLife, pSoldier->stats.bLifeMax );
							 SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
						}
						else
						{
							GetMoraleString( pSoldier, pMoraleStr );
			
							if ( UsingFoodSystem() && pSoldier->ubProfile != ROBOT && !IsVehicle(pSoldier) )
							{
								swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_WITH_FOOD_POPUPTEXT ], pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pMoraleStr, (INT32)(100*(pSoldier->bDrinkLevel - FOOD_MIN) / FOOD_HALF_RANGE), L"%", (INT32)(100*(pSoldier->bFoodLevel - FOOD_MIN) / FOOD_HALF_RANGE), L"%" );
							}
							else
							{
								swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_POPUPTEXT ], pSoldier->stats.bLife, pSoldier->stats.bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pMoraleStr );
							}

							{
								CHAR16	atStr[500];
								swprintf( atStr, L"" );

								pSoldier->PrintDiseaseDesc( atStr );

								wcscat( pStr, atStr );
							}

							SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
			
							// Buggler: skills/traits tooltip on merc portrait
							// clear pStr value
							swprintf( pStr, L"");
			
							INT8 bSeatIndex = GetSeatIndexFromSoldier( pSoldier );
							if( bSeatIndex != (-1) )
							{					
								swprintf( sTemp, L"%s\n", gNewVehicle[ pVehicleList[ pSoldier->iVehicleId ].ubVehicleType ].VehicleSeats[ bSeatIndex ].zSeatName );
								wcscat( pStr, sTemp );
							}

							if (gGameOptions.fNewTraitSystem) // SANDRO - old/new traits check
							{
								UINT8 ubTempSkillArray[30];
								int bNumSkillTraits = 0;
			
								// lets rearrange our skills to a temp array
								// we also get the number of lines (skills) to be displayed 
								for ( UINT8 ubCnt = 1; ubCnt < NUM_SKILLTRAITS_NT; ++ubCnt )
								{
									if ( ProfileHasSkillTrait( pSoldier->ubProfile, ubCnt ) == 2 )
									{
										ubTempSkillArray[bNumSkillTraits] = (ubCnt + NEWTRAIT_MERCSKILL_EXPERTOFFSET);
										++bNumSkillTraits;
									}
									else if ( ProfileHasSkillTrait( pSoldier->ubProfile, ubCnt ) == 1 )
									{
										ubTempSkillArray[bNumSkillTraits] = ubCnt;
										++bNumSkillTraits;
									}
								}
			
								if ( bNumSkillTraits == 0 )
								{
									swprintf( sTemp, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
									wcscat( pStr, sTemp );
								}
								else
								{
									for ( int ubCnt = 0; ubCnt < bNumSkillTraits; ++ubCnt )
									{
										swprintf( sTemp, L"%s\n", gzMercSkillTextNew[ ubTempSkillArray[ubCnt] ] );
										wcscat( pStr, sTemp );
									}
								}
							}
							else
							{
								INT8 bSkill1 = 0, bSkill2 = 0; 	
								bSkill1 = gMercProfiles[ pSoldier->ubProfile ].bSkillTraits[0];
								bSkill2 = gMercProfiles[ pSoldier->ubProfile ].bSkillTraits[1];
			
								if ( bSkill1 == 0 && bSkill2 == 0 )
								{
									swprintf( pStr, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
								}
								else
								{
									//if the 2 skills are the same, add the '(expert)' at the end
									if( bSkill1 == bSkill2 )
									{
										swprintf( pStr, L"%s %s", gzMercSkillText[bSkill1], gzMercSkillText[EXPERT] );
									}
									else
									{
										//Display the first skill
										if( bSkill1 != 0 )
										{
											swprintf( pStr, L"%s\n", gzMercSkillText[bSkill1] );
										}
										if( bSkill2 != 0 )
										{
											swprintf( sTemp, L"%s", gzMercSkillText[bSkill2] );
											wcscat( pStr, sTemp );
										}
									}
								}
							}
							SetRegionFastHelpText( &(gTEAM_FaceRegions[ cnt ]), pStr );
						}
					}
					else
					{
						SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), L"" );
						SetRegionFastHelpText( &(gTEAM_FaceRegions[ cnt ]), L"" );
					}
				}

				if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
				{
					DrawLifeUIBarEx( pSoldier, sTEAMLifeXY[ posIndex ], sTEAMLifeXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );

					if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
					{
							DrawBreathUIBarEx( pSoldier, sTEAMBreathXY[ posIndex ], sTEAMBreathXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );
							DrawMoraleUIBarEx( pSoldier, sTEAMMoraleXY[ posIndex ], sTEAMMoraleXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );
					}

					if ( gTacticalStatus.uiFlags & TURNBASED && pSoldier->stats.bLife >= OKLIFE )
					{
						// Render APs
						SetFont( TINYFONT1 );

						//if ( pSoldier->sLastTarget != NOWHERE && !EnoughPoints( pSoldier, MinAPsToAttack( pSoldier, pSoldier->sLastTarget, TRUE ), 0, FALSE ) || GetUIApsToDisplay( pSoldier ) < 0 )
						if ( !EnoughPoints( pSoldier, MinAPsToAttack( pSoldier, pSoldier->sLastTarget, TRUE, 0 ), 0, FALSE ) || GetUIApsToDisplay( pSoldier ) < 0 )
						{
							SetFontBackground( FONT_MCOLOR_BLACK );
							SetFontForeground( FONT_MCOLOR_DKRED );
						}
						else
						{
							if ( MercDruggedOrDrunk( pSoldier ) )
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								//SetFontForeground( FONT_MCOLOR_LTBLUE );

								// Flugente: new colour for being drugged, as blue on black was hard to see
								SetRGBFontForeground( 250, 5, 250 );
							}
							else if ( pSoldier->bStealthMode )
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								SetFontForeground( FONT_MCOLOR_LTYELLOW );
							}
							else
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								SetFontForeground( FONT_MCOLOR_LTGRAY );
							}
						}
						RestoreExternBackgroundRect( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1 ], TM_AP_WIDTH+1, TM_AP_HEIGHT );

						if (gTacticalStatus.uiFlags & INCOMBAT )
						{
							VarFindFontCenterCoordinates( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1], TM_AP_WIDTH, TM_AP_HEIGHT, TINYFONT1, &sFontX, &sFontY, L"%d", GetUIApsToDisplay( pSoldier ) );
							mprintf( sFontX, sTEAMApXY[ posIndex + 1], L"%d", GetUIApsToDisplay( pSoldier ) );
						}
					}
				}
				else
				{
					//Erase!
					//DrawUIBar( pSoldier->bBreath, sTEAMBreathXY[ posIndex ], sTEAMBreathXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, DRAW_ERASE_BAR );
					//DrawUIBar( pSoldier->stats.bLife, sTEAMLifeXY[ posIndex ], sTEAMLifeXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, DRAW_ERASE_BAR );

					// Erase APs
					RestoreExternBackgroundRect( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1 ], TM_AP_WIDTH+1, TM_AP_HEIGHT );
				}
			}

			RenderSoldierTeamInv( pSoldier, sTEAMHandInvXY[ posIndex ], sTEAMHandInvXY[ posIndex + 1 ], (UINT8)cnt, fDirty );

			HandleSoldierFaceFlash( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] );


			if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
				//HandlePanelFaceAnimations( pSoldier );
			}

		}
		else
		{
			// anv: for unoccupied slots clear tooltips
			SetRegionFastHelpText( &(gTEAM_FaceRegions[ cnt ]), L"" );
			SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), L"" );
		}
	}

	UpdateTEAMPanel( );

	if( fRenderRadarScreen == TRUE )
	{
		// Render clock
		// CHRISL: Change coords to base off of right side of screen and use new paramters in RenderTownIDString
		RenderClock( INTERFACE_CLOCK_TM_X, INTERFACE_CLOCK_TM_Y );
		CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_TM_X, INTERFACE_CLOCK_TM_Y );
		RenderTownIDString( LOCATION_NAME_TM_X, LOCATION_NAME_TM_Y);
	}
	else
	{
		RemoveMouseRegionForPauseOfClock( );
	}
}

BOOLEAN CreateTEAMPanelButtons( )
{
CHAR8	ubString[48];

	FilenameForBPP("INTERFACE\\bottom_bar_buttons.sti", ubString);

	// Load button Graphics
	iTEAMPanelImages[ ENDTURN_IMAGES	]			= LoadButtonImage(ubString,-1,0,-1, 3,-1 );
	iTEAMPanelImages[ ROSTERMODE_IMAGES	]	= UseLoadedButtonImage( iTEAMPanelImages[ ENDTURN_IMAGES	] ,-1, 1,-1, 4,-1 );
	iTEAMPanelImages[ DISK_IMAGES	]				= UseLoadedButtonImage( iTEAMPanelImages[ ENDTURN_IMAGES	] ,-1, 2,-1, 5, -1 );

	// Create buttons
	iTEAMPanelButtons[ TEAM_DONE_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ ENDTURN_IMAGES ], TM_ENDTURN_X, TM_ENDTURN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnEndTurnCallback );
	if ( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ TEAM_DONE_BUTTON ], TacticalStr[ END_TURN_POPUPTEXT ] );



	iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ ROSTERMODE_IMAGES ], TM_ROSTERMODE_X, TM_ROSTERMODE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnRostermodeCallback );
	if ( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ], TacticalStr[ MAPSCREEN_POPUPTEXT ] );

	iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ DISK_IMAGES ], TM_DISK_X, TM_DISK_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnSquadCallback );
	if ( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ], TacticalStr[ CHANGE_SQUAD_POPUPTEXT ] );

	return( TRUE );
}

void	RemoveTEAMPanelButtons( )
{
	UINT32 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_BUTTONS; cnt++ )
	{
		RemoveButton( iTEAMPanelButtons[ cnt ] );
	}

	for ( cnt = 0; cnt < NUM_TEAM_BUTTON_IMAGES; cnt++ )
	{
		UnloadButtonImage( iTEAMPanelImages[ cnt ] );
	}

}

void BtnEndTurnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// END TURN
		UIHandleEndTurn( NULL );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void BtnRostermodeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if ( guiCurrentScreen == GAME_SCREEN )
		{
			GoToMapScreenFromTactical();
			EnableRadarScreenRender( );
		}

	}

}


// callback to handle squad switching callback
void BtnSquadCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Not Implemented Yet" );
		ToggleRadarScreenRender( );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}

void SetTEAMPanelCurrentMerc( UINT8 ubNewID )
{
	// Ignore ther ID given!
	// ALWYAS USE CURRENT MERC!

	fInterfacePanelDirty = DIRTYLEVEL2;

	UpdateTEAMPanel( );

}

void UpdateTEAMPanel( )
{
	INT32		cnt;

// WANNE: In editor mode, always disable the Done button, because we dont have any merc!
#ifdef JA2EDITOR
	DisableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
#else
	if ( (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
	}
	else
	{
		EnableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
	}
#endif

// WANNE: In editor mode, always disable the strategy map button, because we dont have any merc!
#ifdef JA2EDITOR
	DisableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
#else

	if ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
	}
#endif


	if ( gfDisableTacticalPanelButtons )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
		DisableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
		DisableButton( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );

		// OK, disable item regions.......
		for ( cnt = 0; cnt < NUMBER_OF_SOLDIERS_PER_SQUAD; cnt++ )
		{
			MSYS_DisableRegion( &gTEAM_EnemyIndicator[ cnt ] );

			MSYS_DisableRegion( &gTEAM_FirstHandInv[ cnt ] );

			MSYS_DisableRegion( &gTEAM_SecondHandInv[ cnt ] );
		}

		//disable the radar map region
		// If NOT in overhead map
		if ( !InOverheadMap( ) )
		{
			MSYS_DisableRegion(&gRadarRegion);
		}

	}
	else
	{

// WANNE: In editor mode, always disable the squad button, because we dont have any merc!
#ifdef JA2EDITOR
		DisableButton( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );
#else
		EnableButton( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );
#endif

		for ( cnt = 0; cnt < NUMBER_OF_SOLDIERS_PER_SQUAD; cnt++ )
		{
			MSYS_EnableRegion( &gTEAM_EnemyIndicator[ cnt ] );

			MSYS_EnableRegion( &gTEAM_FirstHandInv[ cnt ] );

			MSYS_EnableRegion( &gTEAM_SecondHandInv[ cnt ] );
		}

		//Enable the radar map region
		MSYS_EnableRegion(&gRadarRegion);

	}


}


void HandleMouseOverTeamFaceForContMove( BOOLEAN fOn )
{
	FACETYPE *pFace;

	if ( fOn )
	{
		// Check if we are waiting to continue move...
		if ( CheckForMercContMove( gpSMCurrentMerc ) )
		{
			// Display 'cont' on face....
			// Get face
			pFace = &gFacesData[ gpSMCurrentMerc->iFaceIndex ];

			pFace->fDisplayTextOver = FACE_DRAW_TEXT_OVER;
			wcscpy( pFace->zDisplayText, TacticalStr[ CONTINUE_OVER_FACE_STR ] );

			// While our mouse is here, draw a path!
			PlotPath( gpSMCurrentMerc, gpSMCurrentMerc->pathing.sFinalDestination, NO_COPYROUTE, PLOT, TEMPORARY, (UINT16)gpSMCurrentMerc->usUIMovementMode, NOT_STEALTH, FORWARD, gpSMCurrentMerc->bActionPoints );
		}
	}
	else
	{
		// Remove 'cont' on face....
		// Get face
		pFace = &gFacesData[ gpSMCurrentMerc->iFaceIndex ];

		pFace->fDisplayTextOver = FACE_ERASE_TEXT_OVER;

		// Erase path!
		ErasePath( TRUE );
	}
}


void MercFacePanelMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;
	SOLDIERTYPE	*pSoldier;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if ( ubID == NOBODY )
	{
		return;
	}

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}

	pSoldier = MercPtrs[ ubSoldierID ];

	if ( !pSoldier->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		HandleMouseOverSoldierFaceForContMove( pSoldier, TRUE );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		HandleMouseOverSoldierFaceForContMove( pSoldier, FALSE );
	}

}


void EnemyIndicatorClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	if ( ubID == NOBODY )
	{
		return;
	}

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		//if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
		//{
		//}
		//else
		{
			SOLDIERTYPE *pSoldier;

			pSoldier = MercPtrs[ ubSoldierID ];

			if ( pSoldier->aiData.bOppCnt > 0 )
			{	// Cycle....
				CycleVisibleEnemies( pSoldier );
			}
			else
			{
				MercFacePanelCallback( pRegion, iReason );
			}
		}
	}
}


void MercFacePanelCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
		{
			SetInfoChar( ubID );
		}
		return;
	}

	if ( ubID == NOBODY )
	{
		return;
	}

	// anv: did we click on empty panel?
	if ( !gTeamPanel[ ubID ].fOccupied )
	{
		return;
	}

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}

	if (!OK_INTERRUPT_MERC(MercPtrs[ubSoldierID]))
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( !gfInItemPickupMenu && gpItemPointer == NULL )
		{
			//if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			//if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER ) )
			//{
			//	pVehicle = GetSoldierStructureForVehicle( MercPtrs[ ubSoldierID ]->iVehicleId );

			//	HandleLocateSelectMerc( pVehicle->ubID, 0 );
			//}
			//else
			{
				if ( !InOverheadMap( ) )
				{
					// If we can continue a move, do so!
					if ( CheckForMercContMove( MercPtrs[ ubSoldierID ] ) )
					{
						// Continue
						ContinueMercMovement( MercPtrs[ ubSoldierID ] );
						ErasePath( TRUE );
					}
					else
					{
						// HEADROCK HAM 3.5: Shift-Click a merc's face will add him to the current selection.
						if (!(gTacticalStatus.uiFlags & INCOMBAT) && _KeyDown( SHIFT ) )
						{
							if ( ! (MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED ) )
							{
								if ( OK_CONTROLLABLE_MERC( MercPtrs[ ubSoldierID ] ) && !( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_PASSENGER | SOLDIER_DRIVER ) ) )
								{
									//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s added", MercPtrs[ ubSoldierID ]->name );
									MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags |= SOLDIER_MULTI_SELECTED;
									MercPtrs[ ubSoldierID ]->flags.uiStatusFlags |= SOLDIER_MULTI_SELECTED;
									EndMultiSoldierSelection( TRUE );
								}
							}
							// A shift-click on a selected character will remove that character from the current selection.
							else
							{
								//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s removed", MercPtrs[ ubSoldierID ]->name );
								MercPtrs[ ubSoldierID ]->flags.uiStatusFlags &= (~SOLDIER_MULTI_SELECTED );
								if (ubSoldierID != gusSelectedSoldier)
								{
									MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags |= SOLDIER_MULTI_SELECTED;
								}
								EndMultiSoldierSelection( TRUE );
							}
						}
						else
						{
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s selected", MercPtrs[ ubSoldierID ]->name );
							HandleLocateSelectMerc( ubSoldierID, 0 );
						}
					}
				}
				else
				{
					SelectSoldier( ubSoldierID, TRUE, FALSE );
				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if ( !InOverheadMap( ) )
		{
			// Only if guy is not dead!
			//if ( !( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_DEAD ) && !AM_AN_EPC( MercPtrs[ ubSoldierID ] ) && !( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
			if ( !( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_DEAD ) && !AM_AN_EPC( MercPtrs[ ubSoldierID ] ) )
			{
				gfSwitchPanel = TRUE;
				gbNewPanel = SM_PANEL;
				gubNewPanelParam = (UINT8)ubSoldierID;
				// silversurfer: This needs to be reset to TRUE because inventory view only allows radar screen to be displayed and not the squad list.
				fRenderRadarScreen = TRUE;
			}
		}
	}

}

extern void InternalSelectSoldier( UINT16 usSoldierID, BOOLEAN fAcknowledge, BOOLEAN fForceReselect, BOOLEAN fFromUI );

void HandleLocateSelectMerc( UINT8 ubID, INT8 bFlag	)
{
	BOOLEAN fSelect = FALSE;

	if( !MercPtrs[ ubID ]->bActive )
	{
		return;
	}

	if ( gpItemPointer != NULL )
	{
		// ATE: Disable for feel purposes....
		//return;
	}


	// ATE: No matter what we do... if below OKLIFE, just locate....
	if ( MercPtrs[ ubID ]->stats.bLife < OKLIFE )
	{
		LocateSoldier( ubID, SETLOCATOR );
		return;
	}

	// Flugente: frozen soldiers can't be selected
	if ( MercPtrs[ubID]->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
	{
		LocateSoldier( ubID, SETLOCATOR );
		return;
	}

	if ( _KeyDown( ALT ) )
	{
		if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
		{
			// Select merc
			InternalSelectSoldier( ubID, TRUE, FALSE, TRUE);
			MercPtrs[ ubID ]->flags.fFlashLocator = FALSE;
			ResetMultiSelection( );
		}
		else
		{
			// Just locate....
			LocateSoldier( ubID, SETLOCATOR );
		}
	}
	else
	{
		if ( MercPtrs[ ubID ]->flags.fFlashLocator == FALSE )
		{
			if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
			{
				// If we are currently selected, slide to location
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR);
				}
				else
				{
					LocateSoldier( ubID, SETLOCATOR );
				}
			}
			else
			{
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR );
				}
				else
				{
					fSelect = TRUE;
					//ShowRadioLocator( ubID, SHOW_LOCATOR_NORMAL );
				}
			}
		}
		else
		{
			if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
			{
				// If we are currently selected, slide to location
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,DONTSETLOCATOR);
				}
				else
				{
					LocateSoldier( ubID, DONTSETLOCATOR );
				}

				fSelect = TRUE;
			}
			else
			{
				if ( ubID == gusSelectedSoldier )
				{
					LocateSoldier( ubID, DONTSETLOCATOR );
				}
				else
				{
					fSelect = TRUE;
				}
			}
		}

		if ( bFlag == LOCATEANDSELECT_MERC )
		{
			fSelect = TRUE;
		}

		if ( fSelect )
		{
			// Select merc, only if alive!
			if ( !( MercPtrs[ ubID ]->flags.uiStatusFlags & SOLDIER_DEAD ) )
			{
				InternalSelectSoldier( ubID, TRUE, FALSE, TRUE );
			}
		}
	}

	ResetMultiSelection( );

	// Handle locate select merc....
	HandleMouseOverSoldierFaceForContMove( MercPtrs[ ubID ], TRUE );

}



void ShowRadioLocator( UINT8 ubID, UINT8 ubLocatorSpeed )
{
	RESETTIMECOUNTER( MercPtrs[ ubID ]->timeCounters.FlashSelCounter, FLASH_SELECTOR_DELAY );

	//LocateSoldier( ubID, FALSE );	// IC - this is already being done outside of this function :)
	MercPtrs[ ubID ]->flags.fFlashLocator = TRUE;
	//gbPanelSelectedGuy = ubID;	IC - had to move this outside to make this function versatile
	MercPtrs[ ubID ]->sLocatorFrame = 0;

	if ( ubLocatorSpeed == SHOW_LOCATOR_NORMAL )
	{
		// If we are an AI guy, and we have the baton, make lower...
		// ( MercPtrs[ ubID ]->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL && MercPtrs[ ubID ]->bTeam != gbPlayerNum )
		//
		//ercPtrs[ ubID ]->ubNumLocateCycles = 3;
		//
		//se
		//
			MercPtrs[ ubID ]->ubNumLocateCycles = 5;
		//
	}
	else
	{
		MercPtrs[ ubID ]->ubNumLocateCycles = 3;
	}
}

void EndRadioLocator( UINT8 ubID )
{
	MercPtrs[ ubID ]->flags.fFlashLocator = FALSE;
	MercPtrs[ ubID ]->flags.fShowLocator = FALSE;
}



void CheckForFacePanelStartAnims( SOLDIERTYPE *pSoldier, INT16 sPanelX, INT16 sPanelY )
{

	if ( !pSoldier->bActive )
	{
		return;
	}


	if ( pSoldier->flags.fUIdeadMerc	)
	{
//		pSoldier->sPanelFaceX	= sPanelX;
//		pSoldier->sPanelFaceY	= sPanelY;
	}


}


void FinishAnySkullPanelAnimations( )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32		cnt2;

	cnt2 = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt2 ]; cnt2 <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt2++,pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->stats.bLife == 0 )
		{
			if ( pTeamSoldier->flags.fUIdeadMerc || pTeamSoldier->flags.fClosePanelToDie )
			{
				HandlePlayerTeamMemberDeathAfterSkullAnimation( pTeamSoldier );

				pTeamSoldier->flags.fUIdeadMerc = FALSE;
				pTeamSoldier->flags.fClosePanelToDie = FALSE;
			}
		}
	}

}

void HandlePanelFaceAnimations( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->bTeam != gbPlayerNum )
	{
		return;
	}


	if ( !pSoldier->bActive )
	{
		return;
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// Don't do this for a vehice.
		return;
	}

	// If this is the first time we are active, open panel!
	if ( pSoldier->flags.fUInewMerc )
	{
		pSoldier->flags.fUInewMerc = FALSE;
	}

	if ( pSoldier->flags.fUICloseMerc )
	{
		pSoldier->flags.fUICloseMerc = FALSE;
	}

	if ( pSoldier->flags.fUIdeadMerc	)
	{
		pSoldier->sPanelFaceX = gFacesData[ pSoldier->iFaceIndex ].usFaceX;
		pSoldier->sPanelFaceY = gFacesData[ pSoldier->iFaceIndex ].usFaceY;

		pSoldier->flags.fUIdeadMerc = FALSE;
		pSoldier->flags.fClosePanel		= TRUE;
		pSoldier->flags.fClosePanelToDie = TRUE;
		pSoldier->ubClosePanelFrame = 0;
		pSoldier->ubDeadPanelFrame = 0;
		RESETTIMECOUNTER( pSoldier->timeCounters.PanelAnimateCounter, 160 );
	}

	if ( pSoldier->flags.fClosePanel )
	{
		if ( TIMECOUNTERDONE( pSoldier->timeCounters.PanelAnimateCounter, 160 ) )
		{
				pSoldier->ubClosePanelFrame++;

				if ( pSoldier->ubClosePanelFrame > 5 )
				{
					pSoldier->flags.fClosePanel = FALSE;
					pSoldier->ubClosePanelFrame = 5;

					if ( pSoldier->flags.fClosePanelToDie )
					{
						pSoldier->flags.fDeadPanel = TRUE;
						//PlayJA2Sample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					}
					else
					{
						if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
						{
							RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
						}
					}
				}
				RESETTIMECOUNTER( pSoldier->timeCounters.PanelAnimateCounter, 160 );
		}
	}

	if ( pSoldier->flags.fClosePanel )
	{
		// Render panel!
		//if(gbPixelDepth==16)
		//{
			if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
			{
				RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
				BltVideoObjectFromIndex( FRAME_BUFFER, guiCLOSE, pSoldier->ubClosePanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
				InvalidateRegion( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, pSoldier->sPanelFaceX + TM_FACE_WIDTH, pSoldier->sPanelFaceY + TM_FACE_HEIGHT );
			}
		//}
	}


	if ( pSoldier->flags.fDeadPanel )
	{
		if ( TIMECOUNTERDONE(	pSoldier->timeCounters.PanelAnimateCounter, 160 ) )
		{
				pSoldier->ubDeadPanelFrame++;

				if ( pSoldier->ubDeadPanelFrame == 4 )
				{
					ScreenMsg( FONT_RED, MSG_SKULL_UI_FEEDBACK, pMercDeadString[ 0 ], pSoldier->name );

					PlayJA2Sample( (UINT8)DOORCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					PlayJA2Sample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				}

				if ( pSoldier->ubDeadPanelFrame > 5 )
				{
					pSoldier->flags.fDeadPanel = FALSE;
					pSoldier->ubDeadPanelFrame = 5;
					pSoldier->flags.fClosePanelToDie = FALSE;

					// Finish!
					if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
					{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiDEAD, pSoldier->ubDeadPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

						// Blit hatch!
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiHATCH, 0, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

						RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					}
					HandlePlayerTeamMemberDeathAfterSkullAnimation( pSoldier );

				}
				RESETTIMECOUNTER( pSoldier->timeCounters.PanelAnimateCounter, 160 );
		}
	}

	if ( pSoldier->flags.fDeadPanel )
	{
		// Render panel!
		//if(gbPixelDepth==16)
		//{
				if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
				{
					BltVideoObjectFromIndex( FRAME_BUFFER, guiDEAD, pSoldier->ubDeadPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

					// Blit hatch!
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiHATCH, 0, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

					InvalidateRegion( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, pSoldier->sPanelFaceX + TM_FACE_WIDTH, pSoldier->sPanelFaceY + TM_FACE_HEIGHT );
				}
		//}
	}


	if ( pSoldier->flags.fOpenPanel )
	{
		if ( TIMECOUNTERDONE( pSoldier->timeCounters.PanelAnimateCounter, 160 ) )
		{
				pSoldier->bOpenPanelFrame--;

				if ( pSoldier->bOpenPanelFrame < 0 )
				{
					pSoldier->flags.fOpenPanel = FALSE;
					pSoldier->bOpenPanelFrame = 0;

					if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
					{
						RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					}
				}
				RESETTIMECOUNTER( pSoldier->timeCounters.PanelAnimateCounter, 160 );
		}
	}

	if ( pSoldier->flags.fOpenPanel )
	{
		// Render panel!
		//if(gbPixelDepth==16)
		//{
				if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
				{
					RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCLOSE, pSoldier->bOpenPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
					//InvalidateRegion( sTEAMFacesXY[ ubOpenPanelID ], sTEAMFacesXY[ ubOpenPanelID + 1 ], sTEAMFacesXY[ ubOpenPanelID ] + TM_FACE_WIDTH, sTEAMFacesXY[ ubOpenPanelID + 1 ] + TM_FACE_HEIGHT );
				}
		//}
	}

}



void HandleSoldierFaceFlash( SOLDIERTYPE *pSoldier, INT16 sFaceX, INT16 sFaceY )
{

}



void RenderSoldierTeamInv( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY, UINT8 ubPanelNum, BOOLEAN fDirty )
{
	if ( pSoldier->bActive && !(pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_DRIVER )
		{			
			INVRenderSteeringWheel( guiSAVEBUFFER, guiVEHINV, pSoldier, sX, sY, TM_INV_WIDTH, TM_INV_HEIGHT, fDirty );
		}
		else
		{
			// Look in primary hand
			INVRenderItem( guiSAVEBUFFER, pSoldier, &(pSoldier->inv[ HANDPOS ]), sX, sY, TM_INV_WIDTH, TM_INV_HEIGHT, fDirty, &(gfTEAM_HandInvDispText[ ubPanelNum ][ HANDPOS ] ), 0 , FALSE, 0);
		}

		if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiVEHINV, 1, sX, (INT16)(sY + TM_INV_HAND_SEPY), VO_BLT_SRCTRANSPARENCY, NULL );
			RestoreExternBackgroundRect( sX, (INT16)(sY + TM_INV_HAND_SEPY), (INT16)(TM_INV_WIDTH ) , (INT16)( TM_INV_HEIGHT ) );
		}
		else
		{
			// Do secondary hand
			INVRenderItem( guiSAVEBUFFER, pSoldier, &(pSoldier->inv[ SECONDHANDPOS ]), sX, (INT16)(sY + TM_INV_HAND_SEPY), TM_INV_WIDTH, TM_INV_HEIGHT, fDirty, &(gfTEAM_HandInvDispText[ ubPanelNum ][ SECONDHANDPOS ] ), 0 , FALSE, 0);
		}

	}
}


void TMFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( ubID == NOBODY )
		return;


	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;


	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		//OK FOR NOW, DROP ITEM WHERE BUDDY IS
		{
		//	SOLDIERTYPE *pSoldier;

		//	if(	GetSoldier( &pSoldier, ubSoldierID ) )
		//	{
		//	}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}

}


void TMClickFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8		ubID, ubSoldierID;
	UINT16	usOldHandItem;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );


	if ( ubID == NOBODY )
		return;

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
		return;

	if (iReason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// anv: select vehicle by clicking on the steering wheel
		//if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & SOLDIER_DRIVER )
		//{
		//	SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( MercPtrs[ ubSoldierID ]->iVehicleId );
		//	HandleLocateSelectMerc( pVehicle->ubID, 0 );
		//}
		//else
		{
			// Change to use cursor mode...
			guiPendingOverrideEvent = A_ON_TERRAIN;
		}
	}

	if (iReason == MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if ( !AM_A_ROBOT( MercPtrs[ ubSoldierID ] ) )
		{
			usOldHandItem = MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem;
			//SwapOutHandItem( MercPtrs[ ubSoldierID ] );
			SwapHandItems( MercPtrs[ ubSoldierID ] );
			MercPtrs[ ubSoldierID ]->ReLoadSoldierAnimationDueToHandItemChange( usOldHandItem, MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem );
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
	}
}


void TMClickSecondHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;
	UINT16	usOldHandItem;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	if ( ubID == NOBODY )
		return;

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
		return;

	if (iReason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
			ExitVehicle( MercPtrs[ ubSoldierID ] );
		}
		else if (UsingNewInventorySystem() && !AM_A_ROBOT(MercPtrs[ubSoldierID]))
		{
			MercPtrs[ubSoldierID]->SwitchWeapons();
		}
	}

	if (iReason == MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if ( MercPtrs[ ubSoldierID ]->flags.uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
		}
		else
		{
			if ( !AM_A_ROBOT( MercPtrs[ ubSoldierID ] ) )
			{
				usOldHandItem = MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem;
				SwapHandItems( MercPtrs[ ubSoldierID ] );
				MercPtrs[ ubSoldierID ]->ReLoadSoldierAnimationDueToHandItemChange( usOldHandItem, MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem );
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}
	}
}


BOOLEAN PlayerExistsInSlot( UINT8 ubID )
{
	INT32 cnt;

	for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubID )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


INT8 GetTeamSlotFromPlayerID( UINT8 ubID )
{
	INT8 cnt;

	for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubID )
			{
				return( cnt );
			}
		}
	}

	return( -1 );
}


BOOLEAN RemovePlayerFromTeamSlotGivenMercID( UINT8 ubMercID )
{
	INT32 cnt;

	for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubMercID )
			{
				RemovePlayerFromInterfaceTeamSlot( (UINT8)cnt );
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


void AddPlayerToInterfaceTeamSlot( UINT8 ubID )
{
	INT32	cnt;

	// If we are a vehicle don't ever add.....
	if ( MercPtrs[ ubID ]->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if ( !( MercPtrs[ ubID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
	{
		if ( !PlayerExistsInSlot( ubID ) )
		{
			// Find a free slot
			for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
			{
				if ( !gTeamPanel[ cnt ].fOccupied	)
				{
					gTeamPanel[ cnt ].fOccupied = TRUE;
					gTeamPanel[ cnt ].ubID			= ubID;

					MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ cnt ], 0, cnt );
					MSYS_SetRegionUserData( &gTEAM_FaceRegions[ cnt ], 0, cnt );

					// DIRTY INTERFACE
					fInterfacePanelDirty = DIRTYLEVEL2;

					// Set ID to do open anim
					MercPtrs[ ubID ]->flags.fUInewMerc						= TRUE;

					break;
				}
			}
		}
	}
	else
	{
		// anv: for passengers, position on team panel will be linked with seat in vehicle
		if ( MercPtrs[ ubID ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
		{
			SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( MercPtrs[ ubID ]->iVehicleId );
			if( pVehicle != NULL )
			{
				for( UINT8 iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ MercPtrs[ ubID ]->iVehicleId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
				{
					SOLDIERTYPE *pPassenger = pVehicleList[ MercPtrs[ ubID ]->iVehicleId ].pPassengers[ iCounter ];
					if( pPassenger != NULL && pPassenger->ubID == ubID )
					{
						gTeamPanel[ iCounter ].fOccupied = TRUE;
						gTeamPanel[ iCounter ].ubID = ubID;

						MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ iCounter ], 0, iCounter );
						MSYS_SetRegionUserData( &gTEAM_FaceRegions[ iCounter ], 0, iCounter );

						// DIRTY INTERFACE
						fInterfacePanelDirty = DIRTYLEVEL2;

						// Set ID to do open anim
						MercPtrs[ ubID ]->flags.fUInewMerc = TRUE;
	
						return;
					}
				}
			}	
		}
	}
}

BOOLEAN InitTEAMSlots( )
{
	INT32 cnt;

	for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		gTeamPanel[ cnt ].fOccupied = FALSE;
		gTeamPanel[ cnt ].ubID			= NOBODY;

	}

	return( TRUE );
}


BOOLEAN GetPlayerIDFromInterfaceTeamSlot( UINT8 ubPanelSlot, UINT8 *pubID )
{
	if ( ubPanelSlot >= gGameOptions.ubSquadSize )
	{
		return( FALSE );
	}

	if ( gTeamPanel[ ubPanelSlot ].fOccupied )
	{
		*pubID = gTeamPanel[ ubPanelSlot ].ubID;
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

void RemoveAllPlayersFromSlot( )
{
	int cnt;

	for ( cnt = 0; cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		RemovePlayerFromInterfaceTeamSlot( (UINT8)cnt );
	}
}


BOOLEAN RemovePlayerFromInterfaceTeamSlot( UINT8 ubPanelSlot )
{
	if ( ubPanelSlot >= gGameOptions.ubSquadSize )
	{
		return( FALSE );
	}

	if ( gTeamPanel[ ubPanelSlot ].fOccupied )
	{
		if ( !( MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		// Set Id to close
		MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->flags.fUICloseMerc		= TRUE;
	}

		// Set face to inactive...
		SetAutoFaceInActive( MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->iFaceIndex );


		gTeamPanel[ ubPanelSlot ].fOccupied = FALSE;
		gTeamPanel[ ubPanelSlot ].ubID = NOBODY;

		MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ ubPanelSlot ], 0, NOBODY );
		MSYS_SetRegionUserData( &gTEAM_FaceRegions[ ubPanelSlot ],	0, NOBODY );

		// DIRTY INTERFACE
		fInterfacePanelDirty = DIRTYLEVEL2;

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


/* CHRISL: Adjust function to allow X,Y coordinates to be passed so that we can render the TownID string
seperately between the team and inventory windows. */
void RenderTownIDString( INT16 sX, INT16 sY )
{
	CHAR16	zTownIDString[80];
	INT16 sFontX, sFontY;

	// when NIV is open in tactical
	if (sY+GetFontHeight(COMPFONT) >= SCREEN_HEIGHT) return;

	// Render town, position
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	GetSectorIDString( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTownIDString, TRUE );
	ReduceStringLength( zTownIDString, 80, COMPFONT );
	VarFindFontCenterCoordinates( sX , sY, 80, 16, COMPFONT, &sFontX, &sFontY, zTownIDString );
	mprintf( sFontX, sFontY, L"%s", zTownIDString );
}


void CheckForAndAddMercToTeamPanel( SOLDIERTYPE *pSoldier )
{

	if ( pSoldier->bActive	)
	{
		// Add to interface if the are ours
		if ( pSoldier->bTeam == gbPlayerNum )
		{
			// Are we in the loaded sector?
			if ( pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ && !pSoldier->flags.fBetweenSectors && pSoldier->bInSector )
			{
				// IF on duty....
				if( ( pSoldier->bAssignment ==	CurrentSquad( ) )|| ( SoldierIsDeadAndWasOnSquad( pSoldier, ( INT8 )( CurrentSquad( ) ) ) ) )
				{

					if( pSoldier->bAssignment == ASSIGNMENT_DEAD )
					{
						pSoldier->flags.fUICloseMerc = FALSE;
					}
					// ATE: ALrighty, if we have the insertion code of helicopter..... don't add just yet!
					/// ( will add in heli code )
					if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER )
					{
						AddPlayerToInterfaceTeamSlot( pSoldier->ubID );
					}

					// ARE WE A VEHICLE.....
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
					{
						AddPassangersToTeamPanel( pSoldier->bVehicleID );
					}
				}
			}
			else
			{
				// Make sure we are NOT in this world!
				// Set gridno to one that's not visib;l
				pSoldier->RemoveSoldierFromGridNo( );

				// Remove slot from tactical
				RemoveMercSlot( pSoldier );

			}
		}
	}
}

void CleanUpStack( OBJECTTYPE * pObj, OBJECTTYPE * pCursorObj )
{
	INT16	bMaxPoints;

	if ( !(Item[pObj->usItem].usItemClass & IC_AMMO || Item[pObj->usItem].usItemClass & IC_KIT || Item[pObj->usItem].usItemClass & IC_MEDKIT || Item[pObj->usItem].canteen || Item[pObj->usItem].gascan || Item[pObj->usItem].alcohol > 0.0f) )
	{
		return;
	}

	if ( Item[ pObj->usItem ].usItemClass & IC_AMMO )
	{
		bMaxPoints = Magazine[ Item[ pObj->usItem ].ubClassIndex ].ubMagSize;
	}
	else
	{
		bMaxPoints = 100;
	}

	if ( pCursorObj && pCursorObj->usItem == pObj->usItem )
	{
		DistributeStatus(pCursorObj, pObj, bMaxPoints);
	}
	DistributeStatus(pObj, pObj, bMaxPoints);

	// Flugente: one of the items on the stack might not be full. Make sure it is the first one, so the player can see what is missing at a glance
	if ( pObj->ubNumberOfObjects > 1 )
	{
		if ( Item[pObj->usItem].usItemClass & IC_AMMO )
		{
			UINT16 shots_first	= (*pObj)[0]->data.ubShotsLeft;
			(*pObj)[0]->data.ubShotsLeft = (*pObj)[pObj->ubNumberOfObjects - 1]->data.ubShotsLeft;
			(*pObj)[pObj->ubNumberOfObjects - 1]->data.ubShotsLeft = shots_first;
		}
		else if ( Item[pObj->usItem].usItemClass & IC_MAPFILTER_KIT )
		{
			INT16 status_first = (*pObj)[0]->data.objectStatus;
			(*pObj)[0]->data.objectStatus = (*pObj)[pObj->ubNumberOfObjects - 1]->data.objectStatus;
			(*pObj)[pObj->ubNumberOfObjects - 1]->data.objectStatus = status_first;
		}
	}
}

UINT8 FindNextMercInTeamPanel( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs )
{
	INT32 cnt;
	INT32 bFirstID;
	SOLDIERTYPE *pTeamSoldier;


	bFirstID = GetTeamSlotFromPlayerID( pSoldier->ubID );

	if ( bFirstID == -1 )
	{
		return( pSoldier->ubID );
	}

	for ( cnt = ( bFirstID + 1 ); cnt < gGameOptions.ubSquadSize; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			// Set Id to close
			pTeamSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			if ( fOnlyRegularMercs )
			{
				if ( pTeamSoldier->bActive && ( AM_AN_EPC( pTeamSoldier ) || AM_A_ROBOT( pTeamSoldier ) ) )
				{
					continue;
				}
			}

			if ( fGoodForLessOKLife )
			{
				if ( pTeamSoldier->stats.bLife > 0 && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bTeam == gbPlayerNum && pTeamSoldier->bAssignment < ON_DUTY	&& OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
			else
			{
				if ( OK_CONTROLLABLE_MERC( pTeamSoldier) && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
		}
	}


	if ( ( gusSelectedSoldier != NOBODY ) && ( gGameSettings.fOptions[ TOPTION_SPACE_SELECTS_NEXT_SQUAD ] ) )
	{
		// only allow if nothing in hand and if in SM panel, the Change Squad button must be enabled
		if ( ( ( gsCurInterfacePanel != TEAM_PANEL ) || ( ButtonList[ iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
		{
			SOLDIERTYPE *pNewSoldier;
			INT32		iCurrentSquad;

			//Select next squad
			iCurrentSquad = CurrentSquad( );

			pNewSoldier = FindNextActiveSquad( MercPtrs[ gusSelectedSoldier ] );

			if ( pNewSoldier->bAssignment != iCurrentSquad )
			{
				if ( gGameExternalOptions.fUseXMLSquadNames && pNewSoldier->bAssignment < min(ON_DUTY, (INT8)gSquadNameVector.size()) )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_SQUAD_ACTIVE_STRING], gSquadNameVector[pNewSoldier->bAssignment].c_str() );
				else
					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( CurrentSquad( ) + 1 ) );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( pNewSoldier->bAssignment + 1 ) );

				return( pNewSoldier->ubID );
			}
		}
	}

	// none found,
	// Now loop back
	for ( cnt = 0; cnt < bFirstID; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			pTeamSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			if ( fOnlyRegularMercs )
			{
				if ( pTeamSoldier->bActive && ( AM_AN_EPC( pTeamSoldier ) || AM_A_ROBOT( pTeamSoldier ) ) )
				{
					continue;
				}
			}

			if ( fGoodForLessOKLife )
			{
				if ( pTeamSoldier->stats.bLife > 0 && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bTeam == gbPlayerNum && pTeamSoldier->bAssignment < ON_DUTY	&& OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
			else
			{
				if ( OK_CONTROLLABLE_MERC( pTeamSoldier) && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
		}
	}

	// IF we are here, keep as we always were!
	return( pSoldier->ubID );
}


void HelpTextDoneCallback( void )
{
	gfRerenderInterfaceFromHelpText = TRUE;

	//To redraw the Store Keeper Interface
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}


void DisableTacticalTeamPanelButtons( BOOLEAN fDisable )
{
	gfDisableTacticalPanelButtons = fDisable;
}


void BeginKeyPanelFromKeyShortcut( )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sStartYPosition = 0;
	INT16 sWidth = 0, sHeight = 0;

	if ( gsCurInterfacePanel != SM_PANEL )
	{
		return;
	}

	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	if ( gfInKeyRingPopup )
	{
		return;
	}


	sStartYPosition = INV_INTERFACE_START_Y;
	sWidth = SCREEN_WIDTH;
	sHeight = ( SCREEN_HEIGHT - ( INV_INTERFACE_START_Y ) );
	pSoldier = gpSMCurrentMerc;

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	//CHRISL: In OIV mode, we don't want to offset the keyring popup.
	if(UsingNewInventorySystem() == false)
		InitKeyRingPopup( pSoldier, 0, sStartYPosition, sWidth, sHeight );
	else
		InitKeyRingPopup( pSoldier, 35, sStartYPosition, sWidth, sHeight );
}


void KeyRingItemPanelButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sStartYPosition = 0;
	INT16 sWidth = 0, sHeight = 0;

	if( guiCurrentScreen == MAP_SCREEN )
	{
		if( bSelectedInfoChar == -1 )
		{
			return;
		}

		if( gCharactersList[ bSelectedInfoChar ].fValid == FALSE )
		{
			return;
		}




		pSoldier = &( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] );
		sStartYPosition = MAP_START_KEYRING_Y;
		sWidth = 261;
		sHeight = ( 359 - 107 );
	}
	else
	{
		if ( gpSMCurrentMerc == NULL )
		{
			return;
		}

		sStartYPosition = INV_INTERFACE_START_Y;
		sWidth = SCREEN_WIDTH;
		sHeight = ( SCREEN_HEIGHT - ( INV_INTERFACE_START_Y ) );
		pSoldier = gpSMCurrentMerc;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( guiCurrentScreen == MAP_SCREEN )
		{
			// want the inv done button shutdown and the region behind the keyring shaded
			//ForceButtonUnDirty( giMapInvDoneButton );
			// shade the background
			ShadowVideoSurfaceRect( FRAME_BUFFER , 0, 107, 261, 359 );
			InvalidateRegion( 0, 107, 261, 359 );
		}

		//CHRISL: In OIV mode, we don't want to offset the keyring popup.
		if(UsingNewInventorySystem() == false)
			InitKeyRingPopup( pSoldier, 0, sStartYPosition, sWidth, sHeight );
		else
			InitKeyRingPopup( pSoldier, 35, sStartYPosition, sWidth, sHeight );
	}
}

void KeyRingSlotInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32 uiKeyRing;
	// Copyies of values
	UINT16	usOldItemIndex;
	static BOOLEAN	fRightDown = FALSE;
	static BOOLEAN	fLeftDown = FALSE;
	INT32 iNumberOfKeysTaken = 0;

	uiKeyRing = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	//if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	//{
	//	fLeftDown = TRUE;
	//}
	//else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLeftDown )
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLeftDown = FALSE;

		//if we are in the shop keeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			INVENTORY_IN_SLOT InvSlot;

			if( gMoveingItem.sItemIndex == 0 )
			{
				//Delete the contents of the item cursor
				gMoveingItem.initialize();
			}
			else
			{
				// Return if empty
				//if ( gpSMCurrentMerc->inv[ uiHandPos ].exists() == false )
				//	return;


				// Fill out the inv slot for the item
				//InvSlot.sItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
//			InvSlot.ubNumberOfItems = gpSMCurrentMerc->inv[ uiHandPos ].ubNumberOfObjects;
//			InvSlot.ubItemQuality = gpSMCurrentMerc->inv[ uiHandPos ].gun.bGunStatus;
				//InvSlot.ItemObject = gpSMCurrentMerc->inv[ uiHandPos ];
				//InvSlot.ubLocationOfObject = PLAYERS_INVENTORY;

				//InvSlot.ubIdOfMercWhoOwnsTheItem = gpSMCurrentMerc->ubProfile;


				//Add the item to the Players Offer Area
				//AddItemToPlayersOfferArea( gpSMCurrentMerc->ubProfile, &InvSlot, (UINT8)uiHandPos );

				// Dirty
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
			return;
		}

		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{
			// Return if empty
			if( ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER ) || ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubNumber == 0 ) )
				return;

			// If our flags are set to do this, gofoit!
			if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
			}
			else
			{
				if ( gpItemPopupSoldier->ubID != gusSelectedSoldier )
				{
					SelectSoldier( gpItemPopupSoldier->ubID, FALSE, FALSE );
				}
			}

			usOldItemIndex =	( UINT16 )uiKeyRing ;

			BeginKeyRingItemPointer( gpItemPopupSoldier, (UINT8)usOldItemIndex );
			//BeginItemPointer( gpSMCurrentMerc, (UINT8)uiHandPos );

		}
		else
		{
			UINT8			ubSrcID, ubDestID;
			BOOLEAN		fOKToGo = FALSE;
			BOOLEAN		fDeductPoints = FALSE;

			// WANNE: Instead of checking the item number for a key, check the itemclass!!!
			if (Item[gpItemPointer->usItem].usItemClass != IC_KEY)
			{
				return;
			}

			// ATE: OK, get source, dest guy if different... check for and then charge appropriate APs
			ubSrcID	= ( UINT8 )gCharactersList[ bSelectedInfoChar ].usSolID;
			if ( gpItemPointerSoldier )
			{
				ubDestID = gpItemPointerSoldier->ubID;
			}
			else
			{
				ubDestID = ubSrcID;
			}

			if ( ubSrcID == ubDestID )
			{
				// We are doing this ourselve, continue
				fOKToGo = TRUE;
			}
			else
			{
				// These guys are different....
				fDeductPoints = TRUE;

				// First check points for src guy
				if ( gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
				{
					if ( EnoughPoints( gpItemPointerSoldier, 2, 0, TRUE ) )
					{
						fOKToGo = TRUE;
					}
				}
				else
				{
					fOKToGo = TRUE;
				}

				// Should we go on?
				if ( fOKToGo )
				{
					if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
					{
						if ( EnoughPoints( gpSMCurrentMerc, 2, 0, TRUE ) )
						{
							fOKToGo = TRUE;
						}
						else
						{
							fOKToGo = FALSE;
						}
					}
				}
			}

			if ( fOKToGo )
			{
				//usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
				//usNewItemIndex = gpItemPointer->usItem;

				if ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER || gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == (*gpItemPointer)[0]->data.key.ubKeyID)
				{
					// Try to place here
					if ( ( iNumberOfKeysTaken = AddKeysToSlot( gpItemPopupSoldier, ( INT8 )uiKeyRing, gpItemPointer ) ) )
					{

						if ( fDeductPoints )
						{
							// Deduct points
							if ( gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
							{
								DeductPoints( gpItemPointerSoldier,	2, 0, UNTRIGGERED_INTERRUPT );
							}
							if ( gpItemPopupSoldier->stats.bLife >= CONSCIOUSNESS )
							{
								DeductPoints( gpItemPopupSoldier,	2, 0, UNTRIGGERED_INTERRUPT );
							}
						}

						// Dirty
						fInterfacePanelDirty = DIRTYLEVEL2;

						gpItemPointer->ubNumberOfObjects -= ( UINT8 )iNumberOfKeysTaken;

						// Check if it's the same now!
						if ( gpItemPointer->exists() == false )
						{
							if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
							{
								MAPEndItemPointer();
							}
							else
							{
								EndItemPointer();
							}
						}

						// Setup a timer....
						//guiMouseOverItemTime = GetJA2Clock( );
						//gfCheckForMouseOverItem = TRUE;
						//gbCheckForMouseOverItemPos = (INT8)uiHandPos;

					}
				}
				else
				{
					// Swap!
					SwapKeysToSlot( gpItemPopupSoldier, (INT8) uiKeyRing, gpItemPointer );

					if ( fDeductPoints )
					{
						// Deduct points
						if ( gpItemPointerSoldier && gpItemPointerSoldier->stats.bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpItemPointerSoldier,	2, 0, UNTRIGGERED_INTERRUPT );
						}
						if ( gpSMCurrentMerc->stats.bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpSMCurrentMerc,	2, 0, UNTRIGGERED_INTERRUPT );
						}
					}

					// Dirty
					fInterfacePanelDirty = DIRTYLEVEL2;

				}
			}
		}

		UpdateItemHatches();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		// Return if empty
		if( ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER ) || ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubNumber == 0 ) )
		{
			DeleteKeyRingPopup( );
			fTeamPanelDirty = TRUE;
			return;
		}
		// Some global stuff here - for esc, etc
		// Check for # of slots in item
		if ( !InItemDescriptionBox( ) )
		{
			if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
			{
				//InitKeyItemDescriptionBox( gpItemPopupSoldier, (UINT8)uiKeyRing, MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y, 0 );
		}
		else
		{
				InitKeyItemDescriptionBox( gpItemPopupSoldier, (UINT8)uiKeyRing, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, 0 );
		}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fRightDown = FALSE;
		fLeftDown = FALSE;
	}

}



void DisableSMPpanelButtonsWhenInShopKeeperInterface( BOOLEAN fDontDrawButtons )
{
	//Go through the buttons that will be under the ShopKeepers ATM panel and disable them
	DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
	DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
	DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
	DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
	DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
	DisableButton( iSMPanelButtons[ TALK_BUTTON ] );
	DisableButton( iSMPanelButtons[ MUTE_BUTTON ] );

	DisableButton( giSMStealthButton );

	//Make sure the options button is disabled
	DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );

	//Make sure the mapscreen button is disabled
	DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );

	//DisableButton
	DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
	DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
	DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
	DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
	DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
	DisableButton( iSMPanelButtons[ TALK_BUTTON ] );
	DisableButton( iSMPanelButtons[ MUTE_BUTTON ] );

	DisableButton( giSMStealthButton );
	RenderBackpackButtons(DISABLE_BUTTON);	/* CHRISL: Needed for new inventory backpack buttons */



	if( fDontDrawButtons )
	{
//ATM:
		//Go through the buttons that will be under the ShopKeepers ATM panel and disable them
		//DisableButton
		ButtonList[ iSMPanelButtons[ STANCEUP_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ CLIMB_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ STANCEDOWN_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;

		ButtonList[ giSMStealthButton ]->uiFlags &= ~BUTTON_DIRTY;
		// CHRISL: Backpack buttons for new inventory system
		if((UsingNewInventorySystem() == true))
		{
			ButtonList[ giSMZipperButton ]->uiFlags &= ~BUTTON_DIRTY;
			ButtonList[ giSMDropPackButton ]->uiFlags &= ~BUTTON_DIRTY;
		}


		//Make sure the options button is disabled
		ButtonList[ iSMPanelButtons[ OPTIONS_BUTTON ] ]->uiFlags &= ~BUTTON_ENABLED;

		//Make sure the mapscreen button is disabled
		ButtonList[ iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] ]->uiFlags &= ~BUTTON_ENABLED;
	
		//DisableButton
		ButtonList[ iSMPanelButtons[ STANCEUP_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ CLIMB_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ STANCEDOWN_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;

		ButtonList[ giSMStealthButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		// CHRISL: Backpack buttons for new inventory system
		if((UsingNewInventorySystem() == true))
		{
			ButtonList[ giSMZipperButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
			ButtonList[ giSMDropPackButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		}
	}
}


BOOLEAN IsMouseInRegion( MOUSE_REGION *pRegion )
{
	if ( (gusMouseXPos >= pRegion->RegionTopLeftX ) && (gusMouseXPos <= pRegion->RegionBottomRightX ) && (gusMouseYPos >= pRegion->RegionTopLeftY ) && (gusMouseYPos <= pRegion->RegionBottomRightY ) )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


void EnableButtonsForInItemBox( BOOLEAN fEnable )
{
	if ( !fEnable )
	{
		DisableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
		DisableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
		DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
		EnableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );

		if ( !gfDisableTacticalPanelButtons )
		{
			EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
		}
	}
}


void SMInvMoneyButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	else if (iReason == MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		//If the current merc is to far away, dont allow anything to be done
		if( gfSMDisableForItems )
			return;

		//if the player has an item in his hand,
		if( gpItemPointer != NULL )
		{
			//and the item is money
			if( Item[ gpItemPointer->usItem ].usItemClass == IC_MONEY )
			{
				CHAR16	zText[512];
				CHAR16	zMoney[64];

				// Make sure we go back to movement mode...
				guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				HandleTacticalUI( );

				swprintf( zMoney, L"%d", (*gpItemPointer)[0]->data.money.uiMoneyAmount );

				InsertCommasForDollarFigure( zMoney );
				InsertDollarSignInToString( zMoney );

				//ask the user if they are sure they want to deposit the money
				swprintf( zText, gzMoneyWithdrawMessageText[ CONFIRMATION_TO_DEPOSIT_MONEY_TO_ACCOUNT ], zMoney );

				if( guiCurrentScreen == SHOPKEEPER_SCREEN )
				{
					//if we are in the shop keeper interface, free the cursor
					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						FreeMouseCursor( TRUE );
					}

					DoMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, ConfirmationToDepositMoneyToPlayersAccount, NULL );
				}
				else
					DoMessageBox( MSG_BOX_BASIC_STYLE, zText, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, ConfirmationToDepositMoneyToPlayersAccount, NULL );
			}
		}

		//else bring up the money item description box to remove money from the players account
		else
		{
			//set the flag indicating we are removing money from the players account
			gfAddingMoneyToMercFromPlayersAccount = TRUE;

			//create the temp object from the players account balance
//			if( LaptopSaveInfo.iCurrentBalance > MAX_MONEY_PER_SLOT )
//				CreateMoney( MAX_MONEY_PER_SLOT, &gItemPointer );
//			else
				CreateMoney( LaptopSaveInfo.iCurrentBalance, &gItemPointer );

			InternalInitItemDescriptionBox( &gItemPointer, ITEMDESC_START_X, ITEMDESC_START_Y, 0, gpSMCurrentMerc );
		}
	}
}

void ConfirmationToDepositMoneyToPlayersAccount( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		//add the money to the players account
		AddTransactionToPlayersBook( MERC_DEPOSITED_MONEY_TO_PLAYER_ACCOUNT, gpSMCurrentMerc->ubProfile, GetWorldTotalMin(), (*gpItemPointer)[0]->data.money.uiMoneyAmount);

		// dirty shopkeeper
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

		EndItemPointer( );
	}
}


void ReEvaluateDisabledINVPanelButtons( )
{
	gfReEvaluateDisabledINVPanelButtons = TRUE;
}

void CheckForReEvaluateDisabledINVPanelButtons( )
{
	// OK, if we currently have an item pointer up....
	if ( gfReEvaluateDisabledINVPanelButtons )
	{
		if ( gpItemPointer || gfInItemPickupMenu )
		{
			EnableSMPanelButtons( FALSE, TRUE );
		}
		else
		{
			EnableSMPanelButtons( TRUE, TRUE );
		}

		gfReEvaluateDisabledINVPanelButtons = FALSE;
	}
}


void AbandonBoxingCallback( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		// ok, proceed!
		SetBoxingState( NOT_BOXING );
		gfEnteringMapScreen = TRUE;
	}
	// otherwise do nothing
}

void GoToMapScreenFromTactical( void )
{
	if ( gTacticalStatus.bBoxingState != NOT_BOXING )
	{
		// pop up dialogue asking whether the player wants to abandon the fight
		DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_ABANDON_FIGHT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, AbandonBoxingCallback, NULL );
		return;
	}
	// ok, proceed!
	gfMouseLockedOnBorder = FALSE;
	gfEnteringMapScreen = TRUE;
}

//----LEGION 2---------
BOOLEAN HandleKlerykPistolet( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, UINT16 usReplaceItem )
{
	BOOLEAN fRefuse = FALSE;

	if ( pSoldier->ubProfile == 57 )
	{
		if ( uiHandPos == HANDPOS || uiHandPos == SECONDHANDPOS )
		{
			if ( usReplaceItem == NOTHING )
			{
				fRefuse = TRUE;
			}
			else
			{	//legion
				// Do we have nothing or the leather vest or kevlar leather vest?
				if ( usReplaceItem == 4520 ) //z³oty pistolet items.xml
				{
					// This is good....
					fRefuse = FALSE;
				}
				else
				{
					fRefuse = TRUE;
				}
			}

			if ( fRefuse )
			{
				// Say quote!
				TacticalCharacterDialogue( pSoldier, 85 );
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


//-----------


//---------------------------------------------


// Moved to Weapons.cpp by ADB, rev 1513
/*void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem )
{
	// if in attached weapon mode and don't have weapon with GL attached in hand, reset weapon mode
	if ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )&& !IsGrenadeLauncherAttached( &(pSoldier->inv[ HANDPOS ]) ) )
	{
		if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
		{
			pSoldier->bWeaponMode = WM_NORMAL;
			pSoldier->bDoBurst = FALSE;
			pSoldier->bDoAutofire = 0;
		}
		else
		{
			pSoldier->bWeaponMode = WM_AUTOFIRE;
			pSoldier->bDoBurst = TRUE;
			pSoldier->bDoAutofire = 1;
		}
	}

	// if he is loaded tactically
	if ( pSoldier->bInSector )
	{
		// If this is our main hand
		if ( uiInvPos == HANDPOS || uiInvPos == SECONDHANDPOS )
		{
			// check if we need to change animation!
			pSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItem, usNewItem );
		}

		// if this is head gear
		if ( uiInvPos == HEAD1POS || uiInvPos == HEAD2POS )
		{
			// Could be because of GOGGLES change...	Re-create light...
			pSoldier->DeleteSoldierLight( );
			pSoldier->PositionSoldierLight( );
		}
	}
	else
	{
		// as a minimum
		if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass & IC_WEAPON) && GetShotsPerBurst(&pSoldier->inv[ HANDPOS ]) == 0 )
		{
			if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
			{
				pSoldier->bWeaponMode = WM_NORMAL;
				pSoldier->bDoBurst = FALSE;
				pSoldier->bDoAutofire = 0;
			}
			else
			{
				pSoldier->bWeaponMode = WM_AUTOFIRE;
				pSoldier->bDoAutofire = 1;
				pSoldier->bDoBurst = TRUE;
			}
		}
	}
}*/








