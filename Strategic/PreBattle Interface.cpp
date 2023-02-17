	#include "builddefines.h"
	#include <stdio.h>
	#include "PreBattle Interface.h"
	#include "Button System.h"
	#include "mousesystem.h"
	#include "Map Screen Interface.h"
	#include "jascreens.h"
	#include "gamescreen.h"
	#include "strategicmap.h"
	#include "Game Clock.h"
	#include "Music Control.h"
	#include "sysutil.h"
	#include "Font Control.h"
	#include "Queen Command.h"
	#include "Strategic Movement.h"
	#include "Strategic Pathing.h"
	#include "text.h"
	#include "PopUpBox.h"
	#include "Player Command.h"
	#include "Cursors.h"
	#include "Auto Resolve.h"
	#include "Sound Control.h"
	#include "english.h"
	#include "Map Screen Interface Bottom.h"
	#include "overhead.h"
	#include "Tactical Placement GUI.h"
	#include "Town Militia.h"
	#include "campaign.h"
	#include "GameSettings.h"
	#include "Random.h"
	#include "Creature Spreading.h"
	#include "Multi Language Graphic Utils.h"
	#include "Map Screen Helicopter.h"
	#include "Quests.h"
	#include "Map Screen Interface Border.h"
	#include "Strategic Status.h"
	#include "interface control.h"
	#include "Strategic Town Loyalty.h"
	#include "Squads.h"
	#include "Assignments.h"
	#include "Soldier macros.h"
	#include "history.h"
	#include "Cheats.h"
	// added by SANDRO
	#include "Tactical Save.h"
	#include "message.h"
	#include "CampaignStats.h"				// added by Flugente
	#include "militiasquads.h"				// added by Flugente
	#include "SkillCheck.h"					// added by Flugente
	#include "Strategic Transport Groups.h"

#ifdef JA2UB
#include "ub_config.h"
#endif

#include "GameInitOptionsScreen.h"

extern void InitializeTacticalStatusAtBattleStart();
extern BOOLEAN gfDelayAutoResolveStart;
extern BOOLEAN gfTransitionMapscreenToAutoResolve;
extern UILayout_Map UI_MAP;

#ifdef JA2BETAVERSION
extern BOOLEAN gfExitViewer;
#endif

extern BOOLEAN fMapScreenBottomDirty;

#include "connect.h"

BOOLEAN gfTacticalTraversal = FALSE;
GROUP *gpTacticalTraversalGroup = NULL;
SOLDIERTYPE *gpTacticalTraversalChosenSoldier = NULL;


BOOLEAN gfAutomaticallyStartAutoResolve = FALSE;
BOOLEAN gfAutoAmbush = FALSE;
BOOLEAN gfHighPotentialForAmbush = FALSE;
BOOLEAN gfGotoSectorTransition = FALSE;
BOOLEAN gfEnterAutoResolveMode = FALSE;
BOOLEAN gfEnteringMapScreenToEnterPreBattleInterface = FALSE;
BOOLEAN gfIgnoreAllInput = TRUE;

BOOLEAN gfZoomDone = FALSE;

enum //GraphicIDs for the panel
{
	MAINPANEL,
	TITLE_BAR_PIECE,
	TOP_COLUMN,
	BOTTOM_COLUMN,
	UNINVOLVED_HEADER,
	BOTTOM_LINE,
	BOTTOM_END
};

// WDS - make number of mercenaries, etc. be configurable
//The height of each row
#define ROW_HEIGHT				10
//The start of the black space
#define TOP_Y							113
//The end of the black space
//#define BOTTOM_Y					(349+(OUR_TEAM_SIZE_NO_VEHICLE-18)*ROW_HEIGHT)
#define BOTTOM_Y					349
//The internal height of the uninvolved panel
#define INTERNAL_HEIGHT		27
//The actual height of the uninvolved panel
#define ACTUAL_HEIGHT			24

BOOLEAN gfDisplayPotentialRetreatPaths = FALSE;
UINT16 gusRetreatButtonLeft, gusRetreatButtonTop, gusRetreatButtonRight, gusRetreatButtonBottom;

GROUP *gpBattleGroup = NULL;

void AutoResolveBattleCallback( GUI_BUTTON *btn, INT32 reason );
void GoToSectorCallback( GUI_BUTTON *btn, INT32 reason );
void RetreatMercsCallback( GUI_BUTTON *btn, INT32 reason );

void GetSoldierConditionInfo( SOLDIERTYPE *pSoldier, STR16 szCondition, UINT8 *pubHPPercent, UINT8 *pubBPPercent );

void CheckForRobotAndIfItsControlled( void );

void PutNonSquadMercsInBattleSectorOnSquads( BOOLEAN fExitVehicles );
void PutNonSquadMercsInPlayerGroupOnSquads( GROUP *pGroup, BOOLEAN fExitVehicles );

/*
void InvolvedMoveCallback( MOUSE_REGION *reg, INT32 reason );
void InvolvedClickCallback( MOUSE_REGION *reg, INT32 reason );
void UninvolvedMoveCallback( MOUSE_REGION *reg, INT32 reason );
void UninvolvedClickCallback( MOUSE_REGION *reg, INT32 reason );

SOLDIERTYPE* InvolvedSoldier( INT32 index );
SOLDIERTYPE* UninvolvedSoldier( INT32 index );
*/


MOUSE_REGION PBInterfaceBlanket;
BOOLEAN gfPreBattleInterfaceActive = FALSE;
UINT32 iPBButton[3] = {0,0,0};
UINT32 iPBButtonImage[3] = {0,0,0};
// WDS Graphics bug die to uninitialized variable
UINT32 uiInterfaceImages = 0;
BOOLEAN gfRenderPBInterface = FALSE;
BOOLEAN	gfPBButtonsHidden = FALSE;
BOOLEAN fDisableMapInterfaceDueToBattle = FALSE;

void DoTransitionFromMapscreenToPreBattleInterface();

BOOLEAN gfBlinkHeader = FALSE;

// mouse regions in mapscreen proper than must have thier help text disabled then re-enabled
extern MOUSE_REGION gMapStatusBarsRegion;
extern MOUSE_REGION gCharInfoFaceRegion;
extern MOUSE_REGION gCharInfoHandRegion;

extern INT32 giMapContractButton;
extern INT32 giCharInfoButton[ 2 ];
extern void CreateDestroyMapCharacterScrollButtons( void );
extern void UpdateTheStateOfTheNextPrevMapScreenCharacterButtons( void );

// were we showing the mapscreen inventory when the prebattle interface came up
extern BOOLEAN fShowInventoryFlag;

UINT32 guiNumInvolved = 0;
UINT32 guiNumUninvolved = 0;

//SAVE START

//Using the ESC key in the PBI will get rid of the PBI and go back to mapscreen, but
//only if the PBI isn't persistant (!gfPersistantPBI).
BOOLEAN gfPersistantPBI = FALSE;

//Contains general information about the type of encounter the player is faced with.	This
//determines whether or not you can autoresolve the battle or even retreat.	This code
//dictates the header that is used at the top of the PBI.
UINT8 gubEnemyEncounterCode = NO_ENCOUNTER_CODE;

UINT8 GetEnemyEncounterCode() {	return gubEnemyEncounterCode;}
void SetEnemyEncounterCode( UINT8 aCode ) { gubEnemyEncounterCode = aCode; }

//The autoresolve during tactical battle option needs more detailed information than the
//gubEnemyEncounterCode can provide.	The explicit version contains possibly unique codes
//for reasons not normally used in the PBI.	For example, if we were fighting the enemy
//in a normal situation, then shot at a civilian, the civilians associated with the victim
//would turn hostile, which would disable the ability to autoresolve the battle.
BOOLEAN gubExplicitEnemyEncounterCode = NO_ENCOUNTER_CODE;

UINT8 GetExplicitEnemyEncounterCode() { return gubExplicitEnemyEncounterCode; }
void SetExplicitEnemyEncounterCode( UINT8 aCode ) { gubExplicitEnemyEncounterCode = aCode; }

BOOLEAN gubSpecialEncounterCodeForEnemyHeli = FALSE;

//Location of the current battle (determines where the animated icon is blitted) and if the
//icon is to be blitted.
BOOLEAN gfBlitBattleSectorLocator = FALSE;

extern UINT8 gubPBSectorX = 0;
extern UINT8 gubPBSectorY = 0;
extern UINT8 gubPBSectorZ = 0;

BOOLEAN gfCantRetreatInPBI = FALSE;
//SAVE END

BOOLEAN gfUsePersistantPBI = FALSE;

INT32 giHilitedInvolved = 0;
INT32 giHilitedUninvolved = 0;

extern void CalculateGroupRetreatSector( GROUP *pGroup );

extern void GetMapscreenMercAssignmentString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
extern void GetMapscreenMercLocationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
extern void GetMapscreenMercDestinationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
extern void GetMapscreenMercDepartureString( SOLDIERTYPE *pSoldier, CHAR16 sString[], UINT8 *pubFontColor );



#ifdef JA2BETAVERSION
//The group is passed so we can extract the sector location
void ValidateAndCorrectInBattleCounters( GROUP *pLocGroup )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT8 ubSectorID;
	UINT8 ubInvalidGroups = 0;

	if( !pLocGroup->ubSectorZ )
	{
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->usGroupTeam != OUR_TEAM )
			{
				if( pGroup->ubSectorX == pLocGroup->ubSectorX && pGroup->ubSectorY == pLocGroup->ubSectorY )
				{
					if ( pGroup->pEnemyGroup->ubAdminsInBattle || pGroup->pEnemyGroup->ubTroopsInBattle || pGroup->pEnemyGroup->ubElitesInBattle || pGroup->pEnemyGroup->ubTanksInBattle || pGroup->pEnemyGroup->ubJeepsInBattle || pGroup->pEnemyGroup->ubRobotsInBattle )
					{
						++ubInvalidGroups;
						pGroup->pEnemyGroup->ubAdminsInBattle = 0;
						pGroup->pEnemyGroup->ubTroopsInBattle = 0;
						pGroup->pEnemyGroup->ubElitesInBattle = 0;
						pGroup->pEnemyGroup->ubTanksInBattle = 0;
						pGroup->pEnemyGroup->ubJeepsInBattle = 0;
						pGroup->pEnemyGroup->ubRobotsInBattle = 0;
					}
				}
			}
			pGroup = pGroup->next;
		}
	}

	ubSectorID = (UINT8)SECTOR( pLocGroup->ubSectorX, pLocGroup->ubSectorY );
	pSector = &SectorInfo[ ubSectorID ];

	if( ubInvalidGroups || pSector->ubAdminsInBattle || pSector->ubTroopsInBattle || pSector->ubElitesInBattle || pSector->ubCreaturesInBattle )
	{
		CHAR16 str[ 512 ];
		swprintf( str, L"Strategic info warning: Sector 'in battle' counters are not clear when they should be.	"
									L"If you can provide information on how a previous battle was resolved here or nearby patrol "
									L"(auto resolve, tactical battle, cheat keys, or retreat),"
									L"please forward that info (no data files necessary) as well as the following code (very important):	"
									L"G(%02d:%c%d_b%d) A(%02d:%02d) T(%02d:%02d) E(%02d:%02d) C(%02d:%02d) Ta(%02d:%02d) J(%02d:%02d) R(%02d:%02d)",
									ubInvalidGroups, pLocGroup->ubSectorY + 'A' - 1, pLocGroup->ubSectorX, pLocGroup->ubSectorZ,
									pSector->ubNumAdmins, pSector->ubAdminsInBattle,
									pSector->ubNumTroops, pSector->ubTroopsInBattle,
									pSector->ubNumElites, pSector->ubElitesInBattle,
									pSector->ubNumCreatures, pSector->ubCreaturesInBattle,
									pSector->ubNumTanks, pSector->ubTanksInBattle,
									pSector->ubNumJeeps, pSector->ubJeepsInBattle,
									pSector->ubNumRobots, pSector->ubRobotsInBattle );
		DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubCreaturesInBattle = 0;
		pSector->ubTanksInBattle = 0;
		pSector->ubJeepsInBattle = 0;
		pSector->ubRobotsInBattle = 0;
	}
}
#endif

FLOAT gAmbushRadiusModifier = 1.0f;

void InitPreBattleInterface( GROUP *pBattleGroup, BOOLEAN fPersistantPBI )
{
	VOBJECT_DESC	VObjectDesc;
	INT32 i;
	UINT8 ubGroupID = 0;
	UINT8 ubNumStationaryEnemies = 0;
	UINT8 ubNumMobileEnemies = 0;
	UINT8 ubNumMercs;
	BOOLEAN fUsePluralVersion = FALSE;
	INT8	bBestExpLevel = 0;
	BOOLEAN fRetreatAnOption = TRUE;
	SECTORINFO *pSector;
	BOOLEAN fScoutPresent = FALSE;	// Added by SANDRO
	BOOLEAN fAirDrop = FALSE;		// Added by Flugente
	UINT16  usDeploymentLeadership = 0;

	gAmbushRadiusModifier = 0.0f;

	// ARM: Feb01/98 - Cancel out of mapscreen movement plotting if PBI subscreen is coming up
	if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia )
	{
		AbortMovementPlottingMode( );
	}

	if( gfPreBattleInterfaceActive )
		return;

	//CHRISL: If for some reason we're not looking at a valid sector, leave the preBattleInterface.
	if(	pBattleGroup != NULL && (pBattleGroup->ubSectorX < MINIMUM_VALID_X_COORDINATE ||
		pBattleGroup->ubSectorX > MAXIMUM_VALID_X_COORDINATE ||
		pBattleGroup->ubSectorY < MINIMUM_VALID_Y_COORDINATE ||
		pBattleGroup->ubSectorY > MAXIMUM_VALID_Y_COORDINATE ||
		pBattleGroup->ubSectorZ < MINIMUM_VALID_Z_COORDINATE ||
		pBattleGroup->ubSectorZ > MAXIMUM_VALID_Z_COORDINATE) )
	{
		return;
	}

	gfPersistantPBI = fPersistantPBI;

	if( gfPersistantPBI )
	{
		gfBlitBattleSectorLocator = TRUE;
		gfBlinkHeader = FALSE;

#ifdef JA2BETAVERSION
		if( pBattleGroup )
		{
			ValidateAndCorrectInBattleCounters( pBattleGroup );
		}
#endif

		//	InitializeTacticalStatusAtBattleStart();
		// CJC, Oct 5 98: this is all we should need from InitializeTacticalStatusAtBattleStart()
		if( GetEnemyEncounterCode() != BLOODCAT_AMBUSH_CODE && GetEnemyEncounterCode() != ENTERING_BLOODCAT_LAIR_CODE )
		{
			if ( !CheckFact( FACT_FIRST_BATTLE_FOUGHT, 0 ) )
			{
				SetFactTrue( FACT_FIRST_BATTLE_BEING_FOUGHT );
			}
		}

		//If we are currently in the AI Viewer development utility, then remove it first.	It automatically
		//returns to the mapscreen upon removal, which is where we want to go.
#ifdef JA2BETAVERSION
		if( guiCurrentScreen == AIVIEWER_SCREEN )
		{
			gfExitViewer = TRUE;
			gpBattleGroup = pBattleGroup;
			gfEnteringMapScreen = TRUE;
			gfEnteringMapScreenToEnterPreBattleInterface = TRUE;
			gfUsePersistantPBI = TRUE;
			return;
		}
#endif

		// ATE: Added check for fPersistantPBI = TRUE if pBattleGroup == NULL
		// Searched code and saw that this condition only happens for creatures
			// fixing a bug
		//if( guiCurrentScreen == GAME_SCREEN && pBattleGroup )
		if( guiCurrentScreen == GAME_SCREEN && ( pBattleGroup || fPersistantPBI ) )
		{
			gpBattleGroup = pBattleGroup;
			gfEnteringMapScreen = TRUE;
			gfEnteringMapScreenToEnterPreBattleInterface = TRUE;
			gfUsePersistantPBI = TRUE;
			return;
		}

		if( gfTacticalTraversal && (pBattleGroup == gpTacticalTraversalGroup || gbWorldSectorZ > 0) )
		{
			return;
		}

		// reset the help text for mouse regions
		SetRegionFastHelpText( &gCharInfoHandRegion, L"" );
		SetRegionFastHelpText( &gCharInfoFaceRegion, L"" );
		SetRegionFastHelpText( &gMapStatusBarsRegion, L"" );

		gfDisplayPotentialRetreatPaths = FALSE;

	}

	gpBattleGroup = pBattleGroup;

	//calc sector values
	if( gpBattleGroup )
	{
		gubPBSectorX = gpBattleGroup->ubSectorX;
		gubPBSectorY = gpBattleGroup->ubSectorY;
		gubPBSectorZ = gpBattleGroup->ubSectorZ;

		// get number of enemies thought to be here
		SectorInfo[SECTOR( gubPBSectorX, gubPBSectorY )].bLastKnownEnemies = NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, ENEMY_TEAM );
		fMapPanelDirty = TRUE;
	}
	else if( gfPersistantPBI )
	{
		gubPBSectorX = (UINT8)SECTORX( gubSectorIDOfCreatureAttack );
		gubPBSectorY = (UINT8)SECTORY( gubSectorIDOfCreatureAttack );
		gubPBSectorZ = 0;
	}

	pSector = &SectorInfo[ SECTOR( gubPBSectorX, gubPBSectorY ) ];

	if( !gfPersistantPBI )
	{
		//calculate the non-persistant situation
		gfBlinkHeader = TRUE;

		if( HostileCiviliansPresent() )
		{
			//There are hostile civilians, so no autoresolve allowed.
			SetExplicitEnemyEncounterCode( HOSTILE_CIVILIANS_CODE );
		}
		else if ( GetEnemyEncounterCode() != BLOODCAT_ATTACK_CODE && HostileBloodcatsPresent() )
		{
			//There are bloodcats in the sector, so no autoresolve allowed
			SetExplicitEnemyEncounterCode( HOSTILE_BLOODCATS_CODE );
		}
		else if( gbWorldSectorZ > 0 )
		{
			UNDERGROUND_SECTORINFO *pUnderGroundSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			Assert( pUnderGroundSector );

			//We are underground, so no autoresolve allowed
			if( pUnderGroundSector->ubCreaturesInBattle )
			{
				SetExplicitEnemyEncounterCode( FIGHTING_CREATURES_CODE );
			}
			else if( pUnderGroundSector->ubAdminsInBattle || pUnderGroundSector->ubTroopsInBattle || pUnderGroundSector->ubElitesInBattle || pUnderGroundSector->ubTanksInBattle || pUnderGroundSector->ubJeepsInBattle )
			{
				SetExplicitEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE );
			}
		}
		else if ( pBattleGroup && pBattleGroup->usGroupTeam != OUR_TEAM && NumNonPlayerTeamMembersInSector( pBattleGroup->ubSectorX, pBattleGroup->ubSectorY, MILITIA_TEAM ) > 0 )
		{
			SetEnemyEncounterCode( pBattleGroup->usGroupTeam == ENEMY_TEAM && pBattleGroup->pEnemyGroup->ubIntention == TRANSPORT ? TRANSPORT_INTERCEPT_CODE : ENEMY_ENCOUNTER_CODE );
		}
		else if( GetEnemyEncounterCode() == ENTERING_ENEMY_SECTOR_CODE ||
			GetEnemyEncounterCode() == ENEMY_ENCOUNTER_CODE ||
			GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE ||
			GetEnemyEncounterCode() == ENEMY_INVASION_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE ||
			GetEnemyEncounterCode() == ENTERING_BLOODCAT_LAIR_CODE ||
			GetEnemyEncounterCode() == CREATURE_ATTACK_CODE ||
			GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE ||
			GetEnemyEncounterCode() == ENEMY_INVASION_AIRDROP_CODE ||
			GetEnemyEncounterCode() == CONCEALINSERTION_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
			GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BANDIT_ATTACK_CODE ||
			GetEnemyEncounterCode() == TRANSPORT_INTERCEPT_CODE )
		{
			//use same code
			SetExplicitEnemyEncounterCode( GetEnemyEncounterCode() );
		}
		else
		{
			#ifdef JA2BETAVERSION
				DoScreenIndependantMessageBox( L"Can't determine valid reason for battle indicator.	Please try to provide information as to when and why this indicator first appeared and send whatever files that may help.", MSG_BOX_FLAG_OK, NULL );
			#endif
			gfBlitBattleSectorLocator = FALSE;
			return;
		}
	}

	// silversurfer: moved this up here because this check needs to be done before we draw anything on the map screen - otherwise CTD.
	// Also the necessary variables have to be set.
	//If we are currently in tactical, then set the flag to automatically bring up the mapscreen.
	if( guiCurrentScreen == GAME_SCREEN )
	{
		gfEnteringMapScreen = TRUE;
		gfEnteringMapScreenToEnterPreBattleInterface = TRUE;
		return;
	}

	fMapScreenBottomDirty = TRUE;
	ChangeSelectedMapSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
	// Headrock: Added FALSE argument, We might need TRUE but not sure. Will need to initiate battle :)
	RenderMapScreenInterfaceBottom( FALSE );

	if( !fShowTeamFlag )
	{
		ToggleShowTeamsMode();
	}

	//Define the blanket region to cover all of the other regions used underneath the panel.
	MSYS_DefineRegion( &PBInterfaceBlanket, 0 + xResOffset, 0 + yResOffset, 261 + xResOffset, 359 + yResOffset, MSYS_PRIORITY_HIGHEST - 5, 0, 0, 0 );

	//Create the panel
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_PREBATTLEPANEL );
	if( !AddVideoObject( &VObjectDesc, &uiInterfaceImages ) )
		AssertMsg( 0, "Failed to load interface\\PreBattlePanel.sti" );

	//Create the 3 buttons
	iPBButtonImage[0] = LoadButtonImage( "INTERFACE\\PreBattleButton.sti", -1, 0, -1, 1, -1 );
	if( iPBButtonImage[ 0 ] == -1 )
		AssertMsg( 0, "Failed to load interface\\PreBattleButton.sti" );
	iPBButtonImage[1] = UseLoadedButtonImage( iPBButtonImage[ 0 ], -1, 0, -1, 1, -1 );
	iPBButtonImage[2] = UseLoadedButtonImage( iPBButtonImage[ 0 ], -1, 0, -1, 1, -1 );
	iPBButton[0] = QuickCreateButton( iPBButtonImage[0], 27 + xResOffset, 54 + yResOffset, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 2, DEFAULT_MOVE_CALLBACK, AutoResolveBattleCallback );
	iPBButton[1] = QuickCreateButton( iPBButtonImage[1], 98 + xResOffset, 54 + yResOffset, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 2, DEFAULT_MOVE_CALLBACK, GoToSectorCallback );
	iPBButton[2] = QuickCreateButton( iPBButtonImage[2], 169 + xResOffset, 54 + yResOffset, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 2, DEFAULT_MOVE_CALLBACK, RetreatMercsCallback );

	SpecifyGeneralButtonTextAttributes( iPBButton[0], gpStrategicString[ STR_PB_AUTORESOLVE_BTN ], BLOCKFONT, FONT_BEIGE, 141 );
	SpecifyGeneralButtonTextAttributes( iPBButton[1], gpStrategicString[ STR_PB_GOTOSECTOR_BTN ], BLOCKFONT, FONT_BEIGE, 141 );
	SpecifyGeneralButtonTextAttributes( iPBButton[2], gpStrategicString[ STR_PB_RETREATMERCS_BTN ], BLOCKFONT, FONT_BEIGE, 141 );
	SpecifyButtonHilitedTextColors( iPBButton[0], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonHilitedTextColors( iPBButton[1], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonHilitedTextColors( iPBButton[2], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonTextOffsets( iPBButton[0], 8, 7, TRUE );
	SpecifyButtonTextOffsets( iPBButton[1], 8, 7, TRUE );
	SpecifyButtonTextOffsets( iPBButton[2], 8, 7, TRUE );
	SpecifyButtonTextWrappedWidth( iPBButton[0], 51 );
	SpecifyButtonTextWrappedWidth( iPBButton[1], 51 );
	SpecifyButtonTextWrappedWidth( iPBButton[2], 51 );
	SpecifyButtonTextJustification( iPBButton[0], BUTTON_TEXT_CENTER );
	SpecifyButtonTextJustification( iPBButton[1], BUTTON_TEXT_CENTER );
	SpecifyButtonTextJustification( iPBButton[2], BUTTON_TEXT_CENTER );
	AllowDisabledButtonFastHelp( iPBButton[0], TRUE );
	AllowDisabledButtonFastHelp( iPBButton[1], TRUE );
	AllowDisabledButtonFastHelp( iPBButton[2], TRUE );

	gusRetreatButtonLeft		= ButtonList[ iPBButton[2] ]->Area.RegionTopLeftX;
	gusRetreatButtonTop			= ButtonList[ iPBButton[2] ]->Area.RegionTopLeftY;
	gusRetreatButtonRight		= ButtonList[ iPBButton[2] ]->Area.RegionBottomRightX;
	gusRetreatButtonBottom	= ButtonList[ iPBButton[2] ]->Area.RegionBottomRightY;

	SetButtonCursor(iPBButtonImage[ 0 ], MSYS_NO_CURSOR );
	SetButtonCursor(iPBButtonImage[ 1 ], MSYS_NO_CURSOR );
	SetButtonCursor(iPBButtonImage[ 2 ], MSYS_NO_CURSOR );

	HideButton( iPBButton[0] );
	HideButton( iPBButton[1] );
	HideButton( iPBButton[2] );
	gfPBButtonsHidden = TRUE;

	// ARM: this must now be set before any calls utilizing the GetCurrentBattleSectorXYZ() function
	gfPreBattleInterfaceActive = TRUE;

	CheckForRobotAndIfItsControlled();

	// wake everyone up
	WakeUpAllMercsInSectorUnderAttack( );

	//Count the number of players involved or not involved in this battle
	guiNumUninvolved = 0;
	guiNumInvolved = 0;
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++i )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) )
		{
			if ( PlayerMercInvolvedInThisCombat( MercPtrs[ i ] ) )
			{
				// involved
				if( !ubGroupID )
				{
					//Record the first groupID.	If there are more than one group in this battle, we
					//can detect it by comparing the first value with future values.	If we do, then
					//we set a flag which determines whether to use the singular help text or plural version
					//for the retreat button.
					ubGroupID = MercPtrs[ i ]->ubGroupID;
					if( !gpBattleGroup )
						gpBattleGroup = GetGroup( ubGroupID );
					//if( bBestExpLevel > MercPtrs[ i ]->stats.bExpLevel ) // SANDRO - WTF!! This is a bug!
					if( bBestExpLevel < MercPtrs[ i ]->stats.bExpLevel ) // SANDRO - WTF!! This is a bug!
						bBestExpLevel = MercPtrs[ i ]->stats.bExpLevel;
					if( MercPtrs[ i ]->ubPrevSectorID == 255 )
					{ //Not able to retreat (calculate it for group)
						GROUP *pTempGroup;
						pTempGroup = GetGroup( ubGroupID );
						Assert( pTempGroup );
						CalculateGroupRetreatSector( pTempGroup );
					}
				}
				else if( ubGroupID != MercPtrs[ i ]->ubGroupID )
				{
					fUsePluralVersion = TRUE;
				}

				++guiNumInvolved;

				// SANDRO - added check if we have a scout in group, needed later
				if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( MercPtrs[ i ], SCOUTING_NT ) && gSkillTraitValues.fSCPreventsTheEnemyToAmbushMercs )
				{
					fScoutPresent = TRUE;
				}

				if ( MercPtrs[i]->usSoldierFlagMask & SOLDIER_AIRDROP )
				{
					fAirDrop = TRUE;
				}

				UINT16 deploymentleadership = EffectiveLeadership( MercPtrs[i] );
				FLOAT ambushradiusmodifier = 10 * EffectiveExpLevel( MercPtrs[i] ) + MercPtrs[i]->GetBackgroundValue( BG_AMBUSH_RADIUS );
				if ( gGameOptions.fNewTraitSystem )
				{
					deploymentleadership += 50 * NUM_SKILL_TRAITS( MercPtrs[i], SQUADLEADER_NT );

					ambushradiusmodifier += 50 * NUM_SKILL_TRAITS( MercPtrs[i], SCOUTING_NT );
				}
				// bonus with old traits, so that the check can be won
				else
				{
					deploymentleadership += 30;
				}

				usDeploymentLeadership = max( usDeploymentLeadership, deploymentleadership );

				gAmbushRadiusModifier = max( gAmbushRadiusModifier, ambushradiusmodifier / 100 );

				// Flugente: if a merc is inserted from concealed state, retreat is forbidden, as at least this merc will have to extract manually
				if ( MercPtrs[i]->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION )
					fRetreatAnOption = FALSE;
			}
			else
			{
				++guiNumUninvolved;
			}
		}
	}

	ubNumStationaryEnemies = NumStationaryEnemiesInSector( gubPBSectorX, gubPBSectorY );
	ubNumMobileEnemies = NumMobileEnemiesInSector( gubPBSectorX, gubPBSectorY );
	ubNumMercs = PlayerMercsInSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
	
	BOOLEAN fAmbushPrevented = FALSE;

	if( gfPersistantPBI )
	{
		if( !pBattleGroup )
		{
			if ( gubSpecialEncounterCodeForEnemyHeli )
			{
				if ( GetEnemyEncounterCode() != ENEMY_INVASION_AIRDROP_CODE )
					SetEnemyEncounterCode( ENEMY_INVASION_CODE);
			}
			else
			{
				//creature's attacking!
				switch ( guCreatureAttackType )
				{
				case CREATURE_ATTACK_TYPE_BLOODCAT:	
					SetEnemyEncounterCode( BLOODCAT_ATTACK_CODE );
					break;

				case CREATURE_ATTACK_TYPE_ZOMBIE:
					SetEnemyEncounterCode( ZOMBIE_ATTACK_CODE );
					break;

				case CREATURE_ATTACK_TYPE_BANDIT:
					SetEnemyEncounterCode( BANDIT_ATTACK_CODE );
					break;

				default:
					SetEnemyEncounterCode( CREATURE_ATTACK_CODE );
					break;
				}
			}
		}
		else if ( gpBattleGroup->usGroupTeam == OUR_TEAM )
		{
			if( GetEnemyEncounterCode() != BLOODCAT_AMBUSH_CODE && GetEnemyEncounterCode() != ENTERING_BLOODCAT_LAIR_CODE )
			{
				// Flugente: if the group that causes a battle is a result of a merc no longer being concealed, special code
				if ( gpBattleGroup->pPlayerList->pSoldier->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION )
				{
					SetEnemyEncounterCode( CONCEALINSERTION_CODE );
				}
				else if( ubNumStationaryEnemies )
				{
					SetEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE);
				}
				else
				{
					SetEnemyEncounterCode( ENEMY_ENCOUNTER_CODE );

					GROUP* pGroup = gpGroupList;
					BOOLEAN encounteredTransportGroup = FALSE;
					while (pGroup)
					{
						if (pGroup->usGroupTeam == ENEMY_TEAM && pGroup->pEnemyGroup->ubIntention == TRANSPORT && pGroup->ubSectorX == gpBattleGroup->ubSectorX && pGroup->ubSectorY == gpBattleGroup->ubSectorY && pGroup->ubSectorZ == gpBattleGroup->ubSectorZ)
						{
							encounteredTransportGroup = TRUE;
							break;
						}

						pGroup = pGroup->next;
					}
					if (encounteredTransportGroup)
					{
						SetEnemyEncounterCode( TRANSPORT_INTERCEPT_CODE );
					}
					// Flugente: no ambushes on an airdrop
					else if ( !fAirDrop )
					{
						//Don't consider ambushes until the player has reached 25% (normal) progress
						if( gfHighPotentialForAmbush )
						{
							if( Chance( 90 ) )
							{
								// SANDRO - Scout prevents ambushes no matter what
								if ( !fScoutPresent )
								{
									SetEnemyEncounterCode( ENEMY_AMBUSH_CODE );
								}
								else 
								{
									fAmbushPrevented = TRUE;
									if ( gSkillTraitValues.fSCThrowMessageIfAmbushPrevented )
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ENEMY_AMBUSH_PREVENTED] );
								}
							}
						}
						else if( gfAutoAmbush && ubNumMobileEnemies > ubNumMercs )
						{
							// SANDRO - Scout prevents ambushes no matter what
							if ( !fScoutPresent )
							{
								SetEnemyEncounterCode( ENEMY_AMBUSH_CODE );
							}
							else 
							{
								fAmbushPrevented = TRUE;
								if ( gSkillTraitValues.fSCThrowMessageIfAmbushPrevented )
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ENEMY_AMBUSH_PREVENTED] );
							}
						}
						// Madd:  
						// WANNE: Added an ja2_options.ini Property "ENABLE_CHANCE_OF_ENEMY_AMBUSHES_ON_INSANE_DIFFICULT"
						//////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - changed this a lot, now on any difficulty, the enemy can ambush your squad 
						// based on difficulty level, number of mercs/enemies and other aspects
						else if( gGameExternalOptions.fEnableChanceOfEnemyAmbushes )
						{ 					
							INT32 iChance;
							// Basic chance - progress level/2 minus highest merc exp level*2, and 10% on top
							iChance = (UINT8)( ((CurrentPlayerProgressPercentage() / 2 ) - bBestExpLevel*2 ) + 15 );

							if( pSector->uiFlags & SF_ENEMY_AMBUSH_LOCATION )
								iChance += 20;

							if( gfCantRetreatInPBI )
								iChance += 20;

							// adjust the chance for size of our squad
							if( ubNumMercs == 1 ) // one person is almost invisible to an army group, so reduce the chance a lot
								iChance -= 40;
							else if ( ubNumMercs == 2 ) // 2 persons are still hardly detectable
								iChance -= 25;
							else if ( ubNumMercs <= 10 ) // 3 to 10 mercs
								iChance -= (5 * (5 - ubNumMercs)); // -5% adjustment per merc (-10% to +25%)
							else // more than 10 mercs
								iChance += 30; // maximum of +30% per squad size

							// the more enemies are there the lesser the chance to be ambushed
							// (large groups of enemies can be seen from afar, so the chance is lesser)
							if ((ubNumMobileEnemies + ubNumStationaryEnemies) <= 2 )
								iChance += 20; // can make hunting retreated enemies easier
							else if ((ubNumMobileEnemies + ubNumStationaryEnemies) <= 6 ) // smaller groups actually increase the chance
								iChance += (3 * (6 - (ubNumMobileEnemies + ubNumStationaryEnemies))); // +3% adjustment per enemy
							else
								iChance -= (2 * ((ubNumMobileEnemies + ubNumStationaryEnemies) - 6)); // -2% adjustment per enemy beyond 6

							// adjust the chance for difficulty setting
							iChance = iChance + (zDiffSetting[gGameOptions.ubDifficultyLevel].iChanceOfEnemyAmbushes);

							// adjust the chance for what we know about the sector
							if( WhatPlayerKnowsAboutEnemiesInSector( gubPBSectorX, gubPBSectorY ) == KNOWS_NOTHING )
								iChance += 20;
							// HEADROCK HAM 5: Added new possible value...
							else if( WhatPlayerKnowsAboutEnemiesInSector( gubPBSectorX, gubPBSectorY ) == KNOWS_THEYRE_THERE ||
								WhatPlayerKnowsAboutEnemiesInSector( gubPBSectorX, gubPBSectorY ) == KNOWS_THEYRE_THERE_AND_WHERE_GOING )
								iChance += 5;
							//if( GetSectorFlagStatus( gubPBSectorX, gubPBSectorY, 0, SF_ALREADY_VISITED ) == TRUE )
							//	iChance -= 10; // if we already visited this sector

							// there is always a little chance
							if( iChance <= 0 )
								iChance = 1;
							
							// externalized modifier
							if( gGameExternalOptions.bChanceModifierEnemyAmbushes != 0 )
								iChance = ((iChance * (100 + gGameExternalOptions.bChanceModifierEnemyAmbushes)) / 100);

							if( (INT32)PreRandom( 100 ) < iChance )
							{
								if ( !fScoutPresent )
								{
									SetEnemyEncounterCode( ENEMY_AMBUSH_CODE );
								}
								else 
								{
									fAmbushPrevented = TRUE;
									if ( gSkillTraitValues.fSCThrowMessageIfAmbushPrevented )
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ENEMY_AMBUSH_PREVENTED] );
								}
							}
						}
					}

					// Flugente: improved ambush: if a real squadleader is present, we may deploy our mercs instead of having them being dropped into combat randomly
					if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE && usDeploymentLeadership > 120 )
					{
						SetEnemyEncounterCode( ENEMY_AMBUSH_DEPLOYMENT_CODE );
					}
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
		else
		{ //Are enemies invading a town, or just encountered the player.
			if (pBattleGroup && pBattleGroup->usGroupTeam == ENEMY_TEAM && pBattleGroup->pEnemyGroup->ubIntention == TRANSPORT)
				SetEnemyEncounterCode( TRANSPORT_INTERCEPT_CODE );
			else if( GetTownIdForSector( gubPBSectorX, gubPBSectorY ) )
				SetEnemyEncounterCode( ENEMY_INVASION_CODE );
			//SAM sites not in towns will also be considered to be important
			else if( pSector->uiFlags & SF_SAM_SITE )
				SetEnemyEncounterCode( ENEMY_INVASION_CODE );
			else
				SetEnemyEncounterCode( ENEMY_ENCOUNTER_CODE );
		}

		// haxx
		if ( pBattleGroup && pBattleGroup->usGroupTeam == OUR_TEAM && pBattleGroup->pPlayerList->pSoldier->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION )
		{
			//SetEnemyEncounterCode( CONCEALINSERTION_CODE );
		}
	}

	gfHighPotentialForAmbush = FALSE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - merc records - ambush experienced
	if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE || GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE || GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE || fAmbushPrevented )
	{
		for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++i )
		{
			if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) )
			{
				if ( PlayerMercInvolvedInThisCombat( MercPtrs[ i ] ) && MercPtrs[ i ]->ubProfile != NO_PROFILE )
				{
					if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE || GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE || GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
						gMercProfiles[ MercPtrs[ i ]->ubProfile ].records.usAmbushesExperienced++;
					else if ( fAmbushPrevented && HAS_SKILL_TRAIT( MercPtrs[ i ], SCOUTING_NT ) ) // Scouts actually get this as number of prevented ambushes
						gMercProfiles[ MercPtrs[ i ]->ubProfile ].records.usAmbushesExperienced++;
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( gfAutomaticallyStartAutoResolve )
	{
		DisableButton( iPBButton[1] );
		DisableButton( iPBButton[2] );
	}

	gfRenderPBInterface = TRUE;
	giHilitedInvolved = giHilitedUninvolved = -1;
	MSYS_SetCurrentCursor( CURSOR_NORMAL );
	StopTimeCompression();

	// hide all visible boxes
	HideAllBoxes( );
	fShowAssignmentMenu = FALSE;
	fShowContractMenu = FALSE;
	DisableTeamInfoPanels();
	if( ButtonList[ giMapContractButton ] )
	{
		HideButton( giMapContractButton );
	}
	if( ButtonList[ giCharInfoButton[ 0 ] ] )
	{
		HideButton( giCharInfoButton[ 0 ] );
	}
	if( ButtonList[ giCharInfoButton[ 1 ] ] )
	{
		HideButton( giCharInfoButton[ 1 ] );
	}
	HideButton( giMapContractButton );

	if( GetEnemyEncounterCode() == ENEMY_ENCOUNTER_CODE || GetEnemyEncounterCode() == TRANSPORT_INTERCEPT_CODE )
	{ //we know how many enemies are here, so until we leave the sector, we will continue to display the value.
		//the flag will get cleared when time advances after the fEnemyInSector flag is clear.

		// ALWAYS use these 2 statements together, without setting the boolean, the flag will never be cleaned up!
		pSector->uiFlags |= SF_PLAYER_KNOWS_ENEMIES_ARE_HERE;
		gfResetAllPlayerKnowsEnemiesFlags = TRUE;
	}

	//Set up fast help for buttons depending on the state of the button, and disable buttons
	//when necessary.
	if( gfPersistantPBI )
	{
		if( GetEnemyEncounterCode() == ENTERING_ENEMY_SECTOR_CODE ||
			GetEnemyEncounterCode() == ENTERING_BLOODCAT_LAIR_CODE )
		{ //Don't allow autoresolve for player initiated invasion battle types
			DisableButton( iPBButton[ 0 ] );
			SetButtonFastHelpText( iPBButton[ 0 ], gpStrategicString[ STR_PB_DISABLED_AUTORESOLVE_FASTHELP ] );
		}
		else if( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE ||
			GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
		{
			//Don't allow autoresolve for ambushes
			DisableButton( iPBButton[ 0 ] );
			SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 3 ] );
		}
		else if ( GetEnemyEncounterCode() == CONCEALINSERTION_CODE )
		{
			// Don't allow autoresolve
			DisableButton( iPBButton[0] );
			SetButtonFastHelpText( iPBButton[0], gpStrategicString[STR_PB_DISABLED_AUTORESOLVE_FASTHELP] );

			// No retreat possible, we must go to tactical
			fRetreatAnOption = FALSE;
		}
		else
		{
			SetButtonFastHelpText( iPBButton[ 0 ], gpStrategicString[ STR_PB_AUTORESOLVE_FASTHELP ] );
		}
		SetButtonFastHelpText( iPBButton[ 1 ], gpStrategicString[ STR_PB_GOTOSECTOR_FASTHELP ] );

		if( gfAutomaticallyStartAutoResolve )
		{
			DisableButton( iPBButton[ 1 ] );
		}

		if( gfCantRetreatInPBI )
		{
			gfCantRetreatInPBI = FALSE;
			fRetreatAnOption = FALSE;
		}

		if( gfAutomaticallyStartAutoResolve || !fRetreatAnOption ||
			GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE ||
			GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE ||
			GetEnemyEncounterCode() == CREATURE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
			GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BANDIT_ATTACK_CODE ||
			is_client)
		{
			DisableButton( iPBButton[ 2 ] );
			SetButtonFastHelpText( iPBButton[ 2 ], gzNonPersistantPBIText[ 9 ] );
		}
		else
		{
			if( !fUsePluralVersion )
			{
				SetButtonFastHelpText( iPBButton[ 2 ], gpStrategicString[ STR_BP_RETREATSINGLE_FASTHELP ] );
			}
			else
			{
				SetButtonFastHelpText( iPBButton[ 2 ], gpStrategicString[ STR_BP_RETREATPLURAL_FASTHELP ] );
			}
		}
	}
	else
	{ //use the explicit encounter code to determine what get's disable and the associated help text that is used.

		//First of all, the retreat button is always disabled seeing a battle is in progress.
		DisableButton( iPBButton[ 2 ] );
		SetButtonFastHelpText( iPBButton[ 2 ], gzNonPersistantPBIText[ 0 ] );
		SetButtonFastHelpText( iPBButton[ 1 ], gzNonPersistantPBIText[ 1 ] );
		switch( GetExplicitEnemyEncounterCode() )
		{
			case CREATURE_ATTACK_CODE:
			case ENEMY_ENCOUNTER_CODE:
			case TRANSPORT_INTERCEPT_CODE:
			case ENEMY_INVASION_CODE:
			case ENEMY_INVASION_AIRDROP_CODE:
			case BLOODCAT_ATTACK_CODE:
			case ZOMBIE_ATTACK_CODE:
			case BANDIT_ATTACK_CODE:
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 2 ] );
				break;
			case ENTERING_ENEMY_SECTOR_CODE:
				DisableButton( iPBButton[ 0 ] );
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 3 ] );
				break;
			case ENEMY_AMBUSH_CODE:
			case ENEMY_AMBUSH_DEPLOYMENT_CODE:
				DisableButton( iPBButton[ 0 ] );
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 4 ] );
				break;
			case FIGHTING_CREATURES_CODE:
				DisableButton( iPBButton[ 0 ] );
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 5 ] );
				break;
			case HOSTILE_CIVILIANS_CODE:
				DisableButton( iPBButton[ 0 ] );
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 6 ] );
				break;
			case HOSTILE_BLOODCATS_CODE:
			case BLOODCAT_AMBUSH_CODE:
			case ENTERING_BLOODCAT_LAIR_CODE:
				DisableButton( iPBButton[ 0 ] );
				SetButtonFastHelpText( iPBButton[ 0 ], gzNonPersistantPBIText[ 7 ] );
				break;
			case CONCEALINSERTION_CODE:
				// Don't allow autoresolve
				DisableButton( iPBButton[0] );
				SetButtonFastHelpText( iPBButton[0], gpStrategicString[STR_PB_DISABLED_AUTORESOLVE_FASTHELP] );

				// No retreat possible, we must go to tactical
				DisableButton( iPBButton[2] );
				SetButtonFastHelpText( iPBButton[2], gzNonPersistantPBIText[9] );
				break;
		}
	}

	//Disable the options button when the auto resolve	screen comes up
	EnableDisAbleMapScreenOptionsButton( FALSE );

	UseCreatureMusic(HostileZombiesPresent());
	
#ifdef NEWMUSIC
	GlobalSoundID  = MusicSoundValues[ SECTOR( gubPBSectorX, gubPBSectorY ) ].SoundTacticalTensor[gubPBSectorZ];
	if ( MusicSoundValues[ SECTOR( gubPBSectorX, gubPBSectorY ) ].SoundTacticalTensor[gubPBSectorZ] != -1 )
		SetMusicModeID( MUSIC_TACTICAL_ENEMYPRESENT, MusicSoundValues[ SECTOR( gubPBSectorX, gubPBSectorY ) ].SoundTacticalTensor[gubPBSectorZ] );
	else
#endif
	SetMusicMode( MUSIC_TACTICAL_ENEMYPRESENT );

#ifdef JA2UB	
	if ( gGameUBOptions.AutoResolve == FALSE )
		DisableButton( iPBButton[0] );
#endif
	DoTransitionFromMapscreenToPreBattleInterface();

	// clean up
	gubSpecialEncounterCodeForEnemyHeli = FALSE;
}

void DoTransitionFromMapscreenToPreBattleInterface()
{
	gfZoomDone = FALSE;

	SGPRect DstRect, PBIRect;
	UINT32 uiStartTime, uiCurrTime;
	INT32 iPercentage, iFactor;
	UINT32 uiTimeRange;
	INT16 sStartLeft, sEndLeft, sStartTop, sEndTop;
	INT32 iLeft, iTop, iWidth, iHeight;
	BOOLEAN fEnterAutoResolveMode = FALSE;

	if( !gfExtraBuffer )
		return;

	PauseTime( FALSE );

	PBIRect.iLeft = 0 + xResOffset;
	PBIRect.iTop = 0 + yResOffset;
	PBIRect.iRight = 261 + xResOffset;
	PBIRect.iBottom = 359 + yResOffset;
	iWidth = 261;
	iHeight = 359;

	uiTimeRange = 1000;
	iPercentage = 0;
	uiStartTime = GetJA2Clock();

	GetScreenXYFromMapXY( gubPBSectorX, gubPBSectorY, &sStartLeft, &sStartTop );
	sStartLeft += UI_MAP.GridSize.iX / 2;
	sStartTop += UI_MAP.GridSize.iY / 2;
	sEndLeft = 131 + xResOffset;
	sEndTop = 180 + yResOffset;

	//save the mapscreen buffer
	BlitBufferToBuffer( FRAME_BUFFER, guiEXTRABUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	if( gfEnterAutoResolveMode )
	{ //If we are intending on immediately entering autoresolve, change the global flag so that it will actually
		//render the interface once.	If gfEnterAutoResolveMode is clear, then RenderPreBattleInterface() won't do
		//anything.
		fEnterAutoResolveMode = TRUE;
		gfEnterAutoResolveMode = FALSE;
	}
	//render the prebattle interface
	RenderPreBattleInterface();

	gfIgnoreAllInput = TRUE;

	if( fEnterAutoResolveMode )
	{ //Change it back
		gfEnterAutoResolveMode = TRUE;
	}

	BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, 27 + xResOffset, 54 + yResOffset, 209, 32 );
	RenderButtons();
	BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 27 + xResOffset, 54 + yResOffset, 209, 32 );
	gfRenderPBInterface = TRUE;

	//hide the prebattle interface
	BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, 0 + xResOffset, 0 + yResOffset, 261 + xResOffset, 359 + yResOffset );
	PlayJA2SampleFromFile( "SOUNDS\\Laptop power up (8-11).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
	InvalidateScreen();
	RefreshScreen( NULL );

	while( iPercentage < 100	)
	{
		uiCurrTime = GetJA2Clock();
		iPercentage = (uiCurrTime-uiStartTime) * 100 / uiTimeRange;
		iPercentage = min( iPercentage, 100 );

		//Factor the percentage so that it is modified by a gravity falling acceleration effect.
		iFactor = (iPercentage - 50) * 2;
		if( iPercentage < 50 )
			iPercentage = (UINT32)(iPercentage + iPercentage * iFactor * 0.01 + 0.5);
		else
			iPercentage = (UINT32)(iPercentage + (100-iPercentage) * iFactor * 0.01 + 0.05);

		//Calculate the center point.
		iLeft = sStartLeft - (sStartLeft-sEndLeft+1) * iPercentage / 100;
		if( sStartTop > sEndTop )
			iTop = sStartTop - (sStartTop-sEndTop+1) * iPercentage / 100;
		else
			iTop = sStartTop + (sEndTop-sStartTop+1) * iPercentage / 100;

		DstRect.iLeft = iLeft - iWidth * iPercentage / 200;
		DstRect.iRight = DstRect.iLeft + max( iWidth * iPercentage / 100, 1 );
		DstRect.iTop = iTop - iHeight * iPercentage / 200;
		DstRect.iBottom = DstRect.iTop + max( iHeight * iPercentage / 100, 1 );

		BltStretchVideoSurface( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, 0, &PBIRect, &DstRect );

		InvalidateScreen();
		RefreshScreen( NULL );

		//Restore the previous rect.
		BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, (UINT16)DstRect.iLeft, (UINT16)DstRect.iTop,
			(UINT16)(DstRect.iRight-DstRect.iLeft+1), (UINT16)(DstRect.iBottom-DstRect.iTop+1) );
	}
	BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfZoomDone = TRUE;
}

void KillPreBattleInterface()
{
	if( !gfPreBattleInterfaceActive )
		return;

	fDisableMapInterfaceDueToBattle = FALSE;
	MSYS_RemoveRegion( &PBInterfaceBlanket );

	//The panel
	DeleteVideoObjectFromIndex( uiInterfaceImages );

	//The 3 buttons
	RemoveButton( iPBButton[0] );
	RemoveButton( iPBButton[1] );
	RemoveButton( iPBButton[2] );
	UnloadButtonImage( iPBButtonImage[0] );
	UnloadButtonImage( iPBButtonImage[1] );
	UnloadButtonImage( iPBButtonImage[2] );

	/*
	MSYS_RemoveRegion( &InvolvedRegion );
	if( guiNumUninvolved )
		MSYS_RemoveRegion( &UninvolvedRegion );
	*/

	gfPreBattleInterfaceActive = FALSE;

	//UpdateCharRegionHelpText( );

	// re draw affected regions
	fMapPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	gfDisplayPotentialRetreatPaths = FALSE;

	//Enable the options button when the auto resolve	screen comes up
	EnableDisAbleMapScreenOptionsButton( TRUE );

	ColorFillVideoSurfaceArea( guiSAVEBUFFER, 0, 0, 261 + xResOffset, 359 + yResOffset, 0 );

	EnableTeamInfoPanels();
	if( ButtonList[ giMapContractButton ] )
	{
		ShowButton( giMapContractButton );
	}
	if( ButtonList[ giCharInfoButton[ 0 ] ] )
	{
		ShowButton( giCharInfoButton[ 0 ] );
	}
	if( ButtonList[ giCharInfoButton[ 1 ] ] )
	{
		ShowButton( giCharInfoButton[ 1 ] );
	}
	gfPersistantPBI = FALSE; // If killing the PBI, it must not be persistant anymore!
}


void RenderPBHeader( INT32 *piX, INT32 *piWidth)
{
	CHAR16 str[100];
	INT32 x, width;
	SetFont( FONT10ARIALBOLD );
	if( gfBlinkHeader )
	{
		if( GetJA2Clock() % 1000 < 667 )
		{
			SetFontForeground( FONT_WHITE );
		}
		else
		{
			SetFontForeground( FONT_LTRED );
		}
	}
	else
	{
		SetFontForeground( FONT_BEIGE );
	}
	SetFontShadow( FONT_NEARBLACK );
	if( !gfPersistantPBI )
	{
		swprintf( str, gzNonPersistantPBIText[8] );
	}
	else switch( GetEnemyEncounterCode() )
	{
		case ENEMY_INVASION_CODE:
			swprintf( str, gpStrategicString[ STR_PB_ENEMYINVASION_HEADER ] );
			break;
		case ENEMY_ENCOUNTER_CODE:
		case TRANSPORT_INTERCEPT_CODE:
			swprintf( str, gpStrategicString[ STR_PB_ENEMYENCOUNTER_HEADER ] );
			break;
		case ENEMY_AMBUSH_CODE:
		case ENEMY_AMBUSH_DEPLOYMENT_CODE:
			swprintf( str, gpStrategicString[ STR_PB_ENEMYAMBUSH_HEADER ] );
			gfBlinkHeader = TRUE;
			break;
		case ENTERING_ENEMY_SECTOR_CODE:
			swprintf( str, gpStrategicString[ STR_PB_ENTERINGENEMYSECTOR_HEADER ] );
			break;
		case CREATURE_ATTACK_CODE:
			swprintf( str, gpStrategicString[ STR_PB_CREATUREATTACK_HEADER ] );
			gfBlinkHeader = TRUE;
			break;
		case BLOODCAT_AMBUSH_CODE:
			swprintf( str, gpStrategicString[ STR_PB_BLOODCATAMBUSH_HEADER ] );
			gfBlinkHeader = TRUE;
			break;
		case ENTERING_BLOODCAT_LAIR_CODE:
			swprintf( str, gpStrategicString[ STR_PB_ENTERINGBLOODCATLAIR_HEADER ] );
			break;
		case ENEMY_INVASION_AIRDROP_CODE:
			swprintf( str, gpStrategicString[STR_PB_ENEMYINVASION_AIRDROP_HEADER] );
			break;
		case CONCEALINSERTION_CODE:
			swprintf( str, gpStrategicString[STR_PB_ENEMYENCOUNTER_HEADER] );
			break;
		case BLOODCAT_ATTACK_CODE:
			swprintf( str, gpStrategicString[STR_PB_BLOODCATRAID_HEADER] );
			gfBlinkHeader = TRUE;
			break;
		case ZOMBIE_ATTACK_CODE:
			swprintf( str, gpStrategicString[STR_PB_ZOMBIERAID_HEADER] );
			gfBlinkHeader = TRUE;
			break;
		case BANDIT_ATTACK_CODE:
			swprintf( str, gpStrategicString[STR_PB_BANDITRAID_HEADER] );
			gfBlinkHeader = TRUE;
			break;
	}
	width = StringPixLength( str, FONT10ARIALBOLD );
	x = 130 - width / 2;
	mprintf( x + xResOffset, 4 + yResOffset, str );
	InvalidateRegion( 0, 0, 231 + xResOffset, 12 + yResOffset );
	*piX = x;
	*piWidth = width;
}

void RenderPreBattleInterface()
{
	GROUP *pGroup;
	HVOBJECT hVObject;
	INT32 i, x, y, line, width;
	CHAR16 str[100];
	CHAR16 pSectorName[ 128 ];
	UINT8 ubHPPercent, ubBPPercent;
	BOOLEAN fMouseInRetreatButtonArea;
	UINT8 ubJunk;
	//PLAYERGROUP *pPlayer;

	// Make the background black!
	//ColorFillVideoSurfaceArea( guiSAVEBUFFER, 0, 0, 261 + xResOffset, SCREEN_HEIGHT - 120, 0 );

	//This code determines if the cursor is inside the rectangle consisting of the
	//retreat button.	If it is inside, then we set up the variables so that the retreat
	//arrows get drawn in the mapscreen.
	if( ButtonList[ iPBButton[ 2 ] ]->uiFlags & BUTTON_ENABLED )
	{
		if( gusMouseXPos < gusRetreatButtonLeft || gusMouseXPos > gusRetreatButtonRight ||
				gusMouseYPos < gusRetreatButtonTop || gusMouseYPos > gusRetreatButtonBottom )
			fMouseInRetreatButtonArea = FALSE;
 		else
			fMouseInRetreatButtonArea = TRUE;
		if( fMouseInRetreatButtonArea != gfDisplayPotentialRetreatPaths )
		{
			gfDisplayPotentialRetreatPaths = fMouseInRetreatButtonArea;
			fMapPanelDirty = TRUE;
		}
	}

	if( gfRenderPBInterface )
	{
		// set font destinanation buffer to the save buffer
		SetFontDestBuffer( guiSAVEBUFFER , 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

		if( gfPBButtonsHidden )
		{
			ShowButton( iPBButton[0] );
			ShowButton( iPBButton[1] );
			ShowButton( iPBButton[2] );
			gfPBButtonsHidden = FALSE;
		}
		else
		{
			MarkAButtonDirty( iPBButton[ 0 ] );
			MarkAButtonDirty( iPBButton[ 1 ] );
			MarkAButtonDirty( iPBButton[ 2 ] );
		}

		gfRenderPBInterface = FALSE;
		GetVideoObject( &hVObject, uiInterfaceImages );
		//main panel
		BltVideoObject( guiSAVEBUFFER, hVObject, MAINPANEL, xResOffset, yResOffset, VO_BLT_SRCTRANSPARENCY, NULL );
		//main title

		RenderPBHeader( &x, &width );
		//now draw the title bars up to the text.
		for( i = x - 12; i > 20; i -= 10 )
		{
			BltVideoObject( guiSAVEBUFFER, hVObject, TITLE_BAR_PIECE, i + xResOffset, 6 + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		for( i = x + width + 2; i < 231; i += 10 )
		{
			BltVideoObject( guiSAVEBUFFER, hVObject, TITLE_BAR_PIECE, i + xResOffset, 6 + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_LINE, 0 + xResOffset, BOTTOM_Y + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL);
		BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_LINE, 0 + xResOffset, BOTTOM_Y + yResOffset + 10, VO_BLT_SRCTRANSPARENCY, NULL);
		BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_LINE, 0 + xResOffset, BOTTOM_Y + yResOffset + 20, VO_BLT_SRCTRANSPARENCY, NULL);
		BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_LINE, 0 + xResOffset, BOTTOM_Y + yResOffset + 30, VO_BLT_SRCTRANSPARENCY, NULL);
		//Draw the bottom edges
		for (i = 0; i < max(guiNumUninvolved, 1); i++)
		{
			y = BOTTOM_Y + ROW_HEIGHT * (i + 1) + 30;
			BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_LINE, 0 + xResOffset, y + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL);
		}
		BltVideoObject(guiSAVEBUFFER, hVObject, UNINVOLVED_HEADER, 8 + xResOffset, BOTTOM_Y + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL);
		BltVideoObject(guiSAVEBUFFER, hVObject, BOTTOM_END, 0 + xResOffset, BOTTOM_Y + yResOffset + 35 + ROW_HEIGHT * max(guiNumUninvolved, 1), VO_BLT_SRCTRANSPARENCY, NULL);

		SetFont( BLOCKFONT );
		SetFontForeground( FONT_BEIGE );
		swprintf( str, gpStrategicString[ STR_PB_LOCATION ] );
		width = StringPixLength( str, BLOCKFONT );
		if( width > 64 )
		{
			SetFont( BLOCKFONTNARROW );
			width = StringPixLength( str, BLOCKFONTNARROW );
		}
		mprintf( 65 - width + xResOffset , 17 + yResOffset, str );

		SetFont( BLOCKFONT );
		if( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE )
		{
			swprintf( str, gpStrategicString[STR_PB_CREATURES ] );
		}
		else if( GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE || 
			GetEnemyEncounterCode() == ENTERING_BLOODCAT_LAIR_CODE || 
			GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE )
		{
			swprintf( str, gpStrategicString[ STR_PB_BLOODCATS ] );
		}
		else if ( GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE )
		{
			swprintf( str, gpStrategicString[STR_PB_ZOMBIES] );
		}
		else if ( GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
		{
			swprintf( str, gpStrategicString[STR_PB_BANDITS] );
		}
		else
		{
			swprintf( str, gpStrategicString[ STR_PB_ENEMIES ] );
		}
		
		width = StringPixLength( str, BLOCKFONT );
		if( width > 52 )
		{
			SetFont( BLOCKFONTNARROW );
			width = StringPixLength( str, BLOCKFONTNARROW );
		}
		mprintf( 54 + xResOffset - width , 38 + yResOffset, str );

		SetFont( BLOCKFONT );
		swprintf( str, gpStrategicString[ STR_PB_MERCS ] );
		width = StringPixLength( str, BLOCKFONT );
		if( width > 52 )
		{
			SetFont( BLOCKFONTNARROW );
			width = StringPixLength( str, BLOCKFONTNARROW );
		}
		mprintf( 139 + xResOffset - width , 38 + yResOffset, str );

		SetFont( BLOCKFONT );
		swprintf( str, gpStrategicString[ STR_PB_MILITIA ] );
		width = StringPixLength( str, BLOCKFONT );
		if( width > 52 )
		{
			SetFont( BLOCKFONTNARROW );
			width = StringPixLength( str, BLOCKFONTNARROW );
		}
		mprintf( 224 + xResOffset - width , 38 + yResOffset, str );

		//Draw the bottom columns
		for( i = 0; i < (INT32)max( guiNumUninvolved, 1 ); i++ )
		{
			y = BOTTOM_Y + ROW_HEIGHT * (i+1) + 1 + ACTUAL_HEIGHT;
			BltVideoObject( guiSAVEBUFFER, hVObject, BOTTOM_COLUMN, 161 + xResOffset, y + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL );
		}

        // WDS - make number of mercenaries, etc. be configurable
		for( i = 0; i < (INT32)(25/*3+ OUR_TEAM_SIZE_NO_VEHICLE - max( guiNumUninvolved, 1 )*/); i++ )
		{
			y = TOP_Y + ROW_HEIGHT * i;
			BltVideoObject( guiSAVEBUFFER, hVObject, TOP_COLUMN, 186 + xResOffset, y + yResOffset, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		//location
		SetFont( FONT10ARIAL );
		SetFontForeground( FONT_YELLOW );
		SetFontShadow( FONT_NEARBLACK );

		GetSectorIDString( gubPBSectorX, gubPBSectorY, gubPBSectorZ, pSectorName, TRUE );
		mprintf( 70 + xResOffset, 17 + yResOffset, L"%s %s", gpStrategicString[ STR_PB_SECTOR ], pSectorName );

		//enemy
		SetFont( FONT14ARIAL );
		if( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE ||
			GetEnemyEncounterCode() == ENTERING_BLOODCAT_LAIR_CODE ||
			GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
			GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BANDIT_ATTACK_CODE ||
			WhatPlayerKnowsAboutEnemiesInSector( gubPBSectorX, gubPBSectorY ) < KNOWS_THEYRE_THERE_AND_HOW_MANY ) // HEADROCK HAM 5: New case above this one...
		{
			// don't know how many
			swprintf( str, L"?" );
			SectorInfo[ SECTOR( gubPBSectorX, gubPBSectorY ) ].bLastKnownEnemies = -2;
		}
		else
		{
			// know exactly how many
			i = NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, ENEMY_TEAM );
			swprintf( str, L"%d", i );
			SectorInfo[ SECTOR( gubPBSectorX, gubPBSectorY ) ].bLastKnownEnemies = (INT8)i;
		}
		x = 57 + (27 - StringPixLength( str, FONT14ARIAL )) / 2;
		y = 36;
		mprintf( x + xResOffset, y + yResOffset, str );
		//player
		swprintf( str, L"%d", guiNumInvolved );
		x = 142 + (27 - StringPixLength( str, FONT14ARIAL )) / 2;
		mprintf( x + xResOffset, y + yResOffset, str );
		//militia
		swprintf( str, L"%d", NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, MILITIA_TEAM ) );
		x = 227 + (27 - StringPixLength( str, FONT14ARIAL )) / 2;
		mprintf( x + xResOffset, y + yResOffset, str );
		SetFontShadow( FONT_NEARBLACK );

		SetFont( BLOCKFONT2 );
		SetFontForeground( FONT_YELLOW );

		//print out the participants of the battle.
		// |	NAME	| ASSIGN |	COND	|	HP	|	BP	|
		line = 0;
		y = TOP_Y + 1;
		for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
		{
			if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) )
			{
				if ( PlayerMercInvolvedInThisCombat( MercPtrs[ i ] ) )
				{ //involved
					if( line == giHilitedInvolved )
						SetFontForeground( FONT_WHITE );
					else
						SetFontForeground( FONT_YELLOW );
					//NAME
					wcscpy( str, MercPtrs[ i ]->name );
					x = 17 + (52-StringPixLength( str, BLOCKFONT2)) / 2;
					mprintf( x + xResOffset , y + yResOffset, str );
					//ASSIGN
					GetMapscreenMercAssignmentString( MercPtrs[ i ], str );
					x = 72 + (54-StringPixLength( str, BLOCKFONT2)) / 2;
					mprintf( x + xResOffset, y + yResOffset, str );
					//COND
					GetSoldierConditionInfo( MercPtrs[ i ], str, &ubHPPercent, &ubBPPercent );
					x = 129 + (58-StringPixLength( str, BLOCKFONT2)) / 2;
					mprintf( x + xResOffset, y + yResOffset, str );
					//HP
					swprintf( str, L"%d%%", ubHPPercent );
					x = 189 + (25-StringPixLength( str, BLOCKFONT2)) / 2;
					wcscat( str, sSpecialCharacters[0] );
					mprintf( x + xResOffset, y + yResOffset, str );
					//BP
					swprintf( str, L"%d%%", ubBPPercent );
					x = 217 + (25-StringPixLength( str, BLOCKFONT2)) / 2;
					wcscat( str, sSpecialCharacters[0] );
					mprintf( x + xResOffset, y + yResOffset, str );

					line++;
					y += ROW_HEIGHT;
				}
			}
		}

		//print out the uninvolved members of the battle
		// |	NAME	| ASSIGN |	LOC	|	DEST	|	DEP	|
		if( !guiNumUninvolved )
		{
			SetFontForeground( FONT_YELLOW );
			wcscpy( str, gpStrategicString[ STR_PB_NONE ] );
			x = 17 + (52-StringPixLength( str, BLOCKFONT2)) / 2;
			y = BOTTOM_Y + ROW_HEIGHT + 2 + ACTUAL_HEIGHT;
			mprintf( x + xResOffset, y + yResOffset, str );
		}
		else
		{			
			pGroup = gpGroupList;
			y = BOTTOM_Y + ROW_HEIGHT + 2 + ACTUAL_HEIGHT;
			for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
			{
				if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) )
				{
					if ( !PlayerMercInvolvedInThisCombat( MercPtrs[ i ] ) )
					{
						// uninvolved
						if( line == giHilitedUninvolved )
							SetFontForeground( FONT_WHITE );
						else
							SetFontForeground( FONT_YELLOW );
						//NAME
						wcscpy( str, MercPtrs[ i ]->name );
						x = 17 + (52-StringPixLength( str, BLOCKFONT2)) / 2;
						mprintf( x + xResOffset, y + yResOffset, str );
						//ASSIGN
						GetMapscreenMercAssignmentString( MercPtrs[ i ], str );
						x = 72 + (54-StringPixLength( str, BLOCKFONT2)) / 2;
						mprintf( x + xResOffset, y + yResOffset, str );
						//LOC
						GetMapscreenMercLocationString( MercPtrs[ i ], str );
						x = 128 + (33-StringPixLength( str, BLOCKFONT2)) / 2;
						mprintf( x + xResOffset, y + yResOffset, str );
						//DEST
						GetMapscreenMercDestinationString( MercPtrs[ i ], str );
						if( wcslen( str ) > 0 )
						{
							x = 164 + (41-StringPixLength( str, BLOCKFONT2)) / 2;
							mprintf( x + xResOffset, y + yResOffset, str );
						}
						//DEP
						GetMapscreenMercDepartureString( MercPtrs[ i ], str, &ubJunk );
						x = 208 + (34-StringPixLength( str, BLOCKFONT2)) / 2;
						mprintf( x + xResOffset, y + yResOffset, str );
						line++;
						y += ROW_HEIGHT;
					}
				}
			}
		}

		// mark any and ALL pop up boxes as altered
		MarkAllBoxesAsAltered( );

		if(!gfZoomDone)
			RestoreExternBackgroundRect( 0 + xResOffset, 0 + yResOffset, 261 + xResOffset, 359 + yResOffset );
		else if(!guiNumUninvolved)		
			RestoreExternBackgroundRect( 0 + xResOffset, 0 + yResOffset, 261 + xResOffset, 389 + yResOffset );
		else
			RestoreExternBackgroundRect( 0 + xResOffset, 0 + yResOffset, 261 + xResOffset, y + yResOffset );

		// restore font destinanation buffer to the frame buffer
		SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	}
	else if( gfBlinkHeader )
	{
		RenderPBHeader( &x, &width ); //the text is important enough to blink.
	}

	//InvalidateRegion( 0, 0, 261, 359 );
	if( gfEnterAutoResolveMode )
	{
		gfEnterAutoResolveMode = FALSE;
		EnterAutoResolveMode( gubPBSectorX, gubPBSectorY );
		//return;
	}

	gfIgnoreAllInput = FALSE;

}

void AutoResolveBattleCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( !gfIgnoreAllInput )
	{
		if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
		{
			#ifdef JA2TESTVERSION
				if( _KeyDown( ALT ) )
			#else
				if( _KeyDown( ALT ) && CHEATER_CHEAT_LEVEL() )
			#endif
				{
					if( !gfPersistantPBI )
					{
						return;
					}
					PlayJA2Sample( EXPLOSION_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					gStrategicStatus.usPlayerKills += NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, ENEMY_TEAM );
					EliminateAllEnemies( gubPBSectorX, gubPBSectorY );
					SetMusicMode( MUSIC_TACTICAL_VICTORY );
					btn->uiFlags &= ~BUTTON_CLICKED_ON;
					DrawButton( btn->IDNum );
					InvalidateRegion( btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY );
					ExecuteBaseDirtyRectQueue();
					EndFrameBufferRender( );
					RefreshScreen( NULL );
					KillPreBattleInterface();
					StopTimeCompression();
					SetMusicMode( MUSIC_TACTICAL_NOTHING );

					// Flugente: in any case, reset creature attack variables
					ResetCreatureAttackVariables();

					return;
				}
			gfEnterAutoResolveMode = TRUE;
		}
	}
}

void GoToSectorCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( !gfIgnoreAllInput )
	{
		if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
		{
#ifdef JA2TESTVERSION
			if( _KeyDown( ALT ) )
#else
			if( _KeyDown( ALT ) && CHEATER_CHEAT_LEVEL() )
#endif
			{
				if( !gfPersistantPBI )
				{
					return;
				}

				PlayJA2Sample( EXPLOSION_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				gStrategicStatus.usPlayerKills += NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, ENEMY_TEAM );
				EliminateAllEnemies( gubPBSectorX, gubPBSectorY );
				SetMusicMode( MUSIC_TACTICAL_VICTORY );
				btn->uiFlags &= ~BUTTON_CLICKED_ON;
				DrawButton( btn->IDNum );
				InvalidateRegion( btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY );
				ExecuteBaseDirtyRectQueue();
				EndFrameBufferRender( );
				RefreshScreen( NULL );
				KillPreBattleInterface();
				StopTimeCompression();
				SetMusicMode( MUSIC_TACTICAL_NOTHING );

				// Flugente: in any case, reset creature attack variables
				ResetCreatureAttackVariables();

				return;
			}

			if ( gfPersistantPBI && gpBattleGroup && gpBattleGroup->usGroupTeam == OUR_TEAM &&
				GetEnemyEncounterCode() != ENEMY_AMBUSH_CODE &&
				GetEnemyEncounterCode() != CREATURE_ATTACK_CODE &&
				GetEnemyEncounterCode() != BLOODCAT_AMBUSH_CODE &&
				GetEnemyEncounterCode() != ENEMY_INVASION_AIRDROP_CODE &&
				GetEnemyEncounterCode() != CONCEALINSERTION_CODE &&
				GetEnemyEncounterCode() != BLOODCAT_ATTACK_CODE &&
				GetEnemyEncounterCode() != ZOMBIE_ATTACK_CODE &&
				GetEnemyEncounterCode() != BANDIT_ATTACK_CODE )
			{
				gfEnterTacticalPlacementGUI = TRUE;
			}

			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			DrawButton( btn->IDNum );
			InvalidateRegion( btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY );
			ExecuteBaseDirtyRectQueue();
			EndFrameBufferRender( );
			RefreshScreen( NULL );
			if( gubPBSectorX == gWorldSectorX && gubPBSectorY == gWorldSectorY && !gbWorldSectorZ )
			{
				gfGotoSectorTransition = TRUE;
			}

			// first time going to the sector?
			if( gfPersistantPBI )
			{
				// put everyone on duty, and remove mercs from vehicles, too
				PutNonSquadMercsInBattleSectorOnSquads( TRUE );

				// we nuke the groups existing route & destination in advance
				ClearMovementForAllInvolvedPlayerGroups( );
			}
			else
			{ //Clear the battlegroup pointer.
				gpBattleGroup = NULL;
			}

			// must come AFTER anything that needs gpBattleGroup, as it wipes it out
			SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );

			KillPreBattleInterface();
			SetTacticalInterfaceFlags( 0 );
		}
	}
}

void RetreatMercsCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( !gfIgnoreAllInput )
	{
		if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
		{
			/////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records - times retreated counter
			for( UINT8 i = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; i <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; i++ )
			{
				if ( MercPtrs[i]->bActive && MercPtrs[i]->stats.bLife >= OKLIFE )
				{
					if ( PlayerMercInvolvedInThisCombat( MercPtrs[i] ) && MercPtrs[i]->ubProfile != NO_PROFILE )
						gMercProfiles[ MercPtrs[i]->ubProfile ].records.usBattlesRetreated++;
				}
			}
			/////////////////////////////////////////////////////////////////////////////////

			// get them outta here!
			RetreatAllInvolvedPlayerGroups();

			// NOTE: this code assumes you can never retreat while underground
			HandleLoyaltyImplicationsOfMercRetreat( RETREAT_PBI, gubPBSectorX, gubPBSectorY, 0 );

			if ( NumNonPlayerTeamMembersInSector( gubPBSectorX, gubPBSectorY, MILITIA_TEAM ) )
			{
				//Mercs retreat, but enemies still need to fight the militia
				gfEnterAutoResolveMode = TRUE;
				return;
			}

			//Warp time by 5 minutes so that player can't just go back into the sector he left.
			//WarpGameTime( 300, WARPTIME_NO_PROCESSING_OF_EVENTS );
			ResetMovementForNonPlayerGroupsInLocation( gubPBSectorX, gubPBSectorY );

			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			DrawButton( btn->IDNum );
			InvalidateRegion( btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY );
			ExecuteBaseDirtyRectQueue();
			EndFrameBufferRender( );
			RefreshScreen( NULL );
			KillPreBattleInterface();
			StopTimeCompression();
			gpBattleGroup = NULL;
			gfBlitBattleSectorLocator = FALSE;

			SetMusicMode( MUSIC_TACTICAL_NOTHING );
		}
	}
}

enum
{
	COND_EXCELLENT,
	COND_GOOD,
	COND_FAIR,
	COND_WOUNDED,
	COND_FATIGUED,
	COND_BLEEDING,
	COND_UNCONCIOUS,
	COND_DYING,
	COND_DEAD
};

void GetSoldierConditionInfo( SOLDIERTYPE *pSoldier, STR16 szCondition, UINT8 *pubHPPercent, UINT8 *pubBPPercent )
{
	Assert( pSoldier );
	*pubHPPercent = (UINT8)(pSoldier->stats.bLife * 100 / pSoldier->stats.bLifeMax);
	*pubBPPercent = pSoldier->bBreath;
	//Go from the worst condition to the best.
	if( !pSoldier->stats.bLife )
	{ //0 life
		swprintf( szCondition, pConditionStrings[ COND_DEAD ] );
	}
	else if( pSoldier->stats.bLife < OKLIFE && pSoldier->bBleeding )
	{ //life less than OKLIFE and bleeding
		swprintf( szCondition, pConditionStrings[ COND_DYING ] );
	}
	else if( pSoldier->bBreath < OKBREATH && pSoldier->bCollapsed )
	{ //breath less than OKBREATH
		swprintf( szCondition, pConditionStrings[ COND_UNCONCIOUS ] );
	}
	else if( pSoldier->bBleeding > MIN_BLEEDING_THRESHOLD)
	{ //bleeding
		swprintf( szCondition, pConditionStrings[ COND_BLEEDING ] );
	}
	else if( pSoldier->stats.bLife*100 < pSoldier->stats.bLifeMax*50 )
	{ //less than 50% life
		swprintf( szCondition, pConditionStrings[ COND_WOUNDED ] );
	}
	else if( pSoldier->bBreath < 50 )
	{ //breath less than half
		swprintf( szCondition, pConditionStrings[ COND_FATIGUED ] );
	}
	else if( pSoldier->stats.bLife*100 < pSoldier->stats.bLifeMax*67 )
	{ //less than 67% life
		swprintf( szCondition, pConditionStrings[ COND_FAIR ] );
	}
	else if( pSoldier->stats.bLife*100 < pSoldier->stats.bLifeMax*86 )
	{ //less than 86% life
		swprintf( szCondition, pConditionStrings[ COND_GOOD ] );
	}
	else
	{ //86%+ life
		swprintf( szCondition, pConditionStrings[ COND_EXCELLENT ] );
	}
}

/*
void InvolvedMoveCallback( MOUSE_REGION *reg, INT32 reason )
{
	gfRenderPBInterface = TRUE;
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHilitedInvolved = giHilitedUninvolved = -1;
		return;
	}
	giHilitedInvolved = reg->RelativeYPos / 10;
	giHilitedUninvolved = -1;
}

void InvolvedClickCallback( MOUSE_REGION *reg, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		SOLDIERTYPE *pSoldier;
		INT16 y;
		pSoldier = InvolvedSoldier( giHilitedInvolved );
		if( !pSoldier )
			return;
		y = (INT16)(reg->RegionTopLeftY + giHilitedUninvolved * ROW_HEIGHT + 5);
		if( y + 102 >= 360 )
			y -= 102;
		if( gusMouseXPos >= 76 && gusMouseXPos <= 129 )
			ActivateSoldierPopup( pSoldier, ASSIGNMENT_POPUP, 102, y );
		gfRenderPBInterface = TRUE;
	}
}

void UninvolvedMoveCallback( MOUSE_REGION *reg, INT32 reason )
{
	gfRenderPBInterface = TRUE;
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHilitedInvolved = giHilitedUninvolved = -1;
		return;
	}
	giHilitedUninvolved = reg->RelativeYPos / 10;
	giHilitedInvolved = -1;
}

void UninvolvedClickCallback( MOUSE_REGION *reg, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		SOLDIERTYPE *pSoldier;
		INT16 y;
		pSoldier = UninvolvedSoldier( giHilitedUninvolved );
		if( !pSoldier )
			return;
		y = (INT16)(reg->RegionTopLeftY + giHilitedUninvolved * ROW_HEIGHT + 5);
		if( y + 102 >= 360 )
			y -= 102;
		if( gusMouseXPos >= 76 && gusMouseXPos <= 129 )
		{
			ActivateSoldierPopup( pSoldier, ASSIGNMENT_POPUP, 102, y );
		}
		else if( gusMouseXPos >= 169 && gusMouseXPos <= 204 )
		{
			ActivateSoldierPopup( pSoldier, DESTINATION_POPUP, 186, y );
		}
		else if( gusMouseXPos >= 208 && gusMouseXPos <= 236 )
		{
			ActivateSoldierPopup( pSoldier, CONTRACT_POPUP, 172, y );
		}
		gfRenderPBInterface = TRUE;
	}
}

SOLDIERTYPE* InvolvedSoldier( INT32 index )
{
	GROUP *pGroup;
	PLAYERGROUP *pPlayer=NULL;
	BOOLEAN fFound = FALSE;
	if( index < 0 || index > 19 )
		return NULL;
	pGroup = gpGroupList;
	while( pGroup && !fFound )
	{
		if ( PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			pPlayer = pGroup->pPlayerList;
			while( pPlayer )
			{
				index--;
				if( index <= 0 )
				{
					fFound = TRUE;
					break;
				}
				pPlayer = pPlayer->next;
			}
		}
		pGroup = pGroup->next;
	}
	if( !fFound )
		return NULL;
	return pPlayer->pSoldier;
}

SOLDIERTYPE* UninvolvedSoldier( INT32 index )
{
	GROUP *pGroup;
	PLAYERGROUP *pPlayer=NULL;
	BOOLEAN fFound = FALSE;
	if( index < 0 || index > 19 )
		return NULL;
	pGroup = gpGroupList;
	while( pGroup && !fFound )
	{
		if ( pGroup->usGroupTeam == OUR_TEAM && !PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			pPlayer = pGroup->pPlayerList;
			while( pPlayer )
			{
				index--;
				if( index <= 0 )
				{
					fFound = TRUE;
					break;
				}
				pPlayer = pPlayer->next;
			}
		}
		pGroup = pGroup->next;
	}
	if( !fFound )
		return NULL;
	return pPlayer->pSoldier;
}
*/



void ActivatePreBattleAutoresolveAction()
{
	if( ButtonList[ iPBButton[ 0 ] ]->uiFlags & BUTTON_ENABLED )
	{ //Feign call the autoresolve button using the callback
		AutoResolveBattleCallback( ButtonList[ iPBButton[0] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
	}
}

void ActivatePreBattleEnterSectorAction()
{
	if( ButtonList[ iPBButton[ 1 ] ]->uiFlags & BUTTON_ENABLED )
	{ //Feign call the enter sector button using the callback
		GoToSectorCallback( ButtonList[ iPBButton[1] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
	}
}

void ActivatePreBattleRetreatAction()
{
	if( ButtonList[ iPBButton[ 2 ] ]->uiFlags & BUTTON_ENABLED )
	{ //Feign call the retreat button using the callback
		RetreatMercsCallback( ButtonList[ iPBButton[2] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
	}
}

void ActivateAutomaticAutoResolveStart()
{
	ButtonList[ iPBButton[0] ]->uiFlags |= BUTTON_CLICKED_ON;
	gfIgnoreAllInput = FALSE;
	AutoResolveBattleCallback( ButtonList[ iPBButton[0] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
}

void CalculateNonPersistantPBIInfo()
{
	//We need to set up the non-persistant PBI
	if( !gfBlitBattleSectorLocator ||
			gubPBSectorX != gWorldSectorX || gubPBSectorY != gWorldSectorY || gubPBSectorZ != gbWorldSectorZ )
	{ //Either the locator isn't on or the locator info is in a different sector

		//Calculated the encounter type
		SetEnemyEncounterCode( NO_ENCOUNTER_CODE );
		SetExplicitEnemyEncounterCode( NO_ENCOUNTER_CODE );

		if( HostileCiviliansPresent() )
		{
			//There are hostile civilians, so no autoresolve allowed.
			SetExplicitEnemyEncounterCode( HOSTILE_CIVILIANS_CODE );
		}
		else if( HostileBloodcatsPresent() )
		{
			//There are bloodcats in the sector, so no autoresolve allowed
			SetExplicitEnemyEncounterCode( HOSTILE_BLOODCATS_CODE );
		}
		else if( gbWorldSectorZ > 0 )
		{
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			Assert( pSector );

			if( pSector->ubCreaturesInBattle )
			{
				SetExplicitEnemyEncounterCode( FIGHTING_CREATURES_CODE );
			}
			else if( pSector->ubAdminsInBattle || pSector->ubTroopsInBattle || pSector->ubElitesInBattle || pSector->ubTanksInBattle || pSector->ubJeepsInBattle || pSector->ubRobotsInBattle )
			{
				SetExplicitEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE );
				SetEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE );
			}
		}
		else
		{
			SECTORINFO *pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
			Assert( pSector );

			if( pSector->ubCreaturesInBattle )
			{
				SetExplicitEnemyEncounterCode( FIGHTING_CREATURES_CODE );
			}
			else if( pSector->ubAdminsInBattle || pSector->ubTroopsInBattle || pSector->ubElitesInBattle || pSector->ubTanksInBattle || pSector->ubJeepsInBattle || pSector->ubRobotsInBattle )
			{
				SetExplicitEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE );
				SetEnemyEncounterCode( ENTERING_ENEMY_SECTOR_CODE );
			}
		}

		if ( GetExplicitEnemyEncounterCode() != NO_ENCOUNTER_CODE )
		{
			//Set up the location as well as turning on the blit flag.
			gubPBSectorX = (UINT8)gWorldSectorX;
			gubPBSectorY = (UINT8)gWorldSectorY;
			gubPBSectorZ = (UINT8)gbWorldSectorZ;
			gfBlitBattleSectorLocator = TRUE;
		}
	}
}

void ClearNonPersistantPBIInfo()
{
	gfBlitBattleSectorLocator = FALSE;
}



void PutNonSquadMercsInBattleSectorOnSquads( BOOLEAN fExitVehicles )
{
	GROUP *pGroup, *pNextGroup;

	// IMPORTANT: Have to do this by group, so everyone inside vehicles gets assigned to the same squad.	Needed for
	// the tactical placement interface to work in case of simultaneous multi-vehicle arrivals!

	pGroup = gpGroupList;
	while( pGroup )
	{
		// store ptr to next group in list, temporary groups will get deallocated as soon as the merc in it is put on a squad!
		pNextGroup = pGroup->next;

		if ( PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			// the helicopter group CAN be involved, if it's on the ground, in which case everybody must get out of it
			if ( IsGroupTheHelicopterGroup( pGroup ) )
			{
				// only happens if chopper is on the ground...
				Assert( !fHelicopterIsAirBorne );

				// put anyone in it into movement group
				MoveAllInHelicopterToFootMovementGroup( );
			}
			else
			{
				PutNonSquadMercsInPlayerGroupOnSquads( pGroup, fExitVehicles );
			}
		}

		pGroup = pNextGroup;
	}
}


void PutNonSquadMercsInPlayerGroupOnSquads( GROUP *pGroup, BOOLEAN fExitVehicles )
{
	PLAYERGROUP *pPlayer, *pNextPlayer;
	SOLDIERTYPE *pSoldier;
	INT8 bUniqueVehicleSquad = -1;
	BOOLEAN fSuccess = FALSE;


	if ( pGroup->fVehicle )
	{
		// put these guys on their own squad (we need to return their group ID, and can only return one, so they need a unique one
		bUniqueVehicleSquad = GetFirstEmptySquad();
		if ( bUniqueVehicleSquad == -1 )
		{
			return;
		}
	}


	pPlayer = pGroup->pPlayerList;

	while( pPlayer )
	{
		pSoldier = pPlayer->pSoldier;
		Assert( pSoldier );

		// store ptr to next soldier in group, once removed from group, his info will get memfree'd!
		pNextPlayer = pPlayer->next;

		if ( pSoldier->bActive && pSoldier->stats.bLife && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			// if involved, but off-duty (includes mercs inside vehicles!)
			if ( PlayerMercInvolvedInThisCombat( pSoldier ) && ( pSoldier->bAssignment >= ON_DUTY ) )
			{
				// if in a vehicle, pull him out
				if( pSoldier->bAssignment == VEHICLE )
				{
					if ( fExitVehicles )
					{
						TakeSoldierOutOfVehicle( pSoldier );

						// put them on the unique squad assigned to people leaving this vehicle.	Can't add them to existing squads,
						// because if this is a simultaneous group attack, the mercs could be coming from different sides, and the
						// placement screen can't handle mercs on the same squad arriving from difference edges!
						fSuccess = AddCharacterToSquad( pSoldier, bUniqueVehicleSquad );
						// if we failed, create another squad
						if (!fSuccess)
						{
							bUniqueVehicleSquad = GetFirstEmptySquad();
							if (bUniqueVehicleSquad != -1)
							{
								fSuccess = AddCharacterToSquad(pSoldier, bUniqueVehicleSquad);
							}
						}
					}
					//CHRISL: So what's supposed to happen in the merc is assigned to a vehicle but fExitVehicles is FALSE?
					else
						fSuccess = TRUE;
				}
				else
				{
					// add him to ANY on duty foot squad
					fSuccess = AddCharacterToAnySquad( pSoldier );
				}

				// it better work...
				Assert( fSuccess );

				// clear any desired squad assignments
				pSoldier->ubNumTraversalsAllowedToMerge = 0;
				pSoldier->ubDesiredSquadAssignment = NO_ASSIGNMENT;

				// stand him up
				MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );
			}
		}

		pPlayer = pNextPlayer;
	}
}


void WakeUpAllMercsInSectorUnderAttack( void )
{
	INT32 iCounter = 0, iNumberOfMercsOnTeam = 0;
	SOLDIERTYPE *pSoldier = NULL;


	// get number of possible grunts on team
	iNumberOfMercsOnTeam = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// any mercs not on duty should be added to the first avail squad
	for( iCounter = 0; iCounter < iNumberOfMercsOnTeam; iCounter++ )
	{
		pSoldier = &( Menptr[ iCounter ] );

		if ( pSoldier->bActive && pSoldier->stats.bLife && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			// if involved, but asleep
			if ( PlayerMercInvolvedInThisCombat( pSoldier ) && ( pSoldier->flags.fMercAsleep == TRUE ) )
			{
				// FORCE him wake him up
				SetMercAwake( pSoldier, FALSE, TRUE );
			}
		}
	}
}


// we are entering the sector, clear out all mvt orders for grunts
void ClearMovementForAllInvolvedPlayerGroups( void )
{
	GROUP *pGroup;

	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			// clear their strategic movement (mercpaths and waypoints)
			ClearMercPathsAndWaypointsForAllInGroup( pGroup );
		}
		pGroup = pGroup->next;
	}
}

void RetreatAllInvolvedPlayerGroups( void )
{
	GROUP *pGroup;

	// make sure guys stop their off duty assignments, like militia training!
	// but don't exit vehicles - drive off in them!
	PutNonSquadMercsInBattleSectorOnSquads( FALSE );

	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			// don't retreat empty vehicle groups!
			if( !pGroup->fVehicle || ( pGroup->fVehicle && DoesVehicleGroupHaveAnyPassengers( pGroup ) ) )
			{
				ClearMercPathsAndWaypointsForAllInGroup( pGroup );
				RetreatGroupToPreviousSector( pGroup );
			}
		}
		pGroup = pGroup->next;
	}
}

void RetreatAllInvolvedMilitiaGroups()
{
	// first, determine in which sector we are
	// pick the non-hostile-occupied, reachable adjacent sector with the shortest travel time
	INT16 sBattleSectorX, sBattleSectorY, sBattleSectorZ;
	if ( !GetCurrentBattleSectorXYZ( &sBattleSectorX, &sBattleSectorY, &sBattleSectorZ ) )
	{
		// well... this sucks. We are in retreat code, but no battle is on... abort this
		return;
	}

	// as we move the militia to another sector, anything that modifies them has to be done now - RemoveAutoResolveInterface is too late
	// as we don't want to make the location of that dependend on some state, we use a trick:
	// we drop their gear here, and promote them right now. They can then be properly deleted later on.
	AutoResolveMilitiaDropAndPromote();
	
	// dissolve all militia groups here
	INT16 newX = 0;
	INT16 newY = 0;
	BOOLEAN found = FALSE;
	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( MilitiaGroupInvolvedInThisCombat( pGroup ) )
		{
			// if a group's previous sector was different, pick it
			if ( pGroup->ubPrevX != 0 && pGroup->ubPrevY != 0 && ( pGroup->ubPrevX != pGroup->ubSectorX || pGroup->ubPrevY != pGroup->ubSectorY ) )
			{
				newX = pGroup->ubPrevX;
				newY = pGroup->ubPrevY;
				found = TRUE;
			}

			GROUP* pDeleteGroup = pGroup;
			pGroup = pGroup->next;

			DissolveMilitiaGroup( pDeleteGroup->ubGroupID );
		}
		else
			pGroup = pGroup->next;
	}

	// create new group and instantly place it in new sector
	SECTORINFO *pSector = &SectorInfo[SECTOR( sBattleSectorX, sBattleSectorY )];

	if ( !pSector )
		return;

	// as group size will be cut to MAX_STRATEGIC_ENEMY_GROUP_SIZE, see how many troops are allowed and reduce sector count accordingly
	UINT8 greens	= pSector->ubNumberOfCivsAtLevel[0];
	UINT8 regulars  = pSector->ubNumberOfCivsAtLevel[1];
	UINT8 elites	= pSector->ubNumberOfCivsAtLevel[2];

	pGroup = CreateNewMilitiaGroupDepartingFromSector( SECTOR( sBattleSectorX, sBattleSectorY ), greens, regulars, elites );

	pSector->ubNumberOfCivsAtLevel[0] -= greens;
	pSector->ubNumberOfCivsAtLevel[1] -= regulars;
	pSector->ubNumberOfCivsAtLevel[2] -= elites;

	// if we haven't found a good direction yet, we have to pick the best adjacent sector we can find
	if ( !found )
	{
		INT32 besttime = 0xffffffff;
		UINT8 usDirection = NORTH_STRATEGIC_MOVE;
		for ( UINT8 i = 0; i < 4; ++i )
		{
			INT16 loopX = sBattleSectorX;
			INT16 loopY = sBattleSectorY;

			if ( i == 0 )
			{
				++loopY;
				usDirection = SOUTH_STRATEGIC_MOVE;
			}
			else if ( i == 1 )
			{
				++loopX;
				usDirection = EAST_STRATEGIC_MOVE;
			}
			else if ( i == 2 )
			{
				--loopY;
				usDirection = NORTH_STRATEGIC_MOVE;
			}
			else if ( i == 3 )
			{
				--loopX;
				usDirection = WEST_STRATEGIC_MOVE;
			}

			if ( loopX < 1 || loopX >= MAP_WORLD_X - 1 || loopY < 1 || loopY >= MAP_WORLD_Y - 1 )
				continue;

			if ( SectorIsImpassable( SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), usDirection ) )
				continue;

			// don't retreat into an occupied sector!
			if ( NumNonPlayerTeamMembersInSector( loopX, loopY, ENEMY_TEAM ) > 0 )
				continue;

			INT32 uiTraverseTime = GetSectorMvtTimeForGroup( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), usDirection, pGroup );
			if ( uiTraverseTime < besttime )
			{
				besttime = uiTraverseTime;
				newX = loopX;
				newY = loopY;
				found = TRUE;
			}
		}
	}

	// if we found a good direction, move the group
	if ( found )
	{
		// while militia groups travel, they are not between sectors. As a result, when we retreat a militia group, we have to move them to the next sector immediately
		// do NOT check for a new battle - when calling this function, we are likely still in autoresolve, so this would cause hiccups
		PlaceGroupInSector( pGroup->ubGroupID, pGroup->ubSectorX, pGroup->ubSectorY, newX, newY, 0, FALSE );
	}

	// delete group afterwards, if it hasn't been deleted already - the militia will become static
	if ( pGroup )
		DissolveMilitiaGroup( pGroup->ubGroupID );
}


BOOLEAN PlayerMercInvolvedInThisCombat( SOLDIERTYPE *pSoldier )
{
	Assert( pSoldier );
	Assert( pSoldier->bActive );

	if( !pSoldier->flags.fBetweenSectors &&
			pSoldier->bAssignment != IN_TRANSIT &&
			pSoldier->bAssignment != ASSIGNMENT_POW &&
			pSoldier->bAssignment != ASSIGNMENT_DEAD &&
			pSoldier->bAssignment != ASSIGNMENT_MINIEVENT &&
			pSoldier->bAssignment != ASSIGNMENT_REBELCOMMAND &&
			!(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) &&
			// Robot is involved if it has a valid controller with it, uninvolved otherwise
			( !AM_A_ROBOT( pSoldier ) || ( pSoldier->ubRobotRemoteHolderID != NOBODY ) ) &&
			!SoldierAboardAirborneHeli( pSoldier ) )
	{
		if ( CurrentBattleSectorIs( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
		{
			// involved
			return( TRUE );
		}
	}

	// not involved
	return( FALSE );
}


BOOLEAN PlayerGroupInvolvedInThisCombat( GROUP *pGroup )
{
	Assert( pGroup );

	// player group, non-empty, not between sectors, in the right sector, isn't a group of in transit, dead, or POW mercs,
	// and either not the helicopter group, or the heli is on the ground
	if ( pGroup->usGroupTeam == OUR_TEAM && pGroup->ubGroupSize &&
			!pGroup->fBetweenSectors &&
			!GroupHasInTransitDeadOrPOWMercs( pGroup ) &&
			( !IsGroupTheHelicopterGroup( pGroup ) ||	!fHelicopterIsAirBorne ) )
	{
		if ( CurrentBattleSectorIs( pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubSectorZ ) )
		{
			// involved
			return( TRUE );
		}
	}

	// not involved
	return( FALSE );
}

BOOLEAN MilitiaGroupInvolvedInThisCombat( GROUP *pGroup )
{
	Assert( pGroup );

	// player group, non-empty, not between sectors, in the right sector, isn't a group of in transit, dead, or POW mercs,
	// and either not the helicopter group, or the heli is on the ground
	if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubGroupSize )
	{
		if ( CurrentBattleSectorIs( pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubSectorZ ) )
		{
			// involved
			return(TRUE);
		}
	}

	// not involved
	return(FALSE);
}


BOOLEAN CurrentBattleSectorIs( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	INT16 sBattleSectorX, sBattleSectorY, sBattleSectorZ;
	BOOLEAN fSuccess;

	fSuccess = GetCurrentBattleSectorXYZ( &sBattleSectorX, &sBattleSectorY, &sBattleSectorZ );
	Assert( fSuccess );

	if ( ( sSectorX == sBattleSectorX ) && ( sSectorY == sBattleSectorY ) && ( sSectorZ == sBattleSectorZ ) )
	{
		// yup!
		return( TRUE );
	}
	else
	{
		// wrong sector, no battle here
		return( FALSE );
	}
}



void CheckForRobotAndIfItsControlled( void )
{
	INT32 i;

	// search for the robot on player's team
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && AM_A_ROBOT( MercPtrs[ i ] ))
		{
			// check whether it has a valid controller with it. This sets its ubRobotRemoteHolderID field.
			MercPtrs[ i ]->UpdateRobotControllerGivenRobot( );

			// if he has a controller, set controllers
			if ( MercPtrs[ i ]->ubRobotRemoteHolderID != NOBODY )
			{
				MercPtrs[ MercPtrs[ i ]->ubRobotRemoteHolderID ]->UpdateRobotControllerGivenController( );
			}

			break;
		}
	}
}

void LogBattleResults( UINT8 ubVictoryCode)
{
	INT16 sSectorX, sSectorY, sSectorZ;
	GetCurrentBattleSectorXYZ( &sSectorX, &sSectorY, &sSectorZ );
	if( ubVictoryCode == LOG_VICTORY )
	{
		switch( GetEnemyEncounterCode() )
		{
			case ENEMY_INVASION_CODE:
			case BANDIT_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_DEFENDEDTOWNSECTOR, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENEMY_ENCOUNTER_CODE:
				AddHistoryToPlayersLog( HISTORY_WONBATTLE, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENEMY_AMBUSH_CODE:
			case ENEMY_AMBUSH_DEPLOYMENT_CODE:
			case ENEMY_INVASION_AIRDROP_CODE:
				AddHistoryToPlayersLog( HISTORY_WIPEDOUTENEMYAMBUSH, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENTERING_ENEMY_SECTOR_CODE:
				AddHistoryToPlayersLog( HISTORY_SUCCESSFULATTACK, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case CREATURE_ATTACK_CODE:
			case ZOMBIE_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_CREATURESATTACKED, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case BLOODCAT_AMBUSH_CODE:
			case ENTERING_BLOODCAT_LAIR_CODE:
			case BLOODCAT_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_SLAUGHTEREDBLOODCATS, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case CONCEALINSERTION_CODE:
				break;
			case TRANSPORT_INTERCEPT_CODE:
				AddHistoryToPlayersLog( HISTORY_INTERCEPTED_TRANSPORT_GROUP, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				NotifyTransportGroupDefeated();
				break;
		}

		// Flugente: campaign stats
		gCurrentIncident.usIncidentFlags |= INCIDENT_WIN;
	}
	else
	{
		switch( GetEnemyEncounterCode() )
		{
			case ENEMY_INVASION_CODE:
			case BANDIT_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_LOSTTOWNSECTOR, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENEMY_ENCOUNTER_CODE:
			case TRANSPORT_INTERCEPT_CODE:
				AddHistoryToPlayersLog( HISTORY_LOSTBATTLE, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENEMY_AMBUSH_CODE:
			case ENEMY_AMBUSH_DEPLOYMENT_CODE:
			case ENEMY_INVASION_AIRDROP_CODE:
				AddHistoryToPlayersLog( HISTORY_FATALAMBUSH, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case ENTERING_ENEMY_SECTOR_CODE:
				AddHistoryToPlayersLog( HISTORY_UNSUCCESSFULATTACK, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case CREATURE_ATTACK_CODE:
			case ZOMBIE_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_CREATURESATTACKED, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case BLOODCAT_AMBUSH_CODE:
			case ENTERING_BLOODCAT_LAIR_CODE:
			case BLOODCAT_ATTACK_CODE:
				AddHistoryToPlayersLog( HISTORY_KILLEDBYBLOODCATS, 0, GetWorldTotalMin(), sSectorX, sSectorY );
				break;
			case CONCEALINSERTION_CODE:
				break;
		}
	}

	switch( GetEnemyEncounterCode() )
	{
		case ENEMY_INVASION_CODE:
		case BLOODCAT_ATTACK_CODE:
		case ZOMBIE_ATTACK_CODE:
		case BANDIT_ATTACK_CODE:
			gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACK_ENEMY;
			break;
		case ENEMY_ENCOUNTER_CODE:
		case TRANSPORT_INTERCEPT_CODE:
		case ENTERING_ENEMY_SECTOR_CODE:
		case CREATURE_ATTACK_CODE:
		case ENTERING_BLOODCAT_LAIR_CODE:
		case FIGHTING_CREATURES_CODE:
		case HOSTILE_CIVILIANS_CODE:
		case HOSTILE_BLOODCATS_CODE:
			gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACK_PLAYERSIDE;
			break;
		case ENEMY_AMBUSH_CODE:
		case BLOODCAT_AMBUSH_CODE:
		case ENEMY_AMBUSH_DEPLOYMENT_CODE:
		case ENEMY_INVASION_AIRDROP_CODE:
			gCurrentIncident.usIncidentFlags |= (INCIDENT_ATTACK_ENEMY|INCIDENT_AMBUSH);
			break;
	}

	// Flugente: if battle has ended, close this incident, add it to the global incidents, and clear it
	FinishIncident(sSectorX, sSectorY, (INT8)sSectorZ);
}

void HandlePreBattleInterfaceStates()
{
	if( gfEnteringMapScreenToEnterPreBattleInterface && !gfEnteringMapScreen )
	{
		gfEnteringMapScreenToEnterPreBattleInterface = FALSE;
		if( !gfUsePersistantPBI )
		{
			InitPreBattleInterface( NULL, FALSE );
			gfUsePersistantPBI = TRUE;
		}
		else
		{
			InitPreBattleInterface( gpBattleGroup, TRUE );
		}
	}
	else if( gfDelayAutoResolveStart && gfPreBattleInterfaceActive )
	{
		gfDelayAutoResolveStart = FALSE;
		gfAutomaticallyStartAutoResolve = TRUE;
	}
	else if( gfAutomaticallyStartAutoResolve )
	{
		gfAutomaticallyStartAutoResolve = FALSE;
		ActivateAutomaticAutoResolveStart();
	}
	else if( gfTransitionMapscreenToAutoResolve )
	{
		gfTransitionMapscreenToAutoResolve = FALSE;
	}
}
